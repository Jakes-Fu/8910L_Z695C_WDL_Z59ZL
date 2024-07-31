/****************************************************************************
** File Name:      mmiset_callwin.c                                             *
** Author:                                                                   *
** Date:           15/08/2006                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2006        kelly.li         create
******************************************************************************/

#define _MMISET_CALLWIN_C_


/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_type.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "mmi_nv.h"
#include "guimsgbox.h"
#include "mmk_timer.h"
#include "mmiphone_export.h"
#include "mmiset_security.h"
#include "mmiset_nv.h"
#include "mmiset_id.h"
#include "mmiset_wintab.h"
#include "mmiset_func.h"
#include "mmiset_text.h"
#include "mmicl_export.h"
#include "mmisms_text.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmipb_export.h"//@zhaohui,cr103407
#include "mmifdn_export.h"
#include "mmiidle_statusbar.h"
#include "mmicom_statusarea.h"
#include "guiform.h"
#include "guisetlist.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guidropdownlist.h"
//#include "mmieng.h"
#include "mmieng_export.h"
//#include "mmiset_call.h"
#include "mmiset_call_internal.h"
#include "mmiset_menutable.h"
#ifdef PRODUCT_DM
#include "mn_api_td.h"
#endif
#ifdef ASP_SUPPORT
#include "mmiasp_text.h"
#endif
#ifdef DPHONE_SUPPORT
#include "mmiset_menutable.h"
#endif
#include "ctrlcombox_export.h"
#include "mmisms_export.h"
#include "mmi_appmsg.h"
#include "mmiconnection_export.h"
#include "guirichtext.h"
#include "mmifmm_export.h"
#include "mmicc_text.h"
#include "guistatusbar.h"
#ifdef MCARE_V31_SUPPORT
#include "mcare_interface.h"
#endif
#include "mmimms_export.h"
//#include "mmiset_multisim.h"
#include "mmiset_multisim_internal.h"
#include "mmiidle_export.h"
#include "mmimp3_export.h"
#include "guictrl_api.h"
#include "guitext.h"
#ifndef MONOCHROME_LCD_SUPPORT 
#include "guires.h"
#endif
#include "mmipb_text.h"
#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif
#include "mmicc_internal.h"
#ifdef MMI_SMART_DUAL_SIM_SUPPORT
#include "smartdualsim_text.h"
#include "smartdualsim_export.h"
SmartDS_Forwarding_Direction SmartDS_Forwarding_setting = SMARTDS_FORWARDING_NONE;
PUBLIC void MMISET_CloseSmartDualSIMCFNRCWin(MN_DUAL_SYS_E dual_sys, BOOLEAN  is_action_success, MN_SS_ERROR_CODE_E  error_code);
#endif
//[block list in call setting]
#include "mmicalendar_export.h"
//[end]
#include "mmicom_banner.h"
#include "mn_error.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMISET_CALL_BAR_OPEN,
    MMISET_CALL_BAR_CLOSE,
    MMISET_CALL_BAR_CANCEL_ALL,
    MMISET_CALL_BAR_MODIFY_PWD,
    MMISET_CALL_BAR_MAX
}MMISET_CALL_BAR_E;
typedef enum
{
    MMISET_CALL_CLIR_INVOCATION_E,
        MMISET_CALL_CLIR_SUPPRESSION_E,
        MMISET_CALL_CLIR_NETWORK_DECIDE_E,
        MMISET_CALL_CLIR_MAX
}MMISET_CLL_CLIR_E;

typedef enum
{
    NETWORK_TYPE_DM_E,        // 自动双模T
    NETWORK_TYPE_DM_G_E,        // 自动双模G
    NETWORK_TYPE_GPRS_E,   //  2g单模
    NETWORK_TYPE_TD_E,        // 3g单模
    NETWORK_TYPE_MAX
}NETWORK_TYPE_E;

#ifdef MMI_CSP_SUPPORT
typedef enum
{
    CSP_MENU_NULL,
    CSP_MENU_CF, //call fowarding
    CSP_MENU_CFU, // cf unconditional
    //CSP_MENU_CFC, //cf conditional including cfb, cfnry, cfnrc
    CSP_MENU_CFB, // cf busy
    CSP_MENU_CFNRY, // cf no reply
    CSP_MENU_CFNRC, // cf no reachable
    CSP_MENU_CB, //call barring
    CSP_MENU_BAOC, //cb outgoing
    CSP_MENU_BAIC, // cb incomming
    CSP_MENU_BICROAM, //cb roaming
    CSP_MENU_BOIC, //cb international outgoing
    CSP_MENU_BOIC_EXHC, //boic except outgoing to home plmn 
    CSP_MENU_MAX
}CSP_MENU_SS_TYPE_E;

typedef enum
{
    CSP_MENU_OPER_NULL,
    CSP_MENU_OPER_ACTIVE,
    CSP_MENU_OPER_ACTIVE_WITH_PB,
    CSP_MENU_OPER_ACTIVE_WITH_VM,
    CSP_MENU_OPER_ACTIVE_WITH_NO,
    CSP_MENU_OPER_CANCEL,
    CSP_MENU_OPER_INTEROGATE,
    CSP_MENU_OPER_CANCEL_ALL,
    CSP_MENU_OPER_MODIFY_PASSWORD,
    CSP_MENU_OPER_MAX
}CSP_MENU_SS_OPER_E;
#endif


//@maggie add end   for cr79064

#define MAKE_BREW_RGB(r,g,b) ((r >> 3)<<11 | (g>>2)<<5 | (b>>3))


#define   MMISET_SEARCHNETWORK_SPACE_TIME   3000    //两次尝试切网间的空歇时间，3秒
//@maggie add begin for cr79064
#define     MMISET_SET_MAX_NETWORK_TYPE             4//@zhaohui modify for cr94108
#define       MMISET_SETRAT_MAX_NUM     7//尝试切网的次数

#define MMISET_FULLSCREEN_CLIENT_BOTTOM  (MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)
#define MMISET_FULLSCREEN_CLIENT_RIGHT   (MMI_MAINSCREEN_WIDTH - 1)

//多国语言下，大字体显示，18/20号字体

#define     MMISET_LABEL_HEIGH                      MMI_DEFAULT_FONT_HEIGHT
#define     MMISET_SS_INTERROGATE_NAME_BOTTOM       (MMISET_LABEL_HEIGH + MMI_CLIENT_RECT_TOP)
#define     MMISET_SS_INTERROGATE_STATUS_BOTTOM     (MMISET_SS_INTERROGATE_NAME_BOTTOM + MMISET_LABEL_HEIGH)
#define     MMISET_SS_INTERROGATE_NUMBER_BOTTOM     (MMISET_SS_INTERROGATE_STATUS_BOTTOM + MMISET_LABEL_HEIGH)

#define MMISET_USSD_SELECT_SIM_MAX_NUM        MMI_DUAL_SYS_MAX
// vt set record path
#define MMISET_VT_RECORD_HEIGHT 25
//rv & da
#define MMISET_VT_RECORD_RV_DA_LABEL_LEFT 10
#define MMISET_VT_RECORD_RV_DA_LABEL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_RV_DA_LABEL_TOP (MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT + 5)
#define MMISET_VT_RECORD_RV_DA_LABEL_BOTTOM (MMISET_VT_RECORD_RV_DA_LABEL_TOP + MMISET_VT_RECORD_HEIGHT)

#define MMISET_VT_RECORD_RV_DA_DL_LEFT 10
#define MMISET_VT_RECORD_RV_DA_DL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_RV_DA_DL_TOP (MMISET_VT_RECORD_RV_DA_LABEL_BOTTOM + 10)
#define MMISET_VT_RECORD_RV_DA_DL_BOTTOM (MMISET_VT_RECORD_RV_DA_DL_TOP + MMISET_VT_RECORD_HEIGHT)
//da
#define MMISET_VT_RECORD_DA_LABEL_LEFT 10
#define MMISET_VT_RECORD_DA_LABEL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_DA_LABEL_TOP (MMISET_VT_RECORD_RV_DA_DL_BOTTOM + 10)
#define MMISET_VT_RECORD_DA_LABEL_BOTTOM (MMISET_VT_RECORD_DA_LABEL_TOP + MMISET_VT_RECORD_HEIGHT)

#define MMISET_VT_RECORD_DA_DL_LEFT 10
#define MMISET_VT_RECORD_DA_DL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_DA_DL_TOP (MMISET_VT_RECORD_DA_LABEL_BOTTOM + 10)
#define MMISET_VT_RECORD_DA_DL_BOTTOM (MMISET_VT_RECORD_DA_DL_TOP + MMISET_VT_RECORD_HEIGHT)
//rv
#define MMISET_VT_RECORD_RV_LABEL_LEFT 10
#define MMISET_VT_RECORD_RV_LABEL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_RV_LABEL_TOP (MMISET_VT_RECORD_DA_DL_BOTTOM + 10)
#define MMISET_VT_RECORD_RV_LABEL_BOTTOM (MMISET_VT_RECORD_RV_LABEL_TOP + MMISET_VT_RECORD_HEIGHT)

#define MMISET_VT_RECORD_RV_DL_LEFT 10
#define MMISET_VT_RECORD_RV_DL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_RV_DL_TOP (MMISET_VT_RECORD_RV_LABEL_BOTTOM + 10)
#define MMISET_VT_RECORD_RV_DL_BOTTOM (MMISET_VT_RECORD_RV_DL_TOP + MMISET_VT_RECORD_HEIGHT)

#define MMISET_VT_RECORD_DL_NUM 2 //dropdownlist的下拉数目“话机”“存储卡”

//vt local video setting
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_LEFT MMISET_VT_RECORD_RV_DA_LABEL_LEFT
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_RIGHT MMISET_VT_RECORD_RV_DA_LABEL_RIGHT
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_TOP MMISET_VT_RECORD_RV_DA_LABEL_TOP
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_BOTTON MMISET_VT_RECORD_RV_DA_LABEL_BOTTOM

#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_LEFT MMISET_VT_RECORD_RV_DA_DL_LEFT
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_RIGHT MMISET_VT_RECORD_RV_DA_DL_RIGHT
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_TOP MMISET_VT_RECORD_RV_DA_DL_TOP
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_BOTTOM MMISET_VT_RECORD_RV_DA_DL_BOTTOM

#define MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM MMISET_VT_RECORD_DL_NUM

#define MMISET_VT_REPLACE_PIC_SIZE_MAX (32*1024)
#define MMISET_VT_REPLACE_PIC_WIDTH_MAX 320
#define MMISET_VT_REPLACE_PIC_HEIGHT_MAX 240

#define MMISET_PLMN_DIGITAL_MAX_LEN             (6)
#define MMISET_PLMN_PRIORITY_MAX_LEN             (3)
#define MMISET_CALLSET_ITEM_NUM_MAX             (3)

#ifdef MMI_PREFER_PLMN_SUPPORT
//记录当前操作的是哪一条plmn，主要用于优选网络列表中的属性修改。。。
LOCAL uint16 s_prefer_network_item = 0;
//记录优选网络列表更新的list
LOCAL MN_PHONE_PLMN_SEL_LIST_T *s_update_prefer_list_ptr = PNULL; 
#endif
#ifdef MMI_MULTISIM_COLOR_SUPPORT

#define SIM_COLOR_BUTTON_RECT_HEIGHT   44       //Button height
#define SIM_COLOR_BUTTON_RECT_WIDTH    49       //Button height
#define SIM_COLOR_BUTTON_FORM_HEIGHT   50       //button form height
#define SIM_COLOR_BUTTON_LINE_COUNT     2
typedef struct 
{
    MMI_CTRL_ID_T   button_line_form_id[SIM_COLOR_BUTTON_LINE_COUNT];
    MMI_CTRL_ID_T   button_ctrl_id[MAX_SIM_COLOR_NUM];
}SIMCOLOR_CTRL_LIST_T;
#endif

#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define SET_MINUTE_REMINDER_LABLE_TOP_MARGIN     1
#define SET_MINUTE_REMINDER_LABLE_HEIGHT     14
#else
#define SET_MINUTE_REMINDER_LABLE_TOP_MARGIN     5
#define SET_MINUTE_REMINDER_LABLE_HEIGHT     22
#endif

#ifdef MMI_MULTISIM_COLOR_SUPPORT
#define DEFAULT_BTN_BORDER  {1,MMI_GRAY_COLOR,GUI_BORDER_NONE}
#define SELECTED_BTN_BORDER {3,GUI_RGB2RGB565(255, 100, 0),GUI_BORDER_SOLID}
#endif

#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMISET_MINUTE_REMINDER_REMAIN_TOP_MARGIN  1
#define MMISET_MINUTE_REMINDER_REMAIN_HEIGHT      22
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern MN_PHONE_PLMN_DETAIL_T  *g_plmn_detail_list_ptr;//选择网络的列表
extern MMISET_SS_PWD_INFO_T g_mmiset_ss_pwd;
//ljg add by voice mail number
extern BOOLEAN s_is_vmn_from_sim1;
extern BOOLEAN s_is_vmn_from_sim2;
extern MMI_PARTY_NUMBER_T g_temp_sim_vmnum_info;
extern MMI_ALPHA_ID_T g_temp_sim_alpha_info;

extern MMI_PARTY_NUMBER_T g_sim_vmnum_info;
extern MMI_PARTY_NUMBER_T g_sim2_vmnum_info;
extern MMI_ALPHA_ID_T g_sim_alpha_info;
extern MMI_ALPHA_ID_T g_sim2_alpha_info;
PUBLIC BOOLEAN s_is_invalid_cnf_msg = FALSE;
PUBLIC BOOLEAN s_is_invalid_cnf2_msg = FALSE;
//end

#ifndef MMI_MULTI_SIM_SYS_SINGLE
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
extern MMISET_SS_STATUS_T g_mmiset_ss_status;/*lint !e752*/
#endif
#endif
PUBLIC MMI_HANDLE_T MMISmartDS_OpenNetworkWaittingWin(void);

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*lint -esym(552,g_vmn_oper_type) */
PUBLIC MMISMS_VMN_OPER_TYPE_E  g_vmn_oper_type = 0; /*lint !e64*/  //查询语音信箱号是否为了通话转移

#if defined(MMI_DUALMODE_ENABLE) ||defined (MMI_2G_4G_SUPPORT)
LOCAL BOOLEAN is_rat_switching = FALSE;
//LOCAL MN_GMMREG_RAT_E s_current_sel_rat = MN_GMMREG_RAT_GPRS;//@$discuss,是否需要加宏？
#endif

LOCAL BOOLEAN s_allow_rat_switching = TRUE;

LOCAL BOOLEAN s_allow_set_prefer_plmn = TRUE;
MMI_STRING_T   new_password_value = {0};
MMI_STRING_T   new_password_again_value = {0};

LOCAL BOOLEAN s_is_wizard_cfnrc = FALSE;
LOCAL uint16 s_re_switchrat_count = 0;//切网重试的次数
LOCAL uint8 s_searchnetwork_space_timer_id = 0;//重试切网间隔时间的Timer
LOCAL MN_PHONE_PLMN_STATUS_E s_plmn_status_before_manu_search = PLMN_NORMAL_GSM_GPRS_BOTH;//@fen.xie MS00174570
#ifdef MMI_IP_CALL_SUPPORT
LOCAL uint16 s_ipnumber_list_cur_id = 0;
LOCAL BOOLEAN s_is_add_ipnumber = FALSE; // true添加，false修改
#endif
LOCAL MMISET_CALL_BAR_E s_call_bar_operation = MMISET_CALL_BAR_MAX;
PUBLIC MN_DUAL_SYS_E s_cur_dual_sys = 0;
PUBLIC  uint32  s_call_divert_conditions = 0;
PUBLIC  SET_ID_MENU_E s_call_barr_menu_id =	0;

LOCAL MMI_CTRL_ID_T s_set_divert_bar_wating_hideid_ctrl_id = MMISET_CALLDIVERT_FORM_CTRL_ID;
LOCAL uint8 g_call_reminder_time[CC_CALL_REMINDER_TIME_MAX] = 
{
    CC_CALL_REMINDER_TIME_OFF,
    CC_CALL_REMINDER_TIME_50S,
    CC_CALL_REMINDER_TIME_55S,
    CC_CALL_REMINDER_TIME_60S
};

/* cr00185468: User can change wallpaper number freely */
//MMI_IMAGE_ID_T g_fixed_pic_wallpaper[MMISET_WALLPAPER_FIXED_PIC_MAX_NUM] = {0};

#ifdef VT_SUPPORT
//vt replace 
LOCAL const MMI_TEXT_ID_T s_replace_title_text_id[MMISET_VT_REPLACE_MAX] = {TXT_SET_VT_REPLACE_LOCAL_IMAGE, TXT_SET_VT_REPLACE_REMOTE_IMAGE};
LOCAL const MMISET_VT_REPLACE_MEDIA_TYPE_E s_replace_media_type[MMISET_VT_REPLACE_MAX] = {MMISET_VT_REPLACE_MEDIA_TYPE_IMAGE, MMISET_VT_REPLACE_MEDIA_TYPE_IMAGE};
//LOCAL MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
#endif

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
LOCAL BOOLEAN s_is_cta_deactive_ps[MMI_DUAL_SYS_MAX] = {0};
LOCAL uint8 s_cta_deactive_ps_timer = 0;
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
LOCAL int32 s_dualsys_cfnrc_win_close_time = 0;
LOCAL int32 s_dualsys_cfnrc_success_num = 0;
LOCAL BOOLEAN  s_is_action_dualsys_cfnrc  = FALSE; //记录是否正在进行双卡无法到达转移
#endif

#ifdef MMI_MULTISIM_COLOR_SUPPORT
//Ctrl ID list
LOCAL const SIMCOLOR_CTRL_LIST_T simColor_CtrlID=
{
    {
        MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID,  //BTN_form_id
        MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID,
     },
    {                                               //BTN_ctrl_id[MAX_SIM_COLOR_NUM]    
        MMISET_SIM_COLOR_BTN_COLOR1_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR2_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR3_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR4_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR5_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR6_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR7_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR8_CTRL_ID,
    },
};

LOCAL uint16  s_current_selected_index = 0;
#endif
//[block list in call setting]
#if 0//defined(MMI_BLACKLIST_SUPPORT)    //xiongkai
extern MMISET_BLACKLIST_T *g_blacklist_ptr;
LOCAL MMIPB_BCD_NUMBER_T *s_new_blacklist_num_ptr = PNULL;
#endif
//[end]
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void CreateSimSelectionMenu2(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id);

                                              

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
/*****************************************************************************/
//  Description : ImplementStandbyChanged
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ImplementStandbyChanged(BOOLEAN * is_standby_ptr, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : AppendMultiSysTypeList
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AppendMultiSysTypeList(MMI_CTRL_ID_T ctrl_id, BOOLEAN is_update);
PUBLIC void MMISET_GetVoicemailFromNdtAndMultivar(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//  Description : to handle multi sys time window msg
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMultiSysTypeWindow(
                                               MMI_WIN_ID_T       win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );

/*****************************************************************************/
//  Description : to handle sim active type when all sim is set to on type
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMajorSimWhenAllSimOnWindow(
                                                        MMI_WIN_ID_T       win_id, 
                                                        MMI_MESSAGE_ID_E   msg_id, 
                                                        DPARAM             param
                                                        );

/*****************************************************************************/
//  Description : to handle answer with original sim type
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAnwerWithOriginalSimWindow(
                                                        MMI_WIN_ID_T       win_id, 
                                                        MMI_MESSAGE_ID_E   msg_id, 
                                                        DPARAM             param
                                                        );

/*****************************************************************************/
//  Description : 处理SIM SET NAME WIN MESSAGE
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditSimNameWindow(
                                           MMI_WIN_ID_T          win_id,    // 窗口的ID
                                           MMI_MESSAGE_ID_E         msg_id,     // 窗口的内部消息ID
                                           DPARAM               param       // 相应消息的参数
                                           );

#ifdef MMI_MULTISIM_COLOR_SUPPORT
/*****************************************************************************/
//  Description :  Reset button border 
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  ResetSimColorButtonBorder(MN_DUAL_SYS_E   dual_sys);

/*****************************************************************************/
//  Description :  Color button on click
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ColorButtonOnClick(MMI_WIN_ID_T  win_id,MN_DUAL_SYS_E   dual_sys,BOOLEAN is_selected);

/*****************************************************************************/
//  Description : Init ctrl settings
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E InitEditSimColorWindow(MMI_WIN_ID_T win_id,MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : 处理SIM SET Color WIN MESSAGE
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditSimColorWindow(
                                           MMI_WIN_ID_T          win_id,    // 窗口的ID
                                           MMI_MESSAGE_ID_E         msg_id,     // 窗口的内部消息ID
                                           DPARAM               param       // 相应消息的参数
                                           );
#endif

#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
//  Description : open dual sim set cfnrc window
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL void OpenDualsysCFNRCWin(void);

/*****************************************************************************/
//  Description : 处理dual sim set cfnrc
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDualsysCFNRCWindow(
                                            MMI_WIN_ID_T          win_id,   // 窗口的ID
                                            MMI_MESSAGE_ID_E        msg_id,     // 窗口的内部消息ID
                                            DPARAM              param       // 相应消息的参数
                                            );

/*****************************************************************************/
//  Description : open cfnrc win
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL void OpenCFNRCWin(void);
#endif

/*****************************************************************************/
//  Description : 处理triple sim set cfnrc
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMultiSimSettingWindow(
                                              MMI_WIN_ID_T          win_id,     // 窗口的ID
                                              MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                              DPARAM                param       // 相应消息的参数
                                              );

/*****************************************************************************/
//  Description : AppendPinSelectListItem
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AppendPinSelectListItem(MMISET_SIM_SELECT_TYPE_E select_type, MMI_CTRL_ID_T ctrl_id);
#endif

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : to handle CTA Multi Sys Standby window msg
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCTAMultiSysStandbyWindow(
                                            MMI_WIN_ID_T       win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );
/*****************************************************************************/
//  Description : AppendCTAMultiSysTypeList
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AppendCTAMultiSysTypeList(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : ImplementCTAStandbyChanged
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ImplementCTAStandbyChanged(BOOLEAN * is_standby_ptr, BOOLEAN is_power_select);



/*****************************************************************************/
//  Description : ImplementCTASendSIMNotReadInd
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void ImplementCTASendSIMNotReadInd(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Standby 等待窗口
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCTAStandbyWaitingWindow (
                                           MMI_WIN_ID_T      win_id,    // 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id,     // 窗口的内部消息ID
                                           DPARAM               param       // 相应消息的参数
                                           );
#endif
#endif




                                                                                                                                                                                          
/*****************************************************************************/
//  Description : to handle divert to voice box
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDivertVoiceWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );


/*****************************************************************************/
//  Description : to open bar settings main menu window
//  Global resource dependence : none
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void OpenBarSettingsWindow(void);

//@zhaohui add
/*****************************************************************************/
//  Description : to handle select divert call
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
#ifdef VT_SUPPORT
                                              
LOCAL MMI_RESULT_E HandleSelectCallTypeGeneralWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );                                            
#endif

/*****************************************************************************/
//  Description : set call diver voice or num title
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void SetCallDivertVoiceOrNumTitle(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : to handle open num divert call
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOpenNumDivertWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

/*****************************************************************************/
//  Description : get voice mail address
//  Global resource dependence : 
//  Author:bown.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void GetVoiceMailAddress(MN_DUAL_SYS_E dual_sys, MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type, MN_VOICE_MAIL_TYPE_E voice_mail_type);
PUBLIC void SetVoiceMailAddress(MN_DUAL_SYS_E dual_sys, MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type, MMI_STRING_T* num_string);
/*****************************************************************************/
//  Description : to handle divert to phone number
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDivertNumberWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );

/*****************************************************************************/
//  Description : to handle select popo win message
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectPopBarWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
//  Description : to handle select popo win message
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/                                             
LOCAL MMI_RESULT_E  HandleSelectPopWaitingWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
//  Description : to handle select popo win message
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/                                             
LOCAL MMI_RESULT_E  HandleSelectPopDivertWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void CreateCallWaitingtSelectPopWindow(void);    


/*****************************************************************************/
//  Description : to handle call divert/bar/wating/hide id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallDivertWindow(
                                                          MMI_WIN_ID_T    win_id, 
                                                          MMI_MESSAGE_ID_E   msg_id, 
                                                          DPARAM             param
                                                          );
//add by ljg
LOCAL MMI_RESULT_E  HandleCallDivertConditionsWin(
                                                          MMI_WIN_ID_T    win_id, 
                                                          MMI_MESSAGE_ID_E   msg_id, 
                                                          DPARAM             param
                                                          );

LOCAL MMI_RESULT_E  HandleCallDivertSimCardWin(
                                                          MMI_WIN_ID_T    win_id, 
                                                          MMI_MESSAGE_ID_E   msg_id, 
                                                          DPARAM             param
                                                          );
LOCAL MMI_RESULT_E  HandleCallWaitingWindow(
                                                          MMI_WIN_ID_T    win_id, 
                                                          MMI_MESSAGE_ID_E   msg_id, 
                                                          DPARAM             param
                                                          );
LOCAL MMI_RESULT_E HandleVoiceMailSIMSelectWinMsg(
														  MMI_WIN_ID_T	  win_id, 
														  MMI_MESSAGE_ID_E	 msg_id, 
														  DPARAM			 param
														  );
extern	void CreateSimSelectionMenu(
                                MMI_WIN_ID_T win_id,
                                MMI_CTRL_ID_T ctrl_id);

LOCAL void MMICC_OpenVoicemailSettingSelectSimWin(void);

LOCAL MMI_RESULT_E HandleFixedDialingWindow(
														  MMI_WIN_ID_T	  win_id, 
														  MMI_MESSAGE_ID_E	 msg_id, 
														  DPARAM			 param
														  );

LOCAL MMI_RESULT_E HandleFixedDialingSubWindow(
														  MMI_WIN_ID_T	  win_id, 
														  MMI_MESSAGE_ID_E	 msg_id, 
														  DPARAM			 param
														  );

LOCAL MMI_RESULT_E HandleFixedDialingSwitchWindow(
														  MMI_WIN_ID_T	  win_id, 
														  MMI_MESSAGE_ID_E	 msg_id, 
														  DPARAM			 param
														  );

LOCAL MMI_RESULT_E  HandleSettingsEditVoiceMailNumberMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );

LOCAL MMI_RESULT_E  HandleSettingsEditVoiceMailNumberMsgCallIdle(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );
/*****************************************************************************/
//  Description : handle call bar window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/                                                   
LOCAL MMI_RESULT_E  HandleCallBarWindow(
                                                          MMI_WIN_ID_T    win_id, 
                                                          MMI_MESSAGE_ID_E   msg_id, 
                                                          DPARAM             param
                                                          );
                                                          

/*****************************************************************************/
// Description : to handle set call hide id window
// Global resource dependence : 
// Author:wancan.you
// Note: MMISET_SET_CALL_HIDE_ID_WIN_TAB
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCallHideIdWindow(
                                              MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id,
                                              DPARAM            param
                                              ); 
                                              
/*****************************************************************************/
//  Description : to handle input call bar password
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBarPasswordWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );  
                                            
                                                                                       
/*****************************************************************************/
//  Description : to handle register new call bar password again
//  Global resource dependence : 
//  Author:maql
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRegisterNewBarPasswordAgainWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       );  
                                                       
/*****************************************************************************/
//  Description : to select network
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectNetworkWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSelectLineVisibleByDualSys(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsLockLineVisibleByDualSys(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetSelectLineVisibleSimNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetLockLineVisibleSimNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetCallFowardVisibleSimNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetCallBarVisibleSimNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide cf cb menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSSMenuItemVisible(MN_DUAL_SYS_E dual_sys, CSP_MENU_SS_TYPE_E ss_type, CSP_MENU_SS_OPER_E ss_oper);

/*****************************************************************************/
// Description : set line lock
// Global resource dependence : 
// Author: samboo.shen
// Note:
/*****************************************************************************/
LOCAL void  SetLineLock(void);

/*****************************************************************************/
//  Description : to handle select line msg
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectLineWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM               param
                                           );  

/*****************************************************************************/
//      Description : to handle lock line msg
//     Global resource dependence : 
//  Author:samboo.shen
//     Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLockLineWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E      msg_id,
                                           DPARAM                   param
                                           );
#endif //MMI_CSP_SUPPORT

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
//  Description : handle network setting window msg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetworkSettingWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : handle prefer network window msg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDispPreferNetworkListWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//      Description : HandleDeletePreferPlmnQueryWindow
//     Global resource dependence : 
//  Author: 
//     Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeletePreferPlmnQueryWindow( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              );

/*****************************************************************************/
//  Description : handle prefer network options menu msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetworkOptionMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : handle plmn list display window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePlmnListDispWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : handle plmn list display window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCreatePlmnItemWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : handle plmn list display window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetworkPriorityChangeWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : handle plmn list display window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetWaitingWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : InitPlmnListCtrl
//  Global resource dependence : 
//  Author:michael.shi
//  Note: InitPlmnListCtrl
/*****************************************************************************/
LOCAL void InitPlmnListCtrl(MMI_CTRL_ID_T ctrl_id,uint16 max_item_num);

#if 0
/*****************************************************************************/
//  Description : is plmn already exists in prefer network list.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN isPlmnExistsAlready(MN_DUAL_SYS_E dual_sys,MN_PLMN_T plmn);
#endif

/*****************************************************************************/
//  Description : get prefer network list num.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetCurPreferNetworkIndex(void);

/*****************************************************************************/
//  Description : get prefer network list num.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL void SetCurPreferNetworkIndex(uint16 index);

/*****************************************************************************/
//  Description : isPlmnValid ,
//  Global resource dependence : 
//  Author:michael.shi
//  Note: if plmn is in list ,return FALSE; if plmn is not in list ,return TRUE;
/*****************************************************************************/
LOCAL BOOLEAN isPlmnValid(MN_DUAL_SYS_E dual_sys, MN_PLMN_T plmn);
#endif
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
//  Description : list ip number window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleListIpNumberWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );
                                             


/*****************************************************************************/
//  Description : modify ip number window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleModifyIPNumberWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );

/*****************************************************************************/
//  Description : append ip number item to listbox
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL void AppendIpNumberItemToList(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMI_CTRL_ID_T ctrl_id
                                    );

/*****************************************************************************/
//  Description : append listbox item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendIpNumberItem(
                              MMI_CTRL_ID_T                 ctrl_id,
                              const MMISET_IP_NUMBER_INFO_T *ip_info_ptr,
                              uint16                        pos,
                              BOOLEAN                       is_update
                              );

/*****************************************************************************/
//  Description : handle ip number option menu window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIpNumberOptionWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );
#if defined(MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : handle ip number add option menu window
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIpNumberAddOptionWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );
#endif
#endif
/*****************************************************************************/
//  Description : 处理分钟提示音 第一个界面
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMinuteReminderFirstWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  );
/*****************************************************************************/
//  Description : 处理分钟提示音
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMinuteReminderWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  );
/*****************************************************************************/
//  Description : 自定义分钟提示音窗口
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetMinuteReminderEditWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  );


/*****************************************************************************/
//  Description : 在进行手动选网中，网络连接窗口的处理函数
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleManualSelNetworkConnectWindow(
                                                       MMI_WIN_ID_T       win_id,   // 窗口的ID
                                                       MMI_MESSAGE_ID_E   msg_id,   // 窗口的内部消息ID
                                                       DPARAM             param     // 相应消息的参数
                                                       );

/*****************************************************************************/
//  Description : to handle remind user will select network through manual win msg
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelNetManualWinMsg(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );
#if defined (MMI_DUALMODE_ENABLE) || defined(MMI_2G_4G_SUPPORT)


/*****************************************************************************/
//  Description : to select network method
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectNetworkMethodWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    );
#endif

#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : to handle VT call setting window message
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTSettingsWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

/*****************************************************************************/
//  Description : to handle VT call fallback setting window message
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTFallbackSettingsWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   );

/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTReplaceImageSettingsWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       );

/*****************************************************************************/
//  Description : 判断替代图片格式是否有效
//  Global resource dependence : none
//  Author: samboo
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsReplaceImageValid(MULTIM_SELECT_RETURN_T * multim_select_ptr);

/*****************************************************************************/
//  Description : 预览自定义图片窗口
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReviewCustomPhotoWinMsg(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 );

/*****************************************************************************/
//  Description : 预览自定义图片窗口
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReviewCustomPhotoOptWinMsg
(
 MMI_WIN_ID_T        win_id, 
 MMI_MESSAGE_ID_E    msg_id, 
 DPARAM              param
 );
/*****************************************************************************/
//  Description : 设置VT录音存储位置
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTRecordPathSetWinMsg(
                                               MMI_WIN_ID_T        win_id,
                                               MMI_MESSAGE_ID_E    msg_id,
                                               DPARAM              param
                                               );

/*****************************************************************************/
//  Description : 初始化VT录制位置设置中的控件
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetVTRecordPathSettings(MMIFILE_DEVICE_E *device_table_ptr);


/*****************************************************************************/
//  Description : 初始化VT录制位置设置中的控件
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SaveVTRecordPathSettings(MMIFILE_DEVICE_E rvda_device, 
                                    MMIFILE_DEVICE_E da_device,
                                    MMIFILE_DEVICE_E rv_device);

/*****************************************************************************/
//  Description : to handle vt one time ring setting win
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTOneTimeRingSettingsWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );

/*****************************************************************************/
//  Description : AppendOneTimeRingListItem
//  Global resource dependence : 
//  Author: samboo
//  Note:
/*****************************************************************************/ 
LOCAL void AppendOneTimeRingListItem(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : SetOneTimeRing
//  Global resource dependence : 
//  Author: samboo
//  Note:
/*****************************************************************************/
LOCAL void SetOneTimeRing(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 设置VT本地视频位置
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTLocalVideoSetWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id,
                                           DPARAM              param
                                           );

#if 0//bug337002
/*****************************************************************************/
//  Description : 初始化VT本地视频设置中的控件
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void SetVTLocalVideoSettings(void);

/*****************************************************************************/
//  Description : 初始化VT本地视频设置中的控件
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void SaveVTLocalVideoSettings(void);
#endif

#if 1//bug337002
/*****************************************************************************/
//  Description : to handle set Local Video
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTLocalVideoSetSelectWinMsg(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
//  Description : create screensaver window
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void MMISETAPI_CreateRemoteShutdownCameraSetWin(void);
#endif
#endif  

/*****************************************************************************/
//  Description : 处理自动重拨
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAutoRedialWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
LOCAL void MMISET_AppendSpeedDialDelayListItem(
                                               MMI_CTRL_ID_T        ctrl_id
                                               );

/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
LOCAL void SetSpeedDialDelay(
                              MMI_CTRL_ID_T     ctrl_id
                              );

/*****************************************************************************/
//  Description : 延时拨号处理
//  Global resource dependence : 
//  Author:yiwen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSpeedDialWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  );

/*****************************************************************************/
//  Description : to HandleSetCallSetOtherWindow
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleSetCallSetOtherWindow(
                                               MMI_WIN_ID_T       win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );  
                                               
                                          
/*****************************************************************************/
// Description : to handle set fly mode window msg idle
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeWindow(
                                        MMI_WIN_ID_T      win_id,
                                        MMI_MESSAGE_ID_E  msg_id,
                                        DPARAM            param
                                        );
                                        

/*****************************************************************************/
// Description : to handle set fly mode window msg
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeSetWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E  msg_id,
                                           DPARAM            param
                                           );
//@maggie add begin for cr79064
/*****************************************************************************/
//  Description : HandleNetworkTypeWindow
//  Global resource dependence : 
//  Author:maggie.ren
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetworkTypeWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );

#if defined(MMI_GUI_STYLE_TYPICAL)
/*****************************************************************************/
//  Description : handle set any key and reject mode win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAnykeyRejectWindow(
                                                MMI_WIN_ID_T    win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                );
#endif

/*****************************************************************************/
//  Description : handle set connect prompt remind win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetConnectPromptWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 );

#if 0
/*****************************************************************************/
//  Description : InsertOptMenuNodeByLabel
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void InsertOptMenuNodeByLabel(
                                    MMI_CTRL_ID_T      ctrl_id,        //控件id
                                    MMI_TEXT_ID_T      text_id,        //插入的节点的文本
                                    uint16             node_id,        //节点编号
                                    uint16             parent_node_id, //父节点编号
                                    uint16             index           //位置
                                    );                                                 
 #endif
 
/*****************************************************************************/
//  Description : 语音信箱号码界面打开时，等待界面不允许返回
//  Global resource dependence : 
//  Author: kelly.li
//  Note:2006-02-13
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVMForDivertWaitingWindow(
                                                  MMI_WIN_ID_T      win_id,     // 窗口的ID
                                                  MMI_MESSAGE_ID_E     msg_id,  // 窗口的内部消息ID
                                                  DPARAM                param       // 相应消息的参数
                                                  ); 
#ifdef FLIP_PHONE_SUPPORT
/*****************************************************************************/
//  Description : 翻盖接听
//  Global resource dependence : 
//  Author:jibin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetFlipAnswerWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              ); 
#endif             
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
//  Description : Is this ip number active
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
BOOLEAN IsActiveIPNumber(MN_DUAL_SYS_E dual_sys,uint16 cur_index);
#endif
/*****************************************************************************/
//  Description : to select no answer time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNoAnswerTimeWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );   
                                             
/*****************************************************************************/
//     Description : add fly mode set list to list 
// Global resource dependence : 
//  Author: kelly.li
// Note:
/*****************************************************************************/
LOCAL void AppendFlyModeSetListItem(
                                    MMI_CTRL_ID_T      ctrl_id
                                    );  
                                    

/*****************************************************************************/
//     Description : replace some struct of the  GUILIST_ITEM_T
// Global resource dependence : 
//  Author: Qiang.Zhang
// Note:
/*****************************************************************************/
LOCAL void SetCallSetOthersItemCheck(
                        MMI_CTRL_ID_T ctrl_id,
                        BOOLEAN is_check,
                        uint16 index
                       );

#ifndef MMI_MULTI_SIM_SYS_SINGLE /* lint_lai */
/********************************************************************************
 NAME:      Stk Forbid Input Chars
 DESCRIPTION:
 AUTHOR: Michael.Shi
 DATE:      
********************************************************************************/
LOCAL void SetForbidInputChar(MMI_CTRL_ID_T ctrl_id);
#endif  


/*****************************************************************************/
//  Description : to handle interrogate ss status report
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllCFReportWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param);
/*****************************************************************************/
//  Description : to handle select popo win message
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/                                             
LOCAL MMI_RESULT_E  HandleCallSettingsWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
                                  
/*****************************************************************************/
// Description : to handle set fly mode window msg when open phone
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeOpenPhoneWindow(
                                                 MMI_WIN_ID_T      win_id,
                                                 MMI_MESSAGE_ID_E  msg_id,
                                                 DPARAM            param
                                                 );                                             
/*****************************************************************************/
//  Description : to handle plmn list
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePlmnListWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );                                             
                                             
#if 0 //!defined(MMI_MULTI_SIM_SYS_SINGLE)

/*****************************************************************************/
//  Description :
//  Global resource dependence :                                
//      Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShowHelpWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );
#endif

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
/*****************************************************************************/
//  Description : handle set major sim win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWizardMajorSimSelWindow(
                                                 MMI_WIN_ID_T          win_id,  // 窗口的ID
                                                 MMI_MESSAGE_ID_E       msg_id,     // 窗口的内部消息ID
                                                 DPARAM                 param       // 相应消息的参数
                                                 );

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WizardCFNRCSIMSelectCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMSelectWinCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);


/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void DispatchBySelectType(MN_DUAL_SYS_E dual_sys, MMISET_SIM_SELECT_TYPE_E select_type);
#endif                                             
/*****************************************************************************/
//  Description : to handle register new call bar password
//  Global resource dependence : 
//  Author:maql
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRegisterNewBarPasswordWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       );   
                                                       
/*****************************************************************************/
//  Description : GetNetworkType
//  Global resource dependence : none
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
PUBLIC NETWORK_TYPE_E GetNetworkType(void);

#ifdef MMI_DUALMODE_ENABLE 
/*****************************************************************************/
//  Description : MMISET_AppendNetworkTypeListItem
//  Global resource dependence : 
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void AppendNetworkTypeListItem(
                                     MMI_CTRL_ID_T        ctrl_id
                                     );

/*****************************************************************************/
//  Description : MMISET_SetNetworkType
//  Global resource dependence : none
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void SetNetworkType(//@zhaohui modify for cr94108
                          NETWORK_TYPE_E network_type, //用户的选择
                          BOOLEAN is_resend,        //是否是失败后的重发
                          BOOLEAN only_write_nv,
                          BOOLEAN is_reset_factorysetting
                          );                                               
#endif 

/********************************************************************************
NAME:           ResetReSwitchRatCount
DESCRIPTION:    
PARAM IN:       None
PARAM OUT:      
AUTHOR:     zhaohui
DATE:           2009.1.5
********************************************************************************/
LOCAL void ResetReSwitchRatCount(void);

/*****************************************************************************/
//  Description :网络搜索的窗口，判断是否是在切网状态
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsinSwitchRatState(void);



/*****************************************************************************/
//  Description :显示设置优先网络结果 
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void SetPreferRatResultDisp(BOOLEAN result);

/*****************************************************************************/
//  Description : open call divert pop window
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void OpenSetCallDivertWin(MN_DUAL_SYS_E    dual_sys);   

#if defined(MAINLCD_SIZE_176X220) || defined(MAINLCD_SIZE_128X160)
/*****************************************************************************/
//      Description : 设置输入密码窗口的背景色，image 和title
//     Global resource dependence : 
//  Author: chunjie liu
//     Note:
/*****************************************************************************/
LOCAL void SetPwBackground(
                                 MMI_WIN_ID_T    win_id, //窗口的ID
                                 MMI_CTRL_ID_T   ctrl_id,   //the id of control
                                 BOOLEAN is_need_label
                                 );
/*****************************************************************************/
//      Description : GetPinPukEditRect
//     Global resource dependence : 
//  Author: chunjie liu
//     Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetMinuteReminderEditRect(MMI_WIN_ID_T win_id, BOOLEAN is_need_label);
#endif
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
//      Description : Is Set DualSys MSISDN 
//     Global resource dependence : 
//    Author: dave.ruan
//     Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSetDualSysMSISDN(void);

/*****************************************************************************/
//  Description : Handle Enter Edit MSISDN QueryWin
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEnterEditMSISDNQueryWin(
                                                MMI_WIN_ID_T       win_id,    
                                                MMI_MESSAGE_ID_E      msg_id,    
                                                DPARAM                param      
                                             );
#if 0
/*****************************************************************************/
//      Description : to handle dualsys call divert help function, display the help infor
//     Global resource dependence : 
//   Author:  dave.ruan
//     Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDualsysCFNRCHelpWinMsg( 
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param);
#endif
#endif
//[block list in call setting]
#if 0//defined(MMI_BLACKLIST_SUPPORT)    //xiongkai
/*****************************************************************************/
//  Description : to handle security setting blacklist menu
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistMenuWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );

/*****************************************************************************/
//  Description : Set Blacklist type
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetBlacklistTypeParam(void);

/*****************************************************************************/
//  Description : set backlight setlist
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetBlacklistSetlist(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : handle blacklist type win
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistTypeWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

/*****************************************************************************/
//  Description : save all audio setting
//  Global resource dependence :
//  Author:dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void SaveBlacklistTypeAudio(void);

#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : save all video setting
//  Global resource dependence :
//  Author:dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void SaveBlacklistTypeVideo(void);
#endif

/*****************************************************************************/
//  Description : save all message setting
//  Global resource dependence :
//  Author:dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void SaveBlacklistTypeMessage(void);

/*****************************************************************************/
//  Description : handle blacklist win
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetBlackListWindow(
                                            MMI_WIN_ID_T        win_id,     //IN: 窗口的ID
                                            MMI_MESSAGE_ID_E     msg_id,     //IN: 窗口的内部消息ID
                                            DPARAM                 param        //IN: 相应消息的参数
                                            );

/*****************************************************************************/
//  Description : update list
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void UpdateBlackList(
                           MMI_CTRL_ID_T    ctrl_id        //IN:
                           );

/*****************************************************************************/
//  Description : append list
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void AppendBlacklistItem(
                                MMI_CTRL_ID_T             ctrl_id,         //要设置的列表控件ID号 (输入参数)
                                uint16                    pos,
                                BOOLEAN                   is_update
                                );

/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void HandleBlacklistMenuMsg(
                                 MMI_WIN_ID_T   prev_win_id,
                                 MMI_MENU_ID_T menu_id,    //IN: 被选中的菜单项
                                 MMI_CTRL_ID_T ctrl_id
                                 );

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlackListItmeOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM            param
                                          );
#endif

/*****************************************************************************/
//  Description : Set Blacklist info
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void InitBlacklistInfo(void);

/*****************************************************************************/
//  Description : handle blacklist new win
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNewBlacklistWindow(
                                           MMI_WIN_ID_T    win_id,
                                           MMI_MESSAGE_ID_E   msg_id,
                                           DPARAM             param
                                           );

/*****************************************************************************/
//  Description : handle blacklist edit win
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditBlacklistWindow(
                                               MMI_WIN_ID_T    win_id,
                                               MMI_MESSAGE_ID_E   msg_id,
                                               DPARAM             param
                                               );

LOCAL BOOLEAN SaveBlackListRecord(
                                uint16 index,
                                BOOLEAN is_new,
                                MMI_STRING_T str_name,
                                MMI_STRING_T str_num
                               );

/*****************************************************************************/
//  Description : Save Blacklist info
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveBlacklistInfo(
                                uint16 index,
                                BOOLEAN is_new
                               );

/*****************************************************************************/
//  Description : Save Blacklist info
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetBlacklistInfo(
                            uint16 index,
                            MMIPB_BCD_NUMBER_T *sender_ptr);

/*****************************************************************************/
//  Description : to handle the blacklist of delete query window  
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

/*****************************************************************************/
//  Description : to handle the blacklist of delete all query window    
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistDelAllQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

/*****************************************************************************/
//  Description : to handle the blacklist of delete window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MN_RETURN_RESULT_E DeleteBlacklistItem(
                                             uint16 pos
                                             );

/*****************************************************************************/
//  Description : to handle the blacklist of delete all window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MN_RETURN_RESULT_E DeleteAllBlacklistItem(void);

/*****************************************************************************/
//  Description : set detail from PB
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistDetailFromPB(MMIPB_HANDLE_T handle, MMI_CTRL_ID_T ctrl_id);

#ifdef MMI_PDA_SUPPORT 
/*****************************************************************************/
//  Description : set black list from cl
//  Global resource dependence :
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistFromCL(void);
#endif

/*****************************************************************************/
//  Description : set list from PB  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistFromPB(MMIPB_HANDLE_T handle);

/*****************************************************************************/
// 	Description : alert of the result when add a number into blacklist
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddNumToBlacklist(
                                MMI_STRING_T mmi_pb_name,
                                MMIPB_BCD_NUMBER_T pb_bcd      
                                );

/*****************************************************************************/
//  Description : to handle add to blacklist query window    
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddToBlackListQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

/*****************************************************************************/
//  Description : to handle  add blacklist open setting query window
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistAddQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

#endif
//[end]

LOCAL MMI_RESULT_E HandleNetworkWaittingIndicatorWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );
LOCAL void UpdatePasswordWinSK(MMI_WIN_ID_T    win_id);

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/


//开机设置飞行模式窗口
WINDOW_TABLE( MMISET_FLY_MODE_OPEN_PHONE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleFlyModeOpenPhoneWindow ),    
        WIN_ID( MMISET_FLY_MODE_OPEN_PHONE_WIN_ID ),
        WIN_TITLE( TXT_SET_FLY_MODE ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_FLY_MODE_OPEN_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};

// the window for call hide id
WINDOW_TABLE( MMISET_SET_CALL_HIDE_ID_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetCallHideIdWindow ),    
    WIN_ID( MMISET_SET_CALL_HIDE_ID_WIN_ID ),
    WIN_TITLE(TXT_SET_HIDE_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_CALL_HIDE_ID_MENU_CTRL_ID),
    END_WIN
};



WINDOW_TABLE( MMISET_SET_OPEN_DIVERT_WIN_TAB ) =  //语音信箱还是电话号码
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleOpenNumDivertWindow ),    
        WIN_ID( MMISET_SET_OPEN_DIVERT_WIN_ID ),
        WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_MENU_CTRL(MENU_SET_SS_DIVERT_OPT, MMISET_VOICE_NUM_CTRL_ID),
        END_WIN
};



// the window for divert voice box
WINDOW_TABLE( MMISET_SET_DIVERT_VOICE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDivertVoiceWindow ),    
        WIN_ID( MMISET_SET_DIVERT_VOICE_WIN_ID ),
        WIN_TITLE( TXT_SET_DIVERT_VOICE ),
        CREATE_EDIT_PHONENUM_CTRL(MMISET_VOCIE_MAX_NUM,MMISET_SET_DIVERT_VOICE_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// the window for divert phone number
WINDOW_TABLE( MMISET_SET_DIVERT_NUMBER_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDivertNumberWindow ),    
    WIN_ID( MMISET_SET_DIVERT_NUMBER_WIN_ID ),
    WIN_TITLE( TXT_SET_DIVERT_NUMBER ),
    CREATE_EDIT_PHONENUM_CTRL(MMISET_VOCIE_MAX_NUM, MMISET_SET_DIVERT_NUMBER_CTRL_ID),
    WIN_SOFTKEY(STXT_MAIN_PB, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for input call bar password
WINDOW_TABLE( MMISET_SET_BAR_PASSWORD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleBarPasswordWindow ),    
        WIN_ID( MMISET_SET_BAR_PASSWORD_WIN_ID ),
        WIN_TITLE( TXT_INPUT_PASSWORD ),
        CREATE_EDIT_PASSWORD_CTRL(MMISET_BAR_PASSWORD_MAX_LEN,MMISET_SET_BAR_PASSWORD_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),    
        END_WIN
};

// the window for register new call bar password
WINDOW_TABLE( MMISET_SET_BAR_CHANGE_PW_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleRegisterNewBarPasswordWindow ),    
    WIN_ID( MMISET_SET_BAR_CHANGE_PW_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_EDIT_PASSWORD_CTRL(MMISET_BAR_PASSWORD_MAX_LEN,MMISET_SET_BAR_CHANGE_PW_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for register new call bar password
WINDOW_TABLE( MMISET_SET_BAR_CHANGE_PW_AGAIN_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleRegisterNewBarPasswordAgainWindow ),    
    WIN_ID( MMISET_SET_BAR_CHANGE_PW_AGAIN_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_EDIT_PASSWORD_CTRL(MMISET_BAR_PASSWORD_MAX_LEN,MMISET_SET_BAR_CHANGE_PW_AGAIN_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for select network
WINDOW_TABLE( MMISET_SET_SELECT_NETWORK_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectNetworkWindow ),    
    WIN_ID( MMISET_SET_SELECT_NETWORK_WIN_ID ),
    WIN_TITLE( TXT_NETWORK_SELECT ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_SELECT_NETWORK_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN
};

#ifdef MMI_PREFER_PLMN_SUPPORT
// the window for network setting
WINDOW_TABLE( MMISET_SET_NETWORK_SETTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleNetworkSettingWindow ),    
    WIN_ID( MMISET_SET_NETWORK_SETTING_WIN_ID ),
    WIN_TITLE( TXT_NETWORK_SETTING ),
    CREATE_MENU_CTRL(MENU_SET_NETWORK_SETTING, MMISET_SET_NETWORK_SETTING_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for display prefer plmn list.
WINDOW_TABLE( MMISET_SET_DISP_PREFER_NETWORK_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleDispPreferNetworkListWinMsg ),    
    WIN_ID( MMISET_SET_PREFER_NETWORK_WIN_ID ),
    WIN_TITLE( TXT_SET_PREFER_NETWORK ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_NETWORK_PREFER_PLMN_LIST_CTRL_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for prefer network list options menu
WINDOW_TABLE(MMISET_PREFER_NETWORK_OPT_MENU_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePreferNetworkOptionMsg ),    
    WIN_ID( MMISET_PREFER_NETWORK_OPT_MENU_WIN_ID ),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_SET_PREFER_NETWORK_OPT, MMISET_SET_PREFER_NETWORK_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
//the window for prefer network list options menu
WINDOW_TABLE(MMISET_PREFER_NETWORK_LONGPENOK_OPT_MENU_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePreferNetworkOptionMsg ),    
    WIN_ID( MMISET_PREFER_NETWORK_OPT_MENU_WIN_ID ),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_SET_PREFER_NETWORK_LONGPENOK_OPT, MMISET_SET_PREFER_NETWORK_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

//the window for display all plmn list .
WINDOW_TABLE(MMISET_NETWORK_PLMN_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePlmnListDispWinMsg ),    
    WIN_ID( MMISET_NETWORK_PLMN_LIST_WIN_ID ),
    WIN_TITLE( TXT_SET_NETWORK_PLMN_LIST ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  MMISET_NETWORK_PLMN_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for create plmn item.
WINDOW_TABLE(MMISET_NETWORK_NEW_PLMN_ITEM_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCreatePlmnItemWinMsg ),    
    WIN_ID( MMISET_NETWORK_CREATE_PLMN_ITEM_WIN_ID ),
    WIN_TITLE( TXT_NEW ), // new

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_NETWORK_PLMN_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_NETWORK_PLMN_FORM1_CTRL_ID,MMISET_NETWORK_PLMN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_NETWORK_PLMN_LABEL_CTRL_ID,MMISET_NETWORK_PLMN_FORM1_CTRL_ID),
        CHILD_EDIT_DIGITAL_CTRL(TRUE,MMISET_PLMN_DIGITAL_MAX_LEN,MMISET_NETWORK_PLMN_EDIT_CTRL_ID,MMISET_NETWORK_PLMN_FORM1_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_NETWORK_PLMN_FORM2_CTRL_ID,MMISET_NETWORK_PLMN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_NETWORK_PLMN_PRIORITY_LABEL_CTRL_ID,MMISET_NETWORK_PLMN_FORM2_CTRL_ID),
        CHILD_EDIT_DIGITAL_CTRL(TRUE,MMISET_PLMN_PRIORITY_MAX_LEN,MMISET_NETWORK_PLMN_PRIORITY_CODE_EDIT_CTRL_ID,MMISET_NETWORK_PLMN_FORM2_CTRL_ID),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for create plmn item.
WINDOW_TABLE(MMISET_NETWORK_CHANGE_PRIORITY_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePreferNetworkPriorityChangeWinMsg ),    
    WIN_ID( MMISET_NETWORK_PRIORITY_CHANGE_WIN_ID ),
    WIN_TITLE( TXT_COMMON_PRIORITY ), // new

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_NETWORK_PRIORITY_CHANGE_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_NETWORK_PRIORITY_CHANGE_FORM1_CTRL_ID,MMISET_NETWORK_PRIORITY_CHANGE_FORM_CTRL_ID),
        CHILD_EDIT_DIGITAL_CTRL(TRUE,MMISET_PLMN_PRIORITY_MAX_LEN,MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID,MMISET_NETWORK_PRIORITY_CHANGE_FORM1_CTRL_ID),
    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

//@Lin.Lin add for set ip number, start
// the window for list ip number
#ifdef MMI_IP_CALL_SUPPORT
WINDOW_TABLE( MMISET_LIST_IPNUMBER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleListIpNumberWindow ),    
        WIN_ID( MMISET_LIST_IPNUMBER_WIN_ID ),
        WIN_TITLE( TXT_SET_IP_NUMBER ),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_LIST_IPNUMBER_CTRL_ID),
        WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
        END_WIN
};


// the window for modify ip number
WINDOW_TABLE( MMISET_MODIFY_IPNUMBER_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleModifyIPNumberWindow ),    
    WIN_ID( MMISET_MODIFY_IPNUMBER_WIN_ID ),
    WIN_TITLE( TXT_SET_IP_NUMBER ),
    CREATE_EDIT_PHONENUM_CTRL(MMISET_IPNUMBER_MAX_LEN,MMISET_MODIFY_IPNUMBER_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

// 分钟提示音窗口
WINDOW_TABLE( MMISET_SET_MINUTE_REMINDER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetMinuteReminderWindow ),
    WIN_ID( MMISET_SET_MINUTE_REMINDER_WIN_ID ),
    WIN_TITLE( TXT_SET_CALL_TIME_REMINDER ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_MINUTE_REMINDER_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
//分钟提示音时间自定义窗口
WINDOW_TABLE(MMISET_SET_MINUTE_REMINDER_EDIT_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleSetMinuteReminderEditWindow),  
    WIN_ID( MMISET_SET_MINUTE_REMINDER_EDIT_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_SET_CALL_TIME_REMINDER),
    
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMISET_SET_MINUTE_REMINDER_LABEL_CTRL_ID),
    CREATE_EDIT_DIGITAL_CTRL(2, MMISET_SET_MINUTE_REMINDER_EDITBOX_CTRL_ID),
    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

//提示用户即将进行手动选网 
WINDOW_TABLE(MMISET_SEL_NET_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleSelNetManualWinMsg ),    
        WIN_ID(MMISET_SEL_NET_WIN_ID),
        WIN_TITLE(TXT_NETWORK_SELECT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMISET_SEL_NET_MANUAL_CTRL_ID),
        END_WIN
};



#if defined (MMI_DUALMODE_ENABLE) || defined(MMI_2G_4G_SUPPORT)
#ifdef VT_SUPPORT
// the window for VT call setting 
WINDOW_TABLE( MMISET_SET_VT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVTSettingsWindow ),    
        WIN_ID( MMISET_SET_VT_SETTINGS_WIN_ID ),
        WIN_TITLE(TXT_SET_VIDEOPHONE),
        CREATE_MENU_CTRL(MENU_SET_VT_OPT, MMISET_CALL_VT_SETTINGS_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// the window for VT call fallback setting 
WINDOW_TABLE( MMISET_SET_VT_FALLBACK_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVTFallbackSettingsWindow ),    
        WIN_ID( MMISET_SET_VT_FALLBACK_WIN_ID ),
        WIN_TITLE(TXT_SET_VT_FALLBACK_HANDLE),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_CALL_VT_FALLBACK_SETTINGS_LIST_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// the window for VT call resplace image setting 
WINDOW_TABLE( MMISET_SET_VT_REPLACE_IMAGE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVTReplaceImageSettingsWindow ),
        WIN_ID( MMISET_SET_VT_REPLACE_IMAGE_WIN_ID ),
        WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
        WIN_TITLE(TXT_SET_VT_REPLACE_IMAGE),
        //    CREATE_RICHTEXT_CTRL(MMIVT_REPLACE_IMAGE_RICHTEXT_CTRL_ID),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CALL_VT_REPLACE_IMAGE_LIST_CTRL_ID),
        WIN_SOFTKEY(TXT_OPTION_SPECIFY, TXT_NULL, STXT_RETURN),
        END_WIN
};

// 预览自定义图片窗口
WINDOW_TABLE( MMISET_VT_REPLACE_REVIEW_PHOTO_WIN_TAB ) = 
{
    WIN_ID( MMISET_VT_REPLACE_REVIEW_PHOTO_WIN_ID),
    WIN_TITLE( TXT_SET_VT_REPLACE_IMAGE ),
    WIN_FUNC( (uint32)HandleReviewCustomPhotoWinMsg),
    CREATE_ANIM_CTRL(MMISET_VT_REPLACE_REVIEW_PHOTO_ANIM_CTRL_ID,MMISET_VT_REPLACE_REVIEW_PHOTO_WIN_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    END_WIN
};


WINDOW_TABLE( MMISET_VT_REPLACE_REVIEW_PHOTO_OPT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_ID(MMISET_VT_REPLACE_REVIEW_PHOTO_OPT_WIN_ID),
        WIN_TITLE(TXT_NULL),
        WIN_FUNC( (uint32)HandleReviewCustomPhotoOptWinMsg),
        CREATE_MENU_CTRL(MENU_SET_VT_CUSTOM_PHOTO_OPT, MMISET_VT_CUSTOM_PHOTO_OPT_MENU_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//设置VT录制存储位置窗口
WINDOW_TABLE( MMISET_VT_RECORD_PATH_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleVTRecordPathSetWinMsg), 
        WIN_ID( MMISET_VT_RECORD_PATH_WIN_ID ),
        WIN_TITLE(TXT_SET_SPCIFY_SAVE_PATH),
        WIN_BACKGROUND_ID(IMAGE_COMMON_BG),

        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_VT_RECORD_FORM_CTRL_ID),
        //RV & DA
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_VT_RECORD_RV_DA_FORM_CTRL_ID,MMISET_VT_RECORD_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VT_RECORD_RV_DA_LABEL_CTRL_ID,MMISET_VT_RECORD_RV_DA_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE, MMISET_VT_RECORD_RV_DA_DROPDOWN_CTRL_ID, MMISET_VT_RECORD_RV_DA_FORM_CTRL_ID),
        
        //RV
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_VT_RECORD_RV_FORM_CTRL_ID,MMISET_VT_RECORD_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VT_RECORD_RV_LABEL_CTRL_ID,MMISET_VT_RECORD_RV_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE, MMISET_VT_RECORD_RV_DROPDOWN_CTRL_ID, MMISET_VT_RECORD_RV_FORM_CTRL_ID),
        
        //DA
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_VT_RECORD_DA_FORM_CTRL_ID,MMISET_VT_RECORD_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VT_RECORD_DA_LABEL_CTRL_ID,MMISET_VT_RECORD_DA_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE, MMISET_VT_RECORD_DA_DROPDOWN_CTRL_ID, MMISET_VT_RECORD_DA_FORM_CTRL_ID),
        WIN_SOFTKEY(STXT_SAVE,TXT_NULL,STXT_RETURN),
        END_WIN
};

// the window for VT one time ring setting
WINDOW_TABLE( MMISET_SET_VT_ONE_TIME_RING_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVTOneTimeRingSettingsWindow ),    
    WIN_ID( MMISET_SET_VT_ONE_TIME_RING_WIN_ID ),
    WIN_TITLE(TXT_SET_VT_ONE_TIME_RING),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_VT_ONE_TIME_RING_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    
    END_WIN
};

//设置VT本地视频窗口
WINDOW_TABLE( MMISET_SET_VT_LOCAL_VIDEO_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleVTLocalVideoSetWinMsg), 
    WIN_ID( MMISET_VT_LOCAL_VIDEO_WIN_ID ),
    WIN_TITLE(TXT_SET_VT_LOCAL_VIDEO),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_VT_LOCAL_SETTING_FORM_CTRL_ID),
    //对方关闭摄像头
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER, MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID,MMISET_VT_LOCAL_SETTING_FORM_CTRL_ID),
#if 1//bug337002
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID,MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,FALSE,MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID,MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
#else
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID,MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMISET_VT_REMOTE_SHUTDOWN_CAMERA_DROPDOWN_CTRL_ID, MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID),
               
    WIN_SOFTKEY(STXT_SAVE,TXT_NULL,STXT_RETURN),
#endif
    END_WIN
};
#if 1//bug337002
WINDOW_TABLE( MMISET_SET_REMOTESHUTDOWNCAMERASET_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleVTLocalVideoSetSelectWinMsg ),    
        WIN_ID( MMISET_SET_REMOTESHUTDOWNCAMERASET_WIN_ID ),        
        WIN_TITLE( TXT_COMM_REMOTE_CAMERA_OFF ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_VT_REMOTE_SHUTDOWN_CAMERA_SET_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN
};
#endif
#endif
//#if 0//ndef MMI_SMART_DUAL_SIM_SUPPORT
WINDOW_TABLE( MMISET_NETWORK_METHOD_WIN_TAB) = 
{ 
    WIN_FUNC( (uint32)HandleSelectNetworkMethodWindow ),
    WIN_ID( MMISET_SET_NETWORK_METHOD_WIN_ID ),
    WIN_TITLE( TXT_NETWORK_SELECT ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_NETWORK_METHOD_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN
};
// #else
// WINDOW_TABLE( MMISET_NETWORK_METHOD_WIN_TAB) = 
// { 
//     WIN_FUNC( (uint32)HandleSelectNetworkMethodWindow ),
//         WIN_ID( MMISET_SET_NETWORK_METHOD_WIN_ID ),
//         CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_NETWORK_METHOD_CTRL_ID),
// #ifdef MMI_PDA_SUPPORT
//         WIN_SOFTKEY(TXT_NULL, STXT_SELECT, TXT_COMMON_COMMON_BACK),
// #else
//         WIN_SOFTKEY(TXT_NULL, STXT_SELECT, TXT_COMMON_COMMON_BACK),
// #endif
//         END_WIN
// };
// #endif
#endif



// 自动重拨窗口
WINDOW_TABLE( MMISET_SET_AUTO_REDIAL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetAutoRedialWindow ),    
        WIN_ID( MMISET_SET_AUTO_REDAIL_WIN_ID ),
        WIN_TITLE( STXT_CALL_REPEAT ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_AUTO_REDIAL_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// 延时拨号
WINDOW_TABLE( MMISET_SET_SPEED_DIAL_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSetSpeedDialWindow ),
        WIN_ID( MMISET_SET_SPEED_DIAL_WIN_ID ),
        WIN_TITLE( TXT_SPEED_DIAL ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_SPEED_DIAL_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#if !defined MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMISET_SET_CALL_OTHER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetCallSetOtherWindow ),    
    WIN_ID( MMISET_SET_Call_Other_WIN_ID ),
    WIN_TITLE(TXT_TOOLS_OTHERS),
#if defined(MMI_PDA_SUPPORT) 
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
#else
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_OTHER_SETTING_CTRL_ID),
    END_WIN
};
#else
WINDOW_TABLE( MMISET_SET_CALL_OTHER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSetCallSetOtherWindow ),    
        WIN_ID( MMISET_SET_Call_Other_WIN_ID ),
        WIN_TITLE(TXT_TOOLS_OTHERS),
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_IP_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_IP_KEY_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_IP_FORM_CTRL_ID),//ip

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_MINUTE_REMINDER_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_CALL_MINUTE_REMINDER_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_MINUTE_REMINDER_FORM_CTRL_ID),//语音通话每分钟提示音
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_MINUTE_REMINDER_FORM_CTRL_ID),//语音通话每分钟提示音
        
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_AUTO_CONNECT_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_AUTO_CONNECT_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_AUTO_CONNECT_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_AUTO_CONNECT_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_AUTO_CONNECT_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_VIBRATE_TIP_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VIBRATE_TIP_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_VIBRATE_TIP_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_VIBRATE_TIP_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_VIBRATE_TIP_FORM_CTRL_ID),

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_ANSWER_MODE_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_ANSWER_MODE_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_ANSWER_MODE_FORM_CTRL_ID),
#endif

#ifdef MV_SUPPORT 
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_MAGIC_VOICE_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_MAGIC_VOICE_LIST_CTRL_ID,MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID),
#endif
#ifdef TTS_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_VOICE_REPORT_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VOICE_REPORT_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_VOICE_REPORT_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_VOICE_REPORT_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_VOICE_REPORT_FORM_CTRL_ID),
#endif
#if 0//def TOUCH_PANEL_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_VIBRATE_TP_REPORT_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VIBRATE_TP_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_VIBRATE_TP_REPORT_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_VIBRATE_TP_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_VIBRATE_TP_REPORT_FORM_CTRL_ID),
#endif
#if defined(MMI_MULTI_SIM_SYS_SINGLE)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_FLY_MODE_SELECT_CTRL_ID,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_POWER_ON_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_FLY_MODE_SELECT_ON_POWER_CTRL_ID,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_POWER_ON_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_POWER_ON_FORM_CTRL_ID),
#endif
        END_WIN
};
#endif

#ifdef MMI_CSP_SUPPORT
//the window for select line
WINDOW_TABLE( MMISET_SELECT_LINE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectLineWindow ),    
        WIN_ID( MMISET_LINE_SELECT_WIN_ID ),
        WIN_TITLE( TXT_SET_LINE_SELECT ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_LINE_SELECT_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};

WINDOW_TABLE( MMISET_LOCK_LINE_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleLockLineWindow ),    
        WIN_ID( MMISET_LOCK_LINE_WIN_ID ),
        WIN_TITLE( TXT_SET_LINE_LOCK ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_LOCK_LINE_LIST_CTRL_ID),
        WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
        END_WIN
};

#endif //MMI_CSP_SUPPORT


//设置飞行模式窗口
WINDOW_TABLE( MMISET_FLY_MODE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleFlyModeWindow ),    
        WIN_ID( MMISET_FLY_MODE_WIN_ID ),
        WIN_TITLE( TXT_SET_FLY_MODE ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_FLY_MODE_LIST_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

WINDOW_TABLE( MMISET_FLY_MODE_SET_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFlyModeSetWindow ),    
        WIN_ID( MMISET_FLY_MODE_SET_WIN_ID ),
        WIN_TITLE( TXT_SET_FLY_MODE ),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_FLY_MODE_SET_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
        END_WIN
};



//@maggie add begin for cr79064
WINDOW_TABLE( MMISET_NETWORK_TYPE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleNetworkTypeWindow ),    
        WIN_ID( MMISET_SET_NETWORK_TYPE_WIN_ID ),
        WIN_TITLE( TXT_NETWORK_TYPE ),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_NETWORK_TYPE_CTRL_ID),
        END_WIN
};

#if defined(MMI_GUI_STYLE_TYPICAL)
//设置接听拒接模式窗口
WINDOW_TABLE( MMISET_SET_ANYKEY_REJECT_MODE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetAnykeyRejectWindow ),
    WIN_ID( MMISET_SET_ANYKEY_REJECT_MODE_WIN_ID ),
    WIN_TITLE( TXT_SET_ANYKEY_REJECT_MODE ),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_SET_ANYKEY_REJECT_MODE_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN
};
#endif

//接通提示音窗口
WINDOW_TABLE( MMISET_SET_CONNECT_PROMPT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetConnectPromptWindow ),
        WIN_ID( MMISET_SET_CONNECT_PROMPT_WIN_ID ),
        WIN_TITLE( TXT_SET_CONNECT_PROMPT ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_CONNECT_PROMPT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};


#ifdef MMI_IP_CALL_SUPPORT
WINDOW_TABLE( MMISET_IPNUMBER_ACTIVE_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleIpNumberOptionWindow),
        WIN_ID(MMISET_IPNUMBER_OPTION_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_SET_IPNUMBER_ACTIVE_OPT,MMISET_IPNUMBER_OPT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};

WINDOW_TABLE( MMISET_IPNUMBER_DEACTIVE_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleIpNumberOptionWindow),
        WIN_ID(MMISET_IPNUMBER_OPTION_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_SET_IPNUMBER_DEAVTIVE_OPT,MMISET_IPNUMBER_OPT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};

#if defined(MMI_PDA_SUPPORT)
WINDOW_TABLE( MMISET_IPNUMBER_ADD_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleIpNumberAddOptionWindow),
        WIN_ID(MMISET_IPNUMBER_ADD_OPT_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_SET_IPNUMBER_ADD_OPT,MMISET_IPNUMBER_ADD_OPT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};
#endif
#endif

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
// the window for set time
WINDOW_TABLE( MMISET_SET_SIM_NAME_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditSimNameWindow ),    
    WIN_ID( MMISET_SET_SIM_NAME_WIN_ID ),
    WIN_TITLE( TXT_SET_SIM_NAME ),
#if !defined MMI_GUI_STYLE_TYPICAL 
    WIN_SOFTKEY(STXT_CANCEL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(STXT_CANCEL, TXT_NULL, STXT_RETURN),
#endif
    CREATE_EDIT_TEXT_CTRL(0,MMISET_SET_SIM_NAME_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISET_SET_MULTISYS_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_SIM_ENABLE_SEL ),
    WIN_FUNC( (uint32) HandleSetMultiSysTypeWindow),    
    WIN_ID( MMISET_SET_MULTISYS_TYPE_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_NULL, STXT_SELECT, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_DAULSYS_TYPE_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_NULL),
    WIN_FUNC( (uint32)HandleSetMajorSimWhenAllSimOnWindow ),    
        WIN_ID( MMISET_SET_DUALSYS_SIM_ACTIVE_TYPE_WIN_ID ),
        WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_DUALSYS_SIM_ACTIVE_TYPE_CTRL_ID),
        END_WIN
};

WINDOW_TABLE( MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_SET_REPLY_BY_ORIGINAL_SIM ),
    WIN_FUNC( (uint32)HandleSetAnwerWithOriginalSimWindow ),    
    WIN_ID( MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_CTRL_ID),
    END_WIN
};
/*     //Sam.hua     use SelectSimFunc 
//显示使用SIM卡拨出的选项                                                
WINDOW_TABLE( MMIUSSD_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleUSSDSIMSelectWinMsg),    
        WIN_ID(MMISET_USSD_SELECT_SIM_WIN_ID),
        WIN_TITLE(TXT_SIM_SEL),
#if defined(MMI_PDA_SUPPORT) 
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
        WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_USSD_SELECT_SIM__CTRL_ID),
        END_WIN
};



//设置向导中无应答转移设置选卡
WINDOW_TABLE( MMISET_WIZARD_CFNRC_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleWizardCFNRCSIMSelectWinMsg),    
        WIN_ID(MMISET_WIZARD_CFNRC_SELECT_SIM_WIN_ID),
        WIN_TITLE(TXT_SET_DIVERTS_NOT_REACHABLE),
        WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_WIZARD_CFNRC_SELECT_SIM__CTRL_ID),
        END_WIN
};

*/

WINDOW_TABLE( MMISET_MULTI_SIM_SETTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMultiSimSettingWindow ),    
    WIN_ID( MMISET_SET_MULTI_SYS_SETTING_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL 
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_MULTI_SIM_SETTING_CTRL_ID),
#else
    CREATE_MENU_CTRL(MENU_SET_MULTI_SIM_SETTING, MMISET_SET_MULTI_SIM_SETTING_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE( MMISET_DUAL_MAJOR_SIM_SEL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleWizardMajorSimSelWindow ),    
        WIN_ID( MMISET_DUAL_MAJOR_SIM_SEL_WIN_ID ),
        WIN_TITLE(TXT_SET_SIM_NAME),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_MENU_CTRL(MENU_SET_WIZARD_MAJOR_SIM_SEL, MMISET_DUAL_MAJOR_SIM_SEL_CTRL_ID),
        END_WIN
};

#ifdef MMI_MULTISIM_COLOR_SUPPORT
// the window for set sim color window
WINDOW_TABLE( MMISET_SET_SIM_COLOR_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditSimColorWindow ),    
    WIN_ID( MMISET_SET_SIM_COLOR_WIN_ID ),
    WIN_TITLE( TXT_SET_SIM_COLOR ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SIM_COLOR_SIM_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SIM_COLOR_BTN_FORM_CTRL_ID,MMISET_SIM_COLOR_SIM_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID,MMISET_SIM_COLOR_BTN_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR1_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR2_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR3_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR4_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID,MMISET_SIM_COLOR_BTN_FORM_CTRL_ID),    
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR5_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR6_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR7_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR8_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID),
    
    END_WIN
};
#endif
// add for dual sys end
#endif //!defined(MMI_MULTI_SIM_SYS_SINGLE)

#ifdef FLIP_PHONE_SUPPORT
// 翻盖接听窗口
WINDOW_TABLE( MMISET_SET_FLIP_ANSWER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetFlipAnswerWindow ),
        WIN_ID( MMISET_SET_FLIP_ANSWER_WIN_ID ),
        WIN_TITLE( TXT_SET_FLIP_ANSWER ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_FLIP_ANSWER_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif

// the window for select network plmn list
#if 0//ndef MMI_SMART_DUAL_SIM_SUPPORT
WINDOW_TABLE( MMISET_SET_PLMN_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePlmnListWindow ),    
        WIN_ID( MMISET_SET_PLMN_LIST_WIN_ID ),
        WIN_TITLE( TXT_SET_MANUAL_SEL ),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_PLMN_LIST_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
        END_WIN
};
#else
WINDOW_TABLE( MMISET_SET_PLMN_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePlmnListWindow ),    
    WIN_ID( MMISET_SET_PLMN_LIST_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_PLMN_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

// the window for select no anwer time
WINDOW_TABLE( MMISET_SET_NO_ANSWER_TIME_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleNoAnswerTimeWindow ),
    WIN_TITLE(TXT_SET_DIVERTS_NO_ANSWER),
    WIN_ID( MMISET_SET_NO_ANSWER_TIME_WIN_ID ),
	WIN_SOFTKEY(TXT_NULL, STXT_SELECT, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_NO_ANSWER_TIME_CTRL_ID),
    END_WIN
};
// the window for interrogate all call foward report
WINDOW_TABLE( MMISET_SS_ALL_REPORT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleAllCFReportWindow ),    
        WIN_ID( MMISET_SS_ALL_REPORT_WIN_ID ),
       // WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
       // WIN_TITLE( TXT_SET_QUERY_STATUS ),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_RICHTEXT_CTRL(MMISET_SS_ALL_REPORT_CTRL_ID),
        END_WIN
};



WINDOW_TABLE( MMISET_SELECT_POP_DIVERT_WIN_TAB ) =  //ADDED BY MARY
{
    //CLEAR_LCD,
    WIN_FUNC( (uint32)HandleSelectPopDivertWindow),    
    WIN_ID( MMISET_SLECT_POP_DIVERT_WIN_ID ),
    WIN_TITLE(TXT_SET_CALL_DIVERTS),
    WIN_STYLE( WS_HAS_TRANSPARENT),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_POPMENU_CALLSETTING_DIVERT_ID),
    END_WIN
};


WINDOW_TABLE( MMISET_SELECT_POP_WAITING_WIN_TAB ) =  //ADDED BY MARY
{
    //CLEAR_LCD,
    WIN_FUNC( (uint32)HandleSelectPopWaitingWindow),    
    WIN_ID( MMISET_SLECT_POP_WAITING_WIN_ID ),
    WIN_TITLE(TXT_SET_CALL_WAITING),
    WIN_STYLE( WS_HAS_TRANSPARENT),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_POPMENU_CALLSETTING_WAITING_ID),
    END_WIN
};
WINDOW_TABLE( MMISET_SELECT_POP_BAR_WIN_TAB ) =  //ADDED BY MARY
{
    WIN_FUNC( (uint32)HandleSelectPopBarWindow),    
    WIN_ID( MMISET_SLECT_POP_BAR_WIN_ID ),
    WIN_TITLE(TXT_SET_CALL_BAR),
    WIN_STYLE( WS_HAS_TRANSPARENT),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_POPMENU_CALLSETTING_BAR_ID),
    END_WIN
};

WINDOW_TABLE( MMISET_SET_CALL_BAR_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleCallBarWindow ),    
    WIN_ID( MMISET_SET_CALL_BAR_WIN_ID ),
    WIN_TITLE(TXT_SET_CALL_BAR),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CALL_BAR_MENU_CTRL_ID),
#else
    CREATE_MENU_CTRL(MENU_SET_CALL_BAR_OPT, MMISET_CALL_BAR_MENU_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE( MMISET_SET_CALL_DIVERT_CONDITIONS_WIN ) = 
{
    WIN_FUNC( (uint32)HandleCallDivertConditionsWin ),    
    WIN_ID( MMISET_SET_CALL_DIVERT_CONDITIONS_ID ),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CALL_DIVERT_CONDITIONS_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISET_SET_CALL_DIVERT_SIMCARD_WIN ) = 
{
    WIN_FUNC( (uint32)HandleCallDivertSimCardWin ),    
    WIN_ID( MMISET_SET_CALL_DIVERT_SIMCARD_ID ),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    WIN_STYLE(WS_NO_DEFAULT_STYLE),
    //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CALL_DIVERT_SIMCARD_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISET_SET_CALL_WAITING_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleCallWaitingWindow ),    
    WIN_ID( MMISET_SET_CALL_WAITING_WIN_ID ),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    WIN_STYLE(WS_NO_DEFAULT_STYLE),
    //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CALL_WAITING_MENU_CTRL_ID),
    END_WIN
};
//add by ljg
WINDOW_TABLE( MMISET_SELECT_SIM_VOICEMAIL_WIN_TAB ) = 
{
	WIN_FUNC((uint32) HandleVoiceMailSIMSelectWinMsg),	
	WIN_ID( MMISET_SET_VOICEMAIL_SELECT_SIM_WIN_ID ),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
	END_WIN
};

WINDOW_TABLE( MMISET_SET_FIXED_DIALING_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFixedDialingWindow ),    
    WIN_ID( MMISET_SET_FIXED_DIALING_WIN_ID ),
	WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
//    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_FIXED_DIALING_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISET_SET_FIXED_DIALING_SUB_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFixedDialingSubWindow ),    
    WIN_ID( MMISET_SET_SECURITY_FDN_WIN_ID ),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_FIXED_DIALING_SUB_MENU_CTRL_ID),
    END_WIN
};


WINDOW_TABLE( MMISET_SET_FIXED_DIALING_SWITCH_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFixedDialingSwitchWindow ),    
    WIN_ID( MMISET_SET_SECURITY_FDN_WIN_ID ),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_FIXED_DIALING_SWITCH_CTRL_ID),
    END_WIN
};


WINDOW_TABLE( MMISET_SET_CALL_DIVERT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleCallDivertWindow ),    
    WIN_ID( MMISET_SET_CALL_DIVERT_WIN_ID ),
    WIN_TITLE(TXT_SET_CALL_DIVERTS),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CALL_DIVERT_MENU_CTRL_ID),
#else
    CREATE_MENU_CTRL(MENU_SET_CALL_DIVERTS_OPT, MMISET_CALL_DIVERT_MENU_CTRL_ID),
#endif
//     CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_CALL_DIVERT_MENU_CTRL_ID),
//     CREATE_MENU_CTRL(MENU_SET_CALL_DIVERTS_OPT, MMISET_CALL_DIVERT_MENU_CTRL_ID),
    END_WIN
};

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
WINDOW_TABLE( MMISET_CTA_MULTI_SYS_STANDBY_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SIM_ENABLE_SEL ),
    WIN_FUNC( (uint32) HandleCTAMultiSysStandbyWindow),    
    WIN_ID( MMISET_SET_CTA_MULTISYS_STANDBY_WIN_ID ),

#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_OK),
#else
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif

    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_CTA_MULTI_SYS_STANDBY_CTRL_ID),
    END_WIN
};
#endif

//@zhaohui add start
//the window for divert or restrict(menu:"open,close,inquire"  for audio call)

#ifdef VT_SUPPORT //modify by mary

WINDOW_TABLE( MMISET_SET_DIVERT_AUDIOORVIDEO_CALL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSelectCallTypeGeneralWindow ),    
        WIN_ID( MMISET_SET_SELECT_DIVERT_WIN_ID ),
        WIN_TITLE(TXT_SET_CALL_DIVERTS),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_CALL_MENU_EX_CTRL_ID),
#else
        CREATE_MENU_CTRL(MENU_SET_DIVERT_CALLS_OPT, MMISET_SET_CALL_MENU_EX_CTRL_ID),
#endif
        END_WIN
};

WINDOW_TABLE( MMISET_SET_BAR_AUDIOORVIDEO_CALL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSelectCallTypeGeneralWindow ),    
        WIN_ID( MMISET_SET_SELECT_DIVERT_WIN_ID ),
        WIN_TITLE(TXT_SET_CALL_BAR),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_CALL_MENU_EX_CTRL_ID),
#else
        CREATE_MENU_CTRL(MENU_SET_BARRING_CALLS_OPT, MMISET_SET_CALL_MENU_EX_CTRL_ID),
#endif
        END_WIN
};

#endif

WINDOW_TABLE( MMISET_SET_CALL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallSettingsWindow ),    
    WIN_ID( MMISET_SET_CALL_WIN_ID ),
    WIN_TITLE(TXT_SETTINGS_CALL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_CALL_MENU_CTRL_ID),
#else
    CREATE_MENU_CTRL(MENU_SET_CALL_OPT, MMISET_SET_CALL_MENU_CTRL_ID),
#endif
    END_WIN
};

#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
#if 0
//使用说明窗口
WINDOW_TABLE( MMISET_SET_DUALSYS_DIVERT_HELP_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleDualsysCFNRCHelpWinMsg ),    
    WIN_ID( MMISET_SET_DUALSY_DIVERT_HELP_WIN_ID ),
    WIN_TITLE(TXT_HELP),
    WIN_SOFTKEY(TXT_NULL,TXT_NULL,STXT_RETURN),
    CREATE_TEXT_CTRL(MMISET_HELP_TEXT_CTRL_ID),
    END_WIN
}; 
#endif
#endif
//[block list in call setting]
#if 0//defined(MMI_BLACKLIST_SUPPORT)    //xiongkai
// the window for set blacklist
WINDOW_TABLE( MMISET_SET_SECURITY_BLACKLIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleBlacklistMenuWindow ),    
    WIN_ID( MMISET_SET_SECURITY_BLACKLIST_WIN_ID ),
    WIN_TITLE( TXT_SET_BLACKLIST ),
    CREATE_MENU_CTRL(MENU_BLACKLIST_SET_OPT, MMISET_SET_BLACKLIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
};

#ifndef MMI_GUI_STYLE_TYPICAL
//the window for set blacklist type
#if defined(MMI_BLACKLIST_SUPPORT)
WINDOW_TABLE( MMISET_SET_BLACKLIST_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleBlacklistTypeWindow ),    
    WIN_ID( MMISET_SET_BLACKLIST_TYPE_WIN_ID ),
    WIN_TITLE(TXT_SET_BLACKLIST_SETTING),
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID),

    END_WIN
};
#endif
#else
#if defined(MMI_BLACKLIST_SUPPORT)
//the window for set blacklist type
WINDOW_TABLE( MMISET_SET_BLACKLIST_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleBlacklistTypeWindow ),    
    WIN_ID( MMISET_SET_BLACKLIST_TYPE_WIN_ID ),
    WIN_TITLE(TXT_SET_BLACKLIST_SETTING),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_TYPE_FORM_AUDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_TYPE_AUDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_AUDIO_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE,MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_AUDIO_CTRL_ID),

#ifdef VT_SUPPORT
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_TYPE_FORM_VEDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_TYPE_VEDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_VEDIO_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE,MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_VEDIO_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_TYPE_FORM_MESSAGE_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_TYPE_MESSAGE_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_MESSAGE_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE,MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_MESSAGE_CTRL_ID),

    END_WIN
};
#endif
#endif

//the window for blacklist win
WINDOW_TABLE( MMISET_SET_BLACKLIST_LIST_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SET_BLACKLIST_LIST ),
    WIN_FUNC((uint32) HandleSetBlackListWindow ),    
    WIN_ID( MMISET_SET_BLACKLIST_LIST_WIN_ID ),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMISET_SET_BLACKLIST_LIST_CTRL_ID),
    END_WIN
};

//the window for blacklist opt win
WINDOW_TABLE( MMISET_SET_BLACKLIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleBlacklistOptWinMsg),
    WIN_ID(MMISET_SET_BLACKLIST_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_BLACKLIST_OPT,MMISET_SET_BLACKLIST_LIST_OPT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//the window for new blacklist 
WINDOW_TABLE( MMISET_SET_BLACKLIST_NEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleNewBlacklistWindow ),    
    WIN_ID( MMISET_SET_BLACKLIST_NEW_WIN_ID ),
    WIN_TITLE(TXT_SET_NEW_BLACKLIST),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_OK, STXT_MAIN_PB, STXT_RETURN),
#else        
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_NEW_NAME_LABEL_CTRL_ID,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_NEW_NUM_LABEL_CTRL_ID,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID),
    CHILD_EDIT_PHONENUM_CTRL(TRUE,MMISET_PHONE_NUM_MAX_LEN,MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID),
    END_WIN
};

//the window for new blacklist 
WINDOW_TABLE( MMISET_SET_BLACKLIST_EDIT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleEditBlacklistWindow ),    
    WIN_ID( MMISET_SET_BLACKLIST_EDIT_WIN_ID ),
    WIN_TITLE(TXT_SET_EDIT_BLACKLIST),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_NEW_NAME_LABEL_CTRL_ID,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_NEW_NUM_LABEL_CTRL_ID,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID),
    CHILD_EDIT_PHONENUM_CTRL(TRUE,MMISET_PHONE_NUM_MAX_LEN,MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID),
    END_WIN
};


#ifdef MMI_PDA_SUPPORT
//the window for longpower opt win
WINDOW_TABLE( MMISET_SET_BLACKLIST_ITEM_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleBlackListItmeOptWinMsg),
    WIN_ID(MMISET_BLACKLISTITEM_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_BLACKLIST_ITEM_OPTION,MMISET_BLACKLIST_ITEM_OPT_CTRL_ID),
    END_WIN
};
#endif
#endif //#if defined(MMI_BLACKLIST_SUPPORT)
//[end]

WINDOW_TABLE( SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_WIN_TAB ) = 
{
    WIN_ID(MMISET_SET_SEARCH_NETWORK_WIN_ID),
    WIN_FUNC((uint32) HandleNetworkWaittingIndicatorWinMsg),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE, TRUE, MMI_SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_ANIM_CTRL_ID, MMI_SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMI_SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_LABEL_CTRL_ID, MMI_SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_FORM_CTRL_ID),
//CHILD_TEXT_CTRL(FALSE,MMI_SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_LABEL_CTRL_ID, MMI_SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_FORM_CTRL_ID),
//    CHILD_EDIT_TEXT_CTRL(TRUE, MMISET_MAX_SIM_NAME_LEN, MMI_SMARTDUALSIM_SIM_NAME_EDIT_CTRL_ID, MMI_SMARTDUALSIM_SIM_NAME_FORM_CTRL_ID),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    END_WIN
};

LOCAL BOOLEAN IsSimFdnFunctionAvailable(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN ret = SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(dual_sys);

#ifndef WIN32
    ret = ret && MN_SIM_SERVICE_NOT_SUPPORT != MNSIM_GetSimEFService(dual_sys, MN_SIM_FDN);
#endif

    return ret;
}

LOCAL BOOLEAN IsFdnFunctionAvailable()
{
    BOOLEAN ret = IsSimFdnFunctionAvailable(MN_DUAL_SYS_1);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    ret = ret || IsSimFdnFunctionAvailable(MN_DUAL_SYS_2);
#endif

    return ret;
}

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISET_SET_DIVERT_NUMBER_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);   
}


#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaCallSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_IMAGE_ID_T image_id,   //icon
                                 uint32         menu_id   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

     //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

     //icon
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;//GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = menu_id;

    GUILIST_AppendItem(ctrl_id, &item_t);
}
/*****************************************************************************/
//  Description : InitPdaCallSettingsCtrl
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaCallSettingsCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_CALL_MENU_CTRL_ID;
    uint16 list_max = 7;

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    list_max++;
#endif

#ifndef SUBLCD_SIZE_NONE
    list_max++;
#endif

#ifdef VT_SUPPORT
    list_max++;
#endif

    GUILIST_SetMaxItem(ctrl_id, list_max, FALSE );

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
#if defined(MMI_MULTI_SIM_SYS_DUAL)
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DUAL_SIM_SETTING, IMAGE_NULL, ID_SET_MULTI_SIM_SETTING);
#else
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_MULTI_SIM_SETTING, IMAGE_NULL, ID_SET_MULTI_SIM_SETTING);
#endif
#endif
#ifndef SUBLCD_SIZE_NONE
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_FLIP_ANSWER, IMAGE_NULL, ID_SET_FLIP_ANSWER);
#endif
#ifdef MMI_CSP_SUPPORT
    if((GetCallFowardVisibleSimNum(PNULL, 0) > 0) || (MMIAPIPHONE_GetSimAvailableNum(PNULL, 0) == 0))
#endif //MMI_CSP_SUPPORT        
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_CALL_DIVERTS, IMAGE_NULL, ID_SET_CALL_DIVERTS);
    }    
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_CALL_WAITING, IMAGE_NULL, ID_SET_CALL_WAITING);
#ifdef MMI_CSP_SUPPORT
    if((GetCallBarVisibleSimNum(PNULL, 0) > 0) || (MMIAPIPHONE_GetSimAvailableNum(PNULL, 0) == 0))
#endif //MMI_CSP_SUPPORT        
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_CALL_BAR, IMAGE_NULL, ID_SET_CALL_BAR);
    }
#ifdef VT_SUPPORT
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_VIDEOPHONE, IMAGE_NULL, ID_SET_VIDEOPHONE);
#endif
#ifdef MMI_CSP_SUPPORT
    if(MMIAPIPHONE_IsCPHSSupport() && GetSelectLineVisibleSimNum(PNULL, 0) > 0)
#endif
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_LINE_SELECT, IMAGE_NULL, ID_SET_LINE_SELECT);
    }
#ifdef MMI_CSP_SUPPORT
    if(GetLockLineVisibleSimNum(PNULL, 0) > 0)
#endif
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_LINE_LOCK, IMAGE_NULL, ID_SET_LINE_LOCK);
    }
    //MMI_CSP_SUPPORT
    //AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_HIDE_ID, IMAGE_NULL, ID_SET_HIDE_ID);//delete by James li
    //AppendPdaCallSettingsItem1Line(ctrl_id, STXT_VOICE_MBX_NBR, IMAGE_NULL, ID_SET_VOICE_MAIL);//delete by James li
//#ifdef MMI_VOLTE_SUPPORT
#if !defined(MMI_ONLY_4G_DEL_2G_CUSTOM) && defined(MMI_VOLTE_SUPPORT)
    if(0 < MMIAPIPHONE_GetSimCardOkNum(PNULL, 0))
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_LIST_SET_VOLTE, IMAGE_NULL, ID_SET_VOLTE);
    }
#endif
   AppendPdaCallSettingsItem1Line(ctrl_id, TXT_TOOLS_OTHERS, IMAGE_NULL, ID_SET_OTHER);//delete by James li
}

/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallSettingsWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_MENU_ID_T               menu_id     =   0;
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    MMISET_SIM_SELECT_TYPE_E    select_type =   0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    uint16 index = 0;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_CALL_MENU_CTRL_ID;
    MMI_STRING_T    input_str = {0};

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);   
    if (sim_num == 1)//cr227781 maryxiao
    {
        s_cur_dual_sys = (MN_DUAL_SYS_E)sim_sys; 
    }    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitPdaCallSettingsCtrl();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:           
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &menu_id);

        switch (menu_id)
        {
#ifndef MMI_MULTI_SIM_SYS_SINGLE     
        case ID_SET_MULTI_SIM_SETTING:
            MMK_CreateWin((uint32 *)MMISET_MULTI_SIM_SETTING_WIN_TAB, PNULL);
            break;  
#endif
        case ID_SET_CALL_DIVERTS:
            MMIAPISET_OpenDivertSettingsWindow();
            break;

        case ID_SET_CALL_BAR:
            OpenBarSettingsWindow();
            break;
            
        case ID_SET_CALL_WAITING:			
            MMISET_SetDivertBarWatingHideidCtrlId(MMISET_CALLWATINGT_FORM_CTRL_ID);
            select_type = MMISET_CALL_WAITING;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if(sim_num == 1)
            {
                CreateCallWaitingtSelectPopWindow();
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;
#ifdef VT_SUPPORT
#ifdef MMI_DUALMODE_ENABLE
        case ID_SET_VIDEOPHONE:
            MMK_CreateWin((uint32*)MMISET_SET_VT_WIN_TAB,PNULL);        
            break;
#endif
#endif   
#ifdef MMI_CSP_SUPPORT
        case ID_SET_LINE_SELECT:
            MMIAPISET_SetLineInfo();
            break;

        case ID_SET_LINE_LOCK: //line lock
            //verify pin2 before open line lock setting win
            SetLineLock();
            break;            
#endif


     /* +qingfeng.dong */
        case ID_SET_NETWORK://ID_SET_NETWORK
            select_type = MMISET_SET_NETWORK;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (sim_num == 1)
            {
                MMIAPISET_OpenSelNetWorkWin((MN_DUAL_SYS_E)sim_sys);
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;

#ifndef SUBLCD_SIZE_NONE
        case ID_SET_FLIP_ANSWER:
            MMIAPISET_OpenUssdSelectWin(MMISET_FLIP_ANSWER);
            break;
#endif

#ifdef MMI_DUALMODE_ENABLE
        case ID_SET_NETSETTING:
            MMIAPISET_OpenSelNetWorkWin((MN_DUAL_SYS_E)sim_sys);
            break;
#endif

        case ID_SET_FLY_MODE:
#ifdef MCARE_V31_SUPPORT
            if(MCareV31_Exit_Confirm())
            {
                return MMI_RESULT_FALSE;          
            }
#endif
            MMK_CreateWin((uint32*)MMISET_FLY_MODE_SET_WIN_TAB,PNULL);
            break;
     /* -qingfeng.dong */
            
        case ID_SET_HIDE_ID:		
            select_type = MMISET_CALL_HIDE_ID;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (sim_num == 1)
            {
                MMK_CreatePubListWin((uint32*)MMISET_SET_CALL_HIDE_ID_WIN_TAB, (ADD_DATA)sim_sys);
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;

        case ID_SET_VOICE_MAIL:
            select_type = MMISET_CALL_VOICEMAIL;
#ifdef HERO_ENGINE_TTS_SUPPORT 
{
	extern uint8 s_hero_no_tts;
	s_hero_no_tts = 1;
}
#endif
#ifdef MMI_CSP_SUPPORT
            sim_num = GetCallFowardVisibleSimNum(&sim_sys, 1);
#else
            sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
#endif
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else
            {
                MMISET_SetVmOperType(MMISET_VMN_OPER_TYPE_CALL_SETTING);			
#ifdef MMI_MULTI_SIM_SYS_SINGLE
                s_cur_dual_sys =  0;
                GetVoiceMailAddress(s_cur_dual_sys, MN_MULTI_SUBSCRIBER_ID_1,  MN_VOICE_MAIL);
#else
                if((SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))&&(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2)))
                {
                    MMIRES_GetText(STR_QUERY_VOICEMAIL_NUM_EXT01, PNULL, &input_str);
                    //MMICC_OpenVoicemailSettingSelectSimWin();
                    MMIAPISET_OpenUssdSelectWin(select_type);
                }else if((SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))&&
                    (SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2)))
                {
                    s_cur_dual_sys =  0;
                    GetVoiceMailAddress(s_cur_dual_sys, MN_MULTI_SUBSCRIBER_ID_1,  MN_VOICE_MAIL);
                }else if((SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2))&&
                    (SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1)))
                {
                    s_cur_dual_sys =  1;
                    GetVoiceMailAddress(s_cur_dual_sys, MN_MULTI_SUBSCRIBER_ID_1,  MN_VOICE_MAIL);
                }
                else
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
#endif
            }
            break;

        case ID_SET_OTHER:
            MMK_CreateWin((uint32*)MMISET_SET_CALL_OTHER_WIN_TAB, PNULL);
            break;
//#ifdef MMI_VOLTE_SUPPORT
#if !defined(MMI_ONLY_4G_DEL_2G_CUSTOM) && defined(MMI_VOLTE_SUPPORT)
        case ID_SET_VOLTE:
            if (MMIAPISET_GetFlyMode())
            {
                MMIAPISET_AlertFlyMode();
            }
            else
            {
                MMIAPIENG_SHOWVOLTE_WINDOW();
            }
            break;
#endif
        default:
            break;
        }
        break;
        
    case MSG_GET_FOCUS:
         index = GUILIST_GetCurItemIndex(ctrl_id); 
         GUILIST_RemoveAllItems(ctrl_id);
         InitPdaCallSettingsCtrl();
         GUILIST_SetCurItemIndex(ctrl_id, index); 
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}
#else
/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallSettingsWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_MENU_GROUP_ID_T         group_id    =   0;
    MMI_MENU_ID_T               menu_id     =   0;
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
     MMISET_SIM_SELECT_TYPE_E    select_type =   0;
    // MN_DUAL_SYS_E               dual_sys    =   MN_DUAL_SYS_MAX;
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);   
    if (sim_num == 1)//cr227781 maryxiao
    {
        s_cur_dual_sys = (MN_DUAL_SYS_E)sim_sys; 
    }    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_GetId(MMISET_SET_CALL_MENU_CTRL_ID,&group_id,&menu_id);
        
        MMK_SetAtvCtrl(win_id, MMISET_SET_CALL_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
#ifdef MMI_CSP_SUPPORT
		if(MMIAPIPHONE_IsCPHSSupport())
        {
            MMI_CTRL_ID_T ctrl_id = MMISET_SET_CALL_MENU_CTRL_ID;
            uint32 group = MENU_SET_CALL_OPT;
            
            if(GetCallFowardVisibleSimNum(PNULL, 0) > 0)
            {
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_CALL_DIVERTS, FALSE);
            
            if(GetCallBarVisibleSimNum(PNULL, 0) > 0)
            {
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_CALL_BAR, FALSE);
            }
            if(GetSelectLineVisibleSimNum(PNULL, 0) > 0)
            {
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_LINE_SELECT, FALSE);
            }
            if(GetLockLineVisibleSimNum(PNULL, 0) > 0)
            {
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_LINE_LOCK, FALSE);
            }
        }     
#endif        
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:   
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        GUIMENU_GetId(MMISET_SET_CALL_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
#ifndef MMI_MULTI_SIM_SYS_SINGLE     
        case ID_SET_MULTI_SIM_SETTING:
            MMK_CreateWin((uint32 *)MMISET_MULTI_SIM_SETTING_WIN_TAB, PNULL);
            break;  
#endif
           
        case ID_SET_CALL_DIVERTS:
            MMIAPISET_OpenDivertSettingsWindow();
            break;
            
        case ID_SET_CALL_BAR:
            OpenBarSettingsWindow();
            break;
            
        case ID_SET_CALL_WAITING:
            MMISET_SetDivertBarWatingHideidCtrlId(MMISET_CALLWATINGT_FORM_CTRL_ID);
            select_type = MMISET_CALL_WAITING;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if(sim_num == 1)
            {
                CreateCallWaitingtSelectPopWindow();
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;
#ifdef VT_SUPPORT
#ifdef MMI_DUALMODE_ENABLE
        case ID_SET_VIDEOPHONE:
            MMK_CreateWin((uint32*)MMISET_SET_VT_WIN_TAB,PNULL);        
            break;
#endif
#endif   
#ifdef MMI_CSP_SUPPORT
        case ID_SET_LINE_SELECT:
            MMIAPISET_SetLineInfo();
            break;

        case ID_SET_LINE_LOCK: //line lock
            //verify pin2 before open line lock setting win
            SetLineLock();
            break;            
#endif

     /* +qingfeng.dong */
        case ID_SET_NETWORK://ID_SET_NETWORK
            select_type = MMISET_SET_NETWORK;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (sim_num == 1)
            {
                MMIAPISET_OpenSelNetWorkWin((MN_DUAL_SYS_E)sim_sys);
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;

#ifdef MMI_DUALMODE_ENABLE
        case ID_SET_NETSETTING:
            MMIAPISET_OpenSelNetWorkWin(sim_sys);
            break;
#endif

        case ID_SET_FLY_MODE:
#ifdef MCARE_V31_SUPPORT
            if(MCareV31_Exit_Confirm())
            {
                return MMI_RESULT_FALSE;     ///具体的return值不同，和添加出的函数相关
            }
#endif
            MMK_CreateWin((uint32*)MMISET_FLY_MODE_SET_WIN_TAB,PNULL);
            break;
     /* -qingfeng.dong */
               
        case ID_SET_HIDE_ID:
            select_type = MMISET_CALL_HIDE_ID;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (sim_num == 1)
            {
                MMK_CreatePubListWin((uint32*)MMISET_SET_CALL_HIDE_ID_WIN_TAB, (ADD_DATA)sim_sys);
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;
#ifdef MMI_IP_CALL_SUPPORT
#ifdef DPHONE_SUPPORT 
        case ID_SET_IP_CALL:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        select_type = MMISET_IP_CALL;
        
        //判断是否存在两张卡同时开启，不是则启动active sim
        if (0 == sim_num)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else if (1 == sim_num)
        {
            MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)sim_sys);
        }
        else
        {
            MMIAPISET_OpenUssdSelectWin(select_type);
        }
#else
        MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_1);
#endif
        break;
#endif
        case ID_SET_MINUTE_REMINDER://分钟提示音
            MMK_CreateWin((uint32*)MMISET_SET_MINUTE_REMINDER_WIN_TAB,PNULL);
            break;

        case ID_SET_SPEED_DIAL://延时拨号
            MMK_CreateWin((uint32*)MMISET_SET_SPEED_DIAL_WIN_TAB,PNULL);
            break;

        case ID_SET_AUTO_REDIAL://自动重播
            MMK_CreateWin((uint32*)MMISET_SET_AUTO_REDIAL_WIN_TAB,PNULL);
            break;
            
        case ID_SET_ANYKEY_REJECT_MODE:
            MMK_CreateWin((uint32*)MMISET_SET_ANYKEY_REJECT_MODE_WIN_TAB,PNULL);
            break;
#else
        case ID_SET_OTHER:
            MMK_CreateWin((uint32*)MMISET_SET_CALL_OTHER_WIN_TAB, PNULL);
            break;
#endif
//#ifdef MMI_VOLTE_SUPPORT
#if !defined(MMI_ONLY_4G_DEL_2G_CUSTOM) && defined(MMI_VOLTE_SUPPORT)
        case ID_SET_VOLTE:
            MMIAPIENG_SHOWVOLTE_WINDOW();
            break;
#endif
            
        default:
            break;
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}
#endif


#ifdef MULTI_SIM_GSM_CTA_SUPPORT
/*****************************************************************************/
//  Description : Open CTA MULTI SIM Standby Win
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenCTAMultiSysStandbyWin(BOOLEAN is_power_select)
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (MMIAPIPHONE_GetSimExistedNum(PNULL, 0) > 1)
    {
        if (is_power_select
            || (MMIAPIPB_IsPbReady()
            && MMIAPISMS_IsOrderOk()))
        {
            MMK_CreatePubListWin((uint32*)MMISET_CTA_MULTI_SYS_STANDBY_WIN_TAB, (ADD_DATA)is_power_select);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        }
    }
    else
#endif
    {
        MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, TRUE);

        if (is_power_select)
        {
            uint32 i = 0;
            MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (MMIAPIPHONE_GetSimExistedStatus(i) && !(e_dualsys_setting & (0x01 << i)))
                {
                    ImplementCTASendSIMNotReadInd((MN_DUAL_SYS_E)i);
                }
            }

            // 打开正常开机窗口
            MMIAPIPHONE_OpenNormalStartUpWin();
        }
    }
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : to handle CTA Multi Sys Standby window msg
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCTAMultiSysStandbyWindow(
                                            MMI_WIN_ID_T       win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    uint32 i = 0;
    uint16 cur_selection = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T list_ctrl = MMISET_SET_CTA_MULTI_SYS_STANDBY_CTRL_ID;
    LOCAL BOOLEAN s_is_standby[MMI_DUAL_SYS_MAX] = {0};
    LOCAL MMISET_DUALSYS_TYPE_E old_dualsys_setting = MMISET_DAULSYS_ONLY_SIM1_ON;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    uint32 sim_power_select = (uint32)MMK_GetWinAddDataPtr(win_id);
    BOOLEAN is_power_select = (BOOLEAN)sim_power_select;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        old_dualsys_setting = e_dualsys_setting;
        SCI_MEMSET(s_is_standby, 0x00, sizeof(s_is_standby));
        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (e_dualsys_setting & (0x01 << i))
            {
                s_is_standby[i] = TRUE;
            }
        }
        MMK_SetAtvCtrl(win_id, list_ctrl);
        break;

    case MSG_FULL_PAINT:
        AppendCTAMultiSysTypeList(list_ctrl);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            BOOLEAN is_has_selected = FALSE;

            //get cur selection
            cur_selection = GUILIST_GetCurItemIndex(list_ctrl);

            s_is_standby[cur_selection] = !s_is_standby[cur_selection];

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (s_is_standby[i])
                {
                    is_has_selected = TRUE;
                    break;
                }
            }

            if (!is_has_selected)
            {
                s_is_standby[cur_selection] = !s_is_standby[cur_selection];

                MMIPUB_OpenAlertWarningWin( TXT_SET_STANDBY_MODE_ALERT);
            }
            else
            {
                if (!ImplementCTAStandbyChanged(s_is_standby, is_power_select))
                {
                    s_is_standby[cur_selection] = !s_is_standby[cur_selection];

                    MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                }
            }
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if (is_power_select)
        {
            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (MMIAPIPHONE_GetSimExistedStatus(i) && !(e_dualsys_setting & (0x01 << i)))
                {
                    ImplementCTASendSIMNotReadInd((MN_DUAL_SYS_E)i);
                }
            }

            MMIAPICOM_StatusAreaUpdateSignalState();
            MMIAPISET_SetIsQueryCfu(FALSE, MMI_DUAL_SYS_MAX);

            MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, TRUE);

            // 打开正常开机窗口
            MMIAPIPHONE_OpenNormalStartUpWin();
        }
        else if (old_dualsys_setting != e_dualsys_setting)
        {
            MMI_STRING_T wait_text = {0};
            //进入等待界面
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                s_is_cta_deactive_ps[i] = TRUE;

                MMIAPIPHONE_DeactivePS((MN_DUAL_SYS_E)i);
            }

            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMISET_SET_CTA_MULTISYS_STANDBY_WAITING_WIN_ID,IMAGE_NULL,
                                            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleCTAStandbyWaitingWindow);
        }

        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : AppendCTAMultiSysTypeList
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AppendCTAMultiSysTypeList(MMI_CTRL_ID_T ctrl_id)
{
    uint32 i = 0;
    uint16 cur_selection = 0;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_IMAGE_ID_T status_image_id = IMAGE_NULL;
    MMI_IMAGE_ID_T sim_image_id = IMAGE_NULL;
    MMISET_SIM_NAME_T sim_str = {0};
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

    MMI_IMAGE_ID_T sim_img_id[MMI_DUAL_SYS_MAX] ={0};
    MMI_IMAGE_ID_T no_sim_img_id[MMI_DUAL_SYS_MAX] = {0};
    MMI_IMAGE_ID_T gray_sim_img_id[MMI_DUAL_SYS_MAX] = {0};

    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        sim_img_id[i]      = IMAGE_IDLE_PROMPT_SIM1      + (i-MN_DUAL_SYS_1);    //shoule IMAGE_IDLE_PROMPT_NO_SIM2 == IMAGE_IDLE_PROMPT_NO_SIM1+1.atc
        no_sim_img_id[i]   = IMAGE_IDLE_PROMPT_NO_SIM1   + (i-MN_DUAL_SYS_1);
        gray_sim_img_id[i] = IMAGE_IDLE_PROMPT_GRAY_SIM1 + (i-MN_DUAL_SYS_1);
    }

    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX, FALSE);
    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        SCI_MEMSET(&sim_str, 0x00, sizeof(sim_str));
        SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

        if (e_dualsys_setting & (0x01 << i))
        {
            status_image_id = IMAGE_CHECK_SELECTED_ICON;

            item_data.softkey_id[LEFT_BUTTON] = TXT_CLOSE;
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)TXT_COMMON_OK;

            if (MMIAPIPHONE_GetSimExistedStatus(i))
            {
                sim_image_id = sim_img_id[i];
            }
            else
            {
                sim_image_id = no_sim_img_id[i];
            }
        }
        else
        {
            status_image_id = IMAGE_CHECK_UNSELECTED_ICON;

            item_data.softkey_id[LEFT_BUTTON] = TXT_OPEN;
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)TXT_COMMON_OK;

            if (MMIAPIPHONE_GetSimExistedStatus(i))
            {
                sim_image_id = gray_sim_img_id[i];
            }
            else
            {
                sim_image_id = no_sim_img_id[i];
            }
        }

        sim_str = MMIAPISET_GetSimName(i);
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = sim_image_id;
        
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = MIN(GUILIST_STRING_MAX_NUM, sim_str.wstr_len);

        item_data.item_content[1].item_data.text_buffer.wstr_ptr = (wchar *)sim_str.wstr_arr;

        item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[2].item_data.image_id = status_image_id;

        GUILIST_AppendItem(ctrl_id, &item_t);
    }

    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
 }

/*****************************************************************************/
//  Description : ImplementCTAStandbyChanged
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ImplementCTAStandbyChanged(BOOLEAN * is_standby_ptr, BOOLEAN is_power_select)
{
    uint32 i = 0;
    BOOLEAN is_allow_set = FALSE;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MMISET_DUALSYS_TYPE_E standby_mode = 0;

    //SCI_ASSERT(PNULL != is_standby_ptr);
   if(PNULL == is_standby_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert ImplementCTAStandbyChanged"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_3086_112_2_18_2_50_22_66,(uint8*)"");
        return FALSE;
    } 
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (is_standby_ptr[i])
        {
            standby_mode |= (0x01 << i);
        }
    }

    if (e_dualsys_setting != standby_mode)
    {
        if (is_power_select
            ||(MMIAPIPB_IsPbReady()
            && MMIAPISMS_IsOrderOk()))
        {
            is_allow_set = TRUE;
        }

        if (!is_allow_set)
        {
            return FALSE;
        }

        //记录用户自己设置的情况
        MMISET_SetMultiSysSetting(standby_mode);

        MMIAPICOM_StatusAreaUpdateSignalState();
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : ImplementCTASendSIMNotReadInd
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void ImplementCTASendSIMNotReadInd(MN_DUAL_SYS_E dual_sys)
{
    //send sim_not_ready
    APP_MN_SIM_NOT_READY_IND_T *ind_ptr = PNULL;
    
    SCI_CREATE_SIGNAL(
        ind_ptr,
        APP_MN_SIM_NOT_READY_IND,
        sizeof(APP_MN_SIM_NOT_READY_IND_T),
        P_MN);
    
   // SCI_ASSERT(ind_ptr != PNULL);
   if(PNULL == ind_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert ImplementCTASendSIMNotReadInd"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_3138_112_2_18_2_50_22_67,(uint8*)"");
        return;
    }     
    ind_ptr->reason = MNSIM_ERROR_INSERTED_ERROR;
    ind_ptr->dual_sys = dual_sys;
    
    SCI_SEND_SIGNAL((xSignalHeaderRec*)ind_ptr, P_APP);
}

/*****************************************************************************/
//  Description : Standby 等待窗口
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCTAStandbyWaitingWindow (
                                           MMI_WIN_ID_T      win_id,    // 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id,     // 窗口的内部消息ID
                                           DPARAM               param       // 相应消息的参数
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
   // case MSG_APP_RED:
     //   break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Handle Implement CTA Standby Mode Async Timer
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleImplementCTAStandbyModeAsyncTimer(
                               uint8    timer_id,
                               uint32   param
                               )
{
    if (s_cta_deactive_ps_timer == timer_id)
    {
        uint32 i =0;
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (MMIAPIPHONE_GetSimExistedStatus(i) && !(e_dualsys_setting & (0x01 << i)))
            {
                ImplementCTASendSIMNotReadInd((MN_DUAL_SYS_E)i);
            }
        }

        s_cta_deactive_ps_timer = 0;

        MMIAPISMS_OrderInit();

        //MMIAPIPB_SetAllSIMNotInit();

        MMIAPICOM_StatusAreaUpdateSignalState();
        MMIAPISET_SetIsQueryCfu(FALSE, MMI_DUAL_SYS_MAX);

        MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, TRUE);

#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif

        MMK_CloseWin(MMISET_SET_CTA_MULTISYS_STANDBY_WAITING_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : Implement CTA Standby Mode
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_ImplementCTAStandbyModeAsync(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    s_is_cta_deactive_ps[dual_sys] = FALSE;

    if (MMK_IsOpenWin(MMISET_SET_CTA_MULTISYS_STANDBY_WAITING_WIN_ID))
    {
        uint32 i = 0;
        BOOLEAN s_is_cta_deactive_finished = TRUE;

        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (s_is_cta_deactive_ps[i])
            {
                s_is_cta_deactive_finished = FALSE;

                break;
            }
        }

        if (s_is_cta_deactive_finished)
        {
            if (0 != s_cta_deactive_ps_timer)
            {
                MMK_StopTimer(s_cta_deactive_ps_timer);
                s_cta_deactive_ps_timer = 0;
            }

            s_cta_deactive_ps_timer = MMK_CreateTimerCallback(3000, HandleImplementCTAStandbyModeAsyncTimer, NULL, FALSE);
        }
    }
}
#endif
#endif

/*****************************************************************************/
//  Description : to open call settings main menu window
//  Global resource dependence : none
//  Author: xiangjie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenCallSettingsWindow(void)
{
     MMK_CreateWin((uint32*)MMISET_SET_CALL_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : to open Divert settings main menu window
//  Global resource dependence : none
//  Author: xiangjie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenDivertSettingsWindow(void)
{
    MMISET_SIM_SELECT_TYPE_E    select_type = 0;

    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
#ifdef VT_SUPPORT
    uint32 win_id = MMISET_CALLDIVERT_FORM_CTRL_ID;
#endif

    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return;
    }
    MMISET_SetDivertBarWatingHideidCtrlId(MMISET_CALLDIVERT_FORM_CTRL_ID);
    select_type = MMISET_CALL_FORWARD;
#ifdef MMI_CSP_SUPPORT
    sim_num = GetCallFowardVisibleSimNum(&sim_sys, 1);
#else
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
#endif
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
#ifdef VT_SUPPORT
        MMK_CreateWin((uint32*)MMISET_SET_DIVERT_AUDIOORVIDEO_CALL_WIN_TAB, (ADD_DATA)win_id);   
#else
        MMK_CreateWin((uint32*)MMISET_SET_CALL_DIVERT_WIN_TAB, (ADD_DATA)sim_sys);

#endif
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#endif
}

/*****************************************************************************/
//  Description : to open bar settings main menu window
//  Global resource dependence : none
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void OpenBarSettingsWindow(void)
{
    MMISET_SIM_SELECT_TYPE_E    select_type = 0;

    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
#ifdef VT_SUPPORT
    uint32 win_id = MMISET_CALLBAR_FORM_CTRL_ID;
#endif

    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return;
    }
    MMISET_SetDivertBarWatingHideidCtrlId(MMISET_CALLBAR_FORM_CTRL_ID);
    select_type = MMISET_CALL_BAR;

#ifdef MMI_CSP_SUPPORT
    sim_num = GetCallFowardVisibleSimNum(&sim_sys, 1);
#else
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
#endif
    
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
#ifdef VT_SUPPORT
        MMK_CreateWin((uint32*)MMISET_SET_BAR_AUDIOORVIDEO_CALL_WIN_TAB, (ADD_DATA)win_id);    
#else
        MMK_CreateWin((uint32*)MMISET_SET_CALL_BAR_WIN_TAB, (ADD_DATA)sim_sys);
#endif
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#endif
}


#ifdef VT_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : InitCallDivertList
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void  InitVTCallSelectCallTypeList(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_CALL_MENU_EX_CTRL_ID;

    GUILIST_SetMaxItem(ctrl_id, 3, FALSE );

    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_CALL_AUDIO, IMAGE_NULL, ID_SET_DIVERTS_AUDIO);
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_VIDEOCALL, IMAGE_NULL, ID_SET_DIVERTS_VIDEO);
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_AUDIOANDVIDEO_CALL, IMAGE_NULL, ID_SET_DIVERTS_AUDIOANDVIDEO);
}
#endif
/*****************************************************************************/
//  Description : handle select call type window
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleSelectCallTypeGeneralWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    uint32              wind_id    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_CALL_MENU_EX_CTRL_ID;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
#if defined MMI_GUI_STYLE_TYPICAL
            if(wind_id == MMISET_CALLDIVERT_FORM_CTRL_ID)
            {
                GUIWIN_SetTitleTextId(MMISET_SET_SELECT_DIVERT_WIN_ID, TXT_SET_CALL_DIVERTS, TRUE);
            }
            else
            {
                GUIWIN_SetTitleTextId(MMISET_SET_SELECT_DIVERT_WIN_ID, TXT_SET_CALL_BAR, TRUE);
            }    
#else
            InitVTCallSelectCallTypeList();
#endif
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
      case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:
            //GUIMENU_GetId(MMISET_SET_CALL_MENU_CTRL_ID,&group_id_3,&menu_id_3);
            if(wind_id == MMISET_CALLDIVERT_FORM_CTRL_ID)
            {
                MMK_CreateWin((uint32*)MMISET_SET_CALL_DIVERT_WIN_TAB,(ADD_DATA)ctrl_id);
            }
            else
            {
                MMK_CreateWin((uint32*)MMISET_SET_CALL_BAR_WIN_TAB,(ADD_DATA)ctrl_id);
            }
        break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
        break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
    
}
#endif



/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

LOCAL void InitDivertVoiceNumcontrol(MMI_WIN_ID_T    win_id)
{
      MMI_STRING_T label_name  = {0};
      MMI_CTRL_ID_T             called_fuc_id   =  MMK_GetActiveCtrlId(MMISET_SET_CALL_DIVERT_WIN_ID);

      switch(called_fuc_id)
      {
        case MMISET_DIVERT_UNCONDITION_LABEL_CTRL_ID:
            GUIWIN_SetTitleTextId(win_id,TXT_SET_DIVERTS_ALL_CALLS,FALSE);
            break;
        case MMISET_DIVERT_BUSY_LABEL_CTRL_ID:
            GUIWIN_SetTitleTextId(win_id,TXT_SET_DIVERTS_WHEN_BUSY,FALSE);
            break;
        case MMISET_DIVERT_NOANSWER_LABEL_CTRL_ID:
            GUIWIN_SetTitleTextId(win_id,TXT_SET_DIVERTS_NO_ANSWER,FALSE);
            break;
        case MMISET_DIVERT_UNRECHED_LABEL_CTRL_ID:
            GUIWIN_SetTitleTextId(win_id,TXT_SET_DIVERTS_NOT_REACHABLE,FALSE);
            break;
        default:
            break;
      }
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERT_VOICE, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_VOICE_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERT_NUMBER, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_NUM_LABEL_CTRL_ID, &label_name, FALSE);

    
}
/*****************************************************************************/
//  Description : set call diver voice or num title
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void SetCallDivertVoiceOrNumTitle(MMI_CTRL_ID_T ctrl_id)
{
    MMI_CTRL_ID_T menu_ctrl_id        = MMISET_VOICE_NUM_CTRL_ID;
    
    MMI_STRING_T title_text_Str  = {0};
    GUIMENU_TITLE_INFO_T title_info = {0};
    SET_ID_MENU_E menu_id = ID_SET_DIVERTS_ALL_CALLS;
    MMI_TEXT_ID_T text_id = TXT_NULL;
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_MENU_GROUP_ID_T group_id = 0;

    GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
#else
    uint16 index = GUILIST_GetCurItemIndex(ctrl_id);
     // bug 171530
#ifdef MMI_CSP_SUPPORT
     MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
     if (!(MMISET_IsAlsValid(dual_sys) && IsSSMenuItemVisible(dual_sys, CSP_MENU_CFB, CSP_MENU_OPER_NULL)
          && IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRY, CSP_MENU_OPER_NULL) && IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRC, CSP_MENU_OPER_NULL))
          && index > 0)
     {
          index++;
     }
#endif
    menu_id = menu_id + index;
#endif
    title_info.is_static    = TRUE;
    title_info.group_id = MENU_SET_SS_DIVERT_OPT; 
    switch (menu_id)
    {
    case ID_SET_DIVERTS_ALL_CALLS://?????
        text_id = TXT_SET_DIVERTS_ALL_CALLS;
        break;

#ifdef MMI_CSP_SUPPORT
    case ID_SET_DIVERTS_ALL_CONDITIONAL://????
        text_id = TXT_SET_DIVERTS_ALL_CONDITIONAL;
        break;
#endif
        
    case ID_SET_DIVERTS_WHEN_BUSY://?????
        text_id = TXT_SET_DIVERTS_WHEN_BUSY;
        break;
        
    case ID_SET_DIVERTS_NO_ANSWER://?????
        text_id = TXT_SET_DIVERTS_NO_ANSWER;
        break;
        
    case ID_SET_DIVERTS_NOT_REACHABLE://??????
        text_id = TXT_SET_DIVERTS_NOT_REACHABLE;
        break;
        
    default:
        //do nothing
        break;
   }
    MMI_GetLabelTextByLang(text_id, &title_text_Str);
    title_info.title_ptr    = &title_text_Str;
    GUIMENU_SetMenuTitleEx(menu_ctrl_id, &title_info);

}
/*****************************************************************************/
//  Description : to do num divert
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOpenNumDivertWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    //MMI_CTRL_ID_T ctrl_id = MMISET_VOICE_NUM_CTRL_ID;
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    //uint8                       record_id       =   0;
    MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type = MN_MULTI_SUBSCRIBER_MAX;
    MN_VOICE_MAIL_TYPE_E voice_mail_type = MN_VOICE_MAIL;
    //MMI_STRING_T                wait_text       =   {0};
    uint32                      dual_sys        =   s_cur_dual_sys;
    
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
     //InitDivertVoiceNumcontrol(win_id);
        MMK_SetAtvCtrl(win_id, MMISET_VOICE_NUM_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        SetCallDivertVoiceOrNumTitle(MMISET_CALL_DIVERT_MENU_CTRL_ID);
#ifdef MMI_CSP_SUPPORT                    
        {
            BOOLEAN is_voicemail_visible = TRUE;
            BOOLEAN is_number_visible = TRUE;
            //MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_MENU_ID_T item_id = 0;
            CSP_MENU_SS_TYPE_E ss_type = CSP_MENU_NULL;
            GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID,&group_id,&item_id);
            switch(item_id)
            {
                case ID_SET_DIVERTS_ALL_CALLS:
                    ss_type = CSP_MENU_CFU;
                    is_voicemail_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_VM);
                    is_number_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_NO);
                    break;

#ifdef MMI_CSP_SUPPORT
                case ID_SET_DIVERTS_ALL_CONDITIONAL:
                    ss_type = CSP_MENU_CFB; //?cfb
                    is_voicemail_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_VM);
                    is_number_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_NO);
                    break;                    
#endif

                case ID_SET_DIVERTS_WHEN_BUSY:
                    ss_type = CSP_MENU_CFB;
                    is_voicemail_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_VM);
                    is_number_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_NO);
                    break;
                    
                case ID_SET_DIVERTS_NO_ANSWER:
                    ss_type = CSP_MENU_CFNRY;
                    is_voicemail_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_VM);
                    is_number_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_NO);
                    break;
                    
                case ID_SET_DIVERTS_NOT_REACHABLE:
                    ss_type = CSP_MENU_CFNRC;
                    is_voicemail_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_VM);
                    is_number_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_NO);
                    break;

                default:
                    //do nothing
                    break;
                    
            }
            GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_OPT, ID_SET_SS_DIVERT_VOICE, is_voicemail_visible);
            GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_OPT, ID_SET_SS_DIVERT_NUMBER, is_number_visible);            

            if(MMISET_IsAlsValid(dual_sys))
            {
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE_VMN, FALSE);
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE1_VMN, TRUE);
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE2_VMN, TRUE);
            }
            else
            {
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE_VMN, TRUE);
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE1_VMN, FALSE);
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE2_VMN, FALSE);
            }
        }
#endif //MMI_CSP_SUPPORT           
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId( MMISET_VOICE_NUM_CTRL_ID, &group_id, &menu_id);       
        //record dual_sys type
        MMIAPISET_SetSysForCf(dual_sys);/*lint !e64*/
        
        switch (menu_id)
        {
        case ID_SET_SS_LINE_VMN:
            //record_id = 1;
               line_type = MN_MULTI_SUBSCRIBER_ID_1;
               voice_mail_type = MN_VOICE_MAIL;
            g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_CALL_DIVERT;
            //?sim??????????
            GetVoiceMailAddress(dual_sys, line_type, g_vmn_oper_type);/*lint !e64*/
            break;
            
#ifdef MMI_CSP_SUPPORT        
        case ID_SET_SS_LINE1_VMN:
            //record_id = 1;
            line_type = MN_MULTI_SUBSCRIBER_ID_1;
               voice_mail_type = MN_VOICE_MAIL;
            g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_CALL_DIVERT;
            //从sim卡里读取语音信箱号码
            GetVoiceMailAddress(dual_sys, line_type,  voice_mail_type);/*lint !e64*/
            break;
            
        case ID_SET_SS_LINE2_VMN:
            //record_id = 2;
            line_type = MN_MULTI_SUBSCRIBER_ID_2;
               voice_mail_type = MN_VOICE_MAIL;
            g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_CALL_DIVERT;
            //从sim卡里读取语音信箱号码
            GetVoiceMailAddress(dual_sys, line_type,  voice_mail_type);/*lint !e64*/
            break;
#endif //   MMI_CSP_SUPPORT 
            
        case ID_SET_SS_FAX_VMN:
            //record_id = 3;
            line_type = MN_MULTI_SUBSCRIBER_ID_1;
               voice_mail_type = MN_VOICE_MAIL_FAX;
            g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_CALL_DIVERT;
            //从sim卡里读取语音信箱号码
            GetVoiceMailAddress(dual_sys, line_type, voice_mail_type);/*lint !e64*/
            break;
            
        case ID_SET_SS_DATA_VMN:
            //record_id = 4;
            line_type = MN_MULTI_SUBSCRIBER_ID_1;
               voice_mail_type = MN_VOICE_MAIL_EMAIL;
            g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_CALL_DIVERT;
            //从sim卡里读取语音信箱号码
            GetVoiceMailAddress(dual_sys, line_type,  voice_mail_type);/*lint !e64*/

            break;
            
          case ID_SET_SS_DIVERT_NUMBER:
            {
                OpenSetCallDivertWin(dual_sys);               
            }
            break;
            
        default:
            break;
        }
        break;
       
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : create call divert pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void CreateCallDivertSelectPopWindow(MMI_CTRL_ID_T called_func_id)
{
    MMK_CreatePubListWin((uint32 *)MMISET_SELECT_POP_DIVERT_WIN_TAB,(ADD_DATA)called_func_id);
}

/*****************************************************************************/
//  Description : create call bar pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void CreateCallBarSelectPopWindow(MMI_CTRL_ID_T called_func_id)
{
    MMK_CreatePubListWin((uint32 *)MMISET_SELECT_POP_BAR_WIN_TAB,(ADD_DATA)MMISET_POPMENU_CALLSETTING_BAR_ID);
}
#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : InitCallDivertList
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void  InitCallDivertList(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_CALL_DIVERT_MENU_CTRL_ID;
    uint16        max_item_num = 5;

    // bug 171530
#ifdef MMI_CSP_SUPPORT    
    MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
    BOOLEAN is_cfu_visible = TRUE;
    BOOLEAN is_cfc_visible = TRUE; //All conditional call forwarding
    BOOLEAN is_cfb_visible = TRUE;
    BOOLEAN is_cfnry_visible = TRUE;
    BOOLEAN is_cfnrc_visible = TRUE;
    BOOLEAN is_cancel_all_visible = TRUE;

    if(dual_sys < MMI_DUAL_SYS_MAX)
    {                
        if(IsSSMenuItemVisible(dual_sys, CSP_MENU_CFU, CSP_MENU_OPER_NULL))
        {
            is_cfu_visible = TRUE;
        }
        else
        {
            is_cfu_visible = FALSE;
        }

        if(IsSSMenuItemVisible(dual_sys, CSP_MENU_CFB, CSP_MENU_OPER_NULL))
        {
            is_cfb_visible = TRUE;
        }
        else
        {
            is_cfb_visible = FALSE;
        }

        if(IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRY, CSP_MENU_OPER_NULL))
        {
            is_cfnry_visible = TRUE;
        }
        else
        {
            is_cfnry_visible = FALSE;
        }

        if(IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRC, CSP_MENU_OPER_NULL))
        {
            is_cfnrc_visible = TRUE;
        }
        else
        {
            is_cfnrc_visible = FALSE;
        }

        if(IsSSMenuItemVisible(dual_sys, CSP_MENU_CF, CSP_MENU_OPER_CANCEL_ALL))
        {
            is_cancel_all_visible = TRUE;
        }
        else
        {
            is_cancel_all_visible = FALSE;
        }

        //??????,??????,???????????,??????????
        if(MMISET_IsAlsValid(dual_sys) && is_cfb_visible && is_cfnry_visible && is_cfnrc_visible)     
        {
            is_cfc_visible = TRUE;
            is_cfb_visible = FALSE;
            is_cfnry_visible = FALSE;
            is_cfnrc_visible = FALSE;
            max_item_num = 3;
        }                
        else
        {
            is_cfc_visible = FALSE;
        }               
    }
#endif            
    
    GUILIST_SetMaxItem(ctrl_id, max_item_num, FALSE );
#ifdef MMI_CSP_SUPPORT
    if(is_cfu_visible)
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_ALL_CALLS, IMAGE_NULL, ID_SET_DIVERTS_ALL_CALLS);
    }
#ifdef MMI_CSP_SUPPORT
    if(is_cfc_visible)
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_ALL_CONDITIONAL, IMAGE_NULL, ID_SET_DIVERTS_ALL_CONDITIONAL); 
    }        
#endif 
#ifdef MMI_CSP_SUPPORT
    if(is_cfb_visible)
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_WHEN_BUSY, IMAGE_NULL, ID_SET_DIVERTS_WHEN_BUSY);
    } 
#ifdef MMI_CSP_SUPPORT
    if(is_cfnry_visible)
#endif 
    {
         AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_NO_ANSWER, IMAGE_NULL, ID_SET_DIVERTS_NO_ANSWER);
    }
#ifdef MMI_CSP_SUPPORT
    if(is_cfnrc_visible)
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_NOT_REACHABLE, IMAGE_NULL, ID_SET_DIVERTS_NOT_REACHABLE);
    }
#ifdef MMI_CSP_SUPPORT
    if(is_cancel_all_visible)
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_CANCEL_ALL, IMAGE_NULL, ID_SET_DIVERTS_CANCEL_ALL);
    }
}

/*****************************************************************************/
//  Description : InitCallBarList
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void  InitCallBarList(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_CALL_BAR_MENU_CTRL_ID;
#ifdef MMI_CSP_SUPPORT    
    MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
#endif 

    GUILIST_SetMaxItem(ctrl_id, 7, FALSE );   
#ifdef MMI_CSP_SUPPORT
    if((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_BAOC, CSP_MENU_OPER_NULL))
#endif         
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_ALL_MO_CALLS, IMAGE_NULL, ID_SET_BAR_ALL_MO_CALLS);
    }
#ifdef MMI_CSP_SUPPORT
    if((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_BAIC, CSP_MENU_OPER_NULL))
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_ALL_MT_CALLS, IMAGE_NULL, ID_SET_BAR_ALL_MT_CALLS);
    }
#ifdef MMI_CSP_SUPPORT
    if((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_BICROAM, CSP_MENU_OPER_NULL))
#endif 
    {
         AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_MT_CALLS_ROAM, IMAGE_NULL, ID_SET_BAR_MT_CALLS_ROAM);
    }
#ifdef MMI_CSP_SUPPORT
    if((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_BOIC, CSP_MENU_OPER_NULL))
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_MO_CALLS_INTL, IMAGE_NULL, ID_SET_BAR_MO_CALLS_INTL);
    }
#ifdef MMI_CSP_SUPPORT
    if((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_BOIC_EXHC, CSP_MENU_OPER_NULL))
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_MO_CALLS_INTL_ROAM, IMAGE_NULL, ID_SET_BAR_MO_CALLS_INTL_ROAM);
    }
#ifdef MMI_CSP_SUPPORT
    if((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_CB, CSP_MENU_OPER_NULL))
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_CANCEL_ALL, IMAGE_NULL, ID_SET_BAR_CANCEL_ALL);
    }
#ifdef MMI_CSP_SUPPORT
    if((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_CB, CSP_MENU_OPER_NULL))
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_CHANGE_PW, IMAGE_NULL, ID_SET_BAR_CHANGE_PW);
    }

}
#endif

LOCAL MMI_RESULT_E HandleCallDivertConditionsWin(
                                                          MMI_WIN_ID_T    win_id, 
                                                          MMI_MESSAGE_ID_E   msg_id, 
                                                          DPARAM             param
                                                          )
{
	
	MMI_MENU_GROUP_ID_T 		group_id		=	0;
	MMI_MENU_ID_T				menu_id 		=	0;
	MMI_RESULT_E				recode			=	MMI_RESULT_TRUE;
	
	MMI_CTRL_ID_T				ctrl_id = MMISET_CALL_DIVERT_CONDITIONS_CTRL_ID;

	GUILIST_ITEM_T		item_t = {0};
	GUILIST_ITEM_DATA_T item_data = {0};
	static uint16	 cur_item = 0;
	int i=0;
	BOOLEAN is_need_update = FALSE;
	MMI_STRING_T	title_str = {0};
	const CTRLLIST_ITEM_T *list_item_ptr = PNULL;

	switch(msg_id)
	{
	 case MSG_OPEN_WINDOW:
	 	 cur_item = 0;
		 break;
	 case MSG_FULL_PAINT:	
		MMK_SetAtvCtrl(win_id,ctrl_id);
		GUILIST_SetMaxItem(ctrl_id,2, FALSE );
		GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

		item_t.item_style	 = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
		
		item_t.item_data_ptr = &item_data;
		item_data.item_content[0].item_data_type	 = GUIITEM_DATA_TEXT_ID;			
		item_data.item_content[0].item_data.text_id = STR_FWD_CONDITION_EXT01;
		item_data.item_content[1].item_data_type	= GUIITEM_DATA_TEXT_ID;
        switch(MMIAPISET_GetForwardConditions())
        {
        case 0:
            item_data.item_content[1].item_data.text_id = STXT_DIVERT_ALL;
            break;
            
        case 1:
            item_data.item_content[1].item_data.text_id = TXT_SET_DIVERTS_WHEN_BUSY;
            break;
            
        case 2:
            item_data.item_content[1].item_data.text_id = TXT_SET_DIVERTS_NO_ANSWER;
            break;
            
        case 3:
            item_data.item_content[1].item_data.text_id = STXT_DIVERT_NOT_AVAIL;
            break;
            
        default :
            item_data.item_content[1].item_data.text_id = STXT_CANCEL_ALL_DIVERTS;
            break;
        }
		item_data.item_content[1].is_opacity = TRUE;
// 		GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
		item_data.softkey_id[0] = COMMON_TXT_NULL;
		item_data.softkey_id[1] = STXT_SELECT;
		item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
		GUILIST_AppendItem( ctrl_id, &item_t);
#ifdef MMI_MULTI_SIM_SYS_SINGLE
		item_t.item_style	  = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
		item_t.item_data_ptr = &item_data;
		item_data.item_content[0].item_data_type	  = GUIITEM_DATA_TEXT_ID;			 
		item_data.item_content[0].item_data.text_id = STR_LIST_SIM_EXT01;
		GUILIST_AppendItem( ctrl_id, &item_t);
#else
		item_t.item_style	 = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
		item_t.item_data_ptr = &item_data;
		item_data.item_content[0].item_data_type	 = GUIITEM_DATA_TEXT_ID;			
		item_data.item_content[0].item_data.text_id = STR_FWD_SIM_SELECTION_EXT01;
		item_data.item_content[1].item_data_type	= GUIITEM_DATA_TEXT_ID;
        switch(MMIAPISET_GetForwardSimCards())
        {
        case 0:
            item_data.item_content[1].item_data.text_id = STR_LIST_FROM_SIM1_EXT01;
            break;
            
        case 1:
            item_data.item_content[1].item_data.text_id = STR_LIST_FROM_SIM2_EXT01;
            break;
            
        case 2:
            item_data.item_content[1].item_data.text_id = STR_LIST_BOTH_SIM_EXT01;
            break;
     
        default :
            item_data.item_content[1].item_data.text_id = STR_LIST_FROM_SIM1_EXT01;
            break;
        }
		item_data.item_content[1].is_opacity = TRUE;
//		GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);	
        item_data.softkey_id[0] = COMMON_TXT_NULL;
        item_data.softkey_id[1] = STXT_SELECT;
        item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
        GUILIST_AppendItem( ctrl_id, &item_t);
#endif
        if(4==MMIAPISET_GetForwardConditions()){
        	GUILIST_SetItemGray(ctrl_id, 1, TRUE);
        	GUILIST_SetItemInvalid(ctrl_id, 1, TRUE);
        }
        GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_item);
        break;
//	case MSG_CTL_OK:
//	case MSG_APP_OK:
//	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
        cur_item = GUILIST_GetCurItemIndex(ctrl_id);
        switch (cur_item)
	 {
	 case 0:
	 	 MMK_CreateWin((uint32*)MMISET_SET_CALL_DIVERT_WIN_TAB, PNULL);
		 break;
	 case 1:
#ifdef MMI_MULTI_SIM_SYS_SINGLE
		 MMI_GetLabelTextByLang(STR_INPUT_PHONE_NUM_EXT01, &title_str);
		 MMIPUB_OpenDialogInputFieldWinByTextPtr(&title_str, PNULL, PNULL, PNULL, HandleDivertNumberWindow);
		 
#else
	     MMK_CreateWin((uint32*)MMISET_SET_CALL_DIVERT_SIMCARD_WIN, PNULL);
#endif
		 break;
	 default:
			break;
		}
	break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;
}
//add by ljg
PUBLIC void MMICC_OpenVoicemailSettingSelectSimWin(void)
{
    if (MMK_IsOpenWin(MMISET_SET_VOICEMAIL_SELECT_SIM_WIN_ID))
    {
        MMK_WinGrabFocus(MMISET_SET_VOICEMAIL_SELECT_SIM_WIN_ID);
    } 
    else
    {
        MMK_CreateWin((uint32*)MMISET_SELECT_SIM_VOICEMAIL_WIN_TAB, PNULL);  
    }
}


/*****************************************************************************/
//  Description : Implement get voicemail number form ndt and Multivar.
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_GetVoicemailFromNdtAndMultivar(MN_DUAL_SYS_E dual_sys)
{
    wchar vmtele_str[32] = {0};
    MMI_STRING_T    input_str = {0};
	
    MMIRES_GetText(STR_QUERY_VOICEMAIL_NUM_EXT01, PNULL, &input_str);	
    if (MMISET_GetIVoiceMailNumberToNV(dual_sys,vmtele_str)> 0)
    {
        MMIRES_GetText(STR_LABEL_VOICEMAIL_NUM_EXT01, PNULL, &input_str);
    }
    else
    {
        MMIRES_GetText(STR_QUERY_VOICEMAIL_NUM_EXT01, PNULL, &input_str);
    }
    MMIPUB_OpenDialogInputFieldWinByTextPtr(&input_str, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN_VMAIL, HandleSettingsEditVoiceMailNumberMsg);
}


/*****************************************************************************/
//  Description : Implement get voicemail number form MBDN of sim;
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_GetVoicemailFromMbdn(MN_DUAL_SYS_E dual_sys,uint16  num_len){
	MMI_STRING_T    input_str = {0};
	MMIRES_GetText(STR_QUERY_VOICEMAIL_NUM_EXT01, PNULL, &input_str);	
	if (num_len> 0)
	{
		MMIRES_GetText(STR_LABEL_VOICEMAIL_NUM_EXT01, PNULL, &input_str);
	}else{
		MMIRES_GetText(STR_QUERY_VOICEMAIL_NUM_EXT01, PNULL, &input_str);
	}
	MMIPUB_OpenDialogInputFieldWinByTextPtr(&input_str, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN_VMAIL, HandleSettingsEditVoiceMailNumberMsg);
}

PUBLIC void MMISET_GetVoicemailFromMbdnCallIdle(MN_DUAL_SYS_E dual_sys)
{
	MMI_STRING_T    input_str = {0};
	
    SCI_TRACE_LOW("MMISET_GetVoicemailFromMbdnCallIdle dual_sys = 0x%x", dual_sys);
    
    s_cur_dual_sys = dual_sys;
	MMIRES_GetText(STR_QUERY_VOICEMAIL_NUM_EXT01, PNULL, &input_str);
	MMIPUB_OpenDialogInputFieldWinByTextPtr(&input_str, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN_VMAIL, HandleSettingsEditVoiceMailNumberMsgCallIdle);
}
LOCAL BOOLEAN IsSCAddrValid(MMI_STRING_T *num_str_ptr)
{
    uint32 i = 0;
    BOOLEAN is_valid = TRUE;

    if (PNULL == num_str_ptr)
    {
        return FALSE;
    }
    if (0 < num_str_ptr->wstr_len)
    {
        for (i = 0; i < num_str_ptr->wstr_len; i++)
        {
            if (('+' != num_str_ptr->wstr_ptr[i] &&
                ('0' > num_str_ptr->wstr_ptr[i] ||
                '9' < num_str_ptr->wstr_ptr[i])
                )//输入必须是0-9或者'+'
                ||
                ('+' == num_str_ptr->wstr_ptr[i]
                && (0 < i))//'+'只能在最前面
                )
            {
                is_valid = FALSE;
                break;
            }
        }
    }
    else
    {
        is_valid = FALSE;
    }

    return is_valid;
}

LOCAL MMI_RESULT_E  HandleSettingsEditVoiceMailNumberMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
	{
        MMI_RESULT_E result = MMI_RESULT_TRUE;
        MMI_CTRL_ID_T edit_name_ctrl_id = MMIPUB_INPUTFIELD_CTRL_ID;
        uint8 vmtele_len = 0;	
        uint8 mail_len = 0;	
        wchar vmtele_str[32] = {0};
        MMI_STRING_T num_string = {0};
        MMI_STRING_T edit_str = {0};
        BOOLEAN vmn_cur_sim = FALSE;
        MN_DUAL_SYS_E s_cur_sim_sel = MN_DUAL_SYS_1;
        uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
        MMI_STRING_T num_ptr = {0};
        uint8 voice_mail_null[MMISMS_PBNUM_MAX_LEN + 2] = {0};
        wchar vmtele_null[32] = {0};
        wchar dot_str[] = {'.'};
        if(MN_DUAL_SYS_1 == s_cur_dual_sys){
            vmn_cur_sim = s_is_vmn_from_sim1;
        }else{
            vmn_cur_sim = s_is_vmn_from_sim2;
        }

        switch(msg_id)
        {
        case MSG_OPEN_WINDOW:
            MMIPUB_SetInputDialogImType(win_id, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL, GUIIM_TAG_CLASSIC_NUM);
            //GUIEDIT_ConfigImCustomKey(MMIPUB_INPUTFIELD_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
            //GUIEDIT_SetImTag(MMIPUB_INPUTFIELD_CTRL_ID, GUIIM_TAG_DOT_NUM);		

//             GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, STXT_SOFTKEY_OK_MK,TXT_NULL , TRUE);
//             GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_CANCEL_MK,0,TRUE);	
//             GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,2,TRUE);
            s_cur_sim_sel = s_cur_dual_sys;
            GUIEDIT_SetMaxLen(edit_name_ctrl_id, MMISET_MAX_VM_NUMBER_LEN);
            SCI_MEMSET(vmtele_str, 0x00, sizeof(vmtele_str));
            SCI_TRACE_LOW("ljg*HandleSettingsEditVoiceMailNumberMsg:s_cur_dual_sys=%d,vmn_cur_sim=%d",s_cur_dual_sys,vmn_cur_sim);

            if (vmn_cur_sim)
            {
                if(MN_DUAL_SYS_1 == s_cur_dual_sys)
                {
                    mail_len = MMIAPICOM_GenDispNumber( 
                        g_sim_vmnum_info.number_type, 
                        g_sim_vmnum_info.num_len, 
                        g_sim_vmnum_info.bcd_num, 
                        voice_mail,
                        MMISMS_PBNUM_MAX_LEN + 2);
                }
                else
                {
                    mail_len = MMIAPICOM_GenDispNumber( 
                        g_sim2_vmnum_info.number_type, 
                        g_sim2_vmnum_info.num_len, 
                        g_sim2_vmnum_info.bcd_num, 
                        voice_mail,
                        MMISMS_PBNUM_MAX_LEN + 2);
                }
                SCI_TRACE_LOW("ljg*HandleSettingsEditVoiceMailNumberMsg:mailbox_number=%s,mail_len=%d",voice_mail,mail_len);	
                mail_len = MIN(mail_len, MMISMS_PBNUM_MAX_LEN);
                MMI_STRNTOWSTR(vmtele_str,
                                MMISMS_PBNUM_MAX_LEN,
                                (uint8 *)voice_mail,
                                MMISMS_PBNUM_MAX_LEN,
                                mail_len);
                vmtele_len=mail_len;
            }
            else
            {
                vmtele_len=MMISET_GetIVoiceMailNumberToNV(s_cur_sim_sel,vmtele_str);
                MMI_WSTRNTOSTR(voice_mail_null, MMISMS_PBNUM_MAX_LEN, vmtele_str, MMISMS_PBNUM_MAX_LEN, vmtele_len);
                SCI_TRACE_LOW("ljg*MMISET_GetIVoiceMailNumberToNV:mailbox_number=%s,mail_len=%d",voice_mail_null,vmtele_len);	
                if(voice_mail_null[0] == 'N')
                {
                     vmtele_len = 0;
                     SCI_MEMSET(vmtele_str, 0x00, sizeof(vmtele_str));
                }
            }
            SCI_MEMSET(&num_string, 0, sizeof (num_string));
            num_string.wstr_ptr = vmtele_str;
            num_string.wstr_len = vmtele_len;
            if(MMIAPICOM_IsValidNumberStringEx(&num_string)) //fix Bug 790810 
            {
                GUIEDIT_SetString(edit_name_ctrl_id,
                    vmtele_str,
                    vmtele_len
                    );
            }
            break;
        case MSG_CTL_GET_ACTIVE:
        case MSG_NOTIFY_GET_ACTIVE:	
            result = MMIPUB_HandleDialogInputFieldWinMsg(win_id, msg_id, param);
            GUIEDIT_SetIm(MMIPUB_INPUTFIELD_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            break;
        case MSG_FULL_PAINT:
            UpdatePasswordWinSK(win_id);
            MMIPUB_SetInputDialogImType(win_id, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL, GUIIM_TAG_CLASSIC_NUM);
            //GUIEDIT_ConfigImCustomKey(MMIPUB_INPUTFIELD_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
            //GUIEDIT_SetImTag(MMIPUB_INPUTFIELD_CTRL_ID, GUIIM_TAG_DOT_NUM);		
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            break;

        //case MSG_KEYUP_OK:
        //case MSG_APP_OK:
        case MSG_CTL_OK:
            MMK_CloseWin(win_id);
            break;
        
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            s_cur_sim_sel = s_cur_dual_sys;
            SCI_MEMSET(&num_string, 0, sizeof (num_string));
            GUIEDIT_GetString(edit_name_ctrl_id,&num_string);
            if (num_string.wstr_len > 0)
            {
                if (MMIAPICOM_IsValidNumberStringEx(&num_string))
                {
                    if (vmn_cur_sim)
                    {
                        SetVoiceMailAddress(s_cur_sim_sel, MN_MULTI_SUBSCRIBER_ID_1, &num_string);
                    }
                    else
                    {
                        MMISET_SetVoiceMailNumberToNV(s_cur_sim_sel,num_string.wstr_ptr,num_string.wstr_len);
                        MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
                    }   
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
                }
                MMK_CloseWin(win_id);
            }
            else if(num_string.wstr_len == 0)
            {
                if (vmn_cur_sim)
                {
                    SetVoiceMailAddress(s_cur_sim_sel, MN_MULTI_SUBSCRIBER_ID_1, &num_string);
                }
                else
                {
                    SCI_TRACE_LOW("ljg*HandleSettingsEditVoiceMailNumberMsg: num_string.wstr_len == 0");    
                    voice_mail_null[0] = 'N';
                    MMI_STRNTOWSTR(vmtele_null,
                                   MMISMS_PBNUM_MAX_LEN,
                                   (uint8 *)voice_mail_null,
                                   MMISMS_PBNUM_MAX_LEN,
                                   1);    
                    MMISET_SetVoiceMailNumberToNV(s_cur_sim_sel,num_string.wstr_ptr,num_string.wstr_len);
                    SCI_TRACE_LOW("ljg*HandleSettingsEditVoiceMailNumberMsg: save vm number to nv, num_string.wstr_len=%d", num_string.wstr_len);	
                }   
                MMK_CloseWin(win_id);
            }
            else 
            {
                //MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
            }
            break;
        case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
            {
                MMI_STRING_T text_str = {0};
                GUIEDIT_GetString(edit_name_ctrl_id, &text_str);
                GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,2,TRUE);			
                break;
            }
//         case MSG_CTL_OK:
//             result = MMIPUB_HandleDialogInputFieldWinMsg(win_id, msg_id, param);
//             break;
        default:
            result = MMIPUB_HandleDialogInputFieldWinMsg(win_id, msg_id, param);
            break;
        }
        return result;
}
LOCAL MMI_RESULT_E  HandleSettingsEditVoiceMailNumberMsgCallIdle(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T edit_name_ctrl_id = MMIPUB_INPUTFIELD_CTRL_ID;
    uint8 vmtele_len = 0;	
    uint8 mail_len = 0;	
    wchar vmtele_str[32] = {0};
    MMI_STRING_T num_string = {0};
    MMI_STRING_T edit_str = {0};
    BOOLEAN vmn_cur_sim = FALSE;
    MN_DUAL_SYS_E s_cur_sim_sel = MN_DUAL_SYS_1;
    uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMI_STRING_T num_ptr = {0};
    uint8 voice_mail_null[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    wchar vmtele_null[32] = {0};
    wchar dot_str[] = {'.'};
    if(MN_DUAL_SYS_1 == s_cur_dual_sys){
        vmn_cur_sim = s_is_vmn_from_sim1;
    }else{
        vmn_cur_sim = s_is_vmn_from_sim2;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:    
    case MSG_FULL_PAINT:
        //GUIEDIT_ConfigImCustomKey(MMIPUB_INPUTFIELD_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        //GUIEDIT_SetImTag(MMIPUB_INPUTFIELD_CTRL_ID, GUIIM_TAG_DOT_NUM);		
        GUIEDIT_SetIm(MMIPUB_INPUTFIELD_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
        GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_NULL,LEFT_BUTTON,TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SELECT,MIDDLE_BUTTON,TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,RIGHT_BUTTON,TRUE);
        s_cur_sim_sel = s_cur_dual_sys;
        GUIEDIT_SetMaxLen(edit_name_ctrl_id, MMISET_MAX_VM_NUMBER_LEN);
        SCI_MEMSET(vmtele_str, 0x00, sizeof(vmtele_str));
        SCI_TRACE_LOW("ljg*HandleSettingsEditVoiceMailNumberMsgCallIdle:s_cur_dual_sys=%d,vmn_cur_sim=%d",s_cur_dual_sys,vmn_cur_sim);

        if (vmn_cur_sim)
        {
            if(MN_DUAL_SYS_1 == s_cur_dual_sys)
            {
                mail_len = MMIAPICOM_GenDispNumber( 
                    g_sim_vmnum_info.number_type, 
                    g_sim_vmnum_info.num_len, 
                    g_sim_vmnum_info.bcd_num, 
                    voice_mail,
                    MMISMS_PBNUM_MAX_LEN + 2);
            }
            else
            {
                mail_len = MMIAPICOM_GenDispNumber( 
                    g_sim2_vmnum_info.number_type, 
                    g_sim2_vmnum_info.num_len, 
                    g_sim2_vmnum_info.bcd_num, 
                    voice_mail,
                    MMISMS_PBNUM_MAX_LEN + 2);
            }
            SCI_TRACE_LOW("ljg*HandleSettingsEditVoiceMailNumberMsgCallIdle:mailbox_number=%s,mail_len=%d",voice_mail,mail_len);	
            mail_len = MIN(mail_len, MMISMS_PBNUM_MAX_LEN);
            MMI_STRNTOWSTR(vmtele_str,
                            MMISMS_PBNUM_MAX_LEN,
                            (uint8 *)voice_mail,
                            MMISMS_PBNUM_MAX_LEN,
                            mail_len);
            vmtele_len=mail_len;
        }
        SCI_MEMSET(&num_string, 0, sizeof (num_string));
        num_string.wstr_ptr = vmtele_str;
        num_string.wstr_len = vmtele_len;
        if(MMIAPICOM_IsValidNumberStringEx(&num_string)) //fix Bug 790810 
        {
            GUIEDIT_SetString(edit_name_ctrl_id,
                vmtele_str,
                vmtele_len
                );
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        break;
    case MSG_CTL_MIDSK:
        //case MSG_APP_OK:
    case MSG_APP_WEB:
        s_cur_sim_sel = s_cur_dual_sys;
        SCI_MEMSET(&num_string, 0, sizeof (num_string));
        GUIEDIT_GetString(edit_name_ctrl_id,&num_string);
        if (num_string.wstr_len > 0)
        {
            if (MMIAPICOM_IsValidNumberStringEx(&num_string))
            {
                if (vmn_cur_sim)
                {
                    SetVoiceMailAddress(s_cur_sim_sel, MN_MULTI_SUBSCRIBER_ID_1, &num_string);
                }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
            }
            MMK_CloseWin(win_id);
        }
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMIPUB_HandleDialogInputFieldWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

LOCAL MMI_RESULT_E HandleVoiceMailSIMSelectWinMsg(
														  MMI_WIN_ID_T	  win_id, 
														  MMI_MESSAGE_ID_E	 msg_id, 
														  DPARAM			 param
														  )

{
    MN_DUAL_SYS_E       dual_sys = MN_DUAL_SYS_MAX;
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T menu_ctrl_id = MMISET_CALL_VOICEMAIL_SIM_SELECT_CTRL_ID;
    uint8 node_id = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateSimSelectionMenu(win_id,menu_ctrl_id);
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        break;

    case MSG_FULL_PAINT:
        break;
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:    
    case MSG_APP_CALL4:     
    case MSG_APP_OK:
#if !defined(MMI_PDA_SUPPORT) || defined(MMI_CALLCONTROL_MINI_SUPPORT)
    case MSG_CTL_OK:
#endif
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        node_id = GUIMENU_GetCurNodeId(menu_ctrl_id);
        switch (node_id)
        {
        case CC_SIM_SEL_MENU_VODAFONE:
            s_cur_dual_sys =  0;
            GetVoiceMailAddress(s_cur_dual_sys, MN_MULTI_SUBSCRIBER_ID_1,  MN_VOICE_MAIL);
            break;
        case CC_SIM_SEL_MENU_CHINAUNICOM:
            s_cur_dual_sys =  1;
            GetVoiceMailAddress(s_cur_dual_sys, MN_MULTI_SUBSCRIBER_ID_1,  MN_VOICE_MAIL);
            break;

        default:
            break;
        }
		MMK_CloseWin( win_id );
        break;

	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}
typedef enum
{
    CP_SIM_SEL_MENU_NODE_ROOT,
    CP_SIM_SEL_MENU_SIM1,
    CP_SIM_SEL_MENU_SIM2,
    CP_SIM_SEL_MENU_ALWAYS_ASK
};

LOCAL void CreateSimSelectionMenu2(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{
    uint8 index = 0;
    MMI_STRING_T name_str = {0};
    MMI_STRING_T stb_str = {0};
    GUIMENU_DYNA_ITEM_T menu_ptr = {0};
    GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP);

    MMI_GetLabelTextByLang(STR_ILIST_SIM_ONE,&name_str);
    menu_ptr.item_text_ptr = &name_str;
    GUIMENU_InsertNode(index++,CP_SIM_SEL_MENU_SIM1,CP_SIM_SEL_MENU_NODE_ROOT,&menu_ptr,ctrl_id);

    MMI_GetLabelTextByLang(STR_ILIST_SIM_TWO,&name_str);
    menu_ptr.item_text_ptr = &name_str;
    GUIMENU_InsertNode(index++,CP_SIM_SEL_MENU_SIM2,CP_SIM_SEL_MENU_NODE_ROOT,&menu_ptr,ctrl_id);

//    MMI_GetLabelTextByLang(STR_LIST_SIM_MGR_ASK,&name_str);
//    menu_ptr.item_text_ptr = &name_str;
//    GUIMENU_InsertNode(index++,CP_SIM_SEL_MENU_ALWAYS_ASK,CP_SIM_SEL_MENU_NODE_ROOT,&menu_ptr,ctrl_id);
    GUIMENU_SetPopMenuCentered(ctrl_id,TRUE);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id,TXT_NULL,STXT_SELECT,TXT_COMMON_COMMON_BACK);

    GUIMENU_SetNodeGrayedByNodeId(!IsSimFdnFunctionAvailable(MN_DUAL_SYS_1),
            CP_SIM_SEL_MENU_SIM1, 0, ctrl_id);
    GUIMENU_SetNodeGrayedByNodeId(!IsSimFdnFunctionAvailable(MN_DUAL_SYS_2),
            CP_SIM_SEL_MENU_SIM2, 0, ctrl_id);
}

LOCAL MMI_RESULT_E HandleFixedDialingWindow(
														  MMI_WIN_ID_T	  win_id, 
														  MMI_MESSAGE_ID_E	 msg_id, 
														  DPARAM			 param
														  )

{
		MMI_RESULT_E				recode		=	MMI_RESULT_TRUE;
		MMI_CTRL_ID_T		 menu_ctrl_id = MMISET_FIXED_DIALING_MENU_CTRL_ID;
		MMI_MENU_GROUP_ID_T group_id = 0;
		MMI_MENU_ID_T item_id = 0;
	    uint16	 cur_item = 0;
        GUIMENU_DYNA_ITEM_T 	node_item = {0};
		MMI_STRING_T str_name;
		switch(msg_id)
		{
		case MSG_OPEN_WINDOW:
#ifdef STATUS_TEXT_SUPPORT
			GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
#endif
			CreateSimSelectionMenu2(win_id,menu_ctrl_id);
			MMK_SetAtvCtrl(win_id, menu_ctrl_id);
			break;
		case MSG_CTL_MIDSK:
			
			cur_item = GUIMENU_GetCurNodeId(menu_ctrl_id);
			  // cur_item = GUILIST_GetCurItemIndex(menu_ctrl_id);
			   switch (cur_item)
			{
			case 1:
				s_cur_dual_sys =  0;
				if(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(s_cur_dual_sys)){
#ifndef WIN32					
					if(MN_SIM_SERVICE_NOT_SUPPORT != MNSIM_GetSimEFService(s_cur_dual_sys,MN_SIM_FDN))
#endif
					MMK_CreateWin((uint32 *)MMISET_SET_FIXED_DIALING_SUB_WIN_TAB,(ADD_DATA)s_cur_dual_sys);
				}
				MMK_CloseWin(win_id);
				break;
			case 2:
				s_cur_dual_sys = 1;
				if(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(s_cur_dual_sys)){
#ifndef WIN32						
					if(MN_SIM_SERVICE_NOT_SUPPORT != MNSIM_GetSimEFService(s_cur_dual_sys,MN_SIM_FDN))
#endif
					MMK_CreateWin((uint32 *)MMISET_SET_FIXED_DIALING_SUB_WIN_TAB,(ADD_DATA)s_cur_dual_sys);
				}
				MMK_CloseWin(win_id);
				break;
			default:
				MMK_CloseWin(win_id);
				break;
			   }
			break;
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
			MMK_CloseWin(win_id);
			break;	
		default:
				recode = MMI_RESULT_FALSE;
				break;
		}
			return recode;

}



LOCAL MMI_RESULT_E HandleFixedDialingSubWindow(
														  MMI_WIN_ID_T	  win_id, 
														  MMI_MESSAGE_ID_E	 msg_id, 
														  DPARAM			 param
														  )

{
    
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    
    MMI_CTRL_ID_T               ctrl_id = MMISET_FIXED_DIALING_SUB_MENU_CTRL_ID;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16   cur_item = 0;
	uint16	current_index = 0;
	int i=0;
	MMI_STRING_T    title_str = {0};
    MMI_STRING_T    temp_str = {0};
	MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
	ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_NO_ERR;  
	PUBLIC MN_DUAL_SYS_E s_cur_dual_id = 0;
	BOOLEAN FdnIsSameSet = FALSE;
	//s_cur_dual_sys = param;
	s_cur_dual_id = (MN_DUAL_SYS_E)MMK_GetWinAddDataPtr(win_id);
	fdn_context_ptr = MMIAPIFDN_GetFDNContext(s_cur_dual_id);
    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:
		MMK_SetAtvCtrl(win_id,ctrl_id);
		GUILIST_SetMaxItem(ctrl_id,2, FALSE );
		GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

		item_t.item_style	  = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;

		item_t.item_data_ptr = &item_data;
		item_data.item_content[0].item_data_type	  = GUIITEM_DATA_TEXT_ID;			 
		item_data.item_content[0].item_data.text_id = STR_ILIST_FDN_EXT01;
		item_data.softkey_id[0] = COMMON_TXT_NULL;
		item_data.softkey_id[1] = STXT_SELECT;
		item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
		CTRLLIST_SetToggleState(ctrl_id,0,(fdn_context_ptr->is_fdn_enable)?TRUE:FALSE);
		GUILIST_AppendItem( ctrl_id, &item_t);  

		item_t.item_style	  = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
		item_t.item_data_ptr = &item_data;
		item_data.item_content[0].item_data_type	  = GUIITEM_DATA_TEXT_ID;			 
		item_data.item_content[0].item_data.text_id = STR_LIST_FDN_NUMBER_EXT01;
		GUILIST_AppendItem( ctrl_id, &item_t);

		GUILIST_SetSelectedItem(ctrl_id, current_index, TRUE);
		GUILIST_SetCurItemIndex(ctrl_id, current_index);
     	break;
	case MSG_FULL_PAINT:
		CTRLLIST_SetToggleState(ctrl_id,0,(fdn_context_ptr->is_fdn_enable)?TRUE:FALSE);
		break;
    case MSG_CTL_MIDSK:
        cur_item = GUILIST_GetCurItemIndex(ctrl_id);
        switch (cur_item)
	 {
	 case 0:
	 	 //MMK_CreateWin((uint32*)MMISET_SET_FIXED_DIALING_SWITCH_TAB,PNULL);
		 if(fdn_context_ptr->is_fdn_enable)
		 {
			  fdn_context_ptr->oper_type = MMIFDN_DISABLE_OPER;
			  if (!fdn_context_ptr->is_fdn_enable)
			  {
				  MMIPUB_OpenAlertWarningWin(TXT_FDN_DEACTIVATED);
			  }
			  else
			  {
				  MMIAPIPHONE_SetCurHandlingPinSimID(s_cur_dual_id);
				  MMIAPIPHONE_HandlePinAndPukFlow(
					  MNSIM_PIN_VERIFY,
					  MNSIM_PIN2,
					  (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
					  TRUE);
			  }
		 }
		 else
		 {
			   fdn_context_ptr->oper_type = MMIFDN_ENABLE_OPER;
			   if (fdn_context_ptr->is_fdn_enable)
			   {
				  //重复操作，直接调用成功窗口显示
				  MMIPUB_OpenAlertWarningWin(TXT_FDN_ACTIVATED);
			   }
			   else
			   {
				  MMIAPIPHONE_SetCurHandlingPinSimID(s_cur_dual_id);
				  MMIAPIPHONE_HandlePinAndPukFlow(
					  MNSIM_PIN_VERIFY,
					  MNSIM_PIN2,
					  (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
					  TRUE);
			   }				  
		  }
	     break;
	 case 1:
	 	#if defined(FDN_SUPPORT)
	 	 MMIPB_OpenFDNContactList(s_cur_dual_id);
		#endif
		break;
	 default:
            break;
        }
    break;
	case MSG_FDN_PIN2_SUCCESS:
	    switch (fdn_context_ptr->oper_type)
	    {
	    case MMIFDN_ENABLE_OPER:
	        //SCI_TRACE_LOW:"start PHONEBOOK_SetFDNService true"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5869_112_2_18_2_52_2_325,(uint8*)"");
	        err_code = PHONEBOOK_SetFDNServiceEx(s_cur_dual_id, TRUE, P_APP);//使能FDN
	        if(PHONEBOOK_IsFdnEnabled(s_cur_dual_id))
	            FdnIsSameSet = TRUE; 
	        break;
	        
	    case MMIFDN_DISABLE_OPER:
	        //SCI_TRACE_LOW:"start PHONEBOOK_SetFDNService false"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5874_112_2_18_2_52_2_326,(uint8*)"");
	        err_code = PHONEBOOK_SetFDNServiceEx(s_cur_dual_id, FALSE, P_APP);//关闭FDN
	        if(PHONEBOOK_IsFdnEnabled(s_cur_dual_id)== FALSE)
	            FdnIsSameSet = TRUE; 
	        break;
	        
	    default:
	        //SCI_PASSERT(FALSE, ("fdn_context_ptr->oper_type=%d", fdn_context_ptr->oper_type));
	        break;
	    }
	    
	    //SCI_TRACE_LOW:"HandleSetFDNStateWindow err_code=%d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5883_112_2_18_2_52_2_327,(uint8*)"d", err_code);
	    
	    if (ERR_MNDATAMAG_NO_ERR != err_code)//不存在
	    {
	        MMIPUB_OpenAlertWarningWin( TXT_FDN_NOT_EXIST);
	        MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
	    }
	    else if(FdnIsSameSet)
	    {
	        MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
	        MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
	    }
    break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

LOCAL MMI_RESULT_E HandleFixedDialingSwitchWindow(
														  MMI_WIN_ID_T	  win_id, 
														  MMI_MESSAGE_ID_E	 msg_id, 
														  DPARAM			 param
														  )

{
    
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    
    MMI_CTRL_ID_T               ctrl_id = MMISET_FIXED_DIALING_SWITCH_CTRL_ID;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16   cur_item = 0;
	uint16	current_index = 0;
	MMI_STRING_T    title_str = {0};
    MMI_STRING_T    temp_str = {0};
	MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
	ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_NO_ERR;  
	
	fdn_context_ptr = MMIAPIFDN_GetFDNContext(s_cur_dual_sys);
    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:
	 case MSG_FULL_PAINT:	 	
		MMK_SetAtvCtrl(win_id,ctrl_id);
		GUILIST_SetMaxItem(ctrl_id,2, FALSE );
		GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

		item_t.item_style	  = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;

		item_t.item_data_ptr = &item_data;
		item_data.item_content[0].item_data_type	  = GUIITEM_DATA_TEXT_ID;			 
		item_data.item_content[0].item_data.text_id = STR_POC_ILIST_ON;
		item_data.softkey_id[0] = COMMON_TXT_NULL;
		item_data.softkey_id[1] = STXT_SELECT;
		item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
		GUILIST_AppendItem( ctrl_id, &item_t);  

		item_t.item_style	  = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
		item_t.item_data_ptr = &item_data;
		item_data.item_content[0].item_data_type	  = GUIITEM_DATA_TEXT_ID;			 
		item_data.item_content[0].item_data.text_id = TXT_CLOSE;
		GUILIST_AppendItem( ctrl_id, &item_t);

		GUILIST_SetSelectedItem(ctrl_id, current_index, TRUE);
		GUILIST_SetCurItemIndex(ctrl_id, current_index);
     break;
    case MSG_CTL_MIDSK:
        cur_item = GUILIST_GetCurItemIndex(ctrl_id);
		MMI_GetLabelTextByLang(TXT_ENTER_PIN2, &temp_str);
		switch (cur_item)
	 {
	 case 0:
		 if(fdn_context_ptr->is_fdn_enable)
		 {
			  fdn_context_ptr->oper_type = MMIFDN_DISABLE_OPER;
			  if (!fdn_context_ptr->is_fdn_enable)
			  {
				  MMIPUB_OpenAlertWarningWin(TXT_FDN_DEACTIVATED);
			  }
			  else
			  {
				  MMIAPIPHONE_SetCurHandlingPinSimID(s_cur_dual_sys);
				  MMIAPIPHONE_HandlePinAndPukFlow(
					  MNSIM_PIN_VERIFY,
					  MNSIM_PIN2,
					  (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
					  TRUE);
			  }
		 }
		 else
		 {
			   fdn_context_ptr->oper_type = MMIFDN_ENABLE_OPER;
			   if (fdn_context_ptr->is_fdn_enable)
			   {
				  //重复操作，直接调用成功窗口显示
				  MMIPUB_OpenAlertWarningWin(TXT_FDN_ACTIVATED);
			   }
			   else
			   {
				  MMIAPIPHONE_SetCurHandlingPinSimID(s_cur_dual_sys);
				  MMIAPIPHONE_HandlePinAndPukFlow(
					  MNSIM_PIN_VERIFY,
					  MNSIM_PIN2,
					  (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
					  TRUE);
			   }				  
		  }

		 break;
	 case 1:
		 if(fdn_context_ptr->is_fdn_enable)
		 {
			  fdn_context_ptr->oper_type = MMIFDN_DISABLE_OPER;
			  if (!fdn_context_ptr->is_fdn_enable)
			  {
				  MMIPUB_OpenAlertWarningWin(TXT_FDN_DEACTIVATED);
			  }
			  else
			  {
				  MMIAPIPHONE_SetCurHandlingPinSimID(s_cur_dual_sys);
				  MMIAPIPHONE_HandlePinAndPukFlow(
					  MNSIM_PIN_VERIFY,
					  MNSIM_PIN2,
					  (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
					  TRUE);
			  }
		 }
		 else
		 {
			   fdn_context_ptr->oper_type = MMIFDN_ENABLE_OPER;
			   if (fdn_context_ptr->is_fdn_enable)
			   {
				  //重复操作，直接调用成功窗口显示
				  MMIPUB_OpenAlertWarningWin(TXT_FDN_ACTIVATED);
			   }
			   else
			   {
				  MMIAPIPHONE_SetCurHandlingPinSimID(s_cur_dual_sys);
				  MMIAPIPHONE_HandlePinAndPukFlow(
					  MNSIM_PIN_VERIFY,
					  MNSIM_PIN2,
					  (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
					  TRUE);
			   }				  
		  }

	 	break;
	 default:
            break;
        }
    break;
    case MSG_FDN_PIN2_SUCCESS:
    switch (fdn_context_ptr->oper_type)
    {
    case MMIFDN_ENABLE_OPER:
        //SCI_TRACE_LOW:"start PHONEBOOK_SetFDNService true"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5869_112_2_18_2_52_2_325,(uint8*)"");
        err_code = PHONEBOOK_SetFDNServiceEx(s_cur_dual_sys, TRUE, P_APP);//使能FDN
        break;
        
    case MMIFDN_DISABLE_OPER:
        //SCI_TRACE_LOW:"start PHONEBOOK_SetFDNService false"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5874_112_2_18_2_52_2_326,(uint8*)"");
        err_code = PHONEBOOK_SetFDNServiceEx(s_cur_dual_sys, FALSE, P_APP);//关闭FDN
        break;
        
    default:
        //SCI_PASSERT(FALSE, ("fdn_context_ptr->oper_type=%d", fdn_context_ptr->oper_type));
        break;
    }
    
    //SCI_TRACE_LOW:"HandleSetFDNStateWindow err_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5883_112_2_18_2_52_2_327,(uint8*)"d", err_code);
    
    if (ERR_MNDATAMAG_NO_ERR != err_code)//不存在
    {
        MMIPUB_OpenAlertWarningWin( TXT_FDN_NOT_EXIST);
        MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
    }
    break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}	

typedef enum
{
    SET_MENU_CALLWAITING_SIM_ROOT,
    SET_MENU_CALLWAITING_SIM_ITEM1,
	SET_MENU_CALLWAITING_SIM_ITEM2,
	SET_MENU_CALLDIVERTS_FROM_SIM1,
	SET_MENU_CALLDIVERTS_FROM_SIM2,
	SET_MENU_CALLDIVERTS_FROM_BOTH,
};


	
LOCAL void CreatePopSelectionMenu(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id){
    uint8 index = 0;
    MMI_TEXT_ID_T label = TXT_NULL;
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    uint8 i=0;
    MMI_STRING_T    temp_str = {0};

    GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP);
    if(MMISET_CALL_WAITING_MENU_CTRL_ID == ctrl_id){ 
        MMI_GetLabelTextByLang(STR_ILIST_SIM_ONE, &temp_str);
        node_item.item_text_ptr = &temp_str;
        GUIMENU_InsertNode(index,SET_MENU_CALLWAITING_SIM_ITEM1,SET_MENU_CALLWAITING_SIM_ROOT,&node_item,ctrl_id);
        if(!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1)){
            CTRLMENU_SetNodeGrayedByNodeId(TRUE,SET_MENU_CALLWAITING_SIM_ITEM1,SET_MENU_CALLWAITING_SIM_ROOT,ctrl_id);
        }

        index++;
        i=1;
        MMI_GetLabelTextByLang(STR_ILIST_SIM_TWO, &temp_str);
        node_item.item_text_ptr = &temp_str;
        GUIMENU_InsertNode(index,SET_MENU_CALLWAITING_SIM_ITEM2,SET_MENU_CALLWAITING_SIM_ROOT,&node_item,ctrl_id);
        if(!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_2)){
            CTRLMENU_SetNodeGrayedByNodeId(TRUE,SET_MENU_CALLWAITING_SIM_ITEM2,SET_MENU_CALLWAITING_SIM_ROOT,ctrl_id);
        }

    }else{
        MMI_GetLabelTextByLang(STR_LIST_FROM_SIM1_EXT01, &temp_str);
        node_item.item_text_ptr = &temp_str;
        GUIMENU_InsertNode(index,SET_MENU_CALLDIVERTS_FROM_SIM1,SET_MENU_CALLWAITING_SIM_ROOT,&node_item,ctrl_id);
        if(!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1)){
            CTRLMENU_SetNodeGrayedByNodeId(TRUE,SET_MENU_CALLDIVERTS_FROM_SIM1,SET_MENU_CALLWAITING_SIM_ROOT,ctrl_id);
        }

        index++;
        i=1;
        MMI_GetLabelTextByLang(STR_LIST_FROM_SIM2_EXT01, &temp_str);
        node_item.item_text_ptr = &temp_str;
        GUIMENU_InsertNode(index,SET_MENU_CALLDIVERTS_FROM_SIM2,SET_MENU_CALLWAITING_SIM_ROOT,&node_item,ctrl_id);
        if(!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_2)){
            CTRLMENU_SetNodeGrayedByNodeId(TRUE,SET_MENU_CALLDIVERTS_FROM_SIM2,SET_MENU_CALLWAITING_SIM_ROOT,ctrl_id);
        }

        index++;
        i=2;
        MMI_GetLabelTextByLang(STR_LIST_BOTH_SIM_EXT01, &temp_str);
        node_item.item_text_ptr = &temp_str;
        GUIMENU_InsertNode(index,SET_MENU_CALLDIVERTS_FROM_BOTH,SET_MENU_CALLWAITING_SIM_ROOT,&node_item,ctrl_id);
        if((!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))||(!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_2))){
            CTRLMENU_SetNodeGrayedByNodeId(TRUE,SET_MENU_CALLDIVERTS_FROM_BOTH,SET_MENU_CALLWAITING_SIM_ROOT,ctrl_id);
        }
    }
    GUIMENU_SetPopMenuCentered(ctrl_id,TRUE);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id,TXT_NULL,STXT_SELECT,TXT_COMMON_COMMON_BACK);
}



LOCAL MMI_RESULT_E HandleCallWaitingWindow(
														  MMI_WIN_ID_T	  win_id, 
														  MMI_MESSAGE_ID_E	 msg_id, 
														  DPARAM			 param
														  )

{
		MMI_RESULT_E				recode		=	MMI_RESULT_TRUE;
		MMI_CTRL_ID_T		 menu_ctrl_id = MMISET_CALL_WAITING_MENU_CTRL_ID;
		MMI_MENU_GROUP_ID_T group_id = 0;	
		MMI_MENU_ID_T item_id = 0;
	    uint16	 cur_item = 0;
		
		switch(msg_id)
		{
		case MSG_OPEN_WINDOW:
#ifdef STATUS_TEXT_SUPPORT
			GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
#endif
			CreatePopSelectionMenu(win_id,menu_ctrl_id);
			//GUIMENU_GetId(MMISET_CALL_WAITING_MENU_CTRL_ID,&group_id,&item_id);
			//GUILIST_SetMaxItem(menu_ctrl_id, MMISET_CALLSET_ITEM_NUM_MAX, FALSE);
			//MMIAPISET_AppendListItemByTextId(STR_ILIST_SIM_ONE, TXT_NULL, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_MS);
			//MMIAPISET_AppendListItemByTextId(STR_ILIST_SIM_TWO, TXT_NULL, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_MS);
			MMK_SetAtvCtrl(win_id, menu_ctrl_id);
			break;
		case MSG_CTL_MIDSK:
	//	case MSG_CTL_OK:
	//	case MSG_APP_OK:
	//	case MSG_APP_WEB: 
			   cur_item = GUIMENU_GetCurNodeId(menu_ctrl_id);
			   switch (cur_item)
			{
			case SET_MENU_CALLWAITING_SIM_ITEM1:
				if(SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))break;				
				s_cur_dual_sys =  0;
				MMK_CreateWin((uint32 *)MMISET_SELECT_POP_WAITING_WIN_TAB,(ADD_DATA)cur_item);
				break;
			case SET_MENU_CALLWAITING_SIM_ITEM2:
				if(SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2))break;
				s_cur_dual_sys = 1;
				MMK_CreateWin((uint32 *)MMISET_SELECT_POP_WAITING_WIN_TAB,(ADD_DATA)cur_item);
				break;
			default:
				   break;
			   }
			MMK_CloseWin( win_id );	   
			break;
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
			MMK_CloseWin(win_id);
			break;	
		default:
				recode = MMI_RESULT_FALSE;
				break;
		}
			return recode;

}


LOCAL MMI_RESULT_E HandleCallDivertSimCardWin(
                                                          MMI_WIN_ID_T    win_id, 
                                                          MMI_MESSAGE_ID_E   msg_id, 
                                                          DPARAM             param
                                                          )

{
		MMI_RESULT_E				recode		=	MMI_RESULT_TRUE;
		MMI_CTRL_ID_T		 menu_ctrl_id = MMISET_CALL_DIVERT_SIMCARD_CTRL_ID;
		MMI_WIN_ID_T        inputdialog_win_id = MMISET_SET_BAR_PASSWORD_WIN_ID;
		MMI_MENU_GROUP_ID_T group_id = 0;
		MMI_MENU_ID_T item_id = 0;
		uint16	 cur_item = 0;
		MMI_STRING_T    temp_str = {0};
		MMI_MENU_ID_T               menu_id     =   0;
		
		switch(msg_id)
		{
		case MSG_OPEN_WINDOW:
/*
			GUIMENU_GetId(MMISET_CALL_DIVERT_SIMCARD_CTRL_ID,&group_id,&item_id);
			GUILIST_SetMaxItem(menu_ctrl_id, MMISET_CALLSET_ITEM_NUM_MAX, FALSE);
			MMIAPISET_AppendListItemByTextId(STR_LIST_FROM_SIM1_EXT01, TXT_NULL, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_MS);
			MMIAPISET_AppendListItemByTextId(STR_LIST_FROM_SIM2_EXT01, TXT_NULL, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_MS);
			MMIAPISET_AppendListItemByTextId(STR_LIST_BOTH_SIM_EXT01, TXT_NULL, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_MS);
*/
#ifdef STATUS_TEXT_SUPPORT
			GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
#endif
			CreatePopSelectionMenu(win_id, menu_ctrl_id);
			MMK_SetAtvCtrl(win_id, menu_ctrl_id);
			break;
#ifdef TOUCH_PANEL_SUPPORT 
        case MSG_CTL_PENOK:
#endif 	
		case MSG_CTL_MIDSK:
      //  case MSG_CTL_OK:
      //  case MSG_APP_OK:
      //  case MSG_APP_WEB:
			cur_item = GUIMENU_GetCurNodeId(menu_ctrl_id);
			switch (cur_item)
			{
				case SET_MENU_CALLDIVERTS_FROM_SIM1:
					MMIAPISET_SetForwardSimCards(0);
					break;
				case SET_MENU_CALLDIVERTS_FROM_SIM2:
					MMIAPISET_SetForwardSimCards(1);
					break;
				case SET_MENU_CALLDIVERTS_FROM_BOTH:
					MMIAPISET_SetForwardSimCards(2);

					break;
				default:
					MMIAPISET_SetForwardSimCards(0);
					break;
			}
			MMI_GetLabelTextByLang(STR_INPUT_PHONE_NUM_EXT01, &temp_str);
			MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str, PNULL, PNULL, PNULL, HandleDivertNumberWindow);
			MMK_CloseWin(win_id);
			break;
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
			MMK_CloseWin(win_id);
			break;	
		default:
				recode = MMI_RESULT_FALSE;
				break;
		}
			return recode;

}


/*****************************************************************************/
//  Description : handle call divert window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallDivertWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E                recode      =  MMI_RESULT_TRUE;
#if defined MMI_GUI_STYLE_TYPICAL
    MMI_MENU_GROUP_ID_T         group_id    =  0;
#endif
    MMI_MENU_ID_T               menu_id     =  0;
    MMI_CTRL_ID_T               ctrl_id     =  MMISET_CALL_DIVERT_MENU_CTRL_ID;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
#if !defined MMI_GUI_STYLE_TYPICAL
            InitCallDivertList();
#endif
            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIWIN_SetSoftkeyTextId(win_id, STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
#if defined MMI_GUI_STYLE_TYPICAL           
#ifdef MMI_CSP_SUPPORT
            if (s_cur_dual_sys < MMI_DUAL_SYS_MAX)
            {
                MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
                BOOLEAN is_cfu_visible = TRUE;
                BOOLEAN is_cfc_visible = TRUE; //All conditional call forwarding
                BOOLEAN is_cfb_visible = TRUE;
                BOOLEAN is_cfnry_visible = TRUE;
                BOOLEAN is_cfnrc_visible = TRUE;
                BOOLEAN is_cancel_all_visible = TRUE;
                uint32 group = MENU_SET_CALL_DIVERTS_OPT;
                
                if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFU, CSP_MENU_OPER_NULL))
                {
                    is_cfu_visible = TRUE;
                }
                else
                {
                    is_cfu_visible = FALSE;
                }

                if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFB, CSP_MENU_OPER_NULL))
                {
                    is_cfb_visible = TRUE;
                }
                else
                {
                    is_cfb_visible = FALSE;
                }

                if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRY, CSP_MENU_OPER_NULL))
                {
                    is_cfnry_visible = TRUE;
                }
                else
                {
                    is_cfnry_visible = FALSE;
                }

                if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRC, CSP_MENU_OPER_NULL))
                {
                    is_cfnrc_visible = TRUE;
                }
                else
                {
                    is_cfnrc_visible = FALSE;
                }
                
                if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CF, CSP_MENU_OPER_CANCEL_ALL))
                {
                    is_cancel_all_visible = TRUE;
                }
                else
                {
                    is_cancel_all_visible = FALSE;
                }
                
                //??????,??????,???????????,??????????
                if (MMISET_IsAlsValid(dual_sys) && is_cfb_visible && is_cfnry_visible && is_cfnrc_visible)     
                {
                    is_cfc_visible = TRUE;
                    is_cfb_visible = FALSE;
                    is_cfnry_visible = FALSE;
                    is_cfnrc_visible = FALSE;
                }                
                else
                {
                    is_cfc_visible = FALSE;
                }
                
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_ALL_CALLS,  is_cfu_visible); 
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_ALL_CONDITIONAL,  is_cfc_visible);                    
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_WHEN_BUSY,  is_cfb_visible);                    
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_NO_ANSWER,  is_cfnry_visible);                    
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_NOT_REACHABLE,  is_cfnrc_visible);                                    
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_CANCEL_ALL,  is_cancel_all_visible);
                
            }
#endif
#endif
            break;
            
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
#if !defined MMI_GUI_STYLE_TYPICAL
            GUILIST_GetItemData(ctrl_id, GUILIST_GetCurItemIndex(ctrl_id), &menu_id);
#else
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
#endif
            if (menu_id == ID_SET_DIVERTS_CANCEL_ALL)
            {
                MMISET_CloseAllCallForwarding(s_cur_dual_sys);
            }
            else
            {
                CreateCallDivertSelectPopWindow(ctrl_id);
            }
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
            
    }
    return recode;
}

LOCAL SET_ID_MENU_E GUILIST_GetCurItemIndexMenuId(uint16	index){
	SET_ID_MENU_E recode =  ID_SET_BAR_ALL_MO_CALLS;

	switch(index){
		case 0:
			recode = ID_SET_BAR_ALL_MO_CALLS;
			break;	
		case 1:
			recode = ID_SET_BAR_ALL_MT_CALLS;
			break;
		case 2:
			recode = ID_SET_BAR_MT_CALLS_ROAM;
			break;
		case 3:
			recode = ID_SET_BAR_MO_CALLS_INTL;
			break;
		case 4:
			recode = ID_SET_BAR_MO_CALLS_INTL_ROAM;
			break;
		case 5:
			recode = ID_SET_BAR_CANCEL_ALL;
			break;
		case 6:
			recode = ID_SET_BAR_CHANGE_PW;
			break;
		default:
			recode = ID_SET_BAR_ALL_MO_CALLS;
			break;

	}
	return recode;
}


/*****************************************************************************/
//  Description : handle call bar window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallBarWindow(
                                          MMI_WIN_ID_T    win_id, 
                                          MMI_MESSAGE_ID_E   msg_id, 
                                          DPARAM             param
                                          )
{
    MMI_RESULT_E          recode = MMI_RESULT_TRUE;
#if defined MMI_GUI_STYLE_TYPICAL
    MMI_MENU_GROUP_ID_T group_id = 0;
#endif
    MMI_MENU_ID_T        menu_id = 0;
    MMI_CTRL_ID_T        ctrl_id = MMISET_CALL_BAR_MENU_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#if !defined MMI_GUI_STYLE_TYPICAL
        InitCallBarList();
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id, STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
#if defined MMI_GUI_STYLE_TYPICAL
#ifdef MMI_CSP_SUPPORT
        if (s_cur_dual_sys < MMI_DUAL_SYS_MAX)
        {
            MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
            BOOLEAN is_visible = TRUE;
            uint32 group = MENU_SET_CALL_BAR_OPT;
            
            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_BAOC, CSP_MENU_OPER_NULL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_ALL_MO_CALLS, is_visible);
            
            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_BAIC, CSP_MENU_OPER_NULL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_ALL_MT_CALLS, is_visible);
            
            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_BICROAM, CSP_MENU_OPER_NULL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_MT_CALLS_ROAM, is_visible);
            
            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_BOIC, CSP_MENU_OPER_NULL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_MO_CALLS_INTL, is_visible);
            
            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_BOIC_EXHC, CSP_MENU_OPER_NULL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_MO_CALLS_INTL_ROAM, is_visible);
            
            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CB, CSP_MENU_OPER_CANCEL_ALL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_CANCEL_ALL, is_visible);
            
            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CB, CSP_MENU_OPER_MODIFY_PASSWORD))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_CHANGE_PW, is_visible);
            
        }
#endif  
#endif
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#if !defined MMI_GUI_STYLE_TYPICAL
        GUILIST_GetItemData(ctrl_id, GUILIST_GetCurItemIndex(ctrl_id), &menu_id);
#else
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
#endif
        if (menu_id == ID_SET_BAR_CHANGE_PW)
        {
            if (MMIAPIPHONE_GetSimAvailableNum(PNULL, 0) > 0)
            {
                s_call_bar_operation = MMISET_CALL_BAR_MODIFY_PWD;
                MMK_CreatePubEditWin((uint32*)MMISET_SET_BAR_PASSWORD_WIN_TAB, (ADD_DATA)s_cur_dual_sys);
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
        }
        else if (menu_id == ID_SET_BAR_CANCEL_ALL)
        {
            
            if (MMIAPIPHONE_GetSimAvailableNum(PNULL, 0) > 0)
            {
                s_call_bar_operation = MMISET_CALL_BAR_CANCEL_ALL;
                MMK_CreatePubEditWin((uint32*)MMISET_SET_BAR_PASSWORD_WIN_TAB, (ADD_DATA)s_cur_dual_sys);
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
        }
        else
        {
            CreateCallBarSelectPopWindow(ctrl_id);
            
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
        
    }
    return recode;
}

/*****************************************************************************/
//  Description : set call divert/bar/wating/hide id ctrl id
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetDivertBarWatingHideidCtrlId(MMI_CTRL_ID_T ctri_id)
{
    //SCI_TRACE_LOW:"MMISET_SetDivertBarWatingHideidCtrlId ctrl_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_3766_112_2_18_2_50_23_68,(uint8*)"d", ctri_id);
    
    s_set_divert_bar_wating_hideid_ctrl_id = ctri_id;
}

/*****************************************************************************/
//  Description : Get call divert/bar/wating/hide id ctrl id
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMISET_GetDivertBarWatingHideidCtrlId(void)
{
    //SCI_TRACE_LOW:"MMISET_GetDivertBarWatingHideidCtrlId ctrl_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_3777_112_2_18_2_50_23_69,(uint8*)"d", s_set_divert_bar_wating_hideid_ctrl_id);
    
    return s_set_divert_bar_wating_hideid_ctrl_id;
}

/*****************************************************************************/
//  Description : handle call bar pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectPopBarWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    
    MMI_RESULT_E         recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T        menu_ctrl_id = MMISET_POPMENU_CALLSETTING_BAR_ID;
	uint16 sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(PNULL,0);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		GUILIST_SetMaxItem(menu_ctrl_id, MMISET_CALLSET_ITEM_NUM_MAX, FALSE);
        {
            BOOLEAN is_open_visible  = TRUE;
            BOOLEAN is_close_visible = TRUE;
            BOOLEAN is_query_visible = TRUE;
#ifdef MMI_CSP_SUPPORT            
            MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_MENU_ID_T item_id = 0;
            MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
            CSP_MENU_SS_TYPE_E ss_type = CSP_MENU_NULL;
            
            GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID, &group_id, &item_id);
            switch (item_id)
            {
            case ID_SET_DIVERTS_ALL_CALLS:
                ss_type = CSP_MENU_CFU;
                is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                break;
                
            case ID_SET_DIVERTS_ALL_CONDITIONAL:
                ss_type = CSP_MENU_CFB;
                is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                break;                    
                
            case ID_SET_DIVERTS_WHEN_BUSY:
                ss_type = CSP_MENU_CFB;
                is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                break;
                
            case ID_SET_DIVERTS_NO_ANSWER:
                ss_type = CSP_MENU_CFNRY;
                is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                break;
                
            case ID_SET_DIVERTS_NOT_REACHABLE:
                ss_type = CSP_MENU_CFNRC;
                is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                break;
                
            default:
                //do nothing
                break;
                
            }
            
#endif //MMI_CSP_SUPPORT   
            if (is_open_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_OPEN, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
            if (is_close_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_CLOSE, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
            if (is_query_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_SET_QUERY_STATUS, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
        }
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
	    break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16  current_index = 0;
            uint32  menu_id = ID_SET_CALL_BAR_OPEN;
            
            current_index = GUILIST_GetCurItemIndex(menu_ctrl_id);
            menu_id += current_index;
            
            if (sim_num > 0)
            {
                switch (menu_id)
                {               
                case ID_SET_CALL_BAR_OPEN:
                    s_call_bar_operation = MMISET_CALL_BAR_OPEN;
                    MMK_CreatePubEditWin((uint32*)MMISET_SET_BAR_PASSWORD_WIN_TAB, (ADD_DATA)s_cur_dual_sys);
                    MMK_CloseWin(win_id);
                    break;
                    
                case ID_SET_CALL_BAR_CLOSE:
                    s_call_bar_operation = MMISET_CALL_BAR_CLOSE;
                    MMK_CreatePubEditWin((uint32*)MMISET_SET_BAR_PASSWORD_WIN_TAB, (ADD_DATA)s_cur_dual_sys);
                    MMK_CloseWin(win_id);
                    break;
                    
                case ID_SET_CALL_BAR_STATUS:
                    MMISET_InterrogateStatus(s_cur_dual_sys);//for cr226576 maryxiao
                    MMK_CloseWin(win_id);
                    break;
                    
                default:
                    break;
                }
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
                MMK_CloseWin(win_id);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : init divert waiting barring call control
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/

LOCAL void InitCallDivertControl(void)
{
    MMI_STRING_T label_name  = {0};
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERTS_ALL_CALLS, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_UNCONDITION_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERTS_WHEN_BUSY, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_BUSY_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERTS_NO_ANSWER, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_NOANSWER_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERTS_NOT_REACHABLE, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_UNRECHED_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERTS_CANCEL_ALL, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_CANCELALL_LABEL_CTRL_ID, &label_name, FALSE);
}

#if 0
/*****************************************************************************/
//  Description : InitCallWaitingControl
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void InitCallWaitingControl(void)
{
    
    MMI_STRING_T label_name  = {0};
    
    MMI_GetLabelTextByLang(TXT_OPEN, &label_name);
    GUILABEL_SetText(MMISET_WAITING_ACTIVE_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
    GUILABEL_SetText(MMISET_WAITING_QUERY_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_QUERY_STATUS, &label_name);
    GUILABEL_SetText(MMISET_WAITING_QUERY_LABEL_CTRL_ID, &label_name, FALSE);
}

/*****************************************************************************/
//  Description : InitCallBarControl
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void InitCallBarControl(void)
{
    MMI_STRING_T label_name  = {0};
    
    MMI_GetLabelTextByLang(TXT_BAOC, &label_name);
    GUILABEL_SetText(MMISET_BAR_ALLOUTGOING_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_BAIC, &label_name);
    GUILABEL_SetText(MMISET_BAR_ALLINCOMING_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_BICROAM, &label_name);
    GUILABEL_SetText(MMISET_BAR_ROAMINGINCOMING_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_BOIC, &label_name);
    GUILABEL_SetText(MMISET_BAR_INTERNATIONALCALLING_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_BAR_MO_CALLS_INTL_ROAM, &label_name);
    GUILABEL_SetText(MMISET_BAR_INTLROAMING_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_BAR_CANCEL_ALL, &label_name);
    GUILABEL_SetText(MMISET_BAR_CANCELALL_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_BAR_CHANGE_PW, &label_name);
    GUILABEL_SetText(MMISET_BAR_CHANGEPASSWORD_LABEL_CTRL_ID, &label_name, FALSE);
}
#endif

/*****************************************************************************/
//  Description : open call divert pop window
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void OpenSetCallDivertWin(MN_DUAL_SYS_E    dual_sys)
{
#ifdef  MMI_MULTI_SIM_SYS_DUAL          /* lint_lai */   
        MMIPB_CONTACT_T     local_num                           =   {0};
        uint8                       sim_num[MMIPB_MAX_STRING_LEN + 2]       =   {0};
        wchar                       sim_num_wstr[MMIPB_MAX_STRING_LEN + 2]  =   {0};
        uint8                       num_len                             =   {0};
        BOOLEAN                     is_pb_msisdn                        =   FALSE;           
        MN_DUAL_SYS_E               another_sys                         =   MN_DUAL_SYS_MAX;
        
        another_sys = (dual_sys == MN_DUAL_SYS_1)? MN_DUAL_SYS_2: MN_DUAL_SYS_1;
        
        is_pb_msisdn = MMIAPIPB_ReadMsisdnContact(another_sys, &local_num);
#endif             
                //record dual_sys type

        MMK_CreatePubEditWin((uint32*)MMISET_SET_DIVERT_NUMBER_WIN_TAB,(ADD_DATA)dual_sys);
                
#ifdef  MMI_MULTI_SIM_SYS_DUAL  
        if (is_pb_msisdn && s_is_wizard_cfnrc)
        {
            num_len = MMIAPIPB_BCDNumberToString(&(local_num.number[0]), (uint8)MMIPB_MAX_STRING_LEN+2, sim_num);
//                     num_len = MMIAPICOM_GenDispNumber(
//                         MMIPB_GetNumberTypeFromUint8(local_num.number_t[0].npi_ton), 
//                         local_num.number_t[0].number_len, 
//                         local_num.number_t[0].number,
//                         sim_num,
//                         MMIPB_MAX_STRING_LEN + 2
//                         );

            num_len = MIN(num_len, MMISET_VOCIE_MAX_NUM);                    
            MMI_STRNTOWSTR( sim_num_wstr, MMISET_VOCIE_MAX_NUM, sim_num, num_len, num_len );
            
            GUIEDIT_SetString(
                MMISET_SET_DIVERT_NUMBER_CTRL_ID,
                sim_num_wstr,
                num_len);
        }
#endif                
}


/*****************************************************************************/
//  Description : handle call divert pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectPopDivertWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T        menu_ctrl_id = MMISET_POPMENU_CALLSETTING_DIVERT_ID;
    uint16 sim_num = 0;
    //   MMI_CTRL_ID_T ctrl_id = (MMI_CTRL_ID_T) (uint32)MMK_GetWinAddDataPtr(win_id);
    sim_num = MMIAPIPHONE_GetSimAvailableNum(PNULL,0);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(menu_ctrl_id, MMISET_CALLSET_ITEM_NUM_MAX, FALSE);
        {
            BOOLEAN is_open_visible = TRUE;
            BOOLEAN is_close_visible = TRUE;
            BOOLEAN is_query_visible = TRUE;
#ifdef MMI_CSP_SUPPORT            
            MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_MENU_ID_T item_id = 0;
            MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
            CSP_MENU_SS_TYPE_E ss_type = CSP_MENU_NULL;
            GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID,&group_id,&item_id);
            switch(item_id)
            {
                case ID_SET_DIVERTS_ALL_CALLS:
                    ss_type = CSP_MENU_CFU;
                    is_open_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;

                case ID_SET_DIVERTS_ALL_CONDITIONAL:
                    ss_type = CSP_MENU_CFB;
                    is_open_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;                    
                    
                case ID_SET_DIVERTS_WHEN_BUSY:
                    ss_type = CSP_MENU_CFB;
                    is_open_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;
                    
                case ID_SET_DIVERTS_NO_ANSWER:
                    ss_type = CSP_MENU_CFNRY;
                    is_open_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;
                    
                case ID_SET_DIVERTS_NOT_REACHABLE:
                    ss_type = CSP_MENU_CFNRC;
                    is_open_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;

                default:
                    //do nothing
                    break;
                    
            }

#endif //MMI_CSP_SUPPORT   
            if(is_open_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_OPEN, STXT_SOFTKEY_OK_MK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
            if(is_close_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_CLOSE, STXT_SOFTKEY_OK_MK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
            if(is_query_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_SET_QUERY_STATUS, STXT_SOFTKEY_OK_MK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
        }        
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16  current_index = 0;
            uint32  menu_id  = ID_SET_CALL_DIVERT_OPEN;

            current_index = GUILIST_GetCurItemIndex(menu_ctrl_id);
            menu_id += current_index;

            if(sim_num>0)
            {
                switch (menu_id)
                {
                case ID_SET_CALL_DIVERT_OPEN:    
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT 
                    MMK_CreateWin((uint32*)MMISET_SET_OPEN_DIVERT_WIN_TAB, PNULL);        
#else   
                    MMIAPISET_SetSysForCf(s_cur_dual_sys);/*lint !e64*/
                    OpenSetCallDivertWin(s_cur_dual_sys);
#endif
                    MMK_CloseWin(win_id);
                    break;                      
                case ID_SET_CALL_DIVERT_CLOSE:
                    MMISET_CloseCallForwarding(s_cur_dual_sys);/*lint !e64*/
                    MMK_CloseWin(win_id);
                    break;
                    
                case ID_SET_CALL_DIVERT_STATUS:
                    MMISET_InterrogateStatus(s_cur_dual_sys);//for cr226576
                    MMK_CloseWin(win_id);
                    break;
                    
                default:
                    break;
                }
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
                MMK_CloseWin(win_id);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
/*****************************************************************************/
//  Description : handle call waiting pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectPopWaitingWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 )
{
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T        menu_ctrl_id = MMISET_POPMENU_CALLSETTING_WAITING_ID;
    uint16 sim_num = 0;
    sim_num = MMIAPIPHONE_GetSimAvailableNum(PNULL,0);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(menu_ctrl_id, MMISET_CALLSET_ITEM_NUM_MAX, FALSE);
#if defined (MAINLCD_SIZE_128X160)
	MMIAPISET_AppendListItemByTextId(TXT_OPEN, TXT_COMMON_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
        MMIAPISET_AppendListItemByTextId(STXT_CANCEL, TXT_COMMON_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
        MMIAPISET_AppendListItemByTextId(TXT_SET_QUERY_STATUS, TXT_COMMON_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);

#else
	MMIAPISET_AppendListItemByTextId(TXT_OPEN, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
        MMIAPISET_AppendListItemByTextId(STXT_CANCEL, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
        MMIAPISET_AppendListItemByTextId(TXT_SET_QUERY_STATUS, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
#endif
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16  current_index = 0;
            uint32  menu_id  = ID_SET_CALL_WAITING_OPEN;

            current_index = GUILIST_GetCurItemIndex(menu_ctrl_id);
            menu_id += current_index;

            if(sim_num > 0)
            {
                switch (menu_id)
                { 
                case ID_SET_CALL_WAITING_OPEN:
#ifdef GCF_TA_SUPPORT
                MMISET_SetCallWaitStatus(s_cur_dual_sys,TRUE);
                if(MMI_GMMREG_RAT_GPRS == MMIAPIPHONE_GetTDOrGsm(s_cur_dual_sys))
                {
                    MMISET_DisplayBannerInfo(win_id,STR_CALL_WAITING_ACTIVATED_EXT01);	   
                }
                else		
#endif
                {
                    MMISET_StartCallWaitOrBar(s_cur_dual_sys);/*lint !e64*/
                    MMK_CloseWin(win_id);
                }
                break;
                    
                case ID_SET_CALL_WAITING_CLOSE:
#ifdef GCF_TA_SUPPORT
                MMISET_SetCallWaitStatus(s_cur_dual_sys,FALSE);
                if(MMI_GMMREG_RAT_GPRS == MMIAPIPHONE_GetTDOrGsm(s_cur_dual_sys))
                {
                    MMISET_DisplayBannerInfo(win_id,STR_CALL_WAITING_OFF_EXT01);
                }
                else
#endif
                {
                    MMISET_CloseCallWaitOrBar(s_cur_dual_sys);
                    MMK_CloseWin(win_id);
                }
                break;
                    
                case ID_SET_CALL_WAITING_STATUS:   
#ifdef GCF_TA_SUPPORT
                if(MMI_GMMREG_RAT_GPRS == MMIAPIPHONE_GetTDOrGsm(s_cur_dual_sys))
                {
                    if(MMISET_GetCallWaitStatus(s_cur_dual_sys))
                    {
                        MMIAPISET_CheckCWStatus(STR_STATUS_ACTIVATED_EXT01);
                    }
                    else
                    {
                        MMIAPISET_CheckCWStatus(STR_STATUS_NOT_ACTIVATED_EXT01);
                    }
                }
                else
#endif
                {
                    MMISET_InterrogateStatus(s_cur_dual_sys);//for cr226576
                    MMK_CloseWin(win_id);
                }
                break;
                    
                default:
                    break;
                }
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
                MMK_CloseWin(win_id);
            }
            
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMK_CloseWin(MMISET_COM_SELECT_SIM_WIN_ID);
#endif
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}


#if 0
/*****************************************************************************/
//  Description : InsertOptMenuNodeByLabel
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void InsertOptMenuNodeByLabel(
                                    MMI_CTRL_ID_T      ctrl_id,        //控件id
                                    MMI_TEXT_ID_T      text_id,        //插入的节点的文本
                                    uint16             node_id,        //节点编号
                                    uint16             parent_node_id, //父节点编号
                                    uint16             index           //位置
                                    )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index, node_id, parent_node_id, &node_item, ctrl_id);
}  
#endif
/*****************************************************************************/
//  Description : create call waiting pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void CreateCallWaitingtSelectPopWindow(void)
{
    MMK_CreatePubListWin((uint32 *)MMISET_SELECT_POP_WAITING_WIN_TAB,PNULL);
}

/*****************************************************************************/
// Description : to handle set call hide id window
// Global resource dependence : 
// Author:wancan.you
// Note: MMISET_SET_CALL_HIDE_ID_WIN_TAB
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCallHideIdWindow(
                                              MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id,
                                              DPARAM            param
                                              )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    MN_CALL_CLIR_E      clir_type       =   MN_CALL_CLIR_NETWORK_DECIDE;
    MMI_CTRL_ID_T       ctrl_id         =   MMISET_CALL_HIDE_ID_MENU_CTRL_ID;
    uint16              cur_selection   =   0;    
    uint32              dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMISET_CALL_CLIR_MAX, FALSE);
        MMIAPISET_AppendListItemByTextId(TXT_SET_HIDEID,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SET_DISPLAY_ID,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SET_DISPLAY_ID_BY_NETWORK,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);

        clir_type = MMIAPISET_GetClirType(dual_sys);/*lint !e64*/
        switch (clir_type)
        {
        case MN_CALL_CLIR_SUPPRESSION:
            cur_selection = MMISET_CALL_CLIR_SUPPRESSION_E;
            break;
            
        case MN_CALL_CLIR_INVOCATION:
            cur_selection = MMISET_CALL_CLIR_INVOCATION_E;
            break;
            
        case MN_CALL_CLIR_NETWORK_DECIDE:
            cur_selection = MMISET_CALL_CLIR_NETWORK_DECIDE_E;
            break;
            
        default:
            cur_selection = MMISET_CALL_CLIR_NETWORK_DECIDE_E;
            break;
        }
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        
        switch (cur_selection)
        {
        case MMISET_CALL_CLIR_SUPPRESSION_E:
            clir_type = MN_CALL_CLIR_SUPPRESSION;
            break;
            
        case MMISET_CALL_CLIR_INVOCATION_E:
            clir_type = MN_CALL_CLIR_INVOCATION;
            break;
            
        case MMISET_CALL_CLIR_NETWORK_DECIDE_E:
            clir_type = MN_CALL_CLIR_NETWORK_DECIDE;
            break;
            
        default:
            clir_type = MN_CALL_CLIR_NETWORK_DECIDE;
            break;
        }
        
        MMISET_SetClirType(dual_sys, clir_type);/*lint !e64*/
        MMK_CloseWin(win_id);
        break;
        
#if !defined MMI_GUI_STYLE_TYPICAL
    case MSG_APP_CANCEL:
#endif
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : to handle divert to voice box
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDivertVoiceWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    MMI_STRING_T        voice_number    =   {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    GUI_BG_T        edit_bg = {0};
    GUI_BORDER_T    edit_border = {0};
    
    SCI_MEMSET(&voice_number, 0, sizeof(MMI_STRING_T));
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
        GUIAPICTRL_SetBothRect(MMISET_SET_DIVERT_VOICE_CTRL_ID, &edit_both_rect);

        //set edit single
        GUIEDIT_SetStyle(MMISET_SET_DIVERT_VOICE_CTRL_ID,GUIEDIT_STYLE_SINGLE);

        //set bg
        edit_bg.bg_type = GUI_BG_COLOR;
        edit_bg.color   = MMI_WHITE_COLOR;
        edit_bg.shape   = GUI_SHAPE_ROUNDED_RECT;
        GUIEDIT_SetBg(MMISET_SET_DIVERT_VOICE_CTRL_ID,&edit_bg);

        //set border
        edit_border.type  = GUI_BORDER_ROUNDED;
        edit_border.width = 2;
        edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        GUIEDIT_SetBorder(MMISET_SET_DIVERT_VOICE_CTRL_ID,&edit_border);

        //set font
        //GUIEDIT_SetFontColor(MMISET_SET_DIVERT_VOICE_CTRL_ID,MMI_BLACK_COLOR);

        //set left softkey
        GUIEDIT_SetSoftkey(MMISET_SET_DIVERT_VOICE_CTRL_ID,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_DIVERT_VOICE_CTRL_ID);
        break;
        
 #ifndef MMIPUBWIN_PDASTYLE_SUPPORT 
    case MSG_FULL_PAINT:
         MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_DIVERT);
        break;
#endif
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //get voice box number
        GUIEDIT_GetString(MMISET_SET_DIVERT_VOICE_CTRL_ID,&voice_number);
        if (0 != voice_number.wstr_len)
        {
            if (MMIAPICOM_IsValidNumberStringEx(&voice_number))
            {
                uint32 i = 0;
                BOOLEAN is_valid = TRUE;

                for (i = 0; i < voice_number.wstr_len; i++)
                {
                    if ('*' == voice_number.wstr_ptr[i]
                        || '#' == voice_number.wstr_ptr[i]
                        || 'p' == voice_number.wstr_ptr[i]
                        || 'w' == voice_number.wstr_ptr[i]
                        || 'P' == voice_number.wstr_ptr[i]
                        || 'W' == voice_number.wstr_ptr[i])
                    {
                        is_valid = FALSE;

                        break;
                    }
                }

                if (is_valid)
                {
                    MMISET_CallForwarding((MN_DUAL_SYS_E)MMIAPISET_GetForwardSimCards(),&voice_number);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
                }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
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


/*****************************************************************************/
//  Description : to handle divert to phone number
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDivertNumberWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    uint8                   num_len         =   0,tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    MMI_RESULT_E            recode          =   MMI_RESULT_TRUE;
    MMI_STRING_T           voice_number     =   {0};
    MMIPB_BCD_NUMBER_T      bcd_number      =   {0};
    uint32                  dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id);
	MMI_CTRL_ID_T edit_name_ctrl_id = MMISET_SET_DIVERT_NUMBER_CTRL_ID;//MMIPUB_INPUTFIELD_CTRL_ID;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    GUI_BG_T        edit_bg = {0};
    GUI_BORDER_T    edit_border = {0};

    SCI_MEMSET(&voice_number, 0, sizeof(MMI_STRING_T));
    SCI_MEMSET(&bcd_number, 0, sizeof(MMIPB_BCD_NUMBER_T));
    
    switch (msg_id)
    {
		case MSG_OPEN_WINDOW:
 		//case MSG_FULL_PAINT:
// 			GUIEDIT_SetIm(edit_name_ctrl_id,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);

			GUIWIN_SetSoftkeyBtnState(win_id,MIDDLE_BUTTON,FALSE,FALSE); 
			GUIEDIT_SetMaxLen(edit_name_ctrl_id, MMISET_MAX_VM_NUMBER_LEN);
            GUIEDIT_SetSoftkey(MMISET_SET_DIVERT_NUMBER_CTRL_ID, 0, 1, STXT_MAIN_PB, TXT_COMMON_OK, PNULL);
            MMK_SetAtvCtrl(win_id, MMISET_SET_DIVERT_NUMBER_CTRL_ID);
		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:

        //get voice box number
        GUIEDIT_GetString(MMISET_SET_DIVERT_NUMBER_CTRL_ID,&voice_number);

        if (0 == voice_number.wstr_len)//enter phone booke
        {
            //调用phonebooke接口提供的单选列表
            // MMIAPIPB_OpenNumberList(MMIPB_LIST_FOR_SMS,MMIPB_LIST_RADIO,1,PNULL);
            MMIAPIPB_OpenMainWin(
                MMIPB_ALL_CONTACT,//
                MMIPB_LIST_RADIO, //IN: 号码列表的类型
                MMIPB_LIST_FIELD_NUMBER,
                1,//如果是多选时，此项限制了多选的最大个数
                PNULL,
                SelectContactCallback
                );
        }
        else if (MMIAPICOM_IsValidNumberStringEx(&voice_number))//enter call forwarding
		{
			uint32 i = 0;
			BOOLEAN is_valid = TRUE;

			for (i = 0; i < voice_number.wstr_len; i++)
			{
				if ('*' == voice_number.wstr_ptr[i]
				|| '#' == voice_number.wstr_ptr[i]
				|| 'p' == voice_number.wstr_ptr[i]
				|| 'w' == voice_number.wstr_ptr[i]
				|| 'P' == voice_number.wstr_ptr[i]
				|| 'W' == voice_number.wstr_ptr[i])
				{
					is_valid = FALSE;

					break;
				}
			}

			if (is_valid)
			{
#ifdef MMI_MULTI_SIM_SYS_SINGLE	
				MMISET_CallForwarding(MN_DUAL_SYS_1,&voice_number);/*lint !e64*/
#else
				MMISET_CallForwarding(dual_sys,&voice_number);
#endif
			}
			else
			{
				MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
			}
		}
		else
		{
			MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
		}
        break;

    case MMI_SELECT_CONTACT_CNF:
        //get select phone number
        if (0 < MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param))
        {
            wchar tele_num_wstr[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
            
            MMIAPIPB_GetSelectNumber((MMIPB_HANDLE_T)param, &bcd_number,MMIPB_BCD_NUMBER_MAX_LEN,  0);       
            //MMIAPIPB_GetDetailInSelectedList(PNULL,&bcd_number,PNULL,0,PNULL,PNULL);
            //generate display number
            num_len = MMIAPIPB_BCDNumberToString(&bcd_number, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);
//             num_type = MMIPB_GetNumberTypeFromUint8(bcd_number.npi_ton);
//             num_len = MMIAPICOM_GenDispNumber(num_type,
//                 bcd_number.number_len,
//                 bcd_number.number, 
//                 tele_num,
//                 MMISET_PHONE_NUM_MAX_LEN + 2);

            num_len = MIN(num_len, MMISET_PHONE_NUM_MAX_LEN);
            
            MMI_STRNTOWSTR( tele_num_wstr, MMISET_PHONE_NUM_MAX_LEN, tele_num, num_len, num_len );
            
            //set phone number editbox
            GUIEDIT_SetString(MMISET_SET_DIVERT_NUMBER_CTRL_ID, tele_num_wstr, num_len);
        }
        break;

	case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
		//recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id, msg_id, param);
        break;
    }
    return recode;
}


/*****************************************************************************/
//  Description :open voice message divert call number window
//  Global resource dependence : 
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenDivertCallWin(uint8   mail_len,
                                        uint8*  voice_mail_ptr
                                        )
{       
    if (0 != mail_len)//是否有语音信箱号码
    {
        wchar mail_wstr[ MMISMS_PBNUM_MAX_LEN + 1 ] = {0};

        mail_len = MIN(mail_len, MMISMS_PBNUM_MAX_LEN);
        
        MMI_STRNTOWSTR( mail_wstr, MMISMS_PBNUM_MAX_LEN, voice_mail_ptr, mail_len, mail_len );
        
        MMK_CreatePubEditWin((uint32*)MMISET_SET_DIVERT_VOICE_WIN_TAB,PNULL);
        
        // set the edit box init param
        GUIEDIT_SetString(
            MMISET_SET_DIVERT_VOICE_CTRL_ID,
            mail_wstr,
            mail_len);
        //SCI_TRACE_LOW:"HandleGetVoiMessAddrCnf:enter divert voice box!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_4491_112_2_18_2_50_25_70,(uint8*)"");
    }
    else
    {
        SCI_TRACE_LOW("HandleSetMainMenuWindow:enter divert voice box and no number!");
        if(MMK_IsOpenWin(MMISET_SET_DIVERT_VOICE_WIN_ID))
        {

        }
//         MMIPUB_OpenAlertWarningWin(TXT_SMS_BOX_EMPTY);
    }
}

LOCAL void UpdatePasswordWinSK(MMI_WIN_ID_T    win_id)
{
    MMI_STRING_T      string = {0};
    MMI_IMAGE_ID_T  icon_id = STR_LIST_DELETE;

    GUIEDIT_GetString(MMIPUB_INPUTFIELD_CTRL_ID, &string);
    if (string.wstr_len == 0)
    {
        icon_id = TXT_COMMON_COMMON_BACK;
        GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);                
    }

    GUIWIN_SeSoftkeytButtonTextId(win_id, icon_id, 2, TRUE);
}

/*****************************************************************************/
//  Description : to handle input call bar password
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBarPasswordWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    uint32          dual_sys    =   s_cur_dual_sys;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    MMI_STRING_T   old_password_value = {0};
    uint16 sim_num = 0;
	CTRLCOMBOX_TEXT_TYPE_T edit_type = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info_ptr = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};    
    GUI_RECT_T        win_rect = {0};    
    GUI_BG_T          edit_bg = {0};
    uint16            logic_width = 0;
    uint16            logic_height = 0;
	GUI_BOTH_RECT_T   display_rect = {0};
    MMI_RESULT_E      result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T     *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    GUI_RECT_T        text_rect    = {0};
    MMI_HANDLE_T      ctrl_handle = 0;
    MMI_WIN_ID_T        change_pw_win_id = MMISET_SET_BAR_CHANGE_PW_WIN_ID;
    MMI_STRING_T      string = {0};
    wchar dot_str[] = {'.'};
    MMI_CTRL_ID_T       input_field_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMISET_SET_BAR_PASSWORD_CTRL_ID );

    if (win_info_ptr != PNULL)
    {
        ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_TEXT_CTRL_ID );
    }

    switch (msg_id)
    {

    case MSG_OPEN_WINDOW:
    case MSG_FULL_PAINT:
        GUIEDIT_SetMaxLen(input_field_ctrl_id,MMISET_BAR_PASSWORD_MAX_LEN);
        UpdatePasswordWinSK(win_id);
        MMIPUB_SetInputDialogImType(win_id, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL, GUIIM_TAG_CLASSIC_NUM);
        GUIEDIT_ConfigImCustomKey(input_field_ctrl_id, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(input_field_ctrl_id, GUIIM_TAG_DOT_NUM);
       GUIEDIT_SetSoftkey(MMISET_SET_BAR_PASSWORD_CTRL_ID,0,MMISET_BAR_PASSWORD_MAX_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
       MMK_SetAtvCtrl(win_id,MMISET_SET_BAR_PASSWORD_CTRL_ID);
        break;
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_APP_WEB:	
    case MSG_CTL_MIDSK:
		GUIEDIT_GetString(input_field_ctrl_id,&old_password_value);
		if (old_password_value.wstr_len >= MMISET_BAR_PASSWORD_MAX_LEN)
		{
			if (MMIAPICOM_IsValidNumberStringEx(&old_password_value))
			{
		        SCI_MEMSET(g_mmiset_ss_pwd.old_pwd, 0x00, sizeof(g_mmiset_ss_pwd.old_pwd));
				MMI_WSTRNTOSTR((uint8 *)g_mmiset_ss_pwd.old_pwd,
                            MMISET_BAR_PASSWORD_MAX_LEN,
                            old_password_value.wstr_ptr,
                            old_password_value.wstr_len,
                            old_password_value.wstr_len);
				
        		sim_num = MMIAPIPHONE_GetSimAvailableNum(PNULL,0);

		        switch (s_call_bar_operation)
		        {
		        case MMISET_CALL_BAR_OPEN:
		            if (sim_num > 0)
		            {
		                MMISET_StartCallWaitOrBar(dual_sys);/*lint !e64*/
		            }
		            else
		            {
		                MMIAPIPHONE_AlertSimNorOKStatus();
		            }
		            break;
		            
		        case MMISET_CALL_BAR_CLOSE:
		            if (sim_num > 0)
		            {
		                MMISET_CloseCallWaitOrBar(dual_sys);/*lint !e64*/
		            }
		            else
		            {
		                MMIAPIPHONE_AlertSimNorOKStatus();
		            }
		            break;

		        case MMISET_CALL_BAR_CANCEL_ALL:
		            if (sim_num > 0)
		            {
		                MMISET_CloseAllCallBar(dual_sys);/*lint !e64*/
		            }
		            else
		            {
		                MMIAPIPHONE_AlertSimNorOKStatus();
		            }
		            break;

		        // change call-bar password
		        case MMISET_CALL_BAR_MODIFY_PWD:
		            if (sim_num > 0)
		            {
		               // MMK_CreatePubEditWin((uint32*)MMISET_SET_BAR_CHANGE_PW_WIN_TAB, (ADD_DATA)dual_sys);
					   MMK_CloseWin(MMISET_SET_BAR_PASSWORD_WIN_ID);
					   MMI_GetLabelTextByLang(STR_NEW_BARRING_CODE, &string);
					   MMIPUB_OpenDialogPasswordWinByTextPtr(&string,&change_pw_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleRegisterNewBarPasswordWindow);
					}
		            else
		            {
		                MMIAPIPHONE_AlertSimNorOKStatus();
		            }       
		            break;

		        default:
		            break;
		        }
			}
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
            }
            MMK_CloseWin(win_id);
        }
        else
        {
            //MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);      
        break;
        
    case MSG_APP_FLIP:
    //case MSG_APP_RED:
        recode = MMI_RESULT_FALSE;
        break;
        
    default:
       // recode = MMI_RESULT_FALSE;
	   recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;
}



/*****************************************************************************/
//  Description : to handle register new call bar password
//  Global resource dependence : 
//  Author:maql
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRegisterNewBarPasswordWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    MMI_STRING_T    title       =   {0};
    uint32          dual_sys    =   s_cur_dual_sys;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    MMI_WIN_ID_T        change_pw_again_win_id = MMISET_SET_BAR_CHANGE_PW_AGAIN_WIN_ID;
    MMI_STRING_T      string = {0};
    wchar dot_str[] = {'.'};
    MMI_CTRL_ID_T       input_field_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    case MSG_FULL_PAINT:
        GUIEDIT_SetMaxLen(input_field_ctrl_id,MMISET_BAR_PASSWORD_MAX_LEN);
        UpdatePasswordWinSK(win_id);
        MMIPUB_SetInputDialogImType(win_id, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL, GUIIM_TAG_CLASSIC_NUM);
        GUIEDIT_ConfigImCustomKey(input_field_ctrl_id, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(input_field_ctrl_id, GUIIM_TAG_DOT_NUM);
            break;
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
		SCI_MEMSET(g_mmiset_ss_pwd.new_pwd, 0x00, sizeof(g_mmiset_ss_pwd.new_pwd));
        GUIEDIT_GetString(input_field_ctrl_id,&new_password_value);
		if (new_password_value.wstr_len >= MMISET_BAR_PASSWORD_MAX_LEN)
		{
			if (MMIAPICOM_IsValidNumberStringEx(&new_password_value))
			{
				MMI_WSTRNTOSTR((uint8 *)g_mmiset_ss_pwd.new_pwd,
				                        MMISET_BAR_PASSWORD_MAX_LEN,
				                        new_password_value.wstr_ptr,
				                        new_password_value.wstr_len,
				                        new_password_value.wstr_len);
				MMI_GetLabelTextByLang(STR_VERIFY_BARRING_CODE, &string);
				MMIPUB_OpenDialogPasswordWinByTextPtr(&string,&change_pw_again_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleRegisterNewBarPasswordAgainWindow);
			}
			else
			{
				MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
			}
		}
		else
		{
			//MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
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


LOCAL MMI_RESULT_E  HandleCloseDifferentPasswordWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    uint32          dual_sys    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;
    case MSG_APP_OK:	
        break;
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMK_CloseWin( win_id ); 	
        break;
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
        }
    return recode;
}

/*****************************************************************************/
//  Description : to handle register new call bar password again
//  Global resource dependence : 
//  Author:maql
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRegisterNewBarPasswordAgainWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    MMI_STRING_T    title       =   {0};
    uint32          dual_sys    =   s_cur_dual_sys;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    wchar dot_str[] = {'.'};
    MMI_CTRL_ID_T       input_field_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    case MSG_FULL_PAINT:
        GUIEDIT_SetMaxLen(input_field_ctrl_id,MMISET_BAR_PASSWORD_MAX_LEN);
        UpdatePasswordWinSK(win_id);
        MMIPUB_SetInputDialogImType(win_id, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL, GUIIM_TAG_CLASSIC_NUM);
        GUIEDIT_ConfigImCustomKey(input_field_ctrl_id, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(input_field_ctrl_id, GUIIM_TAG_DOT_NUM);
            break;
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:        
		SCI_MEMSET(g_mmiset_ss_pwd.new_pwd_two, 0x00, sizeof(g_mmiset_ss_pwd.new_pwd_two));
        GUIEDIT_GetString(input_field_ctrl_id,&new_password_again_value);
		if (new_password_again_value.wstr_len >= MMISET_BAR_PASSWORD_MAX_LEN)
		{
			if (MMIAPICOM_IsValidNumberStringEx(&new_password_again_value))
			{
				MMI_WSTRNTOSTR((uint8 *)g_mmiset_ss_pwd.new_pwd_two,
				                MMISET_BAR_PASSWORD_MAX_LEN,
				                new_password_again_value.wstr_ptr,
				                new_password_again_value.wstr_len,
				                new_password_again_value.wstr_len);

				if(MMIAPICOM_StrCompare(&new_password_value, &new_password_again_value))
				{
					MMISET_ChangeCallBarPassWord(dual_sys);
				}
				else
				{
					//MMIPUB_OpenAlertWarningWin(TXT_MSGBOX_PWD_CONFLICT);
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MSGBOX_PWD_CONFLICT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleCloseDifferentPasswordWindow);
					GUIEDIT_ClearAllStr(MMISET_SET_BAR_CHANGE_PW_CTRL_ID);
				}
			}
			else
			{
				MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
			}
		}
        else
        {
            //MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
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


/*****************************************************************************/
// Description : open select network win
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-21
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSelNetWorkWin(MN_DUAL_SYS_E dual_sys)
{
    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else
    {
#if defined(MMI_PREFER_PLMN_SUPPORT) && defined MMI_GUI_STYLE_TYPICAL
        MMK_CreateWin((uint32*)MMISET_SET_NETWORK_SETTING_WIN_TAB,(ADD_DATA)dual_sys);
#else
#if defined (MMI_DUALMODE_ENABLE) || defined(MMI_2G_4G_SUPPORT)
//#ifdef MMI_SMART_DUAL_SIM_SUPPORT
        MMK_CreateWin((uint32*)MMISET_NETWORK_METHOD_WIN_TAB,(ADD_DATA)dual_sys);
/*#else
        MMK_CreatePubListWin((uint32*)MMISET_NETWORK_METHOD_WIN_TAB,(ADD_DATA)dual_sys);
#endif*/
#else
        MMK_CreatePubListWin((uint32*)MMISET_SET_SELECT_NETWORK_WIN_TAB,(ADD_DATA)dual_sys);
#endif
#endif
    }
}

/*****************************************************************************/
// Description : open set network win
// Global resource dependence : 
// Author: mary.xiao
// Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSetNetWorkWin(void)
{
    MMISET_SIM_SELECT_TYPE_E select_type = 0;
    uint32 sim_num = 0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    select_type = MMISET_SET_NETWORK;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
#if defined(MMI_PREFER_PLMN_SUPPORT) && defined MMI_GUI_STYLE_TYPICAL
        MMK_CreateWin((uint32*)MMISET_SET_NETWORK_SETTING_WIN_TAB,(ADD_DATA)sim_sys);
#else
#ifdef MMI_DUALMODE_ENABLE
        MMK_CreatePubListWin((uint32*)MMISET_NETWORK_METHOD_WIN_TAB,(ADD_DATA)sim_sys);
#else
        MMK_CreatePubListWin((uint32*)MMISET_SET_SELECT_NETWORK_WIN_TAB,(ADD_DATA)sim_sys);
#endif
#endif
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#endif
}

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
// Description : open set prefer network win
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenPreferNetWorkWin(void)
{
    MMISET_SIM_SELECT_TYPE_E select_type = 0;
    uint32 sim_num = 0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    select_type = MMISET_SET_PREFER_NETWORK;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);

    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMK_CreateWin((uint32*)MMISET_SET_DISP_PREFER_NETWORK_WIN_TAB, (ADD_DATA)sim_sys);
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#endif
}
#endif

/*****************************************************************************/
//  Description : to select network
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectNetworkWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    uint16 cur_selection = 0;
    uint16 no_seltction = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MN_PHONE_SELECT_PLMN_T  sel_net_mode    =   {MN_PHONE_PLMN_SELECT_MANUAL_MODE, 0};/*lint !e785*/
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_SELECT_NETWORK_CTRL_ID,MMISET_NETWORK_SEL_MAX_ITEM, FALSE );//max item 2
        MMISET_AppendSelectNetworkListItem(dual_sys,MMISET_SET_SELECT_NETWORK_CTRL_ID);/*lint !e64*/

        GUILIST_SetOwnSofterKey( MMISET_SET_SELECT_NETWORK_CTRL_ID, TRUE );
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_SELECT_NETWORK_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_GET_FOCUS:
        MNPHONE_GetPlmnSelectModeEx(dual_sys, &sel_net_mode);/*lint !e64*/
        switch (sel_net_mode.select_mode)
        {
        case MN_PHONE_PLMN_SELECT_AUTO_MODE:
            cur_selection = 0;
            no_seltction = 1;
            break;
            
        case MN_PHONE_PLMN_SELECT_MANUAL_MODE:
            cur_selection = 1;
            no_seltction = 0;
            break;
            
        default:
            //SCI_TRACE_LOW:"HandleSelectNetworkWindow: select network mode is %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_4923_112_2_18_2_50_26_72,(uint8*)"d",sel_net_mode.select_mode);
            break;
        }
        GUILIST_SetSelectedItem( MMISET_SET_SELECT_NETWORK_CTRL_ID, no_seltction, FALSE );
        GUILIST_SetSelectedItem( MMISET_SET_SELECT_NETWORK_CTRL_ID, cur_selection, TRUE );
        break;
        
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            MMISET_SetSelectNetworkMode(dual_sys,MMISET_SET_SELECT_NETWORK_CTRL_ID);/*lint !e64*/
            #if !defined MMI_GUI_STYLE_TYPICAL
            MMK_CloseWin( win_id );
            #endif
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            // 释放保存网络列表信息的指针
            if (PNULL != g_plmn_detail_list_ptr)
            {
                SCI_FREE(g_plmn_detail_list_ptr);
                g_plmn_detail_list_ptr = PNULL;
            }
            
            MMK_CloseWin( win_id );      
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }   
    return recode;
}
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
//  Description : 获得未使用的IPNumber ID
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
uint8 GetNotUsedIPNumberId(MN_DUAL_SYS_E dual_sys)
{
    MMISET_IP_NUMBER_INFO_T     ip_number_info  =   {0};
    int16                       i               =   0;
    
    MMISET_GetIPNumberInfo(dual_sys,&ip_number_info);
    
    for ( i = 0; i < MMISET_IPNUMBER_MAX_NUM; i++ )
    {
        if(0 == strlen( (char*)ip_number_info.number[i] ))
        {
            return i;
        }
    }

    return MMISET_IPNUMBER_MAX_NUM;
}

/*****************************************************************************/
//  Description : list ip number window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleListIpNumberWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    uint32                      dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_LIST_IPNUMBER_CTRL_ID;
    MMISET_IP_NUMBER_INFO_T     ip_number_info  =   {0};/*lint !e64*/
    uint16                      cur_index       =   0;
    uint8                       origin_active_id    =   0;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMISET_IPNUMBER_MAX_NUM, FALSE );//max item 5
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        MMISET_GetIPNumberInfo(dual_sys, &ip_number_info);/*lint !e64*/
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_RemoveAllItems(ctrl_id);
        AppendIpNumberItemToList(dual_sys,ctrl_id);/*lint !e64*/
        GUILIST_SetSelectedItem(ctrl_id, ip_number_info.active_id, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_index);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
        s_ipnumber_list_cur_id = GUILIST_GetCurItemIndex(ctrl_id);
        if(!IsActiveIPNumber(dual_sys, s_ipnumber_list_cur_id))/*lint !e64*/
        {
            MMK_CreateWin((uint32*)MMISET_IPNUMBER_ACTIVE_OPT_WIN_TAB, (ADD_DATA)dual_sys);
        }
        else
        {
            MMK_CreateWin((uint32*)MMISET_IPNUMBER_DEACTIVE_OPT_WIN_TAB, (ADD_DATA)dual_sys);
        }
        break;
#if defined(MMI_PDA_SUPPORT)
    case MSG_APP_MENU:
        MMK_CreateWin((uint32*)MMISET_IPNUMBER_ADD_OPT_WIN_TAB, (ADD_DATA)dual_sys);
        break;
#endif
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        s_ipnumber_list_cur_id = GUILIST_GetCurItemIndex(ctrl_id);
        if(!IsActiveIPNumber(dual_sys, s_ipnumber_list_cur_id))/*lint !e64*/
        {
            MMISET_GetIPNumberInfo(dual_sys,&ip_number_info);/*lint !e64*/
            origin_active_id = ip_number_info.active_id;
            
            if(origin_active_id != s_ipnumber_list_cur_id)
            {
                ip_number_info.active_id = s_ipnumber_list_cur_id; 
                MMISET_SetIPNumberInfo(&ip_number_info);
                GUILIST_SetSelectedItem( MMISET_LIST_IPNUMBER_CTRL_ID, origin_active_id, FALSE );
                GUILIST_SetSelectedItem( MMISET_LIST_IPNUMBER_CTRL_ID, s_ipnumber_list_cur_id, TRUE );               
            }
        }
        else
        {
            MMISET_GetIPNumberInfo(dual_sys,&ip_number_info);/*lint !e64*/
            ip_number_info.active_id = MMISET_IPNUMBER_MAX_NUM; //means ip number invalid
            
            MMISET_SetIPNumberInfo(&ip_number_info);
            GUILIST_SetSelectedItem( MMISET_LIST_IPNUMBER_CTRL_ID, s_ipnumber_list_cur_id, FALSE );
        }
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : Is this ip number active
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
BOOLEAN IsActiveIPNumber(MN_DUAL_SYS_E dual_sys,uint16 cur_index)
{
    MMISET_IP_NUMBER_INFO_T     ipnumber_info   =   {0};/*lint !e64*/
    
    MMISET_GetIPNumberInfo(dual_sys, &ipnumber_info);
    
    return (ipnumber_info.active_id == cur_index);
}

/*****************************************************************************/
//  Description : append ip number item to listbox
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL void AppendIpNumberItemToList(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMI_CTRL_ID_T ctrl_id
                                    )
{
    MMISET_IP_NUMBER_INFO_T     ip_number_info  =   {0};/*lint !e64*/
    int16                       i               =   0;
    
    MMISET_GetIPNumberInfo(dual_sys,&ip_number_info);
    
    for ( i = 0; i < MMISET_IPNUMBER_MAX_NUM; i++ )
    {
        if(0 < strlen( (char*)ip_number_info.number[i] ))
        {
            AppendIpNumberItem( ctrl_id, &ip_number_info, i, FALSE );
        }
    }
}

/*****************************************************************************/
//  Description : append listbox item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendIpNumberItem(
                              MMI_CTRL_ID_T                 ctrl_id,
                              const MMISET_IP_NUMBER_INFO_T *ip_info_ptr,
                              uint16                        pos,
                              BOOLEAN                       is_update
                              )
{
    uint8                   number_len  =   0;
    GUILIST_ITEM_T          item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data   =   {0};/*lint !e64*/
    MMI_IMAGE_ID_T          image_id    =   IMAGE_NULL;
    wchar                   temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    
    //SCI_ASSERT( pos < MMISET_IPNUMBER_MAX_NUM );
   if(pos >= MMISET_IPNUMBER_MAX_NUM)
   {
        //SCI_TRACE_LOW:"mmisetting_assert AppendIpNumberItem pos %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_5100_112_2_18_2_50_26_73,(uint8*)"d", pos);
        return;
    }     
#if !defined MMI_GUI_STYLE_TYPICAL
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
       
    if ( pos == ip_info_ptr->active_id )
    {
        image_id = IMAGE_RADIO_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_RADIO_UNSELECTED_ICON;
    }
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    number_len = strlen( (char*)ip_info_ptr->number[pos] );
    
    item_data.item_content[0].item_data.text_buffer.wstr_len = MIN( GUILIST_STRING_MAX_NUM, number_len );
    
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;
    
    MMI_STRNTOWSTR( temp_wstr,
        GUILIST_STRING_MAX_NUM,
        (uint8*)ip_info_ptr->number[pos], 
        item_data.item_content[0].item_data.text_buffer.wstr_len,
        item_data.item_content[0].item_data.text_buffer.wstr_len );

    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = image_id;
#else
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    if ( pos == ip_info_ptr->active_id )
    {
        image_id = IMAGE_RADIO_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_RADIO_UNSELECTED_ICON;
    }
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = image_id;
    
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    number_len = strlen( (char*)ip_info_ptr->number[pos] );
    
    item_data.item_content[1].item_data.text_buffer.wstr_len = MIN( GUILIST_STRING_MAX_NUM, number_len );
    
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = temp_wstr;
    
    MMI_STRNTOWSTR( temp_wstr,
        GUILIST_STRING_MAX_NUM,
        (uint8*)ip_info_ptr->number[pos], 
        item_data.item_content[1].item_data.text_buffer.wstr_len,
        item_data.item_content[1].item_data.text_buffer.wstr_len );
#endif
    
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}

/*****************************************************************************/
//  Description : handle ip number option menu window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIpNumberOptionWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMI_RESULT_E                recode              =   MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T          group_id           =   0;
    MMI_MENU_ID_T                menu_id            =   0;
    MMISET_IP_NUMBER_INFO_T     ipnumber_info       =   {0};/*lint !e64*/
    uint8                       origin_active_id    =   0;
    uint32                      dual_sys            =   (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T               list_id         =   MMISET_LIST_IPNUMBER_CTRL_ID;
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_IPNUMBER_OPT_CTRL_ID;
    ipnumber_info.dual_sys = dual_sys;/*lint !e64*/
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16  total_num = 0;

            total_num = GUILIST_GetTotalItemNum(list_id);

            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);

            if(0 == total_num)
            {
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_ACTIVE, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_DEACTIVE, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_MODIFY, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_DEL, TRUE);
            }
            else if(MMISET_IPNUMBER_MAX_NUM == total_num)
            {
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_ADD, TRUE);
            }

            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_IPNUMBER_ACTIVE:
            {
                MMISET_GetIPNumberInfo(dual_sys,&ipnumber_info);/*lint !e64*/
                origin_active_id = ipnumber_info.active_id;
                
                if(origin_active_id != s_ipnumber_list_cur_id)
                {
                    ipnumber_info.active_id = s_ipnumber_list_cur_id; 
                    MMISET_SetIPNumberInfo(&ipnumber_info);
                    GUILIST_SetSelectedItem( list_id, origin_active_id, FALSE );
                    GUILIST_SetSelectedItem( list_id, GUILIST_GetCurItemIndex(list_id), TRUE );               
                }
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);           
                MMK_CloseWin(win_id);
            }
            break;
            
        case ID_SET_IPNUMBER_DEACTIVE:
            {
                MMISET_GetIPNumberInfo(dual_sys,&ipnumber_info);/*lint !e64*/
                ipnumber_info.active_id = MMISET_IPNUMBER_MAX_NUM; //means ip number invalid
                
                MMISET_SetIPNumberInfo(&ipnumber_info);
                GUILIST_SetSelectedItem( list_id, GUILIST_GetCurItemIndex(list_id), FALSE );
                
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);           
                MMK_CloseWin(win_id);
            }
            break;

        case ID_SET_IPNUMBER_MODIFY:
            s_is_add_ipnumber = FALSE;
            MMK_CreatePubEditWin((uint32*)MMISET_MODIFY_IPNUMBER_WIN_TAB, (ADD_DATA)dual_sys);
            MMK_CloseWin(win_id);
            break;
            
        case ID_SET_IPNUMBER_ADD:
            s_is_add_ipnumber = TRUE;
            MMK_CreatePubEditWin((uint32*)MMISET_MODIFY_IPNUMBER_WIN_TAB, (ADD_DATA)dual_sys);
            MMK_CloseWin(win_id);
            break;

        case ID_SET_IPNUMBER_DEL:
            {
                uint16                      index = 0;
                uint16                      i = 0;
                MMISET_IP_NUMBER_INFO_T     ip_info = {0};

                index = GUILIST_GetCurItemIndex(MMISET_LIST_IPNUMBER_CTRL_ID);

                if (index < MMISET_IPNUMBER_MAX_NUM)
                {
                    MMISET_GetIPNumberInfo(dual_sys, &ip_info);
 
                    if(ip_info.active_id >= index)
                    {
                        ip_info.active_id--;
                    }

                    for(i = index; i < MMISET_IPNUMBER_MAX_NUM - 1; i++)
                    {
                        SCI_MEMCPY(ip_info.number[i], ip_info.number[i + 1], (MMISET_IPNUMBER_MAX_LEN + 1));
                    }

                    SCI_MEMSET(ip_info.number[MMISET_IPNUMBER_MAX_NUM - 1], 0, (MMISET_IPNUMBER_MAX_LEN + 1));

                    MMISET_SetIPNumberInfo(&ip_info);
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
                }

                MMK_CloseWin(win_id);
            }
            break;
        default:
            //SCI_PASSERT(FALSE,("HandleIpNumberOptionWindow menu_id = %d", menu_id));  
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        case MSG_LOSE_FOCUS:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

#if defined(MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : handle ip number add option menu window
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIpNumberAddOptionWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMI_RESULT_E                recode              =   MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T          group_id           =   0;
    MMI_MENU_ID_T                menu_id            =   0;
    MMISET_IP_NUMBER_INFO_T     ipnumber_info       =   {0};/*lint !e64*/
    //uint8                       origin_active_id    =   0;
    uint32                      dual_sys            =   (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T               list_id         =   MMISET_LIST_IPNUMBER_CTRL_ID;
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_IPNUMBER_ADD_OPT_CTRL_ID;
    ipnumber_info.dual_sys = dual_sys;/*lint !e64*/
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16  total_num = 0;

            total_num = GUILIST_GetTotalItemNum(list_id);

            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);

            if(MMISET_IPNUMBER_MAX_NUM == total_num)
            {
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_ADD, TRUE);
            }
            else
            {
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_ADD, FALSE);
            }

            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {          
        case ID_SET_IPNUMBER_ADD:
            s_is_add_ipnumber = TRUE;
            MMK_CreatePubEditWin((uint32*)MMISET_MODIFY_IPNUMBER_WIN_TAB, (ADD_DATA)dual_sys);
            MMK_CloseWin(win_id);
            break;
        default:
            //SCI_PASSERT(FALSE,("HandleIpNumberOptionWindow menu_id = %d", menu_id));  
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        case MSG_LOSE_FOCUS:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}
#endif

/*****************************************************************************/
//  Description : modify ip number window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleModifyIPNumberWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    uint32              dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id);
    GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    GUI_BG_T            edit_bg         =   {0};
    GUI_BORDER_T        edit_border     =   {0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //@fen.xie MS00186982
        GUIEDIT_SetStyle(MMISET_MODIFY_IPNUMBER_CTRL_ID, GUIEDIT_STYLE_SINGLE);
        
        //set bg
        edit_bg.bg_type = GUI_BG_COLOR;
        edit_bg.color   = MMI_WHITE_COLOR;
        edit_bg.shape   = GUI_SHAPE_ROUNDED_RECT;
        GUIEDIT_SetBg(MMISET_MODIFY_IPNUMBER_CTRL_ID,&edit_bg);

        //set border
        edit_border.type  = GUI_BORDER_ROUNDED;
        edit_border.width = 2;
        edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        GUIEDIT_SetBorder(MMISET_MODIFY_IPNUMBER_CTRL_ID,&edit_border);

        //set font
        //GUIEDIT_SetFontColor(MMISET_MODIFY_IPNUMBER_CTRL_ID,MMI_BLACK_COLOR);

        {
            MMISET_IP_NUMBER_INFO_T     ip_info                                     =   {0};/*lint !e64*/
            wchar                       number_wstr[MMISET_IPNUMBER_MAX_NUM + 1]    =   {0};
            uint16                      num_len                                     =   0;
            GUI_BOTH_RECT_T edit_both_rect = {0};

            edit_both_rect = MMIAPISET_GetPwEditRect(win_id );//MS00199646
            GUIAPICTRL_SetBothRect(MMISET_MODIFY_IPNUMBER_CTRL_ID, &edit_both_rect);

            MMISET_GetIPNumberInfo(dual_sys, &ip_info);/*lint !e64*/
            
            if(s_is_add_ipnumber)
            {
                uint8 not_used_id = GetNotUsedIPNumberId(dual_sys);
                num_len = (uint16)strlen((char*)ip_info.number[not_used_id]);
                MMI_STRNTOWSTR( number_wstr, MMISET_IPNUMBER_MAX_LEN, (uint8*)ip_info.number[not_used_id], num_len, num_len );
            }
            else
            {
                num_len = (uint16)strlen((char*)ip_info.number[s_ipnumber_list_cur_id]);
                MMI_STRNTOWSTR( number_wstr, MMISET_IPNUMBER_MAX_LEN, (uint8*)ip_info.number[s_ipnumber_list_cur_id], num_len, num_len );
            }
            GUIEDIT_SetString(MMISET_MODIFY_IPNUMBER_CTRL_ID, number_wstr, num_len);
        }
        
        //set left softkey
        GUIEDIT_SetSoftkey(MMISET_MODIFY_IPNUMBER_CTRL_ID,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        
        MMK_SetAtvCtrl(win_id, MMISET_MODIFY_IPNUMBER_CTRL_ID);
        break;
        
 #ifndef MMIPUBWIN_PDASTYLE_SUPPORT 
    case MSG_FULL_PAINT:
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_IP);
        break;
#endif
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T                string_info     =   {0};
            MMISET_IP_NUMBER_INFO_T     ip_info         =   {0};/*lint !e64*/
            
            MMISET_GetIPNumberInfo(dual_sys,&ip_info);/*lint !e64*/
            GUIEDIT_GetString(MMISET_MODIFY_IPNUMBER_CTRL_ID, &string_info);
            if(0 != string_info.wstr_len)            
            {
                uint32 i = 0;
                BOOLEAN is_input_valid = TRUE;
                if(s_is_add_ipnumber)
                {
                    uint8 not_used_id = GetNotUsedIPNumberId(dual_sys);
                    
                    SCI_MEMSET(ip_info.number[not_used_id], 0, (MMISET_IPNUMBER_MAX_LEN + 1));
                    
                    MMI_WSTRNTOSTR( (uint8*)ip_info.number[not_used_id], MMISET_IPNUMBER_MAX_LEN, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len);
                    
                    for (i = 0; i < string_info.wstr_len; i++)
                    {
                        if (ip_info.number[not_used_id][i] < '0' || ip_info.number[not_used_id][i] > '9')
                        {
                            is_input_valid = FALSE;
                            break;
                        }
                    }
                }
                else
                {
                    SCI_MEMSET(ip_info.number[s_ipnumber_list_cur_id], 0, (MMISET_IPNUMBER_MAX_LEN + 1));
                    
                    MMI_WSTRNTOSTR( (uint8*)ip_info.number[s_ipnumber_list_cur_id], MMISET_IPNUMBER_MAX_LEN, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len);
                    
                    for (i = 0; i < string_info.wstr_len; i++)
                    {
                        if (ip_info.number[s_ipnumber_list_cur_id][i] < '0' || ip_info.number[s_ipnumber_list_cur_id][i] > '9')
                        {
                            is_input_valid = FALSE;
                            break;
                        }
                    }
                }
                
                if (is_input_valid)
                {
                    MMISET_SetIPNumberInfo(&ip_info);
                    //AppendIpNumberItemToList(dual_sys, MMISET_LIST_IPNUMBER_CTRL_ID);/*lint !e64*/
                    //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
                }
            }
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
#endif
/*****************************************************************************/
//  Description : 处理分钟提示音初始窗口
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMinuteReminderFirstWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_MINUTE_REMINDER_CTRL_ID;
    MMI_STRING_T label_name  = {0};
    
    uint16 sel_item_index = 0;
    uint8 call_reminder_time = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {                   
            GUILIST_ITEM_T                item_t = {0};
            GUILIST_ITEM_DATA_T           item_data = {0};
            uint16     cur_item = 0;

            GUILIST_SetMaxItem(ctrl_id,MMISET_CALL_REMINDER_TIME_MAX, FALSE);//max item 5
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style       = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
            item_t.item_data_ptr = &item_data;
            //item_t.user_data        =  ID_SET_CALL_DIVERTS;
            
            item_data.softkey_id[0] = COMMON_TXT_NULL;
            item_data.softkey_id[1] = STXT_SELECT;
            item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
             item_data.item_content[0].item_data_type       = GUIITEM_DATA_TEXT_ID;                
             item_data.item_content[0].item_data.text_id = STR_SET_MOBILE_PHONEBOOTH;
             item_data.item_content[1].item_data_type       = GUIITEM_DATA_TEXT_ID;
             switch(MMIAPISET_GetMinuteReminderTime())
             {
             case CC_CALL_REMINDER_TIME_OFF:
                 item_data.item_content[1].item_data.text_id = TXT_CLOSE;
                 break;
                 
             case CC_CALL_REMINDER_TIME_50S:
                 item_data.item_content[1].item_data.text_id = STXT_TIME_50_SECONDS_EXT01;
                 break;
                 
             case CC_CALL_REMINDER_TIME_55S:
                 item_data.item_content[1].item_data.text_id = STXT_TIME_55_SECONDS_EXT01;
                 break;
                 
             case CC_CALL_REMINDER_TIME_60S:
                 item_data.item_content[1].item_data.text_id = STXT_TIME_60_SECONDS_EXT01;
                 break;
                 
             default :
                 item_data.item_content[1].item_data.text_id = STR_SETT_LIST_CUSTOMISED_EXT01;
                 break;
             }
             item_data.item_content[1].is_opacity = TRUE;
// 			 GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
             if(!GUILIST_InsertItem(ctrl_id, &item_t, 0))break;

             GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
             GUILIST_SetCurItemIndex(ctrl_id, cur_item);

             GUILIST_SetOwnSofterKey(ctrl_id, TRUE);     
             MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if !defined(MMI_PDA_SUPPORT) 
  //  case MSG_CTL_OK:
#endif
  //  case MSG_APP_OK:
  //  case MSG_APP_WEB:
        sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
        switch (sel_item_index)
        {
        case 0:
            MMK_CloseWin( win_id );  
            MMK_CreateWin((uint32*)MMISET_SET_MINUTE_REMINDER_WIN_TAB,PNULL);
            break;
                        
        default:
            break;
        }
        break;
    //根据用户自定义输入的时间值确定是否选中默认项
    case MSG_GET_FOCUS:
        switch(MMIAPISET_GetMinuteReminderTime())
        {
        case CC_CALL_REMINDER_TIME_OFF:
            sel_item_index = MMISET_CALL_REMINDER_TIME_OFF;
            break;
            
        case CC_CALL_REMINDER_TIME_50S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_50S;
            break;
            
        case CC_CALL_REMINDER_TIME_55S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_55S;
            break;
            
        case CC_CALL_REMINDER_TIME_60S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_60S;
            break;
            
        default :
            sel_item_index = MMISET_CALL_REMINDER_TIME_OTHER;
            MMI_GetLabelTextByLang(STR_SETT_LIST_CUSTOMISED_EXT01, &label_name);
                GUILABEL_SetText(MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID, &label_name, FALSE);
            break;
        }
       
        GUILIST_SetSelectedItem(ctrl_id, sel_item_index, TRUE);
       

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
/*****************************************************************************/
//  Description : 处理分钟提示音
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMinuteReminderWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_MINUTE_REMINDER_CTRL_ID;
    MMI_STRING_T label_name  = {0};
    
    uint16 sel_item_index = 0;
    uint8 call_reminder_time = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_CALL_REMINDER_TIME_MAX, FALSE);//max item 5
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SET_TIME_50S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SET_TIME_55S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SET_TIME_60S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_EXPORT_LOC_CUSTOM, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        
        GUILIST_SetOwnSofterKey(ctrl_id, TRUE);     
        MMK_SetAtvCtrl(win_id,ctrl_id);
        
        switch(MMIAPISET_GetMinuteReminderTime())
        {
        case CC_CALL_REMINDER_TIME_OFF:
            sel_item_index = MMISET_CALL_REMINDER_TIME_OFF;
            break;
            
        case CC_CALL_REMINDER_TIME_50S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_50S;
            break;
            
        case CC_CALL_REMINDER_TIME_55S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_55S;
            break;
            
        case CC_CALL_REMINDER_TIME_60S:
            sel_item_index =MMISET_CALL_REMINDER_TIME_60S;
            break;
            
        default :
            sel_item_index = MMISET_CALL_REMINDER_TIME_OTHER;
            break;
        }
        
        GUILIST_SetCurItemIndex(ctrl_id, sel_item_index);
        GUILIST_SetSelectedItem(ctrl_id, sel_item_index, TRUE);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if !defined(MMI_PDA_SUPPORT) 
    case MSG_CTL_OK:
#endif
    case MSG_APP_OK:
    case MSG_APP_WEB:
        sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
        switch (sel_item_index)
        {
        case MMISET_CALL_REMINDER_TIME_OFF:        
        case MMISET_CALL_REMINDER_TIME_50S:
        case MMISET_CALL_REMINDER_TIME_55S: 
        case MMISET_CALL_REMINDER_TIME_60S:
            call_reminder_time = g_call_reminder_time[sel_item_index];
            MMISET_SetMinuteReminderTime(call_reminder_time);

            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                switch(MMIAPISET_GetMinuteReminderTime())
                {
                case CC_CALL_REMINDER_TIME_OFF:
                    MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
                    break;
                    
                case CC_CALL_REMINDER_TIME_50S:
                    MMI_GetLabelTextByLang(TXT_SET_TIME_50S, &label_name);
                    break;
                    
                case CC_CALL_REMINDER_TIME_55S:
                    MMI_GetLabelTextByLang(TXT_SET_TIME_55S, &label_name);
                    break;
                    
                case CC_CALL_REMINDER_TIME_60S:
                    MMI_GetLabelTextByLang(TXT_SET_TIME_60S, &label_name);
                    break;
                    
                default :
                    MMI_GetLabelTextByLang(TXT_EXPORT_LOC_CUSTOM, &label_name);
                    break;
                }
                        
            GUILABEL_SetText(MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID, &label_name, FALSE);
            MMK_CloseWin( win_id );
            break;
            
        case MMISET_CALL_REMINDER_TIME_OTHER:
            MMK_CreatePubEditWin((uint32*)MMISET_SET_MINUTE_REMINDER_EDIT_WIN_TAB,PNULL);
			break;
            
        default:
            break;
        }
        break;
    //根据用户自定义输入的时间值确定是否选中默认项
    case MSG_GET_FOCUS:
        switch(MMIAPISET_GetMinuteReminderTime())
        {
        case CC_CALL_REMINDER_TIME_OFF:
            sel_item_index = MMISET_CALL_REMINDER_TIME_OFF;
            break;
            
        case CC_CALL_REMINDER_TIME_50S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_50S;
            break;
            
        case CC_CALL_REMINDER_TIME_55S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_55S;
            break;
            
        case CC_CALL_REMINDER_TIME_60S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_60S;
            break;
            
        default :
            sel_item_index = MMISET_CALL_REMINDER_TIME_OTHER;
            MMI_GetLabelTextByLang(TXT_EXPORT_LOC_CUSTOM, &label_name);
                GUILABEL_SetText(MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID, &label_name, FALSE);
            break;
        }
       
        GUILIST_SetSelectedItem(ctrl_id, sel_item_index, TRUE);
       

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
/*****************************************************************************/
//  Description : 自定义分钟提示音窗口
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetMinuteReminderEditWindow(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id =   MMISET_SET_MINUTE_REMINDER_EDITBOX_CTRL_ID;
    MMI_STRING_T time_number = {0};
    wchar num_wchar_str[3] = {0};
    char num_char_str[3] = {0};
    uint32 call_reminder_time = 0;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    GUI_BG_T edit_bg = {0};
    GUI_BORDER_T edit_border = {0};

    time_number.wstr_ptr = num_wchar_str;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

            both_rect.h_rect.top += SET_MINUTE_REMINDER_LABLE_TOP_MARGIN;
            both_rect.v_rect.top += SET_MINUTE_REMINDER_LABLE_TOP_MARGIN;
            both_rect.h_rect.bottom = both_rect.h_rect.top + SET_MINUTE_REMINDER_LABLE_HEIGHT;
            both_rect.v_rect.bottom = both_rect.v_rect.top + SET_MINUTE_REMINDER_LABLE_HEIGHT;

            GUIAPICTRL_SetBothRect(MMISET_SET_MINUTE_REMINDER_LABEL_CTRL_ID, &both_rect);
        }
        IGUICTRL_SetState(MMK_GetCtrlPtr(MMISET_SET_MINUTE_REMINDER_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
        
        GUILABEL_SetTextById(MMISET_SET_MINUTE_REMINDER_LABEL_CTRL_ID,TXT_SET_MINUTE_REMINDER_TIME,FALSE);

        //set bg
        edit_bg.bg_type = GUI_BG_COLOR;
        edit_bg.color = MMI_WHITE_COLOR;
        edit_bg.shape = GUI_SHAPE_ROUNDED_RECT;
        GUIEDIT_SetBg(ctrl_id,&edit_bg);

        //set border
        edit_border.type  = GUI_BORDER_ROUNDED;
        edit_border.width = 2;
        edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        GUIEDIT_SetBorder(ctrl_id,&edit_border);
        
        {
            edit_both_rect = MMITHEME_GetWinClientBothRect(win_id);

            edit_both_rect.h_rect.left = edit_both_rect.v_rect.left = MMI_PASSWORD_EDIT_LEFT;

            edit_both_rect.h_rect.top += SET_MINUTE_REMINDER_LABLE_TOP_MARGIN + SET_MINUTE_REMINDER_LABLE_HEIGHT + MMI_NUMBER_EDIT_TOP_MARGIN;
            edit_both_rect.v_rect.top += SET_MINUTE_REMINDER_LABLE_TOP_MARGIN + SET_MINUTE_REMINDER_LABLE_HEIGHT + MMI_NUMBER_EDIT_TOP_MARGIN;

            edit_both_rect.h_rect.bottom = edit_both_rect.h_rect.top + MMI_NUMBER_EDIT_HEIGHT;
            edit_both_rect.h_rect.right = edit_both_rect.h_rect.right - MMI_NUMBER_EDIT_HEIGHT;
            edit_both_rect.v_rect.bottom = edit_both_rect.v_rect.top + MMI_NUMBER_EDIT_HEIGHT;
            edit_both_rect.v_rect.right = edit_both_rect.v_rect.right - MMI_NUMBER_EDIT_HEIGHT;

        }
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);
        
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_SINGLE);
        
        call_reminder_time = MMIAPISET_GetMinuteReminderTime();
        if (0 != call_reminder_time)
        {
            MMIAPICOM_Int2Str((uint32)call_reminder_time, (uint8*)num_char_str,2);
            MMI_STRNTOWSTR(time_number.wstr_ptr,4,(uint8 *)num_char_str,2,2);
            time_number.wstr_len = MMIAPICOM_Wstrlen(time_number.wstr_ptr);
            GUIEDIT_SetString(ctrl_id, time_number.wstr_ptr, time_number.wstr_len);
        }
        else
        {
            GUIEDIT_SetString(ctrl_id, PNULL, 0);
        }

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_GET_FOCUS:
        call_reminder_time = MMIAPISET_GetMinuteReminderTime();
        if (0 != call_reminder_time)
        {
            MMIAPICOM_Int2Str((uint32)call_reminder_time, (uint8*)num_char_str,2);
            MMI_STRNTOWSTR(time_number.wstr_ptr,4,(uint8 *)num_char_str,2,2);
            time_number.wstr_len = MMIAPICOM_Wstrlen(time_number.wstr_ptr);
            GUIEDIT_SetString(ctrl_id, time_number.wstr_ptr, time_number.wstr_len);
        }
        else
        {
            GUIEDIT_SetString(ctrl_id, PNULL, 0);
        }

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_EDITBOX_UPDATE_STRNUM:
        GUIEDIT_GetString(ctrl_id, &time_number);
        
        MMIAPICOM_WstrToNum(time_number.wstr_ptr, time_number.wstr_len, &call_reminder_time);     
        if (call_reminder_time > CC_CALL_REMINDER_TIME_60S)
        {
            time_number.wstr_ptr = num_wchar_str;
            call_reminder_time = CC_CALL_REMINDER_TIME_60S;
            MMIAPICOM_Int2Str((uint32)call_reminder_time, (uint8*)num_char_str,2);
            MMI_STRNTOWSTR(time_number.wstr_ptr,4,(uint8 *)num_char_str,2,2);
            time_number.wstr_len = MMIAPICOM_Wstrlen(time_number.wstr_ptr);
            GUIEDIT_SetString(ctrl_id, time_number.wstr_ptr, time_number.wstr_len);
            MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id, &time_number);
        
        MMIAPICOM_WstrToNum(time_number.wstr_ptr, time_number.wstr_len, &call_reminder_time);        

        if (0 != call_reminder_time)
        {
            MMISET_SetMinuteReminderTime(call_reminder_time); 
            MMK_CloseWin(win_id);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_INPUT_ERROR);
        }
        break;
 
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:  
        MMK_CloseWin(win_id);
        break;

#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
    case MSG_FULL_PAINT:
#if !defined(MAINLCD_SIZE_176X220) && !defined(MAINLCD_SIZE_128X160)
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_IP);
#else
        SetPwBackground(win_id,ctrl_id,TRUE);
#endif
        break;
#endif

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : deal with signal of APP_MN_DISPLAY_PLMN_IND
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandleDisplayPlmnInd(MN_DUAL_SYS_E dual_sys)
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    if (!MMK_CreateWin((uint32 *)MMISET_SEL_NET_WIN_TAB, (ADD_DATA)dual_sys))
    {
        result = MMI_RESULT_FALSE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 在进行手动选网中，网络连接窗口的处理函数
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleManualSelNetworkConnectWindow(
                                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                       DPARAM             param       // 相应消息的参数
                                                       )
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
   // case MSG_APP_RED:
        //SCI_TRACE_LOW("MMISET_HandleSelNetworkConnectWindow:received MSG_APP_RED and abort plmn search!");
     //   break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}



/*****************************************************************************/
//  Description : to handle remind user will select network through manual win msg
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelNetManualWinMsg(
                                            MMI_WIN_ID_T       win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    GUI_RECT_T              rect             =  MMITHEME_GetClientRect();
    MMI_RESULT_E            recode           = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO        lcd_dev_info     =  {GUI_LCD_0, GUI_BLOCK_0};
    uint32                  dual_sys         =  (uint32)MMK_GetWinAddDataPtr(win_id);
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN; //@$discuss,8800用的是MMI_DEFAULT_MAINLCD_BLOCK，此处直接引用了6800中的block_id是否会有问题？
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);;

            both_rect.h_rect.top = both_rect.v_rect.top = MMISET_SS_INTERROGATE_STATUS_BOTTOM;
            both_rect.h_rect.bottom = both_rect.h_rect.top + (MMISET_SS_INTERROGATE_NUMBER_BOTTOM - MMISET_SS_INTERROGATE_STATUS_BOTTOM);
            both_rect.v_rect.bottom = both_rect.v_rect.top + (MMISET_SS_INTERROGATE_NUMBER_BOTTOM - MMISET_SS_INTERROGATE_STATUS_BOTTOM);

            GUIAPICTRL_SetBothRect(MMISET_SEL_NET_MANUAL_CTRL_ID, &both_rect);
        }
        //set soft key
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK,TXT_NULL,STXT_RETURN,FALSE);
        GUILABEL_SetTextById(MMISET_SEL_NET_MANUAL_CTRL_ID,TXT_SET_MANUAL_SEL,FALSE );
        break;
        
    case MSG_FULL_PAINT:
        //fill rect 
        LCD_FillRect(&lcd_dev_info,rect,MMI_BLACK_COLOR);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //进入手动选网
        //the list of all plmn
        //@YONGWEI.HE
#ifdef MMI_DUALMODE_ENABLE 
        //  MNPHONE_ListPLMNEx(dual_sys, s_current_sel_rat);
        MNPHONE_ListPLMNEx(dual_sys, MN_GMMREG_RAT_GPRS_AND_3G);/*lint !e64*/  //cr149167
#else
        MNPHONE_ListPLMNEx(dual_sys, MN_GMMREG_RAT_GPRS);/*lint !e64*/
#endif
        
        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                                                MMISET_SET_SEARCH_NETWORK_WIN_ID,
                                                                TXT_NETWORK_CONNECT,
                                                                MMIPUB_SOFTKEY_NONE,
                                                                HandleManualSelNetworkConnectWindow);

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

#if defined (MMI_DUALMODE_ENABLE) || defined(MMI_2G_4G_SUPPORT)
/*****************************************************************************/
//  Description : open NetWorkType win
//  Global resource dependence : 
//  Author: panvs.zhong
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenNetWorkTypeWin(void)
{    
    MMK_CreateWin((uint32*)MMISET_NETWORK_TYPE_WIN_TAB,PNULL);
#if 0
    MMISET_SIM_SELECT_TYPE_E select_type = 0;
    uint32 sim_num = 0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    select_type = MMISET_SET_NETWORK_TYPE;
    
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else
    {
        if (MMISET_IsSSProcessing() || (FALSE == MMIAPISET_GetIsQueryCfu()))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SYS_WAITING_AND_TRY_LATER, TXT_NULL,
                IMAGE_PUBWIN_WARNING, PNULL, PNULL,
                MMIPUB_SOFTKEY_ONE, PNULL);
        }
        else
        {
            if (sim_num == 1)
            {
                MMK_CreateWin((uint32*)MMISET_NETWORK_TYPE_WIN_TAB, (ADD_DATA)sim_sys);
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
        }
    }
#endif
}


#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTSettingsWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_MENU_GROUP_ID_T         group_id    =   0;
    MMI_MENU_ID_T               menu_id     =   0;
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISET_CALL_VT_SETTINGS_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMISET_CALL_VT_SETTINGS_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_VT_FALLBACK_HANDLE:
            MMK_CreateWin((uint32*)MMISET_SET_VT_FALLBACK_WIN_TAB,PNULL);
            break;
            
        case ID_SET_VT_SPCIFY_SAVE_PATH:
            MMK_CreateWin((uint32*)MMISET_VT_RECORD_PATH_WIN_TAB,PNULL);
            break;
            
        case ID_SET_VT_REPLACE_IMAGE:
            MMK_CreateWin((uint32*)MMISET_SET_VT_REPLACE_IMAGE_WIN_TAB,PNULL);
            break;  
            
        case ID_SET_VT_LOCAL_VIDEO:
            MMK_CreateWin((uint32*)MMISET_SET_VT_LOCAL_VIDEO_WIN_TAB,PNULL);
            break;
            
        case ID_SET_VT_ONE_TIME_RING:
            MMK_CreateWin((uint32*)MMISET_SET_VT_ONE_TIME_RING_WIN_TAB,PNULL);
            break;            
            
        default:
            //do nothing
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTFallbackSettingsWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_CALL_VT_FALLBACK_SETTINGS_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMISET_VT_FALLBACK_MAX, FALSE );//max item 3
        MMISET_AppendFallbackListItem(ctrl_id);/*lint !e64*/
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMISET_SetSelectFallback(ctrl_id);/*lint !e64*/
        //MMIPUB_OpenAlertSuccessWin( TXT_SET_SUCCESS);
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

/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTReplaceImageSettingsWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_CALL_VT_REPLACE_IMAGE_LIST_CTRL_ID;
    
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        {
            uint16 sel_item_index = 0;
            //从nv中获得替代文件信息
            MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
            MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
            
            sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_SetMaxItem(ctrl_id, MMISET_VT_REPLACE_MAX, FALSE );
            GUILIST_RemoveAllItems(ctrl_id);
            
            {
                GUILIST_ITEM_T item_t = {0};/*lint !e64*/
                GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
                uint32 index = 0;
                item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
                item_t.item_data_ptr = &item_data;
                
                
                for (index = 0; index < MMISET_VT_REPLACE_MAX; index++)
                {
                    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
                    item_data.item_content[0].item_data.text_id = s_replace_title_text_id[index];
                    
                    if (s_replace_file_info[index].is_valid && (s_replace_file_info[index].file_name_len > 0))
                    {
                        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                        item_data.item_content[1].item_data.text_buffer.wstr_len = s_replace_file_info[index].file_name_len;
                        item_data.item_content[1].item_data.text_buffer.wstr_ptr = s_replace_file_info[index].file_name;
                    }
                    else
                    {
                        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
                        item_data.item_content[1].item_data.text_id = TXT_SET_VT_REPLACE_DEFAULT;
                    }   
                    GUILIST_AppendItem( ctrl_id, &item_t );
                }
            }
            GUILIST_SetCurItemIndex(ctrl_id, sel_item_index);
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        {
            MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
            
            uint16 sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
            
            //从nv中获得替代文件信息
            MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
            if (s_replace_file_info[sel_item_index].is_valid && (s_replace_file_info[sel_item_index].file_name_len > 0))
            {
                if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                    break;
                }
                else if (MMIAPIENG_GetIQModeStatus())
                {
                    MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
                    break;
                }
                
                MMK_CreateWin((uint32 *)MMISET_VT_REPLACE_REVIEW_PHOTO_WIN_TAB, PNULL);
            }
            else
            {
                uint32 time_out = 500;
                MMIAPIOCR_ClosePreviewWinOpen();
                //显示请稍候
                MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                MMK_PostMsg(win_id, MSG_MULTIM_WAITING, NULL, 0);                
            }
        }
        //MMK_CloseWin(win_id);
        break;
        
    case MSG_MULTIM_WAITING:
        {
            uint16 sel_item_index = 0;
            MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_PICTURE_JPG,0,MMISET_VT_REPLACE_PIC_SIZE_MAX,0,PNULL,0,0};
            sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
            //select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = s_replace_media_type[sel_item_index];
            MMIPUB_CloseAlertWin();
            select_info.win_id = win_id;
            MMIAPIFMM_OpenSelectPictureWin(&select_info);
        }
        break;
        
    case MSG_MULTIM_SELECTED_RETURN:
        {
            MULTIM_SELECT_RETURN_T * multim_select_ptr = PNULL;
            
           // SCI_ASSERT(param != 0);
        if(param == 0)
        {
             //SCI_TRACE_LOW:"mmisetting_assert HandleVTReplaceImageSettingsWindow"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6023_112_2_18_2_50_28_74,(uint8*)"");
             break;
         } 
            multim_select_ptr = (MULTIM_SELECT_RETURN_T *)param;
            switch (multim_select_ptr->file_type)
            {
            case MMIFMM_FILE_TYPE_PICTURE:
                if (multim_select_ptr->file_name_len > MMIFILE_FULL_PATH_MAX_LEN)
                {
                    return MMI_RESULT_FALSE;
                }
                else if (!IsReplaceImageValid(multim_select_ptr))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
                }
                else
                {
                    MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
                    int16 list_index = GUILIST_GetCurItemIndex(ctrl_id);
                    //SCI_ASSERT(list_index < MMISET_VT_REPLACE_MAX);
               if(list_index >= MMISET_VT_REPLACE_MAX)
               {
                   //SCI_TRACE_LOW:"mmisetting_assert HandleVTReplaceImageSettingsWindow list_index %d"
                   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6045_112_2_18_2_50_28_75,(uint8*)"d", list_index);
                   break;
               }                     
                    //从nv中获得替代文件信息
                    MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
                    
                    s_replace_file_info[list_index].is_valid = TRUE;
                    s_replace_file_info[list_index].file_name_len = MIN((multim_select_ptr->file_name_len), MMIFILE_FULL_PATH_MAX_LEN);
                    SCI_MEMSET(s_replace_file_info[list_index].file_name, 0 , sizeof(s_replace_file_info[list_index].file_name));
                    MMIAPICOM_Wstrncpy(s_replace_file_info[list_index].file_name, multim_select_ptr->file_name_ptr, s_replace_file_info[list_index].file_name_len);
                    s_replace_file_info[list_index].file_size = multim_select_ptr->file_size;
                    s_replace_file_info[list_index].media_type = MMISET_VT_REPLACE_MEDIA_TYPE_IMAGE;
                    s_replace_file_info[list_index].photo_type = multim_select_ptr->pic_type;
                    //存入nv中
                    MMISET_WriteReplaceInfoToNv((MMISET_VT_REPLACE_FILE_INFO_T*)&s_replace_file_info);
                    
                }
                break;
                
            case MMIFMM_FILE_TYPE_MUSIC:
                break;
                
            case MMIFMM_FILE_TYPE_MOVIE:  
                break;
                
            default:
                //SCI_PASSERT(0,("msg_id %d",msg_id));
                break;
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

/*****************************************************************************/
//  Description : 判断替代图片格式是否有效
//  Global resource dependence : none
//  Author: samboo
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsReplaceImageValid(MULTIM_SELECT_RETURN_T * multim_select_ptr)
{
    BOOLEAN is_valid = FALSE;
    uint16 pic_width = 0;
    uint16 pic_height = 0;
    
    if (PNULL == multim_select_ptr)
    {
        //SCI_TRACE_LOW:"IsReplaceImageValid : null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6103_112_2_18_2_50_28_76,(uint8*)"");
    }
    else
    {
        GUIANIM_GetImgWidthHeight(&pic_width, &pic_height, multim_select_ptr->file_name_ptr, multim_select_ptr->file_name_len);
        //SCI_TRACE_LOW:"IsReplaceImageValid width:%d height:%d size:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6108_112_2_18_2_50_28_77,(uint8*)"ddd", pic_width, pic_height, multim_select_ptr->file_size);
        //替代图片分辨率不能大于320*240，文件大小不能超过32k 格式必须是jpeg
        is_valid = (pic_width <= MMISET_VT_REPLACE_PIC_WIDTH_MAX)
            &&(pic_height <= MMISET_VT_REPLACE_PIC_HEIGHT_MAX)
            &&(multim_select_ptr->file_size <= MMISET_VT_REPLACE_PIC_SIZE_MAX)
            &&(GUIANIM_TYPE_IMAGE_JPG == multim_select_ptr->pic_type);
    }
    return is_valid;
}


/*****************************************************************************/
//  Description : 预览自定义图片窗口
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReviewCustomPhotoWinMsg
(
 MMI_WIN_ID_T        win_id, 
 MMI_MESSAGE_ID_E    msg_id, 
 DPARAM              param
 )
{
    MMI_RESULT_E                result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMISET_VT_REPLACE_REVIEW_PHOTO_ANIM_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        //set anim control param
        {
            MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
            uint16 index = GUILIST_GetCurItemIndex(MMISET_CALL_VT_REPLACE_IMAGE_LIST_CTRL_ID);
            
            //从nv中获得替代文件信息
            MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
            MMIAPIFMM_SetPreviewPicParam(FALSE,(wchar *)s_replace_file_info[index].file_name,
                (uint16)s_replace_file_info[index].file_name_len,
                ctrl_id);
            GUIWIN_SetTitleText(win_id, s_replace_file_info[index].file_name, s_replace_file_info[index].file_name_len, TRUE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        {                                                      
            GUI_RECT_T rect = {0};
            GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0}; 
            lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
            lcd_dev_info.block_id = GUI_BLOCK_MAIN;
            rect.left = 0;
            rect.top = MMI_TITLE_HEIGHT;
            rect.right = MMISET_FULLSCREEN_CLIENT_RIGHT;
            rect.bottom = MMISET_FULLSCREEN_CLIENT_BOTTOM;
            LCD_FillRect(&lcd_dev_info, rect, MMI_WINDOW_BACKGROUND_COLOR);
            
        }    
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        MMK_CreateWin((uint32 *) MMISET_VT_REPLACE_REVIEW_PHOTO_OPT_WIN_TAB, PNULL);
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 预览自定义图片窗口
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReviewCustomPhotoOptWinMsg
(
 MMI_WIN_ID_T        win_id, 
 MMI_MESSAGE_ID_E    msg_id, 
 DPARAM              param
 )
{
    uint32          time_out = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMISET_VT_CUSTOM_PHOTO_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T          group_id = 0;
    MMI_MENU_ID_T                menu_id = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, 
            &group_id, 
            &menu_id
            );
        switch(menu_id)
        {
        case ID_SET_VT_CUSTOM_PHOTO_MODIFY:
            //显示请稍候
            MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            MMK_PostMsg(win_id, MSG_MULTIM_WAITING, NULL, 0);
            break;
            
        case ID_SET_VT_CUSTOM_PHOTO_DEL:
            {
                uint16 list_index = GUILIST_GetCurItemIndex(MMISET_CALL_VT_REPLACE_IMAGE_LIST_CTRL_ID);
                MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
                
                //从nv中获得替代文件信息
                MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
                s_replace_file_info[list_index].is_valid = FALSE;
                s_replace_file_info[list_index].file_name_len = 0;
                s_replace_file_info[list_index].file_size = 0;
                SCI_MEMSET(s_replace_file_info[list_index].file_name, 0 , sizeof(s_replace_file_info[list_index].file_name));
                MMISET_WriteReplaceInfoToNv((MMISET_VT_REPLACE_FILE_INFO_T*)&s_replace_file_info);
                MMK_CloseWin(win_id);
            }
            
            break;
            
        default:
            break;
        }
        break;
        
        case MSG_MULTIM_WAITING:
            {
                MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_PICTURE_JPG,0,0,0,PNULL,0,0};
                uint16 list_index = GUILIST_GetCurItemIndex(MMISET_CALL_VT_REPLACE_IMAGE_LIST_CTRL_ID);
                MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
                
                //从nv中获得替代文件信息
                MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
                MMK_CloseWin(win_id);
                MMIPUB_CloseAlertWin();
                select_info.win_id = MMISET_SET_VT_REPLACE_IMAGE_WIN_ID;
                select_info.ticked_file_wstr = s_replace_file_info[list_index].file_name;
                select_info.ticked_file_len = s_replace_file_info[list_index].file_name_len;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
            }
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);      
            break;
            
        case MSG_CLOSE_WINDOW:
            //        MMK_FreeWinAddData(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : MMIAPISET_OpenVTSeting
//  Global resource dependence : 
//  Author: samboo
//  Note:
/*****************************************************************************/
#ifdef VT_SUPPORT
PUBLIC void MMIAPISET_OpenVTSeting(void)
{
    if(MMK_IsOpenWin(MMISET_SET_VT_SETTINGS_WIN_ID) )
    {
        MMK_CloseWin(MMISET_SET_VT_SETTINGS_WIN_ID);
    }
    MMK_CreateWin((uint32*)MMISET_SET_VT_WIN_TAB, PNULL);
}
#endif
/*****************************************************************************/
//  Description : 设置VT录制位置
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTRecordPathSetWinMsg(
                                               MMI_WIN_ID_T        win_id,
                                               MMI_MESSAGE_ID_E    msg_id,
                                               DPARAM              param
                                               )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    const MMI_CTRL_ID_T rv_da_dl_id = MMISET_VT_RECORD_RV_DA_DROPDOWN_CTRL_ID;
    const MMI_CTRL_ID_T rv_dl_id = MMISET_VT_RECORD_RV_DROPDOWN_CTRL_ID;
    const MMI_CTRL_ID_T da_dl_id = MMISET_VT_RECORD_DA_DROPDOWN_CTRL_ID;
    GUI_RECT_T          dropdownlist_rect1 = {MMISET_VT_RECORD_RV_DA_DL_LEFT,MMISET_VT_RECORD_RV_DA_DL_TOP,MMISET_VT_RECORD_RV_DA_DL_RIGHT,MMISET_VT_RECORD_RV_DA_DL_BOTTOM};
    GUI_RECT_T          dropdownlist_rect2 = {MMISET_VT_RECORD_RV_DL_LEFT,MMISET_VT_RECORD_RV_DL_TOP,MMISET_VT_RECORD_RV_DL_RIGHT,MMISET_VT_RECORD_RV_DL_BOTTOM};
    GUI_RECT_T          dropdownlist_rect3 = {MMISET_VT_RECORD_DA_DL_LEFT,MMISET_VT_RECORD_DA_DL_TOP,MMISET_VT_RECORD_DA_DL_RIGHT,MMISET_VT_RECORD_DA_DL_BOTTOM};
    static MMIFILE_DEVICE_E s_set_device_arr[MMI_DEVICE_NUM] = {0};
    uint16 cur_rvda_ind = 0;
    uint16 cur_da_ind = 0;
    uint16 cur_rv_ind = 0;
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMISET_VT_RECORD_RV_DA_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMISET_VT_RECORD_RV_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMISET_VT_RECORD_DA_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        GUIDROPDOWNLIST_SetRect(rv_da_dl_id,&dropdownlist_rect1);
        GUIDROPDOWNLIST_SetRect(rv_dl_id,&dropdownlist_rect2);
        GUIDROPDOWNLIST_SetRect(da_dl_id,&dropdownlist_rect3);
        {
            GUI_RECT_T lab_rect = {MMISET_VT_RECORD_RV_DA_LABEL_LEFT,MMISET_VT_RECORD_RV_DA_LABEL_TOP,MMISET_VT_RECORD_RV_DA_LABEL_RIGHT,MMISET_VT_RECORD_RV_DA_LABEL_BOTTOM};
            GUILABEL_SetRect(MMISET_VT_RECORD_RV_DA_LABEL_CTRL_ID, &lab_rect, FALSE);
        }
        {
            GUI_RECT_T lab_rect = {MMISET_VT_RECORD_RV_LABEL_LEFT,MMISET_VT_RECORD_RV_LABEL_TOP,MMISET_VT_RECORD_RV_LABEL_RIGHT,MMISET_VT_RECORD_RV_LABEL_BOTTOM};
            GUILABEL_SetRect(MMISET_VT_RECORD_RV_LABEL_CTRL_ID, &lab_rect, FALSE);
        }        
        {
            GUI_RECT_T lab_rect = {MMISET_VT_RECORD_DA_LABEL_LEFT,MMISET_VT_RECORD_DA_LABEL_TOP,MMISET_VT_RECORD_DA_LABEL_RIGHT,MMISET_VT_RECORD_DA_LABEL_BOTTOM};
            GUILABEL_SetRect(MMISET_VT_RECORD_DA_LABEL_CTRL_ID, &lab_rect, FALSE);
        } 
        
        SetVTRecordPathSettings(s_set_device_arr);
        MMK_SetAtvCtrl(win_id,rv_da_dl_id);
        break;
        
    case MSG_FULL_PAINT:
        
        break;
            
    case MSG_APP_OK:
    case MSG_CTL_OK:
           
        cur_rvda_ind = GUIDROPDOWNLIST_GetCurItemIndex(MMISET_VT_RECORD_RV_DA_DROPDOWN_CTRL_ID);
        cur_da_ind = GUIDROPDOWNLIST_GetCurItemIndex(MMISET_VT_RECORD_DA_DROPDOWN_CTRL_ID);
        cur_rv_ind = GUIDROPDOWNLIST_GetCurItemIndex(MMISET_VT_RECORD_RV_DROPDOWN_CTRL_ID);
        if(cur_rvda_ind < MMI_DEVICE_NUM && cur_da_ind < MMI_DEVICE_NUM && cur_rv_ind < MMI_DEVICE_NUM)
        {
            SaveVTRecordPathSettings(s_set_device_arr[cur_rvda_ind], s_set_device_arr[cur_da_ind], s_set_device_arr[cur_rv_ind]);
        }
        {
            
            //MMIPUB_OpenAlertSuccessWin( TXT_SET_SUCCESS);
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
//  Description : 初始化VT录制位置设置中的控件
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetVTRecordPathSettings(MMIFILE_DEVICE_E *device_table_ptr)
{
    uint16 rvda_cur_index = 0;
    uint16 da_cur_index = 0;
    uint16 rv_cur_index = 0;
    uint32 i = 0;
    //MMI_TEXT_ID_T record_path_text[MMISET_VT_RECORD_DL_NUM] = {TXT_COMMON_UDISK, TXT_COMMON_SD_CARD};
    MMI_STRING_T  item[MMI_DEVICE_NUM] ={0};
    uint32 record_item_num = 0;
    //MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_SDCARD; 
    MMIFILE_DEVICE_E rvda_user_dev = MMI_DEVICE_SDCARD;
    MMIFILE_DEVICE_E da_user_dev = MMI_DEVICE_SDCARD;
    MMIFILE_DEVICE_E rv_user_dev = MMI_DEVICE_SDCARD;
    
//    if(MMIAPISD_IsSDExist(&fmm_dev))
//    {
//        record_item_num = MMISET_VT_RECORD_DL_NUM;
//    }
//    else
//    {
//        record_item_num = 1;
//     }
    
    GUILABEL_SetTextById(MMISET_VT_RECORD_RV_DA_LABEL_CTRL_ID,TXT_CONECTED_MENU_RECORD_EX02_DA,FALSE);
    GUILABEL_SetTextById(MMISET_VT_RECORD_DA_LABEL_CTRL_ID,TXT_CONECTED_MENU_RECORD_DA,FALSE);
    GUILABEL_SetTextById(MMISET_VT_RECORD_RV_LABEL_CTRL_ID,TXT_CONECTED_MENU_RECORD_EX02,FALSE);
    
    //get effect item
    rvda_user_dev = MMIAPISET_GetVTRecordRVDAPath();
    da_user_dev = MMIAPISET_GetVTRecordDAPath();
    rv_user_dev = MMIAPISET_GetVTRecordRVPath();
    {
        for (i = MMI_DEVICE_UDISK; i< MMI_DEVICE_NUM; i++)
        {
            if(MMIAPIFMM_GetDeviceTypeStatus(i))
            {
                MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(i),&item[record_item_num]);
                if(device_table_ptr != PNULL)
                {
                      device_table_ptr[record_item_num] = i;
                }
                if(rvda_user_dev == i)
                {
                    rvda_cur_index = record_item_num;
                }
                if(da_user_dev == i)
                {
                    da_cur_index = record_item_num;
                }
                if(rv_user_dev == i)
                {
                    rv_cur_index = record_item_num;
                }
                record_item_num++;
            }
        }
    }
    
    
    //rv&da
    //set effect drop down list
    GUIDROPDOWNLIST_AppendItemArray(MMISET_VT_RECORD_RV_DA_DROPDOWN_CTRL_ID,item,record_item_num);
    
    //da
    //set effect drop down list
    GUIDROPDOWNLIST_AppendItemArray(MMISET_VT_RECORD_DA_DROPDOWN_CTRL_ID,item,record_item_num);
    
    //rv
    //set effect drop down list
    GUIDROPDOWNLIST_AppendItemArray(MMISET_VT_RECORD_RV_DROPDOWN_CTRL_ID,item,record_item_num);
    
    GUIDROPDOWNLIST_SetCurItemIndex(MMISET_VT_RECORD_RV_DA_DROPDOWN_CTRL_ID,rvda_cur_index);
    GUIDROPDOWNLIST_SetCurItemIndex(MMISET_VT_RECORD_DA_DROPDOWN_CTRL_ID,da_cur_index);
    GUIDROPDOWNLIST_SetCurItemIndex(MMISET_VT_RECORD_RV_DROPDOWN_CTRL_ID,rv_cur_index);

#if 1
    if(!MMIAPIFMM_GetDeviceTypeStatus(rvda_user_dev))
    {
        if(device_table_ptr != PNULL)
        {
            MMISET_SetVTRecordRVDAPath(device_table_ptr[rvda_cur_index]);
        }
    }
    if(!MMIAPIFMM_GetDeviceTypeStatus(da_user_dev))
    {
        if(device_table_ptr != PNULL)
        {
            MMISET_SetVTRecordRVDAPath(device_table_ptr[da_cur_index]);
        }
    }
    if(!MMIAPIFMM_GetDeviceTypeStatus(rv_user_dev))
    {
        if(device_table_ptr != PNULL)
        {
            MMISET_SetVTRecordRVDAPath(device_table_ptr[rv_cur_index]);
        }
    }
#else
    if(!MMIAPIFMM_GetDeviceTypeStatus(user_dev))
    {
        if(device_table_ptr != PNULL)
        {
            SaveVTRecordPathSettings(device_table_ptr[cur_index], device_table_ptr[cur_index], device_table_ptr[cur_index]);
        }
    }
#endif
}

/*****************************************************************************/
//  Description : 初始化VT录制位置设置中的控件
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SaveVTRecordPathSettings(MMIFILE_DEVICE_E rvda_device, 
                                    MMIFILE_DEVICE_E da_device,
                                    MMIFILE_DEVICE_E rv_device)
{
    MMISET_SetVTRecordRVDAPath(rvda_device);
    MMISET_SetVTRecordDAPath(da_device);
    MMISET_SetVTRecordRVPath(rv_device);
}

/*****************************************************************************/
//  Description : to handle vt one time ring setting win
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTOneTimeRingSettingsWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_VT_ONE_TIME_RING_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        AppendOneTimeRingListItem(ctrl_id);/*lint !e64*/
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        SetOneTimeRing(ctrl_id);/*lint !e64*/
        MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
        MMK_CloseWin( win_id );//bug337284
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

/*****************************************************************************/
//  Description : AppendOneTimeRingListItem
//  Global resource dependence : 
//  Author: samboo
//  Note:
/*****************************************************************************/ 
LOCAL void AppendOneTimeRingListItem(MMI_CTRL_ID_T ctrl_id)
{
    uint16 cur_selection = 0;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get current line
    cur_selection = (uint16)MMIAPISET_GetVTOneTimeRingOption();
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

}  

/*****************************************************************************/
//  Description : SetOneTimeRing
//  Global resource dependence : 
//  Author: samboo
//  Note:
/*****************************************************************************/
LOCAL void SetOneTimeRing(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN cur_selection = (BOOLEAN)GUILIST_GetCurItemIndex(ctrl_id);
    
    MMINV_WRITE(MMINV_SET_VT_ONE_TIME_RING_OPTION, &cur_selection);

    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
}
#if 1//bug337002
PUBLIC void SetRemoteShutdownCameraParam(void)
{
    uint16 cur_index = 0;
    MMI_TEXT_ID_T remote_shutdown_camera_text[MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM] = {TXT_SET_VT_DISPLAY_REMOTE_REPLACE_IMAGE, TXT_SET_VT_DISPLAY_REMOTE_SENDING_IMAGE};
    MMI_STRING_T  remote_shutdown_camera_item ={0};

    GUIFORM_SetStyle(MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
    //remote shut down camera option
    GUILABEL_SetTextById(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID,TXT_COMM_REMOTE_CAMERA_OFF,FALSE);

    cur_index = (uint16)MMIAPISET_GetVTRemoteShutdownCameraOption();
    //GUILABEL_SetTextById(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID,remote_shutdown_camera_text[cur_index],FALSE);
    MMI_GetLabelTextByLang(remote_shutdown_camera_text[cur_index],&remote_shutdown_camera_item);
    GUILABEL_SetText(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID, &remote_shutdown_camera_item, FALSE);
}
#endif

/*****************************************************************************/
//  Description : 设置VT本地视频位置
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTLocalVideoSetWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id,
                                           DPARAM              param
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //GUI_RECT_T              client_rect = MMI_CLIENT_RECT;
    //LOCAL MMI_CTRL_ID_T s_active_ctrl_id = 0;
#if 1//bug337002
    MMI_CTRL_ID_T ctrl_id = MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID;
#else
    const MMI_CTRL_ID_T label_ctrl_id = MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID;
    const MMI_CTRL_ID_T dropdownlist_ctrl_id = MMISET_VT_REMOTE_SHUTDOWN_CAMERA_DROPDOWN_CTRL_ID;
    GUI_RECT_T label_rect = {MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_LEFT,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_TOP,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_RIGHT,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_BOTTON};
    GUI_RECT_T dropdownlist_rect = {MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_LEFT,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_TOP,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_RIGHT,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_BOTTOM};
#endif

    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
#if 1//bug337002
#if !defined(MMI_GUI_STYLE_TYPICAL) 
        GUIFORM_SetType(MMISET_VT_LOCAL_SETTING_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
        SetRemoteShutdownCameraParam();
        MMK_SetAtvCtrl(win_id,MMISET_VT_LOCAL_SETTING_FORM_CTRL_ID);
#else
        GUIFORM_SetStyle(MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUILABEL_SetRect(label_ctrl_id, &label_rect, FALSE);
        GUIDROPDOWNLIST_SetRect(dropdownlist_ctrl_id, &dropdownlist_rect);
        SetVTLocalVideoSettings();
        MMK_SetAtvCtrl(win_id,dropdownlist_ctrl_id);
#endif
        break;

    case MSG_FULL_PAINT:

        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
#if 1//bug337002
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (MSG_CTL_PENOK == msg_id)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        else
        {
            ctrl_id= MMK_GetActiveCtrlId(win_id);
        }
        
        switch (ctrl_id)
        {
        case MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID:
        case MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID:
            MMISETAPI_CreateRemoteShutdownCameraSetWin();
            break;
        default:
            SCI_TRACE_LOW("HandleVTLocalVideoSetWinMsg ctrl_id = %d",ctrl_id);
            break;

        }

#else
        SaveVTLocalVideoSettings();
        {

            //MMIPUB_OpenAlertSuccessWin( TXT_SET_SUCCESS);
        }

        MMK_CloseWin(win_id);
#endif
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

#if 0//bug337002
/*****************************************************************************/
//  Description : 初始化VT本地视频设置中的控件
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void SetVTLocalVideoSettings(void)
{
    uint16 cur_index = 0;
    uint32 i = 0;
    MMI_TEXT_ID_T remote_shutdown_camera_text[MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM] = {TXT_SET_VT_DISPLAY_REMOTE_REPLACE_IMAGE, TXT_SET_VT_DISPLAY_REMOTE_SENDING_IMAGE};
    MMI_STRING_T  remote_shutdown_camera_item[MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM] ={0};
    uint32 remote_shutdown_camera_num = MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM;

    //remote shut down camera option
    GUILABEL_SetTextById(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID,TXT_COMM_REMOTE_CAMERA_OFF,FALSE);

    for (i=0; i<remote_shutdown_camera_num; i++)
    {
        MMI_GetLabelTextByLang(remote_shutdown_camera_text[i],&remote_shutdown_camera_item[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_DROPDOWN_CTRL_ID,remote_shutdown_camera_item,remote_shutdown_camera_num);
    
    cur_index = (uint16)MMIAPISET_GetVTRemoteShutdownCameraOption();
    GUIDROPDOWNLIST_SetCurItemIndex(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_DROPDOWN_CTRL_ID,cur_index);
}

/*****************************************************************************/
//  Description : 初始化VT本地视频设置中的控件
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void SaveVTLocalVideoSettings(void)
{
    uint16 cur_ind = 0;

    cur_ind = GUIDROPDOWNLIST_GetCurItemIndex(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_DROPDOWN_CTRL_ID);
    MMIAPISET_SetVTRemoteShutdownCameraOption((MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION)cur_ind);
}
#endif
#if 1//bug337002
/*****************************************************************************/
//  Description : to handle set Local Video
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTLocalVideoSetSelectWinMsg(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T remote_shutdown_camera_text[MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM] = {TXT_SET_VT_DISPLAY_REMOTE_REPLACE_IMAGE, TXT_SET_VT_DISPLAY_REMOTE_SENDING_IMAGE};
    MMI_CTRL_ID_T ctrl_id = MMISET_VT_REMOTE_SHUTDOWN_CAMERA_SET_CTRL_ID;
    uint16 cur_index = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, 2, FALSE );
        MMIAPISET_AppendListItemByTextIdExt(remote_shutdown_camera_text[0],TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO  );
        MMIAPISET_AppendListItemByTextIdExt(remote_shutdown_camera_text[1],TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO  );
        MMK_SetAtvCtrl(win_id,ctrl_id);   
        break;
        
    case MSG_FULL_PAINT:
        cur_index = (uint16)MMIAPISET_GetVTRemoteShutdownCameraOption();
        GUILIST_SetSelectedItem( ctrl_id, cur_index, TRUE );      
        GUILIST_SetCurItemIndex(ctrl_id,cur_index);
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);     
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        cur_index = (uint16)GUILIST_GetCurItemIndex(ctrl_id);
        MMIAPISET_SetVTRemoteShutdownCameraOption((MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION)cur_index);

        GUILABEL_SetTextById(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID,remote_shutdown_camera_text[cur_index],FALSE);

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
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
//  Description : create screensaver window
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void MMISETAPI_CreateRemoteShutdownCameraSetWin(void)
{
    MMK_CreatePubListWin((uint32*)MMISET_SET_REMOTESHUTDOWNCAMERASET_WIN_TAB,PNULL);
}
#endif

#endif //end VT_SUPPORT
/*****************************************************************************/
//  Description : to select network method
//  Global resource dependence : s_cur_dual_sys
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectNetworkMethodWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    )
{
    MMI_RESULT_E                recode              =   MMI_RESULT_TRUE;
#ifndef _WIN32 /* lint_lai */
     MN_GMMREG_RAT_E            rat                 =   MN_GMMREG_RAT_GPRS;
    MN_PLMN_T                   select_plmn         =   {0,0,2};
#endif 
    MN_DUAL_SYS_E           dual_sys   = (uint32)MMK_GetWinAddDataPtr(win_id);
    uint16                          cur_selection       =   0;
    uint16 no_seltction = 0;
    MN_PHONE_SELECT_PLMN_T sel_net_mode ={MN_PHONE_PLMN_SELECT_MANUAL_MODE, 0};/*lint !e785*/
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_NETWORK_METHOD_CTRL_ID,MMISET_NETWORK_SEL_MAX_ITEM, FALSE );//max item 2
        MMISET_AppendSelectNetworkListItem(dual_sys,MMISET_SET_NETWORK_METHOD_CTRL_ID);/*lint !e64*/

#if 0//ndef MMI_SMART_DUAL_SIM_SUPPORT
        GUILIST_SetOwnSofterKey( MMISET_SET_NETWORK_METHOD_CTRL_ID, TRUE );

        MMK_SetAtvCtrl(win_id, MMISET_SET_NETWORK_METHOD_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
#else
        MMK_SetAtvCtrl(win_id, MMISET_SET_NETWORK_METHOD_CTRL_ID);
#endif
        break;

    case MSG_GET_FOCUS:
        MNPHONE_GetPlmnSelectModeEx(dual_sys, &sel_net_mode);/*lint !e64*/
        switch (sel_net_mode.select_mode)
        {
        case MN_PHONE_PLMN_SELECT_AUTO_MODE:
            cur_selection = 0;
            no_seltction = 1;
            break;
            
        case MN_PHONE_PLMN_SELECT_MANUAL_MODE:
            cur_selection = 1;
            no_seltction = 0;
            break;
            
        default:
            //SCI_TRACE_LOW:"HandleSelectNetworkMethodWindow: select network mode is %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6687_112_2_18_2_50_29_78,(uint8*)"d",sel_net_mode.select_mode);
            break;
        }

        GUILIST_SetSelectedItem( MMISET_SET_NETWORK_METHOD_CTRL_ID, no_seltction, FALSE );
        GUILIST_SetSelectedItem( MMISET_SET_NETWORK_METHOD_CTRL_ID, cur_selection, TRUE );
        break;
 	case MSG_CTL_OK:
 	case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_NETWORK_METHOD_CTRL_ID);
        switch (cur_selection)
        {
            //自动选网
        case 0:
#ifndef _WIN32

            if(SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys))
            {
                MMIPUB_OpenAlertWarningWin(TXT_PUK_BLOCKED);
                return;
            }
            if (MMIAPIPHONE_IsNetworkSelecting(dual_sys) ||(MMICC_GetCallNum()>0) 
                ||(MMIAPIPDP_PsIsPdpLinkExist() && !MNSS_CheckTestSim(dual_sys))
                ||(!MMIAPIPHONE_GetSimExistedStatus(dual_sys)) ||(!(e_dualsys_setting & (0x01 << dual_sys)))
                ||(SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)))
            {
                MMIPUB_OpenAlertWarningWin(STR_SIM_SEARCH_FAIL);
                
                break;
            }
#ifdef MMI_2G_4G_SUPPORT
        rat = MNPHONE_GetCurrentRAT(dual_sys);
        MMIAPIPHONE_GetCurPLMN(&select_plmn, dual_sys);
        SCI_TraceLow("HandleSelectNetworkMethodWindow == rat =%d ",rat);
        if (MN_RETURN_SUCCESS != MNPHONE_SelectPLMNEx(dual_sys,FALSE,select_plmn,rat))
        {
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6717_112_2_18_2_50_29_79,(uint8*)"");
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                break;
        }

#else
            rat = MNPHONE_GetCurrentRATEx(dual_sys);
            MMIAPIPHONE_GetCurPLMN(&select_plmn, dual_sys);
            if (MN_RETURN_SUCCESS != MNPHONE_SelectPLMNEx(dual_sys,FALSE,select_plmn,rat))
            {
                //SCI_TRACE_LOW:"HandleSelectNetworkMethodWindow: MNPHONE_SelectPLMN fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6717_112_2_18_2_50_29_79,(uint8*)"");
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                break;
            }
#endif            
            //进入网络连接界面
            MMISET_OpenNetworkConnectWin(dual_sys,
                                                                    MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                                                    TXT_NETWORK_CONNECT,
                                                                    MMIPUB_SOFTKEY_NONE,
                                                                    MMISET_HandleSelNetworkConnectWindow);
#else
            GUILIST_SetCurItemIndex(MMISET_SET_NETWORK_METHOD_CTRL_ID, cur_selection);
            MMK_CloseWin(win_id);
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif 
            break;
            
            //手动选网
        case 1:
#ifndef _WIN32
            if(SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys))
            {
                MMIPUB_OpenAlertWarningWin(TXT_PUK_BLOCKED);
                return;
            }
            if (MMIAPIPHONE_IsNetworkSelecting(dual_sys) ||(MMICC_GetCallNum()>0) 
                ||(MMIAPIPDP_PsIsPdpLinkExist() && !MNSS_CheckTestSim(dual_sys))
                ||(!MMIAPIPHONE_GetSimExistedStatus(dual_sys)) ||(!(e_dualsys_setting & (0x01 << dual_sys)))
                ||(SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)))
            {
                MMIPUB_OpenAlertWarningWin(STR_SIM_SEARCH_FAIL);
                
                break;
            }
        //the list of all plmn
#ifdef MMI_2G_4G_SUPPORT
        if (MN_RETURN_SUCCESS != MNPHONE_ListPLMNEx(dual_sys, MN_GMMREG_RAT_4G))//@zhaohui,cr106675
        {
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6748_112_2_18_2_50_29_80,(uint8*)"");
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            break;
        }
#else
//            s_current_sel_rat = MNPHONE_GetConfiguredRAT();/*lint !e64*/  //cr149167
            if (MN_RETURN_SUCCESS != MNPHONE_ListPLMNEx(dual_sys,MN_GMMREG_RAT_GPRS_AND_3G))//@zhaohui,cr106675
            {
                //SCI_TRACE_LOW:"HandleSelectNetworkMethodWindow: MNPHONE_ListPLMN fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6748_112_2_18_2_50_29_80,(uint8*)"");
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                break;
            }
            
            //@fen.xie MS00174570:保存手动搜网前的plmn status
            s_plmn_status_before_manu_search = MMIAPIPHONE_GetPlmnStatus(dual_sys);
            
            //MNPHONE_SelectPLMN(TRUE,select_plmn,MN_GMMREG_RAT_GPRS);  
#endif			
            //进入网络连接界面
            MMISET_OpenNetworkConnectWin(dual_sys,
                                                                    MMISET_SET_SEARCH_NETWORK_WIN_ID,
                                                                    TXT_NETWORK_CONNECT,
                                                                    MMIPUB_SOFTKEY_ONE,
                                                                    MMISET_HandleQuitSelNetworkConnectWindow);
#else
			MMISET_OpenNetworkConnectWin(dual_sys,
				MMISET_SET_SEARCH_NETWORK_WIN_ID,
				TXT_NETWORK_CONNECT,
				MMIPUB_SOFTKEY_ONE,
				MMISET_HandleQuitSelNetworkConnectWindow);

            GUILIST_SetCurItemIndex(MMISET_SET_NETWORK_METHOD_CTRL_ID, cur_selection);
            MMK_CloseWin(win_id);
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            break;
            
        default:
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
#if 0//def MMI_SMART_DUAL_SIM_SUPPORT
        case MSG_FULL_PAINT:
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_HDR_NETWORK_SELECTION_EXT01))
            {
                GUIWIN_UpdateStb();
            }
            break;
#endif            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : 判断是否满足提示"切换双模后再试"搜网的情况
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
BOOLEAN MMIAPISET_IsPromptRetry(void)
{
    NETWORK_TYPE_E network_type = GetNetworkType();
    
    if (MMK_IsOpenWin(MMISET_SET_NETWORK_METHOD_WIN_ID))
    {
        uint16                          cur_selection       =   0;
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_NETWORK_METHOD_CTRL_ID);
        
        if((PLMN_NO_SERVICE == s_plmn_status_before_manu_search) /*手动搜网前已经是"无网络"*/
            && ((NETWORK_TYPE_TD_E == network_type) || (NETWORK_TYPE_GPRS_E == network_type))//单模下
            && (1 == cur_selection)//手动搜网
            )
        {
            return TRUE;
        }
    }
    
    return FALSE;
}
#endif

/*****************************************************************************/
//  Description : 处理自动重拨
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAutoRedialWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E    recode          =   MMI_RESULT_TRUE;
    uint16          cur_selection   =   0;
    BOOLEAN         is_open         =   FALSE;
    MMI_CTRL_ID_T   ctrl_id         =   MMISET_SET_AUTO_REDIAL_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_AUTO_REDIAL_CTRL_ID, MMISET_AUTODIAL_MAX_ITEM, FALSE );//max item 3
        
        //add items to listbox
        MMIAPISET_AppendListItemByTextId(TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        //MMIAPISET_AppendListItemByTextId(TXT_SET_DELETE_BLACKLIST, STXT_SOFTKEY_OK_MK, ctrl_id);
        
        if (MMIAPISET_GetIsAutoRedail())
        {
            cur_selection = 0;
        }
        else
        {
            cur_selection = 1;
        }
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_AUTO_REDIAL_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if !defined(MMI_PDA_SUPPORT) 
    case MSG_CTL_OK:
#endif
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //get the selected item
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        
       // SCI_ASSERT((MMISET_AUTODIAL_MAX_ITEM > cur_selection));
     if(MMISET_AUTODIAL_MAX_ITEM <= cur_selection)
     {
         //SCI_TRACE_LOW:"mmisetting_assert HandleSetAutoRedialWindow cur_selection %d"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6869_112_2_18_2_50_29_81,(uint8*)"d", cur_selection);
         break;
     }         
        switch (cur_selection)
        {
        case 0:
            is_open = TRUE;
            break;
            
        case 1:
            is_open = FALSE;
            break;
            //case 2:
            //    MMIAPICC_ResetBlacklist();
            //    break;
            
        default:
            break;
        }
        MMIAPISET_SetIsAutoRedial(is_open);
        //success!
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        
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

LOCAL  uint32   s_set_speeddial_delay;
#define MMISET_SPEDDIAL_DELAY_NUM   9
/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
LOCAL void MMISET_AppendSpeedDialDelayListItem(
                                               MMI_CTRL_ID_T        ctrl_id
                                               )
{
    uint32      cur_selection=0;
    
    MN_RETURN_RESULT_E          return_val    =   MN_RETURN_FAILURE;
    MMINV_READ(MMINV_SET_SPEEDIAL_DELAY_TIME, &cur_selection, return_val);

    //add items to listbox
    
    MMIAPISET_AppendListItemByTextId(TXT_CLOSE, STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_1_SECOND,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_2_SECONDS,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_3_SECONDS,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_SET_SPEED_DIAL_TIME_4S,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_5_SECONDS,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_6_SECONDS,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_SET_SPEED_DIAL_TIME_7S,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_SET_SPEED_DIAL_TIME_8S,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection/1000, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection/1000);
    
}

/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
PUBLIC void MMISET_SetSpeedDialDelay(uint32 time)
{
    s_set_speeddial_delay = time;
}

/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
PUBLIC uint32 MMIAPISET_GetSpeedDialDelay()
{
    return s_set_speeddial_delay;
}


/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
LOCAL void SetSpeedDialDelay(
                              MMI_CTRL_ID_T     ctrl_id
                              )
{
    uint16   cur_selection = 0;
    uint32   delay_time[] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000};
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    
    //SCI_ASSERT((0 <= cur_selection) && (MMISET_SPEDDIAL_DELAY_NUM > cur_selection));
   if(MMISET_SPEDDIAL_DELAY_NUM <= cur_selection)
   {
        //SCI_TRACE_LOW:"mmisetting_assert SetSpeedDialDelay cur_selection %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6974_112_2_18_2_50_30_82,(uint8*)"d", cur_selection);
        return;
    }     
    s_set_speeddial_delay = delay_time[cur_selection];

    //  SetBackLightInfo(backlight_info);
    MMINV_WRITE(MMINV_SET_SPEEDIAL_DELAY_TIME,&s_set_speeddial_delay);
}

/*****************************************************************************/
//  Description : 延时拨号处理
//  Global resource dependence : 
//  Author:yiwen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSpeedDialWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_SPEED_DIAL_CTRL_ID,MMISET_SPEDDIAL_DELAY_NUM, FALSE );//max item 2
        MMISET_AppendSpeedDialDelayListItem(MMISET_SET_SPEED_DIAL_CTRL_ID);
        MMK_SetAtvCtrl(win_id,MMISET_SET_SPEED_DIAL_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        SetSpeedDialDelay(MMISET_SET_SPEED_DIAL_CTRL_ID);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        break;
                
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
                

    default:
        recode = MMI_RESULT_FALSE;
        break;
}

    return recode;
}

#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description : SetVoiceReportParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetVoiceReportParam(void)
{
    
    uint32 loop = 0;
    uint16 cur_selection = 0;
    BOOLEAN                     is_tts_open = FALSE;
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
    TXT_OPEN,
    TXT_CLOSE,
    };

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_VOICE_REPORT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_CALL_TTS, &label_name);
    GUILABEL_SetText(MMISET_VOICE_REPORT_LABEL_CTRL_ID, &label_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_VOICE_REPORT_EDIT_CTRL_ID, text_id[loop]);
    }

    //get current tts setting info
    is_tts_open = MMIAPISET_GetTTSInfo();
    //the status is open
    if (is_tts_open)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
     GUISETLIST_SetTitleTextId(MMISET_VOICE_REPORT_EDIT_CTRL_ID,TXT_SET_CALL_TTS);
    GUISETLIST_SetCurIndex(MMISET_VOICE_REPORT_EDIT_CTRL_ID, cur_selection);

    GUISETLIST_SetCtrlState(MMISET_VOICE_REPORT_EDIT_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//for cr226574

}
#endif

#ifdef TTS_SUPPORT 
/*****************************************************************************/
//  Description : SaveVoiceReportParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SaveVoiceReportParam(void)
{
    uint16      cur_selection = 0;
    BOOLEAN     is_tts_open = FALSE;
    
    //get the selected item
    cur_selection = GUISETLIST_GetCurIndex(MMISET_VOICE_REPORT_EDIT_CTRL_ID);
    
    // set the global virable
    if ( 0 == cur_selection)
    {
        is_tts_open = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPISET_SetTTSInfo cur_selection = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_7090_112_2_18_2_50_30_83,(uint8*)"d",cur_selection);
        is_tts_open = FALSE;
    }
    
    //write the status of TTS into NV
    MMINV_WRITE(MMINV_SET_TTS_ON_OFF,&is_tts_open);
}
#endif

/*****************************************************************************/
//  Description : SaveVibrateTipParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SaveVibrateTipParam(void)
{
    uint16      cur_selection   =   0;
    BOOLEAN     is_open         =   FALSE;
    
    //get the selected item
    cur_selection = GUISETLIST_GetCurIndex(MMISET_VIBRATE_TIP_EDIT_CTRL_ID);
    
    switch (cur_selection)
    {
    case 0:
        is_open = TRUE;
        break;
        
    case 1:
        is_open = FALSE;
        break;
        
    default:
        break;
    }
    MMISET_SetIsConnectPrompt(is_open);
}

/*****************************************************************************/
//  Description : SaveAuTORedialParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SaveAuTORedialParam(void)
{
    uint16 cur_selection = 0;
    BOOLEAN     is_open         =   FALSE;
    cur_selection = GUISETLIST_GetCurIndex(MMISET_AUTO_CONNECT_EDIT_CTRL_ID);
    
    switch (cur_selection)
    {
    case 0:
        is_open = TRUE;
        break;
        
    case 1:
        is_open = FALSE;
        break;
        
    default:
        break;
    }
    MMIAPISET_SetIsAutoRedial(is_open); 
}

#ifdef MV_SUPPORT 
/*****************************************************************************/
//  Description : SaveMagicParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SaveMagicParam(void)
{
    uint16 cur_selection = 0;
    cur_selection = GUISETLIST_GetCurIndex(MMISET_MAGIC_VOICE_LIST_CTRL_ID);
    if(cur_selection == 0)
    {
     MMIAPISET_SetIsMagicVoice(TRUE);
    }
    else
    {
        MMIAPISET_SetIsMagicVoice(FALSE);
    }
}
#endif




/*****************************************************************************/
//  Description : init call setting other control
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
#ifdef MV_SUPPORT 
LOCAL void SetMagicParam(void)
{
    uint32 loop = 0;
    MMI_STRING_T label_name  = {0};
    uint16 cur_selection = 0;
    uint32 text_id[2]=
    {
    TXT_OPEN,
    TXT_CLOSE,
    };
#if !defined MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_MAGIC_VOICE_LIST_CTRL_ID,ALIGN_LEFT);
#endif

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_CC_MAGICVOICE, &label_name);
    GUILABEL_SetText(MMISET_MAGIC_VOICE_LABEL_CTRL_ID, &label_name, FALSE);

#if !defined MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_MAGIC_VOICE_LIST_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
    MMI_GetLabelTextByLang(TXT_CC_MAGICVOICE_PARAM_SET, &label_name);
    GUILABEL_SetText(MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID, &label_name, FALSE);
        
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_MAGIC_VOICE_LIST_CTRL_ID, text_id[loop]);
    }
    
#if !defined(MMI_PDA_SUPPORT) 
    if(MMIAPISET_GetIsMagicVoice())
    {
        cur_selection = 0;
        GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
                MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID,
                GUIFORM_CHILD_DISP_NORMAL
                );  
    }
    else
    {
        cur_selection = 1;
        GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
                MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID,
                GUIFORM_CHILD_DISP_GRAY
                );          
    }
     GUISETLIST_SetTitleTextId(MMISET_MAGIC_VOICE_LIST_CTRL_ID,TXT_CC_MAGICVOICE_PARAM_SET);
    GUISETLIST_SetCurIndex(MMISET_MAGIC_VOICE_LIST_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_MAGIC_VOICE_LIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//for cr226574
#endif
}
#endif

/*****************************************************************************/
//  Description : SetSetCallOtherLabelParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetSetCallOtherLabelParam(void)
{
    MMI_STRING_T label_name  = {0};
    
    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_IP_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_IP_NUMBER, &label_name);
    GUILABEL_SetText(MMISET_IP_KEY_LABEL_CTRL_ID, &label_name, FALSE);
    
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_ANSWER_MODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_ANYKEY_REJECT_MODE, &label_name);
    GUILABEL_SetText(MMISET_ANSWER_MODE_LABEL_CTRL_ID, &label_name, FALSE);        
#endif
}

/*****************************************************************************/
//  Description : SetAutoRedailParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetAutoRedailParam(void)
{

    uint32 loop = 0;
    uint16 cur_selection = 0;
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
    TXT_OPEN,
    TXT_CLOSE,
    };

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_AUTO_CONNECT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(STXT_CALL_REPEAT, &label_name);
    GUILABEL_SetText(MMISET_AUTO_CONNECT_LABEL_CTRL_ID, &label_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_AUTO_CONNECT_EDIT_CTRL_ID, text_id[loop]);
    }

    if (MMIAPISET_GetIsAutoRedail())
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
     GUISETLIST_SetTitleTextId(MMISET_AUTO_CONNECT_EDIT_CTRL_ID,STXT_CALL_REPEAT);
    GUISETLIST_SetCurIndex(MMISET_AUTO_CONNECT_EDIT_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_AUTO_CONNECT_EDIT_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//for cr226574

}

/*****************************************************************************/
//  Description : SetVibrateTipParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetVibrateTipParam(void)
{
    uint32 loop = 0;
    uint16 cur_selection = 0;
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
    TXT_OPEN,
    TXT_CLOSE,
    };

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_VIBRATE_TIP_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_CONNECT_PROMPT, &label_name);
    GUILABEL_SetText(MMISET_VIBRATE_TIP_LABEL_CTRL_ID, &label_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_VIBRATE_TIP_EDIT_CTRL_ID, text_id[loop]);
    }

    if(MMIAPISET_GetIsConnectPrompt())
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
     GUISETLIST_SetTitleTextId(MMISET_VIBRATE_TIP_EDIT_CTRL_ID,TXT_SET_CONNECT_PROMPT);
    GUISETLIST_SetCurIndex(MMISET_VIBRATE_TIP_EDIT_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_VIBRATE_TIP_EDIT_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//for cr226574
}


/*****************************************************************************/
//  Description : SetMinuteReminderParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetMinuteReminderParam(void)
{
    MMI_STRING_T label_name  = {0};
    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_MINUTE_REMINDER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_CALL_TIME_REMINDER, &label_name);
    GUILABEL_SetText(MMISET_CALL_MINUTE_REMINDER_LABEL_CTRL_ID, &label_name, FALSE);
    switch(MMIAPISET_GetMinuteReminderTime())
    {
    case CC_CALL_REMINDER_TIME_OFF:
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        break;
        
    case CC_CALL_REMINDER_TIME_50S:
        MMI_GetLabelTextByLang(TXT_SET_TIME_50S, &label_name);
        break;
        
    case CC_CALL_REMINDER_TIME_55S:
       MMI_GetLabelTextByLang(TXT_SET_TIME_55S, &label_name);
        break;
        
    case CC_CALL_REMINDER_TIME_60S:
        MMI_GetLabelTextByLang(TXT_SET_TIME_60S, &label_name);
        break;
        
    default :
         MMI_GetLabelTextByLang(TXT_EXPORT_LOC_CUSTOM, &label_name);
        break;
    }
    
    GUILABEL_SetText(MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID, &label_name, FALSE);
}

#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : InitPdaCallOtherSettingsCtrl
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaCallOtherSettingsCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_OTHER_SETTING_CTRL_ID;
    MMI_STRING_T text_str  = {0};
    MMI_STRING_T text_str_null  = {0};
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    MMISET_ANYKEY_REJECT_SET_T  mode_info = {0};
#endif
#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
    BOOLEAN is_fly_mode_on = FALSE;
    BOOLEAN is_flymode_openphone = FALSE;
#endif
    uint8   list_menu_count =5;
    wchar temp_wchar[12 + 1]={0};
    
#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
    list_menu_count = list_menu_count+2;
#endif

#ifdef MMI_AUTO_RECORD_SUPPORT
    list_menu_count = list_menu_count+3;
#endif

#ifdef MMI_IP_CALL_SUPPORT
    list_menu_count += 1;
#endif

    GUILIST_SetMaxItem(ctrl_id, list_menu_count, FALSE );
    
#ifdef MMI_IP_CALL_SUPPORT
    //ip setting
    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_IP_NUMBER,text_str,IMAGE_NULL,MMISET_IP_KEY_LABEL_CTRL_ID);
#endif

    //call time reminder
    switch(MMIAPISET_GetMinuteReminderTime())
    {
    case CC_CALL_REMINDER_TIME_OFF:
        MMI_GetLabelTextByLang(TXT_CLOSE, &text_str);
        break;
        
    case CC_CALL_REMINDER_TIME_50S:
        MMI_GetLabelTextByLang(TXT_SET_TIME_50S, &text_str);
        break;
        
    case CC_CALL_REMINDER_TIME_55S:
       MMI_GetLabelTextByLang(TXT_SET_TIME_55S, &text_str);
        break;
        
    case CC_CALL_REMINDER_TIME_60S:
        MMI_GetLabelTextByLang(TXT_SET_TIME_60S, &text_str);
        break;
        
    default :
        {
            char temp_char[4 + 1] = {0};
            MMI_STRING_T second_str = {0};
            uint32 temp_char_len = 0;
            text_str.wstr_ptr = temp_wchar;
            
            sprintf((char*)temp_char, "%d ", MMIAPISET_GetMinuteReminderTime());
            temp_char_len = strlen((char*)temp_char);
            text_str.wstr_len = MIN((sizeof(temp_wchar)/sizeof(wchar) - 1), temp_char_len);
            MMI_STRNTOWSTR(text_str.wstr_ptr, text_str.wstr_len, (uint8 *)temp_char, text_str.wstr_len, text_str.wstr_len);

            MMI_GetLabelTextByLang(TXT_SECOND, &second_str);

            text_str.wstr_len += MIN((sizeof(temp_wchar)/sizeof(wchar) - text_str.wstr_len - 1), second_str.wstr_len);
            MMIAPICOM_Wstrncat(text_str.wstr_ptr, second_str.wstr_ptr, MIN((sizeof(temp_wchar)/sizeof(wchar) - text_str.wstr_len - 1), second_str.wstr_len));
        }
        break;
    } 
    //AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_CALL_TIME_REMINDER,text_str,IMAGE_NULL,MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID);//delete by James li
  #ifdef ZDT_PRODUCT_Q98
 AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_AUTO_HANDFREE,text_str_null,MMIAPISET_GetAutoHandFreeSetting()?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_CALL_AUTO_HANDFREE_CTRL_ID);
#endif

#ifdef CALL_AUTO_REDIAL
    //auto redial
    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_AUTO_REDAIL,text_str_null,MMIAPISET_GetIsAutoRedail()?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_AUTO_CONNECT_EDIT_CTRL_ID);
#endif

    //vibrate tip
    //AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_CONNECT_PROMPT,text_str_null,MMIAPISET_GetIsConnectPrompt()?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_VIBRATE_TIP_EDIT_CTRL_ID);//delete by James li

    //call reject mode
#if 0
     //pda remove this function
    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_ANYKEY_REJECT_MODE,text_str_null,IMAGE_COMMON_RIGHT_ARROW,MMISET_ANSWER_MODE_LABEL_CTRL_ID);
#endif

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    mode_info = MMIAPISET_GetAnykeyRejectMode();
    AppendPdaSettingsItem2Line(ctrl_id, TXT_SET_SMS_REPLY, text_str_null, mode_info.is_sms_reply_open?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON, MMISET_ANSWER_MODE_LABEL_CTRL_ID);
#endif

#ifdef TTS_SUPPORT
//    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_CALL_TTS,text_str_null,MMIAPISET_GetTTSInfo()?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_VOICE_REPORT_EDIT_CTRL_ID);
#endif

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
    is_fly_mode_on = MMIAPISET_GetFlyMode();

    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_FLY_MODE,text_str_null,is_fly_mode_on?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID);

    is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();

    MMI_GetLabelTextByLang(TXT_SET_SELECT_ON_POWER, &text_str);

    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_FLY_MODE,text_str,is_flymode_openphone?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID);
#endif
 #ifdef MMI_AUTO_RECORD_SUPPORT
     AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_AUTO_RECORD,text_str_null,MMIAPISET_GetAutoRecordSetting()?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_CALL_AUTO_RECORD_CTRL_ID);
 #endif
}
 
/*****************************************************************************/
//  Description : 通话设置中的others的设置窗口
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCallSetOtherWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_OTHER_SETTING_CTRL_ID;
    MMI_MENU_ID_T               menu_id     =   0;
    uint16 index = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            InitPdaCallOtherSettingsCtrl();
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;

    case MSG_GET_FOCUS:
        index = GUILIST_GetCurItemIndex(ctrl_id); 
        GUILIST_RemoveAllItems(ctrl_id);
        InitPdaCallOtherSettingsCtrl();
        GUILIST_SetCurItemIndex(ctrl_id, index); 
        break;
    case MSG_FULL_PAINT:
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &menu_id);

        switch (menu_id)
        {
#ifdef MMI_IP_CALL_SUPPORT
        case MMISET_IP_KEY_LABEL_CTRL_ID:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            {
                uint16 sim_sys = MN_DUAL_SYS_1;
                uint32 sim_num = 0;

                sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);   

                if (0 == sim_num)
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                else if (1 == sim_num)
                {
                    MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)sim_sys);
                }
                else
                {
                    MMIAPISET_OpenUssdSelectWin(MMISET_IP_CALL);
                }
            }
#else
            MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_1);
#endif
            break;
#endif

        case MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID:
            MMK_CreateWin((uint32*)MMISET_SET_MINUTE_REMINDER_WIN_TAB,PNULL);
            break;

        case MMISET_AUTO_CONNECT_EDIT_CTRL_ID:
               {
                MMIAPISET_SetIsAutoRedial((BOOLEAN)!MMIAPISET_GetIsAutoRedail());
                SetCallSetOthersItemCheck(ctrl_id,MMIAPISET_GetIsAutoRedail(),index);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
               break;
            
        case MMISET_VIBRATE_TIP_EDIT_CTRL_ID:
               {
                MMISET_SetIsConnectPrompt((BOOLEAN)!MMIAPISET_GetIsConnectPrompt());
                SetCallSetOthersItemCheck(ctrl_id,MMIAPISET_GetIsConnectPrompt(),index);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
               }
            break;
            
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
        case MMISET_ANSWER_MODE_LABEL_CTRL_ID:
            {
                MMISET_ANYKEY_REJECT_SET_T  mode_info = {0};
                BOOLEAN is_sms_reply_open = FALSE;
                
                mode_info = MMIAPISET_GetAnykeyRejectMode();
                is_sms_reply_open = !(mode_info.is_sms_reply_open);
                SetCallSetOthersItemCheck(ctrl_id, is_sms_reply_open, index);
                mode_info.is_sms_reply_open = mode_info.is_sms_reply_open ? FALSE:TRUE;
                MMINV_WRITE(MMINV_SET_ANYKEY_REJECT_MODE,&mode_info);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            break;
#endif
            
#ifdef MV_SUPPORT
        case MMISET_MAGIC_VOICE_LABEL_CTRL_ID:
            MMIAPISET_SetIsMagicVoice((BOOLEAN)!MMIAPISET_GetIsMagicVoice());
            SetCallSetOthersItemCheck(ctrl_id,MMIAPISET_GetIsMagicVoice(),index);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
            

        case MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID:
            MMIAPICC_OpenMagicVoiceSetMenu();
            break;
#endif
#ifdef TTS_SUPPORT
        case MMISET_VOICE_REPORT_EDIT_CTRL_ID:
            MMISET_SetTTS((BOOLEAN)!MMIAPISET_GetTTSInfo());
            SetCallSetOthersItemCheck(ctrl_id,MMIAPISET_GetTTSInfo(),index);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
#endif

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
        case MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID:
            {
                uint32 i  = 0;
                BOOLEAN is_fly_mode_on = MMIAPISET_GetFlyMode();

                if (!is_fly_mode_on)
                {
                    uint32 dual_sys =0;
                    BOOLEAN is_ps_ready = TRUE;
                    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

                    for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                    {
                        if ((e_dualsys_setting & (0x01 << dual_sys))
                            && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                        {
                            is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);

                            if (!is_ps_ready)
                            {
                                break;
                            }
                        }
                    }

                    if (is_ps_ready)
                    {
                        if (MMISET_GetIsQueryOpenFlyMode())
                        {
                            MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                            break;
                        }
                        else
                        {
                            is_fly_mode_on = TRUE;

                            MMIAPISET_SetFlyMode(is_fly_mode_on);

                            MMIAPIPHONE_DeactivePsAccordingToSetting();
                            SetCallSetOthersItemCheck(ctrl_id,is_fly_mode_on,index);
                            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                        }
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                        
                        break;
                    }
                }
                else
                {
                    BOOLEAN is_ps_deactive = TRUE;

                    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                    {
                        if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
                        {
                            is_ps_deactive = FALSE;
                            break;
                        }
                    }
            
                    if (is_ps_deactive)
                    {
                        is_fly_mode_on = FALSE;
                    
                        MMIAPISET_SetFlyMode(is_fly_mode_on);
                        // 激活协议栈
                        MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
                        MMIAPIMMS_RestartAutodownMMS();
                        SetCallSetOthersItemCheck(ctrl_id,is_fly_mode_on,index);
                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                        break;
                    }
                }
            }
            break;

        case MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID:
            {
                BOOLEAN is_flymode_openphone = FALSE;
                is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
                SetCallSetOthersItemCheck(ctrl_id,!is_flymode_openphone,index);
                is_flymode_openphone = !is_flymode_openphone;
                MMINV_WRITE(MMINV_FLY_MODE_OPEN_PHONE,&is_flymode_openphone);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            break;
#endif
               
#ifdef MMI_AUTO_RECORD_SUPPORT
        case MMISET_CALL_AUTO_RECORD_CTRL_ID:
            MMIAPISET_SetAutoRecordSetting((BOOLEAN)!MMIAPISET_GetAutoRecordSetting());
            SetCallSetOthersItemCheck(ctrl_id,MMIAPISET_GetAutoRecordSetting(),index);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;

#endif
#ifdef ZDT_PRODUCT_Q98
	case MMISET_CALL_AUTO_HANDFREE_CTRL_ID:
            MMIAPISET_SetAutoHandFreeSetting((BOOLEAN)!MMIAPISET_GetAutoHandFreeSetting());
            SetCallSetOthersItemCheck(ctrl_id,MMIAPISET_GetAutoHandFreeSetting(),index);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);

		break;
	#endif	

        default:
            break;

        }
        break;

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
     case MSG_PROMPTWIN_OK:
        {
            BOOLEAN is_fly_mode_on = TRUE;

            MMIPUB_CloseQuerytWin(PNULL);
            
            MMISET_StopPSApplication();
            
            MMIAPISET_SetFlyMode(is_fly_mode_on);
            

#ifdef DCD_SUPPORT                      
            MMIAPIDCD_HandFlyModeSetChange();
#endif
            
            MMIAPIPHONE_DeactivePsAccordingToSetting();
            
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
#endif

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;

    }
    return recode;
}
#else

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
/*****************************************************************************/
//  Description : SetFlyModeParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetFlyModeParam(void)
{
    uint32 loop = 0;
    uint16 cur_selection = 0;
    BOOLEAN is_fly_mode_on = MMIAPISET_GetFlyMode();
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
        TXT_OPEN,
        TXT_CLOSE,
    };

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &label_name);
    GUILABEL_SetText(MMISET_FLY_MODE_SELECT_CTRL_ID, &label_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID, text_id[loop]);
    }

    //the status is open
    if (is_fly_mode_on)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }

    GUISETLIST_SetTitleTextId(MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID,TXT_SET_FLY_MODE);
    GUISETLIST_SetCurIndex(MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID, cur_selection);

    GUISETLIST_SetCtrlState(MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
}

/*****************************************************************************/
//  Description : SetFlyModePowerOnParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetFlyModePowerOnParam(void)
{
    uint32 loop = 0;
    uint16 cur_selection = 0;
    BOOLEAN is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
        TXT_OPEN,
        TXT_CLOSE,
    };

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_POWER_ON_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_SELECT_ON_POWER, &label_name);
    GUILABEL_SetText(MMISET_FLY_MODE_SELECT_ON_POWER_CTRL_ID, &label_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID, text_id[loop]);
    }

    //the status is open
    if (is_flymode_openphone)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }

    GUISETLIST_SetTitleTextId(MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID,TXT_SET_SELECT_ON_POWER);
    GUISETLIST_SetCurIndex(MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID, cur_selection);

    GUISETLIST_SetCtrlState(MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCallSetOtherWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_IP_KEY_LABEL_CTRL_ID;
/*#if !defined(MMI_MULTI_SIM_SYS_SINGLE)    
    MMISET_SIM_SELECT_TYPE_E    select_type =   0;
#endif*/
    
    MMIMAINMENU_TYPE_E    menu_style = MMIAPISET_GetMainmenuStyle();
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetSetCallOtherLabelParam();
        SetMinuteReminderParam();
        SetAutoRedailParam();
        SetVibrateTipParam();
#ifdef MV_SUPPORT 
        if(
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
            MMIMAINMENU_GO_SLIDE_E == menu_style
#endif
#ifdef MENU_SWEETS_SUPPORT
            || MMIMAINMENU_SWEETS_E  == menu_style
#endif
            )
        {
            GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_FORM_CTRL_ID,
            MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
            GUIFORM_CHILD_DISP_HIDE);
        }
        else
        {
            GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_FORM_CTRL_ID,
            MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
            GUIFORM_CHILD_DISP_NORMAL);
            SetMagicParam();
        }
#endif
#ifdef TTS_SUPPORT
        SetVoiceReportParam();
#endif
        
#if 0//def TOUCH_PANEL_SUPPORT
        SetVibrateTPParam();
#endif
#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
        SetFlyModeParam();
        SetFlyModePowerOnParam();
#endif
#ifdef MMI_IP_CALL_SUPPORT
        MMK_SetAtvCtrl(win_id, MMISET_IP_KEY_LABEL_CTRL_ID);
#endif
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, FALSE);
        break;

    case MSG_GET_FOCUS:
        SetMinuteReminderParam();
        break;
        
    case MSG_CTL_SETLIST_SWITCH:
        ctrl_id= MMK_GetActiveCtrlId(win_id);
        switch(ctrl_id)
        {
#ifdef MV_SUPPORT 
        case MMISET_MAGIC_VOICE_LIST_CTRL_ID:
            if(GUISETLIST_GetCurIndex(MMISET_MAGIC_VOICE_LIST_CTRL_ID) == 0)
            {

                GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
                        MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID,
                        GUIFORM_CHILD_DISP_NORMAL
                        );
            }
            else
            {
                GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
                        MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID,
                        GUIFORM_CHILD_DISP_GRAY
                        );  
            }
            SaveMagicParam();
            break;
#endif
        case MMISET_AUTO_CONNECT_EDIT_CTRL_ID:
            SaveAuTORedialParam();
            break;

        case MMISET_VIBRATE_TIP_EDIT_CTRL_ID:
            SaveVibrateTipParam();
            break;

#ifdef TTS_SUPPORT
        case MMISET_VOICE_REPORT_EDIT_CTRL_ID:
            SaveVoiceReportParam();
            break;
#endif
#if 0//def TOUCH_PANEL_SUPPORT
        case MMISET_VIBRATE_TP_EDIT_CTRL_ID:
            SaveVibrateTPParam();
            break;
#endif

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
        case MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID:
            {
                uint32 i  = 0;
                BOOLEAN is_fly_mode_on = MMIAPISET_GetFlyMode();
            
                if (!is_fly_mode_on)
                {
                    uint32 dual_sys =0;
                    BOOLEAN is_ps_ready = TRUE;
                    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

                    for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                    {
                        if ((e_dualsys_setting & (0x01 << dual_sys))
                            && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                        {
                            is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);

                            if (!is_ps_ready)
                            {
                                break;
                            }
                        }
                    }

                    if (is_ps_ready)
                    {
                        if (MMISET_GetIsQueryOpenFlyMode())
                        {
                            MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                            break;
                        }
                        else
                        {
                            is_fly_mode_on = TRUE;

                            MMIAPISET_SetFlyMode(is_fly_mode_on);

                            MMIAPIPHONE_DeactivePsAccordingToSetting();
                            
                            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
                        }
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                        
                        break;
                    }
                }
                else
                {
                    BOOLEAN is_ps_deactive = TRUE;

                    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                    {
                        if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
                        {
                            is_ps_deactive = FALSE;
                            break;
                        }
                    }
            
                    if (is_ps_deactive)
                    {
                        is_fly_mode_on = FALSE;
                    
                        MMIAPISET_SetFlyMode(is_fly_mode_on);
                    
                        MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
                    
                        // 激活协议栈
                        MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
                        MMIAPIMMS_RestartAutodownMMS();
                    
                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                        break;
                    }
                }
            }
            break;

        case MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID:
            {
                BOOLEAN is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
            
                is_flymode_openphone = !is_flymode_openphone;

                MMINV_WRITE(MMINV_FLY_MODE_OPEN_PHONE,&is_flymode_openphone);

                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            break;
#endif
        default:
            break;
        }
        break;
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        ctrl_id= MMK_GetActiveCtrlId(win_id);
        if(msg_id==MSG_CTL_PENOK)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        switch (ctrl_id)
        {
#ifdef MMI_IP_CALL_SUPPORT
        case MMISET_IP_KEY_LABEL_CTRL_ID:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            {
                uint16 sim_sys = MN_DUAL_SYS_1;
                uint32 sim_num = 0;

                sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);   

                if (0 == sim_num)
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                else if (1 == sim_num)
                {
                    MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)sim_sys);
                }
                else
                {
                    MMIAPISET_OpenUssdSelectWin(MMISET_IP_CALL);
                }
            }
#else
            MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_1);
#endif
            break;
#endif

        case MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID:
            MMK_CreateWin((uint32*)MMISET_SET_MINUTE_REMINDER_WIN_TAB,PNULL);
            break;

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
        case MMISET_ANSWER_MODE_LABEL_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_ANYKEY_REJECT_MODE_WIN_TAB,PNULL);
            break;
#endif

#ifdef MV_SUPPORT
        case MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID:
#if !defined(MMI_PDA_SUPPORT) 
            if(GUISETLIST_GetCurIndex(MMISET_MAGIC_VOICE_LIST_CTRL_ID) == 0)
#endif
            {
                MMIAPICC_OpenMagicVoiceSetMenu();
            }
            break;
#endif

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
     case MSG_PROMPTWIN_OK:
        {
            BOOLEAN is_fly_mode_on = TRUE;

            MMIPUB_CloseQuerytWin(PNULL);
            
            MMISET_StopPSApplication();
            
            MMIAPISET_SetFlyMode(is_fly_mode_on);
            
            MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);

#ifdef DCD_SUPPORT                      
            MMIAPIDCD_HandFlyModeSetChange();
#endif
            
            MMIAPIPHONE_DeactivePsAccordingToSetting();
            
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
#endif

        default:
            break;

        }
        break;

        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;

    }
    return recode;
}
#endif


/*****************************************************************************/
// Description : to handle set fly mode window msg
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeWindow(
                                        MMI_WIN_ID_T      win_id,
                                        MMI_MESSAGE_ID_E  msg_id,
                                        DPARAM            param
                                        )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    BOOLEAN             is_fly_mode_on  =   FALSE;
    MMI_CTRL_ID_T       ctrl_id         =   MMISET_FLY_MODE_LIST_CTRL_ID;
    uint16              cur_selection   =   0;
    uint32  i = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        is_fly_mode_on = MMIAPISET_GetFlyMode();
        GUILIST_SetMaxItem(MMISET_FLY_MODE_LIST_CTRL_ID,2, FALSE );
        
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, is_fly_mode_on, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,is_fly_mode_on);
        MMK_SetAtvCtrl(win_id,MMISET_FLY_MODE_LIST_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        is_fly_mode_on = MMIAPISET_GetFlyMode();
        
        if (is_fly_mode_on != cur_selection)
        {
            if(0 == cur_selection)
            {
                BOOLEAN is_ps_deactive = TRUE;

                for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
                    {
                        is_ps_deactive = FALSE;
                        break;
                    }
                }
                if (is_ps_deactive)
                {
                    is_fly_mode_on = (BOOLEAN)cur_selection;
                    
                    MMIAPISET_SetFlyMode(is_fly_mode_on);
                   
                    // 激活协议栈
                    MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
                    MMIAPIMMS_RestartAutodownMMS();
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    
                    MMK_CloseWin(win_id);
                    break;
                }
            }
            else
            {
                uint32 dual_sys =MN_DUAL_SYS_1;
                BOOLEAN is_ps_ready = TRUE;
                MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

                for (dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                {
                    if ((e_dualsys_setting & (0x01 << dual_sys))
                        && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                    {
                        is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);

                        if (!is_ps_ready)
                        {
                            break;
                        }
                    }
                }
            
                if (is_ps_ready)
                {
                    if (MMISET_GetIsQueryOpenFlyMode())
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                        break;
                    }
                    else
                    {
                        is_fly_mode_on = (BOOLEAN)cur_selection;
                        
                        MMIAPISET_SetFlyMode(is_fly_mode_on);

                        MMIAPIPHONE_DeactivePsAccordingToSetting();
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    
                    MMK_CloseWin(win_id);
                    break;
                }
            }
        }
        
        //success 
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        
        MMISET_StopPSApplication();
        
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        is_fly_mode_on = (BOOLEAN)cur_selection;
        
        MMIAPISET_SetFlyMode(is_fly_mode_on);

#ifdef DCD_SUPPORT                      
        MMIAPIDCD_HandFlyModeSetChange();
#endif
        
        MMIAPIPHONE_DeactivePsAccordingToSetting();
        
        //success 
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin( win_id );
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}


/*****************************************************************************/
// Description : to handle set fly mode window msg
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeSetWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E  msg_id,
                                           DPARAM            param
                                           )
{
    MMI_RESULT_E        recode                  =   MMI_RESULT_TRUE;
    BOOLEAN             is_fly_mode_on          =   FALSE;
    MMI_CTRL_ID_T       ctrl_id                 =   MMISET_FLY_MODE_SET_CTRL_ID;
    static uint16       cur_index               =   0;   
    BOOLEAN             is_flymode_openphone    =   FALSE;   
    uint32  i = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,ctrl_id);
        cur_index = 0;
        break;
        
    case MSG_FULL_PAINT:
        AppendFlyModeSetListItem(ctrl_id);  
        GUILIST_SetCurItemIndex(ctrl_id,cur_index);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB: 
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        switch(cur_index) 
        {
        case 0:
            is_fly_mode_on = MMIAPISET_GetFlyMode();
            
            if (!is_fly_mode_on)
            {
                uint32 dual_sys =0;
                BOOLEAN is_ps_ready = TRUE;
                MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

                for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                {
                    if ((e_dualsys_setting & (0x01 << dual_sys))
                        && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                    {
                        is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);

                        if (!is_ps_ready)
                        {
                            break;
                        }
                    }
                }

                if (is_ps_ready)
                {
                    if (MMISET_GetIsQueryOpenFlyMode())
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                        break;
                    }
                    else
                    {
                        is_fly_mode_on = TRUE;
                        MMIAPISET_SetFlyMode(is_fly_mode_on);

                        MMIAPIPHONE_DeactivePsAccordingToSetting();
                        
                        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    
                    break;
                }
            }
            else
            {
                BOOLEAN is_ps_deactive = TRUE;
                for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
                    {
                        is_ps_deactive = FALSE;
                        break;
                    }
                }
            
                if (is_ps_deactive)
                {
                    is_fly_mode_on = FALSE;
                    
                    MMIAPISET_SetFlyMode(is_fly_mode_on);
                    // 激活协议栈
                    MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
                    MMIAPIMMS_RestartAutodownMMS();
                    
                    MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    
                    break;
                }
                break;

            }
            break;
        case 1:   
            is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
            
            is_flymode_openphone = !is_flymode_openphone;
            MMINV_WRITE(MMINV_FLY_MODE_OPEN_PHONE,&is_flymode_openphone);
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
            
        default:
            break; 
        }
        break;
        
        case MSG_APP_CANCEL:    
        case MSG_CTL_CANCEL:              
            MMK_CloseWin( win_id );
            break;
            
        case MSG_PROMPTWIN_OK:
            MMIPUB_CloseQuerytWin(PNULL);
            
            MMISET_StopPSApplication();
            
            is_fly_mode_on = TRUE;
            MMIAPISET_SetFlyMode(is_fly_mode_on);

#ifdef DCD_SUPPORT                      
            MMIAPIDCD_HandFlyModeSetChange();
#endif
            
            MMIAPIPHONE_DeactivePsAccordingToSetting();
            
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
            
        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}






//@maggie add begin for cr79064
/*****************************************************************************/
//  Description : HandleNetworkTypeWindow
//  Global resource dependence : 
//  Author:maggie.ren
//  Note: yongwei.he modify 2007-07-18
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetworkTypeWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{//@zhaohui modify for cr94108
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
#ifdef MMI_DUALMODE_ENABLE 
    uint16 cur_sel_item = 0;
    NETWORK_TYPE_E network_type    =   0;/*lint !e64*/

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_NETWORK_TYPE_CTRL_ID, MMISET_SET_MAX_NETWORK_TYPE,FALSE);//max item 4
        AppendNetworkTypeListItem(MMISET_SET_NETWORK_TYPE_CTRL_ID);

        GUILIST_SetOwnSofterKey( MMISET_SET_NETWORK_TYPE_CTRL_ID, TRUE );

        MMK_SetAtvCtrl(win_id,MMISET_SET_NETWORK_TYPE_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
        network_type = GetNetworkType();

        //set selected item
        GUILIST_SetSelectedItem(MMISET_SET_NETWORK_TYPE_CTRL_ID, network_type, TRUE);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_sel_item = GUILIST_GetCurItemIndex(MMISET_SET_NETWORK_TYPE_CTRL_ID);
        SetNetworkType((NETWORK_TYPE_E)cur_sel_item, FALSE, FALSE, FALSE);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
#endif
    
    return recode;    
}

#if defined(MMI_GUI_STYLE_TYPICAL)
/*****************************************************************************/
//  Description : handle set any key and reject mode win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAnykeyRejectWindow(
                                                MMI_WIN_ID_T    win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                )
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_SET_ANYKEY_REJECT_MODE_CTRL_ID;  
    uint16                      selected_num    =   0;
    MMISET_ANYKEY_REJECT_SET_T  mode_info       =   {0};       
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_SET_ANYKEY_REJECT_MAX_ITEM, FALSE );//max item 2
        GUILIST_SetMaxSelectedItem(ctrl_id,MMISET_SET_ANYKEY_REJECT_MAX_ITEM);
        MMISET_AppendAnykeyRejectListItem(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //get current selected items
        selected_num = GUILIST_GetCurItemIndex(ctrl_id);
        mode_info = MMIAPISET_GetAnykeyRejectMode();
        
        switch (selected_num)
        {
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
        case 0:
            mode_info.is_sms_reply_open = mode_info.is_sms_reply_open ? FALSE:TRUE;
            break;
            
        case 1:
            mode_info.is_anykey_answer = mode_info.is_anykey_answer ?  FALSE:TRUE;
            break;
#else
        case 0:
            mode_info.is_anykey_answer = mode_info.is_anykey_answer ?  FALSE:TRUE;
            break;
#endif
            
        default:
            //SCI_TRACE_LOW:"HandleSetAnykeyRejectWindow: selected item is %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_8587_112_2_18_2_50_34_84,(uint8*)"d",selected_num);
            break;
        }
        
        MMINV_WRITE(MMINV_SET_ANYKEY_REJECT_MODE,&mode_info);
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            //save anykey reject mode info
            MMISET_SetAnykeyRejectMode(ctrl_id);
            MMK_CloseWin( win_id );      
            break;
            
        case MSG_APP_RED:
            //save anykey reject mode info
            MMISET_SetAnykeyRejectMode(ctrl_id);
            recode = MMI_RESULT_FALSE;
            break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.            
        case MSG_APP_FLIP:
            //save anykey reject mode info
            MMIDEFAULT_HandleFlipKey(FALSE);
            MMISET_SetAnykeyRejectMode(ctrl_id);
            recode = MMI_RESULT_FALSE;
            break;
#endif            
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;  
}
#endif

/*****************************************************************************/
//  Description : handle set connect prompt remind win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetConnectPromptWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id  =   MMISET_SET_CONNECT_PROMPT_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        MMISET_AppendOpenCloseListItem(ctrl_id,MMIAPISET_GetIsConnectPrompt);       
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if !defined(MMI_PDA_SUPPORT) 
    case MSG_CTL_OK:
#endif
    case MSG_APP_WEB:
    case MSG_APP_OK:

        MMIAPISET_SetByOpenCloseList(ctrl_id, MMIAPISET_GetActiveSim(), MMISET_SetIsConnectPrompt);
        //success!
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        
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


/*****************************************************************************/
//  Description : to open ussd select  window
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenUssdSelectWin(MMISET_SIM_SELECT_TYPE_E select_type)
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)

    MMISET_APPEND_SIM_TYPE_E append_type =MMISET_APPEND_SYS_OK;

    switch((MMISET_SIM_SELECT_TYPE_E)select_type) 
    {
    case MMISET_MP3_PLAYER_CALL_RING:
    case MMISET_MULTIM_MUSIC_CALL_RING:
    //case MMISET_IP_CALL:
    case MMISET_FLIP_ANSWER:
        append_type = MMISET_APPEND_SIM_ALL;
        break;

    case MMISET_SET_PIN:
        append_type = MMISET_APPEND_PIN1_PERMIT;
        break;

    case MMISET_SET_PIN2_MODIFY:
#ifdef FDN_SUPPORT
    case MMISET_SET_FDN:
#endif
        append_type =MMISET_APPEND_SIM_OK;
        break;
    case MMISET_MULTI_SIM_NAME:
#ifdef MMI_MULTISIM_COLOR_SUPPORT
    case MMISET_MULTI_SIM_COLOR:
#endif    
        append_type =MMISET_APPEND_SIM_ALL;
        break;
    default:
        append_type =MMISET_APPEND_SYS_OK;
        break;
    }

    MMISET_SelectSIMFunc(SIMSelectWinCallback,append_type,(ADD_DATA)select_type);
#endif
}

/*****************************************************************************/
// Description : 设置飞行模式
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-8
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPISET_OpenSetFlyMode(void)
{
    MMK_CreateWin((uint32*)MMISET_FLY_MODE_WIN_TAB,PNULL);
    return MMISET_FLY_MODE_WIN_ID;
}

///*****************************************************************************/
////    Description : MMISET_EnterMultiSimSettingMenu
////    Global resource dependence : 
////  Author:
////    Note: 
///*****************************************************************************/
#ifndef MMI_MULTI_SIM_SYS_SINGLE 
PUBLIC void MMISET_EnterMultiSimSettingMenu(void)
{
    if (MMK_IsOpenWin(MMISET_SET_MULTI_SYS_SETTING_WIN_ID))
    {
        MMK_WinGrabFocus(MMISET_SET_MULTI_SYS_SETTING_WIN_ID);

    }
    else
    {
        MMK_CreateWin ((uint32 *)MMISET_MULTI_SIM_SETTING_WIN_TAB,PNULL);
    }
}
#endif

/*****************************************************************************/
//  Description : 语音信箱号码界面打开时，等待界面不允许返回
//  Global resource dependence : 
//  Author: kelly.li
//  Note:2006-02-13
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVMForDivertWaitingWindow(
                                                  MMI_WIN_ID_T      win_id,     // 窗口的ID
                                                  MMI_MESSAGE_ID_E     msg_id,  // 窗口的内部消息ID
                                                  DPARAM                param       // 相应消息的参数
                                                  )
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

#ifdef FLIP_PHONE_SUPPORT
/*****************************************************************************/
//  Description : 翻盖接听
//  Global resource dependence : 
//  Author:jibin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetFlipAnswerWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    uint32                      dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id);
    uint16                      cur_selection   =   0;
    MMISET_FLIP_ANSWER_TYPE_T   flip_answer     =   {0};/*lint !e64*/
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_SET_FLIP_ANSWER_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_FLIP_ANSWER_CTRL_ID,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        
        //add items to listbox
        MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        
        if (MMIAPISET_GetIsFlipAnswer(dual_sys))/*lint !e64*/
        {
            cur_selection = 0;
        }
        else
        {
            cur_selection = 1;
        }
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        
        //        MMISET_AppendOpenCloseListItem(MMISET_SET_FLIP_ANSWER_CTRL_ID,MMIAPISET_GetIsFlipAnswer);
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_FLIP_ANSWER_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16 i = 0;

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                flip_answer.is_open[i] = MMIAPISET_GetIsFlipAnswer((MN_DUAL_SYS_E)i);
            }

            //get the selected item
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            
            //SCI_ASSERT((MMISET_OPEN_CLOSE_ITEM > cur_selection));
          if(MMISET_OPEN_CLOSE_ITEM <= cur_selection)
          {
              //SCI_TRACE_LOW:"mmisetting_assert HandleSetFlipAnswerWindow cur_selection %d"
              SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_8908_112_2_18_2_50_35_85,(uint8*)"d", cur_selection);
              break;
          } 
            
            flip_answer.is_open[dual_sys] = (cur_selection == 0? TRUE:FALSE);
            
            MMISET_SetIsFlipAnswer(&flip_answer);
            //        MMISET_SetByOpenCloseList(MMISET_SET_FLIP_ANSWER_CTRL_ID, dual_sys, MMISET_SetIsFlipAnswer);
            
            //success!
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            
            MMK_CloseWin( win_id );
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
#endif

LOCAL MMI_RESULT_E  HandleCloseFlyModeFirstWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    uint32          dual_sys    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    switch (msg_id)
    {
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		break;
	case MSG_APP_OK:	
		break;
    case MSG_CTL_OK:
    case MSG_APP_WEB:
	    MMK_CloseWin( win_id ); 	
		break;
	default:
		recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
		break;
    	}
	return recode;
}
/*****************************************************************************/
//  Description : 提示飞行模式
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_AlertFlyMode(void)
{
    //MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST);
    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CLOSE_FLY_MODE_FIRST,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleCloseFlyModeFirstWindow);
}

/*****************************************************************************/
//  Description : to handle plmn list
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePlmnListWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    uint32          dual_sys    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    MN_PLMN_T cur_plmn = {0};
    MN_GMMREG_RAT_E  rat  =   MN_GMMREG_RAT_GPRS;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,MMISET_SET_PLMN_LIST_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
//    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMISET_SelectNetwork(dual_sys, MMISET_SET_PLMN_LIST_CTRL_ID);/*lint !e64*/
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
#if 0//def MMI_SMART_DUAL_SIM_SUPPORT
        case MSG_FULL_PAINT:
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_HDR_NETWORK_SELECTION_EXT01))
            {
                GUIWIN_UpdateStb();
            }
            break;
#endif            
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }   
    return recode;
}

                                             
/*****************************************************************************/
//  Description : to select no answer time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNoAnswerTimeWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    recode          =   MMI_RESULT_TRUE;
    MMI_STRING_T    voice_number    =   {0};
    MMI_CTRL_ID_T cur_ctrl_id = MMISET_SET_NO_ANSWER_TIME_CTRL_ID;
    uint32          dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id);
    
    SCI_MEMSET(&voice_number, 0, sizeof(MMI_STRING_T));
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
	case MSG_FULL_PAINT:	
        GUILIST_SetMaxItem(MMISET_SET_NO_ANSWER_TIME_CTRL_ID,MMISET_NO_ANSWER_TIME_MAX_ITEM, FALSE );//max item 6
        MMISET_AppendNoAnswerTimeListItem(MMISET_SET_NO_ANSWER_TIME_CTRL_ID);
        MMK_SetAtvCtrl(win_id,MMISET_SET_NO_ANSWER_TIME_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if (MMK_IsOpenWin(MMISET_SET_DIVERT_NUMBER_WIN_ID))
        {
            //get phone number
            GUIEDIT_GetString(MMISET_SET_DIVERT_NUMBER_CTRL_ID, &voice_number);
        }
        else
        {
            //get voice box number
            GUIEDIT_GetString(MMISET_SET_DIVERT_VOICE_CTRL_ID, &voice_number);
        }
        
        if (0 == voice_number.wstr_len)//no phone number
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
            MMK_CloseWin( win_id ); 
        }
        else if (MMIAPICOM_IsValidNumberStringEx(&voice_number))//enter call forwarding
        {   
            uint32 i = 0;
            BOOLEAN is_valid = TRUE;
            
            for (i = 0; i < voice_number.wstr_len; i++)
            {
                if ('*' == voice_number.wstr_ptr[i]
                    || '#' == voice_number.wstr_ptr[i]
                    || 'p' == voice_number.wstr_ptr[i]
                    || 'w' == voice_number.wstr_ptr[i]
                    || 'P' == voice_number.wstr_ptr[i]
                    || 'W' == voice_number.wstr_ptr[i])
                {
                    is_valid = FALSE;
                    
                    break;
                }
            }
            
            if (is_valid)
            {
                MMISET_StartCallForwarding(dual_sys, &voice_number);/*lint !e64*/
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
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




/*****************************************************************************/
//     Description : add fly mode set list to list 
// Global resource dependence : 
//  Author: kelly.li
// Note:
/*****************************************************************************/
LOCAL void AppendFlyModeSetListItem(
                                    MMI_CTRL_ID_T      ctrl_id
                                    )
{
    GUILIST_ITEM_T          item_t                  =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data               =   {0};/*lint !e64*/
    MMI_STRING_T            fly_string              =   {0};  
    MMI_IMAGE_ID_T          image_id                =   IMAGE_NULL;
    MMI_TEXT_ID_T           fly_text_id[2]          =   {TXT_ACTIVE_OPERATIVE,TXT_SET_SELECT_ON_POWER};
    uint32                  i                       =   0;
    uint16                  cur_selection           =   0;
    BOOLEAN                 is_fly_mode_on          =   FALSE;
    BOOLEAN                 is_flymode_openphone    =   FALSE;
    
    GUILIST_SetMaxItem(ctrl_id,2, FALSE );   
    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)STXT_SELECT;
    item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;
    
    is_fly_mode_on = MMIAPISET_GetFlyMode();
    is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
    
    for (i = 0; i < 2; i++)
    {
        
        if(0 == i)
        {
            if (is_fly_mode_on)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
                item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_CANCEL;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
                item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_SELECT;
            }
        }
        else
        {
            if (is_flymode_openphone)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
                item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_CANCEL;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
                item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_SELECT;
            }
     
        }
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = image_id;
        
        
        MMI_GetLabelTextByLang(fly_text_id[i], &fly_string);
        
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = fly_string.wstr_len;  
        
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = fly_string.wstr_ptr;
        
        //         MMIAPICOM_Wstrncpy( item_data.item_content[1].item_data.text_buffer.wstr,
        //                             fly_string.wstr_ptr,
        //                             item_data.item_content[1].item_data.text_buffer.wstr_len );
        
        //item_data.item_content[1].item_data.text_buffer.is_ucs2 = fly_string.is_ucs2;
        
        GUILIST_AppendItem(ctrl_id,&item_t);
    }//end for
    
    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}


/*****************************************************************************/
//     Description : replace some struct of the  GUILIST_ITEM_T
// Global resource dependence : 
//  Author: Qiang.Zhang
// Note:
/*****************************************************************************/
LOCAL void SetCallSetOthersItemCheck(
                        MMI_CTRL_ID_T ctrl_id,
                        BOOLEAN is_check,
                        uint16 index
                            )
{
    const GUILIST_ITEM_T* item_t = PNULL;

    item_t = GUILIST_GetItemPtrByIndex(ctrl_id, index);

    item_t->item_data_ptr->item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_t->item_data_ptr->item_content[1].item_data.text_id = is_check?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON;

    item_t->item_data_ptr->softkey_id[0] = is_check ? STXT_CANCEL : STXT_SELECT;

    GUILIST_ReplaceItem(ctrl_id, item_t, index);
}
 
 /*****************************************************************************/
 //  Description : to handle interrogate ss status report
 //  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleAllCFReportWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set softkey
        MMK_SetAtvCtrl(win_id,MMISET_SS_ALL_REPORT_CTRL_ID);
        GUIRICHTEXT_SetFocusMode(MMISET_SS_ALL_REPORT_CTRL_ID, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,STXT_RETURN,FALSE);
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

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : ImplementStandbyChanged
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ImplementStandbyChanged(BOOLEAN * is_standby_ptr, MN_DUAL_SYS_E dual_sys)
{
    uint32 i = 0;
    // BOOLEAN is_changed = FALSE;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MMISET_DUALSYS_TYPE_E standby_mode = 0;
    MMISET_GPRS_MODE_TYPE_E mode = DATA_CARD_MODE_1;

    //SCI_ASSERT(PNULL != is_standby_ptr);
   if(PNULL == is_standby_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert ImplementStandbyChanged"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_9243_112_2_18_2_50_36_86,(uint8*)"");
        return FALSE;
    } 

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (is_standby_ptr[i])
        {
            standby_mode |= (0x01 << i);
        }
    }

    if (e_dualsys_setting != standby_mode)
    {
        if (MMIAPISET_GetFlyMode())//如果是飞行模式只切换待机模式
        {
            //记录用户自己设置的情况
            MMISET_SetMultiSysSetting(standby_mode);
            
            MMIAPICOM_StatusAreaUpdateSignalState();
        }
        else
        {
            BOOLEAN is_allow_set = TRUE;

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (!(e_dualsys_setting & (0x01 << i)) && is_standby_ptr[i])
                {
                    if (!MMIAPIPHONE_GetIsPsDeactiveCnf(i))
                    {
                        is_allow_set = FALSE;
                        break;
                    }
                }
            }

            if (!is_allow_set)
            {
                return FALSE;
            }

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if ((e_dualsys_setting & (0x01 << i))
                    && MMIAPIPHONE_GetSimExistedStatus(i)                    
                    && !is_standby_ptr[i])
                {
                    if (!MMIAPIPHONE_GetIsPsReady(i))
                    {
                        is_allow_set = FALSE;
                        break;
                    }
                }
            }

            if (!is_allow_set)
            {
                return FALSE;
            }

            MMIAPISET_SetIsQueryCfu(FALSE, dual_sys);
            mode = MMIAPISET_GetGPRSModeType();

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if ((e_dualsys_setting & (0x01 << i)) && !is_standby_ptr[i])//关闭待机
                {
                    /* pwr on -> pwr off */
#ifdef MMI_GPRS_SUPPORT
                    MMIAPIPDP_ResetPdp(i);
                    MMIAPIPHONE_SetGPRSDetachEx(i);
#endif

                    MMIAPIPHONE_PowerOffPs(i);
                    MMIAPICOM_StatusAreaSetForwardState(i,FALSE);
                    MMIAPICOM_StatusAreaSetRoamingState(i,FALSE);
                    MMIAPIPHONE_SetIdleGprsState(i);
#ifdef QQ_SUPPORT
                    MMIAPIQQ_UpdateSimSelect(i);
#endif

                    if ((DATA_CARD_MANUAL_SELECT != mode) &&	(i == mode))
                    {
                        #ifdef MAINLCD_SIZE_128X160
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SYS_DISABLE_DATA_CARD_ALERT,TXT_NULL,IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                        #else
                        MMIPUB_OpenAlertWarningWin(TXT_SYS_DISABLE_DATA_CARD_ALERT);
                        #endif
                    }

                }
                else if (!(e_dualsys_setting & (0x01 << i)) 
                            && MMIAPIPHONE_GetSimExistedStatus(i)
                            && is_standby_ptr[i])//开启待机
                {
                    /* pwr off -> pwr on */
                    MMIAPIPHONE_PowerOnPs(i);
                }
                MMIAPIPHONE_SetIdleGprsState(i);
            }

            //记录用户自己设置的情况
            MMISET_SetMultiSysSetting(standby_mode);

#ifdef MMI_GPRS_SUPPORT
            MMIAPIPHONE_SetGPRSAttachMode();

            if (!MMIAPICONNECTION_isGPRSNeedAutoAttach())
            {
                for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if(is_standby_ptr[i] && !MMIAPIPDP_PsIsPdpLinkExistEx(i))
                    {
                        MMIAPIPHONE_SetGPRSDetachEx((MN_DUAL_SYS_E)i);
                    }
                }   
            }
            else
            {
                MMIAPIPHONE_SetGPRSAttach();
            }
#endif/*MMI_GPRS_SUPPORT*/

            if (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))//无待机卡
            {
                BOOLEAN is_first = TRUE;
                //目前不支持零待机，无待机卡，则当前有且仅有一个PS启动。
                //遍历所有设置为待机且无SIM卡的卡槽，启动第一个遍历到的PS，其他均关闭PS
                for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
                {
                    if ((standby_mode & (0x01 << i))
                        && !MMIAPIPHONE_GetSimExistedStatus(i))
                    {
                        if (is_first) // 启动第一个遍历到的无SIM卡卡槽PS
                        {
                            MMIAPIPHONE_PowerOnPs((MN_DUAL_SYS_E)i);
                            is_first = FALSE;
                            continue; //继续搜索剩下的设置待机且无SIM卡的卡槽，并关闭相应的ps
                        }
                        
                        MMIAPIPHONE_PowerOffPs((MN_DUAL_SYS_E)i);
                    }
                }
            }
            else
            {
                for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
                {
                    if ((standby_mode & (0x01 << i))
                        && !MMIAPIPHONE_GetSimExistedStatus(i))//关闭不存在SIM卡的PS
                    {
                        MMIAPIPHONE_PowerOffPs((MN_DUAL_SYS_E)i);
                    }
                }
            }

            MMIAPICOM_StatusAreaUpdateSignalState();
        }
    }

    return TRUE;
}

LOCAL BOOLEAN ImplementSmartDualSIMStandbyChanged(BOOLEAN * is_standby_ptr, MN_DUAL_SYS_E dual_sys)
{
    uint32 i = 0;
    // BOOLEAN is_changed = FALSE;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MMISET_DUALSYS_TYPE_E standby_mode = 0;

    //SCI_ASSERT(PNULL != is_standby_ptr);
   if(PNULL == is_standby_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert ImplementStandbyChanged"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_9243_112_2_18_2_50_36_86,(uint8*)"");
        return FALSE;
    } 
    SCI_TRACE_LOW("ImplementSmartDualSIMStandbyChanged--e_dualsys_setting=%d, MMI_DUAL_SYS_MAX=%d", e_dualsys_setting, MMI_DUAL_SYS_MAX);
    SCI_TRACE_LOW("ImplementSmartDualSIMStandbyChanged--is_standby_ptr[%d]=%d", dual_sys, is_standby_ptr[dual_sys]);
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (is_standby_ptr[i])
        {
            standby_mode |= (0x01 << i);
        }
        SCI_TRACE_LOW("ImplementSmartDualSIMStandbyChanged--is_standby_ptr[%d]=%d, standby_mode=%d", i, is_standby_ptr[i], standby_mode);
    }

    if (e_dualsys_setting != standby_mode)
    {
        if (MMIAPISET_GetFlyMode())//如果是飞行模式只切换待机模式
        {
            //记录用户自己设置的情况
            MMISET_SetMultiSysSetting(standby_mode);
            
            MMIAPICOM_StatusAreaUpdateSignalState();
        }
        else
        {
            BOOLEAN is_allow_set = TRUE;

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (!(e_dualsys_setting & (0x01 << i)) && is_standby_ptr[i])
                {
                    if (!MMIAPIPHONE_GetIsPsDeactiveCnf(i))
                    {
                        is_allow_set = FALSE;
                        break;
                    }
                }
            }

            if (!is_allow_set)
            {
                return FALSE;
            }

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if ((e_dualsys_setting & (0x01 << i))
                    && MMIAPIPHONE_GetSimExistedStatus(i)                    
                    && !is_standby_ptr[i])
                {
                    if (!MMIAPIPHONE_GetIsPsReady(i))
                    {
                        is_allow_set = FALSE;
                        break;
                    }
                }
            }

            if (!is_allow_set)
            {
                return FALSE;
            }

            MMIAPISET_SetIsQueryCfu(FALSE, dual_sys);

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if ((e_dualsys_setting & (0x01 << i)) && !is_standby_ptr[i])//关闭待机
                {
                    /* pwr on -> pwr off */
                    MMIAPIPHONE_PowerOffPs(i);
                    MMIAPICOM_StatusAreaSetForwardState(i,FALSE);
                    MMIAPICOM_StatusAreaSetRoamingState(i,FALSE);
                    MMIAPIPHONE_SetIdleGprsState(i);
#ifdef QQ_SUPPORT
                    MMIAPIQQ_UpdateSimSelect(i);
#endif
                }
                else if (!(e_dualsys_setting & (0x01 << i)) 
                            && MMIAPIPHONE_GetSimExistedStatus(i)
                            && is_standby_ptr[i])//开启待机
                {
                    /* pwr off -> pwr on */
                    MMIAPIPHONE_PowerOnPs(i);
                }
                MMIAPIPHONE_SetIdleGprsState(i);
            }

            //记录用户自己设置的情况
            MMISET_SetMultiSysSetting(standby_mode);

#ifdef MMI_GPRS_SUPPORT
            MMIAPIPHONE_SetGPRSAttachMode();
            MMIAPIPHONE_SetGPRSAttach();
#endif/*MMI_GPRS_SUPPORT*/

            if (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))//无待机卡
            {
                BOOLEAN is_first = TRUE;
                //目前不支持零待机，无待机卡，则当前有且仅有一个PS启动。
                //遍历所有设置为待机且无SIM卡的卡槽，启动第一个遍历到的PS，其他均关闭PS
                for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
                {
                    if ((standby_mode & (0x01 << i))
                        && !MMIAPIPHONE_GetSimExistedStatus(i))
                    {
                        if (is_first) // 启动第一个遍历到的无SIM卡卡槽PS
                        {
                            MMIAPIPHONE_PowerOnPs((MN_DUAL_SYS_E)i);
                            is_first = FALSE;
                            continue; //继续搜索剩下的设置待机且无SIM卡的卡槽，并关闭相应的ps
                        }
                        
                        MMIAPIPHONE_PowerOffPs((MN_DUAL_SYS_E)i);
                    }
                }
            }
            else
            {
                for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
                {
                    if ((standby_mode & (0x01 << i))
                        && !MMIAPIPHONE_GetSimExistedStatus(i))//关闭不存在SIM卡的PS
                    {
                        MMIAPIPHONE_PowerOffPs((MN_DUAL_SYS_E)i);
                    }
                }
            }

            MMIAPICOM_StatusAreaUpdateSignalState();
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : AppendMultiSysTypeList
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AppendMultiSysTypeList(MMI_CTRL_ID_T ctrl_id, BOOLEAN is_update)
{
    uint32 i = 0;
    uint16 index = 0;
    uint16 cur_selection = 0;
    uint16 topItemIndex = 0;
    BOOLEAN is_fly_mode_on = FALSE;
    BOOLEAN is_flymode_openphone =   FALSE;
    MMI_STRING_T fly_string =  {0};
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_IMAGE_ID_T status_image_id = IMAGE_NULL;
    MMI_IMAGE_ID_T sim_image_id = 0;//IMAGE_NULL;
    MMISET_SIM_NAME_T sim_str = {0};
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    MMI_STRING_T network_str = {0};
    MMI_IMAGE_ID_T sim_img_id[MMI_DUAL_SYS_MAX] ={0};
    MMI_IMAGE_ID_T no_sim_img_id[MMI_DUAL_SYS_MAX] = {0};
    MMI_IMAGE_ID_T gray_sim_img_id[MMI_DUAL_SYS_MAX] = {0};
    is_fly_mode_on = MMIAPISET_GetFlyMode();
    is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();

    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        sim_img_id[i] = IMAGE_IDLE_PROMPT_SIM1 + (i-MN_DUAL_SYS_1);    //shoule IMAGE_IDLE_PROMPT_NO_SIM2 == IMAGE_IDLE_PROMPT_NO_SIM1+1.atc
        no_sim_img_id[i] = IMAGE_IDLE_PROMPT_NO_SIM1 + (i-MN_DUAL_SYS_1);
        gray_sim_img_id[i] = IMAGE_IDLE_PROMPT_GRAY_SIM1 + (i-MN_DUAL_SYS_1);
    }

    cur_selection = GUILIST_GetCurItemIndex(ctrl_id); 
    topItemIndex = GUILIST_GetTopItemIndex(ctrl_id);

#if !defined MMI_GUI_STYLE_TYPICAL 
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
#else
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT;
#endif
    item_t.item_data_ptr = &item_data;
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

        if (e_dualsys_setting & (0x01 << i)
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
			&& MMIAPIPHONE_GetSimStatus(i) != SIM_STATUS_LOCKED_INVALID
#endif
			)
        {
            status_image_id = IMAGE_CHECK_SELECTED_ICON;

            item_data.softkey_id[LEFT_BUTTON] = TXT_CLOSE;
#ifdef MMI_GUI_STYLE_MINISCREEN     
          item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)IMAGE_NULL;
#else
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
#endif
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;

            if (MMIAPIPHONE_GetSimExistedStatus(i))
            {
                if (is_fly_mode_on)
                {
                    sim_image_id = gray_sim_img_id[i];
                }
                else
                {
                    sim_image_id = sim_img_id[i];
                }
            }
            else
            {
                status_image_id = IMAGE_CHECK_UNSELECTED_ICON;
                sim_image_id = no_sim_img_id[i];
            }
        }
        else
        {
            status_image_id = IMAGE_CHECK_UNSELECTED_ICON;

            item_data.softkey_id[LEFT_BUTTON] = TXT_OPEN;
#ifdef MMI_GUI_STYLE_MINISCREEN     
          item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)IMAGE_NULL;
#else               
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
#endif
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;

            if (MMIAPIPHONE_GetSimExistedStatus(i))
            {
                sim_image_id = gray_sim_img_id[i];
            }
            else
            {
                sim_image_id = no_sim_img_id[i];
            }
        }

        if (is_fly_mode_on)
        {
            status_image_id = IMAGE_CHECK_UNSELECTED_ICON;
            item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
        }

        sim_str = MMIAPISET_GetSimName(i);
#if !defined MMI_GUI_STYLE_TYPICAL      
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_len = MIN(GUILIST_STRING_MAX_NUM, sim_str.wstr_len);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = sim_str.wstr_arr;

        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = status_image_id;
        
        SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
        network_str.wstr_ptr = (wchar *)opn_spn_name;
        SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
        
        MMIAPISET_GetSimServiceString((MN_DUAL_SYS_E)i, &network_str);

        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_len = network_str.wstr_len;  
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = network_str.wstr_ptr;

#else
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = status_image_id;

        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = sim_image_id;
        
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_len = MIN(GUILIST_STRING_MAX_NUM, sim_str.wstr_len);
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = sim_str.wstr_arr;

        SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
        network_str.wstr_ptr = (wchar *)opn_spn_name;
        SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
        
        MMIAPISET_GetSimServiceString((MN_DUAL_SYS_E)i, &network_str);

        item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[3].item_data.text_buffer.wstr_len = network_str.wstr_len;  
        item_data.item_content[3].item_data.text_buffer.wstr_ptr = network_str.wstr_ptr;
#endif
        if(is_update)
        {
            GUILIST_ReplaceItem(ctrl_id, &item_t, index);
        }
        else
        {
            GUILIST_AppendItem(ctrl_id, &item_t);
        }
        index++;

        if (is_fly_mode_on)
        {
            GUILIST_SetItemGray(ctrl_id, i, TRUE);
            GUILIST_SetItemInvalid(ctrl_id, i, TRUE);
        }
        else
        {
            GUILIST_SetItemGray(ctrl_id, i, FALSE);
            GUILIST_SetItemInvalid(ctrl_id, i, FALSE);
        }

        if (!MMIAPIPHONE_GetSimExistedStatus(i)
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
			|| MMIAPIPHONE_GetSimStatus(i) == SIM_STATUS_LOCKED_INVALID
#endif
			)
        {
            GUILIST_SetItemGray(ctrl_id, i, TRUE);
            GUILIST_SetItemInvalid(ctrl_id, i, TRUE);
        }
    }

    if (is_fly_mode_on)
    {
        sim_image_id = IMAGE_CHECK_SELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_CANCEL;
    }
    else
    {
        sim_image_id = IMAGE_CHECK_UNSELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_SELECT;
    }
#if !defined MMI_GUI_STYLE_TYPICAL       
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    
    MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &fly_string);

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = fly_string.wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = fly_string.wstr_ptr;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = sim_image_id;
    
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[2].item_data.text_id = TXT_NULL;  

    if(is_update)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, index);
    }
    else
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }
    index++;

    if (is_flymode_openphone)
    {
        sim_image_id = IMAGE_CHECK_SELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_CANCEL;
    }
    else
    {
        sim_image_id = IMAGE_CHECK_UNSELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_SELECT;
    }
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;

    MMI_GetLabelTextByLang(TXT_SET_SELECT_ON_POWER, &fly_string);  
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = fly_string.wstr_len;  
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = fly_string.wstr_ptr;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = sim_image_id;        
    
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[2].item_data.text_id = TXT_NULL;  
#else
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TWO_ICON_TEXT;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = sim_image_id;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = IMAGE_SECMENU_ICON_CALLSET_FLYMODE;        
    MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &fly_string);

    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer.wstr_len = fly_string.wstr_len;
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = fly_string.wstr_ptr;
    if(is_update)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, index);
    }
    else
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }
    index++;

    if (is_flymode_openphone)
    {
        sim_image_id = IMAGE_CHECK_SELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_CANCEL;
    }
    else
    {
        sim_image_id = IMAGE_CHECK_UNSELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_SELECT;
    }
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TWO_ICON_TEXT;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = sim_image_id;
    
    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = IMAGE_SECMENU_ICON_CALLSET_FLYMODE;        
    MMI_GetLabelTextByLang(TXT_SET_SELECT_ON_POWER, &fly_string);   

    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer.wstr_len = fly_string.wstr_len;  
    
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = fly_string.wstr_ptr;
#endif
    if(is_update)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, index);
    }
    else
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }

//     GUILIST_SetTopItemIndex(ctrl_id, topItemIndex);
     GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
 }

/*****************************************************************************/
//  Description : to handle dual sys time window msg
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMultiSysTypeWindow(
                                            MMI_WIN_ID_T       win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    uint32 i = 0;
    uint16 cur_selection = 0;
    BOOLEAN is_ps_deactive = TRUE;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN             is_fly_mode_on  =   FALSE;
    BOOLEAN             is_flymode_openphone    =   FALSE;
    MMI_CTRL_ID_T list_ctrl = MMISET_SET_DAULSYS_TYPE_CTRL_ID;
    LOCAL BOOLEAN s_is_standby[MMI_DUAL_SYS_MAX] = {0};
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    uint32 dual_sys = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SCI_MEMSET(s_is_standby, 0x00, sizeof(s_is_standby));
        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (e_dualsys_setting & (0x01 << i)
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
				&& MMIAPIPHONE_GetSimStatus(i) != SIM_STATUS_LOCKED_INVALID
#endif
				)
            {
                s_is_standby[i] = TRUE;
            }
        }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)  
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
#endif
        GUILIST_SetMaxItem(list_ctrl, MMI_DUAL_SYS_MAX+2, FALSE);
        AppendMultiSysTypeList(list_ctrl, FALSE);

        MMK_SetAtvCtrl(win_id, list_ctrl);
        break;

    case MSG_FULL_PAINT:
        AppendMultiSysTypeList(list_ctrl, TRUE);
//         GUILIST_ValidCurItemPos(list_ctrl);
        break;

#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
    case MSG_GET_FOCUS:  
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
        break;
#endif 

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            BOOLEAN is_has_selected = FALSE;

            is_fly_mode_on = MMIAPISET_GetFlyMode();
            //get cur selection
            cur_selection = GUILIST_GetCurItemIndex(list_ctrl);
            SCI_TRACE_LOW("HandleSetMultiSysTypeWindow is_fly_mode_on = %d, cur_selection = %d", is_fly_mode_on, cur_selection);

            if(cur_selection == MMI_DUAL_SYS_MAX)
            {
#ifdef MCARE_V31_SUPPORT
                    if(MCareV31_Exit_Confirm())
                    {
                         return MMI_RESULT_FALSE;          
                    }
#endif
                if (!is_fly_mode_on)
                {
                    BOOLEAN is_ps_ready = TRUE;
                    
#ifdef BLUETOOTH_SUPPORT
                    if(MMIAPIBT_IsOpenFileWin())
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_BT_CLOSE_WARNING);
                        return MMI_RESULT_FALSE;
                    } 
#endif

                    for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                    {
                        if ((e_dualsys_setting & (0x01 << dual_sys))
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
							&& MMIAPIPHONE_GetSimStatus(dual_sys) != SIM_STATUS_LOCKED_INVALID
#endif
                            && MMIAPIPHONE_GetSimExistedStatusIncBootUP(dual_sys))
                        {
                            is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);

                            if (!is_ps_ready)
                            {
                                break;
                            }
                        }
                    }

                    SCI_TRACE_LOW("HandleSetMultiSysTypeWindow is_ps_ready = %d", is_ps_ready);

                    if (is_ps_ready)
                    {
                        if (MMISET_GetIsQueryOpenFlyMode())
                        {
                            MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                            break;
                        }
                        else
                        {
                            is_fly_mode_on = TRUE;
                            MMIAPISET_SetFlyMode(is_fly_mode_on);

                            MMIAPIPHONE_DeactivePsAccordingToSetting();
                            //SetCallSetOthersItemCheck(list_ctrl,is_fly_mode_on,cur_selection);
                            SCI_TRACE_LOW("HandleSetMultiSysTypeWindow Set");
                        }
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    
                        break;
                    }
                }
                else
                {
                    is_ps_deactive = TRUE;
                    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                    {
                        if (!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i)
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
							&& MMIAPIPHONE_GetSimStatus(i) != SIM_STATUS_LOCKED_INVALID
#endif
							)
                        {
                            is_ps_deactive = FALSE;
                            break;
                        }
                    }

                    SCI_TRACE_LOW("HandleSetMultiSysTypeWindow is_ps_deactive = %d i = %d", is_ps_deactive, i);

                    if (is_ps_deactive)
                    {
                        is_fly_mode_on = FALSE;
                    
                        MMIAPISET_SetFlyMode(is_fly_mode_on);
                    
                        MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
                    
                        // 激活协议栈
                        MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
                        MMIAPIMMS_RestartAutodownMMS();
                        //SetCallSetOthersItemCheck(list_ctrl,is_fly_mode_on,cur_selection);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                        break;
                    }
                }
            }
            else if(cur_selection == MMI_DUAL_SYS_MAX+1)
            {
               // BOOLEAN  is_fly_on = !is_fly_mode_on;
                
                is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
                
                is_flymode_openphone = !is_flymode_openphone;
                //SetCallSetOthersItemCheck(list_ctrl, is_flymode_openphone, cur_selection);

                MMINV_WRITE(MMINV_FLY_MODE_OPEN_PHONE,&is_flymode_openphone);
            }
            else
            {
                s_is_standby[cur_selection] = !s_is_standby[cur_selection];

                for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if (s_is_standby[i])
                    {
                        is_has_selected = TRUE;
                        break;
                    }
                }

                SCI_TRACE_LOW("HandleSetMultiSysTypeWindow is_has_selected = %d i = %d standby = %d", is_has_selected, i, s_is_standby[cur_selection]);

                if (!is_has_selected || (1 == MMIAPIPHONE_GetSimExistedNum(PNULL, 0) && 1 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0)))
                {
                    s_is_standby[cur_selection] = !s_is_standby[cur_selection];

                    MMIPUB_OpenAlertWarningWin(TXT_SET_STANDBY_MODE_ALERT);
                }
                else
                {
                    if (!ImplementStandbyChanged(s_is_standby,(MN_DUAL_SYS_E)cur_selection))
                    {
                        s_is_standby[cur_selection] = !s_is_standby[cur_selection];

                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    }
                }
            }
        }

        AppendMultiSysTypeList(list_ctrl, TRUE);
//        GUILIST_ValidCurItemPos(list_ctrl);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

     case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        
        MMISET_StopPSApplication();
        
        is_fly_mode_on = TRUE;
        MMIAPISET_SetFlyMode(is_fly_mode_on);
        
        MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
 
#ifdef DCD_SUPPORT                      
        MMIAPIDCD_HandFlyModeSetChange();
#endif
        
        MMIAPIPHONE_DeactivePsAccordingToSetting();
        
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : to handle sim active type when all sim is set to on type
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAnwerWithOriginalSimWindow(
                                                        MMI_WIN_ID_T       win_id, 
                                                        MMI_MESSAGE_ID_E   msg_id, 
                                                        DPARAM             param
                                                        )
{
    
    MMI_RESULT_E            result                                                      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           list_ctrl                                                   =   MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_CTRL_ID;
    int32                   i                                                           =   0;
    BOOLEAN                 is_set                                                      =   FALSE;
    uint16                  list_index_array[MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX]  =   {0};
    uint16                  sel_index_num                                               =   0;
    BOOLEAN                 is_set_array[MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX]       =  {0};
    uint16 index = 0;
    uint16 cur_index = 0;
    const GUILIST_ITEM_T *item_ptr = PNULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        //init list string
        GUILIST_SetMaxItem(list_ctrl, MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX, FALSE );//max item 4
        GUILIST_SetMaxSelectedItem(list_ctrl, MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX);
        MMIAPISET_AppendListItemByTextId(TXT_SET_ORIGIN_REPLY_CALL, STXT_SELECT, list_ctrl, GUIITEM_STYLE_ONE_LINE_CHECK);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        MMIAPISET_AppendListItemByTextId(TXT_SET_ORIGIN_REPLY_MESSAGE, STXT_SELECT, list_ctrl, GUIITEM_STYLE_ONE_LINE_CHECK);
#else
        MMIAPISET_AppendListItemByTextId(TXT_SET_ORIGIN_REPLY_SMS, STXT_SELECT, list_ctrl, GUIITEM_STYLE_ONE_LINE_CHECK);
#ifdef MMS_SUPPORT
        MMIAPISET_AppendListItemByTextId(TXT_SET_ORIGIN_REPLY_MMS, STXT_SELECT, list_ctrl, GUIITEM_STYLE_ONE_LINE_CHECK);
#endif
#endif

#if 0
    MMIAPISET_AppendListItemByTextId(TXT_SET_ORIGIN_REPLY_WAP, STXT_SELECT, list_ctrl, GUIITEM_STYLE_ONE_LINE_CHECK);
#endif

        //set active list item
        for (i = 0; i < MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX; i++)
        {
            is_set = MMIAPISET_GetAnswerWithOriginalSimType(i);
            GUILIST_SetSelectedItem(list_ctrl, (uint16)i, is_set);
        }

        MMK_SetAtvCtrl(win_id, list_ctrl);
        break;

    case MSG_APP_OK:
#if !defined(MMI_PDA_SUPPORT) 
    case MSG_CTL_OK:
#endif
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //save selection
        sel_index_num = GUILIST_GetSelectedItemIndex(list_ctrl, list_index_array, sizeof(list_index_array));
        
        for (i = 0; i < MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX; i++)
        {
            is_set_array[i] = FALSE;
        }
        
        for (i = 0; i < sel_index_num; i++)
        {
            is_set_array[list_index_array[i]] = TRUE;
        }
        
        MMISET_SetAnswerWithOriginalSimType(is_set_array);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : to handle answer with original sim type
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMajorSimWhenAllSimOnWindow(
                                                        MMI_WIN_ID_T       win_id, 
                                                        MMI_MESSAGE_ID_E   msg_id, 
                                                        DPARAM             param
                                                        )
{
    uint16                              cur_selection       =   0;
    MMI_RESULT_E                        result              =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       list_ctrl           =   MMISET_SET_DUALSYS_SIM_ACTIVE_TYPE_CTRL_ID;
    MMISET_MAJOR_SIM_SEL_TYPE_E         e_major_sim_type    =   0;
    uint32                              e_major_sim_func    =   0;
        
    MMI_STRING_T                        title               =   {0};
    BOOLEAN                             is_open             =   FALSE;
    uint32 item_data = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        e_major_sim_func = (uint32)MMK_GetWinAddDataPtr(win_id);
       // SCI_ASSERT(e_major_sim_func < MMISET_MAJOR_SIM_SEL_FUNC_MAX);
        if(e_major_sim_func >= MMISET_MAJOR_SIM_SEL_FUNC_MAX)
        {
             //SCI_TRACE_LOW:"mmisetting_assert HandleSetMajorSimWhenAllSimOnWindow e_major_sim_func %d"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_9968_112_2_18_2_50_38_87,(uint8*)"d", e_major_sim_func);
             return MMI_RESULT_FALSE;
         }         
        //init list string
        if (e_major_sim_func == MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL)
        {
            //speed dial is special, because it won't use manual select function
            GUILIST_SetMaxItem(list_ctrl, MMISET_MAJOR_SIM_SEL_TYPE_MAX - 1, FALSE );//max item 2
            MMIAPISET_AppendMultiSimName(list_ctrl, TXT_COMMON_OK, MMISET_MAJOR_SIM_SEL_TYPE_MAX - 1, MMISET_APPEND_SIM_ALL);
        }
        else
        {
            GUILIST_SetMaxItem(list_ctrl, MMISET_MAJOR_SIM_SEL_TYPE_MAX, FALSE );//max item 3
            MMIAPISET_AppendMultiSimName(list_ctrl, TXT_COMMON_OK, MMISET_MAJOR_SIM_SEL_TYPE_MAX - 1, MMISET_APPEND_SIM_ALL);

//            MMIAPISET_AppendListItemByTextId(TXT_SET_PROMPT_TO_SEL, TXT_COMMON_OK, list_ctrl, GUIITEM_STYLE_ONE_LINE_RADIO);
        }
        
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        
        MMK_SetAtvCtrl(win_id, list_ctrl);
        
        //set active list item
        e_major_sim_type = MMIAPISET_GetMajorSim(e_major_sim_func);
        GUILIST_SetCurItemIndex(list_ctrl, (uint16)e_major_sim_type);
        GUILIST_SetSelectedItem(list_ctrl, (uint16)e_major_sim_type, TRUE);
        
        is_open = FALSE;
        if (MMK_IsOpenWin(MMISET_SET_MULTI_SYS_SETTING_WIN_ID))
        {
           // GUIMENU_GetId(MMISET_SET_MULTI_SIM_SETTING_CTRL_ID, &group_id,&menu_id);
            is_open = TRUE;
        }
        if (MMK_IsOpenWin(MMISET_DUAL_MAJOR_SIM_SEL_WIN_ID))
        {
            // GUIMENU_GetId(MMISET_DUAL_MAJOR_SIM_SEL_CTRL_ID, &group_id,&menu_id);
            is_open = TRUE;
        }
        
        title.wstr_len = 0;
        if (is_open)
        {
            switch (e_major_sim_func)
            {
            case MMISET_MAJOR_SIM_SEL_FUNC_CALL:
                MMI_GetLabelTextByLang(TXT_COMM_CALL, &title);
                break;
                
            case MMISET_MAJOR_SIM_SEL_FUNC_SMS:
                MMI_GetLabelTextByLang(TXT_SET_SMS_SIM_SEL, &title);
                break;
                
            case MMISET_MAJOR_SIM_SEL_FUNC_MMS:
                MMI_GetLabelTextByLang(TXT_MMS, &title);
                break;
                
            case MMISET_MAJOR_SIM_SEL_FUNC_WAP:
                MMI_GetLabelTextByLang(TXT_COMMUNICATION_BROWSER, &title);
                break;
                
            case MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL:
                MMI_GetLabelTextByLang(TXT_COMMON_SPEEDDIAL, &title);
                break;
            case MMISET_MAJOR_SIM_SEL_FUNC_STREAMING:
                MMI_GetLabelTextByLang(TXT_SET_STREAMING, &title);
                break;
            default:
                break;
            }//end switch
        }//end if (is_open)
        
        if (title.wstr_len > 0)
        {
            GUIWIN_SetTitleText(win_id, 
                title.wstr_ptr, 
                title.wstr_len, 
                //title.is_ucs2, 
                FALSE);
        }
        break;
        
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        e_major_sim_func = (uint32)MMK_GetWinAddDataPtr(win_id);
        //SCI_ASSERT(e_major_sim_func < MMISET_MAJOR_SIM_SEL_FUNC_MAX);
        if(e_major_sim_func >= MMISET_MAJOR_SIM_SEL_FUNC_MAX)
        {
             //SCI_TRACE_LOW:"mmisetting_assert HandleSetMajorSimWhenAllSimOnWindow e_major_sim_func %d"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10058_112_2_18_2_50_38_88,(uint8*)"d", e_major_sim_func);
             break;
         }         
        //save selection
        cur_selection = GUILIST_GetCurItemIndex(list_ctrl);
        if(GUILIST_IsCurrItemInvalid(list_ctrl))
        {
            break;
        }
        
        e_major_sim_type = MMISET_GetMajorSimSelType(e_major_sim_func);
        //SCI_TRACE_LOW:"mmiset:HandleSetMajorSimWhenAllSimOnWindow, cur_select=%d, e_major_sim_func=%d, e_major_sim_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10069_112_2_18_2_50_38_89,(uint8*)"ddd", cur_selection, e_major_sim_func, e_major_sim_type);

        GUILIST_GetItemData(list_ctrl, cur_selection, &item_data);
        if(item_data < MMI_DUAL_SYS_MAX) 
        {
            if (item_data != e_major_sim_type)
            {
                //save user selection
                MMISET_SetMajorSimSelType(e_major_sim_func, (MMISET_MAJOR_SIM_SEL_TYPE_E)item_data);
                //Release all pdp link if necessary
                MMIAPIPDP_ReleaseAll(PNULL);
            }
            else
            {
                //do nothing
            }
            //alert success!
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        }
        else
        {
            //SCI_TRACE_LOW:"HandleSetMajorSimWhenAllSimOnWindow,dual_sys[%d]>=MMI_DUAL_SYS_MAX"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10088_112_2_18_2_50_38_90,(uint8*)"d",item_data);
        }
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}


/********************************************************************************
 NAME:      Stk Forbid Input Chars
 DESCRIPTION:
 AUTHOR: Michael.Shi
 DATE:      
********************************************************************************/
LOCAL void SetForbidInputChar(MMI_CTRL_ID_T ctrl_id)
{
    wchar forbid_char_value[]= {0x0d, 0x0a};

    GUIEDIT_ConfigDisableChar(ctrl_id, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
}

#ifdef MMI_MULTISIM_COLOR_SUPPORT
/*****************************************************************************/
//  Description :  Reset button border 
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  ResetSimColorButtonBorder(MN_DUAL_SYS_E   dual_sys)
{
    uint16 index = 0;
    GUI_BORDER_T   btn_border = DEFAULT_BTN_BORDER;/*lint !e572*/
    GUI_BORDER_T   btn_Selected_border = SELECTED_BTN_BORDER;/*lint !e572*/

    for (index=0;index<MAX_SIM_COLOR_NUM;index++)
    {
        GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[index],&btn_border,FALSE);
    }
    //Set used button border
    GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[MMISET_GetSimColorIndex(dual_sys)],&btn_Selected_border,FALSE);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description :  Color button on click
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetBtnIndexByCtrlID(MMI_CTRL_ID_T ctrl_id,uint16* index)
{
    uint16 i = 0;

    for (i=0;i<MAX_SIM_COLOR_NUM;i++)
    {
        if (ctrl_id == simColor_CtrlID.button_ctrl_id[i])
        {
            *index = i;
            return TRUE;
        }
    }

    return FALSE;

}

/*****************************************************************************/
//  Description :  Color button on click
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ColorButtonOnClick(MMI_WIN_ID_T  win_id,MN_DUAL_SYS_E   dual_sys,BOOLEAN is_selected)
{
    MMI_CTRL_ID_T   ctrl_id=0;
    uint16          index = 0;
    GUI_BORDER_T   btn_border = DEFAULT_BTN_BORDER;/*lint !e572*/
    GUI_BORDER_T   btn_Selected_border = SELECTED_BTN_BORDER;/*lint !e572*/
    
   //Get active ctrl id
    ctrl_id= MMK_GetActiveCtrlId(win_id);

//  if (ctrl_id is color button id
    if (GetBtnIndexByCtrlID(ctrl_id,&index))
    {
        if (is_selected)
        {
            MMISET_SetSimColorByIndex(dual_sys,index);  //Set color
            ResetSimColorButtonBorder(dual_sys);        //Reset color button border
        }
        else
        {
            GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[s_current_selected_index],&btn_border,FALSE);            
            s_current_selected_index = index;
            GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[s_current_selected_index],&btn_Selected_border,FALSE);
        }
        return MMI_RESULT_TRUE;
    }

    return MMI_RESULT_FALSE;
}   //ColorButtonOnClick

/*****************************************************************************/
//  Description :  init sim color win 
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E InitEditSimColorWindow(MMI_WIN_ID_T win_id,MN_DUAL_SYS_E dual_sys)
{
    GUI_BG_T btn_bg = {0};
    GUIFORM_CHILD_HEIGHT_T      col_height = {0};
    uint16          i=0,index = 0;
    
    GUIFORM_IsSlide(MMISET_SIM_COLOR_SIM_FORM_CTRL_ID,FALSE);        
    GUIFORM_SetStyle(MMISET_SIM_COLOR_SIM_FORM_CTRL_ID,GUIFORM_STYLE_NORMAL);
    
    btn_bg.bg_type=GUI_BG_COLOR;

    for (i=0;i<SIM_COLOR_BUTTON_LINE_COUNT;i++)
    {
        col_height.type     = GUIFORM_CHILD_HEIGHT_FIXED;
        col_height.add_data = SIM_COLOR_BUTTON_FORM_HEIGHT;
        GUIFORM_SetChildHeight(MMISET_SIM_COLOR_BTN_FORM_CTRL_ID, simColor_CtrlID.button_line_form_id[i], &col_height);
        //Don't use Form border control 
        GUIFORM_PermitChildBorder(simColor_CtrlID.button_line_form_id[i], FALSE);
    }

    //Set button height and bg color
    for (index=0;index<MAX_SIM_COLOR_NUM;index++)  
        {
            btn_bg.color  =MMISET_GetSimColorByIndex(index);
            GUIBUTTON_SetBg(simColor_CtrlID.button_ctrl_id[index],&btn_bg);   //Set BG
            col_height.type     = GUIFORM_CHILD_HEIGHT_FIXED;
            col_height.add_data = SIM_COLOR_BUTTON_RECT_HEIGHT;
            GUIFORM_SetChildHeight(simColor_CtrlID.button_line_form_id[index/4], simColor_CtrlID.button_ctrl_id[index], &col_height);
            IGUICTRL_SetState(MMK_GetCtrlPtr(simColor_CtrlID.button_ctrl_id[index]), GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
        }    
        
    ResetSimColorButtonBorder(dual_sys);    //Reset button border
    
    return MMI_RESULT_TRUE;
}   //InitEditSimColorWindow

/*****************************************************************************/
//  Description :  Handle key move events for select sim color win
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ColorButtonOnkeySelect(MMI_WIN_ID_T win_id,MN_DUAL_SYS_E dual_sys)
{    
    if (s_current_selected_index<MAX_SIM_COLOR_NUM)
    {
        MMISET_SetSimColorByIndex(dual_sys,s_current_selected_index); 
    }

     return TRUE;
}


/*****************************************************************************/
//  Description :  Handle key move events for select sim color win
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ColorButtonOnkeyMove(MMI_WIN_ID_T win_id,MN_DUAL_SYS_E dual_sys,MMI_MESSAGE_ID_E msg_id)
{    
    
    uint16         index = 0;
    GUI_BORDER_T   btn_border = DEFAULT_BTN_BORDER;/*lint !e572*/
    GUI_BORDER_T   btn_Selected_border = SELECTED_BTN_BORDER;/*lint !e572*/
    MMI_CTRL_ID_T   ctrl_id=0;

    
    ctrl_id= MMK_GetActiveCtrlId(win_id);
    if (GetBtnIndexByCtrlID(ctrl_id,&index))
    {
        GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[s_current_selected_index],&btn_border,FALSE);            
        switch (msg_id)
        {
            case MSG_APP_LEFT:
                if (s_current_selected_index>0)
                {
                    s_current_selected_index --;
                }
                break;
            case MSG_APP_RIGHT:
                if (s_current_selected_index<MAX_SIM_COLOR_NUM-1)
                {
                    s_current_selected_index ++;
                }
                break;
            case MSG_APP_UP:
                if (SIM_COLOR_BTN_NUM_ONE_LINE<=s_current_selected_index && 
                    s_current_selected_index<MAX_SIM_COLOR_NUM)
                {
                    s_current_selected_index = s_current_selected_index-SIM_COLOR_BTN_NUM_ONE_LINE;
                 }
                break;
            case MSG_APP_DOWN:
                if (s_current_selected_index<SIM_COLOR_BTN_NUM_ONE_LINE)
                {
                    s_current_selected_index = s_current_selected_index+SIM_COLOR_BTN_NUM_ONE_LINE;
                 }
                break;
            default:
                break;
        }

        if(s_current_selected_index<MAX_SIM_COLOR_NUM)
        {
            GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[s_current_selected_index],&btn_Selected_border,FALSE);
            MMK_SetAtvCtrl(win_id,simColor_CtrlID.button_ctrl_id[s_current_selected_index]);
            return TRUE;
        }
              
    }
    return FALSE;
}

/*****************************************************************************/
//  Description :  HOOK Key events 
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E SimColorWinHookFunc(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result      = MMI_RESULT_FALSE;
    MN_DUAL_SYS_E   dual_sys    = (MN_DUAL_SYS_E)((uint32)MMK_GetWinAddDataPtr(win_id));

    switch(msg_id)
    {
        case MSG_APP_UP:
        case MSG_APP_LEFT:
        case MSG_APP_DOWN:
        case MSG_APP_RIGHT:
        {
            if(ColorButtonOnkeyMove(win_id,dual_sys,msg_id))
            {
                result = MMI_RESULT_TRUE;
            }
        }
        break;
        

    default:
        break;
    }
    
    return  result;
}

/*****************************************************************************/
//  Description : 处理SIM SET Color WIN MESSAGE
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditSimColorWindow(
                                           MMI_WIN_ID_T         win_id,    // 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id,     // 窗口的内部消息ID
                                           DPARAM               param       // 相应消息的参数
                                           )
{
    MMI_RESULT_E            err_code    = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E           dual_sys    = (MN_DUAL_SYS_E)((uint32)MMK_GetWinAddDataPtr(win_id));
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetTitleTextId(win_id,TXT_SET_SIM_COLOR, FALSE);
        InitEditSimColorWindow(win_id,dual_sys);     //Init ctrl settings
        MMK_SetAtvCtrl(win_id,simColor_CtrlID.button_ctrl_id[MMISET_GetSimColorIndex(dual_sys)]);
        s_current_selected_index = MMISET_GetSimColorIndex(dual_sys);
          MMK_SetWinHookFunc(win_id,SimColorWinHookFunc);

        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT            
        ColorButtonOnClick(win_id,dual_sys,FALSE);            //Click is select
#else
        ColorButtonOnClick(win_id,dual_sys,FALSE);            //Click is select
#endif
        MMK_UpdateScreen();
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:

        if (ColorButtonOnkeySelect(win_id,dual_sys))
        {
            MMK_CloseWin(win_id); //Close win in TK mode
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);        
        break;
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;

}   //HandleEditSimColorWindow

#endif
/*****************************************************************************/
//  Description : 处理SIM SET NAME WIN MESSAGE
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditSimNameWindow(
                                           MMI_WIN_ID_T          win_id,     // 窗口的ID
                                           MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                           DPARAM                param       // 相应消息的参数
                                           )
{
    MN_DUAL_SYS_E           dual_sys        =   0;
    MMI_RESULT_E            err_code        =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           edit_ctrl_id    =   MMISET_SET_SIM_NAME_CTRL_ID;
    MMISET_SIM_NAME_T       sim_name        =   {0};
    MMI_STRING_T            edit_string     =   {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetTextMaxLen(edit_ctrl_id,MMISET_MAX_SIM_NAME_LEN/sizeof(wchar), MMISET_MAX_SIM_NAME_LEN);
        
        dual_sys = (uint32)(MMK_GetWinAddDataPtr(win_id));
        
        sim_name = MMIAPISET_GetSimName(dual_sys);
        
        GUIEDIT_SetString(
            edit_ctrl_id,
            sim_name.wstr_arr,
            sim_name.wstr_len
            );

        SetForbidInputChar(edit_ctrl_id);
#if !defined(MMI_PDA_SUPPORT)
        GUIEDIT_SetSoftkey(edit_ctrl_id, 1, 1, TXT_NULL, TXT_COMMON_OK, PNULL);
        GUIEDIT_SetSoftkey(edit_ctrl_id, 2, 1, STXT_RETURN, TXT_COMMON_CLEAR, PNULL);
#endif
        GUIEDIT_SetStyle(edit_ctrl_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
        GUIEDIT_SetFontColor(edit_ctrl_id, MMI_BLACK_COLOR);

        MMK_SetAtvCtrl(win_id, edit_ctrl_id);
        break;

    case MSG_CTL_MIDSK:
        //get the content which the user input
        SCI_MEMSET( &edit_string, 0, sizeof(edit_string) );
        GUIEDIT_GetString(edit_ctrl_id, &edit_string);
        //SCI_ASSERT(edit_string.wstr_len <= MMISET_MAX_SIM_NAME_LEN);
        if(edit_string.wstr_len > MMISET_MAX_SIM_NAME_LEN)
        {
            //SCI_TRACE_LOW:"mmisetting_assert HandleEditSimNameWindow %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10453_112_2_18_2_50_39_91,(uint8*)"d", edit_string.wstr_len);
            break;
        }
        if (edit_string.wstr_len != 0)
        {
            
            sim_name.wstr_len = edit_string.wstr_len;
            MMIAPICOM_Wstrncpy( sim_name.wstr_arr, edit_string.wstr_ptr, sim_name.wstr_len );
            //sim_name.is_ucs2 = edit_string.is_ucs2;
            
            dual_sys = (uint32)(MMK_GetWinAddDataPtr(win_id));
            MMISET_SetSimName(dual_sys, &sim_name);
            
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);           
        }
        else
        {
//             MMIPUB_OpenAlertWarningWin(TXT_COMM_INPUT_NULL);
        }
        break;
    
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        // close the edit window
        MMK_CloseWin(win_id);        
        break;
        
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
    
}

/*****************************************************************************/
//  Description : handle set major sim win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWizardMajorSimSelWindow(
                                                 MMI_WIN_ID_T          win_id,     // 窗口的ID
                                                 MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                                 DPARAM                param       // 相应消息的参数
                                                 )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO            lcd_dev_info    =   {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    BOOLEAN                     is_in_wizard    =   FALSE;
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    is_in_wizard = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:           
        if (is_in_wizard)
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_MAIN_SETTING,TXT_NULL,TXT_NEXT_STEP,FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK,TXT_NULL,STXT_RETURN,FALSE);
        }
        MMK_SetAtvCtrl(win_id, MMISET_DUAL_MAJOR_SIM_SEL_CTRL_ID);
        break;
        
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        GUIMENU_GetId(MMISET_DUAL_MAJOR_SIM_SEL_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_CALL_SIM_SEL:
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_CALL);
            break;
            
        case ID_SET_SMS_SIM_SEL:
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_SMS);
            break;
            
        case ID_SET_MMS_SIM_SEL:
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_MMS);
            break;
            
        case ID_SET_BROWSER_SIM_SEL:
            //SCI_TRACE_LOW:"mmiset: HandleMultiSimSettingWindow wap"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10546_112_2_18_2_50_39_92,(uint8*)"");
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_WAP);
            break;
            
        case ID_SET_SPEED_DIAL:
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL);
            break;
            
            
        case ID_SET_STREAMING_SIM_SEL:
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_STREAMING);
            break;
            
        case ID_SET_ANSWER_USING_ORIGINAL_SIM:
            MMK_CreateWin((uint32 *)MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_TAB, (ADD_DATA)PNULL);
            break;
            
        default:
            //SCI_PASSERT(0,("HandleWizardMajorSimSelWindow menu_id = %d", menu_id));
            break;
        }
        break;
        
        case MSG_APP_CANCEL:     
        case MSG_CTL_CANCEL:
#ifdef MMIPB_SIMDN_SUPPORT
            if (is_in_wizard)
            {
                MMIAPIPB_OpenMSISDNWin(is_in_wizard, FALSE);
            }
#endif
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;      
}

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WizardCFNRCSIMSelectCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;

#if defined(MMI_MULTI_SIM_SYS_DUAL)
    if (isReturnOK)
    {
        s_is_wizard_cfnrc = TRUE;
#ifdef  MMI_SMS_VOICE_MAIL_SUPPORT       
        MMK_CreateWin((uint32*)MMISET_SET_OPEN_DIVERT_WIN_TAB,(ADD_DATA)dual_sys);
#else
        MMIAPISET_SetSysForCf(dual_sys);/*lint !e64*/
        OpenSetCallDivertWin((MN_DUAL_SYS_E)dual_sys);
#endif
    }
    else
    {
        s_is_wizard_cfnrc = FALSE;
    }
#endif 
    return (recode);

}


/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMSelectWinCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    uint32              select_type     =   (uint32)(MMK_GetWinAddDataPtr(MMK_GetFocusWinId()));

    if (isReturnOK)
    {
        s_cur_dual_sys = (MN_DUAL_SYS_E)((uint32)dual_sys);

        DispatchBySelectType((MN_DUAL_SYS_E)s_cur_dual_sys, select_type);
        if (MMISET_CALL_WAITING == select_type) //if MMISET_CALL_WAITING,don't close window
        {
            return MMI_RESULT_FALSE;
        }
        return (recode);
    }
    return recode;
}

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void DispatchBySelectType(MN_DUAL_SYS_E dual_sys, MMISET_SIM_SELECT_TYPE_E select_type)
{
#ifdef VT_SUPPORT
    uint32 win_id = MMISET_CALLDIVERT_FORM_CTRL_ID;
#endif

    switch(select_type) 
    {
case MMISET_CALL_FORWARD:
#ifdef VT_SUPPORT
        MMK_CreateWin((uint32*)MMISET_SET_DIVERT_AUDIOORVIDEO_CALL_WIN_TAB, (ADD_DATA)win_id);

#else
        MMK_CreateWin((uint32*)MMISET_SET_CALL_DIVERT_WIN_TAB, (ADD_DATA)dual_sys);
#endif
        break;
        
    case MMISET_CALL_BAR:
#ifdef VT_SUPPORT
                win_id = MMISET_CALLBAR_FORM_CTRL_ID;
                MMK_CreateWin((uint32*)MMISET_SET_BAR_AUDIOORVIDEO_CALL_WIN_TAB, (ADD_DATA)win_id);
#else
        MMK_CreateWin((uint32*)MMISET_SET_CALL_BAR_WIN_TAB, (ADD_DATA)dual_sys);
#endif
        break;
        
    case MMISET_CALL_WAITING:
        CreateCallWaitingtSelectPopWindow();
        break;
        
    case MMISET_CALL_HIDE_ID:
        MMK_CreatePubListWin((uint32*)MMISET_SET_CALL_HIDE_ID_WIN_TAB, (ADD_DATA)dual_sys);
        break;

    case MMISET_CALL_VOICEMAIL:
        GetVoiceMailAddress(dual_sys, MN_MULTI_SUBSCRIBER_ID_1,  MN_VOICE_MAIL);
        break;

    case MMISET_SET_NETWORK:
        MMIAPISET_OpenSelNetWorkWin(dual_sys);
        break;

#ifdef MMI_PREFER_PLMN_SUPPORT
    case MMISET_SET_PREFER_NETWORK:
        MMK_CreateWin((uint32*)MMISET_SET_DISP_PREFER_NETWORK_WIN_TAB, (ADD_DATA)dual_sys);
        break;
#endif

#ifdef MMI_IP_CALL_SUPPORT
    case MMISET_IP_CALL:
        MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)dual_sys);
        break;
#endif
#ifdef FLIP_PHONE_SUPPORT        
    case MMISET_FLIP_ANSWER:
        MMK_CreateWin((uint32*)MMISET_SET_FLIP_ANSWER_WIN_TAB, (ADD_DATA)dual_sys);
        break;
#endif       
#ifdef MMI_CSP_SUPPORT        
    case MMISET_LINE_SELECT:
        MMK_CreatePubListWin((uint32*)MMISET_SELECT_LINE_WIN_TAB,(ADD_DATA)dual_sys);
        break;

    case MMISET_LINE_LOCK:
        MMIAPIPHONE_SetCurHandlingPinSimID( dual_sys );
        MMIAPIPHONE_HandlePinAndPukFlow(
            MNSIM_PIN_VERIFY,
            MNSIM_PIN2,
            (uint32)MMISET_LOCK_LINE_WIN_TAB,
            FALSE);        
        //MMK_CreatePubListWin((uint32*)MMISET_LOCK_LINE_WIN_TAB,(ADD_DATA)dual_sys);
        break;      
#endif //     MMI_CSP_SUPPORT   


#ifdef MMI_AUDIO_PLAYER_SUPPORT        
    case MMISET_MP3_PLAYER_CALL_RING:
        MMIAPIMP3_SetMp3AsCallRing(dual_sys);
        break; 
#endif

    case MMISET_SET_PIN:
        MMISET_OpenPinWin(dual_sys);
        break;
        
    case MMISET_SET_PIN2_MODIFY:
        MMISET_OpenPin2Win(dual_sys);
        break;
        
#ifdef FDN_SUPPORT

    case MMISET_SET_FDN:
        MMIAPIFDN_SetCurSIM(dual_sys);
        MMISET_OpenFDNWin(dual_sys);
        break;

#endif
    case MMISET_MULTI_SIM_NAME:
        MMK_CreatePubEditWin((uint32 *)MMISET_SET_SIM_NAME_WIN_TAB, (ADD_DATA)dual_sys);
        break;
#ifdef MMI_MULTISIM_COLOR_SUPPORT
    case MMISET_MULTI_SIM_COLOR:
        MMK_CreatePubFormWin((uint32*)MMISET_SET_SIM_COLOR_WIN_TAB,(ADD_DATA)dual_sys);
        //MMK_CreateWin((uint32*)MMISET_SET_SIM_COLOR_WIN_TAB,(ADD_DATA)dual_sys);
        break;
#endif        
    default:
        //SCI_TRACE_LOW:"mmiset_wintab.c:DispatchBySelectType, dual_sys = %d, select_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10802_112_2_18_2_50_40_93,(uint8*)"dd", dual_sys, select_type);
        break;
    }
}

/*****************************************************************************/
//  Description : INIT CONTROL
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void InitMultiSimSettingControl(void)
{
    MMI_STRING_T label_name  = {0};
    MMI_GetLabelTextByLang(TXT_SIM_ENABLE_SEL, &label_name);
    GUILABEL_SetText(MMISET_MUTISIM_IDLEMODE_LABEL_CTRL_ID, &label_name, FALSE);
//    MMI_GetLabelTextByLang(TXT_SET_REPLY_BY_ORIGINAL_SIM, &label_name);
//    GUILABEL_SetText(MMISET_MUTISIM_SIM_REPLY_LABEL_CTRL_ID, &label_name, FALSE);
    MMI_GetLabelTextByLang(TXT_SET_SIM_NAME, &label_name);
    GUILABEL_SetText(MMISET_MUTISIM_SET_NAME_LABEL_CTRL_ID, &label_name, FALSE);
}

#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : InitPdaMultiSimSettingCtrl
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaMultiSimSettingCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_MULTI_SIM_SETTING_CTRL_ID;
    uint32 i = 3;
#ifdef MMI_MULTISIM_COLOR_SUPPORT
    ++i;
#endif
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    if (MMIPHONE_IsSimOk(MN_DUAL_SYS_1) && MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
    {
        i += 1;//"setting" and "help" menu is 2
    }
#endif

    GUILIST_SetMaxItem(ctrl_id, i, FALSE );
    
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SIM_ENABLE_SEL, IMAGE_COMMON_RIGHT_ARROW, ID_SET_MULTISYS_SIM_ONOFF_TYPE);
    //AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_REPLY_BY_ORIGINAL_SIM, IMAGE_COMMON_RIGHT_ARROW, ID_SET_ANSWER_USING_ORIGINAL_SIM);
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_SIM_NAME, IMAGE_NULL, ID_SET_SIM_NAME);
//     AppendPdaCallSettingsItem1Line(ctrl_id, TXT_MAJOR_SIM_SEL, IMAGE_NULL, ID_SET_MAJORSIM_SELECT);

#ifdef MMI_MULTISIM_COLOR_SUPPORT
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_SIM_COLOR, IMAGE_NULL, ID_SET_SIM_COLOR);
#endif    
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    if (MMIPHONE_IsSimOk(MN_DUAL_SYS_1) && MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
    {
        //AppendPdaCallSettingsItem1Line(ctrl_id, TXT_DUAL_SYS_CFNRC, IMAGE_NULL, ID_SET_DUALSIM_CALL_DIVERT);//delete by James li
        //AppendPdaCallSettingsItem1Line(ctrl_id, TXT_HELP, IMAGE_NULL, ID_SET_DUALSIM_CALL_DIVERT_HELP);
    }
#endif    

}

/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMultiSimSettingWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_MENU_ID_T             menu_id = 0;
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    uint16 index = 0;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_MULTI_SIM_SETTING_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#if defined(MMI_MULTI_SIM_SYS_DUAL)
        GUIWIN_SetTitleTextId(win_id,TXT_SET_DUAL_SIM_SETTING,FALSE);
#else
        GUIWIN_SetTitleTextId(win_id,TXT_SET_MULTI_SIM_SETTING,FALSE);
#endif
        InitPdaMultiSimSettingCtrl(); 
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_GET_FOCUS:
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &menu_id);
        switch (menu_id)
        {
        case ID_SET_MULTISYS_SIM_ONOFF_TYPE:
            //MMIAPISET_OpenMultiSimSettingsWindow();
            MMIAPISET_OpenMultiSimStandbySettingsWindow();
            break;

        case ID_SET_ANSWER_USING_ORIGINAL_SIM:
            MMK_CreatePubListWin((uint32 *)MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_TAB, (ADD_DATA)PNULL);
            break;
//         case ID_SET_MAJORSIM_SELECT:
//             MMK_CreateWin((uint32 *)MMISET_DUAL_MAJOR_SIM_SEL_WIN_TAB,PNULL);
//             break;
        case ID_SET_SIM_NAME:
            MMIAPISET_OpenUssdSelectWin(MMISET_MULTI_SIM_NAME);
            break;
#ifdef MMI_MULTISIM_COLOR_SUPPORT            
        case ID_SET_SIM_COLOR:
            MMIAPISET_OpenUssdSelectWin(MMISET_MULTI_SIM_COLOR);
            break;
#endif            
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
        case ID_SET_DUALSIM_CALL_DIVERT:
            MMIAPISET_SetDualSIMCFNRC(MMISET_SET_DUALSYS_CFNRC_WIN_ID);//CFNRC win
            break;
#if 0            
        case ID_SET_DUALSIM_CALL_DIVERT_HELP:
            MMK_CreateWin((uint32 *)MMISET_SET_DUALSYS_DIVERT_HELP_WIN_TAB, NULL);
            break;
#endif
#endif            
        default:
            break;
        }
        break;
    case MSG_APP_OK:
    case MSG_APP_WEB:
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
#else
/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMultiSimSettingWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_MENU_GROUP_ID_T       group_id = 0;
    MMI_MENU_ID_T             menu_id = 0;
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
 /*       InitMultiSimSettingControl();
        GUIWIN_SetTitleTextId(win_id,TXT_SET_DUAL_SIM_SETTING,FALSE);
        MMK_SetAtvCtrl(win_id, MMISET_MUTISIM_IDLEMODE_LABEL_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN, FALSE);*/
#if defined(MMI_MULTI_SIM_SYS_DUAL)
        GUIWIN_SetTitleTextId(win_id,TXT_SET_DUAL_SIM_SETTING,FALSE);
#else
        GUIWIN_SetTitleTextId(win_id,TXT_SET_MULTI_SIM_SETTING,FALSE);
#endif

        if (MMIAPISET_GetMultiSysSettingNum(NULL, 0) > 1)
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_SET_MULTI_SIM_SETTING, ID_SET_MAJORSIM_SELECT, FALSE);
        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_SET_MULTI_SIM_SETTING, ID_SET_MAJORSIM_SELECT, TRUE);
        }
        MMK_SetAtvCtrl(win_id, MMISET_SET_MULTI_SIM_SETTING_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
    case MSG_GET_FOCUS:
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        GUIMENU_GetId(MMISET_SET_MULTI_SIM_SETTING_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_MULTISYS_SIM_ONOFF_TYPE:
            //MMIAPISET_OpenMultiSimSettingsWindow();
            MMIAPISET_OpenMultiSimStandbySettingsWindow();
            break;

        case ID_SET_ANSWER_USING_ORIGINAL_SIM:
            MMK_CreatePubListWin((uint32 *)MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_TAB, (ADD_DATA)PNULL);
            break;
            
        case ID_SET_SIM1_NAME:
            MMK_CreatePubEditWin((uint32 *)MMISET_SET_SIM_NAME_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_1);
            break;

        case ID_SET_SIM2_NAME:
            MMK_CreatePubEditWin((uint32 *)MMISET_SET_SIM_NAME_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_2);
            break;

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        case ID_SET_SIM3_NAME:
            MMK_CreatePubEditWin((uint32 *)MMISET_SET_SIM_NAME_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_3);
            break;

#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case ID_SET_SIM4_NAME:
            MMK_CreatePubEditWin((uint32 *)MMISET_SET_SIM_NAME_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_4);
            break;
#endif/* MMI_MULTI_SIM_SYS_QUAD */
#endif/*  MMI_MULTI_SIM_SYS_QUAD ||  MMI_MULTI_SIM_SYS_TRI*/

        default:
            break;
        }
        break;
    case MSG_APP_OK:
    case MSG_APP_WEB:
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
#endif

/*****************************************************************************/
//  Description : to open multi sys call settings standby menu window
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenMultiSimStandbySettingsWindow(void)
{
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
    if ((MMIAPIPB_IsPbReady()
        && MMIAPISMS_IsOrderOk()))
    {
        MMK_CreatePubListWin((uint32*)MMISET_CTA_MULTI_SYS_STANDBY_WIN_TAB, (ADD_DATA)FALSE);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
    }
#else
    if (MMK_IsOpenWin(MMISET_SET_MULTISYS_TYPE_WIN_ID))
    {
        MMK_WinGrabFocus(MMISET_SET_MULTISYS_TYPE_WIN_ID);
    }
    else
    {
        MMK_CreateWin((uint32*)MMISET_SET_MULTISYS_TYPE_WIN_TAB, PNULL);
    }
#endif
}

/*****************************************************************************/
//  Description : to open multi sys call settings main menu window
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenMultiSimSettingsWindow(void)
{
    MMISET_EnterMultiSimSettingMenu();
}

/*****************************************************************************/
//  Description : AppendPinSelectListItem
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AppendPinSelectListItem(MMISET_SIM_SELECT_TYPE_E select_type, MMI_CTRL_ID_T ctrl_id)
{
    int32                   i           =   0;
    MMISET_SIM_NAME_T       sim_str     =   {0};
    MN_DUAL_SYS_E           dual_sys    =   MN_DUAL_SYS_1;
    GUILIST_ITEM_T          item_t      =   {0};
    GUILIST_ITEM_DATA_T     item_data   =   {0};
    SIM_STATUS_E            sim_status  =   SIM_STATUS_OK;
    MMI_IMAGE_ID_T          image_id    =   IMAGE_NULL;
    
    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX, FALSE);//max item 2
    
    item_t.item_style  = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)TXT_COMMON_OK;
    item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        switch(select_type)
        {
        case MMISET_SET_PIN:
            sim_status = MMIAPIPHONE_GetSimStatus(i);
            if ((SIM_STATUS_OK == sim_status || SIM_STATUS_REGISTRATION_FAILED == sim_status) && MNSIM_IsPin1EnableEx(i))
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
            break;
            
        default:
            image_id = IMAGE_CHECK_UNSELECTED_ICON;
            break;
        }
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = image_id;
        
        dual_sys = (MN_DUAL_SYS_E)i;
        sim_str = MMIAPISET_GetSimName(dual_sys);
        
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = sim_str.wstr_len;
        
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = sim_str.wstr_arr;
        
        //         MMIAPICOM_Wstrncpy( item_data.item_content[1].item_data.text_buffer.wstr,
        //             sim_str.wstr_arr,
        //             item_data.item_content[1].item_data.text_buffer.wstr_len);
        
        //item_data.item_content[1].item_data.text_buffer.is_ucs2 = sim_str.is_ucs2;
        
        GUILIST_AppendItem(ctrl_id,&item_t);
    }
}
#endif  //MMI_MULTI_SIM_SYS_SINGLE

#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
//  Description : open dual sim set cfnrc window
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL void OpenDualsysCFNRCWin(void)
{
    MMI_STRING_T    connect_net_text    =   {0};
    
    s_dualsys_cfnrc_win_close_time = 0;
    s_dualsys_cfnrc_success_num = 0;
    s_is_action_dualsys_cfnrc = TRUE;
    MMI_GetLabelTextByLang(TXT_NETWORK_CONNECT, &connect_net_text);
    
    MMIPUB_OpenWaitWin(
        1,
        &connect_net_text,
        PNULL,
        PNULL,
        MMISET_SET_DUALSYS_CFNRC_WIN_ID,
        NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL,
        MMIPUB_SOFTKEY_ONE,
        HandleDualsysCFNRCWindow
        );
}

/*****************************************************************************/
//  Description : close dual sim set cfnrc window
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseDualsysCFNRCWin(MN_DUAL_SYS_E dual_sys, BOOLEAN  is_action_success, MN_SS_ERROR_CODE_E  error_code)
{
    #ifdef MMI_SMART_DUAL_SIM_SUPPORT
    if(SmartDS_Forwarding_setting != SMARTDS_FORWARDING_NONE)
    {
        MMISET_CloseSmartDualSIMCFNRCWin(dual_sys, is_action_success, error_code);
        return;
    }
    #endif
    if (s_is_action_dualsys_cfnrc)
    {
        if (is_action_success)
        {
            s_dualsys_cfnrc_success_num++;
        }
        else //failed notify which SIM error, and check SIM or CFNRC whether support
        {
            MMI_TEXT_ID_T sim_text_id[] = {
                                                                    TXT_SIM_SEL_SIM1,
                                                                    TXT_SIM_SEL_SIM2,
                                                                    TXT_SIM_SEL_SIM3,
                                                                    TXT_SIM_SEL_SIM4, 
                                                                 }; 
            MMI_STRING_T  sim_string ={0};
            MMI_STRING_T  prompt_text_string = {0};
            MMI_STRING_T  prompt_string = {0};
            wchar blank_wchar[] = {0x20,0x00};
            MN_DUAL_SYS_E another_sim_index = MMI_DUAL_SYS_MAX;
            MMI_STRING_T  another_sim_string ={0};
            MMI_STRING_T  setting_string ={0};
            
            s_dualsys_cfnrc_win_close_time = 0;
            s_dualsys_cfnrc_success_num = 0;
            s_is_action_dualsys_cfnrc = FALSE;
            another_sim_index = (MN_DUAL_SYS_1 == dual_sys)? MN_DUAL_SYS_2: MN_DUAL_SYS_1;
            
            MMI_GetLabelTextByLang(sim_text_id[dual_sys], &sim_string);
            MMI_GetLabelTextByLang(TXT_SET_DUALSIM_DIVERT_FAIL_PROMPT, &prompt_text_string);
            MMI_GetLabelTextByLang(STXT_MAIN_SETTING, &setting_string);
            
            if(MN_SS_ERROR_CALL_BARRED == error_code
                ||MN_SS_ERROR_UNEXPECTED_DATA_VALUE == error_code)
            {
                MMI_STRING_T  msisdn_string ={0};
                
                MMI_GetLabelTextByLang(TXT_PB_MSISDN, &msisdn_string);
                MMI_GetLabelTextByLang(sim_text_id[another_sim_index], &another_sim_string);
                
                prompt_string.wstr_ptr = SCI_ALLOCAZ((sim_string.wstr_len + prompt_text_string.wstr_len + another_sim_string.wstr_len + msisdn_string.wstr_len + setting_string.wstr_len+ 5 ) * sizeof(wchar));
                if(PNULL == prompt_string.wstr_ptr) 
                {
                    return;
                }
                prompt_string.wstr_len = sim_string.wstr_len + prompt_text_string.wstr_len + another_sim_string.wstr_len + msisdn_string.wstr_len + setting_string.wstr_len+4;
                
                MMIAPICOM_Wstrncpy(prompt_string.wstr_ptr, sim_string.wstr_ptr, sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, prompt_text_string.wstr_ptr, prompt_text_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, another_sim_string.wstr_ptr, another_sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, msisdn_string.wstr_ptr, msisdn_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, setting_string.wstr_ptr, setting_string.wstr_len);

                //Alert "SIM1 Set Failed, Please check SIM2 Own Number Setting"
                MMIPUB_OpenQueryWinByTextPtr(&prompt_string, PNULL, IMAGE_PUBWIN_QUERY, PNULL, HandleEnterEditMSISDNQueryWin);
            }
            else
            {
                MMI_STRING_T  cfnrc_string ={0};
                uint32 time_period = 0;
                
                MMI_GetLabelTextByLang(TXT_SET_DIVERTS_NOT_REACHABLE, &cfnrc_string);
                
                prompt_string.wstr_ptr = SCI_ALLOCAZ((sim_string.wstr_len + prompt_text_string.wstr_len + sim_string.wstr_len + cfnrc_string.wstr_len + setting_string.wstr_len + 5 ) * sizeof(wchar));
                if(PNULL == prompt_string.wstr_ptr) 
                {
                    return;
                }
                prompt_string.wstr_len = sim_string.wstr_len*2 + prompt_text_string.wstr_len + cfnrc_string.wstr_len + setting_string.wstr_len +4;

                MMIAPICOM_Wstrncpy(prompt_string.wstr_ptr, sim_string.wstr_ptr, sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, prompt_text_string.wstr_ptr, prompt_text_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, sim_string.wstr_ptr, sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, cfnrc_string.wstr_ptr, cfnrc_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, setting_string.wstr_ptr, setting_string.wstr_len);
                
                //Alert "SIM1 set failed, Please Check SIM1 Call Divert unreachable setting"
                MMIPUB_OpenAlertWinByTextPtr(&time_period, &prompt_string, PNULL, IMAGE_PUBWIN_WARNING,
                                                 PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            
            SCI_FREE(prompt_string.wstr_ptr);
            MMK_CloseWin(MMISET_SET_DUALSYS_CFNRC_WIN_ID);
            return;
        }
        
        s_dualsys_cfnrc_win_close_time++;
        
        //sim1注册完毕后sim2开始注册，避免注册时间间隔太短，注册失败的情况
        if (1 == s_dualsys_cfnrc_win_close_time)
        {
            if (MMISET_RegisterDualsysCFNRC(MN_DUAL_SYS_2))
            {
                //SCI_TRACE_LOW:"mmiset_wintab.c:MMISET_CloseDualsysCFNRCWin,True"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11324_112_2_18_2_50_41_94,(uint8*)"");
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            }
            //SCI_TRACE_LOW:"mmiset_wintab.c:MMISET_CloseDualsysCFNRCWin:s_dualsys_cfnrc_win_close_time = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11330_112_2_18_2_50_41_95,(uint8*)"d", s_dualsys_cfnrc_win_close_time);
        }
        //SCI_TRACE_LOW:"mmiset:CloseDualsysCFNRCWin, s_dualsys_cfnrc_win_close_time=%d, action_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11332_112_2_18_2_50_41_96,(uint8*)"dd", s_dualsys_cfnrc_win_close_time,is_action_success);
        
        if (s_dualsys_cfnrc_win_close_time >= MMI_DUAL_SYS_MAX)
        {
            MMK_CloseWin(MMISET_SET_DUALSYS_CFNRC_WIN_ID);
            
            if (s_dualsys_cfnrc_success_num >= MMI_DUAL_SYS_MAX)
            {
                MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            }
            s_dualsys_cfnrc_win_close_time = 0;
            s_dualsys_cfnrc_success_num = 0;
            s_is_action_dualsys_cfnrc = FALSE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISET_CloseDualsysCFNRCWin: is not dual sys, do not handle!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11353_112_2_18_2_50_41_97,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : 处理dual sim set cfnrc
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDualsysCFNRCWindow(
                                            MMI_WIN_ID_T          win_id,     // 窗口的ID
                                            MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                            DPARAM                param       // 相应消息的参数
                                            )
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
//         case MSG_OPEN_WINDOW:
//         {
            //MMIPUB_SetWinSoftkey(win_id, PNULL, MMI_TEXT_ID_T right_sk_id, BOOLEAN is_fresh)
            //MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
            //if(NULL != win_info_ptr)win_info_ptr->softkey_id[2] = IMAGE_COMMON_CANCEL;
//             GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_CANCEL, 2, FALSE);
//         }
//             break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        if (MNSS_GetCurStateEx(g_mmiset_ss_status.dual_sys))
        {
            MNSS_RelActivingServiceEx(g_mmiset_ss_status.dual_sys);
        }
        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : open cfnrc win
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL void OpenCFNRCWin(void)
{
#if defined(MMI_MULTI_SIM_SYS_DUAL)
    //judge whether dual sys all call read
    if (MMIPHONE_IsSimOk(MN_DUAL_SYS_1) && MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
    {
        //judge whether dual sys msisdn all ready
        if (MMIAPIPB_IsPbReady())
        {
#if defined(SFR_SUPPORT) && defined(SFR_SUPPORT_CTCC)
            if (MMIAPICONNECTION_isGPRSSwitchedOff())
            {
                MMIPUB_OpenAlertWarningWin(TXT_SYS_OPEN_DATA_SERVICE);
                return;
            }
#endif
            if (MMISET_RegisterDualsysCFNRC(MN_DUAL_SYS_1))
            {
                OpenDualsysCFNRCWin();
            }
            else
            {
                //alert fail
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            }
        }
        else
        {
            //alert set pb msisdn first
            MMIPUB_OpenAlertWarningWin(TXT_SET_PB_MSISDN);
        }
    }
    else
    {
        //alert sim not ready
        MMIPUB_OpenAlertWarningWin(TXT_SIM_ISNOT_READY);
    }
#endif
}
#endif


#ifdef MMI_DUALMODE_ENABLE 
/*****************************************************************************/
//  Description : MMISET_AppendNetworkTypeListItem
//  Global resource dependence : 
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void AppendNetworkTypeListItem(
                                     MMI_CTRL_ID_T        ctrl_id
                                     )
{
    
    NETWORK_TYPE_E      network_type    =   0;/*lint !e64*/
    
    //add items to listbox
    #ifdef PLATFORM_SC7702
    MMIAPISET_AppendListItemByTextId(TXT_SET_W_PREFER, TXT_COMMON_OK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);    
    #else
    MMIAPISET_AppendListItemByTextId(TXT_SET_TD_PREFER, TXT_COMMON_OK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);    
    #endif
    MMIAPISET_AppendListItemByTextId(TXT_COMM_GSM_PREFER, STXT_SOFTKEY_OK_MK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);   
    MMIAPISET_AppendListItemByTextId(TXT_SET_GSM, TXT_COMMON_OK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);//@zhaohui modify for cr94108
    #ifdef PLATFORM_SC7702
    MMIAPISET_AppendListItemByTextId(TXT_SET_WCDMA, TXT_COMMON_OK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    #else
    MMIAPISET_AppendListItemByTextId(TXT_SET_TDSCDMA, TXT_COMMON_OK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    #endif
    
    network_type = GetNetworkType();
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, network_type, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,network_type);
    
}
#endif
/*****************************************************************************/
//  Description : GetNetworkType
//  Global resource dependence : none
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
PUBLIC NETWORK_TYPE_E GetNetworkType(void)//@zhaohui modify for cr94108
{ 
    NETWORK_TYPE_E                  net_type    =   0;/*lint !e64*/
#ifdef MMI_DUALMODE_ENABLE 
    MN_GMMREG_RAT_E        gmmreg_rat  =   MN_GMMREG_RAT_GPRS;
    
#ifndef _WIN32
    gmmreg_rat = MNPHONE_GetConfiguredRAT();//user selected rat
#endif
    //SCI_TRACE_LOW:"MMISET_GetNetworkType gmmreg_rat is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11467_112_2_18_2_50_41_98,(uint8*)"d",gmmreg_rat);
    switch(gmmreg_rat) 
    {
    case MN_GMMREG_RAT_GPRS:
        net_type = NETWORK_TYPE_GPRS_E;
        break;
        
    case MN_GMMREG_RAT_3G:
        net_type = NETWORK_TYPE_TD_E;
        break;
        
    case MN_GMMREG_RAT_GPRS_AND_3G:
#ifndef _WIN32
        if(MN_GMMREG_RAT_GPRS == MNPHONE_GetConfiguredDMPreferRAT())
        {
            net_type = NETWORK_TYPE_DM_G_E;
        }
        else
#endif
        {
            net_type = NETWORK_TYPE_DM_E;
        }
        break;
        
    default:
        break;
    }
    
#endif
    return net_type;
}
#ifdef MMI_DUALMODE_ENABLE 
/*****************************************************************************/
//  Description : MMIAPISET_SetNetworkType
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetNetworkType(MN_GMMREG_RAT_E rat)
{
    NETWORK_TYPE_E network_type = NETWORK_TYPE_DM_E;

    //SCI_TRACE_LOW:"MMIAPISET_SetNetworkType rat=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11507_112_2_18_2_50_41_99,(uint8*)"d", rat);

    switch (rat)
    {
    case MN_GMMREG_RAT_GPRS:
        network_type = NETWORK_TYPE_GPRS_E;
        break;

    case MN_GMMREG_RAT_GPRS_AND_3G:
        network_type = NETWORK_TYPE_DM_E;
        break;

    case MN_GMMREG_RAT_3G:
        network_type = NETWORK_TYPE_TD_E;
        break;

    default:
        break;
    }

    SetNetworkType(network_type, FALSE, FALSE, FALSE);
}



/*****************************************************************************/
//  Description : MMISET_SetNetworkType
//  Global resource dependence : none
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void SetNetworkType(//@zhaohui modify for cr94108
                          NETWORK_TYPE_E network_type, //用户的选择
                          BOOLEAN is_resend,        //是否是失败后的重发
                          BOOLEAN only_write_nv,
                          BOOLEAN is_reset_factorysetting
                          )
{       
    MN_PHONE_RAT_INFO_T  rat_info = {0};/*lint !e64*/
    NETWORK_TYPE_E  last_network_type  = GetNetworkType();
    NETWORK_TYPE_E  cur_network_type = NETWORK_TYPE_GPRS_E;
    ERR_MNPHONE_CODE_E  ret  = ERR_MNPHONE_WRITE_ME_FAILURE;//@zhaohui,cr106675
    LOCAL MN_PHONE_RAT_INFO_T s_rat_info_resend = {0};/*lint !e64*/
    LOCAL NETWORK_TYPE_E s_network_type_resend = NETWORK_TYPE_GPRS_E;
#ifndef MMI_NET_TYPE_SET_DEFAULT    
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
    
    //cur_selection_netwok    =   GUILIST_GetCurItemIndex(MMISET_SET_NETWORK_TYPE_CTRL_ID);
    if(!is_resend)
    {
        //SCI_TRACE_LOW:"SetNetworkType cur_selection_netwoktype is %d, only_write_nv is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11553_112_2_18_2_50_41_100,(uint8*)"dd",network_type, only_write_nv);
#ifndef _WIN32
        switch(network_type) 
        {
        case NETWORK_TYPE_DM_E:
            cur_network_type = NETWORK_TYPE_DM_E;
            rat_info.rat = MN_GMMREG_RAT_GPRS_AND_3G;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_3G;
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;

        case NETWORK_TYPE_DM_G_E:
            cur_network_type = NETWORK_TYPE_DM_G_E;
            rat_info.rat = MN_GMMREG_RAT_GPRS_AND_3G;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_GPRS;
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;            
            
        case NETWORK_TYPE_GPRS_E:
            cur_network_type = NETWORK_TYPE_GPRS_E;
            rat_info.rat = MN_GMMREG_RAT_GPRS;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_UNKNOWN;
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;
            
        case NETWORK_TYPE_TD_E:
            cur_network_type = NETWORK_TYPE_TD_E;
            rat_info.rat = MN_GMMREG_RAT_3G;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_UNKNOWN;
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;
            
        default:
            
            break;
        }
#endif  
    }
    else
    {
        rat_info.rat = s_rat_info_resend.rat;
        rat_info.dm_prefer_rat = s_rat_info_resend.dm_prefer_rat;
        cur_network_type = s_network_type_resend;
    }
    
    //SCI_TRACE_LOW:"SetNetworkType %d %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11598_112_2_18_2_50_41_101,(uint8*)"dd",cur_network_type, last_network_type);
    

    
    if (cur_network_type != last_network_type)
    {
#ifdef MMI_DUALMODE_ENABLE
        //avoid frequently switch network
        if (!s_allow_rat_switching)
        {    
            MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
            //not use continue, for we should not allow switch if busy
            return;
        }
#endif
        //note: it's necessary to set gprs attach mode before setting rat.
        MMIAPIPHONE_SetGPRSAttachMode();
        
#ifndef _WIN32
        {
            
#ifndef MMI_NET_TYPE_SET_DEFAULT
            for(dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
            {
                if (MMIAPIPHONE_IsSimCardOk(dual_sys))
                {
                    ret = MNPHONE_SetRATEx(dual_sys, rat_info, FALSE, only_write_nv);
                    SCI_TRACE_LOW("SetNetworkType, dual_sys=%d, ret= %d, only_write_nv = %d", dual_sys, ret, only_write_nv);
                }
                
            }
#else
            ret = MNPHONE_SetRATEx(MN_DUAL_SYS_1, rat_info, FALSE, only_write_nv);
            SCI_TRACE_LOW("SetNetworkType,  ret= %d, only_write_nv = %d", ret, only_write_nv);
#endif
        }
#endif

        if (!only_write_nv)
        {
            if(ERR_MNPHONE_NO_ERR == ret)/*lint !e774 */
            {
                if (!is_resend) //设置成功之后，如果是首次发送，记录下发送的参数
                {
                    s_rat_info_resend.rat = rat_info.rat;
                    s_rat_info_resend.dm_prefer_rat = rat_info.dm_prefer_rat;
                    s_network_type_resend = cur_network_type;
                }
                
                MMISET_SetSwitchRatState(TRUE);
                MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
                
                if(!is_reset_factorysetting)
                {
                    //进入网络连接界面
                    MMISET_OpenNetworkConnectWin(MN_DUAL_SYS_1,
                                                                            MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID,
                                                                            STR_SIM_SEARCHING_NETWORK,
                                                                            MMIPUB_SOFTKEY_NONE,
                                                                            MMISET_HandleWaitSearchNetworkWindow);
                }
            }
            else
            {
#ifdef MMI_DUALMODE_ENABLE
                s_allow_rat_switching = TRUE;
#endif
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            }
        }
    }
    else
    {
#ifdef MMI_GUI_STYLE_TYPICAL
        //MMIPUB_OpenAlertSuccessWin(TXT_PLMN_SELECT_COMPLETE);        
#endif
    }
    
}
#endif
#if defined(MMI_DUALMODE_ENABLE) ||defined (MMI_2G_4G_SUPPORT)
/*****************************************************************************/
// Description : reset network when resume factory setting
// Global resource dependence : 
// Author:kun.yu
// Note: 2009-12-28
/*****************************************************************************/
PUBLIC void MMISET_ResetNetWorkType(BOOLEAN only_write_nv)
{
    NETWORK_TYPE_E network_type = NETWORK_TYPE_DM_E;
    
    network_type = GetNetworkType();
    
    if(network_type != NETWORK_TYPE_DM_E)
    {
        network_type = NETWORK_TYPE_DM_E;
	#ifndef WIN32
        SetNetworkType(network_type, FALSE, only_write_nv, TRUE);
	#endif
    }
    return;
}

/*****************************************************************************/
// Description : set switch rat state
// Global resource dependence : 
// Author:kun.yu
// Note: 2010-02-04
/*****************************************************************************/
PUBLIC void MMISET_SetSwitchRatState(BOOLEAN state)
{
    is_rat_switching = state;
}

/*****************************************************************************/
// Description : get switch rat state
// Global resource dependence : 
// Author:kun.yu
// Note: 2010-02-04
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetSwitchRatState(void)
{
    return is_rat_switching;
}
#endif


/*****************************************************************************/
//  Description : 网络选择后，等待搜网成功,这中间只响应关机键
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleWaitSearchNetworkWindow(
                                                         MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                         MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                         DPARAM             param       // 相应消息的参数
                                                         )
{
    //@zhaohui modify for delete timer
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    static BOOLEAN s_has_closewin_msg = FALSE;//记录是否有退出窗口的消息，当允许退出时，立即响应该消息

    SCI_TRACE_LOW("MMISET_HandleWaitSearchNetworkWindow, win_id = %d, msg_id = %d",win_id,msg_id);

    switch (msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_APP_RED:   
#ifdef MMI_DUALMODE_ENABLE
        s_allow_rat_switching = FALSE;
#endif
        if(0 != s_searchnetwork_space_timer_id)//在等待重新尝试搜网的间隔内，可响应退出
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            s_has_closewin_msg = TRUE;
            MMK_CloseWin(win_id);
        }   
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        
        
    case MSG_SET_SPACE_FOR_RESTART_SETRAT:
        if (s_has_closewin_msg)
        {
            s_has_closewin_msg = FALSE;
            MMK_CloseWin(win_id);
        }
        else
        {   
            s_searchnetwork_space_timer_id = MMK_CreateWinTimer(
                MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID,
                MMISET_SEARCHNETWORK_SPACE_TIME, 
                FALSE
                );
        }
        s_allow_rat_switching = TRUE;

        break;
#ifdef MMI_DUALMODE_ENABLE 
    case MSG_TIMER:
        if (s_searchnetwork_space_timer_id == *(uint8*)param)
        {
            s_searchnetwork_space_timer_id = 0;
            MMK_StopTimer(s_searchnetwork_space_timer_id);           
            SetNetworkType(NETWORK_TYPE_DM_E, TRUE, FALSE, FALSE);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
#endif
    case MSG_CLOSE_WINDOW:
#ifdef MMI_DUALMODE_ENABLE 
        MMISET_SetSwitchRatState(FALSE);
#endif
        if(0 != s_searchnetwork_space_timer_id)
        {
            s_allow_rat_switching = TRUE;
        }
        MMK_StopTimer(s_searchnetwork_space_timer_id);
        s_searchnetwork_space_timer_id = 0;
        s_has_closewin_msg = FALSE;
        ResetReSwitchRatCount();
        result = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);        
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description :关闭网络搜索的窗口
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_CloseSearchNetworkWin(void)
{
    //关闭网络搜索窗口
    MMK_CloseWin(MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID);
}

/*****************************************************************************/
//  Description :判断窗口MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID是否打开
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SearchNetworkWinisOpen(void)
{
    return MMK_IsOpenWin(MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID);
}
/*****************************************************************************/
//  Description :显示搜网结果 
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SearchNetworkResultDisp(BOOLEAN   result)
{
    
    if (MMIAPISET_SearchNetworkWinisOpen())
    {         
        if (result)
        {
            //SCI_TRACE_LOW:"ResultDisp success: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11802_112_2_18_2_50_42_102,(uint8*)"d", result);
            MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        }
        else
        {
            //SCI_TRACE_LOW:"ResultDisp fail: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11807_112_2_18_2_50_42_103,(uint8*)"d", result);
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        }
        MMIAPISET_CloseSearchNetworkWin();
    }
}

/*****************************************************************************/
//  Description :显示设置优先网络结果 
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void SetPreferRatResultDisp(BOOLEAN result)
{
#ifdef MMI_DUALMODE_ENABLE 
    MMISET_SetSwitchRatState(FALSE);
#endif

    s_allow_rat_switching = TRUE;

    if (MMIAPISET_SearchNetworkWinisOpen())
    {         
        if (result)
        {
            //SCI_TRACE_LOW:"ResultDisp success: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11827_112_2_18_2_50_42_104,(uint8*)"d", result);
            //MMIPUB_OpenAlertSuccessWin(TXT_PLMN_SELECT_COMPLETE);
        }
        else
        {
            //SCI_TRACE_LOW:"ResultDisp fail: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11832_112_2_18_2_50_42_105,(uint8*)"d", result);
            MMIPUB_OpenAlertWarningWin(STR_SIM_CONNECTION_FAIL);
        }
        MMIAPISET_CloseSearchNetworkWin();
    }   
}


/*****************************************************************************/
//  Description : 是否包含回車
//  Global resource dependence : 
//  Author: bruce.chi
//  Return: TRUE: contain invalid char. FALSE: do not contain invalid char
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsIncludeReturnChar(
                                             const wchar    * name_ptr, 
                                             uint16           wstr_len
                                             )
{/*lint !e18*/
    uint8       invalid_char_r    =   '\r'; //换行
    uint8       invalid_char_n    =   '\n'; //回车
    uint16      i               =   0;    
    
    if (name_ptr == PNULL || wstr_len == 0)
    {
        return FALSE;
    }
    
    for(i=0; i<wstr_len; i++)
    {
        
        if ((*(name_ptr+i) == invalid_char_n) || (*(name_ptr+i) == invalid_char_r))
        {
            return TRUE;
        }
    }
    
    return FALSE;
}




/*****************************************************************************/
//  Description : open wizard cfnrc win
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_OpenWizardCFNRCWin(void)
{
#ifdef MMI_MULTI_SIM_SYS_DUAL
    MMISET_SelectSIMFunc(WizardCFNRCSIMSelectCallback, MMISET_APPEND_SYS_OK ,PNULL);
#endif
    return FALSE;
    
}

/*****************************************************************************/
//  Description : is open dual sys CFNRC
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsOpenDualSysCfnrc(void)
{
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    return s_is_action_dualsys_cfnrc;
#else
    return FALSE;
#endif
}





/*****************************************************************************/
//  Description : MMIAPISET_HandlePreferRatRsp
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandlePreferRatRspResult(
                                                       BOOLEAN rsp_result
                                                       )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"MMIAPISET_HandlePreferRatRsp rsp_result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11914_112_2_18_2_50_42_106,(uint8*)"d", rsp_result);

    if (rsp_result)//设置成功
    {
        SetPreferRatResultDisp(rsp_result);
        MMIAPIPHONE_SetGPRSAttachMode();
#ifdef MMI_GPRS_SUPPORT
        MMIAPIPDP_ResetPdp(MN_DUAL_SYS_1);
#endif
    }

    if (MMIAPISET_SearchNetworkWinisOpen())
    {  
        MMIDEFAULT_TurnOnBackLight();

        if (!rsp_result)//设置失败
        {
            if (s_re_switchrat_count < MMISET_SETRAT_MAX_NUM)//切网失败，最多允许重试7次
            {
                s_re_switchrat_count ++;
                MMK_SendMsg(MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID, MSG_SET_SPACE_FOR_RESTART_SETRAT, PNULL);
            }   
            else
            {
                //模式切换失败
                // MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                //MMIAPISET_CloseSearchNetworkWin();
                SetPreferRatResultDisp(rsp_result);
            }
        }
    }
    else
    {
#ifdef MMI_DUALMODE_ENABLE 
        MMISET_SetSwitchRatState(FALSE);
#endif
       s_allow_rat_switching = TRUE; 
    }
    
    s_allow_rat_switching = TRUE; 

    return (result);
    
}

/*****************************************************************************/
//  Description :网络搜索的窗口，判断是否是在切网状态
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsinSwitchRatState(void)
{
    if (0 != s_searchnetwork_space_timer_id)
    {
        //SCI_TRACE_LOW:"IsinSwitchRatState is TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11964_112_2_18_2_50_42_107,(uint8*)"");
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"IsinSwitchRatState is FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11969_112_2_18_2_50_42_108,(uint8*)"");
        return FALSE;
    }    
}

/********************************************************************************
NAME:           ResetReSwitchRatCount
DESCRIPTION:    
PARAM IN:       None
PARAM OUT:      
AUTHOR:     zhaohui
DATE:           2009.1.5
********************************************************************************/
LOCAL void ResetReSwitchRatCount(void)
{
    s_re_switchrat_count = 0;
}

/*****************************************************************************/
//  Description :MMIAPISET_IsWizardCfnrc
//  Global resource dependence : 
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsWizardCfnrc(void)
{
    return s_is_wizard_cfnrc;
}





/*****************************************************************************/
// Description : 开机设置飞行模式
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-8
/*****************************************************************************/
PUBLIC void MMIAPISET_SetFlyModeOpenPhone(void)
{
    MMK_CreatePubListWin((uint32*)MMISET_FLY_MODE_OPEN_PHONE_WIN_TAB,PNULL);
}

/*****************************************************************************/
// Description : to handle set fly mode window msg when open phone
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeOpenPhoneWindow(
                                                 MMI_WIN_ID_T      win_id,
                                                 MMI_MESSAGE_ID_E  msg_id,
                                                 DPARAM            param
                                                 )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    BOOLEAN             is_fly_mode_on  =   FALSE;
    MMI_CTRL_ID_T       ctrl_id         =   MMISET_FLY_MODE_OPEN_LIST_CTRL_ID;
    uint16              cur_selection   =   0;    
#if !defined(MMI_PDA_SUPPORT)
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_BOTH_RECT_T win_rect = MMITHEME_GetFullScreenBothRect();
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        is_fly_mode_on = MMIAPISET_GetFlyMode();
        GUILIST_SetMaxItem(ctrl_id,2, FALSE );
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, is_fly_mode_on, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,is_fly_mode_on);
#ifdef PDA_UI_DROPDOWN_WIN
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_INVALID, TRUE); 
#endif
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
#if !defined(MMI_PDA_SUPPORT)
    case MSG_FULL_PAINT:
        if (!MMITHEME_IsMainScreenLandscape())
        {
            GUI_FillRect(&lcd_dev_info, win_rect.v_rect, MMI_BLACK_COLOR);
        }
        else
        {
            GUI_FillRect(&lcd_dev_info, win_rect.h_rect, MMI_BLACK_COLOR);
        }
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        
        is_fly_mode_on = (BOOLEAN)cur_selection;
        
        MMIAPISET_SetFlyMode(is_fly_mode_on);
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIAPIPHONE_NormalInitAfterPwd();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}





/*****************************************************************************/
//  Description :get voice mail address record id
//  Global resource dependence : 
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetVMRecordId(void)
{
    MMI_MENU_GROUP_ID_T          group_id   =   0;
    MMI_MENU_ID_T                menu_id    =   0;
    uint8                        record_id  =   0; 
    
    if (!MMK_IsOpenWin(MMISET_SET_OPEN_DIVERT_WIN_ID))
    {
        return record_id;
    }
    
    //to get the menu id
    GUIMENU_GetId( MMISET_VOICE_NUM_CTRL_ID, &group_id, &menu_id);           
    switch ( menu_id )
    {
    case ID_SET_SS_LINE_VMN:
        record_id = 1;
        break;
#ifdef MMI_CSP_SUPPORT    
    case ID_SET_SS_LINE1_VMN:   
        record_id = 1;
        break;          
    case ID_SET_SS_LINE2_VMN:
        record_id = 2;
        break;
#endif //MMI_CSP_SUPPORT        
    case ID_SET_SS_FAX_VMN:
        record_id = 3;
        break;
    case ID_SET_SS_DATA_VMN:
        record_id = 4;
        break;          
    default:
        break;
    }
    
    return record_id;
}




/*****************************************************************************/
//  Description : open network connect win
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_OpenNetworkConnectWin(MN_DUAL_SYS_E dual_sys,
                                        MMI_WIN_ID_T win_id,
                                        MMI_TEXT_ID_T text_id,
                                        MMIPUB_SOFTKEY_STYLE_E  softkey_style,  //softkey style
                                        MMIPUB_HANDLE_FUNC      handle_func     //handle function
                                    )
{
    MMI_STRING_T connect_net_text = {0};
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T sim_name = {0};
    MMI_STRING_T sim_string_ptr = {0};
#endif

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    MMI_GetLabelTextByLang(text_id, &connect_net_text);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    sim_name = MMIAPISET_GetSimName(dual_sys);

    sim_string_ptr.wstr_len = sim_name.wstr_len;
    sim_string_ptr.wstr_ptr = sim_name.wstr_arr;

    MMIPUB_OpenWaitWin(2,&sim_string_ptr,&connect_net_text,PNULL,win_id,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,softkey_style,handle_func);
#else
    MMIPUB_OpenWaitWin(1,&connect_net_text,PNULL,PNULL,win_id,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,softkey_style,handle_func);
#endif
}

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
//  Description : to handle select line msg
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectLineWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM               param
                                           )
{
    MMI_RESULT_E        recode      =   MMI_RESULT_TRUE;
    uint32              dual_sys    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_LINE_SELECT_LIST_CTRL_ID,MMISET_MAX_LINE, FALSE );//max item 2
        MMISET_AppendLineListItem(dual_sys,MMISET_LINE_SELECT_LIST_CTRL_ID);/*lint !e64*/
        MMK_SetAtvCtrl(win_id,MMISET_LINE_SELECT_LIST_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16 cur_selection = 0;
            MMI_CTRL_ID_T ctrl_id = MMISET_LINE_SELECT_LIST_CTRL_ID;
            //get the selected item
            GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
            if(!MMISET_SetSelectLine(dual_sys,(MMISET_LINE_TYPE_E)cur_selection))
            {
                //fail
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
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

/*****************************************************************************/
//      Description : to handle lock line msg
//     Global resource dependence : 
//  Author:samboo.shen
//     Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLockLineWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E      msg_id,
                                           DPARAM                   param
                                           )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_LOCK_LINE_LIST_CTRL_ID;
    LOCAL MN_DUAL_SYS_E s_dual_sys = MN_DUAL_SYS_1;
    uint16  cur_selection = 0;
    BOOLEAN is_lock_line = FALSE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, 2, FALSE );//max item 2
        s_dual_sys = MMIAPIPHONE_GetCurHandlingPinSimID(); //???????,???pin2???????????
        //add items to listbox
        MMIAPISET_AppendListItemByTextId( TXT_CLOSE, STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId( TXT_OPEN, STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        
        //get current line
        cur_selection = (MMISET_GetLineLockStatus(s_dual_sys) == MMISET_LINE_IS_LOCK)?1:0;
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex( ctrl_id);
        if( 1 == cur_selection )
        {
            is_lock_line = TRUE;
        }
        if(MMI_RESULT_TRUE == MMISET_LockLine(s_dual_sys, is_lock_line))
        {
            //??????
            MMI_STRING_T wait_text = {0};
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MAIN_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
        break;

    case MSG_GET_FOCUS:
        cur_selection = (MMISET_GetLineLockStatus(s_dual_sys) == MMISET_LINE_IS_LOCK)?1:0;
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
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

/*****************************************************************************/
// Description : set line lock
// Global resource dependence : 
// Author: samboo.shen
// Note:
/*****************************************************************************/
LOCAL void  SetLineLock(void)
{
   // ????????
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    MMISET_SIM_SELECT_TYPE_E select_type = 0;

    if (MMIAPISET_GetFlyMode())
    {
        MMIPHONE_AlertSimNorOKStatus();
        return ;
    }

    select_type = MMISET_LINE_LOCK;
    sim_num = MMIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (sim_num == 0)
    {
        MMIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMK_CreateWin((uint32*)MMISET_LOCK_LINE_WIN_TAB, (ADD_DATA)sim_sys);
    }
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#else
    if (MMIAPISET_GetFlyMode())
    {
        MMIPHONE_AlertSimNorOKStatus();
        return ;
    }
    
    if (MMIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
    {
        MMK_CreateWin((uint32*)MMISET_LOCK_LINE_WIN_TAB,PNULL);
        
    }
    else
    {
        MMIPHONE_AlertSimNorOKStatus();
    }
#endif
    
    return;
}
/*****************************************************************************/
// Description : ????
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-6
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPISET_SetLineInfo(void)
{
    MMI_WIN_ID_T                    win_id      =   0;
    
    // ????????
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    MMISET_SIM_SELECT_TYPE_E select_type = 0;

    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return win_id;
    }

    select_type = MMISET_LINE_SELECT;
    sim_num = GetSelectLineVisibleSimNum(&sim_sys, 1);//MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMK_CreateWin((uint32*)MMISET_SELECT_LINE_WIN_TAB, (ADD_DATA)sim_sys);
        win_id = MMISET_LINE_SELECT_WIN_ID;
    }
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
        win_id = MMISET_COM_SELECT_SIM_WIN_ID;
    }
#else
    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return win_id;
    }
    
    if (MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
    {
        MMK_CreateWin((uint32*)MMISET_SELECT_LINE_WIN_TAB,PNULL);
        
        win_id = MMISET_LINE_SELECT_WIN_ID;
    }
    else
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
#endif
    
    return win_id;
}
/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSelectLineVisibleByDualSys(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN result = FALSE;
#ifdef MMI_CSP_SUPPORT    
    if(MMISET_IsAlsValid(dual_sys))
    {
        if(!MMISET_IsCspValid(dual_sys, 1))
        {
            result = TRUE;
        }
        else
        {
            MMISET_LINE_LOCK_STATUS_E line_lock_status = MMISET_GetLineLockStatus(dual_sys);
            result = !(MMISET_LINE_LOCK_INVALID == line_lock_status);
        }
    }
    else
    {
        result = FALSE;
    }
#endif //MMI_CSP_SUPPORT
    return result;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsLockLineVisibleByDualSys(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN result = FALSE;
#ifdef MMI_CSP_SUPPORT        
    if(MMISET_IsAlsValid(dual_sys))
    {
        //if csp2 not valid, hide line lock option(because csp2 dynamic file save the line lock flag)
        if(!MMISET_IsCspValid(dual_sys, 1))
        {
            result = FALSE;
        }
        else
        {
            MMISET_LINE_LOCK_STATUS_E line_lock_status = MMISET_GetLineLockStatus(dual_sys);
            result = !(MMISET_LINE_LOCK_INVALID == line_lock_status);
        }
    }
    else
    {
        result = FALSE;
    }
#endif //MMI_CSP_SUPPORT    
    return result;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetSelectLineVisibleSimNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (IsSelectLineVisibleByDualSys(i))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetLockLineVisibleSimNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (IsLockLineVisibleByDualSys(i))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetCallFowardVisibleSimNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_IsSimAvailable(i) && IsSSMenuItemVisible(i, CSP_MENU_CF, CSP_MENU_OPER_NULL))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetCallBarVisibleSimNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_IsSimAvailable(i) && IsSSMenuItemVisible(i, CSP_MENU_CB, CSP_MENU_OPER_NULL))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide cf cb menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSSMenuItemVisible(MN_DUAL_SYS_E dual_sys, CSP_MENU_SS_TYPE_E ss_type, CSP_MENU_SS_OPER_E ss_oper)
{
    BOOLEAN result = TRUE;

#ifdef MMI_CSP_SUPPORT
    if(MMISET_IsAlsValid(dual_sys))
    {
        MN_BEARER_CAPABILITY_TYPE_E line = 0;
        if(MMISET_IsCspValid(dual_sys, 1))
        {
            line = (MN_BEARER_CAPABILITY_TYPE_E)MMIAPISET_GetCallLineInfo(dual_sys); //if csp2 valid, get line from sim
        }
        else
        {
            line = MN_BEARER_CAPABILITY_LINE_1;//if csp2 invalid, select line1
        }
    
        switch(ss_type)
        {
            case CSP_MENU_CF:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallOffering_CallDivert(dual_sys, line); 
                            break;

                        case CSP_MENU_OPER_CANCEL_ALL:
                            result = MNSIM_CPHS_CallOffering_CancelAllVoiceDivert(dual_sys, line);
                            break; 

                        default:
                            result = TRUE;
                            break;
                    }
                }
                break;

            case CSP_MENU_CFU:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Activate(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE_WITH_PB:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Phonebook(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_VM:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_VoicemailNumber(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_NO:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Number(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Cancel(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;
                    }
                }
                break;

            case CSP_MENU_CFB:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Activate(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE_WITH_PB:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Phonebook(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_VM:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_VoicemailNumber(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_NO:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Number(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Cancel(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;                    
                    }
                }
                break;

            case CSP_MENU_CFNRY:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Activate(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE_WITH_PB:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Phonebook(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_VM:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_VoicemailNumber(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_NO:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Number(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Cancel(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;                    
                    }
                }
                break;

            case CSP_MENU_CFNRC:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Activate(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE_WITH_PB:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Phonebook(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_VM:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_VoicemailNumber(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_NO:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Number(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Cancel(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;                    
                    }
                }
                break;

            case CSP_MENU_CB:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_BarringOfCalls(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL_ALL:
                            result = MNSIM_CPHS_CallRestriction_ClearAllBarrings(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_MODIFY_PASSWORD:
                            result = MNSIM_CPHS_CallRestriction_ChangeBarringPassword(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;
                    }
                }
                break;

            case CSP_MENU_BAOC:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_OutgoingCalls(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallRestriction_OutgoingCalls_Activate(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallRestriction_OutgoingCalls_Cancel(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallRestriction_OutgoingCalls_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;
                    }
                }
                break;

            case CSP_MENU_BAIC:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_IncomingCalls(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallRestriction_IncomingCalls_Activate(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallRestriction_IncomingCalls_Cancel(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallRestriction_IncomingCalls_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;

                    }
                }
                break;

            case CSP_MENU_BICROAM:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming_Activate(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming_Cancel(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;

                    }
                }
                break;

            case CSP_MENU_BOIC:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_AllInternationalCalls(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallRestriction_AllInternationalCalls_Activate(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallRestriction_AllInternationalCalls_Cancel(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallRestriction_AllInternationalCalls_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;

                    }
                }
                break;

            case CSP_MENU_BOIC_EXHC:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming_Activate(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming_Cancel(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;

                    }
                }
                break;

            default:
                result = TRUE;
                break;
        }
    }
    else
#endif //MMI_CSP_SUPPORT        
    {
        result = TRUE;
    }

    SCI_TRACE_LOW("MMISET IsSSMenuItemVisible dual_sys:%d, type:%d, oper:%d,result:%d", dual_sys, ss_type, ss_oper, result);    
    return result;
}
#endif //MMI_CSP_SUPPORT


#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
//  Description : handle network setting window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetworkSettingWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);    
    
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_NETWORK_SETTING_CTRL_ID;

    //SCI_TRACE_LOW:"HandleNetworkSettingWindow,msg_id=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12182_112_2_18_2_50_43_109,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
        MMK_SetAtvCtrl(win_id,MMISET_SET_NETWORK_SETTING_CTRL_ID);
        break;        
       
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_MENU_GROUP_ID_T     group_id     = 0;
            MMI_MENU_ID_T             menu_id   = 0;
            GUIMENU_GetId( MMISET_SET_NETWORK_SETTING_CTRL_ID, &group_id, &menu_id);     
            
            switch (menu_id)
            {   
            case ID_SET_NETWORK_SETTING_SEL_MENU: /// net work selection                    
                MMK_CreateWin((uint32*)MMISET_SET_SELECT_NETWORK_WIN_TAB, (ADD_DATA)dual_sys);
                break;

            case ID_SET_NETWORK_SETTING_PREFER_MENU: ///  偏好网络
                MMK_CreateWin((uint32*)MMISET_SET_DISP_PREFER_NETWORK_WIN_TAB, (ADD_DATA)dual_sys);
                break;

            default:
                break;
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

/*****************************************************************************/
//  Description : handle display prefer network list window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDispPreferNetworkListWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);    
    MMI_CTRL_ID_T ctrl_id = MMISET_NETWORK_PREFER_PLMN_LIST_CTRL_ID;
    
    //静态变量，在list窗口FULL PAINT的时候需要更新list的数据。.
    static MN_PHONE_PLMN_SEL_LIST_T prefer_network_list = {0};

    //SCI_TRACE_LOW:"HandleDispPreferNetworkListWinMsg,msg=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12247_112_2_18_2_50_43_110,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
            GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);

#if !defined MMI_GUI_STYLE_TYPICAL
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif

            MMK_SendMsg(win_id, MSG_SET_LOAD_PREFER_PLMN_LIST, PNULL);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break; 
        
    case MSG_SET_LOAD_PREFER_PLMN_LIST:
        {
            uint16 max_list_num =0;
            
            prefer_network_list = MMIAPISET_LoadPreferNetworkList(dual_sys);
            max_list_num = prefer_network_list.plmn_validate_num;
            
            //SCI_TRACE_LOW:"HandleDispPreferNetworkListWinMsg,max_list_num"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12272_112_2_18_2_50_43_111,(uint8*)"",max_list_num);

            InitPlmnListCtrl(ctrl_id, max_list_num);
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
        }
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            uint16 index = 0;
            
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
    
            if( PNULL != need_item_data_ptr )
            {
                index = need_item_data_ptr->item_index;                    
                MMIAPIPHONE_AppendPreferPlmnList(dual_sys, ctrl_id, prefer_network_list.plmn_arr, index);
            }
        }
        break;            
        
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        {
            uint16 total_item = GUILIST_GetTotalItemNum(ctrl_id);
            
            //SCI_TRACE_LOW:"HandleDispPreferNetworkListWinMsg,total_item_num=%d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12304_112_2_18_2_50_43_112,(uint8*)"d",total_item);
            
            if (total_item > 0)
            {
                uint16 cur_item_index = GUILIST_GetCurItemIndex(ctrl_id);

                SetCurPreferNetworkIndex(cur_item_index);
            }

            MMK_CreateWin((uint32*)MMISET_PREFER_NETWORK_OPT_MENU_WIN_TAB, (ADD_DATA)dual_sys);
        }
        break;

#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
    case MSG_CTL_LIST_LONGOK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint16 total_item = GUILIST_GetTotalItemNum(ctrl_id);
            
            //SCI_TRACE_LOW:"HandleDispPreferNetworkListWinMsg,total_item_num=%d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12325_112_2_18_2_50_43_113,(uint8*)"d",total_item);
            
            if (total_item > 0)
            {
                uint16 cur_item_index = GUILIST_GetCurItemIndex(ctrl_id);

                SetCurPreferNetworkIndex(cur_item_index);
            }

            MMK_CreateWin((uint32*)MMISET_PREFER_NETWORK_LONGPENOK_OPT_MENU_WIN_TAB, (ADD_DATA)dual_sys);
        }
        break;
#endif
#endif
        
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

/*****************************************************************************/
//      Description : HandleDeletePreferPlmnQueryWindow
//     Global resource dependence : 
//  Author: 
//     Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeletePreferPlmnQueryWindow( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMISET_PREFER_NET_DEL_QUERY_WIN_ID;
    MN_PHONE_PLMN_SEL_LIST_T prefer_network_list = {0};
    MMIPUB_INFO_T   *win_info_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {  
            //删除的执行!!!!
            uint32 i = 0;
            uint16 cur_prefer_net = GetCurPreferNetworkIndex();

            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

            if (PNULL != win_info_ptr)
            {
                dual_sys = (MN_DUAL_SYS_E)win_info_ptr->user_data;
            }

            //SCI_TRACE_LOW:"HandleDeletePreferPlmnQueryWindow,OK_Delete_Item= %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12387_112_2_18_2_50_43_114,(uint8*)"d",cur_prefer_net);
            
            //关闭询问窗口
            MMIPUB_CloseQuerytWin(&query_win_id);                
            
            prefer_network_list = MMIAPISET_LoadPreferNetworkList(dual_sys);
            
            if (cur_prefer_net < prefer_network_list.plmn_validate_num)
            {
                if(prefer_network_list.plmn_max_num - 1 == cur_prefer_net ) //恰好是最后一项
                {
                    SCI_MEMSET(&prefer_network_list.plmn_arr[cur_prefer_net],0x00,sizeof(MN_PLMN_T));
                }
                else
                {
                    for(i = cur_prefer_net; i < prefer_network_list.plmn_validate_num;i++)
                    {
                        SCI_MEMCPY(&prefer_network_list.plmn_arr[i], &prefer_network_list.plmn_arr[i+1], sizeof(MN_PLMN_T));
                    }
                    SCI_MEMSET(&prefer_network_list.plmn_arr[prefer_network_list.plmn_validate_num - 1],0x00,sizeof(MN_PLMN_T));
                }
                prefer_network_list.plmn_validate_num--;
                
                //SCI_TRACE_LOW:"HandleDeletePreferPlmnQueryWindow,Delete Plmn: mcc=%d,mnc=%d,mnc_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12411_112_2_18_2_50_43_115,(uint8*)"ddd",prefer_network_list.plmn_arr[cur_prefer_net].mcc,prefer_network_list.plmn_arr[cur_prefer_net].mnc,prefer_network_list.plmn_arr[cur_prefer_net].mnc_digit_num);

                MMIAPISET_SavePreferNetworkList(dual_sys,&prefer_network_list);
            }
        }

        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : handle prefer network options menu msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetworkOptionMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_PREFER_NETWORK_OPT_MENU_CTRL_ID;
    MN_PHONE_PLMN_SEL_LIST_T prefer_network_list = {0};
        
    MMI_WIN_ID_T query_win_id = MMISET_PREFER_NET_DEL_QUERY_WIN_ID;

    //SCI_TRACE_LOW:"HandlePreferNetworkOptionMsg,msg_id=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12449_112_2_18_2_50_43_116,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            prefer_network_list = MMIAPISET_LoadPreferNetworkList(dual_sys);
            
            //SCI_TRACE_LOW:"HandlePreferNetworkOptionMsg,total_list_num=%d,plmn_max_num=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12457_112_2_18_2_50_43_117,(uint8*)"dd",prefer_network_list.plmn_validate_num, prefer_network_list.plmn_max_num);
             
            if (prefer_network_list.plmn_validate_num == prefer_network_list.plmn_max_num)//plmn list full
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_ADD_PREFER_NETWORK_FROM_LIST,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_CREATE_PREFER_NETWORK,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_ADD_PREFER_NETWORK_FROM_LIST,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_CREATE_PREFER_NETWORK,TRUE);
#endif
#endif
            }

            //no prefer list
            if ( 0 == prefer_network_list.plmn_validate_num )
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_DELETE_PREFER_NETWORK,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_DELETE_PREFER_NETWORK,TRUE);
#endif
#endif
            }

            //no prefer list or prefer list is only one
            if ( prefer_network_list.plmn_validate_num <= 1)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_CHANGE_PREFER_NETWORK_PRIORITY,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_CHANGE_PREFER_NETWORK_PRIORITY,TRUE);
#endif
#endif
            }
            
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;

    case MSG_GET_FOCUS:
        {
            prefer_network_list = MMIAPISET_LoadPreferNetworkList(dual_sys);
            
             //SCI_TRACE_LOW:"HandlePreferNetworkOptionMsg,total_list_num=%d,plmn_max_num=%d"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12497_112_2_18_2_50_43_118,(uint8*)"dd",prefer_network_list.plmn_validate_num, prefer_network_list.plmn_max_num);
             
            if (prefer_network_list.plmn_validate_num == prefer_network_list.plmn_max_num)//plmn list full
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_ADD_PREFER_NETWORK_FROM_LIST,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_CREATE_PREFER_NETWORK,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_ADD_PREFER_NETWORK_FROM_LIST,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_CREATE_PREFER_NETWORK,TRUE);
#endif
#endif
            }

            //no prefer list
            if ( 0 == prefer_network_list.plmn_validate_num )
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_DELETE_PREFER_NETWORK,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_DELETE_PREFER_NETWORK,TRUE);
#endif
#endif
            }

            //no prefer list or prefer list is only one
            if ( prefer_network_list.plmn_validate_num <= 1)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_CHANGE_PREFER_NETWORK_PRIORITY,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_CHANGE_PREFER_NETWORK_PRIORITY,TRUE);
#endif
#endif
            }
        }
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_MENU_ID_T menu_id = 0;
            GUIMENU_GetId( ctrl_id, &group_id, &menu_id);     
            
            switch (menu_id)
            {   
            case ID_SET_ADD_PREFER_NETWORK_FROM_LIST:
                {
                    MMK_CreateWin((uint32*)MMISET_NETWORK_PLMN_LIST_WIN_TAB, (ADD_DATA)dual_sys);
                    MMK_CloseWin(win_id);
                }
                break;

            case ID_SET_CREATE_PREFER_NETWORK:
                {
                    MMK_CreatePubFormWin((uint32*)MMISET_NETWORK_NEW_PLMN_ITEM_WIN_TAB, (ADD_DATA)dual_sys);
                    MMK_CloseWin(win_id);
                }
                break;

            case ID_SET_CHANGE_PREFER_NETWORK_PRIORITY:
                {
                    MMK_CreatePubFormWin((uint32*)MMISET_NETWORK_CHANGE_PRIORITY_WIN_TAB, (ADD_DATA)dual_sys);
                    MMK_CloseWin(win_id);
                }
                break;

             case ID_SET_DELETE_PREFER_NETWORK:
                {                            
                    // 弹出delete提示窗口
                    MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_DELETE, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDeletePreferPlmnQueryWindow, (uint32)dual_sys);
                    MMK_CloseWin(win_id);
                }
                break;

            default:
                break;
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

/*****************************************************************************/
//  Description : HandlePlmnListDispWinMsg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlmnListDispWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);    
    MMI_CTRL_ID_T ctrl_id = MMISET_NETWORK_PLMN_LIST_CTRL_ID;
    
    //SCI_TRACE_LOW:"HandlePlmnListDispWinMsg,msg_id=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12603_112_2_18_2_50_44_119,(uint8*)"d",msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
            uint16 max_item_num = MMIAPIPHONE_GetPlmnListNum();    
            
            GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
            
            InitPlmnListCtrl(ctrl_id,max_item_num);
            
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;        
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            uint16 index = 0;
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
    
            if( PNULL != need_item_data_ptr )
            {
                index = need_item_data_ptr->item_index;
                MMIAPIPHONE_AppendPlmnList(dual_sys, ctrl_id,index);
            }
        }
        break;
       
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            //选中的某个item 需要加入偏好网络的列表中。
            uint16 item_data = 0;                    
            MN_PLMN_T plmn_data = {0};
            MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);

            uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);
            item_data = cur_index+1;

            // 获取这个item项，插入优选网络列表
            //如下接口可以获取mcc/mnc。
            MMIAPIPHONE_GetPlmnInfoByIndex(&plmn_data,item_data);  

            if((prefer_network.plmn_validate_num >= prefer_network.plmn_max_num)
                 ||(!isPlmnValid(dual_sys,plmn_data)))
            {
                //提示失败的窗口
                MMIPUB_OpenAlertWarningWin(TXT_SET_PREFER_NETWORK_ADD_ERROR);
            }
            else
            {
                MMK_CloseWin(win_id);
                
                prefer_network.plmn_arr[prefer_network.plmn_validate_num].mcc = plmn_data.mcc;
                prefer_network.plmn_arr[prefer_network.plmn_validate_num].mnc = plmn_data.mnc;
                prefer_network.plmn_arr[prefer_network.plmn_validate_num].mnc_digit_num = plmn_data.mnc_digit_num;
                prefer_network.plmn_validate_num++;

                //SCI_TRACE_LOW:"HandlePlmnListDispWinMsg,Add Plmn: mcc=%d,mnc=%d,mnc_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12668_112_2_18_2_50_44_120,(uint8*)"ddd",plmn_data.mcc,plmn_data.mnc,plmn_data.mnc_digit_num);

                MMIAPISET_SavePreferNetworkList(dual_sys, &prefer_network);
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

/*****************************************************************************/
//  Description : HandleCreatePlmnItemWinMsg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCreatePlmnItemWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);    
    const wchar plmn_info[] = L"MCC/MNC"; //不需要引入字串，该字段显示具有通用性。

    //SCI_TRACE_LOW:"HandleCreatePlmnItemWinMsg,msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12702_112_2_18_2_50_44_121,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T label_text  = {0};
            MMI_STRING_T plmn_priority_text = {0};
            MMI_STRING_T priority_text = {0};
            uint8 buf[4] = {0};               
            wchar plmn_priority[4] ={0};               
            MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);
            uint32 prefer_net_num = prefer_network.plmn_validate_num + 1;
            uint8 priority_range[MMISET_PLMN_PRIORITY_MAX_LEN + 1] = {0};
            uint16 priority_range_len = 0;
            wchar temp_ch1 = L'(';
            wchar temp_ch2 = L')';

            MMIAPICOM_Int2Str(prefer_net_num, buf, 4);
            MMI_STRNTOWSTR(plmn_priority, 4, buf, 4, 4);

            plmn_priority_text.wstr_ptr = plmn_priority;
            plmn_priority_text.wstr_len = sizeof(plmn_priority)/sizeof(wchar);

            label_text.wstr_ptr = (wchar *)plmn_info;
            label_text.wstr_len = sizeof(plmn_info)/sizeof(wchar);

            GUILABEL_SetText(MMISET_NETWORK_PLMN_LABEL_CTRL_ID, &label_text, FALSE); 

            MMI_GetLabelTextByLang(TXT_COMMON_PRIORITY, &priority_text);

            sprintf((char *)priority_range, "1-%d", prefer_net_num);
            priority_range_len = strlen((char *)priority_range);

            label_text.wstr_len = (priority_text.wstr_len + 1 + priority_range_len + 1);
            label_text.wstr_ptr = SCI_ALLOC_APPZ((label_text.wstr_len + 1)*sizeof(wchar));

            if (PNULL == label_text.wstr_ptr)
            {
                break;
            }

            MMI_WSTRNCPY(label_text.wstr_ptr, label_text.wstr_len, priority_text.wstr_ptr, priority_text.wstr_len, priority_text.wstr_len);
            MMI_WSTRNCPY(label_text.wstr_ptr + priority_text.wstr_len, label_text.wstr_len, &temp_ch1, 1, 1);
            MMI_STRNTOWSTR(label_text.wstr_ptr + priority_text.wstr_len + 1, label_text.wstr_len, priority_range, priority_range_len, priority_range_len);
            MMI_WSTRNCPY(label_text.wstr_ptr + priority_text.wstr_len + 1 + priority_range_len, label_text.wstr_len, &temp_ch2, 1, 1);

            GUILABEL_SetText(MMISET_NETWORK_PLMN_PRIORITY_LABEL_CTRL_ID, &label_text, FALSE);

            SCI_FREE(label_text.wstr_ptr);

            GUIEDIT_SetStyle(MMISET_NETWORK_PLMN_EDIT_CTRL_ID, GUIEDIT_STYLE_SINGLE);
            GUIEDIT_SetStyle(MMISET_NETWORK_PLMN_PRIORITY_CODE_EDIT_CTRL_ID, GUIEDIT_STYLE_SINGLE);

            //没有列表的时候，不需要设置优先级
            if (0 == prefer_network.plmn_validate_num)
            {
                GUIFORM_SetChildDisplay(MMISET_NETWORK_PLMN_FORM_CTRL_ID, MMISET_NETWORK_PLMN_FORM2_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            }
            else
            {
                GUIFORM_SetStyle(MMISET_NETWORK_PLMN_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
                GUIFORM_SetStyle(MMISET_NETWORK_PLMN_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
            }

            //优先级
            GUIEDIT_SetString(MMISET_NETWORK_PLMN_PRIORITY_CODE_EDIT_CTRL_ID, plmn_priority_text.wstr_ptr, plmn_priority_text.wstr_len);

            MMK_SetAtvCtrl(win_id, MMISET_NETWORK_PLMN_EDIT_CTRL_ID);
        }
        break;

    case MSG_CTL_MIDSK:
    //case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            // 将编辑好的plmn item加入偏好网络列表中。
            MMI_STRING_T edit_str = {0};
            MMI_STRING_T priority_str = {0};
            MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);
            uint32 prefer_net_num = prefer_network.plmn_validate_num + 1;
            MN_PLMN_T plmn = {0};
            uint8 buf[MMISET_PLMN_DIGITAL_MAX_LEN + 1] = {0};               
            char mcc_str[4] = {0};
            char mnc_str[4] = {0};
            char priority_num[MMISET_PLMN_PRIORITY_MAX_LEN + 1] = {0};
            uint16 plmn_priority = 0;
            
            GUIEDIT_GetString(MMISET_NETWORK_PLMN_EDIT_CTRL_ID, &edit_str);
            GUIEDIT_GetString(MMISET_NETWORK_PLMN_PRIORITY_CODE_EDIT_CTRL_ID, &priority_str);
            
            MMI_WSTRNTOSTR(buf, MMISET_PLMN_DIGITAL_MAX_LEN, edit_str.wstr_ptr, MMISET_PLMN_DIGITAL_MAX_LEN, edit_str.wstr_len);

            strncpy(mcc_str, (char*)buf, 3);
            strcpy(mnc_str, (char*)&(buf[3]));
            
            plmn.mcc = atoi(mcc_str);
            plmn.mnc = atoi(mnc_str);

            MMI_WSTRNTOSTR(priority_num, MMISET_PLMN_PRIORITY_MAX_LEN, priority_str.wstr_ptr, MMISET_PLMN_PRIORITY_MAX_LEN, priority_str.wstr_len); /*lint !e64*/
            plmn_priority = atoi(priority_num);

            if((edit_str.wstr_len < MMISET_PLMN_DIGITAL_MAX_LEN -1) //mnc len may be 2 or 3
              ||(prefer_network.plmn_validate_num >= prefer_network.plmn_max_num)
              ||(!isPlmnValid(dual_sys,plmn) ))
            {                   
                //提示失败的窗口
                MMIPUB_OpenAlertWarningWin(TXT_SET_PREFER_NETWORK_ADD_ERROR);
            }
            else if (plmn_priority > prefer_net_num || plmn_priority < 1)
            {
                //提示失败的窗口
                MMIPUB_OpenAlertWarningWin(TXT_SET_PREFER_NETWORK_ADD_ERROR);
            }
            else
            {
                uint32 i = 0;
                MN_PHONE_PLMN_SEL_LIST_T tmp_prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);

                plmn.mnc_digit_num = edit_str.wstr_len - 3;

                for (i = 0; i < plmn_priority - 1; i++)/*lint !e574*/
                {
                    tmp_prefer_network.plmn_arr[i] = prefer_network.plmn_arr[i];
                }
                tmp_prefer_network.plmn_arr[i].mcc = plmn.mcc;
                tmp_prefer_network.plmn_arr[i].mnc = plmn.mnc;
                tmp_prefer_network.plmn_arr[i].mnc_digit_num = plmn.mnc_digit_num;

                i += 1;

                for (; i < prefer_net_num; i++)
                {
                    tmp_prefer_network.plmn_arr[i] = prefer_network.plmn_arr[i - 1];
                }
                tmp_prefer_network.plmn_validate_num++;
                
                //SCI_TRACE_LOW:"HandleCreatePlmnItemWinMsg,Add Plmn: mcc=%d,mnc=%d,mnc_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12842_112_2_18_2_50_44_122,(uint8*)"ddd",plmn.mcc,plmn.mnc,plmn.mnc_digit_num);
                
                MMIAPISET_SavePreferNetworkList(dual_sys, &tmp_prefer_network);

                MMK_CloseWin(win_id);
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

/*****************************************************************************/
//  Description : handle plmn list display window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetworkPriorityChangeWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);    
    uint16 cur_index = GetCurPreferNetworkIndex();
    uint32 index_priority = 0; //大于0
    
    //SCI_TRACE_LOW:"HandlePreferNetworkPriorityChangeWinMsg,msg = 0x%x,cur_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12879_112_2_18_2_50_44_123,(uint8*)"dd",msg_id,cur_index);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T title_text  = {0};
            MMI_STRING_T plmn_priority_text = {0};
            MMI_STRING_T priority_text = {0};
            MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);
            uint32 prefer_net_num = prefer_network.plmn_validate_num;
            uint8 buf[4] = {0};
            wchar plmn_priority[4] ={0};
            uint8 priority_range[MMISET_PLMN_PRIORITY_MAX_LEN + 1] = {0};
            uint16 priority_range_len = 0;
            wchar temp_ch1 = L'(';
            wchar temp_ch2 = L')';

            index_priority = cur_index + 1;

            MMIAPICOM_Int2Str(index_priority, buf, 4);               
            MMI_STRNTOWSTR(plmn_priority, 4, buf, 4, 4);               

            plmn_priority_text.wstr_ptr = plmn_priority;
            plmn_priority_text.wstr_len = sizeof(plmn_priority)/sizeof(wchar);

            GUIEDIT_SetStyle(MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID, GUIEDIT_STYLE_SINGLE);

            //优先级
            GUIEDIT_SetString(MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID, plmn_priority_text.wstr_ptr, plmn_priority_text.wstr_len);

            MMI_GetLabelTextByLang(TXT_COMMON_PRIORITY, &priority_text);

            sprintf((char *)priority_range, "1-%d", prefer_net_num);
            priority_range_len = strlen((char *)priority_range);

            title_text.wstr_len = (priority_text.wstr_len + 1 + priority_range_len + 1);
            title_text.wstr_ptr = SCI_ALLOC_APPZ((title_text.wstr_len + 1)*sizeof(wchar));

            if (PNULL == title_text.wstr_ptr)
            {
                break;
            }

            MMI_WSTRNCPY(title_text.wstr_ptr, title_text.wstr_len, priority_text.wstr_ptr, priority_text.wstr_len, priority_text.wstr_len);
            MMI_WSTRNCPY(title_text.wstr_ptr + priority_text.wstr_len, title_text.wstr_len, &temp_ch1, 1, 1);
            MMI_STRNTOWSTR(title_text.wstr_ptr + priority_text.wstr_len + 1, title_text.wstr_len, priority_range, priority_range_len, priority_range_len);
            MMI_WSTRNCPY(title_text.wstr_ptr + priority_text.wstr_len + 1 + priority_range_len, title_text.wstr_len, &temp_ch2, 1, 1);

            GUIWIN_SetTitleText(win_id, title_text.wstr_ptr, title_text.wstr_len, FALSE);

            SCI_FREE(title_text.wstr_ptr);

            GUIEDIT_SetStyle(MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID, GUIEDIT_STYLE_SINGLE);

            MMK_SetAtvCtrl(win_id, MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID);
        }
        break;
     
    case MSG_CTL_MIDSK:
    //case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T priority_str = {0};
            char priority_num[MMISET_PLMN_PRIORITY_MAX_LEN + 1] = {0};
            MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);
            uint32 prefer_net_num = MIN(prefer_network.plmn_validate_num, prefer_network.plmn_max_num);
            MN_PLMN_T plmn_temp = {0};
            
            GUIEDIT_GetString(MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID, &priority_str);

            MMI_WSTRNTOSTR(priority_num, MMISET_PLMN_PRIORITY_MAX_LEN, priority_str.wstr_ptr, MMISET_PLMN_PRIORITY_MAX_LEN, priority_str.wstr_len);/*lint !e64*/
            index_priority = atoi(priority_num);

            //SCI_TRACE_LOW:"HandlePreferNetworkPriorityChangeWinMsg,index_priority=%d,cur_index=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12955_112_2_18_2_50_44_124,(uint8*)"dd",index_priority,cur_index);

            if (index_priority == cur_index + 1) //优先级没有调整
            {
                //clsoe option menu and current win.
                MMK_CloseWin(MMISET_PREFER_NETWORK_OPT_MENU_WIN_ID);
                MMK_CloseWin(win_id);
                // success !
#ifdef MMI_GUI_STYLE_TYPICAL
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            }
            else if (index_priority > prefer_net_num || index_priority < 1)
            {
                //提示失败的窗口
                MMIPUB_OpenAlertWarningWin(TXT_INPUT_ERROR);
            }
            else if( index_priority - 1 < cur_index )
            {
                uint32 i = 0;

                SCI_MEMCPY(&plmn_temp, &prefer_network.plmn_arr[cur_index], sizeof(MN_PLMN_T));

                for (i = cur_index; i > index_priority -1; i--)
                {
                    SCI_MEMCPY(&prefer_network.plmn_arr[i], &prefer_network.plmn_arr[i - 1], sizeof(MN_PLMN_T));
                }                        
                SCI_MEMCPY(&prefer_network.plmn_arr[index_priority-1], &plmn_temp, sizeof(MN_PLMN_T));

                MMIAPISET_SavePreferNetworkList(dual_sys, &prefer_network);
            }
            else if (index_priority - 1 > cur_index)
            {
                uint32 i = 0;

                SCI_MEMCPY(&plmn_temp, &prefer_network.plmn_arr[cur_index], sizeof(MN_PLMN_T));

                for (i = cur_index; i < index_priority - 1; i++)
                {
                    SCI_MEMCPY(&prefer_network.plmn_arr[i], &prefer_network.plmn_arr[i + 1], sizeof(MN_PLMN_T));
                }                        
                SCI_MEMCPY(&prefer_network.plmn_arr[index_priority - 1], &plmn_temp, sizeof(MN_PLMN_T));

                MMIAPISET_SavePreferNetworkList(dual_sys, &prefer_network);
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

/*****************************************************************************/
//  Description : HandlePreferNetWaitingWinMsg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetWaitingWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"HandlePreferNetChangeWaitingWinMsg,msg=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_13026_112_2_18_2_50_45_125,(uint8*)"d",msg_id);

    switch(msg_id)
    {        
    case MSG_APP_CANCEL:
        s_allow_set_prefer_plmn = FALSE;
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Description : InitPlmnListCtrl
//  Global resource dependence : 
//  Author:michael.shi
//  Note: InitPlmnListCtrl
/*****************************************************************************/
LOCAL void InitPlmnListCtrl(MMI_CTRL_ID_T ctrl_id,uint16 max_item_num)
{
    uint32 i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16 index = GUILIST_GetCurItemIndex(ctrl_id);

    //需要动态创建plmn列表数，以节省memory!!
    GUILIST_SetMaxItem( ctrl_id, max_item_num, TRUE); 

    item_t.item_style         = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    item_t.item_data_ptr  = &item_data;
    
    if (max_item_num)
    {            
        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
    }
    else
    {
        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
    }
    
    for ( i = 0; i < max_item_num; i++ )
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }       

    GUILIST_SetCurItemIndex(ctrl_id,index);
}

/*****************************************************************************/
//  Description : get prefer network list num.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetCurPreferNetworkIndex(void)
{
    return s_prefer_network_item;
}

/*****************************************************************************/
//  Description : set prefer network list num.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL void SetCurPreferNetworkIndex( uint16 index)
{
    s_prefer_network_item = index;
}

/*****************************************************************************/
//  Description : isPlmnValid ,
//  Global resource dependence : 
//  Author:michael.shi
//  Note: if plmn is in list ,return FALSE; if plmn is not in list ,return TRUE;
/*****************************************************************************/
LOCAL BOOLEAN isPlmnValid(MN_DUAL_SYS_E dual_sys, MN_PLMN_T plmn)
{
    uint32 i=0;
    MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);

    for(i = 0; i < prefer_network.plmn_validate_num; i++)    
    {
        //某些特殊的case可以加快循环体的判断结果
        if( (plmn.mcc == prefer_network.plmn_arr[i].mcc )
            &&(plmn.mnc == prefer_network.plmn_arr[i].mnc) )
        {
            return FALSE;
        }
    }
    
    return TRUE;
}
/*****************************************************************************/
//  Description : MMISET_PreferNetworkListInit
//  Global resource dependence : 
//  Author:michael.shi
//  Note: call l4 MN API:MNPHONE_GetPreferredPlmnListEx
/*****************************************************************************/
PUBLIC MN_PHONE_PLMN_SEL_LIST_T MMIAPISET_LoadPreferNetworkList(MN_DUAL_SYS_E dual_sys)
{
    MN_PHONE_PLMN_SEL_LIST_T return_prefer_list = {0};

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return (return_prefer_list);
    }
    
    MNPHONE_GetPreferredPlmnListEx(dual_sys, MN_PHONE_PLMN_LIST_UPLMN, &return_prefer_list);
    
    //SCI_TRACE_LOW:"LoadMNPreferNetworkList,sim=%d,prefer_valid_num=%d,prefer_max_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_13131_112_2_18_2_50_45_126,(uint8*)"ddd",dual_sys,return_prefer_list.plmn_validate_num,return_prefer_list.plmn_max_num);
    
    return (return_prefer_list);
}

/*****************************************************************************/
//  Description : MMIAPISET_SavePreferNetworkList
//  Global resource dependence : 
//  Author:michael.shi
//  Note: call l4 MN API : MMIAPISET_SavePreferNetworkList
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SavePreferNetworkList(
                                   MN_DUAL_SYS_E   dual_sys,
                                   MN_PHONE_PLMN_SEL_LIST_T   *prefer_network   
                                   )
{
    MMI_STRING_T prompt_str = {0};
    MMI_WIN_ID_T wait_win_id =  MMISET_PREFER_CHANGE_WAIT_WIN_ID;
    uint16 prefer_list_mem_size = sizeof(MN_PHONE_PLMN_SEL_LIST_T);

    if (dual_sys >= MMI_DUAL_SYS_MAX || PNULL == prefer_network)
    {
        return FALSE;
    }

    SCI_TRACE_LOW("MMIAPISET_SavePreferNetworkList, s_allow_set_prefer_plmn= %d ", s_allow_set_prefer_plmn);
    if(!s_allow_set_prefer_plmn)
    {
        MMIPUB_OpenAlertWaitingWin(TXT_COMMON_WAITING);
        return FALSE;
    }
          

    if(PNULL == s_update_prefer_list_ptr)
    {
        s_update_prefer_list_ptr = SCI_ALLOC_APP(prefer_list_mem_size);
    }
    
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);    
    
    MNPHONE_SetPreferredPlmnListEx(dual_sys, prefer_network);

    //保留本次更新的列表，以确认SIM是否可以更新成功。
    if(PNULL != s_update_prefer_list_ptr)
    {
        SCI_MEMSET(s_update_prefer_list_ptr,0x00,prefer_list_mem_size);
        SCI_MEMCPY(s_update_prefer_list_ptr,prefer_network,prefer_list_mem_size);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPISET_SavePreferNetworkList ERROR ! s_update_prefer_list_ptr == PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_13171_112_2_18_2_50_45_127,(uint8*)"");
    }

    //打开waiting的等待窗口
    MMIPUB_OpenWaitWin(1,
                            &prompt_str,
                            PNULL,
                            PNULL,
                            wait_win_id,
                            IMAGE_NULL,
                            ANIM_PUBWIN_WAIT,
                            WIN_ONE_LEVEL,
                            MMIPUB_SOFTKEY_ONE,
                            HandlePreferNetWaitingWinMsg);

    
    //SCI_TRACE_LOW:"MMIAPISET_SavePreferNetworkList,dual_sys=%d,prefer_valid_num=%d,prefer_max_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_13188_112_2_18_2_50_45_128,(uint8*)"ddd",dual_sys,prefer_network->plmn_validate_num,prefer_network->plmn_max_num);

    return TRUE;
}

/*****************************************************************************/
//  Description : MMIAPISET_HandlePreferNetworkListCnf
//  Global resource dependence : 
//  Author:michael.shi
//  Note:  MMIAPISET_HandlePreferNetworkListCnf
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_HandlePreferNetworkListCnf(APP_MN_PREFER_PLMN_CHANGE_CNF_T* prefer_cnf)
{    
    BOOLEAN is_update_success = FALSE;

    if( (PNULL != s_update_prefer_list_ptr)
        &&( 0 == memcmp(&(prefer_cnf->new_prefer_plmn_list),s_update_prefer_list_ptr,sizeof(MN_PHONE_PLMN_SEL_LIST_T))))
    {
        is_update_success = TRUE;
    }

    //SCI_TRACE_LOW:"MMIAPISET_HandlePreferNetworkListCnf,sim=%d,update_sucess=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_13207_112_2_18_2_50_45_129,(uint8*)"dd",prefer_cnf->dual_sys,is_update_success);

    //close wating window
    MMK_CloseWin(MMISET_PREFER_CHANGE_WAIT_WIN_ID);
    s_allow_set_prefer_plmn = TRUE;

    //close priority change window
    MMK_CloseWin(MMISET_NETWORK_PRIORITY_CHANGE_WIN_ID);

    //关闭菜单窗口
    MMK_CloseWin(MMISET_PREFER_NETWORK_OPT_MENU_WIN_ID);                

    if(prefer_cnf->is_ok && is_update_success)
    {
        //弹出提示成功的窗口
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
    }
    else
    {    
        //dispaly failed window        
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
    }
    
    if(PNULL != s_update_prefer_list_ptr)
    {
        SCI_FREE(s_update_prefer_list_ptr);
        s_update_prefer_list_ptr = PNULL;
    }

    if (MMK_IsOpenWin(MMISET_SET_PREFER_NETWORK_WIN_ID))
    {
        MMK_SendMsg(MMISET_SET_PREFER_NETWORK_WIN_ID, MSG_SET_LOAD_PREFER_PLMN_LIST, PNULL);
    }

    return TRUE;
}
#endif
#if defined(MAINLCD_SIZE_176X220) || defined(MAINLCD_SIZE_128X160)
/*****************************************************************************/
//      Description : 设置输入密码窗口的背景色，image 和title
//     Global resource dependence : 
//  Author: chunjie liu
//     Note:
/*****************************************************************************/
LOCAL void SetPwBackground(
                                 MMI_WIN_ID_T    win_id, //窗口的ID
                                 MMI_CTRL_ID_T   ctrl_id,   //the id of control
                                 BOOLEAN is_need_label
                                 )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T bg_rect = MMITHEME_GetClientRect();
    GUI_BOTH_RECT_T  both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifdef MONOCHROME_LCD_SUPPORT 
#else
    GUI_POINT_T dis_point = {0};
    uint16 icon_width = 0;
    uint16 icon_height = 0;     
    //GUI_BOTH_RECT_T  both_rect = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
#endif 
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        bg_rect = both_client_rect.h_rect;
    }
    else
    {
        bg_rect = both_client_rect.v_rect;
    }

#ifdef MONOCHROME_LCD_SUPPORT 
    LCD_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
#else
    LCD_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, IMAGE_NUMBERINPUT_IP, win_id);

    //设置image id
    IMG_EnableTransparentColor(TRUE);
    GUIRES_DisplayImg(PNULL,
            &bg_rect,
            &bg_rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);

    edit_both_rect = GetMinuteReminderEditRect(win_id, is_need_label);

    dis_point.x = (bg_rect.right + bg_rect.left - icon_width) / 2;
    dis_point.y = (bg_rect.bottom + edit_both_rect.v_rect.bottom - icon_height) / 2;
    GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_NUMBERINPUT_IP,
            &lcd_dev_info);
    
    IMG_EnableTransparentColor(FALSE); 
#endif 
}

/*****************************************************************************/
//      Description : GetPinPukEditRect
//     Global resource dependence : 
//  Author: chunjie liu
//     Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetMinuteReminderEditRect(MMI_WIN_ID_T win_id, BOOLEAN is_need_label)
{
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
     return GetPasswordEditBothRect(is_need_label);
#else
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

    both_rect.h_rect.left = both_rect.v_rect.left = MMI_PASSWORD_EDIT_LEFT;
    if (is_need_label)
    {
        both_rect.h_rect.top += MMISET_MINUTE_REMINDER_REMAIN_TOP_MARGIN + MMISET_MINUTE_REMINDER_REMAIN_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
        both_rect.v_rect.top += MMISET_MINUTE_REMINDER_REMAIN_TOP_MARGIN + MMISET_MINUTE_REMINDER_REMAIN_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
    }
    else
    {
        both_rect.h_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
        both_rect.v_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
    }

    both_rect.h_rect.bottom = both_rect.h_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.h_rect.right = both_rect.h_rect.right - MMI_PASSWORD_EDIT_LEFT;
    both_rect.v_rect.bottom = both_rect.v_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.v_rect.right = both_rect.v_rect.right - MMI_PASSWORD_EDIT_LEFT;

    return both_rect;
#endif 
          
}
#endif
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
//      Description : Is Set DualSys MSISDN 
//     Global resource dependence : 
//    Author: dave.ruan
//     Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSetDualSysMSISDN(void)
{
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    BOOLEAN is_dual_sys_msisdn = TRUE;
    
    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        MMIPB_BCD_NUMBER_T sim_number[1]={0};
        
        MMIAPIPB_GetMSISDNumberArrayBySim(sim_number, dual_sys, 0);
        if(0 == sim_number[0].number_len)
        {
            is_dual_sys_msisdn = FALSE;
            break;
        }
    }
    
    return is_dual_sys_msisdn;
}

/*****************************************************************************/
//      Description : Set DualSIM CFNRC
//     Global resource dependence : 
//    Author: dave.ruan
//     Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDualSIMCFNRC(MMI_HANDLE_T win_id)
{
    BOOLEAN is_need_close_msisdn_win = TRUE;
    
    if(MMK_IsOpenWin(MMISET_SET_MULTI_SYS_SETTING_WIN_ID))
    {
        if(IsSetDualSysMSISDN())
        {
            OpenCFNRCWin(); 
        }
        else
        {
            if(!MMK_IsOpenWin(win_id))//MSISDN WIN
            {
                MMIAPIPB_OpenMSISDNWin(FALSE, TRUE);
            }
            else
            {
                //in case close it outside
                is_need_close_msisdn_win = FALSE;
                //alert set pb msisdn first
                MMIPUB_OpenAlertWarningWin(TXT_SET_PB_MSISDN);
//                 MMISmartDS_ForwardFailedDialog(SMARTDS_FORWARDING_NONE, TXT_ERROR);
            }
        }
    }
    
    return is_need_close_msisdn_win;
}

#ifdef MMI_SMART_DUAL_SIM_SUPPORT
/*****************************************************************************/
//  Description : open cfnrc win
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL void SmartDualSIMOpenCFNRCWin(SmartDS_Forwarding_Direction direction)
{
    //judge whether dual sys all call read
    MN_DUAL_SYS_E simForwardStart = MN_DUAL_SYS_1;
	SmartDS_Forwarding_setting = direction;
	if(SMARTDS_FORWARDING_2_TO_1 == direction)
	{
		simForwardStart = MN_DUAL_SYS_2;
	}
    if (MMIPHONE_IsSimOk(MN_DUAL_SYS_1) && MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
    {
        //judge whether dual sys msisdn all ready
        if (MMIAPIPB_IsPbReady())
        {
#if defined(SFR_SUPPORT) && defined(SFR_SUPPORT_CTCC)
            if (MMIAPICONNECTION_isGPRSSwitchedOff())
            {
                MMIPUB_OpenAlertWarningWin(TXT_SYS_OPEN_DATA_SERVICE);
                return;
            }
#endif       
            if (MMISET_RegisterDualsysCFNRC(simForwardStart))
            {
        SCI_TraceLow(
            "smartDS SmartDualSIMOpenCFNRCWin Open waitting win");
                OpenDualsysCFNRCWin();
            }
            else
            {
                //alert fail
                //MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        SCI_TraceLow(
            "smartDS SmartDualSIMOpenCFNRCWin RegisterDualsysCFNRC error.");
                if(MMK_IsOpenWin(MMISET_SET_DUALSYS_CFNRC_WIN_ID))
                {
                    MMK_CloseWin(MMISET_SET_DUALSYS_CFNRC_WIN_ID);
                }
                MMISmartDS_ForwardFailedDialog(SmartDS_Forwarding_setting, TXT_NULL);
            }
        }
        else
        {
            //alert set pb msisdn first
        SCI_TraceLow(
            "smartDS SmartDualSIMOpenCFNRCWin alert set pb msisdn first");
            //MMIPUB_OpenAlertWarningWin(TXT_SET_PB_MSISDN);
            MMISmartDS_ForwardFailedDialog(SMARTDS_FORWARDING_NONE, TXT_ERROR);
        }
    }
    else
    {
        //alert sim not ready
        SCI_TraceLow(
            "smartDS SmartDualSIMOpenCFNRCWin alert sim not ready");
        MMIPUB_OpenAlertWarningWin(STR_SIM_NOT_READY_EXT01);
    }
}
/*****************************************************************************/
// 	Description : Set DualSIM CFNRC
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetSmartDualSIMCFNRC(MMI_HANDLE_T win_id, SmartDS_Forwarding_Direction direction)
{
    BOOLEAN is_need_close_msisdn_win = FALSE;

        if(IsSetDualSysMSISDN())
        {
            SmartDualSIMOpenCFNRCWin(direction); 
			is_need_close_msisdn_win = TRUE;
        }
    return is_need_close_msisdn_win;
}

extern PUBLIC BOOLEAN MMISET_CloseDualsysCFNRC(MN_DUAL_SYS_E dual_sys);
PUBLIC void MMIAPISET_CloseSmartDualSIMCFNRC(SmartDS_Forwarding_Direction direction)
{
    //judge whether dual sys all call read
    MN_DUAL_SYS_E simForwardStart = MN_DUAL_SYS_1;
	SmartDS_Forwarding_setting = direction;
	if(SMARTDS_FORWARDING_CLOSE_2_TO_1 == direction)
	{
		simForwardStart = MN_DUAL_SYS_2;
	}
    //if (MMIPHONE_IsSimOk(MN_DUAL_SYS_1) && MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
    //{
        //judge whether dual sys msisdn all ready
        //if (MMIAPIPB_IsPbReady())
        //{
            if (MMISET_CloseDualsysCFNRC(simForwardStart))
            {
                OpenDualsysCFNRCWin();
            }
            else
            {
                //alert fail
                //MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                if(MMK_IsOpenWin(MMISET_SET_DUALSYS_CFNRC_WIN_ID))
                {
                    MMK_CloseWin(MMISET_SET_DUALSYS_CFNRC_WIN_ID);
                }
                MMISmartDS_ForwardFailedDialog(SmartDS_Forwarding_setting, TXT_NULL);
            }
        //}
        //else
        //{
            //alert set pb msisdn first
        //    MMIPUB_OpenAlertWarningWin(TXT_SET_PB_MSISDN);
        //}
    //}
    //else
    //{
        //alert sim not ready
    //    MMIPUB_OpenAlertWarningWin(TXT_SIM_ISNOT_READY);
    //}
}

/*****************************************************************************/
//  Description : close dual sim set cfnrc window
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseSmartDualSIMCFNRCWin(MN_DUAL_SYS_E dual_sys, BOOLEAN  is_action_success, MN_SS_ERROR_CODE_E  error_code)
{
        SCI_TraceLow(
            "smartDS MMISET_CloseSmartDualSIMCFNRCWin dual_sys=%d;is_action_success=%d;error_code=%d;",
        dual_sys,is_action_success,error_code);
        SCI_TraceLow(
            "smartDS MMISET_CloseSmartDualSIMCFNRCWin SmartDS_Forwarding_setting=%d;s_dualsys_cfnrc_win_close_time=%d;s_dualsys_cfnrc_success_num=%d;s_is_action_dualsys_cfnrc=%d;",
        SmartDS_Forwarding_setting,s_dualsys_cfnrc_win_close_time,s_dualsys_cfnrc_success_num,s_is_action_dualsys_cfnrc);
    if (s_is_action_dualsys_cfnrc)
    {
        if (is_action_success)
        {
            s_dualsys_cfnrc_success_num++;
        }
        else //failed notify which SIM error, and check SIM or CFNRC whether support
        {
            MMI_TEXT_ID_T sim_text_id[] = {
                                                                    TXT_SIM_SEL_SIM1,
                                                                    TXT_SIM_SEL_SIM2,
                                                                    TXT_SIM_SEL_SIM3,
                                                                    TXT_SIM_SEL_SIM4, 
                                                                 }; 
            MMI_STRING_T  sim_string ={0};
            MMI_STRING_T  prompt_text_string = {0};
            MMI_STRING_T  prompt_string = {0};
            wchar blank_wchar[] = {0x20,0x00};
            MN_DUAL_SYS_E another_sim_index = MMI_DUAL_SYS_MAX;
            MMI_STRING_T  another_sim_string ={0};
            MMI_STRING_T  setting_string ={0};
            MMICOM_BANNER_INFO_T banner_info = {0};
            
            s_dualsys_cfnrc_win_close_time = 0;
            s_dualsys_cfnrc_success_num = 0;
            s_is_action_dualsys_cfnrc = FALSE;
            SmartDS_Forwarding_setting = SMARTDS_FORWARDING_NONE;

            banner_info.banner_type = MMI_BANNER_TEXT;
            banner_info.start_point.x = 0;
            banner_info.start_point.y = 0;
            banner_info.text1_id = STR_OPERATION_FAIL_EXT01;
            MMIAPICOM_OpenBannerWin(PNULL, banner_info);

#if 0
            another_sim_index = (MN_DUAL_SYS_1 == dual_sys)? MN_DUAL_SYS_2: MN_DUAL_SYS_1;
            
            MMI_GetLabelTextByLang(sim_text_id[dual_sys], &sim_string);
            MMI_GetLabelTextByLang(TXT_SET_DUALSIM_DIVERT_FAIL_PROMPT, &prompt_text_string);
            MMI_GetLabelTextByLang(STXT_MAIN_SETTING, &setting_string);
            
            if(MN_SS_ERROR_CALL_BARRED == error_code
                ||MN_SS_ERROR_UNEXPECTED_DATA_VALUE == error_code)
            {
                MMI_STRING_T  msisdn_string ={0};
                
                MMI_GetLabelTextByLang(TXT_PB_MSISDN, &msisdn_string);
                MMI_GetLabelTextByLang(sim_text_id[another_sim_index], &another_sim_string);
                
                prompt_string.wstr_ptr = SCI_ALLOCAZ((sim_string.wstr_len + prompt_text_string.wstr_len + another_sim_string.wstr_len + msisdn_string.wstr_len + setting_string.wstr_len+ 5 ) * sizeof(wchar));
                if(PNULL == prompt_string.wstr_ptr) 
                {
                    return;
                }
                prompt_string.wstr_len = sim_string.wstr_len + prompt_text_string.wstr_len + another_sim_string.wstr_len + msisdn_string.wstr_len + setting_string.wstr_len+4;
                
                MMIAPICOM_Wstrncpy(prompt_string.wstr_ptr, sim_string.wstr_ptr, sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, prompt_text_string.wstr_ptr, prompt_text_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, another_sim_string.wstr_ptr, another_sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, msisdn_string.wstr_ptr, msisdn_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, setting_string.wstr_ptr, setting_string.wstr_len);

                //Alert "SIM1 Set Failed, Please check SIM2 Own Number Setting"
                MMIPUB_OpenQueryWinByTextPtr(&prompt_string, PNULL, IMAGE_PUBWIN_QUERY, PNULL, HandleEnterEditMSISDNQueryWin);
            }
            else
            {
                MMI_STRING_T  cfnrc_string ={0};
                uint32 time_period = 0;
                
                MMI_GetLabelTextByLang(TXT_SET_DIVERTS_NOT_REACHABLE, &cfnrc_string);
                
                prompt_string.wstr_ptr = SCI_ALLOCAZ((sim_string.wstr_len + prompt_text_string.wstr_len + sim_string.wstr_len + cfnrc_string.wstr_len + setting_string.wstr_len + 5 ) * sizeof(wchar));
                if(PNULL == prompt_string.wstr_ptr) 
                {
                    return;
                }
                prompt_string.wstr_len = sim_string.wstr_len*2 + prompt_text_string.wstr_len + cfnrc_string.wstr_len + setting_string.wstr_len +4;

                MMIAPICOM_Wstrncpy(prompt_string.wstr_ptr, sim_string.wstr_ptr, sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, prompt_text_string.wstr_ptr, prompt_text_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, sim_string.wstr_ptr, sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, cfnrc_string.wstr_ptr, cfnrc_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, setting_string.wstr_ptr, setting_string.wstr_len);
                
                //Alert "SIM1 set failed, Please Check SIM1 Call Divert unreachable setting"
                MMIPUB_OpenAlertWinByTextPtr(&time_period, &prompt_string, PNULL, IMAGE_PUBWIN_WARNING,
                                                 PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            
            SCI_FREE(prompt_string.wstr_ptr);
#endif
            MMK_CloseWin(MMISET_SET_DUALSYS_CFNRC_WIN_ID);
            return;
        }
        
        s_dualsys_cfnrc_win_close_time++;
        
        //sim1注册完毕后sim2开始注册，避免注册时间间隔太短，注册失败的情况
        if((SmartDS_Forwarding_setting == SMARTDS_FORWARDING_BOTH_SIM)
            || (SmartDS_Forwarding_setting == SMARTDS_FORWARDING_CLOSE_BOTH_SIM))
        {
            if (1 == s_dualsys_cfnrc_win_close_time)
            {
                if ((SmartDS_Forwarding_setting == SMARTDS_FORWARDING_BOTH_SIM) 
                    && MMISET_RegisterDualsysCFNRC(MN_DUAL_SYS_2))
                {
                    //SCI_TRACE_LOW:"mmiset_wintab.c:MMISET_CloseDualsysCFNRCWin,True"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11324_112_2_18_2_50_41_94,(uint8*)"");
                }
                else if((SmartDS_Forwarding_setting == SMARTDS_FORWARDING_CLOSE_BOTH_SIM)
                    && MMISET_CloseDualsysCFNRC(MN_DUAL_SYS_2))
                {
                    SCI_TRACE_LOW("mmiset_wintab.c:MMISET_CloseDualsysCFNRCWin,True,Close CFNRC");
                }
                else
                {
                    //MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                    MMISmartDS_ForwardFailedDialog(SmartDS_Forwarding_setting, TXT_NULL);
                }
                //SCI_TRACE_LOW:"mmiset_wintab.c:MMISET_CloseDualsysCFNRCWin:s_dualsys_cfnrc_win_close_time = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11330_112_2_18_2_50_41_95,(uint8*)"d", s_dualsys_cfnrc_win_close_time);
            }
            //SCI_TRACE_LOW:"mmiset:CloseDualsysCFNRCWin, s_dualsys_cfnrc_win_close_time=%d, action_result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11332_112_2_18_2_50_41_96,(uint8*)"dd", s_dualsys_cfnrc_win_close_time,is_action_success);
            
            if (s_dualsys_cfnrc_win_close_time >= MMI_DUAL_SYS_MAX)
            {
                if((SmartDS_Forwarding_setting <= SMARTDS_FORWARDING_BOTH_SIM)
                    || ((SmartDS_Forwarding_setting > SMARTDS_FORWARDING_BOTH_SIM)
                            && SMARTDS_FORWARDING_NONE == MMISmartDS_getDestForwardType()))
                {
                    MMK_CloseWin(MMISET_SET_DUALSYS_CFNRC_WIN_ID);
                }

                if (s_dualsys_cfnrc_success_num >= MMI_DUAL_SYS_MAX)
                {
                    //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
                    MMISmartDS_ForwardOkDialog(SmartDS_Forwarding_setting);
                }
                else
                {
                    //MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                    MMISmartDS_ForwardFailedDialog(SmartDS_Forwarding_setting, TXT_ERROR);
                }
                SmartDS_Forwarding_setting = SMARTDS_FORWARDING_NONE;
                s_dualsys_cfnrc_win_close_time = 0;
                s_dualsys_cfnrc_success_num = 0;
                s_is_action_dualsys_cfnrc = FALSE;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"mmiset:CloseDualsysCFNRCWin, s_dualsys_cfnrc_win_close_time=%d, action_result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11332_112_2_18_2_50_41_96,(uint8*)"dd", s_dualsys_cfnrc_win_close_time,is_action_success);
        
            if((SmartDS_Forwarding_setting <= SMARTDS_FORWARDING_BOTH_SIM)
                    || ((SmartDS_Forwarding_setting > SMARTDS_FORWARDING_BOTH_SIM)
                            && SMARTDS_FORWARDING_NONE == MMISmartDS_getDestForwardType()))
            {
                MMK_CloseWin(MMISET_SET_DUALSYS_CFNRC_WIN_ID);
            }
            
            if (s_dualsys_cfnrc_success_num > 0)
            {
                //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
                MMISmartDS_ForwardOkDialog(SmartDS_Forwarding_setting);
            }
            else
            {
                //MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                MMISmartDS_ForwardFailedDialog(SmartDS_Forwarding_setting, TXT_ERROR);
            }
            s_dualsys_cfnrc_win_close_time = 0;
            s_dualsys_cfnrc_success_num = 0;
            s_is_action_dualsys_cfnrc = FALSE;
            SmartDS_Forwarding_setting = SMARTDS_FORWARDING_NONE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISET_CloseDualsysCFNRCWin: is not dual sys, do not handle!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11353_112_2_18_2_50_41_97,(uint8*)"");
    }
}

PUBLIC BOOLEAN MMIAPISET_SetSmartDualSIMStandby(BOOLEAN * is_standby_ptr, MN_DUAL_SYS_E dual_sys)
{
    return ImplementSmartDualSIMStandbyChanged(is_standby_ptr, dual_sys);
}

PUBLIC MMI_HANDLE_T MMIAPISET_SmartDualSIMopenFDNbySIM(MN_DUAL_SYS_E dual_sys)
{
	s_cur_dual_sys = dual_sys;
	return MMK_CreateWin((uint32 *)MMISET_SET_FIXED_DIALING_SUB_WIN_TAB,(ADD_DATA)dual_sys);
}
#endif

/*****************************************************************************/
//  Description : Handle Enter Edit MSISDN QueryWin
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEnterEditMSISDNQueryWin(
                                                MMI_WIN_ID_T       win_id,    
                                                MMI_MESSAGE_ID_E      msg_id,    
                                                DPARAM                param      
                                             )
{
    MMI_RESULT_E result =  MMI_RESULT_TRUE;
    

    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMIAPIPB_OpenMSISDNWin(FALSE,TRUE);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;

    }
    return result;
}
#if 0
/*****************************************************************************/
//      Description : to handle dualsys call divert help function, display the help infor
//     Global resource dependence : 
//   Author:  dave.ruan
//     Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDualsysCFNRCHelpWinMsg( 
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T help_string_ptr = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMI_GetLabelTextByLang(TXT_SET_DUAL_HELP, &help_string_ptr);
        GUITEXT_SetString(MMISET_HELP_TEXT_CTRL_ID,help_string_ptr.wstr_ptr,help_string_ptr.wstr_len,FALSE);
        MMK_SetAtvCtrl(win_id, MMISET_HELP_TEXT_CTRL_ID);  
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
#endif
/*Edit by script, ignore 56 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
//[block list in call setting]
#if 0// defined(MMI_BLACKLIST_SUPPORT)   //xiongkai
LOCAL void DisplaybannerInfo(MMI_WIN_ID_T win_id, MMI_TEXT_ID_T text_id)
{
    MMICOM_BANNER_INFO_T banner_info = {0};
    banner_info.banner_type = MMI_BANNER_TEXT;
    banner_info.start_point.x = 0;
    banner_info.start_point.y = 0;
    banner_info.text1_id = text_id;
    MMIAPICOM_OpenBannerWin(win_id, banner_info);
}

/*****************************************************************************/
//  Description : blacklist select contact callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AddBlackListSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISET_SET_BLACKLIST_NEW_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);      
}

/*****************************************************************************/
//  Description : edit blacklist select contact callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EditBlackListSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISET_SET_BLACKLIST_EDIT_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);      
}

/*****************************************************************************/
//  Description : set blacklist select contact callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetBlackListSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);      
}


/*****************************************************************************/
//  Description :InitPdaBlackLIstSettingsListCtrl
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void  InitPdaBlackLIstSettingsListCtrl(void)
{
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    BOOLEAN flag = FALSE;
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID;
    MMI_STRING_T  text_str = {0};

    GUILIST_SetMaxItem(ctrl_id, 3, FALSE );   

    flag =  MMIAPISET_GetIsBlacklistAudioCall();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }
    AppendPdaSettingsItem2Line(
            ctrl_id,
            TXT_SET_BLACKLIST_TYPE_AUDIOCALL,
            text_str,
            image_id,
            (uint32)0);

#ifdef VT_SUPPORT
    flag =  MMIAPISET_GetIsBlacklistVedioCall();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }      
    AppendPdaSettingsItem2Line(            
    ctrl_id,
    TXT_SET_BLACKLIST_TYPE_VEDIOCALL,
    text_str,
    image_id,
    (uint32)0);
#endif


    flag = MMIAPISET_GetIsBlacklistMessage();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }      
    AppendPdaSettingsItem2Line(            
    ctrl_id,
    TXT_SET_BLACKLIST_TYPE_MESSAGE,
    text_str,
    image_id,
    (uint32)0);
}

/*****************************************************************************/
//  Description : to handle security setting blacklist menu
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistMenuWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
   MMI_MENU_GROUP_ID_T          group_id   =   0;
   MMI_MENU_ID_T                menu_id    =   0;
   MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
   MMI_CTRL_ID_T               ctrl_id     =   MMISET_SET_BLACKLIST_CTRL_ID;

   switch(msg_id)
   {
    case MSG_OPEN_WINDOW:
       MMK_SetAtvCtrl(win_id, ctrl_id);
       GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
       break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
       GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
       switch (menu_id)
       {
       case ID_SET_BLACKLIST_LIST:
           MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_LIST_WIN_TAB,PNULL);
           break;

       case ID_SET_BLACKLIST_SETTING:
           MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_TYPE_WIN_TAB,PNULL);
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
        MMISET_FreeBlacklistParam();
        break;

   default:
       recode = MMI_RESULT_FALSE;
       break;
   }

   return recode;
}

/*****************************************************************************/
//  Description : Set Blacklist type
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetBlacklistTypeParam(void)
{
    MMI_STRING_T type_audio_call = {0};
    MMI_STRING_T type_video_call = {0};
    MMI_STRING_T type_message = {0};

    MMI_GetLabelTextByLang(TXT_SET_BLACKLIST_TYPE_AUDIOCALL, &type_audio_call);
    GUILABEL_SetText(MMISET_SET_BLACKLIST_TYPE_AUDIO_CTRL_ID, &type_audio_call, FALSE);

    MMI_GetLabelTextByLang(TXT_SET_BLACKLIST_TYPE_VEDIOCALL, &type_video_call);
    GUILABEL_SetText(MMISET_SET_BLACKLIST_TYPE_VEDIO_CTRL_ID, &type_video_call, FALSE);

    MMI_GetLabelTextByLang(TXT_SET_BLACKLIST_TYPE_MESSAGE, &type_message);
    GUILABEL_SetText(MMISET_SET_BLACKLIST_TYPE_MESSAGE_CTRL_ID, &type_message, FALSE);  
}

/*****************************************************************************/
//  Description : set backlight setlist
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetBlacklistSetlist(MMI_CTRL_ID_T ctrl_id)
{
    uint16 index = 0;
    uint32 loop = 0;
    GUI_BG_T  bg_id = {0};
    uint32 text_id[]=
    {
        TXT_OPEN,
        TXT_CLOSE,
    };
    BOOLEAN is_block = FALSE;
    
    bg_id.img_id = IMAGE_THEME_BG;

    GUISETLIST_SetFontAlign(ctrl_id, ALIGN_LVMIDDLE);
    
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(ctrl_id, text_id[loop]);
    }

    switch(ctrl_id)
    {
        case MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID:
            is_block = MMIAPISET_GetIsBlacklistAudioCall();
            break;

#ifdef VT_SUPPORT
        case MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID:
            is_block = MMIAPISET_GetIsBlacklistVedioCall();
            break;
#endif

        case MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID:
            is_block = MMIAPISET_GetIsBlacklistMessage();
            break;

        default:
            break;
    }
    
    if(is_block)
    {
        index = 0;
    }
    else
    {
        index = 1;
    }
    
    GUISETLIST_SetBackGround(ctrl_id, &bg_id);
    GUISETLIST_SetCurIndex(ctrl_id, index );
    GUISETLIST_SetCtrlState(ctrl_id, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
}


#ifndef MMI_GUI_STYLE_TYPICAL
#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : handle blacklist type win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistTypeWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id     =   MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID;
    uint16 index = 0;
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            InitPdaBlackLIstSettingsListCtrl();
            MMK_SetAtvCtrl(win_id, MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID);
            break;

        case MSG_GET_FOCUS:
        case MSG_SET_BLACKLIST_LIST_UPDATE:
        index = GUILIST_GetCurItemIndex(ctrl_id);        
        GUILIST_RemoveAllItems(ctrl_id);
        InitPdaBlackLIstSettingsListCtrl();
        GUILIST_SetCurItemIndex(ctrl_id,index);  
        break;    
        case MSG_CTL_OK:
        case MSG_APP_OK:

        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:                
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
          {
            index = GUILIST_GetCurItemIndex(ctrl_id);
            if(0 == index)
            {
                   MMISET_SetIsBlacklistAudioCall((BOOLEAN)!MMIAPISET_GetIsBlacklistAudioCall());
            }
            #ifdef VT_SUPPORT
            else if(1 == index)
            {
                    MMISET_SetIsBlacklistVedioCall((BOOLEAN)!MMIAPISET_GetIsBlacklistVedioCall());
            }
            #endif
            else
            {
                 MMISET_SetIsBlacklistMessage((BOOLEAN)!MMIAPISET_GetIsBlacklistMessage());
            }
            MMK_PostMsg(win_id, MSG_SET_BLACKLIST_LIST_UPDATE, PNULL, 0);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
         }
         break;
                 
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
                
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
#endif
#else
/*****************************************************************************/
//  Description : handle blacklist type win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistTypeWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id     =   MMISET_SET_BLACKLIST_CTRL_ID;
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            #ifndef MMI_PDA_SUPPORT
            GUIFORM_SetStyle(MMISET_SET_BLACKLIST_TYPE_FORM_AUDIO_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifdef VT_SUPPORT
            GUIFORM_SetStyle(MMISET_SET_BLACKLIST_TYPE_FORM_VEDIO_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
            GUIFORM_SetStyle(MMISET_SET_BLACKLIST_TYPE_FORM_MESSAGE_CTRL_ID,GUIFORM_STYLE_UNIT);
            #else
            GUIFORM_SetType(MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID,GUIFORM_TYPE_TP);
            GUISETLIST_SetFontAlign(MMISET_SET_BLACKLIST_TYPE_FORM_AUDIO_CTRL_ID,ALIGN_LVMIDDLE);
            GUISETLIST_SetFontAlign(MMISET_SET_BLACKLIST_TYPE_FORM_VEDIO_CTRL_ID,ALIGN_LVMIDDLE);
            GUISETLIST_SetFontAlign(MMISET_SET_BLACKLIST_TYPE_FORM_MESSAGE_CTRL_ID,ALIGN_LVMIDDLE);
            #endif
            SetBlacklistTypeParam();

            SetBlacklistSetlist(MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID);
#ifdef VT_SUPPORT
            SetBlacklistSetlist(MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID);
#endif
            SetBlacklistSetlist(MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID);
            
            MMK_SetAtvCtrl(win_id, MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID);
            break;

        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        #ifndef MMI_PDA_SUPPORT
        case MSG_CTL_OK:
        case MSG_APP_OK:            
        #endif
            ctrl_id= MMK_GetActiveCtrlId(win_id);
            if(msg_id==MSG_CTL_PENOK)
            {
                ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
            }
            switch (ctrl_id)
            {
                case MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID:
                    SaveBlacklistTypeAudio();
                    break;

    #ifdef VT_SUPPORT
                case MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID:
                    SaveBlacklistTypeVideo();
                    break;
    #endif

                case MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID:
                    SaveBlacklistTypeMessage();
                    break;

                default:
                    break;
            }
            break;
            
        case MSG_CTL_SETLIST_SWITCH:
            ctrl_id= MMK_GetActiveCtrlId(win_id);
            switch (ctrl_id)
            {
                case MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID:
                    SaveBlacklistTypeAudio();
                    break;

        #ifdef VT_SUPPORT
                case MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID:
                    SaveBlacklistTypeVideo();
                    break;
        #endif

                case MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID:
                    SaveBlacklistTypeMessage();
                    break;

                default:
                    break;
            }
            break;
     
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
                
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
#endif

/*****************************************************************************/
//  Description : save all audio setting
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SaveBlacklistTypeAudio(void)
{
    uint32 cur_selection = 0;
    BOOLEAN is_block = FALSE;
    
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID); 
    is_block = (BOOLEAN)!cur_selection;
    MMISET_SetIsBlacklistAudioCall(is_block);
}

#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : save all video setting
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SaveBlacklistTypeVideo(void)
{
    uint32 cur_selection = 0;
    BOOLEAN is_block = FALSE;
    
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID); 
    is_block = (BOOLEAN)!cur_selection;
    MMISET_SetIsBlacklistVedioCall(is_block);
}
#endif

/*****************************************************************************/
//  Description : save all message setting
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SaveBlacklistTypeMessage(void)
{
    uint32 cur_selection = 0;
    BOOLEAN is_block = FALSE;
    
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID); 
    is_block = (BOOLEAN)!cur_selection;
    MMISET_SetIsBlacklistMessage(is_block);
}

/*****************************************************************************/
//  Description : handle blacklist win
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetBlackListWindow(
                                            MMI_WIN_ID_T        win_id,     //IN: 窗口的ID
                                            MMI_MESSAGE_ID_E     msg_id,     //IN: 窗口的内部消息ID
                                            DPARAM                 param        //IN: 相应消息的参数
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_BLACKLIST_LIST_CTRL_ID;
    uint16 index = 0;
    MMI_STRING_T                title = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        UpdateBlackList(ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, 0);
        MMK_SetAtvCtrl(win_id, ctrl_id);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;

//     case MSG_FULL_PAINT:
//         MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_MSG_HDR_BLOCK_NUMBER_EXT01);
//         break;

    case MSG_SET_BLACKLIST_LIST_UPDATE:
        UpdateBlackList(ctrl_id);
        index = *(uint16*)param;
        GUILIST_SetCurItemIndex(ctrl_id, index);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;


   #ifdef MMI_PDA_SUPPORT
   case MSG_CTL_LIST_LONGOK:
        MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_ITEM_OPT_WIN_TAB,PNULL);
        break;
   #endif

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_OPT_WIN_TAB,PNULL);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(0 == g_blacklist_ptr->num)
        {
            break;
        }
        else
        {
            //MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_EDIT_WIN_TAB,PNULL);
        }
        break;

    case MMI_SELECT_CONTACT_CNF:
        SetBlacklistFromPB((MMIPB_HANDLE_T)param);
        UpdateBlackList(ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, 0);
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        break;
#ifdef MMI_PDA_SUPPORT
    case MMI_CL_MULTI_SELECT_LIST_RETURN:
        SetBlacklistFromCL();
        UpdateBlackList(ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, 0);
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        break;
#endif
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : update list
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void UpdateBlackList(
                           MMI_CTRL_ID_T    ctrl_id        //IN:
                           )
{
    int32 i = 0;

    MMISET_GetBlackList();

    GUILIST_RemoveAllItems(ctrl_id);

    GUILIST_SetMaxItem( ctrl_id, MMISET_BLACKLIST_MAX_NUM, FALSE );//max item 10

    if(g_blacklist_ptr->num == 0)
    {
        GUILIST_EMPTY_INFO_T empty_info = {0};

        empty_info.text_id = STR_BLOCK_EMPTY_EXT01;
        GUILIST_SetEmptyInfo(ctrl_id,&empty_info);
        return;
    }

    for ( i = g_blacklist_ptr->num; i > 0; i-- )
    {
        AppendBlacklistItem(
                            ctrl_id,
                            i - 1,
                            FALSE
                            );
    }
}

/*****************************************************************************/
//  Description : append list
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AppendBlacklistItem(
                                MMI_CTRL_ID_T             ctrl_id,         //要设置的列表控件ID号 (输入参数)
                                uint16                    pos,
                                BOOLEAN                   is_update
                                )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16 list_index = 0;
    MMI_STRING_T str_num = {0};
    uint16 name_len = 0;

    item_t.item_data_ptr = &item_data;

    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;

    //name
    name_len = MIN(g_blacklist_ptr->blacklist[pos].name_len,MMIPB_NAME_MAX_LEN);
    item_data.item_content[list_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;    
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = name_len;
    
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = g_blacklist_ptr->blacklist[pos].name;

    list_index++;

    //number
    str_num.wstr_len = MIN(g_blacklist_ptr->blacklist[pos].num_len, MMISET_PHONE_NUM_MAX_LEN);

    item_data.item_content[list_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = str_num.wstr_len;

    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = g_blacklist_ptr->blacklist[pos].num;

    //设置功能软键
    item_data.softkey_id[0] = STXT_SOFTKEY_MENU_MK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );      
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );      
    }
}

/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_BLACKLIST_LIST_OPT_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;

    //SCI_TRACE_LOW:"HandleBlacklistOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_7496_112_2_18_2_52_5_329,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(g_blacklist_ptr->num == 0)
        {
            //MMIAPICOM_EnableGrayed(win_id, MENU_MS_BLACKLIST_OPT, ID_SET_BLACKLIST_DEL,TRUE);
            GUIMENU_SetItemVisible(ctrl_id, MENU_BLACKLIST_OPT, ID_SET_BLACKLIST_EDIT,FALSE);
            GUIMENU_SetItemVisible(ctrl_id, MENU_BLACKLIST_OPT, ID_SET_BLACKLIST_DEL,FALSE);
            GUIMENU_SetItemVisible(ctrl_id, MENU_BLACKLIST_OPT,ID_SET_BLACKLIST_DELALL,FALSE);
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        HandleBlacklistMenuMsg(win_id, menu_id,ctrl_id);
        //MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_OK:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlackListItmeOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          )
{

    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_BLACKLIST_ITEM_OPT_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            const GUILIST_ITEM_T* list_item_ptr = PNULL;
            MMI_STRING_T  title_text = {0};
            list_item_ptr = GUILIST_GetItemPtrByIndex( MMISET_SET_BLACKLIST_LIST_CTRL_ID,GUILIST_GetCurItemIndex( MMISET_SET_BLACKLIST_LIST_CTRL_ID));
            if((PNULL !=list_item_ptr )&& (PNULL !=list_item_ptr->item_data_ptr))
            {
                title_text = list_item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
                GUIMENU_SetMenuTitle(&title_text,  ctrl_id);
            }
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;

     case MSG_APP_WEB:
     case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
         {
            GUIMENU_GROUP_T *cur_group_ptr = PNULL;
            uint16 item_cnt =0;
        
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        
            //读取静态菜单数据个数
            cur_group_ptr = MMI_GetMenuGroup(group_id);
            if (PNULL != cur_group_ptr)
            {
                item_cnt = cur_group_ptr->item_count;
            }
        
            /*if((ID_SET_BLACKLIST_DEL == menu_id)&&(item_cnt ==1))//只有唯一的菜单--删除
            {
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,PNULL,HandleBlacklistDelQueryWinMsg);
                MMK_CloseWin(win_id);
            }
            else*/
            {
                HandleBlacklistMenuMsg(win_id, menu_id,ctrl_id);
                //MMK_CloseWin(win_id);
            }
         }
        break;
   
    case MSG_CTL_CANCEL:   
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
            
    default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
#endif

LOCAL void UpdateEditBlackNumberDialogSK(MMI_WIN_ID_T win_id, MMI_HANDLE_T ctrl_handle)
{
    MMI_TEXT_ID_T txt_id = STXT_SOFTKEY_SEARCH_MK;
    MMI_IMAGE_ID_T icon_id = TXT_COMMON_COMMON_BACK;
    MMI_STRING_T edit_str = {0};

    GUIEDIT_GetString(ctrl_handle, &edit_str);
    if (edit_str.wstr_len > 0)
    {
        txt_id = STXT_SOFTKEY_OK_MK;
        icon_id = STR_LIST_DELETE;
    }

    GUIWIN_SeSoftkeytButtonTextId(win_id, txt_id, MIDDLE_BUTTON, FALSE);
    GUIWIN_SeSoftkeytButtonTextId(win_id, icon_id, RIGHT_BUTTON, FALSE);
}

LOCAL MMI_RESULT_E   HandleEditBlackNumberMsg(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E             result = MMI_RESULT_TRUE;
    MMI_STRING_T   number = {0};
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIEDIT_SetTextMaxLen(ctrl_handle, MMISET_PHONE_NUM_MAX_LEN, MMISET_PHONE_NUM_MAX_LEN);
            //GUIEDIT_SetIm(ctrl_handle,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);

            MMIPUB_SetWinSoftkey(win_id, STXT_MAIN_PB, TXT_COMMON_COMMON_BACK, FALSE);
        }
        break;

    case MSG_FULL_PAINT:
        GUIEDIT_SetIm(ctrl_handle,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
        UpdateEditBlackNumberDialogSK(win_id, ctrl_handle);
        //MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_handle, &number);
        if (number.wstr_len == 0)
        {
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_RADIO, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER,
                    1,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    AddBlackListSelectContactCallback
                    );
        }
        else if(SaveBlackListRecord(g_blacklist_ptr->num, TRUE, number, number))
        {
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE); 
            //[after add number to block list,call log will delete logs]
            MMIAPICL_DeleteLogsAfterAddBlockListW(
                    g_blacklist_ptr->blacklist[g_blacklist_ptr->num - 1].num);
            //[end]
            MMK_PostMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MSG_SET_BLACKLIST_LIST_UPDATE, (DPARAM)&g_blacklist_ptr->num, sizeof(uint16));
#ifdef HERO_ENGINE_TTS_SUPPORT
{
	extern uint8 s_hero_no_tts;
	s_hero_no_tts = 1;
}
#endif
            DisplaybannerInfo(win_id, STR_CL_NUMBER_BLOCKED_EXT01);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        UpdateEditBlackNumberDialogSK(win_id, ctrl_handle);
        break;

    case MMI_SELECT_CONTACT_CNF:
        // get the number from the PB
        if (MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param) > 0)
        {
            SetBlacklistDetailFromPB((MMIPB_HANDLE_T)param, ctrl_handle);
        }
        break;
    case MSG_CLOSE_WINDOW:
        break;

    default:
        result = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleBlacklistMenuMsg(
                                 MMI_WIN_ID_T        prev_win_id,
                                 MMI_MENU_ID_T menu_id,    //IN: 被选中的菜单项
                                 MMI_CTRL_ID_T ctrl_id
                                 )
{
    uint32 user_data = 0;

    switch (menu_id)
    {
    case ID_SET_BLACKLIST_NEW:
        MMK_CloseWin(prev_win_id);
//         MMIAPISET_SetAllBlacklistSetting(TRUE);	
        if(g_blacklist_ptr->num == MMISET_BLACKLIST_MAX_NUM)
        {
            //MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);
            DisplaybannerInfo(PNULL,STR_CL_BLOCK_LIST_FULL);
        }
        else  if(!MMIAPISET_GetIsBlacklistAudioCall()
#ifdef VT_SUPPORT
                &&!MMIAPISET_GetIsBlacklistVedioCall()
#endif
                &&!MMIAPISET_GetIsBlacklistMessage()
                )
        {
            user_data = ID_SET_BLACKLIST_NEW;
            MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_OPEN_BLACKLIST_QUERY,IMAGE_PUBWIN_QUERY,PNULL,HandleBlacklistAddQueryWinMsg,(uint32)user_data);
        }
        else
        {
            MMI_STRING_T label_id_str = {0};
#ifdef HERO_ENGINE_TTS_SUPPORT
{
	extern uint8 s_hero_no_tts;
	s_hero_no_tts = 1;
}
#endif
            MMI_GetLabelTextByLang(STXT_PHONE_NUMBER, &label_id_str);
            MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_NEW_WIN_TAB, (ADD_DATA)ctrl_id);
//             MMIPUB_OpenDialogInputFieldWinByTextPtr(&label_id_str, PNULL, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleEditBlackNumberMsg);
        }
        break;

    case ID_SET_BLACKLIST_EDIT:
        MMK_CloseWin(prev_win_id);
        MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_EDIT_WIN_TAB,PNULL);
        break;

    case ID_SET_BLACKLIST_DEL:
        MMK_CloseWin(prev_win_id);
    {
        MMI_STRING_T number = {0};
        MMI_STRING_T title_str = {0};
        wchar    wstr_title_str[100] = {0};
        uint16 index = GUILIST_GetCurItemIndex(MMISET_SET_BLACKLIST_LIST_CTRL_ID);
        uint16 real_index = g_blacklist_ptr->num - index - 1;

        number.wstr_ptr = g_blacklist_ptr->blacklist[real_index].num;
        number.wstr_len = g_blacklist_ptr->blacklist[real_index].num_len;
        title_str.wstr_ptr = wstr_title_str;

//        MMIAPICOM_CustomGenerateReplaceStrByTextId(STR_QUERY_REMOVE_NUM_EXT01, L"%U", number.wstr_ptr, 100, &title_str);

        MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,PNULL,HandleBlacklistDelQueryWinMsg);
        //MMIPUB_OpenConfirmationDialogByTextPtr(PNULL, &title_str, PNULL,
//                         IMAGE_NULL, PNULL, PNULL, MMIPUB_SOFTKEY_OKCANCEL, HandleBlacklistDelQueryWinMsg);
        break;
    }
    case ID_SET_BLACKLIST_CONTACT:
        MMK_CloseWin(prev_win_id);
        if(g_blacklist_ptr->num == MMISET_BLACKLIST_MAX_NUM)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);
        }
        else if(!MMIAPISET_GetIsBlacklistAudioCall()
#ifdef VT_SUPPORT
                &&!MMIAPISET_GetIsBlacklistVedioCall()
#endif
                &&!MMIAPISET_GetIsBlacklistMessage()
                )
        {
            user_data = ID_SET_BLACKLIST_CONTACT;
            MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_OPEN_BLACKLIST_QUERY,IMAGE_PUBWIN_QUERY,PNULL,HandleBlacklistAddQueryWinMsg,(uint32)user_data);
        }
        else
        {
            /*MMIAPIPB_OpenNumberList(
                                MMIPB_LIST_FOR_SMS,
                                MMIPB_LIST_CHECK,
                                MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num,
                                PNULL
                                );*/
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER,
                    MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    SetBlackListSelectContactCallback
                    );
        }
        break;

    case ID_SET_BLACKLIST_DELALL:
        MMK_CloseWin(prev_win_id);
        MMIPUB_OpenQueryWinByTextId(TXT_DELETE_ALL_QUERY,IMAGE_PUBWIN_QUERY,PNULL,HandleBlacklistDelAllQueryWinMsg);
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : handle blacklist new win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNewBlacklistWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T edit_string = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        #ifndef MMI_PDA_SUPPORT
        //GUIFORM_SetStyle(MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        //GUIFORM_SetStyle(MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        #else
        GUIFORM_SetType(MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID,GUIFORM_TYPE_TP);
        #endif
        InitBlacklistInfo();
        GUIEDIT_SetStyle(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID,GUIEDIT_STYLE_MULTI_DYN_DOWN);
        GUIEDIT_SetStyle(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID,GUIEDIT_STYLE_MULTI_DYN_DOWN);
        #ifndef MMI_PDA_SUPPORT
        GUIFORM_SetType(MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID,GUIFORM_TYPE_TP);        
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, STXT_MAIN_PB, STXT_RETURN, FALSE);
        #endif
        MMK_SetAtvCtrl(win_id, MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID);
        break;
#ifndef MMI_PDA_SUPPORT    
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if(g_blacklist_ptr->num == MMISET_BLACKLIST_MAX_NUM)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);            
        }
        else
        {
            if(SaveBlacklistInfo(g_blacklist_ptr->num,TRUE))
            {
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE); 
                //[after add number to block list,call log will delete logs]
                MMIAPICL_DeleteLogsAfterAddBlockListW(
                        g_blacklist_ptr->blacklist[g_blacklist_ptr->num - 1].num);
                //[end]
                MMK_PostMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MSG_SET_BLACKLIST_LIST_UPDATE, (DPARAM)&g_blacklist_ptr->num, sizeof(uint16));
                MMK_CloseWin(win_id);
            }
        }
        break;
  
    
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
       /* MMIAPIPB_OpenNumberList(
                                MMIPB_LIST_FOR_SMS,
                                MMIPB_LIST_RADIO,
                                1,
                                PNULL
                                );*/
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_RADIO, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER,
                    1,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    AddBlackListSelectContactCallback
                    );
        break;
#else
     case MSG_APP_WEB:
     case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                if(g_blacklist_ptr->num == MMISET_BLACKLIST_MAX_NUM)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);            
                }
                else
                {
                    if(SaveBlacklistInfo(g_blacklist_ptr->num,TRUE))
                    {
#ifdef MMI_GUI_STYLE_TYPICAL
                        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);            
#endif
                        MMK_PostMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MSG_SET_BLACKLIST_LIST_UPDATE, (DPARAM)&g_blacklist_ptr->num, sizeof(uint16));
                        MMK_CloseWin(win_id);
                    }
                }

                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
               /* MMIAPIPB_OpenNumberList(
                                MMIPB_LIST_FOR_SMS,
                                MMIPB_LIST_RADIO,
                                1,
                                PNULL
                                );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_RADIO, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_NUMBER,
                        1,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        AddBlackListSelectContactCallback
                        );	
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);
                break;           
            default:
                break;
            }
        }
        break;
#endif

    case MMI_SELECT_CONTACT_CNF:
        // get the number from the PB
        if (MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param) > 0)
        {
            SetBlacklistDetailFromPB((MMIPB_HANDLE_T)param, MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID);
        }
        break;
        
    case MSG_CTL_CANCEL:
        SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
        GUIEDIT_GetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID, &edit_string);

        if(edit_string.wstr_len == 0)
        {
            // close the edit window
            MMK_CloseWin(win_id);
        }
        else
        {
            MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }
        break;

   case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

   case MSG_PROMPTWIN_CANCEL: 
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
            
    default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle blacklist edit win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditBlacklistWindow(
                                           MMI_WIN_ID_T    win_id,
                                           MMI_MESSAGE_ID_E   msg_id,
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 index = 0;
    uint16 pos = 0;
    MMIPB_BCD_NUMBER_T *sender_ptr = ( MMIPB_BCD_NUMBER_T * )MMK_GetWinAddDataPtr(win_id);
    BOOLEAN is_new = FALSE;
    MMI_STRING_T edit_string = {0};

    if(PNULL == sender_ptr)
    {
        index = GUILIST_GetCurItemIndex(MMISET_SET_BLACKLIST_LIST_CTRL_ID);
        pos = g_blacklist_ptr->num - index - 1;
    }
    else
    {
        is_new = TRUE;
        pos = g_blacklist_ptr->num;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //GUIFORM_SetStyle(MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        //GUIFORM_SetStyle(MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        SetBlacklistInfo(pos,sender_ptr);
        GUIEDIT_SetStyle(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID,GUIEDIT_STYLE_MULTI_DYN_DOWN);
        GUIEDIT_SetStyle(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID,GUIEDIT_STYLE_MULTI_DYN_DOWN);
        #ifndef MMI_PDA_SUPPORT
        GUIFORM_SetType(MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID,GUIFORM_TYPE_TP);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, STXT_MAIN_PB, STXT_RETURN, FALSE);
        #endif
        MMK_SetAtvCtrl(win_id, MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    //case MSG_APP_MENU:
        if(SaveBlacklistInfo(pos,is_new))
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);           
#endif
            MMK_PostMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MSG_SET_BLACKLIST_LIST_UPDATE, (DPARAM)&index, sizeof(uint16));
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif
       /* MMIAPIPB_OpenNumberList(
                                MMIPB_LIST_FOR_SMS,
                                MMIPB_LIST_RADIO,
                                1,
                                PNULL
                                );*/
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_RADIO, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER,
                    1,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditBlackListSelectContactCallback
                    );
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;

            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
                break;

            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;

            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

    case MMI_SELECT_CONTACT_CNF:
        // get the number from the PB
        if (MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param) > 0)
        {
            SetBlacklistDetailFromPB((MMIPB_HANDLE_T)param, MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID);
        }
        break;

    case MSG_CTL_CANCEL:
        SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
        GUIEDIT_GetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID, &edit_string);

        if(edit_string.wstr_len == 0)
        {
            // close the edit window
            MMK_CloseWin(win_id);			
        }
        else
        {
            MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }
        break;

   case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

   case MSG_PROMPTWIN_CANCEL: 
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if(!MMK_IsOpenWin(MMISET_SET_BLACKLIST_LIST_WIN_ID))
        {
            MMISET_FreeBlacklistParam();
            if(PNULL != s_new_blacklist_num_ptr)
            {
                SCI_FREE(s_new_blacklist_num_ptr);
                s_new_blacklist_num_ptr = PNULL;
            }
        }
        break;

    default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Set Blacklist info
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void InitBlacklistInfo(void)
{
    wchar name[MMIPB_NAME_MAX_LEN + 1] = {0};
    uint16 name_len = 0;
    wchar num[MMISET_PHONE_NUM_MAX_LEN + 1] = {0};
    uint16 num_len = 0;
    MMI_STRING_T str_name = {0};
    MMI_STRING_T str_num = {0};    
    
    MMI_GetLabelTextByLang(TXT_NAME, &str_name);
    GUILABEL_SetText(MMISET_SET_BLACKLIST_NEW_NAME_LABEL_CTRL_ID, &str_name, FALSE);

    MMI_GetLabelTextByLang(TXT_COMMON_NUMBER, &str_num);
    GUILABEL_SetText(MMISET_SET_BLACKLIST_NEW_NUM_LABEL_CTRL_ID, &str_num, FALSE);

    GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID, name, name_len);
    GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID, num, num_len);

    return;
}

LOCAL BOOLEAN SaveBlackListRecord(
                                uint16 index,
                                BOOLEAN is_new,
                                MMI_STRING_T str_name,
                                MMI_STRING_T str_num
                               )
{
    BOOLEAN result = FALSE;
    uint16 name_len = 0;    
    uint16 num_len = 0;

    if(0 == str_num.wstr_len)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_INPUT_NUM);
        result = FALSE;
    }
    else if(!MMIAPICOM_IsValidNumberStringEx(&str_num))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        result = FALSE;        
    }
    else
    {
        uint16 j=0;
        
        for(j=0; j<g_blacklist_ptr->num;j++)
        {
            uint32 cmp_count =0;
            cmp_count = (uint32)MAX(g_blacklist_ptr->blacklist[j].num_len,str_num.wstr_len);
            if(!MMIAPICOM_Wstrncmp((const wchar *)g_blacklist_ptr->blacklist[j].num, str_num.wstr_ptr, cmp_count)
            &&( j != index))//for edit blacklist and modfy nothing
            {
                MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
                return FALSE;
            }
        }

        name_len = MIN(str_name.wstr_len,MMIPB_NAME_MAX_LEN);
        g_blacklist_ptr->blacklist[index].name_len = name_len;
        MMI_WSTRNCPY(
                     g_blacklist_ptr->blacklist[index].name,
                     MMIPB_NAME_MAX_LEN,
                     str_name.wstr_ptr,
                     name_len,
                     name_len);

        num_len = MIN(str_num.wstr_len,MMISET_PHONE_NUM_MAX_LEN);
        g_blacklist_ptr->blacklist[index].num_len = num_len;
        MMI_WSTRNCPY(
                     g_blacklist_ptr->blacklist[index].num,
                     MMISET_PHONE_NUM_MAX_LEN,
                     str_num.wstr_ptr,
                     num_len,
                     num_len);        

        if(is_new)
        {
            g_blacklist_ptr->num++;
        }

        MMISET_SetBlackList(g_blacklist_ptr);
        result = TRUE;
    }

    return result;
}


/*****************************************************************************/
//  Description : Save Blacklist info
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SaveBlacklistInfo(
                                uint16 index,
                                BOOLEAN is_new
                               )
{
    MMI_STRING_T str_name = {0};
    MMI_STRING_T str_num = {0}; 
    BOOLEAN result = FALSE;
    uint16 name_len = 0;    
    uint16 num_len = 0;

    GUIEDIT_GetString(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID,&str_name);
    GUIEDIT_GetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID,&str_num);

    return SaveBlackListRecord(index, is_new, str_name, str_num);
}

/*****************************************************************************/
//  Description : Set Blacklist info
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetBlacklistInfo(
                            uint16 index,
                            MMIPB_BCD_NUMBER_T *sender_ptr
                            )
{
    MMI_STRING_T str_name = {0};
    MMI_STRING_T str_num = {0};    
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0}; 
    uint8 max_len = 0;
    uint8 tele_len = 0 ;
    wchar num[MMISET_PHONE_NUM_MAX_LEN] = {0};
    BOOLEAN find_result = FALSE;
    MMI_STRING_T name_str = {0};
    wchar str_ptr[MMIPB_MAX_STRING_LEN + 1] = {0};

    MMI_GetLabelTextByLang(TXT_NAME, &str_name);
    GUILABEL_SetText(MMISET_SET_BLACKLIST_NEW_NAME_LABEL_CTRL_ID, &str_name, FALSE);

    MMI_GetLabelTextByLang(TXT_COMMON_NUMBER, &str_num);
    GUILABEL_SetText(MMISET_SET_BLACKLIST_NEW_NUM_LABEL_CTRL_ID, &str_num, FALSE);

    if(PNULL == sender_ptr)
    {
        GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID, g_blacklist_ptr->blacklist[index].name, g_blacklist_ptr->blacklist[index].name_len);
        GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID, g_blacklist_ptr->blacklist[index].num, g_blacklist_ptr->blacklist[index].num_len);
    }
    else
    {
        if(PNULL != s_new_blacklist_num_ptr)
        {
            SCI_MEMSET((uint8 *)str_ptr, 0x00, sizeof(str_ptr));
            name_str.wstr_len = 0;
            name_str.wstr_ptr = str_ptr;
            find_result = MMIAPIPB_GetNameByNumber(s_new_blacklist_num_ptr, &name_str, MMIPB_MAX_STRING_LEN, FALSE);
            if(find_result)
            {
                GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID,name_str.wstr_ptr,name_str.wstr_len);                   
            }
        }

        max_len = MIN(MMISET_PHONE_NUM_MAX_LEN, sender_ptr->number_len);
        tele_len = MMIAPIPB_BCDNumberToString(sender_ptr, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);
//         tele_len = MMIAPICOM_GenDispNumber( 
//                                             MMIPB_GetNumberTypeFromUint8(sender_ptr->npi_ton), 
//                                             max_len, 
//                                             sender_ptr->number,  
//                                             tele_num,
//                                             MMISET_PHONE_NUM_MAX_LEN + 2
//                                             );

        tele_len = MIN(MMISET_PHONE_NUM_MAX_LEN, tele_len);

        MMI_STRNTOWSTR(
                       num,
                       tele_len,
                       tele_num,
                       tele_len,
                       tele_len
                       );
        GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID, num, tele_len);
    }

    return;
}

/*****************************************************************************/
//  Description : to handle the blacklist of delete query window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    uint16 index = 0;
    uint16 real_index = 0;

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(MMISET_SET_BLACKLIST_LIST_CTRL_ID);
        real_index = g_blacklist_ptr->num - index - 1;
        DeleteBlacklistItem(real_index);
#ifdef HERO_ENGINE_TTS_SUPPORT
{
	extern uint8 s_hero_no_tts;
	s_hero_no_tts = 1;
}
#endif
#ifndef MMI_PDA_SUPPORT
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        DisplaybannerInfo(win_id, STR_MSG_NOTE_SCR_NUM_DEL_EXT02);
        MMK_PostMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MSG_SET_BLACKLIST_LIST_UPDATE, (DPARAM)&index, sizeof(uint16));
        //关闭询问窗口
//        recode = MMIPUB_HandleConfirmationDialogWinMsg(win_id, msg_id, param);
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //关闭询问窗口
//        recode = MMIPUB_HandleConfirmationDialogWinMsg(win_id, msg_id, param);
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        MMK_CloseWin(win_id);
        break;

    default:
//        recode = MMIPUB_HandleConfirmationDialogWinMsg(win_id, msg_id, param);
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : to handle the blacklist of delete all query window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistDelAllQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    uint16 index = 0;
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        DeleteAllBlacklistItem();
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);            
        MMK_PostMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MSG_SET_BLACKLIST_LIST_UPDATE, (DPARAM)&index, sizeof(uint16));
        //关闭询问窗口
        //MMIPUB_CloseQuerytWin(PNULL);
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //关闭询问窗口
        //MMIPUB_CloseQuerytWin(PNULL);
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        MMK_CloseWin(win_id);
        break;

    default:
        //recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : to handle the blacklist of delete window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MN_RETURN_RESULT_E DeleteBlacklistItem(
                                             uint16 pos
                                             )
{
    uint32    i = 0;
    MN_RETURN_RESULT_E return_val = MN_RETURN_SUCCESS;
    
    if(pos > (g_blacklist_ptr->num - 1))
    {
        return MN_RETURN_FAILURE;
    }
    
    //将删除位置以后的所有项前移
    for (i=pos+1; i<g_blacklist_ptr->num; i++)
    {
        //g_blacklist_ptr->blacklist[i-1] = g_blacklist_ptr->blacklist[i];
        SCI_MEMCPY(
                   &g_blacklist_ptr->blacklist[i-1],
                   &g_blacklist_ptr->blacklist[i],
                   sizeof(MMISET_BLACKLIST_ITEM_T));
    }
    
    g_blacklist_ptr->num--;
    
    MMISET_SetBlackList(g_blacklist_ptr);
    
    return return_val;
}

/*****************************************************************************/
//  Description : to handle the blacklist of delete all window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MN_RETURN_RESULT_E DeleteAllBlacklistItem(void)
{
    MN_RETURN_RESULT_E return_val = MN_RETURN_SUCCESS;
    
    g_blacklist_ptr->num = 0;

    SCI_MEMSET(g_blacklist_ptr,0x00,sizeof(MMISET_BLACKLIST_T));
    
    MMISET_SetBlackList(g_blacklist_ptr);
    
    return return_val;
}

/*****************************************************************************/
//  Description : set info from PB  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistDetailFromPB(MMIPB_HANDLE_T handle, MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN return_val = TRUE;
    MMIPB_BCD_NUMBER_T number = {0};
    uint8 tele_len = 0;
    uint8 max_len = 0;
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    wchar num[MMISET_PHONE_NUM_MAX_LEN + 1] = {0};
    MMI_STRING_T name_str = {0};
    wchar str_ptr[MMIPB_MAX_STRING_LEN + 1] = {0};

    SCI_MEMSET((uint8 *)str_ptr, 0x00, sizeof(str_ptr));
    name_str.wstr_len = 0;
    name_str.wstr_ptr = str_ptr;

    MMIAPIPB_GetSelectNumber(handle, &number,MMIPB_BCD_NUMBER_MAX_LEN, 0);
    MMIAPIPB_GetSelectName(handle, &name_str,MMIPB_NAME_MAX_LEN,0);
    if(0 == name_str.wstr_len)//name is empty, clean the privous name string
    {
        GUIEDIT_ClearAllStr(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID);
    }
    else
    {
        GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID, name_str.wstr_ptr, name_str.wstr_len);
    }

    max_len = MIN(MMISET_PHONE_NUM_MAX_LEN, number.number_len);
    tele_len = MMIAPIPB_BCDNumberToString(&number, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);

    MMI_STRNTOWSTR(
                   num, 
                   MMISET_PHONE_NUM_MAX_LEN + 2, 
                   tele_num, 
                   MMISET_PHONE_NUM_MAX_LEN + 2, 
                   tele_len
                   );

    GUIEDIT_SetString(ctrl_id, num, tele_len);
    return return_val;
}
#ifdef MMI_PDA_SUPPORT 

LOCAL uint32 GetNextClIndex(uint32 cur_index)
{   
    uint32 i,index = 0;

    for(i=0;i<MMICL_RECORD_MAX_NUM;i++)
    {
      if (MMIAPICL_SelectedListIsValid(i))
        {
            if (index == cur_index)
            {
                return i;
            }
            index ++;
        }
    }
    return MMICL_RECORD_MAX_NUM;
}




/*****************************************************************************/
//  Description : set detail from CL  
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistFromCL(void)
{
    BOOLEAN return_val = TRUE;
    MMIPB_BCD_NUMBER_T number = {0};
    uint8 tele_len = 0;
    uint8 max_len = 0;
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    MMI_STRING_T name_str = {0};
    uint16 count = MMIAPICL_GetSelectedItemCount();
    int32 i = 0;
    BOOLEAN once_duplicated= FALSE;
    wchar name_wstr[MMIPB_NAME_MAX_LEN+1]={0};
    uint32 cl_index = 0;

    if ((MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num) < count)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_BEYOND_MAX_NUM);
        return FALSE;
    }
    cl_index= 0;
    for (i=0; i<MIN(MMISET_BLACKLIST_MAX_NUM, count); i++)
    {
        uint16 j= 0;
        BOOLEAN is_duplicate = FALSE;
        uint16 total_num = 0;

        cl_index = GetNextClIndex(i);
        if(cl_index>=MMICL_RECORD_MAX_NUM)
        {
            break;
        }
        
        SCI_MEMSET(name_wstr,0x00,(MMIPB_NAME_MAX_LEN+1)*sizeof(wchar));   
        SCI_MEMSET(&name_str,0,sizeof(name_str));
        name_str.wstr_ptr = name_wstr;
        // read the phone number from phonebook one by one
        SCI_MEMSET(&number, 0, sizeof(MMIPB_BCD_NUMBER_T));
        
        MMIAPICL_GetSelectPhonenum(&number,cl_index);

        MMIAPICL_GetSelectPhoneName(&name_str,cl_index,MMIPB_NAME_MAX_LEN,&number);

        max_len = MIN(MMISET_PHONE_NUM_MAX_LEN, number.number_len);
        tele_len = MMIAPIPB_BCDNumberToString(&number, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);
        for(j=0; j<g_blacklist_ptr->num;j++)
        {
            uint32 cmp_count =0;
            cmp_count = (uint32)MAX(g_blacklist_ptr->blacklist[j].num_len,tele_len);
            if(!MMIAPICOM_WstrStrncmp((const wchar *)g_blacklist_ptr->blacklist[j].num, tele_num, cmp_count))
            {
                is_duplicate =TRUE;
                once_duplicated = TRUE;
                //MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
                //break;
                break;
            }
        }
        if(!is_duplicate)
        {
            total_num = g_blacklist_ptr->num;
            g_blacklist_ptr->blacklist[total_num].name_len = name_str.wstr_len;
            
            MMI_WSTRNCPY(
                         g_blacklist_ptr->blacklist[total_num].name,
                         MMIPB_NAME_MAX_LEN,
                         name_str.wstr_ptr,
                         MMIPB_NAME_MAX_LEN,
                         name_str.wstr_len
                         );

            g_blacklist_ptr->blacklist[total_num ].num_len = tele_len;
            MMI_STRNTOWSTR(
                           g_blacklist_ptr->blacklist[total_num].num, 
                           MMISET_PHONE_NUM_MAX_LEN + 2, 
                           tele_num, 
                           MMISET_PHONE_NUM_MAX_LEN + 2, 
                           tele_len
                           );
            g_blacklist_ptr->num++;
            MMISET_SetBlackList(g_blacklist_ptr);
        }
    }
    if (once_duplicated)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
    }
       
    return return_val;
}
#endif
/*****************************************************************************/
//  Description : set list from PB  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistFromPB(MMIPB_HANDLE_T handle)
{
    BOOLEAN return_val = TRUE;
    MMIPB_BCD_NUMBER_T number = {0};
    uint8 tele_len = 0;
    uint8 max_len = 0;
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    MMI_STRING_T name_str = {0};
    uint16 count = MMIAPIPB_GetSelectCount(handle);
    int32 i = 0;

    if ((MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num) < count)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_BEYOND_MAX_NUM);
        return FALSE;
    }

    name_str.wstr_ptr = SCI_ALLOCA((MMIPB_NAME_MAX_LEN+1)*sizeof(wchar));
    if(PNULL == name_str.wstr_ptr)
    {
        return FALSE;
    }

    for (i=0; i<MIN(MMISET_BLACKLIST_MAX_NUM, count); i++)
    {
        uint16 j= 0;
        BOOLEAN is_duplicate = FALSE;
        uint16 total_num = 0;

        // read the phone number from phonebook one by one
        SCI_MEMSET(&number, 0, sizeof(MMIPB_BCD_NUMBER_T));

        MMIAPIPB_GetSelectNumber(handle, &number,MMIPB_BCD_NUMBER_MAX_LEN, i);

        SCI_MEMSET(name_str.wstr_ptr,0x00,(MMIPB_NAME_MAX_LEN+1)*sizeof(wchar));
        MMIAPIPB_GetNameByNumber(&number,
                                  &name_str,
                                  MMIPB_NAME_MAX_LEN,
                                  FALSE  //use default name,"no name"
                                  );
        max_len = MIN(MMISET_PHONE_NUM_MAX_LEN, number.number_len);
        tele_len = MMIAPIPB_BCDNumberToString(&number, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);
//         tele_len = MMIAPICOM_GenDispNumber(
//                                             MMIPB_GetNumberTypeFromUint8(number.npi_ton),
//                                             max_len,
//                                             number.number, 
//                                             tele_num,
//                                             MMISET_PHONE_NUM_MAX_LEN + 2
//                                             );

        if(0 == tele_len)
        {
            MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
            continue;
        }//add by yongsheng.wang
        for(j=0; j<g_blacklist_ptr->num;j++)
        {
            uint32 cmp_count =0;
            cmp_count = (uint32)MAX(g_blacklist_ptr->blacklist[j].num_len,tele_len);
            if(!MMIAPICOM_WstrStrncmp((const wchar *)g_blacklist_ptr->blacklist[j].num, tele_num, cmp_count))
            {
                is_duplicate =TRUE;
                MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
                break;
            }
        }
        if(!is_duplicate)
        {
            total_num = g_blacklist_ptr->num;
            g_blacklist_ptr->blacklist[total_num].name_len = name_str.wstr_len;
            
            MMI_WSTRNCPY(
                         g_blacklist_ptr->blacklist[total_num].name,
                         MMIPB_NAME_MAX_LEN,
                         name_str.wstr_ptr,
                         MMIPB_NAME_MAX_LEN,
                         name_str.wstr_len
                         );

            g_blacklist_ptr->blacklist[total_num ].num_len = tele_len;
            MMI_STRNTOWSTR(
                           g_blacklist_ptr->blacklist[total_num].num, 
                           MMISET_PHONE_NUM_MAX_LEN + 2, 
                           tele_num, 
                           MMISET_PHONE_NUM_MAX_LEN + 2, 
                           tele_len
                           );
            g_blacklist_ptr->num++;
            MMISET_SetBlackList(g_blacklist_ptr);
        }
    }

    if(PNULL != name_str.wstr_ptr)
    {
        SCI_FREE(name_str.wstr_ptr);
    }

    return return_val;
}

/*****************************************************************************/
//  Description : open blacklit edit win
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_OpenBlacklistEditWin(MMIPB_BCD_NUMBER_T *orginal_addr_ptr)
{
    if (PNULL == orginal_addr_ptr)
    {
        return;
    }

    s_new_blacklist_num_ptr = SCI_ALLOCA(sizeof(MMIPB_BCD_NUMBER_T));
    if(PNULL == s_new_blacklist_num_ptr)
    {
        return;
    }

    SCI_MEMSET(
               s_new_blacklist_num_ptr,
               0x00,
               sizeof(MMIPB_BCD_NUMBER_T)
               );

    SCI_MEMCPY(
               s_new_blacklist_num_ptr,
               orginal_addr_ptr,
               sizeof(MMIPB_BCD_NUMBER_T)
               );

    MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_EDIT_WIN_TAB,(ADD_DATA)s_new_blacklist_num_ptr);
}

/*****************************************************************************/
//  Description : add blacklist from external,such as PB  
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 调用这个接口的时候,所有的加入黑名单询问、提示都需要调用者自己处理
/*****************************************************************************/
PUBLIC MMISET_BLACKLIST_ERROR_TYPE_E MMIAPISET_AddBlacklistFromExt(
                                                                   MMI_STRING_T * blacklist_name_ptr,
                                                                   MMIPB_BCD_NUMBER_T *pb_num_ptr
                                                                   )
{
    MMISET_BLACKLIST_ERROR_TYPE_E result = MMISET_BLACKLIST_ERROR_MAX;
    uint16 name_len = 0;
    uint16 num_len = 0;
    uint16 index = 0;//g_blacklist_ptr->num
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 tele_len = 0;
    BOOLEAN is_need_free = FALSE;

    if (PNULL == blacklist_name_ptr ||PNULL == pb_num_ptr)
    {
        //SCI_TRACE_LOW:"MMISET: MMIAPISET_AddBlacklistFromExt,pb_name_ptr ||pb_num_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_8514_112_2_18_2_52_7_330,(uint8*)"");
        return MMISET_BLACKLIST_ERROR_PARAM_NULL;
    }
    if(PNULL ==g_blacklist_ptr)
    {
        is_need_free = TRUE;
    }
    if(MMISET_InitBlacklistParam())
    {
        MMISET_GetBlackList();
        if(PNULL !=g_blacklist_ptr)
        {
            if (MMISET_BLACKLIST_MAX_NUM <= g_blacklist_ptr->num)
            {
                if(is_need_free)
                {
                    MMISET_FreeBlacklistParam();
                }
                return MMISET_BLACKLIST_ERROR_BEYOND_MAX_NUM;
            }
            index = g_blacklist_ptr->num;
        }


        tele_len = MMIAPIPB_BCDNumberToString(pb_num_ptr, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);
//         tele_len = MMIAPICOM_GenDispNumber(
//                                         MMIPB_GetNumberTypeFromUint8(pb_num_ptr->npi_ton),
//                                         pb_num_ptr->number_len,
//                                         pb_num_ptr->number,
//                                         tele_num,
//                                         MMISET_PHONE_NUM_MAX_LEN + 2
//                                         );
        /*load from pb,do not need check num if valid*/                                  
        if(0 == pb_num_ptr->number_len)
        {
            result = MMISET_BLACKLIST_ERROR_NUM_EMPTY;
        }
        else
        {
            if(PNULL !=g_blacklist_ptr)
            {
                uint16 i= 0;
                for(i=0; i<g_blacklist_ptr->num;i++)
                {
                    uint32 count =0;
                    count = (uint32)MAX(g_blacklist_ptr->blacklist[i].num_len,tele_len);
                    if(!MMIAPICOM_WstrStrncmp(g_blacklist_ptr->blacklist[i].num, tele_num, count))
                    {
                        if(is_need_free)
                        {
                            MMISET_FreeBlacklistParam(); 
                        }
                        return MMISET_BLACKLIST_ERROR_SAME_NUM;
                    }
                }

                name_len = MIN(blacklist_name_ptr->wstr_len,MMIPB_NAME_MAX_LEN);
                g_blacklist_ptr->blacklist[index].name_len = name_len;
                MMI_WSTRNCPY(
                             g_blacklist_ptr->blacklist[index].name,
                             MMIPB_NAME_MAX_LEN,
                             blacklist_name_ptr->wstr_ptr,
                             name_len,
                             name_len);

                num_len = MIN(tele_len,MMISET_PHONE_NUM_MAX_LEN);
                g_blacklist_ptr->blacklist[index].num_len = num_len;
                MMI_STRNTOWSTR(
                        g_blacklist_ptr->blacklist[index].num,
                        MMISET_PHONE_NUM_MAX_LEN,
                        tele_num,
                        num_len,
                        num_len);

                g_blacklist_ptr->num++;
                MMISET_SetBlackList(g_blacklist_ptr);

                UpdateBlackList(MMISET_SET_BLACKLIST_LIST_CTRL_ID);
                result = MMISET_BLACKLIST_ERROR_NONE;
            }           
        }

        if(is_need_free)
        {
            MMISET_FreeBlacklistParam();
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : add blacklist,such as from CL,sms,mms
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 内部封装了MMIAPISET_AddBlacklistFromExt等接口；加入
//        黑名单时的询问、提示等操作已处理，调用者无需另外处理
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_AddBlacklist(
                                MMI_STRING_T mmi_pb_name,
                                MMIPB_BCD_NUMBER_T pb_bcd      
                                )
{
    MMI_WIN_ID_T query_win_id = MMISET_ADD_BLACKLISTQUERY_WIN_ID;
    MMIAPISET_SetAllBlacklistSetting(TRUE);
    //add to blacklist
    if(!MMIAPISET_GetIsBlacklistAudioCall()
#ifdef VT_SUPPORT
        &&!MMIAPISET_GetIsBlacklistVedioCall()
#endif            
        &&!MMIAPISET_GetIsBlacklistMessage()
        )
    {
        MMISET_BLACKLIST_NAME_NUM_ITEM_T *black_name_num_ptr = PNULL;

        black_name_num_ptr = SCI_ALLOC_APPZ(sizeof(MMISET_BLACKLIST_NAME_NUM_ITEM_T));
        if (PNULL == black_name_num_ptr)
        {
            return FALSE;
        }

        black_name_num_ptr->pb_bcd.npi_ton = pb_bcd.npi_ton;
        black_name_num_ptr->pb_bcd.number_len = pb_bcd.number_len;
        SCI_MEMCPY( 
            black_name_num_ptr->pb_bcd.number,  
            pb_bcd.number, 
            black_name_num_ptr->pb_bcd.number_len 
            );
        black_name_num_ptr->mmi_pb_name.wstr_len = mmi_pb_name.wstr_len;
        black_name_num_ptr->mmi_pb_name.wstr_ptr = SCI_ALLOC_APPZ((MMIPB_NAME_MAX_LEN + 1) * sizeof(wchar));
        if (PNULL == black_name_num_ptr->mmi_pb_name.wstr_ptr)
        {
            SCI_FREE(black_name_num_ptr);

            return FALSE;
        }
        SCI_MEMCPY( 
            black_name_num_ptr->mmi_pb_name.wstr_ptr,  
            mmi_pb_name.wstr_ptr, 
            MMIPB_NAME_MAX_LEN
            );

        MMK_CloseWin(query_win_id);

        MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_ADD_TO_BLACKLIST_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,HandleAddToBlackListQueryWinMsg, (uint32)black_name_num_ptr);
        return FALSE;
    }
    else
    {
        //[after add number to block list,call log will delete logs]
        //return AddNumToBlacklist(mmi_pb_name, pb_bcd);
        if(AddNumToBlacklist(mmi_pb_name,pb_bcd))
        {
            //MMIAPICL_DeleteLogsAfterAddBlockListB(pb_bcd);
            return TRUE;
        }
        return FALSE;
        //[end]
    }
}

/*****************************************************************************/
//  Description : alert of the result when add a number into blacklist
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddNumToBlacklist(
                            MMI_STRING_T mmi_pb_name,
                            MMIPB_BCD_NUMBER_T pb_bcd      
                            )
{
    MMI_RESULT_E    ret = FALSE;
    MMISET_BLACKLIST_ERROR_TYPE_E add_blacklist_result = MMISET_BLACKLIST_ERROR_MAX;
       
    add_blacklist_result = MMIAPISET_AddBlacklistFromExt(&mmi_pb_name,&pb_bcd);      
    
    if(MMISET_BLACKLIST_ERROR_NONE == add_blacklist_result)
    {
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        ret = TRUE;
    }
    else if(MMISET_BLACKLIST_ERROR_BEYOND_MAX_NUM == add_blacklist_result)
    {
        //[block list in call setting]
        //MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);
        DisplaybannerInfo(PNULL,STR_CL_BLOCK_LIST_FULL);
        //[end]
        ret = FALSE;
    }
    else if(MMISET_BLACKLIST_ERROR_SAME_NUM == add_blacklist_result)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
        ret = FALSE;
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        ret = FALSE;
    }

    return ret;

}

/*****************************************************************************/
//  Description : to handle add to blacklist query window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddToBlackListQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *win_info_ptr = PNULL;
    MMISET_BLACKLIST_NAME_NUM_ITEM_T *black_name_num_ptr = PNULL;
    win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL != win_info_ptr)
    {
        black_name_num_ptr  = (MMISET_BLACKLIST_NAME_NUM_ITEM_T *)win_info_ptr->user_data;
    }

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK: 
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:        
        MMIAPISET_SetAllBlacklistSetting(TRUE);
        if (PNULL != black_name_num_ptr)
        {
            if (AddNumToBlacklist(black_name_num_ptr->mmi_pb_name, black_name_num_ptr->pb_bcd))
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            }
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != black_name_num_ptr)
        {
            if (black_name_num_ptr->mmi_pb_name.wstr_ptr)
            {
                SCI_FREE(black_name_num_ptr->mmi_pb_name.wstr_ptr);
            }

            SCI_FREE(black_name_num_ptr);
        }
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description : to handle  add blacklist open setting query window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistAddQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = PNULL;
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        if(!MMIAPISET_GetIsBlacklistAudioCall()
#ifdef VT_SUPPORT
        &&!MMIAPISET_GetIsBlacklistVedioCall()
#endif            
        &&!MMIAPISET_GetIsBlacklistMessage()
        )
        {
            MMIAPISET_SetAllBlacklistSetting(TRUE);
        }
        win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        if ((win_info_ptr != PNULL) && ((uint32 )(win_info_ptr->user_data) != NULL))
        {
            if( ID_SET_BLACKLIST_NEW == win_info_ptr->user_data)
            {
                MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_NEW_WIN_TAB, PNULL);
            }
            else if(ID_SET_BLACKLIST_CONTACT== win_info_ptr->user_data)
            {
               /* MMIAPIPB_OpenNumberList(
                        MMIPB_LIST_FOR_SMS,
                        MMIPB_LIST_CHECK,
                        MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num,
                        PNULL
                        );*/
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER,
                    MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    SetBlackListSelectContactCallback
                    );	
            }
            else
            {
                /*do nothing*/
            }
        }
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        if ((win_info_ptr != PNULL) && ((uint32 )(win_info_ptr->user_data) != NULL))
        {
            if( ID_SET_BLACKLIST_NEW == win_info_ptr->user_data)
            {
                MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_NEW_WIN_TAB, PNULL);
            }
            else if(ID_SET_BLACKLIST_CONTACT== win_info_ptr->user_data)
            {
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER,
                    MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    SetBlackListSelectContactCallback
                    );
            }
            else
            {
                /*do nothing*/
            }
        }
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : delete blacklist,such as from CL,sms,mms
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_DeleteBlacklist(MMIPB_BCD_NUMBER_T pb_bcd)
{

    int32 i,j=0;
    uint8 tele_len =0,max_len =0;
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    BOOLEAN is_need_free =FALSE;
    
    if(PNULL ==g_blacklist_ptr)
    {
        is_need_free = TRUE;
    }
    if(MMISET_InitBlacklistParam())
    {
        MMISET_GetBlackList();

        if(PNULL !=g_blacklist_ptr)
        {
            if (MMISET_BLACKLIST_MAX_NUM <= g_blacklist_ptr->num)
            {
                if(is_need_free)
                {
                    MMISET_FreeBlacklistParam();  
                } 
                return FALSE;
            }  
        }

        max_len = MIN(MMISET_PHONE_NUM_MAX_LEN, pb_bcd.number_len);
        tele_len = MMIAPIPB_BCDNumberToString(&pb_bcd, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);

        if(PNULL !=g_blacklist_ptr)
        {
            for(j=0; j<g_blacklist_ptr->num;j++)
            {
                uint32 cmp_count =0;
                cmp_count = (uint32)MAX(g_blacklist_ptr->blacklist[j].num_len,tele_len);
                if(!MMIAPICOM_WstrStrncmp((const wchar *)g_blacklist_ptr->blacklist[j].num, tele_num, cmp_count))
                {
                    break;
                }
            }
            
            for (i=j+1; i<g_blacklist_ptr->num; i++)
            {
                //g_blacklist_ptr->blacklist[i-1] = g_blacklist_ptr->blacklist[i];
                SCI_MEMCPY(
                           &g_blacklist_ptr->blacklist[i-1],
                           &g_blacklist_ptr->blacklist[i],
                           sizeof(MMISET_BLACKLIST_ITEM_T));
            }
            
            g_blacklist_ptr->num--;
            
            MMISET_SetBlackList(g_blacklist_ptr);

        }
        
        if(is_need_free)
        {
            MMISET_FreeBlacklistParam();  
        }  
    }

    return TRUE;
}

#endif//#if defined(MMI_BLACKLIST_SUPPORT)
//[end]

PUBLIC MMI_HANDLE_T MMISmartDS_OpenNetworkWaittingWin(void)
{
    return MMK_CreateWin((uint32*)SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_WIN_TAB, PNULL);
}

LOCAL MMI_RESULT_E HandleNetworkWaittingIndicatorWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           animCtrl = MMI_SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_ANIM_CTRL_ID;
    MMI_CTRL_ID_T           labelCtrl = MMI_SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_LABEL_CTRL_ID;
    MMISET_SIM_NAME_T       simName = {0};
    MN_DUAL_SYS_E           simID = (MN_DUAL_SYS_E)MMK_GetWinAddDataPtr(win_id);

    MMI_STRING_T            nameInfo = {0};

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_BG_T                backgrand = {0};
            GUI_COLOR_T             font_color_ptr = MMI_DEFAULT_TEXT_COLOR;
            MMI_CTRL_ID_T           formCtrlId = MMI_SMARTDUALSIM_NETWORK_WAITTING_INDICATOR_FORM_CTRL_ID;
            GUIANIM_CTRL_INFO_T     control_info = {0};
            GUIANIM_DATA_INFO_T     data_info = {0};
            GUIANIM_DISPLAY_INFO_T  display_info = {0};
        	MMI_STRING_T text = {0};
            GUI_FONT_T textFont = SONG_FONT_16;

            //style for form
            backgrand.bg_type = GUI_BG_COLOR;
            backgrand.color = MMI_BACKGROUND_COLOR;
            GUIFORM_SetStyle(formCtrlId,GUIFORM_STYLE_NORMAL);
            GUIFORM_SetBg(formCtrlId, &backgrand);
            GUIFORM_PermitChildFont(formCtrlId,FALSE);
            GUIFORM_PermitChildBorder(formCtrlId,FALSE);
            GUIFORM_PermitChildBg(formCtrlId,FALSE);

            //animate setting
            control_info.is_ctrl_id = TRUE;    
            control_info.ctrl_ptr = PNULL;
            control_info.ctrl_id = animCtrl;
            data_info.img_id = IMAGE_WAITING_INDICATOR_RED_DARK;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);

            //label setting
            GUILABEL_SetMargin(labelCtrl, 6);
            GUILABEL_SetFont(labelCtrl, SONG_FONT_20, MMI_WHITE_COLOR);
            GUILABEL_SetTextById(labelCtrl, STR_WAIT_SEARCHING_NETWORK_EXT01, FALSE);
            //GUITEXT_SetFont(labelCtrl, &textFont, PNULL);
            //MMI_GetLabelTextByLang(STR_WAIT_SEARCHING_NETWORK_EXT01, &text);
    	    //GUITEXT_SetString(labelCtrl, text.wstr_ptr, text.wstr_len, FALSE);
            GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, TRUE);
        }
            break;
        case MSG_GET_FOCUS:
            break;
//         case MSG_FULL_PAINT:
//             if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_HDR_NETWORK_SELECTION_EXT01))
//             {
//                 GUIWIN_UpdateStb();
//             }
//             break;
//        case MSG_APP_OK:
        case MSG_APP_WEB:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        case MSG_APP_RED:
        case MSG_APP_FLIP:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            //no matter which card is searching,just both call abort
        {
            uint32 i = 0;
            for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                MNPHONE_AbortPLMNSearchEx((MN_DUAL_SYS_E)i);
            }

            MMIAPISMS_CheckTimerMSG();
        }
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}

 PUBLIC BOOLEAN MMISmartDS_IsPlmnListWinOpen(void)
{
    if(MMK_IsOpenWin(MMISET_SET_PLMN_LIST_WIN_ID))
        return TRUE;
    else
        return FALSE;
}
       
PUBLIC BOOLEAN MMISmartDS_IsNetworkWaittingWinOpen(void)
{
    if(MMK_IsOpenWin(MMISET_SET_SEARCH_NETWORK_WIN_ID))
        return TRUE;
    else
        return FALSE;
}

PUBLIC void MMISmartDS_CloseNetworkWaittingWin(void)
{
    MMK_CloseWin(MMISET_SET_SEARCH_NETWORK_WIN_ID);
}

/*****************************************************************************/
//  Description : open the FDN handle wndow
//  Global resource dependence : 
//  Author:zhenxing
//  Note: 
/*****************************************************************************/

PUBLIC void MMISET_OpenFdnHandleWindow(uint16 sim_sys)
{
     s_cur_dual_sys =  sim_sys;//set the current sim
     SCI_TRACE_LOW("MMISET_OpenFdnHandleWindow s_cur_dual_sys=%d", s_cur_dual_sys);
     if(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(s_cur_dual_sys))
       {
#ifndef WIN32                   
          if(MN_SIM_SERVICE_NOT_SUPPORT != MNSIM_GetSimEFService(s_cur_dual_sys,MN_SIM_FDN))
#endif
           MMK_CreateWin((uint32 *)MMISET_SET_FIXED_DIALING_SUB_WIN_TAB,(ADD_DATA)s_cur_dual_sys);
       }
}



/*****************************************************************************/
//  Description : get voice mail address
//  Global resource dependence : 
//  Author:bown.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void GetVoiceMailAddress(MN_DUAL_SYS_E dual_sys, MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type, MN_VOICE_MAIL_TYPE_E voice_mail_type)
{
    MMI_STRING_T wait_text = {0};
    ERR_MNSMS_CODE_E result = ERR_MNSMS_NONE;

    SCI_TRACE_LOW("ljg222*GetVoiceMailAddress,dual_sys = %d, line_type= %d, voice_mail_type= %d", dual_sys, line_type, voice_mail_type);     

    result = MNSMS_GetVoiMessAddrEx(dual_sys, line_type, voice_mail_type);/*lint !e64*/

    SCI_TRACE_LOW("ljg222*GetVoiceMailAddress,result = %d,OperType=%d", result,MMISET_GetVmOperType()); 
    //进入等待界面
    if (ERR_MNSMS_NONE == result)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MAIN_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleVMForDivertWaitingWindow);
    }
    else
    {
        if(MN_DUAL_SYS_1 == dual_sys)
        {
            s_is_invalid_cnf_msg = TRUE;
        }
        else
        {
            s_is_invalid_cnf2_msg = TRUE;
        }
        switch (MMISET_GetVmOperType())
        {
            case MMISET_VMN_OPER_TYPE_CALL_SETTING:
                MMISET_GetVoicemailFromNdtAndMultivar(dual_sys);	
                break;
            case MMISET_VMN_OPER_TYPE_IDLE_DIAL:
                MMIAPIIDLE_HandleVoiceMailFromNdt(dual_sys);
                break;
            default:
                break;
        }    
    }
	
	return;
}


/*****************************************************************************/
//  Description : set voice mail number to sim;
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC void SetVoiceMailAddress(MN_DUAL_SYS_E dual_sys, MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type, MMI_STRING_T* num_string)
{
     MMI_STRING_T wait_text = {0};
     ERR_MNSMS_CODE_E result = ERR_MNSMS_NONE;
     uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
     MN_SMS_VMW_MBNUM_T voimess_addr_info = {0};
     MMI_PARTY_NUMBER_T party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
     
    SCI_TRACE_LOW("ljg222*SetVoiceMailAddress,dual_sys = %d, line_type= %d, ", dual_sys, line_type);

     MMI_WSTRNTOSTR((uint8 *)voice_mail,
                                   MMISMS_PBNUM_MAX_LEN + 2,
                                   num_string->wstr_ptr,
                                   num_string->wstr_len,
                                   MIN(MMISMS_PBNUM_MAX_LEN, num_string->wstr_len));

     SCI_TRACE_LOW("ljg222*SetVoiceMailAddress,voice_mail = %s",voice_mail);
     MMIAPICOM_GenPartyNumber(voice_mail, (int16)MIN(MMISMS_PBNUM_MAX_LEN, num_string->wstr_len), &party_num );


     //set the parameter 
    voimess_addr_info.mailbox_number_len =MIN(PHONEBOOK_MAX_NORMAL_DN_NUMBER_LEN, party_num.num_len);
    voimess_addr_info.is_ton_npi_exist = TRUE;
    voimess_addr_info.is_ccp_exist = FALSE;
    voimess_addr_info.ccp_id = 0;
    voimess_addr_info.is_ext_exist = FALSE;
    voimess_addr_info.ton = party_num.number_type;
    voimess_addr_info.npi = MN_NUM_PLAN_ISDN_TELE;
    voimess_addr_info.ext_id = 0;

    SCI_MEMSET( voimess_addr_info.alpha_id, 0xFF, sizeof(voimess_addr_info.alpha_id));
    SCI_TRACE_LOW("ljg222*SetVoiceMailAddress, save to temp");
    SCI_MEMSET( g_temp_sim_alpha_info.alpha_id, 0xFF, sizeof(g_temp_sim_alpha_info.alpha_id));
    if(MN_DUAL_SYS_1 == dual_sys)
    {
        voimess_addr_info.alpha_id_len = g_sim_alpha_info.alpha_id_len;
        SCI_MEMCPY( voimess_addr_info.alpha_id, g_sim_alpha_info.alpha_id, g_sim_alpha_info.alpha_id_len);
        g_temp_sim_alpha_info.alpha_id_len = voimess_addr_info.alpha_id_len;
        SCI_MEMCPY( g_temp_sim_alpha_info.alpha_id, voimess_addr_info.alpha_id, voimess_addr_info.alpha_id_len);
    }else{
        voimess_addr_info.alpha_id_len = g_sim2_alpha_info.alpha_id_len;
        SCI_MEMCPY( voimess_addr_info.alpha_id, g_sim2_alpha_info.alpha_id, g_sim2_alpha_info.alpha_id_len);
        g_temp_sim_alpha_info.alpha_id_len = voimess_addr_info.alpha_id_len;
        SCI_MEMCPY( g_temp_sim_alpha_info.alpha_id, voimess_addr_info.alpha_id, voimess_addr_info.alpha_id_len);        
    }
    
    
    SCI_MEMSET( voimess_addr_info.mailbox_number, 0, sizeof(voimess_addr_info.mailbox_number));
    SCI_MEMCPY( voimess_addr_info.mailbox_number, party_num.bcd_num, voimess_addr_info.mailbox_number_len);

    g_temp_sim_vmnum_info.num_len = voimess_addr_info.mailbox_number_len;    
    SCI_MEMSET( g_temp_sim_vmnum_info.bcd_num, 0x00, sizeof(g_temp_sim_vmnum_info.bcd_num)); 
    SCI_MEMCPY( g_temp_sim_vmnum_info.bcd_num, voimess_addr_info.mailbox_number, voimess_addr_info.mailbox_number_len);
    g_temp_sim_vmnum_info.number_type = voimess_addr_info.ton;    
    result=MNSMS_SetVoiMessAddrEx (dual_sys, MN_MULTI_SUBSCRIBER_ID_1, MN_VOICE_MAIL, &voimess_addr_info);
     SCI_TRACE_LOW("ljg222*SetVoiceMailAddress,result = %x",result);
     if (ERR_MNSMS_NONE != result)
     {
          if(MN_DUAL_SYS_1 == dual_sys){
             s_is_invalid_cnf_msg = TRUE;
          }else{
             s_is_invalid_cnf2_msg = TRUE;

          }
     }
}
