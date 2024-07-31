/*****************************************************************************
** File Name:      mmieng_win.c                                              *
** Author:                                                                   *
** Date:           09/2004                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 9/2004         Allen

******************************************************************************/


#define _MMIENG_WIN_C_

/**--------------------------------------------------------------------------*
                                Include Files      
 **--------------------------------------------------------------------------*/
//this line must be at the fisrt line of include sentences now
#include "mmi_app_eng_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmk_type.h"
#include "mmk_timer.h"
#include "mmipub.h"
#include "window_parse.h"

#include "guilabel.h"

//#include "mmieng.h"
#include "mmieng_internal.h"
#include "mmieng_win.h"
#include "mmieng_uitestwin.h"
#include "mmieng_id.h"
#include "mmieng_text.h"

#ifdef ENG_SUPPORT
#include "mmi_image.h"
#include "mmi_text.h"
#include "guilcd.h"
#include "guicommon.h"
#include "guitext.h"

#include "guifont.h"
#include "tb_dal.h"
#include "mn_api.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "guiprgbox.h"
#ifndef WIN32
#include "adc_parameter.h"
#endif
//#include "mmiset.h"
#include "mmiset_export.h"
#include "production_test.h"
#include "guilistbox.h"
#include "mn_type.h"
//#include "efs.h"
#include "mmi_textfun.h"
#include "layer1_engineering.h"
#include "mmieng_main.h"
#include "mmi_nv.h"
#include "mmieng_menutable.h"
//#include "mmimp3_image.h"
#include "mmi_appmsg.h"
//#include "sig_code.h"
#ifdef GPS_SUPPORT
#include "gps_drv.h"    
#endif

//#include "mmiset_call.h"
#include "mmiset_call_export.h"
//#include "mmiset_wintab.h"
//#include "mmiset_text.h"
#include "mmiset_func.h"
#include "mmisd_export.h"
//#include "version.h"

#include "flash.h"
#include "mn_api.h"
#include "mmidc_export.h"
#include "mmidc_camera_text.h"
#include "mmicc_export.h"
#include "mmiudisk_export.h"

/*[START] Jerry Liang for ENG MODE powersweep 2008/08/07*/
//#include "mmiphone_export.h" //modify by mary
/*[END] Jerry Liang for ENG MODE powersweep 2008/08/07*/
#include "mmieng_nv.h"
#include "mmidc_setting.h"
#include "mmidc_export.h"
#include "dal_dcamera.h"

#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif


#include "ref_param.h"

#include "mmifmm_export.h"
//#include "guires.h"
#include "mmimp3_export.h"
#include "mmisrvaud_api.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "guiedit.h"
#include "guiim_base.h"
#ifdef PCLINK_SUPPORT
#include "app_tcp_if.h"
#endif

#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif

#ifdef CMMB_SUPPORT
#include "mtv_api.h"
#include "mbbms_service.h"
#include "mmimtv_main.h"
#include "mtv_setting.h"
#endif
#include "tcpip_api.h"
#include "app_tcp_if.h"
#include "guistring.h"
#include "block_mem.h"
#include "sfs.h"
#include "mmieng_text.h"
#include "mmiconnection_export.h"
//#include "mmiset_display.h"

#ifdef MBBMS_SUPPORT
#include "mmimbbms_main.h"
#endif
#include "mmisms_export.h"
#ifndef _WIN32
#ifdef ENGTD_SUPPORT
#include "layer1_engineering.h"
#endif
#endif
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif

#ifdef WIFI_SUPPORT
#include "wifisupp_api.h"
#endif

#include "mmi_appmsg.h"
#include "mmidisplay_data.h"
#include "guiform.h"
#include "guictrl_api.h"

#ifdef _WIN32
#define __packed
#endif

#include "admm_III.h"
#include "prod_param.h" 

#include "mmi_modu_main.h"
//#include "nvitem.h"
#include "power.h"
#include "tp_export.h"    
#include "audio_config.h"
#include "audio_api.h"
#ifdef MMI_AUTOTEST_SUPPORT
#include "mmi_autotest.h"
#endif

#include "guisetlist.h"

#ifdef TF_LOAD_SUPPORT
#include "tf_load.h"
#include "chg_drv.h"
#include "tf_cfg.h"
#include "mmi_appmsg.h"
#include "mmiacc_id.h"
#include "version.h"
#endif

//#ifdef MMI_DUAL_BATTERY_SUPPORT
#include "dualbat_drvapi.h"
//#endif
#ifdef DSP_USB_LOG
#include "dsp_log.h"      //xuefang.jiang 20110729
#endif

#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#ifdef MMISRV_AUDIO_TEST_SUPPORT
#include "mmisrvaud_test.h"
#endif
#include "nvitem.h"

#include "mmimultim_text.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif

#ifdef SFR_SUPPORT_CUCC
#include "sfr_nv.h"
#endif
/**--------------------------------------------------------------------------*
                                     MACRO DEFINITION
 **--------------------------------------------------------------------------*/
#define MMIENG_WAIT_MSG         "Please wait..."
#define MMIENG_WAIT_MSG_LEN     14

#define MMIENG_AFC_MAX_VALUE     65535   //1023
#define MMIENG_PA_MAX_VALUE     65535

#define MMIENG_PROMPT_TIMEOUT   2000
#define ENG_WAPUA_MAX_LEN       512 

#define MMIENG_ADDR_INFO_MAX_LEN       64
#define MMIENG_RF_TIME                 1000

#define MMIENG_CMMB_CHANNEL_INFO_MAX_LEN    64

#define MMIENG_AUDIO_SET_VOLMAX  36

#define MMIENG_AUDIO_VOL_MAX 9
//add for mp3 audio gain setting
#define MMIENG_MP3_AUDIO_VOL_MAX 15
#define MMIENG_AUDIO_GAIN_LEVEL_MAX 35

#ifdef PCLINK_SUPPORT
#define     MMIENG_PCLINK_MAX_IP_LEN            15          //ip,DNS,subnet mas,and 网关地址的最大长度
#define     MMIENG_PCLINK_PINGTEST_TIMER_OUT        5000    //pc link ping test等待超时的时间，5s   
#endif

#define MMIMTV_INPUT_PAGE_RANGE               20


#define MMIENG_IQ_DATA_SIZE  (50*1024)
#define ENG_IQ_TEST_BUF_SIZE  (50 * 1024)

#define MMIENG_USER_AGENT_MTK "MAUI WAP Browser"
#define MMIENG_USER_AGENT_09A "SonyEricssonK700c/R2AE SEMC-Browser/4.0.3 Profile/MIDP-2.0 Configuration/CLDC-1.1"
#define MMIENG_USER_AGENT_DORADO "Dorado WAP-Browser/1.0.0"
#define MMIENG_USER_AGENT_NOKIA "NokiaN95/1.0 (15.1.002) SymbianOS/9.2 Series60/3.1 Profile/MIDP-2.0 Configuration/CLDC-1.1"
#define MMIENG_USER_AGENT_NF "HS-N51/1.0 Release/10.25.2009 Browser/NF3.5 Profile/MIDP-2.0 Config/CLDC-1.1"

#define MMIENG_UA_PROFILE_NOKIA "http://nds1.nds.nokia.com/uaprof/NN95-5r100.xml"
#define MMIENG_UA_PROFILE_SONYERICSSION "http://wap.sonyericsson.com/UAprof/K700cR201.xml"

#define ENG_UPLMN_POP_MENU_NODE_ID    0
#define ENG_UPLMN_POP_MENU_TEXT_ID    1
#define ENG_UPLMN_POP_MENU_ITEM_DIMEN 2 

typedef enum
{
    ENG_UPLMN_OPT_NULL,
    ENG_UPLMN_ADD,//编辑
    ENG_UPLMN_EDIT,//删除
    ENG_UPLMN_DELETE,//删除全部    
    ENG_UPLMN_OPT_MAX    
}ENG_UPLMN_OPT_E;

const uint32 s_eng_uplmn_opt_item[][ENG_UPLMN_POP_MENU_ITEM_DIMEN] = 
{
    {ENG_UPLMN_ADD, STR_SOFTKEY_ADD_MK},
    {ENG_UPLMN_EDIT, TXT_EDIT},//编辑
    {ENG_UPLMN_DELETE, TXT_COMMON_CLEAR},//删除
};
static uint8 ENG_UPLMN_EDIT_OR_ADD_FLAG ;
typedef enum
{
    MMIENG_RF_CELL_C0 = 0,
    MMIENG_RF_CELL_N0,
    MMIENG_RF_CELL_N1,
    MMIENG_RF_CELL_N2,
    MMIENG_RF_CELL_N3,
    MMIENG_RF_CELL_N4,
    MMIENG_RF_CELL_N5,
    MMIENG_RF_CELL_ALL,
    MMIENG_RF_CELL_MAX
} MMIENG_RF_CELL_E;
typedef enum
{
    MMIENG_DC_PREVIEW = 0,
    MMIENG_DC_CAPTURE,
    MMIENG_DC_REVIEW,

    MMIENG_DC_STATE_MAX
} MMIENG_DC_STATE_E;

typedef struct
{
    int32 BCH;
    int32 BRXL;
    int32 BSIC;
    int32 DSC;
    int32 TN;
    int32 TXLEV;
    int32 RLT;
    int32 TAV;
    int32 CBA;
    int32 CBQ;
    int32 C1;
    int32 C2;    
    int32 LAC;
    int32 CELID;
    int32 ALIG;
    int32 FFSET;
} MMIENG_RF_CELL_INFO_T;

typedef struct
{
    uint8  sec;  
    uint8  min;
    uint16  hour;
} MMIENG_TIME_T;

/*! \brief EditBox and IM data interface */
//typedef struct 
//{
//    GUIEDIT_CTRL_T*       editbox_ptr;
//    const uint16*               add_string;         /*!< [IN]  the string to add editbox when the type is EDITBOX_ADD_CHAR, must be 0-terminate string */
//    uint16                    add_num;            /*!< [IN]  the length of adding char availably when the type is EDITBOX_ADD_CHAR */
//  uint16                      del_num;            //[IN]  type为EDITBOX_DEL_CHAR时有效
//    uint16                    move_left_num;      /*!< [IN]  The number for move left availably when the type istype is EDITBOX_MOVE_LEFT(move continually，cursor move to end from begining ) */
//    uint16                    move_right_num;     /*!< [IN]  The number for move right availably when the type istype is EDITBOX_MOVE_LEFT */
//    uint16                    up_new_pos;         /*!< [OUT] The cursor position avalably when the type istype*/
//    uint16                    down_new_pos;       /*!< [OUT] The cursor position avalably when the type istype为EDITBOX_MOVE_DOWN */
//    uint16                    first_index;        /*!< [IN]  The shadow char begining position avalably when the type is EDITBOX_ENTER_SHADE */
//    uint16                    last_index;         /*!< [IN]  The shadow char end position avalably when the type is  EDITBOX_ENTER_SHADE */
//
//    GUI_RECT_T          rect;         //[OUT] type为EDITBOX_MODIFY_RECT时有效, 控件显示区域，不能利用控件指针直接调整区域，因为控件本身可能有多个区域需要更新
//    BOOLEAN             is_fullscreen;  //[OUT] type为EDITBOX_FULLSCREEN_MODE时有效
//    GUIEDIT_TYPE_E        cur_input_type; //[OUT] type为EDITBOX_SET_INPUTTYPE时有效
//    uint32              cur_input;      //[OUT] type为EDITBOX_SET_INPUT时有效
//    BOOLEAN             is_move_left;   //[IN]  type为EDITBOX_MOVE_DEPTH时有效
//    uint16              move_depth;     //[OUT] type为EDITBOX_MOVE_DEPTH时有效
//} EDITBOX_INPUT_INTERFACE_T;

typedef struct
{
    //double digital_gain;
    //double pga_gain;
    //double pga2_gain;
    float total_gain;
    uint32 hex_value;
    //int16  eng_value;
}MMIENG_AUDIO_MP3_GAIN_T;

#ifdef TF_LOAD_SUPPORT
typedef struct
{
    uint8   *file_addr;  
    uint32  file_len;
} MMIENG_TF_T;


typedef enum
{
    MMIENG_ALL_EXIST=0,
    MMIENG_PAC_NO_EXITS=1,
    MMIENG_FILE_NO_EXITS=2,
    MMIENG_DIR_NO_EXITS=4,
    MMIENG_PAC_ERROR=8,
    MMIENG_FILE_ERROR=16,    
    MMIENG_FILE_INFO_MAX
} MMIENG_TF_FILE_E;

typedef unsigned short WCHAR;

/*.pac file  structure*/
typedef struct _BIN_PACKET_HEADER_T
{
    WCHAR szVersion[24];        // packet struct version
    DWORD  dwSize;              // the whole packet size;
    WCHAR szPrdName[256];       // product name
    WCHAR szPrdVersion[256];   // product version
    int    	    nFileCount;          // the number of files that will be downloaded, the file may be an operation
    DWORD  dwFileOffset;        // the offset from the packet file header to the array of FILE_T struct buffer
    DWORD  dwMode;
    DWORD  dwFlashType;
    DWORD  dwNandStrategy;
    DWORD  dwIsNvBackup;
    DWORD  dwNandPageType;
    WCHAR   szPrdAlias[100];    // product alias
    DWORD  dwOmaDmProductFlag;
    DWORD  dwIsOmaDM;
    DWORD  dwIsPreload;
    DWORD  dwReserved[202];
}BIN_PACKET_HEADER_T,*PBIN_PACKET_HEADER_T;

#ifdef CHIP_ENDIAN_LITTLE
#define TF_WORDSWAP(x)					x
#define TF_DWORDSWAP(x)				x
#else
#define TF_WORDSWAP(x)    		        ((((x)<<8)&0xff00)|(((x)>>8)&0x00ff))
#define TF_DWORDSWAP(x)   	   		 	((((x)<<24)&0xff000000L)|(((x)<<8)&0x00ff0000L)|(((x)>>8)&0x0000ff00L)|(((x)>>24)&0x000000ffL))
#endif
#endif

/**--------------------------------------------------------------------------*
                                     STATIC DEFINITION
 **--------------------------------------------------------------------------*/
extern const MMI_UITEST_DISPLAY_T mmi_uitest_display;
static uint8 s_mmieng_show3result_timer_id = 0;
LOCAL int16 *s_volValue=PNULL;

LOCAL int16 s_vol_array[MMIENG_MP3_AUDIO_VOL_MAX]={0}; //代替MMIENG_AUDIO_VOL_MAX的9级，以配合正常的15级音量

LOCAL AUDMOD_DEVMOD_T s_mode_param={0};
LOCAL AUDIO_NV_ARM_MODE_INFO_T s_arm_audiomode_param={0};

// show net info timer
static uint8 s_mmieng_netinfo_timer_id = 0;
#define PSCELLWIN_LIST_MAX_NUM  10
LOCAL uint16 s_cell_num = 0; 
LOCAL int16 s_cell_arfcn[MN_MAX_NCELL_NUM]; // Neighbour cells measurement report 

static uint8 s_mmieng_up_down_link = 0; // 0 -- uplink  1 -- downlink
static AUDIO_DEVICE_MODE_TYPE_E s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;/*lint !e551*/
//static int16 s_mmieng_contrast_value = 0;


static int16 s_mmieng_brightness_value = 0;
static int16 s_mmieng_save_brightness_value = 95;

static BOOLEAN s_mmieng_afc_inited = FALSE;
static BOOLEAN s_mmieng_pa_inited = FALSE;
static BOOLEAN s_mmieng_agc_inited = FALSE;
static BOOLEAN s_mmieng_errrate_inited = FALSE;

// PLMN info
static MN_PHONE_FPLMN_LIST_T s_mmieng_fplmn = {0};

//#ifdef PCLINK_SUPPORT
//static BOOLEAN s_is_pclink_ping_start = FALSE;
//#endif

#ifdef GPS_SUPPORT
#define GPS_TEST_TEXT_LEN        1024
#define GPS_TEST_TEMP_BUF_LEN    4096
#define GPS_TEST_TIMER_OUT       1000
#define GPS_TEST_MIN_VALID_SNR   13
#define GPS_TEST_DELAY_TIMES     3

LOCAL BOOLEAN s_gps_test_open = FALSE;
LOCAL uint8   s_gps_test_timer = 0;
LOCAL uint32  s_gps_test_position_fix_time = 0;
LOCAL uint32  s_gps_test_position_delay_time = 0;
LOCAL GPS_INFO_T s_gps_test_pre_info = {0};

#endif

LOCAL BOOLEAN s_mtv_dsp_iq_test = FALSE;
LOCAL MMIFILE_HANDLE s_mtv_iq_fh = PNULL;
LOCAL uint32   s_iqdata_save_len = 0;

#ifndef _WIN32
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#ifndef MMI_DUALMODE_ENABLE
extern void RRA_GetRraMonItem(RRA_MONITOR_ITEM_T *item_ptr);  //pizer.fan temp-modify
#endif
#endif
//extern MN_RETURN_RESULT_E MNENG_GetCellBasicInfo (MN_ENG_CELL_BASIC_INFO_T *cell_basic_info);
extern MN_RETURN_RESULT_E MNENG_GetSCellTrafInfo (MN_ENG_SCELL_TRAF_INFO_T *scell_traf_info);
//extern MN_RETURN_RESULT_E MNENG_GetSCellCtrlInfo (MN_ENG_SCELL_CTRL_INFO_T *scell_ctrl_info);
//extern MN_RETURN_RESULT_E MNENG_GetCellIdleInfo (
//                            MN_ENG_CELL_IDLE_INFO_T *cell_idle_info
//                            );
#endif                            
// #ifndef WIN32                             
// extern int32 EFS_ReadPhaseCheck(uint8 *buffer,uint16  buffer_length) ;
// #endif

//extern char   *Cms_Browser_GetVersion( void );
//extern char    *Cms_MMS_Get_Version(void);
#ifndef _WIN32
#if defined(IM_ENGINE_CSTAR)
extern const char* CS_IMM_GetVersion(void);
#elif defined(IM_ENGINE_IEKIE)
extern const char* IIME_IMM_GetVersion(void);
#endif
#endif
#ifdef PCLINK_SUPPORT
extern BOOLEAN TCPIPPCLINK_ResetNetInterface( void );
extern BOOLEAN TCPIPPCLINK_SetNetInterface( void );
#endif
#if 0
extern void MMIBRW_SetUAProfile(uint8 *ua_profile_ptr, uint16 ua_profile_length);
extern void MMIBRW_SetUA(uint8 *ua_ptr, uint16 ua_length);
#endif
#ifdef CMMB_SUPPORT
LOCAL uint8 s_cmmb_timer = 0;
LOCAL CMMB_CHANNEL_INFO_T s_config_channel_info = {0};
#ifdef MBBMS_SUPPORT
LOCAL BOOLEAN s_mbbms_interface_test_enable = FALSE;
//LOCAL BOOLEAN s_mbbms_gba_test_enable = FALSE;
#ifdef DEMOD_HW_SIANO
LOCAL BOOLEAN s_mbbms_log_control_enable = FALSE;
#endif
//LOCAL BOOLEAN s_mbbms_msk_test_enable = FALSE;
LOCAL BOOLEAN s_mbbms_smd_counter_enable = FALSE;
#endif
LOCAL BOOLEAN s_mbbms_audio_enable = FALSE;
#endif

LOCAL const wchar s_saved_wstr[] = {  'S', 'a', 'v', 'e', 'd', 0 };
LOCAL const wchar s_fail_saved_wstr[] = {  'F', 'a', 'i', 'l', ' ' ,'s', 'a', 'v', 'e', 'd', 0 };
LOCAL const wchar s_fail_to_wstr[] = {  'F', 'a', 'i', 'l', ' ' ,'t', 'o', 0 };
LOCAL const wchar s_get_production_wstr[] = {  'g', 'e', 't', ' ', 'p' ,'r', 'o', 'd', 'u', 'c', 't', 'i', 'o', 'n' ,0 };
LOCAL const wchar s_set_wstr[] = {  'S', 'e', 't', 0 };
LOCAL const wchar s_audio_param_wstr[] = {  'a', 'u', 'd', 'i', 'o', ' ', 'p', 'a', 'r', 'a', 'm', 0 };
LOCAL const wchar s_success_wstr[] = {  's', 'u', 'c', 'c', 'e', 's', 's' , 0 };
LOCAL const wchar s_fail_wstr[] = {  'f', 'a', 'i', 'l', 0 };
LOCAL const wchar s_long_press_wstr[] = {  'l', 'o', 'n', 'g', ' ', 'p', 'r', 'e', 's', 's', 0 };
LOCAL const wchar s_red_key_will_wstr[] = {  'r', 'e', 'd', ' ', 'k', 'e', 'y', ' ', 'w', 'i', 'l', 'l', 0 };
LOCAL const wchar s_power_off_wstr[] = {  'p', 'o', 'w', 'e', 'r', ' ', 'o', 'f', 'f', 0 };
LOCAL const wchar s_do_nothing_wstr[] = {  'd', 'o', ' ', 'n', 'o', 't', 'h', 'i', 'n', 'g', 0 };
LOCAL const wchar s_space_wstr[] = { ' ', ' ',  0 };
LOCAL const wchar s_bch_wstr[] = { 'B', 'C', 'H', 0 };
LOCAL const wchar s_brxl_wstr[] = { 'B', 'R', 'X', 'L',  0 };
LOCAL const wchar s_c0_wstr[] = { 'C', '0', 0 };
LOCAL const wchar s_c1_wstr[] = { 'C', '1', 0 };
LOCAL const wchar s_c2_wstr[] = { 'C', '2', 0 };
LOCAL const wchar s_n0_wstr[] = { 'N', '0', 0 };
LOCAL const wchar s_n1_wstr[] = { 'N', '1', 0 };
LOCAL const wchar s_n2_wstr[] = { 'N', '2', 0 };
LOCAL const wchar s_n3_wstr[] = { 'N', '3', 0 };
LOCAL const wchar s_n4_wstr[] = { 'N', '4', 0 };
LOCAL const wchar s_n5_wstr[] = { 'N', '5', 0 };
LOCAL const wchar s_bsic_wstr[] = { 'B', 'S', 'I', 'C',  0 };
LOCAL const wchar s_dsc_wstr[] = { 'D', 'S', 'C',  0 };
LOCAL const wchar s_txlev_wstr[] = { 'T', 'X', 'L', 'E', 'V', 0 };
LOCAL const wchar s_tn_wstr[] = { 'T', 'N', 0 };
LOCAL const wchar s_rlt_wstr[] = { 'R', 'L', 'T', 0 };
LOCAL const wchar s_tav_wstr[] = { 'T', 'A', 'V', 0 };
LOCAL const wchar s_cba_wstr[] = { 'C', 'B', 'A', 0 };
LOCAL const wchar s_cbq_wstr[] = { 'C', 'B', 'Q', 0 };
LOCAL const wchar s_info_wstr[] = { 'I', 'N', 'F', 'O', 0 };
LOCAL const wchar s_celid_wstr[] = { 'C', 'E', 'L', 'I', 'D', 0 };
LOCAL const wchar s_alig_wstr[] = { 'A', 'L', 'I', 'G', 0 };
LOCAL const wchar s_lac_wstr[] = { 'L', 'A', 'C', 0 };
LOCAL const wchar s_ffset_wstr[] = { 'F', 'F', 'S', 'E', 'T', 0 };
LOCAL MMI_TEXT_ID_T s_vol_table[]={
TXT_ENG_AUDIO_VOLUME1,
TXT_ENG_AUDIO_VOLUME2,
TXT_ENG_AUDIO_VOLUME3,
TXT_ENG_AUDIO_VOLUME4,
TXT_ENG_AUDIO_VOLUME5,
TXT_ENG_AUDIO_VOLUME6,
TXT_ENG_AUDIO_VOLUME7,
TXT_ENG_AUDIO_VOLUME8,
TXT_ENG_AUDIO_VOLUME9,
TXT_ENG_AUDIO_VOLUME10,
TXT_ENG_AUDIO_VOLUME11,
TXT_ENG_AUDIO_VOLUME12,
TXT_ENG_AUDIO_VOLUME13,
TXT_ENG_AUDIO_VOLUME14,
TXT_ENG_AUDIO_VOLUME15
};
LOCAL MMIENG_AUDIO_MP3_GAIN_T *s_mp3_gain_array=PNULL;

LOCAL MMIENG_AUDIO_MP3_GAIN_T s_mp3_handsfree[MMIENG_AUDIO_GAIN_LEVEL_MAX] = 
{
{-48.14,    0x3c0001},    //1},
{-46.56,    0x3a0001},    //2},
{-45.08,    0x380001},    //3},
{-43.59,    0x360001},    //4},
{-42.12,    0x340001},    //5},
{-40.54,    0x320001},    //6},
{-39.06,    0x300001},    //7},
{-37.57,    0x2e0001},    //8},
{-36.1 ,    0x2c0001},    //9},
{-34.52,    0x2a0001},    //10},
{-33.04,    0x280001},    //11},
{-31.55,    0x260001},    //12},
{-30.08,    0x240001},    //13},
{-28.5 ,    0x220001},    //14},
{-27.02,    0x200001},    //15},
{-25.53,    0x1e0001},    //16},
{-24.06,    0x1c0001},    //17},
{-22.48,    0x1a0001},    //18},
{-21   ,    0x180001},    //19},
{-19.48,    0x1a0002},    //20},
{-18   ,    0x180002},    //21},
{-16.48,    0x1a0003},    //22},
{-15   ,    0x180003},    //23},
{-13.48,    0x1a0004},    //24},
{-12   ,    0x180004},    //25},
{-10.48,    0x1a0005},    //26},
{-9    ,    0x180005},    //27},
{-7.48 ,    0x1a0006},    //28},
{-6    ,    0x180006},    //29},
{-4.48 ,    0x1a0007},    //30},
{-3    ,    0x180007},    //31},
{-1.48 ,    0x1a0008},    //32},
{ 0    ,    0x180008},    //33},
{ 1.52 ,    0x1a0009},    //34},
{ 3    ,    0x180009}    //35}
};

LOCAL MMIENG_AUDIO_MP3_GAIN_T s_mp3_headset[MMIENG_AUDIO_GAIN_LEVEL_MAX] = 
{
{-51.14,    0x3c0004},    //1},
{-49.56,    0x3a0004},    //2},
{-48.08,    0x380004},    //3},
{-46.59,    0x360004},    //4},
{-45.12,    0x340004},    //5},
{-43.54,    0x320004},    //6},
{-42.06,    0x300004},    //7},
{-40.57,    0x2e0004},    //8},
{-39.1 ,    0x2c0004},    //9},
{-37.52,    0x2a0004},    //10},
{-36.04,    0x280004},    //11},
{-34.55,    0x260004},    //12},
{-33.08,    0x240004},    //13},
{-31.5 ,    0x220004},    //14},
{-30.02,    0x200004},    //15},
{-28.53,    0x1e0004},    //16},
{-27.06,    0x1c0004},    //17},
{-25.48,    0x1a0004},    //18},
{-24   ,    0x180004},    //19},
{-22.48,    0x1a0005},    //20},
{-21   ,    0x180005},    //21},
{-19.48,    0x1a0006},    //22},
{-18   ,    0x180006},    //23},
{-16.48,    0x1a0007},    //24},
{-15   ,    0x180007},    //25},
{-13.48,    0x1a0008},    //26},
{-12   ,    0x180008},    //27},
{-10.48,    0x1a0009},    //28},
{-9    ,    0x180009},    //29},
{ -7.48,    0x1a000a},    //30},
{ -6   ,    0x18000a},    //31},
{ -4.48,    0x1a000b},    //32},
{ -3   ,    0x18000b},    //33},
{ -1.48,    0x1a000c},    //34},
{  0   ,    0x18000c}    //35}
};

LOCAL MMIENG_AUDIO_MP3_GAIN_T s_mp3_headfree[MMIENG_AUDIO_GAIN_LEVEL_MAX] = 
//Total gain,    Hex Value,    Value in Engineering mode
{
{-48.14,    0x3c0041},    //1},
{-46.56,    0x3a0041},    //2},
{-45.08,    0x380041},    //3},
{-43.59,    0x360041},    //4},
{-42.12,    0x340041},    //5},
{-40.54,    0x320041},    //6},
{-39.06,    0x300041},    //7},
{-37.57,    0x2e0041},    //8},
{-36.1 ,    0x2c0041},    //9},
{-34.52,    0x2a0041},    //10},
{-33.04,    0x280041},    //11},
{-31.55,    0x260041},    //12},
{-30.08,    0x240041},    //13},
{-28.5 ,    0x220041},    //14},
{-27.02,    0x200041},    //15},
{-25.53,    0x1e0041},    //16},
{-24.06,    0x1c0041},    //17},
{-22.48,    0x1a0041},    //18},
{-21   ,    0x180041},    //19},
{-19.48,    0x1a0052},    //20},
{-18   ,    0x180052},    //21},
{-16.48,    0x1a0063},    //22},
{-15   ,    0x180063},    //23},
{-13.48,    0x1a0074},    //24},
{-12   ,    0x180074},    //25},
{-10.48,    0x1a0085},    //26},
{-9    ,    0x180085},    //27},
{-7.48 ,    0x1a0096},    //28},
{-6    ,    0x180096},    //29},
{-4.48 ,    0x1a00a7},    //30},
{-3    ,    0x1800a7},    //31},
{-1.48 ,    0x1a00b8},    //32},
{ 0    ,    0x1800b8},    //33},
{ 1.52 ,    0x1a00c9},    //34},
{ 3    ,    0x1800c9}    //35}
};

#ifdef TF_LOAD_SUPPORT
MMIENG_TF_T s_tf_load;
extern const char*  const s_version_info[];

#define TF_UPGRADING_VBAT_MIN  3700  //about 3.7v
wchar g_device_sdcard[2][2]   = {{ 'E', 0 }, { 'F', 0 }};
wchar *g_valid_sdcard = NULL;
//#define MMIFILE_DEVICE_SDCARD               (g_device_sdcard)//"\x00\x45\x00\x00"
#define MMIFILE_DEVICE_SDCARD_LEN           1 //unicode number
#define MMIFILE_PARSE_SIZE                  4096
BOOLEAN slot_exist[2] = {FALSE, FALSE};
#endif

/**--------------------------------------------------------------------------*
                                     TYPE AND CONSTANT
 **--------------------------------------------------------------------------*/
static MMI_STRING_T s_mmieng_show3result_text1 = {0};
static MMI_STRING_T s_mmieng_show3result_text2 = {0};
static MMI_STRING_T s_mmieng_show3result_text3 = {0};

LOCAL const MMI_TEXT_ID_T s_mmieng_sel_band_txt_tab[] = 
{
    TXT_ENG_BAND_PCS,
    TXT_ENG_BAND_GSM,
    TXT_ENG_BAND_DCS,
    TXT_ENG_BAND_DUL,
    TXT_ENG_BAND_QUAD
};
LOCAL MN_DUAL_SYS_E g_dual_sys_eng_win = 0;
#ifdef WIN32
#ifndef ENGTD_SUPPORT
#define MAX_TD_NCELL_REPORT_NUM 32

typedef struct
{
    uint16  arfcn;
    uint8   cell_id;
    int8    rscp;
}LAYER1_TD_NCELL_T;

typedef struct
{
   LAYER1_TD_NCELL_T td_ncell[MAX_TD_NCELL_REPORT_NUM];
}LAYER1_TD_NCELL_ARR_T;

#endif
#endif


/**--------------------------------------------------------------------------*
                                     LOCAL FUNCTION DECLARE
 **--------------------------------------------------------------------------*/

/********************************************************************************
 NAME:          EngCardLogWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        bin.ji
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngCardLogWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngCardLog_WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
//[MS]
LOCAL MMI_RESULT_E EngDTMFSetTimerWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#ifdef NPS_SUPPORT
LOCAL MMI_RESULT_E EngNPSTimerChangeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
LOCAL MMI_RESULT_E Eng_IMS_NV_PARAM_ID_WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E Eng_UPLMN_EDIT_AND_EDIT_WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E  MMIENG_HandleDispPreferNetworkListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E  HandleEngUPLMNOptionMenuMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E  MMIENG_HandleUPLMNRATSETWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngAutoAnswerCallWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngProductSNWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngShow3ResultWinHandleMsgNoAutoClose(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngShow3ResultWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngShowHelpWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL int GetSCNetInfoStr(uint8* net_buf, uint16 buf_len);

LOCAL int GetNCNetInfoStr(uint8* net_buf, uint16 buf_len);

LOCAL int32 EngGetNetInfo(char *buf, int32 buf_length);

LOCAL MMI_RESULT_E EngShowNetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngPSCellWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E ENGNetinfoShowWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E ENGNetinfoStatisticsWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E ENGNWCAPShowWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E ENG_Netinfo_Statistics_WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngPSCellOptionMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL int GetPhoneInfoStr(uint8* phone_buf, uint32 buf_len);

LOCAL MMI_RESULT_E EngShowPhoneWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngAssertTargetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL uint32 AppendForbidPLMNItem(MMI_CTRL_ID_T list_id);

//LOCAL MMI_RESULT_E HandlePLMNPromptWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngForbidPLMNWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngNetLostWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL int GetTCVInfoStr(uint8* tcv_buf, uint8 buf_len);

LOCAL MMI_RESULT_E EngTCVWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#if 0
LOCAL MMI_RESULT_E HandleAFCWaitWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngAFCWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E HandlePAWaitWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngPAWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E HandleAGCWaitWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL int GetAGCInfoStr(uint8* agc_buf, uint8 buf_len);

LOCAL MMI_RESULT_E EngAGCWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
LOCAL MMI_RESULT_E HandleErrRateWaitWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL int GetErrRateInfoStr(uint8* rate_buf, uint8 buf_len);

LOCAL MMI_RESULT_E EngErrRateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngUSBLOGWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngCAPOUTPUTWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
// LOCAL MMI_RESULT_E EngContrastWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngBrightnessWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL void ShowSetAudioParamResult(BOOLEAN result);

LOCAL MMI_RESULT_E EngSidetoneWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngVoiceParamWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL int32 GetProductionInfoStr(char* pd_buf, int32 buf_len);

LOCAL int32 GetProductionSNStr(uint8* pd_buf, uint8 buf_len,  const PRODUCTION_TEST_INFO_T *s_mmieng_product);


LOCAL MMI_RESULT_E EngProductWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL void ShowSetPowerKeyResult(BOOLEAN result);

LOCAL void AppendPowerKeyItem(MMI_CTRL_ID_T list_id);

LOCAL void AppendListItem(
                            MMI_CTRL_ID_T list_id,
                            MMI_TEXT_ID_T *text_id_tab,
                            int32 tab_len
                            );

LOCAL MMI_RESULT_E EngPowerKeyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef DSP_USB_LOG
LOCAL MMI_RESULT_E EngDspCardLogEnableWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);//xuefangjiang 20110729
#endif

LOCAL MMI_RESULT_E EngSensorModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);//@songbin.zeng

LOCAL void AppendBandSelectItem(MMI_CTRL_ID_T list_id);

LOCAL MMI_RESULT_E EngBandSelectWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngShowLayer1MonitorWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*[START] Jerry Liang for ENG MODE powersweep 2008/07/30*/
//LOCAL MMI_RESULT_E EngShowPowersweepInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*[END] Jerry Liang for ENG MODE powersweep 2008/07/30*/

LOCAL MMI_RESULT_E EngShowRFWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL BOOLEAN GetRFCellInfo(MMIENG_RF_CELL_INFO_T *info, MMIENG_RF_CELL_E cell);

LOCAL void DrawRFWin(MMIENG_RF_CELL_E cell );

LOCAL int16 EngDVCovFrameFre2ListItemPos(MMIENG_DV_FRAME_FRE_E dv_frame_frequency);

LOCAL MMIENG_DV_FRAME_FRE_E EngDVCovListItemPos2FrameFre(int16 item_pos);

LOCAL MMI_RESULT_E EngDVFrameFreWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#if 0
LOCAL MMI_RESULT_E EngWapUAWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngWapUAProfileWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngWapMmsSasTestModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL BOOLEAN SetWapMmsSasTestMode(BOOLEAN is_on);
#endif
LOCAL MMI_RESULT_E BtAddressInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#ifndef WIN32
#ifdef  BLUETOOTH_SUPPORT_SPRD_BT
LOCAL MMI_RESULT_E BtVerInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
#endif
LOCAL MMI_RESULT_E BTSSPDebugModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E BTBQBModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef GPS_SUPPORT
LOCAL void SetGPSText( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, BOOLEAN is_open, BOOLEAN is_need_update );

LOCAL MMI_RESULT_E EngShowGPSWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
LOCAL MMI_RESULT_E HandleChipTestMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef CMMB_SUPPORT
LOCAL MMI_RESULT_E HandleCMMBTestMainMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleCMMBTestWaitWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleCMMBRDTestMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleCMMBFactoryTestMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleCMMBTestOKSignalMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleCMMBTestNoSignalMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleCMMBSerialNumberWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E HandleCMMBDisplayWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL void DisplayChannelStatusInfo(MTV_CHANNEL_STATUS_T* channel_info_ptr);
LOCAL MMI_RESULT_E EngMtvLOGSwitchWinHandleMsg(MMI_WIN_ID_T      win_id,
                                               MMI_MESSAGE_ID_E  msg_id,
                                               DPARAM            param);
LOCAL MMI_RESULT_E EngMtvLOGToBBSwitchWinHandleMsg(MMI_WIN_ID_T      win_id,
                                                   MMI_MESSAGE_ID_E  msg_id,
                                                   DPARAM            param);
LOCAL MMI_RESULT_E EngMtvLOGStorageWinHandleMsg(MMI_WIN_ID_T      win_id,
                                                MMI_MESSAGE_ID_E  msg_id,
                                                DPARAM            param);
#ifdef MBBMS_SUPPORT
#ifdef DEMOD_HW_SIANO
LOCAL MMI_RESULT_E HandleLOGControlEnableWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

LOCAL MMI_RESULT_E HandleInterfaceTestEnableWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleSMDCounterEnableWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
LOCAL MMI_RESULT_E HandleAudioServiceEnableWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

LOCAL MMI_RESULT_E HandleChipTestPlayMp3MP4WinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E HandleChipTestDCWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E HandleChipTestLoopBackWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E HandleChipTestSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngIQModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngIQModeOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleIQDataWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngIQDataModeSelectWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngIQDataPowerOnStartWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleIQDataModeSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleIQDataModeSettingFormatWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleIQDataModeSettingTimeLimitWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleIQDataModeSettingFrameLengthWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleIQDataModeSettingBlockCountWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*[START] Jerry Liang for IQ MODE 2008/07/31*/        
LOCAL MMI_RESULT_E HandleIQDataModeSettingARFCNWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*[END] Jerry Liang for IQ MODE 2008/07/31*/        

LOCAL MMI_RESULT_E HandleIQDataModeSettingChannelWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleIQDataQueryWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngShow3rdPartyVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL void Show3rdPartyVersionInfo(BOOLEAN   need_update);

LOCAL int32 GetAdcCalibrateInfoStr(uint8* adc_info_buf, uint16 buf_len);
LOCAL MMI_RESULT_E EngShowAdcCalibrateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef PCLINK_SUPPORT
LOCAL MMI_RESULT_E HandlePClinkEnableWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandlePClinkInputIPAddrWinMsg(MMI_WIN_ID_T   win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandlePClinkInputSNMaskWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandlePClinkInputDefGWWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandlePClinkInputDNSWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);  

LOCAL MMI_RESULT_E HandlePCLinkInputTestAdfressWinMsg(MMI_WIN_ID_T  win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandlePCLinkPingTestWaitWinMsg(MMI_WIN_ID_T  win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);                                  
#endif

#ifdef CAMERA_SUPPORT
LOCAL MMI_RESULT_E HandleDCParamScreenSizeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleDCParamScreenSwitchWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleDCParamScreenPrevSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);  
LOCAL MMI_RESULT_E HandleDCParamSensorAngleWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleDCParamSensorIDWinMsg(MMI_WIN_ID_T win_id,
                                                          MMI_MESSAGE_ID_E msg_id,
                                                          DPARAM param
                                                          );

LOCAL BOOLEAN CheckIfDCScreenSwitchPrevSetSupported(
                                                CAMERA_ENG_VISUAL_MODE_E switch_mode,
                                                CAMERA_ENG_PREVIEW_MODE_E prev_mode);
#endif
LOCAL MMI_RESULT_E EngAudioSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngAudioVolWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL void MMIENG_AppendListItemByTextId(                                   
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style,
                                                uint8 * data_buf
                                                );
LOCAL double MMIENG_Log(double m,double base);   
LOCAL double MMIENG_Ln(double x);
LOCAL double MMIENG_Sqrt(double b);
LOCAL double MMIENG_Horner(double x);
LOCAL double MMIENG_Coef(int n);   
LOCAL double MMIENG_Exp(double x);    
LOCAL double MMIENG_Pow(double m,double n);    
LOCAL int16 MMIENG_DextoInt(uint8 * buf);
LOCAL MMI_RESULT_E EngAudioMp3WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL uint32 EngAudioRead(uint32 menu_id , int16 * vol_array);
LOCAL uint32 EngAudioWrite(uint32 menu_id , int16 * vol_array);
LOCAL void AudioMp3AppendListItem(uint16 index);

#ifdef WIN32
PUBLIC uint32 PROD_ReadAudioParam(  
             NV_AUDIO_E   audio_mode,
             void*        mode_param_ptr
             );

PUBLIC uint32 PROD_WriteAudioParam(
    //const uint8* pucModeName, 
    NV_AUDIO_E   audio_mode, 
    void*        mode_param_ptr,
    BOOLEAN      is_flush
    );
#endif

LOCAL uint32 GetRunningNVCountingInfo(char *buffer, uint32 buffer_lengh) ;
LOCAL MMI_RESULT_E HandleRunningNVCountingWinMsg(
                                                 MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                 DPARAM             param       // 相应消息的参数
                                                 );

//MS00216980 cheney
LOCAL MMI_RESULT_E EngCMCCTestWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL void MMI_CalculateStandbyTime2File(void);
#ifdef MMI_SMS_RETRY_SUPPORT
LOCAL MMI_RESULT_E EngSMSRetryWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif



LOCAL MMI_RESULT_E EngCallBarringWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);//MS00186848
LOCAL MMI_RESULT_E EngUpdateIMEIWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngUpdateIMEIEditWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngGetFlashInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#ifdef WRE_SUPPORT
LOCAL MMI_RESULT_E EngShowWREVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
#ifdef ENG_SECONDEDAC_SUPPORT
LOCAL MMI_RESULT_E EngSECONDEDACEnableWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
LOCAL MMI_RESULT_E EngMemPressTestEnterWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngMemPressTestWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef SFR_SUPPORT_CUCC
LOCAL uint8 EngCUCCDMRegMode2ListItemPos(MMIENG_CUCCDM_REGMODE_E cuccdm_regmode);
LOCAL MMIENG_CUCCDM_REGMODE_E EngCUCCDMRegModeListItemPos2FrameFre(int16 item_pos);
#endif
/*****************************************************************************/
//  Description : handle set lcd logic angle window msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E EngSetAngleWinHandleMsg(
                                           MMI_WIN_ID_T         win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM               param
                                           );

#ifdef ENGTD_SUPPORT
LOCAL int GetTDNCNetInfoStr(uint8* net_buf, uint16 buf_len);
#endif


PUBLIC void MMIENG_Int2Str(int16 value, uint8* buf, uint16 buf_len);
/*****************************************************************************/
//  Description : HandleUaAgentWinMsg
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaAgentWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HandleUaAgentWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaAgentSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HandleUaProfileWinMsg
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaProfileWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HandleUaProfileSetWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaProfileSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HandleUaAgentListWinMsg
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaAgentListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HandleUaProfileListWinMsg
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaProfileListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : HandleUaAgentShowWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaAgentShowWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HandleUaProfileShowWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaProfileShowWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HandleUaAgentEditWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaAgentEditWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HandleUaProfileEditWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaProfileEditWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HandleBrowserLogWinMsg
//  Global resource dependence : 
//  Author: sally.he
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBrowserLogWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : change url win
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBrowserChangeURLWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HandleScreenLogWinMsg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleScreenLogWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E  HandleChiptestTpWinMsg (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef MMI_AUTOTEST_SUPPORT
/*****************************************************************************/
//  Description : HandleAutoTestSetWinMsg
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoTestSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

#ifdef WIFI_SUPPORT
LOCAL MMI_RESULT_E HandleWlanPerformanceModeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleWifiIperfWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleWlanRfSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);


#ifndef WIN32
extern void iperf_test_start(char* param);
#endif

#ifndef _WIN32
extern char* WIFI_GetVersion(void);
#endif
#endif

//for SUPPORT_STANDBY_RECORD
LOCAL MMI_RESULT_E StandbyTimeInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
//end for SUPPORT_STANDBY_RECORD

#ifdef TF_LOAD_SUPPORT
LOCAL MMI_RESULT_E HandleTFLoadWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
PUBLIC void GPIO_SetLcdBackLight (BOOLEAN b_on);

#endif


#ifdef MMISRV_AUDIO_TEST_SUPPORT
/*****************************************************************************/
// 	Description : Audio self test, play audio.
//	Global resource dependence : none
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EngAudioSelfTestPlayHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : Audio self test, display audio info.
//	Global resource dependence : none
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EngAudioSelfTestDisplayInfoHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : Audio self test.
//	Global resource dependence : none
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EngAudioSelfTestHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

#if 0
LOCAL MMI_RESULT_E  EngAddContactHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
#ifdef MP3_SUPPORT
LOCAL MMI_RESULT_E EngMp3PlayWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

#ifdef SFR_SUPPORT_CUCC
/********************************************************************************
 NAME:          EngCUCCDMSetClyWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngCUCCDMSetClyWinHandleMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param);
/********************************************************************************
 NAME:          EngCUCCDMSetClyWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngCUCCDMSetModeWinHandleMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param);
#endif
/**--------------------------------------------------------------------------*
                                     GLOBAL DEFINITION
 **--------------------------------------------------------------------------*/
#ifdef PCLINK_SUPPORT
MN_GPRS_PCLINK_CFG_T    s_pclink_config_info = {0};
#endif

WINDOW_TABLE(MMIENG_SHOW3RESULT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngShow3ResultWinHandleMsg),
    WIN_ID(MMIENG_SHOW3RESULT_WIN_ID),
    WIN_TITLE(TXT_ENG_RESULT),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL2_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL3_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_SHOW3RESULT_NOAUTOCLOSE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngShow3ResultWinHandleMsgNoAutoClose),
    WIN_ID(MMIENG_SHOW3RESULT_WIN_ID),
    WIN_TITLE(TXT_ENG_RESULT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL2_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL3_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_SHOWHELP_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngShowHelpWinHandleMsg),
    WIN_ID(MMIENG_SHOWHELP_WIN_ID),
    WIN_TITLE(TXT_ENG_HELP_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_SHOW_3RDPARTY_VERSION_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngShow3rdPartyVersionWinHandleMsg),
    WIN_ID(MMIENG_SHOW_3RDPARTY_VERSION_WIN_ID),
    WIN_TITLE(TXT_ENG_3RDPARTY_VERSION_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    
    CREATE_TEXT_CTRL(MMIENG_3RDPARTY_VERSION_CTRL_ID),
                
    END_WIN
};

WINDOW_TABLE(MMIENG_SHOWNET_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngShowNetWinHandleMsg),
    WIN_ID(MMIENG_SHOWNET_WIN_ID),
    WIN_TITLE(TXT_ENG_NET_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),  
    END_WIN
};

WINDOW_TABLE(MMIENG_PSCELL_OPTION_MENU_WIN_TAB) = 
{   
    WIN_FUNC( (uint32)EngPSCellOptionMenuWinHandleMsg),
    WIN_ID(MMIENG_PSCELL_OPTION_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_PSCELL_OPTION, MMIENG_OPTION_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_PSCELL_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngPSCellWinHandleMsg),
    WIN_ID(MMIENG_PSCELL_WIN_ID),
    WIN_TITLE(TXT_ENG_PSCELL_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
//net info start
WINDOW_TABLE(MMIENG_NET_INFO_SHOW_WIN_TAB) = 
{
	WIN_FUNC((uint32)ENGNetinfoShowWinHandleMsg),
	WIN_ID(MMIENG_NET_INFO_SHOW_WIN_ID),
	WIN_TITLE(TXT_ENG_NET_INFO),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MENU_NET_INFO_SHOW, MMIENG_NET_INFO_SHOW_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_NW_CAP_SHOW_WIN_TAB) = 
{
    WIN_FUNC((uint32)ENGNWCAPShowWinHandleMsg),
    WIN_ID(MMIENG_NW_CAP_SHOW_WIN_ID),
    WIN_TITLE(TXT_ENG_NW_CAP),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_NETINFO_STATISTICS_WIN_TAB) = 
{
	WIN_FUNC((uint32)ENGNetinfoStatisticsWinHandleMsg),
	WIN_ID(MMIENG_NETINFO_STATISTICS_WIN_ID),
	WIN_TITLE(TXT_ENG_NETINFO_STATISTICS),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MENU_NETINFO_STATISTICS_ENG, MMIENG_NETINFO_STATISTICS_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_NETINFO_STATISTICS_HANDLE_WIN_TAB) = 
{
    WIN_FUNC((uint32)ENG_Netinfo_Statistics_WinHandleMsg),
    WIN_ID(MMIENG_NETINFO_STATISTICS_HANDLE_WIN_ID),
    WIN_TITLE(TXT_ENG_NETINFO_STATISTICS),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};
//net info end
WINDOW_TABLE(MMIENG_SHOWPHONE_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngShowPhoneWinHandleMsg),
    WIN_ID(MMIENG_SHOWPHONE_WIN_ID),
    WIN_TITLE(TXT_ENG_PHONE_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_SHOW_ADC_CALIBRATE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngShowAdcCalibrateWinHandleMsg),
    WIN_ID(MMIENG_SHOW_ADC_CALIBRATE_WIN_ID),
    WIN_TITLE(TXT_ENG_ADC_CALIBRATE_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_ASSERTTARGET_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngAssertTargetWinHandleMsg),
    WIN_ID(MMIENG_ASSERTTARGET_WIN_ID),
    WIN_TITLE(TXT_ENG_ASSERTTARGET),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_PHONENUM_CTRL(ENG_TARGET_MAX_LEN, MMIENG_EDITBOX1_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_FORBIDPLMN_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngForbidPLMNWinHandleMsg),
    WIN_ID(MMIENG_FORBIDPLMN_WIN_ID),
    WIN_TITLE(TXT_ENG_FORBIDPLMN),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_CARDLOG_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngCardLog_WinHandleMsg),
    WIN_ID(MMIENG_CARDLOG_WIN_ID),
    WIN_TITLE(TXT_ENG_CARDLOG),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

#ifdef NPS_SUPPORT
//[MS]NPS
WINDOW_TABLE(MMIENG_NPS_TIMER_CHANGE_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngNPSTimerChangeWinHandleMsg),
    WIN_ID(MMIENG_NPS_TIMER_CHANGE_WIN_ID),
    WIN_TITLE(TXT_ENG_NPS_TIMER_CHANGE),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,NPS_TIME_CHANGE_FORM_CTRL_ID),
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, NPS_TIME_CHANGE1_FORM_CTRL_ID,NPS_TIME_CHANGE_FORM_CTRL_ID),
	      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIENG_NPS_TIME_LABEL_CTRL_ID,NPS_TIME_CHANGE1_FORM_CTRL_ID),
	      CHILD_EDIT_TEXT_CTRL(TRUE, 4, MMIENG_NPS_TIME_EDIT1_CTRL_ID, NPS_TIME_CHANGE1_FORM_CTRL_ID),
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, NPS_TIME_CHANGE2_FORM_CTRL_ID,NPS_TIME_CHANGE_FORM_CTRL_ID),
	     CHILD_EDIT_TEXT_CTRL(TRUE, 3, MMIENG_NPS_TIME_EDIT2_CTRL_ID, NPS_TIME_CHANGE2_FORM_CTRL_ID),
	     CHILD_TEXT_CTRL(FALSE,MMIENG_NPS_TIME_TEXT_CTRL_ID,NPS_TIME_CHANGE2_FORM_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};
#endif

WINDOW_TABLE(MMIENG_IMS_NV_PARAM_ID_WIN_TAB) = 
{
    WIN_FUNC((uint32)Eng_IMS_NV_PARAM_ID_WinHandleMsg),
    WIN_ID(MMIENG_NPS_TIMER_CHANGE_WIN_ID),
    WIN_TITLE(TXT_ENG_IMS_NV_PARAM_ID),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,NPS_TIME_CHANGE_FORM_CTRL_ID),
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, NPS_TIME_CHANGE1_FORM_CTRL_ID,NPS_TIME_CHANGE_FORM_CTRL_ID),
	      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIENG_NPS_TIME_LABEL_CTRL_ID,NPS_TIME_CHANGE1_FORM_CTRL_ID),
	      CHILD_EDIT_TEXT_CTRL(TRUE, 4, MMIENG_NPS_TIME_EDIT1_CTRL_ID, NPS_TIME_CHANGE1_FORM_CTRL_ID),
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, NPS_TIME_CHANGE2_FORM_CTRL_ID,NPS_TIME_CHANGE_FORM_CTRL_ID),
	     CHILD_EDIT_TEXT_CTRL(TRUE,255, MMIENG_NPS_TIME_EDIT2_CTRL_ID, NPS_TIME_CHANGE2_FORM_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_UPDATE, TXT_READ, TXT_COMMON_COMMON_BACK),
    END_WIN
};


WINDOW_TABLE(MMIENG_UPLMN_EDIT_AND_ADD_WIN_TAB) = 
{
    WIN_FUNC((uint32)Eng_UPLMN_EDIT_AND_EDIT_WinHandleMsg),
    WIN_ID(MMIENG_UPLMN_EDIT_AND_ADD_WIN_ID),
    WIN_TITLE(TXT_ENG_IMS_NV_PARAM_ID),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,NPS_TIME_CHANGE_FORM_CTRL_ID),
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, NPS_TIME_CHANGE1_FORM_CTRL_ID,NPS_TIME_CHANGE_FORM_CTRL_ID),
	      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIENG_NPS_TIME_LABEL_CTRL_ID,NPS_TIME_CHANGE1_FORM_CTRL_ID),
	      CHILD_EDIT_TEXT_CTRL(TRUE, 3, MMIENG_NPS_TIME_EDIT1_CTRL_ID, NPS_TIME_CHANGE1_FORM_CTRL_ID),
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, NPS_TIME_CHANGE2_FORM_CTRL_ID,NPS_TIME_CHANGE_FORM_CTRL_ID),
	     CHILD_EDIT_TEXT_CTRL(TRUE,3, MMIENG_NPS_TIME_EDIT2_CTRL_ID, NPS_TIME_CHANGE2_FORM_CTRL_ID),
	     CHILD_EDIT_TEXT_CTRL(TRUE,1, MMIENG_EDITBOX2_CTRL_ID, NPS_TIME_CHANGE2_FORM_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_UPDATE, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};

//uplmn menu
WINDOW_TABLE(ENG_UPLMN_OPTION_MENU_WIN_TAB) = 
{   
    WIN_FUNC( (uint32)HandleEngUPLMNOptionMenuMsg),
    WIN_ID(MMIENG_UPLMN_OPTION_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};


WINDOW_TABLE( MMIENG_UPLMN_CHANGE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)MMIENG_HandleDispPreferNetworkListWinMsg ),    
    WIN_ID( MMIENG_UPLMN_OPTION_WIN_ID ),
    WIN_TITLE( TXT_ENG_UPLMN_CAHNGE_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIENG_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_EDIT,TXT_NULL , STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIENG_UPLMN_SET_RAT_WIN_TAB) = 
{
    WIN_FUNC((uint32)MMIENG_HandleUPLMNRATSETWinMsg),
    WIN_ID(MMIENG_UPLMN_SET_RAT_WIN_ID),
    WIN_TITLE(TXT_ENG_UPDATE_TEST_SET),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_FORM_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_AUTO_ANSWER_CALL_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngAutoAnswerCallWinHandleMsg),
    WIN_ID(MMIENG_AUTO_ANSWER_CALL_WIN_ID),
    WIN_TITLE(TXT_ENG_AUTO_ANSWER_CALL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

#ifdef MMI_SMS_RETRY_SUPPORT
WINDOW_TABLE(MMIENG_SMS_RETRY_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngSMSRetryWinHandleMsg),
    WIN_ID(MMIENG_SMS_RETRY_WIN_ID),
    WIN_TITLE(TXT_SMS_RETRY_SET),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
#endif



WINDOW_TABLE(MMIENG_NETLOST_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngNetLostWinHandleMsg),
    WIN_ID(MMIENG_NETLOST_WIN_ID),
    WIN_TITLE(TXT_ENG_NETLOST),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIENG_LABEL1_CTRL_ID),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    //CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIENG_LABEL2_CTRL_ID),
    //CREATE_TEXT_CTRL( MMIENG_TEXTBOX2_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_TCV_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngTCVWinHandleMsg),
    WIN_ID(MMIENG_TCV_WIN_ID),
    WIN_TITLE(TXT_ENG_TCV),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};
#if 0
WINDOW_TABLE(MMIENG_AFC_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngAFCWinHandleMsg),
    WIN_ID(MMIENG_AFC_WIN_ID),
    WIN_TITLE(TXT_ENG_AFC),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL( MMIENG_TEXTBOX1_CTRL_ID),
//    CREATE_PRGBOX_CTRL(GUIPRGBOX_STYLE_HORIZONTAL_PROCESS, MMIENG_PRGBOX_CTRL_ID),
    END_WIN
};
#endif
#if 0
WINDOW_TABLE(MMIENG_PA_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngPAWinHandleMsg),
    WIN_ID(MMIENG_PA_WIN_ID),
    WIN_TITLE(TXT_ENG_PA),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
//    CREATE_PRGBOX_CTRL(GUIPRGBOX_STYLE_HORIZONTAL_PROCESS, MMIENG_PRGBOX_CTRL_ID),
    CREATE_TEXT_CTRL( MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};
#endif

WINDOW_TABLE(MMIENG_AUDIOMP3_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngAudioMp3WinHandleMsg),
    WIN_ID(MMIENG_AudioMP3_WIN_ID),
    WIN_TITLE(TXT_ENG_AUDIO_SETTING),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_MP3_LISTBOX_CTRL_ID),
    /*                      
    CREATE_EDIT_DIGITAL_CTRL(ENG_CLIENT_LEFT,ENG_CLIENT_TOP+MMI_EDITNOX_IM_ICON_RECT_HEIGHT,
                            ENG_CLIENT_RIGHT,MMI_INPUT_EDITBOX_BOTTOM,  
                            4, MMIENG_WAPUAPROFILE_CTRL_ID),
    */
    END_WIN
};


WINDOW_TABLE(MMIENG_AUDIOVOL_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngAudioVolWinHandleMsg),
    WIN_ID(MMIENG_AudioVol_WIN_ID),
    WIN_TITLE(TXT_ENG_AUDIO_SETTING),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
    CREATE_PRGBOX_CTRL(GUIPRGBOX_STYLE_HORIZONTAL_PROCESS, MMIENG_PRGBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_AUDIOSET_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngAudioSetWinHandleMsg),
    WIN_ID(MMIENG_AUDIOSET_WIN_ID),
    WIN_TITLE(TXT_ENG_AUDIO_SETTING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
#if 0
WINDOW_TABLE(MMIENG_AGC_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngAGCWinHandleMsg),
    WIN_ID(MMIENG_AGC_WIN_ID),
    WIN_TITLE(TXT_ENG_AGC),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};
#endif
WINDOW_TABLE(MMIENG_ERRRATE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngErrRateWinHandleMsg),
    WIN_ID(MMIENG_ERRRATE_WIN_ID),
    WIN_TITLE(TXT_ENG_ERRRATE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};
WINDOW_TABLE(MMIENG_USBLOG_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngUSBLOGWinHandleMsg),
    WIN_ID(MMIENG_USBLOG_WIN_ID),
    WIN_TITLE(TXT_COMM_USB_LOG),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
WINDOW_TABLE(MMIENG_CAPOUPUT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngCAPOUTPUTWinHandleMsg),
    WIN_ID(MMIENG_CAPOUPUT_WIN_ID),
    WIN_TITLE(TXT_CAP_OUTPUT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
//WINDOW_TABLE(MMIENG_CONTRAST_TAB) = 
//{
//  //CLEAR_LCD,
//  //WIN_PRIO( WIN_ONE_LEVEL ),
//  WIN_FUNC((uint32)EngContrastWinHandleMsg),
//  WIN_ID(MMIENG_CONTRAST_WIN_ID),
//  WIN_TITLE(TXT_ENG_CONTRAST),
//    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
//  CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
//  CREATE_PRGBOX_CTRL(GUIPRGBOX_STYLE_HORIZONTAL_PROCESS, MMIENG_PRGBOX_CTRL_ID),
//  END_WIN
// };

WINDOW_TABLE(MMIENG_BRIGHTNESS_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngBrightnessWinHandleMsg),
    WIN_ID(MMIENG_BRIGHTNESS_WIN_ID),
    WIN_TITLE(TXT_ENG_BRIGHTNESS),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
    CREATE_PRGBOX_CTRL(GUIPRGBOX_STYLE_HORIZONTAL_PROCESS, MMIENG_PRGBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_SIDETONE_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngSidetoneWinHandleMsg),
    WIN_ID(MMIENG_SIDETONE_WIN_ID),
    WIN_TITLE(TXT_ENG_HDSTSDTONE),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_SIDETONE_FORM_CTRL_ID), 
    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_EDITBOX1_CTRL_ID, MMIENG_SIDETONE_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN,MMIENG_SIDETONE_SOFTKEY_CTRL_ID,MMIENG_SIDETONE_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(5, MMIENG_EDITBOX1_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE(MMIENG_VOICEPARAM_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngVoiceParamWinHandleMsg),
    WIN_ID(MMIENG_VOICEPARAM_WIN_ID),
    WIN_TITLE(TXT_ENG_VOICE_PARAM),
    
   
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_FORM_CTRL_ID), 
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_FORM1_CTRL_ID,MMIENG_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL1_CTRL_ID, MMIENG_FORM1_CTRL_ID),
            CHILD_EDIT_DIGITAL_CTRL(TRUE, 5,MMIENG_EDITBOX1_CTRL_ID, MMIENG_FORM1_CTRL_ID),  

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_FORM2_CTRL_ID,MMIENG_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL2_CTRL_ID, MMIENG_FORM2_CTRL_ID),
            CHILD_EDIT_DIGITAL_CTRL(TRUE, 5,MMIENG_EDITBOX2_CTRL_ID, MMIENG_FORM2_CTRL_ID),  
#ifdef MMI_PDA_SUPPORT

       CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN,MMIENG_VOICE_PARAM_SOFTKEY_CTRL_ID,MMIENG_FORM_CTRL_ID),
#else
       WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif

    END_WIN
};

WINDOW_TABLE(MMIENG_PRODUCT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngProductWinHandleMsg),
    WIN_ID(MMIENG_PRODUCT_WIN_ID),
    WIN_TITLE(TXT_ENG_PRODUCT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_PRODUCT_SN_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngProductSNWinHandleMsg),
    WIN_ID(MMIENG_PRODUCT_WIN_ID),
    WIN_TITLE(TXT_ENG_PRODUCT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_POWERKEY_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngPowerKeyWinHandleMsg),
    WIN_ID(MMIENG_POWERKEY_WIN_ID),
    WIN_TITLE(TXT_ENG_POWER_KEY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
#ifdef DSP_USB_LOG
/*xuefangjiang 20110729*/
WINDOW_TABLE(MMIENG_DSPCARD_LOG_ENABLE_TAB) = 
{
    WIN_FUNC((uint32)EngDspCardLogEnableWinHandleMsg),
    WIN_ID(MMIENG_DSPCARD_LOG_ENABLE_WIN_ID),
    WIN_TITLE(TXT_ENG_DSPCARD_LOG_ENABLE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
#endif
WINDOW_TABLE(MMIENG_SENSOR_MODE_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngSensorModeWinHandleMsg),
    WIN_ID(MMIENG_SENSOR_MODE_WIN_ID),
    WIN_TITLE(TXT_ENG_SENSOR_MODE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

 //MS00216980 cheney
WINDOW_TABLE(MMIENG_CMCC_TEST_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngCMCCTestWinHandleMsg),
	WIN_ID(MMIENG_CMCC_TEST_WIN_ID),
	WIN_TITLE(TXT_ENG_CMCC_TEST_CONFIG),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_GET_FLASH_INFO_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngGetFlashInfoWinHandleMsg),
	WIN_ID(MMIENG_GET_FLASH_INFO_WIN_ID),
	WIN_TITLE(TXT_ENG_GET_FLASH_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_UPDATE_IMEI_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngUpdateIMEIWinHandleMsg),
	WIN_ID(MMIENG_UPDATE_IMEI_WIN_ID),
	WIN_TITLE(TXT_ENG_UPDATE_IMEI),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIENG_LISTBOX_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_UPDATE_IMEI_EDIT_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
	WIN_FUNC((uint32)EngUpdateIMEIEditWinHandleMsg),
	WIN_ID(MMIENG_UPDATE_IMEI_EIDT_WIN_ID),
	WIN_TITLE(TXT_ENG_UPDATE_IMEI),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MN_MAX_IMEI_LENGTH*2 -1, MMIENG_EDITBOX1_CTRL_ID),
	END_WIN
};


WINDOW_TABLE(MMIENG_DV_FRAME_FRE_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngDVFrameFreWinHandleMsg),
    WIN_ID(MMIENG_DV_FRAME_FRE_WIN_ID),
    WIN_TITLE(TXT_ENG_DV_FRAME_FRE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_BANDSELECT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngBandSelectWinHandleMsg),
    WIN_ID(MMIENG_BANDSELECT_WIN_ID),
    WIN_TITLE(TXT_ENG_BAND),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_LAYER1MONITOR_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngShowLayer1MonitorWinHandleMsg),
    WIN_ID(MMIENG_LAYER1MONITOR_WIN_ID),
    WIN_TITLE(TXT_ENG_LAYER1_MONITOR),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),

    END_WIN
};

/*[START] Jerry Liang for ENG MODE powersweep 2008/07/30*/
/*WINDOW_TABLE(MMIENG_POWERSWEEPINFO_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngShowPowersweepInfoWinHandleMsg),
    WIN_ID(MMIENG_POWERSWEEPINFO_WIN_ID),
    WIN_TITLE(TXT_ENG_POWERSWEEP_INFO),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),

    END_WIN
};*/// modify by mary
/*[END] Jerry Liang for ENG MODE powersweep 2008/07/30*/


WINDOW_TABLE(MMIENG_SHOW_RF_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngShowRFWinHandleMsg),
    WIN_ID(MMIENG_SHOW_RF_WIN_ID),
        WIN_TITLE(TXT_ENG_RF_PARAM),
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
        CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};

#if 0
WINDOW_TABLE(MMIENG_WAPUA_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngWapUAWinHandleMsg),
    WIN_ID(MMIENG_WAPUA_WIN_ID),
    WIN_TITLE(TXT_ENG_UA),
    CREATE_EDIT_TEXT_CTRL(ENG_WAPUA_MAX_LEN,MMIENG_WAPUA_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIENG_WAPUAPROFILE_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngWapUAProfileWinHandleMsg),
    WIN_ID(MMIENG_WAPUAPROFILE_WIN_ID),
    WIN_TITLE(TXT_ENG_UA_PROFILE),
    CREATE_EDIT_TEXT_CTRL(ENG_WAPUA_MAX_LEN,MMIENG_WAPUAPROFILE_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIENG_WAPMMS_SAS_TEST_MODE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngWapMmsSasTestModeWinHandleMsg),
    WIN_ID(MMIENG_WAPMMS_SAS_TEST_MODE_WIN_ID),
    WIN_TITLE(TXT_ENG_SAS_TEST_MODE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
#endif

WINDOW_TABLE(MMIENG_BT_SSP_DEBUG_TAB) =
{
    WIN_FUNC((uint32)BTSSPDebugModeWinHandleMsg),
	WIN_ID(MMIENG_BT_SSP_DEBUG_WIN_ID),
	WIN_TITLE(TXT_ENG_BT_SSP_DEBUG),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
	END_WIN
};
WINDOW_TABLE(MMIENG_BT_BQB_MODE_TAB) =
{
    WIN_FUNC((uint32)BTBQBModeWinHandleMsg),
		WIN_ID(MMIENG_BT_BQB_MODE_WIN_ID),
		WIN_TITLE(TXT_ENG_BT_BQB_MODE),
		WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
		CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
		END_WIN
};
WINDOW_TABLE(MMIENG_BT_ADDRESS_INFO_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)BtAddressInfoWinHandleMsg),
    WIN_ID(MMIENG_BT_ADDRESS_INFO_WIN_ID),
    WIN_TITLE(TXT_EMG_BT_ADDR_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL( MMIENG_BTADDRESS_CTRL_ID),
    END_WIN
};

#ifndef WIN32
#ifdef  BLUETOOTH_SUPPORT_SPRD_BT
WINDOW_TABLE(MMIENG_BT_VER_INFO_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)BtVerInfoWinHandleMsg),
    WIN_ID(MMIENG_BT_VER_INFO_WIN_ID),
    WIN_TITLE(TXT_ENG_VERSION_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL( MMIENG_BTVER_CTRL_ID),
    END_WIN
};
#endif
#endif
#ifdef GPS_SUPPORT
WINDOW_TABLE(MMIENG_GPS_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngShowGPSWinHandleMsg),
    WIN_ID(MMIENG_SHOW_GPS_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};
#endif
WINDOW_TABLE(MMIENG_CHIP_TEST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleChipTestMenuWinMsg ),    
    WIN_ID( MMIENG_CHIP_TEST_MENU_WIN_ID ),
    WIN_TITLE(TXT_ENG_CHIP_TEST),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_CHIP_TEST, MMIENG_CHIP_TEST_MENU_CTRL_ID),
    END_WIN
};


#ifdef CMMB_SUPPORT

//工厂测试、研发调试
WINDOW_TABLE(MMIENG_CMMB_TEST_MAIN_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCMMBTestMainMenuWinMsg ),    
        WIN_ID( MMIENG_CMMB_TEST_MENU_WIN_ID ),
        WIN_TITLE(TXT_ENG_CMMB_TEST),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_MENU_CTRL(MENU_CMMB_MAIN, MMIENG_CMMB_TEST_MENU_CTRL_ID),
        END_WIN
};

//研发调试：IQ数据、Display
WINDOW_TABLE(MMIENG_CMMB_RD_TEST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCMMBRDTestMenuWinMsg ),    
        WIN_ID( MMIENG_CMMB_RD_TEST_MENU_WIN_ID ),
        WIN_TITLE(TXT_ENG_CMMB_RD_TEST),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_MENU_CTRL(MENU_CMMB_RD_TEST, MMIENG_CMMB_RD_TEST_MENU_CTRL_ID),
        END_WIN
};

//工厂测试：有信号测试、无信号测试
WINDOW_TABLE(MMIENG_CMMB_FACTORY_TEST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCMMBFactoryTestMenuWinMsg ),    
        WIN_ID( MMIENG_CMMB_FACTORY_TEST_MENU_WIN_ID ),
        WIN_TITLE(TXT_ENG_CMMB_FACTORY_TEST),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_MENU_CTRL(MENU_CMMB_FACTORY_TEST, MMIENG_CMMB_FACTORY_TEST_MENU_CTRL_ID),
        END_WIN
};

//有信号测试
WINDOW_TABLE(MMIENG_CMMB_OK_SIGNAL_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCMMBTestOKSignalMenuWinMsg ),    
        WIN_ID( MMIENG_CMMB_HAVE_SIGNAL_MENU_WIN_ID ),
        WIN_TITLE(TXT_ENG_CMMB_FACTORY_HAVE_SIGNAL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_MENU_CTRL(MENU_CMMB_OK_SIGNAL, MMIENG_CMMB_FACTORY_HAVE_SIGNAL_MENU_CTRL_ID),
        END_WIN
};

//无信号测试: RF测试、解码测试
WINDOW_TABLE(MMIENG_CMMB_NO_SIGNAL_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCMMBTestNoSignalMenuWinMsg ),    
        WIN_ID( MMIENG_CMMB_NO_SIGNAL_MENU_WIN_ID ),
        WIN_TITLE(TXT_ENG_CMMB_FACTORY_NO_SIGNAL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_MENU_CTRL(MENU_CMMB_NO_SIGNAL, MMIENG_CMMB_FACTORY_NO_SIGNAL_MENU_CTRL_ID),
        END_WIN
};

WINDOW_TABLE(MMIENG_CMMB_SN_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCMMBSerialNumberWinMsg ),    
    WIN_ID( MMIENG_CMMB_SN_WIN_ID ),
    WIN_TITLE(TXT_ENG_CMMB_SN_NO),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_CMMB_SN_TEXT_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_CMMB_DISPLAY_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCMMBDisplayWinMsg ),    
    WIN_ID( MMIENG_CMMB_DISPLAY_WIN_ID ),
    WIN_TITLE(TXT_ENG_CMMB_DISPLAY),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_CMMB_TEXT_CTRL_ID),
    END_WIN
};

#ifdef MBBMS_SUPPORT

#ifdef DEMOD_HW_SIANO
WINDOW_TABLE(MMIENG_LOG_CONTROL_ENABLE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleLOGControlEnableWinMsg),
    WIN_ID(MMIENG_SIANO_LOG_CONTROL_WIN_ID),
    WIN_TITLE(TXT_ENG_MBBMS_MSK),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LOG_ENABLE_CONTROL_CTRL_ID),
    END_WIN
};
#endif
WINDOW_TABLE(MMIENG_INTERFACE_TEST_ENABLE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleInterfaceTestEnableWinMsg),
    WIN_ID(MMIENG_GBA_AND_MSK_ENABLE_WIN_ID),
    WIN_TITLE(TXT_ENG_MBBMS_INTERFACE_TEST),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_GBA_AND_MSK_TEST_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_SMD_COUNTER_ENABLE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSMDCounterEnableWinMsg),
    WIN_ID(MMIENG_SMD_COUNTER_ENABLE_WIN_ID),
    WIN_TITLE(TXT_ENG_MBBMS_SMD_COUNTER),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_SMD_COUNTER_ENABLE_CTRL_ID),
    END_WIN
};
#endif
WINDOW_TABLE(MMIENG_AUDIO_SERVICE_ENABLE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleAudioServiceEnableWinMsg),
    WIN_ID(MMIENG_AUDIO_SERVICE_ENABLE_WIN_ID),
    WIN_TITLE(TXT_ENG_MBBMS_AUDIO_SERVICE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_AUDIO_SERVICE_ENABLE_CTRL_ID),
    END_WIN
};
WINDOW_TABLE(MMIENG_MTV_LOG_SWITCH_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngMtvLOGSwitchWinHandleMsg),
    WIN_ID(MMIENG_MTV_LOG_SWITCH_WIN_ID),
    WIN_TITLE(TXT_ENG_MTV_LOG_SWITCH),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
WINDOW_TABLE(MMIENG_MTV_LOG_TO_BB_SWITCH_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngMtvLOGToBBSwitchWinHandleMsg),
    WIN_ID(MMIENG_MTV_LOG_TO_BB_SWITCH_WIN_ID),
    WIN_TITLE(TXT_ENG_MTV_LOG_TO_BB_SWITCH),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
WINDOW_TABLE(MMIENG_MTV_LOG_STORAGE_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngMtvLOGStorageWinHandleMsg),
    WIN_ID(MMIENG_MTV_LOG_STORAGE_WIN_ID),
    WIN_TITLE(TXT_ENG_MTV_LOG_STORAGE_SELECT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

#endif

#ifdef TF_LOAD_SUPPORT
WINDOW_TABLE(MMIENG_TFLOAD_WIN_TAB) = 
{
    //CLEAR_LCD,
       //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleTFLoadWinMsg),
    WIN_ID( MMIENG_TFLOAD_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_TITLE(TXT_ENG_TF_UPDATE),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

WINDOW_TABLE(MMIENG_CHIP_TEST_SETTING_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleChipTestSettingWinMsg ),    
    WIN_ID( MMIENG_CHIP_TEST_SETTING_WIN_ID ),
    WIN_TITLE(TXT_ENG_POWERON_RUN),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_CHIP_TEST_SETTING_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_CHIP_TEST_PLAYMP3MP4_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleChipTestPlayMp3MP4WinMsg),    
    WIN_ID( MMIENG_CHIP_TEST_PLAYMP3MP4_WIN_ID ),
    WIN_TITLE(TXT_ENG_CHIP_TEST),
    //WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIENG_CHIP_TEST_DC_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleChipTestDCWinMsg),    
    WIN_ID( MMIENG_CHIP_TEST_DCPREVIEW_WIN_ID ),
    WIN_TITLE(TXT_ENG_CHIP_TEST),
    //WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIENG_CHIP_TEST_LOOPBACK_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleChipTestLoopBackWinMsg),
    WIN_ID(MMIENG_UITESTLOOPBACK_WIN_ID),
    WIN_TITLE(TXT_ENG_UILOOPBACK_TITLE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_IQ_MODE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngIQModeWinHandleMsg),
    WIN_ID(MMIENG_IQ_MODE_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_MODE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_IQ_MODE, MMIENG_IQ_MODE_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_IQ_MODE_ONOFF_WIN_TAB) = //开关
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngIQModeOnOffWinHandleMsg),
    WIN_ID(MMIENG_IQ_MODE_ONOFF_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_MODE_ONOFF),//TXT_ENG_IQ_MODE_ONOFF
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_IQ_DATA_ENABLE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleIQDataWinMsg),    
    WIN_ID(MMIENG_IQ_DATA_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_DATA),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_IQ_DATA_ENABLE, MMIENG_IQ_DATA_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_IQ_DATA_DISABLE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleIQDataWinMsg),    
    WIN_ID(MMIENG_IQ_DATA_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_DATA),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_IQ_DATA_DISABLE, MMIENG_IQ_DATA_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_IQ_DATA_MODE_SELECT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngIQDataModeSelectWinHandleMsg),
    WIN_ID(MMIENG_IQ_DATA_MODE_SELECT_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_DATA_MODE_SETTINGS),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_IQ_DATA_POWER_ON_START_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngIQDataPowerOnStartWinHandleMsg),
    WIN_ID(MMIENG_IQ_DATA_POWER_ON_START_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_DATA_POWER_ON_START),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_IQ_DATA_MODE_SETTING_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleIQDataModeSettingWinMsg),    
    WIN_ID(MMIENG_IQ_DATA_MODE_SETTING_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_DATA_MODE_SETTINGS),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_IQ_DATA_MODE_SETTING, MMIENG_IQ_DATA_MODE_SETTING_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_IQ_DATA_MODE_SETTING_FORMAT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleIQDataModeSettingFormatWinMsg),    
    WIN_ID( MMIENG_IQ_DATA_MODE_SETTING_FORMAT_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_DATA_FORMAT_SETTING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(2, MMIENG_IQ_DATA_MODE_SETTING_FORMAT_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_IQ_DATA_MODE_SETTING_TIME_LIMIT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleIQDataModeSettingTimeLimitWinMsg),    
    WIN_ID( MMIENG_IQ_DATA_MODE_SETTING_TIME_LIMIT_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_DATA_TIME_LIMIT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(4, MMIENG_IQ_DATA_MODE_SETTING_TIME_LIMIT_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_IQ_DATA_MODE_SETTING_FRAME_LENGTH_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleIQDataModeSettingFrameLengthWinMsg),    
    WIN_ID( MMIENG_IQ_DATA_MODE_SETTING_FRAME_LENGTH_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_DATA_FRAME_LENGTH),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(4, MMIENG_IQ_DATA_MODE_SETTING_FRAME_LENGTH_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_IQ_DATA_MODE_SETTING_BLOCK_COUNT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleIQDataModeSettingBlockCountWinMsg),    
    WIN_ID( MMIENG_IQ_DATA_MODE_SETTING_BLOCK_COUNT_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_DATA_BLOCK_COUNT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(2, MMIENG_IQ_DATA_MODE_SETTING_BLOCK_COUNT_CTRL_ID),
    END_WIN
};

/*[START] Jerry Liang for IQ MODE 2008/07/31*/        
WINDOW_TABLE(MMIENG_IQ_DATA_MODE_SETTING_ARFCN_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleIQDataModeSettingARFCNWinMsg),    
    WIN_ID( MMIENG_IQ_DATA_MODE_SETTING_ARFCN_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_DATA_ARFCN),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(4, MMIENG_IQ_DATA_MODE_SETTING_ARFCN_CTRL_ID),
    END_WIN
};
/*[END] Jerry Liang for IQ MODE 2008/07/31*/        

WINDOW_TABLE(MMIENG_IQ_DATA_MODE_SETTING_CHANNEL_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleIQDataModeSettingChannelWinMsg),
    WIN_ID(MMIENG_IQ_DATA_MODE_SETTING_CHANNEL_WIN_ID),
    WIN_TITLE(TXT_ENG_IQ_DATA_CHANNEL_SETTING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_IQ_DATA_MODE_SETTING_CHANNEL_CTRL_ID),
    END_WIN
};

//the window for handle query
WINDOW_TABLE(MMIENG_HANDLE_QUERY_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleIQDataQueryWin),    
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_ID(MMIENG_IQ_DATA_HANDLE_QUERY_WIN_ID),
    END_WIN
};

#ifdef  CAMERA_SUPPORT
WINDOW_TABLE(MMIENG_DC_PARAM_SCREEN_SIZE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleDCParamScreenSizeWinMsg),
    WIN_ID(MMIENG_DCPARAM_SCREENSIZE_WIN_ID),
    WIN_TITLE(TXT_ENG_DC_PARAM_SCREEN_SIZE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_DC_PARAM_SCREEN_SWITCH_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleDCParamScreenSwitchWinMsg),
    WIN_ID(MMIENG_DCPARAM_SCREENSWITCH_WIN_ID),
    WIN_TITLE(TXT_ENG_DC_PARAM_SCREEN_SWITCH),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_DC_PARAM_PREV_SET_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleDCParamScreenPrevSetWinMsg),
    WIN_ID(MMIENG_DCPARAM_PREVSET_WIN_ID),
    WIN_TITLE(TXT_ENG_DC_PARAM_PREV_SET),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_DC_PARAM_SENSOR_ANGLE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleDCParamSensorAngleWinMsg),
    WIN_ID(MMIENG_DCPARAM_SENSOR_ANGLE_WIN_ID),
    WIN_TITLE(TXT_ENG_DC_PARAM_SENSOR_ANGLE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_DC_PARAM_SENSOR_ID_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDCParamSensorIDWinMsg),
    WIN_ID(MMIENG_DCPARAM_SENSOR_ID_WIN_ID),
    WIN_TITLE(TXT_SWITCH_SENSOR),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
#endif

WINDOW_TABLE(MMIENG_UA_AGENT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleUaAgentWinMsg),
    WIN_ID(MMIENG_UA_AGENT_WIN_ID),
    WIN_TITLE(TXT_ENG_UA_AGENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_UA_SUB, MMIENG_UA_SUB_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_UA_AGENT_SET_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleUaAgentSetWinMsg),
    WIN_ID(MMIENG_UA_AGENT_SET_WIN_ID),
    WIN_TITLE(TXT_ENG_UA_AGENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_UA_SELECT_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_UA_AGENT_EDIT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleUaAgentEditWinMsg),
    WIN_ID(MMIENG_UA_AGENT_EDIT_WIN_ID),
    WIN_TITLE(TXT_ENG_UA_AGENT),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIENG_MAX_URL_LEN, MMIENG_UAAGENT_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_UA_AGENT_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleUaAgentListWinMsg),
    WIN_ID(MMIENG_UA_AGENT_LIST_WIN_ID),
    WIN_TITLE(TXT_ENG_UA_AGENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_UA_SELECT_LIST_NUM_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_UA_AGENT_SHOW_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleUaAgentShowWinMsg),
    WIN_ID(MMIENG_UA_AGENT_SHOW_WIN_ID),
    WIN_TITLE(TXT_ENG_UA_AGENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),  
    END_WIN
};

WINDOW_TABLE(MMIENG_UA_PROFILE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleUaProfileWinMsg),
    WIN_ID(MMIENG_UA_PROFILE_WIN_ID),
    WIN_TITLE(TXT_ENG_UA_PROFILE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_UA_SUB, MMIENG_UA_SUB_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_UA_PROFILE_SET_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleUaProfileSetWinMsg),
    WIN_ID(MMIENG_UA_PROFILE_SET_WIN_ID),
    WIN_TITLE(TXT_ENG_UA_PROFILE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_PROFILE_SELECT_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_UA_PROFILE_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleUaProfileListWinMsg),
    WIN_ID(MMIENG_UA_PROFILE_LIST_WIN_ID),
    WIN_TITLE(TXT_ENG_UA_PROFILE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_PROFILE_SELECT_LIST_NUM_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_UA_PROFILE_SHOW_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleUaProfileShowWinMsg),
    WIN_ID(MMIENG_UA_PROFILE_SHOW_WIN_ID),
    WIN_TITLE(TXT_ENG_UA_PROFILE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),  
    END_WIN
};


WINDOW_TABLE(MMIENG_UA_PROFILE_EDIT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleUaProfileEditWinMsg),
    WIN_ID(MMIENG_UA_PROFILE_EDIT_WIN_ID),
    WIN_TITLE(TXT_ENG_UA_PROFILE),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIENG_MAX_URL_LEN, MMIENG_UAPROFILE_CTRL_ID),
    END_WIN
};

#ifdef PCLINK_SUPPORT
WINDOW_TABLE(MMIENG_PCLINK_ENABLE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandlePClinkEnableWinMsg),
    WIN_ID(MMIENG_PCLINK_ENABLE_WIN_ID),
    WIN_TITLE(TXT_ENG_PCLINK_ENABLE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_PCLINK_ENABLE_CTRL_ID),
    END_WIN
};

//the window for input gateway
WINDOW_TABLE(MMIENG_PCLINK_INPUT_IPADDR_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandlePClinkInputIPAddrWinMsg),    
    WIN_ID(MMIENG_PCLINK_INPUT_IPADDR_WIN_ID),
    WIN_TITLE(TXT_ENG_PCLINK_IP),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //参数为测试用
    CREATE_EDIT_IP_CTRL(MMIENG_PCLINK_INPUT_IPADDR_CTRL_ID),
    END_WIN
};

//the window for input gateway
WINDOW_TABLE(MMIENG_PCLINK_INPUT_SNMASK_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandlePClinkInputSNMaskWinMsg),    
    WIN_ID(MMIENG_PCLINK_INPUT_SNMASK_WIN_ID),
    WIN_TITLE(TXT_ENG_PCLINK_SNMASK),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //参数为测试用
    CREATE_EDIT_IP_CTRL(MMIENG_PCLINK_INPUT_SNMASK_CTRL_ID),
    END_WIN
};

//the window for input gateway
WINDOW_TABLE(MMIENG_PCLINK_INPUT_DEFGW_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandlePClinkInputDefGWWinMsg),    
    WIN_ID(MMIENG_PCLINK_INPUT_DEFGW_WIN_ID),
    WIN_TITLE(TXT_ENG_PCLINK_DEFGW),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //参数为测试用
    CREATE_EDIT_IP_CTRL(MMIENG_PCLINK_INPUT_DEFGW_CTRL_ID),
    END_WIN
};

//the window for input gateway
WINDOW_TABLE(MMIENG_PCLINK_INPUT_DNS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandlePClinkInputDNSWinMsg),    
    WIN_ID(MMIENG_PCLINK_INPUT_DNS_WIN_ID),
    WIN_TITLE(TXT_ENG_PCLINK_DNS),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //参数为测试用
    CREATE_EDIT_IP_CTRL(MMIENG_PCLINK_INPUT_DNS_CTRL_ID),
    END_WIN
};

//the window for input test address
WINDOW_TABLE(MMIENG_PCLINK_INPUT_TEST_ADDRESS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandlePCLinkInputTestAdfressWinMsg),    
    WIN_ID(MMIENG_PCLINK_INPUTUSERNAME_WIN_ID),
    WIN_TITLE(TXT_ENG_PCLINK_TEST_ADDR),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //参数为测试用
    CREATE_EDIT_IP_CTRL(MMIENG_PCLINK_INPUT_TEST_ADDRESS_CTRL_ID),
    END_WIN
};
#endif

//MS00186848
WINDOW_TABLE(MMIENG_CALL_BARRING_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngCallBarringWinHandleMsg),
    WIN_ID(MMIENG_CALL_BARRING_WIN_ID),
    WIN_TITLE(TXT_ENG_CALL_BARRING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_SET_ANGLE_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngSetAngleWinHandleMsg),
    WIN_ID(MMIENG_SET_ANGLE_WIN_ID),
    WIN_TITLE(TXT_ENG_SET_ANGLE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_NV_COUNTING_WIN_TAB) = 
{
        //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32)HandleRunningNVCountingWinMsg),    
        WIN_ID(MMIENG_NV_COUNTING_WIN_ID),
              WIN_TITLE(TXT_ENG_NVCOUNT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),      
        CREATE_TEXT_CTRL( MMIENG_NV_COUNTING_TEXTBOX_CTRL_ID), 
        END_WIN
};

WINDOW_TABLE(MMIENG_BROWSER_LOG_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleBrowserLogWinMsg),
    WIN_ID(MMIENG_BROWSER_LOG_WIN_ID),
    WIN_TITLE(TXT_ENG_BROWSER_LOG),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};


WINDOW_TABLE(MMIENG_BROWSER_CHANGE_URL_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleBrowserChangeURLWinMsg),
        WIN_ID(MMIENG_BROWSER_LOG_WIN_ID),
        WIN_TITLE(TXT_ENG_BROWSER_LOG),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
        END_WIN
};

WINDOW_TABLE(MMIENG_SCREEN_LOG_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleScreenLogWinMsg),
    WIN_ID(MMIENG_SCREEN_LOG_WIN_ID),
    WIN_TITLE(TXT_ENG_SCREEN_LOG),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};


WINDOW_TABLE(MMIENG_CHIPTEST_TP_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleChiptestTpWinMsg),
    WIN_ID(MMIENG_CHIPTEST_TP_WIN_ID),    
    END_WIN
};

#ifdef MMI_AUTOTEST_SUPPORT
WINDOW_TABLE(MMIENG_AUTOTEST_SET_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleAutoTestSetWinMsg),
    WIN_ID(MMIENG_AUTO_TEST_SET_WIN_ID),
    WIN_TITLE(TXT_ENG_AUTO_TEST_SET),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
#endif

#ifdef WIFI_SUPPORT
WINDOW_TABLE(MMIENG_WLAN_PERFORMANCE_MODE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleWlanPerformanceModeWinMsg),
    WIN_ID(MMIENG_WLAN_PERFORMANCE_MODE_WIN_ID),
    WIN_TITLE(TXT_ENG_WLAN_PERFORMANCE_MODE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
WINDOW_TABLE(MMIENG_WIFI_IPERF_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleWifiIperfWinMsg),
    WIN_ID(MMIENG_WIFI_IPERF_WIN_ID),
    WIN_TITLE(TXT_ENG_WLAN_IPERF),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_FORM_CTRL_ID), 
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,  MMIENG_LABEL1_CTRL_ID, MMIENG_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, 256,MMIENG_EDITBOX1_CTRL_ID, MMIENG_FORM_CTRL_ID),  
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,  MMIENG_LABEL2_CTRL_ID, MMIENG_FORM_CTRL_ID),
    CHILD_TEXT_CTRL(TRUE,MMIENG_TEXTBOX1_CTRL_ID, MMIENG_FORM_CTRL_ID),  
#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID, MMIENG_FORM_CTRL_ID),  
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

WINDOW_TABLE(MMIENG_WLAN_RADIO_FREQUENCY_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWlanRfSetWinMsg),    
    WIN_ID(MMIENG_WLAN_RADIO_FREQUENCY_WIN_ID),
    WIN_TITLE(TXT_ENG_WLAN_RADIO_FREQUENCY),
    WIN_SOFTKEY(STXT_SOFTKEY_SELECT_MK, TXT_NULL, STXT_FINISH),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_RADIO_FREQUENCY_FORM_CTRL_ID),
    
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_WLAN_POWER_SAVE_FORM_CTRL_ID,MMIENG_RADIO_FREQUENCY_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_WLAN_POWER_SAVE_LABEL_CTRL_ID, MMIENG_WLAN_POWER_SAVE_FORM_CTRL_ID),//主菜单风格
        CHILD_SETLIST_CTRL(TRUE, MMIENG_WLAN_POWER_SAVE_SETLIST_CTRL_ID, MMIENG_WLAN_POWER_SAVE_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_WLAN_ROAMING_FORM_CTRL_ID,MMIENG_RADIO_FREQUENCY_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIENG_WLAN_ROAMING_LABEL_CTRL_ID,MMIENG_WLAN_ROAMING_FORM_CTRL_ID),//待机风格
        CHILD_SETLIST_CTRL(TRUE, MMIENG_WLAN_ROAMING_SETLIST_CTRL_ID, MMIENG_WLAN_ROAMING_FORM_CTRL_ID),

    END_WIN
};


#endif

// for SUPPORT_STANDBY_RECORD
WINDOW_TABLE(MMIENG_STANDBY_TIME_INFO_TAB) =
{
    WIN_FUNC((uint32)StandbyTimeInfoWinHandleMsg),
    WIN_ID(MMIENG_STANDBY_TIME_INFO_WIN_ID),
    WIN_TITLE(TXT_ENG_SHOW_STANDBY_TIME),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};

#ifdef WRE_SUPPORT
WINDOW_TABLE(MMIENG_SHOW_WREVERSION_WIN_TAB) = 
{
    WIN_FUNC((uint32)EngShowWREVersionWinHandleMsg),
    WIN_ID(MMIENG_SHOW_WRE_VERSION_WIN_ID),
        WIN_TITLE(TXT_ENG_WRE_VERSION_INFO),
#if defined MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif	
        WIN_SOFTKEY(TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN),
        CREATE_TEXT_CTRL(MMIENG_SHOW_WRE_VERSION_CTRL_ID),
    END_WIN
};
#endif
//end for SUPPORT_STANDBY_RECORD

#if MMISRV_AUDIO_TEST_SUPPORT
WINDOW_TABLE(MMIENG_AUDIO_SELF_TEST_TAB) =
{
    WIN_FUNC((uint32)EngAudioSelfTestHandleMsg),
    WIN_ID(MMIENG_AUDIO_SELF_TEST),
    WIN_TITLE(TXT_ENG_AUDIO_SELF_TEST),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
WINDOW_TABLE(MMIENG_AUDIO_SELF_TEST_PLAY_TAB) =
{
    WIN_FUNC((uint32)EngAudioSelfTestPlayHandleMsg),
    WIN_ID(MMIENG_AUDIO_SELF_TEST_PLAY),
    //WIN_TITLE(TXT_ENG_AUDIO_PLAY),
    //WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    END_WIN
};         
#ifdef PRINTSCREEN_SUPPORT
WINDOW_TABLE(MMIENG_AUDIO_SELF_TEST_DISPLAY_INFO_TAB) =
{
    WIN_FUNC((uint32)EngAudioSelfTestDisplayInfoHandleMsg),
    WIN_ID(MMIENG_AUDIO_SELF_TEST_DISPLAY_INFO),
    WIN_TITLE(TXT_ENG_AUDIO_DISPLAY_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
    END_WIN
};         
#endif
#endif

WINDOW_TABLE(MMIENG_DTMF_SETTIMER_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngDTMFSetTimerWinHandleMsg),
    WIN_ID(MMIENG_DTMF_SETTIMER_WIN_ID),
    WIN_TITLE(TXT_DTMF_SET_TIMER),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(4, MMIENG_DTMF_EDIT_CTRL_ID),
    END_WIN
};

#ifdef ENG_SECONDEDAC_SUPPORT
WINDOW_TABLE(MMIENG_SECONDEDAC_ENABLE_TAB) = 
{
    WIN_FUNC((uint32)EngSECONDEDACEnableWinHandleMsg),
    WIN_ID(MMIENG_SECONDEDAC_ENABLE_WIN_ID),
    WIN_TITLE(TXT_ENG_SECONDEDAC_ENABLE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
#endif
#if 0
//Add Contact for test 
WINDOW_TABLE(MMIENG_ADD_CONTACT_WIN_TAB) = 
{
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_ENG_ADD_CONTACT),
    WIN_FUNC((uint32) EngAddContactHandleMsg),
    WIN_ID( MMIENG_ADD_CONTACT_WIN_ID),
    CREATE_FORM_CTRL( GUIFORM_LAYOUT_ORDER, MMIENG_FORM_CTRL_ID ),
    //name
    CHILD_FORM_CTRL( TRUE, GUIFORM_LAYOUT_SBS, MMIENG_FORM_NAME_ID, MMIENG_FORM_CTRL_ID ),
        CHILD_LABEL_CTRL( GUILABEL_ALIGN_LEFT, FALSE, MMIENG_COMMON_LABEL_CTRL1_ID, MMIENG_FORM_NAME_ID ),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIENG_COMMON_LIST_CTRL1_ID, MMIENG_FORM_NAME_ID ),
    //head icon
    CHILD_FORM_CTRL( TRUE, GUIFORM_LAYOUT_SBS, MMIENG_FORM_ICON_ID,MMIENG_FORM_CTRL_ID ),
        CHILD_LABEL_CTRL( GUILABEL_ALIGN_LEFT, FALSE, MMIENG_COMMON_LABEL_CTRL2_ID, MMIENG_FORM_ICON_ID ),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIENG_DROP_LIST_ICON_ID, MMIENG_FORM_ICON_ID ),
    //phone number
    CHILD_FORM_CTRL( TRUE, GUIFORM_LAYOUT_SBS, MMIENG_FORM_PHONENUM_ID,MMIENG_FORM_CTRL_ID ),
        CHILD_LABEL_CTRL( GUILABEL_ALIGN_LEFT, FALSE, MMIENG_COMMON_LABEL_CTRL3_ID, MMIENG_FORM_PHONENUM_ID ),
        CHILD_EDIT_PHONENUM_CTRL(TRUE, 20,MMIENG_COMMON_EDIT_CTRL3_ID, MMIENG_FORM_PHONENUM_ID ),

    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_CANCEL),
    END_WIN
};
/*telephony end*/
#endif

#ifdef SFR_SUPPORT_CUCC
WINDOW_TABLE(MMIENG_CUCC_DM_SET_CLY_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngCUCCDMSetClyWinHandleMsg),
    WIN_ID(MMIENG_CUCC_DM_SET_CLY_WIN_ID),
    WIN_TITLE(TXT_CUCC_DM_SET_REG_PER),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_CUCC_DM_SET_MODE_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngCUCCDMSetModeWinHandleMsg),
    WIN_ID(MMIENG_CUCC_DM_SET_MODE_WIN_ID),
    WIN_TITLE(TXT_CUCC_DM_SET_REG_MODE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};
#endif

//Memory Press Test Enter
WINDOW_TABLE(MMIENG_MEM_PRESS_TEST_ENTER_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)EngMemPressTestEnterWinHandleMsg),

    WIN_ID(MMIENG_MEM_PRESS_TEST_ENTER_WIN_ID),
    WIN_TITLE(TXT_ENG_MEM_PRESS_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIENG_COMMON_CTRL1_ID),

    END_WIN
};

WINDOW_TABLE(MMIENG_MEM_PRESS_TEST_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)EngMemPressTestWinHandleMsg),

    WIN_ID(MMIENG_MEM_PRESS_TEST_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),

    END_WIN
};

#ifdef MP3_SUPPORT
WINDOW_TABLE(MMIENG_MP3PLAY_TEST_TAB) = 
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)EngMp3PlayWinHandleMsg),
    WIN_ID(MMIENG_MP3PLAY_TEST_WIN_ID),

    WIN_TITLE(TXT_ENG_MP3_PLAY_TEST),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
/**--------------------------------------------------------------------------*
                                     EXTERNAL DECLARE
 **--------------------------------------------------------------------------*/
extern BOOLEAN g_mmieng_showversion_from_ui ;
extern const MMI_ENG_DISPLAY_T mmi_eng_display;

extern MMI_VOICEPARAM_E g_mmieng_voice_param;

//extern LCD_ID_E g_mmieng_lcd;

//extern uint32 g_mmieng_afc_value;               // AFC
//extern uint32 g_mmieng_pa_value[4];                // PA
//extern uint32 g_mmieng_agc_value;               // AGC

extern uint32 g_mmieng_total_pch_number;
extern uint32 g_mmieng_lost_pch_number;

extern void LCD_SetBackLightBrightness( uint32  brightless );

/*===============================================================================
                                IMPLEMENTATION
===============================================================================*/


/********************************************************************************
 NAME:          MMIEng_SetResultWinText
 DESCRIPTION:   
 PARAM IN:      txt1 - 
                txt2 - 
                txt3 - 
 PARAM OUT:     None
 AUTHOR:        allen
 DATE:          2004.09.10
********************************************************************************/
PUBLIC void MMIEng_SetResultWinText(MMI_STRING_T txt1, MMI_STRING_T txt2, MMI_STRING_T txt3)
{
    if (txt1.wstr_ptr != PNULL && txt1.wstr_len > 0)
    {
        //s_mmieng_show3result_text1.is_ucs2 = txt1.is_ucs2;
        s_mmieng_show3result_text1.wstr_len = txt1.wstr_len;
        s_mmieng_show3result_text1.wstr_ptr = txt1.wstr_ptr;
    }
    else
    {
        //s_mmieng_show3result_text1.is_ucs2 = FALSE;
        s_mmieng_show3result_text1.wstr_len = 0;
        s_mmieng_show3result_text1.wstr_ptr = PNULL;
    }
    if (txt2.wstr_ptr != PNULL && txt2.wstr_len > 0)
    {
        //s_mmieng_show3result_text2.is_ucs2 = txt2.is_ucs2;
        s_mmieng_show3result_text2.wstr_len = txt2.wstr_len;
        s_mmieng_show3result_text2.wstr_ptr = txt2.wstr_ptr;
    }
    else
    {
        //s_mmieng_show3result_text2.is_ucs2 = FALSE;
        s_mmieng_show3result_text2.wstr_len = 0;
        s_mmieng_show3result_text2.wstr_ptr = PNULL;
    }
    if (txt3.wstr_ptr != PNULL && txt3.wstr_len > 0)
    {
        //s_mmieng_show3result_text3.is_ucs2 = txt3.is_ucs2;
        s_mmieng_show3result_text3.wstr_len = txt3.wstr_len;
        s_mmieng_show3result_text3.wstr_ptr = txt3.wstr_ptr;
    }
    else
    {
        //s_mmieng_show3result_text3.is_ucs2 = FALSE;
        s_mmieng_show3result_text3.wstr_len = 0;
        s_mmieng_show3result_text3.wstr_ptr = PNULL;
    }
}

#ifdef SFR_SUPPORT_CUCC
/********************************************************************************
 NAME:          EngCUCCDMRegMode2ListItemPos
 DESCRIPTION:   
 PARAM IN:
 PARAM OUT:     
 AUTHOR:        
 DATE:          
********************************************************************************/
LOCAL uint8 EngCUCCDMRegMode2ListItemPos(MMIENG_CUCCDM_REGMODE_E cuccdm_regmode)
{
    uint8 item_pos = 0;
    
    switch(cuccdm_regmode)
    {
    case CUCCDM_REGMODE_REG:
        item_pos = 0;
        break;
        
    case CUCCDM_REGMODE_TEST:
        item_pos = 1;
        break;
        
    default:
        //SCI_TRACE_LOW:"MMIENG_WIN.C EngCUCCDMRegMode2ListItemPos dv_frame_frequency is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8339_112_2_18_2_17_58_162,(uint8*)"d", cuccdm_regmode);
        break;
    }

    return item_pos;
}

/********************************************************************************
 NAME:          EngCUCCDMRegModeListItemPos2FrameFre
 DESCRIPTION:   
 PARAM IN:
 PARAM OUT:     
 AUTHOR:        
 DATE:          
********************************************************************************/
LOCAL MMIENG_CUCCDM_REGMODE_E EngCUCCDMRegModeListItemPos2FrameFre(int16 item_pos)
{
    MMIENG_CUCCDM_REGMODE_E cuccdm_regmode = CUCCDM_REGMODE_REG;
    
    switch(item_pos)
    {
    case 0:
        cuccdm_regmode = CUCCDM_REGMODE_REG;
        break;
		
    case 1:
        cuccdm_regmode = CUCCDM_REGMODE_TEST;
        break;
		
    default:
        //SCI_TRACE_LOW:"MMIENG_WIN.C EngCUCCDMRegModeListItemPos2FrameFre item_pos is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8384_112_2_18_2_17_58_163,(uint8*)"d",item_pos);
        break;
    }

    return cuccdm_regmode;
}

/********************************************************************************
 NAME:          EngCUCCDMSetClyWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngCUCCDMSetClyWinHandleMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T           text_id_tab[] = {
                                            TXT_ENG_CUCC_DM_5MIN, 
                                            TXT_ENG_CUCC_DM_HOUR, 
                                            TXT_ENG_CUCC_DM_DAY, 
                                            TXT_ENG_CUCC_DM_WEEK,
                                            TXT_ENG_CUCC_DM_MONTH
                                            };
    int16                   item_pos = 0;
    int16                   item_total_num = 0;
    MMI_CTRL_ID_T           list_id = MMIENG_LISTBOX_CTRL_ID;

    item_total_num = ARR_SIZE(text_id_tab);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(list_id, item_total_num, FALSE );//max item 5
        GUILIST_SetOwnSofterKey(list_id,TRUE);
        AppendListItem(
                    list_id,
                    text_id_tab,
                    item_total_num
                    );

        item_pos =MMISFR_GetCUCCRegCircle();

        GUILIST_SetSelectedItem(list_id, (uint16)item_pos, TRUE);
        GUILIST_SetCurItemIndex(list_id, (int16)item_pos);
        MMK_SetAtvCtrl(win_id, list_id);
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_CUCC_DM_SET_CLY_WIN_ID);
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        item_pos = GUILIST_GetCurItemIndex(list_id);
        MMISFR_SetCUCCRegCircle(item_pos);
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
/********************************************************************************
 NAME:          EngCUCCDMSetModeWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngCUCCDMSetModeWinHandleMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T           text_id_tab[] = {
                                            TXT_ENG_CUCC_DM_REG, 
                                            TXT_ENG_CUCC_DM_TEST
                                            };
    int16                   item_pos = 0;
    int16                   item_total_num = 0;
    MMI_CTRL_ID_T           list_id = MMIENG_LISTBOX_CTRL_ID;
    MMIENG_CUCCDM_REGMODE_E   cuccdm_regmode = CUCCDM_REGMODE_REG;

    item_total_num = ARR_SIZE(text_id_tab);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(list_id, item_total_num, FALSE );// item 2
        GUILIST_SetOwnSofterKey(list_id,TRUE);
        AppendListItem(
                    list_id, 
                    text_id_tab, 
                    item_total_num
                    );
        
        cuccdm_regmode = (MMIENG_CUCCDM_REGMODE_E) MMISFR_GetCUCCRegServer();
        item_pos =EngCUCCDMRegMode2ListItemPos(cuccdm_regmode);

        GUILIST_SetSelectedItem(list_id, (uint16)item_pos, TRUE);
        GUILIST_SetCurItemIndex(list_id, (int16)item_pos);
        MMK_SetAtvCtrl(win_id, list_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_CUCC_DM_SET_MODE_WIN_ID);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        item_pos = GUILIST_GetCurItemIndex(list_id);
        cuccdm_regmode = EngCUCCDMRegModeListItemPos2FrameFre(item_pos);
        MMISFR_SetCUCCRegServer((uint8)cuccdm_regmode);
        //success!
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

PUBLIC BOOLEAN MMIAPIENG_CreateWinCUCCDMSetCly(void)
{
    return MMK_CreateWin((uint32 * )MMIENG_CUCC_DM_SET_CLY_TAB, PNULL);
}

PUBLIC BOOLEAN MMIAPIENG_CreateWinCUCCDMSetMode(void)
{
    return MMK_CreateWin((uint32 * )MMIENG_CUCC_DM_SET_MODE_TAB, PNULL);
}

#endif

/********************************************************************************
 NAME:          EngShow3ResultWinHandleMsgNoAutoClose
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.10
********************************************************************************/
LOCAL MMI_RESULT_E EngShow3ResultWinHandleMsgNoAutoClose(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;    
     GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T          client_rect = MMITHEME_GetClientRect();
    GUI_RECT_T          lab_rect ={0};
#ifdef MMI_PDA_SUPPORT
    GUI_BOTH_RECT_T     client_both_rect =  MMITHEME_GetWinClientBothRect(win_id);
    client_rect.top = client_both_rect.v_rect.top;
#endif
    lab_rect = client_rect;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:      
        lab_rect.bottom = lab_rect.top + 40;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = lab_rect.top + 40 ;
        GUILABEL_SetRect(MMIENG_LABEL2_CTRL_ID, &lab_rect, FALSE);
        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL3_CTRL_ID, &lab_rect, FALSE);
        // start timer: if user do not press key in time, window 'll be closed when over time
        GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID, mmi_eng_display.bkgd_color);
        GUILABEL_SetBackgroundColor(MMIENG_LABEL2_CTRL_ID, mmi_eng_display.bkgd_color);
        GUILABEL_SetBackgroundColor(MMIENG_LABEL3_CTRL_ID, mmi_eng_display.bkgd_color);
        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &s_mmieng_show3result_text1, FALSE);
        GUILABEL_SetText(MMIENG_LABEL2_CTRL_ID, &s_mmieng_show3result_text2, FALSE);
        GUILABEL_SetText(MMIENG_LABEL3_CTRL_ID, &s_mmieng_show3result_text3, FALSE);
        break;
    
    case MSG_LCD_SWITCH:        
         lab_rect.bottom = lab_rect.top + 40;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = lab_rect.top + 40 ;
        GUILABEL_SetRect(MMIENG_LABEL2_CTRL_ID, &lab_rect, FALSE);
        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL3_CTRL_ID, &lab_rect, FALSE);
        break;
        
       
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, client_rect, mmi_eng_display.bkgd_color);
        GUIWIN_SetSoftkeyTextId(MMIENG_SHOW3RESULT_WIN_ID,  (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
        break;
        
    case MSG_APP_WEB:       
    case MSG_KEYDOWN_OK:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(MMIENG_SHOW3RESULT_WIN_ID);
        break;

    case MSG_TIMER:
        if (*(uint8*)param == s_mmieng_show3result_timer_id) 
        {
            MMK_CloseWin(MMIENG_SHOW3RESULT_WIN_ID);
        }
        else
            result = MMI_RESULT_FALSE;      // need additional process
        break;
        
    case MSG_CLOSE_WINDOW:          
        //s_mmieng_show3result_text1.is_ucs2 = FALSE;
        s_mmieng_show3result_text1.wstr_len = 0;
        s_mmieng_show3result_text1.wstr_ptr = PNULL;
        //s_mmieng_show3result_text2.is_ucs2 = FALSE;
        s_mmieng_show3result_text2.wstr_len = 0;
        s_mmieng_show3result_text2.wstr_ptr = PNULL;
        //s_mmieng_show3result_text3.is_ucs2 = FALSE;
        s_mmieng_show3result_text3.wstr_len = 0;
        s_mmieng_show3result_text3.wstr_ptr = PNULL;
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngShow3ResultWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.10
********************************************************************************/
LOCAL MMI_RESULT_E EngShow3ResultWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T          client_rect = {0}; 
    GUI_RECT_T          lab_rect={0};
    GUI_BOTH_RECT_T     client_both_rect =  MMITHEME_GetWinClientBothRect(win_id);
    int16               lab_height = 0;

    if(MMK_IsWindowLandscape(win_id))
    {
        client_rect = client_both_rect.h_rect;
    }
    else
    {
        client_rect = client_both_rect.v_rect;
    }

    lab_rect = client_rect;
    lab_height = (client_rect.bottom - client_rect.top)/3;  
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:       
        lab_rect.bottom = lab_rect.top + lab_height;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        lab_rect.top =  lab_rect.bottom;
        lab_rect.bottom = lab_rect.top + lab_height ;
        GUILABEL_SetRect(MMIENG_LABEL2_CTRL_ID, &lab_rect, FALSE);
        lab_rect.top =  lab_rect.bottom;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL3_CTRL_ID, &lab_rect, FALSE);        
            
        // start timer: if user do not press key in time, window 'll be closed when over time
        if (s_mmieng_show3result_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_show3result_timer_id);
            s_mmieng_show3result_timer_id = 0;
        }
        s_mmieng_show3result_timer_id = MMK_CreateWinTimer(MMIENG_SHOW3RESULT_WIN_ID, 
                                                        ENG_SHOWRESULT_TIMER_DELAY,
                                                        FALSE
                                                        );      
        GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID, mmi_eng_display.bkgd_color);
        GUILABEL_SetBackgroundColor(MMIENG_LABEL2_CTRL_ID, mmi_eng_display.bkgd_color);
        GUILABEL_SetBackgroundColor(MMIENG_LABEL3_CTRL_ID, mmi_eng_display.bkgd_color);
        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &s_mmieng_show3result_text1, FALSE);
        GUILABEL_SetText(MMIENG_LABEL2_CTRL_ID, &s_mmieng_show3result_text2, FALSE);
        GUILABEL_SetText(MMIENG_LABEL3_CTRL_ID, &s_mmieng_show3result_text3, FALSE);
        break;

    case MSG_LCD_SWITCH:
        lab_rect.bottom = lab_rect.top + lab_height;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        lab_rect.top =  lab_rect.bottom;
        lab_rect.bottom = lab_rect.top + lab_height ;
        GUILABEL_SetRect(MMIENG_LABEL2_CTRL_ID, &lab_rect, FALSE);
        lab_rect.top =  lab_rect.bottom;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL3_CTRL_ID, &lab_rect, FALSE);    
        break;
        
    case MSG_FULL_PAINT:       
        LCD_FillRect(&lcd_dev_info, client_rect, mmi_eng_display.bkgd_color);
        GUIWIN_SetSoftkeyTextId(MMIENG_SHOW3RESULT_WIN_ID,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
        break;

    case MSG_APP_WEB:       
    case MSG_KEYDOWN_OK:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(MMIENG_SHOW3RESULT_WIN_ID);
        break;

    case MSG_TIMER:
        if (*(uint8*)param == s_mmieng_show3result_timer_id) 
        {
            MMK_CloseWin(MMIENG_SHOW3RESULT_WIN_ID);
        }
        else
            result = MMI_RESULT_FALSE;      // need additional process
        break;
        
    case MSG_CLOSE_WINDOW:
        if (s_mmieng_show3result_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_show3result_timer_id);
            s_mmieng_show3result_timer_id = 0;
        }
        //s_mmieng_show3result_text1.is_ucs2 = FALSE;
        s_mmieng_show3result_text1.wstr_len = 0;
        s_mmieng_show3result_text1.wstr_ptr = PNULL;
        //s_mmieng_show3result_text2.is_ucs2 = FALSE;
        s_mmieng_show3result_text2.wstr_len = 0;
        s_mmieng_show3result_text2.wstr_ptr = PNULL;
        //s_mmieng_show3result_text3.is_ucs2 = FALSE;
        s_mmieng_show3result_text3.wstr_len = 0;
        s_mmieng_show3result_text3.wstr_ptr = PNULL;
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngShowHelpWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.13
********************************************************************************/
LOCAL MMI_RESULT_E EngShowHelpWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    wchar wstr[80] = {0};
    char* str = "Use the left soft key to enter a menu; use the right soft key to leave a menu.";
    uint16 len = strlen( str );
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        text.wstr_ptr = wstr;//"Use the left soft key to enter a menu; use the right soft key to leave a menu.";
        text.wstr_len = len;

        MMI_STRNTOWSTR( wstr, 80, (uint8*)str, len, len );

        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
        MMK_SetAtvCtrl(MMIENG_SHOWHELP_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        break;
        
    case MSG_CTL_OK:        
    case MSG_CTL_MIDSK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_SHOWHELP_WIN_ID);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngShow3rdPartyVersionWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        zhaohui
 DATE:          2008.09.19
********************************************************************************/
LOCAL MMI_RESULT_E EngShow3rdPartyVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    GUI_RECT_T                rect = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIENG_3RDPARTY_VERSION_CTRL_ID);
        Show3rdPartyVersionInfo(FALSE);
        break;
        
    case MSG_FULL_PAINT:
        lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
        lcd_dev_info.block_id = GUI_BLOCK_MAIN;
        LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        break;
        
    case MSG_CTL_OK:        
    case MSG_CTL_CANCEL:
    case MSG_CTL_MIDSK:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : 显示第三方软件版本信息
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void Show3rdPartyVersionInfo(
                            BOOLEAN   need_update
                            )
{
    wchar        unicode_cr = 0x0d;
    MMI_STRING_T    cr_s = {0};
    MMI_STRING_T    text_s = {0};
    uint16           uint16_str[MMIENG_3RDPARTYVER_TEXT_LEN + 1] = {0};
    uint16          uint16_str_len = 0;
    char*          temp_ptr = PNULL;
    uint16          temp_len = 0;
    
    cr_s.wstr_ptr = &unicode_cr;
    cr_s.wstr_len = 1;

    //set im
    temp_ptr = "Im Version:";
    temp_len = 11;
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行     
#ifndef _WIN32
#if defined(IM_ENGINE_CSTAR)
    temp_ptr = CS_IMM_GetVersion();     /*lint !e605*/
#elif defined(IM_ENGINE_IEKIE)
    temp_ptr = IIME_IMM_GetVersion();
#else
    temp_ptr = "IM_R1.0.5003.C0.2";
#endif

#else
    temp_ptr = "IM_R1.0.5003.C0.2"; // this is a example for simulator
#endif  
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#if (defined(DM_SUPPORT) || defined(DM_ONLY_SELFREG_SUPPORT))
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    //set dm
    temp_ptr = "Dm Version: ";
    temp_len =(uint16)SCI_STRLEN((char*)temp_ptr); 
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行     
    temp_ptr = MMIAPIDM_GetVersion();   // this is a example for simulator
    if (PNULL == temp_ptr)
    {
        temp_ptr = "NONE";
    }
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    
#endif  
#ifdef JAVA_SUPPORT    
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    //set java
    temp_ptr = "Java Version: ";
    temp_len =(uint16)SCI_STRLEN((char*)temp_ptr); 
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );/*lint !e605*/
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行     
    temp_ptr = MMIAPIJAVA_GetVersion();/*lint !e605*/
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#endif

/* BEGIN: Added by George.Liu 01526, 2010/8/23   MS:00196383 */
#ifdef WIFI_SUPPORT
    temp_ptr = "WiFi Version:";
    temp_len = 13;
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#ifndef _WIN32
    temp_ptr = WIFI_GetVersion();
    temp_len = SCI_STRLEN((char*)temp_ptr);
#else
    temp_ptr = "WIFI_CSR_1.0";  // this is a example for simulator
    temp_len = 12;
#endif
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#endif
/* END:   Added by George.Liu 01526, 2010/8/23   MS:00196383 */

#ifdef BROWSER_SUPPORT_DORADO 
    if (PNULL != MMIAPIBROWSER_GetVersion(MMIBROWSER_TYPE_DORADO))
    {
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
        //set Browser
        temp_ptr = "Browser Version(DORADO): ";
        temp_len =(uint16)SCI_STRLEN((char*)temp_ptr); 
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
        temp_ptr = MMIAPIBROWSER_GetVersion(MMIBROWSER_TYPE_DORADO);
        temp_len = SCI_STRLEN((char*)temp_ptr);
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    }
#endif    
#ifdef BROWSER_SUPPORT_NETFRONT
    if (PNULL != MMIAPIBROWSER_GetVersion(MMIBROWSER_TYPE_NF))
    {
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
        //set Browser
        temp_ptr = "Browser Version(NF): ";
        temp_len =(uint16)SCI_STRLEN((char*)temp_ptr); 
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
        temp_ptr = MMIAPIBROWSER_GetVersion(MMIBROWSER_TYPE_NF);
        temp_len = SCI_STRLEN((char*)temp_ptr);
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    }
#endif    
#ifdef OPERA_MINI_SUPPORT
    if (PNULL != MMIAPIBROWSER_GetVersion(MMIBROWSER_TYPE_OPERA_MINI))
    {
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
        //set Browser
        temp_ptr = "Browser Version(Opera Mini): ";
        temp_len =(uint16)SCI_STRLEN((char*)temp_ptr); 
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
        temp_ptr = MMIAPIBROWSER_GetVersion(MMIBROWSER_TYPE_OPERA_MINI);
        temp_len = SCI_STRLEN((char*)temp_ptr);
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    }

#endif    

    text_s.wstr_ptr = uint16_str;
    text_s.wstr_len = uint16_str_len;

    GUITEXT_SetString(MMIENG_3RDPARTY_VERSION_CTRL_ID, text_s.wstr_ptr, text_s.wstr_len, need_update);
}

/********************************************************************************
 NAME:          EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        gang.tong
 DATE:          2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngShowLayer1MonitorWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    LAYER1_MONITOR_ITEM_T monitor_item = {0};
#ifndef MMI_DUALMODE_ENABLE
    RRA_MONITOR_ITEM_T item_ptr = {0};  //pizer.fan temp-modify
#endif

#ifndef MMI_DUALMODE_ENABLE
    uint8 showbuf[1224] = {0};
#else
    uint8 showbuf[1024] = {0};
#endif
    uint8 item_show_buf[64] = {0};
    
#ifndef _WIN32
    LAYER1_GetLayer1MonItem(&monitor_item); 

#ifndef MMI_MULTI_SIM_SYS_SINGLE
#ifndef MMI_DUALMODE_ENABLE
    RRA_GetRraMonItem(&item_ptr);//pizer.fan temp-modify
#endif //endif MMI_DUALMODE_ENABLE
#endif

#else
    SCI_MEMSET(&monitor_item,0,sizeof(LAYER1_MONITOR_ITEM_T));
#ifndef MMI_DUALMODE_ENABLE
    SCI_MEMSET(&item_ptr,0,sizeof(RRA_MONITOR_ITEM_T)); //pizer.fan temp-modify
#endif //endif MMI_DUALMODE_ENABLE

#endif
    //@yongwei.he fix cr97274 begin
    sprintf((char*)item_show_buf, "%s = %lu\n", "bsic_search_total", monitor_item.bsic_search_total);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "bsic_search_succ", monitor_item.bsic_search_succ);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "bsic_confirm_total", monitor_item.bsic_confirm_total);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "bsic_confirm_succ", monitor_item.bsic_confirm_succ);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "scell_bcch_total", monitor_item.scell_bcch_total);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "scell_bad_bcch", monitor_item.scell_bad_bcch);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "ncell_bcch_total", monitor_item.ncell_bcch_total);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "ncell_bad_bcch", monitor_item.ncell_bad_bcch);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "rxlev_total", monitor_item.rxlev_total);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "rxlev_bad", monitor_item.rxlev_bad);
    strcat((char*)showbuf, (char*)item_show_buf);
    
    sprintf((char*)item_show_buf, "%s = %lu\n", "pch_block_total", monitor_item.pch_block_total);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "bad_pch_num", monitor_item.bad_pch_num);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "rla_sort_total", monitor_item.rla_sort_total);
    strcat((char*)showbuf, (char*)item_show_buf);
    
    sprintf((char*)item_show_buf, "%s = %lu\n", "tx_rach_num", monitor_item.send_rach_num);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "rx_agch_num", monitor_item.rx_agch_num);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "tx_prach_num",monitor_item.send_prach_num);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "rx_pagch_num", monitor_item.rx_pagch_num);
    strcat((char*)showbuf, (char*)item_show_buf);
//pizer.fan temp-modify

    sprintf((char*)item_show_buf, "%s = %lu\n", "dummy_pch_cnt", monitor_item.dummy_pch_cnt);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "detected_dummy_pch_cnt", monitor_item.detected_dummy_pch_cnt);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "burst_pch_cnt1",monitor_item.burst_pch_cnt1);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "burst_pch_cnt2", monitor_item.burst_pch_cnt2);
    strcat((char*)showbuf, (char*)item_show_buf);   
    sprintf((char*)item_show_buf, "%s = %lu\n", "burst_pch_cnt3",monitor_item.burst_pch_cnt3);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "burst_pch_cnt4", monitor_item.burst_pch_cnt4);
    strcat((char*)showbuf, (char*)item_show_buf);

    
#ifndef MMI_DUALMODE_ENABLE
    sprintf((char*)item_show_buf, "%s = %lu\n", "bcch_camp_num", monitor_item.bcch_camp_num);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "pwrswp_num", monitor_item.pwrswp_num);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "handover_total", monitor_item.handover_total);
    strcat((char*)showbuf, (char*)item_show_buf);
    sprintf((char*)item_show_buf, "%s = %lu\n", "handover_fail", monitor_item.handover_fail);
    strcat((char*)showbuf, (char*)item_show_buf);   

//pizer.fan temp-modify
#if 0    
    sprintf((char*)item_show_buf, "%s = %lu\x0d", "powersweep_req_1", item_ptr.powersweep_req_1);
    strcat((char*)showbuf, (char*)item_show_buf);   
    sprintf((char*)item_show_buf, "%s = %lu\x0d", "powersweep_to_layer1_0", item_ptr.powersweep_to_layer1_0);
    strcat((char*)showbuf, (char*)item_show_buf);   
    sprintf((char*)item_show_buf, "%s = %lu\x0d", "powersweep_to_layer1_1", item_ptr.powersweep_to_layer1_1);
    strcat((char*)showbuf, (char*)item_show_buf);   
    sprintf((char*)item_show_buf, "%s = %lu\x0d", "fab_powersweep_cnf_num", item_ptr.fab_powersweep_cnf_num);
    strcat((char*)showbuf, (char*)item_show_buf);   
    sprintf((char*)item_show_buf, "%s = %lu\x0d", "rach_exp_num", item_ptr.rach_exp_num);
    strcat((char*)showbuf, (char*)item_show_buf);   
    sprintf((char*)item_show_buf, "%s = %lu\x0d", "rach_delay_exp_num",item_ptr.rach_delay_exp_num);
    strcat((char*)showbuf, (char*)item_show_buf);   
    sprintf((char*)item_show_buf, "%s = %lu\x0d", "fab_scell_meas_num",item_ptr.fab_scell_meas_num);
    strcat((char*)showbuf, (char*)item_show_buf);
#endif
    
#endif
    text.wstr_len = strlen((char*)showbuf);
    //SCI_TRACE_LOW:"EngShowLayer1MonitorWinHandleMsg text.wstr_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_3108_112_2_18_2_17_47_126,(uint8*)"d",text.wstr_len);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            wchar* wstr = (wchar*)SCI_ALLOC_APP(1225 * sizeof(wchar));

            text.wstr_ptr = wstr;

            MMI_STRNTOWSTR( wstr, 1224, showbuf, text.wstr_len, text.wstr_len );

            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
            MMK_SetAtvCtrl(win_id, MMIENG_TEXTBOX1_CTRL_ID);

            SCI_FREE( wstr );
        }
        break;
        
    case MSG_FULL_PAINT:
        // the softkey is different between UI-TEST mode and Engineer mode
        if (g_mmieng_showversion_from_ui)
            GUIWIN_SetSoftkeyTextId(win_id,  0, STXT_UP, 0, FALSE);
        else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, FALSE);
        break;
        
    case MSG_KEYDOWN_UP:
        // only in UI-TEST mode, UP key will do the next action; otherwise will do nothing
        if (g_mmieng_showversion_from_ui)                    
        {
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin(win_id);
        }    
        break;

    case MSG_CTL_MIDSK:     
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        // only in Engineer mode, OK / CANCEL key will close this window; otherwise will do nothing
        if (!g_mmieng_showversion_from_ui)
            MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT        
    case MSG_KEYDOWN_FLIP:      // close flip
        // in UI-TEST mode, close FLIP will do nothing
        if (!g_mmieng_showversion_from_ui)
            result = MMI_RESULT_FALSE;
        break;
#endif
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


/********************************************************************************
 NAME:          EngShowPowersweepInfoWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        Jerry Liang
 DATE:          2008.07.30
********************************************************************************/
// LOCAL MMI_RESULT_E EngShowPowersweepInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
// {
//     MMI_RESULT_E result = MMI_RESULT_TRUE;
//     
//     switch (msg_id) 
//     {
//     case MSG_OPEN_WINDOW:
//         {
//             wchar* wstr = (wchar*)SCI_ALLOC_APP(4097 * sizeof(wchar));
//             MMI_STRING_T text = {0};
//             MN_ENG_POWER_SWEEP_INFO_T sweep_info = {0}; 
//             uint8 item_show_buf[64] = {0};
//         #ifdef _WIN32 
//             int i;  
//         #endif  
//             int j;
// 
//             uint8 *no_sim = "NO SIM CARD!! \n"; /*lint !e64*/
// 
//             uint8 *showbuf = PNULL;
//             
//             //该数组不做宏控制: wast some memory(about 60 Bytes) but keep code clearly!!!
//             uint8 sim_str[][30] = {"SIM CARD 1 \n","SIM CARD 2 \n","SIM CARD 3 \n","SIM CARD 4 \n"}; 
//             
//             MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
// 
//             showbuf = (uint8 *)SCI_ALLOCA((MMI_DUAL_SYS_MAX * 2048));
// 
//             if (PNULL == showbuf)
//             {
//                 MMK_CloseWin(win_id);
//                 break;
//             }
// 
//             SCI_MEMSET(showbuf, 0x00, (MMI_DUAL_SYS_MAX * 2048));
// 
//             if(0 == MMIAPIPHONE_GetSimExistedNum(PNULL,1))
//             {        
//                 strcat((char*)showbuf, (char*)no_sim);
//             }
//             else
//             {
//                 for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
//                 {
//                     //BOOLEAN is_sim_exist = 
//                     if(MMIAPIPHONE_GetSimExistedStatus(dual_sys))
//                     {
//                     #ifndef _WIN32    
//                         MNENG_GetPowerSweepInfoEx( dual_sys,&sweep_info );
//                     #else
//                         sweep_info.num = 40;
//                         for( i=0;i<40;i++)
//                         {
//                              sweep_info.pwr_result_list[i].arfcn = i+1;
//                              sweep_info.pwr_result_list[i].rssi = (i+1)*100;
//                         }
//                     #endif
//                         
//                         strcat((char*)showbuf, (char*)sim_str[dual_sys]);
//                         
//                         for( j=0;j<sweep_info.num;j++)
//                         {
//                             sprintf((char*)item_show_buf, "%d  arfcn=%d  rssi=%d \x0d", j+1, sweep_info.pwr_result_list[j].arfcn, sweep_info.pwr_result_list[j].rssi);
//                             strcat((char*)showbuf, (char*)item_show_buf);
//                             SCI_MEMSET(item_show_buf,0,(sizeof(uint8)*64));
//                         }                
//                     }
//                 }
//             }
//             
//             text.wstr_len = MIN(strlen((char*)showbuf), 4096);/*lint !e666*/
//             
//             text.wstr_ptr = wstr;
//             
//             MMI_STRNTOWSTR( wstr, 4096,  showbuf, text.wstr_len, text.wstr_len );
//             GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len,FALSE);
//             MMK_SetAtvCtrl(win_id, MMIENG_TEXTBOX1_CTRL_ID);
//             
//             SCI_FREE( wstr );
//             SCI_FREE( showbuf );
//         }
//         break;
//         
//     case MSG_FULL_PAINT:
//         // the softkey is different between UI-TEST mode and Engineer mode
//         if (g_mmieng_showversion_from_ui)
//             GUIWIN_SetSoftkeyTextId(win_id,  0, STXT_UP, 0, FALSE);
//         else
//             GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, FALSE);
//         break;
//         
//     case MSG_KEYDOWN_UP:
//         // only in UI-TEST mode, UP key will do the next action; otherwise will do nothing
//         if (g_mmieng_showversion_from_ui)                    
//         {
//             MMIAPIENG_RunUITestNextStep();
//             MMK_CloseWin(win_id);
//         }    
//         break;
// 
//     case MSG_CTL_MIDSK:     
//     case MSG_CTL_OK:
//     case MSG_CTL_CANCEL:
//         // only in Engineer mode, OK / CANCEL key will close this window; otherwise will do nothing
//         if (!g_mmieng_showversion_from_ui)
//             MMK_CloseWin(win_id);
//         break;
// 
// #ifdef FLIP_PHONE_SUPPORT        
//     case MSG_KEYDOWN_FLIP:      // close flip
//         // in UI-TEST mode, close FLIP will do nothing
//         if (!g_mmieng_showversion_from_ui)
//             result = MMI_RESULT_FALSE;
//         break;
// #endif
//     
//     default:
//         result = MMI_RESULT_FALSE;
//         break;
//     }
//     return (result);
// }

/********************************************************************************
 NAME:          GetSCNetInfoStr
 DESCRIPTION:   
 PARAM IN:      net_buf - 
                buf_len - 
 PARAM OUT:     net info string len
 AUTHOR:        allen
 DATE:          2004.09.08
********************************************************************************/
LOCAL int GetSCNetInfoStr(uint8* net_buf, uint16 buf_len)
{
    MN_PHONE_SCELL_MEAS_T scell = {0};
    uint8 digit_buf1[19] = {0};
    uint8 digit_buf2[19] = {0};
    int len1 = 0, len2 = 0;
    int offset = 0;
    
    if(PNULL == net_buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetSCNetInfoStr net_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_3317_112_2_18_2_17_47_127,(uint8*)"");
        return -1;
    }
    
    //SCI_TRACE_LOW:"mmieng_win.c GetSCNetInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_3321_112_2_18_2_17_47_128,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }

    
    #ifndef _WIN32
#ifdef MMI_DUALMODE_ENABLE
    scell = em_GetRSSIEx(MMIAPISET_GetActiveSim());
#else
    scell = MNPHONE_GetRSSIEx(MMIAPISET_GetActiveSim());
#endif
    #else
    scell.arfcn = 123;
    scell.rxlev =67;// 567;
    #endif
    MMIAPICOM_Int2Str(scell.arfcn, digit_buf1, 18); // absolute RF channel number
    MMIAPICOM_Int2Str(scell.rxlev, digit_buf2, 18); // Radio Signal Strength Indicator
    len1 = SCI_STRLEN((char*)digit_buf1);
    MMI_MEMCPY(net_buf + offset, buf_len - offset, digit_buf1, 18, len1);
    offset += len1;
    len2 = SCI_STRLEN((char*)digit_buf2);
    if (len1 + len2 + 1 <= ENG_WIN_MAX_CHAR)
    {
        MMI_MEMCPY(net_buf + offset, buf_len - offset, ",", 1, 1);
        offset += 1;
        MMI_MEMCPY(net_buf + offset, buf_len - offset, digit_buf2, 18, len2);
        offset += len2;
    }
    return (offset);
}

/********************************************************************************
 NAME:          GetNCNetInfoStr
 DESCRIPTION:   
 PARAM IN:      net_buf - 
                buf_len - 
 PARAM OUT:     net info string len
 AUTHOR:        allen
 DATE:          2004.09.08
********************************************************************************/
#ifdef MMI_DUALMODE_ENABLE
extern   ERR_MNPHONE_CODE_E em_GetMeasReportEx(
                MN_DUAL_SYS_E dual_sys,
                APP_MN_NCELL_RSSI_CNF_T *ncell_meas_ptr,
                MN_PHONE_CELL_MEAS_T    *scell_meas_ptr
                );      /*lint !e762*/
#endif
LOCAL int GetNCNetInfoStr(uint8* net_buf, uint16 buf_len)
{
    APP_MN_NCELL_RSSI_CNF_T ncell = {PNULL};
//     MN_PHONE_CELL_MEAS_T     scell={PNULL};
    uint8 digit_buf1[19] = {0};
    uint8 digit_buf2[19] = {0};
    int i = 0;
//@yongwei.he fix cr98371 begin    
       int index_present[MN_MAX_NCELL_NUM] = {0};
       int num_present = 0;
//@yongwei.he fix cr98371 end      
    int len1 = 0, len2 = 0;
    int offset = 0;
    
    if(PNULL == net_buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetNCNetInfoStr net_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_3385_112_2_18_2_17_48_129,(uint8*)"");
        return -1;
    }
    
    //SCI_TRACE_LOW:"mmieng_win.c GetNCNetInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_3389_112_2_18_2_17_48_130,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
#ifndef _WIN32
#ifdef MMI_DUALMODE_ENABLE
#ifdef _8800D_OLD_API
#ifdef TIANLONG_MOCOR
    if (em_GetMeasReport(&ncell,PNULL) == MN_RETURN_SUCCESS)
#else
    if (em_GetMeasReport(&ncell)==MN_RETURN_SUCCESS)
#endif
#else
    if (em_GetMeasReportEx(MMIAPISET_GetActiveSim(), &ncell, PNULL)==MN_RETURN_SUCCESS)//(MNPHONE_GetMeasReportEx(MMIAPISET_GetActiveSim(), &ncell,&scell) == MN_RETURN_SUCCESS)
#endif
#else
#ifdef _8800D_OLD_API
#ifdef TIANLONG_MOCOR
    if (MNPHONE_GetMeasReport(&ncell,PNULL) == MN_RETURN_SUCCESS)
#else
    if (MNPHONE_GetMeasReport(&ncell)==MN_RETURN_SUCCESS)
#endif
#else
    if (MNPHONE_GetMeasReportEx(MMIAPISET_GetActiveSim(), &ncell, PNULL)==MN_RETURN_SUCCESS)//(MNPHONE_GetMeasReportEx(MMIAPISET_GetActiveSim(), &ncell,&scell) == MN_RETURN_SUCCESS)
#endif
#endif
#else 
    for (i = 0; i < MN_MAX_NCELL_NUM; i++)      // this is for example for simulator
    {
        ncell.ncell_power[i].cell_present = TRUE;
        ncell.ncell_power[i].arfcn = i * 10;
        ncell.ncell_power[i].rxlev = i * 50;
    }
//  if (TRUE)
#endif
    {
//@yongwei.he fix cr98371 begin
                for (i = 0; i < MN_MAX_NCELL_NUM; i++)      // this is for example for simulator
                {
                         if (ncell.ncell_power[i].cell_present) // this channel is served
                        {
                            index_present[num_present++] = i;
                        }
                }
        
                for (i = 0; i < num_present; i++)
          {
                   // order
                MMIAPICOM_Int2Str(i + 1, digit_buf1, 18);       //( i + 1)   is the No. of NCELL
                len1 = SCI_STRLEN((char*)digit_buf1);
                MMI_MEMCPY(net_buf + offset, buf_len - offset, digit_buf1, 18, len1);
                offset += len1;
                MMI_MEMCPY(net_buf + offset, buf_len - offset, ". ", 2, 2);
                offset += 2;
                     MMIAPICOM_Int2Str(ncell.ncell_power[index_present[i]].arfcn, digit_buf1, 18);
                     MMIAPICOM_Int2Str(ncell.ncell_power[index_present[i]].rxlev, digit_buf2, 18);
                     len1 = SCI_STRLEN((char*)digit_buf1);
                     MMI_MEMCPY(net_buf + offset, buf_len - offset, digit_buf1, 18, len1);
                     offset += len1;
                     len2 = SCI_STRLEN((char*)digit_buf2);
                     if (len1 + len2 + 1 <= ENG_WIN_MAX_CHAR)
                     {
                         MMI_MEMCPY(net_buf + offset, buf_len - offset,  ",", 1, 1);
                         offset += 1;
                         MMI_MEMCPY(net_buf + offset, buf_len - offset, digit_buf2, 18, len2);
                         offset += len2;
                     }
                     net_buf[offset] = CR_CHAR;
                     offset += 1;
          }
                for (i = num_present; i < MN_MAX_NCELL_NUM; i++)
                {
                    // order
                    MMIAPICOM_Int2Str(i + 1, digit_buf1, 18);       //( i + 1)   is the No. of NCELL
                    len1 = SCI_STRLEN((char*)digit_buf1);
                    MMI_MEMCPY(net_buf + offset, buf_len - offset, digit_buf1, 18, len1);
                    offset += len1;
                    MMI_MEMCPY(net_buf + offset, buf_len - offset, ". ", 2, 2);
                    offset += 2;
                    MMI_MEMCPY(net_buf + offset, buf_len - offset, "---", 3, 3);
                    offset += 3;
                    net_buf[offset] = CR_CHAR;
                    offset += 1;
                }
//@yongwei.he fix cr98371 end                
    }
    return (offset);
}

/********************************************************************************
 NAME:          EngShowNetWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        ying.xu
 DATE:          2010.7.27
********************************************************************************/
LOCAL int32 EngGetNetInfo(char *buf, int32 buf_length)
{

    char temp_str[1024] = {0} ;   
    int32 offset= 0;
    int32 length = 0;
    char* str = PNULL;
    ALL_NETWORK_INFO_NAME_T* netnamestr=PNULL;



     buf[offset] = CR_CHAR;
     offset += 1;

    netnamestr = MMIAPIPHONE_GetAllNetworkNameInfo();
    if (netnamestr !=PNULL)
        {
        str = "FULL NAME: ";
        length = SCI_STRLEN(str);       
        SCI_MEMCPY(buf  + offset , str,length);
        offset += length;
        if (netnamestr->fullname_len >0)
            {
            length= netnamestr->fullname_len;
            SCI_MEMCPY(buf  + offset , netnamestr->full_name,length);
            offset += length;
            }
        buf[offset] = CR_CHAR;
        offset += 1;     
        
        str = "SHORT NAME: ";
        length = SCI_STRLEN(str);       
        SCI_MEMCPY(buf  + offset , str,length);
        offset += length;
        if (netnamestr->shortname_len >0)
            {
            length= netnamestr->shortname_len;
            SCI_MEMCPY(buf  + offset , netnamestr->short_name,length);
            offset += length;
            }        
        buf[offset] = CR_CHAR;
        offset += 1;

        SCI_TRACE_LOW("[MMIENG]:EngGetNetInfo  short len = %d,  full name len = %d",netnamestr->shortname_len,netnamestr->fullname_len); 
        }
    ////////////////
    // served CELL
    ////////////////
    str = "SCELL(arfcn,rxlev)";
    length = SCI_STRLEN(str);       
    if(buf_length - offset <  length)
    {
        return offset;
    }          
    SCI_MEMCPY(buf  + offset , str,length)
        offset += length;
    
    if(buf_length - offset <  1)
    {
        return offset;
    }   
    buf[offset] = CR_CHAR;
    offset += 1;
    
    length = GetSCNetInfoStr((uint8*)temp_str, 1023);
    if(buf_length - offset <  length)
    {
        return offset;
    } 
    SCI_MEMCPY(buf  + offset ,temp_str,length)
        offset += length;
    
    if(buf_length - offset <  1)
    {
        return offset;
    } 
    buf[offset] = CR_CHAR;
    offset += 1;
    
    //////////////////
    // neighbor CELL
    //////////////////
    str = "NCELL(arfcn,rxlev)";
    length = SCI_STRLEN(str);     
    if(buf_length - offset <  length)
    {
        return offset;
    } 
    SCI_MEMCPY(buf  + offset , str,length)
        offset += length;
    
    if(buf_length - offset <  1)
    {
        return offset;
    } 
    buf[offset] = CR_CHAR;
    offset += 1;
    
    length =  GetNCNetInfoStr((uint8*)temp_str, 1023);
    if(buf_length - offset <  length)
    {
        return offset;
    } 
    SCI_MEMCPY(buf  + offset , temp_str,length)
        offset += length;      
#ifdef ENGTD_SUPPORT
    
    if(buf_length - offset <  1)
    {
        return offset;
    } 
    buf[offset] = CR_CHAR;
    offset += 1;
    
    str = "TDNCELL(arfcn,rxlev)";
    length = SCI_STRLEN(str);        
    if(buf_length - offset <  length)
    {
        return offset;
    } 
    SCI_MEMCPY(buf  + offset , str,length)
        offset += length;
    
    if(buf_length - offset <  1)
    {
        return offset;
    } 
    buf[offset] = CR_CHAR;
    offset += 1;
    
    length =    GetTDNCNetInfoStr((uint8*)temp_str, 1023);
    if(buf_length - offset <  length)
    {
        return offset;
    } 
    SCI_MEMCPY(buf  + offset , temp_str,length)
        offset += length;  
    
    if(buf_length - offset <  1)
    {
        return offset;
    } 
    buf[offset] = CR_CHAR;
    offset += 1;
#endif 
    return offset; 
}
/********************************************************************************
 NAME:          EngShowNetWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.08
********************************************************************************/
LOCAL MMI_RESULT_E EngShowNetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    char buf[1024] = {0};
    wchar wstr[1024] = {0};
    wchar wstr2[1024+40] = {0};
    int length = 0; 
    
    MMI_RESULT_E result = MMI_RESULT_TRUE;  
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:       
            length = EngGetNetInfo(buf, 1023);
            {
                MMI_STRING_T            item_spn_str    ={0};
                wchar               opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN]    = {0};
                wchar               opn_spn_name2[]    = {L"SPN NAME  ="};

                MMIAPICOM_Wstrcat(wstr2,opn_spn_name2);
                item_spn_str.wstr_ptr = (wchar *)opn_spn_name;
                item_spn_str.wstr_len = 0;   
                MMIAPIPHONE_GetSPNNameFromNitz(MN_DUAL_SYS_1, &item_spn_str);  
                if( item_spn_str.wstr_len>0)
                {
                MMIAPICOM_Wstrcat(wstr2,item_spn_str.wstr_ptr);
                }
            }
        
        MMI_STRNTOWSTR( wstr, 1023, (uint8*)buf, length, length);
        MMIAPICOM_Wstrcat(wstr2,wstr);
        length = MMIAPICOM_Wstrlen(wstr2);
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, wstr2, length, FALSE);           

        if (s_mmieng_netinfo_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_netinfo_timer_id);
            s_mmieng_netinfo_timer_id = 0;
        }
        s_mmieng_netinfo_timer_id = MMK_CreateWinTimer(win_id, 2000, FALSE);
        MMK_SetAtvCtrl(win_id,MMIENG_TEXTBOX1_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:     
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:    
    case MSG_KEYDOWN_OK:        
    case MSG_KEYDOWN_CANCEL:
    case MSG_APP_WEB:
        MMK_CloseWin(win_id);
        break;
    case MSG_KEYUP_UP:    
    case MSG_KEYUP_DOWN:
        if (s_mmieng_netinfo_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_netinfo_timer_id);
            s_mmieng_netinfo_timer_id = 0;
        }
        s_mmieng_netinfo_timer_id = MMK_CreateWinTimer(win_id, 2000, FALSE);
        break;
    case MSG_TIMER:
        if (*(uint8*)param == s_mmieng_netinfo_timer_id) 
        {        
            length = EngGetNetInfo(buf, 1023);
            MMI_STRNTOWSTR( wstr, 1023, (uint8*)buf, length, length);
            {
                MMI_STRING_T            item_spn_str    ={0};
                wchar               opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN]    = {0};
                wchar               opn_spn_name2[]    = {L"SPN NAME  ="};

                MMIAPICOM_Wstrcat(wstr2,opn_spn_name2);
                item_spn_str.wstr_ptr = (wchar *)opn_spn_name;
                item_spn_str.wstr_len = 0;   
                MMIAPIPHONE_GetSPNNameFromNitz(MN_DUAL_SYS_1, &item_spn_str);  
                if( item_spn_str.wstr_len>0)
                {
                MMIAPICOM_Wstrcat(wstr2,item_spn_str.wstr_ptr);
                }
            }
        
        MMI_STRNTOWSTR( wstr, 1023, (uint8*)buf, length, length);
        MMIAPICOM_Wstrcat(wstr2,wstr);
        length = MMIAPICOM_Wstrlen(wstr2);
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, wstr2, length, TRUE);         
        
            s_mmieng_netinfo_timer_id = MMK_CreateWinTimer(win_id, 2000, FALSE);
        }
        else
            result = MMI_RESULT_FALSE;      // need additional process
        break;
            
    case MSG_LOSE_FOCUS:
        MMK_PauseTimer(s_mmieng_netinfo_timer_id);
        break;
        
    case MSG_GET_FOCUS: 
        MMK_ResumeTimer(s_mmieng_netinfo_timer_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (s_mmieng_netinfo_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_netinfo_timer_id);
            s_mmieng_netinfo_timer_id = 0;
        }
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

LOCAL int32 IsForcedCell(int16  arfcn)
{
    int32 i = 0;
    for(i = 0; i < s_cell_num; i++)
    {
        if(arfcn == s_cell_arfcn[i])
        {
            return 1;
        }        
    }
    return 0;
}

LOCAL int32 ClearAllCellListData()
{
    //SCI_MEMSET(s_cell_arfcn, -1, sizeof(s_cell_arfcn));
	uint32 i = 0;
	for(i = 0; i < MN_MAX_NCELL_NUM; i++)
	{
		s_cell_arfcn[i] = -1;
	}
    s_cell_num = 0;
    return 0;
}

LOCAL int32 AddCellListData(int16 arfcn)
{
    if(s_cell_num >= MN_MAX_NCELL_NUM)
    {
        return -1;
    }
    s_cell_arfcn[s_cell_num] = arfcn;
    s_cell_num++;
    return 0;
}

LOCAL int32 DelCellListData(int16 arfcn)
{
    int16 i = 0;
    if(s_cell_num == 0)
    {
        return -1;
    }
    for(i = 0; i < s_cell_num; i++ )
    {
        if(s_cell_arfcn[i] == arfcn)
        {
            break;
        }
    }
    if(i == s_cell_num)
    {
        return -2;
    }
    
    for(i; i < s_cell_num - 1; i++)/*lint !e520*/
    {
        s_cell_arfcn[i] = s_cell_arfcn[i + 1];
    }
    s_cell_arfcn[s_cell_num - 1] = -1;
    s_cell_num--;
    return s_cell_num;
}

LOCAL void UpdatePSCellListItem(   
                            MMI_WIN_ID_T     win_id,
                            MMI_CTRL_ID_T    ctrl_id,
                            MN_DUAL_SYS_E    dual_sys,
                            BOOLEAN          is_update
                            )
{
    int32 index = 0;
    APP_MN_NCELL_RSSI_CNF_T ncell = {PNULL};
    MN_PHONE_CELL_MEAS_T    scell  = {0};
    ERR_MNPHONE_CODE_E ps_error = 0;    
    wchar wbuffer[GUILIST_STRING_MAX_NUM + 1] = {0};
    char buffer[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint16 length = 0;
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
#ifdef WIN32
    static int32 timer = 0;
#endif
    item_t.item_data_ptr = &item_data;
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;   
    item_t.item_state    = GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    
#ifdef WIN32
    timer++;
    scell.arfcn = 123;
    scell.rxlev = 456;  
    scell.cell_present = TRUE;
    for (index = 0; index < MN_MAX_NCELL_NUM; index ++)      // this is for example for simulator
    {
        ncell.ncell_power[index].cell_present = TRUE;
        ncell.ncell_power[index].arfcn = (index + timer) % 6 ;
        ncell.ncell_power[index].rxlev = index ;
    }
#else
    ps_error = MNPHONE_GetMeasReportEx(dual_sys, &ncell, &scell);
#endif    
    if(scell.cell_present)
    {
        length = sprintf(buffer, "%03d,    %03d,    SCELL", scell.arfcn, scell.rxlev );  
        item_t.user_data    = scell.arfcn;
        MMI_STRNTOWSTR(wbuffer , GUILIST_STRING_MAX_NUM , (const uint8 *)buffer , GUILIST_STRING_MAX_NUM, length);
        item_data.item_content[0].item_data.text_buffer.wstr_len = length;                 
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = wbuffer;

        if(1 == IsForcedCell(scell.arfcn))
        {
            item_data.item_content[1].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {
            item_data.item_content[1].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }
     
        if( is_update )
        {
            GUILIST_ReplaceItem( ctrl_id, &item_t, 0 );
        }
        else
        {
            GUILIST_AppendItem( ctrl_id, &item_t );
        }
    }
    for(index = 0; index < MN_MAX_NCELL_NUM; index++)
    {
        if(ncell.ncell_power[index].cell_present)
        {
            SCI_MEMSET(buffer, 0, GUILIST_STRING_MAX_NUM);
            SCI_MEMSET(wbuffer, 0, GUILIST_STRING_MAX_NUM*2);
            length = sprintf(buffer, "%03d,    %03d,    NCELL", 
                ncell.ncell_power[index].arfcn,
                ncell.ncell_power[index].rxlev );    
            item_t.user_data    = ncell.ncell_power[index].arfcn;
            MMI_STRNTOWSTR(wbuffer , GUILIST_STRING_MAX_NUM , (const uint8 *)buffer , GUILIST_STRING_MAX_NUM, length);
            item_data.item_content[0].item_data.text_buffer.wstr_len = length;                 
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = wbuffer;
            if(1 == IsForcedCell(ncell.ncell_power[index].arfcn))
            {
                item_data.item_content[1].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                item_data.item_content[1].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }

            if( is_update )
            {
                GUILIST_ReplaceItem( ctrl_id, &item_t, index + 1 );
            }
            else
            {
                GUILIST_AppendItem( ctrl_id, &item_t );
            }
         }
    }    
}

LOCAL MMI_RESULT_E EngPSCellOptionMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T   menu_id = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIENG_OPTION_MENU_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUIMENU_GetId(MMIENG_OPTION_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch ( menu_id )
        {
        case ID_ENG_FROCE_CELL:    
#ifndef PRODUCT_DM
            if(1 <= s_cell_num && s_cell_num <= MN_MAX_NCELL_NUM)
            {
                extern void PS_ForceCampon(uint16 sim_no,uint16 arfcn_count,  ///1 -- 6
                                                            uint16 *arfcn_list);
                uint16   sim_num = (int32)MMK_GetWinAddDataPtr( win_id); 
                PS_ForceCampon( sim_num, s_cell_num, s_cell_arfcn);  /*lint !e64*/
            }                 
#endif      
            break;
        case ID_ENG_CANCEL_FORCE_CELL:
#ifndef PRODUCT_DM
            {
            extern void PS_CancelForceCampon( void  );
            PS_CancelForceCampon(); /*lint !e718 !e746 !e628*/
            }
#endif
           
            ClearAllCellListData();           
            break;            
        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
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

LOCAL MMI_RESULT_E EngPSCellWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    //int length = 0; 
    MN_DUAL_SYS_E sim_num = (int32)MMK_GetWinAddDataPtr( win_id);
    
    MMI_RESULT_E result = MMI_RESULT_TRUE; 
    MMI_CTRL_ID_T   ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:      
        GUILIST_SetMaxItem(ctrl_id, PSCELLWIN_LIST_MAX_NUM, FALSE);
        UpdatePSCellListItem(win_id, ctrl_id, sim_num, FALSE);
        if (s_mmieng_netinfo_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_netinfo_timer_id);
            s_mmieng_netinfo_timer_id = 0;
        }
        s_mmieng_netinfo_timer_id = MMK_CreateWinTimer(win_id, 1000, FALSE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        ClearAllCellListData();
        break;
        
    case MSG_CTL_MIDSK:     
    case MSG_CTL_OK:    
    case MSG_KEYDOWN_OK:        
    case MSG_APP_WEB:        
    case MSG_APP_MENU:        
        MMK_CreateWin((uint32 *) MMIENG_PSCELL_OPTION_MENU_WIN_TAB, (ADD_DATA)sim_num);    
        break;
    
    case MSG_CTL_CANCEL:    
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;
        

    case MSG_CTL_LIST_CHECK_CONTENT:   
        {
            uint16 sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
            uint32 arfcn = 0;
            GUILIST_GetItemData( ctrl_id, sel_item_index , &arfcn );
            if(0 == IsForcedCell(arfcn))
            {
                AddCellListData(arfcn);
            }
            else   
            {
                DelCellListData(arfcn);
            }
            UpdatePSCellListItem(win_id, ctrl_id, sim_num, TRUE);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
        }        
        break;


    case MSG_TIMER:    
        if((*(uint8*)param) != s_mmieng_netinfo_timer_id)
        {
            break;
        }
        UpdatePSCellListItem(win_id, ctrl_id, sim_num, TRUE);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
        s_mmieng_netinfo_timer_id = MMK_CreateWinTimer(win_id, 1000, FALSE);
        break;

    case MSG_LOSE_FOCUS:
        if (s_mmieng_netinfo_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_netinfo_timer_id);
            s_mmieng_netinfo_timer_id = 0;
        }
        break;
        
    case MSG_GET_FOCUS: 
        UpdatePSCellListItem(win_id, ctrl_id, sim_num, TRUE);
        if (s_mmieng_netinfo_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_netinfo_timer_id);
            s_mmieng_netinfo_timer_id = 0;
        }
        s_mmieng_netinfo_timer_id = MMK_CreateWinTimer(win_id, 1000, FALSE);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (s_mmieng_netinfo_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_netinfo_timer_id);
            s_mmieng_netinfo_timer_id = 0;
        }        
        ClearAllCellListData();
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


//net info start
LOCAL MMI_RESULT_E ENGNetinfoShowWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint32 group_id = 0;
	uint32 menu_id = 0;
	uint32  dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);	
        MN_RETURN_RESULT_E nv_result =MN_RETURN_SUCCESS;

	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
		        g_dual_sys_eng_win = dual_sys;
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, FALSE);
			MMK_SetAtvCtrl(MMIENG_NET_INFO_SHOW_WIN_ID, MMIENG_NET_INFO_SHOW_CTRL_ID);
		    break;
			
		case MSG_FULL_PAINT:
			break;

		case MSG_KEYDOWN_CANCEL:
		case MSG_CTL_CANCEL:

			MMK_CloseWin(MMIENG_NET_INFO_SHOW_WIN_ID);
			break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_MIDSK:
		case MSG_KEYDOWN_OK:
		case MSG_CTL_OK:
        case MSG_APP_WEB: 
			GUIMENU_GetId(MMIENG_NET_INFO_SHOW_CTRL_ID, &group_id, &menu_id);
			SCI_TRACE_LOW("mymark ENGNetinfoShowWinHandleMsg menuid = %d",menu_id);
 			MMK_CreateWin((uint32*)MMIENG_NW_CAP_SHOW_WIN_TAB,(ADD_DATA)menu_id);
        		break;
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}



/*****************************************************************************/
//  Description : ShowNetInfo
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/ 

typedef enum {
	ENG_STRING_STRING,
	ENG_STRING_NUM,
	ENG_STRING_NUM_DBM,
	ENG_STRING_SUPPOT,
	ENG_STRING_PERCENT
}MMIENG_NETINFO_SHOW_TYPE_E;

  typedef struct  
  {
	  char** item;
	  MMIENG_NETINFO_SHOW_TYPE_E * item_type;
	  uint32 * data;
	  uint32 arrow;
	  uint32 colunm;
  }netinfoshow_t;
  


LOCAL uint16 MMIENGARRAY_StrcatFromStrToUCS2(netinfoshow_t netinfo, uint16  *uint16_str,uint16  *uint16_str_len)
{   
	wchar           unicode_cr = 0x0d;
	MMI_STRING_T    cr_s = {0};
	char temp_ptr[100] = {0};	
	uint16          temp_len = 0;
	uint16          i=0;
	uint32          arrow_colunm = netinfo.colunm*netinfo.arrow;
	//char * cSupport[] = {"Support","not Support"};
	
	cr_s.wstr_ptr = &unicode_cr;
   	cr_s.wstr_len = 1;
	SCI_MEMSET(temp_ptr, 0, 100);
	
	for(i=0;i<netinfo.colunm;i++)
	{
		SCI_TRACE_LOW("mymark netinfo.item[%d] = %s",i ,netinfo.item[i]);
		SCI_TRACE_LOW("mymark netinfo.item_type[%d] = %d",i ,netinfo.item_type[i]);
		SCI_TRACE_LOW("mymark netinfo.data[%d] = %d",arrow_colunm+i,netinfo.data[arrow_colunm+i]);
	/*
	          temp_len =  SCI_STRLEN ( netinfo.item[i] );
                SCI_MEMCPY(temp_ptr,netinfo.item[i], temp_len) ;
		   MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	*/
		switch(netinfo.item_type[i])
		{
			case ENG_STRING_NUM:
				sprintf(temp_ptr,"%s: %d ",netinfo.item[i],netinfo.data[arrow_colunm+i]);
				break;
			case ENG_STRING_NUM_DBM:
				sprintf(temp_ptr,"%s: %ddBm ",netinfo.item[i],netinfo.data[arrow_colunm+i]);
				break;
			case ENG_STRING_SUPPOT:
				if(netinfo.data[arrow_colunm+i])
				{
					sprintf(temp_ptr,"%s: Support ",netinfo.item[i]);
					break;
				}
				else
				{	
					sprintf(temp_ptr,"%s: not Support",netinfo.item[i]);
					break;
				}
			case ENG_STRING_PERCENT:
				sprintf(temp_ptr,"%s: %d%% ",netinfo.item[i],netinfo.data[arrow_colunm+i]);
				break;
		}

	
		temp_len = SCI_STRLEN((char*)temp_ptr);
		MMIAPICOM_StrcatFromStrToUCS2(uint16_str, uint16_str_len, (uint8*)temp_ptr, temp_len);
		MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, uint16_str_len, &cr_s);//换行	
		SCI_TRACE_LOW("mymark MMIENGARRAY_StrcatFromStrToUCS2 %s",temp_ptr);
		SCI_TRACE_LOW("mymark MMIENGARRAY_StrcatFromStrToUCS2 uint16_str_len = %d",*uint16_str_len);
	}

	return *uint16_str_len;
}
LOCAL void ShowNetinfoStatisticsDroptimesInfo(
                           mn_dropTimes_t      dropTimes , BOOLEAN   need_update
                            )
 {
	wchar           unicode_cr = 0x0d;
	MMI_STRING_T    cr_s = {0};
	MMI_STRING_T    text_s = {0};
	uint16          uint16_str[400] = {0};
	uint16          uint16_str_len = 0;
	char            temp_ptr[50] = {0};
	uint16          temp_len = 0;	
	uint8 i=0;
	uint8 j =0;

	const char * item_colunm_name[] =  { "Drop times on 2G ",  "Drop times on 4G "};
	const MMIENG_NETINFO_SHOW_TYPE_E   item_type[] = {ENG_STRING_NUM,ENG_STRING_NUM};
	uint32 item_data[2]={0};

	uint8 cell_colunm = (sizeof(item_colunm_name)/(sizeof(char *)));
	netinfoshow_t adjacentcell_t= {0};

	item_data[0] = dropTimes.dropOn2g ; 
	item_data[1] = dropTimes.dropOn4g ;
	
	cr_s.wstr_ptr = &unicode_cr;
	cr_s.wstr_len = 1;
		
	adjacentcell_t.item=item_colunm_name;
	adjacentcell_t.item_type= item_type;
	adjacentcell_t.data = item_data;
	adjacentcell_t.colunm = cell_colunm;

	SCI_MEMSET(temp_ptr, 0, 50);
	
	adjacentcell_t.arrow = 0;
	MMIENGARRAY_StrcatFromStrToUCS2(adjacentcell_t,uint16_str, &uint16_str_len);	
		
	text_s.wstr_ptr = uint16_str;
	text_s.wstr_len = uint16_str_len;
	GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, need_update);
 }

LOCAL  void ShowNetinfoStatisticsAttachtimeInfo(
                          mn_attachTime_t     attachTime,  BOOLEAN   need_update
                            )
 {
	wchar           unicode_cr = 0x0d;
	MMI_STRING_T    cr_s = {0};
	MMI_STRING_T    text_s = {0};
	uint16          uint16_str[400] = {0};
	uint16          uint16_str_len = 0;
	char            temp_ptr[50] = {0};
	uint16          temp_len = 0;	
	uint8 i=0;
	uint8 j =0;

	const char * item_colunm_name[] =  { "Time on 2G ",  "Time on 4G ", "Time on VOLTE ","Time on Unknow","All the time "};
	const MMIENG_NETINFO_SHOW_TYPE_E   item_type[] = {ENG_STRING_NUM,ENG_STRING_NUM,ENG_STRING_NUM,ENG_STRING_NUM,ENG_STRING_NUM};
	uint32 item_data[5]={0};

	uint8 cell_colunm = (sizeof(item_colunm_name)/(sizeof(char *)));
	netinfoshow_t adjacentcell_t= {0};

	item_data[0] = attachTime.timeOn2g ;
	item_data[1] = attachTime.timeOn4g ;
	item_data[2] = attachTime.timeOnVolte ;
	item_data[3] = attachTime.timeOnUnknow ;
	item_data[4] = attachTime.allTheTime ;

	cr_s.wstr_ptr = &unicode_cr;
	cr_s.wstr_len = 1;
		
	adjacentcell_t.item=item_colunm_name;
	adjacentcell_t.item_type= item_type;
	adjacentcell_t.data = item_data;
	adjacentcell_t.colunm = cell_colunm;

	SCI_MEMSET(temp_ptr, 0, 50);

	adjacentcell_t.arrow = 0;
	MMIENGARRAY_StrcatFromStrToUCS2(adjacentcell_t,uint16_str, &uint16_str_len);	
		
	text_s.wstr_ptr = uint16_str;
	text_s.wstr_len = uint16_str_len;
	GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, need_update);
 }

LOCAL  void ShowNetinfoStatisticsHandoverInfo(                    
	mn_handoverInfo_t   handoverInfo, BOOLEAN   need_update
                            )
 {
	wchar           unicode_cr = 0x0d;
	MMI_STRING_T    cr_s = {0};
	MMI_STRING_T    text_s = {0};
	uint16          uint16_str[600] = {0};
	uint16          uint16_str_len = 0;
	char            temp_ptr[50] = {0};

	uint16          temp_len = 0;
	
	uint8 i=0;
	uint8 j =0;

	const char * item_array_name[] = { "<2G-2G HO> ", "<2G-2G CCO> ", "<2G-4G CCO> ","<4G-2G CCO> ","<2G-4G HO> ","<4G-4G HO> ","<4G-2G HO> ","<SRVCC 2G> "};
	const char * item_colunm_name[] = { "success ",  "fail ",  "passratio ","delay"};
	const MMIENG_NETINFO_SHOW_TYPE_E   item_type[] = {ENG_STRING_NUM,ENG_STRING_NUM,ENG_STRING_PERCENT,ENG_STRING_NUM};
	uint32 item_data[40]={0};

	uint8 cell_arrow = (sizeof(item_array_name)/(sizeof(char *)));
	uint8 cell_colunm = (sizeof(item_colunm_name)/(sizeof(char *)));
	netinfoshow_t adjacentcell_t= {0};
	  
	cr_s.wstr_ptr = &unicode_cr;
    	cr_s.wstr_len = 1;
		
	adjacentcell_t.item=item_colunm_name;
	adjacentcell_t.item_type= item_type;
	adjacentcell_t.data = item_data;
	adjacentcell_t.colunm = cell_colunm;
	
	SCI_MEMSET(temp_ptr, 0, 50);

	for(i=0;i<cell_arrow;i++)
	{
		item_data[i*cell_colunm] = handoverInfo.handoverStatistic[i].success ;
		item_data[i*cell_colunm+1] =  handoverInfo.handoverStatistic[i].fail ;
		item_data[i*cell_colunm+2] =  handoverInfo.handoverStatistic[i].passRatio ;
		item_data[i*cell_colunm+3] =  handoverInfo.handoverStatistic[i].delay ;
	}
	
	for(i=0;i<cell_arrow;i++)
	{
		sprintf(temp_ptr,"%s",item_array_name[i]);
		temp_len = SCI_STRLEN((char*)temp_ptr);
		MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

		adjacentcell_t.arrow = i;
		MMIENGARRAY_StrcatFromStrToUCS2(adjacentcell_t,uint16_str, &uint16_str_len);	
	}
	
    text_s.wstr_ptr = uint16_str;
    text_s.wstr_len = uint16_str_len;
    GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, need_update);
 }
LOCAL void ShowNetinfoStatisticsReselectInfo(
                          mn_reselectInfo_t  reselectInfo, BOOLEAN    need_update
                            )
{   
    wchar           unicode_cr = 0x0d;
    MMI_STRING_T    cr_s = {0};
    MMI_STRING_T    text_s = {0};
    uint16          uint16_str[400] = {0};
    uint16          uint16_str_len = 0;
    char            temp_ptr[50] = {0};

    uint16          temp_len = 0;
	uint8 i=0;
	uint8 j =0;

	const char * item_array_name[] = { "<2G-2G> ",  "<2G-4G> ",  "<4G-2G> ","<4G-4G> "};
	const char * item_colunm_name[] = { "success ",  "fail ",  "passratio ","delay"};
	const MMIENG_NETINFO_SHOW_TYPE_E   item_type[] = {ENG_STRING_NUM,ENG_STRING_NUM,ENG_STRING_PERCENT,ENG_STRING_NUM};
	uint32 item_data[16]={0};
	
	uint8 cell_arrow = (sizeof(item_array_name)/(sizeof(char *)));
	uint8 cell_colunm = (sizeof(item_colunm_name)/(sizeof(char *)));
	netinfoshow_t adjacentcell_t= {0};
	  
	
	for(i=0;i<cell_arrow;i++)
	{
		item_data[i*cell_colunm] = reselectInfo.reselStatistic[i].success ;
		item_data[i*cell_colunm + 1] = reselectInfo.reselStatistic[i].fail ;
		item_data[i*cell_colunm + 2] = reselectInfo.reselStatistic[i].passRatio ;	
		item_data[i*cell_colunm + 3] = reselectInfo.reselStatistic[i].delay ;
		//SCI_TRACE_LOW("mymark ShowNetinfoStatisticsReselectInfo i = %d success=%d,fail=%d,paseeration=%d,delay=%d " ,i,reselectInfo.reselStatistic[i].success, reselectInfo.reselStatistic[i].fail ,reselectInfo.reselStatistic[i].passRatio,reselectInfo.reselStatistic[i].delay);
	}
	
	cr_s.wstr_ptr = &unicode_cr;
    	cr_s.wstr_len = 1;
	adjacentcell_t.item=item_colunm_name;
	adjacentcell_t.item_type= item_type;
	adjacentcell_t.data = item_data;
	adjacentcell_t.colunm = cell_colunm;
	
	SCI_MEMSET(temp_ptr, 0, 50);
	for(i=0;i<cell_arrow;i++)
	{
		sprintf(temp_ptr,"%s",item_array_name[i]);
		temp_len = SCI_STRLEN((char*)temp_ptr);
		MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

		adjacentcell_t.arrow = i;
		MMIENGARRAY_StrcatFromStrToUCS2(adjacentcell_t,uint16_str, &uint16_str_len);	
	}
	
    text_s.wstr_ptr = uint16_str;
    text_s.wstr_len = uint16_str_len;
    GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, need_update);
}


LOCAL void ShowOutFieldNetWorkInfo(
                            mn_capInfoLte_t   capInfoLte,  BOOLEAN need_update
                            )
{   
	wchar           unicode_cr = 0x0d;
	MMI_STRING_T    cr_s = {0};
	MMI_STRING_T    text_s = {0};
	uint16          uint16_str[400] = {0};
	uint16          uint16_str_len = 0;
	char            temp_ptr[50] = {0};

	uint16          temp_len = 0;
	uint8 i=0;
	uint8 j =0;

	const char * item_colunm_name[] = { "Bsrvcc ",  "EMFBI ",  "IPV6 ","ASRVCC ","ESRVCC "};
	const MMIENG_NETINFO_SHOW_TYPE_E   item_type[] = {ENG_STRING_SUPPOT,ENG_STRING_SUPPOT,ENG_STRING_SUPPOT,ENG_STRING_SUPPOT,ENG_STRING_SUPPOT};

	uint32 item_data[5]={0};
	
	uint8 cell_colunm = (sizeof(item_colunm_name)/(sizeof(char *)));
	netinfoshow_t adjacentcell_t= {0};
	  
	cr_s.wstr_ptr = &unicode_cr;
    	cr_s.wstr_len = 1;
	item_data[0] = capInfoLte.bsrvccFlag;
	item_data[1] = capInfoLte.emfbiFlag;
	item_data[2] = capInfoLte.ipv6Flag;
	item_data[3] = capInfoLte.asrvccFlag;
	item_data[4] = capInfoLte.esrvccFlag;
	adjacentcell_t.item=item_colunm_name;
	adjacentcell_t.item_type= item_type;
	adjacentcell_t.data = item_data;
	adjacentcell_t.colunm = cell_colunm;
/*
	for(i=0;i<5;i++)
	{
		SCI_TRACE_LOW("mymark item_data[%d] = %d",i,item_data[i]);
	}
*/
	SCI_MEMSET(temp_ptr, 0, 50);
	
	if(capInfoLte.lteCipherFlag)
	{
		sprintf(temp_ptr,"4G Cipher %s","Support");
	}
	else
	{
		sprintf(temp_ptr,"4G Cipher %s","not Support");
	}
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	
	adjacentcell_t.arrow = 0;
	MMIENGARRAY_StrcatFromStrToUCS2(adjacentcell_t,uint16_str, &uint16_str_len);	

    text_s.wstr_ptr = uint16_str;
    text_s.wstr_len = uint16_str_len;
    GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, need_update);
}

LOCAL void ShowBetweenAdjacentCell_2G_Info(
                            uint16      gsmNcellNum,mn_gsmNcell_t*   gsmNcell,  BOOLEAN need_update
                            )
{   
	wchar           unicode_cr = 0x0d;
	MMI_STRING_T    cr_s = {0};
	MMI_STRING_T    text_s = {0};
	uint16          uint16_str[400] = {0};
	uint16          uint16_str_len = 0;
	char            temp_ptr[50] = {0};

	uint16          temp_len = 0;
	uint8 i=0;
	uint8 j =0;

	const char * item_colunm_name[] = { "arfcn ",  "bsic ",  "rxlev "};
	const MMIENG_NETINFO_SHOW_TYPE_E   item_type[] = {ENG_STRING_NUM,ENG_STRING_NUM,ENG_STRING_NUM_DBM};
	uint32 item_data[15]={0};

	uint8 cell_arrow = gsmNcellNum;
	uint8 cell_colunm = (sizeof(item_colunm_name)/(sizeof(char *)));
	netinfoshow_t adjacentcell_t= {0};

	cr_s.wstr_ptr = &unicode_cr;
	cr_s.wstr_len = 1;

	for(i=0;i<cell_arrow;i++)
	{
		item_data[i*cell_colunm]     = gsmNcell[i].arfcn;
		item_data[i*cell_colunm+1] = gsmNcell[i].bsic;
		item_data[i*cell_colunm+2] = gsmNcell[i].rxlev;
	}
	
	SCI_TRACE_LOW("[ENG] ShowBetweenAdjacentCell_2G_Info gsmNcellNum=%d ",gsmNcellNum);

	adjacentcell_t.item=item_colunm_name;
	adjacentcell_t.item_type= item_type;
	adjacentcell_t.data = item_data;
	adjacentcell_t.colunm = cell_colunm;

	SCI_MEMSET(temp_ptr, 0, 50);

	if(gsmNcellNum != 0)
	{
		for(i=0;i<cell_arrow;i++)
		{
			sprintf(temp_ptr,"<cell_%d>",i);
			temp_len = SCI_STRLEN((char*)temp_ptr);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
			MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

			adjacentcell_t.arrow = i;
			MMIENGARRAY_StrcatFromStrToUCS2(adjacentcell_t,uint16_str, &uint16_str_len);	
		}
	}
	else
	{
			sprintf(temp_ptr,"Cell_Num = 0",i);
			temp_len = SCI_STRLEN((char*)temp_ptr);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
			MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	}

	text_s.wstr_ptr = uint16_str;
	text_s.wstr_len = uint16_str_len;
	GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, need_update);
}


LOCAL void ShowAdjacentCellInfo( uint16  lteNcellNum,mn_lteNcell_t*   lteNcell,  BOOLEAN need_update)
{   
	wchar           unicode_cr = 0x0d;
	MMI_STRING_T    cr_s = {0};
	MMI_STRING_T    text_s = {0};
	uint16          uint16_str[400] = {0};
	uint16          uint16_str_len = 0;
	char            temp_ptr[50] = {0};

	uint16          temp_len = 0;
	uint8 i=0;
	uint8 j =0;

	const char * item_name[2] = { "Frequency ",  "PCI "};
	const MMIENG_NETINFO_SHOW_TYPE_E   item_type[2] = {ENG_STRING_NUM,ENG_STRING_NUM};
	uint32 item_data[10]={0};
	uint8 cell_arrow = lteNcellNum;
	uint8 cell_colunm = (sizeof(item_name)/(sizeof(char *)));

	netinfoshow_t adjacentcell_t= {0};

	cr_s.wstr_ptr = &unicode_cr;
	cr_s.wstr_len = 1;


	for(i=0;i<cell_arrow;i++)
	{
		item_data[i*cell_colunm]     = lteNcell[i].frequency;
		item_data[i*cell_colunm+1] = lteNcell[i].pcid;
	}

	adjacentcell_t.item=item_name;
	adjacentcell_t.item_type= item_type;
	adjacentcell_t.data = item_data;
	adjacentcell_t.colunm = cell_colunm;

	SCI_MEMSET(temp_ptr, 0, 50);
	if(lteNcellNum != 0)
	{
		for(i=0;i<cell_arrow;i++)
		{
			sprintf(temp_ptr,"<cell_%d>",i);
			temp_len = SCI_STRLEN((char*)temp_ptr);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
			MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

			adjacentcell_t.arrow = i;
			MMIENGARRAY_StrcatFromStrToUCS2(adjacentcell_t,uint16_str, &uint16_str_len);	
			
			sprintf(temp_ptr,"RSRP: %ddBM",lteNcell[i].rsrp);
			temp_len = SCI_STRLEN((char*)temp_ptr);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
			MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

			sprintf(temp_ptr,"RSRQ: %ddBM",lteNcell[i].rsrq);
			temp_len = SCI_STRLEN((char*)temp_ptr);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
			MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
		}
	}
	else
	{
		sprintf(temp_ptr,"Cell_Num = 0");
		temp_len = SCI_STRLEN((char*)temp_ptr);
		MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	}
	

	text_s.wstr_ptr = uint16_str;
	text_s.wstr_len = uint16_str_len;
	GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, need_update);
}


/*****************************************************************************/
//  Description : ShowServingCellInfo
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void ShowServingCellInfo(
                            mn_lteScell_t lteScell,  BOOLEAN need_update
                            )
{   
	wchar           unicode_cr = 0x0d;
	MMI_STRING_T    cr_s = {0};
	MMI_STRING_T    text_s = {0};
	MMI_STRING_T plmn_name = {0};
	uint16          uint16_str[400] = {0};
	uint16          uint16_str_len = 0;
	uint16          temp_len = 0;
	uint8 i = 0;
	char temp_ptr[100] = {0};
	uint16 mnc =0;
	SCI_MEMSET(temp_ptr, 0, 100);

	cr_s.wstr_ptr = &unicode_cr;
	cr_s.wstr_len = 1;
	
	SCI_TRACE_LOW("[MMIENG] lteScell.plmn = [%2X] [%2X] [%2X] ",lteScell.plmn[0],lteScell.plmn[1],lteScell.plmn[2]);
	sprintf(temp_ptr,"PLMN: %d%d%d ",lteScell.plmn[0]&0x0F,(lteScell.plmn[0]&0xF0)>>4,lteScell.plmn[1]&0x0F);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);

	if(0xF == ((lteScell.plmn[1]&0xF0)>>4))
	{
		mnc = (lteScell.plmn[2]&0x0F)*10 + ((lteScell.plmn[2]&0xF0)>>4);
		sprintf(temp_ptr," %d%d ",mnc/10,mnc%10);
	}
	else
	{
		mnc = (lteScell.plmn[2]&0x0F)*100 + ((lteScell.plmn[2]&0xF0)>>4)*10+((lteScell.plmn[1]&0xF0)>>4);	
		sprintf(temp_ptr," %d ",mnc);
	}

	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	SCI_TRACE_LOW("[MMIENG] lteScell.imsi[0] = %d ",lteScell.imsi[0]);

	if(lteScell.imsi[0]!= 0)
	{
		sprintf(temp_ptr,"IMSI: ");
		temp_len = SCI_STRLEN((char*)temp_ptr);
		MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		for(i=1;i<=lteScell.imsi[0];i++)
		{
			sprintf(temp_ptr,"%d",lteScell.imsi[i]);
			temp_len = SCI_STRLEN((char*)temp_ptr);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		}
		MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	}
	else
	{
		sprintf(temp_ptr,"IMSI: None ");
		temp_len = SCI_STRLEN((char*)temp_ptr);
		MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	}

	sprintf(temp_ptr,"Network: ");
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMI_GetLabelTextByLang(MMIAPIPHONE_GetCarrierNameId(g_dual_sys_eng_win), &plmn_name);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &plmn_name);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	 
	if(lteScell.roamingFlag)
	{
		sprintf(temp_ptr,"Roaming: %s","roaming");
	}
	else
	{
		sprintf(temp_ptr,"Roaming: %s","Non-roaming");
	}
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"BAND: %d",lteScell.bandInfo);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"EARFCN_DL: %d",lteScell.dlEarfcn);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"PCI: %d",lteScell.pcid);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"RSRP: %ddBm",lteScell.rsrp);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"RSRQ: %ddBm",lteScell.rsrq);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"TS MODE: %d",lteScell.tsMode);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"BAND WIDTH: %d",lteScell.bandwidth);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"SA: %d",lteScell.SA);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"SSP: %d",lteScell.SSP);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"SINR: %d",lteScell.SINR);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"UL_MCS: %d",lteScell.ulMcs);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"DL_MCS: %d",lteScell.dlMcs);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"UL_BLER: %d",lteScell.ulBler);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"DL_BLER: %d",lteScell.dlBler);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"E-NodeB ID: %d",lteScell.enodeBID);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"CELL ID: %d",lteScell.cellId);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	text_s.wstr_ptr = uint16_str;
	text_s.wstr_len = uint16_str_len;
	GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, need_update);
	}

/*****************************************************************************/
//  Description : ShowServingCellInfo
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void ShowServingUECapInfo(
                            mn_ueCapGsm_t ueCapabilityGsm,  BOOLEAN need_update
                            )
{   
	wchar           unicode_cr = 0x0d;
	MMI_STRING_T    cr_s = {0};
	MMI_STRING_T    text_s = {0};
	MMI_STRING_T plmn_name = {0};
	uint16          uint16_str[400] = {0};
	uint16          uint16_str_len = 0;
	uint16          temp_len = 0;
	uint8 i = 0;
	char temp_ptr[100] = {0};
	uint16 mnc =0;

	MN_IMSI_T tmp_imsi = MNSIM_GetImsiEx(g_dual_sys_eng_win);
	SCI_MEMSET(temp_ptr, 0, 100);

	cr_s.wstr_ptr = &unicode_cr;
	cr_s.wstr_len = 1;


	SCI_TraceLow("ENG Plmn [%2x] [%2x] [%2x]",ueCapabilityGsm.Plmn[0],ueCapabilityGsm.Plmn[1],ueCapabilityGsm.Plmn[2]);
	
	sprintf(temp_ptr,"PLMN: %d%d%d ",ueCapabilityGsm.Plmn[0]&0x0F,(ueCapabilityGsm.Plmn[0]&0xF0)>>4,ueCapabilityGsm.Plmn[1]&0x0F);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);

	if(0xF == ((ueCapabilityGsm.Plmn[1]&0xF0)>>4))
	{
		mnc = (ueCapabilityGsm.Plmn[2]&0x0F)*10 + ((ueCapabilityGsm.Plmn[2]&0xF0)>>4);
		sprintf(temp_ptr," %d%d ",mnc/10,mnc%10);
	}
	else
	{
		mnc = (ueCapabilityGsm.Plmn[2]&0x0F)*100 + ((ueCapabilityGsm.Plmn[2]&0xF0)>>4)*10+((ueCapabilityGsm.Plmn[1]&0xF0)>>4);	
		sprintf(temp_ptr," %d ",mnc);
	}

	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	if(tmp_imsi.imsi_len != 0)
	{
		sprintf(temp_ptr,"IMSI: ");
		temp_len = SCI_STRLEN((char*)temp_ptr);
		MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		SCI_MEMSET(temp_ptr, 0, 100);
		MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)tmp_imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, temp_ptr);
		SCI_TraceLow("ENG imsi_val[%s]",temp_ptr+1);
		temp_len = SCI_STRLEN((char*)temp_ptr) -1;
		MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)(temp_ptr+1), temp_len);
		MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	}
	else
	{
		sprintf(temp_ptr,"IMSI: None ");
		temp_len = SCI_STRLEN((char*)temp_ptr);
		MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	}

	sprintf(temp_ptr,"Network: ");
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMI_GetLabelTextByLang(MMIAPIPHONE_GetCarrierNameId(g_dual_sys_eng_win), &plmn_name);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &plmn_name);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	 
	if(ueCapabilityGsm.Roaming)
	{
		sprintf(temp_ptr,"Roaming: %s","roaming");
	}
	else
	{
		sprintf(temp_ptr,"Roaming: %s","Non-roaming");
	}
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行


	sprintf(temp_ptr,"CellID: ");
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	for(i=0;i<=1;i++)
	{
		sprintf(temp_ptr,"%d",ueCapabilityGsm.CellId[i]);
		temp_len = SCI_STRLEN((char*)temp_ptr);
		MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	}
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"Arfcn: %d",ueCapabilityGsm.Arfcn);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"Bsic: %d",ueCapabilityGsm.Bsic);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	sprintf(temp_ptr,"Rla: %d",ueCapabilityGsm.Rla);
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	text_s.wstr_ptr = uint16_str;
	text_s.wstr_len = uint16_str_len;
	GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, need_update);
}


/********************************************************************************
 NAME:          ENGNWCAPShowWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        zhaohui
 DATE:          2008.10.24
********************************************************************************/
LOCAL MMI_RESULT_E ENGNWCAPShowWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	uint8 buf[1024] = {0};
	MMI_STRING_T text = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint32 menu_id= (uint32)MMK_GetWinAddDataPtr(win_id);
	wchar w_buf[1024] = {0};
	MN_NET_INFO_T pNetInfo;
        
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
		SCI_TRACE_LOW("[ENG] ENGNWCAPShowWinHandleMsg menuid = %d",menu_id);
		MMK_SetAtvCtrl(win_id, MMIENG_TEXTBOX1_CTRL_ID);
	
		MNPHONE_FacGetNetInfo(&pNetInfo,g_dual_sys_eng_win );
		switch(menu_id)
		{
			case ID_ENG_SERVINGCELL:		
				
				if(MN_GMMREG_RAT_GPRS == MNPHONE_GetCurrentRAT(g_dual_sys_eng_win))
				{		
					SCI_TRACE_LOW("[ENG] ENGNWCAPShowWinHandleMsg line[%d] ",__LINE__);
					ShowServingUECapInfo( pNetInfo.ueCapabilityGsm,FALSE);
				}
				else
				{
					ShowServingCellInfo( pNetInfo.nwCapabilityLte.lteScell,FALSE);
				}
				//text.wstr_len = GetAdcCalibrateInfoStr(buf, 1024);
				GUIWIN_SetTitleTextId(MMIENG_NW_CAP_SHOW_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_SERVING_CELL, FALSE);
				break;
			case ID_ENG_ADJACENTCELL:	
				ShowAdjacentCellInfo(pNetInfo.nwCapabilityLte.lteNcellNum,pNetInfo.nwCapabilityLte.lteNcell,FALSE);
				GUIWIN_SetTitleTextId(MMIENG_NW_CAP_SHOW_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_THE_ADJACENT_CELL, FALSE);
				break;
			case ID_ENG_BETWEEN_ADJACENTCELL_2G:
				ShowBetweenAdjacentCell_2G_Info(pNetInfo.nwCapabilityLte.gsmNcellNum,pNetInfo.nwCapabilityLte.gsmNcell,FALSE);
				GUIWIN_SetTitleTextId(MMIENG_NW_CAP_SHOW_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_BETWEEN_ADJACENT_CELL_2G, FALSE);
				break;
			case ID_ENG_OUTFIELD_NETWORK_INFORMATION:
				ShowOutFieldNetWorkInfo(pNetInfo.nwCapabilityLte.capInfoLte,FALSE);
				GUIWIN_SetTitleTextId(MMIENG_NW_CAP_SHOW_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_OUTFIELD_NETWORK_INFORMATION, FALSE);
				break;			
		}

        if (s_mmieng_netinfo_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_netinfo_timer_id);
            s_mmieng_netinfo_timer_id = 0;
        }
      //  s_mmieng_netinfo_timer_id = MMK_CreateWinTimer(win_id, 1000, FALSE);
		
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
		
    case MSG_TIMER:    
        if((*(uint8*)param) != s_mmieng_netinfo_timer_id)
        {
            break;
        }
     // MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
     //   s_mmieng_netinfo_timer_id = MMK_CreateWinTimer(win_id, 1000, FALSE);
        break;

    case MSG_LOSE_FOCUS:
        if (s_mmieng_netinfo_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_netinfo_timer_id);
            s_mmieng_netinfo_timer_id = 0;
        }
        break;
        
    case MSG_GET_FOCUS: 

        if (s_mmieng_netinfo_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_netinfo_timer_id);
            s_mmieng_netinfo_timer_id = 0;
        }
        //s_mmieng_netinfo_timer_id = MMK_CreateWinTimer(win_id, 1000, FALSE);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (s_mmieng_netinfo_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_netinfo_timer_id);
            s_mmieng_netinfo_timer_id = 0;
        }        
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;

    }
    return (result);
}

//netinfo statistics

LOCAL MMI_RESULT_E ENGNetinfoStatisticsWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint32 group_id = 0;
	uint32 menu_id = 0;
	uint32  dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);	
        MN_RETURN_RESULT_E nv_result =MN_RETURN_SUCCESS;

	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
			g_dual_sys_eng_win = dual_sys;
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, FALSE);
			MMK_SetAtvCtrl(MMIENG_NETINFO_STATISTICS_WIN_ID, MMIENG_NETINFO_STATISTICS_CTRL_ID);
		    break;
			
		case MSG_FULL_PAINT:
			break;

		case MSG_KEYDOWN_CANCEL:
		case MSG_CTL_CANCEL:

			MMK_CloseWin(MMIENG_NETINFO_STATISTICS_WIN_ID);
			break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_MIDSK:
		case MSG_KEYDOWN_OK:
		case MSG_CTL_OK:
        case MSG_APP_WEB: 
			GUIMENU_GetId(MMIENG_NETINFO_STATISTICS_CTRL_ID, &group_id, &menu_id);
			SCI_TRACE_LOW("mymark ENGNetinfoStatisticsWinHandleMsg menuid = %d",menu_id);
 			MMK_CreateWin((uint32*)MMIENG_NETINFO_STATISTICS_HANDLE_WIN_TAB,(ADD_DATA)menu_id);
        		break;
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}


/********************************************************************************
 NAME:          ENGNWCAPShowWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        zhaohui
 DATE:          2008.10.24
********************************************************************************/
LOCAL MMI_RESULT_E ENG_Netinfo_Statistics_WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[1024] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 menu_id= (uint32)MMK_GetWinAddDataPtr(win_id);
    wchar w_buf[1024] = {0};
	MN_NET_INFO_STATISTIC_T pNetinfoStatistic;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
		SCI_TRACE_LOW("mymark ENG_Netinfo_Statistics_WinHandleMsg menuid = %d",menu_id);
		  MMK_SetAtvCtrl(win_id, MMIENG_TEXTBOX1_CTRL_ID);
		MNPHONE_FacGetNetInfoStatistic(&pNetinfoStatistic, g_dual_sys_eng_win);
		switch(menu_id)
		{
			case ID_ENG_NETINFO_RESELECT:
				ShowNetinfoStatisticsReselectInfo(pNetinfoStatistic.reselectInfo ,FALSE);
				GUIWIN_SetTitleTextId(MMIENG_NETINFO_STATISTICS_HANDLE_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_NETINFO_RESELECT, FALSE);
				break;
			case ID_ENG_NETINFO_HANDOVER:
				ShowNetinfoStatisticsHandoverInfo(pNetinfoStatistic.handoverInfo,FALSE);
				GUIWIN_SetTitleTextId(MMIENG_NETINFO_STATISTICS_HANDLE_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_NETINFO_HANDOVER, FALSE);
				break;
			case ID_ENG_NETINFO_ATTACHTIME:
				ShowNetinfoStatisticsAttachtimeInfo(pNetinfoStatistic.attachTime,FALSE);
				GUIWIN_SetTitleTextId(MMIENG_NETINFO_STATISTICS_HANDLE_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_NETINFO_ATTACHTIME, FALSE);
				break;
			case ID_ENG_NETINFO_DROPTIMES:
				ShowNetinfoStatisticsDroptimesInfo(pNetinfoStatistic.dropTimes,FALSE);
				GUIWIN_SetTitleTextId(MMIENG_NETINFO_STATISTICS_HANDLE_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_NETINFO_DROPTIMES, FALSE);
				break;	
		}
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

//net info end

/********************************************************************************
 NAME:          GetPhoneInfoStr
 DESCRIPTION:   
 PARAM IN:      phone_buf - 
                buf_len - 
 PARAM OUT:     phone info string len
 AUTHOR:        allen
 DATE:          2004.09.25
********************************************************************************/
LOCAL int GetPhoneInfoStr(uint8* phone_buf, uint32 buf_len)
{
    SCI_DATE_T cur_date = {31, 12, 2004, 6};
    SCI_TIME_T cur_time = {59, 59, 23};
       
       SCI_DATE_T  last_date={0,0,0,0};
       SCI_TIME_T   last_time={0,0,0};
    MN_IMEI_T imei = {0};
    char tmp_buf1[20] = {0};
    char tmp_buf2[20] = {0};
       MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    uint32 offset = 0;
    uint32 freespace = 0;
    uint32 totalspace = 0;
    uint32 voltage = 0;
    uint len = 0;
    uint i = 0;
    int32 j = 0;
    MMIENG_TIME_T s_time_from_turnon = {0};
    uint32  s_sec = 0;
#if defined(PLATFORM_SC7702)
#ifndef _WIN32    
    CHIP_ADIE_ID_E adie_type = ADIE_NOAC;
#endif
#endif
    char*          temp_ptr = PNULL;
    uint16          temp_len = 0;
#ifndef _WIN32    
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
#endif
#ifdef MMI_DUAL_BATTERY_SUPPORT
    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
#endif
    if(PNULL == phone_buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetPhoneInfoStr phone_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4039_112_2_18_2_17_49_131,(uint8*)"");
        return -1;
    }
    
    //SCI_TRACE_LOW:"mmieng_win.c GetPhoneInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4043_112_2_18_2_17_49_132,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
    //手机开机到现在的时间，不过1193小时后会溢出
    s_sec = SCI_GetTickCount()/1000;
    s_time_from_turnon.hour = s_sec/3600;
    s_time_from_turnon.min = (s_sec/60)%60;
    s_time_from_turnon.sec = s_sec%60;  

    //////////////
    // Date Time
    //////////////
    MMI_MEMCPY(phone_buf, buf_len, "<Date Time>", 11, 11);
    offset = 11;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    if (ERR_TM_NONE == TM_GetSysDate(&cur_date))
    {
        MMIAPISET_FormatDateStrByDateStyle(cur_date.year, cur_date.mon, cur_date.mday,'-',(uint8*)tmp_buf1,20);
        MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, 10);
        offset += 10;
    }
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    if (ERR_TM_NONE == TM_GetSysTime(&cur_time))
    {
        MMIAPISET_FormatTimeStrByTime(cur_time.hour, cur_time.min,(uint8*)tmp_buf1,20);
        MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, 5);
        offset += 5;
    }
    phone_buf[offset] = CR_CHAR;
    offset += 1;

    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<Nitz Time Zone> ", 16, 16);
    offset += 16;
    {
        int timezone_nitz = MMIAPIPHONE_GetTimeZoneFromNitz();  //count by 15minutes.
        int timezone_remainder = 0;

        if (timezone_nitz <0)
            {
                phone_buf[offset] = '-';
                offset += 1;
                timezone_nitz = 0-timezone_nitz;
            }
        else
            {
                phone_buf[offset] = '+';
                offset += 1;
            }
        timezone_remainder =timezone_nitz- (timezone_nitz/4)*4;
        timezone_nitz = timezone_nitz/4;
        if(timezone_nitz>=10)   
            {
            phone_buf[offset] ='0'+timezone_nitz/10;
            timezone_nitz = timezone_nitz-timezone_nitz/10*10;
            offset += 1;
        }
        phone_buf[offset] = '0'+timezone_nitz;
        offset += 1;
        if (timezone_remainder ==1)
            {
                phone_buf[offset] ='.';
                phone_buf[offset+1] ='2';
                phone_buf[offset+2] ='5';
                offset+=3;
            }
        else if (timezone_remainder==2)
            {
                phone_buf[offset] ='.';
                phone_buf[offset+1] ='5';
                offset+=2;            
            }
        else if (timezone_remainder == 3)
            {
                phone_buf[offset] ='.';
                phone_buf[offset+1] ='7';
                phone_buf[offset+2] ='5';
                offset+=3;
            }
        /*
        phone_buf[offset]=CR_CHAR;
        offset++;        
        if(MMIAPIPHONE_GetDaylightFromNitz())
        {
            
            strcpy(phone_buf+offset, "Summer");
            offset+=strlen("Summer");
        }else
        {
            strcpy(phone_buf+offset, "Winter");
            offset+=strlen("Winter");        
        }*/
        
    }
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    //////////////
    // Time From Turn on,
    //////////////
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<Time from Turn on>", 19, 19);
    offset += 19;
    phone_buf[offset] = CR_CHAR;
    offset += 1;    
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "hour:", 5, 5);
    offset += 5;
    sprintf(tmp_buf1, "%d", s_time_from_turnon.hour);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, SCI_STRLEN(tmp_buf1));/*lint !e666*/
    offset += SCI_STRLEN(tmp_buf1);
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "min:", 4, 4);
    offset += 4;
    sprintf(tmp_buf1, "%d", s_time_from_turnon.min);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, SCI_STRLEN(tmp_buf1));/*lint !e666*/
    offset += SCI_STRLEN(tmp_buf1);
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    //////////////
    // EFS Status
    //////////////
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<EFS Status>", 12, 12);/*lint !e666*/
    offset += 12;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "total:", 6, 6);/*lint !e666*/
    offset += 6;
    #ifndef _WIN32
    freespace = EFS_GetFreeSpace();
    totalspace = freespace + EFS_GetUsedSpace();
    #endif
    sprintf(tmp_buf1, "%ld", totalspace);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, SCI_STRLEN(tmp_buf1));/*lint !e666*/
    offset += SCI_STRLEN(tmp_buf1);
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "free:", 5, 5);
    offset += 5;
    sprintf(tmp_buf1, "%ld", freespace);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, SCI_STRLEN(tmp_buf1));/*lint !e666*/
    offset += SCI_STRLEN(tmp_buf1);
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    //////////////
    // Battery
    //////////////
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<Battery>", 9, 9);
    offset += 9;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "voltage:", 8, 8);
    offset += 8;
#ifndef _WIN32
    #ifdef MMI_DUAL_BATTERY_SUPPORT
    voltage=dbat_info->chgmng_info.bat_statistic_vol;
    #else
    voltage = p_chgmng_info->bat_statistic_vol;
    #endif
#endif

    sprintf(tmp_buf1, "%6.2f", (double)voltage / 1000);
    len = SCI_STRLEN(tmp_buf1);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, len);
    offset += len;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "cur voltage:", 12, 12);
    offset += 12;
#ifndef _WIN32
    #ifdef MMI_DUAL_BATTERY_SUPPORT
    voltage=dbat_info->chgmng_info.bat_cur_vol;
    #else
    voltage = p_chgmng_info->bat_cur_vol;
    #endif
#endif
    sprintf(tmp_buf1, "%6.2f", (double)voltage / 1000);
    len = SCI_STRLEN(tmp_buf1);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, len);
    offset += len;
    phone_buf[offset] = CR_CHAR;
    offset += 1;

    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "charging voltage:", 17, 17);
    offset += 17;
#ifndef _WIN32
    #ifdef MMI_DUAL_BATTERY_SUPPORT
    voltage=dbat_info->chgmng_info.chg_vol ;
    #else
    voltage = p_chgmng_info->chg_vol;
    #endif
#endif
    sprintf(tmp_buf1, "%6.2f", (double)voltage / 1000);
    len = SCI_STRLEN(tmp_buf1);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, len);
    offset += len;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "charging cvalue:", 16, 16);
    offset += 16;
#ifndef _WIN32
    #ifdef MMI_DUAL_BATTERY_SUPPORT
    voltage=dbat_info->chgmng_info.charging_current  ;
    #else
    voltage = p_chgmng_info->charging_current ;
    #endif
#endif
    sprintf(tmp_buf1, "%6.2f", (double)voltage / 1000);
    len = SCI_STRLEN(tmp_buf1);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, len);
    offset += len;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    
    //////////////
    // IMEI
    //////////////
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<IMEI>", 6, 6);
    offset += 6;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    for(j=0;j < MMI_DUAL_SYS_MAX; j++)
    {
         if(!MNNV_GetIMEIEx(MN_DUAL_SYS_1 + j, imei))
         {
            SCI_MEMSET(imei, 0, sizeof(MN_IMEI_T));
         }
        
        SCI_MEMSET(tmp_buf1, 0, 18);
        for (i = 0; i < 8; i++) 
        {
            tmp_buf1[2 * i] = (imei[i]) & 0x0F;
            tmp_buf1[2 * i + 1] = imei[i] >> 4;
        }
        sprintf(    tmp_buf2,
                "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
                tmp_buf1[1], tmp_buf1[2], tmp_buf1[3],
                tmp_buf1[4], tmp_buf1[5], tmp_buf1[6], tmp_buf1[7],
                tmp_buf1[8], tmp_buf1[9], tmp_buf1[10], tmp_buf1[11],
                tmp_buf1[12], tmp_buf1[13], tmp_buf1[14], tmp_buf1[15]
            );
        MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf2, 19, 15);
        offset += 15;
        phone_buf[offset] = CR_CHAR;
        offset += 1;
    }
    //////////////
    // CHIP ID
    //////////////
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<CHIP TYPE>", 11, 11);
    offset += 11;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
#ifndef _WIN32
    temp_ptr = CHIP_GetChipInfo();      /*lint !e605*/
#else
    temp_ptr = "SC6600R1";  
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, temp_ptr, temp_len, temp_len);
    offset += temp_len;
	
#if defined(PLATFORM_SC7702)
#ifndef _WIN32    
    adie_type = CHIP_GetADIEChipID();
    if (ADIE_AC == adie_type)
    {
        temp_ptr = "ADIE_AC"; 
    }
    else
    {
        temp_ptr = "ADIE_NOAC"; 
    }
#else
        temp_ptr = "ADIE_NOAC"; 
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, temp_ptr, temp_len, temp_len);
    offset += temp_len;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
#endif

    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<LAST POWEROFF TIME>", 20, 20);
    offset += 20;
    phone_buf[offset] = CR_CHAR;
    offset += 1;

    MMINV_READ(MMIENG_NV_LAST_DATA_ID, &last_date,return_value);   

    MMIAPISET_FormatDateStrByDateStyle(last_date.year, last_date.mon, last_date.mday,'-',(uint8*)tmp_buf1,20);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, 10);
    offset += 10;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMINV_READ(MMIENG_NV_LAST_TIME_ID, &last_time,return_value);    

    MMIAPISET_FormatTimeStrByTime(last_time.hour, last_time.min,(uint8*)tmp_buf1,20);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, 5);
    offset += 5;
    phone_buf[offset] = CR_CHAR;
    offset += 1;

    return (offset);
}

/********************************************************************************
 NAME:          EngShowPhoneWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.08
********************************************************************************/
LOCAL MMI_RESULT_E EngShowPhoneWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[521] = {0};
      wchar wstr[521] = {0};
      char* str = PNULL;
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:

        str = (char*)buf;
        text.wstr_len = GetPhoneInfoStr(buf, 520);
        text.wstr_ptr = wstr;
	  //SCI_ALLOCA(200*1024);
        MMI_STRNTOWSTR( wstr, 520, (uint8*)str, text.wstr_len, text.wstr_len );
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
        MMK_SetAtvCtrl(MMIENG_SHOWPHONE_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:     
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_SHOWPHONE_WIN_ID);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          GetAdcCalibrateInfoStr
 DESCRIPTION:   
 PARAM IN:      adc_info_buf - 
                buf_len - 
 PARAM OUT:     adc info string len
 AUTHOR:        zhaohui
 DATE:          2008.10.24
********************************************************************************/
LOCAL int32 GetAdcCalibrateInfoStr(uint8* adc_info_buf, uint16 buf_len)
{
//     SCI_DATE_T calibrate_date = {0};
//     SCI_TIME_T calibrate_time = {0};
    char tmp_buf1[20] = {0};
//     const int32 temp_buf_length = 20;
    int32 length = 0 ;
    int32 result = 0;
    int32 offset = 0;
    
    uint32 adc_reserved_info = 0;
	uint32 adc_reserved_info_temp=0;
    int32 i =0 ;
#ifdef PLATFORM_SC7702 
    uint32 w_adc_reserved_info = 0;
	uint32 temp_info = 0;
	uint32 w_cali_bit = 0x0003c000;
#endif
    const char *item[] = { "GSM_calib_afc -bit0 ",  "GSM850_calib_agc -bit1 ",  "GSM850_calib_apc -bit2 ", "GSM900_calib_agc -bit3 ", 
                    "GSM900_calib_apc -bit4", "DCS1800_calib_agc -bit5", "DCS1800_calib_apc -bit6",  "PCS1900_calib_agc -bit7", 
                    "PCS1900_calib_apc -bit8", "GSM_FT_flag -bit9",
                    "LTE_calib_afc -bit0", "LTE_TDD_calib_agc -bit1", "LTE_TDD_calib_apc -bit2", "reserved",
#ifdef PRODUCT_DM
#ifdef PLATFORM_SC7702
                    "W BAND 1 ", "W BAND 2 ", "W BAND 5 ", 
                    "W BAND 8 ", "ANT Test ", "W Final Test ",
#else
                    "TD_AFC ", "TD APC ", "TD AGC ", 
                    "TD Final Test ", "ANT Test ", "CMMB Test ",
#endif                   
#else
                    "LTE_FDD_calib_agc -bit4","LTE_FDD_calib_apc -bit5", "reserved ", 
                    "reserved", "LTE_FT_flag -bit8 ", "reserved ", 
#endif
                    "reserved ", "reserved ", "reserved ", "reserved ", 
                    "reserved ", "reserved ", "reserved ", 
                    "MMI Test -bit27 ",
                    "Custom1 ", "Custom2 ", "Custom3 ", "Custom4 ", 
                    };
    const int32 item_type[32] = {1,1,1,1,
                                1, 1,1,1,
                                1,1,
                                1, 1, 1,0,      //2, 2, 2, 2, 
#ifdef PRODUCT_DM
#ifdef PLATFORM_SC7702
                                1, 1, 1,
                                1, 2, 2,  
#else
                                1, 1, 1,
                                2, 2, 2,  
#endif                                
#else                            
                                1, 1, 0,
                                0, 1, 0,
#endif
                                0, 0, 0, 0, 0, 0, 0,
                                2,
                                3, 3, 3, 3
                                };
    // 0:reserved 1:adc calibrate 2:test 3:custom

    const char  *if_calibrated[] = {" <N>", " <Y>"} ;
    int32 calibrated_length[2]  = { 0 };
    const char *if_passed[] = {"failed" , "passed"} ;
    int32 passed_length[2]  = {0};

    calibrated_length[0] =  SCI_STRLEN(if_calibrated[0]);
    calibrated_length[1] =  SCI_STRLEN(if_calibrated[1]);

    passed_length[0]  = SCI_STRLEN(if_passed[0]);
    passed_length[1]  = SCI_STRLEN(if_passed[1]);
    
    if(PNULL == adc_info_buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetAdcCalibrateInfoStr adc_info_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4342_112_2_18_2_17_49_133,(uint8*)"");
        return -1;
    }
    
    //SCI_TRACE_LOW:"mmieng_win.c GetAdcCalibrateInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4346_112_2_18_2_17_49_134,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
    //从NV中读取adc_reserved[7]的信息
#ifdef WIN32    
    adc_reserved_info = 0xc3fe0155;/*lint !e64*/ 
#else
adc_reserved_info=MNPHONE_GetLteCalibInfo();
adc_reserved_info=adc_reserved_info<<10;

adc_reserved_info_temp=MNPHONE_GetGsmCalibInfo();
adc_reserved_info=adc_reserved_info|(adc_reserved_info_temp&0x3ff);

adc_reserved_info_temp = 0;
adc_reserved_info_temp = REFPARAM_GetCaliMarkerValue();
adc_reserved_info=adc_reserved_info|(adc_reserved_info_temp&0x8000000);

//adc_reserved_info=0x3200f;
	SCI_TraceLow("GetAdcCalibrateInfoStr@mmieng_win.c : adc_reserved_info=0x%x",adc_reserved_info);
#ifdef PLATFORM_SC7702 
    w_adc_reserved_info = REFPARAM_GetCaliMarkerValueEx(NETWORK_TYPE_WCDMA);
	temp_info = w_adc_reserved_info & (0x00000001 << 12);//保存w 第12位信息
	
	w_adc_reserved_info = w_adc_reserved_info << 14;
	w_adc_reserved_info = w_adc_reserved_info & w_cali_bit;//w信息保存14-17位

    //将接口REFPARAM_GetCaliMarkerValueEx提供的WBAND 信息赋给原信息对应位置
	adc_reserved_info = adc_reserved_info & 0xfffc3fff;	
	adc_reserved_info = adc_reserved_info | w_adc_reserved_info;
	
    //将接口REFPARAM_GetCaliMarkerValueEx提供的W Final Test 信息赋给原信息对应位置
	adc_reserved_info = adc_reserved_info & 0xfff7ffff;
	temp_info = temp_info << 7;
	adc_reserved_info = adc_reserved_info | temp_info;
    
    //SCI_TRACE_LOW:"GetAdcCalibrateInfoStr %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4357_112_2_18_2_17_50_135,(uint8*)"d",adc_reserved_info);
#endif
#endif

     if ( offset + 21 >= buf_len)
     {
         return offset ;
     }     
     MMI_MEMCPY(adc_info_buf + offset, buf_len - offset, "<Adc Calibrate Info>", 20, 20);
     offset += 20;
     adc_info_buf[offset] = CR_CHAR;
     offset += 1;
    
    if(ARR_SIZE(item) != ARR_SIZE(item_type))       /*lint !e506 !e774*/
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetAdcCalibrateInfoStr array definition error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4371_112_2_18_2_17_50_136,(uint8*)"");
        return -1;        
    }
    
    for(i = 0; i<32; i++)
    {      
        //dont display type 0 and 3
        if(0 == item_type[i] || 3 == item_type[i])  
        {
            continue;
        } 

        //serial num                   
        SCI_MEMSET(tmp_buf1, 0, 20);
        length = sprintf(tmp_buf1," ");
        if ( offset + length >= buf_len )
        {
           return offset ;
        }
        SCI_MEMCPY(adc_info_buf + offset,  tmp_buf1, length);        
        offset += length;    

        //item 
        length =  SCI_STRLEN(item[i]) ; 
        if ( offset + length >= buf_len )
        {
           return offset ;
        }      
         SCI_MEMCPY(adc_info_buf + offset, item[i], length) ;
         offset += length;
         
         //result
         result = (adc_reserved_info>>i) & 0x00000001 ;/*lint !e774*/
         switch (item_type[i])
         {
            case 0:             
                break;
            case 1:
                length =  SCI_STRLEN ( if_calibrated[result] );
                if ( offset + length >= buf_len ) 
                {
                    return offset ;
                }
                SCI_MEMCPY(adc_info_buf + offset, if_calibrated[result], length) ;
                offset += length;
                break;
            case 2:
                length =  SCI_STRLEN ( if_passed[result] );
                if ( offset + length >= buf_len ) 
                {
                    return offset ;
                }
                SCI_MEMCPY(adc_info_buf + offset, if_passed[result], length) ;
                offset += length;
                break;
            case 3:
                break;
            default:
                break;
         }   
         
         //enter key
          if ( offset + 1 >= buf_len)
          {
              return offset ;
          }   
          adc_info_buf[offset] = CR_CHAR;
          offset += 1;
    }   
    
    return (offset);

}

BOOLEAN ZDT_CheckChipCalibrationed(uint32* cali_info)
{
    int32 result = 0;
    uint32 adc_reserved_info = 0;
    uint32 adc_reserved_info_temp=0;
    int32 i =0 ;
    const char *item[] = { "GSM_calib_afc -bit0 ",  "GSM850_calib_agc -bit1 ",  "GSM850_calib_apc -bit2 ", "GSM900_calib_agc -bit3 ", 
                    "GSM900_calib_apc -bit4", "DCS1800_calib_agc -bit5", "DCS1800_calib_apc -bit6",  "PCS1900_calib_agc -bit7", 
                    "PCS1900_calib_apc -bit8", "GSM_FT_flag -bit9",
                    "LTE_calib_afc -bit0", "LTE_TDD_calib_agc -bit1", "LTE_TDD_calib_apc -bit2", "reserved",
                    "LTE_FDD_calib_agc -bit4","LTE_FDD_calib_apc -bit5", "reserved ", 
                    "reserved", "LTE_FT_flag -bit8 ", "reserved ", 
                    "reserved ", "reserved ", "reserved ", "reserved ",
                    "reserved ", "reserved ", "reserved ", 
                    "MMI Test ",
                    "Custom1 ", "Custom2 ", "Custom3 ", "Custom4 ", 
                    };
#ifdef MMI_ONLY_4G_DEL_2G_CUSTOM   
    const int32 item_type[32] = {0,0,0,0,
                                0, 0,0,0,
                                0,0,
                                1, 1, 1,0,
                                1, 1, 0,
                                0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0,
                                0,
                                3, 3, 3, 3
                                };
#else
    const int32 item_type[32] = {1,1,1,1,
                                1, 1,1,1,
                                1,0,
                                1, 1, 1,0,      //2, 2, 2, 2, 
                                1, 1, 0,
                                0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0,
                                0,
                                3, 3, 3, 3
                                };
#endif
#ifndef WIN32        
    adc_reserved_info=MNPHONE_GetLteCalibInfo();
    adc_reserved_info=adc_reserved_info<<10;
    adc_reserved_info_temp=MNPHONE_GetGsmCalibInfo();
    adc_reserved_info=adc_reserved_info|(adc_reserved_info_temp&0x3ff);
    if(cali_info != PNULL)
    {
        *cali_info = adc_reserved_info;
    }
    if(ARR_SIZE(item) != ARR_SIZE(item_type))       /*lint !e506 !e774*/
    {
        return FALSE;        
    }
    for(i = 0; i<32; i++)
    {      
        if(0 == item_type[i] || 3 == item_type[i])  
        {
            continue;
        } 
         result = (adc_reserved_info>>i) & 0x00000001 ;/*lint !e774*/
         if(result == 0)
         {
            return FALSE;
         }   
    }   
    #endif
    return TRUE;
}
/********************************************************************************
 NAME:          EngShowAdcCalibrateWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        zhaohui
 DATE:          2008.10.24
********************************************************************************/
LOCAL MMI_RESULT_E EngShowAdcCalibrateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[1024] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    wchar w_buf[1024] = {0};
        
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        text.wstr_len = GetAdcCalibrateInfoStr(buf, 1024);
        MMI_STRNTOWSTR(w_buf,1024, (const uint8 *)buf,text.wstr_len,text.wstr_len);    
        text.wstr_ptr = w_buf;
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len,FALSE);
        MMK_SetAtvCtrl(win_id, MMIENG_TEXTBOX1_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngAssertTargetWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.26
********************************************************************************/
LOCAL MMI_RESULT_E EngAssertTargetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 target[21] = {0};
    MMI_PARTY_NUMBER_T party_num = {MN_NUM_TYPE_UNKNOW, 0, PNULL};  
    MMI_STRING_T target_buf = {PNULL, 0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint16 i = 0;
	uint16 j = 0 ;
  //  GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        //GUIEDIT_SetRect(MMK_ConvertIdToHandle(MMIENG_EDITBOX1_CTRL_ID), &rect);
        {
            GUI_BOTH_RECT_T both_client_rect  = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));
            GUIAPICTRL_SetBothRect (MMK_ConvertIdToHandle(MMIENG_EDITBOX1_CTRL_ID), &both_client_rect);
        }
        GUIEDIT_SetStyle(MMIENG_EDITBOX1_CTRL_ID, GUIEDIT_STYLE_SINGLE);
        //LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        MMK_SetAtvCtrl(MMIENG_ASSERTTARGET_WIN_ID, MMIENG_EDITBOX1_CTRL_ID);
        break;
/*
    case MSG_LCD_SWITCH:
        GUIEDIT_SetRect(MMK_ConvertIdToHandle(MMIENG_EDITBOX1_CTRL_ID), &rect);
            break;
    case MSG_FULL_PAINT:
         LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;
*/  
  
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_ASSERTTARGET_WIN_ID);
        break;
    

    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIEDIT_GetPhoneNumString(MMIENG_EDITBOX1_CTRL_ID, &target_buf);
        if (target_buf.wstr_len > 0)
        {
            uint8 temp_str[ENG_TARGET_MAX_LEN + 1] = {0};

            MMI_WSTRNTOSTR( temp_str, ENG_TARGET_MAX_LEN, target_buf.wstr_ptr, target_buf.wstr_len, target_buf.wstr_len );

            // need to convert the target phone number to BCD
            if (MMIAPICOM_GenPartyNumber( temp_str, target_buf.wstr_len, &party_num))
            {
#ifndef _WIN32
                 MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
                target[0] = (uint8)party_num.num_len;
		j= 	MIN(party_num.num_len,20);
                for (i = 0; i <j ; i++)
                {
                    target[i + 1] = party_num.bcd_num[i];
                }
                if (party_num.num_len != target_buf.wstr_len)
                    target[j] = target[j] | 0xF0;
                
                #ifndef _WIN32
                for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                {
                    MNSMS_SetAssertSmsDestAddrEx(dual_sys, target);
                }
                #endif
            }
            MMK_CloseWin(MMIENG_ASSERTTARGET_WIN_ID);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          AppendForbidPLMNItem
 DESCRIPTION:   
 PARAM IN:      list_id - 
 PARAM OUT:     return forbid PLMN num
 AUTHOR:        allen
 DATE:          2004.11.05
********************************************************************************/
LOCAL uint32 AppendForbidPLMNItem(MMI_CTRL_ID_T list_id)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar wstr[6] = {0};
    uint32 i = 0;
    
#ifndef _WIN32
    if (ERR_MNPHONE_NO_ERR != MNPHONE_GetFplmnListEx(MN_DUAL_SYS_1, &s_mmieng_fplmn))//%#discuss
        s_mmieng_fplmn.plmn_num = 0;
#else
    s_mmieng_fplmn.plmn_num = 2;
    s_mmieng_fplmn.plmn_arr[0].mcc = 460;
    s_mmieng_fplmn.plmn_arr[0].mnc = 00;        // PLMN_CMCC
    s_mmieng_fplmn.plmn_arr[1].mcc = 460;
    s_mmieng_fplmn.plmn_arr[1].mnc = 01;        // PLMN_UNICOM
#endif   
    if (0 == s_mmieng_fplmn.plmn_num)
        return 0;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    //set softkey
    item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_ENG_CLEAR;
    //item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)TXT_ENG_RETURN;
    
    GUILIST_SetMaxItem( list_id, (uint16)s_mmieng_fplmn.plmn_num, FALSE );//max item 4
    GUILIST_SetOwnSofterKey(list_id,TRUE);

    for ( i = 0; i < s_mmieng_fplmn.plmn_num; i++ )
    {        
        char str[6] = {0};
        SCI_MEMSET(wstr,0,6);
        sprintf( str, "%03d", s_mmieng_fplmn.plmn_arr[i].mcc );     // country code
        sprintf( str + 3 , "%02d", s_mmieng_fplmn.plmn_arr[i].mnc );    // network code
        item_data.item_content[0].item_data.text_buffer.wstr_len = 5;

        item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr;

        MMI_STRNTOWSTR( wstr, GUILIST_STRING_MAX_NUM, (uint8*)str, 6, item_data.item_content[0].item_data.text_buffer.wstr_len );
        //item_data.item_content[0].item_data.text_buffer.is_ucs2 = FALSE;
        
        GUILIST_AppendItem( list_id, &item_t );
        // clear forbid PLMN
        s_mmieng_fplmn.plmn_arr[i].mcc = 0;
        s_mmieng_fplmn.plmn_arr[i].mnc = 0;
    }
    // clear forbid PLMN
    s_mmieng_fplmn.plmn_num = 0;
    
    GUILIST_SetCurItemIndex(MMIENG_LISTBOX_CTRL_ID, 0);
    GUILIST_SetSelectedItem(MMIENG_LISTBOX_CTRL_ID, 0, FALSE);
    return (s_mmieng_fplmn.plmn_num);
}

/********************************************************************************
 NAME:          HandlePLMNPromptWindow
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.11.08
********************************************************************************/
/*LOCAL MMI_RESULT_E HandlePLMNPromptWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
        case MSG_CLOSE_WINDOW:
            MMK_CloseWin(MMIENG_FORBIDPLMN_WIN_ID);             
        default:
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
    }
    return (result);
}
*/
/********************************************************************************
 NAME:          EngForbidPLMNWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.11.05
********************************************************************************/
LOCAL MMI_RESULT_E EngForbidPLMNWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:">>>>>>>>>  enter EngForbidPLMNWinHandleMsg, msg_id = 0x%02X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4674_112_2_18_2_17_50_137,(uint8*)"d", msg_id);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        if (0 == AppendForbidPLMNItem(MMIENG_LISTBOX_CTRL_ID))
        {
            MMIPUB_OpenAlertWarningWin(TXT_ENG_NO_PLMN);
            MMK_CloseWin(MMIENG_FORBIDPLMN_WIN_ID);
        }
        else
        {
            MMK_SetAtvCtrl(MMIENG_FORBIDPLMN_WIN_ID, MMIENG_LISTBOX_CTRL_ID);
        }
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_FORBIDPLMN_WIN_ID);
        break;


    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        // forbid PLMN data have been cleared when append PLMN items
        #ifndef _WIN32
        MNPHONE_SetFplmnListEx(MN_DUAL_SYS_1, &s_mmieng_fplmn);//%#discuss
        #endif
        MMK_CloseWin(MMIENG_FORBIDPLMN_WIN_ID);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngNetLostWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngNetLostWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{   
    char text_buf[64] = {0};
    char text_temp[16] = {0};
     wchar wstr[64] = {0};
    
     char* str = PNULL;
    int32 length = 0;
    int32 offset = 0;
    int32 no_cell = 0;
    int32 search_times = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:  
        #ifndef _WIN32
        no_cell = MNPHONE_GetTimesOfNoCellAvailableEx(MN_DUAL_SYS_1);//%#discuss
        search_times = MNPHONE_GetTimesOfSearchPlmnEx(MN_DUAL_SYS_1);//%#discuss
        #else
        no_cell = 0;
        search_times = 0;
        #endif
        
        str =  "No Cell:\n";
        length= strlen(str);    
    SCI_MEMCPY(text_buf + offset, str, length);
    offset += length;

    SCI_MEMSET((char*)text_temp, 0, 16);
    sprintf((char*)text_temp, "%ld\n", no_cell);
    length= strlen(text_temp);
    SCI_MEMCPY(text_buf + offset, text_temp, length);
    offset += length;

        str = "Net Research:\n";
       length= strlen(str);     
     SCI_MEMCPY(text_buf + offset, str, length);
    offset += length;
            
    SCI_MEMSET((char*)text_temp, 0, 16);
    sprintf((char*)text_temp, "%ld", search_times);
    length= strlen(text_temp);
    SCI_MEMCPY(text_buf + offset, text_temp, length);
    offset += length;

        MMI_STRNTOWSTR( wstr, 64, (uint8*)text_buf, offset, offset );

    GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, wstr, offset, FALSE);
        break;

    case MSG_CTL_CANCEL:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_CANCEL:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(MMIENG_NETLOST_WIN_ID);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          GetTCVInfoStr
 DESCRIPTION:   
 PARAM IN:      tcv_buf - 
                buf_len - 
 PARAM OUT:     net info string len
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL int GetTCVInfoStr(uint8* tcv_buf, uint8 buf_len)
{
    uint32 temperature = 0;
    uint32 current = 0;
    uint32 value = 0;
    
    uint8 digit_buf1[19] = {0};
    int len1 = 0;
    int offset = 0;

    if(PNULL == tcv_buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetTCVInfoStr tcv_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4808_112_2_18_2_17_50_138,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_win.c GetTCVInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4811_112_2_18_2_17_50_139,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
    #ifndef _WIN32
    temperature = ADC_GetTemperature();
    current = ADC_GetChargeCurrent();
    value = ADC_GetValue(0);
    #endif
    
    ////////////////
    //  Temperature
    ////////////////
    MMI_MEMCPY(tcv_buf + offset, buf_len - offset, "<Temperature>", 13, 13);
    offset += 13;
    tcv_buf[offset] = CR_CHAR;
    offset += 1;
    MMIAPICOM_Int2Str(temperature, digit_buf1, 18);
    len1 = SCI_STRLEN((char*)digit_buf1);
    MMI_MEMCPY(tcv_buf + offset, buf_len - offset, digit_buf1, 18, len1);
    offset += len1;
    tcv_buf[offset] = CR_CHAR;
    offset += 1;
    ////////////////////
    //  Charge Current
    ////////////////////
    MMI_MEMCPY(tcv_buf + offset, buf_len - offset, "<Charge Current>", 16, 16);
    offset += 16;
    tcv_buf[offset] = CR_CHAR;
    offset += 1;
    MMIAPICOM_Int2Str(current, digit_buf1, 18);
    len1 = SCI_STRLEN((char*)digit_buf1);
    MMI_MEMCPY(tcv_buf + offset, buf_len - offset, digit_buf1, 18, len1);
    offset += len1;
    tcv_buf[offset] = CR_CHAR;
    offset += 1;
    ////////////
    //  Volume
    ////////////
    MMI_MEMCPY(tcv_buf + offset, buf_len - offset, "<Volume>", 8, 8);
    offset += 8;
    tcv_buf[offset] = CR_CHAR;
    offset += 1;
    MMIAPICOM_Int2Str(value, digit_buf1, 18);
    len1 = SCI_STRLEN((char*)digit_buf1);
    MMI_MEMCPY(tcv_buf + offset, buf_len - offset, digit_buf1, 18, len1);
    offset += len1;
    return (offset);
}

/********************************************************************************
 NAME:          EngTCVWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngTCVWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    //GUI_RECT_T rect = MMITHEME_GetClientRect();
    MMI_STRING_T text = {0};
    uint8 buf[151] = {0};
    wchar wstr[151] = {0};
    char* str = PNULL;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    //SCI_TRACE_LOW:">>>>>>>>>  enter EngTCVWinHandleMsg, msg_id = 0x%02X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4882_112_2_18_2_17_51_140,(uint8*)"d", msg_id);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
    //  LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);            

        str = (char*)buf;
        text.wstr_len = GetTCVInfoStr(buf, 150);
        text.wstr_ptr = wstr;
        MMI_STRNTOWSTR( wstr, 150, (uint8*)str, text.wstr_len, text.wstr_len );
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
        break;
        
    case MSG_FULL_PAINT:
        //LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        break;

    case MSG_CTL_CANCEL:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
    case MSG_APP_WEB:
        MMK_CloseWin(MMIENG_TCV_WIN_ID);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#if 0
/********************************************************************************
 NAME:          HandleAFCWaitWindow
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.28
********************************************************************************/
LOCAL MMI_RESULT_E HandleAFCWaitWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        #ifndef _WIN32
        MMK_CloseWin(MMIENG_AFC_WIN_ID);
        #else
        g_mmieng_afc_value = 10;        // simulator for window
        #endif
        MMK_CloseWin(MMIENG_WAITING_WIN_ID);
        break;

    case MSG_APP_RED:
        #ifndef _WIN32
        MMK_CloseWin(MMIENG_AFC_WIN_ID);
        #endif
        MMK_CloseWin(MMIENG_WAITING_WIN_ID);
        result = MMI_RESULT_TRUE;
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngAFCWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngAFCWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_RECT_T rect = MMITHEME_GetClientRect();
    uint8 data_buf[20] = {0};
    wchar wstr[30] = {0};
    char* str = PNULL;
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN l1_result = TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
     GUI_RECT_T      prg_rect = rect ;
     GUI_RECT_T lab_rect  = rect ;
        
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
         lab_rect.bottom  =  ((rect.top  + rect.bottom)>>1) - 1;
        lab_rect.top = lab_rect.bottom - 40;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        
        prg_rect.top = ((rect.top  + rect.bottom)>>1) + 1;
        prg_rect.bottom= prg_rect.top + 30;
        prg_rect.left += 1;
        prg_rect.right -= 1;          
        GUIPRGBOX_SetRect(MMIENG_PRGBOX_CTRL_ID,&prg_rect);
        GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID, mmi_eng_display.bkgd_color);
        GUIPRGBOX_SetParamByCtrl(MMIENG_PRGBOX_CTRL_ID, MMIENG_AFC_MAX_VALUE, MMIENG_AFC_MAX_VALUE);
       
        s_mmieng_afc_inited = FALSE;

        if (!s_mmieng_afc_inited)
        {
            str = MMIENG_WAIT_MSG;
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 150, (uint8*)str, text.wstr_len, text.wstr_len );
            MMIPUB_OpenWaitWin(1,&text,PNULL,PNULL,MMIENG_WAITING_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleAFCWaitWindow);
            
#ifndef _WIN32
            l1_result = L1API_GetAFC(P_APP);
            if (!l1_result)
            {
                MMK_CloseWin(MMIENG_WAITING_WIN_ID);
                MMK_CloseWin(MMIENG_AFC_WIN_ID);
            }
#else
            l1_result = TRUE;
#endif
        }
        
        sprintf((char*)data_buf, "%ld", g_mmieng_afc_value);        
        str = (char*)data_buf;
        text1.wstr_len = strlen(str);
        text1.wstr_ptr = wstr;
        MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );        
        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, FALSE);        
        break;

        case MSG_LCD_SWITCH:
        lab_rect.bottom  =  ((rect.top  + rect.bottom)>>1) - 1;
        lab_rect.top = lab_rect.bottom - 40;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        prg_rect.top = ((rect.top  + rect.bottom)>>1) + 1;
        prg_rect.bottom= prg_rect.top + 30;
        prg_rect.left += 1;
        prg_rect.right -= 1;  
            GUIPRGBOX_SetRect(MMIENG_PRGBOX_CTRL_ID,&prg_rect);
        break;
              
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_AFC_WIN_ID);
        break;

    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        text1.wstr_ptr = PNULL;
        text1.wstr_len = 0;
        text3.wstr_ptr = PNULL;
        text3.wstr_len = 0;
#ifndef _WIN32

        if (L1API_SetAFC(g_mmieng_afc_value) == SCI_TRUE)
        {
            text2.wstr_ptr = s_saved_wstr;//"Saved";
            text2.wstr_len = 5;
        }
        else
        {
            text2.wstr_ptr = s_fail_saved_wstr; //"Fail saved";
            text2.wstr_len = 10;
        }
#else
        text2.wstr_ptr = s_saved_wstr;//"Saved";
        text2.wstr_len = 5;
#endif
        MMK_CloseWin(MMIENG_AFC_WIN_ID);
        MMIEng_SetResultWinText(text1, text2, text3);
        MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
        break;

    case MSG_KEYDOWN_UP:
        if (g_mmieng_afc_value < MMIENG_AFC_MAX_VALUE)
        {
            g_mmieng_afc_value += 3000;
            if (g_mmieng_afc_value > MMIENG_AFC_MAX_VALUE)
                g_mmieng_afc_value = MMIENG_AFC_MAX_VALUE;
            sprintf((char*)data_buf, "%ld", g_mmieng_afc_value);

            str = (char*)data_buf;
            text1.wstr_len = strlen(str);
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,g_mmieng_afc_value,g_mmieng_afc_value,MMIENG_PRGBOX_CTRL_ID);
        }
        break;
        
    case MSG_KEYDOWN_DOWN:
        if (g_mmieng_afc_value > 0)
        {
            if (g_mmieng_afc_value > 3000)
                g_mmieng_afc_value -= 3000;
            else
                g_mmieng_afc_value = 0;
            sprintf((char*)data_buf, "%ld", g_mmieng_afc_value);

            str = (char*)data_buf;
            text1.wstr_len = strlen(str);
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );

        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,g_mmieng_afc_value,g_mmieng_afc_value,MMIENG_PRGBOX_CTRL_ID);
        }
        break;
        
    case MSG_KEYDOWN_LEFT:
        if (g_mmieng_afc_value > 0)
        {
            g_mmieng_afc_value--;
            sprintf((char*)data_buf, "%ld", g_mmieng_afc_value);

            str = (char*)data_buf;
            text1.wstr_len = strlen(str);
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,g_mmieng_afc_value,g_mmieng_afc_value,MMIENG_PRGBOX_CTRL_ID);
        }
        break;
        
    case MSG_KEYDOWN_RIGHT:
        if (g_mmieng_afc_value < MMIENG_AFC_MAX_VALUE)
        {
            g_mmieng_afc_value++;
            sprintf((char*)data_buf, "%ld", g_mmieng_afc_value);

            str = (char*)data_buf;
            text1.wstr_len = strlen(str);
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,g_mmieng_afc_value,g_mmieng_afc_value,MMIENG_PRGBOX_CTRL_ID);
        }
        break;

    case MSG_GET_FOCUS:     
    case MSG_ENG_L1_GET_CNF:
        LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        if (!s_mmieng_afc_inited)
        {
            s_mmieng_afc_inited = TRUE;
            //LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
            
            // AFC valid value is in 0 ~~ 1023
            //SCI_PASSERT(g_mmieng_afc_value <= MMIENG_AFC_MAX_VALUE, ("EngAFCWinHandleMsg: g_mmieng_afc_value is %d",g_mmieng_afc_value));         
            if (g_mmieng_afc_value > MMIENG_AFC_MAX_VALUE)
            {
                //SCI_TRACE_LOW:"EngAFCWinHandleMsg: g_mmieng_afc_value is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_5156_112_2_18_2_17_51_141,(uint8*)"d",g_mmieng_afc_value);
                g_mmieng_afc_value = MMIENG_AFC_MAX_VALUE;
            }
            SCI_MEMSET((char*)data_buf, 0, 20);
            sprintf((char*)data_buf, "%ld", g_mmieng_afc_value);

            str = (char*)data_buf;
            text1.wstr_len = strlen(str);
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, FALSE);
            GUIPRGBOX_SetPosByCtrl(FALSE,g_mmieng_afc_value,g_mmieng_afc_value,MMIENG_PRGBOX_CTRL_ID);
            if (MSG_ENG_L1_GET_CNF == msg_id)
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, 0);
            }
        }
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

LOCAL MMI_RESULT_E EngAFCWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    const int32 buffer_size = 32;
    uint8 data_buf[32 + 1] = {0};
    wchar wstr[32 + 1] = {0};
    char* str = PNULL;
    int32 length = 0;
    MMI_STRING_T text = {0} ;
    BOOLEAN l1_result = FALSE;
    
    switch (msg_id) 
    {
     case MSG_OPEN_WINDOW:     
        s_mmieng_afc_inited = FALSE;
        if (!s_mmieng_afc_inited)
        {
            str = MMIENG_WAIT_MSG;
            length  = SCI_STRLEN(str);          
            MMI_STRNTOWSTR( wstr, buffer_size, (uint8*)str,length, length);
            text.wstr_ptr = wstr;
            text.wstr_len =  length;            
            MMIPUB_OpenWaitWin(1,&text,PNULL,PNULL,MMIENG_WAITING_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleAFCWaitWindow);            
#ifndef  WIN32
            l1_result = L1API_GetAFC(P_APP);
            if (!l1_result)
            {
                MMK_CloseWin(MMIENG_WAITING_WIN_ID);
                MMK_CloseWin(MMIENG_AFC_WIN_ID);
            }
#else
            l1_result = TRUE;
#endif
        }
        break;
        case MSG_GET_FOCUS: 
            //lint -fallthrough
    case MSG_ENG_L1_GET_CNF:    
        //LCD_FillRect( MMITHEME_GetDefaultLcdDev(), rect, mmi_eng_display.bkgd_color);
        if (!s_mmieng_afc_inited)
        {
            s_mmieng_afc_inited = TRUE;        
            
            // AFC valid value is in 0 ~~ 1023
            //SCI_PASSERT(g_mmieng_afc_value <= MMIENG_AFC_MAX_VALUE, ("EngAFCWinHandleMsg: g_mmieng_afc_value is %d",g_mmieng_afc_value));         
            if (g_mmieng_afc_value > MMIENG_AFC_MAX_VALUE)
            {
                //SCI_TRACE_LOW:"EngAFCWinHandleMsg: g_mmieng_afc_value is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_5231_112_2_18_2_17_51_142,(uint8*)"d",g_mmieng_afc_value);
                g_mmieng_afc_value = MMIENG_AFC_MAX_VALUE;
            }
            SCI_MEMSET((char*)data_buf, 0, buffer_size + 1);
            sprintf((char*)data_buf, "%ld", g_mmieng_afc_value);
            str = (char*)data_buf;
            length = SCI_STRLEN(str);           
            MMI_STRNTOWSTR( wstr, buffer_size, (uint8*)str, text.wstr_len, text.wstr_len );
            GUITEXT_SetString( MMIENG_TEXTBOX1_CTRL_ID,  wstr,  length,  FALSE);
             if (MSG_ENG_L1_GET_CNF == msg_id)
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, 0);
            }
        }
        break;
    

     case MSG_KEYDOWN_CANCEL:
            //lint -fallthrough
    case MSG_CTL_CANCEL:
            //lint -fallthrough
    case MSG_KEYDOWN_OK:
            //lint -fallthrough
    case MSG_CTL_OK:
      MMK_CloseWin(MMIENG_AFC_WIN_ID);
        break;

        default:
        break;
        }
    return (l1_result);
}
/********************************************************************************
 NAME:          HandlePAWaitWindow
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.28
********************************************************************************/
LOCAL MMI_RESULT_E HandlePAWaitWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        #ifndef _WIN32
        MMK_CloseWin(MMIENG_PA_WIN_ID);
        #else
        //g_mmieng_pa_value = 10;     // simulator for window
        #endif
        MMK_CloseWin(MMIENG_WAITING_WIN_ID);
        break;

    case MSG_APP_RED:
        #ifndef _WIN32
        MMK_CloseWin(MMIENG_PA_WIN_ID);
        #endif
        MMK_CloseWin(MMIENG_WAITING_WIN_ID);
        result = MMI_RESULT_TRUE;
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngPAWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/

LOCAL MMI_RESULT_E EngPAWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_RECT_T rect = MMITHEME_GetClientRect();
    uint8 data_buf[20] = {0};
    wchar wstr[30] = {0};
    char* str = PNULL;
    MMI_STRING_T text = {0}; 
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN l1_result = TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T          prg_rect = rect ;
    GUI_RECT_T   lab_rect = rect;
    
        
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        
        lab_rect.bottom  =  ((rect.top  + rect.bottom)>>1) - 1;
        lab_rect.top = lab_rect.bottom - 40;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        prg_rect.top = ((rect.top  + rect.bottom)>>1 )+ 1;
        prg_rect.bottom= prg_rect.top + 30;
        prg_rect.left += 1;
        prg_rect.right -= 1;  
        GUIPRGBOX_SetRect(MMIENG_PRGBOX_CTRL_ID,&prg_rect);       
        GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID, mmi_eng_display.bkgd_color);
        GUIPRGBOX_SetParamByCtrl(MMIENG_PRGBOX_CTRL_ID, MMIENG_PA_MAX_VALUE, MMIENG_PA_MAX_VALUE);
        s_mmieng_pa_inited = FALSE;
        if (!s_mmieng_pa_inited)
        {
            // L1 will transfer the PA data by message, MMI should receive them in application 's PS message function
            // when MMI get them, MMI will close the wait window, then PA window will re-get focus
            str = MMIENG_WAIT_MSG;
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMIPUB_OpenWaitWin(1,&text,PNULL,PNULL,MMIENG_WAITING_WIN_ID,IMAGE_NULL,
               ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandlePAWaitWindow);
            #ifndef _WIN32
            l1_result = L1API_GetPA(P_APP);
            if (!l1_result)
            {
                MMK_CloseWin(MMIENG_WAITING_WIN_ID);
                MMK_CloseWin(MMIENG_PA_WIN_ID);
            }
            #else
            l1_result = TRUE;
            #endif
        }
            sprintf((char*)data_buf, "%ld", g_mmieng_pa_value);
            
            str = (char*)data_buf;
            text1.wstr_len = strlen(str);
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );

 //           GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, FALSE);
 //           GUIPRGBOX_SetPosByCtrl(TRUE,g_mmieng_pa_value,g_mmieng_pa_value,MMIENG_PRGBOX_CTRL_ID);

        break;
        
        case MSG_LCD_SWITCH:
        lab_rect.bottom  =  ((rect.top  + rect.bottom)>>1) - 1;
        lab_rect.top = lab_rect.bottom - 40;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        prg_rect.top = ((rect.top  + rect.bottom)>>1) + 1;
        prg_rect.bottom= prg_rect.top + 30;
        prg_rect.left += 1;
        prg_rect.right -= 1;  
        GUIPRGBOX_SetRect(MMIENG_PRGBOX_CTRL_ID,&prg_rect);
      break;
              
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        break;
        

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_PA_WIN_ID);
        break;

    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        text1.wstr_ptr = PNULL;
        text1.wstr_len = 0;
        text3.wstr_ptr = PNULL;
        text3.wstr_len = 0;
        #ifndef _WIN32
        if (L1API_SetPA(g_mmieng_pa_value) == SCI_TRUE)
        {
            text2.wstr_ptr = s_saved_wstr;//"Saved";
            text2.wstr_len = 5;
        }
        else
        {
            text2.wstr_ptr = s_fail_saved_wstr;//"Fail saved";
            text2.wstr_len = 10;
        }
        #else
        text2.wstr_ptr = s_saved_wstr;//"Saved";
        text2.wstr_len = 5;
        #endif
        MMK_CloseWin(MMIENG_PA_WIN_ID);
        MMIEng_SetResultWinText(text1, text2, text3);
        MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
        break;

    case MSG_KEYDOWN_UP:
        if (g_mmieng_pa_value < MMIENG_PA_MAX_VALUE)
        {
            g_mmieng_pa_value += 3000;
            if (g_mmieng_pa_value > MMIENG_PA_MAX_VALUE)
                g_mmieng_pa_value = MMIENG_PA_MAX_VALUE;
            sprintf((char*)data_buf, "%ld", g_mmieng_pa_value);
            
            str = (char*)data_buf;
            text1.wstr_len = strlen(str);
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,g_mmieng_pa_value,g_mmieng_pa_value,MMIENG_PRGBOX_CTRL_ID);
        }
        break;
        
    case MSG_KEYDOWN_DOWN:
        if (g_mmieng_pa_value > 0)
        {
            if (g_mmieng_pa_value > 3000)
                g_mmieng_pa_value -= 3000;
            else
                g_mmieng_pa_value = 0;
            sprintf((char*)data_buf, "%ld", g_mmieng_pa_value);
            
            str = (char*)data_buf;
            text1.wstr_len = strlen(str);
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,g_mmieng_pa_value,g_mmieng_pa_value,MMIENG_PRGBOX_CTRL_ID);
        }
        break;
        
    case MSG_KEYDOWN_LEFT:
        if (g_mmieng_pa_value > 0)
        {
            g_mmieng_pa_value--;
            sprintf((char*)data_buf, "%ld", g_mmieng_pa_value);

            str = (char*)data_buf;
            text1.wstr_len = strlen(str);
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,g_mmieng_pa_value,g_mmieng_pa_value,MMIENG_PRGBOX_CTRL_ID);
        }
        break;
        
    case MSG_KEYDOWN_RIGHT:
        if (g_mmieng_pa_value < MMIENG_PA_MAX_VALUE)
        {
            g_mmieng_pa_value++;
            sprintf((char*)data_buf, "%ld", g_mmieng_pa_value);

            str = (char*)data_buf;
            text1.wstr_len = strlen(str);
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,g_mmieng_pa_value,g_mmieng_pa_value,MMIENG_PRGBOX_CTRL_ID);
        }
        break;
    
    case MSG_GET_FOCUS:         
    case MSG_ENG_L1_GET_CNF:
        LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        if (!s_mmieng_pa_inited)
        {
            s_mmieng_pa_inited = TRUE;
            
            // AFC valid value is in 0 ~~ 65535
            SCI_PASSERT(g_mmieng_pa_value <= MMIENG_PA_MAX_VALUE, ("EngPAWinHandleMsg: g_mmieng_pa_value is %d",g_mmieng_pa_value));            
            SCI_MEMSET((char*)data_buf, 0, 20);
            sprintf((char*)data_buf, "%ld", g_mmieng_pa_value);

            str = (char*)data_buf;
            text1.wstr_len = strlen(str);
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text1.wstr_len, text1.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text1, FALSE);
            GUIPRGBOX_SetPosByCtrl(FALSE,g_mmieng_pa_value,g_mmieng_pa_value,MMIENG_PRGBOX_CTRL_ID);
            if (MSG_ENG_L1_GET_CNF == msg_id)
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, 0);
            }
        }
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

LOCAL MMI_RESULT_E EngPAWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    const int32 buffer_size = 32;
    uint8 data_buf[32 + 1] = {0};
    wchar wstr[32 + 1] = {0};
    char* str = PNULL;
    int32 length = 0;
    MMI_STRING_T text = {0} ;
    BOOLEAN l1_result = FALSE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:     
        s_mmieng_pa_inited = FALSE;
        if (!s_mmieng_pa_inited)
        {
            // L1 will transfer the PA data by message, MMI should receive them in application 's PS message function
            // when MMI get them, MMI will close the wait window, then PA window will re-get focus
            str = MMIENG_WAIT_MSG;
            length  = SCI_STRLEN(str);          
            MMI_STRNTOWSTR( wstr, buffer_size, (uint8*)str,length, length);
            text.wstr_ptr = wstr;
            text.wstr_len =  length;       
            MMIPUB_OpenWaitWin(1,&text,PNULL,PNULL,MMIENG_WAITING_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandlePAWaitWindow);
#ifndef  WIN32
            l1_result = L1API_GetPA(P_APP);
            if (!l1_result)
            {
                MMK_CloseWin(MMIENG_WAITING_WIN_ID);
                MMK_CloseWin(MMIENG_PA_WIN_ID);
            }
#else
            l1_result = TRUE;
#endif
        }         
    break;
    case MSG_GET_FOCUS:
        //lint -fallthrough
    case MSG_ENG_L1_GET_CNF:
        // LCD_FillRect( MMITHEME_GetDefaultLcdDev(),  rect, mmi_eng_display.bkgd_color);
        if (!s_mmieng_pa_inited)
        {
            s_mmieng_pa_inited = TRUE;     
            
            // AFC valid value is in 0 ~~ 1023
            //SCI_PASSERT(g_mmieng_afc_value <= MMIENG_AFC_MAX_VALUE, ("EngAFCWinHandleMsg: g_mmieng_afc_value is %d",g_mmieng_afc_value));         
            SCI_MEMSET((char*)data_buf, 0, buffer_size + 1);
            
            sprintf((char*)data_buf, "%ld\n%ld\n%ld\n%ld", g_mmieng_pa_value[0],
                g_mmieng_pa_value[1],g_mmieng_pa_value[2],g_mmieng_pa_value[3]);          
            
            str = (char*)data_buf;
            length = SCI_STRLEN(str);           
            MMI_STRNTOWSTR( wstr, buffer_size, (uint8*)str, text.wstr_len, text.wstr_len );
            GUITEXT_SetString( MMIENG_TEXTBOX1_CTRL_ID,  wstr,  length,  FALSE);
            if (MSG_ENG_L1_GET_CNF == msg_id)
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, 0);
            }
        }
        break;
                
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:   
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(MMIENG_PA_WIN_ID);
        break;
        
    default:
        break;
    }
    return(l1_result);
}


/********************************************************************************
 NAME:          HandleAGCWaitWindow
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.28
********************************************************************************/
LOCAL MMI_RESULT_E HandleAGCWaitWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        #ifndef _WIN32
        MMK_CloseWin(MMIENG_AGC_WIN_ID);
        #else
        g_mmieng_agc_value = 10;        // simulator for window
        #endif
        MMK_CloseWin(MMIENG_WAITING_WIN_ID);
        break;

    case MSG_APP_RED:
        #ifndef _WIN32
        MMK_CloseWin(MMIENG_AGC_WIN_ID);
        #endif
        MMK_CloseWin(MMIENG_WAITING_WIN_ID);
        result = MMI_RESULT_TRUE;
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          GetAGCInfoStr
 DESCRIPTION:   
 PARAM IN:      agc_buf - 
                buf_len - 
 PARAM OUT:     agc info string len
 AUTHOR:        allen
 DATE:          2004.09.28
********************************************************************************/
LOCAL int GetAGCInfoStr(uint8* agc_buf, uint8 buf_len)
{
    uint8 digit_buf1[19] = {0};

    int len1 = 0;
    int offset = 0;
    
    if(PNULL == agc_buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetAGCInfoStr agc_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_5662_112_2_18_2_17_52_143,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_win.c GetAGCInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_5665_112_2_18_2_17_52_144,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    MMI_MEMCPY(agc_buf + offset, buf_len - offset, "<AGC Value>", 11, 11);
    offset += 11;
    agc_buf[offset] = CR_CHAR;
    offset += 1;
    MMIAPICOM_Int2Str(g_mmieng_agc_value, digit_buf1, 18);
    len1 = SCI_STRLEN((char*)digit_buf1);
    MMI_MEMCPY(agc_buf + offset, buf_len - offset, digit_buf1, 18, len1);
    offset += len1;
    return (offset);
}

/********************************************************************************
 NAME:          EngAGCWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngAGCWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_RECT_T rect = MMITHEME_GetClientRect();
    uint8 text_buf[151] = {0};
    wchar wstr[151] = {0};
    char* str = PNULL;
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN l1_result = TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        
    //SCI_TRACE_LOW:">>>>>>>>>  enter EngAGCWinHandleMsg, msg_id = 0x%02X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_5701_112_2_18_2_17_52_145,(uint8*)"d", msg_id);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        s_mmieng_agc_inited = FALSE;
        if (!s_mmieng_agc_inited)
        {
            // L1 will transfer the AGC data by message, MMI should receive them in application 's PS message function
            // when MMI get them, MMI will close the wait window, then AGC window will re-get focus
               MMIPUB_OpenWaitWin(1,&text,PNULL,PNULL,MMIENG_WAITING_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleAGCWaitWindow);
            #ifndef _WIN32
            l1_result = L1API_GetAGC(P_APP);
            if (!l1_result)
            {
                MMK_CloseWin(MMIENG_WAITING_WIN_ID);
                MMK_CloseWin(MMIENG_AGC_WIN_ID);
            }
            #else
            l1_result = TRUE;
            #endif
        }
        break;
    
        case MSG_FULL_PAINT:                
            //LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);  
        break;
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_AGC_WIN_ID);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(MMIENG_AGC_WIN_ID);
        break;

    case MSG_GET_FOCUS: 
    case MSG_ENG_L1_GET_CNF:
        if (!s_mmieng_agc_inited)
        {
            s_mmieng_agc_inited = TRUE; 
            
           LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);

            str = (char*)text_buf;
            text.wstr_len =  GetAGCInfoStr(text_buf, 150);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 150, (uint8*)str, text.wstr_len, text.wstr_len );


            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
            if (MSG_ENG_L1_GET_CNF == msg_id)
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, 0);
            }
        }
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

/********************************************************************************
 NAME:          HandleErrRateWaitWindow
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.28
********************************************************************************/
LOCAL MMI_RESULT_E HandleErrRateWaitWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        #ifndef _WIN32
        MMK_CloseWin(MMIENG_ERRRATE_WIN_ID);
        #else
        g_mmieng_total_pch_number = 1024;       // simulator for window
        g_mmieng_lost_pch_number = 5;           // simulator for window
        #endif
        MMK_CloseWin(MMIENG_WAITING_WIN_ID);
        break;

    case MSG_APP_RED:
        #ifndef _WIN32
        MMK_CloseWin(MMIENG_ERRRATE_WIN_ID);
        #endif
        MMK_CloseWin(MMIENG_WAITING_WIN_ID);
        result = MMI_RESULT_TRUE;
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          GetErrRateInfoStr
 DESCRIPTION:   
 PARAM IN:      rate_buf - 
                buf_len - 
 PARAM OUT:     errrate info string len
 AUTHOR:        allen
 DATE:          2004.09.28
********************************************************************************/
LOCAL int GetErrRateInfoStr(uint8* rate_buf, uint8 buf_len)
{
    uint8 digit_buf1[19] = {0};
    int len1 = 0;
    int offset = 0;
    
    if(PNULL == rate_buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetErrRateInfoStr rate_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_5831_112_2_18_2_17_53_146,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_win.c GetErrRateInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_5834_112_2_18_2_17_53_147,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
    ////////////////
    //  total pch
    ////////////////
    MMI_MEMCPY(rate_buf + offset, buf_len - offset, "<Total>", 7, 7);
    offset += 7;
    rate_buf[offset] = CR_CHAR;
    offset += 1;
    MMIAPICOM_Int2Str(g_mmieng_total_pch_number, digit_buf1, 18);
    len1 = SCI_STRLEN((char*)digit_buf1);
    MMI_MEMCPY(rate_buf + offset, buf_len - offset, digit_buf1, 18, len1);
    offset += len1;
    rate_buf[offset] = CR_CHAR;
    offset += 1;
    ////////////////////
    //  lost pch
    ////////////////////
    MMI_MEMCPY(rate_buf + offset, buf_len - offset, "<Lost>", 6, 6);
    offset += 6;
    rate_buf[offset] = CR_CHAR;
    offset += 1;
    MMIAPICOM_Int2Str(g_mmieng_lost_pch_number, digit_buf1, 18);
    len1 = SCI_STRLEN((char*)digit_buf1);
    MMI_MEMCPY(rate_buf + offset, buf_len - offset, digit_buf1, 18, len1);
    offset += len1;
    return (offset);
}

/********************************************************************************
 NAME:          EngErrRateWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngErrRateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    //GUI_RECT_T rect = MMITHEME_GetClientRect();
    uint8 text_buf[151] = {0};
    wchar wstr[151] = {0};
    char* str = PNULL;
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN l1_result = TRUE;
    //GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        
    //SCI_TRACE_LOW:">>>>>>>>>  enter EngErrRateWinHandleMsg, msg_id = 0x%02X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_5887_112_2_18_2_17_53_148,(uint8*)"d", msg_id);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        s_mmieng_errrate_inited = FALSE;
        if (!s_mmieng_errrate_inited)
        {
            str = MMIENG_WAIT_MSG;
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 150, (uint8*)str, text.wstr_len, text.wstr_len );

            MMIPUB_OpenWaitWin(1,&text,PNULL,PNULL,MMIENG_WAITING_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleErrRateWaitWindow);
            // L1 will transfer the error rate data by message, MMI should receive them in application 's PS message function
            // when MMI get them, MMI will close the wait window, then ERRRATE window will re-get focus
            #ifndef _WIN32
            l1_result = L1API_GetBlockErrorRatio(P_APP);
            if (!l1_result)
            {
                MMK_CloseWin(MMIENG_ERRRATE_WIN_ID);
                MMK_CloseWin(MMIENG_WAITING_WIN_ID);
            }
            #else
            l1_result = TRUE;
            #endif
        }
        break;
        
    case MSG_FULL_PAINT:     
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_ERRRATE_WIN_ID);
        break;

    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(MMIENG_ERRRATE_WIN_ID);
        break;
        
    case MSG_GET_FOCUS:
        if (!s_mmieng_errrate_inited)
        {
            s_mmieng_errrate_inited = TRUE;
            
            //LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);

            str = (char*)text_buf;
            text.wstr_len =  GetErrRateInfoStr(text_buf, 150);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 150, (uint8*)str, text.wstr_len, text.wstr_len );
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
        }
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/********************************************************************************
 NAME:			MMIAPIENG_get usb log
 DESCRIPTION:		
 AUTHOR:		fen.xie
 DATE:			2010.07.06
********************************************************************************/
PUBLIC int32 MMIENG_GetUSBLog(BOOLEAN *is_on)
{    
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_ENG_USBLOG,is_on,return_value);        
    if(MN_RETURN_SUCCESS!=return_value)
    {
        *is_on = FALSE;      
        MMINV_WRITE(MMINV_ENG_USBLOG,is_on);
    } 
    return return_value;
}

/********************************************************************************
 NAME:			MMIAPIENG_Set usb log
 DESCRIPTION:		
 AUTHOR:		fen.xie
 DATE:			2010.07.06
********************************************************************************/
PUBLIC int32 MMIENG_SetUSBLog(BOOLEAN is_on)
{
    MMINV_WRITE(MMINV_ENG_USBLOG,&is_on);
    return 0;
}

/********************************************************************************
 NAME:          EngARMLOGPOWERWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngUSBLOGWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{   
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    BOOLEAN is_on = FALSE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2        
           
        MMIENG_GetUSBLog(&is_on);
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);         

        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, is_on, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,is_on);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);     
        break;
        
    case MSG_FULL_PAINT:
        break;  
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        is_on = GUILIST_GetCurItemIndex(ctrl_id);       
        //MMINV_WRITE(MMINV_ENG_USBLOG,&is_on);  
        MMIENG_SetUSBLog(is_on);
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);       
        MMK_CloseWin(win_id);
        break;      
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/********************************************************************************
 NAME:          EngAudioRead
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     目前返回值无正常意义，需要统一
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL uint32 EngAudioRead(uint32 menu_id , int16 * vol_array)
{  
    NV_AUDIO_E   audio_mode=NV_AUDIO_HEADSET_E;     //对应dsp_audio nv
    AUDIO_DEVICE_MODE_TYPE_E arm_audio_mode = AUDIO_DEVICE_MODE_HANDFREE;  //MP3 audio对应的是audio_arm***.nvm
    AUDIO_NV_ARM_MODE_INFO_T *audmod_arm_devmod_ptr = &s_arm_audiomode_param;
    AUDIO_NV_ARM_RESULT_E    result = AUDIO_NV_ARM_NO_ERROR; 
    char  *name_ptr = PNULL;
    uint8  name_len = 0;
    uint32 arm_volume = 0;
    uint offset=0; 
    uint16 iLoop=0;
    uint16 jLoop=0;
    
    //SCI_TRACE_LOW:"EngAudioRead:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_6032_112_2_18_2_17_53_149,(uint8*)"d",menu_id,MMIENG_MENU_ID_MAX);
    switch(menu_id)
    {
        
    case ID_ENG_AUDIO_HANDSET_RECEIVER:
        //offset+=18;
        offset=110;
        audio_mode=NV_AUDIO_HANDSET_E;
        break;
    case ID_ENG_AUDIO_HANDSET_SIDETONE:
        //offset+=24;
        offset=92;
        audio_mode=NV_AUDIO_HANDSET_E;
        break;
    case ID_ENG_AUDIO_HANDSET_MIC:
        offset=68;
        audio_mode=NV_AUDIO_HANDSET_E;        
        break;

    case ID_ENG_AUDIO_HEADSET_EAR:
        //offset+=42;
        offset=110;
        audio_mode=NV_AUDIO_HEADSET_E;        
        break;
    case ID_ENG_AUDIO_HEADSET_MIC:
        offset=68;
        audio_mode=NV_AUDIO_HEADSET_E;        
        break;

    case ID_ENG_AUDIO_HANDSFREE_SPEAKER:
        //offset+=42;
        offset=110;
        audio_mode=NV_AUDIO_HANDSFREE_E;
        break;
    case ID_ENG_AUDIO_HANDSFREE_MIC:
        offset+=68;
        audio_mode=NV_AUDIO_HANDSFREE_E;
        break;

    case ID_ENG_AUDIO_MP4HFTP_SPEAKER:
        offset+=110;
        audio_mode=NV_AUDIO_MP4HFTP_E;        
        break;

    case ID_ENG_AUDIO_MP4HFTR_MIC:
        offset+=68;
        audio_mode=NV_AUDIO_MP4HFTR_E;
        break;

    case ID_ENG_AUDIO_MP4HEADSET_EAR:
        audio_mode=NV_AUDIO_MP4HEADSET_E;
        break;
    case ID_ENG_AUDIO_RECORD_MIC:
        offset+=68;
        audio_mode=NV_AUDIO_RECORD_E;
        break;

    case ID_ENG_AUDIO_BT_EAR:
        //offset+=42;
        offset=110;
        audio_mode=NV_AUDIO_BTHS_E;
        break; 
    case ID_ENG_AUDIO_BT_MIC:
        offset=68;
        audio_mode=NV_AUDIO_BTHS_E;
        break;        
        
    case ID_ENG_AUDIO_MP3_LOUDSPK_SPEAKER:
        arm_audio_mode = AUDIO_DEVICE_MODE_HANDFREE;
        s_mp3_gain_array = &s_mp3_handsfree[0];
        break;
    case ID_ENG_AUDIO_MP3_EARPHONE_EAR:
        arm_audio_mode = AUDIO_DEVICE_MODE_EARPHONE;
        s_mp3_gain_array = &s_mp3_headset[0];
        break;
    case ID_ENG_AUDIO_MP3_HEADFREE_SPEAKER:
        arm_audio_mode = AUDIO_DEVICE_MODE_EARFREE;
        s_mp3_gain_array = &s_mp3_headfree[0];
        break;
    default:
        break;
    }   
    //SCI_TRACE_LOW:"EngAudioRead"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_6090_112_2_18_2_17_53_150,(uint8*)"");

    if(menu_id!=ID_ENG_AUDIO_MP3_LOUDSPK_SPEAKER
        &&menu_id!=ID_ENG_AUDIO_MP3_EARPHONE_EAR
        &&menu_id!=ID_ENG_AUDIO_MP3_HEADFREE_SPEAKER)
    {   
        
        if(SCI_ERROR==PROD_ReadAudioParam(audio_mode,&s_mode_param))
        {
            return SCI_ERROR;
        }

        if(menu_id==ID_ENG_AUDIO_BT_MIC||menu_id==ID_ENG_AUDIO_HANDSET_MIC
        ||menu_id==ID_ENG_AUDIO_HANDSFREE_MIC||menu_id==ID_ENG_AUDIO_MP4HFTR_MIC
        ||menu_id==ID_ENG_AUDIO_RECORD_MIC||menu_id==ID_ENG_AUDIO_HEADSET_MIC)
        {
            SCI_MEMCPY(vol_array,s_mode_param.tAudioStruct.extend2+offset,MMIENG_AUDIO_VOL_MAX*sizeof(uint16));
        }
        else
        {
            SCI_MEMCPY(vol_array,s_mode_param.tAudioStruct.extend+offset,MMIENG_AUDIO_VOL_MAX*sizeof(uint16));
        }
        for(iLoop = 0 ;iLoop<MMIENG_AUDIO_VOL_MAX;iLoop++)
        {
            vol_array[iLoop]=(int16)((20*MMIENG_Log((double)vol_array[iLoop]/4096,10)+0.5));/*lint !e747*/
            
            //SCI_TRACE_LOW:"EngAudioRead:log"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_6111_112_2_18_2_17_53_151,(uint8*)"");
            if(vol_array[iLoop]<-18)
            {
                vol_array[iLoop]=-18;
            }
            else if(vol_array[iLoop]>18)
            {
                vol_array[iLoop]=18;
            }
            if(vol_array[iLoop]<0)
            {
                vol_array[iLoop]+=17;//修正负数4舍5入情况
            }
            else
            {
                vol_array[iLoop]+=18;
            }
        }
             
    }
    else  //MP3 audio
    {       
        name_ptr = AUDIONVARM_GetAudioModeName(arm_audio_mode);
        if (PNULL == name_ptr)
        {
            SCI_TRACE_LOW("EngAudioRead:AUDIONVARM_GetAudioModeName name_ptr is null!\n");
            return SCI_ERROR;
        }
        name_len = strlen(name_ptr);
        if (name_len >= AUDIO_NV_ARM_MODE_NAME_MAX_LEN)
        {
            SCI_TRACE_LOW("EngAudioRead:AUDIONVARM_GetAudioModeName name_len>=AUDIO_NV_ARM_MODE_NAME_MAX_LEN!\n");
            return SCI_ERROR;
        }
        SCI_MEMCPY(audmod_arm_devmod_ptr->ucModeName, name_ptr, name_len);
        audmod_arm_devmod_ptr->ucModeName[name_len] = '\0';

        result = AUDIONVARM_GetModeParam(name_ptr, audmod_arm_devmod_ptr);
        if(result != AUDIO_NV_ARM_NO_ERROR)
        {
            SCI_TRACE_LOW("EngAudioRead:AUDIONVARM_GetModeParam result=%d!\n", result);
            return SCI_ERROR;
        }

        //to find the gain index in s_mp3_gain_array[];
        for(iLoop = 0 ;iLoop<MMIENG_MP3_AUDIO_VOL_MAX;iLoop++)
        {
            arm_volume = audmod_arm_devmod_ptr->tAudioNvArmModeStruct.app_config_info_set.app_config_info->arm_volume[iLoop+1];
            
            for(jLoop = 1 ;jLoop<MMIENG_AUDIO_GAIN_LEVEL_MAX;jLoop++)
            {               
                if(s_mp3_gain_array[jLoop].hex_value == arm_volume)
                {
                    vol_array[iLoop] = jLoop;
                    break;
                }
            } 

            if(MMIENG_AUDIO_GAIN_LEVEL_MAX == jLoop)  //if not in the array;
            {
                vol_array[iLoop] = (uint16)((MMIENG_AUDIO_GAIN_LEVEL_MAX-1)*iLoop)/(uint16)(MMIENG_MP3_AUDIO_VOL_MAX);
            }
        }
    }
    //SCI_TRACE_LOW:"EngAudioRead:end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_6135_112_2_18_2_17_53_152,(uint8*)"");
    return SCI_SUCCESS;
}
/********************************************************************************
 NAME:          EngARMLOGPOWERWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL uint32 EngAudioWrite(uint32 menu_id , int16 *vol_array)
{  
    NV_AUDIO_E   audio_mode=NV_AUDIO_HEADSET_E;
    AUDIO_DEVICE_MODE_TYPE_E arm_audio_mode = AUDIO_DEVICE_MODE_HANDFREE;
    AUDIO_NV_ARM_MODE_INFO_T *audmod_arm_devmod_ptr = &s_arm_audiomode_param;
    AUDIO_NV_ARM_RESULT_E    result = AUDIO_NV_ARM_NO_ERROR; 
#ifdef AUDIO_PARAM_ARM   
    AUDIO_RESULT_E set_devmode_result = AUDIO_NO_ERROR;
#endif
    uint offset=0;
    uint16 iLoop=0;
    uint16 jLoop=0; 
    uint16 buf=0;
    switch(menu_id)
    {
        
    case ID_ENG_AUDIO_HANDSET_RECEIVER:
        //offset+=18;
        offset=110;
        audio_mode=NV_AUDIO_HANDSET_E;        
        break;
    case ID_ENG_AUDIO_HANDSET_SIDETONE:
        //offset+=24;
        offset=92;
        audio_mode=NV_AUDIO_HANDSET_E;        
        break;
    case ID_ENG_AUDIO_HANDSET_MIC:
        offset=68;
        audio_mode=NV_AUDIO_HANDSET_E;        
        break;

    case ID_ENG_AUDIO_HEADSET_EAR:
        //offset+=42;
        offset=110;
        audio_mode=NV_AUDIO_HEADSET_E;        
        break;
    case ID_ENG_AUDIO_HEADSET_MIC:
        offset+=68;
        audio_mode=NV_AUDIO_HEADSET_E;        
        break;

    case ID_ENG_AUDIO_HANDSFREE_SPEAKER:
        //offset+=42;
        offset=110;
        audio_mode=NV_AUDIO_HANDSFREE_E;
        break;
    case ID_ENG_AUDIO_HANDSFREE_MIC:
        offset+=68;
        audio_mode=NV_AUDIO_HANDSFREE_E;
        break;

    case ID_ENG_AUDIO_MP4HFTP_SPEAKER:
        offset+=110;
        audio_mode=NV_AUDIO_MP4HFTP_E;        
        break;

    case ID_ENG_AUDIO_MP4HFTR_MIC:
        offset+=68;
        audio_mode=NV_AUDIO_MP4HFTR_E;
        break;

    case ID_ENG_AUDIO_MP4HEADSET_EAR:
        audio_mode=NV_AUDIO_MP4HEADSET_E;
        break;

    case ID_ENG_AUDIO_RECORD_MIC:
        offset+=68;
        audio_mode=NV_AUDIO_RECORD_E;
        break;

    case ID_ENG_AUDIO_BT_EAR:
        //offset+=42;
        offset=110;
        audio_mode=NV_AUDIO_BTHS_E;
        break; 
    case ID_ENG_AUDIO_BT_MIC:
        offset+=68;
        audio_mode=NV_AUDIO_BTHS_E;
        break; 

    case ID_ENG_AUDIO_MP3_LOUDSPK_SPEAKER:
        arm_audio_mode = AUDIO_DEVICE_MODE_HANDFREE;
        break;
    case ID_ENG_AUDIO_MP3_EARPHONE_EAR:
        arm_audio_mode = AUDIO_DEVICE_MODE_EARPHONE;
        break;
    case ID_ENG_AUDIO_MP3_HEADFREE_SPEAKER:
        arm_audio_mode = AUDIO_DEVICE_MODE_EARFREE;
        break;
    default:
        break;
    }       
    
    if(menu_id!=ID_ENG_AUDIO_MP3_LOUDSPK_SPEAKER
        &&menu_id!=ID_ENG_AUDIO_MP3_EARPHONE_EAR
        &&menu_id!=ID_ENG_AUDIO_MP3_HEADFREE_SPEAKER)
    {            
   
        buf=(uint16)(MMIENG_Pow(10.0,(double)(*s_volValue-18)/20)*4096);
        if(menu_id==ID_ENG_AUDIO_BT_MIC||menu_id==ID_ENG_AUDIO_HANDSET_MIC
        ||menu_id==ID_ENG_AUDIO_HANDSFREE_MIC||menu_id==ID_ENG_AUDIO_MP4HFTR_MIC
        ||menu_id==ID_ENG_AUDIO_RECORD_MIC||menu_id==ID_ENG_AUDIO_HEADSET_MIC)
        {
           *(s_mode_param.tAudioStruct.extend2+offset+(s_volValue-s_vol_array))=buf;
        }
        else
        {
            *(s_mode_param.tAudioStruct.extend+offset+(s_volValue-s_vol_array))=buf;
        }
        if(SCI_ERROR==PROD_WriteAudioParam(audio_mode,&s_mode_param,TRUE))
        {
            return SCI_ERROR;
        }
    }  
    else
    {
        //将1-35中的级别（数组下标）转化为相应的十六进制值
        for(iLoop = 0 ;iLoop<MMIENG_MP3_AUDIO_VOL_MAX;iLoop++)
        {
            jLoop = vol_array[iLoop];   //取下标值                                           //+1表示存储1-35
            audmod_arm_devmod_ptr->tAudioNvArmModeStruct.app_config_info_set.app_config_info->arm_volume[iLoop+1] = s_mp3_gain_array[jLoop].hex_value;    
        }
        //设置audio mode param
        result = AUDIONVARM_WriteModeParamToFlash((char *)audmod_arm_devmod_ptr->ucModeName, audmod_arm_devmod_ptr);
        result = AUDIONVARM_SetModeParam((char *)audmod_arm_devmod_ptr->ucModeName, audmod_arm_devmod_ptr);
        if(result != AUDIO_NV_ARM_NO_ERROR)
        {
            SCI_TRACE_LOW("EngAudioWrite:AUDIONVARM_SetModeParam result=%d\n", result);
            return SCI_ERROR;
        }
        else
        {          
#ifdef AUDIO_PARAM_ARM   
            set_devmode_result = AUDIO_SetDevModeEx(INVALID_HANDLE, arm_audio_mode);
            SCI_TRACE_LOW("EngAudioWrite:AUDIO_SetDevModeEx result=%d\n", set_devmode_result);
#else
            AUDIO_SetDevMode(arm_audio_mode);
#endif
        }
    }
    return SCI_SUCCESS;
}
/********************************************************************************
 NAME:          EngARMLOGPOWERWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngAudioSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{   
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint8 iLoop=0; 
    uint32 menu_id= (uint32)MMK_GetWinAddDataPtr(win_id);
    uint8 data_buf[20] = {0};   
    uint8 vol_max = MMIENG_AUDIO_VOL_MAX; 
    
    if(menu_id==ID_ENG_AUDIO_MP3_LOUDSPK_SPEAKER
        || menu_id==ID_ENG_AUDIO_MP3_EARPHONE_EAR
        || menu_id==ID_ENG_AUDIO_MP3_HEADFREE_SPEAKER)
    {
        vol_max = MMIENG_MP3_AUDIO_VOL_MAX;  //MP3为15级音量     
    }

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,vol_max, FALSE );//max item 2
        EngAudioRead(menu_id,s_vol_array);
        while(iLoop!=vol_max)
        {  
            if(menu_id!=ID_ENG_AUDIO_MP3_LOUDSPK_SPEAKER
                &&menu_id!=ID_ENG_AUDIO_MP3_EARPHONE_EAR
                &&menu_id!=ID_ENG_AUDIO_MP3_HEADFREE_SPEAKER)
            {
                sprintf((char*)data_buf, "%ld",s_vol_array[iLoop]);
            }
            else
            {   
                sprintf((char*)data_buf, "%g", s_mp3_gain_array[s_vol_array[iLoop]].total_gain); 
                SCI_TRACE_LOW("s_vol_array[%d]=%d\n",iLoop,s_vol_array[iLoop]);
            }
            MMIENG_AppendListItemByTextId(s_vol_table[iLoop++],TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT,data_buf);
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);     
        break;
        
  
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
     iLoop = GUILIST_GetCurItemIndex(ctrl_id);  
     s_volValue=&s_vol_array[iLoop];
     if(menu_id!=ID_ENG_AUDIO_MP3_LOUDSPK_SPEAKER
         &&menu_id!=ID_ENG_AUDIO_MP3_EARPHONE_EAR
         &&menu_id!=ID_ENG_AUDIO_MP3_HEADFREE_SPEAKER)
     {
            MMK_CreateWin((uint32*)MMIENG_AUDIOVOL_WIN_TAB, (ADD_DATA)(menu_id));
     }
     else
     {
          MMK_CreateWin((uint32*)MMIENG_AUDIOMP3_WIN_TAB, (ADD_DATA)(menu_id));
     }
        break;  
        
    case MSG_ENG_AUDIO_REFREH:  
        //EngAudioRead(menu_id,s_vol_array);
        GUILIST_RemoveAllItems( ctrl_id);
        iLoop=0;
        while(iLoop!=vol_max)
        {  
            if(menu_id!=ID_ENG_AUDIO_MP3_LOUDSPK_SPEAKER
                &&menu_id!=ID_ENG_AUDIO_MP3_EARPHONE_EAR
                &&menu_id!=ID_ENG_AUDIO_MP3_HEADFREE_SPEAKER)
            {
                sprintf((char*)data_buf, "%ld",s_vol_array[iLoop]);
            }
            else
            {   
                sprintf((char*)data_buf, "%g", s_mp3_gain_array[s_vol_array[iLoop]].total_gain); 
            }
            MMIENG_AppendListItemByTextId(s_vol_table[iLoop++],TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT,data_buf);
        }
        MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, 0);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : to append one item to listbox by text id and set soft keys
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/

LOCAL double MMIENG_Coef(int n)    
{    
    double t=0.0;
    if(n==0) return 0;    
    t=1.0/n;    
    if(n%2==0) t=-t;    
    return t;    
}//x^n的系数    


LOCAL double MMIENG_Horner(double x)    
{    
    const int N=100;  
    int i=0;
    double u=MMIENG_Coef(N);    
    for(i=N-1;i>=0;i--)    
        u=u*x+MMIENG_Coef(i);    
    return u;    
}    

LOCAL double MMIENG_Sqrt(double b)    
{    
    double x=1;int step=0;    
    while((x*x-b<-0.000000000000001||x*x-b>0.000000000000001)&&step<50)    
    {x=(b/x+x)/2.0;step++;}    
    return x;    
}//开平方    

LOCAL double MMIENG_Ln(double x)//ln(1+x)=x-x^2/2+x^3/3-x^4/4……    
{    
    int i=0;
    if(x>1.5)    
    {    
        for(;x>1.25;i++)    
            x=MMIENG_Sqrt(x);    
        return (1<<i)*MMIENG_Horner(x-1);   /*lint !e790*/ 
    }    
    else if(x<0.7&&x>0)    
    {    
        for(i=0;x<0.7;i++)    
            x=MMIENG_Sqrt(x);    
        return (1<<i)*MMIENG_Horner(x-1);    /*lint !e790*/ 
    }    
    else if(x>0) 
    {
        return MMIENG_Horner(x-1);
    }
    return 0;
}    
/*****************************************************************************/
//  Description : to append one item to listbox by text id and set soft keys
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
LOCAL double MMIENG_Log(double m,double base)    
{    
    if(m==0)
    {
        return 0;
    }
    return MMIENG_Ln(m)/MMIENG_Ln(base);//换底公式    
}  

LOCAL double MMIENG_Exp(double x)    
{    
    const int N=100; 
    double sum=1;   
    int i=0;
    for(i=N;i>0;i--)    
    {    
        sum/=i;    
        sum*=x;    
        sum+=1;    
    }    
    return sum;    
    
}//e^x≈1+x+x^2/2!+x^3/3!+……+x^n/n!    

LOCAL double MMIENG_Pow(double m,double n)    
{    
    return MMIENG_Exp(n*MMIENG_Ln(m));    
}   

/*****************************************************************************/
//  Description : to append one item to listbox by text id and set soft keys
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void MMIENG_AppendListItemByTextId(                                   
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style,
                                                uint8 * data_buf
                                                )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    wchar wbuf[30] = {0};

    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    
    item_data.item_content[1].item_data.text_buffer.wstr_len= strlen((char*)data_buf);
     item_data.item_content[1].item_data.text_buffer.wstr_ptr = wbuf;
    MMI_STRNTOWSTR(wbuf, 30, data_buf, 
        item_data.item_content[1].item_data.text_buffer.wstr_len,
        item_data.item_content[1].item_data.text_buffer.wstr_len );

    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;    
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );      
}

/********************************************************************************
 NAME:          EngPAWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngAudioVolWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{   
    uint8 data_buf[31] = {0};
    wchar wstr[31] = {0};
    int32 buff_length = 30;
    char* str = PNULL;
    MMI_STRING_T text = {0}; 
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 menu_id= (uint32)MMK_GetWinAddDataPtr(win_id);
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    const GUI_RECT_T client_rect = MMITHEME_GetClientRect();
    GUI_RECT_T          prg_rect = client_rect;   
    uint16      prg_rect_interval_from_win_border = (client_rect.right - client_rect.left + 1)/20;
    uint16      prg_rect_height =  2*GUIFONT_GetHeight(MMI_DEFAULT_TEXT_FONT);
    uint16      lab_rect_height =  GUIFONT_GetHeight(MMI_DEFAULT_TEXT_FONT);
    GUI_RECT_T lab_rect = client_rect;
    static int16 volume = 0;
        
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        prg_rect.top = (client_rect.bottom + client_rect.top) >> 1 ;
        prg_rect.bottom = prg_rect.top + prg_rect_height;
        prg_rect.left += prg_rect_interval_from_win_border;
        prg_rect.right -= prg_rect_interval_from_win_border;
        lab_rect.bottom = prg_rect.top - lab_rect_height;
        lab_rect.top  = lab_rect.bottom - lab_rect_height;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);       
        GUIPRGBOX_SetRect(MMIENG_PRGBOX_CTRL_ID,&prg_rect);
        GUIWIN_SetSoftkeyTextId(MMIENG_AudioVol_WIN_ID,  (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
        
        GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID, MMI_BACKGROUND_COLOR);
        LCD_FillRect(&lcd_dev_info, client_rect, MMI_BACKGROUND_COLOR);
        //GUILABEL_SetFontParam(MMIENG_LABEL1_CTRL_ID, mmi_eng_display.text_font, mmi_eng_display.spec_text_color, mmi_eng_display.char_space);
        GUILABEL_SetFont(MMIENG_LABEL1_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_RED_COLOR);

        volume = *s_volValue;
        
         sprintf((char*)data_buf, "%ld", volume);
        
        str = (char*)data_buf;
        text.wstr_len = strlen(str);
        text.wstr_ptr = wstr;
        MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, text.wstr_len, text.wstr_len );
        
        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);
        
       // volValue= (uint16)(uint32)(MMK_GetWinAddDataPtr(win_id));
        GUIPRGBOX_SetParamByCtrl(MMIENG_PRGBOX_CTRL_ID, MMIENG_AUDIO_SET_VOLMAX, MMIENG_AUDIO_SET_VOLMAX);
        GUIPRGBOX_SetPosByCtrl(FALSE,volume,volume,MMIENG_PRGBOX_CTRL_ID);
        
        break;
        
    case MSG_LCD_SWITCH:
         prg_rect.top = (client_rect.bottom + client_rect.top) >> 1 ;
        prg_rect.bottom = prg_rect.top + prg_rect_height;
        prg_rect.left += prg_rect_interval_from_win_border;
        prg_rect.right -= prg_rect_interval_from_win_border;
        lab_rect.bottom = prg_rect.top - lab_rect_height ;
        lab_rect.top  = lab_rect.bottom - lab_rect_height;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);       
        GUIPRGBOX_SetRect(MMIENG_PRGBOX_CTRL_ID,&prg_rect);
        break;
        
    case MSG_FULL_PAINT:
         LCD_FillRect(&lcd_dev_info, client_rect, MMI_BACKGROUND_COLOR);

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
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        //MMK_PostMsg(MMIENG_AUDIOSET_WIN_ID, MSG_ENG_AUDIO_REFREH, NULL, 0); 
        MMK_CloseWin(MMIENG_AudioVol_WIN_ID);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:    
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        *s_volValue = volume;
        EngAudioWrite(menu_id,s_vol_array);
        MMK_CloseWin(MMIENG_AudioVol_WIN_ID);
        MMK_PostMsg(MMIENG_AUDIOSET_WIN_ID, MSG_ENG_AUDIO_REFREH, NULL, 0);
        break;
        
    case MSG_KEYDOWN_LEFT:
        if (volume > 0)
        {
            volume-=1;
//             if (volume < 0)
//                     volume = 0;    
            sprintf((char*)data_buf, "%ld", volume);
            
            str = (char*)data_buf;
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, text.wstr_len, text.wstr_len );
            
            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,volume,volume,MMIENG_PRGBOX_CTRL_ID);
        }
        break;
        
    case MSG_KEYDOWN_RIGHT:
        if (volume < MMIENG_AUDIO_SET_VOLMAX)
        {
            volume += 1;
//             if (volume > MMIENG_AUDIO_SET_VOLMAX)
//                 volume = MMIENG_AUDIO_SET_VOLMAX;
            sprintf((char*)data_buf, "%ld", volume);
            
            str = (char*)data_buf;
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, text.wstr_len, text.wstr_len );
            
            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,volume,volume,MMIENG_PRGBOX_CTRL_ID);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_MOVE:
        if(param == 0) break;
        {
            GUI_RECT_T progressbar_rect = {0};
            uint16 progressbar_length = 0;
            GUI_POINT_T tp_point = {0};
            
            GUIAPICTRL_GetRect(MMIENG_PRGBOX_CTRL_ID,&progressbar_rect); /*lint !e516*/
            progressbar_length = (progressbar_rect.right - progressbar_rect.left + 1);
            tp_point.x= MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
            
            if(!GUI_PointIsInRect(tp_point,progressbar_rect)) break;

            volume = (tp_point.x - progressbar_rect.left + 1)*MMIENG_AUDIO_SET_VOLMAX/progressbar_length;
            sprintf((char*)data_buf, "%ld", volume);            
            str = (char*)data_buf;
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, text.wstr_len, text.wstr_len );
            
            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,volume,volume,MMIENG_PRGBOX_CTRL_ID);
            
             
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
/********************************************************************************
 NAME:          MMIENG_DextoInt
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        
 DATE:      
********************************************************************************/
LOCAL int16 MMIENG_DextoInt(uint8 * buf)
{
    uint8 * pTemp=buf;
    int16 result=0;
    while(*pTemp!='\0')
    {
           if((*pTemp)>='0'&&(*pTemp)<='9')
           {
                  *pTemp-='0';
           }
           else if((*pTemp)>='a'&&(*pTemp)<='f')
           {
               *pTemp=*pTemp-'a'+10;
           }
           else if((*pTemp)>='A'&&(*pTemp)<='F')
           {
               *pTemp=*pTemp-'A'+10;
           }
           else
           {    
               result = -1;
               break;
           }
           result=result*16+*pTemp;
           pTemp++;
    }
    return result;
}

/********************************************************************************
 NAME:          AudioMp3AppendListItem
 DESCRIPTION:   Used for Mp3 volume edit window
 PARAM IN:      menu_id - which MMIENG_AUDIO_MP3_GAIN_T array to be used 
                index - selected item index
 AUTHOR:        leon.wang
 DATE:          2012.9.11
********************************************************************************/
LOCAL void AudioMp3AppendListItem(uint16 index)
{
    MMI_CTRL_ID_T           list_ctrl_id = MMIENG_MP3_LISTBOX_CTRL_ID;
    wchar                   txt_array[10] = {0};
    char                    buf[10] = {0};
    uint16                  txt_len = 0;
    uint16                  iloop = 0;
    
    GUILIST_SetMaxItem(list_ctrl_id,MMIENG_AUDIO_GAIN_LEVEL_MAX, FALSE);

    for(iloop=0; iloop< MMIENG_AUDIO_GAIN_LEVEL_MAX; iloop++)
    {       
        sprintf(buf, "%g", s_mp3_gain_array[iloop].total_gain);
        SCI_MEMSET(txt_array, 0,  10*sizeof(wchar));
        txt_len = SCI_STRLEN(buf);
        txt_len = MIN(txt_len,9);
        MMI_STRNTOWSTR(txt_array, 9, (uint8*)buf, 9, txt_len);
        MMIAPISET_AppendListItemByTextBuffer(txt_array, 
                                            txt_len, 
                                            0,
                                            TXT_ENG_OK, 
                                            TXT_NULL, 
                                            TXT_ENG_RETURN, 
                                            list_ctrl_id,
                                            GUIITEM_STYLE_ONE_LINE_RADIO
                                            );
    }
    
    GUILIST_SetSelectedItem(list_ctrl_id, index, TRUE);
    GUILIST_SetCurItemIndex(list_ctrl_id, index);
} 

/********************************************************************************
 NAME:          EngWapUAWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        
 DATE:      
********************************************************************************/

LOCAL MMI_RESULT_E EngAudioMp3WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    uint32  menu_id= (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T ctrl_id = MMIENG_MP3_LISTBOX_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:   
        AudioMp3AppendListItem((uint16)*s_volValue);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
        
            switch (src_id)
            {
            case MMIENG_MP3_LISTBOX_CTRL_ID:
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                 MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                 break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                 MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            default:
                break;
            }
        }
        break;
#endif
 //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK: 
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {        
            //save to nv
            int16 volume = GUILIST_GetCurItemIndex(ctrl_id);

            *s_volValue = volume;
            EngAudioWrite(menu_id,s_vol_array); 
            MMK_PostMsg(MMIENG_AUDIOSET_WIN_ID, MSG_ENG_AUDIO_REFREH, NULL, 0);
            MMK_CloseWin(win_id);
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#ifdef WIN32
/*****************************************************************************/
//  Description:    The function read PROD parameters about audio from NVItem .
//  Global resource dependence: 
//  Author:         Benjamin.Wang
//  Note:           
/*****************************************************************************/
PUBLIC uint32 PROD_ReadAudioParam(  
             NV_AUDIO_E   audio_mode,
             void*        mode_param_ptr
             )
{


    return SCI_SUCCESS;
    
}
/*****************************************************************************/
//  Description:    The function write PROD parameters about audio to NVItem .
//  Global resource dependence: 
//  Author:         Benjamin.Wang
//  Note:           is_flush：SCI_FALSE-〉表示数据只写入CACHE，SCI_TRUE->表示数据写入NV，请flush数据Cache
/*****************************************************************************/
PUBLIC uint32 PROD_WriteAudioParam(
    //const uint8* pucModeName, 
    NV_AUDIO_E   audio_mode, 
    void*        mode_param_ptr,
    BOOLEAN      is_flush
    )
{   
    return SCI_SUCCESS;    
}
#endif

/********************************************************************************
 NAME:          EngARMLOGPOWERWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngCAPOUTPUTWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{   
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    BOOLEAN is_on = FALSE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2        
        MMINV_READ(MMIENG_NV_CAP_OUTPUT,&is_on,return_value);        
        if(MN_RETURN_SUCCESS!=return_value)
        {
#ifdef MMI_GPRS_SUPPORT
            is_on = TCPIPDEBUG_IsPktOutputEnable();
#endif
            MMINV_WRITE(MMIENG_NV_CAP_OUTPUT,&is_on);
        }             
 
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_OFF,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_ON,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);         

        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, is_on, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,is_on);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);     
        break;
        
    case MSG_FULL_PAINT:
        break;  
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        is_on = GUILIST_GetCurItemIndex(ctrl_id);       
        MMINV_WRITE(MMIENG_NV_CAP_OUTPUT,&is_on);
#ifdef MMI_GPRS_SUPPORT
#ifndef _WIN32
        TCPIPDEBUG_EnablePktOutput( is_on );
#endif
#endif
       // MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);       
        MMK_CloseWin(win_id);
        break;      
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/********************************************************************************
 NAME:          EngContrastWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
//LOCAL MMI_RESULT_E EngContrastWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
//{
//  GUI_RECT_T rect = {ENG_CLIENT_LEFT, ENG_CLIENT_TOP, ENG_CLIENT_RIGHT, ENG_CLIENT_BOTTOM};
//  uint8 data_buf[20] = {0};
//  MMI_STRING_T text = {0};
//  MMI_RESULT_E result = MMI_RESULT_TRUE;
//  GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
//      
//  switch (msg_id) 
//  {
//  case MSG_OPEN_WINDOW:
//      LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
//      
//      GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID, mmi_eng_display.bkgd_color);
//      // first get the LCD 's min & max contrast value
//      // at the same time, get the current LCD 's contrast real value(not grade value)
//      #ifndef _WIN32
//      GUIREF_LCDGetInfo(SUB_LCD_ID, &s_mmieng_lcd_info);
//      s_mmieng_contrast_value = MMIAPISET_GetSubLCDContrastValue(SUB_LCD_ID);
//      LCD_SetContrast(SUB_LCD_ID, s_mmieng_contrast_value);
//      //s_mmieng_contrast_value = s_mmieng_lcd_info.contrast_defaut;
//      #else
//      s_mmieng_lcd_info.contrast_min = 0;
//      s_mmieng_lcd_info.contrast_max = 63;
//      s_mmieng_contrast_value = 16;
//      #endif
//      SCI_PASSERT(s_mmieng_lcd_info.contrast_max > s_mmieng_lcd_info.contrast_min, 
//          ("EngContrastWinHandleMsg: s_mmieng_lcd_info.contrast_max is %d,s_mmieng_lcd_info.contrast_min is %d",s_mmieng_lcd_info.contrast_max,s_mmieng_lcd_info.contrast_min));
//      SCI_PASSERT(s_mmieng_contrast_value >= s_mmieng_lcd_info.contrast_min, 
//          ("EngContrastWinHandleMsg: s_mmieng_contrast_value is %d",s_mmieng_contrast_value));
//
//      GUIPRGBOX_SetPosByCtrl(FALSE,
//            s_mmieng_contrast_value - s_mmieng_lcd_info.contrast_min,
//          s_mmieng_contrast_value - s_mmieng_lcd_info.contrast_min,
//            s_mmieng_lcd_info.contrast_max - s_mmieng_lcd_info.contrast_min,
//            MMIENG_PRGBOX_CTRL_ID);
//      break;
//      
//  case MSG_FULL_PAINT:
//              LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
//      SCI_MEMSET((char*)data_buf, 0, 20);
//      sprintf((char*)data_buf, "%d", s_mmieng_contrast_value);
//      text.wstr_ptr = data_buf;
//      text.wstr_len = SCI_STRLEN((char*)data_buf);
//      GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);
//      GUIWIN_SetSoftkeyTextId(MMIENG_CONTRAST_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
//      break;
//      
//  case MSG_KEYDOWN_FLIP:      // close flip
//  case MSG_KEYDOWN_RED:
//      MMIAPISET_ResetSubLcdContrast(SUB_LCD_ID);
//      MMK_CloseWin(MMIENG_CONTRAST_WIN_ID);
//      break;
//
//  case MSG_KEYDOWN_OK:
//  case MSG_CTL_OK:
//    case MSG_APP_WEB:
//      break;
//
//    case MSG_KEYDOWN_CANCEL:      
//  case MSG_CTL_CANCEL:
//      //not restore the current contrast value
//      MMIAPISET_ResetSubLcdContrast(SUB_LCD_ID);
//      MMK_CloseWin(MMIENG_CONTRAST_WIN_ID);
//      break;
//  
//  case MSG_KEYDOWN_UP:
//  case MSG_KEYLONG_UP:
//      if (s_mmieng_contrast_value < s_mmieng_lcd_info.contrast_max)
//      {
//          s_mmieng_contrast_value += (s_mmieng_lcd_info.contrast_max - s_mmieng_lcd_info.contrast_min) / 10;
//          if (s_mmieng_contrast_value > s_mmieng_lcd_info.contrast_max)
//              s_mmieng_contrast_value = s_mmieng_lcd_info.contrast_max;
//          sprintf((char*)data_buf, "%d", s_mmieng_contrast_value);
//          text.wstr_ptr = data_buf;
//          text.wstr_len = SCI_STRLEN((char*)data_buf);
//          GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
//            GUIPRGBOX_SetPosByCtrl(TRUE,
//                s_mmieng_contrast_value - s_mmieng_lcd_info.contrast_min,
//              s_mmieng_contrast_value - s_mmieng_lcd_info.contrast_min,
//                s_mmieng_lcd_info.contrast_max - s_mmieng_lcd_info.contrast_min,
//                MMIENG_PRGBOX_CTRL_ID);
//      
//          #ifndef  _WIN32
//          LCD_SetContrast(SUB_LCD_ID, s_mmieng_contrast_value);
//          #endif
//      }
//      break;
//      
//  case MSG_KEYDOWN_DOWN:
//  case MSG_KEYLONG_DOWN:
//      if (s_mmieng_contrast_value > s_mmieng_lcd_info.contrast_min)
//      {
//          s_mmieng_contrast_value -= (s_mmieng_lcd_info.contrast_max - s_mmieng_lcd_info.contrast_min) / 10;
//          if (s_mmieng_contrast_value < s_mmieng_lcd_info.contrast_min)
//              s_mmieng_contrast_value = s_mmieng_lcd_info.contrast_min;
//          sprintf((char*)data_buf, "%d", s_mmieng_contrast_value);
//          text.wstr_ptr = data_buf;
//          text.wstr_len = SCI_STRLEN((char*)data_buf);
//          GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
//            GUIPRGBOX_SetPosByCtrl(TRUE,
//                s_mmieng_contrast_value - s_mmieng_lcd_info.contrast_min,
//              s_mmieng_contrast_value - s_mmieng_lcd_info.contrast_min,
//                s_mmieng_lcd_info.contrast_max - s_mmieng_lcd_info.contrast_min,
//                MMIENG_PRGBOX_CTRL_ID);
//      
//          #ifndef  _WIN32
//          LCD_SetContrast(SUB_LCD_ID, s_mmieng_contrast_value);
//          #endif
//      }
//      break;
//      
//  case MSG_KEYDOWN_LEFT:
//  case MSG_KEYLONG_LEFT:
//      if (s_mmieng_contrast_value > s_mmieng_lcd_info.contrast_min)
//      {
//          s_mmieng_contrast_value--;
//          sprintf((char*)data_buf, "%d", s_mmieng_contrast_value);
//          text.wstr_ptr = data_buf;
//          text.wstr_len = SCI_STRLEN((char*)data_buf);
//          GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
//            GUIPRGBOX_SetPosByCtrl(TRUE,
//                s_mmieng_contrast_value - s_mmieng_lcd_info.contrast_min,
//              s_mmieng_contrast_value - s_mmieng_lcd_info.contrast_min,
//                s_mmieng_lcd_info.contrast_max - s_mmieng_lcd_info.contrast_min,
//                MMIENG_PRGBOX_CTRL_ID);
//          
//          #ifndef  _WIN32
//          LCD_SetContrast(SUB_LCD_ID, s_mmieng_contrast_value);
//          #endif
//      }
//      break;
//      
//  case MSG_KEYDOWN_RIGHT:
//  case MSG_KEYLONG_RIGHT:
//      if (s_mmieng_contrast_value < s_mmieng_lcd_info.contrast_max)
//      {
//          s_mmieng_contrast_value++;
//          sprintf((char*)data_buf, "%d", s_mmieng_contrast_value);
//          text.wstr_ptr = data_buf;
//          text.wstr_len = SCI_STRLEN((char*)data_buf);
//          GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
//          GUIPRGBOX_SetPosByCtrl(TRUE,
//                s_mmieng_contrast_value - s_mmieng_lcd_info.contrast_min,
//              s_mmieng_contrast_value - s_mmieng_lcd_info.contrast_min,
//                s_mmieng_lcd_info.contrast_max - s_mmieng_lcd_info.contrast_min,
//                MMIENG_PRGBOX_CTRL_ID);
//      
//          #ifndef  _WIN32
//          LCD_SetContrast(SUB_LCD_ID, s_mmieng_contrast_value);
//          #endif
//      }
//      break;
//      
//  case MSG_CLOSE_WINDOW:
//      SCI_MEMSET(&s_mmieng_lcd_info,0,sizeof(LCD_INFO_T));
//      if(PNULL != s_mmieng_lcd_info.lcdbuff_ptr)
//      {
//          SCI_Free(s_mmieng_lcd_info.lcdbuff_ptr);
//      }
//      
//      break;
//  
//  default:
//      result = MMI_RESULT_FALSE;
//      break;
//  }
//  return (result);
//}

/********************************************************************************
 NAME:          EngBrightnessWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngBrightnessWinHandleMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    uint8               data_buf[20]    = {0};
    wchar wstr[20] = {0};
    char* str = PNULL;
    MMI_RESULT_E        result          = MMI_RESULT_TRUE;
    MMI_STRING_T        text            = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T         client_rect            = MMITHEME_GetClientRect();
    GUI_RECT_T          prg_rect =  client_rect;
    GUI_RECT_T          lab_rect = client_rect;
        
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        lab_rect.bottom = ((client_rect.top + client_rect.bottom )>>1) - 1;
        lab_rect.top = lab_rect.bottom - 40;
        prg_rect.top = ((client_rect.top + client_rect.bottom )>>1 )+ 1;
        prg_rect.bottom = prg_rect.top + 30;
        prg_rect.left += 1;
        prg_rect.right -= 1;        
        
         GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE); 
    GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID,  mmi_eng_display.bkgd_color);
        GUIPRGBOX_SetRect(MMIENG_PRGBOX_CTRL_ID,&prg_rect);
    
        s_mmieng_brightness_value = MMIAPISET_GetRealBacklightValue();
    sprintf((char*)data_buf, "%d", s_mmieng_brightness_value);
            
            str = (char*)data_buf;
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 20, (uint8*)str, text.wstr_len, text.wstr_len );

        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);          
        GUIPRGBOX_SetParamByCtrl(MMIENG_PRGBOX_CTRL_ID, s_mmieng_save_brightness_value - 1, s_mmieng_save_brightness_value - 1);
            GUIPRGBOX_SetPosByCtrl(FALSE,s_mmieng_brightness_value - 1,s_mmieng_brightness_value - 1,MMIENG_PRGBOX_CTRL_ID);
            
        #ifndef WIN32
        LCD_SetBackLightBrightness(s_mmieng_brightness_value);
        #endif
        break;

    case MSG_LCD_SWITCH:
        lab_rect.bottom = ((client_rect.top + client_rect.bottom )>>1) - 1;
        lab_rect.top = lab_rect.bottom - 40;
        prg_rect.top = ((client_rect.top + client_rect.bottom )>>1 )+ 1;
        prg_rect.bottom = prg_rect.top + 30;
        prg_rect.left += 1;
        prg_rect.right -= 1;        
        
         GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);       
        GUIPRGBOX_SetRect(MMIENG_PRGBOX_CTRL_ID,&prg_rect);
        break;
        
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, client_rect, mmi_eng_display.bkgd_color);
/*      SCI_MEMSET((char*)data_buf, 0, 20);
        sprintf((char*)data_buf, "%d", s_mmieng_brightness_value);


        str = (char*)data_buf;
        text.wstr_len = strlen(str);
        text.wstr_ptr = wstr;
        MMI_STRNTOWSTR( wstr, 20, str, text.wstr_len, text.wstr_len );

        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
    */  
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPISET_ResetContrast(MAIN_LCD_ID);
        break;          
    
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:        
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(win_id);
        break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMIAPISET_ResetContrast(MAIN_LCD_ID);
        MMK_CloseWin(win_id);
        break;

    case MSG_LOSE_FOCUS:
        MMIAPISET_ResetContrast(MAIN_LCD_ID);
        break;

    case MSG_GET_FOCUS:     
         GUIPRGBOX_SetPosByCtrl(FALSE,s_mmieng_brightness_value - 1,s_mmieng_brightness_value - 1,MMIENG_PRGBOX_CTRL_ID);
        
#ifndef  _WIN32
            LCD_SetBackLightBrightness(s_mmieng_brightness_value);
#endif
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYPRESSUP_DOWNSIDE:
    case MSG_KEYPRESSUP_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        break;
    case MSG_KEYLONG_DOWNSIDE:
#else
    case MSG_KEYDOWN_DOWN:
#endif
        if (s_mmieng_brightness_value - 10 > 0)
        {
            s_mmieng_brightness_value -= 10;
        }
        else
        {
            s_mmieng_brightness_value = 1;
        }
        {
            sprintf((char*)data_buf, "%d", s_mmieng_brightness_value);
            
            str = (char*)data_buf;
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 20, (uint8*)str, text.wstr_len, text.wstr_len );

        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,s_mmieng_brightness_value - 1,s_mmieng_brightness_value - 1,MMIENG_PRGBOX_CTRL_ID);
        
            #ifndef  _WIN32
            LCD_SetBackLightBrightness(s_mmieng_brightness_value);
            #endif
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_KEYLONG_UPSIDE:
#else
    case MSG_KEYDOWN_UP:
#endif
        if (s_mmieng_brightness_value + 10 <= s_mmieng_save_brightness_value)
        {
            s_mmieng_brightness_value +=10;
        }
        else
        {
            s_mmieng_brightness_value=s_mmieng_save_brightness_value;
        }

        {   
            sprintf((char*)data_buf, "%d", s_mmieng_brightness_value);

            str = (char*)data_buf;
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 20, (uint8*)str, text.wstr_len, text.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,s_mmieng_brightness_value - 1,s_mmieng_brightness_value - 1,MMIENG_PRGBOX_CTRL_ID);
        
            #ifndef  _WIN32             
            LCD_SetBackLightBrightness(s_mmieng_brightness_value);
            #endif
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_KEYUP_DOWNSIDE:
#else
    case MSG_KEYDOWN_LEFT:
#endif
        if (s_mmieng_brightness_value > 1)
        {
            s_mmieng_brightness_value--;
            
            sprintf((char*)data_buf, "%d", s_mmieng_brightness_value);
            
            str = (char*)data_buf;
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 20, (uint8*)str, text.wstr_len, text.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,s_mmieng_brightness_value - 1,s_mmieng_brightness_value - 1,MMIENG_PRGBOX_CTRL_ID);
        
            #ifndef  _WIN32             
            LCD_SetBackLightBrightness(s_mmieng_brightness_value);
            #endif
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_KEYUP_UPSIDE:
#else
    case MSG_KEYDOWN_RIGHT:
#endif
        if (s_mmieng_brightness_value < s_mmieng_save_brightness_value)
        {
            s_mmieng_brightness_value++;
            
            sprintf((char*)data_buf, "%d", s_mmieng_brightness_value);
            
            str = (char*)data_buf;
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 20, (uint8*)str, text.wstr_len, text.wstr_len );

            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
            GUIPRGBOX_SetPosByCtrl(TRUE,s_mmieng_brightness_value - 1,s_mmieng_brightness_value - 1,MMIENG_PRGBOX_CTRL_ID);
        
            #ifndef  _WIN32
            LCD_SetBackLightBrightness(s_mmieng_brightness_value);
            #endif
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/********************************************************************************
 NAME:          ShowSetAudioParamResult
 DESCRIPTION:   
 PARAM IN:      result - 
 PARAM OUT:     None
 AUTHOR:        allen
 DATE:          2004.11.08
********************************************************************************/
LOCAL void ShowSetAudioParamResult(BOOLEAN result)
{
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 
    
    text1.wstr_ptr = s_set_wstr; /*lint !e605*/ //"Set";
    text1.wstr_len = 3;
    text2.wstr_ptr = s_audio_param_wstr;/*lint !e605*/ //"audio param";
    text2.wstr_len = 11;
    if (result)
    {
        text3.wstr_ptr = s_success_wstr;/*lint !e605*/ //"success";
        text3.wstr_len = 7;
    }
    else
    {
        text3.wstr_ptr = s_fail_wstr;/*lint !e605*/ //"fail";
        text3.wstr_len = 4;
    }
    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:          EngSidetoneWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngSidetoneWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{   
    MMI_STRING_T side_tone = {PNULL, 0};
#ifndef _WIN32
    AUDIO_TEST_DATA_T   audio_data = {0};
#endif
    MMI_RESULT_E result = MMI_RESULT_TRUE;  
        
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        // set window 's title
        switch (g_mmieng_voice_param)
        {
            case MMI_HANDHOLD_SIDETONE_GAIN:
                s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
                GUIWIN_SetTitleTextId(MMIENG_SIDETONE_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_HDSTSDTONE, FALSE);
                break;
                
            case MMI_EARPHONE_SIDETONE_GAIN:
                s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_EARPHONE;
                GUIWIN_SetTitleTextId(MMIENG_SIDETONE_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_EARPHSDTONE, FALSE);
                break;

            default:
                break;
        }
        //rect.bottom = rect.top + 40;
        //GUIEDIT_SetStyle(MMIENG_EDITBOX1_CTRL_ID, GUIEDIT_STYLE_SINGLE);
        //GUIEDIT_SetRect(MMK_ConvertIdToHandle(MMIENG_EDITBOX1_CTRL_ID),  &rect);      
        GUIEDIT_SetSoftkey(MMIENG_EDITBOX1_CTRL_ID, 0, 1, TXT_NULL, TXT_ENG_OK, PNULL);
        MMK_SetAtvCtrl(MMIENG_SIDETONE_WIN_ID, MMIENG_EDITBOX1_CTRL_ID);
        break;
/*
    case MSG_LCD_SWITCH:
        rect.bottom = rect.top + 40;
        GUIEDIT_SetRect(MMK_ConvertIdToHandle(MMIENG_EDITBOX1_CTRL_ID),  &rect);
        break;
    */  
    case MSG_FULL_PAINT:
     //   {
     //       GUI_RECT_T rect = MMITHEME_GetClientRect();     
     //       GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
   //         LCD_FillRect(&lcd_dev_info, rect, MMI_WINDOW_BACKGROUND_COLOR);
   //    }
        //GUIWIN_SetSoftkeyTextId(MMIENG_SIDETONE_WIN_ID,  (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_SIDETONE_WIN_ID);
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
                MMK_SendMsg(win_id,MSG_APP_OK, PNULL);

                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_MIDSK: 
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIEDIT_GetString(MMIENG_EDITBOX1_CTRL_ID, &side_tone);
        if (side_tone.wstr_len > 0)
        {
            #ifndef WIN32
            if (AUDIO_NO_ERROR == AUD_GetEngineeringParameter(s_mmieng_audio_dev_mode, &audio_data))
            {
                audio_data.digital_sidetone_gain = atoi((char*)side_tone.wstr_ptr);/*lint !e63*/

                AUD_SetEngineeringParameter(s_mmieng_audio_dev_mode, audio_data);
            }
            else
            {
                ShowSetAudioParamResult(FALSE);
            }
            #endif
            MMK_CloseWin(MMIENG_SIDETONE_WIN_ID);
        }
        //else
        //{
        //  MMK_SetAtvCtrl(MMIENG_SIDETONE_WIN_ID, MMIENG_EDITBOX1_CTRL_ID);
        //}
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngVoiceParamWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngVoiceParamWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    wchar wstr[30] = {0};
    char temp_str[30] = {0};
    char* str = PNULL;
    uint16 length= 30;
    MMI_STRING_T up_link = {PNULL, 0};
    MMI_STRING_T down_link = {PNULL, 0};
#ifndef _WIN32
     AUDIO_TEST_DATA_T audio_data = {0};
#endif
    MMI_RESULT_E result = MMI_RESULT_TRUE;
 
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMIENG_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMIENG_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetType(MMIENG_FORM_CTRL_ID, GUIFORM_TYPE_TP);
		
        str = "Uplink:";
        text.wstr_len = strlen(str);
        text.wstr_ptr = wstr;
        MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text.wstr_len, text.wstr_len );

        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);

        str = "Downlink:";
        text.wstr_len = strlen(str);
        text.wstr_ptr = wstr;
        MMI_STRNTOWSTR( wstr, 30, (uint8*)str, text.wstr_len, text.wstr_len );

        GUILABEL_SetText(MMIENG_LABEL2_CTRL_ID, &text, FALSE);
        // set window 's title
        switch (g_mmieng_voice_param)
        {
            case MMI_HANDHOLD_DIGITAL_GAIN:
                s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
                GUIWIN_SetTitleTextId(MMIENG_VOICEPARAM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_HDSTDGT, FALSE);
                break;
                
            case MMI_HANDFREE_DIGITAL_GAIN:
                s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_HANDFREE;
                GUIWIN_SetTitleTextId(MMIENG_VOICEPARAM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_HDFREDGT, FALSE);
                break;
                
            case MMI_EARPHONE_DIGITAL_GAIN:
                s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_EARPHONE;
                GUIWIN_SetTitleTextId(MMIENG_VOICEPARAM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_EARPHDGT, FALSE);
                break;
                
            case MMI_HANDHOLD_PGA_GAIN:
                s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
                GUIWIN_SetTitleTextId(MMIENG_VOICEPARAM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_HDSTPGA, FALSE);
                break;
                
            case MMI_HANDFREE_PGA_GAIN:
                s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_HANDFREE;
                GUIWIN_SetTitleTextId(MMIENG_VOICEPARAM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_HDFREPGA, FALSE);
                break;
            
            case MMI_EARPHONE_PGA_GAIN:
                s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_EARPHONE;
                GUIWIN_SetTitleTextId(MMIENG_VOICEPARAM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_EARPHPGA, FALSE);
                break;
                
            case MMI_HANDHOLD_DIGITAL_SCALE:
                s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
                GUIWIN_SetTitleTextId(MMIENG_VOICEPARAM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_HDSTSCL, FALSE);
                break;
                
            case MMI_HANDFREE_DIGITAL_SCALE:
                s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_HANDFREE;
                GUIWIN_SetTitleTextId(MMIENG_VOICEPARAM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_HDFRESCL, FALSE);
                break;
                
            case MMI_EARPHONE_DIGITAL_SCALE:
                s_mmieng_audio_dev_mode = AUDIO_DEVICE_MODE_EARPHONE;
                GUIWIN_SetTitleTextId(MMIENG_VOICEPARAM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_EARPHSCL, FALSE);
                break;
                
            default:
                break;
        }
        GUIEDIT_SetStyle(MMIENG_EDITBOX1_CTRL_ID, GUIEDIT_STYLE_SINGLE);
        GUIEDIT_SetSoftkey(MMIENG_EDITBOX1_CTRL_ID,2,1,TXT_ENG_RETURN,TXT_ENG_CLEAR,PNULL);
        GUIEDIT_SetStyle(MMIENG_EDITBOX2_CTRL_ID, GUIEDIT_STYLE_SINGLE);
        GUIEDIT_SetSoftkey(MMIENG_EDITBOX2_CTRL_ID,2,1,TXT_ENG_RETURN,TXT_ENG_CLEAR,PNULL);
        MMK_SetAtvCtrl(MMIENG_VOICEPARAM_WIN_ID, MMIENG_EDITBOX1_CTRL_ID);
 #ifndef WIN32       
        // first input the up link
        s_mmieng_up_down_link = 0;

        if (AUDIO_NO_ERROR != AUD_GetEngineeringParameter(s_mmieng_audio_dev_mode, &audio_data))
        {
            //error
            ShowSetAudioParamResult(FALSE);
        }

        else
        {
            audio_data.UL_digital_gain = 101;
            audio_data.DL_digital_gain = 102;
            audio_data.UL_PGA_gain = 201;
            audio_data.DL_PGA_gain = 202;
            audio_data.UL_digital_scale = 301;
            audio_data.DL_digital_scale = 302;
         }
#endif
        switch (g_mmieng_voice_param)
        {
        case MMI_HANDHOLD_DIGITAL_GAIN:
        case MMI_HANDFREE_DIGITAL_GAIN:
        case MMI_EARPHONE_DIGITAL_GAIN:
#ifndef _WIN32
            SCI_MEMSET(temp_str, 0 , length);                
            SCI_MEMSET(wstr, 0 , length*2);
            length = sprintf(temp_str, "%d", audio_data.UL_digital_gain);
            MMI_STRNTOWSTR(wstr, 30, (uint8*)temp_str, length, length);
            GUIEDIT_SetString(MMIENG_EDITBOX1_CTRL_ID, wstr,length);
            
            SCI_MEMSET(temp_str, 0 , length);
            SCI_MEMSET(wstr, 0 , length*2);
            length = sprintf(temp_str, "%d", audio_data.DL_digital_gain);
            MMI_STRNTOWSTR(wstr, 30, (uint8*)temp_str, length, length);
            GUIEDIT_SetString(MMIENG_EDITBOX2_CTRL_ID, wstr,length);
#endif
            break;
            
            
        case MMI_HANDHOLD_PGA_GAIN:
        case MMI_HANDFREE_PGA_GAIN:
        case MMI_EARPHONE_PGA_GAIN:
#ifndef _WIN32
            SCI_MEMSET(temp_str, 0 , length);                
            SCI_MEMSET(wstr, 0 , length*2);
            length = sprintf(temp_str, "%d", audio_data.UL_PGA_gain);
            MMI_STRNTOWSTR(wstr, 30, (uint8*)temp_str, length, length);
            GUIEDIT_SetString(MMIENG_EDITBOX1_CTRL_ID, wstr,length);
            
            SCI_MEMSET(temp_str, 0 , length);
            SCI_MEMSET(wstr, 0 , length*2);
            length = sprintf(temp_str, "%d", audio_data.DL_PGA_gain);
            MMI_STRNTOWSTR(wstr, 30, (uint8*)temp_str, length, length);
            GUIEDIT_SetString(MMIENG_EDITBOX2_CTRL_ID, wstr,length);
#endif
            break;
            
        case MMI_HANDHOLD_DIGITAL_SCALE:
        case MMI_HANDFREE_DIGITAL_SCALE:
        case MMI_EARPHONE_DIGITAL_SCALE:   
#ifndef _WIN32            
            SCI_MEMSET(temp_str, 0 , length);                
            SCI_MEMSET(wstr, 0 , length*2);
            length = sprintf(temp_str, "%d", audio_data.UL_digital_scale);
            MMI_STRNTOWSTR(wstr, 30, (uint8*)temp_str, length, length);
            GUIEDIT_SetString(MMIENG_EDITBOX1_CTRL_ID, wstr,length);
            
            SCI_MEMSET(temp_str, 0 , length);
            SCI_MEMSET(wstr, 0 , length*2);
            length = sprintf(temp_str, "%d", audio_data.DL_digital_scale);
            MMI_STRNTOWSTR(wstr, 30, (uint8*)temp_str, length, length);
            GUIEDIT_SetString(MMIENG_EDITBOX2_CTRL_ID, wstr,length);
#endif
            break;
            
            
        default:
            break;
        }
        
        break;  

    /*      
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, client_rect, mmi_eng_display.bkgd_color);
        GUIWIN_SetSoftkeyTextId(MMIENG_VOICEPARAM_WIN_ID,  (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
        break;
*/

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_VOICEPARAM_WIN_ID);
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
                MMK_SendMsg(win_id,MSG_APP_OK, PNULL);

                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIEDIT_GetString(MMIENG_EDITBOX1_CTRL_ID, &up_link);
        GUIEDIT_GetString(MMIENG_EDITBOX2_CTRL_ID, &down_link);
        if (0 == up_link.wstr_len)
        {
            //MMK_SetAtvCtrl(MMIENG_VOICEPARAM_WIN_ID, MMIENG_EDITBOX1_CTRL_ID);
            s_mmieng_up_down_link = 0;
            MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, 0);
        }
        else if (0 == down_link.wstr_len)
        {
            //MMK_SetAtvCtrl(MMIENG_VOICEPARAM_WIN_ID, MMIENG_EDITBOX2_CTRL_ID);
            s_mmieng_up_down_link = 1;
            MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, 0);
        }
        else
        {
#ifndef _WIN32
                switch (g_mmieng_voice_param)
                {
                    case MMI_HANDHOLD_DIGITAL_GAIN:
                    case MMI_HANDFREE_DIGITAL_GAIN:
                    case MMI_EARPHONE_DIGITAL_GAIN:
                        audio_data.UL_digital_gain = atoi((char*)up_link.wstr_ptr);
                        audio_data.DL_digital_gain = atoi((char*)down_link.wstr_ptr);
                        break;


                    case MMI_HANDHOLD_PGA_GAIN:
                    case MMI_HANDFREE_PGA_GAIN:
                    case MMI_EARPHONE_PGA_GAIN:
                        audio_data.UL_PGA_gain = atoi((char*)up_link.wstr_ptr);
                        audio_data.DL_PGA_gain = atoi((char*)down_link.wstr_ptr);
                        break;

                    case MMI_HANDHOLD_DIGITAL_SCALE:
                    case MMI_HANDFREE_DIGITAL_SCALE:
                    case MMI_EARPHONE_DIGITAL_SCALE:
                        audio_data.UL_digital_scale = atoi((char*)up_link.wstr_ptr);
                        audio_data.DL_digital_scale = atoi((char*)down_link.wstr_ptr);
                        break;

                        
                    default:
                        break;
                }
                if(AUDIO_NO_ERROR != AUD_SetEngineeringParameter(s_mmieng_audio_dev_mode, audio_data)) 
                {            //error
                     ShowSetAudioParamResult(FALSE);
                }
                else
                {
                    MMK_CloseWin(MMIENG_VOICEPARAM_WIN_ID);      
                }
#endif
          }
        break;
        
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
        if (0 == s_mmieng_up_down_link)
        {
            MMK_SetAtvCtrl(MMIENG_VOICEPARAM_WIN_ID, MMIENG_EDITBOX2_CTRL_ID);
            s_mmieng_up_down_link = 1;
        }
        else
        {
            MMK_SetAtvCtrl(MMIENG_VOICEPARAM_WIN_ID, MMIENG_EDITBOX1_CTRL_ID);
            s_mmieng_up_down_link = 0;
        }
        MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, 0);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          GetProductionSNStr
 DESCRIPTION:   
 PARAM IN:      pd_buf - 
                buf_len - 
 PARAM OUT:     production info string len
 AUTHOR:        allen
 DATE:          2004.10.04
********************************************************************************/
LOCAL int32 GetProductionSNStr(uint8* pd_buf, uint8 buf_len, 
                        const PRODUCTION_TEST_INFO_T *s_mmieng_product )
{
    int len1 = 0;
    int offset = 0;
  
    if(PNULL == pd_buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetProductionSNStr pd_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_7674_112_2_18_2_17_56_153,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_win.c GetProductionSNStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_7677_112_2_18_2_17_56_154,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
    ////////////////
    //  S/N
    ////////////////
    MMI_MEMCPY(pd_buf + offset, buf_len - offset, "<S/N>", 5, 5);
    offset += 5;
    pd_buf[offset] = CR_CHAR;
    offset += 1;
    len1 = SCI_STRLEN(s_mmieng_product->serial_num);
     MMI_MEMCPY(pd_buf + offset, buf_len - offset, s_mmieng_product->serial_num, 32, len1);
    offset += len1;
    pd_buf[offset] = CR_CHAR;
    offset += 1;

    return (offset);
}



/********************************************************************************
 NAME:          GetProductionInfoStr
 DESCRIPTION:   
 PARAM IN:      pd_buf - 
                buf_len - 
 PARAM OUT:     production info string len
 AUTHOR:        allen
 DATE:          2004.10.04
********************************************************************************/
/*
LOCAL int GetProductionInfoStr(uint8* pd_buf, uint8 buf_len)
{
    uint16 i = 0;
    int len1 = 0;
    int offset = 0;
    
    SCI_ASSERT(pd_buf != PNULL);
    SCI_PASSERT(buf_len >= 2, ("GetProductionInfoStr: buf_len is %d",buf_len));
        
    ////////////////
    //  S/N
    ////////////////
    MMI_MEMCPY(pd_buf + offset, buf_len - offset, "<S/N>", 5, 5);
    offset += 5;
    pd_buf[offset] = CR_CHAR;
    offset += 1;
    len1 = SCI_STRLEN(s_mmieng_product.serial_num);
    MMI_MEMCPY(pd_buf + offset, buf_len - offset, s_mmieng_product.serial_num, 32, len1);
    offset += len1;
    pd_buf[offset] = CR_CHAR;
    offset += 1;
    ////////////////////////////
    //  station test result
    ////////////////////////////
    MMI_MEMCPY(pd_buf + offset, buf_len - offset, "<Station Result>", 16, 16);
    offset += 16;
    for (i = 0; i < s_mmieng_product.total_item_num; i++)
    {
        pd_buf[offset] = CR_CHAR;
        offset += 1;
        /////
        len1 = SCI_STRLEN(s_mmieng_product.item_info[i].item_name);
        MMI_MEMCPY(pd_buf + offset, buf_len - offset, s_mmieng_product.item_info[i].item_name, 20, len1);
        offset += len1;
        MMI_MEMCPY(pd_buf + offset, buf_len - offset, ": ", 2, 2);
        offset += 2;
        if (PRODUCTION_TEST_SUCCESS == s_mmieng_product.item_info[i].error_code)
        {
            MMI_MEMCPY(pd_buf + offset, buf_len - offset, "Passed", 6, 6);
            offset += 6;
        }
        else if (PRODUCTION_TEST_NOT_DONE == s_mmieng_product.item_info[i].error_code)
        {
            MMI_MEMCPY(pd_buf + offset, buf_len - offset, "No test", 7, 7);
            offset += 7;
        }
        else
        {
            MMI_MEMCPY(pd_buf + offset, buf_len - offset, "Fail", 4, 4);
            offset += 4;
        }
    }
    return (offset);
}
*/


/********************************************************************************
 NAME:          EngProductSNWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngProductSNWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#ifndef _WIN32
     uint8 text_buf[151] = {0};
	 wchar wstr[151] = {0};
	 uint8* str = PNULL;
#endif   
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
#ifndef _WIN32
             PRODUCTION_TEST_INFO_T s_mmieng_product = {0};
             
        if (SCI_SUCCESS == SCI_GetProductionTestInfo(&s_mmieng_product))
        {
#ifndef _WIN32
            text1.wstr_len = GetProductionSNStr(text_buf, 150,&s_mmieng_product);
#else
            text1.wstr_len = 0;
#endif
            str = text_buf;
            text1.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 150, (uint8*)str, text1.wstr_len, text1.wstr_len );

            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text1.wstr_ptr, text1.wstr_len, FALSE);
        }
        else
#endif
        {
            MMK_CloseWin(MMIENG_PRODUCT_WIN_ID);
            text1.wstr_ptr = s_fail_to_wstr;   /*lint !e605*/     //"Fail to";
            text1.wstr_len = 7;
            text2.wstr_ptr = s_get_production_wstr; /*lint !e605*/ //"get production";
            text2.wstr_len = 14;
            MMIEng_SetResultWinText(text1, text2, text3);
            MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
        }
      }  
        MMK_SetAtvCtrl(MMIENG_PRODUCT_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        break;        

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_PRODUCT_WIN_ID);
        break;

    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(MMIENG_PRODUCT_WIN_ID);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngProductWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL MMI_RESULT_E EngProductWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    const int32 buffer_size = 1024 ;
    uint8 text_buf[1025] = {0};
    wchar wstr[1025] = {0};     
    int32 str_length = 1024 ;
    MMI_RESULT_E result = MMI_RESULT_TRUE; 
    GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        LCD_FillRect(&lcd_dev_info, MMITHEME_GetClientRect(), mmi_eng_display.bkgd_color);
        str_length = GetProductionInfoStr((char*)text_buf, buffer_size);      
        MMI_STRNTOWSTR(wstr, buffer_size, text_buf, buffer_size, str_length);        
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID , wstr , (uint16)str_length, FALSE);       
        MMK_SetAtvCtrl(MMIENG_PRODUCT_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_PRODUCT_WIN_ID);
        break;

    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(MMIENG_PRODUCT_WIN_ID);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          ShowSetPowerKeyResult
 DESCRIPTION:   
 PARAM IN:      result -- 
 PARAM OUT:     None
 AUTHOR:        allen
 DATE:          2004.09.27
********************************************************************************/
LOCAL void ShowSetPowerKeyResult(BOOLEAN result)
{
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 
    
    text1.wstr_ptr = s_long_press_wstr;     /*lint !e605*///"long press";
    text1.wstr_len = 10;
    text2.wstr_ptr = s_red_key_will_wstr;   /*lint !e605*///"red key will";
    text2.wstr_len = 12;
    if (result)
    {
        text3.wstr_ptr = s_power_off_wstr;  /*lint !e605*///"power off";
        text3.wstr_len = 9;
    }
    else
    {
        text3.wstr_ptr = s_do_nothing_wstr; /*lint !e605*///"do nothing";
        text3.wstr_len = 10;
    }
    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:          AppendPowerKeyItem
 DESCRIPTION:   
 PARAM IN:      list_id - 
 PARAM OUT:     None
 AUTHOR:        allen
 DATE:          2004.10.11
********************************************************************************/
LOCAL void AppendPowerKeyItem(MMI_CTRL_ID_T list_id)
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    GUILIST_SetMaxItem( list_id, 2, FALSE );//max item 2
    GUILIST_SetOwnSofterKey(list_id,TRUE);

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_ENG_YES;
    
    //set softkey
    item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_ENG_OK;
    //item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)TXT_ENG_RETURN;
    
    GUILIST_AppendItem( list_id, &item_t );
    
    item_data.item_content[0].item_data.text_id = TXT_ENG_NO;
    
    GUILIST_AppendItem( list_id, &item_t );
    
    if (MMIAPIENG_GetPowerKeyFlag())
    {
        GUILIST_SetSelectedItem( list_id, 0, TRUE );
        GUILIST_SetCurItemIndex( list_id, 0);
    }
    else
    {
        GUILIST_SetSelectedItem( list_id, 1, TRUE );
        GUILIST_SetCurItemIndex( list_id, 1 );
    }
}

/********************************************************************************
 NAME:          AppendListItem
 DESCRIPTION:   
 PARAM IN:      list_id - 
 PARAM OUT:     None
 AUTHOR:        songbin.zeng
 DATE:          2006.08.08
********************************************************************************/
LOCAL void AppendListItem(
                            MMI_CTRL_ID_T list_id,
                            MMI_TEXT_ID_T *text_id_tab,
                            int32 tab_len
                            )
{
    uint16                  i         = 0;
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    
    item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_ENG_OK;
    //item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)TXT_ENG_RETURN;    
    
    for ( i = 0; i < tab_len; i ++ )
    {
        item_data.item_content[0].item_data.text_id = text_id_tab[i];
        
        GUILIST_AppendItem( list_id, &item_t );
    }
}


/********************************************************************************
 NAME:          EngPowerKeyWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.10.11
********************************************************************************/
LOCAL MMI_RESULT_E EngPowerKeyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        AppendPowerKeyItem(MMIENG_LISTBOX_CTRL_ID);     
        MMK_SetAtvCtrl(MMIENG_POWERKEY_WIN_ID, MMIENG_LISTBOX_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        break;      

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_POWERKEY_WIN_ID);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            uint16      index = 0;
            BOOLEAN     keyflag = TRUE;

            index = GUILIST_GetCurItemIndex(MMIENG_LISTBOX_CTRL_ID);

            keyflag = (0 == index);

            MMIAPIENG_SetPowerKeyFlag(keyflag);

            ShowSetPowerKeyResult(keyflag);

            MMK_CloseWin(MMIENG_POWERKEY_WIN_ID);
        }
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#ifdef DSP_USB_LOG
/********************************************************************************
 NAME:          EngDspCardLogEnableWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        xuefang.jiang  
 DATE:          20110729
********************************************************************************/
LOCAL MMI_RESULT_E EngDspCardLogEnableWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    BOOLEAN is_on = FALSE;
    //MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;  
    MMI_TEXT_ID_T text_id = TXT_NULL;
    BOOLEAN is_udcard = FALSE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2  

        is_on = (BOOLEAN)REFPARAM_GetDspCardLogFlag();
        //SCI_TRACE_LOW:"is_on %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8076_112_2_18_2_17_57_155,(uint8*)"d",is_on);
        /*MMINV_READ(NV_REF_PARAMETER,&is_on,return_value);        
        if(MN_RETURN_SUCCESS!=return_value)
        {
            is_on = FALSE;      
            MMINV_WRITE(NV_REF_PARAMETER,&is_on);
        }   */          
 
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);         

        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, is_on, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,is_on);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);     
        break;
        
    case MSG_FULL_PAINT:
        break;  
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
        is_on = (BOOLEAN)GUILIST_GetCurItemIndex(ctrl_id);  
        if(is_on==FALSE)
        {
#ifndef WIN32
            SIO_DSPCardLogClose(); 
#endif
            //SCI_TRACE_LOW:"is_on_close %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8115_112_2_18_2_17_57_156,(uint8*)"d",is_on);
        }
        else
        {

            // 判断保存文件所需空间是否不足
            if(!MMIAPIFMM_GetDeviceTypeStatus(file_dev))
            {
                //device selected doesn't exist!
                text_id = MMIAPIFMM_GetAlertTextIDByDev((file_dev));
            
                //SCI_TRACE_LOW:"IS sdcard %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8126_112_2_18_2_17_57_157,(uint8*)"d",is_udcard);
            
                MMIPUB_OpenAlertWinByTextId(PNULL,
                                            text_id,
                                            TXT_NULL,
                                            IMAGE_PUBWIN_WARNING,
    										PNULL,
                                            PNULL,
                                            MMIPUB_SOFTKEY_ONE,
                                            PNULL);
            }
            
            else
            {
#ifndef WIN32
 
                SIO_DSPCardLogOpen();
#endif
                //SCI_TRACE_LOW:"is_on_open %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8144_112_2_18_2_17_57_158,(uint8*)"d",is_on);
            }   
            
        }
        //MMINV_WRITE(NV_REF_PARAMETER,&is_on); 
        MMK_CloseWin(win_id);
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);  
		
        result = MMI_RESULT_TRUE;  
        break;      
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#endif
/********************************************************************************
 NAME:          EngSenSorModeWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.08.08
********************************************************************************/
LOCAL MMI_RESULT_E EngSensorModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16                  index = 0;
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T           text_id_tab[] = {TXT_ENG_DEFAULT, TXT_ENG_RGB, TXT_ENG_YUV};
    int16                   item_total_num = sizeof(text_id_tab) / sizeof(MMI_TEXT_ID_T);
    MMI_CTRL_ID_T           list_id = MMIENG_LISTBOX_CTRL_ID;
    MMIENG_SENSOR_MODE_E    mode = MMIENG_SENSOR_MODE_DEFAULT;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(list_id, item_total_num, FALSE );//max item 3
        GUILIST_SetOwnSofterKey(list_id,TRUE);
        AppendListItem(
            list_id, 
            text_id_tab, 
            item_total_num
            );
        
        mode = MMIAPIENG_GetSensorMode();
        
        //SCI_TRACE_LOW:"MMIENG_WIN.C EngSensorModeWinHandleMsg mode is %d,item_total_num is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8194_112_2_18_2_17_57_159,(uint8*)"dd",mode,item_total_num);
        if((int16)mode >=  item_total_num)
        {
            return MMI_RESULT_FALSE;
        }
        GUILIST_SetSelectedItem(list_id, (uint16)mode, TRUE);
        GUILIST_SetCurItemIndex(list_id, (int16)mode);
        MMK_SetAtvCtrl(win_id, list_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);
        //SCI_TRACE_LOW:"MMIENG_WIN.C EngSensorModeWinHandleMsg index is %d,item_total_num is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8217_112_2_18_2_17_57_160,(uint8*)"dd",index,item_total_num);
        if((int16)index >=  item_total_num)
        {
            return MMI_RESULT_FALSE;
        }
        MMIAPIENG_SetSensorMode((MMIENG_SENSOR_MODE_E)index);
        //success!
       // MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/********************************************************************************
//MS00216980 cheney
 NAME:			EngCMCCTestWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		cheney.chen
 DATE:			2010.12.09
********************************************************************************/
LOCAL MMI_RESULT_E EngCMCCTestWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    //uint16 					index = 0;
    MMI_RESULT_E 			result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T 			text_id_tab[] = {TXT_ENG_CMCC_BUSINESS_SERVER, TXT_ENG_CMCC_TEST_SERVER};
    int16					item_total_num = sizeof(text_id_tab) / sizeof(MMI_TEXT_ID_T);
    MMI_CTRL_ID_T   		list_id = MMIENG_LISTBOX_CTRL_ID;
    BOOLEAN 	test_flag =  FALSE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(list_id, item_total_num, FALSE );//max item 2
        GUILIST_SetOwnSofterKey(list_id,TRUE);
        AppendListItem(
            list_id, 
            text_id_tab, 
            item_total_num
            );
        
        test_flag = MMIAPIENG_GetCMCCTestFlag();
        
        GUILIST_SetSelectedItem(list_id, (uint16)test_flag, TRUE);
        GUILIST_SetCurItemIndex(list_id, (int16)test_flag);
        MMK_SetAtvCtrl(win_id, list_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
	
        test_flag = GUILIST_GetCurItemIndex(list_id);
	 //SCI_TRACE_LOW:"[mmieng] list_id = %d"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8286_112_2_18_2_17_57_161,(uint8*)"d", test_flag);
        MMIAPIENG_SetCMCCTestFlag(&test_flag);
        //success!
        //MMIPUB_OpenAlertSuccessWin(TXT_ENG_SUCCESS_NEED_RESET);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/********************************************************************************
 NAME:          EngDVCovFrameFre2ListItemPos
 DESCRIPTION:   
 PARAM IN:
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.12.05
********************************************************************************/
LOCAL int16 EngDVCovFrameFre2ListItemPos(MMIENG_DV_FRAME_FRE_E dv_frame_frequency)
{
    int16 item_pos = 0;
    
    switch(dv_frame_frequency)
    {
    case MMIENG_DV_FRAME_FRE_7HZ:
        item_pos = 0;
        break;
        
    case MMIENG_DV_FRAME_FRE_10HZ:
        item_pos = 1;
        break;
        
    case MMIENG_DV_FRAME_FRE_15HZ:
        item_pos = 2;
        break;

    case MMIENG_DV_FRAME_FRE_20HZ:
        item_pos = 3;
        break;

    case MMIENG_DV_FRAME_FRE_25HZ:
        item_pos = 4;
        break;

    case MMIENG_DV_FRAME_FRE_30HZ:
        item_pos = 5;
        break;

    default:
        //SCI_TRACE_LOW:"MMIENG_WIN.C EngDVCovFrameFre2ListItemPos dv_frame_frequency is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8339_112_2_18_2_17_58_162,(uint8*)"d", dv_frame_frequency);
        break;
    }

    return item_pos;
}

/********************************************************************************
 NAME:          EngDVCovListItemPos2FrameFre
 DESCRIPTION:   
 PARAM IN:
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.12.05
********************************************************************************/
LOCAL MMIENG_DV_FRAME_FRE_E EngDVCovListItemPos2FrameFre(int16 item_pos)
{
    MMIENG_DV_FRAME_FRE_E dv_frame_frequency = MMIENG_DV_FRAME_FRE_7HZ;
    
    switch(item_pos)
    {
    case 0:
        dv_frame_frequency = MMIENG_DV_FRAME_FRE_7HZ;
        break;
        
    case 1:
        dv_frame_frequency = MMIENG_DV_FRAME_FRE_10HZ;
        break;
        
    case 2:
        dv_frame_frequency = MMIENG_DV_FRAME_FRE_15HZ;
        break;

    case 3:
        dv_frame_frequency = MMIENG_DV_FRAME_FRE_20HZ;
        break;

    case 4:
        dv_frame_frequency = MMIENG_DV_FRAME_FRE_25HZ;
        break;

    case 5:
        dv_frame_frequency = MMIENG_DV_FRAME_FRE_30HZ;
        break;

    default:
        //SCI_TRACE_LOW:"MMIENG_WIN.C EngDVCovListItemPos2FrameFre item_pos is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8384_112_2_18_2_17_58_163,(uint8*)"d",item_pos);
        break;
    }

    return dv_frame_frequency;
}


/********************************************************************************
 NAME:          EngDVFrameFreWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.12.05
********************************************************************************/
LOCAL MMI_RESULT_E EngDVFrameFreWinHandleMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T           text_id_tab[] = {
                                            TXT_ENG_DV_FRAME_FRE_10, 
                                            TXT_ENG_DV_FRAME_FRE_15, 
                                            TXT_ENG_DV_FRAME_FRE_20,
                                            TXT_ENG_DV_FRAME_FRE_25,
                                            TXT_ENG_DV_FRAME_FRE_30
                                            };
    int16                   item_pos = 0;
    int16                   item_total_num = 0;
    MMI_CTRL_ID_T           list_id = MMIENG_LISTBOX_CTRL_ID;
    MMIENG_DV_FRAME_FRE_E   dv_frame_frequency = MMIENG_DV_FRAME_FRE_MIN;

    item_total_num = ARR_SIZE(text_id_tab);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(list_id, item_total_num, FALSE );//max item 5
        GUILIST_SetOwnSofterKey(list_id,TRUE);
        AppendListItem(
                    list_id, 
                    text_id_tab, 
                    item_total_num
                    );
        
        dv_frame_frequency = MMIAPIENG_GetDVFrameFre();
                item_pos =EngDVCovFrameFre2ListItemPos(dv_frame_frequency);

        GUILIST_SetSelectedItem(list_id, (uint16)item_pos, TRUE);
        GUILIST_SetCurItemIndex(list_id, (int16)item_pos);
        MMK_SetAtvCtrl(win_id, list_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_DV_FRAME_FRE_WIN_ID);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        item_pos = GUILIST_GetCurItemIndex(list_id);
                dv_frame_frequency = EngDVCovListItemPos2FrameFre(item_pos);
        MMIAPIENG_SetDVFrameFre(dv_frame_frequency);
        //success!
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/********************************************************************************
 NAME:          AppendBandSelectItem
 DESCRIPTION:   
 PARAM IN:      list_id - 
 PARAM OUT:     None
 AUTHOR:        allen
 DATE:          2004.10.08
********************************************************************************/
LOCAL void AppendBandSelectItem(MMI_CTRL_ID_T ctrl_id)
{
    uint32                  total_item = 0;
    uint32                  i = 0;
    
    total_item = ARR_SIZE(s_mmieng_sel_band_txt_tab);
    GUILIST_SetMaxItem(ctrl_id, total_item, FALSE );//max item 5
    GUILIST_SetOwnSofterKey(ctrl_id,TRUE);

    for(i = 0; i < total_item; i ++)
    {
        MMIAPISET_AppendListItemByTextIdExt(s_mmieng_sel_band_txt_tab[i],TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    }
    
    // get current band
    switch (MNPHONE_GetBandEx(MMIAPISET_GetActiveSim()))//%#discuss
    {
    case MN_PHONE_BAND_PCS:
        i = 0;
        break;
        
    case MN_PHONE_BAND_GSM:
        i = 1;
        break;
        
    case MN_PHONE_BAND_DCS:
        i = 2;
        break;
                    
    case MN_PHONE_BAND_GSM_DCS:
        i = 3;
        break;
    
    case MN_PHONE_BAND_GSM850_GSM_DCS_PCS:
        i = 4;
        break;
        
    default:
        i = 4;
        break;
    }
    
    if (i < total_item)
    {
        //set select item
        GUILIST_SetSelectedItem(ctrl_id, i,TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id, i);
       
    }
}

/********************************************************************************
 NAME:          EngBandSelectWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        allen
 DATE:          2004.10.08
********************************************************************************/
LOCAL MMI_RESULT_E EngBandSelectWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MN_PHONE_BAND_TYPE_E    sel_band = {0};
    MMI_STRING_T    wait_text = {0};
    ERR_MNPHONE_CODE_E mn_error = ERR_MNPHONE_NOT_RIGHT_PARAM ;

    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        AppendBandSelectItem(ctrl_id);
        MMK_SetAtvCtrl(MMIENG_BANDSELECT_WIN_ID, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
       
        break;
 
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(MMIENG_LISTBOX_CTRL_ID);
        switch (index)
        {
        case 0:     // PCS
            sel_band = MN_PHONE_BAND_PCS;
            break;
            
        case 1:     // GSM
            sel_band = MN_PHONE_BAND_GSM;
            break;
            
        case 2:     // DCS
            sel_band = MN_PHONE_BAND_DCS;
            break;
            
        case 3:     // DUAL BAND
            sel_band = MN_PHONE_BAND_GSM_DCS;
            break;

        case 4:     // DUADRUPLE BAND
            sel_band = MN_PHONE_BAND_GSM850_GSM_DCS_PCS;
            break;
            
        default:
            //SCI_TRACE_LOW:"MMIENG_WIN.C EngBandSelectWinHandleMsg index is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8590_112_2_18_2_17_58_164,(uint8*)"d",index);
            break;
        }

        if (!MMIAPISET_GetFlyMode())
        {
            MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
            
            for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
            {
                if(MMIAPIPHONE_IsSimAvailable(dual_sys))
                {
                   mn_error = MNPHONE_SelectBandEx(dual_sys, sel_band);                 
                   //SCI_TRACE_LOW:"MMIENG_WIN.C,EngBandSelectWinHandleMsg,MNPHONE_SelectBandEx, sim%d, result = %d"
                   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8604_112_2_18_2_17_58_165,(uint8*)"dd",dual_sys + 1,mn_error);
                }
            }
        }
        if(ERR_MNPHONE_NO_ERR == mn_error)
        {
#ifndef WIN32
            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MAIN_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
#endif    
        }
        else
        {
            MMI_STRING_T text_str = {0};
            uint8 str_buf[100] = {0};
            wchar w_str_buf[100] = {0};
            int32 length = 0;      
            sprintf((char*)str_buf,"SIM set fail");
            length = SCI_STRLEN((char*)str_buf);
            MMI_STRNTOWSTR(w_str_buf,100, str_buf,length, length);               
            text_str.wstr_ptr = w_str_buf;      
            text_str.wstr_len = length;
            MMIPUB_OpenAlertWinByTextPtr(PNULL,&text_str,PNULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,
                MMIPUB_SOFTKEY_ONE,PNULL);
        }

        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          MMIAPIENG_BandSelectCnf
 DESCRIPTION:   
 PARAM IN:      None 
 PARAM OUT:     None
 AUTHOR:        songbin.zeng
 DATE:          2006.11.16
********************************************************************************/
PUBLIC void MMIAPIENG_BandSelectCnf(DPARAM  param)
{
    APP_MN_SET_BAND_CNF_T    band_select_cnf = *((APP_MN_SET_BAND_CNF_T *)param);
    //MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MMI_STRING_T text_str = {0};
    uint8        str_buf[100] = {0};
    wchar     w_str_buf[100] = {0};

    if (band_select_cnf.is_ok)
    {
        //success!
        sprintf((char*)str_buf,"SIM%d set done",(band_select_cnf.dual_sys+1));
        MMI_STRNTOWSTR(w_str_buf,100, str_buf,strlen((char*)str_buf),strlen((char*)str_buf));       
        //text_str.is_ucs2 = 0;
        text_str.wstr_ptr = w_str_buf;
        text_str.wstr_len = strlen((char*)str_buf);
        MMIPUB_OpenAlertWinByTextPtr(PNULL,&text_str,PNULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else
    {   
        sprintf((char*)str_buf,"SIM%d set fail",(band_select_cnf.dual_sys+1));
        MMI_STRNTOWSTR(w_str_buf,100, str_buf,strlen((char*)str_buf),strlen((char*)str_buf));               
        //text_str.is_ucs2 = 0;
        text_str.wstr_ptr = w_str_buf;      
        text_str.wstr_len = strlen((char*)str_buf);
        MMIPUB_OpenAlertWinByTextPtr(PNULL,&text_str,PNULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    // get current band
    if (MMK_IsOpenWin(MMIENG_BANDSELECT_WIN_ID))
    {
        MMK_CloseWin(MAIN_WAIT_WIN_ID);
        MMK_CloseWin(MMIENG_BANDSELECT_WIN_ID);
    }
   
}

/*****************************************************************************/
//  Description : is eng band select cmd
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsBandSelectWinOpen(void)
{
    return MMK_IsOpenWin(MMIENG_BANDSELECT_WIN_ID);
}

/*********************************************************************************
 NAME:          MMIAPIENG_CreateProductWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateProductWin(void)
{
    return MMK_CreateWin((uint32*)MMIENG_PRODUCT_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:          MMIAPIENG_CreateProductSNWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.10.11
********************************************************************************/
BOOLEAN MMIAPIENG_CreateProductSNWin(void)
{
    return MMK_CreateWin((uint32*)MMIENG_PRODUCT_SN_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:          MMIAPIENG_CreatePhoneInfoWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreatePhoneInfoWin(void)
{
    return MMK_CreateWin((uint32*)MMIENG_SHOWPHONE_WIN_TAB, PNULL);
}

LOCAL MMI_RESULT_E OpenPSCellWin(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    if (isReturnOK)
    {
        return MMK_CreateWin((uint32 *) MMIENG_PSCELL_WIN_TAB, (ADD_DATA)dual_sys); 
    }
    else
    {
        return MMI_RESULT_FALSE;
    }
}

LOCAL MMI_RESULT_E Open_NETINFO_TO_NWCAP_Win(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    if (isReturnOK)
    {
        return MMK_CreateWin((uint32 *) MMIENG_NET_INFO_SHOW_WIN_TAB, (ADD_DATA)dual_sys); 
    }
    else
    {
        return MMI_RESULT_FALSE;
    }
}

LOCAL MMI_RESULT_E Open_NETINFO_STATISTICS_Win(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    if (isReturnOK)
    {
        return MMK_CreateWin((uint32 *) MMIENG_NETINFO_STATISTICS_WIN_TAB, (ADD_DATA)dual_sys); 
    }
    else
    {
        return MMI_RESULT_FALSE;
    }
}

LOCAL MMI_RESULT_E Open_OpenIMS_NV_PARAM_Win(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    if (isReturnOK)
    {
        return MMK_CreateWin((uint32 *) MMIENG_IMS_NV_PARAM_ID_WIN_TAB, (ADD_DATA)dual_sys); 
    }
    else
    {
        return MMI_RESULT_FALSE;
    }
}

/********************************************************************************
 NAME:          MMIAPIENG_CreateShowNetWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowNetWin(void)
{    
#ifndef MMI_DUALMODE_ENABLE
    uint16  sim_ok[2] = {0};
    uint32  sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(sim_ok, 1);

    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return 0;
    }
    else if (1 == sim_num)
    {
      //  MMK_CreateWin((uint32 *) MMIENG_PSCELL_WIN_TAB, (ADD_DATA)sim_ok); 
	 MMK_CreateWin((uint32 *) MMIENG_NET_INFO_SHOW_WIN_TAB, (ADD_DATA)sim_ok[0]); 

		
        return 1;        
    }
    else
    {
        MMI_WIN_ID_T    selectSimWinID = MMIENG_SIM_SELECT_WIN_ID;
        MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};
        //return MMK_CreateWin((uint32 *) MMIENG_SHOW_RF_WIN_TAB, PNULL);    
        //return MMK_CreateWin((uint32 *) MMIENG_PSCELL_WIN_TAB, 0);    
        // return MMK_CreateWin((uint32 *) MMIENG_SIM_SELECT_WIN_TAB, PNULL);    

        caller_data.append_type = (uint32)MMISET_APPEND_SYS_OK;
        //caller_data.callback_func = OpenPSCellWin ;
        caller_data.callback_func = Open_NETINFO_TO_NWCAP_Win ;
        caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;
        caller_data.publist_id = MMIENG_LISTBOX_CTRL_ID;
        caller_data.win_id_ptr = &selectSimWinID;
        caller_data.user_data  = PNULL;
        caller_data.extra_data = PNULL;

        return MMIAPIPHONE_OpenSelectSimWin(&caller_data);
    }
#else
    return MMK_CreateWin((uint32 *) MMIENG_NET_INFO_SHOW_WIN_TAB, PNULL);    
#endif
}

/********************************************************************************
 NAME:          MMIAPIENG_Netinfo_Statistics_ShowWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_Netinfo_Statistics_ShowWin(void)
{    
#ifndef MMI_DUALMODE_ENABLE
       uint16  sim_ok[2] = {0};
    uint32  sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(sim_ok, 1);

    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return 0;
    }
    else if (1 == sim_num)
    {
  
	 MMK_CreateWin((uint32 *) MMIENG_NETINFO_STATISTICS_WIN_TAB, (ADD_DATA)sim_ok[0]); 
       return 1;        
    }
    else
    {
        MMI_WIN_ID_T    selectSimWinID = MMIENG_SIM_SELECT_WIN_ID;
        MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};
        caller_data.append_type = (uint32)MMISET_APPEND_SYS_OK;
        caller_data.callback_func = Open_NETINFO_STATISTICS_Win ;
        caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;
        caller_data.publist_id = MMIENG_LISTBOX_CTRL_ID;
        caller_data.win_id_ptr = &selectSimWinID;
        caller_data.user_data  = PNULL;
        caller_data.extra_data = PNULL;

        return MMIAPIPHONE_OpenSelectSimWin(&caller_data);
    }
#else
    return MMK_CreateWin((uint32 *) MMIENG_NETINFO_STATISTICS_WIN_TAB, PNULL);    
#endif
}


/********************************************************************************
 NAME:          MMIAPIENG_OpenIMS_NV_PARAM_ShowWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_OpenIMS_NV_PARAM_ShowWin(void)
{    
#ifndef MMI_DUALMODE_ENABLE
       uint16  sim_ok[2] = {0};
    uint32  sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(sim_ok, 1);
SCI_TRACE_LOW("[ENG] func[%s] line[%d] sim %d",__FUNCTION__,__LINE__,sim_ok[0]);
    if (0 == sim_num)
    {
   
        MMIAPIPHONE_AlertSimNorOKStatus();
        return 0;
    }
    else if (1 == sim_num)
    {
 
	 MMK_CreateWin((uint32 *) MMIENG_IMS_NV_PARAM_ID_WIN_TAB, (ADD_DATA)sim_ok[0]); 
       return 1;        
    }
    else
    {
    
        MMI_WIN_ID_T    selectSimWinID = MMIENG_SIM_SELECT_WIN_ID;
        MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};
        caller_data.append_type = (uint32)MMISET_APPEND_SYS_OK;
        caller_data.callback_func = Open_OpenIMS_NV_PARAM_Win ;
        caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;
        caller_data.publist_id = MMIENG_LISTBOX_CTRL_ID;
        caller_data.win_id_ptr = &selectSimWinID;
        caller_data.user_data  = PNULL;
        caller_data.extra_data = PNULL;
	
        return MMIAPIPHONE_OpenSelectSimWin(&caller_data);
    }
#else

    return MMK_CreateWin((uint32 *) MMIENG_IMS_NV_PARAM_ID_WIN_TAB, PNULL);    
#endif
}


/*****************************************************************************/
//  Description : HandlePreferNetWaitingWinMsg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIENG_HandleUPLMNWaitingWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}



/*****************************************************************************/
//  Description : MMIAPISET_WaitPreferNetworkList
//  Global resource dependence : 
//  Author:michael.shi
//  Note: call l4 MN API : MMIAPISET_SavePreferNetworkList
/*****************************************************************************/
LOCAL BOOLEAN MMIAPIENG_WaitPreferNetworkList( )
{
    MMI_STRING_T prompt_str = {0};
    MMI_WIN_ID_T wait_win_id =  MMIENG_UPLMN_WAIT_WIN_ID;
 
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);    
    
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
                            MMIENG_HandleUPLMNWaitingWinMsg);

    return TRUE;
}

LOCAL MMI_RESULT_E Open_UPLMN_CHANGE_Win(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
	if (isReturnOK)
	{
		MNSIM_GetPLMNWACTEx(dual_sys);
		return MMIAPIENG_WaitPreferNetworkList();
	}
	else
	{
		return MMI_RESULT_FALSE;
	}
}


PUBLIC MMI_RESULT_E MMIAPIENG_HandlePlmnCnf(
                                       DPARAM              param
                                       )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	APP_MN_PREFER_PLMN_CHANGE_CNF_T prefer_plmn_sim_cnf = *((APP_MN_PREFER_PLMN_CHANGE_CNF_T *)param);
	MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

	APP_MN_PREFER_PLMN_CHANGE_CNF_T* prefer_plmn_sim_cnf_P =PNULL;

	if (MMK_IsOpenWin(MMIENG_UPLMN_EDIT_AND_ADD_WIN_ID))
	{
		MMK_CloseWin(MMIENG_UPLMN_EDIT_AND_ADD_WIN_ID);
	}
	
	if (MMK_IsOpenWin(MMIENG_UPLMN_OPTION_WIN_ID))
	{
		MMK_CloseWin(MMIENG_UPLMN_OPTION_WIN_ID);
	}
	
	prefer_plmn_sim_cnf_P = SCI_ALLOC_APP(sizeof(APP_MN_PREFER_PLMN_CHANGE_CNF_T));
	if(PNULL == prefer_plmn_sim_cnf_P)
	{
		SCI_TRACE_LOW("[ENG] func[%s] line[%d] dual_sys %d ",__FUNCTION__,__LINE__);
		result = MMI_RESULT_FALSE;
		return result;
	}

	SCI_Memcpy(prefer_plmn_sim_cnf_P, &prefer_plmn_sim_cnf,sizeof(APP_MN_PREFER_PLMN_CHANGE_CNF_T));
	
	dual_sys = prefer_plmn_sim_cnf.dual_sys;
	
	MMK_CloseWin(MMIENG_UPLMN_WAIT_WIN_ID);

	MMK_CreateWin((uint32 *)MMIENG_UPLMN_CHANGE_WIN_TAB , (ADD_DATA)prefer_plmn_sim_cnf_P);

	return result;

}



/********************************************************************************
 NAME:          MMIAPIENG_OpenIMS_NV_PARAM_ShowWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_UPLMN_CHANGE_ShowWin(void)
{    
#ifndef MMI_DUALMODE_ENABLE
    uint16  sim_ok[2] = {0};
    uint32  sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(sim_ok, 1);
	
    if (0 == sim_num)
    {
   
        MMIAPIPHONE_AlertSimNorOKStatus();
        return 0;
    }
    else if (1 == sim_num)
    { 	
	 MNSIM_GetPLMNWACTEx(sim_ok[0]);
	 MMIAPIENG_WaitPreferNetworkList();
       return 1;        
    }
    else
    {
    
        MMI_WIN_ID_T    selectSimWinID = MMIENG_SIM_SELECT_WIN_ID;
        MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};
        caller_data.append_type = (uint32)MMISET_APPEND_SYS_OK;
        caller_data.callback_func = Open_UPLMN_CHANGE_Win ;
        caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;
        caller_data.publist_id = MMIENG_LISTBOX_CTRL_ID;
        caller_data.win_id_ptr = &selectSimWinID;
        caller_data.user_data  = PNULL;
        caller_data.extra_data = PNULL;
	
        return MMIAPIPHONE_OpenSelectSimWin(&caller_data);
    }
#else
	 MNSIM_GetPLMNWACTEx(sim_ok[0]);
	 MMIAPIENG_WaitPreferNetworkList();
       return 1;     
#endif
}

/********************************************************************************
 NAME:          MMIAPIENG_CreateRFWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.11.22
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateRFWin(void)
{
    return MMK_CreateWin((uint32 *) MMIENG_SHOW_RF_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:          EngShowRFWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:      0   2006.11.22
********************************************************************************/
LOCAL MMI_RESULT_E EngShowRFWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{

    MMI_RESULT_E result = MMI_RESULT_TRUE;
    LOCAL uint8 s_mmieng_rf_timer_id = 0;
    LOCAL MMIENG_RF_CELL_E sel_cell = MMIENG_RF_CELL_ALL;
    
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                GUI_FONT_T font = MMI_DEFAULT_BIG_FONT ;
                GUITEXT_SetFont(MMIENG_TEXTBOX1_CTRL_ID, &font, 0 );
            }
            DrawRFWin(MMIENG_RF_CELL_ALL);
            sel_cell = MMIENG_RF_CELL_ALL;
                    
            // create refresh timer
            if(0 != s_mmieng_rf_timer_id)
            {
                MMK_StopTimer(s_mmieng_rf_timer_id);
            }
            s_mmieng_rf_timer_id = MMK_CreateWinTimer(win_id, MMIENG_RF_TIME, FALSE);
            MMK_SetAtvCtrl(win_id, MMIENG_TEXTBOX1_CTRL_ID); 
            break;
            
        case MSG_FULL_PAINT:                 
            break;
        case MSG_KEYUP_UP:
        case MSG_KEYUP_DOWN:
            if (s_mmieng_rf_timer_id != 0)
            {
                MMK_StopTimer(s_mmieng_rf_timer_id);
                s_mmieng_rf_timer_id = 0;
                s_mmieng_rf_timer_id = MMK_CreateWinTimer(win_id, MMIENG_RF_TIME, FALSE);
            }
            break;
        case MSG_APP_LEFT:
            if (MMIENG_RF_CELL_C0 !=sel_cell)
            {
                sel_cell --;
                DrawRFWin(sel_cell);
            }
            else
            {
                sel_cell = MMIENG_RF_CELL_ALL;
                DrawRFWin(sel_cell);
            }
            break;

         case MSG_APP_RIGHT:
             if (MMIENG_RF_CELL_ALL !=sel_cell)
             {
                 sel_cell ++;
                 DrawRFWin(sel_cell);
             }
             else
             {
                 sel_cell = MMIENG_RF_CELL_C0;
                 DrawRFWin(sel_cell);
             }
             break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_TIMER:
            if (*(uint8*)param == s_mmieng_rf_timer_id) 
            {
                DrawRFWin(sel_cell);            
                s_mmieng_rf_timer_id = MMK_CreateWinTimer(win_id, MMIENG_RF_TIME, FALSE);
            }
            else
            {
                result = MMI_RESULT_FALSE;      // need additional process
            }           
            break;
                
        case MSG_LOSE_FOCUS:
            MMK_PauseTimer(s_mmieng_rf_timer_id);
            break;
            
        case MSG_GET_FOCUS:            
            MMK_ResumeTimer(s_mmieng_rf_timer_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            if (s_mmieng_rf_timer_id != 0)
            {
                MMK_StopTimer(s_mmieng_rf_timer_id);
                s_mmieng_rf_timer_id = 0;
            }
            break;
        
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return (result);
}

/********************************************************************************
 NAME:          GetRFCellInfo
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.11.22
********************************************************************************/
LOCAL BOOLEAN GetRFCellInfo(
                                                                MMIENG_RF_CELL_INFO_T *info, 
                                                                MMIENG_RF_CELL_E cell
                                                                )
{
#ifndef _WIN32
    MN_ENG_CELL_BASIC_INFO_T cell_basic_info = {0};
    MN_ENG_CELL_IDLE_INFO_T  cell_idle_info = {0};
    int cell_index = 0;
#endif

    if(PNULL == info)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetRFCellInfo info == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_8909_112_2_18_2_17_59_166,(uint8*)"");
        return FALSE;
    } 

    SCI_MEMSET(info, 0, sizeof(MMIENG_RF_CELL_INFO_T));

#ifndef _WIN32

    if (MN_RETURN_SUCCESS != MNENG_GetCellBasicInfo(&cell_basic_info))
    {
        
    }
    else
    {}

    if (MN_RETURN_SUCCESS != MNENG_GetCellIdleInfo(&cell_idle_info))
    {
        
    }
    else
    {}
    
    switch (cell)
    {
        case MMIENG_RF_CELL_C0:
        {
            MN_ENG_SCELL_TRAF_INFO_T scell_traf_info = {0};
                    MN_ENG_SCELL_CTRL_INFO_T scell_ctrl_info = {0};

            info->BCH = cell_basic_info.scell_info.arfcn;
            info->BSIC = (cell_basic_info.scell_info.bsic.ncc << 3) + cell_basic_info.scell_info.bsic.bcc;
            info->CELID = cell_basic_info.scell_info.cell_id;
            info->LAC = cell_basic_info.scell_info.lai.lac;

            info->BRXL = cell_idle_info.scell_info.rssi + 110;
            
            info->C1 = cell_idle_info.scell_info.c1;
            info->C2 = cell_idle_info.scell_info.c2;


            if (MN_RETURN_SUCCESS == MNENG_GetSCellTrafInfo(&scell_traf_info))
            {
                info->TN = scell_traf_info.channel_dec.tn;
                info->TXLEV = scell_traf_info.power_level;
                info->TAV = scell_traf_info.ta;
            }
            

            if (MN_RETURN_SUCCESS == MNENG_GetSCellCtrlInfo(&scell_ctrl_info))
            {
                           info->DSC = scell_ctrl_info.ctl_chn_param.dsc;
                info->RLT = scell_ctrl_info.rlt;
                           info->CBA = scell_ctrl_info.rach_param.cba;
            }
        

            info->CBQ = 0;//待定
            break;
        }
        
        case MMIENG_RF_CELL_N0:
        case MMIENG_RF_CELL_N1:
        case MMIENG_RF_CELL_N2:
        case MMIENG_RF_CELL_N3:
        case MMIENG_RF_CELL_N4:
        case MMIENG_RF_CELL_N5:
            switch (cell)
            {
            case MMIENG_RF_CELL_N0:
                cell_index = 0;
                break;
            case MMIENG_RF_CELL_N1:
                cell_index = 1;
                break;

            case MMIENG_RF_CELL_N2:
                cell_index = 2;
                break;

            case MMIENG_RF_CELL_N3:
                cell_index = 3;
                break;

            case MMIENG_RF_CELL_N4:
                cell_index = 4;
                break;

            case MMIENG_RF_CELL_N5:
                cell_index = 5;
                break;

            default:
                //SCI_TRACE_LOW:"MMIENG_WIN.C GetRFCellInfo cell is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_9001_112_2_18_2_17_59_167,(uint8*)"d",cell);
                break;
            }
            
            //SCI_TRACE_LOW:"MMIENG_MAIN.C GetRFCellInfo cell_index is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_9005_112_2_18_2_17_59_168,(uint8*)"d",cell_index);
            //if(cell_index >=  MN_MAX_NCELL_NUM)
            //{
            //    return FALSE;
            //}
            info->BCH = cell_basic_info.ncell_info[cell_index].arfcn;
            info->BSIC = (cell_basic_info.scell_info.bsic.ncc << 3) + cell_basic_info.scell_info.bsic.bcc;
            info->CELID = cell_basic_info.ncell_info[cell_index].cell_id;
            info->LAC = cell_basic_info.ncell_info[cell_index].lai.lac;
            
            info->BRXL = cell_idle_info.ncell_info[cell_index].rssi + 110;
            info->C1 = cell_idle_info.ncell_info[cell_index].c1;
            info->C2 = cell_idle_info.ncell_info[cell_index].c2;
            
            info->CBA = 0;//待定
            info->CBQ = 0;//待定
            info->ALIG = 0;//待定
            info->FFSET = 0;//待定
            break;

        default:        
            //SCI_TRACE_LOW:"MMIENG_WIN.C GetRFCellInfo cell is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_9026_112_2_18_2_17_59_169,(uint8*)"d",cell);
            break;
    }
#endif
    return TRUE;
}

/********************************************************************************
 NAME:          DrawRFWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.11.22
********************************************************************************/
LOCAL void DrawRFWin(
                     MMIENG_RF_CELL_E cell
                     )
{

     MMIENG_RF_CELL_INFO_T rf_info = {0};

    uint32 i = 0;
    uint32 j = 0;

    uint32 row = 0;
    uint32 col = 0;
    char rf_data_str[20] = {0};
    wchar wstr[20] = {0};
    wchar w_cr[] = {CR_CHAR,0};
//     char* str = PNULL;
    wchar buffer[1024] = {0};
    const uint32 buffer_length = 1023;
    uint32 offset = 0;
    uint32 len = 0;
    wchar ch = ':';
    

    switch (cell)
    {       
    case MMIENG_RF_CELL_ALL:
        {
            const wchar * row_str_ptr[] = { s_space_wstr, s_bch_wstr, s_brxl_wstr, s_c1_wstr, s_c2_wstr };
        
            const wchar * col_str_ptr[] = { s_c0_wstr, s_n0_wstr, s_n1_wstr, s_n2_wstr, s_n3_wstr, s_n4_wstr, s_n5_wstr };
    
            int32 rf_data[5] = {0};
            MMIENG_RF_CELL_E cur_cell = MMIENG_RF_CELL_C0;    
            
           
           row = ARR_SIZE(row_str_ptr);     /*lint !e661*/
             //draw title text  
            for(i = 0; i < row; i ++)
            {
                len = MMIAPICOM_Wstrlen(row_str_ptr[i]);
                MMIAPICOM_Wstrncpy(buffer + offset, row_str_ptr[i],len);
                offset += len ;

                len = MMIAPICOM_Wstrlen(s_space_wstr);
                MMIAPICOM_Wstrncpy(buffer + offset, s_space_wstr,len);
                offset += len ;
            }       
            MMIAPICOM_Wstrncpy(buffer + offset, w_cr,1);
            offset += 1 ;     
            
            //draw content text
            col = ARR_SIZE(col_str_ptr);
        
            for(i = 0; i < col; i ++)
            {
             
                len = MMIAPICOM_Wstrlen(col_str_ptr[i]);  
                MMIAPICOM_Wstrncpy(buffer + offset, col_str_ptr[i],len);
                offset += len ;                
                
                len = MMIAPICOM_Wstrlen(s_space_wstr);
                MMIAPICOM_Wstrncpy(buffer + offset, s_space_wstr,len);
                offset += len ;
                       
                switch (i)
                {
                case 0:
                    cur_cell = MMIENG_RF_CELL_C0;
                    break;
                case 1:
                    cur_cell = MMIENG_RF_CELL_N0;
                    break;
                    
                case 2:
                    cur_cell = MMIENG_RF_CELL_N1;
                    break;
                    
                case 3:
                    cur_cell = MMIENG_RF_CELL_N2;
                    break;
                    
                case 4:
                    cur_cell = MMIENG_RF_CELL_N3;
                    break;
                    
                case 5:
                    cur_cell = MMIENG_RF_CELL_N4;
                    break;
                    
                case 6:
                    cur_cell = MMIENG_RF_CELL_N5;
                    break;
                    
                default:
                    //SCI_TRACE_LOW:"MMIENG_WIN.C DrawRFWin index is %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_9134_112_2_18_2_17_59_170,(uint8*)"d",i);
                    break;
                }
                GetRFCellInfo(&rf_info, cur_cell);
                rf_data[0] = rf_info.BCH;
                rf_data[1] = rf_info.BRXL;
                rf_data[2] = rf_info.C1;
                rf_data[3] = rf_info.C2;
                
                for (j = 1; j < row; j ++)  /*lint !e661*/
                {   
                    SCI_MEMSET(rf_data_str, 0 ,  sizeof(rf_data_str));
                    sprintf(rf_data_str, "%d ", rf_data[j]);        /*lint !e661*/
                    len = SCI_STRLEN(rf_data_str);
                    if( 20 > len)
                    {
                          MMI_STRNTOWSTR( wstr, 20, (uint8*)rf_data_str, len, len );
                    }          
                    if( offset + len < buffer_length)
                    {
                        MMIAPICOM_Wstrncpy(buffer + offset, wstr,len);       
                        offset += len ;   
                    }
                }
                  MMIAPICOM_Wstrncpy(buffer + offset, w_cr,1);
                     offset += 1 ;    
            }
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, buffer,offset, TRUE);
            break;
            }
            
        case MMIENG_RF_CELL_C0:
            {
                const wchar * wstr_ptr[] = {
                    s_bch_wstr, s_brxl_wstr, s_bsic_wstr, s_dsc_wstr, s_txlev_wstr, s_tn_wstr, s_rlt_wstr, 
                        s_tav_wstr, s_cba_wstr, s_cbq_wstr, s_c1_wstr, s_c2_wstr };
                int32 rf_data[12] = {0};    
                
               
                   GetRFCellInfo(&rf_info, cell);
                rf_data[0] = rf_info.BCH;
                rf_data[1] = rf_info.BRXL;
                rf_data[2] = rf_info.BSIC;
                rf_data[3] = rf_info.DSC;
                rf_data[4] = rf_info.TXLEV;
                rf_data[5] = rf_info.TN;
                rf_data[6] = rf_info.RLT;
                rf_data[7] = rf_info.TAV;
                rf_data[8] = rf_info.CBA;
                rf_data[9] = rf_info.CBQ;
                rf_data[10] = rf_info.C1;
                rf_data[11] = rf_info.C2;               
                for(i = 0; i < 6; i ++)
                {                    
                    for (j = 0; j < 2; j ++)
                    {  
                        //draw content text
                        len = MMIAPICOM_Wstrlen(wstr_ptr[i * 2 + j]);                    
                      MMIAPICOM_Wstrncpy(buffer + offset, wstr_ptr[i * 2 + j],len);
                      offset += len ;     

                       len = MMIAPICOM_Wstrlen(s_space_wstr);
                       MMIAPICOM_Wstrncpy(buffer + offset, s_space_wstr,len);
                        offset += len ;

                     //draw content ":"
                       MMIAPICOM_Wstrncpy(buffer + offset, &ch,1);
                      offset += 1 ;    

                       len = MMIAPICOM_Wstrlen(s_space_wstr);
                      MMIAPICOM_Wstrncpy(buffer + offset, s_space_wstr,len);
                      offset += len ;
                        //draw rf data
                      sprintf(rf_data_str, "%d  ", (int)rf_data[i * 2 + j]);
                      len = strlen(rf_data_str);
                      MMI_STRNTOWSTR( wstr, 20, (uint8*)rf_data_str, len, len );
                      MMIAPICOM_Wstrncpy(buffer + offset, wstr,len);
                      offset += len ;                      
                 }
                  MMIAPICOM_Wstrncpy(buffer + offset, w_cr,1);
                    offset += 1 ;   
                }
                GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, buffer,offset, TRUE);
                break;
            } 
            
        case MMIENG_RF_CELL_N0:
        case MMIENG_RF_CELL_N1:
        case MMIENG_RF_CELL_N2:
        case MMIENG_RF_CELL_N3:
        case MMIENG_RF_CELL_N4:
        case MMIENG_RF_CELL_N5:      
            {
                const wchar * wstr_ptr[] = { s_bch_wstr, s_brxl_wstr, s_bsic_wstr, s_cba_wstr, s_celid_wstr,
                     s_cbq_wstr, s_alig_wstr, s_c1_wstr, s_lac_wstr, s_c2_wstr, s_ffset_wstr, PNULL };
                const wchar * title_ptr[] = { s_c0_wstr, s_n0_wstr, s_n1_wstr, s_n2_wstr, s_n3_wstr, s_n4_wstr, s_n5_wstr, PNULL };
                int32 rf_data[12] = {0};            
                   GetRFCellInfo(&rf_info, cell);
                rf_data[0] = rf_info.BCH;
                rf_data[1] = rf_info.BRXL;
                rf_data[2] = rf_info.BSIC;
                rf_data[3] = rf_info.CBA;
                rf_data[4] = rf_info.CELID;
                rf_data[5] = rf_info.CBQ;
                rf_data[6] = rf_info.ALIG;
                rf_data[7] = rf_info.C1;
                rf_data[8] = rf_info.LAC;
                rf_data[9] = rf_info.C2;
                rf_data[10] = rf_info.FFSET;

                //draw title text                        
                  len = MMIAPICOM_Wstrlen(title_ptr[cell]);
                  MMI_STRNTOWSTR( wstr, 20, (uint8*)rf_data_str, len, len );
                  MMIAPICOM_Wstrncpy(buffer + offset, title_ptr[cell],len);
                  offset += len ;        
                  
                  MMIAPICOM_Wstrncpy(buffer + offset, w_cr,1);
                    offset += 1 ;   
                                           
                for(i = 0; i < 6; i ++)
                {                   
                    for (j = 0; j < 2; j ++)
                    {                       
                      if((i * 2 + j) >= 11) break;
                        //draw content text
                        len = MMIAPICOM_Wstrlen(wstr_ptr[i * 2 + j]);                    
                      MMIAPICOM_Wstrncpy(buffer + offset, wstr_ptr[i * 2 + j],len);
                      offset += len ;     
                      
                       len = MMIAPICOM_Wstrlen(s_space_wstr);
                       MMIAPICOM_Wstrncpy(buffer + offset, s_space_wstr,len);
                        offset += len ;

                     //draw content ":"
                       MMIAPICOM_Wstrncpy(buffer + offset, &ch,1);
                      offset += 1 ;    

                       len = MMIAPICOM_Wstrlen(s_space_wstr);
                      MMIAPICOM_Wstrncpy(buffer + offset, s_space_wstr,len);
                      offset += len ;
                      
                        //draw rf data
                      sprintf(rf_data_str, "%d  ", (int)rf_data[i * 2 + j]);
                      len = strlen(rf_data_str);
                      MMI_STRNTOWSTR( wstr, 20, (uint8*)rf_data_str, len, len );
                      MMIAPICOM_Wstrncpy(buffer + offset, wstr,len);
                      offset += len ;                      
                 }
                  MMIAPICOM_Wstrncpy(buffer + offset, w_cr,1);
                    offset += 1 ;   
                }   
                 GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, buffer,offset, TRUE);
                break;
            }
            
        default:
            //SCI_TRACE_LOW:"MMIENG_WIN.C DrawRFWin cell is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_9290_112_2_18_2_17_59_171,(uint8*)"d",cell);
            break;
        }
}

/********************************************************************************
 NAME:          MMIAPIENG_CreateWinDVFrameFre
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        songbin.zeng
 DATE:          2006.12.05
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateWinDVFrameFre(void)
{
    return MMK_CreateWin((uint32 * )MMIENG_DV_FRAME_FRE_TAB, PNULL);
}

#if 0
/********************************************************************************
 NAME:          EngWapUAWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        
 DATE:      
********************************************************************************/
LOCAL MMI_RESULT_E EngWapUAProfileWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMIBROWSER_UA_T     wap_ua={0};
    MMI_CTRL_ID_T       editbox_ctrl_id = MMIENG_WAPUAPROFILE_CTRL_ID;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MMI_STRING_T string_info = {0};
    uint16              wap_ua_len = 0;
    wchar wstr[MMIBROWSER_MAX_URL_LEN + 1] = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set im
        GUIEDIT_SetIm(editbox_ctrl_id,
            GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,
            GUIIM_TYPE_ENGLISH);

        SCI_MEMSET(&wap_ua,0,sizeof(MMIBROWSER_UA_T));
        MMINV_READ(MMINV_BROWSER_UA,&wap_ua,return_value);
        if (return_value==MN_RETURN_SUCCESS)
        {
            wap_ua_len = strlen((char *)(wap_ua.uaprofile));

            MMI_STRNTOWSTR( wstr, MMIBROWSER_MAX_URL_LEN, wap_ua.uaprofile, wap_ua_len, wap_ua_len );

            GUIEDIT_SetString(editbox_ctrl_id,
                                            wstr,
                                            wap_ua_len
                                            );
        }
        else
        {
            GUIEDIT_ClearAllStr(editbox_ctrl_id);
        }
        MMK_SetAtvCtrl(win_id,editbox_ctrl_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK: 
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        //save to nv
        SCI_MEMSET(&string_info,0,sizeof(MMI_STRING_T));
        GUIEDIT_GetString(
                                    editbox_ctrl_id,
                                    &string_info);

        MMI_WSTRNTOSTR( wap_ua.uaprofile, MMIBROWSER_MAX_URL_LEN, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len );
        
        MMIBRW_SetUAProfile( wap_ua.uaprofile, string_info.wstr_len);
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngWapUAWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        
 DATE:      
********************************************************************************/
LOCAL MMI_RESULT_E EngWapUAWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMIBROWSER_UA_T     wap_ua={0};
    MMI_CTRL_ID_T       editbox_ctrl_id = MMIENG_WAPUA_CTRL_ID;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MMI_STRING_T string_info = {0};
    uint16              wap_ua_len = 0;
    wchar wstr[MMIBROWSER_MAX_URL_LEN + 1] = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set im
        GUIEDIT_SetIm(editbox_ctrl_id,
            GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,
            GUIIM_TYPE_ENGLISH);

        SCI_MEMSET(&wap_ua,0,sizeof(MMIBROWSER_UA_T));
        MMINV_READ(MMINV_BROWSER_UA,&wap_ua,return_value);
        if (return_value==MN_RETURN_SUCCESS)
        {
            wap_ua_len = strlen((char *)(wap_ua.user_agent));

            MMI_STRNTOWSTR( wstr, MMIBROWSER_MAX_URL_LEN, wap_ua.user_agent, wap_ua_len, wap_ua_len );

            GUIEDIT_SetString(editbox_ctrl_id,
                                            wstr,
                                            wap_ua_len
                                            );
        }
        else
        {
            GUIEDIT_ClearAllStr(editbox_ctrl_id);
        }
        MMK_SetAtvCtrl(win_id,editbox_ctrl_id);
        break;

    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        //save to nv
        SCI_MEMSET(&string_info,0,sizeof(MMI_STRING_T));
        GUIEDIT_GetString(
                                    editbox_ctrl_id,
                                    &string_info);

        MMI_WSTRNTOSTR( wap_ua.user_agent, MMIBROWSER_MAX_URL_LEN, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len );

        MMIBRW_SetUA( wap_ua.user_agent, string_info.wstr_len);
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngWapMmsSasTestModeWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        hui.zhao
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngWapMmsSasTestModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    BOOLEAN is_test_mode_open = FALSE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        //GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        
        if (MMIAPIENG_GetWapMmsSasTestMode())
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;  
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (0 == cur_selection)
        {
            is_test_mode_open = TRUE;
        }
        else
        {
            is_test_mode_open = FALSE;
        }     
        SetWapMmsSasTestMode(is_test_mode_open);
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);      
        MMK_CloseWin( win_id );
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          SetWapMmsSasTestMode
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        hui.zhao
 DATE:          2009.07.27
********************************************************************************/
LOCAL BOOLEAN SetWapMmsSasTestMode(BOOLEAN is_on)
{
    MMINV_WRITE(MMIENG_NV_WAPMMS_SAS_TEST_MODE_ID, &is_on);

    return TRUE;
}
#endif

/********************************************************************************
 NAME:          BtAddressInfoWinHandle
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        
 DATE:      
********************************************************************************/
LOCAL MMI_RESULT_E BtAddressInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

#ifdef  BLUETOOTH_SUPPORT
    
    MMI_STRING_T        address_info_str = {0};
    BT_ADDRESS          device_addr = {0};
    uint8               addr_str[MMIENG_ADDR_INFO_MAX_LEN] = {0};
    wchar               addr_wstr[MMIENG_ADDR_INFO_MAX_LEN] = {0};    
    MMI_CTRL_ID_T       ctrl_id = MMIENG_BTADDRESS_CTRL_ID;  

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:               
        MMIAPIBT_GetAddress(&device_addr);
        sprintf((char *)addr_str, "%02x:%02x:%02x:%02x:%02x:%02x",device_addr.addr[5],device_addr.addr[4],device_addr.addr[3],
        device_addr.addr[2],device_addr.addr[1],device_addr.addr[0]);
        address_info_str.wstr_len = MIN(strlen((char *)addr_str), MMIENG_ADDR_INFO_MAX_LEN);/*lint !e666*/
        address_info_str.wstr_ptr = addr_wstr;
        MMI_STRNTOWSTR(address_info_str.wstr_ptr, MMIENG_ADDR_INFO_MAX_LEN, addr_str, MMIENG_ADDR_INFO_MAX_LEN, address_info_str.wstr_len);
        //address_info_str.is_ucs2 = FALSE;
        GUITEXT_SetString(ctrl_id,address_info_str.wstr_ptr, address_info_str.wstr_len, FALSE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
        break;


    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

#endif
    
    return (result);
}
#ifndef WIN32
#ifdef  BLUETOOTH_SUPPORT_SPRD_BT
/********************************************************************************
 NAME:          BtAddressInfoWinHandle
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        
 DATE:      
********************************************************************************/
LOCAL MMI_RESULT_E BtVerInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    uint32                        len = 0;
    const char*                       ver_info_ptr = PNULL;
    wchar*                     dst_ptr = PNULL;
    MMI_CTRL_ID_T       ctrl_id = MMIENG_BTVER_CTRL_ID;  

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:               
        ver_info_ptr = SCI_GetBTVersionInfo();
        len = strlen(ver_info_ptr);
        dst_ptr =SCI_ALLOC_APP((len+1)*sizeof(wchar));
        SCI_MEMSET(dst_ptr,0,((len+1)*sizeof(wchar)));
        MMIAPICOM_StrToWstr(ver_info_ptr, dst_ptr);/*lint !e64*/
        GUITEXT_SetString(ctrl_id,dst_ptr, len, FALSE);
        SCI_FREE(dst_ptr);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
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
#endif

#endif
/********************************************************************************
 NAME:          BTSSPDebugModeWinHandleMsg   
 AUTHOR:        Leon.Wang
 DATE:          2012.11.14
********************************************************************************/
LOCAL MMI_RESULT_E BTSSPDebugModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E  result = MMI_RESULT_TRUE;
#ifdef BLUETOOTH_SUPPORT
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16        cur_selection = 0;
	BOOLEAN       ssp_state = FALSE;
	BT_STATUS     ret_code = BT_SUCCESS;
	MMI_STRING_T  error_info = {0};
	char          error_txt[50] = {0};
	wchar         error_wtxt[50] = {0};
#ifndef WIN32
	ssp_state = BT_GetSSPDebugMode();
#endif
    switch (msg_id) 
    {
	case MSG_OPEN_WINDOW:
		GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
		GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
		MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
		
		if (!ssp_state)
		{
			cur_selection = 1;
		}		
		GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
		GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
		MMK_SetAtvCtrl(win_id,ctrl_id);
		break;
		
	case MSG_FULL_PAINT:
		break;
		
	case MSG_KEYDOWN_CANCEL:
	case MSG_KEYDOWN_OK:
		break;
		
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_MIDSK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
		cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
#ifndef WIN32
		if (0 == cur_selection && !ssp_state)
		{
		     ret_code = BT_SetSSPDebugMode(TRUE);
		}
		else if(1 == cur_selection && ssp_state)
		{
			 ret_code = BT_SetSSPDebugMode(FALSE);
		}
#endif
		if(BT_NOT_SUPPORTED==ret_code)
		{
		    sprintf(error_txt, "SSP Debug Mode is not support!");
			MMIAPICOM_StrToWstr((uint8*)error_txt, error_wtxt);
			error_info.wstr_ptr = error_wtxt;
			error_info.wstr_len = MMIAPICOM_Wstrlen(error_wtxt);

			MMIPUB_OpenAlertTextWinByTextPtr(PNULL, //display error_code
                &error_info,
			    PNULL,
				PNULL,
				MMIPUB_SOFTKEY_ONE,
			    PNULL);
		}
		else if(BT_SUCCESS!= ret_code)
		{
			sprintf(error_txt, "Error Code: %d", (uint16)(ret_code));
			MMIAPICOM_StrToWstr((uint8*)error_txt, error_wtxt);
			error_info.wstr_ptr = error_wtxt;
			error_info.wstr_len = MMIAPICOM_Wstrlen(error_wtxt);
			
			MMIPUB_OpenAlertTextWinByTextPtr(PNULL, //display error_code
                &error_info,
				PNULL,
				PNULL,
				MMIPUB_SOFTKEY_ONE,
			    PNULL);
		}
		
		MMK_CloseWin(win_id);
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
    }

#endif
	return result;
}

/********************************************************************************
 NAME:          BTBQBModeWinHandleMsg   
 AUTHOR:        Leon.Wang
 DATE:          2012.12.4
********************************************************************************/
LOCAL MMI_RESULT_E BTBQBModeWinHandleMsg(MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param)
{
	MMI_RESULT_E  result = MMI_RESULT_TRUE;
#ifdef BLUETOOTH_SUPPORT
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16        cur_selection = 0;
	BOOLEAN       bqb_state = FALSE;
#ifndef WIN32
	bqb_state = UART_GetControllerBqbMode();
#endif
    switch (msg_id) 
    {
	case MSG_OPEN_WINDOW:
		GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
		GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
		MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
		
		if (!bqb_state)/*lint !e774*/
		{
			cur_selection = 1;
		}		
		GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
		GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
		MMK_SetAtvCtrl(win_id,ctrl_id);
		break;
		
	case MSG_FULL_PAINT:
		break;
		
	case MSG_KEYDOWN_CANCEL:
	case MSG_KEYDOWN_OK:
		break;
		
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_MIDSK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
		cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
		SCI_TRACE_LOW("mymark_cur_selection=%d  bqb_state=%d",cur_selection,bqb_state);
#ifndef WIN32
		if (0 == cur_selection && !bqb_state)/*lint !e774*/
		{
 			UART_SetControllerBqbMode(TRUE); 
		    //SIO_SetControllerBqbMode(TRUE);
		}
		else if(1 == cur_selection && bqb_state)/*lint !e774*/
		{
			UART_SetControllerBqbMode(FALSE);
		    //SIO_SetControllerBqbMode(FALSE);
		}
#endif
		MMK_CloseWin(win_id);
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
    }

#endif
	return result;
}
/*lint +e628*/

/********************************************************************************
 NAME:          EngBTopenSSPDebugWin   
 AUTHOR:        Leon.Wang
 DATE:          2012.11.14
********************************************************************************/
PUBLIC void EngBTopenSSPDebugWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_BT_SSP_DEBUG_TAB, PNULL);
}

/********************************************************************************
 NAME:          EngBTopenBQPModeWin   
 AUTHOR:        Leon.Wang
 DATE:          2012.12.4
********************************************************************************/
PUBLIC void EngBTopenBQBModeWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_BT_BQB_MODE_TAB, PNULL);
}
#ifdef GPS_SUPPORT
/********************************************************************************
 NAME:          SetGPSText
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        James.Zhang
 DATE:      0   2007.11.28
********************************************************************************/
LOCAL void SetGPSText( 
                      MMI_WIN_ID_T  win_id,
                      MMI_CTRL_ID_T ctrl_id,
                      BOOLEAN       is_open,
                      BOOLEAN       is_need_update
                      )
{
    MMI_STRING_T string   = {0};
    uint8*       text_ptr = SCI_ALLOC_APP( GPS_TEST_TEXT_LEN + 1 );
    wchar*       wstr_ptr = SCI_ALLOC_APP( ( GPS_TEST_TEXT_LEN + 1 ) * sizeof(wchar) );
    
    if(PNULL == text_ptr)
    {
        //SCI_TRACE_LOW:"mmieng_win.c SetGPSText text_ptr == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_9621_112_2_18_2_18_0_172,(uint8*)"");
        return FALSE;
    } 
    
    SCI_MEMSET( text_ptr, 0, GPS_TEST_TEXT_LEN + 1 );
    SCI_MEMSET( wstr_ptr, 0, ( GPS_TEST_TEXT_LEN + 1 ) * sizeof(wchar) );

    string.wstr_ptr = wstr_ptr;
    
    if( is_open )
    {
        uint16          i            = 0;
        uint16          star_in_view = 0;
        BOOLEAN         is_rf_ok     = FALSE;
        BOOLEAN         is_update_title = FALSE;
        GPS_INFO_T_PTR  gps_info_ptr = PNULL;
        const uint8*    south_string = (const uint8*)"SOUTH";
        const uint8*    north_string = (const uint8*)"NORTH";
        const uint8*    west_string  = (const uint8*)"WEST";
        const uint8*    east_string  = (const uint8*)"EAST";
        const uint8*    invalid_string  = (const uint8*)"INVALID";
        const uint8*    no_string    = (const uint8*)"NOT READY";
        const uint8*    ok_string    = (const uint8*)"OK";
        const uint8*    north_or_south_string = PNULL;
        const uint8*    east_or_west_string = PNULL;
        uint8           title_text[50] = {0};
        wchar           title_wstr[50] = {0};
        uint8*          gps_test_ptr = SCI_ALLOC_APP( GPS_TEST_TEMP_BUF_LEN );
        uint16          len = 0;
        
        if(PNULL == gps_test_ptr)
        {
            //SCI_TRACE_LOW:"mmieng_win.c SetGPSText gps_test_ptr == null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_9653_112_2_18_2_18_0_173,(uint8*)"");
            return ;
        }   
        
        GPS_ReadData( gps_test_ptr, GPS_TEST_TEMP_BUF_LEN );
        gps_info_ptr = GPS_GetGpsInfo();
        
        if(PNULL == gps_info_ptr)
        {
            //SCI_TRACE_LOW:"mmieng_win.c SetGPSText GPS_GetGpsInfo gps_info_ptr == null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_9662_112_2_18_2_18_0_174,(uint8*)"");
            return ;
        }   
        
        /* is position fix */
        if( !gps_info_ptr->is_position_fix )
        {
            is_update_title = !is_need_update;
            
            if( gps_info_ptr->is_position_fix != s_gps_test_pre_info.is_position_fix )
            {
                s_gps_test_position_delay_time++;
                
                /* update all info */
                if( s_gps_test_position_delay_time > GPS_TEST_DELAY_TIMES )
                {
                    s_gps_test_position_fix_time = 0;
                    s_gps_test_position_delay_time = 0;
                    
                    s_gps_test_pre_info = *gps_info_ptr;
                    
                    is_update_title = TRUE;
                }
                /* don't update fix time, sf, latitude, longitude */
                else
                {
                    BOOLEAN pre_is_position_fix = s_gps_test_pre_info.is_position_fix;
                    uint16  pre_cnt_of_sat_fix  = s_gps_test_pre_info.cnt_of_sat_fix;
                    uint16  pre_north_or_south  = s_gps_test_pre_info.north_or_south;
                    uint16  pre_latitude_high   = s_gps_test_pre_info.latitude_high;
                    uint16  pre_latitude_low    = s_gps_test_pre_info.latitude_low;
                    uint16  pre_east_or_west    = s_gps_test_pre_info.east_or_west;
                    uint16  pre_longitude_high  = s_gps_test_pre_info.longitude_high;
                    uint16  pre_longitude_low   = s_gps_test_pre_info.longitude_low;
                    
                    s_gps_test_pre_info = *gps_info_ptr;
                    
                    s_gps_test_pre_info.is_position_fix = pre_is_position_fix;
                    s_gps_test_pre_info.cnt_of_sat_fix  = pre_cnt_of_sat_fix;
                    s_gps_test_pre_info.north_or_south  = pre_north_or_south;
                    s_gps_test_pre_info.latitude_high   = pre_latitude_high;
                    s_gps_test_pre_info.latitude_low    = pre_latitude_low;
                    s_gps_test_pre_info.east_or_west    = pre_east_or_west;
                    s_gps_test_pre_info.longitude_high  = pre_longitude_high;
                    s_gps_test_pre_info.longitude_low   = pre_longitude_low;
                }
            }
            else
            {
                s_gps_test_position_fix_time++;
                
                s_gps_test_pre_info = *gps_info_ptr;
            }
        }
        else
        {
            if( gps_info_ptr->is_position_fix != s_gps_test_pre_info.is_position_fix )
            {
                is_update_title = TRUE;
            }
            
            s_gps_test_pre_info = *gps_info_ptr;
        }
        
        if( is_update_title )
        {
            if( !s_gps_test_pre_info.is_position_fix )
            {
                sprintf( (char*)title_text," GPS position..." );
            }
            else
            {
                sprintf( (char*)title_text," GPS position fix time is %ld s", s_gps_test_position_fix_time );
            }

            len = strlen( (const char*)title_text );
            MMI_STRNTOWSTR( title_wstr, 50, title_text, 50, len );

            GUIWIN_SetTitleText( win_id, title_wstr, len, is_need_update );
        }
        
        /* is rf ok */
        if( s_gps_test_pre_info.cnt_of_chn_valid > 0 )
        {
            for( i = 0; i < s_gps_test_pre_info.cnt_of_chn_valid; i++ )
            {
                if( s_gps_test_pre_info.gps_chn[i].snr > 0 )
                {
                    star_in_view++;
                    
                    if( s_gps_test_pre_info.gps_chn[i].snr > GPS_TEST_MIN_VALID_SNR )
                    {
                        is_rf_ok = TRUE;
                    }
                }
            }
        }
        
        if( 'N' == s_gps_test_pre_info.north_or_south )
        {
            north_or_south_string = north_string;
        }
        else if( 'S' == s_gps_test_pre_info.north_or_south )
        {
            north_or_south_string = south_string;
        }
        else
        {
            north_or_south_string = invalid_string;
        }
        
        if( 'E' == s_gps_test_pre_info.east_or_west )
        {
            east_or_west_string = east_string;
        }
        else if( 'W' == s_gps_test_pre_info.east_or_west )
        {
            east_or_west_string = west_string;
        }
        else
        {
            east_or_west_string = invalid_string;
        }
        
        sprintf( (char*)text_ptr, " PTime: %ld \n BB: %-10s RF: %-10s\n SIV: %-10d SF: %-10d\n latitude  %s: %5d.%4d\n longitude %s: %5d.%4d\n valid chn num: %d\n",
            s_gps_test_position_fix_time,
            ( s_gps_test_pre_info.is_hw_work ) ? ok_string : no_string, ( is_rf_ok ) ? ok_string : no_string,
            star_in_view, s_gps_test_pre_info.cnt_of_sat_fix, 
            north_or_south_string, s_gps_test_pre_info.latitude_high, s_gps_test_pre_info.latitude_low,
            east_or_west_string, s_gps_test_pre_info.longitude_high, s_gps_test_pre_info.longitude_low,
            s_gps_test_pre_info.cnt_of_chn_valid );
        
        string.wstr_len  = strlen( (const char*)text_ptr );
        
        /* detail text */
        sprintf( (char*)( text_ptr + string.wstr_len ), " CID  SID  SNR     CID  SID  SNR\n");
        string.wstr_len += strlen( (const char*)( text_ptr + string.wstr_len ) );
        
        for( i = 0; i < s_gps_test_pre_info.cnt_of_chn_valid; i++ )
        {
            sprintf( (char*)( text_ptr + string.wstr_len ), " %5d%5d%5d     ",
                i + 1, s_gps_test_pre_info.gps_chn[i].sat_id, s_gps_test_pre_info.gps_chn[i].snr );
            
            string.wstr_len += strlen( (const char*)( text_ptr + string.wstr_len ) );
            
            if( 0 != i % 2 )
            {
                sprintf( (char*)( text_ptr + string.wstr_len ), "\n");
                string.wstr_len += strlen( (const char*)( text_ptr + string.wstr_len ) );
            }
        }
        
        SCI_FREE( gps_test_ptr );
    }
    else
    {
        sprintf( (char*)text_ptr, "GPS HW Module is not existed" );
        string.wstr_len = strlen( (const char*)( text_ptr + string.wstr_len ) );
    }
    
    MMI_STRNTOWSTR( wstr_ptr, GPS_TEST_TEXT_LEN, text_ptr, GPS_TEST_TEXT_LEN, string.wstr_len );

    GUITEXT_SetString( ctrl_id, string.wstr_ptr, string.wstr_len, is_need_update );
    
    SCI_FREE( text_ptr );
    SCI_FREE( wstr_ptr );
}

/********************************************************************************
 NAME:          EngShowGPSWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        jian.ma
 DATE:      0   2007.07.23
********************************************************************************/
LOCAL MMI_RESULT_E EngShowGPSWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"EngShowGPSWinHandleMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_9841_112_2_18_2_18_1_175,(uint8*)"d", msg_id);
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            if( GPS_ERR_NONE == GPS_Open( GPS_MODE_TEST_NMEA ) )
            {
                s_gps_test_open = TRUE;
                
                MMIDEFAULT_AllowTurnOffBackLight( FALSE );
                
                s_gps_test_timer = MMK_CreateWinTimer( win_id, GPS_TEST_TIMER_OUT, TRUE );
                
                SCI_MEMSET( &s_gps_test_pre_info, 0, sizeof(GPS_INFO_T) );
                s_gps_test_position_fix_time = 0;
                s_gps_test_position_delay_time = 0;
            }
            else
            {
                s_gps_test_open = FALSE;

                GUIWIN_SetTitleTextId( win_id, TXT_ENG_GPS_TEST, FALSE );
            }

            SetGPSText( win_id, MMIENG_TEXTBOX1_CTRL_ID, s_gps_test_open, FALSE );
            MMK_SetAtvCtrl( win_id, MMIENG_TEXTBOX1_CTRL_ID );
        }
        break;
        
    case MSG_TIMER:
        {
            SetGPSText( win_id, MMIENG_TEXTBOX1_CTRL_ID, s_gps_test_open, TRUE );
        }
        break;

    case MSG_CTL_CANCEL:
        {
            if( s_gps_test_open )
            {
                if ( 0 != s_gps_test_timer )
                {
                    MMK_StopTimer( s_gps_test_timer );
                    s_gps_test_timer = 0;
                }
                
                MMIDEFAULT_AllowTurnOffBackLight( TRUE );
                
                GPS_Close();
            }
            
            MMK_CloseWin( win_id );
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          MMIENG_OpenGPSTestWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        jian.ma
 DATE:      0   2007.07.23
********************************************************************************/
PUBLIC void MMIENG_OpenGPSTestWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_GPS_WIN_TAB, PNULL);
}
#endif

/********************************************************************************
 NAME:          MMIAPIENG_CreateChipTestWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        ryan.xu
 DATE:          2007.7.31
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateChipTestWin(void)
{
    return MMK_CreateWin((uint32*)MMIENG_CHIP_TEST_WIN_TAB, PNULL);
}

/******************************************************************************
**
 NAME:          MMIAPIENG_CreateCMMBTestWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR: xin.li
 DATE: 2009.07.23
*******************************************************************************
*/
#ifdef CMMB_SUPPORT
/********************************************************************************
 NAME:          MMIAPIENG_CreateCMMBChannelStatusWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR: xin.li
 DATE:2009.07.23
********************************************************************************/
BOOLEAN MMIAPIENG_CreateCMMBTestWin(void)
{
    return MMK_CreateWin((uint32*)MMIENG_CMMB_TEST_MAIN_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:          MMIAPIENG_CreateCMMBChannelStatusWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR: xin.li
 DATE:2009.07.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateCMMBChannelStatusWin(void)
{
    if(MMK_IsOpenWin(MMIENG_CMMB_DISPLAY_WIN_ID))
    {
        MMK_CloseWin(MMIENG_CMMB_DISPLAY_WIN_ID);
    }
    
    //SCI_TRACE_LOW:" MMIAPIENG_CreateCMMBChannelStatusWin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_9964_112_2_18_2_18_1_176,(uint8*)"");
    return MMK_CreateWin((uint32*)MMIENG_CMMB_DISPLAY_WIN_TAB, PNULL);
}
#endif

/********************************************************************************
 NAME:			MMIAPIENG_CreateTFloadWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: tao.feng
 DATE: 2010.12.16
********************************************************************************/
#ifdef TF_LOAD_SUPPORT
BOOLEAN MMIAPIENG_CreateTFloadWin(void)
{
    return MMK_CreateWin((uint32*)MMIENG_TFLOAD_WIN_TAB, PNULL);
}
#endif

/*****************************************************************************/
//  Description : chip test mp3 play window
//  Global resource dependence : 
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChipTestPlayMp3MP4WinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    LOCAL uint8 flv_play_timer_id = 0;
    LOCAL uint8 h264_play_timer_id = 0;    
    const wchar flv_file_name[] = {'D', ':','\\', 'V', 'i', 'd', 'e', 'o', '\\', 't', 'e', 's', 't', '.', 'f', 'l', 'v', 0};
    const wchar flv_file_name1[] = {'E', ':','\\', 'V', 'i', 'd', 'e',  'o', '\\', 't', 'e', 's', 't', '.', 'f', 'l', 'v', 0};  
#ifdef H264_DEC_SUPPORT
    const wchar h264_file_name[] = {'D', ':','\\', 'V', 'i', 'd', 'e', 'o', '\\', 't', 'e', 's', 't', '.', 'm', 'p', '4', 0};
    const wchar h264_file_name1[] = {'E', ':','\\', 'V', 'i', 'd', 'e',  'o', '\\', 't', 'e', 's', 't', '.', 'm', 'p', '4', 0};  
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:             
#ifdef VIDEO_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
        MMIAPIVP_MiniFunction_ExitVideoPlayer();
#else
        MMIAPIVP_ExitVideoPlayer();   
#endif
        if(MMIAPIFMM_IsFileExist(flv_file_name, MMIAPICOM_Wstrlen(flv_file_name)))
        {
            if (flv_play_timer_id != 0)
            {
                MMK_StopTimer(flv_play_timer_id);
                flv_play_timer_id = 0;
            }
            flv_play_timer_id = MMK_CreateTimer(MMI_1SECONDS * 10, FALSE); 
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
            MMIAPIVP_MiniFunction_PlayVideo(flv_file_name, MMIAPICOM_Wstrlen(flv_file_name));
#else
            MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_FILE,flv_file_name, MMIAPICOM_Wstrlen(flv_file_name),PNULL);/*lint !e605*/
#endif
        }
        else if(MMIAPIFMM_IsFileExist(flv_file_name1, MMIAPICOM_Wstrlen(flv_file_name1)))
        {
            if (flv_play_timer_id != 0)
            {
                MMK_StopTimer(flv_play_timer_id);
                flv_play_timer_id = 0;
            }
            flv_play_timer_id = MMK_CreateTimer(MMI_1SECONDS * 10, FALSE); 
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
            MMIAPIVP_MiniFunction_PlayVideo(flv_file_name1, MMIAPICOM_Wstrlen(flv_file_name1));
#else
            MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_FILE,flv_file_name1, MMIAPICOM_Wstrlen(flv_file_name1),PNULL);/*lint !e605*/
#endif
        }
        else
        {
#ifdef H264_DEC_SUPPORT
            if(MMIAPIFMM_IsFileExist(h264_file_name, MMIAPICOM_Wstrlen(h264_file_name)))
            {
                if (h264_play_timer_id != 0)
                {
                    MMK_StopTimer(h264_play_timer_id);
                    h264_play_timer_id = 0;
                }
                h264_play_timer_id = MMK_CreateTimer(MMI_1SECONDS * 10, FALSE); 
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
                MMIAPIVP_MiniFunction_PlayVideo(h264_file_name, MMIAPICOM_Wstrlen(h264_file_name));
#else
                MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_FILE,h264_file_name, MMIAPICOM_Wstrlen(h264_file_name),PNULL);/*lint !e605*/
#endif
            }
            else if(MMIAPIFMM_IsFileExist(h264_file_name1, MMIAPICOM_Wstrlen(h264_file_name1)))
            {
                if (h264_play_timer_id != 0)
                {
                    MMK_StopTimer(h264_play_timer_id);
                    h264_play_timer_id = 0;
                }
                h264_play_timer_id = MMK_CreateTimer(MMI_1SECONDS * 10, FALSE); 
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
                MMIAPIVP_MiniFunction_PlayVideo(h264_file_name1, MMIAPICOM_Wstrlen(h264_file_name1));
#else
                MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_FILE,h264_file_name1, MMIAPICOM_Wstrlen(h264_file_name1),PNULL); /*lint !e605*/                     
#endif
            }
            else
            {
                MMI_STRING_T string = {0};
                char str[] = "Can't find test video file"; 
                int32 length = SCI_STRLEN(str) ;
                wchar wstr[128] = {0};
                MMI_STRNTOWSTR(wstr,  127, (uint8*)str,  length , length);
                string.wstr_ptr = wstr;
                string.wstr_len = length;
                MMIPUB_OpenAlertWinByTextPtr(PNULL, &string, PNULL, IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                //    MMK_CloseWin(win_id);
            }
#else
            MMI_STRING_T string = {0};
            char str[] = "Can't find test video file"; 
            int32 length = SCI_STRLEN(str) ;
            wchar wstr[128] = {0};
            MMI_STRNTOWSTR(wstr,  127, (uint8*)str,  length , length);
            string.wstr_ptr = wstr;
            string.wstr_len = length;
            MMIPUB_OpenAlertWinByTextPtr(PNULL, &string, PNULL, IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
        }
#endif
        break;
        


        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
    case MSG_GET_FOCUS:
        {
            GUI_RECT_T client_rect = MMITHEME_GetClientRect();
            GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
            rect.top = client_rect.top ;
            LCD_FillRect(MMITHEME_GetDefaultLcdDev(), rect, MMI_BACKGROUND_COLOR);  
        }
        break;
        
    case MSG_TIMER: 
#ifdef VIDEO_PLAYER_SUPPORT
        if(flv_play_timer_id == *((uint8 *) param))
        {
            if(0 != flv_play_timer_id)
            {
                MMK_StopTimer(flv_play_timer_id);
                flv_play_timer_id = 0; 
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
            MMIAPIVP_MiniFunction_ExitVideoPlayer();
#else
            MMIAPIVP_ExitVideoPlayer();   
#endif
#ifdef H264_DEC_SUPPORT
                if(MMIAPIFMM_IsFileExist(h264_file_name, MMIAPICOM_Wstrlen(h264_file_name)))
                {
                    if (h264_play_timer_id != 0)
                    {
                        MMK_StopTimer(h264_play_timer_id);
                        h264_play_timer_id = 0;
                    }
                    h264_play_timer_id = MMK_CreateTimer(MMI_1SECONDS * 10, FALSE); 
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
                    MMIAPIVP_MiniFunction_PlayVideo(h264_file_name, MMIAPICOM_Wstrlen(h264_file_name));
#else
                    MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_FILE,h264_file_name, MMIAPICOM_Wstrlen(h264_file_name),PNULL);/*lint !e605*/
#endif
                }
                else if(MMIAPIFMM_IsFileExist(h264_file_name1, MMIAPICOM_Wstrlen(h264_file_name1)))
                {
                    if (h264_play_timer_id != 0)
                    {
                        MMK_StopTimer(h264_play_timer_id);
                        h264_play_timer_id = 0;
                    }
                    h264_play_timer_id = MMK_CreateTimer(MMI_1SECONDS * 10, FALSE); 
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
                    MMIAPIVP_MiniFunction_PlayVideo(h264_file_name1, MMIAPICOM_Wstrlen(h264_file_name1));
#else
                    MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_FILE,h264_file_name1, MMIAPICOM_Wstrlen(h264_file_name1),PNULL); /*lint !e605*/                     
#endif
                }
                else
                {
                    MMI_STRING_T string = {0};
                    char str[] = "Can't find test video file"; 
                    int32 length = SCI_STRLEN(str) ;
                    wchar wstr[128] = {0};
                    MMI_STRNTOWSTR(wstr,  127, (uint8*)str,  length , length);
                    string.wstr_ptr = wstr;
                    string.wstr_len = length;
                    MMIPUB_OpenAlertWinByTextPtr(PNULL, &string, PNULL, IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    //    MMK_CloseWin(win_id);
                }
#endif     
            }
        }
        else if(h264_play_timer_id == *((uint8 *) param))
        {
            if(0 != h264_play_timer_id)
            {
                MMK_StopTimer(h264_play_timer_id);
                h264_play_timer_id = 0;   
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
            MMIAPIVP_MiniFunction_ExitVideoPlayer();
#else                
                MMIAPIVP_ExitVideoPlayer();    
#endif
            }                    
        }    
#endif
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_OK:
    case MSG_APP_WEB:
#ifdef MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
            MMIAPIVP_MiniFunction_ExitVideoPlayer();
#else
        MMIAPIVP_ExitVideoPlayer();
#endif
#endif
        MMK_CreateWin((uint32*)MMIENG_CHIP_TEST_DC_WIN_TAB, PNULL);    
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

/*****************************************************************************/
//  Description : chip test dc window
//  Global resource dependence : 
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChipTestDCWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)  
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIAPIDC_OpenForChipTest();
        break;
     
    case MSG_FULL_PAINT: 
        break;

     case MSG_GET_FOCUS:
         {
             GUI_RECT_T client_rect = MMITHEME_GetClientRect();
             GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
             rect.top =  client_rect.top ;
             LCD_FillRect(MMITHEME_GetDefaultLcdDev(), rect, MMI_BACKGROUND_COLOR);             
         }        
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_OK:
    case MSG_APP_WEB:
#ifdef MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
        MMK_CreateWin((uint32*)MMIENG_CHIP_TEST_LOOPBACK_WIN_TAB, PNULL);
        MMK_CloseWin(win_id);
        break;  
        
    default:
        break;
    }      
    return result;
}

/********************************************************************************
NAME:           UITestLoopbackWinHandleMsg
DESCRIPTION:    
PARAM IN:       
PARAM OUT:      
AUTHOR:     allen
DATE:           2004.09.08
********************************************************************************/
LOCAL MMI_RESULT_E HandleChipTestLoopBackWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
//     LOCAL AUDIO_DEVICE_MODE_TYPE_E old_aud_dev_mode = {AUDIO_DEVICE_MODE_HANDHOLD};
    GUI_RECT_T lab_rect = MMITHEME_GetClientRect();
    GUI_BOTH_RECT_T  lab_both_rect = MMITHEME_GetWinTitleBothRect(win_id);
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    static  uint16  original_call_volume =  0;  
    const  uint16  test_call_volume = 9;   
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    if(lcd_width > lcd_height)
    {
        lab_rect.top = lab_both_rect.h_rect.bottom;
    }
    else
    {
        lab_rect.top = lab_both_rect.v_rect.bottom;
    }

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:              
        original_call_volume =  MMIAPISET_GetCallVolume();  
       
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);         
        GUILABEL_SetBackgroundColor(MMIENG_UITEST_LABEL_CTRL_ID, mmi_uitest_display.bkgd_color);
        GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, STXT_UP, TXT_NULL, FALSE);  

        MMISRVAUD_EnableVoiceLB(TRUE, test_call_volume) ;
       
        break;

    case MSG_LCD_SWITCH:   
        break;
            
    case MSG_FULL_PAINT:
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_OK:
    case MSG_APP_WEB:
#ifdef MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
//#ifdef MMI_ENG_GREENKEYTONEXT_ENABLE
//  case MSG_KEYDOWN_GREEN:
//#endif
    
        MMK_CloseWin(win_id);
        break;
                
    case MSG_KEYDOWN_CANCEL:
        break;  

    case MSG_CTL_CANCEL:
        break;
    

    case MSG_CTL_OK:
        break;
        
    case MSG_GET_FOCUS:
        MMISRVAUD_EnableVoiceLB(TRUE, test_call_volume) ;
        break;
        
    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        //AUD_EnableVoiceLoopback(hVoiceRing, FALSE, 100);
        MMISRVAUD_EnableVoiceLB(FALSE, original_call_volume) ;         
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : start chip test
//  Global resource dependence : 
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIENGAPI_StartChipTest(void)
{   
#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
            MMK_CreateWin((uint32*)MMIENG_CHIPTEST_TP_WIN_TAB, PNULL);   
#else
            MMK_CreateWin((uint32*)MMIENG_CHIP_TEST_PLAYMP3MP4_WIN_TAB, PNULL);         
#endif
    return ;
}

/*****************************************************************************/
//  Description : chip test menu
//  Global resource dependence : 
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChipTestMenuWinMsg(
                                       MMI_WIN_ID_T     win_id,     
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T   menu_id = 0;

    //SCI_TRACE_LOW:"HandleChipTestMenuWinMsg(), msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_10336_112_2_18_2_18_2_177,(uint8*)"d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(MMIENG_CHIP_TEST_MENU_WIN_ID, MMIENG_CHIP_TEST_MENU_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:

        GUIMENU_GetId(MMIENG_CHIP_TEST_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));

        switch ( menu_id )
        {
        case ID_ENG_CHIP_TEST:          
#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
            MMK_CreateWin((uint32*)MMIENG_CHIPTEST_TP_WIN_TAB, PNULL);   
#else
            MMK_CreateWin((uint32*)MMIENG_CHIP_TEST_PLAYMP3MP4_WIN_TAB, PNULL);
            MMK_CloseWin(win_id);
#endif
            break;
        case ID_ENG_CHIP_TEST_SETTING:
            MMK_CreateWin((uint32*)MMIENG_CHIP_TEST_SETTING_WIN_TAB, PNULL);
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
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

#ifdef CMMB_SUPPORT
/*****************************************************************************/
//  Description : cmmb test main win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCMMBTestWaitWinMsg(MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MBBMS_SERVICE_RESULT_E  play_result = MTVSERVICE_RST_RET_OK;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_ENG_MTV_PLAY_CNF:
        //SCI_TRACE_LOW:"HandleCMMBTestWaitWinMsg msg MSG_ENG_MTV_PLAY_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_10403_112_2_18_2_18_2_178,(uint8*)"");
        MMK_CloseWin(MMIENG_CMMB_WAIT_WIN_ID);
        
        if(MTVSERVICE_RST_RET_OK != MMIAPIENG_GetPlayResult())
        {
            //play failed!
            MMIPUB_OpenAlertFailWin(TXT_MTV_PLAY_NETWORK_ERROR_NEED_RETRY);
        }
        else
        {
            //play ok, display channel info
            MMIAPIENG_CreateCMMBChannelStatusWin();
        }
        break;
        
    case MSG_ENG_MTV_SEARCH_CNF:
        //SCI_TRACE_LOW:"HandleCMMBTestWaitWinMsg msg MSG_ENG_MTV_SEARCH_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_10419_112_2_18_2_18_2_179,(uint8*)"");
        MMK_CloseWin(MMIENG_CMMB_WAIT_WIN_ID);
        
        //RF test, display rssi only
        MMIAPIENG_CreateCMMBChannelStatusWin();
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : cmmb test main win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCMMBTestMainMenuWinMsg(
                                                MMI_WIN_ID_T     win_id,    
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM              param
                                                )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T   menu_id = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_TurnOnBackLight(); 
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
#ifdef UI_P3D_SUPPORT
        MMI_Disable3DEffect(MMI_3D_EFFECT_CMMB_ENGINE);
#endif 

        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(MMIENG_CMMB_TEST_MENU_WIN_ID, MMIENG_CMMB_TEST_MENU_CTRL_ID);
        MMITHEME_UpdateRect();

        //init CMMB
        if(!MMIAPIMTV_IsMTVOpen())
        {
            //当前已开启MTV的情况下，不需再做初始
            MMIAPIENG_InitMTV();
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUIMENU_GetId(MMIENG_CMMB_TEST_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch ( menu_id )
        {
        case ID_ENG_CMMB_FACTORY_TEST:
            //工厂测试
            MMK_CreateWin((uint32*)MMIENG_CMMB_FACTORY_TEST_WIN_TAB, PNULL);
            break;

        case ID_ENG_CMMB_RD_TEST:
            //研发调试
            MMK_CreateWin((uint32*)MMIENG_CMMB_RD_TEST_WIN_TAB, PNULL);
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
        if(!MMIAPIMTV_IsMTVOpen())
        {
            MMIAPIENG_ReleaseMTV();
        }
        
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_CMMB_ENGINE);
#endif         
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : cmmb RD test win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCMMBRDTestMenuWinMsg(
                                                MMI_WIN_ID_T     win_id,    
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM              param
                                                )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T   menu_id = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(MMIENG_CMMB_RD_TEST_MENU_WIN_ID, MMIENG_CMMB_RD_TEST_MENU_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUIMENU_GetId(MMIENG_CMMB_RD_TEST_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch ( menu_id )
        {
        case ID_ENG_CMMB_DISPLAY:
            //Display
            MMIAPIENG_CreateCMMBChannelStatusWin();
            break;
#ifdef MBBMS_SUPPORT
           
        case ID_MBBMS_SMD_COUNTER:
            MMK_CreateWin(MMIENG_SMD_COUNTER_ENABLE_WIN_TAB,PNULL);
            break;
            
        case ID_MBBMS_AUDIO:
            MMK_CreateWin(MMIENG_AUDIO_SERVICE_ENABLE_WIN_TAB,PNULL);
            break;
#ifdef DEMOD_HW_SIANO
        case ID_MBBMS_LOG_CONTROL:
           MMK_CreateWin(MMIENG_LOG_CONTROL_ENABLE_WIN_TAB,PNULL);
#endif
           break;                    
#endif
        case ID_MBBMS_INTERFACE_TEST:
            MMK_CreateWin(MMIENG_INTERFACE_TEST_ENABLE_WIN_TAB,PNULL);
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
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : cmmb Factory test win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCMMBFactoryTestMenuWinMsg(
                                                   MMI_WIN_ID_T     win_id,     
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM              param
                                                   )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T   menu_id = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(MMIENG_CMMB_FACTORY_TEST_MENU_WIN_ID, MMIENG_CMMB_FACTORY_TEST_MENU_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUIMENU_GetId(MMIENG_CMMB_FACTORY_TEST_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch ( menu_id )
        {
        case ID_ENG_CMMB_FACTORY_HAVE_SIGNAL:
            //有信号测试
            MMK_CreateWin((uint32*)MMIENG_CMMB_OK_SIGNAL_WIN_TAB, PNULL);
            break;
            
        case ID_ENG_CMMB_FACTORY_NO_SIGNAL:
            //无信号测试
            MMK_CreateWin((uint32*)MMIENG_CMMB_NO_SIGNAL_WIN_TAB, PNULL);
            break;
            
        case ID_ENG_CMMB_SN:
            //CMMB serial number
            MMK_CreateWin((uint32*)MMIENG_CMMB_SN_WIN_TAB, PNULL);
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
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : CMMB serial number win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCMMBSerialNumberWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MBBMS_SERVICE_RET_E     err_code = 0;
    uint8                   str_info[MMIENG_CMMB_CHANNEL_INFO_MAX_LEN +1] = {0};
    wchar                   wstr_info[MMIENG_CMMB_CHANNEL_INFO_MAX_LEN +1] ={0};
    uint8                   str_len = 0;
    wchar                   wstr_len = 0;
    MBBMS_SERVICE_RET_E     ret_code = MBBMS_SERVICE_FUNC_OK;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //get cmmb sn
        ret_code = MTVSERVICE_GetCmmbSn(MMIENG_CMMB_CHANNEL_INFO_MAX_LEN, str_info);
        
        if(MBBMS_SERVICE_FUNC_OK == ret_code)
        {
            //SCI_TRACE_LOW:"MTVSERVICE_GetCmmbSn ok SN:%s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_10677_112_2_18_2_18_3_180,(uint8*)"s", str_info);
            MMIAPICOM_StrToWstr((char*)str_info, wstr_info);
        }
        else
        {
            sprintf((char*)str_info, "%s", "Error!");
            MMIAPICOM_StrToWstr((char*)str_info, wstr_info);
        }
        
        wstr_len = MMIAPICOM_Wstrlen(wstr_info);
        GUITEXT_SetString(MMIENG_CMMB_SN_TEXT_CTRL_ID, wstr_info, wstr_len, FALSE);
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
//  Description : cmmb Factory test ok signal win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCMMBTestOKSignalMenuWinMsg(
                                                    MMI_WIN_ID_T     win_id,    
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM              param
                                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T    wait_text = {0};
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T   menu_id = 0;
    MMIENG_CMMB_INI_FILE_INFO_T ini_file_info ={0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(MMIENG_CMMB_HAVE_SIGNAL_MENU_WIN_ID, MMIENG_CMMB_FACTORY_HAVE_SIGNAL_MENU_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUIMENU_GetId(MMIENG_CMMB_FACTORY_HAVE_SIGNAL_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch ( menu_id )
        {
        case ID_ENG_CMMB_PLAY:
            //工厂测试 -> 播放
            SCI_MEMSET(&ini_file_info, 0, sizeof(ini_file_info));
            if(!MMIAPIMTV_IsMTVOpen())
            {
                //直播
                SCI_MEMSET(&ini_file_info, 0, sizeof(ini_file_info));
                if(CmmbTest_ParseIni(&ini_file_info))
                {
                    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
                    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIENG_CMMB_WAIT_WIN_ID,IMAGE_NULL,
                        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleCMMBTestWaitWinMsg);
                    
                    MMIAPIENG_PlayMTVService(ini_file_info.network_id, ini_file_info.service_id);
                }
                else
                {
                    //提示配置文件错误
                    MMIPUB_OpenAlertFailWin(TXT_ENG_CMMB_INI_FILE_ERROR);
                }
            }
            else
            {
                //mobiletv application is already opened, need stop first
                MMIPUB_OpenAlertFailWin(TXT_EXIT_MOBILETV);
            }
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
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : cmmb Factory test no signal win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCMMBTestNoSignalMenuWinMsg(
                                                    MMI_WIN_ID_T     win_id,    
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM              param
                                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T    wait_text = {0};
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T   menu_id = 0;
    MMIENG_CMMB_INI_FILE_INFO_T ini_file_info ={0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(MMIENG_CMMB_NO_SIGNAL_MENU_WIN_ID, MMIENG_CMMB_FACTORY_NO_SIGNAL_MENU_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUIMENU_GetId(MMIENG_CMMB_FACTORY_NO_SIGNAL_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch ( menu_id )
        {
        case ID_ENG_CMMB_RF_TEST:
            //RF测试
            if(!MMIAPIMTV_IsMTVOpen())
            {
                SCI_MEMSET(&ini_file_info, 0, sizeof(ini_file_info));
                if(CmmbTest_ParseIni(&ini_file_info))
                {
                    if(MMK_IsOpenWin(MAIN_WAIT_WIN_ID))
                    {
                        MMK_CloseWin(MAIN_WAIT_WIN_ID);
                    }
                    
                    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
                    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIENG_CMMB_WAIT_WIN_ID,IMAGE_NULL,
                        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleCMMBTestWaitWinMsg);
                    MMITHEME_UpdateRect();
                    
                    MMIAPIENG_MTVRFTest(ini_file_info.rf_test_freq);
                }
                else
                {
                    //提示配置文件错误
                    MMIPUB_OpenAlertFailWin(TXT_ENG_CMMB_INI_FILE_ERROR);
                }
            }
            else
            {
                //mobiletv application is already opened, need stop first
                MMIPUB_OpenAlertFailWin(TXT_EXIT_MOBILETV);
            }
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
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : display CMMB channel status info
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void DisplayChannelStatusInfo(MTV_CHANNEL_STATUS_T* channel_info_ptr)
{
    MMI_WIN_ID_T    win_id = MMIENG_CMMB_DISPLAY_WIN_ID;
    MMI_CTRL_ID_T   ctrl_id = MMIENG_CMMB_TEXT_CTRL_ID;
    GUI_FONT_T      font = MMI_DEFAULT_NORMAL_FONT;
    char  buffer[MMIENG_CMMB_CHANNEL_INFO_MAX_LEN*7] = {0};
    wchar wstr_buffer[MMIENG_CMMB_CHANNEL_INFO_MAX_LEN*7] = {0};
    int32 length = 0;
    int32 offset = 0;
    uint8 str_info[MMIENG_CMMB_CHANNEL_INFO_MAX_LEN +1] = {0};
    wchar wstr_info[MMIENG_CMMB_CHANNEL_INFO_MAX_LEN +1] ={0};
    
    //SCI_TRACE_LOW:"[CMMB ENG]: CMMB Channel info Heartbeat:%d  Block count:%d  Error block count:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_10884_112_2_18_2_18_3_181,(uint8*)"ddd", channel_info_ptr->heartbeat, channel_info_ptr->block_count, channel_info_ptr->error_block_count);
    
    //SCI_TRACE_LOW:"[CMMB ENG]: CMMB Channel info Frequency offset:%d  RSSI:%d  Padding:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_10887_112_2_18_2_18_3_182,(uint8*)"ddd", channel_info_ptr->frequency_offset, channel_info_ptr->rssi, channel_info_ptr->padding);
    
    
    //正常播放显示全部的Channel信息
    if(MMIENG_MTV_CHANNEL_RF_TEST != MMIAPIENG_GetMTVChannelDisplayMode())
    {
        //heartbeat
        //sprintf((char*)str_info, "%s %d\n", "Heartbeat: ", channel_info_ptr->heartbeat);
        //length = SCI_STRLEN(str_info);
        //SCI_MEMCPY(buffer + offset, str_info, length);
        //offset += length ;
        
        //block_count
        //SCI_MEMSET(str_info, 0, sizeof(str_info));       
        //sprintf((char*)str_info, "%s %d\n", "Block count: ", channel_info_ptr->block_count);
        //length = SCI_STRLEN(str_info);
        //SCI_MEMCPY(buffer + offset, str_info, length);        
        //offset += length ;
        
        //error_block_count
        //SCI_MEMSET(str_info, 0, sizeof(str_info));     
        //sprintf((char*)str_info, "%s %d\n", "Error block count: ", channel_info_ptr->error_block_count);
        //length = SCI_STRLEN(str_info);
        //SCI_MEMCPY(buffer + offset, str_info, length);           
        //offset += length ;
        
        //frequency_offset
        //SCI_MEMSET(str_info, 0, sizeof(str_info));        
        sprintf((char*)str_info, "%s %d\n", "Frequency offset: ", channel_info_ptr->frequency_offset);
        length = SCI_STRLEN(str_info);
        SCI_MEMCPY(buffer + offset, str_info, length);    
        offset += length ;
        
        //rssi
        SCI_MEMSET(str_info, 0, sizeof(str_info));      
        sprintf((char*)str_info, "%s %d\n", "RSSI: ", channel_info_ptr->rssi);
        length = SCI_STRLEN(str_info);
        SCI_MEMCPY(buffer + offset, str_info, length);    
        offset += length ; 
        
        //padding
        //SCI_MEMSET(str_info, 0, sizeof(str_info));
        //sprintf((char*)str_info, "%s %d\n", "Padding: ", channel_info_ptr->padding);
        //length = SCI_STRLEN(str_info);
        //SCI_MEMCPY(buffer + offset, str_info, length);    
        //offset += length ;
        
        //sync_stat
        SCI_MEMSET(str_info, 0, sizeof(str_info));
        sprintf((char*)str_info, "%s %d\n", "sync_stat: ", channel_info_ptr->sync_stat);
        length = SCI_STRLEN(str_info);
        SCI_MEMCPY(buffer + offset, str_info, length);    
        offset += length ;
        
        //freqency
        SCI_MEMSET(str_info, 0, sizeof(str_info));
        sprintf((char*)str_info, "%s %d\n", "freqency: ", channel_info_ptr->freqency);
        length = SCI_STRLEN(str_info);
        SCI_MEMCPY(buffer + offset, str_info, length);    
        offset += length ;
        
        //BER
        SCI_MEMSET(str_info, 0, sizeof(str_info));
        sprintf((char*)str_info, "%s %d\n", "BER: ", channel_info_ptr->BER);
        length = SCI_STRLEN(str_info);
        SCI_MEMCPY(buffer + offset, str_info, length);    
        offset += length ;
        
        //SNR
        SCI_MEMSET(str_info, 0, sizeof(str_info));
        sprintf((char*)str_info, "%s %d\n", "SNR: ", channel_info_ptr->SNR);
        length = SCI_STRLEN(str_info);
        SCI_MEMCPY(buffer + offset, str_info, length);    
        offset += length ;
        
#if 0
        SCI_MEMSET(str_info, 0, sizeof(str_info));       
        
        /* 比较a/b 和c/d的大小，等同于比较a*d 和c * b的大小*/
        if (channel_info_ptr->rssi >= s_config_channel_info.rssi_min
            && channel_info_ptr->rssi <= s_config_channel_info.rssi_max
            && channel_info_ptr->error_block_count * s_config_channel_info.bler_num_blk < s_config_channel_info.bler_err_blk * channel_info_ptr->block_count)
        {
            sprintf(str_info, "%s", "pass!");
        }
        else
        {
            sprintf(str_info, "%s", "fail!");
        }        
        
        length = SCI_STRLEN(str_info);
        SCI_MEMCPY(buffer + offset, str_info, length);    
        offset += length ;
#endif
    }
    else
    {
        //RF测试仅显示RSSI信息
        //rssi
        SCI_MEMSET(str_info, 0, sizeof(str_info));       
        sprintf((char*)str_info, "%s %d", "RSSI:", channel_info_ptr->rssi);
        length = SCI_STRLEN(str_info);
        SCI_MEMCPY(buffer + offset, str_info, length);    
        offset += length ;     
    }
    
    MMI_STRNTOWSTR(wstr_buffer, (MMIENG_CMMB_CHANNEL_INFO_MAX_LEN*7), buffer,  offset, offset);
    GUITEXT_SetString(ctrl_id, wstr_buffer, offset, TRUE);
    GUITEXT_SetFont(ctrl_id, &font, PNULL);
    MMK_SetAtvCtrl(win_id, ctrl_id);
    return;
}

/*****************************************************************************/
//  Description : cmmb display win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCMMBDisplayWinMsg(
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MTV_CHANNEL_STATUS_T    channel_info = {0};
    MBBMS_SERVICE_RET_E     err_code = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //SCI_TRACE_LOW:"[MMIMTV]: HandleCMMBDisplayWinMsg MSG_OPEN_WINDOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_11016_112_2_18_2_18_4_183,(uint8*)"");
        err_code = MTVSERVICE_GetChannelStatus(&channel_info);
        //SCI_TRACE_LOW:"[MMIMTV]: MTVSERVICE_GetChannelStatus, return = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_11018_112_2_18_2_18_4_184,(uint8*)"d", err_code);
        DisplayChannelStatusInfo(&channel_info);
        
        //启动timer周期获取CHANNEL数据
        s_cmmb_timer = MMK_CreateWinTimer(win_id, 1000, FALSE);
        break;
        
    case MSG_TIMER:
        if(*(uint8*)param == s_cmmb_timer)
        {
            err_code = MTVSERVICE_GetChannelStatus(&channel_info);
            //SCI_TRACE_LOW:"[MMIMTV]: MTVSERVICE_GetChannelStatus, return = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_11029_112_2_18_2_18_4_185,(uint8*)"d", err_code);
            DisplayChannelStatusInfo(&channel_info);
            s_cmmb_timer = MMK_CreateWinTimer(win_id, 1000, FALSE);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //SCI_TRACE_LOW:"[MMIMTV]: HandleCMMBDisplayWinMsg MSG_APP_CANCEL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_11037_112_2_18_2_18_4_186,(uint8*)"");
        MMIAPIENG_StopMTVFlow();
        while(MMIMBBMS_FLOW_STATE_IDLE != MMIAPIENG_GetMTVFlowInfo()->flow_type)
        {
            SCI_Sleep(100);
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_cmmb_timer);
        s_cmmb_timer = 0;
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#ifdef MBBMS_SUPPORT

#ifdef DEMOD_HW_SIANO
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoxiang.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLOGControlEnableWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LOG_ENABLE_CONTROL_CTRL_ID;
    uint16 cur_selection = 0;
   
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);     
        
        if (s_mbbms_log_control_enable)
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;        
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (0 == cur_selection)
        {
            s_mbbms_log_control_enable = TRUE;
        }
        else
        {
            s_mbbms_log_control_enable = FALSE;
        }
        //set log control
        MTVSERVICE_SetLogOutput(s_mbbms_log_control_enable);            
        //设置NV项
       // MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoxiang.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInterfaceTestEnableWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_GBA_AND_MSK_TEST_CTRL_ID;
    uint16 cur_selection = 0;
   
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);     
        
        if (s_mbbms_interface_test_enable)
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;        
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (0 == cur_selection)
        {
            s_mbbms_interface_test_enable = TRUE;
        }
        else
        {
            s_mbbms_interface_test_enable = FALSE;
        }
        
        //设置NV项
        //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : HandleSMDCounterEnableWinMsg
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMDCounterEnableWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_SMD_COUNTER_ENABLE_CTRL_ID;
    uint16 cur_selection = 0;
   
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);     
        
        if (s_mbbms_smd_counter_enable)
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;        
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (0 == cur_selection)
        {
            s_mbbms_smd_counter_enable = TRUE;
        }
        else
        {
            s_mbbms_smd_counter_enable = FALSE;
        }
        
        //设置NV项
        //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : is smd counter enable
//  Global resource dependence : 
//  Author: lisa.lin
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsSMDCounterEnable(void)
{
    return s_mbbms_smd_counter_enable;
}

/*****************************************************************************/
// 	Description : is interface testing eable
//	Global resource dependence : 
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsInterfaceTestingEnable(void)
{
    return s_mbbms_interface_test_enable;
}

#endif

/*****************************************************************************/
//  Description : HandleSMDCounterEnableWinMsg
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAudioServiceEnableWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_AUDIO_SERVICE_ENABLE_CTRL_ID;
    uint16 cur_selection = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
   
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);     
        
        //read nv
        MMINV_READ(MMIENG_NV_MBBMS_AUDIO_ID, &s_mbbms_audio_enable, return_value);
        if(MN_RETURN_SUCCESS != return_value)
        {
            s_mbbms_audio_enable = FALSE;
            MMINV_WRITE(MMIENG_NV_MBBMS_AUDIO_ID, &s_mbbms_audio_enable);
        }

        if (s_mbbms_audio_enable)
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;        
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (0 == cur_selection)
        {
            s_mbbms_audio_enable = TRUE;
        }
        else
        {
            s_mbbms_audio_enable = FALSE;
        }
        
        //设置NV项
        MMINV_WRITE(MMIENG_NV_MBBMS_AUDIO_ID, &s_mbbms_audio_enable);
        //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : is smd counter enable
//  Global resource dependence : 
//  Author: lisa.lin
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsAudioServiceEnable(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    //read nv
    MMINV_READ(MMIENG_NV_MBBMS_AUDIO_ID, &s_mbbms_audio_enable, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        s_mbbms_audio_enable = FALSE;
        MMINV_WRITE(MMIENG_NV_MBBMS_AUDIO_ID, &s_mbbms_audio_enable);
    }
    
    return s_mbbms_audio_enable;
}

/********************************************************************************
 NAME:         EngMtvLOGSwitchWinHandleMsg
 DESCRIPTION:
 PARAM IN:     win_id - 
               msg_id - 
               param - 
 PARAM OUT:        
 AUTHOR:       Bill Ji
 DATE:         20104.04.09
********************************************************************************/
LOCAL MMI_RESULT_E EngMtvLOGSwitchWinHandleMsg(MMI_WIN_ID_T      win_id,
                                               MMI_MESSAGE_ID_E  msg_id,
                                               DPARAM            param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    BOOLEAN         is_on = FALSE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );
        is_on = MTV_GetVLogFlag();
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);         
        GUILIST_SetSelectedItem(ctrl_id, is_on, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,is_on);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_FULL_PAINT:
        break;
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        is_on = GUILIST_GetCurItemIndex(ctrl_id);       
        MTV_SetVLogFlag(is_on);
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);       
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:         EngMtvLOGToBBSwitchWinHandleMsg
 DESCRIPTION:
 PARAM IN:     win_id - 
               msg_id - 
               param - 
 PARAM OUT:        
 AUTHOR:       Bill Ji
 DATE:         20104.04.09
********************************************************************************/
LOCAL MMI_RESULT_E EngMtvLOGToBBSwitchWinHandleMsg(MMI_WIN_ID_T      win_id,
                                                   MMI_MESSAGE_ID_E  msg_id,
                                                   DPARAM            param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    BOOLEAN         is_on = FALSE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );
        is_on = MTV_GetVLogToBBFlag();
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);         
        GUILIST_SetSelectedItem(ctrl_id, is_on, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,is_on);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_FULL_PAINT:
        break;
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        is_on = GUILIST_GetCurItemIndex(ctrl_id);       
        MTV_SetVLogToBBFlag(is_on);
       // MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);       
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:         EngMtvLOGStorageWinHandleMsg
 DESCRIPTION:
 PARAM IN:     win_id - 
               msg_id - 
               param - 
 PARAM OUT:        
 AUTHOR:       Bill Ji
 DATE:         20104.04.09
********************************************************************************/
LOCAL MMI_RESULT_E EngMtvLOGStorageWinHandleMsg(MMI_WIN_ID_T      win_id,
                                                MMI_MESSAGE_ID_E  msg_id,
                                                DPARAM            param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    BOOLEAN         is_on = FALSE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );
        is_on = MTV_GetVLogStorage();
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_MTV_LOG_STORAGE_U,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_MTV_LOG_STORAGE_SD,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);         
        GUILIST_SetSelectedItem(ctrl_id, is_on, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,is_on);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_FULL_PAINT:
        break;
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        is_on = GUILIST_GetCurItemIndex(ctrl_id);
        MTV_SetVLogStorage(is_on);
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);       
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#endif

/*****************************************************************************/
//  Description : chip test setting
//  Global resource dependence : 
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChipTestSettingWinMsg(
                                       MMI_WIN_ID_T     win_id,     
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    //MMI_MENU_GROUP_ID_T group_id = 0;
    uint16 index = 0;

    //SCI_TRACE_LOW:"HandleChipTestSettingWinMsg(), msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_11577_112_2_18_2_18_5_187,(uint8*)"d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIENG_CHIP_TEST_SETTING_CTRL_ID, 2, FALSE );//max item 2
        GUILIST_SetOwnSofterKey(MMIENG_CHIP_TEST_SETTING_CTRL_ID,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,MMIENG_CHIP_TEST_SETTING_CTRL_ID,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,MMIENG_CHIP_TEST_SETTING_CTRL_ID,GUIITEM_STYLE_ONE_LINE_RADIO);  

        if (MMIAPIENG_GetChipTestMode() == MMIENG_CHIP_TEST_SETTING_ON)
        {
            index = 0;
        }
        else
        {
            index = 1;
        }
        //set selected item
        GUILIST_SetSelectedItem(MMIENG_CHIP_TEST_SETTING_CTRL_ID, index, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(MMIENG_CHIP_TEST_SETTING_CTRL_ID, index);   
        MMK_SetAtvCtrl(win_id, MMIENG_CHIP_TEST_SETTING_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:  
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        index = GUILIST_GetCurItemIndex(MMIENG_CHIP_TEST_SETTING_CTRL_ID);
        switch ( index )
        {
        case 0:
            MMIAPIENG_SetChipTestMode(MMIENG_CHIP_TEST_SETTING_ON);
           // MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            MMK_CloseWin(win_id);
            break;
        case 1:
            MMIAPIENG_SetChipTestMode(MMIENG_CHIP_TEST_SETTING_OFF);
            //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            MMK_CloseWin(win_id);
            break;
            
        default:
            //SCI_TRACE_LOW:"MMIENG_WIN.C HandleChipTestSettingWinMsg index is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_11620_112_2_18_2_18_5_188,(uint8*)"d",index);
            break;
        }

        break;

    case MSG_APP_CANCEL:
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

#ifdef TF_LOAD_SUPPORT
/*****************************************************************************/
//  Description : 	find update file in TF CARD
//  Global resource dependence : 
//  Author: tao.feng
//  Note:
/*****************************************************************************/

LOCAL BOOLEAN MMIAPIENG_GetDeviceStatus(void)
{
	uint32 i = 0;
	for(i=0; i<2; i++)
	{
		if (MMIAPIFMM_GetDeviceStatus((wchar *)g_device_sdcard[i], MMIFILE_DEVICE_SDCARD_LEN))
		{
			slot_exist[i] = TRUE; 			
		}
	}

	if(slot_exist[0] || slot_exist[1])
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

LOCAL int tf_strncmp(char * str1, char * str2, int length)
{
   while (length--)
   {
      if (*str1 > *str2)
         return 1;
      if (*str1++ < *str2++)
         return -1;
   }
   return 0;
}

LOCAL char *tf_strstr(char * str1, char * str2, uint32 len)
{
   uint32 i = 0x0;

   while (i<len)
   {
      if (*str1 == *str2)  /* found first char of str2 in str1 */
      {
         if (tf_strncmp(str1, str2, strlen(str2)) == 0)
         {
            return str1;   /* found str2 at str1 */
         }
      }
      str1++;
      i++;
   }
   return (char*)0;
}

LOCAL MMIENG_TF_FILE_E MMIAPIENG_FindUpdateFile(void)
{
	uint16  full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
	uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
	uint32 i = 0;
	uint32  file_len = 0, nPacketSize = 0;	
	BIN_PACKET_HEADER_T bph;	
   	uint8   *data_ptr = PNULL, *tmp_ptr = PNULL;

	g_valid_sdcard = NULL;
	
	for(i=0; i<2; i++)
	{
		if(!slot_exist[i])
		{
			continue;
		}

		if (MMIAPIFMM_CombineFullPath((wchar *)g_device_sdcard[i],
		                        MMIFILE_DEVICE_SDCARD_LEN,
		                        TFLOAD_DEFAULT_DIR,
		                        TFLOAD_DEFAULT_DIR_DIR_LEN,
		                        NULL,
		                        0,
		                        full_path_name,
		                        &full_path_len))

		{
		    if(MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
		    {
		    	g_valid_sdcard = (wchar *)g_device_sdcard[i];
				break;
		    }
		}
	}

	if(!g_valid_sdcard)
	{
		return MMIENG_DIR_NO_EXITS;
	}

	SCI_MEMSET(full_path_name, 0, sizeof(full_path_name));
	full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
	if (MMIAPIFMM_CombineFullPath(g_valid_sdcard,
	                        MMIFILE_DEVICE_SDCARD_LEN,
	                        TFLOAD_DEFAULT_DIR,
	                        TFLOAD_DEFAULT_DIR_DIR_LEN,
	                        TFLOAD_DEFAULT_FILENAME,
	                        TFLOAD_DEFAULT_FILELEN,
	                        full_path_name,
	                        &full_path_len))
	{

		if(!MMIAPIFMM_IsFileExist(full_path_name, full_path_len))
		{
			return MMIENG_PAC_NO_EXITS;
		}

		MMIAPIFMM_GetFileInfo(full_path_name,  full_path_len,  &file_len, NULL, NULL);
		//SCI_TRACE_LOW:"MMIAPIENG Read Pac file: file_len=0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_11759_112_2_18_2_18_5_189,(uint8*)"d", file_len);
		if(file_len == 0x0)
		{
		   return  MMIENG_PAC_ERROR;
		}

		nPacketSize = file_len;        
		//check packet size, it must large than the header struct
		if(nPacketSize < sizeof(BIN_PACKET_HEADER_T))
		{
			return MMIENG_PAC_ERROR;
		}
			
		if(MMIAPIFMM_ReadFilesDataSyn(full_path_name, full_path_len, (uint8 *)&bph, sizeof(BIN_PACKET_HEADER_T)))
		{
			// check packet product name we must swap the little endian to bigendian
			for(i = 0; i < 256; i++ )
			{
				bph.szPrdName[i] = TF_WORDSWAP(bph.szPrdName[i]);
			}

			if(TF_LoadWstrCMP(bph.szPrdName, TFLOAD_PRODUCT_NAME)!=0 )
			{
				return MMIENG_PAC_ERROR;
			}
		}
		else
		{
			return MMIENG_PAC_ERROR;
		}	   		
	}

	SCI_MEMSET(full_path_name, 0, sizeof(full_path_name));
	full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
	if (MMIAPIFMM_CombineFullPath(g_valid_sdcard,
	                        MMIFILE_DEVICE_SDCARD_LEN,
	                        TFLOAD_DEFAULT_DIR,
	                        TFLOAD_DEFAULT_DIR_DIR_LEN,
	                        TFLOAD_DEFAULT_BOOTNAME,
	                        TFLOAD_DEFAULT_BOOTLEN,
	                        full_path_name,
	                        &full_path_len))
	{

		if(!MMIAPIFMM_IsFileExist(full_path_name, full_path_len))
		{
			return MMIENG_FILE_NO_EXITS;
		}

        MMIAPIFMM_GetFileInfo(full_path_name,  full_path_len,  &file_len, NULL, NULL);
		//SCI_TRACE_LOW:"MMIAPIENG Read Pac file: file_len=0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_11809_112_2_18_2_18_5_190,(uint8*)"d", file_len);
		if(file_len == 0x0)
		{
		   return  MMIENG_FILE_ERROR;
		}

        data_ptr = (uint8*)SCI_ALLOCA(MMIFILE_PARSE_SIZE);
        if(!data_ptr)
        {
           return  MMIENG_FILE_ERROR;
        }
           
		if(MMIAPIFMM_ReadFilesDataSyn(full_path_name, full_path_len, data_ptr, MMIFILE_PARSE_SIZE))
		{
            tmp_ptr = (uint8 *)tf_strstr((char *)data_ptr, "Project Version:", MMIFILE_PARSE_SIZE);

            if(!tmp_ptr)
            {
                SCI_FREE(data_ptr);
                return  MMIENG_FILE_ERROR;
            }

            if(0 != strcmp((char *)tmp_ptr, s_version_info[PROJECT_VERSION]))
			{
			    SCI_FREE(data_ptr);
				return MMIENG_FILE_ERROR;
			}
                        
            SCI_FREE(data_ptr);
		}
		else
		{
		    SCI_FREE(data_ptr);
			return MMIENG_FILE_ERROR;
		}	
	}

	return MMIENG_ALL_EXIST;
}


/*****************************************************************************/
//  Description : 	read tf boot file 
//  Global resource dependence : 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPIENG_ReadBootFile(void)
{
	uint16  full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
	uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
	uint32  file_len;
	uint8   *data_ptr =PNULL;

	SCI_MEMSET(full_path_name, 0, sizeof(full_path_name));
	full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
	if (MMIAPIFMM_CombineFullPath(g_valid_sdcard,
	                        MMIFILE_DEVICE_SDCARD_LEN,
	                        TFLOAD_DEFAULT_DIR,
	                        TFLOAD_DEFAULT_DIR_DIR_LEN,
	                        TFLOAD_DEFAULT_BOOTNAME,
	                        TFLOAD_DEFAULT_BOOTLEN,
	                        full_path_name,
	                        &full_path_len))
	{
		   MMIAPIFMM_GetFileInfo(full_path_name,  full_path_len,  &file_len, NULL, NULL);
		   //SCI_TRACE_LOW:"MMIAPIENG_ReadBootFile: file_len=0x%x"
		   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_11873_112_2_18_2_18_5_191,(uint8*)"d", file_len);
		   if(file_len == 0x0)
		   {
			    return  SCI_FALSE;
		   }
		   data_ptr = (uint8*)SCI_ALLOCA(file_len);
           if(!data_ptr)
           {
               return  SCI_FALSE;
           }

		   if(MMIAPIFMM_ReadFilesDataSyn(full_path_name, full_path_len, data_ptr, file_len))
		   {
		   		s_tf_load.file_addr = data_ptr;
				s_tf_load.file_len   = file_len;
				return  SCI_TRUE;
		   }
		   else
		   {
		   		SCI_FREE(data_ptr);
				return  SCI_FALSE;
		   }
	}

	return  SCI_FALSE;
}



/*****************************************************************************/
//  Description : 	copy boot  file to sram
//  Global resource dependence : 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void MMIAPIENG_CopyBootFile(void)
{
        uint32 i,len;
	uint8 *srcaddr;
	uint8 *dscaddr;
	uint32  tf_exec_addr =TF_GetBootExecAddress();
	//SCI_TRACE_LOW:"MMIAPIENG_CopyBootFile: tf_exec_addr=0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_11912_112_2_18_2_18_5_192,(uint8*)"d", tf_exec_addr);
	//SCI_ASSERT((tf_exec_addr< s_tf_load.file_addr) || (tf_exec_addr>  (s_tf_load.file_addr + s_tf_load.file_len));
	//SCI_MEMCPY((uint8*)tf_exec_addr,  (uint8*)s_tf_load.file_addr,  s_tf_load.file_len);
	#if 1
	srcaddr =  s_tf_load.file_addr;
	dscaddr =  tf_exec_addr; /*lint !e64*/
	len = s_tf_load.file_len;
	for(i=0;i<len;i++)
	{
           *dscaddr= *srcaddr;
	     dscaddr++;	
	     srcaddr++;	
	}
	#endif
}


/*****************************************************************************/
//  Description : 	Handle low battery condition before TF upgrading
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTFUpWarnWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param   
                                         )
{
	LOCAL uint8 tf_timer_id = 0;
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
    CHGMNG_STATE_INFO_T* p_chgmng_info;

	//SCI_TRACE_LOW:"HandleTFUpWarnWindow: msg_id=0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_11943_112_2_18_2_18_5_193,(uint8*)"d", msg_id);
	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:
			recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
			MMK_PostMsg(MMI_TF_UPGRADING_WARNING_ID, MSG_ENG_UPGRADING_WARN, PNULL,0);
			break;

	       case MSG_ENG_UPGRADING_WARN:
            p_chgmng_info = CHGMNG_GetModuleState();
		   	if(((p_chgmng_info->bat_cur_vol < TF_UPGRADING_VBAT_MIN) && !CHGMNG_IsChargeConnect())\
				|| !MMIAPIENG_GetDeviceStatus()\
					|| (MMIAPIENG_FindUpdateFile() != MMIENG_ALL_EXIST))
		   	{
					tf_timer_id = MMK_CreateTimer(MMI_1SECONDS * 5, TRUE); 
			}
			else
			{
				MMK_PostMsg(MMIENG_TFLOAD_WIN_ID, MSG_ENG_UPGRADING_ALLOW, PNULL,0);
				MMK_CloseWin(MMI_TF_UPGRADING_WARNING_ID); 
			}
			break;

		 case MSG_APP_RED:
		 case MSG_CTL_OK:
		 case MSG_APP_OK:
			break;

		 case MSG_TIMER:	
			if(tf_timer_id == *((uint8 *) param))
			{
				if(0 != tf_timer_id)
				{
					MMK_StopTimer(tf_timer_id);
					tf_timer_id = 0;
					MMK_CloseWin(MMI_TF_UPGRADING_WARNING_ID); 
				}
			}
			else
			{
			    recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
			}
			break;
			
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
			MMK_CloseWin(MMI_TF_UPGRADING_WARNING_ID); 
			break;

		case MSG_CLOSE_WINDOW:
			if(0 != tf_timer_id)
			{
				MMK_StopTimer(tf_timer_id);
				tf_timer_id = 0;
			}
			break;
		    
		default:
			recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
			break;
	}

	return recode;  
}


/*****************************************************************************/
// 	Description : TF LOAD handler
//	Global resource dependence : 
//   Author: tao.feng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTFLoadWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T text1 = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_TEXT_ID_T content_id = TXT_ENG_TF_UPDATE_NOTIFICATION;
	BOOLEAN ret;
	MMIENG_TF_FILE_E TF_FILE_INFO = MMIENG_ALL_EXIST;
    CHGMNG_STATE_INFO_T* p_chgmng_info;

	//SCI_TRACE_LOW:"MMIENG_WIN HandleTFLoadWinMsg: msg_id=0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_12022_112_2_18_2_18_6_194,(uint8*)"d", msg_id);
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
	    	MMI_GetLabelTextByLang(content_id, &text1);
			GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text1.wstr_ptr, text1.wstr_len, FALSE);
			MMK_SetAtvCtrl(MMIENG_TFLOAD_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
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
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                 MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
	   	 case MSG_APP_OK:		
	   	 case MSG_CTL_OK:	
	   	 case MSG_APP_WEB:	
            p_chgmng_info = CHGMNG_GetModuleState();
			if((p_chgmng_info->bat_cur_vol < TF_UPGRADING_VBAT_MIN) && !CHGMNG_IsChargeConnect())
			{
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DIAL_CHARGE_PHONE_UPD,PNULL,
				IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, HandleTFUpWarnWindow);
			}
			else if (!MMIAPIENG_GetDeviceStatus())
			{
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EN_TF_UPDATE_WARNING_3,TXT_NULL,
				IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, HandleTFUpWarnWindow);				
			}
			else if((TF_FILE_INFO =MMIAPIENG_FindUpdateFile()) != MMIENG_ALL_EXIST)
			{
				if(TF_FILE_INFO == MMIENG_PAC_ERROR)
				{
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EN_TF_UPDATE_WARNING_5,TXT_NULL, 
					IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, HandleTFUpWarnWindow);
				}

   				if(TF_FILE_INFO == MMIENG_FILE_ERROR)
				{
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EN_TF_UPDATE_WARNING_6,TXT_NULL,
					IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, HandleTFUpWarnWindow);
				}                

				#ifdef TF_LOAD_PROTECT
				if(TF_FILE_INFO == MMIENG_FILE_NO_EXITS)
				{   
				    MMIAPISET_InitAudioDevice(0);   //set to silent
					GPIO_SetLcdBackLight(SCI_FALSE);
					TF_LoadInit();
					TF_EnterInit();
					//should never enter here
					while(1) {} /*lint !e716*/
				}
				#else
				if(TF_FILE_INFO == MMIENG_FILE_NO_EXITS)
				{
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EN_TF_UPDATE_WARNING_7,TXT_NULL, 
						IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, HandleTFUpWarnWindow);
				}
				else
				#endif

				MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EN_TF_UPDATE_WARNING_4,TXT_NULL, 
				IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, HandleTFUpWarnWindow);
			}
			else
			{
				MMK_PostMsg(MMIENG_TFLOAD_WIN_ID, MSG_ENG_UPGRADING_ALLOW, PNULL,0);
			}
		 	break;
			
		 case MSG_ENG_UPGRADING_ALLOW:
		 	//SCI_TRACE_LOW:"file update file and jump to sram execute"
		 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_12105_112_2_18_2_18_6_195,(uint8*)"");
			ret = MMIAPIENG_ReadBootFile();
			if(ret)
			{
			    MMIAPISET_InitAudioDevice(0);   //set to silent
				GPIO_SetLcdBackLight(SCI_FALSE);
				TF_LoadInit();
				MMIAPIENG_CopyBootFile();
			//	SCI_TRACE_LOW("TF_LoadInit success");
				TF_Jump();
			       while(1) {} /*lint !e716*/
			}
			else
			{
				//SCI_TRACE_LOW:"read boot file  fail "
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_12119_112_2_18_2_18_6_196,(uint8*)"");
			}
		 	break;
			
		 case MSG_APP_CANCEL:	 
		 case MSG_CTL_CANCEL:
            //SCI_TRACE_LOW:"MMIENG_WIN HandleTFLoadWinMsg: MSG_KEYDOWN_CANCEL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_12125_112_2_18_2_18_6_197,(uint8*)"");
			MMK_CloseWin(win_id);
			break;


        case MSG_CLOSE_WINDOW:
            //SCI_TRACE_LOW:"MMIENG_WIN HandleTFLoadWinMsg: MSG_CLOSE_WINDOW"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_12131_112_2_18_2_18_6_198,(uint8*)"");
            break;
		
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}
#endif

//extern BOOLEAN MMIAPIENG_GetIsCardLogOn(void);
extern void MMIAPIENG_SetIsCardLogOn(
                                     BOOLEAN is_cardlog_on
                                     );
extern void MMIAPIENG_SetIsBrowserLogOn(BOOLEAN is_browser_log_on);

/*****************************************************************************/
//  Description : open cardlog win
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenCardLogWin(void)
{
/*,@cr239352 renwei add begin*/
    if(MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertFailWin( TXT_ENG_CARDLOG_OPEN_ERROR_UDISK);
    }
    else if(MMIAPIENG_IsUSBLOGON())
    {
        MMIPUB_OpenAlertFailWin( TXT_ENG_CARDLOG_OPEN_ERROR_USBLOG);
    }
#ifndef DPHONE_SUPPORT 
    else if (MMIAPISD_IsUsbLogOn())
    {
        //无法使用card log
        MMIPUB_OpenAlertFailWin( TXT_ENG_CARDLOG_OPEN_ERROR_SDUSBLOG);
    }
#endif 
    else
    {
        MMK_CreateWin((uint32*)MMIENG_CARDLOG_WIN_TAB, PNULL);
    }
/*,@cr239352 renwei add end*/
}

/*****************************************************************************/
//  Description : open NPS timer change window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
#ifdef NPS_SUPPORT
PUBLIC void MMIAPIENG_OpenNPSTimerChangeWin(void)
{//[MS]
    MMK_CreateWin((uint32*)MMIENG_NPS_TIMER_CHANGE_WIN_TAB, PNULL);
}
#endif
/*****************************************************************************/
//  Description : open IMS_NV_PARAM_ID change window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenIMS_NV_PARAM_IDWin(void)
{//[MS]
    MMK_CreateWin((uint32*)MMIENG_IMS_NV_PARAM_ID_WIN_TAB, PNULL);
}

PUBLIC void MMIAPIENG_SetCardLog(BOOLEAN mode)
{
 	BOOLEAN is_cardlog_on = mode;
	 MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();
	MMINV_WRITE(MMIENG_NV_IS_CARDLOG_ON, &is_cardlog_on);
	SCI_TRACE_LOW("MMIAPIENG_SetCardLog:is_cardlog_on %d ",is_cardlog_on);

	if(TRUE == is_cardlog_on)
	{
		iq_mode_param.b_status = TRUE;
		#ifdef MMI_AUDIO_PLAYER_SUPPORT            
        		MMIAPIMP3_ResetAllMp3();
		#endif
		MMIAPISET_ResetDisplaySetting();
		#ifdef CARD_LOG
			#ifndef WIN32
			LogSaveOutput_SetReady();
			#endif
		#endif	
	}
 	else
 	{
		iq_mode_param.b_status = FALSE;	
		#ifdef CARD_LOG
			#ifndef WIN32
			LogSaveOutput_SetNotReady();
			#endif
		#endif 
	}	
	 MMIAPIENG_SetIQMode(&iq_mode_param);
}
/********************************************************************************
 NAME:          EngCardLogWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        bin.ji
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngCardLog_WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        //GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        
        if (MMIAPIENG_GetIsCardLogOn())
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;  
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            break;
        }
	SCI_TRACE_LOW("EngCardLog_WinHandleMsg cur_selection=%d ",cur_selection);
        if (0 == cur_selection) //开启
        {
            MMIAPIENG_SetCardLog(TRUE);
        }
        else
        {
 		MMIAPIENG_SetCardLog(FALSE);           
        }
	  MMK_CloseWin( win_id );
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngCardLogWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        bin.ji
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngCardLogWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        //GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        
        if (MMIAPIENG_GetIsCardLogOn())
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;  
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            break;
        }
        if (0 == cur_selection) //开启
        {
            MMIPUB_OpenQueryWinByTextId(TXT_ENG_CARDLOG_FORMAT_CONTINUE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }
        else
        {
            MMIAPISET_SetByOpenCloseList(ctrl_id, MMIAPISET_GetActiveSim(),MMIAPIENG_SetIsCardLogOn);
            //success!
            
            MMK_CloseWin( win_id );
        }
        break;
        
    case MSG_PROMPTWIN_OK:
#ifdef MMI_AUDIO_PLAYER_SUPPORT            
        MMIAPIMP3_ResetAllMp3();
#endif
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        MMIAPISET_SetByOpenCloseList(ctrl_id, MMIAPISET_GetActiveSim(),MMIAPIENG_SetIsCardLogOn);
        //success!
        
        MMK_CloseWin( win_id );
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:          EngNPSTimerChangeWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:       zhenxing
 DATE:          
********************************************************************************/
#ifdef NPS_SUPPORT

LOCAL MMI_RESULT_E EngNPSTimerChangeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T label_ctrl_id = MMIENG_NPS_TIME_LABEL_CTRL_ID;
	MMI_CTRL_ID_T  edit1_ctrl_id = MMIENG_NPS_TIME_EDIT1_CTRL_ID;
	MMI_CTRL_ID_T  edit2_ctrl_id = MMIENG_NPS_TIME_EDIT2_CTRL_ID;
	MMI_CTRL_ID_T  text_ctrl_id = MMIENG_NPS_TIME_TEXT_CTRL_ID;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_BOTH_RECT_T  both_rect = MMITHEME_GetWinClientBothRect(win_id); 
	GUI_RECT_T  both_client_rect = {0};
	GUI_RECT_T  nps_label_rect = {0};
	GUI_RECT_T  nps_edit1_rect = {0};
	GUI_RECT_T  nps_edit2_rect = {0};
	GUI_RECT_T  nps_text_rect = {0};
	MMI_STRING_T  text_string = {0};
	GUI_FONT_T  text_font = MMI_DEFAULT_NORMAL_FONT_SIZE;
	GUI_COLOR_T text_color = MMI_WHITE_COLOR;
	GUI_COLOR_T by_color = MMI_BLACK_COLOR;
	MMI_STRING_T  one_hour_value = {0};
	uint32        one_hour = 0;
	MMI_STRING_T  hour_account_value = {0};
	uint32        hour_account = 0;
	uint8   one_hour_time = 0;
	uint8   *one_hour_time1 = PNULL;
	uint8   hour_account_time = 0;
	uint8   *hour_account_time1 = PNULL;
	uint16  i = 0;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
		if(MMITHEME_IsMainScreenLandscape())
		{
			both_client_rect = both_rect.h_rect;
		}
		else
		{
			both_client_rect = both_rect.v_rect;
		}
		GUIFORM_SetType(NPS_TIME_CHANGE_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
        GUIFORM_SetBg(NPS_TIME_CHANGE_FORM_CTRL_ID, &by_color);
		GUIFORM_PermitChildBg(NPS_TIME_CHANGE_FORM_CTRL_ID, FALSE);
		GUIFORM_SetType(NPS_TIME_CHANGE1_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
        GUIFORM_PermitChildBg(NPS_TIME_CHANGE1_FORM_CTRL_ID, FALSE);
		GUIFORM_PermitChildFont(NPS_TIME_CHANGE1_FORM_CTRL_ID, FALSE);
             nps_label_rect = both_client_rect;
		     nps_label_rect.bottom = nps_label_rect.top + 28;
		     GUILABEL_SetRect(label_ctrl_id, &nps_label_rect, FALSE);	
	         GUILABEL_SetFont(label_ctrl_id, text_font, text_color);
		     CTRLLABEL_SetTextById(label_ctrl_id, TXT_ENG_NPS_TIMER_CHANGE1, FALSE);
             nps_edit1_rect = both_client_rect;
		     nps_edit1_rect.top = nps_label_rect.bottom + 1;
		     nps_edit1_rect.bottom = nps_edit1_rect.top + 66;
		     GUIEDIT_SetStyle(edit1_ctrl_id,	GUIEDIT_STYLE_MULTI_DYN_DOWN);//设置显示风格
		     GUIEDIT_SetIm(edit1_ctrl_id, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);	
			 GUIEDIT_PermitBorder(edit1_ctrl_id, FALSE);
		     GUIAPICTRL_SetBothRect(edit1_ctrl_id, &nps_edit1_rect);
		     CTRLBASEEDIT_SetLabelId(win_id,edit1_ctrl_id,TXT_ENG_NPS_TIMER_ONE_HOUR,FALSE);
		     GUIEDIT_SetSoftkeyEx(edit1_ctrl_id, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID, PNULL);
        GUIFORM_SetType(NPS_TIME_CHANGE2_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
        GUIFORM_PermitChildBg(NPS_TIME_CHANGE2_FORM_CTRL_ID, FALSE);
		GUIFORM_PermitChildFont(NPS_TIME_CHANGE2_FORM_CTRL_ID, FALSE);
             nps_edit2_rect = both_client_rect;
		     nps_edit2_rect.top = nps_edit1_rect.bottom + 1;
		     nps_edit2_rect.bottom = nps_edit2_rect.top + 66;
             GUIEDIT_SetStyle(edit2_ctrl_id,	GUIEDIT_STYLE_MULTI_DYN_DOWN);//设置显示风格
		     GUIEDIT_SetIm(edit2_ctrl_id, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);	
			 GUIEDIT_PermitBorder(edit2_ctrl_id, FALSE);
		     GUIAPICTRL_SetBothRect(edit2_ctrl_id, &nps_edit2_rect);
		     CTRLBASEEDIT_SetLabelId(win_id,edit2_ctrl_id,TXT_ENG_NPS_TIMER_HOUR_ACCOUNT,FALSE);
 		     GUIEDIT_SetSoftkeyEx(edit2_ctrl_id, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID, PNULL);
             nps_text_rect = both_client_rect;
		     nps_text_rect.top = nps_edit2_rect.bottom + 1;
		     nps_text_rect.bottom = nps_text_rect.top + 60;
		     MMI_GetLabelTextByLang(TXT_ENG_NPS_TIMER_CHANGE_VALUE, &text_string);
             GUITEXT_SetRect(text_ctrl_id, &nps_text_rect);
		     GUITEXT_SetFont(text_ctrl_id, &text_font, &text_color);
		     GUITEXT_SetString(text_ctrl_id, text_string.wstr_ptr, text_string.wstr_len, FALSE);
		MMK_SetAtvCtrl(win_id,NPS_TIME_CHANGE1_FORM_CTRL_ID);
        break;
    case MSG_FULL_PAINT:
		if(MMITHEME_IsMainScreenLandscape())
		{
			both_client_rect = both_rect.h_rect;
		}
		else
		{
			both_client_rect = both_rect.v_rect;
		}
		GUI_FillRect(&lcd_dev_info, both_client_rect, MMI_BLACK_COLOR);
        break;
	case MSG_CTL_EDITBOX_UPDATE_STRNUM:
		MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
		break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;  
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
		GUIEDIT_GetString(edit1_ctrl_id,&one_hour_value);
		GUIEDIT_GetString(edit2_ctrl_id,&hour_account_value);
		if((one_hour_value.wstr_len > 0) && (hour_account_value.wstr_len > 0))
		{
            one_hour_time1 = MMIAPICOM_WstrToStr(one_hour_value.wstr_ptr, &one_hour_time);
			hour_account_time1 = MMIAPICOM_WstrToStr(hour_account_value.wstr_ptr, &hour_account_time);
            //if(one_hour != 0)
		//	{
		//		one_hour = 0;
		//	}
			for ( i = 0; i < one_hour_value.wstr_len; ++i )
			{
                 if ( one_hour_time1[i] >= '0' && one_hour_time1[i] <= '9' )
				 {
                    one_hour *= 10;
                    one_hour +=  one_hour_time1[i] - '0' ;
				 }
                 else
				 {
                   break;
				 }
			}
			//if(hour_account != 0)
			//{
			//	hour_account = 0;
			//}
			for ( i = 0; i < hour_account_value.wstr_len; ++i )
			{
                 if ( hour_account_time1[i] >= '0' && hour_account_time1[i] <= '9' )
				 {
                    hour_account *= 10;
                    hour_account +=  hour_account_time1[i] - '0' ;
				 }
                 else
				 {
                   break;
				 }
			}
            one_hour = one_hour * 1000;//转化为秒
            SCI_TRACE_LOW("[eng] EngNPSTimerChangeWinHandleMsg one_hour = %d", one_hour);
			SCI_TRACE_LOW("[eng] EngNPSTimerChangeWinHandleMsg hour_account = %d", hour_account);
			MMIAPINPS_PromptTimerChange(TRUE, (uint32)one_hour, (uint16)hour_account);			
		    MMK_CloseWin(win_id);
		}
        else
		{
			SCI_MEMSET(&one_hour_value, 0x00, sizeof(one_hour_value));
			SCI_MEMSET(&hour_account_value, 0x00, sizeof(hour_account_value));
		}
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif

/********************************************************************************
 NAME:          Eng_IMS_NV_PARAM_ID_WinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:       zhenxing
 DATE:          
********************************************************************************/

LOCAL MMI_RESULT_E Eng_IMS_NV_PARAM_ID_WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   	 MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T label_ctrl_id = MMIENG_NPS_TIME_LABEL_CTRL_ID;
	MMI_CTRL_ID_T  edit1_ctrl_id = MMIENG_NPS_TIME_EDIT1_CTRL_ID;
	MMI_CTRL_ID_T  edit2_ctrl_id = MMIENG_NPS_TIME_EDIT2_CTRL_ID;
	MMI_CTRL_ID_T  text_ctrl_id = MMIENG_NPS_TIME_TEXT_CTRL_ID;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_BOTH_RECT_T  both_rect = MMITHEME_GetWinClientBothRect(win_id); 
	GUI_RECT_T  both_client_rect = {0};
	GUI_RECT_T  nps_label_rect = {0};
	GUI_RECT_T  nps_edit1_rect = {0};
	GUI_RECT_T  nps_edit2_rect = {0};
	GUI_RECT_T  nps_text_rect = {0};
	MMI_STRING_T  text_string = {0};
	GUI_FONT_T  text_font = MMI_DEFAULT_NORMAL_FONT_SIZE;
	GUI_COLOR_T text_color = MMI_WHITE_COLOR;
	GUI_COLOR_T by_color = MMI_BLACK_COLOR;
	
	MMI_STRING_T  sIMS_NV_ID = {0};
	uint16        uiIMS_NV_ID = 0;
	MMI_STRING_T  sIMS_NV_VALUE = {0};
	wchar wcIMS_NV_Value[256] = {0};
	uint8  ucIMS_NV_ID[4] = {0};
	uint8   *ucIMS_NV_ID1 = PNULL;
	uint8   ucIMS_NV_Value[256] = {0};
	uint8   ucIMS_NV_Value_None = " ";
	uint8   *ucIMS_NV_Value1 = PNULL;
	
	uint16  i = 0;
	uint32  dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);	
   

	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
			SCI_TRACE_LOW("[ENG] func[%s] line[%d] dual_sys %d ",__FUNCTION__,__LINE__,dual_sys);
		g_dual_sys_eng_win = dual_sys;
		if(MMITHEME_IsMainScreenLandscape())
		{
			both_client_rect = both_rect.h_rect;
		}
		else
		{
			both_client_rect = both_rect.v_rect;
		}
		GUIFORM_SetType(NPS_TIME_CHANGE_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
        GUIFORM_SetBg(NPS_TIME_CHANGE_FORM_CTRL_ID, &by_color);
		GUIFORM_PermitChildBg(NPS_TIME_CHANGE_FORM_CTRL_ID, FALSE);
		GUIFORM_SetType(NPS_TIME_CHANGE1_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
        GUIFORM_PermitChildBg(NPS_TIME_CHANGE1_FORM_CTRL_ID, FALSE);
		GUIFORM_PermitChildFont(NPS_TIME_CHANGE1_FORM_CTRL_ID, FALSE);
             nps_label_rect = both_client_rect;
		     nps_label_rect.bottom = nps_label_rect.top + 28;
		     GUILABEL_SetRect(label_ctrl_id, &nps_label_rect, FALSE);	
	         GUILABEL_SetFont(label_ctrl_id, text_font, text_color);
		     CTRLLABEL_SetTextById(label_ctrl_id, TXT_ENG_IMS_NV_PARAM_MES, FALSE);
             nps_edit1_rect = both_client_rect;
		     nps_edit1_rect.top = nps_label_rect.bottom + 1;
		     nps_edit1_rect.bottom = nps_edit1_rect.top + 66+66;
		     GUIEDIT_SetStyle(edit1_ctrl_id,	GUIEDIT_STYLE_MULTI_DYN_DOWN);//设置显示风格
		     GUIEDIT_SetIm(edit1_ctrl_id, GUIIM_TYPE_DIGITAL , GUIIM_TYPE_DIGITAL );	
			 GUIEDIT_PermitBorder(edit1_ctrl_id, FALSE);
		     GUIAPICTRL_SetBothRect(edit1_ctrl_id, &nps_edit1_rect);
		     CTRLBASEEDIT_SetLabelId(win_id,edit1_ctrl_id,TXT_ENG_IMS_NV_ID,FALSE);
		     GUIEDIT_SetSoftkeyEx(edit1_ctrl_id, 2, 1, TXT_COMMON_UPDATE, TXT_READ, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, EDIT_BUTTON_TEXT_ID, PNULL);
        GUIFORM_SetType(NPS_TIME_CHANGE2_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
        GUIFORM_PermitChildBg(NPS_TIME_CHANGE2_FORM_CTRL_ID, FALSE);
		GUIFORM_PermitChildFont(NPS_TIME_CHANGE2_FORM_CTRL_ID, FALSE);
             nps_edit2_rect = both_client_rect;
		     nps_edit2_rect.top = nps_edit1_rect.bottom + 1;
		     nps_edit2_rect.bottom = nps_edit2_rect.top + 66;
             GUIEDIT_SetStyle(edit2_ctrl_id,	GUIEDIT_STYLE_MULTI_DYN_DOWN);//设置显示风格
		     GUIEDIT_SetIm(edit2_ctrl_id,  GUIIM_TYPE_ABC | GUIIM_TYPE_ENGLISH | GUIIM_TYPE_DIGITAL , GUIIM_TYPE_DIGITAL);	
			 GUIEDIT_PermitBorder(edit2_ctrl_id, FALSE);
		     GUIAPICTRL_SetBothRect(edit2_ctrl_id, &nps_edit2_rect);
		     CTRLBASEEDIT_SetLabelId(win_id,edit2_ctrl_id,TXT_ENG_IMS_NV_VALUE,FALSE);
 		     GUIEDIT_SetSoftkeyEx(edit2_ctrl_id, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);

		MMK_SetAtvCtrl(win_id,NPS_TIME_CHANGE1_FORM_CTRL_ID);
        break;
    case MSG_FULL_PAINT:
		if(MMITHEME_IsMainScreenLandscape())
		{
			both_client_rect = both_rect.h_rect;
		}
		else
		{
			both_client_rect = both_rect.v_rect;
		}
		GUI_FillRect(&lcd_dev_info, both_client_rect, MMI_BLACK_COLOR);
        break;
	case MSG_CTL_EDITBOX_UPDATE_STRNUM:
		MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
		break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;  
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
		//ucIMS_NV_Value1 = "this is a test for 123 to test 256 charactor show SCI_TRACE_LOW[eng] one one one_hour = %d ";
		GUIEDIT_GetString(edit1_ctrl_id,&sIMS_NV_ID);
	
		if(sIMS_NV_ID.wstr_len > 0)
		{

            		ucIMS_NV_ID1 = MMIAPICOM_WstrToStr(sIMS_NV_ID.wstr_ptr, ucIMS_NV_ID);

			for ( i = 0; i < sIMS_NV_ID.wstr_len; ++i )
			{
				if ( ucIMS_NV_ID1[i] >= '0' && ucIMS_NV_ID1[i] <= '9' )
				{
					uiIMS_NV_ID *= 10;
					uiIMS_NV_ID +=  ucIMS_NV_ID1[i] - '0' ;
				}
				else
				{
					break;
				}
			}
			
			SCI_TRACE_LOW("[eng] Eng_IMS_NV_PARAM_ID_WinHandleMsg uiIMS_NV_ID = %d dual_sys %d", uiIMS_NV_ID,dual_sys);
			MNPHONE_ImsNvParamGet(uiIMS_NV_ID,ucIMS_NV_Value,  dual_sys);	
			SCI_TRACE_LOW("[eng] Eng_IMS_NV_PARAM_ID_WinHandleMsg ucIMS_NV_Value = %s", ucIMS_NV_Value);
			sIMS_NV_VALUE.wstr_len = strlen(ucIMS_NV_Value) ;
			if(sIMS_NV_VALUE.wstr_len <1)
			{
				CTRLBASEFLEX_ClearAllStr(edit2_ctrl_id);
			}
			else
			{
				MMI_STRNTOWSTR( wcIMS_NV_Value, 256, (uint8*)ucIMS_NV_Value, 256, sIMS_NV_VALUE.wstr_len );
		 		GUIEDIT_SetString(edit2_ctrl_id, wcIMS_NV_Value,sIMS_NV_VALUE.wstr_len);
			}
		}
	
        break;

	case MSG_CTL_OK:


		GUIEDIT_GetString(edit1_ctrl_id,&sIMS_NV_ID);
		GUIEDIT_GetString(edit2_ctrl_id,&sIMS_NV_VALUE);
		if((sIMS_NV_ID.wstr_len > 0) && (sIMS_NV_VALUE.wstr_len > 0))
		{		
         		ucIMS_NV_ID1 = MMIAPICOM_WstrToStr(sIMS_NV_ID.wstr_ptr, ucIMS_NV_ID);
			ucIMS_NV_Value1 = MMIAPICOM_WstrToStr(sIMS_NV_VALUE.wstr_ptr, ucIMS_NV_Value);

			for ( i = 0; i < sIMS_NV_ID.wstr_len; ++i )
			{
				if ( ucIMS_NV_ID1[i] >= '0' && ucIMS_NV_ID1[i] <= '9' )
				{
					uiIMS_NV_ID *= 10;
					uiIMS_NV_ID +=  ucIMS_NV_ID1[i] - '0' ;
				}
				else
				{
					break;
				}
			}
			
			SCI_TRACE_LOW("[eng] Eng_IMS_NV_PARAM_ID_WinHandleMsg uiIMS_NV_ID = %d dual_sys %d", uiIMS_NV_ID,dual_sys);
			SCI_TRACE_LOW("[eng] Eng_IMS_NV_PARAM_ID_WinHandleMsg  len %d sIMS_NV_VALUE = %s", sIMS_NV_VALUE.wstr_len,ucIMS_NV_Value1);
		 	MNPHONE_ImsNvParamSet(uiIMS_NV_ID,ucIMS_NV_Value1,  dual_sys);

		}
		else if((sIMS_NV_ID.wstr_len > 0) && (sIMS_NV_VALUE.wstr_len == 0))
		{
			ucIMS_NV_ID1 = MMIAPICOM_WstrToStr(sIMS_NV_ID.wstr_ptr, ucIMS_NV_ID);

			for ( i = 0; i < sIMS_NV_ID.wstr_len; ++i )
			{
				if ( ucIMS_NV_ID1[i] >= '0' && ucIMS_NV_ID1[i] <= '9' )
				{
					uiIMS_NV_ID *= 10;
					uiIMS_NV_ID +=  ucIMS_NV_ID1[i] - '0' ;
				}
				else
				{
					break;
				}
			}
			
			SCI_MEMSET(ucIMS_NV_Value, 0 , 256);
		 	MNPHONE_ImsNvParamSet(uiIMS_NV_ID,ucIMS_NV_Value,  dual_sys);

		}
		  break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
// 	Description : Append All Plmn List ...
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC void MMIENG_AppendPlmnList(
 						 APP_MN_PREFER_PLMN_CHANGE_CNF_T *  prefer_plmn_sim_cnf,
                                                                                MMI_CTRL_ID_T ctrl_id,                                                                               
                                                                                uint16 index
                                                                                )
{   
    GUILIST_ITEM_DATA_T item_data = {0};
    MN_PLMN_T plmn = {0};
    MMI_STRING_T plmn_name = {0};
    wchar plmn_name_arr[60+1]={0};
    wchar plmn_code[20 +1] = {0};        
    uint16 plmn_code_len = 20;
	
    if(index < MN_PHONE_PLMN_SEL_LIST_NUM -1 )
    {

        plmn.mcc = prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[index].mcc;
        plmn.mnc = prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[index].mnc;
        plmn.mnc_digit_num =prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[index].mnc_digit_num;

        //PLMN NAME Content.
        //get plmn name info
      plmn_name.wstr_ptr =(wchar *)plmn_name_arr;
	plmn_name.wstr_len = 60;
	
        MMIAPIPHONE_GenPLMNDisplay(prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[index], &plmn, plmn_name.wstr_ptr, &plmn_name.wstr_len, FALSE);
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
        item_data.item_content[0].item_data.text_buffer.wstr_len = plmn_name.wstr_len;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = plmn_name.wstr_ptr;

        //PLMN CODE Content .        
        //get plmn code info
        MMIAPIPHONE_GenPLMNDisplay(prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[index], &plmn,plmn_code, &plmn_code_len, TRUE);
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
        item_data.item_content[1].item_data.text_buffer.wstr_len = plmn_code_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = plmn_code;
        
        //set softkey
       // item_data.softkey_id[0] =  TXT_EDIT;
       // item_data.softkey_id[1] =  TXT_NULL;
      //  item_data.softkey_id[2] = STXT_RETURN;

        GUILIST_SetItemData( ctrl_id, &item_data, index );    
        //GUILIST_SetOwnSofterKey(ctrl_id, TRUE);

    }
}


/*****************************************************************************/
//  Description : InitPlmnListCtrl
//  Global resource dependence : 
//  Author:michael.shi
//  Note: InitPlmnListCtrl
/*****************************************************************************/
LOCAL void MMIENG_InitPlmnListCtrl(MMI_CTRL_ID_T ctrl_id,uint16 max_item_num)
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


LOCAL MMI_RESULT_E MMIENG_HandleUPLMNRATSETWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T           text_id_tab[] = {
                                            TXT_ENG_UPDATE_TEST_NEVER, 
                                            TXT_ENG_UPDATE_TEST_ONE_DAY, 
                                            TXT_ENG_UPDATE_TEST_ONE_WEEK,
                                            TXT_ENG_UPDATE_TEST_ONE_MONTH                                   
                                            };
	MMI_DATA_ID_T     update_e_tab[] = {   MMIENG_UPDATE_TEST_NEVER,  MMIENG_UPDATE_TEST_DAY, MMIENG_UPDATE_TEST_WEEK, MMIENG_UPDATE_TEST_MONTH};
	int16                   item_pos = 0;
	int16                   item_total_num = 0;
	MMI_CTRL_ID_T           list_id = MMIENG_FORM_CTRL_ID;
	uint16                  i         = 0;
	APP_MN_PREFER_PLMN_CHANGE_CNF_T *  prefer_plmn_sim_cnf= MMK_GetWinAddDataPtr(win_id);    
	GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T	 item_data = {0};/*lint !e64*/
	USB_SERVICE_E is_SetOn= MMIENG_UPDATE_TEST_NEVER ;
	uint16 cur_item_index = 0;
	uint16 total_item = 0;
	item_total_num = ARR_SIZE(text_id_tab);
	
	switch (msg_id) 
	{
	    case MSG_OPEN_WINDOW:
	        GUILIST_SetMaxItem(list_id, item_total_num, FALSE );//max item 5
	        //GUILIST_SetOwnSofterKey(list_id,TRUE);
	        // AppendListItem(list_id, text_id_tab, item_total_num );
	        {
	            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	            item_t.item_data_ptr = &item_data;
			    
	            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
			    
	            item_data.softkey_id[0] = TXT_COMMON_OK;
	            item_data.softkey_id[1] = TXT_NULL;
	            item_data.softkey_id[2] = STXT_RETURN; 
			    
	            for ( i = 0; i < item_total_num; i ++ )
	            {
	                item_t.user_data = update_e_tab[i];
	                item_data.item_content[0].item_data.text_id = text_id_tab[i];
			        
	                GUILIST_AppendItem( list_id, &item_t );
	            }
	        }
			

		 total_item = GUILIST_GetTotalItemNum(MMIENG_LISTBOX_CTRL_ID);
		if (total_item > 0)
		{
			 cur_item_index = GUILIST_GetCurItemIndex(MMIENG_LISTBOX_CTRL_ID);
			
			item_pos = prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[cur_item_index]; //MN_GMMREG_RAT_E
			
		}
				   
	        GUILIST_SetSelectedItem(list_id, (uint16)item_pos, TRUE);
	        GUILIST_SetCurItemIndex(list_id, (int16)item_pos);
	        MMK_SetAtvCtrl(win_id, list_id);
	        break;
        
	    case MSG_FULL_PAINT:
	        break;
	        
	    case MSG_KEYDOWN_CANCEL:
	    case MSG_CTL_CANCEL:
	        MMK_CloseWin(win_id);
	        break;
	    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_KEYDOWN_OK:
	    case MSG_CTL_OK:
	    case MSG_APP_WEB:
		if(GUILIST_GetTotalItemNum(list_id))
		{
			 item_pos = GUILIST_GetCurItemIndex(list_id);
			
			 cur_item_index = GUILIST_GetCurItemIndex(MMIENG_LISTBOX_CTRL_ID);
			
			 prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[cur_item_index] = item_pos;
			
			 MNSIM_SetPLMNWACTEx(prefer_plmn_sim_cnf->dual_sys, &(prefer_plmn_sim_cnf->new_prefer_plmn_list));
			 MMIAPIENG_WaitPreferNetworkList();
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
//  Description : HandleEngFreqOptionMsg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/

LOCAL void CreateEngUPLMNOptionMenu(uint32 ctr_id,uint16 max)
{
    MMI_STRING_T  kstring = {0};
    uint16        node_index =0;
    uint32         i = 0;
    uint32        item_num = 0;  
    GUIMENU_DYNA_ITEM_T     node_item = {0};
	uint16 curtotalitem = 0;
	uint16 maxtotalitem = max;
    item_num = ARR_SIZE(s_eng_uplmn_opt_item);
    curtotalitem = GUILIST_GetTotalItemNum(MMIENG_LISTBOX_CTRL_ID) ;
	SCI_TRACE_LOW("mymark func[%s] line[%d] total_item %d max %d",__FUNCTION__,__LINE__,curtotalitem,MN_PHONE_PLMN_SEL_LIST_NUM);  
    for(i = 0; i < item_num; i++)
    {
        MMI_GetLabelTextByLang(s_eng_uplmn_opt_item[i][ENG_UPLMN_POP_MENU_TEXT_ID], &kstring);
        node_item.item_text_ptr = &kstring;
        GUIMENU_InsertNode(node_index, s_eng_uplmn_opt_item[i][ENG_UPLMN_POP_MENU_NODE_ID], 0, &node_item, ctr_id);
	
	if ( maxtotalitem  <= curtotalitem )
	{
	      if(ENG_UPLMN_ADD == s_eng_uplmn_opt_item[i][ENG_UPLMN_POP_MENU_NODE_ID])
            {
                //empty freq can not delete
                GUIMENU_SetNodeGrayed(TRUE, node_index, 0,  ctr_id);
            }
	}
      	if ( 0 == curtotalitem)
        {
            //no freq, need not to delete
            if(ENG_UPLMN_DELETE == s_eng_uplmn_opt_item[i][ENG_UPLMN_POP_MENU_NODE_ID])
            {
                GUIMENU_SetNodeGrayed(TRUE, node_index, 0,  ctr_id);
            }
	      if(ENG_UPLMN_EDIT == s_eng_uplmn_opt_item[i][ENG_UPLMN_POP_MENU_NODE_ID])
            {
                GUIMENU_SetNodeGrayed(TRUE, node_index, 0,  ctr_id);
            }
        }
        node_index ++;
    }
    
}
/********************************************************************************
 NAME:          Eng_UPLMN_EDIT_AND_EDIT_WinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:       zhenxing
 DATE:          
********************************************************************************/

LOCAL MMI_RESULT_E Eng_UPLMN_EDIT_AND_EDIT_WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   	 MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T label_ctrl_id = MMIENG_NPS_TIME_LABEL_CTRL_ID;
	MMI_CTRL_ID_T  edit1_ctrl_id = MMIENG_NPS_TIME_EDIT1_CTRL_ID;
	MMI_CTRL_ID_T  edit2_ctrl_id = MMIENG_NPS_TIME_EDIT2_CTRL_ID;
	MMI_CTRL_ID_T  edit3_ctrl_id = MMIENG_EDITBOX2_CTRL_ID;
	MMI_CTRL_ID_T  text_ctrl_id = MMIENG_NPS_TIME_TEXT_CTRL_ID;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_BOTH_RECT_T  both_rect = MMITHEME_GetWinClientBothRect(win_id); 
	GUI_RECT_T  both_client_rect = {0};
	GUI_RECT_T  nps_label_rect = {0};
	GUI_RECT_T  nps_edit1_rect = {0};
	GUI_RECT_T  nps_edit2_rect = {0};
	GUI_RECT_T  nps_edit3_rect = {0};
	GUI_RECT_T  nps_text_rect = {0};
	MMI_STRING_T  text_string = {0};
	GUI_FONT_T  text_font = MMI_DEFAULT_NORMAL_FONT_SIZE;
	GUI_COLOR_T text_color = MMI_WHITE_COLOR;
	GUI_COLOR_T by_color = MMI_BLACK_COLOR;
	 MMI_STRING_T text = {0};
	 char * str = PNULL;
	 wchar wstr[80] = {0};
	uint16 curtotalitem = 0;
	uint16 curitemindex = 0;

	MMI_STRING_T  sUPLMN_MCC_ID = {0};
	MMI_STRING_T  sUPLMN_MNC_ID = {0};
	MMI_STRING_T  sUPLMN_RAT_ID = {0};

	uint8  ucUPLMN_MCC_ID[3] = {0};
	uint8  ucUPLMN_MNC_ID[3] = {0};
	uint8  ucUPLMN_RAT_ID[3] = {0};
	
	uint8   *ucUPLMN_MCC_ID1 = PNULL;
	uint8   *ucUPLMN_MNC_ID1 = PNULL;
	uint8   *ucUPLMN_RAT_ID1 = PNULL;

	uint16   uiPLMN_MCC_ID = 0;
	uint16   uiPLMN_MNC_ID = 0;
	uint16   uiPLMN_RAT_ID = 0;

	uint16  i = 0;
	APP_MN_PREFER_PLMN_CHANGE_CNF_T *  prefer_plmn_sim_cnf= MMK_GetWinAddDataPtr(win_id);    

	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
			
		if(MMITHEME_IsMainScreenLandscape())
		{
			both_client_rect = both_rect.h_rect;
		}
		else
		{
			both_client_rect = both_rect.v_rect;
		}

		//edit1
		GUIFORM_SetType(NPS_TIME_CHANGE_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
		GUIFORM_SetBg(NPS_TIME_CHANGE_FORM_CTRL_ID, &by_color);
		GUIFORM_PermitChildBg(NPS_TIME_CHANGE_FORM_CTRL_ID, FALSE);
		GUIFORM_SetType(NPS_TIME_CHANGE1_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
		GUIFORM_PermitChildBg(NPS_TIME_CHANGE1_FORM_CTRL_ID, FALSE);
		GUIFORM_PermitChildFont(NPS_TIME_CHANGE1_FORM_CTRL_ID, FALSE);
		nps_label_rect = both_client_rect;
		nps_label_rect.bottom = nps_label_rect.top + 28;
		GUILABEL_SetRect(label_ctrl_id, &nps_label_rect, FALSE);	
		GUILABEL_SetFont(label_ctrl_id, text_font, text_color);
		CTRLLABEL_SetTextById(label_ctrl_id, TXT_ENG_IMS_NV_PARAM_MES, FALSE);
		nps_edit1_rect = both_client_rect;
		nps_edit1_rect.top = nps_label_rect.bottom + 1;
		nps_edit1_rect.bottom = nps_edit1_rect.top + 66;
		GUIEDIT_SetStyle(edit1_ctrl_id,	GUIEDIT_STYLE_MULTI_DYN_DOWN);//设置显示风格
		GUIEDIT_SetIm(edit1_ctrl_id, GUIIM_TYPE_DIGITAL , GUIIM_TYPE_DIGITAL );	
		GUIEDIT_PermitBorder(edit1_ctrl_id, FALSE);
		GUIEDIT_SetFontColor(edit1_ctrl_id, text_color);
		GUIAPICTRL_SetBothRect(edit1_ctrl_id, &nps_edit1_rect);	
		str = "MCC:";
		text.wstr_len = strlen(str);
		text.wstr_ptr = wstr;
		MMI_STRNTOWSTR( wstr, 10, (uint8*)str, text.wstr_len, text.wstr_len );
		CTRLBASEEDIT_SetLabel(win_id, edit1_ctrl_id, text.wstr_ptr,text.wstr_len ,FALSE);
		// CTRLBASEEDIT_SetLabelId(win_id,edit1_ctrl_id,TXT_ENG_IMS_NV_ID,FALSE);
		GUIEDIT_SetSoftkeyEx(edit1_ctrl_id, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);
		//FORM2
		GUIFORM_SetType(NPS_TIME_CHANGE2_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
		GUIFORM_PermitChildBg(NPS_TIME_CHANGE2_FORM_CTRL_ID, FALSE);
		GUIFORM_PermitChildFont(NPS_TIME_CHANGE2_FORM_CTRL_ID, FALSE);
		//edit2
		nps_edit2_rect = both_client_rect;
		nps_edit2_rect.top = nps_edit1_rect.bottom + 1;
		nps_edit2_rect.bottom = nps_edit2_rect.top + 66;
		GUIEDIT_SetStyle(edit2_ctrl_id,	GUIEDIT_STYLE_MULTI_DYN_DOWN);//设置显示风格
		GUIEDIT_SetIm(edit2_ctrl_id,   GUIIM_TYPE_DIGITAL , GUIIM_TYPE_DIGITAL);	
		GUIEDIT_PermitBorder(edit2_ctrl_id, FALSE);
		GUIEDIT_SetFontColor(edit2_ctrl_id, text_color);
		GUIAPICTRL_SetBothRect(edit2_ctrl_id, &nps_edit2_rect);
		str = "MNC: ";
		text.wstr_len = strlen(str);
		text.wstr_ptr = wstr;
		MMI_STRNTOWSTR( wstr, 10, (uint8*)str, text.wstr_len, text.wstr_len );
		CTRLBASEEDIT_SetLabel(win_id, edit2_ctrl_id, text.wstr_ptr,text.wstr_len ,FALSE);
		//CTRLBASEEDIT_SetLabelId(win_id,edit2_ctrl_id,TXT_ENG_IMS_NV_VALUE,FALSE);
		GUIEDIT_SetSoftkeyEx(edit2_ctrl_id, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);
		//edit3
		nps_edit3_rect = both_client_rect;
		nps_edit3_rect.top = nps_edit2_rect.bottom + 1;
		nps_edit3_rect.bottom = nps_edit3_rect.top + 66;
		GUIEDIT_SetStyle(edit3_ctrl_id,	GUIEDIT_STYLE_MULTI_DYN_DOWN);//设置显示风格
		GUIEDIT_SetIm(edit3_ctrl_id,  GUIIM_TYPE_DIGITAL , GUIIM_TYPE_DIGITAL);	
		GUIEDIT_PermitBorder(edit3_ctrl_id, FALSE);
		GUIEDIT_SetFontColor(edit3_ctrl_id, text_color);
		GUIAPICTRL_SetBothRect(edit3_ctrl_id, &nps_edit3_rect);
		str = "RAT:   ";
		text.wstr_len = strlen(str);
		text.wstr_ptr = wstr;
		MMI_STRNTOWSTR( wstr, 80, (uint8*)str, text.wstr_len, text.wstr_len );
		CTRLBASEEDIT_SetLabel(win_id, edit3_ctrl_id, text.wstr_ptr,text.wstr_len ,FALSE);
		//CTRLBASEEDIT_SetLabelId(win_id,edit3_ctrl_id,TXT_ENG_IMS_NV_VALUE,FALSE);
		GUIEDIT_SetSoftkeyEx(edit3_ctrl_id, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);

		if(ENG_UPLMN_EDIT == ENG_UPLMN_EDIT_OR_ADD_FLAG)
		{
			char temp_str[10] = {0};
			uint8 length = 10;
			
			curtotalitem = GUILIST_GetTotalItemNum(MMIENG_LISTBOX_CTRL_ID) ;
			if(curtotalitem > 0 )
			{
				curitemindex = GUILIST_GetCurItemIndex(MMIENG_LISTBOX_CTRL_ID); 
				SCI_TRACE_LOW("[ENG] func[%s] line[%d] curitemindex[%d]",__FUNCTION__,__LINE__,curitemindex);
				SCI_MEMSET(temp_str, 0 , length);                
				SCI_MEMSET(wstr, 0 , length*2);
				length = sprintf(temp_str, "%d", prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mcc);
				MMI_STRNTOWSTR(wstr, 10, (uint8*)temp_str, length, length);
				GUIEDIT_SetString(edit1_ctrl_id, wstr,length);

				SCI_MEMSET(temp_str, 0 , length);                
				SCI_MEMSET(wstr, 0 , length*2);
				length = sprintf(temp_str, "%d", prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mnc);
				MMI_STRNTOWSTR(wstr, 10, (uint8*)temp_str, length, length);
				GUIEDIT_SetString(edit2_ctrl_id, wstr,length);

				SCI_MEMSET(temp_str, 0 , length);                
				SCI_MEMSET(wstr, 0 , length*2);
				length = sprintf(temp_str, "%d", prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[curitemindex]);
				MMI_STRNTOWSTR(wstr, 10, (uint8*)temp_str, length, length);
				GUIEDIT_SetString(edit3_ctrl_id, wstr,length);
			}
		}


		MMK_SetAtvCtrl(win_id,NPS_TIME_CHANGE1_FORM_CTRL_ID);
        break;
    case MSG_FULL_PAINT:
		if(MMITHEME_IsMainScreenLandscape())
		{
			both_client_rect = both_rect.h_rect;
		}
		else
		{
			both_client_rect = both_rect.v_rect;
		}
		GUI_FillRect(&lcd_dev_info, both_client_rect, MMI_BLACK_COLOR);
        break;
	case MSG_CTL_EDITBOX_UPDATE_STRNUM:
		MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
		break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;  
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_OK:
		//ucIMS_NV_Value1 = "this is a test for 123 to test 256 charactor show SCI_TRACE_LOW[eng] one one one_hour = %d ";
		GUIEDIT_GetString(edit1_ctrl_id,&sUPLMN_MCC_ID);
		GUIEDIT_GetString(edit2_ctrl_id,&sUPLMN_MNC_ID);
		GUIEDIT_GetString(edit3_ctrl_id,&sUPLMN_RAT_ID);
		if((sUPLMN_MCC_ID.wstr_len > 0)&&(sUPLMN_MNC_ID.wstr_len > 0)&&(sUPLMN_RAT_ID.wstr_len > 0))
		{

            		ucUPLMN_MCC_ID1 = MMIAPICOM_WstrToStr(sUPLMN_MCC_ID.wstr_ptr, ucUPLMN_MCC_ID);

			for ( i = 0; i < sUPLMN_MCC_ID.wstr_len; ++i )
			{
				if ( ucUPLMN_MCC_ID1[i] >= '0' && ucUPLMN_MCC_ID1[i] <= '9' )
				{
					uiPLMN_MCC_ID *= 10;
					uiPLMN_MCC_ID +=  ucUPLMN_MCC_ID1[i] - '0' ;
				}
				else
				{
					break;
				}
			}
			
			ucUPLMN_MNC_ID1 = MMIAPICOM_WstrToStr(sUPLMN_MNC_ID.wstr_ptr, ucUPLMN_MNC_ID);
			for ( i = 0; i < sUPLMN_MNC_ID.wstr_len; ++i )
			{
				if ( ucUPLMN_MNC_ID1[i] >= '0' && ucUPLMN_MNC_ID1[i] <= '9' )
				{
					uiPLMN_MNC_ID *= 10;
					uiPLMN_MNC_ID +=  ucUPLMN_MNC_ID1[i] - '0' ;
				}
				else
				{
					break;
				}
			}
			ucUPLMN_RAT_ID1 = MMIAPICOM_WstrToStr(sUPLMN_RAT_ID.wstr_ptr, ucUPLMN_RAT_ID);
			for ( i = 0; i < sUPLMN_RAT_ID.wstr_len; ++i )
			{
				if ( ucUPLMN_RAT_ID1[i] >= '0' && ucUPLMN_RAT_ID1[i] <= '9' )
				{
					uiPLMN_RAT_ID *= 10;
					uiPLMN_RAT_ID +=  ucUPLMN_RAT_ID1[i] - '0' ;
				}
				else
				{
					break;
				}
			}

			SCI_TRACE_LOW("[ENG] func[%s] line[%d]  mcc[%d] mnc[%d] rat[%d] ",__FUNCTION__,__LINE__, uiPLMN_MCC_ID,uiPLMN_MNC_ID,uiPLMN_RAT_ID);

			if(ENG_UPLMN_EDIT == ENG_UPLMN_EDIT_OR_ADD_FLAG)
			{		
				curitemindex = GUILIST_GetCurItemIndex(MMIENG_LISTBOX_CTRL_ID);
				prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mcc = uiPLMN_MCC_ID;
				prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mnc = uiPLMN_MNC_ID;
				prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[curitemindex]= uiPLMN_RAT_ID;
				if(uiPLMN_MNC_ID>99)
				{
					prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mnc_digit_num = 3;
				}
				else
				{
					prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mnc_digit_num = 2;
				}
					
				MNSIM_SetPLMNWACTEx(prefer_plmn_sim_cnf->dual_sys, &(prefer_plmn_sim_cnf->new_prefer_plmn_list));
				MMIAPIENG_WaitPreferNetworkList();
			}
			else if(ENG_UPLMN_ADD == ENG_UPLMN_EDIT_OR_ADD_FLAG )
			{
				curtotalitem = GUILIST_GetTotalItemNum(MMIENG_LISTBOX_CTRL_ID) ;

				if(curtotalitem <  prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_max_num)
				{
					curitemindex = prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_validate_num;
					prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_validate_num++;
					prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mcc = uiPLMN_MCC_ID;
					prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mnc = uiPLMN_MNC_ID;
					prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[curitemindex]= uiPLMN_RAT_ID;
					if(uiPLMN_MNC_ID>99)
					{
						prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mnc_digit_num = 3;
					}
					else
					{
						prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mnc_digit_num = 2;
					}
				
					MNSIM_SetPLMNWACTEx(prefer_plmn_sim_cnf->dual_sys, &(prefer_plmn_sim_cnf->new_prefer_plmn_list));
					MMIAPIENG_WaitPreferNetworkList();
					
				}
				
			}

		}
	
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}




/*****************************************************************************/
//  Description : HandleEngFreqOptionMsg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEngUPLMNOptionMenuMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   MMI_RESULT_E            result = MMI_RESULT_TRUE;
   uint32                  menu_id = 0;
   uint16 index=0;
	uint8 curtotalitem = 0;
	uint8   curitemindex = 0;
   APP_MN_PREFER_PLMN_CHANGE_CNF_T *  prefer_plmn_sim_cnf= MMK_GetWinAddDataPtr(win_id);    
   switch (msg_id)
   {
   case MSG_OPEN_WINDOW:
       CreateEngUPLMNOptionMenu(MENU_POPUP_CTRL_ID,prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_max_num); 
       MMK_SetAtvCtrl(win_id, MENU_POPUP_CTRL_ID);
       break;
       
   case MSG_APP_OK:
   case MSG_CTL_OK:
   case MSG_APP_WEB:
   case MSG_CTL_MIDSK:
       menu_id = GUIMENU_GetCurNodeId(MENU_POPUP_CTRL_ID);     
       switch(menu_id)  
       {
       case ENG_UPLMN_ADD:

           ENG_UPLMN_EDIT_OR_ADD_FLAG = ENG_UPLMN_ADD;
           MMK_CreateWin((uint32 *)MMIENG_UPLMN_EDIT_AND_ADD_WIN_TAB, (ADD_DATA)prefer_plmn_sim_cnf);
           
           break;
       case ENG_UPLMN_EDIT:

           ENG_UPLMN_EDIT_OR_ADD_FLAG = ENG_UPLMN_EDIT;
           MMK_CreateWin((uint32 *)MMIENG_UPLMN_EDIT_AND_ADD_WIN_TAB, (ADD_DATA)prefer_plmn_sim_cnf);
         
           break;
       case ENG_UPLMN_DELETE:

		curtotalitem = GUILIST_GetTotalItemNum(MMIENG_LISTBOX_CTRL_ID) ;
		if(curtotalitem > 0 )
		{
			curitemindex = GUILIST_GetCurItemIndex(MMIENG_LISTBOX_CTRL_ID); 
			SCI_TRACE_LOW("[ENG] func[%s] line[%d] curindex before mcc[%d] mnc[%d] digt[%d] rat[%d]",__FUNCTION__,__LINE__,prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mcc,
											prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mnc,
											prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mnc_digit_num,
											prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[curitemindex]);
			if(curitemindex == (curtotalitem -1) )
			{
				prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_validate_num--;
				SCI_MEMSET(&(prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curtotalitem-1]), 0 , sizeof(prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curtotalitem-1]));
 				prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[curtotalitem-1]= 0 ;
				MNSIM_SetPLMNWACTEx(prefer_plmn_sim_cnf->dual_sys, &(prefer_plmn_sim_cnf->new_prefer_plmn_list));
				MMIAPIENG_WaitPreferNetworkList();				
			}
			else if(curitemindex < curtotalitem )
			{	
				SCI_MEMSET(&(prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex]), 0 , sizeof(prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex]));
				SCI_MEMCPY(&(prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex]),&(prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curtotalitem-1]),sizeof(prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex]));
				prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_validate_num--;
				prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[curitemindex] = prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[curtotalitem-1];
				SCI_MEMSET(&(prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curtotalitem-1]), 0 , sizeof(prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curtotalitem-1]));
 				prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[curtotalitem-1]= 0 ;				
				MNSIM_SetPLMNWACTEx(prefer_plmn_sim_cnf->dual_sys, &(prefer_plmn_sim_cnf->new_prefer_plmn_list));
				MMIAPIENG_WaitPreferNetworkList();	
			}

			SCI_TRACE_LOW("[ENG] func[%s] line[%d] curitemindex[%d] curtotalitem[%d] max[%d]",__FUNCTION__,__LINE__,curitemindex,curtotalitem,prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_validate_num);	
			SCI_TRACE_LOW("[ENG] func[%s] line[%d] curindex aftermcc[%d] mnc[%d] digt[%d] rat[%d]",__FUNCTION__,__LINE__,prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mcc,
														prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mnc,
														prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_arr[curitemindex].mnc_digit_num,
														prefer_plmn_sim_cnf->new_prefer_plmn_list.rat[curitemindex]);
		}

           break;
       default:
           break;
           
       }
       MMK_CloseWin(win_id);
       
       break;
       
       case MSG_APP_CANCEL:
       case MSG_CTL_CANCEL:
       case MSG_LOSE_FOCUS:
           MMK_CloseWin(win_id);
           break;
           
       default:
           result = MMI_RESULT_FALSE;
           break;
   }
   
   return result;
}



/*****************************************************************************/
//  Description : handle display prefer network list window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIENG_HandleDispPreferNetworkListWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    APP_MN_PREFER_PLMN_CHANGE_CNF_T *  prefer_plmn_sim_cnf= MMK_GetWinAddDataPtr(win_id);    
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    
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
        
            max_list_num = prefer_plmn_sim_cnf->new_prefer_plmn_list.plmn_validate_num;
            
            MMIENG_InitPlmnListCtrl(ctrl_id, max_list_num);
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_EDIT,TXT_NULL , STXT_RETURN, FALSE);
        }
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            uint16 index = 0;
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
    	
            if( PNULL != need_item_data_ptr )
            {
                index = need_item_data_ptr->item_index;
                MMIENG_AppendPlmnList(  prefer_plmn_sim_cnf,ctrl_id,index);
            }
        }
        break;            
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        {
            uint16 total_item = GUILIST_GetTotalItemNum(ctrl_id);
   
            //if (total_item > 0)
            {
			uint16 cur_item_index = GUILIST_GetCurItemIndex(ctrl_id);
	
			
			 MMK_CreateWin((uint32 *)ENG_UPLMN_OPTION_MENU_WIN_TAB, (ADD_DATA)prefer_plmn_sim_cnf);
        		 //creat dynamic menu
       		 GUIMENU_CreatDynamic(PNULL,MMIENG_UPLMN_OPTION_MENU_WIN_ID,MENU_POPUP_CTRL_ID,GUIMENU_STYLE_POPUP);
			
				
            }
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
	  break;
	 #if 0
        {
		uint16 total_item = GUILIST_GetTotalItemNum(ctrl_id);
	
            if (total_item > 0)
            {
			uint16 cur_item_index = GUILIST_GetCurItemIndex(ctrl_id);
			
			MMK_CreateWin((uint32 *)MMIENG_UPLMN_SET_RAT_WIN_TAB , (ADD_DATA)prefer_plmn_sim_cnf);			
            }
        }
      #endif

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:            
        MMK_CloseWin( win_id );      
        break;
               
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


/********************************************************************************
 NAME:          EngAutoAnswerCallWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        bin.ji
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngAutoAnswerCallWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        //GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        
        if (MMIAPIENG_GetAutoAnswerCallSet())
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;  
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        MMIAPISET_SetByOpenCloseList(ctrl_id, MMIAPISET_GetActiveSim(),MMIAPIENG_SetAutoAnswerCallSet);
        MMK_CloseWin( win_id );
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#ifdef MMI_SMS_RETRY_SUPPORT
/********************************************************************************
 NAME:          EngAutoAnswerCallWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        bin.ji
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngSMSRetryWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    BOOLEAN RetryEnable=FALSE;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        //GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        
        if (MMIAPISMS_GetResendCount())
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;  
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
     if (cur_selection)
        {
            RetryEnable = FALSE;
        }
        else
        {
            RetryEnable = TRUE;
        }
        MMIAPISMS_SetResendCount(RetryEnable);
        MMK_CloseWin( win_id );
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif


/********************************************************************************
 NAME:          EngCallBarringWinHandleMsg
 DESCRIPTION:   MS00186848
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        fen.xie
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngCallBarringWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    BOOLEAN IsEnableCallBarring = FALSE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        
        if (MMIAPIENG_GetCallBarring())
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;  
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (cur_selection)
        {
            IsEnableCallBarring = FALSE;
        }
        else
        {
            IsEnableCallBarring = TRUE;
        }
        MMIAPIENG_SetCallBarring(IsEnableCallBarring);
        MMK_CloseWin( win_id );
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : handle set lcd logic angle window msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E EngSetAngleWinHandleMsg(
                                           MMI_WIN_ID_T         win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM               param
                                           )
{
    uint16              cur_selection = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIENG_LISTBOX_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,4,FALSE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_ANGLE_0,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_ANGLE_90,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_ANGLE_180,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_ANGLE_270,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        
        //get logic angle
        cur_selection = (uint16)GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

        //set logic angle
//        GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,(LCD_ANGLE_E)cur_selection);
        MMK_SetScreenAngle(( LCD_ANGLE_E)cur_selection );
        MMK_CloseWin( win_id );
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/********************************************************************************
 NAME:          MMIENG_OpenIQModeWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        zhaohui
 DATE:      0   2009.03.03
********************************************************************************/
PUBLIC void MMIENG_OpenIQModeWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_IQ_MODE_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : open IQ Mode win
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenIQModeOnOffWin(void)
{
    if (MMIAPIENG_GetIQDataStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_ENG_IQ_DATA_STOP_IQ_DATA_WARNING);
    }
    else
    {
        MMK_CreateWin((uint32*)MMIENG_IQ_MODE_ONOFF_WIN_TAB, PNULL);
    }
}

/*****************************************************************************/
//  Description : EngIQModeWinHandleMsg
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E EngIQModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_CTRL_ID_T ctrl_id = MMIENG_IQ_MODE_CTRL_ID;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            if (MMIAPIENG_GetIQModeStatus())
            {
                GUIMENU_SetItemGrayed(MMIENG_IQ_MODE_CTRL_ID,MENU_IQ_MODE, ID_ENG_IQ_DATA, FALSE);
            }
            else
            {
                GUIMENU_SetItemGrayed(MMIENG_IQ_MODE_CTRL_ID,MENU_IQ_MODE, ID_ENG_IQ_DATA, TRUE);
            }
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;

        case MSG_GET_FOCUS:
            if (MMIAPIENG_GetIQModeStatus())
            {
                GUIMENU_SetItemGrayed(MMIENG_IQ_MODE_CTRL_ID,MENU_IQ_MODE, ID_ENG_IQ_DATA, FALSE);
            }
            else
            {
                GUIMENU_SetItemGrayed(MMIENG_IQ_MODE_CTRL_ID,MENU_IQ_MODE, ID_ENG_IQ_DATA, TRUE);
            }
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
            
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            GUIMENU_GetId(ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *)(&menu_id));
            switch (menu_id)
            {
                case ID_ENG_IQ_DATA_ONOFF:
                    MMIAPIENG_OpenIQModeOnOffWin();
                    break;

                case ID_ENG_IQ_DATA:
                    MMIAPIENG_OpenIQDataWin();
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
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : EngIQModeOnOffWinHandleMsg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E EngIQModeOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();

    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
            GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
            MMIAPISET_AppendListItemByTextIdExt(TXT_COMMON_OPEN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
            MMIAPISET_AppendListItemByTextIdExt(TXT_COMMON_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  

            if (MMIAPIENG_GetIQModeStatus())
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
            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;

        case MSG_FULL_PAINT:
            break;
            
        case MSG_KEYDOWN_CANCEL:
        case MSG_KEYDOWN_OK:
            break;

        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
 
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            if (0 == cur_selection)
            {
                iq_mode_param.b_status = TRUE;
                                
                MMIAPISET_StopAllRing(TRUE);

                MMIDEFAULT_EnableKeyTpRing("ENG IQ MODE", FALSE);
            }
            else
            {
                iq_mode_param.b_status = FALSE;
                MMIDEFAULT_EnableKeyTpRing("ENG IQ MODE", TRUE);
            }

            MMIAPIENG_SetIQMode(&iq_mode_param);
            //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : HandleIQDataWinMsg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIQDataWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_CTRL_ID_T ctrl_id = MMIENG_IQ_DATA_CTRL_ID;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIMENU_GetId(ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *)(&menu_id));
            if (MMIAPIENG_GetIQDataStatus())
            {
                GUIMENU_SetItemGrayed(MMIENG_IQ_DATA_CTRL_ID,group_id, ID_ENG_IQ_DATA_SETTINGS, TRUE);
            }
            else
            {
                GUIMENU_SetItemGrayed(MMIENG_IQ_DATA_CTRL_ID,group_id, ID_ENG_IQ_DATA_SETTINGS, FALSE);
            }
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;

        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            GUIMENU_GetId(ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *)(&menu_id));
            switch (menu_id)
            {
                case ID_ENG_IQ_DATA_ENABLE:
                    if (MMIAPIENG_StartIQData())
                    {
                        iq_mode_param.b_is_start = TRUE;
                        MMIAPIENG_SetIQMode(&iq_mode_param);
                        //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
                        MMK_CloseWin(win_id);
                    }
                    break;

                case ID_ENG_IQ_DATA_DISABLE:
                    MMIAPIENG_StopQData();
                    //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
                    MMK_CloseWin(win_id);
                    break;

                case ID_ENG_IQ_DATA_MODES_SETTINGS:
                    MMK_CreateWin((uint32*)MMIENG_IQ_DATA_MODE_SELECT_WIN_TAB, PNULL);
                    break;

                case ID_ENG_IQ_DATA_POWER_ON_START:
                    MMK_CreateWin((uint32*)MMIENG_IQ_DATA_POWER_ON_START_WIN_TAB, PNULL);
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
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : MMIAPIENG_OpenIQDataWin
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenIQDataWin(void)
{
    if (MMK_IsOpenWin(MMIENG_IQ_DATA_WIN_ID))
    {
        return;
    }

    if (MMIAPIENG_GetIQModeStatus())
    {
        if (MMIAPIENG_GetIQDataStatus())
        {
            MMK_CreateWin((uint32*)MMIENG_IQ_DATA_DISABLE_WIN_TAB, PNULL);
        }
        else
        {
            MMK_CreateWin((uint32*)MMIENG_IQ_DATA_ENABLE_WIN_TAB, PNULL);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIENG_WIN.C MMIAPIENG_OpenIQDataWin IQ Mode Is Disable!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_12824_112_2_18_2_18_7_199,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : EngIQDataModeSelectWinHandleMsg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E EngIQDataModeSelectWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID;
    uint16 cur_selection = 0;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();

    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(ctrl_id,MMIENG_IQ_MODE_MAX, FALSE );//max item 5
            GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
            MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_IQ_DATA_MODE_A_SETTINGS,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
            MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_IQ_DATA_MODE_B_SETTINGS,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO); 
            MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_IQ_DATA_MODE_C_SETTINGS,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
            MMIAPISET_AppendListItemByTextIdExt(TXT_EXPORT_LOC_CUSTOM,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO); 

            cur_selection = iq_mode_param.cur_mode;

            //set current item
            GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;

        case MSG_FULL_PAINT:
            cur_selection = iq_mode_param.cur_mode;

            //set selected item
            GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            MMK_CreateWin((uint32*)MMIENG_IQ_DATA_MODE_SETTING_WIN_TAB, PNULL);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : EngIQDataPowerOnStartWinHandleMsg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E EngIQDataPowerOnStartWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();

    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
    
            MMISET_AppendOpenCloseListItem(ctrl_id,MMIAPIENG_GetIQPowerOnStartStatus);      
            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;

        case MSG_FULL_PAINT:
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            if (0 == cur_selection)
            {
                iq_mode_param.b_power_on_start = TRUE;
            }
            else
            {
                iq_mode_param.b_power_on_start = FALSE;
            }

            MMIAPIENG_SetIQMode(&iq_mode_param);
            //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : HandleIQDataModeSettingWinMsg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIQDataModeSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    uint16 cur_selection = 0;
    MMI_CTRL_ID_T ctrl_id = MMIENG_IQ_DATA_MODE_SETTING_CTRL_ID;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;

        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            GUIMENU_GetId(ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *)(&menu_id));
            switch (menu_id)
            {
                case ID_ENG_IQ_DATA_MODE_SETTING_ENABLE:
                    cur_selection = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);
                    iq_mode_param.cur_mode = cur_selection;

                    MMIAPIENG_SetIQMode(&iq_mode_param);
                    //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
                    MMK_CloseWin(win_id);
                    break;

                case ID_ENG_IQ_DATA_MODE_SETTING_FORMAT:
                    MMK_CreateWin((uint32*)MMIENG_IQ_DATA_MODE_SETTING_FORMAT_WIN_TAB, PNULL);
                    break;

                case ID_ENG_IQ_DATA_MODE_SETTING_CHANNEL:
                    MMK_CreateWin((uint32*)MMIENG_IQ_DATA_MODE_SETTING_CHANNEL_WIN_TAB, PNULL);
                    break;

                case ID_ENG_IQ_DATA_MODE_SETTING_TIME_LIMIT:
                    MMK_CreateWin((uint32*)MMIENG_IQ_DATA_MODE_SETTING_TIME_LIMIT_WIN_TAB, PNULL);
                    break;

                case ID_ENG_IQ_DATA_MODE_SETTING_FRAME_LENGTH:
                    MMK_CreateWin((uint32*)MMIENG_IQ_DATA_MODE_SETTING_FRAME_LENGTH_WIN_TAB, PNULL);
                    break;

                case ID_ENG_IQ_DATA_MODE_SETTING_BLOCK_COUNT:
                    MMK_CreateWin((uint32*)MMIENG_IQ_DATA_MODE_SETTING_BLOCK_COUNT_WIN_TAB, PNULL);
                    break;

/*[START] Jerry Liang for IQ MODE 2008/07/31*/        
           case ID_ENG_IQ_DATA_MODE_SETTING_ARFCN:
                    MMK_CreateWin((uint32*)MMIENG_IQ_DATA_MODE_SETTING_ARFCN_WIN_TAB, PNULL);
                    break;
/*[END] Jerry Liang for IQ MODE 2008/07/31*/                            

                default:                    
                    //SCI_TRACE_LOW:"MMIENG_WIN.C HandleIQDataModeSettingWinMsg menu_id=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_13003_112_2_18_2_18_8_200,(uint8*)"d", menu_id);
                    break;
            }
            break;

        case MSG_APP_CANCEL:
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

/*****************************************************************************/
//  Description : HandleIQDataModeSettingFormatWinMsg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIQDataModeSettingFormatWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    //GUI_RECT_T rect = MMITHEME_GetClientRect();
    MMI_RESULT_E result = MMI_RESULT_TRUE;
//     GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_CTRL_ID_T   ctrl_id = MMIENG_IQ_DATA_MODE_SETTING_FORMAT_CTRL_ID;
    uint16 cur_selection = 0;
    uint8 format_buf[10] = {0};
    wchar wstr[10] = {0};
    uint16 len = 0;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();
    MMI_STRING_T  edit_string = {0};
     

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            //GUIEDIT_SetRect(MMIENG_IQ_DATA_MODE_SETTING_FORMAT_CTRL_ID,&edit_rect);
            GUI_BOTH_RECT_T  edit_rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));   
             GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(ctrl_id), &edit_rect);
        }
        cur_selection = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);

        SCI_MEMSET(format_buf, 0, sizeof(format_buf));
        sprintf((char *)format_buf, "%d", iq_mode_param.record_param[cur_selection].format);

        len = strlen((char *)format_buf);
        MMI_STRNTOWSTR( wstr, 10, format_buf, len, len );
        
        GUIEDIT_SetString(
            ctrl_id,
            wstr,
            len
            );

        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_SINGLE);
        
        //LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
       // LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        cur_selection = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);
        GUIEDIT_GetString(ctrl_id, &edit_string);

        if (0 != edit_string.wstr_len)
        {
            SCI_MEMSET(format_buf, 0x00, sizeof(format_buf));
            MMI_WSTRNTOSTR(format_buf, 10, edit_string.wstr_ptr, edit_string.wstr_len, edit_string.wstr_len );

            iq_mode_param.record_param[cur_selection].format = atoi((char *)(format_buf));/*lint !e64*/

            if (AUD_REC_DSP_DATA_MIN <= iq_mode_param.record_param[cur_selection].format && AUD_REC_DSP_DATA_MAX >= iq_mode_param.record_param[cur_selection].format)
            {
                MMIAPIENG_SetIQMode(&iq_mode_param);

                MMK_CloseWin(win_id);
                
              //  MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_INPUT_ERROR);
            }
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_INPUT_ERROR);
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
//  Description : HandleIQDataModeSettingTimeLimitWinMsg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIQDataModeSettingTimeLimitWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    //GUI_RECT_T rect = MMITHEME_GetClientRect();
    MMI_RESULT_E result = MMI_RESULT_TRUE;
   // GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_CTRL_ID_T   ctrl_id = MMIENG_IQ_DATA_MODE_SETTING_TIME_LIMIT_CTRL_ID;
    uint16 cur_selection = 0;
    uint8 time_buf[10] = {0};
    wchar wstr[10] = {0};
    uint16 len = 0;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();
    MMI_STRING_T  edit_string = {0};
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
         {
            //GUIEDIT_SetRect(MMIENG_IQ_DATA_MODE_SETTING_FORMAT_CTRL_ID,&edit_rect);
            GUI_BOTH_RECT_T  edit_rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));   
             GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(ctrl_id), &edit_rect);
        }       

        cur_selection = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);

        SCI_MEMSET(time_buf, 0, sizeof(time_buf));
        sprintf((char *)time_buf, "%ld", (int32)iq_mode_param.record_param[cur_selection].time_limit);

        len = strlen((char *)time_buf);
        MMI_STRNTOWSTR( wstr, 10, time_buf, len, len );
        
        GUIEDIT_SetString(
            ctrl_id,
            wstr,
            len
            );
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_SINGLE);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        

        /*
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        break;
*/
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        cur_selection = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);
        GUIEDIT_GetString(ctrl_id, &edit_string);

        if (0 != edit_string.wstr_len)
        {
            SCI_MEMSET(time_buf, 0x00, sizeof(time_buf));
            MMI_WSTRNTOSTR(time_buf, 10, edit_string.wstr_ptr, edit_string.wstr_len, edit_string.wstr_len );

            iq_mode_param.record_param[cur_selection].time_limit = atoi((char *)(time_buf));

            MMIAPIENG_SetIQMode(&iq_mode_param);

          //  MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_INPUT_ERROR);
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
//  Description : HandleIQDataModeSettingFrameLengthWinMsg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIQDataModeSettingFrameLengthWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
//     GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_CTRL_ID_T   ctrl_id = MMIENG_IQ_DATA_MODE_SETTING_FRAME_LENGTH_CTRL_ID;
    uint16 cur_selection = 0;
    uint8 frame_buf[10] = {0};
    wchar wstr[10] = {0};
    uint16 len = 0;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();
    MMI_STRING_T  edit_string = {0};
 
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
             GUI_BOTH_RECT_T  edit_rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));   
             GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(ctrl_id), &edit_rect);
        }       
        
        cur_selection = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);

        SCI_MEMSET(frame_buf, 0, sizeof(frame_buf));
        sprintf((char *)frame_buf, "%ld", (int32)iq_mode_param.record_param[cur_selection].frame_length);
        
        len = strlen((char *)frame_buf);
        MMI_STRNTOWSTR( wstr, 10, frame_buf, len, len );
        
        GUIEDIT_SetString(
            ctrl_id,
            wstr,
            len
            );
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_SINGLE);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
/*
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        break;
*/
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        cur_selection = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);
        GUIEDIT_GetString(ctrl_id, &edit_string);

        if (0 != edit_string.wstr_len)
        {
            SCI_MEMSET(frame_buf, 0x00, sizeof(frame_buf));
            MMI_WSTRNTOSTR( frame_buf, 10, edit_string.wstr_ptr, edit_string.wstr_len, edit_string.wstr_len );

            iq_mode_param.record_param[cur_selection].frame_length = atoi((char *)(frame_buf));

            MMIAPIENG_SetIQMode(&iq_mode_param);

           // MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_INPUT_ERROR);
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
//  Description : HandleIQDataModeSettingBlockCountWinMsg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIQDataModeSettingBlockCountWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
//     GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_CTRL_ID_T   ctrl_id = MMIENG_IQ_DATA_MODE_SETTING_BLOCK_COUNT_CTRL_ID;
    uint16 cur_selection = 0;
    uint8 block_buf[10] = {0};
    wchar wstr[10] = {0};
    uint16 len = 0;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();
    MMI_STRING_T  edit_string = {0};
  
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
         {
            //GUIEDIT_SetRect(MMIENG_IQ_DATA_MODE_SETTING_FORMAT_CTRL_ID,&edit_rect);
            GUI_BOTH_RECT_T  edit_rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));   
            GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(ctrl_id), &edit_rect);
        }
       
        cur_selection = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);

        SCI_MEMSET(block_buf, 0, sizeof(block_buf));
        sprintf((char *)block_buf, "%d", iq_mode_param.record_param[cur_selection].block_count);

        len = strlen((char *)block_buf);
        MMI_STRNTOWSTR( wstr, 10, block_buf, len, len );

        GUIEDIT_SetString(
            ctrl_id,
            wstr,
            len
            );
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_SINGLE);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
/*
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        break;
*/
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        cur_selection = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);
        GUIEDIT_GetString(ctrl_id, &edit_string);

        if (0 != edit_string.wstr_len)
        {
            SCI_MEMSET(block_buf, 0x00, sizeof(block_buf));
            MMI_WSTRNTOSTR( block_buf, 10, edit_string.wstr_ptr, edit_string.wstr_len, edit_string.wstr_len );

            iq_mode_param.record_param[cur_selection].block_count = atoi((char *)(block_buf));

            MMIAPIENG_SetIQMode(&iq_mode_param);

          //  MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_INPUT_ERROR);
        }
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}


/********************************************************************************
 NAME:          HandleIQDataModeSettingARFCNWinMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        Jerry Liang
 DATE:          2008.07.31
********************************************************************************/
LOCAL MMI_RESULT_E HandleIQDataModeSettingARFCNWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{   
    MMI_RESULT_E result = MMI_RESULT_TRUE;
//     GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_CTRL_ID_T   ctrl_id = MMIENG_IQ_DATA_MODE_SETTING_ARFCN_CTRL_ID;
    uint16 cur_selection = 0;
    uint8 arfcn_buf[10] = {0};
    wchar wstr[10] = {0};
    uint16 len = 0;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();
    MMI_STRING_T  edit_string = {0};
 
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
          {
             GUI_BOTH_RECT_T  edit_rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));   
             GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(ctrl_id), &edit_rect);
        }
       
        cur_selection = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);

        SCI_MEMSET(arfcn_buf, 0, sizeof(arfcn_buf));
        sprintf((char *)arfcn_buf, "%d", iq_mode_param.record_param[cur_selection].arfcn);

        len = strlen((char *)arfcn_buf);
        MMI_STRNTOWSTR( wstr, 10, arfcn_buf, len, len );

        GUIEDIT_SetString(
            ctrl_id,
            wstr,
            len
            );
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_SINGLE);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
/*
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        break;
*/
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        cur_selection = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);
        GUIEDIT_GetString(ctrl_id, &edit_string);

        if (0 != edit_string.wstr_len)
        {
            SCI_MEMSET(arfcn_buf, 0x00, sizeof(arfcn_buf));
            MMI_WSTRNTOSTR(arfcn_buf, 10, edit_string.wstr_ptr, edit_string.wstr_len, edit_string.wstr_len);

            iq_mode_param.record_param[cur_selection].arfcn = atoi((char *)(arfcn_buf));

            MMIAPIENG_SetIQMode(&iq_mode_param);

           // MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_INPUT_ERROR);
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
//  Description : HandleIQDataModeSettingChannelWinMsg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIQDataModeSettingChannelWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_IQ_DATA_MODE_SETTING_CHANNEL_CTRL_ID;
    uint16 cur_selection = 0;
    uint16 index = 0;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();

    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(ctrl_id, RECORD_OUTPUT_MAX, FALSE);//max item 2
            GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
            //add items to listbox
            MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_IQ_DATA_CHANNEL_FILE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
            MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_IQ_DATA_CHANNEL_ARM_LOG,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO); 

            GUILIST_SetItemGray(ctrl_id, RECORD_OUTPUT_ARM_LOG, TRUE);

            index = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);
            cur_selection = iq_mode_param.record_param[index].output_channel;

            //set selected item
            GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
            
            //set current item
            GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;

        case MSG_FULL_PAINT:
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            if (!GUILIST_IsCurrItemInvalid(ctrl_id))
            {
                index = GUILIST_GetCurItemIndex(MMIENG_IQ_DATA_MODE_SELECT_CTRL_ID);
                cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
                if (0 == cur_selection)
                {
                    iq_mode_param.record_param[index].output_channel = RECORD_OUTPUT_SAVE_TO_FILE;
                }
                else
                {
                    iq_mode_param.record_param[index].output_channel = RECORD_OUTPUT_ARM_LOG;
                }

                MMIAPIENG_SetIQMode(&iq_mode_param);
              //  MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
                MMK_CloseWin(win_id);
            }
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : MMIAPIENG_AlertIQMode
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_AlertIQMode(void)
{
    //提示IQ模式下不能录音
    MMIPUB_OpenAlertWarningWin(TXT_ENG_IQ_MODE_WARNING);
}

/*****************************************************************************/
//  Description : HandleIQDataQueryWin
//  Global resource dependence :                                
//      Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIQDataQueryWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    static BOOLEAN is_succeed = FALSE;
    static BOOLEAN is_need_add_to_fs = FALSE;
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            is_need_add_to_fs = TRUE;
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;

        case MSG_ENG_IQ_RESULT_CNF:
            is_succeed = *(BOOLEAN *)param;
            break;

        case MSG_UDISK_FORMAT_CNF:
            //MMIPUB_CloseQuerytWin(PNULL);
            MMIPUB_CloseAlertWin();
            MMIAPIUdisk_HandleFormatCnf(*(BOOLEAN *)param);
            MMIAPIENG_StartIQData();
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            is_need_add_to_fs = FALSE;
#ifdef MMI_RECORD_SUPPORT
            MMIPUB_OpenAlertWaitingWin(TXT_COMMON_WAITING);
            
            MMIAPIFMM_FormatDeviceAsyn((wchar *)MMIAPIFMM_GetDevicePath(MMIAPIRECORD_GetRecordStorage()), 
                                      MMIAPIFMM_GetDevicePathLen(MMIAPIRECORD_GetRecordStorage()), 
                                      MMIFILE_FORMAT_FAT16,
                                      win_id,
                                      MSG_UDISK_FORMAT_CNF
                                     );   

            MMK_CloseWin(win_id);
#endif            
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            is_need_add_to_fs = FALSE;
            MMIPUB_CloseQuerytWin(PNULL);
#ifdef MMI_RECORD_SUPPORT     
			MMIENG_DeletRecordFile();
			if (!is_succeed)
			{		
				MMIPUB_OpenAlertFailWin(TXT_ERROR);    
			}
#endif            
            iq_mode_param.b_is_start = FALSE;
            MMIAPIENG_SetIQMode(&iq_mode_param);
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
                        
            if (is_need_add_to_fs)
            {
                is_need_add_to_fs = FALSE;
#ifdef MMI_RECORD_SUPPORT 
				MMIENG_DeletRecordFile();
				if (!is_succeed)
				{			
					MMIPUB_OpenAlertFailWin(TXT_ERROR);      
				}
#endif                
                iq_mode_param.b_is_start = FALSE;
                MMIAPIENG_SetIQMode(&iq_mode_param);
            }
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;

        default:
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
    }

    return result;
}

/*****************************************************************************/
//  Description : Open IQ Data Query Win
//  Global resource dependence :                                
//      Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_OpenHandleIQDataQueryWin(BOOLEAN is_succeed)
{
    MMI_WIN_ID_T query_win_id = MMIENG_IQ_DATA_HANDLE_QUERY_WIN_ID;
    MMIPUB_CloseQuerytWin(PNULL);
    MMIPUB_OpenQueryWinByTextId(TXT_ENG_IQ_DATA_RECORD_AGAIN, IMAGE_PUBWIN_QUERY, &query_win_id, HandleIQDataQueryWin);
    MMK_PostMsg(query_win_id, MSG_ENG_IQ_RESULT_CNF, &is_succeed, sizeof(BOOLEAN));

    return TRUE;
    //return MMK_CreateWin((uint32 *)MMIENG_HANDLE_QUERY_WIN_TAB, (ADD_DATA) is_suc_ptr);
}

/*****************************************************************************/
//  Description : is arm log on when power on
//  Global resource dependence :                                
//      Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsUSBLOGON(void)
{
    BOOLEAN is_on = FALSE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
     MMINV_READ(MMINV_ENG_USBLOG,&is_on,return_value);        
    if(MN_RETURN_SUCCESS!=return_value)
    {
        is_on = FALSE;      
        MMINV_WRITE(MMINV_ENG_USBLOG,&is_on);
    }    
    SCI_TRACE_LOW("MMIAPIENG_IsUSBLOGON is_on = %d", is_on);// @cr239352 renwei add
    return is_on;
}

#ifdef PCLINK_SUPPORT
/*****************************************************************************/
//  Description : HandlePClinkEnableWinMsg
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePClinkEnableWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_PCLINK_ENABLE_CTRL_ID;
    uint16 cur_selection = 0;
    //    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
#ifndef WIN32
        MNGPRS_GetPclinkCfg(&s_pclink_config_info);
#endif
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);     
        
        if (s_pclink_config_info.is_pclink_open)
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;        
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (0 == cur_selection)
        {
            s_pclink_config_info.is_pclink_open = TRUE;
        }
        else
        {
            s_pclink_config_info.is_pclink_open = FALSE;
        }
        
        //设置NV项
#ifndef WIN32
        MNGPRS_SetPclinkCfg(&s_pclink_config_info);
#endif
        MMIPUB_OpenAlertSuccessWin(TXT_ENG_SUCCESS_NEED_RESET);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : HandlePClinkInputDNSWinMsg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePClinkInputIPAddrWinMsg(
                                                 MMI_WIN_ID_T   win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_PCLINK_INPUT_IPADDR_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef WIN32
        MNGPRS_GetPclinkCfg(&s_pclink_config_info);
#endif
        GUIEDIT_SetIP (ctrl_id,s_pclink_config_info.ipaddr);
        MMK_SetAtvCtrl(win_id,ctrl_id);
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
                MMK_SendMsg(win_id,MSG_APP_OK, PNULL);

                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        s_pclink_config_info.ipaddr = GUIEDIT_GetIP(ctrl_id);
        //设置NV项
#ifndef WIN32
        MNGPRS_SetPclinkCfg(&s_pclink_config_info);
#endif      
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    default:    
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
    
}

/*****************************************************************************/
//  Description : HandlePClinkInputSNMaskWinMsg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePClinkInputSNMaskWinMsg(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_PCLINK_INPUT_SNMASK_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef WIN32
        MNGPRS_GetPclinkCfg(&s_pclink_config_info);
#endif
        GUIEDIT_SetIP (ctrl_id,s_pclink_config_info.snmask);
        MMK_SetAtvCtrl(win_id,ctrl_id);
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
                MMK_SendMsg(win_id,MSG_APP_OK, PNULL);

                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        s_pclink_config_info.snmask = GUIEDIT_GetIP(ctrl_id);
        //设置NV项
#ifndef WIN32
        MNGPRS_SetPclinkCfg(&s_pclink_config_info);
#endif
        MMK_CloseWin(win_id);       
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    default:    
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
    
}

/*****************************************************************************/
//  Description : HandlePClinkInputDefGWWinMsg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePClinkInputDefGWWinMsg(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_PCLINK_INPUT_DEFGW_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef WIN32
      MNGPRS_GetPclinkCfg(&s_pclink_config_info);
#endif
        GUIEDIT_SetIP (ctrl_id,s_pclink_config_info.defgw);
        MMK_SetAtvCtrl(win_id,ctrl_id);
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
                MMK_SendMsg(win_id,MSG_APP_OK, PNULL);

                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        s_pclink_config_info.defgw = GUIEDIT_GetIP(ctrl_id);
        //设置NV项
#ifndef WIN32
        MNGPRS_SetPclinkCfg(&s_pclink_config_info);
#endif
        MMK_CloseWin(win_id);       
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    default:    
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
    
}

/*****************************************************************************/
//  Description : HandlePClinkInputDNSWinMsg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePClinkInputDNSWinMsg(
                                              MMI_WIN_ID_T  win_id, 
                                              MMI_MESSAGE_ID_E  msg_id, 
                                              DPARAM                param
                                              )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_PCLINK_INPUT_DNS_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef WIN32
        MNGPRS_GetPclinkCfg(&s_pclink_config_info);
#endif
        GUIEDIT_SetIP (ctrl_id,s_pclink_config_info.dns);
        MMK_SetAtvCtrl(win_id,ctrl_id);
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
                MMK_SendMsg(win_id,MSG_APP_OK, PNULL);

                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        s_pclink_config_info.dns = GUIEDIT_GetIP(ctrl_id);
        //设置NV项
#ifndef WIN32
        MNGPRS_SetPclinkCfg(&s_pclink_config_info);
#endif      
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    default:    
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
    
}

/*****************************************************************************/
//  Description : handle message of input test address window of pc link
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePCLinkInputTestAdfressWinMsg(
                                             MMI_WIN_ID_T   win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
//     MMI_STRING_T string_info = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIENG_PCLINK_INPUT_TEST_ADDRESS_CTRL_ID;
    uint32 pclink_test_addr = 0;
    MN_RETURN_RESULT_E nv_result =MN_RETURN_SUCCESS;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //从NV中读出来
        MMINV_READ(MMIENG_NV_PCLINK_TEST_ADDR_ID, &pclink_test_addr, nv_result);
        if (MN_RETURN_SUCCESS != nv_result)
        {
            SCI_MEMSET(&pclink_test_addr,0,sizeof(uint32));
            MMINV_WRITE(MMIENG_NV_PCLINK_TEST_ADDR_ID, &pclink_test_addr);
        }
        
        if (0 == pclink_test_addr)//若test address为空，则直接设置默认的gateway地址
        {
            GUIEDIT_SetIP (ctrl_id,s_pclink_config_info.defgw);
            
        }
        else
        {
            GUIEDIT_SetIP (ctrl_id,pclink_test_addr);
        }
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
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
                MMK_SendMsg(win_id,MSG_APP_OK, PNULL);

                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        pclink_test_addr = GUIEDIT_GetIP(ctrl_id);      
        //存入NV
        MMINV_WRITE(MMIENG_NV_PCLINK_TEST_ADDR_ID, &pclink_test_addr);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    default:        
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
    
}

/*****************************************************************************/
//  Description : start pingtest for pc link
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIENG_StartPingTest(void)
{
    MMI_STRING_T    ping_test_text = {0};
    // 显示“Ping test start ...”对话框
    MMI_GetLabelTextByLang(TXT_ENG_PCLINK_PING_TEST_START, &ping_test_text);
    MMIPUB_OpenWaitWin(1,&ping_test_text,PNULL,PNULL,MMIENG_PCLINK_PING_TEST_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandlePCLinkPingTestWaitWinMsg);
}

/* ping request handle */
TCPIP_PING_HANDLE    g_pclink_ping_handle = TCPIP_PING_HANDLE_NULL;
TCPIP_PING_RESULT_E  g_pclink_ping_res = PINGRES_SUCCESS;

/*****************************************************************************/
//  Description : start pingtest for pc link
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePCLinkPingTestWaitWinMsg(
                                                 MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                 DPARAM             param       // 相应消息的参数
                                                 )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint8 addr[MMIENG_PCLINK_MAX_IP_LEN + 1] = {0};
    uint32 pclink_test_addr = 0;
    MN_RETURN_RESULT_E nv_result = MN_RETURN_SUCCESS;
    
    switch (msg_id)
    {   
    case MSG_OPEN_WINDOW:
        //从NV中读出来
        MMINV_READ(MMIENG_NV_PCLINK_TEST_ADDR_ID, &pclink_test_addr, nv_result);
        if (MN_RETURN_SUCCESS != nv_result)
        {
            SCI_MEMSET(&pclink_test_addr,0,sizeof(uint32));
            MMINV_WRITE(MMIENG_NV_PCLINK_TEST_ADDR_ID, &pclink_test_addr);
        }
#ifndef WIN32
        TCPIPPCLINK_SetNetInterface();   // 设置PCLINK网络接口
#endif
        MMIAPICONNECTION_IpNumToStr(pclink_test_addr, &addr);/*lint !e64*/

        // send ping request
        g_pclink_ping_handle = sci_async_ping((char*)addr, 0, 0, SCI_IdentifyThread(), 0);
        if( TCPIP_PING_HANDLE_NULL == g_pclink_ping_handle )
        {
            MMK_CloseWin(win_id);
            MMIPUB_OpenAlertWinByTextId(
                NULL,TXT_ENG_PCLINK_PING_TEST_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

        }
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;

    case MSG_APP_CANCEL://取消ping test的处理
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        sci_ping_cancel(g_pclink_ping_handle);
        // 弹出“Ping test canceled!”对话框    
        MMIPUB_OpenAlertWinByTextId(NULL,TXT_ENG_PCLINK_PING_TEST_CANCELED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_ENG_PCLINK_PING_SUC:
        {  
            MMK_CloseWin(win_id);
            // 弹出“Ping test OK!”对话框
            MMIPUB_OpenAlertWinByTextId(NULL,TXT_ENG_PCLINK_PING_TEST_OK,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }       
        break;

    case MSG_ENG_PCLINK_PING_FAIL:
        {
            MMK_CloseWin(win_id);        
            // 显示“Ping test error!”对话框
            switch(g_pclink_ping_res)
            {
            case PINGRES_ICMP_TIMEOUT:
            case PINGRES_DNS_TIMEOUT:
                MMIPUB_OpenAlertWinByTextId(
                    NULL,TXT_ENG_PCLINK_PING_TEST_TIMEOUT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;

            case PINGRES_DNS_ERROR:
            case PINGRES_ICMP_ERROR:
            default:
                MMIPUB_OpenAlertWinByTextId(
                    NULL,TXT_ENG_PCLINK_PING_TEST_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
            }
         }
        break;

    case MSG_CLOSE_WINDOW:
#ifndef WIN32
        TCPIPPCLINK_ResetNetInterface();   // 清除PCLINK网络接口
#endif
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);        
        break;      
    }
    return result;
}
#endif

#ifdef  CAMERA_SUPPORT

/********************************************************************************
 NAME:          HandleDCParamScreenSizeWinMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        zhaohui
 DATE:          2008.12.30
********************************************************************************/
LOCAL MMI_RESULT_E HandleDCParamScreenSizeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16                  index = 0;
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T           text_id_tab[] = {TXT_ENG_DC_176_220, TXT_ENG_DC_240_320, TXT_ENG_DC_220_400};
    int16                   item_total_num = ARR_SIZE(text_id_tab);
    MMI_CTRL_ID_T           list_id = MMIENG_LISTBOX_CTRL_ID;
    CAMERA_ENG_LCD_SIZE_MODE_E  mode = CAMERA_ENG_LCD_176X220;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(list_id, item_total_num, FALSE );//max item 3
        GUILIST_SetOwnSofterKey(list_id,TRUE);
        AppendListItem(
            list_id, 
            text_id_tab, 
            item_total_num
            );
        // #ifndef WIN32        
        mode = MMIAPIDC_GetLCDSizeMode();
        // #endif        
        //SCI_TRACE_LOW:"MMIENG_MAIN.C HandleDCParamScreenSizeWinMsg mode is %d,item_total_num is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_14287_112_2_18_2_18_10_201,(uint8*)"dd",mode,item_total_num);
        if((int16)mode >=  item_total_num)
        {
            break;
        }
        GUILIST_SetSelectedItem(list_id, (uint16)mode, TRUE);
        GUILIST_SetCurItemIndex(list_id, (int16)mode);
        MMK_SetAtvCtrl(win_id, list_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);
        //SCI_TRACE_LOW:"MMIENG_MAIN.C HandleDCParamScreenSizeWinMsg mode is %d,item_total_num is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_14310_112_2_18_2_18_10_202,(uint8*)"dd",index,item_total_num);
        if((int16)index >=  item_total_num)
        {
            break;
        }
        
        MMIAPIDC_SetLCDSizeMode((CAMERA_ENG_LCD_SIZE_MODE_E)index);
        MMIAPIDC_TurnOnEngTestMode();
        //success!
       // MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : HandleDCParamScreenSwitchWinMsg
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCParamScreenSwitchWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    MMI_TEXT_ID_T           text_id_tab[] = {TXT_ENG_DC_ACROSS_SCREEN, TXT_ENG_DC_UPRIGHT_SCREEN};
    int16                   item_total_num = ARR_SIZE(text_id_tab);
    CAMERA_ENG_PREVIEW_MODE_E mode = CAMERA_ENG_PREVIEW_HOR_MODE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, item_total_num, FALSE );//max item 3
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        AppendListItem(
            ctrl_id, 
            text_id_tab, 
            item_total_num
            );
        
//#ifndef WIN32
        mode = MMIAPIDC_GetPreviewMode();
        if (CAMERA_ENG_PREVIEW_HOR_MODE == mode)
        {
            cur_selection = 0;
        }   
        else
        {
            cur_selection = 1;
        }
//#endif    
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

        if(DCAMERA_IMG_SENSOR_TYPE_3P4 == MMIAPIDC_GetDCModulSensorTypeById(SETTING_SENSOR_REAR)
            || DCAMERA_IMG_SENSOR_TYPE_3P4 == MMIAPIDC_GetDCModulSensorTypeById(SETTING_SENSOR_FRONT)
            )
        {
            GUILIST_SetItemGray(ctrl_id, 0, TRUE);
            GUILIST_SetItemGray(ctrl_id, 1, TRUE);
        }
            
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;      
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
//#ifndef WIN32
        if (0 == cur_selection)
        {
            mode = CAMERA_ENG_PREVIEW_HOR_MODE;
        }
        else
        {
            mode = CAMERA_ENG_PREVIEW_VER_MODE;
        }
        if (CheckIfDCScreenSwitchPrevSetSupported(mode,MMIAPIDC_GetVisualMode()))/*lint !e64*/
        {
            MMIAPIDC_SetPreviewMode(mode);
//#endif
            MMIAPIDC_TurnOnEngTestMode();
          //  MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_FACILITY_NOT_SUPPORTED);
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : HandleDCParamScreenPrevSetWinMsg
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCParamScreenPrevSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    MMI_TEXT_ID_T           text_id_tab[] = { TXT_ENG_DC_FULL_VIEW,TXT_ENG_DC_FULL_SCREEN };
    int16                   item_total_num = ARR_SIZE(text_id_tab);
    CAMERA_ENG_VISUAL_MODE_E mode = CAMERA_ENG_FULL_VISUAL_ANGLE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, item_total_num, FALSE );//max item 3
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        AppendListItem(
            ctrl_id, 
            text_id_tab, 
            item_total_num
            );
        
//#ifndef WIN32
        mode = MMIAPIDC_GetVisualMode();
        if (CAMERA_ENG_FULL_VISUAL_ANGLE == mode)//全视角
        {
            cur_selection = 0;
        }
        else//全屏幕
        {
            cur_selection = 1;
        }
//#endif    
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;
                
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
//#ifndef WIN32
        if (0 == cur_selection)
        {
            mode = CAMERA_ENG_FULL_VISUAL_ANGLE;
        }
        else
        {
            mode = CAMERA_ENG_FULL_SCREEN;
        }
        if (CheckIfDCScreenSwitchPrevSetSupported(MMIAPIDC_GetPreviewMode(),mode))/*lint !e64*/
        {
            MMIAPIDC_SetVisualMode(mode);
//#endif
            MMIAPIDC_TurnOnEngTestMode();
           // MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_FACILITY_NOT_SUPPORTED);
        }       
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : Handle DCParam Sensor Angle Win Msg
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCParamSensorAngleWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    MMI_TEXT_ID_T           text_id_tab[] =
    { TXT_SENSOR_0,TXT_SENSOR_90,TXT_SENSOR_180,TXT_SENSOR_270,
        TXT_SENSOR_MIRROR,TXT_SENSOR_FLIP
    };
    int16                   item_total_num = ARR_SIZE(text_id_tab);
    DCAMERA_ROTATION_MODE_E     angle = 0;
    static SETTING_SENSOR_ID_E s_dc_sensor_id = 0;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        s_dc_sensor_id = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/

        //SCI_TRACE_LOW:"mmieng HandleDCParamSensorAngleWinMsg s_dc_sensor_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_14534_112_2_18_2_18_11_203,(uint8*)"d",s_dc_sensor_id);
        
        GUILIST_SetMaxItem(ctrl_id, item_total_num, FALSE );//max item 3
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        AppendListItem(
            ctrl_id, 
            text_id_tab, 
            item_total_num
            );
            
        angle = MMIAPIDC_GetDCModuleSensorAngle(s_dc_sensor_id);
        switch(angle)
        {
        case DCAMERA_ROTATION_0:
            cur_selection = 0;
            break;
        case DCAMERA_ROTATION_90:
            cur_selection = 1;
            break;
        case DCAMERA_ROTATION_180:
            cur_selection = 2;
            break;
        case DCAMERA_ROTATION_270:
            cur_selection = 3;
            break;
        case DCAMERA_ROTATION_MIRROR:
            cur_selection = 4;
            break;
        case DCAMERA_ROTATION_FLIP:
            cur_selection = 5;
            break;
        default:
            cur_selection = 0;
            break;
        }
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;
                
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        switch(cur_selection)
        {
        case 0:
            angle = DCAMERA_ROTATION_0;
            break;
        case 1:
            angle = DCAMERA_ROTATION_90;
            break;
        case 2:
            angle = DCAMERA_ROTATION_180;
            break;
        case 3:
            angle = DCAMERA_ROTATION_270;
            break;        
        case 4:
            angle = DCAMERA_ROTATION_MIRROR;
            break;
        case 5:
            angle = DCAMERA_ROTATION_FLIP;
            break;
        default:
            angle = DCAMERA_ROTATION_0;
            break;
        }

        MMIAPIDC_SetDCModuleSensorAngle(angle, s_dc_sensor_id);
        MMIAPIDC_TurnOnEngTestMode();
       // MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : Handle DCParam Sensor ID Win Msg
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCParamSensorIDWinMsg(MMI_WIN_ID_T win_id,
                                                          MMI_MESSAGE_ID_E msg_id,
                                                          DPARAM param
                                                          )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = 0;
    int32 sensor_number = 0;
    MMI_TEXT_ID_T  text_id_tab[] =
    {
        TXT_POSTPOSITIVE_SENSOR,
        TXT_PREPOSITIVE_SENSOR
    };
    int16 item_total_num = ARR_SIZE(text_id_tab);
    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, item_total_num, FALSE );//max item 3
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        AppendListItem(
            ctrl_id, 
            text_id_tab, 
            item_total_num
            );
            
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

        sensor_number = MMIAPIDC_GetDCModuleSensorNumber();
        if (sensor_number == MAX_MMI_SENSOR_NUM)
        {
            sensor_type = MMIAPIDC_GetDCModulSensorTypeById(SETTING_SENSOR_REAR);
            if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
            {
                GUILIST_SetItemGray(ctrl_id, 0, TRUE);
            }
            sensor_type = MMIAPIDC_GetDCModulSensorTypeById(SETTING_SENSOR_FRONT);
            if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
            {
                GUILIST_SetItemGray(ctrl_id, 1, TRUE);
            }
        } 
        else
        {
            sensor_type = MMIAPIDC_GetDCModulSensorTypeById(SETTING_SENSOR_REAR);
            if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
            {
                GUILIST_SetItemGray(ctrl_id, 0, TRUE);
            }
            GUILIST_SetItemGray(ctrl_id, 1, TRUE);
        }
            
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;
                
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        MMK_CreateWin((uint32*)MMIENG_DC_PARAM_SENSOR_ANGLE_WIN_TAB, (ADD_DATA)cur_selection);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

#endif

/*****************************************************************************/
//  Description : CheckIfDCScreenSwitchPrevSupported
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_SaveLastTime(void)
{ 
     SCI_DATE_T  cur_date={0};
     SCI_TIME_T   cur_time={0};
     
     TM_GetSysDate(&cur_date);
     TM_GetSysTime(&cur_time);

     MMINV_WRITE(MMIENG_NV_LAST_DATA_ID, &cur_date);    
     MMINV_WRITE(MMIENG_NV_LAST_TIME_ID, &cur_time);    
}

#ifdef  CAMERA_SUPPORT
/*****************************************************************************/
//  Description : CheckIfDCScreenSwitchPrevSupported
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckIfDCScreenSwitchPrevSetSupported(
                                                CAMERA_ENG_VISUAL_MODE_E switch_mode,
                                                CAMERA_ENG_PREVIEW_MODE_E prev_mode
                                                )
{
    BOOLEAN     result = TRUE;

#ifdef MAINLCD_SIZE_240X400
    #if defined (PLATFORM_SC6600L) || defined (PLATFORM_SC6600R)
        if ((CAMERA_ENG_PREVIEW_HOR_MODE ==prev_mode)
            && (CAMERA_ENG_FULL_VISUAL_ANGLE == switch_mode))//wqvga版本,6600R,6600L不支持横屏全视角 
        {
            result = FALSE;
        }   
    #endif
#else
    #ifdef PLATFORM_SC6600R
        if ((CAMERA_ENG_PREVIEW_VER_MODE ==prev_mode)
            && (CAMERA_ENG_FULL_VISUAL_ANGLE == switch_mode))//6600R不支持竖屏全视角 
        {
            result = FALSE;
        }
    #endif
#endif

    return result;
}
#endif
/*****************************************************************************/
//  Description : MMIAPIENG_OpenUserAgentWin
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenUserAgentWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_UA_AGENT_WIN_TAB, PNULL);
}
/*****************************************************************************/
//  Description : MMIAPIENG_OpenUaProfileWin
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenUaProfileWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_UA_PROFILE_WIN_TAB, PNULL);
}
/*****************************************************************************/
//  Description : EngUaAgentOnOffWinHandleMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E EngUaAgentOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{    
    int32 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    int32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};    
            
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_UA_AGENT, FALSE);
            GUILIST_SetMaxItem( list_id, 2, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
            
            item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );                      

            index = (MMIENG_GetUserAgentIsUse())? 0: 1;
            GUILIST_SetSelectedItem( list_id,index, TRUE );
            GUILIST_SetCurItemIndex( list_id, index);          
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);    
        MMIENG_SetUserAgentIsUse((0 == index)? TRUE: FALSE);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);    
}

/*****************************************************************************/
//  Description : HandleUaAgentWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaAgentWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 group_id = 0;
    uint32 menu_id = 0;
    MMI_CTRL_ID_T ctrl_id = MMIENG_UA_SUB_MENU_CTRL_ID;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
    {
        MMI_STRING_T title_string = {0};
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, FALSE);
        MMI_GetLabelTextByLang(TXT_ENG_UA_AGENT, &title_string);
        GUIMENU_SetMenuTitle(&title_string, ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
    }
        break;
			
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB: 
        GUIMENU_GetId(MMIENG_UA_SUB_MENU_CTRL_ID, &group_id, &menu_id);
        switch (menu_id)
        {
        case ID_ENG_UA_SET_ONOFF:
#ifdef ENG_SUPPORT
            MMIENG_OpenListSelectWin((uint32)EngUaAgentOnOffWinHandleMsg);
#endif
            break;
            
        case ID_ENG_UA_SET:
            MMK_CreateWin((uint32*)MMIENG_UA_AGENT_SET_WIN_TAB, PNULL);
            break;
            
        case ID_ENG_UA_SET_SHOW:
            MMK_CreateWin((uint32*)MMIENG_UA_AGENT_SHOW_WIN_TAB, PNULL);
            break;
            
        default:
            break;
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
/*****************************************************************************/
//  Description : HandleUaAgentWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaAgentSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_UA_SELECT_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    BOOLEAN is_use_list = FALSE;

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
    {
        MMI_TEXT_ID_T   text_id_tab[] = {TXT_ENG_UA_LIST,TXT_ENG_UA_EDIT };
        uint16  item_total_num = ARR_SIZE(text_id_tab);
        
        GUILIST_SetMaxItem(ctrl_id, item_total_num, FALSE );//max item 3
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        AppendListItem(ctrl_id, text_id_tab, item_total_num);
        is_use_list = MMIENG_GetUserAgentIsUseList();
        cur_selection = (is_use_list)? 0: 1;
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        MMK_SetAtvCtrl(win_id,ctrl_id);
    }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;
                
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (0 == cur_selection)
        {
            is_use_list = TRUE;
            MMK_CreateWin((uint32*)MMIENG_UA_AGENT_LIST_WIN_TAB, PNULL);
        }
        else
        {
            is_use_list = FALSE;
            MMK_CreateWin((uint32*)MMIENG_UA_AGENT_EDIT_WIN_TAB, PNULL);
        }
        MMIENG_SetUserAgentIsUseList(is_use_list);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : HandleUaAgentListWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaAgentListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_UA_SELECT_LIST_NUM_CTRL_ID;
    uint32                 list_num = 0;

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
    {
        MMI_TEXT_ID_T   text_id_tab[] = 
                {
                TXT_ENG_UA_NONE ,
                TXT_ENG_UA_DORADO,
                TXT_ENG_UA_MTK,
                TXT_ENG_UA_09A,
                TXT_ENG_UA_NOKIA,
                TXT_ENG_UA_NF
                };
        uint16  item_total_num = ARR_SIZE(text_id_tab);
    
        GUILIST_SetMaxItem(ctrl_id, item_total_num, FALSE );//max item 3
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        AppendListItem(ctrl_id, text_id_tab, item_total_num);
        list_num = MMIENG_GetUserAgentListNum();
        GUILIST_SetSelectedItem(ctrl_id, list_num, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,list_num);
        MMK_SetAtvCtrl(win_id,ctrl_id);
    }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;
                
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    {
        MMIENG_UA_T     ua_set={0};

        list_num = GUILIST_GetCurItemIndex(ctrl_id);
        MMIENG_SetUserAgentListNum(list_num);
        SCI_MEMSET(&ua_set,0,sizeof(MMIENG_UA_T));
        MMIENG_GetUa(&ua_set);
        SCI_MEMSET(ua_set.user_agent,0,MMIENG_MAX_URL_LEN);
        switch(list_num) 
        {
        case 1:
            strncpy((char *)ua_set.user_agent,MMIENG_USER_AGENT_DORADO,MIN(MMIENG_MAX_URL_LEN,strlen(MMIENG_USER_AGENT_DORADO)));   /*lint !e666*/
            break;
        case 2:
            strncpy((char *)ua_set.user_agent,MMIENG_USER_AGENT_MTK,MIN(MMIENG_MAX_URL_LEN,strlen(MMIENG_USER_AGENT_MTK)));         /*lint !e666*/
            break;
        case 3:
            strncpy((char *)ua_set.user_agent,MMIENG_USER_AGENT_09A,MIN(MMIENG_MAX_URL_LEN,strlen(MMIENG_USER_AGENT_09A)));         /*lint !e666*/
            break;
        case 4:
            strncpy((char *)ua_set.user_agent,MMIENG_USER_AGENT_NOKIA,MIN(MMIENG_MAX_URL_LEN,strlen(MMIENG_USER_AGENT_NOKIA)));     /*lint !e666*/
            break;
        case 5:
            strncpy((char *)ua_set.user_agent,MMIENG_USER_AGENT_NF,MIN(MMIENG_MAX_URL_LEN,strlen(MMIENG_USER_AGENT_NF)));           /*lint !e666*/
            break;
        default:
            break;
        }
        MMIENG_SetUa(&ua_set);
        MMK_CloseWin(win_id);
    }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : HandleUaAgentShowWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaAgentShowWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;  
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:   
    {
        wchar wstr[1024] = {0};
        MMIENG_UA_T     ua_set={0};
        uint32 length = 0; 
        char    *ua_user_agent_ptr = PNULL;

        MMIENG_GetUa(&ua_set);
        length = SCI_STRLEN((char *)ua_set.user_agent);
        if (MMIENG_GetUserAgentIsUse() && (length > 0))//use the eng's user_agent
        {
            ua_user_agent_ptr = (char *)ua_set.user_agent;
        }
        else//use the default
        {
            ua_user_agent_ptr = MMIAPICOM_GetUserAgent();
            length = SCI_STRLEN(ua_user_agent_ptr);
        }
        
        if (length > 0)
        {
            MMI_STRNTOWSTR(wstr, 1023, 
                (uint8*)ua_user_agent_ptr, length, 
                MIN(length, 1023));
                
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, wstr, length, FALSE);           
        }

        MMK_SetAtvCtrl(win_id,MMIENG_TEXTBOX1_CTRL_ID);
    }
        break;
        
    case MSG_CTL_MIDSK:     
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:    
    case MSG_KEYDOWN_OK:        
    case MSG_KEYDOWN_CANCEL:
    case MSG_APP_WEB:
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
/*****************************************************************************/
//  Description : EngUaProfileOnOffWinHandleMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E EngUaProfileOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{    
    int32 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    int32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};    
            
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_UA_PROFILE, FALSE);
            GUILIST_SetMaxItem( list_id, 2, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
            
            item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );                      

            index = (MMIENG_GetUaProfileIsUse())? 0: 1;
            GUILIST_SetSelectedItem( list_id,index, TRUE );
            GUILIST_SetCurItemIndex( list_id, index);          
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);    
        MMIENG_SetUaProfileIsUse((0 == index)? TRUE: FALSE);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);    
}

/*****************************************************************************/
//  Description : HandleUaProfileShowWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaProfileShowWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;  
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:   
    {
        wchar wstr[1024] = {0};
        MMIENG_UA_T     ua_set={0};
        uint32 length = 0; 
        char    *ua_profile_ptr = PNULL;

        MMIENG_GetUa(&ua_set);
        length = SCI_STRLEN((char *)ua_set.uaprofile);
        if (MMIENG_GetUaProfileIsUse() && (length > 0))//use the eng's ua profile
        {
            ua_profile_ptr = (char *)ua_set.uaprofile;
        }
        else//use the default
        {
            ua_profile_ptr = MMIAPICOM_GetUserAgentProfile();
            length = SCI_STRLEN(ua_profile_ptr);
        }
        
        if (length > 0)
        {
            MMI_STRNTOWSTR(wstr, 1023, 
                (uint8*)ua_profile_ptr, length, 
                MIN(length, 1023));
                
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, wstr, length, FALSE);           
        }

        MMK_SetAtvCtrl(win_id,MMIENG_TEXTBOX1_CTRL_ID);
    }
        break;
        
    case MSG_CTL_MIDSK:     
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:    
    case MSG_KEYDOWN_OK:        
    case MSG_KEYDOWN_CANCEL:
    case MSG_APP_WEB:
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


/*****************************************************************************/
//  Description : HandleUaProfileWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaProfileWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 group_id = 0;
    uint32 menu_id = 0;
    MMI_CTRL_ID_T ctrl_id = MMIENG_UA_SUB_MENU_CTRL_ID;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
    {
        MMI_STRING_T title_string = {0};
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, FALSE);
        MMI_GetLabelTextByLang(TXT_ENG_UA_PROFILE, &title_string);
        GUIMENU_SetMenuTitle(&title_string, ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
    }
        break;
			
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB: 
        GUIMENU_GetId(MMIENG_UA_SUB_MENU_CTRL_ID, &group_id, &menu_id);
        switch (menu_id)
        {
        case ID_ENG_UA_SET_ONOFF:
#ifdef ENG_SUPPORT
            MMIENG_OpenListSelectWin((uint32)EngUaProfileOnOffWinHandleMsg);
#endif
            break;
            
        case ID_ENG_UA_SET:
            MMK_CreateWin((uint32*)MMIENG_UA_PROFILE_SET_WIN_TAB, PNULL);
            break;
            
        case ID_ENG_UA_SET_SHOW:
            MMK_CreateWin((uint32*)MMIENG_UA_PROFILE_SHOW_WIN_TAB, PNULL);
            break;
            
        default:
            break;
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : HandleUaProfileSetWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaProfileSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_PROFILE_SELECT_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    BOOLEAN is_use_list = FALSE;

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
    {
        MMI_TEXT_ID_T           text_id_tab[] = { TXT_ENG_UA_LIST,TXT_ENG_UA_EDIT };
        uint16  item_total_num = ARR_SIZE(text_id_tab);
        
        GUILIST_SetMaxItem(ctrl_id, item_total_num, FALSE );//max item 3
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        AppendListItem(ctrl_id, text_id_tab, item_total_num);
        is_use_list = MMIENG_GetUaProfileIsUseList();
        cur_selection = (is_use_list)? 0: 1;
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        MMK_SetAtvCtrl(win_id,ctrl_id);
    }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;
                
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (0 == cur_selection)
        {
            is_use_list = TRUE;
            MMK_CreateWin((uint32*)MMIENG_UA_PROFILE_LIST_WIN_TAB, PNULL);
        }
        else
        {
            is_use_list = FALSE;
            MMK_CreateWin((uint32*)MMIENG_UA_PROFILE_EDIT_WIN_TAB, PNULL);
        }
        MMIENG_SetUaProfileIsUseList(is_use_list);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}


/*****************************************************************************/
//  Description : HandleUaProfileListWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaProfileListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_PROFILE_SELECT_LIST_NUM_CTRL_ID;
    uint32                 list_num = 0;

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
    {
        MMI_TEXT_ID_T   text_id_tab[] = {TXT_ENG_UA_NONE, TXT_ENG_PROFILE_SONYERICSSON,TXT_ENG_PROFILE_NOKIA};
        uint16  item_total_num = ARR_SIZE(text_id_tab);
        
        GUILIST_SetMaxItem(ctrl_id, item_total_num, FALSE );//max item 3
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        AppendListItem(ctrl_id, text_id_tab, item_total_num);
        
        list_num = MMIENG_GetUaProfileListNum();
        GUILIST_SetSelectedItem(ctrl_id, list_num, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,list_num);
        MMK_SetAtvCtrl(win_id,ctrl_id);
    }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;
                
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    {
        MMIENG_UA_T     ua_set={0};
        
        list_num = GUILIST_GetCurItemIndex(ctrl_id);
        MMIENG_SetUaProfileListNum(list_num);
        SCI_MEMSET(&ua_set,0,sizeof(MMIENG_UA_T));
        MMIENG_GetUa(&ua_set);
        SCI_MEMSET(ua_set.uaprofile,0,MMIENG_MAX_URL_LEN);
        switch(list_num) 
        {
        case 1:
            strncpy((char *)ua_set.uaprofile,MMIENG_UA_PROFILE_SONYERICSSION,MIN(MMIENG_MAX_URL_LEN,strlen(MMIENG_UA_PROFILE_SONYERICSSION)));/*lint !e666*/
            break;
            
        case 2:
            strncpy((char *)ua_set.uaprofile,MMIENG_UA_PROFILE_NOKIA,MIN(MMIENG_MAX_URL_LEN,strlen(MMIENG_UA_PROFILE_NOKIA)));/*lint !e666*/
            break;
            
        default:
            break;
        }
              
        MMIENG_SetUa(&ua_set);
        MMK_CloseWin(win_id);
    }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : HandleUaAgentEditWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaAgentEditWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_CTRL_ID_T       editbox_ctrl_id = MMIENG_UAAGENT_CTRL_ID;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
     {
        wchar wstr[MMIENG_MAX_URL_LEN + 1] = {0};
        uint16  ua_len = 0;
        MMIENG_UA_T ua = {0};

       //set im
        GUIEDIT_SetIm(editbox_ctrl_id, GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ENGLISH);/*lint !e655*/
        SCI_MEMSET(&ua,0,sizeof(MMIENG_UA_T));
        MMIENG_GetUa(&ua);
        ua_len=strlen((char *)(ua.user_agent));
        if (ua_len > 0)
        {
            MMI_STRNTOWSTR( wstr, MMIENG_MAX_URL_LEN, ua.user_agent, ua_len, ua_len );
            GUIEDIT_SetString(editbox_ctrl_id,wstr,ua_len);
        }
        MMK_SetAtvCtrl(win_id,editbox_ctrl_id);
    }
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
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    {
        MMI_STRING_T string_info = {0};
        MMIENG_UA_T ua = {0};
        //save to nv
        SCI_MEMSET(&string_info,0,sizeof(MMI_STRING_T));
        GUIEDIT_GetString(editbox_ctrl_id,&string_info);
        SCI_MEMSET(ua.user_agent,0,MMIENG_MAX_URL_LEN);
        MMI_WSTRNTOSTR(ua.user_agent, MMIENG_MAX_URL_LEN, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len );

        MMIENG_SetUa(&ua);
        MMK_CloseWin(win_id);
    }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : HandleUaProfileEditWinMsg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUaProfileEditWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_CTRL_ID_T       editbox_ctrl_id = MMIENG_UAPROFILE_CTRL_ID;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    {
        wchar wstr[MMIENG_MAX_URL_LEN + 1] = {0};
        uint16  ua_len = 0;
        MMIENG_UA_T ua = {0};
        
        //set im
        GUIEDIT_SetIm(editbox_ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ENGLISH);/*lint !e655*/
        SCI_MEMSET(&ua,0,sizeof(MMIENG_UA_T));
        MMIENG_GetUa(&ua);
        ua_len = strlen((char *)(ua.uaprofile));
        if (ua_len > 0)
        {
            MMI_STRNTOWSTR( wstr, MMIENG_MAX_URL_LEN, ua.uaprofile, ua_len, ua_len );
            GUIEDIT_SetString(editbox_ctrl_id,wstr,ua_len);
        }
        MMK_SetAtvCtrl(win_id,editbox_ctrl_id);
    }
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
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    {
        MMI_STRING_T string_info = {0};
        MMIENG_UA_T ua = {0};
        
        //save to nv
        SCI_MEMSET(&string_info,0,sizeof(MMI_STRING_T));
        GUIEDIT_GetString(editbox_ctrl_id,&string_info);
        SCI_MEMSET(ua.uaprofile,0,MMIENG_MAX_URL_LEN);
        MMI_WSTRNTOSTR( ua.uaprofile, MMIENG_MAX_URL_LEN, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len );
              
        MMIENG_SetUa(&ua);
        MMK_CloseWin(win_id);
    }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#ifdef ENGTD_SUPPORT
/********************************************************************************
 NAME:          GetNCNetInfoStr
 DESCRIPTION:   
 PARAM IN:      net_buf - 
                buf_len - 
 PARAM OUT:     net info string len
 AUTHOR:        allen
 DATE:          2004.09.08
********************************************************************************/
LOCAL int GetTDNCNetInfoStr(uint8* net_buf, uint16 buf_len)
{
    LAYER1_TD_NCELL_ARR_T    ncell = {0};

    uint8 digit_buf1[19] = {0};
    uint8 digit_buf2[19] = {0};
    uint8 digit_buf3[19] = {0};
    int i = 0;
    int len1 = 0, len2 = 0,len3=0;
    int offset = 0;
    
    if(PNULL == net_buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetTDNCNetInfoStr net_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_15604_112_2_18_2_18_13_204,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_win.c GetTDNCNetInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_15607_112_2_18_2_18_13_205,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }

    
#ifndef _WIN32  
    ncell=LAYER1_GetLayer1TDNcellList();
#endif
    for (i = 0; i < MAX_TD_NCELL_REPORT_NUM; i++)
    {
        // order
        MMIAPICOM_Int2Str(i + 1, digit_buf1, 18);       //( i + 1)   is the No. of NCELL
        len1 = SCI_STRLEN((char*)digit_buf1);
        if(buf_len - offset <  18)
        {
            return offset;
        } 
        MMI_MEMCPY(net_buf + offset, buf_len - offset, digit_buf1, 18, len1);
        offset += len1;
        
        if(buf_len - offset <  2)
        {
            return offset;
        } 
        MMI_MEMCPY(net_buf + offset, buf_len - offset, ". ", 2, 2);
        offset += 2;
        MMIAPICOM_Int2Str((uint32)ncell.td_ncell[i].arfcn, digit_buf1, 18);
        MMIAPICOM_Int2Str((uint32)ncell.td_ncell[i].cell_id, digit_buf2, 18);
        MMIENG_Int2Str((int16)ncell.td_ncell[i].rscp, digit_buf3, 18);
        
        len1 = SCI_STRLEN((char*)digit_buf1);
        if(buf_len - offset <  len1)
        {
            return offset;
        } 
        MMI_MEMCPY(net_buf + offset, buf_len - offset, digit_buf1, 18, len1);
        offset += len1;
        len2 = SCI_STRLEN((char*)digit_buf2);
        if (len1 + len2 + 1 <= ENG_WIN_MAX_CHAR)
        {
            if(buf_len - offset <  1)
            {
                return offset;
            } 
            MMI_MEMCPY(net_buf + offset, buf_len - offset,  ",", 1, 1);
            offset += 1;
            
            if(buf_len - offset <  len2 )
            {
                return offset;
            } 
            MMI_MEMCPY(net_buf + offset, buf_len - offset, digit_buf2, 18, len2);
            offset += len2;
        }
        len3 = SCI_STRLEN((char*)digit_buf3);
        if (len1 + len2+len3 + 1 <= ENG_WIN_MAX_CHAR)
        {
            if(buf_len - offset <  1 )
            {
                return offset;
            } 
            MMI_MEMCPY(net_buf + offset, buf_len - offset,  ",", 1, 1);
            offset += 1;
            
            if(buf_len - offset <  len3 )
            {
                return offset;
            } 
            MMI_MEMCPY(net_buf + offset, buf_len - offset, digit_buf3, 18, len3);
            offset += len3;
        }
        
        if(buf_len - offset < 1)
        {
            return offset;
        }        
        net_buf[offset] = CR_CHAR;
        offset += 1;
    }
    
    
    return (offset);
}
#endif

/********************************************************************************
 NAME:          Int2Str
 DESCRIPTION:   
 PARAM IN:      value - 
                buf - 
                buf_len - 
 PARAM OUT:     None
 AUTHOR:        allen
 DATE:          2004.09.14
********************************************************************************/
PUBLIC void MMIENG_Int2Str(int16 value, uint8* buf, uint16 buf_len)
{
    int32 i = 0, j = 0;
    int32 semi_len = 0;
    int32 offset = 0;
    uint8 tmp_chr = 0x00;
    BOOLEAN is_abs=FALSE;
 
    if(PNULL == buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c MMIENG_Int2Str buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_15713_112_2_18_2_18_13_206,(uint8*)"");
        return ;
    }    
    //SCI_TRACE_LOW:"mmieng_win.c MMIENG_Int2Str buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_15716_112_2_18_2_18_13_207,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return ;
    }
    
    if (value == 0)
    {
        buf[i] = (uint8)(0 + '0');
        buf[i + 1] = 0;
        return;
    }
    if(value&0x8000)
    {
        is_abs=TRUE;
        value=(int16)(~((uint16)value-1));  /*lint !e502*/
        
    }
    while (value != 0)
    {
        buf[i] = (uint8)((value % 10) + '0');   /*lint !e502*/
        value /= 10;
        i++;
        if (i >= buf_len)
            break;
    }
    if(is_abs)
    {
        buf[i++]='-';
    }
    if (i < buf_len)
        buf[i] = 0;
    semi_len = i / 2;
    offset = i % 2 - 1;
    for (j = 0; j < semi_len; j++)
    {
        tmp_chr = buf[j];
        buf[j] = buf[2 * semi_len + offset - j];
        buf[2 * semi_len + offset - j] = tmp_chr;
    }
}
/********************************************************************************
 NAME:          MMIENGWIN_OpenAudioSetWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     None
 AUTHOR:        ying.xu
 DATE:          2010.06.21
********************************************************************************/

PUBLIC BOOLEAN MMIENGWIN_OpenAudioSetWin(uint32 menuid)
{
    MMK_CreateWin((uint32*)MMIENG_AUDIOSET_WIN_TAB,(ADD_DATA)menuid);
    return TRUE;
}   
/********************************************************************************
 NAME:          MMIENGWIN_SetMMITestBit
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     None
 AUTHOR:        ying.xu
 DATE:          2010.08.17
********************************************************************************/
PUBLIC uint32  MMIENGWIN_SetMMITestBit(uint32 result)
{

    uint32 adc_reserved_info = 0;
        //从NV中读取adc_reserved[7]的信息
#ifdef WIN32    
    adc_reserved_info = 0xc3fe0155;/*lint !e64*/
#else
    adc_reserved_info = REFPARAM_GetCaliMarkerValue();
    //SCI_TRACE_LOW:"GetAdcCalibrateInfoStr %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_15786_112_2_18_2_18_13_208,(uint8*)"d",adc_reserved_info);
#endif

    adc_reserved_info &= ~ (0x00000001 << 27) ;
    adc_reserved_info |=  (result&0x01) << 27;
    return REFPARAM_SetCaliMarkerValue(adc_reserved_info) ; 
}   

#ifdef MMI_AUTOTEST_SUPPORT
/*****************************************************************************/
//  Description : HandleAutoTestSetWinMsg
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoTestSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    uint8  *at_set_ptr = (uint8*)MMK_GetWinAddDataPtr(win_id);
    uint32 at_string[] = {TXT_ENG_AT_ALL_SET,TXT_ENG_AT_IMAGE_ID,TXT_ENG_AT_TEXT,TXT_ENG_AT_WIN_MSG, TXT_ENG_AT_TP, TXT_ENG_AT_KEY,TXT_ENG_AT_MSG,
                          TXT_ENG_AT_SOFTKEY,TXT_ENG_AT_TITLE,TXT_ENG_AT_POINTER_STATE,TXT_ENG_AT_BACKLIGHT};// @CR247145 renwei modify
  
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        //GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        
        if (MMI_GetAtSetValue(*at_set_ptr))
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
        MMK_SetAtvCtrl(win_id,ctrl_id);

        GUIWIN_SetTitleTextId(win_id, at_string[*at_set_ptr], FALSE);        
        break; 
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            /*, @CR247145 renwei modify begin*/ 
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            
            if(MMI_AUTO_TEST_ALL_SET == *at_set_ptr)
            {
                if (1 == cur_selection) //close
                {
                    MMI_SetAllAtValue(FALSE);
                }
                else
                {
                    MMI_SetAllAtValue(TRUE);
                }
            }
            else
            {
                
                if (1 == cur_selection) //close
                {
                    MMI_SetAtValue(*at_set_ptr, FALSE);
                }
                else
                {
                    MMI_SetAtValue(*at_set_ptr, TRUE);        
                }
            }
            
            /*, @CR247145 renwei modify end*/ 
            MMK_CloseWin(win_id);
            break;
        }
                
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif

#ifdef WIFI_SUPPORT
LOCAL MMI_RESULT_E HandleWlanPerformanceModeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
  
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMISET_OPEN_CLOSE_ITEM, FALSE);//max item 2
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE, TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);  
#ifndef WIN32        
        if (WIFISUPP_LOG_ON == WIFISUPP_GetLogSwitch())
        {
            cur_selection = 1;
        }
        else
        {
            cur_selection = 0;
        }
#endif        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break; 
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifndef WIN32
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (1 == cur_selection) //close
        {
            WIFISUPP_SetLogSwitch(WIFISUPP_LOG_ON);
            TCPIP_SetGlobalSendWin(0); //default
            SCI_SetArmLogFlag(TRUE);
            TCPIPDEBUG_EnableCap(TRUE);
        }
        else
        {
            WIFISUPP_SetLogSwitch(WIFISUPP_LOG_OFF);
            TCPIP_SetGlobalSendWin(32 * 1024);
            SCI_SetArmLogFlag(FALSE);
            TCPIPDEBUG_EnableCap(FALSE);
        }
#endif        
        MMK_CloseWin(win_id);
        break;        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : HandleWifiIperfWinMsg
//  Global resource dependence : 
//  Author: YING.XU
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWifiIperfWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T editbox_id = MMIENG_EDITBOX1_CTRL_ID;

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            char *input_str = "Input";
            char *result_str = "Result";
            char *command_str = "-c 192.168.1.3";
            MMI_STRING_T text = {0};
            wchar wstr[32] = {0} ;

            text.wstr_ptr = wstr;
            
            text.wstr_len = strlen(input_str);
            MMI_STRNTOWSTR( wstr, 31, (uint8*)input_str, text.wstr_len, text.wstr_len );
            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);
            
            text.wstr_len = strlen(result_str);
            MMI_STRNTOWSTR( wstr, 31, (uint8*)result_str, text.wstr_len, text.wstr_len );
            GUILABEL_SetText(MMIENG_LABEL2_CTRL_ID, &text, FALSE);

            text.wstr_len = strlen(command_str);
            MMI_STRNTOWSTR(wstr, 31, (uint8*)command_str, text.wstr_len, text.wstr_len);
            GUIEDIT_SetString(editbox_id, text.wstr_ptr, text.wstr_len);
            
            GUIEDIT_SetIm(editbox_id,  GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ENGLISH); /*lint !e655*/
            MMK_SetAtvCtrl(win_id,editbox_id);
        }
        break; 
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T edit_text = {0};
            char str[256] = {0} ;

#if defined(MMI_PDA_SUPPORT)
            if (MSG_CTL_PENOK == msg_id)
            {
                if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
                {
                    MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);

                    return MMI_RESULT_TRUE;
                }
                else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
                {
                    MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);

                    return MMI_RESULT_TRUE;
                }
            }
#endif

            GUIEDIT_GetString(editbox_id,  &edit_text);
            MMI_WSTRNTOSTR((uint8*)str, 255, edit_text.wstr_ptr, edit_text.wstr_len, edit_text.wstr_len );
#ifndef WIN32
            iperf_test_start(str);
#endif
            MMK_CloseWin(win_id);
        }
        break;        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : HandleWifiIperfWinMsg
//  Global resource dependence : 
//  Author: George.Liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWlanRfSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            recode      =   MMI_RESULT_TRUE;
    WIFISUPP_COMMAND_INFO_T command_info ={0};

    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set power save param
        GUIFORM_SetStyle(MMIENG_WLAN_POWER_SAVE_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUILABEL_SetTextById(MMIENG_WLAN_POWER_SAVE_LABEL_CTRL_ID, TXT_ENG_WLAN_POWER_SAVE, FALSE);
        GUISETLIST_AddItemById(MMIENG_WLAN_POWER_SAVE_SETLIST_CTRL_ID, TXT_COMM_OFF);
        GUISETLIST_AddItemById(MMIENG_WLAN_POWER_SAVE_SETLIST_CTRL_ID, TXT_COMM_ON);
        GUISETLIST_SetCtrlState(MMIENG_WLAN_POWER_SAVE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
        GUISETLIST_SetCurIndex(MMIENG_WLAN_POWER_SAVE_SETLIST_CTRL_ID, 1);
        GUISETLIST_SetTitleTextId(MMIENG_WLAN_POWER_SAVE_SETLIST_CTRL_ID, TXT_ENG_WLAN_POWER_SAVE);    

        //set roaming param
        GUIFORM_SetStyle(MMIENG_WLAN_ROAMING_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUILABEL_SetTextById(MMIENG_WLAN_ROAMING_LABEL_CTRL_ID, TXT_ENG_WLAN_ROAMING, FALSE);
        GUISETLIST_AddItemById(MMIENG_WLAN_ROAMING_SETLIST_CTRL_ID, TXT_COMM_OFF);
        GUISETLIST_AddItemById(MMIENG_WLAN_ROAMING_SETLIST_CTRL_ID, TXT_COMM_ON);
        GUISETLIST_SetCtrlState(MMIENG_WLAN_ROAMING_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
        GUISETLIST_SetCurIndex(MMIENG_WLAN_ROAMING_SETLIST_CTRL_ID, 1);
        GUISETLIST_SetTitleTextId(MMIENG_WLAN_ROAMING_SETLIST_CTRL_ID, TXT_ENG_WLAN_ROAMING);    

        MMK_SetAtvCtrl(win_id, MMIENG_RADIO_FREQUENCY_FORM_CTRL_ID);
        break;
        
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:

#ifndef WIN32        
        //handle power save setting
        command_info.command_id = WIFIDRV_COMMAND_POWER_SAVE;
        command_info.para1 = GUISETLIST_GetCurIndex(MMIENG_WLAN_POWER_SAVE_SETLIST_CTRL_ID);
        //SCI_TRACE_LOW:"WIFI:info:MMI: HandleWlanRfSetWinMsg WIFISUPP_Command cmd=0x%x, para1=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_16069_112_2_18_2_18_14_209,(uint8*)"dd", command_info.command_id, command_info.para1);
        WIFISUPP_Command(&command_info);

        //handle roaming setting
        command_info.command_id = WIFIDRV_COMMAND_SET_ROAMING_MODE;
        command_info.para1 = GUISETLIST_GetCurIndex(MMIENG_WLAN_ROAMING_SETLIST_CTRL_ID);
        //SCI_TRACE_LOW:"WIFI:info:MMI: HandleWlanRfSetWinMsg WIFISUPP_Command cmd=0x%x, para1=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_16075_112_2_18_2_18_14_210,(uint8*)"dd", command_info.command_id, command_info.para1);
        WIFISUPP_Command(&command_info);
#endif        
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
//  Description : change url win
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBrowserChangeURLWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        //GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN, TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE, TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);  
        
        if (MMIAPIENG_GetIsBrowserChangeURL())
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;    
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (0 == cur_selection) //open
        {
            MMIAPIENG_SetIsBrowserChangeURL(TRUE);
        }
        else
        {
            MMIAPIENG_SetIsBrowserChangeURL(FALSE);            
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
//  Description : HandleBrowserLogWinMsg
//  Global resource dependence : 
//  Author: sally.he
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBrowserLogWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        //GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        
        if (MMIAPIENG_GetIsBrowserLogOn())
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
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;    
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (0 == cur_selection) //open
        {
            MMIAPIENG_SetIsBrowserLogOn(TRUE);
        }
        else
        {
            MMIAPIENG_SetIsBrowserLogOn(FALSE);            
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
//  Description : open cardlog win
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenBrowserLogWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_BROWSER_LOG_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : browser change url by wre
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenBrowserChangeURLWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_BROWSER_CHANGE_URL_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : HandleScreenLogWinMsg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleScreenLogWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            wchar* off_wstr = L"OFF";
            wchar* debug_string_wstr = L"DEBUG STRING";
            wchar* debug_rect_wstr = L"DEBUG RECT";

            GUILIST_SetMaxItem(ctrl_id,MMIENG_SCREEN_DEBUG_MAX, FALSE );//max item 2
            //GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
            MMIAPISET_AppendListItemByTextBuffer(off_wstr, MMIAPICOM_Wstrlen( off_wstr ), 
                0,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO); 

            MMIAPISET_AppendListItemByTextBuffer(debug_string_wstr, MMIAPICOM_Wstrlen( debug_string_wstr ), 
                0,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
            
            MMIAPISET_AppendListItemByTextBuffer(debug_rect_wstr, MMIAPICOM_Wstrlen( debug_rect_wstr ), 
                0,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
            
            cur_selection = (uint16)MMIAPIENG_GetIsScreenLogOn();
            
            //set selected item
            GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
            
            //set current item
            GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }

        break;    
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

        MMIAPIENG_SetIsScreenLogOn( cur_selection );

        MMK_CloseWin(win_id);
        break;        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#ifdef MMISRV_AUDIO_TEST_SUPPORT
/*****************************************************************************/
// 	Description : Audio self test, play audio.
//	Global resource dependence : none
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EngAudioSelfTestPlayHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
        //int32 index = 0;
        MMI_RESULT_E result = MMI_RESULT_TRUE;
        //int32 list_id = MMIENG_LISTBOX_CTRL_ID ;
        
        switch (msg_id) 
        {
        case MSG_OPEN_WINDOW:
            break;
            
        case MSG_FULL_PAINT:
            MMISRVAUD_TestDrawBg();
            break;      
            
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            break;
        default:
            result = MMISRVAUD_TestHandleKey(win_id, msg_id, param);
            break;
        }
        return (result);     
}

/*****************************************************************************/
// 	Description : Audio self test, display audio info.
//	Global resource dependence : none
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EngAudioSelfTestDisplayInfoHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
        int32 index = 0;
        MMI_RESULT_E result = MMI_RESULT_TRUE;
        int32 list_id = MMIENG_LISTBOX_CTRL_ID ;
        
        switch (msg_id) 
        {
        case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};       
#ifdef PRINTSCREEN_SUPPORT
                GUIWIN_SetTitleTextId(win_id, TXT_ENG_AUDIO_DISPLAY_INFO, FALSE);
#endif
                GUILIST_SetMaxItem( list_id, 3, FALSE );
                GUILIST_SetOwnSofterKey(list_id,FALSE);
                
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
                item_t.item_data_ptr = &item_data;
                
                item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
                
                item_data.item_content[0].item_data.text_id = TXT_COMM_ON;    
                GUILIST_AppendItem( list_id, &item_t );
                
                item_data.item_content[0].item_data.text_id = TXT_COMM_OFF;    
                GUILIST_AppendItem( list_id, &item_t );
               
                GUILIST_SetSelectedItem( list_id, index, TRUE );
                GUILIST_SetCurItemIndex( list_id, index);          
                MMK_SetAtvCtrl(win_id, list_id);
            }
            break;
            
        case MSG_FULL_PAINT:
            break;      
            
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            index = GUILIST_GetCurItemIndex(list_id);   
            switch(index)
            {
            case 0:
                MMISRVAUD_TestSetEnable(TRUE);
                MMK_CloseWin(win_id);
                break;
            case 1:
                MMISRVAUD_TestSetEnable(FALSE);
                MMK_CloseWin(win_id);
                break;
            default:
                break;
            }
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
        }
        return (result); 
}

/*****************************************************************************/
// 	Description : Audio self test.
//	Global resource dependence : none
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EngAudioSelfTestHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
        int32 index = 0;
        MMI_RESULT_E result = MMI_RESULT_TRUE;
        int32 list_id = MMIENG_LISTBOX_CTRL_ID ;
        
        switch (msg_id) 
        {
        case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};       

                GUIWIN_SetTitleTextId(win_id, TXT_ENG_AUDIO_SELF_TEST, FALSE);
                
                GUILIST_SetMaxItem( list_id, 2, FALSE );
                GUILIST_SetOwnSofterKey(list_id,FALSE);
                
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
                item_t.item_data_ptr = &item_data;
                
                item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;    
                item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID; 
                item_data.item_content[0].item_data.image_id = IMAGE_NUMBER_1;
                item_data.item_content[1].item_data.text_id = TXT_ENG_AUDIO_PLAY;    
                GUILIST_AppendItem( list_id, &item_t );
                
            #ifdef PRINTSCREEN_SUPPORT
                item_data.item_content[0].item_data.image_id = IMAGE_NUMBER_2;
                item_data.item_content[1].item_data.text_id = TXT_ENG_AUDIO_DISPLAY_INFO;    
                GUILIST_AppendItem( list_id, &item_t );
            #endif
               
                GUILIST_SetSelectedItem( list_id, index, TRUE );
                GUILIST_SetCurItemIndex( list_id, index);          
                MMK_SetAtvCtrl(win_id, list_id);
            }
            break;
            
        case MSG_FULL_PAINT:
            break;      
            
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            index = GUILIST_GetCurItemIndex(list_id);   
            switch(index)
            {
            case 0:
                MMK_CreateWin((uint32*)MMIENG_AUDIO_SELF_TEST_PLAY_TAB, PNULL);
                
                break;
        #ifdef PRINTSCREEN_SUPPORT        
            case 1:
#ifdef ENG_SUPPORT
                MMIENG_OpenListSelectWin((uint32)EngAudioSelfTestDisplayInfoHandleMsg);
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
        return (result); 
}    

#endif


/*****************************************************************************/
//  Description : open screenlog win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenScreenLogWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_SCREEN_LOG_WIN_TAB, PNULL);
}

#ifdef MMI_AUTOTEST_SUPPORT
/*****************************************************************************/
//  Description : open auto test set win
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenAutoTestSetWin(uint8 set_id)
{
    uint8* type_id_ptr = PNULL;
    
    if (set_id >=  MMI_AUTO_TEST_MAX)
    {
        return;
    }

    type_id_ptr = (uint8*)SCI_ALLOCA(sizeof(uint8));

    *type_id_ptr = set_id;
    MMK_CreateWin((uint32*)MMIENG_AUTOTEST_SET_WIN_TAB, (ADD_DATA)type_id_ptr);
}
#endif

/********************************************************************************
 NAME:			GetProductionInfoStr
 DESCRIPTION:	
 PARAM IN:		pd_buf - 
				buf_len - 
 PARAM OUT:		production info string len
 AUTHOR:		allen
 DATE:			2004.10.04
 modified by ying.xu,20100609
********************************************************************************/
LOCAL int32 GetProductionInfoStr(char* pd_buf, int32 buf_len)
{	
    int32 i = 0;
    int32 len1 = 0;
    int32 offset = 0;
    char num[8] = {0};    
    uint16 item_num = 0;
    int32 station_num = 0;
//     uint16 phase_length = sizeof(SP09_PHASE_CHECK_T);
    char *isTested[2] = {"Tested!", "Not Tested!"};
    char *isPassed[2] ={"Passed!", "Failed!"} ;
    
    uint8 temp_buffer[1025] = {0};
    int32 temp_buffer_lenght = 1024 ;
    
    int32 test_length[2] ={0};	
    int32 pass_length[2] = {0};

#ifndef WIN32
    SP09_PHASE_CHECK_T  s_mmieng_product = {0};    
#else
    SP09_PHASE_CHECK_T  s_mmieng_product  = {SP09_SPPH_MAGIC_NUMBER,
        "12345678901234567890123",
        "12345678901234567890123",
        15,
    {"123456789","234567891","345678912","456789123","567891234",
    "678912345","789123456","891234567","912345678","123456789",
    "234567891","345678912","456789123","567891234","678912345"},
    {0}, 0x01, {0},	0xf00f,0x0ff0 };
#endif
    
    test_length[0] = SCI_STRLEN(isTested[0]) ;
    test_length[1] = SCI_STRLEN(isTested[1]) ;
    pass_length[0] = SCI_STRLEN(isPassed[0]) ;
    pass_length[1] = SCI_STRLEN(isPassed[1]) ;        
    
    //read data from flash
#ifndef WIN32       
    if(0 == EFS_ReadPhaseCheck(temp_buffer,temp_buffer_lenght))
    {
        return 0;
    } 
    SCI_MEMCPY(&s_mmieng_product, temp_buffer, SP09_MAX_PHASE_BUFF_SIZE);   
#endif

#if defined(__BIG_ENDIAN) || defined(__BigEndian)	
    if(SP09_SPPH_MAGIC_NUMBER != CHANGE_ENDIAN_SEQUENCE_32(s_mmieng_product.Magic))
#else   
    if(SP09_SPPH_MAGIC_NUMBER != s_mmieng_product.Magic) 
#endif    
    {
        SCI_TRACE_LOW("MMIENG_WIN GetProductionInfoStr magic num = %x", s_mmieng_product.Magic); 
        
        len1 = sprintf((char*)temp_buffer,"error! magic num = %x\n",s_mmieng_product.Magic);
        offset += len1;
        SCI_MEMCPY(pd_buf, temp_buffer, len1);
        //return 0;
    }
        ////////////////
        //  S/N
        ////////////////
        MMI_MEMCPY(pd_buf + offset, buf_len - offset, "<S/N1>", 6, 6);
        offset += 6;
        pd_buf[offset] = CR_CHAR;
        offset += 1;
        len1 = MIN(SP09_MAX_SN_LEN,SCI_STRLEN(s_mmieng_product.SN1));/*lint !e666*/
        MMI_MEMCPY(pd_buf + offset, buf_len - offset, 
            s_mmieng_product.SN1, SP09_MAX_SN_LEN , len1);
        offset += len1;
        pd_buf[offset] = CR_CHAR;
        offset += 1;

//Michael: 这部分暂时处理成如下的方式，也是09A的处理方式
//逻辑分析上是应该有SN2/SN3/SN4等多卡信息的处理的
//但是，目前底层提供的只有SN1/SN2,因此，这部分还需要再讨论!!!
//待底层有SN3/SN4 的信息，再考虑优化这个多卡处理流程....
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MMI_MEMCPY(pd_buf + offset, buf_len - offset, "<S/N2>", 6, 6);
        offset += 6;
        pd_buf[offset] = CR_CHAR;
        offset += 1;
        len1 = MIN(SP09_MAX_SN_LEN,SCI_STRLEN(s_mmieng_product.SN2));/*lint !e666*/
        MMI_MEMCPY(pd_buf + offset, buf_len - offset, 
            s_mmieng_product.SN2, SP09_MAX_SN_LEN , len1);
        offset += len1;
        pd_buf[offset] = CR_CHAR;
        offset += 1;
#endif	
        
        
        ////////////////////////////
        //  station test result
        ////////////////////////////
        MMI_MEMCPY(pd_buf + offset, buf_len - offset, "<Station Result>", 16, 16);
        offset += 16;
        
 #if defined(__BIG_ENDIAN) || defined(__BigEndian)
        station_num = CHANGE_ENDIAN_SEQUENCE_32(s_mmieng_product.StationNum);
#else
        station_num = s_mmieng_product.StationNum;
#endif       
        for (i = 0; i <  station_num; i++)
        {
            pd_buf[offset] = CR_CHAR;
            offset += 1;
            
            //num		        
            SCI_MEMSET(num, 0 ,8);
            sprintf(num,"<%d> ",i+1);
            len1  = SCI_STRLEN(num);
            if(buf_len - offset < len1)
            {
                return offset;
            }
            MMI_MEMCPY(pd_buf + offset, buf_len - offset, num, 7, len1);		
            offset += len1;	
            
            //name          
            len1 = MIN(SP09_MAX_STATION_NAME_LEN, SCI_STRLEN(s_mmieng_product.StationName[i]));/*lint !e666*/
            if(buf_len - offset < len1)
            {
                return offset;
            }
            MMI_MEMCPY(pd_buf + offset, buf_len - offset, 
                s_mmieng_product.StationName[i], SP09_MAX_STATION_NAME_LEN, len1);
            offset += len1;

            if(buf_len - offset < 2)
            {
                return offset;
            }
            MMI_MEMCPY(pd_buf + offset, buf_len - offset, ": ", 2, 2);	
            offset += 2;

            // if tested
#if defined(__BIG_ENDIAN) || defined(__BigEndian)	
            item_num = (CHANGE_ENDIAN_SEQUENCE_16(s_mmieng_product.iTestSign) >> i) & (uint16)0x0001;
#else    
            item_num = (s_mmieng_product.iTestSign >> i) & (uint16)0x0001; 
#endif   
              if(s_mmieng_product.SignFlag == 0x00)
            {
                item_num = (~item_num)&(uint16)0x0001;
            }
            
            if(buf_len - offset < test_length[item_num])
            {
                return offset;
            }
            MMI_MEMCPY(pd_buf + offset, buf_len - offset, isTested[item_num],
                test_length[item_num], test_length[item_num]);
            
            offset += test_length[item_num];
            
            //if passed  
            if(item_num == 0)
            {
                MMI_MEMCPY(pd_buf + offset, buf_len - offset, ": ", 2, 2);		
                offset += 2;
#if defined(__BIG_ENDIAN) || defined(__BigEndian)	
                item_num = (CHANGE_ENDIAN_SEQUENCE_16(s_mmieng_product.iItem) >> i) & (uint16)0x0001;
#else     
                item_num = (s_mmieng_product.iItem >> i)&(uint16)0x0001;
#endif               
                if(s_mmieng_product.SignFlag == 0x00)
                {
                    item_num = (~item_num)&(uint16)0x0001;
                }
                
                if(buf_len - offset < pass_length[item_num])
                {
                    return offset;
                }
                MMI_MEMCPY(pd_buf + offset, buf_len - offset, isPassed[item_num],
                    pass_length[item_num],pass_length[item_num]);
                offset += pass_length[item_num];
            }
            
        }		// end of 	for (i = 0; i <  SP09_MAX_STATION_NUM; i++)
        
        return offset;
    //return 0;
}

LOCAL MMI_RESULT_E  HandleChiptestTpWinMsg (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
     MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:      
        {
             GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
             LCD_FillRect(MMITHEME_GetDefaultLcdDev(), rect, MMI_BACKGROUND_COLOR);    
         }
             MMIAPITP_COORDINATE_OpenWin(FALSE);
         
        break;
   
    case MSG_GET_FOCUS:
        MMK_CreateWin((uint32*)MMIENG_CHIP_TEST_PLAYMP3MP4_WIN_TAB, PNULL);
        MMK_CloseWin(win_id);
        break;  
        
    default:
        break;
    }      
    return result;
}

#ifdef WIFI_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: George.Liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenWlanPerformanceModeWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_WLAN_PERFORMANCE_MODE_WIN_TAB, 0);
}

/*****************************************************************************/
//  Description : open WIFI iperf  win
//  Global resource dependence : 
//  Author: YING.XU
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenWifiIperfWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_WIFI_IPERF_WIN_TAB, 0);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: George.Liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenWlanRadioFrequencyWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_WLAN_RADIO_FREQUENCY_WIN_TAB, 0);
}
#endif

// SUPPORT_STANDBY_RECORD
PUBLIC void MMIENG_OpenStandbyInfoWin(void)
{    
    MMK_CreateWin((uint32*)MMIENG_STANDBY_TIME_INFO_TAB,PNULL);
}

/*******************************************************************************/
// NAME:			StandbyTimeInfoWinHandleMsg
//DESCRIPTION:	
// PARAM IN:		win_id - 
//				msg_id - 
//				param - 
// PARAM OUT:		
//AUTHOR:	Rex.Yang	
// DATE:	
/*******************************************************************************/
LOCAL MMI_RESULT_E StandbyTimeInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_STRING_T  standbyTimeInfoStr={0};
    MMI_CTRL_ID_T ctrl_id=MMIENG_TEXTBOX1_CTRL_ID;
    uint32 standby_startTime,standby_time;
    uint16 s_day,s_hour,s_minute,s_second,t_minute;
    uint32 t_hour;
    STANDBY_RECORD_T standbyInfo={0};
    uint32 nv_id = MMIENG_NV_POWERON_TIME_ID;
    uint8 char_str[128]={0};
    wchar char_wstr[128]={0};
    const uint32 buffer_length = 127;

#define MMIENG_SIXTY_RADIX          60
#define MMIENG_HOUR_PER_DAY      24
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
      MMI_ReadNVItem(nv_id , &standbyInfo);
            standby_startTime=standbyInfo.standbyStartTime;
            standby_time=standbyInfo.standbytime;
            s_day=standbyInfo.startDay;
        //SCI_TRACE_LOW:"standby_start:%lu standby_time:%lu\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_16729_112_2_18_2_18_15_211,(uint8*)"uu",standby_startTime,standby_time);
        
        s_second=(uint16)(standby_startTime%MMIENG_SIXTY_RADIX);
        standby_startTime=standby_startTime/MMIENG_SIXTY_RADIX;
        s_minute=(uint16)(standby_startTime%MMIENG_SIXTY_RADIX);
        standby_startTime=standby_startTime/MMIENG_SIXTY_RADIX;
        s_hour=(uint16)(standby_startTime%MMIENG_HOUR_PER_DAY);
        standby_startTime=standby_startTime/MMIENG_HOUR_PER_DAY;

		MMI_ReadNVItem(MMIENG_NV_STANDBY_TIME_ID , &standby_time);
        standby_time=standby_time/MMIENG_SIXTY_RADIX;
        t_minute=(uint16)(standby_time%MMIENG_SIXTY_RADIX);
        t_hour=standby_time/MMIENG_SIXTY_RADIX;
        
        //SCI_TRACE_LOW:"s_d:%u s_h:%u s_m:%u s_s:%u t_h:%lu t_m:%un"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_16742_112_2_18_2_18_15_212,(uint8*)"uuuuuu",s_day,s_hour,s_minute,s_second,t_hour,t_minute);
        sprintf((char *)char_str,"start(DHMS):%u:%u:%u:%u\nstandby(HM):%lu:%u",s_day,
            s_hour,s_minute,s_second,t_hour,t_minute);
        standbyTimeInfoStr.wstr_ptr=char_wstr;
        standbyTimeInfoStr.wstr_len=/*min*/(strlen((char *)char_str) < buffer_length ? strlen((char *)char_str) : buffer_length);
        MMI_STRNTOWSTR(standbyTimeInfoStr.wstr_ptr,buffer_length,\
            char_str,buffer_length,(uint32)standbyTimeInfoStr.wstr_len);
        GUITEXT_SetString(ctrl_id,standbyTimeInfoStr.wstr_ptr,standbyTimeInfoStr.wstr_len,FALSE);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_FULL_PAINT:
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB: 
    case MSG_CTL_MIDSK:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/**********************************************************************************/
//  Description : record standby time file init
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/**********************************************************************************/
PUBLIC void MMIENG_RecordStandbyInit(void)
{
    STANDBY_RECORD_T standby = {0};
    SCI_DATE_T startDay;
    uint32 nv_id = MMIENG_NV_POWERON_TIME_ID;
    MN_RETURN_RESULT_E error= 0;
    
    error = MMI_ReadNVItem(nv_id, &standby);    
    //SCI_TRACE_LOW:"MMIENG_WIN.C MMIENG_RecordStandbyInit MMI_ReadNVItem = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_16783_112_2_18_2_18_15_213,(uint8*)"d",error);
    //SCI_TRACE_LOW:"MMIENG_WIN.C MMIENG_RecordStandbyInit ENALBE = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_16784_112_2_18_2_18_15_214,(uint8*)"d",standby.standbyEn);
    
    if(1 == standby.standbyEn)
    {
        TM_GetSysDate(&startDay);
        standby.standbyStartTime = TM_GetTotalSeconds();
        standby.startDay = startDay.mday;
        MMI_WriteNVItem( nv_id,  &standby );      
        Power_RegFunCalculateStandby(MMI_CalculateStandbyTime2File);
    }				
    return;
}

/**********************************************************************************/
//  Description : MMI_CalculateStandbyTime2File
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/**********************************************************************************/
LOCAL void MMI_CalculateStandbyTime2File(void)
{
    STANDBY_RECORD_T standby = {0};
    uint32 standbyEndTime=0;
    uint32 nv_id = MMIENG_NV_POWERON_TIME_ID;
    MN_RETURN_RESULT_E error = 0;
    error = MMI_ReadNVItem(nv_id, &standby);    
    //SCI_TRACE_LOW:"MMIENG_WIN.C MMI_CalculateStandbyTime2File MMI_ReadNVItem = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_16808_112_2_18_2_18_15_215,(uint8*)"d",error);
    standbyEndTime=TM_GetTotalSeconds();
    if(((standbyEndTime>standby.standbyStartTime)&&(0==standby.standbytime))&&\
        ((0!=standby.standbyStartTime)&&(1==standby.standbyEn)))
    {
        standby.standbytime=standbyEndTime-standby.standbyStartTime;
        MMI_WriteNVItem(nv_id , &standby);
    }
    return;
}

/**********************************************************************************/
//  Description : MMI_EnableAndResetStanbyRecord
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/**********************************************************************************/
PUBLIC void MMIENG_EnableAndResetStanbyRecord(uint32 enable)
{
    STANDBY_RECORD_T standby = {0};
    uint32 nv_id = MMIENG_NV_POWERON_TIME_ID;
    standby.standbyEn = enable;
    if(1 != enable)
    {
        Power_RegFunCalculateStandby(SCI_NULL);
    }
    MMI_WriteNVItem(nv_id , &standby);
}

/**********************************************************************************/
//  Description : MMIENG_GetStanbyRecordEnableState
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/**********************************************************************************/
PUBLIC uint32 MMIENG_GetStanbyRecordEnableState(void)
{
    STANDBY_RECORD_T standby = {0};
    uint32 nv_id = MMIENG_NV_POWERON_TIME_ID;	
    MMI_ReadNVItem(nv_id , &standby);
    return standby.standbyEn;
}


//end SUPPORT_STANDBY_RECORD


/********************************************************************************
 NAME:			MMIENG_OpenGetFlashInfoWin
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		ying.xu
 DATE:			
********************************************************************************/
PUBLIC void MMIENG_OpenGetFlashInfoWin(void)
{
	MMK_CreateWin((uint32*)MMIENG_GET_FLASH_INFO_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			EngGetFlashInfoWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		ying.xu
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E EngGetFlashInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E 			result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   		textbox_id = MMIENG_TEXTBOX1_CTRL_ID;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:   
		{
			uint32 manufactor_id = 0;
			uint32 device_id = 0;
			uint32 extend_id = 0;
			char   str[128] = {0};
			wchar  wstr[128] = {0};
			uint32 buf_length = 127;	
			uint32 str_length = 0;
			uint32 offset = 0;
			MMI_STRING_T text = {0};
#ifndef WIN32
			PROD_GetFlashID(&manufactor_id, &device_id, &extend_id);
#endif			
			str_length = sprintf(&str[offset],"Manufacture ID:%x\n", manufactor_id);
			offset += str_length;
			str_length = sprintf(&str[offset],"Device ID:%x\n", device_id);
			offset += str_length;
			str_length = sprintf(&str[offset],"Extend ID:%x", extend_id);
			offset += str_length;
			MMI_STRNTOWSTR(wstr, buf_length, (uint8*)str,  offset, offset);
			text.wstr_ptr = wstr;
			text.wstr_len = offset; 
			GUITEXT_SetString(textbox_id , text.wstr_ptr, text.wstr_len, FALSE );
			}
        MMK_SetAtvCtrl(win_id, textbox_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:	
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/********************************************************************************
NAME:			MMIENG_OpenuUpdateIMEIWin
DESCRIPTION:	
PARAM IN:		
PARAM OUT:		
AUTHOR:		ying.xu
********************************************************************************/
PUBLIC void MMIENG_OpenuUpdateIMEIWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_UPDATE_IMEI_WIN_TAB, PNULL);
}

LOCAL NV_ITEM_ID_E GetImeiId(
                            MN_DUAL_SYS_E     dual_sys       //IN:
                            )
{
    NV_ITEM_ID_E    nv_id = NV_IMEI;
    switch (dual_sys)
    {
    case MN_DUAL_SYS_1:
        nv_id = NV_IMEI;
        break;
#if defined MULTI_SIM_SYS_DUAL || defined MULTI_SIM_SYS_TRI || defined MULTI_SIM_SYS_QUAD
    case MN_DUAL_SYS_2:
        nv_id = NV_IMEI1;
        break;
#endif
        
#if defined MULTI_SIM_SYS_TRI || defined MULTI_SIM_SYS_QUAD
    case MN_DUAL_SYS_3:
        nv_id = NV_IMEI2;
        break;
#endif
#if defined MMI_DUAL_SIM_SYS_QUAD
    case MN_DUAL_SYS_4:
        nv_id = NV_IMEI3;
        break;
#endif

    default:
        break;
    }
    return (nv_id);
}

/********************************************************************************
NAME:			EngGetFlashInfoWinHandleMsg
DESCRIPTION:	
PARAM IN:		win_id - 
msg_id - 
param - 
PARAM OUT:		
AUTHOR:		ying.xu
DATE:			
********************************************************************************/
LOCAL BOOLEAN ReadIMEINum(MN_DUAL_SYS_E dual_sys, uint8 *pbuf, uint32 buffer_length)
{
    uint16  nv_id = NV_IMEI;
    int32 error = 0;
    
    nv_id =  GetImeiId(dual_sys);
    //SCI_ASSERT(buffer_length >= MN_MAX_IMEI_LENGTH);/*assert verified*/		
    if(buffer_length < MN_MAX_IMEI_LENGTH)
    {
        return  FALSE;
    }
    error = EFS_NvitemRead(nv_id, MN_MAX_IMEI_LENGTH, pbuf);	
    
    //SCI_TRACE_LOW:"MMIENG_WIN ReadIMEINum EFS_NvitemRead error = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_16992_112_2_18_2_18_16_216,(uint8*)"d", error);
    return error == 0 ?TRUE:FALSE;	
}

LOCAL BOOLEAN WriteIMEINum(MN_DUAL_SYS_E dual_sys, uint8 *pbuf, uint32 buffer_length)
{
    NV_ITEM_ID_E    nv_id = NV_IMEI;
    int32 error = 0;    
    nv_id =  GetImeiId(dual_sys);
    error = EFS_NvitemWrite(nv_id, MN_MAX_IMEI_LENGTH, pbuf, FALSE);	
    //SCI_TRACE_LOW:"MMIENG_WIN ReadIMEINum EFS_NvitemRead error = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_17002_112_2_18_2_18_16_217,(uint8*)"d", error);
    return error == 0 ?TRUE:FALSE;	
}

LOCAL BOOLEAN ConvertIMEICode2Num(char *dst,  uint32 dst_length, uint8 *src, uint32 src_length)
{
    int32 i = 0;
    char tmp_buf1[MN_MAX_IMEI_LENGTH*2 + 1] = {0};
    for (i = 0; i < MN_MAX_IMEI_LENGTH; i++) 
    {
        tmp_buf1[2 * i] = (src[i]) & 0x0F;
        tmp_buf1[2 * i + 1] = src[i] >> 4;
    }
    sprintf(    dst,
        "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
        tmp_buf1[0],tmp_buf1[1], tmp_buf1[2], tmp_buf1[3],
        tmp_buf1[4], tmp_buf1[5], tmp_buf1[6], tmp_buf1[7],
        tmp_buf1[8], tmp_buf1[9], tmp_buf1[10], tmp_buf1[11],
        tmp_buf1[12], tmp_buf1[13], tmp_buf1[14], tmp_buf1[15]
        );
    return TRUE;
}

LOCAL BOOLEAN ConvertNum2IMEICode(uint8 *dst,  uint32 dst_length, char *src, uint32 src_length)
{
    int32 i = 0;
    uint8 tmp_buf1[MN_MAX_IMEI_LENGTH*2 + 1] = {0};
    tmp_buf1[0] = 0xA;
    for (i = 0; i < src_length; i++) /*lint !e574 !e737*/
    {
        tmp_buf1[i+1] = (uint8) ( (src[i]>= 'A') ? src[i] - 'A' + 10 : src[i] - '0');
    }
    for (i = 0; i < MN_MAX_IMEI_LENGTH; i++) 
    {
        dst[i] = (tmp_buf1[2 * i] & 0x0f) | ((tmp_buf1[2 * i + 1] << 4) & 0Xf0);
    }
    return TRUE;
}

/********************************************************************************
NAME:			EngGetFlashInfoWinHandleMsg
DESCRIPTION:	
PARAM IN:		win_id - 
msg_id - 
param - 
PARAM OUT:		
AUTHOR:		ying.xu
DATE:			
********************************************************************************/
LOCAL BOOLEAN UpateIMEIListData(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T  list_id)
{
    int32 i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint8 imei_buffer[MN_MAX_IMEI_LENGTH + 1] = {0};    
    char num_buffer[MN_MAX_IMEI_LENGTH*2 + 1] = {0};
    wchar wbuffer[MN_MAX_IMEI_LENGTH*2 + 1] = {0};
    uint16  length = 0;
         MMI_IMAGE_ID_T   sim_image[2]     = {IMAGE_IDLE_PROMPT_SIM1,IMAGE_IDLE_PROMPT_SIM2};
    GUILIST_SetMaxItem(list_id, MMI_DUAL_SYS_MAX, FALSE );//max item 3
    GUILIST_RemoveAllItems(list_id );
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;////GUIITEM_STYLE_ONE_LINE_TEXT
    item_t.item_data_ptr = &item_data;
    
    for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        SCI_MEMSET(imei_buffer, 0, MN_MAX_IMEI_LENGTH);
        SCI_MEMSET(num_buffer, 0, MN_MAX_IMEI_LENGTH*2);
        SCI_MEMSET(wbuffer, 0, MN_MAX_IMEI_LENGTH*2);
        ReadIMEINum(MN_DUAL_SYS_1  + i, imei_buffer, MN_MAX_IMEI_LENGTH);
        
        ConvertIMEICode2Num(num_buffer, MN_MAX_IMEI_LENGTH*2,imei_buffer, MN_MAX_IMEI_LENGTH);
	 item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;  
         length =  strlen((char*)num_buffer)-1;
        item_data.item_content[1].item_data.text_buffer.wstr_len= length;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = wbuffer;

 	 item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;              
	 item_data.item_content[0].item_data.image_id = sim_image[i];	
        
       MMI_STRNTOWSTR(wbuffer, MN_MAX_IMEI_LENGTH*2, (const uint8*)&num_buffer[1], MN_MAX_IMEI_LENGTH*2-1,length);
        
        GUILIST_AppendItem( list_id, &item_t );    
    }
    return TRUE;
}

LOCAL MMI_RESULT_E HandleIMEIUpdateSuccWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
	switch (msg_id)
	{
		case MSG_APP_OK:
		case MSG_CTL_OK: 
		case MSG_CTL_MIDSK:
		case MSG_APP_WEB:	
		  MMIAPIPHONE_PowerReset();
		  MMK_CloseWin(win_id); 
		break;	 
		case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;	
        case MSG_CLOSE_WINDOW:
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
		    break;
	    default:
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
	        break;
	}
    return (result);
}

/********************************************************************************
NAME:			EngGetFlashInfoWinHandleMsg
DESCRIPTION:	
PARAM IN:		win_id - 
msg_id - 
param - 
PARAM OUT:		
AUTHOR:		ying.xu
DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E EngUpdateIMEIWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;    
    uint16                  index = 0;
    MMI_CTRL_ID_T           list_id = MMIENG_LISTBOX_CTRL_ID;
    //MMI_STRING_T            str = {0};
    //uint16                  str_length = 0;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        UpateIMEIListData(win_id, list_id);
        MMK_SetAtvCtrl(win_id, list_id);
        break;
        
    case MSG_GET_FOCUS:
        UpateIMEIListData(win_id, list_id);
        break;
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
      case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);
        MMK_CreateWin((uint32*)MMIENG_UPDATE_IMEI_EDIT_WIN_TAB, (ADD_DATA)index);
        break;      
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   // case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
   // case MSG_APP_WEB:
	{	 
			MMI_STRING_T  wait_text = {0};
		 	uint32 time_period = 5000;
			 MMI_WIN_ID_T alert_win_id = ENG_UPDATEIMEI_WARNING_ID;
		 	 MMI_GetLabelTextByLang(TXT_IMEIUPDATE_SUCCESS_RESET, &wait_text);
			MMIPUB_OpenAlertWinByTextPtr(&time_period,&wait_text,PNULL,IMAGE_PUBWIN_QUERY,&alert_win_id,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleIMEIUpdateSuccWin);
    	}
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/********************************************************************************
NAME:			EngGetFlashInfoWinHandleMsg
DESCRIPTION:	
PARAM IN:		win_id - 
msg_id - 
param - 
PARAM OUT:		
AUTHOR:		ying.xu
DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E EngUpdateIMEIEditWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;    
    uint32                  index = MMK_GetWinAddDataPtr(win_id );/*lint !e64*/    
    MMI_CTRL_ID_T           ctrl_id = MMIENG_EDITBOX1_CTRL_ID;
    MMI_STRING_T            str = {0};
    //uint16                  str_length = 0;
    uint8 imei_buffer[MN_MAX_IMEI_LENGTH + 1] = {0};
    char num_buffer[2*MN_MAX_IMEI_LENGTH + 1] = {0};    
    wchar wbuffer[MN_MAX_IMEI_LENGTH*2 + 1] = {0};
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            ReadIMEINum(index , imei_buffer,  MN_MAX_IMEI_LENGTH);      
            ConvertIMEICode2Num(num_buffer,  MN_MAX_IMEI_LENGTH*2, imei_buffer, MN_MAX_IMEI_LENGTH);
            str.wstr_len= strlen((char*)num_buffer)-1;
            MMI_STRNTOWSTR(wbuffer, MN_MAX_IMEI_LENGTH*2,  (const uint8*)&num_buffer[1], MN_MAX_IMEI_LENGTH*2 -1,str.wstr_len);
            str.wstr_ptr = wbuffer;
            GUIEDIT_SetString(ctrl_id,  str.wstr_ptr, str.wstr_len);
            /*add for bug110091:*/
            GUIEDIT_SetIm(ctrl_id, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);/*lint !e655*/     
            /*end bug110091*/
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:    
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id, &str);
        MMI_WSTRNTOSTR((uint8*)num_buffer, MN_MAX_IMEI_LENGTH*2, str.wstr_ptr, str.wstr_len,str.wstr_len); 
	MMIAPICOM_Strupper(num_buffer);
	if(str.wstr_len != 15)
	{
	    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR,
	        TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
	        break;
	}
        ConvertNum2IMEICode(imei_buffer,  MN_MAX_IMEI_LENGTH, num_buffer, str.wstr_len);
        if(WriteIMEINum(index , imei_buffer,  MN_MAX_IMEI_LENGTH))
        {
            MMK_CloseWin(win_id);
        }
        else       
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR,
                TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
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
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

#else   //if define ENG_SUPPORT

static MMI_STRING_T s_mmieng_show3result_text1 = {0};
static MMI_STRING_T s_mmieng_show3result_text2 = {0};
static MMI_STRING_T s_mmieng_show3result_text3 = {0};

/********************************************************************************
 NAME:          MMIAPIENG_BandSelectCnf
 DESCRIPTION:   
 PARAM IN:      None 
 PARAM OUT:     None
 AUTHOR:        songbin.zeng
 DATE:          2006.11.16
********************************************************************************/
PUBLIC void MMIAPIENG_BandSelectCnf(DPARAM  param)
{

}

/*****************************************************************************/
//  Description : start chip test
//  Global resource dependence : 
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIENGAPI_StartChipTest(void)
{

}

/*****************************************************************************/
//  Description : MMIAPIENG_OpenIQDataWin
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenIQDataWin(void)
{

}

/*****************************************************************************/
//  Description : MMIAPIENG_AlertIQMode
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_AlertIQMode(void)
{
}

/*****************************************************************************/
//  Description : Open IQ Data Query Win
//  Global resource dependence :                                
//      Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_OpenHandleIQDataQueryWin(BOOLEAN is_succeed)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : is arm log on when power on
//  Global resource dependence :                                
//      Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsUSBLOGON(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : is eng band select cmd
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsBandSelectWinOpen(void)
{
    return FALSE;
}


/********************************************************************************
 NAME:          MMIEng_SetResultWinText
 DESCRIPTION:   
 PARAM IN:      txt1 - 
                txt2 - 
                txt3 - 
 PARAM OUT:     None
 AUTHOR:        allen
 DATE:          2004.09.10
********************************************************************************/
PUBLIC void MMIEng_SetResultWinText(MMI_STRING_T txt1, MMI_STRING_T txt2, MMI_STRING_T txt3)
{
}
#endif//ENG_SUPPORT

/********************************************************************************
 NAME:          MMIENGWIN_OpenNVCountingWin
 DESCRIPTION:   
 PARAM IN:      
 PARAM OUT:     
 AUTHOR:        ying.xu
 DATE:      0   2010.08.17
********************************************************************************/
PUBLIC BOOLEAN MMIENGWIN_OpenNVCountingWin(void)
{
#ifdef ENG_SUPPORT
     MMK_CreateWin((uint32*)MMIENG_NV_COUNTING_WIN_TAB, PNULL);
#endif
#ifdef MMI_SAVE_RUNING_NV_INFO
     MMI_GetAllModuleSumNvSize();
#endif
     return TRUE;
}

#ifdef ENG_SUPPORT
/*****************************************************************************/
//  Description : start HandleRunningNVCountingWinMsg for pc link
//  Global resource dependence : none
//  Author: ying.xu
//  Note:   2010.08.17
/*****************************************************************************/
LOCAL uint32 GetRunningNVCountingInfo(char *buffer, uint32 buffer_lengh)
{
    char tmp_buf[128] = {0};
    uint32 tmp_buf_length = 127;    
    uint32 offset = 0;

    uint32 str_length = 0;
    uint32 i = 0;
    
    i = MMI_GetAllNVSize();
    SCI_MEMSET(tmp_buf, 0 ,tmp_buf_length + 1) ;
    sprintf(tmp_buf, "All running NV size: %d KB\n", i);    
    str_length = SCI_STRLEN(tmp_buf) ;
    if( offset + str_length > buffer_lengh )
    {
        return offset ;
    }
    SCI_MEMCPY(buffer + offset , tmp_buf,  str_length );
    offset += str_length;
    
    i = MMI_GetAllModuleSumNvSize() ;
    SCI_MEMSET(tmp_buf, 0 ,tmp_buf_length + 1) ;
    sprintf(tmp_buf, "MMI running NV size: %d KB\n", i);    
    str_length = SCI_STRLEN(tmp_buf) ;
    if( offset + str_length > buffer_lengh )
    {
        return offset ;
    }
    SCI_MEMCPY(buffer + offset , tmp_buf,  str_length );
    offset += str_length;
    
    i = MMI_GetAllMMINVItem();
    SCI_MEMSET(tmp_buf, 0 ,tmp_buf_length + 1) ;
    sprintf(tmp_buf, "MMI running NV item num: %d \n", i);
    str_length = SCI_STRLEN(tmp_buf) ;
    if( offset + str_length > buffer_lengh )
    {
        return offset ;
    }
    SCI_MEMCPY(buffer + offset , tmp_buf,  str_length );
    offset += str_length;
#if 0   
    i= MMI_GetFlashSecterSize() ;
    SCI_MEMSET(tmp_buf, 0 ,tmp_buf_length + 1) ;
    sprintf(tmp_buf, "Flash sector size: %d KB\n", i);
    str_length = SCI_STRLEN(tmp_buf) ;
    if( offset + str_length > buffer_lengh )
    {
        return offset ;
    }
    SCI_MEMCPY(buffer + offset , tmp_buf,  str_length );
    offset += str_length;
    
    i = MMI_GetMinimumNVConfig();
    SCI_MEMSET(tmp_buf, 0 ,tmp_buf_length + 1) ;
    sprintf(tmp_buf, "Minimum requried sector num: %d \n", i);
    str_length = SCI_STRLEN(tmp_buf) ;
    if( offset + str_length > buffer_lengh )
    {
        return offset ;
    }
    SCI_MEMCPY(buffer + offset , tmp_buf,  str_length );
    offset += str_length;
    
    i = MMI_GetOptimumNVConfig() ;
    SCI_MEMSET(tmp_buf, 0 ,tmp_buf_length + 1) ;
    sprintf(tmp_buf, "Optimum requried sector num: %d ", i);
    str_length = SCI_STRLEN(tmp_buf) ;
    if( offset + str_length > buffer_lengh )
    {
        return offset ;
    }
    SCI_MEMCPY(buffer + offset , tmp_buf,  str_length );
    offset += str_length;
#endif
    return offset  ;
}

/*****************************************************************************/
//  Description : start HandleRunningNVCountingWinMsg for pc link
//  Global resource dependence : none
//  Author: ying.xu
//  Note:   2010.08.17
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRunningNVCountingWinMsg(
                                                 MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                 DPARAM             param       // 相应消息的参数
                                                 )
{
    
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {           
            wchar wstr[1024] = {0};
            char str[1024] = {0};
            uint32 buffer_length = 1023;
            uint32 str_length = 0;
            str_length = GetRunningNVCountingInfo(str, buffer_length) ;
    
            MMI_STRNTOWSTR( wstr, buffer_length, (uint8*)str, str_length, str_length );         
    
        GUITEXT_SetString(MMIENG_NV_COUNTING_TEXTBOX_CTRL_ID,  wstr,  str_length, FALSE);
        MMK_SetAtvCtrl(win_id,  MMIENG_NV_COUNTING_TEXTBOX_CTRL_ID);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_WEB:       
    case MSG_KEYDOWN_OK:
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif

#ifdef WRE_SUPPORT
/*****************************************************************************/
//  Description : EngShowWREVersionWinHandleMsg
//  Global resource dependence : none
//  Author: juan.wu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E EngShowWREVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    char   * str_ptr    = PNULL;
    uint16          str_len = 0;
    uint16           uint16_str[MMIENG_WRE_VER_MAX_LEN +1] = {0};
    uint16          uint16_str_len = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIENG_SHOW_WRE_VERSION_CTRL_ID);
        str_ptr = MMIWRE_GetVersionInfo();
        str_len = SCI_STRLEN((char*)str_ptr);
        str_len = MIN(str_len,MMIENG_WRE_VER_MAX_LEN);
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)str_ptr, str_len );
        GUITEXT_SetString(MMIENG_SHOW_WRE_VERSION_CTRL_ID, uint16_str, uint16_str_len, FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#if defined MMI_PDA_SUPPORT
        {
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
        }
#endif   
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_MENU:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;        
    }
    return result;
}
#endif


/********************************************************************************
 NAME:          EngDTMFSetTimerWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:     
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngDTMFSetTimerWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
       MMI_RESULT_E result = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO	    lcd_dev_info = {GUI_MAIN_LCD_ID};
	GUI_RECT_T              rect = {0, 0, 0, 0};
	MMI_CTRL_ID_T  edit1_ctrl_id = MMIENG_DTMF_EDIT_CTRL_ID;
	MMI_STRING_T  one_hour_value = {0};
	uint32        one_hour = 0;
	GUI_RECT_T editbox_rc  = {0,0,10,20};
	
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
		GUIEDIT_SetStyle(edit1_ctrl_id,	GUIEDIT_STYLE_MULTI_DYN_DOWN);//设置显示风格
		GUIEDIT_SetIm(edit1_ctrl_id, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);	
		GUIEDIT_SetFont(edit1_ctrl_id, SONG_FONT_14);
		GUIEDIT_SetSoftkeyEx(edit1_ctrl_id, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);
		MMK_SetAtvCtrl(win_id, edit1_ctrl_id);
        break;
    case MSG_FULL_PAINT:
		GUI_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);
        break;
	case MSG_CTL_EDITBOX_UPDATE_STRNUM:
		//MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
		break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;  
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
		GUIEDIT_GetString(edit1_ctrl_id,&one_hour_value);
		if((one_hour_value.wstr_len > 0))
		{
		    MMIAPICOM_WstrToNum(one_hour_value.wstr_ptr, one_hour_value.wstr_len, &one_hour);
           	}
		SCI_TRACE_LOW("[eng] EngDTMFTimerChangeWinHandleMsg one_hour = %d", one_hour);
		
              MMITEST_DTMFSetTime(one_hour);			
              MMK_CloseWin(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#ifdef ENG_SECONDEDAC_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence :                                
//      Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsSECONDEDACON(void)
{
    BOOLEAN is_on = TRUE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMIENG_NV_SECONDDAC_ID,&is_on,return_value);        
    if(MN_RETURN_SUCCESS!=return_value)
    {
        is_on = TRUE;      
        MMINV_WRITE(MMIENG_NV_SECONDDAC_ID,&is_on);
    }    
    SCI_TRACE_LOW("MMIAPIENG_IsSECONDEDACON is_on = %d", is_on);
    return is_on;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :                                
//      Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetSECONDEDACON(void)
{
    BOOLEAN is_on = TRUE;
#ifndef WIN32
    L1_SET_DSPPara(66,0,0,0); 
#endif
    MMINV_WRITE(MMIENG_NV_SECONDDAC_ID,&is_on);
    SCI_TRACE_LOW("MMIAPIENG_SetSECONDEDACON ");
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :                                
//      Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetSECONDEDACOFF(void)
{
    BOOLEAN is_on = FALSE;
#ifndef WIN32
    L1_SET_DSPPara(66,1,0,0); 
#endif
    MMINV_WRITE(MMIENG_NV_SECONDDAC_ID,&is_on);
    SCI_TRACE_LOW("MMIAPIENG_SetSECONDEDACOFF ");
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :                                
//      Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SECONDEDAC_InitforPowerOn(void)
{
    BOOLEAN is_on = MMIAPIENG_IsSECONDEDACON();
    SCI_TRACE_LOW("MMIAPIENG_SECONDEDAC_InitforPowerOn is_on =%d",is_on);
    if(is_on)
    {
        MMIAPIENG_SetSECONDEDACON();
    }
    else
    {
        MMIAPIENG_SetSECONDEDACOFF();
    }
}

/********************************************************************************
 NAME:          EngDspCardLogEnableWinHandleMsg
 DESCRIPTION:   
 PARAM IN:      win_id - 
                msg_id - 
                param - 
 PARAM OUT:     
 AUTHOR:        
 DATE:          
********************************************************************************/
LOCAL MMI_RESULT_E EngSECONDEDACEnableWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    BOOLEAN is_on = FALSE;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2  

        is_on = (BOOLEAN)MMIAPIENG_IsSECONDEDACON();
        SCI_TRACE_LOW("second DAC is_on %d",is_on);
 
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);         

        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, is_on, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,is_on);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);     
        break;
        
    case MSG_FULL_PAINT:
        break;  
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    
        is_on = (BOOLEAN)GUILIST_GetCurItemIndex(ctrl_id);  
        if(is_on==FALSE)
        {
            MMIAPIENG_SetSECONDEDACOFF();
        }
        else
        {
            MMIAPIENG_SetSECONDEDACON();
        }
        MMK_CloseWin(win_id);
		
        result = MMI_RESULT_TRUE;  
        break;      
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#endif

#if 0
LOCAL MMIPB_ERROR_E MMIENG_AddContact(MMI_STRING_T *name,uint16 name_idx,uint8 photoid,uint8 *phone_num,int16 phone_num_len)
{
    MMIPB_CONTACT_T  contact_info = {0};
    uint32           pos          = 0;
    BOOLEAN          is_exist     = FALSE;
    MMI_PARTY_NUMBER_T  party_num = {0};
    MMIPB_ERROR_E  operator_error = MMIPB_ERROR_SUCCESS;

    if(PNULL == name)
    {
        SCI_TRACE_LOW("MMIENG_AddContact name is null");
        return FALSE;
    }

    if(PNULL == phone_num)
    {
        SCI_TRACE_LOW("MMIENG_AddContact phonenum is null");
        return FALSE;
    }

    if((!MMIPB_IsPbReady()))
    {
        SCI_TRACE_LOW("MMIPB_FillPhoneContactFull PB NOT READY");
        return FALSE;
    }

    SCI_MEMSET(&contact_info, 0x0, sizeof(contact_info));

    if (MMIAPICOM_GenPartyNumber((uint8 *)(phone_num), phone_num_len, &party_num))
    {
        contact_info.number[0].npi_ton = (uint8)((party_num.number_type << 0X04) | MMIPB_DEFAULT_NPI);
        contact_info.number[0].number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
        SCI_MEMCPY(contact_info.number[0].number, party_num.bcd_num, sizeof(party_num.bcd_num));
    }

    contact_info.name.wstr_len = name->wstr_len;
    contact_info.storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);;
    MMIAPICOM_Wstrncpy(contact_info.name.wstr, name->wstr_ptr, contact_info.name.wstr_len);

    contact_info.contact_id = (name_idx + 1);//cintact id is from 1,save as order
    contact_info.photo_id_flag = (photoid + 1);

    //judge weather contact has existed
    is_exist = MMIPB_IsNameExist(name,contact_info.storage_id,&pos);
    if(TRUE == is_exist)
    {
       operator_error =  MMIAPIPB_ModifyContact(&contact_info);
    }else
    {
      operator_error =  MMIPB_AddContact(&contact_info);
    }

    return operator_error;
}

LOCAL MMI_RESULT_E  EngAddContactHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret_value      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id       = MMIENG_COMMON_LIST_CTRL1_ID;
    uint16       cur_selection  = 0;
    MMI_STRING_T itemStr        = {0};
    char* str = PNULL;
    uint8 i = 0;
    wchar wstr[10] = {0};
    MMI_STRING_T text = {0};

    uint32  droplist_name_id[6] = {
                                    TXT_ENG_FATHER, //father
                                    TXT_ENG_MOTHER, //mother
                                    TXT_ENG_GRANDPA,//grandpa
                                    TXT_ENG_GRANDMA,//gradma
                                    TXT_ENG_TEACHER,//teacher
                                    TXT_ENG_CLASSMATE  //classmate
                                  };
    uint32 droplist_imageid[6]  = {
                                    TXT_ENG_IMAGE1,//image1
                                    TXT_ENG_IMAGE2,//image2
                                    TXT_ENG_IMAGE3,//image3
                                    TXT_ENG_IMAGE4,//image4
                                    TXT_ENG_IMAGE5,//image5
                                    TXT_ENG_IMAGE6 //image6
                                  };

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMI_HANDLE_T form_ctrl                  = MMK_GetCtrlHandleByWin(win_id, MMIENG_FORM_CTRL_ID);
            MMI_HANDLE_T drop_list_name_ctrl        = MMK_GetCtrlHandleByWin(win_id, MMIENG_COMMON_LIST_CTRL1_ID);
            MMI_HANDLE_T drop_list_icon_ctrl        = MMK_GetCtrlHandleByWin(win_id, MMIENG_DROP_LIST_ICON_ID);
            MMI_HANDLE_T form1                      = MMK_GetCtrlHandleByWin(win_id, MMIENG_FORM_NAME_ID);
            MMI_HANDLE_T form2                      = MMK_GetCtrlHandleByWin(win_id, MMIENG_FORM_ICON_ID);
            MMI_HANDLE_T form3                      = MMK_GetCtrlHandleByWin(win_id, MMIENG_FORM_PHONENUM_ID);
            GUIFORM_CHILD_HEIGHT_T height_info = {0};
            GUI_BG_T               display_bg = {0};
            GUIFORM_CHILD_HEIGHT_T form_height = {0};
            GUIFORM_CHILD_WIDTH_T form_width  = {0};

            display_bg.bg_type = GUI_BG_COLOR;
            display_bg.color   = MMI_WHITE_COLOR;

            //form
            GUIFORM_SetType(MMIENG_FORM_CTRL_ID, GUIFORM_TYPE_TP);
            GUIFORM_SetBg(MMIENG_FORM_CTRL_ID,&display_bg);
            GUIFORM_PermitChildFont(form_ctrl,FALSE);

            //set unit
            GUIFORM_SetStyle(MMIENG_FORM_NAME_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_FORM_ICON_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_FORM_PHONENUM_ID,GUIFORM_STYLE_UNIT);
            //height value
            form_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            form_height.add_data = 70;
            //width value
            form_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
            form_width.add_data = 65;
            //set height
            GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_FORM_NAME_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FORM_NAME_ID,MMIENG_COMMON_LABEL_CTRL1_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FORM_NAME_ID,MMIENG_COMMON_LIST_CTRL1_ID,&form_height);
            CTRLFORM_SetChildWidth(form1,drop_list_name_ctrl,&form_width);

            GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_FORM_ICON_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FORM_ICON_ID,MMIENG_COMMON_LABEL_CTRL2_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FORM_ICON_ID,MMIENG_DROP_LIST_ICON_ID,&form_height);
            CTRLFORM_SetChildWidth(form2,drop_list_icon_ctrl,&form_width);

            GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_FORM_PHONENUM_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FORM_PHONENUM_ID,MMIENG_COMMON_LABEL_CTRL3_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FORM_PHONENUM_ID,MMIENG_COMMON_EDIT_CTRL3_ID,&form_height);
            CTRLFORM_SetChildWidth(form3,MMK_GetCtrlHandleByWin(win_id, MMIENG_COMMON_EDIT_CTRL3_ID),&form_width);

            str = "Name";
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 5, (uint8*)str, text.wstr_len, text.wstr_len );
            GUILABEL_SetText(MMIENG_COMMON_LABEL_CTRL1_ID, &text, TRUE);

            height_info.type = GUIFORM_CHILD_HEIGHT_AUTO;
            GUIFORM_SetChildHeight(form_ctrl, drop_list_name_ctrl, &height_info);
            GUIDROPDOWNLIST_SetVisible(drop_list_name_ctrl,TRUE,TRUE);
            GUIDROPDOWNLIST_SetMaxItem(drop_list_name_ctrl,6);

            for(i=0;i<ARR_SIZE(droplist_name_id);i++)
            {
                GUIDROPDOWNLIST_AppendItemById(drop_list_name_ctrl,droplist_name_id[i]);
            }
            GUIDROPDOWNLIST_SetCurItemIndex(drop_list_name_ctrl,0);

            //icon id
            GUIFORM_SetChildHeight(form_ctrl, drop_list_icon_ctrl, &height_info);

            str = "ImageID";
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 8, (uint8*)str, text.wstr_len, text.wstr_len );
            GUILABEL_SetText(MMIENG_COMMON_LABEL_CTRL2_ID, &text, FALSE);

            GUIDROPDOWNLIST_SetMaxItem(drop_list_icon_ctrl,6);
            for(i=0;i<ARR_SIZE(droplist_imageid);i++)
            {
                GUIDROPDOWNLIST_AppendItemById(drop_list_icon_ctrl,droplist_imageid[i]);
            }
            GUIDROPDOWNLIST_SetCurItemIndex(drop_list_icon_ctrl,0);

            //phone number
            str = "Number";
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 7, (uint8*)str, text.wstr_len, text.wstr_len );
            GUILABEL_SetText(MMIENG_COMMON_LABEL_CTRL3_ID, &text, FALSE);
            GUIEDIT_SetStyle(MMIENG_COMMON_EDIT_CTRL3_ID, GUIEDIT_STYLE_SINGLE);

            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
            break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_APP_MENU:
        case MSG_CTL_OK:
        {
            uint16 name_index = 0;
            uint16 image_index = 0;
            MMI_STRING_T name = {0};
            MMI_STRING_T phone_num = {0};
            char str_phone_num[MMIPB_BCD_NUMBER_MAX_LEN+1] = {0};//fixbug:1340385

            MMIPB_ERROR_E  add_ret = MMIPB_ERROR_SUCCESS;

            name_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIENG_COMMON_LIST_CTRL1_ID);
            image_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIENG_DROP_LIST_ICON_ID);
            if((0 <= name_index) && (6 > name_index))
            {
                MMI_GetLabelTextByLang(droplist_name_id[name_index],&name); //get name string
            }
            GUIEDIT_GetString(MMIENG_COMMON_EDIT_CTRL3_ID,&phone_num); //get phone number
            if (0 < MMIAPICOM_Wstrlen(phone_num.wstr_ptr))
            {
                if (TRUE == IsSCAddrValid(&phone_num))
                {
                    MMI_WSTRNTOSTR((uint8 *)str_phone_num,
                                    20,
                                    phone_num.wstr_ptr,
                                    20,
                                    phone_num.wstr_len
                                    );          
                }
                else
                {
                    //提示输入错误
                    MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
                }
            }else
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMM_INPUT_NULL);
            }
            add_ret = MMIENG_AddContact(&name,name_index,(uint8)image_index,str_phone_num,strlen(str_phone_num));
            if(MMIPB_ERROR_SUCCESS == add_ret)
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMM_SAVE_SUCCESS);
                MMK_CloseWin(win_id);
            }else
            {
                 MMIPUB_OpenAlertWarningWin(TXT_MMI_SAVEFAILED);
            }
        }
            break;
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            ret_value = MMI_RESULT_FALSE;
            break;
    }
    return ret_value;
}
PUBLIC void MMIENG_AddContactTest(void)
{
    MMK_CreateWin((uint32*)MMIENG_ADD_CONTACT_WIN_TAB, PNULL);
}
#endif

//================================================================================================//
//================================     ENG MEM PRESS TEST START      =============================//
//================================================================================================//

#include "mem_sim_eng_data.h"
#include "mmicom_trace.h"

#define UDISK_ENG_FOLDER       "D:\\UDISKTEST"
#define UDISK_ENG_FILE_FOLDER  "D:\\UDISKTEST\\"
#define SD_ENG_FOLDER          "E:\\SDTEST"
#define SD_ENG_FILE_FOLDER     "E:\\SDTEST\\"
#define ENG_FILE_EXTENSION     "txt"

#define MEM_ENG_TIMER_OUT               (40)
#define ENG_MEM_PRESS_LIST_NUM          (2)
#define ENG_MEM_PRESS_FOLDER_NAME_LEN   (64)
#define ENG_MEM_FILE_SIZE               (300)// 单位: KB
typedef enum
{
    MEM_PRESS_TYPE_FOR_UDISK,
    MEM_PRESS_TYPE_FOR_SD,
    MEM_PRESS_TYPE_MAX
} ENG_MEM_PRESS_TEST_TYPE_E;

typedef enum
{
    MEM_ENG_MSG_DEFAULT,
    MEM_ENG_MSG_RUN,
    MEM_ENG_MSG_STOP,
    MEM_ENG_MSG_REPEAT,
    MEM_ENG_MSG_ERROR
} ENG_MEM_PRESS_TEST_MSG_E;

static uint32   s_mem_eng_file_index = 0;
static uint8    s_mem_eng_timer_id   = 0;
static uint8    s_mem_eng_stop_flag  = 0;
static ENG_MEM_PRESS_TEST_MSG_E  s_mem_eng_msg_flag= MEM_ENG_MSG_DEFAULT;
static ENG_MEM_PRESS_TEST_TYPE_E s_mem_eng_type    = MEM_PRESS_TYPE_MAX;

LOCAL SFS_ERROR_E MEM_EngCheckDeviceExist(void)
{
    MMIFILE_DEVICE_E device = MMI_DEVICE_NUM;
    SFS_ERROR_E error = SFS_ERROR_NONE;

    TRACE_APP_ENGINEERMODE("s_mem_eng_type = %d",s_mem_eng_type);
    if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_UDISK)
    {
        device = MMI_DEVICE_UDISK;
    }
    else if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_SD)
    {
        device = MMI_DEVICE_SDCARD;
    }
    else
    {
        TRACE_APP_ENGINEERMODE("get device path failed , s_mem_eng_type = %d",s_mem_eng_type);
    }

    if (MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device))) //设备已经注册了
    {
        error = SFS_ERROR_NONE;
        TRACE_APP_ENGINEERMODE("MEM is Exist");
    }
    else
    {
        MMIAPIFMM_RegisterDevice(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device));
        error = MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device));
        if(error !=SFS_ERROR_NONE)
        {
            TRACE_APP_ENGINEERMODE("MEM is Register succeed");
        }
        else
        {
            TRACE_APP_ENGINEERMODE("MEM is Register failed,error = %d",error);
        }
    }

    return error;
}
LOCAL BOOLEAN MEM_EngCreateDirectoryDir(void)
{
    wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    int full_path_len = 0;
    BOOLEAN result = TRUE;

    TRACE_APP_ENGINEERMODE("creat folder,s_mem_eng_type = %d",s_mem_eng_type);
    if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_UDISK)
    {
        full_path_len = strlen(UDISK_ENG_FOLDER);
        MMI_STRNTOWSTR(full_path_name,MMIFILE_FULL_PATH_MAX_LEN, (char *) UDISK_ENG_FOLDER,full_path_len,full_path_len);
    }
    else if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_SD)
    {
        full_path_len = strlen(SD_ENG_FOLDER);
        MMI_STRNTOWSTR(full_path_name,MMIFILE_FULL_PATH_MAX_LEN, (char *) SD_ENG_FOLDER,full_path_len,full_path_len);
    }
    else
    {
        TRACE_APP_ENGINEERMODE("MEM_EngCreateDirectoryDir failed , s_mem_eng_type = %d",s_mem_eng_type);
    }

    full_path_len = MMIAPICOM_Wstrlen(full_path_name);
    if (!MMIAPIFMM_IsFolderExist (full_path_name, full_path_len))
    {
        result = MMIAPIFMM_CreateDir (full_path_name, full_path_len);
    }

    return result;
}

LOCAL void MEM_EngDelDirectoryDir(void)
{
    wchar path_d[] = L"d:";
    wchar path_e[] = L"e:";
    int full_file_len = 0;
    wchar full_filename[MMIFILE_FULL_PATH_MAX_LEN +1]= {0};
    char strfilename[MMIFILE_FULL_PATH_MAX_LEN + 1]= {0};

    TRACE_APP_ENGINEERMODE("delete folder,s_mem_eng_type = %d",s_mem_eng_type);

    if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_UDISK)
    {
        strcpy(strfilename,"UDISKTEST");
        full_file_len = strlen(strfilename);
        MMI_STRNTOWSTR(full_filename,MMIFILE_FULL_PATH_MAX_LEN, (char *) strfilename,MMIFILE_FULL_PATH_MAX_LEN,full_file_len);
        MMIAPIFMM_DeleteTree(path_d, full_filename);
    }
    else if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_SD)
    {
        strcpy(strfilename,"SDTEST");
        full_file_len = strlen(strfilename);
        MMI_STRNTOWSTR(full_filename,MMIFILE_FULL_PATH_MAX_LEN, (char *) strfilename,MMIFILE_FULL_PATH_MAX_LEN,full_file_len);
        MMIAPIFMM_DeleteTree(path_e, full_filename);
    }
    else
    {
        TRACE_APP_ENGINEERMODE("delete folder failed , s_mem_eng_type = %d",s_mem_eng_type);
    }

}

LOCAL void MEM_EngGenerateFileName(char *str,uint32 index)
{
    TRACE_APP_ENGINEERMODE("s_mem_eng_type = %d",s_mem_eng_type);
    if (str == NULL)
    {
        return;
    }
    else
    {
        char date_char[20] = {0};
        char time_char[20] = {0};
        SCI_DATE_T  sys_date = {0};
        SCI_TIMEX_T time_ptr = {0};

        TM_GetSysDate(&sys_date);
        sprintf(date_char,"%02d%02d",sys_date.mon,sys_date.mday);

        TM_GetSysTimeEx(&time_ptr);
        sprintf(time_char,"%02d%02d%02d_%05d",time_ptr.hour,time_ptr.min,time_ptr.sec,index);

        if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_UDISK)
        {
            sprintf(str, "%s%s%s.%s", UDISK_ENG_FILE_FOLDER,date_char,time_char,ENG_FILE_EXTENSION);
        }
        else if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_SD)
        {
            sprintf(str, "%s%s%s.%s", SD_ENG_FILE_FOLDER,date_char,time_char,ENG_FILE_EXTENSION);
        }
        else
        {
            TRACE_APP_ENGINEERMODE("set file name failed , s_mem_eng_type = %d",s_mem_eng_type);
        }
    }
}

LOCAL MMIFILE_HANDLE MEM_EngCreatefile(char* strfilename, int full_file_len,BOOLEAN is_write)
{
    int8 fileNameLen = 0;
    wchar full_filename[ENG_MEM_PRESS_FOLDER_NAME_LEN +1]= {0};
    MMIFILE_HANDLE hFile = NULL;

    TRACE_APP_ENGINEERMODE("creat file,strfilename = %s",strfilename);
    MMI_STRNTOWSTR(full_filename,ENG_MEM_PRESS_FOLDER_NAME_LEN, (char *) strfilename,ENG_MEM_PRESS_FOLDER_NAME_LEN,full_file_len);

    if(is_write)
    {
    if (!MMIAPIFMM_IsFileExist(full_filename,MMIAPICOM_Wstrlen(full_filename)))
    {
        hFile = MMIAPIFMM_CreateFile(full_filename, SFS_MODE_WRITE | SFS_MODE_CREATE_NEW, NULL, NULL);
    }
        else
        {
            TRACE_APP_ENGINEERMODE("this file has already exist!");
        }
    }
    else
    {
        if (MMIAPIFMM_IsFileExist(full_filename,MMIAPICOM_Wstrlen(full_filename)))
        {
            hFile = MMIAPIFMM_CreateFile(full_filename, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);
        }
        else
        {
            TRACE_APP_ENGINEERMODE("this file has not exist!");
        }
    }

    TRACE_APP_ENGINEERMODE("get file handle(%d),hFile = %d",is_write,hFile);
    return hFile;
}

LOCAL uint8 MEM_EngGenFile(uint32 index)
{
    uint32 data_size = MEM_SIM_ENG_DATA_SIZE;
    uint32 buf_write = 0;
    uint32 buf_read = 0;
    uint32 i = 0;
    uint8 *data_buf_ptr = NULL;
    char strfilename[ENG_MEM_PRESS_FOLDER_NAME_LEN + 1]= {0};
    int full_file_len = 0;
    MMIFILE_HANDLE hHandle = 0;

    //1.获取将要创建的文件全路径名称
    MEM_EngGenerateFileName(strfilename,index);
    full_file_len = strlen(strfilename);

    //2.创建文件并写入数据
    hHandle = MEM_EngCreatefile(strfilename,full_file_len,TRUE);
    if (hHandle == 0)
    {
        MMIAPIFMM_CloseFile(hHandle);
        return 0;
    }

    for (i = 0; i < 1000; i++)
    {
        MMIAPIFMM_WriteFile(hHandle, mem_eng_file_memo, data_size, &buf_write, NULL);
        if (buf_write == 0)
        break;
    }
    MMIAPIFMM_CloseFile(hHandle);
    if (buf_write == 0)
    {
        TRACE_APP_ENGINEERMODE("buf_write failed");
        return 0;
    }

    //3.读取文件并与指定数组比较，判断写入是否成功
    hHandle = MEM_EngCreatefile(strfilename,full_file_len,FALSE);
    if (hHandle == 0)
    {
        MMIAPIFMM_CloseFile(hHandle);
        return 0;
}

    data_buf_ptr = SCI_ALLOCA(sizeof(uint8)*data_size);
    if(NULL == data_buf_ptr)
{
        MMIAPIFMM_CloseFile(hHandle);
        TRACE_APP_ENGINEERMODE("alloca data_buf_ptr mem failed");
        return 0;
    }
    for (i = 0; i < 1000; i++)
    {
        SCI_MEMSET( data_buf_ptr, 0x00, data_size);
        MMIAPIFMM_ReadFile(hHandle, data_buf_ptr, data_size, &buf_read, NULL);
        if (buf_read == 0)
            break;
        if(0 != SCI_MEMCMP(mem_eng_file_memo,data_buf_ptr,data_size))
        {
            if(data_buf_ptr != PNULL)
            {
                SCI_FREE(data_buf_ptr);
                data_buf_ptr = PNULL;
        }
            MMIAPIFMM_CloseFile(hHandle);
            TRACE_APP_ENGINEERMODE("compare file failed");
            return 0;
    }
    }

    //4.释放空间，关闭文件
    if(data_buf_ptr != PNULL)
    {
        SCI_FREE(data_buf_ptr);
        data_buf_ptr = PNULL;
    }
    MMIAPIFMM_CloseFile(hHandle);
    if (buf_read == 0)
    {
        TRACE_APP_ENGINEERMODE("buf_read failed");
        return 0;
    }

    TRACE_APP_ENGINEERMODE("write and read file pass,index = %d",index);

    return 1;
}

LOCAL void MEM_EngPressTimerStart(MMI_WIN_ID_T win_id)
{
    TRACE_APP_ENGINEERMODE("start s_mem_eng_stop_flag = %d",s_mem_eng_stop_flag);
    s_mem_eng_stop_flag = 0;
    if (s_mem_eng_timer_id != 0)
    {
        MMK_StopTimer(s_mem_eng_timer_id);
        s_mem_eng_timer_id = 0;
    }

    s_mem_eng_file_index = 0;

    if( SFS_NO_ERROR != MEM_EngCheckDeviceExist())
    {
        TRACE_APP_ENGINEERMODE("device is not exist!");
    }
    if(FALSE == MEM_EngCreateDirectoryDir())
    {
        TRACE_APP_ENGINEERMODE("create folder failed!");
    }

    s_mem_eng_timer_id = MMK_CreateWinTimer(win_id, MEM_ENG_TIMER_OUT, FALSE);
    s_mem_eng_msg_flag = MEM_ENG_MSG_RUN;
    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);

}

LOCAL void MEM_EngPressTimerStop(MMI_WIN_ID_T win_id)
{
    TRACE_APP_ENGINEERMODE("stop_flag = %d, timer_id = %d",s_mem_eng_stop_flag,s_mem_eng_timer_id);

    s_mem_eng_msg_flag = MEM_ENG_MSG_STOP;

    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);

    s_mem_eng_stop_flag = 1;
    if (s_mem_eng_timer_id == 0)
    {
        s_mem_eng_msg_flag = MEM_ENG_MSG_DEFAULT;
        MMK_CloseWin(MMIENG_MEM_PRESS_TEST_WIN_ID);
    }
}

LOCAL void MEM_EngPressTimerOut(MMI_WIN_ID_T win_id)
{
    MMIFILE_DEVICE_E device = MMI_DEVICE_NUM;
    SFS_ERROR_E error = SFS_ERROR_NONE;
    uint32 freeSpaceHigh = 0;
    uint32 freeSpaceLow = 0;
    uint32 totalFreeSpace = 0;
    BOOLEAN result =FALSE;

    TRACE_APP_ENGINEERMODE("s_mem_eng_stop_flag = %d",s_mem_eng_stop_flag);

    if (s_mem_eng_timer_id != 0)
    {
        MMK_StopTimer(s_mem_eng_timer_id);
        s_mem_eng_timer_id = 0;
    }

    // 1.判断当前是否执行cancel操作
    if (s_mem_eng_stop_flag)
    {
        MEM_EngDelDirectoryDir();
        MMK_CloseWin(MMIENG_MEM_PRESS_TEST_WIN_ID);
        s_mem_eng_msg_flag = MEM_ENG_MSG_DEFAULT;
        return;
    }

    //2.判断U盘内存剩余空间
    if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_UDISK)
    {
        device = MMI_DEVICE_UDISK;
    }
    else if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_SD)
    {
        device = MMI_DEVICE_SDCARD;
    }
    else
    {
        TRACE_APP_ENGINEERMODE("get device path failed , s_mem_eng_type = %d",s_mem_eng_type);
    }
    result = MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(device),
                                 MMIAPIFMM_GetDevicePathLen(device),
                                 &freeSpaceHigh,
                                 &freeSpaceLow);
    totalFreeSpace = (freeSpaceHigh << 22) | (freeSpaceLow >> 10);

    //3.当U盘内存剩余空间小于单个文件大小时，删除整个文件夹，重新开始新的循环
    if(totalFreeSpace < ENG_MEM_FILE_SIZE)
    {
        s_mem_eng_file_index = 0;
        s_mem_eng_msg_flag = MEM_ENG_MSG_REPEAT;

        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        MEM_EngDelDirectoryDir();
        MEM_EngPressTimerStart(win_id);
        return;
    }

    //4.创建/写入/读取/对比 文件，如有出错，则删除整个文件，结束循环；反之，创建的index++
    if (!MEM_EngGenFile(s_mem_eng_file_index))
    {
        s_mem_eng_file_index = 0;
        s_mem_eng_msg_flag = MEM_ENG_MSG_ERROR;
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        MEM_EngDelDirectoryDir();
        return;
    }
    else
    {
        s_mem_eng_file_index++;
    }

    //5.启动下一个Timer
    s_mem_eng_timer_id = MMK_CreateWinTimer(win_id, MEM_ENG_TIMER_OUT, FALSE);
    return;

}

/********************************************************************************
 NAME:          EngMemPressTestWinHandleMsg
 DESCRIPTION:
 PARAM IN:      win_id
                msg_id
                param
 PARAM OUT:
 AUTHOR:        fangfang.yao
 DATE:          2020.04.20
********************************************************************************/
LOCAL MMI_RESULT_E EngMemPressTestWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    wchar wstr[200]={0};
    char str[100] = {0};
    uint16 len = 0;

    TRACE_APP_ENGINEERMODE("mem press test Enter!");
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMI_TEXT_ID_T title_id = 0;
            MMK_SetAtvCtrl(win_id, MMIENG_TEXTBOX1_CTRL_ID);

            if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_UDISK)
            {
                title_id = TXT_ENG_UDISK_PRESS_INFO;
            }
            else if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_SD)
            {
                title_id = TXT_ENG_SD_PRESS_INFO;
            }
            else
            {
                TRACE_APP_ENGINEERMODE("set win title failed , s_mem_eng_type = %d",s_mem_eng_type);
            }
            GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),title_id, FALSE);
        }
        break;

        case MSG_CTL_OK:
        {
            MEM_EngPressTimerStart(win_id);
        }
        break;

        case MSG_CTL_MIDSK:
        case MSG_CTL_CANCEL:
        case MSG_KEYDOWN_RED:
        {
            MEM_EngPressTimerStop(win_id);
        }
        break;

        case MSG_TIMER:
        {
            MEM_EngPressTimerOut(win_id);
        }
        break;
        case MSG_FULL_PAINT:
        {
            memset(str,0,sizeof(str));
            if (s_mem_eng_msg_flag == MEM_ENG_MSG_RUN)
            {
                if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_UDISK)
                {
                    strcpy(str,"UDISK test running ...");
                }
                else if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_SD)
                {
                    strcpy(str,"SD test running ...");
                }
                else
                {
                    strcpy(str,"type get wrong ...");
                }
            }
            else if (s_mem_eng_msg_flag == MEM_ENG_MSG_STOP)
            {
                if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_UDISK)
                {
                    strcpy(str,"Delete udisk test infor,Please wait ...");
                }
                else if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_SD)
                {
                    strcpy(str,"Delete sd test infor,Please wait ...");
                }
                else
                {
                    strcpy(str,"type get wrong ...");
                }
            }
            else if(s_mem_eng_msg_flag == MEM_ENG_MSG_REPEAT)
            {
                if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_UDISK)
                {
                    strcpy(str,"End of cycle,Delete udisk test infor,Please wait ...");
                }
                else if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_SD)
                {
                    strcpy(str,"End of cycle,Delete sd test infor,Please wait ...");
                }
                else
                {
                    strcpy(str,"type get wrong ...");
                }
            }
            else if(s_mem_eng_msg_flag == MEM_ENG_MSG_ERROR)
            {
                if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_UDISK)
                {
                    strcpy(str,"Run udisk card press test,write or read may be wrong .");
                }
                else if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_SD)
                {
                    strcpy(str,"Run sd card press test,write or read may be wrong .");
                }
                else
                {
                    strcpy(str,"type get wrong ...");
                }
            }
            else
            {
                if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_UDISK)
                {
                    strcpy(str,"Use left soft key to run udisk card press test,and use right soft key to stop .");
                }
                else if( s_mem_eng_type == MEM_PRESS_TYPE_FOR_SD)
                {
                    strcpy(str,"Use left soft key to run sd card press test,and use right soft key to stop .");
                }
                else
                {
                    strcpy(str,"type get wrong ...");
                }
            }

            len = strlen(str);
            text.wstr_ptr = wstr;
            text.wstr_len = len;
            MMI_STRNTOWSTR( wstr, 200, (uint8*)str, len, len );
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr, text.wstr_len, TRUE);
            GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_SIM_PRESS_RUN, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_SIM_PRESS_STOP, TRUE);
        }
        break;
        default:
            result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

LOCAL MMI_RESULT_E EngMemPressTestEnterWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_COMMON_CTRL1_ID;
    uint16       cur_selection = 0;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;

        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        {
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            if(0 == cur_selection)
            {
                // udisk
                s_mem_eng_type = MEM_PRESS_TYPE_FOR_UDISK;
                MMK_CreateWin((uint32*)MMIENG_MEM_PRESS_TEST_WIN_TAB,   PNULL);
            }
            else if(1 == cur_selection)
            {
                //sd
                s_mem_eng_type = MEM_PRESS_TYPE_FOR_SD;
                MMK_CreateWin((uint32*)MMIENG_MEM_PRESS_TEST_WIN_TAB,   PNULL);
            }
            else
            {
                TRACE_APP_ENGINEERMODE("get index failed, cur_selection = %d",cur_selection);
            }
        }
        break;

        case MSG_CTL_CANCEL:
        case MSG_KEYDOWN_RED:
        {
            MMK_CloseWin( win_id );
        }
        break;

        case MSG_FULL_PAINT:
        {
            GUILIST_ITEM_T  item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            uint16 curIndex = GUILIST_GetCurItemIndex(ctrl_id);

            GUILIST_SetMaxItem(ctrl_id,ENG_MEM_PRESS_LIST_NUM, FALSE );//max item 2 open and close
            GUILIST_SetOwnSofterKey( ctrl_id, FALSE );

            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
            item_t.item_data_ptr = &item_data;

            item_data.item_content[0].item_data_type  = GUIITEM_DATA_TEXT_ID;

            item_data.item_content[0].item_data.text_id = TXT_ENG_UDISK_PRESS_INFO;
            GUILIST_AppendItem( ctrl_id, &item_t );
            //item_data.item_content[0].item_data.text_id = TXT_ENG_SD_PRESS_INFO;
            //GUILIST_AppendItem( ctrl_id, &item_t );

            //set selected item
            GUILIST_SetSelectedItem(ctrl_id, curIndex, TRUE);

            //set current item
            GUILIST_SetCurItemIndex(ctrl_id, curIndex);
        }
        break;
        case MSG_CLOSE_WINDOW:
        {
            s_mem_eng_type = MEM_PRESS_TYPE_MAX;
        }
        break;

        default:
            result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
//================================================================================================//
//================================     ENG MEM PRESS TEST END      ===============================//
//================================================================================================//

#ifdef MP3_SUPPORT
/********************************************************************************/
// NAME:          EngMp3PlayWinHandleMsg
// DESCRIPTION:
// PARAM IN:
// PARAM OUT:
// AUTHOR:        chunjuan.liang
// DATE:          2004.09.08
/********************************************************************************/
LOCAL MMI_RESULT_E EngMp3PlayWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN      play_ret = FALSE;
    const wchar mp3_file_name_phone[] = {'D', ':','\\', 'A', 'u', 'd', 'i', 'o', '\\', 't', 'e', 's', 't', '.', 'm', 'p', '3', 0};
    const wchar mp3_file_name_sd[] = {'E', ':','\\', 'A', 'u', 'd', 'i',  'o', '\\', 't', 'e', 's', 't', '.', 'm', 'p', '3', 0};
    uint8       mp3_vol = MMIAPISET_GetCallRingVolume();

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUILABEL_SetFont(MMIENG_LABEL1_CTRL_ID,SONG_FONT_22,MMI_BLACK_COLOR);

            if (0 == mp3_vol)
            {
                mp3_vol = 3;
            }
            if(MMIAPIFMM_IsFileExist(mp3_file_name_phone, MMIAPICOM_Wstrlen(mp3_file_name_phone)))
            {
                text.wstr_ptr = mp3_file_name_phone;
                play_ret = MMIAPIFMM_PlayMusicFileEx(MMISET_RING_TYPE_OTHER,FALSE,FALSE,
                                           mp3_file_name_phone,MMIAPICOM_Wstrlen(mp3_file_name_phone),
                                           0xffff,0,mp3_vol,PNULL);
                text.wstr_ptr = mp3_file_name_phone;
                text.wstr_len = MMIAPICOM_Wstrlen(mp3_file_name_phone);
                SCI_TRACE_LOW("EngMp3PlayWinHandleMsg PLAY phone mp3 file ret = %d",play_ret);
            }else if(MMIAPIFMM_IsFileExist(mp3_file_name_sd, MMIAPICOM_Wstrlen(mp3_file_name_sd)))
            {
                text.wstr_ptr = mp3_file_name_sd;
                text.wstr_len = MMIAPICOM_Wstrlen(mp3_file_name_phone);
                play_ret = MMIAPIFMM_PlayMusicFileEx(MMISET_RING_TYPE_OTHER,FALSE,FALSE,
                                               mp3_file_name_sd,MMIAPICOM_Wstrlen(mp3_file_name_sd),
                               0xffff,0,mp3_vol,PNULL);
                SCI_TRACE_LOW("EngMp3PlayWinHandleMsg PLAY sd mp3 file ret = %d",play_ret);
            }
            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);
            break;

        case MSG_FULL_PAINT:
            {
                GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
                GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,
                    IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
            }
            break;
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
            MMK_CloseWin(win_id);
            break;
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif
        }
            break;
#endif
        case MSG_KEYDOWN_UPSIDE:
            // adjust play volume
            if (mp3_vol < MMISET_VOL_MAX)
            {
                mp3_vol++;
            }
            break;
        case MSG_KEYDOWN_DOWNSIDE:
            // adjust play volume
            if (mp3_vol > MMISET_VOL_ONE)
            {
                mp3_vol--;
            }
            break;
        case MSG_KEYDOWN_RED:
            break;
        case MSG_LOSE_FOCUS:
        case MSG_CLOSE_WINDOW:
            // stop play
            MMIAPISET_StopAppRing();
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return (result);
}
PUBLIC void MMIEng_OpenPlayMp3Win(void)
{
    MMK_CreateWin((uint32)MMIENG_MP3PLAY_TEST_TAB,PNULL);
}
#endif
/*Edit by script, ignore 3 case. Thu Apr 26 19:00:53 2012*/ //IGNORE9527


/*Edit by script, ignore 88 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527

