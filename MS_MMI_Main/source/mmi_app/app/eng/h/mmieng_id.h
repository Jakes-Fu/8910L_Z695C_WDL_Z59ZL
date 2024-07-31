/*****************************************************************************
** File Name:      mmieng_id.h                                                *
** Author:                                                                   *
** Date:           2006/05/17                                                *
** Copyright:      2006 Spreadrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe eng win and control id    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/10/09     songbin.zeng      Create                                   *
******************************************************************************/

#ifndef _MMIENG_ID_H_
#define _MMIENG_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

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
// window ID
typedef enum
{
    MMIENG_WIN_ID_START = (MMI_MODULE_ENG << 16),

#include "mmieng_id.def"
#ifdef GPS_SUPPORT
    MMIENG_UITESTGPS_WIN_ID,
	MMIENG_SHOW_GPS_WIN_ID,
#endif
    MMIENG_DTMF_SETTIMER_WIN_ID,
    MMIENG_MAX_WIN_ID
}MMIENG_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIENG_CTRL_ID_ID_START = MMIENG_MAX_WIN_ID, //(MMI_MODULE_ENG << 16),
    MMIENG_MAINMENU_CTRL_ID,
    MMIENG_FORM_CTRL_ID,
    MMIENG_FORM1_CTRL_ID,
    MMIENG_FORM2_CTRL_ID,
    MMIENG_UITEST_FORM1_CTRL_ID,
    MMIENG_UITEST_FORM2_CTRL_ID,
    MMIENG_UITEST_MENU_CTRL_ID,		//MS00193068
    MMIENG_UITEST_TEXT_CTRL_ID,
    MMIENG_UITEST_TEXTBOX_CTRL_ID,
    MMIENG_UITEST_LISTBOX_CTRL_ID,
    MMIENG_UITEST_EDITBOX_CTRL_ID,
    MMIENG_UITEST_LABEL_CTRL_ID,
#ifdef ATV_SUPPORT
    MMIENG_UITEST_LABEL1_CTRL_ID,
    MMIENG_UITEST_LABEL2_CTRL_ID,
    MMIENG_UITEST_LABEL3_CTRL_ID,
    MMIENG_UITEST_LABEL4_CTRL_ID,
    MMIENG_UITEST_LABEL5_CTRL_ID,
#endif
    MMIENG_TEXTBOX1_CTRL_ID,
    MMIENG_TEXTBOX2_CTRL_ID,
    MMIENG_TEXTBOX3_CTRL_ID,
    MMIENG_LABEL1_CTRL_ID,
    MMIENG_LABEL2_CTRL_ID,
    MMIENG_LABEL3_CTRL_ID,
    MMIENG_LABEL4_CTRL_ID,
    MMIENG_LABEL5_CTRL_ID,
    MMIENG_LABEL6_CTRL_ID,
    MMIENG_LABEL7_CTRL_ID,
    MMIENG_LABEL8_CTRL_ID,
    MMIENG_LABEL9_CTRL_ID,
    MMIENG_LABEL10_CTRL_ID,
    MMIENG_LABEL11_CTRL_ID,
    MMIENG_LABEL12_CTRL_ID,
    MMIENG_EDITBOX1_CTRL_ID,
    MMIENG_EDITBOX2_CTRL_ID,
    MMIENG_COMMON_CTRL1_ID,
    MMIENG_COMMON_CTRL2_ID,
    MMIENG_COMMON_CTRL3_ID,
    MMIENG_COMMON_CTRL4_ID,
    MMIENG_COMMON_CTRL5_ID,
    MMIENG_COMMON_CTRL6_ID,
    MMIENG_COMMON_CTRL7_ID,
    MMIENG_COMMON_CTRL8_ID,
    MMIENG_COMMON_CTRL9_ID,
    MMIENG_COMMON_CTRL10_ID,
    MMIENG_COMMON_CTRL11_ID,

    MMIENG_COMMON_LIST_CTRL1_ID,
    MMIENG_COMMON_LABEL_CTRL1_ID,
    MMIENG_COMMON_LABEL_CTRL2_ID,
    MMIENG_COMMON_LABEL_CTRL3_ID,
    MMIENG_COMMON_LABEL_CTRL4_ID,
    MMIENG_COMMON_LABEL_CTRL5_ID,
    MMIENG_COMMON_LABEL_CTRL6_ID,
    MMIENG_COMMON_LABEL_CTRL7_ID,
    MMIENG_COMMON_LABEL_CTRL8_ID,
    MMIENG_COMMON_LABEL_CTRL9_ID,
    MMIENG_COMMON_LABEL_CTRL10_ID,
    MMIENG_COMMON_LABEL_CTRL11_ID,
    MMIENG_COMMON_LABEL_CTRL12_ID,
    MMIENG_COMMON_LABEL_CTRL13_ID,
    MMIENG_COMMON_LABEL_CTRL14_ID,
        
    MMIENG_COMMON_EDIT_CTRL1_ID,
    MMIENG_COMMON_EDIT_CTRL2_ID,
    MMIENG_COMMON_EDIT_CTRL3_ID,
    MMIENG_COMMON_EDIT_CTRL4_ID,
    MMIENG_COMMON_EDIT_CTRL5_ID,
    MMIENG_COMMON_EDIT_CTRL6_ID,
    MMIENG_COMMON_EDIT_CTRL7_ID,
    MMIENG_COMMON_EDIT_CTRL8_ID,
    MMIENG_COMMON_EDIT_CTRL9_ID,
    MMIENG_COMMON_EDIT_CTRL10_ID,
    MMIENG_COMMON_EDIT_CTRL11_ID,
