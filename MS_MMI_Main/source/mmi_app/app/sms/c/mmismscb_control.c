/***************************************************************************
** File Name:      mmismscb_api.c                                          *
** Author:                                                                 *
** Date:           04/2006                                                   *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to define the api function.           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2006          Tracy Zhang       Modularize
****************************************************************************/
#include "mmi_app_sms_trc.h"
#include "mmiidle_func.h"
#if defined(MMI_SMSCB_SUPPORT)

/**------------------------------------------------------------------------*
**                         Include Files                                   *
**-------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_export.h"
#include "mmismscb_control.h"
#include "mmi_common.h"
#include "mmi_default.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiphone_export.h"
#include "guitext.h"
#include "guimsgbox.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_textfun.h"
#include "mmikl_export.h"
#include "mmk_timer.h"
#include "mmipub.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_id.h"
#include "mmisms_nv.h"
//#include "mmisms_menutable.h"
//#include "mmismsapp_wintab.h"
#include "mmi_appmsg.h"
#include "gui_ucs2b_converter.h"
#include "guiedit.h"
#ifdef MMI_PDA_SUPPORT
#include "guictrl_api.h"
#include "guibutton.h"
#endif
//#include "guires.h"

#include "guitab.h"
#include "guirichtext.h"
//#ifdef SCREENSAVER_SUPPORT
//#include "mmiss_export.h"
//#endif

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
#include "mmiidle_statusbar.h"
#endif

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmi_pubmenuwin.h"
#include "mmi_signal_ext.h"
#include "mmismscb_wintab.h"
#include "mmisms_internal.h"

#include "variant_config_nv.h"
#include "mmicc_export.h"
#ifdef MMI_ETWS_SUPPORT
#include "mmi_ring.h"
#include "mmicc_export.h"
#include "window_parse.h"
#include "mmicc_id.h"
#include "mmismscb_export.h"
#ifdef RECORD_SUPPORT
#include "mmirecord_id.h"
#endif
#include "mmidc_camera_id.h"
#endif


/******************************************************************************************************************/
//
//
//                        小区广播模块内部宏定义
//
//
/******************************************************************************************************************/
#ifdef PDA_UI_DROPDOWN_WIN
#define MMI_DISPLAY_TIME_LEN    9
#endif

#define MMI_VIBRATE_INTERVAL 500
#define MMI_PER_INSTALL_PARAM 50

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
#define CB_CELL_INFO_INDEX_ID        50
#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef MMI_ETWS_SUPPORT
MMISMS_CB_MESSAGE_T s_cb_msg = {0};            //小区广播消息数组
#else
LOCAL MMISMS_CB_MESSAGE_T s_cb_msg = {0};            //小区广播消息数组
#endif
LOCAL MN_DUAL_SYS_E s_dual_card_sys = MN_DUAL_SYS_1;
LOCAL BOOLEAN s_is_alert_type_msg = FALSE; //check if the message is alert type. added by Tong Yongli

LOCAL MN_CB_ALERT_MSG_T s_cb_all_alert_info[MN_DUAL_SYS_MAX] = {0};    //记录ALERT所有信息

#ifdef MMI_ETWS_SUPPORT
LOCAL MMISMS_CB_CHANNEL_T s_etws_channel_list[MN_DUAL_SYS_MAX] = {0};
LOCAL uint16 s_cur_display_cb_msg_index = 0;
LOCAL uint8 s_repeated_period_time_id = 0;
LOCAL uint8 s_vibrator_period_time_id = 0;
LOCAL uint16 s_etws_alert_num  = 0;
LOCAL uint16 s_repeated_channel = 0;
LOCAL MMI_STRING_T etws_prompt_str = {0};
LOCAL MMI_STRING_T etws_title_str = {0};
#endif
const MN_CB_MSG_ID_T g_cb_alert_id[] =
{
    4370,
};
extern BOOLEAN cell_broadcast;

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
LOCAL uint8 * s_cell_info_idle_str = PNULL;
#endif
#ifdef MMI_ETWS_SUPPORT
LOCAL BOOLEAN s_is_etws_alert_msg = FALSE;	// <<3GPP 23.041 9.4.1.2.1>> Emergency User Alert
LOCAL BOOLEAN s_is_etws_need_popup = FALSE;	// <<3GPP 23.041 9.4.1.2.1>> Popup
LOCAL BOOLEAN s_is_need_enable = FALSE;
LOCAL BOOLEAN s_is_personal = FALSE;
LOCAL BOOLEAN s_is_need_torch = FALSE;
LOCAL BOOLEAN s_is_need_redisplay = FALSE; // display etws prompt with dismiss button after call
LOCAL BOOLEAN s_is_emer_call = FALSE; // make emergency call
LOCAL MN_DUAL_SYS_E s_etws_card_sys = MN_DUAL_SYS_1;
LOCAL BOOLEAN s_is_etws_display = FALSE;
LOCAL MN_DUAL_SYS_E s_etws_delay_show_dual_sim_context;
LOCAL uint16 s_etws_vibrate_type_context = 0;
LOCAL uint16 s_etws_alert_period_context = 0;
LOCAL uint16 s_etws_is_contiued_context = 0;
LOCAL BOOLEAN s_etws_redisplay_is_need_vibrate = FALSE;

CMAS_CHANNEL_INFO_T s_cmas_chile_channel_info[] = 
{
    { 4370, 0, 0, 0, {0}, text_cb_cmas_chile_4370_title, text_cb_cmas_chile_4370_popup_title, 1,  0,  1, 1,  1,  1, 0, 240 },
    { 4380, 0, 0, 0, {0}, text_cb_cmas_chile_4380_title, text_cb_cmas_chile_4380_popup_title, 0,  1,  1, 5,  0,  0, 0, 10 },

    { 0 }
};

//Colombia
CMAS_CHANNEL_INFO_T s_cmas_co_channel_info[] = 
{
    { 4370,  0, 0, 0, {0}, text_cb_cmas_co_4370_title, 0, 1,  0,  1, 1,  1,  1, 0, 10 },
    {     50,  0, 0, 0, {0},    text_cb_cmas_co_50_title, 0, 1,  0,  1, 1,  1,  0, 0, 10 },
    { 0 }
};

//Ecuador
CMAS_CHANNEL_INFO_T s_cmas_ec_channel_info[] = 
{
    { 4370,  0, 0, 0, {0},  text_cb_cmas_ec_4370_title, 0, 1,  0,  1, 1,  1,  1, 0, 10 },
    {     50,  0, 0, 0, {0},     text_cb_cmas_ec_50_title, 0, 1,  0,  1, 1,  1,  0, 0, 10 },
    { 0 }
};

CMAS_CHANNEL_INFO_T s_cmas_eu_alert_nl_channel_info[] = 
{
    { 4371,  0, 0, 0, {0}, text_cb_cmas_eu_alert_nl_4371_title, 0, 1,  0,  1, 1,  1,  1, 0, 10 },
    { 0 }
};

//Greece
CMAS_CHANNEL_INFO_T s_cmas_gr_channel_info[] = 
{
    { 4370,  0, 0, 0, {0},  text_cb_cmas_gr_title, 0, 1,  1,  1, 1,  1,  1, 0, 10 },
    { 4371,  0, 0, 0, {0},  text_cb_cmas_gr_title, 0, 1,  1,  1, 1,  1,  1, 0, 10 },
    { 4375,  0, 0, 0, {0},  text_cb_cmas_gr_title, 0, 1,  1,  1, 1,  1,  1, 0, 10 },
    { 4379,  0, 0, 0, {0},  text_cb_cmas_gr_title, 0, 1,  1,  1, 1,  1,  1, 0, 10 },
    {   919,  0, 0, 0, {0},  text_cb_cmas_gr_title, 0, 0,  0,  1, 1,  1,  1, 0, 10 },
    {   920,  0, 0, 0, {0},  text_cb_cmas_gr_title, 0, 0,  0,  1, 1,  1,  1, 0, 10 },
    
    { 0 }
};

//Israel
CMAS_CHANNEL_INFO_T s_cmas_il_channel_info[] = 
{
    { 4370,  0, 0, 0, {0}, text_cb_list_emergency_alert, 0, 1,  1,  1, 1,  1,  1, 0, 10 }, 
    { 4371,  0, 0, 0, {0},          text_cb_chl_warn_note, 0, 1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4372,  0, 0, 0, {0},          text_cb_chl_warn_note, 0, 1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4373,  0, 0, 0, {0},                    text_cb_chl_info, 0, 1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4374,  0, 0, 0, {0},                    text_cb_chl_info, 0, 1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4375,  0, 0, 0, {0},                    text_cb_chl_info, 0, 1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4376,  0, 0, 0, {0},                    text_cb_chl_info, 0, 1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4377,  0, 0, 0, {0},                    text_cb_chl_info, 0, 1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4378,  0, 0, 0, {0},                    text_cb_chl_info, 0, 1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4379,  0, 0, 0, {0}, text_cb_chl_public_assistance, 0, 0,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4380,  0, 0, 0, {0},                    text_cb_chl_test, 0, 0,   1,  1, 1,  1,  0, 0, 10 }, 
    { 4381,  0, 0, 0, {0},             text_cb_chl_exercise, 0,  0,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4384,  0, 0, 0, {0},          text_cb_chl_warn_note, 0,  1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4385,  0, 0, 0, {0},          text_cb_chl_warn_note, 0,  1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4386,  0, 0, 0, {0},                    text_cb_chl_info, 0,  1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4387,  0, 0, 0, {0},                    text_cb_chl_info, 0,  1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4388,  0, 0, 0, {0},                    text_cb_chl_info, 0,  1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4389,  0, 0, 0, {0},                    text_cb_chl_info, 0,   1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4390,  0, 0, 0, {0},                    text_cb_chl_info, 0,   1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4391,  0, 0, 0, {0},                    text_cb_chl_info, 0,   1,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4392,  0, 0, 0, {0}, text_cb_chl_public_assistance, 0,  0,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4393,  0, 0, 0, {0},                    text_cb_chl_test, 0,   0,  1,  1, 1,  1,  0, 0, 10 }, 
    { 4394,  0, 0, 0, {0},             text_cb_chl_exercise, 0,   0,  1,  1, 1,  1,  0, 0, 10 }, 
    
    { 0 }
};

//Italy
CMAS_CHANNEL_INFO_T s_cmas_it_channel_info[] = 
{
    { 4370,  0, 0,      0, {0},       text_cb_ita_IT_Alert, 0,  1,  0,  0, 1,  1,  1, 0, 10 }, 
    { 4371,  0, 0,      0, {0},       text_cb_ita_IT_Alert, 0,  1,  1,  1, 1,  1,  1, 0, 10 }, 
    { 4375,  0, 0, 4371, {0},      text_cb_ita_IT_Alert,  0,  1,  1,  0, 1,  1,  1, 0, 10 }, 
    { 4379,  0, 0, 4371, {0},       text_cb_ita_IT_Alert, 0,  1,  1,  0, 1,  1,  1, 0, 10 }, 
    { 4381,  0, 0,      0, {0}, text_cb_ita_Test_setting,  0,  0,  1,  1, 1,  1,  1, 0, 10 }, 
    { 4383,  0, 0, 4370, {0},       text_cb_ita_IT_Alert,  0,  1,  0,  0, 1,  1,  1, 0, 10 }, 
    { 4384,  0, 0, 4371, {0},       text_cb_ita_IT_Alert,  0,  1,  1,  0, 1,  1,  1, 0, 10 }, 
    { 4388,  0, 0, 4371, {0},       text_cb_ita_IT_Alert,  0,  1,  1,  0, 1,  1,  1, 0, 10 }, 
    { 4392,  0, 0, 4371, {0},       text_cb_ita_IT_Alert,  0,  1,  1,  0, 1,  1,  1, 0, 10 }, 
    { 4394,  0, 0, 4381, {0}, text_cb_ita_Test_setting,  0,  0,  1,  0, 1,  1,  1, 0, 10 }, 

    { 0 }
};

//Pureto Rico
CMAS_CHANNEL_INFO_T s_cmas_pr_channel_info[] = 
{
    { 4370,  0, 0, 0, {0},  text_cb_cmas_pr_4370_title, 0, 1,  0,  1, 1,  1,  1, 0, 10 },
    { 0 }
};
    
//Mexico
CMAS_CHANNEL_INFO_T s_cmas_mx_channel_info[] = 
{
    { 4370,  0, 0, 0, {0},  text_cb_cmas_mx_4370_title, 0, 1,  0,  0, 1,  1,  1, 0, 10 },
    { 4371,  0, 0, 0, {0},  text_cb_cmas_mx_4370_title, 0, 1,  0,  0, 1,  1,  1, 0, 10 },
    { 4372,  0, 0, 0, {0},  text_cb_cmas_mx_4370_title, 0, 1,  0,  0, 1,  1,  1, 0, 10 },
    { 4373,  0, 0, 0, {0},  text_cb_cmas_mx_4370_title, 0, 1,  0,  0, 1,  1,  1, 0, 10 },
    {     50,  0, 0, 0, {0},     text_cb_cmas_mx_50_title, 0, 1,  0,  1, 1,  1,  0, 0, 10 },
    
    { 0 }
};

// New Zealand
CMAS_CHANNEL_INFO_T s_cmas_nz_channel_info[] = 
{
    { 4370, 0, 0, 0, {0}, text_cb_cmas_nz_4370_title, 0, 1,  0,  1, 1,  1,  1, 1, 10 },
    { 0 }
};

// Peru
CMAS_CHANNEL_INFO_T s_cmas_pe_channel_info[] = 
{
    { 4370, 0, 0, 0, {0}, text_cb_cmas_pe_4370_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },
    { 4380, 0, 0, 0, {0}, text_cb_cmas_pe_4380_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },
    { 4381, 0, 0, 0, {0}, text_cb_cmas_pe_4381_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },
    { 4382, 0, 0, 0, {0}, text_cb_cmas_pe_4382_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },
    { 4383, 0, 0, 0, {0}, text_cb_cmas_pe_4383_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },
    { 4396, 0, 0, 0, {0}, text_cb_cmas_pe_4396_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },
    { 4397, 0, 0, 0, {0}, text_cb_cmas_pe_4397_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },
    { 4398, 0, 0, 0, {0}, text_cb_cmas_pe_4398_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },
    { 4399, 0, 0, 0, {0}, text_cb_cmas_pe_4399_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },
    {   519, 0, 0, 0, {0},  text_cb_cmas_pe_519_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },
    {   919, 0, 0, 0, {0},  text_cb_cmas_pe_919_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },
    {     50, 0, 0, 0, {0},    text_cb_cmas_pe_50_title, 0, 1,  0,  1, 1,  1,  1, 0, 180 },

    { 0 }
};

