/****************************************************************************
** File Name:      mmijava_permissionsetwin.c
** Author:
** Date:           2012/8/15
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.
** Description:    This file is used to describe functions of permission set
****************************************************************************/

#include "mmi_app_myriad_java_trc.h"
#include "mmijava_id.h"
#include "mmidisplay_data.h"
//#include <java_mmi_text.h>
//#include <os_api.h>
#include "window_parse.h"
#include "jbed_exported_defines.h"
//#include <cpl_tpal.h>
#include "jbed_jbni.h"
#include "guiform.h"
//#include <guilistbox.h>
#include "guidropdownlist.h"
#include "mmipub.h"
#include "mmiconnection_export.h"
#include "mmijava_menutable.h"
#include "mmifmm_export.h"
//#include <mmijava_common.h>
#include "guilabel.h"
#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmijava_netsetwin.h"
#include "cpl_midletinfo.h"

typedef struct
{
    int           suite_id;
    int           item_num;
    int           item_cur;
    MMI_STRING_T* item_ptr;
}NW_ITEM_NAMES;  //存放


const MMI_TEXT_ID_T sim_not_ok_str[] =
{
     STR_JAVA_SIM1_NO,
 #if defined(MMI_MULTI_SIM_SYS_DUAL)||defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
     STR_JAVA_SIM2_NO,
 #endif
 #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
     TXT_JAVA_SELECT_NET_SIM3_NOT_OK,
 #endif
 #if defined(MMI_MULTI_SIM_SYS_QUAD)
     TXT_JAVA_SELECT_NET_SIM4_NOT_OK,
 #endif
};

LOCAL NW_ITEM_NAMES s_nw_item_names[MMI_DUAL_SYS_MAX + 1];

/**************************************************************************************************
  * Description: UI of network setting
  * Input Paras:
      > win_id  :
      > cursuiteid : suite id
  * Output Paras:
      > return : void
  * note :
  *************************************************************************************************/
LOCAL void MMIJava_setting_SetSelectNetFormParam(MMI_WIN_ID_T win_id, int32 cur_suite_id);

/**************************************************************************************************
  * Description: win_func of JBED_NETWORK_CONNECT_SETTING_WIN_TAB
  * Input Paras:
  * Output Paras:
  * note :
  *************************************************************************************************/
LOCAL MMI_RESULT_E MMIJava_setting_HandleNetworkConnectWinMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param);

/**************************************************************************************************
  * Description: get app install dir of one midlet
  * Input Paras:
      > suite_id  : unique id of midlet suite
      > path_len :max len of w_path
  * Output Paras:
      > return : TRUE ,w_path is valid,else w_path is invalid
      > w_path : install dir 0f app
  * note :
  *************************************************************************************************/
LOCAL BOOLEAN MMIJava_setting_GetAppPath(IN int32 suite_id,IN int32 path_len,OUT short * w_path);
/**************************************************************************************************
  * Description: after installing a suite,create its setting info about network
  * Input Paras:
      > suite_id  : unique id of midlet suite
      > app_setting_info : initial info of suite about network
  * Output Paras:
      > return : TRUE ,create Ok;or else,fail
  * note : call the func when a suite is installed over
  *************************************************************************************************/
LOCAL BOOLEAN MMIJAVA_setting_creatAppNetInfo(IN int32 suite_id);

/**************************************************************************************************
  * Description: delete all setting info of all suites about network
  * Input Paras:
  * Output Paras:
      > return : TRUE ,del ok is valid;or else, fail
  * note : call the func when deleting all
  *************************************************************************************************/
LOCAL BOOLEAN MMIJAVA_setting_delAllAppNetInfo(void);

LOCAL MMI_RESULT_E HandleNetworkSelectWin(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param);

/**************************************************************************************************
 ** window table of network connection settings
 **************************************************************************************************/
