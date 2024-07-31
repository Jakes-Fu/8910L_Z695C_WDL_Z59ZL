/*************************************************************************
 ** File Name:      mmi_nv.c                                             *
 ** Author:         Great.Tian                                           *
 ** Date:           01/28/2004                                           *
 ** Copyright:      2001 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 01/28/2004     Panda luo        Create.                              *
*************************************************************************/
#define _MMI_NV_C

//#include "std_header.h"
#include "mmi_nv.h"
#include "mmimms_export.h"
#include "mmiconnection_export.h"
#if defined(MMI_MSD_SUPPORT)
#include "mmimsd_export.h"
#endif
#include "mmi_modu_main.h"
#include "mmi_module.h"
//#include "mmiidle_screensaver.h"
#include "mmi_wallpaper_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipdp_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef MMI_AZAN_SUPPORT
#include "mmiazan_export.h"
#endif
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif

#if defined(MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
#include "mmi_mainmenu_istyle.h"
#endif
#include "mmiphone_export.h"
#include "mmiidle_export.h"
#include "mmisrv_cameraroll_export.h"

//#define MMI_NVITEM_NUM  (MMI_NV_MAX_NUM - MMI_UINV_USER_BASE)   // the number of the mmi nv item

const uint16 mmi_nv_len[] =
{
    //imsi
    sizeof(MN_IMSI_T)*MMI_DUAL_SYS_MAX,              //MMINV_PHONE_IMSI
    sizeof(MMIPHONE_NV_NETWORK_NAME_T)*MMI_DUAL_SYS_MAX,              //MMINV_NETWORK_NAME
#ifdef MMS_SUPPORT
    //mms
    sizeof(MMIMMS_NV_SETTING_T),              //MMINV_MMS_SETTING
#endif

//    sizeof(BOOLEAN),                        //MMINV_MMS_IS_DELETE_DONE, (boolean)


    //browser nv item start add by figo.feng 2004.09.14  
//    sizeof(MMICMSBRW_BOOKMARK_T),
//    sizeof(MMICMSBRW_SETTING_T),
//    1,
//    1,
#ifdef BROWSER_SUPPORT
    sizeof(MMIBROWSER_UA_T),
#endif
#ifdef MMI_GPRS_SUPPORT
    sizeof(MMICONNECTION_GPRS_SETTINGS_E),
    MMI_DUAL_SYS_MAX * sizeof(MMIPDP_FLOWRATE_T),
    //browser nv item end
    
    //connection start
    sizeof(MMICONNECTION_SETTING_T),
    sizeof(MMICONNECTION_SETTING_T),
    sizeof(MMICONNECTION_SETTING_T),
#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
    sizeof(PHONE_PLMN_NAME_E)*MMI_DUAL_SYS_MAX,//MMINV_CONNECTION_CONNECTION_PLMN
    sizeof(MN_PLMN_T)*MMI_DUAL_SYS_MAX,//MMINV_CARRIER_PLMN replace MMINV_CONNECTION_CONNECTION_PLMN
    //sizeof(MMIPHONE_SIM_SPN_INFO_T)*MMI_DUAL_SYS_MAX,//MMINV_CARRIER_SPN 
    sizeof(MMICONNECTION_APNTYPEINDEX_T),//MMINV_SET_APN_TYPE_INDEX
#endif
    //connection end
    //added by feng.xiao
    #ifdef DATA_ROAMING_SUPPORT
        sizeof(MMICONNECTION_SETTING_DATA_ROAMING_E),//MMINV_CONNECTION_DATA_ROAMING
    #endif
#ifdef MMI_GPRS_SWITCH_SUPPORT
        sizeof(MMICONNECTION_SETTING_GPRS_SWITCH_E),
#endif
	 sizeof(MMISET_VODAFONE_SETTINGS_T),
#endif
#if defined(MMI_MSD_SUPPORT)
    //MSD START
    sizeof(MMIMSD_NV_INFO_T),
    //MSD END   
#endif
    sizeof(MMISET_DIRECTIONKEY_SHORTCUT_T),  //���������
    sizeof(MMINV_LEAVEWORD_ITEM_INFO_T),    //����������Ϣ
    
#if 0
    1,  //MMINV_MPEG4_PLAY_PARAM
    1,  //MMINV_MPEG4_VOLUME
    1,  //MMINV_MPEG4_BRIGHT
#endif
    
    sizeof(MMIIDLE_WALLPAPER_WIDTH_HEIGHT_T),//MMINV_WALLPAPER_BITMAP_INFO

    // begin main menu nv data 

#ifdef MATRIX_MAIN_MENU_SUPPORT
    // �������˵�
    2,                                                    // MMINV_ICONMENU_ITEM_NUM    �������˵�����
    MMIMENU_MAIN_MAX_NUM*sizeof(MMIMAINMENU_ITEM_INFO_T), // MMINV_ICONMENU_ITEM_INFO   �������˵�����
#endif
#ifdef OPTION_PAGE_MAIN_MENU_SUPPORT
    // ѡ����˵�
    2,                                                    // MMINV_OPTMENU_1_ITEM_NUM   ѡ���1ҳ����
    MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM*sizeof(MMIMAINMENU_ITEM_INFO_T), // MMINV_OPTMENU_1_ITEM_INFO  ѡ���һҳ����
    2,                                                    // MMINV_OPTMENU_2_ITEM_NUM   ѡ���2ҳ����
    MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM*sizeof(MMIMAINMENU_ITEM_INFO_T), // MMINV_OPTMENU_2_ITEM_INFO  ѡ��ڶ�ҳ����
    2,                                                    // MMINV_OPTMENU_3_ITEM_NUM   ѡ���3ҳ����
    MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM*sizeof(MMIMAINMENU_ITEM_INFO_T), // MMINV_OPTMENU_3_ITEM_INFO  ѡ�����ҳ����
    2,                                                    // MMINV_OPTMENU_4_ITEM_NUM   ѡ���4ҳ����
    MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM*sizeof(MMIMAINMENU_ITEM_INFO_T), // MMINV_OPTMENU_4_ITEM_INFO  ѡ�����ҳ����
#endif

    // ����ҳ�����˵�
#if (defined QBTHEME_SUPPORT) || (defined MMI_SAM_THEME)
    2,                                                                      // MMINV_SLIDEMENU_CUR_PAGEINDEX,��ǰҳ
    2,                                                                      // MMINV_SLIDEMENU_VALID_PAGENUM
#endif

#if defined(MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
//size
    //istyle
    sizeof(ISTYLE_PAGES_INFO_T),//MMINV_MAINMENU_ISTYLE_PAGES
    sizeof(ISTYLE_BAR_INFO_T),//MMINV_MAINMENU_ISTYLE_BAR
    sizeof(ISTYLE_FOLDERS_INFO_T),//MMINV_MAINMENU_ISTYLE_FOLDERS
#endif

#ifdef QBTHEME_SUPPORT
    sizeof(MMIMAINMENU_PAGE_ITEM_INFO_T),               // MMINV_QBTHEME_DOCKBAR_INFO
#endif
    // end main menu nv data, �벻Ҫ����֮�������������
#ifdef MMI_AZAN_SUPPORT
	sizeof(AZAN_ALARM_T), //MMINV_AZAN_ALARM
	sizeof(uint16),       //MMINV_AZAN_COUNTRY_ID,    // Country id
	sizeof(uint16),       //MMINV_AZAN_CITY_ID,       // City id
	sizeof(uint16),        //MMINV_AZAN_CAL_METHOD_ID, // Calculation method
	sizeof(BOOLEAN),      //MMINV_AZAN_STATUS_ID
#endif
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    sizeof(MN_PLMN_T)*MMI_DUAL_SYS_MAX, //MMINV_HPLMN_INFO
#endif
#ifdef MMI_RESET_PHONE_AUTO_PIN
    sizeof(MN_BLOCK_T) * MMI_DUAL_SYS_MAX, // MMINV_SIM_PIN_VALUE,
#endif

    sizeof(MMITHEME_ACCENT_COLOR_E), //MMI_NV_THEME_ACCENT_ID
    sizeof(BOOLEAN),//[MS] MMINV_PHONE_FIRST_POWER_ON
    sizeof(MMIPHONE_HANDLE_VODAFONE_SELECT_T),   //MMINV_PHONE_FIRST_VODAFONE_SIM
    sizeof(MMI_RECORD_INSERT_SIM_CARD_T)*MMI_DUAL_SYS_MAX,//MMI_NV_INSERT_SIM_TIME
#ifdef UNISOC_MMI_SIM_LOCK_SUPPORT
    sizeof(MMIPHONE_SIM_LOCK_INFO_T),//MMI_NV_SIM_LOCK_INFO_ID
#endif
    MMITHEME_MENUMAIN_TOTAL_ITEM_MAX_NUM*sizeof(uint32),
    sizeof(BOOLEAN),	
    sizeof(IDLE_PUSH_MSG_NV_T),	
    sizeof(uint8),
    sizeof(MMIFILE_DEVICE_E),//dong.chunguang add for prefer disk MMINV_SETTING_PREFER_DISK
    sizeof(BOOLEAN),
    sizeof(ALL_NETWORK_INFO_NAME_T),  
    sizeof(MMISRV_CAMERAROLL_ONEDRIVEINFO_T),
#ifdef MMI_GPRS_SUPPORT//dong.chunguang add for apn requirement
    sizeof(uint8),//MMINV_CONNECTION_SIM1_APN_DEFAULT
    sizeof(uint8),//MMINV_CONNECTION_SIM2_APN_DEFAULT
    sizeof(uint8),//MMINV_CONNECTION_VODAFONE1_APN_DEFAULT
    sizeof(uint8),//MMINV_CONNECTION_VODAFONE2_APN_DEFAULT
#endif
#ifdef MMI_BT_PBAP_SUPPORT
	sizeof(BT_ADDRESS),        //MMINV_BT_ADDRESS_INFO,    //connected device address
#endif
    sizeof(MMICONNECTION_MVNOINDEX_T),//MMINV_CONNECTION_MVNO_CHECK
    // PHONEBOOK
};

/*****************************************************************************/
//  Description : register common module nv len and  max item
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMI_RegCommonNv(void)
{    
    MMI_RegModuleNv(MMI_MODULE_COMMON, mmi_nv_len, sizeof(mmi_nv_len)/sizeof(uint16));
}