//Taiwan
CMAS_CHANNEL_INFO_T s_cmas_pws_channel_info[] = 
{
    {  911, 0, 0, 0, TXT_CMAS_PWS_911_TITLE,         0, 0, 1,  1,  1, 4,  0,  0, 0, 10 },
    {  919, 0, 0, 0, {0},     text_cb_cmas_pws_919_title, 0, 1,  1,  1, 4,  0,  0, 0, 10 },

    { 4370, 0, 0, 0, TXT_CMAS_PWS_4370_TITLE,      0,   0, 1,  0,  1, 1,  1,  1, 0, 10 },
    { 4371, 0, 0, 0, TXT_CMAS_PWS_4371_TITLE,      0,  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4372, 0, 0, 0, TXT_CMAS_PWS_4372_TITLE,      0,  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4373, 0, 0, 0, TXT_CMAS_PWS_4373_TITLE,      0,  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4374, 0, 0, 0, TXT_CMAS_PWS_4374_TITLE,      0,  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4375, 0, 0, 0, TXT_CMAS_PWS_4375_TITLE,      0,  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4376, 0, 0, 0, TXT_CMAS_PWS_4376_TITLE,      0,  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4377, 0, 0, 0, TXT_CMAS_PWS_4377_TITLE,      0,  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4378, 0, 0, 0, TXT_CMAS_PWS_4378_TITLE,      0,  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4379, 0, 0, 0, TXT_CMAS_PWS_4379_TITLE,      0,  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4380, 0, 0, 0, TXT_CMAS_PWS_4380_TITLE,      0,  0, 0,  1,  1, 5,  1,  1, 0, 10 },

    { 4383, 0, 0, 0, {0},  text_cb_cmas_pws_4383_title, 0, 1,  0,  1, 1,  1,  1, 0, 10 },
    { 4384, 0, 0, 0, {0},  text_cb_cmas_pws_4384_title, 0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4385, 0, 0, 0, {0},  text_cb_cmas_pws_4385_title, 0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4386, 0, 0, 0, {0},  text_cb_cmas_pws_4386_title, 0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4387, 0, 0, 0, {0},  text_cb_cmas_pws_4387_title, 0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4388, 0, 0, 0, {0},  text_cb_cmas_pws_4388_title, 0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4389, 0, 0, 0, {0},  text_cb_cmas_pws_4389_title, 0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4390, 0, 0, 0, {0},  text_cb_cmas_pws_4390_title, 0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4391, 0, 0, 0, {0},  text_cb_cmas_pws_4391_title, 0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4392, 0, 0, 0, {0},  text_cb_cmas_pws_4392_title, 0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4393, 0, 0, 0, {0},  text_cb_cmas_pws_4393_title, 0, 0,  1,  1, 5,  1,  1, 0, 10 },

    { 0 }
};

//Romania
CMAS_CHANNEL_INFO_T s_cmas_ro_channel_info[] = 
{
    { 4370, 0, 0, 0, {0}, text_cb_chal_title_presidential_alert, 0, 1,  0,  1, 1,  1,  1, 0, 10 },        
    { 4371, 0, 0, 0, {0},                     text_cb_chl_extr_alert, 0, 1,  1,  1, 1,  1,  1, 0, 10 },        
    { 4375, 0, 0, 0, {0},        text_cb_chal_title_severe_alert, 0, 1,  1,  1, 1,  1,  1, 0, 10 },        
    { 4379, 0, 0, 0, {0},             text_cb_chal_title_org_alert, 0, 1,  1,  1, 1,  1,  1, 0, 10 },        
    { 4381, 0, 0, 0, {0},      text_cb_chal_title_exercise_alert, 0, 0,  1,  1, 1,  1,  1, 0, 10 },        
    { 0 }
};

// UAE
CMAS_CHANNEL_INFO_T s_cmas_uae_channel_info[] = 
{
    { 4370,     1, 4383, 0, TXT_CMAS_UAE_4370_TITLE,      0, 0, 1,  0,  0, 1,  1,  1, 0, 10 },         
    { 4371,     1, 4384, 0, TXT_CMAS_UAE_4371_TITLE,      0, 0, 1,  0,  0, 2,  1,  1, 0, 10 },         
    { 4372,     1, 4385, 0, TXT_CMAS_UAE_4372_TITLE,      0, 0, 1,  0,  1, 2,  1,  1, 0, 10 },    
    { 4373,     1, 4386, 0, TXT_CMAS_UAE_4373_TITLE,      0, 0, 1,  0,  0, 2,  1,  1, 0, 10 },         
    { 4374,     1, 4387, 0, TXT_CMAS_UAE_4374_TITLE,      0, 0, 1,  0,  1, 2,  1,  1, 0, 10 },         
    { 4375,     1, 4388, 0, TXT_CMAS_UAE_4375_TITLE,      0, 0, 1,  0,  1, 2,  1,  1, 0, 10 },         
    { 4376,     1, 4389, 0, TXT_CMAS_UAE_4376_TITLE,      0, 0, 1,  0,  1, 2,  1,  1, 0, 10 },         
    { 4377,     1, 4390, 0, TXT_CMAS_UAE_4377_TITLE,      0, 0, 1,  0,  1, 2,  1,  1, 0, 10 },         
    { 4378,     1, 4391, 0, TXT_CMAS_UAE_4378_TITLE,      0, 0, 1,  0,  1, 2,  1,  1, 0, 10 },         
    { 4379,     1, 4392, 0, TXT_CMAS_UAE_4379_TITLE,      0, 0, 1,  1,  1, 4,  1,  1, 0, 10 },         
    { 4380,     1, 4393, 0, TXT_CMAS_UAE_4380_TITLE,      0, 0, 0,  1,  1, 5,  1,  1, 0, 10 },         
    { 4381,     1, 4394, 0, TXT_CMAS_UAE_4381_TITLE,      0, 0, 1,  0,  1, 6,  1,  1, 0, 10 },         
    { 4396,     1, 4397, 0, TXT_CMAS_UAE_4396_TITLE,      0, 0, 1,  1,  1, 7,  1,  1, 0, 10 },         
    { 4398,     1, 4399, 0, TXT_CMAS_UAE_4398_TITLE,      0, 0, 1,  0,  0, 8,  1,  1, 0, 10 },         
                                                                  
    { 4383, 4370,     1, 0,  {0}, text_cb_cmas_uae_4383_title, 0, 1,  0,  0, 1,  1,  1, 0, 10 },         
    { 4384, 4371,     1, 0,  {0}, text_cb_cmas_uae_4384_title, 0, 1,  0,  0, 2,  1,  1, 0, 10 },         
    { 4385, 4372,     1, 0,  {0}, text_cb_cmas_uae_4385_title, 0, 1,  0,  1, 2,  1,  1, 0, 10 },         
    { 4386, 4373,     1, 0,  {0}, text_cb_cmas_uae_4386_title, 0, 1,  0,  0, 2,  1,  1, 0, 10 },         
    { 4387, 4374,     1, 0,  {0}, text_cb_cmas_uae_4387_title, 0, 1,  0,  1, 2,  1,  1, 0, 10 },         
    { 4388, 4375,     1, 0,  {0}, text_cb_cmas_uae_4388_title, 0, 1,  0,  1, 2,  1,  1, 0, 10 },         
    { 4389, 4376,     1, 0,  {0}, text_cb_cmas_uae_4389_title, 0, 1,  0,  1, 2,  1,  1, 0, 10 },         
    { 4390, 4377,     1, 0,  {0}, text_cb_cmas_uae_4390_title, 0, 1,  0,  1, 2,  1,  1, 0, 10 },         
    { 4391, 4378,     1, 0,  {0}, text_cb_cmas_uae_4391_title, 0, 1,  0,  1, 2,  1,  1, 0, 10 },         
    { 4392, 4379,     1, 0,  {0}, text_cb_cmas_uae_4392_title, 0, 1,  1,  1, 4,  1,  1, 0, 10 },         
    { 4393, 4380,     1, 0,  {0}, text_cb_cmas_uae_4393_title, 0, 0,  1,  1, 5,  1,  1, 0, 10 },         
    { 4394, 4381,     1, 0,  {0}, text_cb_cmas_uae_4394_title, 0, 1,  0,  1, 6,  1,  1, 0, 10 },   
    { 4397, 4396,     1, 0,  {0}, text_cb_cmas_uae_4397_title, 0, 1,  1,  1, 7,  1,  1, 0, 10 },         
    { 4399, 4398,     1, 0,  {0}, text_cb_cmas_uae_4399_title, 0, 1,  0,  0, 8,  1,  1, 0, 10 },         
    
    { 0 }
};

//Venezuela
CMAS_CHANNEL_INFO_T s_cmas_ve_channel_info[] = 
{
    { 4370, 0, 0, 0, {0}, text_cb_cmas_ve_4370_title, 0, 1,  0,  1, 1,  1,  1, 0, 10 },
    { 0 }
};

CMAS_CONTROL_LIST_T s_cmas_spec_list[] = 
{
    {730,   s_cmas_chile_channel_info},
    {424,   s_cmas_uae_channel_info},
    {430,   s_cmas_uae_channel_info},
    {431,   s_cmas_uae_channel_info},
    {466,   s_cmas_pws_channel_info},
    {202,   s_cmas_gr_channel_info},
    {204,   s_cmas_eu_alert_nl_channel_info},
    {206,   s_cmas_eu_alert_nl_channel_info},
    {208,   s_cmas_eu_alert_nl_channel_info},
    {214,   s_cmas_eu_alert_nl_channel_info},
    {216,   s_cmas_eu_alert_nl_channel_info},
    {218,   s_cmas_eu_alert_nl_channel_info},
    {219,   s_cmas_eu_alert_nl_channel_info},
    {220,   s_cmas_eu_alert_nl_channel_info},
    {222,   s_cmas_it_channel_info},
    {226,   s_cmas_ro_channel_info},
    {228,   s_cmas_eu_alert_nl_channel_info},
    {230,   s_cmas_eu_alert_nl_channel_info},
    {231,   s_cmas_eu_alert_nl_channel_info},
    {232,   s_cmas_eu_alert_nl_channel_info},
    {234,   s_cmas_eu_alert_nl_channel_info},
    {238,   s_cmas_eu_alert_nl_channel_info},
    {240,   s_cmas_eu_alert_nl_channel_info},
    {242,   s_cmas_eu_alert_nl_channel_info},
    {244,   s_cmas_eu_alert_nl_channel_info},
    {246,   s_cmas_eu_alert_nl_channel_info},
    {247,   s_cmas_eu_alert_nl_channel_info},
    {248,   s_cmas_eu_alert_nl_channel_info},
    {250,   s_cmas_eu_alert_nl_channel_info},
    {255,   s_cmas_eu_alert_nl_channel_info},
    {257,   s_cmas_eu_alert_nl_channel_info},
    {260,   s_cmas_eu_alert_nl_channel_info},
    {262,   s_cmas_eu_alert_nl_channel_info},
    {268,   s_cmas_eu_alert_nl_channel_info},
    {270,   s_cmas_eu_alert_nl_channel_info},
    {272,   s_cmas_eu_alert_nl_channel_info},
    {274,   s_cmas_eu_alert_nl_channel_info},
    {278,   s_cmas_eu_alert_nl_channel_info},
    {280,   s_cmas_eu_alert_nl_channel_info},
    {284,   s_cmas_eu_alert_nl_channel_info},
    {293,   s_cmas_eu_alert_nl_channel_info},
    {294,   s_cmas_eu_alert_nl_channel_info},
    {297,   s_cmas_eu_alert_nl_channel_info},
    {330,   s_cmas_pr_channel_info},
    {340,   s_cmas_eu_alert_nl_channel_info},
    {334,   s_cmas_mx_channel_info},
    {530,   s_cmas_nz_channel_info},
    {425,   s_cmas_il_channel_info},
    {716,   s_cmas_pe_channel_info},
    {732,   s_cmas_co_channel_info},
    {734,   s_cmas_ve_channel_info},
    {740,   s_cmas_ec_channel_info},
    
    {0}
};
#endif

extern BOOLEAN cell_broadcast;

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : Get Current Channel table according the MCC
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC CMAS_CHANNEL_INFO_T* GetCMASChannelTable( MN_DUAL_SYS_E dual_sys)
{
    CMAS_CHANNEL_INFO_T           *current_channel_table_ptr = NULL;
    uint32 i = 0;
    MN_PLMN_T plmn_info = {0};
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 sim_ok_array[MMI_DUAL_SYS_MAX] = {0,};
    uint32 sim_ok_num = 0;
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_MEMSET(sim_ok_array, 0x00, sizeof(sim_ok_array));

        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(MMIAPIPHONE_IsSimOk((MN_DUAL_SYS_E)i))
            {
                sim_ok_array[sim_ok_num] = (MN_DUAL_SYS_E)i;
                sim_ok_num ++;
            }
        }

        dual_sys = (MN_DUAL_SYS_E)(sim_ok_array[0]);   //else use the ok sim. or if no ok, user the sim1.(sim_ok have inited with 0. )
    }
#else
    if (dual_sys >= MMI_DUAL_SYS_MAX)
        dual_sys = MN_DUAL_SYS_1;
#endif

    SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));

    MMIAPIPHONE_GetCurPLMN(&plmn_info, dual_sys);

    SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelTable MMIAPIPHONE_GetCurPLMN plmn_info.mcc = %d", plmn_info.mcc);

    if( plmn_info.mcc < 200 )
    {
        MMIAPIPHONE_GetSimHPLMN(&plmn_info, dual_sys);
        SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelTable MMIAPIPHONE_GetSimHPLMN plmn_info.mcc = %d", plmn_info.mcc);
    }

    if(MMIAPISMSCB_IsSetMcc())
    {
        plmn_info.mcc = MMIAPISMSCB_GetSetMcc();
    }

    SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelTable plmn_info.mcc = %d", plmn_info.mcc);

    i = 0;
    do
    {
        if(plmn_info.mcc == s_cmas_spec_list[i].mcc)
        {
            SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelTable table index i = %d", i);
            current_channel_table_ptr = s_cmas_spec_list[i].channel_info;
            return current_channel_table_ptr;
        }
        i ++;
    }
    while(0 != s_cmas_spec_list[i].mcc);

    return NULL;
}

/*****************************************************************************/
//  Description : Get Current Channel info according the channel ID
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC CMAS_CHANNEL_INFO_T* GetCMASChannelInfo( MN_DUAL_SYS_E dual_sys, uint16 msg_id)
{
    int i = 0;
    CMAS_CHANNEL_INFO_T* current_channel_table = GetCMASChannelTable(dual_sys);

    SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelInfo, dual_sys = %d, msg_id = %d", dual_sys, msg_id);

    if( NULL == current_channel_table ) 
    {
        SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelInfo, current_channel_table = NULL");
        return NULL;
    }

    do
    {
        if( msg_id == current_channel_table[i].channel_id)
        {
            return &current_channel_table[i];
        }
        i++;
    }
    while(0 != current_channel_table[i].channel_id);

    return NULL;
}
#endif