WINDOW_TABLE( JBED_NETWORK_CONNECT_SETTING_WIN_TAB ) =
{
    WIN_FUNC((uint32)MMIJava_setting_HandleNetworkConnectWinMsg),
    WIN_ID(JBED_NETWORK_CONNECT_SETTING_WIN_ID),

    //FORM 父控件，填充屏幕状态栏和SOFTKEY之外的区域
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID),

        //网络选择(如卡1，卡2，wifi等)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_NW_SETTING_CHOOSE_NET_FORM_CTRL_ID   ,MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_NW_SETTING_CHOOSE_NET_TITLE_LABEL_CTRL_ID,MMIJAVA_NW_SETTING_CHOOSE_NET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_NW_SETTING_CHOOSE_NET_TITLE_LABEL_EDIT_ID,MMIJAVA_NW_SETTING_CHOOSE_NET_FORM_CTRL_ID),

        //卡1 APN设置
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER ,MMIJAVA_NW_SETTING_SIM1_FORM_CTRL_ID        ,MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_NW_SETTING_SIM1_TITLE_LABEL_CTRL_ID ,MMIJAVA_NW_SETTING_SIM1_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_NW_SETTING_SIM1_TITLE_LABEL_EDIT_ID ,MMIJAVA_NW_SETTING_SIM1_FORM_CTRL_ID),

        //卡2 APN设置
    #if defined(MMI_MULTI_SIM_SYS_DUAL)||defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER ,MMIJAVA_NW_SETTING_SIM2_FORM_CTRL_ID        ,MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_NW_SETTING_SIM2_TITLE_LABEL_CTRL_ID ,MMIJAVA_NW_SETTING_SIM2_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_NW_SETTING_SIM2_TITLE_LABEL_EDIT_ID ,MMIJAVA_NW_SETTING_SIM2_FORM_CTRL_ID),
    #endif

        //卡3 APN设置
    #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER ,MMIJAVA_NW_SETTING_SIM3_FORM_CTRL_ID        ,MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_NW_SETTING_SIM3_TITLE_LABEL_CTRL_ID ,MMIJAVA_NW_SETTING_SIM3_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_NW_SETTING_SIM3_TITLE_LABEL_EDIT_ID ,MMIJAVA_NW_SETTING_SIM3_FORM_CTRL_ID),
    #endif

        //卡4 APN设置
    #if defined(MMI_MULTI_SIM_SYS_QUAD)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER ,MMIJAVA_NW_SETTING_SIM4_FORM_CTRL_ID        ,MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_NW_SETTING_SIM4_TITLE_LABEL_CTRL_ID ,MMIJAVA_NW_SETTING_SIM4_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMIJAVA_NW_SETTING_SIM4_TITLE_LABEL_EDIT_ID ,MMIJAVA_NW_SETTING_SIM4_FORM_CTRL_ID),
    #endif

    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE( JBED_NETWORK_RADIO_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleNetworkSelectWin ),
    WIN_ID( JBED_NETWORK_RADIO_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJAVA_NW_RADIO_CRTL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

/**************************************************************************************************
  * Description: UI of network setting
  * Input Paras:
      > win_id  :
      > cursuiteid : suite id
  * Output Paras:
      > return : void
  * note :
  *************************************************************************************************/
LOCAL void MMIJava_setting_SetSelectNetFormParam(MMI_WIN_ID_T win_id, int32 cur_suite_id)
{
    uint16                form_space            = 0;
    MMI_STRING_T*         dropdownlist_item_ptr = PNULL;
    uint16                list_num              = 0;
    uint8                 linkset_num           = 0;
    JAVA_APP_SETTING_T    app_setting_info      = {0};
    int32                 i                     = MN_DUAL_SYS_1;
    uint16                index                 = 0;
#if defined GPRS_SUPPORT
    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
#endif

    MMI_TEXT_ID_T java_nw_account[] =
    {
        STR_JAVA_SIM1_NETWORK,
    #if defined(MMI_MULTI_SIM_SYS_DUAL)||defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        STR_JAVA_SIM2_NETWORK,
    #endif
    #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_JAVA_NW_SIM3_LABEL,
    #endif
    #if defined(MMI_MULTI_SIM_SYS_QUAD)
         TXT_JAVA_NW_SIM4_LABEL
    #endif
    };

    MMI_CTRL_ID_T   label_id[] =
    {
        MMIJAVA_NW_SETTING_SIM1_TITLE_LABEL_CTRL_ID,
        MMIJAVA_NW_SETTING_SIM2_TITLE_LABEL_CTRL_ID,
        MMIJAVA_NW_SETTING_SIM3_TITLE_LABEL_CTRL_ID,
        MMIJAVA_NW_SETTING_SIM4_TITLE_LABEL_CTRL_ID
    };

    MMI_CTRL_ID_T   label_edit_id[] =
    {
        MMIJAVA_NW_SETTING_SIM1_TITLE_LABEL_EDIT_ID,
        MMIJAVA_NW_SETTING_SIM2_TITLE_LABEL_EDIT_ID,
        MMIJAVA_NW_SETTING_SIM3_TITLE_LABEL_EDIT_ID,
        MMIJAVA_NW_SETTING_SIM4_TITLE_LABEL_EDIT_ID
    };

    MMI_CTRL_ID_T   form_id[] =
    {
        MMIJAVA_NW_SETTING_SIM1_FORM_CTRL_ID,
        MMIJAVA_NW_SETTING_SIM2_FORM_CTRL_ID,
        MMIJAVA_NW_SETTING_SIM3_FORM_CTRL_ID,
        MMIJAVA_NW_SETTING_SIM4_FORM_CTRL_ID
    };

    MMI_TEXT_ID_T label_str[] =
    {
        STR_ILIST_SIM_ONE,
    #if defined(MMI_MULTI_SIM_SYS_DUAL)|| defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        STR_ILIST_SIM_TWO,
    #endif
    #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_SIM_SEL_SIM3,
    #endif
    #if defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_SIM_SEL_SIM4,
    #endif
    };

    SCI_MEMSET(&app_setting_info,0,sizeof(JAVA_APP_SETTING_T));

    if(!MMIJAVA_setting_getAppNetInfo(cur_suite_id, &app_setting_info))   //如果获取信息失败，设置默认值
    {
        int32 preferredSim = -1;

        //在这里设置默认值
        SCI_TRACE_LOW("====myriad log: in MMIJava_setting_SetSelectNetFormParam(),get saved info failed====");

        preferredSim = MMIJAVA_GetPreferredSimId();
        if (preferredSim == -1)
        {
            for(i = MN_DUAL_SYS_1; i < MMIJAVA_GetMaxSimNumber(); i++)
            {
                if(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(i))
                {
                    break;
                }
            }
            preferredSim = i;
        }

        if (preferredSim < MMIJAVA_GetMaxSimNumber())
        {
            app_setting_info.nw_type = preferredSim;
            app_setting_info.sim_apn[app_setting_info.nw_type] =
                MMIAPICONNECTION_GetAccountDefaultIndex(preferredSim, MMICONNECTION_ACCOUNT_JAVA);
        }
        else
        {
            //Does not find the correct network setting.
            app_setting_info.nw_type                           = JAVA_NETWORK_TYPE_SIM1;
            app_setting_info.sim_apn[app_setting_info.nw_type] = 0;
        }
    }

    /*----- set label text ------------*/
    GUIFORM_SetStyle(MMIJAVA_NW_SETTING_CHOOSE_NET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_PermitChildFont(MMIJAVA_NW_SETTING_CHOOSE_NET_FORM_CTRL_ID, FALSE);
    GUILABEL_SetFont(MMIJAVA_NW_SETTING_CHOOSE_NET_TITLE_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, MMI_WHITE_COLOR);
    GUILABEL_SetTextById(MMIJAVA_NW_SETTING_CHOOSE_NET_TITLE_LABEL_CTRL_ID,STR_JAVA_NETWORK,FALSE);

    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        GUIFORM_SetStyle(form_id[i],GUIFORM_STYLE_UNIT);
        GUIFORM_PermitChildFont(form_id[i], FALSE);
        if (MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)i))
        {
            GUILABEL_SetFont(label_id[i], MMI_DEFAULT_BIG_FONT, MMI_WHITE_COLOR);
            GUILABEL_SetTextById(label_id[i],java_nw_account[i],FALSE);
        }
        else
        {
            GUILABEL_SetFont(label_id[i], MMI_DEFAULT_BIG_FONT, MMI_DARK_GRAY_COLOR);
            GUILABEL_SetTextById(label_id[i],java_nw_account[i],FALSE);
            GUIFORM_SetChildDisplay(MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID, form_id[i], GUIFORM_CHILD_DISP_GRAY);
        }
    }
    /*----- set label text over------------*/