#ifdef ENG_SUPPORT
    MMIENG_TESTSIM_CTRL_ID,
    MMIENG_PRGBOX_CTRL_ID,
    MMIENG_LISTBOX_CTRL_ID,
	NPS_TIME_CHANGE_FORM_CTRL_ID,
	NPS_TIME_CHANGE1_FORM_CTRL_ID,
	NPS_TIME_CHANGE2_FORM_CTRL_ID,
	MMIENG_NPS_TIME_LABEL_CTRL_ID,
	MMIENG_NPS_TIME_EDIT1_CTRL_ID,
	MMIENG_NPS_TIME_EDIT2_CTRL_ID,
	MMIENG_NPS_TIME_TEXT_CTRL_ID,
    MMIENG_MP3_LISTBOX_CTRL_ID,
    MMIENG_UAAGENT_CTRL_ID,
    MMIENG_UAPROFILE_CTRL_ID,
    MMIENG_WAPUA_CTRL_ID,
    MMIENG_BTADDRESS_CTRL_ID,
#ifndef WIN32
    MMIENG_BTVER_CTRL_ID,
#endif
    MMIENG_WAPUAPROFILE_CTRL_ID,
    MMIENG_CHIP_TEST_MENU_CTRL_ID,
    MMIENG_CHIP_TEST_SETTING_CTRL_ID,
#if defined ENG_BAND_FREQ_SUPPORT //zhouqin
    MMIENG_BAND_LISTBOX_CTRL_ID,
    MMIENG_FREQ_LISTBOX_CTRL_ID,
    MMIENG_FREQ_EDIT_CTRL_ID,
#endif
#endif
#ifdef CMMB_SUPPORT
    MMIENG_CMMB_TEST_MENU_CTRL_ID,
    MMIENG_CMMB_RD_TEST_MENU_CTRL_ID,
    MMIENG_CMMB_FACTORY_TEST_MENU_CTRL_ID,
    MMIENG_CMMB_FACTORY_HAVE_SIGNAL_MENU_CTRL_ID,
    MMIENG_CMMB_FACTORY_NO_SIGNAL_MENU_CTRL_ID,
#endif
    //MMIENG_MMS_SETNET_CTRL_ID,
#ifdef ENG_SUPPORT
    MMIENG_IQ_MODE_CTRL_ID,
    MMIENG_IQ_DATA_CTRL_ID,	
    MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID,
    MMIENG_IQ_DATA_MODE_SETTING_CTRL_ID,
    MMIENG_IQ_DATA_MODE_SETTING_FORMAT_CTRL_ID,
    MMIENG_IQ_DATA_MODE_SETTING_TIME_LIMIT_CTRL_ID,
    MMIENG_IQ_DATA_MODE_SETTING_FRAME_LENGTH_CTRL_ID,
    MMIENG_IQ_DATA_MODE_SETTING_BLOCK_COUNT_CTRL_ID,
/*[START] Jerry Liang for IQ MODE 2008/08/01*/        
    MMIENG_IQ_DATA_MODE_SETTING_ARFCN_CTRL_ID,
/*[END] Jerry Liang for IQ MODE 2008/08/01*/            
    MMIENG_IQ_DATA_MODE_SETTING_CHANNEL_CTRL_ID,
#endif
    MMIENG_SHOW_VERSION_CTRL_ID,
    MMIENG_SHOW_WRE_VERSION_CTRL_ID,
    MMIENG_3RDPARTY_VERSION_CTRL_ID,
#ifdef PCLINK_SUPPORT
	MMIENG_PCLINK_ENABLE_CTRL_ID,
	MMIENG_PCLINK_INPUT_IPADDR_CTRL_ID,
	MMIENG_PCLINK_INPUT_SNMASK_CTRL_ID,
	MMIENG_PCLINK_INPUT_DEFGW_CTRL_ID,
	MMIENG_PCLINK_INPUT_DNS_CTRL_ID,
	MMIENG_PCLINK_INPUT_TEST_ADDRESS_CTRL_ID,
#endif
#ifdef CMMB_SUPPORT
	MMIENG_CMMB_TEXT_CTRL_ID,
    MMIENG_CMMB_SN_TEXT_CTRL_ID,
    MMIENG_CMMB_LABEL1_CTRL_ID,
    MMIENG_CMMB_LABEL2_CTRL_ID,
    MMIENG_CMMB_LABEL3_CTRL_ID,
    MMIENG_CMMB_LABEL4_CTRL_ID,
    MMIENG_CMMB_LABEL5_CTRL_ID,
    MMIENG_CMMB_LABEL6_CTRL_ID,