/******************************************************************************************************************/
//
//
//                        小区广播control模块处理函数声明
//
//
/******************************************************************************************************************/
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMsgToCBMsgList
//  功能描述:        将收到的新小区广播加入到广播收件箱列表中去
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddNewMsgToCBMsgList(
                                APP_MN_SMSCB_MSG_IND_T *cb_msg_ptr,    //IN: 广播信息信号指针
                                BOOLEAN is_read
                                );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         InitCBSoftConfChannel
//  功能描述:        根据配置化软件判断需要配置的NV Channels. NV_VARIANT_CONFIG_TELE/CELL_BROADCAST_CHANNEL
//  全局变量使用:    cell_broadcast_channel
//  返回值:          
//  作者:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMISMS_CB_CHANNEL_T InitCBSoftConfChannel(void);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMarcoMsgToCBMsgList
//  功能描述:        将收到的新小区广播加入到广播收件箱列表中去
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddNewMarcoMsgToCBMsgList(
                                APP_MN_SMSCB_MACRO_MSG_IND_T *cb_msg_ptr    //IN: 广播信息信号指针
                                );

/**------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                             *
**-------------------------------------------------------------------------*/

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMsgToCBMsgList
//  功能描述:        将收到的新小区广播加入到广播收件箱列表中去
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddNewMsgToCBMsgList(
                                APP_MN_SMSCB_MSG_IND_T *cb_msg_ptr,    //IN: 广播信息信号指针
                                BOOLEAN is_read
                                )
{
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *last_msg_ptr = PNULL;
    uint16 msg_real_len = 0;
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_MESSAGE_ARRAY_T msg_arr_ptr = {0};
    MMISMS_CB_MESSAGE_ARRAY_T* cb_message_array = PNULL;
    MMISMS_CB_MESSAGE_ARRAY_T* temp_message_array = PNULL;
    uint8 index = 0;
    uint8 cb_num = 0;
    uint8 read_count = 0;
#endif

    if (PNULL == cb_msg_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:AddNewMsgToCBMsgList cb_msg_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2140_112_2_18_2_55_33_889,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"SMSCB: AddNewMsgToCBMsgList"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2143_112_2_18_2_55_33_890,(uint8*)"");

    cur_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
    if (PNULL == cur_msg_ptr)
    {
        //SCI_TRACE_LOW:"SMSCB: cur_msg_ptr alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2148_112_2_18_2_55_33_891,(uint8*)"");
        return;
    }
    //SCI_MEMSET(cur_msg_ptr, 0 , sizeof(MMISMS_CB_MESSAGE_ITEM_T));

#ifdef MMI_ETWS_SUPPORT
    s_cur_display_cb_msg_index = 0;
#endif
    // 先给相应的广播数组项赋值并设为未读，然后处理链表
    cur_msg_ptr->msg_id  = cb_msg_ptr->msg_id;
    cur_msg_ptr->is_read = FALSE;
    cur_msg_ptr->dual_sys = cb_msg_ptr->dual_sys;
    //将收到的广播消息的内容存入消息内容结构数组中去
    if (cb_msg_ptr->dcs.alphabet_type == MN_SMS_UCS2_ALPHABET)
    {
        msg_real_len = MMIAPICOM_Wstrlen((const wchar*)cb_msg_ptr->msg_arr) * sizeof(wchar);
    } 
    else
    {
        msg_real_len = strlen((char *)cb_msg_ptr->msg_arr);
    }
    ConvertCBMsgContextToMsgItem(
        cb_msg_ptr->msg_arr,
        MIN(cb_msg_ptr->msg_len, msg_real_len),
        cb_msg_ptr->dcs.alphabet_type,
        FALSE,
        cur_msg_ptr
        );
    //获得接受到广播时的系统菶掌诤褪奔?
    TM_GetSysTime(&cur_msg_ptr->time);
    TM_GetSysDate(&cur_msg_ptr->date);
	
#ifdef MMI_ETWS_SUPPORT
    if( PNULL == s_cb_msg.msg_ptr ) 
    {		
        SCI_TRACE_LOW("MMISMSCB: s_cb_msg.msg_ptr is PNULL");
        s_cb_msg.num = MMISMS_NV_LoadSmsCBMessageListNum();
        read_count = 0;

        if( s_cb_msg.num > 0 )
        {
            temp_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
            SCI_MEMSET(temp_msg_ptr , 0, sizeof(MMISMS_CB_MESSAGE_ITEM_T));	
            s_cb_msg.msg_ptr = temp_msg_ptr;

            cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));	

            if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
            {
                SCI_MEMCPY(temp_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
                read_count ++;
            }
            else
            {
                SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
            }

            for( index = 1; index < s_cb_msg.num; index ++)
            {
                last_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
                SCI_MEMSET(last_msg_ptr , 0, sizeof(MMISMS_CB_MESSAGE_ITEM_T));	
                if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
                {
                    SCI_MEMCPY(last_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
                    temp_msg_ptr->msg_next_ptr = last_msg_ptr;
                    temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
                }
                else
                {
                    SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
                }
            }
        }
    }
#endif	

    //收件箱的广播消息总数加一
    s_cb_msg.num++;
    if (PNULL == s_cb_msg.msg_ptr)
    {
        s_cb_msg.msg_ptr = cur_msg_ptr;
    }
    else
    {//结构调整，以前是最新的放在最后，调整后最新的放在最前面        
        cur_msg_ptr->msg_next_ptr = s_cb_msg.msg_ptr;
        s_cb_msg.msg_ptr = cur_msg_ptr;
        
        if (s_cb_msg.num > MMI_MAX_CB_MSG_NUM)
        {//如果大于最大条数，销毁最后一个节点，把最新的插入到头节点。
            s_cb_msg.num = MMI_MAX_CB_MSG_NUM;
            temp_msg_ptr = s_cb_msg.msg_ptr;
            while (PNULL != temp_msg_ptr->msg_next_ptr)
            {//找到最后一个节点，然后销毁空间，倒数第二个节点next要置空
                last_msg_ptr = temp_msg_ptr;
                temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
            }
            if (PNULL != last_msg_ptr)
            {
                last_msg_ptr->msg_next_ptr = PNULL;
            }            
            SCI_FREE(temp_msg_ptr);
            temp_msg_ptr = PNULL;
        }
    }
	
#ifdef MMI_ETWS_SUPPORT
	temp_msg_ptr = s_cb_msg.msg_ptr;

	index = 0;
	cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
	if ( PNULL == cb_message_array)
	{
            SCI_TRACE_LOW("MMISMSCB: cb_message_array = PNULL!!");
            return ;
	}
	SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));

	MMISMSNV_SaveSmsCBMessageListNum(s_cb_msg.num);

	while( (PNULL != temp_msg_ptr) && ( index < s_cb_msg.num))
	{
            SCI_MEMCPY(cb_message_array, temp_msg_ptr, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            MMIAPISMS_NV_WriteSmsCBItem(TRUE, index, cb_message_array);
//		MMISMSNV_SaveSmsCBMessageList(cb_message_array, index);                  //保存语言列表到 NV 中去
            temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
            index ++;
	}

	if( PNULL != cb_message_array)
	{
            SCI_FREE(cb_message_array);
            cb_message_array = PNULL;
	}
#endif

}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         InitCBSoftConfChannel
//  功能描述:        根据配置化软件判断需要配置的NV Channels. NV_VARIANT_CONFIG_TELE/CELL_BROADCAST_CHANNEL
//  全局变量使用:    cell_broadcast_channel
//  返回值:          
//  作者:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMISMS_CB_CHANNEL_T InitCBSoftConfChannel(void)
{
#if 0
    MMISMS_CB_CHANNEL_T channel_list[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    MMISMS_CB_CHANNEL_T return_channel_list = {0};
    int32 i = 0;
    
    MMINV_READ(TELE_NV_CELL_BROADCAST_CHANNEL, &channel_list, return_val);
    
    if (return_val != MN_RETURN_SUCCESS)
    {
        //设定广播信道列表初始值
        SCI_MEMSET(&channel_list[dual_sys], 0x00, sizeof(MMISMS_CB_CHANNEL_T));
        MMINV_WRITE(MMINV_SMSCB_CHANNEL_LIST, &channel_list);
    }

    SCI_MEMCPY(&return_channel_list,&channel_list[dual_sys],sizeof(MMISMS_CB_CHANNEL_T));
    
    return (return_channel_list);
#endif
    int i = 0;
    MMISMS_CB_CHANNEL_T return_channel_list = {0};
    MN_CB_MSG_ID_T cell_broadcast_channel[5] = {0};
    MN_RETURN_RESULT_E ret_value = MN_RETURN_FAILURE;
    //ret_value = MMISMS_GetNDTNvSetting(TELE_NV_CELL_BROADCAST_CHANNEL,&return_channel_list);
    ret_value = MMISMS_GetNDTNvSetting(TELE_NV_CELL_BROADCAST_CHANNEL,&cell_broadcast_channel);
    
    if (MN_RETURN_SUCCESS != ret_value)
    {    
        return_channel_list.num = 0;
    }
    else
    {
        return_channel_list.num = 0;
        for(i = 0; i < 5; i++)
        {
            if(0 != cell_broadcast_channel[i])
            {
                return_channel_list.channel_arr[i].msg_id = cell_broadcast_channel[i];
                return_channel_list.channel_arr[i].is_enabled = TRUE;
                return_channel_list.num++;
            }
            else
            {
                 break;
            }
        }
    }
    return return_channel_list;
}

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         InitCB_ETWSChannel
//  功能描述:       
//  全局变量使用:    cell_broadcast_channel
//  返回值:          
//  作者:   
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void InitCB_ETWSChannel(MN_DUAL_SYS_E dual_sys)
{
    uint16 i = 0;
    CMAS_CHANNEL_INFO_T* current_channel_table = PNULL;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 sim_ok_array[MMI_DUAL_SYS_MAX] = {0,};
    uint32 sim_ok_num = 0;
#endif
    MMI_STRING_T etws_title = {0};

    SCI_TRACE_LOW("[MMISMSCB] InitCB_ETWSChannel");

    current_channel_table = GetCMASChannelTable(dual_sys);

    for(i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i ++)
    {
        SCI_MEMSET( &s_etws_channel_list[i] , 0, sizeof(MMISMS_CB_CHANNEL_T));
        MMIAPISMSCB_SetETWSNum(0);
    }

    if( PNULL == current_channel_table )
    {
        return;
    }

    i = 0;
    do
    {
        s_etws_channel_list[dual_sys].channel_arr[i].msg_id = current_channel_table[i].channel_id;
        s_etws_channel_list[dual_sys].channel_arr[i].is_enabled = current_channel_table[i].is_default_enabled;
        if( 0 != current_channel_table[i].title_string_id )
        {
            MMI_GetLabelTextByLang(current_channel_table[i].title_string_id , &etws_title);
            s_etws_channel_list[dual_sys].channel_arr[i].name.wstr_len = (uint16)MIN(MMI_CB_CHANNEL_NAME_LEN, etws_title.wstr_len);
            MMIAPICOM_Wstrncpy(s_etws_channel_list[dual_sys].channel_arr[i].name.wstr, etws_title.wstr_ptr, etws_title.wstr_len);
        }
        else
        {
            s_etws_channel_list[dual_sys].channel_arr[i].name.wstr_len = (uint16)MIN(MMI_CB_CHANNEL_NAME_LEN, MMIAPICOM_Wstrlen(current_channel_table[i].title_id));
           MMIAPICOM_Wstrncpy(s_etws_channel_list[dual_sys].channel_arr[i].name.wstr, current_channel_table[i].title_id, s_etws_channel_list[dual_sys].channel_arr[i].name.wstr_len);
        }
        i++;
    }
    while(0 != current_channel_table[i].channel_id);
    s_etws_channel_list[dual_sys].num = i;

    MMIAPISMSCB_SetETWSNum(s_etws_channel_list[dual_sys].num);
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMarcoMsgToCBMsgList
//  功能描述:        将收到的新小区广播加入到广播收件箱列表中去
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddNewMarcoMsgToCBMsgList(
                                APP_MN_SMSCB_MACRO_MSG_IND_T *cb_msg_ptr    //IN: 广播信息信号指针
                                )
{
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *last_msg_ptr = PNULL;
    uint16 msg_real_len = 0;
#ifdef MMI_ETWS_SUPPORT
	MMISMS_CB_MESSAGE_ARRAY_T msg_arr_ptr = {0};
	MMISMS_CB_MESSAGE_ARRAY_T* cb_message_array = PNULL;
	MMISMS_CB_MESSAGE_ARRAY_T* temp_message_array = PNULL;
	uint8 index = 0;
	uint8 read_count = 0;
#endif

    if (PNULL == cb_msg_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:AddNewMarcoMsgToCBMsgList cb_msg_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2220_112_2_18_2_55_33_892,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"SMSCB: AddNewMarcoMsgToCBMsgList"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2223_112_2_18_2_55_33_893,(uint8*)"");
    
    cur_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
    if (PNULL == cur_msg_ptr)
    {
        //SCI_TRACE_LOW:"SMSCB: cur_msg_ptr alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2228_112_2_18_2_55_33_894,(uint8*)"");
        return;
    }
    //SCI_MEMSET(cur_msg_ptr, 0 , sizeof(MMISMS_CB_MESSAGE_ITEM_T));

#ifdef MMI_ETWS_SUPPORT
    s_cur_display_cb_msg_index = 0;
#endif
    // 先给相应的广播数组项赋值并设为未读，然后处理链表
    cur_msg_ptr->msg_id  = cb_msg_ptr->msg_id;
    cur_msg_ptr->is_read = FALSE;
    cur_msg_ptr->dual_sys = cb_msg_ptr->dual_sys;
    //将收到的广播消息的内容存入消息内容结构数组中去
    if (cb_msg_ptr->dcs.alphabet_type == MN_SMS_UCS2_ALPHABET)
    {
        msg_real_len = MMIAPICOM_Wstrlen((const wchar*)cb_msg_ptr->msg_arr) * sizeof(wchar);
    } 
    else
    {
        msg_real_len = strlen((char *)cb_msg_ptr->msg_arr);
    }
    ConvertCBMsgContextToMsgItem(
        cb_msg_ptr->msg_arr,
        MIN(cb_msg_ptr->msg_len, msg_real_len),
        cb_msg_ptr->dcs.alphabet_type,
        TRUE,
        cur_msg_ptr
        );
    //获得接受到广播时的系统日期和时间
    TM_GetSysTime(&cur_msg_ptr->time);
    TM_GetSysDate(&cur_msg_ptr->date);

#ifdef MMI_ETWS_SUPPORT
    if( PNULL == s_cb_msg.msg_ptr )
    {	
        SCI_TRACE_LOW("MMISMSCB: s_cb_msg.msg_ptr is PNULL");
        s_cb_msg.num = MMISMS_NV_LoadSmsCBMessageListNum();
        read_count = 0;

        if( s_cb_msg.num > 0 )
        {
            temp_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
            SCI_MEMSET(temp_msg_ptr , 0, sizeof(MMISMS_CB_MESSAGE_ITEM_T));	
            s_cb_msg.msg_ptr = temp_msg_ptr;

            cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));	

            if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
            {
                SCI_MEMCPY(temp_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
                read_count ++;
            }
            else
            {
                SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
            }

            for( index = 1; index < s_cb_msg.num; index ++)
            {
                last_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
                SCI_MEMSET(last_msg_ptr , 0, sizeof(MMISMS_CB_MESSAGE_ITEM_T));	
                if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
                {
                    SCI_MEMCPY(last_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
                    temp_msg_ptr->msg_next_ptr = last_msg_ptr;
                    temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
                }
                else
                {
                    SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
                }
            }
        }
    }
#endif

    //收件箱的广播消息总数加一
    s_cb_msg.num++;
    if (PNULL == s_cb_msg.msg_ptr)
    {
        s_cb_msg.msg_ptr = cur_msg_ptr;
    }
    else
    {//结构调整，以前是最新的放在最后，调整后最新的放在最前面        
        cur_msg_ptr->msg_next_ptr = s_cb_msg.msg_ptr;
        s_cb_msg.msg_ptr = cur_msg_ptr;

        if (s_cb_msg.num > MMI_MAX_CB_MSG_NUM)
        {//如果大于最大条数，销毁最后一个节点，把最新的插入到头节点。
            s_cb_msg.num = MMI_MAX_CB_MSG_NUM;
            temp_msg_ptr = s_cb_msg.msg_ptr;
            while (PNULL != temp_msg_ptr->msg_next_ptr)
            {//找到最后一个节点，然后销毁空间，倒数第二个节点next要置空
                last_msg_ptr = temp_msg_ptr;
                temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
            }
            if (PNULL != last_msg_ptr)
            {
                last_msg_ptr->msg_next_ptr = PNULL;
            }            
            SCI_FREE(temp_msg_ptr);
            temp_msg_ptr = PNULL;
        }
    }

#ifdef MMI_ETWS_SUPPORT
    temp_msg_ptr = s_cb_msg.msg_ptr;

    index = 0;
    cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
    if ( PNULL == cb_message_array)
    {
        SCI_TRACE_LOW("MMISMSCB: cb_message_array = PNULL!!");
        return ;
    }
    SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));	

    MMISMSNV_SaveSmsCBMessageListNum(s_cb_msg.num);

    while( (PNULL != temp_msg_ptr) && ( index < s_cb_msg.num))
    {
        SCI_MEMCPY(cb_message_array, temp_msg_ptr, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
        MMIAPISMS_NV_WriteSmsCBItem(TRUE, index, cb_message_array);
        temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
        index ++;
    }

    if( PNULL != cb_message_array)
    {
        SCI_FREE(cb_message_array);
        cb_message_array = PNULL;
    }
#endif

}

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_IsCBDefaultEnable
//  功能描述:        根据配置化软件判断是否打开CB功能NV_VARIANT_CONFIG_TELE/CELL_BROADCAST
//  全局变量使用:    cell_broadcast
//  返回值:          TRUE 默认打开CB，FALSE 默认关闭CB
//  作者:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CONFIG_T MMISMSCB_GetCBDefaultConfig(void)
{	
    MMISMS_CB_CONFIG_T cb_config = CB_CONFIG_OFF;
    MN_RETURN_RESULT_E ret_value = MN_RETURN_FAILURE;

    ret_value = MMISMS_GetNDTNvSetting(TELE_NV_CELL_BROADCAST,&cb_config);

    SCI_TRACE_LOW("[MMISMSCB] MMISMSCB_GetCBDefaultConfig ret_value = %d, cb_config = %d.",ret_value,cb_config);

    if (MN_RETURN_SUCCESS != ret_value)
    {    
        cb_config = CB_CONFIG_OFF;
    }

    return cb_config;
}

#else /* MMI_ETWS_SUPPORT */
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_IsCBDefaultEnable
//  功能描述:        根据配置化软件判断是否打开CB功能NV_VARIANT_CONFIG_TELE/CELL_BROADCAST
//  全局变量使用:    cell_broadcast
//  返回值:          TRUE 默认打开CB，FALSE 默认关闭CB
//  作者:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMISMSCB_IsCBDefaultEnable(void)
{	
    BOOLEAN bEnabled = FALSE;
    MN_RETURN_RESULT_E ret_value = MN_RETURN_FAILURE;
    ret_value = MMISMS_GetNDTNvSetting(TELE_NV_CELL_BROADCAST,&bEnabled);
    SCI_TRACE_LOW("[MMISMSCB] MMISMSCB_IsCBDefaultEnable ret_value = %d, bEnabled = %d.",ret_value,bEnabled);
    if (MN_RETURN_SUCCESS != ret_value)
    {    
        bEnabled = TRUE;
    }

    return bEnabled;
}
#endif /* MMI_ETWS_SUPPORT */

//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_GetCBMsgType
//  功能描述           :        
//  全局变量使用:    s_is_alert_type_msg
//  返回值                 :    TRUE    : Alert Message
//                                          FALSE  : Normal Message 
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMIAPISMSCB_GetCBMsgType(void)
{
    return s_is_alert_type_msg;
}

 /*****************************************************************************/
//  Description : get cbmi from sim
//  Global resource dependence : none
//  Author: CBK
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetCBMIFromSim(DPARAM param)
{
    APP_MN_SIM_READY_IND_T      sim_ready_ind   = *((APP_MN_SIM_READY_IND_T *)param);

    uint16 file_id = 0x6F45; //EFCBMI
    uint8 path_len = 6;
    uint8 path_ptr[6] = {0x3F, 0x00, 0x7F, 0xFF, 0x6F, 0x45};

    if(sim_ready_ind.is_usim)
    {
        //USIM卡的路径3F 00 7F FF 6F 45
        path_ptr[3] = 0xFF; 
    }
    else
    {
        //SIM卡的路径3F 00 7F 20 6F 45
        path_ptr[3] = 0x20;
    }

    s_cb_all_alert_info[sim_ready_ind.dual_sys].sim_cbmi_num = 0;

    /*if((sim_ready_ind.dual_sys >= MMI_DUAL_SYS_MAX) || 
        (!s_cb_cbmis[sim_ready_ind.dual_sys].num && (ERR_MNDATAMAG_NO_ERR != MNSIM_GetSimBinaryFileEx(sim_ready_ind.dual_sys,mn_file_name))) )
    {
        return FALSE;
        SCI_TRACE_LOW("SMSCB: MMIAPISMSCB_GetCBMIFromSim Get Failed!!!dual_sys=%d", sim_ready_ind.dual_sys);
    }*/


#ifndef WIN32
    MNSIM_ReadSimEF(sim_ready_ind.dual_sys, file_id, path_len, path_ptr);
#endif


    return TRUE;
}

 /*****************************************************************************/
//  Description : get g_cb_alert_id[index]
//  Global resource dependence : none
//  Author: CBK
//  Note: 
/*****************************************************************************/
PUBLIC MN_CB_MSG_ID_T MMIAPISMSCB_GetCBAlertID(uint8 index)
 {
     return g_cb_alert_id[index];
 }
//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_GetCBAllAlertInfo
//  功能描述           :        
//  全局变量使用:    s_cb_all_alert_info
//  返回值                 :    
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MN_CB_ALERT_MSG_T * MMIAPISMSCB_GetCBAllAlertInfo(MN_DUAL_SYS_E dual_sys)
{
    return  &s_cb_all_alert_info[dual_sys];
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description :
//    Global resource dependence : 
//  Author:Candice.Chen
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetETWSNum(void)
{
    return s_etws_alert_num;
}

/*****************************************************************************/
//     Description :
//    Global resource dependence : 
//  Author:Candice.Chen
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetETWSNum(uint16 num)
{
    s_etws_alert_num = num;
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_InitCBAlertInfo
//  功能描述           :        
//  全局变量使用:    s_cb_alert_msg
//  返回值                 :    void 
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMIAPISMSCB_InitCBAlertInfo(MN_DUAL_SYS_E dual_sys)
{
    uint8 i = 0;
    MMISMS_CB_CHANNEL_T channel_list = {0};
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_CHANNEL_T etws_channel_list = {0};
#endif

    s_cb_all_alert_info[dual_sys].emegence_num = 0;
#ifdef MMI_ETWS_SUPPORT
    InitCB_ETWSChannel(dual_sys);
    SCI_MEMCPY(&etws_channel_list, &s_etws_channel_list, sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = InitCBSoftConfChannel();
#endif

#ifdef MMI_ETWS_SUPPORT
    // ETWS
    s_cb_all_alert_info[dual_sys].emegence_num = etws_channel_list.num;
    for(i = 0; i < s_cb_all_alert_info[dual_sys].emegence_num; i++)
    {
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].type= MMI_CB_ETWS_ID;
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].cb_alert_id = etws_channel_list.channel_arr[i].msg_id;
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].name.wstr_len = etws_channel_list.channel_arr[i].name.wstr_len;
        MMIAPICOM_Wstrncpy(s_cb_all_alert_info[dual_sys].cb_alert_content[i].name.wstr, etws_channel_list.channel_arr[i].name.wstr, s_cb_all_alert_info[dual_sys].cb_alert_content[i].name.wstr_len ) ;
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].is_enabled = etws_channel_list.channel_arr[i].is_enabled;
    }
    MMIAPISMSCB_SetETWSNum(s_cb_all_alert_info[dual_sys].emegence_num);

	//Default emergency
    if(0 == s_cb_all_alert_info[dual_sys].emegence_num)
    {
        for(i = 0; i < MMI_CB_ALERT_NUM; i++)
        {
            s_cb_all_alert_info[dual_sys].cb_alert_content[i].type = MMI_CB_EMERGENCE_ID;
            s_cb_all_alert_info[dual_sys].cb_alert_content[i].cb_alert_id = g_cb_alert_id[i];
            s_cb_all_alert_info[dual_sys].emegence_num++;
        }
    }
#else	
    for(i = 0; i < MMI_CB_ALERT_NUM; i++)
    {
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].type = MMI_CB_EMERGENCE_ID;
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].cb_alert_id = g_cb_alert_id[i];
        s_cb_all_alert_info[dual_sys].emegence_num++;
    }
