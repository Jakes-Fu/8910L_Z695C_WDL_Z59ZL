/*****************************************************************************
** File Name:      mmicl_wintab.c                                            *
** Author:                                                                   *
** Date:           12/2003                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng     Creat
******************************************************************************/

#define _MMICL_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_cl_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmi_menutable.h"
#include "mmicl_export.h"
#include "mmi_nv.h"
#include "guilistbox.h"
#include "mmipb_export.h"
//#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiphone_export.h"
#include "guimsgbox.h"
#include "mmisms_export.h"
#include "mmimms_export.h"
#include "mmi_textfun.h"
#include "mmisms_text.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipub.h"
#include "mmicc_id.h"
#include "mmicc_menutable.h"
#include "mmicc_nv.h"
//#include "mmiset_nv.h"
#include "mmi_text.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "mmicl_internal.h"
#include "mmicc_text.h"
#include "mmicc_image.h"
#include "guitab.h"
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
#include "mmimp_location.h"
#include "mmiphonesearch_export.h"
#endif
#include "mmi_mainmenu_export.h"
#include "mmiudisk_export.h"
#include "mmivt_export.h"
#include "mmiussd_export.h"
#include "mmk_app.h"
#include "guirichtext.h"
#include "mmicom_string.h"
//#include "mmiset_wintab.h"
//#include "mmiset_display.h"
#ifdef ASP_SUPPORT
#include "mmiasp_cortrol_layer.h"
#include "mmiasp_export.h"
#endif
#include "guictrl_api.h"

#ifdef MMI_PDA_SUPPORT
#include "guibutton.h"
#include "mmipb_adapt.h"
#endif


#include "guitoolbar.h"

#ifdef MMI_GPRS_SUPPORT
#include "mmipdp_export.h"
#endif

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
#include "mmibt_export.h"
#endif
#ifdef MMI_PDA_SUPPORT
#include "guires.h"
#endif
#include "mmipb_text.h"
#include "mmibt_text.h"
#include "mmipb_image.h"

#include "guidropdownlist.h"
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h" 
#endif
#include "mmipb_view.h"

//#include "mmipb_adapt.h"

#include "mmicom_banner.h"
#include "guitext.h"

#include "mmi_solarlunar.h"
#include "mmi_pubmenuwin.h"
#include "mmi_position.h"


#ifdef MMI_CL_TOOLBAR_SUPPORT
#undef MMI_CL_TOOLBAR_SUPPORT
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define     MMICL_DISPLAY_TIME_LEN  9
#define     MMICL_DISPLAY_DATE_LEN  11

#define     MMICL_TIMER_MAX (3600*1000-1) //call time display ma

#define     MMICL_DETAIL_HEADING_WIDTH			90
#define     MMICL_DETAIL_HEADING_RIGHT              (MMICL_DETAIL_HEADING_WIDTH - 10)

#define     MMICL_DETAIL_LEFT_MARGIN    5
#define     MMICL_DETAIL_RIGHT_MARGIN   5
#define     MMICL_DETAIL_LINE_SPACE     5


#define MMICL_TOOLBAR_ICON_GAP_TOP      5
#define MMICL_TOOLBAR_ICON_GAP_RIGHT    10
#define MMICL_TOOLBAR_PANEL_GAP         25

#define MAX_CALL_TIME_LEN   /*4*/6

// ͨ����ʱ��item�ĸ߶�
#define		MMI_CL_ITEM_HIGHT					30
#define     MMI_CL_CALL_TIME_LEFT               5
#ifdef VT_SUPPORT
//@zhaohui add
#define		MMICL_IMAGE_INC				(IMAGE_CL_DIALED_VTCALL_ICON - IMAGE_CL_DIALED_CALL_ICON)//ͨ����¼�п��ӵ绰Сͼ��image_id�������ͨ�绰Сͼ��image_id������
#endif

#define MMICL_MAX_SEARCH_LEN     20

/**--------------------------------------------------------------------------*
**                         PDA  MultiSIM                                     *
    ICON    NAME/NUMBER              |-|
            SimName Date    Time     |-|
**                         PDA  SingleSIM                                     *
    ICON    NAME/NUMBER              |-|
            Date    Time             |-|
**                         TK  MultiSIM                                      *


**                         BAR  MultiSIM                                      *


**--------------------------------------------------------------------------*/

//call log list item�и�����ʾ����
#ifndef MMI_MULTI_SIM_SYS_SINGLE 
#ifdef TOUCH_PANEL_SUPPORT
#define CL_LIST_ITEM_ICON_INDEX 5               //ICON/Selected ICON
#define CL_LIST_ITEM_TYPE_INDEX 0               //Image ID
#define CL_LIST_ITEM_NUM_OR_NAME_INDEX 1        //Contact Name or Number
#define CL_LIST_ITEM_SIM_INDEX  2                //Sim Name
#define CL_LIST_ITEM_DATE_INDEX 3               //Date
#define CL_LIST_ITEM_TIME_INDEX 4               //Time
#else
#define CL_LIST_ITEM_TYPE_INDEX 0               //Image ID
#define CL_LIST_ITEM_SIM_INDEX 1                //Sim Name
#define CL_LIST_ITEM_NUM_OR_NAME_INDEX 2        //Contact Name or Number
#define CL_LIST_ITEM_DATE_INDEX 3               //Date
#define CL_LIST_ITEM_TIME_INDEX 4               //Time
#define CL_LIST_ITEM_ICON_INDEX 5               //ICON/Selected ICON
#endif
#else
#ifdef TOUCH_PANEL_SUPPORT
#define CL_LIST_ITEM_ICON_INDEX 5               //ICON/Selected ICON
#define CL_LIST_ITEM_TYPE_INDEX 0               //Image ID
#define CL_LIST_ITEM_NUM_OR_NAME_INDEX 1        //Contact Name or Number
#define CL_LIST_ITEM_SIM_INDEX  4                //Sim Name shouldnot be displayed
#define CL_LIST_ITEM_DATE_INDEX 2               //Date
#define CL_LIST_ITEM_TIME_INDEX 3               //Time
#else
#define CL_LIST_ITEM_TYPE_INDEX 0
#define CL_LIST_ITEM_SIM_INDEX 0
#define CL_LIST_ITEM_NUM_OR_NAME_INDEX 1
#define CL_LIST_ITEM_DATE_INDEX 2
#define CL_LIST_ITEM_TIME_INDEX 3
#define CL_LIST_ITEM_ICON_INDEX 5               //ICON/Selected ICON
#endif
#endif

//[implement custom call log list UI]
#define CL_CUSTOM_LIST_ITEM_NUM_OR_NAME_INDEX        0
#define CL_CUSTOM_LIST_ITEM_SIM_CARD_INDEX           1
#ifdef MMI_MULTI_SIM_SYS_SINGLE
#define CL_CUSTOM_LIST_ITEM_TYPE_AND_DT_INDEX        1
#else
#define CL_CUSTOM_LIST_ITEM_TYPE_AND_DT_INDEX        2
#endif
#define CL_DETAIL_CALL_TYPE_INDEX                   0
#define CL_DETAIL_DURATION_INDEX                    1
#define CL_DETAIL_NUM_TYPE_AND_START_DT_INDEX       2
#define CL_DETAIL_SIM_CARD_INDEX                    3
//[end]

#ifdef CALL_TIMES_SUPPORT
#define CL_CT_LIST_ITEM_TYPE_INDEX 1
#define CL_CT_LIST_ITEM_SIM_INDEX 3
#define CL_CT_LIST_ITEM_CONTACT_INDEX 2
#define CL_CT_LIST_ITEM_DATE_INDEX 4
#define CL_CT_LIST_ITEM_TIME_INDEX 5

#endif
#ifdef MMI_GPRS_SUPPORT//for gprs flowrate
#define MMICL_ONE_K_LEN                    1024
#define MMICL_DSP_STR_LEN                  16
#endif

#ifdef MMI_PDA_SUPPORT
#define CL_SAVE_MAX_LEN (PHONE_NUM_MAX_LEN + 10)
//#define CL_SELECT_LABLE_LEN 55
#endif

#define WEEK_DAY_NUM    7

#ifndef ARRAY_SIZE
#define ARRAY_SIZE( a )     (sizeof(a)/sizeof (a)[0])
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
//add by bingjie.chen
typedef enum
{
    MISSED_NUMBER1,
    DAILED_NUMBER2,
    RECEIVED_NUMBER3,
#ifdef MMI_BLACKLIST_SUPPORT
    REJECTED_NUMBER4,
#endif
    DELETE_NUMBER5,
    TIMER_NUMBER6,
#ifdef MMI_GPRS_SUPPORT
    GPRS_NUMBER7, 
#endif
    NUMBER_MAX
}CL_NUMBER_LIST_E;
#endif

typedef enum
{
    CL_TOOLBAR_TYPE_INIT,
    CL_TOOLBAR_DIAL_AUDIO,
    #ifdef VT_SUPPORT
    CL_TOOLBAR_DIAL_VIDEO,
    #else
    CL_TOOLBAR_DIAL_IPCALL,
    #endif
    CL_TOOLBAR_DIAL_SMS,
    CL_TOOLBAR_DIAL_VIEW
}CL_TOOLBAR_TYPE_E;

typedef struct
{
    BOOLEAN         need_select_return;
    MMI_WIN_ID_T    return_win_id;
}MMICL_SELECT_AND_RETURN_INFO_T;

typedef struct 
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2];
    MN_DUAL_SYS_E dual_sys;
#if defined(CL_SAVE_NAME_SUPPORT)
    MMICL_NAME_T    name; 
#endif
}MMICL_DETAIL_INFO_T;

#ifdef MMI_PDA_SUPPORT
/*for support add contacts from calllog to message-edit win*/ 
typedef struct 
{
    BOOLEAN is_valid;
    //uint16 list_index;
}MMICL_SELECT_ITEM_INFO_T;

typedef struct 
{
    uint16 sel_count;
    //BOOLEAN is_all_selected;
    MMICL_SELECT_ITEM_INFO_T sel_item[MMICL_RECORD_MAX_NUM + 1];
}MMICL_ALL_SELECT_INFO_T;
#endif

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
//call log���ڷ�����Ϣ����Ҫ������֮���Ƿ���Ҫ���أ��Լ������ĸ��Ӵ���
LOCAL MMICL_SELECT_AND_RETURN_INFO_T s_select_return_info = {0}; 

//��¼call log���鴰���е������Ϣ
LOCAL MMICL_DETAIL_INFO_T *s_call_log_detail_info_ptr = PNULL;

//�����趨����WIN
typedef struct 
{
    BOOLEAN is_cl_need_update;
    MMI_HANDLE_T    updated_win_handle;
}MMICL_UPDATED_DATA_TAG;

LOCAL MMICL_UPDATED_DATA_TAG g_cl_updated_Data  = {0};

#ifdef MMI_PDA_SUPPORT
//��msg�������call logʱ����¼���б�ѡ�е�item�������Ϣ
LOCAL MMICL_ALL_SELECT_INFO_T s_call_log_all_select_info = {0};
#endif

#ifdef MMI_CL_MINI_SUPPORT
typedef struct 
{
    BOOLEAN       is_select;    //��¼�Ƿ������״̬
    BOOLEAN log_selected_index[MMICL_RECORD_TOTAL_NUM]; //��¼�ؼ���ÿ���绰��¼�ı��״̬
}MMICL_SELECT_INFO_T;

LOCAL MMICL_SELECT_INFO_T s_select_info = {0};

#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
LOCAL THEMELIST_ITEM_STYLE_T*   s_itemstyle_ptr = PNULL;
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMICL_CALL_TYPE_E   s_cur_sel_log_list_type;//��ǰ��ѡ�е�log list������

//call log data 
typedef struct
{
    MMICL_CALL_TYPE_E   g_cur_sel_filter_type;                      //List Filter  Type
    MMIPB_CONTACT_T *   matched_contact;                            //��ϵ����Ϣ 
    uint16              contact_matched_number;                     //��ϵ��ƥ�����Ŀ
    MMI_HANDLE_T        cl_list_handle ;                            //Applet������Ҫ��Handle
    MMI_HANDLE_T        main_win_id;
    BOOLEAN             isSelectingItem;                            //�Ƿ����ڽ���ɾ��ѡ��
    BOOLEAN             contact_selected_index[MMICL_MAX_MATCH_ITEM];//ͨ����¼�б�ѡ��
    BOOLEAN             isCalllogSelected;
}MMICL_PB_CALLLOG_T;

MMICL_PB_CALLLOG_T  *g_pbcalllog_data=PNULL;        //ȫ�ֱ���
LOCAL uint32 s_mmicl_match_item_index = 0;
LOCAL MMICL_MATCH_TOTAL_INFO_T *s_match_call_info_ptr = PNULL; //CL������������ƥ���¼

LOCAL wchar* s_match_key_ptr = PNULL; //CL���������ؼ����ַ���
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
LOCAL BOOLEAN s_is_multiselecting = FALSE;
LOCAL BOOLEAN s_log_selected_index[MMICL_MAX_MATCH_ITEM]={0};
#endif

LOCAL MMICL_GROUP_LIST_ITEM_T * s_group_list_info = PNULL;
LOCAL MN_DUAL_SYS_E s_sim_sel_dual_sys = MN_DUAL_SYS_1;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : SetAllLogFillter
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetAllLogFillter(MMICL_CALL_TYPE_E filter_type);
#endif

/*****************************************************************************/
// 	Description : GetAllLogFillter
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMICL_CALL_TYPE_E GetAllLogFillter(void);

#ifdef MMI_CL_TOOLBAR_SUPPORT
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : get current tool bar log list control id
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL CC_CONTROL_ID_E GetLogToolBarCtrlId(MMI_WIN_ID_T  win_id);
#endif
#endif
#endif

#ifdef MMI_PDA_SUPPORT
LOCAL BOOLEAN SetAllLogFillterPBNumber(MMIPB_CONTACT_T    *contact_ptr);
LOCAL MMIPB_BCD_NUMBER_T* GetAllLogFillterPBNumber(uint16 index);   
#endif

#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
/*****************************************************************************/
// 	Description : start multiselect 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void BeginMultiSelecting(void);
/*****************************************************************************/
// 	Description : exit multiselect
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void ExitMultiSelecting(void);
/*****************************************************************************/
// 	Description : to is multiselecting
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN isMultiSelecting(void);
/*****************************************************************************/
// 	Description : check current index is selecting or not
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN isCurrentSelected(uint16 index);
/*****************************************************************************/
// 	Description : Set current index 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetCurrentSelect(uint16 index,BOOLEAN isSelected);
#endif

/*****************************************************************************/
// 	Description : API for call log data control
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
//    Call log data
//        Init/Free   API
//        Get/Set APIS
//            ListHandle
//            AllLogFillter
//            AllLogFillterPBNumber
//            GetAllLogFillterPBNumberType
//        InitMatchedItem/AddMatchedItem/GetMatchedItemCount
/*****************************************************************************/
LOCAL BOOLEAN FreePbCalllogData(void)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    //Don't need free contact ptr    
    SCI_FREE(g_pbcalllog_data);
    
    return TRUE;
}
LOCAL MMI_HANDLE_T  CalllogPBData_GetMainWinID(void)
{
    if (g_pbcalllog_data ==PNULL)
        return PNULL;

    return g_pbcalllog_data->main_win_id;
}

LOCAL void  CalllogPBData_SetMainWinID(MMI_HANDLE_T main_win_id)
{
    if (g_pbcalllog_data ==PNULL)
        return;

    g_pbcalllog_data->main_win_id= main_win_id;
    return;

}

LOCAL BOOLEAN InitPbCalllogData(MMI_WIN_ID_T parent_win_id)
{
    if (g_pbcalllog_data !=PNULL)
    {   
       FreePbCalllogData();    
    }
    
    g_pbcalllog_data=SCI_ALLOC_APPZ(sizeof(MMICL_PB_CALLLOG_T));
    if (g_pbcalllog_data ==PNULL)
        return FALSE;
    
    CalllogPBData_SetMainWinID(parent_win_id);
    return TRUE;
}


LOCAL MMI_HANDLE_T   CalllogPBData_GetListHandle(void)
{
    if (g_pbcalllog_data!=PNULL)
    {
        return g_pbcalllog_data->cl_list_handle;
    }
    return PNULL;
}

LOCAL BOOLEAN   CalllogPBData_SetListHandle(MMI_HANDLE_T list_handle)
{
    if ((g_pbcalllog_data!=PNULL) &&(list_handle!=PNULL))
    {
        g_pbcalllog_data->cl_list_handle =list_handle ;
        return TRUE;
    }
    return FALSE;
}

LOCAL BOOLEAN SetAllLogFillter(MMICL_CALL_TYPE_E filter_type)
{

    if (filter_type >=MMICL_CALL_TYPE_MAX)
        return FALSE;

    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    g_pbcalllog_data->g_cur_sel_filter_type = filter_type;

    if (MMICL_CALL_PB_NUMBER_MATCHED ==filter_type)
    {
        g_pbcalllog_data->contact_matched_number= 0;
    }
    
    return TRUE;
}

LOCAL MMICL_CALL_TYPE_E GetAllLogFillter(void)
{
    if (g_pbcalllog_data ==PNULL)
        return MMICL_CALL_DIALED;

    return g_pbcalllog_data->g_cur_sel_filter_type;
}

LOCAL uint16    GetCalllogPBData_MatchedItemCount(void)
{
    if (g_pbcalllog_data==PNULL)
        return 0;
    return  g_pbcalllog_data->contact_matched_number;
}

LOCAL BOOLEAN SetAllLogFillterPBNumber(MMIPB_CONTACT_T    *contact_ptr)
{

    if (contact_ptr ==PNULL)
        return FALSE;
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    g_pbcalllog_data->matched_contact= contact_ptr;
    return TRUE;
}

#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error
LOCAL MMIPB_BCD_NUMBER_T* GetAllLogFillterPBNumber(uint16 index)
{
    if (MMICL_CALL_PB_NUMBER_MATCHED != GetAllLogFillter())
        return PNULL;

    if (g_pbcalllog_data ==PNULL)
        return PNULL;
        
    if (PNULL == g_pbcalllog_data->matched_contact)
        return PNULL;

    if (index>MMIPB_MAX_NV_PHONE_NUM)
        index = 0;
        
    return &(g_pbcalllog_data->matched_contact->number[index]);
}
LOCAL uint8 GetAllLogFillterPBNumberType(uint16 index)
{
    uint8   number_type;
    if (MMICL_CALL_PB_NUMBER_MATCHED != GetAllLogFillter())
        return PNULL;

    if (g_pbcalllog_data ==PNULL)
        return FALSE;
        
    if (PNULL == g_pbcalllog_data->matched_contact)
        return PNULL;

    if (index>=MMIPB_MAX_NV_PHONE_NUM)
        index = 0;

    number_type = g_pbcalllog_data->matched_contact->num_type[index];
    
    //if(MMIPB_IsUsim(g_pbcalllog_data->matched_contact->storage_id))
        {  
    //        number_type = number_type-1;
        }    
        
    return  number_type;
}

LOCAL void CalllogPBData_InitMatchedItem(void)
{
    if (g_pbcalllog_data ==PNULL)
        return ;
     g_pbcalllog_data->contact_matched_number=0 ;
}

LOCAL BOOLEAN CalllogPBData_AddMatchedItemCount(void)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;
     if (g_pbcalllog_data->contact_matched_number>=MMICL_MAX_MATCH_ITEM)
        return FALSE;
     g_pbcalllog_data->contact_matched_number++ ;
        
    return TRUE;
}
#endif


/*****************************************************************************/
//  Description : API for call log     Call log selection
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
//    Control APIS
//        Start/Stop/IsSelecting
//    Get/Set APIS
//        SetAllSelected
//        SelectedByIndex
/*****************************************************************************/
LOCAL BOOLEAN   CalllogPBData_StartSelection(void)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

     g_pbcalllog_data->isSelectingItem = TRUE;
     SCI_MEMSET(g_pbcalllog_data->contact_selected_index,0x00,sizeof(g_pbcalllog_data->contact_selected_index) );
    return TRUE;
}
LOCAL BOOLEAN   CalllogPBData_StopSelection(void)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

     g_pbcalllog_data->isSelectingItem = FALSE;

    return TRUE;
}
LOCAL BOOLEAN CalllogPBData_IsSelecting(void)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;
    return g_pbcalllog_data->isSelectingItem;
}


LOCAL BOOLEAN CalllogPBData_GetIsSelectedByIndex(uint16 index)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    return g_pbcalllog_data->contact_selected_index[index];

}

LOCAL BOOLEAN CalllogPBData_SetSelectedByIndex(uint16 index,BOOLEAN isSelected)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;
    g_pbcalllog_data->contact_selected_index[index] = isSelected;

    return TRUE;
}

LOCAL BOOLEAN CalllogPBData_SetAllSelected(BOOLEAN isSelected)
{
    uint16  i;
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    for (i=0;i<MMICL_MAX_MATCH_ITEM;i++)
    {
        g_pbcalllog_data->contact_selected_index[i] = isSelected;
    }
    return TRUE;
}

#ifndef MMI_CALLLOG_HIDE_MAINMENU
/*****************************************************************************/
// 	Description : to handle the call log main menu window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCLMainMenuWindow(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );
#endif

/*****************************************************************************/
// 	Description : to set call log list item
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void SetLogListItem(void);

/*****************************************************************************/
// 	Description : load call log list into listbox
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void AppendLogListItem(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info
                             );
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : to set call log select list item
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL void SetLogSelectListItem(void);

/*****************************************************************************/
// 	Description : load call log list into listbox for sms/mms
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL void AppendLogListItemForMessage(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info
                             );
/*****************************************************************************/
//  Description : MMICC_CreateNewSoftKey
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMICC_CreateNewSoftKey(MMI_HANDLE_T win_handle, MMI_HANDLE_T form_id,
                            MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, 
                            MMI_TEXT_ID_T rightkey_id);
#endif

/*****************************************************************************/
// 	Description : to handle the call log list menu window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLogListMenuWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add the num to blacklist   
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void AddNumToBlackList(
                        MMIPB_BCD_NUMBER_T  pb_bcd,    //IN:
                        MMI_WIN_ID_T win_id
                        );
#endif

/*****************************************************************************/
// 	Description : to handle the call log list window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLogListWindow(
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        );

/*****************************************************************************/
// 	Description : to handle the child window message
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleCallLogChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#if 0
/*****************************************************************************/
// 	Description : to handle the dialed child window message
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDialedChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
// 	Description : to handle the received child window message
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReceivedChildWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description : to handle the missed child window message
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMissedChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
// 	Description : to handle the refused child window message
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRefusedChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );
#endif
#endif

#ifdef MMI_PDA_SUPPORT 
/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );
#if defined MMI_ISTYLE_SUPPORT
/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  Istyle_HandleAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );
#endif
/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  Istyle_HandleSelectAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );

/*****************************************************************************/
// 	Description : to handle select contact from call log child window message
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );


#if 0
// 	Description : set call log title button background
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void Istyle_SetButtonBG(MMI_CTRL_ID_T ctrl_id, uint32 button_index);
#endif

#endif

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: WARNING: "GetCurrTypeNum" just get record num by type, only for pda project
//                 "GetRecordNumByType" not only get record num, but also assign arraycall_info
/*****************************************************************************/
LOCAL uint16 GetCurrTypeNum(MMICL_CALL_TYPE_E      call_type, MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info);

#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : Get menu offset by calltype
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL uint16 GetMenuIDOffsetByCallType(MMICL_CALL_TYPE_E call_type);
#endif

/*****************************************************************************/
//	Description : the handle function of call list delete menu window
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleLogListDeleteQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//	Description : the handle function of call list delete all menu window
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleLogListDeleteAllQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//	Description : the handle function of call list delete all menu window
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_Istyle_HandleLogListDeleteAllQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );
#endif

/*****************************************************************************/
// 	Description : set detail of call log list
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void SetLogListDetail(void);

/*****************************************************************************/
// 	Description : to handle the call log list detail window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLogListDetailWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description : delete current recode item
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DeleteCurrentRecord(void);

/*****************************************************************************/
// 	Description : ring back (�ظ��绰)
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleRingBack(CC_CALL_SIM_TYPE_E call_sim_type, 
                                                BOOLEAN is_video_call
                                                );
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
// 	Description : add phone number to name 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAddNumToName(void);
#endif

#if defined(CL_SAVE_NAME_SUPPORT)
/*****************************************************************************/
// 	Description : get display phone name of current log list 
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayPhoneName(
                              MMI_STRING_T*   tele_num//out
                              );
#endif

/*****************************************************************************/
// 	Description : get display phone number of current log list 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void GetDisplayPhoneNum(
                              uint8*   tele_num, //out
                              uint8	   max_tele_len,
                              MN_DUAL_SYS_E *dual_sys_ptr
                              );

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : get dual sys and isvideocall of current log list 
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void GetCallLogInfoDetail(MN_DUAL_SYS_E *dual_sys_ptr,BOOLEAN *isVideoCall);
#endif

/*****************************************************************************/
// 	Description : distill phone number from call log 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleDistillPhoneNum(void);

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
// 	Description : add a number into forbidden number list
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleForbiddenNum(void);
#endif

/*****************************************************************************/
// 	Description : get bcd phone number of current log list 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void GetPartyPhoneNum(
                            MMIPB_BCD_NUMBER_T*   bcd_number//out
                            );

/*****************************************************************************/
// 	Description : answer sms back
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAnswerSms(void);

//#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : ring back (�ظ��绰)
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleRingBackByType(uint32 call_type);
//#endif

/*****************************************************************************/
// 	Description : answer sms back
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAnswerMms(void);

/*****************************************************************************/
// 	Description : delete all call log with menu id
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DeleteAllCallLog(
                            MMICL_CALL_TYPE_E  call_type//in
                            );

/*****************************************************************************/
// 	Description : ��ȡ��ǰѡ���Calllog��arrayInfo�е�����ֵ
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL uint16 CL_GetArrayIndexofCurSelected(void);

/*****************************************************************************/
// 	Description : �򿪵�ǰѡ���Calllog��ʱ���б���
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void CL_OpenCallTimeListOrDetailWin(MMICL_CALL_ARRAY_BCDINFO_T * callinfo);

/*****************************************************************************/
// 	Description : display the time of last call
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimerWindow(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : display call time in control
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DisplayCallTimeItem(
                            uint8 line, 
                            CC_NV_ITEM_E   nv_item, //@$discuss 6800????CC_NV_ITEM_E,????????MMI_NV_ITME_E
                            MN_DUAL_SYS_E dual_sys,
                            MMI_CTRL_ID_T ctrl_id,
                            uint16* index_ptr,
                            MMI_TEXT_ID_T head_text_id
                            );
#else //MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : set label text about last call time
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DisplayCallTimeItem(
                            CC_NV_ITEM_E   nv_item,
                            MN_DUAL_SYS_E dual_sys,
                            MMI_CTRL_ID_T ctrl_id,
                            uint16* index_ptr,
                            MMI_TEXT_ID_T head_text_id
                            );
#endif //MMI_CSP_SUPPORT

/*****************************************************************************/
// 	Description : ת��uint32��ʱ��Ϊʱ���֣���ĸ�ʽ
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void ConvertTimeToDisplay(uint32  call_time,//in
                                uint8   *time//out
                                );

/*****************************************************************************/
// 	Description : reset the timer
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void ResetCalllogTimer(MN_DUAL_SYS_E dual_sys);

#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
// 	Description : ip call
//	Global resource dependence : 
//  Author:lin.lin
//	Note: 
/*****************************************************************************/
LOCAL void HandleIpCall(void);
#endif

/*****************************************************************************/
// 	Description : add all tabs and all child windows to parent window.
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL void  AddTabsAndChildWindows(void);


#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : to handle sim select window
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  CallTimeSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

#endif

/*****************************************************************************/
// 	Description : set one item contained head & content in richtext
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetHeadAndContentInRText(MMI_TEXT_ID_T head_text_id,      //��������id
                               MMI_CTRL_ID_T ctrl_id,            //���ڵĿؼ�����
                               MMI_STRING_T content_str,         //��ʾ����
                               uint16   *index
                               ); 

#ifdef MMI_CL_TOOLBAR_SUPPORT
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : handle toolbar item sel 
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_HandleToolBarItemSel(CL_TOOLBAR_TYPE_E node_id);

/*****************************************************************************/
// 	Description : createtoolbar
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_CreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
// 	Description : update cl toolbar
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_UpdateCLWinToolbar(MMI_WIN_ID_T      win_id,MMI_CTRL_ID_T       list_ctrl_id);
#endif
#endif
#endif

#if defined MMI_PDA_SUPPORT
LOCAL MMI_RESULT_E  HandleLogListMenukeyWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#ifdef MMI_PDA_SUPPORT
LOCAL void DrawListItemIconData(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info,GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr);
#endif

#ifndef MMI_CALLLOG_HIDE_MAINMENU
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaCallLogItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //�ؼ�ID
                                 MMI_TEXT_ID_T  text_1,     //���ı�
                                 MMI_STRING_T  text_str,     //���ı�
                                 MMI_IMAGE_ID_T image_id,   //ͼ��ID
                                 uint32         user_data   //�û�����
                                 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaCallLogListCtrl(void);
#endif

#endif



#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : display gprs flow rate
//	Global resource dependence : 
//  Author:li.li
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGprsFlowrateWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : handle message of sim list window 
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMSelectForGprsCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

#endif                                                

/*****************************************************************************/
//  Description : set the gprs flowrate window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void SetGprsFlowrateWindow(MMI_CTRL_ID_T ctrl_id,MN_DUAL_SYS_E  dual_sys);

/*****************************************************************************/
// 	Description : the gprs flowrate size
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
BOOLEAN FlowrateFormSize(
                    MMI_STRING_T* str_dst,        //out.
                    uint32 size
                    );
#endif
#ifdef CALL_TIMES_SUPPORT

/*****************************************************************************/
// 	Description : load call times list into listbox
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void AppendCallTimeListItem(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info,
                             uint16     selected_call_log_index
                             );

/*****************************************************************************/
// 	Description : to set call time list item
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimeListItem(MMI_CTRL_ID_T call_times_list_ctrl_id);

/*****************************************************************************/
// 	Description : set call time list title window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void  SetCallTimeListTitle(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : to handle call time list window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimeListWinMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  );

#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : to handle call time list option window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void  SetCallTimesListOptMenuTitle(uint16 call_times_index);

/*****************************************************************************/
// 	Description : to handle the call times list option window message
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimesListOptWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
// 	Description : to delete one call times list
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void DeleteOneCallTimesItem(uint16 select_index);

#endif

/*****************************************************************************/
// 	Description : set detail of call times list
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimesListDetail(uint16 select_index);

/*****************************************************************************/
// 	Description : to handle the call times detail window message
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimesDetailWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : to handle the contact call log list menu window message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailLogListMenuWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
// 	Description : to handle the call log list menu window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllLogListPopMenuWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );
#endif                                            

/*****************************************************************************/
// 	Description : to handle pub edit win to edit number before call
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditBeforeDialWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

                                            
/*****************************************************************************/
// 	Description : get record num by type and get record content to arraycall_info
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: WARNING: "GetCurrTypeNum" just get record num by type, only for pda project
//                 "GetRecordNumByType" not only get record num, but also assign arraycall_info
/*****************************************************************************/
LOCAL uint8 GetRecordNumByType(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info_ptr, MMICL_CALL_TYPE_E e_call_type);

/*****************************************************************************/
// 	Description : Get Current Record Info for next operation
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void GetCurrentRecordInfo(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info , uint16* sel_item_index);

#if 1 //ndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : ָ��MMICL_CALL_BCDINFO_T������MMIPB_BCD_NUMBER_T
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN Cl_ConvertBCDNumberFromCallInfo(MMIPB_BCD_NUMBER_T* pb_num_ptr,MMICL_CALL_BCDINFO_T* callinfo);

/*****************************************************************************/
// 	Description : �ӵ�ǰ��MMICL_CALL_BCDINFO_T������MMIPB_BCD_NUMBER_T  
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN Cl_GetCurrentBCDNumber(MMIPB_BCD_NUMBER_T* pb_num_ptr);

/*****************************************************************************/
// 	Description : ��ָ����MMICL_CALL_BCDINFO_T,��ȡ�˺����Ӧ�ĺ������
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN Cl_GetNumberTypeStrByNumber(wchar* name_str,MMICL_CALL_BCDINFO_T* callinfo);

/*****************************************************************************/
// 	Description : �ӵ�ǰ��MMICL_CALL_BCDINFO_T����ȡ��ǰ���벢�ж��Ƿ�����ϵ���б���
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckCurrentNumberInContact(void);

/*****************************************************************************/
// 	Description : �ӵ�ǰ��MMICL_CALL_BCDINFO_T������ϵ������
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void  OpenNumberInContact(void);
#endif

/*****************************************************************************/
// 	Description : ��õ�ǰ��Ҫ���µ�LOG WINHANDLE
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T MMICL_GetUpdatedWinHandle();


/*****************************************************************************/
// 	Description : ��ȡͨ����¼����ϵ���������ߺ�����ʾ
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  ��Ҫ���� Calltimes
          //�������ϵ�˵�ͨ����¼����Ҫת��Ϊ�������
/*****************************************************************************/
LOCAL BOOLEAN GetNameOrNumberContent(wchar *in_str,uint16 max_len,MMICL_CALL_BCDINFO_T *callinfo,
                                    BOOLEAN isAddCalltimes,BOOLEAN isUseNumberType);
                                    
/*****************************************************************************/
// 	Description : ��ȡCall type �� ICON ID
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetCallTypeImageID(MMICL_CALL_BCDINFO_T *callinfo);

#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : ��ȡ������ʾ��WSTRING
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  ����/��������Դ�ִ���ʾ�������������� Mon/Day ��ʾ   ���������
/*****************************************************************************/
LOCAL BOOLEAN GetDateContent(MMI_STRING_T* in_str,uint16 max_len,
                MMICL_CALL_BCDINFO_T *callinfo,uint16 call_time_index,BOOLEAN isNeedYear);

#endif
/*****************************************************************************/
// 	Description : ��ȡʱ����ʾ��WSTRING( HOUR:MIN)
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTimeContent(MMI_STRING_T* in_str,uint16 max_len,MMICL_CALL_BCDINFO_T *callinfo,uint16 call_time_index);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description :��ǰ�ļ�¼�Ƿ���Ҫ���뵽�б���
//	Global resource dependence : 
//  Author:sam.hua
//	Note: ����PDA�µ�PB�е�ͨ����¼�б�
/*****************************************************************************/
LOCAL BOOLEAN IsAddCurrentLogToList(MMICL_CALL_TYPE_E call_type,MMICL_CALL_BCDINFO_T *callinfo);

/*****************************************************************************/
// 	Description : to handle CallTime pop list win
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimePopListWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );
#endif

#ifdef CALL_TIMES_SUPPORT
/*****************************************************************************/
// 	Description : to delete one call times list
//	Global resource dependence : 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_DeleteOneCallTimesItem(
                                        MMICL_CALL_TYPE_E    call_type,  //call ����
                                        uint16               select_index
                                        );
#endif

#ifdef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : Set log list item for mark or unmark
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetCurrentSelect(uint16 index,BOOLEAN isSelected);

/*****************************************************************************/
// 	Description : Set all log list item for mark or unmark
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetAllSelect(BOOLEAN isSelected);

/*****************************************************************************/
// 	Description : Exit mark for Set all log list item
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void ExitSelect(void);

/*****************************************************************************/
// 	Description : to delete marked call log record item
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void DeleteRecord(void);

/*****************************************************************************/
// 	Description : menu grayed
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_LogListMenuGrayed(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// 	Description : ALL item is marked
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTotalItemIsMark(void);
#endif

#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
/*****************************************************************************/
// 	Description : multiselecting  win handler
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChildCallLogSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
LOCAL void SetItemStyle(MMI_CTRL_ID_T ctrl_id, uint16 index);
#endif

#ifdef MMI_LDN_SUPPORT
/*****************************************************************************/
// 	Description : Is CL LDN Record
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSimRecord(MMICL_CALL_ARRAY_BCDINFO_T *callinfo);

#endif

//[enter call log first UI]
LOCAL BOOLEAN MMIAPICL_IsVoiceMailNum(
                                 uint8 *tele_num,
                                 uint8 tele_len,
                                 MN_DUAL_SYS_E dual_sys);

LOCAL MMI_RESULT_E HandleLogEmptyStageWindow(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );

LOCAL void setLogEmpty(MMI_CTRL_ID_T ctrl_id);

LOCAL void setLogEmpty_new();

LOCAL MMI_RESULT_E HandleCustomLogListWindow(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

LOCAL void SetCustomLogListItem(
                            MMI_WIN_ID_T win_id,
                            MMI_CTRL_ID_T list_ctrl_id,
                            MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info,
                            MMICL_GROUP_LIST_ITEM_T *p_group_list_info);

LOCAL void AppendCustomLogListItem(
                             MMI_WIN_ID_T win_id,
                             MMI_CTRL_ID_T                  ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *p_arraycall_info,
                             MMICL_GROUP_LIST_ITEM_T        *p_group_list_info
                             );

LOCAL void GetCallTypeAndDateTimeContent(
                                            MMI_STRING_T* out_str,
                                            uint32 out_str_buf_size,
                                            MMICL_CALL_BCDINFO_T *p_callinfo);

LOCAL BOOLEAN GetTimeAndDateContent(
                            MMI_STRING_T* in_str,
                            uint16 max_len,
                            MMICL_CALL_BCDINFO_T *callinfo,
                            uint16 call_time_index);

LOCAL uint8 GetLogGroupListFromRecord(
                            MMICL_GROUP_LIST_ITEM_T *p_group_list_info,
                            MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info
                            );

LOCAL void GetCallInfoAndCallTimes(
                                    MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info,
                                    MMICL_GROUP_LIST_ITEM_T *p_group_list_info,
                                    uint8 group_list_index,
                                    MMICL_CALL_BCDINFO_T **pp_callinfo,
                                    uint8 * p_call_times);

LOCAL BOOLEAN GetGroupNameOrNumberContent(
                                            wchar *in_str,
                                            uint16 max_len,
                                            MMICL_CALL_BCDINFO_T *p_callinfo,
                                            uint16  call_times,
                                            BOOLEAN isAddCalltimes);

LOCAL void HandleRingBackByGroup(
                                        MMICL_GROUP_LIST_ITEM_T *p_group_list_info,
                                        MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info,
                                        CC_CALL_SIM_TYPE_E call_sim_type,
                                        BOOLEAN is_video_call);

LOCAL void GetDisplayPhoneNumByGroup(
                              MMICL_GROUP_LIST_ITEM_T *p_group_list_info,
                              MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info,
                              uint8   *tele_num, //out
                              uint8    max_tele_len,
                              MN_DUAL_SYS_E *dual_sys_ptr
                              );

LOCAL MMI_RESULT_E HandleLogBlockNumWindow(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

LOCAL void SetLogBlockNumber(MMI_CTRL_ID_T text_ctrl_id);

LOCAL MMI_RESULT_E  HandleLogSimSelMenuWindow(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

LOCAL void BinaryInsertSort(
                    MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info,
                    uint8 record_index,
                    uint8 call_time_index,
                    MMICL_GROUP_ITEM_DETAIL_T  *p_detail_info,
                    uint8 *p_total_num);

LOCAL void CalllogOpenBannerWin(MMI_WIN_ID_T win_id,MMI_TEXT_ID_T text1_id);

LOCAL void GetCallTypeString(
                                BOOLEAN is_group_list,
                                MMICL_CALL_TYPE_E call_type,
                                MMI_STRING_T * p_typeStr
                                );

LOCAL BOOLEAN IsNeedDisplaySimCard(MMICL_CALL_BCDINFO_T *p_callinfo);
//[end]

//added by du.yuxuan
LOCAL MMI_RESULT_E HandleLogListDetailMenuWindow(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

LOCAL void HandleContactView(void);
LOCAL void AppendLogListDetailItem(
                                    MMI_CTRL_ID_T                   ctrl_id,
                                    MMICL_CALL_TYPE_E               call_type,
                                    MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info,
                                    uint16                  selected_call_log_index
                                    );
LOCAL void SetLogListDetailItem();
LOCAL BOOLEAN InitLogListDetailContactItem(
                                    MMI_CTRL_ID_T           ctrl_id,
                                    MMICL_CALL_TYPE_E       call_type,
                                    MMICL_CALL_BCDINFO_T    *p_callinfo
                                    );


LOCAL void AppendContactAnim(MMICL_CALL_BCDINFO_T *callinfo, void *item_ptr, MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id);
LOCAL BOOLEAN SetLogListDetailContactItem(MMICL_CALL_BCDINFO_T* p_callinfo);
//end

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
#ifndef MMI_CALLLOG_HIDE_MAINMENU
#ifdef TOUCH_PANEL_SUPPORT
// the window for call log main menu
WINDOW_TABLE( MMICL_MAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCLMainMenuWindow ), 
    WIN_ID( MMICL_MAIN_MENU_WIN_ID ),
    WIN_TITLE(STXT_MAIN_CL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMICL_MAIN_MENU_CTRL_ID),
    END_WIN
};
#else
// the window for call log main menu
WINDOW_TABLE( MMICL_MAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCLMainMenuWindow ), 
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_ID( MMICL_MAIN_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_CL, MMICL_MAIN_MENU_CTRL_ID),
    END_WIN
};
#endif

#if defined (TOUCH_PANEL_SUPPORT) || defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
// the window for call log icon main menu
WINDOW_TABLE( MMICL_ICON_MAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCLMainMenuWindow ), 
    WIN_ID( MMICL_MAIN_MENU_WIN_ID ),  
    WIN_TITLE(STXT_MAIN_CL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMICL_MAIN_MENU_CTRL_ID),   
    END_WIN
};
#else
// the window for call log icon main menu
WINDOW_TABLE( MMICL_ICON_MAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCLMainMenuWindow ), 
    WIN_ID( MMICL_MAIN_MENU_WIN_ID ),  
#ifdef DPHONE_SUPPORT//syy modify 2010.11.27 -begin

#else
    WIN_STATUSBAR,
#endif//syy modify 2010.11.27 -end
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_ICON_CL, MMICL_MAIN_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
#endif
// the window for call log
WINDOW_TABLE( MMICL_LOG_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleLogListWindow),    
    WIN_ID( MMICL_LOG_LIST_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if defined MMI_GUI_STYLE_TYPICAL || defined MMI_CL_MINI_SUPPORT || defined MMI_GUI_STYLE_MINISCREEN
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL, MMICL_LOG_TAB_CTRL_ID),
#else
	CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMICL_LOG_TAB_CTRL_ID),
#endif
    END_WIN
};

// the child window for dialed
WINDOW_TABLE( MMICL_DIALED_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallLogChildWindow),    
    WIN_ID( MMICL_DIALED_CHILD_WIN_ID ),
//	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_DIALED_LIST_CTRL_ID),
    END_WIN
};

// the child window for received
WINDOW_TABLE( MMICL_RECEIVED_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallLogChildWindow),    
    WIN_ID( MMICL_RECEIVED_CHILD_WIN_ID ),
//	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_RECEIVED_LIST_CTRL_ID),
    END_WIN
};

// the child window for missed
WINDOW_TABLE( MMICL_MISSED_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallLogChildWindow),    
    WIN_ID( MMICL_MISSED_CHILD_WIN_ID ),
//	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_MISSED_LIST_CTRL_ID),
    END_WIN
};

#if defined(MMI_BLACKLIST_SUPPORT)
// the child window for refused
WINDOW_TABLE( MMICL_REFUSED_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallLogChildWindow),    
    WIN_ID( MMICL_REFUSED_CHILD_WIN_ID ),
//	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_REFUSED_LIST_CTRL_ID),
    END_WIN
};
#endif

#ifdef MMI_PDA_SUPPORT
// the child window for refused
WINDOW_TABLE( MMICL_ALL_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleAllChildWindow),    
    WIN_ID( MMICL_ALL_CHILD_WIN_ID ),
//	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMICL_ALL_LOG_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMICL_ALL_LOG_LIST_FORM_CTRL_ID, MMICL_ALL_LOG_FORM_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMICL_ALL_LOG_LIST_DROPLIST_CTRL_ID,MMICL_ALL_LOG_LIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E, MMICL_ALL_LIST_CTRL_ID,MMICL_ALL_LOG_FORM_CTRL_ID),
    
/*
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMICL_ALL_LOG_LIST_DROPLIST_FORM_CTRL_ID, MMICL_ALL_LOG_FORM_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMICL_ALL_LOG_LIST_DROPLIST_CTRL_ID,MMICL_ALL_LOG_LIST_DROPLIST_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMICL_ALL_LOG_LIST_FORM_CTRL_ID, MMICL_ALL_LOG_FORM_CTRL_ID),    
    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E, MMICL_ALL_LIST_CTRL_ID,MMICL_ALL_LOG_LIST_FORM_CTRL_ID),
*/    
    END_WIN
};

/*for support add contacts from calllog to message-edit win*/ 
WINDOW_TABLE( MMICL_ALL_SELECT_CHILD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSelectAllChildWindow),    
    WIN_ID( MMICL_ALL_SELECT_CHILD_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMICL_ALL_LOG_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMICL_ALL_LOG_LIST_FORM_CTRL_ID, MMICL_ALL_LOG_FORM_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMICL_ALL_LOG_LIST_DROPLIST_CTRL_ID, MMICL_ALL_LOG_LIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_CHECKLIST_E, MMICL_ALL_LIST_CTRL_ID, MMICL_ALL_LOG_FORM_CTRL_ID),
    END_WIN
};

#endif

// the window for call log list detail menu
WINDOW_TABLE( MMICL_LOG_LIST_DETAIIL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleLogListDetailWindow ),    
    WIN_ID( MMICL_LOG_LIST_DETAIIL_WIN_ID ),
    /*WIN_TITLE( STXT_DETAIL ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    #ifdef MMI_PDA_SUPPORT    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_LOG_LIST_DETAIL_CTRL_ID),
    #else
    CREATE_RICHTEXT_CTRL(MMICL_LOG_LIST_DETAIL_CTRL_ID),
    #endif
    */
    //added by du.yuxuan
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMICL_LOG_LIST_DETAIL_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMICL_LOG_LIST_DETAIL_CONTACT_LIST_CTRL_ID, MMICL_LOG_LIST_DETAIL_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMICL_LOG_LIST_DETAIL_LIST_CTRL_ID, MMICL_LOG_LIST_DETAIL_FORM_CTRL_ID),
    //end
    END_WIN
};

// the window for display last call time
WINDOW_TABLE( MMICL_CALL_TIMER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallTimerWindow ),    
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_ID( MMICL_CALL_TIMER_WIN_ID ),
    WIN_TITLE(TXT_NULL),

    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_CL_RESET, TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_CALL_TIMER_CTRL_ID),
    #else    
    CREATE_RICHTEXT_CTRL(MMICL_CALL_TIMER_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #endif
    
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMICL_LOG_LIST_MENUKEY_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLogListMenukeyWindow),
    WIN_ID(MMICL_LOG_LIST_MENUKEY_WIN_ID),
    CREATE_POPMENU_CTRL(MENU_CL_MEMU_KEY,MMICL_LOG_LIST_MENUKEY_CTRL_ID),
   
    END_WIN
};
#endif


WINDOW_TABLE( MMICL_LOG_LIST_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLogListMenuWindow),
    WIN_ID(MMICL_LOG_LIST_MENU_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CL_LOG_LIST_OPT,MMICL_LOG_LIST_MENU_CTRL_ID),
#else
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CL_LOG_LIST_OPT,MMICL_LOG_LIST_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),


#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    WIN_TITLE(STXT_OPTION),
#endif

    END_WIN
};

WINDOW_TABLE( MMICL_LOG_LIST_MENU_WIN2_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLogListMenuWindow),
    WIN_ID(MMICL_LOG_LIST_MENU_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CL_LOG_LIST_OPT2,MMICL_LOG_LIST_MENU_CTRL_ID),
#else   
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CL_LOG_LIST_OPT2,MMICL_LOG_LIST_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

#if defined(MMI_BLACKLIST_SUPPORT)
WINDOW_TABLE( MMICL_REFUSE_LIST_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLogListMenuWindow),
    WIN_ID(MMICL_LOG_LIST_MENU_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CL_REFUSE_OPT,MMICL_LOG_LIST_MENU_CTRL_ID),
#else   
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CL_REFUSE_OPT,MMICL_LOG_LIST_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMICL_REFUSE_LIST_MENU_WIN2_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLogListMenuWindow),
    WIN_ID(MMICL_LOG_LIST_MENU_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CL_REFUSE_OPT2,MMICL_LOG_LIST_MENU_CTRL_ID),
#else 
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CL_REFUSE_OPT2,MMICL_LOG_LIST_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif

//added by du.yuxuan
WINDOW_TABLE( MMICL_LOG_LIST_DETAIL_MENU_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleLogListDetailMenuWindow),
    WIN_ID(MMICL_LOG_LIST_DETAIL_MENU_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CL_DETAIL_OPT,MMICL_LOG_LIST_DETAIL_MENU_CTRL_ID),
#else
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CL_DETAIL_OPT,MMICL_LOG_LIST_DETAIL_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
//end

#ifdef CALL_TIMES_SUPPORT
WINDOW_TABLE( MMICL_CALL_TIMES_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallTimeListWinMsg),    
    WIN_ID( MMICL_CALL_TIMES_WIN_ID ),
#ifndef MMI_PDA_SUPPORT
    WIN_TITLE(STXT_MAIN_CL),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMICL_CALL_TIMES_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, MMICL_CALL_TIMES_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE, MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE, MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_CL_TOOLBAR_ADD, MMICL_CALL_TIMES_TITLE_BTN_CTRL_ID, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID),
#endif
#ifdef TOUCH_PANEL_SUPPORT    
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMICL_CALL_TIMES_BTN_FORM_CTRL_ID, MMICL_CALL_TIMES_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_CL_DETAIL_CALL_BUTTON,MMICL_CALL_TIMES_BTN_CALL_CTRL_ID ,MMICL_CALL_TIMES_BTN_FORM_CTRL_ID),
#ifdef MMI_IP_CALL_SUPPORT
    CHILD_BUTTON_CTRL(FALSE,IMAGE_CL_DETAIL_IPCALL_BUTTON,MMICL_CALL_TIMES_BTN_IPCALL_CTRL_ID ,MMICL_CALL_TIMES_BTN_FORM_CTRL_ID),
#endif
    CHILD_BUTTON_CTRL(FALSE,IMAGE_CL_DETAIL_MESSAGE_BUTTON,MMICL_CALL_TIMES_BTN_MSG_CTRL_ID ,MMICL_CALL_TIMES_BTN_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_CL_DETAIL_DELETE_BUTTON,MMICL_CALL_TIMES_BTN_DEL_CTRL_ID ,MMICL_CALL_TIMES_BTN_FORM_CTRL_ID),
#endif
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMICL_CALL_TIMES_LIST_FORM_CTRL_ID, MMICL_CALL_TIMES_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMICL_CALL_TIMES_MENU_CTRL_ID, MMICL_CALL_TIMES_FORM_CTRL_ID),
    //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_CALL_TIMES_MENU_CTRL_ID),
    END_WIN
};

#ifndef MMI_CL_MINI_SUPPORT
WINDOW_TABLE( MMICL_CALL_TIMES_LIST_OPTION_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleCallTimesListOptWindow),
    WIN_ID(MMICL_CALL_TIMES_LIST_OPTION_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CL_CALL_TIMES_LIST_OPT,MMICL_CALL_TIMES_LIST_OPT_CTRL_ID),
#else 
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CL_CALL_TIMES_LIST_OPT,MMICL_CALL_TIMES_LIST_OPT_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif

// the window for call log list detail menu
WINDOW_TABLE( MMICL_CALL_TIMES_DETAIIL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallTimesDetailWindow),    
    WIN_ID( MMICL_CALL_TIMES_DETAIIL_WIN_ID ),
    WIN_TITLE( STXT_DETAIL ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    #ifdef MMI_PDA_SUPPORT    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_CALL_TIMES_DETAIL_CTRL_ID),
    #else
    CREATE_RICHTEXT_CTRL(MMICL_CALL_TIMES_DETAIL_CTRL_ID),
    #endif
    END_WIN
};
#endif

#ifdef MMI_GPRS_SUPPORT
WINDOW_TABLE( MMICL_GPRS_FLOWRATE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleGprsFlowrateWinMsg ),    
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_ID( MMICL_GPRS_FLOWRATE_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_CL_RESET, TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_GPRS_FLOWRATE_CTRL_ID),
    #else    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMICL_GPRS_FLOWRATE_CTRL_ID),
    #endif
    
   
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMICL_PB_DETAIL_LOG_LIST_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePBDetailLogListMenuWindow),
    WIN_ID(MMICL_PB_DETAIL_CALLLOG_MENU_WIN_ID),
    CREATE_POPMENU_CTRL(MENU_CL_PB_DETAIL_MEMU,MMICL_PB_DETAIL_CALLLOG_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMICL_ALL_LIST_POP_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleAllLogListPopMenuWindow),
    WIN_ID(MMICL_ALL_LIST_POP_MENU_WIN_ID),
    CREATE_POPMENU_CTRL(MENU_ALL_LIST_POPUP_MEMU,MMICL_ALL_LIST_POPUP_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
#endif
//Use pubedit at first
WINDOW_TABLE( MMICL_EDIT_BEFORE_DIAL_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditBeforeDialWindow ),    
        WIN_ID( MMICL_EDIT_BEFORE_DIAL_WIN_ID ),
        WIN_TITLE( TXT_EDIT_NUMBER_BEFORE_DIAL ),
        CREATE_EDIT_PHONENUM_CTRL(PHONE_NUM_MAX_LEN, MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
        END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMICL_CALL_TIME_POP_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleCallTimePopListWindow),    
    WIN_ID( MMICL_CALL_TIME_POP_WIN_ID ),
    WIN_TITLE( STR_LOG_LIST_SAVE ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_CALL_TIME_POP_LIST_CTRL_ID),
    END_WIN
};
#endif

#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
WINDOW_TABLE( MMICL_CHILD_SELECT_WIN_TAB ) = 
{
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleChildCallLogSelectWindow),    
    WIN_ID( MMICL_CHILD_SELECT_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_CHILD_SELECT_LIST_CTRL_ID),
    END_WIN
};
#endif

//[enter call log first UI]
WINDOW_TABLE( MMICL_EMPTY_STAGE_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleLogEmptyStageWindow),
    WIN_ID( MMICL_EMPTY_STAGE_WIN_ID),
    WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, STXT_SOFTKEY_CALL_MK, TXT_COMMON_COMMON_BACK),
    //CREATE_RICHTEXT_CTRL(MMICL_LOG_EMPTY_CTRL_ID),
    //CREATE_TEXT_CTRL(MMICL_LOG_EMPTY_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMICL_CUSTOM_LOG_LIST_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCustomLogListWindow),
    WIN_ID( MMICL_CUSTOM_LOG_LIST_WIN_ID ),
    WIN_TITLE( STXT_MAIN_CL  ),
    WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, STXT_SOFTKEY_CALL_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_CUSTOM_LOG_LIST_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMICL_BLOCK_NUMBER_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleLogBlockNumWindow),
    WIN_ID( MMICL_BLOCK_NUMBER_WIN_ID ),
    WIN_TITLE(TXT_ADD_TO_BLACKLIST),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SAVE_MK, TXT_COMMON_COMMON_BACK),
    CREATE_TEXT_CTRL(MMICL_BLOCK_NUMBER_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMICL_SIM_SEL_MENU_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleLogSimSelMenuWindow),
    WIN_ID( MMICL_SIM_SEL_MENU_WIN_ID ),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};
//[end]

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : to handle the contact call log list window message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailCallLogListWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
// 	Description : to handle the contact call log list select item message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

// the window for call log icon main menu
WINDOW_TABLE( MMICL_PBDETAIL_CALLLOG_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePBDetailCallLogListWindow ), 
    WIN_ID( MMICL_PB_DETAIL_CALLLOG_WIN_ID ),  
//    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMICL_PB_DETAIIL_CALLLOG_LIST_CTRL_ID),   
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMICL_ALL_LOG_PB_DETAIL_LIST_CTRL_ID),   

    END_WIN
};

// the window for call log icon main menu
WINDOW_TABLE( MMICL_PBDETAIL_SELECT_WIN_TAB ) = 
{

    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePBDetailSelectWindow ), 
    WIN_ID( MMICL_PB_DETAIL_SELECT_WIN_ID ),  
    WIN_TITLE(TXT_DELETE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMICL_PB_DETAIL_SELECT_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS, MMICL_PB_DETAIL_SELECT_TITLE_FORM_CTRL_ID, MMICL_PB_DETAIL_SELECT_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMICL_PB_DETAIL_SELECT_TITLE_LABEL_CTRL_ID, MMICL_PB_DETAIL_SELECT_TITLE_FORM_CTRL_ID),
    CHILD_CHECKBOX_CTRL(TRUE, MMICL_PB_DETAIL_SELECT_TITLE_BUTTON_CTRL_ID, MMICL_PB_DETAIL_SELECT_TITLE_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE,GUILIST_CHECKLIST_E,MMICL_PB_DETAIL_SELECT_LIST_CTRL_ID,MMICL_PB_DETAIL_SELECT_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMICL_PB_DETAIL_SELECT_FORM_CTRL_ID),

    END_WIN
};


#endif



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : if enter into call log main menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
#ifdef MMI_PDA_SUPPORT
LOCAL void OpenCl_PDAMain(BOOLEAN is_permit,DPARAM  param_ptr)
{
    if (is_permit)
    {
        MMIPB_SetCLUnProtect(TRUE);
        MMIAPIPB_OpenMainWin(
                    MMIPB_CALLLOG_APPLICATION,//
                    MMIPB_LIST_NORMAL_MAIN, //IN: �����б������
                    MMIPB_LIST_FIELD_NUMBER,
                    0,//����Ƕ�ѡʱ�����������˶�ѡ��������
                    PNULL,
                    PNULL
                    );
    }
    else
    {
    }
    return;
}
#endif

/*****************************************************************************/
//  Description : if enter into call log main menu
//  Global resource dependence :
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_OpenMainMenuWindow(void)
{
    //[enter call log first UI]
    /*s_select_return_info.need_select_return = FALSE;
    s_select_return_info.return_win_id = 0;*/
    //[end]

#ifdef MMI_PDA_SUPPORT
    if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
    {
        MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, OpenCl_PDAMain, PNULL, 0);
    }
    else
    {
    MMIAPIPB_OpenMainWin(
                MMIPB_CALLLOG_APPLICATION,//
                MMIPB_LIST_NORMAL_MAIN, //IN: �����б������
                MMIPB_LIST_FIELD_NUMBER,
                0,//����Ƕ�ѡʱ�����������˶�ѡ��������
                PNULL,
                PNULL
                );
    }

#else
    //��˽����
    //[enter call log first UI]
    //MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CL, (MMISET_PRIVACY_APP_ENTRY_CALLBACK_PFUNC)MMICL_OpenMainMenuWindowExt);
    MMICL_OpenMainWindow();
    //[end]
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : to handle enter missed,received and dialed calls menu
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenCallsListWindow(
                                         MMICL_CALL_TYPE_E call_type
                                         )
{
    //[enter call log first UI]
    /*s_select_return_info.need_select_return = FALSE;
    s_select_return_info.return_win_id = 0;
    
    MMICL_HandleEnterCallsMenu(call_type);*/
    MMICL_OpenMainWindow();
    //[end]
}

//[enter call log first UI]
PUBLIC void MMICL_OpenMainWindow()
{
#ifdef MMI_LDN_SUPPORT
    if (!MMICL_IsCallInfoReady())
    {
        //MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        CalllogOpenBannerWin(PNULL,STR_CL_LOG_WAIT);
        return;
    }
#endif

    //s_cur_sel_log_list_type = MMICL_CALL_ALL;

    //��˽����
    if (MMIAPISET_IsOpenPrivacyProtect( MMISET_PROTECT_CL ) && !MMIAPISET_IsValidatePrivacyWinOpen(MMISET_PROTECT_CL))
    {
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CL, MMICL_OpenCLFirstWindow);
    }
    else
    {
        MMICL_OpenCLFirstWindow();
    }
}

PUBLIC void MMICL_OpenCLFirstWindow(void)
{
    /*uint8 record_num = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"HandleCallLogChildWindow call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4640_112_2_18_2_11_36_61,(uint8*)"");
        return;
    }
    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
    record_num = arraycall_info->record_num;

    SCI_FREE(arraycall_info);
    if(record_num == 0){
        MMK_CreateWin((uint32 *)MMICL_EMPTY_STAGE_WIN_TAB,PNULL);
    }else{
        //MMIAPICL_OpenCLWindow();
        MMK_CreateWin((uint32 *)MMICL_CUSTOM_LOG_LIST_WIN_TAB,PNULL);
    }*/
    MMK_CloseMiddleWin(MMICL_START_WIN_ID, MMICL_END_WIN_ID);
    MMK_CreateWin((uint32 *)MMICL_CUSTOM_LOG_LIST_WIN_TAB,PNULL);
}

LOCAL MMI_RESULT_E HandleLogEmptyStageWindow(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    //MMI_CTRL_ID_T ctrl_id = MMICL_LOG_EMPTY_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //setLogEmpty(ctrl_id);
        //MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyBtnGray(win_id,LEFT_BUTTON,TRUE);
        GUIWIN_SetSoftkeyBtnGray(win_id,MIDDLE_BUTTON,TRUE);
        break;
    case MSG_FULL_PAINT:
        setLogEmpty_new();
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

LOCAL void setLogEmpty(MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item_data = {0};
    uint16 index = 0;
    GUI_RECT_T richtext_rect = {0};

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));

    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = STR_CL_NOTE_NO_LOG_EXT02;
    item_data.text_set_align = TRUE;
    item_data.setting_align = ALIGN_HVMIDDLE;
    MMICL_GetTextRect(&richtext_rect);
    GUIRICHTEXT_SetRect(ctrl_id,richtext_rect);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
}

LOCAL void setLogEmpty_new()
{
    MMI_STRING_T log_empty_string = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect = {0};
    GUISTR_STYLE_T      str_style = {0};
    GUISTR_STATE_T state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK/*|GUISTR_STATE_SINGLE_LINE*/;

    str_style.align = ALIGN_HVMIDDLE;
    str_style.font  = MMI_DEFAULT_TEXT_FONT;
    str_style.font_color = MMI_WHITE_COLOR;
    //str_style.bg_color = MMI_BLACK_COLOR;

    MMI_GetLabelTextByLang(STR_CL_NOTE_NO_LOG_EXT02, &log_empty_string);
    //GUITEXT_SetString(MMISET_SET_UPDATE_TIME_MSGBOX_CTRL_ID,log_empty_string.wstr_ptr,log_empty_string.wstr_len,FALSE);
    //GUITEXT_SetAlign(ctrl_id,ALIGN_HVMIDDLE);
    MMICL_GetTextRect(&rect);
    GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_MENU_HDR_LOG_EXT02);

    GUISTR_DrawTextToLCDInRect(
            &lcd_dev_info,
            &rect,
            &rect,
            (const MMI_STRING_T    *)&log_empty_string,
            &str_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
}

PUBLIC BOOLEAN MMICL_GetTextRect(GUI_RECT_T *rect_ptr)
{
   uint16 mainscreen_height          = 0;
   uint16 mainscreen_width           = 0;

   if (PNULL == rect_ptr)
   {
      return FALSE;
   }
   if (!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &mainscreen_width, &mainscreen_height))
   {
      return FALSE;
   }

   rect_ptr->left = 0;
#if defined MMI_ONLY_IDLE_DISP_STATUSBAR
      rect_ptr->top = MMI_TITLE_HEIGHT + 1;
#else
      rect_ptr->top = 0/*MMI_STATUSBAR_HEIGHT + MMI_TITLE_HEIGHT +
      1mainscreen_height/2 - 2 * MMI_STATUSBAR_HEIGHT*/;
#endif
   rect_ptr->right = mainscreen_width - 1;
#if defined(MMI_PDA_SUPPORT)
   rect_ptr->bottom = mainscreen_height - 1;
#else
   rect_ptr->bottom = mainscreen_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - 1;
#endif

   return TRUE;
}

LOCAL MMI_RESULT_E HandleCustomLogListWindow(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   list_ctrl_id = MMICL_CUSTOM_LOG_LIST_CTRL_ID;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);

    static MMICL_CALL_ARRAY_BCDINFO_T* p_arraycall_info = PNULL;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);

            SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));

            GUILIST_SetItemHorMove(list_ctrl_id,TRUE);
            if (p_arraycall_info == PNULL)
            {
                p_arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            }
            if(p_arraycall_info == PNULL)
            {
                //SCI_TRACE_LOW:"HandleCallLogChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4640_112_2_18_2_11_36_61,(uint8*)"");
                return recode;
            }

            if(s_group_list_info == PNULL)
            {
                s_group_list_info = (MMICL_GROUP_LIST_ITEM_T*)SCI_ALLOCAZ(MMICL_RECORD_TOTAL_NUM * sizeof(MMICL_GROUP_LIST_ITEM_T));
            }
            if(s_group_list_info == PNULL)
            {
                return recode;
            }

            MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;

    case MSG_FULL_PAINT:
        MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_MENU_HDR_LOG_EXT02);
        MMICL_GetRecordInfo(MMICL_CALL_ALL, p_arraycall_info);
        SetCustomLogListItem(win_id,list_ctrl_id,p_arraycall_info,s_group_list_info);

        MMIAPICC_ClearMissedCallNum();
        break;
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;

    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1] && GetRecordNumByType(p_arraycall_info, MMICL_CALL_ALL) > 0)
        {
            HandleRingBackByGroup(s_group_list_info, p_arraycall_info, CC_CALL_SIM_MAX, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
        //@samboo.shen һ��vt
    case MSG_APP_VIDEO_TEL:
        //SCI_TRACE_LOW:"kick the key VT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4743_112_2_18_2_11_36_62,(uint8*)"");
        if (GetRecordNumByType(p_arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            HandleRingBackByGroup(s_group_list_info, p_arraycall_info, CC_MAJOR_SIM_CALL, FALSE);
        }
        break;
    case MSG_GET_FOCUS:
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        {
            uint8 record_num = GetRecordNumByType(p_arraycall_info,
            /*s_cur_sel_log_list_type*/MMICL_CALL_ALL);

            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    uint16 current_index = GUILIST_GetCurItemIndex(list_ctrl_id);
                    MMICL_CALL_BCDINFO_T  *p_callinfo =
                            &(p_arraycall_info->call_bcdinfo[s_group_list_info[current_index].position[0]]);
                    MMI_STRING_T namestrid = {0};
                    wchar wchar_namestr[PHONE_NUM_MAX_LEN + MAX_CALL_TIME_LEN+ 1] = {0};
                    BOOLEAN find_result = FALSE;

                    namestrid.wstr_ptr = wchar_namestr;
                    find_result = MMIAPICL_GetNameByClInfo(*p_callinfo,&namestrid,FALSE);
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB,
                            (ADD_DATA)find_result);   //rong.gu to do
                }
            }
        }
        break;
    case MSG_CTL_MIDSK:
        if(GetRecordNumByType(p_arraycall_info, MMICL_CALL_ALL) > 0)
        {
            HandleRingBackByGroup(s_group_list_info, p_arraycall_info, CC_CALL_SIM_MAX, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
    case MSG_APP_WEB:
        if(GetRecordNumByType(p_arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                #ifdef MMI_LDN_SUPPORT
                if (!IsSimRecord(p_arraycall_info))
                #endif
                {
                    CL_OpenCallTimeListOrDetailWin(p_arraycall_info);
                }
            }
        }
        break;

    case MSG_KEYDOWN_RIGHT:
        //MMK_CreateWin((uint32 *)MMICL_SIM_SEL_MENU_WIN_TAB,PNULL);
        break;

    //��Ҫ��ӦMSG_KEYLONG_HASH��MSG_KEYLONG_STAR�����Դ˴�������ӦKEYDOWN��Ϣ
    //boda,  it should use the press event
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
        MMIAPIIDLE_OpenDialWin(msg_id, TRUE);
       break;

    case MSG_KEYUP_HASH:
    case MSG_KEYUP_STAR:
        //��������ӦKEYDOWN��Ϣ�����Դ˴���Ҫ��KEYUP��Ϣת��ΪKEYDOWN��Ϣ���ٴ���������
        msg_id = msg_id + (MSG_KEYDOWN - MSG_KEYUP);
        MMIAPIIDLE_OpenDialWin(msg_id, TRUE);
        break;

    case MSG_APP_CANCEL:
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4640_112_2_18_2_11_36_61,(uint8*)"");
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        //MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        break;

    case MSG_CLOSE_WINDOW:
        if (p_arraycall_info != PNULL)
        {
            SCI_FREE(p_arraycall_info);
        }
        if (s_group_list_info != PNULL)
        {
            SCI_FREE(s_group_list_info);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

LOCAL void SetCustomLogListItem(
                            MMI_WIN_ID_T win_id,
                            MMI_CTRL_ID_T list_ctrl_id,
                            MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info,
                            MMICL_GROUP_LIST_ITEM_T *p_group_list_info)
{
    uint16 sel_item_index = 0;

    if(p_arraycall_info == PNULL)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2626_112_2_18_2_11_32_54,(uint8*)"");
        return;
    }

    //get the current selected item index
    //���绰��¼�б��Ѿ���ʱ������ˢ���б���Ҫ���¶�λ��ǰ��ѡ�е�item!
    sel_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);


    //load log list
    GUILIST_SetMaxItem(list_ctrl_id, MMICL_RECORD_MAX_NUM, FALSE );//max item 20

    AppendCustomLogListItem(win_id, list_ctrl_id, MMICL_CALL_ALL, p_arraycall_info, p_group_list_info);
    GUILIST_SetCurItemIndex(list_ctrl_id, sel_item_index);
}

LOCAL void AppendCustomLogListItem(
                             MMI_WIN_ID_T win_id,
                             MMI_CTRL_ID_T                  ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *p_arraycall_info,
                             MMICL_GROUP_LIST_ITEM_T        *p_group_list_info
                             )
{
    uint8 i = 0;

    GUILIST_ITEM_T item_t                          = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data                       = {0};/*lint !e64*/

    MMI_STRING_T name_number_str = {0};
    wchar name_number_wchar[PHONE_NUM_MAX_LEN + 1 + MAX_CALL_TIME_LEN]  ={0};

    MMI_STRING_T sim_name_str  = {0};
    wchar sim_name_wstr[GUILIST_STRING_MAX_NUM + 1]={0};

    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_STRING_T calltype_and_time_str       ={0};
    wchar wstr_type_time_str[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMICL_CALL_BCDINFO_T   *p_callinfo = PNULL;

    uint8 group_count = 0;
    uint16 call_times = 0;
    BOOLEAN is_gray = FALSE;

    BOOLEAN is_display_sim_card = TRUE;


    //item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_WITH_SIMSEL_MS;
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    #else
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TEXT_MS;
    #endif

    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT | GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;

    if (p_arraycall_info == PNULL || p_group_list_info == PNULL)
    {
        return;
    }

    SCI_MEMSET(p_group_list_info, 0, MMICL_RECORD_TOTAL_NUM * sizeof(MMICL_GROUP_LIST_ITEM_T));
    group_count = GetLogGroupListFromRecord(p_group_list_info,p_arraycall_info);

    is_gray = p_arraycall_info->record_num == 0 ? TRUE : FALSE;
    GUIWIN_SetSoftkeyBtnGray(win_id,LEFT_BUTTON,is_gray);
    GUIWIN_SetSoftkeyBtnGray(win_id,MIDDLE_BUTTON,is_gray);
    //set list item
    if (p_arraycall_info->record_num == 0)
    {
        //MMK_CloseWin(win_id);
        //MMK_CreateWin((uint32 *)MMICL_EMPTY_STAGE_WIN_TAB,PNULL);
        GUILIST_EMPTY_INFO_T empty_info = {0};

        empty_info.text_id = STR_CL_NOTE_NO_LOG_EXT02;
        GUILIST_SetEmptyInfo(ctrl_id,&empty_info);
    }
    else
    {
        for ( i = 0; i < group_count; i++ )
        {
            //Init Text Contents     Name/Number, SimName, Date, Time
            SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));
            name_number_str.wstr_ptr = name_number_wchar;
            SCI_MEMSET(sim_name_wstr,0,sizeof(sim_name_wstr));
            sim_name_str.wstr_ptr = sim_name_wstr;
            sim_name_str.wstr_len = 0;

            SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_wstr));
            datetime_str.wstr_ptr = datetime_wstr;
            SCI_MEMSET(&calltype_and_time_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&wstr_type_time_str,0,sizeof(wstr_type_time_str));
            calltype_and_time_str.wstr_ptr = wstr_type_time_str;

            call_times = 0;

            GetCallInfoAndCallTimes(p_arraycall_info,p_group_list_info,i,&p_callinfo,&call_times);

            //Add Contact name/or call number content
            if (GetGroupNameOrNumberContent(name_number_wchar,PHONE_NUM_MAX_LEN+MAX_CALL_TIME_LEN,
                    p_callinfo,call_times,TRUE))
            {
                name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
                item_data.item_content[CL_CUSTOM_LIST_ITEM_NUM_OR_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_CUSTOM_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                item_data.item_content[CL_CUSTOM_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
            }

            is_display_sim_card = IsNeedDisplaySimCard(p_callinfo);
            #ifndef MMI_MULTI_SIM_SYS_SINGLE
            //Add sim card image content
            {
                MMI_IMAGE_ID_T image_id = p_callinfo->dual_sys == MN_DUAL_SYS_1 ?
                        IMAGE_CUSTOM_SIM1_SMALL : IMAGE_CUSTOM_SIM2_SMALL;
                /*SCI_TRACE_LOW("AppendCustomLogListItem p_callinfo->dual_sys = %d",
                        p_callinfo->dual_sys);*/
                if(is_display_sim_card)
                {
                    item_data.item_content[CL_CUSTOM_LIST_ITEM_SIM_CARD_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[CL_CUSTOM_LIST_ITEM_SIM_CARD_INDEX].item_data.image_id = image_id;
                }
            }
            #endif

            if (call_times > 0
                    && p_callinfo->call_start_time[p_callinfo->call_times - 1] > 0)
            {
                //Add call type, call start time content
                BOOLEAN is_missed_call = p_callinfo->call_type == MMICL_CALL_MISSED;
                GUI_COLOR_T font_color_normal = is_missed_call ? MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT)
                        : MMI_BLACK_COLOR ;//MMI_DEFAULT_TEXT_COLOR;   ///ZDT_PRODUCT_Z685S
                GUI_COLOR_T font_color_focus = is_missed_call ? MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_3)
                        : MMI_BLACK_COLOR;// MMI_DARK_GRAY_COLOR;//MMI_DEFAULT_TEXT_COLOR;
                uint8 alpha = 255 * 60 / 100;
                uint8 call_type_dt_index = CL_CUSTOM_LIST_ITEM_TYPE_AND_DT_INDEX;

                GetCallTypeAndDateTimeContent(&calltype_and_time_str, GUILIST_STRING_MAX_NUM + 1, p_callinfo);

                #ifndef MMI_MULTI_SIM_SYS_SINGLE
                call_type_dt_index = is_display_sim_card ? CL_CUSTOM_LIST_ITEM_TYPE_AND_DT_INDEX : 1;
                item_t.item_style = is_display_sim_card ?
                        GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TEXT_MS :
                        GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
                #endif
                item_data.item_content[call_type_dt_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[call_type_dt_index].item_data.text_buffer.wstr_len = calltype_and_time_str.wstr_len;
                item_data.item_content[call_type_dt_index].item_data.text_buffer.wstr_ptr = calltype_and_time_str.wstr_ptr;

                /*item_data.item_content[CL_CUSTOM_LIST_ITEM_TYPE_AND_DT_INDEX].font_color_id
                        = MMITHEME_COLOR_BLUEMMI_BLUE_COLOR;*/
                GUILIST_SetItemContentOpacityFontColor(&item_data,call_type_dt_index,alpha,alpha);
                item_data.item_content[call_type_dt_index].is_opacity = TRUE;
                item_data.item_content[call_type_dt_index].is_custom_font_color = TRUE;
                item_data.item_content[call_type_dt_index].custom_font_color =
                        font_color_normal;
                item_data.item_content[call_type_dt_index].custom_font_color_focus =
                        font_color_focus;
            }

            item_t.user_data = (uint32)i;

            {
                /*MMI_IMAGE_ID_T sim_image_id = IMAGE_CL_SIM_1;
                uint8 sel_item_index = 0;

                sim_image_id = (s_sim_sel_dual_sys == MN_DUAL_SYS_1 ? IMAGE_CL_SIM_1
                        : IMAGE_CL_SIM_2);

                sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
                if(i == sel_item_index)
                {
                    GUILIST_SetQuickSimSelIcon(ctrl_id,sim_image_id);
                    s_sim_sel_dual_sys = MN_DUAL_SYS_1;
                }*/
            }

            GUILIST_AppendItem( ctrl_id, &item_t );
        }
    }
}

LOCAL void GetCallTypeAndDateTimeContent(
                                            MMI_STRING_T* out_str,
                                            uint32 out_str_buf_size,
                                            MMICL_CALL_BCDINFO_T *p_callinfo)
{
    MMI_STRING_T time_str       ={0};
    wchar wstr_time_str[GUILIST_STRING_MAX_NUM + 1] = {0};
    MMI_STRING_T call_type_string = {0};
    //wchar wstr_type[MMICL_CALL_TYPE_STRING_MAX_LEN + 2] = {0};
    //MMI_STRING_T temp_str = {0};
    //wchar comma_and_space[3] = {',',' ',0};
    wchar wstr_old[] = {L"%U"};

    //SCI_MEMSET(&time_str,0,sizeof(MMI_STRING_T));
    SCI_MEMSET(&wstr_time_str,0,sizeof(wstr_time_str));
    time_str.wstr_ptr = wstr_time_str;

    //temp_str.wstr_ptr = wstr_type;

    GetCallTypeString(TRUE,p_callinfo->call_type,&call_type_string);
    /*MMIAPICOM_Wstrncpy(temp_str.wstr_ptr,call_type_string.wstr_ptr,call_type_string.wstr_len);
    MMIAPICOM_Wstrcat(temp_str.wstr_ptr,comma_and_space);
    temp_str.wstr_len = MMIAPICOM_Wstrlen(temp_str.wstr_ptr);*/
    GetTimeAndDateContent(&time_str,GUILIST_STRING_MAX_NUM,p_callinfo,
            p_callinfo->call_times - 1);
    //MMIAPICOM_MergeTwoString(&temp_str,&time_str,out_str,GUILIST_STRING_MAX_NUM);
    MMIAPICOM_Wstrncpy(out_str->wstr_ptr,
            call_type_string.wstr_ptr,
            MIN(call_type_string.wstr_len, out_str_buf_size - 1));
    MMIAPICOM_WstrReplace(out_str->wstr_ptr, out_str_buf_size, wstr_old, wstr_time_str);
    out_str->wstr_len = MMIAPICOM_Wstrlen(out_str->wstr_ptr);
}

typedef struct
{
    MMICL_CALL_TYPE_E   call_type;
    MMI_TEXT_ID_T       txt_id;
} MMICL_CALL_TYPE_T;

LOCAL void GetCallTypeString(
                                BOOLEAN is_group_list,
                                MMICL_CALL_TYPE_E call_type,
                                MMI_STRING_T * p_typeStr
                                )
{
    uint8 i = 0;
    uint8 type_count = 0;
    /*uint8 calltype[MMICL_CALL_TYPE_STRING_MAX_LEN] = {0};
    uint8 calltype_len = 0;
    uint8 temp_str[MMICL_CALL_TYPE_STRING_MAX_LEN + 1] = {0};
    uint8 temp_str_len = 0;*/
    MMICL_CALL_TYPE_T *p_calltype_arr = PNULL;
    MMICL_CALL_TYPE_T const group_list_calltype_table[] =
    {
        {MMICL_CALL_MISSED, STR_CL_MISSED_EXT01},
        {MMICL_CALL_RECEIVED, STR_CL_INCOMING_EXT01},
        {MMICL_CALL_DIALED, STR_CL_OUTGOING_EXT01},
        {MMICL_CALL_REFUSED, STR_CL_REJECTED_EXT01}
    };
    MMICL_CALL_TYPE_T const group_detail_calltype_table[] =
    {
        {MMICL_CALL_MISSED, STXT_MISSED_CALLS},
        {MMICL_CALL_RECEIVED, STXT_INCOMING_CALLS_EXT01},
        {MMICL_CALL_DIALED, STXT_OUTGOING_CALLS_EXT01},
        {MMICL_CALL_REFUSED, STR_CALL_REJECTED}
    };

    p_calltype_arr = is_group_list ? group_list_calltype_table :
            group_detail_calltype_table;
    type_count = is_group_list ? ARRAY_SIZE(group_list_calltype_table) :
            ARRAY_SIZE(group_detail_calltype_table);

    for (i = 0; i < type_count; i++)
    {
        if (call_type == p_calltype_arr[i].call_type)
        {
            //SCI_MEMCPY(calltype,s_calltype_table[i].call_type_string,MMICL_CALL_TYPE_STRING_MAX_LEN);
            MMI_GetLabelTextByLang(p_calltype_arr[i].txt_id,p_typeStr);
            break;
        }
    }

    //sprintf((uint8 *)temp_str,"%s, ",calltype);
    //temp_str_len = strlen((uint8 *)temp_str);
    /*calltype_len = strlen((uint8 *)calltype);
    MMI_STRNTOWSTR(p_typeStr->wstr_ptr,MMICL_CALL_TYPE_STRING_MAX_LEN + 2,/*temp_str,
            MMICL_CALL_TYPE_STRING_MAX_LEN + 1,temp_str_len
            calltype,calltype_len,calltype_len);
    p_typeStr->wstr_len = MMIAPICOM_Wstrlen(p_typeStr->wstr_ptr);*/
}

LOCAL uint8 GetLogGroupListFromRecord(
                            MMICL_GROUP_LIST_ITEM_T *p_group_list_info,
                            MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info
                            )
{
    int group_count = 0;//Group list total number
    int i = 0, j = 0;
    BOOLEAN is_same_num = FALSE;
    MMICL_CALL_BCDINFO_T *p_callinfo = PNULL;

    // Insert all records to group list.
    for (j = 0; j < p_arraycall_info->record_num; j++)
    {
        is_same_num = FALSE;
        p_callinfo = &(p_arraycall_info->call_bcdinfo[j]);

        for (i = 0; i < group_count; i++)
        {
            MMIPB_BCD_NUMBER_T num1 = {0};
            MMIPB_BCD_NUMBER_T num2 = {0};
            size_t size = sizeof(num1.number);
            SCI_MEMCPY(num1.number,  p_group_list_info[i].number, size);
            num1.number_len = p_group_list_info[i].num_len;//strlen(num1.number);
            SCI_MEMCPY(num2.number,  p_callinfo->number, size);
            num2.number_len = p_callinfo->number_len;//strlen(num2.number);
            /*if (memcmp((char *)p_arraycall_info->call_bcdinfo[j].number,
                    (char *)p_group_list_info[i].number,
                    MMIPB_BCD_NUMBER_MAX_LEN) == 0)*/
            if (0 == MMIAPIPB_BcdSmartCmp(&num1, &num2) && p_group_list_info[i].dual_sys == p_callinfo->dual_sys)
            {
                p_group_list_info[i].position[p_group_list_info[i].total_num] = j;
                p_group_list_info[i].total_num++;
                is_same_num = TRUE;
                break;
            }
        }

        if (is_same_num == FALSE)//if the record is different from all group,add it to group list
        {
            SCI_MEMCPY(p_group_list_info[group_count].number,p_callinfo->number,p_callinfo->number_len);
            p_group_list_info[group_count].num_len = p_callinfo->number_len;
            p_group_list_info[group_count].position[0] = j;
            p_group_list_info[group_count].total_num++;
	        p_group_list_info[group_count].dual_sys = p_callinfo->dual_sys;
            group_count++;
        }
    }
    return group_count;
}

LOCAL BOOLEAN GetGroupNameOrNumberContent(
                                            wchar *in_str,
                                            uint16 max_len,
                                            MMICL_CALL_BCDINFO_T *p_callinfo,
                                            uint16  call_times,
                                            BOOLEAN isAddCalltimes)
{
    MMI_STRING_T namestrid = {0};
    wchar wchar_namestr[PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+ 1] = {0};
    MMI_STRING_T temp_strid={0};

    if ((in_str == PNULL) || (p_callinfo == PNULL))
    {
        return FALSE;
    }

    temp_strid.wstr_ptr = in_str;

    SCI_MEMSET(&namestrid,0,sizeof(MMI_STRING_T));
    SCI_MEMSET(wchar_namestr,0,sizeof(wchar_namestr));
    namestrid.wstr_ptr = wchar_namestr;

    if ((!p_callinfo->is_num_present) || (p_callinfo->is_emergency_call))
    {
        MMI_STRING_T res_strid = {0};
        if (p_callinfo->is_emergency_call)
        {
            MMI_GetLabelTextByLang(/*TXT_KL_PSW_EMERGENCY_CALL*/STR_CL_EMERGENCY_EXT01, &res_strid);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_UNKNOW_NUM, &res_strid);
        }
        res_strid.wstr_len = MIN(GUILIST_STRING_MAX_NUM, res_strid.wstr_len);
#ifdef CALL_TIMES_SUPPORT
        if ((call_times > 1) &&(isAddCalltimes))
        {
            MMICL_AddCallTimesStr(&res_strid,call_times,&temp_strid,GUILIST_STRING_MAX_NUM);
        }
        else
#endif
        {
            MMI_WSTRNCPY(in_str, GUILIST_STRING_MAX_NUM, res_strid.wstr_ptr, res_strid.wstr_len, res_strid.wstr_len);
        }
    }
    else
    {
        BOOLEAN find_result=FALSE;
        uint8   tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
        uint16  tele_num_len = 0;
#ifdef CALL_TIMES_SUPPORT
        uint16 temp_str_len = 0;
#endif
#if defined(MMI_PDA_SUPPORT) || defined(CALL_TIMES_SUPPORT)
        uint8   temp_str[PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+2 ] = {0};
#endif

        SCI_MEMSET(tele_num, 0x00, sizeof(tele_num));

        //get number
        tele_num_len = MMICL_GenDispNumber(p_callinfo->call_type,
            p_callinfo->number_type,
            p_callinfo->number_len,
            p_callinfo->number,
            tele_num,
            PHONE_NUM_MAX_LEN + 2);

        find_result = MMIAPICL_IsVoiceMailNum(tele_num, tele_num_len, p_callinfo->dual_sys);
        if (find_result)
        {
            MMI_GetLabelTextByLang(STR_CL_VOICEMAIL_EXT01, &namestrid);
        }
        else
        {
            find_result = MMIAPICL_GetNameByClInfo(*p_callinfo, &namestrid, FALSE);
        }

        //no name in pb
        if ( !find_result || 0 == namestrid.wstr_len )
        {
#ifdef CALL_TIMES_SUPPORT
            if((call_times > 1)&&(isAddCalltimes))
            {
                if(TRUE == MMISET_IsCurrentLanguageTextAlignRTLDirection())
                {
                    sprintf((char *)temp_str, "(%d)%s", call_times, tele_num);
                }
                else
                {
                    sprintf((char *)temp_str, "%s(%d)", tele_num, call_times);
                }
                temp_str_len = strlen((char *)temp_str) ;
                MMI_STRNTOWSTR(in_str,max_len,temp_str,temp_str_len,temp_str_len);
            }
            else
#endif
            {
                MMI_STRNTOWSTR(in_str,max_len,tele_num,tele_num_len,tele_num_len);
            }
        }
        else//there is name
        {
#ifdef CALL_TIMES_SUPPORT
            if((call_times > 1) &&(isAddCalltimes))
            {
                MMICL_AddCallTimesStr(&namestrid,call_times,&temp_strid,max_len);
            }
            else
#endif
            {
                //namestrid.wstr_len= MMIAPICOM_Wstrlen(namestrid.wstr_ptr);
                MMI_WSTRNCPY(in_str,max_len,(const wchar*)namestrid.wstr_ptr,
                            namestrid.wstr_len,namestrid.wstr_len);
            }
        }
    }

    return TRUE;

}

LOCAL void GetCallInfoAndCallTimes(
                                    MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info,
                                    MMICL_GROUP_LIST_ITEM_T *p_group_list_info,
                                    uint8 group_list_index,
                                    MMICL_CALL_BCDINFO_T **pp_callinfo,
                                    uint8 * p_call_times)
{
    int group_total_record_num = 0;
    int i = 0;

    if(p_arraycall_info == PNULL || p_group_list_info == PNULL)
    {
        return;
    }

    *pp_callinfo =
        &(p_arraycall_info->call_bcdinfo[p_group_list_info[group_list_index].position[0]]);
    group_total_record_num = p_group_list_info[group_list_index].total_num;

    for (; i < group_total_record_num; i++)
    {
        *p_call_times +=
                p_arraycall_info->call_bcdinfo[p_group_list_info[group_list_index].position[i]].call_times;
    }
}

LOCAL void HandleRingBackByGroup(
                                        MMICL_GROUP_LIST_ITEM_T *p_group_list_info,
                                        MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info,
                                        CC_CALL_SIM_TYPE_E call_sim_type,
                                        BOOLEAN is_video_call)
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint32 call_type = CC_CALL_NORMAL_CALL;
    MMI_STRING_T *name_ptr = PNULL;

    if(p_group_list_info == PNULL)
    {
        return;
    }

    //get the phone number
    GetDisplayPhoneNumByGroup(p_group_list_info, p_arraycall_info, tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);

    num_len = (uint8)strlen((char*)tele_num);

    //call
    if ( num_len > 0 )
    {
        if (is_video_call)
        {
            call_type = CC_CALL_VIDEO_CALL;
        }
        else
        {
            call_type = CC_CALL_NORMAL_CALL;
        }
        MMIAPICC_MakeCall(
                                        dual_sys,
                                        tele_num,
                                        num_len,
                                        PNULL,
                                        name_ptr,
                                        call_sim_type,
                                        call_type,
                                        PNULL
                                        );
    }
    else
    {
        //��ʾ�պ�!
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}

LOCAL void GetDisplayPhoneNumByGroup(
                              MMICL_GROUP_LIST_ITEM_T *p_group_list_info,
                              MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info,
                              uint8   *tele_num, //out
                              uint8    max_tele_len,
                              MN_DUAL_SYS_E *dual_sys_ptr
                              )
{
    MMICL_CALL_BCDINFO_T *p_call_bcdinfo = PNULL;

    if(max_tele_len > (PHONE_NUM_MAX_LEN + 2))
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneNum max_tele_len > (PHONE_NUM_MAX_LEN + 2)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8591_112_2_18_2_11_44_79,(uint8*)"");
        return;
    }

    if(tele_num == PNULL || dual_sys_ptr == PNULL || p_group_list_info == PNULL)
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneNum PNULL == tele_num ||PNULL == dual_sys_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8597_112_2_18_2_11_44_80,(uint8*)"");
        return;
    }

    //���ͨ����¼���鴰�ڴ򿪣�����Ӵ�����Ϣ�л��
    if ((MMK_IsFocusWin(MMICL_LOG_LIST_DETAIIL_WIN_ID)
    #ifdef CALL_TIMES_SUPPORT
        || MMK_IsFocusWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID)
    #endif
        )&& (PNULL != s_call_log_detail_info_ptr))
    {
        SCI_MEMCPY(tele_num, s_call_log_detail_info_ptr->tele_num, max_tele_len);
        *dual_sys_ptr = s_call_log_detail_info_ptr->dual_sys;
    }
    //������б��л��
    else
    {
        uint8 array_index = 0;

        if(PNULL == p_arraycall_info)
        {
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8620_112_2_18_2_11_44_81,(uint8*)"");
            return;
        }

        array_index = GUILIST_GetCurItemIndex(MMICL_CUSTOM_LOG_LIST_CTRL_ID);

        p_call_bcdinfo =
                &(p_arraycall_info->call_bcdinfo[p_group_list_info[array_index].position[0]]);
        if (p_call_bcdinfo->is_num_present)//�绰�������
        {
            //get number
            MMICL_GenDispNumber(p_call_bcdinfo->call_type,
                p_call_bcdinfo->number_type,
                p_call_bcdinfo->number_len,
                p_call_bcdinfo->number,
                tele_num,
                max_tele_len);
        }

        *dual_sys_ptr = p_call_bcdinfo->dual_sys;
    }
}

LOCAL MMI_RESULT_E HandleLogBlockNumWindow(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   text_ctrl_id = MMICL_BLOCK_NUMBER_CTRL_ID;
    //GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);


    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetLogBlockNumber(text_ctrl_id);
        MMK_SetAtvCtrl(win_id, text_ctrl_id);
        break;

    case MSG_FULL_PAINT:
        MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_MSG_HDR_BLOCK_NUMBER_EXT01);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        break;

    case MSG_CTL_MIDSK:
        {
            MMIPB_BCD_NUMBER_T  tele_num = {0};
            //get the phone number(bcd)
            GetPartyPhoneNum(&tele_num);
            AddNumToBlackList(tele_num,win_id);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_WEB:
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

LOCAL void SetLogBlockNumber(MMI_CTRL_ID_T text_ctrl_id)
{
    uint8            group_index = 0;
    uint8            record_index = 0;

    uint8   tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint16  tele_num_len = 0;

    wchar number_wchar[PHONE_NUM_MAX_LEN + 1] = {0};

    GUI_BG_T bg = {0};
    GUI_FONT_T font = 0;
    GUI_COLOR_T font_color = 0;

    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMICL_CALL_BCDINFO_T *p_callbcdinfo       = PNULL;

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetPartyPhoneNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8757_112_2_18_2_11_45_82,(uint8*)"");
        return;
    }

    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    GetCurrentRecordInfo(arraycall_info, &group_index);
    record_index = s_group_list_info[group_index].position[0];
    p_callbcdinfo = &(arraycall_info->call_bcdinfo[record_index]);

    tele_num_len = MMICL_GenDispNumber(p_callbcdinfo->call_type,p_callbcdinfo->number_type,
            p_callbcdinfo->number_len,p_callbcdinfo->number,
            tele_num,PHONE_NUM_MAX_LEN + 2);
    MMI_STRNTOWSTR(number_wchar,PHONE_NUM_MAX_LEN,tele_num,tele_num_len,tele_num_len);

    bg.bg_type = GUI_BG_COLOR;
    bg.color = MMI_WHITE_COLOR;
    GUITEXT_SetBg(text_ctrl_id,&bg);

    font = MMI_DEFAULT_TEXT_FONT;
    font_color = MMI_BLACK_COLOR;
    GUITEXT_SetFont(text_ctrl_id,&font,&font_color);
    GUITEXT_SetString(text_ctrl_id,number_wchar,MMIAPICOM_Wstrlen(number_wchar),FALSE);

    if(arraycall_info != PNULL)
    {
        SCI_FREE(arraycall_info);
    }
}

LOCAL void InsertMenuNode(
                                MMI_CTRL_ID_T       ctrl_id,        //�ؼ�id
                                MMI_TEXT_ID_T       text_id,        //����Ľڵ���ı�
                                MMI_IMAGE_ID_T      select_icon_id, //item selected icon id
                                uint16              node_id,        //�ڵ���
                                uint16              parent_node_id, //���ڵ���
                                uint16              index
                                 )
{
    MMI_STRING_T            string = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &string);
    node_item.item_text_ptr = &string;
    node_item.select_icon_id = select_icon_id;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}

LOCAL void CreateSimSelectionMenu(
                                MMI_WIN_ID_T win_id,
                                MMI_CTRL_ID_T ctrl_id)
{
    uint8 index = 0;
    MMI_TEXT_ID_T label = TXT_NULL;

    GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP);
    label = TXT_NET_UK_VODAFONE;
    InsertMenuNode(ctrl_id,label,IMAGE_CL_SIM_1,CL_SIM_SEL_MENU_VODAFONE,
            CL_SIM_SEL_MENU_NODE_ROOT,index);

    index++;
    label = TXT_NET_CHINA_UNICOM;
    InsertMenuNode(ctrl_id,label,IMAGE_CL_SIM_2,CL_SIM_SEL_MENU_CHINAUNICOM,
            CL_SIM_SEL_MENU_NODE_ROOT,index);

    GUIMENU_SetPopMenuCentered(ctrl_id,TRUE);

    //GUIMENU_SetPopMenuSoftkeyType(ctrl_id,GUISK_DATA_IMAGE_ID,GUISK_DATA_TEXT_ID,GUISK_DATA_IMAGE_ID);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id,TXT_NULL,STXT_SOFTKEY_SELECT_MK,TXT_COMMON_COMMON_BACK);
}

LOCAL MMI_RESULT_E  HandleLogSimSelMenuWindow(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T menu_ctrl_id = MMICL_SIM_SEL_MENU_CTRL_ID;
    uint8 node_id = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

        CreateSimSelectionMenu(win_id,menu_ctrl_id);
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);

        break;

    case MSG_GET_FOCUS:

        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifndef  MMI_PDA_SUPPORT    //not respond menu key
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        node_id = GUIMENU_GetCurNodeId(menu_ctrl_id);
        switch (node_id)
        {
        case CL_SIM_SEL_MENU_VODAFONE:
            s_sim_sel_dual_sys = MN_DUAL_SYS_1;
            MMK_CloseWin(win_id);
            break;

        case CL_SIM_SEL_MENU_CHINAUNICOM:
            s_sim_sel_dual_sys = MN_DUAL_SYS_2;
            MMK_CloseWin(win_id);
            break;

        default:
            break;
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_OK:

        break;

    case MSG_PROMPTWIN_CANCEL:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

PUBLIC BOOLEAN MMIAPICL_DeleteLogsAfterAddBlockListB(MMIPB_BCD_NUMBER_T pb_bcd)
{
    return MMICL_DeleteCallInfoByNum(pb_bcd);
}

PUBLIC BOOLEAN MMIAPICL_DeleteLogsAfterAddBlockListW(const wchar* p_num_wchar)
{
    uint8 block_num[PHONE_NUM_MAX_LEN + 2] = {0};//MMISET_PHONE_NUM_MAX_LEN
    MMI_PARTY_NUMBER_T party_num = {0};
    MMIPB_BCD_NUMBER_T pb_bcd = {0};

    MMIAPICOM_WstrToStr(p_num_wchar,block_num);
    MMIAPICOM_GenPartyNumber(block_num,strlen((char*)block_num),&party_num);

    pb_bcd.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
    pb_bcd.number_len = MIN(MN_MAX_ADDR_BCD_LEN, party_num.num_len);
    MMI_MEMCPY(
                    pb_bcd.number,
                    sizeof(pb_bcd.number),
                    (void*)&party_num.bcd_num,
                    sizeof(party_num.bcd_num),
                    pb_bcd.number_len
                    );
    return MMICL_DeleteCallInfoByNum(pb_bcd);
}

LOCAL uint8 GetGroupItemDetail(
                                uint8 group_index,
                                MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info,
                                MMICL_GROUP_ITEM_DETAIL_T  *p_detail_info)
{
    uint8 i = 0, j = 0;
    MMICL_GROUP_LIST_ITEM_T *p_group_list_item = PNULL;
    MMICL_CALL_BCDINFO_T *p_callinfo = PNULL;
    uint8 detail_total_num = 0;

    if(s_group_list_info == PNULL || p_detail_info == PNULL)
    {
        return 0;
    }

    p_group_list_item = &(s_group_list_info[group_index]);

    for (i = 0;i < p_group_list_item->total_num;i++)
    {
        uint8 record_index = p_group_list_item->position[i];

        p_callinfo = &(p_arraycall_info->call_bcdinfo[record_index]);
        for (j = 0; j < p_callinfo->call_times;j++)
        {
            BinaryInsertSort(p_arraycall_info,record_index,j,p_detail_info,&detail_total_num);
            if(detail_total_num >= MMICL_RECORD_MAX_NUM*MMICL_MAX_CALL_TIMES)
            {
			    detail_total_num = MMICL_RECORD_MAX_NUM*MMICL_MAX_CALL_TIMES;
			    break;
            }
        }
    }
    return detail_total_num;
}

LOCAL void BinaryInsertSort(
                    MMICL_CALL_ARRAY_BCDINFO_T *p_arraycall_info,
                    uint8 record_index,
                    uint8 call_time_index,
                    MMICL_GROUP_ITEM_DETAIL_T  *p_detail_info,
                    uint8 *p_total_num)
{
    int start = 0, end = *p_total_num;
    uint8 i = 0, insert_index = 0;
    uint32 detail_start_time = 0;
    MMICL_CALL_BCDINFO_T *p_callinfo = PNULL;

    p_callinfo = &(p_arraycall_info->call_bcdinfo[record_index]);

    while (start <= end)
    {
        int middle = (start + end) / 2;
        if(middle == *p_total_num)
        {
            break;
        }

        detail_start_time = p_arraycall_info->call_bcdinfo[p_detail_info[middle]
                .record_index].call_start_time[p_detail_info[middle].call_time_index];
        if (p_callinfo->call_start_time[call_time_index] <= detail_start_time)
        {
            end = middle - 1;
        }
        else
        {
            start = middle + 1;
        }
    }

    /*detail_start_time = p_arraycall_info->call_bcdinfo[p_detail_info[end]
            .record_index].call_start_time[p_detail_info[end].call_time_index];
    insert_index = p_callinfo->call_start_time[call_time_index] < detail_start_time ? end : end +
    1;*/
    insert_index = start;
    if (insert_index < *p_total_num)
    {
        for (i = *p_total_num; i > insert_index; i--)
        {
            p_detail_info[i] = p_detail_info[i - 1];
        }
    }
    p_detail_info[insert_index].record_index = record_index;
    p_detail_info[insert_index].call_time_index = call_time_index;
    (*p_total_num)++;
}

PUBLIC void MMICL_FormatTimeStrByTime(
                                        uint32 duration_time,
                                        MMI_STRING_T *p_time_str
                                        )
{
    MMI_STRING_T res_strid = {0};
    MMI_STRING_T txt_hou = {0};
    MMI_STRING_T txt_min = {0};
    MMI_STRING_T txt_sec = {0};
    uint8 time_label[5] = {0};
    uint8 time_str[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    uint8 time_min_str[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    uint8 time_sec_str[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    wchar wsz_hour[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    wchar wsz_min[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    wchar wsz_sec[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    MMI_STRING_T txt_temp = {0};
    MMI_STRING_T txt_temp2 = {0};
    MMI_TM_T tm = {0};

    if (p_time_str == PNULL)
    {
        return;
    }

    tm = MMIAPICOM_Second2Tm(duration_time);
    sprintf((char *)time_str,"%u ",tm.tm_hour);
    sprintf((char *)time_min_str,"%u ",tm.tm_min);
    sprintf((char *)time_sec_str," %u ",tm.tm_sec);

    MMIAPICOM_StrToWstr(time_str, wsz_hour);
    MMIAPICOM_StrToWstr(time_min_str, wsz_min);
    MMIAPICOM_StrToWstr(time_sec_str, wsz_sec);

    txt_hou.wstr_ptr = wsz_hour;
    txt_hou.wstr_len = MMIAPICOM_Wstrlen(wsz_hour);
    txt_min.wstr_ptr = wsz_min;
    txt_min.wstr_len = MMIAPICOM_Wstrlen(wsz_min);
    txt_sec.wstr_ptr = wsz_sec;
    txt_sec.wstr_len = MMIAPICOM_Wstrlen(wsz_sec);
    if (tm.tm_hour > 0)
    {
        MMI_GetLabelTextByLang(STR_TIME_H_EX01, &res_strid);
        MMIAPICOM_CatTwoString(&txt_temp, &txt_hou, &res_strid);
        MMI_GetLabelTextByLang(STR_TIME_MIN_EX01, &res_strid);
        MMIAPICOM_CatTwoString(&txt_temp2, &txt_temp, &txt_min);
        MMIAPICOM_CatTwoString(p_time_str, &txt_temp2, &res_strid);
        MMIAPICOM_DestroyString(txt_temp);
        MMIAPICOM_DestroyString(txt_temp2);
    }
    else if (tm.tm_min > 0 || tm.tm_sec > 0)
    {
        MMI_GetLabelTextByLang(STR_TIME_MIN_EX01, &res_strid);
        MMIAPICOM_CatTwoString(&txt_temp, &txt_min, &res_strid);
        MMI_GetLabelTextByLang(STR_TIME_SECOND_EX01, &res_strid);
        MMIAPICOM_CatTwoString(&txt_temp2, &txt_temp, &txt_sec);
        MMIAPICOM_CatTwoString(p_time_str, &txt_temp2, &res_strid);
        MMIAPICOM_DestroyString(txt_temp);
        MMIAPICOM_DestroyString(txt_temp2);
    }
    else
    {
         MMI_GetLabelTextByLang(STR_TIME_MIN_EX01, &res_strid);
         MMIAPICOM_CatTwoString(p_time_str, &txt_min, &res_strid);
    }

    /*if(duration_time == 0)
    {
        MMI_GetLabelTextByLang(STR_TIME_MIN_EX01, &res_strid);
        MMIAPICOM_WstrToStr(res_strid.wstr_ptr, time_label);
        sprintf((char *)time_str,"%u %s",duration_time,time_label);
    }
    else if(duration_time < 60)
    {
        MMI_GetLabelTextByLang(STR_TIME_SECOND_EX01, &res_strid);
        MMIAPICOM_WstrToStr(res_strid.wstr_ptr,time_label);
        sprintf((char *)time_str,"%u %s",duration_time,time_label);
    }
    else
    {
        uint8 min_str[5] = {0};
        uint8 sec_str[5] = {0};

        MMI_GetLabelTextByLang(STR_TIME_H_EX01, &res_strid);
        MMIAPICOM_WstrToStr(res_strid.wstr_ptr,time_label);
        tm = MMIAPICOM_Second2Tm(duration_time);
        if(tm.tm_hour > 0)
        {
            sprintf((char *)time_str,"%u %s",tm.tm_hour,time_label);
        }
        else
        {
            MMI_GetLabelTextByLang(STR_TIME_SECOND_EX01, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr,time_label);
            sprintf((char *)sec_str,"%u %s",duration_time,time_label);
        }

        MMI_GetLabelTextByLang(STR_TIME_MIN_EX01, &res_strid);
        MMIAPICOM_WstrToStr(res_strid.wstr_ptr,time_label);
        sprintf((char *)min_str,"%u %s",tm.tm_min,time_label);

        strcat(time_str,min_str);
        strcat(time_str,sec_str);
    }

    MMI_STRNTOWSTR(p_time_str->wstr_ptr,str_max_len,time_str,strlen(time_str),
            strlen(time_str));
    p_time_str->wstr_len = MMIAPICOM_Wstrlen(p_time_str->wstr_ptr);*/

    return;
}

LOCAL MMI_RESULT_E SendMsgSelectSimCallback(MN_DUAL_SYS_E dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    if(isReturnOK)
    {
        uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
        uint8 num_len = 0;
        MN_DUAL_SYS_E record_dual_sys = MN_DUAL_SYS_MAX;

        //get the phone number
        GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &record_dual_sys);
        num_len = (uint8)strlen((char*)tele_num);

        //call
        if ( num_len > 0 )
        {
            MMIAPISMS_ExitFromSMS();

            //����sms�ӿں���
            MMIAPISMS_AnswerMessage(dual_sys ,PNULL,tele_num,num_len);
        }
        else
        {
            //��ʾ����û�к���
            MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
        }
    }

    return recode;
}

LOCAL void CalllogOpenBannerWin(MMI_WIN_ID_T win_id,MMI_TEXT_ID_T text1_id)
{
    GUI_POINT_T start_point = {0, 0};
    MMICOM_BANNER_INFO_T banner_info = {0};

    banner_info.banner_type = MMI_BANNER_TEXT;
    banner_info.text1_id = text1_id;
    banner_info.start_point = start_point;
    MMIAPICOM_OpenBannerWin(win_id,banner_info);
}

LOCAL void CalllogSendMessage()
{
    uint16 sim_card_ok = 0 ;
    uint32 sim_card_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_card_ok, 1);

    if( 0 == sim_card_ok_num )
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if ( 1 == sim_card_ok_num )
    {
        SendMsgSelectSimCallback(sim_card_ok, 1, PNULL);
    }
    else
    {
        MMICL_SelectSIMFunc(SendMsgSelectSimCallback,MMISET_APPEND_SIM_OK,PNULL);
    }
}

LOCAL BOOLEAN IsNeedDisplaySimCard(MMICL_CALL_BCDINFO_T *p_callinfo)
{
    BOOLEAN is_display_sim_card = TRUE;

    if(p_callinfo->is_emergency_call/* && MMIAPIPHONE_GetSimExistedNum(PNULL,0) ==
    0*/)
    {
        is_display_sim_card = FALSE;
    }
    return is_display_sim_card;
}
//[end]

/*****************************************************************************/
//  Description : Some local functions
//  Global resource dependence :
//  Author:Sam.hua
//  Note:
//  Cl_WSTRAddChar  add a char in to a exist wchar array (!!ע�����ĳ��Ȳ�Ҫ����)
//  Cl_ConvertBCDNumberFromCallInfo //��ָ��MMICL_CALL_BCDINFO_T������MMIPB_BCD_NUMBER_T
//  Cl_GetCurrentBCDNumber          //�ӵ�ǰ��MMICL_CALL_BCDINFO_T������MMIPB_BCD_NUMBER_T    
//  Cl_GetNumberTypeStrByNumber     //��ָ����MMICL_CALL_BCDINFO_T,��ȡ�˺����Ӧ�ĺ������
//  CheckCurrentNumberInContact     //�ӵ�ǰ��MMICL_CALL_BCDINFO_T����ȡ��ǰ���벢�ж��Ƿ�����ϵ���б���
//  OpenNumberInContact             //�ӵ�ǰ��MMICL_CALL_BCDINFO_T������ϵ������
/*****************************************************************************/

#if 1 //def MMI_CL_MINI_SUPPORT//boda
/*****************************************************************************/
//  Description : ָ��MMICL_CALL_BCDINFO_T������MMIPB_BCD_NUMBER_T
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Cl_ConvertBCDNumberFromCallInfo(MMIPB_BCD_NUMBER_T* pb_num_ptr,MMICL_CALL_BCDINFO_T* callinfo)
{
    if (pb_num_ptr == PNULL ||callinfo == PNULL)
    {
        return FALSE;
    }

    SCI_MEMSET(pb_num_ptr,0x00, sizeof(MMIPB_BCD_NUMBER_T));

    pb_num_ptr->number_len = MIN(callinfo->number_len,MMIPB_BCD_NUMBER_MAX_LEN);
    pb_num_ptr->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(callinfo->number_type,
            callinfo->number_plan);
    MMI_MEMCPY(pb_num_ptr->number,MMIPB_BCD_NUMBER_MAX_LEN,callinfo->number,MMIPB_BCD_NUMBER_MAX_LEN,pb_num_ptr->number_len);

    return TRUE;
}

/*****************************************************************************/
// 	Description : �ӵ�ǰ��MMICL_CALL_BCDINFO_T������MMIPB_BCD_NUMBER_T  
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN Cl_GetCurrentBCDNumber(MMIPB_BCD_NUMBER_T* pb_num_ptr)
{
    uint16 array_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;

    if (pb_num_ptr ==PNULL)
    {
        return FALSE;
    }

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_1835_112_2_18_2_11_31_52,(uint8*)"");
        return FALSE;
    }

    array_index = CL_GetArrayIndexofCurSelected();

    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);

    Cl_ConvertBCDNumberFromCallInfo(pb_num_ptr,&(arraycall_info->call_bcdinfo[array_index]));

    SCI_FREE(arraycall_info);

    return TRUE;

}
/*****************************************************************************/
//  Description : ��ָ����MMICL_CALL_BCDINFO_T,��ȡ�˺����Ӧ�ĺ������
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Cl_GetNumberTypeStrByNumber(wchar* name_str,MMICL_CALL_BCDINFO_T* callinfo)
{
    MMIPB_BCD_NUMBER_T pb_num = {0};
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 tele_num_len = 0;
    wchar type_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar num_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
//    wchar out_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    MMI_STRING_T    type_string_id={0};
    MMI_STRING_T    num_string_id={0};
    uint16 typestr_len = 0;

    type_string_id.wstr_ptr =type_wstr;
    num_string_id.wstr_ptr =num_wstr;

    SCI_MEMSET(tele_num, 0x00, sizeof(tele_num));
    //get number
    tele_num_len = MMICL_GenDispNumber(callinfo->call_type,
        callinfo->number_type,
        callinfo->number_len,
        callinfo->number,
        tele_num,
        PHONE_NUM_MAX_LEN + 2);

    Cl_ConvertBCDNumberFromCallInfo(&pb_num,callinfo);

    if (MMIAPIPB_GetNumberTypeByNumber(&pb_num, type_wstr, GUILIST_STRING_MAX_NUM))
    {
        typestr_len = MMIAPICOM_Wstrlen(type_wstr);
        MMI_WSTRNCPY(name_str,GUILIST_STRING_MAX_NUM,(const wchar *)type_wstr,typestr_len,typestr_len);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : �ӵ�ǰ��MMICL_CALL_BCDINFO_T����ȡ��ǰ���벢�ж��Ƿ�����ϵ���б���
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckCurrentNumberInContact(void)
{

    MMIPB_BCD_NUMBER_T pb_num = {0};
    wchar type_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    Cl_GetCurrentBCDNumber(&pb_num);

    return MMIAPIPB_GetNumberTypeByNumber(&pb_num, type_wstr, GUILIST_STRING_MAX_NUM);
}
/*****************************************************************************/
// 	Description : �ӵ�ǰ��MMICL_CALL_BCDINFO_T������ϵ������
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void  OpenNumberInContact(void)
{
    MMIPB_BCD_NUMBER_T pb_num = {0};

    Cl_GetCurrentBCDNumber(&pb_num);

    MMIAPIPB_OpenNumberContactWin(&pb_num);

	return;
}
#endif


/*****************************************************************************/
// 	Description : ���õ�ǰ��Ҫ���µ�LOG WINHANDLE
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_SetUpdatedWinHandle(MMI_HANDLE_T  updated_win_handle,BOOLEAN isNeedUpdate)
{
    if (isNeedUpdate)
    {
        g_cl_updated_Data.updated_win_handle = updated_win_handle;
        g_cl_updated_Data.is_cl_need_update = FALSE;            
    }
    else
    {
        g_cl_updated_Data.updated_win_handle = 0;
        g_cl_updated_Data.is_cl_need_update = FALSE;        
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : ��õ�ǰ��Ҫ���µ�LOG WINHANDLE
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T MMICL_GetUpdatedWinHandle()
{
    return g_cl_updated_Data.updated_win_handle;
}


#if 0
/*****************************************************************************/
// 	Description : ��õ�ǰ��Ҫ���µ�LOG WINHANDLE
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMICL_GetIsNeedUpdated(void)
{
    return g_cl_updated_Data.is_cl_need_update;
}
#endif

/*****************************************************************************/
// 	Description : ��ȡ��ǰѡ���Calllog��arrayInfo�е�����ֵ
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL uint16 CL_GetArrayIndexofCurSelected(void)
{
    uint32 array_index = 0;
#ifdef MMI_PDA_SUPPORT    
    uint16 sel_item_index= 0;

    sel_item_index = GUILIST_GetCurItemIndex(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type));
    GUILIST_GetItemData(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type), sel_item_index, &array_index);
#else
    array_index =(uint32) GUILIST_GetCurItemIndex(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type));  
#endif    

    return (uint16)array_index;
}

/*****************************************************************************/
// 	Description : �򿪵�ǰѡ���Calllog��ʱ���б���
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void CL_OpenCallTimeListOrDetailWin(MMICL_CALL_ARRAY_BCDINFO_T * callinfo)
{
    uint16 array_index =0;
    if (callinfo == PNULL)
    {
        return;
    }
    array_index = CL_GetArrayIndexofCurSelected();
    //U200�У�����ӵ绰�������ͨ����¼ֱ����ʾͨ��ʱ���б�
#ifdef CALL_TIMES_SUPPORT    
//    if((callinfo->call_bcdinfo[array_index].call_times>1)||s_cur_sel_log_list_type==MMICL_CALL_ALL)
    if(callinfo->call_bcdinfo[array_index].call_times>0)    
    {
        MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_WIN_TAB, PNULL);
    }
    else
#endif    
    {
        MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, PNULL);
    }
}
#ifndef MMI_CALLLOG_HIDE_MAINMENU
#if defined (TOUCH_PANEL_SUPPORT) || defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaCallLogItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //�ؼ�ID
                                 MMI_TEXT_ID_T  text_1,     //���ı�
                                 MMI_STRING_T  text_str,     //���ı�
                                 MMI_IMAGE_ID_T image_id,   //ͼ��ID
                                 uint32         user_data   //�û�����
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar temp_wstr[40 + 2] = {0};
    wchar temp_len = 0;

    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        MMI_WSTRNCPY(temp_wstr,40+2,(const wchar*)text_str.wstr_ptr,text_str.wstr_len,text_str.wstr_len);
        temp_len = text_str.wstr_len;
    }

#ifdef MMI_GUI_STYLE_MINISCREEN
    item_t.item_style    = GUIITEM_STYLE_CUSTOMIZE;
#else
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT;
#endif

    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = image_id;

    //text 1
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_1;

    //text 2    
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = temp_len;
    }
        
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaCallLogListCtrl(void)
{
    //MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    //uint8 i = 0;
    //BOOLEAN flag = FALSE;
    MMI_CTRL_ID_T       ctrl_id = MMICL_MAIN_MENU_CTRL_ID;
    MMI_STRING_T  text_str = {0};
    //uint8 curr_index = 0;
    //MMI_TEXT_ID_T  text_id = TXT_NULL;
    MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info = PNULL;
    uint16 call_num = 0;
    uint8 num_string[3+ 1] = {0};
    uint16 num_string_len = 0;
    wchar num_wstring[3 + 1]= {0};
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
    MMI_TEXT_ID_T List_Number[] = {IMAGE_NUMBER_1,
                                   IMAGE_NUMBER_2,
                                   IMAGE_NUMBER_3,
                                   IMAGE_NUMBER_4,
                                   IMAGE_NUMBER_5,
                                   IMAGE_NUMBER_6,
                                   IMAGE_NUMBER_7
    };
#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
    uint16  i = 0;
#endif        
    GUILIST_SetMaxItem(ctrl_id, 20, FALSE );   
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetAllRecordsNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2076_112_2_18_2_11_31_53,(uint8*)"");
        return;
    }
   
    //read logs of calls from NV
    MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);

    call_num = GetCurrTypeNum( MMICL_CALL_MISSED,arraycall_info);    
    num_string_len = sprintf((char *)num_string, "%d", call_num);
    MMI_STRNTOWSTR(num_wstring, 3, (const uint8 *)num_string, 3, num_string_len);    
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN) 
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_MISSED_CALLS,text_str, List_Number[MISSED_NUMBER1],ID_CL_MISSED_CALLS);
    SetItemStyle(ctrl_id, i++);
#else
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_MISSED_CALLS,text_str, IMAGE_SECMENU_ICON_CL_MISS,ID_CL_MISSED_CALLS);
#endif
    SCI_MEMSET(num_string,0,sizeof(num_string));
    SCI_MEMSET(num_wstring,0,sizeof(num_wstring));
    
    call_num = GetCurrTypeNum( MMICL_CALL_DIALED,arraycall_info);    
    num_string_len = sprintf((char *)num_string, "%d", call_num);
    MMI_STRNTOWSTR(num_wstring, 3, (const uint8 *)num_string, 3, num_string_len);    
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_DIALED_CALLS,text_str,List_Number[DAILED_NUMBER2], ID_CL_DIALED_CALLS);
    SetItemStyle(ctrl_id, i++);
#else
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_DIALED_CALLS,text_str,IMAGE_SECMENU_ICON_CL_DIAL, ID_CL_DIALED_CALLS);
#endif
    SCI_MEMSET(num_string,0,sizeof(num_string));
    SCI_MEMSET(num_wstring,0,sizeof(num_wstring));

    call_num = GetCurrTypeNum( MMICL_CALL_RECEIVED,arraycall_info);    
    num_string_len = sprintf((char *)num_string, "%d", call_num);
    MMI_STRNTOWSTR(num_wstring, 3, (const uint8 *)num_string, 3, num_string_len);    
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_RECEIVED_CALLS,text_str,List_Number[RECEIVED_NUMBER3], ID_CL_RECEIVED_CALLS);
    SetItemStyle(ctrl_id, i++);
#else
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_RECEIVED_CALLS,text_str,IMAGE_SECMENU_ICON_CL_RECEIVE, ID_CL_RECEIVED_CALLS);
#endif
    SCI_MEMSET(num_string,0,sizeof(num_string));
    SCI_MEMSET(num_wstring,0,sizeof(num_wstring));
    
#if defined(MMI_BLACKLIST_SUPPORT)
    call_num = GetCurrTypeNum( MMICL_CALL_REFUSED,arraycall_info);    
    num_string_len = sprintf((char *)num_string, "%d", call_num);
    MMI_STRNTOWSTR(num_wstring, 3, (const uint8 *)num_string, 3, num_string_len);    
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_REFUSED_CALLS,text_str,List_Number[REJECTED_NUMBER4],ID_CL_REFUSED_CALLS);
    SetItemStyle(ctrl_id, i++);
#else
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_REFUSED_CALLS,text_str,IMAGE_SECMENU_ICON_CL_REJECT,ID_CL_REFUSED_CALLS);
#endif    
    SCI_MEMSET(num_string,0,sizeof(num_string));
    SCI_MEMSET(num_wstring,0,sizeof(num_wstring));
#endif

#ifdef ASP_SUPPORT
    SCI_MEMSET(&text_str,0x00,sizeof(MMI_STRING_T));
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CC_ANSWER_RECORD,text_str,IMAGE_SECMENU_ICON_CALL_RECORD,ID_CL_ASP);
#endif
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
    AppendPdaCallLogItem2Line(ctrl_id,TXT_DELALL,text_str,List_Number[DELETE_NUMBER5],ID_CL_DELETE_ALL_RECORDS);
    SetItemStyle(ctrl_id, i++);
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_CALL_TIMERS,text_str,List_Number[TIMER_NUMBER6],ID_CL_CALL_TIMERS);
    SetItemStyle(ctrl_id, i++);
#ifdef MMI_GPRS_SUPPORT
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_GPRS_FLOWRATE,text_str,List_Number[GPRS_NUMBER7],ID_CL_GPRS_FLOWRATE);
    SetItemStyle(ctrl_id, i++);
#endif
#else
    AppendPdaCallLogItem2Line(ctrl_id,TXT_DELALL,text_str,IMAGE_SECMENU_ICON_CL_DELALL,ID_CL_DELETE_ALL_RECORDS);
#ifndef CMCC_UI_STYLE
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_CALL_TIMERS,text_str,IMAGE_SECMENU_ICON_CL_TIME,ID_CL_CALL_TIMERS);
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_GPRS_FLOWRATE,text_str,IMAGE_SECMENU_ICON_CL_GPRS,ID_CL_GPRS_FLOWRATE);
#endif
#endif

    SCI_FREE(arraycall_info);

}

/*****************************************************************************/
//  Description : to handle the call log main menu window message
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCLMainMenuWindow(
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
    uint16 index = 0;
    uint32 user_data = 0;
    MMI_CTRL_ID_T ctrl_id = MMICL_MAIN_MENU_CTRL_ID;
    //GUILIST_POS_INFO_T pos_info = {0};
//add by bingjie.chen
    static uint16 index_pos = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_GUI_STYLE_MINISCREEN
        if (!s_itemstyle_ptr)
        {
            s_itemstyle_ptr = (THEMELIST_ITEM_STYLE_T*)SCI_ALLOC_APPZ(sizeof(*s_itemstyle_ptr));
        }
#endif
        InitPdaCallLogListCtrl();
#ifdef MMI_CL_MINI_SUPPORT
        ExitSelect();
#endif
        MMK_SetAtvCtrl(win_id, MMICL_MAIN_MENU_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:           
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &user_data);

        switch (user_data)
        {
        case ID_CL_DIALED_CALLS:
            index_pos = GUILIST_GetCurItemIndex(ctrl_id);
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_DIALED);
            break;

        case ID_CL_MISSED_CALLS:
            index_pos = GUILIST_GetCurItemIndex(ctrl_id);
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_MISSED);
            break;

        case ID_CL_RECEIVED_CALLS:
            index_pos = GUILIST_GetCurItemIndex(ctrl_id);
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_RECEIVED);
            break;

#if defined(MMI_BLACKLIST_SUPPORT)
        case ID_CL_REFUSED_CALLS:
            index_pos = GUILIST_GetCurItemIndex(ctrl_id);
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_REFUSED);
            break;
#endif

#ifdef ASP_SUPPORT
        case ID_CL_ASP:
        {
            index_pos = GUILIST_GetCurItemIndex(ctrl_id);
            MMIAPIASP_EnterAsp();
            break;
        }
#endif

        case ID_CL_DELETE_ALL_RECORDS:
            //get all records number
            index_pos = GUILIST_GetCurItemIndex(ctrl_id);
            if (0 == MMICL_GetAllRecordsNum()) //û�м�¼
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_ENTRY);
            }
            else
            {
                if (MMK_IsOpenWin(MMICL_QUERY_WIN_ID))
                {
                    MMK_CloseWin(MMICL_QUERY_WIN_ID);
                }
                //ѯ���Ƿ�ɾ��!!
                MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
            }
            break;

        case ID_CL_CALL_TIMERS:
            {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                uint16 sim_card_ok = 0 ;
                uint32 sim_card_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_card_ok, 1);

                index_pos = GUILIST_GetCurItemIndex(ctrl_id);
                if( 0 == sim_card_ok_num )
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                else if ( 1 == sim_card_ok_num )
                {
                    MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, (ADD_DATA)sim_card_ok);       
                }
                else
                {
                    MMICL_SelectSIMFunc(CallTimeSelectSimCallback,MMISET_APPEND_SIM_OK,PNULL);
                }
#else
                index_pos = GUILIST_GetCurItemIndex(ctrl_id);
                MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, PNULL);
#endif
            }
            break;

        case ID_CL_GPRS_FLOWRATE:

#ifdef MMI_GPRS_SUPPORT

            MMIAPICL_OpenGprsFlowRateWin();
            index_pos = GUILIST_GetCurItemIndex(ctrl_id);
#endif
            break;

        case ID_CL_TIMER_RESET:
            //ѯ���Ƿ�����!!
            index_pos = GUILIST_GetCurItemIndex(ctrl_id);
            MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
            break;

        default:
            break;
        }
        break;

    case MMI_CL_MAINMENU_UPDATE_INDEX:
        index_pos = *(uint32 *)param;
        break;

    case MSG_GET_FOCUS:

        //top_index = GUILIST_GetTopItemIndex(ctrl_id);
#ifdef MMI_CL_MINI_SUPPORT
        ExitSelect();
#endif
        InitPdaCallLogListCtrl();
#ifdef MMI_CL_MINI_SUPPORT
        GUILIST_SetCurItemIndex(ctrl_id, index_pos);
#else
        //Set list focused index for
        if (s_cur_sel_log_list_type != MMICL_CALL_ALL && index_pos>=ID_CL_CALLLOG_MENUS)
        {
            GUILIST_SetTopItemIndex(ctrl_id, GetMenuIDOffsetByCallType(s_cur_sel_log_list_type));
            GUILIST_SetCurItemIndex(ctrl_id, GetMenuIDOffsetByCallType(s_cur_sel_log_list_type));
            s_cur_sel_log_list_type = MMICL_CALL_ALL;
        }
        else
        {
            GUILIST_SetTopItemIndex(ctrl_id, index_pos);
            GUILIST_SetCurItemIndex(ctrl_id, index_pos);
        }
#endif
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_OK:
//fixed by bingjie.chen
        MMIAPICC_ClearMissedCallNum();
        DeleteAllCallLog(MMICL_CALL_ALL);
        MMIPUB_CloseQuerytWin(&query_win_id);

        break;

    case MSG_PROMPTWIN_CANCEL:
         //�ر�ѯ�ʴ���
         MMIPUB_CloseQuerytWin(&query_win_id);
         break;
#ifdef MMI_GUI_STYLE_MINISCREEN
    case MSG_CLOSE_WINDOW:
        {
            if (s_itemstyle_ptr)
            {
                SCI_FREE(s_itemstyle_ptr);
                s_itemstyle_ptr = PNULL;
            }
            break;
        }
#endif
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
#else
/*****************************************************************************/
//  Description : to handle the call log main menu window message
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCLMainMenuWindow(
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           )
{
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, MMICL_MAIN_MENU_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        s_cur_sel_log_list_type = MMICL_CALL_ALL;   //reset flag here
        break;
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT  // menu ������Ӧ
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        GUIMENU_GetId(MMICL_MAIN_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_DIALED_CALLS:
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_DIALED);
            break;

        case ID_CL_MISSED_CALLS:
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_MISSED);
            break;

        case ID_CL_RECEIVED_CALLS:
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_RECEIVED);
            break;

#if defined(MMI_BLACKLIST_SUPPORT)
        case ID_CL_REFUSED_CALLS:
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_REFUSED);
            break;
#endif

#ifdef ASP_SUPPORT
        case ID_CL_ASP:
        {
             MMIAPIASP_EnterAsp();
            break;
        }
#endif

        case ID_CL_DELETE_ALL_RECORDS:
            //get all records number
            if (0 == MMICL_GetAllRecordsNum()) //û�м�¼
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_ENTRY);
            }
            else
            {
                if (MMK_IsOpenWin(MMICL_QUERY_WIN_ID))
                {
                    MMK_CloseWin(MMICL_QUERY_WIN_ID);
                }
                //ѯ���Ƿ�ɾ��!!
                MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
            }
            break;

        case ID_CL_CALL_TIMERS:
            {            
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                uint16 sim_card_ok = 0 ;
                uint32 sim_card_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_card_ok, 1);

                if( 0 == sim_card_ok_num )
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                else if ( 1 == sim_card_ok_num )
                {
                    MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, (ADD_DATA)sim_card_ok);       
                }            
                else
                {
                    MMICL_SelectSIMFunc(CallTimeSelectSimCallback,MMISET_APPEND_SIM_OK,PNULL);
                }
#else
                MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, PNULL);
#endif
            }
            break;

        case ID_CL_GPRS_FLOWRATE:
#ifdef MMI_GPRS_SUPPORT
            MMIAPICL_OpenGprsFlowRateWin();
#endif
            break;

#ifdef AOC_SUPPORT
        case ID_CL_COSTS_TOTAL:
        case ID_CL_COSTS_MAX:
        case ID_CL_COSTS_RATE:
        case ID_CL_COSTS_RESET:
            MMICL_COSTS_HandleMainMenuMsg(menu_id);
            break;
#endif

        case ID_CL_TIMER_RESET:
            //ѯ���Ƿ�����!!
            MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
            break;

        default:
            break;
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_OK:
        GUIMENU_GetId(MMICL_MAIN_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_DELETE_ALL_RECORDS:
            //�ر�ѯ�ʴ���
            MMIPUB_CloseQuerytWin(&query_win_id);
            MMIAPICC_ClearMissedCallNum();
            DeleteAllCallLog(s_cur_sel_log_list_type);
            //success!
            MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
            break;

        case ID_CL_TIMER_RESET:
            //�ر�ѯ�ʴ���
            MMIPUB_CloseQuerytWin(&query_win_id);
            MMIAPICL_SetDefaultCallTimeToNV();
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            break;

        default:
            break;
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        //�ر�ѯ�ʴ���
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
#endif
#endif
/*****************************************************************************/
//  Description : to handle enter missed,received and dialed calls menu
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
void MMICL_HandleEnterCallsMenu(
                                MMICL_CALL_TYPE_E call_type
                                )
{
#ifdef MMI_LDN_SUPPORT
    if (!MMICL_IsCallInfoReady())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);

        return;
    }
#endif

    s_cur_sel_log_list_type = call_type;

    //��˽����
    if (MMIAPISET_IsOpenPrivacyProtect( MMISET_PROTECT_CL ) && !MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID) && !MMIAPISET_IsValidatePrivacyWinOpen(MMISET_PROTECT_CL))
    {
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CL, MMIAPICL_OpenCLWindow);
    }
    else
    {
        MMIAPICL_OpenCLWindow();
    }
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : MMICL_CreateAllLogChildWin
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMICL_CreateAllLogChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMK_CreateChildWin(parent_win_id, (uint32*)MMICL_ALL_CHILD_WIN_TAB, (ADD_DATA)parent_win_id);
    SetAllLogFillter(MMICL_CALL_ALL);
}

/*****************************************************************************/
// 	Description : create calllog wintab for sms/mms to select receiver
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
PUBLIC void MMICL_CreateAllLogSelectChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMK_CreateChildWin(parent_win_id, (uint32*)MMICL_ALL_SELECT_CHILD_WIN_TAB, (ADD_DATA)parent_win_id);
    SetAllLogFillter(MMICL_CALL_ALL);
}

/*****************************************************************************/
// 	Description : MMICL_CreatePBDetailLogChildWin
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
PUBLIC void MMICL_CreatePBDetailLogChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMK_CreateChildWin(parent_win_id, (uint32*)MMICL_PBDETAIL_CALLLOG_WIN_TAB, (ADD_DATA)parent_win_id);
    SetAllLogFillter(MMICL_CALL_PB_NUMBER_MATCHED);
}


#endif
/*****************************************************************************/
// 	Description : MMIAPICL_OpenCLWindow
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenCLWindow(void)
{
    MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);//@samboo cr102623 20071207
    //close the window of call log list menu and detail win
    MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_REFUSED_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
    MMK_CloseWin(MMICL_QUERY_WIN_ID);
#ifdef CALL_TIMES_SUPPORT
    MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
#endif
   
    MMK_CreateParentWin((uint32 *)MMICL_LOG_LIST_WIN_TAB, PNULL);	
    MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_DIALED_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_RECEIVED_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_MISSED_CHILD_WIN_TAB, NULL);
#if defined(MMI_BLACKLIST_SUPPORT)
    MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_REFUSED_CHILD_WIN_TAB, NULL);
#endif

    switch (s_cur_sel_log_list_type)
    {
    case MMICL_CALL_DIALED:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_DIALED_CHILD_WIN_ID);
        break;
        
    case MMICL_CALL_RECEIVED:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_RECEIVED_CHILD_WIN_ID);
        break;
        
    case MMICL_CALL_MISSED:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_MISSED_CHILD_WIN_ID);
        break;
        
#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_CALL_REFUSED:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_REFUSED_CHILD_WIN_ID);
        break;		
#endif

    default:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_DIALED_CHILD_WIN_ID);
        break;
    }
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Discription: append spliter
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void AppendSplitListItem(	 MMI_CTRL_ID_T    ctrl_id, wchar first_letter)
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT;
    item_t.first_letter = first_letter;
    item_t.item_data_ptr = &item_data;
    item_t.item_state |=GUIITEM_STATE_SPLIT;
    item_t.user_data = 0xffff;

    GUILIST_SetSplitOnside(ctrl_id, TRUE);

    GUILIST_AppendItem( ctrl_id, &item_t );

}
#endif

/*****************************************************************************/
// 	Description : to set call log list item
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void SetLogListItem(void)
{
    uint16 sel_item_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_HANDLE_T    list_handle = PNULL;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2626_112_2_18_2_11_32_54,(uint8*)"");
        return;
    }
    
    //get the current selected item index
    //���绰��¼�б��Ѿ���ʱ������ˢ���б���Ҫ���¶�λ��ǰ��ѡ�е�item!
    
    list_handle = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
    
    sel_item_index = GUILIST_GetCurItemIndex(list_handle);
    
    //read logs of calls from NV
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    
    //load log list
    GUILIST_SetMaxItem(list_handle, MMICL_RECORD_MAX_NUM, FALSE );//max item 20

#ifdef MMI_PDA_SUPPORT
    CalllogPBData_InitMatchedItem();
    AppendLogListItem(list_handle, s_cur_sel_log_list_type, arraycall_info);
#if defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
         GUILIST_SetTextFont(list_handle, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
    }
#endif
#else
    AppendLogListItem(list_handle, s_cur_sel_log_list_type, arraycall_info);
#endif
    GUILIST_SetCurItemIndex(list_handle, sel_item_index);
    
    SCI_FREE(arraycall_info);
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : to set call log select list item
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL void SetLogSelectListItem(void)
{
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_HANDLE_T    list_handle = PNULL;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogSelectListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2667_112_2_18_2_11_32_55,(uint8*)"");
        return;
    }
    list_handle = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
    GUILIST_SetMaxItem(list_handle, MMICL_RECORD_MAX_NUM, FALSE );//max item 20
    //read logs of calls from NV
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    AppendLogListItemForMessage(list_handle, s_cur_sel_log_list_type, arraycall_info);
    
    SCI_FREE(arraycall_info);
}
#endif

/*****************************************************************************/
// 	Description : to set call log list item
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMICL_SetLogListItem(void)
{
    SetLogListItem();
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: WARNING: "GetCurrTypeNum" just get record num by type, only for pda project
//                 "GetRecordNumByType" not only get record num, but also assign arraycall_info
/*****************************************************************************/
LOCAL uint16 GetCurrTypeNum(MMICL_CALL_TYPE_E      call_type, MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info)
{
    uint16 i = 0;
    uint16 count = 0;

    if(arraycall_info != PNULL)
    {
        if(MMICL_CALL_ALL == call_type)
        {
            count = arraycall_info->record_num;
        }    
        else
        {
            for ( i = 0; i < arraycall_info->record_num; i++ )
            {
                if(call_type == arraycall_info->call_bcdinfo[i].call_type)
                {
                    count++;
                }
            }
        }
    }
    return count;    
}
#ifdef MMI_PDA_SUPPORT

#if 0
/*****************************************************************************/
// 	Description : to set call log list item for istyle,missed call
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL void IStyle_MissCallSetLogListItem(void)
{
    uint16 sel_item_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_CTRL_ID_T list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;

	if (MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle()) 
	{
		return;
	}
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2739_112_2_18_2_11_33_56,(uint8*)"");
        return;
    }
        
    //read logs of calls from NV
    MMICL_GetRecordInfo(MMICL_CALL_MISSED, arraycall_info);
    
    //load log list
    GUILIST_SetMaxItem(list_ctrl_id, MMICL_RECORD_MAX_NUM, FALSE );//max item 20
    GUILIST_RemoveAllItems(list_ctrl_id);
    AppendLogListItem(list_ctrl_id, MMICL_CALL_MISSED, arraycall_info);

    GUILIST_SetCurItemIndex(list_ctrl_id, sel_item_index);
    
    SCI_FREE(arraycall_info);

}

/*****************************************************************************/
// 	Description : load call log list into listbox for istyle
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL void Istyle_AppendLogListItem(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info
                             )
{
    uint8 i = 0;
    GUILIST_ITEM_T item_t                          = {0};
    GUILIST_ITEM_DATA_T item_data                       = {0};

    MMI_STRING_T name_number_str = {0};
    wchar name_number_wchar[MMIPB_NAME_MAX_LEN + 1]  ={0};     
    
    MMI_STRING_T sim_name_str  = {0};
    wchar sim_name_wstr[GUILIST_STRING_MAX_NUM + 1]={0};
    MMISET_SIM_NAME_T   sim_name = {0};

    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_STRING_T time_str       ={0};
    wchar wstr_time_str[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_IMAGE_ID_T	image_id = IMAGE_NULL;
    BOOLEAN     isUseNumberType = FALSE;

    MMICL_CALL_BCDINFO_T *callinfo=PNULL;

    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT | GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_ICON;				

    item_t.item_data_ptr = &item_data;

    //set list item
    if ( arraycall_info->record_num == 0
        #ifdef MMI_PDA_SUPPORT
        || (call_type != MMICL_CALL_ALL && 0 == GetCurrTypeNum( call_type,arraycall_info) )
        #endif
        )  
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        
        //set softkey
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_BLANK;
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
    else
    {
        for ( i = 0; i < arraycall_info->record_num; i++ )
        {
            callinfo = &arraycall_info->call_bcdinfo[i];

            if( !IsAddCurrentLogToList(call_type,callinfo))
            {
                continue;
            }
        //Append item
            //Init Text Contents     Name/Number, SimName, Date, Time
            SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));            
            name_number_str.wstr_ptr = name_number_wchar;
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
            SCI_MEMSET(sim_name_wstr,0,sizeof(sim_name_wstr));
            sim_name_str.wstr_ptr = sim_name_wstr;
            sim_name_str.wstr_len = 0;
#endif
            SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_str));            
            datetime_str.wstr_ptr = datetime_wstr;
            SCI_MEMSET(&time_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&wstr_time_str,0,sizeof(wstr_time_str));            
            time_str.wstr_ptr = wstr_time_str;

            //Add Image content
            image_id = GetCallTypeImageID(callinfo);
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data.image_id = image_id;


#ifndef MMI_MULTI_SIM_SYS_SINGLE    
            GetSimNameContent(sim_name_wstr,callinfo);
            sim_name_str.wstr_len = MMIAPICOM_Wstrlen(sim_name_wstr);
            
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].font_color_id=MMIAPISET_GetSimColorID(callinfo->dual_sys);
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].is_default =TRUE;
#endif                 
            
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_len =sim_name_str.wstr_len;
#else
            //Single don't need sim name
#endif

            //Add Contact name/or call number content
            if (GetNameOrNumberContent(name_number_wchar,callinfo,TRUE,FALSE))
            {	
    			name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
            }  

            //Add date and time content
            if (0<callinfo->call_start_time[callinfo->call_times -1])
            {
                GetDateContent(&datetime_str, callinfo,callinfo->call_times -1);
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;

                GetTimeContent(&time_str,callinfo,callinfo->call_times -1);
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
            }
            if (CalllogPBData_IsSelecting())
                {
                   if (CalllogPBData_GetIsSelectedByIndex(i))
                   {
                    item_t.item_state  |= GUIITEM_STATE_SELECTED;
                   }
                   else
                   {
                   item_t.item_state &= ~GUIITEM_STATE_SELECTED;
                   }
                   item_t.item_state &= ~GUIITEM_STATE_CONTENT_CHECK;

                }
                else
                {
                    item_t.item_state &= ~GUIITEM_STATE_SELECTED;
        		    item_data.item_content[CL_LIST_ITEM_ICON_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
        			item_data.item_content[CL_LIST_ITEM_ICON_INDEX].item_data.image_id = IMAGE_ISTYLE_MOREINFO_ICON;
                }
            item_t.user_data = (uint32)i;

            GUILIST_AppendItem( ctrl_id, &item_t );
        }
    }
}
#endif
#endif

/*****************************************************************************/
//  Description : ��ͨ��������ʾ�ӵ�WSTRING��ĩβ
//  Global resource dependence : 
//  Author:Sam.hua
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_AddCallTimesStr(MMI_STRING_T *in_str_id,uint16 call_times,MMI_STRING_T* out_strid,uint16 max_len)
{
    MMI_STRING_T    calltimes_strid={0};
    char calltimes_str[MAX_CALL_TIME_LEN]={0};
    wchar calltime_wstr[MAX_CALL_TIME_LEN+1]={0};
    uint16 calltimestr_len=0;

    MMI_STRING_T    input_strid={0};
    wchar input_wstr[GUILIST_STRING_MAX_NUM+1]={0};

    if((in_str_id ==PNULL)||(out_strid ==PNULL))
    {
        return FALSE;
    }
    //Check call_times and len
    if ((in_str_id->wstr_ptr == PNULL)/*||(call_times>MMICL_MAX_CALL_TIMES)*/
        ||(in_str_id->wstr_len+MAX_CALL_TIME_LEN>max_len))
    {
        return FALSE;
    }

    sprintf((char *)calltimes_str, "(%d)", call_times);
    calltimestr_len = strlen((char *)calltimes_str);
    MMI_STRNTOWSTR(calltime_wstr, MAX_CALL_TIME_LEN, (uint8 *)calltimes_str, MAX_CALL_TIME_LEN, MAX_CALL_TIME_LEN);
    calltimes_strid.wstr_ptr = calltime_wstr;
    calltimes_strid.wstr_len = MMIAPICOM_Wstrlen(calltime_wstr);

    SCI_MEMSET(input_wstr,0,sizeof(input_wstr));
    MMI_WSTRNCPY(input_wstr,max_len,(const wchar*)in_str_id->wstr_ptr,in_str_id->wstr_len,in_str_id->wstr_len);
    input_strid.wstr_ptr = input_wstr;
    input_strid.wstr_len = MMIAPICOM_Wstrlen(input_wstr);

    MMIAPICOM_MergeTwoString(&input_strid,&calltimes_strid,out_strid,max_len);

    return TRUE;
}

/*****************************************************************************/
// 	Description : ��ȡͨ����¼����ϵ���������ߺ�����ʾ
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: //isAddCalltimes   ��Ҫ���� Calltimes
          //isUseNumberType  ��Ҫת��Ϊ�������
/*****************************************************************************/
LOCAL BOOLEAN GetNameOrNumberContent(wchar *in_str,uint16 max_len,MMICL_CALL_BCDINFO_T *callinfo,
                                    BOOLEAN isAddCalltimes,BOOLEAN isUseNumberType)
{
    MMI_STRING_T namestrid = {0}; 
    wchar wchar_namestr[PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+ 1] = {0}; 
    MMI_STRING_T temp_strid={0};

    if ((in_str == PNULL)||(callinfo==PNULL))
    {
        return FALSE;
    }

    temp_strid.wstr_ptr = in_str;
    
    SCI_MEMSET(&namestrid,0,sizeof(MMI_STRING_T));
    SCI_MEMSET(wchar_namestr,0,sizeof(wchar_namestr));
    namestrid.wstr_ptr = wchar_namestr;
    
    if ((!callinfo->is_num_present) ||(callinfo->is_emergency_call))
    {
        MMI_STRING_T res_strid = {0}; 
        if (callinfo->is_emergency_call)
        {
            MMI_GetLabelTextByLang(TXT_KL_PSW_EMERGENCY_CALL, &res_strid);
        }    
        else
        {
            MMI_GetLabelTextByLang(TXT_UNKNOW_NUM, &res_strid);
        }               
        res_strid.wstr_len = MIN(GUILIST_STRING_MAX_NUM, res_strid.wstr_len);
#ifdef CALL_TIMES_SUPPORT        
        if ((callinfo->call_times > 1) &&(isAddCalltimes))
        {
            MMICL_AddCallTimesStr(&res_strid,callinfo->call_times,&temp_strid,GUILIST_STRING_MAX_NUM);
        }
        else
#endif        
        {
            MMI_WSTRNCPY(in_str, GUILIST_STRING_MAX_NUM, res_strid.wstr_ptr, res_strid.wstr_len, res_strid.wstr_len);
        }
    }
    else
    {
        BOOLEAN find_result=FALSE;
        uint8   tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
        uint16  tele_num_len = 0;
#ifdef CALL_TIMES_SUPPORT
        uint16 temp_str_len = 0;
#endif        
#if defined(MMI_PDA_SUPPORT) ||defined(CALL_TIMES_SUPPORT)
        uint8   temp_str[PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+2 ] = {0};
#endif
#ifdef MMI_PDA_SUPPORT   
        wchar   num_wstr[PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+2] ={0};
        MMI_STRING_T num_strid = {0};
#endif              
        
        SCI_MEMSET(tele_num, 0x00, sizeof(tele_num));
    
        //get number
        tele_num_len = MMICL_GenDispNumber(callinfo->call_type,
            callinfo->number_type,
            callinfo->number_len,
            callinfo->number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);

        find_result = MMIAPICL_GetNameByClInfo(*callinfo, &namestrid, FALSE);

        //no name in pb
        if ( !find_result || 0 == namestrid.wstr_len )
        {
#ifdef CALL_TIMES_SUPPORT
            if((callinfo->call_times > 1)&&(isAddCalltimes))
            {
                sprintf((char *)temp_str, "%s(%d)", tele_num, callinfo->call_times);
                temp_str_len = strlen((char *)temp_str) ;
                MMI_STRNTOWSTR(in_str,max_len,temp_str,temp_str_len,temp_str_len);
            }
            else
#endif
            {
                MMI_STRNTOWSTR(in_str,max_len,tele_num,tele_num_len,tele_num_len);
            }
        }
        else//there is name
        {
#ifdef MMI_PDA_SUPPORT   
             //  Use type+number replaced Contact Name
            if (isUseNumberType)
            {
                MMI_STRING_T    type_strid = {0};
                wchar   type_wstr[GUILIST_STRING_MAX_NUM+1]={0};


                SCI_MEMSET(type_wstr,0,sizeof(type_wstr));
                type_strid.wstr_ptr = type_wstr;
                type_strid.wstr_len = 0;
                if (Cl_GetNumberTypeStrByNumber(type_wstr,callinfo))
                {
                    //Clear Name string
                    SCI_MEMSET(wchar_namestr,0,sizeof(wchar_namestr));
                    namestrid.wstr_len = 0;
                    SCI_MEMSET(num_wstr,0,sizeof(num_wstr));
                    num_strid.wstr_ptr = num_wstr;

                    //generate ' : '+number string id
                    sprintf((char *)temp_str, " : %s", tele_num);
                    MMI_STRNTOWSTR(num_wstr, PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+2 , 
                                    (uint8 *)temp_str, PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+2 , 
                                    PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+2 );
                    type_strid.wstr_len = MMIAPICOM_Wstrlen(type_wstr);
                    num_strid.wstr_len = MMIAPICOM_Wstrlen(num_wstr);
                    //merge it 
                    MMIAPICOM_MergeTwoString(&type_strid,&num_strid,&namestrid,max_len);
                }
            }
#endif                
#ifdef CALL_TIMES_SUPPORT
            if((callinfo->call_times > 1) &&(isAddCalltimes))
            {
                MMICL_AddCallTimesStr(&namestrid,callinfo->call_times,&temp_strid,max_len);
            }
            else
#endif            
            {
                namestrid.wstr_len= MMIAPICOM_Wstrlen(namestrid.wstr_ptr);
                MMI_WSTRNCPY(in_str,max_len,(const wchar*)namestrid.wstr_ptr,
                            namestrid.wstr_len,namestrid.wstr_len);
            }
        }
    }

    return TRUE;

}

/*****************************************************************************/
// 	Description : ��ȡCall type �� ICON ID
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetCallTypeImageID(MMICL_CALL_BCDINFO_T *callinfo)
{
    uint32 offset = 0;
    if (callinfo ==PNULL)
        return PNULL;
        
    if (callinfo->is_emergency_call)
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())        //IPHONE set time and to blue color
        {
            return IMAGE_ISTYLE_CL_LIST_SOS_ICON;
        }
        else
#endif        
        {
            return IMAGE_CL_SOS_CALL_ICON;
        }

     }

#ifdef VT_SUPPORT
    if (CC_CALL_VIDEO==callinfo->isvideocall)
    {
        offset = MMICL_IMAGE_INC;
    }
#endif

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())        //IPHONE set time and to blue color
    {
        switch ( callinfo->call_type)
        {
            case MMICL_CALL_MISSED:
               return  (MMI_IMAGE_ID_T)(ISTYLE_IMAGE_CL_MISSED_LIST_ICON);
            case MMICL_CALL_RECEIVED:
               return  (MMI_IMAGE_ID_T)(ISTYLE_IMAGE_CL_RECEIVED_LIST_ICON);
            case MMICL_CALL_DIALED:
               return (MMI_IMAGE_ID_T)(ISTYLE_IMAGE_CL_DIALED_LIST_ICON);
#if defined(MMI_BLACKLIST_SUPPORT)
            case MMICL_CALL_REFUSED:
               return (MMI_IMAGE_ID_T)(ISTYLE_IMAGE_CL_REFUSEDLIST_ICON);
#endif
            default:
               //SCI_TRACE_LOW:"AppendLogListItem:the call_type %d is error!"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_3123_112_2_18_2_11_33_57,(uint8*)"d",callinfo->call_type);
               break;
        }
       return PNULL;

    }
    
#endif
   
    switch ( callinfo->call_type)
    {
        case MMICL_CALL_MISSED:
           return  (MMI_IMAGE_ID_T)(IMAGE_CL_MISSED_CALL_ICON+offset);
        case MMICL_CALL_RECEIVED:
           return  (MMI_IMAGE_ID_T)(IMAGE_CL_RECEIVED_CALL_ICON+offset);
        case MMICL_CALL_DIALED:
           return (MMI_IMAGE_ID_T)(IMAGE_CL_DIALED_CALL_ICON+offset);
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_CALL_REFUSED:
           return (MMI_IMAGE_ID_T)(IMAGE_CL_REFUSED_CALL_ICON+offset);
#endif
        default:
           //SCI_TRACE_LOW:"AppendLogListItem:the call_type %d is error!"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_3123_112_2_18_2_11_33_57,(uint8*)"d",callinfo->call_type);
           break;
    }
    return PNULL;
}

/*****************************************************************************/
// 	Description : ��ȡCall type �� ICON ID
//	Global resource dependence : 
//  Author:tao.xue
//	Note:  
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPICL_GetCallTypeImageID(BOOLEAN is_emergency_call, BOOLEAN is_video_call, MMICL_CALL_TYPE_E call_type)
{
    uint32 offset = 0;
        
    if (is_emergency_call)
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())        //IPHONE set time and to blue color
        {
            return IMAGE_ISTYLE_CL_LIST_SOS_ICON;
        }
        else
#endif        
        {
            return IMAGE_CL_SOS_CALL_ICON;
        }
    }

#ifdef VT_SUPPORT
    if (CC_CALL_VIDEO==is_video_call)
    {
        offset = MMICL_IMAGE_INC;
    }
#endif
   
    switch ( call_type)
    {
        case MMICL_CALL_MISSED:
           return  (MMI_IMAGE_ID_T)(IMAGE_CL_MISSED_CALL_ICON+offset);
        case MMICL_CALL_RECEIVED:
           return  (MMI_IMAGE_ID_T)(IMAGE_CL_RECEIVED_CALL_ICON+offset);
        case MMICL_CALL_DIALED:
           return (MMI_IMAGE_ID_T)(IMAGE_CL_DIALED_CALL_ICON+offset);
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_CALL_REFUSED:
           return (MMI_IMAGE_ID_T)(IMAGE_CL_REFUSED_CALL_ICON+offset);
#endif
        default:
           //SCI_TRACE_LOW:"MMIAPICL_GetCallTypeImageID:the call_type %d is error!"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_3162_112_2_18_2_11_33_58,(uint8*)"d",call_type);
           break;
    }
    return PNULL;
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE    
/*****************************************************************************/
// 	Description : ��ȡSIM_NAME WSTRING
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSimNameContent(wchar* in_str,uint16 max_len,MN_DUAL_SYS_E dual_sys)
{
    MMISET_SIM_NAME_T sim_name={0};
    if (MN_DUAL_SYS_MAX <= dual_sys)
    {
        return FALSE;
    }

    //Max len should less than defined      .........
    //Add sim name content and set its color if needed
    sim_name = MMIAPISET_GetSimName(dual_sys);

    MMI_WSTRNCPY(in_str,max_len,(const wchar*)sim_name.wstr_arr,sim_name.wstr_len,sim_name.wstr_len);
    return TRUE;
}
#endif

/*****************************************************************************/
// 	Description : ��ȡ������ʾ��WSTRING
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  ����/��������Դ�ִ���ʾ�������������� Mon/Day ��ʾ   ���������
/*****************************************************************************/
LOCAL BOOLEAN GetDateContent(MMI_STRING_T* in_str,uint16 max_len,
                MMICL_CALL_BCDINFO_T *callinfo,uint16 call_time_index,BOOLEAN isNeedYear)
{

    SCI_DATE_T sys_data = {0};
    MMI_TM_T tm = {0};
    uint32     cl_sec = 0;
    uint32     sys_sec = 0;
    uint8      date_str[20] = {0};
    MMI_STRING_T    res_str={0};

    if ((callinfo == PNULL) ||(in_str == PNULL)||(call_time_index>=MMICL_MAX_CALL_TIMES))
    {
        return FALSE;
    }
    if (in_str->wstr_ptr == PNULL)
    {
        return FALSE;
    }
#ifdef CALL_TIMES_SUPPORT    
    tm = MMIAPICOM_Second2Tm( callinfo->call_start_time[call_time_index]);
#else
    tm = MMIAPICOM_Second2Tm( callinfo->call_start_time);
#endif
    TM_GetSysDate(&sys_data);

    cl_sec =MMIAPICOM_Tm2Second(0,0,0,tm.tm_mday,tm.tm_mon,tm.tm_year);
    sys_sec =MMIAPICOM_Tm2Second(0,0,0,sys_data.mday,sys_data.mon,sys_data.year);

    if(cl_sec == sys_sec)
    {   
        MMI_GetLabelTextByLang(TXT_TODAY, &res_str);        //today
    }
    if (cl_sec < sys_sec)
    {
        if( (sys_sec - cl_sec) <= 60*60*24)
        {
            MMI_GetLabelTextByLang(TXT_YESTERDAY, &res_str);    //yesterday
        }
    }
    
    if (res_str.wstr_len >0)
    {
        MMI_WSTRNCPY(in_str->wstr_ptr,max_len,(const wchar*)res_str.wstr_ptr,
                        res_str.wstr_len,res_str.wstr_len);
    }
    else
    {
        MMIAPISET_FormatDateStrByDateEx(tm.tm_year, tm.tm_mon, tm.tm_mday,'/', date_str,20,isNeedYear);
        MMI_STRNTOWSTR(in_str->wstr_ptr,max_len,(const uint8*)date_str,(uint32)strlen((char*)date_str),(uint32)strlen((char*)date_str));
    }
    in_str->wstr_len = (uint16)MMIAPICOM_Wstrlen(in_str->wstr_ptr);
    return TRUE;
    

}
/*****************************************************************************/
//  Description : ��ȡʱ��������ʾ��WSTRING
//  Global resource dependence : 
//  Author:Sam.hua
//  Note:  �������ʱ�䣬������������
/*****************************************************************************/
LOCAL BOOLEAN GetTimeAndDateContent(MMI_STRING_T* in_str,uint16 max_len,MMICL_CALL_BCDINFO_T *callinfo,uint16 call_time_index)
{
    SCI_DATE_T sys_date = {0};
    MMI_TM_T tm = {0};
    uint8      time_str[10] = {0};
    uint32     cl_sec = 0;
    uint32     sys_sec = 0;
    MMI_STRING_T txt_time = {0};
    wchar time_wstr[10] = {0};

    if ((callinfo == PNULL) ||(in_str == PNULL)||(call_time_index>=MMICL_MAX_CALL_TIMES))
    {
        return FALSE;
    }
    if (in_str->wstr_ptr == PNULL)
    {
        return FALSE;
    }
#ifdef CALL_TIMES_SUPPORT
    tm = MMIAPICOM_Second2Tm( callinfo->call_start_time[call_time_index]);
#else
    tm = MMIAPICOM_Second2Tm( callinfo->call_start_time);
#endif
    TM_GetSysDate(&sys_date);

    cl_sec = MMIAPICOM_Tm2Second(0,0,0,tm.tm_mday,tm.tm_mon,tm.tm_year);
    sys_sec = MMIAPICOM_Tm2Second(0,0,0,sys_date.mday,sys_date.mon,sys_date.year);

    txt_time.wstr_ptr = time_wstr;
    MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min, time_str,10);
    MMI_STRNTOWSTR(txt_time.wstr_ptr,10,(const uint8*)time_str,
            strlen((char*)time_str),strlen((char*)time_str));
    txt_time.wstr_len = (uint16)MMIAPICOM_Wstrlen(txt_time.wstr_ptr);

    //fixed by bingjie.chen
    if(cl_sec == sys_sec)       //Today ,return Time_str
    {
        MMI_WSTRNCPY(in_str->wstr_ptr,max_len,txt_time.wstr_ptr,txt_time.wstr_len,txt_time.wstr_len);
    }
    else                        //use Date
    {
#ifdef CALENDAR_SUPPORT
        uint8 call_time_weeks_of_year = MMICALENDAR_GetWeekInYear(tm.tm_year,tm.tm_mon,tm.tm_mday);
        uint8 cur_date_weeks_of_year = MMICALENDAR_GetWeekInYear(sys_date.year,sys_date.mon,sys_date.mday);
#endif
        wchar space[] = {' ',0};
        wchar date_space_wstr[20] = {0};
        MMI_STRING_T txt_date_space = {0};
        MMI_STRING_T txt_date_time = {0};

        txt_date_space.wstr_ptr = date_space_wstr;
#ifdef CALENDAR_SUPPORT
        if(call_time_weeks_of_year == cur_date_weeks_of_year && tm.tm_year == sys_date.year)
        {
            //���������ջ�ȡ�ܼ�
            uint8 week_num = MMICALENDAR_GetWeekday(tm.tm_year,tm.tm_mon,tm.tm_mday);
            MMI_STRING_T weekday = {0};
            const MMI_TEXT_ID_T week_text_arr[WEEK_DAY_NUM] =
            {
                STR_CLK_SUNDAY,
                STR_CLK_MONDAY,
                STR_CLK_TUESDAY,
                STR_CLK_WEDNESDAY,
                STR_CLK_THURSDAY,
                STR_CLK_FRIDAY,
                STR_CLK_SATURDAY
            };

            MMI_GetLabelTextByLang(week_text_arr[week_num], &weekday);
            MMIAPICOM_Wstrncpy(txt_date_space.wstr_ptr,weekday.wstr_ptr,weekday.wstr_len);
        }
        else
#endif
        {
            BOOLEAN is_display_year = tm.tm_year == sys_date.year ? FALSE : TRUE;
            uint8      date_str[20] = {0};

            MMIAPISET_FormatDateStrByDateEx(tm.tm_year, tm.tm_mon, tm.tm_mday,'/', date_str,20,is_display_year);
            MMI_STRNTOWSTR(txt_date_space.wstr_ptr,20,(const uint8*)date_str,
                    (uint32)strlen((char*)date_str),(uint32)strlen((char*)date_str));
        }
        MMIAPICOM_Wstrcat(txt_date_space.wstr_ptr,&space);
        txt_date_space.wstr_len = MMIAPICOM_Wstrlen(txt_date_space.wstr_ptr);
        MMIAPICOM_CatTwoString(&txt_date_time,&txt_date_space,&txt_time);
        MMI_WSTRNCPY(in_str->wstr_ptr,max_len,txt_date_time.wstr_ptr,txt_date_time.wstr_len,txt_date_time.wstr_len);
        MMIAPICOM_DestroyString(txt_date_time);
    }

    in_str->wstr_len = (uint16)MMIAPICOM_Wstrlen(in_str->wstr_ptr);

    return TRUE;

}


/*****************************************************************************/
// 	Description : ��ȡʱ����ʾ��WSTRING( HOUR:MIN)
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTimeContent(MMI_STRING_T* in_str,uint16 max_len,MMICL_CALL_BCDINFO_T *callinfo,uint16 call_time_index)
{
    MMI_TM_T tm = {0};
    uint8   time_str[10] = {0};

    if ((callinfo == PNULL) ||(in_str == PNULL)||(call_time_index>=MMICL_MAX_CALL_TIMES))
    {
        return FALSE;
    }
    if (in_str->wstr_ptr == PNULL)
    {
        return FALSE;
    }
#ifdef CALL_TIMES_SUPPORT    
    tm = MMIAPICOM_Second2Tm(callinfo->call_start_time[call_time_index]);
#else
    tm = MMIAPICOM_Second2Tm(callinfo->call_start_time);
#endif

    MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min, time_str,10);

    MMI_STRNTOWSTR(in_str->wstr_ptr,max_len,(const uint8*)time_str,
                strlen((char*)time_str),strlen((char*)time_str));

    in_str->wstr_len = (uint16)MMIAPICOM_Wstrlen(in_str->wstr_ptr);
    return TRUE;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description :��ǰ�ļ�¼�Ƿ���Ҫ���뵽�б���
//	Global resource dependence : 
//  Author:sam.hua
//	Note: ����PDA�µ�PB�е�ͨ����¼�б�
/*****************************************************************************/
LOCAL BOOLEAN IsAddCurrentLogToList(MMICL_CALL_TYPE_E call_type,MMICL_CALL_BCDINFO_T *callinfo)
{
    if (callinfo==PNULL)
    {
        return FALSE;
    }

    if(call_type != MMICL_CALL_ALL &&  callinfo->call_type != call_type)
    {
        return FALSE;
    }
    if (call_type == MMICL_CALL_ALL &&MMICL_CALL_MATCHED!=GetAllLogFillter() &&MMICL_CALL_PB_NUMBER_MATCHED!=GetAllLogFillter())
    {
        if((MMICL_CALL_ALL!=GetAllLogFillter()) &&  (callinfo->call_type != GetAllLogFillter()))
        {
            return FALSE;
        }
    }
    
    if (call_type == MMICL_CALL_ALL && MMICL_CALL_PB_NUMBER_MATCHED==GetAllLogFillter())
    {
        int j=0;
        BOOLEAN is_sameNumber = FALSE;
        MMIPB_BCD_NUMBER_T test={0};
        MMIPB_BCD_NUMBER_T *pb_number = PNULL;

        test.number_len = callinfo->number_len;
        for (j=0;j<test.number_len;j++)
            test.number[j] = callinfo->number[j];
        
        for (j=0;j<MMIPB_MAX_NV_PHONE_NUM;j++)
        {
            pb_number = GetAllLogFillterPBNumber(j);
            if (pb_number == PNULL || pb_number->number_len == 0)
            {
                continue;
            }
            //Compare Number,if Not equal,continue
            if(0 == MMIAPIPB_BcdSmartCmp(&test,pb_number))
            {    
                is_sameNumber = TRUE;
                break;
            }
        }

        if (!is_sameNumber)
        {
            return FALSE;
        }
        CalllogPBData_AddMatchedItemCount();
     } 

    return TRUE;
}
#endif 

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : load call log list into listbox for sms/mms
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL void AppendLogListItemForMessage(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info
                             )
{
    uint8 i = 0;

    GUILIST_ITEM_T item_t                          = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data                       = {0};/*lint !e64*/

    MMI_STRING_T name_number_str = {0};
    wchar name_number_wchar[PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+ 1]  ={0};     
    
    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_IMAGE_ID_T	image_id = IMAGE_NULL;
    BOOLEAN     isUseNumberType = FALSE;

    MMICL_CALL_BCDINFO_T    *callinfo=PNULL;
    
    
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_CHECK_ICON;

    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT | GUIITEM_STATE_CONTENT_CHECK;

    item_t.item_data_ptr = &item_data;


    //set softkey
    if ( s_select_return_info.need_select_return )
    {
        item_data.softkey_id[0] = TXT_COMMON_OK;
    }
    else
    {
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_OPTION;
    }
    
    item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        
   
    //set list item
    if ( arraycall_info->record_num == 0
        || (call_type != MMICL_CALL_ALL && 0 == GetCurrTypeNum( call_type,arraycall_info) )
        )  
    {
        //return;
    }
    else
    {
        for ( i = 0; i < arraycall_info->record_num; i++ )
        {

            callinfo = &arraycall_info->call_bcdinfo[i];
            if(call_type != MMICL_CALL_ALL &&  callinfo->call_type != call_type)
            {
                continue;
            }
            if (call_type == MMICL_CALL_ALL &&MMICL_CALL_MATCHED!=GetAllLogFillter() &&MMICL_CALL_PB_NUMBER_MATCHED!=GetAllLogFillter())
            {
                if((MMICL_CALL_ALL!=GetAllLogFillter()) &&  (callinfo->call_type != GetAllLogFillter()))
                {
                    continue;
                }
            }


            
            //Init Text Contents     Name/Number, Date
            SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));            
            name_number_str.wstr_ptr = name_number_wchar;

            SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_wstr));            
            datetime_str.wstr_ptr = datetime_wstr;

            //Add Image content
            image_id = GetCallTypeImageID(callinfo);
            item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[1].item_data.image_id = image_id;


            //Add Contact name/or call number content
            if (GetNameOrNumberContent(name_number_wchar,PHONE_NUM_MAX_LEN+MAX_CALL_TIME_LEN,callinfo,TRUE,isUseNumberType))
            {	
				name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[0].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                item_data.item_content[0].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
            }  
#ifdef CALL_TIMES_SUPPORT
            if(callinfo->call_times>0)
            {
                //Add date and time content
                if (0<callinfo->call_start_time[callinfo->call_times -1])
                {
                    GetTimeAndDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,callinfo->call_times -1);
                    item_data.item_content[2].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[2].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                    item_data.item_content[2].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
                }
            }            
#else
            if (0<callinfo->call_start_time)
            {
                GetTimeAndDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,0);
                item_data.item_content[2].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[2].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[2].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
            }
#endif
            item_t.user_data = (uint32)i;
            GUILIST_AppendItem( ctrl_id, &item_t );
            
            if (MMICL_SelectedListIsValid(i))
            {
                GUILIST_SetSelectedItem(ctrl_id, i, TRUE);
            }
        }
    }
}

/*****************************************************************************/
//  Description : MMICC_CreateNewSoftKey
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMICC_CreateNewSoftKey(MMI_HANDLE_T win_handle, MMI_HANDLE_T form_id,
                            MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, 
                            MMI_TEXT_ID_T rightkey_id)
{
    GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
    GUIFORM_DYNA_CHILD_T child_data = {0};

    child_data.is_softkey = TRUE;
    child_data.guid=SPRD_GUI_SOFTKEY_ID;
    child_data.child_handle=MMICALLLOG_SOFTKEY_CTRL_ID;
    softkey_data.leftsoft_id = leftkey_id;
    softkey_data.midsoft_id = midkey_id;
    softkey_data.rightsoft_id = rightkey_id;
    child_data.init_data_ptr=&softkey_data;
    GUIFORM_CreatDynaChildCtrl(win_handle,form_id,&child_data);
    return TRUE;
}

#endif

/*****************************************************************************/
// 	Description : load call log list into listbox
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void AppendLogListItem(
                             MMI_CTRL_ID_T                  ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info
                             )
{
    uint8 i = 0;

    GUILIST_ITEM_T item_t                          = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data                       = {0};/*lint !e64*/

    MMI_STRING_T name_number_str = {0};
    wchar name_number_wchar[PHONE_NUM_MAX_LEN + 1+MAX_CALL_TIME_LEN]  ={0};     

#ifndef MMI_MULTI_SIM_SYS_SINGLE    
    MMI_STRING_T sim_name_str  = {0};
    wchar sim_name_wstr[GUILIST_STRING_MAX_NUM + 1]={0};

#endif

    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_STRING_T time_str       ={0};
    wchar wstr_time_str[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_IMAGE_ID_T	image_id = IMAGE_NULL;
    BOOLEAN     isUseNumberType = FALSE;

    MMICL_CALL_BCDINFO_T    *callinfo=PNULL;

#ifdef HOME_LOCATION_INQUIRE_SUPPORT
//    uint16  city_ucs2_str[MMIFINDMP_CITYNAME_LENGTH + 1] = {0};
    uint16  str_length = 0;
    uint8   origin_number_arr[CC_MAX_TELE_NUM_LEN + 2] = {0};
    int16   origin_num_len  = 0;
    MMI_STRING_T location_str       ={0};
    wchar location_wstr[MMIFINDMP_CITYNAME_LENGTH + 1] = {0};
#endif


#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_ICON;
    }
    else
#endif
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;
    }
#else
#ifdef TOUCH_PANEL_SUPPORT            
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;
#else
#ifndef MMI_MULTI_SIM_SYS_SINGLE
//fix by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;    
#else
    item_t.item_style = GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;
#endif
#else
#ifdef MMI_CL_MINI_SUPPORT
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;    
#else
    item_t.item_style = GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT;
#endif

#endif
#endif
#endif

    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT | GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;

    //set softkey
    item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;

    if ( s_select_return_info.need_select_return )
    {
        item_data.softkey_id[0] = TXT_COMMON_OK;
    }
    else
    {
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
        if (isMultiSelecting())
        {
            item_data.softkey_id[0] = TXT_COMMON_OK;
            item_data.softkey_id[1] = (MMI_TEXT_ID_T)STXT_SOFTKEY_MARK_MK;
        }
        else
#endif            
        {
            item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_OPTION;
        }
    }

    //set list item
    if ( arraycall_info->record_num == 0
#ifdef MMI_PDA_SUPPORT
        || (call_type != MMICL_CALL_ALL && 0 == GetCurrTypeNum( call_type,arraycall_info) )
#endif
        )  
    {
#ifndef MMI_PDA_SUPPORT
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        
        //set softkey
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_BLANK;
        GUILIST_AppendItem( ctrl_id, &item_t );
#endif
    }
    else
    {
        if (call_type == MMICL_CALL_ALL && MMICL_CALL_PB_NUMBER_MATCHED==GetAllLogFillter())
        {
            isUseNumberType = TRUE;
        }
    
        for ( i = 0; i < arraycall_info->record_num; i++ )
        {

            callinfo = &(arraycall_info->call_bcdinfo[i]);
#ifdef MMI_PDA_SUPPORT
            //Check current log should be added to list or not
            if(!IsAddCurrentLogToList(call_type,callinfo))
            {   
                continue;
            }    
            
#endif
            //Init Text Contents     Name/Number, SimName, Date, Time
            SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));            
            name_number_str.wstr_ptr = name_number_wchar;
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
            SCI_MEMSET(sim_name_wstr,0,sizeof(sim_name_wstr));
            sim_name_str.wstr_ptr = sim_name_wstr;
            sim_name_str.wstr_len = 0;
#endif
            SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_wstr));            
            datetime_str.wstr_ptr = datetime_wstr;
            SCI_MEMSET(&time_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&wstr_time_str,0,sizeof(wstr_time_str));            
            time_str.wstr_ptr = wstr_time_str;

            //Add Image content
            image_id = GetCallTypeImageID(callinfo);
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data.image_id = image_id;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
            if (!callinfo->is_emergency_call)
            {
                MMIAPICL_GetSimNameContent(sim_name_wstr,GUILIST_STRING_MAX_NUM,callinfo->dual_sys);
            }
            sim_name_str.wstr_len = MMIAPICOM_Wstrlen(sim_name_wstr);
            
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].font_color_id=MMIAPISET_GetSimColorID(callinfo->dual_sys);
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].is_default =TRUE;
#endif                 
#ifdef MMI_CL_MINI_SUPPORT
                item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[2].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
                item_data.item_content[2].item_data.text_buffer.wstr_len =sim_name_str.wstr_len;
#else            
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_len =sim_name_str.wstr_len;
#endif    
#else
            //Single don't need sim name
#endif

            //Add Contact name/or call number content
            if (GetNameOrNumberContent(name_number_wchar,PHONE_NUM_MAX_LEN+MAX_CALL_TIME_LEN,callinfo,TRUE,isUseNumberType))
            {
                name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
//fix by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
                item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[1].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                item_data.item_content[1].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
#else
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
#endif
            }

#ifdef TOUCH_PANEL_SUPPORT
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
            SCI_MEMSET(&location_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&location_wstr,0,sizeof(location_wstr));       
            location_str.wstr_ptr = location_wstr;

            if (callinfo->is_num_present)//�绰���벻����
            {
                uint8 origin_number_str[CC_MAX_TELE_NUM_LEN + 2] = {0};
//                uint8 tel_number_str[CC_MAX_TELE_NUM_LEN + 2] = {0};

                MMICL_GenDispNumber(callinfo->call_type,
                                    callinfo->number_type,
                                    callinfo->number_len,
                                    callinfo->number, 
                                    origin_number_str,
                                    CC_MAX_TELE_NUM_LEN + 2);

                if(MMIAPICC_RemoveIpNumber(origin_number_str, strlen((char*)origin_number_str), origin_number_arr,CC_MAX_TELE_NUM_LEN))
                {
                    origin_num_len = strlen((char*)origin_number_arr);
                }
                else
                {
                    SCI_MEMCPY(origin_number_arr,origin_number_str,CC_MAX_TELE_NUM_LEN+2);
                    origin_num_len = strlen((char*)origin_number_arr);
                }
                
                str_length = MMIAPIPHONESEARCH_FindLocation(origin_number_arr, origin_num_len, (uint16*)location_wstr);
            }
            else
            {
                str_length = 0;
            }
            if( str_length == 0 )
            {
                MMI_GetLabelTextByLang(TXT_UNKNOWN, &location_str);
            }   
            else if( str_length == MMIPHONESEARCH_LOCALNUMBER )
            {
                MMI_GetLabelTextByLang(TXT_LOCAL, &location_str);
            }   
            else
            {
                location_str.wstr_ptr = location_wstr;
                //location_str.is_ucs2 = TRUE;
                location_str.wstr_len  = str_length;
            }
            item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = location_str.wstr_len;
            item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = location_str.wstr_ptr;


#endif
#endif
#ifdef CALL_TIMES_SUPPORT            
            if (callinfo->call_times>0)
            {
                //Add date and time content
                if (0<callinfo->call_start_time[callinfo->call_times -1])
                {
/*            
                GetDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,0,FALSE);
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
*/
                //GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
#ifdef MMI_CL_MINI_SUPPORT
                GetTimeAndDateContent(&time_str, GUILIST_STRING_MAX_NUM, callinfo, callinfo->call_times - 1);
                item_data.item_content[4].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[4].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[4].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr; 
#else          
                GetTimeAndDateContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,callinfo->call_times -1);
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr; 
#endif
                }

            }    
#else
                //Add date and time content
            if (0<callinfo->call_start_time)
            {
/*            
                GetDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,0,FALSE);
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
*/
                //GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
#ifdef MMI_CL_MINI_SUPPORT
                GetTimeAndDateContent(&time_str, GUILIST_STRING_MAX_NUM, callinfo, 0);
                item_data.item_content[4].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[4].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[4].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr; 
#else                
                GetTimeAndDateContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
            }
#endif
#endif
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
            if (MMITHEME_IsIstyle())        //IPHONE set time and to blue color
            {
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].font_color_id=MMITHEME_COLOR_BLUE;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].is_default =TRUE;
            }
#endif

            //Add selected content or icon content
            if (CalllogPBData_IsSelecting())
            {
                if (CalllogPBData_GetIsSelectedByIndex(i))
                {
                    item_t.item_state  |= GUIITEM_STATE_SELECTED;
                }
                else
                {
                    item_t.item_state &= ~GUIITEM_STATE_SELECTED;
                }
                item_t.item_state &= ~GUIITEM_STATE_CONTENT_CHECK;
            }
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
            else if (isMultiSelecting())
            {
                if (isCurrentSelected(i))
                {
                    item_t.item_state  |= GUIITEM_STATE_SELECTED;
                }
                else
                {
                    item_t.item_state &= ~GUIITEM_STATE_SELECTED;
                }
                item_t.item_state &= ~GUIITEM_STATE_CONTENT_CHECK;

            }
#endif
            else
            {
#ifdef TOUCH_PANEL_SUPPORT            
                item_t.item_state &= ~GUIITEM_STATE_SELECTED;
                item_data.item_content[CL_LIST_ITEM_ICON_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
                if (MMITHEME_IsIstyle())
                {
                    item_data.item_content[CL_LIST_ITEM_ICON_INDEX].item_data.image_id = IMAGE_ISTYLE_MOREINFO_ICON;
                }
                else
#endif                
                {
                    item_data.item_content[CL_LIST_ITEM_ICON_INDEX].item_data.image_id = IMAGE_CL_DIAL_ICON;
                }
#endif            
            }
#ifdef MMI_CL_MINI_SUPPORT

                if(s_select_info.is_select)
            
                {
                    if(s_select_info.log_selected_index[i])
                    {
                    item_data.item_content[5].item_data_type = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[5].item_data.image_id = IMAGE_MARKED_ICON;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
                    item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_CANCEL_ICON;
#else
                    item_data.softkey_id[1] = (MMI_TEXT_ID_T)STXT_CANCEL;
#endif
                    } 
                    else
                    {
                    item_data.item_content[5].item_data_type = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[5].item_data.image_id = IMAGE_NULL;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
                    item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_SELECTED_ICON;
#else
                    item_data.softkey_id[1] = (MMI_TEXT_ID_T)STXT_SOFTKEY_MARK_MK;
#endif
                    }
                }
                else
                {
                    item_data.item_content[5].item_data_type = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[5].item_data.image_id = IMAGE_NULL;
                }
                      
#endif
            item_t.user_data = (uint32)i;
            GUILIST_AppendItem( ctrl_id, &item_t );

        }
    }
}

/*****************************************************************************/
// 	Description : make a Mo call by record index
//	Global resource dependence : 
//  Author:Kelly.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_MakeCallByRecordIndex(
                                        MMICL_CALL_TYPE_E	call_type,
                                        uint16				record_index
                                        )
{
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;
	 
    #if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
    #endif
    MMI_STRING_T*  name_ptr = PNULL;
#if defined(CL_SAVE_NAME_SUPPORT)
    MMI_STRING_T  call_name = {0};
    wchar temp_str[MMICL_NAME_MAX_LEN +1] = {0};
#endif

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"MMIAPICL_MakeCallByRecordIndex call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_3814_112_2_18_2_11_35_59,(uint8*)"");
        return;
    }
    
    MMICL_GetRecordInfo(call_type, arraycall_info);
    
    #ifdef MMI_PDA_SUPPORT
    if(call_type != MMICL_CALL_ALL) //list �������ô˷���
    {  
        uint8 i = 0;
        uint8 count = 0;
        for(i = 0; i<arraycall_info->record_num ; i++)
        {
            if(arraycall_info->call_bcdinfo[i].call_type == call_type)
            {
                count++;
                if((record_index + 1) == count)   //��all���ҵ�λ��
                {
                     record_index = i;
                     break;
                }
            }
        }
    }
    #endif

    #ifdef MMI_PDA_SUPPORT
    if ((0 != GetCurrTypeNum( call_type,arraycall_info))&& arraycall_info->call_bcdinfo[record_index].is_num_present)//�绰�������
    #else
    if ((arraycall_info->record_num != 0)&& arraycall_info->call_bcdinfo[record_index].is_num_present)//�绰�������
    #endif
    {
        CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_GetCallSimType(arraycall_info->call_bcdinfo[record_index].dual_sys);
	
        //get number
        MMICL_GenDispNumber(arraycall_info->call_bcdinfo[record_index].call_type,
            arraycall_info->call_bcdinfo[record_index].number_type,
            arraycall_info->call_bcdinfo[record_index].number_len,
            arraycall_info->call_bcdinfo[record_index].number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);
        
        num_len = (uint8)strlen((char*)tele_num);
        
        //call
        if ( num_len > 0 )
        {
#if defined(CL_SAVE_NAME_SUPPORT)
            if (arraycall_info->call_bcdinfo[record_index].name.is_use_nv_name)
            {
                call_name.wstr_ptr = temp_str;
                call_name.wstr_len = arraycall_info->call_bcdinfo[record_index].name.wstr_len;
                MMI_WSTRNCPY(call_name.wstr_ptr,
                                            MMICL_NAME_MAX_LEN, 
                                            arraycall_info->call_bcdinfo[record_index].name.wstr, 
                                            MMICL_NAME_MAX_LEN, 
                                            call_name.wstr_len);
                name_ptr = &call_name;
            }
            else
            {
                name_ptr = PNULL;
            }
#endif

            #if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
            cc_result = MMIAPICC_MakeCall(
                                            arraycall_info->call_bcdinfo[record_index].dual_sys,
                                            tele_num,
                                            num_len,
                                            PNULL,
                                            name_ptr,
                                            sim_type,
                                            CC_CALL_NORMAL_CALL,
                                            PNULL
                                            );
            if(CC_RESULT_SUCCESS == cc_result)
            {
               MMIAPIBT_SetCallSuccess();
            }
            else
            {
               MMIAPIBT_SetCallFail();
            }
            #else
            MMIAPICC_MakeCall(
                                            arraycall_info->call_bcdinfo[record_index].dual_sys,
                                            tele_num,
                                            num_len,
                                            PNULL,
                                            name_ptr,
                                            sim_type,
                                            CC_CALL_NORMAL_CALL,
                                            PNULL
                                            );

            #endif
        }
    }
    else
    {
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
        MMIAPIBT_SetCallFail(); //mdy by qing.yu@for cr238786
#endif
        //SCI_TRACE_LOW:"MMIAPICL_MakeCallByRecordIndex: the call number is not exited, arraycall_info->record_num is = %d,is num present = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_3917_112_2_18_2_11_35_60,(uint8*)"dd",arraycall_info->record_num,arraycall_info->call_bcdinfo[record_index].is_num_present);
    }
    
    SCI_FREE(arraycall_info);

}

/*****************************************************************************/
// 	Description : add all tabs and all child windows to parent window.
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL void  AddTabsAndChildWindows(void)
{
    MMI_STRING_T    str_text = {0};
#if defined(MMI_BLACKLIST_SUPPORT)
    MMI_IMAGE_ID_T cl_pda_tab[4][2] = 
#else
    MMI_IMAGE_ID_T cl_pda_tab[3][2] = 
#endif
        {{IMAGE_CL_MISSED_SELECTED, IMAGE_CL_MISSED_RELEASED},
        {IMAGE_CL_DIALED_SELECTED, IMAGE_CL_DIALED_RELEASED},
        {IMAGE_CL_RECEIVED_SELECTED, IMAGE_CL_RECEIVED_RELEASED},
#if defined(MMI_BLACKLIST_SUPPORT)
        {IMAGE_CL_REFUSED_SELECTED, IMAGE_CL_REFUSED_RELEASED}
#endif
        };

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
#if defined(MMI_BLACKLIST_SUPPORT)
    MMI_IMAGE_ID_T istyle_cl_pda_tab[4][2] = 
#else
    MMI_IMAGE_ID_T istyle_cl_pda_tab[4][2] = 
#endif
        {{ISTYLE_IMAGE_CL_MISSED_SELECTED, ISTYLE_IMAGE_CL_MISSED_RELEASED},
        {ISTYLE_IMAGE_CL_DIALED_SELECTED, ISTYLE_IMAGE_CL_DIALED_RELEASED},
        {ISTYLE_IMAGE_CL_RECEIVED_SELECTED, ISTYLE_IMAGE_CL_RECEIVED_RELEASED},
#if defined(MMI_BLACKLIST_SUPPORT)
        {ISTYLE_IMAGE_CL_REFUSED_SELECTED, ISTYLE_IMAGE_CL_REFUSED_RELEASED}
#endif
    };
#endif

    MMI_IMAGE_ID_T (*pda_tab_ptr)[2] = {PNULL};

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        pda_tab_ptr = istyle_cl_pda_tab;
    }
    else
#endif
    {
        pda_tab_ptr = cl_pda_tab;
    }
    
    GUITAB_SetMaxItemNum(MMICL_LOG_TAB_CTRL_ID, 4);

    MMI_GetLabelTextByLang(TXT_CL_MISSED_CALLS_SHORT, &str_text);
    GUITAB_AppendSimpleItem(
        MMICL_LOG_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[0][0],
        pda_tab_ptr[0][1]);
    
    GUITAB_AddChildWindow(
        MMICL_LOG_TAB_CTRL_ID,
        MMICL_MISSED_CHILD_WIN_ID,
        0
        );

   MMI_GetLabelTextByLang(TXT_CL_DIALED_CALLS_SHORT, &str_text);
    GUITAB_AppendSimpleItem(
       MMICL_LOG_TAB_CTRL_ID,
       &str_text,
        pda_tab_ptr[1][0],
        pda_tab_ptr[1][1]);
   
   GUITAB_AddChildWindow(
       MMICL_LOG_TAB_CTRL_ID,
       MMICL_DIALED_CHILD_WIN_ID, 
       1
       );
   
   MMI_GetLabelTextByLang(TXT_CL_RECEIVED_CALLS_SHORT, &str_text);
   GUITAB_AppendSimpleItem(
       MMICL_LOG_TAB_CTRL_ID,
       &str_text,
        pda_tab_ptr[2][0],
        pda_tab_ptr[2][1]);
   
   GUITAB_AddChildWindow(
       MMICL_LOG_TAB_CTRL_ID,
       MMICL_RECEIVED_CHILD_WIN_ID, 
       2
       );
   
#if defined(MMI_BLACKLIST_SUPPORT)
   MMI_GetLabelTextByLang(TXT_CL_REFUSED_CALLS_SHORT, &str_text);
   GUITAB_AppendSimpleItem(
       MMICL_LOG_TAB_CTRL_ID,
       &str_text,
        pda_tab_ptr[3][0],
        pda_tab_ptr[3][1]);
   
   GUITAB_AddChildWindow(
       MMICL_LOG_TAB_CTRL_ID,
       MMICL_REFUSED_CHILD_WIN_ID, 
       3
       );
#endif

   switch (s_cur_sel_log_list_type)
    {
    case MMICL_CALL_MISSED:
        GUITAB_SetCurSel(MMICL_LOG_TAB_CTRL_ID, 0);
        break;
        
    case MMICL_CALL_DIALED:
        GUITAB_SetCurSel(MMICL_LOG_TAB_CTRL_ID, 1);
        break;
        
    case MMICL_CALL_RECEIVED:
        GUITAB_SetCurSel(MMICL_LOG_TAB_CTRL_ID, 2);
        break;
        
#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_CALL_REFUSED:
        GUITAB_SetCurSel(MMICL_LOG_TAB_CTRL_ID, 3);
        break;
#endif
        
    default:
        GUITAB_SetCurSel(MMICL_LOG_TAB_CTRL_ID, 0);
        break;
    }
}

/*****************************************************************************/
// 	Description : to handle tab parent window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLogListWindow(
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 cur_select = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMICL_LOG_TAB_CTRL_ID);
        AddTabsAndChildWindows();
        break;

    case MSG_NOTIFY_TAB_SWITCH:
#ifdef MMI_CL_MINI_SUPPORT
         ExitSelect();
#endif
        cur_select = GUITAB_GetCurSel(MMICL_LOG_TAB_CTRL_ID);

        switch (cur_select)
        {
        case 0:
            s_cur_sel_log_list_type = MMICL_CALL_MISSED;
            break;

        case 1:
            s_cur_sel_log_list_type = MMICL_CALL_DIALED;
            break;

        case 2:
            s_cur_sel_log_list_type = MMICL_CALL_RECEIVED;
            break;

#if defined(MMI_BLACKLIST_SUPPORT)
        case 3:
            s_cur_sel_log_list_type = MMICL_CALL_REFUSED;
            break;
#endif

        default:
            s_cur_sel_log_list_type = MMICL_CALL_MISSED;
            break;
        }
        MMK_PostMsg(MMICL_MAIN_MENU_WIN_ID, MMI_CL_MAINMENU_UPDATE_INDEX, &cur_select, sizeof(cur_select));
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#ifdef MMI_CL_TOOLBAR_SUPPORT
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : handle toolbar item sel 
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_HandleToolBarItemSel(CL_TOOLBAR_TYPE_E node_id)
{
    switch(node_id)
    {
        case     CL_TOOLBAR_DIAL_AUDIO:
            HandleRingBackByType(CC_CALL_NORMAL_CALL);
            break;

        #ifdef VT_SUPPORT
        case     CL_TOOLBAR_DIAL_VIDEO:        
            HandleRingBackByType(CC_CALL_VIDEO_CALL);            
            break;
        #endif
#ifdef MMI_IP_CALL_SUPPORT
        case     CL_TOOLBAR_DIAL_IPCALL:        
            HandleRingBackByType(CC_CALL_IP_CALL);            
            break;
#endif
        case     CL_TOOLBAR_DIAL_SMS:
            HandleAnswerSms();    
            break;
#ifdef CALL_TIMES_SUPPORT
        case     CL_TOOLBAR_DIAL_VIEW:
            MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_WIN_TAB, PNULL);
            break;
#endif       
        default:
            break;

    }           
}

/*****************************************************************************/
//  Description : Update EntryList Toolbar State 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMICL_CreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_CTRL_ID_T list_ctrl_id)
{ 
#ifdef GUIF_TOOLBAR
    MMICL_CALL_TYPE_E call_type = MMICL_CALL_TYPE_MAX;
    GUITOOLBAR_INIT_DATA_T init_data = {0};
    GUITOOLBAR_ITEM_T cl_toolbar_item_a [] = 
    {
        {CL_TOOLBAR_DIAL_AUDIO, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_CALL, IMAGE_TOOLBAR_CL_CALL_GRAY,0},/*lint !e785*/
        #ifdef  VT_SUPPORT
        {CL_TOOLBAR_DIAL_VIDEO, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_VIDEOCALL, IMAGE_TOOLBAR_CL_VIDEOCALL,0},/*lint !e785*/
        #endif
#ifdef MMI_IP_CALL_SUPPORT
        {CL_TOOLBAR_DIAL_IPCALL, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_IPCALL, IMAGE_TOOLBAR_CL_IPCALL_GRAY,0},/*lint !e785*/
#endif
        {CL_TOOLBAR_DIAL_SMS, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_WRITESMS, IMAGE_TOOLBAR_CL_WRITESMS_GRAY,0},/*lint !e785*/
    };  

    switch(list_ctrl_id)
    {
    case MMICL_DIALED_LIST_CTRL_ID:
        call_type = MMICL_CALL_DIALED;
        break;
        
    case MMICL_RECEIVED_LIST_CTRL_ID:
        call_type = MMICL_CALL_RECEIVED;
        break;
    
    case MMICL_MISSED_LIST_CTRL_ID:
        call_type = MMICL_CALL_MISSED;
        break;
    
#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_REFUSED_LIST_CTRL_ID:
        call_type = MMICL_CALL_REFUSED;
        break;
#endif
    
    case MMICL_ALL_LIST_CTRL_ID:
        call_type = MMICL_CALL_ALL;
        break;
        
    default:
        return;     
    }
           
    MMK_GetBothRect(win_id, &init_data.both_rect);
    
    GUITOOLBAR_CreateDynamic(win_id, ctrl_id, &init_data);
    GUITOOLBAR_SetAllItem(MMK_GetCtrlHandleByWin(win_id, ctrl_id), 
        cl_toolbar_item_a, 
        sizeof(cl_toolbar_item_a)/sizeof(GUITOOLBAR_ITEM_T));
#endif        
}
/*****************************************************************************/
// 	Description : update cl toolbar
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_UpdateCLWinToolbar(
									MMI_WIN_ID_T      win_id,
									MMI_CTRL_ID_T       list_ctrl_id                                                                       
									)
{
    uint16  icon_width = 0;
    uint16  icon_height = 0;
    uint16  cur_index = 0;
    GUI_RECT_T cur_rect = {0};
    GUI_POINT_T pt_bar = {0};
    MMI_CTRL_ID_T       toolbar_ctrl_id = {0};
    //MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMICL_CALL_TYPE_E call_type = MMICL_CALL_TYPE_MAX;
    MMI_HANDLE_T   toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, list_ctrl_id);//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);
    MMI_HANDLE_T    list_handle;
    
    switch(list_ctrl_id)
    {
        case MMICL_DIALED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_DIALED_CTRL_ID;
            call_type = MMICL_CALL_DIALED;
            break;

        case MMICL_RECEIVED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_RECEIVED_CTRL_ID;
            call_type = MMICL_CALL_RECEIVED;
            break;
        
        case MMICL_MISSED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_MISSED_CTRL_ID;
            call_type = MMICL_CALL_MISSED;
            break;
        
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_REFUSED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_REFUSED_CTRL_ID;
            call_type = MMICL_CALL_REFUSED;
            break;
#endif
        
     case MMICL_ALL_LIST_CTRL_ID:
            toolbar_ctrl_id = MMICL_TOOLBAR_ALLLIST_CTRL_ID;
            call_type = MMICL_CALL_ALL;
            break;       
        default:              
            return;     
    }
    toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, toolbar_ctrl_id);
    list_handle = MMK_GetCtrlHandleByWin(win_id, list_ctrl_id);

    GUITOOLBAR_SetVisible(toolbar_ctrl_handle,TRUE);

    cur_index = GUILIST_GetCurItemIndex(list_handle);	
    GUILIST_GetItemRect(list_handle, cur_index, &cur_rect);
    GUITOOLBAR_GetIconWidthHeight(toolbar_ctrl_handle, &icon_width, &icon_height);
    
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        pt_bar.x = cur_rect.right;
    }
    else
    {
        pt_bar.x = cur_rect.left;
    }
    
    if (0 != cur_index)
    {
        pt_bar.y = cur_rect.bottom - MMICL_TOOLBAR_PANEL_GAP + 1; //����
        GUITOOLBAR_SetPanelPopForm(toolbar_ctrl_handle, GUITOOLBAR_DIR_DOWN);
    }
    else
    {
        pt_bar.y = cur_rect.bottom + MMICL_TOOLBAR_PANEL_GAP - 1; //����
        GUITOOLBAR_SetPanelPopForm(toolbar_ctrl_handle, GUITOOLBAR_DIR_UP);
    }

    if (GUITOOLBAR_SetPanelBasePt(toolbar_ctrl_handle, &pt_bar, FALSE))
    {
		GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_SHRINK );
        GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_EXPAND );
    }
    else        //�����ͬ��PTR,��᷵��FALSE
    {
        if(GUITOOLBAR_GetShowType(toolbar_ctrl_handle)==GUITOOLBAR_EXPAND)
        {
            GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_SHRINK );
        }
        else
        {
            GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_EXPAND );
        }
    }

    
}

#else
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : handle toolbar item sel 
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_HandleToolBarItemSel(CL_TOOLBAR_TYPE_E node_id)
{
    switch(node_id)
    {
        case     CL_TOOLBAR_DIAL_AUDIO:
            HandleRingBack(CC_CALL_SIM_MAX, FALSE);             //info_ptr can be null
            break;
#ifdef VT_SUPPORT
        case     CL_TOOLBAR_DIAL_VIDEO:        
            HandleRingBack(CC_CALL_SIM_MAX, TRUE);            
            break;
#endif
#ifdef MMI_IP_CALL_SUPPORT
        case     CL_TOOLBAR_DIAL_IPCALL:        
            HandleIpCall();                
            break;
#endif
        case     CL_TOOLBAR_DIAL_SMS:
            HandleAnswerSms();    
            break;
        default:
            break;
    }           
}

/*****************************************************************************/
//  Description : Update EntryList Toolbar State 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMICL_CreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_CTRL_ID_T list_ctrl_id)
{    
    MMICL_CALL_TYPE_E call_type = MMICL_CALL_TYPE_MAX;
    GUITOOLBAR_INIT_DATA_T init_data = {0};
    GUITOOLBAR_ITEM_T cl_toolbar_item_a [] = 
    {
        {CL_TOOLBAR_DIAL_AUDIO, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_CALL, IMAGE_TOOLBAR_CL_CALL_GRAY,0},/*lint !e785*/
        #ifdef  VT_SUPPORT
        {CL_TOOLBAR_DIAL_VIDEO, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_VIDEOCALL, IMAGE_TOOLBAR_CL_VIDEOCALL_GRAY,0},/*lint !e785*/
        #endif
        #ifdef MMI_IP_CALL_SUPPORT
        {CL_TOOLBAR_DIAL_IPCALL, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_IPCALL, IMAGE_TOOLBAR_CL_IPCALL_GRAY,0},/*lint !e785*/
        #endif
        {CL_TOOLBAR_DIAL_SMS, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_WRITESMS, IMAGE_TOOLBAR_CL_WRITESMS_GRAY,0},/*lint !e785*/
    };  

    switch(list_ctrl_id)
    {
    case MMICL_DIALED_LIST_CTRL_ID:
        call_type = MMICL_CALL_DIALED;
        break;
        
    case MMICL_RECEIVED_LIST_CTRL_ID:
        call_type = MMICL_CALL_RECEIVED;
        break;
    
    case MMICL_MISSED_LIST_CTRL_ID:
        call_type = MMICL_CALL_MISSED;
        break;
    
#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_REFUSED_LIST_CTRL_ID:
        call_type = MMICL_CALL_REFUSED;
        break;
#endif

    default:
        return;     
    }
           
    MMK_GetBothRect(list_ctrl_id, &init_data.both_rect);
    
    GUITOOLBAR_CreateDynamic(win_id, ctrl_id, &init_data);
    GUITOOLBAR_SetAllItem(ctrl_id, 
        cl_toolbar_item_a, 
        sizeof(cl_toolbar_item_a)/sizeof(GUITOOLBAR_ITEM_T));
    
}
/*****************************************************************************/
// 	Description : update cl toolbar
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_UpdateCLWinToolbar(
									MMI_WIN_ID_T      win_id,
									MMI_CTRL_ID_T       list_ctrl_id                                                                       
									)
{
    uint16  icon_width = 0;
    uint16  icon_height = 0;
    uint16  cur_index = 0;
    GUI_RECT_T cur_rect = {0};
    GUI_POINT_T pt_bar = {0};
    MMI_CTRL_ID_T       toolbar_ctrl_id = {0};
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMICL_CALL_TYPE_E call_type = MMICL_CALL_TYPE_MAX;
    
    switch(list_ctrl_id)
    {
        case MMICL_DIALED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_DIALED_CTRL_ID;
            call_type = MMICL_CALL_DIALED;
            break;

        case MMICL_RECEIVED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_RECEIVED_CTRL_ID;
            call_type = MMICL_CALL_RECEIVED;
            break;
        
        case MMICL_MISSED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_MISSED_CTRL_ID;
            call_type = MMICL_CALL_MISSED;
            break;
        
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_REFUSED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_REFUSED_CTRL_ID;
            call_type = MMICL_CALL_REFUSED;
            break;
#endif
                
        default:              
            return;     
    }
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));    

    if(call_type  != MMICL_CALL_MATCHED)
    {
        MMICL_GetRecordInfo( call_type, arraycall_info);
    }
    
    if(( MMICL_CALL_MATCHED == call_type && (s_match_call_info_ptr != NULL && 0 == s_match_call_info_ptr->match_num))
    ||
    #ifdef MMI_PDA_SUPPORT
    (MMICL_CALL_MATCHED != call_type &&  0 == GetCurrTypeNum( call_type,arraycall_info))
    #else
    (MMICL_CALL_MATCHED != call_type &&  0 == arraycall_info->record_num)
    #endif    
    )
    {
        GUITOOLBAR_SetVisible(toolbar_ctrl_id,FALSE);
        SCI_FREE(arraycall_info);
        arraycall_info = PNULL;
        return;
    }
    else 
    {
        GUITOOLBAR_SetVisible(toolbar_ctrl_id,TRUE);
    }
    SCI_FREE(arraycall_info);
    arraycall_info = PNULL;
    
    cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);	
    GUILIST_GetItemRect(list_ctrl_id, cur_index, &cur_rect);
    GUITOOLBAR_GetIconWidthHeight(toolbar_ctrl_id, &icon_width, &icon_height);
    
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        pt_bar.x = cur_rect.right;
    }
    else
    {
        pt_bar.x = cur_rect.left;
    }
    
    if (0 != cur_index)
    {
        pt_bar.y = cur_rect.bottom - MMICL_TOOLBAR_PANEL_GAP + 1; //����
        GUITOOLBAR_SetPanelPopForm(toolbar_ctrl_id, GUITOOLBAR_DIR_DOWN);
    }
    else
    {
        pt_bar.y = cur_rect.bottom + MMICL_TOOLBAR_PANEL_GAP - 1; //����
        GUITOOLBAR_SetPanelPopForm(toolbar_ctrl_id, GUITOOLBAR_DIR_UP);
    }
    GUITOOLBAR_SetPanelBasePt(toolbar_ctrl_id, &pt_bar, FALSE);
    
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        cur_rect.left += MMICL_TOOLBAR_ICON_GAP_RIGHT;
        cur_rect.right = cur_rect.left + icon_width - 1;
    }
    else
    {
        cur_rect.right -= MMICL_TOOLBAR_ICON_GAP_RIGHT;
        cur_rect.left = cur_rect.right - icon_width + 1;
    }
    
    cur_rect.top += MMICL_TOOLBAR_ICON_GAP_TOP;
    cur_rect.bottom = cur_rect.top + icon_height - 1;
    GUITOOLBAR_SetIconRect(toolbar_ctrl_id, &cur_rect, TRUE);	
}
#endif
#endif
#endif

#ifdef MMI_PDA_SUPPORT
LOCAL void DrawListItemIconData(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info,GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr)
{    
    MMIPB_BCD_NUMBER_T pb_num = {0};
    wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
    uint32 custom_photo_name_len = 0;
    uint32 photo_file_size = 0;
    GUIANIM_FILE_INFO_T anim_path = {0};
    GUIITEM_CONTENT_T       item_content = {0};      /*lint !e64*/
    uint32 array_index = 0;
    if(arraycall_info == PNULL ||need_item_content_ptr == PNULL )
    {
        return;
    }
    GUILIST_GetItemData(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type), need_item_content_ptr->item_index, &array_index);
    
    SCI_MEMSET(&pb_num,0x00, sizeof(MMIPB_BCD_NUMBER_T));

    pb_num.number_len = arraycall_info->call_bcdinfo[array_index].number_len;
    pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(arraycall_info->call_bcdinfo[array_index].number_type,
      arraycall_info->call_bcdinfo[array_index].number_plan);
    MMI_MEMCPY(pb_num.number,MMIPB_BCD_NUMBER_MAX_LEN,
      arraycall_info->call_bcdinfo[array_index].number,MMIPB_BCD_NUMBER_MAX_LEN,
      pb_num.number_len);

    if(MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num,custom_photo_name, &custom_photo_name_len, &photo_file_size))
    {
       item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;     
       anim_path.full_path_wstr_ptr = custom_photo_name;
       anim_path.full_path_wstr_len = custom_photo_name_len;
       item_content.item_data.anim_path_ptr = &anim_path;
       GUILIST_SetItemContent(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type), &item_content, need_item_content_ptr->item_index, need_item_content_ptr->item_content_index); 
    }          
}
#endif

/*****************************************************************************/
// 	Description : Get ctrl id for different win id
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetCtrlIDFromWinID(MMI_WIN_ID_T win_id)
{
    switch (win_id)
    {
        case MMICL_DIALED_CHILD_WIN_ID:
            return MMICL_DIALED_LIST_CTRL_ID;
        case MMICL_RECEIVED_CHILD_WIN_ID:
            return MMICL_RECEIVED_LIST_CTRL_ID;
        case MMICL_MISSED_CHILD_WIN_ID:
            return MMICL_MISSED_LIST_CTRL_ID;
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_REFUSED_CHILD_WIN_ID:
            return MMICL_REFUSED_LIST_CTRL_ID;
#endif            
        default:
            break;
    }
    return MMICL_DIALED_LIST_CTRL_ID;
}


#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : Get menu offset by calltype
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL uint16 GetMenuIDOffsetByCallType(MMICL_CALL_TYPE_E call_type)
{
    switch (call_type)
    {
        case MMICL_CALL_MISSED:
            return 0;
        case MMICL_CALL_RECEIVED:
            return (uint16)ID_CL_RECEIVED_CALLS-(uint16)ID_CL_MISSED_CALLS;
        case MMICL_CALL_DIALED:
            return (uint16)ID_CL_DIALED_CALLS-(uint16)ID_CL_MISSED_CALLS;
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_CALL_REFUSED:
            return (uint16)ID_CL_REFUSED_CALLS-(uint16)ID_CL_MISSED_CALLS;
#endif            
        default:
            return 0;
    }
}
#endif

/*****************************************************************************/
// 	Description : to handle dialed call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallLogChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_DIALED_LIST_CTRL_ID;
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef  MMI_MULTI_GREEN_KEY_SINGLE
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
#ifdef MMI_CL_TOOLBAR_SUPPORT
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T toolbars_ctrl_id = GetLogToolBarCtrlId(win_id);
#endif
#endif
#endif
#ifdef MMI_CL_MINI_SUPPORT
    CC_CONTROL_ID_E list_id;
#endif

    list_ctrl_id = GetCtrlIDFromWinID(win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        { 
#ifdef MMI_CL_TOOLBAR_SUPPORT        
#ifndef MMI_PDA_SUPPORT        
#ifdef  TOUCH_PANEL_SUPPORT
            MMICL_CreateToolbar(win_id,toolbars_ctrl_id,list_ctrl_id);
#endif
#endif
#endif
            GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);

            SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));

            GUILIST_SetItemHorMove(list_ctrl_id,TRUE);
            if (PNULL == arraycall_info)
            {
                arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            }

            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"HandleCallLogChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4640_112_2_18_2_11_36_61,(uint8*)"");
                return recode;
            }

            MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        
        break;
        
    case MSG_FULL_PAINT:
        MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
        SetLogListItem();
#ifndef MMI_CL_MINI_SUPPORT		
        if (MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID))
        {
            GUIMENU_SetSelectedItem(MMICL_MAIN_MENU_CTRL_ID, GetMenuIDOffsetByCallType(s_cur_sel_log_list_type));
        }
#endif

#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
#ifdef MMI_CL_TOOLBAR_SUPPORT
        MMICL_UpdateCLWinToolbar(win_id, list_ctrl_id);
#endif        
#endif
        if(arraycall_info->record_num >0)
        {
            GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,TXT_NULL,STXT_RETURN,FALSE );
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL,STXT_RETURN, FALSE);
        }    
#endif

        if (s_cur_sel_log_list_type ==MMICL_CALL_MISSED)
        {
            MMIAPICC_ClearMissedCallNum();
            MMK_CloseWin(MMICC_MISSCALL_WIN_ID);
        }    
        break;
        
#ifdef MMI_PDA_SUPPORT
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);

                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                DrawListItemIconData(arraycall_info,need_item_content_ptr);
            }
            break;
        }
#endif
        
#ifdef  MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1] && GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
#else

    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
         {
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX); 
            
            if(s_is_allow_sim_call[dual_sys] && GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
            {
                HandleRingBack(sim_type, FALSE);
            }
            s_is_allow_sim_call[dual_sys]= FALSE;
         }
         break;
#endif

        //@samboo.shen һ��vt
    case MSG_APP_VIDEO_TEL:
        //SCI_TRACE_LOW:"kick the key VT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4743_112_2_18_2_11_36_62,(uint8*)"");
        if (GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        break;	
        
#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
        if ((GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
            &&s_is_allow_sim_call[MN_DUAL_SYS_1])
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;  
#endif

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)    
        if (MMITHEME_IsIstyle())
        {
            if(PNULL == arraycall_info)
            {
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4927_112_2_18_2_11_37_63,(uint8*)"");
                return recode;
            }

            if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        else
#endif        
        {
            GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
            uint16 cur_sel = 0;

            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            if(item_content_ptr == PNULL)
            {
               break;
            }
            cur_sel = item_content_ptr->item_index;
            if(item_content_ptr->item_content_index == CL_LIST_ITEM_ICON_INDEX)    //Calltype ICON
            {
                HandleRingBackByType(CC_CALL_DEFAULT_CALL);
            }
        }
        break;
#endif

#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
        if (GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);
            }
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_TPMOVE_LEFT_SIDE:
        HandleAnswerSms();
        break;
    case MSG_NOTIFY_TPMOVE_RIGHT_SIDE:
        HandleRingBackByType(CC_CALL_NORMAL_CALL);
        break;
#endif

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT  
#ifdef MMI_CALLLOG_HIDE_MAINMENU
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        if (GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0 )
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif        
#endif
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:        
    case MSG_APP_MENU:
        {
            uint8 record_num = GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type);
#ifdef MMI_PDA_SUPPORT           
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
        }
        break;
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMI_CL_TOOLBAR_SUPPORT
    case MSG_NOTIFY_LIST_MOVEBOTTOM:
    case MSG_NOTIFY_LIST_MOVETOP:   
    case MSG_NOTIFY_LIST_MOVEDOWN:  
    case MSG_NOTIFY_LIST_MOVEUP:
    case MSG_NOTIFY_LIST_PREPAGE:
    case MSG_NOTIFY_LIST_NXTPAGE:     
        {
            GUITOOLBAR_SetShowType(toolbars_ctrl_id, GUITOOLBAR_SHRINK);
            MMICL_UpdateCLWinToolbar(win_id,list_ctrl_id); 
        }
        break;
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
             GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
             if (GUILIST_START_SLIDE == state)
             {
                GUITOOLBAR_SetVisible(toolbars_ctrl_id, FALSE);
             }
             else if (GUILIST_END_SLIDE == state)
             {
                MMICL_UpdateCLWinToolbar(win_id,list_ctrl_id);
             }

        }
        break;
#endif        
#endif
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)    
        if (MMITHEME_IsIstyle())
        {
            if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)    
            {
                HandleRingBackByType(CC_CALL_DEFAULT_CALL);            
            }
        }
        else
#endif        
        
        {
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMI_CL_TOOLBAR_SUPPORT
            MMI_NOTIFY_T notify = {0};
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            
            if (toolbars_ctrl_id == notify.src_id)
            {
                CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(toolbars_ctrl_id, GUITOOLBAR_GetClickItemIndex(toolbars_ctrl_id));
                MMICL_HandleToolBarItemSel(node_id);
            }
            else
#endif            
#endif
#endif
            {
                if(PNULL == arraycall_info)
                {
                    //SCI_TRACE_LOW:"HandleCallLogChildWindow MSG_CTL_PENOK arraycall_info is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4927_112_2_18_2_11_37_63,(uint8*)"");
                    return recode;
                }
   
                if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info);
                    }
                }
           }
      }
      break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef MMI_CL_MINI_SUPPORT
 
    list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
    if(s_select_info.is_select)
    {
        if(s_select_info.log_selected_index[GUILIST_GetCurItemIndex(list_id)])
            SetCurrentSelect(GUILIST_GetCurItemIndex(list_id), FALSE);
        else
            SetCurrentSelect(GUILIST_GetCurItemIndex(list_id), TRUE);

        MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
    }
    else
    {
        if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
#ifdef MMI_LDN_SUPPORT
                if (!IsSimRecord(arraycall_info))
#endif
                {
                    CL_OpenCallTimeListOrDetailWin(arraycall_info);
                }
           
            }
        }
    }

#else
        if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
#ifdef MMI_LDN_SUPPORT
                if (!IsSimRecord(arraycall_info))
#endif
                {
                    CL_OpenCallTimeListOrDetailWin(arraycall_info);
                }
                
            }
        }
#endif
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //MMK_CloseWin(win_id);
        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
        }
#ifdef MMI_CL_MINI_SUPPORT
        ExitSelect();
#endif
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

#if 0
/*****************************************************************************/
// 	Description : to handle dialed call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDialedChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_DIALED_LIST_CTRL_ID;
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef  MMI_MULTI_GREEN_KEY_SINGLE
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T toolbars_ctrl_id = MMICL_TOOLBAR_DIALED_CTRL_ID;
#endif
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        { 
#ifndef MMI_PDA_SUPPORT        
#ifdef  TOUCH_PANEL_SUPPORT
        MMICL_CreateToolbar(win_id,toolbars_ctrl_id,list_ctrl_id);
#endif
#endif
        GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);

        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));

        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"HandleDialedChildWindow call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5030_112_2_18_2_11_37_64,(uint8*)"");
            return recode;
        }
        MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        s_cur_sel_log_list_type = MMICL_CALL_DIALED;
#ifdef MMI_PDA_SUPPORT
        MMICL_GetRecordInfo(MMICL_CALL_DIALED, arraycall_info);
#endif
        SetLogListItem();
        if (MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID))
        {
            GUIMENU_SetSelectedItem(MMICL_MAIN_MENU_CTRL_ID, (uint16)((uint32)ID_CL_DIALED_CALLS - (uint32)ID_CL_MISSED_CALLS));
            // GUIMENU_SetFirstAndSelectedItem(0, (uint16)((uint32)ID_CL_DIALED_CALLS - (uint32)ID_CL_MISSED_CALLS), MMICL_MAIN_MENU_CTRL_ID);
        }
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
        MMICL_UpdateCLWinToolbar(win_id, MMICL_DIALED_LIST_CTRL_ID);
#endif
#endif
        break;
        
 #ifdef MMI_PDA_SUPPORT
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
#endif
        
#ifdef  MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1] && GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
#else

    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
         {
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX); 
            
            if(s_is_allow_sim_call[dual_sys] && GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
            {
                HandleRingBack(sim_type, FALSE);
            }
            s_is_allow_sim_call[dual_sys]= FALSE;
         }
         break;
#endif

        //@samboo.shen һ��vt
    case MSG_APP_VIDEO_TEL:
        //SCI_TRACE_LOW:"kick the key VT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5118_112_2_18_2_11_37_65,(uint8*)"");
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        break;	
        
#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
        if ((GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
            &&s_is_allow_sim_call[MN_DUAL_SYS_1])
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;  
#endif

#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);
            }
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT  
#ifdef MMI_CALLLOG_HIDE_MAINMENU
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0 )
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif        
#endif
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:        
	case MSG_APP_MENU:
        {
            uint8 record_num = GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED);
#ifdef MMI_PDA_SUPPORT           
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
        }
        break;
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_LIST_MOVEBOTTOM:
    case MSG_NOTIFY_LIST_MOVETOP:   
    case MSG_NOTIFY_LIST_MOVEDOWN:  
    case MSG_NOTIFY_LIST_MOVEUP:
    case MSG_NOTIFY_LIST_PREPAGE:
    case MSG_NOTIFY_LIST_NXTPAGE:     
        {
            GUITOOLBAR_SetShowType(toolbars_ctrl_id, GUITOOLBAR_SHRINK);
            MMICL_UpdateCLWinToolbar(win_id,MMICL_DIALED_LIST_CTRL_ID); 
        }
        break;
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
             GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
             if (GUILIST_START_SLIDE == state)
             {
                GUITOOLBAR_SetVisible(toolbars_ctrl_id, FALSE);
             }
             else if (GUILIST_END_SLIDE == state)
             {
                MMICL_UpdateCLWinToolbar(win_id,MMICL_DIALED_LIST_CTRL_ID);
             }

        }
        break;
#endif
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            #ifndef MMI_PDA_SUPPORT
            #ifdef TOUCH_PANEL_SUPPORT
            MMI_NOTIFY_T notify = {0};
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            
            if (toolbars_ctrl_id == notify.src_id)
            {
                CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(toolbars_ctrl_id, GUITOOLBAR_GetClickItemIndex(toolbars_ctrl_id));
                MMICL_HandleToolBarItemSel(node_id);
            }
            else
            #endif
            #endif
            {
                if(PNULL == arraycall_info)
                {
                    //SCI_TRACE_LOW:"HandleDialedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5269_112_2_18_2_11_38_66,(uint8*)"");
                    return recode;
                }
   
                if(GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info);
                    }
                }
           }
      }
      break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //MMK_CloseWin(win_id);
        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : to handle received call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReceivedChildWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_RECEIVED_LIST_CTRL_ID;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T toolbars_ctrl_id = MMICL_TOOLBAR_RECEIVED_CTRL_ID;
#endif
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
        #ifndef MMI_PDA_SUPPORT
        #ifdef TOUCH_PANEL_SUPPORT
        MMICL_CreateToolbar(win_id,toolbars_ctrl_id,list_ctrl_id);
        #endif
        #endif
        GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"HandleReceivedChildWindow call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5370_112_2_18_2_11_38_67,(uint8*)"");
            return recode;
        }
        MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        MMICL_GetRecordInfo(MMICL_CALL_RECEIVED, arraycall_info);
#endif
        s_cur_sel_log_list_type = MMICL_CALL_RECEIVED;
        SetLogListItem();
        if (MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID))
        {
            GUIMENU_SetSelectedItem(MMICL_MAIN_MENU_CTRL_ID, (uint16)((uint32)ID_CL_RECEIVED_CALLS - (uint32)ID_CL_MISSED_CALLS));
            // GUIMENU_SetFirstAndSelectedItem(0, (uint16)((uint32)ID_CL_RECEIVED_CALLS - (uint32)ID_CL_MISSED_CALLS), MMICL_MAIN_MENU_CTRL_ID);
        }
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
        MMICL_UpdateCLWinToolbar(win_id, MMICL_RECEIVED_LIST_CTRL_ID);
#endif
#endif
        break;

#ifdef MMI_PDA_SUPPORT
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
#endif

#ifdef MMI_MULTI_GREEN_KEY_SINGLE        
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        if ((GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)&&s_is_allow_sim_call[MN_DUAL_SYS_1])
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1] = FALSE;
        break;
#else
    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {            
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
            
            if(s_is_allow_sim_call[dual_sys] && (GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0))
            {
                HandleRingBack(sim_type, FALSE);
            }
            s_is_allow_sim_call[dual_sys] = FALSE;
        }
        break;
#endif

        //@samboo.shen һ��vt
    case MSG_APP_VIDEO_TEL:  
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        break;
        
#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:    
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
        }
        break;
#endif
       
#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);
            }
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif 

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_CALLLOG_HIDE_MAINMENU
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif
#endif
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:    
	case MSG_APP_MENU:

        {
            uint8 record_num = GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED);
#ifdef MMI_PDA_SUPPORT           
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
        }
        break;

#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_LIST_MOVEBOTTOM:
    case MSG_NOTIFY_LIST_MOVETOP:   
    case MSG_NOTIFY_LIST_MOVEDOWN:  
    case MSG_NOTIFY_LIST_MOVEUP:
    case MSG_NOTIFY_LIST_PREPAGE:
    case MSG_NOTIFY_LIST_NXTPAGE:     
        {
            GUITOOLBAR_SetShowType(toolbars_ctrl_id, GUITOOLBAR_SHRINK);
            MMICL_UpdateCLWinToolbar(win_id,MMICL_RECEIVED_LIST_CTRL_ID); 
        }
        break;
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
            GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
            if (GUILIST_START_SLIDE == state)
            {
               GUITOOLBAR_SetVisible(toolbars_ctrl_id, FALSE);
            }
            else if (GUILIST_END_SLIDE == state)
            {
               MMICL_UpdateCLWinToolbar(win_id,MMICL_RECEIVED_LIST_CTRL_ID);
            }
        }
       break;
#endif
#endif        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {   
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
            MMI_NOTIFY_T notify = {0};
            
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));       
            if (toolbars_ctrl_id == notify.src_id)
            {
                CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(toolbars_ctrl_id, GUITOOLBAR_GetClickItemIndex(toolbars_ctrl_id));
                MMICL_HandleToolBarItemSel(node_id);
            }
            else
#endif
#endif
            {
                if(PNULL == arraycall_info)
                {
                    //SCI_TRACE_LOW:"HandleReceivedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5605_112_2_18_2_11_38_68,(uint8*)"");
                    return recode;
                }
 
                if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info);

                    }
                }
            }
            break;
        }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : to handle missed call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMissedChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_MISSED_LIST_CTRL_ID;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T toolbars_ctrl_id = MMICL_TOOLBAR_MISSED_CTRL_ID;
#endif
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
        MMICL_CreateToolbar(win_id,toolbars_ctrl_id,list_ctrl_id);
#endif
#endif
        
        GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"HandleMissedChildWindow call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5708_112_2_18_2_11_38_69,(uint8*)"");
            return recode;
        }
        MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        MMICL_GetRecordInfo(MMICL_CALL_MISSED, arraycall_info);
#endif
        s_cur_sel_log_list_type = MMICL_CALL_MISSED;
        SetLogListItem();
        if (MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID))
        {
            GUIMENU_SetSelectedItem(MMICL_MAIN_MENU_CTRL_ID, 0);
            // GUIMENU_SetFirstAndSelectedItem(0, 0/*((uint32)ID_CL_MISSED_CALLS -ID_CL_MISSED_CALLS)*/, MMICL_MAIN_MENU_CTRL_ID);
        }
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
        MMICL_UpdateCLWinToolbar(win_id, MMICL_MISSED_LIST_CTRL_ID);
#endif
#endif
        //�ر�idle״̬����ʾλ�ӵ绰�Ĵ����Լ�״̬��ͼ��
        MMIAPICC_ClearMissedCallNum();
        MMK_CloseWin(MMICC_MISSCALL_WIN_ID);
        break;

#ifdef MMI_PDA_SUPPORT
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
#endif

#ifdef MMI_MULTI_GREEN_KEY_SINGLE        
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1] && (GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1] = FALSE;
        break;
#else
    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys =  MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {            
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
            
            if(s_is_allow_sim_call[dual_sys] && (GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))
            {
                HandleRingBack(sim_type, FALSE);
            }
            s_is_allow_sim_call[dual_sys] = FALSE;
        }
        break;
#endif   
        
#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
        if((GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))        
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
        }
        break;
#endif
        
        //@samboo.shen һ��vt
    case MSG_APP_VIDEO_TEL:
        if((GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))   
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        break;
        
    
#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
        if((GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))   
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);
            }
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif 
    
    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_CALLLOG_HIDE_MAINMENU
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        if((GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))   
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif        
#endif
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
	case MSG_APP_MENU:
        {
            uint8 record_num = GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED);
#ifdef MMI_PDA_SUPPORT           
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
	 }
        break;    
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_LIST_MOVEBOTTOM:
    case MSG_NOTIFY_LIST_MOVETOP:   
    case MSG_NOTIFY_LIST_MOVEDOWN:  
    case MSG_NOTIFY_LIST_MOVEUP:
    case MSG_NOTIFY_LIST_PREPAGE:
    case MSG_NOTIFY_LIST_NXTPAGE:     
        {
            GUITOOLBAR_SetShowType(toolbars_ctrl_id, GUITOOLBAR_SHRINK);
            MMICL_UpdateCLWinToolbar(win_id,MMICL_MISSED_LIST_CTRL_ID); 
        }
        break;
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
            GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
            if (GUILIST_START_SLIDE == state)
            {
               GUITOOLBAR_SetVisible(toolbars_ctrl_id, FALSE);
            }
            else if (GUILIST_END_SLIDE == state)
            {
               MMICL_UpdateCLWinToolbar(win_id,MMICL_MISSED_LIST_CTRL_ID);
            }
        }
       break;
#endif
#endif        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
            MMI_NOTIFY_T notify = {0};
            
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            
            if (toolbars_ctrl_id == notify.src_id)
            {
                CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(toolbars_ctrl_id, GUITOOLBAR_GetClickItemIndex(toolbars_ctrl_id));
                MMICL_HandleToolBarItemSel(node_id);
            }
            else
#endif
#endif
            {
                if(PNULL == arraycall_info)
                {
                    //SCI_TRACE_LOW:"HandleMissedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5945_112_2_18_2_11_39_70,(uint8*)"");
                    return recode;
                }

                if(GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info); 
                    }
                }
            }
            break;
        }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
       
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRefusedChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_REFUSED_LIST_CTRL_ID;
    static BOOLEAN                  s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif    
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T toolbars_ctrl_id = MMICL_TOOLBAR_REFUSED_CTRL_ID;
#endif
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
        MMICL_CreateToolbar(win_id,toolbars_ctrl_id,list_ctrl_id);
#endif
#endif
        
        GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"HandleRefusedChildWindow call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_6049_112_2_18_2_11_39_71,(uint8*)"");
            return recode;
        }
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);

        MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        MMICL_GetRecordInfo(MMICL_CALL_REFUSED, arraycall_info);
#endif
        s_cur_sel_log_list_type = MMICL_CALL_REFUSED;
        SetLogListItem();
        if (MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID))
        {
            GUIMENU_SetSelectedItem(MMICL_MAIN_MENU_CTRL_ID, (uint16)((uint32)ID_CL_REFUSED_CALLS - (uint32)ID_CL_MISSED_CALLS));
            // GUIMENU_SetFirstAndSelectedItem(0, (uint16)((uint32)ID_CL_REFUSED_CALLS - (uint32)ID_CL_MISSED_CALLS), MMICL_MAIN_MENU_CTRL_ID);
        }
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
        MMICL_UpdateCLWinToolbar(win_id, MMICL_REFUSED_LIST_CTRL_ID);
#endif
#endif
        break;
     #ifdef MMI_PDA_SUPPORT
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
        #endif

#ifdef MMI_MULTI_GREEN_KEY_SINGLE        
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:  
        if(s_is_allow_sim_call[MN_DUAL_SYS_1] && (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0))
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
#else
    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys =  MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {            
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
            
            if(s_is_allow_sim_call[dual_sys] && (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0))            
            {
                HandleRingBack(sim_type, FALSE);
            }
            s_is_allow_sim_call[dual_sys] = FALSE;
        }
        break;
#endif

#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:   
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)        
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
        }
        break;
#endif
        
        //@samboo.shen һ��vt
    case MSG_APP_VIDEO_TEL:
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)    
        {
            HandleRingBack(CC_MAJOR_SIM_CALL,TRUE);
        }
        break;
        
#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)  
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICL_REFUSE_LIST_MENU_WIN_TAB, PNULL);
            }
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_CALLLOG_HIDE_MAINMENU
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)  
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif        
#endif
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        break;
    case MSG_LOSE_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);    
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
	case MSG_APP_MENU:
        {
            uint8 record_num = GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED);
#ifdef MMI_PDA_SUPPORT           
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
        }
        break;
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_LIST_MOVEBOTTOM:
    case MSG_NOTIFY_LIST_MOVETOP:   
    case MSG_NOTIFY_LIST_MOVEDOWN:  
    case MSG_NOTIFY_LIST_MOVEUP:
    case MSG_NOTIFY_LIST_PREPAGE:
    case MSG_NOTIFY_LIST_NXTPAGE:     
        {
            GUITOOLBAR_SetShowType(toolbars_ctrl_id, GUITOOLBAR_SHRINK);
            MMICL_UpdateCLWinToolbar(win_id,MMICL_REFUSED_LIST_CTRL_ID); 
        }
        break;
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
            GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
            if (GUILIST_START_SLIDE == state)
            {
               GUITOOLBAR_SetVisible(toolbars_ctrl_id, FALSE);
            }
            else if (GUILIST_END_SLIDE == state)
            {
               MMICL_UpdateCLWinToolbar(win_id,MMICL_REFUSED_LIST_CTRL_ID);
            }
        }
       break;
#endif
#endif        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
            MMI_NOTIFY_T notify = {0};
            
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            
            if (toolbars_ctrl_id == notify.src_id)
            {
                CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(toolbars_ctrl_id, GUITOOLBAR_GetClickItemIndex(toolbars_ctrl_id));
                MMICL_HandleToolBarItemSel(node_id);
            }
            else
#endif
#endif
            {
                if(PNULL == arraycall_info)
                {
                    //SCI_TRACE_LOW:"HandleRefusedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_6288_112_2_18_2_11_40_72,(uint8*)"");
                    return recode;
                }

                if(GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info);
                    }
                }
            }
            break;
        }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void CL_ValidatePrivacyTabSwitch(BOOLEAN is_permitted, DPARAM param_ptr)
{
    if (is_permitted)
    {
        MMIPB_SetCLUnProtect(TRUE);
        //@@@ MMIAPIPB_SetCurAppType();
    }
    else
    {
        MMIAPIPB_SwitchToPreTab();
    }
}

/*****************************************************************************/
//  Description : ����list����ʱ��ͼƬ���ַ����Ļص�����
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetListSildeData(MMITHEME_LIST_CUSTDATA_T* data_ptr)
{
    if (PNULL == data_ptr)
    {
        return;
    }
/*
    data_ptr->data_type = GUILIST_TYPE_TEXT_ID;
    data_ptr->str_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);            //item color
    data_ptr->str_style.font = MMI_LIST_DEFAULT_ITEM_FONT;
    data_ptr->str_style.align = ALIGN_HVMIDDLE;

    data_ptr->right_text_id = TXT_DIAL;
    data_ptr->left_text_id = TXT_SMS;
    data_ptr->left_icon_id = IMAGE_SMS_NOT_READ_SMS_ICON;
    data_ptr->right_icon_id = IMG_CC_ANSWER_ICON;*/
}

/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
//    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
    MMI_WIN_ID_T info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);
    uint8 call_log_num = 0;
    uint16      i=0;
    MMI_HANDLE_T      dropdownlist_ctr_handle;
    MMI_HANDLE_T      list_handle = 0;
    uint32  droplist_text_id[] =
            {
                TXT_CL_CALLS_ALL,
                TXT_CL_MISSED_CALLS,
                TXT_CL_RECEIVED_CALLS,
                TXT_CL_DIALED_CALLS,
#if defined(MMI_BLACKLIST_SUPPORT)
                TXT_CL_REFUSED_CALLS
#endif
            };
    uint32  droplist_type[]={
                MMICL_CALL_ALL,
                MMICL_CALL_MISSED,
                MMICL_CALL_RECEIVED,
                MMICL_CALL_DIALED,
#if defined(MMI_BLACKLIST_SUPPORT)
                MMICL_CALL_REFUSED
#endif
        };
    dropdownlist_ctr_handle =  MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_LIST_DROPLIST_CTRL_ID);
    list_handle = MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

 		

#ifdef MMI_PDA_SUPPORT 
        {
        MMI_HANDLE_T  main_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_FORM_CTRL_ID);
        MMI_HANDLE_T  droplist_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_LIST_FORM_CTRL_ID);
        GUIFORM_CHILD_HEIGHT_T              height_info = {0}; 
   
        if (!InitPbCalllogData(info_ptr))
        {
            MMK_CloseParentWin(MMK_GetParentWinHandle(win_id));
            break;
        }

        GUIFORM_IsSlide(main_form_handle,FALSE);
        GUIFORM_PermitChildBorder(main_form_handle, FALSE);
        GUILIST_PermitBorder(list_handle, FALSE);
       // GUILIST_SetUserBg(list_handle, FALSE);
        GUIFORM_PermitChildBg(main_form_handle,FALSE);

        height_info.type = GUIFORM_CHILD_HEIGHT_LEFT;
    	GUIFORM_SetChildHeight(main_form_handle, list_handle, &height_info);

        GUIFORM_PermitChildBorder(droplist_form_handle, TRUE);

        GUIDROPDOWNLIST_SetMaxItem(dropdownlist_ctr_handle,5);
        for(i=0; i<ARR_SIZE(droplist_text_id); i++) 
            {
                GUIDROPDOWNLIST_AppendItemById(dropdownlist_ctr_handle, droplist_text_id[i]);
            }
#ifdef MMI_CL_TOOLBAR_SUPPORT
        MMICL_CreateToolbar(win_id,MMICL_TOOLBAR_ALLLIST_CTRL_ID,list_ctrl_id);
#endif
        GUIDROPDOWNLIST_SetCurItemIndex(dropdownlist_ctr_handle,0);
        SetAllLogFillter(droplist_type[0]);
        GUIDROPDOWNLIST_SetButton(win_id, dropdownlist_ctr_handle, TRUE);

        }

        GUILIST_SetItemHorMove(list_handle, TRUE);
        // GUILIST_RegGetDataFunc(list_handle,SetListSildeData);

        CalllogPBData_SetListHandle( MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
#if defined MMI_ISTYLE_SUPPORT        
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
        else 
#endif			
        {
#endif
            //GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);
            
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"HandleAllChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_6502_112_2_18_2_11_40_73,(uint8*)"");
                return recode;
            }
            MMK_SetAtvCtrl(win_id, list_handle);
#ifdef MMI_PDA_SUPPORT
        }
#endif
        break;
        
    case MSG_FULL_PAINT:
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            break;
        }
        //�ر�idle״̬����ʾλ�ӵ绰�Ĵ����Լ�״̬��ͼ��
        MMIAPICC_ClearMissedCallNum();
#ifdef MMI_PDA_SUPPORT 
#if defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
        else 
#endif			
        {
            MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
#endif
            s_cur_sel_log_list_type = MMICL_CALL_ALL;
            SetLogListItem();
#ifdef MMI_PDA_SUPPORT
        }
#endif

        break;

#ifdef MMI_PDA_SUPPORT
    /*add for istyle*/

     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
#endif


#ifdef MMI_MULTI_GREEN_KEY_SINGLE        
    case MSG_KEYDOWN_GREEN:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1])
        {
            if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
            {
                HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
            }
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
#else
    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys =  MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {            
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);

            if(s_is_allow_sim_call[dual_sys])
            {
                if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
                {
                    HandleRingBack(sim_type, FALSE);
                }
            }
            s_is_allow_sim_call[dual_sys] = FALSE;
        }
        break;
#endif

#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
    case MSG_APP_VIDEO_TEL:        
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
#if 1//OPEN_VT_CHANGE
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
#else
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
#endif
        }
        break;
#endif
        

        
#ifdef MMI_PDA_SUPPORT    //to do open list menu

    case MSG_CTL_DROPDOWNLIST_SELECTED:
        {
        uint16 dropdownlist_index = 0;
        dropdownlist_index = GUIDROPDOWNLIST_GetCurItemIndex(dropdownlist_ctr_handle);
        SetAllLogFillter(droplist_type[dropdownlist_index]);
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
        }
        break;
    case MSG_CTL_LIST_LONGOK:
		{
            MMI_NOTIFY_T notify = {0};
            // If not list long pen ok
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            if (notify.src_handle !=list_handle)
            {
                break;
            }
		}
        call_log_num = GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type);

        if( call_log_num > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
#ifdef MMI_GPRS_SUPPORT
            else
            {
                MMK_CreateWin((uint32 *)MMICL_ALL_LIST_POP_MENU_WIN_TAB, (ADD_DATA)info_ptr);
            }
#endif
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
        if (g_pbcalllog_data ==PNULL)
        {
            if(!InitPbCalllogData(info_ptr))
            {
                break;
            }
        }
        {
        uint16 dropdownlist_index = 0;
        dropdownlist_index = GUIDROPDOWNLIST_GetCurItemIndex(dropdownlist_ctr_handle);
        SetAllLogFillter(droplist_type[dropdownlist_index]);
        }

        CalllogPBData_SetListHandle(MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));
#if defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
#endif		
        if (PNULL == arraycall_info)
        {
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        }
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_handle);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }

        GUIWIN_SetTitleIsVisible(win_id, FALSE, FALSE);
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        break;
#endif
    case MSG_LOSE_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);
        break;

    case MSG_CTL_TAB_SWITCH:
        //��˽���
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //��Ҫ������˽���
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
 
    case MSG_CTL_OK:
    case MSG_APP_OK:
	case MSG_APP_MENU:
        {
            uint8 record_num = GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type);
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
        }
        break;
        
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
#if defined MMI_ISTYLE_SUPPORT		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
        else
#endif			
        {
            GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
            uint16 cur_sel = 0;

            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            if(item_content_ptr == PNULL)
            {
               break;
            }
            cur_sel = item_content_ptr->item_index;
            if(item_content_ptr->item_content_index == CL_LIST_ITEM_TYPE_INDEX)    //Calltype ICON
            {
#ifdef MMI_CL_TOOLBAR_SUPPORT
                MMI_HANDLE_T toolbar_ctrl_handle;//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);
                toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMICL_TOOLBAR_ALLLIST_CTRL_ID);//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);
                MMICL_UpdateCLWinToolbar(win_id,MMICL_ALL_LIST_CTRL_ID);
#endif                
            }
            else if(item_content_ptr->item_content_index == CL_LIST_ITEM_ICON_INDEX)
            {
                HandleRingBackByType(CC_CALL_DEFAULT_CALL);
            }
        }       

        break; 
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT    
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
#if defined MMI_ISTYLE_SUPPORT        
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
        else 
#endif			
        {
                
            {
#ifdef MMI_CL_TOOLBAR_SUPPORT
                MMI_NOTIFY_T notify = {0};
                MMI_HANDLE_T hToolbar =  MMK_GetCtrlHandleByWin(win_id,MMICL_TOOLBAR_ALLLIST_CTRL_ID); 

                SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
                
                if (MMICL_TOOLBAR_ALLLIST_CTRL_ID == notify.src_id)
                {
                    CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                    node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(hToolbar, GUITOOLBAR_GetClickItemIndex(hToolbar));
                    MMICL_HandleToolBarItemSel(node_id);
                    break;
                }
                else
                {
                    GUITOOLBAR_SetVisible(hToolbar,FALSE);
                }
#endif                
            }

        
#endif
            {
                if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info);
                    }
                }
            }
            break;
#ifdef MMI_PDA_SUPPORT 
        }
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TPMOVE_LEFT_SIDE:
        HandleAnswerSms();
        break;
    case MSG_NOTIFY_TPMOVE_RIGHT_SIDE:
        HandleRingBackByType(CC_CALL_NORMAL_CALL);
        break;
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(info_ptr); 
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }

        FreePbCalllogData();
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT    
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
#endif
        break;
        
    default:
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT    
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
#endif
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;

}

/*****************************************************************************/
// 	Description : to handle select contact from call log child window message
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_WIN_ID_T info_ptr = 0;
    uint16      i=0;
    MMI_HANDLE_T      dropdownlist_ctr_handle;
    MMI_HANDLE_T      list_handle = 0;
    uint32  droplist_text_id[] ={
                TXT_CL_CALLS_ALL,
                TXT_CL_MISSED_CALLS,
                TXT_CL_RECEIVED_CALLS,
                TXT_CL_DIALED_CALLS,
#if defined(MMI_BLACKLIST_SUPPORT)
                TXT_CL_REFUSED_CALLS
#endif
            };
    uint32  droplist_type[]={
                MMICL_CALL_ALL,
                MMICL_CALL_MISSED,
                MMICL_CALL_RECEIVED,
                MMICL_CALL_DIALED,
#if defined(MMI_BLACKLIST_SUPPORT)
                MMICL_CALL_REFUSED
#endif
            };
	GUILIST_MARK_ITEM_T *list_param_ptr = PNULL;
    MMI_HANDLE_T main_form_handle       = MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_FORM_CTRL_ID);
    uint16 max_selected_num = 0;

    info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);
    dropdownlist_ctr_handle =  MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_LIST_DROPLIST_CTRL_ID);
    list_handle = MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        if (PNULL == arraycall_info)
        {
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        }
        else
        {
            SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        }
        {
            MMI_HANDLE_T  droplist_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_LIST_FORM_CTRL_ID);
            GUIFORM_CHILD_HEIGHT_T              height_info = {0}; 
       
            if (!InitPbCalllogData(info_ptr))
            {
                MMK_CloseParentWin(MMK_GetParentWinHandle(win_id));
                break;
            }
            GUIFORM_IsSlide(main_form_handle,FALSE);
            GUIFORM_PermitChildBorder(main_form_handle, FALSE);
            GUILIST_PermitBorder(list_handle, FALSE);
            GUIFORM_PermitChildBg(main_form_handle,FALSE);
            height_info.type = GUIFORM_CHILD_HEIGHT_LEFT;
        	GUIFORM_SetChildHeight(main_form_handle, list_handle, &height_info);
            GUIFORM_PermitChildBorder(droplist_form_handle, TRUE);
            GUIDROPDOWNLIST_SetMaxItem(dropdownlist_ctr_handle,5);
            for(i=0; i<ARR_SIZE(droplist_text_id); i++) 
                {
                    GUIDROPDOWNLIST_AppendItemById(dropdownlist_ctr_handle, droplist_text_id[i]);
                }
#ifdef MMI_CL_TOOLBAR_SUPPORT
            MMICL_CreateToolbar(win_id,MMICL_TOOLBAR_ALLLIST_CTRL_ID,list_ctrl_id);
#endif            
            GUIDROPDOWNLIST_SetCurItemIndex(dropdownlist_ctr_handle,0);
            SetAllLogFillter(droplist_type[0]);
            GUIDROPDOWNLIST_SetButton(win_id, dropdownlist_ctr_handle, TRUE);

        }
        CalllogPBData_SetListHandle( MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        SCI_MEMSET(&s_call_log_all_select_info, 0x00, sizeof(MMICL_ALL_SELECT_INFO_T));
        max_selected_num = MMISMS_DEST_ADDR_MAX_NUM - MMIAPISMS_GetDestCount();
        GUILIST_SetMaxSelectedItem(MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LIST_CTRL_ID), max_selected_num);
        if (MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())	
        {
			MMICC_CreateNewSoftKey(win_id, main_form_handle, STXT_SOFTKEY_ADD_MK, TXT_NULL, STXT_CANCEL);
            GUISOFTKEY_SetButtonGray(win_id, MMICALLLOG_SOFTKEY_CTRL_ID,LEFT_BUTTON, TRUE);			
        }
#if defined MMI_ISTYLE_SUPPORT		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleSelectAllChildWindow(win_id, msg_id, param);
        }
        else   
#endif			
        {
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"HandleSelectAllChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_6980_112_2_18_2_11_41_74,(uint8*)"");
                return recode;
            }
            MMK_SetAtvCtrl(win_id, list_handle);

        }
        break;
        
    case MSG_FULL_PAINT:
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            break;
        }
        //�ر�idle״̬����ʾλ�ӵ绰�Ĵ����Լ�״̬��ͼ��
        MMIAPICC_ClearMissedCallNum(); 
        MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
        s_cur_sel_log_list_type = MMICL_CALL_ALL;
        SetLogSelectListItem();
        break;
        
    //to do open list menu
    case MSG_CTL_DROPDOWNLIST_SELECTED:
        {
            uint16 dropdownlist_index = 0;
            dropdownlist_index = GUIDROPDOWNLIST_GetCurItemIndex(dropdownlist_ctr_handle);
            SetAllLogFillter(droplist_type[dropdownlist_index]);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
            SCI_MEMSET(&s_call_log_all_select_info, 0x00, sizeof(MMICL_ALL_SELECT_INFO_T));
            GUILIST_SetAllSelected(MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LIST_CTRL_ID), FALSE);
        }
        break;

    case MSG_GET_FOCUS:
        {
            uint16 dropdownlist_index = 0;
            dropdownlist_index = GUIDROPDOWNLIST_GetCurItemIndex(dropdownlist_ctr_handle);
            SetAllLogFillter(droplist_type[dropdownlist_index]);
        }

        CalllogPBData_SetListHandle(MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));
#if defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleSelectAllChildWindow(win_id, msg_id, param);
        }
#endif		
        if (PNULL == arraycall_info)
        {
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        }
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_handle);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }

        GUIWIN_SetTitleIsVisible(win_id, FALSE, FALSE);
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        break;

    case MSG_LOSE_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);
        break;

    case MSG_CTL_TAB_SWITCH:
        SCI_MEMSET(&s_call_log_all_select_info, 0x00, sizeof(MMICL_ALL_SELECT_INFO_T));
        GUILIST_SetAllSelected(MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LIST_CTRL_ID), FALSE);
        //��˽���
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //��Ҫ������˽���
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_SendMsg(
            MMK_GetPrevWinId(MMK_GetParentWinHandle(win_id)), 
            MMI_CL_MULTI_SELECT_LIST_RETURN, 
            PNULL
            );
        MMK_CloseWin(MMK_GetParentWinHandle(win_id));
        break;

	case MSG_CTL_LIST_MARK_ITEM:
        list_param_ptr = (GUILIST_MARK_ITEM_T*)param;
		if(list_param_ptr == PNULL)
		{
			break;
		}
        if(list_param_ptr->had_maked)
        {
            if (!s_call_log_all_select_info.sel_item[list_param_ptr->item_idx].is_valid)
            {
                s_call_log_all_select_info.sel_count ++; 
                s_call_log_all_select_info.sel_item[list_param_ptr->item_idx].is_valid = TRUE;
            }
        }
        else
        {
            if (s_call_log_all_select_info.sel_item[list_param_ptr->item_idx].is_valid)
            {
                if(s_call_log_all_select_info.sel_count > 0) s_call_log_all_select_info.sel_count --; 
                s_call_log_all_select_info.sel_item[list_param_ptr->item_idx].is_valid = FALSE;
            }
        }
        if (MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())	
        {
            if(s_call_log_all_select_info.sel_count >0)
            {
                GUISOFTKEY_SetButtonGray(win_id, MMICALLLOG_SOFTKEY_CTRL_ID,LEFT_BUTTON, FALSE);
            }
            else
            {
                GUISOFTKEY_SetButtonGray(win_id, MMICALLLOG_SOFTKEY_CTRL_ID,LEFT_BUTTON, TRUE);
            }
        }    
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleSelectAllChildWindow(win_id, msg_id, param);
        }
        else
#endif			
        {					
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_SendMsg(
        			MMK_GetPrevWinId(MMK_GetParentWinHandle(win_id)),
        			MMI_CL_MULTI_SELECT_LIST_RETURN, 
        			PNULL
        			);
                MMK_CloseWin(MMK_GetParentWinHandle(win_id));
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            MMK_CloseWin(MMK_GetParentWinHandle(win_id));
        }
        else
#endif			
        {
            MMK_CloseParentWin(info_ptr); 
        }
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        FreePbCalllogData();
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleSelectAllChildWindow(win_id, msg_id, param);
        }
#endif		
        break;
        
    default:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleSelectAllChildWindow(win_id, msg_id, param);
        }
#endif		
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

/*****************************************************************************/
//  Description : the handle function of call list delete menu window
//  Global resource dependence :
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleLogListDeleteQueryWinMsg(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifdef MMI_CL_MINI_SUPPORT
        if(s_select_info.is_select)
        {
            DeleteRecord();
        }
        else
        {
            DeleteCurrentRecord();
        }
#else
        DeleteCurrentRecord();
        CalllogOpenBannerWin(win_id,STR_CL_NOTE_DELETED_EXT01);
#endif
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : the handle function of call list delete all menu window
//  Global resource dependence :
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleLogListDeleteAllQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        MMK_CloseWin(win_id);
        DeleteAllCallLog(MMICL_CALL_ALL);

        CalllogOpenBannerWin(win_id,STR_CL_NOTE_DEL_LOG_EXT02);

        //success!
        // close log list window
        //MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        //close the window of call log list menu and detail win
        MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
        MMK_CloseWin(MMICL_REFUSED_LOG_LIST_MENU_WIN_ID);
        MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

#ifdef ZDT_PRODUCT_Z685S
PUBLIC void ResetFactory_Cl(void)
{
	DeleteAllCallLog(MMICL_CALL_ALL);
	MMIAPICL_SetDefaultCallTimeToNV();
	MMIAPICC_ClearMissedCallNum();
}
#endif
#ifdef MMI_PDA_SUPPORT
LOCAL MMI_RESULT_E  HandleLogListMenukeyWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    uint32          group_id = 0, menu_id = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
    MMK_SetAtvCtrl(win_id, MMICL_LOG_LIST_MENUKEY_CTRL_ID);
    break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_CloseWin(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(MMICL_LOG_LIST_MENUKEY_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
            case ID_CL_DELETE_ALL:
                           //ѯ���Ƿ�ɾ��!! 
                MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
                MMK_CloseWin(win_id);
                break;
#ifdef     MMI_CALLLOG_HIDE_MAINMENU
#ifdef ASP_SUPPORT            
            case ID_CL_ASP:
                {
                    MMIAPIASP_EnterAsp();
                    MMK_CloseWin(win_id);
                    break;
                }
#endif  
            case ID_CL_CALL_TIMERS:
                {            
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                    uint16 sim_card_ok = 0 ;
                    uint32 sim_card_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_card_ok, 1);
                    
                    if( 0 == sim_card_ok_num )
                    {
                        MMIAPIPHONE_AlertSimNorOKStatus();
                    }
                    else if ( 1 == sim_card_ok_num )
                    {
                        MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, (ADD_DATA)sim_card_ok);       
                    }            
                    else
                    {
                        MMICL_SelectSIMFunc(CallTimeSelectSimCallback,MMISET_APPEND_SIM_OK,PNULL);
                    }
#else
                    MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, PNULL);
#endif
                MMK_CloseWin(win_id);
                }
                break;
                
            case ID_CL_GPRS_FLOWRATE:
#ifdef MMI_GPRS_SUPPORT
                MMIAPICL_OpenGprsFlowRateWin();
                MMK_CloseWin(win_id);
#endif
                break;
                
#endif

            default:
                break;
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//	Description : the handle function of call list delete all menu window
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_Istyle_HandleLogListDeleteAllQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        MMK_CloseWin(win_id);
        DeleteAllCallLog(MMICL_CALL_ALL);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : insert dyna menu node by label
//	Global resource dependence : 
//  Author:rong.gu
//	Note:
/*****************************************************************************/
LOCAL void InsertContextMenuNode(
                        	     MMI_CTRL_ID_T      ctrl_id,        //�ؼ�id
                        	     MMI_TEXT_ID_T   	text_id,        //����Ľڵ���ı�
                        	     uint16          	node_id,        //�ڵ���
                        	     uint16          	parent_node_id, //���ڵ���
                                     uint16                index
                                 )
{
    MMI_STRING_T            string = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &string);
    node_item.item_text_ptr = &string;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}


#endif


#ifdef MMI_PDA_SUPPORT
LOCAL void  MMICL_SetLogListDetailMenuTitle(void)
 {
    GUIMENU_TITLE_INFO_T title_info = {0};
    MMI_STRING_T title_str = {0}; 
    MMI_STRING_T    string = {0};
    uint8 temp_str[PHONE_NUM_MAX_LEN] = {0};
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 disp_str[CL_SAVE_MAX_LEN] = {0};
    uint16 disp_num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;        

    wchar wchar_disp_str[CL_SAVE_MAX_LEN] = {0};

    MMI_GetLabelTextByLang(TXT_CALL, &title_str);
    title_info.is_static = TRUE;
    title_info.title_ptr = &title_str;
    title_info.sub_title_ptr = PNULL;
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    title_info.group_id = MENU_CL_DIAL_SUBMENU_OPT;
#else
    title_info.group_id = MENU_CL_LOG_LIST_OPT;
#endif
    GUIMENU_SetMenuTitleEx(MMICL_LOG_LIST_MENU_CTRL_ID, &title_info);

    MMI_GetLabelTextByLang(TXT_PHONE_NUMBER, &string);

    MMIAPICOM_WstrToStr(string.wstr_ptr,temp_str);
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);

    disp_num_len= sprintf((char *)disp_str, "%s %s",temp_str, tele_num);
    MMI_STRNTOWSTR(wchar_disp_str, disp_num_len, (const uint8 *)disp_str, disp_num_len, disp_num_len);

    title_info.title_ptr->wstr_ptr = wchar_disp_str;
    title_info.title_ptr->wstr_len = disp_num_len;
    title_info.group_id = MENU_CL_ADD_TO_PB;
    GUIMENU_SetMenuTitleEx(MMICL_LOG_LIST_MENU_CTRL_ID, &title_info);

    SCI_MEMSET(tele_num,0x00,sizeof(tele_num));
    SCI_MEMSET(wchar_disp_str,0x00,sizeof(wchar_disp_str));
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);
    MMI_STRNTOWSTR(wchar_disp_str, PHONE_NUM_MAX_LEN + 2, (const uint8 *)tele_num, PHONE_NUM_MAX_LEN + 2,  (uint16)strlen( (char *)tele_num ));
    title_info.title_ptr->wstr_ptr = wchar_disp_str;
    title_info.title_ptr->wstr_len = MMIAPICOM_Wstrlen(wchar_disp_str);
    title_info.group_id = MENU_CL_LOG_LIST_OPT;
    GUIMENU_SetMenuTitleEx(MMICL_LOG_LIST_MENU_CTRL_ID, &title_info);

#if defined(MMI_BLACKLIST_SUPPORT)
    //Sam@Dec06 CR0144206 should set Refuse menu title here
    title_info.group_id = MENU_CL_REFUSE_OPT;	
    GUIMENU_SetMenuTitleEx(MMICL_LOG_LIST_MENU_CTRL_ID, &title_info);
#endif 
 }
#endif

/*****************************************************************************/
//  Description : to handle the call log list menu window message
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLogListMenuWindow(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    uint32          group_id = 0, menu_id = 0;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T    query_win_id = MMICL_QUERY_WIN_ID;
    MMI_WIN_ID_T    confirmation_win_id = MMICL_CONFIRMATION_WIN_ID;
    //MMI_CTRL_ID_T ctrl_list_id;
    #ifdef MMI_CL_MINI_SUPPORT
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
    #endif


    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:


#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    {
        MMI_STRING_T            titlestr = {0};       
        MMI_GetLabelTextByLang(STXT_OPTION, &titlestr);
        GUIMENU_SetMenuTitle(&titlestr, MMICL_LOG_LIST_MENU_CTRL_ID);
    }
#endif		

#ifdef MMI_PDA_SUPPORT
        MMICL_SetLogListDetailMenuTitle();
#endif
        MMK_SetAtvCtrl(win_id, MMICL_LOG_LIST_MENU_CTRL_ID);
        {
            //cr99896
            if(MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
            {
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_DIAL_VIDEOCALL,TRUE);
            }

            if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE)&&MMIAPICC_IsVideoCall())//@samboo vtͨ��ʱ�޷������µĺ��� 20071127
            {
                #ifdef    MMI_PDA_SUPPORT
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_DIAL_VIDEOCALL,TRUE);
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_DIAL_AUDIOCALL,TRUE);
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_DIAL_IPCALL,TRUE);                  
                #else
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
#ifdef VT_SUPPORT
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_DIAL_SUBMENU_OPT,ID_CL_DIAL_VIDEOCALL,TRUE);
#endif
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_DIAL_SUBMENU_OPT,ID_CL_DIAL_AUDIOCALL,TRUE);
#ifdef MMI_IP_CALL_SUPPORT
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_DIAL_SUBMENU_OPT,ID_CL_DIAL_IPCALL,TRUE);
#endif
#endif
                #endif
            }

            if(TRUE == MMIAPISET_IsDisableShareViaOption())
            {
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT2 , ID_CL_LOG_LIST_SEND, FALSE);
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_SEND, FALSE);
            }

            if(MMIAPIUDISK_UdiskIsRun())
            {
                #ifdef  MMI_PDA_SUPPORT
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_SEND_MMS,TRUE);        
                #else
                //grayed mms menu
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_SEND_OPT,ID_CL_SEND_MMS,TRUE);                
                #endif
            }
#ifdef MMI_BLACKLIST_SUPPORT
            {
                MMIPB_BCD_NUMBER_T  tele_num = {0};
                //get the phone number(bcd)
                GetPartyPhoneNum(&tele_num);

                if (MMIAPISET_IsCCBlacklist(&tele_num,MN_CALL_TYPE_NORMAL))
                {
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT2 , ID_CL_LOG_LIST_REFUSE_ADD, FALSE);
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_REFUSE_ADD, FALSE);                    
                }
                else
                {
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT2 , ID_CL_LOG_LIST_REFUSE_REMOVE, FALSE);
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_REFUSE_REMOVE, FALSE);                    
                }
            }
#endif
            {
                BOOLEAN is_match_pb = (BOOLEAN)MMK_GetWinAddDataPtr(win_id);
                if(is_match_pb)
                {
                    GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_LOG_LIST_SAVE,TRUE);
                }
            }
        }
#ifdef MMI_CL_MINI_SUPPORT

        MMICL_LogListMenuGrayed(win_id, MMICL_LOG_LIST_MENU_CTRL_ID);

#endif
        break;

    case MSG_GET_FOCUS:
        if(MMIAPIUDISK_UdiskIsRun())
        {
            #ifdef  MMI_PDA_SUPPORT
            GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_SEND_MMS,TRUE);
            #else
                    //grayed mms menu
            GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_SEND_OPT,ID_CL_SEND_MMS,TRUE);
            #endif
        }
        else
        {
            #ifdef  MMI_PDA_SUPPORT
            GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_SEND_MMS,FALSE);
            #else
                  //grayed mms menu
            GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_SEND_OPT,ID_CL_SEND_MMS,FALSE);
            #endif
        }
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifndef  MMI_PDA_SUPPORT    //not respond menu key
    //case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        GUIMENU_GetId(MMICL_LOG_LIST_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_LOG_DETAIL:
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, PNULL);
            MMK_CloseWin(win_id);
            break;
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
        case ID_CL_LOG_LIST_DELETE_MULTI:
            MMK_CreateWin((uint32 *)MMICL_CHILD_SELECT_WIN_TAB, PNULL);
            MMK_CloseWin(win_id);

            break;
#endif
        case ID_CL_ADD_TO_PB:
            HandleDistillPhoneNum();
            MMK_CloseWin(win_id);
            break;

        case ID_CL_LOG_LIST_DELETE:
            //ѯ���Ƿ�ɾ��!!
#ifdef CALL_TIMES_SUPPORT
            if(MMK_IsOpenWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID)
                /*||MMK_IsOpenWin(MMICL_CALL_TIMES_WIN_ID)*/)
            {
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
            }
            else
#endif
            {
                //MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteQueryWinMsg);
                //MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteQueryWinMsg, PNULL);
                MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_CL_QUERY_DELETE_CALL_EXT01,PNULL,
                        IMAGE_NULL,&confirmation_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,CC_HandleLogListDeleteQueryWinMsg);
                MMK_CloseWin(win_id);
            }
            break;

#ifdef VT_SUPPORT
       case ID_CL_DIAL_VIDEOCALL://@zhaohui add
            HandleRingBack(CC_CALL_SIM_MAX, TRUE);
#ifdef CALL_TIMES_SUPPORT
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
#endif
            MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;
#endif

        case ID_CL_DIAL_AUDIOCALL:
            HandleRingBack(CC_CALL_SIM_MAX, FALSE);
#ifdef CALL_TIMES_SUPPORT
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
#endif
            MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;

#ifdef MMI_IP_CALL_SUPPORT
        case ID_CL_DIAL_IPCALL:
            HandleIpCall();
#ifdef CALL_TIMES_SUPPORT
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
#endif
            MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_CL_ADD_TO_EXIST_CONTACT:
            HandleAddNumToName();
            MMK_CloseWin(win_id);
            break;
#endif
        case ID_CL_SEND_SMS:
            HandleAnswerSms();
            MMK_CloseWin(win_id);
            break;

        case ID_CL_SEND_MMS:
            HandleAnswerMms();
            MMK_CloseWin(win_id);
            break;

#if defined(MMI_BLACKLIST_SUPPORT)
        case ID_CL_LOG_LIST_REFUSE_ADD:
            {
                MMK_CreateWin((uint32 *)MMICL_BLOCK_NUMBER_WIN_TAB,PNULL);
                MMK_CloseWin(win_id);
            }
            break;
        case ID_CL_LOG_LIST_REFUSE_REMOVE:
            {
                MMIPB_BCD_NUMBER_T  tele_num = {0};
                //get the phone number(bcd)
                GetPartyPhoneNum(&tele_num);
                if (MMIAPISET_DeleteBlacklist(tele_num))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMPLETE);
                }
                MMK_CloseWin(win_id);
            }
            break;
#endif

        case ID_CL_DELETE_ALL:
            //ѯ���Ƿ�ɾ��!! 
            //MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
            MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_CL_QUERY_DEL_LOG_EXT02,PNULL,
                    IMAGE_NULL,&confirmation_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,CC_HandleLogListDeleteAllQueryWinMsg);
            MMK_CloseWin(win_id);
            break;

        case ID_CL_CALL_TIMERS:
            {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                uint16 sim_card_ok = 0 ;
                uint32 sim_card_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_card_ok, 1);

                if( 0 == sim_card_ok_num )
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                else if ( 1 == sim_card_ok_num )
                {
                    MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, (ADD_DATA)sim_card_ok);       
                }
                else
                {
                    MMICL_SelectSIMFunc(CallTimeSelectSimCallback,MMISET_APPEND_SIM_OK,PNULL);
                }
#else
                MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, PNULL);
#endif
            }
            break;

        case ID_CL_GPRS_FLOWRATE:
#ifdef MMI_GPRS_SUPPORT
            MMIAPICL_OpenGprsFlowRateWin();
#endif
            break;

#ifdef AOC_SUPPORT
        case ID_CL_COSTS_TOTAL:
        case ID_CL_COSTS_MAX:
        case ID_CL_COSTS_RATE:
        case ID_CL_COSTS_RESET:
            MMICL_COSTS_HandleMainMenuMsg(menu_id);
            break;
#endif
//add by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
        case ID_CL_MARK_LIST:

            SetCurrentSelect(GUILIST_GetCurItemIndex(list_id), TRUE);
            s_select_info.is_select = TRUE;
            MMK_CloseWin(win_id);
            break;

        case ID_CL_CANCEL_MARK_LIST:

            SetCurrentSelect(GUILIST_GetCurItemIndex(list_id), FALSE);

            MMK_CloseWin(win_id);
            break;

        case ID_CL_MARKALL_LIST:

            s_select_info.is_select = TRUE;
            SetAllSelect(TRUE);
            MMK_CloseWin(win_id);
            break;

        case ID_CL_CANCLE_MARKALL_LIST:

            s_select_info.is_select = FALSE;
            SetAllSelect(FALSE);
            MMK_CloseWin(win_id);
            break;
#endif
        case ID_CL_LOG_LIST_SEND:
            //MMICL_SelectSIMFunc(SendMsgSelectSimCallback,MMISET_APPEND_SIM_OK,PNULL);
            //CalllogSendMessage();
            HandleAnswerSms();
            MMK_CloseWin(win_id);
            break;
        default:
            break;
        }
        break;

    #ifdef  MMI_PDA_SUPPORT    //not respond menu key
    case MSG_APP_OK:
    case MSG_CTL_OK:
    #endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_OK:
        //�رռ�¼��menu���ں�ѯ�ʴ���
        GUIMENU_GetId(MMICL_LOG_LIST_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_SEND_MMS:
            MMIPUB_CloseQuerytWin(&query_win_id);
            HandleAnswerMms();
            break;

        case ID_CL_LOG_LIST_DELETE:
            //ѯ���Ƿ�ɾ��!! 
#ifdef CALL_TIMES_SUPPORT
            if(MMK_IsOpenWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID)
                /*||MMK_IsOpenWin(MMICL_CALL_TIMES_WIN_ID)*/)
            {
                uint32 select_index = (uint32)MMK_GetWinAddDataPtr(MMICL_CALL_TIMES_DETAIIL_WIN_ID);

                MMIPUB_CloseQuerytWin(&query_win_id);
                /*DeleteOneCallTimesItem((uint16)select_index);*/
                MMICL_DeleteOneCallTimesItem(s_cur_sel_log_list_type,select_index);
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
            }
#endif
            break;

        default:
            break;
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        //�ر�ѯ�ʴ���
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add the num to blacklist   
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void AddNumToBlackList(
                        MMIPB_BCD_NUMBER_T  pb_bcd,    //IN:
                        MMI_WIN_ID_T win_id
                        )
{
    uint8  num_ton = 0;//@fen.xie MS00180421
    BOOLEAN is_plus_symbol = FALSE;
    MMI_STRING_T mmi_pb_name ={0};

    num_ton = pb_bcd.npi_ton>> 4;//@fen.xie MS00180421
    if ((0 == pb_bcd.number_len) && (MN_NUM_TYPE_INTERNATIONAL == num_ton))/*@fen.xie MS00180421: '+' */
    {
        is_plus_symbol = TRUE;
    }

    if ((pb_bcd.number_len > 0) || is_plus_symbol)
    {
        mmi_pb_name.wstr_ptr = SCI_ALLOCA((PHONE_NUM_MAX_LEN + 1)*sizeof(wchar));
        if (PNULL != mmi_pb_name.wstr_ptr)
        {
            SCI_MEMSET(mmi_pb_name.wstr_ptr, 0x00, sizeof(wchar)*(PHONE_NUM_MAX_LEN + 1));
        }
        else
        {
            return;
        }
        MMIAPIPB_GetNameByNumber(&pb_bcd,
                                  &mmi_pb_name,
                                  PHONE_NUM_MAX_LEN,
                                  FALSE  //use default name,"no name"
                                  );
        if (MMIAPISET_AddBlacklist(mmi_pb_name, pb_bcd))
        {
            GUI_POINT_T start_point = {0, 0};
            MMICOM_BANNER_INFO_T banner_info = {0};

            //DeleteCurrentRecord();

            banner_info.banner_type = MMI_BANNER_TEXT;
            banner_info.text1_id = STR_CL_NUMBER_BLOCKED_EXT01;
            banner_info.start_point = start_point;
            MMIAPICOM_OpenBannerWin(win_id,banner_info);
        }
        SCI_FREE(mmi_pb_name.wstr_ptr);
    }
    else
    {
        //��ʾ����û�к���
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}
#endif

/*****************************************************************************/
// 	Description : set detail of call log list
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void SetLogListDetail(void)
{
    uint8   tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    wchar   tele_num_wstr[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8   time[MMICL_DISPLAY_TIME_LEN + 1 ] = {0};
    wchar   time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    uint8   date[MMICL_DISPLAY_DATE_LEN + 1] = {0};
    wchar   date_wstr[MMICL_DISPLAY_DATE_LEN + 1] = {0};
    uint8   duration_time[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    wchar   duration_time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    uint16  sel_item_index = 0;
    MMI_TM_T    tm = {0};
    MMI_STRING_T    name_str = {0};
    MMI_STRING_T    num_str = {0};
    MMI_STRING_T    time_str = {0};
    MMI_STRING_T    date_str = {0};
    MMI_STRING_T    duration_time_str = {0};
    MMI_STRING_T    call_type_str = {0};
    MMIPB_BCD_NUMBER_T  bcd_number = {0};
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_STRING_T    sim_num = {0};
    MMISET_SIM_NAME_T sim_name = {0};
#endif
    MMI_STRING_T    location_str = {0};
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
    uint16  city_ucs2_str[MMIFINDMP_CITYNAME_LENGTH + 1] = {0};
    uint16  str_length = 0;
    uint8   origin_number_arr[CC_MAX_TELE_NUM_LEN + 2] = {0};
    int16   origin_num_len  = 0;
#endif

    uint16 index = 0;
    MMI_CTRL_ID_T ctrl_id = MMICL_LOG_LIST_DETAIL_CTRL_ID;
    
    //initialize
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListDetail call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_7798_112_2_18_2_11_43_75,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(&tm,0,sizeof(tm));
    SCI_MEMSET(&name_str,0,sizeof(name_str));
    name_str.wstr_ptr = (wchar *)SCI_ALLOC_APP( ( MMIPB_MAX_STRING_LEN + 1 ) * sizeof(wchar) );
    SCI_MEMSET(&num_str,0,sizeof(num_str));
    SCI_MEMSET(&time_str,0,sizeof(time_str));
    SCI_MEMSET(&date_str,0,sizeof(date_str));
    SCI_MEMSET(&bcd_number,0,sizeof(bcd_number));

    GetCurrentRecordInfo(arraycall_info, &sel_item_index);

    //get name
    if (!arraycall_info->call_bcdinfo[sel_item_index].is_num_present)//�绰���벻����
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_UNKNOW_NUM,&num_str);//�պ�
    }
    else
    {
        //get number
        MMICL_GenDispNumber(arraycall_info->call_bcdinfo[sel_item_index].call_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_len,
            arraycall_info->call_bcdinfo[sel_item_index].number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);
        num_str.wstr_len = (uint16)strlen((char*)tele_num);
        num_str.wstr_ptr = tele_num_wstr;//(uint8 *)tele_num;
        MMI_STRNTOWSTR( tele_num_wstr, PHONE_NUM_MAX_LEN, (uint8 *)tele_num, num_str.wstr_len, num_str.wstr_len );

        MMIAPICL_GetNameByClInfo(arraycall_info->call_bcdinfo[sel_item_index], &name_str, FALSE);
    }
    
    //get time and date
#ifdef CALL_TIMES_SUPPORT
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_start_time[0]);
#else
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_start_time);
#endif
    MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min,time,MMICL_DISPLAY_TIME_LEN + 1 );
    time_str.wstr_ptr = time_wstr;//(uint8 *)time;
    //time_str.is_ucs2 = FALSE;
    time_str.wstr_len = (uint16)strlen((char*)time);

    MMI_STRNTOWSTR( time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)time, time_str.wstr_len, time_str.wstr_len );
    
    MMIAPISET_FormatDateStrByDateStyle(tm.tm_year,tm.tm_mon,tm.tm_mday,'/', date,MMICL_DISPLAY_DATE_LEN + 1);
    date_str.wstr_ptr = date_wstr;
    //date_str.is_ucs2 = FALSE;
    date_str.wstr_len = (uint16)strlen((char*)date);
    MMI_STRNTOWSTR( date_wstr, MMICL_DISPLAY_DATE_LEN, (uint8 *)date, date_str.wstr_len, date_str.wstr_len );
    
    //get duration time
#ifdef CALL_TIMES_SUPPORT
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[0]);
#else
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_duration_time);
#endif
    sprintf((char*)duration_time, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    duration_time_str.wstr_ptr  = duration_time_wstr;
    duration_time_str.wstr_len    = (uint16)strlen((char*)duration_time);
    //duration_time_str.is_ucs2   = FALSE;
    MMI_STRNTOWSTR( duration_time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)duration_time, duration_time_str.wstr_len, duration_time_str.wstr_len );

    //get location
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
    if (arraycall_info->call_bcdinfo[sel_item_index].is_num_present)//�绰���벻����
    {
        uint8 origin_number_str[CC_MAX_TELE_NUM_LEN + 1] = {0};

        MMI_WSTRNTOSTR((uint8 *)origin_number_str, CC_MAX_TELE_NUM_LEN, num_str.wstr_ptr, num_str.wstr_len, num_str.wstr_len);

        if(MMIAPICC_RemoveIpNumber( origin_number_str, num_str.wstr_len, origin_number_arr,CC_MAX_TELE_NUM_LEN))
        {
            origin_num_len = strlen((char*)origin_number_arr);
        }
        else
        {
            MMI_WSTRNTOSTR((uint8 *)origin_number_arr, CC_MAX_TELE_NUM_LEN, num_str.wstr_ptr, num_str.wstr_len, num_str.wstr_len );
            //MMI_MEMCPY(origin_number_arr, CC_MAX_TELE_NUM_LEN + 2, num_str.str_ptr, num_str.length, num_str.length);
            origin_num_len = num_str.wstr_len;
        }
        
        str_length = MMIAPIPHONESEARCH_FindLocation(origin_number_arr, origin_num_len, (uint16*)city_ucs2_str);
    }
    else
    {
        str_length = 0;
    }
    if( str_length == 0 )
    {
        MMI_GetLabelTextByLang(TXT_UNKNOWN, &location_str);
    }   
    else if( str_length == MMIPHONESEARCH_LOCALNUMBER )
    {
        MMI_GetLabelTextByLang(TXT_LOCAL, &location_str);
    }   
    else
    {
        location_str.wstr_ptr = city_ucs2_str;
        //location_str.is_ucs2 = TRUE;
        location_str.wstr_len  = str_length;
    }
#endif
    
    //get call type
    if (arraycall_info->call_bcdinfo[sel_item_index].isvideocall)
    {
        MMI_GetLabelTextByLang(TXT_CALL_VIDEO, &call_type_str);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CALL_AUDIO, &call_type_str);
    }   

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    //Emergency Number does not display sim name!!    
    if(!arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        //get sim name
        sim_name = MMIAPISET_GetSimName(arraycall_info->call_bcdinfo[sel_item_index].dual_sys);
        
        sim_num.wstr_len  = sim_name.wstr_len;
        sim_num.wstr_ptr = sim_name.wstr_arr;
    }
#endif

    //display name in pb
    if (arraycall_info->call_bcdinfo[sel_item_index].is_num_present && (0 != name_str.wstr_len))
    {
        //if the num is present and the name is being
        SetHeadAndContentInRText(TXT_NAME,ctrl_id,name_str,&index);
    }

    //display num
    SetHeadAndContentInRText(TXT_COMMON_NUMBER,ctrl_id,num_str,&index);

#ifdef CALL_TIMES_SUPPORT
    if (arraycall_info->call_bcdinfo[sel_item_index].call_start_time[0] > 0)
#else
    if (arraycall_info->call_bcdinfo[sel_item_index].call_start_time > 0)
#endif
    {
        //display date
        SetHeadAndContentInRText(TXT_DATE,ctrl_id,date_str,&index);

        //display time
        SetHeadAndContentInRText(TXT_TIME,ctrl_id,time_str,&index);
    }

#ifndef CMCC_UI_STYLE
    //display duration
    if ((MMICL_CALL_DIALED == arraycall_info->call_bcdinfo[sel_item_index].call_type ||
        MMICL_CALL_RECEIVED == arraycall_info->call_bcdinfo[sel_item_index].call_type)
        &&(!arraycall_info->call_bcdinfo[sel_item_index].isvideocall)
#ifdef CALL_TIMES_SUPPORT
        && arraycall_info->call_bcdinfo[sel_item_index].call_start_time[0] > 0
#else
        && arraycall_info->call_bcdinfo[sel_item_index].call_start_time > 0
#endif
        )//@zhaohui modify,��Ƶͨ������ʾʱ����Ϣ
    {
        SetHeadAndContentInRText(TXT_CL_CALL_TIME,ctrl_id,duration_time_str,&index);
    }
#endif

    //display location
    SetHeadAndContentInRText(TXT_GUISHUDI,ctrl_id,location_str,&index);

    //display type  voice call/video call	
    SetHeadAndContentInRText(TXT_COMMON_TYPE,ctrl_id,call_type_str,&index);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    //Emergency Number does not display sim name!!
    if(!arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        //display sim name
        SetHeadAndContentInRText(TXT_CL_DETAIL_SIM_HEAD,ctrl_id,sim_num,&index);
     }
#endif   

    //��¼�����Ϣ��Ϊ�绰�����ṩ��Ҫ��Ϣ
    s_call_log_detail_info_ptr->dual_sys = arraycall_info->call_bcdinfo[sel_item_index].dual_sys;
    SCI_MEMSET(s_call_log_detail_info_ptr->tele_num, 0, (PHONE_NUM_MAX_LEN + 2));
    SCI_MEMCPY(s_call_log_detail_info_ptr->tele_num, tele_num, (PHONE_NUM_MAX_LEN + 2));
#if defined(CL_SAVE_NAME_SUPPORT)
    if (arraycall_info->call_bcdinfo[sel_item_index].name.is_use_nv_name)
    {
        SCI_MEMCPY((void *)&s_call_log_detail_info_ptr->name,(void *)&arraycall_info->call_bcdinfo[sel_item_index].name,sizeof(MMICL_NAME_T));
    }
#endif
    //free memory
    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
    }
    
    if (PNULL != name_str.wstr_ptr)
    {
        SCI_FREE(name_str.wstr_ptr);
    }
}

//added by du.yuxuan
LOCAL MMI_RESULT_E HandleLogListDetailMenuWindow(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    uint32          group_id = 0, menu_id = 0;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    BOOLEAN is_match_pb = (BOOLEAN)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T ctrl_id = MMICL_LOG_LIST_DETAIL_MENU_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_SetItemVisible(ctrl_id,MENU_CL_DETAIL_OPT,ID_CL_CONTACT_VIEW,is_match_pb);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        GUIMENU_GetId(MMICL_LOG_LIST_DETAIL_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_SEND_SMS:
        case ID_CL_LOG_LIST_SEND:
            //MMICL_SelectSIMFunc(SendMsgSelectSimCallback,MMISET_APPEND_SIM_OK,PNULL);
            //CalllogSendMessage();
            HandleAnswerSms();
            MMK_CloseWin(win_id);
            break;
        case ID_CL_CONTACT_VIEW:
            HandleContactView();
            MMK_CloseWin(win_id);
            break;
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

LOCAL void HandleContactView(void)
{
    MMIPB_BCD_NUMBER_T  tele_num = {0};
    uint8  num_ton = 0;//@fen.xie MS00180421
    BOOLEAN is_plus_symbol = FALSE;
    BOOLEAN is_num_valid = TRUE;
    uint8   tele_string[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8   tele_length = 0;

    //get the phone number(bcd)
    GetPartyPhoneNum(&tele_num);

    //+bug132483
    tele_length = MMIAPIPB_BCDNumberToString(&tele_num, PHONE_NUM_MAX_LEN + 2, tele_string);
    is_num_valid = MMIAPICOM_IsValidNumberString((char *)tele_string, tele_length);
    SCI_TRACE_LOW("yanyan.an is_num_valid:%d", is_num_valid);
    SCI_TRACE_BUF("yanyan.an tele_num.number:%d", tele_string, tele_length);
    if (FALSE == is_num_valid)
    {
        //��ʾ��Ч����
        MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        return;
    }
    //-bug132483

    num_ton = tele_num.npi_ton>> 4;//@fen.xie MS00180421
    if ((0 == tele_num.number_len) && (MN_NUM_TYPE_INTERNATIONAL == num_ton))/*@fen.xie MS00180421: '+' */
    {
        is_plus_symbol = TRUE;
    }

    if ((tele_num.number_len > 0) || is_plus_symbol)
    {
        MMIAPIPB_OpenNumberContactWin(&tele_num);
    }
    else
    {
        //��ʾ����û�к���
        MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        return;
    }
}


/*****************************************************************************/
//  Description : load call log list detail into listbox
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendLogListDetailItem(
                                    MMI_CTRL_ID_T                   ctrl_id,
                                    MMICL_CALL_TYPE_E               call_type,
                                    MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info,
                                    uint16                  selected_call_log_index
                                    )
{
    int i = 0;
    MMICL_CALL_BCDINFO_T     *p_callinfo = PNULL;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    MMI_STRING_T sim_name_str  = {0};
    wchar sim_name_wstr[GUILIST_STRING_MAX_NUM + 1]={0};

    MMI_STRING_T call_type_string = {0};
    //wchar wstr_type[MMICL_CALL_TYPE_STRING_MAX_LEN + 2] = {0};

    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_STRING_T time_str       ={0};
    wchar wstr_time_str[GUILIST_STRING_MAX_NUM + 1] = {0};

    uint8   duration_time[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    MMI_STRING_T    duration_time_str = {0};
    MMI_TM_T    tm = {0};
    wchar   duration_time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};

    uint8    detail_list_num = 0;
    MMICL_GROUP_ITEM_DETAIL_T s_group_detail_info[MMICL_RECORD_MAX_NUM*MMICL_MAX_CALL_TIMES] = {0};

#ifdef MMI_MULTI_SIM_SYS_SINGLE
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_2TEXT_AND_TEXT_MS;
#else
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_2TEXT_AND_TEXT_ICON_MS;
#endif
    item_t.item_data_ptr = &item_data;
    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT;

    if(TRUE == MMIAPISET_IsDisableShareViaOption())
    {
        item_data.softkey_id[0] = TXT_NULL;
    }
    else
    {
        item_data.softkey_id[0] = STXT_SOFTKEY_MENU_MK;
    }
    item_data.softkey_id[1] = STXT_SOFTKEY_CALL_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;

    detail_list_num = GetGroupItemDetail(selected_call_log_index,arraycall_info,
            s_group_detail_info);
    if (detail_list_num == 0)
    {
        return;
    }

    for ( i = detail_list_num - 1; i >= 0; i-- )
    {
        MMI_STRING_T        text_display = {0};

        p_callinfo =
                &arraycall_info->call_bcdinfo[s_group_detail_info[i].record_index];

        SCI_MEMSET(&call_type_string,0,sizeof(MMI_STRING_T));
        //SCI_MEMSET(&wstr_type,0,sizeof(wstr_type));
        //call_type_string.wstr_ptr = wstr_type;

        SCI_MEMSET(&time_str,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&wstr_time_str,0,sizeof(wstr_time_str));
        time_str.wstr_ptr = wstr_time_str;

        SCI_MEMSET(duration_time, 0x00, sizeof(duration_time));
        SCI_MEMSET(&tm,0,sizeof(tm));
        SCI_MEMSET(&duration_time_str,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&duration_time_wstr,0,sizeof(duration_time_wstr));
        duration_time_str.wstr_ptr = duration_time_wstr;

        //Add call type content
        GetCallTypeString(FALSE,p_callinfo->call_type,&call_type_string);
        item_data.item_content[CL_DETAIL_CALL_TYPE_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[CL_DETAIL_CALL_TYPE_INDEX].item_data.text_buffer.wstr_len = call_type_string.wstr_len;
        item_data.item_content[CL_DETAIL_CALL_TYPE_INDEX].item_data.text_buffer.wstr_ptr = call_type_string.wstr_ptr;

        if (p_callinfo->call_times > 0)
        {
            uint32  call_duration_time = 0;
            uint8    call_time_index     = 0;

            call_time_index = s_group_detail_info[i].call_time_index;
            call_duration_time = p_callinfo->call_duration_time[call_time_index];

            //Add call duration time content
            if (call_duration_time != 0)
            {
                tm = MMIAPICOM_Second2Tm(call_duration_time);
#if defined (MAINLCD_SIZE_128X160) 				
                sprintf((char*)duration_time, "%02d:%02d", tm.tm_min, tm.tm_sec);
#else
                sprintf((char*)duration_time, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
#endif
                duration_time_str.wstr_ptr  = duration_time_wstr;
                duration_time_str.wstr_len  = (uint16)strlen((char*)duration_time);
                MMI_STRNTOWSTR( duration_time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)duration_time, duration_time_str.wstr_len, duration_time_str.wstr_len );

                item_data.item_content[CL_DETAIL_DURATION_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_DETAIL_DURATION_INDEX].item_data.text_buffer.wstr_len = duration_time_str.wstr_len;
                item_data.item_content[CL_DETAIL_DURATION_INDEX].item_data.text_buffer.wstr_ptr = duration_time_str.wstr_ptr;
                item_data.item_content[CL_DETAIL_DURATION_INDEX].is_custom_font_color = TRUE;
                item_data.item_content[CL_DETAIL_DURATION_INDEX].custom_font_color = MMI_BLACK_COLOR;//MMI_DEFAULT_TEXT_COLOR;
                item_data.item_content[CL_DETAIL_DURATION_INDEX].custom_font_color_focus = MMI_DEFAULT_TEXT_COLOR;
                item_data.item_content[CL_DETAIL_DURATION_INDEX].is_opacity = TRUE;
            }

            //Add number type and call start time content
            if (0 < p_callinfo->call_start_time[call_time_index])
            {
                MMI_STRING_T    type_strid = {0};
                wchar   type_wstr[GUILIST_STRING_MAX_NUM+1] = {0};

                SCI_MEMSET(type_wstr, 0, sizeof(type_wstr));
                type_strid.wstr_ptr = type_wstr;
                type_strid.wstr_len = 0;

                if (Cl_GetNumberTypeStrByNumber(type_wstr, p_callinfo))
                {
                    MMIAPICOM_Wstrncat(type_wstr, L", ", 2);
                    type_strid.wstr_len = MMIAPICOM_Wstrnlen(type_wstr, GUILIST_STRING_MAX_NUM);
                }

                GetTimeAndDateContent(&time_str,GUILIST_STRING_MAX_NUM,p_callinfo,call_time_index);
                MMIAPICOM_CatTwoString(&text_display, &type_strid, &time_str);
                item_data.item_content[CL_DETAIL_NUM_TYPE_AND_START_DT_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_DETAIL_NUM_TYPE_AND_START_DT_INDEX].item_data.text_buffer.wstr_len = text_display.wstr_len;
                item_data.item_content[CL_DETAIL_NUM_TYPE_AND_START_DT_INDEX].item_data.text_buffer.wstr_ptr = text_display.wstr_ptr;
                item_data.item_content[CL_DETAIL_NUM_TYPE_AND_START_DT_INDEX].is_custom_font_color = TRUE;
            }

            //set content 2's color and opacity
            {
                BOOLEAN is_missed_call = p_callinfo->call_type == MMICL_CALL_MISSED;
                GUI_COLOR_T font_color_normal = is_missed_call ? MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT)
                        : MMI_DEFAULT_TEXT_COLOR;
                GUI_COLOR_T font_color_focus  = is_missed_call ? MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_3)
                        : MMI_DEFAULT_TEXT_COLOR;
                uint8 alpha = 255 * 60 / 100;

                item_data.item_content[CL_DETAIL_NUM_TYPE_AND_START_DT_INDEX].custom_font_color
                        = font_color_normal;
                item_data.item_content[CL_DETAIL_NUM_TYPE_AND_START_DT_INDEX].custom_font_color_focus
                        = font_color_focus;
                item_data.item_content[CL_DETAIL_NUM_TYPE_AND_START_DT_INDEX].is_opacity = TRUE;
                GUILIST_SetItemContentOpacityFontColor(&item_data,CL_DETAIL_NUM_TYPE_AND_START_DT_INDEX,alpha,alpha);
            }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
            //Add sim card image content
            {
                MMI_IMAGE_ID_T image_id = p_callinfo->dual_sys == MN_DUAL_SYS_1 ?
                        IMAGE_CUSTOM_SIM1_SMALL : IMAGE_CUSTOM_SIM2_SMALL;

                if(IsNeedDisplaySimCard(p_callinfo))
                {
                    item_data.item_content[CL_DETAIL_SIM_CARD_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[CL_DETAIL_SIM_CARD_INDEX].item_data.image_id = image_id;
                }
            }
#endif
        }

        item_t.user_data = i;
        GUILIST_AppendItem( ctrl_id, &item_t );
        MMIAPICOM_DestroyString(text_display);
    }
}

LOCAL void SetLogListDetailItem()
{
    uint16 sel_item_index = 0;
    uint16 current_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_HANDLE_T list_handle = MMICL_LOG_LIST_DETAIL_LIST_CTRL_ID;

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10476_112_2_18_2_11_50_100,(uint8*)"");
        return;
    }

    current_index = GUILIST_GetCurItemIndex(list_handle);

    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    GetCurrentRecordInfo(arraycall_info, &sel_item_index);

    GUILIST_SetMaxItem(list_handle, MMICL_RECORD_MAX_NUM, FALSE );

    AppendLogListDetailItem(list_handle, MMICL_CALL_ALL, arraycall_info,sel_item_index);

    GUILIST_SetCurItemIndex(list_handle, current_index);

    SCI_FREE(arraycall_info);

}

LOCAL BOOLEAN MMIAPICL_IsVoiceMailNum(
                                 uint8 *tele_num,
                                 uint8 tele_len,
                                 MN_DUAL_SYS_E dual_sys)
{
	uint32 i = 0;
	MNSIM_ECC_T sim_emc = {0};//��¼sim���洢��emc����
	uint8 temp_tele_num[CC_MAX_TELE_NUM_LEN + 1] = {0};
	uint8 temp_tele_len = 0;
	uint8 temp_tele_num2[CC_MAX_TELE_NUM_LEN + 1] = {0};
	uint8 vm1tele_len = 0;
	uint8 vm2tele_len = 0;
	wchar vm1tele_str[CC_MAX_TELE_NUM_LEN+1]  = {0};
	wchar vm2tele_str[CC_MAX_TELE_NUM_LEN+1]  = {0};
	BOOLEAN is_from_sim1 = FALSE;
	BOOLEAN is_from_sim2 = FALSE;
        MMIPB_BCD_NUMBER_T bcd_tele_num = {0};
        MMIPB_BCD_NUMBER_T bcd_vm_num = {0};

	if(PNULL == tele_num)
	{
		SCI_TraceLow("mmicl: MMIAPICL_IsVoiceMailNum tele_num is NULL");
		return FALSE;
	}
	if(0 == tele_len)
	{
		SCI_TraceLow("mmicl: MMIAPICL_IsVoiceMailNum tele_len is 0");
		return FALSE;
	}
	if ('+' == tele_num[0])
	{
		temp_tele_len = MIN(tele_len - 1, CC_MAX_TELE_NUM_LEN);
		if(0 == temp_tele_len)
		{
			SCI_TraceLow("mmicl: MMIAPICL_IsVoiceMailNum temp_tele_len is 0");
			return FALSE;
		}
		SCI_MEMCPY(temp_tele_num, tele_num + 1, temp_tele_len);
	}
	else
	{
		temp_tele_len = MIN(tele_len, CC_MAX_TELE_NUM_LEN);
		SCI_MEMCPY(temp_tele_num, tele_num, temp_tele_len);
	}
	
	if(dual_sys == MN_DUAL_SYS_1){
		is_from_sim1 = MMISET_GetIsVMFromSIM(MN_DUAL_SYS_1);
		if (TRUE == is_from_sim1)
		{
			vm1tele_len = MMISET_GetSimVoiceMailNumber(MN_DUAL_SYS_1, vm1tele_str);
		}
		else
		{
			vm1tele_len = MMISET_GetIVoiceMailNumberToNV(MN_DUAL_SYS_1,vm1tele_str);
		}
		
		if (vm1tele_len>0)
		{
			MMIAPICOM_WstrToStr(vm1tele_str,temp_tele_num2);
			SCI_TraceLow("mmicl: MMIAPICL_IsVoiceMailNum  vm1tele_len=%d  temp_tele_num=%s  temp_tele_num2=%s",vm1tele_len,temp_tele_num,temp_tele_num2);
			MMIAPIPB_StringToBCDNumber(&temp_tele_num, &bcd_tele_num);
			MMIAPIPB_StringToBCDNumber(&temp_tele_num2, &bcd_vm_num);
			if(0 == MMIAPIPB_BcdSmartCmp(&bcd_tele_num, &bcd_vm_num))
			{
				return TRUE;
			}
		}
	}

	if(dual_sys == MN_DUAL_SYS_2){
		is_from_sim2 = MMISET_GetIsVMFromSIM(MN_DUAL_SYS_2);
		if (TRUE == is_from_sim2)
		{
			vm2tele_len = MMISET_GetSimVoiceMailNumber(MN_DUAL_SYS_2, vm2tele_str);
		}
		else
		{
			vm2tele_len = MMISET_GetIVoiceMailNumberToNV(MN_DUAL_SYS_2,vm2tele_str);
		}

		if (vm2tele_len > 0)
		{
			MMIAPICOM_WstrToStr(vm2tele_str,temp_tele_num2);
			SCI_TraceLow("mmicl: MMIAPICL_IsVoiceMailNum  vm2tele_len=%d  temp_tele_num=%s  temp_tele_num2=%s ",vm2tele_len,temp_tele_num,temp_tele_num2);
			MMIAPIPB_StringToBCDNumber(&temp_tele_num, &bcd_tele_num);
			MMIAPIPB_StringToBCDNumber(&temp_tele_num2, &bcd_vm_num);
			if(0 == MMIAPIPB_BcdSmartCmp(&bcd_tele_num, &bcd_vm_num))
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}


LOCAL BOOLEAN InitLogListDetailContactItem(
                                    MMI_CTRL_ID_T           ctrl_id,
                                    MMICL_CALL_TYPE_E       call_type,
                                    MMICL_CALL_BCDINFO_T    *p_callinfo
                                    )
{
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    //MMICL_CALL_BCDINFO_T    *callinfo=PNULL;

    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint16  tele_num_len =0;
    //MMI_STRING_T name_number_str ={0};
    //wchar name_number_wchar[PHONE_NUM_MAX_LEN + 1]  ={0};
    BOOLEAN find_result = FALSE;

    MMIPB_BCD_NUMBER_T pb_num = {0};
    wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
    uint32 custom_photo_name_len = 0;
    uint32 photo_file_size = 0;
    GUIANIM_FILE_INFO_T anim_path = {0};

    MMI_STRING_T first_name_str = {0};
    MMI_STRING_T last_name_str  = {0};
    MMI_STRING_T abbr_name_str  = {0};
    wchar first_name_wchar[PHONE_NUM_MAX_LEN + 1]  = {0};
    wchar last_name_wchar[PHONE_NUM_MAX_LEN + 1] = {0};
    wchar abbr_name_wchar[MMIPB_MAX_STRING_LEN + 1] = {0};
    uint16 entry_id = 0,storage_id = 0;
    BOOLEAN is_abbr_valid = FALSE;
    BOOLEAN is_anim_valid = FALSE;
    uint8 list_style = 0;

    item_t.item_data_ptr = &item_data;
    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT | GUIITEM_STATE_CONTENT_CHECK | GUIITEM_STATE_NEED_ITEM_CONTENT;

    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;

    if(p_callinfo == PNULL)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10526_112_2_18_2_11_50_101,(uint8*)"");
        return FALSE;
    }

    SCI_MEMSET(tele_num, 0x00, sizeof(tele_num));
    SCI_MEMSET(&pb_num,0x00, sizeof(MMIPB_BCD_NUMBER_T));
    //SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
    //SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));
    //name_number_str.wstr_ptr = name_number_wchar;
    first_name_str.wstr_ptr = first_name_wchar;
    last_name_str.wstr_ptr = last_name_wchar;
    abbr_name_str.wstr_ptr = abbr_name_wchar;

    pb_num.number_len = p_callinfo->number_len;
    pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(p_callinfo->number_type,
            p_callinfo->number_plan);
    MMI_MEMCPY(pb_num.number,MMIPB_BCD_NUMBER_MAX_LEN,
            p_callinfo->number,MMIPB_BCD_NUMBER_MAX_LEN,
            pb_num.number_len);

    anim_path.full_path_wstr_ptr = custom_photo_name;
    find_result = MMIAPIPB_GetNameSurnameAvatarByNumber(&pb_num,&last_name_str,&first_name_str,
            &abbr_name_str,&anim_path,&entry_id,&storage_id,PHONE_NUM_MAX_LEN,FALSE);
    is_anim_valid = anim_path.full_path_wstr_ptr != PNULL && anim_path.full_path_wstr_len > 0;
    is_abbr_valid = !is_anim_valid && (abbr_name_str.wstr_ptr != PNULL && abbr_name_str.wstr_len > 0);

    if(is_anim_valid)
    {
        item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
        item_data.item_content[0].item_data.anim_path_ptr = &anim_path;
    }
    else
    {
        MMI_IMAGE_ID_T image_id = IMAGE_NULL;

        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        image_id = is_abbr_valid ? IMAGE_LISTITEM_AVATAR_INITIALS
                : IMAGE_CONTACTS_UNDEFINED_CONTACT;
        item_data.item_content[0].item_data.image_id = image_id;
    }

    //get number
    tele_num_len = MMICL_GenDispNumber(p_callinfo->call_type,
                    p_callinfo->number_type,
                    p_callinfo->number_len,
                    p_callinfo->number,
                    tele_num,
                    PHONE_NUM_MAX_LEN + 2);
    //find_result = MMIAPICL_GetNameByClInfo(*p_callinfo, &name_number_str, FALSE);

    if(last_name_str.wstr_len  == 0 || first_name_str.wstr_len == 0)
    {
		item_t.item_style = GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS;
		if ( !find_result )
		{
			SCI_TRACE_LOW("InitLogListDetailContactItem num len:%d", tele_num_len);
			if(tele_num_len > 0)
			{
				find_result = MMIAPICL_IsVoiceMailNum(tele_num, tele_num_len, p_callinfo->dual_sys);
				if (find_result)
				{
					MMI_GetLabelTextByLang(STR_CL_VOICEMAIL_EXT01, &first_name_str);
				}
				else
				{
					//no name in pb
					MMI_STRNTOWSTR( first_name_str.wstr_ptr, PHONE_NUM_MAX_LEN, (uint8 *)tele_num, PHONE_NUM_MAX_LEN, tele_num_len );
					first_name_str.wstr_len = MMIAPICOM_Wstrlen(first_name_str.wstr_ptr);
				}
			}
			else
			{
				MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_UNKNOW_NUM, &first_name_str);
			}
		}
		else if ((last_name_str.wstr_len  == 0 )&& (first_name_str.wstr_len == 0))
		{

				MMI_STRNTOWSTR( first_name_str.wstr_ptr, PHONE_NUM_MAX_LEN, (uint8 *)tele_num, PHONE_NUM_MAX_LEN, tele_num_len );
				first_name_str.wstr_len = MMIAPICOM_Wstrlen(first_name_str.wstr_ptr);
		}
		
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr =
                first_name_str.wstr_len == 0 ? last_name_str.wstr_ptr :
                first_name_str.wstr_ptr;
        item_data.item_content[1].item_data.text_buffer.wstr_len =
                first_name_str.wstr_len == 0 ? last_name_str.wstr_len :
                first_name_str.wstr_len;
        list_style = 1;
    }
    else
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_MASK_MS;
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = first_name_str.wstr_ptr;
        item_data.item_content[1].item_data.text_buffer.wstr_len = first_name_str.wstr_len;

        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = last_name_str.wstr_ptr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = last_name_str.wstr_len;
        list_style = 2;
    }
    if(is_abbr_valid)
    {
        uint8 abbr_content_index = list_style == 1 ? 2 : 3;

        item_data.item_content[abbr_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[abbr_content_index].item_data.text_buffer.wstr_ptr
                = abbr_name_str.wstr_ptr;
        item_data.item_content[abbr_content_index].item_data.text_buffer.wstr_len
                = abbr_name_str.wstr_len;
    }

    item_data.softkey_id[1] = find_result ? STXT_SOFTKEY_VIEW_MK :
            STXT_SOFTKEY_SAVE_MK;

    SCI_MEMSET(s_call_log_detail_info_ptr->tele_num, 0, (PHONE_NUM_MAX_LEN + 2));
    SCI_MEMCPY(s_call_log_detail_info_ptr->tele_num, tele_num, (PHONE_NUM_MAX_LEN + 2));

    GUILIST_AppendItem( ctrl_id, &item_t );
    return find_result;
}

LOCAL void AppendContactAnim(MMICL_CALL_BCDINFO_T *callinfo,
                                    void *item_ptr,
                                    MMI_WIN_ID_T win_id,
                                    MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_NEED_ITEM_CONTENT_T* item_content_ptr = item_ptr;
    MMIPB_BCD_NUMBER_T pb_num = {0};
    uint32 item_index  = 0;
    uint16 item_content_index = 0;
    GUILIST_ITEM_T* list_item_ptr = PNULL;
    wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
    uint32 custom_photo_name_len = 0;
    uint32 photo_file_size = 0;
    GUIANIM_FILE_INFO_T anim_path = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_index =((GUILIST_NEED_ITEM_CONTENT_T*)item_ptr)->item_index;
    list_item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,item_index);
    item_content_index = ((GUILIST_NEED_ITEM_CONTENT_T*)item_ptr)->item_content_index;

    SCI_MEMSET(&pb_num,0x00, sizeof(MMIPB_BCD_NUMBER_T));

    pb_num.number_len = callinfo->number_len;
    pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(callinfo->number_type,
    callinfo->number_plan);
    MMI_MEMCPY(pb_num.number,MMIPB_BCD_NUMBER_MAX_LEN,
    callinfo->number,MMIPB_BCD_NUMBER_MAX_LEN,pb_num.number_len);

    if(MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num,custom_photo_name, &custom_photo_name_len, &photo_file_size))
    {
        item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
        anim_path.full_path_wstr_ptr = custom_photo_name;
        anim_path.full_path_wstr_len = custom_photo_name_len;
        item_data.item_content[0].item_data.anim_path_ptr = &anim_path;
    }
    else
    {
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = IMAGE_CONTACTS_UNDEFINED_CONTACT;
    }
    GUILIST_SetItemContent( MMICL_LOG_LIST_DETAIL_CONTACT_LIST_CTRL_ID, &item_data.item_content[item_content_index],  item_index, item_content_index );
}

LOCAL BOOLEAN SetLogListDetailContactItem(MMICL_CALL_BCDINFO_T* p_callinfo)
{
    //uint16 sel_item_index = 0;
    uint16 current_index = 0;
    //MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_HANDLE_T    list_handle = MMICL_LOG_LIST_DETAIL_CONTACT_LIST_CTRL_ID;
    BOOLEAN find_result = FALSE;

    /*arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2626_112_2_18_2_11_32_54,(uint8*)"");
        return;
    }*/
    if(p_callinfo == PNULL)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2626_112_2_18_2_11_32_54,(uint8*)"");
        return FALSE;
    }
    current_index = GUILIST_GetCurItemIndex(list_handle);
    //SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    //GetCurrentRecordInfo(arraycall_info, &sel_item_index);
    //load log list
    GUILIST_SetMaxItem(list_handle, 1, FALSE);

    find_result = InitLogListDetailContactItem(list_handle, MMICL_CALL_ALL, p_callinfo);

    GUILIST_SetCurItemIndex(list_handle, current_index);
    return find_result;

    //SCI_FREE(arraycall_info);
}

LOCAL void HandleAddToPBOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = param->menu_id;
    switch (menu_id)
    {
        case ID_CL_ADD_TO_PB:
            HandleDistillPhoneNum();
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_CL_ADD_TO_EXIST_CONTACT:
            HandleAddNumToName();
            break;
#endif
    }
}

#ifdef PHONE_NUMBER_VOICE
static MMI_WIN_ID_T s_hero_loglistdetail_win_id = 0;
#endif

LOCAL MMI_RESULT_E  HandleLogListDetailWindow(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                              )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T ctrl_id = MMICL_LOG_LIST_DETAIL_FORM_CTRL_ID;
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    static MMICL_CALL_BCDINFO_T    *s_callinfo_ptr = PNULL;
    GUI_BG_T        bg_info = {0};
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    static BOOLEAN find_result = FALSE;
    MMI_STRING_T name_number_str ={0};
    wchar name_number_wchar[PHONE_NUM_MAX_LEN + 1]  ={0};
    uint16 current_index = 0;
    uint16 hor_space = 0;
    uint16 ver_space = 0;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};

    MMI_HANDLE_T win_form_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMICL_LOG_LIST_DETAIL_FORM_CTRL_ID);
    MMI_HANDLE_T list1_child_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMICL_LOG_LIST_DETAIL_CONTACT_LIST_CTRL_ID);
    MMI_HANDLE_T list2_child_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMICL_LOG_LIST_DETAIL_LIST_CTRL_ID);
    MMI_CTRL_ID_T active_ctl = MMICL_LOG_LIST_DETAIL_LIST_CTRL_ID;

    GUIFORM_CHILD_HEIGHT_T contact_item_height = {0};
    GUIFORM_CHILD_HEIGHT_T list_height = {0};

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    bg_info.bg_type = GUI_BG_COLOR;
    bg_info.color = MMI_WHITE_COLOR;

    GUIFORM_SetBg(win_form_ctrl_id, &bg_info);
    GUIFORM_SetSpace(win_form_ctrl_id, &hor_space, &ver_space);
    //GUIFORM_SetChildDock(win_form_ctrl_id,list1_child_ctrl_id, GUIFORM_CHILD_DOCK_TOP);
    GUIFORM_SetType(win_form_ctrl_id,GUIFORM_TYPE_TP);
    GUIFORM_SetCircularHandleUpDown(win_form_ctrl_id,TRUE);
    GUIFORM_SetDividingLine(win_form_ctrl_id,0,PNULL);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

#ifdef PHONE_NUMBER_VOICE
				Trace_Log_Buf_Print(" zdthero000	HandleLogListDetailWindow  000 ");

				s_hero_loglistdetail_win_id = win_id;
#endif
		
        if (s_call_log_detail_info_ptr == PNULL)
        {
            s_call_log_detail_info_ptr = (MMICL_DETAIL_INFO_T *)SCI_ALLOCAZ(sizeof(MMICL_DETAIL_INFO_T));
        }
        if (arraycall_info == PNULL)
        {
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        }
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        GetCurrentRecordInfo(arraycall_info, &current_index);
        s_callinfo_ptr = &(arraycall_info->call_bcdinfo[s_group_list_info[current_index].position[0]]);

        contact_item_height.add_data = DETAIL_VIEW_CONTACT_ITEM_HEIGHT;
        contact_item_height.type     = GUIFORM_CHILD_HEIGHT_FIXED;
        GUIFORM_SetChildHeight(win_form_ctrl_id,list1_child_ctrl_id,&contact_item_height);
        list_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(win_form_ctrl_id, list2_child_ctrl_id,
                &list_height);
        GUIFORM_SetChildUpDownPostion(win_form_ctrl_id,TRUE);

        find_result = SetLogListDetailContactItem(s_callinfo_ptr);
        SetLogListDetailItem();
        MMK_SetAtvCtrl(win_id, list2_child_ctrl_id);
        MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_MENU_HDR_LOG_EXT02);
        break;

    case MSG_GET_FOCUS:
        MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_MENU_HDR_LOG_EXT02);
        find_result = SetLogListDetailContactItem(s_callinfo_ptr);
        //InitLogListDetailContactItem(list1_child_ctrl_id,MMICL_CALL_ALL,s_callinfo_ptr);
        break;

    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        if (MMICL_LOG_LIST_DETAIL_LIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        }
        break;
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        if (MMICL_LOG_LIST_DETAIL_LIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            HandleRingBackByGroup(s_group_list_info, arraycall_info, CC_CALL_SIM_MAX, FALSE);
            s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        }
        break;
    case MSG_CTL_MIDSK:
        active_ctl = MMK_GetActiveCtrlId(win_id);
        switch (active_ctl)
        {
        case MMICL_LOG_LIST_DETAIL_LIST_CTRL_ID:
            HandleRingBackByGroup(s_group_list_info, arraycall_info, CC_CALL_SIM_MAX, FALSE);
            s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
            break;
        case MMICL_LOG_LIST_DETAIL_CONTACT_LIST_CTRL_ID:
            //name_number_str.wstr_ptr = name_number_wchar;
            //find_result = MMIAPICL_GetNameByClInfo(*s_callinfo_ptr, &name_number_str, FALSE);
            if ( find_result )
            {
                HandleContactView();
            }
            else
            {
                //HandleDistillPhoneNum();
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_CL_ADD_TO_PB;
                menuparam.win_id =   MMICL_ADD_TO_PB_MENU_WIN_ID;
                menuparam.ctrl_id =  MMICL_ADD_TO_PB_MENU_CTRL_ID;
                menuparam.dst_handle = win_id;
                menuparam.func.OnMenuOk = HandleAddToPBOptOk;
                MMIPUB_OpenPubMenuWin( &menuparam );
            }
            break;
        default:
            break;
        }
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (MMICL_LOG_LIST_DETAIL_LIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            if(FALSE == MMIAPISET_IsDisableShareViaOption())
            {
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIL_MENU_WIN_TAB, (ADD_DATA)find_result);
            }
        }
        break;
    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
        AppendContactAnim(s_callinfo_ptr, param, win_id, MMICL_LOG_LIST_DETAIL_CONTACT_LIST_CTRL_ID);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
        }
        if (PNULL != s_call_log_detail_info_ptr)
        {
            SCI_FREE(s_call_log_detail_info_ptr);
        }
#ifdef PHONE_NUMBER_VOICE
				s_hero_loglistdetail_win_id = 0;
#endif
		
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return (recode);
}
//end

#ifdef PHONE_NUMBER_VOICE
PUBLIC unsigned int hero_LogListDetail_GetWinId(void)
{
	return s_hero_loglistdetail_win_id;
}

PUBLIC void hero_LogListDetail_ResetWinId(void)
{
	s_hero_loglistdetail_win_id = 0;
}
#endif

/*****************************************************************************/
// Description : Get Current Record Info for next operation
// Global resource dependence : 
//  Author:tao.xue
// Note: 
/*****************************************************************************/
LOCAL void GetCurrentRecordInfo(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info_ptr , uint16* sel_item_index_ptr)
{
    uint16   sel_item_index =0;
    MMI_CTRL_ID_T list_ctrl_id = 0;
    
    list_ctrl_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);

    //read logs of calls from NV
#ifdef MMI_PDA_SUPPORT
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info_ptr);
        
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        sel_item_index = s_match_call_info_ptr->call_info[s_mmicl_match_item_index].array_index;
    }
    else
    {        
        sel_item_index = CL_GetArrayIndexofCurSelected();
    }
#else
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        sel_item_index = s_match_call_info_ptr->call_info[s_mmicl_match_item_index].array_index;
        MMICL_GetRecordInfo(s_match_call_info_ptr->call_info[sel_item_index].call_type, arraycall_info_ptr);
        sel_item_index = s_match_call_info_ptr->call_info[sel_item_index].array_index;
    }
    else
    {
        MMICL_GetRecordInfo(/*s_cur_sel_log_list_type*/MMICL_CALL_ALL, arraycall_info_ptr);
        sel_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    }
#endif

    *sel_item_index_ptr = sel_item_index;
}
/*****************************************************************************/
// 	Description : delete current recode item
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DeleteCurrentRecord(void)
{
    uint8       record_num = 0;
    uint16      array_index = 0;

    array_index = CL_GetArrayIndexofCurSelected();

    //delete appointed record from nv
    record_num = MMICL_DeleteGroupAppointedCallInfo(&s_group_list_info[array_index],(uint8)array_index);

    //success!
    //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);


//    if (0 == record_num) //û�м�¼
//    {
        // close log list window
//        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
//    }

    //close the window of call log list menu and detail win
    //MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_REFUSED_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
}

/*****************************************************************************/
// 	Description : ring back (�ظ��绰)
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleRingBack(CC_CALL_SIM_TYPE_E call_sim_type, 
                                                BOOLEAN is_video_call
                                                )
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint32 call_type = CC_CALL_NORMAL_CALL;
    MMI_STRING_T *name_ptr = PNULL;
#if defined(CL_SAVE_NAME_SUPPORT)
    MMI_STRING_T  call_name = {0};
    wchar temp_str[MMICL_NAME_MAX_LEN +1] = {0};
#endif

#if defined(CL_SAVE_NAME_SUPPORT)
    call_name.wstr_ptr = temp_str;
    if (GetDisplayPhoneName(&call_name))
    {
        name_ptr = &call_name;
    }
    else
    {
        name_ptr = PNULL;
    }
#endif
    //get the phone number
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);
    
    num_len = (uint8)strlen((char*)tele_num);
    
    //call
    if ( num_len > 0 )
    {
        if (is_video_call)
        {
            call_type = CC_CALL_VIDEO_CALL;
        }
        else
        {
            call_type = CC_CALL_NORMAL_CALL;
        }
        MMIAPICC_MakeCall(
                                        dual_sys,
                                        tele_num,
                                        num_len,
                                        PNULL,
                                        name_ptr,
                                        call_sim_type,
                                        call_type,
                                        PNULL
                                        );
    }
    else
    {
        //��ʾ�պ�!
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
    
}

//#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : ring back (�ظ��绰)
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleRingBackByType(uint32 call_type)
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    BOOLEAN  is_video_call = FALSE;


    //get the phone number
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);

    num_len = (uint8)strlen((char*)tele_num);

    if (call_type == CC_CALL_DEFAULT_CALL)
    {
        //Get Current log type, if video call,call video call back, if audio call,audio call back
 #ifdef TOUCH_PANEL_SUPPORT
       GetCallLogInfoDetail(&dual_sys,&is_video_call);
#endif
        call_type = CC_CALL_NORMAL_CALL;
    }
    //call
    if ( num_len > 0 )
    {
        if (is_video_call)
        {
            call_type = CC_CALL_VIDEO_CALL;
        }
        MMIAPICC_MakeCall(
                        dual_sys,
                        tele_num,
                        num_len,
                        PNULL,
                        PNULL,
                        CC_CALL_SIM_MAX,
                        call_type,
                        PNULL
                        );
    }
    else
    {
        //��ʾ�պ�!
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CL_EMPTY_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }

}
//#endif
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
// 	Description : ip call
//	Global resource dependence : 
//  Author:lin.lin
//	Note: 
/*****************************************************************************/
LOCAL void HandleIpCall(void)
{
    uint8       tele_num[PHONE_NUM_MAX_LEN + 2] = {0},num_len = 0;
    MN_DUAL_SYS_E dual_sys_dummy = MN_DUAL_SYS_1;
    MMI_STRING_T *name_ptr = PNULL;
#if defined(CL_SAVE_NAME_SUPPORT)
    MMI_STRING_T  call_name = {0};
    wchar temp_str[MMICL_NAME_MAX_LEN +1] = {0};
#endif

#if defined(CL_SAVE_NAME_SUPPORT)
    call_name.wstr_ptr = temp_str;
    if (GetDisplayPhoneName(&call_name))
    {
        name_ptr = &call_name;
    }
    else
    {
        name_ptr = PNULL;
    }
#endif
    //get the phone number
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys_dummy);
        
    num_len = (uint8)strlen((char*)tele_num);
    
    //call
    if (num_len > 0)
    {
        MMIAPICC_MakeCall(
                                        dual_sys_dummy,
                                        tele_num,
                                        num_len,
                                        PNULL,
                                        name_ptr,
                                        CC_CALL_SIM_MAX,
                                        CC_CALL_IP_CALL,
                                        PNULL
                                        );
    }
    else
    {
        //��ʾ�պ�!
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}
#endif

/*****************************************************************************/
// 	Description : get current log list control id
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
PUBLIC CC_CONTROL_ID_E MMICL_GetLogListCtrlId(MMICL_CALL_TYPE_E call_type)
{
    return MMICL_CUSTOM_LOG_LIST_CTRL_ID;
}

#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
#ifdef MMI_CL_TOOLBAR_SUPPORT
/*****************************************************************************/
// 	Description : get current tool bar log list control id
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL CC_CONTROL_ID_E GetLogToolBarCtrlId(MMI_WIN_ID_T win_id)
{
    CC_CONTROL_ID_E result = MMICL_TOOLBAR_DIALED_CTRL_ID;

    switch (win_id)
    {
        case MMICL_DIALED_CHILD_WIN_ID:
            return MMICL_TOOLBAR_DIALED_CTRL_ID;
        case MMICL_RECEIVED_CHILD_WIN_ID:
            return MMICL_TOOLBAR_RECEIVED_CTRL_ID;
        case MMICL_MISSED_CHILD_WIN_ID:
            return MMICL_TOOLBAR_MISSED_CTRL_ID;
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_REFUSED_CHILD_WIN_ID:
            return MMICL_TOOLBAR_REFUSED_CTRL_ID;
#endif            
        default:
            break;
    }
    return MMICL_TOOLBAR_MISSED_CTRL_ID;
}
#endif
#endif
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
// 	Description : add phone number to name 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAddNumToName(void)
{
    MMIPB_BCD_NUMBER_T  tele_num = {0};
    uint8  num_ton = 0;//@fen.xie MS00180421
    BOOLEAN is_plus_symbol = FALSE;
    BOOLEAN is_num_valid = TRUE;
    uint8   tele_string[PHONE_NUM_MAX_LEN + 2] = {0};
	uint8   tele_length = 0;
    
    //get the phone number(bcd)
    GetPartyPhoneNum(&tele_num);
    
    //+bug132483
    tele_length = MMIAPIPB_BCDNumberToString(&tele_num, PHONE_NUM_MAX_LEN + 2, tele_string);
    is_num_valid = MMIAPICOM_IsValidNumberString((char *)tele_string, tele_length);
	SCI_TRACE_LOW("yanyan.an is_num_valid:%d", is_num_valid);
	SCI_TRACE_BUF("yanyan.an tele_num.number:%d", tele_string, tele_length);
	if (FALSE == is_num_valid)
	{
		//��ʾ��Ч����
        MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        return; 
	}
    //-bug132483
    
    num_ton = tele_num.npi_ton>> 4;//@fen.xie MS00180421
    if ((0 == tele_num.number_len) && (MN_NUM_TYPE_INTERNATIONAL == num_ton))/*@fen.xie MS00180421: '+' */
    {
        is_plus_symbol = TRUE;
    }

    //call
    if ((tele_num.number_len > 0) || is_plus_symbol)
    {
        MMIAPIPB_AddToExistContact(&tele_num, MMIPB_ADD_TYPE_NUMBER, PNULL);
    }
    else
    {
        //��ʾ����û�к���
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CL_EMPTY_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}
#endif
#if defined(CL_SAVE_NAME_SUPPORT)
/*****************************************************************************/
// 	Description : get display phone name of current log list 
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayPhoneName(
                              MMI_STRING_T*   call_name//out
                              )
{
    if(PNULL == call_name ||PNULL == call_name->wstr_ptr)
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneName call_name OR call_name->wstr_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8509_112_2_18_2_11_44_77,(uint8*)"");
        return FALSE;
    }

    //���ͨ����¼���鴰�ڴ򿪣�����Ӵ�����Ϣ�л��
    if (MMK_IsFocusWin(MMICL_LOG_LIST_DETAIIL_WIN_ID) && (PNULL != s_call_log_detail_info_ptr))
    {
        if (s_call_log_detail_info_ptr->name.is_use_nv_name)
        {
            call_name->wstr_len = s_call_log_detail_info_ptr->name.wstr_len;
            MMI_WSTRNCPY(call_name->wstr_ptr,
                                        MMICL_NAME_MAX_LEN, 
                                        s_call_log_detail_info_ptr->name.wstr, 
                                        MMICL_NAME_MAX_LEN, 
                                        call_name->wstr_len);

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    //������б��л��
    else
    {
        uint16 array_index = 0;
        MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"GetDisplayPhoneNum call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8544_112_2_18_2_11_44_78,(uint8*)"");
            return FALSE;
        }
    
        SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

        if(PNULL != s_match_call_info_ptr)
        {
            s_cur_sel_log_list_type = MMICL_CALL_MATCHED;
        }
        GetCurrentRecordInfo(arraycall_info, &array_index);
        
        if (arraycall_info->call_bcdinfo[array_index].name.is_use_nv_name)
        {
            call_name->wstr_len = arraycall_info->call_bcdinfo[array_index].name.wstr_len;
            MMI_WSTRNCPY(call_name->wstr_ptr, 
                                        MMICL_NAME_MAX_LEN, 
                                        arraycall_info->call_bcdinfo[array_index].name.wstr, 
                                        MMICL_NAME_MAX_LEN, 
                                        call_name->wstr_len);
        
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
            return TRUE;
        }
        else
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
            return FALSE;
        }
    }
}
#endif

/*****************************************************************************/
//  Description : get display phone number of current log list 
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void GetDisplayPhoneNum(
                              uint8*    tele_num, //out
                              uint8    max_tele_len,
                              MN_DUAL_SYS_E * dual_sys_ptr
                              )
{
    if(max_tele_len > (PHONE_NUM_MAX_LEN + 2))
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneNum max_tele_len > (PHONE_NUM_MAX_LEN + 2)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8591_112_2_18_2_11_44_79,(uint8*)"");
        return;
    }

    if(PNULL == tele_num ||PNULL == dual_sys_ptr)
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneNum PNULL == tele_num ||PNULL == dual_sys_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8597_112_2_18_2_11_44_80,(uint8*)"");
        return;
    }

    //���ͨ����¼���鴰�ڴ򿪣�����Ӵ�����Ϣ�л��
    if ((MMK_IsFocusWin(MMICL_LOG_LIST_DETAIIL_WIN_ID)
    #ifdef CALL_TIMES_SUPPORT
        ||MMK_IsFocusWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID)
    #endif
        )&& (PNULL != s_call_log_detail_info_ptr))
    {
        SCI_MEMCPY(tele_num, s_call_log_detail_info_ptr->tele_num, max_tele_len);
        *dual_sys_ptr = s_call_log_detail_info_ptr->dual_sys;
    }
    //������б��л��
    else
    {
        uint16 list_index = 0;
        uint16 array_index = 0;
        MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;

        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"GetDisplayPhoneNum call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8620_112_2_18_2_11_44_81,(uint8*)"");
            return;
        }

        SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

        if(PNULL != s_match_call_info_ptr)
        {
            s_cur_sel_log_list_type = MMICL_CALL_MATCHED;
        }
        GetCurrentRecordInfo(arraycall_info, &list_index);
        array_index= s_group_list_info[list_index].position[0];

        if (arraycall_info->call_bcdinfo[array_index].is_num_present)//�绰�������
        {
            //get number
            MMICL_GenDispNumber(arraycall_info->call_bcdinfo[array_index].call_type,
                arraycall_info->call_bcdinfo[array_index].number_type,
                arraycall_info->call_bcdinfo[array_index].number_len,
                arraycall_info->call_bcdinfo[array_index].number,
                tele_num,
                max_tele_len);
        }

        *dual_sys_ptr = arraycall_info->call_bcdinfo[array_index].dual_sys;

        //if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
        }
    }
}

/*****************************************************************************/
// 	Description : distill phone number from call log 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleDistillPhoneNum(void)
{
    MMIPB_BCD_NUMBER_T  tele_num = {0};
    uint8  num_ton = 0;//@fen.xie MS00180421
    BOOLEAN is_plus_symbol = FALSE;
	BOOLEAN is_num_valid = TRUE;
    uint8   tele_string[PHONE_NUM_MAX_LEN + 2] = {0};
	uint8   tele_length = 0;

    //get the phone number(bcd)
    GetPartyPhoneNum(&tele_num);

    //+bug132483
    tele_length = MMIAPIPB_BCDNumberToString(&tele_num, PHONE_NUM_MAX_LEN + 2, tele_string);
    is_num_valid = MMIAPICOM_IsValidNumberString((char *)tele_string, tele_length);
	SCI_TRACE_LOW("yanyan.an is_num_valid:%d", is_num_valid);
	SCI_TRACE_BUF("yanyan.an tele_num.number:%d", tele_string, tele_length);
	if (FALSE == is_num_valid)
	{
		//��ʾ��Ч����
        MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        return; 
	}
    //-bug132483
    
    num_ton = tele_num.npi_ton>> 4;//@fen.xie MS00180421
    if ((0 == tele_num.number_len) && (MN_NUM_TYPE_INTERNATIONAL == num_ton))/*@fen.xie MS00180421: '+' */
    {
        is_plus_symbol = TRUE;
    }

    if ((tele_num.number_len > 0) || is_plus_symbol)
    {
        MMIAPIPB_AddContactWin(&tele_num,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
        //MMIAPIPB_OpenAddMenu(&tele_num,PB_GROUP_ALL);
    }
    else
    {
        //��ʾ����û�к���
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
// 	Description : add a number into forbidden number list
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleForbiddenNum(void)
{
    MMIPB_BCD_NUMBER_T  tele_num = {0};
    uint8  num_ton = 0;//@fen.xie MS00180421
    BOOLEAN is_plus_symbol = FALSE;
    MMI_STRING_T mmi_calllog_name ={0};
    MMISET_BLACKLIST_ERROR_TYPE_E add_blacklist_result = MMISET_BLACKLIST_ERROR_MAX;
    
    //get the phone number(bcd)
    GetPartyPhoneNum(&tele_num);

    num_ton = tele_num.npi_ton>> 4;//@fen.xie MS00180421
    if ((0 == tele_num.number_len) && (MN_NUM_TYPE_INTERNATIONAL == num_ton))/*@fen.xie MS00180421: '+' */
    {
        is_plus_symbol = TRUE;
    }

    if ((tele_num.number_len > 0) || is_plus_symbol)
    {
        MMIAPIPB_GetNameByNumber(&tele_num,
                                  &mmi_calllog_name,
                                  MMICL_NAME_MAX_LEN,
                                  TRUE  //use default name,"no name"
                                  );
        add_blacklist_result = MMIAPISET_AddBlacklistFromExt(&mmi_calllog_name,&tele_num);      
        
        if(MMISET_BLACKLIST_ERROR_NONE == add_blacklist_result)
        {
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        }
        else if(MMISET_BLACKLIST_ERROR_BEYOND_MAX_NUM== add_blacklist_result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);            
        }
        else if(MMISET_BLACKLIST_ERROR_SAME_NUM == add_blacklist_result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        }
        if(MMK_IsOpenWin( MMICL_LOG_LIST_MENU_WIN_ID))
        {
            MMK_CloseWin( MMICL_LOG_LIST_MENU_WIN_ID);
        }
    }
    else
    {
        //��ʾ����û�к���
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}
#endif

/*****************************************************************************/
// 	Description : get bcd phone number of current log list 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void GetPartyPhoneNum(
                            MMIPB_BCD_NUMBER_T*   bcd_number//out
                            )
{
    uint16                          group_index = 0;
    uint16                          record_index = 0;

    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetPartyPhoneNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8757_112_2_18_2_11_45_82,(uint8*)"");
        return;
    }

    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    GetCurrentRecordInfo(arraycall_info, &group_index);

    if(PNULL == bcd_number)
    {
        //SCI_TRACE_LOW:"GetPartyPhoneNum PNULL == bcd_number"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8767_112_2_18_2_11_45_83,(uint8*)"");
        return;
    }


    record_index = s_group_list_info[group_index].position[0];
    if (arraycall_info->call_bcdinfo[record_index].is_num_present)//�绰�������
    {
        //get number
        bcd_number->number_len = arraycall_info->call_bcdinfo[record_index].number_len;
        bcd_number->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(arraycall_info->call_bcdinfo[record_index].number_type,
            arraycall_info->call_bcdinfo[record_index].number_plan);
        MMI_MEMCPY(bcd_number->number,MMIPB_BCD_NUMBER_MAX_LEN,
            arraycall_info->call_bcdinfo[record_index].number,MMIPB_BCD_NUMBER_MAX_LEN,
            bcd_number->number_len);
    }

    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
    }
}

PUBLIC  void MMIAPICL_HandleAnswerSms(void)
{
	HandleAnswerSms();
}

/*****************************************************************************/
// 	Description : answer sms back
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAnswerSms(void)
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    
    //get the phone number
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);
    num_len = (uint8)strlen((char*)tele_num);
    
    //call
    if ( num_len > 0 )
    {
	if( SCI_FALSE == MMIAPISMS_OpenConfirmation(PNULL,3))//MMISMS_ENTER_BY_CL
	{
		return  ;
	}

        MMIAPISMS_ExitFromSMS();

        //����sms�ӿں���
        MMIAPISMS_AnswerMessage(dual_sys ,PNULL,tele_num,num_len);
    }
    else
    {
        //��ʾ����û�к���
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}

/*****************************************************************************/
// 	Description : answer sms back
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAnswerMms(void)
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len                         = 0;
    MN_DUAL_SYS_E dual_sys                = MN_DUAL_SYS_1;
//     uint32 record_id                      = 0;
    MMI_STRING_T string_to = {0};
    
    if (MMIAPIUDISK_UdiskIsRun()) //U��ʹ����
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }
    
    //get the phone number
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);
    num_len = (uint8)strlen((char*)tele_num);
    
    //call
    if ( num_len > 0 )
    {
        //����mms�ӿں���
        string_to.wstr_ptr= (wchar*)SCI_ALLOC_APP(num_len * sizeof(wchar));
        MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (uint8 *)tele_num, num_len, num_len);
        string_to.wstr_len = num_len;
        MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_CL_SEND);
        MMIAPIMMS_AnswerMMS(dual_sys,PNULL,&string_to);
        SCI_FREE(string_to.wstr_ptr);        
    }
    else
    {
        //��ʾ����û�к���
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}


/*****************************************************************************/
// 	Description : delete all call log with menu id
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
#ifdef MMI_PDA_SUPPORT
LOCAL void DeleteAllCallLog(
                            MMICL_CALL_TYPE_E  call_type//in
                            )
{
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_info = PNULL;
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_last_info = PNULL;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    uint8 i = 0,count = 0;
    BOOLEAN result = FALSE;
   
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"DeleteAllCallLog call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8877_112_2_18_2_11_45_84,(uint8*)"");
        return;
    }   
    if(call_type == MMICL_CALL_ALL)
    {
        if (MMICL_CALL_ALL==GetAllLogFillter())
        {
            SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

            MMICL_WriteNV(MMICL_CALL_ALL, arraycall_info);
            
            //if (PNULL != missed_call)
            {
                SCI_FREE(arraycall_info);
                arraycall_info = PNULL;
            } 
#ifdef MMI_LDN_SUPPORT
            result = MMIAPICL_DeleteAllLdnInfo(MMICL_CALL_DIALED);
#endif
            return;
         }
    }
    arraycall_last_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_last_info)
    {
        //SCI_TRACE_LOW:"DeleteAllCallLog call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8899_112_2_18_2_11_45_85,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(arraycall_last_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    //get the value of arraycall_info from nv
    return_value = MMICL_ReadAllNV(arraycall_info);
    for(i = 0; i <arraycall_info->record_num; i++)
    {
        if (call_type != MMICL_CALL_ALL)
        {
            if(call_type != arraycall_info->call_bcdinfo[i].call_type)
            {
                memcpy(&arraycall_last_info->call_bcdinfo[count++], &arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );
            }
        }
        else
        {
            if(GetAllLogFillter()==MMICL_CALL_PB_NUMBER_MATCHED)
            {
                //Delete selected item
                if (CalllogPBData_GetIsSelectedByIndex(i))
                {
                    continue;
                }
				memcpy(&arraycall_last_info->call_bcdinfo[count++], &arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );				
            }
            else if( arraycall_info->call_bcdinfo[i].call_type!=GetAllLogFillter())
            {
                memcpy(&arraycall_last_info->call_bcdinfo[count++], &arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );
            }        
            else
            {
                continue;
            }
        }
    }
    arraycall_last_info->record_num = count;
    
    MMICL_WriteNV(MMICL_CALL_ALL, arraycall_last_info);
    
    //if (PNULL != missed_call)
    {
        SCI_FREE(arraycall_info);
        arraycall_info = PNULL;
        SCI_FREE(arraycall_last_info);
        arraycall_last_info = PNULL;
    }
   
    //SCI_TRACE_LOW:"DeleteAllCallLog: the call_type is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8950_112_2_18_2_11_45_86,(uint8*)"d",call_type);
}
#else
/*****************************************************************************/
//  Description : delete all call log with menu id
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void DeleteAllCallLog(
                            MMICL_CALL_TYPE_E  call_type//in
                            )
{
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_info = PNULL;
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_last_info = PNULL;
    uint8 i = 0,count = 0;
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
    uint16 cur_count=0;
#endif

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"DeleteAllCallLog call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8969_112_2_18_2_11_45_87,(uint8*)"");
        return;
    }
    if(call_type == MMICL_CALL_ALL) //All .memset it and write NV
    {
        SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        MMICL_WriteNV(MMICL_CALL_ALL, arraycall_info);
        SCI_FREE(arraycall_info);
        return;
    }

    //If no,copy and overwrite
    arraycall_last_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_last_info)
    {
        SCI_FREE(arraycall_info);
        //SCI_TRACE_LOW:"DeleteAllCallLog call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8985_112_2_18_2_11_45_88,(uint8*)"");
        return;
    }

    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(arraycall_last_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    //get the value of arraycall_info from nv
    MMICL_ReadNV(MMICL_CALL_ALL,arraycall_info);
    for(i = 0; i <arraycall_info->record_num; i++)
    {
        if (call_type != MMICL_CALL_ALL)
        {
            if(call_type != arraycall_info->call_bcdinfo[i].call_type)
            {
                memcpy(&arraycall_last_info->call_bcdinfo[count++], &arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );
            }
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
            else
            {
                if (isMultiSelecting())
                {
                    if (!isCurrentSelected(cur_count))
                    {
                        memcpy(&arraycall_last_info->call_bcdinfo[count++], &arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );
                    }
                    cur_count++;
                }
            }
#endif
        }
    }
    arraycall_last_info->record_num = count;

    MMICL_WriteNV(MMICL_CALL_ALL, arraycall_last_info);

    SCI_FREE(arraycall_info);
    SCI_FREE(arraycall_last_info);

    //SCI_TRACE_LOW:"DeleteAllCallLog: the call_type is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9012_112_2_18_2_11_45_89,(uint8*)"d",call_type);
#ifdef MMI_LDN_SUPPORT
    if(!MMIAPICL_DeleteAllLdnInfo(call_type))
#endif
    {
       // MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
    }
}
#endif

/*****************************************************************************/
// 	Description : display the time of last call
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimerWindow(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    GUI_RECT_T      rect = MMITHEME_GetClientRect();
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T ctrl_id = MMICL_CALL_TIMER_CTRL_ID;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
#ifdef MMI_CSP_SUPPORT    
    uint8 line = MMIAPISET_GetCallLineInfo(dual_sys);
#endif    
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
        
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetTitleTextId(win_id,(MMI_TEXT_ID_T)TXT_CL_CALL_TIMERS, FALSE);
        #ifdef MMI_PDA_SUPPORT
        GUILIST_SetMaxItem(ctrl_id, 100, FALSE);
        #endif
        //set softkey
        #ifndef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_CL_RESET,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN,FALSE);
        #endif
        //GUIRICHTEXT_SetScroll(ctrl_id, TRUE);
        {
            MMI_STRING_T item_str = {0};
            uint16 display_index = 0;
            SCI_MEMSET(&item_str,0,sizeof(MMI_STRING_T));
            //set name 
#ifdef MMI_CSP_SUPPORT
            DisplayCallTimeItem(line, MMINV_CL_LASTCALL_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_TIMER_LAST_CALL);//????
            DisplayCallTimeItem(line, MMINV_CL_RECEIVEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_RECEIVED_CALLS);//????
            DisplayCallTimeItem(line, MMINV_CL_DIALEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DIALED_CALLS);//????
            DisplayCallTimeItem(line, MMINV_CL_ALLCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DEL_ALLLOGS);//????  
#else //MMI_CSP_SUPPORT
            DisplayCallTimeItem(MMINV_CL_LASTCALL_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_TIMER_LAST_CALL);//????
            DisplayCallTimeItem(MMINV_CL_RECEIVEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_RECEIVED_CALLS);//????
            DisplayCallTimeItem(MMINV_CL_DIALEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DIALED_CALLS);//????
            DisplayCallTimeItem(MMINV_CL_ALLCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DEL_ALLLOGS);//????  
#endif //MMI_CSP_SUPPORT 
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef   MMI_PDA_SUPPORT      
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
 case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                break;            
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, rect, MMI_GRAY_WHITE_COLOR);
        break;
   #ifdef  MMI_PDA_SUPPORT   //not respond menu key
    case MSG_GET_FOCUS:
        {
            uint16 display_index = 0;
            GUILIST_RemoveAllItems(ctrl_id);
            //set name 
#ifdef MMI_CSP_SUPPORT
            DisplayCallTimeItem(line, MMINV_CL_LASTCALL_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_TIMER_LAST_CALL);//????
            DisplayCallTimeItem(line, MMINV_CL_RECEIVEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_RECEIVED_CALLS);//????
            DisplayCallTimeItem(line, MMINV_CL_DIALEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DIALED_CALLS);//????
            DisplayCallTimeItem(line, MMINV_CL_ALLCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DEL_ALLLOGS);//????  
#else //MMI_CSP_SUPPORT
            DisplayCallTimeItem(MMINV_CL_LASTCALL_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_TIMER_LAST_CALL);//????
            DisplayCallTimeItem(MMINV_CL_RECEIVEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_RECEIVED_CALLS);//????
            DisplayCallTimeItem(MMINV_CL_DIALEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DIALED_CALLS);//????
            DisplayCallTimeItem(MMINV_CL_ALLCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DEL_ALLLOGS);//????  
#endif //MMI_CSP_SUPPORT            
        }
        break;
   #endif
         
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
        #ifndef  MMI_PDA_SUPPORT   //not respond menu key
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        
        MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
        break;
        #endif
    case MSG_PROMPTWIN_OK:
        //�ر�ѯ�ʴ���
        MMIPUB_CloseQuerytWin(&query_win_id);
        ResetCalllogTimer(dual_sys);
//        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&query_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //�ر�ѯ�ʴ���
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : display call time in control
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DisplayCallTimeItem(
                            uint8 line, 
                            CC_NV_ITEM_E   nv_item, //@$discuss 6800????CC_NV_ITEM_E,????????MMI_NV_ITME_E
                            MN_DUAL_SYS_E dual_sys,
                            MMI_CTRL_ID_T ctrl_id,
                            uint16* index_ptr,
                            MMI_TEXT_ID_T head_text_id
                            )
{
    uint8               time[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    wchar               time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    CC_CALL_TIME_COUNT_T call_time = {0};/*lint !e64*/
    MMI_STRING_T        time_str = {0};
//     MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    switch(nv_item)
    {
    case MMINV_CL_LASTCALL_TIME:
        MMIAPICL_GetLastCallTime(line, &call_time, dual_sys);
        break;

    case MMINV_CL_ALLCALLS_TIME:
        MMIAPICL_GetAllCallsTime(line, &call_time, dual_sys);
        break;

    case MMINV_CL_RECEIVEDCALLS_TIME:
        MMIAPICL_GetReceivedCallsTime(line, &call_time, dual_sys);
        break;

    case MMINV_CL_DIALEDCALLS_TIME:
        MMIAPICL_GetDialedCallsTime(line, &call_time, dual_sys);
        break;
        
    default:   
        //SCI_TRACE_LOW:"SetCallTimeLabel:nv_item %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9180_112_2_18_2_11_46_90,(uint8*)"d",nv_item);
    }

    if(0 == line)
    {
        call_time.line1_call_time = MIN(call_time.line1_call_time, MMICL_TIMER_MAX);
        //divert time type
        ConvertTimeToDisplay(call_time.line1_call_time, time);

    }
    else if(1 == line)
    {
        call_time.line2_call_time = MIN(call_time.line2_call_time, MMICL_TIMER_MAX);
        //divert time type
        ConvertTimeToDisplay(call_time.line2_call_time, time);

    }

    time_str.wstr_ptr = time_wstr;
    //time_str.is_ucs2 = FALSE;
    time_str.wstr_len = (uint16)strlen((char*)time);
    MMI_STRNTOWSTR( time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)time, time_str.wstr_len, time_str.wstr_len );
    
    SetHeadAndContentInRText(head_text_id, ctrl_id, time_str, index_ptr);

}
#else //MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : display call time in control
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DisplayCallTimeItem(
                            CC_NV_ITEM_E   nv_item, //@$discuss 6800���õ���CC_NV_ITEM_E,��ʵ�ʵ��õĻ���MMI_NV_ITME_E
                            MN_DUAL_SYS_E dual_sys,
                            MMI_CTRL_ID_T ctrl_id,
                            uint16* index_ptr,
                            MMI_TEXT_ID_T head_text_id
                            )
{
    uint8               time[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    wchar               time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    CC_CALL_TIME_COUNT_T call_time = {0};/*lint !e64*/
    MMI_STRING_T        time_str = {0};
//     MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    switch(nv_item)
    {
    case MMINV_CL_LASTCALL_TIME:
        MMIAPICL_GetLastCallTime(&call_time, dual_sys);
        break;

    case MMINV_CL_ALLCALLS_TIME:
        MMIAPICL_GetAllCallsTime(&call_time, dual_sys);
        break;

    case MMINV_CL_RECEIVEDCALLS_TIME:
        MMIAPICL_GetReceivedCallsTime(&call_time, dual_sys);
        break;

    case MMINV_CL_DIALEDCALLS_TIME:
        MMIAPICL_GetDialedCallsTime(&call_time, dual_sys);
        break;
        
    default:   
        //SCI_TRACE_LOW:"SetCallTimeLabel:nv_item %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9180_112_2_18_2_11_46_90,(uint8*)"d",nv_item);
        break;
    }
    
    call_time.call_time = MIN(call_time.call_time, MMICL_TIMER_MAX);

    //divert time type
    ConvertTimeToDisplay(call_time.call_time, time);
    time_str.wstr_ptr = time_wstr;
    //time_str.is_ucs2 = FALSE;
    time_str.wstr_len = (uint16)strlen((char*)time);
    MMI_STRNTOWSTR( time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)time, time_str.wstr_len, time_str.wstr_len );
    
    SetHeadAndContentInRText(head_text_id, ctrl_id, time_str, index_ptr);

}
#endif //MMI_CSP_SUPPORT

/*****************************************************************************/
// 	Description : ת��uint32��ʱ��Ϊʱ���֣���ĸ�ʽ
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void ConvertTimeToDisplay(uint32  call_time,//in
                                uint8   *time//out
                                )
{
    uint8      sec     = 0;     // seconds after the minute - [0,59] 
    uint8      min     = 0;     // minutes after the hour - [0,59] 
    uint32     hour    = 0;    
    
    if(PNULL == time)
    {
        //SCI_TRACE_LOW:"ConvertTimeToDisplay  time is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9210_112_2_18_2_11_46_91,(uint8*)"");
        return;
    }
    
    sec       = (uint8)(call_time % 60);
    call_time = call_time/60;
    min       = (uint8)(call_time % 60);
    hour      = (uint32)(call_time / 60);
    
    sprintf((char*)time, "%02d:%02d:%02d",hour,min,sec);
}

/*****************************************************************************/
// 	Description : reset the timer
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void ResetCalllogTimer(MN_DUAL_SYS_E dual_sys)
{
//     CC_CALL_TIME_COUNT_T  time = {0};/*lint !e64*/

    MMIAPICL_SetDefaultCallTimeToNVBySim(dual_sys);
}

/*****************************************************************************/
// 	Description : open window CL main menu window
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_OpenMainMenuWindowExt(void)
{
    MMIMAINMENU_TYPE_E  mainmenu_style = MMIAPISET_GetMainmenuStyle();
    
    SCI_ASSERT(mainmenu_style <= MMIMAINMENU_TYPE_MAX); /*assert verified*/

    MMK_CloseMiddleWin(MMICL_START_WIN_ID, MMICL_END_WIN_ID);

#ifdef MMI_CALLLOG_HIDE_MAINMENU
    MMIAPICL_OpenCallsListWindow(MMICL_CALL_MISSED);
#else
    MMK_CreateWin((uint32*)MMICL_ICON_MAIN_MENU_WIN_TAB, PNULL);
#endif
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : get selected cl item number
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSelectedItemNum(uint8* tele_num,  uint8 max_tele_len)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    if(max_tele_len > (PHONE_NUM_MAX_LEN + 2))
    {
        //SCI_TRACE_LOW:"MMIAPICL_GetSelectedItemNum max_tele_len > (PHONE_NUM_MAX_LEN + 2)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9317_112_2_18_2_11_46_93,(uint8*)"");
        return FALSE;
    }
    
    if(PNULL == tele_num)
    {
        //SCI_TRACE_LOW:"MMIAPICL_GetSelectedItemNum PNULL == tele_num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9323_112_2_18_2_11_46_94,(uint8*)"");
        return FALSE;
    }
    
    GetDisplayPhoneNum(tele_num, max_tele_len, &dual_sys);
    MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
    //close the window of call log list menu and detail win
    MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_REFUSED_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : get current selected call type
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMICL_CALL_TYPE_E MMICL_GetCurCallType(void)
{
    return s_cur_sel_log_list_type;
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : to handle sim select window
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  CallTimeSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    if(isReturnOK)
        {
            MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, (ADD_DATA)dual_sys);
        }
        
    return recode;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : display one item of the cl detail
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetHeadAndContentInRText(MMI_TEXT_ID_T head_text_id,      //��������id
                               MMI_CTRL_ID_T ctrl_id,            //���ڵĿؼ�����
                               MMI_STRING_T content_str,         //��ʾ����
                               uint16   *index
                               )
{
    GUILIST_ITEM_T item_t = {0};    /*lint !e64*/ 
    GUILIST_ITEM_DATA_T item_data                       = {0};/*lint !e64*/
    MMI_STRING_T string_prefix = {0};       //���������
//     MMI_STRING_T title_string = {0};        //�������ӡ�����
    //MMI_STRING_T colon_string = {0};
    //wchar colon_str[] = {':', 0};
    uint8 wstr_len = 0;
    wchar dst_string[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};

    if(PNULL == index)
    {
        //SCI_TRACE_LOW:"SetHeadAndContentInRText PNULL == index"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9385_112_2_18_2_11_46_95,(uint8*)"");
        return;
    }
    

    if(0 == content_str.wstr_len)
    {
        return;
    }
    
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;
     
    /*display head*/
    //get content of the head
    MMI_GetLabelTextByLang(head_text_id, &string_prefix);
    wstr_len = MMIAPICOM_CopyStr2Buf(dst_string, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
    
    item_data.item_content[0].item_data_type= GUIITEM_DATA_TEXT_BUFFER;    
    item_data.item_content[0].item_data.text_buffer.wstr_len = wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = dst_string;

    item_data.item_content[1].item_data_type= GUIITEM_DATA_TEXT_BUFFER;    
    item_data.item_content[1].item_data.text_buffer.wstr_len = content_str.wstr_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr =  content_str.wstr_ptr;

    GUILIST_AppendItem(ctrl_id, &item_t);
}
#else
/*****************************************************************************/
// 	Description : display one item of the cl detail
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetHeadAndContentInRText(MMI_TEXT_ID_T head_text_id,      //��������id
                               MMI_CTRL_ID_T ctrl_id,            //���ڵĿؼ�����
                               MMI_STRING_T content_str,         //��ʾ����
                               uint16   *index
                               )
{
    GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
    MMI_STRING_T string_prefix = {0};       //���������
//     MMI_STRING_T title_string = {0};        //�������ӡ�����
    MMI_STRING_T colon_string = {0};
    wchar colon_str[] = {':', 0};
    uint8 wstr_len = 0;
    wchar dst_string[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};

    if(PNULL == index)
    {
        //SCI_TRACE_LOW:"SetHeadAndContentInRText PNULL == index"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9434_112_2_18_2_11_46_96,(uint8*)"");
        return;
    }

    if(0 == content_str.wstr_len)
    {
        return;
    }
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    
    /*display head*/
    //get content of the head
    MMI_GetLabelTextByLang(head_text_id, &string_prefix);
    wstr_len = MMIAPICOM_CopyStr2Buf(dst_string, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.len = wstr_len;

    //add ":"
    colon_string.wstr_ptr = (wchar *)colon_str;
    colon_string.wstr_len = 1;
    wstr_len = MMIAPICOM_CopyStr2Buf(dst_string + item_data.text_data.buf.len, &colon_string, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.len += wstr_len;
    item_data.text_data.buf.str_ptr = dst_string;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, index);

    /*display content*/
    item_data.text_data.buf.len = content_str.wstr_len;
    item_data.text_data.buf.str_ptr = content_str.wstr_ptr;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, index);
}
#endif 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMICL_UpdateCalllogWin(BOOLEAN is_update)
{
    MMI_HANDLE_T    win_handle=0;
    
    win_handle = MMICL_GetUpdatedWinHandle();

    if (win_handle == MMK_GetFocusWinHandle() || win_handle ==MMK_GetFocusWinId())
    {
        MMK_PostMsg(win_handle, MSG_FULL_PAINT, 0,0);
    }
    
}

#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenGprsFlowRateWin(void)
{
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    MMI_WIN_ID_T    query_win_id = MMICL_QUERY_WIN_ID;

    //MS00214101:��������ʧ�ܣ����ȹر���ش���
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMK_CloseWin(MMICL_COM_SELECT_SIM_WIN_ID);
#endif    
    MMK_CloseWin(MMICL_GPRS_FLOWRATE_WIN_ID);
    MMIPUB_CloseQuerytWin(&query_win_id);//MS00225189

    sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_sys, 1);

    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (1 == sim_num)
    {
        MMK_CreateWin((uint32*)MMICL_GPRS_FLOWRATE_WIN_TAB, (ADD_DATA)sim_sys);
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else
    {
    MMICL_SelectSIMFunc(SIMSelectForGprsCallback, MMISET_APPEND_SIM_OK,PNULL);
    }
#endif
}

/*****************************************************************************/
// 	Description : to close the gprs flow rate window
//	Global resource dependence : none
//  Author: fuyan.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_CloseGprsFlowRateWin(void)
{
    MMI_WIN_ID_T    query_win_id = MMICL_QUERY_WIN_ID;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMK_CloseWin(MMICL_COM_SELECT_SIM_WIN_ID);
#endif    
    MMK_CloseWin(MMICL_GPRS_FLOWRATE_WIN_ID);
    MMIPUB_CloseQuerytWin(&query_win_id);//MS00225189
}

/*****************************************************************************/
// 	Description : display gprs flow rate
//	Global resource dependence : 
//  Author:li.li
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGprsFlowrateWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_STRING_T        title_body = {0};
    MMI_STRING_T        title = {0};
    MMISET_SIM_NAME_T   sim_name = {0};	
    wchar               sim_select[MMISET_MAX_SIM_NAME_LEN+1] = {0};
    uint16              name_len = 0;
#endif
    MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_1;  
    MMI_WIN_ID_T    query_win_id = MMICL_QUERY_WIN_ID;

    //SCI_TRACE_LOW:"HandleGprsFlowrateWinMsg dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9557_112_2_18_2_11_47_97,(uint8*)"d", dual_sys);
    dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
            
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_MULTI_SIM_SYS_SINGLE        
        sim_name = MMIAPISET_GetSimName(dual_sys);
        name_len = sim_name.wstr_len;
        MMI_WSTRNCPY(sim_select, name_len, sim_name.wstr_arr, name_len, name_len);

        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CL_GPRS_FLOWRATE, &title_body);

        title.wstr_len = name_len + title_body.wstr_len;

        if (MMIAPICOM_IsASCIIString(sim_name.wstr_arr, sim_name.wstr_len)
            && MMIAPICOM_IsASCIIString(title_body.wstr_ptr, title_body.wstr_len))//����Ascii�룬���Ͽո�
        {
            title.wstr_len += 1;
        }
        
        title.wstr_ptr = (wchar*)(SCI_ALLOCA((title.wstr_len+1)*sizeof(wchar)));
        if (PNULL != title.wstr_ptr)
        {
            SCI_MEMSET(title.wstr_ptr, 0, ((title.wstr_len+1)*sizeof(wchar)));
            MMI_WSTRNCPY(title.wstr_ptr, name_len, sim_select, name_len, name_len);    

            if (MMIAPICOM_IsASCIIString(sim_name.wstr_arr, sim_name.wstr_len)
                && MMIAPICOM_IsASCIIString(title_body.wstr_ptr, title_body.wstr_len))//����Ascii�룬���Ͽո�
            {
                *(title.wstr_ptr + name_len) = 0x0020;
                name_len += 1;
            }

            MMI_WSTRNCPY((title.wstr_ptr + name_len), title_body.wstr_len, title_body.wstr_ptr, title_body.wstr_len, title_body.wstr_len);
            
            GUIWIN_SetTitleText(win_id,title.wstr_ptr,title.wstr_len,FALSE);	
            SCI_FREE(title.wstr_ptr);
        }
        else//alloc fail 
        {
            GUIWIN_SetTitleTextId(win_id,(MMI_TEXT_ID_T)TXT_CL_GPRS_FLOWRATE, FALSE);
        }
        
#else
        GUIWIN_SetTitleTextId(win_id,(MMI_TEXT_ID_T)TXT_CL_GPRS_FLOWRATE, FALSE);
#endif

         //set softkey
        #ifndef   MMI_PDA_SUPPORT      
        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_CL_RESET,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN,FALSE);
        #else       
        GUILIST_SetMaxItem(MMICL_GPRS_FLOWRATE_CTRL_ID, 100, FALSE);
        #endif
        SetGprsFlowrateWindow(MMICL_GPRS_FLOWRATE_CTRL_ID,dual_sys);        
        MMK_SetAtvCtrl(win_id, MMICL_GPRS_FLOWRATE_CTRL_ID);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef   MMI_PDA_SUPPORT      
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
 case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                break;            
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
        
#ifndef MMI_PDA_SUPPORT        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_OK:    
    case MSG_APP_OK:

        MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
        break;
#endif       
    case MSG_PROMPTWIN_OK:
        //�ر�ѯ�ʴ���
        MMIPUB_CloseQuerytWin(&query_win_id);
        MMIAPIPDP_ClearFlowrate(dual_sys);
//        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //�ر�ѯ�ʴ���
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : handle message of sim list window 
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMSelectForGprsCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    if (isReturnOK)
    {
            MMK_CreateWin((uint32*)MMICL_GPRS_FLOWRATE_WIN_TAB, (ADD_DATA)dual_sys);
    }

    return recode;

}
#endif

/*****************************************************************************/
// 	Description : the gprs flowrate size
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
BOOLEAN FlowrateFormSize(
                        MMI_STRING_T    *str_dst,        //out.
                        uint32            size
                        )
{
    uint8 display_str[MMICL_DSP_STR_LEN] = {0};
    uint16 display_str_len = 0;
    
    if (((float)size/MMICL_ONE_K_LEN) < 0.100000 && ((float)size/MMICL_ONE_K_LEN) > 0.000000)
    {
        sprintf((char *)display_str, "%.1fk", 0.1);
    }
    else
    {
        sprintf((char *)display_str, "%.1fK", (float)size/MMICL_ONE_K_LEN);
    }

    display_str_len = (uint16)strlen((char*)display_str);

    str_dst->wstr_len = display_str_len;

    str_dst->wstr_ptr = (wchar*)SCI_ALLOCA((str_dst->wstr_len + 1) * sizeof(wchar));
    SCI_MEMSET(str_dst->wstr_ptr, 0x00, ((str_dst->wstr_len + 1) * sizeof(wchar)));

    MMI_STRNTOWSTR(str_dst->wstr_ptr,
                                str_dst->wstr_len,
                                (uint8 *)display_str,
                                display_str_len,
                                display_str_len);
    
    return TRUE;
}


#ifdef MMI_PDA_SUPPORT
LOCAL void SetGprsFlowrateWindow(MMI_CTRL_ID_T ctrl_id,MN_DUAL_SYS_E  dual_sys)
{
    uint16   index_ptr = 0;
    MMIPDP_FLOWRATE_T   gprs_flowrate = {0};
    MMI_STRING_T       last_sent = {0};
    MMI_STRING_T       last_recv = {0};
    MMI_STRING_T       total_send = {0};
    MMI_STRING_T       total_recv = {0};
    MMIAPIPDP_GetFlowrate(&gprs_flowrate,dual_sys);
    FlowrateFormSize(&last_sent,gprs_flowrate.last_send);
    SetHeadAndContentInRText(TXT_BROWSER_LAST_SENT, ctrl_id, last_sent, &index_ptr);
    SCI_FREE(last_sent.wstr_ptr);

    FlowrateFormSize(&last_recv,gprs_flowrate.last_recv);
    SetHeadAndContentInRText(TXT_BROWSER_LAST_RECV, ctrl_id, last_recv, &index_ptr);
    SCI_FREE(last_recv.wstr_ptr);

    FlowrateFormSize(&total_send,gprs_flowrate.total_send);
    SetHeadAndContentInRText(TXT_BROWSER_TOTAL_SENT, ctrl_id, total_send, &index_ptr);
    SCI_FREE(total_send.wstr_ptr);

    FlowrateFormSize(&total_recv,gprs_flowrate.total_recv);
    SetHeadAndContentInRText(TXT_BROWSER_TOTAL_RECV, ctrl_id, total_recv, &index_ptr);
    SCI_FREE(total_recv.wstr_ptr);
}
#else
/*****************************************************************************/
//  Description : set the gprs flowrate window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void SetGprsFlowrateWindow(MMI_CTRL_ID_T ctrl_id,MN_DUAL_SYS_E  dual_sys)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint16              index = 0;
    MMI_STRING_T        dst_temp1 = {0};
    MMIPDP_FLOWRATE_T   gprs_flowrate = {0};

    MMIAPIPDP_GetFlowrate(&gprs_flowrate,dual_sys);

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));

   
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_BROWSER_LAST_SENT;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    SCI_MEMSET(&dst_temp1, 0, sizeof(MMI_STRING_T));
    FlowrateFormSize(&dst_temp1,gprs_flowrate.last_send);
    item_data.text_data.buf.len = MIN(dst_temp1.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dst_temp1.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_FREE(dst_temp1.wstr_ptr);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_BROWSER_LAST_RECV;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    SCI_MEMSET(&dst_temp1, 0, sizeof(MMI_STRING_T));
    FlowrateFormSize(&dst_temp1,gprs_flowrate.last_recv);
    item_data.text_data.buf.len = MIN(dst_temp1.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dst_temp1.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_FREE(dst_temp1.wstr_ptr);

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_BROWSER_TOTAL_SENT;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    SCI_MEMSET(&dst_temp1, 0, sizeof(MMI_STRING_T));
    FlowrateFormSize(&dst_temp1,gprs_flowrate.total_send);
    item_data.text_data.buf.len = MIN(dst_temp1.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dst_temp1.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_FREE(dst_temp1.wstr_ptr);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_BROWSER_TOTAL_RECV;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    SCI_MEMSET(&dst_temp1, 0, sizeof(MMI_STRING_T));
    FlowrateFormSize(&dst_temp1,gprs_flowrate.total_recv);
    item_data.text_data.buf.len = MIN(dst_temp1.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dst_temp1.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_FREE(dst_temp1.wstr_ptr);
}
#endif
#endif

/*****************************************************************************/
//  Description : phonebook callback
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:  
/*****************************************************************************/
PUBLIC void MMICL_HandlePBEventFunc(MMIEVENT_PB_EVENT_E event)
{
    if((MMIEVENT_PB_ADD == event)
       || (MMIEVENT_PB_DELETE == event)
       || (MMIEVENT_PB_UPDATE == event))
    {
        if(MMK_IsOpenWin(MMICL_LOG_LIST_DETAIIL_WIN_ID)
#ifdef CALL_TIMES_SUPPORT
           || MMK_IsOpenWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID)
#endif
           )
        {
            MMICL_UpdateCalllogWin(TRUE);
        }
    }
}


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : Istyle_MMITHEME_GetWinClientBothRect
//	Global resource dependence : 
//    Author:chen yao for istyle 20110720
//	Note: ����istyle����TAB�ƶ�����Ļ�·����ͻ��������䶯
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T Istyle_MMITHEME_GetWinClientBothRect(
                                                     MMI_HANDLE_T win_handle
                                                     )
{
	GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_handle);

	client_rect.v_rect.top -= MMI_ISTYLE_TITLE_HEIGHT;
	client_rect.h_rect.top -= MMI_ISTYLE_TITLE_HEIGHT; 

	return client_rect;
}

#if 0

/*****************************************************************************/
//  Description : istyle����У���button��"title"����ʵ��softkey
//  Global resource dependence : none
//  Author :chen yao for istyle 20110720
//  Note: 
/*****************************************************************************/
PUBLIC void Istyle_CreateDialWinTitle(MMI_WIN_ID_T      win_id,MMI_CTRL_ID_T ctrl_id)
{
	GUI_SOFTKEY_T *ctrl_ptr = 0;
	GUI_BOTH_RECT_T softkey_both_rect = Istyle_MMITHEME_GetWinClientBothRect(win_id);
	GUI_BG_T button_bg = {0};
	GUI_BG_T middle_button_bg = {0};
	//GUI_BG_T button_fg = {0};
	//wchar   add_text[] = L" ";
	//uint16 len = MMIAPICOM_Wstrlen(add_text);	
	GUI_FONT_ALL_T     font={CAF_FONT_14,MMI_WHITE_COLOR};
    MMI_HANDLE_T button_ctrl_handle[GUISOFTKEY_BUTTON_NUM] = {0};

	MMK_DestroyDynaCtrl(ctrl_id);
	
	//����"title"
	GUISOFTKEY_CreateCtrlDyn(win_id, ctrl_id, TXT_CL_CALLS_ALL, TXT_CL_CALLS_ALL, TXT_CL_CALLS_ALL);

	ctrl_ptr = (GUI_SOFTKEY_T *)MMK_GetCtrlPtr(MMK_GetCtrlHandleByWin(win_id,ctrl_id));

	//����softkeyλ��
	softkey_both_rect.v_rect.bottom = softkey_both_rect.v_rect.top + MMI_ISTYLE_TITLE_HEIGHT;
	softkey_both_rect.h_rect.bottom = softkey_both_rect.h_rect.top + MMI_ISTYLE_TITLE_HEIGHT;
	GUICTRL_SetBothRect((IGUICTRL_T * )ctrl_ptr,  &softkey_both_rect);

	//����softkey��������draw
	ctrl_ptr->bg_type = GUI_SOFTKEY_BG_IMAGE_ONLY;
	ctrl_ptr->bg_image = IMAGE_ISTYLE_TITLE_BAR;
	ctrl_ptr->rect = softkey_both_rect.v_rect;
	//GUISOFTKEY_Update(win_id,ctrl_id);

	//�м��button��ʵ�ǵ�TITLE�ã�����ҪBG
	middle_button_bg.bg_type = GUI_BG_NONE;
	
	button_bg.bg_type = GUI_BG_IMG;
	button_bg.img_id = IMAGE_ISTYLE_TITLE_MENU;
	
	{
		//button layout info
		GUI_BOTH_RECT_T button0_both_rect_ptr ={{MMI_ISTYLE_TITLE_LEFT_BUTTON_X, 0, MMI_ISTYLE_TITLE_LEFT_BUTTON_X+MMI_ISTYLE_TITLE_LEFT_BUTTON_WIDTH-1, 0},
										{MMI_ISTYLE_TITLE_LEFT_BUTTON_X, 0, MMI_ISTYLE_TITLE_LEFT_BUTTON_X+MMI_ISTYLE_TITLE_LEFT_BUTTON_WIDTH-1, 0}};
		GUI_BOTH_RECT_T button1_both_rect_ptr ={{MMI_ISTYLE_TITLE_MIDDLE_BUTTON_X, 0, MMI_ISTYLE_TITLE_MIDDLE_BUTTON_X+MMI_ISTYLE_TITLE_MIDDLE_BUTTON_WIDTH-1, 0},
										{MMI_ISTYLE_TITLE_MIDDLE_BUTTON_X, 0, MMI_ISTYLE_TITLE_MIDDLE_BUTTON_X+MMI_ISTYLE_TITLE_MIDDLE_BUTTON_WIDTH-1, 0}};
		GUI_BOTH_RECT_T button2_both_rect_ptr ={{MMI_ISTYLE_TITLE_RIGHT_BUTTON_X, 0, MMI_ISTYLE_TITLE_RIGHT_BUTTON_X+MMI_ISTYLE_TITLE_RIGHT_BUTTON_WIDTH-1, 0},
										{MMI_ISTYLE_TITLE_RIGHT_BUTTON_X, 0, MMI_ISTYLE_TITLE_RIGHT_BUTTON_X+MMI_ISTYLE_TITLE_RIGHT_BUTTON_WIDTH-1, 0}};

		button0_both_rect_ptr.v_rect.top = softkey_both_rect.v_rect.top+((MMI_ISTYLE_TITLE_HEIGHT-MMI_ISTYLE_TITLE_BUTTON_HEIGHT)>>1);
		button0_both_rect_ptr.v_rect.bottom =button0_both_rect_ptr.v_rect.top+MMI_ISTYLE_TITLE_BUTTON_HEIGHT-1;
		button1_both_rect_ptr.v_rect.top = button0_both_rect_ptr.v_rect.top;
		button1_both_rect_ptr.v_rect.bottom =button0_both_rect_ptr.v_rect.bottom;
		button2_both_rect_ptr.v_rect.top = button0_both_rect_ptr.v_rect.top;
		button2_both_rect_ptr.v_rect.bottom =button0_both_rect_ptr.v_rect.bottom;
		
		button0_both_rect_ptr.h_rect.top = button0_both_rect_ptr.v_rect.top;
		button0_both_rect_ptr.h_rect.bottom =button0_both_rect_ptr.v_rect.bottom;
		button1_both_rect_ptr.h_rect.top = button0_both_rect_ptr.v_rect.top;
		button1_both_rect_ptr.h_rect.bottom =button0_both_rect_ptr.v_rect.bottom;
		button2_both_rect_ptr.h_rect.top = button0_both_rect_ptr.v_rect.top;
		button2_both_rect_ptr.h_rect.bottom =button0_both_rect_ptr.v_rect.bottom;

        button_ctrl_handle[0] = GUIBUTTONSOFTKEY_GetButtonHandle(win_id, 0);
        button_ctrl_handle[1] = GUIBUTTONSOFTKEY_GetButtonHandle(win_id, 1);
        button_ctrl_handle[2] = GUIBUTTONSOFTKEY_GetButtonHandle(win_id, 2);

		GUIBUTTON_SetFont(button_ctrl_handle[0], &font);
		GUIBUTTON_SetFont(button_ctrl_handle[1], &font);
		GUIBUTTON_SetFont(button_ctrl_handle[2], &font);
		
		//����softkey��3��button
		if(MMICL_ALL_CHILD_WIN_ID == win_id)
		{
			GUI_BG_T all_button_bg = {GUI_BG_IMG, 0, 0, 0, FALSE};
			GUI_BG_T misscall_button_bg = {GUI_BG_IMG, 0, 0, 0, FALSE};

			GUI_BG_T misscall_button_fg = {GUI_BG_IMG, 0, 0, 0, FALSE};
			GUI_BG_T delete_button_fg = {GUI_BG_IMG, 0, 0, 0, FALSE};
			
			//ȫ��ͨ��
			button0_both_rect_ptr.v_rect.left = MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X;
			button0_both_rect_ptr.v_rect.right= MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;
			button0_both_rect_ptr.h_rect.left = MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X;
			button0_both_rect_ptr.h_rect.right= MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;
			all_button_bg.img_id = IMAGE_ISTYLE_ALLCALL_OFF;
			
			GUIBUTTON_SetTextId(button_ctrl_handle[0], TXT_CL_CALLS_ALL);
			GUIBUTTON_SetRect(button_ctrl_handle[0], &button0_both_rect_ptr.v_rect);
			GUICTRL_SetBothRect(MMK_GetCtrlPtr(button_ctrl_handle[0]), &button0_both_rect_ptr);
			GUIBUTTON_SetBg(button_ctrl_handle[0], &all_button_bg);
			GUIBUTTON_SetPressedBg(button_ctrl_handle[0], &all_button_bg);
			
			//δ������
			button1_both_rect_ptr.v_rect.left = MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X;
			button1_both_rect_ptr.v_rect.right= MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;
			button1_both_rect_ptr.h_rect.left = MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X;
			button1_both_rect_ptr.h_rect.right= MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;
			misscall_button_bg.img_id = IMAGE_ISTYLE_MISSCALL_ON;
			misscall_button_fg.img_id = IMAGE_ISTYLE_MISSCALL_OFF;
			GUIBUTTON_SetTextId(button_ctrl_handle[1], TXT_CC_MISS_CALL);
			GUIBUTTON_SetRect(button_ctrl_handle[1], &button1_both_rect_ptr.v_rect);
			GUICTRL_SetBothRect(MMK_GetCtrlPtr(button_ctrl_handle[1]), &button1_both_rect_ptr);
			GUIBUTTON_SetBg(button_ctrl_handle[1], &misscall_button_bg);
			GUIBUTTON_SetPressedBg(button_ctrl_handle[1], &misscall_button_fg);
			
			//���
			button2_both_rect_ptr.v_rect.left = MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X;
			button2_both_rect_ptr.v_rect.right= MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X+MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_WIDTH-1;
			button2_both_rect_ptr.h_rect.left = MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X;
			button2_both_rect_ptr.h_rect.right= MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X+MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_WIDTH-1;
			
			delete_button_fg.img_id = IMAGE_ISTYLE_TITLE_MENU_DOWN;
			GUIBUTTON_SetTextId(button_ctrl_handle[2], TXT_DELETE);
			GUIBUTTON_SetRect(button_ctrl_handle[2], &button2_both_rect_ptr.v_rect);
			GUICTRL_SetBothRect(MMK_GetCtrlPtr(button_ctrl_handle[2]), &button2_both_rect_ptr);
			GUIBUTTON_SetBg(button_ctrl_handle[2], &button_bg);
			GUIBUTTON_SetPressedBg(button_ctrl_handle[2], &delete_button_fg);
		}
	}
}

/*****************************************************************************/
// 	Description : set call log title button background
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void Istyle_SetButtonBG(MMI_CTRL_ID_T ctrl_id, uint32 button_index)
{
    GUI_BG_T button_bg_left = {GUI_BG_IMG, 0, 0, 0, FALSE};
	GUI_BG_T button_bg_mid = {GUI_BG_IMG, 0, 0, 0,FALSE};
    //MMI_TEXT_ID_T text_id = 0;
    GUI_SOFTKEY_T *ctrl_ptr = (GUI_SOFTKEY_T *)MMK_GetCtrlPtr(ctrl_id);
    MMI_HANDLE_T button_ctrl_handle[GUISOFTKEY_BUTTON_NUM] = {0};
	GUI_BOTH_RECT_T softkey_both_rect = Istyle_MMITHEME_GetWinClientBothRect(MMICL_ALL_CHILD_WIN_ID);
	
    GUI_RECT_T button_rect = {0, 0, 0, 0};
	
	button_rect.top = softkey_both_rect.v_rect.top+((MMI_ISTYLE_TITLE_HEIGHT - MMI_ISTYLE_TITLE_BUTTON_HEIGHT)>>1);
	button_rect.bottom = button_rect.top + MMI_ISTYLE_TITLE_BUTTON_HEIGHT - 1;

    switch (button_index)
    {
    case 0:
        //ȫ��ͨ��
        button_rect.left = MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X;
        button_rect.right= MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;

		button_bg_left.img_id = IMAGE_ISTYLE_ALLCALL_OFF;
		button_bg_mid.img_id = IMAGE_ISTYLE_MISSCALL_ON;
        break;
        
    case 1:
        //δ������
        button_rect.left = MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X;
        button_rect.right= MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;

		button_bg_left.img_id = IMAGE_ISTYLE_ALLCALL_ON;
		button_bg_mid.img_id = IMAGE_ISTYLE_MISSCALL_OFF;
        break;
        
    case 2:
        //���
        button_rect.left = MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X;
        button_rect.right= MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X + MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_WIDTH-1;
        break;

    default:
        return;
    }
	/*����button��������ʾѡ�л���δѡ*/
    if (PNULL != ctrl_ptr)
    {
        button_ctrl_handle[0] = GUIBUTTONSOFTKEY_GetButtonHandle(ctrl_ptr->win_handle, 0);
        button_ctrl_handle[1] = GUIBUTTONSOFTKEY_GetButtonHandle(ctrl_ptr->win_handle, 1);
        button_ctrl_handle[2] = GUIBUTTONSOFTKEY_GetButtonHandle(ctrl_ptr->win_handle, 2);
    }
    GUIBUTTON_SetBg(button_ctrl_handle[0], &button_bg_left);
	GUIBUTTON_SetBg(button_ctrl_handle[1], &button_bg_mid);
}
#endif
#if defined MMI_ISTYLE_SUPPORT
/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  Istyle_HandleAllChildWindow(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T      list_handle = 0;
    MMI_CTRL_ID_T list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;
    static BOOLEAN  s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_WIN_ID_T real_win_id = MMK_GetWinId(win_id); 
	MMI_WIN_ID_T info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);

    list_handle = MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id);

    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {        
            SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"Istyle_HandleAllChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10080_112_2_18_2_11_48_98,(uint8*)"");
                MMK_CloseParentWin(info_ptr);
                return recode;
            }

            //Istyle_CreateDialWinTitle(win_id, MMICALLLOG_SOFTKEY_CTRL_ID);

            GUILIST_SetTextFont(CalllogPBData_GetListHandle(), SONG_FONT_14, MMI_BLACK_COLOR);

            s_cur_sel_log_list_type = MMICL_CALL_ALL;

            MMK_SetAtvCtrl(win_id, CalllogPBData_GetListHandle());
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            //Istyle_MoveChildWinAboveTab(MMK_ConvertIdToHandle(win_id));
            MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
            s_cur_sel_log_list_type = MMICL_CALL_ALL;
            SetLogListItem();
        }
        break;

	 /*add for istyle*/
    case MSG_CTL_LIST_CHECK_CONTENT:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle())
        {
            if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    CL_OpenCallTimeListOrDetailWin(arraycall_info);
                }
            }
        }
#endif		
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (MMICL_ALL_SELECT_CHILD_WIN_ID == real_win_id)
        {
            break;
        }
        else
        {
            if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)    
            {
                HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
            }
		}
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_GET_FOCUS:
        if (MMICL_ALL_SELECT_CHILD_WIN_ID == real_win_id)
        {
            GUIWIN_SetTitleIsVisible(win_id, TRUE, FALSE);
    		//cancel and ok
    		//ȡ��sk
    		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
    		GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_CANCEL, FALSE);
    		//���sk
    		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
    		GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_FINISH, FALSE);
        }
        else
        {
    		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
        	GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, FALSE);
            GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_LEFT, IMAGE_ISTYLE_TITLE_RETURN, IMAGE_ISTYLE_TITLE_RETURN_DOWN, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_LEFT, MSG_APP_CANCEL, 0);

            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        	GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_OPTION, FALSE);
            
            GUIWIN_SetTitleIsVisible(win_id, TRUE, TRUE);
        }
        break;

    case MSG_CTL_TAB_SWITCH:
        //��˽���
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //��Ҫ������˽���
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
 
     case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

/*****************************************************************************/
// 	Description : to handle select call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  Istyle_HandleSelectAllChildWindow(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T      list_handle = 0;
    MMI_CTRL_ID_T list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;
    static BOOLEAN  s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
	MMI_WIN_ID_T info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);

    list_handle = MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id);

    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {        
            SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"Istyle_HandleAllChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10080_112_2_18_2_11_48_98,(uint8*)"");
                MMK_CloseParentWin(info_ptr);
                return recode;
            }

            //Istyle_CreateDialWinTitle(win_id, MMICALLLOG_SOFTKEY_CTRL_ID);

            GUILIST_SetTextFont(CalllogPBData_GetListHandle(), SONG_FONT_14, MMI_BLACK_COLOR);

            s_cur_sel_log_list_type = MMICL_CALL_ALL;

            MMK_SetAtvCtrl(win_id, CalllogPBData_GetListHandle());
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            //Istyle_MoveChildWinAboveTab(MMK_ConvertIdToHandle(win_id));
            MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
            s_cur_sel_log_list_type = MMICL_CALL_ALL;
            SetLogListItem();
        }
        break;

	 /*add for istyle*/
    case MSG_CTL_LIST_CHECK_CONTENT:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle())
        {
            if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    CL_OpenCallTimeListOrDetailWin(arraycall_info);
                }
            }
        }
#endif		
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_GET_FOCUS:
            GUIWIN_SetTitleIsVisible(win_id, TRUE, FALSE);
    		//cancel and ok
    		//ȡ��sk
    		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
    		GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_CANCEL, FALSE);
    		//���sk
    		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
    		GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_FINISH, FALSE);
        break;

    case MSG_CTL_TAB_SWITCH:
        //��˽���
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //��Ҫ������˽���
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
 
     case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  Istyle_HandlePBDetailCalllogChildWindow(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
//    MMI_CTRL_ID_T	list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;
    static BOOLEAN  s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
//    MMI_HANDLE_T    list_handle = MMK_GetCtrlHandleByWin(win_id,MMICL_ALL_LIST_CTRL_ID);

    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {        
            SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"Istyle_HandlePBDetailCalllogChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10215_112_2_18_2_11_49_99,(uint8*)"");
                return recode;
            }

            //Istyle_CreateDialWinTitle(win_id, MMICALLLOG_SOFTKEY_CTRL_ID);

            GUILIST_SetTextFont(CalllogPBData_GetListHandle(), SONG_FONT_14, MMI_BLACK_COLOR);

            s_cur_sel_log_list_type = MMICL_CALL_ALL;

            MMK_SetAtvCtrl(win_id, CalllogPBData_GetListHandle());
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            
            //Istyle_MoveChildWinAboveTab(MMK_ConvertIdToHandle(win_id));
            MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
            s_cur_sel_log_list_type = MMICL_CALL_ALL;
            SetLogListItem();
        }
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
    	GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_OPTION, FALSE);
        }
        else
        {
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
    	GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_NULL, FALSE);
        }
        GUIWIN_SetTitleIsVisible(win_id, TRUE, TRUE);
        
        break;

	 /*add for istyle*/
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle())
        {
            if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    CL_OpenCallTimeListOrDetailWin(arraycall_info);
                }
            }
        }
#endif		
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
      
       if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)    
        {
            HandleRingBackByType(CC_CALL_DEFAULT_CALL);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_GET_FOCUS:

		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
    	GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, FALSE);

        break;

    case MSG_CTL_TAB_SWITCH:
        //��˽���
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //��Ҫ������˽���
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
 
     case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}



#endif
#ifdef CALL_TIMES_SUPPORT
/*****************************************************************************/
// 	Description : load call times list into listbox
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void AppendCallTimeListItem(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info,
                             uint16     selected_call_log_index
                             )
{
    GUILIST_ITEM_T item_t                          = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data                       = {0};/*lint !e64*/

    MMI_STRING_T name_number_str = {0};
    wchar name_number_wchar[PHONE_NUM_MAX_LEN + MAX_CALL_TIME_LEN+1]  ={0};     

#ifndef MMI_MULTI_SIM_SYS_SINGLE    
    MMI_STRING_T sim_name_str  = {0};
    wchar sim_name_wstr[GUILIST_STRING_MAX_NUM + 1]={0};
#endif

    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_STRING_T time_str       ={0};
    wchar wstr_time_str[GUILIST_STRING_MAX_NUM + 1] = {0};
#ifndef MMI_CL_MINI_SUPPORT    
    MMI_IMAGE_ID_T	image_id = IMAGE_NULL;
#endif


#ifdef TOUCH_PANEL_SUPPORT
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;
#else
#ifndef MMI_MULTI_SIM_SYS_SINGLE
//add by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT; //GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;
#else
    item_t.item_style = GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;
#endif
#else
#ifdef MMI_CL_MINI_SUPPORT
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT;
#else
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT;
#endif

#endif
#endif
    
    item_t.item_data_ptr = &item_data;
    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT;

    //set softkey
    if ( s_select_return_info.need_select_return )
    {
        item_data.softkey_id[0] = TXT_COMMON_OK;
    }
    else
    {
//add by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_DELETE;
#else
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_OPTION;
#endif
    }
    
    item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        
    //set list item
    if ( arraycall_info->call_bcdinfo[selected_call_log_index].call_times < 1
    #ifdef MMI_PDA_SUPPORT
        || ((call_type != MMICL_CALL_ALL)&& (call_type != MMICL_CALL_MATCHED) && (0 == GetCurrTypeNum( call_type,arraycall_info)) )
    #endif
        )  
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        
        //set softkey
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_BLANK;
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
    else
    {
        uint16 i = 0;
        MMICL_CALL_BCDINFO_T     *callinfo=PNULL;

        for ( i = arraycall_info->call_bcdinfo[selected_call_log_index].call_times; i > 0 ; i -- )
        {
            callinfo = &arraycall_info->call_bcdinfo[selected_call_log_index];

            //Init Text Contents     Name/Number, SimName, Date, Time
            SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));            
            name_number_str.wstr_ptr = name_number_wchar;
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
            SCI_MEMSET(sim_name_wstr,0,sizeof(sim_name_wstr));
            sim_name_str.wstr_ptr = sim_name_wstr;
            sim_name_str.wstr_len = 0;
#endif
            SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_wstr));            
            datetime_str.wstr_ptr = datetime_wstr;
            SCI_MEMSET(&time_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&wstr_time_str,0,sizeof(wstr_time_str));            
            time_str.wstr_ptr = wstr_time_str;
//fix by bingjie.chen
#ifndef MMI_CL_MINI_SUPPORT
            //Add Image content
            image_id = GetCallTypeImageID(callinfo);
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data.image_id = image_id;

#ifndef MMI_MULTI_SIM_SYS_SINGLE    
            if (!callinfo->is_emergency_call)
            {
                MMIAPICL_GetSimNameContent(sim_name_wstr,GUILIST_STRING_MAX_NUM,callinfo->dual_sys);
            }
            sim_name_str.wstr_len = MMIAPICOM_Wstrlen(sim_name_wstr);
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].font_color_id=MMIAPISET_GetSimColorID(callinfo->dual_sys);
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].is_default =TRUE;
#endif                 
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_len =sim_name_str.wstr_len;
#else
            //Single card don't need sim name
#endif

            //Add Contact name/or call number content no Calltimes, use numertype
            if (callinfo->is_emergency_call)
            {
                uint8   tele_num[PHONE_NUM_MAX_LEN + 2] = {0};

                //get number
                MMICL_GenDispNumber(callinfo->call_type,
                    callinfo->number_type,
                    callinfo->number_len,
                    callinfo->number, 
                    tele_num,
                    PHONE_NUM_MAX_LEN + 2);
                name_number_str.wstr_len = (uint16)strlen((char*)tele_num);
                name_number_str.wstr_ptr = name_number_wchar;//(uint8 *)tele_num;
                MMI_STRNTOWSTR( name_number_wchar, PHONE_NUM_MAX_LEN, (uint8 *)tele_num, name_number_str.wstr_len, name_number_str.wstr_len );            
				name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
            }
            else
            {
            	if (GetNameOrNumberContent(name_number_wchar,PHONE_NUM_MAX_LEN+MAX_CALL_TIME_LEN,callinfo,FALSE,TRUE))
            	{	
					name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
            	    item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            	    item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
            	    item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
            	}  
            }
#endif       
	     if(callinfo->call_times>0)
            {
                //Add date and time content
                if (0<callinfo->call_start_time[callinfo->call_times -1])
                {
//add by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
                 
                    GetDateContent(&datetime_str, GUILIST_STRING_MAX_NUM,callinfo,i-1,TRUE);
                    item_data.item_content[0].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[0].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                    item_data.item_content[0].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
                    
                    GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,i-1);
                    item_data.item_content[1].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[1].item_data.text_buffer.wstr_len = time_str.wstr_len;
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
#else
                    GetDateContent(&datetime_str, GUILIST_STRING_MAX_NUM,callinfo,i-1,FALSE);
                    item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                    item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
                    
                    GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,i-1);
                    item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                    item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
#endif
                }
            }
            //item_t.user_data = (uint32)j++;//remember item index of 
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
            if (MMITHEME_IsIstyle())        //IPHONE set time and to blue color
            {
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].font_color_id=MMITHEME_COLOR_BLUE;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].is_default =TRUE;
            }
#endif    
            
            GUILIST_AppendItem( ctrl_id, &item_t );
        }
    }
}

/*****************************************************************************/
// 	Description : to set call time list item
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimeListItem(MMI_CTRL_ID_T call_times_list_ctrl_id)
{
    uint16 sel_item_index = 0;
	uint16 current_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMICL_CALL_TYPE_E call_type = s_cur_sel_log_list_type;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10476_112_2_18_2_11_50_100,(uint8*)"");
        return;
    }
    
	current_index = GUILIST_GetCurItemIndex(call_times_list_ctrl_id);

    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    GetCurrentRecordInfo(arraycall_info, &sel_item_index);
        
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        call_type = s_match_call_info_ptr->call_info[s_mmicl_match_item_index].call_type;
    }

    //load log list
    GUILIST_SetMaxItem(call_times_list_ctrl_id, MMICL_MAX_CALL_TIMES, FALSE );//max item MMICL_MAX_CALL_TIMES
    GUILIST_RemoveAllItems(call_times_list_ctrl_id);
#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle() && MMICL_CALL_ALL == s_cur_sel_log_list_type)
    {
    //	Istyle_AppendLogListItem(call_times_list_ctrl_id, s_cur_sel_log_list_type, arraycall_info,sel_item_index);
    }
    //else
#endif    
    {
    	AppendCallTimeListItem(call_times_list_ctrl_id, call_type, arraycall_info,sel_item_index);
    }
#else
    AppendCallTimeListItem(call_times_list_ctrl_id, call_type, arraycall_info,sel_item_index);
#endif
	
    GUILIST_SetCurItemIndex(call_times_list_ctrl_id, current_index);//set at first index
    
    // (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
    }
}

/*****************************************************************************/
// 	Description : set call time list title window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void  SetCallTimeListTitle(MMI_WIN_ID_T win_id)
{
    uint16   sel_item_index =0;
    MMICL_CALL_ARRAY_BCDINFO_T * arraycall_info =PNULL;
    wchar title_string_ptr[PHONE_NUM_MAX_LEN +1] ={0};
    uint16 title_string_len =0;
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10526_112_2_18_2_11_50_101,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    GetCurrentRecordInfo(arraycall_info, &sel_item_index);
        
    /*����Ϊ��*/
    if ( !arraycall_info->call_bcdinfo[sel_item_index].is_num_present )  //show number
    {
        GUIWIN_SetTitleTextId(win_id,TXT_UNKNOW_NUM, FALSE);
    }
    else if(arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        GUIWIN_SetTitleTextId(win_id,TXT_KL_PSW_EMERGENCY_CALL, FALSE);//��������
    }
    else    //from PB first,then show num
    {
        uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
        MMI_STRING_T         name_str ={0};
        uint16  tele_num_len =0;
        BOOLEAN find_result =FALSE;
        wchar str_ptr[MMIPB_MAX_STRING_LEN + 1]     = {0};
        
        SCI_MEMSET(tele_num, 0x00, sizeof(tele_num));

        //get number
        tele_num_len = MMICL_GenDispNumber(arraycall_info->call_bcdinfo[sel_item_index].call_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_len,
            arraycall_info->call_bcdinfo[sel_item_index].number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);
 
        name_str.wstr_ptr = str_ptr;
        find_result = MMIAPICL_GetNameByClInfo(arraycall_info->call_bcdinfo[sel_item_index], &name_str, FALSE);

        //no name in pb
        if ( !find_result || 0 == name_str.wstr_len )
        {
            MMI_STRNTOWSTR( title_string_ptr,
                    PHONE_NUM_MAX_LEN,
                    (uint8 *)tele_num,
                    PHONE_NUM_MAX_LEN,
                    tele_num_len);
            title_string_len = tele_num_len;
        }
        else//there is name
        {
            title_string_len = name_str.wstr_len;
            MMI_WSTRNCPY (title_string_ptr, 
                PHONE_NUM_MAX_LEN, 
                name_str.wstr_ptr, 
                PHONE_NUM_MAX_LEN, 
                title_string_len);
        }
        GUIWIN_SetTitleText(win_id,title_string_ptr,title_string_len,FALSE);
    }
    
    SCI_FREE(arraycall_info);
}


#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//	Description : the handle function of call time list delete window
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleCallTimeListDeleteSelectedQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
//        DeleteAllCallLog(s_cur_sel_log_list_type);
        DeleteCurrentRecord();
        MMK_CloseWin(win_id);
        MMK_CloseWin(MMICL_CALL_TIMES_WIN_ID);  //close 
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}
#endif
#ifdef TOUCH_PANEL_SUPPORT

//layout info,will move to mmiposition.h later
#ifdef MAINLCD_SIZE_320X480
#define CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT      62
#define CALLTIME_LIST_CONTACT_INFO_BAR_ICON_HEIGHT 52
#define CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN 5
#define CALLTIME_LIST_CONTACT_INFO_BAR_LABEL_WID (MMI_MAINSCREEN_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT-MMI_LIST_ITEM_SELECT_ICON_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN*2)
#define CALLTIME_LIST_BUTTON_FORM_HEIGHT     52
#define CALLTIME_LIST_BUTTON_HEIGHT          40   
#elif (defined MAINLCD_SIZE_240X320) || (defined MAINLCD_SIZE_240X400)
#define CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT     50
#define CALLTIME_LIST_CONTACT_INFO_BAR_ICON_HEIGHT 43
#define CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN 3
#define CALLTIME_LIST_BUTTON_FORM_HEIGHT     45
#define CALLTIME_LIST_BUTTON_HEIGHT          40   
#define CALLTIME_LIST_CONTACT_INFO_BAR_LABEL_WID (MMI_MAINSCREEN_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT-MMI_LIST_ITEM_SELECT_ICON_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN*2)
#else
#define CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT     50
#define CALLTIME_LIST_CONTACT_INFO_BAR_ICON_HEIGHT 43
#define CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN 3
#define CALLTIME_LIST_BUTTON_FORM_HEIGHT     45
#define CALLTIME_LIST_BUTTON_HEIGHT          40   
#define CALLTIME_LIST_CONTACT_INFO_BAR_LABEL_WID (MMI_MAINSCREEN_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT-MMI_LIST_ITEM_SELECT_ICON_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN*2)
#endif

#define CALLTIME_LIST_BUTTON_FORM_BG_COLOR  GUI_RGB2RGB565(234,234,234)

/*****************************************************************************/
// 	Description : init title bar of call time list window
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimeListWinTitleBar(MMI_WIN_ID_T win_id)
{
    GUIANIM_CTRL_INFO_T                 ctrl_info = {0};
    GUIANIM_DATA_INFO_T                 data_info = {0};
    GUIANIM_DISPLAY_INFO_T              display_info = {0};
    GUIFORM_CHILD_WIDTH_T               sbs_width = {0};
    GUIFORM_CHILD_HEIGHT_T              sbs_height = {0};
    GUI_BG_T                            bg_info = {0};
//    uint16                              image_width = 0;
//    uint16                              image_height = 0;
    MN_DUAL_SYS_E                       dual_sys = 0;
    MMICL_CALL_ARRAY_BCDINFO_T          *arraycall_info = PNULL;
    uint16                              sel_item_index = 0;
    uint16                              hor_space = 10;
    uint16                              ver_space = 0;

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10678_112_2_18_2_11_50_102,(uint8*)"");
        return;
    }
//    if (g_pbcalllog_data == PNULL)
//    {
//        return;
//    }
//    if (g_pbcalllog_data->matched_contact==PNULL)
    {
	//        return;
    }
    GUIFORM_IsSlide(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildBg(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, FALSE);
    GUILIST_PermitBorder(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, FALSE);
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_CL_DETAIL_TITLE_BG;
    GUIFORM_SetBg(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, &bg_info);
    
    sbs_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    sbs_height.add_data = CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT;
	GUIFORM_SetChildHeight(MMICL_CALL_TIMES_FORM_CTRL_ID, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, &sbs_height);
    GUIFORM_SetMargin(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN);
    GUIFORM_SetSpace(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID,&hor_space,&ver_space);

    sbs_height.add_data = CALLTIME_LIST_CONTACT_INFO_BAR_ICON_HEIGHT;
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID, &sbs_height);
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID, &sbs_height);
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, MMICL_CALL_TIMES_TITLE_BTN_CTRL_ID, &sbs_height);  

    //Set Width
    sbs_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    sbs_width.add_data = CALLTIME_LIST_CONTACT_INFO_BAR_ICON_HEIGHT; //Same as height
    GUIFORM_SetChildWidth(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID,MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID,&sbs_width);
    sbs_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
	sbs_width.add_data = (CALLTIME_LIST_CONTACT_INFO_BAR_LABEL_WID*100/MMI_MAINSCREEN_WIDTH);
    GUIFORM_SetChildWidth(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID,MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID,&sbs_width);

    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    GetCurrentRecordInfo(arraycall_info, &sel_item_index);
        
    /*����Ϊ��*/
    if ( !arraycall_info->call_bcdinfo[sel_item_index].is_num_present )  //show number
    {
        GUILABEL_SetTextById(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID, TXT_UNKNOW_NUM, FALSE);
        //Set ICON
        ctrl_info.is_ctrl_id     = TRUE;
        ctrl_info.ctrl_id        = MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID;
        ctrl_info.ctrl_ptr       = PNULL;
        data_info.img_id         = IAMGE_PB_DEFAULT_PHOTO;
        display_info.is_update   = FALSE;
        display_info.is_disp_one_frame = TRUE;
        display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
        SCI_FREE(arraycall_info);
        return;

    }
    else if(arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        GUILABEL_SetTextById(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID, TXT_KL_PSW_EMERGENCY_CALL, FALSE);
        //Set ICON
        ctrl_info.is_ctrl_id     = TRUE;
        ctrl_info.ctrl_id        = MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID;
        ctrl_info.ctrl_ptr       = PNULL;
        data_info.img_id         = IAMGE_PB_DEFAULT_PHOTO;
        display_info.is_update   = FALSE;
        display_info.is_disp_one_frame = TRUE;
        display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
        SCI_FREE(arraycall_info);

        return;

    }

    //If number in contact.
    if (CheckCurrentNumberInContact())
    {
        MMIPB_BCD_NUMBER_T pb_num = {0};
        wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
        uint32 custom_photo_name_len = 0;
        uint32 photo_file_size = 0;
        MMI_STRING_T    name_strid = {0};
        wchar name_wstr[PHONE_NUM_MAX_LEN + 1]={0};
        GUIANIM_FILE_INFO_T                 file_info  = {0};
        BOOLEAN find_result = FALSE;

        
        SCI_MEMSET(&pb_num,0x00, sizeof(MMIPB_BCD_NUMBER_T));
        pb_num.number_len = arraycall_info->call_bcdinfo[sel_item_index].number_len;
        pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(arraycall_info->call_bcdinfo[sel_item_index].number_type,
          arraycall_info->call_bcdinfo[sel_item_index].number_plan);
        MMI_MEMCPY(pb_num.number,MMIPB_BCD_NUMBER_MAX_LEN,
          arraycall_info->call_bcdinfo[sel_item_index].number,MMIPB_BCD_NUMBER_MAX_LEN,
          pb_num.number_len);
        if(MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num,custom_photo_name, &custom_photo_name_len, &photo_file_size))
        {
            ctrl_info.is_wallpaper_ctrl = FALSE;
            ctrl_info.is_ctrl_id = TRUE;
            ctrl_info.ctrl_id = MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID;
    
            display_info.is_update = FALSE;
            display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
            display_info.is_disp_one_frame = TRUE;
            display_info.is_crop_rect = TRUE;
            display_info.is_zoom = TRUE;
            display_info.is_syn_decode = TRUE;
            file_info.full_path_wstr_ptr = custom_photo_name;
            file_info.full_path_wstr_len = custom_photo_name_len;
            GUIANIM_SetParam(&ctrl_info,PNULL,&file_info,&display_info);
        }
        else
        {
//            uint8                               group_id = PB_GROUP_ALL;

            //Get SIM ICON
//            MMIPB_GetAllGroupIDs(g_pbcalllog_data->matched_contact->union_t.group, &group_id, 1);
//		    if(group_id >= MMIPB_MAX_GROUP_NUM)
//		    {
//			    group_id =PB_GROUP_ALL;
//		    }
            
            //Set ICON

            ctrl_info.is_ctrl_id     = TRUE;
            ctrl_info.ctrl_id        = MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID;
            ctrl_info.ctrl_ptr       = PNULL;
            data_info.img_id         = IAMGE_PB_DEFAULT_PHOTO;//AAA_GetImageIcon(group_id);
            display_info.is_update   = FALSE;
            display_info.is_disp_one_frame = TRUE;
            display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
        }      
        //Set Label
        name_strid.wstr_ptr = name_wstr;
        find_result = MMIAPICL_GetNameByClInfo(arraycall_info->call_bcdinfo[sel_item_index], &name_strid, FALSE);
        GUILABEL_SetText(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID , &name_strid, FALSE);

        //Set Button
        GUIBUTTON_SetVisible(MMICL_CALL_TIMES_TITLE_BTN_CTRL_ID, FALSE,FALSE);
    }
    else
    {
        MMI_STRING_T    number_strid={0};
        wchar number_wstr[PHONE_NUM_MAX_LEN + 1]={0};
        uint8 number_str[PHONE_NUM_MAX_LEN + 1] ={0};
    
        //Set ICON
        ctrl_info.is_ctrl_id     = TRUE;
        ctrl_info.ctrl_id        = MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID;
        ctrl_info.ctrl_ptr       = PNULL;
        data_info.img_id         = IAMGE_PB_DEFAULT_PHOTO;
        display_info.is_update   = FALSE;
        display_info.is_disp_one_frame = TRUE;
        display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
    
        //Set label text 
        GetDisplayPhoneNum(number_str,PHONE_NUM_MAX_LEN+1,&dual_sys);
        MMI_STRNTOWSTR(number_wstr,PHONE_NUM_MAX_LEN+1,number_str,PHONE_NUM_MAX_LEN+1,PHONE_NUM_MAX_LEN+1);
        number_strid.wstr_ptr = number_wstr;
        number_strid.wstr_len = MMIAPICOM_Wstrlen(number_wstr);
        GUILABEL_SetText(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID , &number_strid, FALSE);

        GUIBUTTON_SetVisible(MMICL_CALL_TIMES_TITLE_BTN_CTRL_ID, TRUE,FALSE);
    }

    SCI_FREE(arraycall_info);
    return;
}
#endif
/*****************************************************************************/
// 	Description : init controls of call time list window
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimeListWinCtrl(MMI_WIN_ID_T win_id)
{
    GUIFORM_CHILD_HEIGHT_T  height_info={0};
    uint16 ver_space = 0;
    uint16 hor_space = 0;
#ifdef TOUCH_PANEL_SUPPORT
    GUI_BORDER_T    btn_border= {1,MMI_GRAY_WHITE_COLOR,GUI_BORDER_NONE};
    GUI_BG_T        bg_info = {0};
#endif

    //Set Main Form
    GUIFORM_IsSlide(MMICL_CALL_TIMES_FORM_CTRL_ID,FALSE);
    GUIFORM_IsSlide(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,FALSE);
    GUIFORM_PermitChildBg(MMICL_CALL_TIMES_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildBorder(MMICL_CALL_TIMES_FORM_CTRL_ID, FALSE);
    GUIFORM_SetMargin(MMICL_CALL_TIMES_FORM_CTRL_ID, 0);
    GUIFORM_SetSpace(MMICL_CALL_TIMES_FORM_CTRL_ID, &hor_space, &ver_space);

#ifdef MMI_PDA_SUPPORT
    //Set Title bar
    if (!MMITHEME_IsIstyle())
    {
    
        SetCallTimeListWinTitleBar(win_id);         //Android Style
        //GUILABEL_SetBackgroundColor(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID,MMI_RED_COLOR);
    }    
#if defined(MMI_ISTYLE_SUPPORT)	
    else
    {
        height_info.type = GUIFORM_CHILD_HEIGHT_FIXED;
        height_info.add_data = 0;
        GUIFORM_SetChildHeight(MMICL_CALL_TIMES_FORM_CTRL_ID, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, &height_info);
        GUIFORM_SetVisible(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, FALSE,FALSE);   //Istyle Mode
        SetCallTimeListTitle(win_id);
    }
#endif	
#else    
#endif
#ifdef TOUCH_PANEL_SUPPORT
    //Set Btn Form BG
    bg_info.bg_type = GUI_BG_COLOR;
    bg_info.color = CALLTIME_LIST_BUTTON_FORM_BG_COLOR;

    GUIFORM_SetBg(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,&bg_info);
    
    GUIFORM_PermitChildBorder(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID, FALSE);
    GUIFORM_SetMargin(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID, 0);
    GUIFORM_SetSpace(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID, &hor_space, &ver_space);
    //Set Btn Border
    height_info.type = GUIFORM_CHILD_HEIGHT_FIXED;
    height_info.add_data = CALLTIME_LIST_BUTTON_FORM_HEIGHT;  //Gui requirements

    GUIBUTTON_SetBorder(MMICL_CALL_TIMES_BTN_CALL_CTRL_ID, &btn_border,FALSE);
#ifdef MMI_IP_CALL_SUPPORT
    GUIBUTTON_SetBorder(MMICL_CALL_TIMES_BTN_IPCALL_CTRL_ID, &btn_border,FALSE);
#endif
    GUIBUTTON_SetBorder(MMICL_CALL_TIMES_BTN_MSG_CTRL_ID, &btn_border,FALSE);
    GUIBUTTON_SetBorder(MMICL_CALL_TIMES_BTN_DEL_CTRL_ID, &btn_border,FALSE);

    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,MMICL_CALL_TIMES_BTN_CALL_CTRL_ID, &height_info);
#ifdef MMI_IP_CALL_SUPPORT
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,MMICL_CALL_TIMES_BTN_IPCALL_CTRL_ID, &height_info);
#endif
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,MMICL_CALL_TIMES_BTN_MSG_CTRL_ID, &height_info);
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,MMICL_CALL_TIMES_BTN_DEL_CTRL_ID, &height_info);

#endif
    //Set list ctrl
    height_info.type = GUIFORM_CHILD_HEIGHT_LEFT;
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_FORM_CTRL_ID, MMICL_CALL_TIMES_MENU_CTRL_ID, &height_info);
    GUILIST_PermitBorder(MMICL_CALL_TIMES_MENU_CTRL_ID, FALSE);

    return;
}

/*****************************************************************************/
// 	Description : to handle call time list window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimeListWinMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   call_times_list_ctrl_id = MMICL_CALL_TIMES_MENU_CTRL_ID;
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info =PNULL;
    MMI_WIN_ID_T	query_win_id =MMICL_QUERY_WIN_ID;
    uint16 cur_selection=0;
#ifndef  MMI_MULTI_GREEN_KEY_SINGLE
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
#endif
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
     
    switch(msg_id)
    {   
    case MSG_OPEN_WINDOW:
        {

            SetCallTimeListWinCtrl(win_id);
            GUILIST_SetMaxItem(call_times_list_ctrl_id, MMICL_MAX_CALL_TIMES, FALSE );
            /*title*/
//            SetCallTimeListTitle(win_id);
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"HandleDialedChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10951_112_2_18_2_11_51_103,(uint8*)"");
                return recode;
            }
#ifndef MMI_PDA_SUPPORT
            SetCallTimeListTitle(win_id);
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif

            MMK_SetAtvCtrl(win_id, call_times_list_ctrl_id);
        }
        break;

    case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
#endif
        SetCallTimeListItem(call_times_list_ctrl_id);
        break;
    case MSG_GET_FOCUS:
        MMK_SetAtvCtrl(win_id, call_times_list_ctrl_id);
#ifdef MMI_PDA_SUPPORT
    if (!MMITHEME_IsIstyle())
    {
        SetCallTimeListWinTitleBar(win_id);         //Android Style
        //GUILABEL_SetBackgroundColor(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID,MMI_RED_COLOR);
    }    
#else
        SetCallTimeListTitle(win_id);
#endif
        break;
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
    {
        //add anim into list item
        GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
        if(PNULL != need_item_content_ptr)
        {
               DrawListItemIconData(arraycall_info,need_item_content_ptr);                
        }
        break;
    }
    case MSG_NOTIFY_TPMOVE_LEFT_SIDE:
        HandleAnswerSms();
        break;
    case MSG_NOTIFY_TPMOVE_RIGHT_SIDE:
        HandleRingBackByType(CC_CALL_NORMAL_CALL);
        break;
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
#endif
    case MSG_APP_MENU:
#ifdef MMI_PDA_SUPPORT
        
#else
#ifdef MMI_CL_MINI_SUPPORT
        MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
#else
        cur_selection = GUILIST_GetCurItemIndex(call_times_list_ctrl_id);
        MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_LIST_OPTION_WIN_TAB, (ADD_DATA)cur_selection);
#endif
#endif
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef MMI_PDA_SUPPORT
        
#else
//fixed by bingjie.chen
        cur_selection = GUILIST_GetCurItemIndex(call_times_list_ctrl_id);
        MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_DETAIIL_WIN_TAB, (ADD_DATA)cur_selection);       
#endif        
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T ctrl_id = 0;
            MMIPB_BCD_NUMBER_T bcd_number = {0};
            

            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
            cur_selection = GUILIST_GetCurItemIndex(call_times_list_ctrl_id);
            GetPartyPhoneNum(&bcd_number);
            switch(ctrl_id)
            {
            case MMICL_CALL_TIMES_BTN_CALL_CTRL_ID:
                HandleRingBackByType(CC_CALL_NORMAL_CALL);
                break;
                
#ifdef MMI_IP_CALL_SUPPORT
            case MMICL_CALL_TIMES_BTN_IPCALL_CTRL_ID:
                HandleRingBackByType(CC_CALL_IP_CALL);
                break;
#endif

            case MMICL_CALL_TIMES_BTN_MSG_CTRL_ID:
                HandleAnswerSms();
                break;

            case MMICL_CALL_TIMES_BTN_DEL_CTRL_ID:
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleCallTimeListDeleteSelectedQueryWinMsg);
                break;
#ifdef MMI_PDA_SUPPORT
            case MMICL_CALL_TIMES_TITLE_BTN_CTRL_ID:
                MMK_CreatePubListWin((uint32 *)MMICL_CALL_TIME_POP_LIST_WIN_TAB,PNULL);
                break;
#endif

#ifndef MMI_PDA_SUPPORT     //PDAʱ����Ҫ����DETAIL����
            case MMICL_CALL_TIMES_MENU_CTRL_ID:
                MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_DETAIIL_WIN_TAB, (ADD_DATA)cur_selection);
                break;
#endif
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527


    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;

     case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
        }
        break;
// add by bingjie.chen        

    case MSG_PROMPTWIN_OK: 
#ifdef CALL_TIMES_SUPPORT
        MMIPUB_CloseQuerytWin(&query_win_id);
        cur_selection = GUILIST_GetCurItemIndex(call_times_list_ctrl_id);
        MMICL_DeleteOneCallTimesItem(s_cur_sel_log_list_type,cur_selection);
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        //MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;



//add by bingjie.chen
#ifdef  MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
         s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
         break;

    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1] && GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
        {
        HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;

#else
                
         case MSG_KEYDOWN_CALL1:
         case MSG_KEYDOWN_CALL2:
         case MSG_KEYDOWN_CALL3:
         case MSG_KEYDOWN_CALL4:
              {
                    dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
                    s_is_allow_sim_call[dual_sys] = TRUE;
              }
              break;
                 
         case MSG_KEYUP_CALL1:
         case MSG_KEYUP_CALL2:
         case MSG_KEYUP_CALL3:
         case MSG_KEYUP_CALL4:
             {
                 dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
                 sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX); 
             
                 if(s_is_allow_sim_call[dual_sys] && GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
                 {
                    HandleRingBack(sim_type, FALSE);
                 }
                    s_is_allow_sim_call[dual_sys]= FALSE;
             }
              break;
    
#endif
             
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : to handle call time list option window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void  SetCallTimesListOptMenuTitle(uint16 call_times_index)
 {
    GUIMENU_TITLE_INFO_T title_info = {0};
    uint8 temp_str[PHONE_NUM_MAX_LEN] = {0};
    wchar temp_wstr_1[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar temp_wstr_2[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar temp_wstr_3[GUILIST_STRING_MAX_NUM + 1] = {0};
    MMI_TM_T tm = {0};
    uint16   sel_item_index =0;
    MMICL_CALL_ARRAY_BCDINFO_T * arraycall_info =PNULL;
    uint16  times_index = 0;
    MMI_STRING_T title_string ={0};
    uint16 temp_str_len = 0;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetCallTimesListOptMenuTitle call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11100_112_2_18_2_11_51_104,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    GetCurrentRecordInfo(arraycall_info, &sel_item_index);
    
    times_index = arraycall_info->call_bcdinfo[sel_item_index].call_times -call_times_index -1;
    tm = MMIAPICOM_Second2Tm( arraycall_info->call_bcdinfo[sel_item_index].call_start_time[times_index]);

    /* display date */
    SCI_MEMSET(temp_str,0x00,sizeof(temp_str));
    MMIAPISET_FormatDateStrByDateStyle(tm.tm_year, tm.tm_mon, tm.tm_mday,
        '/', temp_str,PHONE_NUM_MAX_LEN );
    SCI_MEMSET(temp_wstr_1,0x00,sizeof(wchar) * (GUILIST_STRING_MAX_NUM + 1));
    temp_str_len = SCI_STRLEN((char*)temp_str);
    MMI_STRNTOWSTR( (wchar*)temp_wstr_1,
        GUILIST_STRING_MAX_NUM,
        (uint8 *)temp_str,
        PHONE_NUM_MAX_LEN,
        temp_str_len
        );
    
    /*display time*/
    SCI_MEMSET(temp_str,0x00,sizeof(temp_str));
    MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min,temp_str,PHONE_NUM_MAX_LEN);
    SCI_MEMSET(temp_wstr_2,0x00,sizeof(wchar) * (GUILIST_STRING_MAX_NUM + 1));
    temp_str_len = SCI_STRLEN((char*)temp_str);
    MMI_STRNTOWSTR((wchar*)temp_wstr_2,
        GUILIST_STRING_MAX_NUM,
        (uint8 *)temp_str,
        PHONE_NUM_MAX_LEN,
        temp_str_len
        );
    SCI_MEMSET(temp_wstr_3,0x00,sizeof(wchar) * (GUILIST_STRING_MAX_NUM + 1));
    MMI_STRNTOWSTR((wchar*)temp_wstr_3,GUILIST_STRING_MAX_NUM,(uint8 *)"    ", 4,4);//date ��time֮������4�ո�
    MMIAPICOM_Wstrncat((wchar*)temp_wstr_1,(const wchar*)temp_wstr_3,4);
    temp_str_len = MMIAPICOM_Wstrlen(temp_wstr_2);
    MMIAPICOM_Wstrncat((wchar*)temp_wstr_1,temp_wstr_2,temp_str_len);  
    
    title_string.wstr_ptr = temp_wstr_1;
    title_string.wstr_len = MMIAPICOM_Wstrlen((const wchar*)temp_wstr_1);
    
    title_info.is_static = TRUE;
    title_info.title_ptr = &title_string;
    title_info.sub_title_ptr = PNULL;
    title_info.group_id = MENU_CL_CALL_TIMES_LIST_OPT;
    GUIMENU_SetMenuTitleEx(MMICL_CALL_TIMES_LIST_OPT_CTRL_ID, &title_info);

    SCI_FREE(arraycall_info);
}
#endif


#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : to handle the call times list option window message
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimesListOptWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    uint32          group_id = 0, menu_id = 0;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
    MMI_CTRL_ID_T option_ctrl_id =MMICL_CALL_TIMES_LIST_OPT_CTRL_ID;
    uint32    select_index = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (!CheckCurrentNumberInContact())
        {
            GUIMENU_SetItemVisible(MMICL_CALL_TIMES_LIST_OPT_CTRL_ID,MENU_CL_CALL_TIMES_LIST_OPT , ID_CL_CONTACT_VIEW, FALSE);
        }
#ifdef MMI_BLACKLIST_SUPPORT
        {
            MMIPB_BCD_NUMBER_T  tele_num = {0};
            //get the phone number(bcd)
            GetPartyPhoneNum(&tele_num);
            
            if (MMIAPISET_IsCCBlacklist(&tele_num,MN_CALL_TYPE_NORMAL))
            {
                GUIMENU_SetItemVisible(MMICL_CALL_TIMES_LIST_OPT_CTRL_ID,MENU_CL_CALL_TIMES_LIST_OPT , ID_CL_LOG_LIST_REFUSE_ADD, FALSE);
            }
            else
            {
                GUIMENU_SetItemVisible(MMICL_CALL_TIMES_LIST_OPT_CTRL_ID,MENU_CL_CALL_TIMES_LIST_OPT , ID_CL_LOG_LIST_REFUSE_REMOVE, FALSE);
            }
        }    
#endif            
        
        SetCallTimesListOptMenuTitle((uint16)select_index);
        MMK_SetAtvCtrl(win_id, option_ctrl_id);
        break;
        
    case MSG_GET_FOCUS:
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifndef  MMI_PDA_SUPPORT    //not respond menu key
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        GUIMENU_GetId(option_ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_EDIT_BEFORE_DIAL:
            {
                uint8 tele_num[PHONE_NUM_MAX_LEN + 2];
                MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
                //Get dispaynum
                GetDisplayPhoneNum(tele_num,PHONE_NUM_MAX_LEN + 2 , &dual_sys);

                MMIAPICL_OpenEditBeforeDialWin((uint16)strlen((char*)tele_num),tele_num);
                MMK_CloseWin(win_id);
            }

            break;
        case ID_CL_CONTACT_VIEW:
            OpenNumberInContact();
            MMK_CloseWin(win_id);
            break;
        
        case ID_CL_LOG_LIST_DELETE:
            //ѯ���Ƿ�ɾ��!! 
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
            //MMK_CloseWin(win_id);
            break;
            
        case ID_CL_LOG_DETAIL:
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, PNULL);
            MMK_CloseWin(win_id);
            break;
            
        case ID_CL_ADD_TO_PB:
            HandleDistillPhoneNum();
            MMK_CloseWin(win_id);
            break;
            
 #ifdef VT_SUPPORT
       case ID_CL_DIAL_VIDEOCALL://@zhaohui add
#if 1//OPEN_VT_CHANGE
            HandleRingBack(CC_CALL_SIM_MAX, TRUE);
#else
            HandleRingBack(CC_CALL_SIM_MAX, TRUE, PNULL);
#endif
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;
#endif

        case ID_CL_DIAL_AUDIOCALL:
            HandleRingBack(CC_CALL_SIM_MAX, FALSE);
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;
            
#ifdef MMI_IP_CALL_SUPPORT
        case ID_CL_DIAL_IPCALL:
            HandleIpCall();
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT            
        case ID_CL_ADD_TO_EXIST_CONTACT:
            HandleAddNumToName();
            MMK_CloseWin(win_id);
            break;
#endif            
        case ID_CL_SEND_SMS:
            HandleAnswerSms();
            MMK_CloseWin(win_id);
            break;
            
        case ID_CL_SEND_MMS:
            HandleAnswerMms();
            MMK_CloseWin(win_id);
            break;
            
#if defined(MMI_BLACKLIST_SUPPORT)
        case ID_CL_LOG_LIST_REFUSE_ADD:
            {
                MMIPB_BCD_NUMBER_T  tele_num = {0};
                //get the phone number(bcd)
                GetPartyPhoneNum(&tele_num);
                //AddNumToBlackList(tele_num);
                MMK_CloseWin(win_id);
            }
            break;
        case ID_CL_LOG_LIST_REFUSE_REMOVE:
                {
                    MMIPB_BCD_NUMBER_T  tele_num = {0};
                    //get the phone number(bcd)
                    GetPartyPhoneNum(&tele_num);
                    if (MMIAPISET_DeleteBlacklist(tele_num))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMPLETE);                    
                    }    
                    
                    MMK_CloseWin(win_id);
                }
            break;
#endif

#ifdef AOC_SUPPORT
        case ID_CL_COSTS_TOTAL:
        case ID_CL_COSTS_MAX:
        case ID_CL_COSTS_RATE:
        case ID_CL_COSTS_RESET:
            MMICL_COSTS_HandleMainMenuMsg(menu_id);
            break;
#endif

        default:
            break;
        }
        break;

    #ifdef  MMI_PDA_SUPPORT    //not respond menu key
    case MSG_APP_OK:
    case MSG_CTL_OK:               
    #endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        //�رռ�¼��menu���ں�ѯ�ʴ���
        GUIMENU_GetId(option_ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_LOG_LIST_DELETE:
            MMIPUB_CloseQuerytWin(&query_win_id);
            DeleteOneCallTimesItem((uint16)select_index);
#if !defined(MMI_PDA_SUPPORT)
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            MMK_CloseWin(win_id);
            break;
        case ID_CL_SEND_MMS:
            MMIPUB_CloseQuerytWin(&query_win_id);
            HandleAnswerMms();
            break;
            
#if defined(MMI_BLACKLIST_SUPPORT)
        case ID_CL_LOG_LIST_REFUSE_ADD:
            {
                MMIPB_BCD_NUMBER_T  tele_num = {0};
                //get the phone number(bcd)
                GetPartyPhoneNum(&tele_num);
                //AddNumToBlackList(tele_num);
                MMK_CloseWin(win_id);
            }
            break;
        case ID_CL_LOG_LIST_REFUSE_REMOVE:
                {
                    MMIPB_BCD_NUMBER_T  tele_num = {0};
                    //get the phone number(bcd)
                    GetPartyPhoneNum(&tele_num);
                    if (MMIAPISET_DeleteBlacklist(tele_num))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMPLETE);                    
            }            
                    MMK_CloseWin(win_id);
                    
                }
            break;
#endif

        default:
            break;
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //�ر�ѯ�ʴ���
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : to delete one call times list
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void DeleteOneCallTimesItem(uint16 select_index)
{
    uint16   sel_item_index =0;
    MMICL_CALL_ARRAY_BCDINFO_T * arraycall_info =PNULL;
    uint16  real_call_times_index = 0;
    uint16  i =0;
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetCallTimesListOptMenuTitle call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11365_112_2_18_2_11_51_105,(uint8*)"");
        return;
    }

    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    GetCurrentRecordInfo(arraycall_info, &sel_item_index);

    real_call_times_index  = arraycall_info->call_bcdinfo[sel_item_index].call_times - select_index -1;
    arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index] = 0;
    arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[real_call_times_index] =0;
    arraycall_info->call_bcdinfo[sel_item_index].call_times--;

#ifdef MMI_LDN_SUPPORT
    //delete ldn
    //if (arraycall_info->call_bcdinfo[pos].is_sim_record)
    {
        MMICL_CALL_INFO_T call_info = {0};

        call_info.dual_sys = arraycall_info->call_bcdinfo[sel_item_index].dual_sys;
        call_info.call_num.is_num_present = arraycall_info->call_bcdinfo[sel_item_index].is_num_present;
        call_info.call_num.number.number_plan = arraycall_info->call_bcdinfo[sel_item_index].number_plan;
        call_info.call_num.number.number_type = arraycall_info->call_bcdinfo[sel_item_index].number_type;
        call_info.call_num.number.num_len = MIN(arraycall_info->call_bcdinfo[sel_item_index].number_len, MN_MAX_ADDR_BCD_LEN);
        SCI_MEMCPY(&call_info.call_num.number.party_num, arraycall_info->call_bcdinfo[sel_item_index].number, call_info.call_num.number.num_len);

        if (arraycall_info->call_bcdinfo[sel_item_index].is_sim_record && 0 < arraycall_info->sim_record_num)
        {
            arraycall_info->sim_record_num--;
        }

        MMICL_DeleteCallInfo(s_cur_sel_log_list_type, call_info.dual_sys, &call_info);
    }
#endif

    //��������0��ɾ������ϵ�˵ļ�¼,  ����
    if(0 == arraycall_info->call_bcdinfo[sel_item_index].call_times)
    {
        DeleteCurrentRecord();
        MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
        MMK_CloseWin(MMICL_CALL_TIMES_WIN_ID);
        return;
    }
    //�������е�ʱ����ǰ��һλ
    for(i=0;i <select_index;i++)
    {
        arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index + i] =
            arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index+i+1];
        arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[real_call_times_index + i]= 
            arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[real_call_times_index+i+1];
    }
    //����ʱ����0
    arraycall_info->call_bcdinfo[sel_item_index].call_start_time[arraycall_info->call_bcdinfo[sel_item_index].call_times] =0;
    arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[arraycall_info->call_bcdinfo[sel_item_index].call_times]=0;

    //update NV 
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        MMICL_WriteNV(s_match_call_info_ptr->call_info[sel_item_index].call_type, arraycall_info);
    }
    else
    {
        MMICL_WriteNV(s_cur_sel_log_list_type, arraycall_info);
    }
    
    MMICL_UpdateEventInd();
    
    SCI_FREE(arraycall_info);
}
#endif

/*****************************************************************************/
// 	Description : set detail of call times list
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimesListDetail(uint16 select_index)
{
    uint8   tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    wchar   tele_num_wstr[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8   time[MMICL_DISPLAY_TIME_LEN + 1 ] = {0};
    wchar   time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    uint8   date[MMICL_DISPLAY_DATE_LEN + 1] = {0};
    wchar   date_wstr[MMICL_DISPLAY_DATE_LEN + 1] = {0};
#ifndef CMCC_UI_STYLE
    uint8   duration_time[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    wchar   duration_time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
#endif
    uint16  sel_item_index = 0;
    MMI_TM_T    tm = {0};
    MMI_STRING_T    name_str = {0};
    wchar str_ptr[MMIPB_MAX_STRING_LEN + 1]     = {0};
    MMI_STRING_T    num_str = {0};
    MMI_STRING_T    time_str = {0};
    MMI_STRING_T    date_str = {0};
    MMI_STRING_T    duration_time_str = {0};
    MMI_STRING_T    call_type_str = {0};
    MMIPB_BCD_NUMBER_T  bcd_number = {0};
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_STRING_T    sim_num = {0};
    MMISET_SIM_NAME_T sim_name = {0};
#endif
    MMI_STRING_T    location_str = {0};
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
    uint16  city_ucs2_str[MMIFINDMP_CITYNAME_LENGTH + 1] = {0};
    uint16  str_length = 0;
    uint8   origin_number_arr[CC_MAX_TELE_NUM_LEN + 2] = {0};
    int16   origin_num_len  = 0;
#endif

    uint16 index = 0;
    uint16 real_call_times_index = 0;
    MMI_CTRL_ID_T ctrl_id = MMICL_CALL_TIMES_DETAIL_CTRL_ID;
    
    //initialize
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListDetail call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11482_112_2_18_2_11_52_106,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(&tm,0,sizeof(tm));
    SCI_MEMSET(&name_str,0,sizeof(name_str));
    SCI_MEMSET(&num_str,0,sizeof(num_str));
    SCI_MEMSET(&time_str,0,sizeof(time_str));
    SCI_MEMSET(&date_str,0,sizeof(date_str));
    SCI_MEMSET(&bcd_number,0,sizeof(bcd_number));
    
    GetCurrentRecordInfo(arraycall_info, &sel_item_index);
        
    //get name
    if (!arraycall_info->call_bcdinfo[sel_item_index].is_num_present)//�绰���벻����
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_UNKNOW_NUM,&num_str);//�պ�
    }
    else
    {

        //get number
        MMICL_GenDispNumber(arraycall_info->call_bcdinfo[sel_item_index].call_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_len,
            arraycall_info->call_bcdinfo[sel_item_index].number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);
        num_str.wstr_len = (uint16)strlen((char*)tele_num);
        num_str.wstr_ptr = tele_num_wstr;//(uint8 *)tele_num;
        MMI_STRNTOWSTR( tele_num_wstr, PHONE_NUM_MAX_LEN, (uint8 *)tele_num, num_str.wstr_len, num_str.wstr_len );

        name_str.wstr_ptr = str_ptr;
        MMIAPICL_GetNameByClInfo(arraycall_info->call_bcdinfo[sel_item_index], &name_str, FALSE);
    }
    
    //get time and date
    real_call_times_index = arraycall_info->call_bcdinfo[sel_item_index].call_times - select_index -1;
    
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index]);
    MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min,time,MMICL_DISPLAY_TIME_LEN + 1 );
    time_str.wstr_ptr = time_wstr;//(uint8 *)time;
    //time_str.is_ucs2 = FALSE;
    time_str.wstr_len = (uint16)strlen((char*)time);

    MMI_STRNTOWSTR( time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)time, time_str.wstr_len, time_str.wstr_len );
    
    MMIAPISET_FormatDateStrByDateStyle(tm.tm_year,tm.tm_mon,tm.tm_mday,'/', date,MMICL_DISPLAY_DATE_LEN + 1);
    date_str.wstr_ptr = date_wstr;
    //date_str.is_ucs2 = FALSE;
    date_str.wstr_len = (uint16)strlen((char*)date);
    MMI_STRNTOWSTR( date_wstr, MMICL_DISPLAY_DATE_LEN, (uint8 *)date, date_str.wstr_len, date_str.wstr_len );
    
#ifndef CMCC_UI_STYLE
    //get duration time
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[real_call_times_index]);
    sprintf((char*)duration_time, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    duration_time_str.wstr_ptr  = duration_time_wstr;
    duration_time_str.wstr_len    = (uint16)strlen((char*)duration_time);
    //duration_time_str.is_ucs2   = FALSE;
    MMI_STRNTOWSTR( duration_time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)duration_time, duration_time_str.wstr_len, duration_time_str.wstr_len );
#endif

    //get location
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
    if (arraycall_info->call_bcdinfo[sel_item_index].is_num_present)//�绰���벻����
    {
        uint8 origin_number_str[CC_MAX_TELE_NUM_LEN + 1] = {0};

        MMI_WSTRNTOSTR((uint8 *)origin_number_str, CC_MAX_TELE_NUM_LEN, num_str.wstr_ptr, num_str.wstr_len, num_str.wstr_len);

        if(MMIAPICC_RemoveIpNumber( origin_number_str, num_str.wstr_len, origin_number_arr,CC_MAX_TELE_NUM_LEN))
        {
            origin_num_len = strlen((char*)origin_number_arr);
        }
        else
        {
            MMI_WSTRNTOSTR((uint8 *)origin_number_arr, CC_MAX_TELE_NUM_LEN, num_str.wstr_ptr, num_str.wstr_len, num_str.wstr_len );
            //MMI_MEMCPY(origin_number_arr, CC_MAX_TELE_NUM_LEN + 2, num_str.str_ptr, num_str.length, num_str.length);
            origin_num_len = num_str.wstr_len;
        }
        
        str_length = MMIAPIPHONESEARCH_FindLocation(origin_number_arr, origin_num_len, (uint16*)city_ucs2_str);
    }
    else
    {
        str_length = 0;
    }
    if( str_length == 0 )
    {
        MMI_GetLabelTextByLang(TXT_UNKNOWN, &location_str);
    }   
    else if( str_length == MMIPHONESEARCH_LOCALNUMBER )
    {
        MMI_GetLabelTextByLang(TXT_LOCAL, &location_str);
    }   
    else
    {
        location_str.wstr_ptr = city_ucs2_str;
        //location_str.is_ucs2 = TRUE;
        location_str.wstr_len  = str_length;
    }
#endif
    
    //get call type
    if (arraycall_info->call_bcdinfo[sel_item_index].isvideocall)
    {
        MMI_GetLabelTextByLang(TXT_CALL_VIDEO, &call_type_str);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CALL_AUDIO, &call_type_str);
    }   

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    //Emergency Number does not display sim name!!    
    if(!arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        //get sim name
        sim_name = MMIAPISET_GetSimName(arraycall_info->call_bcdinfo[sel_item_index].dual_sys);
        
        sim_num.wstr_len  = sim_name.wstr_len;
        sim_num.wstr_ptr = sim_name.wstr_arr;
    }
#endif

    //display name in pb
    if (arraycall_info->call_bcdinfo[sel_item_index].is_num_present && (0 != name_str.wstr_len))
    {
        //if the num is present and the name is being
        SetHeadAndContentInRText(TXT_NAME,ctrl_id,name_str,&index);
    }

    //display num
    SetHeadAndContentInRText(TXT_COMMON_NUMBER,ctrl_id,num_str,&index);

#ifdef CALL_TIMES_SUPPORT
    if (arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index] > 0)
#else
    if (arraycall_info->call_bcdinfo[sel_item_index].call_start_time > 0)
#endif
    {
        //display date
        SetHeadAndContentInRText(TXT_DATE,ctrl_id,date_str,&index);

        //display time
        SetHeadAndContentInRText(TXT_TIME,ctrl_id,time_str,&index);
    }

    //display duration
    if ((MMICL_CALL_DIALED == s_cur_sel_log_list_type || MMICL_CALL_RECEIVED == s_cur_sel_log_list_type)
        &&(!arraycall_info->call_bcdinfo[sel_item_index].isvideocall)
#ifdef CALL_TIMES_SUPPORT
        && arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index] > 0
#else
        && arraycall_info->call_bcdinfo[sel_item_index].call_start_time > 0
#endif
        )//@zhaohui modify,��Ƶͨ������ʾʱ����Ϣ
    {
        SetHeadAndContentInRText(TXT_CL_CALL_TIME,ctrl_id,duration_time_str,&index);
    }

    //display location
    SetHeadAndContentInRText(TXT_GUISHUDI,ctrl_id,location_str,&index);

    //display type  voice call/video call	
    SetHeadAndContentInRText(TXT_COMMON_TYPE,ctrl_id,call_type_str,&index);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    //Emergency Number does not display sim name!!
    if(!arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        //display sim name
        SetHeadAndContentInRText(TXT_CL_DETAIL_SIM_HEAD,ctrl_id,sim_num,&index);
     }
#endif   

    //��¼�����Ϣ��Ϊ�绰�����ṩ��Ҫ��Ϣ
    s_call_log_detail_info_ptr->dual_sys = arraycall_info->call_bcdinfo[sel_item_index].dual_sys;
    SCI_MEMSET(s_call_log_detail_info_ptr->tele_num, 0, (PHONE_NUM_MAX_LEN + 2));
    SCI_MEMCPY(s_call_log_detail_info_ptr->tele_num, tele_num, (PHONE_NUM_MAX_LEN + 2));
#if defined(CL_SAVE_NAME_SUPPORT)
    if (arraycall_info->call_bcdinfo[sel_item_index].name.is_use_nv_name)
    {
        SCI_MEMCPY((void *)&s_call_log_detail_info_ptr->name,(void *)&arraycall_info->call_bcdinfo[sel_item_index].name,sizeof(MMICL_NAME_T));
    }
#endif
    //free memory
    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
    }
}

/*****************************************************************************/
// 	Description : to handle the call times detail window message
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimesDetailWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T ctrl_id = MMICL_CALL_TIMES_DETAIL_CTRL_ID;
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
#endif
    uint32 sel_time_index = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL == s_call_log_detail_info_ptr)
        {
            s_call_log_detail_info_ptr = (MMICL_DETAIL_INFO_T *)SCI_ALLOCAZ(sizeof(MMICL_DETAIL_INFO_T));
            if(PNULL == s_call_log_detail_info_ptr)
            {
                //SCI_TRACE_LOW:"HandleLogListDetailWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11705_112_2_18_2_11_52_107,(uint8*)"");
                return MMI_RESULT_FALSE;
            }
            SCI_MEMSET(s_call_log_detail_info_ptr, 0, sizeof(MMICL_DETAIL_INFO_T));
        }
        if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type)
        {
            GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)STXT_OPTION,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN, FALSE);
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        GUILIST_SetMaxItem(ctrl_id, 100, FALSE);
#endif
//add by bingjie.chen
        GUIRICHTEXT_SetLineSpace(ctrl_id, 0);
        SetCallTimesListDetail((uint16)sel_time_index);
#ifdef DPHONE_SUPPORT //added by taiping.lai 2011/01/12
#ifndef MMI_PDA_SUPPORT
        GUIRICHTEXT_SetItemBorderSpace(ctrl_id, 0);
#endif
#endif 	 
        break;
            
    case MSG_GET_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        break;

    case MSG_LOSE_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK: //rong.gu to do
    case MSG_APP_OK:
    case MSG_APP_MENU:
        switch (s_cur_sel_log_list_type)
        {
        case MMICL_CALL_MISSED:
        case MMICL_CALL_RECEIVED:
        case MMICL_CALL_DIALED:
        case MMICL_CALL_MATCHED:
#ifdef MMI_PDA_SUPPORT            
        case MMICL_CALL_ALL:
#endif            
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_CALL_REFUSED:
#endif
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN2_TAB, PNULL);
            break;
            
        default:
    //           SCI_TRACE_LOW("HandleLogListDetailWindow:s_cur_sel_log_list_type %d is error!",s_cur_sel_log_list_type);
            break;
        }
        break;
        
#ifdef MMI_MULTI_GREEN_KEY_SINGLE         
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        break;
#else        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);                
            HandleRingBack(sim_type, FALSE);
        }
        break;
#endif

#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
#if 1//OPEN_VT_CHANGE
        HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
#else
        HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
#endif
        break;        
#endif
        
        //@samboo.shen һ��vt
    case MSG_APP_VIDEO_TEL:
        HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        MMK_CloseWin(win_id);
        break; 
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != s_call_log_detail_info_ptr)
        {
            SCI_FREE(s_call_log_detail_info_ptr);
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

#endif

/*****************************************************************************/
// 	Description : get record num by type and get record content to arraycall_info
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: WARNING: "GetCurrTypeNum" just get record num by type, only for pda project
//                 "GetRecordNumByType" not only get record num, but also assign arraycall_info
/*****************************************************************************/
LOCAL uint8 GetRecordNumByType(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info_ptr, MMICL_CALL_TYPE_E e_call_type)
{
    uint8 call_log_num = 0;
    if(PNULL == arraycall_info_ptr)
    {
        //SCI_TRACE_LOW:"mmicl GetRecordNumByType null ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11835_112_2_18_2_11_52_108,(uint8*)"");
        return call_log_num;
    }
    //[implement custom call log list UI]
    #ifdef MMI_PDA_SUPPORT
    if (MMICL_CALL_ALL ==e_call_type)
    {
        e_call_type = GetAllLogFillter();
        if (e_call_type == MMICL_CALL_PB_NUMBER_MATCHED)
        {
             call_log_num= GetCalllogPBData_MatchedItemCount();
             return call_log_num;
        }
    }
    #endif
    //[end]

    MMICL_GetRecordInfo(e_call_type, arraycall_info_ptr);
#ifdef MMI_PDA_SUPPORT            
    call_log_num = GetCurrTypeNum( e_call_type, arraycall_info_ptr);
#else
    call_log_num = arraycall_info_ptr->record_num;
#endif

#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error
    if (MMICL_CALL_PB_NUMBER_MATCHED ==e_call_type)
    {
        call_log_num= GetCalllogPBData_MatchedItemCount();
    }
#endif
    return call_log_num;
}
/*****************************************************************************/
//  Description : ���CL��ĳ����¼��Ӧ��������get name by cl record, the name is from pb or the record itself
//  Global resource dependence : none
//  Author: tao.xue
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetNameByClInfo( MMICL_CALL_BCDINFO_T cl_nv_info, MMI_STRING_T* name_str, BOOLEAN is_use_default_name)
{
    BOOLEAN b_search_result = FALSE;
    MMIPB_BCD_NUMBER_T pb_num = {0};
    int origin_num_len = 0;
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8 tele_number_no_ip[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint32 tele_len = 0;

    //SCI_TRACE_LOW:"Enter MMIAPICL_GetNameByClInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11878_112_2_18_2_11_52_109,(uint8*)"");

    if(PNULL == name_str || PNULL == name_str->wstr_ptr || 0 == cl_nv_info.number_len)
    {
        //SCI_TRACE_LOW:"MMIAPICL_GetNameByClInfo index:parameter error !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11882_112_2_18_2_11_52_110,(uint8*)"");
        return FALSE;
    }
#if defined(CL_SAVE_NAME_SUPPORT)
    if (cl_nv_info.name.is_use_nv_name)
    {
        name_str->wstr_len  = cl_nv_info.name.wstr_len;
        if (name_str->wstr_len > 0)
        {
            MMI_WSTRNCPY(
                                        name_str->wstr_ptr,
                                        MMICL_NAME_MAX_LEN,
                                        cl_nv_info.name.wstr,
                                        MMICL_NAME_MAX_LEN,
                                        name_str->wstr_len
                                        );
            b_search_result = TRUE;
        }
        else
        {
            b_search_result = FALSE;
        }
    }
    else
#endif
    {
        //remove ip header
        tele_len = MMICL_GenDispNumber(cl_nv_info.call_type,
            cl_nv_info.number_type,
            (uint8)cl_nv_info.number_len,
            cl_nv_info.number,
            tele_num,
            CC_MAX_TELE_NUM_LEN + 2);

        pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(cl_nv_info.number_type, MN_NUM_PLAN_UNKNOW);
        pb_num.number_len = MIN(MN_MAX_ADDR_BCD_LEN, cl_nv_info.number_len);
        MMI_MEMCPY(
                                pb_num.number,
                                sizeof(pb_num.number),
                                (void*)cl_nv_info.number,
                                sizeof(cl_nv_info.number),
                                pb_num.number_len
                                );

        b_search_result = MMIAPIPB_GetNameByNumber(&pb_num, name_str, PHONE_NUM_MAX_LEN, is_use_default_name);/*lint !e506 */

        if(!b_search_result && MMIAPICC_RemoveIpNumber(tele_num, tele_len, tele_number_no_ip,CC_MAX_TELE_NUM_LEN))
        {
            SCI_MEMSET(&pb_num, 0x00, sizeof(pb_num));

            origin_num_len = strlen((char*)tele_number_no_ip);

            if(MMIAPICOM_GenPartyNumber(tele_number_no_ip, origin_num_len, &party_num))
            {
                pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_num.number_len = MIN(MN_MAX_ADDR_BCD_LEN, party_num.num_len);
                MMI_MEMCPY(
                                        pb_num.number,
                                        sizeof(pb_num.number),
                                        (void*)&party_num.bcd_num,
                                        sizeof(party_num.bcd_num),
                                        pb_num.number_len
                                        );

                b_search_result = MMIAPIPB_GetNameByNumber(&pb_num, name_str, PHONE_NUM_MAX_LEN, is_use_default_name);/*lint !e506 */
            }
            else
            {
                return FALSE;
            }
        }
    }

    return b_search_result;
}

/*****************************************************************************/
//  Description : Select SIM API of CL app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMICL_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC  MMICLSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E append_type,
                                        ADD_DATA        add_data
                                        )
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MMI_WIN_ID_T    selectSimWinID = MMICL_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    caller_data.append_type = (uint32)append_type;
    caller_data.callback_func =MMICLSelectSIMWinCallback ;
    caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;
    caller_data.publist_id = MMICL_COM_SELECT_SIM_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = add_data;
    caller_data.extra_data = PNULL;

    MMIAPIPHONE_OpenSelectSimWin( &caller_data);
#else
    MMICLSelectSIMWinCallback(MN_DUAL_SYS_1, 1, PNULL);
#endif
    return MMI_RESULT_TRUE;
}




#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error

/*****************************************************************************/
//	Description : the handle function of call list delete all menu window
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleLogListDeleteSelectedQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        DeleteAllCallLog(s_cur_sel_log_list_type);
        CalllogPBData_StopSelection();
        MMK_CloseWin(win_id);
        MMK_CloseWin(MMICL_PB_DETAIL_SELECT_WIN_ID);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}


/*****************************************************************************/
// 	Description : to handle the contact call log list window message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailCallLogListWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_ALL_LOG_PB_DETAIL_LIST_CTRL_ID;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
    MMI_WIN_ID_T info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);
    //uint8 call_log_num = 0;

    MMI_HANDLE_T parent_win_id = MMK_GetParentWinHandle(win_id);
    MMIPB_CONTACT_T * contact_ptr = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(parent_win_id);
    MMI_HANDLE_T    list_handle =MMK_GetCtrlHandleByWin(win_id,MMICL_ALL_LOG_PB_DETAIL_LIST_CTRL_ID);

    if(contact_ptr == PNULL)
    {
        return  recode;
    }

    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        if (CalllogPBData_GetMainWinID() == PNULL)
        {
            InitPbCalllogData(parent_win_id);
        }

        SetAllLogFillterPBNumber(contact_ptr);
        SetAllLogFillter(MMICL_CALL_PB_NUMBER_MATCHED);
        s_cur_sel_log_list_type = MMICL_CALL_ALL;
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        CalllogPBData_SetListHandle(MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));

        if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"HandlePBDetailCallLogListWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_12079_112_2_18_2_11_53_111,(uint8*)"");
                return recode;
            }
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);

#if defined(MMI_ISTYLE_SUPPORT)        
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
        else     
#endif			
        {
            GUIAPICTRL_SetBothRect(list_handle,&list_both_rect);
            MMK_SetAtvCtrl(win_id, list_handle);
        }
        break;
   
    case MSG_LOSE_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);
        break;
    case MSG_FULL_PAINT:
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            break;
        }
        //�ر�idle״̬����ʾλ�ӵ绰�Ĵ����Լ�״̬��ͼ��
        MMIAPICC_ClearMissedCallNum();
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
        else 
#endif			
        {
            MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
            SetLogListItem();
        }

        break;

#ifdef MMI_PDA_SUPPORT
    /*add for istyle*/
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
        else
#endif			
        {
            GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
            uint16 cur_sel = 0;

            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            if(item_content_ptr == PNULL)
            {
               break;
            }
            cur_sel = item_content_ptr->item_index;
            if(item_content_ptr->item_content_index == CL_LIST_ITEM_ICON_INDEX)
            {
                HandleRingBackByType(CC_CALL_DEFAULT_CALL);
            }
        }       

        break;

     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
#endif


#ifdef MMI_MULTI_GREEN_KEY_SINGLE        
    case MSG_KEYDOWN_GREEN:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1])
        {
            if(  GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
            {
                HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
            }
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
#else
    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys =  MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {            
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);

            if(s_is_allow_sim_call[dual_sys])
            {
                if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
                {
                    HandleRingBack(sim_type, FALSE);
                }
            }
            s_is_allow_sim_call[dual_sys] = FALSE;
        }
        break;
#endif

#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
    case MSG_APP_VIDEO_TEL:        
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
#if 1//OPEN_VT_CHANGE
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
#else
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
#endif
        }
        break;
#endif
        

        
#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif

    case MSG_GET_FOCUS:

        CalllogPBData_SetListHandle(MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));
#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
#endif		
        if (PNULL == arraycall_info)
        {
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        }
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }

        GUIWIN_SetTitleIsVisible(win_id, FALSE, FALSE);
		MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);

        break;
#endif

    case MSG_CTL_TAB_SWITCH:
        //��˽���
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //��Ҫ������˽���
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
        else 
#endif			
        {
#ifdef CALL_TIMES_SUPPORT        
            MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_WIN_TAB, PNULL);
#else
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, (ADD_DATA)win_id);
#endif
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:        
	case MSG_APP_MENU:
	    //check is no log or not
        if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)>0)
        {
#ifdef MMI_GPRS_SUPPORT
            MMK_CreateWin((uint32 *)MMICL_PB_DETAIL_LOG_LIST_MENU_WIN_TAB, (ADD_DATA)win_id);
#endif
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //Set Back 
        MMK_CloseParentWin(info_ptr);    
        break;
        
    case MSG_CLOSE_WINDOW:

        SetAllLogFillter(MMICL_CALL_ALL);

        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
#endif
        break;
        
    default:
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)    
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
#endif
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;

}

/*****************************************************************************/
// 	Description : to handle the contact call log list menu window message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailLogListMenuWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    uint32       group_id = 0, menu_id = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(  GetCalllogPBData_MatchedItemCount()== 0)
        {
            GUIMENU_SetItemGrayed(MMICL_PB_DETAIL_CALLLOG_MENU_CTRL_ID,MENU_CL_PB_DETAIL_MEMU,
                    ID_CL_DELETE,TRUE);
        }

        MMK_SetAtvCtrl(win_id, MMICL_PB_DETAIL_CALLLOG_MENU_CTRL_ID);
    break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_CloseWin(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        GUIMENU_GetId(MMICL_PB_DETAIL_CALLLOG_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
            case ID_CL_DELETE:
                //ѯ���Ƿ�ɾ��!! 
               if(  GetCalllogPBData_MatchedItemCount()== 0)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_ENTRY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
                else
                {
                    //Enter new WinTAB
                    CalllogPBData_StartSelection();
                    MMK_CreateWin((uint32*)MMICL_PBDETAIL_SELECT_WIN_TAB,PNULL);
                }
                //Enter SelectWin
                MMK_CloseWin(win_id);
                break;
           default:
                break;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : to handle the call log list menu window message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllLogListPopMenuWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    uint32 group_id = 0, menu_id = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIPB_BCD_NUMBER_T  tele_num = {0};
            //get the phone number(bcd)
            GetPartyPhoneNum(&tele_num);
        if (CheckCurrentNumberInContact())
        {
            GUIMENU_SetItemVisible(MMICL_ALL_LIST_POPUP_MENU_CTRL_ID,MENU_ALL_LIST_POPUP_MEMU , ID_CL_LOG_LIST_SAVE, FALSE);
        }
        else
        {
            GUIMENU_SetItemVisible(MMICL_ALL_LIST_POPUP_MENU_CTRL_ID,MENU_ALL_LIST_POPUP_MEMU , ID_CL_CONTACT_VIEW, FALSE);
        }
#if defined(MMI_BLACKLIST_SUPPORT)            
            if (MMIAPISET_IsCCBlacklist(&tele_num,MN_CALL_TYPE_NORMAL))
            {
                GUIMENU_SetItemVisible(MMICL_ALL_LIST_POPUP_MENU_CTRL_ID,MENU_ALL_LIST_POPUP_MEMU , ID_CL_LOG_LIST_REFUSE_ADD, FALSE);
            }
            else
            {
                GUIMENU_SetItemVisible(MMICL_ALL_LIST_POPUP_MENU_CTRL_ID,MENU_ALL_LIST_POPUP_MEMU , ID_CL_LOG_LIST_REFUSE_REMOVE, FALSE);
            }
#endif
        MMK_SetAtvCtrl(win_id,MMICL_ALL_LIST_POPUP_MENU_CTRL_ID );
        }    
    break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_CloseWin(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        GUIMENU_GetId(MMICL_ALL_LIST_POPUP_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
            case ID_CL_EDIT_BEFORE_DIAL:
                {
                    uint8 tele_num[PHONE_NUM_MAX_LEN + 2];
                    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
                    //Get dispaynum
                    GetDisplayPhoneNum(tele_num,PHONE_NUM_MAX_LEN + 2 , &dual_sys);

                    MMIAPICL_OpenEditBeforeDialWin((uint16)strlen((char*)tele_num),tele_num);
                    MMK_CloseWin(win_id);
                }
                break;
            case ID_CL_CONTACT_VIEW:
                OpenNumberInContact();
                MMK_CloseWin(win_id);
                break;
            case ID_CL_ADD_TO_PB:
                HandleDistillPhoneNum();
                MMK_CloseWin(win_id);
                break;
#if defined(MMI_BLACKLIST_SUPPORT)
            case ID_CL_LOG_LIST_REFUSE_ADD:
                {
                    MMIPB_BCD_NUMBER_T  tele_num = {0};
                    //get the phone number(bcd)
                    GetPartyPhoneNum(&tele_num);
                    //AddNumToBlackList(tele_num);
                    MMK_CloseWin(win_id);
                    
                }
                 break;         
            case ID_CL_LOG_LIST_REFUSE_REMOVE:
                {
                    MMIPB_BCD_NUMBER_T  tele_num = {0};
                    //get the phone number(bcd)
                    GetPartyPhoneNum(&tele_num);
                    if (MMIAPISET_DeleteBlacklist(tele_num))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMPLETE);                    
                    }    
                    MMK_CloseWin(win_id);
                    
                }
                 break;                  
#endif                 
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
            case ID_CL_ADD_TO_EXIST_CONTACT:
                HandleAddNumToName();
                MMK_CloseWin(win_id);
                break;
#endif
            case ID_CL_LOG_LIST_DELETE:
                DeleteCurrentRecord();
                MMK_CloseWin(win_id);                
                break;
            
#ifdef VT_SUPPORT
           case ID_CL_DIAL_VIDEOCALL://@zhaohui add
#if 1//OPEN_VT_CHANGE
                HandleRingBack(CC_CALL_SIM_MAX, TRUE);
#else
                HandleRingBack(CC_CALL_SIM_MAX, TRUE, PNULL);
#endif
                MMK_CloseWin(win_id);
                break;
#endif

            case ID_CL_DIAL_AUDIOCALL:
                HandleRingBack(CC_CALL_SIM_MAX, FALSE);
                MMK_CloseWin(win_id);
                break;
                
#ifdef MMI_IP_CALL_SUPPORT
            case ID_CL_DIAL_IPCALL:
                HandleIpCall();
                MMK_CloseWin(win_id);
                break;
#endif

            case ID_CL_SEND_SMS:
                HandleAnswerSms();
                MMK_CloseWin(win_id);
                break;
                
            case ID_CL_SEND_MMS:
                HandleAnswerMms();
                MMK_CloseWin(win_id);
                break;
                
           default:
                break;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}


/*****************************************************************************/
// 	Description : SelectWin APIS
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void CalllogPBDataSelectWin_SetListHandle(MMI_HANDLE_T list_handle)
{
    CalllogPBData_SetListHandle(list_handle);
    return;
}
LOCAL uint16 CalllogPBDataSelectWin_GetMaxNum(void)
{
    return GetCalllogPBData_MatchedItemCount();
}
LOCAL void CalllogPBDataSelectWin_UpdateLogList(void)
{
	SetLogListItem();
	return;
}

LOCAL void CalllogPBDataSelectWin_SetSelectedbyIndex(uint16 index)
{
    if (CalllogPBData_GetIsSelectedByIndex(index))
    {
        CalllogPBData_SetSelectedByIndex(index, FALSE);
    }
    else
    {
        CalllogPBData_SetSelectedByIndex(index, TRUE);
    }
	return;
}

LOCAL void CalllogPBDataSelectWin_SetAllSelected(BOOLEAN isSelected,MMI_HANDLE_T list_handle)
{
    uint16 i =0;
    uint32 sel_item_index =0;
                
    if (isSelected == FALSE)
    {
        CalllogPBData_SetAllSelected(isSelected);
    }
    else
    {
        for (i = 0;i<GUILIST_GetTotalItemNum(list_handle);i++)
        {
            GUILIST_GetItemData(list_handle, i, &sel_item_index);    
            CalllogPBData_SetSelectedByIndex(sel_item_index,isSelected);
        }
    }
    return ;
}

LOCAL void CalllogPBDataSelectWin_ExitSelection(void)
{
    CalllogPBData_StopSelection();
    return;
}
/*****************************************************************************/

//
/*****************************************************************************/
// 	Description : to handle the contact call log list select item message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    GUIFORM_CHILD_WIDTH_T  child_width = {0};

    MMI_HANDLE_T list_handle =MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_LIST_CTRL_ID);
    MMI_HANDLE_T btn_handle = MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_TITLE_BUTTON_CTRL_ID);
    MMI_HANDLE_T titleform_handle = MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_TITLE_FORM_CTRL_ID);
    MMI_HANDLE_T form_handle = MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_FORM_CTRL_ID);
    MMI_HANDLE_T label_handle = MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_TITLE_LABEL_CTRL_ID);
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        CalllogPBDataSelectWin_SetListHandle(list_handle);
        
        GUILABEL_SetTextById(label_handle,TXT_PB_SELECT_ALL, FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, TRUE);
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = 40;
        GUIFORM_SetChildWidth(titleform_handle,btn_handle,&child_width);
        GUIFORM_PermitChildBorder(titleform_handle, FALSE);
        GUIBUTTON_PermitBorder(btn_handle, FALSE);

        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(form_handle,list_handle, &child_height);
        GUIFORM_IsSlide(form_handle, FALSE);
        GUIFORM_PermitChildBg(form_handle,FALSE);
        GUILIST_PermitBorder(list_handle,FALSE);
        
        GUILIST_SetMaxSelectedItem(list_handle,CalllogPBDataSelectWin_GetMaxNum());
        GUIBUTTON_SetSelect(MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_TITLE_BUTTON_CTRL_ID), FALSE);                
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);

        MMK_SetAtvCtrl(win_id, list_handle);
        break;
    case MSG_FULL_PAINT:
//        CalllogPBDataSelectWin_SetAllSelected(FALSE,list_handle);
        CalllogPBDataSelectWin_UpdateLogList();
        break;
    case MSG_GET_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        break;
    case MSG_LOSE_FOCUS:
         MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);
         break;
    case MSG_CTL_LIST_MARK_ITEM:
        {
            uint32 sel_item_index =0;
                
            GUILIST_GetItemData(list_handle, GUILIST_GetCurItemIndex(list_handle), &sel_item_index);
            CalllogPBDataSelectWin_SetSelectedbyIndex(sel_item_index);
            if(GUILIST_GetSelectedItemNum(list_handle) <  GUILIST_GetTotalItemNum(list_handle))           
            {
                if (GUIBUTTON_GetSelect(btn_handle))
                {
                    GUIBUTTON_SetSelect(btn_handle, FALSE);
					GUIBUTTON_Update(btn_handle);
                }
            }
            else
            {
                if (!GUIBUTTON_GetSelect(btn_handle))
                {
                    GUIBUTTON_SetSelect(btn_handle, TRUE);
					GUIBUTTON_Update(btn_handle);
                }
            }
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    if (GUILIST_GetSelectedItemNum(list_handle)<1)
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_ENTRY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    }
                    else
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteSelectedQueryWinMsg);                
                    }
                    break;
                case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                    break;
                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    MMK_CloseWin(win_id);   
                    break;
                case MMICL_PB_DETAIL_SELECT_TITLE_BUTTON_CTRL_ID:
                    CalllogPBDataSelectWin_SetAllSelected(GUIBUTTON_GetSelect(btn_handle),list_handle);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, 0,0);
                    break;
                default:
                    break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //Set Back 
        MMK_CloseWin(win_id);    
        break;
    case MSG_CLOSE_WINDOW:
        CalllogPBDataSelectWin_ExitSelection();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
/*****************************************************************************/
// 	Description : to handle pub edit win to edit number before call
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditBeforeDialWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    GUI_BG_T            edit_bg = {0};
    GUI_BORDER_T        edit_border = {0};
    
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set edit single
        GUIEDIT_SetStyle(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,GUIEDIT_STYLE_SINGLE);

        //set bg
        edit_bg.bg_type = GUI_BG_COLOR;
        edit_bg.color   = MMI_WHITE_COLOR;
        edit_bg.shape   = GUI_SHAPE_ROUNDED_RECT;
        GUIEDIT_SetBg(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,&edit_bg);

        //set border
        edit_border.type  = GUI_BORDER_ROUNDED;
        edit_border.width = 2;
        edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        GUIEDIT_SetBorder(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,&edit_border);

        //set font
        GUIEDIT_SetFontColor(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,MMI_BLACK_COLOR);

        //set left softkey
        GUIEDIT_SetSoftkey(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        
        MMK_SetAtvCtrl(win_id,MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T    edit_str = {0};
            uint8           temp_str[MMISMS_PBNUM_MAX_LEN + 1]  = {0};

            GUIEDIT_GetString(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,&edit_str);
            if (0 != edit_str.wstr_len)
            {
                if (MMIAPICOM_IsValidNumberStringEx(&edit_str))
                {
                    //convert string
                    MMI_WSTRNTOSTR((uint8 *)temp_str,MMISMS_PBNUM_MAX_LEN,
                        edit_str.wstr_ptr,edit_str.wstr_len,
                        edit_str.wstr_len);
                    MMIAPICC_MakeCall(
                                     MN_DUAL_SYS_MAX,
                                            temp_str,
                                            strlen((char *)temp_str),
                                            PNULL,
                                            PNULL,
                                            CC_CALL_SIM_MAX,
                                            CC_CALL_NORMAL_CALL,
                                            PNULL
                                            );
                   MMK_CloseWin( win_id );      
                       
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_INVALID_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : to handle CallTime pop list win
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimePopListWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E        recode  =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMICL_CALL_TIME_POP_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
        GUILIST_ITEM_T  item = {0};
        GUILIST_ITEM_DATA_T item_date = {0};


        GUILIST_SetMaxItem(ctrl_id, 2,FALSE);
        item.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        item.item_data_ptr = &item_date;
        item_date.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_date.item_content[0].item_data.text_id= TXT_ADDTOPB_NEW;
        GUILIST_AppendItem(ctrl_id,&item );
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        item_date.item_content[0].item_data.text_id = TXT_ADDTOPB_EXIST;
        GUILIST_AppendItem(ctrl_id,&item );
#endif
        MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            if(GUILIST_GetCurItemIndex(ctrl_id) == 0)
            {
                HandleDistillPhoneNum();
            }
            else
            {
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
                HandleAddNumToName();
#endif
            }
        }
        MMK_CloseWin( win_id );      

        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}                                            
#endif

/*****************************************************************************/
//  Description :open call number window
//  Global resource dependence : 
//  Author:Sam.hua
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenEditBeforeDialWin(uint8   tel_len,
                                           uint8*  tel_num_ptr
                                           )
{       

    if (0 != tel_len)   //No Number
    {
        wchar mail_wstr[ MMISMS_PBNUM_MAX_LEN + 1 ] = {0};
        MMI_STRNTOWSTR( mail_wstr, MMISMS_PBNUM_MAX_LEN, tel_num_ptr, tel_len, tel_len );

        if (!MMK_IsOpenWin(MMICL_EDIT_BEFORE_DIAL_WIN_ID))
        {
            MMK_CreatePubEditWin((uint32*)MMICL_EDIT_BEFORE_DIAL_WIN_TAB,PNULL);
        }
        else
        {   MMK_CloseWin(MMICL_EDIT_BEFORE_DIAL_WIN_ID);
            MMK_CreatePubEditWin((uint32*)MMICL_EDIT_BEFORE_DIAL_WIN_TAB,PNULL);
        }
        // set the edit box init param
        GUIEDIT_SetString(
            MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,
            mail_wstr,
            tel_len);
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SMS_BOX_EMPTY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}



#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : get display phone number of current log list 
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void GetCallLogInfoDetail(MN_DUAL_SYS_E *dual_sys_ptr,BOOLEAN *isVideoCall)
{

    uint32 array_index = 0;

    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_12898_112_2_18_2_11_55_112,(uint8*)"");
        return;
    }

    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    
    array_index = CL_GetArrayIndexofCurSelected();

    *dual_sys_ptr = arraycall_info->call_bcdinfo[array_index].dual_sys;
    *isVideoCall  = arraycall_info->call_bcdinfo[array_index].isvideocall;

    SCI_FREE(arraycall_info);
    
    return;
}
#endif
#ifdef SEARCH_SUPPORT
/*****************************************************************************/
// 	Description : ����������ͨ����¼ƥ�����"��"�ص�����
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void CL_OpenMatchedItem(uint32 user_data)
{    
    if (0 != s_match_call_info_ptr->match_num)
    {
        s_cur_sel_log_list_type = MMICL_CALL_MATCHED;
        s_mmicl_match_item_index = user_data;
#ifdef CALL_TIMES_SUPPORT
        if(s_match_call_info_ptr->call_info[s_mmicl_match_item_index].call_times>1)
        {
            MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_WIN_TAB, PNULL);
        }
        else
        {
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, PNULL);
        }
#else
        MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, PNULL);
#endif
    }          
}

/*****************************************************************************/
// 	Description : ����������ͨ����¼ƥ�������ʾ�ص�����
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void CL_SetMatchItemData(
                            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr,
                            uint32 user_data
                             )
{

    MMI_STRING_T name_number_str = {0};
    wchar name_number_wchar[PHONE_NUM_MAX_LEN+MAX_CALL_TIME_LEN + 1]  ={0};     

#ifndef MMI_MULTI_SIM_SYS_SINGLE    
    MMI_STRING_T sim_name_str  = {0};
    wchar sim_name_wstr[GUILIST_STRING_MAX_NUM + 1]={0};
#endif

    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_STRING_T time_str       ={0};
    wchar wstr_time_str[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_IMAGE_ID_T	image_id = IMAGE_NULL;
    
    MMICL_MATCH_TOTAL_INFO_T* info_ptr = PNULL;
    GUILIST_ITEM_DATA_T item_data = {0};
    uint32 cur_match_index = user_data;

    MMICL_CALL_BCDINFO_T *callinfo=PNULL;

    if (PNULL != s_match_call_info_ptr)
    {
        info_ptr = s_match_call_info_ptr;
    }
    else
    {
        return ;
    }

    if (0 == info_ptr->match_num)
    {        
        SCI_TRACE_LOW("warning! CL_SetMatchItemData 0 == info_ptr->match_num");
        return;
    }
    else
    {
        item_data.softkey_id[LEFT_BUTTON]      = TXT_NULL;
        item_data.softkey_id[MIDDLE_BUTTON]    = TXT_VIEW;
        item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;
    
        callinfo = &info_ptr->call_info[cur_match_index];
    //Append item
        //Init Text Contents     Name/Number, SimName, Date, Time
        SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));            
        name_number_str.wstr_ptr = name_number_wchar;
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
        SCI_MEMSET(sim_name_wstr,0,sizeof(sim_name_wstr));
        sim_name_str.wstr_ptr = sim_name_wstr;
        sim_name_str.wstr_len = 0;
#endif
        SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_wstr));            
        datetime_str.wstr_ptr = datetime_wstr;
        SCI_MEMSET(&time_str,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&wstr_time_str,0,sizeof(datetime_wstr));            
        time_str.wstr_ptr = wstr_time_str;

        //Add Image content
        image_id = GetCallTypeImageID(callinfo);
        item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data.image_id = image_id;


#ifndef MMI_MULTI_SIM_SYS_SINGLE    
        if (!callinfo->is_emergency_call)
        {
            MMIAPICL_GetSimNameContent(sim_name_wstr,GUILIST_STRING_MAX_NUM,callinfo->dual_sys);
        }
        sim_name_str.wstr_len = MMIAPICOM_Wstrlen(sim_name_wstr);
        
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
        item_data.item_content[CL_LIST_ITEM_SIM_INDEX].font_color_id=MMIAPISET_GetSimColorID(callinfo->dual_sys);
        item_data.item_content[CL_LIST_ITEM_SIM_INDEX].is_default =TRUE;
#endif                 
        
        item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
        item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_len =sim_name_str.wstr_len;
#else
        //Single don't need sim name
#endif

        //Add Contact name/or call number content
        if (GetNameOrNumberContent(name_number_wchar,PHONE_NUM_MAX_LEN+MAX_CALL_TIME_LEN,callinfo,TRUE,FALSE))
        {	
			name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
            item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
            item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
        }  

#ifdef CALL_TIMES_SUPPORT            
        if (callinfo->call_times>0)
        {
            //Add date and time content
            if (0<callinfo->call_start_time[callinfo->call_times -1])
            {
/*            
                GetDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,0,FALSE);
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
*/				
                //GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
                GetTimeAndDateContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,callinfo->call_times -1);
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
                
            }
        }    
#else
            //Add date and time content
        if (0<callinfo->call_start_time)
        {
/*            
                GetDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,0,FALSE);
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
*/				
                //GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
                GetTimeAndDateContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
        }
#endif            

    GUILIST_SetItemData(need_item_data_ptr->ctrl_id, &item_data, need_item_data_ptr->item_index);
    }
}

/*****************************************************************************/
// 	Description : ����������ͨ����¼�����ӿ�
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_LocalSearch(const MMI_STRING_T *key_word_str, MMISEARCH_DATA_T *match_data_ptr, uint32 max_num)
{
    uint8 i = 0;
    uint8 type_index = 0;
    MMI_STRING_T name_str = {0};
    wchar tele_num_wstr[MMIPB_MAX_STRING_LEN + 1] = {0};
    uint16 tele_num_wstr_len = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    wchar wstr_ptr[MMIPB_MAX_STRING_LEN + 1] = {0};
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    MMIPB_BCD_NUMBER_T bcd_number = {0};
    uint16 tele_len = 0;
    MMICL_MATCH_TOTAL_INFO_T* info_ptr = PNULL;
#ifdef MMI_PDA_SUPPORT
    GUIITEM_STYLE_E item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;				
#else
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
    GUIITEM_STYLE_E item_style = GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;     //GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;   
#else
    GUIITEM_STYLE_E item_style = GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT;
#endif
#endif

    if (PNULL != s_match_call_info_ptr)
    {
        SCI_MEMSET((void*)s_match_call_info_ptr, 0x00, sizeof(MMICL_MATCH_TOTAL_INFO_T));
    }
    else
    {
        s_match_call_info_ptr = (MMICL_MATCH_TOTAL_INFO_T*)SCI_ALLOCAZ(sizeof(MMICL_MATCH_TOTAL_INFO_T));
    }
    
    if (PNULL != s_match_call_info_ptr)
    {
        info_ptr = s_match_call_info_ptr;
    }
    else
    {
        return ;
    }

    if (PNULL == match_data_ptr || PNULL == match_data_ptr->match_item_ptr)
    {
        return ;
    }
    
#ifndef MMI_PDA_SUPPORT
    for(type_index = MMICL_CALL_MISSED; type_index < MMICL_CALL_MATCHED; type_index++)
#endif
    {
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            SCI_FREE(s_match_call_info_ptr);
            return ;
        }        

        MMICL_GetRecordInfo((MMICL_CALL_TYPE_E)type_index, arraycall_info);

        for ( i = 0; i < arraycall_info->record_num; i++ )
        {
            tele_len = MMICL_GenDispNumber(arraycall_info->call_bcdinfo[i].call_type,
                                                                        arraycall_info->call_bcdinfo[i].number_type,
                                                                        arraycall_info->call_bcdinfo[i].number_len,
                                                                        arraycall_info->call_bcdinfo[i].number, 
                                                                        tele_num,
                                                                        PHONE_NUM_MAX_LEN + 2);

            if (0 == tele_len || !arraycall_info->call_bcdinfo[i].is_num_present)
            {
                break;
            }

            bcd_number.number_len = arraycall_info->call_bcdinfo[i].number_len;
            bcd_number.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(arraycall_info->call_bcdinfo[i].number_type,
                arraycall_info->call_bcdinfo[i].number_plan);
            MMI_MEMCPY(bcd_number.number,MMIPB_BCD_NUMBER_MAX_LEN,
                arraycall_info->call_bcdinfo[i].number,MMIPB_BCD_NUMBER_MAX_LEN,
                bcd_number.number_len);

            //search number
            SCI_MEMSET(tele_num_wstr, 0, sizeof(tele_num_wstr));
            tele_num_wstr_len = MIN(tele_len, MMIPB_MAX_STRING_LEN);

            MMI_STRNTOWSTR(tele_num_wstr,
                                        MMIPB_MAX_STRING_LEN,
                                        (uint8 *)tele_num,
                                        tele_len,
                                        tele_num_wstr_len);

            //search from pb
            SCI_MEMSET(wstr_ptr, 0, sizeof(wstr_ptr));

            name_str.wstr_ptr = wstr_ptr;
            MMIAPICL_GetNameByClInfo(arraycall_info->call_bcdinfo[i], &name_str, FALSE);

            if ((0 != name_str.wstr_len)
            && MMIAPICOM_WstrnstrExt(name_str.wstr_ptr, name_str.wstr_len, key_word_str->wstr_ptr, key_word_str->wstr_len, FALSE))
            {
                //��ӵ�ƥ���б�
                SCI_MEMCPY(&info_ptr->call_info[info_ptr->match_num],&arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T));               
                info_ptr->call_info[info_ptr->match_num].array_index = i;
                info_ptr->match_num++;
                match_data_ptr->match_item_ptr[match_data_ptr->match_num].match_item_type = MMISEARCH_TYPE_CL;
                match_data_ptr->match_item_ptr[match_data_ptr->match_num].match_item_style = item_style;
                match_data_ptr->match_item_ptr[match_data_ptr->match_num].user_data = match_data_ptr->match_num;//���ڱ���ü�¼��ͨ����¼�е�����ֵ
                match_data_ptr->match_item_ptr[match_data_ptr->match_num].open_pfunc = CL_OpenMatchedItem;
                match_data_ptr->match_item_ptr[match_data_ptr->match_num].set_data_pfunc = CL_SetMatchItemData;

                match_data_ptr->match_num++;
                
                if (match_data_ptr->match_num >= max_num)
                {
                    SCI_FREE(arraycall_info);
                    return;
                }
            }
        }

        SCI_FREE(arraycall_info);
    }
 
    return ;
}

/*****************************************************************************/
// 	Description : ���������˳���ͨ����¼ģ���ͷ�ģ���ڲ���Դ
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_FreeLocalSearchResource(void)
{    
    if (PNULL != s_match_call_info_ptr)
    {
        SCI_FREE(s_match_call_info_ptr);
    }
}
#endif
#ifdef MMI_PDA_SUPPORT 
/*****************************************************************************/
//  Description : ����б�ؼ��б�ѡ�е�item��Ŀ
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: ��SMS, MMS������ģ����á�
/*****************************************************************************/

PUBLIC uint16 MMICL_GetSelectedItemCount(void)   //RETURN: ��ѡ�е�ITEM������
{
    return s_call_log_all_select_info.sel_count;
}

/*****************************************************************************/
//  Description : if the selected item is valid
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: ��SMS, MMS������ģ����á�
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_SelectedListIsValid(uint16 index)
{
    return s_call_log_all_select_info.sel_item[index].is_valid;
}

/*****************************************************************************/
// 	Description : get bcd phone number of selected loglist item
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: ��SMS, MMS������ģ����á�
/*****************************************************************************/
PUBLIC void MMICL_GetSelectPhonenum(
                            MMIPB_BCD_NUMBER_T*   bcd_number ,//out
                            uint16 sel_item_index
                            )
{
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    uint32 array_index = 0;
	
	if (PNULL == bcd_number)
	{
	    return;
	}
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetPartyPhoneNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_13268_112_2_18_2_11_55_113,(uint8*)"");
        return;
    }

    GUILIST_GetItemData(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type), sel_item_index, &array_index);
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        sel_item_index = s_match_call_info_ptr->call_info[sel_item_index].array_index;
    }
    else
    {
        sel_item_index = array_index;
    }
    
    if (arraycall_info->call_bcdinfo[sel_item_index].is_num_present)//�绰�������
    {
        //get number
        bcd_number->number_len = arraycall_info->call_bcdinfo[sel_item_index].number_len;
        bcd_number->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(arraycall_info->call_bcdinfo[sel_item_index].number_type,
        arraycall_info->call_bcdinfo[sel_item_index].number_plan);
        MMI_MEMCPY(bcd_number->number,MMIPB_BCD_NUMBER_MAX_LEN,
            arraycall_info->call_bcdinfo[sel_item_index].number,MMIPB_BCD_NUMBER_MAX_LEN,
            bcd_number->number_len);
    }
    
    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
        arraycall_info = PNULL;
    }
}

/*****************************************************************************/
// 	Description : get bcd phone number of selected loglist item
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: ��SMS, MMS������ģ����á�
/*****************************************************************************/
PUBLIC void MMICL_GetSelectPhoneName(
                            MMI_STRING_T*   call_name, //out
                            uint16 sel_item_index,
                            uint16 max_name_len,
                            MMIPB_BCD_NUMBER_T    *bcd_number
                            )
{
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    uint32 array_index = 0;
	
	if (PNULL == call_name || PNULL == bcd_number)
	{
	    return;
	}
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetPartyPhoneNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_13324_112_2_18_2_11_55_114,(uint8*)"");
        return;
    }

    //read logs of calls from NV
    GUILIST_GetItemData(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type), sel_item_index, &array_index);
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        sel_item_index = s_match_call_info_ptr->call_info[sel_item_index].array_index;
    }
    else
    {
        sel_item_index = array_index;
    }

#if defined(CL_SAVE_NAME_SUPPORT)
    if (arraycall_info->call_bcdinfo[sel_item_index].name.is_use_nv_name)
    {
        call_name->wstr_len = arraycall_info->call_bcdinfo[sel_item_index].name.wstr_len;
        MMI_WSTRNCPY(call_name->wstr_ptr, 
                                    MMICL_NAME_MAX_LEN, 
                                    arraycall_info->call_bcdinfo[sel_item_index].name.wstr, 
                                    MMICL_NAME_MAX_LEN, 
                                    call_name->wstr_len);
    
        SCI_FREE(arraycall_info);
        arraycall_info = PNULL;
    }
    else
#endif
    {
        MMIAPIPB_GetNameByNumber(bcd_number,
								 call_name,
								 max_name_len,
								 FALSE);
        //call_name->wstr_len = 0;
        SCI_FREE(arraycall_info);
        arraycall_info = PNULL;
    }
}
#endif

/*****************************************************************************/
//  Description : Generate to the string number
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMICL_GenDispNumber(
                                        MMICL_CALL_TYPE_E call_type,
                                        MN_NUMBER_TYPE_E  number_type,
                                        uint8             party_len,
                                        uint8             *party_num,
                                        uint8             *tele_num,
                                        uint16            max_tele_len//!!!!!!!!!����ʾ�ĺ������󳤶� + 2(/0 �ַ�����������)
                                     )
{
    uint16 tele_len = 0;

    if (MMICL_CALL_MISSED == call_type
        || MMICL_CALL_RECEIVED == call_type
#if defined(MMI_BLACKLIST_SUPPORT)
        || MMICL_CALL_REFUSED == call_type
#endif
        )
    {
        tele_len = MMIAPICOM_GenNetDispNumber(number_type,
                                                                    party_len,
                                                                    party_num, 
                                                                    tele_num,
                                                                    max_tele_len);
    }
    else
    {
        tele_len = MMIAPICOM_GenDispNumber(number_type,
                                                                    party_len,
                                                                    party_num, 
                                                                    tele_num,
                                                                    max_tele_len);
    }

    return tele_len;
}

#ifdef CALL_TIMES_SUPPORT
/*****************************************************************************/
// 	Description : to delete one call times list
//	Global resource dependence : 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_DeleteOneCallTimesItem(
                                        MMICL_CALL_TYPE_E    call_type,  //call ����
                                        uint16 select_index    
                                        )
{
    uint8                       i                     = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info        = PNULL;
    uint16                      real_call_times_index = 0;
    uint16                      array_index           = 0;
    uint8                       pos                   = 0;

    if(call_type >= MMICL_CALL_MATCHED)
    {
        //SCI_TRACE_LOW:"MMICL_RecordCallInfo: the type of call is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_535_112_2_18_2_11_16_1,(uint8*)"");
        return;
    }

    //init arraycall_info
    arraycall_info = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"MMICL_DeleteAppointedCallInfo call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_686_112_2_18_2_11_16_4,(uint8*)"");
        return;
    }
       
    array_index = CL_GetArrayIndexofCurSelected();
    
    //get the value of arraycall_info from nv
    MMICL_ReadNV(call_type, arraycall_info);

    //Convert Index to Array Index
    pos = arraycall_info->call_bcdinfo[array_index].array_index;

    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    //get the value of arraycall_info from nv
    MMICL_ReadNV(MMICL_CALL_ALL, arraycall_info);
    
#ifdef MMI_LDN_SUPPORT
    //delete ldn
    //if (arraycall_info->call_bcdinfo[pos].is_sim_record)
    {
        MMICL_CALL_INFO_T call_info = {0};
        
        call_info.dual_sys = arraycall_info->call_bcdinfo[pos].dual_sys;
        call_info.call_num.is_num_present = arraycall_info->call_bcdinfo[pos].is_num_present;
        call_info.call_num.number.number_plan = arraycall_info->call_bcdinfo[pos].number_plan;
        call_info.call_num.number.number_type = arraycall_info->call_bcdinfo[pos].number_type;
        call_info.call_num.number.num_len = MIN(arraycall_info->call_bcdinfo[pos].number_len, MN_MAX_ADDR_BCD_LEN);
        SCI_MEMCPY(&call_info.call_num.number.party_num, arraycall_info->call_bcdinfo[pos].number, call_info.call_num.number.num_len);
        
        if (arraycall_info->call_bcdinfo[pos].is_sim_record && 0 < arraycall_info->sim_record_num)
        {
            arraycall_info->sim_record_num--;
        }
        
        MMICL_DeleteCallInfo(s_cur_sel_log_list_type, call_info.dual_sys, &call_info);
    }
#endif
    
    //ɾ��ָ��λ�õĵ绰��¼
    real_call_times_index  = arraycall_info->call_bcdinfo[pos].call_times - select_index - 1;
    arraycall_info->call_bcdinfo[pos].call_start_time[real_call_times_index] = 0;
    arraycall_info->call_bcdinfo[pos].call_duration_time[real_call_times_index] =0;
    arraycall_info->call_bcdinfo[pos].call_times--;
    
    //��������0��ɾ������ϵ�˵ļ�¼,  ����
    if(0 == arraycall_info->call_bcdinfo[pos].call_times)
    {
        DeleteCurrentRecord();
        MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
        MMK_CloseWin(MMICL_CALL_TIMES_WIN_ID);
        SCI_FREE(arraycall_info);
        return;
    }

    //�������е�ʱ����ǰ��һλ
    for(i=0; i < select_index; i++)
    {
        arraycall_info->call_bcdinfo[pos].call_start_time[real_call_times_index + i] =
            arraycall_info->call_bcdinfo[pos].call_start_time[real_call_times_index+i+1];
        arraycall_info->call_bcdinfo[pos].call_duration_time[real_call_times_index + i]= 
            arraycall_info->call_bcdinfo[pos].call_duration_time[real_call_times_index+i+1];
    }

    //����ʱ����0
    arraycall_info->call_bcdinfo[pos].call_start_time[arraycall_info->call_bcdinfo[pos].call_times]    = 0;
    arraycall_info->call_bcdinfo[pos].call_duration_time[arraycall_info->call_bcdinfo[pos].call_times] = 0;
    
    MMICL_WriteNV(MMICL_CALL_ALL, arraycall_info);
       
    MMICL_UpdateEventInd();   
    
    
    SCI_FREE(arraycall_info);
}
#endif
#ifdef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : Set log list item for mark or unmark
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetCurrentSelect(uint16 index,BOOLEAN isSelected)
{
    uint8 i = 0;
    uint8 mark_count = 0;
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);

    if (index >= GUILIST_GetTotalItemNum(list_id))
    {
        return;
    }

    s_select_info.log_selected_index[index] = isSelected;

    //��ȡ�����ʱ�������ǰ�ؼ�������Ԫ�ض�ȡ���˱�ǣ���Ҫ��is_select��״̬��ΪFALSE
    if (!isSelected)
    {
        for(i = 0; i < GUILIST_GetTotalItemNum(list_id); i++)
        {
            if(s_select_info.log_selected_index[i])
                
                mark_count++;
        }
        
        if (0 == mark_count)
        {
            s_select_info.is_select = FALSE;
        }
    }
}

/*****************************************************************************/
// 	Description : Set all log list item for mark or unmark
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetAllSelect(BOOLEAN isSelected)
{
    int i = 0;
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);

    for(i = 0; i < GUILIST_GetTotalItemNum(list_id); i++)
    {
        s_select_info.log_selected_index[i] = isSelected;
    }
}

/*****************************************************************************/
// 	Description : Exit mark for Set all log list item
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void ExitSelect(void)
{
    s_select_info.is_select = FALSE;

    SCI_MEMSET(s_select_info.log_selected_index, 0, sizeof(s_select_info.log_selected_index));
}

/*****************************************************************************/
// 	Description : to delete marked call log record item
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void DeleteRecord(void)
{
    uint16                      i                       = 0;
    uint16                      j                       = 0;
    uint16                      pos                     = 0;
    uint16                      record_count            = 0;
    uint16                      record_count_copy       = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info          = PNULL;
    MMICL_CALL_ARRAY_BCDINFO_T* all_arraycall_info      = PNULL;
    MMICL_CALL_ARRAY_BCDINFO_T* all_arraycall_info_copy = PNULL;
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
    
    //init arraycall_info
    arraycall_info = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    all_arraycall_info = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    all_arraycall_info_copy = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info || PNULL == all_arraycall_info || PNULL == all_arraycall_info_copy)
    {
        //SCI_TRACE_LOW:"MMICL_DeleteAppointedCallInfo call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_686_112_2_18_2_11_16_4,(uint8*)"");

        if (arraycall_info != PNULL)
        {
            SCI_FREE(arraycall_info);
        }

        if (all_arraycall_info != PNULL)
        {
            SCI_FREE(all_arraycall_info);
        }

        if (all_arraycall_info_copy != PNULL)
        {
            SCI_FREE(all_arraycall_info_copy);
        }
        return;
    }
    
    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(all_arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(all_arraycall_info_copy, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    //get the value of arraycall_info from nv
    MMICL_ReadNV(s_cur_sel_log_list_type, arraycall_info);
    MMICL_ReadNV(MMICL_CALL_ALL, all_arraycall_info);
    
    record_count_copy = all_arraycall_info->record_num;
    record_count = all_arraycall_info->record_num;

    for(i = 0; i < GUILIST_GetTotalItemNum(list_id); i++)
    {
        if(s_select_info.log_selected_index[i])

        {
            pos = arraycall_info->call_bcdinfo[i].array_index;
 
            //��Ҫɾ��λ�õĵ绰��¼����
            SCI_MEMSET(&(all_arraycall_info->call_bcdinfo[pos]),0,sizeof(MMICL_CALL_BCDINFO_T));
#ifdef MMI_LDN_SUPPORT
            //delete ldn
            //if (arraycall_info->call_bcdinfo[pos].is_sim_record)
            {
                MMICL_CALL_INFO_T call_info = {0};
                
                call_info.dual_sys = arraycall_info->call_bcdinfo[pos].dual_sys;
                call_info.call_num.is_num_present = arraycall_info->call_bcdinfo[pos].is_num_present;
                call_info.call_num.number.number_plan = arraycall_info->call_bcdinfo[pos].number_plan;
                call_info.call_num.number.number_type = arraycall_info->call_bcdinfo[pos].number_type;
                call_info.call_num.number.num_len = MIN(arraycall_info->call_bcdinfo[pos].number_len, MN_MAX_ADDR_BCD_LEN);
                SCI_MEMCPY(&call_info.call_num.number.party_num, arraycall_info->call_bcdinfo[pos].number, call_info.call_num.number.num_len);
                
                if (arraycall_info->call_bcdinfo[pos].is_sim_record && 0 < arraycall_info->sim_record_num)
                {
                    arraycall_info->sim_record_num--;
                }
                
                MMICL_DeleteCallInfo(s_cur_sel_log_list_type, call_info.dual_sys, &call_info);
            }
#endif
 
            record_count--;
        }
    }
    
    //ɾ����ǵ�Ԫ�أ����������Ԫ��ȥ��
    for (i = 0, j = 0; j < record_count_copy; j++)
    {    
        if (all_arraycall_info->call_bcdinfo[j].is_num_present)
        {
            SCI_MEMCPY(&all_arraycall_info_copy->call_bcdinfo[i++], &all_arraycall_info->call_bcdinfo[j],sizeof(MMICL_CALL_BCDINFO_T));
        }
    }

    all_arraycall_info_copy->record_num = record_count;
    
    MMICL_WriteNV(MMICL_CALL_ALL, all_arraycall_info_copy);
    
    MMICL_UpdateEventInd();   

    s_select_info.is_select = FALSE;
    
    SCI_FREE(arraycall_info);
    SCI_FREE(all_arraycall_info);
    SCI_FREE(all_arraycall_info_copy);
}

/*****************************************************************************/
// 	Description : set menu grayed
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_LogListMenuGrayed(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
    if (s_select_info.is_select)
    {
        if (GetTotalItemIsMark())
        {
            GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARK_LIST,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCEL_MARK_LIST,FALSE);
            GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARKALL_LIST,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCLE_MARKALL_LIST,FALSE);
        }
        else
        {
            if (s_select_info.log_selected_index[GUILIST_GetCurItemIndex(list_id)])
            {
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARK_LIST,TRUE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCEL_MARK_LIST,FALSE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARKALL_LIST,FALSE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCLE_MARKALL_LIST,FALSE);
            }
            else
            {
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARK_LIST,FALSE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCEL_MARK_LIST,TRUE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARKALL_LIST,FALSE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCLE_MARKALL_LIST,FALSE);
            }

        }
    }
    else
    {
        GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARK_LIST,FALSE);
        GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCEL_MARK_LIST,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARKALL_LIST,FALSE);
        GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCLE_MARKALL_LIST,TRUE);
    }

}

/*****************************************************************************/
// 	Description : ALL item is marked
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTotalItemIsMark(void)
{
    int i = 0;
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);

    for(i = 0; i < GUILIST_GetTotalItemNum(list_id); i++)
    {
         if (!s_select_info.log_selected_index[i])
         {
             return FALSE;
         }
    }

    return TRUE;
}
#endif
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
LOCAL MMI_RESULT_E HandleChildLogListDeleteSelectedQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
        //
        DeleteAllCallLog(s_cur_sel_log_list_type);
        MMK_CloseWin(win_id);
        MMK_CloseWin(MMICL_CHILD_SELECT_WIN_ID);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : start multiselect 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void BeginMultiSelecting(void)
{
    s_is_multiselecting = TRUE;
    SCI_MEMSET(s_log_selected_index,0,sizeof(s_log_selected_index));
    return;
}
/*****************************************************************************/
// 	Description : exit multiselect
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void ExitMultiSelecting(void)
{
    s_is_multiselecting = FALSE;
    SCI_MEMSET(s_log_selected_index,0,sizeof(s_log_selected_index));
    return;
}
/*****************************************************************************/
// 	Description : to is multiselecting
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN isMultiSelecting(void)
{
    return s_is_multiselecting ;
}

/*****************************************************************************/
// 	Description : check current index is selecting or not
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN isCurrentSelected(uint16 index)
{
    
    return s_log_selected_index[index];
}
/*****************************************************************************/
// 	Description : Set current index 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetCurrentSelect(uint16 index,BOOLEAN isSelected)
{
    s_log_selected_index[index]= isSelected;
    
    return;
}

/*****************************************************************************/
// 	Description : Set log list for multiselecting  
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetLogListItemForSelect(MMI_CTRL_ID_T ctrl_id)
{
    uint16 sel_item_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2626_112_2_18_2_11_32_54,(uint8*)"");
        return;
    }
    
    //get the current selected item index
    //���绰��¼�б��Ѿ���ʱ������ˢ���б���Ҫ���¶�λ��ǰ��ѡ�е�item!
    
    sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
    
    //read logs of calls from NV
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    
    //load log list
    GUILIST_SetMaxItem(ctrl_id, MMICL_RECORD_MAX_NUM, FALSE );//max item 20

    AppendLogListItem(ctrl_id, s_cur_sel_log_list_type, arraycall_info);
    
    GUILIST_SetCurItemIndex(ctrl_id, sel_item_index);
    
    SCI_FREE(arraycall_info);
}
/*****************************************************************************/
// 	Description : multiselecting  win handler
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChildCallLogSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_CHILD_SELECT_LIST_CTRL_ID;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        { 
            BeginMultiSelecting();
            GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
            GUILIST_SetMaxSelectedItem(list_ctrl_id,MMICL_RECORD_MAX_NUM);
            MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        SetLogListItemForSelect(list_ctrl_id);
        break;

    case MSG_GET_FOCUS:
        break;
    case MSG_CTL_LIST_MARK_ITEM:
        {
            uint32 sel_item_index =0;
            BOOLEAN is_selected = FALSE;

            GUILIST_GetItemData(list_ctrl_id, GUILIST_GetCurItemIndex(list_ctrl_id), &sel_item_index);
            is_selected = isCurrentSelected(sel_item_index);
            is_selected = !is_selected;
            SetCurrentSelect(sel_item_index, is_selected);
            GUILIST_SetSelectedItem(list_ctrl_id,sel_item_index,is_selected);
            break;
        }

    case MSG_CTL_OK:
    case MSG_APP_OK:        
    case MSG_APP_MENU:
        //Delete selected record
        if (GUILIST_GetSelectedItemNum(list_ctrl_id)<1)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_ENTRY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,HandleChildLogListDeleteSelectedQueryWinMsg);                
        }
        
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        ExitMultiSelecting();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
LOCAL void SetItemStyle(MMI_CTRL_ID_T ctrl_id, uint16 index)
{
    THEMELIST_ITEM_STYLE_T* itemstyle_ptr = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT);
    if (s_itemstyle_ptr)
    {
        SCI_MEMCPY(s_itemstyle_ptr, itemstyle_ptr, sizeof(*s_itemstyle_ptr));
        s_itemstyle_ptr->content[1].rect.right = MMI_MAINSCREEN_WIDTH/4*3 - MMI_LIST_ICON_LEFT_SPACE;
		s_itemstyle_ptr->content[1].rect_focus.right = MMI_MAINSCREEN_WIDTH/4*3 - MMI_LIST_ICON_LEFT_SPACE;
        s_itemstyle_ptr->content[2].rect.left = MMI_MAINSCREEN_WIDTH/4*3 + MMI_LIST_ICON_LEFT_SPACE * 2;
		s_itemstyle_ptr->content[2].rect_focus.left = MMI_MAINSCREEN_WIDTH/4*3 + MMI_LIST_ICON_LEFT_SPACE * 2;
        GUILIST_SetItemStyleEx(ctrl_id, index, s_itemstyle_ptr);
    }
}
#endif

#ifdef MMI_LDN_SUPPORT
/*****************************************************************************/
// 	Description : Is CL LDN Record
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSimRecord(MMICL_CALL_ARRAY_BCDINFO_T *callinfo)
{
    uint16 array_index =0;
    
    if (callinfo == PNULL)
    {
        return FALSE;
    }
    
    array_index = CL_GetArrayIndexofCurSelected();
    
    if (callinfo->call_bcdinfo[array_index].is_sim_record)
    {
        return TRUE;
    }
    
    return FALSE;
    
}
#endif
