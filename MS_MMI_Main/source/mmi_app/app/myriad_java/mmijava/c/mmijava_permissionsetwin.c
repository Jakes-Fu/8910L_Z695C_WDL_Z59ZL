/****************************************************************************
** File Name:      mmijava_permissionsetwin.c
** Author:
** Date:           2012/8/15
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.
** Description:    This file is used to describe functions of permission set
****************************************************************************/

#include "jbed_exported_defines.h"
#include "mmi_app_myriad_java_trc.h"
#include "mmijava_id.h"
#include "java_mmi_text.h"
//#include <os_api.h>
#include "window_parse.h"
//#include <cpl_tpal.h>
//#include <jbed_jbni.h>
#include "guiform.h"
//#include <guilistbox.h>
#include "guidropdownlist.h"
#include "mmipub.h"
#include "mmiconnection_export.h"
#include "mmidisplay_data.h"
//#include <mmijava_menutable.h>
//#include <mmifmm_export.h>
#include <mmijava_common.h>
#include "guilabel.h"
//#ifdef WIFI_SUPPORT
//#include <mmiwifi_export.h>
//#endif
//#include <jbed_trace.h>
#include "mmijava_permissionsetwin.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                   *
 **---------------------------------------------------------------------------*/
typedef struct permit_info_map_tag
{
    BOOLEAN be_check;        //wether be modifyed,
    uint8   cur_permit_item; //permission item,start from 0
    uint8   permit_item_num; //number of perssion
    uint8 * permit_array;    //
}PERMIT_INFO_MAP_T;


//Use the following macro to jurge if we have the specified permission item.
#if defined(PDAP_PIM_SUPPORT) || defined(PDAP_FC_SUPPORT)
#define WRITEUSERDATA_IDX         STATIC_com_jbed_security_GenData_WRITEUSERDATA_IDX
#define READUSERDATA_IDX          STATIC_com_jbed_security_GenData_READUSERDATA_IDX
#endif
#define NETACCESS_IDX             STATIC_com_jbed_security_GenData_NETACCESS_IDX
#define MULTIMEDIARECORDING_IDX   STATIC_com_jbed_security_GenData_MULTIMEDIARECORDING_IDX
#define MESSAGING_IDX             STATIC_com_jbed_security_GenData_MESSAGING_IDX
#ifdef JBED_MIDP_21
#define LOWLEVELNETACCESS_IDX     STATIC_com_jbed_security_GenData_LOWLEVELNETACCESS_IDX
#endif
#define LOCALCONNECTIVITY_IDX     STATIC_com_jbed_security_GenData_LOCALCONNECTIVITY_IDX
#define APPAUTOINVOCATION_IDX     STATIC_com_jbed_security_GenData_APPAUTOINVOCATION_IDX

/* cosider changing it to a local variable and abstract an API */
LOCAL int32                 g_permission_num = 0;
LOCAL PERMIT_INFO_MAP_T*    permit_info_map = NULL;

/**************************************************************************************************
  * Description: UI of permission setting
  * Input Paras:
  * Output Paras:
  * note :
  *************************************************************************************************/
LOCAL void MMIJava_setting_SetPermitInfoFormParam(MMI_WIN_ID_T win_id, int event_id);

/**************************************************************************************************
  * Description: win_func of JBED_CONNECT_PERMISSION_SETTING_WIN_TAB
  * Input Paras:
  * Output Paras:
  * note :
  *************************************************************************************************/
LOCAL MMI_RESULT_E MMIJava_setting_HandlePermissionSetWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param);

LOCAL MMI_RESULT_E HandlePermissionSelectWin(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param);

/**************************************************************************************************
 ** window table of network peimission settings
 **************************************************************************************************/