#endif
	
    s_cb_all_alert_info[dual_sys].soft_conf_num = channel_list.num;
    for(i = 0; i < s_cb_all_alert_info[dual_sys].soft_conf_num; i++)
    {
        s_cb_all_alert_info[dual_sys].soft_conf_id[i] = channel_list.channel_arr[i].msg_id;
    }
	
}

/*****************************************************************************/
//  Description : store cbmi value
//  Global resource dependence : none
//  Author: CBK
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_StoreCBMIValue(MN_CB_ALERT_MSG_T *file_buf, MN_DUAL_SYS_E dual_sys)
{
    uint8 uiCurNum = 0;
    MN_CB_ALERT_MSG_T cb_alert_msg;

     SCI_TRACE_LOW("SMSCB: MMISMSCB_StoreCBMIValue dual_sys:%d, num:%d",dual_sys, file_buf->sim_cbmi_num); 

    //modified by Tong Yongli. According to Coverity CID 26099. && --> ||
    if((dual_sys >= MMI_DUAL_SYS_MAX) || ((MMI_MAX_CB_CHANNEL_NUM - file_buf->emegence_num) <= file_buf->sim_cbmi_num))
    {
        SCI_TRACE_LOW("MMIAPISMSCB_StoreCBMIValue --> CBMI numbers is up to MMI_MAX_CB_CHANNEL_NUM.");
        return FALSE;
    }
    
    s_cb_all_alert_info[dual_sys].sim_cbmi_num
		= file_buf->sim_cbmi_num;
    s_cb_all_alert_info[dual_sys].emegence_num = MMI_CB_ALERT_NUM;

    for(uiCurNum = s_cb_all_alert_info[dual_sys].emegence_num; uiCurNum < s_cb_all_alert_info[dual_sys].sim_cbmi_num + s_cb_all_alert_info[dual_sys].emegence_num; uiCurNum++)
    {
        s_cb_all_alert_info[dual_sys].cb_alert_content[uiCurNum].type = MMI_CB_SIM_CBMI_ID;
        s_cb_all_alert_info[dual_sys].cb_alert_content[uiCurNum].cb_alert_id = file_buf->cb_alert_content[uiCurNum].cb_alert_id;
        SCI_TRACE_LOW("MMIAPISMSCB_StoreCBMIValue %d.",file_buf->cb_alert_content[uiCurNum].cb_alert_id);
    }
    return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMsgNodeToCBMsgList
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            CBK
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void AddNewMsgNodeToCBMsgList(MMISMS_CB_MESSAGE_ITEM_T* cur_msg_ptr)
{
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *last_msg_ptr = PNULL;

    //收件箱的广播消息总数加一
    s_cb_msg.num++;
    if (PNULL == s_cb_msg.msg_ptr)
    {
        s_cb_msg.msg_ptr = cur_msg_ptr;
    }
    else
    {//结构调整，以前是最新的放在最后，调整后最新的放在最前面        
        cur_msg_ptr->msg_next_ptr = s_cb_msg.msg_ptr;
        s_cb_msg.msg_ptr = cur_msg_ptr;
        
        if (s_cb_msg.num > MMI_MAX_CB_MSG_NUM)
        {//如果大于最大条数，销毁最后一个节点，把最新的插入到头节点。
            s_cb_msg.num = MMI_MAX_CB_MSG_NUM;
            temp_msg_ptr = s_cb_msg.msg_ptr;
            while (PNULL != temp_msg_ptr->msg_next_ptr)
            {//找到最后一个节点，然后销毁空间，倒数第二个节点next要置空
                last_msg_ptr = temp_msg_ptr;
                temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
            }
            if (PNULL != last_msg_ptr)
            {
                last_msg_ptr->msg_next_ptr = PNULL;
            }            
            SCI_FREE(temp_msg_ptr);
            temp_msg_ptr = PNULL;
        }
    }
}

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Num
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:           Candice
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC void MMISMS_Set_CB_Message_Num( uint16 num )
{
    if( num <= MMINV_MAX_SMSCB_RECORDS )
        s_cb_msg.num = num;
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Ptr
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:         
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_MESSAGE_T *MMISMS_Get_CB_Message_Parent_Ptr( void )
{
    return &s_cb_msg;
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Num
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC uint16 MMISMS_Get_CB_Message_Num( void )
{
    return s_cb_msg.num;
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Ptr
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC MMISMS_CB_MESSAGE_ITEM_T *MMISMS_Get_CB_Message_Ptr( void )
{
    return s_cb_msg.msg_ptr;
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_DeleteMsgFromCBMsgList
//  功能描述:        从小区广播收件箱列表中删除一条记录
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            liqing.peng
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMS_DeleteMsgFromCBMsgList(
                                  uint16      index
                                  )
{
    uint32 i = 0;    
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_MESSAGE_ARRAY_T* cb_message_array = PNULL;
    MMISMS_CB_MESSAGE_ARRAY_T* temp_message_array = PNULL;
    uint8 cb_num = 0;
    uint8 read_count = 0;
#endif	

    //SCI_TRACE_LOW:"SMSCB: DeleteMsgFromCBMsgList num = d%, index = d%"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2297_112_2_18_2_55_33_895,(uint8*)"", s_cb_msg.num, index);
    if (0 == s_cb_msg.num || MMI_MAX_CB_MSG_NUM < index)
    {
        return;
    }

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
    //读取最新一条的时候，需要停止滚动
    cur_msg_ptr = s_cb_msg.msg_ptr;

    if (0 == index && !cur_msg_ptr->is_read)
    {
        GUISTBSCROLLMSG_SetIsValid(GUISTBSCROLLMSG_SCROLL_ITEM_NEW_SMSCB, FALSE);
    }
#endif

    if (1 == s_cb_msg.num)
    {
        temp_msg_ptr = s_cb_msg.msg_ptr;
        SCI_FREE(temp_msg_ptr);
        temp_msg_ptr = PNULL;
        s_cb_msg.msg_ptr = PNULL;
		
#ifdef MMI_ETWS_SUPPORT
        MMISMS_NV_DelAllSmsCBItem();
#endif
    }
    else
    {
        if (0 == index)
        {//删除第一个节点
            temp_msg_ptr = s_cb_msg.msg_ptr;
            s_cb_msg.msg_ptr = temp_msg_ptr->msg_next_ptr;
            SCI_FREE(temp_msg_ptr);
            temp_msg_ptr = PNULL;
        }
        else
        {
            temp_msg_ptr = s_cb_msg.msg_ptr;
            //查找到需要删除的节点
            for (i=0; i < index; i++)
            {
                cur_msg_ptr = temp_msg_ptr;
                temp_msg_ptr = temp_msg_ptr->msg_next_ptr; 
            }
            if (PNULL != cur_msg_ptr)
            {
                cur_msg_ptr->msg_next_ptr = temp_msg_ptr->msg_next_ptr;
            }
            SCI_FREE(temp_msg_ptr);
            temp_msg_ptr = PNULL;
        }
    }
    s_cb_msg.num--;

#ifdef MMI_ETWS_SUPPORT
    temp_msg_ptr = s_cb_msg.msg_ptr;

    index = 0;
    cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
    if ( PNULL == cb_message_array)
    {
        return ;
    }
    SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));	

    MMISMSNV_SaveSmsCBMessageListNum(s_cb_msg.num);

    while( (PNULL != temp_msg_ptr) && ( index < s_cb_msg.num))
    {
        SCI_MEMCPY(cb_message_array, temp_msg_ptr, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
        MMIAPISMS_NV_WriteSmsCBItem(TRUE, index, cb_message_array);
        temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
        index ++;
    }

    if( s_cb_msg.num < MMINV_MAX_SMSCB_RECORDS)
        MMISMS_NV_DelSmsCBItem(s_cb_msg.num, MMINV_MAX_SMSCB_RECORDS-1);	

    if( PNULL != cb_message_array)
    {
        SCI_FREE(cb_message_array);
        cb_message_array = PNULL;
    }
#endif

    //SCI_TRACE_LOW:"SMSCB: DeleteMsgFromCBMsgList num1 = d%"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2337_112_2_18_2_55_33_896,(uint8*)"", s_cb_msg.num);
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         IsCBMsgListEmpty
//  功能描述:        判断广播收件箱是否为空
//  全局变量使用:    s_cb_msg
//  返回值:          TRUE 为空，FALSE 不为空
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMISMS_IsCBMsgListEmpty(void)
{
#ifdef MMI_ETWS_SUPPORT
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    MMISMS_CB_MESSAGE_ARRAY_T* cb_message_array = PNULL;
    uint8 cb_num = 0;
#endif

    if(0 == s_cb_msg.num)
    {
#ifdef MMI_ETWS_SUPPORT
        cb_num = MMISMS_NV_LoadSmsCBMessageListNum();
        if( cb_num > 0)
            return FALSE;
        else
            return TRUE;
#else
        return TRUE;
#endif
    }
    else
    {
        return FALSE;
    }
}

#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
//  Description : Get Unread SMS CB Number
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetUnreadSMSCBNum(void)
{
    uint16 unread_num = 0;
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;

    if (0 == s_cb_msg.num)
    {
        return unread_num;
    }

    cur_msg_ptr = s_cb_msg.msg_ptr;

    while (PNULL != cur_msg_ptr)
    {
        if (!cur_msg_ptr->is_read)
        {
            unread_num++;
        }
        
        cur_msg_ptr = cur_msg_ptr->msg_next_ptr; 
    }

    return unread_num;
}

/*****************************************************************************/
//  Description : Get Unread SMS CB Channel
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetUnreadSMSCBChannel(wchar *name_ptr, uint16 max_len)
{
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMISMS_CB_CHANNEL_NAME_T channel_name = {0};
    uint8 channel_str[MMI_CB_CHANNEL_NAME_LEN] = {0};
    uint16 channel_len = 0;

    if (0 == s_cb_msg.num || PNULL == s_cb_msg.msg_ptr || PNULL == name_ptr || 0 == max_len)
    {
        return FALSE;
    }

    cur_msg_ptr = s_cb_msg.msg_ptr;

    while(PNULL != cur_msg_ptr)
    {
        if (!cur_msg_ptr->is_read)
        {
            break;
        }
        else
        {
            cur_msg_ptr = cur_msg_ptr->msg_next_ptr;
        }
    }

    if (PNULL == cur_msg_ptr)
    {
        return FALSE;
    }

    if (cur_msg_ptr->dual_sys < MMI_DUAL_SYS_MAX)   
    {
        channel_list = MMISMS_NV_LoadSmsCBChannelList(cur_msg_ptr->dual_sys);
    }

    channel_name = MMISMS_GetCBChannelNameByMsgId(&channel_list, cur_msg_ptr->msg_id);

    if (0 == channel_name.wstr_len)
    {
        channel_len = sprintf((char *)channel_str, "%d", cur_msg_ptr->msg_id);

        MMI_STRNTOWSTR(channel_name.wstr,
                                MMI_CB_CHANNEL_NAME_LEN,
                                (uint8 *)channel_str,
                                MMI_CB_CHANNEL_NAME_LEN,
                                channel_len
                                );

        channel_name.wstr_len = channel_len;
    }

    channel_name.wstr_len = MIN(max_len, channel_name.wstr_len);

    MMI_WSTRNCPY(name_ptr, channel_name.wstr_len, channel_name.wstr, channel_name.wstr_len, channel_name.wstr_len);

    return TRUE;
}

/*****************************************************************************/
//  Description : Get Unread SMS CB Time
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetUnreadSMSCBTime(wchar *time_ptr, uint16 max_len)
{
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
    uint8 time[MMI_DISPLAY_TIME_LEN + 1] = {0};
    uint16 time_len = 0;

    if (0 == s_cb_msg.num || PNULL == s_cb_msg.msg_ptr || PNULL == time_ptr || 0 == max_len)
    {
        return FALSE;
    }

    cur_msg_ptr = s_cb_msg.msg_ptr;

    while(PNULL != cur_msg_ptr)
    {
        if (!cur_msg_ptr->is_read)
        {
            break;
        }
        else
        {
            cur_msg_ptr = cur_msg_ptr->msg_next_ptr;
        }
    }

    if (PNULL == cur_msg_ptr)
    {
        return FALSE;
    }

    MMIAPISET_FormatTimeStrByTime(cur_msg_ptr->time.hour, cur_msg_ptr->time.min, time, MMI_DISPLAY_TIME_LEN);

    time_len = strlen((char *)time);

    time_len = MIN(max_len, time_len);

    MMI_STRNTOWSTR(time_ptr, time_len, time, time_len, time_len);

    return TRUE;
}
#endif

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetScrollItemNewSMSCB(void)
{
    GUISTBSCROLLMSG_SCROLL_ITEM_T scroll_item = {0};
    MMI_STRING_T msg_arr[3] = {0};
    wchar colon_str[]=L": \n";
    uint32 total_len = 0;
    MMI_STRING_T smscb_name = {0};
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;

    if (0 == s_cb_msg.num || PNULL == s_cb_msg.msg_ptr)
    {
        return;
    }
    
    scroll_item.is_valid = TRUE;
    scroll_item.type = GUISTBSCROLLMSG_SCROLL_ITEM_NEW_SMSCB;

    //get smscb name
    MMI_GetLabelTextByLang(STR_CB_NEW, &smscb_name);

    //icon
    scroll_item.icon_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;

    cur_msg_ptr = s_cb_msg.msg_ptr;

    msg_arr[0] = smscb_name;
    msg_arr[1].wstr_ptr = colon_str;
    msg_arr[1].wstr_len = MMIAPICOM_Wstrlen(colon_str);
    msg_arr[2].wstr_ptr = cur_msg_ptr->context;
    msg_arr[2].wstr_len = cur_msg_ptr->len;

    total_len = msg_arr[0].wstr_len+msg_arr[1].wstr_len+msg_arr[2].wstr_len;
    scroll_item.string_info.wstr_ptr = SCI_ALLOCAZ((total_len + 1)*sizeof(wchar));
    scroll_item.string_info.wstr_len = total_len;
    MMIAPICOM_ConnectSomeString(msg_arr, 3, &scroll_item.string_info);

    GUISTBSCROLLMSG_AppendScrollItem(&scroll_item);

    if (PNULL != scroll_item.string_info.wstr_ptr)
    {
        SCI_FREE(scroll_item.string_info.wstr_ptr);
    }   
}
#endif


#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_HandleETWSVibrateTimer(uint8 timer_id, uint32 param)
{
    BOOLEAN is_continued = (BOOLEAN) param;

    if( 0 == is_continued )
    {
        MMIAPISET_StopSOSVibrate();

        MMK_StopTimer(s_vibrator_period_time_id);
        s_vibrator_period_time_id = 0;
    }

#ifdef CAMERA_SUPPORT
    if (MMK_IsOpenWin(MMIDC_MAIN_WIN_ID))
    {
        s_is_etws_display = TRUE;
    }
#endif

    MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);

    MMIDEFAULT_TurnOnBackLight();
#ifdef SCREENSAVER_SUPPORT
    if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
    {
        MMIAPIIDLESS_CloseScreenSaver();
    }
#endif
    s_is_need_enable = TRUE;

    MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);

    if( MMICC_IsExistIncommingCall())
    {
        MMIAPICC_PlayNewCallRing(MMIAPICC_GetCallDualSys());
    }
    else if(MMIALM_GetAlarmVibrateDelayedFlag())
    {
        MMIALM_SetAlarmVibrateDelayedFlag(FALSE);
        MMIALM_StartRingOrVib();
    }
#ifdef COUNTEDTIME_SUPPORT
    else if(MMIAPICT_GetCTVibrateDelayedFlag())
    {
        MMIAPICT_SetCTVibrateDelayedFlag(FALSE);
        MMIAPICT_StartRingOrVib();
    }
#endif
}

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_IsETWSAlert(void)
{
    if( 0 == s_vibrator_period_time_id  )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

PUBLIC void MMIAPISMSCB_ETWSRedisplaySetIsNeedVibrate(BOOLEAN vibrate_enable)
{
    s_etws_redisplay_is_need_vibrate = vibrate_enable;
}

PUBLIC BOOLEAN MMIAPISMSCB_ETWSRedisplayGetIsNeedVibrate()
{
    return s_etws_redisplay_is_need_vibrate;
}

PUBLIC BOOLEAN MMIAPISMSCB_IsETWSAlerting()
{
    return s_vibrator_period_time_id != 0;
}

PUBLIC void MMIAPISMSCB_RedisplayETWSMsg()
{
    MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;
    
    SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(s_etws_delay_show_dual_sim_context);

    if(MMIAPISMSCB_IsSetMcc())
    {
        plmn_info.mcc = MMIAPISMSCB_GetSetMcc();
    }

    if(MMIAPISMSCB_ETWSRedisplayGetIsNeedVibrate() && (s_etws_vibrate_type_context != 0))
    {
        if(CB_PWS_MCC == plmn_info.mcc)
        {
            MMIAPISET_SetSOSVibrate(s_etws_delay_show_dual_sim_context , TRUE, FALSE);
        }
        else if( CB_NZ_MCC== plmn_info.mcc)
        {
            MMIAPISET_SetSOSVibrate(s_etws_delay_show_dual_sim_context , TRUE, TRUE);
        }
        else
        {
            MMIAPISET_SetSOSVibrate(s_etws_delay_show_dual_sim_context , FALSE, FALSE);
        }

        switch(s_etws_alert_period_context)
        {
            case 5:
                s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_5SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, s_etws_is_contiued_context, FALSE);
                break;
            case 30:
                s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_30SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, s_etws_is_contiued_context, FALSE);
                break;
            case 10:
            default:
                s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_10500MSECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, s_etws_is_contiued_context, FALSE);
                break;
        }
    }

    MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
    MMIDEFAULT_TurnOnBackLight();

#ifdef SCREENSAVER_SUPPORT
    if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
    {
        MMIAPIIDLESS_CloseScreenSaver();
    }
#endif

    MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
    MMISMSCB_ETWSSetisNeedRedisplay(FALSE);
    MMIAPISMSCB_ETWSRedisplaySetIsNeedVibrate(FALSE);
    s_etws_vibrate_type_context = 0;
    s_etws_is_contiued_context = 0;
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_StopETWSVibrateTimer(void)
{
    MMIAPISET_StopSOSVibrate();

    MMK_StopTimer(s_vibrator_period_time_id);
    s_vibrator_period_time_id = 0;	
}


/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMISMSCB_HandleETWSRepeatedTimer(uint8 timer_id, uint32 param)
{
    s_repeated_channel = 0;

    MMK_StopTimer(s_repeated_period_time_id);
    s_repeated_period_time_id = 0;	
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMIAPISMSCB_HandleCBMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleCBMsgInd(
                             DPARAM        param            //IN: 系统回调参数

                             )
{
    APP_MN_SMSCB_MSG_IND_T *cb_msg_ptr = (APP_MN_SMSCB_MSG_IND_T *)param;
#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
    BOOLEAN cell_info_flag = FALSE;
#endif
#ifdef MMI_ETWS_SUPPORT
    BOOLEAN cb_card_enabled = FALSE;
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MMI_STRING_T prompt_content_str = {0};
    MMI_STRING_T prompt_str = {0};
    MMISMS_CB_MESSAGE_ITEM_T* cb_msg_item = {0};
    uint32 msg_real_len = 0;
    uint8 ring_vol=0;
    uint16 etws_alert_num = 0;
    wchar time_str[CB_MSG_TIME_MAX_LEN + 1] = {0};	
    uint16 time_len = 0;
    uint16 etws_context_len = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    uint16 etws_title_len = 0;
    MMI_STRING_T etws_title = {0};
    MMISMS_CB_LANGUAGE_T lang_list = {0};
    uint32 lang_index = 0;
#endif
	
    if (PNULL == cb_msg_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMIAPISMSCB_HandleCBMsgInd cb_msg_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2492_112_2_18_2_55_33_899,(uint8*)"");
        return;
    }
    SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd");

#ifdef MMI_ETWS_SUPPORT
    etws_alert_num = MMIAPISMSCB_GetETWSNum();

    if( etws_alert_num > 0)
    {
//        MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
        MN_CB_ALERT_MSG_T *cb_msg_id_ptr = PNULL;
        MMISMS_CB_CHANNEL_T etws_channel_list = {0};

        InitCB_ETWSChannel(cb_msg_ptr->dual_sys);
        SCI_MEMCPY(&etws_channel_list, &s_etws_channel_list, sizeof(MMISMS_CB_CHANNEL_T));
    }

    current_channel_info = GetCMASChannelInfo(cb_msg_ptr->dual_sys, cb_msg_ptr->msg_id);

    if( NULL != current_channel_info)
    {
        cb_msg_item = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
        if (PNULL == cb_msg_item)
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, allocate cb_msg_item buffer failed");
            return;
        }
		
        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, ETWS received");
        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, channel_id = %d, associated_channel = %d, language_id = %d", current_channel_info->channel_id , current_channel_info->associated_channel, cb_msg_ptr->dcs.language_id);

    // stop recording
#ifdef MMI_RECORD_SUPPORT
        if( MMIAPIRECORD_IsRecording() )
        {
            MMK_SendMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_APP_OK, PNULL);
        }
#endif
#ifdef CAMERA_SUPPORT
        if (MMK_IsOpenWin(MMIDC_MAIN_WIN_ID))
        {
            s_is_etws_display = TRUE;
        }
        MMIAPIDC_Interrupt();
#endif    

        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(cb_msg_ptr->dual_sys);

        if(MMIAPISMSCB_IsSetMcc())
        {
            plmn_info.mcc = MMIAPISMSCB_GetSetMcc();
        }

        if( CB_IT_MCC == plmn_info.mcc )
        {
            if( (4370 != current_channel_info->channel_id ) && (4370 != current_channel_info->associated_channel))
            {
                MMIAPISMSCB_NV_LoadSmsCBLanguageList(cb_msg_ptr->dual_sys, &lang_list);
                lang_index = MMISMSCB_ConvertLangIDToLangIndex(cb_msg_ptr->dcs.language_id);
                if( (FALSE ==lang_list.selected_arr[0]) &&
                     (FALSE == lang_list.selected_arr[lang_index] ) )
                {
                    return;
                }
            }
        }

        // add cb to msg list
        AddNewMsgToCBMsgList(cb_msg_ptr, FALSE);

        // ignore secondary channel cb
        if( 1 == current_channel_info->primary_channel)
        {
            s_repeated_channel = current_channel_info->secondary_channel;
            s_repeated_period_time_id = MMK_CreateTimerCallback(MMI_5MINUTES,  MMISMSCB_HandleETWSRepeatedTimer, NULL, FALSE);
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, current_channel_info->primary_channel = %d", current_channel_info->primary_channel);
        }
        else  if(( 1 == current_channel_info->secondary_channel )  &&  (s_repeated_channel == cb_msg_ptr->msg_id)  )
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, current_channel_info->secondary_channel = %d, cb_msg_ptr->msg_id = %d", current_channel_info->secondary_channel, cb_msg_ptr->msg_id);
            return;
        }
        else
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, neither primary channel nor secondary channel");
        }

        // init popup string buffer
	etws_context_len = MMI_CB_CHANNEL_NAME_LEN + 1 \
				+CB_MSG_TIME_MAX_LEN + 1\
				+ MN_CB_MSG_LEN + 1;
        etws_title_len = MMI_CB_CHANNEL_NAME_LEN + 1;

        prompt_content_str.wstr_ptr = SCI_ALLOCAZ(MN_CB_MSG_LEN * sizeof(wchar));
        SCI_MEMSET(prompt_content_str.wstr_ptr , 0, MN_CB_MSG_LEN* sizeof(wchar));	
        prompt_content_str.wstr_len = 0;

        prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));	
        prompt_str.wstr_len = 0;

        if( PNULL != etws_prompt_str.wstr_ptr )
        {
            SCI_FREE(etws_prompt_str.wstr_ptr);
            etws_prompt_str.wstr_ptr = PNULL;
        }
        etws_prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        SCI_MEMSET(etws_prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));	
        etws_prompt_str.wstr_len = 0;
		
        if( PNULL != etws_title_str.wstr_ptr )
        {
            SCI_FREE(etws_title_str.wstr_ptr);
            etws_title_str.wstr_ptr = PNULL;
        }
        etws_title_str.wstr_ptr = SCI_ALLOCAZ(etws_title_len * sizeof(wchar));
        SCI_MEMSET(etws_title_str.wstr_ptr , 0, etws_title_len* sizeof(wchar));
        etws_title_str.wstr_len = 0;
		
        // Title
        if( (CB_CHILE_MCC == plmn_info.mcc) || (CB_IT_MCC == plmn_info.mcc ) || (CB_PE_MCC == plmn_info.mcc) )
        {
            if( 0 != current_channel_info->popup_string_id )
            {
                MMI_GetLabelTextByLang(current_channel_info->popup_string_id , &etws_title);
                MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
                MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L" ");
                prompt_str.wstr_len = etws_title.wstr_len+1;
            }
            else if( 0 != current_channel_info->title_string_id )
            {
                MMI_GetLabelTextByLang(current_channel_info->title_string_id , &etws_title);
                MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
                MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L" ");
                prompt_str.wstr_len = etws_title.wstr_len+1;
            }
            else
            {
                MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, current_channel_info->title_id, MMIAPICOM_Wstrlen(current_channel_info->title_id)) ;
                MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L" ");
                prompt_str.wstr_len = MMIAPICOM_Wstrlen(current_channel_info->title_id) +1;
            }

            // Time
            SCI_MEMSET(time_str, 0, (CB_MSG_TIME_MAX_LEN + 1));
            time_len = ConvertCBTimeToStrForTitle(
                s_cb_msg.msg_ptr->date,
                s_cb_msg.msg_ptr->time,
                time_str
                );
            MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, time_str);
            prompt_str.wstr_len += time_len;
        }
        else
        {
            if( 0 != current_channel_info->title_string_id )
            {
                MMI_GetLabelTextByLang(current_channel_info->title_string_id , &etws_title);
                MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
                MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
                prompt_str.wstr_len = etws_title.wstr_len+1;
            }
            else
            {
                MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, current_channel_info->title_id, MMIAPICOM_Wstrlen(current_channel_info->title_id)) ;
                MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
                prompt_str.wstr_len = MMIAPICOM_Wstrlen(current_channel_info->title_id) +1;
            }
        }
	
        MMIAPICOM_Wstrncpy(etws_title_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_title_str.wstr_len = prompt_str.wstr_len;

        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));
        prompt_str.wstr_len = 0;

        // Time
        if( CB_NZ_MCC == plmn_info.mcc)
        {
            SCI_MEMSET(time_str, 0, (CB_MSG_TIME_MAX_LEN + 1));		
            time_len = ConvertCBTimeToStrForDetails(
			s_cb_msg.msg_ptr->date,
			s_cb_msg.msg_ptr->time,
			time_str
			);		
            MMI_WSTRNCPY(prompt_str.wstr_ptr+prompt_str.wstr_len,
			CB_MSG_TIME_MAX_LEN,
			time_str,
			time_len,
			time_len);
            prompt_str.wstr_len += time_len;
        }

        //Content	
        MMI_WSTRNCPY(prompt_str.wstr_ptr+prompt_str.wstr_len,
			MN_CB_MSG_LEN,
			s_cb_msg.msg_ptr->context,
			s_cb_msg.msg_ptr->len,
			s_cb_msg.msg_ptr->len);
		
        prompt_str.wstr_len += s_cb_msg.msg_ptr->len;
        MMIAPICOM_Wstrncpy(etws_prompt_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_prompt_str.wstr_len = prompt_str.wstr_len;
		
        if (MMK_IsOpenWin(MMISMS_CB_MSG_LIST_WIN_ID))
        {
            uint16 cur_index = 0;

            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, MMISMS_CB_MSG_LIST_WIN_ID is open, update cb ");
            cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);;

            //刷新当前小区广播收件箱列表
            GUILIST_RemoveAllItems(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
            MMISMS_UpdateCBMsgList(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

            if( (cur_index + 1 ) < MMI_MAX_CB_MSG_NUM )
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index + 1);
            else
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index );
			
            if (MMK_IsFocusWin(MMISMS_CB_MSG_LIST_WIN_ID))
            {
                MMK_SendMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }
		
        // alert
        if( current_channel_info->alert_level > 0)
            s_is_etws_alert_msg = TRUE;
		
        if(current_channel_info->alert_tone_type)
        {
            s_is_etws_alert_msg = TRUE;
            s_is_etws_need_popup = TRUE;
        }

        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, vibrate_type=%d,mcc=%d ",current_channel_info->vibrate_type,plmn_info.mcc);

        s_etws_vibrate_type_context = current_channel_info->vibrate_type;
        // vibrate
        if(current_channel_info->vibrate_type)
        {
            // Display ETWS prompt
            MMIDEFAULT_TurnOnBackLight();

            s_etws_delay_show_dual_sim_context = cb_msg_ptr->dual_sys;

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

            if( MMICC_IsExistIncommingCall())
            {
                MMIAPICC_StopRingOrVibrate();
            }

            if( (CB_CHILE_MCC == plmn_info.mcc)
                ||(CB_NZ_MCC== plmn_info.mcc)
                ||(CB_PE_MCC== plmn_info.mcc)
                ||(CB_PWS_MCC== plmn_info.mcc))
            {
                s_is_need_enable = TRUE;
            }
            else
            {
                s_is_need_enable = FALSE;
            }

            if( (CB_PWS_MCC == plmn_info.mcc)
                || (CB_NZ_MCC== plmn_info.mcc))
            {
                s_is_personal = TRUE;
            }
            else
            {
                s_is_personal = FALSE;
            }

            if( CB_NZ_MCC == plmn_info.mcc)
            {
                s_is_need_torch = TRUE;
            }
            else
            {
                s_is_need_torch = FALSE;
            }
            s_etws_card_sys = cb_msg_ptr->dual_sys;

            MMIAPISMSCB_SetEMCallStatus(FALSE);
            MMIAPISMSCB_SetIsNeedRedisplay(FALSE);

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

            s_cur_display_cb_msg_index = 0;

            MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
            MMIAPISET_SetSOSVibrate(s_etws_card_sys , s_is_personal, s_is_need_torch);

            // stop vibration and alert after timeout
            switch(current_channel_info->alert_period)
            {
                case 5:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_5SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                    break;
					
                case 30:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_30SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                    break;
				
                case 180:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_3MINUTES,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                    break;

                case 240:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_4MINUTES,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                    break;
                
                case 10:
                default:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_10500MSECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                    break;
            }
        }
        else
        {
            if(CB_IL_MCC == plmn_info.mcc)
            {
                SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, receive israel normal alert");

                // Display ETWS prompt
                MMIDEFAULT_TurnOnBackLight();
                if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
                {
                    MMIAPISMSCB_StopETWSVibrateTimer();
                    MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
                }
                MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
                MMISMS_SetNewSMSRingFlag(TRUE);
                MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
                
                MMISMS_ShowNewSMSCBPrompt();
            }
            else
            {
                SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd normal ringtone and vibration");

                // Display ETWS prompt
                MMIDEFAULT_TurnOnBackLight();
                if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
                {
			MMIAPISMSCB_StopETWSVibrateTimer();
			MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
                }

                MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
                MMISMS_SetNewSMSRingFlag(TRUE);
                MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());

                MMIAPISMSCB_SetEMCallStatus(FALSE);
                MMIAPISMSCB_SetIsNeedRedisplay(FALSE);

                s_is_need_enable = TRUE;
                MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
            }
	}

	if( PNULL != prompt_content_str.wstr_ptr)
	{
            SCI_FREE(prompt_content_str.wstr_ptr);
            prompt_content_str.wstr_ptr = PNULL;
	}

	if( PNULL != prompt_str.wstr_ptr)
	{
            SCI_FREE(prompt_str.wstr_ptr);
            prompt_str.wstr_ptr= PNULL;
	}

        return ;
    }
