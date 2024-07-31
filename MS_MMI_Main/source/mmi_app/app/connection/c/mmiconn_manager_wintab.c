
#define _MMICONNECT_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_connection_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"

#include "mmi_default.h"
#include "mmidisplay_data.h"
#include "mmiconnection_id.h"
#include "window_parse.h"
#include "mmiacc.h"
#include "mmi_menutable.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmicom_string.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiidle_subwintab.h"
#include "mmiphone_export.h"
#include "mmiconnection_menutable.h"
#include "mmiconnection_text.h"
#include "mmiconnection_image.h"
#include "mmiconnection_id.h"
#include "mmitv_out.h"
#include "mmiconnection_export.h"
#include "mmibt_export.h" // baokun add
#include "mmi_nv.h"
#include "mmi_mainmenu_export.h"
#include "mmipub.h"
#include "guirichtext.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "mmiset_text.h"
//#include "mmiset_call.h"
#include "mmiset_call_export.h"
#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mn_api.h"
#include "mmipdp_export.h"
#ifdef MMI_GUI_STYLE_TYPICAL
#include "guictrl_api.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef MMS_SUPPORT
#include "mmimms_export.h"
#endif
//#include "mmibrw_text.h"
#ifdef BLUETOOTH_SUPPORT
//#include "mmibt_text.h"
#endif
#ifdef GPS_SUPPORT
#include "mmigps_text.h"
#endif
#ifdef _ATEST_
#include "atest_ota.h"
#endif//_ATEST_
//#include "mmieng.h"
#include "mmieng_export.h"
#ifdef DCD_SUPPORT
#include "mmidcd_export.h"
#endif
#ifdef MMI_GPRS_SUPPORT
#include "mmi_data.h"
#endif
#include "mmi_appmsg.h"
#include "mmiconnection_internal.h"
#if defined(DATA_ACCOUNT_IN_TCARD_SUPPORT) && defined(BROWSER_SUPPORT)
#include "gui_ucs2b_converter.h"
#include "mmiudisk_export.h"
#endif
#ifdef MMI_GPRS_SWITCH_SUPPORT
#include "mmiidle_statusbar.h"
#include "mmicom_statusarea.h"
#endif
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
#include "gui_ucs2b_converter.h"

#ifdef MMI_GPRS_DATA_SIM_SET
#include "mmiset_export.h"
#endif


#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
#include "variant_config_nv.h"
#endif
#include "mmipicview_export.h"
#include "mmimms_id.h"
#include "mmimms_text.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMICONNECTION_DETAIL_LINE_SPACE    5

#ifdef MMI_GPRS_SUPPORT
#define NETWORK_ACCOUNT_MAGIC_FLAG "OTAC"
#define NETWORK_ACCOUNT_MAGIC_OFFSET 0x00
#define NETWORK_ACCOUNT_MAGIC_LENGTH 0x04

#define NETWORK_ACCOUNT_VERSION_OFFSET 0x04
#define NETWORK_ACCOUNT_VERSION_LENGTH 0x02

#define NETWORK_ACCOUNT_NUM_OFFSET 0x06

#define NETWORK_ACCOUNT_HEADER_LENGTH 0x08

#define NETWORK_ACCOUNT_OFFSET_LENGTH 0x04

#define NETWORK_ACCOUNT_ITEM_UIODER_LEN 0x02

#define NETWORK_ACCOUNT_ITEM_MCC_OFFSET 0x02
#define NETWORK_ACCOUNT_ITEM_MNC_OFFSET 0x04
#define NETWORK_ACCOUNT_ITEM_PORT_OFFSET 0x06
#define NETWORK_ACCOUNT_ITEM_ACCESS_TYPE_OFFSET 0x08
#define NETWORK_ACCOUNT_ITEM_ACCESS_OPTION_OFFSET 0x09
#define NETWORK_ACCOUNT_ITEM_AUTH_TYPE_OFFSET 0x0A
#define NETWORK_ACCOUNT_ITEM_ACCOUNT_TYPE_OFFSET 0x0B
#define NETWORK_ACCOUNT_ITEM_MVNO_TYPE_OFFSET 0x0C
#define NETWORK_ACCOUNT_ITEM_VISIBLE_OFFSET 0x0D
#define NETWORK_ACCOUNT_ITEM_ROAMING_OFFSET 0x0E
#define NETWORK_ACCOUNT_ITEM_ROAMING_PROTOCOL_OFFSET 0x0F

#define NETWORK_ACCOUNT_ITEM_NAME_LEN_OFFSET (NETWORK_ACCOUNT_ITEM_ROAMING_PROTOCOL_OFFSET+1)
#define NETWORK_ACCOUNT_ITEM_NAME_LEN_LENGTH 0x02

#define NETWORK_ACCOUNT_ITEM_GATEWAY_LENGTH 0x02
#define NETWORK_ACCOUNT_ITEM_DNS1_LENGTH 0x02
#define NETWORK_ACCOUNT_ITEM_DNS2_LENGTH 0x02
#define NETWORK_ACCOUNT_ITEM_USERNAME_LENGTH 0x02
#define NETWORK_ACCOUNT_ITEM_PASSWORD_LENGTH 0x02
#define NETWORK_ACCOUNT_ITEM_APN_LENGTH 0x02
#define NETWORK_ACCOUNT_ITEM_HOMEPAGE_LENGTH 0x02
#define NETWORK_ACCOUNT_ITEM_SPN_LENGTH 0x02

#if 1
#define CONVERT2U8TOU16(a, b) (a + ((b << 8)&0xFF00))
#define CONVERT4U8TOU32(a, b, c, d) (a + ((b << 8)&0xFF00) + ((c << 16)&0xFF0000) + ((d << 24)&0xFF000000))
#else
#define CONVERT2U8TOU16(a, b) (b + ((a << 8)&0xFF00))
#define CONVERT4U8TOU32(a, b, c, d) (d + ((c << 8)&0xFF00) + ((b << 16)&0xFF0000) + ((a << 24)&0xFF000000))
#endif
#endif

#define MMICONNECTION_DEFAULT_DNS       "0.0.0.0"
#define MMICONNECTION_INVALID_GATEWAY_FOR_SNTP              "0.0.0.0"
#define MMICONNECTION_CMCC_DERECT_LINK_APN              "cmnet"

#ifdef DATA_ACCOUNT_IN_TCARD_SUPPORT
#define NETACCOUNT_FILE_TCARD_PATH      L"E:\\System\\Network_Account\\"
#define NETACCOUNT_FILE_TCARD1_PATH     L"F:\\System\\Network_Account\\"
#define NETACCOUNT_FILE_UDISK_PATH      L"D:\\System\\Network_Account\\"
#ifdef WIN32
#define NETACCOUNT_FILE_DOWNLOAD_URL   L"http://172.16.14.173:8082/daveruan/R_NETWORK_ACCOUNT.ntac"  //Index of/daveruan/R_NETWORK_ACCOUNT.ntac
#else
#define NETACCOUNT_FILE_DOWNLOAD_URL   L"http://51wre.com/ntac/"
#endif
#define NETACCOUNT_FILE_PATH "E:\\System\\Network_Account\\R_NETWORK_ACCOUNT"
#define NETACCOUNT_FILE_NAME "R_NETWORK_ACCOUNT.ntac"
#endif
/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#ifdef MMI_GPRS_SUPPORT
LOCAL MMICONNECTION_SETTING_T  s_connection_setting_info = {0};
LOCAL MMICONNECTION_PNNAPN_EXIST_T  s_is_pnn_apn_exist[MMI_DUAL_SYS_MAX] = {0};
LOCAL MN_DUAL_SYS_E s_connection_handling_sim_id = MN_DUAL_SYS_1;
LOCAL MMICONNECTION_CALLBACK_FUNC s_connection_callback_func_ptr = PNULL;
LOCAL MMICONNECTION_APNTYPEINDEX_T  s_apntype_index = {0};
#endif
#ifdef MMI_GPRS_SWITCH_SUPPORT
LOCAL BOOLEAN s_gprs_turned_off = FALSE;
#endif
MMICONNECTION_NOTIFY_LIST_T    *s_subscriber_list_ptr = PNULL;

//MS00216980 cheney
#define MMIDCD_HOME_PAGE             "http://dcd1.monternet.com"
#define MMIDCD_HOME_PAGE_TEST    "http://218.206.176.248"

#ifndef MMI_GUI_STYLE_TYPICAL
#define MMICONNECT_STRNUM_MAX_LEN                    40
#endif /* MMI_GUI_STYLE_TYPICAL */

//˵�����˱��������������õ������ã��������䣬ע��wap����ҳ����ʡ��http://��mms�Ĳ������Ĳ�����ʡ��
//table��Ŀԭ���ϲ�����24�����������24������Ҫ�޸�MMICMSBRW_Reaserch����
#ifdef MMI_GPRS_SUPPORT
#define LINK_SETTING_TABLE_LEN  (sizeof(setting_search_table)/sizeof(MMICONNECTION_LINKSETTING_ITEM_T))
const MMICONNECTION_LINKSETTING_ITEM_T  setting_search_table[]=
{
    //China
    //MCC/MNC:460/00 CMCC
#ifdef CMCC_UI_STYLE
    {460, 00, 80, 1, MN_PAP, MMICONNECTION_ACCOUNT_BROWSER, TXT_CONNECTION_CMWAP, "10.0.0.172", "", "", "cmwap", "http://wap.monternet.com"},
    {460, 00, 80, 1, MN_PAP, MMICONNECTION_ACCOUNT_MMS, TXT_CONNECTION_CMMMS, "10.0.0.172", "", "", "cmwap", "http://mmsc.monternet.com"},
    {460, 00, 0, 1, MN_PAP, MMICONNECTION_ACCOUNT_BROWSER, TXT_CONNECTION_CMNET, "0.0.0.0", "", "", "cmnet", "http://www.monternet.com"},
#else
    {460, 00, 80, 1, MN_AUTH_NONE, MMICONNECTION_ACCOUNT_BROWSER, TXT_CONNECTION_CMCCWAP, "10.0.0.172", "", "", "cmwap", "http://wap.monternet.com"},
    {460, 00, 80, 1, MN_AUTH_NONE, MMICONNECTION_ACCOUNT_MMS, TXT_CONNECTION_CHINAMOBILEMMS, "10.0.0.172", "", "", "cmwap", "http://mmsc.monternet.com"},
    {460, 00, 0, 1, MN_AUTH_NONE, MMICONNECTION_ACCOUNT_BROWSER, TXT_CONNECTION_CMCCWWW, "0.0.0.0", "", "", "cmnet", "http://www.monternet.com"},
#endif
#ifdef DCD_SUPPORT
    {460, 00, 80, 1, MN_AUTH_NONE, MMICONNECTION_ACCOUNT_DCD, TXT_CONNECTION_CMCCDCD, "10.0.0.172", "", "", "cmwap", MMIDCD_HOME_PAGE},
#endif

    //MCC/MNC:460/01 CUCC
    {460, 01, 80, 1, MN_AUTH_NONE, MMICONNECTION_ACCOUNT_BROWSER, TXT_CONNECTION_CUCC_WAP, "10.0.0.172", "", "", "uniwap", "http://wap.uni-info.com.cn"},
    {460, 01, 80, 1, MN_AUTH_NONE, MMICONNECTION_ACCOUNT_MMS, TXT_CONNECTION_CUCC_MMS, "10.0.0.172", "", "", "uniwap", "http://mmsc.myuni.com.cn"},
    {460, 01, 0, 1, MN_AUTH_NONE, MMICONNECTION_ACCOUNT_BROWSER, TXT_CONNECTION_CUCC_NET, "0.0.0.0", "", "", "uninet", "http://www.chinaunicom.com.cn"},

};

#define LINK_SETTING_NAME_TABLE_LEN  (sizeof(setting_name_table)/sizeof(MMICONNECTION_NAME_T))
const MMICONNECTION_NAME_T setting_name_table[] =
{
// Modify Temply By zhangpeng.xu, For fix linux compiling error, 2018/11/06
#if 1 //def CMCC_UI_STYLE  
    {L"CMWAP", TXT_CONNECTION_CMCCWAP},
    {L"CMMMS", TXT_CONNECTION_CHINAMOBILEMMS},
    {L"CMNET", TXT_CONNECTION_CMCCWWW},
#else
    {L"�й��ƶ�����", TXT_CONNECTION_CMCCWAP},
    {L"�й��ƶ�����", TXT_CONNECTION_CHINAMOBILEMMS},
    {L"�й��ƶ�������", TXT_CONNECTION_CMCCWWW},
#endif
#if 0 //def DCD_SUPPORT
    {L"�й��ƶ���Ѷ", TXT_CONNECTION_CMCCDCD},
    {L"�й���ͨWAP", TXT_CONNECTION_CUCC_WAP},
    {L"�й���ͨ����", TXT_CONNECTION_CUCC_MMS},
    {L"�й���ͨ������", TXT_CONNECTION_CUCC_NET},
#else
    {L"CMCCDCD", TXT_CONNECTION_CMCCDCD},
    {L"CUCCWAP", TXT_CONNECTION_CUCC_WAP},
    {L"CUCCMMS", TXT_CONNECTION_CUCC_MMS},
    {L"CUCCNET", TXT_CONNECTION_CUCC_NET},
#endif
};
#endif

#define LINK_CUSTOMED_LEN  (sizeof(s_customed_index)/sizeof(s_customed_index[0]))

#ifdef CMCC_UI_STYLE
#ifndef DCD_SUPPORT
static uint8 s_customed_index[]={0,1,2};//�ƶ����������ϱ��еı��
#else
static uint8 s_customed_index[]={0,1,2,5};//�ƶ����������ϱ��еı��
#endif
#else
#ifndef LOW_MEMORY_SUPPORT_32X16
#ifndef DCD_SUPPORT
const uint8 s_customed_index[]={0,1,2,3,4};//�ƶ����������ϱ��еı��
#else
const uint8 s_customed_index[]={0,1,2,3,4,5};//�ƶ����������ϱ��еı��
#endif
#else
const uint8 s_customed_index[]={0,1,2};//�ƶ����������ϱ��еı��
#endif
#endif

#define LINK_CUSTOMED_RES_LEN  (sizeof(s_customed_res_index)/sizeof(s_customed_res_index[0]))
const uint8 s_customed_res_index[]={0,1,2,3};//����Ĭ��ֻ����ǰ��5��
#ifdef MMI_GPRS_SUPPORT
LOCAL uint8 s_connection_index = 1;  /*lint -e551*/
#endif
LOCAL uint8 s_network_type_select = 2; //lulifei add for the Network Type select requirment

typedef BOOLEAN (*PLMN_CMP_CALLBACK)(PHONE_PLMN_NAME_E plmn1, PHONE_PLMN_NAME_E plmn2);

LOCAL THEMELIST_ITEM_STYLE_T s_list_style = {0};

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMICONNECTION_LINKSET_OP_NULL,
    MMICONNECTION_LINKSET_OP_EDIT,
    MMICONNECTION_LINKSET_OP_READ,
    MMICONNECTION_LINKSET_OP_NEW
}MMICONNECTION_LINKSET_OP_E;

typedef enum
{
    NETWORK_TYPE_DM_E,        // �Զ�˫ģT
    NETWORK_TYPE_DM_G_E,        // �Զ�˫ģG
    NETWORK_TYPE_GPRS_E,   //  2g��ģ
    NETWORK_TYPE_TD_E,        // 3g��ģ
    NETWORK_TYPE_MAX
}NETWORK_TYPE_E;

 typedef enum
{
    ID_CONNECTION_NETWORK_SETTING_OPERATOR,
    ID_CONNECTION_NETWORK_SETTING_TYPE,
    ID_CONNECTION_NETWORK_SETTING_MOBILE_DATA,
    ID_CONNECTION_NETWORK_SETTING_PIN_REQUEST,
    ID_CONNECTION_NETWORK_SETTING_CHANGE_PIN,
    ID_CONNECTION_NETWORK_SETTING_CHANGE_PIN2,
#if defined(OMA_CP_SEC_SUPPORT) 
    ID_CONNECTION_NETWORK_SETTING_CHANGE_OTA
#endif    
}NETWORK_SETTING_E;

 typedef enum{
     CONNECTION_SETTING_CONTRACT,
     CONNECTION_SETTING_PAYG
 }CONNECTION_SETTING_VODAFONE_E;


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : set the select list of the searchresult list box to high priority
//	Global resource dependence :
//  Author:wenbao.bai
//	Note:
/****************************************************************************/
LOCAL void MMICONNECTION_SetHighPriList(MN_DUAL_SYS_E dual_sys, uint8 list_index);

/*****************************************************************************/
// 	Description :clear the priority of the setting link lists box
//	Global resource dependence :
//  Author:wenbao.bai
//	Note:
/****************************************************************************/
LOCAL void MMICONNECTION_ClearHighPriList(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ChangeAppSelectedIndex(MN_DUAL_SYS_E dual_sys, uint16 list_index, MMICONNECTION_NOTIFY_EVENT_E event);

/*****************************************************************************/
//  Description :append one line icon text list item
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void AppendOneLineTextListItem(
                                     MMI_CTRL_ID_T       ctrl_id,
                                     GUIITEM_STYLE_E     item_style,
                                     MMI_TEXT_ID_T       left_softkey_id,
                                     MMI_TEXT_ID_T       mid_softkey_id,
                                     MMI_IMAGE_ID_T      icon_id,
                                     wchar*              wstr_ptr,
                                     uint16              wstr_len,
                                     uint16              pos,
                                     BOOLEAN             is_update
                                     );

#ifdef MMI_GPRS_SUPPORT
/****************************************************************************/
// 	Description : translate the setting item to richtext item, and add it to richtext control
//	Global resource dependence : none
//  Author: bei.wu
//	Note:
/*****************************************************************************/
LOCAL void SetSettingDetailEntry(MMI_CTRL_ID_T ctrl_id,
                                 MMICONNECTION_LINKSETTING_DETAIL_T* item
                                 );

/*****************************************************************************/
// 	Description : Connection menu
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleConnectionMenuWinMsg(
									 MMI_WIN_ID_T win_id,
									 MMI_MESSAGE_ID_E msg_id,
									 DPARAM param);
#endif

/*****************************************************************************/
// 	Description : Connection menu
//	Global resource dependence :
//  Author: lifei.lu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleConnectionMenuNetworkTypeWin(
									 MMI_WIN_ID_T win_id,
									 MMI_MESSAGE_ID_E msg_id,
									 DPARAM param);

LOCAL MMI_RESULT_E HandleConnectionMenuNetworkSettingWin(
	                                                                         MMI_WIN_ID_T win_id,
									 MMI_MESSAGE_ID_E msg_id,
									 DPARAM param);

#ifdef MMI_2G_4G_SUPPORT
PUBLIC MMI_TEXT_ID_T GetNetworkTypeStringId(void);
PUBLIC void CreateNetworkTypeWin(void);
#endif
#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
//  Description : Is Same Network Plmn
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSameNetworkPlmn(MN_PLMN_T plmn1, MN_PLMN_T plmn2);

/*****************************************************************************/
//  Description : adjust whether any other items exist with same mcc&mnc
//  Global resource dependence : none
//  Author: bean.yu
//  Note:cmp mcc and mnc
/*****************************************************************************/
LOCAL BOOLEAN IsSamePLMNItemExistStill( MMICONNECTION_RES_INFO_T *conn_res, uint32 index );

/*****************************************************************************/
//  Description : adjust whether any other items exist with same mcc&mnc
//  Global resource dependence : none
//  Author: bean.yu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValidNetworkAccount(MN_DUAL_SYS_E dual_sys, MMICONNECTION_RES_ITEM_T *conn_res, uint32 *mvnotype_appointed_type);

/*****************************************************************************/
//  Description : get default account num
//  Global resource dependence : none
//  Author: bean.yu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetDefaultNetworkAccountNum( MMICONNECTION_RES_ITEM_T* res_item, uint32 index, BOOLEAN spn_empty, BOOLEAN res_spn_empty );

/*****************************************************************************/
//  Description : set default for esp account
//  Global resource dependence : none
//  Author: bean.yu
//  Note: index is lastest group items whatever empty or not.
/*****************************************************************************/
LOCAL void SetDefaultNetworkAccount(MMICONNECTION_RES_INFO_T *setting_info, MN_DUAL_SYS_E dual_sys, uint32 cur_index, uint32 first_index);
/*****************************************************************************/
//  Description : Is Same Network name
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSameNetworkName(PHONE_PLMN_NAME_E plmn1, PHONE_PLMN_NAME_E plmn2);

/*****************************************************************************/
// 	Description : handle message of setting list window of connection
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingListWinMsg(
                                           MMI_WIN_ID_T	    win_id,
                                           MMI_MESSAGE_ID_E	msg_id,
                                           DPARAM           param
                                           );

/*****************************************************************************/
// 	Description : handle message of setting link list window of browser
//	Global resource dependence : none
//  Author: bei.wu
//	Note:   cr115674
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkDetailViewWinMsg(
                                                     MMI_WIN_ID_T	    win_id,
                                                     MMI_MESSAGE_ID_E	msg_id,
                                                     DPARAM				param
                                                     );

/*****************************************************************************/
// 	Description : handle message of setting link list menu window of connection
//      This menu is to deal to a selected list such as,edit,delete,etc.
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkListMenuWinMsg(
                                                   MMI_WIN_ID_T	    win_id,
                                                   MMI_MESSAGE_ID_E	msg_id,
                                                   DPARAM			param
                                                   );

/*****************************************************************************/
// 	Description : handle message of  search link list window
//	Global resource dependence : none
//  Author: wenbao.bai
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSearchResultLinkListWinMsg(
                                                    MMI_WIN_ID_T    	win_id,
                                                    MMI_MESSAGE_ID_E	msg_id,
                                                    DPARAM				param
                                                    );

/*****************************************************************************/
//     Description : to set the MMICONNECTION_GPRS_SETTINGS_CTRL_ID
//    Global resource dependence :
//  Author:wancan.you
//    Note:
/*****************************************************************************/
LOCAL void SetGPRSSettingsWin(void);

/*****************************************************************************/
//     Description : to handle window message about gprs settings.
//    Global resource dependence :
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGPRSSettingsWinMsg(
                                                       MMI_WIN_ID_T        win_id,
                                                       MMI_MESSAGE_ID_E msg_id,
                                                       DPARAM            param
                                                       );

#if 0 //bug185707
/*****************************************************************************/
// 	Description : handle message of Add  setting list window of connection
//	Global resource dependence : none
//  Author: wenbao.bai
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddSettingListWinMsg(
                                              MMI_WIN_ID_T	    win_id,
                                              MMI_MESSAGE_ID_E	msg_id,
                                              DPARAM			param
                                              );
#endif
/*****************************************************************************/
// 	Description : handle message of setting link list window of connection
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkMenuWinMsg(
                                                MMI_WIN_ID_T    	win_id,
                                                MMI_MESSAGE_ID_E	msg_id,
                                                DPARAM				param
                                                );

/*****************************************************************************/
// 	Description : handle message of Securityconnect setting list window of connection
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingSecurityconnectListWinMsg(
                                                          MMI_WIN_ID_T	win_id,
                                                          MMI_MESSAGE_ID_E	msg_id,
                                                          DPARAM				param
                                                          );

#ifdef IPVERSION_SUPPORT_V4_V6
/*****************************************************************************/
// 	Description : handle message of ip type setting list window of connection
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingIpTypeconnectListWinMsg(
                                                          MMI_WIN_ID_T	win_id,
                                                          MMI_MESSAGE_ID_E	msg_id,
                                                          DPARAM				param
                                                          );

#endif

/*****************************************************************************/
// 	Description : deal with account type setting
//	Global resource dependence : none
//  Author: zhongping.shi
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingAccountTypeconnectListWinMsg(
                                                             MMI_WIN_ID_T win_id,
                                                             MMI_MESSAGE_ID_E msg_id,
                                                             DPARAM param
                                                             );
                                                             
/*****************************************************************************/
// 	Description : handle message of input username name window of connection
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputUserNameWinMsg(
                                             MMI_WIN_ID_T	win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             );

/*****************************************************************************/
// 	Description : handle message of input password window of connection
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputPasswordWinMsg(
                                             MMI_WIN_ID_T	win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             );

/*****************************************************************************/
// 	Description : handle message of input apn window of connection
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputApnWinMsg(
                                        MMI_WIN_ID_T	win_id,
                                        MMI_MESSAGE_ID_E	msg_id,
                                        DPARAM				param
                                        );


/*****************************************************************************/
// 	Description : handle message of input gateway window of connection
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputGatewayWinMsg(
                                            MMI_WIN_ID_T	win_id,
                                            MMI_MESSAGE_ID_E	msg_id,
                                            DPARAM				param
                                            );

/*****************************************************************************/
// 	Description : handle message of port list window of connection
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortListWinMsg(
                                              MMI_WIN_ID_T	win_id,
                                              MMI_MESSAGE_ID_E	msg_id,
                                              DPARAM				param
                                              );

/*****************************************************************************/
// 	Description : handle message of input agent name window of connection
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputAgentNameWinMsg(
                                              MMI_WIN_ID_T	win_id,
                                              MMI_MESSAGE_ID_E	msg_id,
                                              DPARAM				param
                                              );

/*****************************************************************************/
// 	Description : handle message of input homepage window of connection
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputHomepageWinMsg(
                                             MMI_WIN_ID_T	win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             );

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkAdvanceWinMsg(
                                                MMI_WIN_ID_T	win_id,
                                                MMI_MESSAGE_ID_E	msg_id,
                                                DPARAM				param
                                                );

/*****************************************************************************/
// 	Description : handle message of setting dns window of connection
//	Global resource dependence : none
//  Author: bei.wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkDNSWinMsg(
                                                MMI_WIN_ID_T	    win_id,
                                                MMI_MESSAGE_ID_E	msg_id,
                                                DPARAM				param
                                                );
/*****************************************************************************/
// 	Description : handle message of setting net type window of connection
//	Global resource dependence : none
//  Author: bei.wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkNetTypeWinMsg(
                                                MMI_WIN_ID_T	    win_id,
                                                MMI_MESSAGE_ID_E	msg_id,
                                                DPARAM				param
                                                );

#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
/*****************************************************************************/
//  Description : Auto Adapting Network account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void AutoAdapting(MN_DUAL_SYS_E dual_sys, MMICONNECTION_ACCOUNT_TYPE_E account_type, BOOLEAN is_need_clean);
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//     Description : to handle window message
//    Global resource dependence :
//  Author:wancan.you
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  ConnectionSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

#endif

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify Connection
//    Global resource dependence :
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyConnectionFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param);
#endif
#endif

/*****************************************************************************/
// 	Description : insert dynamenu node by label
//	Global resource dependence :
//  Author:fen.xie
//	Note:
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                    MMI_CTRL_ID_T      ctrl_id,        //�ؼ�id
                    MMI_TEXT_ID_T   	text_id,       //����Ľڵ���ı�
                    uint32          	node_id,        //�ڵ���
                    uint32          	parent_node_id, //���ڵ���
                    uint16          	index,           //λ��
                    MMI_IMAGE_ID_T      select_icon_id, //item selected icon id
                    BOOLEAN    is_grayed   //is_grayed
                    );

#ifdef MMI_GPRS_DATA_SIM_SET
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGPRSModeSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

#ifdef DATA_ROAMING_SUPPORT
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGPRSDataRoamingSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

#ifdef MMI_GPRS_SUPPORT

/*****************************************************************************/
//  Description :Parse Network Account Info
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:   is_tcard_file: Tcard file OR resource
/*****************************************************************************/
LOCAL BOOLEAN ParseNetworkAccountInfo(MMICONNECTION_RES_INFO_T *res_info_ptr, MMIRES_DATA_INFO_T file_info);

/*****************************************************************************/
//  Description : Load NetAccount Info From Resource
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadNetworkAccountInfoFromRes(MN_DUAL_SYS_E dual_sys, MMICONNECTION_RES_INFO_T *res_info_ptr);

/*****************************************************************************/
//  Description : Load NetAccount Info From Table List
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadNetworkAccountInfoFromTableList(MMICONNECTION_RES_INFO_T *res_info_ptr);

/*****************************************************************************/
//  Description : Load NetAccount Info
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadNetworkAccountInfo(MN_DUAL_SYS_E dual_sys, MMICONNECTION_RES_INFO_T *res_info_ptr);

/*****************************************************************************/
//  Description : Unload NetAccount Info
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UnloadNetworkAccountInfo(MMICONNECTION_RES_INFO_T *res_info_ptr);

/*****************************************************************************/
//  Description : Save NetAccount Info
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveNetworkAccountInfo(MMICONNECTION_LINKSETTING_DETAIL_T *link_detail_ptr,//IN/OUT
                                     MMICONNECTION_RES_ITEM_T *res_item_ptr,//IN
                                     BOOLEAN is_default_unchange,
                                     uint16 list_index);//IN

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_GPRS_SUPPORT
/**************************************************************************************************************/
//  Description : �˽ӿ����ж��ܷ���������ʻ�����ǰ���ſ����Խ���,�ж�����Ϊ:
//  ������ģʽ���޿������رգ�puk ��,pin����reject������֧�ֶ�������������ʻ�
//  Param in: ok_idx: ����ǰok_num ��1����ok_idx����洢���ſ���ok�ģ��������Ϊ��Чֵ
//                  ok_num:��ǰ�����ſ�Ϊok ��
//  Note: ok_idxֻ����ok_num Ϊ1��ʱ������Ч�����������Ч��
/**************************************************************************************************************/
LOCAL void GetSimStatusforAcessNetWorkAccount(uint16* ok_idx,uint16 * ok_num);

/*****************************************************************************/
//  Description :Init pda connection main list
//  Global resource dependence :
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void InitPdaConnectionMainListCtrl(void);
#endif

/*****************************************************************************/
// 	Description : Setting link menu
//	Global resource dependence :
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void  InitPdaSettingLinkListCtrl(MMICONNECTION_LINKSETTING_DETAIL_T* setting_detail);

/*****************************************************************************/
// 	Description : Setting advance ctrl
//	Global resource dependence :
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void  InitPdaSettingAdvanceCtrl(MMICONNECTION_LINKSETTING_DETAIL_T* setting_detail);

/*****************************************************************************/
// 	Description : Setting link menu long press list item
//	Global resource dependence :
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingListLongOKMenuWinMsg(
									 MMI_WIN_ID_T win_id,
									 MMI_MESSAGE_ID_E msg_id,
									 DPARAM param);

/*****************************************************************************/
// 	Description : Connection long ok menu,for mms,browser,dcd,java..
//	Global resource dependence :
//  Author: jixin.xu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLinkListLongOKMenuWinMsg(
									 MMI_WIN_ID_T win_id,
									 MMI_MESSAGE_ID_E msg_id,
									 DPARAM param);


#endif /* MMI_GUI_STYLE_TYPICAL */

/*****************************************************************************/
//  Description :setting link list choose win handle fun
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWapSettingListWinMsg(
                                         MMI_WIN_ID_T win_id,
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM param);
//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
/*****************************************************************************/
// 	Description : handle data roaming query window
//	Global resource dependence : none
//  Author: feng.xiao
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDataRoamingQueryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
// 	Description : handle data roaming cut off query window
//	Global resource dependence : none
//  Author: feng.xiao
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDataRoamingCutoffQueryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif


/*****************************************************************************/
//  Description : Select SIM API of Connection app
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMICONNECTION_SelectSIMFunc(void);
#endif

#ifdef DATA_ACCOUNT_IN_TCARD_SUPPORT
#if defined(BROWSER_SUPPORT)
/*****************************************************************************/
// 	Description : CheckNetworkAccountResFileToDownload
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void CheckNetworkAccountResFileToDownload(void);

/*****************************************************************************/
// 	Description : HandleReplaceExistFileQueryWinMsg
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReplaceExistFileQueryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*******************************************************************/
//  Interface:		HandleNetworkAccountDownloadCB
//  Description : 	Handle Network Account DownloadCB
//  Global resource dependence :
//  Author: dave.ruan
//  Note:
/*******************************************************************/
LOCAL void HandleNetworkAccountDownloadCB(
								   MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr
								   );
#endif

/*******************************************************************/
//  Interface:		MoveFileToNetworkAccount
//  Description : 	�������ص��������ȴ洢��T��,��ͬ�ļ������Ը��ǡ�
//  Global resource dependence :
//  Author: dave.ruan
//  Note:
/*******************************************************************/
LOCAL void MoveFileToNetworkAccount(const wchar *file_path);

/*****************************************************************************/
//  Description : get location file dir
//  Global resource dependence :
//  Author:dave.ruan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetValidNetAccountFileDir(uint16* device_dir,const wchar *file_name_ptr,uint32 file_size);

/*****************************************************************************/
// 	Description : handle refrsh setting list query window
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRefreshSettingListQueryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#endif

/*****************************************************************************/
// 	Description : Get GPRS Mode Settings.
//	Global resource dependence : none
//    Author:
//	Note:
/*****************************************************************************/
LOCAL MMICONNECTION_GPRS_SETTINGS_E MMICONNECTION_GetGPRSModeStatus(void);

#ifdef MMI_GPRS_SWITCH_SUPPORT

/*****************************************************************************/
//  Description : Turn On GRPS
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPICONNECTION_TurnOffGPRS(void);
/*****************************************************************************/
//  Description : Turn off GPRS
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPICONNECTION_TurnOnGPRS(void);

#if defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : GPRS switch win handler
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGprsSwitchWinMsg(MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param);
#endif
#endif
//lulifei add for UI spec
LOCAL MMI_RESULT_E HandleConnEmptyPortWarningWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param);
LOCAL MMI_RESULT_E HandleConnInvaildPortWarningWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetGprsCrWindow(
                                               MMI_WIN_ID_T       win_id,
                                               MMI_MESSAGE_ID_E   msg_id,
                                               DPARAM             param
                                               );

/*****************************************************************************/
//  Description : Vodafone switch win handler
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVodafoneSwitchWinMsg(MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param);

/*****************************************************************************/
//  Description : Update the network type
//  Global resource dependence :
//  Author:lifei.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdatePdaConnectionItem2Line(
                                 MMI_STRING_T  text_str    //���ı�
                                 );

PUBLIC void MMICONNECTION_UpdatePINToggle();
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:lifei.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVodafoneSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handleConfirmationDialog
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E handleConfirmationDialog(
                                                MMI_WIN_ID_T    win_id, // ���ڵ�ID
                                                MMI_MESSAGE_ID_E   msg_id, // ���ڵ��ڲ���ϢID
                                                DPARAM             param   // ��Ӧ��Ϣ�Ĳ���
                                                );

/*****************************************************************************/
//  Description : to set vodafone apn
//  Global resource dependence :
//  Author:lifei.lu
//  Note:
/*****************************************************************************/
LOCAL void InitVodafoneApn() ;
PUBLIC void SetVodafoneApn(MN_DUAL_SYS_E dual_sys,CONNECTION_SETTING_VODAFONE_E vodafone_type);
LOCAL void InitVodafoneApnList(MN_DUAL_SYS_E dual_sys,CONNECTION_SETTING_VODAFONE_E vodafone_type) ;
/*****************************************************************************/
// Description : Get if is Orange Portocol Type
// Global resource dependence:
// Author:dong.chunguang
// Note:
/*****************************************************************************/
#ifdef IPVERSION_SUPPORT_V4_V6
LOCAL uint8  MMICONN_GetIP_PortocolType(void);
#endif
/*****************************************************************************/
// Description : Get if is Orange Ploand card
// Global resource dependence:
// Author:dong.chunguang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMICONNECTION_IsPolandSimCard(MN_DUAL_SYS_E   dual_sys);
/**--------------------------------------------------------------------------*
**                         WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/
#ifdef MMI_GPRS_SUPPORT
WINDOW_TABLE( MMICONNECTION_LINKLIST_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleWapSettingListWinMsg),
    WIN_ID(MMICONNECTION_LINKLIST_WIN_ID),
    WIN_STYLE( WS_HAS_SOFTKEY),
    WIN_TITLE(TXT_COMMON_WWW_ACCOUNT),
    WIN_SOFTKEY(PNULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_LIST_SETTING_CTRL_ID),
    END_WIN
};
#endif

#if defined MMI_GPRS_SUPPORT
#ifdef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE(MMICONNECTION_MAIN_MENU_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleConnectionMenuWinMsg),
    WIN_ID( MMICONNECTION_MENU_WIN_ID ),
    WIN_TITLE(TXT_SETTINGS_LINK),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#else
WINDOW_TABLE(MMICONNECTION_MAIN_MENU_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleConnectionMenuWinMsg),
    WIN_ID( MMICONNECTION_MENU_WIN_ID ),
    WIN_TITLE(TXT_SETTINGS_LINK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICONNECTION_MENU_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
#endif /* MMI_PDA_SUPPORT */
    END_WIN
};
#endif /* MMI_GUI_STYLE_TYPICAL */
#endif

#ifdef MMI_GPRS_SUPPORT

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(MMICONNECTION_SETTINGLINKLIST_LONGOKMENU_WIN_TAB)=
{
    WIN_FUNC( (uint32)HandleSettingListLongOKMenuWinMsg),
    WIN_ID(MMICONNECTION_SETTINGLINKLISTLONGOKMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CONNECTION_LONGOK_LINKLIST,MMICONNECTION_MENU_SETTINGLINK_LONGOK_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMICONNECTION_LINKLIST_LONGOKMENU_WIN_TAB)=
{
    WIN_FUNC( (uint32)HandleLinkListLongOKMenuWinMsg),
    WIN_ID(MMICONNECTION_LINKLISTLONGOKMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CONNECTION_LINKLIST_LONGOK,MMICONNECTION_MENU_SETTINGLINK_LONGOK_CTRL_ID),
    END_WIN
};
#endif

WINDOW_TABLE(MMICONNECTION_SETTINGLINKLISTMENU_WIN_TAB)=
{
    WIN_FUNC( (uint32)HandleSettingLinkListMenuWinMsg),
#ifdef MAINLCD_SIZE_128X160
    WIN_TITLE(TXT_COMMON_LINKSETTING),
#endif
    WIN_ID(MMICONNECTION_SETTINGLINKLISTMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CONNECTION_SETTING_LINKLIST,MMICONNECTION_MENU_SETTINGLINKLIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};


//the window for connect setting list
WINDOW_TABLE(MMICONNECTION_SETTINGLIST_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSettingListWinMsg),
    WIN_ID(MMICONNECTION_SETTINGLIST_WIN_ID),
    WIN_TITLE(TXT_COMMON_WWW_ACCOUNT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICONNECTION_LIST_SETTING_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMICONNECTION_SEARCHLINKLIST_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSearchResultLinkListWinMsg),
    WIN_ID(MMICONNECTION_SEARCHLINKLIST_WIN_ID),
    WIN_TITLE(TXT_CONNECTION_LINKLIST),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TYPE_NONE,MMICONNECTION_LIST_SEARCHLINKLIST_CTRL_ID),
    END_WIN
};

//GPRS Settings
WINDOW_TABLE( MMICONNECTION_GPRS_SETTINGS_WIN_TAB ) =
{
    WIN_TITLE( TXT_DATA_CONNECTION_SET),
    WIN_FUNC((uint32) HandleGPRSSettingsWinMsg),
    WIN_ID( MMICONNECTION_GPRS_SETTINGS_WIN_ID ),
    #ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_GPRS_SETTINGS_CTRL_ID ),
	END_WIN
};
#if 0 //bug185707
//the window for connect link setting list
WINDOW_TABLE(MMICONNECTION_ADDSETTINGLIST_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleAddSettingListWinMsg),
    WIN_ID(MMICONNECTION_ADDSETTINGLIST_WIN_ID),
    WIN_TITLE(TXT_COMMON_WWW_ACCOUNT),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E,MMICONNECTION_LIST_SETTINGADDLIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SELECT, STXT_RETURN),
    END_WIN
};
#endif
#if !defined MMI_GUI_STYLE_TYPICAL
//the window for setting menu
WINDOW_TABLE(MMICONNECTION_SETTINGLINKMENU_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSettingLinkMenuWinMsg),
    WIN_ID(MMICONNECTION_SETTINGLINKMENU_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICONNECTION_MENU_SETTINGLINK_CTRL_ID),
    END_WIN
};
#else
//the window for setting menu
WINDOW_TABLE(MMICONNECTION_SETTINGLINKMENU_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSettingLinkMenuWinMsg),
    WIN_ID(MMICONNECTION_SETTINGLINKMENU_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_CONNECTION_SETTING_LINK, MMICONNECTION_MENU_SETTINGLINK_CTRL_ID),
    END_WIN
};
#endif

WINDOW_TABLE(MMICONNECTION_SETTINGLINKDETAILVIEW_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSettingLinkDetailViewWinMsg),
    WIN_ID(MMICONNECTION_SETTINGLINKDETAILVIEW_WIN_ID),
    WIN_TITLE(TXT_COMMON_LINKSETTING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMICONNECTION_RICHTEXT_SETTINGLINKDETAILVIEW_CTRL_ID),
    END_WIN
};

//the window for security connect setting list
WINDOW_TABLE(MMICONNECTION_SETTINGSECURITYCONNECTLIST_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSettingSecurityconnectListWinMsg),
    WIN_ID(MMICONNECTION_SETTINGSECURITYCONNECTLIST_WIN_ID),
    WIN_TITLE(TXT_CONNECTION_AUTH_TYPE),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_LIST_SETTINGSECURITYCONNECT_CTRL_ID),
    END_WIN
};

#ifdef IPVERSION_SUPPORT_V4_V6
WINDOW_TABLE(MMICONNECTION_SETTINGIPTYPE_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSettingIpTypeconnectListWinMsg),
    WIN_ID(MMICONNECTION_SETTINGIPTYPE_WIN_ID),
    WIN_TITLE(TXT_CONNECTION_IP_TYPE),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_LIST_SETTINGIPTYPE_CTRL_ID),
    END_WIN
};
#endif

WINDOW_TABLE(MMICONNECTION_SETTINGACCOUNTTYPE_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSettingAccountTypeconnectListWinMsg),
    WIN_ID(MMICONNECTION_SETTINGACCOUNTTYPE_WIN_ID),
    WIN_SOFTKEY(NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_LIST_SETTINGACCOUNTTYPE_CTRL_ID),
    END_WIN
};
//the window for input user name
WINDOW_TABLE(MMICONNECTION_INPUTUSERNAME_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleInputUserNameWinMsg),
    WIN_ID(MMICONNECTION_INPUTUSERNAME_WIN_ID),
    WIN_TITLE(TXT_COMMON_USERNAME),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //����Ϊ������
    CREATE_EDIT_TEXT_CTRL(MMICONNECTION_MAX_USERNAME_LEN ,MMICONNECTION_EDIT_INPUTUSERNAME_CTRL_ID),
    END_WIN
};

//the window for input password
WINDOW_TABLE(MMICONNECTION_INPUTPASSWORD_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleInputPasswordWinMsg),
    WIN_ID(MMICONNECTION_INPUTPASSWORD_WIN_ID),
    WIN_TITLE(TXT_COMMON_PASSWORD),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //����Ϊ������
    CREATE_EDIT_PASSWORD_CTRL(MMICONNECTION_MAX_PASSWORD_LEN ,MMICONNECTION_EDIT_INPUTPASSWORD_CTRL_ID),
    END_WIN
};

//the window for input apn
WINDOW_TABLE(MMICONNECTION_INPUTAPN_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleInputApnWinMsg),
    WIN_ID(MMICONNECTION_INPUTAPN_WIN_ID),
    WIN_TITLE(TXT_CONNECTION_APN),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //����Ϊ������
    CREATE_EDIT_TEXT_CTRL(MMICONNECTION_MAX_APN_LEN ,MMICONNECTION_EDIT_INPUTAPN_CTRL_ID),
    END_WIN
};


//the window for input gateway
WINDOW_TABLE(MMICONNECTION_INPUTGATEWAY_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleInputGatewayWinMsg),
    WIN_ID(MMICONNECTION_INPUTGATEWAY_WIN_ID),
    WIN_TITLE(TXT_COMMON_GATEWAY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //����Ϊ������

    CREATE_EDIT_TEXT_CTRL(MMICONNECTION_MAX_GATEWAY_LEN, MMICONNECTION_EDIT_INPUTGATEWAY_CTRL_ID),
    END_WIN
};


//the window for port list
WINDOW_TABLE(MMICONNECTION_PORTLIST_WIN_TAB) =
{
    WIN_FUNC((uint32)HandlePortListWinMsg),
    WIN_ID(MMICONNECTION_EDITPORT_WIN_ID),
    WIN_TITLE(TXT_CONNECTION_PORT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(MMICONNECTION_MAX_PORT_LEN,MMICONNECTION_EDIT_INPUTPORT_CTRL_ID),
    END_WIN
};

//the window for input agent name
WINDOW_TABLE(MMICONNECTION_INPUTAGENTNAME_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleInputAgentNameWinMsg),
    WIN_ID(MMICONNECTION_INPUTAGENTNAME_WIN_ID),
    WIN_TITLE(TXT_CONNECTION_CHANGENAME),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //����Ϊ������
    CREATE_EDIT_TEXT_CTRL(MMICONNECTION_MAX_LINKMODENAME_LEN ,MMICONNECTION_EDIT_INPUTAGENTNAME_CTRL_ID),
    END_WIN
};


//the window for input homepage
WINDOW_TABLE(MMICONNECTION_INPUTHOMEPAGE_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleInputHomepageWinMsg),
    WIN_ID(MMICONNECTION_INPUTHOMEPAGE_WIN_ID),
    WIN_TITLE(TXT_COMMON_HOMEPAGE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //����Ϊ������
    CREATE_EDIT_TEXT_CTRL(MMICONNECTION_HOMEPAGE_MAX_URL_LEN ,MMICONNECTION_EDIT_INPUTHOMEPAGE_CTRL_ID),
    END_WIN
};


#if !defined MMI_GUI_STYLE_TYPICAL
//the window for setting link dns
WINDOW_TABLE(MMICONNECTION_SETTINGLINK_ADVANCE_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleSettingLinkAdvanceWinMsg),
    WIN_ID( MMICONNECTION_SETTINGLINK_ADVANCE_WIN_ID ),
    WIN_TITLE(TXT_CONNECTION_ADVANCE),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICONNECTION_MENU_SETTINGLINK_ADVANCE_CTRL_ID),
    END_WIN
};
#else
//the window for setting link dns
WINDOW_TABLE(MMICONNECTION_SETTINGLINK_ADVANCE_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSettingLinkAdvanceWinMsg),
    WIN_ID(MMICONNECTION_SETTINGLINK_ADVANCE_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_CONNECTION_SETTING_LINK_ADVANCE, MMICONNECTION_MENU_SETTINGLINK_ADVANCE_CTRL_ID),
    END_WIN
};
#endif

//the window for setting link dns
WINDOW_TABLE(MMICONNECTION_SETTINGLINK_DNS_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSettingLinkDNSWinMsg),
    WIN_ID(MMICONNECTION_SETTINGLINK_DNS_WIN_ID),
    WIN_TITLE(TXT_CONNECTION_DNS),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_IP_CTRL(MMICONNECTION_EDIT_INPUT_DNS_CTRL_ID),
    END_WIN
};

//the window for setting net type
WINDOW_TABLE(MMICONNECTION_SETTINGLINK_NETTYPE_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSettingLinkNetTypeWinMsg),
    WIN_ID(MMICONNECTION_SETTINGLINK_NETTYPE_WIN_ID),
    WIN_TITLE(TXT_NETWORK_TYPE),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_LIST_SELECT_NETTYPE_CTRL_ID),
    END_WIN
};

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*		//Sam.hua	use SelectSimFunc
WINDOW_TABLE(MMICONNECTION_SELECT_SIM_WIN_TAB) =
{
    WIN_TITLE(TXT_SIM_SEL),
    WIN_FUNC((uint32) HandleConnectionSelectSimWinMsg),
    WIN_ID( MMICONNECTION_SELECT_SIM_WIN_ID ),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICONNECTION_SELECT_SIM_CTRL_ID),

    END_WIN
};
*/
#endif
#endif

#ifdef MMI_GPRS_SWITCH_SUPPORT
#if defined MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE(MMICONNECTION_GPRS_SWITCH_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleGprsSwitchWinMsg),
    WIN_ID(MMICONNECTION_GPRS_SWITCH_WIN_ID),
    WIN_TITLE(TXT_GPRS),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_GPRSSWITCH_LIST_CTRL_ID),
    END_WIN
};

#endif
#endif

#ifdef MMI_GPRS_DATA_SIM_SET
WINDOW_TABLE( MMICONNECTION_GPRS_MODE_WIN_TAB ) =
{
    WIN_TITLE( TXT_CONNECTION_GPRS_MODE_SETTING ),
    WIN_FUNC((uint32) HandleGPRSModeSettingWinMsg ),
    WIN_ID( MMICONNECTION_GPRS_MODE_WIN_ID ),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_GPRS_MODE_CTRL_ID),
    END_WIN
};
#endif

#ifdef DATA_ROAMING_SUPPORT
WINDOW_TABLE( MMICONNECTION_GPRS_DATA_ROAMING_WIN_TAB ) =
{
    WIN_TITLE( STR_GPRS_LIST_ROAMING_EXT02 ),
        WIN_FUNC((uint32) HandleGPRSDataRoamingSettingWinMsg ),
        WIN_ID( MMICONNECTION_GPRS_DATA_ROAMING_WIN_ID ),
        WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_GPRS_DATA_ROAMING_CTRL_ID),
        END_WIN
};
#endif

//lulifei add the network type
WINDOW_TABLE( MMICONNECTION_NETWORK_TYPE_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleConnectionMenuNetworkTypeWin ),
    WIN_ID( MMICONNECTION_NETWORK_TYPE_WIN_ID ),
    ///WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_NETWORK_TYPE_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMICONNECTION_NETWORK_SETTING_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleConnectionMenuNetworkSettingWin ),
    WIN_ID( MMICONNECTION_NETWORK_SETTING_WIN_ID ),
    ///WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICONNECTION_NETWORK_SETTING_CTRL_ID),
    END_WIN
};
//lulifei add for vpdafone
WINDOW_TABLE( MMICONNECTION_VODAFONE_SETTING_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleVodafoneSettingWinMsg ),
    WIN_ID( MMICONNECTION_VODAFONE_SETTINGS_WIN_ID ),
    ///WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_VODAFONE_SETTINGS_CTRL_ID),
    END_WIN
};


//end
//add for gprs requirement
WINDOW_TABLE( MMISET_GPRS_CR_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetGprsCrWindow ),
    WIN_ID( MMISET_GPRS_CR_WIN_ID ),
    ///WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_GPRS_CR_CTRL_ID),
    END_WIN
};
//end
WINDOW_TABLE( MMICONNECTION_VODAFONE_WIN_TAB ) =
{
    WIN_TITLE( TXT_NET_VODAFONE_1 ),
    WIN_FUNC((uint32) HandleVodafoneSwitchWinMsg ),
    WIN_ID( MMICONNECTION_VODAFONE_WIN_ID ),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_VODAFONE_CTRL_ID),
    END_WIN
};

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :append one line icon text list item
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void AppendOneLineTextListItem(
                                     MMI_CTRL_ID_T       ctrl_id,
                                     GUIITEM_STYLE_E     item_style,
                                     MMI_TEXT_ID_T       left_softkey_id,
                                     MMI_TEXT_ID_T       mid_softkey_id,
                                     MMI_IMAGE_ID_T      icon_id,
                                     wchar*              wstr_ptr,
                                     uint16              wstr_len,
                                     uint16              pos,
                                     BOOLEAN             is_update
                                     )
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16                  i         = 0;

    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;

    item_data.softkey_id[0] = left_softkey_id;
//#ifdef MMI_GUI_STYLE_MINISCREEN
//	item_data.softkey_id[1] = IMAGE_NULL ;
//#else
	item_data.softkey_id[1] = mid_softkey_id;
//#endif
	item_data.softkey_id[2] = STXT_RETURN;

    if( 0 != icon_id )
    {
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = icon_id;
        i++;
    }

    item_data.item_content[i].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[i].item_data.text_buffer.wstr_len = wstr_len;

    item_data.item_content[i].item_data.text_buffer.wstr_ptr = wstr_ptr;

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
//  Description :
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ChangeAppSelectedIndex(MN_DUAL_SYS_E dual_sys, uint16 list_index, MMICONNECTION_NOTIFY_EVENT_E event)
{
    //���ø���APPӦ���ṩ�ĺ���
    MMICONNECTION_NOTIFY_LIST_T *cursor = PNULL;
    SCI_TRACE_LOW("[szp]:ChangeAppSelectedIndex list_index = %d",list_index);
    cursor = s_subscriber_list_ptr;
    while (PNULL != cursor)
    {
        cursor->notify_info.notify_func(event, list_index, dual_sys);
        cursor = cursor->next;
    }
}

#ifdef MMI_GPRS_SUPPORT
/****************************************************************************/
// 	Description : translate the setting item to richtext item, and add it to richtext control
//	Global resource dependence : none
//  Author: bei.wu
//	Note:
/*****************************************************************************/
LOCAL void SetSettingDetailEntry(MMI_CTRL_ID_T ctrl_id, MMICONNECTION_LINKSETTING_DETAIL_T* item)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    uint16 index = 0;
    MMI_STRING_T  dststring={0};
#ifndef MMI_DUALMODE_ENABLE
    CONNECTION_MMI_TEXT_ID_E  text_id[4] = {TXT_CONNECTION_AUTHTYPE_PAP, TXT_CONNECTION_AUTHTYPE_CHAP, TXT_CONNECTION_AUTHTYPE_PAPORCHAP, TXT_NONE};/*lint !e64*/
#endif
    wchar ucs2buf[MMICONNECTION_HOMEPAGE_MAX_URL_LEN+1] = {0};
    uint8 port_str[MMICONNECTION_MAX_PORT_LEN+2] = {0};
    uint32 port_str_len = 0;
    MMI_STRING_T        colon ={0};
    wchar colon_char[2] = {':', 0};

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;

    colon.wstr_ptr = colon_char;
    colon.wstr_len = 1;
#ifdef MMI_DUALMODE_ENABLE
    //�Ƿ����
    MMIAPICOM_CatIDAndString(&dststring,TXT_CONNECTION_AUTH_TYPE, &colon);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if(PNULL != dststring.wstr_ptr)
    {
        SCI_FREE(dststring.wstr_ptr);
        dststring.wstr_ptr = PNULL;
    }

    if(item->auth_type > MN_CHAP)/*lint !e685*/
    {
        item->auth_type = MN_PAP;
    }
    MMI_GetLabelTextByLang(text_id[item->auth_type],&dststring);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
#endif

    //�û���
    MMIAPICOM_CatIDAndString(&dststring, TXT_COMMON_USERNAME, &colon);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if(PNULL != dststring.wstr_ptr)
    {
        SCI_FREE(dststring.wstr_ptr);
        dststring.wstr_ptr = PNULL;
    }

    MMIAPICOM_StrToWstr((uint8 *)item->username, ucs2buf);
    item_data.text_data.buf.len = item->username_len;//MIN(item->username_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = ucs2buf;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //����
    MMIAPICOM_CatIDAndString(&dststring, TXT_COMMON_PASSWORD, &colon);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if(PNULL != dststring.wstr_ptr)
    {
        SCI_FREE(dststring.wstr_ptr);
        dststring.wstr_ptr = PNULL;
    }

    MMIAPICOM_StrToWstr((uint8 *)item->password, ucs2buf);
    item_data.text_data.buf.len = item->password_len;//MIN(item->password_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = ucs2buf;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //�����
    MMIAPICOM_CatIDAndString(&dststring, TXT_CONNECTION_APN, &colon);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if(PNULL != dststring.wstr_ptr)
    {
        SCI_FREE(dststring.wstr_ptr);
        dststring.wstr_ptr = PNULL;
    }

    MMIAPICOM_StrToWstr((uint8 *)item->apn, ucs2buf);
    item_data.text_data.buf.len = item->apn_len;//MIN(item->apn_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = ucs2buf;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //����
    MMIAPICOM_CatIDAndString(&dststring, TXT_COMMON_GATEWAY, &colon);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //MMIAPICOM_StrToWstr((uint8 *)item->gateway, ucs2buf);
    SCI_MEMSET(ucs2buf, 0, sizeof(ucs2buf));
    GUI_UTF8ToWstr(ucs2buf, MMICONNECTION_HOMEPAGE_MAX_URL_LEN, (const uint8 *)item->gateway, strlen((char *)item->gateway));

    item_data.text_data.buf.len = item->gateway_len;//MIN(item->gateway_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = ucs2buf;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if(PNULL != dststring.wstr_ptr)
    {
        SCI_FREE(dststring.wstr_ptr);
        dststring.wstr_ptr = PNULL;
    }

    //�˿�
    MMIAPICOM_CatIDAndString(&dststring, TXT_CONNECTION_PORT, &colon);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if(PNULL != dststring.wstr_ptr)
    {
        SCI_FREE(dststring.wstr_ptr);
        dststring.wstr_ptr = PNULL;
    }

    SCI_MEMSET(ucs2buf, 0, sizeof(ucs2buf));
    _snprintf((char *)port_str, (MMICONNECTION_MAX_PORT_LEN+1),"%d", item->port);
    MMIAPICOM_StrToWstr((uint8 *)port_str, ucs2buf);
    port_str_len = strlen((char *)port_str);
    item_data.text_data.buf.len = MIN(port_str_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = ucs2buf;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //������������
    MMIAPICOM_CatIDAndString(&dststring, TXT_CONNECTION_CHANGENAME, &colon);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if(PNULL != dststring.wstr_ptr)
    {
        SCI_FREE(dststring.wstr_ptr);
        dststring.wstr_ptr = PNULL;
    }

    item_data.text_data.buf.len = item->name_len;//MIN(item->name_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item->name;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //��ҳ���߲�������
    MMIAPICOM_CatIDAndString(&dststring, TXT_CONNECTION_SETHOMEPAGE, &colon);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if(PNULL != dststring.wstr_ptr)
    {
        SCI_FREE(dststring.wstr_ptr);
        dststring.wstr_ptr = PNULL;
    }

    SCI_MEMSET(ucs2buf, 0, sizeof(ucs2buf));
    MMIAPICOM_StrToWstr((uint8 *)item->homepage, ucs2buf);
    dststring.wstr_ptr = ucs2buf;
    dststring.wstr_len = item->homepage_len;
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //��������
    MMIAPICOM_CatIDAndString(&dststring, TXT_NETWORK_TYPE, &colon);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if(PNULL != dststring.wstr_ptr)
    {
        SCI_FREE(dststring.wstr_ptr);
        dststring.wstr_ptr = PNULL;
    }

    if(item->access_type)
    {
        MMI_GetLabelTextByLang(TXT_CONNECTION_NETTYPE_HTTP, &dststring);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CONNECTION_NETTYPE_WAP, &dststring);
    }
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //DNS
    MMIAPICOM_CatIDAndString(&dststring, TXT_CONNECTION_DNS, &colon);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if(PNULL != dststring.wstr_ptr)
    {
        SCI_FREE(dststring.wstr_ptr);
        dststring.wstr_ptr = PNULL;
    }

    SCI_MEMSET(ucs2buf, 0, sizeof(ucs2buf));
    MMIAPICOM_StrToWstr((uint8 *)item->dns, ucs2buf);
    dststring.wstr_ptr = ucs2buf;
    dststring.wstr_len = SCI_STRLEN((char *)item->dns);
    item_data.text_data.buf.len = MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
}
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description : 1 line list
//	Global resource dependence :
//  Author: lifei.lu
//	Note:
/*****************************************************************************/
LOCAL void AppendPdaConnectionItem1ToggleLine(
                                 MMI_CTRL_ID_T  ctrl_id,    //�ؼ�ID
                                 MMI_TEXT_ID_T  text_1,     //���ı�
                                 uint32         user_data   //�û�����
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;
    item_data.softkey_id[0] = NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_CHANGE;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
// 	Description : 1 line list
//	Global resource dependence :
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void AppendPdaConnectionItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //�ؼ�ID
                                 MMI_TEXT_ID_T  text_1,     //���ı�
                                 uint32         user_data   //�û�����
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : 2 line list
//  Global resource dependence :
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void AppendPdaConnectionItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //�ؼ�ID
                                 MMI_TEXT_ID_T  text_1,     //���ı�
                                 MMI_STRING_T  text_str,     //���ı�
                                 MMI_IMAGE_ID_T image_id,   //ͼ��ID
                                 uint32         user_data   //�û�����
                                 )
{
    THEMELIST_ITEM_STYLE_T  *pitem_style = PNULL;
    THEMELIST_ITEM_STYLE_T  my_style1 = {0};
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar temp_wstr[MMICONNECT_STRNUM_MAX_LEN + 2] = {0};
    wchar temp_len = 0;
    wchar *temp_wstr_ptr = PNULL;
    wchar *malloc_wstr_ptr = PNULL;
    uint32 i = GUILIST_GetTotalItemNum(ctrl_id);

    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        temp_wstr_ptr = temp_wstr;
        temp_len = text_str.wstr_len;
        /*����ִ�̫����̬����*/
        if(text_str.wstr_len > MMICONNECT_STRNUM_MAX_LEN)
        {
            malloc_wstr_ptr = SCI_ALLOC_APPZ(text_str.wstr_len*sizeof(wchar));
            if(PNULL != malloc_wstr_ptr)
            {
                temp_wstr_ptr = malloc_wstr_ptr;
            }
            else
            {
                /*������벻��������ʹ��Ĭ�ϵģ�ȡmin*/
                temp_len= MIN(text_str.wstr_len,MMICONNECT_STRNUM_MAX_LEN);
            }
        }

        SCI_MEMSET(temp_wstr_ptr, 0x00, (temp_len*sizeof(wchar)));

        MMIAPICOM_Wstrncpy(temp_wstr_ptr,
            text_str.wstr_ptr,
            temp_len);
        temp_len = text_str.wstr_len;
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;

        pitem_style = THEMELIST_GetStyle(item_t.item_style);
        if (pitem_style)
        {
            SCI_MEMCPY(&s_list_style, pitem_style, sizeof(THEMELIST_ITEM_STYLE_T));
            s_list_style.content[2].state = GUIITEM_CONTENT_STATE_NONE;
            s_list_style.main_index = 2;
        }
    }
    else
    {
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    }

    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    //text 2
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr_ptr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = temp_len;
    }

    //anim
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = image_id;

    GUILIST_AppendItem(ctrl_id, &item_t);
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        GUILIST_SetItemStyleEx(ctrl_id, i, &s_list_style);
    }

    if(PNULL != malloc_wstr_ptr)
    {
        SCI_FREE(malloc_wstr_ptr);
        malloc_wstr_ptr = PNULL;
    }
}


/*****************************************************************************/
//  Description : update 2 line list
//  Global resource dependence :
//  Author: lifei.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdatePdaConnectionItem2Line(
                                 MMI_STRING_T  text_str    //���ı�
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar temp_wstr[MMICONNECT_STRNUM_MAX_LEN + 2] = {0};
    wchar temp_len = 0;
    wchar *temp_wstr_ptr = PNULL;
    wchar *malloc_wstr_ptr = PNULL;
    MMI_CTRL_ID_T  ctrl_id;
    MMI_TEXT_ID_T  text_1;
    uint16 pos= 0;

        text_1=STR_SETT_LIST_NET_TYPE_EXT01;
#if defined(MMI_MULTI_SIM_SYS_DUAL)
    ctrl_id = MMICONNECTION_MENU_CTRL_ID;
    pos = 4;
#else
    ctrl_id = MMICONNECTION_NETWORK_SETTING_CTRL_ID;
    pos = 1;
#endif
        temp_wstr_ptr = temp_wstr;
        temp_len = text_str.wstr_len;
        /*����ִ�̫����̬����*/
        if(text_str.wstr_len > MMICONNECT_STRNUM_MAX_LEN)
        {
            malloc_wstr_ptr = SCI_ALLOC_APPZ(text_str.wstr_len*sizeof(wchar));
            if(PNULL != malloc_wstr_ptr)
            {
                temp_wstr_ptr = malloc_wstr_ptr;
            }
            else
            {
                /*������벻��������ʹ��Ĭ�ϵģ�ȡmin*/
                temp_len= MIN(text_str.wstr_len,MMICONNECT_STRNUM_MAX_LEN);
            }
        }

        SCI_MEMSET(temp_wstr_ptr, 0x00, (temp_len*sizeof(wchar)));

        MMIAPICOM_Wstrncpy(temp_wstr_ptr,
            text_str.wstr_ptr,
            temp_len);
        temp_len = text_str.wstr_len;
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
        item_t.item_data_ptr = &item_data;
        item_t.user_data     = ID_CONNECTION_NETWORK_SETTING_TYPE;
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    //text 2
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr_ptr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = temp_len;
    }else{
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[2].item_data.text_id = STR_DETAILS_EMPTY_EXT01;
    }
    item_data.item_content[2].is_opacity = TRUE;
    GUILIST_SetItemContentOpacityFontColor(&item_data,2,255*0.6,255*0.6);
    item_data.softkey_id[0] = NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;

    GUILIST_ReplaceItem(ctrl_id, &item_t,pos);
    if(PNULL != malloc_wstr_ptr)
    {
        SCI_FREE(malloc_wstr_ptr);
        malloc_wstr_ptr = PNULL;
    }
}

#ifdef MMI_GPRS_SUPPORT
/**************************************************************************************************************/
//  Description : �˽ӿ����ж��ܷ���������ʻ�����ǰ���ſ����Խ���,�ж�����Ϊ:
//  ������ģʽ���޿������رգ�puk ��,pin����reject������֧�ֶ�������������ʻ�
//  Param in: ok_idx: ����ǰok_num ��1����ok_idx����洢���ſ���ok�ģ��������Ϊ��Чֵ
//                  ok_num:��ǰ�����ſ�Ϊok ��
//  Note: ok_idxֻ����ok_num Ϊ1��ʱ������Ч�����������Ч��
/**************************************************************************************************************/
LOCAL void GetSimStatusforAcessNetWorkAccount(uint16* ok_idx,uint16 * ok_num)
{
    uint16 sim_ok_num = 0;
    uint16 sim_ok = 0;
    uint16 sim_available_num = 0;
    uint16 sim_sys = 0;
    
    if((PNULL == ok_idx) || (PNULL == ok_num))
    {
        SCI_TRACE_LOW("GetSimStatusforAcessNetWorkAccount,ok_idx is null!");
        return ;
    }
    sim_available_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys,1);
    SCI_TRACE_LOW("GetSimStatusforAcessNetWorkAccount,sim_available_num:%d\n",sim_available_num);
    if(0 == sim_available_num)
    {
        *ok_num = 0;
        SCI_TRACE_LOW("GetSimStatusforAcessNetWorkAccount,sim_available_num is 0!,can not enter win!\n");
    }
    else if(1 == sim_available_num)
    {
        if(TRUE == MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)sim_sys))
        {
            *ok_num = 1;
            *ok_idx = sim_sys;
        }
        else
        {
            *ok_num = 0;
            SCI_TRACE_LOW("SimIsOkforAcessNetWorkAccount,sim_available_num is 1,but sim card is not ok!,can not enter win!\n");
        }
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else
    {
        sim_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
        if(1 == sim_ok_num)
        {
            *ok_idx = sim_ok;
        }
        else
        {
            SCI_TRACE_LOW("SimIsOkforAcessNetWorkAccount,sim_ok_num:%d\n",sim_ok_num);
        }
        *ok_num = sim_ok_num;
    }
#endif
}

/*****************************************************************************/
// 	Description : Connection menu
//	Global resource dependence :
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void  InitPdaConnectionMainListCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMICONNECTION_MENU_CTRL_ID;
    MMI_STRING_T text_str= {0};
    MMI_STRING_T	temp_str = {0};
    MMICONNECTION_GPRS_SETTINGS_E gprs_settings = MMICONNECTION_GPRS_ATTACH_ONNEED;
    MMI_TEXT_ID_T    text_id = TXT_NULL;
#ifndef  MMI_DUALMODE_ENABLE
    uint16 dual_sys = MN_DUAL_SYS_1;
#endif
    uint32 sim_num                        = 0;
    MN_PHONE_SELECT_PLMN_T  sel_net_mode    =   {MN_PHONE_PLMN_SELECT_MANUAL_MODE, 0};/*lint !e785*/
    uint16 index = 0;
    GUILIST_POS_INFO_T pos_info = {0};
    NETWORK_TYPE_E  last_network_type  = GetNetworkType();
    wchar sim1_vodafone[64] = {0};
    uint8 ifSim1Vodafone = 0;
    wchar sim2_vodafone[64] = {0};
    uint8 ifSim2Vodafone = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    uint8 max_list_item = 0;
#ifdef MMI_GPRS_SWITCH_SUPPORT
    MMI_IMAGE_ID_T	image_switch_id = NULL;
    MMICONNECTION_SETTING_GPRS_SWITCH_E gprs_switch_status = MMICONNECTION_SETTING_GPRS_SWITCH_ON;
#endif
#ifdef DATA_ROAMING_SUPPORT
    MMICONNECTION_SETTING_DATA_ROAMING_E data_roaming_status = MMAPIICONNECTION_Setting_GetDataRoamingStatus();
    MMI_IMAGE_ID_T image_id = NULL;
#endif
#ifdef MMI_GPRS_SUPPORT
    uint16 sim_ok_idx = 0;
    uint16 sim_ok_num = 0;
    BOOLEAN is_gray = FALSE;
#endif

    return_value = EFS_NvitemRead(TEL_NV_NETWORK_TYPE_SELECT,sizeof(uint8), &s_network_type_select);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //#ifdef WIN32
        s_network_type_select = 3;
        // #else
        //          s_network_type_select = 2;
        // #endif
    }

    GUILIST_GetCurPosInfo(ctrl_id, &pos_info);
    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetMaxItem(ctrl_id, MMI_CONNECTION_MENU_LIST_TYPE_MAX, FALSE);

    ifSim1Vodafone = MMIAPIPHONE_GetVodafoneSimSelectResult(MN_DUAL_SYS_1,  sim1_vodafone);
#if defined(MMI_MULTI_SIM_SYS_DUAL)
    ifSim2Vodafone = MMIAPIPHONE_GetVodafoneSimSelectResult(MN_DUAL_SYS_2,  sim2_vodafone);
    if ((ifSim1Vodafone != 0) || (ifSim2Vodafone!=0))
    {
        InitVodafoneApn();
        if (ifSim1Vodafone != 0)
        {
            InitVodafoneApnList(MN_DUAL_SYS_1, ifSim1Vodafone - 1);
        }
        if (ifSim2Vodafone != 0)
        {
            InitVodafoneApnList(MN_DUAL_SYS_2, ifSim2Vodafone - 1);
        }
    }
#else
    if(ifSim1Vodafone != 0)
    {
        InitVodafoneApn();
        InitVodafoneApnList(MN_DUAL_SYS_1,ifSim1Vodafone-1);
    }
#endif

#ifdef MMI_GPRS_SUPPORT
    AppendPdaConnectionItem1Line(            
    ctrl_id,
    TXT_COMMON_WWW_ACCOUNT,
    (uint32)MMI_CONNECTION_MENU_LIST_GPRS_NET_USERID
    );
    GetSimStatusforAcessNetWorkAccount(&sim_ok_idx,&sim_ok_num);
    if(0 == sim_ok_num)
    {
        is_gray = TRUE;
    }
    else
    {
        is_gray = FALSE;
    }
    GUILIST_SetItemGray(ctrl_id, index, is_gray);
    GUILIST_SetItemInvalid(ctrl_id, index, is_gray);
    index++;
#ifdef MMI_GPRS_SWITCH_SUPPORT
    gprs_switch_status = MMIAPICONNECTION_GetGPRSSwitchStatus();
    sim_num = MMIAPIPHONE_GetSimAvailableNum(PNULL, 0);

    if(0 == sim_num)
    {
        image_switch_id = IMAGE_CONTROL_LIST_CHECK_DISABLED;
    }
    else if(gprs_switch_status == MMICONNECTION_SETTING_GPRS_SWITCH_ON)
    {
        image_switch_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        image_switch_id = IMAGE_CHECK_UNSELECTED_ICON;
    }
    SCI_MEMSET(&text_str,0,sizeof(MMI_STRING_T));

    AppendPdaConnectionItem2Line(ctrl_id,
                                 TXT_GPRS_SERVICE,
                                 text_str,
                                 image_switch_id,
                                 MMI_CONNECTION_MENU_LIST_GPRS_SWITCH);
    // �����������ã�����Ҫ�����������û��û����ɲ���
    if(0 == sim_num)
    {
        GUILIST_SetItemGray(ctrl_id, index, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, index, TRUE);
    }
    else
    {
        GUILIST_SetItemGray(ctrl_id, index, FALSE);
        GUILIST_SetItemInvalid(ctrl_id, index, FALSE);
    }
    index++;
#endif

//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
    if(data_roaming_status == MMICONNECTION_SETTING_DATA_ROAMING_ENABLE)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }
    text_id = TXT_CONNECTION_DATA_ROAMING_TIP;
    MMI_GetLabelTextByLang(text_id, &text_str);
    AppendPdaConnectionItem2Line(ctrl_id,
                                 TXT_CONNECTION_DATA_ROAMING,
                                 text_str,
                                 image_id,
                                 MMI_CONNECTION_MENU_LIST_GPRS_ROAMING);

    index++;

// #ifndef MMI_MULTI_SIM_SYS_DUAL
//     text_str.wstr_ptr = sim1_vodafone;
//     text_str.wstr_len = MMIAPICOM_Wstrlen(text_str.wstr_ptr);
//     AppendPdaConnectionItem2Line(
//         ctrl_id,
//         STR_VODAFONE_EXT01,
//         text_str,
//         IMAGE_COMMON_BACK,
//         (uint32)MMI_CONNECTION_MENU_LIST_VODAFONE
//         );
//     index++;
// #endif

#endif
#endif

    SCI_MEMSET(&text_str, 0x00, sizeof(text_str));

#ifdef  MMI_DUALMODE_ENABLE
    AppendPdaConnectionItem1Line(ctrl_id, TXT_NETWORK_SELECT, (uint32)MMI_CONNECTION_MENU_LIST_NETSETTING);
    index++;
#else
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&dual_sys, 1);  
#if 0	
    if(1 == sim_num )
    {
        MNPHONE_GetPlmnSelectModeEx((MN_DUAL_SYS_E)dual_sys, &sel_net_mode);
        switch (sel_net_mode.select_mode)
        {
        case MN_PHONE_PLMN_SELECT_AUTO_MODE:
            text_id = TXT_COMMON_AUTO_SELECT;
            break;
            
        case MN_PHONE_PLMN_SELECT_MANUAL_MODE:
            text_id = TXT_SET_MANUAL_SEL;
            break;
            
        default:     
            text_id = TXT_NULL;
            break;
        }
        
        MMI_GetLabelTextByLang(text_id, &text_str);    
        AppendPdaConnectionItem2Line(
            ctrl_id, 
            TXT_NETWORK_SELECT,
            text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)MMI_CONNECTION_MENU_LIST_NETWORK
            );
        index++;
    }
    else
#endif
    {
        AppendPdaConnectionItem1Line(
            ctrl_id,
            TXT_NETWORK_SELECT,
            (uint32)MMI_CONNECTION_MENU_LIST_NETWORK
            );
        if (MMIAPISET_GetFlyMode() || (0 == sim_num))
        {
            GUILIST_SetItemGray(ctrl_id, index, TRUE);
            GUILIST_SetItemInvalid(ctrl_id, index, TRUE);
        }
        else
        {
            GUILIST_SetItemGray(ctrl_id, index, FALSE);
            GUILIST_SetItemInvalid(ctrl_id, index, FALSE);
        }
        index++;
    }
#endif
#ifndef MMI_ONLY_4G_DEL_2G_CUSTOM
    AppendPdaConnectionItem1Line(            
        ctrl_id,
        TXT_NETWORK_TYPE,//network account
        (uint32)MMI_CONNECTION_MENU_LIST_ACCESS_NETWORK_TYPE/*MMI_CONNECTION_MENU_LIST_NETWORK_TYPE*/
        );
    if (MMIAPISET_GetFlyMode() || (0 == sim_num))
    {
        GUILIST_SetItemGray(ctrl_id, index, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, index, TRUE);
    }
    else
    {
        GUILIST_SetItemGray(ctrl_id, index, FALSE);
        GUILIST_SetItemInvalid(ctrl_id, index, FALSE);
    }
    index++;
#endif

#ifdef MMI_PREFER_PLMN_SUPPORT
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str));
    
    AppendPdaConnectionItem2Line(
        ctrl_id, 
        TXT_SET_PREFER_NETWORK,
        text_str,
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMI_CONNECTION_MENU_LIST_GPRS_PREFER_NETWORK
        );
    
    index++;
#endif
    
#ifdef MMI_GPRS_DATA_SIM_SET
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str));
    if (MMIAPIPHONE_GetSimExistedNum(PNULL, 0) > 1)
    {
        AppendPdaConnectionItem2Line(ctrl_id, TXT_CONNECTION_GPRS_MODE_SETTING, text_str, IMAGE_COMMON_RIGHT_ARROW, (uint32)MMI_CONNECTION_MENU_LIST_GPRS_MODE);
    }
    
    index++;
#endif

    if(MMICONNECTION_HasVodafoneSIMcard())
    {
        SCI_MEMSET(&text_str, 0x00, sizeof(text_str));
        AppendPdaConnectionItem2Line(ctrl_id, TXT_NET_VODAFONE_1, text_str, IMAGE_COMMON_RIGHT_ARROW, (uint32)MMI_CONNECTION_SET_VODAFONE_PAY);
        index++;
    }

    GUILIST_SetCurPosInfo(ctrl_id, &pos_info);
}

/*****************************************************************************/
// 	Description : Connection long ok menu
//	Global resource dependence :
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingListLongOKMenuWinMsg(
									 MMI_WIN_ID_T win_id,
									 MMI_MESSAGE_ID_E msg_id,
									 DPARAM param)
{

    MMI_RESULT_E recode = MMI_RESULT_TRUE;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    uint32          group_id = 0, menu_id = 0;
#endif
    uint16           i = 0;
    MN_DUAL_SYS_E dual_sys = s_connection_handling_sim_id;
    uint16          list_index = 0;
    uint16          apn_index = 0;
    //SCI_TRACE_LOW:"SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_1626_112_2_18_2_12_8_0,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            const GUILIST_ITEM_T *list_item = NULL;
            MMI_STRING_T title_text = {0};
            list_item = GUILIST_GetItemPtrByIndex(MMICONNECTION_LIST_SETTING_CTRL_ID, GUILIST_GetCurItemIndex( MMICONNECTION_LIST_SETTING_CTRL_ID));
            if(list_item && list_item->item_data_ptr)
            {
                title_text  = list_item->item_data_ptr->item_content[1].item_data.text_buffer;
            }
            GUIMENU_SetMenuTitle(&title_text, MMICONNECTION_MENU_SETTINGLINK_LONGOK_CTRL_ID);
            MMK_SetAtvCtrl(win_id, MMICONNECTION_MENU_SETTINGLINK_LONGOK_CTRL_ID);
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        GUIMENU_GetId(MMICONNECTION_MENU_SETTINGLINK_LONGOK_CTRL_ID,&group_id,&menu_id);
        switch(menu_id)
        {
        case ID_CONNECTION_SETTING_DELETE:
            SCI_TRACE_LOW("[szp]:HandleSettingListLongOKMenuWinMsg total = %d, invisible = %d",s_connection_setting_info.settinglink_num[dual_sys], s_connection_setting_info.invisible_apn_num[dual_sys]);
            i = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SETTING_CTRL_ID);
            apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, i);
            if(!s_connection_setting_info.setting_detail[dual_sys][apn_index].is_customed_detail)
            {
                 if (1 == (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys]))
                 {
                     MMIPUB_OpenAlertWarningWin(TXT_CONNECTION_DELETE_FAIL);
                     MMK_CloseWin(win_id);//������һ��
                 }
                 else
                 {
					MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                 }
            }
            break;

        default:
            break;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_PROMPTWIN_OK:                                    //delete
        #if 0
        if (1 == s_connection_setting_info.settinglink_num[dual_sys])
        {
            MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
            break;
        }
        #endif
        list_index = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SETTING_CTRL_ID);
        apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, list_index);
        if ((apn_index < s_connection_setting_info.settinglink_num[dual_sys])
            && (0 < (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys])))
        {
            for(i = (uint32)apn_index; i < (uint32)(s_connection_setting_info.settinglink_num[dual_sys]-1); i++)
            {
                SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][i],&s_connection_setting_info.setting_detail[dual_sys][i+1],sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            }
            SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][s_connection_setting_info.settinglink_num[dual_sys] - 1],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys]--;
        }
        else if ((apn_index == s_connection_setting_info.settinglink_num[dual_sys]) 
                 && (0 < (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys])))
        {
            SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][s_connection_setting_info.settinglink_num[dual_sys] - 1],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys]--;
        }
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO,&s_connection_setting_info);

        //����Ӧ��ѡ�е�ITEM
        ChangeAppSelectedIndex(dual_sys, apn_index, MMICONNECTION_NOTIFY_EVENT_DEL_ITEM);

        GUILIST_RemoveItem(MMICONNECTION_LIST_SETTING_CTRL_ID, list_index);
        MMK_CloseWin(win_id);
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;

}

/*****************************************************************************/
// 	Description : Connection long ok menu,for mms,browser,dcd,java..
//	Global resource dependence :
//  Author: jixin.xu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLinkListLongOKMenuWinMsg(
									 MMI_WIN_ID_T win_id,
									 MMI_MESSAGE_ID_E msg_id,
									 DPARAM param)
{

    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32          group_id = 0, menu_id = 0;
    uint16           i = 0;
    MN_DUAL_SYS_E dual_sys = s_connection_handling_sim_id;
    uint16          list_index = 0;
    uint16          apn_index = 0;
    //SCI_TRACE_LOW:"[MMICONNECTION]: HandleLinkListLongOKMenuWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_1742_112_2_18_2_12_9_1,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            const GUILIST_ITEM_T *list_item = NULL;
            MMI_STRING_T title_text = {0};
            list_item = GUILIST_GetItemPtrByIndex(MMICONNECTION_LIST_SETTING_CTRL_ID, GUILIST_GetCurItemIndex( MMICONNECTION_LIST_SETTING_CTRL_ID));
            if(list_item && list_item->item_data_ptr)
            {
                title_text  = list_item->item_data_ptr->item_content[1].item_data.text_buffer;
            }
            GUIMENU_SetMenuTitle(&title_text, MMICONNECTION_MENU_SETTINGLINK_LONGOK_CTRL_ID);
            MMK_SetAtvCtrl(win_id, MMICONNECTION_MENU_SETTINGLINK_LONGOK_CTRL_ID);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(MMICONNECTION_MENU_SETTINGLINK_LONGOK_CTRL_ID,&group_id,&menu_id);
        switch(menu_id)
        {
        case ID_CONNECTION_SETTING_DELETE:
            SCI_TRACE_LOW("[szp]:HandleLinkListLongOKMenuWinMsg total = %d, invisible = %d",s_connection_setting_info.settinglink_num[dual_sys], s_connection_setting_info.invisible_apn_num[dual_sys]);
            i = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SETTING_CTRL_ID);
            apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, i);
            if(!s_connection_setting_info.setting_detail[dual_sys][apn_index].is_customed_detail)
            {
                if (1 == (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys]))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_CONNECTION_DELETE_FAIL);
                    MMK_CloseWin(win_id);//������һ��
                }
                else
                {
					MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                }
            }
            break;

        case ID_CONNECTION_SETTING_EDIT:                                                                           //�༭һ��ѡ�е�����
            {
                uint32 temp_param=0;
                uint16 index= 0;
                MMICONNECTION_LINKSET_OP_E op = MMICONNECTION_LINKSET_OP_EDIT;
                temp_param = op<<16;
                index = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SETTING_CTRL_ID);
                temp_param+=index;
                //�����ò˵�����
                MMK_CreateWin((uint32*)MMICONNECTION_SETTINGLINKMENU_WIN_TAB, (ADD_DATA)temp_param);
                MMK_CloseWin(win_id);//������һ��
            }
            break;

        default:
            break;
    }
    break;

    case MSG_PROMPTWIN_OK:                                    //delete
        list_index = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SETTING_CTRL_ID);
        apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, list_index);
        if ((apn_index < s_connection_setting_info.settinglink_num[dual_sys]) 
            && (0 < (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys])))
        {
            for(i = (uint32)apn_index; i < (uint32)(s_connection_setting_info.settinglink_num[dual_sys]-1); i++)
            {
                SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][i],&s_connection_setting_info.setting_detail[dual_sys][i+1],sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            }
            SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][s_connection_setting_info.settinglink_num[dual_sys] - 1],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys]--;
        }
        else if ((apn_index == s_connection_setting_info.settinglink_num[dual_sys]) 
                 && (0 < (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys])))
        {
            SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][s_connection_setting_info.settinglink_num[dual_sys] - 1],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys]--;
        }
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO,&s_connection_setting_info);

        //����Ӧ��ѡ�е�ITEM
        ChangeAppSelectedIndex(dual_sys, apn_index, MMICONNECTION_NOTIFY_EVENT_DEL_ITEM);
        //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
        GUILIST_RemoveItem(MMICONNECTION_LIST_SETTING_CTRL_ID, list_index);
        MMK_CloseWin(win_id);
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
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
//  Description :HandleSetPowerManagerWindow
//  Global resource dependence :
//  Author:lifei.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleConnectionMenuNetworkTypeWin(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

#ifdef MMI_ORIG_UNISOC_SUPPORT
    MMI_TEXT_ID_T       str_id[] = {STR_NETWORK_3G_EXT01,STR_NETWORK_2G_EXT01,
		                                        STR_NETWORK_2G_ONLY_EXT01,STR_NETWORK_3G_ONLY_EXT01};
#else
    MMI_TEXT_ID_T       str_id[] = {STR_NETWORK_3G_EXT01,STR_NETWORK_2G_ONLY_EXT01,STR_NETWORK_3G_ONLY_EXT01};
    MMI_TEXT_ID_T       str_id_no_2g_only[] = {STR_NETWORK_3G_EXT01,STR_NETWORK_3G_ONLY_EXT01};
#endif

    int16               i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_T      item4_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    GUILIST_ITEM_DATA_T item4_data = {0};
    uint16  max_items = 0;
    uint16   cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMICONNECTION_NETWORK_TYPE_CTRL_ID;
    NETWORK_TYPE_E  cur_network_type = NETWORK_TYPE_GPRS_E;
#ifdef PRODUCT_DM
    MN_PHONE_RAT_INFO_T  rat_info = {0};/*lint !e64*/
#endif
    NETWORK_TYPE_E  last_network_type  = GetNetworkType();
    ERR_MNPHONE_CODE_E  ret  = ERR_MNPHONE_WRITE_ME_FAILURE;//@zhaohui,cr106675
    MMI_STRING_T  text_str_network_type = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

         MMK_SetAtvCtrl(win_id,ctrl_id);

         if(1 == s_network_type_select)
         {
             max_items = 2;
         }
         else if (3 == s_network_type_select)
         {
             // for Vodafone disable 2G only case
             max_items = 2;
         }
         else
         {
#ifdef MMI_ORIG_UNISOC_SUPPORT
             max_items = 4;
#else
             max_items = 3;
#endif
         }
         cur_item  = 0;
        //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
        GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        item_t.item_style    = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;

        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
        item_data.softkey_id[2] = STXT_RETURN;
        GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,RIGHT_BUTTON,TRUE);
        for(i = 0; i < max_items; i++)
        {
            //if(i==4)//the CONFSERVICE is tongle style
			//	continue;
            item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
            if (3 == s_network_type_select)
            {
                item_data.item_content[0].item_data.text_id = str_id_no_2g_only[i];
            }
            else
            {
            item_data.item_content[0].item_data.text_id = str_id[i];
            }
            if(!GUILIST_InsertItem(ctrl_id, &item_t, i))
            {
                break;
            }
        }
#ifdef MMI_ORIG_UNISOC_SUPPORT
        switch(last_network_type){
        case NETWORK_TYPE_GPRS_E:
            cur_item = 2;
            break;
        case NETWORK_TYPE_TD_E:
            cur_item = 3;
            break;
        case NETWORK_TYPE_DM_G_E:
            cur_item = 1;
            break;
        case NETWORK_TYPE_DM_E:
            cur_item = 0;
            break;
        default:
            break;
		}
#else
        if (3 == s_network_type_select)
        {
            switch(last_network_type)
            {
            case NETWORK_TYPE_TD_E:
                cur_item = 1;
                break;
            case NETWORK_TYPE_DM_E:
                cur_item = 0;
                break;
            default:
                break;
            }

        }
        else
        {
            switch(last_network_type)
            {
            case NETWORK_TYPE_GPRS_E:
                cur_item = 1;
                break;
            case NETWORK_TYPE_TD_E:
                cur_item = 2;
                break;
            case NETWORK_TYPE_DM_E:
                cur_item = 0;
                break;
            default:
                break;
            }
        }
#endif
#ifdef STATUS_TEXT_SUPPORT
        GUIWIN_SetStbItemText(MMICOM_WIN_TEXT_INFO,L"System");
#endif
        GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_item);
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
		break;
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_item = GUILIST_GetCurItemIndex(ctrl_id);
#ifdef PRODUCT_DM
#ifdef MMI_ORIG_UNISOC_SUPPORT
        switch(cur_item) {
        case 0:
            cur_network_type = NETWORK_TYPE_DM_E;
            rat_info.rat = MN_GMMREG_RAT_GPRS_AND_3G;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_3G;
            MMI_GetLabelTextByLang(str_id[0],&text_str_network_type);
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;
            
        case 1:
            cur_network_type = NETWORK_TYPE_DM_G_E;
            rat_info.rat = MN_GMMREG_RAT_GPRS_AND_3G;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_GPRS;
            MMI_GetLabelTextByLang(str_id[1],&text_str_network_type);
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;
            
        case 2:
            cur_network_type = NETWORK_TYPE_GPRS_E;
            rat_info.rat = MN_GMMREG_RAT_GPRS;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_UNKNOWN;
            MMI_GetLabelTextByLang(str_id[2],&text_str_network_type);
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;
            
        case 3:
            cur_network_type = NETWORK_TYPE_TD_E;
            rat_info.rat = MN_GMMREG_RAT_3G;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_UNKNOWN;
            MMI_GetLabelTextByLang(str_id[3],&text_str_network_type);
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;
            
        default:
            break;
        }
#else
        switch(cur_item)
        {
        case 0:
            cur_network_type = NETWORK_TYPE_DM_E;
            rat_info.rat = MN_GMMREG_RAT_GPRS_AND_3G;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_3G;
            MMI_GetLabelTextByLang(str_id[0],&text_str_network_type);
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;
            
        case 1:
            if(3 == s_network_type_select)
            {
                cur_network_type = NETWORK_TYPE_TD_E;
                rat_info.rat = MN_GMMREG_RAT_3G;
                rat_info.dm_prefer_rat = MN_GMMREG_RAT_UNKNOWN;
                MMI_GetLabelTextByLang(str_id[2],&text_str_network_type);
                //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            }
            else
            {
                cur_network_type = NETWORK_TYPE_GPRS_E;
                rat_info.rat = MN_GMMREG_RAT_GPRS;
                rat_info.dm_prefer_rat = MN_GMMREG_RAT_UNKNOWN;
                MMI_GetLabelTextByLang(str_id[1],&text_str_network_type);
                //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            }
            break;
            
        case 2:
            cur_network_type = NETWORK_TYPE_TD_E;
            rat_info.rat = MN_GMMREG_RAT_3G;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_UNKNOWN;
            MMI_GetLabelTextByLang(str_id[2],&text_str_network_type);
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;
            
        default:
            break;
        }
#endif
#ifndef _WIN32
        if (MMIAPISET_GetFlyMode())
        {
            ret = MNPHONE_SetRATEx(MN_DUAL_SYS_1, rat_info, FALSE, TRUE);
            MMISET_SetSwitchRatState(TRUE);
        }
        else if(last_network_type != cur_network_type)
        {
            ret = MNPHONE_SetRATEx(MN_DUAL_SYS_1, rat_info, FALSE, FALSE);
            MMISET_SetSwitchRatState(TRUE);
        }
        GUIWIN_UpdateStb();
        SCI_TRACE_LOW("Connection_SetNetworkType,  ret= %d, rat_info.rat=%d, rat_info.dm_prefer_rat=%d",ret,rat_info.rat,rat_info.dm_prefer_rat );
#endif
#endif
        MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
        UpdatePdaConnectionItem2Line(text_str_network_type);
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
//  Description :HandleNetworkSetting
//  Global resource dependence :
//  Author:lifei.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleConnectionMenuNetworkSettingWin(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
     MMI_TEXT_ID_T    text_id = TXT_NULL;
    MMI_STRING_T text_str= {0};
    uint16  max_items = 0;
    uint16   cur_item = 0;
    MMI_CTRL_ID_T       ctrl_id = MMICONNECTION_NETWORK_SETTING_CTRL_ID;
    NETWORK_TYPE_E  last_network_type  = GetNetworkType();
    uint32 menu_id = 0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        if(s_network_type_select == 0)
        {
            max_items = 6;
        }
        else
        {
            max_items = 7;
        }
        cur_item  = 0;
        GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        AppendPdaConnectionItem1Line(
            ctrl_id,
            STR_OPERATE_NETWORK_EXT01,
            (uint32)ID_CONNECTION_NETWORK_SETTING_OPERATOR
            );
        cur_item++;
        if(s_network_type_select != 0){
        #ifdef MMI_2G_4G_SUPPORT
            text_id = GetNetworkTypeStringId();
        #else			
            switch(last_network_type){
            case NETWORK_TYPE_GPRS_E:
                text_id = STR_NETWORK_2G_ONLY_EXT01;
                break;
            case NETWORK_TYPE_TD_E:
                text_id  = STR_NETWORK_3G_ONLY_EXT01;
                break;
            case NETWORK_TYPE_DM_G_E:
                text_id  = STR_NETWORK_2G_EXT01;
                break;
            case NETWORK_TYPE_DM_E:
                text_id = STR_NETWORK_3G_EXT01;
                break;
            default:
                text_id = STR_NETWORK_2G_ONLY_EXT01;
                break;
            }
        #endif		
            MMI_GetLabelTextByLang(text_id, &text_str);
            AppendPdaConnectionItem2Line(
                ctrl_id,
                STR_SETT_LIST_NET_TYPE_EXT01,
                text_str,
                IMAGE_NULL,
                ID_CONNECTION_NETWORK_SETTING_TYPE);
            cur_item++;
        }

	if(FALSE == MMIAPISET_IsDisableShareViaOption())
	{
        AppendPdaConnectionItem1ToggleLine(
            ctrl_id,
            STR_LIST_MOBILE_DATA,
            (uint32)ID_CONNECTION_NETWORK_SETTING_MOBILE_DATA
            );
        CTRLLIST_SetToggleState(ctrl_id,cur_item,(MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_ON)?TRUE:FALSE);
        cur_item++;
	}
				
        AppendPdaConnectionItem1ToggleLine(
            ctrl_id,
            STXT_PIN_CODE_CNTRL,
            (uint32)ID_CONNECTION_NETWORK_SETTING_PIN_REQUEST
            );
        CTRLLIST_SetToggleState(ctrl_id,cur_item,(MNSIM_IsPin1EnableEx(MN_DUAL_SYS_1))?TRUE:FALSE);
        cur_item++;
		
        AppendPdaConnectionItem1Line(
            ctrl_id,
            STXT_PIN_CHANGE,
            (uint32)ID_CONNECTION_NETWORK_SETTING_CHANGE_PIN
            );	
        cur_item++;
		 
        AppendPdaConnectionItem1Line(
            ctrl_id,
            STXT_PIN2_CHANGE,
            (uint32)ID_CONNECTION_NETWORK_SETTING_CHANGE_PIN2
            );
        cur_item++;
	  
#if defined(OMA_CP_SEC_SUPPORT)		
        AppendPdaConnectionItem1ToggleLine(
            ctrl_id,
            STR_USER_PIN_REQUEST,
            (uint32)ID_CONNECTION_NETWORK_SETTING_CHANGE_OTA
            );
            CTRLLIST_SetToggleState(ctrl_id,cur_item,(MMIAPISET_GetOTASecStatus(MN_DUAL_SYS_1) == TRUE)?TRUE:FALSE);
#endif		
        break;
    case MSG_FULL_PAINT:
        {
            MMI_STRING_T  text_str_network_type = {0};	

            text_id = GetNetworkTypeStringId();	
            MMI_GetLabelTextByLang(text_id,&text_str_network_type);
            UpdatePdaConnectionItem2Line(text_str_network_type);
            MMICONNECTION_UpdatePINToggle();
        }
		break;
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
        break;
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_item = GUILIST_GetCurItemIndex(ctrl_id);
	
       GUILIST_GetItemData(ctrl_id, cur_item, &menu_id);
        /*if(!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
        {
            MMIAPIPHONE_AlertSimNorOKStatusII(MN_DUAL_SYS_1);
	   break;
        }*/
        SCI_TRACE_LOW(" func [%s] line [%d] cur_item[%d] menu_id[%d]",__FUNCTION__,__LINE__,cur_item,menu_id);
        switch(menu_id) {
            case ID_CONNECTION_NETWORK_SETTING_OPERATOR :
                    MMIAPISET_OpenSelNetWorkWin(MN_DUAL_SYS_1);
	    break;
	    case ID_CONNECTION_NETWORK_SETTING_TYPE :
	        #ifdef MMI_2G_4G_SUPPORT
                 CreateNetworkTypeWin();	
	        #else
                 MMK_CreateWin((uint32 *)MMICONNECTION_NETWORK_TYPE_WIN_TAB, PNULL);
	        #endif
	    break;
	    case ID_CONNECTION_NETWORK_SETTING_MOBILE_DATA :
            if (MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_ON)
            {
                MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_OFF);
                CTRLLIST_SetToggleState(ctrl_id,cur_item,FALSE);
            }
            else
            {
                MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_ON);
                CTRLLIST_SetToggleState(ctrl_id,cur_item,TRUE);
            }
            break;
        case ID_CONNECTION_NETWORK_SETTING_PIN_REQUEST :
            if (MNSIM_IsPin1EnableEx(MN_DUAL_SYS_1))
            {
                MMIAPISET_SetSIMPinEnable(FALSE);
                //CTRLLIST_SetToggleState(ctrl_id,cur_item,FALSE);
            }
            else
            {
                MMIAPISET_SetSIMPinEnable(TRUE);
                //CTRLLIST_SetToggleState(ctrl_id,cur_item,TRUE);
            }
	    break;
	    case ID_CONNECTION_NETWORK_SETTING_CHANGE_PIN :
                 MMIAPISET_ChangeSIMPin();
	    break;
	    case ID_CONNECTION_NETWORK_SETTING_CHANGE_PIN2 :
                 MMISET_OpenPin2Win(MN_DUAL_SYS_1);
	    break;
#if defined(OMA_CP_SEC_SUPPORT)		
	    case ID_CONNECTION_NETWORK_SETTING_CHANGE_OTA :
            if (MMIAPISET_GetOTASecStatus(MN_DUAL_SYS_1) == TRUE)
            {
                MMIAPISET_SetOTASecStatus(0, FALSE);
                CTRLLIST_SetToggleState(ctrl_id,cur_item,FALSE);
            }
            else
            {
                MMIAPISET_SetOTASecStatus(0, TRUE);
                CTRLLIST_SetToggleState(ctrl_id,cur_item,TRUE);
            }
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


#if defined MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : Connection menu
//	Global resource dependence :
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleConnectionMenuWinMsg(
									 MMI_WIN_ID_T win_id,
									 MMI_MESSAGE_ID_E msg_id,
									 DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMICONNECTION_MENU_CTRL_ID;
    uint16 index = 0;
    uint32 user_data = 0;
    BOOLEAN fly_mode = FALSE;

    //SCI_TRACE_LOW:"mmiconnection_wintab.c, HandleConnectionMenuWinMsg(), msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_1866_112_2_18_2_12_9_2,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#if defined(MMI_2G_4G_SUPPORT)	
        NetworkType2G4GInit();
        #endif	
MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_FULL_PAINT:
        InitPdaConnectionMainListCtrl();
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_WEB:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &user_data);
        switch ( (MMI_CONNECTION_MAIN_LIST_TYPE_E)user_data )
        {
            //lulifei add for setting UI 20150715
//         case MMI_CONNECTION_MENU_LIST_FLIGHT_MODE:
//             fly_mode = MMIAPISET_GetFlyMode();
//             //MMIAPISET_SetFlyMode(!fly_mode);
//             MMIAPISET_SetFlyModeWholeFlow(!fly_mode);
//             MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
//             
//             break;
            
//         case MMI_CONNECTION_MENU_LIST_DUAL_SIM:
// #if defined(MMI_MULTI_SIM_SYS_DUAL)
//             MMISmartDS_MainMenuWin();
// #else
//             MMK_CreateWin((uint32 *)MMICONNECTION_NETWORK_SETTING_WIN_TAB, PNULL);
// #endif
//             break;
            
//         case MMI_CONNECTION_MENU_LIST_ACCESS_POINTS:
//             MMIAPICONNECTION_OpenSettingListWin();
//             break;
            
        case MMI_CONNECTION_MENU_LIST_ACCESS_NETWORK_TYPE:
#if defined(MMI_2G_4G_SUPPORT)
            MMIAPISET_Open2G4GSettingsWindow();	
#else
            MMK_CreateWin((uint32 *)MMICONNECTION_NETWORK_TYPE_WIN_TAB, PNULL);
#endif
            break;
            
// #ifndef MMI_MULTI_SIM_SYS_DUAL
//         case MMI_CONNECTION_MENU_LIST_VODAFONE:
//             MMK_CreateWin((uint32 *)MMICONNECTION_VODAFONE_SETTING_WIN_TAB, PNULL);
//             break;
// #endif
            
#ifdef BLUETOOTH_SUPPORT
        case ID_CONNECTION_BLUETOOTH:
            MMIAPIBT_OpenMainMenuWin();
            break;
#endif
            
#ifdef MMI_GPRS_SUPPORT
        case MMI_CONNECTION_MENU_LIST_GPRS_NET_USERID:
            MMIAPICONNECTION_OpenSettingListWin();
            break;

        case MMI_CONNECTION_MENU_LIST_GPRS_MODESET:
            MMIAPICONNECTION_OpenGprsWin();
            break;
#endif
#ifdef MMI_DUALMODE_ENABLE
        case MMI_CONNECTION_MENU_LIST_NETSETTING:
#else
        case MMI_CONNECTION_MENU_LIST_NETWORK:
#endif
            MMIAPISET_OpenSetNetWorkWin();
            break;

//         case MMI_CONNECTION_MENU_LIST_NETWORK_TYPE:
//             MMIAPISET_OpenNetWorkTypeWin();
//             break;

#ifdef MMI_PREFER_PLMN_SUPPORT
        case MMI_CONNECTION_MENU_LIST_GPRS_PREFER_NETWORK:
            MMIAPISET_OpenPreferNetWorkWin();
            break;
#endif

#ifdef MMI_GPRS_DATA_SIM_SET
        case MMI_CONNECTION_MENU_LIST_GPRS_MODE:
             MMK_CreatePubListWin((uint32 *)MMICONNECTION_GPRS_MODE_WIN_TAB, PNULL);
            break;
#endif

        //////////////////////////////////////////////////////////////////////////
        // added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
        case MMI_CONNECTION_MENU_LIST_GPRS_ROAMING:
        {
            if(!MMICONNECTION_IsSupportDomesticRoaming())
            {
                if (MMAPIICONNECTION_Setting_GetDataRoamingStatus() == MMICONNECTION_SETTING_DATA_ROAMING_ENABLE)
                {
                    //search the SIM in roaming status with activated pdp links
                    if (MMIAPICONNECTION_IsRoamingActivedPdpLinkExist())
                    {
                        MMIPUB_OpenQueryTextWinByTextId(TXT_CONNECTION_DATA_ROAMING_CUTOFF_QUERY,
                            PNULL,
                            HandleDataRoamingCutoffQueryWinMsg);
                    }
                    else
                    {
                        MMIAPICONNECTION_Setting_SetDataRoamingStatus(MMICONNECTION_SETTING_DATA_ROAMING_DISABLE);
                    }
                }
                else
                {
                    MMIPUB_OpenQueryTextWinByTextId(TXT_CONNECTION_DATA_ROAMING_QUERY,
                        PNULL,
                        HandleDataRoamingQueryWinMsg);
//                     MMIAPICONNECTION_Setting_SetDataRoamingStatus(MMICONNECTION_SETTING_DATA_ROAMING_ENABLE);
                }
                MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICONNECTION_GPRS_DATA_ROAMING_WIN_TAB, PNULL);
            }

            break;
        }
#endif

#ifdef MMI_GPRS_SWITCH_SUPPORT
        case MMI_CONNECTION_MENU_LIST_GPRS_SWITCH:
        {
            if (MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_ON)
            {
                MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_OFF);
            }
            else
            {
                MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_ON);
            }


            MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);  //Update window
            break;
        }
#endif
        case MMI_CONNECTION_SET_VODAFONE_PAY:
            MMICONNECTION_OpenVodafoneWin(MN_DUAL_SYS_1);
            break;
#if 0
#ifdef WIFI_SUPPORT
        case ID_CONNECTION_WIFI:
            MMIAPIWIFI_OpenMainWin();
            break;
#endif

#ifdef MMI_TVOUT_ENABLE
        case ID_CONNECTION_OPEN_TV_OUT:
            if(TRUE == MMITHEME_IsTVOpen())
            {
                MMIAPITVOUT_Close();
            }
            else
            {
                MMIAPITVOUT_OpenWin();
            }
            break;

        case ID_CONNECTION_TV_OUT_SETTING:
            MMIAPITVOUTSetting_OpenWin();
            break;
#endif



#ifdef GPS_SUPPORT
        case ID_CONNECTION_GPS:
            MMIAPIGPS_OpenMainWin();
            break;
#endif
#endif
        default:
            break;
        }

        break;

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
#else

#ifdef MMI_GPRS_SWITCH_SUPPORT
/*****************************************************************************/
// 	Description : Set GPRS switch win.
//	Global resource dependence : none
//    Author:
//	Note:
/*****************************************************************************/
LOCAL void SetGPRSSwitchSettingsWin(void)
{
    uint16 cur_selection = 0;
    MMICONNECTION_GPRS_SETTINGS_E gprs_settings = MMICONNECTION_GPRS_ATTACH_ONNEED;

    GUILIST_SetMaxItem(MMICONNECTION_GPRSSWITCH_LIST_CTRL_ID, MMICONNECTION_SETTING_GPRS_SWITCH_MAX, FALSE );//max item 2

    // add the item to listbox
    MMIAPISET_AppendListItemByTextId( TXT_COMMON_ON, TXT_COMMON_OK, MMICONNECTION_GPRSSWITCH_LIST_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId( TXT_COMMON_OFF, TXT_COMMON_OK, MMICONNECTION_GPRSSWITCH_LIST_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);

    gprs_settings = MMIAPICONNECTION_GetGPRSSwitchStatus();

    cur_selection = (uint16)gprs_settings;

    GUILIST_SetSelectedItem(MMICONNECTION_GPRSSWITCH_LIST_CTRL_ID, cur_selection, TRUE);
    GUILIST_SetCurItemIndex(MMICONNECTION_GPRSSWITCH_LIST_CTRL_ID, cur_selection);
}

/*****************************************************************************/
// 	Description : Handle GPRS switch setting win.
//	Global resource dependence : none
//    Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGprsSwitchWinMsg(MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMICONNECTION_SETTING_GPRS_SWITCH_E gprs_settings = MMICONNECTION_SETTING_GPRS_SWITCH_ON;
    uint16 cur_selection = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMICONNECTION_GPRSSWITCH_LIST_CTRL_ID);
        SetGPRSSwitchSettingsWin();
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(MMICONNECTION_GPRSSWITCH_LIST_CTRL_ID);

        gprs_settings = (MMICONNECTION_SETTING_GPRS_SWITCH_E)cur_selection;

        MMIAPICONNECTION_SetGPRSSwitchStatus(gprs_settings);
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
#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : Connection menu
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleConnectionMenuWinMsg(
									 MMI_WIN_ID_T win_id,
									 MMI_MESSAGE_ID_E msg_id,
									 DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    uint32  node_id = 0;
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_CTRL_ID_T ctrl_id = MMICONNECTION_MENU_CTRL_ID;


    //SCI_TRACE_LOW:"mmiconnection_wintab.c, HandleConnectionMenuWinMsg(), msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_2010_112_2_18_2_12_9_3,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
        MMK_SetAtvCtrl(MMICONNECTION_MENU_WIN_ID, MMICONNECTION_MENU_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
        {
#ifdef MMI_GPRS_SUPPORT
            uint16 sim_sys                        = MN_DUAL_SYS_1;
            uint32 sim_num                        = 0;

            sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
#ifdef MMI_GPRS_SWITCH_SUPPORT
            if ((1 < sim_num) || MMICONNECTION_SETTING_GPRS_SWITCH_OFF == MMIAPICONNECTION_GetGPRSSwitchStatus())
#else
            if (1 < sim_num)
#endif
            {
                GUIMENU_SetNodeGrayed(TRUE, s_connection_index, 0, MMICONNECTION_MENU_CTRL_ID);
            }
            else
            {
                GUIMENU_SetNodeGrayed(FALSE, s_connection_index, 0, MMICONNECTION_MENU_CTRL_ID);
            }
#endif
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_KEYDOWN_WEB:
        node_id = GUIMENU_GetCurNodeId(MMICONNECTION_MENU_CTRL_ID);

        switch ( node_id )
        {
#ifdef MMI_TVOUT_ENABLE
        case ID_CONNECTION_OPEN_TV_OUT:
            if(TRUE == MMITHEME_IsTVOpen())
            {
                MMIAPITVOUT_Close();
            }
            else
            {
                MMIAPITVOUT_OpenWin();
            }
            break;

        case ID_CONNECTION_TV_OUT_SETTING:
            MMIAPITVOUTSetting_OpenWin();
            break;
#endif

#ifdef BLUETOOTH_SUPPORT
        case ID_CONNECTION_BLUETOOTH:
            MMIAPIBT_OpenMainMenuWin();
            break;
#endif

#ifdef GPS_SUPPORT
        case ID_CONNECTION_GPS:
            MMIAPIGPS_OpenMainWin();
            break;
#endif

#ifdef MMI_GPRS_SUPPORT
        case ID_CONNECTION_NET_USERID:
            MMIAPICONNECTION_OpenSettingListWin();
            break;

        case ID_CONNECTION_GPRS:
            MMIAPICONNECTION_OpenGprsWin();
            break;
#endif
        case ID_CONNECTION_SET_NETWORK://ID_SET_NETWORK
            MMIAPISET_OpenSetNetWorkWin();
            break;
        case ID_CONNECTION_SET_VODAFONE_PAY:
            MMICONNECTION_OpenVodafoneWin(MN_DUAL_SYS_1);
            break;
#ifdef MMI_DUALMODE_ENABLE
        case ID_CONNECTION_SET_NETSETTING:
            MMIAPISET_OpenSetNetWorkWin();
            break;
#endif

        default:
            break;

#ifdef MMI_GPRS_SWITCH_SUPPORT
        case ID_CONNECTION_GRPS_SWITCH:
            MMK_CreateWin((uint32)MMICONNECTION_GPRS_SWITCH_WIN_TAB,PNULL);
            break;

#endif
#ifdef WIFI_SUPPORT
        case ID_CONNECTION_WIFI:
            MMIAPIWIFI_OpenMainWin();
            break;
#endif
        }

        break;

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
#endif
/*****************************************************************************/
// 	Description : Get UI list index by apn list index 
//	Global resource dependence : none
//  Author: zhongping.shi
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICONNECTION_GetListIndexByAPNIndex(MN_DUAL_SYS_E dual_sys, uint16 apn_index)
{
    uint16 count = 0;
    uint16 list_index = 0;

    if (apn_index < s_connection_setting_info.settinglink_num[dual_sys])
    {
        list_index = apn_index;
        for (count = 0; count < apn_index; count++)
        {
            if (FALSE == s_connection_setting_info.setting_detail[dual_sys][count].visible)
            {
                list_index--;
            }
        }
    }

    return list_index;
}
/*****************************************************************************/
// 	Description : Get apn list index by UI list index
//	Global resource dependence : none
//  Author: zhongping.shi
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICONNECTION_GetAPNIndexByListIndex(MN_DUAL_SYS_E dual_sys, uint16 list_index)
{
    uint16 count = 0;
    uint16 apn_index = 0;
    uint16 display_cnt = 0;
    
    if (apn_index < s_connection_setting_info.settinglink_num[dual_sys])
    {
        apn_index = list_index;
        for (count = 0; (count < s_connection_setting_info.settinglink_num[dual_sys]) && (display_cnt < list_index+1); count++)
        {
            if (FALSE == s_connection_setting_info.setting_detail[dual_sys][count].visible)
            {
                apn_index++;
            }
            else
            {
                display_cnt++;
            }
        }
    }
    
    return apn_index;
}

#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : handle message of setting list window of browser
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingListWinMsg(
                                           MMI_WIN_ID_T	    win_id,
                                           MMI_MESSAGE_ID_E	msg_id,
                                           DPARAM           param
                                           )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint16 i = 0;
    MMI_CTRL_ID_T ctrl_id = MMICONNECTION_LIST_SETTING_CTRL_ID;
    MN_DUAL_SYS_E dual_sys = s_connection_handling_sim_id;
	uint16 cur_index  = 0;
	uint16 list_index = 0;
	uint16 apn_index  = 0;
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //��̬���LIST ITEM
        GUILIST_SetMaxItem( ctrl_id, MMICONNECTION_MAX_SETTING_NUM, FALSE );//max item 20
#ifdef _ATEST_
#ifdef WIN32
        ATESTOTA_GetTotalMsgNumberInInternetAccount(s_connection_setting_info.settinglink_num[dual_sys]);
#endif//WIN32
#endif//_ATEST_

        for(i = 0; i < s_connection_setting_info.settinglink_num[dual_sys]; i++)                              //��ʾLIST
        {
            SCI_TRACE_LOW("[szp]HandleSettingListWinMsg  s_connection_setting_info.setting_detail[%d][%d].visible = %d", 
                            dual_sys,i,s_connection_setting_info.setting_detail[dual_sys][i].visible);
            if(TRUE == s_connection_setting_info.setting_detail[dual_sys][i].visible)
            {
                if (!s_connection_setting_info.setting_detail[dual_sys][i].is_customed_detail)
                {
                    AppendOneLineTextListItem(ctrl_id,
                        GUIITEM_STYLE_ONE_LINE_ICON_TEXT,//GUIITEM_STYLE_ONE_LINE_TEXT_MS,
                        STXT_OPTION,
                        TXT_NULL,
                        IMAGE_CONNECTION_LINK_ITEM_RW,
                        s_connection_setting_info.setting_detail[dual_sys][i].name,
                        s_connection_setting_info.setting_detail[dual_sys][i].name_len,
                        list_index,
                        FALSE
                        );
                    list_index ++;
                }
                else
                {
                    AppendOneLineTextListItem(ctrl_id,
                        GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
                        STXT_OPTION,
                        TXT_NULL,
                        IMAGE_CONNECTION_LINK_ITEM_RO,
                        s_connection_setting_info.setting_detail[dual_sys][i].name,
                        s_connection_setting_info.setting_detail[dual_sys][i].name_len,
                        list_index,
                        FALSE
                        );
                    list_index++;
                }
            }
        }

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_GET_FOCUS:
        {
            GUILIST_POS_INFO_T pos_info = {0};

            GUILIST_GetCurPosInfo(ctrl_id, &pos_info);
            GUILIST_RemoveAllItems(ctrl_id);

            GUILIST_SetMaxItem(ctrl_id, MMICONNECTION_MAX_SETTING_NUM,FALSE);//max item 20
            for(i = 0; i < s_connection_setting_info.settinglink_num[dual_sys]; i++)
            {
                SCI_TRACE_LOW("[szp]HandleSettingListWinMsg F s_connection_setting_info.setting_detail[%d][%d].visible = %d", 
                                dual_sys,i,s_connection_setting_info.setting_detail[dual_sys][i].visible);
                if(TRUE == s_connection_setting_info.setting_detail[dual_sys][i].visible)
                {
                    if(!s_connection_setting_info.setting_detail[dual_sys][i].is_customed_detail)
                    {
                        AppendOneLineTextListItem(ctrl_id,
                            GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
                            STXT_OPTION,
                            TXT_NULL,
                            IMAGE_CONNECTION_LINK_ITEM_RW,
                            s_connection_setting_info.setting_detail[dual_sys][i].name,
                            s_connection_setting_info.setting_detail[dual_sys][i].name_len,
                            list_index,
                            FALSE
                            );
                        list_index++;
                    }
                    else
                    {
                        AppendOneLineTextListItem(ctrl_id,
                            GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
                            STXT_OPTION,
                            TXT_NULL,
                            IMAGE_CONNECTION_LINK_ITEM_RO,
                            s_connection_setting_info.setting_detail[dual_sys][i].name,
                            s_connection_setting_info.setting_detail[dual_sys][i].name_len,
                            list_index,
                            FALSE
                            );
                        list_index++;
                    }
                }
            }

            GUILIST_SetCurPosInfo(ctrl_id, &pos_info);
        }
        break;

//     case MSG_FULL_PAINT:
// 	{
// 		uint8 default_apn = 0;
// 		MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
// 
// 		if(!MMK_IsFocusWin(MMICONNECTION_SETTINGLIST_WIN_ID))
// 			break;
// 
// 		if(default_apn >= s_connection_setting_info.settinglink_num[dual_sys])
// 		{
// 			default_apn = 0;
// 			MMICONNECTION_SetApnDefaultId(dual_sys,default_apn);
// 		}
// 		list_index = MMIAPICONNECTION_GetListIndexByAPNIndex(dual_sys, default_apn);
// 		
// 		GUILIST_SetSelectedItem(ctrl_id, (uint16)list_index, TRUE);
//         GUILIST_SetCurItemIndex(ctrl_id, (uint16)list_index);
// 	}
// 	break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)MMICONNECTION_SETTINGLINKLISTMENU_WIN_TAB, PNULL);
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        MMK_CreateWin((uint32 *)MMICONNECTION_SETTINGLINKLIST_LONGOKMENU_WIN_TAB, PNULL);
        break;
#endif

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        {
            uint32 temp_param=0;
            MMICONNECTION_LINKSET_OP_E op = MMICONNECTION_LINKSET_OP_EDIT;

            SCI_TRACE_LOW("[szp]:HandleSettingListWinMsg total = %d, invisible = %d",s_connection_setting_info.settinglink_num[dual_sys], s_connection_setting_info.invisible_apn_num[dual_sys]);

            if (0 != s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys])
            {
                cur_index = GUILIST_GetCurItemIndex(ctrl_id);
                apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, cur_index);
#ifdef _ATEST_
#ifdef WIN32       
                ATESTOTA_GetOtaConfigInfoFromLinkSetting(s_connection_setting_info.setting_detail[dual_sys][apn_index]); // added by Mingxuan.Zhu
#endif//WIN32
#endif//_ATEST_
                if ((!s_connection_setting_info.setting_detail[dual_sys][apn_index].is_customed_detail) && (0 != s_connection_setting_info.settinglink_num[dual_sys]))
                {
                    op = MMICONNECTION_LINKSET_OP_EDIT;
                    temp_param = op << 16;
                    temp_param += apn_index;   
                    MMK_CreateWin((uint32*)MMICONNECTION_SETTINGLINKMENU_WIN_TAB, (ADD_DATA)temp_param);
                }
                else
                {
                    op = MMICONNECTION_LINKSET_OP_READ;
                    temp_param = op << 16;
                    temp_param += apn_index;  
                    MMK_CreateWin((uint32*)MMICONNECTION_SETTINGLINKDETAILVIEW_WIN_TAB, (ADD_DATA)temp_param);
                }
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(MMICONNECTION_SETTINGLINKMENU_WIN_ID);
        MMK_CloseWin(MMICONNECTION_SETTINGSECURITYCONNECTLIST_WIN_ID);
        MMK_CloseWin(MMICONNECTION_INPUTAGENTNAME_WIN_ID);
        MMK_CloseWin(MMICONNECTION_INPUTUSERNAME_WIN_ID);
        MMK_CloseWin(MMICONNECTION_INPUTPASSWORD_WIN_ID);
        MMK_CloseWin(MMICONNECTION_INPUTAPN_WIN_ID);
        MMK_CloseWin(MMICONNECTION_INPUTGATEWAY_WIN_ID);
        MMK_CloseWin(MMICONNECTION_EDITPORT_WIN_ID);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//������һ��
        break;

    case MSG_PROMPTWIN_OK:                                    //delete
        list_index = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SETTING_CTRL_ID);
        apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, list_index);
        if ((apn_index < s_connection_setting_info.settinglink_num[dual_sys]) 
            && (0 < (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys])))
        {
            for(i = (uint32)apn_index; i < (uint32)(s_connection_setting_info.settinglink_num[dual_sys]-1); i++)
            {
                SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][i],&s_connection_setting_info.setting_detail[dual_sys][i+1],sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            }
            SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][s_connection_setting_info.settinglink_num[dual_sys] - 1],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys]--;
        }
        else if ((apn_index ==  s_connection_setting_info.settinglink_num[dual_sys])
            && (0 < (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys])))
        {
            SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][s_connection_setting_info.settinglink_num[dual_sys] - 1],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys]--;
        }
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO,&s_connection_setting_info);
        //����Ӧ��ѡ�е�ITEM
        ChangeAppSelectedIndex(dual_sys, apn_index, MMICONNECTION_NOTIFY_EVENT_DEL_ITEM);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
#endif
        GUILIST_RemoveItem(MMICONNECTION_LIST_SETTING_CTRL_ID, list_index);
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    default:
        //SCI_TRACE_LOW:"[MMICONNECTION]:HandleSettingListWinMsg() msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_2294_112_2_18_2_12_10_4,(uint8*)"d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;

}


/*****************************************************************************/
// 	Description : handle message of setting link list menu window of connection
//      This menu is to deal to a selected list such as,edit,delete,etc.
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkListMenuWinMsg(
                                                   MMI_WIN_ID_T	    win_id,
                                                   MMI_MESSAGE_ID_E	msg_id,
                                                   DPARAM			param
                                                   )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint16			apn_index = 0;
    uint16			list_index = 0;
    uint32			group_id = 0;
    uint32			menu_id = 0;
    uint32			i = 0;
    MN_DUAL_SYS_E dual_sys = s_connection_handling_sim_id;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //������ƶ�����������⴦��
        i = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SETTING_CTRL_ID);
        apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, i);
        if(0 == s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys] ||
            s_connection_setting_info.setting_detail[dual_sys][apn_index].is_customed_detail)
        {
            GUIMENU_SetItemGrayed(MMICONNECTION_MENU_SETTINGLINKLIST_CTRL_ID,MENU_CONNECTION_SETTING_LINKLIST,ID_CONNECTION_SETTING_DELETE,TRUE);
            GUIMENU_SetItemGrayed(MMICONNECTION_MENU_SETTINGLINKLIST_CTRL_ID,MENU_CONNECTION_SETTING_LINKLIST,ID_CONNECTION_SETTING_EDIT,TRUE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMICONNECTION_MENU_SETTINGLINKLIST_CTRL_ID,MENU_CONNECTION_SETTING_LINKLIST,ID_CONNECTION_SETTING_DELETE,FALSE);
            GUIMENU_SetItemGrayed(MMICONNECTION_MENU_SETTINGLINKLIST_CTRL_ID,MENU_CONNECTION_SETTING_LINKLIST,ID_CONNECTION_SETTING_EDIT,FALSE);
        }

        MMK_SetAtvCtrl(win_id,MMICONNECTION_MENU_SETTINGLINKLIST_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(MMICONNECTION_MENU_SETTINGLINKLIST_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch(menu_id)//�˵��ڵ�ID
        {
        case ID_CONNECTION_SETTING_SETGBUILD:     //�½�����
            if(s_connection_setting_info.settinglink_num[dual_sys] >= MMICONNECTION_MAX_SETTING_NUM)
            {
                MMIPUB_OpenAlertWarningWin(TXT_CONNECTION_LISTFULL);
            }
            else
            {
                //�������ɣ���������+index����16λ�Ų������ͣ���16λ��index
                uint32 temp_param=0;
                MMICONNECTION_LINKSET_OP_E op = MMICONNECTION_LINKSET_OP_NEW;
                temp_param = op<<16;
                temp_param+=s_connection_setting_info.settinglink_num[dual_sys];
                SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][s_connection_setting_info.settinglink_num[dual_sys]],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T) );
                //�����ò˵�����
                MMK_CreateWin((uint32*)MMICONNECTION_SETTINGLINKMENU_WIN_TAB, (ADD_DATA)temp_param);
            }
            MMK_CloseWin(win_id);
            break;
        case ID_CONNECTION_SETTING_DELETE:      										//ɾ��һ��ѡ�е�����
            SCI_TRACE_LOW("[szp]:HandleSettingLinkListMenuWinMsg total = %d, invisible = %d",s_connection_setting_info.settinglink_num[dual_sys], s_connection_setting_info.invisible_apn_num[dual_sys]);
            if (1 == (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys]))
            {
                MMIPUB_OpenAlertWarningWin(TXT_CONNECTION_DELETE_FAIL);
                MMK_CloseWin(win_id);//������һ��
            }
            else
            {
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                MMK_CloseWin(win_id);
            }
            break;

        case ID_CONNECTION_SETTING_EDIT:                                                                         //�༭һ��ѡ�е�����
			/*
			{
                uint32 temp_param=0;
                uint16 index= 0;
                MMICONNECTION_LINKSET_OP_E op = MMICONNECTION_LINKSET_OP_EDIT;
                temp_param = op<<16;
                index = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SETTING_CTRL_ID);
                temp_param+=index;
                //�����ò˵�����
                MMK_CreateWin((uint32*)MMICONNECTION_SETTINGLINKMENU_WIN_TAB, (ADD_DATA)temp_param);
                MMK_CloseWin(win_id);//������һ��
            }
            */
	        {
	            uint32 temp_param=0;
				uint16 index= 0;
	            MMICONNECTION_LINKSET_OP_E op = MMICONNECTION_LINKSET_OP_EDIT;

	            index = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SETTING_CTRL_ID);
                apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, index);
#ifdef _ATEST_
#ifdef WIN32
	            ATESTOTA_GetOtaConfigInfoFromLinkSetting(s_connection_setting_info.setting_detail[dual_sys][apn_index]); // added by Mingxuan.Zhu
#endif//WIN32
#endif//_ATEST_
	            if((!s_connection_setting_info.setting_detail[dual_sys][apn_index].is_customed_detail) 
	                && (0 != s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys]))
	            {
	                op = MMICONNECTION_LINKSET_OP_EDIT;
	                temp_param = op<<16;
	                temp_param+=apn_index;
	                MMK_CreateWin((uint32*)MMICONNECTION_SETTINGLINKMENU_WIN_TAB, (ADD_DATA)temp_param);
	            }
	            else
	            {
	                op = MMICONNECTION_LINKSET_OP_READ;
	                temp_param = op<<16;
	                temp_param+=apn_index;
	                MMK_CreateWin((uint32*)MMICONNECTION_SETTINGLINKDETAILVIEW_WIN_TAB, (ADD_DATA)temp_param);
	            }
				MMK_CloseWin(win_id);
	        }
            break;
#if 0 // bug185707
        case ID_CONNECTION_SETTING_ADDLINKSETTING:                                                  //�������嵥�����
            if(s_connection_setting_info.settinglink_num[dual_sys] >= MMICONNECTION_MAX_SETTING_NUM)
            {
                MMIPUB_OpenAlertWarningWin(TXT_CONNECTION_LISTFULL);
            }
            else
            {
                //�������б���
                MMK_CreateWin((uint32*)MMICONNECTION_ADDSETTINGLIST_WIN_TAB, PNULL);
            }
            MMK_CloseWin(win_id);
            break;
#endif
        case ID_CONNECTION_SETTING_REASERCH:  //����PLMN �������嵥���������ʵ�����
            {
                /*
                �������޸������㷨��Ŀ����Ϊ�˿˷�ԭ�����㷨ֻ���������б�С��24������²���ʹ��
                �޸ĺ���㷨û��������ƣ�ֻҪ������UINT�����ݵķ�Χ�ڼ���
                */
                PHONE_PLMN_NAME_E name = PLMN_INVALID;
                uint32 search_num =0;

                if (!MMIAPIPHONE_IsSimCardOk(dual_sys))
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                    recode = MMI_RESULT_FALSE;
                }
                else
                {
                    BOOLEAN is_res_data_exist = FALSE;
                    MMICONNECTION_RES_INFO_T res_info = {0};

                    is_res_data_exist = LoadNetworkAccountInfo(dual_sys, &res_info);

                    if (!is_res_data_exist || PNULL == res_info.res_item_ptr)
                    {
                        UnloadNetworkAccountInfo(&res_info);
                        break;
                    }

                    MMIAPIPHONE_GetFullPlmn(dual_sys, PNULL, &name);

                    for(i = 0;i<res_info.res_num;i ++)
                    {
                        PHONE_PLMN_NAME_E plmn_name = PLMN_INVALID;
                        MN_PLMN_T plmn = {0};

                        plmn.mcc = res_info.res_item_ptr[i].mcc;
                        plmn.mnc = res_info.res_item_ptr[i].mnc;

                        plmn_name = MMIAPIPHONE_GetNetWorkId(&plmn);

                        if (IsSameNetworkName(name, plmn_name))
                        {
                            search_num++;
                        }
                    }

                    UnloadNetworkAccountInfo(&res_info);

                    if(0 == search_num)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_CONNECTION_HAVENOITEM);
                    }
                    else
                    {
                        //�������б���
                        MMK_CreateWin((uint32*)MMICONNECTION_SEARCHLINKLIST_WIN_TAB, PNULL);//����PLMN �������嵥���������ʵ�����
                    }
                }
                MMK_CloseWin(win_id);
            }
            break;

#if defined(DATA_ACCOUNT_IN_TCARD_SUPPORT) && defined(BROWSER_SUPPORT)
        case ID_CONNECTION_SETTING_DOWNLOAD_RES:
            //���������ݴ���
            CheckNetworkAccountResFileToDownload();
            MMK_CloseWin(win_id);
            break;
#endif
        default:
            break;

        }
        break;

    case MSG_PROMPTWIN_OK:                                    //delete
        list_index = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SETTING_CTRL_ID);
        apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, list_index);
        if ((apn_index < s_connection_setting_info.settinglink_num[dual_sys]) 
            && (0 < (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys])))
        {
            for(i = (uint32)apn_index; i < (uint32)(s_connection_setting_info.settinglink_num[dual_sys]-1); i++)
            {
                SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][i],&s_connection_setting_info.setting_detail[dual_sys][i+1],sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            }
            SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][s_connection_setting_info.settinglink_num[dual_sys] - 1],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys]--;
        }
        else if ((apn_index ==  s_connection_setting_info.settinglink_num[dual_sys])
                 && (0 < (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys])))
        {
            SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][s_connection_setting_info.settinglink_num[dual_sys] - 1],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys]--;
        }
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO,&s_connection_setting_info);
        //����Ӧ��ѡ�е�ITEM
        ChangeAppSelectedIndex(dual_sys, apn_index, MMICONNECTION_NOTIFY_EVENT_DEL_ITEM);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
#endif
        GUILIST_RemoveItem(MMICONNECTION_LIST_SETTING_CTRL_ID, list_index);
        MMK_CloseWin(win_id);
        MMIPUB_CloseQuerytWin(PNULL);
        break;
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
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
// 	Description : handle message of  search link list window
//	Global resource dependence : none
//  Author: wenbao.bai
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSearchResultLinkListWinMsg(
                                                    MMI_WIN_ID_T	    win_id,
                                                    MMI_MESSAGE_ID_E	msg_id,
                                                    DPARAM				param
                                                    )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint32			i = 0,j=0,k=0;
    uint16 cur_select = 0;
	MN_PLMN_T sim_hplmn = {0};
    BOOLEAN is_find = FALSE;
    MN_DUAL_SYS_E dual_sys = s_connection_handling_sim_id;
    MN_PLMN_T plmn = {0};
    BOOLEAN is_res_data_exist = FALSE;
    MMICONNECTION_RES_INFO_T res_info = {0};
    BOOLEAN visible = FALSE;
#ifdef MMI_VOLTE_SUPPORT
    uint8 volte_info = 0;
    BOOLEAN volte_on = FALSE;
#endif    
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }

#ifdef MMI_VOLTE_SUPPORT
    volte_info = MMIAPISET_GetVolteInfo();
    if (volte_info & (1 << dual_sys))
    {
        volte_on = TRUE;
    }
    else
    {
        volte_on = FALSE;
    }
#endif    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            is_res_data_exist = LoadNetworkAccountInfo(dual_sys, &res_info);

            if (!is_res_data_exist || PNULL == res_info.res_item_ptr)
            {
                MMK_CloseWin(win_id);
                UnloadNetworkAccountInfo(&res_info);
                break;
            }

            MMK_SetAtvCtrl(MMICONNECTION_SEARCHLINKLIST_WIN_ID,MMICONNECTION_LIST_SEARCHLINKLIST_CTRL_ID);
            GUILIST_SetMaxItem(MMICONNECTION_LIST_SEARCHLINKLIST_CTRL_ID, res_info.res_num,FALSE);

            for(i = 0; i < res_info.res_num; i ++)
            {
                plmn.mcc = res_info.res_item_ptr[i].mcc;
                plmn.mnc = res_info.res_item_ptr[i].mnc;
				
				MMIAPIPHONE_GetSimHPLMN(&sim_hplmn, dual_sys);		
                if ((sim_hplmn.mcc == plmn.mcc) && (sim_hplmn.mnc == plmn.mnc))
                {
                    if (((TRUE == res_info.res_item_ptr[i].visible) && (MMICONNECTION_ACCOUNT_IMS != res_info.res_item_ptr[i].account_type)) || 
                        (
#ifdef MMI_VOLTE_SUPPORT
						(TRUE == volte_on) && 
#endif
						(MMICONNECTION_ACCOUNT_IMS == res_info.res_item_ptr[i].account_type))) 
					{
						AppendOneLineTextListItem(MMICONNECTION_LIST_SEARCHLINKLIST_CTRL_ID,
							GUIITEM_STYLE_ONE_LINE_TEXT_MS,
							PNULL,
							PNULL,
							PNULL,
							res_info.res_item_ptr[i].name,
							res_info.res_item_ptr[i].name_len,
							i,
							FALSE
							);
					}
				}
			}
            UnloadNetworkAccountInfo(&res_info);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            cur_select = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SEARCHLINKLIST_CTRL_ID);

            is_res_data_exist = LoadNetworkAccountInfo(dual_sys, &res_info);

            if (!is_res_data_exist || PNULL == res_info.res_item_ptr)
            {
                UnloadNetworkAccountInfo(&res_info);
                break;
            }

            MMIAPIPHONE_GetSimHPLMN(&sim_hplmn, dual_sys);		
            for(i = 0; i < res_info.res_num; i ++)
            {
                plmn.mcc = res_info.res_item_ptr[i].mcc;
                plmn.mnc = res_info.res_item_ptr[i].mnc;

                for (j = 0; j < MMI_DUAL_SYS_MAX; j++)
                {
                    if ((sim_hplmn.mcc == plmn.mcc) && (sim_hplmn.mnc == plmn.mnc) && (TRUE == res_info.res_item_ptr[i].visible))
                    {
                        if(k == cur_select)
                        {
                            is_find = TRUE;

                            break;
                        }

                        k ++;
                        break;
                    }
                }

                if (is_find)
                {
                    break;
                }
            }

            if(s_connection_setting_info.settinglink_num[dual_sys]<MMICONNECTION_MAX_SETTING_NUM && i < res_info.res_num)
            {
                uint8 index = s_connection_setting_info.settinglink_num[dual_sys];

                visible = SaveNetworkAccountInfo(&s_connection_setting_info.setting_detail[dual_sys][index],
                                                 &res_info.res_item_ptr[i], TRUE, i);
				if (FALSE == visible)
				{
                    s_connection_setting_info.invisible_apn_num[dual_sys]++;
				}
				
                MMICONNECTION_ClearHighPriList(dual_sys);
                MMICONNECTION_SetHighPriList(dual_sys, index);
                s_connection_setting_info.settinglink_num[dual_sys]++;
                MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);

                if (((MMICONNECTION_ACCOUNT_BROWSER == res_info.res_item_ptr[i].account_type) && (FALSE == s_apntype_index.internet_index[dual_sys].is_valid))
                 || ((MMICONNECTION_ACCOUNT_IMS == res_info.res_item_ptr[i].account_type) && (FALSE == s_apntype_index.ims_index[dual_sys].is_valid))
                 || ((MMICONNECTION_ACCOUNT_IA == res_info.res_item_ptr[i].account_type) && (FALSE == s_apntype_index.ims_index[dual_sys].is_valid))
                 || ((MMICONNECTION_ACCOUNT_XCAP == res_info.res_item_ptr[i].account_type) && (FALSE == s_apntype_index.ims_index[dual_sys].is_valid))
                 || (MMICONNECTION_ACCOUNT_MMS == res_info.res_item_ptr[i].account_type))
                {
                    MMICONNECTION_AdaptingAccount(dual_sys, res_info.res_item_ptr[i].account_type);//Ĭ���б�����ӵ��ʻ���������һ���ʻ�ƥ��
                }

                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);

                MMK_CloseWin(win_id);
                MMK_CloseWin(MMICONNECTION_SETTINGLINKLISTMENU_WIN_ID);
            }
            else
            {
            	MMIPUB_OpenAlertWarningWin(TXT_CONNECTION_LISTFULL);
            }

            UnloadNetworkAccountInfo(&res_info);
        }
        break;
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
//     Description : to set the MMICONNECTION_GPRS_SETTINGS_CTRL_ID
//    Global resource dependence :
//  Author:wancan.you
//    Note:
/*****************************************************************************/
LOCAL void SetGPRSSettingsWin(void)
{
    uint16 cur_selection = 0;
    MMICONNECTION_GPRS_SETTINGS_E gprs_settings = MMICONNECTION_GPRS_ATTACH_ONNEED;

    GUILIST_SetMaxItem(MMICONNECTION_GPRS_SETTINGS_CTRL_ID, MMICONNECTION_GPRS_SETTINGS_MAX, FALSE );//max item 2

    // add the item to listbox
    MMIAPISET_AppendListItemByTextId( TXT_CONNECTION_GPRS_DETACH, TXT_COMMON_OK, MMICONNECTION_GPRS_SETTINGS_CTRL_ID, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT);
    MMIAPISET_AppendListItemByTextId( TXT_CONNECTION_GPRS_ALWAYS, TXT_COMMON_OK, MMICONNECTION_GPRS_SETTINGS_CTRL_ID, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT);
#ifdef MMI_DUALMODE_ENABLE
    MMIAPISET_AppendListItemByTextId( TXT_ENERGY_SAVING, TXT_COMMON_OK, MMICONNECTION_GPRS_SETTINGS_CTRL_ID, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT);
#endif
    gprs_settings = MMICONNECTION_GetGPRSModeStatus();

    cur_selection = (uint16)gprs_settings;

    GUILIST_SetSelectedItem(MMICONNECTION_GPRS_SETTINGS_CTRL_ID, cur_selection, TRUE);
    GUILIST_SetCurItemIndex(MMICONNECTION_GPRS_SETTINGS_CTRL_ID, cur_selection);
}

/*****************************************************************************/
//     Description : to handle window message about gprs settings.
//    Global resource dependence :
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGPRSSettingsWinMsg(
                                                       MMI_WIN_ID_T        win_id,
                                                       MMI_MESSAGE_ID_E msg_id,
                                                       DPARAM            param
                                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMICONNECTION_GPRS_SETTINGS_E gprs_settings = MMICONNECTION_GPRS_ATTACH_ONNEED;
    uint16 cur_selection = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMICONNECTION_GPRS_SETTINGS_CTRL_ID);
        SetGPRSSettingsWin();
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(MMICONNECTION_GPRS_SETTINGS_CTRL_ID);

        gprs_settings = (MMICONNECTION_GPRS_SETTINGS_E)cur_selection;

        MMIAPICONNECTION_SetGPRSStatus(gprs_settings);

        if (!MMIAPIPDP_PsIsPdpLinkExist())
        {
            if (MMICONNECTION_GPRS_ATTACH_ONNEED == MMICONNECTION_GetGPRSModeStatus())
            {
                MMIAPIPHONE_SetGPRSDetach();
            }
            else
            {
                MMIAPIPHONE_SetGPRSAttach();
            }
        }

#ifdef MMI_DUALMODE_ENABLE
#ifndef WIN32
        {
            uint32 i = 0;
            MMICONNECTION_GPRS_SETTINGS_E grps_status = MMICONNECTION_GetGPRSModeStatus();

            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (MMICONNECTION_GPRS_ALWAYS_LC == grps_status)
                {
                    MNGPRS_SetEnergySavingMode(i, TRUE);
                }
                else
                {
                    MNGPRS_SetEnergySavingMode(i, FALSE);
                }
            }
        }
#endif
#endif
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
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
// 	Description : insert dynamenu node by label
//	Global resource dependence :
//  Author:fen.xie
//	Note:
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                    MMI_CTRL_ID_T      ctrl_id,        //�ؼ�id
                    MMI_TEXT_ID_T   	text_id,       //����Ľڵ���ı�
                    uint32          	node_id,        //�ڵ���
                    uint32          	parent_node_id, //���ڵ���
                    uint16          	index,           //λ��
                    MMI_IMAGE_ID_T      select_icon_id, //item selected icon id
                    BOOLEAN    is_grayed   //is_grayed
                    )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};


    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.select_icon_id = select_icon_id;
    node_item.is_grayed = is_grayed;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}

#ifdef MMI_GPRS_SWITCH_SUPPORT
/*****************************************************************************/
//  Description : Get GPRS switch mode (on or mff)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
uint8 gprs_switch_settings_fix_nv = TRUE;
PUBLIC MMICONNECTION_SETTING_GPRS_SWITCH_E MMIAPICONNECTION_GetGPRSSwitchStatus(void)
{
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
	MMICONNECTION_SETTING_GPRS_SWITCH_E  gprs_switch_settings=MMICONNECTION_SETTING_GPRS_SWITCH_ON;
	uint8 is_support_mobile_roaming = TRUE;
	uint8 ret = 0;
	static uint8 gprs_switch_settings_fix_nv_flag = FALSE;

#ifdef MMI_GPRS_SUPPORT
	MMINV_READ(MMINV_CONNECTION_GPRS_SWITCH, &gprs_switch_settings, return_value);
	if(!gprs_switch_settings_fix_nv_flag)
	{
		if(!(EFS_NvitemRead(OTHER_NV_ENABLE_MOBILE_DATA, NVITEM_GetLength(OTHER_NV_ENABLE_MOBILE_DATA), &is_support_mobile_roaming)))
		{	
			gprs_switch_settings_fix_nv_flag = TRUE;
			gprs_switch_settings_fix_nv = is_support_mobile_roaming;
		}
	}
	if (MN_RETURN_SUCCESS != return_value)
	{	
		if(gprs_switch_settings_fix_nv)
		{
			gprs_switch_settings = MMICONNECTION_SETTING_GPRS_SWITCH_ON;
		}
		else
		{
			gprs_switch_settings = MMICONNECTION_SETTING_GPRS_SWITCH_OFF;
		} 
		MMINV_WRITE(MMINV_CONNECTION_GPRS_SWITCH, &gprs_switch_settings);
	}
	SCI_TRACE_LOW("MMIAPICONNECTION_GetGPRSSwitchStatus  gprs_switch_settings = %d", gprs_switch_settings);
#endif

    return gprs_switch_settings;
}
/*****************************************************************************/
//  Description : Set GPRS switch mode (on or mff)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_E gprs_switch_settings)
{
#ifdef MMI_GPRS_SUPPORT
    if (gprs_switch_settings == MMIAPICONNECTION_GetGPRSSwitchStatus()) //Same value
    {
        return TRUE;
    }

    MMINV_WRITE(MMINV_CONNECTION_GPRS_SWITCH, &gprs_switch_settings);

	SCI_TRACE_LOW("MMIAPICONNECTION_SetGPRSSwitchStatus, gprs_switch_settings = %d", gprs_switch_settings);

    if (gprs_switch_settings != MMICONNECTION_SETTING_GPRS_SWITCH_OFF)
    {
        s_gprs_turned_off = FALSE;
        MMIAPICONNECTION_TurnOnGPRS();
    }
    else
    {
        s_gprs_turned_off = TRUE;
        MMIAPICONNECTION_TurnOffGPRS();
        //zhuyongjing add for gallery uploading
        if(MMIAPIPICVIEW_IsPicFolderWinOpen()){
            MMIAPIPICVIEW_ReloadFile();
        }
        //add end
    }
    MMIAPICOM_StatusAreaUpdateSignalState();
    MMIAPIPHONE_SetGPRSAttachMode();
#endif

    return TRUE;
}
/*****************************************************************************/
//  Description :Turn Off GPRS
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPICONNECTION_TurnOffGPRS(void)
{
    if (MMIAPIPDP_PsIsPdpLinkExist())
    {
        MMIAPIPDP_ReleaseAll(NULL);    
        MMIAPIPHONE_SetGPRSDetach();
    }
    else
    {
        MMIAPIPHONE_SetGPRSDetach();
    }
    return TRUE;
}


/*****************************************************************************/
//  Description :Turn on GPRS
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPICONNECTION_TurnOnGPRS(void)
{
    if (!MMIAPIPDP_PsIsPdpLinkExist())
    {
        if (MMICONNECTION_GPRS_ATTACH_ONNEED == MMICONNECTION_GetGPRSModeStatus())
        {
            MMIAPIPHONE_SetGPRSDetach();
#if defined(MMI_UPDATE_TIME_SUPPORT) && defined(MMI_UPDATE_TIME_WHEN_STARTUP)
            MMIAPIPHONE_StartTimerToGetNitzTime();
#endif
        }
        else
        {
            MMIAPIPHONE_SetGPRSAttach();
        }
    }

    return TRUE;
}


PUBLIC void MMIAPICONNECTION_DetachIfGprsOff(void)
{
    MMICONNECTION_SETTING_GPRS_SWITCH_E  gprs_switch_settings=MMICONNECTION_SETTING_GPRS_SWITCH_ON;
    
    gprs_switch_settings = MMIAPICONNECTION_GetGPRSSwitchStatus();
    SCI_TRACE_LOW("MMIAPICONNECTION_DetachIfGprsOff, gprs_switch_settings = %d", gprs_switch_settings);
    if (gprs_switch_settings == MMICONNECTION_SETTING_GPRS_SWITCH_OFF)
        {
        SCI_TRACE_LOW("MMIAPICONNECTION_DetachIfGprsOff, Detach gprs");
        MMIAPIPHONE_SetGPRSDetach();        //detach it again
    }
}

#endif


/*****************************************************************************/
//  Description : Set GPRS Attach mode (always or manual)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_SetGPRSStatus(MMICONNECTION_GPRS_SETTINGS_E gprs_settings)
{
#ifdef MMI_GPRS_SUPPORT
    MMINV_WRITE(MMINV_CONNECTION_GPRS_SETTINGS, &gprs_settings);
#endif

}

/*****************************************************************************/
// 	Description : Get GPRS Mode Settings.
//	Global resource dependence : none
//    Author:
//	Note:
/*****************************************************************************/
LOCAL MMICONNECTION_GPRS_SETTINGS_E MMICONNECTION_GetGPRSModeStatus(void)
{
    MMICONNECTION_GPRS_SETTINGS_E gprs_mode_settings = MMICONNECTION_GPRS_ATTACH_ONNEED;
#ifdef MMI_GPRS_SUPPORT
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_CONNECTION_GPRS_SETTINGS, &gprs_mode_settings, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
#ifdef CMCC_UI_STYLE
        gprs_mode_settings = MMICONNECTION_GPRS_ALWAYS;
#else
#ifdef MMI_DUALMODE_ENABLE
        gprs_mode_settings = MMICONNECTION_GPRS_ALWAYS_LC;
#else
        gprs_mode_settings = MMICONNECTION_GPRS_ATTACH_ONNEED;
#endif
#endif
        MMINV_WRITE(MMINV_CONNECTION_GPRS_SETTINGS, &gprs_mode_settings);
    }

    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_2841_112_2_18_2_12_11_5,(uint8*)"d", gprs_mode_settings);
#endif
    return gprs_mode_settings;
}


/*****************************************************************************/
//  Description : Get GPRS Attach mode (always or manual)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMICONNECTION_GPRS_SETTINGS_E MMIAPICONNECTION_GetGPRSStatus(void)
{
    MMICONNECTION_GPRS_SETTINGS_E gprs_settings = MMICONNECTION_GPRS_ATTACH_ONNEED;
#ifdef MMI_GPRS_SUPPORT
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
#ifdef MMI_GPRS_SWITCH_SUPPORT
    if (MMICONNECTION_SETTING_GPRS_SWITCH_OFF == MMIAPICONNECTION_GetGPRSSwitchStatus())
    {
        return MMICONNECTION_GPRS_TURNED_OFF;
    }
#endif
    MMINV_READ(MMINV_CONNECTION_GPRS_SETTINGS, &gprs_settings, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
#ifdef MMI_DUALMODE_ENABLE
        gprs_settings = MMICONNECTION_GPRS_ALWAYS_LC;
#else
        gprs_settings = MMICONNECTION_GPRS_ATTACH_ONNEED;
#endif

        MMINV_WRITE(MMINV_CONNECTION_GPRS_SETTINGS, &gprs_settings);
    }

    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_2841_112_2_18_2_12_11_5,(uint8*)"d", gprs_settings);
#endif
    return gprs_settings;
}

/*****************************************************************************/
//  Description : �Ƿ���Ҫ�Զ�Attach GPRS
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_isGPRSNeedAutoAttach(void)
{
    MMICONNECTION_GPRS_SETTINGS_E gprs_settings = MMICONNECTION_GPRS_ATTACH_ONNEED;
    BOOLEAN ret = FALSE;
    //gprs_settings = MMIAPICONNECTION_GetGPRSStatus();
    gprs_settings = MMICONNECTION_GetGPRSModeStatus();
#ifdef MMI_GPRS_SWITCH_SUPPORT
    if(MMICONNECTION_SETTING_GPRS_SWITCH_OFF == MMIAPICONNECTION_GetGPRSSwitchStatus())
        {
            ret = FALSE;
            SCI_TRACE_LOW("MMIAPICONNECTION_isGPRSNeedAutoAttach MMICONNECTION_SETTING_GPRS_SWITCH_OFF=%d",ret);

            return ret;
        }
#endif

    if (MMICONNECTION_GPRS_ATTACH_ONNEED == gprs_settings)
    {
        ret = FALSE;
    }
    else
    {
        ret = TRUE;
    }
    SCI_TRACE_LOW("MMIAPICONNECTION_isGPRSNeedAutoAttach ret=%d",ret);
    return ret;

}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_isGPRSSwitchedOff(void)
{
#ifdef MMI_GPRS_SWITCH_SUPPORT
	SCI_TRACE_LOW("MMIAPICONNECTION_isGPRSSwitchedOff, s_gprs_turned_off = %d", s_gprs_turned_off);
    return s_gprs_turned_off;
#else
    return FALSE;
#endif
}

#ifdef MMI_GPRS_SUPPORT
#if 0  //bug185707
/*****************************************************************************/
// 	Description : handle message of Add  setting list window of browser
//	Global resource dependence : none
//  Author: wenbao.bai
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddSettingListWinMsg(
                                              MMI_WIN_ID_T  	win_id,
                                              MMI_MESSAGE_ID_E	msg_id,
                                              DPARAM			param
                                              )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint16			i = 0;
    MMI_CTRL_ID_T   ctrl_id = MMICONNECTION_LIST_SETTINGADDLIST_CTRL_ID;
    MN_DUAL_SYS_E dual_sys = s_connection_handling_sim_id;
    BOOLEAN is_res_data_exist = FALSE;
    MMICONNECTION_RES_INFO_T res_info = {0};
    MMI_TEXT_ID_T middle_softkey_id = TXT_NULL;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
    {
        MMK_SetAtvCtrl(MMICONNECTION_ADDSETTINGLIST_WIN_ID,MMICONNECTION_LIST_SETTINGADDLIST_CTRL_ID);
        is_res_data_exist = LoadNetworkAccountInfo(dual_sys, &res_info);

        if (!is_res_data_exist || PNULL == res_info.res_item_ptr)
        {
            MMK_CloseWin(win_id);
            UnloadNetworkAccountInfo(&res_info);
            break;
        }
        //��̬���LIST ITEM
        GUILIST_SetMaxItem(ctrl_id, res_info.res_num,FALSE);//max item 20

        if (s_connection_setting_info.settinglink_num[dual_sys] < MMICONNECTION_MAX_SETTING_NUM)
        {
            GUILIST_SetMaxSelectedItem(ctrl_id, MMICONNECTION_MAX_SETTING_NUM - s_connection_setting_info.settinglink_num[dual_sys]);
            middle_softkey_id = STXT_SELECT;
        }
        else
        {
            GUILIST_SetMaxSelectedItem(ctrl_id, 0);
            middle_softkey_id = TXT_NULL;
        }
        GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
        GUILIST_ChangeCheckListSoftkeyIndex(ctrl_id, MIDDLE_BUTTON);
        for(i = 0; i<res_info.res_num; i++)
        {
            MMIAPISET_AppendListItemByTextBuffer(res_info.res_item_ptr[i].name,
            res_info.res_item_ptr[i].name_len,
            PNULL,
            TXT_COMMON_OK,
            STXT_SELECT,
            STXT_RETURN,
            ctrl_id,
            GUIITEM_STYLE_ONE_LINE_CHECK);

        }
        UnloadNetworkAccountInfo(&res_info);
        }
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_SK_SKB_ICON_PRESSED:
        {
            const GUILIST_ITEM_T *item_ptr = PNULL;
            GUILIST_ITEM_T  item = {0};/*lint !e64*/
            GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
            uint16 cur_index = 0;
            uint16 selected_num = 0;

            selected_num = GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);
            cur_index = GUILIST_GetCurItemIndex(ctrl_id);
            item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);
            if(PNULL == item_ptr)
            {
                recode = MMI_RESULT_FALSE;
                break;
            }
            if(GUILIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
            {
                GUILIST_SetSelectedItem(ctrl_id, cur_index, FALSE);
            }
            else
            {
                if (s_connection_setting_info.settinglink_num[dual_sys] + selected_num < MMICONNECTION_MAX_SETTING_NUM)
                {
                    GUILIST_SetSelectedItem(ctrl_id, cur_index, TRUE);
                }
            }

            selected_num = GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);

            is_res_data_exist = LoadNetworkAccountInfo(dual_sys, &res_info);

            if (!is_res_data_exist || PNULL == res_info.res_item_ptr)
            {
                UnloadNetworkAccountInfo(&res_info);
                break;
            }
            GUILIST_ChangeCheckListSoftkeyIndex(ctrl_id, MIDDLE_BUTTON);

            for(i = 0; i<res_info.res_num; i++)
            {
                item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, i);

                if (GUILIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
                {
                    middle_softkey_id = STXT_CANCEL;
                }
                else
                {
                    if (s_connection_setting_info.settinglink_num[dual_sys] + selected_num < MMICONNECTION_MAX_SETTING_NUM)
                    {
                        middle_softkey_id = STXT_SELECT;
                    }
                    else
                    {
                        middle_softkey_id = TXT_NULL;
                    }
                }

                SCI_MEMSET(&item_data, 0x00, sizeof(GUILIST_ITEM_DATA_T));
                SCI_MEMCPY(&item_data, item_ptr->item_data_ptr, sizeof(GUILIST_ITEM_DATA_T));
                item_data.softkey_id[1] = middle_softkey_id;

                SCI_MEMSET(&item, 0x00, sizeof(GUILIST_ITEM_T));
                SCI_MEMCPY(&item, item_ptr, sizeof(GUILIST_ITEM_T));
                item.item_data_ptr = &item_data;
                GUILIST_ReplaceItem(ctrl_id, &item, i);
            }

            UnloadNetworkAccountInfo(&res_info);

            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
#ifndef MMI_PDA_SUPPORT
    case MSG_CTL_OK:
    case MSG_APP_OK:
        case MSG_APP_MENU:
        {
            const GUILIST_ITEM_T *item_ptr = NULL;
            BOOLEAN    modify_yes=FALSE;//��Ӧ���Ƿ��иĶ�
           // BOOLEAN    update_yes=FALSE;//�Ƿ��б䶯�������Ƿ�ҪдNV
            BOOLEAN is_need_adapt[MMICONNECTION_ACCOUNT_MAX] = {0};
            uint16 adapt_index = 0;

            if(s_connection_setting_info.settinglink_num[dual_sys] >= MMICONNECTION_MAX_SETTING_NUM)
            {
                MMIPUB_OpenAlertWarningWin(TXT_CONNECTION_LISTFULL);
                break;
            }

            is_res_data_exist = LoadNetworkAccountInfo(dual_sys, &res_info);

            if (!is_res_data_exist || PNULL == res_info.res_item_ptr)
            {
                UnloadNetworkAccountInfo(&res_info);
                break;
            }

            //Selete or deselect
            for(;i<res_info.res_num;i++)
            {
                item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,i);
                modify_yes=FALSE;

                if(GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED))
                {
                    modify_yes=TRUE;
                    //update_yes = TRUE;
                }
                if(modify_yes && s_connection_setting_info.settinglink_num[dual_sys]<MMICONNECTION_MAX_SETTING_NUM)
                {
                    uint16 index = s_connection_setting_info.settinglink_num[dual_sys];

                    SaveNetworkAccountInfo(&s_connection_setting_info.setting_detail[dual_sys][index],
                                                                &res_info.res_item_ptr[i], FALSE, TRUE, i);

                    MMICONNECTION_ClearHighPriList(dual_sys);
                    MMICONNECTION_SetHighPriList(dual_sys, (uint8)index);
                    s_connection_setting_info.settinglink_num[dual_sys] ++;

                    adapt_index = (uint16)res_info.res_item_ptr[i].account_type - (uint16)MMICONNECTION_ACCOUNT_BROWSER;
                    if (adapt_index < MMICONNECTION_ACCOUNT_MAX)
                    {
                        is_need_adapt[adapt_index] = TRUE;
                    }
                } //if(modify_yes)

                if(s_connection_setting_info.settinglink_num[dual_sys] > MMICONNECTION_MAX_SETTING_NUM)
                {
                    break;
                }
            //for(;i<res_info.res_num;i++)
//             if(update_yes)
//             {
//                 uint16 index = s_connection_setting_info.settinglink_num[dual_sys];
//
//                 //i = GUILIST_GetCurItemIndex(ctrl_id);
//                 //GUILIST_SetSelectedItem(ctrl_id, i, TRUE);
//
//                 SaveNetworkAccountInfo(&s_connection_setting_info.setting_detail[dual_sys][index],
//                                                             &res_info.res_item_ptr[i], FALSE, TRUE, i);
//
//                 MMICONNECTION_ClearHighPriList(dual_sys);
//                 MMICONNECTION_SetHighPriList(dual_sys, index);
//                 s_connection_setting_info.settinglink_num[dual_sys] ++;
//                 //update_yes = TRUE;
//
//                 adapt_index = (uint16)res_info.res_item_ptr[i].account_type - (uint16)MMICONNECTION_ACCOUNT_BROWSER;
//                 if (adapt_index < MMICONNECTION_ACCOUNT_MAX)
//                 {
//                     is_need_adapt[adapt_index] = TRUE;
//                 }
//             }

            }
            MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);

            for (i = 0; i < MMICONNECTION_ACCOUNT_MAX; i++)
            {
                if (is_need_adapt[i])
                {
                    MMICONNECTION_AdaptingAccount(dual_sys, (MMICONNECTION_ACCOUNT_TYPE_E)i);//Ĭ���б�����ӵ��ʻ���������һ���ʻ�ƥ��
                }
            }

            UnloadNetworkAccountInfo(&res_info);

#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            MMK_CloseWin(win_id);
        }
        break;
#endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#if !defined MMI_GUI_STYLE_TYPICAL
        {
            const GUILIST_ITEM_T *item_ptr = NULL;
            BOOLEAN    modify_yes=FALSE;//��Ӧ���Ƿ��иĶ�
            BOOLEAN    update_yes=FALSE;//�Ƿ��б䶯�������Ƿ�ҪдNV
            BOOLEAN is_need_adapt[MMICONNECTION_ACCOUNT_MAX] = {0};
            uint16 adapt_index = 0;

            if(s_connection_setting_info.settinglink_num[dual_sys] >= MMICONNECTION_MAX_SETTING_NUM)
            {
                MMK_CloseWin(win_id);
                break;
            }

            is_res_data_exist = LoadNetworkAccountInfo(dual_sys, &res_info);

            if (!is_res_data_exist || PNULL == res_info.res_item_ptr)
            {
                UnloadNetworkAccountInfo(&res_info);
                break;
            }

            //Selete or deselect
            for(;i<res_info.res_num;i++)
            {
                item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,i);
                modify_yes=FALSE;
                if(GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED))
                {
                    modify_yes=TRUE;
                    update_yes = TRUE;
                }
                if(modify_yes && s_connection_setting_info.settinglink_num[dual_sys]<MMICONNECTION_MAX_SETTING_NUM)
                {
                    uint16 index = s_connection_setting_info.settinglink_num[dual_sys];

                    SaveNetworkAccountInfo(&s_connection_setting_info.setting_detail[dual_sys][index],
                                                                &res_info.res_item_ptr[i], FALSE, TRUE, i);

                    MMICONNECTION_ClearHighPriList(dual_sys);
                    MMICONNECTION_SetHighPriList(dual_sys, (uint8)index);
                    s_connection_setting_info.settinglink_num[dual_sys] ++;

                    adapt_index = (uint16)res_info.res_item_ptr[i].account_type - (uint16)MMICONNECTION_ACCOUNT_BROWSER;
                    if (adapt_index < MMICONNECTION_ACCOUNT_MAX)
                    {
                        is_need_adapt[adapt_index] = TRUE;
                    }
                } //if(modify_yes)

                if(s_connection_setting_info.settinglink_num[dual_sys] > MMICONNECTION_MAX_SETTING_NUM)
                {
                    break;
                }
            }//for(;i<res_info.res_num;i++)


            MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);

            for (i = 0; i < MMICONNECTION_ACCOUNT_MAX; i++)
            {
                if (is_need_adapt[i])
                {
                    MMICONNECTION_AdaptingAccount(dual_sys, (MMICONNECTION_ACCOUNT_TYPE_E)i);//Ĭ���б�����ӵ��ʻ���������һ���ʻ�ƥ��
                }
            }

            UnloadNetworkAccountInfo(&res_info);
    //        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);
        }
#else
        MMK_CloseWin(win_id);
#endif
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode ;
}
#endif

/*****************************************************************************/
//  Description :setting link list choose win handle fun
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWapSettingListWinMsg(
                                         MMI_WIN_ID_T win_id,
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM param
                                           )
{
    MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
    uint16  link_index                                   = 0;
    uint16  apn_index                                    = 0;
    MN_DUAL_SYS_E  dual_sys                              = MN_DUAL_SYS_MAX;
    uint8 index                                          = 0;
    uint8 cur_index                                   = 0;
    uint8 list_index                                     = 0;
    uint16 sim_default_index                             = 0;
    MMI_CTRL_ID_T ctrl_id = MMICONNECTION_LIST_SETTING_CTRL_ID;
    uint32 add_data       = (uint32)MMK_GetWinAddDataPtr(win_id);

    dual_sys = (add_data)&0xFF;/*lint !e64*/
    sim_default_index = (add_data & 0xFF00) >> 8;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //��̬���LIST ITEM
        cur_index = GUILIST_GetCurItemIndex(MMIMMS_SETTING_MENU_CTRL_ID);
        GUILIST_SetMaxItem( ctrl_id, MMICONNECTION_MAX_SETTING_NUM, FALSE );//max item 8
#ifndef MMI_PDA_SUPPORT
        GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
#endif
		GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

        for(index = 0; index < s_connection_setting_info.settinglink_num[dual_sys]; index++)                              //��ʾLIST
        {
            if(TRUE == s_connection_setting_info.setting_detail[dual_sys][index].visible)
            {
                MMI_IMAGE_ID_T image_id = IMAGE_CONNECTION_LINK_ITEM_RW;
                
                if(s_connection_setting_info.setting_detail[dual_sys][index].is_customed_detail)
                {
                    image_id = IMAGE_CONNECTION_LINK_ITEM_RO;
                }
                
                AppendOneLineTextListItem(ctrl_id,
                    GUIITEM_STYLE_ONE_LINE_TEXT_MS,
                    TXT_NULL,
                    STXT_SOFTKEY_SELECT_MK,
                    PNULL,
                    s_connection_setting_info.setting_detail[dual_sys][index].name,
                    s_connection_setting_info.setting_detail[dual_sys][index].name_len,
                    list_index,
                    FALSE
                    );
                list_index ++;
            }
        }
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        GUILIST_SetSelectedItem(ctrl_id,sim_default_index,TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,sim_default_index);

        if(cur_index==0)
        {
            GUIWIN_SetTitleTextId(win_id, STR_SIM1_MMS_APN_EXT01, TRUE);
        }
        else if (cur_index==1)
        {
            GUIWIN_SetTitleTextId(win_id, STR_SIM2_MMS_APN_EXT01, TRUE);
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_GET_FOCUS:
        link_index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_RemoveAllItems(ctrl_id);
        
        GUILIST_SetMaxItem(ctrl_id, MMICONNECTION_MAX_SETTING_NUM,FALSE);
        for (index = 0; index < s_connection_setting_info.settinglink_num[dual_sys]; index++)
        {
            if(TRUE == s_connection_setting_info.setting_detail[dual_sys][index].visible)
            {
                MMI_IMAGE_ID_T image_id = IMAGE_CONNECTION_LINK_ITEM_RW;
                if(s_connection_setting_info.setting_detail[dual_sys][index].is_customed_detail)
                {
                    image_id = IMAGE_CONNECTION_LINK_ITEM_RO;
                }
                AppendOneLineTextListItem(ctrl_id,
                    //GUIITEM_STYLE_ONE_LINE_RADIO_ICON,
                    GUIITEM_STYLE_ONE_LINE_TEXT_MS,
                    //   STXT_OPTION,
                    PNULL,
                    STXT_SOFTKEY_SELECT_MK,
                    // image_id,
                    PNULL,
                    s_connection_setting_info.setting_detail[dual_sys][index].name,
                    s_connection_setting_info.setting_detail[dual_sys][index].name_len,
                    list_index,
                    FALSE
                    );
                list_index ++;
            }
        }
        
        GUILIST_SetSelectedItem(ctrl_id, link_index, TRUE);
        break;

//     case MSG_CTL_OK:
//     case MSG_APP_OK:
//     case MSG_APP_MENU:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        link_index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_SetSelectedItem(ctrl_id, link_index, TRUE);
        apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, link_index);
        if (PNULL != s_connection_callback_func_ptr)
        {
            (*s_connection_callback_func_ptr)(dual_sys, apn_index);
        }
        MMK_CloseWin(win_id);

        break;

//     case MSG_CTL_OK:
//     case MSG_APP_OK:
//     case MSG_APP_MENU:
//         s_connection_handling_sim_id = dual_sys;
//         MMK_CreateWin((uint32 *)MMICONNECTION_SETTINGLINKLISTMENU_WIN_TAB, PNULL);
//         break;
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        s_connection_handling_sim_id = dual_sys;
        MMK_CreateWin((uint32 *)MMICONNECTION_LINKLIST_LONGOKMENU_WIN_TAB, PNULL);
        break;
#endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo �رմ���
        MMK_CloseWin(win_id);
        break;

    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }

    return err_code;
}


#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description : Setting link menu
//	Global resource dependence :
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void  InitPdaSettingLinkListCtrl(MMICONNECTION_LINKSETTING_DETAIL_T* setting_detail)
{
    MMI_CTRL_ID_T ctrl_id = MMICONNECTION_MENU_SETTINGLINK_CTRL_ID;
    uint8 temp_str[MMICONNECT_STRNUM_MAX_LEN + 2] = {0};
    wchar temp_wstr[MMICONNECT_STRNUM_MAX_LEN + 2] = {0};
    MMI_STRING_T text_str= {0};
//#ifndef MMI_DUALMODE_ENABLE
    MMI_TEXT_ID_T text_id = TXT_NULL;
//#endif
    GUILIST_POS_INFO_T pos_info = {0};

    GUILIST_GetCurPosInfo(ctrl_id, &pos_info);
    GUILIST_RemoveAllItems(ctrl_id);

    if(PNULL == setting_detail)
    {
       //SCI_TRACE_LOW:"setting_detail is null"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_3336_112_2_18_2_12_12_6,(uint8*)"");
       return;
    }
    //set max item
    GUILIST_SetMaxItem(ctrl_id, 10, FALSE );

    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    text_str.wstr_ptr = setting_detail->name;
    text_str.wstr_len = MMIAPICOM_Wstrlen(setting_detail->name) ;
    AppendPdaConnectionItem2Line(
        ctrl_id,
        TXT_COMMON_NAME,
        text_str,
        IMAGE_NULL,
        (uint32)ID_CONNECTION_SETTING_CHANGENAME
        );

    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    text_str.wstr_len = MIN(setting_detail->username_len, MMICONNECT_STRNUM_MAX_LEN);
    MMI_STRNTOWSTR(temp_wstr, MMICONNECT_STRNUM_MAX_LEN, setting_detail->username, text_str.wstr_len, text_str.wstr_len);
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr) ;
    AppendPdaConnectionItem2Line(
        ctrl_id,
        TXT_COMMON_USERNAME,
        text_str,
        IMAGE_NULL,
        (uint32)ID_CONNECTION_SETTING_USERNAME
        );

    {//display password as '*'
        uint16  i = 0;

        SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
        text_str.wstr_len = SCI_STRLEN((char*)(setting_detail->password)) ;
        text_str.wstr_len = MIN(text_str.wstr_len, MMICONNECT_STRNUM_MAX_LEN);
        for (i = 0; i < text_str.wstr_len; i++)
        {
            temp_wstr[i] = 0x2A;//*
        }
        text_str.wstr_ptr = temp_wstr;
        AppendPdaConnectionItem2Line(
            ctrl_id,
            TXT_COMMON_PASSWORD,
            text_str,
            IMAGE_NULL,
            (uint32)ID_CONNECTION_SETTING_PASSWORD
            );
    }

    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    text_str.wstr_len = MIN(setting_detail->apn_len, MMICONNECT_STRNUM_MAX_LEN);
    MMI_STRNTOWSTR(temp_wstr, MMICONNECT_STRNUM_MAX_LEN, setting_detail->apn, text_str.wstr_len, text_str.wstr_len);
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr) ;
    AppendPdaConnectionItem2Line(
        ctrl_id,
        TXT_CONNECTION_APN,
        text_str,
        IMAGE_NULL,
        (uint32)ID_CONNECTION_SETTING_APN
        );

// #ifdef MMI_DUALMODE_ENABLE
    if(0 == (uint16)setting_detail->auth_type)
    {
       text_id = TXT_CONNECTION_AUTHTYPE_PAP;
    }
    else if(1 == (uint16)setting_detail->auth_type)
    {
       text_id = TXT_CONNECTION_AUTHTYPE_CHAP;
    }
    else if (2 == (uint16)setting_detail->auth_type)
    {
        text_id = TXT_CONNECTION_AUTHTYPE_PAPORCHAP;
    }
    else if (3 == (uint16)setting_detail->auth_type)
    {
        text_id = TXT_NONE;
    }

    MMI_GetLabelTextByLang(text_id, &text_str);
    AppendPdaConnectionItem2Line(
    ctrl_id,
    TXT_CONNECTION_AUTH_TYPE,
    text_str,
    IMAGE_COMMON_RIGHT_ARROW,
    (uint32)ID_CONNECTION_SETTING_SECRET
    );
// #endif

#ifdef IPVERSION_SUPPORT_V4_V6
	if(MMICONNECTION_IP_V4V6 == (uint16)setting_detail->ip_type)
    {
		text_id = TXT_COMMON_IPV4V6;
    }
    else if(MMICONNECTION_IP_V4 == (uint16)setting_detail->ip_type)
    {
		text_id = TXT_COMMON_IPV4;
    }
    else
    {
        text_id = TXT_COMMON_IPV6;
    }
    MMI_GetLabelTextByLang(text_id, &text_str);
    AppendPdaConnectionItem2Line(
		ctrl_id,
		TXT_CONNECTION_IP_TYPE,
		text_str,
#ifdef TOUCH_PANEL_SUPPORT
		IMAGE_COMMON_RIGHT_ARROW,
#else
		IMAGE_NULL,
#endif
		(uint32)ID_CONNECTION_SETTING_IPTYPE
		);
#endif

    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    text_str.wstr_len = MIN(setting_detail->gateway_len, MMICONNECT_STRNUM_MAX_LEN);
    MMI_STRNTOWSTR(temp_wstr, MMICONNECT_STRNUM_MAX_LEN, setting_detail->gateway, text_str.wstr_len, text_str.wstr_len);
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr) ;
    AppendPdaConnectionItem2Line(
        ctrl_id,
        TXT_COMMON_GATEWAY,
        text_str,
        IMAGE_NULL,
        (uint32)ID_CONNECTION_SETTING_GATEWAY
        );

    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    sprintf((char *)temp_str, "%d", setting_detail->port);
    text_str.wstr_len = SCI_STRLEN((char*)temp_str);
    text_str.wstr_len = MIN(text_str.wstr_len, MMICONNECT_STRNUM_MAX_LEN);
    MMI_STRNTOWSTR(temp_wstr, MMICONNECT_STRNUM_MAX_LEN, temp_str, text_str.wstr_len, text_str.wstr_len);
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr) ;
    AppendPdaConnectionItem2Line(
        ctrl_id,
        TXT_CONNECTION_PORT,
        text_str,
        IMAGE_NULL,
        (uint32)ID_CONNECTION_SETTING_PORT
        );

    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    text_str.wstr_len = MIN(setting_detail->homepage_len, MMICONNECT_STRNUM_MAX_LEN);
    MMI_STRNTOWSTR(temp_wstr, MMICONNECT_STRNUM_MAX_LEN, setting_detail->homepage, text_str.wstr_len, text_str.wstr_len);
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr) ;
    AppendPdaConnectionItem2Line(
        ctrl_id,
        TXT_CONNECTION_SETHOMEPAGE,
        text_str,
        IMAGE_NULL,
        (uint32)ID_CONNECTION_SETTING_SETHOMEPAGE
        );

    AppendPdaConnectionItem1Line(
        ctrl_id,
        TXT_CONNECTION_ADVANCE,
        (uint32)ID_CONNECTION_SETTING_ADVANCE
        );

    GUILIST_SetCurPosInfo(ctrl_id, &pos_info);
}

/*****************************************************************************/
// 	Description : handle message of setting link list window of browser
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkMenuWinMsg(
                                               MMI_WIN_ID_T 	win_id,
                                               MMI_MESSAGE_ID_E	msg_id,
                                               DPARAM		    param
                                               )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint32          input_param=0;
    static  MMICONNECTION_LINKSETTING_DETAIL_T* temp_setting_detail=PNULL;
    uint16			list_index = 0;
    MMICONNECTION_LINKSET_OP_E op=MMICONNECTION_LINKSET_OP_NULL;
    CONNECTION_MMI_TEXT_ID_E	text_id[] = {TXT_CONNECTION_SETTING1,TXT_CONNECTION_SETTING2,TXT_CONNECTION_SETTING3,TXT_CONNECTION_SETTING4,TXT_CONNECTION_SETTING5,\
        TXT_CONNECTION_SETTING6,TXT_CONNECTION_SETTING7,TXT_CONNECTION_SETTING8,TXT_CONNECTION_SETTING9,TXT_CONNECTION_SETTING10,TXT_CONNECTION_SETTING11,TXT_CONNECTION_SETTING12,\
        TXT_CONNECTION_SETTING13,TXT_CONNECTION_SETTING14,TXT_CONNECTION_SETTING15,TXT_CONNECTION_SETTING16,TXT_CONNECTION_SETTING17,TXT_CONNECTION_SETTING18,TXT_CONNECTION_SETTING19,TXT_CONNECTION_SETTING20}; //wenbao.bai 2007.10.30
    MMI_STRING_T    string_info={0};
    MN_DUAL_SYS_E dual_sys = s_connection_handling_sim_id;
    MMI_WIN_ID_T linkMenu_dialog_id = MMICONNECTION_SETTINGLINKMENU_WIN_ID;

    MMI_STRING_T    access_point_name = {0};
    wchar text_temp[32] = {0};
    wchar old_str_N[16] = {0};
    wchar replace_str[16] = {0};
    wchar replace_str_num[8] = {0};

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(MMICONNECTION_SETTINGLINKMENU_WIN_ID,MMICONNECTION_MENU_SETTINGLINK_CTRL_ID);
        temp_setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T*)SCI_ALLOCA(sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
        if(temp_setting_detail==PNULL)
        {
            MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
            MMK_CloseWin(win_id);
        }
        else
        {
            input_param = (uint32)MMK_GetWinAddDataPtr(win_id);
            op=(MMICONNECTION_LINKSET_OP_E )(input_param>>16);
            list_index = input_param&0xFFFF;//��16λ��ʾ�˵��е�list_index��Ҳ����ȫ�ֱ���������
            SCI_MEMSET(temp_setting_detail,0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));

            if(op==MMICONNECTION_LINKSET_OP_EDIT || op==MMICONNECTION_LINKSET_OP_READ)
            {
                SCI_MEMCPY(temp_setting_detail,&s_connection_setting_info.setting_detail[dual_sys][list_index],sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            }
            else if(op==MMICONNECTION_LINKSET_OP_NEW)
            {
                uint32 i = 0;
                uint32 j = 0;
                uint16 text_num = sizeof(text_id)/sizeof(text_id[0]);
                BOOLEAN is_name_exist = FALSE;

                for (i = 0; i < text_num; i++)
                {
                    MMI_GetLabelTextByLang(STR_ACCESS_POINT_EXT01, &access_point_name);
                    MMIAPICOM_Wstrncpy(text_temp, access_point_name.wstr_ptr,access_point_name.wstr_len);
                    MMIAPICOM_Wstrncpy(old_str_N,L"%N",MMIAPICOM_Wstrlen(L"%N"));
                    if(i > 8)
                    {
                        SCI_MEMSET(replace_str,0,sizeof(replace_str));
                        SCI_MEMSET(replace_str_num,0,sizeof(replace_str_num));
                        itoa(i-9,replace_str_num,10);
                        MMIAPICOM_Wstrcat(replace_str,L"1");
                        MMIAPICOM_Wstrcat(replace_str,replace_str_num);
                    }
                    else
                    {
                        itoa(i+1,replace_str,10);
                    }
                    MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), old_str_N, replace_str);
                    //MMI_GetLabelTextByLang(text_id[i],&string_info);
                    string_info.wstr_ptr= text_temp;
                    string_info.wstr_len= MMIAPICOM_Wstrlen(string_info.wstr_ptr);
                    string_info.wstr_len = MIN(string_info.wstr_len, MMICONNECTION_MAX_LINKMODENAME_LEN);

                    is_name_exist = FALSE;

                    for (j = 0; j < s_connection_setting_info.settinglink_num[dual_sys]; j++)
                    {
                        if (string_info.wstr_len == s_connection_setting_info.setting_detail[dual_sys][j].name_len
                            && 0 == memcmp(s_connection_setting_info.setting_detail[dual_sys][j].name, string_info.wstr_ptr, string_info.wstr_len*sizeof(wchar))
                            && TRUE == s_connection_setting_info.setting_detail[dual_sys][j].visible)
                        {
                            is_name_exist = TRUE;
                            break;
                        }
                    }

                    if (!is_name_exist)
                    {
                        break;
                    }
                }

                if (!is_name_exist)
                {
                    MMIAPICOM_Wstrncpy(temp_setting_detail->name,string_info.wstr_ptr,string_info.wstr_len);
                    temp_setting_detail->name_len = string_info.wstr_len;
                }

                /* added by plum peng for cr125973 */
                temp_setting_detail->access_type = 1;
                temp_setting_detail->uiorder = 5;
                temp_setting_detail->auth_type = MN_AUTH_NONE;
                temp_setting_detail->visible = TRUE;
            }

            GUIWIN_SetTitleText(win_id, temp_setting_detail->name, temp_setting_detail->name_len, FALSE);
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_CANCEL, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_EDIT, STXT_RETURN, FALSE);
#endif
        }
        InitPdaSettingLinkListCtrl(temp_setting_detail);
        break;


    case MSG_GET_FOCUS:
        InitPdaSettingLinkListCtrl(temp_setting_detail);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
#ifdef MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

            if (MSG_CTL_PENOK == msg_id
                && MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_CTL_OK, PNULL);
            }
            else if (MSG_CTL_PENOK == msg_id
                && MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_CTL_CANCEL, PNULL);
            }
            else
            {
#endif
#if defined MMI_GPRS_SUPPORT
                uint16 index = 0;
                uint32 user_data  = 0;
	       MMI_STRING_T temp_str={0};
                index = GUILIST_GetCurItemIndex(MMICONNECTION_MENU_SETTINGLINK_CTRL_ID);
                GUILIST_GetItemData(MMICONNECTION_MENU_SETTINGLINK_CTRL_ID, index, &user_data);

                switch((MMICONNECTION_ID_MENU_E)user_data)//�˵��ڵ�ID
                {
#ifdef IPVERSION_SUPPORT_V4_V6
                case ID_CONNECTION_SETTING_IPTYPE:
                    //MMK_CreatePubListWin((uint32*)MMICONNECTION_SETTINGIPTYPE_WIN_TAB, (ADD_DATA)temp_setting_detail);
                    MMK_CreateWin((uint32 *)MMICONNECTION_SETTINGIPTYPE_WIN_TAB,(ADD_DATA)temp_setting_detail);
    		        break;
#endif
//                 case ID_CONNECTION_SETTING_ACCOUNTTYPE:
//                     MMK_CreateWin((uint32 *)MMICONNECTION_SETTINGACCOUNTTYPE_WIN_TAB,(ADD_DATA)temp_setting_detail);
//                     break;

                case ID_CONNECTION_SETTING_SECRET:
                    MMK_CreatePubListWin((uint32*)MMICONNECTION_SETTINGSECURITYCONNECTLIST_WIN_TAB, (ADD_DATA)temp_setting_detail);
                    break;

                case ID_CONNECTION_SETTING_USERNAME://�û���:
                    //MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTUSERNAME_WIN_TAB, (ADD_DATA)temp_setting_detail);//�򿪱༭�û�������
                    linkMenu_dialog_id = MMICONNECTION_INPUTUSERNAME_WIN_ID;
		  MMI_GetLabelTextByLang(TXT_COMMON_USERNAME, &temp_str);
		  MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN,HandleInputUserNameWinMsg);
		  MMK_SetWinUserData (linkMenu_dialog_id, (void *) temp_setting_detail);
                    break;

                case ID_CONNECTION_SETTING_PASSWORD://����:
                    //MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTPASSWORD_WIN_TAB, (ADD_DATA)temp_setting_detail);//�򿪱༭���봰��
                    linkMenu_dialog_id = MMICONNECTION_INPUTPASSWORD_WIN_ID;
		  MMI_GetLabelTextByLang(TXT_COMMON_PASSWORD, &temp_str);
		  //MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_CUSTOMER,HandleInputPasswordWinMsg);
		  MMIPUB_OpenDialogPasswordWinByTextPtr(&temp_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN ,HandleInputPasswordWinMsg);
		  MMK_SetWinUserData (linkMenu_dialog_id, (void *) temp_setting_detail);
                    break;

                case ID_CONNECTION_SETTING_APN://���������:
                    //MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTAPN_WIN_TAB, (ADD_DATA)temp_setting_detail);//�򿪱༭�����������
                    linkMenu_dialog_id = MMICONNECTION_INPUTAPN_WIN_ID;
		  MMI_GetLabelTextByLang(TXT_CONNECTION_APN, &temp_str);
		  MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN,HandleInputApnWinMsg);
		  MMK_SetWinUserData (linkMenu_dialog_id, (void *) temp_setting_detail);
                    break;

                case ID_CONNECTION_SETTING_GATEWAY://����IP:
                    //MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTGATEWAY_WIN_TAB, (ADD_DATA)temp_setting_detail);//�򿪱༭����IP����
                    linkMenu_dialog_id = MMICONNECTION_INPUTGATEWAY_WIN_ID;
		  MMI_GetLabelTextByLang(TXT_COMMON_GATEWAY, &temp_str);
		  MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN,HandleInputGatewayWinMsg);
		  MMK_SetWinUserData (linkMenu_dialog_id, (void *) temp_setting_detail);
                    break;

                case ID_CONNECTION_SETTING_PORT://���ض˿�:
                    //MMK_CreatePubEditWin((uint32*)MMICONNECTION_PORTLIST_WIN_TAB, (ADD_DATA)temp_setting_detail);//��ѡ�����ض˿ڴ���
                    linkMenu_dialog_id = MMICONNECTION_EDITPORT_WIN_ID;
		  MMI_GetLabelTextByLang(TXT_CONNECTION_PORT, &temp_str);
		  MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN,HandlePortListWinMsg);
		  MMK_SetWinUserData (linkMenu_dialog_id, (void *) temp_setting_detail);
                    break;

                case ID_CONNECTION_SETTING_CHANGENAME://��������:
                    //MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTAGENTNAME_WIN_TAB, (ADD_DATA)temp_setting_detail);//�򿪱༭�������ƴ���
                    linkMenu_dialog_id = MMICONNECTION_INPUTAGENTNAME_WIN_ID;
		  MMI_GetLabelTextByLang(TXT_COMMON_NAME, &temp_str);
		  MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN,HandleInputAgentNameWinMsg);
		  MMK_SetWinUserData (linkMenu_dialog_id, (void *) temp_setting_detail);
                    break;

                case ID_CONNECTION_SETTING_SETHOMEPAGE:   //������ҳ����
                    //MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTHOMEPAGE_WIN_TAB, (ADD_DATA)temp_setting_detail);
                    linkMenu_dialog_id = MMICONNECTION_INPUTHOMEPAGE_WIN_ID;
		  MMI_GetLabelTextByLang(TXT_CONNECTION_SETHOMEPAGE, &temp_str);
		  MMIPUB_OpenDialogInputFieldWinByTextPtrLen(&temp_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN,MMICONNECTION_HOMEPAGE_MAX_URL_LEN,HandleInputHomepageWinMsg);
		  MMK_SetWinUserData (linkMenu_dialog_id, (void *) temp_setting_detail);
                    break;

                case ID_CONNECTION_SETTING_ADVANCE:
                    MMK_CreateWin((uint32 *)MMICONNECTION_SETTINGLINK_ADVANCE_WIN_TAB, (ADD_DATA)temp_setting_detail);
                    break;

                case ID_CONNECTION_SETTING_DNS:
                    //MMK_CreatePubEditWin((uint32 *)MMICONNECTION_SETTINGLINK_DNS_WIN_TAB, (ADD_DATA)temp_setting_detail);
                    linkMenu_dialog_id = MMICONNECTION_SETTINGLINK_DNS_WIN_ID;
		  MMI_GetLabelTextByLang(TXT_CONNECTION_DNS, &temp_str);
		  MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN,HandleSettingLinkDNSWinMsg);
		  MMK_SetWinUserData (linkMenu_dialog_id, (void *) temp_setting_detail);
                    break;
                case ID_CONNECTION_SETTING_NETTYPE:
                    MMK_CreateWin((uint32 *)MMICONNECTION_SETTINGLINK_NETTYPE_WIN_TAB,(ADD_DATA)temp_setting_detail);
                    break;

                default:
                    break;
                }
#endif
#ifdef MMI_PDA_SUPPORT
            }
#endif
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        /*lulifei modify for the UI spec
        input_param = (uint32)MMK_GetWinAddDataPtr(win_id);
        op=(MMICONNECTION_LINKSET_OP_E )(input_param>>16);
        list_index = input_param&0xFFFF;//��16λ��ʾ�˵��е�list_index��Ҳ����ȫ�ֱ���������

        if(0 == memcmp(&s_connection_setting_info.setting_detail[dual_sys][list_index],temp_setting_detail,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            MMIPUB_OpenQueryWinByTextId(TXT_EXIT_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }*/
        MMK_CloseWin(win_id);
        break;
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
        if (temp_setting_detail->name_len == 0)
        {
            MMIPUB_OpenAlertWarningWin(TXT_ENVSET_EMPTY_NAME);
        }
        else
        {
            input_param = (uint32)MMK_GetWinAddDataPtr(win_id);
            op = (MMICONNECTION_LINKSET_OP_E )(input_param>>16);
            list_index = input_param&0xFFFF;//��16λ��ʾ�˵��е�list_index��Ҳ����ȫ�ֱ���������
            if (MMICONNECTION_LINKSET_OP_READ != op)
            {
                if (0 != memcmp(&s_connection_setting_info.setting_detail[dual_sys][list_index],temp_setting_detail,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)))
                {
                    SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][list_index],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                    SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][list_index],temp_setting_detail,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                    s_connection_setting_info.setting_detail[dual_sys][list_index].is_default_unchange = FALSE;
                    s_connection_setting_info.setting_detail[dual_sys][list_index].list_index = 0;
                    s_connection_setting_info.setting_detail[dual_sys][list_index].hplmn = 0;
                }
                if (MMICONNECTION_LINKSET_OP_NEW == op)
                {
                    s_connection_setting_info.settinglink_num[dual_sys]++;
                }

                MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);

                if (MMICONNECTION_LINKSET_OP_EDIT == op)
                {
                    ChangeAppSelectedIndex(dual_sys, list_index, MMICONNECTION_NOTIFY_EVENT_EDIT_ITEM);
                }
            }

            MMK_CloseWin(win_id);
            MMK_CloseWin(MMICONNECTION_SETTINGLINKLISTMENU_WIN_ID);
            if (MMK_IsOpenWin(MMICONNECTION_LINKLIST_WIN_ID))
            {
                const GUILIST_ITEM_T *item_ptr = PNULL;
                BOOLEAN is_select              = FALSE;
                MMI_IMAGE_ID_T image_id        = IMAGE_CONNECTION_LINK_ITEM_RW;
                MMI_CTRL_ID_T ctrl_id          = MMICONNECTION_LIST_SETTING_CTRL_ID;
                uint16  link_index             = 0;
                uint16  apn_link_index         = 0;
                
                link_index = GUILIST_GetCurItemIndex(ctrl_id);
                apn_link_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, link_index);
                if (link_index < MMICONNECTION_MAX_SETTING_NUM)
                {
                    item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,link_index);
                    is_select = GUILIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED);
                    if(s_connection_setting_info.setting_detail[dual_sys][apn_link_index].is_customed_detail)
                    {
                        image_id = IMAGE_CONNECTION_LINK_ITEM_RO;
                    }
                    AppendOneLineTextListItem(ctrl_id,
                        GUIITEM_STYLE_ONE_LINE_RADIO_ICON,
                        PNULL,
                        PNULL,
                        image_id,
                        s_connection_setting_info.setting_detail[dual_sys][apn_link_index].name,
                        s_connection_setting_info.setting_detail[dual_sys][apn_link_index].name_len,
                        link_index,
                        TRUE
                        );
                    if (is_select)
                    {
                        //GUILIST_SetItemState((GUILIST_ITEM_T *)item_ptr, GUIITEM_STATE_SELECTED, TRUE);
						CTRLLIST_SetSelectedItem(ctrl_id, link_index, TRUE);
                    }
                }

            }
        }
        break;
    case MSG_CLOSE_WINDOW:
        if(temp_setting_detail!=PNULL)
        {
            SCI_FREE(temp_setting_detail);
            temp_setting_detail = PNULL;
        }
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

#else
/*****************************************************************************/
// 	Description : handle message of setting link list window of browser
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkMenuWinMsg(
                                               MMI_WIN_ID_T 	win_id,
                                               MMI_MESSAGE_ID_E	msg_id,
                                               DPARAM		    param
                                               )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint32          input_param=0;
    static  MMICONNECTION_LINKSETTING_DETAIL_T* temp_setting_detail=PNULL;
    uint32			group_id = 0;
    uint32			menu_id = 0;
    uint16			list_index = 0;
    uint16			apn_index = 0;
    MMICONNECTION_LINKSET_OP_E op=MMICONNECTION_LINKSET_OP_NULL;
    CONNECTION_MMI_TEXT_ID_E	text_id[] = {TXT_CONNECTION_SETTING1,TXT_CONNECTION_SETTING2,TXT_CONNECTION_SETTING3,TXT_CONNECTION_SETTING4,TXT_CONNECTION_SETTING5,\
        TXT_CONNECTION_SETTING6,TXT_CONNECTION_SETTING7,TXT_CONNECTION_SETTING8,TXT_CONNECTION_SETTING9,TXT_CONNECTION_SETTING10,TXT_CONNECTION_SETTING11,TXT_CONNECTION_SETTING12,\
        TXT_CONNECTION_SETTING13,TXT_CONNECTION_SETTING14,TXT_CONNECTION_SETTING15,TXT_CONNECTION_SETTING16,TXT_CONNECTION_SETTING17,TXT_CONNECTION_SETTING18,TXT_CONNECTION_SETTING19,TXT_CONNECTION_SETTING20}; //wenbao.bai 2007.10.30
    MMI_STRING_T    string_info={0};
    MN_DUAL_SYS_E dual_sys = s_connection_handling_sim_id;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(MMICONNECTION_SETTINGLINKMENU_WIN_ID,MMICONNECTION_MENU_SETTINGLINK_CTRL_ID);
        temp_setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T*)SCI_ALLOCA(sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
        if(temp_setting_detail==PNULL)
        {
            MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
            MMK_CloseWin(win_id);
        }
        else
        {
            input_param = (uint32)MMK_GetWinAddDataPtr(win_id);
            op=(MMICONNECTION_LINKSET_OP_E )(input_param>>16);
            list_index = input_param&0xFFFF;//��16λ��ʾ�˵��е�list_index��Ҳ����ȫ�ֱ���������
            apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, list_index);
            
            SCI_MEMSET(temp_setting_detail,0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));

            if(op==MMICONNECTION_LINKSET_OP_EDIT || op==MMICONNECTION_LINKSET_OP_READ)
            {
                SCI_MEMCPY(temp_setting_detail,&s_connection_setting_info.setting_detail[dual_sys][apn_index],sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            }
            else if(op==MMICONNECTION_LINKSET_OP_NEW)
            {
                uint32 i = 0;
                uint32 j = 0;
                uint16 text_num = sizeof(text_id)/sizeof(text_id[0]);
                BOOLEAN is_name_exist = FALSE;
                string_info.wstr_ptr = SCI_ALLOCAZ((MMICONNECTION_MAX_LINKMODENAME_LEN + 2)*sizeof(wchar));
                for (i = 0; i < text_num; i++)
                {
                     SCI_MEMSET(string_info.wstr_ptr, 0x00, (MMICONNECTION_MAX_LINKMODENAME_LEN + 2)*sizeof(wchar));
                    MMI_GetLabelTextByLang(text_id[i],&string_info);

                    string_info.wstr_len = MIN(string_info.wstr_len, MMICONNECTION_MAX_LINKMODENAME_LEN);

                    is_name_exist = FALSE;

                    for (j = 0; j < s_connection_setting_info.settinglink_num[dual_sys]; j++)
                    {
                        if (string_info.wstr_len == s_connection_setting_info.setting_detail[dual_sys][j].name_len
                            && 0 == memcmp(s_connection_setting_info.setting_detail[dual_sys][j].name, string_info.wstr_ptr, string_info.wstr_len*sizeof(wchar))
                            && TRUE == s_connection_setting_info.setting_detail[dual_sys][j].visible)
                        {
                            is_name_exist = TRUE;
                            break;
                        }
                    }

                    if (!is_name_exist)
                    {
                        break;
                    }
                }

                if (!is_name_exist)
                {
                    MMIAPICOM_Wstrncpy(temp_setting_detail->name,string_info.wstr_ptr,string_info.wstr_len);
                    temp_setting_detail->name_len = string_info.wstr_len;
                }

                /* added by plum peng for cr125973 */
                temp_setting_detail->access_type = 1;
                temp_setting_detail->visible = TRUE;
            }


#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_EDIT, STXT_RETURN, FALSE);
#endif
        SCI_FREE(string_info.wstr_ptr);
	string_info.wstr_ptr = PNULL;
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
#ifdef MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

            if (MSG_CTL_PENOK == msg_id
                && MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_CTL_OK, PNULL);
            }
            else if (MSG_CTL_PENOK == msg_id
                && MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_CTL_CANCEL, PNULL);
            }
            else
            {
#endif
#if defined MMI_GPRS_SUPPORT
		  GUIMENU_GetId(MMICONNECTION_MENU_SETTINGLINK_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
                switch(menu_id)//�˵��ڵ�ID
                {

                case ID_CONNECTION_SETTING_SECRET:
                    MMK_CreatePubListWin((uint32*)MMICONNECTION_SETTINGSECURITYCONNECTLIST_WIN_TAB, (ADD_DATA)temp_setting_detail);
                    break;

                case ID_CONNECTION_SETTING_USERNAME://�û���:
                    MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTUSERNAME_WIN_TAB, (ADD_DATA)temp_setting_detail);//�򿪱༭�û�������
                    break;

                case ID_CONNECTION_SETTING_PASSWORD://����:
                    MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTPASSWORD_WIN_TAB, (ADD_DATA)temp_setting_detail);//�򿪱༭���봰��
                    break;

                case ID_CONNECTION_SETTING_APN://���������:
                    MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTAPN_WIN_TAB, (ADD_DATA)temp_setting_detail);//�򿪱༭�����������
                    break;

                case ID_CONNECTION_SETTING_GATEWAY://����IP:
                    MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTGATEWAY_WIN_TAB, (ADD_DATA)temp_setting_detail);//�򿪱༭����IP����
                    break;

                case ID_CONNECTION_SETTING_PORT://���ض˿�:
                    MMK_CreatePubEditWin((uint32*)MMICONNECTION_PORTLIST_WIN_TAB, (ADD_DATA)temp_setting_detail);//��ѡ�����ض˿ڴ���
                    break;

                case ID_CONNECTION_SETTING_CHANGENAME://��������:
                    MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTAGENTNAME_WIN_TAB, (ADD_DATA)temp_setting_detail);//�򿪱༭�������ƴ���
                    break;

                case ID_CONNECTION_SETTING_SETHOMEPAGE:   //������ҳ����
                    MMK_CreatePubEditWin((uint32*)MMICONNECTION_INPUTHOMEPAGE_WIN_TAB, (ADD_DATA)temp_setting_detail);
                    break;

                case ID_CONNECTION_SETTING_ADVANCE:
                    MMK_CreateWin((uint32 *)MMICONNECTION_SETTINGLINK_ADVANCE_WIN_TAB, (ADD_DATA)temp_setting_detail);
                    break;

                default:
                    break;
                }
#endif
#ifdef MMI_PDA_SUPPORT
            }
#endif
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        input_param = (uint32)MMK_GetWinAddDataPtr(win_id);
        op=(MMICONNECTION_LINKSET_OP_E )(input_param>>16);
        list_index = input_param&0xFFFF;//��16λ��ʾ�˵��е�list_index��Ҳ����ȫ�ֱ���������
        //�Ƿ��޸�, ADD_NEW is modified
        if(0 == memcmp(&s_connection_setting_info.setting_detail[dual_sys][apn_index],temp_setting_detail,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
        	MMIPUB_OpenAlertWarningWin(TXT_EXIT_QUERY);
            //MMIPUB_OpenQueryWinByTextId(TXT_EXIT_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }
        break;
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
        if (0 == temp_setting_detail->name_len)
        {
            MMIPUB_OpenAlertWarningWin(TXT_ENVSET_EMPTY_NAME);
        }
        else
        {
            input_param = (uint32)MMK_GetWinAddDataPtr(win_id);
            op=(MMICONNECTION_LINKSET_OP_E )(input_param>>16);
            list_index = input_param&0xFFFF;//��16λ��ʾ�˵��е�list_index��Ҳ����ȫ�ֱ���������
            apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, list_index);
            if (op != MMICONNECTION_LINKSET_OP_READ)
            {
                if (0 != memcmp(&s_connection_setting_info.setting_detail[dual_sys][apn_index],temp_setting_detail,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)))
                {
                    SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][apn_index],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                    SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][apn_index],temp_setting_detail,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                    s_connection_setting_info.setting_detail[dual_sys][apn_index].is_default_unchange = FALSE;
                    s_connection_setting_info.setting_detail[dual_sys][apn_index].list_index = 0;
                    s_connection_setting_info.setting_detail[dual_sys][apn_index].hplmn = 0;
                }
                if (op == MMICONNECTION_LINKSET_OP_NEW)
                {
                    s_connection_setting_info.settinglink_num[dual_sys]++;
                }

                MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);

                if (op == MMICONNECTION_LINKSET_OP_EDIT)
                {
                    ChangeAppSelectedIndex(dual_sys, apn_index, MMICONNECTION_NOTIFY_EVENT_EDIT_ITEM);
                }
            }
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMICONNECTION_SETTINGLINKLISTMENU_WIN_ID);
        }
        break;
    case MSG_CLOSE_WINDOW:
        if(temp_setting_detail!=PNULL)
        {
            SCI_FREE(temp_setting_detail);
            temp_setting_detail = PNULL;
        }
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
#endif

#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : handle message of setting link list window of browser
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkDetailViewWinMsg(
                                                     MMI_WIN_ID_T	win_id,
                                                     MMI_MESSAGE_ID_E	msg_id,
                                                     DPARAM				param
                                                     )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMICONNECTION_RICHTEXT_SETTINGLINKDETAILVIEW_CTRL_ID;
    uint16			list_index = 0;
    uint16			apn_index = 0;
    uint32          input_param=0;
    MN_DUAL_SYS_E dual_sys = s_connection_handling_sim_id;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }

    input_param = (uint32)MMK_GetWinAddDataPtr(win_id);
    apn_index = input_param&0xFFFF;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetSettingDetailEntry(ctrl_id, &s_connection_setting_info.setting_detail[dual_sys][apn_index]);
        GUIRICHTEXT_SetLineSpace(ctrl_id, MMICONNECTION_DETAIL_LINE_SPACE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
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
// 	Description : handle message of setting security connect list window of browser
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingSecurityconnectListWinMsg(
                                                          MMI_WIN_ID_T  	win_id,
                                                          MMI_MESSAGE_ID_E	msg_id,
                                                          DPARAM			param
                                                          )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint16			list_index = 0;
    uint16          i=0;
    MMI_CTRL_ID_T   ctrl_id = MMICONNECTION_LIST_SETTINGSECURITYCONNECT_CTRL_ID;
    CONNECTION_MMI_TEXT_ID_E	text_id[4] = {TXT_CONNECTION_AUTHTYPE_PAP, TXT_CONNECTION_AUTHTYPE_CHAP, TXT_CONNECTION_AUTHTYPE_PAPORCHAP, TXT_NONE};/*lint !e64*/
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail=(MMICONNECTION_LINKSETTING_DETAIL_T*)MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(MMICONNECTION_SETTINGSECURITYCONNECTLIST_WIN_ID,MMICONNECTION_LIST_SETTINGSECURITYCONNECT_CTRL_ID);
        list_index = (uint16)setting_detail->auth_type;
        //��̬���LIST ITEM
        GUILIST_SetMaxItem( ctrl_id,4,FALSE );//max item 4
        for(i = 0;i<4;i++)
        {
#if !defined MMI_GUI_STYLE_TYPICAL
            MMIAPISET_AppendListItemByTextId(text_id[i], TXT_NULL, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT );
#else
            MMIAPISET_AppendListItemByTextId(text_id[i], TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT );
#endif
        }
        GUILIST_SetSelectedItem(ctrl_id,list_index,TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,list_index);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        list_index = GUILIST_GetCurItemIndex(ctrl_id);
        setting_detail->auth_type = (uint32)list_index;//(MN_PCO_AUTH_TYPE_E)list_index;
        MMK_CloseWin(win_id);//������һ��
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//������һ��
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

#ifdef IPVERSION_SUPPORT_V4_V6
/*****************************************************************************/
// 	Description : handle message of setting ip type connect list window of browser
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingIpTypeconnectListWinMsg(
                                                          MMI_WIN_ID_T  	win_id,
                                                          MMI_MESSAGE_ID_E	msg_id,
                                                          DPARAM			param
                                                          )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint16			list_index = 0;
    uint16          i=0;
    MMI_CTRL_ID_T   ctrl_id = MMICONNECTION_LIST_SETTINGIPTYPE_CTRL_ID;
    CONNECTION_MMI_TEXT_ID_E	text_id[3] = {TXT_COMMON_IPV4,TXT_COMMON_IPV4V6, TXT_COMMON_IPV6};/*lint !e64*/
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail=(MMICONNECTION_LINKSETTING_DETAIL_T*)MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //list_index = (uint16)setting_detail->ip_type;
        //��̬���LIST ITEM
        GUILIST_SetMaxItem( ctrl_id,3,FALSE );//max item 2
        for(i = 0;i<3;i++)
        {
#if !defined MMI_GUI_STYLE_TYPICAL
            MMIAPISET_AppendListItemByTextId(text_id[i], TXT_NULL, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT );
#else
            MMIAPISET_AppendListItemByTextId(text_id[i], TXT_NULL, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT );
#endif
        }
        //dong.chunguang modify as nv define 
        switch(setting_detail->ip_type){
            case 0:
                //ipv4
                list_index = 0;
                break;
            case 1:
                //ipv6
                list_index = 2;
                break;
            case 2:
                //ipv4v6
                list_index = 1;
                break;
            default:
                break;
        }
        //end
        GUILIST_SetSelectedItem(ctrl_id,list_index,TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,list_index);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        list_index = GUILIST_GetCurItemIndex(ctrl_id);
        //setting_detail->ip_type= (uint32)list_index;//(MN_PCO_AUTH_TYPE_E)list_index;
        //set nv of the ip protocol
        switch(list_index){
            case 0:
                //ipv4
                setting_detail->ip_type = MMICONNECTION_IP_V4;
                break;
            case 1:
                //ipv4v6
                setting_detail->ip_type = MMICONNECTION_IP_V4V6;
                break;
            case 2:
                //ipv6
                setting_detail->ip_type = MMICONNECTION_IP_V6;
                break;
            default:
                break;
        }
        //end
        MMK_CloseWin(win_id);//������һ��
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//������һ��
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
#endif

/*****************************************************************************/
// 	Description : deal with account type setting
//	Global resource dependence : none
//  Author: zhongping.shi
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingAccountTypeconnectListWinMsg(MMI_WIN_ID_T win_id,
                                                             MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint16			list_index = 0;
    uint16          i = 0;
    MMI_CTRL_ID_T   ctrl_id = MMICONNECTION_LIST_SETTINGACCOUNTTYPE_CTRL_ID;
    CONNECTION_MMI_TEXT_ID_E	text_id[5] = {TXT_CONNECTION_INTERNET, TXT_CONNECTION_MMS, TXT_CONNECTION_IA,
                                              TXT_CONNECTION_IMS, TXT_CONNECTION_XCAP};
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T*)MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(ctrl_id, 5, FALSE);
            for (i = 0; i < 5; i++)
            {
                MMIAPISET_AppendListItemByTextId(text_id[i], TXT_NULL, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT );
            }
            
            switch (setting_detail->account_type)
            {
                case MMICONNECTION_ACCOUNT_BROWSER:
                    list_index = 0;
                    break;
                    
                case MMICONNECTION_ACCOUNT_MMS:
                    list_index = 1;
                    break;
                    
                case MMICONNECTION_ACCOUNT_IA:
                    list_index = 2;
                    break;
                    
                case MMICONNECTION_ACCOUNT_IMS:
                    list_index = 3;
                    break;
                    
                case MMICONNECTION_ACCOUNT_XCAP:
                    list_index = 4;
                    break;
                    
                default:
                    break;
            }

            GUILIST_SetSelectedItem(ctrl_id, list_index, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, list_index);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_APP_OK:
        case MSG_CTL_OK:
            list_index = GUILIST_GetCurItemIndex(ctrl_id);
            switch (list_index)
            {
                case 0:
                    setting_detail->account_type = MMICONNECTION_ACCOUNT_BROWSER;
                    break;
                    
                case 1:
                    setting_detail->account_type = MMICONNECTION_ACCOUNT_MMS;
                    break;
                    
                case 2:
                    setting_detail->account_type = MMICONNECTION_ACCOUNT_IA;
                    break;
                    
                case 3:
                    setting_detail->account_type = MMICONNECTION_ACCOUNT_IMS;
                    break;
                    
                case 4:
                    setting_detail->account_type = MMICONNECTION_ACCOUNT_XCAP;
                    break;
                    
                default:
                    break;
            }
            
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
#endif

#if defined MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : handle message of input username window of browser
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputUserNameWinMsg(
                                             MMI_WIN_ID_T	win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_STRING_T string_info = {0};
    MMI_CTRL_ID_T   ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail=(MMICONNECTION_LINKSETTING_DETAIL_T*)MMK_GetWinUserData(win_id);
    wchar ucs2buf[MMICONNECTION_MAX_USERNAME_LEN+1] = {0};
    uint16 ucs2buf_len = 0;
    GUIIM_TYPE_T allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T init_im = GUIIM_TYPE_NONE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set im
        GUIEDIT_SetTextMaxLen(ctrl_id,MMICONNECTION_MAX_USERNAME_LEN/2, MMICONNECTION_MAX_USERNAME_LEN);
        allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
        init_im = GUIIM_TYPE_ABC;
        GUIEDIT_SetIm(ctrl_id, allow_im, init_im);
        MMIAPICOM_StrToWstr((uint8 *)setting_detail->username, ucs2buf);
        //����ȱʡֵ
        GUIEDIT_SetString(ctrl_id,
            ucs2buf,
            setting_detail->username_len);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //�����û���
        GUIEDIT_GetString(ctrl_id,&string_info);
		if(0 == string_info.wstr_len) {
			break;
		}
        SCI_MEMSET(setting_detail->username,0,sizeof(setting_detail->username));
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8 *)ucs2buf);//ֵ��������ASCII�룬�������������
        ucs2buf_len = MIN(strlen((char*)ucs2buf),MMICONNECTION_MAX_USERNAME_LEN);
        SCI_MEMCPY(setting_detail->username,(uint8*)ucs2buf,ucs2buf_len);
        setting_detail->username_len = strlen((char*)ucs2buf);
        MMK_CloseWin(win_id);
        break;
    case MSG_FULL_PAINT:
#ifdef TOUCH_PANEL_SUPPORT
       MMK_SendMsg(MMICONNECTION_SETTINGLINKMENU_WIN_ID, MSG_FULL_PAINT, PNULL);
#endif
       GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
       GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,2,TRUE);
       break;

    //case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
     case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);//������һ��
        break;

    default:
        recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;
    }
    return recode;

}


/*****************************************************************************/
// 	Description : handle message of input password window of browser
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputPasswordWinMsg(
                                             MMI_WIN_ID_T	    win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_STRING_T   string_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T*)MMK_GetWinUserData(win_id);
    //MMI_CTRL_ID_T ctrl_id = MMIPUB_GetPasswordDialogCtrlId(win_id);
	MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );
    wchar tempbuf[MMICONNECTION_MAX_PASSWORD_LEN+1] = {0};
    uint16 tempbuf_len = 0;
    BOOLEAN is_disp_im_icon = TRUE;
    BOOLEAN is_disp_num_info = TRUE;
    GUIIM_TYPE_T allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T init_im = GUIIM_TYPE_NONE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set im
        GUIEDIT_SetTextMaxLen(ctrl_id,MMICONNECTION_MAX_PASSWORD_LEN/2, MMICONNECTION_MAX_PASSWORD_LEN);
        allow_im =   GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
        init_im = GUIIM_TYPE_ABC;
        GUIEDIT_SetIm(ctrl_id,allow_im,init_im);
        MMIPUB_SetInputDialogEditorType(win_id, DIALOG_PASSWORD_EDITOR);
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_MULTI);

        GUIEDIT_SetDispImIcon(ctrl_id, &is_disp_im_icon, &is_disp_num_info);

        MMIAPICOM_StrToWstr((uint8 *)setting_detail->password, tempbuf);
        //����ȱʡֵ
        GUIEDIT_SetString(ctrl_id,tempbuf,setting_detail->password_len);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //��������
        GUIEDIT_GetString(ctrl_id,&string_info);
		if(0 == string_info.wstr_len) {
			break;
		}
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8 *)tempbuf);//ֵ��������ASCII���������������
        SCI_MEMSET(setting_detail->password,0,sizeof(setting_detail->password));
        tempbuf_len = MIN(strlen((char*)tempbuf),MMICONNECTION_MAX_PASSWORD_LEN);
        SCI_MEMCPY(setting_detail->password,(uint8*)tempbuf,tempbuf_len);
        setting_detail->password_len = strlen((char*)tempbuf);
        MMK_CloseWin(win_id);
        break;
case MSG_FULL_PAINT:
#ifdef TOUCH_PANEL_SUPPORT
        MMK_SendMsg(MMICONNECTION_SETTINGLINKMENU_WIN_ID, MSG_FULL_PAINT, PNULL);
#endif
        GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,2,TRUE);
        break;

    //case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);//������һ��
        break;

    default:

        recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;
    }
    return recode;

}


/*****************************************************************************/
// 	Description : handle message of input apn window of browser
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputApnWinMsg(
                                        MMI_WIN_ID_T	win_id,
                                        MMI_MESSAGE_ID_E	msg_id,
                                        DPARAM				param
                                        )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_STRING_T string_info = {0};
    MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T*)MMK_GetWinUserData(win_id);
    wchar tempbuf[MMICONNECTION_MAX_APN_LEN+1] = {0};
    uint16 tempbuf_len = 0;
    GUIIM_TYPE_T allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T init_im = GUIIM_TYPE_NONE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set im
        GUIEDIT_SetTextMaxLen(ctrl_id,MMICONNECTION_MAX_APN_LEN/2, MMICONNECTION_MAX_APN_LEN);
        allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
        init_im = GUIIM_TYPE_ABC;
        GUIEDIT_SetIm(ctrl_id,allow_im,init_im);
        MMIAPICOM_StrToWstr((uint8 *)setting_detail->apn, tempbuf);
        //����ȱʡֵ
        GUIEDIT_SetString(ctrl_id,tempbuf,setting_detail->apn_len);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //������������
        GUIEDIT_GetString(ctrl_id,&string_info);
		if(0 == string_info.wstr_len) {
			break;
		}
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8 *)tempbuf);//ֻ��������ASCII���������������
        SCI_MEMSET(setting_detail->apn,0,sizeof(setting_detail->apn));
        tempbuf_len = MIN(strlen((char*)tempbuf),MMICONNECTION_MAX_APN_LEN);
        SCI_MEMCPY(setting_detail->apn,(uint8*)tempbuf,tempbuf_len);
        setting_detail->apn_len = strlen((char*)tempbuf);
        MMK_CloseWin(win_id);
        break;
    //case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);//������һ��
        break;
   case MSG_FULL_PAINT:
#ifdef TOUCH_PANEL_SUPPORT
   	MMK_SendMsg(MMICONNECTION_SETTINGLINKMENU_WIN_ID, MSG_FULL_PAINT, PNULL);
#endif
       GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
       GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,2,TRUE);
       break;

    default:

        recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;
    }
    return recode;

}


/*****************************************************************************/
// 	Description : handle message of input gateway window of browser
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputGatewayWinMsg(
                                            MMI_WIN_ID_T	win_id,
                                            MMI_MESSAGE_ID_E	msg_id,
                                            DPARAM				param
                                            )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T*)MMK_GetWinUserData(win_id);
    //MMI_CTRL_ID_T ctrl_id = MMICONNECTION_EDIT_INPUTGATEWAY_CTRL_ID;
    MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );
    MMI_STRING_T string_info = {0};
    wchar tempbuf[MMICONNECTION_MAX_GATEWAY_LEN + 1] = {0};
    char tempbuf_byte[MMICONNECTION_MAX_GATEWAY_LEN + 1] = {0};
    uint16 tempbuf_len = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		GUIEDIT_SetTextMaxLen(ctrl_id,MMICONNECTION_MAX_GATEWAY_LEN/2, MMICONNECTION_MAX_GATEWAY_LEN);
        MMIAPICOM_StrToWstr((uint8 *)setting_detail->gateway, tempbuf);
		GUIEDIT_SetString(ctrl_id,tempbuf,MMICONNECTION_MAX_GATEWAY_LEN);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIEDIT_GetString(ctrl_id, &string_info);
		if(0 == string_info.wstr_len) {
			break;
		}
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8 *)tempbuf_byte);//ֻ��������ASCII���������������
        SCI_MEMSET(setting_detail->gateway, 0, sizeof(setting_detail->gateway));
        tempbuf_len = strlen((char*)tempbuf_byte);
        SCI_MEMCPY(setting_detail->gateway,(uint8*)tempbuf_byte,tempbuf_len);
        setting_detail->gateway_len = strlen((char*)tempbuf_byte);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    //case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//������һ��
        break;
    case MSG_FULL_PAINT:
#ifdef TOUCH_PANEL_SUPPORT
   	 MMK_SendMsg(MMICONNECTION_SETTINGLINKMENU_WIN_ID, MSG_FULL_PAINT, PNULL);		
#endif
        GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,2,TRUE);
        break;

    default:
        recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;
    }
    return recode;

}


/*****************************************************************************/
// 	Description : handle message of port list window of browser
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortListWinMsg(
                                        MMI_WIN_ID_T	    win_id,
                                        MMI_MESSAGE_ID_E	msg_id,
                                        DPARAM				param
                                        )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint32          port_value = 0;
    uint8           port_str[MMICONNECTION_MAX_PORT_LEN+2] = {0};
    wchar           port_str2[MMICONNECTION_MAX_PORT_LEN+2] = {0};
    uint8			port_len = 0;
    MMI_STRING_T string_info= {0};
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T*)MMK_GetWinUserData(win_id);
    MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );
    GUIIM_TYPE_T         input_mode = GUIIM_TYPE_DIGITAL;
    wchar                   dot_str[] = {'.'};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
	GUIEDIT_SetTextMaxLen(ctrl_id,MMICONNECTION_MAX_PORT_LEN/2, MMICONNECTION_MAX_PORT_LEN);
        //����ȱʡֵ
        //if(setting_detail->port!=0)
         GUIEDIT_ConfigImCustomKey(ctrl_id, input_mode, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_DOT_NUM);
        MMIPUB_SetDialogBgLayerAlpha(win_id, 127);

        {
            _snprintf((char*)port_str, (MMICONNECTION_MAX_PORT_LEN+1),"%d", setting_detail->port);
            port_len=strlen((char*)port_str);
            MMIAPICOM_StrToWstr((uint8 *)port_str, port_str2);
            GUIEDIT_SetString(ctrl_id,port_str2,port_len);
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIEDIT_GetString(ctrl_id,&string_info);
        if(0 == string_info.wstr_len)
        {
            break;
        }
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8 *)port_str2);//ֻ�����������֣��������������
        port_value = atoi((char*)port_str2);
        if(MMICONNECTION_MAX_PORT_VALUE < port_value)
        {
            _snprintf((char*)port_str, (MMICONNECTION_MAX_PORT_LEN+1),"%d", setting_detail->port);
            port_len=strlen((char*)port_str);
            MMIAPICOM_StrToWstr((uint8 *)port_str, port_str2);
            GUIEDIT_SetString(ctrl_id,port_str2,port_len);
            MMIPUB_OpenAlertWarningWin(STR_AP_PORT_INVALID_EXT01);
        }
        else
        {
            setting_detail->port = (uint16)port_value;
            MMK_CloseWin(win_id);
        }


        break;
    case MSG_FULL_PAINT:
        GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,2,TRUE);
        GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
        break;
//    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);//������һ��
        break;

    default:

        recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        GUIEDIT_SetIm(ctrl_id,input_mode,input_mode);
        break;
    }
    return recode;
}


/*****************************************************************************/
// 	Description : handle message of input agent name window of browser
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputAgentNameWinMsg(
                                              MMI_WIN_ID_T  	win_id,
                                              MMI_MESSAGE_ID_E	msg_id,
                                              DPARAM			param
                                              )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_STRING_T string_info = {0};
    //MMI_CTRL_ID_T ctrl_id = MMICONNECTION_EDIT_INPUTAGENTNAME_CTRL_ID;
    MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T *)MMK_GetWinUserData(win_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetString(ctrl_id,setting_detail->name,setting_detail->name_len);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIEDIT_GetString(ctrl_id,&string_info);
	if(0 == string_info.wstr_len) {
		break;
	}
        SCI_MEMSET(setting_detail->name,0,sizeof(setting_detail->name));
        MMIAPICOM_Wstrncpy(setting_detail->name,string_info.wstr_ptr,string_info.wstr_len);
        setting_detail->name_len = string_info.wstr_len;
        MMK_CloseWin(win_id);
        break;
case MSG_FULL_PAINT:
#ifdef TOUCH_PANEL_SUPPORT
	 MMK_SendMsg(MMICONNECTION_SETTINGLINKMENU_WIN_ID, MSG_FULL_PAINT, PNULL);
#endif
        GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_BACKSPACE,2,TRUE);
        break;

//    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);//������һ��
        break;

    default:
        recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;
    }
    return recode;

}

/*****************************************************************************/
// 	Description : handle message of input homepage window of browser
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputHomepageWinMsg(
                                             MMI_WIN_ID_T	    win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_STRING_T string_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail =(MMICONNECTION_LINKSETTING_DETAIL_T *)MMK_GetWinUserData(win_id);
    //MMI_CTRL_ID_T ctrl_id = MMICONNECTION_EDIT_INPUTHOMEPAGE_CTRL_ID;
    MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );
    MMI_STRING_T		inputstr = {0};
    wchar tempbuf[MMICONNECTION_HOMEPAGE_MAX_URL_LEN+1]= {0};
    uint16 tempbuf_len = 0;
    GUIIM_TYPE_T allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T init_im = GUIIM_TYPE_NONE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set im
        allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
        init_im = GUIIM_TYPE_ABC;
        GUIEDIT_SetIm(ctrl_id,allow_im,init_im);
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);
        MMIAPICOM_StrToWstr((uint8 *)setting_detail->homepage, tempbuf);
        GUIEDIT_SetString(ctrl_id,tempbuf,setting_detail->homepage_len);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIEDIT_GetString(ctrl_id,&string_info);
		if(0 == string_info.wstr_len) {
			break;
		}
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8 *)tempbuf);//ֻ��������ASCII���������������
        SCI_MEMSET(setting_detail->homepage,0,sizeof(setting_detail->homepage));
        tempbuf_len = MIN(strlen((char*)tempbuf),MMICONNECTION_HOMEPAGE_MAX_URL_LEN);
        SCI_MEMCPY(setting_detail->homepage,tempbuf,tempbuf_len);
        setting_detail->homepage_len=strlen((char *)tempbuf);
        MMK_CloseWin(win_id);
        break;
//    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);//������һ��
        break;
    case MSG_FULL_PAINT:
        GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,2,TRUE);
        break;

    default:
        recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;
    }
    return recode;

}
#endif

#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description : Connection setting advance
//	Global resource dependence :
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void  InitPdaSettingAdvanceCtrl(MMICONNECTION_LINKSETTING_DETAIL_T* setting_detail)
{
    MMI_CTRL_ID_T       ctrl_id = MMICONNECTION_MENU_SETTINGLINK_ADVANCE_CTRL_ID;
    MMI_STRING_T text_str= {0};
    MMI_TEXT_ID_T    text_id = TXT_NULL;
    wchar temp_wstr[MMICONNECT_STRNUM_MAX_LEN + 2] = {0};
    uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);

    //set max item
    GUILIST_SetMaxItem(ctrl_id, 2, FALSE );
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str));
    MMIAPICOM_StrToWstr((uint8 *)setting_detail->dns, temp_wstr);
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr) ;
    AppendPdaConnectionItem2Line(
    ctrl_id,
    TXT_CONNECTION_DNS,
    text_str,
    IMAGE_NULL,
    (uint32)ID_CONNECTION_SETTING_DNS
    );

    SCI_MEMSET(&text_str, 0x00, sizeof(text_str));
    if(0 == setting_detail->access_type)
    {
        text_id = TXT_CONNECTION_NETTYPE_WAP;
    }
    else
    {
        text_id = TXT_CONNECTION_NETTYPE_HTTP;
    }
    MMI_GetLabelTextByLang(text_id, &text_str);
    AppendPdaConnectionItem2Line(
    ctrl_id,
    TXT_NETWORK_TYPE,
    text_str,
    IMAGE_COMMON_RIGHT_ARROW,
    (uint32)ID_CONNECTION_SETTING_NETTYPE
    );

    GUILIST_SetCurItemIndex(ctrl_id, cur_index);
}

#if defined MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : handle message of setting link advance window using list for pda
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkAdvanceWinMsg(
                                                  MMI_WIN_ID_T  	win_id,
                                                  MMI_MESSAGE_ID_E	msg_id,
                                                  DPARAM			param
                                                  )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T *)MMK_GetWinAddDataPtr(win_id);
    MN_DUAL_SYS_E dual_sys = s_connection_handling_sim_id;

    if (dual_sys >= MMI_DUAL_SYS_MAX /*&& MSG_CLOSE_WINDOW != msg_id*/)
    {
        //MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_RETURN, FALSE);
        InitPdaSettingAdvanceCtrl(setting_detail);
        MMK_SetAtvCtrl(win_id, MMICONNECTION_MENU_SETTINGLINK_ADVANCE_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
        InitPdaSettingAdvanceCtrl(setting_detail);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
            if (MSG_CTL_PENOK == msg_id
                && MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_CTL_OK, PNULL);
            }
            else if (MSG_CTL_PENOK == msg_id
                && MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_CTL_CANCEL, PNULL);
            }
            else
#endif
            {
                uint16 index = 0;
                uint32 user_data  = 0;
				MMI_WIN_ID_T linkMenu_dialog_id;
				wchar temp_str[32] = {0};

                index = GUILIST_GetCurItemIndex(MMICONNECTION_MENU_SETTINGLINK_ADVANCE_CTRL_ID);
                GUILIST_GetItemData(MMICONNECTION_MENU_SETTINGLINK_ADVANCE_CTRL_ID, index, &user_data);

                switch((MMICONNECTION_ID_MENU_E)user_data)
                {
                case ID_CONNECTION_SETTING_DNS:
//                    linkMenu_dialog_id = MMICONNECTION_SETTINGLINK_DNS_WIN_ID;
//					MMI_GetLabelTextByLang(TXT_CONNECTION_DNS, &temp_str);
//					MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_CUSTOMER,HandleSettingLinkDNSWinMsg);
					MMK_SetWinUserData (MMICONNECTION_SETTINGLINK_DNS_WIN_ID, (void *) setting_detail);

                    MMK_CreatePubEditWin((uint32 *)MMICONNECTION_SETTINGLINK_DNS_WIN_TAB, MMK_GetWinAddDataPtr(win_id));
                    break;
                case ID_CONNECTION_SETTING_NETTYPE:
                    MMK_CreatePubListWin((uint32 *)MMICONNECTION_SETTINGLINK_NETTYPE_WIN_TAB, MMK_GetWinAddDataPtr(win_id));
                    break;
                default:
                    break;
                }
            }
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        if (0 == setting_detail->name_len)
        {
            MMIPUB_OpenAlertWarningWin(TXT_ENVSET_EMPTY_NAME);
        }
        else
        {
            uint16 list_index = 0;
            uint32 input_param=0;
            MMICONNECTION_LINKSET_OP_E op=MMICONNECTION_LINKSET_OP_NULL;

            input_param = (uint32)MMK_GetWinAddDataPtr(MMICONNECTION_SETTINGLINKMENU_WIN_ID);
            op=(MMICONNECTION_LINKSET_OP_E )(input_param>>16);
            list_index = input_param&0xFFFF;//��16λ��ʾ�˵��е�list_index��Ҳ����ȫ�ֱ���������
            if (MMICONNECTION_LINKSET_OP_READ != op)
            {
                if (0 != memcmp(&s_connection_setting_info.setting_detail[dual_sys][list_index], setting_detail, sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)))
                {
                    SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][list_index],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                    SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][list_index],setting_detail,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                    s_connection_setting_info.setting_detail[dual_sys][list_index].is_default_unchange = FALSE;
                    s_connection_setting_info.setting_detail[dual_sys][list_index].list_index = 0;
                    s_connection_setting_info.setting_detail[dual_sys][list_index].hplmn = 0;
                }
                if (MMICONNECTION_LINKSET_OP_NEW == op)
                {
                    s_connection_setting_info.settinglink_num[dual_sys]++;
                }
                MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);

                if (MMICONNECTION_LINKSET_OP_EDIT == op)
                {
                    ChangeAppSelectedIndex(dual_sys, list_index, MMICONNECTION_NOTIFY_EVENT_EDIT_ITEM);
                }
            }
			//MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
			MMK_CloseWin(MMICONNECTION_SETTINGLINKMENU_WIN_ID);
			MMK_CloseWin(MMICONNECTION_SETTINGLINKLISTMENU_WIN_ID);
			MMK_CloseWin(win_id);
        }
        break;

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

#else
#if defined MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : handle message of setting link advance window of browser
//	Global resource dependence : none
//  Author: figo.feng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkAdvanceWinMsg(
                                                  MMI_WIN_ID_T  	win_id,
                                                  MMI_MESSAGE_ID_E	msg_id,
                                                  DPARAM			param
                                                  )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint32			group_id = 0;
    uint32			menu_id = 0;
    MMI_STRING_T title = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T *)MMK_GetWinAddDataPtr(win_id);
    MN_DUAL_SYS_E dual_sys = s_connection_handling_sim_id;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMI_GetLabelTextByLang(TXT_CONNECTION_ADVANCE, &title);
            GUIMENU_SetMenuTitle(&title, MMICONNECTION_MENU_SETTINGLINK_ADVANCE_CTRL_ID);
            MMK_SetAtvCtrl(win_id, MMICONNECTION_MENU_SETTINGLINK_ADVANCE_CTRL_ID);
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            GUIMENU_GetId(MMICONNECTION_MENU_SETTINGLINK_ADVANCE_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
            switch (menu_id)
            {
                case ID_CONNECTION_SETTING_DNS:
                    MMK_CreatePubEditWin((uint32 *)MMICONNECTION_SETTINGLINK_DNS_WIN_TAB, MMK_GetWinAddDataPtr(win_id));
                    break;
                case ID_CONNECTION_SETTING_NETTYPE:
                    MMK_CreatePubListWin((uint32 *)MMICONNECTION_SETTINGLINK_NETTYPE_WIN_TAB, MMK_GetWinAddDataPtr(win_id));
                    break;
                default:
                    break;
            }
            break;
            
        case MSG_APP_OK:
        case MSG_CTL_OK:
            if (0 == setting_detail->name_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_ENVSET_EMPTY_NAME);
            }
            else
            {
                uint16 list_index = 0;
                uint32 input_param=0;
                MMICONNECTION_LINKSET_OP_E op = MMICONNECTION_LINKSET_OP_NULL;

                input_param = (uint32)MMK_GetWinAddDataPtr(MMICONNECTION_SETTINGLINKMENU_WIN_ID);
                op = (MMICONNECTION_LINKSET_OP_E )(input_param>>16);
                list_index = input_param&0xFFFF;//��16λ��ʾ�˵��е�list_index��Ҳ����ȫ�ֱ���������
                if (MMICONNECTION_LINKSET_OP_READ != op)
                {
                    if (0 != memcmp(&s_connection_setting_info.setting_detail[dual_sys][list_index],setting_detail,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)))
                    {
                        SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][list_index],0,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                        SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][list_index],setting_detail,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                        s_connection_setting_info.setting_detail[dual_sys][list_index].is_default_unchange = FALSE;
                        s_connection_setting_info.setting_detail[dual_sys][list_index].list_index = 0;
                        s_connection_setting_info.setting_detail[dual_sys][list_index].hplmn = 0;
                    }
                    if(MMICONNECTION_LINKSET_OP_NEW == op)
                    {
                        s_connection_setting_info.settinglink_num[dual_sys]++;
                    }
                    MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);

                    if (MMICONNECTION_LINKSET_OP_EDIT == op)
                    {
                        ChangeAppSelectedIndex(dual_sys, list_index, MMICONNECTION_NOTIFY_EVENT_EDIT_ITEM);
                    }
                }
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMICONNECTION_SETTINGLINKMENU_WIN_ID);
                MMK_CloseWin(MMICONNECTION_SETTINGLINKLISTMENU_WIN_ID);
            }
            break;

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
#endif

#if defined MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : handle message of setting dns window of browser
//	Global resource dependence : none
//  Author: bei.wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkDNSWinMsg(
                                              MMI_WIN_ID_T	win_id,
                                              MMI_MESSAGE_ID_E	msg_id,
                                              DPARAM				param
                                              )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    //lulifei modify for UI spec
    MMI_CTRL_ID_T ctrl_id = MMICONNECTION_EDIT_INPUT_DNS_CTRL_ID;
	MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T *)MMK_GetWinAddDataPtr(win_id);
	
    GUIIM_TYPE_T         input_mode = GUIIM_TYPE_DIGITAL;
    wchar                   dot_str[] = {'.'};
    uint32 temp =0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

        MMIAPICONNECTION_IpStrToNum(setting_detail->dns, &temp);
        GUIEDIT_SetIP (ctrl_id,temp);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        temp = GUIEDIT_GetIP(ctrl_id);
        //change uint32 to string
        MMIAPICONNECTION_IpNumToStr(temp, (uint8 *)&setting_detail->dns);
        setting_detail->dns_len = SCI_STRLEN((char*)setting_detail->dns);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL://yongsheng.wang bug133637
        MMK_CloseWin(win_id);//������һ��
        break;
    case MSG_FULL_PAINT:
       GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
       GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,2,TRUE);
       break;

    default:
		recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;

}

/*****************************************************************************/
// 	Description : handle message of setting net type window of browser
//	Global resource dependence : none
//  Author: bei.wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingLinkNetTypeWinMsg(
												 MMI_WIN_ID_T	    win_id,
												 MMI_MESSAGE_ID_E	msg_id,
												 DPARAM				param
												 )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMICONNECTION_LIST_SELECT_NETTYPE_CTRL_ID;
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_detail = (MMICONNECTION_LINKSETTING_DETAIL_T *)MMK_GetWinAddDataPtr(win_id);
    uint16 list_index = 0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, 2, FALSE);
#ifdef MMI_PDA_SUPPORT
        MMIAPISET_AppendListItemByTextId(TXT_CONNECTION_NETTYPE_WAP, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_CONNECTION_NETTYPE_HTTP, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#else
        MMIAPISET_AppendListItemByTextId(TXT_CONNECTION_NETTYPE_WAP, TXT_NULL, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_CONNECTION_NETTYPE_HTTP, TXT_NULL, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
        GUILIST_SetSelectedItem(ctrl_id, setting_detail->access_type,TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, setting_detail->access_type);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        list_index = GUILIST_GetCurItemIndex(ctrl_id);
        setting_detail->access_type = list_index;
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);//������һ��
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
#endif

/*****************************************************************************/
// 	Description : set the select list of the searchresult list box to high priority
//	Global resource dependence :
//  Author:wenbao.bai
//	Note:
/****************************************************************************/
LOCAL void MMICONNECTION_SetHighPriList(MN_DUAL_SYS_E dual_sys, uint8 list_index)
{
    if (list_index >= MMICONNECTION_MAX_SETTING_NUM || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    s_connection_setting_info.setting_detail[dual_sys][list_index].priority = TRUE;
}
/*****************************************************************************/
// 	Description :clear the priority of the setting link lists box
//	Global resource dependence :
//  Author:wenbao.bai
//	Note:
/****************************************************************************/
LOCAL void MMICONNECTION_ClearHighPriList(MN_DUAL_SYS_E dual_sys)
{
    uint8 i = 0;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    for(i = 0;i<s_connection_setting_info.settinglink_num[dual_sys]; i ++)
    {
        s_connection_setting_info.setting_detail[dual_sys][i].priority = FALSE;
    }
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//     Description : to handle window message
//    Global resource dependence :
//  Author:wancan.you
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  ConnectionSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode  = MMI_RESULT_TRUE;
    if (isReturnOK)
    {
        s_connection_handling_sim_id = (MN_DUAL_SYS_E)dual_sys;

        MMK_CreateWin((uint32 *)MMICONNECTION_SETTINGLIST_WIN_TAB, PNULL);
    }
    return recode;
}
#endif
#endif

/*****************************************************************************/
// 	Description : �����б��޸�����
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_ChangeSettingListLanguage(void)
{
#ifdef MMI_GPRS_SUPPORT
    uint16 i = 0;
    uint32 dual_sys = 0;
    BOOLEAN is_res_data_exist = FALSE;
    MMICONNECTION_RES_INFO_T res_info = {0};

    //SCI_TRACE_LOW:"MMIAPICONNECTION_ChangeSettingListLanguage"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_4987_112_2_18_2_12_15_7,(uint8*)"" );

    for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        is_res_data_exist = LoadNetworkAccountInfo(dual_sys, &res_info);

        if (!is_res_data_exist || PNULL == res_info.res_item_ptr)
        {
            UnloadNetworkAccountInfo(&res_info);
            continue;
        }

        for(i = 0; i < s_connection_setting_info.settinglink_num[dual_sys]; i++)
        {
            if (s_connection_setting_info.setting_detail[dual_sys][i].is_default_unchange)
            {
                uint16 j = s_connection_setting_info.setting_detail[dual_sys][i].list_index;

                if (j < res_info.res_num)
                {
                    s_connection_setting_info.setting_detail[dual_sys][i].name_len = MIN(res_info.res_item_ptr[j].name_len, MMICONNECTION_MAX_LINKMODENAME_LEN);
                    MMIAPICOM_Wstrncpy(s_connection_setting_info.setting_detail[dual_sys][i].name,res_info.res_item_ptr[j].name,s_connection_setting_info.setting_detail[dual_sys][i].name_len);
                }
            }
        }

        UnloadNetworkAccountInfo(&res_info);
    }
    MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);
#endif
}

/*****************************************************************************/
// 	Description : �����б�����
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_ResetSettingList(BOOLEAN is_clean_user_data)
{
#ifdef MMI_GPRS_SUPPORT
    uint16 i = 0;
    uint16 reset_list_num = 0;
    uint16 custom_list_num = 0;
    uint32 dual_sys = 0;
    MMICONNECTION_RES_INFO_T res_info = {0};
    BOOLEAN is_res_data_exist = FALSE;
    const uint8 *customed_index_ptr = PNULL;
    BOOLEAN visible = FALSE;
    uint8 visible_num = 0;
#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
		MN_PLMN_T carrier_plmn[MMI_DUAL_SYS_MAX] = {0};
#endif

    //SCI_TRACE_LOW:"MMICONNECTION_ResetSettingList is_clean_user_data=%d, settinglink_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_5026_112_2_18_2_12_16_8,(uint8*)"dd", is_clean_user_data, s_connection_setting_info.settinglink_num[dual_sys]);

    if (is_clean_user_data)
    {
        SCI_MEMSET(&s_connection_setting_info, 0, (sizeof(s_connection_setting_info)));
    }

    for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        is_res_data_exist = LoadNetworkAccountInfo(dual_sys, &res_info);

        if (!is_res_data_exist || PNULL == res_info.res_item_ptr)
        {
            UnloadNetworkAccountInfo(&res_info);
            continue;
        }

        if (res_info.is_res_data)
        {
            custom_list_num = LINK_CUSTOMED_RES_LEN;
            customed_index_ptr = (const uint8 *)s_customed_res_index;
        }
        else
        {
            custom_list_num = LINK_CUSTOMED_LEN;
            customed_index_ptr = (const uint8 *)s_customed_index;
        }

        custom_list_num = MIN(custom_list_num, res_info.res_num);
        custom_list_num = MIN(custom_list_num, MMICONNECTION_MAX_SETTING_NUM);


        for(i = 0; i < s_connection_setting_info.settinglink_num[dual_sys]; i++)
        {
            if (is_clean_user_data)
            {
                uint16 j = customed_index_ptr[i];
                BOOLEAN is_customed_detail = FALSE;

                if (j >= res_info.res_num)
                {
                    break;
                }

                if (MMICONNECTION_ACCESS_READONLY == res_info.res_item_ptr[j].access_option)
                {
                    is_customed_detail = TRUE;
                }
                else
                {
                    is_customed_detail = FALSE;
                }

                visible = SaveNetworkAccountInfo(&s_connection_setting_info.setting_detail[dual_sys][i],
                                                 &res_info.res_item_ptr[j], TRUE, j);
				
				if (FALSE == visible)
				{
                    visible_num ++;
				}

                //MMICONNECTION_SetHighPriList(dual_sys, (uint8)i);
            }
            else if (s_connection_setting_info.setting_detail[dual_sys][i].is_default_unchange)
            {
                uint16 j = s_connection_setting_info.setting_detail[dual_sys][i].list_index;

                if (j < res_info.res_num && PNULL != res_info.res_item_ptr[j].name && 0 != res_info.res_item_ptr[j].name_len)
                {
                    s_connection_setting_info.setting_detail[dual_sys][i].name_len = MIN(res_info.res_item_ptr[j].name_len, MMICONNECTION_MAX_LINKMODENAME_LEN);
                    MMIAPICOM_Wstrncpy(s_connection_setting_info.setting_detail[dual_sys][i].name,res_info.res_item_ptr[j].name,s_connection_setting_info.setting_detail[dual_sys][i].name_len);
                }
            }
#ifdef IPVERSION_SUPPORT_V4_V6
            if(MMICONNECTION_IsPolandSimCard(dual_sys))
            {
                s_connection_setting_info.setting_detail[dual_sys][i].ip_type = MMICONNECTION_IP_V6;
            }else
                s_connection_setting_info.setting_detail[dual_sys][i].ip_type = MMICONN_GetIP_PortocolType();
#endif
        }
        
        s_connection_setting_info.invisible_apn_num[dual_sys] = visible_num;
        
        UnloadNetworkAccountInfo(&res_info);
    }
	{//reset apn default
		uint8 cur_index = 0;
		MMINV_WRITE(MMINV_CONNECTION_VODAFONE1_APN_DEFAULT, &cur_index);
		MMINV_WRITE(MMINV_CONNECTION_SIM1_APN_DEFAULT, &cur_index);
		MMINV_WRITE(MMINV_CONNECTION_VODAFONE2_APN_DEFAULT, &cur_index);
		MMINV_WRITE(MMINV_CONNECTION_SIM2_APN_DEFAULT, &cur_index);
	}
    //reset PLMN
#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
	SCI_MEMSET(carrier_plmn, 0x00, sizeof(carrier_plmn));
	MMINV_WRITE(MMINV_CARRIER_PLMN, carrier_plmn);
#endif
#ifdef DERECT_LINK_FIRST_SUPPORT
    MMICONNECTION_SetDerectLinkToFirstIndex(&s_connection_setting_info);
#endif
    MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);
#endif
}

/*****************************************************************************/
// 	Description : �����б�����
//	Global resource dependence : reset dual_sys network account
//    Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_ResetSettingListEx(MN_DUAL_SYS_E dual_sys, BOOLEAN is_clean_user_data)
{
#ifdef MMI_GPRS_SUPPORT
    uint16 i = 0;
    uint16 reset_list_num = 0;
    uint16 custom_list_num = LINK_CUSTOMED_LEN;
    BOOLEAN is_res_data_exist = FALSE;
    MMICONNECTION_RES_INFO_T res_info = {0};
    const uint8 *customed_index_ptr = PNULL;
    BOOLEAN visible = FALSE;
    uint8 visible_num = 0;
    
    is_res_data_exist = LoadNetworkAccountInfo(dual_sys, &res_info);

    //SCI_TRACE_LOW:"MMICONNECTION_ResetSettingList is_clean_user_data=%d, settinglink_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_5123_112_2_18_2_12_16_9,(uint8*)"dd", is_clean_user_data, s_connection_setting_info.settinglink_num[dual_sys]);

    if (!is_res_data_exist || PNULL == res_info.res_item_ptr)
    {
        UnloadNetworkAccountInfo(&res_info);
        return;
    }

    if (res_info.is_res_data)
    {
        custom_list_num = LINK_CUSTOMED_RES_LEN;
        customed_index_ptr = (const uint8 *)s_customed_res_index;
    }
    else
    {
        custom_list_num = LINK_CUSTOMED_LEN;
        customed_index_ptr = (const uint8 *)s_customed_index;
    }

    custom_list_num = MIN(custom_list_num, res_info.res_num);
    custom_list_num = MIN(custom_list_num, MMICONNECTION_MAX_SETTING_NUM);

    if (is_clean_user_data)
    {
        reset_list_num = s_connection_setting_info.settinglink_num[dual_sys];

        s_connection_setting_info.settinglink_num[dual_sys] = 0;
        SCI_MEMSET(s_connection_setting_info.setting_detail[dual_sys], 0, (sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)*reset_list_num));
    }

    for(i = 0; i < s_connection_setting_info.settinglink_num[dual_sys]; i++)
    {
        if (is_clean_user_data)
        {
            uint16 j = customed_index_ptr[i];
            BOOLEAN is_customed_detail = FALSE;

            if (j >= res_info.res_num)
            {
                break;
            }

            if (MMICONNECTION_ACCESS_READONLY == res_info.res_item_ptr[j].access_option)
            {
                is_customed_detail = TRUE;
            }
            else
            {
                is_customed_detail = FALSE;
            }

            visible = SaveNetworkAccountInfo(&s_connection_setting_info.setting_detail[dual_sys][i],
                                             &res_info.res_item_ptr[j], TRUE, j);
			
            if (FALSE == visible)
            {
                visible_num ++;
            }

            //MMICONNECTION_SetHighPriList(dual_sys, (uint8)i);
        }
        else if (s_connection_setting_info.setting_detail[dual_sys][i].is_default_unchange)
        {
            uint16 j = s_connection_setting_info.setting_detail[dual_sys][i].list_index;

            if (j < res_info.res_num)
            {
                s_connection_setting_info.setting_detail[dual_sys][i].name_len = MIN(res_info.res_item_ptr[j].name_len, MMICONNECTION_MAX_LINKMODENAME_LEN);
                MMIAPICOM_Wstrncpy(s_connection_setting_info.setting_detail[dual_sys][i].name,res_info.res_item_ptr[j].name,s_connection_setting_info.setting_detail[dual_sys][i].name_len);
            }
        }
#ifdef IPVERSION_SUPPORT_V4_V6
        if(MMICONNECTION_IsPolandSimCard(dual_sys))
        {
            s_connection_setting_info.setting_detail[dual_sys][i].ip_type = MMICONNECTION_IP_V6;
        }else
            s_connection_setting_info.setting_detail[dual_sys][i].ip_type = MMICONN_GetIP_PortocolType();
#endif
    }
    s_connection_setting_info.invisible_apn_num[dual_sys] = visible_num;

#ifdef DERECT_LINK_FIRST_SUPPORT
    MMICONNECTION_SetDerectLinkToFirstIndex(&s_connection_setting_info);
#endif
    MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);

    UnloadNetworkAccountInfo(&res_info);
#endif
}
/*****************************************************************************/
// 	Description : open connection menu
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_OpenMenuWin(void)
{
    MMI_WIN_ID_T    win_id = MMICONNECTION_MENU_WIN_ID;
    MMI_CTRL_ID_T   ctrl_id = MMICONNECTION_MENU_CTRL_ID;
#ifdef MMI_TVOUT_ENABLE
    MMI_TEXT_ID_T   label = TXT_NULL;
#endif
    GUI_BOTH_RECT_T both_rect = {0};
    BOOLEAN is_grayed = FALSE;
    MMI_STRING_T    title_str = {0};
    uint32  i = 0;
    MMI_TEXT_ID_T text_id = TXT_NULL;

    MMK_CloseWin(win_id);

#if defined MMI_GPRS_SUPPORT
     MMK_CreateWin((uint32 *)MMICONNECTION_MAIN_MENU_WIN_TAB,PNULL);
#else
     MMIAPISET_OpenSetNetWorkWin();
#endif

    both_rect = MMITHEME_GetWinClientBothRect(win_id);

    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect, win_id, ctrl_id, GUIMENU_STYLE_THIRD);
    MMI_GetLabelTextByLang(TXT_SETTINGS_LINK, &title_str);
    GUIMENU_SetMenuTitle(&title_str, ctrl_id);

#ifdef BLUETOOTH_SUPPORT
    if(MMIMAINMENU_ISTYLE_E != MMIAPISET_GetMainmenuStyle()
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
        && MMIMAINMENU_GO_SLIDE_E != MMIAPISET_GetMainmenuStyle()
#endif
#ifdef MENU_SWEETS_SUPPORT
        && MMIMAINMENU_SWEETS_E  != MMIAPISET_GetMainmenuStyle()
#endif
        )
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_BLUETOOTH,//text_ptr
            ID_CONNECTION_BLUETOOTH,//node_id
            0, //parent_node_id
            (uint16)i++,//index
#ifndef MMI_RES_LOW_COST
            IMAGE_SECMENU_ICON_LINKSET_BT,
#else
	     IMAGE_NULL,
#endif
            is_grayed
            );
    }
#endif

#ifdef MMI_TVOUT_ENABLE
    if(TRUE == MMITHEME_IsTVOpen())
    {
        label = TXT_CONNECTION_EXIT_TVOUT;
    }
    else
    {
        label = TXT_CONNECTION_OPEN_TVOUT;
    }

    InsertNodeByLabel(
            ctrl_id,//ctrl_id
            label,//text_id
            ID_CONNECTION_OPEN_TV_OUT,//node_id
            0, //parent_node_id
            (uint16)i++,//index
            IMAGE_SECMENU_ICON_LINKSET_TV,
            is_grayed
            );

#endif

#ifdef MMI_GPRS_SUPPORT
#ifdef BROWSER_SUPPORT
    if(MMIBROWSER_TYPE_MAX != MMIBM_GetDefaultBrowser())
#endif
    {
        InsertNodeByLabel(
                ctrl_id,//ctrl_id
                TXT_COMMON_WWW_ACCOUNT,//text_ptr
                ID_CONNECTION_NET_USERID,//node_id
                0, //parent_node_id
                (uint16)i++,//index
#ifndef MMI_RES_LOW_COST
                IMAGE_SECMENU_ICON_LINKSET_NET,
#else
	         IMAGE_NULL,
#endif
                is_grayed
                );
    }
#ifdef MMI_GPRS_SWITCH_SUPPORT

    InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_GPRS_SERVICE,//text_ptr
            ID_CONNECTION_GRPS_SWITCH,//node_id
            0, //parent_node_id
            (uint16)i++,//index
#ifndef MMI_RES_LOW_COST
            IMAGE_SECMENU_ICON_LINKSET_NET,
#else
	     IMAGE_NULL,
#endif
            FALSE
            );


#endif

#ifdef MMI_GPRS_SWITCH_SUPPORT
    if ((MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_OFF)||(MMIAPISET_GetMultiSysSettingNum(PNULL, 0) > 1))
#else
    if (MMIAPISET_GetMultiSysSettingNum(PNULL, 0) > 1)
#endif
    {
        is_grayed = TRUE;
    }
    else
    {
        is_grayed = FALSE;
    }

    s_connection_index = i;

    InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_DATA_CONNECTION_SET,//text_ptr
            ID_CONNECTION_GPRS,//node_id
            0, //parent_node_id
            (uint16)i++,//index
#ifndef MMI_RES_LOW_COST
            IMAGE_SECMENU_ICON_CONNECTION_GPRS,
#else
  	     IMAGE_NULL,
#endif
            is_grayed
            );
    //////////////////////////////////////////////////////////////////////////
    // added by feng.xiao
#ifdef  DATA_ROAMING_SUPPORT
        InsertNodeByLabel(ctrl_id,//ctrl_id
                          TXT_CONNECTION_DATA_ROAMING,//text_ptr
                          ID_CONNECTION_DATA_ROAMING,//node_id
                          0, //parent_node_id
                          (uint16)i++,//index
#ifndef MMI_RES_LOW_COST
                          IMAGE_SECMENU_ICON_CONNECTION_GPRS,
#else
			     IMAGE_NULL,
#endif
                          FALSE);
    #endif
#endif

/* BEGIN: Added by George.Liu 01526, 2010/7/23   PN:MergeToMw1029forCustom */
#ifdef WIFI_SUPPORT
        if (MMIMAINMENU_ISTYLE_E != MMIAPISET_GetMainmenuStyle()
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
            && MMIMAINMENU_GO_SLIDE_E != MMIAPISET_GetMainmenuStyle()
#endif
#ifdef MENU_SWEETS_SUPPORT
            && MMIMAINMENU_SWEETS_E  != MMIAPISET_GetMainmenuStyle()
#endif
        )
    {
        InsertNodeByLabel(
                ctrl_id,//ctrl_id
                TXT_WIFI,
                ID_CONNECTION_WIFI,//node_id
                0, //parent_node_id
                (uint16)i++,//index
                IMAGE_SECMENU_ICON_LINKSET_WIFI,
                FALSE
                );
    }
#endif

#ifdef MMI_PREFER_PLMN_SUPPORT
    text_id = TXT_NETWORK_SETTING;
#else
    text_id = TXT_NETWORK_SELECT;
#endif

#ifdef MMI_DUALMODE_ENABLE
 	InsertNodeByLabel(
            ctrl_id,//ctrl_id
            text_id,//text_ptr
            ID_CONNECTION_SET_NETSETTING,//node_id
            0, //parent_node_id
            (uint16)i++,//index
            IMAGE_SECMENU_ICON_CALLSET_NETSET,
            FALSE
            );
#else
    InsertNodeByLabel(
            ctrl_id,//ctrl_id
            text_id,//text_ptr
            ID_CONNECTION_SET_NETWORK,//node_id
            0, //parent_node_id
            (uint16)i++,//index
#if 0 //  ndef MMI_RES_LOW_COST   zhigang.peng delete 2018-10-03 for pass compiling
            IMAGE_SECMENU_ICON_CALLSET_NETSEL,
#else
	     IMAGE_NULL,
#endif
            FALSE
            );
#endif

    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        text_id,//text_ptr 
        ID_CONNECTION_SET_NETWORK_TYPE,//node_id
        0, //parent_node_id
        (uint16)i++,//index
        IMAGE_NULL,
        FALSE
        );
}

/*****************************************************************************/
// 	Description : close connection menu
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_CloseMenuWin(void)
{
	MMK_CloseWin(MMICONNECTION_MENU_WIN_ID);
}

#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : open setting list window of connection
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_OpenSettingListWin(void)
{
    uint16 sim_num = 0;
    uint16 sim_ok = 0;

    sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

    //SCI_TRACE_LOW:"[MMICONNECTION]:MMIAPICONNECTION_OpenSettingListWin sim_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_5398_112_2_18_2_12_16_10,(uint8*)"d", sim_ok);

    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (1 == sim_num)
    {
        s_connection_handling_sim_id = (MN_DUAL_SYS_E)sim_ok;

        MMK_CreateWin((uint32 *)MMICONNECTION_SETTINGLIST_WIN_TAB, PNULL);
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else
    {
        //MMK_CreateWin((uint32 *)MMICONNECTION_SELECT_SIM_WIN_TAB, PNULL);
        MMICONNECTION_SelectSIMFunc();
    }
#endif
}
#endif

/*****************************************************************************/
// 	Description : open setting list window of connection
//	Global resource dependence : none
//  Author: Ying.Xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_OpenGprsWin(void)
{
    //SCI_TRACE_LOW("[MMICONNECTION]:MMIAPICONNECTION_OpenGprsWin enter");
    //�������б���
#ifdef MMI_GPRS_SUPPORT
    MMK_CreatePubListWin((uint32*)MMICONNECTION_GPRS_SETTINGS_WIN_TAB, PNULL);
#endif
}

#ifdef MMI_GPRS_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify Connection
//    Global resource dependence :
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyConnectionFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    //SCI_TRACE_LOW:"HandleSIMPlugNotifyConnectionFunc dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_5440_112_2_18_2_12_16_11,(uint8*)"dd", dual_sys, notify_event);

    switch (notify_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:
    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
        if (dual_sys == s_connection_handling_sim_id)
        {
            MMK_CloseWin(MMICONNECTION_SETTINGLIST_WIN_ID);
        }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MMK_CloseWin(MMICONNECTION_COM_SELECT_SIM_WIN_ID);
#endif
        break;

    default:
        break;
    }
}
#endif
#endif
/*****************************************************************************/
// 	Description : if sim change,recheck the mcc and mnc to set the ip type
//	Global resource dependence : none
//  Author:dong.chunguang
//	Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_UpdateIpType(uint32 dual_sys, BOOLEAN isSameSim)
{
#ifdef MMI_GPRS_SUPPORT
    uint16 i = 0;

    if(!isSameSim)
    {
        for(i = 0; i < s_connection_setting_info.settinglink_num[dual_sys]; i++)
        {
#ifdef IPVERSION_SUPPORT_V4_V6
            s_connection_setting_info.setting_detail[dual_sys][i].ip_type = MMICONNECTION_IP_V6;
#endif
        }
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);
    }
#endif
}
/*****************************************************************************/
// 	Description : init connection
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_Init(void)
{
#ifdef MMI_GPRS_SUPPORT
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
#ifdef SIM_PLUG_IN_SUPPORT
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};
#endif

    MMINV_READ(MMINV_CONNECTION_SETTING_INFO,&s_connection_setting_info,return_value);

    if(MN_RETURN_SUCCESS != return_value)
    {
        //SCI_TRACE_LOW:"[MMICONNECTION]:MMICONNECTION_Init setting read Failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_5476_112_2_18_2_12_16_12,(uint8*)"");

        MMICONNECTION_ResetSettingList(TRUE);//��̶�����
    }
#ifdef MMI_GPRS_SWITCH_SUPPORT
    if (MMICONNECTION_SETTING_GPRS_SWITCH_OFF==MMIAPICONNECTION_GetGPRSSwitchStatus())
    {
        s_gprs_turned_off = TRUE;
    }
    else
    {
        s_gprs_turned_off = FALSE;
    }
#endif
#ifdef SIM_PLUG_IN_SUPPORT
    sim_plug_notify_info.module_id = MMI_MODULE_CONNECTION;
    sim_plug_notify_info.notify_func = HandleSIMPlugNotifyConnectionFunc;
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
#endif

#if defined(DATA_ACCOUNT_IN_TCARD_SUPPORT) && defined(BROWSER_SUPPORT)

    MMICONNECTION_RegBrowserCB();
#endif
    MMICONNECTION_InitVodafoneSIMcardSettings();
#endif
}

/*****************************************************************************/
// 	Description : Get link setting item by index
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMICONNECTION_LINKSETTING_DETAIL_T* MMIAPICONNECTION_GetLinkSettingItemByIndex(MN_DUAL_SYS_E dual_sys, uint8 index)
{
	SCI_TRACE_LOW("wys sim=%d,index=%d",dual_sys,index);
#ifdef MMI_GPRS_SUPPORT
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return PNULL;
    }

    if (index >= s_connection_setting_info.settinglink_num[dual_sys])
    {
        index= 0;
    }

    return &s_connection_setting_info.setting_detail[dual_sys][index];
#else
    return PNULL;
#endif
}
PUBLIC MMICONNECTION_APNTYPEINDEX_T *MMIAPICONNECTION_GetApnTypeIndex(MN_DUAL_SYS_E dual_sys)
{
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
    
    MMIAPICONNECTION_AdaptingAccount(dual_sys, MMICONNECTION_ACCOUNT_MAX);
    
    return &s_apntype_index;
}
PUBLIC MMICONNECTION_PNNAPN_EXIST_T *MMIAPICONNECTION_GetPNNAPNExist(MN_DUAL_SYS_E dual_sys)
{    
    return &s_is_pnn_apn_exist[dual_sys];
}

PUBLIC void MMIAPICONNECTION_SetPNNAPNExist(MN_DUAL_SYS_E dual_sys, BOOLEAN bIsPNNAPNExist, BOOLEAN is_sim_ready_status)
{    
    s_is_pnn_apn_exist[dual_sys].bIsPNNAPNExist = bIsPNNAPNExist;
    s_is_pnn_apn_exist[dual_sys].is_sim_ready_status = is_sim_ready_status;
    return;
}

/*****************************************************************************/
// 	Description : Get link setting num
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICONNECTION_GetLinkSettingNum(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_GPRS_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        return s_connection_setting_info.settinglink_num[dual_sys];
    }

    return 0;
#else
    return 0;
#endif
}


/*****************************************************************************/
// 	Description : set homepage by index
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_SetHomepage(
                                        MN_DUAL_SYS_E dual_sys,
                                         uint8 index,
                                         const MMI_STRING_T *string_ptr
                                         )
{
    BOOLEAN return_val = FALSE;
#ifdef MMI_GPRS_SUPPORT
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_item_ptr = PNULL;

    if (PNULL == string_ptr)
    {
        return return_val;
    }

    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);
    if(setting_item_ptr
        &&!setting_item_ptr->is_customed_detail
        && string_ptr->wstr_len > 0
        && string_ptr->wstr_len <= MMICONNECTION_HOMEPAGE_MAX_URL_LEN)
    {
        MMIAPICOM_WstrToStr(string_ptr->wstr_ptr, (uint8 *)setting_item_ptr->homepage);
        setting_item_ptr->homepage_len = string_ptr->wstr_len;
        setting_item_ptr->is_default_unchange = FALSE;
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);
        return_val = TRUE;
    }
#endif
    return return_val;
}

/*****************************************************************************/
// 	Description : ����һ��LIST
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_AddNewItem(
                                        MN_DUAL_SYS_E dual_sys,
                                        const MMICONNECTION_LINKSETTING_DETAIL_T *setting_item_ptr
                                        )
{
    BOOLEAN return_val = FALSE;
#ifdef MMI_GPRS_SUPPORT
    uint16 list_index = 0;

    SCI_TRACE_LOW("MMIAPICONNECTION_AddNewItem dual_sys = %d, settinglink_num =%d", dual_sys, s_connection_setting_info.settinglink_num[dual_sys]);
    if(PNULL != setting_item_ptr
        && dual_sys < MMI_DUAL_SYS_MAX
        && s_connection_setting_info.settinglink_num[dual_sys] < MMICONNECTION_MAX_SETTING_NUM)
    {
        list_index = s_connection_setting_info.settinglink_num[dual_sys];
        SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][list_index],setting_item_ptr,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
        s_connection_setting_info.settinglink_num[dual_sys]++;
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);
        return_val = TRUE;
    }
#endif
    return return_val;
}

/*****************************************************************************/
// 	Description : ���ԭ��һ��LIST������
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_ReplaceItem(
                                         MN_DUAL_SYS_E dual_sys,
                                         uint16 list_index,
                                         const MMICONNECTION_LINKSETTING_DETAIL_T *setting_item_ptr
                                         )
{
    BOOLEAN return_val = FALSE;

#ifdef MMI_GPRS_SUPPORT
    if (PNULL == setting_item_ptr || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return return_val;
    }

    if(list_index < s_connection_setting_info.settinglink_num[dual_sys])
    {
        SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][list_index],setting_item_ptr,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);
        return_val = TRUE;
    }

#endif
    return return_val;
}

/*****************************************************************************/
// 	Description : ע��
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_RegisterNotifyInfo(MMICONNECTION_NOTIFY_INFO_T *notify_info_ptr)
{
    BOOLEAN return_val = FALSE;
    MMICONNECTION_NOTIFY_LIST_T *cursor = PNULL;

    if(PNULL != notify_info_ptr)
    {
        if(PNULL == s_subscriber_list_ptr)
        {
            s_subscriber_list_ptr = SCI_ALLOC_APP(sizeof(MMICONNECTION_NOTIFY_LIST_T));
            SCI_MEMSET(s_subscriber_list_ptr,0,sizeof(MMICONNECTION_NOTIFY_LIST_T));
            SCI_MEMCPY(&(s_subscriber_list_ptr->notify_info),notify_info_ptr,sizeof(MMICONNECTION_NOTIFY_INFO_T));
        }
        else
        {
            cursor = s_subscriber_list_ptr;
            while(PNULL != cursor->next)
            {
                if(cursor->notify_info.module_id == notify_info_ptr->module_id)
                {
                    return FALSE;   //�Ѿ�ע�����
                }
                cursor = cursor->next;
            }
            if(cursor->notify_info.module_id == notify_info_ptr->module_id)
            {
                return FALSE;   //�Ѿ�ע�����
            }
            cursor->next = SCI_ALLOC_APP(sizeof(MMICONNECTION_NOTIFY_LIST_T));
            SCI_MEMSET(cursor->next,0,sizeof(MMICONNECTION_NOTIFY_LIST_T));
            SCI_MEMCPY(&(cursor->next->notify_info),notify_info_ptr,sizeof(MMICONNECTION_NOTIFY_INFO_T));
        }
        return_val = TRUE;
    }

    return return_val;
}

/*****************************************************************************/
// 	Description : ����
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_UnregisterNotifyInfo(uint32 module_id)
{
    MMICONNECTION_NOTIFY_LIST_T *cursor = PNULL;
    MMICONNECTION_NOTIFY_LIST_T *cursor_free = PNULL;

    cursor = s_subscriber_list_ptr;
    if(PNULL != cursor)
    {
        if(cursor->notify_info.module_id == module_id)
        {
            s_subscriber_list_ptr = cursor->next;
            SCI_FREE(cursor);
        }
        else
        {
            while(PNULL != cursor->next)
            {
                if(cursor->next->notify_info.module_id == module_id)
                {
                    cursor_free = cursor->next;
                    cursor->next = cursor_free->next;
                    SCI_FREE(cursor_free);
                    break;
                }
                cursor = cursor->next;
            }
        }
    }

}

//#ifdef MRAPP_SUPPORT
/*****************************************************************************/
//  Description : get s_connection_setting_info
//  Global resource dependence : none
//  Author:
//  Note:for SKY
/*****************************************************************************/
PUBLIC MMICONNECTION_SETTING_T * MMIMRAPP_GetConnetInfo(void)
{
#ifdef MMI_GPRS_SUPPORT
	return &s_connection_setting_info;
#else
       return PNULL;
#endif
}
//#endif

#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
//  Description : Is Same Network Plmn
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:cmp mcc and mnc
/*****************************************************************************/
LOCAL BOOLEAN IsSameNetworkPlmn(MN_PLMN_T plmn1, MN_PLMN_T plmn2)
{
	if ((plmn1.mcc == plmn2.mcc)
		&&(plmn1.mnc == plmn2.mnc))
	{
		return TRUE;
	}
	return FALSE;
}

/*****************************************************************************/
//  Description : adjust whether any other items exist with same mcc&mnc
//  Global resource dependence : none
//  Author: bean.yu
//  Note:cmp mcc and mnc
/*****************************************************************************/
LOCAL BOOLEAN IsSamePLMNItemExistStill( MMICONNECTION_RES_INFO_T *conn_res, uint32 index )
{
	SCI_TRACE_LOW("IsSamePLMNItemExistStill enter, index = %d, cur_mnc=%d, cur_mnc:%d", index, conn_res->res_item_ptr[index].mcc, conn_res->res_item_ptr[index].mnc);

	if ((index > 0 && index < (conn_res->res_num - 1)) &&  
	    (conn_res->res_item_ptr[index].mcc != conn_res->res_item_ptr[index+1].mcc \
		 || conn_res->res_item_ptr[index].mnc != conn_res->res_item_ptr[index+1].mnc))
	{
		return FALSE;
	}
	else if ((index == conn_res->res_num - 1) && 
	         (conn_res->res_item_ptr[index].mcc == conn_res->res_item_ptr[index-1].mcc) && 
		     (conn_res->res_item_ptr[index].mnc == conn_res->res_item_ptr[index-1].mnc))
	{
	    return FALSE;
	}
	
	return TRUE;
}

/*****************************************************************************/
//  Description : adjust whether any other items exist with same mcc&mnc
//  Global resource dependence : none
//  Author: bean.yu
//  Note:cmp mcc and mnc
/*****************************************************************************/
LOCAL BOOLEAN IsValidNetworkAccount(MN_DUAL_SYS_E dual_sys, MMICONNECTION_RES_ITEM_T *conn_res, uint32 *mvnotype_appointed_type)
{
    BOOLEAN isValid = FALSE;
    MMI_STRING_T cur_mvno_string = {0};
    wchar cur_mvno_value[MMICONNECTION_HOMEPAGE_MAX_URL_LEN + 1] = {0};
    
    if ((NULL != conn_res) && (NULL != mvnotype_appointed_type))
    {
        cur_mvno_string.wstr_len = conn_res->mvno_value_len;
        MMIAPICOM_StrToWstr(conn_res->mvno_value, cur_mvno_value);
        cur_mvno_string.wstr_ptr = (wchar *)cur_mvno_value;
        
        switch (conn_res->mvno_type)
        {
            case MMICONNECTION_MVNOTYPE_SPN:
                isValid = MMIPHONE_CheckSPNString(dual_sys, &cur_mvno_string);
                if ((TRUE == isValid) && (MMICONNECTION_MVNOTYPE_MAX == *mvnotype_appointed_type))
                {
                    *mvnotype_appointed_type = MMICONNECTION_MVNOTYPE_SPN;
                }            
                SCI_TRACE_LOW("IsValidNetworkAccount leave, isValid=%d", isValid);
                break;
                
            case MMICONNECTION_MVNOTYPE_GID:
                isValid = MMIPHONE_CheckGIDString(dual_sys, &cur_mvno_string);
                if ((TRUE == isValid) && (MMICONNECTION_MVNOTYPE_MAX == *mvnotype_appointed_type))
                {
                    *mvnotype_appointed_type = MMICONNECTION_MVNOTYPE_GID;
                }            
                break;
                
            case MMICONNECTION_MVNOTYPE_PNN:
                if (s_is_pnn_apn_exist[dual_sys].is_sim_ready_status)
                {
                    s_is_pnn_apn_exist[dual_sys].bIsPNNAPNExist = TRUE;
                }
                else
                {
                    isValid = MMIPHONE_CheckPNNString(dual_sys, &cur_mvno_string);
                    if ((TRUE == isValid) && (MMICONNECTION_MVNOTYPE_MAX == *mvnotype_appointed_type))
                    {
                        *mvnotype_appointed_type = MMICONNECTION_MVNOTYPE_PNN;
                    }            
                }
                break;   
                
            case MMICONNECTION_MVNOTYPE_IMSI:
                isValid = MMIPHONE_CheckIMSIString(dual_sys, &cur_mvno_string);
                if ((TRUE == isValid) && (MMICONNECTION_MVNOTYPE_MAX == *mvnotype_appointed_type))
                {
                    *mvnotype_appointed_type = MMICONNECTION_MVNOTYPE_IMSI;
                }            
                break;
                
            case MMICONNECTION_MVNOTYPE_IMSIR:
                isValid = MMIPHONE_CheckIMSIRString(dual_sys, &cur_mvno_string);
                if ((TRUE == isValid) && (MMICONNECTION_MVNOTYPE_MAX == *mvnotype_appointed_type))
                {
                    *mvnotype_appointed_type = MMICONNECTION_MVNOTYPE_IMSIR;
                }            
                break;
                
            default:
                isValid = FALSE;
                break;
        }

    	SCI_TRACE_LOW("IsValidNetworkAccount enter, cur_mmc=%d, cur_mnc:%d, mvno_type=%d, mvno_value=%s",
    				  conn_res->mcc, conn_res->mnc, conn_res->mvno_type, conn_res->mvno_value);
    }  
    
	return isValid;
}

/*****************************************************************************/
//  Description : set default for esp account
//  Global resource dependence : none
//  Author: bean.yu
//  Note: index is lastest group items whatever empty or not.
/*****************************************************************************/
LOCAL uint32 GetDefaultNetworkAccountNum( MMICONNECTION_RES_ITEM_T* res_item, uint32 index, 
	BOOLEAN sim_spn_empty, BOOLEAN res_spn_empty ){

	uint32 num = 1;    //default : itself
	uint32 i;
	SCI_TRACE_LOW("GetDefaultNetworkAccountNum enter, spn_empty=%d, mcc=%d, mnc=%d", sim_spn_empty, res_item[index].mcc, res_item[index].mnc);

	if( sim_spn_empty )
	{  
	    //find the lastest group.
		for(i = 0; index > i ;i++)
		{
			if( MMIAPICOM_StrCmp( res_item[index-i].mvno_value, res_item[index-i].mvno_value_len, res_item[index-i-1].mvno_value, res_item[index-i-1].mvno_value_len ) != 0 
				|| res_item[index-i].mcc != res_item[index-i-1].mcc || res_item[index-i].mnc != res_item[index-i-1].mnc )
		    {
				break;
			}
			num++;
		}
	}
	else
	{  
		if( !res_spn_empty )
		{    // no empty item exists, find the lastest group.
			for(i = 0; index > i ;i++)
			{
				if( MMIAPICOM_StrCmp( res_item[index-i].mvno_value, res_item[index-i].mvno_value_len, res_item[index-i-1].mvno_value, res_item[index-i-1].mvno_value_len ) != 0 
					|| res_item[index-i].mcc != res_item[index-i-1].mcc || res_item[index-i].mnc != res_item[index-i-1].mnc )
				{
					break;
				}
				num++;
			}
		}
		else
		{ //find the lastest empty group.
			for(i = 0; index > i ;i++)
			{
				if( res_item[index-i-1].mvno_value_len != 0  || res_item[index-i].mcc != res_item[index-i-1].mcc || res_item[index-i].mnc != res_item[index-i-1].mnc )
				{
					break;
				}
				num++;
			}
		}
	}

	return num ;
}

/*****************************************************************************/
//  Description : Is Same Network Plmn
//  Global resource dependence : none
//  Author: wancan.you
//  Note:cmp mcc and network name
/*****************************************************************************/
LOCAL BOOLEAN IsSameNetworkName(PHONE_PLMN_NAME_E plmn1, PHONE_PLMN_NAME_E plmn2)
{
    const MMI_NETWORK_NAME_T *plmn1_network_info_ptr = PNULL;
    const MMI_NETWORK_NAME_T *plmn2_network_info_ptr = PNULL;
    BOOLEAN is_plmn1_india_mcc = FALSE;
    BOOLEAN is_plmn2_india_mcc = FALSE;
    BOOLEAN is_plmn_name_bsnl_cellone = FALSE;

    plmn1_network_info_ptr = MMIAPIPHONE_GetNetWorkInfo(plmn1);
    plmn2_network_info_ptr = MMIAPIPHONE_GetNetWorkInfo(plmn2);

    if (PNULL != plmn1_network_info_ptr && PNULL != plmn2_network_info_ptr)
    {
        if (404 == plmn1_network_info_ptr->mcc
            || 405 == plmn1_network_info_ptr->mcc)
        {
            is_plmn1_india_mcc = TRUE;
        }

        if (404 == plmn2_network_info_ptr->mcc
            || 405 == plmn2_network_info_ptr->mcc)
        {
            is_plmn2_india_mcc = TRUE;
        }

        //Cellone and BSNL use the same network account
        if((plmn1_network_info_ptr->name_index == TXT_NET_CELLONE && plmn2_network_info_ptr->name_index == TXT_NET_BSNL)
            ||(plmn1_network_info_ptr->name_index == TXT_NET_BSNL && plmn2_network_info_ptr->name_index == TXT_NET_CELLONE))
        {
            is_plmn_name_bsnl_cellone = TRUE;
        }

        if (((plmn1_network_info_ptr->mcc == plmn2_network_info_ptr->mcc) || (is_plmn1_india_mcc && is_plmn2_india_mcc))
            && (plmn1_network_info_ptr->name_index == plmn2_network_info_ptr->name_index
            ||is_plmn_name_bsnl_cellone))
        {
            return TRUE;
        }
    }

    return FALSE;
}

#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
LOCAL uint8  MMICONN_GetVodafoneSimConfig(void)
{
     uint8     vodafone_config  =  0;
     uint16    nv_len = 0;
     nv_len = NVITEM_GetLength(TEL_NV_OPERATOR_PAY_SET);
     EFS_NvitemRead(TEL_NV_OPERATOR_PAY_SET, nv_len, &vodafone_config);
     SCI_TRACE_LOW("[conn] MMICONN_GetVodafoneSimConfig vodafone_config=%d", vodafone_config);
     return vodafone_config;
}

LOCAL void MMICONNECTION_SaveNetworkAccount(MN_DUAL_SYS_E dual_sys, MN_PLMN_T *sim_hplmn, MMICONNECTION_RES_INFO_T *res_info)
{
    uint32 i = 0;
    BOOLEAN is_firstin = TRUE;
    int32 first_item_index = -1;
	
	SCI_TRACE_LOW("[szp]AutoAdapting sim.mcc=%d, sim.mnc=%d, dual_sys = %d", sim_hplmn->mcc, sim_hplmn->mnc, dual_sys);
    if ((PNULL != res_info) && (PNULL != res_info->res_item_ptr) && 0 < res_info->res_num)
    {
        for (i = 0; i < res_info->res_num; i++)
        {
            MN_PLMN_T plmn = {0};
        
            if (s_connection_setting_info.settinglink_num[dual_sys] >= MMICONNECTION_MAX_SETTING_NUM)
            {
                break;
            }
            plmn.mcc = res_info->res_item_ptr[i].mcc;
            plmn.mnc = res_info->res_item_ptr[i].mnc;
            if ((sim_hplmn->mcc == plmn.mcc) && (sim_hplmn->mnc == plmn.mnc))
            {
                if (TRUE == is_firstin)
                {
        	        SCI_TRACE_LOW("[szp]AutoAdapting plmn.mcc=%d, plmn.mnc=%d, dual_sys = %d", sim_hplmn->mcc, sim_hplmn->mnc, dual_sys);
                    first_item_index = i;
                    is_firstin = FALSE;
                }
                
                if (!IsSamePLMNItemExistStill(res_info, i) && (0 == s_connection_setting_info.settinglink_num[dual_sys]))
                {
                    SCI_TRACE_LOW("[szp]MMICONNECTION_SaveNetworkAccount first_item_index=%d, i=%d", first_item_index, i);
                    SetDefaultNetworkAccount(res_info, dual_sys, i, first_item_index);  
                }
            }                        
        }
    }

    return;
}

LOCAL void MMICONNECTION_MatchByAccountType(MN_DUAL_SYS_E dual_sys, MN_PLMN_T *sim_hplmn, MMICONNECTION_ACCOUNT_TYPE_E account_type)
{
    uint32 i = 0;
    uint8 apn_order[MMICONNECTION_ACCOUNT_MAX] = {0};
    BOOLEAN is_account_appointed[MMICONNECTION_ACCOUNT_MAX] = {0};
    MN_RETURN_RESULT_E nv_result = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_APN_TYPE_INDEX, &s_apntype_index, nv_result);
    if (MN_RETURN_SUCCESS != nv_result)
    {
        SCI_MEMSET(&s_apntype_index, 0, sizeof(MMICONNECTION_APNTYPEINDEX_T));
    }
    
    if (MMICONNECTION_ACCOUNT_MAX == account_type)
    {
        SCI_MEMSET(&s_apntype_index.internet_index[dual_sys], 0, sizeof(MMICONNECTION_TYPEINDEX_T));
        SCI_MEMSET(&s_apntype_index.xcap_index[dual_sys], 0, sizeof(MMICONNECTION_TYPEINDEX_T));
        SCI_MEMSET(&s_apntype_index.ims_index[dual_sys], 0, sizeof(MMICONNECTION_TYPEINDEX_T));
        SCI_MEMSET(&s_apntype_index.ia_index[dual_sys], 0, sizeof(MMICONNECTION_TYPEINDEX_T));
    }
    else if (MMICONNECTION_ACCOUNT_BROWSER == account_type)
    {
        s_apntype_index.internet_index[dual_sys].is_valid = FALSE;
    }
    else if (MMICONNECTION_ACCOUNT_IMS == account_type)
    {
        s_apntype_index.ims_index[dual_sys].is_valid = FALSE;
    }
    else if (MMICONNECTION_ACCOUNT_IA == account_type)
    {
        s_apntype_index.ia_index[dual_sys].is_valid = FALSE;
    }
    else if (MMICONNECTION_ACCOUNT_XCAP == account_type)
    {
        s_apntype_index.xcap_index[dual_sys].is_valid = FALSE;
    }
    
    for (i = 0; i < s_connection_setting_info.settinglink_num[dual_sys]; i++)
    {
        SCI_TRACE_LOW("AutoAdapting account_type:%d", s_connection_setting_info.setting_detail[dual_sys][i].account_type);
        if ((sim_hplmn->mcc == s_connection_setting_info.setting_detail[dual_sys][i].plmn.mcc) &&
            (sim_hplmn->mnc == s_connection_setting_info.setting_detail[dual_sys][i].plmn.mnc))
        {
            if (MMICONNECTION_ACCOUNT_MAX == account_type || 
                (s_connection_setting_info.setting_detail[dual_sys][i].account_type == account_type))
            {
                switch (s_connection_setting_info.setting_detail[dual_sys][i].account_type)
                {
                    case MMICONNECTION_ACCOUNT_BROWSER:
                        if ((TRUE == s_connection_setting_info.setting_detail[dual_sys][i].visible) && ((!is_account_appointed[MMICONNECTION_ACCOUNT_BROWSER]) 
                            || (apn_order[MMICONNECTION_ACCOUNT_BROWSER] > s_connection_setting_info.setting_detail[dual_sys][i].uiorder)))
                        {
                            apn_order[MMICONNECTION_ACCOUNT_BROWSER] = s_connection_setting_info.setting_detail[dual_sys][i].uiorder;
                            is_account_appointed[MMICONNECTION_ACCOUNT_BROWSER]= TRUE;
                        #ifdef BROWSER_SUPPORT
                            MMIAPIBROWSER_SetNetSettingIndex(dual_sys, i);
                        #endif
                        #ifdef MMI_STREAMING_PLAYER_SUPPORT
                            MMIAPIVP_ResetSettingInfo();
                        #endif
                            s_apntype_index.internet_index[dual_sys].index = i;
                            s_apntype_index.internet_index[dual_sys].is_valid = TRUE;
                        }
                        break;

                    case MMICONNECTION_ACCOUNT_MMS:
                        if ((!is_account_appointed[MMICONNECTION_ACCOUNT_MMS])
                            || (apn_order[MMICONNECTION_ACCOUNT_MMS] > s_connection_setting_info.setting_detail[dual_sys][i].uiorder))
                        {
                            apn_order[MMICONNECTION_ACCOUNT_MMS] = s_connection_setting_info.setting_detail[dual_sys][i].uiorder;
                            is_account_appointed[MMICONNECTION_ACCOUNT_MMS] = TRUE;
                        
                            #ifdef MMS_SUPPORT
                            MMIAPIMMS_Setting_SetNetSettingIndex(dual_sys, i);
                            #endif
                        }
                        break;
                        
                    case MMICONNECTION_ACCOUNT_DCD:
                        if (!is_account_appointed[MMICONNECTION_ACCOUNT_DCD])
                        {
                            is_account_appointed[MMICONNECTION_ACCOUNT_DCD] = TRUE;

                            #ifdef DCD_SUPPORT
                            MMIAPIDCD_Setting_SetNetSettingIndex(dual_sys, i);
                            #endif
                        }
                        break;

                    case MMICONNECTION_ACCOUNT_IMS:
                        if (!is_account_appointed[MMICONNECTION_ACCOUNT_IMS])
                        {
                            is_account_appointed[MMICONNECTION_ACCOUNT_IMS] = TRUE;                                    
                            s_apntype_index.ims_index[dual_sys].index = i;
                            s_apntype_index.ims_index[dual_sys].is_valid = TRUE;
                        }
                        break;
                        
                    case MMICONNECTION_ACCOUNT_IA:
                        if (!is_account_appointed[MMICONNECTION_ACCOUNT_IA])
                        {
                            is_account_appointed[MMICONNECTION_ACCOUNT_IA] = TRUE;
                            s_apntype_index.ia_index[dual_sys].index = i;
                            s_apntype_index.ia_index[dual_sys].is_valid = TRUE;
                        }
                        break;
                        
                    case MMICONNECTION_ACCOUNT_XCAP:
                        if (!is_account_appointed[MMICONNECTION_ACCOUNT_XCAP])
                        {
                            is_account_appointed[MMICONNECTION_ACCOUNT_XCAP] = TRUE;
                            s_apntype_index.xcap_index[dual_sys].index = i;
                            s_apntype_index.xcap_index[dual_sys].is_valid = TRUE;
                        }
                        break;
                    
                    default:
                        break;
                }
            }
        }
    }

    MMINV_WRITE(MMINV_SET_APN_TYPE_INDEX, &s_apntype_index);

    return;
}

/*****************************************************************************/
//  Description : Auto Adapting Network account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void AutoAdapting(MN_DUAL_SYS_E dual_sys, MMICONNECTION_ACCOUNT_TYPE_E account_type, BOOLEAN is_need_clean)
{
    PHONE_PLMN_NAME_E hplmn = PLMN_INVALID;
    PHONE_PLMN_NAME_E conection_plmn_name[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    PLMN_CMP_CALLBACK plmn_cmp_fun = PNULL;
    BOOLEAN is_res_data_exist = FALSE;
    MMICONNECTION_RES_INFO_T res_info = {0};
#ifdef CMCC_UI_STYLE
    BOOLEAN is_need_add_default = TRUE;
#endif      
	MN_PLMN_T sim_hplmn = {0};
	MN_PLMN_T carrier_hplmn[MMI_DUAL_SYS_MAX] = {0};
	
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

	MMIAPIPHONE_GetSimHPLMN(&sim_hplmn, dual_sys);		
    //SCI_TRACE_LOW:"AutoAdapting dual_sys=%d, account_type=%d, is_need_clean=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMICONN_MANAGER_WINTAB_5786_112_2_18_2_12_17_13,(uint8*)"ddd", dual_sys, account_type, is_need_clean);

    if (SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(dual_sys))
    {
		
		MMINV_READ(MMINV_CARRIER_PLMN, carrier_hplmn, return_value);

		if (return_value != MN_RETURN_SUCCESS)
		{
			SCI_MEMSET(carrier_hplmn, 0x00, sizeof(carrier_hplmn));

			MMINV_WRITE(MMINV_CARRIER_PLMN, carrier_hplmn);
		}
		        
		MMIAPIPHONE_GetSimHPLMN(&sim_hplmn, dual_sys);		
		MMIAPIPHONE_SyncSimSPN(dual_sys);
        
		SCI_TRACE_LOW("AutoAdapting sim.mcc=%d, sim.mnc=%d, carrier.mcc=%d, sim.mnc=%d, dual_sys = %d",
			          sim_hplmn.mcc, sim_hplmn.mnc, carrier_hplmn[dual_sys].mcc, carrier_hplmn[dual_sys].mnc, dual_sys);
			          
        if ((carrier_hplmn[dual_sys].mcc != sim_hplmn.mcc) || (carrier_hplmn[dual_sys].mnc != sim_hplmn.mnc) || (!MMIAPIPHONE_IsSameSim(dual_sys)))
        {
            carrier_hplmn[dual_sys] = sim_hplmn;
            MMINV_WRITE(MMINV_CARRIER_PLMN, carrier_hplmn);
            
            
            if (is_need_clean)
            {
                is_res_data_exist = LoadNetworkAccountInfo(dual_sys, &res_info);
                if (!is_res_data_exist || PNULL == res_info.res_item_ptr)
                {
                    UnloadNetworkAccountInfo(&res_info);
                    return;
                }

#ifdef CMCC_UI_STYLE //���ư汾����������˺�
                is_need_add_default = MMICONNECTION_IsNeedAddDefault(dual_sys, carrier_hplmn, &res_info);
#else
                s_connection_setting_info.settinglink_num[dual_sys] = 0;
                SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys], 0x00, sizeof(s_connection_setting_info.setting_detail[dual_sys]));
#endif

#ifdef CMCC_UI_STYLE //���ư汾����������˺�
                if (is_need_add_default)
#endif
                {
                    MMICONNECTION_SaveNetworkAccount(dual_sys, &sim_hplmn, &res_info);
                    
                    #ifdef DERECT_LINK_FIRST_SUPPORT
                    MMICONNECTION_SetDerectLinkToFirstIndex(&s_connection_setting_info);
                    #endif
                    MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);
                    if (0 == s_connection_setting_info.settinglink_num[dual_sys])
                    {
                        MMICONNECTION_ResetSettingListEx(dual_sys, TRUE);
                    }
                }

                UnloadNetworkAccountInfo(&res_info);
            }
            
            MMICONNECTION_MatchByAccountType(dual_sys, &sim_hplmn, account_type);
            
        }
        else if (( sim_hplmn.mcc <= 0 || sim_hplmn.mnc <= 0 ) && (0 == s_connection_setting_info.settinglink_num[dual_sys] ||MN_RETURN_SUCCESS != return_value))
        {
            carrier_hplmn[dual_sys] = sim_hplmn;
            MMINV_WRITE(MMINV_CARRIER_PLMN, carrier_hplmn);
            MMICONNECTION_ResetSettingListEx(dual_sys, TRUE);
        }
        else
        {   
            /*do nothing*/
        }
    }
}
#endif

/*****************************************************************************/
//  Description : Auto Adapting specified account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_AdaptingAccount(MN_DUAL_SYS_E dual_sys, MMICONNECTION_ACCOUNT_TYPE_E account_type)
{
#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
	uint32 i = 0;
	MN_PLMN_T carrier_hplmn[MMI_DUAL_SYS_MAX] = {0};

	if (dual_sys >= MMI_DUAL_SYS_MAX)
	{

		SCI_MEMSET(carrier_hplmn, 0x00, sizeof(carrier_hplmn));

		MMINV_WRITE(MMINV_CARRIER_PLMN, carrier_hplmn);

		for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
		{
			AutoAdapting((MN_DUAL_SYS_E)i, account_type, FALSE);
		}
	}
	else
	{
	    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	    MMINV_READ(MMINV_CARRIER_PLMN, carrier_hplmn, return_value);

		if (return_value != MN_RETURN_SUCCESS)
		{
			SCI_MEMSET(carrier_hplmn, 0x00, sizeof(carrier_hplmn));
		}
		else
		{
			carrier_hplmn[dual_sys].mcc = 0x0;
			carrier_hplmn[dual_sys].mnc = 0x0;
		}

	    MMINV_WRITE(MMINV_CARRIER_PLMN, carrier_hplmn);

	    AutoAdapting(dual_sys, account_type, FALSE);
	}
#endif
}

/*****************************************************************************/
//  Description : set default for esp account
//  Global resource dependence : none
//  Author: bean.yu
//  Note: index is lastest group items whatever empty or not.
/*****************************************************************************/
LOCAL void SetDefaultNetworkAccount(MMICONNECTION_RES_INFO_T *conn_res, MN_DUAL_SYS_E dual_sys, uint32 cur_index, uint32 first_index)
{
	uint32 index = 0;
	uint32 connection_index = 0;
	uint32 none_mvnotype_cnt = 0;
	BOOLEAN is_valid_mvno_apn = FALSE;
    BOOLEAN visible = FALSE;
    uint8 invisible_num = 0;
    
    uint32 mvnotype_appointed_type = MMICONNECTION_MVNOTYPE_MAX;
	SCI_TRACE_LOW("SetDefaultNetworkAccount enter, dual_sys=%d, cur_index=%d, first_index=%d", dual_sys, cur_index, first_index);
	
	for (index = first_index; index <= cur_index; index++)
	{
        if (MMICONNECTION_MVNOTYPE_NONE != conn_res->res_item_ptr[index].mvno_type)
        {
            is_valid_mvno_apn = IsValidNetworkAccount(dual_sys, &conn_res->res_item_ptr[index], &mvnotype_appointed_type);
            SCI_TRACE_LOW("SetDefaultNetworkAccount enter1, is_valid_mvno_apn=%d, mvnotype_appointed_type=%d", is_valid_mvno_apn, mvnotype_appointed_type);
            if ((TRUE == is_valid_mvno_apn) && (conn_res->res_item_ptr[index].mvno_type == mvnotype_appointed_type))
            {
                visible = SaveNetworkAccountInfo(&s_connection_setting_info.setting_detail[dual_sys][connection_index],
          									     &conn_res->res_item_ptr[index], TRUE, index);
				if (FALSE == visible)
				{
                    invisible_num ++;
				}
                MMICONNECTION_SetHighPriList(dual_sys, (uint8)connection_index);
                s_connection_setting_info.settinglink_num[dual_sys] ++;
                connection_index ++;
            }
        }
        else
        {
            none_mvnotype_cnt ++;
        }
	}
	
	if (0 == s_connection_setting_info.settinglink_num[dual_sys])
	{
	    if (0 != none_mvnotype_cnt)
	    {
            for (index = first_index; index <= cur_index; index++)
            {
                if (MMICONNECTION_MVNOTYPE_NONE == conn_res->res_item_ptr[index].mvno_type)
                {                    
                    SCI_TRACE_LOW("SetDefaultNetworkAccount enter2, none_mvnotype_cnt=%d, mvnotype_appointed_type=%d", none_mvnotype_cnt, mvnotype_appointed_type);
                    visible = SaveNetworkAccountInfo(&s_connection_setting_info.setting_detail[dual_sys][connection_index],
                                                     &conn_res->res_item_ptr[index], TRUE, index);
                    if (FALSE == visible)
                    {
                        invisible_num++;
                    }
                    MMICONNECTION_SetHighPriList(dual_sys, (uint8)connection_index);
                    s_connection_setting_info.settinglink_num[dual_sys]++;
                    connection_index ++;
                }
            }            
	    }
	    else
	    {
            for (index = first_index; index <= cur_index; index++)
            {
                if (conn_res->res_item_ptr[first_index].mvno_type == conn_res->res_item_ptr[index].mvno_type)
                {
                    SCI_TRACE_LOW("SetDefaultNetworkAccount enter3, none_mvnotype_cnt=%d, mvnotype_appointed_type=%d", none_mvnotype_cnt, mvnotype_appointed_type);
                    visible = SaveNetworkAccountInfo(&s_connection_setting_info.setting_detail[dual_sys][connection_index],
                                                     &conn_res->res_item_ptr[index], TRUE, index);
                    if (FALSE == visible)
                    {
                        invisible_num ++;
                    }
                    MMICONNECTION_SetHighPriList(dual_sys, (uint8)connection_index);
                    s_connection_setting_info.settinglink_num[dual_sys]++;
                    connection_index ++;
                }
            }
	    }
	}
	
    s_connection_setting_info.invisible_apn_num[dual_sys] = invisible_num;

	return;    
}

/*****************************************************************************/
//  Description : Auto Adapting Network account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_AutoAdapting(MN_DUAL_SYS_E dual_sys)
{
#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
    uint32 i = 0;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            AutoAdapting((MN_DUAL_SYS_E)i, MMICONNECTION_ACCOUNT_MAX, TRUE);
        }
    }
    else
    {
        AutoAdapting(dual_sys, MMICONNECTION_ACCOUNT_MAX, TRUE);
    }
#endif
}
#if 0
/*****************************************************************************/
//  Description : Get account default index
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICONNECTION_GetAccountDefaultIndex(MN_DUAL_SYS_E dual_sys,
                                                                                                        MMICONNECTION_ACCOUNT_TYPE_E account_type)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    uint32 i = 0;

    //SCI_TRACE_LOW:"[MMICONNECTION]:MMICONNECTION_GetAccountDefaultIndex dual_sys=%d,account_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6086_112_2_18_2_12_18_16,(uint8*)"dd", dual_sys, account_type);

    if (dual_sys >= MMI_DUAL_SYS_MAX || account_type >= MMICONNECTION_ACCOUNT_MAX)
    {
        return 0;
    }

    if (0 == s_connection_setting_info.settinglink_num[dual_sys])
    {
        MMINV_READ(MMINV_CONNECTION_SETTING_INFO,&s_connection_setting_info,return_value);

        if (MN_RETURN_SUCCESS != return_value)
        {
            //SCI_TRACE_LOW:"[MMICONNECTION]:MMICONNECTION_GetAccountDefaultIndex setting read Failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6099_112_2_18_2_12_18_17,(uint8*)"");
            MMICONNECTION_ResetSettingList(TRUE);//��̶�����
        }
    }

    for (i = 0; i < s_connection_setting_info.settinglink_num[dual_sys]; i++)
    {
        if (account_type == s_connection_setting_info.setting_detail[dual_sys][i].account_type)
        {
            return i;
        }
    }

    return 0;
}
#else
/*****************************************************************************/
//  Description : Get account default index
//  Global resource dependence : none
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICONNECTION_GetAccountDefaultIndex(MN_DUAL_SYS_E dual_sys,
                                             MMICONNECTION_ACCOUNT_TYPE_E account_type)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    uint32 i = 0;
	uint8 default_apn = 0;

    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6086_112_2_18_2_12_18_16,(uint8*)"dd", dual_sys, account_type);
    if (dual_sys >= MMI_DUAL_SYS_MAX || account_type >= MMICONNECTION_ACCOUNT_MAX)
    {
        return 0;
    }
    if (0 == s_connection_setting_info.settinglink_num[dual_sys])
    {
        MMINV_READ(MMINV_CONNECTION_SETTING_INFO,&s_connection_setting_info,return_value);

        if (MN_RETURN_SUCCESS != return_value)
        {
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6099_112_2_18_2_12_18_17,(uint8*)"");
            MMICONNECTION_ResetSettingList(TRUE);//��̶�����
        }
    }
	if(dual_sys == MN_DUAL_SYS_1)
	{
		if(MMICONNECTION_IsVodafoneVersion())
		{
			uint8 default_id = 0;
			MMINV_READ(MMINV_CONNECTION_VODAFONE1_APN_DEFAULT, &default_apn, return_value);
	        if (MN_RETURN_SUCCESS != return_value)
	        {
				MMINV_WRITE(MMINV_CONNECTION_VODAFONE1_APN_DEFAULT, &default_id);
	        }
		}
		else
		{
			uint8 default_id = 0;
			MMINV_READ(MMINV_CONNECTION_SIM1_APN_DEFAULT, &default_apn, return_value);
	        if (MN_RETURN_SUCCESS != return_value)
	        {
				MMINV_WRITE(MMINV_CONNECTION_SIM1_APN_DEFAULT, &default_id);
	        }
		}
	}
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else{
		if(MMICONNECTION_IsVodafoneVersion())
		{
			uint8 default_id = 0;
			MMINV_READ(MMINV_CONNECTION_VODAFONE2_APN_DEFAULT, &default_apn, return_value);
	        if (MN_RETURN_SUCCESS != return_value)
	        {
				MMINV_WRITE(MMINV_CONNECTION_VODAFONE2_APN_DEFAULT, &default_id);
	        }
		}
		else
		{
			uint8 default_id = 0;
			MMINV_READ(MMINV_CONNECTION_SIM2_APN_DEFAULT, &default_apn, return_value);
	        if (MN_RETURN_SUCCESS != return_value)
	        {
				MMINV_WRITE(MMINV_CONNECTION_SIM2_APN_DEFAULT, &default_id);
	        }
		}

	}
#endif
	if(default_apn > s_connection_setting_info.settinglink_num[dual_sys])
	{
		default_apn = 0;
	}
	return default_apn;
}
#endif
#endif

#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
//  Description :Parse Network Account Info
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:   is_tcard_file: Tcard file OR resource
/*****************************************************************************/
LOCAL BOOLEAN ParseNetworkAccountInfo(MMICONNECTION_RES_INFO_T *res_info_ptr, MMIRES_DATA_INFO_T file_info)
{
    BOOLEAN is_res_data_ok = TRUE;
    uint16 account_num = 0;

    if (PNULL != file_info.data_ptr && NETWORK_ACCOUNT_HEADER_LENGTH < file_info.data_len)
    {
        //is magic header "ntac"
        if(0 != memcmp(file_info.data_ptr,NETWORK_ACCOUNT_MAGIC_FLAG,NETWORK_ACCOUNT_MAGIC_LENGTH))
        {
            return FALSE;
        }

        account_num = MMICOM_READ_2BYTE(file_info.data_ptr, NETWORK_ACCOUNT_NUM_OFFSET);

        if (0 == account_num)
        {
            is_res_data_ok = FALSE;
        }
        else if (PNULL != res_info_ptr)
        {
            uint16 i = 0;
            uint32 offset_addr = 0;
            uint32 data_offset = 0;

            res_info_ptr->res_item_ptr = SCI_ALLOCAZ(sizeof(MMICONNECTION_RES_ITEM_T) * account_num);

            if (PNULL == res_info_ptr->res_item_ptr)
            {
                res_info_ptr->res_num = 0;
            }
            else
            {
                //res_info_ptr->res_num = account_num;
                res_info_ptr->is_res_data = TRUE;

                for (i = 0; i < account_num; i++)
                {
                    uint8 a = 0;
                    uint8 b = 0;
                    uint8 c = 0;
                    uint8 d = 0;
                    uint16 len = 0;
                    uint32 temp_offset_addr = 0;
                    const wchar *name_ptr = PNULL;
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                    uint16 j = 0;
#endif
                    a = MMICOM_READ_BYTE(file_info.data_ptr, (NETWORK_ACCOUNT_HEADER_LENGTH + i * NETWORK_ACCOUNT_OFFSET_LENGTH));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (NETWORK_ACCOUNT_HEADER_LENGTH + i * NETWORK_ACCOUNT_OFFSET_LENGTH + 1));
                    c = MMICOM_READ_BYTE(file_info.data_ptr, (NETWORK_ACCOUNT_HEADER_LENGTH + i * NETWORK_ACCOUNT_OFFSET_LENGTH + 2));
                    d = MMICOM_READ_BYTE(file_info.data_ptr, (NETWORK_ACCOUNT_HEADER_LENGTH + i * NETWORK_ACCOUNT_OFFSET_LENGTH + 3));

                    temp_offset_addr = offset_addr;

                    offset_addr = CONVERT4U8TOU32(a, b, c, d);

                    if (0 != data_offset &&
                        temp_offset_addr + data_offset != offset_addr )//��Դ����
                    {
                        is_res_data_ok = FALSE;

                        break;
                    }
                    //get ip type,( uiorder is insert before mcc,so we have to adjust it.
                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + 1));
#ifdef IPVERSION_SUPPORT_V4_V6
                    res_info_ptr->res_item_ptr[i].ip_type= CONVERT2U8TOU16(a, b);
#endif
                    //get uioder,( uiorder is insert before mcc,so we have to adjust it.
                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_MCC_OFFSET));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_MCC_OFFSET + 1));
                    res_info_ptr->res_item_ptr[i].uiorder= CONVERT2U8TOU16(a, b);

                    offset_addr  = offset_addr+NETWORK_ACCOUNT_ITEM_UIODER_LEN;
                    //get mcc
                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_MCC_OFFSET));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_MCC_OFFSET + 1));

                    res_info_ptr->res_item_ptr[i].mcc = CONVERT2U8TOU16(a, b);

                    //get mnc
                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_MNC_OFFSET));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_MNC_OFFSET + 1));

                    res_info_ptr->res_item_ptr[i].mnc = CONVERT2U8TOU16(a, b);

                    //get port
                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_PORT_OFFSET));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_PORT_OFFSET + 1));

                    res_info_ptr->res_item_ptr[i].port = CONVERT2U8TOU16(a, b);

                    //get access type
                    res_info_ptr->res_item_ptr[i].access_type = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_ACCESS_TYPE_OFFSET));

                    //get access option
                    res_info_ptr->res_item_ptr[i].access_option = (MMICONNECTION_ACCESS_OPTION_E)MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_ACCESS_OPTION_OFFSET));

                    //get auth type
                    res_info_ptr->res_item_ptr[i].auth_type = (MN_PCO_AUTH_TYPE_E)MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_AUTH_TYPE_OFFSET));

                    //get account type
                    res_info_ptr->res_item_ptr[i].account_type = (MMICONNECTION_ACCOUNT_TYPE_E)MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_ACCOUNT_TYPE_OFFSET));

                    //get mvno type
                    res_info_ptr->res_item_ptr[i].mvno_type = (MMICONNECTION_ACCOUNT_TYPE_E)MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_MVNO_TYPE_OFFSET));

                    //get visible
                    res_info_ptr->res_item_ptr[i].visible = (MMICONNECTION_ACCOUNT_TYPE_E)MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_VISIBLE_OFFSET));

                    //get roaming
                    res_info_ptr->res_item_ptr[i].roaming = (MMICONNECTION_ACCOUNT_TYPE_E)MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_ROAMING_OFFSET));

                    //get roaming protocol
                    #ifdef IPVERSION_SUPPORT_V4_V6
                    res_info_ptr->res_item_ptr[i].roaming_protocol = (MMICONNECTION_ACCOUNT_TYPE_E)MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_ROAMING_PROTOCOL_OFFSET));
                    #endif
                    //get name
                    name_ptr = (const wchar *)(file_info.data_ptr + offset_addr + NETWORK_ACCOUNT_ITEM_NAME_LEN_OFFSET + NETWORK_ACCOUNT_ITEM_NAME_LEN_LENGTH);

                    len = MMIAPICOM_Wstrlen(name_ptr);
                    res_info_ptr->res_item_ptr[i].name_len = MIN(MMICONNECTION_MAX_LINKMODENAME_LEN, len);

                    res_info_ptr->res_item_ptr[i].name = SCI_ALLOCAZ((res_info_ptr->res_item_ptr[i].name_len + 1) * sizeof(wchar));
                    if(PNULL == res_info_ptr->res_item_ptr[i].name)
                    {
                        is_res_data_ok = FALSE;

                        i += 1;

                        break;
                    }

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                    for (j = 0; j < res_info_ptr->res_item_ptr[i].name_len; j++)
                    {
                        res_info_ptr->res_item_ptr[i].name[j] = ((name_ptr[j]<< 8)&0xFF00) + ((name_ptr[j]>>8)&0x00FF);
                    }
#else
                    MMI_WSTRNCPY(res_info_ptr->res_item_ptr[i].name, res_info_ptr->res_item_ptr[i].name_len, name_ptr, res_info_ptr->res_item_ptr[i].name_len, res_info_ptr->res_item_ptr[i].name_len);
#endif

                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_NAME_LEN_OFFSET));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + NETWORK_ACCOUNT_ITEM_NAME_LEN_OFFSET + 1));

                    data_offset = CONVERT2U8TOU16(a, b) + NETWORK_ACCOUNT_ITEM_NAME_LEN_OFFSET + NETWORK_ACCOUNT_ITEM_NAME_LEN_LENGTH;

                    //get gateway
                    res_info_ptr->res_item_ptr[i].gateway = (const char *)(file_info.data_ptr + offset_addr + data_offset + NETWORK_ACCOUNT_ITEM_GATEWAY_LENGTH);
                    if(PNULL == res_info_ptr->res_item_ptr[i].gateway)
                    {
                        is_res_data_ok = FALSE;

                        i += 1;

                        break;
                    }

                    len = strlen(res_info_ptr->res_item_ptr[i].gateway);
                    res_info_ptr->res_item_ptr[i].gateway_len = MIN(MMICONNECTION_MAX_GATEWAY_LEN, len);

                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset + 1));

                    data_offset += CONVERT2U8TOU16(a, b) + NETWORK_ACCOUNT_ITEM_GATEWAY_LENGTH;

                    //get dns1
                    res_info_ptr->res_item_ptr[i].dns1 = (const char *)(file_info.data_ptr + offset_addr + data_offset + NETWORK_ACCOUNT_ITEM_DNS1_LENGTH);
                    if(PNULL == res_info_ptr->res_item_ptr[i].dns1)
                    {
                        is_res_data_ok = FALSE;

                        i += 1;

                        break;
                    }

                    len = strlen(res_info_ptr->res_item_ptr[i].dns1);
                    res_info_ptr->res_item_ptr[i].dns1_len = MIN(MMICONNECTION_MAX_DNS_LEN, len);

                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset + 1));

                    data_offset += CONVERT2U8TOU16(a, b) + NETWORK_ACCOUNT_ITEM_DNS1_LENGTH;

                    //get dns2
                    res_info_ptr->res_item_ptr[i].dns2 = (const char *)(file_info.data_ptr + offset_addr + data_offset + NETWORK_ACCOUNT_ITEM_DNS2_LENGTH);
                    if(PNULL == res_info_ptr->res_item_ptr[i].dns2)
                    {
                        is_res_data_ok = FALSE;

                        i += 1;

                        break;
                    }

                    len = strlen(res_info_ptr->res_item_ptr[i].dns2);
                    res_info_ptr->res_item_ptr[i].dns2_len = MIN(MMICONNECTION_MAX_DNS_LEN, len);

                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset + 1));

                    data_offset += CONVERT2U8TOU16(a, b) + NETWORK_ACCOUNT_ITEM_DNS2_LENGTH;

                    //get username
                    res_info_ptr->res_item_ptr[i].username = (const char *)(file_info.data_ptr + offset_addr + data_offset + NETWORK_ACCOUNT_ITEM_USERNAME_LENGTH);
                    if(PNULL == res_info_ptr->res_item_ptr[i].username)
                    {
                        is_res_data_ok = FALSE;

                        i += 1;

                        break;
                    }

                    len = strlen(res_info_ptr->res_item_ptr[i].username);
                    res_info_ptr->res_item_ptr[i].username_len = MIN(MMICONNECTION_MAX_USERNAME_LEN, len);

                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset + 1));

                    data_offset += CONVERT2U8TOU16(a, b) + NETWORK_ACCOUNT_ITEM_USERNAME_LENGTH;

                    //get password
                    res_info_ptr->res_item_ptr[i].password = (const char *)(file_info.data_ptr + offset_addr + data_offset + NETWORK_ACCOUNT_ITEM_PASSWORD_LENGTH);
                    if(PNULL == res_info_ptr->res_item_ptr[i].password)
                    {
                        is_res_data_ok = FALSE;

                        i += 1;

                        break;
                    }

                    len = strlen(res_info_ptr->res_item_ptr[i].password);
                    res_info_ptr->res_item_ptr[i].password_len = MIN(MMICONNECTION_MAX_PASSWORD_LEN, len);

                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset + 1));

                    data_offset += CONVERT2U8TOU16(a, b) + NETWORK_ACCOUNT_ITEM_PASSWORD_LENGTH;

                    //get apn
                    res_info_ptr->res_item_ptr[i].apn = (const char *)(file_info.data_ptr + offset_addr + data_offset + NETWORK_ACCOUNT_ITEM_APN_LENGTH);
                    if(PNULL == res_info_ptr->res_item_ptr[i].apn)
                    {
                        is_res_data_ok = FALSE;

                        i += 1;

                        break;
                    }

                    len = strlen(res_info_ptr->res_item_ptr[i].apn);
                    res_info_ptr->res_item_ptr[i].apn_len = MIN(MMICONNECTION_MAX_APN_LEN, len);

                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset + 1));

                    data_offset += CONVERT2U8TOU16(a, b) + NETWORK_ACCOUNT_ITEM_APN_LENGTH;

                    //get homepage
                    res_info_ptr->res_item_ptr[i].homepage = (const char *)(file_info.data_ptr + offset_addr + data_offset + NETWORK_ACCOUNT_ITEM_HOMEPAGE_LENGTH);
                    if(PNULL == res_info_ptr->res_item_ptr[i].homepage)
                    {
                        is_res_data_ok = FALSE;

                        i += 1;

                        break;
                    }

                    len = strlen(res_info_ptr->res_item_ptr[i].homepage);
                    res_info_ptr->res_item_ptr[i].homepage_len = MIN(MMICONNECTION_HOMEPAGE_MAX_URL_LEN, len);

                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset + 1));

                    data_offset += CONVERT2U8TOU16(a, b) + NETWORK_ACCOUNT_ITEM_HOMEPAGE_LENGTH;

                    //get mvno value

                    res_info_ptr->res_item_ptr[i].mvno_value = (const char *)(file_info.data_ptr + offset_addr + data_offset + NETWORK_ACCOUNT_ITEM_SPN_LENGTH);
                    if(PNULL == res_info_ptr->res_item_ptr[i].mvno_value)
                    {
                        is_res_data_ok = FALSE;

                        i += 1;

                        break;
                    }

                    len = strlen(res_info_ptr->res_item_ptr[i].mvno_value);
                    res_info_ptr->res_item_ptr[i].mvno_value_len = MIN(MMICONNECTION_HOMEPAGE_MAX_URL_LEN, len);

                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset + 1));

                    data_offset += CONVERT2U8TOU16(a, b) + NETWORK_ACCOUNT_ITEM_SPN_LENGTH;

                }

                res_info_ptr->res_num = i;
                //is_res_data_ok = TRUE;
            }
        }
    }
    else
    {
        is_res_data_ok = FALSE;
    }

    //SCI_TRACE_LOW:"ParseNetworkAccountInfo is_res_data_ok=%d, account_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6374_112_2_18_2_12_18_18,(uint8*)"dd", is_res_data_ok, account_num);

    return is_res_data_ok;
}

/*****************************************************************************/
//  Description : Load NetAccount Info From Resource
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadNetworkAccountInfoFromRes(MN_DUAL_SYS_E dual_sys, MMICONNECTION_RES_INFO_T *res_info_ptr)
{
#ifdef CMCC_UI_STYLE
    return FALSE;  //cmcc load from list
#else

    MMIRES_DATA_INFO_T file_info = {0}; /*lint !e64*/
    BOOLEAN is_res_data_exist = FALSE;
    uint16 account_num = 0;
    uint32 network_account_id = NULL;

#ifdef DATA_ACCOUNT_IN_TCARD_SUPPORT
    if(!MMIAPIUDISK_UdiskIsRun())
    {
        uint16 ntac_file_path_w[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
        char ntac_file_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
        MMIFILE_HANDLE file_handle = 0;

        strncpy( ntac_file_path,NETACCOUNT_FILE_PATH,strlen((char*)NETACCOUNT_FILE_PATH));
        strcat(ntac_file_path,".ntac");

        MMI_STRNTOWSTR(ntac_file_path_w, MMIFILE_FULL_PATH_MAX_LEN, (uint8*)ntac_file_path, MMIFILE_FULL_PATH_MAX_LEN, MMIFILE_FULL_PATH_MAX_LEN );
        file_handle = MMIAPIFMM_CreateFile((const uint16*)ntac_file_path_w,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);

        if (SFS_INVALID_HANDLE != file_handle)
        {
            file_info.type = 0;
            file_info.data_len = MMIAPIFMM_GetFileSize(file_handle);
            file_info.data_ptr = SCI_ALLOCAZ(file_info.data_len);

            if (PNULL != file_info.data_ptr)
            {
                uint32 read_size = 0;
                SFS_ERROR_E error = SFS_NO_ERROR;

                error = MMIAPIFMM_ReadFile(file_handle, file_info.data_ptr, file_info.data_len, &read_size, PNULL);
                if ((SFS_NO_ERROR == error) && ParseNetworkAccountInfo(res_info_ptr, file_info))//�����ļ�����,����TRUE
                {
                    MMIAPIFMM_CloseFile(file_handle);

                    SCI_FREE(file_info.data_ptr);

                    return TRUE;
                }
                else
                {
                    UnloadNetworkAccountInfo(res_info_ptr);
                }

                SCI_FREE(file_info.data_ptr);
            }

            MMIAPIFMM_CloseFile(file_handle);
        }

     }
#endif
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        network_account_id = (MMIPLMN_NETWORK_ACC_ID_T)MMIAPIPLMN_GetLocalNetworkAccount(dual_sys);
    }

    if (NULL == network_account_id)
    {
#ifdef ORANGE_SUPPORT
        network_account_id = (MMIPLMN_NETWORK_ACC_ID_T)R_NETWORK_DATA_ORANGE;
#else
        network_account_id = (MMIPLMN_NETWORK_ACC_ID_T)R_NETWORK_ACCOUNT;
#endif

    }
#else
    network_account_id = R_NETWORK_ACCOUNT;
#endif

    //is_res_data_exist is false if come this line
    is_res_data_exist = MMI_GetDataInfo(network_account_id, &file_info);
    if (is_res_data_exist && ParseNetworkAccountInfo(res_info_ptr, file_info))
    {
        is_res_data_exist = TRUE;
    }
    else
    {
        UnloadNetworkAccountInfo(res_info_ptr);

        is_res_data_exist = FALSE;
    }

    //SCI_TRACE_LOW:"LoadNetworkAccountInfoFromRes is_res_data_exist=%d, account_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6454_112_2_18_2_12_18_19,(uint8*)"dd", is_res_data_exist, account_num);

    return is_res_data_exist;
#endif
}

#ifdef FOTA_SUPPORT
#ifdef CHIP_VER_UIS8910FF

LOCAL uint8* _GetApnMemAddress(void)
{
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD   
	uint8 *addr;

    if (PROD_GetApnMemSection(&addr) > 0)
        return addr + 20;
#endif
    return NULL;
}

LOCAL uint32 _GetApnMemLen(void)
{
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    uint8 *addr;
    if (PROD_GetApnMemSection(&addr) > 0)
        return *((uint32 *)(addr + 16));
#endif    
	return 0;
}

LOCAL uint8* _GetApnVersion(void)
{
    uint8 *addr;

    if (PROD_GetApnMemSection(&addr) > 0)
        return addr;

    return NULL;
}

LOCAL BOOLEAN LoadNetworkAccountInfoFromMem(MMICONNECTION_RES_INFO_T *res_info_ptr)
{
    MMIRES_DATA_INFO_T file_info = {0};

    file_info.type = 0;
    file_info.data_ptr = _GetApnMemAddress();
    file_info.data_len = _GetApnMemLen();

    SCI_TraceLow(" [sky] -- LoadNetworkAccountInfoFromMem - apn data (addr : %x, len : %d", file_info.data_ptr, file_info.data_len);

    if (file_info.data_len == 0 || file_info.data_ptr == NULL)
    {
        return FALSE;
    }
    return ParseNetworkAccountInfo(res_info_ptr, file_info);
}

#endif
#endif

/*****************************************************************************/
//  Description : Load NetAccount Info From Table List
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadNetworkAccountInfoFromTableList(MMICONNECTION_RES_INFO_T *res_info_ptr)
{
    if (PNULL != res_info_ptr)
    {
        res_info_ptr->res_item_ptr = SCI_ALLOCAZ(sizeof(MMICONNECTION_RES_ITEM_T) * LINK_SETTING_TABLE_LEN);

        if (PNULL == res_info_ptr->res_item_ptr)
        {
            res_info_ptr->res_num = 0;

            return FALSE;
        }
        else
        {
            uint32 i =0;

            res_info_ptr->res_num = LINK_SETTING_TABLE_LEN;
            res_info_ptr->is_res_data = FALSE;

            for (i = 0; i < LINK_SETTING_TABLE_LEN; i++)
            {
                MMI_STRING_T name_string = {0};
#ifdef CMCC_UI_STYLE
                uint32 j =0;
                uint16 custom_list_num = LINK_CUSTOMED_LEN;
#endif
                //mcc
                res_info_ptr->res_item_ptr[i].mcc = setting_search_table[i].mcc;

                //mnc
                res_info_ptr->res_item_ptr[i].mnc = setting_search_table[i].mnc;

                //port
                res_info_ptr->res_item_ptr[i].port = setting_search_table[i].port;

                //access_type
                res_info_ptr->res_item_ptr[i].access_type = setting_search_table[i].access_type;

                //access_option
                res_info_ptr->res_item_ptr[i].access_option = MMICONNECTION_ACCESS_EDITABLE;
#ifdef CMCC_UI_STYLE
                for (j = 0; j < custom_list_num; j++)
                {
                    if (i == (uint32)s_customed_index[j])
                    {
                        res_info_ptr->res_item_ptr[i].access_option = MMICONNECTION_ACCESS_READONLY;

                        break;
                    }
                }
#endif

                //auth_type
                res_info_ptr->res_item_ptr[i].auth_type = setting_search_table[i].auth_type;

                //account_type
                res_info_ptr->res_item_ptr[i].account_type = setting_search_table[i].account_type;

                //name
                MMI_GetLabelTextByLang(setting_search_table[i].name_index, &name_string);
                res_info_ptr->res_item_ptr[i].name_len = MIN(name_string.wstr_len, MMICONNECTION_MAX_USERNAME_LEN);
                res_info_ptr->res_item_ptr[i].name = SCI_ALLOCAZ((res_info_ptr->res_item_ptr[i].name_len + 1) * sizeof(wchar));
                MMI_WSTRNCPY(res_info_ptr->res_item_ptr[i].name,
                            res_info_ptr->res_item_ptr[i].name_len,
                            name_string.wstr_ptr,
                            res_info_ptr->res_item_ptr[i].name_len,
                            res_info_ptr->res_item_ptr[i].name_len);

                //gateway
                res_info_ptr->res_item_ptr[i].gateway = setting_search_table[i].gateway;
                res_info_ptr->res_item_ptr[i].gateway_len = strlen(res_info_ptr->res_item_ptr[i].gateway);

                //dns1
                res_info_ptr->res_item_ptr[i].dns1 = (const char *)MMICONNECTION_DEFAULT_DNS;
                res_info_ptr->res_item_ptr[i].dns1_len = strlen(res_info_ptr->res_item_ptr[i].dns1);

                //dns2
                res_info_ptr->res_item_ptr[i].dns2= (const char *)MMICONNECTION_DEFAULT_DNS;
                res_info_ptr->res_item_ptr[i].dns2_len = strlen(res_info_ptr->res_item_ptr[i].dns2);

                //username
                res_info_ptr->res_item_ptr[i].username = setting_search_table[i].username;
                res_info_ptr->res_item_ptr[i].username_len = strlen(setting_search_table[i].username);

                //password
                res_info_ptr->res_item_ptr[i].password = setting_search_table[i].password;
                res_info_ptr->res_item_ptr[i].password_len = strlen(setting_search_table[i].password);

                //apn
                res_info_ptr->res_item_ptr[i].apn = setting_search_table[i].apn;
                res_info_ptr->res_item_ptr[i].apn_len = strlen(setting_search_table[i].apn);

                //homepage
                res_info_ptr->res_item_ptr[i].homepage = setting_search_table[i].homepage;
                res_info_ptr->res_item_ptr[i].homepage_len = strlen(setting_search_table[i].homepage);
            }
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : Load NetAccount Info
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadNetworkAccountInfo(MN_DUAL_SYS_E dual_sys, MMICONNECTION_RES_INFO_T *res_info_ptr)
{
    BOOLEAN is_res_data_exist = FALSE;

#if defined (FOTA_SUPPORT) && !defined (CHIP_VER_UIS8910FF) && !defined(WIN32)
    is_res_data_exist = LoadNetworkAccountInfoFromMem(res_info_ptr);
#else
    is_res_data_exist = LoadNetworkAccountInfoFromRes(dual_sys, res_info_ptr);
#endif

    if (!is_res_data_exist)
    {
        is_res_data_exist = LoadNetworkAccountInfoFromTableList(res_info_ptr);
    }

    return is_res_data_exist;
}

/*****************************************************************************/
//  Description : Unload NetAccount Info
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UnloadNetworkAccountInfo(MMICONNECTION_RES_INFO_T *res_info_ptr)
{
    if (PNULL != res_info_ptr && PNULL != res_info_ptr->res_item_ptr)
    {
        uint32 i = 0;

        for (i = 0; i < res_info_ptr->res_num; i++)
        {
            if (PNULL != res_info_ptr->res_item_ptr[i].name)
            {
                SCI_FREE(res_info_ptr->res_item_ptr[i].name);
                res_info_ptr->res_item_ptr[i].name = PNULL;
            }
        }

        SCI_FREE(res_info_ptr->res_item_ptr);
        res_info_ptr->res_item_ptr = PNULL;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : Save NetAccount Info
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveNetworkAccountInfo(MMICONNECTION_LINKSETTING_DETAIL_T *link_detail_ptr,//IN/OUT
                                     MMICONNECTION_RES_ITEM_T           *res_item_ptr,//IN
                                     BOOLEAN                            is_default_unchange,
                                     uint16                             list_index)//IN
{
    BOOLEAN visible = TRUE;
    
    if (PNULL == link_detail_ptr || PNULL == res_item_ptr)
    {
        return FALSE;
    }

    //access type
    link_detail_ptr->access_type = res_item_ptr->access_type;

    //auth type
    link_detail_ptr->auth_type=res_item_ptr->auth_type;

    //port
    link_detail_ptr->port = res_item_ptr->port;

    //name
    link_detail_ptr->name_len = MIN(res_item_ptr->name_len, MMICONNECTION_MAX_LINKMODENAME_LEN);
    SCI_MEMSET(link_detail_ptr->name, 0x00, sizeof(link_detail_ptr->name));

    {
        MMI_TEXT_ID_T name_text_id = MMICONNECTION_GetChineseNetNameTextId(res_item_ptr->name, res_item_ptr->name_len);

        if((TXT_NULL != name_text_id)
            &&!MMIAPISET_CheckLanguageStatus(MMISET_LANGUAGE_SIMP_CHINESE)
            && !MMIAPISET_CheckLanguageStatus(MMISET_LANGUAGE_TRAD_CHINESE))
        {
            MMI_STRING_T name_string = {0};

            MMI_GetLabelTextByLang(name_text_id, &name_string);

            link_detail_ptr->name_len = MIN(name_string.wstr_len, MMICONNECTION_MAX_LINKMODENAME_LEN);
            MMIAPICOM_Wstrncpy(link_detail_ptr->name, name_string.wstr_ptr, link_detail_ptr->name_len);
        }
        else
        {
            MMIAPICOM_Wstrncpy(link_detail_ptr->name, res_item_ptr->name, link_detail_ptr->name_len);
        }
    }

    //gateway
    link_detail_ptr->gateway_len = res_item_ptr->gateway_len;
    SCI_MEMSET(link_detail_ptr->gateway,0x00,sizeof(link_detail_ptr->gateway));
    SCI_MEMCPY(link_detail_ptr->gateway,res_item_ptr->gateway,link_detail_ptr->gateway_len);
    SCI_TRACE_LOW("[szp]AutoAdap  gateway:%s, gateway_len:%d",link_detail_ptr->gateway, link_detail_ptr->gateway_len);
    //user name
    link_detail_ptr->username_len = MIN(res_item_ptr->username_len, MMICONNECTION_MAX_USERNAME_LEN);
    SCI_MEMSET(link_detail_ptr->username, 0x00, sizeof(link_detail_ptr->username));
    SCI_MEMCPY(link_detail_ptr->username, res_item_ptr->username, link_detail_ptr->username_len);

    //password
    link_detail_ptr->password_len = MIN(res_item_ptr->password_len, MMICONNECTION_MAX_PASSWORD_LEN);
    SCI_MEMSET(link_detail_ptr->password, 0x00, sizeof(link_detail_ptr->password));
    SCI_MEMCPY(link_detail_ptr->password, res_item_ptr->password, link_detail_ptr->password_len);

    //apn
    link_detail_ptr->apn_len = MIN(res_item_ptr->apn_len, MMICONNECTION_MAX_APN_LEN);
    SCI_MEMSET(link_detail_ptr->apn, 0x00, sizeof(link_detail_ptr->apn));
    SCI_MEMCPY(link_detail_ptr->apn, res_item_ptr->apn, link_detail_ptr->apn_len);

    //home page
    link_detail_ptr->homepage_len = MIN(res_item_ptr->homepage_len, MMICONNECTION_HOMEPAGE_MAX_URL_LEN);
    SCI_MEMSET(link_detail_ptr->homepage, 0x00, sizeof(link_detail_ptr->homepage));
    SCI_MEMCPY(link_detail_ptr->homepage, res_item_ptr->homepage, link_detail_ptr->homepage_len);

    //dns1
    link_detail_ptr->dns_len = MIN(res_item_ptr->dns1_len, MMICONNECTION_MAX_DNS_LEN);
    SCI_MEMSET(link_detail_ptr->dns, 0x00, sizeof(link_detail_ptr->dns));
    SCI_MEMCPY(link_detail_ptr->dns, res_item_ptr->dns1, link_detail_ptr->dns_len);

    //is_customed_detail
    link_detail_ptr->is_customed_detail = (MMICONNECTION_ACCESS_READONLY == res_item_ptr->access_option) ? TRUE : FALSE;

    //priority
    //link_detail_ptr->priority = FALSE;

    //is_default_unchange
    link_detail_ptr->is_default_unchange = is_default_unchange;

    //list index
    link_detail_ptr->list_index = list_index;

    //plmn
    link_detail_ptr->plmn.mcc = res_item_ptr->mcc;
    link_detail_ptr->plmn.mnc = res_item_ptr->mnc;

    //hplmn
    link_detail_ptr->hplmn = MMIAPIPHONE_GetNetWorkId( &link_detail_ptr->plmn );

    //account_type
    link_detail_ptr->account_type = res_item_ptr->account_type;
#ifdef IPVERSION_SUPPORT_V4_V6
    link_detail_ptr->ip_type = res_item_ptr->ip_type;
#endif
    link_detail_ptr->visible = res_item_ptr->visible;
    link_detail_ptr->uiorder = res_item_ptr->uiorder;
    visible = res_item_ptr->visible;
    SCI_TRACE_LOW("[szp] mcc=%d,mnc=%d,mvnotype=%d,uiorder=%d,visible=%d", res_item_ptr->mcc,res_item_ptr->mnc, res_item_ptr->mvno_type, res_item_ptr->uiorder, visible);
    
    return visible;
}

/*****************************************************************************/
// 	Description : create Linklist Win for mms,browser,dcd,java
//	Global resource dependence : none
//  Author: jixin.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_CreateLinklistWin(ADD_DATA param, MMICONNECTION_CALLBACK_FUNC callback_func)
{
    /*avoid ��������*/
    MMK_CloseWin(MMICONNECTION_LINKLIST_WIN_ID);

    s_connection_callback_func_ptr = callback_func;
    MMK_CreateWin((uint32*)MMICONNECTION_LINKLIST_WIN_TAB, param);
}

/*****************************************************************************/
// 	Description : check avalid data account without gateway IP for SNTP of current SIM
//	Global resource dependence : s_connection_setting_info
//    Author: dave.ruan
//	Note:item_num_ptr total derect link item number;
//             detail_item:setting detail,include dual_sys and item_index
//             dual_sys sim index
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_GetCurSIMNoProxyAccountIndex(MN_DUAL_SYS_E   dual_sys, uint8* item_num,MMICONNECTION_SETTING_DETAIL_ITEM_T* detail_item,uint8 max_item_num )
{
    uint8    settinglink_dual_sys_index = 0;
    uint8    total_settinglink_num = 0;
    MN_PLMN_T cur_service_plmn = {0};

    if(PNULL == item_num ||PNULL == detail_item)
    {
        //SCI_TRACE_LOW:"MMIAUT::GetCurSIMNoProxyAccountIndex  item_num ||detail_item is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6732_112_2_18_2_12_19_20,(uint8*)"");
        return;
    }

    MMIAPIPHONE_GetCurPLMN(&cur_service_plmn, dual_sys);

    if(MMIAPIPHONE_IsSimOk(dual_sys))//SIM init complete
    {
        for(settinglink_dual_sys_index = 0; settinglink_dual_sys_index < s_connection_setting_info.settinglink_num[dual_sys];settinglink_dual_sys_index++)
        {
            if(0 < s_connection_setting_info.setting_detail[dual_sys][settinglink_dual_sys_index].apn_len
                //&&(0 == strncmp(MMICONNECTION_INVALID_GATEWAY_FOR_SNTP, (char*)s_connection_setting_info.setting_detail[dual_sys][settinglink_dual_sys_index].gateway,strlen(MMICONNECTION_INVALID_GATEWAY_FOR_SNTP)))
            )
            {
#ifndef WIN32   //only mcc = 460,apn is cmnet;other mcc is use default,not really in china
                if(460 != cur_service_plmn.mcc
                && (0==strncmp(MMICONNECTION_CMCC_DERECT_LINK_APN,(char*)s_connection_setting_info.setting_detail[dual_sys][settinglink_dual_sys_index].apn,strlen(MMICONNECTION_CMCC_DERECT_LINK_APN))))
                {
                    continue;
                }
#endif
                if(total_settinglink_num >= max_item_num)
                {
                    break;
                }

                detail_item[total_settinglink_num].dual_sys = dual_sys;
                detail_item[total_settinglink_num].item_index = settinglink_dual_sys_index;
                total_settinglink_num++;
            }
            //SCI_TRACE_LOW:"MMIAUT::SearchDirectLinkAccountIndex total_settinglink_num =%d,dual_sys =%d,settinglink_dual_sys_index=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6763_112_2_18_2_12_19_21,(uint8*)"ddd",total_settinglink_num,dual_sys,settinglink_dual_sys_index);
        }
    }
    * item_num = total_settinglink_num;

    //SCI_TRACE_LOW:"MMIAUT::SearchDirectLinkAccountIndex * item_num =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6768_112_2_18_2_12_19_22,(uint8*)"d",* item_num);
}

/*****************************************************************************/
// 	Description : check avalid data account without gateway IP for SNTP
//	Global resource dependence : s_connection_setting_info
//  Author: dave.ruan
//	Note:item_num_ptr total derect link item number;
//             detail_item:setting detail,include dual_sys and item_index
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_GetNoProxyAccountIndex(uint8* item_num,MMICONNECTION_SETTING_DETAIL_ITEM_T* detail_item, uint8 max_item_num)
{
    MN_DUAL_SYS_E   dual_sys =MMI_DUAL_SYS_MAX;
    uint8    settinglink_dual_sys_index = 0;
    uint8    total_settinglink_num = 0;

    if(PNULL == item_num ||PNULL == detail_item)
    {
        //SCI_TRACE_LOW:"MMIAUT::GetNoProxyAccountIndex  item_num ||detail_item is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6784_112_2_18_2_12_19_23,(uint8*)"");
        return;
    }

    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys ++)
    {
        MN_PLMN_T cur_service_plmn = {0};

        MMIAPIPHONE_GetCurPLMN(&cur_service_plmn, dual_sys);

        if(MMIAPIPHONE_IsSimOk(dual_sys))//SIM init complete
        {
            for(settinglink_dual_sys_index = 0;settinglink_dual_sys_index < s_connection_setting_info.settinglink_num[dual_sys];settinglink_dual_sys_index++)
            {
                if(0 < s_connection_setting_info.setting_detail[dual_sys][settinglink_dual_sys_index].apn_len
                   // &&(0 == strncmp(MMICONNECTION_INVALID_GATEWAY_FOR_SNTP, (char*)s_connection_setting_info.setting_detail[dual_sys][settinglink_dual_sys_index].gateway,strlen(MMICONNECTION_INVALID_GATEWAY_FOR_SNTP)))
                )
                {
                    //�ų�һ���쳣��ֱ�����������ʹ��Ĭ�ϻ�����CMCC��ʱ����ʾ������
#ifndef WIN32   //only mcc = 460,apn is cmnet;other mcc is use default,not really in china
                    if(460 != cur_service_plmn.mcc
                    && (0==strncmp(MMICONNECTION_CMCC_DERECT_LINK_APN,(char*)s_connection_setting_info.setting_detail[dual_sys][settinglink_dual_sys_index].apn,strlen(MMICONNECTION_CMCC_DERECT_LINK_APN))))
                    {
                        continue;
                    }
#endif                    
                    if(total_settinglink_num >= max_item_num)
                    {
                        break;
                    }

                    detail_item[total_settinglink_num].dual_sys = dual_sys;
                    detail_item[total_settinglink_num].item_index = settinglink_dual_sys_index;
                    total_settinglink_num++;
                }
                //SCI_TRACE_LOW:"MMIAUT::SearchDirectLinkAccountIndex total_settinglink_num =%d,dual_sys =%d,settinglink_dual_sys_index=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6820_112_2_18_2_12_19_24,(uint8*)"ddd",total_settinglink_num,dual_sys,settinglink_dual_sys_index);
            }
        }
    }
    * item_num = total_settinglink_num;

    //SCI_TRACE_LOW:"MMIAUT::SearchDirectLinkAccountIndex * item_num =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_6826_112_2_18_2_12_19_25,(uint8*)"d",* item_num);
}


/*****************************************************************************/
//  Description : Select SIM API of Connection app
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMICONNECTION_SelectSIMFunc(void)
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) && defined (MMI_GPRS_SUPPORT)
    MMI_WIN_ID_T    selectSimWinID = MMICONNECTION_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    caller_data.append_type = (uint32)MMISET_APPEND_SIM_OK;
    caller_data.callback_func =ConnectionSelectSimCallback ;
    caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;

    caller_data.publist_id = MMICONNECTION_COM_SELECT_SIM_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = PNULL;
    caller_data.extra_data = PNULL;

    MMIAPIPHONE_OpenSelectSimWin(&caller_data);

#endif
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Get Chinese Net Name Text Id
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:CMCC and CUCC
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMICONNECTION_GetChineseNetNameTextId(const wchar *resource_name, uint16 name_len)
{
    MMI_TEXT_ID_T net_name_txt_id = TXT_NULL;
    uint32 i = 0;

    if(PNULL == resource_name || 0 == name_len)
    {
        return net_name_txt_id;
    }

    for(i = 0; i < LINK_SETTING_NAME_TABLE_LEN; i++)
    {
        if(0 == memcmp(setting_name_table[i].name_ptr, resource_name, name_len *sizeof(wchar)))
        {
            net_name_txt_id = setting_name_table[i].name_text_id;
            break;
        }
    }

     return net_name_txt_id;
}

#ifdef DERECT_LINK_FIRST_SUPPORT
/*****************************************************************************/
//  Description : Set Derect Link Index First
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:change the derect link detail as first account if not
/*****************************************************************************/
PUBLIC BOOLEAN MMICONNECTION_SetDerectLinkToFirstIndex(MMICONNECTION_SETTING_T *connection_setting)
{
    MN_DUAL_SYS_E  dual_sys = MMI_DUAL_SYS_MAX;
    uint32 i = 0;

    if(PNULL == connection_setting)
    {
        return FALSE;
    }

    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        MMICONNECTION_LINKSETTING_DETAIL_T temp_setting_detail = {0};

        if(0 < connection_setting->setting_detail[dual_sys][0].apn_len
                &&(0 == strncmp(MMICONNECTION_INVALID_GATEWAY_FOR_SNTP, (char*)connection_setting->setting_detail[dual_sys][0].gateway,strlen(MMICONNECTION_INVALID_GATEWAY_FOR_SNTP)))
            )
        {
            //the first account is derect Link, goto next SIM card
            continue;
        }
        else
        {
            SCI_MEMCPY(&temp_setting_detail, &connection_setting->setting_detail[dual_sys][0], sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
        }

        for(i = 1; i < connection_setting->settinglink_num[dual_sys]; i++)// search from 1, 0 is not derect link
        {
            if(0 < connection_setting->setting_detail[dual_sys][i].apn_len
                    &&(0 == strncmp(MMICONNECTION_INVALID_GATEWAY_FOR_SNTP, (char*)connection_setting->setting_detail[dual_sys][i].gateway,strlen(MMICONNECTION_INVALID_GATEWAY_FOR_SNTP)))
                )
            {
                //copy link index i to first item
                SCI_MEMSET(&connection_setting->setting_detail[dual_sys][0], 0x00, sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                SCI_MEMCPY(&connection_setting->setting_detail[dual_sys][0], &connection_setting->setting_detail[dual_sys][i], sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));

                //copy first link index to i
                SCI_MEMSET(&connection_setting->setting_detail[dual_sys][i], 0x00, sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                SCI_MEMCPY(&connection_setting->setting_detail[dual_sys][i], &temp_setting_detail, sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                break;
            }
        }
    }

     return TRUE;
}
#endif

#endif

//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
/*****************************************************************************/
// 	Description : handle data roaming query window
//	Global resource dependence : none
//  Author: feng.xiao
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDataRoamingQueryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E	res = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, TRUE);
        break;
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
		{
            MMIAPICONNECTION_Setting_SetDataRoamingStatus(MMICONNECTION_SETTING_DATA_ROAMING_ENABLE);
            MMK_CloseWin(win_id);
			break;
		}
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }
    default:
        {
            res = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            break;
        }
    }
    return res;
}

/*****************************************************************************/
// 	Description : handle data roaming cut off query window
//	Global resource dependence : none
//  Author: feng.xiao
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDataRoamingCutoffQueryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E	res = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
		{
            MMIAPIPDP_ReleaseAll(NULL);
            MMIAPICONNECTION_Setting_SetDataRoamingStatus(MMICONNECTION_SETTING_DATA_ROAMING_DISABLE);
            MMK_CloseWin(win_id);
			break;
		}
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }
    default:
        {
            res = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            break;
        }
    }
    return res;
}
#endif

#if defined(DATA_ACCOUNT_IN_TCARD_SUPPORT)
#if defined(BROWSER_SUPPORT)
/*****************************************************************************/
// 	Description : CheckNetworkAccountResFileToDownload
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void CheckNetworkAccountResFileToDownload(void)
{
    uint16 ntac_file_path_w[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    char ntac_file_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 file_path_len = 0;

    strncpy( ntac_file_path,NETACCOUNT_FILE_PATH,strlen((char*)NETACCOUNT_FILE_PATH));
    strcat(ntac_file_path,".ntac");

    MMI_STRNTOWSTR(ntac_file_path_w, MMIFILE_FULL_PATH_MAX_LEN, (uint8*)ntac_file_path, MMIFILE_FULL_PATH_MAX_LEN, MMIFILE_FULL_PATH_MAX_LEN );
    file_path_len = MMIAPICOM_Wstrlen((const wchar *)ntac_file_path_w);

    if(MMIAPIFMM_IsFileExist((const wchar *)ntac_file_path_w,file_path_len))
    {
        MMIPUB_OpenQueryTextWinByTextId(TXT_CONNECTION_REPLACE_DATA_QUERY,
                                        PNULL,
                                        HandleReplaceExistFileQueryWinMsg);
    }
    else
    {
        MMICONNECTION_OpenNetAccountDownLoadUrl();
    }
}

/*****************************************************************************/
// 	Description : handle refrsh setting list query window
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReplaceExistFileQueryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMICONNECTION_OpenNetAccountDownLoadUrl();
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        res = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
        break;
    }

    return res;
}

/*******************************************************************/
//  Interface:	MMICONNECTION_OpenNetAccountDownLoadUrl
//  Description : 	mmi connection Open Net account  DownLoad Url
//  Global resource dependence :
//  Author:dave.ruan
//  Note:
/*******************************************************************/
PUBLIC void MMICONNECTION_OpenNetAccountDownLoadUrl(void)
{
    uint8 *url_ptr = PNULL;
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
    uint16 str_len = (uint16)(MMIAPICOM_Wstrlen(NETACCOUNT_FILE_DOWNLOAD_URL) * 3 + 1);

    url_ptr = SCI_ALLOCAZ(str_len * sizeof(uint8));
    if (PNULL != url_ptr)
    {
    	GUI_WstrToUTF8(url_ptr, str_len, NETACCOUNT_FILE_DOWNLOAD_URL, MMIAPICOM_Wstrlen(NETACCOUNT_FILE_DOWNLOAD_URL));

    	entry_param.type = MMIBROWSER_ACCESS_URL;
    	entry_param.dual_sys = MN_DUAL_SYS_MAX;
    	entry_param.url_ptr = (char *)url_ptr;
    	entry_param.user_agent_ptr = PNULL;

    	MMIAPIBROWSER_Entry(&entry_param);
    	SCI_FREE(url_ptr);
    }
}

/*******************************************************************/
//  Interface:		HandleNetworkAccountDownloadCB
//  Description : 	Handle Network Account DownloadCB
//  Global resource dependence :
//  Author: dave.ruan
//  Note:
/*******************************************************************/
LOCAL void HandleNetworkAccountDownloadCB(
								   MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr
								   )
{
    if (PNULL != param_ptr && MMIBROWSER_MIME_TYPE_NTAC == param_ptr->mime_type)
    {
        //SCI_TRACE_LOW:"HandleNetworkAccountDownloadCB enter"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONN_MANAGER_WINTAB_7030_112_2_18_2_12_19_26,(uint8*)"");
        MoveFileToNetworkAccount(param_ptr->file_name);
        MMIAPIBROWSER_Close();
        MMIPUB_OpenQueryTextWinByTextId(TXT_CONNECTION_REFRESH_ACCOUNT,
                                        PNULL,
                                        HandleRefreshSettingListQueryWinMsg);
    }
}

/*******************************************************************/
//  Description : 	MMICONNECTION_RegBrowserCB
//  Global resource dependence :
//  Author: dave.ruan
//  Note:
/*******************************************************************/
PUBLIC void MMICONNECTION_RegBrowserCB(void)
{
	MMIBROWSER_MIME_DOWNLOAD_PARAM_T ntac_callback = {0};

	ntac_callback.call_back = HandleNetworkAccountDownloadCB;
	ntac_callback.mime_type = MMIBROWSER_MIME_TYPE_NTAC;
	MMIAPIBROWSER_RegMimeFunc(&ntac_callback);
}
#endif

/*******************************************************************/
//  Interface:		MoveFileToNetworkAccount
//  Description : 	�������ص��������ȴ洢��T��,��ͬ�ļ������Ը��ǡ�
//  Global resource dependence :
//  Author: dave.ruan
//  Note:
/*******************************************************************/
LOCAL void MoveFileToNetworkAccount(const wchar *file_path)
{
    wchar *dest_file_path_ptr = PNULL;
    wchar   file_name[MMIFILE_FILE_NAME_MAX_LEN+1]   = {0};
    uint16  file_name_len = 0;
    uint16  file_path_len = 0;
    uint32  file_size = 0;
    SFS_ERROR_E sfs_error = SFS_NO_ERROR;
    wchar new_full_path[MMIFILE_FULL_PATH_MAX_LEN+1]   = {0};
    uint16 ntac_file_name_len = 0;
    uint16 new_full_path_len = MMIFILE_FULL_PATH_MAX_LEN+1;

    if(PNULL == file_path)
    {
        return;
    }

    file_path_len = MMIAPICOM_Wstrlen(file_path);
    MMIAPIFMM_SplitFullPath(file_path, file_path_len,PNULL, PNULL, PNULL, PNULL,
                                    file_name, &file_name_len);

    //check if file name is "R_NETWORK_ACCOUNT.ntac", if not rename it, then copy to dest dir
    ntac_file_name_len  = strlen(NETACCOUNT_FILE_NAME);
    if(0 != MMIAPICOM_WstrStrncmp((const wchar*)file_name, (const uint8*)NETACCOUNT_FILE_NAME, ntac_file_name_len))
    {
        wchar ntac_file_name_w[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
        wchar  device_name[MMIFILE_DEVICE_NAME_MAX_LEN+2] = {0};
        uint16  device_name_len = 0;
        wchar  dir_name[MMIFILE_DIR_NAME_MAX_LEN+2] = {0};
        uint16  dir_name_len = 0;

        MMI_STRNTOWSTR(
                                ntac_file_name_w,
                                MMIFILE_FULL_PATH_MAX_LEN,
                                (uint8*)NETACCOUNT_FILE_NAME,
                                MMIFILE_FULL_PATH_MAX_LEN,
                                ntac_file_name_len);

        //get real file name
        if(MMIAPIFMM_SplitFullPath(file_path, file_path_len,
                             device_name, &device_name_len,
                             dir_name, &dir_name_len,
                             NULL, NULL))
        {
            MMIAPIFMM_CombineFullPath(device_name, device_name_len,
                                   dir_name, dir_name_len,
                                   ntac_file_name_w, ntac_file_name_len,
                                   new_full_path, &new_full_path_len);
            file_path_len = new_full_path_len;

            if(MMIAPIFMM_IsFileExist(new_full_path,new_full_path_len))
            {
                sfs_error = MMIAPIFMM_DeleteFile(new_full_path, NULL);
            }

            if (SFS_NO_ERROR == sfs_error)
            {
                if(MMIAPIFMM_RenameFile(file_path,MMIAPICOM_Wstrlen(file_path),
                                                            ntac_file_name_w,strlen(NETACCOUNT_FILE_NAME)))
                {
                    //use new_full_path instead of file_path
                }
                else
                {
                    SCI_MEMSET(new_full_path, 0x00, MMIFILE_FILE_NAME_MAX_LEN);
                    MMI_WSTRNCPY(
                                                new_full_path,
                                                MMIFILE_FILE_NAME_MAX_LEN,
                                                file_path,
                                                MMIFILE_FILE_NAME_MAX_LEN,
                                                file_path_len);
                }
            }
            else
            {
                SCI_MEMSET(new_full_path, 0x00, MMIFILE_FILE_NAME_MAX_LEN);
                MMI_WSTRNCPY(
                                            new_full_path,
                                            MMIFILE_FILE_NAME_MAX_LEN,
                                            file_path,
                                            MMIFILE_FILE_NAME_MAX_LEN,
                                            file_path_len);
            }
        }
    }
    else
    {
        //use file_path
        SCI_MEMSET(new_full_path, 0x00, MMIFILE_FILE_NAME_MAX_LEN);
        MMI_WSTRNCPY(
                                    new_full_path,
                                    MMIFILE_FILE_NAME_MAX_LEN,
                                    file_path,
                                    MMIFILE_FILE_NAME_MAX_LEN,
                                    file_path_len);
    }

    dest_file_path_ptr = (wchar *)SCI_ALLOCAZ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));
    if(PNULL == dest_file_path_ptr)
    {
        return;
    }

    MMIAPIFMM_GetFileInfo(new_full_path,file_path_len,&file_size,PNULL,PNULL);

    MMIAPIFMM_SplitFullPath(new_full_path, file_path_len,PNULL, PNULL, PNULL, PNULL,
                                    file_name, &file_name_len);

    //dest_file_path is set in GetValidNetAccountFileDir
    if(GetValidNetAccountFileDir(dest_file_path_ptr, file_name, file_size))
    {
        sfs_error = MMIAPIFMM_CopyFile(new_full_path, dest_file_path_ptr);
        if (SFS_NO_ERROR == sfs_error)
        {
            // ����ԭ�����ļ����ͻ���
            //sfs_error = MMIAPIFMM_DeleteFile(new_full_path, NULL);
        }
        else if (SFS_ERROR_NO_SPACE == sfs_error)
        {
           MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);
        }
        else
        {
           MMIPUB_OpenAlertWarningWin(TXT_MMI_SAVEFAILED);
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
    }

    SCI_FREE(dest_file_path_ptr);
}

/*****************************************************************************/
//  Description : get location file dir
//  Global resource dependence :
//  Author:dave.ruan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetValidNetAccountFileDir(uint16* device_dir,const wchar *file_name_ptr,uint32 file_size)
{
    MMIFILE_DEVICE_E file_dev_type = MMIAPIFMM_GetDefaultDisk();
    BOOLEAN          result = TRUE;

    if(MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, file_dev_type, file_size, PNULL))
    {
        switch (file_dev_type)
        {
        case MMI_DEVICE_UDISK:
            MMIAPICOM_Wstrncpy(device_dir,NETACCOUNT_FILE_UDISK_PATH,MMIAPICOM_Wstrlen(NETACCOUNT_FILE_UDISK_PATH));
            break;
        case MMI_DEVICE_SDCARD:
            MMIAPICOM_Wstrncpy(device_dir,NETACCOUNT_FILE_TCARD_PATH,MMIAPICOM_Wstrlen(NETACCOUNT_FILE_TCARD_PATH));
            break;
        case MMI_DEVICE_SDCARD_1:
            MMIAPICOM_Wstrncpy(device_dir,NETACCOUNT_FILE_TCARD1_PATH,MMIAPICOM_Wstrlen(NETACCOUNT_FILE_TCARD1_PATH));
            break;
        default:
            MMIAPICOM_Wstrncpy(device_dir,NETACCOUNT_FILE_UDISK_PATH,MMIAPICOM_Wstrlen(NETACCOUNT_FILE_UDISK_PATH));
            break;
        }
        MMIAPICOM_Wstrcat(device_dir, file_name_ptr);
    }
    else
    {
        result = FALSE;
    }
    return result;
}

/*****************************************************************************/
// 	Description : handle refrsh setting list query window
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRefreshSettingListQueryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMICONNECTION_RefreshSettingList();
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        res = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
        break;
    }

    return res;
}

#ifdef MMI_GPRS_SUPPORT
/*******************************************************************/
//  Description : 	MMICONNECTION_RefreshSettingList
//  Global resource dependence :
//  Author: dave.ruan
//  Note: refresh  setting list after load list
/*******************************************************************/
PUBLIC void MMICONNECTION_RefreshSettingList(void)
{
#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
	MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
	MN_PLMN_T carrier_hplmn[MMI_DUAL_SYS_MAX] = 0;
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	MMINV_READ(MMINV_CARRIER_PLMN, carrier_hplmn, return_value);
	if (return_value != MN_RETURN_SUCCESS)
	{
		SCI_MEMSET(carrier_hplmn, 0x00, sizeof(carrier_hplmn));
	}

	for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX;dual_sys++)
	{
		if(MMIAPIPHONE_IsSimCardOk(dual_sys))
		{
			carrier_hplmn[dual_sys].mcc= 0x0;
			carrier_hplmn[dual_sys].mnc= 0x0;
			    	MMINV_WRITE(MMINV_CARRIER_PLMN, carrier_hplmn);
			    	MMICONNECTION_AutoAdapting(dual_sys);
		}
	}
#endif
#ifdef MMI_GUI_STYLE_TYPICAL
	MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
}
#endif
#endif

#ifdef MMI_GPRS_DATA_SIM_SET
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGPRSModeSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMICONNECTION_GPRS_MODE_CTRL_ID;
    MMISET_GPRS_MODE_TYPE_E     mode_type = DATA_CARD_MANUAL_SELECT;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            int32           i= 0;
            MMI_TEXT_ID_T   text_id[] = {TXT_CONNECTION_DATA_CARD_SIM1, TXT_CONNECTION_DATA_CARD_SIM2, TXT_CONNECTION_DATA_CARD_SIM3, TXT_CONNECTION_DATA_CARD_SIM4};

            GUILIST_SetMaxItem(ctrl_id, DATA_CARD_MODE_MAX, FALSE );

            for(i = 0; i < MN_DUAL_SYS_MAX; i++)
            {
                MMIAPISET_AppendListItemByTextId(text_id[i], STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT);
            }

            MMIAPISET_AppendListItemByTextId(TXT_CONNECTION_DATA_PROMPT_ALWAYS, STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT);

            mode_type = MMISET_GetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_WAP);
            if (DATA_CARD_MANUAL_SELECT == mode_type)
            {
                uint16  total_num = 0;

                total_num = GUILIST_GetTotalItemNum(ctrl_id);

                if (total_num != 0)
                {
                    mode_type = total_num - 1;
                }
            }
            GUILIST_SetSelectedItem(ctrl_id, (uint16)mode_type, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, (uint16)mode_type);

            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN, FALSE);
        }
        break;

    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            uint16  cur_selection = 0;
            uint16  total_num = 0;

            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            total_num = GUILIST_GetTotalItemNum(ctrl_id);

            if (cur_selection == total_num - 1)
            {
                mode_type = DATA_CARD_MANUAL_SELECT;
				MMISET_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_WAP,MMISET_MAJOR_SIM_SEL_MANUAL_SEL);
            }
            else
            {
                mode_type = (MMISET_GPRS_MODE_TYPE_E)cur_selection;
				MMISET_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_WAP,(MMISET_GPRS_MODE_TYPE_E)cur_selection);
            }

            MMIAPISET_SetGPRSModeType(mode_type);
			
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            MMK_CloseWin(win_id);
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

    return (recode);
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenGPRSModeSetting(void)
{
    MMK_CreatePubListWin((uint32 *)MMICONNECTION_GPRS_MODE_WIN_TAB, PNULL);
}
#endif


#ifdef DATA_ROAMING_SUPPORT
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGPRSDataRoamingSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMICONNECTION_GPRS_DATA_ROAMING_CTRL_ID;
    MMICONNECTION_SETTING_DATA_ROAMING_E data_roaming_status = MMAPIICONNECTION_Setting_GetDataRoamingStatus();
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            int32           i= 0;
            MMI_TEXT_ID_T   text_id[] = {STR_POC_ILIST_OFF, STR_ROAMING_DOMESTIC, STR_ROAMING_ALL};
            
            GUILIST_SetMaxItem(ctrl_id, MMICONNECTION_SETTING_DATA_ROAMING_MAX, FALSE );
            
            for(i = 0; i < MMICONNECTION_SETTING_DATA_ROAMING_MAX; i++)
            {
                MMIAPISET_AppendListItemByTextId(text_id[i], STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT);
            }
            
            switch(data_roaming_status)
            {
            case MMICONNECTION_SETTING_DATA_ROAMING_ENABLE:
                GUILIST_SetSelectedItem(ctrl_id, (uint16)2, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, (uint16)2);
                break;
            case MMICONNECTION_SETTING_DATA_ROAMING_DOMESTIC_ENABLE:
                GUILIST_SetSelectedItem(ctrl_id, (uint16)1, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, (uint16)1);
                break;
            case MMICONNECTION_SETTING_DATA_ROAMING_DISABLE:
            default:
                GUILIST_SetSelectedItem(ctrl_id, (uint16)0, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, (uint16)0);
                break;
            }
            
            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN, FALSE);
        }
        break;
        
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            uint16  cur_selection = 0;
            uint16  total_num = 0;
            
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            total_num = GUILIST_GetTotalItemNum(ctrl_id);

            switch(cur_selection)
            {
            case 2:
                MMIAPICONNECTION_Setting_SetDataRoamingStatus(MMICONNECTION_SETTING_DATA_ROAMING_ENABLE);
                break;
            case 1:
                MMIAPICONNECTION_Setting_SetDataRoamingStatus(MMICONNECTION_SETTING_DATA_ROAMING_DOMESTIC_ENABLE);
                break;
            case 0:
            default:
                MMIAPICONNECTION_Setting_SetDataRoamingStatus(MMICONNECTION_SETTING_DATA_ROAMING_DISABLE);
                break;
            }

            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            MMK_CloseWin(win_id);
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

    return (recode);
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenGPRSDataRoamingSetting(void)
{
    MMK_CreatePubListWin((uint32 *)MMICONNECTION_GPRS_DATA_ROAMING_WIN_TAB, PNULL);
}
#endif


/*****************************************************************************/
//  Description :HandleSetGprsCrWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetGprsCrWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    BOOLEAN                   is_sensor_mute    =  FALSE;
    BOOLEAN          is_sensor_change_wallpaper =  FALSE;
    uint32                   sensor_data        =   0;
#endif
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    int16               i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16  max_items = 0;
    uint16   cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_GPRS_CR_CTRL_ID;
	MMICONNECTION_GPRS_SETTINGS_E grps_status = 0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

        MMK_SetAtvCtrl(win_id,ctrl_id);

        max_items = 2;
        cur_item  = 0;
        //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
        GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        item_t.item_style    = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;

        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
		item_data.item_content[0].item_data.text_buffer.wstr_ptr = L"Always online";
		item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(L"Always online");
        if(!GUILIST_InsertItem(ctrl_id, &item_t, 0)) {
            break;
        }

        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
		item_data.item_content[0].item_data.text_buffer.wstr_ptr = L"When needed";
		item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(L"When needed");
        if(!GUILIST_InsertItem(ctrl_id, &item_t, 1)) {
            break;
        }

		grps_status = MMIAPICONNECTION_GetGPRSStatus();
		if(grps_status == MMICONNECTION_GPRS_ATTACH_ONNEED){
			GUILIST_SetSelectedItem(ctrl_id, 1, TRUE);
		}else{
			GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
		}
        //GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_item);
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
		cur_item = GUILIST_GetCurItemIndex(ctrl_id);
		switch(cur_item)
		{
		case 0:
	        MMIAPICONNECTION_SetGPRSStatus(MMICONNECTION_GPRS_ALWAYS);

	        if (!MMIAPIPDP_PsIsPdpLinkExist())
	        {
	            if (MMICONNECTION_GPRS_ATTACH_ONNEED == MMICONNECTION_GetGPRSModeStatus())
	            {
	                MMIAPIPHONE_SetGPRSDetach();
	            }
	            else
	            {
	                MMIAPIPHONE_SetGPRSAttach();
	            }
	        }
			break;
		case 1:
			MMIAPICONNECTION_SetGPRSStatus(MMICONNECTION_GPRS_ATTACH_ONNEED);

	        if (!MMIAPIPDP_PsIsPdpLinkExist())
	        {
	            if (MMICONNECTION_GPRS_ATTACH_ONNEED == MMICONNECTION_GetGPRSModeStatus())
	            {
	                MMIAPIPHONE_SetGPRSDetach();
	            }
	            else
	            {
	                MMIAPIPHONE_SetGPRSAttach();
	            }
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
//  Description : open phone setting window
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterGprsSettingWin(void)
{
    MMK_CreateWin((uint32 *)MMISET_GPRS_CR_WIN_TAB, PNULL);
}



LOCAL MMISET_VODAFONE_SETTINGS_T cur_vodafone_settings={0};

#define VODAFONE_DEBUG_LOG  SCI_TRACE_LOW

PUBLIC void MMICONNECTION_InitVodafoneSIMcardSettings(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    VODAFONE_DEBUG_LOG("MMICONNECTION_InitVodafoneSIMcardSettings");
    MMINV_READ(MMINV_CONNECTION_VODAFONE_SWITCH, &cur_vodafone_settings, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(&cur_vodafone_settings,0,sizeof(MMISET_VODAFONE_SETTINGS_T));
        MMINV_WRITE(MMINV_CONNECTION_VODAFONE_SWITCH, &cur_vodafone_settings);
    }
    return;
}

LOCAL void SaveVodafoneSIMcardSettings(void)
{
    VODAFONE_DEBUG_LOG("SaveVodafoneSIMcardSettings");
    MMINV_WRITE(MMINV_CONNECTION_VODAFONE_SWITCH, &cur_vodafone_settings);
    return;
}

PUBLIC BOOLEAN MMICONNECTION_IsVodafoneSIMcard(MN_DUAL_SYS_E dual_sys)
{
    VODAFONE_DEBUG_LOG(" MMICONNECTION_IsVodafoneSIMcard %d",dual_sys);
    if (dual_sys>=MN_DUAL_SYS_MAX)
        return FALSE;
    return cur_vodafone_settings.vodafone_sim[dual_sys].is_vodafone_sim;
}

LOCAL BOOLEAN CONNECTION_CheckVodafoneSIMcard(uint16 mcc,uint16 mnc,uint16 spn_len)
{
    if (mcc == 234 && mnc == 15 && spn_len == 0)
    {
        return TRUE;
    }
    return FALSE;
}

PUBLIC BOOLEAN MMICONNECTION_IsUiorderMatched(MN_DUAL_SYS_E dual_sys,uint16 uiOrder)
{
    VODAFONE_DEBUG_LOG("MMICONNECTION_IsUiorderMatched dual_sys %d, uiOrder, %d",dual_sys,uiOrder);
    if((uiOrder ==10)&&(cur_vodafone_settings.vodafone_sim[dual_sys].setting_value==0))
        return TRUE;
    if((uiOrder ==20)&&(cur_vodafone_settings.vodafone_sim[dual_sys].setting_value==1))
        return TRUE;
    return FALSE;
}

LOCAL MN_DUAL_SYS_E GetNextVodafoneSIMcard(MN_DUAL_SYS_E dual_sys)
{
    while (dual_sys<MN_DUAL_SYS_MAX)
    {
        if (cur_vodafone_settings.vodafone_sim[dual_sys].is_vodafone_sim)	//test code,all enabled
        {
            return dual_sys;
        }
        dual_sys++;
    }
    VODAFONE_DEBUG_LOG("GetNextVodafoneSIMcard %d",dual_sys);
    return MN_DUAL_SYS_MAX;
}

PUBLIC BOOLEAN MMICONNECTION_HasVodafoneSIMcard(void)
{
    if (MN_DUAL_SYS_MAX == GetNextVodafoneSIMcard(MN_DUAL_SYS_1))
        return FALSE;
    return TRUE;
}


PUBLIC uint16 GetVodafoneSettings(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys < MN_DUAL_SYS_MAX)
        {
        VODAFONE_DEBUG_LOG("GetVodafoneSettings %d ",cur_vodafone_settings.vodafone_sim[dual_sys].setting_value);
            return cur_vodafone_settings.vodafone_sim[dual_sys].setting_value;
        }
    return 0;
}

LOCAL void MMICONN_SetVodafoneSettings(MN_DUAL_SYS_E dual_sys,uint16 type)
{
    if (dual_sys < MN_DUAL_SYS_MAX)
    {
         VODAFONE_DEBUG_LOG("MMICONN_SetVodafoneSettings dual_sys %d,type %d",dual_sys,type);

         cur_vodafone_settings.vodafone_sim[dual_sys].setting_value=type;
         cur_vodafone_settings.vodafone_sim[dual_sys].is_configured=TRUE;
         cur_vodafone_settings.vodafone_sim[dual_sys].is_new_sim=FALSE;
         SaveVodafoneSIMcardSettings();
    }
    return;
}

LOCAL void MMICONN_ChangeVodafoneAPNSettings(MN_DUAL_SYS_E dual_sys)
{
	MN_PLMN_T carrier_hplmn[MMI_DUAL_SYS_MAX] = {0};
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	VODAFONE_DEBUG_LOG("MMICONN_ChangeVodafoneAPNSettings");
	//clear old settings
	MMINV_READ(MMINV_CARRIER_PLMN, carrier_hplmn, return_value);
	carrier_hplmn[dual_sys].mcc = 0x00;
	carrier_hplmn[dual_sys].mnc = 0x00;
	MMINV_WRITE(MMINV_CARRIER_PLMN, carrier_hplmn);
	MMICONNECTION_AutoAdapting(dual_sys);
    return;
}

/*****************************************************************************/
// Description : open select vodafone win
// Global resource dependence :
// Author: sam.hua
// Note:
/*****************************************	************************************/
PUBLIC void MMICONNECTION_OpenVodafoneWin(MN_DUAL_SYS_E dual_sys)
{
    uint32 sim_num = 0;

    cur_vodafone_settings.cur_sim_index = GetNextVodafoneSIMcard(dual_sys);

    VODAFONE_DEBUG_LOG("MMICONNECTION_OpenVodafoneWin dual_sys %d",dual_sys);

    if (cur_vodafone_settings.cur_sim_index >= MN_DUAL_SYS_MAX)
    {
        VODAFONE_DEBUG_LOG("MMICONNECTION_OpenVodafoneWin, MN_DUAL_SYS_MAX");
        return;
    }

    //MMK_CreatePubListWin((uint32 *)MMICONNECTION_VODAFONE_WIN_TAB, PNULL);
    MMK_CreateWin((uint32 *)MMICONNECTION_VODAFONE_WIN_TAB, PNULL);

    return;
}

LOCAL uint16 ConfigureVodafoneSettingsForNewSIM(void)
{
    MNSIM_SPN_T spn  = {0};
    MN_PLMN_T cur_plmn = {0, 0, 2};
    uint16 i,spn_len = 0;
    uint16 ret = MN_DUAL_SYS_1;

    VODAFONE_DEBUG_LOG("ConfigureVodafoneSettingsForNewSIM");
    cur_vodafone_settings.should_auto_popup = FALSE;
    for (i = MN_DUAL_SYS_1;i<MN_DUAL_SYS_MAX;i++)
    {
        if(cur_vodafone_settings.vodafone_sim[i].is_new_sim)
        {
            //Get SPN/mnc/mcc
            if (MN_RETURN_SUCCESS != MNSIM_GetSPNEx(i,&spn))
                break;

            VODAFONE_DEBUG_LOG("cbk spn_condition:%d,spn:%d", spn.display_condition, spn.id_num[0]);
            //MMIAPICOM_ConvSIMStr2PhoneAlpha((uint8*)spn.id_num,&spn_len, MN_SPN_ID_NUM_LEN);
            //lulifei add for coverity 20151003
#if defined(MMI_MULTI_SIM_SYS_DUAL)
            MMIAPIPHONE_GetCurPLMN(&cur_plmn, i);
#else
           MMIAPIPHONE_GetCurPLMN(&cur_plmn,MN_DUAL_SYS_1);
#endif

//#ifdef WIN32
//            cur_vodafone_settings.vodafone_sim[i].is_vodafone_sim = TRUE;
//#else
            cur_vodafone_settings.vodafone_sim[i].is_vodafone_sim =
            CONNECTION_CheckVodafoneSIMcard(cur_plmn.mcc,cur_plmn.mnc,spn_len);
//#endif
            if(cur_vodafone_settings.vodafone_sim[i].is_vodafone_sim)
            {
                if(cur_vodafone_settings.should_auto_popup==FALSE)  	//save first index
                {
                    ret = i;
                }
                cur_vodafone_settings.should_auto_popup =TRUE;
            }
            else
            {
                cur_vodafone_settings.vodafone_sim[i].is_new_sim = FALSE; //not vodafone sim.don't check it again.
            }
        }
    }
    //Save configure
    SaveVodafoneSIMcardSettings();

return ret;
}


/*****************************************************************************/
// Description : open select vodafone win
// Global resource dependence :
// Author: sam.hua
// Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenVodafoneWinIfNeeded(void)
{
    uint16 first_new_sim_index;

    VODAFONE_DEBUG_LOG("MMIAPISET_OpenVodafoneWinIfNeeded");
    if (!cur_vodafone_settings.new_sim_checked)
    {
        VODAFONE_DEBUG_LOG("MMIAPISET_OpenVodafoneWinIfNeeded. no new_sim_checked");
        return;
    }

    cur_vodafone_settings.new_sim_checked = FALSE;

    first_new_sim_index = ConfigureVodafoneSettingsForNewSIM();
    if(cur_vodafone_settings.should_auto_popup)
    {
        VODAFONE_DEBUG_LOG("MMIAPISET_OpenVodafoneWinIfNeeded should_auto_popup ==TRUE");
        MMICONNECTION_OpenVodafoneWin(first_new_sim_index);
        cur_vodafone_settings.should_auto_popup = FALSE;
    }
    return;

}

PUBLIC void MMIAPISET_ClearVodafoneFlagForNewSIM(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys>=MN_DUAL_SYS_MAX)
    	return;
    VODAFONE_DEBUG_LOG("MMIAPISET_ClearVodafoneFlagForNewSIM");
    cur_vodafone_settings.vodafone_sim[dual_sys].is_configured= FALSE;
    cur_vodafone_settings.vodafone_sim[dual_sys].setting_value= 0;
    cur_vodafone_settings.vodafone_sim[dual_sys].is_new_sim=TRUE;
    cur_vodafone_settings.vodafone_sim[dual_sys].is_vodafone_sim=FALSE;

    cur_vodafone_settings.new_sim_checked = TRUE;
    return;
}


/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVodafoneSwitchWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMICONNECTION_VODAFONE_CTRL_ID;
    uint16 vodafone_sim_id[MN_DUAL_SYS_MAX]={0};
    uint16  vodafone_sim_num = 0;
    MMI_TEXT_ID_T   text_id[] = {TXT_SIM_SEL_SIM1, TXT_SIM_SEL_SIM2};
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            int32           i= 0;
            //            MMI_TEXT_ID_T   text_id[] = {TXT_SET_TIME_10S, TXT_SET_TIME_20S, TXT_CONNECTION_DATA_CARD_SIM3, TXT_CONNECTION_DATA_CARD_SIM4};

            GUILIST_SetMaxItem(ctrl_id, 2, FALSE );
            MMIAPISET_AppendListItemByTextId(TXT_SET_TIME_10S, STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT);
            MMIAPISET_AppendListItemByTextId(TXT_SET_TIME_20S, STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT);


            GUILIST_SetSelectedItem(ctrl_id,
            (uint16)GetNextVodafoneSIMcard(cur_vodafone_settings.cur_sim_index), TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, (uint16)GetNextVodafoneSIMcard(cur_vodafone_settings.cur_sim_index));

            GUIWIN_SetTitleTextId(win_id, text_id[cur_vodafone_settings.cur_sim_index], TRUE);

            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN, FALSE);
        }
        break;

        case MSG_CTL_PENOK:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            uint16  cur_selection = 0;
            uint16  total_num = 0;

            VODAFONE_DEBUG_LOG("HandleVodafoneSwitchWinMsg, Change settings");
            //save current settings
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

            MMICONN_SetVodafoneSettings(cur_vodafone_settings.cur_sim_index,cur_selection);
            MMICONN_ChangeVodafoneAPNSettings(cur_vodafone_settings.cur_sim_index);

            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            cur_vodafone_settings.cur_sim_index=
            GetNextVodafoneSIMcard(cur_vodafone_settings.cur_sim_index+1);

            if (cur_vodafone_settings.cur_sim_index<MN_DUAL_SYS_MAX)
            {
                //Reset WIN Text;
                GUILIST_SetSelectedItem(ctrl_id,
                (uint16)GetNextVodafoneSIMcard(cur_vodafone_settings.cur_sim_index), TRUE);
                GUILIST_SetCurItemIndex(ctrl_id,
                (uint16)GetNextVodafoneSIMcard(cur_vodafone_settings.cur_sim_index));
                GUIWIN_SetTitleTextId(win_id, text_id[cur_vodafone_settings.cur_sim_index], TRUE);
            }
            else
            {
                MMK_CloseWin(win_id);
            }
        }
        break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        case MSG_APP_RED:
            VODAFONE_DEBUG_LOG("HandleVodafoneSwitchWinMsg, Don't change settings");
            cur_vodafone_settings.cur_sim_index=
            GetNextVodafoneSIMcard(cur_vodafone_settings.cur_sim_index+1);

            if (cur_vodafone_settings.cur_sim_index<MN_DUAL_SYS_MAX)
            {
                //Reset WIN Text;
                GUILIST_SetSelectedItem(ctrl_id,
                (uint16)GetNextVodafoneSIMcard(cur_vodafone_settings.cur_sim_index), TRUE);
                GUILIST_SetCurItemIndex(ctrl_id,
                (uint16)GetNextVodafoneSIMcard(cur_vodafone_settings.cur_sim_index));
                GUIWIN_SetTitleTextId(win_id, text_id[cur_vodafone_settings.cur_sim_index], TRUE);
            }
            else
            {
                MMK_CloseWin(win_id);
            }
        break;

        default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return (recode);
}

/*Edit by script, ignore 29 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
//lulifei add for UI spec
LOCAL MMI_RESULT_E HandleConnEmptyPortWarningWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param) {
    MMI_RESULT_E         result = MMI_RESULT_TRUE;

    switch(msg_id) {
	case MSG_OPEN_WINDOW:
        //GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, LEFT_BUTTON, TRUE);
        //GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_CANCEL, RIGHT_BUTTON, TRUE);
        break;
        case MSG_APP_OK:
        case MSG_CTL_OK:
			break;
        case MSG_APP_WEB: {
            MMK_CloseWin(win_id);
        }
        break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            //MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }
    return result;
}

//lulifei add for UI spec
LOCAL MMI_RESULT_E HandleConnInvaildPortWarningWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param) {
    MMI_RESULT_E         result = MMI_RESULT_TRUE;

    switch(msg_id) {
        case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, TRUE);
        break;
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB: {
            MMK_CloseWin(win_id);
        }
        break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }
    return result;
}

/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:lifei.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVodafoneSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMICONNECTION_VODAFONE_SETTINGS_CTRL_ID;
    wchar vodafone_text[32] = {0};
    uint16 vodafone_select = 0;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            int32           i= 0;
            GUILIST_SetMaxItem(ctrl_id, 2, FALSE);
            MMIAPISET_AppendListItemByTextId(STR_VODAFONE_CONTRACT_EXT01, PNULL, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT);
            MMIAPISET_AppendListItemByTextId(STR_VODAFONE_PAYG_EXT01, PNULL, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT);
            
            vodafone_select = MMIAPIPHONE_GetVodafoneSimSelectResult(MN_DUAL_SYS_1, vodafone_text);
            GUILIST_SetSelectedItem(ctrl_id,vodafone_select -1, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, vodafone_select -1);
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;

        case MSG_CTL_PENOK:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        	break;
        	
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                uint16  cur_selection = 0;
                MMI_STRING_T name_string = {0};
                MMIPHONE_HANDLE_VODAFONE_SELECT_T vodafone_info = {0};

                cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
                switch (cur_selection)
                {
                    case CONNECTION_SETTING_CONTRACT :
                        //dong.chunguang add for reset default apn id when UK change
                        if (vodafone_select != 1)
                        {
                            MMICONNECTION_SetApnDefaultId(MN_DUAL_SYS_1, 0);//reset default apn id
                        }
                        //end
                        MMI_GetLabelTextByLang(STR_VODAFONE_CONTRACT_EXT01, &name_string);
                        vodafone_info.select_sim1_num = 1;
                        MMIAPICOM_Wstrncpy(vodafone_info.vodafone_sim1_text, name_string.wstr_ptr, name_string.wstr_len);
                        MMINV_WRITE(MMINV_PHONE_FIRST_VODAFONE_SIM, &vodafone_info);
                        SetVodafoneApn(MN_DUAL_SYS_1,CONNECTION_SETTING_CONTRACT);
                        SCI_TRACE_LOW("[setting] vodafone sim1 select result: Vodafone UI Contract");
                        
                        break;
            		  
                    case CONNECTION_SETTING_PAYG :
                        //dong.chunguang add for reset default apn id when UK change
                        if(vodafone_select!=2)
                        {
                        	MMICONNECTION_SetApnDefaultId(MN_DUAL_SYS_1, 0);//reset default apn id
                        }
                        //end
                        
                        MMI_GetLabelTextByLang(STR_VODAFONE_PAYG_EXT01, &name_string);
                        vodafone_info.select_sim1_num = 2;
                        MMIAPICOM_Wstrncpy(vodafone_info.vodafone_sim1_text, name_string.wstr_ptr, name_string.wstr_len);
                        MMINV_WRITE(MMINV_PHONE_FIRST_VODAFONE_SIM, &vodafone_info);
                        
                        SetVodafoneApn(MN_DUAL_SYS_1, CONNECTION_SETTING_PAYG);
                        SCI_TRACE_LOW("[settings] vodafone sim1 select result: Vodafone UI PAYG");
                        
                        break;
                        
                    default:
                    	// do nothing
                        break;
                }
                
                MMK_CloseWin(win_id);
                break;
        	}
        	
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        case MSG_APP_RED:
            MMK_CloseWin(win_id);
            break;
        	
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : handleConfirmationDialog
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E handleConfirmationDialog(
                                                MMI_WIN_ID_T    win_id, // ���ڵ�ID
                                                MMI_MESSAGE_ID_E   msg_id, // ���ڵ��ڲ���ϢID
                                                DPARAM             param   // ��Ӧ��Ϣ�Ĳ���
                                                )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;
    uint16			list_index = 0;
    uint16			apn_index = 0;
    uint32			i = 0;
    MN_DUAL_SYS_E   dual_sys = s_connection_handling_sim_id;
	uint8			default_id = 0;
    uint16			default_list_index = 0;
    uint16			default_apn_index = 0;
    
    SCI_TRACE_LOW("[szp]:handleConfirmationDialog total = %d, invisible = %d",s_connection_setting_info.settinglink_num[dual_sys], s_connection_setting_info.invisible_apn_num[dual_sys]);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, TRUE);
		break;
    case MSG_APP_OK:
        break;
    case MSG_APP_WEB:
        list_index = GUILIST_GetCurItemIndex(MMICONNECTION_LIST_SETTING_CTRL_ID);
        apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, list_index);
		//change the default account
		default_id = MMIAPICONNECTION_GetAccountDefaultIndex(dual_sys,0);
		if (apn_index < default_id)
		{
            default_list_index = MMIAPICONNECTION_GetListIndexByAPNIndex(dual_sys, default_id);
            default_apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, default_list_index-1);
			MMICONNECTION_SetApnDefaultId(dual_sys, default_apn_index);
		}
		else if (apn_index == MMIAPICONNECTION_GetAccountDefaultIndex(dual_sys,0))
		{
		    default_apn_index = MMIAPICONNECTION_GetAPNIndexByListIndex(dual_sys, 0);
			MMICONNECTION_SetApnDefaultId(dual_sys, default_apn_index);
		}
		if ((apn_index < s_connection_setting_info.settinglink_num[dual_sys]) 
		    && (0 < (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys])))
        {
            for (i = (uint32)apn_index; i < (uint32)(s_connection_setting_info.settinglink_num[dual_sys]-1); i++)
            {
                SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][i], &s_connection_setting_info.setting_detail[dual_sys][i+1], sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            }
            SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][s_connection_setting_info.settinglink_num[dual_sys] - 1], 0, sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys]--;
        }
        else if ((apn_index == s_connection_setting_info.settinglink_num[dual_sys])
                 && (0 < (s_connection_setting_info.settinglink_num[dual_sys] - s_connection_setting_info.invisible_apn_num[dual_sys])))
        {
            SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][s_connection_setting_info.settinglink_num[dual_sys] - 1], 0, sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys]--;
        }
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO,&s_connection_setting_info);
        
        //����Ӧ��ѡ�е�ITEM
        ChangeAppSelectedIndex(dual_sys, apn_index, MMICONNECTION_NOTIFY_EVENT_DEL_ITEM);
        GUILIST_RemoveItem(MMICONNECTION_LIST_SETTING_CTRL_ID, list_index);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        break;
    }

    result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
    
    return (result);
}

/*****************************************************************************/
// 	Description : get default APN
//	Global resource dependence : none
//  Author: weipeng.wang
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICONNECTION_GetDefaultAPN(MN_DUAL_SYS_E dual_sys)
{
    MMI_STRING_T temp_str = {0};
    uint8 default_apn = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    if(dual_sys == MN_DUAL_SYS_1)
    {
        if (MMICONNECTION_IsVodafoneVersion())
        {
            MMINV_READ(MMINV_CONNECTION_VODAFONE1_APN_DEFAULT, &default_apn, return_value);
        }
        else
        {
            MMINV_READ(MMINV_CONNECTION_SIM1_APN_DEFAULT, &default_apn, return_value);
        }
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else
    {
        if (MMICONNECTION_IsVodafoneVersion())
        {
            MMINV_READ(MMINV_CONNECTION_VODAFONE2_APN_DEFAULT, &default_apn, return_value);
        }
        else
        {
            MMINV_READ(MMINV_CONNECTION_SIM2_APN_DEFAULT, &default_apn, return_value);
        }
    }
#endif
    if (default_apn >= s_connection_setting_info.settinglink_num[dual_sys])
    {
        default_apn = 0;
        MMICONNECTION_SetApnDefaultId(dual_sys, default_apn);
    }
    
    return default_apn;
}

/*****************************************************************************/
//  Description : MMICONNECTION_SetApnDefaultId
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_SetApnDefaultId(MN_DUAL_SYS_E dual_sys,uint8 cur_index)
{
	if (dual_sys == MN_DUAL_SYS_1)
	{
		if (MMICONNECTION_IsVodafoneVersion())
		{
			MMINV_WRITE(MMINV_CONNECTION_VODAFONE1_APN_DEFAULT, &cur_index);
		}
		else
		{
			MMINV_WRITE(MMINV_CONNECTION_SIM1_APN_DEFAULT, &cur_index);
		}
	}
	else
	{
		if (MMICONNECTION_IsVodafoneVersion())
		{
			MMINV_WRITE(MMINV_CONNECTION_VODAFONE2_APN_DEFAULT, &cur_index);
		}
		else
		{
			MMINV_WRITE(MMINV_CONNECTION_SIM2_APN_DEFAULT, &cur_index);
		}
	}
	
	return;
}
/*****************************************************************************/
// 	Description : MMICONNECTION_IsVodafoneVersion
//	Global resource dependence :
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMICONNECTION_IsVodafoneVersion(void)
{
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;
    MMIPHONE_HANDLE_VODAFONE_SELECT_T   vodafone_info = {0};
    MMINV_READ(MMINV_PHONE_FIRST_VODAFONE_SIM, &vodafone_info, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        return FALSE;
    }
    
	if (vodafone_info.select_sim1_num == 0 && vodafone_info.select_sim2_num == 0)
	{
		return FALSE;
	}
    return TRUE;
}

#ifdef MMI_GPRS_SUPPORT
//lulifei add for vodafone
LOCAL MMICONNECTION_SETTING_T  vodafone_contract_connect_info = {0};
LOCAL MMICONNECTION_SETTING_T  vodafone_payg_connect_info = {0};

LOCAL void InitVodafoneApn() 
{
    uint8 i = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint16 reset_list_num = MMICONNECTION_MAX_SETTING_NUM;

    SCI_TRACE_LOW("InitVodafoneApn, first-s_connection_setting_info = %d", s_connection_setting_info.settinglink_num[dual_sys]);
    
    MMINV_READ(MMINV_CONNECTION_SETTING_CONTRACT_INFO, &vodafone_contract_connect_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        for (dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys ++)
        {
            SCI_MEMSET(vodafone_contract_connect_info.setting_detail[dual_sys], 0, (sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)*reset_list_num));
            vodafone_contract_connect_info.settinglink_num[dual_sys] = 0;
            for(i = 0; i < 3; i++)
            {
                SCI_MEMCPY(&vodafone_contract_connect_info.setting_detail[dual_sys][i], &s_connection_setting_info.setting_detail[dual_sys][i], sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                vodafone_contract_connect_info.settinglink_num[dual_sys]++;
        	}
        }
        
        MMINV_WRITE(MMINV_CONNECTION_SETTING_CONTRACT_INFO, &vodafone_contract_connect_info);
    }

    MMINV_READ(MMINV_CONNECTION_SETTING_PAYG_INFO, &vodafone_payg_connect_info, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        for (dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys ++)
        {
            SCI_MEMSET(vodafone_payg_connect_info.setting_detail[dual_sys], 0, (sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)*reset_list_num));
            vodafone_payg_connect_info.settinglink_num[dual_sys] =0;
            for (i = 3; i < 6; i++)
            {
                SCI_MEMCPY(&vodafone_payg_connect_info.setting_detail[dual_sys][i-3], &s_connection_setting_info.setting_detail[dual_sys][i], sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                vodafone_payg_connect_info.settinglink_num[dual_sys]++;
    	    }
     	}
        MMINV_WRITE(MMINV_CONNECTION_SETTING_PAYG_INFO, &vodafone_payg_connect_info);
    }
    
    return;
}

//lulifei add for vodafone
LOCAL void InitVodafoneApnList(MN_DUAL_SYS_E dual_sys,CONNECTION_SETTING_VODAFONE_E vodafone_type) 
{
    uint8 i = 0;
    uint8 same_or_not = 0;
    
    SCI_TRACE_LOW("InitVodafoneApnList,first init");
    
    for (i = 0; i < 3; i++)
    {
        if (0 == memcmp(&vodafone_payg_connect_info.setting_detail[dual_sys][i], &s_connection_setting_info.setting_detail[dual_sys][i+3], sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)))
        {
            SCI_TRACE_LOW("InitVodafoneApnList,init here");
            same_or_not++;
        }
    }
    if (same_or_not > 2)
    {
        //init the apn list when first into connectivity
        if(vodafone_type == CONNECTION_SETTING_CONTRACT)
        {
            for (i = 3; i < 6; i++)
            {
                SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][i], 0, (sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)));
                s_connection_setting_info.settinglink_num[dual_sys]--;
            }
        }
        
        if (vodafone_type == CONNECTION_SETTING_PAYG)
        {
            for (i = 0; i < 3; i++)
            {
                SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][i], &s_connection_setting_info.setting_detail[dual_sys][i+3], sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
                SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys][i+3], 0, (sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)));
                s_connection_setting_info.settinglink_num[dual_sys]--;
            }
        }
        
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);
    }
    
    return;
}

PUBLIC void SetVodafoneApn(MN_DUAL_SYS_E dual_sys,CONNECTION_SETTING_VODAFONE_E vodafone_type) 
{
    uint8 i = 0;
    uint16 reset_list_num = MMICONNECTION_MAX_SETTING_NUM;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    //set the real apn list when choose in the vodafone switch
    SCI_TRACE_LOW("SetVodafoneApn,storage-s_connection_setting_info_num =%d", s_connection_setting_info.settinglink_num[dual_sys]);
    
    if(vodafone_type == CONNECTION_SETTING_CONTRACT)
    {
    	//copy the connection list to payg nv for saving
        SCI_MEMSET(&vodafone_payg_connect_info.setting_detail[dual_sys], 0, (sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)*reset_list_num));
        vodafone_payg_connect_info.settinglink_num[dual_sys] = 0;
        
        for (i = 0; i <= s_connection_setting_info.settinglink_num[dual_sys] - 1; i++)
        {
            SCI_MEMCPY(&vodafone_payg_connect_info.setting_detail[dual_sys][i], &s_connection_setting_info.setting_detail[dual_sys][i], sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            vodafone_payg_connect_info.settinglink_num[dual_sys] ++;
            
            SCI_TRACE_LOW("SetVodafoneApn,storage-vodafone_payg_connect_info_name=%s", vodafone_payg_connect_info.setting_detail[dual_sys][i].name);
        }
        
        MMINV_WRITE(MMINV_CONNECTION_SETTING_PAYG_INFO, &vodafone_payg_connect_info);
        
        //change the apn list with contract
        SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys], 0, (sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)*reset_list_num));
        s_connection_setting_info.settinglink_num[dual_sys] = 0;
        
        for (i = 0; i <= vodafone_contract_connect_info.settinglink_num[dual_sys] - 1; i++)
        {
            SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][i], &vodafone_contract_connect_info.setting_detail[dual_sys][i], sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys] ++;
            
            SCI_TRACE_LOW("SetVodafoneApn, finish-s_connection_setting_info_name = %s", s_connection_setting_info.setting_detail[dual_sys][i].name);
        }
        
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);
   }
   
   if (vodafone_type == CONNECTION_SETTING_PAYG)
   {
        //copy the connection list to contract nv for saving
        SCI_MEMSET(&vodafone_contract_connect_info.setting_detail[dual_sys], 0, (sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)*reset_list_num));
        vodafone_contract_connect_info.settinglink_num[dual_sys] = 0;
        
        for (i = 0; i <= s_connection_setting_info.settinglink_num[dual_sys] - 1; i++)
        {
            SCI_MEMCPY(&vodafone_contract_connect_info.setting_detail[dual_sys][i], &s_connection_setting_info.setting_detail[dual_sys][i], sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            vodafone_contract_connect_info.settinglink_num[dual_sys]++;
            
            SCI_TRACE_LOW("SetVodafoneApn, storage-vodafone_contract_connect_info_name = %s", vodafone_contract_connect_info.setting_detail[dual_sys][i].name);
        }
        
        MMINV_WRITE(MMINV_CONNECTION_SETTING_CONTRACT_INFO, &vodafone_contract_connect_info);
        
        //change the apn list with contract
        //maxlistnum = MAX(vodafone_payg_connect_info.settinglink_num[dual_sys],s_connection_setting_info.settinglink_num[dual_sys]);
        SCI_MEMSET(&s_connection_setting_info.setting_detail[dual_sys], 0, (sizeof(MMICONNECTION_LINKSETTING_DETAIL_T)*reset_list_num));
        s_connection_setting_info.settinglink_num[dual_sys] = 0;
        
        for (i = 0; i <= vodafone_payg_connect_info.settinglink_num[dual_sys]-1; i++)
        {
            SCI_MEMCPY(&s_connection_setting_info.setting_detail[dual_sys][i], &vodafone_payg_connect_info.setting_detail[dual_sys][i], sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            s_connection_setting_info.settinglink_num[dual_sys]++;
            
            SCI_TRACE_LOW("SetVodafoneApn,finish-s_connection_setting_info_name=%s", s_connection_setting_info.setting_detail[dual_sys][i].name);
        }
        
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &s_connection_setting_info);
    }
}
#endif

PUBLIC void MMICONNECTION_UpdatePINToggle()
{
	MMI_CTRL_ID_T ctrl_id = MMICONNECTION_NETWORK_SETTING_CTRL_ID;
	uint32 menu_id = 0;
	uint16 i = 0,total_item = 0;

	total_item = CTRLLIST_GetTotalItemNum(ctrl_id);
	
	for(i = 0;i<total_item;i++)
	{
		GUILIST_GetItemData(ctrl_id, i, &menu_id);
		if( ID_CONNECTION_NETWORK_SETTING_PIN_REQUEST == menu_id)
		{
			CTRLLIST_SetToggleState(ctrl_id, i, (MNSIM_IsPin1EnableEx(MN_DUAL_SYS_1)) ? TRUE : FALSE);
			break;
		}
	}

	return;
}
/*****************************************************************************/
// Description : Get if is Orange Portocol Type
// Global resource dependence:
// Author:dong.chunguang
// Note:
/*****************************************************************************/
#ifdef IPVERSION_SUPPORT_V4_V6
LOCAL uint8  MMICONN_GetIP_PortocolType(void)
{
    uint8     portocoltype  =  MMICONNECTION_IP_V4V6;
    uint16    nv_len = 0;
    nv_len = NVITEM_GetLength(SETTINGS_NV_IP_PORTOCOL_SELECTION);
    EFS_NvitemRead(SETTINGS_NV_IP_PORTOCOL_SELECTION, nv_len, &portocoltype);
    return portocoltype;
}
#endif
/*****************************************************************************/
// Description : Get if is Orange Ploand card
// Global resource dependence:
// Author:dong.chunguang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMICONNECTION_IsPolandSimCard(MN_DUAL_SYS_E   dual_sys)
{
    BOOLEAN                     isPolandSimCard = FALSE;
    MN_PLMN_T sim_hplmn = {0};
    //get single card mnc value
    //MMIAPIPHONE_GetCurServiceStatus(dual_sys,&service_status);
    MMIAPIPHONE_GetSimHPLMN(&sim_hplmn, dual_sys);
    if((260 == sim_hplmn.mcc) &&
        (5 == sim_hplmn.mnc ))
    {
        isPolandSimCard = TRUE;
    }
    else
    {
        isPolandSimCard = FALSE;
    }
    return(isPolandSimCard);
}

/*****************************************************************************/
// Description : Get if is Support domestic roaming menu
// Global resource dependence:
// Author:cheney.wu
// Note:
/*****************************************************************************/
PUBLIC uint8 MMICONNECTION_IsSupportDomesticRoaming(void)
{
    uint8 is_support_domestic_roaming = FALSE;
    uint16    nv_len = 0;
    MN_PLMN_T cur_plmn = {0};
    
    nv_len = NVITEM_GetLength(SETTINGS_NV_ENABLE_DOMESTIC_ROAMING);
    EFS_NvitemRead(SETTINGS_NV_ENABLE_DOMESTIC_ROAMING, nv_len, &is_support_domestic_roaming);
    
    SCI_TRACE_LOW("MMICONNECTION_IsSupportDomesticRoaming Entry");
    if(is_support_domestic_roaming)
    {
        MMIAPIPHONE_GetSimHPLMN(&cur_plmn,MMIAPISET_GetActiveSim());
        if(0 == cur_plmn.mcc)
        {
            SCI_TRACE_LOW("MMICONNECTION_IsSupportDomesticRoaming: SIM not ready!!!!");
            return 0xFF;
        }
        if(260 == cur_plmn.mcc)
        {
            if(06 == cur_plmn.mnc)
            {
                SCI_TRACE_LOW("MMICONNECTION_IsSupportDomesticRoaming: Support!!!!");
                return TRUE;
            }
            SCI_TRACE_LOW("MMICONNECTION_IsSupportDomesticRoaming: NOT Support!!!!");
            return FALSE;
        }
        
        SCI_TRACE_LOW("MMICONNECTION_IsSupportDomesticRoaming: Support!!!!");
        return TRUE;
    }
    
    SCI_TRACE_LOW("MMICONNECTION_IsSupportDomesticRoaming: NOT Support!!!!");
    return FALSE;
}