#if defined GPRS_SUPPORT
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
#ifndef MMI_GPRS_SUPPORT
      //Previous 880XG platform does not define MMI_GPRS_SUPPORT but GPRS_SUPPORT
        linkset_num = MMIAPICONNECTION_GetLinkSettingNum();
#else //MMI_GPRS_SUPPORT
        linkset_num = MMIAPICONNECTION_GetLinkSettingNum(i);
#endif

        dropdownlist_item_ptr = SCI_ALLOCA(linkset_num*sizeof(MMI_STRING_T));
        if(PNULL == dropdownlist_item_ptr)
        {
            SCI_TRACE_LOW("MMIJava_setting_SetSelectNetFormParam ALLOCA FAILED");
            return;
        }

        //RESET
        SCI_MEMSET(dropdownlist_item_ptr,0,linkset_num*sizeof(MMI_STRING_T));

        for(index = 0; index < linkset_num; index++)
        {
#ifndef MMI_GPRS_SUPPORT
      //Previous 880XG platform does not define MMI_GPRS_SUPPORT but GPRS_SUPPORT
            link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(index);
#else //MMI_GPRS_SUPPORT
            link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(i, index);
#endif

#if 1 //显示网络名字
            dropdownlist_item_ptr[index].wstr_len = MIN(GUILIST_STRING_MAX_NUM,link_setting_ptr->name_len);
            dropdownlist_item_ptr[index].wstr_ptr = SCI_ALLOCA((DROPDOWNLIST_STRING_MAX_NUM + 1) * sizeof(wchar));
            SCI_MEMSET(dropdownlist_item_ptr[index].wstr_ptr, 0, ((DROPDOWNLIST_STRING_MAX_NUM + 1) * sizeof(wchar)));

            MMI_WSTRNCPY(dropdownlist_item_ptr[index].wstr_ptr,dropdownlist_item_ptr[index].wstr_len,link_setting_ptr->name,dropdownlist_item_ptr[index].wstr_len,dropdownlist_item_ptr[index].wstr_len);
#else //显示APN 名
            dropdownlist_item_ptr[index].wstr_len = MIN(GUILIST_STRING_MAX_NUM,link_setting_ptr->name_len*2);
            dropdownlist_item_ptr[index].wstr_ptr = SCI_ALLOC_A((DROPDOWNLIST_STRING_MAX_NUM + 1) * sizeof(wchar));
            SCI_MEMSET(dropdownlist_item_ptr[index].wstr_ptr, 0, ((DROPDOWNLIST_STRING_MAX_NUM + 1) * sizeof(wchar)));
            SCI_MEMSET(wPath,0,sizeof(wPath));
            //GUI_UTF8ToWstr(wPath, 100, (uint8*)link_setting_ptr->apn,strlen(link_setting_ptr->apn));
            Jbni_convertUtf8ToUcs2((char*)link_setting_ptr->apn, strlen(link_setting_ptr->apn), wPath, strlen(link_setting_ptr->apn)<<1);
            MMI_WSTRNCPY(dropdownlist_item_ptr[index].wstr_ptr,dropdownlist_item_ptr[index].wstr_len,wPath,dropdownlist_item_ptr[index].wstr_len,dropdownlist_item_ptr[index].wstr_len);
#endif
        }

        GUIFORM_PermitChildFont(form_id[i], FALSE);
        if (MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)i))
        {
            GUILABEL_SetFont(label_edit_id[i], MMI_DEFAULT_SMALL_FONT, MMI_DARK_WHITE_COLOR);
        }
        else
        {
            GUILABEL_SetFont(label_edit_id[i], MMI_DEFAULT_SMALL_FONT, MMI_DARK_GRAY_COLOR);
        }
        GUILABEL_SetText(label_edit_id[i],&dropdownlist_item_ptr[app_setting_info.sim_apn[i]],FALSE);

        s_nw_item_names[i+1].suite_id = cur_suite_id;
        s_nw_item_names[i+1].item_num = linkset_num;
        s_nw_item_names[i+1].item_cur = app_setting_info.sim_apn[i];
        s_nw_item_names[i+1].item_ptr = dropdownlist_item_ptr;
    }