#endif

    cb_card_enabled = IsCBSwitchSelected(s_dual_card_sys);
    if(!cb_card_enabled)
    {
        SCI_TRACE_LOW("MMIAPISMSCB_HandleCBMsgInd cb_card_enabled return false.");
        return;
    }

    s_is_alert_type_msg = ((cb_msg_ptr->msg_code) >> 9) & 0x1; 
    //SCI_TRACE_LOW:"SMSCB: MMIAPISMSCB_HandleCBMsgInd dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2495_112_2_18_2_55_33_900,(uint8*)"d", cb_msg_ptr->dual_sys);
#ifdef MMI_ETWS_SUPPORT
    s_is_etws_alert_msg = ((cb_msg_ptr->msg_code) >> 9) & 0x01;
    s_is_etws_need_popup = ((cb_msg_ptr->msg_code) >> 8) & 0x01;
#endif

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
    cell_info_flag = MMISMSCB_GetCellInfoFlag(cb_msg_ptr->dual_sys);
    if(cell_info_flag && (CB_CELL_INFO_INDEX_ID == cb_msg_ptr->msg_id))
    {
        if(PNULL == s_cell_info_idle_str)
        {
            s_cell_info_idle_str = SCI_ALLOCAZ(MN_CB_MSG_LEN * sizeof(uint8));
        }

        if(PNULL != s_cell_info_idle_str)
        {
            SCI_MEMSET(s_cell_info_idle_str, 0, MN_CB_MSG_LEN * sizeof(uint8));
            SCI_MEMCPY(s_cell_info_idle_str, cb_msg_ptr->msg_arr, cb_msg_ptr->msg_len);
        }
        return;
    }
