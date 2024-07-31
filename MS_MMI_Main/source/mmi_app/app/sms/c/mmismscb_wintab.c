/******************************************************************************
**
**
**
**  С���㲥ģ��
**
**
**  
**
**
**
*******************************************************************************/
#include "mmi_app_sms_trc.h"
#if defined(MMI_SMSCB_SUPPORT)

#define _MMISMS_CB_WINTAB_C_

/**--------------------------------------------------------------------------*
**                                                                          *
**                         ������ͷ�ļ�                                     *
**                                                                          *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "guilistbox.h"
#include "guilabel.h"
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
#include "mmismscb_wintab.h"
//#include "mmismscb_control.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_id.h"
#include "mmisms_nv.h"
#include "mmisms_menutable.h"
#include "mmi_appmsg.h"
#include "gui_ucs2b_converter.h"
#include "guiedit.h"
#ifdef MMI_PDA_SUPPORT
#include "guictrl_api.h"
#include "guibutton.h"
#endif
//#include "guires.h"
#include "guiform.h"
#include "guitab.h"
#include "guirichtext.h"
#ifdef SCREENSAVER_SUPPORT
#include "mmiss_export.h"
#endif

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
#include "mmiidle_statusbar.h"
#endif

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmi_pubmenuwin.h"
#include "mmisms_commonui.h"

#ifdef MMI_ETWS_SUPPORT
#include "mmismscb_control.h"
#include "mmicc_export.h"
#include "mmisms_chatreadwin.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#endif

#ifdef MMI_ETWS_SUPPORT
#include "mmicc_id.h"
#include "mmialarm_id.h"
#include "nvitem.h"
#include "variant_config_nv.h"
#include "mmismscb_export.h"
#include "mmicom_banner.h"
#endif

#ifdef WIN32
//#define CB_TEST_IT
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
/******************************************************************************************************************/
//
//
//                        С���㲥ģ���ڲ��궨��
//
//
/******************************************************************************************************************/
#define CB_CHANNEL_INDEX_MAX_LEN              5                                   //С���㲥�ŵ�������󳤶�
#define CB_CHANNEL_NAME_MAX_LEN               10                                  //С���㲥�ŵ�������󳤶�
#ifndef MMI_ETWS_SUPPORT
#define CB_MSG_TIME_MAX_LEN                   24                                  //С���㲥��Ϣ����ʱ����󳤶�
#endif
#define MMI_PER_INSTALL_PARAM 50
#define MCC_MALAYSIA_ZONE 502  //Malaysia MCC
//#define CB_CHANNEL_50_EXIST TRUE
#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
#define CB_CELL_INFO_INDEX_ID        50
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

const MMI_TEXT_ID_T g_cb_lang_text_arr[MMI_MAX_CB_LANGUAGE_NUM]={                                             //С���㲥�����б�����ID�ž�̬����
    STXT_LANGUAGE_CHINESE_CN,
    UNISOC_LANGUAGE_ENGLISH,
    UNISOC_LANGUAGE_GERMAN,
    UNISOC_LANGUAGE_ITALIAN,
    STXT_LANGUAGE_FRENCH,
    STXT_LANGUAGE_SPANISH,
    UNISOC_LANGUAGE_DUTCH,
    UNISOC_LANGUAGE_SWEDISH,
    STXT_LANGUAGE_PORTUGUESE,	
    UNISOC_LANGUAGE_DANISH,	
    UNISOC_LANGUAGE_FINNISH,	
    UNISOC_LANGUAGE_NORWEGIAN,	
    STXT_LANGUAGE_GREEK,	
    STXT_LANGUAGE_TURKISH,	
    UNISOC_LANGUAGE_HUNGARIAN,	
    UNISOC_LANGUAGE_POLISH,	
    STXT_LANGUAGE_CZECH,	
    STXT_LANGUAGE_HEBREW,
    STXT_LANGUAGE_ARABIC,
    STXT_LANGUAGE_RUSSIAN,	
    STXT_LANGUAGE_ICELANDIC,
#if defined MULTI_SIM_SYS_SINGLE || defined MULTI_SIM_SYS_DUAL    
    STR_CB_LANG_UNSPECIFIED,
#endif    
    STR_CB_LANG_ALL
};

const MN_SMSCB_LANGUAGE_ID_E g_cb_lang_id_arr[MMI_MAX_CB_LANGUAGE_NUM]={                               //С���㲥�����б�����ID�ž�̬����
    MN_SMSCB_CHINESE_ID,
    MN_SMSCB_ENGLISH_ID,
    MN_SMSCB_GERMAN_ID,
    MN_SMSCB_ITALIAN_ID,
    MN_SMSCB_FRENCH_ID,
    MN_SMSCB_SPANISH_ID,
    MN_SMSCB_DUTCH_ID,
    MN_SMSCB_SWEDISH_ID,
    MN_SMSCB_PORTUGUESE_ID,
    MN_SMSCB_DANISH_ID,
    MN_SMSCB_FINNISH_ID,
    MN_SMSCB_NORWEGIAN_ID,
    MN_SMSCB_GREEK_ID,
    MN_SMSCB_TURKISH_ID,
    MN_SMSCB_HUNGARIAN_ID,
    MN_SMSCB_POLISH_ID,
    MN_SMSCB_CZECH_ID,
    MN_SMSCB_HEBREW_ID,
    MN_SMSCB_ARABIC_ID,
    MN_SMSCB_RUSSIAN_ID,
    MN_SMSCB_ICELANDIC_ID,
#if defined MULTI_SIM_SYS_SINGLE || defined MULTI_SIM_SYS_DUAL        
    MN_SMSCB_LANGUAGE_UNSPECIFIED,
#endif    
    MN_SMSCB_LANGUAGE_NUM
};
#if 0
wchar  lang_name[MMI_MAX_CB_LANGUAGE_NUM][11] = 
{
    {0x4E2D,0x6587},// Chinese
    {0x0045,0x006E,0x0067,0x006C,0x0069,0x0073,0x0068},//English
    {0x0044,0x0065,0x0075,0x0074,0x0073,0x0063,0x0068},// German
    {0x0049,0x0074,0x0061,0x006C,0x0069,0x0061,0x006E,0x006F},//Italian
    {0x0046,0x0072,0x0061,0x006E,0x00E7,0x0061,0x0069,0x0073},//French
    {0x0045,0x0073,0x0070,0x0061,0x00F1,0x006F,0x006C},//Spanish
    {0x004E,0x0065,0x0064,0x0065,0x0072,0x006C,0x0061,0x006E,0x0064,0x0073},//Dutch
    {0x0053,0x0076,0x0065,0x006E,0x0073,0x006B,0x0061},//Swedish
    {0x0050,0x006F,0x0072,0x0074,0x0075,0x0067,0x0075,0x00EA,0x0073}, //Portuguese
    {0x0044,0x0061,0x006E,0x0073,0x006B},//Danish
    {0x0053,0x0075,0x006F,0x006D,0x0069},//Finnish
    {0x004E,0x006F,0x0072,0x0073,0x006B},//Norwegian
    {0x0395,0x03BB,0x03BB,0x03B7,0x03BD,0x03B9,0x03BA,0x03AC},//Greek
    {0x0054,0x00FC,0x0072,0x006B,0x00E7,0x0065},//Turkish
    {0x004D,0x0061,0x0067,0x0079,0x0061,0x0072},//Hungarian
    {0x0050,0x006F,0x006C,0x0073,0x006B,0x0069},//Polish
    {0x010C,0x0065,0x0161,0x0074,0x0069,0x006E,0x0061},//Czech
    {0x05E2,0x05D1,0x05E8,0x05D9,0x05EA},//Hebrew
    {0x0627,0xFEDF,0xFECC,0xFEAD,0xFE91,0xFEF4,0xFE94},//Arabic
    {0x0420,0x0443,0x0441,0x0441,0x043A,0x0438,0x0439},//Russian
    {0x00CD,0x0073,0x006C,0x0065,0x006E,0x0073,0x006B,0x0061}//Icelandic
};
#endif

LOCAL BOOLEAN is_system_factory = FALSE;


#ifndef MMI_GUI_STYLE_TYPICAL
typedef enum
{
    MMISMS_PDA_SMS_CB_SETTINGS_INIT,    
    MMISMS_PDA_SMS_CB_SETTINGS_SWITCH, 
    MMISMS_PDA_SMS_CB_SETTINGS_50_CHANNEL,
    MMISMS_PDA_SMS_CB_SETTINGS_LANG,
    MMISMS_PDA_SMS_CB_SETTINGS_CHANNEL,    
    MMISMS_PDA_SMS_CB_SETTINGS_READ_ITEM,//added by Tong Yongli
#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
    MMISMS_PDA_SMS_CB_SETTINGS_CELLINFO,
#endif
    MMISMS_PDA_SMS_CB_SETTINGS_MAX
    
}MMISMS_PDA_CB_SETTINGS_TYPE_E;
#endif
#if 0
typedef struct
{
    LOCAL MN_DUAL_SYS_E s_dual_card_sys;
} MMISMS_MSG_LIST_WIN_PARAM_T;
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//LOCAL MMISMS_CB_MESSAGE_T s_cb_msg = {0};            //С���㲥��Ϣ����
//LOCAL MN_DUAL_SYS_E s_dual_card_sys = MN_DUAL_SYS_1;
#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
LOCAL BOOLEAN s_cell_info_flag[MMI_DUAL_SYS_MAX] = {0};
#endif

#if defined(CB_TEST_CHILE)
LOCAL s_set_mcc = CB_CHILE_MCC;
#elif defined(CB_TEST_CO)
LOCAL s_set_mcc =  CB_CO_MCC ;
#elif defined(CB_TEST_EC)
LOCAL s_set_mcc =  CB_EC_MCC ;
#elif defined(CB_TEST_GR)
LOCAL s_set_mcc =  CB_GR_MCC ;
#elif defined(CB_TEST_IL)
LOCAL s_set_mcc =  CB_IL_MCC;
#elif defined(CB_TEST_IT)
LOCAL s_set_mcc =  CB_IT_MCC;
#elif defined(CB_TEST_MX)
LOCAL s_set_mcc =  CB_MX_MCC ;
#elif defined(CB_TEST_NZ)
LOCAL s_set_mcc =  CB_NZ_MCC ;
#elif defined(CB_TEST_PE)
LOCAL s_set_mcc = CB_PE_MCC;
#elif defined(CB_TEST_PR)
LOCAL s_set_mcc  = CB_PR_MCC;
#elif defined(CB_TEST_PWS)
LOCAL s_set_mcc  = CB_PWS_MCC;
#elif defined(CB_TEST_RO)
LOCAL s_set_mcc =  CB_RO_MCC ;
#elif defined(CB_TEST_UAE)
LOCAL s_set_mcc =  CB_UAE_MCC;
#elif defined(CB_TEST_VE)
LOCAL s_set_mcc =  CB_VE_MCC;
#elif defined(CB_TEST_NONE)
LOCAL s_set_mcc =  CB_EMPTY_MCC;
#else
LOCAL s_set_mcc =  0;
#endif

#ifdef MMI_ETWS_SUPPORT
LOCAL MMISMS_CB_CHANNEL_T s_nv_channel_list[MN_DUAL_SYS_MAX] = {0};
LOCAL MMISMS_CB_CHANNEL_T s_save_to_nv_channel_list = {0};
LOCAL uint32  s_visible_channel_num = 0;
LOCAL uint16 s_cur_channel_index = 0;
LOCAL BOOLEAN s_is_it_pop_toast = FALSE;
LOCAL BOOLEAN s_is_etws_spec_changed = FALSE;
LOCAL MMISMS_CB_MESSAGE_ARRAY_T *s_cb_storage_message_list_ptr = PNULL;
BOOLEAN s_mmismscb_etws_message_is_need_redisplay = FALSE;
LOCAL MMI_STRING_T etws_title_str = {0};
LOCAL BOOLEAN is_set_mcc = FALSE;
#endif

LOCAL uint16 s_list_index = 0;
/******************************************************************************************************************/
//
//
//                        С���㲥ģ�鴰�ڴ���������
//
//
/******************************************************************************************************************/

/******************************************************************************************************************/
//      
//    ����/�رչ㲥ѡ��ڴ�����
//  
/******************************************************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBStartUpWindow(
                                            MMI_WIN_ID_T    win_id,     // STARTUP NORMAL���ڵ�ID
                                            MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                            DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                            );

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCB50ChannelWindow
//  ��������:        С���㲥50 Ƶ������/�رմ��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleSmsCB50ChannelWindow(
                                            MMI_WIN_ID_T    win_id,     // STARTUP NORMAL���ڵ�ID
                                            MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                            DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                            );

/****************************************************************************************************************/
//      
//    ����ѡ���б��ں���
//  
/****************************************************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBLanguageListWindow(
                                                 MMI_WIN_ID_T    win_id,     // STARTUP NORMAL���ڵ�ID
                                                 MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                                 DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                                 );

/*****************************************************************************************************************/
//      
//    �㲥�ŵ�ѡ���б��ڴ�����
//  
/*****************************************************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBChannelListWindow(
                                                MMI_WIN_ID_T    win_id,     // STARTUP NORMAL���ڵ�ID
                                                MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                                DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                                );

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBChannelOptWindowInit
//  ��������:        �㲥�ŵ������б�˵����ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void HandleSmsCBChannelOptWindowInit( MMIPUB_MENU_NOTIFY_T *param );

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBChannelOptWindowOk
//  ��������:        �㲥�ŵ������б�˵����ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void HandleSmsCBChannelOptWindowOk( MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : ���/�༭�㲥�ŵ����ڴ�����
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSCBChannelEditWindow(
                                                MMI_WIN_ID_T        win_id, //IN: ���ڵ�ID
                                                MMI_MESSAGE_ID_E     msg_id, //IN: ���ڵ��ڲ���ϢID
                                                DPARAM                 param    //IN: ��Ӧ��Ϣ�Ĳ���
                                                );

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         SwitchCBChannel
//  ��������:        ����ָ���ŵ�
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void SwitchCBChannel(uint16 index,  uint16 channel_index);

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         DeleteCBChannel
//  ��������:        ɾ��ָ���ŵ�
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void DeleteCBChannel(uint16 index);

/*****************************************************************************/
//     Description : get channel information
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetChannelInfo(BOOLEAN is_add);

/****************************************************************************************************************/
//      
//    �ŵ�����鿴���ں���
//  
/****************************************************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBChannelCheckWindow(
                                                 MMI_WIN_ID_T    win_id,     // STARTUP NORMAL���ڵ�ID
                                                 MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                                 DPARAM param        // ��Ӧ��Ϣ�Ĳ���
                                                 );

/****************************************************************************************************************/
//      
//    ����С���㲥�б��ں���
//  
/****************************************************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBMsgListWindow(
                                            MMI_WIN_ID_T        win_id,     //IN: ���ڵ�ID
                                            MMI_MESSAGE_ID_E     msg_id,     //IN: ���ڵ��ڲ���ϢID
                                            DPARAM                 param        //IN: ��Ӧ��Ϣ�Ĳ���
                                            );

/***************************************************************************************************************/
//      
//    ��ʾ�㲥���ݴ��ڴ�����
//  
//    
/***************************************************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBReadWindow(
                                         MMI_WIN_ID_T        win_id,     //IN: ���ڵ�ID
                                         MMI_MESSAGE_ID_E     msg_id,     //IN: ���ڵ��ڲ���ϢID
                                         DPARAM             param        //IN: ��Ӧ��Ϣ�Ĳ���
                                         );

/*****************************************************************************/
//     Description : set SMSCB channel name as window title
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL void SetChannelNameAsTitle(
                                 MMI_WIN_ID_T            win_id,         //IN: ���ڵ�ID
                                 MMISMS_CB_CHANNEL_T     *channel_list,    //IN: �ŵ��б�ṹ
                                 uint16                    msg_id            //IN: С���ŵ�ID
                                 );

/***************************************************************************************************************/
//      
//    ��ʾCLASS0���㲥���ݴ��ڴ�����
//  
//    
/***************************************************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBClass0ReadWindow(
                                               MMI_WIN_ID_T            win_id, //IN: ���ڵ�ID
                                               MMI_MESSAGE_ID_E     msg_id, //IN: ���ڵ��ڲ���ϢID
                                               DPARAM                 param    //IN: ��Ӧ��Ϣ�Ĳ���
                                               );


#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsETWSReadWindow
//  ��������:        ��ʾCLASS0���㲥���ݴ��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleSmsETWSReadWindow(
                                               MMI_WIN_ID_T            win_id, //IN: ���ڵ�ID
                                               MMI_MESSAGE_ID_E     msg_id, //IN: ���ڵ��ڲ���ϢID
                                               DPARAM                 param    //IN: ��Ӧ��Ϣ�Ĳ���
                                               );
#endif

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void AppendSmsCBLanguageListItem(
                                     MMI_CTRL_ID_T	    ctrl_id,
                                     MMISMS_CB_LANGUAGE_T   *lang_list,
                                     BOOLEAN is_update
                                     );

/*****************************************************************************/
//     Description : С���㲥�˵�����Ϣ����ӿں���
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL void HandleMainMenuMsg(
                             MMI_MENU_ID_T menu_id,    //IN: ��ѡ�еĲ˵���
                             MMI_CTRL_ID_T ctrl_id
                             );

/*****************************************************************************/
//     Description : handle SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBMainMenuWindow(
                                             MMI_WIN_ID_T        win_id,     // ���ڵ�ID
                                             MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                             DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                             );


/*****************************************************************************/
//     Description : handle SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBNoSimSettingWindow(
                                             MMI_WIN_ID_T        win_id,     // ���ڵ�ID
                                             MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                             DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                             );

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBChannelListWindow
//  ��������:        �㲥�ŵ�ѡ���б��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            liqing.peng
//
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN IsAllLangSelected(
                                MMISMS_CB_LANGUAGE_T   *lang_list
                                );

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         SetCBChannelStringInfo
//  ��������:        �������/�༭�㲥�ŵ������еı༭�ؼ��ĳ�ʼֵ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void SetCBChannelStringInfo(MMI_HANDLE_T win_id);

/*****************************************************************************/
//     Description : ���/�༭�㲥�ŵ����ڴ�����
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSCBChannelNameEditWindow(
                                                    MMI_WIN_ID_T        win_id, //IN: ���ڵ�ID
                                                    MMI_MESSAGE_ID_E     msg_id, //IN: ���ڵ��ڲ���ϢID
                                                    DPARAM                 param    //IN: ��Ӧ��Ϣ�Ĳ���
                                                    );

/*****************************************************************************/
//     Description : ���/�༭�㲥�ŵ����ڴ�����
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSCBChannelIndexEditWindow(
                                                     MMI_WIN_ID_T        win_id, //IN: ���ڵ�ID
                                                     MMI_MESSAGE_ID_E     msg_id, //IN: ���ڵ��ڲ���ϢID
                                                     DPARAM                 param    //IN: ��Ӧ��Ϣ�Ĳ���
                                                     );

/*******************************************************************************************************************************/
//
//
//                        С���㲥ģ�鴰����Ϣ����������
//
//
/******************************************************************************************************************************/
LOCAL void InitSmsCBStartUpList(MMI_CTRL_ID_T ctrl_id);

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         InitSmsCB50ChannelEnable
//  ��������:        ��ʼ��С���㲥50Ƶ�δ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void InitSmsCB50ChannelEnable(MMI_CTRL_ID_T ctrl_id);

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         InitCBChannelList
//  ��������:        ��ʼ���㲥�����б���
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void InitCBChannelList(void);

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         WstrToLongNum
//  ��������:        �������/�༭�㲥�ŵ������еı�ǩ�ؼ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN WstrToLongNum(wchar* str,uint8 len, uint32* num);

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         EditCBChannel
//  ��������:        �༭�㲥�ŵ�
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void EditCBChannel(
                         MMI_STRING_T index_str,    //Ҫ�༭���ŵ������ַ���   (�������)
                         MMI_STRING_T name_str      //Ҫ�༭���ŵ������ַ���   (�������)
                         );

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         AddCBChannel
//  ��������:        ��ӹ㲥�ŵ�
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddCBChannel(
                        MMI_STRING_T index_str,    //Ҫ��ӵ��ŵ������ַ���   (�������)
                        MMI_STRING_T name_str      //Ҫ��ӵ��ŵ������ַ���   (�������)
                        );

/*****************************************************************************/
//  Description : to handle the smscb of delete query window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCBDelQueryWinMsg(
		                                    MMI_WIN_ID_T        win_id,    //IN:
		                                    MMI_MESSAGE_ID_E    msg_id,    //IN:
		                                    DPARAM                param    //IN:
		                                    );



/*****************************************************************************/
//  Description : to handle all the smscb of delete query window    
//  Global resource dependence : 
//  Author:Tong Yongli
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCBAllDelQueryWinMsg(
		                                    MMI_WIN_ID_T        win_id,    //IN:
		                                    MMI_MESSAGE_ID_E    msg_id,    //IN:
		                                    DPARAM                param    //IN:
		                                    );


/*****************************************************************************/
//  Description : handle SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBSettingWindow(
                                             MMI_WIN_ID_T        win_id,     // ���ڵ�ID
                                             MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                             DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                             );

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_LoadCellInfoEnabledFlag
//  ��������:        ��ȡNV��С���㲥λ����Ϣ��������־
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥λ����Ϣ��������־
//  ����:            Yongli Tong
//
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN NV_LoadCellInfoEnabledFlag(MN_DUAL_SYS_E dual_sys, int8 * result);
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_SaveCellInfoEnabledFlag
//  ��������:        ����С���㲥λ����Ϣ��������־��NV��ȥ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Yongli Tong
//
//----------------------------------------------------------------------------------------------------------------

LOCAL MN_RETURN_RESULT_E NV_SaveCellInfoEnabledFlag(
			MN_DUAL_SYS_E dual_sys, 
			BOOLEAN	 cell_info_enabled
		);
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_SwitchCellInfoStatus
//  ��������:        �л�С���㲥λ����Ϣ����״̬
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Yongli Tong
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void MMISMSCB_SwitchCellInfoStatus(
            BOOLEAN cell_info_flag, 
            MN_DUAL_SYS_E dual_sys
        );

#endif


#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_MULTI_SIM_SYS_SINGLE 
/*****************************************************************************/
//  Description :settings item 1 line 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaCBSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //�ؼ�ID
                                 MMI_TEXT_ID_T  text_1,     //���ı�
                                 uint32         user_data   //�û�����
                                 );
/*****************************************************************************/
//  Description :settings item 2 line 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
 LOCAL void AppendPdaCBSettingsItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //�ؼ�ID
                                 MMI_TEXT_ID_T  text_1,     //���ı�
                                 MMI_STRING_T  text_str,     //���ı�
                                 MMI_IMAGE_ID_T image_id,   //ͼ��ID
                                 uint32         user_data,   //�û�����
                                 BOOLEAN is_need_replace
                                 );
#endif

/*****************************************************************************/
//  Description :init settings list
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/                                 
LOCAL void  InitPdaCbSmsSettingsListCtrl(MMI_CTRL_ID_T ctrl_id,BOOLEAN *flag,BOOLEAN * flag_50_channel);

/*****************************************************************************/
//	Description : the handle function of Delete Smscb
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteSmscbQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );
/*****************************************************************************/
//  Description : sms cb list opt init
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSmsCBListOptWindowInit( MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//  Description : sms cb list opt ok option
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSmsCBListOptWindowOk( MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//	Description : the handle function of All Delete Smscb
//	Global resource dependence : 
//	Author: Tong Yongli
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllDeleteSmscbQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );


#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : sms cb opt
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSmsCBOptWindow( MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//  Description :MMISMSCB_InitCBReadButtonForm
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/ 
LOCAL void InitCBReadButtonForm(void);  

/*****************************************************************************/
//  Description :MMISMS_CBGetTextRect
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMISMS_CBGetTextRect(GUI_RECT_T *rect_ptr);
#endif

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : handle cb extract number item opt
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsCBExtractNumberOptOk(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//  Description : get current channel index from visible channel list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetCurChannelIndex( uint16 item_list_index );

/*****************************************************************************/
//     Description : MMISMSCB_SetAllLangList
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
LOCAL void MMISMSCB_SetAllLangList(MMISMS_CB_LANGUAGE_T* lang_list );

/*****************************************************************************/
//     Description : MMISMSCB_Convert
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC uint32 MMISMSCB_ConvertLangIDToLangIndex(MN_SMSCB_LANGUAGE_ID_E lang_id);
#endif

/******************************************************************************************************************/
//
//
//                        С���㲥ģ�鴰�ڶ���
//
//
/******************************************************************************************************************/

//��ʾ�㲥���ݴ���

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMISMS_CB_READ_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSmsCBReadWindow ),
    WIN_ID( MMISMS_CB_READ_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    
    CREATE_TEXT_CTRL(MMISMS_CB_SHOW_MSG_TEXTBOX_CTRL_ID ),

    #ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS,MMISMS_READ_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_NULL, MMISMS_READ_FORWARD_CTRL_ID, MMISMS_READ_FORM_CTRL_ID),  
    CHILD_BUTTON_CTRL(FALSE, IMAGE_NULL,MMISMS_READ_DELETE_CTRL_ID, MMISMS_READ_FORM_CTRL_ID),
    #endif

    END_WIN
};
#else
WINDOW_TABLE( MMISMS_CB_READ_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSmsCBReadWindow ),
    WIN_ID( MMISMS_CB_READ_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(PNULL, PNULL, TXT_COMMON_COMMON_BACK),
#ifdef MMI_ETWS_SUPPORT
    CREATE_RICHTEXT_CTRL(MMISMS_CB_SHOW_MSG_TEXTBOX_CTRL_ID),
#else
    CREATE_TEXT_CTRL(MMISMS_CB_SHOW_MSG_TEXTBOX_CTRL_ID ),
#endif
    END_WIN
};
#endif

#ifdef MMI_ETWS_SUPPORT
WINDOW_TABLE( MMISMS_CB_DISPLAY_ETWS_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSmsETWSReadWindow ),
    WIN_ID( MMISMS_ETWS_NEWMSG_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(PNULL, PNULL, PNULL),
    CREATE_RICHTEXT_CTRL(MMISMS_CB_ETWS_IDLE_RICHTEXT_CTRL_ID),
    END_WIN
};
#endif

//��ʾCLASS0���㲥���ݴ���
WINDOW_TABLE( MMISMS_CB_CLASS0_READ_WIN_TAB ) = 
{
    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleSmsCBClass0ReadWindow ),
    WIN_ID( MMISMS_CB_CLASS0_READ_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_EXIT),
#ifdef MMI_ETWS_SUPPORT
    CREATE_RICHTEXT_CTRL(MMISMS_CB_CLASS0_SHOW_MSG_TEXTBOX_CTRL_ID ),
#else
    CREATE_TEXT_CTRL(MMISMS_CB_CLASS0_SHOW_MSG_TEXTBOX_CTRL_ID ),
#endif
    END_WIN
};

//���չ㲥��Ϣ�б���
WINDOW_TABLE( MMISMS_CB_MSG_LIST_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SMS_CB_MSG_LIST ),
    WIN_FUNC((uint32) HandleSmsCBMsgListWindow ),    
    WIN_ID( MMISMS_CB_MSG_LIST_WIN_ID ),
//#ifndef MMI_GUI_STYLE_TYPICAL
//    WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, STXT_SOFTKEY_READ_MK, TXT_COMMON_COMMON_BACK), 
//#else
//    WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, STXT_SOFTKEY_READ_MK, TXT_COMMON_COMMON_BACK), 
//#endif    
    WIN_SOFTKEY(STXT_OPTION, STXT_SOFTKEY_READ_MK, TXT_COMMON_COMMON_BACK), 
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMISMS_CB_MSG_LISTBOX_CTRL_ID),
    END_WIN
};


//��ʾ����/�رչ㲥ѡ���
WINDOW_TABLE( MMISMS_CB_STARTUP_WIN_TAB ) = 
{
    WIN_TITLE( STR_CB_ILIST_CELL_BROADCAST ),
    WIN_FUNC((uint32) HandleSmsCBStartUpWindow ),    
    WIN_ID( MMISMS_CB_STARTUP_WIN_ID ),
    WIN_SOFTKEY(PNULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISMS_CB_STARTUP_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( PHONE_SMS_CB_ALERT_WIN_TAB ) = 
{
    WIN_ID( PHONE_SMS_CB_ALERT_WIN_ID ),
    END_WIN
};

//��ʾ����/�ر�50 Ƶ��ѡ���
WINDOW_TABLE( MMISMS_CB_50_CHANNEL_WIN_TAB ) = 
{
    WIN_TITLE( STR_CB_CELL_INFO ),
    WIN_FUNC((uint32) HandleSmsCB50ChannelWindow ),    
    WIN_ID( MMISMS_CB_50_CHANNEL_WIN_ID ),
    WIN_SOFTKEY(PNULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISMS_CB_50_CHANNEL_LISTBOX_CTRL_ID),
    END_WIN
};

//�㲥�����б����ݴ���
WINDOW_TABLE( MMISMS_CB_LANGUAGE_LIST_WIN_TAB ) = 
{
    WIN_TITLE( STR_CB_LANGUAGE ),
    WIN_FUNC((uint32) HandleSmsCBLanguageListWindow ),    
    WIN_ID( MMISMS_CB_LANGUAGE_LIST_WIN_ID ),
//#ifndef MMI_GUI_STYLE_TYPICAL
    //WIN_SOFTKEY(STXT_SOFTKEY_SAVE_MK, TXT_NULL, STXT_CANCEL),
//#else
    //WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//#endif
    WIN_SOFTKEY(STXT_SOFTKEY_SAVE_MK, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK), 
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMISMS_CB_LANGUAGE_LISTBOX_CTRL_ID),
    END_WIN
};


//�㲥�ŵ��б����ݴ���
WINDOW_TABLE( MMISMS_CB_CHANNEL_LIST_WIN_TAB ) = 
{
    WIN_TITLE( STR_CB_CHANNEL ),
    WIN_FUNC((uint32) HandleSmsCBChannelListWindow ),    
    WIN_ID( MMISMS_CB_CHANNEL_LIST_WIN_ID ),
    //WIN_SOFTKEY(STXT_OPTION, STR_CB_ILIST_CELL_BROADCAST, STXT_RETURN), 
    WIN_SOFTKEY(PNULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID),
    END_WIN
};


//��ӱ༭Ƶ������
WINDOW_TABLE( MMISMS_CB_ADD_EDIT_CHANNEL_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSMSCBChannelEditWindow ),    
    WIN_ID( MMISMS_CB_ADD_EDIT_CHANNEL_WIN_ID ),
    //WIN_TITLE(TXT_NULL),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISMS_CB_ADD_EDIT_CHANNEL_CTRL_ID),

    //CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_CB_CHANNEL_INDEX_FORM_CTRL_ID,MMISMS_CB_ADD_EDIT_CHANNEL_CTRL_ID),
	    //CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISMS_CB_CHANNEL_INDEX_LABEL_CTRL_ID,MMISMS_CB_CHANNEL_INDEX_FORM_CTRL_ID),
	    //CHILD_EDIT_DIGITAL_CTRL(TRUE,CB_CHANNEL_INDEX_MAX_LEN,MMISMS_CB_CHANNEL_SHOW_INDEX_LABEL_CTRL_ID,MMISMS_CB_CHANNEL_INDEX_FORM_CTRL_ID),

    //CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_CB_CHANNEL_NAME_FORM_CTRL_ID,MMISMS_CB_ADD_EDIT_CHANNEL_CTRL_ID),
	    //CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISMS_CB_CHANNEL_NAME_LABEL_CTRL_ID,MMISMS_CB_CHANNEL_NAME_FORM_CTRL_ID),
	    //CHILD_EDIT_TEXT_CTRL(TRUE,CB_CHANNEL_NAME_MAX_LEN,MMISMS_CB_CHANNEL_SHOW_NAME_LABEL_CTRL_ID,MMISMS_CB_CHANNEL_NAME_FORM_CTRL_ID),

    CHILD_EDIT_TEXT_CTRL(TRUE,CB_CHANNEL_INDEX_MAX_LEN,MMISMS_CB_CHANNEL_SHOW_INDEX_LABEL_CTRL_ID,MMISMS_CB_ADD_EDIT_CHANNEL_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,CB_CHANNEL_NAME_MAX_LEN,MMISMS_CB_CHANNEL_SHOW_NAME_LABEL_CTRL_ID,MMISMS_CB_ADD_EDIT_CHANNEL_CTRL_ID),

#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(STXT_SOFTKEY_SAVE_MK, TXT_NULL, STXT_SOFTKEY_CANCEL_MK,MMISMS_CB_CHANNEL_ADD_SOFTKEY_CTRL_ID,MMISMS_CB_ADD_EDIT_CHANNEL_CTRL_ID),
#else
    WIN_SOFTKEY(STXT_SOFTKEY_SAVE_MK, TXT_NULL, STXT_SOFTKEY_CANCEL_MK),     //custom_save_key         
#endif

    END_WIN
};

//��ӱ༭Ƶ������
WINDOW_TABLE( MMISMS_CB_ADD_EDIT_CHANNELINDEX_WIN_TAB ) = 
{
    WIN_FUNC((uint32)  HandleSMSCBChannelIndexEditWindow),    
    WIN_TITLE(STR_CB_CHANNEL_INDEX),
    WIN_ID( MMISMS_CB_ADD_EDIT_CHANNELINDEX_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(CB_CHANNEL_INDEX_MAX_LEN,MMISMS_CB_CHANNEL_INDEX_EDIT_CTRL_ID),
    END_WIN
};

//��ӱ༭Ƶ������
WINDOW_TABLE( MMISMS_CB_ADD_EDIT_CHANNELNAME_WIN_TAB ) = 
{
    WIN_FUNC((uint32)  HandleSMSCBChannelNameEditWindow),    
    WIN_TITLE(STR_CB_CHANNEL_NAME),
    WIN_ID( MMISMS_CB_ADD_EDIT_CHANNELNAME_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    
    CREATE_EDIT_TEXT_CTRL(CB_CHANNEL_NAME_MAX_LEN,MMISMS_CB_CHANNEL_NAME_EDIT_CTRL_ID),

    END_WIN
};

//�ŵ�����쿴����
WINDOW_TABLE( MMISMS_CB_CHANNEL_CHECK_WIN_TAB ) = 
{
    WIN_TITLE( STXT_DETAIL ),
    WIN_FUNC((uint32) HandleSmsCBChannelCheckWindow ),    
    WIN_ID( MMISMS_CB_CHECK_CHANNEL_NUM_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMISMS_CB_CHANNEL_CHECK_CTRL_ID),

    END_WIN
};

// broadcast main menu window //MMI_GUI_STYLE_TYPICAL ר�ã�sms�ع��ݲ�����
WINDOW_TABLE( MMISMS_CB_MAIN_MENU_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SMS_CB ),
    WIN_FUNC((uint32) HandleSmsCBMainMenuWindow ),    
    WIN_ID( MMISMS_CB_MAIN_MENU_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    
    CREATE_MENU_CTRL(MENU_SMS_BROADCAST_OPT, MMISMS_CB_MAIN_MENU_CTRL_ID),
    END_WIN
};

#ifdef MMI_ETWS_SUPPORT
//�޿�С���㲥���ô���
WINDOW_TABLE( MMISMS_CB_SETTING_NO_SIM_WIN_TAB ) =
{
    WIN_TITLE( STR_CB_CELL_BROADCAST ),
    WIN_FUNC((uint32) HandleSmsCBNoSimSettingWindow ),
    WIN_ID( MMISMS_CB_SETTING_WIN_ID ),
    WIN_SOFTKEY(PNULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),

#ifndef MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_CB_SETTING_CTRL_ID),
#else
    CREATE_MENU_CTRL(MENU_SMS_BROADCAST_SETTING_OPT,MMISMS_CB_SETTING_CTRL_ID),
#endif
    END_WIN
};
#endif

//����С���㲥���ô���
WINDOW_TABLE( MMISMS_CB_SETTING_WIN_TAB ) = 
{
    WIN_TITLE( STR_CB_CELL_BROADCAST ),
    WIN_FUNC((uint32) HandleSmsCBSettingWindow ),    
    WIN_ID( MMISMS_CB_SETTING_WIN_ID ),
    WIN_SOFTKEY(PNULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),

#ifndef MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_CB_SETTING_CTRL_ID),
#else
    CREATE_MENU_CTRL(MENU_SMS_BROADCAST_SETTING_OPT,MMISMS_CB_SETTING_CTRL_ID),
#endif
    END_WIN
};

LOCAL const MMI_TEXT_ID_T setting_title_id[] ={
#ifdef MMI_MULTI_SIM_SYS_SINGLE 
                STR_CB_SETTINGS
#else
                STR_CB_SIM1,
                STR_CB_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
                TXT_SMS_CB_SET_CARD3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
                TXT_SMS_CB_SET_CARD4
#endif
#endif
        	};


/******************************************************************************************************************/
//
//
//                        С���㲥ģ�� NV ������غ�������
//
//
/******************************************************************************************************************/

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_LoadSmsCBEnabledFlag
//  ��������:        ��ȡNV��С���㲥��������־
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥������־
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMISMS_CB_CONFIG_T NV_LoadSmsCBEnabledConfig(MN_DUAL_SYS_E dual_sys)
{
    MMISMS_CB_CONFIG_T cb_config[MMI_DUAL_SYS_MAX] = {CB_CONFIG_OFF};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    int32 i = 0;

    MMINV_READ(MMINV_SMSCB_ENABLE, cb_config, return_val);
    for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
    {
        SCI_TRACE_LOW("NV_LoadSmsCBEnabledConfig MMINV_READ bEnabled[%d] = %d. return_val = %d.", i, cb_config[i], return_val);
    }
    if(return_val != MN_RETURN_SUCCESS)
    {
        for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
        {
            cb_config[i] = MMISMSCB_GetCBDefaultConfig();        //Ĭ��Ϊ�ر�״̬
        }

        MMINV_WRITE(MMINV_SMSCB_ENABLE, &cb_config);
    }
    
    return cb_config[dual_sys];
}

#else
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_LoadSmsCBEnabledFlag
//  ��������:        ��ȡNV��С���㲥��������־
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥������־
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN NV_LoadSmsCBEnabledFlag(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN bEnabled[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    int32 i = 0;

    /*for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
    {
        bEnabled[i] = MMISMSCB_IsCBDefaultEnable();        //Ĭ��Ϊ�ر�״̬
    }*/

    MMINV_READ(MMINV_SMSCB_ENABLE, bEnabled, return_val);
    for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
    {
        SCI_TRACE_LOW("NV_LoadSmsCBEnabledFlag MMINV_READ bEnabled[%d] = %d. return_val = %d.", i, bEnabled[i], return_val);
    }
    if(return_val != MN_RETURN_SUCCESS)
    {
        for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
        {
            bEnabled[i] = MMISMSCB_IsCBDefaultEnable();        //Ĭ��Ϊ�ر�״̬
        }
		
        MMINV_WRITE(MMINV_SMSCB_ENABLE, &bEnabled);
    }
    
    return bEnabled[dual_sys];
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_LoadSmsCB50ChannelFlag
//  ��������:        ��ȡNV��50Ƶ��С���㲥��������־
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥50Ƶ�ε�������־
//  ����:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN NV_LoadSmsCB50ChannelFlag(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN bEnabled[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    int32 i = 0;

    MMINV_READ(MMINV_SMSCB_50_CHANNEL_ENABLE, bEnabled, return_val);
    if(return_val != MN_RETURN_SUCCESS)
    {
        is_system_factory = TRUE;
        for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
        {
            bEnabled[i] = FALSE;        //Ĭ��Ϊ��״̬
        }		
        MMINV_WRITE(MMINV_SMSCB_50_CHANNEL_ENABLE, &bEnabled);
    }
    
    return bEnabled[dual_sys];
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_SaveSmsCB50ChannelFlag
//  ��������:        ����NV��50Ƶ��С���㲥��������־
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------

LOCAL void NV_SaveSmsCB50ChannelFlag(
                                   BOOLEAN bEnabled             //Ҫ�����������־ֵ (�������)
                                   )
{
    BOOLEAN wEnabled[MMI_DUAL_SYS_MAX] = {0};
    int32 i = 0;
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();

    MMINV_READ(MMINV_SMSCB_50_CHANNEL_ENABLE, wEnabled, return_val);
    wEnabled[dual_sys] = bEnabled;

    MMINV_WRITE(MMINV_SMSCB_50_CHANNEL_ENABLE, &wEnabled);
    
}

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_SaveSmsCBEnabledFlag
//  ��������:        ����С���㲥��������־��NV��ȥ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void NV_SaveSmsCBEnabledConfig(
                                   MMISMS_CB_CONFIG_T cb_config_value             //Ҫ�����������־ֵ (�������)
                                   )
{
    MMISMS_CB_CONFIG_T cb_config[MMI_DUAL_SYS_MAX] = {CB_CONFIG_OFF};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();

    MMINV_READ(MMINV_SMSCB_ENABLE, cb_config, return_val);
    cb_config[dual_sys] = cb_config_value;

    MMINV_WRITE(MMINV_SMSCB_ENABLE, &cb_config);
}
#else
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_SaveSmsCBEnabledFlag
//  ��������:        ����С���㲥��������־��NV��ȥ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------

LOCAL void NV_SaveSmsCBEnabledFlag(
                                   BOOLEAN bEnabled             //Ҫ�����������־ֵ (�������)
                                   )
{
    BOOLEAN wEnabled[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();

    MMINV_READ(MMINV_SMSCB_ENABLE, wEnabled, return_val);
    wEnabled[dual_sys] = bEnabled;

    MMINV_WRITE(MMINV_SMSCB_ENABLE, &wEnabled);
    
}
#endif

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_LoadCellInfoEnabledFlag
//  ��������:        ��ȡNV��С���㲥λ����Ϣ��������־
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥λ����Ϣ��������־
//  ����:            Yongli Tong
//
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN NV_LoadCellInfoEnabledFlag(MN_DUAL_SYS_E dual_sys, int8 * result)
{
    BOOLEAN cell_info_flag[MMI_DUAL_SYS_MAX] = {
    FALSE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    ,FALSE
#endif
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    ,FALSE
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    ,FALSE
#endif
    };
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SMSCB_CELL_INFO_ENABLE, cell_info_flag, return_val);
    
    if(return_val != MN_RETURN_SUCCESS)
    {
        MMINV_WRITE(MMINV_SMSCB_CELL_INFO_ENABLE, &cell_info_flag);
    }
    *result = (int8)return_val;
    
    return cell_info_flag[dual_sys];
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_SaveCellInfoEnabledFlag
//  ��������:        ����С���㲥λ����Ϣ��������־��NV��ȥ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Yongli Tong
//
//----------------------------------------------------------------------------------------------------------------

LOCAL MN_RETURN_RESULT_E NV_SaveCellInfoEnabledFlag(
			MN_DUAL_SYS_E dual_sys, 
			BOOLEAN	 cell_info_enabled
		)
{
    BOOLEAN cell_info_flag[MMI_DUAL_SYS_MAX] = {
        FALSE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,FALSE
#endif
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,FALSE
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,FALSE
#endif
    };
    int8 i = 0;
    int8 result = MN_RETURN_SUCCESS;

    for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
    {
        cell_info_flag[i] = NV_LoadCellInfoEnabledFlag((MN_DUAL_SYS_E)i, &result);
        if(MN_RETURN_SUCCESS == result)
        {
            break;
        }
    }
    if(MN_RETURN_FAILURE == result)
    {
        return result;
    }

    cell_info_flag[dual_sys] = cell_info_enabled;

    MMINV_WRITE(MMINV_SMSCB_CELL_INFO_ENABLE, &cell_info_flag);
    return result;
}
#endif

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_LoadSmsCBLanguageList
//  ��������:        ��ȡNV�б����С���㲥����ѡ���б�ṹ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥����ѡ���б�ṹ
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL  void NV_LoadSmsCBLanguageList(MN_DUAL_SYS_E dual_sys, MMISMS_CB_LANGUAGE_T* result_lang_list)
{
    MMISMS_CB_LANGUAGE_T lang_list[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    int32 i = 0, j = 0;

    MMINV_READ(MMINV_SMSCB_LANGUAGE_LIST, &lang_list, return_val);

    if(return_val != MN_RETURN_SUCCESS)
    {
        //�趨�㲥����ѡ���б��ʼֵ
        for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
        {
            for(j = 0; j < MMI_MAX_CB_LANGUAGE_NUM; j++)
            {
                lang_list[i].selected_arr[j]=TRUE;
            }
        }

        MMINV_WRITE(MMINV_SMSCB_LANGUAGE_LIST, &lang_list);
    }

    SCI_MEMCPY(result_lang_list,&lang_list[dual_sys],sizeof(MMISMS_CB_LANGUAGE_T));
}
#else
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_LoadSmsCBLanguageList
//  ��������:        ��ȡNV�б����С���㲥����ѡ���б�ṹ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥����ѡ���б�ṹ
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMISMS_CB_LANGUAGE_T NV_LoadSmsCBLanguageList(MN_DUAL_SYS_E dual_sys)
{
    MMISMS_CB_LANGUAGE_T lang_list[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    MMISMS_CB_LANGUAGE_T result_lang_list = {0};
    int32 i = 0, j = 0;

       SCI_TRACE_LOW("MMISMSCB: NV_LoadSmsCBLanguageList");

    MMINV_READ(MMINV_SMSCB_LANGUAGE_LIST, &lang_list, return_val);
    
    if(return_val != MN_RETURN_SUCCESS)
    {
        //�趨�㲥����ѡ���б��ʼֵ
        for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
        {
            for(j = 0; j < MMI_MAX_CB_LANGUAGE_NUM; j++)
            {
                lang_list[i].selected_arr[j]=TRUE;
            }
        }
        
        MMINV_WRITE(MMINV_SMSCB_LANGUAGE_LIST, &lang_list);
    }

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }
    SCI_MEMCPY(&result_lang_list,&lang_list[dual_sys],sizeof(MMISMS_CB_LANGUAGE_T));
    
    return result_lang_list;
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_SaveSmsCBLanguageList
//  ��������:        ����С���㲥�����б�ṹ��NV��ȥ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void NV_SaveSmsCBLanguageList(
                                    MMISMS_CB_LANGUAGE_T   lang_list    //Ҫ�ı���С���㲥�����б�ṹ (�������)
                                    )
{
    MMISMS_CB_LANGUAGE_T   wlang_list[MMI_DUAL_SYS_MAX] = {0};
    int32 i = 0;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();

       SCI_TRACE_LOW("MMISMSCB: NV_SaveSmsCBLanguageList");

    for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
    {
#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBLanguageList((MN_DUAL_SYS_E)i, &wlang_list[i]);
#else
        wlang_list[i] = NV_LoadSmsCBLanguageList((MN_DUAL_SYS_E)i);
#endif
    }

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }
    SCI_MEMCPY(&wlang_list[dual_sys],&lang_list,sizeof(MMISMS_CB_LANGUAGE_T));
    
    MMINV_WRITE(MMINV_SMSCB_LANGUAGE_LIST, &wlang_list);
}

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMS_GetSmsCBEnabledFlag
//  ��������:        �Ƿ���С���㲥
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN MMISMS_GetSmsCBEnabledFlag(MN_DUAL_SYS_E dual_sys)
{
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    BOOLEAN cb_enabled = FALSE;
    MMISMS_CB_CONFIG_T cb_config_value = CB_CONFIG_OFF;
    CMAS_CHANNEL_INFO_T* current_channel_table = GetCMASChannelTable(dual_sys);

    SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

    if (TRUE == is_set_mcc)
    {
        plmn_info.mcc  = s_set_mcc;
    }

    SCI_TRACE_LOW("MMISMS_GetSmsCBEnabledFlag, plmn_info.mcc  = %d", plmn_info.mcc );

    if( PNULL != current_channel_table )
    {
        cb_config_value = CB_CONFIG_ON;
    }
    else
    {
	cb_config_value = NV_LoadSmsCBEnabledConfig(dual_sys);
    }

    if( CB_CONFIG_ON == cb_config_value)
    {
        cb_enabled = TRUE;
    }
    else
    {
        cb_enabled = FALSE;
    }

    SCI_TRACE_LOW("MMISMS_GetSmsCBEnabledFlag, cb_config_value  = %d, cb_enabled = %d", cb_config_value, cb_enabled );

    return cb_enabled;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_LoadSmsCBChannelList
//  ��������:        ��ȡNV��С���㲥�ŵ��б�ṹ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥�ŵ��б�ṹ
//  ����:
//
//----------------------------------------------------------------------------------------------------------------
LOCAL  void NV_LoadSmsCBChannelList( void )
{
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    int32 i = 0;

    SCI_MEMSET(s_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T)*MN_DUAL_SYS_MAX);

    MMINV_READ(MMINV_SMSCB_CHANNEL_LIST, &s_nv_channel_list, return_val);
    
    if (return_val != MN_RETURN_SUCCESS)
    {
        MMINV_WRITE(MMINV_SMSCB_CHANNEL_LIST, &s_nv_channel_list);
    }

    SCI_TRACE_LOW("NV_LoadSmsCBChannelList, sizeof(MMISMS_CB_CHANNEL_T) = %d", sizeof(MMISMS_CB_CHANNEL_T));
}
#endif

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_LoadSmsCBChannelList
//  ��������:        ��ȡNV��С���㲥�ŵ��б�ṹ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥�ŵ��б�ṹ
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMS_NV_LoadSmsCBChannelList(void)
{
    NV_LoadSmsCBChannelList();
}
#else
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_LoadSmsCBChannelList
//  ��������:        ��ȡNV��С���㲥�ŵ��б�ṹ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥�ŵ��б�ṹ
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CHANNEL_T MMISMS_NV_LoadSmsCBChannelList(MN_DUAL_SYS_E dual_sys)
{
    MMISMS_CB_CHANNEL_T channel_list[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    MMISMS_CB_CHANNEL_T return_channel_list = {0};
    int32 i = 0;

       SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_LoadSmsCBChannelList");
	
    MMINV_READ(MMINV_SMSCB_CHANNEL_LIST, &channel_list, return_val);
    
    if (return_val != MN_RETURN_SUCCESS)
    {
        //�趨�㲥�ŵ��б��ʼֵ
        for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
        {
            SCI_MEMSET(&channel_list[i], 0x00, sizeof(MMISMS_CB_CHANNEL_T));
        }
        
        MMINV_WRITE(MMINV_SMSCB_CHANNEL_LIST, &channel_list);
    }

    SCI_MEMCPY(&return_channel_list,&channel_list[dual_sys],sizeof(MMISMS_CB_CHANNEL_T));
    
    return (return_channel_list);
}
#endif

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_SaveSmsCBLanguageList
//  ��������:        ����С���㲥�ŵ��б�ṹ��NV��ȥ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void NV_SaveSmsCBChannelList( void )
{
    int32 i = 0;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();

    SCI_TRACE_LOW("NV_SaveSmsCBChannelList, dual_sys = %d",dual_sys);

    SCI_MEMSET(s_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T)*MN_DUAL_SYS_MAX);

    NV_LoadSmsCBChannelList();

    SCI_MEMCPY(&s_nv_channel_list[dual_sys],&s_save_to_nv_channel_list, sizeof(MMISMS_CB_CHANNEL_T));

    MMINV_WRITE(MMINV_SMSCB_CHANNEL_LIST, &s_nv_channel_list);
}
#else
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         NV_SaveSmsCBLanguageList
//  ��������:        ����С���㲥�ŵ��б�ṹ��NV��ȥ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void NV_SaveSmsCBChannelList(
                                   MMISMS_CB_CHANNEL_T   *channel_list   //Ҫ�����С���㲥�ŵ��б�ṹ (�������)
                                   )
{
    MMISMS_CB_CHANNEL_T wchannel_list[MMI_DUAL_SYS_MAX] = {0};
    int32 i = 0;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();

       SCI_TRACE_LOW("MMISMSCB: NV_SaveSmsCBChannelList");

    for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
    {
        wchannel_list[i] = MMISMS_NV_LoadSmsCBChannelList((MN_DUAL_SYS_E)i);
    }

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }
    SCI_MEMCPY(&wchannel_list[dual_sys],channel_list,sizeof(MMISMS_CB_CHANNEL_T));

    MMINV_WRITE(MMINV_SMSCB_CHANNEL_LIST, &wchannel_list);
}
#endif

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMS_NV_LoadSmsCBMessageList
//  ��������:        ��ȡNV��С���㲥��Ϣ��Ŀ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥��Ϣ
//  ����:            
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC uint8 MMISMS_NV_LoadSmsCBMessageListNum(void)
{
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    uint8 message_num = 0;
    uint8 return_message_num = 0;
	
    MMINV_READ(MMINV_SMSCB_MESSAGE_LIST_NUM, &message_num, return_val);

    if (return_val != MN_RETURN_SUCCESS)
    {
        message_num = 0;
        MMINV_WRITE(MMINV_SMSCB_MESSAGE_LIST_NUM, &message_num);
    }

    return_message_num = message_num;

    SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_LoadSmsCBMessageListNum, return_message_num = %d", return_message_num);

     return (return_message_num);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSNV_SaveSmsCBMessageList
//  ��������:        ����С���㲥��Ϣ��Ŀ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:          
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSNV_SaveSmsCBMessageListNum( uint8 message_num)
{
	SCI_TRACE_LOW("MMISMSCB: MMISMSNV_SaveSmsCBMessageListNum, message_num = %d", message_num);

	if( message_num > MMINV_MAX_SMSCB_RECORDS )
		message_num = MMINV_MAX_SMSCB_RECORDS;
    MMINV_WRITE(MMINV_SMSCB_MESSAGE_LIST_NUM,&message_num);
}


//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMS_NV_LoadSmsCBMessageList
//  ��������:        ��ȡNV��С���㲥��Ϣ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥��Ϣ
//  ����:            
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_MESSAGE_ARRAY_T* MMISMS_NV_LoadSmsCBMessageList(uint8 index)
{
    MMISMS_CB_MESSAGE_ARRAY_T message_array = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
	
	SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_LoadSmsCBMessageList");
	if(PNULL == s_cb_storage_message_list_ptr)
	{
     	  s_cb_storage_message_list_ptr = SCI_ALLOC_APP(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
	}
    MMINV_READ(MMINV_SMSCB_MESSAGE_LIST_FIRST+index, &message_array, return_val);
    
    if (return_val != MN_RETURN_SUCCESS)
    {
		SCI_MEMSET((MMISMS_CB_MESSAGE_ARRAY_T*)&message_array, 0x00, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
        MMINV_WRITE(MMINV_SMSCB_MESSAGE_LIST_FIRST+index, &message_array);
	}
	
	
    SCI_MEMCPY(s_cb_storage_message_list_ptr, &message_array,sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
	
    return (s_cb_storage_message_list_ptr);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSNV_SaveSmsCBMessageList
//  ��������:        ����С���㲥��Ϣ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:          
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSNV_SaveSmsCBMessageList(MMISMS_CB_MESSAGE_ARRAY_T*  message_array, uint8 index)
{
    MMISMS_CB_MESSAGE_ARRAY_T wmessage_array = {0};
    SCI_TRACE_LOW("MMISMSCB: MMISMSNV_SaveSmsCBMessageList");
	
    SCI_MEMCPY(&wmessage_array,&message_array,sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
	
    MMINV_WRITE(MMINV_SMSCB_MESSAGE_LIST_FIRST+index,&wmessage_array);
}
#endif

/*********************************************************************************************************************/
//
//
//                        С���㲥ģ�� MN��ӿڵ��ú�������
//
//
/*********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         IsCBChannelListEmpty
//  ��������:        �жϹ㲥�ŵ��б��Ƿ�Ϊ��
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          TRUE Ϊ�գ�FALSE ��Ϊ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN IsCBChannelListEmpty(void)
{
    MMISMS_CB_CHANNEL_T   channel_list = {0};

    SCI_TRACE_LOW("MMISMSCB: IsCBChannelListEmpty");

    //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif
    if(0 == channel_list.num)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

LOCAL void NV_RefreshPreloadChannelList(void)
{
    MMISMS_CB_CHANNEL_T   channel_list = {0};
    MMISMS_CB_CHANNEL_T wchannel_list[MMI_DUAL_SYS_MAX] = {0};
    int32 i = 0;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();

     SCI_TRACE_LOW("MMISMSCB: NV_RefreshPreloadChannelList");

    for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
    {
#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBChannelList();
        SCI_MEMCPY(&wchannel_list[i], &s_nv_channel_list[i], sizeof(MMISMS_CB_CHANNEL_T));
#else
        wchannel_list[i] = MMISMS_NV_LoadSmsCBChannelList((MN_DUAL_SYS_E)i);
#endif
    }
    
    channel_list.channel_arr->msg_id = 12;

    SCI_MEMCPY(&wchannel_list[dual_sys],&channel_list,sizeof(MMISMS_CB_CHANNEL_T));
    
    MMINV_WRITE(MMINV_SMSCB_CHANNEL_LIST, &wchannel_list);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         IsCBChannelListFull
//  ��������:        �жϹ㲥�ŵ��б��Ƿ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          TRUE ������FALSE δ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN IsCBChannelListFull(void)
{
    MMISMS_CB_CHANNEL_T   channel_list = {0};

    //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif

    if(channel_list.num>=MMI_MAX_CB_CHANNEL_NUM)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         ConvertChannelListToMsgIDMuster
//  ��������:        ���ŵ��б�ת��С���㲥���ŵ�ID����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ת������ŵ�ID����
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MN_CB_MSG_ID_MUSTER_T ConvertChannelListToMsgIDMuster(
                                                            MMISMS_CB_CHANNEL_T *channel_list  //�ŵ��б� (�������)
                                                            )
{
    MN_CB_MSG_ID_MUSTER_T msg_id = {0};
    uint32 i = 0;
    uint32 j = 0;
    
    for(i=0,j=0;i<channel_list->num;i++)
    {
        if(channel_list->channel_arr[i].is_enabled)
        {
            msg_id.msg_id_arr[j++]=channel_list->channel_arr[i].msg_id;
        }
    }
    
    msg_id.num=j;
    
    return msg_id;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         ConvertLangListToLangIDMuster
//  ��������:        ������ѡ���б�ת��С���㲥������ID����
//  ȫ�ֱ���ʹ��:    g_cb_lang_id_arr
//  ����ֵ:          ת������ŵ�ID����
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MN_CB_LANGUAGE_ID_MUSTER_T ConvertLangListToLangIDMuster(
                                                               MMISMS_CB_LANGUAGE_T *lang_list //�����б� (�������)
                                                               )
{
    MN_CB_LANGUAGE_ID_MUSTER_T lang_id = {0};
    
    uint32 i = 0,j = 0;
    
    for(i=0,j=0;i<MN_CB_LANGUAGE_ID_MAX_NUM;i++)
    {
        if(lang_list->selected_arr[i])                          //��ĳ�����Ա�ѡ��
        {
            lang_id.language_id_arr[j++]=g_cb_lang_id_arr[i];   //�򽫾�̬�����е�����ID�Ÿ����Լ�������
        }
    }
    
    lang_id.num=j;
    
    return lang_id;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         GetCBChannelNameByMsgId
//  ��������:        ��С���ŵ�ID�����Ӧ���ŵ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          �ŵ�����
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CHANNEL_NAME_T MMISMS_GetCBChannelNameByMsgId(                                        //RETURN: 
                                                       MMISMS_CB_CHANNEL_T    *channel_list,    //IN: �ŵ��б�ṹ
                                                       uint16                 msg_id            //IN: С���ŵ�ID
                                                       )
{
    int32 i  = 0;
    MMISMS_CB_CHANNEL_NAME_T ret = {0};
    
    SCI_MEMSET(&ret, 0, sizeof (MMISMS_CB_CHANNEL_NAME_T));
    
    for (i=0; i<channel_list->num; i++)
    {
        if (channel_list->channel_arr[i].msg_id == msg_id)
        {
            ret = channel_list->channel_arr[i].name;
        }
    }
    return (ret);
}

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         ConvertCBTimeToStrForTitle
//  ��������:        ��С���㲥ʱ��תΪUCS2���׼�ַ�����ʽ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ת������ַ�������
//  ����: 
//
//----------------------------------------------------------------------------------------------------------------
uint16 ConvertCBTimeToStrForTitle(                            //RETURN:
                          SCI_DATE_T    date,        //IN: Ҫ����ת�������ڽṹ
                          SCI_TIME_T    time,       //IN: Ҫ����ת����ʱ��ṹ
                          wchar            *str_ptr    //OUT: ����ַ�����ָ��
                          )
{
    uint16 len = 0;
    uint8 time_str[CB_MSG_TIME_MAX_LEN +1 ] = {0};

    SCI_MEMSET(time_str, 0, (CB_MSG_TIME_MAX_LEN + 1));

    len = sprintf(
        (char *)time_str, 
        "%2d.%02d.%04d %02d:%02d\n",
        date.mday, date.mon, date.year, time.hour, time.min, time.sec
        );

    MMI_STRNTOWSTR( str_ptr, CB_MSG_TIME_MAX_LEN, (uint8 *)time_str, CB_MSG_TIME_MAX_LEN, len );

    return (len);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         ConvertCBTimeToStrForDetails
//  ��������:        ��С���㲥ʱ��תΪUCS2���׼�ַ�����ʽ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ת������ַ�������
//  ����: 
//
//----------------------------------------------------------------------------------------------------------------
uint16 ConvertCBTimeToStrForDetails(                            //RETURN:
                          SCI_DATE_T    date,        //IN: Ҫ����ת�������ڽṹ
                          SCI_TIME_T    time,       //IN: Ҫ����ת����ʱ��ṹ
                          //BOOLEAN        is_to_ucs2, //IN: �Ƿ�ת��ΪUCS2��ʽ�ַ���
                          wchar            *str_ptr    //OUT: ����ַ�����ָ��
                          )
{
    uint16 len = 0;
    uint8 time_str[CB_MSG_TIME_MAX_LEN +1 ] = {0};
    
    SCI_MEMSET(time_str, 0, (CB_MSG_TIME_MAX_LEN + 1));
    
    len = sprintf(
        (char *)time_str, 
        "%2d.%02d.%04d\n%02d:%02d\n",
        date.mday, date.mon, date.year, time.hour, time.min, time.sec
        );

    MMI_STRNTOWSTR( str_ptr, CB_MSG_TIME_MAX_LEN, (uint8 *)time_str, CB_MSG_TIME_MAX_LEN, len );
    
    return (len);
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         ConvertCBTimeToStr
//  ��������:        ��С���㲥ʱ��תΪUCS2���׼�ַ�����ʽ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ת������ַ�������
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
uint16 ConvertCBTimeToStr(                            //RETURN:
                          SCI_DATE_T    date,        //IN: Ҫ����ת�������ڽṹ
                          SCI_TIME_T    time,       //IN: Ҫ����ת����ʱ��ṹ
                          //BOOLEAN        is_to_ucs2, //IN: �Ƿ�ת��ΪUCS2��ʽ�ַ���
                          wchar            *str_ptr    //OUT: ����ַ�����ָ��
                          )
{
    uint16 len = 0;
    uint8 time_str[CB_MSG_TIME_MAX_LEN +1 ] = {0};
    
    SCI_MEMSET(time_str, 0, (CB_MSG_TIME_MAX_LEN + 1));
    
    len = sprintf(
        (char *)time_str, 
        "%02d/%02d %02d:%02d\n",
        date.mon, date.mday, time.hour, time.min
        );
    
    MMI_STRNTOWSTR( str_ptr, CB_MSG_TIME_MAX_LEN, (uint8 *)time_str, CB_MSG_TIME_MAX_LEN, len );
    
    return (len);
}

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_ConvertCBTimeToStr
//  ��������:        ��С���㲥ʱ��תΪUCS2���׼�ַ�����ʽ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ת������ַ�������
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC uint16 MMISMSCB_ConvertCBTimeToStr(                            //RETURN:
								   SCI_DATE_T    date,        //IN: Ҫ����ת�������ڽṹ
								   SCI_TIME_T    time,       //IN: Ҫ����ת����ʱ��ṹ
								   //BOOLEAN        is_to_ucs2, //IN: �Ƿ�ת��ΪUCS2��ʽ�ַ���
								   wchar            *str_ptr    //OUT: ����ַ�����ָ��
								   )
{
	uint16 len = 0;
	len = ConvertCBTimeToStr(date, time, *str_ptr);
	
	return len;
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         ConvertCBMsgContextToMsgItem
//  ��������:        ��С���㲥���ݰ�������뷽ʽ��ѹΪС���㲥��Ϣ��ṹ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          С���㲥��Ϣ
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC uint16 ConvertCBMsgContextToMsgItem(                                           //RETURN: 
                                    MN_CB_MSG_ARR_T           msg_arr,         //IN: С���㲥��Ϣ��������ָ��
                                    uint16                    msg_len,         //IN: С���㲥��Ϣ�������ݳ���
                                    MN_SMS_ALPHABET_TYPE_E    alphabet_type,   //IN: С���㲥��Ϣ���ݱ��뷽ʽ
                                    BOOLEAN                   is_macro_msg,    //IN: �Ƿ�Ϊ��С���㲥
                                    MMISMS_CB_MESSAGE_ITEM_T  *msg_item_ptr    //IN: С���㲥��Ϣ��ָ��
                                    )  
{
    uint16 msg_max_len = 0;
    
    //SCI_TRACE_LOW:"SMSCB: ConvertCBMsgContextToMsgItem msg_len = %d, alphabet_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_1283_112_2_18_2_55_31_886,(uint8*)"dd",msg_len, alphabet_type);
    
    //���ǳ��㲥�����ݳ���Ϊ MN_CB_MACRO_MSG_LEN ����Ϊ MN_CB_MSG_LEN
    if (is_macro_msg)
    {
        msg_max_len = MN_CB_MACRO_MSG_LEN;
    }
    else 
    {
        msg_max_len = MN_CB_MSG_LEN;
    }
    
    switch (alphabet_type)
    {
    case MN_SMS_DEFAULT_ALPHABET:
        msg_item_ptr->len = MMIAPICOM_Default2Wchar(
            (uint8 *)msg_arr,
            msg_item_ptr->context,
            (uint16)MIN(msg_len, msg_max_len)
            );
        break;
        
    case MN_SMS_8_BIT_ALPHBET:
        //8 Bit ��׼ Ascii �ַ���ʽ
        MMI_STRNTOWSTR(msg_item_ptr->context,msg_max_len,(uint8 *)msg_arr,msg_len,msg_len);
        msg_item_ptr->len = (uint16)MIN(msg_len, msg_max_len);
        break;
        
    case MN_SMS_UCS2_ALPHABET:
        {
            uint8 temp_arr[MN_CB_MACRO_MSG_LEN] = {0};

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
            SCI_MEMCPY(temp_arr, msg_arr, MIN(msg_len, MN_CB_MACRO_MSG_LEN));
#else
            GUI_UCS2L2UCS2B(temp_arr,
                                        MN_CB_MACRO_MSG_LEN,
                                        msg_arr,
                                        MIN(msg_len, MN_CB_MACRO_MSG_LEN)
                                        );
#endif

            //UCS2�ַ���ʽ
            msg_item_ptr->len = MIN( msg_len, msg_max_len );
            MMI_WSTRNCPY(
                msg_item_ptr->context,
                MN_CB_MACRO_MSG_LEN,
                (wchar *)temp_arr,
                msg_item_ptr->len/sizeof(wchar),
                msg_item_ptr->len/sizeof(wchar)
                );

            msg_item_ptr->len = msg_item_ptr->len/sizeof(wchar);
        }
        break;
        
    case MN_SMS_RESERVED_ALPHABET:
    default:
        // ����״̬
        // ��δ�����ʱ��������
        break;
    }
    
    return (msg_item_ptr->len);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         DisplayCBMsgContext
//  ��������:        ��ʾС���㲥��Ϣ����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void DisplayCBMsgContext(
                               MMI_CTRL_ID_T            control_id,            //IN: ������ʾ�Ŀؼ�ID
                               MMISMS_CB_MESSAGE_ITEM_T    *cb_msg_item_ptr    //IN/OUT: Ҫ��ʾ�Ĺ㲥��Ϣ�ṹ��ָ��
                               )
{
    MMI_STRING_T context = {0};
    uint16 len = 0;
    wchar time_str[CB_MSG_TIME_MAX_LEN + 1] = {0};
#ifdef MMI_ETWS_SUPPORT
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MMI_STRING_T title_str = {0};
    GUIRICHTEXT_ITEM_T  item_data = {0}; 
    uint16    index= 0;
    MMI_STRING_T etws_title = {0};
#endif

    SCI_TRACE_LOW("[MMISMSCB] DisplayCBMsgContext");

    if (PNULL == cb_msg_item_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:DisplayCBMsgContext cb_msg_item_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_1372_112_2_18_2_55_31_887,(uint8*)"");
        return;
    }

#ifdef MMI_ETWS_SUPPORT
    SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(cb_msg_item_ptr->dual_sys);

    if(TRUE==is_set_mcc)
    {
        plmn_info.mcc  = s_set_mcc;
    }

    if(CB_PWS_MCC == plmn_info.mcc)
    {
        title_str.wstr_ptr = SCI_ALLOCAZ(MMI_CB_CHANNEL_NAME_LEN * sizeof(wchar));
        SCI_MEMSET(title_str.wstr_ptr , 0, MMI_CB_CHANNEL_NAME_LEN* sizeof(wchar));	
        title_str.wstr_len = 0;

        current_channel_info = GetCMASChannelInfo(cb_msg_item_ptr->dual_sys, cb_msg_item_ptr->msg_id);

        if( NULL !=current_channel_info )
        {

            if( 0 != current_channel_info->title_string_id )
            {
                MMI_GetLabelTextByLang(current_channel_info->title_string_id , &etws_title);
                MMIAPICOM_Wstrncpy(title_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
                MMIAPICOM_Wstrncpy(title_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
                title_str.wstr_len = etws_title.wstr_len;
            }
            else
            {
                MMIAPICOM_Wstrncpy(title_str.wstr_ptr, current_channel_info->title_id, MMIAPICOM_Wstrlen(current_channel_info->title_id)) ;
                MMIAPICOM_Wstrncpy(title_str.wstr_ptr, current_channel_info->title_id, MMIAPICOM_Wstrlen(current_channel_info->title_id)) ;
                title_str.wstr_len = MMIAPICOM_Wstrlen(current_channel_info->title_id);	
            }

        }
        else
        {
            title_str.wstr_len = 0;		
        }
    }
#endif

    //��������ʾ�ؼ�����ʾ��С���㲥��Ϣ����
    SCI_MEMSET(time_str, 0, (CB_MSG_TIME_MAX_LEN + 1));

    //��������ʾ�ؼ�����ʾ�յ���С���㲥��Ϣ��ϵͳʱ��
    len = ConvertCBTimeToStrForDetails(
        cb_msg_item_ptr->date,
        cb_msg_item_ptr->time,
        //cb_msg_item_ptr->is_ucs2,
        time_str
        );

#ifdef MMI_ETWS_SUPPORT
    if( (CB_PWS_MCC == plmn_info.mcc ) && (title_str.wstr_len > 0) )
    {
        context.wstr_len = cb_msg_item_ptr->len + len + title_str.wstr_len + 1;
    }
    else if( (CB_CHILE_MCC == plmn_info.mcc) || (CB_IT_MCC == plmn_info.mcc ) || (CB_PE_MCC == plmn_info.mcc) )
    {
        context.wstr_len = cb_msg_item_ptr->len + 1;
    }
    else
#endif
    {
    context.wstr_len = cb_msg_item_ptr->len + len;
    }

    //������ʾ���ݻ�����
    context.wstr_ptr = SCI_ALLOC_APP((context.wstr_len + 1) * sizeof(wchar) );
    SCI_MEMSET(context.wstr_ptr, 0, ((context.wstr_len + 1) * sizeof(wchar)));

#ifdef MMI_ETWS_SUPPORT
    if( (CB_PWS_MCC == plmn_info.mcc ) && (title_str.wstr_len > 0) )
    {
        MMIAPICOM_Wstrncpy(context.wstr_ptr, title_str.wstr_ptr, title_str.wstr_len) ;
        MMIAPICOM_Wstrcat(context.wstr_ptr, L"\n");

    //��ϵͳʱ���ַ���������ʾ���ݻ�����
    MMI_WSTRNCPY(context.wstr_ptr + title_str.wstr_len + 1,
                            context.wstr_len,
                            time_str,
                            len,
                            len);

    //��С���㲥��Ϣ���ݿ�����ʾ���ݻ�����
    MMI_WSTRNCPY(context.wstr_ptr+ title_str.wstr_len + 1 + len,
                            cb_msg_item_ptr->len,
                            cb_msg_item_ptr->context,
                            cb_msg_item_ptr->len,
                            cb_msg_item_ptr->len);
    }
    else if( (CB_CHILE_MCC == plmn_info.mcc) || (CB_IT_MCC == plmn_info.mcc ) || (CB_PE_MCC == plmn_info.mcc) )
    {
        //��С���㲥��Ϣ���ݿ�����ʾ���ݻ�����
        MMI_WSTRNCPY(context.wstr_ptr,
                                cb_msg_item_ptr->len,
                                cb_msg_item_ptr->context,
                                cb_msg_item_ptr->len,
                                cb_msg_item_ptr->len);
    }
    else
#endif
    {
    //��ϵͳʱ���ַ���������ʾ���ݻ�����
    MMI_WSTRNCPY(context.wstr_ptr,
                            context.wstr_len,
                            time_str,
                            len,
                            len);

    //��С���㲥��Ϣ���ݿ�����ʾ���ݻ�����
    MMI_WSTRNCPY(context.wstr_ptr+len,
                            context.wstr_len-len,
                            cb_msg_item_ptr->context,
                            cb_msg_item_ptr->len,
                            cb_msg_item_ptr->len);
    }

#ifdef MMI_ETWS_SUPPORT
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;

    item_data.tag_type = GUIRICHTEXT_TAG_PARSE_URL | GUIRICHTEXT_TAG_PARSE_PHONENUM;
    item_data.text_data.buf.len = context.wstr_len;
    item_data.text_data.buf.str_ptr = context.wstr_ptr;
    item_data.text_set_font = TRUE;
    item_data.setting_font = CAF_FONT_18;

    GUIRICHTEXT_AddItem(control_id, &item_data, &index);

    GUIRICHTEXT_SetFocusMode(control_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
    GUIRICHTEXT_SetFocusItem(control_id, 0);
#else

    GUITEXT_SetString(control_id, context.wstr_ptr, context.wstr_len, FALSE);
#endif

    //����ǰ�Ķ���С���㲥��Ϊ�Ѷ�
    cb_msg_item_ptr->is_read = TRUE;

    //�ͷ���ʾ���ݻ�����
    if (PNULL != context.wstr_ptr)
    {
        SCI_FREE(context.wstr_ptr);
        context.wstr_ptr = PNULL;
    }
#ifdef MMI_ETWS_SUPPORT
    if (PNULL != title_str.wstr_ptr)
    {
        SCI_FREE(title_str.wstr_ptr);
        title_str.wstr_ptr = PNULL;
    }
#endif
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_Startup
//  ��������:        ����С���㲥�ӿں���
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MN_RETURN_SUCCESS �����ɹ���MN_RETURN_FAILURE ����ʧ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
MN_RETURN_RESULT_E MMISMSCB_Startup(
                                    MMISMS_CB_CHANNEL_T  *channel_list,    //С���㲥���ŵ��б�  (�������)
                                    MMISMS_CB_LANGUAGE_T *lang_list,        //С���㲥�������б�  (�������)
                                    MN_DUAL_SYS_E dual_sys
                                    )
{
    
    MN_CB_LANGUAGE_ID_MUSTER_T cb_language_muster = {0};
    MN_CB_MSG_ID_MUSTER_T cb_msg_muster = {0};

    SCI_TRACE_LOW("MMISMSCB_Startup");
    
    cb_msg_muster=ConvertChannelListToMsgIDMuster(channel_list);

    cb_language_muster=ConvertLangListToLangIDMuster(lang_list);
    
    return MNSMSCB_StartCbMsgEx(dual_sys, &cb_msg_muster,&cb_language_muster);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_Reset
//  ��������:        ����С���㲥�����ӿں���
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MN_RETURN_SUCCESS ����ɹ���MN_RETURN_FAILURE ����ʧ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
MN_RETURN_RESULT_E MMISMSCB_Reset(
                                  MMISMS_CB_CHANNEL_T  *channel_list,   //С���㲥���ŵ��б�  (�������)
                                  MMISMS_CB_LANGUAGE_T *lang_list,       //С���㲥�������б�  (�������)
                                  MN_DUAL_SYS_E dual_sys
                                  )
{
    
    MN_CB_LANGUAGE_ID_MUSTER_T cb_language_muster = {0};
    MN_CB_MSG_ID_MUSTER_T cb_msg_muster = {0};

    SCI_TRACE_LOW("MMISMSCB_Reset");
    
    cb_msg_muster=ConvertChannelListToMsgIDMuster(channel_list);
    
    cb_language_muster=ConvertLangListToLangIDMuster(lang_list);
    
    return MNSMSCB_ChangeCbParamEx(dual_sys, &cb_msg_muster,&cb_language_muster);
}


//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_Close
//  ��������:        �ر�С���㲥�ӿں���
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MN_RETURN_SUCCESS �رճɹ���MN_RETURN_FAILURE �ر�ʧ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
MN_RETURN_RESULT_E MMISMSCB_Close(void)
{
    return MNSMSCB_StopCbMsgEx(MMISMSCB_GetSelectCard());
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_Init
//  ��������:        ��ʼ��С���㲥�ӿں���(���ֻ���ʼ��ʱ����, MMIMAIN.c �е���)
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MN_RETURN_SUCCESS ��ʼ���ɹ���MN_RETURN_FAILURE ��ʼ��ʧ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
MN_RETURN_RESULT_E MMISMSCB_Init(MN_DUAL_SYS_E dual_sys)
{
    MMISMS_CB_CHANNEL_T channel_card_list = {0};
    MMISMS_CB_LANGUAGE_T lang_card_list = {0};
    BOOLEAN cb_card_enabled = FALSE;
    MN_RETURN_RESULT_E result = MN_RETURN_FAILURE;
    uint8 i = 0, j = 0;
    BOOLEAN is_save_cb_channels = FALSE;
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_CHANNEL_T etws_channel_list = {0};
    MMISMS_CB_CHANNEL_T* etws_channel_list_ptr = PNULL;
    uint16 etws_alert_num = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    uint16 nv_mcc = 0;
    NVITEM_ERROR_E nv_result = NVERR_NONE;
#endif	
#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
    int8 cell_info_result = MN_RETURN_FAILURE;
#endif
	
    SCI_TRACE_LOW("MMISMSCB_Init, dual_sys = %d", dual_sys);
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return result;
    }
    
#if defined(CB_TEST_CHILE) || defined(CB_TEST_CO) || defined(CB_TEST_EC) || defined(CB_TEST_GR) || defined(CB_TEST_IT) \
    || defined(CB_TEST_MX) || defined(CB_TEST_NZ) || defined(CB_TEST_PE) || defined(CB_TEST_PR) || defined(CB_TEST_PWS) \
    || defined(CB_TEST_RO) || defined(CB_TEST_UAE) || defined(CB_TEST_VE) || defined(CB_TEST_NONE)
    {
        is_set_mcc = TRUE;
    }
#else

    nv_result = EFS_NvitemRead(TELE_NV_SMSCB_MCC_SET, sizeof(uint16), (&nv_mcc) );
    if( NVERR_NONE == nv_result)
    {
        SCI_TRACE_LOW("EFS_NvitemRead(TELE_NV_SMSCB_MCC_SET) success, nv_mcc = %d", nv_mcc);
        if( 0 != nv_mcc)
        {
            is_set_mcc = TRUE;
            s_set_mcc = nv_mcc;
        }
    }
    else
    {
        SCI_TRACE_LOW("EFS_NvitemRead(TELE_NV_SMSCB_MCC_SET) fail, nv_result = %d", nv_result);
    }
#endif

    if(TRUE == s_is_etws_spec_changed)
    {
        if(MMK_IsFocusWin(MMISMS_CB_CHANNEL_OPT_WIN_ID))
        {
            MMK_CloseWin(MMISMS_CB_CHANNEL_OPT_WIN_ID);
        }
        if(MMK_IsOpenWin(MMISMS_CB_CHANNEL_LIST_WIN_ID))
        {
            MMK_CloseWin(MMISMS_CB_CHANNEL_LIST_WIN_ID);
        }
        if(MMK_IsOpenWin(MMISMS_CB_ADD_EDIT_CHANNELINDEX_WIN_ID))
        {
            MMK_CloseWin(MMISMS_CB_ADD_EDIT_CHANNELINDEX_WIN_ID);
        }
        if(MMK_IsOpenWin(MMISMS_CB_ADD_EDIT_CHANNELNAME_WIN_ID))
        {
            MMK_CloseWin(MMISMS_CB_ADD_EDIT_CHANNELNAME_WIN_ID);
        }
     }

    //��С���㲥������־Ϊ��������С���㲥
    cb_card_enabled= FALSE;
    SCI_MEMSET(&lang_card_list,0x00,sizeof(MMISMS_CB_LANGUAGE_T));
    SCI_MEMSET(&channel_card_list,0x00,sizeof(MMISMS_CB_CHANNEL_T));
	
    SCI_TRACE_LOW("MMISMSCB_Init");

#ifdef MMI_ETWS_SUPPORT
    cb_card_enabled = MMISMS_GetSmsCBEnabledFlag(dual_sys);
#else
    cb_card_enabled = NV_LoadSmsCBEnabledFlag(dual_sys);
#endif
	
#ifdef MMI_ETWS_SUPPORT
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

    if( TRUE == is_set_mcc )
    {
        plmn_info.mcc  = s_set_mcc;
    }
    if( CB_IT_MCC != plmn_info.mcc )
    {
        NV_LoadSmsCBLanguageList(dual_sys, &lang_card_list);
    }
    else
    {
        MMISMSCB_SetAllLangList(&lang_card_list);
    }
#else
    lang_card_list = NV_LoadSmsCBLanguageList(dual_sys); 
#endif

#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_card_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_card_list = MMISMS_NV_LoadSmsCBChannelList(dual_sys);
#endif

#ifdef MMI_ETWS_SUPPORT
    InitCB_ETWSChannel(dual_sys);
    etws_channel_list_ptr = MMISMSCB_Get_ETWS_Channel_Ptr(dual_sys);
    SCI_MEMCPY(&etws_channel_list, etws_channel_list_ptr, sizeof(MMISMS_CB_CHANNEL_T));
    etws_alert_num = MMIAPISMSCB_GetETWSNum();
#endif	

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
    s_cell_info_flag[dual_sys] = NV_LoadCellInfoEnabledFlag(dual_sys, &cell_info_result);
    SCI_TRACE_LOW("NV_LoadCellInfoEnabledFlag result = %d.",cell_info_result);
#endif

    MMISMSCB_SetSelectCard(dual_sys);
    SCI_TRACE_LOW("MMISMSCB_Init: cb_card_enabled = %d, channel_card_list.num = %d,dual_sys =%d..",cb_card_enabled, channel_card_list.num,dual_sys);
	
    is_save_cb_channels = FALSE;
	
#ifdef MMI_ETWS_SUPPORT
	//	if( (etws_alert_num >0 )
	//		&& (channel_card_list.num >= etws_alert_num) )
    if( etws_alert_num >0 )
    {
        if(channel_card_list.num >= etws_alert_num)
        {
            for(i = 0; i < channel_card_list.num; i++)
            {
                for(j = 0; j < etws_alert_num; j++)
                {
                    if(channel_card_list.channel_arr[i].msg_id == etws_channel_list.channel_arr[j].msg_id)
                    {
                        is_save_cb_channels = TRUE;
                        break;
                    }
                }
                if(is_save_cb_channels)
                {
                    break;
                }
            }
        }
    }
    else
#endif	
		if(channel_card_list.num >= MMI_CB_ALERT_NUM)
		{
			for(i = 0; i < channel_card_list.num; i++)
			{
				for(j = 0; j < MMI_CB_ALERT_NUM; j++)
				{
					if(channel_card_list.channel_arr[i].msg_id == MMIAPISMSCB_GetCBAlertID(j))
					{
						is_save_cb_channels = TRUE;
						break;
					}
				}
				if(is_save_cb_channels)
				{
					break;
				}
			}
		}
		
#ifdef MMI_ETWS_SUPPORT
    if(TRUE == s_is_etws_spec_changed)
    {
        SCI_TRACE_LOW("s_is_etws_spec_changed = TURE reset all nv data first , dual_sys = %d", dual_sys);
        is_save_cb_channels = FALSE;
#ifdef MMI_ETWS_SUPPORT
        SCI_MEMSET(&s_save_to_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T));
        SCI_MEMCPY(&s_save_to_nv_channel_list,&etws_channel_list, sizeof(MMISMS_CB_CHANNEL_T));
        NV_SaveSmsCBChannelList();
#else
        NV_SaveSmsCBChannelList(&etws_channel_list);
#endif
        s_is_etws_spec_changed = FALSE;
    }
#endif

    if(!is_save_cb_channels)
    {
        SCI_MEMSET(&channel_card_list,0x00,sizeof(MMISMS_CB_CHANNEL_T));
#ifdef MMI_ETWS_SUPPORT
        MMISMSCB_SaveCBEmergenceChannel(dual_sys);
        NV_LoadSmsCBChannelList();
        SCI_MEMCPY(&channel_card_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
        for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
            MMISMSCB_SaveCBEmergenceChannel(i);
        channel_card_list = MMISMS_NV_LoadSmsCBChannelList(dual_sys);
#endif
    }

		if(cb_card_enabled)
		{
			for(i = 0; i < channel_card_list.num; i++)
			{
				SCI_TRACE_LOW("MMISMSCB_Startup msg_id[%d] = %d.is_enable = %d.", i, channel_card_list.channel_arr[i].msg_id, channel_card_list.channel_arr[i].is_enabled);
			}
			MMISMSCB_Startup(&channel_card_list, &lang_card_list, dual_sys);
			result = MMISMSCB_Reset(&channel_card_list, &lang_card_list, dual_sys);    //�����趨С���㲥����
		}   
		else
		{
			SCI_TRACE_LOW("TYL CLOSE MMISMSCB_CLOSE");
			result = MMISMSCB_Close();
		}
		
		//SCI_TRACE_LOW:"MMISMSCB_Init result[%d] = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_1520_112_2_18_2_55_31_888,(uint8*)"dd",dual_sys,result);
		
		return result; 
}

/******************************************************************************************************************/
//
//
//                        С���㲥ģ�鹦�ܴ���������
//
//
/******************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         AppendCBChannelListItem
//  ��������:        ����С���㲥�ŵ��б����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//------- --------------------------------------------------------------------------------------------------------
#ifdef MMI_PDA_SUPPORT
LOCAL void AppendCBChannelListItem(
                                   MMI_CTRL_ID_T          ctrl_id,       //Ҫ���õ��б�ؼ�ID�� (�������)
                                   MMISMS_CB_CHANNEL_ITEM_T  channel_item,   //�趨���ŵ��б���ṹ (�������)
                                   uint16                    pos
                                   )
{
    uint8 szIndex[20] = {0};
    //uint8 szTemp[15] = {0};
    wchar szIndex_wstr[20] = {0};
    //wchar szIndex_name[20] = {0};
    int index_len = 0; 
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;/*lint !e64*/

    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    
    if (channel_item.is_enabled)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }
    
    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = image_id;

    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    if( channel_item.name.wstr_len > 0 )
    {
        item_data.item_content[0].item_data.text_buffer.wstr_len = channel_item.name.wstr_len;    
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = channel_item.name.wstr;
    }
    else
    {    
        SCI_MEMSET(szIndex, 0, sizeof(szIndex));
        sprintf((char*) szIndex,"%d",channel_item.msg_id );
        index_len = strlen((char*)szIndex);        
        GUI_UTF8ToWstr(szIndex_wstr, sizeof(szIndex_wstr) / sizeof(wchar), szIndex, index_len);        
        item_data.item_content[0].item_data.text_buffer.wstr_len = MIN( index_len, GUILIST_STRING_MAX_NUM );        
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = szIndex_wstr;        
    }

            
    SCI_MEMSET(szIndex, 0, sizeof(szIndex));
    sprintf((char*) szIndex,"%d", channel_item.msg_id );
    index_len = strlen((char*)szIndex);
    GUI_UTF8ToWstr(szIndex_wstr, sizeof(szIndex_wstr) / sizeof(wchar), szIndex, index_len);
    
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer.wstr_len = MIN( index_len, GUILIST_STRING_MAX_NUM );    
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = szIndex_wstr;
       
    GUILIST_AppendItem( ctrl_id, &item_t );    
}
#else

LOCAL void AppendCBChannelListItem(
                                   MMI_CTRL_ID_T          ctrl_id,       //Ҫ���õ��б�ؼ�ID�� (�������)
                                   MMISMS_CB_CHANNEL_ITEM_T  channel_item,   //�趨���ŵ��б���ṹ (�������)
                                   uint16                    pos
                                   )
{
    uint8 szIndex[8] = {0};
    wchar szIndex_wstr[8] = {0};
    int index_len = 0; 
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;/*lint !e64*/
#ifdef MMI_ETWS_SUPPORT
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
#endif

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;

    if (channel_item.is_enabled)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }

    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = image_id;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    if( channel_item.name.wstr_len > 0 )
    {
        item_data.item_content[0].item_data.text_buffer.wstr_len = channel_item.name.wstr_len;
        
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = channel_item.name.wstr;
//         MMI_WSTRNCPY( item_data.item_content[1].item_data.text_buffer.wstr,
//             MMI_CB_CHANNEL_NAME_LEN,
//             channel_item.name.wstr,
//             item_data.item_content[1].item_data.text_buffer.wstr_len,
//             item_data.item_content[1].item_data.text_buffer.wstr_len);
    }
    else
    {
        SCI_MEMSET(szIndex, 0, 8);
        sprintf((char*) szIndex,"%d", channel_item.msg_id );
        index_len = strlen((char*)szIndex);
        
        item_data.item_content[0].item_data.text_buffer.wstr_len = MIN( index_len, GUILIST_STRING_MAX_NUM );
        
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = szIndex_wstr;

        MMI_STRNTOWSTR( szIndex_wstr,
            GUILIST_STRING_MAX_NUM,
            (uint8 *)szIndex,
            item_data.item_content[0].item_data.text_buffer.wstr_len,
            item_data.item_content[0].item_data.text_buffer.wstr_len);
    }

    //���ù������
    item_data.softkey_id[0] = STXT_OPTION;

#ifdef MMI_ETWS_SUPPORT
    current_channel_info = GetCMASChannelInfo(dual_sys, channel_item.msg_id);
    if( NULL != current_channel_info)
    {
        SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

        if(TRUE==is_set_mcc)
        {
            plmn_info.mcc  = s_set_mcc;
        }

        SCI_TraceLow("mmismscb AppendCBChannelListItem: plmn_info.mcc=%d,channel id=%d",plmn_info.mcc,current_channel_info->channel_id);

        if(( CB_RO_MCC == plmn_info.mcc) && (current_channel_info->channel_id < 1000)) // RO_alert channel 919,918,917,916 will not be displayed
            return;
        
        if( current_channel_info->can_be_disabled)
        {
            if (channel_item.is_enabled)
            {
                item_data.softkey_id[1] = STR_CB_SOFTKEY_DISABLE;
            }
            else
            {
                item_data.softkey_id[1] = STR_CB_SOFTKEY_ENABLE;
            }
        }
        else
        {
//            if( CB_UAE_MCC == plmn_info.mcc)//UAE can not be disabled should not need to display
//                return;

            if( CB_NZ_MCC == plmn_info.mcc)
            {
                item_data.item_content[0].is_opacity = FALSE;
                item_data.item_content[0].custom_font_color = MMI_GRAY_COLOR;
                item_data.item_content[0].custom_font_color_focus = MMI_GRAY_COLOR;
                item_data.item_content[0].is_custom_font_color = TRUE;
            }
            item_data.softkey_id[1] = TXT_NULL;
        }
    }
    else
#endif
    {
        if (channel_item.is_enabled)
        {
            item_data.softkey_id[1] = STR_CB_SOFTKEY_DISABLE;
        }
        else
        {
            item_data.softkey_id[1] = STR_CB_SOFTKEY_ENABLE;
        }
    }
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;

    GUILIST_AppendItem( ctrl_id, &item_t );
}
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : Get Small Icon by sim index
//  Global resource dependence : none
//  Author: 
//  Note: for search module and delete by num
/***************************************************************************/
LOCAL MMI_IMAGE_ID_T GetSimSmallIcon(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }

    return (MMI_IMAGE_ID_T)(IMAGE_SMS_SIM_CARD1+(uint32)dual_sys);
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         AppendCBMsgListItem
//  ��������:        ���ù㲥��Ϣ�ռ����б����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AppendCBMsgListItem(
                                MMI_CTRL_ID_T             ctrl_id,         //Ҫ���õ��б�ؼ�ID�� (�������)
                                MMISMS_CB_CHANNEL_T       *channel_list,
                                BOOLEAN                   is_read,
                                MMISMS_CB_MESSAGE_ITEM_T * cb_msg_ptr
                                )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMISMS_CB_CHANNEL_NAME_T channel_name = {0};
    uint8 channel_str[MMI_CB_CHANNEL_NAME_LEN] = {0};
    uint16 channel_len = 0;
    uint16 list_index = 0;
    MMI_STRING_T subject = {0};
    MMI_STRING_T str_time = {0};
    wchar temp_arr1[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    wchar temp_arr2[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    SCI_DATE_T cur_date = {0};
    uint8 time_string[MMIMESSAGE_DIAPLAY_LEN] = {0};
    MN_CB_MSG_ID_T msg_id = cb_msg_ptr->msg_id;
    MN_DUAL_SYS_E dual_sys = cb_msg_ptr->dual_sys;
    uint16 len = cb_msg_ptr->len;
    wchar *msg_ptr = cb_msg_ptr->context;
    SCI_DATE_T date = cb_msg_ptr->date;
    SCI_TIME_T time = cb_msg_ptr->time;
#ifdef MMI_ETWS_SUPPORT
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    CMAS_CHANNEL_INFO_T* current_channel_table = PNULL;
    uint16 i = 0;
    BOOLEAN is_exist_title_name = FALSE;
    MMI_STRING_T etws_channel = {0};
    MMI_STRING_T etws_popup_title = {0};
#endif
    subject.wstr_ptr = temp_arr1;
    str_time.wstr_ptr = temp_arr2;

#ifdef MMI_ETWS_SUPPORT
    current_channel_table = GetCMASChannelTable(cb_msg_ptr->dual_sys);

    SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(cb_msg_ptr->dual_sys);
	
    if(TRUE==is_set_mcc)
    {
        plmn_info.mcc  = s_set_mcc;
    }

    if( PNULL != current_channel_table)
    {
        if( CB_CHILE_MCC == plmn_info.mcc) 
        {
            do
            {
                if( msg_id == current_channel_table[i].channel_id )
                {
                    if( 0 != current_channel_table[i].popup_string_id )
                    {
                        is_exist_title_name = TRUE;
                        MMI_GetLabelTextByLang(current_channel_table[i].popup_string_id, &etws_popup_title);
                        MMIAPICOM_Wstrncpy(channel_name.wstr, etws_popup_title.wstr_ptr, etws_popup_title.wstr_len) ;
                        channel_name.wstr_len = etws_popup_title.wstr_len;
                    }
                    break;
                }
                else
                {
                    i++;
                }
            }
            while(0 != current_channel_table[i].channel_id);
        }
        else
        {
            do
            {	
                if( msg_id == current_channel_table[i].channel_id )
                {
                    if( 0 != current_channel_table[i].title_string_id )
                    {
                        is_exist_title_name = TRUE;
                        MMI_GetLabelTextByLang(current_channel_table[i].title_string_id, &etws_channel);
                        MMIAPICOM_Wstrncpy(channel_name.wstr, etws_channel.wstr_ptr, etws_channel.wstr_len) ;
                        channel_name.wstr_len = etws_channel.wstr_len;
                    }
                    else
                    {
                        is_exist_title_name = TRUE;
                        MMIAPICOM_Wstrncpy(channel_name.wstr, current_channel_table[i].title_id, MMIAPICOM_Wstrlen(current_channel_table[i].title_id)) ;
                        channel_name.wstr_len = MMIAPICOM_Wstrlen(current_channel_table[i].title_id) +1;
                    }
                    break;
                }
                else
                {
                    i++;
                }
            }
            while(0 != current_channel_table[i].channel_id);
        }
    }
#endif
		
#ifdef MMI_ETWS_SUPPORT
    if( FALSE == is_exist_title_name )
#endif		
    channel_name = MMISMS_GetCBChannelNameByMsgId( channel_list, msg_id );

    if ( channel_name.wstr_len == 0 )
    {
        channel_len = sprintf((char *)channel_str, 
            "%d", 
            msg_id
            );

        MMI_STRNTOWSTR(channel_name.wstr,
                                MMI_CB_CHANNEL_NAME_LEN,
                                (uint8 *)channel_str,
                                MMI_CB_CHANNEL_NAME_LEN,
                                channel_len
                                );

        channel_name.wstr_len = channel_len;
    }

    item_t.item_style = GUIITEM_STYLE_TWO_LINE_2TEXT_AND_TEXT_MS;//GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL;

    item_t.item_data_ptr = &item_data;
    
    //item_data.item_content[list_index].item_data_type = GUIITEM_DATA_IMAGE_ID;

    //read or unread icon
    /*if ( is_read)
    {
        item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_READ_SMS_ICON;
    }
    else
    {
        item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_NOT_READ_SMS_ICON;
    }*/

#ifndef MMI_MULTI_SIM_SYS_SINGLE  
    item_data.item_content[list_index].icon_id = GetSimSmallIcon(dual_sys);
#endif

    //list_index++;

    //channel name
    item_data.item_content[list_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = channel_name.wstr_len;
    
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = channel_name.wstr;

    list_index++;

    //set time
    TM_GetSysDate(&cur_date);
    if ((date.mday == cur_date.mday)
        && (date.mon == cur_date.mon) 
        && (date.year == cur_date.year))
    {
        str_time.wstr_len = sprintf((char *)time_string, "%d:%02d", time.hour, time.min);
    }
    else
    {
        str_time.wstr_len = sprintf((char *)time_string, "%d/%d", date.mon, date.mday);
    }

    str_time.wstr_len = MIN(str_time.wstr_len,MMIMESSAGE_DIAPLAY_LEN);

    MMI_STRNTOWSTR( str_time.wstr_ptr,
                    MMIMESSAGE_DIAPLAY_LEN,
                    (uint8 *)time_string,
                    str_time.wstr_len,
                    str_time.wstr_len
                    );

    item_data.item_content[list_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = str_time.wstr_len;

    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = str_time.wstr_ptr;

    list_index++;

    //set subject
    subject.wstr_len  = MIN(len, MMIMESSAGE_DIAPLAY_LEN);

    MMI_WSTRNCPY(
        subject.wstr_ptr,
        MMIMESSAGE_DIAPLAY_LEN,
        msg_ptr, 
        subject.wstr_len,
        subject.wstr_len
        );

    item_data.item_content[list_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = subject.wstr_len;
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = subject.wstr_ptr;
    
    /*//���ù������
#ifndef MMI_GUI_STYLE_TYPICAL
    item_data.softkey_id[0] = STXT_OPTION;
#else
    item_data.softkey_id[0] = TXT_COMMON_FORWARD;
#endif
    item_data.softkey_id[1] = TXT_READ;
    item_data.softkey_id[2] = STXT_RETURN;*/

    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;

    GUILIST_AppendItem( ctrl_id, &item_t );
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         SetCBLanguageListItemTitle
//  ��������:        ���ù㲥�����б�����⺯��
//  ȫ�ֱ���ʹ��:    g_cb_lang_text_arr
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
void SetCBLanguageListItemTitle(
                                MMI_CTRL_ID_T  ctrl_id        //Ҫ���õ��б�ؼ�ID�� (�������)
                                )
{
    uint32 i = 0;    
    
    //�趨MMI_MAX_CB_CHANNEL_NUM��ѡ��
    GUILIST_SetMaxItem( ctrl_id, MMI_MAX_CB_LANGUAGE_NUM, FALSE );//max item 11
    
    //���"ȫ������"һ��
    MMIAPISET_AppendListItemByTextId( STR_CB_LANG_ALL, STXT_SOFTKEY_SELECT_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_CHECK );

    for( i = 0; i < MMI_MAX_CB_LANGUAGE_NUM - 1; i++ )
    {
        MMIAPISET_AppendListItemByTextId( g_cb_lang_text_arr[i], STXT_SOFTKEY_SELECT_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_CHECK );
    }
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         AddCBChannelListItem
//  ��������:        С���㲥������ŵ����ŵ��б���
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MN_RETURN_SUCCESS ��ӳɹ���MN_RETURN_FAILURE ���ʧ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MN_RETURN_RESULT_E AddCBChannelListItem(
                                              MMI_STRING_T index_str, //Ҫ��ӵĹ㲥�ŵ������ַ��� (�������)
                                              MMI_STRING_T name_str   //Ҫ��ӵĹ㲥�ŵ������ַ��� (�������)
                                              )
{
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMISMS_CB_LANGUAGE_T lang_list = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    uint16 index_len = index_str.wstr_len;
    uint16 name_len = name_str.wstr_len;
    uint32 channel_index = 0;
#ifdef MMI_ETWS_SUPPORT
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
#endif
    //��NV��ȡ�ŵ��б�������б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list=MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif

#ifdef MMI_ETWS_SUPPORT
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

    if(TRUE==is_set_mcc)
    {
        plmn_info.mcc  = s_set_mcc;
    }
#endif

#ifdef MMI_ETWS_SUPPORT
    //��NV��ȡ�ŵ��б�������б�
    if( CB_IT_MCC != plmn_info.mcc )
    {
        NV_LoadSmsCBLanguageList(dual_sys, &lang_list);
    }
    else
    {
        MMISMSCB_SetAllLangList(&lang_list);
    }
#else
    lang_list=NV_LoadSmsCBLanguageList(MMISMSCB_GetSelectCard());
#endif

    //�ŵ��б����������һ
    channel_list.num++;

    channel_list.channel_arr[channel_list.num-1].msg_id=0;
    //���ŵ������༭���е��ַ���תΪ�ŵ�ID��
    MMIAPICOM_WstrToNum(index_str.wstr_ptr, (uint8)index_len,&channel_index);
    channel_list.channel_arr[channel_list.num-1].msg_id = channel_index;

    MMI_WSTRNCPY(channel_list.channel_arr[channel_list.num-1].name.wstr,
        sizeof(channel_list.channel_arr[channel_list.num-1].name.wstr),
        name_str.wstr_ptr,
        name_len,
        name_len );
    channel_list.channel_arr[channel_list.num-1].name.wstr_len=name_len;
    channel_list.channel_arr[channel_list.num-1].is_enabled=TRUE;

    //����㲥��������
    SCI_TRACE_LOW("AddCBChannelListItem");
    return_val=MMISMSCB_Reset(&channel_list, &lang_list,MMISMSCB_GetSelectCard());

    if(return_val==MN_RETURN_SUCCESS)               //��������㲥�������óɹ��򱣴�༭����ŵ��б�NV��ȥ
    {
#ifdef MMI_ETWS_SUPPORT
        SCI_MEMSET(&s_save_to_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T));
        SCI_MEMCPY(&s_save_to_nv_channel_list,&channel_list, sizeof(MMISMS_CB_CHANNEL_T));
        NV_SaveSmsCBChannelList();
#else
        NV_SaveSmsCBChannelList(&channel_list);
#endif
    }

    return return_val;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         EditCBChannelListItem
//  ��������:        С���㲥�༭�ŵ��б���ĳһ��
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MN_RETURN_SUCCESS �༭�ɹ���MN_RETURN_FAILURE �༭ʧ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MN_RETURN_RESULT_E EditCBChannelListItem(
                                               MMI_STRING_T index_str,  //���ĵĹ㲥�ŵ������ַ��� (�������)
                                               MMI_STRING_T name_str,   //���ĵĹ㲥�ŵ������ַ��� (�������)
                                               uint16 pos                         //���ĵ��ŵ��б���λ��     (�������)
                                               )
{
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMISMS_CB_LANGUAGE_T lang_list = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    uint16 index_len = index_str.wstr_len;
    uint16 name_len = name_str.wstr_len;
    uint32 channel_index = 0;
    uint16 i = 0;
    BOOLEAN is_50_channel_exist = FALSE;
#ifdef MMI_ETWS_SUPPORT
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
#endif

#ifdef MMI_ETWS_SUPPORT
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

    if(TRUE==is_set_mcc)
    {
        plmn_info.mcc  = s_set_mcc;
    }
#endif

#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));

    //��NV��ȡ�ŵ��б�������б�
    if( CB_IT_MCC != plmn_info.mcc )
    {
        NV_LoadSmsCBLanguageList(dual_sys, &lang_list);
    }
    else
    {
        MMISMSCB_SetAllLangList(&lang_list);
    }
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
    lang_list=NV_LoadSmsCBLanguageList(MMISMSCB_GetSelectCard());
#endif

    channel_list.channel_arr[pos].msg_id=0;
    //���ŵ������༭���е��ַ���תΪ�ŵ�ID��
    MMIAPICOM_WstrToNum(index_str.wstr_ptr, (uint8)index_len,&channel_index);
    channel_list.channel_arr[pos].msg_id = channel_index;

    //���ŵ����Ʊ༭���е��ַ���תΪ�ŵ��ַ�
    MMI_WSTRNCPY(channel_list.channel_arr[pos].name.wstr,
                sizeof(channel_list.channel_arr[pos].name.wstr),
                name_str.wstr_ptr,
                name_len,
                name_len );
    channel_list.channel_arr[pos].name.wstr_len=name_len;

    //����㲥��������
    return_val=MMISMSCB_Reset(&channel_list, &lang_list,MMISMSCB_GetSelectCard());

    if(return_val==MN_RETURN_SUCCESS)               //��������㲥�������óɹ��򱣴�༭����ŵ��б�NV��ȥ
    {
#ifdef MMI_ETWS_SUPPORT
        SCI_MEMSET(&s_save_to_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T));
        SCI_MEMCPY(&s_save_to_nv_channel_list,&channel_list, sizeof(MMISMS_CB_CHANNEL_T));
        NV_SaveSmsCBChannelList();
#else
        NV_SaveSmsCBChannelList(&channel_list);
#endif
#ifdef CB_CHANNEL_50_EXIST
        for(i = 0; i < channel_list.num; i++)
        {
            if(MMI_PER_INSTALL_PARAM == channel_list.channel_arr[i].msg_id)
            {
                is_50_channel_exist = TRUE;
                break;
            }
        }
        if(!is_50_channel_exist)
        {
            NV_SaveSmsCB50ChannelFlag(FALSE);
        }
#endif
    }

    return return_val;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         DeleteCBChannelListItem
//  ��������:        С���㲥ɾ���ŵ��б���ĳһ��
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MN_RETURN_SUCCESS ɾ���ɹ���MN_RETURN_FAILURE ɾ��ʧ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MN_RETURN_RESULT_E DeleteCBChannelListItem(
                                                 uint16 pos       //ɾ�����ŵ��б���λ��     (�������)
                                                 )
{
    uint16  i = 0;
    uint16 temp_pos = 0;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMISMS_CB_LANGUAGE_T lang_list = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
#ifdef MMI_ETWS_SUPPORT
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    uint16 etws_alert_num = 0;
    etws_alert_num = MMIAPISMSCB_GetETWSNum();
#endif

    //��NV��ȡ�ŵ��б�������б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif

#ifdef MMI_ETWS_SUPPORT
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(MMISMSCB_GetSelectCard());

    if(TRUE==is_set_mcc)
    {
        plmn_info.mcc  = s_set_mcc;
    }

    if( CB_IT_MCC != plmn_info.mcc )
    {
        NV_LoadSmsCBLanguageList(MMISMSCB_GetSelectCard(), &lang_list);
    }
    else
    {
        MMISMSCB_SetAllLangList(&lang_list);
    }
#else
    lang_list = NV_LoadSmsCBLanguageList(MMISMSCB_GetSelectCard());
#endif

    if(pos > (channel_list.num - 1))
    {
        return MN_RETURN_FAILURE;
    }
#ifdef MMI_ETWS_SUPPORT
    if( etws_alert_num !=0 )
    {
        temp_pos = pos;
    }
    else
#endif
    {
        temp_pos = pos + MMI_CB_ALERT_NUM;
    }

#ifdef CB_CHANNEL_50_EXIST
    if(MMI_PER_INSTALL_PARAM == channel_list.channel_arr[temp_pos].msg_id)
    {
        NV_SaveSmsCB50ChannelFlag(FALSE);
    }
#endif

    SCI_TRACE_LOW("DeleteCBChannelListItem pos = %d.",pos);
    for(i = 0; i < channel_list.num;i++)
        SCI_TRACE_LOW("DeleteCBChannelListItem channel_list[%d] = %d.",i,channel_list.channel_arr[i].msg_id);

    //��ɾ��λ���Ժ��������ǰ��
    for(i=temp_pos;i<channel_list.num-1;i++)
    {
        channel_list.channel_arr[i]=channel_list.channel_arr[i+1];
    }

    //�ŵ�������һ
    channel_list.num--;
	
#ifdef MMI_ETWS_SUPPORT
    if( 0 == etws_alert_num  )
#endif
    {
        if(MMI_CB_ALERT_NUM == channel_list.num)
        {
            GUIWIN_SetSoftkeyTextId(MMISMS_CB_CHANNEL_LIST_WIN_ID, STXT_SOFTKEY_MENU_MK, TXT_NULL, TXT_COMMON_COMMON_BACK, TRUE);
            GUIWIN_SeSoftkeytButtonTextId(MMISMS_CB_CHANNEL_LIST_WIN_ID, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
            GUIWIN_SeSoftkeytButtonTextId(MMISMS_CB_CHANNEL_LIST_WIN_ID, STXT_SOFTKEY_MENU_MK, LEFT_BUTTON, TRUE);
        }
    }

    //����㲥��������
    return_val = MMISMSCB_Reset(&channel_list, &lang_list,MMISMSCB_GetSelectCard());

    if(return_val == MN_RETURN_SUCCESS)               //��������㲥�������óɹ��򱣴�༭����ŵ��б�NV��ȥ
    {
#ifdef MMI_ETWS_SUPPORT
        SCI_MEMSET(&s_save_to_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T));
        SCI_MEMCPY(&s_save_to_nv_channel_list,&channel_list, sizeof(MMISMS_CB_CHANNEL_T));
        NV_SaveSmsCBChannelList();
#else
        NV_SaveSmsCBChannelList(&channel_list);
#endif
    }

    return return_val;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         IsEditChannelIndexRepeat
//  ��������:        �жϵ�ǰ�޸ĵ��ŵ������Ƿ��ظ�
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          TRUE �ظ���FALSE ���ظ�
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
BOOLEAN IsEditChannelIndexRepeat(
                                 uint16 pos,                               //�ŵ��б���λ��     (�������)
                                 MMI_STRING_T index_str         //�ŵ������ַ���     (�������)
                                 )
{

    uint16 index_len = index_str.wstr_len;
    uint32 channel_index = 0;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    uint32 i = 0;

    //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif

    //���ŵ������༭���е��ַ���תΪ�ŵ�ID��
    MMIAPICOM_WstrToNum(index_str.wstr_ptr, (uint8)index_len,&channel_index);

    for(i=0;i<channel_list.num;i++)
    {
        //NV�����е��ŵ��б���������޴�����
        if(channel_list.channel_arr[i].msg_id==channel_index && i!=pos)   
        {
            return TRUE;
        }
    }

    return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         IsAddChannelIndexRepeat
//  ��������:        �жϵ�ǰ��ӵ��ŵ������Ƿ��ظ�
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          TRUE �ظ���FALSE ���ظ�
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
BOOLEAN IsAddChannelIndexRepeat(
                                MMI_STRING_T  index_str    //�����жϵ��ŵ������ַ��� (�������)
                                )
{
    
    uint16 index_len = index_str.wstr_len;
    uint32 channel_index= 0;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    uint32 i = 0;

    //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif

    //���ŵ������༭���е��ַ���תΪ�ŵ�ID��
    MMIAPICOM_WstrToNum(index_str.wstr_ptr, (uint8)index_len,&channel_index);
    
    for(i=0;i<channel_list.num;i++)
    {
        //NV�����е��ŵ��б��޴�����
        if(channel_list.channel_arr[i].msg_id==channel_index)
        {
            return TRUE;
        }
    }

    return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_DirectDisplayNewCBMsg
//  ��������:        ������ʾ�յ�����С���㲥����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSCB_DirectDisplayNewCBMsg(
                                 APP_MN_SMSCB_MSG_IND_T *cb_msg_ptr    //�㲥��Ϣ�ź�ָ��  (�������)

                                 )
{ 
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMISMS_CB_MESSAGE_ITEM_T* cb_msg_item = {0};
    MMISMS_CB_CHANNEL_NAME_T msg_channel_name = {0};
    uint16 msg_real_len = 0;
    
    if (PNULL == cb_msg_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMSCB_DirectDisplayNewCBMsg cb_msg_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2361_112_2_18_2_55_33_897,(uint8*)"");
        return;
    }    

    cb_msg_item = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
    if (PNULL == cb_msg_item)
    {
        return;
    }

    cb_msg_item->msg_id=cb_msg_ptr->msg_id;
    if (cb_msg_ptr->dcs.alphabet_type == MN_SMS_UCS2_ALPHABET)
    {
        msg_real_len = MMIAPICOM_Wstrlen((const wchar*)cb_msg_ptr->msg_arr) * sizeof(wchar);
    } 
    else
    {
        msg_real_len = strlen((char *)cb_msg_ptr->msg_arr);
    }
    cb_msg_item->len=MIN(cb_msg_ptr->msg_len, msg_real_len);
    cb_msg_item->is_read=FALSE;
    cb_msg_item->dual_sys = cb_msg_ptr->dual_sys;
    
    //���յ��Ĺ㲥��Ϣ�����ݴ�����Ϣ���ݽṹ��ȥ
    ConvertCBMsgContextToMsgItem(cb_msg_ptr->msg_arr,MIN(cb_msg_ptr->msg_len, msg_real_len),
        cb_msg_ptr->dcs.alphabet_type,FALSE,cb_msg_item);
    
    //��ý��ܵ��㲥ʱ��ϵͳ���ں�ʱ��
    TM_GetSysTime(&cb_msg_item->time);
    TM_GetSysDate(&cb_msg_item->date);
    
    //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[cb_msg_item->dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(cb_msg_item->dual_sys);
#endif

    //�����Ӧ���ŵ�����
    msg_channel_name = MMISMS_GetCBChannelNameByMsgId(&channel_list,cb_msg_item->msg_id);
    
    if( msg_channel_name.wstr_len == 0 )
    {
        uint8 channel_name[MMI_CB_CHANNEL_NAME_LEN] = {0};

        sprintf((char*) channel_name, "%d" , cb_msg_item->msg_id );

        msg_channel_name.wstr_len = strlen((char *)channel_name);

        MMI_STRNTOWSTR(msg_channel_name.wstr,
                                    MMI_CB_CHANNEL_NAME_LEN,
                                    (uint8 *)channel_name,
                                    MMI_CB_CHANNEL_NAME_LEN,
                                    msg_channel_name.wstr_len );
    }
    
    
    //�����ڵı�����Ϊ�ŵ�����
    GUIWIN_SetTitleText(MMISMS_CB_CLASS0_READ_WIN_ID,msg_channel_name.wstr,
        msg_channel_name.wstr_len, FALSE );
    
    MMK_SetWinUserData(MMK_ConvertIdToHandle(MMISMS_CB_CLASS0_READ_WIN_ID), (ADD_DATA)cb_msg_item);

    //��ʾ�㲥��Ϣ����
    DisplayCBMsgContext(MMISMS_CB_CLASS0_SHOW_MSG_TEXTBOX_CTRL_ID,cb_msg_item);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_DirectDisplayNewMarcoCBMsg
//  ��������:        ������ʾ�յ�����С���㲥����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSCB_DirectDisplayNewMarcoCBMsg(
                                 APP_MN_SMSCB_MACRO_MSG_IND_T *cb_msg_ptr    //�㲥��Ϣ�ź�ָ��  (�������)
                                 )
{ 
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMISMS_CB_MESSAGE_ITEM_T* cb_msg_item = {0};
    MMISMS_CB_CHANNEL_NAME_T msg_channel_name = {0};
    uint32 msg_real_len = 0;
    
    if (PNULL == cb_msg_ptr)
    {
        //SCI_TRACE_LOW:"MMISMSCB:MMISMS_DirectDisplayNewMarcoCBMsg cb_msg_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2427_112_2_18_2_55_33_898,(uint8*)"");
        return;
    }

    cb_msg_item = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
    if (PNULL == cb_msg_item)
    {
        return;
    }
    
    cb_msg_item->msg_id=cb_msg_ptr->msg_id;
    if (cb_msg_ptr->dcs.alphabet_type == MN_SMS_UCS2_ALPHABET)
    {
        msg_real_len = MMIAPICOM_Wstrlen((const wchar*)cb_msg_ptr->msg_arr) * sizeof(wchar);
    } 
    else
    {
        msg_real_len = strlen((char *)cb_msg_ptr->msg_arr);
    }
    cb_msg_item->len=MIN(cb_msg_ptr->msg_len, msg_real_len);
    cb_msg_item->is_read=FALSE;
    cb_msg_item->dual_sys = cb_msg_ptr->dual_sys;
    
    //���յ��Ĺ㲥��Ϣ�����ݴ�����Ϣ���ݽṹ��ȥ
    ConvertCBMsgContextToMsgItem(cb_msg_ptr->msg_arr,MIN(cb_msg_ptr->msg_len, msg_real_len),
        cb_msg_ptr->dcs.alphabet_type,TRUE,cb_msg_item);
    
    //��ý��ܵ��㲥ʱ��ϵͳ���ں�ʱ��
    TM_GetSysTime(&cb_msg_item->time);
    TM_GetSysDate(&cb_msg_item->date);
    
    //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[cb_msg_item->dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(cb_msg_item->dual_sys);
#endif

    //�����Ӧ���ŵ�����
    msg_channel_name = MMISMS_GetCBChannelNameByMsgId(&channel_list,cb_msg_item->msg_id);

    if( msg_channel_name.wstr_len == 0 )
    {
        uint8 channel_name[MMI_CB_CHANNEL_NAME_LEN] = {0};

        sprintf((char*) channel_name, "%d" , cb_msg_item->msg_id );

        msg_channel_name.wstr_len = strlen((char *)channel_name);

        MMI_STRNTOWSTR(msg_channel_name.wstr,
                                    MMI_CB_CHANNEL_NAME_LEN,
                                    (uint8 *)channel_name,
                                    MMI_CB_CHANNEL_NAME_LEN,
                                    msg_channel_name.wstr_len );
    }


    //�����ڵı�����Ϊ�ŵ�����
    GUIWIN_SetTitleText(MMISMS_CB_CLASS0_READ_WIN_ID,msg_channel_name.wstr,
        msg_channel_name.wstr_len, FALSE );

    MMK_SetWinUserData(MMK_ConvertIdToHandle(MMISMS_CB_CLASS0_READ_WIN_ID), (ADD_DATA)cb_msg_item);
    
    //��ʾ�㲥��Ϣ����
    DisplayCBMsgContext(MMISMS_CB_CLASS0_SHOW_MSG_TEXTBOX_CTRL_ID,cb_msg_item);
}

/*****************************************************************************/
//     Description : to show the new SMSCB prompt
//    Global resource dependence : 
//  Author:Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowNewSMSCBPrompt(void)
{
    uint32 time_out = 0;
    MMI_STRING_T prompt_str = {0};
    MMI_WIN_PRIORITY_E win_priority = WIN_ONE_LEVEL;
    MMI_WIN_ID_T alert_win_id = MMISMS_CB_NEWMSG_WIN_ID;

    BOOLEAN is_alert_type_msg = FALSE;

    is_alert_type_msg = MMIAPISMSCB_GetCBMsgType();

	
    //�����յ���С���㲥����ʾ�����ʾ��
#ifdef MMI_ETWS_SUPPORT
    if(!MMIAPISMSCB_GetETWSAlertFlag())
    {
        //        MMI_GetLabelTextByLang(TXT_SMS_CB_RECEIVE_NEW, &prompt_str);
        //RES_ADD_STRING(STR_CB_NEW,"New cell broadcast")
        MMI_GetLabelTextByLang(STR_CB_NEW, &prompt_str);
    }
    else
    {

        MMI_GetLabelTextByLang(TXT_SMS_CB_ETWS_MSG, &prompt_str);
    }
#else
    if(!is_alert_type_msg)
    {
        MMI_GetLabelTextByLang(STR_CB_NEW, &prompt_str);
    }
    else
    {
        MMI_GetLabelTextByLang(STR_CB_EMERGENCY, &prompt_str);
    }
#endif

#ifdef SCREENSAVER_SUPPORT
    if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
    {
        MMIAPIIDLESS_CloseScreenSaver();
    }
#endif
    if (MMISMS_IsForbitNewMsgWin(TRUE))
    {
        win_priority = WIN_LOWEST_LEVEL;
    }
#ifdef MMI_ETWS_SUPPORT
    if(!MMIAPISMSCB_GetETWSAlertFlag())
    {
        MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
    }
#else
    MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
#endif

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
    MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_NEW_SMSCB);
#endif

#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_SMSCB);
#endif

    MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleNewMsgWin);
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : to handle the new message window
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_HandleCommonAlertWinMsg(
												  MMI_WIN_ID_T        win_id,        //IN:
												  MMI_MESSAGE_ID_E    msg_id,        //IN:
												  DPARAM                param        //IN:
												  )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		break;
		
	case MSG_TIMER:
		MMIPUB_SetWinSoftkeyEx(win_id, TXT_NULL, TXT_COMMON_OK, TXT_NULL, TRUE);
		//        MMIPUB_SetTextDialogBlankTitle(win_id, TRUE);
        break;
		
    case MSG_NOTIFY_CANCEL:
		//        MMK_CloseWin(win_id);
        break;
		
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
	
    return recode;
}


/*****************************************************************************/
//  Description : Handle class0 sms
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
BOOLEAN MMISMSCB_ETWSGetisNeedRedisplay()
{
    return s_mmismscb_etws_message_is_need_redisplay;
}

void MMISMSCB_ETWSSetisNeedRedisplay(BOOLEAN flag)
{
    s_mmismscb_etws_message_is_need_redisplay = flag;
}

LOCAL MMI_RESULT_E  HandleDismissETWSWin(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_UNISOC_IDLE_EMERGENCY_CALL_SUPPORT        
        if( MMK_IsOpenWin(MMIIDLE_EMERGENCY_CALL_WIN_ID) || (TRUE == MMIAPISMSCB_GetEMCallStatus()) )
#else            
        if( TRUE == MMIAPISMSCB_GetEMCallStatus() )
#endif            
        {
                MMIAPISMSCB_SetIsNeedRedisplay(TRUE);
            MMK_CloseWin(win_id);
            break;
        }
        else
        {
            MMIAPISMSCB_SetIsNeedRedisplay(FALSE);
        }

        MMK_SetWinPriority(MMK_ConvertIdToHandle(MMISMS_ETWS_NEWMSG_WIN_ID),WIN_THREE_LEVEL);

        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        MMIDEFAULT_StopKeybadBackLightTimer();				
        MMIDEFAULT_StopAutoKeylockTimer();	
        MMIDEFAULT_StopScreenSaverTimer();
        break;

    case MSG_APP_WEB:
        if( TRUE == MMIAPISMSCB_GetIsNeedEnable())
        {
            if( MMK_IsOpenWin(MMIALM_ALIVEWIN_DURING_ETWS_ID) )
            {
                MMK_CloseWin(MMIALM_ALIVEWIN_DURING_ETWS_ID);
            }
	
            if( TRUE == MMIAPISMSCB_IsETWSAlert())
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
            }
            MMK_CloseWin(win_id);
        }
        if( TRUE == MMIALM_GetAlarmRedisplayFlag())
        {
            MMIALM_SetAlarmRedisplayFlag(FALSE);
            MMIALM_Redisplay();
        }
#ifdef COUNTEDTIME_SUPPORT
        if( TRUE == MMIAPICT_GetCTRedisplayFlag())
        {
            MMIAPICT_SetCTRedisplayFlag(FALSE);
            MMIAPICT_Redisplay();
        }
#endif
        break;

    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_0:
        MMIAPIIdle_OpenEmergencyWin( msg_id) ;
         break;

    case MSG_APP_GREEN:
        if( MMICC_IsExistIncommingCall())
        {
            MMICC_AnswerCall();
        }
        break;

    case MSG_KEYDOWN_RED:
        if( MMIAPICC_IsInState(CC_IN_CALL_STATE) )
        {
            MMIAPICC_ReleaseCallByRedkey();
        }
        break;

    case MSG_CLOSE_WINDOW:
        if( TRUE == MMIAPISMSCB_GetIsNeedEnable())
        {
            MMIAPISMSCB_SetIsETWSDisplay(FALSE);

            if( TRUE == MMIAPISMSCB_IsETWSAlert())
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
            }
            MMK_CloseWin(win_id);
         }

        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        MMIDEFAULT_StartSettingLCDBackLightTimer();	
        MMIDEFAULT_StartAutoKeylockTimer();		
        MMIDEFAULT_StartScreenSaverTimer();
	break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : add cb message to richtext
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void AddSMSCBContenttoRichText(MMI_CTRL_ID_T ctrl_id , MMI_STRING_T* etws_prompt_str)
{

    GUIRICHTEXT_ITEM_T  item_data = {0};
    MMISMS_CB_MESSAGE_ITEM_T cb_msg_item = {0};
    MMISMS_CB_CHANNEL_NAME_T msg_channel_name = {0};
    uint16 index            = 0;
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.tag_type = GUIRICHTEXT_TAG_NONE;
    item_data.text_data.buf.str_ptr = etws_prompt_str->wstr_ptr;
    item_data.text_data.buf.len = etws_prompt_str->wstr_len;

            GUIRICHTEXT_AddItem(
                ctrl_id,
                &item_data,
                &index
                );
}

/*****************************************************************************/
//     Description : to show the new SMSCB prompt
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowNewETWSPrompt(MMI_STRING_T* etws_prompt_str, MMI_STRING_T* etws_title_string,BOOLEAN is_enable)
{
    uint32 time_out = 0;
    MMI_STRING_T prompt_str = {0};
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;
    MMI_WIN_ID_T alert_win_id = MMISMS_ETWS_NEWMSG_WIN_ID;
	
    BOOLEAN is_alert_type_msg = FALSE;
    MMI_STRING_T prompt_title_str = {0};
    MMI_STRING_T prompt_content_str = {0};
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_ETWS_IDLE_RICHTEXT_CTRL_ID;


    if(MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID)) 
    {
        MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
    }

        SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
	
    is_alert_type_msg = MMIAPISMSCB_GetCBMsgType();

    if(TRUE==is_set_mcc)
    {
        plmn_info.mcc  = s_set_mcc;
    }
	
    //�����յ���С���㲥����ʾ�����ʾ��
#ifdef SCREENSAVER_SUPPORT
    if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
    {
        MMIAPIIDLESS_CloseScreenSaver();
    }
#endif

    SCI_TRACE_LOW("[MMISMSCB] MMISMS_ShowNewETWSPrompt, is_enable = %d", is_enable);

    if( PNULL != etws_title_str.wstr_ptr )
    {
        SCI_FREE(etws_title_str.wstr_ptr);
    }
    etws_title_str.wstr_ptr = SCI_ALLOCAZ(etws_title_string->wstr_len * sizeof(wchar));
    SCI_MEMSET(etws_title_str.wstr_ptr , 0, etws_title_string->wstr_len* sizeof(wchar));
    etws_title_str.wstr_len = etws_title_string->wstr_len;

    MMIAPICOM_Wstrncpy(etws_title_str.wstr_ptr, etws_title_string->wstr_ptr, etws_title_string->wstr_len );
    etws_title_str.wstr_len = etws_title_string->wstr_len;

    MMK_CreateWin((uint32*)MMISMS_CB_DISPLAY_ETWS_WIN_TAB , PNULL);
    AddSMSCBContenttoRichText(MMISMS_CB_ETWS_IDLE_RICHTEXT_CTRL_ID, etws_prompt_str);
}

/*****************************************************************************/
//     Description : to show the MSCB prompt
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowETWSPrompt(uint16 index)
{
    uint32 time_out = 0;
    MMI_STRING_T prompt_str = {0};

    BOOLEAN is_alert_type_msg = FALSE;
    MMI_STRING_T prompt_title_str = {0};
    MMI_STRING_T prompt_content_str = {0};
    MMI_WIN_ID_T     win_id = MMISMS_ETWS_NEWMSG_WIN_ID;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_ETWS_IDLE_RICHTEXT_CTRL_ID;
    MMISMS_CB_MESSAGE_T* cur_idle_cb_msg = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
    uint16 etws_context_len = 0;
    uint16 etws_title_len = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    uint16 i = 0;
    MMI_STRING_T etws_title = {0};
    MMI_STRING_T etws_prompt_str = {0};
    wchar time_str[CB_MSG_TIME_MAX_LEN + 1] = {0};	
    uint16 time_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    SCI_TRACE_LOW("MMISMS_ShowETWSPrompt, index = %d", index);

    MMIAPISMSCB_SetCurDispCBIndex(index);

    cur_idle_cb_msg = MMISMS_Get_CB_Message_Parent_Ptr();
    temp_msg_ptr = cur_idle_cb_msg->msg_ptr;

    etws_context_len = MMI_CB_CHANNEL_NAME_LEN + 1 \
            +CB_MSG_TIME_MAX_LEN + 1\
            + MN_CB_MACRO_MSG_LEN + 1;
    etws_title_len = MMI_CB_CHANNEL_NAME_LEN + 1;

    prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
    SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));
    prompt_str.wstr_len = 0;

    etws_prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
    SCI_MEMSET(etws_prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));

    if( PNULL != etws_title_str.wstr_ptr )
    {
        SCI_FREE(etws_title_str.wstr_ptr);
    }
    etws_title_str.wstr_ptr = SCI_ALLOCAZ(etws_title_len * sizeof(wchar));
    SCI_MEMSET(etws_title_str.wstr_ptr , 0, etws_title_len* sizeof(wchar));
    etws_title_str.wstr_len = etws_title_len;

    for( i =0; i < index; i ++ )
    {
        temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
    }

    dual_sys = MMIAPISMSCB_GetSimIndex();
    current_channel_info = GetCMASChannelInfo(dual_sys, temp_msg_ptr->msg_id);

    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

    if( TRUE == is_set_mcc )
    {
        plmn_info.mcc  = s_set_mcc;
    }

    if( NULL != current_channel_info)
    {
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
            else if( 0 != current_channel_info->title_id)
            {
                MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, current_channel_info->title_id, MMIAPICOM_Wstrlen(current_channel_info->title_id)) ;
                MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L" ");
                prompt_str.wstr_len = MMIAPICOM_Wstrlen(current_channel_info->title_id) +1;
            }

            //Time
            SCI_MEMSET(time_str, 0, (CB_MSG_TIME_MAX_LEN + 1));
                time_len = ConvertCBTimeToStrForTitle(
                temp_msg_ptr->date,
                temp_msg_ptr->time,
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
            else if( 0 != current_channel_info->title_id)
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
                temp_msg_ptr->date,
                temp_msg_ptr->time,
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
                    temp_msg_ptr->context,
                    temp_msg_ptr->len,
                    temp_msg_ptr->len);

        prompt_str.wstr_len += temp_msg_ptr->len;
        MMIAPICOM_Wstrncpy(etws_prompt_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_prompt_str.wstr_len = prompt_str.wstr_len;
    }

    GUIWIN_SetTitleText(win_id,etws_title_str.wstr_ptr, etws_title_str.wstr_len, TRUE );
    GUIWIN_SetTitleTextAlign(win_id, ALIGN_HMIDDLE);

    GUIRICHTEXT_DeleteAllItems(ctrl_id);
    AddSMSCBContenttoRichText(ctrl_id, &etws_prompt_str);

    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);

    if( PNULL != prompt_str.wstr_ptr)
    {
        SCI_FREE(prompt_str.wstr_ptr);
        prompt_str.wstr_ptr= PNULL;
    }

    if( PNULL != etws_prompt_str.wstr_ptr )
    {
        SCI_FREE(etws_prompt_str.wstr_ptr);
        etws_prompt_str.wstr_ptr= PNULL;
    }
}
#endif

/*****************************************************************************/
//     Description : ��һЩ�ض�������״̬����Ҫ��/�ر�С���㲥����
//                         Situation 1: MCC = 502(Malaysia) and switch from 3G to 2G should be closed CB Switch.
//    Global resource dependence : 
//    Author: Yongli Tong
//    Note: 
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMISMSCB_SwitchCBStatusOnSomeNetworkCircumstance(MN_DUAL_SYS_E dual_sys, BOOLEAN cb_flag)
{
    PHONE_PLMN_NAME_E plmn_name = 0;
    PHONE_PLMN_NAME_E hplmn_name = 0;	
    MN_RETURN_RESULT_E result = MN_RETURN_FAILURE;
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_CONFIG_T cb_config_value = CB_CONFIG_OFF;
#endif

    MMIAPIPHONE_GetFullPlmn(dual_sys, &plmn_name, &hplmn_name);
    if((hplmn_name >= PLMN_502_12) && (hplmn_name <= PLMN_502_156) && (!cb_flag))//Malaysia MCC
    {
        result = MMISMSCB_Close();
#ifdef MMI_ETWS_SUPPORT
        if( TRUE == cb_flag )
        {
            cb_config_value = CB_CONFIG_ON;
        }
        else
        {
            cb_config_value = CB_CONFIG_OFF;
        }
        NV_SaveSmsCBEnabledConfig(cb_config_value);
#else
        NV_SaveSmsCBEnabledFlag(cb_flag);
#endif
    }
    return result;
}

/*****************************************************************************/
//     Description : С���㲥�˵�����Ϣ����ӿں���
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL void HandleMainMenuMsg(
                             MMI_MENU_ID_T menu_id,    //IN: ��ѡ�еĲ˵���
                             MMI_CTRL_ID_T ctrl_id
                             )
{
    switch (menu_id)
    {
    case MMISMS_MENUCB_STARTUP_ITEM_ID:                 //�����㲥�˵��ѡ��
        MMK_CreateWin((uint32 *)MMISMS_CB_STARTUP_WIN_TAB, (ADD_DATA)ctrl_id);
        break;
        
    case MMISMS_MENUCB_READ_ITEM_ID:                     //�Ķ��㲥�˵��ѡ��
        if (!MMISMS_IsCBMsgListEmpty())
        {
            MMK_CreateWin((uint32 *)MMISMS_CB_MSG_LIST_WIN_TAB, PNULL);
        }
        else  
        {
            MMIPUB_OpenAlertWarningWin(STR_CB_MESSAGE_EMPTY);  
        }
        break;
        
    case MMISMS_MENUCB_LANGUAGE_ITEM_ID:                 //���ù㲥���Բ˵��ѡ��
        MMK_CreatePubListWin((uint32*)MMISMS_CB_LANGUAGE_LIST_WIN_TAB, PNULL);  
        break;
        
    case MMISMS_MENUCB_CHANNEL_ADD_ITEM_ID:              //��ӹ㲥�ŵ��˵��ѡ��
        if (!IsCBChannelListFull())
        {
        #ifdef MMI_PDA_SUPPORT
            MMK_CreatePubFormWin((uint32 *)MMISMS_CB_ADD_EDIT_CHANNEL_WIN_TAB, PNULL);
        #else
            MMK_CreateWin((uint32 *)MMISMS_CB_ADD_EDIT_CHANNEL_WIN_TAB, PNULL);
        #endif
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(STR_CB_LIST_FULL);
        }
        break;
        
    case MMISMS_MENUCB_CHANNEL_LIST_ITEM_ID:             //���ù㲥�ŵ��˵��ѡ��
        if (!IsCBChannelListEmpty())
        {
            MMK_CreateWin((uint32 *)MMISMS_CB_CHANNEL_LIST_WIN_TAB, PNULL);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_SMS_CB_CHANNEL_LIST_EMPTY);
        }
        break;

    case MMISMS_MENUCB_CHANNEL_ITEM_ID:
        MMK_CreateWin((uint32*)MMISMS_CB_CHANNEL_LIST_WIN_TAB, PNULL);
        break;
    default:
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBStartUpWindow
//  ��������:        С���㲥����/�رչ㲥ѡ��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleSmsCBStartUpWindow(
                                            MMI_WIN_ID_T    win_id,     // STARTUP NORMAL���ڵ�ID
                                            MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                            DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 index  = 0;
    MMI_CTRL_ID_T ctrl_id = (MMI_CTRL_ID_T)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {   
    case MSG_OPEN_WINDOW:
        //��ʼ���㲥�����б���
        InitSmsCBStartUpList(ctrl_id);
        //����㲥�����б�
        MMK_SetAtvCtrl(win_id,MMISMS_CB_STARTUP_LISTBOX_CTRL_ID);
        
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(MMISMS_CB_STARTUP_LISTBOX_CTRL_ID,&index,2);
        //�л�С���㲥����״̬
        if( 0 == index )
        {
            MMISMSCB_SwitchSmsCBStatus(TRUE);    //����С���㲥
        }
        else
        {
            MMISMSCB_SwitchSmsCBStatus(FALSE);   //�ر�С���㲥
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


//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCB50ChannelWindow
//  ��������:        С���㲥50 Ƶ������/�رմ��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleSmsCB50ChannelWindow(
                                            MMI_WIN_ID_T    win_id,     // STARTUP NORMAL���ڵ�ID
                                            MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                            DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 index  = 0;
    MMI_CTRL_ID_T ctrl_id = (MMI_CTRL_ID_T)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {   
    case MSG_OPEN_WINDOW:
        //��ʼ���㲥�����б���
        InitSmsCB50ChannelEnable(ctrl_id);
        //����㲥�����б�
        MMK_SetAtvCtrl(win_id,MMISMS_CB_50_CHANNEL_LISTBOX_CTRL_ID);
        
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(MMISMS_CB_50_CHANNEL_LISTBOX_CTRL_ID,&index,2);
        //�л�С���㲥����״̬
#ifdef CB_CHANNEL_50_EXIST
        if( 0 == index )
        {
            MMISMSCB_SwitchCB50Channel(TRUE);    //����С���㲥
        }
        else
        {
            MMISMSCB_SwitchCB50Channel(FALSE);   //�ر�С���㲥
        }
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


//----------------------------------------------------------------------------------------------------------------
//
//  ��������         InitSmsCBStartUpList
//  ��������:        ��ʼ���㲥�����б���
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void InitSmsCBStartUpList(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN cb_enabled = FALSE;
    int32 i = 0;
    const MMI_CTRL_ID_T enable_ctrl_id[] ={
        MMISMS_CB_CHILD_SET_CARD1_CTRL_ID,
        MMISMS_CB_CHILD_SET_CARD2_CTRL_ID,
        MMISMS_CB_CHILD_SET_CARD3_CTRL_ID,
        MMISMS_CB_CHILD_SET_CARD4_CTRL_ID
        };
    
    //�趨2��ѡ�� һ������ һ���ر�
    GUILIST_SetMaxItem( MMISMS_CB_STARTUP_LISTBOX_CTRL_ID, 2, FALSE );//max item 2
    
    MMIAPISET_AppendListItemByTextId( STR_CB_SOFTKEY_ENABLE, STXT_SOFTKEY_OK_MK, MMISMS_CB_STARTUP_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    MMIAPISET_AppendListItemByTextId( STR_CB_SOFTKEY_DISABLE, STXT_SOFTKEY_OK_MK, MMISMS_CB_STARTUP_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //��ȡNV�е�������־
    for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
    {
        if(ctrl_id == enable_ctrl_id[i])
        {
            break;
        }
    }

    if (i < MMI_DUAL_SYS_MAX)
    {
#ifdef MMI_ETWS_SUPPORT
        cb_enabled = MMISMS_GetSmsCBEnabledFlag((MN_DUAL_SYS_E)i);
#else
        cb_enabled = NV_LoadSmsCBEnabledFlag((MN_DUAL_SYS_E)i);
#endif
    }
    
    //����NV�е�������־���趨�������ر��ѡ��
    if(cb_enabled)
    {
        //GUILIST_SetToggleState(MMISMS_CB_SETTING_CTRL_ID,0,TRUE);
        GUILIST_SetSelectedItem(MMISMS_CB_STARTUP_LISTBOX_CTRL_ID,0,TRUE);
        GUILIST_SetCurItemIndex(MMISMS_CB_STARTUP_LISTBOX_CTRL_ID,0);
    }
    else
    {
        //GUILIST_SetToggleState(MMISMS_CB_SETTING_CTRL_ID,0,FALSE);
        GUILIST_SetSelectedItem(MMISMS_CB_STARTUP_LISTBOX_CTRL_ID,1,TRUE);
        GUILIST_SetCurItemIndex(MMISMS_CB_STARTUP_LISTBOX_CTRL_ID,1);
    }
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         InitSmsCB50ChannelEnable
//  ��������:        ��ʼ��С���㲥50Ƶ�δ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void InitSmsCB50ChannelEnable(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN cb_enabled = FALSE;
    int32 i = 0;
    const MMI_CTRL_ID_T enable_ctrl_id[] ={
        MMISMS_CB_CHILD_SET_CARD1_CTRL_ID,
        MMISMS_CB_CHILD_SET_CARD2_CTRL_ID,
        MMISMS_CB_CHILD_SET_CARD3_CTRL_ID,
        MMISMS_CB_CHILD_SET_CARD4_CTRL_ID
        };
    
    //�趨2��ѡ�� һ������ һ���ر�
    GUILIST_SetMaxItem( MMISMS_CB_50_CHANNEL_LISTBOX_CTRL_ID, 2, FALSE );//max item 2
    
    MMIAPISET_AppendListItemByTextId( STR_CB_SOFTKEY_ENABLE, STXT_SOFTKEY_OK_MK, MMISMS_CB_50_CHANNEL_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    MMIAPISET_AppendListItemByTextId( STR_CB_SOFTKEY_DISABLE, STXT_SOFTKEY_OK_MK, MMISMS_CB_50_CHANNEL_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //��ȡNV�е�������־
    for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
    {
        if(ctrl_id == enable_ctrl_id[i])
        {
            break;
        }
    }

    if (i < MMI_DUAL_SYS_MAX)
    {
        cb_enabled = NV_LoadSmsCB50ChannelFlag((MN_DUAL_SYS_E)i);
    }
    
    //����NV�е�������־���趨�������ر��ѡ��
    if(cb_enabled)
    {
        GUILIST_SetSelectedItem(MMISMS_CB_50_CHANNEL_LISTBOX_CTRL_ID,0,TRUE);
        GUILIST_SetCurItemIndex(MMISMS_CB_50_CHANNEL_LISTBOX_CTRL_ID,0);
    }
    else
    {
        GUILIST_SetSelectedItem(MMISMS_CB_50_CHANNEL_LISTBOX_CTRL_ID,1,TRUE);
        GUILIST_SetCurItemIndex(MMISMS_CB_50_CHANNEL_LISTBOX_CTRL_ID,1);
    }

}


//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_SwitchSmsCBStatus
//  ��������:        �л�С���㲥����״̬
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
void MMISMSCB_SwitchSmsCBStatus(
                                BOOLEAN cb_enabled      //Ҫ�л���״̬ (�������)
                                )
{
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMISMS_CB_LANGUAGE_T lang_list = {0};
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_CONFIG_T cb_config_value = CB_CONFIG_OFF;
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
#endif

    //����ǰ״̬��Ҫ�л���״̬��ͬ������л�
#ifdef MMI_ETWS_SUPPORT
    if(cb_enabled != MMISMS_GetSmsCBEnabledFlag(dual_sys))
#else
    if(cb_enabled != NV_LoadSmsCBEnabledFlag(dual_sys))
#endif		
    {
        if(cb_enabled)                       //���ǹ㲥�ѹر���򿪣�����ر�
        {
            //��NV��ȡ�ŵ��б�������б�
#ifdef MMI_ETWS_SUPPORT
            NV_LoadSmsCBChannelList();
            SCI_MEMCPY(&channel_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
            channel_list = MMISMS_NV_LoadSmsCBChannelList(dual_sys);
#endif

#ifdef MMI_ETWS_SUPPORT
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

            if( TRUE == is_set_mcc )
            {
                plmn_info.mcc  = s_set_mcc;
            }

            if( CB_IT_MCC != plmn_info.mcc )
            {
                NV_LoadSmsCBLanguageList(dual_sys, &lang_list);
            }
            else
            {
                MMISMSCB_SetAllLangList(&lang_list);
            }
#else
            lang_list = NV_LoadSmsCBLanguageList(dual_sys);
#endif

            //����С���㲥
            SCI_TRACE_LOW("MMISMSCB_SwitchSmsCBStatus MMISMSCB_Startup");
            return_val=MMISMSCB_Startup(&channel_list, &lang_list, dual_sys);  
            //!!!!!!!!!!!!!!!!!!!�˴������⣬ClearCase�ϵ� 7100D ƽ̨Ҫ������С���㲥��Ҫ�����趨һ��С���㲥����!!!!
            return_val=MMISMSCB_Reset(&channel_list, &lang_list, dual_sys);    //�����趨С���㲥����
            
        }
        else 
        {
            return_val=MMISMSCB_Close();
        }
        
        if(return_val==MN_RETURN_SUCCESS)     //��������رչ㲥�ɹ�����ʾ�ɹ������浽NV��ȥ   
        {
            if (MMK_IsOpenWin(MMISMS_CB_STARTUP_WIN_ID))
            {    
                if(cb_enabled)
                {
                    MMIPUB_OpenAlertSuccessWin(TXT_SMS_CB_TURN_ON_SUCC);
                }
                else
                {
                    MMIPUB_OpenAlertSuccessWin(TXT_SMS_CB_TURN_OFF_SUCC);
                }
            }
            
#ifdef MMI_ETWS_SUPPORT
            if( TRUE == cb_enabled )
            {
                cb_config_value = CB_CONFIG_ON;
            }
            else
            {
                cb_config_value = CB_CONFIG_OFF;
            }
            NV_SaveSmsCBEnabledConfig(cb_config_value);
#else
            NV_SaveSmsCBEnabledFlag(cb_enabled);
#endif
        }
        else 
        {
            
            if (MMK_IsOpenWin(MMISMS_CB_STARTUP_WIN_ID))
            {
                //���¼���Ϊ�����ڿ�����رչ㲥ʧ��ʱ��ѡ��ѡ�����
                GUILIST_SetSelectedItem(MMISMS_CB_STARTUP_LISTBOX_CTRL_ID,cb_enabled,FALSE);
                GUILIST_SetSelectedItem(MMISMS_CB_STARTUP_LISTBOX_CTRL_ID,(BOOLEAN)(!cb_enabled),TRUE);
                
                //��ʾʧ��
                if(cb_enabled)
                {   
                    MMIPUB_OpenAlertSuccessWin(TXT_SMS_CB_TURN_ON_FAIL);
                }
                else
                {
                    MMIPUB_OpenAlertSuccessWin(TXT_SMS_CB_TURN_OFF_FAIL);
                }
            }
            
        }
        
        MMK_CloseWin(MMISMS_CB_STARTUP_WIN_ID);
    }
    else
    {
        if (MMK_IsOpenWin(MMISMS_CB_STARTUP_WIN_ID))
        {
            if(cb_enabled)
            {
                MMIPUB_OpenAlertSuccessWin(TXT_SMS_CB_TURN_ON_SUCC);
            }
            else
            {
                MMIPUB_OpenAlertSuccessWin(TXT_SMS_CB_TURN_OFF_SUCC);
            }
            MMK_CloseWin(MMISMS_CB_STARTUP_WIN_ID);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_SwitchCB50Channel
//  ��������:        �л�50Ƶ��С���㲥
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
void MMISMSCB_SwitchCB50Channel(
                                BOOLEAN cb_enabled      //Ҫ�л���״̬ (�������)
                                )
{
    uint16 i = 0;
    MMISMS_CB_CHANNEL_T cb_channel_info = {0};
    MMISMS_CB_LANGUAGE_T lang_card_list = {0};
    //BOOLEAN cb_card_enabled = FALSE;
    BOOLEAN cb_major_enabled = FALSE;
    BOOLEAN cb_50_channel_exist = FALSE;
    MN_RETURN_RESULT_E result = MN_RETURN_FAILURE;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
#ifdef MMI_ETWS_SUPPORT
    uint16 etws_alert_num = 0;
    etws_alert_num = MMIAPISMSCB_GetETWSNum();
#endif    

    if(cb_enabled != NV_LoadSmsCB50ChannelFlag(dual_sys))
    {
        NV_SaveSmsCB50ChannelFlag(cb_enabled);
#ifdef CB_CHANNEL_50_EXIST
#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBChannelList();
        SCI_MEMCPY(&cb_channel_info, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
        cb_channel_info = MMISMS_NV_LoadSmsCBChannelList(dual_sys);
#endif
        //cb_card_enabled = NV_LoadSmsCBEnabledFlag(dual_sys);
#ifdef MMI_ETWS_SUPPORT
        cb_major_enabled = MMISMS_GetSmsCBEnabledFlag(dual_sys);
#else
        cb_major_enabled = NV_LoadSmsCBEnabledFlag(dual_sys);
#endif

#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBLanguageList(dual_sys, &lang_card_list);
#else
        lang_card_list = NV_LoadSmsCBLanguageList(dual_sys); 
#endif

        for(i = 0; i < cb_channel_info.num; i++)
        {
            if(MMI_PER_INSTALL_PARAM == cb_channel_info.channel_arr[i].msg_id)
            {
                cb_50_channel_exist = TRUE;
                break;
            }
        }
        if(cb_50_channel_exist)
        {
            if(cb_enabled)
            {
                cb_channel_info.channel_arr[i].is_enabled = cb_enabled;
#ifdef MMI_ETWS_SUPPORT
                SCI_MEMSET(&s_save_to_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T));
                SCI_MEMCPY(&s_save_to_nv_channel_list,&cb_channel_info, sizeof(MMISMS_CB_CHANNEL_T));
                NV_SaveSmsCBChannelList();
#else
                NV_SaveSmsCBChannelList(&cb_channel_info);
#endif
                if(cb_major_enabled)
                {
                    SCI_TRACE_LOW("MMISMSCB_SwitchCB50Channel ON MMISMSCB_Startup");
                    MMISMSCB_Startup(&cb_channel_info, &lang_card_list, dual_sys);
                    result = MMISMSCB_Reset(&cb_channel_info, &lang_card_list, dual_sys);    //�����趨С���㲥����(dual_sys);
                }
                else
                {
                    result = MMISMSCB_Close();
                }
            }
            else
            {
#ifdef MMI_ETWS_SUPPORT
                if( 0 == etws_alert_num  )
#endif
                    DeleteCBChannel(i - MMI_CB_ALERT_NUM);
            }
        }
        else
        {
            if(cb_enabled)
            {
                cb_channel_info.channel_arr[i].is_enabled = cb_enabled;
                cb_channel_info.channel_arr[i].msg_id = MMI_PER_INSTALL_PARAM;
                cb_channel_info.num++;
#ifdef MMI_ETWS_SUPPORT
                SCI_MEMSET(&s_save_to_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T));
                SCI_MEMCPY(&s_save_to_nv_channel_list,&cb_channel_info, sizeof(MMISMS_CB_CHANNEL_T));
                NV_SaveSmsCBChannelList();
#else    
                NV_SaveSmsCBChannelList(&cb_channel_info);
#endif
                AppendCBChannelListItem( MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID,
					cb_channel_info.channel_arr[i], i-1 );
                if(cb_major_enabled)
                {
                    SCI_TRACE_LOW("MMISMSCB_SwitchCB50Channel OFF MMISMSCB_Startup");
                    MMISMSCB_Startup(&cb_channel_info, &lang_card_list, dual_sys);
                    result = MMISMSCB_Reset(&cb_channel_info, &lang_card_list, dual_sys);
                }
                else
                {
                    result = MMISMSCB_Close();
                }
            }
            else
            {
                return;
            }
        }
#endif
    }
    //GUILIST_SetSelectedItem(MMISMS_CB_50_CHANNEL_LISTBOX_CTRL_ID,cb_enabled,cb_enabled);
}

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_SwitchCellInfoStatus
//  ��������:        �л�С���㲥λ����Ϣ����״̬
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Yongli Tong
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void MMISMSCB_SwitchCellInfoStatus(
            BOOLEAN cell_info_flag, 
            MN_DUAL_SYS_E dual_sys
        )
{
    int8 result = 0;
    int8 iTemp = 0;
    BOOLEAN temp_cell_info = FALSE;
    BOOLEAN is_cell_info_exist = FALSE;
    MMISMS_CB_CHANNEL_T channel_card_list = {0};
    MMISMS_CB_LANGUAGE_T lang_card_list = {0};
    uint8 * p_cell_info_id = PNULL;
    MMI_STRING_T cell_id_str = {0};
    uint8 * p_cell_info_name = PNULL;
    MMI_STRING_T cell_name_str = {0};
    temp_cell_info = NV_LoadCellInfoEnabledFlag(dual_sys, &result);
    //read ok
    if((MN_RETURN_SUCCESS == result) && (temp_cell_info == s_cell_info_flag[dual_sys]))
    {
        result = NV_SaveCellInfoEnabledFlag(dual_sys, !temp_cell_info);
        //write ok
        if(MN_RETURN_SUCCESS == result)
        {
#ifdef MMI_ETWS_SUPPORT
            NV_LoadSmsCBChannelList();
            SCI_MEMCPY(&channel_card_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
            channel_card_list = MMISMS_NV_LoadSmsCBChannelList(dual_sys);
#endif

#ifdef MMI_ETWS_SUPPORT
            NV_LoadSmsCBLanguageList(dual_sys, &lang_card_list);
#else
            lang_card_list = NV_LoadSmsCBLanguageList(dual_sys); 
#endif
            for(iTemp = 0; iTemp < channel_card_list.num; iTemp++)
            {
                if((CB_CELL_INFO_INDEX_ID == channel_card_list.channel_arr[iTemp].msg_id) && channel_card_list.channel_arr[iTemp].is_enabled)
                {
                    is_cell_info_exist = TRUE;
                    //sync between global var and nv
                    break;
                }
            }
            if(is_cell_info_exist)
            {
                if(s_cell_info_flag[dual_sys])
                {
                    channel_card_list.channel_arr[iTemp].is_enabled = FALSE;
                }
                else
                {
                    channel_card_list.channel_arr[iTemp].is_enabled = TRUE;
                }
#ifdef MMI_ETWS_SUPPORT
                SCI_MEMSET(&s_save_to_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T));
                SCI_MEMCPY(&s_save_to_nv_channel_list,&channel_card_list, sizeof(MMISMS_CB_CHANNEL_T));
                NV_SaveSmsCBChannelList();
#else    
                NV_SaveSmsCBChannelList(&channel_card_list);
#endif
                MMISMSCB_Reset(&channel_card_list, &lang_card_list, dual_sys);
            }
            else
            {
                if((!s_cell_info_flag[dual_sys]) && (channel_card_list.num < MMI_MAX_CB_CHANNEL_NUM))
                {
                    p_cell_info_id = SCI_ALLOCAZ(2 * sizeof(uint8));
                    if(PNULL == p_cell_info_id)
                    {
                        MMIPUB_OpenAlertSuccessWin(TXT_ERROR);
                        SCI_TRACE_LOW("MMISMSCB_SwitchCellInfoStatus memory alloc eror.");
                        SCI_FREE(p_cell_info_id);
                        return;
                    }
                    MMIAPICOM_Int2Str(CB_CELL_INFO_INDEX_ID, p_cell_info_id, 2);
                    cell_id_str.wstr_len = 2;
                    cell_id_str.wstr_ptr = SCI_ALLOCAZ(2 * sizeof(wchar));
                    if(PNULL == cell_id_str.wstr_ptr)
                    {
                        MMIPUB_OpenAlertSuccessWin(TXT_ERROR);
                        SCI_TRACE_LOW("MMISMSCB_SwitchCellInfoStatus memory alloc eror.");
                        SCI_FREE(p_cell_info_id);
                        SCI_FREE(cell_id_str.wstr_ptr);
                        return;
                    }
                    cell_id_str.wstr_ptr = MMIAPICOM_StrToWstr(p_cell_info_id, cell_id_str.wstr_ptr);

                    p_cell_info_name = SCI_ALLOCAZ(10 * sizeof(uint8));
                    SCI_STRCPY(p_cell_info_name, "cell info");
                    if(PNULL == p_cell_info_name)
                    {
                        MMIPUB_OpenAlertSuccessWin(TXT_ERROR);
                        SCI_TRACE_LOW("MMISMSCB_SwitchCellInfoStatus memory alloc eror.");
                        SCI_FREE(p_cell_info_id);
                        SCI_FREE(cell_id_str.wstr_ptr);
                        SCI_FREE(p_cell_info_name);
                        return;
                    }
                    cell_name_str.wstr_len = 10;
                    cell_name_str.wstr_ptr = SCI_ALLOCAZ(10 * sizeof(wchar));
                    if(PNULL == cell_name_str.wstr_ptr)
                    {
                        MMIPUB_OpenAlertSuccessWin(TXT_ERROR);
                        SCI_TRACE_LOW("MMISMSCB_SwitchCellInfoStatus memory alloc eror.");
                        SCI_FREE(p_cell_info_id);
                        SCI_FREE(cell_id_str.wstr_ptr);
                        SCI_FREE(p_cell_info_name);
                        SCI_FREE(cell_name_str.wstr_ptr);
                        return;
                    }
                    cell_name_str.wstr_ptr = MMIAPICOM_StrToWstr(p_cell_info_name, cell_name_str.wstr_ptr);
                    AddCBChannel(cell_id_str,cell_name_str);
                    MMISMSCB_Reset(&channel_card_list, &lang_card_list, dual_sys);

                    SCI_FREE(p_cell_info_id);
                    SCI_FREE(cell_id_str.wstr_ptr);
                    SCI_FREE(p_cell_info_name);
                    SCI_FREE(cell_name_str.wstr_ptr);
                }
                if((!s_cell_info_flag[dual_sys]) && (channel_card_list.num >= MMI_MAX_CB_CHANNEL_NUM))
                {
                    MMIPUB_OpenAlertSuccessWin(TXT_ADD_FAIL);
                    SCI_TRACE_LOW("The cell broadcast channels in the lists are up to max.");
                    return;
                }
            }
            s_cell_info_flag[dual_sys] = !temp_cell_info;
            GUILIST_SetSelectedItem(MMISMS_CB_SETTING_CTRL_ID,MMISMS_PDA_SMS_CB_SETTINGS_CELLINFO,s_cell_info_flag[dual_sys]);

        }
        //write error
        else
        {
            s_cell_info_flag[dual_sys] = NV_LoadCellInfoEnabledFlag(dual_sys, &result);
            SCI_TRACE_LOW("MMISMSCB_SwitchCellInfoStatus write error result = %d.",result);
            MMIPUB_OpenAlertSuccessWin(TXT_SMS_CB_TURN_ON_FAIL);
        }
    }
    //read error
    else
    {
        s_cell_info_flag[dual_sys] = NV_LoadCellInfoEnabledFlag(dual_sys, &result);
        SCI_TRACE_LOW("MMISMSCB_SwitchCellInfoStatus read error result = %d.",result);
        MMIPUB_OpenAlertSuccessWin(TXT_SMS_CB_TURN_ON_FAIL);
    }

}

#endif

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void AppendSmsCBLanguageListItem(
                                     MMI_CTRL_ID_T	    ctrl_id,
                                     MMISMS_CB_LANGUAGE_T   *lang_list,
                                     BOOLEAN is_update
                                     )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16  cur_selection = 0;
    uint16  cur_top = 0;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint32 i = 0;
    MMI_STRING_T smscb_string = {0};
    uint16 index = 0;

    if (PNULL == lang_list)
    {
        //SCI_TRACE_LOW:"MMISMS:AppendSmsCBLanguageListItem lang_list = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2925_112_2_18_2_55_34_903,(uint8*)"");
        return;
    }

    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    
    for( i = 0; i < MMI_MAX_CB_LANGUAGE_NUM; i++ )
    {
        if (0 == i)
        {
            MMI_GetLabelTextByLang(STR_CB_LANG_ALL, &smscb_string);
            if (IsAllLangSelected(lang_list))
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
        }
        else
        {
            MMI_GetLabelTextByLang(g_cb_lang_text_arr[i - 1], &smscb_string);
            //smscb_string.wstr_len = MMIAPICOM_Wstrlen(lang_name[i - 1]);
            //smscb_string.wstr_ptr = lang_name[i - 1];

            if (IsAllLangSelected(lang_list) || lang_list->selected_arr[i - 1])
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
        }
#ifndef MMI_SMS_MINI_SUPPORT
#ifndef MMI_GUI_STYLE_TYPICAL
        if (IMAGE_CHECK_UNSELECTED_ICON == image_id)
        {
            item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)STXT_SOFTKEY_SAVE_MK;
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)STXT_SOFTKEY_MARK_MK;
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)TXT_COMMON_COMMON_BACK;
        }
		else
        {
            item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)STXT_SOFTKEY_SAVE_MK;
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)STXT_SOFTKEY_UNMARK_MK;
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)TXT_COMMON_COMMON_BACK;
        }
#else
        if (IMAGE_CHECK_UNSELECTED_ICON == image_id)
        {
            item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)TXT_NULL;
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)TXT_COMMON_COMMON_BACK;
        }
		else
        {
            item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)TXT_RETURN;
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)TXT_COMMON_COMMON_BACK;
        }
#endif

#else
        if (IMAGE_CHECK_UNSELECTED_ICON == image_id)
        {
            item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)STXT_SOFTKEY_MENU_MK;
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_OK	;
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)TXT_COMMON_COMMON_BACK;
        }
        else
        {
            item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)STXT_SOFTKEY_MENU_MK;
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_RETURN;
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)TXT_COMMON_COMMON_BACK;//(MMI_TEXT_ID_T)STXT_RETURN;
        }

#endif
        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = image_id;
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_len = smscb_string.wstr_len;  
        
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = smscb_string.wstr_ptr;
//         MMIAPICOM_Wstrncpy( item_data.item_content[1].item_data.text_buffer.wstr,
//             smscb_string.wstr_ptr,
//             item_data.item_content[1].item_data.text_buffer.wstr_len );
        
        //item_data.item_content[1].item_data.text_buffer.is_ucs2 = smscb_string.is_ucs2;

        if(is_update)
        {
            GUILIST_ReplaceItem(ctrl_id, &item_t, index);
        }
        else
        {
            GUILIST_AppendItem(ctrl_id, &item_t);
        }
        index++;

    }
        
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBLanguageListWindow
//  ��������:        �㲥�ŵ�����ѡ���б��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------

LOCAL BOOLEAN is_all_language_selected(MMISMS_CB_LANGUAGE_T * langptr)
{
    uint32 i = 0;

    for(i=0;i<MMI_MAX_CB_LANGUAGE_NUM;i++)
    {  
        if(langptr->selected_arr[i]==FALSE)
            return FALSE;
    }

    return TRUE;
}
LOCAL MMI_RESULT_E HandleSmsCBLanguageListWindow(
                                                 MMI_WIN_ID_T    win_id,     // STARTUP NORMAL���ڵ�ID
                                                 MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                                 DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                                 )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    uint16 index = 0;
    static MMISMS_CB_CHANNEL_T cur_channel_list;
    static MMISMS_CB_LANGUAGE_T cur_lang_list;
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    static BOOLEAN s_is_langlist_change = FALSE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_LANGUAGE_LISTBOX_CTRL_ID;
    static BOOLEAN is_lang_all_sel = FALSE;
    uint32 i = 0;
#ifdef MMI_ETWS_SUPPORT
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBChannelList();
        SCI_MEMCPY(&cur_channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));
#else
        cur_channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif

        //��NV��ȡ����ѡ���б�
#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBLanguageList(MMISMSCB_GetSelectCard(), &cur_lang_list);
#else
        cur_lang_list    = NV_LoadSmsCBLanguageList(MMISMSCB_GetSelectCard());
#endif

        is_lang_all_sel = IsAllLangSelected(&cur_lang_list);
        s_is_langlist_change = FALSE;
#ifdef MMI_GUI_STYLE_TYPICAL
        //GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
#endif

        GUILIST_SetMaxItem(ctrl_id,MMI_MAX_CB_LANGUAGE_NUM, FALSE );//max item 11

        AppendSmsCBLanguageListItem(ctrl_id, &cur_lang_list, FALSE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
        AppendSmsCBLanguageListItem(ctrl_id, &cur_lang_list, TRUE);
        break;

#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_SMS_MINI_SUPPORT
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif        
#endif
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:   
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        //����ָ���Ĺ㲥������
        index = GUILIST_GetCurItemIndex(ctrl_id);
        
        if(0 < index) //��ǰ���ĳһ������
        {
            index--;
            
            if(cur_lang_list.selected_arr[index])                     //�����ѡ����        
            {
                cur_lang_list.selected_arr[index]=FALSE;
                is_lang_all_sel = FALSE;
            }
            else
            {
                cur_lang_list.selected_arr[index]=TRUE;
                is_lang_all_sel = is_all_language_selected(&cur_lang_list);
            }
            
            s_is_langlist_change = TRUE;
        }
        else //��ǰ���"ȫ������"
        {
            if(is_lang_all_sel)
            {
                //ȡ����������ѡ��
                for(i=0;i<MMI_MAX_CB_LANGUAGE_NUM;i++)
                {  
                    cur_lang_list.selected_arr[i]=FALSE;
                }
                is_lang_all_sel = FALSE;
            }
            else
            {
                //ѡ������ѡ��
                for(i=0;i<MMI_MAX_CB_LANGUAGE_NUM;i++)
                {  
                    cur_lang_list.selected_arr[i]=TRUE;
                }
                is_lang_all_sel = TRUE;
            }
            s_is_langlist_change = TRUE;
        }
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if(s_is_langlist_change)
        {
            return_val = MMISMSCB_Reset(&cur_channel_list, &cur_lang_list,MMISMSCB_GetSelectCard());    //�����趨С���㲥����
            
            if (return_val == MN_RETURN_SUCCESS)                             //�����趨С���㲥���Գɹ�����ʾ�ɹ�        
            {                
                NV_SaveSmsCBLanguageList(cur_lang_list);                  //���������б� NV ��ȥ
            }
            else                                                          //�����趨С���㲥���Գ�������ʾ����        
            {
                MMIPUB_OpenAlertFailWin(TXT_COMMON_SETTING_FAIL);        
            }
        }

        MMK_CloseWin( win_id );
        break;

    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
#else
    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        if(s_is_langlist_change)
        {
            return_val=MMISMSCB_Reset(&cur_channel_list, &cur_lang_list,MMISMSCB_GetSelectCard());    //�����趨С���㲥����
            
            if(return_val==MN_RETURN_SUCCESS)                             //�����趨С���㲥���Գɹ�����ʾ�ɹ�        
            {        
                MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
                
                NV_SaveSmsCBLanguageList(cur_lang_list);                  //���������б� NV ��ȥ
            }
            else                                                          //�����趨С���㲥���Գ�������ʾ����        
            {
                MMIPUB_OpenAlertFailWin(TXT_COMMON_SETTING_FAIL);        
            }
        }
        MMK_CloseWin( win_id );
        break;
#endif        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBChannelListWindow
//  ��������:        �㲥�ŵ�ѡ���б��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            liqing.peng
//
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN IsAllLangSelected(
                                MMISMS_CB_LANGUAGE_T   *lang_list
                                )
{
    BOOLEAN is_all_sel = TRUE;
    uint32 i = 0;
    
     if (PNULL == lang_list)
    {
        //SCI_TRACE_LOW:"MMISMS:IsAllLangSelected lang_list = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_3168_112_2_18_2_55_35_904,(uint8*)"");
        return FALSE;
    }
    
    for(i=0;i<MN_CB_LANGUAGE_ID_MAX_NUM;i++)
    {  
        if(!lang_list->selected_arr[i])        //������δ��ѡ��
        {
            is_all_sel = FALSE;
            break;
        }
    }
    
    return is_all_sel;
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : get current channel index from visible channel list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetCurChannelIndex( uint16 item_list_index )
{
    MN_DUAL_SYS_E dual_sys;
    uint16 etws_alert_num = 0;
    BOOLEAN search_result = FALSE;
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    int16 i = 0;
    int16 j = 0;	
    uint16 channel_index = 0;
    static MMISMS_CB_CHANNEL_T cur_channel_list = {0};
    MMISMS_CB_CHANNEL_T etws_channel_list = {0};
    MMISMS_CB_CHANNEL_T* etws_channel_list_ptr = PNULL;
	
    dual_sys = MMISMSCB_GetSelectCard();	
    etws_alert_num = MMIAPISMSCB_GetETWSNum();

    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&cur_channel_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));

    InitCB_ETWSChannel(dual_sys);
    etws_channel_list_ptr = MMISMSCB_Get_ETWS_Channel_Ptr(dual_sys);
    SCI_MEMCPY(&etws_channel_list, etws_channel_list_ptr, sizeof(MMISMS_CB_CHANNEL_T));
	
    s_visible_channel_num = 0;
    for(i=0;i<etws_alert_num;i++)
    {
        current_channel_info = GetCMASChannelInfo(dual_sys, etws_channel_list.channel_arr[i].msg_id);
        if( NULL != current_channel_info)
        {
            if(TRUE == current_channel_info->can_be_visible)
            {
                s_visible_channel_num ++;
            }
        }
    }

    // no hidden channel
    if( etws_alert_num == s_visible_channel_num )
    {
        channel_index = item_list_index;
    }
    // etws channel
    else if( etws_alert_num > 0 && (item_list_index < s_visible_channel_num))
    {
        // checek if etws channel
        for( i = 0; i < etws_alert_num; i ++)
        {
            current_channel_info = GetCMASChannelInfo(dual_sys, etws_channel_list.channel_arr[i].msg_id);
            if( NULL != current_channel_info)
            {
                if( TRUE == current_channel_info->can_be_visible )
                {
                    j ++;
                }
            }
            else
            {
                break;
            }

            if( ( j > 0 )  && (item_list_index == j-1 ) )
            {
                search_result = TRUE;
                channel_index = i ;
                break;
            }
        }

        if( etws_alert_num == i )
        {
            channel_index = item_list_index;
        }
    }
    // user added channel
    else if( (s_visible_channel_num > 0 ) && (item_list_index > s_visible_channel_num -1))
    {
        channel_index = item_list_index + (etws_alert_num - s_visible_channel_num );
    }
    else
    {
        channel_index = item_list_index;
    }

    return channel_index;

}
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBChannelListWindow
//  ��������:        �㲥�ŵ�ѡ���б��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            rong.gu
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleSmsCBChannelListWindow(
                                                MMI_WIN_ID_T    win_id,     // STARTUP NORMAL���ڵ�ID
                                                MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                                DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    //MMISMS_CB_CHANNEL_T channel_list = {0};
    uint16 index=0;
#ifdef MMI_ETWS_SUPPORT
    uint16 etws_alert_num = 0;
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    MMISMS_CB_CHANNEL_T etws_channel_list = {0};
    uint16 i = 0;
    uint16 j = 0;
    MMISMS_CB_CHANNEL_T cur_channel_list = {0};
    MMICOM_BANNER_INFO_T banner_info = {0};
    GUI_POINT_T alert_start_point = {0, 0};
    MMISMS_CB_CHANNEL_T* etws_channel_list_ptr = PNULL;

    etws_alert_num = MMIAPISMSCB_GetETWSNum();
#endif

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_NULL, FALSE);
        GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, IMAGE_ISTYLE_TITLE_ADD, IMAGE_ISTYLE_TITLE_ADD_DOWN, FALSE);
        GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);
#endif
        MMK_SetAtvCtrl(win_id,MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);

        GUIWIN_SetSoftkeyTextId(MMISMS_CB_CHANNEL_LIST_WIN_ID, STXT_OPTION, TXT_NULL, TXT_COMMON_COMMON_BACK, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(MMISMS_CB_CHANNEL_LIST_WIN_ID, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(MMISMS_CB_CHANNEL_LIST_WIN_ID, STXT_OPTION, LEFT_BUTTON, TRUE);

#ifdef MMI_ETWS_SUPPORT
        s_is_it_pop_toast = FALSE;

        InitCB_ETWSChannel(dual_sys);
        etws_channel_list_ptr = MMISMSCB_Get_ETWS_Channel_Ptr(dual_sys);
        SCI_MEMCPY(&etws_channel_list, etws_channel_list_ptr, sizeof(MMISMS_CB_CHANNEL_T));
        s_visible_channel_num = 0;
        for(i=0;i<etws_alert_num;i++)
        {
            current_channel_info = GetCMASChannelInfo(dual_sys, etws_channel_list.channel_arr[i].msg_id);
            if( NULL != current_channel_info)
            {
                if(TRUE == current_channel_info->can_be_visible)
                {
                    s_visible_channel_num ++;
                }
            }
        }
#endif

        break;

    case MSG_FULL_PAINT:
        InitCBChannelList();
        break;
    
    case MSG_GET_FOCUS:
        GUILIST_SetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID, s_list_index);
        break;
        
    case MSG_LOSE_FOCUS:
        s_list_index = GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        //MMK_CreateWin((uint32*) MMISMS_CB_CHANNEL_OPT_B_WIN_TAB , PNULL ); 
        {
            MMIPUB_MENU_PARAM_T menuparam = {0};
            menuparam.group_id = MENU_SMS_CB_CHANNEL_LIST_OPT_B;
            menuparam.win_id =   MMISMS_CB_CHANNEL_OPT_WIN_ID;
            menuparam.ctrl_id =  MMISMS_CB_CHANNEL_LIST_MENU_CTRL_ID;    
            menuparam.dst_handle = win_id;
            menuparam.func.OnMenuInit = HandleSmsCBChannelOptWindowInit;
            menuparam.func.OnMenuOk = HandleSmsCBChannelOptWindowOk;                    
            MMIPUB_OpenPubMenuWin( &menuparam ); 
            MMK_SetWinUserData(MMISMS_CB_CHANNEL_OPT_WIN_ID, (void*)menuparam.group_id);
        }    
        break;                
        
    case MSG_CTL_LIST_LONGOK:
        //MMK_CreateWin((uint32*) MMISMS_CB_CHANNEL_OPT_A_WIN_TAB , PNULL );
        {
            MMIPUB_MENU_PARAM_T menuparam = {0};
            menuparam.group_id = MENU_SMS_CB_CHANNEL_LIST_OPT_A;
            menuparam.win_id =   MMISMS_CB_CHANNEL_OPT_WIN_ID;
            menuparam.ctrl_id =  MMISMS_CB_CHANNEL_LIST_MENU_CTRL_ID;    
            menuparam.dst_handle = win_id;
            menuparam.func.OnMenuInit = HandleSmsCBChannelOptWindowInit;
            menuparam.func.OnMenuOk = HandleSmsCBChannelOptWindowOk;                    
            MMIPUB_OpenPubMenuWin( &menuparam );
        }
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
        if (!IsCBChannelListFull())
        { 
            //MMK_CreatePubFormWin((uint32 *)MMISMS_CB_ADD_EDIT_CHANNEL_WIN_TAB, PNULL);
            MMK_CreateWin((uint32 *)MMISMS_CB_ADD_EDIT_CHANNEL_WIN_TAB, PNULL);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(STR_CB_LIST_FULL);
        }
        break;
#endif
    
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //��õ�ǰѡ�е��ŵ����б��е�������
        {
            index = GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);
			
#ifdef MMI_ETWS_SUPPORT
            NV_LoadSmsCBChannelList();
            SCI_MEMCPY(&cur_channel_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
            s_cur_channel_index = GetCurChannelIndex(index);

            if( etws_alert_num > 0)
            {
                MN_CB_ALERT_MSG_T *cb_msg_id_ptr = PNULL;
                InitCB_ETWSChannel(dual_sys);
                etws_channel_list_ptr = MMISMSCB_Get_ETWS_Channel_Ptr(dual_sys);
                SCI_MEMCPY(&etws_channel_list, etws_channel_list_ptr, sizeof(MMISMS_CB_CHANNEL_T));

                // Cannot disable alart if the channel default is enabled
                if( etws_alert_num > 0 && (index < s_visible_channel_num))
                {
                    current_channel_info = GetCMASChannelInfo(dual_sys, cur_channel_list.channel_arr[s_cur_channel_index].msg_id);
                    if( NULL != current_channel_info)
                    {
                        if( current_channel_info->can_be_disabled)
                        {
                            SwitchCBChannel(index, s_cur_channel_index);
                        }
                    }
                }
                else
                {
                    SwitchCBChannel(index, s_cur_channel_index);
                }
                if( TRUE == s_is_it_pop_toast  )
                {
                    if(MMK_IsOpenWin(MMICOMMON_BANNER_WIN_ID))
                    {
                        MMK_CloseWin(MMICOMMON_BANNER_WIN_ID);
                    }
                    banner_info.banner_type = MMI_BANNER_TEXT;
                    banner_info.text1_id = text_cb_ita_IT_Alert_pop_toast;//text��Ҫ������Դ,������ȷ��ʾ
                    banner_info.start_point = alert_start_point;
                    MMIAPICOM_OpenBannerWin(win_id, banner_info);
                    s_is_it_pop_toast = FALSE;
                }
            }
            else
#endif
            {
                index += MMI_CB_ALERT_NUM;
                SwitchCBChannel(index, index);
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
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
#else
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBChannelListWindow
//  ��������:        �㲥�ŵ�ѡ���б��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleSmsCBChannelListWindow(
                                                MMI_WIN_ID_T    win_id,     // STARTUP NORMAL���ڵ�ID
                                                MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                                DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    uint16 index=0;
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_CHANNEL_T cur_channel_list = {0};
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MMISMS_CB_CHANNEL_T etws_channel_list = {0};
    MMISMS_CB_CHANNEL_T* etws_channel_list_ptr = PNULL;
#endif
    
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMK_SetAtvCtrl(win_id,MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        InitCBChannelList();
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBChannelList();
        SCI_MEMCPY(&channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));
#else
        channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif
        index=GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);
        //�����Ѵ򿪵��ŵ���򿪵�һ��Ϊ"�ر��ŵ�"��С���㲥�ŵ������˵�����
        //����򿪵�һ��Ϊ"���ŵ�"��С���㲥�ŵ������˵�����
        if(channel_list.channel_arr[index].is_enabled ||
            channel_list.num == 0)
        {
            //MMK_CreateWin((uint32*) MMISMS_CB_CHANNEL_OPT_B_WIN_TAB , PNULL );
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_CB_CHANNEL_LIST_OPT_B;
                menuparam.win_id =   MMISMS_CB_CHANNEL_OPT_WIN_ID;
                menuparam.ctrl_id =  MMISMS_CB_CHANNEL_LIST_MENU_CTRL_ID;    
                menuparam.dst_handle = win_id;
                menuparam.func.OnMenuInit = HandleSmsCBChannelOptWindowInit;
                menuparam.func.OnMenuOk = HandleSmsCBChannelOptWindowOk;
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }   
        }
        else
        {
            //MMK_CreateWin((uint32*) MMISMS_CB_CHANNEL_OPT_A_WIN_TAB , PNULL );
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_CB_CHANNEL_LIST_OPT_A;
                menuparam.win_id =   MMISMS_CB_CHANNEL_OPT_WIN_ID;
                menuparam.ctrl_id =  MMISMS_CB_CHANNEL_LIST_MENU_CTRL_ID;    
                menuparam.dst_handle = win_id;
                menuparam.func.OnMenuInit = HandleSmsCBChannelOptWindowInit;
                menuparam.func.OnMenuOk = HandleSmsCBChannelOptWindowOk;                    
                MMIPUB_OpenPubMenuWin( &menuparam );
            }
        }
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //��õ�ǰѡ�е��ŵ����б��е�������
        index = GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);
#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBChannelList();
        SCI_MEMCPY(&channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));
        s_cur_channel_index = GetCurChannelIndex(index);

        if( etws_alert_num > 0)
        {
            MN_CB_ALERT_MSG_T *cb_msg_id_ptr = PNULL;
            InitCB_ETWSChannel();
            etws_channel_list_ptr = MMISMSCB_Get_ETWS_Channel_Ptr(dual_sys);
            SCI_MEMCPY(&etws_channel_list, etws_channel_list_ptr, sizeof(MMISMS_CB_CHANNEL_T));
				
            // Cannot disable alart if the channel default is enabled
            if( etws_alert_num > 0 && (index < s_visible_channel_num))
            {
                current_channel_info = GetCMASChannelInfo(dual_sys, cur_channel_list.channel_arr[s_cur_channel_index].msg_id);
                if( NULL != current_channel_info)
                {
                    if( current_channel_info->can_be_disabled)
                    {
                        SwitchCBChannel(index, s_cur_channel_index);
                    }
                }
            }
            else
            {
                SwitchCBChannel(index, s_cur_channel_index);
            }
        }
        else
#endif
        {
            SwitchCBChannel(index, index);
        }

        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
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
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         InitCBChannelList
//  ��������:        ��ʼ���㲥�����б���
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void InitCBChannelList(void)
{
    uint32 i = 0, j = 0;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    uint16 current_index = 0;
    BOOLEAN is_alert_channel = FALSE;
#ifdef MMI_ETWS_SUPPORT
    uint16 etws_alert_num = 0;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MMISMS_CB_CHANNEL_NAME_T channel_name = {0};
    MMI_STRING_T etws_channel = {0};
#endif
    current_index = GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);

    
    //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif

    //�趨MMI_MAX_CB_CHANNEL_NUM��ѡ��
    GUILIST_SetMaxItem( MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID, MMI_MAX_CB_CHANNEL_NUM, FALSE );//max item 10
    GUILIST_SetMaxSelectedItem( MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID, MMI_MAX_CB_CHANNEL_NUM );    
	
#ifdef MMI_ETWS_SUPPORT
    etws_alert_num = MMIAPISMSCB_GetETWSNum();
#endif	
    //���ݴ�NV��ȡ���ŵ��б��������б�ؼ���ÿһ��
	
    if(channel_list.num > 0)
    {
        for(i=0;i<channel_list.num;i++)
        {
#ifdef MMI_ETWS_SUPPORT
            if( 0 == etws_alert_num ) 
#endif
            {
                for(j = 0; j < MMI_CB_ALERT_NUM;j++)
                {
                    if(channel_list.channel_arr[i].msg_id == MMIAPISMSCB_GetCBAlertID(j))
                    {
                        is_alert_channel = TRUE;
                        continue;
                    }
                }
                if(is_alert_channel &&(MMI_PER_INSTALL_PARAM != channel_list.channel_arr[i].msg_id))
                {
                    is_alert_channel = FALSE;
                    continue;
                }
            }

#ifdef MMI_ETWS_SUPPORT
            if( etws_alert_num > 0 && ( i < etws_alert_num))
            {
                current_channel_info = GetCMASChannelInfo(dual_sys, channel_list.channel_arr[i].msg_id);
                if( NULL != current_channel_info)
                {
                    if( TRUE == current_channel_info->can_be_visible )
                    {
                        if( 0 != current_channel_info->title_string_id  )
                        {
                            MMI_GetLabelTextByLang(current_channel_info->title_string_id , &etws_channel);
                            SCI_MEMSET(&channel_list.channel_arr[i].name, 0x00, sizeof(MMISMS_CB_CHANNEL_NAME_T));
                            channel_list.channel_arr[i].name.wstr_len = (uint16)MIN(MMI_CB_CHANNEL_NAME_LEN, etws_channel.wstr_len);
                            MMIAPICOM_Wstrncpy(channel_list.channel_arr[i].name.wstr, etws_channel.wstr_ptr, etws_channel.wstr_len) ;
                            AppendCBChannelListItem( MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID, channel_list.channel_arr[i] , j);
                        }
                        else if( 0 != current_channel_info->title_id)
                        {
                            channel_list.channel_arr[i].name.wstr_len = MMIAPICOM_Wstrlen(current_channel_info->title_id) +1;
                            MMIAPICOM_Wstrncpy(channel_list.channel_arr[i].name.wstr, current_channel_info->title_id, channel_list.channel_arr[i].name.wstr_len) ;
                            AppendCBChannelListItem( MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID, channel_list.channel_arr[i] , j);
                        }                                
                        j ++;
                    }
                }
            }
            else
            {
                AppendCBChannelListItem( MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID, channel_list.channel_arr[i] , j);
                j ++;
            }
#else
            AppendCBChannelListItem( MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID, channel_list.channel_arr[i], i );
#endif
        }
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(MMISMS_CB_CHANNEL_LIST_WIN_ID,  PNULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK, FALSE);
    }

    GUILIST_SetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID, current_index);
}
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBChannelOptWindowInit
//  ��������:        �㲥�ŵ������б�˵����ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void HandleSmsCBChannelOptWindowInit( MMIPUB_MENU_NOTIFY_T *param )
{
	//    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	//    uint16 index = 0;
	//    MMI_MENU_GROUP_ID_T group_id= param->group_id;
	//    MMI_MENU_ID_T menu_id = 0;
    
    MMI_MENU_GROUP_ID_T group_id = (MMI_MENU_GROUP_ID_T)MMK_GetWinUserData(param->win_id);
#ifdef MMI_ETWS_SUPPORT
    uint16 index = 0;
    uint16 etws_alert_num = 0;
    MMISMS_CB_CHANNEL_T* etws_channel_list_ptr = PNULL;

    etws_alert_num = MMIAPISMSCB_GetETWSNum();
#endif
    
    //����㲥�ŵ������б�˵�
#ifndef MMI_PDA_SUPPORT
    if(GUILIST_GetTotalItemNum(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID) == 0)
    {
        MMIAPICOM_EnableGrayed(param->win_id, group_id,MMISMS_MENUCB_CHANNEL_SWITCH_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(param->win_id, group_id,MMISMS_MENUCB_CHANNEL_EDIT_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(param->win_id, group_id,MMISMS_MENUCB_CHANNEL_DELETE_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(param->win_id, group_id,MMISMS_MENUCB_CHANNEL_CHECK_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(param->win_id, group_id,MMISMS_MENUCB_CHANNEL_ALL_DEL_ITEM_ID,TRUE);
    }
#ifdef MMI_ETWS_SUPPORT
    else
    {
        index = GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);
        if( etws_alert_num > 0)
        {
            MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
            MN_CB_ALERT_MSG_T *cb_msg_id_ptr = PNULL;
            MMISMS_CB_CHANNEL_T etws_channel_list = {0};

            InitCB_ETWSChannel(dual_sys);
            etws_channel_list_ptr = MMISMSCB_Get_ETWS_Channel_Ptr(dual_sys);
            SCI_MEMCPY(&etws_channel_list, etws_channel_list_ptr, sizeof(MMISMS_CB_CHANNEL_T));

            s_cur_channel_index = GetCurChannelIndex(index);

            // Cannot disable alart if the channel default is enabled
            if( index < s_visible_channel_num )
            {
                MMIAPICOM_EnableGrayed(param->win_id, group_id,MMISMS_MENUCB_CHANNEL_EDIT_ITEM_ID,TRUE);
                MMIAPICOM_EnableGrayed(param->win_id, group_id,MMISMS_MENUCB_CHANNEL_DELETE_ITEM_ID,TRUE);
                MMIAPICOM_EnableGrayed(param->win_id, group_id,MMISMS_MENUCB_CHANNEL_ALL_DEL_ITEM_ID,TRUE);
            }
            else
            {
                MMIAPICOM_EnableGrayed(param->win_id, group_id,MMISMS_MENUCB_CHANNEL_ALL_DEL_ITEM_ID,TRUE);
            }
        }
    }
#endif /* MMI_ETWS_SUPPORT */
#else 
    {
        GUIMENU_BUTTON_INFO_T button_info = {0};
        button_info.is_static = TRUE;
        button_info.group_id = group_id;
        button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(param->ctrl_id, &button_info);
    }
#endif
    
    MMK_SetAtvCtrl(param->win_id, param->ctrl_id);
    GUIWIN_SetSoftkeyTextId(param->win_id,  PNULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK, FALSE);
    return ;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBChannelOptWindowOk
//  ��������:        �㲥�ŵ������б�˵����ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void HandleSmsCBChannelOptWindowOk( MMIPUB_MENU_NOTIFY_T *param )
{
//    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 index = 0;
    MMI_MENU_GROUP_ID_T group_id= param->group_id;
    MMI_MENU_ID_T menu_id = param->menu_id;
    MMI_HANDLE_T win_handle = param->win_handle;
	
    //added by Tong Yongli
    uint16 i = 0;
    BOOLEAN flag = FALSE;
    MN_CB_MSG_ID_T cb_msg_id = 0;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMI_STRING_T prompt_str = {0};
    MMI_WIN_ID_T query_win_id = MMIPUB_ALERT_WIN_ID;
#ifdef MMI_ETWS_SUPPORT
    uint16 etws_alert_num = 0;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MMISMS_CB_CHANNEL_T etws_channel_list = {0};
    MMISMS_CB_CHANNEL_T cur_channel_list = {0};
    MMISMS_CB_CHANNEL_T* etws_channel_list_ptr = PNULL;

    etws_alert_num = MMIAPISMSCB_GetETWSNum();
#endif    

    index = GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);

#ifdef MMI_ETWS_SUPPORT
    s_cur_channel_index = GetCurChannelIndex(index);
    index = s_cur_channel_index;
#endif

#ifdef MMI_ETWS_SUPPORT
    if( 0 == etws_alert_num  )
#endif
        index += MMI_CB_ALERT_NUM;
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif
	
#ifdef MMI_ETWS_SUPPORT
    if( 0 == etws_alert_num  )
#endif
    {
        for(i = 0; i < MMI_CB_ALERT_NUM; i++)
        {
            cb_msg_id=  MMIAPISMSCB_GetCBAlertID(i);
            if(cb_msg_id == channel_list.channel_arr[index].msg_id)
            {
                flag = TRUE;
                break;
            }
        }
    }

    //�жϹ㲥�б�����˵��ڼ��ѡ��
    GUIMENU_GetId(param->ctrl_id, &group_id, &menu_id);
    switch (menu_id)
    {
    case MMISMS_MENUCB_CHANNEL_SWITCH_ITEM_ID:    //�ŵ�����\ȡ���˵���
        //��õ�ǰѡ�е��ŵ����б��е�������
        //index = GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);
#ifdef MMI_ETWS_SUPPORT
            NV_LoadSmsCBChannelList();
            SCI_MEMCPY(&cur_channel_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
            s_cur_channel_index = GetCurChannelIndex(index);                

            if( etws_alert_num > 0)
            {
                MN_CB_ALERT_MSG_T *cb_msg_id_ptr = PNULL;
                InitCB_ETWSChannel(MMISMSCB_GetSelectCard());
                etws_channel_list_ptr = MMISMSCB_Get_ETWS_Channel_Ptr(dual_sys);
                SCI_MEMCPY(&etws_channel_list, etws_channel_list_ptr, sizeof(MMISMS_CB_CHANNEL_T));

                // Cannot disable alart if the channel default is enabled
                if( etws_alert_num > 0 && (index < s_visible_channel_num))
                {
                    current_channel_info = GetCMASChannelInfo(dual_sys, cur_channel_list.channel_arr[s_cur_channel_index].msg_id);
                    if( NULL != current_channel_info)
                    {
                        if( current_channel_info->can_be_disabled)
                        {
                            SwitchCBChannel(index, s_cur_channel_index);
                        }
                    }
                }
                else
                {
                    SwitchCBChannel(index, s_cur_channel_index);
                }
            }
            else
#endif
                SwitchCBChannel(index, index);
        break;
        
    case MMISMS_MENUCB_CHANNEL_EDIT_ITEM_ID:      //�༭�ŵ��˵���
        //#ifdef MMI_PDA_SUPPORT
        //MMK_CreatePubFormWin((uint32 *)MMISMS_CB_ADD_EDIT_CHANNEL_WIN_TAB, PNULL);
        //#else
        {
            BOOLEAN *isAdd_ptr = SCI_ALLOCA(sizeof(BOOLEAN));
            *isAdd_ptr = FALSE;
            //added by Tong Yongli
            if(flag)
            {
                MMIPUB_OpenAlertWarningWin(STR_CB_IMPORTANT);
                return;
            } 
            MMK_CreateWin((uint32 *)MMISMS_CB_ADD_EDIT_CHANNEL_WIN_TAB, (ADD_DATA)isAdd_ptr);
        }
       // #endif
        break;
        
    case MMISMS_MENUCB_CHANNEL_DELETE_ITEM_ID:    //ɾ���ŵ��˵���
        //added by Tong Yongli
        if(flag)
        {
            MMIPUB_OpenAlertWarningWin(STR_CB_IMPORTANT);
            return;
        }
        //�����Ƿ�ɾ����ѯ�ʴ���
        //MMIPUB_OpenQueryWinByTextId(STR_CB_DELETE_CHANNEL,IMAGE_PUBWIN_QUERY,PNULL,HandleCBDelQueryWinMsg);
        MMI_GetLabelTextByLang(STR_CB_DELETE_CHANNEL_QUERY, &prompt_str);
        MMIPUB_OpenConfirmationDialogByTextPtr(  PNULL,&prompt_str,PNULL, IMAGE_NULL,          
                                         &query_win_id, PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,  HandleCBDelQueryWinMsg);
        MMK_CloseWin(win_handle);
        break;

    case MMISMS_MENUCB_CHANNEL_ALL_DEL_ITEM_ID:  //ȫ��ɾ���ŵ��˵���added by Tong Yongli
        //�����Ƿ�ɾ����ѯ�ʴ���
        MMI_GetLabelTextByLang(STR_CB_DELETE_ALL_QUERY, &prompt_str);
        MMIPUB_OpenConfirmationDialogByTextPtr(  PNULL,&prompt_str,PNULL, IMAGE_NULL,          
                                         &query_win_id, PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,  HandleCBAllDelQueryWinMsg);
        //MMIPUB_OpenQueryWinByTextId(STR_CB_DELETE_ALL,IMAGE_PUBWIN_QUERY,PNULL,HandleCBAllDelQueryWinMsg);
        MMK_CloseWin(win_handle);
        break;
        
    case MMISMS_MENUCB_CHANNEL_CHECK_ITEM_ID:     //�鿴�ŵ��Ų˵���
        MMK_CreateWin((uint32 *)MMISMS_CB_CHANNEL_CHECK_WIN_TAB, PNULL );
        MMK_CloseWin(win_handle);
        break;

    case MMISMS_MENUCB_CHANNEL_ADD_ITEM_ID:
        if (!IsCBChannelListFull())
        { 
            //#ifdef MMI_PDA_SUPPORT
            //MMK_CreatePubFormWin((uint32 *)MMISMS_CB_ADD_EDIT_CHANNEL_WIN_TAB, PNULL);
            //#else
            BOOLEAN *isAdd_ptr = SCI_ALLOCA(sizeof(BOOLEAN));
            *isAdd_ptr = TRUE;
            MMK_CreateWin((uint32 *)MMISMS_CB_ADD_EDIT_CHANNEL_WIN_TAB, (ADD_DATA)isAdd_ptr);
            //#endif
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(STR_CB_LIST_FULL);
            MMK_CloseWin(win_handle);
        }	
        break;
            
    default:
        break;
    }
    
    return;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         SwitchCBChannel
//  ��������:        ����ָ���ŵ�
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void SwitchCBChannel(
                           uint16 index,    //Ҫ���ص��ŵ����б��е�������  (�������)
                           uint16 channel_index
                           )
{
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMISMS_CB_LANGUAGE_T lang_list = {0};
    MN_CB_MSG_ID_T cb_msg_id = 0;
    uint8 i = 0;
#ifdef MMI_ETWS_SUPPORT
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    uint16 etws_alert_num = 0;
    uint16 nv_index = channel_index;
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    BOOLEAN  is_need_enabled = FALSE;
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;

    etws_alert_num = MMIAPISMSCB_GetETWSNum();
#endif    
    
    //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif
	
#ifdef MMI_ETWS_SUPPORT
    if( 0 == etws_alert_num  )
#endif
    {
        //added by Tong Yongli
        for(i = 0; i < MMI_CB_ALERT_NUM; i++)
        {
            cb_msg_id=  MMIAPISMSCB_GetCBAlertID(i);
            if((cb_msg_id == channel_list.channel_arr[index].msg_id) && (cb_msg_id != MMI_PER_INSTALL_PARAM))
            {
                return;
            }
        }
    }

    if(channel_list.num == 0)
    {
        return;
    }

#ifdef MMI_ETWS_SUPPORT
    SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
#endif

    if(TRUE==is_set_mcc)
    {
        plmn_info.mcc  = s_set_mcc;
    }

    //��NV��ȡ�����б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBLanguageList(dual_sys, &lang_list);
#else
    lang_list=NV_LoadSmsCBLanguageList(MMISMSCB_GetSelectCard());
#endif
    
    //����ǰ���ŵ�������־ȡ��
#ifdef MMI_ETWS_SUPPORT
    if( CB_GR_MCC == plmn_info.mcc )
    {
        if( TRUE == channel_list.channel_arr[nv_index].is_enabled )
        {
            channel_list.channel_arr[nv_index].is_enabled=FALSE;

            // Enable channel 919
            if(  (4370 == channel_list.channel_arr[nv_index].msg_id )
            ||  (4371 == channel_list.channel_arr[nv_index].msg_id )
            ||  (4375 == channel_list.channel_arr[nv_index].msg_id ) )
            {
                 for( i = 0; i < channel_list.num; i ++)
                {
                    if( 919 == channel_list.channel_arr[i].msg_id )
                    {
                        channel_list.channel_arr[i].is_enabled = TRUE;
                        break;
                    }
                }
            }
            // Enable channel 920 
            else if( 4379 == channel_list.channel_arr[nv_index].msg_id )
            {
                 for( i = 0; i < channel_list.num; i ++)
                {
                    if( 920 == channel_list.channel_arr[i].msg_id )
                    {
                        channel_list.channel_arr[i].is_enabled = TRUE;
                        break;
                    }
                }
             }
         }
        else if( FALSE == channel_list.channel_arr[nv_index].is_enabled )
        {
                switch( channel_list.channel_arr[nv_index].msg_id )
                {
                    case 4370:
                    case 4371:
                    case 4375:
                    {
                        channel_list.channel_arr[nv_index].is_enabled=TRUE;
                       for( i = 0; i < channel_list.num; i ++)
                       {
                            if( ( FALSE == channel_list.channel_arr[i].is_enabled )
                                && ( (4370 == channel_list.channel_arr[i].msg_id )
                                     ||  (4371 == channel_list.channel_arr[i].msg_id ) 
                                     ||  (4375 == channel_list.channel_arr[i].msg_id ) ) )
                            {
                                is_need_enabled = TRUE;
                                break;
                            }
                        }
                        // Set channel 919
                        for( i = 0; i < channel_list.num; i ++)
                        {
                            if( 919 == channel_list.channel_arr[i].msg_id )
                            {
                                if( TRUE == is_need_enabled )
                                {
                                      channel_list.channel_arr[i].is_enabled = TRUE;
                                      break;
                                 }
                                 else
                                 {
                                      channel_list.channel_arr[i].is_enabled = FALSE;
                                      break;
                                 }
                             }
                        }
                    }
                    break;

                    case 4379:
                   {
                        // Set channel 920
                        for( i = 0; i < channel_list.num; i ++)
                        {
                            if( 920 == channel_list.channel_arr[i].msg_id )
                            {
                                channel_list.channel_arr[i].is_enabled = FALSE;
                                break;
                             }
                        }
                        channel_list.channel_arr[nv_index].is_enabled=TRUE;
                        break;
                      }
                    
                  default:
                      channel_list.channel_arr[nv_index].is_enabled=TRUE;
                     break;
                }
          }
    }
    else
    {
         //����ǰ���ŵ�������־ȡ��
        if(channel_list.channel_arr[nv_index].is_enabled)
        {
            channel_list.channel_arr[nv_index].is_enabled=FALSE;
            for( i = nv_index +1; i < etws_alert_num; i ++)
            {
                 current_channel_info = GetCMASChannelInfo(dual_sys, channel_list.channel_arr[i].msg_id);
                if(PNULL != current_channel_info)
                {
                    if( current_channel_info->associated_channel == channel_list.channel_arr[nv_index].msg_id )
                    {
                        channel_list.channel_arr[i].is_enabled=FALSE;
                    }
                }
            }

            // disable IT Alert need to popup toast
            if( (CB_IT_MCC == plmn_info.mcc ) && (4371 == channel_list.channel_arr[nv_index].msg_id ) )
            {
                s_is_it_pop_toast = TRUE;
            }
        }
        else
        {
            channel_list.channel_arr[nv_index].is_enabled=TRUE;
            for( i = nv_index + 1; i < etws_alert_num; i ++)
            {
                 current_channel_info = GetCMASChannelInfo(dual_sys, channel_list.channel_arr[i].msg_id);
                if(PNULL != current_channel_info)
                {
                    if( current_channel_info->associated_channel == channel_list.channel_arr[nv_index].msg_id )
                    {
                        channel_list.channel_arr[i].is_enabled=TRUE;
                    }
                }
            }
        }
    }
#else
    if(channel_list.channel_arr[index].is_enabled)
    {
        channel_list.channel_arr[index].is_enabled=FALSE;
    }
    else
    {
        channel_list.channel_arr[index].is_enabled=TRUE;
    }
#endif

    SCI_TRACE_LOW("SwitchCBChannel index = %d.:",index);
    for(i = 0; i < channel_list.num; i++)
    {
        SCI_TRACE_LOW("channel_list.channel_arr[%d].msg_id = %d.",i,channel_list.channel_arr[i].msg_id);
    }
    
    return_val=MMISMSCB_Reset(&channel_list, &lang_list,MMISMSCB_GetSelectCard());    //�����趨С���㲥����
    
    if(return_val==MN_RETURN_SUCCESS)                     //��������ر��ŵ��ɹ�����ʾ�ɹ������浽NV��ȥ
    {   
        //MMIPUB_OpenAlertSuccessWin(TXT_SMS_CB_SETTING_SUCC);
        
#ifdef MMI_ETWS_SUPPORT
        SCI_MEMSET(&s_save_to_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T));
        SCI_MEMCPY(&s_save_to_nv_channel_list,&channel_list, sizeof(MMISMS_CB_CHANNEL_T));
        NV_SaveSmsCBChannelList();
#else
        NV_SaveSmsCBChannelList(&channel_list);             //�����ŵ��б� NV ��ȥ
#endif
#ifdef CB_CHANNEL_50_EXIST
        NV_SaveSmsCB50ChannelFlag(channel_list.channel_arr[index].is_enabled);
#endif
        GUILIST_SetSelectedItem( MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID, index-1, channel_list.channel_arr[nv_index].is_enabled );
        
        MMK_CloseWin( MMISMS_CB_CHANNEL_OPT_WIN_ID ); 
    }
    else //��������ر��ŵ�ʧ������ʾʧ��         
    {     
        MMIPUB_OpenAlertFailWin(TXT_COMMON_SETTING_FAIL);           
    }
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         DeleteCBChannel
//  ��������:        ɾ��ָ���ŵ�
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void DeleteCBChannel(
                           uint16 index    //Ҫɾ�����ŵ����б��е�������  (�������)
                           )
{
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    
    //�ӹ㲥�ŵ��б���ɾ��ָ����ĳһ���ŵ�
    return_val = DeleteCBChannelListItem(index);
    
    if(return_val == MN_RETURN_SUCCESS)       //��ɾ���ŵ��ɹ�����ʾ�ɹ������浽NV��ȥ
    { 
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMMON_DELETESUCESS);
#endif
        //����ŵ��б����¸�ֵ
        GUILIST_RemoveAllItems(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);
        InitCBChannelList();
        
        MMK_CloseWin( MMISMS_CB_CHANNEL_OPT_WIN_ID );
        //����ʱ�ŵ��б�Ϊ���򷵻ظ���һ���˵�
        //if(IsCBChannelListEmpty())                      
        //    MMK_CloseWin( MMISMS_CB_CHANNEL_LIST_WIN_ID );         
    }         
    else                                   //��ɾ���ŵ�ʧ������ʾɾ��ʧ��         
    {      
        MMIPUB_OpenAlertFailWin(TXT_COMMON_DEL_FAIL);  
    }
}

/*****************************************************************************/
//     Description : ���/�༭�㲥�ŵ����ڴ�����
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSCBChannelEditWindow(
                                                MMI_WIN_ID_T        win_id, //IN: ���ڵ�ID
                                                MMI_MESSAGE_ID_E     msg_id, //IN: ���ڵ��ڲ���ϢID
                                                DPARAM                 param    //IN: ��Ӧ��Ϣ�Ĳ���
                                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T title = {0};
    MMI_STRING_T test_str = {0};
    uint16 ver_space = 0;
    uint16 hor_space = 0;
    GUI_BG_T bg = {0};
    GUI_RECT_T  client_rect = MMITHEME_GetClientRect();
    BOOLEAN *isAdd_ptr = MMK_GetWinAddDataPtr(win_id);
    MMI_HANDLE_T win_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_CB_ADD_EDIT_CHANNEL_CTRL_ID);
    MMI_HANDLE_T channel_index_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_CB_CHANNEL_SHOW_INDEX_LABEL_CTRL_ID);
    MMI_HANDLE_T channel_name_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_CB_CHANNEL_SHOW_NAME_LABEL_CTRL_ID);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:


        //���ñ�ǩ����
        /*MMI_GetLabelTextByLang(STR_CB_CHANNEL_INDEX, &title);
        GUILABEL_SetText(MMISMS_CB_CHANNEL_INDEX_LABEL_CTRL_ID,&title,FALSE);
        MMI_GetLabelTextByLang(STR_CB_CHANNEL_NAME, &title);
        GUILABEL_SetText(MMISMS_CB_CHANNEL_NAME_LABEL_CTRL_ID,&title,FALSE);*/
        GUIEDIT_SetTextMaxLen(channel_index_ctr_id, CB_CHANNEL_INDEX_MAX_LEN, CB_CHANNEL_INDEX_MAX_LEN);
        GUIEDIT_SetTextMaxLen(channel_name_ctr_id, CB_CHANNEL_NAME_MAX_LEN, CB_CHANNEL_NAME_MAX_LEN);

		
        client_rect.bottom = client_rect.bottom -6;
        GUIFORM_SetRect(win_form_ctr_id, &client_rect);
        bg.bg_type = GUI_BG_COLOR;
        bg.color = MMITHEME_GetTransparenceColor();
        GUIFORM_SetBg(win_form_ctr_id, &bg);
        GUIFORM_SetCircularHandleUpDown(win_form_ctr_id,TRUE);
        GUIFORM_SetMargin(win_form_ctr_id,0);
        GUIFORM_PermitChildBorder(win_form_ctr_id,FALSE);

        GUIFORM_SetChildSpace(win_form_ctr_id, channel_index_ctr_id,&hor_space, &ver_space);
        GUIFORM_SetChildSpace(win_form_ctr_id, channel_name_ctr_id,&hor_space, &ver_space);

        GUIEDIT_PermitBorder(channel_index_ctr_id, FALSE);
        GUIEDIT_PermitBorder(channel_name_ctr_id, FALSE);

        CTRLBASEEDIT_SetLabelId(win_id,channel_index_ctr_id,STR_CB_CHANNEL_INDEX,FALSE);
        GUIEDIT_SetDefaultStringById(channel_index_ctr_id, STR_CB_ADD_INDEX);
        GUIEDIT_SetStyle(channel_index_ctr_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
        GUIEDIT_SetIm(channel_index_ctr_id, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);
		GUIEDIT_SetImSymbol(channel_index_ctr_id, TRUE);//Set to forbid symbol input
        CTRLBASEEDIT_SetLabelId(win_id,channel_name_ctr_id,STR_CB_CHANNEL_NAME,FALSE);
        GUIEDIT_SetDefaultStringById(channel_name_ctr_id, STR_CB_ADD_NAME);
        GUIEDIT_SetStyle(channel_name_ctr_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
        //GUIEDIT_SetIm(channel_name_ctr_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
        //�Ƿ�Ϊ�༭�ŵ�
        if (*isAdd_ptr)
        {              
            //�����ڵı�����Ϊ"����ŵ�"
            GUIWIN_SetTitleTextId(
                win_id,
                STR_CB_ADD_CHANNEL,
                FALSE
                );
        }
        else
        {
            //�����ڵı�����Ϊ"�༭�ŵ�"
            GUIWIN_SetTitleTextId(
                win_id,
                STR_CB_EDIT_CHANNEL,
                FALSE
                );
            //�趨�༭��ĳ�ʼֵ
            SetCBChannelStringInfo(win_id);
        }
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_SOFTKEY_SAVE_MK, FALSE);
        GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, MMISMS_CB_ADD_EDIT_CHANNEL_CTRL_ID);
 #endif
        //GUIWIN_SetSoftkeyTextId(win_id,  STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, MMISMS_CB_CHANNEL_SHOW_INDEX_LABEL_CTRL_ID);
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                if(GetChannelInfo(*isAdd_ptr))
                {
                    MMK_CloseWin( MMISMS_CB_CHANNEL_OPT_WIN_ID );		
                    MMK_CloseWin( win_id );
                }
                break;

            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
               MMK_CloseWin(MMISMS_CB_CHANNEL_OPT_WIN_ID);
               MMK_CloseWin(win_id);
                break;            
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
    case MSG_NOTIFY_TITLE_MENU_ICON:
        if(GetChannelInfo(*isAdd_ptr))
        {
            MMK_CloseWin( MMISMS_CB_CHANNEL_OPT_WIN_ID );		
            MMK_CloseWin( win_id );
        }
        
        break;
#endif    

    case MSG_FULL_PAINT:
    {
        GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
        LCD_FillRect(&lcd_dev_info, full_rect,MMI_BLACK_COLOR);
    }
    break;

    case MSG_GET_FOCUS:
        GUIWIN_UpdateStb();
        break;
#if 0
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
    {
        MMI_HANDLE_T src_handle = ((MMI_NOTIFY_T*)param)->src_handle;
        if(src_handle == channel_index_ctr_id)
        {
            GUIEDIT_GetString(channel_index_ctr_id, &test_str);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else if(src_handle == channel_name_ctr_id)
        {
            
        }
    }
    break;
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if(GetChannelInfo(*isAdd_ptr))
        {
            MMK_CloseWin( MMISMS_CB_CHANNEL_OPT_WIN_ID );		
            MMK_CloseWin( win_id );
        }
        break;
        
    case MSG_CTL_CANCEL:        
    case MSG_APP_CANCEL:
        MMK_CloseWin(MMISMS_CB_CHANNEL_OPT_WIN_ID);
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != isAdd_ptr)
        {
            SCI_FREE(isAdd_ptr);
        }
        break;
		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//     Description : get channel information
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetChannelInfo(BOOLEAN is_add)
{
    uint32 channel_index = 0;
    MMI_STRING_T index_str = {0};
    MMI_STRING_T name_str = {0};
    BOOLEAN result = FALSE;
    
    //����ŵ������༭����������ַ���
    GUIEDIT_GetString(MMISMS_CB_CHANNEL_SHOW_INDEX_LABEL_CTRL_ID, &index_str);
    
    //����ŵ����Ʊ༭����������ַ���
    GUIEDIT_GetString(MMISMS_CB_CHANNEL_SHOW_NAME_LABEL_CTRL_ID, &name_str);
    
    //����ŵ��������е�����
    WstrToLongNum(index_str.wstr_ptr, (uint8)index_str.wstr_len, &channel_index);
    
    if (index_str.wstr_len == 0)  //��������Ϊ���򱨴�
    {
        MMIPUB_OpenAlertWarningWin(TXT_SMS_CB_CHANNEL_INDEX_EMPTY);
    }
    else if (channel_index > 65535)
    {
        MMIPUB_OpenAlertWarningWin(STR_CB_INDEX_BIG);
    }
    else
    {
        //�����Ƿ�Ϊ�༭�ŵ�������ŵ���༭�ŵ�
        
        if (is_add)  
        {
            AddCBChannel(index_str, name_str);
        }
        else                   
        {
            EditCBChannel(index_str, name_str);
        }
        result = TRUE;
    }
    
    return (result);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         WstrToLongNum
//  ��������:        �������/�༭�㲥�ŵ������еı�ǩ�ؼ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN WstrToLongNum(wchar* str,uint8 len, uint32* num)
{
    uint8 i = 0;
    BOOLEAN result = FALSE;
    
    if (PNULL == num)
    {
        //SCI_TRACE_LOW:"MMISMS:WstrToLongNum num = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_3764_112_2_18_2_55_36_905,(uint8*)"");
        return FALSE;
    }
    if(NULL == str)
    {
        return FALSE;
    }
    
    for( i = 0; i < len; i++ ) 
    {
        result = TRUE;
        if( ( '0' <= str[i] ) && ( '9' >= str[i] ) ) 
        {
            *num = ( *num ) * 10 + ( str[i] - '0');
        }
        else
        {
            result = FALSE;
            break;
        }
    }
    
    return result;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         SetCBChannelStringInfo
//  ��������:        �������/�༭�㲥�ŵ������еı༭�ؼ��ĳ�ʼֵ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void SetCBChannelStringInfo(MMI_HANDLE_T win_id)
{
    uint16 index = 0;
    uint8 data[CB_CHANNEL_INDEX_MAX_LEN+1] = {0};
    wchar data_wstr[CB_CHANNEL_INDEX_MAX_LEN+1] = {0};
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMI_STRING_T string = {0};

    MMI_HANDLE_T channel_index_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_CB_CHANNEL_SHOW_INDEX_LABEL_CTRL_ID);
    MMI_HANDLE_T channel_name_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_CB_CHANNEL_SHOW_NAME_LABEL_CTRL_ID);
#ifdef MMI_ETWS_SUPPORT
    uint16 etws_alert_num = 0;
    etws_alert_num = MMIAPISMSCB_GetETWSNum();
#endif    
    
    index=GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);

#ifdef MMI_ETWS_SUPPORT
    s_cur_channel_index = GetCurChannelIndex(index);
    index = s_cur_channel_index;
#endif

#ifdef MMI_ETWS_SUPPORT
    if( 0 == etws_alert_num )
#endif		
        index += MMI_CB_ALERT_NUM;
    //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
    NV_LoadSmsCBChannelList();
    SCI_MEMCPY(&channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));
#else
    channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif

    //��������
    string.wstr_len = channel_list.channel_arr[index].name.wstr_len;
    string.wstr_ptr = channel_list.channel_arr[index].name.wstr;
    
    GUIEDIT_SetString(channel_name_ctr_id, string.wstr_ptr, string.wstr_len);
    
    //����index
    sprintf((char*)data,"%d",channel_list.channel_arr[index].msg_id);
    
    SCI_MEMSET(&string, 0, sizeof(string));

    MMI_STRNTOWSTR(data_wstr,
                                CB_CHANNEL_INDEX_MAX_LEN,
                                (uint8 *)data,
                                CB_CHANNEL_INDEX_MAX_LEN,
                                strlen((char*)data));

    string.wstr_ptr=data_wstr;
    string.wstr_len=strlen((char*)data);    

    GUIEDIT_SetString(channel_index_ctr_id, string.wstr_ptr, string.wstr_len);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         EditCBChannel
//  ��������:        �༭�㲥�ŵ�
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void EditCBChannel(
                         MMI_STRING_T index_str,    //Ҫ�༭���ŵ������ַ���   (�������)
                         MMI_STRING_T name_str      //Ҫ�༭���ŵ������ַ���   (�������)
                         )
{
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    uint16 index=0;
    uint32 channel_index = 0;
    MMISMS_CB_CHANNEL_T channel_list = {0};
#ifdef MMI_ETWS_SUPPORT
    uint16 etws_alert_num = 0;
    etws_alert_num = MMIAPISMSCB_GetETWSNum();
#endif    

    index=GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);

#ifdef MMI_ETWS_SUPPORT
    s_cur_channel_index = GetCurChannelIndex(index);
    index = s_cur_channel_index;
#endif

#ifdef MMI_ETWS_SUPPORT
    if( 0 == etws_alert_num  )
#endif
        index += MMI_CB_ALERT_NUM;
    //���Ѵ���һ����ͬ�������򱨴� 
    if(IsEditChannelIndexRepeat(index,index_str))       
    {
        MMIPUB_OpenAlertWarningWin(STR_CB_INDEX_EXIST);
    }
    else
    {
        //�༭�ŵ��б��е�ָ����
        return_val=EditCBChannelListItem(index_str,name_str,index);
        
        if(return_val==MN_RETURN_SUCCESS)         //���༭�ŵ��ɹ�����ʾ�ɹ�
        {           
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
#endif

            GUILIST_RemoveAllItems(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);
            InitCBChannelList();

            WstrToLongNum(index_str.wstr_ptr, (uint8)index_str.wstr_len, &channel_index);
#ifdef CB_CHANNEL_50_EXIST
            if(channel_index == MMI_PER_INSTALL_PARAM)
            {
#ifdef MMI_ETWS_SUPPORT
                NV_LoadSmsCBChannelList();
                SCI_MEMCPY(&channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));
#else
                channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif
                for(index  = 0; index < channel_list.num; index++)
                {
                    if((channel_list.channel_arr[index].msg_id == MMI_PER_INSTALL_PARAM) && (channel_list.channel_arr[index].is_enabled))
                    {
                        NV_SaveSmsCB50ChannelFlag(TRUE);
                    }
                    
                }
            }
#endif
            MMK_CloseWin( MMISMS_CB_CHANNEL_OPT_WIN_ID );
            MMK_CloseWin( MMISMS_CB_ADD_EDIT_CHANNEL_WIN_ID );
        }
        else    //���༭�ŵ�ʧ������ʾʧ��     
        {
            MMIPUB_OpenAlertFailWin(TXT_MMI_SAVEFAILED);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         AddCBChannel
//  ��������:        ��ӹ㲥�ŵ�
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddCBChannel(
                        MMI_STRING_T index_str,    //Ҫ��ӵ��ŵ������ַ���   (�������)
                        MMI_STRING_T name_str      //Ҫ��ӵ��ŵ������ַ���   (�������)
                        )
{
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;
    uint32 channel_index = 0;
    
    //���Ѵ���һ����ͬ�������򱨴� 
    if(IsAddChannelIndexRepeat(index_str))               
    {
        MMIPUB_OpenAlertWarningWin(STR_CB_INDEX_EXIST);
    }
    else
    {
        //��NV���ŵ��б�ṹ�����һ���ŵ�
        return_val=AddCBChannelListItem(index_str,name_str);
        
        if(return_val==MN_RETURN_SUCCESS)         //������ŵ��ɹ�����ʾ��ӳɹ�       
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
#endif
            WstrToLongNum(index_str.wstr_ptr, (uint8)index_str.wstr_len, &channel_index);
#ifdef CB_CHANNEL_50_EXIST
            if(channel_index == MMI_PER_INSTALL_PARAM)
            {
                NV_SaveSmsCB50ChannelFlag(TRUE);
            }
#endif
        }        
        else    
        {
            MMIPUB_OpenAlertFailWin(TXT_MMI_SAVEFAILED);
        }
        
        MMK_CloseWin( MMISMS_CB_ADD_EDIT_CHANNEL_WIN_ID );   
    }
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBChannelCheckWindow
//  ��������:        �鿴�㲥�ŵ����봰�ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleSmsCBChannelCheckWindow(
                                                 MMI_WIN_ID_T    win_id,     // STARTUP NORMAL���ڵ�ID
                                                 MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                                 DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                                 )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMI_STRING_T title = {0};
    uint16 index = 0;
    uint8 data[CB_CHANNEL_INDEX_MAX_LEN+1] = {0};
    wchar data_wstr[CB_CHANNEL_INDEX_MAX_LEN+1] = {0};
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    uint16 richtext_index = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;

        //channel index
        MMI_GetLabelTextByLang(STR_CB_CHANNEL_INDEX, &title);
        item_data.text_data.buf.len = title.wstr_len;
        item_data.text_data.buf.str_ptr = title.wstr_ptr;

        GUIRICHTEXT_AddItem(MMISMS_CB_CHANNEL_CHECK_CTRL_ID, &item_data, &richtext_index);

        //��NV��ȡ�ŵ��б�
#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBChannelList();
        SCI_MEMCPY(&channel_list, &s_nv_channel_list[MMISMSCB_GetSelectCard()], sizeof(MMISMS_CB_CHANNEL_T));
#else
        channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
#endif

        index=GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);

        //channel index
        sprintf((char*)data,"%d",channel_list.channel_arr[index].msg_id);
        MMI_STRNTOWSTR(data_wstr,
                                    CB_CHANNEL_INDEX_MAX_LEN,
                                    (uint8 *)data,
                                    CB_CHANNEL_INDEX_MAX_LEN,
                                    strlen((char*)data));

        item_data.text_data.buf.str_ptr = title.wstr_ptr=data_wstr;
        item_data.text_data.buf.len=strlen((char*)data);

        GUIRICHTEXT_AddItem(MMISMS_CB_CHANNEL_CHECK_CTRL_ID, &item_data, &richtext_index);

        //channel name
        MMI_GetLabelTextByLang(STR_CB_CHANNEL_NAME, &title);
        item_data.text_data.buf.str_ptr = title.wstr_ptr;
        item_data.text_data.buf.len = title.wstr_len;

        GUIRICHTEXT_AddItem(MMISMS_CB_CHANNEL_CHECK_CTRL_ID, &item_data, &richtext_index);

        item_data.text_data.buf.str_ptr=channel_list.channel_arr[index].name.wstr;
        item_data.text_data.buf.len=channel_list.channel_arr[index].name.wstr_len;

        //channel name
        GUIRICHTEXT_AddItem(MMISMS_CB_CHANNEL_CHECK_CTRL_ID, &item_data, &richtext_index);

        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        
        MMK_SetAtvCtrl(win_id, MMISMS_CB_CHANNEL_CHECK_CTRL_ID);
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

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBMsgListWindow
//  ��������:        �㲥�ռ����б��ڴ�����
//  ȫ�ֱ���ʹ��:    s_cb_msg
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleSmsCBMsgListWindow(
                                            MMI_WIN_ID_T        win_id,     //IN: ���ڵ�ID
                                            MMI_MESSAGE_ID_E     msg_id,     //IN: ���ڵ��ڲ���ϢID
                                            DPARAM                 param        //IN: ��Ӧ��Ϣ�Ĳ���
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_MSG_LISTBOX_CTRL_ID;
#ifdef MMI_GUI_STYLE_TYPICAL
    uint16 index = 0;
    uint16 pos = 0;
    MMI_STRING_T sms_msg = {0};
#endif

    //MMISMS_MSG_LIST_WIN_PARAM_T * param_to_create_msg_list_win = MMK_GetWinUserData(win_id);
        
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    case MSG_SMSCB_LIST_UPDATE:
        MMISMS_UpdateCBMsgList(ctrl_id);

#ifdef MMI_ETWS_SUPPORT
        GUIWIN_SetTitleTextId(win_id, STR_SMSTX_LIST_INBOX, FALSE);
#endif

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_MAIN_SETTING, FALSE);
        GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, MMISMS_CB_MSG_LISTBOX_CTRL_ID);
 #endif
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OPTION, LEFT_BUTTON, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:    
    case MSG_CTL_LIST_LONGOK:
#ifndef MMI_GUI_STYLE_TYPICAL
        //MMK_CreateWin((uint32 *)MMISMS_CB_LIST_OPT_WIN_TAB, PNULL); 
        {
            MMIPUB_MENU_PARAM_T menuparam = {0};
            menuparam.group_id = MENU_SMS_BROADCAST_LIST_OPT;
            menuparam.win_id =   MMISMS_CB_LISTOPT_WIN_ID;
            menuparam.ctrl_id =  MMISMS_CB_LISTOPT_CTRL_ID;    
            menuparam.dst_handle = win_id;
            menuparam.func.OnMenuInit = HandleSmsCBListOptWindowInit;
            menuparam.func.OnMenuOk = HandleSmsCBListOptWindowOk;                    
            MMIPUB_OpenPubMenuWin( &menuparam ); 
        }
#else
        index = GUILIST_GetCurItemIndex(ctrl_id);

        if(0 == MMISMS_Get_CB_Message_Num())
        {
            break;
        }
        else
        {
            MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;

            cur_msg_ptr = MMISMS_Get_CB_Message_Ptr();
            for (pos = 0; pos < index; pos++)
            {
                cur_msg_ptr = cur_msg_ptr->msg_next_ptr;
            }
            if (PNULL == cur_msg_ptr)
            {
                break;
            }

            sms_msg.wstr_len = cur_msg_ptr->len;
            sms_msg.wstr_ptr = cur_msg_ptr->context;
            MMISMS_WriteNewMessage(
                                    cur_msg_ptr->dual_sys,
                                    &sms_msg,
                                    FALSE,
                                    PNULL,
                                    0
                                    );
        }
#endif
        break;
        
#ifdef MMI_PDA_SUPPORT 
#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_NOTIFY_TITLE_MENU_ICON:
        MMISMSCB_OpenSettingWin();
        break;
        
    case MSG_APP_MENU:
        //MMK_CreateWin((uint32 *)MMISMS_CB_OPT_WIN_TAB, PNULL);
        {
            MMIPUB_MENU_PARAM_T menuparam = {0};
            menuparam.group_id = MENU_SMS_BROADCAST_MENU_OPT;
            menuparam.win_id = MMISMS_CB_LISTOPT_WIN_ID;
            menuparam.ctrl_id = MMISMS_CB_OPT_CTRL_ID;    
            menuparam.dst_handle = win_id;
            menuparam.func.OnMenuOk = HandleSmsCBOptWindow;                    
            MMIPUB_OpenPubMenuWin( &menuparam ); 
        }
        //MMISMSCB_OpenSettingWin();
        break;
#endif
#endif    
	
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:        
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(0 == MMISMS_Get_CB_Message_Num())
        {
            break;
        }
        else
        {
            MMK_CreateWin((uint32 *)MMISMS_CB_READ_WIN_TAB, PNULL);
        }
        break;
       
    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        //���Ұ���ʾ���ڹر�
        MMK_CloseWin(MMISMS_CB_NEWMSG_WIN_ID);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :get richtext rect
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void InitCBReadButtonForm(void)
{
    GUI_BG_T         bg = {0};
    GUI_BOTH_RECT_T both_rect = {0};
    GUI_BOTH_RECT_T client_rect = {0};
    client_rect = MMITHEME_GetWinClientBothRect(MMISMS_CB_READ_WIN_ID);
    
    both_rect.v_rect.left = client_rect.v_rect.left ;
    both_rect.v_rect.right = client_rect.v_rect.right;
    both_rect.v_rect.top = client_rect.v_rect.bottom - 58;
    both_rect.v_rect.bottom = client_rect.v_rect.bottom ;
    
    both_rect.h_rect.left = client_rect.h_rect.left;
    both_rect.h_rect.right = client_rect.h_rect.right;
    both_rect.h_rect.top = client_rect.h_rect.bottom - 58 ;
    both_rect.h_rect.bottom = client_rect.h_rect.bottom;
    GUIAPICTRL_SetBothRect(MMISMS_READ_FORM_CTRL_ID, &both_rect);
    GUIFORM_IsSlide(MMISMS_READ_FORM_CTRL_ID, FALSE);
    
    bg.bg_type = GUI_BG_IMG;                             
    GUIBUTTON_SetRunSheen(MMISMS_READ_FORWARD_CTRL_ID, FALSE);
    GUIBUTTON_SetRunSheen(MMISMS_READ_DELETE_CTRL_ID, FALSE);     
    
    bg.img_id = IMAGE_SMS_BUTTON_FORM_BG;                      
    GUIFORM_SetBg(MMISMS_READ_FORM_CTRL_ID, &bg);
    GUIFORM_SetMargin(MMISMS_READ_FORM_CTRL_ID, 6);
    
    bg.img_id = IMAGE_SMS_BUTTON_NORMAL;        
    
    GUIBUTTON_SetBg(MMISMS_READ_FORWARD_CTRL_ID, &bg);
    GUIBUTTON_SetBg(MMISMS_READ_DELETE_CTRL_ID, &bg);            
    
    bg.img_id = IMAGE_SMS_BUTTON_PRESSED;           
    GUIBUTTON_SetPressedBg(MMISMS_READ_FORWARD_CTRL_ID, &bg);
    GUIBUTTON_SetPressedBg(MMISMS_READ_DELETE_CTRL_ID, &bg);
    
    bg.img_id =  IMAGE_SMS_FORWARD_SMS_PDA;          
    GUIBUTTON_SetFg(MMISMS_READ_FORWARD_CTRL_ID, &bg);       
    GUIBUTTON_SetPressedFg(MMISMS_READ_FORWARD_CTRL_ID, &bg);
    
    bg.img_id =  IMAGE_SMS_DELETE_SMS_PDA;          
    GUIBUTTON_SetFg(MMISMS_READ_DELETE_CTRL_ID, &bg);       
    GUIBUTTON_SetPressedFg(MMISMS_READ_DELETE_CTRL_ID, &bg);     
}

/*****************************************************************************/
//  Description :get richtext rect
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CBGetTextRect(GUI_RECT_T *rect_ptr)
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
   
    rect_ptr->top = MMI_STATUSBAR_HEIGHT + MMI_TITLE_HEIGHT + 1;
    rect_ptr->right = mainscreen_width - 1;
    rect_ptr->bottom = mainscreen_height - 58 -1;  // 40 BUTTON FORM HEIGHT

    return TRUE;
}
#endif

#ifdef MMI_ETWS_SUPPORT
#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : handle extract number save contact
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void HandleCBMsgWinExtractNumberSaveContact(GUITEXT_TAG_INFO_T tag_info)
{
    uint8 *number_ptr = PNULL;
    uint8 num_len = 0;
    
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
    number_ptr = SCI_ALLOCAZ(tag_info.str_len + 1);
    if (PNULL == number_ptr)
    {
        return;
    }
    MMISMS_GetPhoneNumber(number_ptr, tag_info.str_len , tag_info.str_ptr, tag_info.str_len); //��-���ӵĺ��봦��
    // save to phonebook
    MMIAPICOM_GenPartyNumber(number_ptr, (uint16)SCI_STRLEN((char *)number_ptr), &party_num);
    
    pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
    pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
    SCI_MEMCPY(
        pb_bcd.number,
        party_num.bcd_num,
        pb_bcd.number_len
        );
    MMIAPIPB_AddContactWin(&pb_bcd, PB_GROUP_ALL, MMIPB_ADD_TYPE_NUMBER);  
    SCI_FREE(number_ptr);
    number_ptr = PNULL;
}
#endif
#endif
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBReadWindow
//  ��������:        ��ʾ�㲥���ݴ��ڴ�����
//  ȫ�ֱ���ʹ��:    s_cb_msg
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleSmsCBReadWindow(
                                         MMI_WIN_ID_T        win_id,     //IN: ���ڵ�ID
                                         MMI_MESSAGE_ID_E     msg_id,     //IN: ���ڵ��ڲ���ϢID
                                         DPARAM             param        //IN: ��Ӧ��Ϣ�Ĳ���
                                         )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_SHOW_MSG_TEXTBOX_CTRL_ID;
    uint16 index = 0;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    uint16 pos = 0;
    MMI_STRING_T sms_msg = {0}; 
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
#ifdef MMI_PDA_SUPPORT
    GUI_RECT_T rect_ptr = {0};
#endif
#ifdef MMI_ETWS_SUPPORT
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element            = {0};
    GUIRICHTEXT_FOCUS_TYPE_T focus_type                    = GUIRICHTEXT_FOCUS_NONE;
    wchar *focus_num_ptr                                 = PNULL;
    uint8 *number_ptr                                    = PNULL;    
    uint8 num_len = 0;    
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};    
    GUITEXT_TAG_INFO_T tag_info = {0};   
    CMAS_CHANNEL_INFO_T* current_channel_table = PNULL;
	uint16 i = 0;
    MMI_STRING_T etws_channel = {0};
    MMISMS_CB_CHANNEL_NAME_T channel_name = {0};
    BOOLEAN	is_exist_title_name = FALSE;
    wchar title_wstr[MMI_CB_CHANNEL_NAME_LEN+CB_MSG_TIME_MAX_LEN+1] = {0};
    uint16 title_len = 0;
    wchar time_str[CB_MSG_TIME_MAX_LEN + 1] = {0};	
    uint16 time_len = 0;
    MMISMS_CB_CHANNEL_NAME_T msg_channel_name = {0};
    uint8 channel_name_str[MMI_CB_CHANNEL_NAME_LEN] = {0};
#endif
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        current_channel_table = GetCMASChannelTable(dual_sys);
        is_exist_title_name = FALSE;

        index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
        
        cur_msg_ptr = MMISMS_Get_CB_Message_Ptr();
        
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
        //��ȡ����һ����ʱ����Ҫֹͣ����
        if (0 == index && !cur_msg_ptr->is_read)
        {
            GUISTBSCROLLMSG_SetIsValid(GUISTBSCROLLMSG_SCROLL_ITEM_NEW_SMSCB, FALSE);
        }
#endif
        
        for (pos = 0; pos < index; pos++)
        {
            cur_msg_ptr = cur_msg_ptr->msg_next_ptr;
        }
        if (PNULL == cur_msg_ptr)
        {
            MMK_CloseWin(win_id);
            break;
        }
        
        //SCI_TRACE_LOW:"SMSCB: HandleSmsCBReadWindow index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_4273_112_2_18_2_55_37_906,(uint8*)"d", index);
        
#ifdef MMI_ETWS_SUPPORT
        SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(cur_msg_ptr->dual_sys);
			
        if(TRUE==is_set_mcc)
        {
            plmn_info.mcc  = s_set_mcc;
        }
#endif

#ifdef MMI_ETWS_SUPPORT
        if( CB_PWS_MCC != plmn_info.mcc ) 
        {
            //��NV��ȡ�ŵ��б�
            NV_LoadSmsCBChannelList();
            SCI_MEMCPY(&channel_list, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
            i = 0;

            do
            {	
                if( cur_msg_ptr->msg_id == current_channel_table[i].channel_id )
                {
                    if( 0 != current_channel_table[i].popup_string_id )
                    {
                        is_exist_title_name = TRUE;
                        MMI_GetLabelTextByLang(current_channel_table[i].popup_string_id , &etws_channel);
                        channel_name.wstr_len = (uint16)MIN(MMI_CB_CHANNEL_NAME_LEN, etws_channel.wstr_len);
                        MMIAPICOM_Wstrncpy(channel_name.wstr, etws_channel.wstr_ptr, etws_channel.wstr_len) ;
                    }
                    else if( 0 != current_channel_table[i].title_string_id )
                    {
                        is_exist_title_name = TRUE;
                        MMI_GetLabelTextByLang(current_channel_table[i].title_string_id , &etws_channel);
                        channel_name.wstr_len = (uint16)MIN(MMI_CB_CHANNEL_NAME_LEN, etws_channel.wstr_len);
                        MMIAPICOM_Wstrncpy(channel_name.wstr, etws_channel.wstr_ptr, etws_channel.wstr_len) ;
                    }
                    else if( 0 != current_channel_table[i].title_id)
                    {
                        is_exist_title_name = TRUE;
                        channel_name.wstr_len  = MMIAPICOM_Wstrlen(current_channel_table[i].title_id) +1;
                        MMIAPICOM_Wstrncpy(channel_name.wstr, current_channel_table[i].title_id, etws_channel.wstr_len) ;
                    }                    
                    break;
                }
                else
                {
                    i++;
                }
            }
            while(0 != current_channel_table[i].channel_id);

            // display time at title
            if( (CB_CHILE_MCC == plmn_info.mcc) || (CB_IT_MCC == plmn_info.mcc ) || (CB_PE_MCC == plmn_info.mcc) )
            {
                if( TRUE == is_exist_title_name)
                {
                    title_len = channel_name.wstr_len;
                    MMIAPICOM_Wstrncpy(title_wstr, channel_name.wstr, channel_name.wstr_len) ;

                    MMIAPICOM_Wstrcat(title_wstr, L" ");
                    title_len = title_len + 1;

                    time_len = ConvertCBTimeToStrForTitle(
                    cur_msg_ptr->date,
                    cur_msg_ptr->time,
                    time_str
                    );
                    title_len = title_len + time_len + 1;
                    MMIAPICOM_Wstrcat(title_wstr, time_str);

                    GUIWIN_SetTitleText(win_id, title_wstr, title_len, FALSE);
                }
                else
                {
                    msg_channel_name = MMISMS_GetCBChannelNameByMsgId( &channel_list, cur_msg_ptr->msg_id);
    
                    if (0 == msg_channel_name.wstr_len)
                    {
                        msg_channel_name.wstr_len =  sprintf((char *)channel_name_str, "%d", cur_msg_ptr->msg_id );
                        MMI_STRNTOWSTR(msg_channel_name.wstr,
                                    MMI_CB_CHANNEL_NAME_LEN,
                                    (uint8 *)channel_name_str,
                                    MMI_CB_CHANNEL_NAME_LEN,
                                    msg_channel_name.wstr_len);
                        
                    }

                    title_len = msg_channel_name.wstr_len;
                    MMIAPICOM_Wstrncpy(title_wstr, msg_channel_name.wstr, msg_channel_name.wstr_len) ;

                    MMIAPICOM_Wstrcat(title_wstr, L" ");
                    title_len = title_len + 1;
                
                    time_len = ConvertCBTimeToStrForTitle(
                        cur_msg_ptr->date,
                        cur_msg_ptr->time,
                        time_str
                        );
                        title_len = title_len + time_len + 1;
                        MMIAPICOM_Wstrcat(title_wstr, time_str);

                    GUIWIN_SetTitleText(
                            win_id, 
                            title_wstr,
                            title_len,
                            FALSE
                            );
                }
            }
            else 
            {
                if( TRUE == is_exist_title_name)
                {
                    GUIWIN_SetTitleText(
                            win_id, 
                            channel_name.wstr,
                            channel_name.wstr_len,
                            FALSE
                            );
                }
                else
                {
                    SetChannelNameAsTitle(
                            win_id,
                            &channel_list,
                            cur_msg_ptr->msg_id
                            );
                }
            }
        }
#else /* MMI_ETWS_SUPPORT*/

            //��NV��ȡ�ŵ��б�
        channel_list = MMISMS_NV_LoadSmsCBChannelList(MMISMSCB_GetSelectCard());
        SetChannelNameAsTitle(
                win_id,
                &channel_list,
                cur_msg_ptr->msg_id
                );

#endif /* MMI_ETWS_SUPPORT*/

        //��ʾ�㲥��Ϣ������
        DisplayCBMsgContext(ctrl_id, cur_msg_ptr);
        MMK_PostMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_SMSCB_LIST_UPDATE, NULL, 0);
#ifdef MMI_PDA_SUPPORT
        MMISMS_CBGetTextRect(&rect_ptr);
        GUITEXT_SetRect(ctrl_id, &rect_ptr);
        InitCBReadButtonForm();
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_LCD_SWITCH:
        MMISMS_CBGetTextRect(&rect_ptr);
        GUITEXT_SetRect(ctrl_id, &rect_ptr);
        break;
#endif     
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if(param != PNULL)
        {
            MMI_CTRL_ID_T     button_id = 0;
            button_id = ((MMI_NOTIFY_T*)param)->src_id;
            switch(button_id)
            {
            case MMISMS_READ_FORWARD_CTRL_ID:
                MMK_SendMsg(win_id, MSG_CTL_OK, PNULL);
                break;                  
            case MMISMS_READ_DELETE_CTRL_ID:
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                break;
            default:
                break;                              
            }           
        }   
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
#if 0
    case MSG_CTL_OK:
    case MSG_APP_OK:
        index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
        
        if(0 == MMISMS_Get_CB_Message_Num())
        {
            break;
        }
        else
        {
            cur_msg_ptr = MMISMS_Get_CB_Message_Ptr();
            for (pos = 0; pos < index; pos++)
            {
                cur_msg_ptr = cur_msg_ptr->msg_next_ptr;
            }
            if (PNULL == cur_msg_ptr)
            {
                break;
            }
            
            sms_msg.wstr_len = cur_msg_ptr->len;
            sms_msg.wstr_ptr = cur_msg_ptr->context;
            MMISMS_WriteNewMessage(
                cur_msg_ptr->dual_sys,
                &sms_msg,
                FALSE,
                PNULL,
                0
                );
        }
        
        break;        
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        
        index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
        
        MMISMS_DeleteMsgFromCBMsgList((uint16)index);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
#endif
        
        //if(0 < s_cb_msg.num)
        {
            MMK_PostMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_SMSCB_LIST_UPDATE, NULL, 0);
        }
        //else
        //{
        //    MMK_CloseWin(MMISMS_CB_MSG_LIST_WIN_ID);
        //    MMIPUB_OpenAlertWarningWin(TXT_SMS_CB_LIST_EMPTY);
        //}
        
        MMK_CloseWin(win_id);
        break;
#endif 

#ifdef MMI_ETWS_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT 
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT  
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef MMI_SMS_CHAT_SUPPORT		
        {
            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            
            switch(focus_type) 
            {
            case GUIRICHTEXT_FOCUS_PHONENUM:
                tag_info.type = GUITEXT_TAG_PHONENUM;
                tag_info.str_len = richtext_element.data.len;
                if(0 != tag_info.str_len)
                {
                    tag_info.str_ptr = (wchar *)SCI_ALLOC_APPZ((tag_info.str_len + 1) * sizeof(wchar));
                    if(PNULL != tag_info.str_ptr)
                    {
                        MMIAPICOM_Wstrncpy(tag_info.str_ptr, richtext_element.data.str_ptr, tag_info.str_len);
                    }
                }
                HandleCBMsgWinExtractNumberSaveContact(tag_info);
                if(PNULL != tag_info.str_ptr)
                {
                    SCI_FREE(tag_info.str_ptr);
                }
                break;
                
            case GUIRICHTEXT_FOCUS_URL:
                {
#ifdef BROWSER_SUPPORT
                    MMIBROWSER_ENTRY_PARAM entry_param = {0}; 
                    uint8 *url_ptr  = PNULL;
                    uint16 url_len = 0;
                    url_len = MMIAPICOM_Wstrlen( richtext_element.data.str_ptr);
                    url_ptr = SCI_ALLOC_APPZ(url_len + 1);
                    
                    if (PNULL == url_ptr)
                    {
                        break;
                    }
                    MMI_WSTRNTOSTR(url_ptr, url_len,  richtext_element.data.str_ptr, url_len, url_len);
                    entry_param.type = MMIBROWSER_ACCESS_URL;
                    entry_param.dual_sys = MN_DUAL_SYS_MAX;
                    entry_param.url_ptr = (char *)url_ptr;
                    entry_param.user_agent_ptr = PNULL;
#ifdef  CMCC_UI_STYLE
                    entry_param.is_alert= TRUE;
#endif
                    MMIAPIBROWSER_Entry(&entry_param);
                    if(PNULL != url_ptr)
                    {
                        SCI_FREE(url_ptr);
                    }
                    
#endif
                }
                break;
                
            default:
                break;
            }
        }
#endif
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        {
            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            
            if(focus_type != GUIRICHTEXT_FOCUS_NONE)
            {            
                switch(focus_type) 
                {
                case GUIRICHTEXT_FOCUS_PHONENUM:
                    {
                        MMIPUB_MENU_PARAM_T menuparam = {0};
                        menuparam.group_id = MENU_SMS_BROADCAST_EXTRACT_NUMBER_POP_MENU;
                        menuparam.win_id =   MMISMS_CB_EXTRACT_NUMBER_POP_WIN_ID;
                        menuparam.ctrl_id =  MMISMS_CB_EXTRACT_NUMBER_POP_CTRL_ID;    
                        menuparam.dst_handle = win_id;
                        menuparam.func.OnMenuOk = HandleSmsCBExtractNumberOptOk;    
                        MMIPUB_OpenPubMenuWin( &menuparam );   
                        break;
                    }
                    
                default:
                    break;
                }
            }
            
        }
        break;
#endif            
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    case MSG_CTL_CANCEL:        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef MMI_ETWS_SUPPORT
#ifndef MMI_PDA_SUPPORT
    case MSG_FULL_PAINT:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        {
            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            
            if(focus_type != GUIRICHTEXT_FOCUS_NONE)
            {            
                switch(focus_type) 
                {
                case GUIRICHTEXT_FOCUS_PHONENUM:
                    {
                        GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_MENU_MK, LEFT_BUTTON,FALSE);                        
                        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SAVE_MK, MIDDLE_BUTTON, FALSE);        
                        GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  FALSE);
                        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
                        GUIWIN_UpdateSoftkey(win_id);
                        break;
                    }
                    
                case GUIRICHTEXT_FOCUS_URL:
                    {
                        GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_NULL, LEFT_BUTTON,FALSE);                      
                        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_GO_TO_MK, MIDDLE_BUTTON, FALSE);       
                        GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  FALSE);
                        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
                        GUIWIN_UpdateSoftkey(win_id);
                        break;
                    }
                    
                default:
                    break;
                }
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK, FALSE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
                GUIWIN_UpdateSoftkey(win_id);
            }
        }
        break;
#endif // MMI_PDA_SUPPORT
        
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
#ifdef VT_SUPPORT
    case MSG_KEYLONG_GREEN:
#endif
        {
            CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
            uint32 call_type = CC_CALL_NORMAL_CALL;
            CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
            
#ifdef VT_SUPPORT
            if(MSG_KEYLONG_GREEN == msg_id)
            {
                call_type = CC_CALL_VIDEO_CALL;
            }
#endif
            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            GUIRICHTEXT_GetFocusItemIndex(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,&index);
            
            if(focus_type ==GUIRICHTEXT_FOCUS_PHONENUM && 0 != index)
            {
                uint16 min_num_len = MIN(richtext_element.data.len,MMISMS_PBNUM_MAX_LEN);
                
                focus_num_ptr = SCI_ALLOC_APP((min_num_len + 1)*sizeof(wchar));
                SCI_MEMSET(focus_num_ptr, 0, ((min_num_len + 1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,min_num_len+1);
                number_ptr = SCI_ALLOC_APP(min_num_len + 1);
                SCI_MEMSET(number_ptr, 0, (min_num_len + 1));
                MMI_WSTRNTOSTR((uint8 *)number_ptr, MMISMS_PBNUM_MAX_LEN, focus_num_ptr, richtext_element.data.len, min_num_len);
                
                {
                    cc_result = MMIAPICC_MakeCall(
                                                  dual_sys,
                                                  number_ptr,
                                                  richtext_element.data.len,
                                                  PNULL,
                                                  PNULL,
                                                  sim_type,
                                                  call_type,
                                                  PNULL
                                                  );
                }
                SCI_FREE(focus_num_ptr);
                SCI_FREE(number_ptr);
                focus_num_ptr = PNULL;
                number_ptr = PNULL;
            }
            else
            {
                num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
                
                if (num_len > 0)
                {
                    cc_result = MMIAPICC_MakeCall(
                                                  dual_sys,
                                                  number,
                                                  num_len,
                                                  PNULL,
                                                  PNULL,
                                                  sim_type,
                                                  call_type,
                                                  PNULL
                                                  );
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                }
            }            
        }
        break;
#else //MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:   
        {
            CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX);
            
            uint32 call_type = CC_CALL_NORMAL_CALL;
            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            GUIRICHTEXT_GetFocusItemIndex(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,&index);
            if(focus_type ==GUIRICHTEXT_FOCUS_PHONENUM && 0 != index)
            {
                uint16 min_num_len = MIN(richtext_element.data.len,MMISMS_PBNUM_MAX_LEN);
                
                focus_num_ptr = SCI_ALLOC_APP((min_num_len + 1)*sizeof(wchar));
                SCI_MEMSET(focus_num_ptr, 0, ((min_num_len + 1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,min_num_len+1);
                number_ptr = SCI_ALLOC_APP(min_num_len + 1);
                SCI_MEMSET(number_ptr, 0, (min_num_len + 1));
                MMI_WSTRNTOSTR((uint8 *)number_ptr, MMISMS_PBNUM_MAX_LEN, focus_num_ptr, richtext_element.data.len, min_num_len);
                
                {
                    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
                    
                    cc_result = MMIAPICC_MakeCall(
                                                  dual_sys,
                                                  number_ptr,
                                                  richtext_element.data.len,
                                                  PNULL,
                                                  PNULL,
                                                  sim_type,
                                                  call_type,
                                                  PNULL
                                                  );
                }
                SCI_FREE(focus_num_ptr);
                SCI_FREE(number_ptr);
                focus_num_ptr = PNULL;
                number_ptr = PNULL;
            }
            else
            {
                num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
                
                if (num_len > 0)
                {
                    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
                    
                    cc_result = MMIAPICC_MakeCall(
                                                  dual_sys,
                                                  number,
                                                  num_len,
                                                  PNULL,
                                                  PNULL,
                                                  sim_type,
                                                  call_type,
                                                  PNULL
                                                  );
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                }            
            }
        }
        break;
#endif //MMI_MULTI_GREEN_KEY_SINGLE
        
#endif //MMI_ETWS_SUPPORT   
        
    default:        
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsETWSReadWindow
//  ��������:        ��ʾCLASS0���㲥���ݴ��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleSmsETWSReadWindow(
                                               MMI_WIN_ID_T            win_id, //IN: ���ڵ�ID
                                               MMI_MESSAGE_ID_E     msg_id, //IN: ���ڵ��ڲ���ϢID
                                               DPARAM                 param    //IN: ��Ӧ��Ϣ�Ĳ���
                                               )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMISMS_CB_MESSAGE_T* cur_idle_cb_msg = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
    uint16 i = 0;
    MMISMS_CB_MESSAGE_ARRAY_T* cb_message_array = PNULL;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_ETWS_IDLE_RICHTEXT_CTRL_ID;
    BOOLEAN is_display_older_cb_msg = FALSE;
    uint16 index = 0;

    SCI_TRACE_LOW("[MMISMSCB]: HandleSmsETWSReadWindow, msg_id = 0x%X", msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_UNISOC_IDLE_EMERGENCY_CALL_SUPPORT
        if( MMK_IsOpenWin(MMIIDLE_EMERGENCY_CALL_WIN_ID) || (TRUE == MMIAPISMSCB_GetEMCallStatus()) )
#else            
        if( TRUE == MMIAPISMSCB_GetEMCallStatus() )            
#endif            
        {
            MMIAPISMSCB_SetIsNeedRedisplay(TRUE);
            MMK_CloseWin(win_id);
            break;
        }
        else
        {
            MMIAPISMSCB_SetIsNeedRedisplay(FALSE);
        }

        if( TRUE == MMIAPISMSCB_GetIsNeedEnable())
        {
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, STXT_SOFTKEY_DISMISS_MK, TXT_NULL, FALSE);
        }

        MMK_SetWinPriority(MMK_ConvertIdToHandle(win_id),WIN_THREE_LEVEL);

        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        MMIDEFAULT_StopKeybadBackLightTimer();				
        MMIDEFAULT_StopAutoKeylockTimer();	
        MMIDEFAULT_StopScreenSaverTimer();

        if( 0 != etws_title_str.wstr_len)
        {
            GUIWIN_SetTitleText(
                win_id, 
                etws_title_str.wstr_ptr,
                etws_title_str.wstr_len,
                FALSE
                );
//            MMIAPICOM_StatusAreaSetViewHeaderinfo(etws_title_str.wstr_ptr,  etws_title_str.wstr_len);
        }        

        MMK_SetAtvCtrl(win_id, MMISMS_CB_ETWS_IDLE_RICHTEXT_CTRL_ID);   

        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if( TRUE == MMIAPISMSCB_GetIsNeedEnable())
        {
            if( MMK_IsOpenWin(MMIALM_ALIVEWIN_DURING_ETWS_ID) )
            {
                MMK_CloseWin(MMIALM_ALIVEWIN_DURING_ETWS_ID);
            }

            if( TRUE == MMIAPISMSCB_IsETWSAlert())
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
            }

            cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            if ( PNULL == cb_message_array)
            {
                SCI_TRACE_LOW("MMISMSCB: cb_message_array = PNULL!!");
                MMK_CloseWin(win_id);
                break;
            }            

            cur_idle_cb_msg = MMISMS_Get_CB_Message_Parent_Ptr();
            temp_msg_ptr = cur_idle_cb_msg->msg_ptr;

            index = MMIAPISMSCB_GetCurDispCBIndex();
            SCI_TRACE_LOW("MMISMS_ShowETWSPrompt, s_cur_display_cb_msg_index = %d, cur_idle_cb_msg->num = %d", index, cur_idle_cb_msg->num);
            
            if( index > 0 )
            {
                for( i = 0; i < index; i ++ )
                {
                    temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
                }
            }

            // set cb message as read
            temp_msg_ptr->is_read = TRUE;
            SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            SCI_MEMCPY(cb_message_array, temp_msg_ptr, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            MMIAPISMS_NV_WriteSmsCBItem(TRUE, index, cb_message_array);            

            if( PNULL != cb_message_array)
            {
                SCI_FREE(cb_message_array);
                cb_message_array = PNULL;            
            }

            is_display_older_cb_msg = FALSE;
            // display next unread cb message
            if( index < cur_idle_cb_msg->num - 1)
            {
                for( i = index+1; i < cur_idle_cb_msg->num; i ++ )
                {
                    temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
                    if( FALSE == temp_msg_ptr->is_read )
                    {
                        MMISMS_ShowETWSPrompt(i);
                        is_display_older_cb_msg = TRUE;
                        break;
                    }
                }
            }

            if( FALSE == is_display_older_cb_msg )
            {
                MMK_CloseWin(win_id);
            }
            
        }

        if( TRUE == MMIALM_GetAlarmRedisplayFlag())
        {                
            MMIALM_SetAlarmRedisplayFlag(FALSE);
            MMIALM_Redisplay();
        }
#ifdef COUNTEDTIME_SUPPORT
        if( TRUE == MMIAPICT_GetCTRedisplayFlag())
        {                
            MMIAPICT_SetCTRedisplayFlag(FALSE);
            MMIAPICT_Redisplay();
        }
#endif
        break;

    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_0:
#ifdef MMI_UNISOC_IDLE_EMERGENCY_CALL_SUPPORT        
        MMIAPIIdle_OpenEmergencyWin( msg_id) ;
#endif
         break;

    case MSG_APP_GREEN:
        if( MMICC_IsExistIncommingCall())
        {                
            MMICC_AnswerCall();
        }
        break;

    case MSG_KEYDOWN_RED:
        if( MMIAPICC_IsInState(CC_IN_CALL_STATE) )
        {
            MMIAPICC_ReleaseCallByRedkey();
        }
        break;      

    case MSG_CLOSE_WINDOW:
        if( TRUE == MMIAPISMSCB_GetIsNeedEnable())
        {
            MMIAPISMSCB_SetIsETWSDisplay(FALSE);
            
            if( TRUE == MMIAPISMSCB_IsETWSAlert())
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
            }
            MMK_CloseWin(win_id);
        }

        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        MMIDEFAULT_StartSettingLCDBackLightTimer();	
        MMIDEFAULT_StartAutoKeylockTimer();		
        MMIDEFAULT_StartScreenSaverTimer();
	break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//     Description : handle cb extract number item opt
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsCBExtractNumberOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    MMI_MENU_ID_T menu_id = 0;
    GUITEXT_TAG_INFO_T tag_info = {0};
    uint8 *number_ptr = PNULL;
    uint8 num_len = 0;
    MMI_STRING_T string_to                 = {0};
    MMI_STRING_T string_subject                 = {0};
    CC_RESULT_E cc_result = 0;
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};
    wchar *focus_num_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    
    if( PNULL == param )
    {
        SCI_TRACE_LOW("HandleSmsCBExtractNumberOptOk param pnull");
        return;
    }
    
    menu_id =param->menu_id;
    
    GUIRICHTEXT_GetFocusElement(MMISMS_CB_SHOW_MSG_TEXTBOX_CTRL_ID, &richtext_element);
    
    focus_num_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1)*sizeof(wchar));
    if (PNULL == focus_num_ptr)
    {
        SCI_TRACE_LOW("HandleSmsCBExtractNumberOptOk focus_num_ptr = PNULL");
        return;
    }
    //SCI_MEMSET(focus_num_ptr, 0, ((richtext_element.data.len + 1)*sizeof(wchar)));
    MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,richtext_element.data.len);
    number_ptr = SCI_ALLOC_APP(richtext_element.data.len + 1);
    SCI_MEMSET(number_ptr, 0, (richtext_element.data.len + 1));
    MMI_WSTRNTOSTR((uint8 *)number_ptr, richtext_element.data.len, focus_num_ptr, richtext_element.data.len, richtext_element.data.len);
    SCI_FREE(focus_num_ptr);
    focus_num_ptr = PNULL;
    
    switch (menu_id)
    {
    case MMISMS_MENUCB_DIAL_ITEM_ID:
        num_len = SCI_STRLEN((char *)number_ptr);
        {
            cc_result = MMIAPICC_MakeCall(
                (MN_DUAL_SYS_E)dual_sys,
                number_ptr,
                num_len,
                PNULL,
                PNULL,
                CC_CALL_SIM_MAX,
                CC_CALL_NORMAL_CALL,
                PNULL
                );
        }
        
        SCI_FREE(focus_num_ptr);
        SCI_FREE(number_ptr);
        focus_num_ptr = PNULL;
        number_ptr = PNULL;
        break;

    case MMISMS_MENUCB_ANSWER_SMS_ITEM_ID:
        MMIAPISMS_AnswerMessage((MN_DUAL_SYS_E)dual_sys, PNULL, number_ptr,
            (uint8)(strlen((char *)number_ptr) > MMIMMS_MAX_ADDRESS_LEN ? MMIMMS_MAX_ADDRESS_LEN : strlen((char *)number_ptr)));
        break;
        
    default :
        break;
    }    
    SCI_FREE(number_ptr);
}
#endif

/*****************************************************************************/
//     Description : set SMSCB channel name as window title
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL void SetChannelNameAsTitle(
                                 MMI_WIN_ID_T            win_id,         //IN: ���ڵ�ID
                                 MMISMS_CB_CHANNEL_T     *channel_list,    //IN: �ŵ��б�ṹ
                                 uint16                  msg_id            //IN: С���ŵ�ID
                                 )
{
    MMISMS_CB_CHANNEL_NAME_T msg_channel_name = {0};

    msg_channel_name = MMISMS_GetCBChannelNameByMsgId(
        channel_list,
        msg_id
        );
    
    if (0 == msg_channel_name.wstr_len)
    {
        uint8 channel_name[MMI_CB_CHANNEL_NAME_LEN] = {0};
        msg_channel_name.wstr_len =  sprintf(
            (char *)channel_name, 
            "%d", 
            msg_id
            );

        MMI_STRNTOWSTR(msg_channel_name.wstr,
                                    MMI_CB_CHANNEL_NAME_LEN,
                                    (uint8 *)channel_name,
                                    MMI_CB_CHANNEL_NAME_LEN,
                                    msg_channel_name.wstr_len);
    }
    //�����ڵı�����Ϊ�ŵ�����
    GUIWIN_SetTitleText(
        win_id, 
        msg_channel_name.wstr,
        msg_channel_name.wstr_len,
        FALSE
        );
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         HandleSmsCBClass0ReadWindow
//  ��������:        ��ʾCLASS0���㲥���ݴ��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleSmsCBClass0ReadWindow(
                                               MMI_WIN_ID_T            win_id, //IN: ���ڵ�ID
                                               MMI_MESSAGE_ID_E     msg_id, //IN: ���ڵ��ڲ���ϢID
                                               DPARAM                 param    //IN: ��Ӧ��Ϣ�Ĳ���
                                               )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)MMK_GetWinUserData(win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SOFTKEY_SAVE_MK, TXT_NULL, TXT_COMMON_COMMON_BACK, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK,2, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SAVE_MK,0, FALSE);
        MMK_SetAtvCtrl(win_id, MMISMS_CB_CLASS0_SHOW_MSG_TEXTBOX_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef MMI_SMSCB_SUPPORT
        if(MMIAPISMSCB_GetCBMsgType())
        {
            MMIAPISET_StopAllRing(FALSE);
            MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,TRUE,2000);
        }
#endif
#ifdef MMI_ETWS_SUPPORT
        if(MMIAPISMSCB_GetETWSAlertFlag())
        {
            MMIAPISET_StopRing(MMISET_RING_TYPE_ALARM);
            MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,TRUE,MMI_VIBRATE_INTERVAL);
	    MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_NONE);
        }
#endif
        {
            if(!cur_msg_ptr)
            {
                break;
            }

            cur_msg_ptr->is_read = TRUE;
            AddNewMsgNodeToCBMsgList(cur_msg_ptr);            
            MMK_SetWinUserData(MMK_ConvertIdToHandle(MMISMS_CB_CLASS0_READ_WIN_ID), PNULL);
            //����С���㲥�ռ����б�������Ҫˢ���б�
            if (MMK_IsOpenWin(MMISMS_CB_MSG_LIST_WIN_ID))
            {
                uint16 cur_index = 0;
                
                cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);;
                
                //ˢ�µ�ǰС���㲥�ռ����б�
                GUILIST_RemoveAllItems(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
                MMISMS_UpdateCBMsgList(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
                
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index + 1);
                
                if (MMK_IsFocusWin(MMISMS_CB_MSG_LIST_WIN_ID))
                {
                    MMK_SendMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
                }
            }
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:        
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
#ifdef MMI_SMSCB_SUPPORT
        if(MMIAPISMSCB_GetCBMsgType())
        {
            MMIAPISET_StopAllRing(FALSE);
            MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,TRUE,2000);
        }
#endif
#ifdef MMI_ETWS_SUPPORT
        if(MMIAPISMSCB_GetETWSAlertFlag())
        {
            MMIAPISET_StopRing(MMISET_RING_TYPE_ALARM);
            MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,TRUE,MMI_VIBRATE_INTERVAL);
            MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_NONE);
        }
#endif
        MMK_CloseWin(win_id);
        break;
		
    case MSG_CLOSE_WINDOW:
#ifdef MMI_ETWS_SUPPORT
        if(MMIAPISMSCB_GetETWSAlertFlag())
        {
            MMIAPISET_StopRing(MMISET_RING_TYPE_ALARM);
            MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,TRUE,MMI_VIBRATE_INTERVAL);
            MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_NONE);
        }
#endif
        if(cur_msg_ptr)
        {
            SCI_FREE(cur_msg_ptr);
        }
        break;
        
    default:       
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//     Description : MMISMSCB_OpenCBList
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_OpenCBList(void)
{
    MMK_CreateWin((uint32*)MMISMS_CB_MSG_LIST_WIN_TAB, PNULL);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         IsCBSwitchSelected
//  ��������:        ��ȡCB ON / OFF��ֵ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          
//  ����:            Yongli.tong
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN IsCBSwitchSelected(
                                MN_DUAL_SYS_E dual_sys
                                )
{
    BOOLEAN bEnabled = FALSE;
#ifdef MMI_ETWS_SUPPORT
    bEnabled = MMISMS_GetSmsCBEnabledFlag(dual_sys);
#else
    bEnabled = NV_LoadSmsCBEnabledFlag(dual_sys);
#endif
    
    return bEnabled;
}

/*****************************************************************************/
//     Description : MMISMSCB_SaveCBEmergenceChannel
//    Global resource dependence : 
//  Author: Tong Yongli
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_SaveCBEmergenceChannel(MN_DUAL_SYS_E dual_sys)
{
	
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MN_CB_ALERT_MSG_T *cb_msg_id_ptr = PNULL;
    uint16 total_num = 0;
    uint16 temp_num = 0;
    uint16 channel_index = 0;
    uint32 i = 0, j = 0;
	
    //cb_msg_id_ptr = (MN_CB_ALERT_MSG_T *)SCI_ALLOCAZ(sizeof(MN_CB_ALERT_MSG_T) );
    MMIAPISMSCB_InitCBAlertInfo(dual_sys);
	
    cb_msg_id_ptr = MMIAPISMSCB_GetCBAllAlertInfo(dual_sys);
    total_num = cb_msg_id_ptr->emegence_num + cb_msg_id_ptr->sim_cbmi_num;
    temp_num = total_num + cb_msg_id_ptr->soft_conf_num;
    SCI_TRACE_LOW("MMISMSCB_SaveCBEmergenceChannel : emegence_num = %d, sim_cbmi_num = %d, soft_conf_num = %d.", cb_msg_id_ptr->emegence_num, cb_msg_id_ptr->sim_cbmi_num, cb_msg_id_ptr->soft_conf_num);
	
    if(0 != total_num)
    {
        //emegence & cbmi   //mark cbmi
        if(cb_msg_id_ptr->emegence_num !=0)
		{
			for(i = 0; i < cb_msg_id_ptr->emegence_num; i++)
			{
				for(j = cb_msg_id_ptr->emegence_num; j < cb_msg_id_ptr->emegence_num + cb_msg_id_ptr->sim_cbmi_num; j++)
				{
					if(cb_msg_id_ptr->cb_alert_content[j].cb_alert_id == cb_msg_id_ptr->cb_alert_content[i].cb_alert_id)
					{
						cb_msg_id_ptr->cb_alert_content[j].type = -1;
						temp_num--;
					}
				}
			}
        }
        //cbmi & soft conf //mark cbmi
        if((cb_msg_id_ptr->sim_cbmi_num !=0) && (cb_msg_id_ptr->soft_conf_num !=0))
        {
            for(i = cb_msg_id_ptr->emegence_num; i < cb_msg_id_ptr->emegence_num + cb_msg_id_ptr->sim_cbmi_num; i++)
            {
                for(j = 0; j < cb_msg_id_ptr->soft_conf_num; j++)
                {
                    // modified by Tong Yongli. According to Coverity CID 26044
                    if((cb_msg_id_ptr->soft_conf_id[j] == cb_msg_id_ptr->cb_alert_content[i].cb_alert_id)
						&& (cb_msg_id_ptr->cb_alert_content[i].cb_alert_id != 0)
						&& ( cb_msg_id_ptr->cb_alert_content[i].type  > 0) /* Candice */
						)
                    {
                        cb_msg_id_ptr->cb_alert_content[i].type = -1;
                        temp_num--;
					}
				}
            }
        }
        //emegence & soft conf //mark soft conf
        if((cb_msg_id_ptr->emegence_num !=0) && (cb_msg_id_ptr->soft_conf_num !=0))
        {
            for(i = 0; i < cb_msg_id_ptr->emegence_num; i++)
            {
                for(j = 0; j < cb_msg_id_ptr->soft_conf_num; j++)
                {
                    if(cb_msg_id_ptr->soft_conf_id[j] == cb_msg_id_ptr->cb_alert_content[i].cb_alert_id)
                    {
                        cb_msg_id_ptr->soft_conf_id[j] = -1;
                        temp_num--;
					}
				}
            }
        }
		
        SCI_TRACE_LOW("MMISMSCB_SaveCBEmergenceChannel : temp_num = %d, total_num = %d.",temp_num, total_num);
        //MMISMS_CB_CHANNEL_ITEM_T ci_cbmi = {0};
        channel_index = 0;
        for(i = 0; i < total_num; i++)
        {
            channel_list.num = temp_num;
#ifdef MMI_ETWS_SUPPORT
            if(MMI_CB_ETWS_ID == cb_msg_id_ptr->cb_alert_content[i].type)
            {
                channel_list.channel_arr[channel_index].is_enabled = cb_msg_id_ptr->cb_alert_content[i].is_enabled;
                channel_list.channel_arr[channel_index].name.wstr_len = cb_msg_id_ptr->cb_alert_content[i].name.wstr_len;
                MMIAPICOM_Wstrncpy(channel_list.channel_arr[channel_index].name.wstr, cb_msg_id_ptr->cb_alert_content[i].name.wstr, channel_list.channel_arr[channel_index].name.wstr_len ) ;
            }
            else
#endif			
            if(MMI_CB_EMERGENCE_ID == cb_msg_id_ptr->cb_alert_content[i].type)
            {
                channel_list.channel_arr[channel_index].is_enabled = TRUE;
            }
            else if(MMI_CB_SIM_CBMI_ID == cb_msg_id_ptr->cb_alert_content[i].type)
            {
                channel_list.channel_arr[channel_index].is_enabled = FALSE;
            }
            else
            {
                continue;
            }
            channel_list.channel_arr[channel_index].msg_id = cb_msg_id_ptr->cb_alert_content[i].cb_alert_id;
            SCI_TRACE_LOW("MMISMSCB_SaveCBEmergenceChannel msg_id = %d.",channel_list.channel_arr[channel_index].msg_id);
#ifndef MMI_ETWS_SUPPORT
            NV_SaveSmsCBChannelList(&channel_list);
#endif
            channel_index++;
        }
        for(i = 0; i < cb_msg_id_ptr->soft_conf_num; i++)
        {
            // modified by Tong Yongli. according to Coverity CID 26045
            if(cb_msg_id_ptr->soft_conf_id[i] == 0)
                continue;
            channel_list.num = temp_num;
            channel_list.channel_arr[channel_index].is_enabled = TRUE;
            channel_list.channel_arr[channel_index].msg_id = cb_msg_id_ptr->soft_conf_id[i];
#ifndef MMI_ETWS_SUPPORT
            NV_SaveSmsCBChannelList(&channel_list);
#endif
            channel_index++;
        }

#ifdef MMI_ETWS_SUPPORT
        SCI_MEMSET(&s_save_to_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T));
        SCI_MEMCPY(&s_save_to_nv_channel_list,&channel_list, sizeof(MMISMS_CB_CHANNEL_T));
        MMISMSCB_SetSelectCard(dual_sys);
        NV_SaveSmsCBChannelList();
#endif
    }
	
}

/*****************************************************************************/
//     Description : MMISMSCB_SaveCBMIFromSim
//    Global resource dependence : 
//  Author: Tong Yongli
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_SaveCBMIFromSim(MN_DUAL_SYS_E dual_sys)
{
    MN_CB_ALERT_MSG_T *cb_sim_cbmi_ptr = PNULL;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    uint32 i = 0;
    cb_sim_cbmi_ptr = (MN_CB_ALERT_MSG_T *)SCI_ALLOCAZ(sizeof(MN_CB_ALERT_MSG_T) );

    if(PNULL == cb_sim_cbmi_ptr)
    {
        SCI_TRACE_LOW("MMISMSCB_SaveCBMIFromSim malloc error.");
        SCI_FREE(cb_sim_cbmi_ptr);
        return;
    }
    cb_sim_cbmi_ptr = MMIAPISMSCB_GetCBAllAlertInfo(dual_sys);

    for(i = cb_sim_cbmi_ptr->emegence_num; i < cb_sim_cbmi_ptr->sim_cbmi_num + cb_sim_cbmi_ptr->emegence_num; i++)
    {
        if(MMI_CB_SIM_CBMI_ID == cb_sim_cbmi_ptr->cb_alert_content[i].type)
        {
            channel_list.num = cb_sim_cbmi_ptr->sim_cbmi_num;
            channel_list.channel_arr[i].is_enabled = FALSE;
            channel_list.channel_arr[i].msg_id = cb_sim_cbmi_ptr->cb_alert_content[i + cb_sim_cbmi_ptr->emegence_num].cb_alert_id;
#ifdef MMI_ETWS_SUPPORT
            SCI_MEMSET(&s_save_to_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T));
            SCI_MEMCPY(&s_save_to_nv_channel_list,&channel_list, sizeof(MMISMS_CB_CHANNEL_T));
            NV_SaveSmsCBChannelList();
#else
            NV_SaveSmsCBChannelList(&channel_list);
#endif
        }
    }
}

/*****************************************************************************/
//     Description : handle SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBMainMenuWindow(
                                             MMI_WIN_ID_T        win_id,     // ���ڵ�ID
                                             MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                             DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_MAIN_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id= 0;
    MMI_MENU_ID_T menu_id = 0;
    
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
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMISMS_MENUCB_STARTUP_ITEM_ID:                            //�����㲥�˵��ѡ��
        case MMISMS_MENUCB_READ_ITEM_ID:                               //�Ķ��㲥�˵��ѡ��
        case MMISMS_MENUCB_LANGUAGE_ITEM_ID:                           //���ù㲥���Բ˵��ѡ��
        case MMISMS_MENUCB_CHANNEL_ADD_ITEM_ID:                        //��ӹ㲥�ŵ��˵��ѡ��
        case MMISMS_MENUCB_CHANNEL_LIST_ITEM_ID:                       //���ù㲥�ŵ��˵��ѡ��
            HandleMainMenuMsg(menu_id,ctrl_id);
            break;
            
        default:
            //SCI_TRACE_LOW:"SMSCB: HandleSmsCBMainMenuWindow menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_4530_112_2_18_2_55_37_907,(uint8*)"d", menu_id);
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
    
    return (recode);
}

/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMISMSCB_OpenMainMenu(void)
{
    MMK_CreateWin((uint32 *)MMISMS_CB_MAIN_MENU_WIN_TAB, PNULL);
}

/*****************************************************************************/
//     Description : ���/�༭�㲥�ŵ����ڴ�����
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSCBChannelIndexEditWindow(
                                                     MMI_WIN_ID_T        win_id, //IN: ���ڵ�ID
                                                     MMI_MESSAGE_ID_E     msg_id, //IN: ���ڵ��ڲ���ϢID
                                                     DPARAM                 param    //IN: ��Ӧ��Ϣ�Ĳ���
                                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_CHANNEL_INDEX_EDIT_CTRL_ID;
    MMI_STRING_T label_string = {0};
    MMI_STRING_T edit_string = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_GetString(MMISMS_CB_CHANNEL_SHOW_INDEX_LABEL_CTRL_ID, &label_string);
        GUIEDIT_SetFont(MMISMS_CB_CHANNEL_INDEX_EDIT_CTRL_ID,SONG_FONT_29);
        //shipeng.lv modify for cr150891
        if(PNULL != label_string.wstr_ptr && 0 != label_string.wstr_len)
        {
            GUIEDIT_SetString(ctrl_id, label_string.wstr_ptr, label_string.wstr_len);
        }
        //end shipeng
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint32 i = 0;
            BOOLEAN is_input_num = TRUE;

            GUIEDIT_GetString(ctrl_id, &edit_string);

            for (i = 0; i < edit_string.wstr_len; i++)
            {
                if (edit_string.wstr_ptr[i] < '0' || edit_string.wstr_ptr[i] > '9')
                {
                    is_input_num = FALSE;
                    break;
                }
            }

            if (!is_input_num)
            {
                //��ʾ�������
                MMIPUB_OpenAlertFailWin(TXT_INPUT_ERROR);  
            }
            else
            {
                label_string.wstr_len = edit_string.wstr_len;
                label_string.wstr_ptr = edit_string.wstr_ptr;
                GUIEDIT_SetString(MMISMS_CB_CHANNEL_SHOW_INDEX_LABEL_CTRL_ID, label_string.wstr_ptr, label_string.wstr_len);
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
    
    return (recode);
}

/*****************************************************************************/
//     Description : ���/�༭�㲥�ŵ����ڴ�����
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSCBChannelNameEditWindow(
                                                    MMI_WIN_ID_T        win_id, //IN: ���ڵ�ID
                                                    MMI_MESSAGE_ID_E     msg_id, //IN: ���ڵ��ڲ���ϢID
                                                    DPARAM                 param    //IN: ��Ӧ��Ϣ�Ĳ���
                                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_CHANNEL_NAME_EDIT_CTRL_ID;
    MMI_STRING_T label_string = {0};
    MMI_STRING_T edit_string = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_GetString(MMISMS_CB_CHANNEL_SHOW_NAME_LABEL_CTRL_ID, &label_string);
        //shipeng.lv modify for cr150891
        if(PNULL != label_string.wstr_ptr && 0 != label_string.wstr_len)
        {
            GUIEDIT_SetString(ctrl_id, label_string.wstr_ptr, label_string.wstr_len);
        }
        //shipeng end
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIEDIT_GetString(ctrl_id, &edit_string);
        
        label_string.wstr_len = edit_string.wstr_len;
        label_string.wstr_ptr = edit_string.wstr_ptr;
        GUIEDIT_SetString(MMISMS_CB_CHANNEL_SHOW_NAME_LABEL_CTRL_ID, label_string.wstr_ptr, label_string.wstr_len);
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
    
    return (recode);
}

/*****************************************************************************/
//  Description : to handle the smscb of delete query window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCBDelQueryWinMsg(
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
        //�ر�ѯ�ʴ���
        //MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        //��õ�ǰѡ�е��ŵ����б��е���������ɾ��ָ�����ŵ�
        index = GUILIST_GetCurItemIndex(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);

#ifdef MMI_ETWS_SUPPORT
        s_cur_channel_index = GetCurChannelIndex(index);
        DeleteCBChannel(s_cur_channel_index);
#else
        DeleteCBChannel(index);
#endif

        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //�ر�ѯ�ʴ���
        //MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : to handle all the smscb of delete query window    
//  Global resource dependence : 
//  Author:Tong Yongli
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCBAllDelQueryWinMsg(
		                                    MMI_WIN_ID_T        win_id,    //IN:
		                                    MMI_MESSAGE_ID_E    msg_id,    //IN:
											DPARAM                param    //IN:
											)
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    MMISMS_CB_CHANNEL_T cb_channel_info = {0};
    MMISMS_CB_LANGUAGE_T lang_card_list = {0};
    BOOLEAN cb_card_enabled = FALSE;
    MN_RETURN_RESULT_E result = MN_RETURN_FAILURE;
    uint16 i = 0;
#ifdef MMI_ETWS_SUPPORT
    uint16 etws_alert_num = 0;
    etws_alert_num = MMIAPISMSCB_GetETWSNum();
#endif    
	
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //�ر�ѯ�ʴ���
        //MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBChannelList();
        SCI_MEMCPY(&cb_channel_info, &s_nv_channel_list[dual_sys], sizeof(MMISMS_CB_CHANNEL_T));
#else
        cb_channel_info = MMISMS_NV_LoadSmsCBChannelList(dual_sys);
#endif

#ifdef MMI_ETWS_SUPPORT
	cb_card_enabled = MMISMS_GetSmsCBEnabledFlag(dual_sys);
#else
        cb_card_enabled = NV_LoadSmsCBEnabledFlag(dual_sys);
#endif
#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBLanguageList(dual_sys, &lang_card_list);
#else
        lang_card_list = NV_LoadSmsCBLanguageList(dual_sys);
#endif

        GUILIST_RemoveAllItems(MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID);
		
#ifdef MMI_ETWS_SUPPORT
        if( 0 == etws_alert_num  )
#endif	 
        {
            cb_channel_info.num = MMI_CB_ALERT_NUM;

            for(i = 0; i < MMI_CB_ALERT_NUM; i++)
            {
                cb_channel_info.channel_arr[i].msg_id = MMIAPISMSCB_GetCBAlertID(i);
                if(MMI_PER_INSTALL_PARAM == cb_channel_info.channel_arr[i].msg_id)
                {
                    cb_channel_info.channel_arr[i].is_enabled = NV_LoadSmsCB50ChannelFlag(dual_sys);
                }
                else
                {
                    cb_channel_info.channel_arr[i].is_enabled = TRUE;
                }
            }
        }
#ifdef MMI_ETWS_SUPPORT
        SCI_MEMSET(&s_save_to_nv_channel_list, 0x00, sizeof(MMISMS_CB_CHANNEL_T));
        SCI_MEMCPY(&s_save_to_nv_channel_list,&cb_channel_info, sizeof(MMISMS_CB_CHANNEL_T));
        NV_SaveSmsCBChannelList();
#else
        NV_SaveSmsCBChannelList(&cb_channel_info);
#endif
        if(cb_card_enabled)
        {
			MMISMSCB_Startup(&cb_channel_info, &lang_card_list, dual_sys);
            result = MMISMSCB_Reset(&cb_channel_info, &lang_card_list, dual_sys);    //�����趨С���㲥����(dual_sys);
        }
        else
        {
            result = MMISMSCB_Close();
        }
#ifdef CB_CHANNEL_50_EXIST
        NV_SaveSmsCB50ChannelFlag(FALSE);
#endif
        GUIWIN_SetSoftkeyTextId(MMISMS_CB_CHANNEL_LIST_WIN_ID, STXT_SOFTKEY_MENU_MK, TXT_NULL, TXT_COMMON_COMMON_BACK, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(MMISMS_CB_CHANNEL_LIST_WIN_ID, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(MMISMS_CB_CHANNEL_LIST_WIN_ID, STXT_SOFTKEY_MENU_MK, LEFT_BUTTON, TRUE);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //�ر�ѯ�ʴ���
        //MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
		
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
	
    return (recode);
	
}

/*****************************************************************************/
//  Discription: Handle no cb message alert win msg
//  Global resource dependence: none 
//  Author: Yongli Tong 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNoCBMessageAlertMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param 
                                               )
{
    MMI_RESULT_E        bResult       = MMI_RESULT_TRUE;
    switch(msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_KEYUP_CANCEL:
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_KEYUP_OK:
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYUP_WEB:
        MMK_CloseWin(win_id);
        break;

    default:
	SCI_TRACE_LOW("TYL unknown %x",msg_id);
        MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return bResult;
}


#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : handle SMSCB setting window      
//  Global resource dependence : 
//  Author: RONG.GU
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBSettingWindow(
                                             MMI_WIN_ID_T        win_id,     // ���ڵ�ID
                                             MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                             DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_SETTING_CTRL_ID;
    uint16 index = 0;
    static BOOLEAN flag = FALSE;
    static BOOLEAN flag_50_channel = FALSE;
    uint32 user_data = 0;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
#ifdef MMI_ETWS_SUPPORT
    MMI_STRING_T title = {0};
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMISMSCB_SetSelectCard(dual_sys);

#ifdef MMI_ETWS_SUPPORT
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

            if(TRUE==is_set_mcc)
            {
                plmn_info.mcc  = s_set_mcc;
            }

            if( CB_IT_MCC == plmn_info.mcc  )
            {
                GUIWIN_SetTitleTextId(win_id, text_cb_ita_Public_setting, FALSE);
            }
            else
            {
                GUIWIN_SetTitleTextId(win_id, setting_title_id[dual_sys] , FALSE);
            }
#else
            GUIWIN_SetTitleTextId(win_id, setting_title_id[dual_sys] , FALSE);
#endif
            InitPdaCbSmsSettingsListCtrl(ctrl_id, &flag, &flag_50_channel);
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;

    //case MMISMS_UPDATE_LIST:
    case MSG_FULL_PAINT:
	index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_RemoveAllItems(ctrl_id);
        InitPdaCbSmsSettingsListCtrl(ctrl_id,&flag,&flag_50_channel);
	GUILIST_SetCurItemIndex(ctrl_id, index);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
         index = GUILIST_GetCurItemIndex(ctrl_id);
         GUILIST_GetItemData(ctrl_id, index, &user_data);    
         switch((MMISMS_PDA_CB_SETTINGS_TYPE_E)user_data)
         {  
         case MMISMS_PDA_SMS_CB_SETTINGS_SWITCH:
             MMISMSCB_SwitchSmsCBStatus((BOOLEAN)!flag);
             break;
         case MMISMS_PDA_SMS_CB_SETTINGS_50_CHANNEL:
             MMISMSCB_SwitchCB50Channel((BOOLEAN)!flag_50_channel);
             break;

         case MMISMS_PDA_SMS_CB_SETTINGS_LANG:
             MMK_CreateWin((uint32*)MMISMS_CB_LANGUAGE_LIST_WIN_TAB, PNULL);
             //MMK_CreatePubListWin((uint32*)MMISMS_CB_LANGUAGE_LIST_WIN_TAB, PNULL);  
             break;
         
         case MMISMS_PDA_SMS_CB_SETTINGS_CHANNEL:
             if(MMIAPIPHONE_GetIsStartupSmscbFlag(dual_sys))
             {
                 MMK_CreateWin((uint32*)MMISMS_CB_CHANNEL_LIST_WIN_TAB, PNULL); 
             }
             else
             {
                 MMIPUB_OpenAlertWarningWin(TXT_SMS_CB_LIST_NO_NETWORK);  
             }      
             break;

         case MMISMS_PDA_SMS_CB_SETTINGS_READ_ITEM://added by Tong Yongli
             if (!MMISMS_IsCBMsgListEmpty())
            {
                MMK_CreateWin((uint32 *)MMISMS_CB_MSG_LIST_WIN_TAB, PNULL);
            }
            else  
            {
                // 2019.05.08 for bug 1065193 ,press red key can not return to idle
                //MMI_WIN_ID_T alert_win_id = PHONE_SMS_CB_ALERT_WIN_ID;
                MMIPUB_OpenAlertWarningWin(STR_CB_MESSAGE_EMPTY);  
                //MMIPUB_OpenAlertWinByTextId(PNULL,STR_CB_MESSAGE_EMPTY,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandleNoCBMessageAlertMsg);
                //MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_CB_MESSAGE_EMPTY, PNULL, IMAGE_NULL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_DIALOG_OK, HandleNoCBMessageAlertMsg);
            }
            break;

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
        case MMISMS_PDA_SMS_CB_SETTINGS_CELLINFO:
            MMISMSCB_SwitchCellInfoStatus(!s_cell_info_flag[dual_sys], dual_sys);
            MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, NULL);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
            break;
#endif

         default:
             break;
         }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
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
#else
/*****************************************************************************/
//  Description : handle SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBSettingWindow(
                                             MMI_WIN_ID_T        win_id,     // ���ڵ�ID
                                             MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                             DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_SETTING_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id= 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_STRING_T title = {0};
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {

            MMISMSCB_SetSelectCard(dual_sys);
            MMI_GetLabelTextByLang(setting_title_id[dual_sys] , &title);
            GUIMENU_SetMenuTitle(&title, ctrl_id);
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMISMS_MENUCB_STARTUP_ITEM_ID:                            //�����㲥�˵��ѡ��
        case MMISMS_MENUCB_LANGUAGE_ITEM_ID:                           //���ù㲥���Բ˵��ѡ��
        case MMISMS_MENUCB_CHANNEL_ITEM_ID:
            {
                const MMI_CTRL_ID_T sim_ctrl_id[] ={
                    MMISMS_CB_CHILD_SET_CARD1_CTRL_ID,
                    MMISMS_CB_CHILD_SET_CARD2_CTRL_ID,
                    MMISMS_CB_CHILD_SET_CARD3_CTRL_ID,
                    MMISMS_CB_CHILD_SET_CARD4_CTRL_ID
                };

                HandleMainMenuMsg(menu_id, sim_ctrl_id[dual_sys]);
                MMISMSCB_SetSelectCard(dual_sys);
            }
            break;
        default:
            //SCI_TRACE_LOW:"SMSCB: HandleSmsCBMainMenuWindow menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_4888_112_2_18_2_55_38_908,(uint8*)"d", menu_id);
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
    
    return (recode);
}
#endif

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : handle SMSCB No SIM setting window      
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBNoSimSettingWindow(
                                             MMI_WIN_ID_T        win_id,     // ���ڵ�ID
                                             MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                             DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CB_SETTING_CTRL_ID;
    uint16 index = 0;
    static BOOLEAN flag = FALSE;
    static BOOLEAN flag_50_channel = FALSE;
    uint32 user_data = 0;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
//            MMISMSCB_SetSelectCard(dual_sys);
//             GUIWIN_SetTitleTextId(win_id, setting_title_id[dual_sys] , FALSE);
            InitPdaCbSmsSettingsListCtrl(ctrl_id, &flag, &flag_50_channel);
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;

    //case MMISMS_UPDATE_LIST:
    case MSG_FULL_PAINT:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_RemoveAllItems(ctrl_id);
        InitPdaCbSmsSettingsListCtrl(ctrl_id,&flag,&flag_50_channel);
        GUILIST_SetCurItemIndex(ctrl_id, index);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
         index = GUILIST_GetCurItemIndex(ctrl_id);
         GUILIST_GetItemData(ctrl_id, index, &user_data);    
         switch((MMISMS_PDA_CB_SETTINGS_TYPE_E)user_data)
         {  
         case MMISMS_PDA_SMS_CB_SETTINGS_SWITCH:
             MMISMSCB_SwitchSmsCBStatus((BOOLEAN)!flag);
             break;
         case MMISMS_PDA_SMS_CB_SETTINGS_50_CHANNEL:
             MMISMSCB_SwitchCB50Channel((BOOLEAN)!flag_50_channel);
             break;

         case MMISMS_PDA_SMS_CB_SETTINGS_LANG:
             MMK_CreateWin((uint32*)MMISMS_CB_LANGUAGE_LIST_WIN_TAB, PNULL);
             //MMK_CreatePubListWin((uint32*)MMISMS_CB_LANGUAGE_LIST_WIN_TAB, PNULL);  
             break;
         
         case MMISMS_PDA_SMS_CB_SETTINGS_CHANNEL:
             if(MMIAPIPHONE_GetIsStartupSmscbFlag(dual_sys))
             {
                 MMK_CreateWin((uint32*)MMISMS_CB_CHANNEL_LIST_WIN_TAB, PNULL); 
             }
             else
             {
                 MMIPUB_OpenAlertWarningWin(TXT_SMS_CB_LIST_NO_NETWORK);  
             }      
             break;

         case MMISMS_PDA_SMS_CB_SETTINGS_READ_ITEM://added by Tong Yongli
             if (!MMISMS_IsCBMsgListEmpty())
            {
                MMK_CreateWin((uint32 *)MMISMS_CB_MSG_LIST_WIN_TAB, PNULL);
            }
            else  
            {
                MMI_WIN_ID_T alert_win_id = PHONE_SMS_CB_ALERT_WIN_ID;
                //MMIPUB_OpenAlertWarningWin(STR_CB_MESSAGE_EMPTY);  
                //MMIPUB_OpenAlertWinByTextId(PNULL,STR_CB_MESSAGE_EMPTY,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandleNoCBMessageAlertMsg);
                MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_CB_MESSAGE_EMPTY, PNULL, IMAGE_NULL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_DIALOG_OK, HandleNoCBMessageAlertMsg);
            }
            break;

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
        case MMISMS_PDA_SMS_CB_SETTINGS_CELLINFO:
            MMISMSCB_SwitchCellInfoStatus(!s_cell_info_flag[dual_sys], dual_sys);
            MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, NULL);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
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
    
    return (recode);
}
#endif


#ifndef MMI_GUI_STYLE_TYPICAL
LOCAL void AppendPdaCBSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //�ؼ�ID
                                 MMI_TEXT_ID_T  text_1,     //���ı�
                                 uint32         user_data   //�û�����
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_AND_TEXT_MS;//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    GUILIST_AppendItem(ctrl_id, &item_t);
}


/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaCBSettingsItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //�ؼ�ID
                                 MMI_TEXT_ID_T  text_1,     //���ı�
                                 MMI_STRING_T  text_str,     //���ı�
                                 MMI_IMAGE_ID_T image_id,   //ͼ��ID
                                 uint32         user_data,   //�û�����
                                 BOOLEAN is_need_replace
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar temp_wstr[MMISMS_STRNUM_MAX_LEN + 2] = {0};
    wchar temp_len = 0;
    uint16 index = 0;
    index = GUILIST_GetCurItemIndex(ctrl_id); 
    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        MMIAPICOM_Wstrncpy(temp_wstr,
                                          text_str.wstr_ptr,
                                          text_str.wstr_len);
        temp_len = text_str.wstr_len;
    }
    
    item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    /*//text 2    
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = temp_len;
    }*/
    
    //anim
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = image_id;

    if(is_need_replace)
    {
        GUILIST_ReplaceItem(ctrl_id,&item_t,index);      
    }
    else
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }  
}

/*****************************************************************************/
//  Description :init settings list
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/                                 
LOCAL void  InitPdaCbSmsSettingsListCtrl(MMI_CTRL_ID_T ctrl_id,BOOLEAN *flag,BOOLEAN * flag_50_channel)
{
    //GUILIST_ITEM_T      item_t    = {0};   /*lint !e64*/
    //GUILIST_ITEM_DATA_T item_data = {0};   /*lint !e64*/
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    MMI_IMAGE_ID_T      image_50_id = IMAGE_NULL;
    MMI_STRING_T text_str= {0};
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
    MMI_IMAGE_ID_T cell_info_img_id = IMAGE_NULL;
#endif
#ifdef MMI_ETWS_SUPPORT
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    CMAS_CHANNEL_INFO_T* current_channel_table = PNULL;
#endif

    BOOLEAN cb_enabled  = FALSE;
    BOOLEAN cb_50_enable = FALSE;

#ifdef MMI_ETWS_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 i = 0;
    uint32 sim_ok_array[MMI_DUAL_SYS_MAX] = {0,};
    uint32 sim_ok_num = 0;
    
    SCI_MEMSET(sim_ok_array, 0x00, sizeof(sim_ok_array));
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(MMIAPIPHONE_IsSimOk((MN_DUAL_SYS_E)i))
        {
            sim_ok_array[sim_ok_num] = (MN_DUAL_SYS_E)i;
            sim_ok_num ++;
        }
    }
    
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = (MN_DUAL_SYS_E)(sim_ok_array[0]);   //else use the ok sim. or if no ok, user the sim1.(sim_ok have inited with 0. )
    }
#else
    dual_sys = MN_DUAL_SYS_1;
#endif
#endif

    //set max item
    GUILIST_SetMaxItem(ctrl_id, MMISMS_PDA_SMS_CB_SETTINGS_MAX, FALSE );   
#ifdef MMI_ETWS_SUPPORT
    cb_enabled = MMISMS_GetSmsCBEnabledFlag(MMISMSCB_GetSelectCard());
#else
    cb_enabled = NV_LoadSmsCBEnabledFlag(MMISMSCB_GetSelectCard());
#endif
    cb_50_enable = NV_LoadSmsCB50ChannelFlag(MMISMSCB_GetSelectCard());
    *flag =  cb_enabled;
    *flag_50_channel = cb_50_enable;
    if(is_system_factory)
    {
        is_system_factory = FALSE;
#ifdef CB_CHANNEL_50_EXIST
	MMISMSCB_SwitchCB50Channel(FALSE);
	MMISMSCB_SwitchCB50Channel(TRUE);
#endif
    }

#ifdef MMI_ETWS_SUPPORT
    current_channel_table = GetCMASChannelTable(dual_sys);
#endif    

    if(cb_enabled)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }    

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
    dual_sys = MMISMSCB_GetSelectCard();
    if(s_cell_info_flag[dual_sys])
    {
        cell_info_img_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        cell_info_img_id = IMAGE_CHECK_UNSELECTED_ICON;
    }
#endif

    if(cb_50_enable)
    {
        image_50_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_50_id = IMAGE_CHECK_UNSELECTED_ICON;
    }


    SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
    
    if(TRUE==is_set_mcc)
    {
        plmn_info.mcc  = s_set_mcc;
    }

    if(CB_UAE_MCC != plmn_info.mcc)
    {
        AppendPdaCBSettingsItem2Line(            
        ctrl_id,
        STR_CB_ILIST_CELL_BROADCAST,
        text_str,
        image_id,
        (uint32)MMISMS_PDA_SMS_CB_SETTINGS_SWITCH,
        FALSE);
        GUILIST_SetToggleState(ctrl_id,0,cb_enabled);

#ifdef MMI_ETWS_SUPPORT
        // default turn on CB and cannot disable
        if( PNULL != current_channel_table )
        {
            CTRLLIST_SetItemInvalid(ctrl_id,0,TRUE);
            CTRLLIST_SetItemGray(ctrl_id,0,TRUE);
        }
        else
        {
            CTRLLIST_SetItemInvalid(ctrl_id,0,FALSE);
            CTRLLIST_SetItemGray(ctrl_id,0,FALSE);
        }
#endif
    }
    
    AppendPdaCBSettingsItem2Line(
    ctrl_id,
    STR_CB_CELL_INFO,
    text_str,
    image_50_id,
    (uint32)MMISMS_PDA_SMS_CB_SETTINGS_50_CHANNEL,
    FALSE);

    if(CB_UAE_MCC != plmn_info.mcc)
        GUILIST_SetToggleState(ctrl_id,1,cb_50_enable);
    else
        GUILIST_SetToggleState(ctrl_id,0,cb_50_enable);

    AppendPdaCBSettingsItem1Line(            
    ctrl_id,
    STR_CB_LANGUAGE,
    //text_str,
    //IMAGE_COMMON_RIGHT_ARROW,
    (uint32)MMISMS_PDA_SMS_CB_SETTINGS_LANG);

    AppendPdaCBSettingsItem1Line(            
    ctrl_id,
    STR_CB_CHANNEL,
    (uint32)MMISMS_PDA_SMS_CB_SETTINGS_CHANNEL);

    AppendPdaCBSettingsItem1Line(
    ctrl_id,
#ifdef MMI_ETWS_SUPPORT
    STR_SMSTX_LIST_INBOX,
#else    
    STR_CB_MESSAGE,
#endif    
    (uint32)MMISMS_PDA_SMS_CB_SETTINGS_READ_ITEM);//added by Tong Yongli
    
#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
    AppendPdaCBSettingsItem2Line(            
    ctrl_id,
    TXT_CELL_INFO,
    text_str,
    cell_info_img_id,
    (uint32)MMISMS_PDA_SMS_CB_SETTINGS_CELLINFO,
    FALSE); //Candice
#endif

}
#if 0
/*****************************************************************************/
//  Description : handle SMSCB setting window      
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBSettingCardWindow(
                                             MMI_WIN_ID_T        win_id,     // ���ڵ�ID
                                             MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                             DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = (MMI_CTRL_ID_T)MMK_GetWinAddDataPtr(win_id);
    //MMI_MENU_GROUP_ID_T group_id= 0;
    //MMI_MENU_ID_T menu_id = 0;
    uint16 index = 0;
    uint32 user_data = 0;
    static BOOLEAN flag = FALSE;
    BOOLEAN flag_temp = FALSE;
    SetSelectCard(ctrl_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitPdaCbSmsSettingsListCtrl(ctrl_id, &flag);
        MMK_SetAtvCtrl(win_id, ctrl_id);        
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
        GUILIST_RemoveAllItems(ctrl_id);
        InitPdaCbSmsSettingsListCtrl(ctrl_id,&flag);
        break;
    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    #ifndef MMI_PDA_SUPPORT
    case MSG_CTL_OK:
    case MSG_APP_OK:
    #endif
    case MSG_APP_WEB:        
         index = GUILIST_GetCurItemIndex(ctrl_id);
         GUILIST_GetItemData(ctrl_id, index, &user_data);                      
         switch((MMISMS_PDA_CB_SETTINGS_TYPE_E)user_data)
         {  
         case MMISMS_PDA_SMS_CB_SETTINGS_SWITCH:
             flag_temp = !flag;
             MMISMSCB_SwitchSmsCBStatus(flag_temp);
             MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, NULL);
             MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
             break;

         case MMISMS_PDA_SMS_CB_SETTINGS_LANG:
             MMK_CreatePubListWin((uint32*)MMISMS_CB_LANGUAGE_LIST_WIN_TAB, PNULL);  
             break;
         
         case MMISMS_PDA_SMS_CB_SETTINGS_CHANNEL:
             MMK_CreateWin((uint32*)MMISMS_CB_CHANNEL_LIST_WIN_TAB, PNULL);         
             break;

         default:
             break;
         }
    break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //MMK_CloseWin(win_id);
        MMK_CloseParentWin(MMISMS_CB_SETTING_WIN_ID);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}
#endif
#else
#if 0
/*****************************************************************************/
//  Description : handle SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsCBSettingCardWindow(
                                             MMI_WIN_ID_T        win_id,     // ���ڵ�ID
                                             MMI_MESSAGE_ID_E     msg_id,     // ���ڵ��ڲ���ϢID
                                             DPARAM             param        // ��Ӧ��Ϣ�Ĳ���
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = (MMI_CTRL_ID_T)MMK_GetWinAddDataPtr(win_id);
    MMI_MENU_GROUP_ID_T group_id= 0;
    MMI_MENU_ID_T menu_id = 0;
    
    SetSelectCard(ctrl_id);
    
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
    #ifndef MMI_PDA_SUPPORT
    case MSG_CTL_OK:
    case MSG_APP_OK:
    #endif
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMISMS_MENUCB_STARTUP_ITEM_ID:                            //�����㲥�˵��ѡ��
        case MMISMS_MENUCB_LANGUAGE_ITEM_ID:                           //���ù㲥���Բ˵��ѡ��
        case MMISMS_MENUCB_CHANNEL_ITEM_ID:
            HandleMainMenuMsg(menu_id,ctrl_id);
            break;
            
        default:
            //SCI_TRACE_LOW:"SMSCB: HandleSmsCBMainMenuWindow menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_5214_112_2_18_2_55_39_909,(uint8*)"d", menu_id);
            break;
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //MMK_CloseWin(win_id);
        MMK_CloseParentWin(MMISMS_CB_SETTING_WIN_ID);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}
#endif
#endif
/*****************************************************************************/
//     Description : to handle window message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  SMSCBSettingSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    if (isReturnOK)
    {
        MMK_CreateWin((uint32 *)MMISMS_CB_SETTING_WIN_TAB, (ADD_DATA)dual_sys);
        
    }    
    return recode;
}

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_GetCellInfoFlag
//  ��������:        Get the cell info flag
//  ȫ�ֱ���ʹ��:    s_cell_info_flag
//  ����ֵ:          ��
//  ����:            Yongli Tong
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMISMSCB_GetCellInfoFlag(MN_DUAL_SYS_E dual_sys)
{
    return s_cell_info_flag[dual_sys];
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         UpdateCBMsgList
//  ��������:        ˢ��С���㲥�ռ����б�
//  ȫ�ֱ���ʹ��:    s_cb_msg
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMS_UpdateCBMsgList(
                           MMI_CTRL_ID_T    ctrl_id        //IN:
                           )
{
    MMISMS_CB_CHANNEL_T channel_list[MMI_DUAL_SYS_MAX] = {0};
    int32 i = 0;
    MMISMS_CB_CHANNEL_T select_channel_list = {0};
    MMISMS_CB_MESSAGE_ITEM_T * cur_msg_ptr = PNULL;
    uint16 cur_index = 0;
    uint16 num = 0;
#ifdef MMI_ETWS_SUPPORT
//	MMISMS_CB_MESSAGE_ARRAY_T cb_message_array = {0};
    MMISMS_CB_MESSAGE_ARRAY_T* cb_message_array = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *last_msg_ptr = PNULL;
    uint8 cb_num = 0;
    uint8 index = 0;
    MMISMS_CB_MESSAGE_T* restore_cb_msg = PNULL;
#endif

    for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)   
    {
#ifdef MMI_ETWS_SUPPORT
        NV_LoadSmsCBChannelList();
        SCI_MEMCPY(&channel_list[i], &s_nv_channel_list[i], sizeof(MMISMS_CB_CHANNEL_T));
#else
        channel_list[i] = MMISMS_NV_LoadSmsCBChannelList((MN_DUAL_SYS_E)i);
#endif
    }
    cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
    GUILIST_SetMaxItem( ctrl_id, MMI_MAX_CB_MSG_NUM, FALSE );//max item 10    

    //���ù㲥��Ϣ�ռ����б��ÿһ��
    cur_msg_ptr = MMISMS_Get_CB_Message_Ptr();
    num = MMISMS_Get_CB_Message_Num();

#ifdef MMI_ETWS_SUPPORT
    if( (PNULL == cur_msg_ptr) && (0 == num))
    {
        cb_num = MMISMS_NV_LoadSmsCBMessageListNum();
        if( cb_num > 0)
        {
            restore_cb_msg = MMISMS_Get_CB_Message_Parent_Ptr();

            cur_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
            SCI_MEMSET(cur_msg_ptr , 0, sizeof(MMISMS_CB_MESSAGE_ITEM_T));	

            restore_cb_msg->msg_ptr = cur_msg_ptr;
            temp_msg_ptr = cur_msg_ptr;

            cb_message_array = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));	

            if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
            {
                SCI_MEMCPY(cur_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            }
            else
            {
                SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
            }
            for( index = 1; index < cb_num; index ++)
            {
                SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));	
                if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
                {
                    last_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
                    SCI_MEMCPY(last_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
                    temp_msg_ptr->msg_next_ptr = last_msg_ptr;
                    temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
                }
                else
                {
                    SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
                }
            }

            MMISMS_Set_CB_Message_Num(index);
            num = index;
        }
    }
#endif	

    while (PNULL != cur_msg_ptr)
    {
        select_channel_list = channel_list[cur_msg_ptr->dual_sys];
		
        AppendCBMsgListItem(
            ctrl_id,
            &select_channel_list,
            cur_msg_ptr->is_read, 
            cur_msg_ptr
            );
        cur_msg_ptr = cur_msg_ptr->msg_next_ptr;
    }
	
    GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, MIN(cur_index, num - 1));
	
    if (0 == num)
    {
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIWIN_SetSoftkeyTextId(MMISMS_CB_MSG_LIST_WIN_ID, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(MMISMS_CB_MSG_LIST_WIN_ID, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#endif
    }
}


/*****************************************************************************/
//  Description : open SMSCB CLASS0 window      
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_OpenCLASS0Win(void)
{
    MMK_CreateWin((uint32*)MMISMS_CB_CLASS0_READ_WIN_TAB ,PNULL);
}

/*****************************************************************************/
//  Description : open SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
void MMISMSCB_OpenSettingWin(void)
{
    uint16 sim_num = 0;
    uint16 sim_ok = 0;
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);

#ifdef MMI_ETWS_SUPPORT
    if (0 == sim_num )
    {
        MMK_CreateWin((uint32 *)MMISMS_CB_SETTING_NO_SIM_WIN_TAB, (ADD_DATA)sim_ok);
    }
#else
    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
#endif    
    else if (sim_num==1)
    {
        MMK_CreateWin((uint32 *)MMISMS_CB_SETTING_WIN_TAB, (ADD_DATA)sim_ok);
    }
    else
    {
        MMISMS_SelectSIMFunc(SMSCBSettingSelectSimCallback,PNULL,TRUE);
    }
#if 0    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else
    {
        int32 i = 0;

        const uint32 *child_win_tab[] ={
            MMISMS_CB_CHILD_SET_CARD1_WIN_TAB,
            MMISMS_CB_CHILD_SET_CARD2_WIN_TAB,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_CB_CHILD_SET_CARD3_WIN_TAB,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_CB_CHILD_SET_CARD4_WIN_TAB
#endif
    	};   

        const MMI_CTRL_ID_T ctrl_id[] ={
            MMISMS_CB_CHILD_SET_CARD1_CTRL_ID,
            MMISMS_CB_CHILD_SET_CARD2_CTRL_ID,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_CB_CHILD_SET_CARD3_CTRL_ID,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_CB_CHILD_SET_CARD4_CTRL_ID
#endif
    	};

        const MMI_WIN_ID_T child_win_id[] ={
            MMISMS_CB_CHILD_SET_CARD1_WIN_ID,
            MMISMS_CB_CHILD_SET_CARD2_WIN_ID,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_CB_CHILD_SET_CARD3_WIN_ID,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_CB_CHILD_SET_CARD4_WIN_ID
#endif
    	};      

        MMK_CreateParentWin((uint32 *)MMISMS_MULTI_CB_SETTING_WIN_TAB, PNULL);

        GUITAB_SetMaxItemNum(MMISMS_CB_SETTING_TAB_CTRL_ID, sim_num);
        //GUITAB_SetItemDefaultWidthByImage(MMISMS_CB_SETTING_TAB_CTRL_ID, IMAGE_TAB_SELECT2);

        //create child window
        for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
        {
            if(MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)i))
            {
                MMK_CreateChildWin(MMISMS_CB_SETTING_WIN_ID, (uint32 *)child_win_tab[i], (ADD_DATA)ctrl_id[i]);
            }
        }

        //set focus window
        for(i = MN_DUAL_SYS_1;i<MMI_DUAL_SYS_MAX;i++)
        {
            if(MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)i))
            {
                MMK_SetChildWinFocus(MMISMS_CB_SETTING_WIN_ID, child_win_id[i]);
                break;
            }
        }
    }
#endif
#endif
}

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : sms cb opt
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSmsCBOptWindow( MMIPUB_MENU_NOTIFY_T *param )
{
//    MMI_RESULT_E recode = MMI_RESULT_TRUE;
//    MMI_CTRL_ID_T ctrl_id = param->ctrl_id;
//    MMI_MENU_GROUP_ID_T  group_id = param->group_id;
    MMI_MENU_ID_T        menu_id = param->menu_id;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    //GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
    switch(menu_id)
    {
        case MMISMS_MENU_BROADCAST_SETTING_ITEM_ID:
        MMISMSCB_OpenSettingWin();
        MMK_CloseWin(param->win_handle);
        break;
        
        default:
        break;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

//    return (recode);
}

#endif

/*****************************************************************************/
//	Description : the handle function of Delete Smscb
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteSmscbQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32	index = 0;

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
        index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

        MMISMS_DeleteMsgFromCBMsgList((uint16)index);
        
        //if(0 < s_cb_msg.num)
        {
            MMK_PostMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_SMSCB_LIST_UPDATE, NULL, 0);
        }
        
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return result;
}

/*****************************************************************************/
//	Description : the handle function of All Delete Smscb
//	Global resource dependence : 
//	Author: Tong Yongli
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllDeleteSmscbQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32	total_num = 0;
    uint16 i = 0;

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
        total_num = CTRLLIST_GetTotalItemNum(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
        for(i = 0; i < total_num; i++)
        {
            MMISMS_DeleteMsgFromCBMsgList((uint16)i);
        }
        
        //if(0 < s_cb_msg.num)
        {
            MMK_PostMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_SMSCB_LIST_UPDATE, NULL, 0);
        }
        
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return result;
}


/*****************************************************************************/
//  Description : sms cb list opt init
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSmsCBListOptWindowInit( MMIPUB_MENU_NOTIFY_T *param )
{
//    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    //MMI_CTRL_ID_T ctrl_id = MMISMS_CB_LISTOPT_CTRL_ID;
    //MMI_MENU_GROUP_ID_T group_id = {0};
    //MMI_MENU_ID_T menu_id = {0};
    //uint16 index = 0;
    //MMI_STRING_T sms_msg = {0};
    MMI_STRING_T      title_text_ptr = {0};
    GUIMENU_BUTTON_INFO_T button_info = {0};
    MMI_MENU_GROUP_ID_T group_id = (MMI_MENU_GROUP_ID_T)MMK_GetWinUserData(param->win_id);
    const GUILIST_ITEM_T *listitem = GUILIST_GetItemPtrByIndex( MMISMS_CB_MSG_LISTBOX_CTRL_ID,GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID));
#ifdef MMI_ETWS_SUPPORT
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
#endif

    if(listitem && listitem->item_data_ptr)
    {
        title_text_ptr  = listitem->item_data_ptr->item_content[2].item_data.text_buffer;
    }
  
    GUIMENU_SetMenuTitle(&title_text_ptr, param->ctrl_id);

#ifndef MMI_PDA_SUPPORT
    if(0 == GUILIST_GetTotalItemNum(MMISMS_CB_MSG_LISTBOX_CTRL_ID))
    {
        GUIMENU_SetItemGrayed(param->ctrl_id, MENU_SMS_BROADCAST_LIST_OPT,MMISMS_MENU_BROADCAST_FOWARD_ITEM_ID,TRUE);
        GUIMENU_SetItemGrayed(param->ctrl_id, MENU_SMS_BROADCAST_LIST_OPT,MMISMS_MENU_BROADCAST_DELETE_ITEM_ID,TRUE);    
        GUIMENU_SetItemGrayed(param->ctrl_id, MENU_SMS_BROADCAST_LIST_OPT,MMISMS_MENU_BROADCAST_ALL_DEL_ITEM_ID,TRUE);
    }
#endif        

#ifdef MMI_ETWS_SUPPORT
    SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

    if(TRUE==is_set_mcc)
    {
        plmn_info.mcc  = s_set_mcc;
    }

    if(( CB_PWS_MCC == plmn_info.mcc ) || (CB_UAE_MCC == plmn_info.mcc) || (CB_RO_MCC == plmn_info.mcc))
    {
        CTRLMENU_SetItemVisible(param->ctrl_id, MENU_SMS_BROADCAST_LIST_OPT,MMISMS_MENU_BROADCAST_FOWARD_ITEM_ID,FALSE);
    }
#endif


    button_info.is_static = TRUE;
    button_info.group_id = group_id;
    button_info.button_style = GUIMENU_BUTTON_STYLE_OK_CANCEL;
    GUIMENU_SetButtonStyle(param->ctrl_id, &button_info);
    MMK_SetAtvCtrl(param->win_id, param->ctrl_id);
    //GUIWIN_SetSoftkeyTextId(param->win_id,  PNULL, STXT_SOFTKEY_SELECT_MK, (MMI_IMAGE_ID_T)IMAGE_COMMON_BACK, FALSE);

    return;
}

/*****************************************************************************/
//  Description : sms cb list opt ok option
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSmsCBListOptWindowOk( MMIPUB_MENU_NOTIFY_T *param )
{
    //MMI_RESULT_E recode = MMI_RESULT_TRUE;
//    MMI_CTRL_ID_T ctrl_id = param->ctrl_id;
  //  MMI_MENU_GROUP_ID_T group_id = param->group_id;
    MMI_MENU_ID_T menu_id =param->menu_id;
    uint16 index = 0;
    MMI_STRING_T sms_msg = {0};
    MMI_STRING_T prompt_str = {0};
    MMI_WIN_ID_T query_win_id = MMIPUB_ALERT_WIN_ID;

    switch(menu_id)
    {
    case MMISMS_MENU_BROADCAST_FOWARD_ITEM_ID:
        index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

        if (0 == MMISMS_Get_CB_Message_Num())
        {
            break;
        }
        else
        {
            MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
            uint16 pos = 0;

            cur_msg_ptr = MMISMS_Get_CB_Message_Ptr();

            for (pos = 0; pos < index; pos++)
            {
                cur_msg_ptr = cur_msg_ptr->msg_next_ptr;
            }

            if (PNULL == cur_msg_ptr)
            {
                break;
            }

            sms_msg.wstr_len = cur_msg_ptr->len;
            sms_msg.wstr_ptr = cur_msg_ptr->context;
            MMISMS_WriteNewMessage(
                                    cur_msg_ptr->dual_sys,
                                    &sms_msg,
                                    FALSE,
                                    PNULL,
                                    0
                                    );
        }
        break;
    
    case MMISMS_MENU_BROADCAST_DELETE_ITEM_ID:
        MMI_GetLabelTextByLang(STXT_SMS_CLEAR_MESSAGE, &prompt_str);
        MMIPUB_OpenConfirmationDialogByTextPtr(  PNULL,&prompt_str,PNULL, IMAGE_NULL,          
                                         &query_win_id, PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,  HandleDeleteSmscbQueryWinMsg);
        //MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,HandleDeleteSmscbQueryWinMsg);
        break;
    case MMISMS_MENU_BROADCAST_ALL_DEL_ITEM_ID:
        MMIPUB_OpenQueryWinByTextId(TXT_ALL_DELETE,IMAGE_PUBWIN_QUERY,PNULL,HandleAllDeleteSmscbQueryWinMsg);
        break;
    
#ifndef MMI_PDA_SUPPORT
    case MMISMS_MENU_BROADCAST_SETTING_ITEM_ID:
        MMISMSCB_OpenSettingWin();
        break;
#endif   

    default:
        break;
    }

    MMK_CloseWin(param->win_id);
    
    return ;
}
#endif

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : MMIAPISMSCB_IsSetMcc
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_IsSetMcc(void)
{
    SCI_TRACE_LOW("MMIAPISMSCB_IsSetMcc, is_set_mcc = %d", is_set_mcc);
    return is_set_mcc;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetSetMcc
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetSetMcc(void)
{
    SCI_TRACE_LOW("MMIAPISMSCB_GetSetMcc, s_set_mcc = %d", s_set_mcc);
    return s_set_mcc;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_SetIsSpecChange
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetIsSpecChange(BOOLEAN is_change)
{
     s_is_etws_spec_changed = TRUE;
}

/*****************************************************************************/
//     Description : MMISMSCB_SetAllLangList
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
LOCAL void MMISMSCB_SetAllLangList(MMISMS_CB_LANGUAGE_T* lang_list )
{
    uint32 i = 0;
    MMISMS_CB_LANGUAGE_T all_lang_list = {0};
    
    if( PNULL == lang_list )
    {
        return;
    }

    all_lang_list.selected_arr[0] = TRUE;
    
    SCI_MEMCPY(lang_list, &all_lang_list, sizeof(MMISMS_CB_LANGUAGE_T));

  }

/*****************************************************************************/
//     Description : MMISMSCB_ConvertLangIDToLangIndex
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC uint32 MMISMSCB_ConvertLangIDToLangIndex(MN_SMSCB_LANGUAGE_ID_E lang_id)
{
    uint32 i = 0;
    uint32 index = 0;
    
    for( i = 1 ; i <  MMI_MAX_CB_LANGUAGE_NUM ; i++)
    {
        if( g_cb_lang_id_arr[i] == lang_id )
        {
            index =  i ;
			break;
        }
    }
    if( index >= MMI_MAX_CB_LANGUAGE_NUM )
    {
        index =  0;
    }

    SCI_TRACE_LOW("[MMISMSCB] MMISMSCB_ConvertLangIDToLangIndex, lang_id = %d, index = %d", lang_id , index);

    return index;
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMIAPISMSCB_NV_LoadSmsCBLanguageList
//  ��������:        ��ȡNV�б����С���㲥����ѡ���б�ṹ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          NV�б����С���㲥����ѡ���б�ṹ
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMIAPISMSCB_NV_LoadSmsCBLanguageList(MN_DUAL_SYS_E dual_sys, MMISMS_CB_LANGUAGE_T* lang_card_list)
{
    NV_LoadSmsCBLanguageList(dual_sys, lang_card_list);
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_Get_CB_NV_Channel_Ptr
//  ��������:        
//  ȫ�ֱ���ʹ��:    s_cb_msg
//  ����ֵ:          ��
//  ����:         
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CHANNEL_T *MMISMSCB_Get_CB_NV_Channel_Ptr( MN_DUAL_SYS_E dual_sys )
{
    return &s_nv_channel_list[dual_sys];
}

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_Get_CB_Save_To_NV_Channel_Ptr
//  ��������:        
//  ȫ�ֱ���ʹ��:    s_cb_msg
//  ����ֵ:          ��
//  ����:         
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CHANNEL_T *MMISMSCB_Get_CB_Save_To_NV_Channel_Ptr( MN_DUAL_SYS_E dual_sys )
{
    return &s_save_to_nv_channel_list;
}
#endif

#endif//#if defined(MMI_SMSCB_SUPPORT)




/*Edit by script, ignore 20 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