#endif
#ifdef ENG_SUPPORT
    MMIENG_GBA_ENABLE_TEST_CTRL_ID,
    MMIENG_LOG_ENABLE_CONTROL_CTRL_ID,
    MMIENG_MSK_ENABLE_TEST_CTRL_ID,
    MMIENG_GBA_AND_MSK_TEST_CTRL_ID,    
    MMIENG_SMD_COUNTER_ENABLE_CTRL_ID,
    MMIENG_AUDIO_SERVICE_ENABLE_CTRL_ID,
    MMIENG_UA_SELECT_LISTBOX_CTRL_ID,
    MMIENG_PROFILE_SELECT_LISTBOX_CTRL_ID,
    MMIENG_UA_SELECT_LIST_NUM_CTRL_ID,
    MMIENG_PROFILE_SELECT_LIST_NUM_CTRL_ID,

	MMIENG_NV_COUNTING_TEXTBOX_CTRL_ID,
	//gmw---begin
	MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,
	//gmw---end
    MMIENG_UA_SUB_MENU_CTRL_ID,
    MMIENG_OPTION_MENU_CTRL_ID,
#endif    
#ifdef WIFI_SUPPORT
    /*BEGIN WLAN */
    MMIENG_RADIO_FREQUENCY_FORM_CTRL_ID,
    MMIENG_WLAN_POWER_SAVE_SETLIST_CTRL_ID,
    MMIENG_WLAN_POWER_SAVE_FORM_CTRL_ID,
    MMIENG_WLAN_POWER_SAVE_LABEL_CTRL_ID,
    MMIENG_WLAN_ROAMING_SETLIST_CTRL_ID,
    MMIENG_WLAN_ROAMING_FORM_CTRL_ID,
    MMIENG_WLAN_ROAMING_LABEL_CTRL_ID,
    /* END WLAN */
#endif
    
#ifdef MEDIA_DEMO_SUPPORT
    MMIENG_MEDIA_DEMO_CTRL_ID, 
    MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,
    MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, 
#endif
#ifdef ENG_SUPPORT
  MMIENG_VOICE_PARAM_SOFTKEY_CTRL_ID,
#ifdef MMI_PDA_SUPPORT
  MMIENG_SIDETONE_FORM_CTRL_ID,
  MMIENG_SIDETONE_SOFTKEY_CTRL_ID,
#endif
#endif
#ifdef TP_TEST_SUPPORT
  TP_GET_BUTTON_CTRL_ID,
  TP_SET_BUTTON_CTRL_ID,
  TP_TEST_BUTTON_CTRL_ID,
  TP_WIDTH_EDITBOX_CTRL_ID,
  TP_HEIGHT_EDITBOX_CTRL_ID,
  REFERENCE_RESISTOR_EDITBOX_CTRL_ID,
  REFERENCE_VOLTAGE_EDITBOX_CTRL_ID,
  DPOINT_XLIMIT_EDITBOX_CTRL_ID,
  DPOINT_YLIMIT_EDITBOX_CTRL_ID,
  SPOINT_RT_MAX_EDITBOX_CTRL_ID,
  SPOINT_RT_MIN_EDITBOX_CTRL_ID,
  DPOINT_RT_MAX_EDITBOX_CTRL_ID,
  DPOINT_RT_MIN_EDITBOX_CTRL_ID,
  DOUBLE_POINT_EDITBOX_CTRL_ID,
  RESERVED_EDITBOX_CTRL_ID,
#endif
  MMIENG_DTMF_EDIT_CTRL_ID,

#ifdef MMI_UNISOC_MANUALLY_CIT_SUPPORT
  UNISOC_TEST_CAMERA_ANIM_CTRL_ID,
#endif
#if defined ENG_NETWORK_SET //zhouqin
MMIENG_NETWORK_CTRL_ID,
#endif
#if defined ENG_BAND_FREQ_SUPPORT //zhouqin
MMIENG_BAND_FREQ_CTRL_ID,
MMIENG_SET_BAND_CTRL_ID,
#endif
MMIENG_NET_INFO_SHOW_CTRL_ID,
MMIENG_NETINFO_STATISTICS_CTRL_ID,

    MMIENG_FORM_NAME_ID,
    MMIENG_FORM_ICON_ID,
    MMIENG_DROP_LIST_ICON_ID,
    MMIENG_FORM_PHONENUM_ID,
MMIENG_SHOW_PHONE_FIRSTTIME_CTRL_ID,
MMIENG_AGINGTEST_MENU_CTRL_ID,
    MMIENG_MAX_CTRL_ID
}MMIENG_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
    /*****************************************************************************/
    //  Description : Register acc menu group
    //  Global resource dependence : none
    //  Author: haiyang.hu
    //  Note:
    /*****************************************************************************/
    PUBLIC void MMIENG_RegWinIdNameArr(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