#endif

    SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd: is_etws_alert_msg=%d,need_popup=%d,class_is_present=%d,sms_class=%d,display_mode=%d",\
    s_is_etws_alert_msg,s_is_etws_need_popup,cb_msg_ptr->dcs.class_is_present,cb_msg_ptr->dcs.sms_class,cb_msg_ptr->display_mode);
    if (((1 == cb_msg_ptr->dcs.class_is_present)
        && (MN_SMS_CLASS_0 == cb_msg_ptr->dcs.sms_class)
#ifdef MMI_ETWS_SUPPORT
        && (!s_is_etws_alert_msg)
#endif
        )
        || ((MN_CB_DISPLAY_IMMEDIATE == cb_msg_ptr->display_mode)
#ifdef MMI_ETWS_SUPPORT
        && (!s_is_etws_alert_msg)
#endif
        )
#ifdef MMI_ETWS_SUPPORT
        || (s_is_etws_need_popup && s_is_etws_alert_msg)
#endif
    )
    {
        //若收到的广播信息为 CLASS0 级且要立即显示则提取消息的有关信息
        //调用窗口立即显示有关内容
        //先关闭阅读窗口再打开保证了若此时在阅读窗口下也能刷新新的信息内容
        MMK_CloseWin(MMISMS_CB_CLASS0_READ_WIN_ID);
        //MMK_CreateWin((uint32*)MMISMS_CB_CLASS0_READ_WIN_TAB ,PNULL);
        SCI_TRACE_LOW("MMIAPISMSCB_HandleCBMsgInd CLASS0 message received");
        
#ifdef MMI_ETWS_SUPPORT
        if(!s_is_etws_alert_msg)
        {
            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
            MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
        }
        else
        {
            MMIAPISET_PlayRingById(TRUE, 1, 1, MMISET_RING_TYPE_ALARM, PNULL);
            MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, TRUE, MMI_VIBRATE_INTERVAL);
        }