WINDOW_TABLE( JBED_CONNECT_PERMISSION_SETTING_WIN_TAB ) =
{
    WIN_FUNC((uint32)MMIJava_setting_HandlePermissionSetWinMsg),
    WIN_ID(JBED_CONNECT_PERMISSION_SETTING_WIN_ID),

    //FORM 父控件，填充屏幕状态栏和SOFTKEY之外的区域
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID),
#ifdef APPAUTOINVOCATION_IDX
        //程序自动启动
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_APP_AUTOPLAY_FORM_CTRL_ID,MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_APP_AUTOPLAY_TITLE_LABEL_CTRL_ID,MMIJAVA_APP_AUTOPLAY_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_APP_AUTOPLAY_TITLE_LABEL_EDIT_ID,MMIJAVA_APP_AUTOPLAY_FORM_CTRL_ID),
#endif
#ifdef LOCALCONNECTIVITY_IDX
        //本地连接访问
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_LOCAL_CONNECT_FORM_CTRL_ID,MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_LOCAL_CONNECT_TITLE_LABEL_CTRL_ID,MMIJAVA_LOCAL_CONNECT_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_LOCAL_CONNECT_TITLE_LABEL_EDIT_ID,MMIJAVA_LOCAL_CONNECT_FORM_CTRL_ID),
#endif
#ifdef LOWLEVELNETACCESS_IDX
        //低层次的网络访问
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_LOW_LEVEL_NET_FORM_CTRL_ID,MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_LOW_LEVEL_NET_TITLE_LABEL_CTRL_ID,MMIJAVA_LOW_LEVEL_NET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_LOW_LEVEL_NET_TITLE_LABEL_EDIT_ID,MMIJAVA_LOW_LEVEL_NET_FORM_CTRL_ID),
#endif
#ifdef MESSAGING_IDX
        //信息
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_MESSAGE_FORM_CTRL_ID,MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_MESSAGE_TITLE_LABEL_CTRL_ID,MMIJAVA_MESSAGE_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_MESSAGE_TITLE_LABEL_EDIT_ID,MMIJAVA_MESSAGE_FORM_CTRL_ID),
#endif
#ifdef MULTIMEDIARECORDING_IDX
        //多媒体
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_MULTIMEDIA_FORM_CTRL_ID,MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_MULTIMEDIA_TITLE_LABEL_CTRL_ID,MMIJAVA_MULTIMEDIA_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_MULTIMEDIA_TITLE_LABEL_EDIT_ID,MMIJAVA_MULTIMEDIA_FORM_CTRL_ID),
#endif
#ifdef NETACCESS_IDX
        //网络接入
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_NETWORK_LINK_FORM_CTRL_ID,MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_NETWORK_LINK_TITLE_LABEL_CTRL_ID,MMIJAVA_NETWORK_LINK_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_NETWORK_LINK_TITLE_LABEL_EDIT_ID,MMIJAVA_NETWORK_LINK_FORM_CTRL_ID),
#endif
#ifdef READUSERDATA_IDX
        //读用户信息
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_READ_USERINFO_FORM_CTRL_ID,MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_READ_USERINFO_TITLE_LABEL_CTRL_ID,MMIJAVA_READ_USERINFO_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_READ_USERINFO_TITLE_LABEL_EDIT_ID,MMIJAVA_READ_USERINFO_FORM_CTRL_ID),
#endif
#ifdef WRITEUSERDATA_IDX
        //写用户信息
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_WRITE_USERINFO_FORM_CTRL_ID,MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_WRITE_USERINFO_TITLE_LABEL_CTRL_ID,MMIJAVA_WRITE_USERINFO_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_WRITE_USERINFO_TITLE_LABEL_EDIT_ID,MMIJAVA_WRITE_USERINFO_FORM_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE( JBED_PERMISSION_RADIO_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePermissionSelectWin ),
    WIN_ID( JBED_PERMISSION_RADIO_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJAVA_PM_RADIO_CRTL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

/**************************************************************************************************
  * Description: UI of permission setting
  * Input Paras:
  * Output Paras:
  * note :
  *************************************************************************************************/
LOCAL void MMIJava_setting_SetPermitInfoFormParam(MMI_WIN_ID_T win_id,int event_id)
{
    uint8  i                            = 0;
    uint8  j                            = 0;
    uint16 form_space                   = 0;
    MMI_STRING_T* dropdownlist_item_ptr = PNULL;
    MMI_HANDLE_T form_handle            = MMK_GetCtrlHandleByWin(win_id, MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID);
    MMI_HANDLE_T child_handle           = 0;
    MMI_STRING_T name_ptr               = {0};
    MMIEvent * cur_mmi_event            = NULL;
    NAMSPermssions_t * permissions      = NULL;//(NAMSPermssions_t *)(cur_mmi_event->data);

    MMI_CTRL_ID_T form_id[] =
    {
#ifdef APPAUTOINVOCATION_IDX
        MMIJAVA_APP_AUTOPLAY_FORM_CTRL_ID,          //程序自动启动
#endif
#ifdef LOCALCONNECTIVITY_IDX
        MMIJAVA_LOCAL_CONNECT_FORM_CTRL_ID,         //本地连接
#endif
#ifdef LOWLEVELNETACCESS_IDX
        MMIJAVA_LOW_LEVEL_NET_FORM_CTRL_ID,         //低层次网络访问
#endif
#ifdef MESSAGING_IDX
        MMIJAVA_MESSAGE_FORM_CTRL_ID,               //信息
#endif
#ifdef MULTIMEDIARECORDING_IDX
        MMIJAVA_MULTIMEDIA_FORM_CTRL_ID,            //多媒体
#endif
#ifdef NETACCESS_IDX
        MMIJAVA_NETWORK_LINK_FORM_CTRL_ID,          //网络连接
#endif
#ifdef READUSERDATA_IDX
        MMIJAVA_READ_USERINFO_FORM_CTRL_ID,         //读用户信息
#endif
#ifdef WRITEUSERDATA_IDX
        MMIJAVA_WRITE_USERINFO_FORM_CTRL_ID,        //写用户信息
#endif
    };

    /*显示权限名的label*/
    MMI_CTRL_ID_T label_id[] =
    {
#ifdef APPAUTOINVOCATION_IDX
        MMIJAVA_APP_AUTOPLAY_TITLE_LABEL_CTRL_ID,
#endif
#ifdef LOCALCONNECTIVITY_IDX
        MMIJAVA_LOCAL_CONNECT_TITLE_LABEL_CTRL_ID,
#endif
#ifdef LOWLEVELNETACCESS_IDX
        MMIJAVA_LOW_LEVEL_NET_TITLE_LABEL_CTRL_ID,
#endif
#ifdef MESSAGING_IDX
        MMIJAVA_MESSAGE_TITLE_LABEL_CTRL_ID,
#endif
#ifdef MULTIMEDIARECORDING_IDX
        MMIJAVA_MULTIMEDIA_TITLE_LABEL_CTRL_ID,
#endif
#ifdef NETACCESS_IDX
        MMIJAVA_NETWORK_LINK_TITLE_LABEL_CTRL_ID,
#endif
#ifdef READUSERDATA_IDX
        MMIJAVA_READ_USERINFO_TITLE_LABEL_CTRL_ID,
#endif
#ifdef WRITEUSERDATA_IDX
        MMIJAVA_WRITE_USERINFO_TITLE_LABEL_CTRL_ID,
#endif
    };

    /*显示对应权限名的权限值的label*/
    MMI_CTRL_ID_T label_edit_id[] =
    {
#ifdef APPAUTOINVOCATION_IDX
        MMIJAVA_APP_AUTOPLAY_TITLE_LABEL_EDIT_ID,
#endif
#ifdef LOCALCONNECTIVITY_IDX
        MMIJAVA_LOCAL_CONNECT_TITLE_LABEL_EDIT_ID,
#endif
#ifdef LOWLEVELNETACCESS_IDX
        MMIJAVA_LOW_LEVEL_NET_TITLE_LABEL_EDIT_ID,
#endif
#ifdef MESSAGING_IDX
        MMIJAVA_MESSAGE_TITLE_LABEL_EDIT_ID,
#endif
#ifdef MULTIMEDIARECORDING_IDX
        MMIJAVA_MULTIMEDIA_TITLE_LABEL_EDIT_ID,
#endif
#ifdef NETACCESS_IDX
        MMIJAVA_NETWORK_LINK_TITLE_LABEL_EDIT_ID,
#endif
#ifdef READUSERDATA_IDX
        MMIJAVA_READ_USERINFO_TITLE_LABEL_EDIT_ID,
#endif
#ifdef WRITEUSERDATA_IDX
        MMIJAVA_WRITE_USERINFO_TITLE_LABEL_EDIT_ID,
#endif
    };

    /*权限名*/
    MMI_TEXT_ID_T text_label_name[] =
    {
    #ifdef APPAUTOINVOCATION_IDX
        STR_JAVA_AUTO_START,
    #endif
    #ifdef LOCALCONNECTIVITY_IDX
        STR_JAVA_LOCAL_CONNECTION,
    #endif
    #ifdef LOWLEVELNETACCESS_IDX
        TXT_JAVA_PERMIT_LOW_NET,
    #endif
    #ifdef MESSAGING_IDX
        STR_JAVA_MESSAGING,
    #endif
    #ifdef MULTIMEDIARECORDING_IDX
        STR_JAVA_RECORDING,
    #endif
    #ifdef NETACCESS_IDX
        STR_JAVA_NET_ACCESS,
    #endif
    #ifdef READUSERDATA_IDX
        STR_JAVA_READ_DATA,
    #endif
    #ifdef WRITEUSERDATA_IDX
        STR_JAVA_WRITE_DATA
    #endif
    };

    /*权限值*/
    MMI_TEXT_ID_T text_option_str[] =
    {
        STR_JAVA_ALWAYS_ALLOW,
        STR_JAVA_NOT_ALLOW,
        STR_JAVA_ASK_EVERY,
        STR_JAVA_ASK_FIRST,
        STR_JAVA_ASK_EVERY,
    };

    cur_mmi_event = MMIJAVA_find_MMIEvent(event_id);
    if(cur_mmi_event == NULL)
    {
        return;
    }
    permissions      = (NAMSPermssions_t *)(cur_mmi_event->data);

    dropdownlist_item_ptr = SCI_ALLOCA((sizeof(text_option_str)/sizeof(MMI_TEXT_ID_T))*sizeof(MMI_STRING_T));
    if(PNULL == dropdownlist_item_ptr)
    {
        SCI_TRACE_LOW("MMIJava_setting_SetPermitInfoFormParam ALLOCA FAILED");
        return;
    }
    g_permission_num = permissions->permissions_num;
    SCI_TRACE_LOW("====myriad log========function group num - %d ,",g_permission_num);

    /*** set label string***/
    for (i = 0; i < g_permission_num; i++)
    {
        MMI_GetLabelTextByLang(text_label_name[i],&name_ptr);
        GUIFORM_SetStyle(form_id[i],GUIFORM_STYLE_UNIT);
        GUIFORM_PermitChildFont(form_id[i], FALSE);
        GUILABEL_SetFont(label_id[i], MMI_DEFAULT_BIG_FONT, MMI_WHITE_COLOR);
        GUILABEL_SetText(label_id[i],&name_ptr,FALSE);
    }

    permit_info_map = (PERMIT_INFO_MAP_T *) SCI_ALLOCA(((unsigned int)g_permission_num) * sizeof(PERMIT_INFO_MAP_T));
    if(permit_info_map == NULL)
    {
        SCI_FREEIF(dropdownlist_item_ptr);
        SCI_TRACE_LOW("permit_info_map ALLOCA FAILED");
        return;
    }
    SCI_MEMSET((void*)permit_info_map,0,((unsigned int)g_permission_num )* sizeof(PERMIT_INFO_MAP_T));

    for (i = 0; i < g_permission_num; i++)
    {
        int32 types_index             = 0;
        int32 permission_answer       = 0;
        uint8 dropdownlist_num        = 0;
        uint8 permission_types        = 0;
        uint8 current_permission      = 0;
        int32 permission_answer_index = 0;
        uint8 permit_arr[10]          = {0};

        SCI_MEMSET(permit_arr,0,10);
        permission_types   = permissions->permssions_body[i].allowed;
        current_permission = permissions->permssions_body[i].current;
        SCI_TRACE_LOW("==== myriag log: function group(%d),allowed(0x%x),current(0x%x) ====",i,permission_types,current_permission);

        while(permission_types)
        {
            if (permission_types & 0x01)
            {
                permission_answer_index++;
                switch(types_index)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    {
                        permit_arr[dropdownlist_num] = 1 <<types_index;
                        MMI_GetLabelTextByLang(text_option_str[types_index], &dropdownlist_item_ptr[dropdownlist_num++]);
                        SCI_TRACE_LOW("==== myriad log: function group(item:%d),dropdownlist_num(%d)! ===",i,dropdownlist_num);
                    }
                    break;

                    default:
                    {
                        SCI_TRACE_LOW("==== myriad log: serious error (%d)! ===",types_index);
                    }
                    break;
                }
            }

            if (current_permission & 0x01)
            {
                permission_answer = (permission_answer_index - 1);
            }
            types_index++;
            permission_types >>= 1;
            current_permission >>= 1;
        }

        if(0 ==dropdownlist_num)
        {
            /*---- no permissions,set form grayed ------*/
            child_handle = MMK_GetCtrlHandleByWin(win_id, form_id[i]);
            GUIFORM_SetChildDisplay(form_handle,child_handle,GUIFORM_CHILD_DISP_GRAY);
        }
        else
        {
            GUIFORM_PermitChildFont(form_id[i], FALSE);
            GUILABEL_SetFont(label_edit_id[i], MMI_DEFAULT_SMALL_FONT, MMI_DARK_WHITE_COLOR);
            GUILABEL_SetText(label_edit_id[i],&dropdownlist_item_ptr[permission_answer],FALSE);
            permit_info_map[i].permit_array = (uint8 *)SCI_ALLOCA(dropdownlist_num * sizeof(uint8));
            if(permit_info_map[i].permit_array ==NULL)
            {
                for(j = 0; j < i; j++)
                {
                    SCI_FREEIF(permit_info_map[j].permit_array);
                }
                SCI_FREEIF(permit_info_map);

                for(j = 0; j < g_permission_num; j++)
                {
                    child_handle = MMK_GetCtrlHandleByWin(win_id, form_id[j]);
                    GUIFORM_SetChildDisplay(form_handle,child_handle,GUIFORM_CHILD_DISP_GRAY);
                }
                SCI_FREEIF(dropdownlist_item_ptr);
                SCI_TRACE_LOW("permit_info_map[i].permit_array ALLOCA FAILED");
                return;
            }

            SCI_MEMCPY(permit_info_map[i].permit_array,permit_arr,dropdownlist_num);
            permit_info_map[i].be_check        = 0 ;
            permit_info_map[i].cur_permit_item = permission_answer ;
            permit_info_map[i].permit_item_num = dropdownlist_num ;
        }
    }

    MMK_SetAtvCtrl(win_id, form_id[0]);

    GUIFORM_SetSpace (MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID,&form_space,&form_space);
    GUIFORM_SetMargin(MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID,0);
    GUIFORM_SetDividingLine(MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID, 0, MMITHEME_GetTransparenceColor());

    SCI_FREEIF(dropdownlist_item_ptr);
    return;
}