#endif

    //for net type dropdown list
    list_num = MMI_DUAL_SYS_MAX;
#ifdef WIFI_SUPPORT
    list_num++;
#endif

    dropdownlist_item_ptr = SCI_ALLOCA(list_num*sizeof(MMI_STRING_T));

    if(PNULL == dropdownlist_item_ptr)
    {
        SCI_TRACE_LOW("MMIJava_setting_SetSelectNetFormParam ALLOCA FAILED 2");
        return;
    }

    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        MMI_GetLabelTextByLang(label_str[i], &dropdownlist_item_ptr[i]);
    }

#ifdef WIFI_SUPPORT
    MMI_GetLabelTextByLang(TXT_WIFI, &dropdownlist_item_ptr[i]);
#endif


#if defined(CMCC_UI_STYLE)
    {
        uint32 type = 0;

        CPL_midletInfo_getSuiteInfoItem(cur_suite_id, MI_PREINSTALLED, (void *)&type);

        if(PREINSTALLED == type)
        {
            GUIFORM_SetChildDisplay(MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID,MMIJAVA_NW_SETTING_SIM1_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
#if defined(MMI_MULTI_SIM_SYS_DUAL)|| defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
            GUIFORM_SetChildDisplay(MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID,MMIJAVA_NW_SETTING_SIM2_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
#endif
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            GUIFORM_SetChildDisplay(MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID,MMIJAVA_NW_SETTING_SIM3_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            GUIFORM_SetChildDisplay(MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID,MMIJAVA_NW_SETTING_SIM4_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
#endif
        }
    }
#endif //CMCC_UI_STYLE

    GUIFORM_PermitChildFont(MMIJAVA_NW_SETTING_CHOOSE_NET_FORM_CTRL_ID, FALSE);
    GUILABEL_SetFont(MMIJAVA_NW_SETTING_CHOOSE_NET_TITLE_LABEL_EDIT_ID, MMI_DEFAULT_SMALL_FONT, MMI_DARK_WHITE_COLOR);
    GUILABEL_SetText(MMIJAVA_NW_SETTING_CHOOSE_NET_TITLE_LABEL_EDIT_ID,\
                     &dropdownlist_item_ptr[app_setting_info.nw_type],FALSE);

    s_nw_item_names[0].suite_id = cur_suite_id;
    s_nw_item_names[0].item_num = list_num;
    s_nw_item_names[0].item_cur = app_setting_info.nw_type;
    s_nw_item_names[0].item_ptr = dropdownlist_item_ptr;

    GUIFORM_SetSpace (MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID,&form_space,&form_space);
    GUIFORM_SetMargin(MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID,0);
    GUIFORM_SetDividingLine(MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID, 0, MMITHEME_GetTransparenceColor());
}


/**************************************************************************************************
  * Description: win_func of JBED_NETWORK_CONNECT_SETTING_WIN_TAB
  * Input Paras:
  * Output Paras:
  * note :
  *************************************************************************************************/
LOCAL MMI_RESULT_E MMIJava_setting_HandleNetworkConnectWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E    recode   = MMI_RESULT_TRUE;
#ifdef NETWORKSETTING_FOR_ALLJAVA
    int32           suite_id = CONFIGID;
#else
    int32           suite_id = (int32)MMK_GetWinAddDataPtr(win_id);
#endif
    MMI_CTRL_ID_T   label_edit_id[] =
    {
        MMIJAVA_NW_SETTING_CHOOSE_NET_TITLE_LABEL_EDIT_ID,
        MMIJAVA_NW_SETTING_SIM1_TITLE_LABEL_EDIT_ID,
    #if defined(MMI_MULTI_SIM_SYS_DUAL)|| defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        MMIJAVA_NW_SETTING_SIM2_TITLE_LABEL_EDIT_ID,
    #endif
    #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        MMIJAVA_NW_SETTING_SIM3_TITLE_LABEL_EDIT_ID,
    #endif
    #if defined(MMI_MULTI_SIM_SYS_QUAD)
        MMIJAVA_NW_SETTING_SIM4_TITLE_LABEL_EDIT_ID
    #endif
    };

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_BG_T bg  = {0};

            MMIJAVA_UpdateTitle(STR_CONNECTIVITY_EXT01);
            SCI_MEMSET(s_nw_item_names,0,sizeof(NW_ITEM_NAMES));
        #ifndef MMI_PDA_SUPPORT
            GUIFORM_SetCircularHandleUpDown(MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID,TRUE);
        #endif
        #if !defined(MMI_GUI_STYLE_TYPICAL)
            GUIFORM_SetType(MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID,GUIFORM_TYPE_TP);
        #endif

            //set form background color
            bg.bg_type = GUI_BG_COLOR;
            bg.color = MMI_BLACK_COLOR;
            GUIFORM_SetBg(MMIJAVA_NW_SETTING_FATHER_FORM_CRTL_ID, &bg);

            SCI_TRACE_LOW("HandleJavaNetSelectWinMsg");
            MMIJava_setting_SetSelectNetFormParam(win_id,suite_id);
            MMK_SetAtvCtrl(win_id,MMIJAVA_NW_SETTING_CHOOSE_NET_FORM_CTRL_ID);
        }
        break;

        case MSG_FULL_PAINT:
            break;

        case MSG_GET_FOCUS:
        {
            int i=0;

            MMIJAVA_UpdateTitle(STR_CONNECTIVITY_EXT01);
            for(i = 0; i < sizeof(label_edit_id)/sizeof(label_edit_id[0]); i++)
            {
                GUILABEL_SetText(label_edit_id[i],&(s_nw_item_names[i].item_ptr[s_nw_item_names[i].item_cur]),FALSE);
            }
        }
        break;

        case MSG_KEYLONG_POWER:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        {
            int cur_idx ;

            cur_idx = (MMK_GetActiveCtrlId(win_id) - MMIJAVA_NW_SETTING_CHOOSE_NET_FORM_CTRL_ID)/3;

            /*bug 149465 start*/
            if(0>cur_idx||cur_idx>MMI_DUAL_SYS_MAX) //modify for coverity
            {
                break;
            }
            /*else if(0 == cur_idx)
            {
                uint8 sim_index = 0;
                BOOLEAN has_available_sim = FALSE;

                for(sim_index=MN_DUAL_SYS_1;sim_index<MMI_DUAL_SYS_MAX;sim_index++)
                {
                    if(MMIAPIPHONE_IsSimAvailable(sim_index))
                    {
                        has_available_sim = TRUE;
                    }
                }

                if(!has_available_sim)
                {
                    MMIPUB_OpenAlertFailWin();
                    break;
                }
            }*/
            else if((0 != cur_idx)&&(!MMIAPIPHONE_IsSimAvailable(cur_idx-1)))
            {
                MMIPUB_OpenAlertFailWin(sim_not_ok_str[cur_idx-1]);
                break;
            }
            /*bug 149465 end*/

            MMK_CreateWin((uint32*)JBED_NETWORK_RADIO_WIN_TAB, (ADD_DATA)cur_idx);
        }
        break;

        case MSG_APP_WEB:
        case MSG_APP_OK:
#ifdef MMI_PDA_SUPPORT
        case MSG_APP_MENU:
#endif
        {
            MMK_CloseWin(win_id);
        }
        break;

        case MSG_CLOSE_WINDOW:
        {
            int i=0,j=0;

            SCI_FREEIF(s_nw_item_names[0].item_ptr);
            for(i=1;i<MMI_DUAL_SYS_MAX+1;i++)
            {
                if(s_nw_item_names[i].item_ptr != NULL)
                {
                    for(j=0;j<s_nw_item_names[i].item_num;j++)
                    {
                        SCI_FREEIF(s_nw_item_names[i].item_ptr[j].wstr_ptr);
                    }
                    SCI_FREEIF(s_nw_item_names[i].item_ptr);
                }
            }
            SCI_MEMSET(s_nw_item_names,0,sizeof(NW_ITEM_NAMES));
        }
        break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return(recode);
}

LOCAL MMI_RESULT_E HandleNetworkSelectWin(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    int recode  = MMI_RESULT_TRUE;
    uint16       cur_index = 0;
    uint16       sel_index = 0;
    uint16       max_index = 0;
    int          nw_idx    = 0;
    int          new_index = 0;
    int          j         = 0;
    MMI_CTRL_ID_T ctrl_id  = MMIJAVA_NW_RADIO_CRTL_ID;
    BOOLEAN valid_flag     = TRUE;
    uint32 i               = MN_DUAL_SYS_1;
    JAVA_APP_SETTING_T  app_setting_info = {0};

    MMI_TEXT_ID_T title_id[] =
    {
        STR_JAVA_NETWORK,
        STR_JAVA_SIM1_NETWORK,
    #if defined(MMI_MULTI_SIM_SYS_DUAL)||defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        STR_JAVA_SIM2_NETWORK,
    #endif
    #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_JAVA_NW_SIM3_LABEL,
    #endif
    #if defined(MMI_MULTI_SIM_SYS_QUAD)
         TXT_JAVA_NW_SIM4_LABEL
    #endif
    };

    nw_idx = (int)MMK_GetWinAddDataPtr(win_id);

    max_index = s_nw_item_names[nw_idx].item_num;
    cur_index = s_nw_item_names[nw_idx].item_cur;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMIJAVA_UpdateTitle(STR_JAVA_SELECT);
            GUILIST_SetMaxItem(ctrl_id,max_index,FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            for(j=0;j<max_index;j++)
            {
                MMIAPISET_AppendListItemByTextBuffer(s_nw_item_names[nw_idx].item_ptr[j].wstr_ptr,
                                                     s_nw_item_names[nw_idx].item_ptr[j].wstr_len,
                                                     0,
                                                     IMAGE_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK,
                                                     ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_MS);
            }
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
            SCI_MEMSET(&app_setting_info,0,sizeof(JAVA_APP_SETTING_T));
            MMIJAVA_setting_getAppNetInfo(s_nw_item_names[nw_idx].suite_id,&app_setting_info);
            new_index = GUILIST_GetCurItemIndex(ctrl_id);

            if(0==nw_idx)
            {
                app_setting_info.nw_type = new_index;
            #if defined(WIFI_SUPPORT)
                if(new_index == JAVA_NETWORK_TYPE_WIFI)
                {
                    if(MMIWIFI_STATUS_CONNECTED != MMIAPIWIFI_GetStatus())
                    {
                        MMIAPIWIFI_OpenListWin();
                        break;
                    }
                }
                else
            #endif
                {
                    if(!MMIAPIPHONE_IsSimAvailable(new_index))
                    {
                        valid_flag = FALSE;
                        MMIPUB_OpenAlertWarningWin(sim_not_ok_str[new_index]);
                    }
                }
            }
            else
            {
                app_setting_info.sim_apn[nw_idx-1] = JAVA_NETWORK_TYPE_SIM1 +(new_index - MN_DUAL_SYS_1);
            }

            //write info to file
            if(valid_flag)
            {
                if(MMIJAVA_setting_setAppNetInfo(s_nw_item_names[nw_idx].suite_id,app_setting_info))
                {
                    s_nw_item_names[nw_idx].item_cur = new_index;
                    MMIPUB_OpenAlertSuccessWin(STR_JAVA_OPERATE_SUCCESS);
                    MMK_CloseWin(win_id);
                }
                else
                {
                   MMIPUB_OpenAlertFailWin(STR_JAVA_OPERATE_ERROR);
                }
            }
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


/*==================================*/
//应用设置信息读取接口函数
//1.安装应用完成创建设置信息
//2.读信息函数
//3.改写函数
//4.删除函数

/**************************************************************************************************
  * Description: get app install dir of one midlet
  * Input Paras:
      > suite_id  : unique id of midlet suite
      > path_len :max len of w_path
  * Output Paras:
      > return : TRUE ,w_path is valid,else w_path is invalid
      > w_path : install dir 0f app
  * note :
  *************************************************************************************************/
LOCAL BOOLEAN MMIJava_setting_GetAppPath(IN int32 suite_id,IN int32 path_len,OUT short * w_path)
{
    char    tmp_buff[MMIFMM_PATHNAME_LEN]  = {0};

    SCI_MEMSET(tmp_buff,0,sizeof(tmp_buff));
#ifdef AMS_TCK_RUNNER
    sprintf(tmp_buff,"%stck_.conf",Jbed_getRootPath());
#else
    sprintf(tmp_buff,"%ss%ld_.conf",Jbed_getRootPath(),suite_id);
#endif
    SCI_TRACE_LOW("app setting info path: %s",tmp_buff);

    //GUI_UTF8ToWstr((unsigned short*)w_path, path_len, (uint8*)tmp_buff,strlen(tmp_buff));
    path_len = path_len;
    Jbni_convertUtf8ToUcs2((char*)tmp_buff, strlen(tmp_buff), (unsigned short*)w_path, strlen(tmp_buff)<<1);

    return TRUE;
}

/**************************************************************************************************
  * Description: after installing a suite,create its setting info about network
  * Input Paras:
      > suite_id  : unique id of midlet suite
      > app_setting_info : initial info of suite about network
  * Output Paras:
      > return : TRUE ,create Ok;or else,fail
  * note : call the func when a suite is installed over
  *************************************************************************************************/
LOCAL BOOLEAN MMIJAVA_setting_creatAppNetInfo(IN int32 suite_id)
{
    SFS_HANDLE file_handle  = 0;
    uint32 bytes_written    = 0;
    int32 i                 = 0;
    short setting_file[MMIFMM_PATHNAME_LEN] = {0};
    JAVA_APP_SETTING_T  app_setting_info = {0};
    int32 preferredSim      = -1;

    if(!MMIJava_setting_GetAppPath(suite_id,MMIFMM_PATHNAME_LEN,setting_file))
    {
        return FALSE;
    }

    preferredSim = MMIJAVA_GetPreferredSimId();
    if (preferredSim == -1)
    {
        for(i = MN_DUAL_SYS_1; i < MMIJAVA_GetMaxSimNumber(); i++)
        {
            if(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(i))
            {
                break;
            }
        }
        preferredSim = i;
    }

    if (preferredSim < MMIJAVA_GetMaxSimNumber())
    {
        app_setting_info.nw_type = preferredSim;
        app_setting_info.sim_apn[app_setting_info.nw_type] =
            MMIAPICONNECTION_GetAccountDefaultIndex(preferredSim, MMICONNECTION_ACCOUNT_JAVA);
    }
    else
    {
        //Does not find the correct network setting.
        app_setting_info.nw_type                           = JAVA_NETWORK_TYPE_SIM1;
        app_setting_info.sim_apn[app_setting_info.nw_type] = 0;
    }
    MMIJAVA_setting_setAppNetInfo(suite_id, app_setting_info);

    file_handle = MMIAPIFMM_CreateFile((const unsigned short*)setting_file, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_CREATE_ALWAYS,0,0);
    SCI_TRACE_LOW("MMIJAVA_setting_creatAppNetInfo file handle:%d",file_handle);

    if(file_handle !=0)
    {
        MMIAPIFMM_WriteFile(file_handle, &app_setting_info, sizeof(JAVA_APP_SETTING_T), &bytes_written, 0);
        SCI_TRACE_LOW("MMIJAVA_setting_creatAppNetInfo bytes_written=%d", bytes_written);
        MMIAPIFMM_CloseFile(file_handle);
        return TRUE;
    }

    return FALSE;
}

/**************************************************************************************************
  * Description: get setting info of suite about network
  * Input Paras:
      > suite_id  : unique id of midlet suite
  * Output Paras:
      > return : TRUE ,app_setting_info is valid;or else, invalid
      > app_setting_info : network info of suite network
  * note :
  *************************************************************************************************/
PUBLIC BOOLEAN MMIJAVA_setting_getAppNetInfo(IN int32 suite_id,OUT JAVA_APP_SETTING_T * app_setting_info)
{
    SFS_HANDLE file_handle  = 0;
    uint32 bytes_read       = 0;
    uint16 setting_file[MMIFMM_PATHNAME_LEN] = {0};

    SCI_MEMSET(setting_file,0,sizeof(setting_file));
    if(!MMIJava_setting_GetAppPath(suite_id,MMIFMM_PATHNAME_LEN,(short*)setting_file))
    {
        return FALSE;
    }

    if(!MMIAPIFMM_IsFileExist(setting_file,(uint16)MMIAPICOM_Wstrlen(setting_file)))
    {
        SCI_TRACE_LOW("===myriad log:exec MMIJAVA_setting_creatAppNetInfo()====");
        MMIJAVA_setting_creatAppNetInfo(suite_id);
    }

    file_handle = MMIAPIFMM_CreateFile((const unsigned short*)setting_file,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,0,0);
    SCI_TRACE_LOW("===myriad log:MMIJAVA_setting_getAppNetInfo file handle:%d",file_handle);

    if(file_handle !=0)
    {
        MMIAPIFMM_ReadFile(file_handle, app_setting_info, sizeof(JAVA_APP_SETTING_T), &bytes_read, 0);
        SCI_TRACE_LOW("===myriad log:MMIJAVA_setting_getAppNetInfo bytes_read=%d", bytes_read);
        MMIAPIFMM_CloseFile(file_handle);
        return TRUE;
    }

    return FALSE;
}


/**************************************************************************************************
  * Description: set setting info of suite about network
  * Input Paras:
      > suite_id  : unique id of midlet suite
      > app_setting_info : network info of suite network
  * Output Paras:
      > return : TRUE ,set ok is valid;or else, fail
  * note :
  *************************************************************************************************/
PUBLIC BOOLEAN MMIJAVA_setting_setAppNetInfo(IN int32 suite_id,IN JAVA_APP_SETTING_T app_setting_info)
{
    SFS_HANDLE file_handle  = 0;
    uint32 bytes_written    = 0;
    short setting_file[MMIFMM_PATHNAME_LEN] = {0};

    if(!MMIJava_setting_GetAppPath(suite_id,MMIFMM_PATHNAME_LEN,setting_file))
    {
        return FALSE;
    }

    file_handle = MMIAPIFMM_CreateFile((const unsigned short*)setting_file, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS,0,0);
    SCI_TRACE_LOW("MMIJAVA_setting_setAppNetInfo file handle:%d",file_handle);

    if(file_handle !=0)
    {
        MMIAPIFMM_SetFileSize(file_handle,0);
        MMIAPIFMM_SetFilePointer(file_handle, 0, SFS_SEEK_BEGIN);
        MMIAPIFMM_WriteFile(file_handle, &app_setting_info, sizeof(JAVA_APP_SETTING_T), &bytes_written, 0);
        SCI_TRACE_LOW("MMIJAVA_setting_setAppNetInfo bytes_written=%d", bytes_written);
        MMIAPIFMM_CloseFile(file_handle);
        return TRUE;
    }

    return FALSE;
}

/**************************************************************************************************
  * Description: delete setting info of suite about network
  * Input Paras:
      > suite_id  : unique id of midlet suite
  * Output Paras:
      > return : TRUE ,del ok is valid;or else, fail
  * note : call the func when uninstall a midlet suite
  *************************************************************************************************/
PUBLIC BOOLEAN MMIJAVA_setting_delAppNetInfo(IN int32 suite_id)
{
    short setting_file[MMIFMM_PATHNAME_LEN] = {0};

    if(!MMIJava_setting_GetAppPath(suite_id,MMIFMM_PATHNAME_LEN,(short *)setting_file))
    {
        return FALSE;
    }

    MMIAPIFMM_DeleteFile((unsigned short*)setting_file, PNULL);
    SCI_TRACE_LOW("in MMIJAVA_setting_delAppNetInfo");
    return TRUE;
}

/**************************************************************************************************
  * Description: delete all setting info of all suites about network
  * Input Paras:
  * Output Paras:
      > return : TRUE ,del ok is valid;or else, fail
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC BOOLEAN MMIJAVA_setting_delAllAppNetInfo(void)
{
    wchar filter[]                              = L"*.conf";
    SFS_ERROR_E     result                      = SFS_NO_ERROR;
    SFS_HANDLE      file_handle                 = NULL;
    SFS_FIND_DATA_T find_data                   = {0};
    wchar filename[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    char  file_dir[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    char  char_file[MMIFILE_FULL_PATH_MAX_LEN+1]= {0};

    SCI_TRACE_LOW("====myriad log: MMIJAVA_setting_delAllAppNetInfo() ===");

    SCI_MEMSET(filename,0,sizeof(filename));
    SCI_MEMSET(file_dir,0,sizeof(file_dir));
    sprintf(file_dir,"%s",Jbed_getRootPath());
    //GUI_UTF8ToWstr(filename, MMIFILE_FULL_PATH_MAX_LEN, (uint8*)file_dir,strlen(file_dir));
    Jbni_convertUtf8ToUcs2((char*)file_dir, strlen(file_dir), (unsigned short*)filename, strlen(file_dir)<<1);
    result = SFS_FindFirstFileCtrl(filename,filter,NULL,&file_handle,&find_data,PNULL);

    if (NULL != file_handle && SFS_NO_ERROR == result)
    {
        do
        {
            SCI_MEMSET(char_file,0,sizeof(char_file));
            SCI_MEMSET(file_dir ,0,sizeof(file_dir ));
            SCI_MEMSET(filename ,0,sizeof(filename ));

            //GUI_WstrToUTF8((unsigned char*)char_file,MMIFILE_FULL_PATH_MAX_LEN,find_data.name,MMIAPICOM_Wstrlen(find_data.name));
            Jbni_convertUcs2ToUtf8((uint16*)find_data.name,MMIAPICOM_Wstrlen(find_data.name),(char*)char_file,MMIAPICOM_Wstrlen(find_data.name)<<1);
            sprintf(file_dir,"%s%s",Jbed_getRootPath(),char_file);
            SCI_TRACE_LOW("====myriad log: file name - %s ===",file_dir);
            //GUI_UTF8ToWstr(filename, MMIFILE_FULL_PATH_MAX_LEN, (uint8*)file_dir,strlen(file_dir));
            Jbni_convertUtf8ToUcs2((char*)file_dir, strlen(file_dir), (unsigned short*)filename, strlen(file_dir)<<1);

            if(MMIAPICOM_Wstrlen(filename) + 2 <= MMIFILE_FULL_PATH_MAX_LEN)
            {
                result = MMIAPIFMM_DeleteFile(filename,PNULL);
                //SCI_TRACE_LOW:" ====myriad log: MMIAPIFMM_DeleteFile result (%d)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_SETTING_1221_112_2_18_2_42_2_2,(uint8*)"d",result);
            }
        }
        while (SFS_NO_ERROR == SFS_FindNextFile(file_handle, &find_data));
    }
    if (NULL != file_handle)
    {
        SFS_FindClose(file_handle);
    }

    return TRUE;
}

/**************************************************************************************************
  * Description: MMIJAVA_OpenNetSettingWin
  * Input Paras:
  * Output Paras:
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_OpenNetSettingWin(uint32 suiteid)
{
    SCI_TRACE_LOW("MMIJAVA_OpenNetSettingWin suiteid = %d",suiteid);
    MMK_CreateWin((uint32*)JBED_NETWORK_CONNECT_SETTING_WIN_TAB, (ADD_DATA)suiteid);
}