#else
        MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
        MMISMS_SetNewSMSRingFlag(TRUE);
        MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
        if(s_is_alert_type_msg)
        {
            MMIAPISET_PlayRingById(TRUE, 1,1,MMISET_RING_TYPE_ALARM,PNULL);
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,TRUE,MMI_VIBRATE_INTERVAL);
	 }
#endif

        MMISMSCB_OpenCLASS0Win();
        MMISMSCB_DirectDisplayNewCBMsg(cb_msg_ptr);
    }
    else
    {
        SCI_TRACE_LOW("MMIAPISMSCB_HandleCBMsgInd normal message received");
        //若收到的广播信息为普通级且要保存到广播消息收件箱列表中去并显示提示窗口
        //添加新的广播消息内容到广播消息收件箱列表中去
        AddNewMsgToCBMsgList(cb_msg_ptr, FALSE);

        //若在小区广播收件箱列表窗口中则要刷新列表
        if (MMK_IsOpenWin(MMISMS_CB_MSG_LIST_WIN_ID))
        {
            uint16 cur_index = 0;
            SCI_TRACE_LOW("MMIAPISMSCB_HandleCBMsgInd normal message received  MMISMS_CB_MSG_LIST_WIN_ID is opened");
            cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);;

            //刷新当前小区广播收件箱列表
            GUILIST_RemoveAllItems(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
            MMISMS_UpdateCBMsgList(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

            if( (cur_index + 1 ) < MMI_MAX_CB_MSG_NUM )
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index + 1);
            else
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index );

            if (MMK_IsFocusWin(MMISMS_CB_MSG_LIST_WIN_ID))
            {
                MMK_SendMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }
		
#ifdef MMI_ETWS_SUPPORT
        if(!s_is_etws_alert_msg)
        {
            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
        }
        else
        {
            MMIAPISET_PlayRingById(TRUE, 1, 1, MMISET_RING_TYPE_ALARM, PNULL);
            MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, TRUE, MMI_VIBRATE_INTERVAL);
        }
#else
	MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
	MMISMS_SetNewSMSRingFlag(TRUE);  
#endif

        MMISMS_ShowNewSMSCBPrompt();
        MMIDEFAULT_TurnOnBackLight();
    }
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMIAPISMSCB_HandleMarcoCBMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleMarcoCBMsgInd(
									 DPARAM        param            //IN: 系统回调参数
									 )
{
    APP_MN_SMSCB_MACRO_MSG_IND_T *cb_msg_ptr = (APP_MN_SMSCB_MACRO_MSG_IND_T *)param;
    BOOLEAN cb_card_enabled = FALSE;
#ifdef MMI_ETWS_SUPPORT
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MMI_STRING_T prompt_content_str = {0};
    MMI_STRING_T prompt_str = {0};
    MMISMS_CB_MESSAGE_ITEM_T* cb_msg_item = {0};
    uint32 msg_real_len = 0;
    uint8 ring_vol=0;
    uint16 etws_alert_num = 0;
    wchar time_str[CB_MSG_TIME_MAX_LEN + 1] = {0};	
    uint16 time_len = 0;
    uint16 etws_context_len = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    uint16 etws_title_len = 0;
    MMI_STRING_T etws_title = {0};
    MMISMS_CB_LANGUAGE_T lang_list = {0};
    uint32 lang_index = 0;
#endif

    if (PNULL == cb_msg_ptr)
    {
        return;
    }
    //SCI_TRACE_LOW:"SMSCB: MMISMSCB_HandleMacroCBMsgInd dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2560_112_2_18_2_55_33_902,(uint8*)"d", cb_msg_ptr->dual_sys);
    SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd");

#ifdef MMI_ETWS_SUPPORT
    etws_alert_num = MMIAPISMSCB_GetETWSNum();

    if( etws_alert_num > 0)
    {
//        MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
        MN_CB_ALERT_MSG_T *cb_msg_id_ptr = PNULL;
        MMISMS_CB_CHANNEL_T etws_channel_list = {0};
		
        InitCB_ETWSChannel(cb_msg_ptr->dual_sys);
        SCI_MEMCPY(&etws_channel_list, &s_etws_channel_list, sizeof(MMISMS_CB_CHANNEL_T));
    }

    current_channel_info = GetCMASChannelInfo(cb_msg_ptr->dual_sys, cb_msg_ptr->msg_id);
	
    if( NULL != current_channel_info)
    {
        cb_msg_item = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
        if (PNULL == cb_msg_item)
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, allocate cb_msg_item buffer failed");
            return;
        }

        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, ETWS received");
        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, channel_id = %d, associated_channel = %d, language_id = %d", current_channel_info->channel_id , current_channel_info->associated_channel, cb_msg_ptr->dcs.language_id);

    // stop recording
#ifdef MMI_RECORD_SUPPORT
        if( MMIAPIRECORD_IsRecording() )
        {
            MMK_SendMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_APP_OK, PNULL);
        }
#endif
#ifdef CAMERA_SUPPORT
        if (MMK_IsOpenWin(MMIDC_MAIN_WIN_ID))
        {
            s_is_etws_display = TRUE;
        }
        MMIAPIDC_Interrupt();
#endif    

        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(cb_msg_ptr->dual_sys);
        
        if(MMIAPISMSCB_IsSetMcc())
        {
            plmn_info.mcc = MMIAPISMSCB_GetSetMcc();
        }

        if( CB_IT_MCC == plmn_info.mcc )
        {
            if( (4370 != current_channel_info->channel_id ) && (4370 != current_channel_info->associated_channel))
            {
                MMIAPISMSCB_NV_LoadSmsCBLanguageList(cb_msg_ptr->dual_sys, &lang_list);
                lang_index = MMISMSCB_ConvertLangIDToLangIndex(cb_msg_ptr->dcs.language_id);
                if( (FALSE ==lang_list.selected_arr[0]) &&
                     (FALSE == lang_list.selected_arr[lang_index] ) )
                {
                    return;
                }
            }
        }

        // add cb to msg list
        AddNewMarcoMsgToCBMsgList(cb_msg_ptr);

        // ignore secondary channel cb
        if( 1 == current_channel_info->primary_channel)
        {
            s_repeated_channel = current_channel_info->secondary_channel;
            s_repeated_period_time_id = MMK_CreateTimerCallback(MMI_5MINUTES,  MMISMSCB_HandleETWSRepeatedTimer, NULL, FALSE);
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, current_channel_info->primary_channel = %d", current_channel_info->primary_channel);
        }
        else  if(( 1 == current_channel_info->secondary_channel )  &&  (s_repeated_channel == cb_msg_ptr->msg_id)  )
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, current_channel_info->secondary_channel = %d, cb_msg_ptr->msg_id = %d", current_channel_info->secondary_channel, cb_msg_ptr->msg_id);
            return;
        }
        else
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, neither primary channel nor secondary channel");
        }

        // init popup string buffer
        etws_context_len = MMI_CB_CHANNEL_NAME_LEN + 1 \
			+CB_MSG_TIME_MAX_LEN + 1\
			+ MN_CB_MACRO_MSG_LEN + 1;
        etws_title_len = MMI_CB_CHANNEL_NAME_LEN + 1;

        prompt_content_str.wstr_ptr = SCI_ALLOCAZ(MN_CB_MACRO_MSG_LEN * sizeof(wchar));
        SCI_MEMSET(prompt_content_str.wstr_ptr , 0, MN_CB_MACRO_MSG_LEN * sizeof(wchar));	
        prompt_content_str.wstr_len = 0;

        prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len * sizeof(wchar));	
        prompt_str.wstr_len = 0;

        if( PNULL != etws_prompt_str.wstr_ptr )
        {
            SCI_FREE(etws_prompt_str.wstr_ptr);
            etws_prompt_str.wstr_ptr = PNULL;
        }
        etws_prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        SCI_MEMSET(etws_prompt_str.wstr_ptr , 0, etws_context_len * sizeof(wchar));	
        etws_prompt_str.wstr_len = 0;

        if( PNULL != etws_title_str.wstr_ptr )
        {
            SCI_FREE(etws_title_str.wstr_ptr);
            etws_title_str.wstr_ptr = PNULL;
        }
        etws_title_str.wstr_ptr = SCI_ALLOCAZ(etws_title_len * sizeof(wchar));
        SCI_MEMSET(etws_title_str.wstr_ptr , 0, etws_title_len* sizeof(wchar));
        etws_title_str.wstr_len = 0;

        // Title
        if( (CB_CHILE_MCC == plmn_info.mcc) || (CB_IT_MCC == plmn_info.mcc ) || (CB_PE_MCC == plmn_info.mcc) )
        {
            if( 0 != current_channel_info->popup_string_id )
            {
                MMI_GetLabelTextByLang(current_channel_info->popup_string_id , &etws_title);
                MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
                MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L" ");
                prompt_str.wstr_len = etws_title.wstr_len+1;
            }
            else if( 0 != current_channel_info->title_string_id )
            {
                MMI_GetLabelTextByLang(current_channel_info->title_string_id , &etws_title);
                MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
                MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L" ");
                prompt_str.wstr_len = etws_title.wstr_len+1;
            }
            else
            {
                MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, current_channel_info->title_id, MMIAPICOM_Wstrlen(current_channel_info->title_id)) ;
                MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L" ");
                prompt_str.wstr_len = MMIAPICOM_Wstrlen(current_channel_info->title_id) +1;
            }

            //Time
            SCI_MEMSET(time_str, 0, (CB_MSG_TIME_MAX_LEN + 1));
            time_len = ConvertCBTimeToStrForTitle(
                s_cb_msg.msg_ptr->date,
                s_cb_msg.msg_ptr->time,
                time_str
                );
            MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, time_str);
            prompt_str.wstr_len += time_len;
        }
        else
        {
            if( 0 != current_channel_info->title_string_id )
            {
                MMI_GetLabelTextByLang(current_channel_info->title_string_id , &etws_title);
                MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
                MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
                prompt_str.wstr_len = etws_title.wstr_len +1;
            }
            else
            {
            MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, current_channel_info->title_id, MMIAPICOM_Wstrlen(current_channel_info->title_id)) ;
                MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
            prompt_str.wstr_len = MMIAPICOM_Wstrlen(current_channel_info->title_id) +1;
            }
        }
		
        MMIAPICOM_Wstrncpy(etws_title_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_title_str.wstr_len = prompt_str.wstr_len;

        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));
        prompt_str.wstr_len = 0;

        // Time
        if( CB_NZ_MCC == plmn_info.mcc)
        {
            SCI_MEMSET(time_str, 0, (CB_MSG_TIME_MAX_LEN + 1));		
            time_len = ConvertCBTimeToStrForDetails(
				s_cb_msg.msg_ptr->date,
				s_cb_msg.msg_ptr->time,
				time_str
				);		
            MMI_WSTRNCPY(prompt_str.wstr_ptr+prompt_str.wstr_len,
				CB_MSG_TIME_MAX_LEN,
				time_str,
				time_len,
				time_len);
            prompt_str.wstr_len += time_len;
        }

        //Content	
        MMI_WSTRNCPY(prompt_str.wstr_ptr+prompt_str.wstr_len,
			MN_CB_MACRO_MSG_LEN,
			s_cb_msg.msg_ptr->context,
			s_cb_msg.msg_ptr->len,
			s_cb_msg.msg_ptr->len);

        prompt_str.wstr_len += s_cb_msg.msg_ptr->len;
        MMIAPICOM_Wstrncpy(etws_prompt_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_prompt_str.wstr_len = prompt_str.wstr_len;

        if (MMK_IsOpenWin(MMISMS_CB_MSG_LIST_WIN_ID))
        {
            uint16 cur_index = 0;

            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, MMISMS_CB_MSG_LIST_WIN_ID is open, update cb ");
            cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);;

            //刷新当前小区广播收件箱列表
            GUILIST_RemoveAllItems(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
            MMISMS_UpdateCBMsgList(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
			
            if( (cur_index + 1 ) < MMI_MAX_CB_MSG_NUM )
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index + 1);
            else
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index );

            if (MMK_IsFocusWin(MMISMS_CB_MSG_LIST_WIN_ID))
            {
                MMK_SendMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }

        // alert
        if( current_channel_info->alert_level > 0)
            s_is_etws_alert_msg = TRUE;

        if(current_channel_info->alert_tone_type)
        {
            s_is_etws_alert_msg = TRUE;
            s_is_etws_need_popup = TRUE;
        }

        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, vibrate_type=%d,mcc=%d ",current_channel_info->vibrate_type,plmn_info.mcc);

        s_etws_vibrate_type_context = current_channel_info->vibrate_type;
        // vibrate
        if(current_channel_info->vibrate_type)
        {
            // Display ETWS prompt
            MMIDEFAULT_TurnOnBackLight();

            s_etws_delay_show_dual_sim_context = cb_msg_ptr->dual_sys;

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

            if( MMICC_IsExistIncommingCall())
            {
                MMIAPICC_StopRingOrVibrate();
            }

            if( (CB_CHILE_MCC == plmn_info.mcc)
                ||(CB_NZ_MCC== plmn_info.mcc)
                ||(CB_PE_MCC== plmn_info.mcc)
                ||(CB_PWS_MCC== plmn_info.mcc))
            {
                s_is_need_enable = TRUE;
            }
            else
            {
                s_is_need_enable = FALSE;
            }

            if( (CB_PWS_MCC == plmn_info.mcc)
                || (CB_NZ_MCC== plmn_info.mcc))
            {
                s_is_personal = TRUE;
            }
            else
            {
                s_is_personal = FALSE;
            }

            if( CB_NZ_MCC == plmn_info.mcc)
            {
                s_is_need_torch = TRUE;
            }
            else
            {
                s_is_need_torch = FALSE;
            }
            s_etws_card_sys = cb_msg_ptr->dual_sys;

            MMIAPISMSCB_SetEMCallStatus(FALSE);
            MMIAPISMSCB_SetIsNeedRedisplay(FALSE);

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

            s_cur_display_cb_msg_index = 0;

            MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
            MMIAPISET_SetSOSVibrate(s_etws_card_sys , s_is_personal, s_is_need_torch);
			
            // stop vibration and alert after timeout
            switch(current_channel_info->alert_period)
            {
                case 5:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_5SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                    break;

                case 30:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_30SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                    break;
				
                case 180:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_3MINUTES,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                    break;

                case 240:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_4MINUTES,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                    break;

                case 10:
                default:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_10500MSECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                    break;
            }
        }
        else
        {
            if(CB_IL_MCC == plmn_info.mcc)
            {
                SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, receive israel normal alert");

                // Display ETWS prompt
                MMIDEFAULT_TurnOnBackLight();
                if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
                {
                    MMIAPISMSCB_StopETWSVibrateTimer();
                    MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
                }
                MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
                MMISMS_SetNewSMSRingFlag(TRUE);
                MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
                
                MMISMS_ShowNewSMSCBPrompt();
            }
            else
            {
                SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, normal ringtone and vibration");

                // Display ETWS prompt
                MMIDEFAULT_TurnOnBackLight();
                if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
                {
                    MMIAPISMSCB_StopETWSVibrateTimer();
                    MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
                }

                MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
                MMISMS_SetNewSMSRingFlag(TRUE);
                MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());

                MMIAPISMSCB_SetEMCallStatus(FALSE);
                MMIAPISMSCB_SetIsNeedRedisplay(FALSE);

                s_is_need_enable = TRUE;
                MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
            }
	}

	if( PNULL != prompt_content_str.wstr_ptr)
	{
		SCI_FREE(prompt_content_str.wstr_ptr);
		prompt_content_str.wstr_ptr = PNULL;
	}

	if( PNULL != prompt_str.wstr_ptr)
	{
		SCI_FREE(prompt_str.wstr_ptr);
		prompt_str.wstr_ptr= PNULL;
	}

        return ;
    }