LOCAL void MMIJava_setting_updatePermitShowInfo(void)
{
    /*显示对应权限名的权限值的label*/
    MMI_CTRL_ID_T label_edit_id[] =
    {
#ifdef APPAUTOINVOCATION_IDX
        MMIJAVA_APP_AUTOPLAY_TITLE_LABEL_EDIT_ID,
#endif
#ifdef LOCALCONNECTIVITY_IDX
        MMIJAVA_LOCAL_CONNECT_TITLE_LABEL_EDIT_ID,
#endif
#ifdef LOWLEVELNETACCESS_IDX
        MMIJAVA_LOW_LEVEL_NET_TITLE_LABEL_EDIT_ID,
#endif
#ifdef MESSAGING_IDX
        MMIJAVA_MESSAGE_TITLE_LABEL_EDIT_ID,
#endif
#ifdef MULTIMEDIARECORDING_IDX
        MMIJAVA_MULTIMEDIA_TITLE_LABEL_EDIT_ID,
#endif
#ifdef NETACCESS_IDX
        MMIJAVA_NETWORK_LINK_TITLE_LABEL_EDIT_ID,
#endif
#ifdef READUSERDATA_IDX
        MMIJAVA_READ_USERINFO_TITLE_LABEL_EDIT_ID,
#endif
#ifdef WRITEUSERDATA_IDX
        MMIJAVA_WRITE_USERINFO_TITLE_LABEL_EDIT_ID,
#endif
    };

    /*权限值*/
    MMI_TEXT_ID_T text_option_str[] =
    {
        STR_JAVA_ALWAYS_ALLOW,
        STR_JAVA_NOT_ALLOW,
        STR_JAVA_ASK_EVERY,
        STR_JAVA_ASK_FIRST,
        STR_JAVA_ASK_EVERY,
    };

    int total_idx = sizeof(label_edit_id)/sizeof(label_edit_id[0]);
    int i =0,j=0;
    MMI_STRING_T name_ptr ={0};

    if(permit_info_map == NULL)
    {
        return;
    }

    for(i=0; i< total_idx;i++)
    {
        //be_check表示被重设过
        if(permit_info_map[i].be_check ==1)
        {
            for(j=0;j<sizeof(text_option_str)/sizeof(text_option_str[0]);j++)
            {
                //找出新值的索引
                if((permit_info_map[i].permit_array[permit_info_map[i].cur_permit_item]) & (1<<j))
                {
                    MMI_GetLabelTextByLang(text_option_str[j],&name_ptr);
                    GUILABEL_SetText(label_edit_id[i],&name_ptr,FALSE);
                    continue;
                }
            }
        }
    }
}