#endif	
	
    cb_card_enabled = IsCBSwitchSelected(s_dual_card_sys);
    if(!cb_card_enabled)
    {
        SCI_TRACE_LOW("MMIAPISMSCB_HandleMarcoCBMsgInd cb_card_enabled return false.");
        return;
    }
	
    s_is_alert_type_msg = ((cb_msg_ptr->msg_code) >> 9) & 0x1; 
#ifdef MMI_ETWS_SUPPORT
    s_is_etws_alert_msg = ((cb_msg_ptr->msg_code) >> 9) & 0x01;
    s_is_etws_need_popup = ((cb_msg_ptr->msg_code) >> 8) & 0x01;
#endif

    SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd: is_etws_alert_msg=%d,need_popup=%d,class_is_present=%d,sms_class=%d,display_mode=%d",\
    s_is_etws_alert_msg,s_is_etws_need_popup,cb_msg_ptr->dcs.class_is_present,cb_msg_ptr->dcs.sms_class,cb_msg_ptr->display_mode);
    if (((1 == cb_msg_ptr->dcs.class_is_present)
        && (MN_SMS_CLASS_0 == cb_msg_ptr->dcs.sms_class)
#ifdef MMI_ETWS_SUPPORT
        && (!s_is_etws_alert_msg)
#endif
        )
        || ((MN_CB_DISPLAY_IMMEDIATE == cb_msg_ptr->display_mode)
#ifdef MMI_ETWS_SUPPORT
        && (!s_is_etws_alert_msg)
#endif
        )
#ifdef MMI_ETWS_SUPPORT
        || (s_is_etws_need_popup && s_is_etws_alert_msg)
#endif
		)
    {
        //若收到的广播信息为 CLASS0 级且要立即显示则提取消息的有关信息
        //调用窗口立即显示有关内容
        //先关闭阅读窗口再打开保证了若此时在阅读窗口下也能刷新新的信息内容
        MMK_CloseWin(MMISMS_CB_CLASS0_READ_WIN_ID);
        //MMK_CreateWin((uint32*)MMISMS_CB_CLASS0_READ_WIN_TAB ,PNULL);
        SCI_TRACE_LOW("MMIAPISMSCB_HandleMarcoCBMsgInd CLASS0 message received");
#ifdef MMI_ETWS_SUPPORT
        if(!s_is_etws_alert_msg)
        {
            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
            MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
        }
        else
        {
            MMIAPISET_PlayRingById(TRUE, 1, 1, MMISET_RING_TYPE_ALARM, PNULL);
            MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, TRUE, MMI_VIBRATE_INTERVAL);
        }
#else
        MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
        MMISMS_SetNewSMSRingFlag(TRUE);
        MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
        if(s_is_alert_type_msg)
        {
            MMIAPISET_PlayRingById(TRUE, 1,1,MMISET_RING_TYPE_ALARM,PNULL);
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,TRUE,MMI_VIBRATE_INTERVAL);
	 }
#endif
 
        MMISMSCB_OpenCLASS0Win();
        MMISMSCB_DirectDisplayNewMarcoCBMsg(cb_msg_ptr);
    }
    else
    {
        SCI_TRACE_LOW("MMIAPISMSCB_HandleMarcoCBMsgInd normal message received");

        //若收到的广播信息为普通级且要保存到广播消息收件箱列表中去并显示提示窗口
        //添加新的广播消息内容到广播消息收件箱列表中去
        AddNewMarcoMsgToCBMsgList(cb_msg_ptr);

        //若在小区广播收件箱列表窗口中则要刷新列表
        if (MMK_IsOpenWin(MMISMS_CB_MSG_LIST_WIN_ID))
        {
            uint16 cur_index = 0;
            SCI_TRACE_LOW("MMIAPISMSCB_HandleMarcoCBMsgInd normal message received  MMISMS_CB_MSG_LIST_WIN_ID is opened");
            cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);;

            //刷新当前小区广播收件箱列表
            GUILIST_RemoveAllItems(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
            MMISMS_UpdateCBMsgList(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

            if( (cur_index + 1 ) < MMI_MAX_CB_MSG_NUM )
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index + 1);
            else
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index );
	
            if (MMK_IsFocusWin(MMISMS_CB_MSG_LIST_WIN_ID))
            {
                MMK_SendMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }

#ifdef MMI_ETWS_SUPPORT
	if(!s_is_etws_alert_msg)
	{
		MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
		MMISMS_SetNewSMSRingFlag(TRUE);
	}
	else
	{
		MMIAPISET_PlayRingById(TRUE, 1, 1, MMISET_RING_TYPE_ALARM, PNULL);
		MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, TRUE, MMI_VIBRATE_INTERVAL);
	}
#else
	MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
	MMISMS_SetNewSMSRingFlag(TRUE);
#endif

        MMISMS_ShowNewSMSCBPrompt();
        MMIDEFAULT_TurnOnBackLight();
    }
}

/*****************************************************************************/
//  Description : set setting card
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_SetSelectCard(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        s_dual_card_sys = dual_sys;
    }
    else
    {
        s_dual_card_sys = MN_DUAL_SYS_1;
    }
}

/*****************************************************************************/
//  Description : get setting card
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E  MMISMSCB_GetSelectCard( void )
{
    return s_dual_card_sys;
}

/***************************************************************************/
//     Description : 初始化小区广播接口函数(在手机初始化时调用)
//    Global resource dependence : none
//  Author:肖凡
//    Note:
/***************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMIAPISMSCB_Init(MN_DUAL_SYS_E dual_sys)
{
    return MMISMSCB_Init(dual_sys);
}

/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenMainMenu(void)
{
    MMISMSCB_OpenMainMenu();
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_RestoreFactorySetting(void)
{
    MMISMS_CB_CONFIG_T default_cb_config = CB_CONFIG_OFF;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    default_cb_config = MMISMSCB_GetCBDefaultConfig();

    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MN_DUAL_SYS_MAX; dual_sys++)
    {
        //save important channel
        MMISMSCB_SaveCBEmergenceChannel(dual_sys);
       SCI_TRACE_LOW("MMIAPISMSCB_RestoreFactorySetting --> dual_sys = %d, default_cb_config = %d.", dual_sys, default_cb_config);
        if(CB_CONFIG_ON == default_cb_config)
        {
            MMISMSCB_SwitchSmsCBStatus(TRUE);
        }
        else
       {
            MMISMSCB_SwitchSmsCBStatus(FALSE); //恢复出厂设置，关闭小区广播。    
        }
    }
}

#else /* MMI_ETWS_SUPPORT */
/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_RestoreFactorySetting(void)
{
    // added by Tong Yongli
    BOOLEAN is_default_enable_cb = FALSE;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    is_default_enable_cb = MMISMSCB_IsCBDefaultEnable();

    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MN_DUAL_SYS_MAX; dual_sys++)
    {
        //save important channel
        MMISMSCB_SaveCBEmergenceChannel(dual_sys);
       SCI_TRACE_LOW("MMIAPISMSCB_RestoreFactorySetting --> dual_sys = %d, is_default_enable_cb = %d.", dual_sys, is_default_enable_cb);
        if(is_default_enable_cb)
        {
            MMISMSCB_SwitchSmsCBStatus(TRUE);
        }
        else
       {
            MMISMSCB_SwitchSmsCBStatus(FALSE); //恢复出厂设置，关闭小区广播。    
        }
    }
}
#endif /* MMI_ETWS_SUPPORT */

/*****************************************************************************/
//  Description : open SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenSettingWin(void)
{
    MMISMSCB_OpenSettingWin();
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_OpenCBList
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_OpenCBList(void)
{
    MMISMSCB_OpenCBList();

    return TRUE;
}

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
/*****************************************************************************/
//     Description : MMIAPISMSCB_GetCellInfoStr
//    Global resource dependence : 
//  Author: Yongli Tong
//    Note: 
/*****************************************************************************/
PUBLIC uint8 * MMIAPISMSCB_GetCellInfoStr(void)
{
    return s_cell_info_idle_str;
}
#endif

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : MMIAPISMSCB_GetETWSAlertFlag
//    Global resource dependence : 
//  Author: Yongli Tong
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetETWSAlertFlag(void)
{
    return s_is_etws_alert_msg;
}
#endif

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : Set SMS CB Status
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SwitchSmsCBStatus(BOOLEAN cb_enabled)
{
    MMISMSCB_SwitchSmsCBStatus(cb_enabled);
}
#endif

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : to show the new SMSCB prompt
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_ShowNewETWSPrompt(void)
{
    // prompt
    if(  TRUE == s_is_need_redisplay )
    {
        MMIAPISMSCB_SetEMCallStatus(FALSE);
        MMIAPISMSCB_SetIsNeedRedisplay(FALSE);
        if( FALSE == MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
        {
            MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
        }
        // ring and vibrate
        if( (TRUE == MMIAPISMSCB_IsETWSAlert() )
            && ( FALSE == MMIAPISET_IsSOSVibrate()) )
        {
            MMIAPISET_SetSOSVibrate(s_etws_card_sys , s_is_personal, s_is_need_torch);
        }
    }
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_SetEMCallStatus
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetEMCallStatus(BOOLEAN is_emer_call)
{
    s_is_emer_call = is_emer_call;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetEMCallStatus
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetEMCallStatus(void)
{
    return s_is_emer_call;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedEnable
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetIsNeedEnable(void)
{
    return s_is_need_enable;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedEnable
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetIsNeedEnable(BOOLEAN is_need_enable)
{
     s_is_need_enable = is_need_enable;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedRedisplay
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetIsNeedRedisplay(void)
{
    return s_is_need_redisplay;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedEnable
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetIsNeedRedisplay(BOOLEAN is_need_redisplay)
{
     s_is_need_redisplay = is_need_redisplay;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsETWSDisplay
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetIsETWSDisplay(void)
{
    SCI_TRACE_LOW("MMIAPISMSCB_GetIsETWSDisplay, s_is_etws_display = %d", s_is_etws_display);
    return s_is_etws_display;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_SetIsETWSDisplay
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetIsETWSDisplay(BOOLEAN is_etws_display)
{
     s_is_etws_display = is_etws_display;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetCurDispCBIndex
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetCurDispCBIndex(void)
{
    SCI_TRACE_LOW("MMIAPISMSCB_GetCurDispIndex, s_cur_display_cb_msg_index = %d", s_cur_display_cb_msg_index);
    return s_cur_display_cb_msg_index;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_SetCurDispCBIndex
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetCurDispCBIndex(uint16 index)
{
     s_cur_display_cb_msg_index = index;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetCurDispCBIndex
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetSimIndex(void)
{
    SCI_TRACE_LOW("MMIAPISMSCB_GetSimIndex, s_etws_card_sys = %d", s_etws_card_sys);
    return s_etws_card_sys;
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_Get_ETWS_Channel_Ptr
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:         
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CHANNEL_T *MMISMSCB_Get_ETWS_Channel_Ptr( MN_DUAL_SYS_E dual_sys )
{
    return &s_etws_channel_list[dual_sys];
}

#endif

#endif//#if defined(MMI_SMSCB_SUPPORT)