/**************************************************************************************************
  * Description: win_func of JBED_CONNECT_PERMISSION_SETTING_WIN_TAB
  * Input Paras:
  * Output Paras:
  * note :
  *************************************************************************************************/
LOCAL MMI_RESULT_E MMIJava_setting_HandlePermissionSetWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    int recode                            = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T label_edit_id[] =
    {
#ifdef APPAUTOINVOCATION_IDX
        MMIJAVA_APP_AUTOPLAY_TITLE_LABEL_EDIT_ID,
#endif
#ifdef LOCALCONNECTIVITY_IDX
        MMIJAVA_LOCAL_CONNECT_TITLE_LABEL_EDIT_ID,
#endif
#ifdef LOWLEVELNETACCESS_IDX
        MMIJAVA_LOW_LEVEL_NET_TITLE_LABEL_EDIT_ID,
#endif
#ifdef MESSAGING_IDX
        MMIJAVA_MESSAGE_TITLE_LABEL_EDIT_ID,
#endif
#ifdef MULTIMEDIARECORDING_IDX
        MMIJAVA_MULTIMEDIA_TITLE_LABEL_EDIT_ID,
#endif
#ifdef NETACCESS_IDX
        MMIJAVA_NETWORK_LINK_TITLE_LABEL_EDIT_ID,
#endif
#ifdef READUSERDATA_IDX
        MMIJAVA_READ_USERINFO_TITLE_LABEL_EDIT_ID,
#endif
#ifdef WRITEUSERDATA_IDX
        MMIJAVA_WRITE_USERINFO_TITLE_LABEL_EDIT_ID,
#endif
    };

    int  *cur_evt_id = MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_BG_T bg = {0};

            MMIJAVA_UpdateTitle(STR_PERMISSION_EXT01);
        #ifndef MMI_PDA_SUPPORT
            GUIFORM_SetCircularHandleUpDown(MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID,TRUE);
        #endif
        #if !defined(MMI_GUI_STYLE_TYPICAL)
            GUIFORM_SetType(MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID,GUIFORM_TYPE_TP);
        #endif

            //set form background color
            bg.bg_type = GUI_BG_COLOR;
            bg.color = MMI_BLACK_COLOR;
            GUIFORM_SetBg(MMIJAVA_PM_SETTING_FATHER_FORM_CRTL_ID, &bg);

            MMIJava_setting_SetPermitInfoFormParam(win_id,*cur_evt_id);
            MMIJAVA_util_deallocMMIEvent_byEventID(*cur_evt_id);
        }
        break;

        case MSG_GET_FOCUS:
            MMIJAVA_UpdateTitle(STR_PERMISSION_EXT01);
            MMIJava_setting_updatePermitShowInfo();
            break;

        case MSG_FULL_PAINT:
            break;

    #ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
    #endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        {
            int cur_idx ;
            cur_idx = (MMK_GetActiveCtrlId(win_id) - MMIJAVA_APP_AUTOPLAY_FORM_CTRL_ID)/3;
            MMK_CreateWin((uint32 *)JBED_PERMISSION_RADIO_WIN_TAB, (ADD_DATA)cur_idx);
        }
        break;

        /*之前的设置实际上并没有真正保存,只有关闭设置窗口的时候才真正保存,因此ok和cancel处理成相同*/
        case MSG_KEYLONG_POWER:
        case MSG_KEYDOWN_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        case MSG_APP_WEB:
        case MSG_APP_OK:
#ifdef MMI_PDA_SUPPORT
        case MSG_APP_MENU:
#endif
        {
            uint8 i = 0;
            uint8 check_count = 0;

            if(permit_info_map ==NULL)
            {
                MMK_CloseWin(win_id);
                MMIJAVA_NotifyJVMExit();
                break;
            }

            for(i = 0; i < g_permission_num; i++)
            {
                //统计被更改的项数
                if(permit_info_map[i].be_check == 1)
                {
                    check_count++ ;
                }
            }

            for(i = 0; i < g_permission_num; i++)
            {
                //找到北更改的第一项
                if(1 == permit_info_map[i].be_check)
                {
                    break;
                }
            }

            if(i < g_permission_num)
            {
                Jbed_nAms_setMIDletPermissions((check_count<<4) |i, permit_info_map[i].permit_array[permit_info_map[i].cur_permit_item], EPA_CHECK);
                permit_info_map[i].be_check = 0;
                MMIJAVA_util_openWaitingWin(NULL);
            }
            else //这个表示没有被更改过
            {
                for(i = 0; i < g_permission_num; i++)
                {
                    SCI_FREEIF(permit_info_map[i].permit_array);
                }
                SCI_FREEIF(permit_info_map);
                MMK_CloseWin(win_id);
                MMIJAVA_NotifyJVMExit();
            }
        }
        break;

        case MSG_CLOSE_WINDOW:
            //MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
            MMK_FreeWinAddData(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

LOCAL void AppendPermissionItems(MMI_CTRL_ID_T ctrl_id,int perm_idx)
{
    int i=0,j=0;
    GUIITEM_STYLE_E list_style = GUIITEM_STYLE_ONE_LINE_RADIO;

    /*权限值*/
    MMI_TEXT_ID_T text_option_str[] =
    {
        STR_JAVA_ALWAYS_ALLOW,
        STR_JAVA_NOT_ALLOW,
        STR_JAVA_ASK_EVERY,
        STR_JAVA_ASK_FIRST,
        STR_JAVA_ASK_EVERY,
    };

    for(i=0;i<permit_info_map[perm_idx].permit_item_num;i++)
    {
        for(j=0;j<sizeof(text_option_str)/sizeof(text_option_str[0]);j++)
        {
            if((permit_info_map[perm_idx].permit_array[i]) & (1<<j))
            {
                MMIAPISET_AppendListItemByTextIdExt(text_option_str[j],
                                                    TXT_NULL,
                                                    STXT_SOFTKEY_SELECT_MK,
                                                    TXT_COMMON_COMMON_BACK,
                                                    ctrl_id,
                                                    GUIITEM_STYLE_ONE_LINE_TEXT_MS);
                continue;
            }
        }
    }
}

LOCAL MMI_RESULT_E HandlePermissionSelectWin(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    int recode  = MMI_RESULT_TRUE;
    uint16       cur_index = 0;
    uint16       sel_index = 0;
    uint16       max_index = 0;
    int          perm_idx  = 0;
    int          new_index = 0;
    MMI_CTRL_ID_T ctrl_id  = MMIJAVA_PM_RADIO_CRTL_ID;

    MMI_TEXT_ID_T title_id[] =
    {
    #ifdef APPAUTOINVOCATION_IDX
        STR_JAVA_AUTO_START,
    #endif
    #ifdef LOCALCONNECTIVITY_IDX
        STR_JAVA_LOCAL_CONNECTION,
    #endif
    #ifdef LOWLEVELNETACCESS_IDX
        TXT_JAVA_PERMIT_LOW_NET,
    #endif
    #ifdef MESSAGING_IDX
        STR_JAVA_MESSAGING,
    #endif
    #ifdef MULTIMEDIARECORDING_IDX
        STR_JAVA_RECORDING,
    #endif
    #ifdef NETACCESS_IDX
        STR_JAVA_NET_ACCESS,
    #endif
    #ifdef READUSERDATA_IDX
        STR_JAVA_READ_DATA,
    #endif
    #ifdef WRITEUSERDATA_IDX
        STR_JAVA_WRITE_DATA
    #endif
    };

    perm_idx = (int)MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMIJAVA_UpdateTitle(STR_JAVA_SELECT);
            max_index = permit_info_map[perm_idx].permit_item_num;
            cur_index = permit_info_map[perm_idx].cur_permit_item;
            GUILIST_SetMaxItem(ctrl_id,max_index,FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            AppendPermissionItems(ctrl_id,perm_idx);
            MMK_SetAtvCtrl(win_id,ctrl_id);
            GUILIST_SetSelectedItem(ctrl_id,cur_index, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id,cur_index);
        }
        break;

        case MSG_FULL_PAINT:
            break;

        case MSG_KEYLONG_POWER:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;

        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_APP_OK:
        {
            new_index = GUILIST_GetCurItemIndex(ctrl_id);
            if(new_index == permit_info_map[perm_idx].cur_permit_item)
            {
                SCI_TRACE_LOW("===permission set:nothing to change==");
                MMK_CloseWin(win_id);
                break;
            }
            /*更新值，但这里未有同步更新到java*/
            permit_info_map[perm_idx].cur_permit_item = new_index;
            permit_info_map[perm_idx].be_check = 1;
            MMK_CloseWin(win_id);
        }
        break;

        case MSG_CLOSE_WINDOW:
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

/**************************************************************************************************
  * Description: MMIJAVA_OpenPermissionWin
  * Input Paras:
  * Output Paras:
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_OpenPermissionWin(int eventid)
{
    int * pevt_id_ptr= PNULL;

    pevt_id_ptr = (int *)SCI_ALLOCA(sizeof(int));
    if(PNULL == pevt_id_ptr)
    {
        return;
    }

    *pevt_id_ptr = eventid;

    MMK_CreateWin((uint32*)JBED_CONNECT_PERMISSION_SETTING_WIN_TAB, (ADD_DATA)pevt_id_ptr);

}

/**************************************************************************************************
  * Description: MMIJAVA_OpenPermissionWin
  * Input Paras:
  * Output Paras:
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_HandlePermissionEvent(void)
{
    uint8 i =0;

    for(i = 0; i < g_permission_num; i++)
    {
        if(permit_info_map[i].be_check)
        {
            break;
        }
    }

    if(i < g_permission_num)
    {
        Jbed_nAms_setMIDletPermissions(i, permit_info_map[i].permit_array[permit_info_map[i].cur_permit_item], EPA_CHECK);
        permit_info_map[i].be_check = 0;
    }
    else
    {
        MMIJAVA_util_closeWaitingWin(2);
        MMK_CloseWin(JBED_CONNECT_PERMISSION_SETTING_WIN_ID);
        MMIPUB_OpenAlertWinByTextId(NULL, STR_JAVA_OPERATE_SUCCESS, TXT_NULL, IMAGE_PUBWIN_SUCCESS, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);

        for(i = 0; i < g_permission_num; i++)
        {
            SCI_FREEIF(permit_info_map[i].permit_array);
        }
        SCI_FREEIF(permit_info_map);
        MMIJAVA_NotifyJVMExit();
    }
}
/**************************************************************************************************
  * Description: MMIJAVA_OpenPermissionWin
  * Input Paras:
  * Output Paras:
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_SetPermisson(int suite_id)
{
    if(MMIJAVA_HandleStateConflict())
    {
        return;
    }

#if defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN)
    /* We already check INSTALL or UNINSTALL conflict before, if the jbed thread
     * still exists at this time, it means that the midlet is running on background
     * or vm is starting or closing.
     */
    if (isJbedThreadExist())
    {
        SCI_TRACE_LOW("=== Myriad MMIJAVA_SetPermisson Jbed thread exist.");

        if (Jbed_tm_getRunningAppNum() > 0) // the midlet is running on background.
        {
            MMIJAVA_PostMIDletRunningBanner();
            return;
        }
        else // vm is starting or closing.
        {
            MMIJAVA_OpenAlertByTextId(STR_JAVA_VM_BUSY);
            return;
        }
    }
#endif

    //entry point of "permission setting" sub-Menu of app setting
    Jbed_nAms_queryMIDletPermissions(suite_id);
    MMIJAVA_util_openWaitingWin(NULL);
}
