/*****************************************************************************
** File Name:      mmiphone.c                                                *
** Author:                                                                   *
** Date:           12/2003                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe phone module                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2003       Jassmine       Create
** 03/2004       Jassmine.Meng     Modify
******************************************************************************/

#define _MMIPHONE_C_



/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_phone_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_signal_ext.h"
#include "mmiphone_export.h"
#include "mmiphone_internal.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "tb_dal.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmisms_text.h"
#include "mmiidle_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_nv.h"
#include "variant_config_nv.h"
#include "mmidisplay_data.h"
#include "mmicl_export.h"
#include "mmi_default.h"
#include "mmialarm_export.h"
#include "mmiset_export.h"
#include "mmiset_call_export.h"
#include "mmiacc.h"
#include "guilcd.h"
//#include "guilabel.h"
#include "guilistbox.h"
#include "guianim.h"
#include "dal_chr.h"
#include "mmiudisk_export.h"
#include "mmi_appmsg.h"
#include "mmimms_export.h"
#include "mmimp3_export.h"
#include "mmiacc.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmi_imagefun.h"
#if defined(MMI_MSD_SUPPORT)
#include "mmimsd_export.h"
#endif
#include "mmi_textfun.h"
#include "mmicc_export.h"
#include "mmicc_id.h"
#include "mn_api.h"
#include "mmiidle_export.h"
#include "sig_code.h"
#include "dal_power.h"
#include "mmi_wallpaper_export.h"
#include "mmiengtd_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef WIN32
#include "mmiudisk_simu.h"
#else
#include "usbservice_api.h"
#include "gpio_prod_api.h"
#endif
#include "sci_api.h"
#include "mmipub.h"
#include "mmiidle_subwintab.h"
#include "mmisd_export.h"
#ifdef AOC_SUPPORT
#include "mmicl_costs_wintab.h"
#endif
#include "mmifmm_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif

#ifdef TOUCH_PANEL_SUPPORT
#include "tp_export.h"
#endif
//#include "mmieng.h"
#include "mmieng_export.h"
#ifdef ENG_SUPPORT
#include "mmieng_nv.h"
#endif
#include "mn.h"
#include "mmifm_export.h"
#include "mmivcard_export.h"
#include "mmiset_nv.h"
#include "mn_type.h"
//macro replace
//#ifdef MMI_STREAMING_ENABLE 
#ifdef STREAMING_SUPPORT_NEXTREAMING 
#include "mmist_id.h"
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#include "mmiset_multisim_export.h"
#endif
#include "mn_api.h"

#include "mmipb_export.h"
#include "mmidc_export.h"
//#include "mmicountedtime_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmisms_export.h"
#if defined(MMI_SMSCB_SUPPORT)
#include "mmisms_export.h"
#endif
#include "guiedit.h"
#include "guires.h"
#include "mmipicview_export.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#ifndef WIN32
#ifndef MMI_DUALMODE_ENABLE
#include "atc_ppp.h"/*lint -e766*/
#endif
#endif
#include "mmistk_export.h"
//#include "brw_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef MMI_ENABLE_DCD
#include "mmidcd_export.h"
#endif
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
#include "mmimtv_export.h"

#ifdef MBBMS_SUPPORT
#include "mmimbbms_wintab.h"
#endif

#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif

#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#ifdef TTS_SUPPORT
#ifndef WIN32
//#include"mmiebook.h"
#include "mmiebook_export.h"
#include "mmitts_export.h"
#include "mmiebook_id.h"
#endif
#endif
#ifdef HERO_ENGINE_TTS_SUPPORT 
#ifndef WIN32
#include "mmitts_export.h"
#endif
#endif

#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif

#ifdef QQ_SUPPORT
#include "mmiqq_export.h"
#endif
#ifdef ENGTD_SUPPORT
#include "mmieng_dynamic_main.h"
#endif
#if defined(MMI_UPDATE_TIME_SUPPORT)
#include "mmiaut_export.h"
#endif /*MMI_UPDATE_TIME_SUPPORT*/
#include "mmiphone_nitz.h"
 
#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif

#include "mmiudisk_export.h"

#include "in_message.h"
#ifdef DRM_SUPPORT
#include "mmidrm.h"
#endif

#ifdef IKEYBACKUP_SUPPORT
#include "mmiikeybackup_api.h"
#endif

#ifdef MMI_VCALENDAR_SUPPORT
#include "mmialarm_id.h"
#include "mmivcalendar.h"
#include "mmischedule_export.h"
#endif

#ifdef MCARE_V31_SUPPORT
#include "mcare_interface.h"
#endif

#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif

#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif

#ifdef MMIWNS_SUPPORT
#include "mmiwns_export.h"
#endif

//#ifdef MMI_DUAL_BATTERY_SUPPORT
#include "dualbat_drvapi.h"
//#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif
#include "mmicalendar_export.h"
#include "mmiphone_charge.h"
#include "mmiidle_statusbar.h"
#include "mmicom_statusarea.h"
#ifdef BROWSER_SUPPORT_DORADO
#include "mmibrowser_dorado_api.h"
#endif
#ifdef MMI_CSTAR_UDB_SUPPORT
#include "mmiim.h"
#endif
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
#include "mmi_autotest.h"
#include "mmicom_banner.h"
#ifdef MMI_SMART_DUAL_SIM_SUPPORT
#include "smartdualsim_export.h"
#endif
#include "mmisrv_cameraroll_export.h"
#include "simat_proactive.h"
#include "nvitem.h"

#include "tasks_id.h"
#include "mmi_appmsg.h"
#ifdef MMI_ETWS_SUPPORT
#include "mmismscb_export.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#ifdef MMI_ONLY_4G_DEL_2G_CUSTOM
#define RAT_ONLY_4G (4)
extern void Set2G4GMode(uint32 dual_sys, uint8 mode);
#endif

//for inline edit
MMI_APPLICATION_T g_ctrl_inlineedit;                // SMS的实体


 
 
 //for inlineedit

typedef struct _IEDT_HANDLE_SYNC {
    MMI_HANDLE_T handle;
	BOOLEAN  is_sync;
	uint16 msg_id;
	BOOLEAN result;
} IEDT_HANDLE_SYNC;

typedef struct _IEDT_SETBG_SIGNAL {
		SIGNAL_VARS
    MMI_HANDLE_T handle;
    GUI_BG_T	    bg;			
} IEDT_SETBG_SIGNAL;


typedef struct _IEDT_SETRECT_SIGNAL {
		SIGNAL_VARS
    MMI_HANDLE_T handle;
    GUI_RECT_T	    rect;			
} IEDT_SETRECT_SIGNAL;


typedef struct _IEDT_SETDISPLAYRECT_SIGNAL {
		SIGNAL_VARS
    MMI_HANDLE_T handle;
    GUI_RECT_T	    rect;
	BOOLEAN is_update;
} IEDT_SETDISPLAYRECT_SIGNAL;

typedef struct _IEDT_SETLEN_SIGNAL {
		SIGNAL_VARS
    MMI_HANDLE_T handle;
    uint16	    len;			
} IEDT_SETLEN_SIGNAL;


 typedef struct _IEDT_SETBORDER_SIGNAL {
		SIGNAL_VARS
    MMI_HANDLE_T handle;
    GUI_BORDER_T	    border;			
} IEDT_SETBORDER_SIGNAL;  


 typedef struct _IEDT_SETFOCUS_SIGNAL {
		SIGNAL_VARS
    MMI_HANDLE_T handle;
    MMI_HANDLE_T  win_handle;
	BOOLEAN is_focus;
    BOOLEAN is_need_paint;
} IEDT_SETFOCUS_SIGNAL;  

typedef struct _IEDT_SETFONT_SIGNAL {
		SIGNAL_VARS
    MMI_CTRL_ID_T ctrl_id;
    GUI_FONT_T  font;
} IEDT_SETFONT_SIGNAL;  


typedef struct _IEDT_SETFONTCOLOR_SIGNAL {
		SIGNAL_VARS
    MMI_CTRL_ID_T ctrl_id;
    GUI_COLOR_T  fontcolor;
} IEDT_SETFONTCOLOR_SIGNAL;  

typedef struct _IEDT_SETIM_SIGNAL {
		SIGNAL_VARS
   MMI_CTRL_ID_T ctrl_id;
   GUIIM_TYPE_T      allow_im;
   GUIIM_TYPE_T      init_im;
} IEDT_SETIM_SIGNAL; 

typedef struct _IEDT_SET_SIGNAL {
		SIGNAL_VARS
   MMI_CTRL_ID_T ctrl_id;
} IEDT_SET_SIGNAL; 


typedef struct _IEDT_SETVISIBLE_SIGNAL {
		SIGNAL_VARS
   MMI_CTRL_ID_T ctrl_id;
	BOOLEAN is_visible;
    BOOLEAN is_update;
} IEDT_SETVISIBLE_SIGNAL; 
 

typedef struct _IEDT_SETSTYLE_SIGNAL {
		SIGNAL_VARS
    MMI_CTRL_ID_T ctrl_id;
	GUIEDIT_STYLE_E style;
} IEDT_SETSTYLE_SIGNAL; 


typedef struct _IEDT_SETSTRING_SIGNAL {
		SIGNAL_VARS
    MMI_CTRL_ID_T ctrl_id;
	wchar* wstr_ptr;
	uint16 wstr_len;
} IEDT_SETSTRING_SIGNAL; 


typedef struct _IEDT_SETHANDLEREDKEY_SIGNAL {
		SIGNAL_VARS
    MMI_CTRL_ID_T ctrl_id;
	BOOLEAN is_handle;
} IEDT_SETHANDLEREDKEY_SIGNAL; 


typedef struct _IEDT_SETPSWSTYLE_SIGNAL {
		SIGNAL_VARS
    MMI_CTRL_ID_T ctrl_id;
    GUIEDIT_PASSWORD_STYLE_E style;
} IEDT_SETPSWSTYLE_SIGNAL; 


typedef struct _IEDT_SETDEFSTR_SIGNAL {
		SIGNAL_VARS
    MMI_CTRL_ID_T ctrl_id;
    wchar *wstr_ptr;
    uint16 wstr_len;
} IEDT_SETDEFSTR_SIGNAL; 


typedef struct _IEDT_CONFIGDISCHAR_SIGNAL {
		SIGNAL_VARS
    MMI_CTRL_ID_T ctrl_id;
    wchar *value_ptr;
    uint16 value_count;
} IEDT_CONFIGDISCHAR_SIGNAL; 



typedef struct _IEDT_SETENABLE_SIGNAL {
		SIGNAL_VARS
    MMI_HANDLE_T ctrl_handle;
   BOOLEAN is_enable;
} IEDT_SETENABLE_SIGNAL; 


typedef struct _IEDT_SETDISPLEFTNUM_SIGNAL {
		SIGNAL_VARS
   MMI_CTRL_ID_T     ctrl_id;
   BOOLEAN is_disp_num_info;
} IEDT_SETDISPLEFTNUM_SIGNAL; 

typedef struct _IEDT_SETALIGN_SIGNAL {
		SIGNAL_VARS
   MMI_CTRL_ID_T     ctrl_id;
   GUI_ALIGN_E    align;
} IEDT_SETALIGN_SIGNAL; 


typedef struct _IEDT_SETMARGIN_SIGNAL {
		SIGNAL_VARS
   MMI_CTRL_ID_T     ctrl_id;
   uint16            margin_lr;
   uint16            margin_tb;
} IEDT_SETMARGIN_SIGNAL; 



typedef struct _IEDT_SETCURSORHIDE_SIGNAL {
		SIGNAL_VARS
   MMI_CTRL_ID_T     ctrl_id;
	BOOLEAN          is_hide;
} IEDT_SETCURSORHIDE_SIGNAL; 

typedef struct _IEDT_PERMITBORDER_SIGNAL {
		SIGNAL_VARS
   MMI_CTRL_ID_T     ctrl_id;
	BOOLEAN          is_permit;
} IEDT_PERMITBORDER_SIGNAL; 

typedef struct _IEDT_SETBGTRANSPARENT_SIGNAL {
		SIGNAL_VARS
   MMI_CTRL_ID_T     ctrl_id;
	BOOLEAN          is_transparent;
} IEDT_SETBGTRANSPARENT_SIGNAL; 


typedef struct _IEDT_SETDIVIDINGLINE_SIGNAL {
		SIGNAL_VARS
   MMI_CTRL_ID_T     ctrl_id;
   uint16          line_width;
   GUI_COLOR_T     line_color;
} IEDT_SETDIVIDINGLINE_SIGNAL; 


typedef struct _IEDT_SETTHEMELINE_SIGNAL {
		SIGNAL_VARS
   MMI_CTRL_ID_T     ctrl_id;
   GUI_COLOR_T          line_focused_color;
   GUI_COLOR_T     line_unfocused_color;
} IEDT_SETTHEMELINE_SIGNAL; 

typedef struct _IEDT_CLEARSTR_SIGNAL {
	SIGNAL_VARS
	MMI_CTRL_ID_T     ctrl_id;
} IEDT_CLEARSTR_SIGNAL; 

#define MAX_NETWORK_NUM (ARR_SIZE(s_network_nametable))

#define MAX_NETWORK_MNC_DIGIT_NUM_3_NUM (ARR_SIZE(s_network_mnc_digit_num_3_nametable))//mnc_digit_num=3

#define MAX_MCC_MNC_DIGIT_DISPLAY 20

#ifdef MMI_SIM_NODIGITNUM_SUPPORT
#define MCC_MNC_DIGIT_INVALID 0xFF    
#endif

#ifdef ENG_SUPPORT
#define MMIENG_STANDBY_TIME_COUNT 1000
#endif
LOCAL BOOLEAN s_phone_is_power_off_req[MMI_DUAL_SYS_MAX];

#ifdef MMI_ETWS_SUPPORT
LOCAL uint16 s_cur_mcc = 0XFFFF;
#endif

// sanity check for macros MMI_SIM_LOCK_SUPPORT and MMI_SIM_LOCK_TYPE2_SUPPORT
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && defined(MMI_SIM_LOCK_SUPPORT)
#error "marco MMI_SIM_LOCK_SUPPORT and MMI_SIM_LOCK_TYPE2_SUPPORT must be enabled mutually exclusive, check your .mk"
#endif

#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
////MMI_SIM_LOCK_TYPE2_SUPPORT 锁卡功能(双卡):
////注意只能选择如下的一种形态或者自己添加锁卡形态!!!!!!!!!!!!!!
//#define DUAL_SIMLOCK_A
////1、双卡中任意一张卡满足条件，都可以正常使用！
//#define DUAL_SIMLOCK_B
////1、如果卡1插入正确的卡，不管卡2正确与否，则卡1卡2都可以正常初始化 
////2、如果卡1不插卡，卡2插入正确的卡，那么卡2可以正常使用 
////3、其他情况下，两张卡(存在的情况下)都锁定。 
#define DUAL_SIMLOCK_C
////1、如果卡1插入正确的卡，则可以正常使用，否则(存在的情况下)则锁定。
////2、如果卡2插入正确的卡，则可以正常使用，否则(存在的情况下)则锁定。
//#define DUAL_SIMLOCK_D
////1、卡1正确时，启动卡1卡2；其他情况(存在的情况下)锁定
//#define DUAL_SIMLOCK_E
////1、卡1(存在的情况下)不合法时锁定卡1，合法时不锁定；卡2任何情况下都不锁定

//BASIC : change the sim power on sequence, original is asynchrous but for this feature to be synchrous.
//defined the valid sim MCC/MNC by the below table
LOCAL const PHONE_SIM_ID_T sim_lock_table[] =
{
	//460, 0, 02,	//CMCC for test in china
	460, 1, 02,	//CUCC for test in china
	//460, 2, 02,	//CMCC for test in china
	460, 3, 02,	//CTCC for test in china
	//460, 5, 02,	//CTCC for test in china
	//460, 6, 02,	//CUCC for test in china
	//460, 7, 02,	//CMCC for test in china
	466, 92, 02,	//CHT mobile
	//466, 05, 02,	//GT mobile
};

#define MMI_MAX_SIM_LOCK_NUM  (ARR_SIZE(sim_lock_table)) 
#endif // defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef struct
{
    BOOLEAN need_handle_pin[MMI_DUAL_SYS_MAX];		//need to handle pin
    BOOLEAN is_handling_pin[MMI_DUAL_SYS_MAX];		//is handling pin
    MN_DUAL_SYS_E   cur_handling_sim_id;					//current handling sim id
    APP_MN_SIM_GET_PIN_IND_T sim_pin_ind[MMI_DUAL_SYS_MAX];	        //save SIM_PIN_IND
}MMIPHONE_HANDLE_PIN_STATUS_T;                                  //add for dual sim


LOCAL const MMI_TEXT_ID_T s_sim_status_string[SIM_STATUS_MAX + 1] = 
{
    TXT_NULL,                               //SIM_STATUS_OK,
    TXT_SIM_REJECTED,               //SIM_STATUS_REJECTED,
#ifndef MMI_IDLE_MINI_SUPPORT
    /*TXT_SIM_REGISTRATION_FAILED*/STR_SIM_REGISTER_FAIL,//SIM_STATUS_REGISTRATION_FAILED,
#else
    TXT_SIM_REGISTRATION_FAILED_MINI,//SIM_STATUS_REGISTRATION_FAILED,
#endif

    STR_PIN_BLOCKED_EXT01,                //SIM_STATUS_PIN_BLOCKED
    TXT_NOT_SUPPORT_SIM,            //SIM_STATUS_NOT_SUPPORT_SIM, //@zhaohui,cr109170
    TXT_PUK_BLOCKED,                //SIM_STATUS_PUK_BLOCKED,
    TXT_PUK_BLOCKED,                //SIM_STATUS_PUK2_BLOCKED,
#ifndef MMI_IDLE_MINI_SUPPORT
    //[for not insert sim card error]
    /*TXT_INSERT_SIM*/STXT_EMERGENCY_ONLY, //SIM_STATUS_NO_SIM,
    //[end]
#else
    TXT_INSERT_SIM_MINI,            //SIM_STATUS_NO_SIM,
#endif
    STR_SIM_SEARCHING_NETWORK, //SIM_NOT_INITED
#if defined(MMI_SIM_LOCK_SUPPORT)
    TXT_SIM_LOCKED,                                //SIM_LOCKED
#endif
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    TXT_SIM_LOCKED_INVALID,  //sim locked,Invalid SIM
#endif
    TXT_NULL                                //SIM_STATUS_MAX
};



LOCAL const MMI_NETWORK_ACI_INFO_T s_network_acitable[] = 
{
    001, "001",
    460, "CHN",//China
    466, "TPE",//Chinese Taipei
    886, "TPE",//Chinese Taipei
    455, "MAC",//Macao China
    454, "HKG",//Hongkong China
//    188, "188",
    202, "GRE",///Greece
    204, "NED",//Netherlands
    206, "BEL",//Belgium
    208, "FRA",//France
    213, "AND",//Andorra
    214, "ESP",//Spain
    216, "HUN",//Hungary
    218, "BIH",//Bosnia
    219, "CRO",//Croatia
    220, "SCG",//Serbia
    222, "ITA",//Italy
    226, "ROM",//Romania
    228, "SUI",//Switzerland
    230, "CZE",//Czech Republic
    231, "SVK",//Slovakia
    232, "AUT",//Austria
//    234, "234",//Guernsey
    238, "DEN",//Denmark
    240, "SWE",//Sweden
    242, "NOR",//Norway
    244, "FIN",//Finland
    246, "LTU",//Lithuania
    247, "LAT",//Latvia
    248, "EST",//Estonia
    250, "RUS",//Russian
    255, "UKR",//Ukraine
    257, "BLR",//Belarus
    259, "MDA",//Moldova
    260, "POL",//Poland
    262, "GER",//Germany
    266, "GIB",//Gibraltar
    268, "POR",//Portugal
    270, "LUX",//Luxembourg
    272, "IRL",//Ireland
    274, "ISL",//Iceland
    276, "ALB",//Albania
    278, "MLT",//Malta
    280, "CYP",//Cyprus
    282, "GEO",//Georgia
    283, "ARM",//Armenia
    284, "BUL",//Bulgaria
    286, "TUR",//Turkey
//    288, "288",//Faroe Islands
//    290, "290",//Greenland
//    293, "293",//Kosovo
    294, "MKD",//Republic of Macedonia
    295, "LIE",//Liechtenstein
    302, "CAN",//Canada
    310, "USA",//USA
    334, "MEX",//Mexico
    338, "ANT",//Antigua and Barbuda
//    340, "340",//Guadeloupe
    344, "ANT",//Antigua and Barbuda
//    350, "350",//Bermuda
//    362, "362",//Netherlands Antilles
    368, "CUB",//Cuba
    370, "DOM",//Dominican Republic
    400, "AZE",//Azerbaijan
    401, "KAZ",//Kazakhstan
    404, "IND",//India
    405, "IND",//India
    410, "PAK",//Pakistan
    412, "AFG",//Afghanistan
    413, "SRI",//Sri Lanka
    414, "MYA",//Myanmar
    415, "LIB",//Lebanon
    416, "JOR",//Jordan
    417, "SYR",//Syria
    419, "KUW",//Kuwait
    420, "KSA",//Saudi Arabia
    421, "YEM",//Yemen
    422, "OMA",//Oman
    424, "UAE",//United Arab Emirates
    425, "ISR",//Israel
    426, "BRN",//Bahrain
    427, "QAT",//Qatar
    428, "MGL",//Mongolia
    429, "NEP",//Nepal
    432, "IRI",//Iran
    434, "UZB",//Uzbekistan
    437, "KGZ",//Kyrgyzstan
    438, "TKM",//Turkmenistan
    440, "JAP",//JAPAN,NIPPON
    450, "KOR",//South Korea
    452, "VIE",//Vietnam
    456, "CAM",//Cambodia
    457, "LAO",//Laos
    470, "BAN",//Bangladesh
    472, "MDV",//Maldives
    502, "MAS",//Malaysia
    505, "AUS",//Australia
    510, "INA",//Indonesia
    515, "PHI",//Philippines
    520, "THA",//Thailand
    525, "SIN",//Singapore
    528, "BRU",//Brunei
    530, "NZL",//New Zealand
    539, "TGA",//Tonga
    541, "VAN",//Vanuatu
    542, "FIJ",//Fiji
//    544, "544",
//    546, "546",//New Caledonia
//    547, "547",//French Polynesia
    550, "FSM",//Federated States of Micronesia
    602, "EGY",//Egypt
    603, "ALG",//Algeria
    604, "MAR",//Morocco
    605, "TUN",//Tunisia
    607, "GAM",//Gambia
    608, "SEN",//Senegal
    610, "MLI",//Mali
    611, "GUI",//Guinea
    612, "COT",//Ivory Coast,Cote D'lvoire
    613, "BUR",//Burkina Faso
    614, "NIG",//Niger
    615, "TOG",//Togo
    616, "BEN",//Benin
    617, "MRI",//Mauritius
    618, "LBR",//Liberia
    620, "GHA",//Ghana
    621, "NGR",//Nigeria
    622, "CHA",//Chad
    624, "CMR",//Cameroon
    625, "CPV",//Cape Verde
    628, "GAB",//Gabon
    629, "CGO",//Republic of the Congo
    630, "COD",//Democratic Republic of the Congo
    631, "ANG",//Angola
    633, "SEY",//Seychelles
    634, "SUD",//Sudan
    635, "RWA",//Rwanda
    636, "ETH",//Ethiopia
    637, "SOM",//Somalia
    639, "KEN",//Kenya
    640, "TAN",//Tanzania
    641, "UGA",//Uganda
    642, "BDI",//Burundi
    643, "MOZ",//Mozambique
    645, "ZAM",//Zambia
    646, "MAD",//Madagascar
    647, "647",//Réunion
    648, "648",//Zimbabwe
    649, "NAM",//Namibia
    650, "MAW",//Malawi
    651, "LES",//Lesotho
    652, "BOT",//Botswana
    653, "SWZ",//Swaziland
    655, "RSA",//South Africa
    704, "GUA",//Guatemala
    706, "ESA",//El Salvador
    708, "HON",//Honduras
    710, "NIC",//Nicaragua
    714, "PAN",//Panama
    716, "PER",//Peru
    722, "ARG",//Argentina
    724, "BRA",//Brazil
    730, "CHI",//Chile
    734, "VEN",//Venezuela
    736, "BOL",//Bolivia
    740, "ECU",//Ecuador
    744, "PAR",//Paraguay
    746, "SUR",//Suriname
};

LOCAL const MMI_NETWORK_NAME_T s_network_nametable[] =
{
001,01,TXT_NET_TEST,PLMN_TEST,

#include "plmn_ts25_database_table.h"
#include "plmn_extra_database_table.h"

};

LOCAL const MMI_NETWORK_NAME_T s_network_mnc_digit_num_3_nametable[] =//mnc_digit_num=3
{
    226, 5, TXT_NET_DIGI,  PLMN_226_05,  //UMTS2100
    310, 30, TXT_NET_INTERNET, PLMN_310_30,
    334, 20, TXT_NET_TELCELMMS, PLMN_334_20,
#if 0
310,20,TXT_NET_203,PLMN_310_020,
310,30,TXT_NET_204,PLMN_310_030,
310,32,TXT_NET_205,PLMN_310_032,
//310,50,TXT_NET_237,PLMN_310_50,
//334,3,TXT_NET_280,PLMN_334_03,
334,20,TXT_NET_277,PLMN_334_020,
//334,50,TXT_NET_284,PLMN_334_50,
//334,90,TXT_NET_283,PLMN_334_090,


    405,25,TXT_NET_386,PLMN_405_025,
    405,27,TXT_NET_386,PLMN_405_027,
    405,29,TXT_NET_386,PLMN_405_029,
    405,30,TXT_NET_386,PLMN_405_030,
    405,31,TXT_NET_386,PLMN_405_031,
    405,32,TXT_NET_386,PLMN_405_032,
    405,34,TXT_NET_386,PLMN_405_034,
    405,35,TXT_NET_386,PLMN_405_035,
    405,36,TXT_NET_386,PLMN_405_036,
    405,37,TXT_NET_386,PLMN_405_037,
    405,38,TXT_NET_386,PLMN_405_038,
    405,39,TXT_NET_386,PLMN_405_039,
    405,41,TXT_NET_386,PLMN_405_041,
    405,42,TXT_NET_386,PLMN_405_042,
    405,43,TXT_NET_386,PLMN_405_043,
    405,44,TXT_NET_386,PLMN_405_044,
    405,45,TXT_NET_386,PLMN_405_045,
    405,46,TXT_NET_386,PLMN_405_046,
    405,47,TXT_NET_386,PLMN_405_047,

    310, 26    ,  TXT_NET_T_MOBILE                  ,  PLMN_WWC_7               ,
	310, 90    ,  TXT_NET_AT_T_WIRELESS_1           ,  PLMN_310_90              ,
	334, 02    ,  TXT_NET_TELCEL                    ,  PLMN_334_02              ,
	334, 04    ,  TXT_NET_334_04                    ,  PLMN_334_04              ,
	334, 05    ,  TXT_NET_334_04                    ,  PLMN_334_05              ,
    334, 30    ,  TXT_NET_MOVISTAR                  ,  PLMN_334_30              ,
    338, 70    ,  TXT_NET_706_01                    ,  PLMN_338_070             ,

        410, 34    ,  TXT_NET_UFONE                     ,  PLMN_410_34              ,
	704, 30    ,  TXT_NET_MOVISTAR                  ,  PLMN_704_30              ,
	706, 40    ,  TXT_NET_MOVISTAR                  ,  PLMN_706_40              ,
        708,1,TXT_NET_737,PLMN_708_1,
	708, 20    ,  TXT_NET_744_04                    ,  PLMN_708_20              ,
        714,20,TXT_NET_748,PLMN_714_20,
	740, 10    ,  TXT_NET_CLARO                     ,  PLMN_740_10              ,
#endif
};
typedef enum
{
    MMIPHONE_PS_POWER_ON_NONE,
    MMIPHONE_PS_POWER_ON_EMERGE,
    MMIPHONE_PS_POWER_ON_NORMAL
}MMIPHONE_PS_POWER_ON_TYPE;

#ifdef CSC_XML_SUPPORT
typedef struct
{
    uint16 net_work_num;
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *csc_network_info;
}MMI_CSC_NETWORK_NAME_T;
#endif

#ifdef UNISOC_ROAMING_INDICATION
/*
//当前Roaming indication(MVNO list)表格的信息来源如下:
//http://shexsvn01.spreadtrum.com/svn/Product/Product_PM/Project_微软_SC7701/MS-input/MS-Standards/Roaming相关资料/MVNO_10142015_Update.xlsx
MVNO roaming table
        MVNO                                                Host                            Country
Name                MCC         MNC                 Name                    MCC     MNC
Jazztel             214         21                  Orange                  214     3       Spain
Yoigo               214         4                   Movistar                214     7       Spain
ONO                 214         18                  Movistar                214     7       Spain
Mobil R             214         6                   Vodafone                214     1       Spain
Euskaltel           214         3                   Orange                  214     3       Spain
Telecable           214         6                   vodafone                214     1       Spain
Simyo               214         21                  Orange                  214     3       Spain
BT                  214         6                   Vodafone                214     1       Spain
Pepephone           214         6                   Vodafone                214     1       Spain
Carrefour           214         3                   Orange                  214     3       Spain
RACC                214         10                  Movistar                214     7       Spain
Neo-Sky             214         6                   Vodafone                214     1       Spain
PosteMobile         222         33                  WIND IT                 222     88      Italy
PosteMobile         222         10                  Vodafone IT             222     10      Italy
Fastweb             222         99                  H3G                     222     99      Italy
BIP                 222         99                  H3G                     222     99      Italy
CoopVoce            222         1                   TIM                     222     1       Italy
Yesss!              232         12                  A1 Austria              232     1       Austria
Ge org!             232         12                  A1 Austria              232     1       Austria
BOB                 232         11                  A1 Austria              232     1       Austria
upc ch              228         53                  Orange CH               228     3       Switzerland
Telfort             204         12                   KPN NL                 204     8       Netherland
Tele2               204         2                   Mobile NL               204     16      Netherland
Lycamobile          204         9                    KPN NL                 204     8       Netherland
Intercity           204         17                   KPN NL                 204     8       Netherland
UPC NL              204         18                   Vodafone               204     4       Netherland
Ziggo               204         15                   Vodafone               204     4       Netherland
Telenet BE          206         5                    Mobistar BE 206-10     206     10      Belgium
tele.ring           232         7                   T-Mobile                232     3       Austria
Ben NL              204         16                  T-Mobile NL             204     16      Netherland
HT HR               219         1                   T-Mobile HR             219     1       Croatia
bonbon              219         1                   T-Mobile HR             219     1       Croatia
frog                226         3                   COSMOTE                 226     3       Romania
frog                226         6                   COSMOTE                 226     6       Romania
MTV Mobile          226         3                   COSMOTE                 226     3       Romania
MTV Mobile          226         6                   COSMOTE                 226     6       Romania
heyah               260         2                   T-Mobile.pl             260     2       Poland
Saunalahti          244         21                  Elisa                   244     5       Finland

congstar            262         01                  (DT) Telekom Germany    262     01      Germany
congstar.de         262         01                  (DT) Telekom Germany    262     01      Germany
Virgin              234         30/31/32/33/34      EE                      234     30/31/32/33/34  UK
LIFE                234         33                  EE                      234     30/31/32/33/34  UK
Asda                234         15                  Vodafone                234     15      UK
Lebara              234         15                  Vodafone                234     15      UK
TalkTalk            234         15                  Vodafone                234     15      UK
BT                  234         15                  Vodafone                234     15      UK
Talkmobile          234         15                  Vodafone                234     15      UK
Sainsbury's         234         15                  Vodafone                234     15      UK
OVIVO               234         15                  Vodafone                234     15      UK
Gamma               234         15                  Vodafone                234     15      UK
Tesco               234         10                  O2                      234     10      UK
giffgaff            234         10                  O2                      234     10      UK
TMO (BEN)           204         20                  T-Mobile NL             204     16      NL

Virgin Mobile       505         2                   Optus                   505     2       Australia
Boost               505         1                   Telstra                 505     1       Australia
H3G                 232         5                   T-Mobile A              232     3       Austria										
H3G                 232         10                  T-Mobile A	            232	    3	    Austria										
SMART               515         03                  SUN                     515     05      Philippine										
SUN	                515	        05	                SMART	                515	    03	    Philippine										
T-Mobile A	        232	        3	                3 AT	                232	    5	    Austria
T-Mobile A	        232	        3	                3 AT	                232	    10	    Austria
3 AT	            232	        5	                3 AT	                232	    10	    Austria
3 AT	            232	        10	                3 AT	                232	    5	    Austria

EI Telecom 	        208	        26	                SFR	                    208	    10	    France	
EI Telecom 	        208	        26	                OrangeF	                208	    1	    France	
EI Telecom 	        208	        26	                Bouygues	            208	    20	    France	
Virgin	            208	        23	                SFR	                    208	    10	    France	
Tele2	            232	        19	                3 AT	                232	    5	    Austria	
Tele2	            232	        19	                T-Mobile A	            232	    3	    Austria	
Tele2	            232	        19	                A1	                    232	    1	    Austria	
Tele2	            232	        19	                telering	            232	    7	    Austria	
Tele2	            232	        19	                3AT	                    232	    10	    Austria	
U Mobile	        502	        18	                MY MAXIS	            502	    12	    Malaysia	
Virgin mobile	    420	        5	                STC	                    420	    1	    KSA	
FRiENDi mobile	    420	        5	                STC	                    420	    1	    KSA	
Telenor DK	        238	        77	                Telia-Telenor DK	    238	    66	    Denmark	
Telenor DK	        238	        2	                Telia-Telenor DK	    238	    66	    Denmark	
Telia-Telenor DK	238	        66	                Telenor DK	            238	    77	    Denmark	
Telia-Telenor DK	238	        66	                Telenor DK	            238	    2	    Denmark	
Telia DK	        238	        20	                Telia-Telenor DK	    238	    66	    Denmark	
Telia-Telenor DK	238	        66	                Telia DK	            238	    20	    Denmark	
Tele2	            240	        7	                Telenor	                240	    24	    Sweden	
Simple	            730	        7	                movistar	            730	    2	    Chile	            SPN: Simple
Virgin Mobile	    730	        7	                movistar	            730	    2	    Chile	            SPN: Virgin
WOM	                730	        9	                movistar	            730	    2   	Chile	            SPN: WOM  (Hybrid MVNO, they have their own 4G/3G network)
WOM	                730	        9	                Claro Chile	            730	    3	    Chile	            SPN: WOM  (Hybrid MVNO, they have their own 4G/3G network)
WOM	                730	        9	                Entel Chile	            730	    1	    Chile	            SPN: WOM  (Hybrid MVNO, they have their own 4G/3G network)
Falabella	        730	        1	                Entel Chile	            730	    1	    Chile	            SPN: Falabella
Virgin Mobile	    716	        6	                movistar Peru	        716	    6	    Peru	            SPN: Virgin
Tuenti	            716	        6	                movistar Peru	        716	    6	    Peru	            SPN: tuenti
Movil ?xito	        732	        111	                Tigo Colombia	        732	    111/103	Colombia        	SPN:Movil Exito
UFF	                732	        103	                Tigo Colombia	        732	    103/111	Colombia	        SPN:Uff!
Virgin Mobile	    732	        123	                Movistar Colombia	    732	    123	    Colombia	        SPN: Virgin Mobile
ETB	                732	        187	                Tigo Colombia	        732	    103/111	Colombia	        SPN: ETB 4G  (Hybrid, they have their own 4G network)
Avantel	            732	        130	                CLARO/TIGO/MOVISTAR	    732	    101/123/111/103	Colombia	SPN: Avantel LTE  (Hybrid, they have their own 4G network)
DNA	                244	        12	                Suomen Yhteisverkko	    244	    36	    Finland	
DNA	                244	        13	                Suomen Yhteisverkko	    244	    36	    Finland	
DNA	                244	        3	                Suomen Yhteisverkko	    244	    36	    Finland	
Telia FI	        244	        91	                Suomen Yhteisverkko	    244	    36	    Finland	
O2	                262	        7/8/11	            E-Plus	                262	    3/5/12/17/20/77	Germany	
E-Plus	            262	        3/5/12/17/20/77	    O2	                    262	    7/8/11	Germany	
Lebara 	            505	        3	                Optus	                505	    2	    Australia	
AIS	                520	        3	                TOT	                    520	    15	    Thailand	

*/
typedef struct
{
    uint16 mvno_mcc;
    uint16 mvno_mnc;
    uint16 host_mcc;
    uint16 host_mnc;
}MMI_MVNO_ROAMING_T;

#define MAX_MVNO_ROAMING_NUM (ARR_SIZE(s_mvno_roaming_table))

LOCAL const MMI_MVNO_ROAMING_T s_mvno_roaming_table[] =
{
    #include "mvno_list.h"
};
#endif

#define  MMI_IMS_TOGGLE_CONFIG_PARAMETER 1
#ifdef MMI_VOLTE_SUPPORT
#ifdef MMI_IMS_TOGGLE_CONFIG_PARAMETER
//#define DATA_IMS_TOGGLE_START_ADDR 0x703F0000
//#define DATA_IMS_TOGGLE_OFFSET 0x10
#define MAX_IMS_TOGGLE_MNC_NUM (ARR_SIZE(s_ims_toggle_mnc_table))
LOCAL const MMI_IMS_TOGGLE_MNC_T s_ims_toggle_mnc_table[] =
{
    #include "ims_toggle_mnc_table.h"
};

#endif
#endif
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL PHONE_SIM_STATUS_T          s_sim_status[MMI_DUAL_SYS_MAX];        //the status of sim

LOCAL BOOLEAN               s_sim_existed_status[MMI_DUAL_SYS_MAX] = {0}; //变量用来记录SIM卡是否存在
LOCAL BOOLEAN s_is_power_flag = FALSE;
LOCAL BOOLEAN s_is_cphs_supported = FALSE; //MMI use CPHS config or not
LOCAL char s_4g_icon_display_style = 0;
LOCAL char s_volte_icon_display_style [MMI_DUAL_SYS_MAX] = {0};
LOCAL char s_volte_icon_display_set[MMI_DUAL_SYS_MAX] = {0};
LOCAL char s_volte_menu_display_set[MMI_DUAL_SYS_MAX] = {0};
LOCAL char s_prefer_network_menu_display_style = 0;
#ifdef MMI_INDIAN_SOS_SUPPORT
LOCAL BOOLEAN s_is_indiasos_supported = FALSE;
#endif
#if defined(MMI_DUALMODE_ENABLE) ||defined (MMI_2G_4G_SUPPORT)
LOCAL MMI_GMMREG_RAT_E  s_is_TD[MMI_DUAL_SYS_MAX];//MN_GMMREG_RAT_GPRS;
#endif
LOCAL MMIPHONE_HANDLE_PIN_STATUS_T s_handle_pin_status;         //add for dual sim
LOCAL BOOLEAN s_is_same_sim[MMI_DUAL_SYS_MAX] = {0};

#ifndef MMI_MULTI_SIM_SYS_SINGLE
LOCAL SIM_QUERY_STATUS_E s_query_sim_status = SIM_QUERY_STATUS_NONE;
#endif

LOCAL BOOLEAN s_is_power_on_sim_cnf[MMI_DUAL_SYS_MAX] = {FALSE};
LOCAL SIM_QUERY_STATUS_E s_query_sim_present_status = SIM_QUERY_STATUS_NONE;
LOCAL MMIPHONE_PS_POWER_ON_TYPE s_ps_ready[MMI_DUAL_SYS_MAX] = {0};/*lint !e64*/
LOCAL BOOLEAN s_is_ps_deactive_cnf[MMI_DUAL_SYS_MAX] = {FALSE};
LOCAL BOOLEAN s_is_ps_ready[MMI_DUAL_SYS_MAX] = {FALSE};
LOCAL BOOLEAN s_is_sim_present_ind[MMI_DUAL_SYS_MAX] = {FALSE};
LOCAL BOOLEAN s_is_network_status_ready[MMI_DUAL_SYS_MAX] = {FALSE};
#if defined(MMI_SMSCB_SUPPORT)
LOCAL BOOLEAN s_is_startup_smscb[MMI_DUAL_SYS_MAX] = {FALSE};
#endif
LOCAL MMIPHONE_NOTIFY_LIST_T *s_mmiphone_subscriber_list_ptr = PNULL;
LOCAL MMIPHONE_SIM_PLUG_NOTIFY_LIST_T *s_mmiphone_sim_plug_subscriber_list_ptr = PNULL;
MN_DUAL_SYS_E s_plug_sim_id = MN_DUAL_SYS_1;
MMIPHONE_SIM_PLUG_EVENT_E s_plug_event = MMIPHONE_NOTIFY_SIM_PLUG_MAX;
LOCAL MMI_OPERATE_PIN_T s_operate_pin_blocked_info;//用于pin被锁后传递参数
LOCAL MN_DUAL_SYS_E s_cur_handling_sim_id = MMI_DUAL_SYS_MAX;
LOCAL MMI_HANDLE_T s_need_update_network_status_win = 0;//网络状态更新时候需要刷新网络信息的窗口
#ifdef MMI_SIM_LOCK_SUPPORT
LOCAL MN_SIM_LOCK_STATUS_T s_sim_unlock_config_data = {0};//记下 SIM lock 解锁用计数器和定时器的配置信息
LOCAL MMI_OPERATE_SIM_LOCK_T s_sim_lock_operate_value = {0}; 
LOCAL MMIPHONE_HANDLE_SIMLOCK_STATUS_T  s_handle_sim_lock_status = {0};//为多卡记住SIM Lock status
#endif

#if defined MMI_RESET_PHONE_AUTO_PIN
LOCAL BOOLEAN s_use_pin_in_nv = FALSE;
LOCAL BOOLEAN s_use_dual_pin_in_nv[MMI_DUAL_SYS_MAX] = {FALSE};
#endif

#ifdef MMI_SIM_LANGUAGE_SUPPORT
// 是否做过优选语言的动作。
LOCAL BOOLEAN s_is_language_auto_set = FALSE; 

//做优选语言的sim卡，系统运行中只选定一张固定的sim卡做操作。
LOCAL MN_DUAL_SYS_E s_prefer_language_sim = MN_DUAL_SYS_1; 
#endif

#ifdef CSC_XML_SUPPORT
LOCAL MMI_CSC_NETWORK_NAME_T *s_csc_network_name_ptr = PNULL;
#endif

#ifdef MMI_LDN_SUPPORT
uint16  s_ldn_flag = 0;
#endif

#ifdef MMI_SIM_NODIGITNUM_SUPPORT
       BOOLEAN s_is_autoadapt_flag[MMI_DUAL_SYS_MAX]= {FALSE};            
#endif
LOCAL  APP_MN_SIM_GET_PIN_IND_T s_sim_pin_information = {0};
LOCAL  uint8   s_puk_remain_status = 0;
LOCAL  BOOLEAN  s_second_pin_query_flag = FALSE;
extern BOOLEAN  s_pin_input_ok_status;

#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
LOCAL BOOLEAN s_is_simlock_off =  FALSE;//SIM LOCK是否开启，解锁后，每次开机都将会置为TRUE
LOCAL BOOLEAN s_is_sim1_right =  FALSE;//检测SIM1是否合法的标志
LOCAL BOOLEAN s_is_sim2_right =  FALSE;//检测SIM2是否合法的标志
LOCAL BOOLEAN s_is_sim1_poweron =  FALSE;//检测SIM1是否被POWER ON(SIM不可以重复的POWER ON 或者POWER OFF)
LOCAL BOOLEAN s_is_sim2_poweron =  FALSE;//检测SIM2是否被POWER ON(SIM不可以重复的POWER ON 或者POWER OFF)
LOCAL BOOLEAN s_is_in_pwd_win =  FALSE;//检测是否为手机密码输入界面
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
//extern MMI_SYSTEM_T g_mmi_system; 
extern uint32   g_operate_pin_win_tab;
extern BOOLEAN g_is_operate_pin_special_func;
extern uint32 g_sim_pin_sys_dual;
extern PUBLIC MMI_RESULT_E HandlePukInputExceptPhone (
                                              MMI_WIN_ID_T   win_id, 	// PUK INPUTING窗口的ID
                                              MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                              DPARAM 		    param		// 相应消息的参数
                                              );

extern BOOLEAN s_first_language_select_flag;

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMI_APPLICATION_T   g_mmiphone_app;         //phone的实体
PHONE_SERVICE_STATUS_T  g_service_status[MMI_DUAL_SYS_MAX];         //the status of service
MN_BLOCK_T g_pin_value[MMI_DUAL_SYS_MAX] ={0};
#ifdef DPHONE_SUPPORT 
BOOLEAN g_pin_lock_hookhandfree = FALSE;
#endif
BOOLEAN g_is_sim_only_check_status[MMI_DUAL_SYS_MAX] = {0};
#ifdef UNISOC_MMI_SIM_LOCK_SUPPORT
MMIPHONE_SIM_LOCK_INFO_T g_sim_lock_info = {0};
#endif
BOOLEAN s_ims_same_sim[MMI_DUAL_SYS_MAX] = {0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : create input pin window
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E CreateInputPinWindow(APP_MN_SIM_GET_PIN_IND_T sim_get_pin_ind);

/*****************************************************************************/
// 	Description : IsNeedSetGPRSAttachMode
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedSetGPRSAttachMode(void);

/*****************************************************************************/
// 	Description : 处理PHONE APP的PS消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePsAndRefMsg(
                                     PWND                app_ptr, 
                                     uint16              msg_id, 
                                     DPARAM              param
                                     );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_NETWORK_STATUS_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetworkStatusInd(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_LIST_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreferRatRsp( 
                                        DPARAM  param
                                        );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_SELECT_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectPlmnCnf(
                                       DPARAM              param
                                       );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_POWER_ON_CNF
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOnSimCnf(
                                       DPARAM              param
                                       );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_POWER_OFF_CNF
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOffSimCnf(
                                       DPARAM              param
                                       );

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PREFER_PLMN_CHANGE_CNF
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreferPlmnChangeCnf(
                                       DPARAM              param
                                       );
#endif

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PS_POWER_OFF_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerPsOffCnf(
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_DEACTIVE_PS_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeactivePsCnf(
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_LIST_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlmnListCnf(
                                     DPARAM              param
                                     );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SET_BAND_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectBandCnf(
                                       DPARAM              param
                                       );

/*****************************************************************************/
// 	Description : get the network name
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetNetWorkNameId(
                                     MN_DUAL_SYS_E dual_sys,
                                     MN_PLMN_T *plmn_ptr,
                                     MN_PHONE_PLMN_STATUS_E  plmn_status,
                                     BOOLEAN                 is_slecting
                                     );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_GET_PIN_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPinInd(
                                   DPARAM              param
                                   );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_PIN_FUNC_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPinCnf(
                                   DPARAM              param
                                   );


/*****************************************************************************/
// Description : 
//  User can check the netwotk's mcc/mnc, and sim's mcc/mnc.
//  If they are not equal, it is roaming.
//  But maybe the local operator regard this as not-roaming.
//  Then the user can config in this function to resolve this case.
// Global resource dependence : 
// Author: Wuding.Yang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsRealRoaming(MN_DUAL_SYS_E dual_sys, MN_PLMN_T cur_plmn);

/*****************************************************************************/
// 	Description : 保存网络状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreNetworkStatus(
                               APP_MN_NETWORK_STATUS_IND_T *status_ptr
                               );

/*****************************************************************************/
// 	Description : 在提示还可以 输入多少次密码后，启动pin输入框
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePin1Maxtimes(
                                      MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM 		        param		// 相应消息的参数
                                      );

/*****************************************************************************/
// 	Description : 在pin blocked的情况下关闭窗口，则启动输入puk的窗口
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinBlocked(
                                    MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                    DPARAM 		        param		// 相应消息的参数
                                    );

/*****************************************************************************/
// 	Description : 在pin1或者pin2 blocked的情况下关闭窗口，则启动输入puk的窗口
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinBlockedExceptPhone(
                                               MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                               MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                               DPARAM 		        param		// 相应消息的参数
                                               );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_READY_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimReadyInd(
                                     DPARAM              param
                                     );


/*****************************************************************************/
// 	Description : deal with signal of APP_MN_READ_SIM_EF_CNF
//	Global resource dependence : 
//    Author: Tong Yongli
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimEFReadInd(
                                     DPARAM              param
                                     );

/*****************************************************************************/


/*****************************************************************************/
// 	Description : 保存 SIM 状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreSimReadyStatus(
                               APP_MN_SIM_READY_IND_T  *sim_status_ptr
                               );

/*****************************************************************************/
// 	Description : 保存 SIM  not ready 状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreSimNotReadyStatus(
                                  APP_MN_SIM_NOT_READY_IND_T  sim_status_ptr
                                  );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_NOT_READY_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimNotReadyInd(
                                        DPARAM              param
                                        );
#ifdef UNISOC_MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : handle the sim lock info
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void HandleSimLockInfo(APP_MN_SIM_NOT_READY_IND_T   sim_not_ready_ind);
#endif
/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SCELL_RSSI_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleScellRssiInd(
                                      DPARAM              param
                                      );

#if defined(MMI_SMSCB_SUPPORT)
/*****************************************************************************/
// 	Description : startup sms cb
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StartupSmsCB(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : ClearCbEnable
//	Global resource dependence : 
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
LOCAL void ClearCbEnable(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
// 	Description : get opn display string
//	Global resource dependence : 
//  Author: wancan.you
//	Note:  
/*****************************************************************************/
LOCAL void GetOpnString(MN_DUAL_SYS_E dual_sys,//IN:
                                            MMI_STRING_T *opn_str_ptr,//IN/OUT
                                            uint16 max_opn_len
                                            );

/*****************************************************************************/
// 	Description : get spn display string
//	Global resource dependence : 
//  Author: wancan.you
//	Note:  
/*****************************************************************************/
LOCAL void GetSpnString(MN_DUAL_SYS_E dual_sys,//IN:
                                            MMI_STRING_T *spn_str_ptr,//IN/OUT
                                            uint16 max_spn_len
                                            );

/*****************************************************************************/
// 	Description : compare spn and opn
//	Global resource dependence : 
//  Author: kun.yu
//	Note:  
/*****************************************************************************/
LOCAL BOOLEAN MMIPHONE_IsSPNEqualtoOPN(MMI_STRING_T *opn_str_ptr,//IN:
                                                                                    MMI_STRING_T *spn_str_ptr//IN:
                                                                                    );

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: wancan.you
//    Note: 
/*****************************************************************************/
//LOCAL void MMIPHONE_SetDualSysSetting(BOOLEAN is_need_start_ps);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPresentInd(
                                     DPARAM param
                                     );

/*****************************************************************************/
// 	Description : PowerOnPsForEmerge
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
LOCAL void PowerOnPsForEmerge(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : the process message function of the virtual window
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMI_RESULT_E  IdleApplet_HandleEvent(    
                                     IAPPLET_T*          iapplet_ptr,
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );

/*****************************************************************************/
//  Description : Notify Rx level Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void NotifyRxLevelChange(MN_DUAL_SYS_E dual_sys, uint8 rx_level);

/*****************************************************************************/
//  Description : Notify PLMN Status Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void NotifyPlmnStatusChange(MN_DUAL_SYS_E dual_sys, MN_PHONE_PLMN_STATUS_E plmn_status);

/*****************************************************************************/
//  Description : Notify Service Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifyServiceChange(MN_DUAL_SYS_E dual_sys, MMIPHONE_NOTIFY_EVENT_E notify_event);

#ifdef MMI_VOLTE_SUPPORT
LOCAL MMI_RESULT_E HandleImsStatusInd(DPARAM param );
LOCAL void MMIAPICOM_StatusAreaSetVolteState(MN_DUAL_SYS_E dual_sys,MMI_GMMREG_RAT_E  rat);
#endif
#ifdef MMI_2G_4G_SUPPORT
LOCAL MMI_RESULT_E HandleRatChangeCnf(DPARAM param );
#endif

//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmiphone_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_IDLE_APPLET_ID), sizeof(CAF_APPLET_T), IdleApplet_HandleEvent, 0, TXT_IDLE },
};

//applet的信息
LOCAL const CAF_STATIC_APPLET_INFO_T s_mmiphone_applet_info = 
{
    (CAF_STATIC_APPLET_T *)s_mmiphone_applet_array,
    ARR_SIZE(s_mmiphone_applet_array)
};

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPlugInInd(
                                     DPARAM param
                                     );
#endif

#ifdef MMI_LDN_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLdnReadyInd(
                                     DPARAM param
                                     );

/*****************************************************************************/
// 	Description : ldn update confirm
//	Global resource dependence : none
//	Author:dave.ruan
//	Note:delete all/datlete both type is erase
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLDNUpdateCnf( DPARAM param);
#endif

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
// 	Description : Handle SIM Plug Alert Win Msg
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMPlugAlertWinMsg(
                                                                                              MMI_WIN_ID_T win_id,
                                                                                              MMI_MESSAGE_ID_E 	msg_id,
                                                                                              DPARAM param
                                                                                              );

/*****************************************************************************/
//  Description : Notify SIM Plug Ind
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifySIMPlugInd(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event);
#endif

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void OpenSetStandByModeWaiting(void);
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void ImplementStandByModeAsync(void);

/*****************************************************************************/
//  Description : HandleSetStandbyMdoeWaitingWindow
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetStandbyMdoeWaitingWindow(
                                         MMI_WIN_ID_T    win_id, // 窗口的ID
                                         MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                         DPARAM             param   // 相应消息的参数
                                         );
/*****************************************************************************/
// 	Description : HandleSetStandByModeQueryWinMsg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetStandByModeQueryWinMsg( 
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E    msg_id, 
                                           DPARAM              param
                                           );
#endif

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : 在SIM Lock 解锁次数达到限制的情况下弹出"SIM Restricted"窗口
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLocked(
                                    MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                    DPARAM 		        param		// 相应消息的参数
                                    );
#endif
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
/*****************************************************************************/
// 	Description : Adjust the sim is Legal or not
//	Global resource dependence : 
//    Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIPHONE_IsLegalSim(APP_MN_SIM_READY_IND_T* sim_ready_ind_ptr);

/*****************************************************************************/
// 	Description : Adjust the running state is Legal or not: TRUE(LOCKED),FALSE(UNLOCKED)
//	Global resource dependence : 
//    Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIPHONE_IsLegalState(void);
#endif


#ifdef SIM_LOCK_CT_SUPPORT
typedef struct 
{
    uint8 sms_num;
    uint8 pin_status;
    uint8 pin_remain;
    uint8 sim_present;
    uint8 sim_ready;
    uint8 power_over;
    uint8 pwon_cnf;
    BOOLEAN is_cdma;
    uint8 need_check;
    uint8 check_over;
} CDMA_SMARTCARD_STATUS_T;

CDMA_SMARTCARD_STATUS_T g_smartcard_status_t[3] = {0};

uint8 MMICDMA_HandleUimPresent(MN_DUAL_SYS_E dual_sys,BOOLEAN is_present,uint8 sms_num)
{
    s_is_sim_present_ind[dual_sys] = TRUE;

    MMIAPISMS_SetMaxSimSmsNum(dual_sys, sms_num);

    MMIAPIPHONE_SetSimExistedStatus(dual_sys, is_present);

    MMIAPIPHONE_SetGPRSAttachMode();

    //stk refresh type = SIMAT_RF_RESET(4) need power on ps.
    if (4 == MMIAPISTK_GetStkRefreshType(dual_sys)) 
    {
        MMIAPISTK_SetStkRefreshType(dual_sys, 0);

        MMIAPIPHONE_PowerOnPs(dual_sys);

        MMIAPISTK_CloseRefreshWaitingWin();
    }

    return 0;
}
uint8 ATC_SendTo_APP_UimPresent(MN_DUAL_SYS_E dual_sys,BOOLEAN is_present,uint8 sms_num)
{
    APP_MN_SIM_PRESENT_IND_T  *signal_ptr = PNULL;
    
    if(dual_sys >= MN_DUAL_SYS_MAX)
    {
        return 1;
    }
    
    // sim ready
    signal_ptr = SCI_ALLOCA(sizeof(APP_MN_SIM_NOT_READY_IND_T));
    if (signal_ptr == PNULL)
    {
        return 2;
    }
    signal_ptr->SignalCode = (uint16)APP_MN_SIM_PRESENT_IND;
    signal_ptr->SignalSize = sizeof(APP_MN_SIM_NOT_READY_IND_T);
    signal_ptr->Sender     = P_ATC;
    signal_ptr->dual_sys   = dual_sys;
    signal_ptr->Pre        = SCI_NULL;
    signal_ptr->Suc        = SCI_NULL;
    
    signal_ptr->is_sim_present = is_present;
    signal_ptr->sms_num = sms_num;

    SCI_SEND_SIGNAL((xSignalHeaderRec*)signal_ptr, P_APP);

    return 0;
}

uint8 ATC_SendTo_APP_UimNotReady(MN_DUAL_SYS_E dual_sys)
{
    APP_MN_SIM_NOT_READY_IND_T  *signal_ptr = PNULL;
    
    if(dual_sys >= MN_DUAL_SYS_MAX)
    {
        return 1;
    }
    
    // sim ready
    signal_ptr = SCI_ALLOCA(sizeof(APP_MN_SIM_NOT_READY_IND_T));
    if (signal_ptr == PNULL)
    {
        return 2;
    }
    signal_ptr->SignalCode = (uint16)APP_MN_SIM_NOT_READY_IND;
    signal_ptr->SignalSize = sizeof(APP_MN_SIM_NOT_READY_IND_T);
    signal_ptr->Sender     = P_ATC;
    signal_ptr->dual_sys   = dual_sys;
    signal_ptr->Pre        = SCI_NULL;
    signal_ptr->Suc        = SCI_NULL;
    
    signal_ptr->reason = MNSIM_ERROR_PIN1_BLOCKED;

    SCI_SEND_SIGNAL((xSignalHeaderRec*)signal_ptr, P_APP);

    return 0;
}

uint8 ATC_SendTo_APP_UimPWOnCfn(MN_DUAL_SYS_E dual_sys,BOOLEAN is_uim_ok)
{
    APP_MN_SIM_POWER_ON_CNF_T *signal_ptr = PNULL;
    
    if(dual_sys >= MN_DUAL_SYS_MAX)
    {
        return 1;
    }
    
    // sim ready
    signal_ptr = SCI_ALLOCA(sizeof(APP_MN_SIM_POWER_ON_CNF_T));
    if (signal_ptr == PNULL)
    {
            SCI_PASSERT(0, ("ATC_SendTo_APP_UimPWOnCfn Alloc  %ld FAIL",sizeof(APP_MN_SIM_POWER_ON_CNF_T)));
            return 2;
    }
    signal_ptr->SignalCode = (uint16)APP_MN_SIM_POWER_ON_CNF;
    signal_ptr->SignalSize = sizeof(APP_MN_SIM_POWER_ON_CNF_T);
    signal_ptr->Sender     = P_ATC;
    signal_ptr->Pre = SCI_NULL;
    signal_ptr->Suc = SCI_NULL;
    signal_ptr->dual_sys   = dual_sys;
    signal_ptr->is_ok = is_uim_ok;
    SCI_SEND_SIGNAL((xSignalHeaderRec*)signal_ptr, P_APP);
    return 0;
}

BOOLEAN  MMICDMA_GetSmartCard_Over(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN res = FALSE;
    if(g_smartcard_status_t[dual_sys].check_over)
    {
        res = TRUE;
    }
    return res;
}

BOOLEAN MMICDMA_GetSmartCard_NeedCheck(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN res = FALSE;
    if(g_smartcard_status_t[dual_sys].need_check)
    {
        res = TRUE;
    }
    return res;
}

BOOLEAN  SmartCard_IsCdma(MN_PLMN_T plmn,MN_DUAL_SYS_E  dual_sys)
{
    BOOLEAN res = FALSE;
#if 	0
    if( (460 == plmn.mcc) &&
        ((03 == plmn.mnc ) || (05 == plmn.mnc ) || (11 == plmn.mnc )))
    {
        res = TRUE;
    }
#endif	
	res = MMIAPICmp_IMSI_Info(dual_sys);

    return res;
}

BOOLEAN MMIPhone_CDMA_SimPowerOver(MN_DUAL_SYS_E dual_sys)
{
    if(FALSE == MMICDMA_GetSmartCard_Over(dual_sys))
    {
        g_smartcard_status_t[dual_sys].need_check = 0;
        g_smartcard_status_t[dual_sys].check_over = 1;
    }
		    
    return FALSE;
}

BOOLEAN  MMICDMA_CheckCard_OK(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN res = FALSE;
    if(g_smartcard_status_t[dual_sys].is_cdma)
    {
        res = TRUE;
    }
    return res;
}

#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register common module applet info
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_CommonMduRegAppletInfo(void)
{
    MMI_RegAppletInfo( MMI_MODULE_COMMON, &s_mmiphone_applet_info );
}

/*****************************************************************************/
// 	Description : set PUK remain flag//用于记录PUK输入正确、以及PUK被锁情况
//	Global resource dependence : none
//  Author: 
//	Note:  1表示PUK输入正确；2表示PUK输入正确，且第二张sim卡pin码需要创建
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetPukRemainStatus(uint8 set_value)
{
	s_puk_remain_status = set_value;
	SCI_TRACE_LOW("MMIAPIPHONE_SetPukRemainStatus s_puk_remain_flag=%d", s_puk_remain_status);
}
/*****************************************************************************/
// 	Description : get PUK remain flag//用于记录PUK输入正确、以及PUK被锁情况
//	Global resource dependence : none
//  Author: 
//	Note:1表示PUK输入正确；2表示PUK输入正确，且第二张sim卡pin码需要创建
/*****************************************************************************/
PUBLIC uint8 MMIAPIPHONE_GetPukRemainStatus(void)  
{
	SCI_TRACE_LOW("MMIAPIPHONE_GetPukRemainStatus s_puk_remain_flag=%d", s_puk_remain_status);
	return s_puk_remain_status;
}

/*****************************************************************************/
// 	Description : 获取sim2pin码标志 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetSim2PinQueryFlay(void)  
{
	SCI_TRACE_LOW("MMIAPIPHONE_GetSim2PinQueryFlay s_second_pin_query_flag=%d", s_second_pin_query_flag);
	return s_second_pin_query_flag;
}
#ifdef MMI_VOLTE_SUPPORT
#include "mem_prod.h"
LOCAL void MMIAPIPHONE_SetImsStatus(MN_DUAL_SYS_E dual_sys, MN_PLMN_T hplmn)
{
#ifdef MMI_IMS_TOGGLE_CONFIG_PARAMETER
    uint32 i = 0;
    uint16 len=MAX_IMS_TOGGLE_MNC_NUM;
    unsigned char *addr;
    MMI_IMS_TOGGLE_MNC_T *p_mmi_ims_toggle_list=&s_ims_toggle_mnc_table;
    MMI_IMS_TOGGLE_ST  *p_mmi_ims_toggle=NULL;
#ifndef WIN32
    PROD_GetImsToggleMemSection(&addr);
    p_mmi_ims_toggle=(MMI_IMS_TOGGLE_ST *)addr;
    SCI_TRACE_LOW("MMIAPIPHONE_SetImsStatus: IMS Toggle Sector addr is 0x%x, flag is 0x%x",(int32)addr,p_mmi_ims_toggle->flag);
    if(0xface==p_mmi_ims_toggle->flag)
    {
        SCI_TRACE_LOW("MMIAPIPHONE_SetImsStatus use IMS sector data, sim mcc=%d, mnc=%d, toggle_len=%d",hplmn.mcc,hplmn.mnc,p_mmi_ims_toggle->len);
        // Only when flag equal to 0xface and len less than 512, the data is validate.
        if(p_mmi_ims_toggle->len < 512)
        {
            p_mmi_ims_toggle_list=(MMI_IMS_TOGGLE_MNC_T *)(((uint32)addr)+sizeof(MMI_IMS_TOGGLE_ST));
            len=p_mmi_ims_toggle->len/sizeof(MMI_IMS_TOGGLE_MNC_T);
        }
    }
    else
    {
        SCI_TRACE_LOW("MMIAPIPHONE_SetImsStatus use local data, sim mcc=%d, mnc=%d",hplmn.mcc,hplmn.mnc);
    }
#endif
    //mcc mnc check	
    for(i=0;i<len;i++)
    {
        MMI_IMS_TOGGLE_MNC_T *lp_mmi_ims_toggle_list=p_mmi_ims_toggle_list+i;
        if ((lp_mmi_ims_toggle_list->mcc == hplmn.mcc) && (lp_mmi_ims_toggle_list->mnc == hplmn.mnc))
        {
            s_volte_icon_display_style[dual_sys] = lp_mmi_ims_toggle_list->ims_call_icon;
            s_volte_icon_display_set[dual_sys] = lp_mmi_ims_toggle_list->ims_icon_status;
            s_volte_menu_display_set[dual_sys] = lp_mmi_ims_toggle_list->ims_toggle_status;
	    return ;		
        }
    }

#endif
    s_volte_icon_display_style[dual_sys] = 0;
    s_volte_icon_display_set[dual_sys] =1;
    s_volte_menu_display_set[dual_sys] =1;
}
#endif
/*****************************************************************************/
// 	Description : 获取sim2pin码标志 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetSim2PinQueryFlay(BOOLEAN pin_flag)  
{	
     s_second_pin_query_flag = pin_flag;
	SCI_TRACE_LOW("MMIAPIPHONE_SetSim2PinQueryFlay s_second_pin_query_flag=%d", s_second_pin_query_flag);
}
/*****************************************************************************/
//  Description : the process message function of the virtual window
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMI_RESULT_E  IdleApplet_HandleEvent(    
                                     IAPPLET_T*          iapplet_ptr,
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{

    MMI_RESULT_E    result = MMI_RESULT_FALSE;
	//MMIFILE_DEVICE_E device_type = MMI_DEVICE_SDCARD;    
	
#ifdef HERO_ENGINE_SUPPORT
	result = MMIHEROAPP_handleMrMsg(msg_id, param);
#endif 
#ifdef MRAPP_SUPPORT
	result = MMIMRAPP_handleMrMsg(msg_id, param);
#endif 
    //SCI_TRACE_LOW:"VirWin_HandleMsg THE msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2039_112_2_18_2_45_3_0,(uint8*)"d",msg_id);
    switch (msg_id)
    {
#if defined MMI_AUTOTEST_SUPPORT && defined ENG_SUPPORT
    case MSG_MMI_AUTO_TEST_MODULE:
        MMI_HandleAutoTestModule( (const char*)param );
        break;
#endif
        
    case MSG_KEYDOWN_SDCARD_DETECT:
    case MSG_KEYUP_SDCARD_DETECT:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive SD 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2044_112_2_18_2_45_3_1,(uint8*)"d",msg_id);
		MMIAPISD_HandleSDPlug((void *)param, msg_id);
        //MMIAPISD_HandleSDPlug(msg_id);
        break;

#ifdef DPHONE_SUPPORT 
#else       
    case MSG_HANDLE_SD_ERR:	
		MMIAPISD_HandleSDPlug((void *)param, msg_id);	
        break;   
#endif   

#ifdef JAVA_SUPPORT_MYRIAD
    case MSG_JAVA_MMI_EVENT:
        MMIAPIJAVA_HandleJbedMMIEvent(param);
        break;
    case MSG_JAVA_OTA_INSTALL_START:
        {
            SCI_TRACE_LOW("IdleApplet_HandleEvent:receive MSG_JAVA_OTA_INSTALL_START!");
#ifdef BROWSER_SUPPORT
            if(MMIBROWSER_SUCC == MMIAPIBROWSER_Close())
            {
                if(PNULL != param)
                {
                    uint16* path_name = (uint16*)param;

                    MMIAPIJAVA_Install(path_name,MMIAPICOM_Wstrlen(path_name));
                }
            }
#endif
        }
        break;
#endif

    case MSG_APP_RED:
        result = MMI_RESULT_TRUE;
        break;
#ifdef MOTION_SENSOR_TYPE_OPTICAL
    case MSG_GSENSOR:      
        SCI_TRACE_LOW("receive MSG_OPSENSOR");
        result = MMIDEFAULT_HandleOpsensorMsg(param);
        break;
#endif
    case MSG_FILE_READFILE_ASYN_CALLBACK:
    case MSG_FILE_WRITEFILE_ASYN_CALLBACK:
    case MSG_FILE_READFILE_BYOFFSET_ASYN_CALLBACK:
    case MSG_FILE_WRITEFILE_BYOFFSET_ASYN_CALLBACK:
        MMIAPIFMM_OperationAsynCnf((void*)param, msg_id);
        break;
        
//    case MSG_FMM_COPY_FILE_READ_CNF:
//        MMIFMM_FileCopyWriteDest(*(SFS_ERROR_E *)param);
//        break;
//        
//    case MSG_FMM_COPY_FILE_WRITE_CNF:
//        MMIFMM_FileCopyReadSrc(*(SFS_ERROR_E *)param);
//         break;
        
    case MSG_FILE_COPY_READ_CNF:
        MMIAPIFMM_CopyWriteDest(*(SFS_ERROR_E *)param);
        break;
        
    case MSG_FILE_COPY_WRITE_CNF:
        MMIAPIFMM_CopyReadSrc(*(SFS_ERROR_E *)param);
        break;
        
    case MSG_ALM_ALERTING_END:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_ALM_ALERTING_END"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2090_112_2_18_2_45_3_2,(uint8*)"");
        if (!MMK_IsFocusWin(PHONE_POWER_OFF_WIN_ID))
        {
            if(MMK_IsOpenWin(PHONE_STARTUP_ALARM_WIN_ID))
            {
                MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);
            }
            MMK_CreateWin((uint32*)PHONE_STARTUP_ALARM_WIN_TAB,PNULL);
        }
        result = MMI_RESULT_TRUE;
        break;

#ifdef MMI_AUDIO_PLAYER_SUPPORT
    case MSG_MP3_PLAY_CALL_BACK:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_MP3_PLAY_CALL_BACK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2103_112_2_18_2_45_3_3,(uint8*)"");
        //Mp3正常播放结束的回调函数。
        MMIAPIMP3_PlayMp3CallBack();
        break;

    case MSG_MP3_FATAL_ERROR:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_MP3_FATAL_ERROR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2109_112_2_18_2_45_3_4,(uint8*)"");
        MMIAPIMP3_HandleFatalError();
        break;
#ifdef  MMIAP_LYRIC_SUPPORT    
    case MSG_MP3_START_LIRIC:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received [MMIMP3] MSG_MP3_START_LIRIC"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2114_112_2_18_2_45_3_5,(uint8*)"");
        MMIAPIMP3_StartLyric();
        break;
#endif
    case MSG_MP3_FOLDER_FULL:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received [MMIMP3] MSG_MP3_FOLDER_FULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2119_112_2_18_2_45_3_6,(uint8*)"");
        {
			if (PNULL != param)
			{
                MMIFILE_DEVICE_E dev = (MMIFILE_DEVICE_E)param;/*lint !e549*/
                MMIAPIMP3_FolderFull(dev);
			}
        }
        
        break;
#endif
#ifdef KURO_SUPPORT
    case MSG_KUR_PLAY_CALL_BACK:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_KUR_PLAY_CALL_BACK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2124_112_2_18_2_45_3_7,(uint8*)"");
        MMIAPIKUR_PlayKurCallBack();
        break;

    case MSG_KUR_FATAL_ERROR:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_KUR_FATAL_ERROR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2129_112_2_18_2_45_3_8,(uint8*)"");
        MMIAPIKUR_HandleFatalError();
        break;
#endif

    case MSG_MPEG4_DELETE_ONE:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_MPEG4_DELETE_ONE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2134_112_2_18_2_45_3_9,(uint8*)"");
        break;
        
   case MSG_MMS_ACTIVE_RECV_SEND:
       //SCI_TRACE_LOW:"VirWin_HandleMsg: MMIAPIMMS_ActiveRecvSendModule"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2138_112_2_18_2_45_3_10,(uint8*)"");
       //启动彩信发送和接收，通过异步实现
       MMIAPIMMS_ActiveRecvSendModule();
       break;
        
    case MSG_MMIDV_DEL_UNSAVED_MPEG4:
        MMITHEME_SetUpdateDelayCount(2);
        break;
        
    case MSG_MMIDV_CLOSE_RECORD_WIN:
        MMIAPIDC_Exit();
        break;
        
//    case MSG_CMSBRW_FS_READ:	//wanqiangzh modify for STK
//    case MSG_CMSBRW_FS_WRITE:	//wanqiangzh modify for STK
//        MMICMSBRW_HandleWriteAndRead(msg_id, param);
//        break;
//    case MSG_CMSBRW_STOP_BGSOUND:
//        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
//        break;

    case MSG_RECORD_END:
        MMIAPIDC_Exit();
        break;
//    case MSG_CMSBRW_BRW_OP:
//        SCI_TRACE_LOW("VirWin_HandleMsg:MSG_CMSBRW_BRW_OP");
//        {			
//            if(param!=PNULL)
//            {
//                MMIBRW_VIRWIN_MESSAGE_T      *message=NULL;
//                message = (MMIBRW_VIRWIN_MESSAGE_T *)param;
//                MMIBRW_HandleBrwOpMsg(READ_FILE_RESP,&message->param);	
//                //SCI_Free(message->param);
//                //SCI_Free(message);
//            }
//            else
//            {
//                SCI_TRACE_LOW("warning:param null");
//            }
//        }
//        
//        break;
#ifdef  CAMERA_SUPPORT        
    case MSG_DV_OPEN_MSG:
        MMIAPIDC_OpenVideoWin();
        break;
        
    case MSG_DC_OPEN_MSG:
        MMIAPIDC_OpenPhotoWin();
        break;
#endif
        //msg for MobileTV
    case MSG_MBBMS_PRE_EXIT_MSG:
#ifdef VT_SUPPORT
        if(!MMIAPIVT_IsVtCalling())
        {
            MMIAPIMTV_PreExitCallback();
        }
#else
        MMIAPIMTV_PreExitCallback();
#endif
        break;
#ifdef MMI_VCALENDAR_SUPPORT
    case MSG_VCALENDAR_WRITE_SMS_CNF:
    case MSG_VCALENDAR_SAVE_SCH_CNF:
        MMIVCAL_HandleWriteCNF(msg_id, param);
        break;
#endif
#ifdef 	JAVA_SUPPORT_IA		
#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
    case MSG_JAVA_OTA_DOWNLOAD_START:
        MMIAPIJAVA_OpenWapWin();        
        break;
#endif
#endif

    case MSG_SET_OPENALARM_EXPIRED_WIN:
        MMIAPIALM_OpenAlarmExpiredWin();
        break;   
        
    case MSG_SLEEP_LCD:
        MMIDEFAULT_TurnOffBackLight();
        break;

    case MSG_AUDIO_CALLACK:
        MMISRVAUD_HandleCallBackMsg((void *)param);
        break;
   case MSG_AUDIO_EU_ALERT_NOTIFY:
	   {
		   //MMI_STRING_T initing_str = {0};
		   
		  // SCI_MEMSET(&initing_str, 0, sizeof(initing_str));
		   //MMI_GetLabelTextByLang(TXT_WARNING, &initing_str);
		   
		   //MMIPUB_OpenWaitWin(1,&initing_str,PNULL,PNULL,PNULL,IMAGE_NULL,

            //IMAGE_PUBWIN_WARNING,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,PNULL);
         MMISRVAUD_StartEUAlertCount();
         MMIPUB_OpenAlertWarningWin(STR_NOTE_EU_HIGHVOL);

	    //MMK_CreatePubListWin((uint32 *)EU_ALERTWIN_TAB, PNULL);  
	   }
	    break;
    case MSG_SET_WALLPAPER_WAIT:
        MMIAPIFMM_HandleWallpaperWaitMsg();
        break;
        
#ifdef BROWSER_SUPPORT_DORADO
    case BRW_SNAPSHOT_FFS_PAGE_SAVE_CNF:			
    case BRW_SNAPSHOT_FFS_CSS_SAVE_CNF:		
    case BRW_SNAPSHOT_FFS_IMAGE_SAVE_ONE_CNF:	
    case BRW_SNAPSHOT_FFS_IMAGE_SAVE_CNF:
    case BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF:
    case BRW_SNAPSHOT_FFS_IMAGE_OPEN_ONE_CNF:
	BRW_SnapshotHandleFfsMsg(msg_id,*((SFS_ERROR_E*)param));
	break;
    
    case MSG_BRW_MSG_INIT_CNF:
    case MSG_BRW_MSG_EXIT_CNF:
    case MSG_BRW_MSG_TYPE_ENTER_FILENM_REQ:
    case MSG_BRW_MSG_TYPE_DL_CONFIRM_REQ:     
    case MSG_BRW_MSG_TYPE_AUTH_REQ:           
    case MSG_BRW_MSG_TYPE_CHANGE_URL_IND:     
    case MSG_BRW_MSG_TYPE_NEW_PAGE_REQ_IND:   
    case MSG_BRW_MSG_TYPE_UPDATE_CONTENT_IND: 
    case MSG_BRW_MSG_TYPE_PROGRESS_IND:       
    case MSG_BRW_MSG_TYPE_ERROR_IND:            
    case MSG_BRW_MSG_TYPE_DOWNLOAD_OK_IND:     
    case MSG_BRW_MSG_TYPE_DOWNLOAD_ERR_IND:   
    case MSG_BRW_MSG_TYPE_PAGE_GET_IND:
    case MSG_BRW_MSG_TYPE_MEDIA_GET_ONE_IND:
    case MSG_BRW_MSG_TYPE_DISPLAY_READY_IND:  
    case MSG_BRW_MSG_TYPE_SSL_CERT_IND:       
    case MSG_BRW_MSG_TYPE_REPAINT_READY_IND:
    case MSG_BRW_MSG_TYPE_PAGE_STOP_IND:
    case MSG_BRW_MSG_TYPE_OMA_DL_CONFIRM_REQ:        
    case MSG_BRW_MSG_TYPE_LAYOUTCTRL_DESTROY_REQ:
//    case MSG_BRW_MSG_TYPE_PDP_ACTIVE_CNF:
    case MSG_BRW_STOP_BGSOUND:
    case MSG_BRW_MSG_TYPE_HTTP_REFRESH_IND:        
        MMIAPIBROWSER_HandleAppMsg(msg_id,param);
        break;
        
#endif

#ifdef BROWSER_SUPPORT
    case MSG_BROWSER_MANAGER_MAKECALL_REQ:
    case MSG_BROWSER_MANAGER_SENDDTMF_REQ:
    case MSG_BROWSER_MANAGER_ADD_PBITEM_REQ:
    case MSG_BROWSER_MANAGER_OPEN_RTSP_IND:
    case MSG_BROWSER_MANAGER_SENDSMS_REQ:
    case MSG_BROWSER_MANAGER_SENDSMSTO_REQ:
    case MSG_BROWSER_MANAGER_SENDMMS_REQ:
    case MSG_BROWSER_MANAGER_SENDMMSTO_REQ:
    case MSG_BROWSER_MANAGER_SENDMAIL_REQ:
    case MSG_BROWSER_MANAGER_SENDMAILTO_REQ:
    case MSG_BROWSER_MANAGER_TEL_REQ:
        MMIAPIBROWSER_HandleAppMsg(msg_id,param);
        break;
#endif

    case MSG_APPLET_CLEAR_FREE_MODULE:
        MMK_PostMsg( MMIAPPLET_MANAGER_LIST_WIN_ID, MSG_APPLET_MANAGER_UPDATE, PNULL, 0 );
        break;

    case MSG_APPLET_SWITCH:
        MMK_ReturnIdleWin();
        break;
        
#if defined(OPERA_MOBILE_SUPPORT_V11) || defined(OPERA_MINI_SUPPORT_VER42) || defined(OPERA_MINI_SUPPORT_VER6)
    case MSG_MMI_OPERA_ASYNC_EXIT:
         {
            MMIBROWSER_ENTRY_PARAM *ptr = (MMIBROWSER_ENTRY_PARAM *)param;

#ifdef BROWSER_SUPPORT_DORADO
#ifndef WIN32
            //opera exit
            MMIAPIBROWSER_Close();//  OperaMini_Exit()
#endif
            //dorado entry
            if (ptr != 0)
            {
                MMIAPIBROWSER_Entry(ptr);
                if (ptr->url_ptr != 0)
                {
                    SCI_FREE(ptr->url_ptr);
                }
            }
        }
#else
            //only free
            if (ptr != 0)
            {
                if (ptr->url_ptr != 0)
                {
                    SCI_FREE(ptr->url_ptr);
                }
            }
        }
#endif
        break;
#endif        
        
#ifdef TTS_SUPPORT
#ifndef WIN32
    case MSG_TTS_PLAY:
        //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_PLAY"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2302_112_2_18_2_45_4_11,(uint8*)"");
        MMIAPITTS_SynthTextByTask();
        break;
    case MSG_TTS_STOP:
        //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_STOP"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2306_112_2_18_2_45_4_12,(uint8*)"");
        MMIAPITTS_StopSynthTextByTask();
        break;
    case MSG_TTS_OVER:
#ifdef TTS_SUPPORT_EJ	
        //SCI_TRACE_LOW:"virwin_MSG_TTS_OVER"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2311_112_2_18_2_45_4_13,(uint8*)"");
        MMK_PostMsg( MMIKL_CLOCK_DISP_WIN_ID == MMK_GetFocusWinId() ? MMK_GetPrevWinId(MMIKL_CLOCK_DISP_WIN_ID) : MMK_GetFocusWinId(), MSG_TTS_OVER, PNULL, 0 );
#endif
        break;
    case MSG_TTS_AUDIO_PLAY:
        {
            uint32 pcm_stream_id = *(uint32*)param;
            //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_PLAY"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2318_112_2_18_2_45_4_14,(uint8*)"");
            MMIAPITTS_PlayPcmStream(pcm_stream_id);
        }
        break;

    case MSG_TTS_ERROR_RESOURCE:
#ifdef EBOOK_SUPPORT
        if(MMK_IsOpenWin(MMIEBOOK_TXT_SHOW_WIN_ID))
        {
            MMIAPIEBOOK_SetTTSOnOFF(FALSE);
        }
#endif
#if 0        
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_TTS_NO_RESOURCE, 
            TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
#endif
        break;
#endif
#endif
#ifdef HERO_ENGINE_TTS_SUPPORT
#ifndef WIN32
    case MSG_TTS_PLAY:
        //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_PLAY"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2302_112_2_18_2_45_4_11,(uint8*)"");
        MMIAPITTS_SynthTextByTask();
        break;
    case MSG_TTS_STOP:
        //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_STOP"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2306_112_2_18_2_45_4_12,(uint8*)"");
        MMIAPITTS_StopSynthTextByTask();
        break;
    case MSG_TTS_OVER:
#ifdef TTS_SUPPORT_EJ	
        //SCI_TRACE_LOW:"virwin_MSG_TTS_OVER"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2311_112_2_18_2_45_4_13,(uint8*)"");
        MMK_PostMsg( MMIKL_CLOCK_DISP_WIN_ID == MMK_GetFocusWinId() ? MMK_GetPrevWinId(MMIKL_CLOCK_DISP_WIN_ID) : MMK_GetFocusWinId(), MSG_TTS_OVER, PNULL, 0 );
#endif
        break;
    case MSG_TTS_AUDIO_PLAY:
        {
            uint32 pcm_stream_id = *(uint32*)param;
            //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_PLAY"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2318_112_2_18_2_45_4_14,(uint8*)"");
            MMIAPITTS_PlayPcmStream(pcm_stream_id);
        }
        break;

    case MSG_TTS_ERROR_RESOURCE:
        break;
#endif
#endif
#if 1 //def FILE_LOG_SUPPORT
	case MSG_USRE_LOG_SAVE:
	{
		  Trace_Log_Save();
	}
	break;
	case MSG_USRE_LOG_AT_SAVE:
	{
		Trace_AT_Save();
	}
	break;
	case MSG_USRE_LOG_SOCKET_SAVE:
	{
		Trace_Socket_Save();
	}
	break;
	case MSG_USRE_LOG_BUF_SAVE:
	{
		Trace_BUF_Save();
	}
	break;
	case MSG_USRE_LOG_EXT_SAVE:
	break;			
	case MSG_USRE_LOG_DEL_ALL:
	{
		UserFile_RemoveDirFile("TEST");
	}
	break;
#endif

	default:
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : if use CPHS 
//    when the call connection is not encrypted
//	Global resource dependence : 
//  Author:baokun
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsCPHSSupport(void)
{
    return  s_is_cphs_supported;
}

PUBLIC char MMIAPIPHONE_4g_icon_display_style(void)
{
    return  s_4g_icon_display_style;
}

PUBLIC char MMIAPIPHONE_volte_icon_display_style(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
        return 0;
	
    return  s_volte_icon_display_style[dual_sys];
}

PUBLIC char MMIAPIPHONE_volte_icon_display_set(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
        return 0;
	   
    return s_volte_icon_display_set[dual_sys];
}

PUBLIC char MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_E dual_sys)
{
    char menu_display_status = 0;
    if(dual_sys >= MMI_DUAL_SYS_MAX)
        return 0;

    if((s_volte_menu_display_set[dual_sys] == 1) || (s_volte_menu_display_set[dual_sys]==3))
        menu_display_status = 1;

    return menu_display_status;
}

PUBLIC char MMIAPIPHONE_ims_value_set(MN_DUAL_SYS_E dual_sys)
{
    char ims_value = 0;
    if(dual_sys >= MMI_DUAL_SYS_MAX)
        return 0;

    if((s_volte_menu_display_set[dual_sys] == 1) || (s_volte_menu_display_set[dual_sys]==2))
        ims_value = 1;

    return ims_value;
}
PUBLIC char MMIAPIPHONE_prefer_network_display_style(void)
{
	return s_prefer_network_menu_display_style;
}

PUBLIC BOOLEAN MMIAPIPHONE_IsIndiaSOSSupport(void)
{
#ifdef MMI_INDIAN_SOS_SUPPORT
    return s_is_indiasos_supported;
#else
    return FALSE;
#endif
}
/*****************************************************************************/
// 	Description : init the phone application
//	Global resource dependence : g_service_status,g_phone_app
//  Author: Jassmine
//	Note:
/*****************************************************************************/
void MMIAPIPHONE_AppInit(void)
{
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
    uint16   nv_len = 0;

#if 0//def FILE_LOG_SUPPORT
	TraceUser_Init();   ///FILE_LOG_SUPPORT  ZDT_PRODUCT_Z685S
    Trace_Log_Print("MMIAPIPHONE_AppInit");
#endif
	nv_len = 1;//NVITEM_GetLength(SETTINGS_NV_DEFAULT_BOOKMARK);
    if(nv_len == sizeof(s_is_cphs_supported))
	{
		EFS_NvitemRead(TELE_NV_SET_CPHS_ON, nv_len, (uint8*)&s_is_cphs_supported);
	}		
// 4G icon display style
    nv_len = NVITEM_GetLength(OTHER_NV_4G_ICON_DISPLAY_STYLE);
    if(NVERR_NONE != EFS_NvitemRead(OTHER_NV_4G_ICON_DISPLAY_STYLE, nv_len, (uint8*)&s_4g_icon_display_style))
    {
        s_4g_icon_display_style = 0;
    }
	
// Volte icon display style
/*    nv_len = NVITEM_GetLength(OTHER_NV_VOLTE_ICON_DISPLAY_STYLE);
    if(NVERR_NONE != EFS_NvitemRead(OTHER_NV_VOLTE_ICON_DISPLAY_STYLE, nv_len, (uint8*)&s_volte_icon_display_style))
    {
        s_volte_icon_display_style = 0;
    }*/
// Volte icon display set
 /*   nv_len = NVITEM_GetLength(OTHER_NV_VOLTE_ICON_DISPLAY_SET);
    if(NVERR_NONE != EFS_NvitemRead(OTHER_NV_VOLTE_ICON_DISPLAY_SET, nv_len, (uint8*)&s_volte_icon_display_set))
    {
        s_volte_icon_display_set = 1;
    }	*/	
// Volte menu display set
/*    nv_len = NVITEM_GetLength(OTHER_NV_VOLTE_MENU_DISPLAY_SET);
    if(NVERR_NONE !=EFS_NvitemRead(OTHER_NV_VOLTE_MENU_DISPLAY_SET, nv_len, (uint8*)&s_volte_menu_display_set))
    {
        s_volte_menu_display_set = 1;
    }	*/
//Prefer network display style
    nv_len = NVITEM_GetLength(OTHER_NV_NETWORK_MENU_DISPLAY_STYLE);
    if(NVERR_NONE !=EFS_NvitemRead(OTHER_NV_NETWORK_MENU_DISPLAY_STYLE, nv_len, (uint8*)&s_prefer_network_menu_display_style))
    {
    	s_prefer_network_menu_display_style = 0;
    }		

    // 初始化记录服务状态的结构，主要用于IDLE界面显示
    SCI_MEMSET(&s_sim_status[0], 0, sizeof(s_sim_status));
    SCI_MEMSET(&g_service_status[0], 0, sizeof(g_service_status));
    SCI_MEMSET(&g_pin_value[0], 0, sizeof(g_pin_value)); 
    
    //Udisk没有在运行
    MMIAPIUdisk_SetRunStatus(FALSE);
    
    g_mmiphone_app.ProcessMsg = HandlePsAndRefMsg;
    
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        s_sim_status[i].simStatus = SIM_NOT_INITED;

        g_service_status[i].is_first_rssi    = TRUE;
        g_service_status[i].is_slecting = TRUE; 
        
        s_is_ps_ready[i]= FALSE;
        s_ps_ready[i] = MMIPHONE_PS_POWER_ON_NONE;

        s_is_ps_deactive_cnf[i] = TRUE;

        s_sim_existed_status[i] = TRUE;
    } 
#ifdef DPHONE_SUPPORT
	MMIDEFAULT_InitHookAndHF();
#endif

#if defined(MMI_UPDATE_TIME_SUPPORT)
    MMIAPIAUT_AutoUpdateTimeInit();
#endif/*MMI_UPDATE_TIME_SUPPORT*/

#ifdef DRM_SUPPORT
#ifdef BROWSER_SUPPORT    
    {
	MMIBROWSER_MIME_DOWNLOAD_PARAM_T drm_callback = {0};

	drm_callback.call_back = MMIDRM_HandleDLDrmCB;
	
	drm_callback.mime_type = MMIBROWSER_MIME_TYPE_DM;
	MMIAPIBROWSER_RegMimeFunc(&drm_callback);
	
	drm_callback.mime_type = MMIBROWSER_MIME_TYPE_DCF;
	MMIAPIBROWSER_RegMimeFunc(&drm_callback);
    }
#endif
#endif
    MMIPHONE_InitNITZ();
    MMIPHONE_InitNetworkNameInfo();
    
    nv_len = NVITEM_GetLength(SETTINGS_NV_INDIAN_SOS_SUPPORT);
#ifdef MMI_INDIAN_SOS_SUPPORT
    if(NVERR_NONE !=EFS_NvitemRead(SETTINGS_NV_INDIAN_SOS_SUPPORT, nv_len, (uint8*)&s_is_indiasos_supported))
    {
    	s_is_indiasos_supported = 0;
    }		
#endif
#ifdef MMI_RESET_PHONE_AUTO_PIN
    {
        MN_BLOCK_T sim_pin_value[MMI_DUAL_SYS_MAX] = {0};
        MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
        //get imsi from nv
        MMINV_READ(MMINV_SIM_PIN_VALUE,sim_pin_value,return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            SCI_MEMSET(sim_pin_value, 0, sizeof(MN_BLOCK_T) * MMI_DUAL_SYS_MAX);
            MMINV_WRITE(MMINV_SIM_PIN_VALUE,sim_pin_value);
        }
        SCI_MEMCPY(&g_pin_value[0], &sim_pin_value, sizeof(g_pin_value)); 
    }
#endif        
}
#ifdef UNISOC_MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : handle the sim lock info
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void HandleSimLockInfo(APP_MN_SIM_NOT_READY_IND_T   sim_not_ready_ind)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MN_IMSI_T           cur_imsi_info = {0};
    MN_SIM_LOCK_STATUS_T sim_unlock_status = {0};
    
    MMINV_READ(MMI_NV_SIM_LOCK_INFO_ID, &g_sim_lock_info, return_value);
    if(MN_RETURN_SUCCESS !=return_value)
    {

        if(MNPHONE_GetSIMLockStatus(&sim_unlock_status))
        {
            g_sim_lock_info.counter = sim_unlock_status.max_num_trials;
	 }
        else
        {
        g_sim_lock_info.counter = MAX_SIM_LOCK_NUMBER;
        }

        g_sim_lock_info.imsi_info = MNSIM_GetImsiEx(sim_not_ready_ind.dual_sys);
        g_sim_lock_info.cur_sim_id = sim_not_ready_ind.dual_sys;
        MMINV_WRITE(MMI_NV_SIM_LOCK_INFO_ID, &g_sim_lock_info);   
    }
    else
    {
        //如果不需要init，则判断是否为同一张卡，不同卡则重置
        cur_imsi_info = MNSIM_GetImsiEx(sim_not_ready_ind.dual_sys);
        //不一致
        if((MMIAPICOM_StrCmp(cur_imsi_info.imsi_val, cur_imsi_info.imsi_len, g_sim_lock_info.imsi_info.imsi_val, g_sim_lock_info.imsi_info.imsi_len)))
        {
            //只判断卡1的情况
            if(sim_not_ready_ind.dual_sys != MN_DUAL_SYS_2
                || !MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1) )
            {
                SCI_TRACE_LOW("[UNISOC_SIM_LOCK]HandleSimLockInfo,not ready sim=%d",sim_not_ready_ind.dual_sys);          
			if(MNPHONE_GetSIMLockStatus(&sim_unlock_status))
			{
				g_sim_lock_info.counter = sim_unlock_status.max_num_trials;
			}
			else
			{
                g_sim_lock_info.counter = MAX_SIM_LOCK_NUMBER;
			}
                g_sim_lock_info.imsi_info = cur_imsi_info;
                g_sim_lock_info.cur_sim_id = sim_not_ready_ind.dual_sys;
                MMINV_WRITE(MMI_NV_SIM_LOCK_INFO_ID, &g_sim_lock_info);
         
            }
         
        }           
    }
    SCI_TRACE_LOW("HandleSimLockInfo cur_dual_sys = %d",sim_not_ready_ind.dual_sys);
    SCI_TRACE_LOW("HandleSimLockInfo counter = %d", g_sim_lock_info.counter);
    SCI_TRACE_LOW("HandleSimLockInfo nv_dual_sys = %d",g_sim_lock_info.cur_sim_id);
    SCI_TRACE_LOW("mmiphone.c HandleSimLockInfo read_nv result =%d",return_value);
    SCI_TRACE_LOW("mmiphone.c  counter =%d",g_sim_lock_info.counter);
}
#endif
/*****************************************************************************/
// 	Description : 处理PHONE APP的PS消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePsAndRefMsg(
                                     PWND                app_ptr, 
                                     uint16              msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E   e_dual_sys = MN_DUAL_SYS_1;

    switch (msg_id)
    {
    case APP_MN_NETWORK_STATUS_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_NETWORK_STATUS_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2430_112_2_18_2_45_4_15,(uint8*)"");
        // 该消息用于在网络服务状态改变时，汇报当前网络服务状态，是紧急呼叫状态，
        // 还是无服务状态，还是GSM服务状态，还是GSM、GPRS状态。
        // 获得网络的状态需在idle界面显示。
        result = HandleNetworkStatusInd(param);
        break;

    case APP_MN_NETWORK_INFO_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_NETWORK_INFO_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2438_112_2_18_2_45_4_16,(uint8*)"");
        // 该消息用于在网络信息改变时，上报当前网络PLMN
        // 网络名称,包括full name/short name,其中每个结构都有状态、字符类型、长度等
        // 还有当前的网络时间LSA等信息
        result = MMIPHONE_HandleNetworkInfoInd(param);
        break;
        
    case APP_MN_PREFER_RAT_RSP:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PREFER_RAT_RSP"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2446_112_2_18_2_45_4_17,(uint8*)"");
        result  = HandlePreferRatRsp(param);
        break;
        
    case APP_MN_PLMN_LIST_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PLMN_LIST_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2451_112_2_18_2_45_4_18,(uint8*)"");
        //该消息用于通知用户MNPHONE_ListPLMN()的操作结果。
        // 结构中包含存在网络的列表和每个网络的具体信息。
        result = HandlePlmnListCnf(param);
        break;
        
    case APP_MN_PLMN_SELECT_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PLMN_SELECT_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2458_112_2_18_2_45_4_19,(uint8*)"");
        // 该消息用于通知用户MNPHONE_SelectPLMN()的操作结果。
        // 结构中包含选择操作完成后当前的网络状况。
        result = HandleSelectPlmnCnf(param);
        break;
        
    case APP_MN_DISPLAY_PLMN_IND:
        e_dual_sys = ((APP_MN_DISPLAY_PLMN_IND_T *)param)->dual_sys;
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_DISPLAY_PLMN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2466_112_2_18_2_45_4_20,(uint8*)"");
        //该消息用于通知用户进行手动选网	
        //if (!MMK_IsFocusWin(PHONE_POWER_OFF_WIN_ID))
        //{
        //    result = MMIAPISET_HandleDisplayPlmnInd(e_dual_sys);
        //}
        break;
        
    case APP_MN_SIM_PRESENT_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_PRESENT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2475_112_2_18_2_45_4_21,(uint8*)"");
        result = HandleSimPresentInd(param);
        break;
        
    case APP_MN_GPRS_ATTACH_RESULT:
        MMIAPICONNECTION_DetachIfGprsOff();
        result = MMI_RESULT_FALSE;
        break;

#ifdef SIM_PLUG_IN_SUPPORT        
    case APP_MN_SIM_PLUG_IN_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_PLUG_IN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2480_112_2_18_2_45_4_22,(uint8*)"");
        result = HandleSimPlugInInd(param);
        break;
#endif        

#ifdef MMI_LDN_SUPPORT
    case APP_MN_LND_READY_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_LND_READY_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2486_112_2_18_2_45_4_23,(uint8*)"");
        result = HandleLdnReadyInd(param);
        break;
    case APP_MN_LND_UPDATE_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_LND_UPDATE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2490_112_2_18_2_45_4_24,(uint8*)"");
        result = HandleLDNUpdateCnf(param);
        break;
#endif
        
    case APP_MN_SET_BAND_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SET_BAND_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2496_112_2_18_2_45_4_25,(uint8*)"");
        // 该消息用于通知用户MNPHONE_SelectBand()的操作结果。
        // 结构中包含选择操作完成后当前的Band状况。
        result = HandleSelectBandCnf(param);
        MMIAPIENG_BandSelectCnf(param);
        break;
        
    case APP_MN_SIM_GET_PIN_IND:
    {
#ifdef MMI_RESET_PHONE_AUTO_PIN
        POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();
#endif

        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_GET_PIN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2504_112_2_18_2_45_4_26,(uint8*)"");
#if ((defined MMI_SIM_LOCK_SUPPORT || defined UNISOC_MMI_SIM_LOCK_SUPPORT)) && (defined MMI_RESET_PHONE_AUTO_PIN) 
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        SCI_TRACE_LOW("[UNISOC_SIM_LOCK]HandlePsAndRefMsg s_use_dual_pin_in_nv[1]=%d",s_use_dual_pin_in_nv[MN_DUAL_SYS_1]);
#else
        SCI_TRACE_LOW("[UNISOC_SIM_LOCK]HandlePsAndRefMsg s_use_dual_pin_in_nv[1][2]=%d%d",s_use_dual_pin_in_nv[MN_DUAL_SYS_1],s_use_dual_pin_in_nv[MN_DUAL_SYS_2]);
#endif
#endif
        // 该消息用于通知用户要求输入PIN1或者PUK1，并可获得PIN1是否已经被锁。
#if defined(MMI_SIM_LOCK_SUPPORT) || defined(MMI_RESET_PHONE_AUTO_PIN) || defined(UNISOC_MMI_SIM_LOCK_SUPPORT)
        if (FALSE
#if ((defined MMI_SIM_LOCK_SUPPORT || defined UNISOC_MMI_SIM_LOCK_SUPPORT)) && (defined MMI_RESET_PHONE_AUTO_PIN)
    ||s_use_dual_pin_in_nv[MN_DUAL_SYS_1]
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    ||s_use_dual_pin_in_nv[MN_DUAL_SYS_2]
#endif
#endif
#ifdef MMI_RESET_PHONE_AUTO_PIN
        || (RESTART_BY_ASSERT == restart_condition)
#endif
        )   /*lint !e774*/
        {
            APP_MN_SIM_GET_PIN_IND_T *sim_get_pin_ind_ptr = PNULL;
            
            sim_get_pin_ind_ptr = (APP_MN_SIM_GET_PIN_IND_T *)param;
#if ((defined MMI_SIM_LOCK_SUPPORT || defined UNISOC_MMI_SIM_LOCK_SUPPORT)) && (defined MMI_RESET_PHONE_AUTO_PIN)            
            SCI_TRACE_LOW("[UNISOC_SIM_LOCK]HandlePsAndRefMsg:dual_pin_sys=%d,dual_simlock_status=%d",sim_get_pin_ind_ptr->dual_sys,s_use_dual_pin_in_nv[sim_get_pin_ind_ptr->dual_sys]);
#endif
            if(MMI_DUAL_SYS_MAX<= sim_get_pin_ind_ptr->dual_sys)
            {
                //SCI_TRACE_LOW:"HandlePsAndRefMsg:dual_sys=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2514_112_2_18_2_45_4_27,(uint8*)"d",sim_get_pin_ind_ptr->dual_sys);
                result = MMI_RESULT_FALSE;
                break;
            }
            if (MNSIM_IsPin1EnableEx(sim_get_pin_ind_ptr->dual_sys)
                && MMI_PIN_MIN_LENGTH <= g_pin_value[sim_get_pin_ind_ptr->dual_sys].blocklen
#if ((defined MMI_SIM_LOCK_SUPPORT || defined UNISOC_MMI_SIM_LOCK_SUPPORT)) && (defined MMI_RESET_PHONE_AUTO_PIN)                
                &&s_use_dual_pin_in_nv[sim_get_pin_ind_ptr->dual_sys]
#endif     
                )//deactive ps后，由于底层不会自动重新输入pin码，需要 手动
            {
                //response pin before receive signal of sim is ready
                SCI_TRACE_LOW("HandlePsAndRefMsg:MNPHONE_ResponsePinEx");
                MNPHONE_ResponsePinEx(sim_get_pin_ind_ptr->dual_sys, MNSIM_PIN1, FALSE, &g_pin_value[sim_get_pin_ind_ptr->dual_sys], PNULL);
#if ((defined MMI_SIM_LOCK_SUPPORT || defined UNISOC_MMI_SIM_LOCK_SUPPORT)) && (defined MMI_RESET_PHONE_AUTO_PIN)
                s_use_dual_pin_in_nv[sim_get_pin_ind_ptr->dual_sys] = FALSE;
#endif
            }
            else
            {
                result = HandleSimPinInd(param);
            }
        }
        else
#endif
        {
            SCI_TRACE_LOW("[UNISOC_SIM_LOCK]HandlePsAndRefMsg Inputing PIN");
            result = HandleSimPinInd(param);
        }
        }
        break;
        
    case APP_MN_SIM_PIN_FUNC_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_PIN_FUNC_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2533_112_2_18_2_45_4_28,(uint8*)"");
        // 该消息用于通知用户输入的PIN1，PUK1或者PIN2,PUK2，并可获得PIN1,PIN2是否已经被锁。
        result = HandleSimPinCnf(param);
        break;


    case APP_MN_READ_SIM_EF_CNF:
        SCI_TRACE_LOW("HandlePsAndRefMsg --> APP_MN_READ_SIM_EF_CNF");
        HandleSimEFReadInd(param);
        break;
        
    case APP_MN_SIM_READY_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_READY_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2539_112_2_18_2_45_4_29,(uint8*)"");
        // 该消息用于汇报SIM卡准备就绪，同时会附带PIN的状况，HPLMN 等SIM卡的信息。
        HandleSimReadyInd(param);
		#ifdef FLASH_SUPPORT
			if(!MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)&&//pin 码
				!MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)//puk 码
                #ifdef MMI_SIM_LOCK_SUPPORT
                      &&!MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
                      &&!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
                #endif	   
				&&MMIAPIPHONE_GetOpenIdleFlag()//这里需要打开IDLE窗口
				)
			{
			   //只有在所有的pin码和puk码都输入完成之后，才尝试进入idle，
			   //且，只有在不存在防盗保护和USB窗口的情况下才能成功进入IDLE.
			   MMIAPIPHONE_SetOpenIdleFlag(FALSE);
			   MMIAPIPHONE_AtemptEnterIdle();
			}
        #endif
        //init call log
        MMIAPICL_Init();

//If HW_SUPPORT on,divert off.
#ifdef  HW_DEBUG
    {
        MN_SS_REGISTER_PARAM_U register_param = {0};
        SIMAT_DO_SS_STRING_T ss_string = {0};
        MNSS_SENDSS_RESULT_E ss_err_code = MNSS_SUCCESS;
        MN_SS_BASIC_SERVICE_T     basic_service = {0};
        BOOLEAN   is_on = FALSE;
        APP_MN_SIM_READY_IND_T      sim_ready_ind   = *((APP_MN_SIM_READY_IND_T *)param);

        basic_service.service_type = MN_SS_ALL_SERVICE;
        basic_service.service_code = 0;

        //disable all call forwarding
        ss_err_code = MNSS_SendSSEx(sim_ready_ind.dual_sys, MN_SS_ERASESS, MN_SS_ALL_FORWARD, basic_service, register_param, ss_string);
        MMINV_WRITE(MMIENG_NV_CALLFORWARD_QUERY_ID, &is_on);

        SCI_TRACE_LOW("dual sim is %d,error code is %d",sim_ready_ind.dual_sys,ss_err_code);

    }
#endif

        // get EF_CBMI info from SIM. added by Tong Yongli
        //MMIAPISMSCB_GetCBMIFromSim(param);

        result = MMI_RESULT_FALSE;
        break;

#ifdef MMI_NETWORK_NAME_SUPPORT
    case APP_MN_NETNAME_READY_IND:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_NETNAME_READY_IND");
        // 该消息用于在SIM卡初始化完成时，上报当前那张卡可以使用PNN
        result = MMIPHONE_HandleNetNameReadyInd(param);
        break;
                
    case APP_MN_OPER_NAME_IND:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_OPER_NAME_IND");
        // 该消息用于在SIM卡初始化完成时，上报当前那张卡可以使用PNN
        //result = MMIPHONE_HandleOperNameInd(param);
        break;

    case APP_MN_SIM_RECORD_NUM_CNF:
        // 该消息用于汇报SIM卡中PNN,OPL PNNI等总数
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_SIM_RECORD_NUM_CNF");
        result = MMIPHONE_HandleSIMRecordNumCnf(param);
        break;
    
    case APP_MN_SIM_RECORD_READ_CNF:
        // 该消息用于汇报读取SIM卡中PNN,OPL PNNI等具体某条的内容
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_SIM_RECORD_READ_CNF");
        result = MMIPHONE_HandleSIMRecordReadCnf(param);
        break;
    
    case APP_MN_SIM_BINARY_READ_CNF:
        // 该消息用于汇报读取SIM卡中SPNI具体某条的内容
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_SIM_BINARY_READ_CNF");
        //get SPNI cnf
        result = MMIPHONE_HandleSIMBinaryReadCnf(param);
        break;
#endif
            
    case APP_MN_CPHS_READY_IND:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_CPHS_READY_IND");
#ifdef MMI_CPHS_SUPPORT
		if(MMIAPIPHONE_IsCPHSSupport())
		{
			MMIAPISET_HandleCPHSReadyInd(param);
		}
#endif
#ifdef MMI_NETWORK_NAME_SUPPORT
        // 该消息用于在SIM卡初始化完成时，上报当前那张卡可以使用ONS
        result = MMIPHONE_HandleCPHSReadyInd(param);
#endif
        break;

#ifdef MMI_CSP_SUPPORT
    case APP_MN_LINE_LOCKSTATUS_UPDATE_IND: 
	if(MMIAPIPHONE_IsCPHSSupport())
	{
       	 MMIAPISET_HandleLineLockStatusUpdateInd(param);
	}
        break;

    case APP_MN_LINE_SELECTED_UPDATE_IND:
	if(MMIAPIPHONE_IsCPHSSupport())
	{
       	MMIAPISET_HandleLineSelectedUpdateInd(param);
	}
        break;
#endif
    case APP_MN_FDN_SERVICE_IND://mmi fdn service ind
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_FDN_SERVICE_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2565_112_2_18_2_45_4_30,(uint8*)"");
        result = MMI_RESULT_FALSE;
        break;
        
    case APP_MN_PS_READY_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PS_READY_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2570_112_2_18_2_45_4_31,(uint8*)"");
        e_dual_sys = ((APP_MN_PS_READY_IND_T *)param)->dual_sys;
        MMIAPISET_GetCallLineInfo(e_dual_sys);
        MMIAPIPHONE_SetIsPsReady(e_dual_sys, TRUE);
        break;
        
    case APP_MN_SIM_NOT_READY_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_NOT_READY_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2577_112_2_18_2_45_4_32,(uint8*)"");
        // 该消息用于汇报SIM卡未能成功初始化或是SIMCARD被拔出。从该消息中可获得具体的原因，
        // 并显示在界面上。
        HandleSimNotReadyInd(param);
       
        result = MMI_RESULT_FALSE;
        break;
        
    case APP_MN_SCELL_RSSI_IND:
        //SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_SCELL_RSSI_IND");
        // 该消息用于在服务小区信号强度改变时，汇报当前服务小区信号强度。
        // 例：在idle界面显示。
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SCELL_RSSI_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2589_112_2_18_2_45_5_33,(uint8*)"");
        result = HandleScellRssiInd(param);
        break;
        
    case APP_MN_PS_POWER_ON_CNF:
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_PS_POWER_ON_CNF,dual_sys=%d",e_dual_sys);
#else
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PS_POWER_ON_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2594_112_2_18_2_45_5_34,(uint8*)"");
#endif
        e_dual_sys = ((APP_MN_PS_POWER_ON_CNF_T *)param)->dual_sys;  
        MMIAPISET_GetCallLineInfo(e_dual_sys);
        MMIAPIPHONE_SetIsPsReady(e_dual_sys, TRUE);
        MMIAPICOM_StatusAreaUpdateSignalState();
        break;
        
    case APP_MN_PS_POWER_OFF_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PS_POWER_OFF_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2601_112_2_18_2_45_5_35,(uint8*)"");
        HandlePowerPsOffCnf(param);
        break;
        
    case APP_MN_DEACTIVE_PS_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_DEACTIVE_PS_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2606_112_2_18_2_45_5_36,(uint8*)"");
        HandleDeactivePsCnf(param);
        break;
        
    case APP_MN_SIM_POWER_ON_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_POWER_ON_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2611_112_2_18_2_45_5_37,(uint8*)"");
#ifdef MMI_SIM_LOCK_SUPPORT
        if(MMK_IsOpenWin(PHONE_SIM_LOCK_WAIT_WIN_ID))
        {
            MMK_CloseWin(PHONE_SIM_LOCK_WAIT_WIN_ID);
        }
#endif
        HandlePowerOnSimCnf(param);
        break;

    case APP_MN_SIM_POWER_OFF_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_POWER_OFF_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2622_112_2_18_2_45_5_38,(uint8*)"");
        HandlePowerOffSimCnf(param);
        break;

#ifdef MMI_PREFER_PLMN_SUPPORT
    //更新优选网络的列表信息
    case APP_MN_PREFER_PLMN_CHANGE_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PREFER_PLMN_CHANGE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2629_112_2_18_2_45_5_39,(uint8*)"");
        HandlePreferPlmnChangeCnf(param);
        break;
#else
	case APP_MN_PREFER_PLMN_CHANGE_CNF:
		//SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PREFER_PLMN_CHANGE_CNF"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2629_112_2_18_2_45_5_39,(uint8*)"");
		MMIAPIENG_HandlePlmnCnf(param);
		break;
#endif
#ifdef MMI_VOLTE_SUPPORT
    case APP_MN_IMS_STATE_IND:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_IMS_STATE_IND");
        result = HandleImsStatusInd(param);
        break;

#endif
#ifdef MMI_2G_4G_SUPPORT
    case APP_MN_CHANGE_RAT_CNF:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_CHANGE_RAT_CNF");
        result = HandleRatChangeCnf(param);
        break;

#endif

#ifdef AOC_SUPPORT
        //获取PUCT计费标准信号相应处理
    case APP_MN_PUCT_INFO_CNF:
        //SCI_TRACE_LOW:"HandlePuctInfoCnf: receive APP_MN_PUCT_INFO_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2637_112_2_18_2_45_5_40,(uint8*)"");
        HandlePuctInfoCnf(param);
        break;
        
    case APP_MN_AOC_MODIFIED_IND:
        //SCI_TRACE_LOW:"None Handle: receive APP_MN_AOC_MODIFIED_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2642_112_2_18_2_45_5_41,(uint8*)"");
        break;
#endif
#if defined(MMI_SYNCTOOL_SUPPORT) || defined(MMI_SYNCTOOL_V3)//CR159271
	case ATC_AUTO_TEST_CMD_IND:
		SCI_TRACE_LOW("[atc] ATC_AUTO_TEST_CMD_IND !");
		ATC_ExtCmd(param);
		break;
#endif
#ifdef MMI_DUAL_BATTERY_SUPPORT
    case DBAT_MSG_UPDATE_STATE_IND:
    case DBAT_MSG_BAT_PLUGIN_IND:
    case DBAT_MSG_BAT_PLUGOUT_IND:
    case DBAT_MSG_SWITCH_BAT_IND://目前不处理
    case DBAT_MSG_CHARGE_START_IND:
    case DBAT_MSG_WARNING_IND:
    case DBAT_MSG_SHUTDOWN_IND://和单电池处理一致
    case DBAT_MSG_CHARGE_FINISH:
    case DBAT_MSG_CHARGE_DISCONNECT:
    case DBAT_MSG_CHARGE_FAULT:
        result = MMIPHONE_ChargeHandlePsAndRefMsg(app_ptr,msg_id,param);
        break;
#else
    case CHR_MSG_CAP_IND:
    case CHR_MSG_CHARGE_START_IND:
    case CHR_MSG_CHARGE_END_IND:
    case CHR_MSG_WARNING_IND:
    case CHR_MSG_SHUTDOWN_IND:
    case CHR_MSG_CHARGE_FINISH_IND:
    case CHR_MSG_CHARGE_DISCONNECT_IND:
    case CHR_MSG_CHARGE_OVER_TEMPTURE_IND:
    case CHR_MSG_CHARGE_FAULT_IND:
        result = MMIPHONE_ChargeHandlePsAndRefMsg(app_ptr,msg_id,param);
        break;
#endif     
    case MISC_MSG_RTC_ALARM:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive MISC_MSG_RTC_ALARM"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2671_112_2_18_2_45_5_42,(uint8*)"");
        // 该消息用于汇报闹钟所设定的时间到，开始闹铃。
        // 注：设备关机时，启动闹铃的方式与该消息无关
        // MMIAPIALM_StartAlarm(TRUE);
        MMIALMSVC_StartService();
        break;
	case MSG_SET_DEL_ALARMSET_CNF:
		//delete one alarm event finished confirm
		MMIALM_DeleteCnf();
		break;        
    case USB_PLUGIN_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_PLUGIN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2682_112_2_18_2_45_5_43,(uint8*)"");
        // 该消息用于汇报U盘插入。
        result = MMIAPIUdisk_HandleUsbCablePlugIn();
        //for bug502113
        MMIAPICOM_StatusAreaSetUSBConnectedState(TRUE);
        break;
        
    case USB_PLUGOUT_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_PLUGOUT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2688_112_2_18_2_45_5_44,(uint8*)"");
        // 该消息用于汇报U盘被拔出。
        //   MMIAPIUdisk_SetIsUse(FALSE);
        result = MMIAPIUdisk_HandleUsbCablePlugOut();
        MMIAPIPHONE_CleanEventDelaied(MMI_USB_DELAY);
        //for bug502113
        MMIAPICOM_StatusAreaSetUSBConnectedState(FALSE);
	 SIO_PPP_UsbPlugOut();
		
        //MMIAPIUdisk_SetUSBDelay(FALSE);
        break;
	case USB_SERVICE_UDISK_STARTED_IND:
		//usb service started
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_UDISK);
		break;
	case USB_SERVICE_UDISK_STOPPED_IND:
		//usb service stopped
        MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_UDISK);
		break;
	case USB_SERVICE_UCOM_STARTED_IND:
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_UCOM);
		break;
	case USB_SERVICE_UCOM_STOPPED_IND:
		MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_UCOM);
		break;
	case USB_SERVICE_UPCC_STARTED_IND:
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_UPCC);
		break;
	case USB_SERVICE_UPCC_STOPPED_IND:
		MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_UPCC);
		break;
	case USB_SERVICE_LOG_STARTED_IND:
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_LOG);
		break;
	case USB_SERVICE_LOG_STOPPED_IND:
		MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_LOG);
		break;
#ifdef UCOM_UMS_SUPPORT

	case USB_SERVICE_UCOM_UMS_STARTED_IND:
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_UMS);
        break;
    case USB_SERVICE_UCOM_UMS_STOPPED_IND:
		MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_UMS);
		break;
#endif
    case USB_READY_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_READY_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2722_112_2_18_2_45_5_45,(uint8*)"");
        // 该消息用于汇报U盘已经就绪
        //USB要在菜单中开启，所以处理也在开启处
    //    MMIAPIUdisk_SetUPMIsReady(TRUE);
        break;
        
    case USB_ERROR_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_ERROR_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2729_112_2_18_2_45_5_46,(uint8*)"");
        // 该消息用于汇报U盘操作失败
        //result = MMIUDISK_Handlefail();
        break;

    case APP_AUDIO_END_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_AUDIO_END_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2735_112_2_18_2_45_5_47,(uint8*)"");
        MMISRVAUD_HandleCallBackMsg((void *)param);
        break;
      
#ifdef WRE_SUPPORT
    case APP_WRE_AUDIO_END_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_WRE_AUDIO_END_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2741_112_2_18_2_45_5_48,(uint8*)"");
        MMIWRE_HandleCallBackMsg((void *)param);
        break;
#endif

#ifdef ASP_SUPPORT
    case APP_ASP_PLAY_AUDIO_END_IND:
        MMIAPIASP_HandlePlayAudioEnd();
        break;
#endif

#ifdef JAVA_SUPPORT_SUN
    case APP_JAVA_MP4_DISPLAY_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg(),receive APP_JAVA_MP4_DISPLAY_IND!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2759_112_2_18_2_45_5_49,(uint8*)"");
        MMIAPIJAVA_CallbackFrameEndNotifyProc();
        break;

    case  APP_JAVA_MP4_END_IND:
         //SCI_TRACE_LOW:"HandlePsAndRefMsg(),receive APP_JAVA_MP4_END_IND!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2764_112_2_18_2_45_5_50,(uint8*)"");
         MMIAPIJAVA_CallbackEndNotifyProc();
         break;
    case APP_JAVA_SNAPSHOT_END_IND:
         //SCI_TRACE_LOW:"HandlePsAndRefMsg(),receive APP_JAVA_SNAPSHOT_END_IND!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2768_112_2_18_2_45_5_51,(uint8*)"");
         MMIAPIJAVA_CallbackSnapshotEndNotifyProc();
         break;
    case APP_JAVA_END_IND:
    case APP_JAVA_LAUNCH_INPUT_METHOD_IND:
    case APP_JAVA_LAUNCH_FILE_BROWSER_IND:
    case APP_JAVA_LAUNCH_WAP_BROWSER_IND:
    case APP_JAVA_START_AMS_IND:
    case APP_JAVA_STATUSBAR_OPERATION_IND:
    case APP_JAVA_SCREEN_REFLUSH_IND:
    case APP_JAVA_LAUNCH_PRELOAD_MIDLET_IND:
    case APP_JAVA_LAUNCH_VM_EXIT_WIN_IND:
    case APP_JAVA_BACKGROUND_TIMER_EVENT:
    case APP_JAVA_PDP_ACTIVE:
    case APP_JAVA_PDP_DEACTIVE:
    case APP_JAVA_VIDEO:
        //SCI_TRACE_LOW("HandlePsAndRefMsg:receive Java related message");
        MMIAPIJAVA_DispatchVmMessage(param);
        break;
#endif
#ifdef JAVA_SUPPORT_IA
    case MSG_OPEN_TEXTEDIT_WIN:
        //SCI_TRACE_LOW:"apple phone param = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2790_112_2_18_2_45_5_52,(uint8*)"d",param);
        MMIAPIJAVA_OpenTextEditWindow(PNULL);
        break;
    case MSG_JAVA_OUT_OF_MEMORY:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;
            //SCI_TRACE_LOW:"HandlePsAndRefMsg msg_id = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2794_112_2_18_2_45_5_53,(uint8*)"d", msg_id);
            MMIAPIJAVA_jwe_vmErrorHandle(common_data.data);
        }        
        break;
    case MSG_JAVA_CLOSE_APP_WIN:
        {
            APP_MN_HANDLE_DATA_T common_data = *(APP_MN_HANDLE_DATA_T*)param;
            //SCI_TRACE_LOW:"HandlePsAndRefMsg msg_id = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2794_112_2_18_2_45_5_53,(uint8*)"d", msg_id);
            MMIAPIJAVA_jwe_vmErrorHandle(common_data.handle_data);
            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_CLOSE_APP_WIN param = %d, %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2798_112_2_18_2_45_5_54,(uint8*)"dd",param,*(MMI_HANDLE_T *)param);
		    if(MMK_IsOpenWin(common_data.handle_data))
	        {
    	        MMK_CloseWin(common_data.handle_data);
	        }
        }        
        break;
    case MSG_JAVA_AUTORUNAPP:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_AUTORUNAPP param = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2805_112_2_18_2_45_5_55,(uint8*)"d",param);
	        MMIAPIJAVA_AutoRunApp(common_data.data);	
        }        
        break;
    case  MSG_JAVA_SEND_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_SEND_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2809_112_2_18_2_45_5_56,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StartSendTimer(common_data.data);
        }        
        break;
    case MSG_JAVA_LINK_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_LINK_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2813_112_2_18_2_45_5_57,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StartLinkTimer(common_data.data);
        }        
        break;
    case MSG_JAVA_CONNECT_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_CONNECT_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2817_112_2_18_2_45_5_58,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StartConnectTimer(common_data.data);
        }
        
        break;
    case MSG_JAVA_RECEIVE_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_RECEIVE_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2821_112_2_18_2_45_5_59,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StartRecvTimer(common_data.data);
        }
        break;
    case MSG_JAVA_STOP_SOCKET_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_STOP_SOCKET_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2825_112_2_18_2_45_5_60,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StopSockTimer(common_data.data);
        }
        break;
    case MSG_JAVA_UDP_LINK:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            MMIAPIJAVA_StartUDPLink(common_data.data);
        }        
        break;
    case MSG_JAVA_PAINT:
        MMIAPIJAVA_CreateWinPaintTimer();
        break;
    case MSG_JAVA_VM_END_IND:
        MMIAPIJAVA_Exit_FinalizeResource();
        //if(MMK_IsOpenWin(*(MMI_HANDLE_T *)param))
        //	MMK_CloseWin(*(MMI_HANDLE_T *)param);
        break;
    case MSG_JAVA_VM_SUSPEND_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_VM_SUSPENDED_IND param = %d %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2840_112_2_18_2_45_5_61,(uint8*)"dd", param,*(int32 *)param);
	    MMIAPIJAVA_StartCheckVmState(MSG_JAVA_VM_SUSPEND_IND,param);	 
        break;
    case MSG_JAVA_MEDIA_STOPTIME_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_MEDIA_STOPTIME_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2844_112_2_18_2_45_5_62,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StartMediaStopTimer(common_data.data);
        }        
        break;
     case MSG_JAVA_DEACTIVE_PDP:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_DEACTIVE_PDP param = %d %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2848_112_2_18_2_45_5_63,(uint8*)"dd", param,*(int32 *)param);
        MMIAPIJAVA_PDPDeactive();
        break;
     case MSG_JAVA_PLATFORM_REQUEST:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;
	
            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_PLATFORM_REQUEST param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2852_112_2_18_2_45_5_64,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_PlatformRequest(*(uint32 *)common_data.data);
        }        
        break;
     case MSG_JAVA_AUDIO_PLAY_AGAIN:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_AUDIO_PLAY_AGAIN param = %d %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2856_112_2_18_2_45_5_65,(uint8*)"dd", param,*(int32 *)param);
        MMIAPIJAVA_AudioClientPlayAgain();
        break;
#endif
     case MSG_FS_DELETE_CNF:
         {
			 MMIFILE_ASYN_CALLBACK_T  *callback_ptr = (MMIFILE_ASYN_CALLBACK_T*) param;
			 BOOLEAN                  fs_result = FALSE;

			 if(SFS_ERROR_NONE == callback_ptr->error)
			 {
				 fs_result = TRUE;
			 }
			 MMK_PostMsg(callback_ptr->win_id, callback_ptr->msg_id, &fs_result, sizeof(fs_result));
			 MMIAPICOM_TriggerMMITask();
         }
		 break;

#ifdef DRM_SUPPORT
    case DRM_SIG_DH_INSTALL_FILE_REQ:
    case DRM_SIG_DH_INSTALL_FILE_CNF:
    case DRM_SIG_GET_FILE_MIME_CNF:
	case DRM_SIG_CANCEL_INSTALL_FILE_CNF:
    case DRM_SIG_DH_DELETE_RIGHTS_FILE_CNF:

        MMIDRM_HandleDrmControlMsg(msg_id, param);
        break;
#endif

#ifdef IKEYBACKUP_SUPPORT
    case MSG_SMS_IKB_BACKUP_ITEM_DATA_REQ:
        {
            void *original_param = PNULL;

            original_param = (void*)((uint32)param);
            MMIAPISMS_IKeyBackup(original_param);
        }
        
        break;
    case MSG_SMS_IKB_RESTORE_ITEM_DATA_REQ:
        {
            void *original_param = PNULL;

            original_param = (void*)((uint32)param);
            MMIAPISMS_IKeyRestore(original_param);
        }
        break;
    case MSG_IKB_LOAD_ALL_NODE_RES:
    case MSG_IKB_BACKUP_DATA_RES:
    case MSG_IKB_BACKUP_ITEM_DATA_REQ:
    case MSG_IKB_RESTORE_DATA_RES:
    case MSG_IKB_RESTORE_ITEM_DATA_REQ:
    case MSG_IKB_GET_ALL_PACK_DATA_CNF:
    case MSG_IKB_PROGRESS_VALUE_RES:
    case MSG_IKB_CANCEL_ACTION_RES:
        MMIAPIIKB_HandleAppSigRes(msg_id, param);
        break;
#endif

#ifdef MMI_VCALENDAR_SUPPORT
    case MSG_VCALENDAR_SAVE_IND:
        MMIVCAL_HandleWriteCNF(MSG_VCALENDAR_SAVE_SCH_CNF,PNULL);
        break;
    case   MSG_SCH_GET_ALL_FILE_FINISH:
        MMK_SendMsg(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID, MSG_SCH_GET_ALL_FILE_FINISH,PNULL);
        break;
    case  MSG_SCH_LOAD_NEXT_VCALENDAR:
        MMK_SendMsg(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID, MSG_SCH_LOAD_NEXT_VCALENDAR,PNULL);
        break;
#ifdef CALENDAR_SUPPORT
    case MSG_SCH_DELETE_IND:
        MMIAPISCH_DelSchNotify();
        break;
#endif
#endif
#ifdef FLASH_SUPPORT
    case MSG_FLASH_UPDATE_WINDOW:
		//SCI_TRACE_LOW:"flash_printf:@@@@@MSG_FLASH_UPDATE_WINDOW  0  ,received the message."
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2931_112_2_18_2_45_5_66,(uint8*)"");
        if(MMIAPIFlash_FlashIsVisiable() &&  MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
        {
			//SCI_TRACE_LOW:"flash_printf:@@@@@MSG_FLASH_UPDATE_WINDOW  1  ,received the message."
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2934_112_2_18_2_45_5_67,(uint8*)"");
            MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_FULL_PAINT, 0, 0);
        }
        break;
#endif
#ifdef MMI_SNTP_SUPPORT
    case SNTP_SIG_GET_TIME_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive SNTP_SIG_GET_TIME_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2941_112_2_18_2_45_5_68,(uint8*)"");
        MMIAPIAUT_ManualUpdateTimeHandleSNTPcnf(msg_id, param);
    break;
#endif
#ifdef CAMERAROLL_SUPPORT	
	case  MSG_CAMERA_ROLL_UPLOAD_PHOTO_NOTIFY :
	case MSG_CAMERA_ROLL_UPLOAD_PHOTO_CNF :	
	case  MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_NOTIFY :
	case MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_CNF:	
case  MSG_CAMERA_ROLL_DOWNLOAD_PHOTO_NOTIFY:
	case MSG_CAMERA_ROLL_DOWNLOAD_PHOTO_CNF:	
case  MSG_CAMERA_ROLL_INIT_CNF :
	case MSG_CAMERA_ROLL_ERROR_NOTIFY:	
	case MSG_CAMERA_ROLL_CANCEL_CNF:
        case MSG_CAMERA_ROLL_DELETE_PHOTO_NOTIFY:
	case MSG_CAMERA_ROLL_SHARE_LINK_CNF:		
	case MSG_CAMERA_ROLL_SIGNOFF_CNF :
	case MSG_CAMERA_ROLL_EMAIL_INVITE_CNF:
		result = MMIAPIPICVIEW_HandleCameraRollSig(msg_id, param);
		break;
#endif
    //zhuyongjing add for gallery
#ifdef PIC_VIEWER_SUPPORT
        case MSG_PICVIEWER_MUTIL_UPLOAD_FINISH:
        case MSG_PICVIEWER_DISCONNECT_DELETE_FINISH:
        case MSG_PICVIEWER_MARKABLE_DELETE_FINISH:
        case MSG_PICVIEWER_GOTO_SUBFOLDER_FINISH:
        case MSG_PICVIEWER_GET_THUMBNAILS_FINISH:
        case MSG_PICVIEWER_SERVER_DELETE_FINISH:
        result = MMIAPIPICVIEW_HandleGalleryTaskSig(msg_id, param);
		break;
#endif
#ifdef MSA_SUPPORT			
		case MSG_MS_ACCOUNT_REFRESH_TOKEN_REQ:
			MMIAPIMSA_Exec_token_refresh_req();
			break;
#endif	
    default:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:msg_id = 0x%04x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2946_112_2_18_2_45_5_69,(uint8*)"d", msg_id);
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : Interrogate Cfu for idle icon display
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_InterrogateCfuForIdle(MN_DUAL_SYS_E dual_sys)
{
    uint32 i = 0;
    BOOLEAN is_allow_do_cfu = TRUE;
    MN_DUAL_SYS_E cfu_dual_sys = MN_DUAL_SYS_MAX;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMISET_DAULSYS_ONLY_SIM1_ON;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if ((e_dualsys_setting & (0x01 << i)) && MMIAPIPHONE_GetSimExistedStatus(i)
            && !s_is_network_status_ready[i])
        {
            is_allow_do_cfu = FALSE;

            break;
        }
    }
    
    if (is_allow_do_cfu && !MMIAPISET_GetIsQueryCfu())
    {
        if (!MMIAPISET_GetCfuInfo(dual_sys)
            &&((PLMN_NORMAL_GSM_ONLY == g_service_status[dual_sys].plmn_status)
            || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[dual_sys].plmn_status)))
        {
            cfu_dual_sys = dual_sys;
        }

        if (MN_DUAL_SYS_MAX == cfu_dual_sys)
        {
            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (!MMIAPISET_GetCfuInfo(i)
                    &&((PLMN_NORMAL_GSM_ONLY == g_service_status[i].plmn_status)
                    || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[i].plmn_status)))
                {
                    cfu_dual_sys = (MN_DUAL_SYS_E)i;

                    break;
                }
            }
        }
        
        //SCI_TRACE_LOW:"MMIAPIPHONE_InterrogateCfuForIdle dual_sys=%d,cfu_dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2992_112_2_18_2_45_5_70,(uint8*)"dd", dual_sys, cfu_dual_sys);
        
        if (MN_DUAL_SYS_MAX != cfu_dual_sys)
        {
            if(MMIAPIENG_GetCallForwardQuery())
            {
                //从网络中查询无条件转移状态,只需查询线路1是否设置了CFU，因为线路2无法设置CFU
                MMIAPISET_InterrogateCfuStatusFromPs(cfu_dual_sys, 0);
            }
            else
            {
                if (!MMIAPISET_GetIsQueryCfu())
                {
                    MMIAPISET_SetIsQueryCfu(TRUE,dual_sys);
                }
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_InterrogateCfuForIdle is_allow_do_cfu=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3012_112_2_18_2_45_6_71,(uint8*)"d", is_allow_do_cfu);
    }
}

#if defined(MMI_SMSCB_SUPPORT)
/*****************************************************************************/
// 	Description : get  s_is_startup_smscb flag
//	Global resource dependence : 
//  Author: Tong Yongli
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsStartupSmscbFlag(MN_DUAL_SYS_E dual_sys)
{
    return s_is_startup_smscb[dual_sys];
}

PUBLIC void MMIAPIPHONE_DisplayNetworkInfo(void)
{
    uint16 i = 0;
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    MMI_STRING_T network_str = {0};
    SCI_TRACE_LOW("[PLMN]:----------Begin----------");
    network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;

    for (i = 0; i < MAX_NETWORK_NUM; i++)
    {
        wchar ucs2buf[MMICONNECTION_MAX_USERNAME_LEN+1] = {0};
        MMI_GetLabelTextByLang(network_table_ptr[i].name_index, &network_str);
        MMIAPICOM_WstrToStr(network_str.wstr_ptr, ucs2buf);
        SCI_TRACE_LOW("[PLMN]: mcc = %d, mnc = %d, Operator Name: %s", network_table_ptr[i].mcc, network_table_ptr[i].mnc, ucs2buf);
        SCI_Sleep(1);
    }
    SCI_TRACE_LOW("[PLMN]:----------End----------");
    
    return;
}

/*****************************************************************************/
// 	Description : startup sms cb
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StartupSmsCB(MN_DUAL_SYS_E dual_sys)
{
    SCI_TRACE_LOW("StartupSmsCB:s_is_startup_smscb[dual_sys]%d",s_is_startup_smscb[dual_sys]);
    if (!s_is_startup_smscb[dual_sys])
    {
        s_is_startup_smscb[dual_sys] = TRUE;

        MMIAPISMSCB_Init(dual_sys);
    }
}

/*****************************************************************************/
// 	Description : ClearCbEnable
//	Global resource dependence : 
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
LOCAL void ClearCbEnable(MN_DUAL_SYS_E dual_sys)
{
    s_is_startup_smscb[dual_sys] = FALSE;
}
#endif

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_NETWORK_STATUS_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetworkStatusInd(
                                          DPARAM              param
                                          )
{
    APP_MN_NETWORK_STATUS_IND_T network_status_ind = *((APP_MN_NETWORK_STATUS_IND_T *)param);
    MMICONNECTION_PNNAPN_EXIST_T * pnn_apn_exist = MMIAPICONNECTION_GetPNNAPNExist(network_status_ind.dual_sys);
    MN_PLMN_T carrier_hplmn[MMI_DUAL_SYS_MAX] = {0};
	MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;    
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMISET_DAULSYS_ONLY_SIM1_ON;
    uint16 sim_ok = MN_DUAL_SYS_1;
    BOOLEAN bIsFound = FALSE;    

    SCI_TRACE_LOW("mmiphone.c HandleNetworkStatusInd:s_is_autoadapt_flag[%d]= %d", network_status_ind.dual_sys, s_is_autoadapt_flag[network_status_ind.dual_sys]);
    if (PLMN_FULL_PS_SERVICE == network_status_ind.plmn_status)
    {
        network_status_ind.plmn_status = PLMN_NORMAL_GPRS_ONLY;
    }
#ifdef WRE_SUPPORT
	{
		MMIWRE_NetworkIndHandle(param);
	}
#endif
    
#ifdef MMI_MULTI_SIM_SYS_SINGLE    
    network_status_ind.dual_sys = MN_DUAL_SYS_1;
#endif

#ifdef MMI_NETWORK_NAME_SUPPORT
#ifdef WIN32
    if(0 == network_status_ind.lac)
    {
         network_status_ind.lac = 0x1001;// set lac temp
    }
#endif
    if(0 != network_status_ind.lac)
    {
        MN_PLMN_T  cur_plmn = {0};

        cur_plmn.mcc  = network_status_ind.mcc;
        cur_plmn.mnc  = network_status_ind.mnc;
        cur_plmn.mnc_digit_num = network_status_ind.mnc_digit_num;
        //set  PNN str when lac changes
        MMIPHONE_SetPNNWithLac(network_status_ind.dual_sys, cur_plmn, network_status_ind.lac, network_status_ind.plmn_is_roaming);
        if (pnn_apn_exist->bIsPNNAPNExist)
        {
            MMIAPICONNECTION_SetPNNAPNExist(network_status_ind.dual_sys, FALSE, FALSE);
            MMINV_READ(MMINV_CARRIER_PLMN, carrier_hplmn, return_value);
        
            if (return_value != MN_RETURN_SUCCESS)
            {
                SCI_MEMSET(carrier_hplmn, 0x00, sizeof(carrier_hplmn));
            }
            else
            {
                carrier_hplmn[network_status_ind.dual_sys].mcc = 0x0;
                carrier_hplmn[network_status_ind.dual_sys].mnc = 0x0;
            }
            MMINV_WRITE(MMINV_CARRIER_PLMN, carrier_hplmn);
            SCI_TRACE_LOW("[szp]ReAutoAdapting. PNN");
            MMIAPICONNECTION_AutoAdapting(network_status_ind.dual_sys);
        }
    }
#endif

	//更新设置界面窗口
//#if !defined(MMI_MULTI_SIM_SYS_SINGLE) //for Austrtua operator
    if ((network_status_ind.plmn_is_selecting != s_is_network_status_ready[network_status_ind.dual_sys])
        ||(network_status_ind.plmn_status != g_service_status[network_status_ind.dual_sys].plmn_status)
        )
    {
        MMIAPIPHONE_UpdateNetworkStatusWin();
    }
//#endif   
    
    if (!network_status_ind.plmn_is_selecting)
    {
        s_is_network_status_ready[network_status_ind.dual_sys] = TRUE;
    }
    
    e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    if ((PLMN_NORMAL_GSM_ONLY != g_service_status[network_status_ind.dual_sys].plmn_status)
        && (PLMN_NORMAL_GPRS_ONLY != g_service_status[network_status_ind.dual_sys].plmn_status)
        && (PLMN_NORMAL_GSM_GPRS_BOTH != g_service_status[network_status_ind.dual_sys].plmn_status)
        && ((PLMN_NORMAL_GSM_ONLY == network_status_ind.plmn_status)
            || (PLMN_NORMAL_GPRS_ONLY == network_status_ind.plmn_status)
            || (PLMN_NORMAL_GSM_GPRS_BOTH == network_status_ind.plmn_status)))
    {
        SCI_TRACE_LOW("===== HandleNetworkStatusInd BQB MMIAPIBT_Service_Indicator TRUE");
        MMIAPIBT_Service_Indicator(TRUE);
    }
    else if (((PLMN_NORMAL_GSM_ONLY == g_service_status[network_status_ind.dual_sys].plmn_status)
               || (PLMN_NORMAL_GPRS_ONLY == g_service_status[network_status_ind.dual_sys].plmn_status)
               || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[network_status_ind.dual_sys].plmn_status))
             && ((PLMN_NORMAL_GSM_ONLY != network_status_ind.plmn_status)
                 && (PLMN_NORMAL_GPRS_ONLY != network_status_ind.plmn_status)
                 && (PLMN_NORMAL_GSM_GPRS_BOTH != network_status_ind.plmn_status)))
    {
        MMIAPIBT_Service_Indicator(FALSE);
    }
#endif
    
    // 保存网络状态信息
    StoreNetworkStatus(&network_status_ind);
    bIsFound = MMIAPIPHONE_GetDataServiceSIM(&sim_ok);
    if ((!MNSS_CheckTestSim(network_status_ind.dual_sys))
         && (MMIAPIAUT_GetNeedUpdateTimeEnable())
         && (TRUE == bIsFound)
         && (MMIAPIAUT_GetIsNeedUpdateTimeByNet())
         && (MMIAPIAUT_GetIsFirstQuerySNTPFlag())
         && ((PLMN_NORMAL_GPRS_ONLY == network_status_ind.plmn_status) || (PLMN_NORMAL_GSM_GPRS_BOTH == network_status_ind.plmn_status)))
    {
        MMIAPIAUT_SetIsFirstQuerySNTPFlag(FALSE);
        MMIAPIPHONE_GetNitzTime(sim_ok);
    }
    if((MMICONNECTION_SETTING_DATA_ROAMING_DISABLE != MMAPIICONNECTION_Setting_GetDataRoamingStatus()) &&
        (MMICONNECTION_SETTING_DATA_ROAMING_ENABLE != MMAPIICONNECTION_Setting_GetDataRoamingStatus()))//bug714797, judge the setting status
    {
        MMAPIICONNECTION_Setting_SetDataRoamingDefaultStatus();
    }
    
    #if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_SetNetWorkRoamStatus(MMIAPIPHONE_GetIsNetworkRoaming());
    MMIAPIBT_SetOperatorName();
    #endif

#if defined(MMI_DUALMODE_ENABLE) ||defined (MMI_2G_4G_SUPPORT)
    MMIAPIPHONE_TDOrGsm(network_status_ind.rat,network_status_ind.dual_sys);    
#endif

    MMIAPIPHONE_SetIdleGprsState(network_status_ind.dual_sys);
    NotifyPlmnStatusChange(network_status_ind.dual_sys, network_status_ind.plmn_status);
    
    //set idle string
    MMIAPICOM_StatusAreaSetNetWorkName();    // MAIN_SetIdleNetWorkName();

    MMIAPICOM_StatusAreaSetRoamingState(network_status_ind.dual_sys, network_status_ind.plmn_is_roaming);
#ifdef MMI_VOLTE_SUPPORT	
    MMIAPICOM_StatusAreaSetVolteState(network_status_ind.dual_sys, network_status_ind.rat);
#endif
#if defined(MMI_CSP_SUPPORT)
    if(MMISET_IsAlsValid(network_status_ind.dual_sys))
       
    {
        MAIN_UpdateLineState();  //重新刷新line state的显示
    }
#endif //MMI_CSP_SUPPORT
   SCI_TRACE_LOW("enter ljg*HandleNetworkStatusInd==MMIAPIENG_GetCallForwardQuery=%d,dual_sys=%d",MMIAPIENG_GetCallForwardQuery(),network_status_ind.dual_sys);
   if (MNSS_CheckTestSim(network_status_ind.dual_sys))
   {
      SCI_TraceLow("MNSS_CheckTestSim : test sim");
   }
   else
   {
      MMIAPISET_InterrogateCfuStatusFromPs(network_status_ind.dual_sys,0);
   }
/*
    if(MMIAPIENG_GetCallForwardQuery())
    {
        MMIPHONE_InterrogateCfuForIdle(network_status_ind.dual_sys);        //查询Cfu
    }
*/
#if defined(MMI_SMSCB_SUPPORT)
#ifdef MMI_ETWS_SUPPORT
    if( (s_cur_mcc !=  g_service_status[network_status_ind.dual_sys].plmn.mcc )
        && ( 0XFFFF != s_cur_mcc) )
    {
        MMIAPISMSCB_SetIsSpecChange(TRUE);
        MMIAPISMSCB_Init(network_status_ind.dual_sys);
    }
    else
#endif
    {
        StartupSmsCB(network_status_ind.dual_sys);
    }
#endif

#ifdef MMI_ETWS_SUPPORT
    s_cur_mcc = g_service_status[network_status_ind.dual_sys].plmn.mcc;
#endif

    SCI_TRACE_LOW("mmiphone.c HandleNetworkStatusInd:g_service_status[network_status_ind.dual_sys].plmn_status:%d",g_service_status[network_status_ind.dual_sys].plmn_status);
    if ((PLMN_NORMAL_GSM_ONLY == g_service_status[network_status_ind.dual_sys].plmn_status)
        || (PLMN_NORMAL_GPRS_ONLY == g_service_status[network_status_ind.dual_sys].plmn_status)
        || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[network_status_ind.dual_sys].plmn_status))
    {        
//#ifdef MMI_ENABLE_DCD    //for dcd internation           
//        MMIAPIDCD_SetCurMCC(g_service_status[network_status_ind.dual_sys].plmn.mcc, g_service_status[network_status_ind.dual_sys].plmn.mnc);                		    
//#endif
            
     //   MMIAPIDM_HandleNetChange(network_status_ind.dual_sys);
    }
    //if no service, the rx display 0 directly
    else if (!g_service_status[network_status_ind.dual_sys].is_slecting
        && PLMN_NO_SERVICE == g_service_status[network_status_ind.dual_sys].plmn_status)
    {
        g_service_status[network_status_ind.dual_sys].is_first_rssi = TRUE;//跳到No Service时，直接显示格数
        g_service_status[network_status_ind.dual_sys].rssiLevel = 0;

        MMIAPICOM_StatusAreaSetRxLevel(network_status_ind.dual_sys, 0, TRUE);
    }
#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_UpdateMainWin();
#endif

#if defined(MMI_UPDATE_TIME_SUPPORT) && defined(MMI_UPDATE_TIME_WHEN_STARTUP)
    //if(network_status_ind.gprs_support)
    {
        //MMIAPIPHONE_StartGetNitzTime();   //remove it for GCF Test
        //MMIAPIAUT_PdpRetryProcess(network_status_ind.dual_sys);
    }
#endif

#ifdef MMI_SHOW_MSA
    if(TRUE == bIsFound &&
       network_status_ind.dual_sys == sim_ok)
    {
	    if(NO_DATA_SERVICE_TYPE != MMIAPIPHONE_GetDataServiceType(sim_ok) 
#ifdef MMI_GPRS_SWITCH_SUPPORT
         	&&(FALSE == MMIAPICONNECTION_isGPRSSwitchedOff())
#endif         	
		)
	    {
		MMISRV_CAMERAROLL_Check_Network_status(TRUE);
		MMISRV_CAMERAROLL_Check_auto_upload_photo();
	    }
	    else
	    {
	    	MMISRV_CAMERAROLL_Check_Network_status(FALSE);
	    }
    }
#endif


    if(MMIAPICONNECTION_isGPRSSwitchedOff() &&(network_status_ind.ps_attach_state ==MN_ATTACHED_STATE) )
   {
        MMIAPICONNECTION_DetachIfGprsOff();
    }
#ifdef FLYMODE_ECC_SUPPORT
    if ((MMICC_IsFocusFlymodeEccWin()) && 
		((PLMN_NORMAL_GSM_ONLY == g_service_status[network_status_ind.dual_sys].plmn_status) || 
         (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[network_status_ind.dual_sys].plmn_status) || 
         (PLMN_EMERGENCY_ONLY == g_service_status[network_status_ind.dual_sys].plmn_status)))
    {
        MMICC_CloseFlymodeEccWin(network_status_ind.dual_sys);
    }
#endif

#ifdef SFR_SUPPORT
    SCI_TraceLow("[Self Reg] HandleNetworkStatusInd : network_status_ind.dual_sys = %d", network_status_ind.dual_sys);
    for(sim_ok = 0; sim_ok < MMI_DUAL_SYS_MAX; sim_ok++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(sim_ok) && (! MMIPHONE_IsSimOk(sim_ok)))
        {
            return (result);
        }
    }

    MMISFR_Run();
#endif
#ifdef CMCCDM_SUPPORT
    SCI_TraceLow("[Self Reg] HandleNetworkStatusInd : network_status_ind.dual_sys = %d", network_status_ind.dual_sys);
    for(sim_ok = 0; sim_ok < MMI_DUAL_SYS_MAX; sim_ok++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(sim_ok) && (! MMIPHONE_IsSimOk(sim_ok)))
        {
            return (result);
        }
    }

    MMICMCCDM_DMRun();
#endif
//zmt add start
#ifdef ZDT_HTTP_APP_SUPPORT
    MMIHTTP_Net_StatusInd(&network_status_ind);
#endif
//zmt add end
    return (result);
}

/*****************************************************************************/
// 	Description : 设置IDLE GPRS显示状态
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetIdleGprsState(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_DUALMODE_ENABLE
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_SetIdleGprsState err dual_sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3145_112_2_18_2_45_6_72,(uint8*)"d", dual_sys);
        return;
    }

    //SCI_TRACE_LOW:"[dual mode] MMIAPIPHONE_SetIdleGprsState: rat = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3149_112_2_18_2_45_6_73,(uint8*)"d", g_service_status[dual_sys].rat);

    switch(g_service_status[dual_sys].rat) 
    {
    case MMI_GMMREG_RAT_GPRS:
        MMIAPIPHONE_StopSTTDOrGsm();
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示，Gprs状态
        MMISUB_UpdateDisplaySubLcd(); 
#endif
        break;
        
    case MMI_GMMREG_RAT_3G:
        MAIN_SetIdleTdState();
        break;
        
    case MMI_GMMREG_RAT_UNKNOWN:
        MMIAPIPHONE_StopSTTDOrGsm();
        MAIN_SetIdleUnknownState();
        break;
        
    default:
        break;
    }
    if( (PLMN_NORMAL_GPRS_ONLY == g_service_status[dual_sys].plmn_status)
        || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[dual_sys].plmn_status) )
    {
#ifdef MMI_GPRS_SUPPORT
        if (MMIPDP_NOTIFY_EVENT_ACTIVED == MMIAPIPDP_GetPdpLastEvent(dual_sys)) /*@CR255469 modify 2011.08.23*/
        {
            MMIAPICOM_StatusAreaSetGprsInfo(dual_sys, MMIPDP_NOTIFY_EVENT_ACTIVED, FALSE);
        }
        else
#endif
        {
            MMIAPICOM_StatusAreaSetGprsState(dual_sys, TRUE);
        }
    }
    else
    {
#ifdef MMI_GPRS_SUPPORT
        if (MMIPDP_NOTIFY_EVENT_DEACTIVED == MMIAPIPDP_GetPdpLastEvent(dual_sys)) /*@CR255469 modify 2011.08.23*/
        {
            MMIAPICOM_StatusAreaSetGprsInfo(dual_sys, MMIPDP_NOTIFY_EVENT_DEACTIVED, FALSE);
        }
        else
#endif
        {
            MMIAPICOM_StatusAreaSetGprsState(dual_sys, FALSE);
        }
    }
#else
    //SCI_TRACE_LOW:"MMIAPIPHONE_SetIdleGprsState dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3201_112_2_18_2_45_6_74,(uint8*)"d", dual_sys);

    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_SetIdleGprsState err dual_sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3205_112_2_18_2_45_6_75,(uint8*)"d", dual_sys);
        return;
    }

#if defined(MMI_GPRS_ATTACH_ICON_SUPPORT)
    //只有当进行GPRS连接时才显示G图标
    if( (PLMN_NORMAL_GPRS_ONLY == g_service_status[dual_sys].plmn_status)
        || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[dual_sys].plmn_status))
    {
        MMIAPICOM_StatusAreaSetGprsState(dual_sys, TRUE);
    }
    else
    {
        MMIAPICOM_StatusAreaSetGprsState(dual_sys, FALSE);
    }
#else
    //只要网络支持GPRS就显示G图标
    if (MMIAPISET_GetMultiSysSettingNum(PNULL, 0) > 1)
    {
        if( (PLMN_NO_SERVICE == g_service_status[dual_sys].plmn_status)
            || (PLMN_EMERGENCY_ONLY == g_service_status[dual_sys].plmn_status) )
        {
            MMIAPICOM_StatusAreaSetGprsState(dual_sys, FALSE);
        }
        else
        {
            MMIAPICOM_StatusAreaSetGprsState(dual_sys, g_service_status[dual_sys].gprs_support);
        }
    }
    else
    {
        if( (PLMN_NORMAL_GPRS_ONLY == g_service_status[dual_sys].plmn_status)
            || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[dual_sys].plmn_status) )
        {
            MMIAPICOM_StatusAreaSetGprsState(dual_sys, TRUE);
        }
        else
        {
            MMIAPICOM_StatusAreaSetGprsState(dual_sys, FALSE);
        }
    }
#endif
#endif
}


/*****************************************************************************/
// Description : 
//  User can check the netwotk's mcc/mnc, and sim's mcc/mnc.
//  If they are not equal, it is roaming.
//  But maybe the local operator regard this as not-roaming.
//  Then the user can config in this function to resolve this case.
// Global resource dependence : 
// Author: Wuding.Yang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsRealRoaming(MN_DUAL_SYS_E dual_sys, MN_PLMN_T cur_plmn)
{
#ifdef UNISOC_ROAMING_INDICATION
    uint32 i = 0;
    SCI_TRACE_LOW("CheckIsRealRoaming cur_plmn.mcc=%d,cur_plmn.mnc=%d,sim mcc=%d,sim mnc=%d",cur_plmn.mcc,cur_plmn.mnc,s_sim_status[dual_sys].hplmn.mcc,s_sim_status[dual_sys].hplmn.mnc);
    for(i=0;i<MAX_MVNO_ROAMING_NUM;i++)
    {
        if (((s_mvno_roaming_table[i].host_mcc == cur_plmn.mcc) && (s_mvno_roaming_table[i].host_mnc == cur_plmn.mnc)) 
            && ((s_mvno_roaming_table[i].mvno_mcc == s_sim_status[dual_sys].hplmn.mcc) && (s_mvno_roaming_table[i].mvno_mnc== s_sim_status[dual_sys].hplmn.mnc)))
        {
            SCI_TRACE_LOW("CheckIsRealRoaming mvno_mcc=%d,mvno_mnc=%d,host_mcc=%d,host_mnc=%d",s_mvno_roaming_table[i].mvno_mcc,s_mvno_roaming_table[i].mvno_mnc,s_mvno_roaming_table[i].host_mcc,s_mvno_roaming_table[i].host_mnc);
            return FALSE;
        }
    }

    //China              460/0  460/1 
    if(((460 == cur_plmn.mcc) 
        && ( cur_plmn.mnc <= 7 || (9 == cur_plmn.mnc) || (11 == cur_plmn.mnc)))
        &&((460 == s_sim_status[dual_sys].hplmn.mcc) 
        && (s_sim_status[dual_sys].hplmn.mnc <= 7 || (9 == s_sim_status[dual_sys].hplmn.mnc) || (11 == s_sim_status[dual_sys].hplmn.mnc))))
    {
        SCI_TRACE_LOW("CheckIsRealRoaming not roaming");
        return FALSE;
    }
    
    //cmcc              460         00/02
    if(((460 == cur_plmn.mcc) 
     && ((0 == cur_plmn.mnc) ||(2 == cur_plmn.mnc)))
     &&((460 == s_sim_status[dual_sys].hplmn.mcc) 
     && ((0 == s_sim_status[dual_sys].hplmn.mnc) || (2 == s_sim_status[dual_sys].hplmn.mnc))))
    {
    SCI_TRACE_LOW("CheckIsRealRoaming cmcc  ");
     return FALSE;
    }
    
    //Virgin              234         30/33/38      EE                      234     30/33  UK
    if(((234 == cur_plmn.mcc) 
        && ((30 == cur_plmn.mnc) ||(33 == cur_plmn.mnc) || (38 == cur_plmn.mnc)))
        &&((234 == s_sim_status[dual_sys].hplmn.mcc) 
        && ((30 == s_sim_status[dual_sys].hplmn.mnc) || (33 == s_sim_status[dual_sys].hplmn.mnc))))
    {
        return FALSE;
    }

    //LIFE                234         33            EE                      234     30/31/32/33/34  UK
    if(((234 == cur_plmn.mcc) && ((33 == cur_plmn.mnc)))
        &&((234 == s_sim_status[dual_sys].hplmn.mcc) 
        && ((30 == s_sim_status[dual_sys].hplmn.mnc) || (31 == s_sim_status[dual_sys].hplmn.mnc) || (32 == s_sim_status[dual_sys].hplmn.mnc)
          ||(33 == s_sim_status[dual_sys].hplmn.mnc) || (34 == s_sim_status[dual_sys].hplmn.mnc))))
    {
        return FALSE;
    }

    //O2	262	11/7/8	262	3/5/12/17/20/77
    if(((262 == cur_plmn.mcc) 
        && ((7 == cur_plmn.mnc) ||(8 == cur_plmn.mnc)||(11 == cur_plmn.mnc)))
        &&((262 == s_sim_status[dual_sys].hplmn.mcc) 
        && ((3 == s_sim_status[dual_sys].hplmn.mnc) || (5 == s_sim_status[dual_sys].hplmn.mnc)
        || (12 == s_sim_status[dual_sys].hplmn.mnc) || (17 == s_sim_status[dual_sys].hplmn.mnc) 
        || (20 == s_sim_status[dual_sys].hplmn.mnc) || (77 == s_sim_status[dual_sys].hplmn.mnc))))
    {
        return FALSE;
    }

    //E-Plus	262	3/5/12/17/20/77	262	2011/7/8
    if(((262 == cur_plmn.mcc) 
         && ((3 == cur_plmn.mnc) || (5 == cur_plmn.mnc)  || (12 == cur_plmn.mnc)
         || (17 == cur_plmn.mnc) || (20 == cur_plmn.mnc) || (77 == cur_plmn.mnc)))
        &&((262 == s_sim_status[dual_sys].hplmn.mcc) 
        && ((7 == s_sim_status[dual_sys].hplmn.mnc) || (8 == s_sim_status[dual_sys].hplmn.mnc))))
    {
        return FALSE;
    }
    
    //Netherland
    if(((204 == cur_plmn.mcc) 
        && ((2 == cur_plmn.mnc)||(9 == cur_plmn.mnc)||(12 == cur_plmn.mnc)||(15 == cur_plmn.mnc)
        ||(16 == cur_plmn.mnc)||(17 == cur_plmn.mnc)||(18 == cur_plmn.mnc)||(20 == cur_plmn.mnc)))
        &&((204 == s_sim_status[dual_sys].hplmn.mcc) 
        && ((4 == s_sim_status[dual_sys].hplmn.mnc) || (8 == s_sim_status[dual_sys].hplmn.mnc) || (16 == s_sim_status[dual_sys].hplmn.mnc))))
    {
       SCI_TRACE_LOW("CheckIsRealRoaming Netherland.");
        return FALSE;
    }
    
    //Belgium
    if(((206 == cur_plmn.mcc) 
        && ((1 == cur_plmn.mnc)||(5 == cur_plmn.mnc)))
        &&((206 == s_sim_status[dual_sys].hplmn.mcc) 
        && ((1 == s_sim_status[dual_sys].hplmn.mnc) || (20 == s_sim_status[dual_sys].hplmn.mnc))))
    {
       SCI_TRACE_LOW("CheckIsRealRoaming Belgium.");
        return FALSE;
    }
    
    //Philippine
    if(((515 == cur_plmn.mcc) 
        && ((3 == cur_plmn.mnc)||(5 == cur_plmn.mnc)))
        &&((515 == s_sim_status[dual_sys].hplmn.mcc) 
        && ((3 == s_sim_status[dual_sys].hplmn.mnc) || (5 == s_sim_status[dual_sys].hplmn.mnc))))
    {
       SCI_TRACE_LOW("CheckIsRealRoaming Philippine.");
        return FALSE;
    }
#else
    //UK GB
    //Lycamobile-SIM(234/26) on the O2-network(234/10) is not roaming
    if (((234 == cur_plmn.mcc) && (10 == cur_plmn.mnc)) && ((234 == s_sim_status[dual_sys].hplmn.mcc) && (26 == s_sim_status[dual_sys].hplmn.mnc)))
    {
        return FALSE;
    }

    //HongKong
    //PCCW(3G)-SIM(454/19) on the PCCW(2G)-network(454/16) is not roaming
    // 3(3G)-SIM(454/03) on the 3(2G)-network(454/04) is not roaming
    //NewWorld-SIM(454/10) on the NewWorld-network(454/00) is not roaming
    if ((((454 == cur_plmn.mcc) && (16 == cur_plmn.mnc)) && ((454 == s_sim_status[dual_sys].hplmn.mcc) && (19 == s_sim_status[dual_sys].hplmn.mnc)))
        ||(((454 == cur_plmn.mcc) && (4 == cur_plmn.mnc)) && ((454 == s_sim_status[dual_sys].hplmn.mcc) && (3 == s_sim_status[dual_sys].hplmn.mnc)))
        ||(((454 == cur_plmn.mcc) && (0 == cur_plmn.mnc)) && ((454 == s_sim_status[dual_sys].hplmn.mcc) && (10 == s_sim_status[dual_sys].hplmn.mnc))))
    {
        return FALSE;
    }
#endif

    return TRUE;
}

/*****************************************************************************/
// 	Description : 保存网络状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreNetworkStatus(
                               APP_MN_NETWORK_STATUS_IND_T *status_ptr
                               )
{
    MN_DUAL_SYS_E dual_sys = status_ptr->dual_sys;
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"StoreNetworkStatus error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3261_112_2_18_2_45_6_76,(uint8*)"d", dual_sys);
        return;
    }
    g_service_status[dual_sys].plmn_status = status_ptr->plmn_status;
    g_service_status[dual_sys].gprs_support = status_ptr->gprs_support;
    g_service_status[dual_sys].mbms_support = status_ptr->mbms_support;
    g_service_status[dual_sys].edge_support = status_ptr->edge_support;
    g_service_status[dual_sys].hsdpa_support = status_ptr->hsdpa_support;
    g_service_status[dual_sys].hsupa_support = status_ptr->hsupa_support;
    g_service_status[dual_sys].lac = status_ptr->lac;
#ifdef MMI_NETWORK_NAME_SUPPORT
    if((g_service_status[dual_sys].plmn.mcc != status_ptr->mcc)
    ||(g_service_status[dual_sys].plmn.mnc != status_ptr->mnc))
    {
        MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
        
        network_name[dual_sys].opn_dcs = status_ptr->opn_dcs;
        network_name[dual_sys].opn_len = status_ptr->opn_len;
        network_name[dual_sys].is_add_ci = status_ptr->is_add_ci;

        if (status_ptr->is_opn_exist && 0 < status_ptr->opn_len)
        {
            network_name[dual_sys].opn_len = MIN(status_ptr->opn_len, MAX_ONSHORTFORM_LEN);
            SCI_MEMCPY(network_name[dual_sys].opn, status_ptr->opn, network_name[dual_sys].opn_len);
        }
        else
        {
            network_name[dual_sys].opn_len = 0;
            SCI_MEMSET(network_name[dual_sys].opn, 0x00, MAX_ONSHORTFORM_LEN);
        }
    }
#endif
    g_service_status[dual_sys].plmn.mcc = status_ptr->mcc;
    g_service_status[dual_sys].plmn.mnc = status_ptr->mnc;
    g_service_status[dual_sys].plmn.mnc_digit_num	= status_ptr->mnc_digit_num;
    g_service_status[dual_sys].cell_id = status_ptr->cell_id;

    if (status_ptr->plmn_is_roaming)
    {
        g_service_status[dual_sys].isRoaming = CheckIsRealRoaming(dual_sys,g_service_status[dual_sys].plmn);
    }
    else
    {
        g_service_status[dual_sys].isRoaming = FALSE;
    }
    SCI_TRACE_LOW("StoreNetworkStatus: plmn_is_roaming=%d, isRoaming=%d", status_ptr->plmn_is_roaming, g_service_status[dual_sys].isRoaming);
    
    g_service_status[dual_sys].is_slecting = status_ptr->plmn_is_selecting;    

#if defined(MMI_NETWORK_NAME_SUPPORT)
    {
        MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
        
        network_name[dual_sys].spn_len= MIN(status_ptr->spn_len,MN_SPN_ID_NUM_LEN);
        SCI_TRACE_LOW("StoreNetworkStatus: net_spn = %s, net_spn_len = %d", status_ptr->spn, status_ptr->spn_len); 
        if((0 < status_ptr->spn_len) && ((MN_SPN_ID_NUM_LEN + 1) > status_ptr->spn_len))
        {
            network_name[dual_sys].is_spn_supported = TRUE;
            SCI_MEMCPY(network_name[dual_sys].spn, status_ptr->spn, network_name[dual_sys].spn_len);
        }
        else
        {
            network_name[dual_sys].is_spn_supported = FALSE;
            SCI_MEMSET(network_name[dual_sys].spn, 0x00, MN_SPN_ID_NUM_LEN);
        }
    }
#else
    g_service_status[dual_sys].is_add_ci = status_ptr->is_add_ci;
    g_service_status[dual_sys].is_spn_need = status_ptr->is_spn_need;
    SCI_MEMSET(g_service_status[dual_sys].spn, 0x00, sizeof(g_service_status[dual_sys].spn));

    if (status_ptr->is_spn_need && 0 < status_ptr->spn_len)
    {
        g_service_status[dual_sys].spn_len = MIN(status_ptr->spn_len, sizeof(g_service_status[dual_sys].spn));
        SCI_MEMCPY(g_service_status[dual_sys].spn, status_ptr->spn, g_service_status[dual_sys].spn_len);
    }
    else
    {
        g_service_status[dual_sys].spn_len = 0;
    }

    g_service_status[dual_sys].opn_dcs	= status_ptr->opn_dcs;
    g_service_status[dual_sys].is_opn_need	= status_ptr->is_opn_need;
    g_service_status[dual_sys].is_opn_exist	= status_ptr->is_opn_exist;
    SCI_MEMSET(g_service_status[dual_sys].opn, 0x00, sizeof(g_service_status[dual_sys].opn));

    if (status_ptr->is_opn_exist && 0 < status_ptr->opn_len)
    {
        g_service_status[dual_sys].opn_len = MIN(status_ptr->opn_len, sizeof(g_service_status[dual_sys].opn));
        SCI_MEMCPY(g_service_status[dual_sys].opn, status_ptr->opn, g_service_status[dual_sys].opn_len);
    }
    else
    {
        g_service_status[dual_sys].opn_len = 0;
    }
#endif

    g_service_status[dual_sys].rat = (MMI_GMMREG_RAT_E)status_ptr->rat;

    SCI_TRACE_LOW("StoreNetworkStatus, plmn_status = %d, is_slecting = %d, isRoaming = %d, rat=%d, dual_sys = %d", g_service_status[dual_sys].plmn_status, g_service_status[dual_sys].is_slecting, g_service_status[dual_sys].isRoaming,g_service_status[dual_sys].rat , dual_sys);

    //SCI_TRACE_LOW:"NetworkStatus g:%d, e:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3307_112_2_18_2_45_6_77,(uint8*)"ddd", status_ptr->gprs_support, status_ptr->edge_support, status_ptr->hsdpa_support);

}

/*****************************************************************************/
// 	Description : ????plmn
//	Global resource dependence : 
//  Author: yongwei.he
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetCurPLMN(
                                MN_PLMN_T * cur_plmn_ptr,
                                MN_DUAL_SYS_E dual_sys
                                )
{
    cur_plmn_ptr->mcc = 	g_service_status[dual_sys].plmn.mcc;
    cur_plmn_ptr->mnc = 	g_service_status[dual_sys].plmn.mnc;
    cur_plmn_ptr->mnc_digit_num = g_service_status[dual_sys].plmn.mnc_digit_num;	
}

/*****************************************************************************/
// 	Description : get the sim hplmn
//	Global resource dependence : 
//  Author: fen.xie
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetSimHPLMN(
                                MN_PLMN_T * cur_plmn_ptr,
                                MN_DUAL_SYS_E dual_sys
                                )
{
    cur_plmn_ptr->mcc = 	s_sim_status[dual_sys].hplmn.mcc;
    cur_plmn_ptr->mnc = 	s_sim_status[dual_sys].hplmn.mnc;
    cur_plmn_ptr->mnc_digit_num = 	s_sim_status[dual_sys].hplmn.mnc_digit_num;	
}

/*****************************************************************************/
// 	Description : Gen PLMN Display
//	Global resource dependence : 
//  Author: wancan.you
//	Note:is_display_digit_format:TRUE Display 460 00
//          is_display_digit_format:FALSE Display CMCC
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GenPLMNDisplay(
                                                                            MMI_GMMREG_RAT_E rat,//IN
                                                                            MN_PLMN_T *plmn_ptr,//IN
                                                                            wchar *display_str,//[IN/OUT]
                                                                            uint16 *display_str_len,//[IN/OUT]
                                                                            BOOLEAN is_display_digit_format)
{
    MMI_TEXT_ID_T network_name_id = TXT_NULL;
    MMI_STRING_T network_str = {0};
    BOOLEAN is_digit_output = FALSE;
#ifdef CSC_XML_SUPPORT
    BOOLEAN is_csc_name = FALSE;
#endif

    if (PNULL == plmn_ptr ||PNULL == display_str ||PNULL == display_str_len)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GenPLMNDisplay plmn_ptr=%d, display_str=%d, display_str_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3407_112_2_18_2_45_6_78,(uint8*)"ddd",plmn_ptr, display_str, is_display_digit_format);
        return FALSE;
    }
 
    //SCI_TRACE_LOW:"MMIAPIPHONE_GenPLMNDisplay mcc=%d, mnc=%d, mnc_digit_num=%d, str_len=%d, is_display_digit_format=%d,rat=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3412_112_2_18_2_45_6_79,(uint8*)"dddddd",plmn_ptr->mcc, plmn_ptr->mnc, plmn_ptr->mnc_digit_num, *display_str_len, is_display_digit_format, rat);

    SCI_MEMSET(display_str, 0x00, (*display_str_len * sizeof(wchar)));

#ifdef CSC_XML_SUPPORT
    is_csc_name = MMIAPIPHONE_GetNetWorkNameByCSC(plmn_ptr->mcc, plmn_ptr->mnc, &network_str);

    if (!is_csc_name)
#endif
    {
        network_name_id = MMIAPIPHONE_GetNetWorkNameId(rat, plmn_ptr);

        MMI_GetLabelTextByLang(network_name_id, &network_str);
    }

    if (PNULL != network_str.wstr_ptr
        && 0 != network_str.wstr_len
        && !is_display_digit_format)
    {
        *display_str_len = MIN(*display_str_len, network_str.wstr_len);

        MMI_WSTRNCPY(display_str,
                                *display_str_len,
                                network_str.wstr_ptr,
                                *display_str_len,
                                *display_str_len);

        is_digit_output = FALSE;
    }
    else
    {
        uint8 temp_str[MAX_MCC_MNC_DIGIT_DISPLAY + 1] = {0};
        uint16 temp_str_len = 0;

        switch (plmn_ptr->mnc_digit_num)
        {
        case 2:
            sprintf((char*)temp_str, "%03d %02d", plmn_ptr->mcc, plmn_ptr->mnc);
            break;

        case 3:
            sprintf((char*)temp_str, "%03d %03d", plmn_ptr->mcc, plmn_ptr->mnc);
            break;

        default:
            sprintf((char*)temp_str, "%03d %d", plmn_ptr->mcc, plmn_ptr->mnc);
            break;
        }

        temp_str_len = strlen((char*)temp_str);

        *display_str_len = MIN(*display_str_len, temp_str_len);

        MMI_STRNTOWSTR(display_str,
                                *display_str_len,
                                temp_str,
                                *display_str_len,
                                *display_str_len);

        is_digit_output = TRUE;
    }

    //SCI_TRACE_LOW:"MMIAPIPHONE_GenPLMNDisplay is_digit_output=%d, network_name_id=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3474_112_2_18_2_45_6_80,(uint8*)"dd", is_digit_output, network_name_id);

#ifdef CSC_XML_SUPPORT
    if (is_csc_name && PNULL != network_str.wstr_ptr)
    {
        SCI_FREE(network_str.wstr_ptr);
    }
#endif

    return is_digit_output;
}

/*****************************************************************************/
// 	Description : Gen PLMN Display from SPN/PNN of SIM
//	Global resource dependence : 
//  Author: sam.hua
//	Note:is_display_digit_format:TRUE Display 460 00
//          is_display_digit_format:FALSE Display CMCC
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GenPLMNDisplaybySIM(
                                                                            MN_PLMN_T cur_plmn,//IN
                                                                            wchar *display_str,//[IN/OUT]
                                                                            uint16 *display_str_len,//[IN/OUT]
                                                                            MN_DUAL_SYS_E   dual_sys,
                                                                            uint16 lac)
{
    MMI_STRING_T network_str = {0};
    wchar temp_name_str[GUILIST_STRING_MAX_NUM + 1] = {0};

    if (PNULL == display_str ||PNULL == display_str_len)
    {
        return FALSE;
    }
 
    SCI_TRACE_LOW("MMIAPIPHONE_GenPLMNDisplaybySIM mcc=%d, mnc=%d,  str_len=%d",
                    cur_plmn.mcc, cur_plmn.mnc, *display_str_len);

    network_str.wstr_ptr = temp_name_str;
    
    if(!MMIPHONE_GetSPNPNNString(dual_sys,cur_plmn ,lac, &network_str))
    {
        return FALSE;
    }

    SCI_TRACE_LOW("MMIAPIPHONE_GenPLMNDisplaybySIM network_str.wstr_len = %d",network_str.wstr_len);    
    SCI_TRACE_LOW("MMIAPIPHONE_GenPLMNDisplaybySIM *display_str_len = %d", *display_str_len);    

    if (PNULL != network_str.wstr_ptr
        && 0 != network_str.wstr_len)
    {
        SCI_MEMSET(display_str, 0x00, (*display_str_len * sizeof(wchar)));

        *display_str_len = MIN(*display_str_len, network_str.wstr_len);
        SCI_MEMCPY(display_str, temp_name_str, (*display_str_len * sizeof(wchar)));
        network_str.wstr_ptr = display_str;
        
        return TRUE;
   }

    return FALSE;
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_LIST_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreferRatRsp( 
                                        DPARAM  param
                                        )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
#ifdef MMI_DUALMODE_ENABLE//@fen.xie MS00174570
    APP_MN_PREFER_RAT_RSP_T* prefer_rat_rsp = (APP_MN_PREFER_RAT_RSP_T*)param;
    
    result = MMIAPISET_HandlePreferRatRspResult(prefer_rat_rsp->set_result);
#endif
    return result;
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_LIST_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlmnListCnf(
                                     DPARAM              param
                                     )
{
    APP_MN_PLMN_LIST_CNF_T      plmn_list_cnf = *((APP_MN_PLMN_LIST_CNF_T *)param);
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    if(plmn_list_cnf.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandlePlmnListCnf error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3508_112_2_18_2_45_7_81,(uint8*)"d", plmn_list_cnf.dual_sys);
        return MMI_RESULT_FALSE;
    }
    
    if ((MN_PHONE_OPERATE_SUCCESS == plmn_list_cnf.is_valid_result) &&
        (0 < plmn_list_cnf.plmn_detail_list.plmn_num))
    {
        MMIAPISET_OpenPlmnListWin(plmn_list_cnf.dual_sys, &(plmn_list_cnf.plmn_detail_list));
    }
#ifdef MMI_DUALMODE_ENABLE//@fen.xie MS00174570
    else if (MMIAPISET_IsPromptRetry()//单模下手动搜网且搜网前已经是无网络
        && (MN_PHONE_OPERATE_SUCCESS == plmn_list_cnf.is_valid_result) &&  (0 == plmn_list_cnf.plmn_detail_list.plmn_num)//结果为无网络
        )
    {
        //提示"切换到双模再试"
        MMIPUB_OpenAlertWarningWin( TXT_TO_DUAL_AND_TRY_LATER);
    }
#endif
    else//fail
    {
      //  MMIPUB_OpenAlertWarningWin(STR_SIM_SEARCH_FAIL);
    }
    
    //关闭网络连接窗口
    MMIAPISET_CloseConnectNetworkWin();
#if 1//def MMI_SMART_DUAL_SIM_SUPPORT
    MMISmartDS_CloseNetworkWaittingWin();
#endif

    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PS_POWER_OFF_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerPsOffCnf(
                                        DPARAM              param
                                        )
{
    APP_MN_PS_POWER_OFF_CNF_T ps_power_off_cnf   = *((APP_MN_PS_POWER_OFF_CNF_T *)param);
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    dual_sys = ps_power_off_cnf.dual_sys;
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    SCI_TRACE_LOW("HandlePowerPsOffCnf,dual_sys=%d",dual_sys);
#endif    
    g_service_status[dual_sys].is_slecting = TRUE;
    MMIAPIPHONE_SetIsPsDeactiveCnf(dual_sys, TRUE);
    g_service_status[dual_sys].is_first_rssi = TRUE;
    g_service_status[dual_sys].rssiLevel = 0;
    g_service_status[dual_sys].plmn_status = PLMN_NO_SERVICE;
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_Service_Indicator(FALSE);
#endif	
    MMIAPICOM_StatusAreaSetGprsState(dual_sys, FALSE);//关闭gprs图标
    MMIAPICOM_StatusAreaSetRxLevel(dual_sys, 0, TRUE);//信号显示0格
    MMIAPIPHONE_SetIsPsReady(dual_sys, FALSE);
    MMIAPICOM_StatusAreaSetRoamingState(dual_sys, FALSE);
    MMIAPICOM_StatusAreaSetForwardState(dual_sys, FALSE);
    MMIAPICOM_StatusAreaSetGprsState(dual_sys, FALSE);
#ifdef MMI_CSP_SUPPORT
    if(MMIAPIPHONE_IsCPHSSupport())
    {
    	MAIN_UpdateLineState(); //重新刷新line state的显示
    }
#endif  
#ifdef SIM_PLUG_IN_SUPPORT
    ImplementStandByModeAsync();
#endif    

#if 0//177149
    if (MMIAPISTK_IsStkRefresh())
    {
        MMIAPIPHONE_PowerReset();
    }
#endif
    
    if (MMIAPISTK_IsStkRefresh() && (SIMAT_RF_RESET == MMISTK_GetStkRefreshType(dual_sys)))
    {
        MNPHONE_PowerOffSimEx(dual_sys);
    }

    if (s_phone_is_power_off_req[dual_sys])
    {
        s_phone_is_power_off_req[dual_sys] = FALSE;

        MNPHONE_PowerOffSimEx(dual_sys);
    }
#ifdef MMS_SUPPORT
    MMIAPIMMS_DestroyOperateWaitingWin();
#endif


    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_DEACTIVE_PS_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeactivePsCnf(
                                        DPARAM              param
                                        )
{
    APP_MN_DEACTIVE_PS_CNF_T ps_deactive_cnf   = *((APP_MN_DEACTIVE_PS_CNF_T *)param);
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    dual_sys = ps_deactive_cnf.dual_sys;
    
    g_service_status[dual_sys].is_slecting = TRUE;
    SCI_TRACE_LOW("HandleDeactivePsCnf MMIPHONE_SetSimStatus");
    MMIPHONE_SetSimStatus(dual_sys, SIM_NOT_INITED);
    MMIAPIPHONE_SetIsPsDeactiveCnf(dual_sys, TRUE);
    g_service_status[dual_sys].is_first_rssi = TRUE;
    g_service_status[dual_sys].rssiLevel = 0;
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_Service_Indicator(FALSE);
#endif		
    MMIAPICOM_StatusAreaSetGprsState(dual_sys, FALSE);//关闭gprs图标
    MMIAPICOM_StatusAreaSetRxLevel(dual_sys, 0, TRUE);//信号显示0格
    MMIAPIPHONE_SetIsPsReady(dual_sys, FALSE);
    MMIAPICOM_StatusAreaSetRoamingState(dual_sys, FALSE);
    MMIAPICOM_StatusAreaSetGprsState(dual_sys, FALSE);    

    if(!MMIAPISET_IsCfuStatusSavedInSim(dual_sys))
    {
        MMIAPICOM_StatusAreaSetForwardState(dual_sys, FALSE);			
    }  
    
#ifdef MMI_CSP_SUPPORT    
    if(MMISET_IsAlsValid(dual_sys))
       
    {
        MAIN_UpdateLineState();  //????line state???
    }    
#endif //MMI_CSP_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT
    ImplementStandByModeAsync();
#endif
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
    MMIAPISET_ImplementCTAStandbyModeAsync(dual_sys);
#endif
//bug 134457
/*#ifdef MMI_SIM_LOCK_SUPPORT
        //deactive ps and normal init
        if(MMIPHONE_GetPhoneNeedRestart(ps_deactive_cnf.dual_sys))
        {
            MMIAPIPHONE_PowerOnSIM(ps_deactive_cnf.dual_sys);
            MMIPHONE_SetPhoneNeedRestart(ps_deactive_cnf.dual_sys,FALSE);
        }
#endif     */   
    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_POWER_ON_CNF
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOnSimCnf(
                                       DPARAM              param
                                       )
{
    APP_MN_SIM_POWER_ON_CNF_T power_on_sim_cnf = *((APP_MN_SIM_POWER_ON_CNF_T *)param);
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
#endif
    
    dual_sys = power_on_sim_cnf.dual_sys;  
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    SCI_TRACE_LOW("HandlePowerOnSimCnf(),dual_sys[%d],exist[%d],sim_lock,sim1_rihgt = %d,sim2_right=%d", 
			dual_sys, power_on_sim_cnf.is_ok,s_is_sim1_right,s_is_sim2_right);
#else    
    //SCI_TRACE_LOW:"HandlePowerOnSimCnf:dual_sys=%d,is_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3634_112_2_18_2_45_7_82,(uint8*)"dd", dual_sys, power_on_sim_cnf.is_ok);
#endif
    #ifdef SIM_LOCK_CT_SUPPORT
    g_smartcard_status_t[dual_sys].pwon_cnf = 1;
    if(MMICDMA_GetSmartCard_NeedCheck(dual_sys) && FALSE == MMICDMA_GetSmartCard_Over(dual_sys))
    {
       Trace_Log_Print("PS: HandlePowerOnSimCnf IS GSM");
       MMIPhone_CDMA_SimPowerOver(dual_sys);
        
        ATC_SendTo_APP_UimPresent(dual_sys,FALSE,0);
        ATC_SendTo_APP_UimNotReady(dual_sys);
        ATC_SendTo_APP_UimPWOnCfn(dual_sys,FALSE);
        MNPHONE_PowerOffSimEx(dual_sys);
        return result;
    }
#endif

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandlePowerOnSimCnf error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3638_112_2_18_2_45_7_83,(uint8*)"d", dual_sys);
        return MMI_RESULT_FALSE;
    }
    
    MMIAPIPHONE_SetSimExistedStatus(dual_sys, power_on_sim_cnf.is_ok);

    s_is_power_on_sim_cnf[dual_sys] = TRUE;
    
#ifdef MMI_MULTI_SIM_SYS_DUAL
    if (SIM_QUERY_STATUS_START == s_query_sim_status)
    {
        uint32 i = 0;
        BOOLEAN is_query_end = TRUE;
        
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
        {
            if(!s_is_power_on_sim_cnf[i])
            {
                is_query_end = FALSE;
                break;
            }
        }

        if (is_query_end)
        {
            s_query_sim_status = SIM_QUERY_STATUS_END;//设置查询结束
        }

        if (!g_is_sim_only_check_status[dual_sys])
        {
            if ((e_dualsys_setting & (0x01 << dual_sys))
                && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
            {
                MMIAPIPHONE_PowerOnPs(dual_sys);
            }
        }
        else
        {
            MNPHONE_PowerOffSimEx(dual_sys);
        }
    }       
    
    if (SIM_QUERY_STATUS_END == s_query_sim_status && !g_is_sim_only_check_status[dual_sys])
    {
        uint32 i = 0;

        if (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))//无待机卡
        {
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
            {
                if (e_dualsys_setting & (0x01 << i))
                {
                    MMIAPIPHONE_PowerOnPs((MN_DUAL_SYS_E)i);
                    break;
                }
            }
        }
#if defined(MMI_CSP_SUPPORT)
        if(MMISET_IsAlsValid(dual_sys))
        {
            MAIN_UpdateLineState();  //重新刷新line state的显示
        }
#endif  //MMI_CSP_SUPPORT    
        MMIAPISET_SetIsQueryCfu(FALSE,dual_sys);
        //MMIAPIPHONE_SetGPRSAttachMode();

        //卡1不存在不用排序
        if (!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
        {
            MMIAPIPB_InitSimStatus(MN_DUAL_SYS_1);
        }
        
        //卡2不存在不用排序
        if (!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
        {
            MMIAPIPB_InitSimStatus(MN_DUAL_SYS_2);
        }
        
        s_query_sim_status = SIM_QUERY_STATUS_NONE;

#if defined(MMI_MSD_SUPPORT)
#ifdef MMI_SIM_LOCK_SUPPORT
        if(!MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_ALERT_WIN_ID))
#endif	
        {
            MMIAPIMSD_SimLegalityCheck();//待机确认后验证防盗密码
        }
#endif//MMI_MSD_SUPPORT
/******************************************************************************************************************/
//CASE: 无论SIM 是否合法有效(for sim lock )，sim cnf 之后都做power on ps的动作
// 这样，就需要在所有的卡都收到cnf之后，确认是否是lock状态
//如果是不允许的sim，就将所有PS 都关闭。
/******************************************************************************************************************/
//for ps start sequence of sim lock ,there are two methods( SIM2 is last ):
//(1). all ps power on at first, and last to check the ps need power off or not (not used)
//(2). all ps are not powered on at first, and last to check which ps need power on.(used)
/******************************************************************************************************************/
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
		if(!s_is_simlock_off)
		{
			//all ps are not powered on at first, and last to check which ps need power on.
			//result: SIM2 PS power on at first, SIM1 PS power on last dependent sim.
			if(MN_DUAL_SYS_2 == dual_sys)
			{
#ifdef DUAL_SIMLOCK_A//A、双卡中任意一张卡满足条件，都可以正常使用！
				if(s_is_sim1_right || s_is_sim2_right) 
				{
					if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))                
					{
						MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_1); //sim2 ps has been powered on above in this function.
						SCI_TRACE_LOW("HandlePowerOnSimCnf s_is_sim1_right!");
					}
				}
				else
				{
					MN_DUAL_SYS_E dual_sim = MN_DUAL_SYS_1;
					//set sim status not valid For display end user indication string info!!!!!!!!!!      
					SCI_TRACE_LOW("HandlePowerOnSimCnf other case ,all sim invalid!");
					for(dual_sim = MN_DUAL_SYS_1; dual_sim < MMI_DUAL_SYS_MAX; dual_sim++)
					{
						//针对存在的sim卡并且卡被锁，则提示invlid sim.
						//其它所有not ready 的sim卡提示信息跟原有一致!
						if(MMIAPIPHONE_GetSimExistedStatus(dual_sim))                
						{                    
							MMIPHONE_SetSimStatus(dual_sim, SIM_STATUS_LOCKED_INVALID);
						}
						//如果在pin码界面开启了PS，那么在锁定卡时，需要关闭PS
						if (s_ps_ready[dual_sim] == MMIPHONE_PS_POWER_ON_EMERGE)
						{
							MMIAPIPHONE_PowerOffPs(dual_sim);
						}
					}
				}
#elif defined(DUAL_SIMLOCK_B)//B、卡1正确时，启动卡1卡2；卡2插入正确的卡时，只有在卡1不存在时，卡2正常初始化
				if(s_is_sim1_right) 
				{
					if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))                
					{
						MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_1); //sim2 ps has been powered on above in this function.
						SCI_TRACE_LOW("HandlePowerOnSimCnf s_is_sim1_right!");
					}
				}
				else if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1) && s_is_sim2_right)
				{
					//sim2检测OK，sim1不存在时，启动sim2                
					if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))                
					{
						MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_2);
						SCI_TRACE_LOW("HandlePowerOnSimCnf sim1 not exist, s_is_sim2_right!");
					}
				}
				else
				{
					MN_DUAL_SYS_E dual_sim = MN_DUAL_SYS_1;
					//set sim status not valid For display end user indication string info!!!!!!!!!!      
					SCI_TRACE_LOW("HandlePowerOnSimCnf other case ,all sim invalid!");
					for(dual_sim = MN_DUAL_SYS_1; dual_sim < MMI_DUAL_SYS_MAX; dual_sim++)
					{
						//针对存在的sim卡并且卡被锁，则提示invlid sim.
						//其它所有not ready 的sim卡提示信息跟原有一致!
						if(MMIAPIPHONE_GetSimExistedStatus(dual_sim))                
						{                    
							MMIPHONE_SetSimStatus(dual_sim, SIM_STATUS_LOCKED_INVALID);
						}
						//如果在pin码界面开启了PS，那么在锁定卡时，需要关闭PS
						if (s_ps_ready[dual_sim] == MMIPHONE_PS_POWER_ON_EMERGE)
						{
							MMIAPIPHONE_PowerOffPs(dual_sim);
						}
					}
				}
#elif defined(DUAL_SIMLOCK_C)//C、卡1正确时，启动卡1，此时如果卡2不正确则锁定卡2；卡2插入正确的卡时，卡2正常初始化，此时如果卡1不正确则锁定卡1;其他情况锁定
				if (s_is_sim1_right) 
				{
					if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))                
					{
						MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_1); //sim2 ps has been powered on above in this function.
						SCI_TRACE_LOW("HandlePowerOnSimCnf s_is_sim1_right!");
					}
				}
				else
				{
					SCI_TRACE_LOW("HandlePowerOnSimCnf !s_is_sim1_right!");
					if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))                
					{                    
						MMIPHONE_SetSimStatus(MN_DUAL_SYS_1, SIM_STATUS_LOCKED_INVALID);
					}
					//如果在pin码界面开启了PS，那么在锁定卡时，需要关闭PS
					if (s_ps_ready[MN_DUAL_SYS_1] == MMIPHONE_PS_POWER_ON_EMERGE)
					{
						MMIAPIPHONE_PowerOffPs(MN_DUAL_SYS_1);
					}
				}
				
				if (s_is_sim2_right) 
				{
					if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))                
					{
						MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_2);
						SCI_TRACE_LOW("HandlePowerOnSimCnf s_is_sim2_right!");
					}
				}
				else
				{
					SCI_TRACE_LOW("HandlePowerOnSimCnf !s_is_sim2_right!");
					if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))                
					{                    
						MMIPHONE_SetSimStatus(MN_DUAL_SYS_2, SIM_STATUS_LOCKED_INVALID);
					}
					//如果在pin码界面开启了PS，那么在锁定卡时，需要关闭PS
					if (s_ps_ready[MN_DUAL_SYS_2] == MMIPHONE_PS_POWER_ON_EMERGE)
					{
						MMIAPIPHONE_PowerOffPs(MN_DUAL_SYS_2);
					}
				}
#elif defined(DUAL_SIMLOCK_D)//D、卡1正确时，启动卡1卡2；其他情况锁定
				if(s_is_sim1_right) 
				{
					if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))                
					{
						MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_1); //sim2 ps has been powered on above in this function.
						SCI_TRACE_LOW("HandlePowerOnSimCnf s_is_sim1_right!");
					}
				}
				else
				{
					MN_DUAL_SYS_E dual_sim = MN_DUAL_SYS_1;
					//set sim status not valid For display end user indication string info!!!!!!!!!!      
					SCI_TRACE_LOW("HandlePowerOnSimCnf other case ,all sim invalid!");
					for(dual_sim = MN_DUAL_SYS_1; dual_sim < MMI_DUAL_SYS_MAX; dual_sim++)
					{
						//针对存在的sim卡并且卡被锁，则提示invlid sim.
						//其它所有not ready 的sim卡提示信息跟原有一致!
						if(MMIAPIPHONE_GetSimExistedStatus(dual_sim))                
						{                    
							MMIPHONE_SetSimStatus(dual_sim, SIM_STATUS_LOCKED_INVALID);
						}
						//如果在pin码界面开启了PS，那么在锁定卡时，需要关闭PS
						if (s_ps_ready[dual_sim] == MMIPHONE_PS_POWER_ON_EMERGE)
						{
							MMIAPIPHONE_PowerOffPs(dual_sim);
						}
					}
				}
#elif defined(DUAL_SIMLOCK_E)//E、卡1不合法时锁定卡1，合法时不锁定；卡2任何情况下都不锁定
				if (s_is_sim1_right) 
				{
					if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))                
					{
						MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_1); //sim2 ps has been powered on above in this function.
						SCI_TRACE_LOW("HandlePowerOnSimCnf s_is_sim1_right!");
					}
				}
				else
				{
					if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
					{                    
						MMIPHONE_SetSimStatus(MN_DUAL_SYS_1, SIM_STATUS_LOCKED_INVALID);
					}
					//如果在pin码界面开启了PS，那么在锁定卡时，需要关闭PS
					if (s_ps_ready[MN_DUAL_SYS_1] == MMIPHONE_PS_POWER_ON_EMERGE)
					{
						MMIAPIPHONE_PowerOffPs(MN_DUAL_SYS_1);
					}
				}
				//启动sim2
				if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))                
				{
					MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_2);
					SCI_TRACE_LOW("HandlePowerOnSimCnf sim2 exist");
				}
#endif
			}
		}
#endif // defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    }    
#elif defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    if (SIM_QUERY_STATUS_START == s_query_sim_status)
    {
        uint32 i = 0;
        BOOLEAN is_query_end = TRUE;
        
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
        {
            if(!s_is_power_on_sim_cnf[i])
            {
                is_query_end = FALSE;
                break;
            }
        }

        if (is_query_end)
        {
            s_query_sim_status = SIM_QUERY_STATUS_END;//设置查询结束
        }
        
        if (!g_is_sim_only_check_status[dual_sys])
        {
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
            {
                if ((dual_sys == i) && (e_dualsys_setting & (0x01 << i))
                    && MMIAPIPHONE_GetSimExistedStatus(i))
                {
                    if (MMIAPIIDLE_IdleWinIsOpen())
                    {
                        MMIAPIPHONE_PowerOnPs(dual_sys);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"HandlePowerOnSimCnf: Idle is not open"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3759_112_2_18_2_45_7_84,(uint8*)"");
                    }
                    break;
                }
            }
        }
        else
        {
            MNPHONE_PowerOffSimEx(dual_sys);
        }
    }       

    if (SIM_QUERY_STATUS_END == s_query_sim_status && !g_is_sim_only_check_status[dual_sys])
    {
        uint32 i = 0;

        if (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))//无待机卡
        {
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
            {
                if (e_dualsys_setting & (0x01 << i))
                {
                    MMIAPIPHONE_PowerOnPs((MN_DUAL_SYS_E)i);
                    break;
                }
            }
        }

#if defined(MMI_CSP_SUPPORT)
        if(MMISET_IsAlsValid(dual_sys))
        {
            MAIN_UpdateLineState();  //重新刷新line state的显示
        }
#endif  //MMI_CSP_SUPPORT  
        MMIAPISET_SetIsQueryCfu(FALSE,dual_sys);
        //MMIAPIPHONE_SetGPRSAttachMode();

        //卡不存在不用排序
        for (i = 0; i < MMI_DUAL_SYS_MAX; i ++)
        {
            if (!MMIAPIPHONE_GetSimExistedStatus(i))
            {
                MMIAPIPB_InitSimStatus(i);
            }
        }
        s_query_sim_status = SIM_QUERY_STATUS_NONE;

#if defined(MMI_MSD_SUPPORT)
#ifdef MMI_SIM_LOCK_SUPPORT
        if(!MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_ALERT_WIN_ID))
#endif	
        {
            MMIAPIMSD_SimLegalityCheck();//待机确认后验证防盗密码
        }
#endif//MMI_MSD_SUPPORT
    }    
#else
    MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    if (!s_is_simlock_off)
    {
        if( !s_is_sim1_right )
        {
            //set sim status not valid For display end user indication string info!!!!!!!!!!      
            SCI_TRACE_LOW("HandlePowerOnSimCnf sim1 invalid!");
            MMIPHONE_SetSimStatus(MN_DUAL_SYS_1, SIM_STATUS_LOCKED_INVALID);
            //如果在pin码界面开启了PS，那么在锁定卡时，需要关闭PS
			if (s_ps_ready[MN_DUAL_SYS_1] == MMIPHONE_PS_POWER_ON_EMERGE)
            {
                MMIAPIPHONE_PowerOffPs(MN_DUAL_SYS_1);
            }
        }
    }
#endif
#if defined(MMI_MSD_SUPPORT)
#ifdef MMI_SIM_LOCK_SUPPORT
    if(!MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_ALERT_WIN_ID))
#endif
    {
        MMIAPIMSD_SimLegalityCheck();
    }
#endif//MMI_MSD_SUPPORT
#endif

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    //只做某一张SIM卡的优选语言的配置，其他的sim 卡不做配置。
    if(( !s_is_language_auto_set )&&( power_on_sim_cnf.is_ok ))
    {
        MMISET_LANGUAGE_TYPE_E nv_language_type = (MMISET_LANGUAGE_TYPE_E)MMIAPISET_GetNVLanguageType();

        //语言是否需要优选配置。
        if(MMISET_LANGUAGE_AUTO == nv_language_type ) 
        {
            MMISET_LANGUAGE_TYPE_E prefer_language = {0};
            BOOLEAN is_prefer_set = MMIAPISET_GetPreferLanguageTypeFromSim(dual_sys, &prefer_language);

            //SCI_TRACE_LOW:"HandlePowerOnSimCnf,dual_sys=%d,is_prefer_set=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3840_112_2_18_2_45_7_85,(uint8*)"dd",dual_sys,is_prefer_set);

            if( is_prefer_set )
            {
                s_is_language_auto_set = TRUE; //标记某张sim卡成功做了优选语言
                
                s_prefer_language_sim = dual_sys; //记录当前优选语言采用的SIM卡。

                //做系统资源的加载和设置。   
                MMIAPISET_SetLanguageType(nv_language_type);    
            }
        }
    }
#endif

    if (MMIAPISTK_IsStkRefresh() && (SIMAT_RF_RESET == MMISTK_GetStkRefreshType(dual_sys)))
    {
        if (!MMIAPISET_GetFlyMode()
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            && (e_dualsys_setting & (0x01 << dual_sys))
#endif
            )
        {
            SCI_TRACE_LOW("MMIPHONE: MMIAPISTK_IsStkRefresh Power on ps");
            MMIAPIPHONE_PowerOnPs(dual_sys);
        }
        MMIAPISTK_SetStkRefresh(FALSE);
        MMIAPISMS_SetIsSMSRefresh(dual_sys, FALSE);
        MMIAPISTK_CloseRefreshWaitingWin();
    }

    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_POWER_OFF_CNF
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOffSimCnf(
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_SIM_POWER_OFF_CNF_T power_off_sim_cnf = *((APP_MN_SIM_POWER_OFF_CNF_T *)param);
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint32 i = 0;
    BOOLEAN is_sim_status_checked = TRUE;

    dual_sys = power_off_sim_cnf.dual_sys;

    //SCI_TRACE_LOW:"HandlePowerOffSimCnf:dual_sys=%d,is_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3874_112_2_18_2_45_7_86,(uint8*)"dd", dual_sys, power_off_sim_cnf.is_ok);

    g_is_sim_only_check_status[dual_sys] = FALSE;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (g_is_sim_only_check_status[i])
        {
            is_sim_status_checked = FALSE;
            break;
        }
    }

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
    if (is_sim_status_checked)
    {
        MMIAPISET_OpenCTAMultiSysStandbyWin(TRUE);
    }
#else
    MMIAPIPHONE_SetSimExistedStatus(dual_sys, FALSE);
    MMIPHONE_SetSimStatus(dual_sys, SIM_NOT_INITED);
#endif
#ifdef MMI_SIM_LOCK_SUPPORT
        //deactive ps and normal init
        if(MMIPHONE_GetPhoneNeedRestart(power_off_sim_cnf.dual_sys))
        {
            MMIAPIPHONE_PowerOnSIM(power_off_sim_cnf.dual_sys);
            MMIPHONE_SetPhoneNeedRestart(power_off_sim_cnf.dual_sys,FALSE);
        }
#endif 
#if defined(UNISOC_MMI_SIM_LOCK_SUPPORT)&&!defined(MMI_SIM_LOCK_SUPPORT)
        //deactive ps and normal init
        if(MMIPHONE_GetPhoneNeedRestart(power_off_sim_cnf.dual_sys))
        {
            MMIAPIPHONE_PowerOnSIM(power_off_sim_cnf.dual_sys);
            MMIPHONE_SetPhoneNeedRestart(power_off_sim_cnf.dual_sys,FALSE);
        }
#endif

    if (MMIAPISTK_IsStkRefresh() && (SIMAT_RF_RESET == MMISTK_GetStkRefreshType(dual_sys)))
    {
        MMIAPIPHONE_PowerOnSIM(dual_sys);
    }

    return result;
}

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PREFER_PLMN_CHANGE_CNF
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreferPlmnChangeCnf(
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_PREFER_PLMN_CHANGE_CNF_T prefer_plmn_sim_cnf = *((APP_MN_PREFER_PLMN_CHANGE_CNF_T *)param);
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    dual_sys = prefer_plmn_sim_cnf.dual_sys;
   
    //SCI_TRACE_LOW:"HandlePowerOffSimCnf:dual_sys=%d,is_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3915_112_2_18_2_45_7_87,(uint8*)"dd", dual_sys, prefer_plmn_sim_cnf.is_ok);

    MMIAPISET_HandlePreferNetworkListCnf(&prefer_plmn_sim_cnf);
    
    return result;
}

#endif

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_SELECT_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectPlmnCnf(
                                       DPARAM              param
                                       )
{
    APP_MN_PLMN_SELECT_CNF_T    plmn_select_cnf = *((APP_MN_PLMN_SELECT_CNF_T *)param);
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E   dual_sys = MN_DUAL_SYS_1;
    
    dual_sys = plmn_select_cnf.dual_sys;
    
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleSelectPlmnCnf error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3939_112_2_18_2_45_7_88,(uint8*)"d", dual_sys);
        return MMI_RESULT_FALSE;
    }

    if (plmn_select_cnf.plmn_is_selected)
    {
        //success!
        // 保存网络状态信息
        g_service_status[dual_sys].plmn_status = plmn_select_cnf.plmn_status;
        g_service_status[dual_sys].lac         = plmn_select_cnf.lac;
        g_service_status[dual_sys].plmn.mcc         = plmn_select_cnf.mcc;
        g_service_status[dual_sys].plmn.mnc         = plmn_select_cnf.mnc;
        g_service_status[dual_sys].plmn.mnc_digit_num = plmn_select_cnf.mnc_digit_num;
        g_service_status[dual_sys].cell_id     = plmn_select_cnf.cell_id;
           
        //MMIPUB_OpenAlertSuccessWin(TXT_PLMN_SELECT_COMPLETE);

        //关闭网络列表窗口
        MMIAPISET_ClosePlmnListWin();
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(STR_SIM_CONNECTION_FAIL);
    }
    
    //关闭网络连接窗口
    MMIAPISET_CloseConnectNetworkWin();
#if 1//def MMI_SMART_DUAL_SIM_SUPPORT
    MMISmartDS_CloseNetworkWaittingWin();
#endif
    
    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SET_BAND_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectBandCnf(
                                       DPARAM              param
                                       )
{
    APP_MN_SET_BAND_CNF_T    band_select_cnf = *((APP_MN_SET_BAND_CNF_T *)param);
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    MMIPUB_CloseAlertWin();
    if (!MMIAPIENG_IsBandSelectWinOpen())
    {
        //SCI_TRACE_LOW:"HandleSelectBandCnf band_select_cnf.is_ok = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3984_112_2_18_2_45_8_89,(uint8*)"d",band_select_cnf.is_ok);
#if 0
        if (band_select_cnf.is_ok)
        {
            //success!
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
        }
#endif
        //关闭网络连接窗口
        MMIAPISET_CloseConnectNetworkWin();
#if 1//def MMI_SMART_DUAL_SIM_SUPPORT
    MMISmartDS_CloseNetworkWaittingWin();
#endif
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : get the network name
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetNetWorkNameId(
                                     MN_DUAL_SYS_E dual_sys,
                                     MN_PLMN_T *plmn_ptr,
                                     MN_PHONE_PLMN_STATUS_E  plmn_status,
                                     BOOLEAN                 is_slecting
                                     )
{
    MMI_TEXT_ID_T   network_name_id = TXT_NULL;

    if(PNULL == plmn_ptr)
    {
        //SCI_TRACE_LOW:"GetNetWorkNameId: the network plmn_ptr is error! %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4016_112_2_18_2_45_8_90,(uint8*)"d",plmn_ptr);
        return  TXT_NULL;
    }
    
	SCI_TRACE_LOW("GetNetWorkNameId, is_slecting = %d, plmn_status = %d, dual_sys = %d", is_slecting, plmn_status, dual_sys);

    if (is_slecting)
    {
        network_name_id = STR_SIM_SEARCHING_NETWORK;
    }
    else
    {
        switch(plmn_status)
        {
        case PLMN_NO_SERVICE:
            network_name_id = STR_SIM_NO_SERVICE_EXT01;
            break;

        case PLMN_EMERGENCY_ONLY:
        case PLMN_EMERGENCY_SIM_INVALID:
        case PLMN_EMERGENCY_GPRS_ONLY:
        case PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY:
        case PLMN_FULL_PS_SERVICE:
            network_name_id = STXT_EMERGENCY_ONLY;
            break;
            
        case PLMN_REGISTER_GPRS_ONLY:
        case PLMN_REGISTER_SERVICE:    
            network_name_id = STR_SIM_REGISTERING;
            break;
        
        case PLMN_NORMAL_GPRS_ONLY:
        case PLMN_NORMAL_GSM_ONLY:
        case PLMN_NORMAL_GSM_GPRS_BOTH:
            network_name_id = MMIAPIPHONE_GetNetWorkNameId(MMIAPIPHONE_GetTDOrGsm(dual_sys), plmn_ptr);

            if (TXT_NULL == network_name_id)
            {
                network_name_id = TXT_FULL_SERVICE;
            }
            break;
            
        default:
            //SCI_TRACE_LOW:"GetNetWorkNameId: the network status is error! %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4057_112_2_18_2_45_8_91,(uint8*)"d", plmn_status);
            network_name_id = STXT_EMERGENCY_ONLY;
            break;
        }
    }
    
    return (network_name_id);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_GET_PIN_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPinInd(
                                   DPARAM              param
                                   )
{    
    APP_MN_SIM_GET_PIN_IND_T         sim_get_pin_ind = *((APP_MN_SIM_GET_PIN_IND_T *)param);
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 i = 0;
    BOOLEAN is_need_handle = FALSE;
#endif
    g_sim_pin_sys_dual = sim_get_pin_ind.dual_sys;
    if(sim_get_pin_ind.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleSimPinInd:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4084_112_2_18_2_45_8_92,(uint8*)"d", sim_get_pin_ind.dual_sys);
        return MMI_RESULT_FALSE;
    }

    if (e_dualsys_setting & (0x01 << sim_get_pin_ind.dual_sys))
    {
        PowerOnPsForEmerge(sim_get_pin_ind.dual_sys);
    }
	// Bug 1351696, 输入pin时若只插入一张被禁用sim卡，仍然开启协议栈，作为紧急呼叫用
	else if (MMIAPIPHONE_GetSimExistedNum(PNULL, 0) == 1)
	{
		PowerOnPsForEmerge(sim_get_pin_ind.dual_sys);
	}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    s_handle_pin_status.sim_pin_ind[sim_get_pin_ind.dual_sys] = sim_get_pin_ind;

    for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(( i != sim_get_pin_ind.dual_sys)&&(s_handle_pin_status.is_handling_pin[i]))
        {
            is_need_handle = TRUE;
            break;            
        }
    }
    
    if(is_need_handle)
    {
        s_handle_pin_status.need_handle_pin[sim_get_pin_ind.dual_sys] = TRUE;
		
        return result;
    }
    else
    {            
        s_handle_pin_status.is_handling_pin[sim_get_pin_ind.dual_sys] = TRUE;   //set the status (lock)
        s_handle_pin_status.cur_handling_sim_id = sim_get_pin_ind.dual_sys;
    }
#else
        s_handle_pin_status.is_handling_pin[sim_get_pin_ind.dual_sys] = TRUE;
#endif
    
    CreateInputPinWindow(sim_get_pin_ind);

    //MMIAPIPHONE_SetGPRSAttachMode();
    
    return result;
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_PIN_FUNC_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPinCnf(
                                   DPARAM              param
                                   )
{
    APP_MN_SIM_PIN_FUNC_CNF_T        sim_pin_cnf = *((APP_MN_SIM_PIN_FUNC_CNF_T *)param);
    MMI_WIN_ID_T         window_id = MMK_GetFocusWinId();
    MMI_RESULT_E            result = MMI_RESULT_TRUE;

    //cr17666,请稍候界面来闹钟，接收到ps消息仍需要处理
    if (MMK_IsOpenWin(PHONE_PIN_WAIT_WIN_ID))
    {
        window_id = PHONE_PIN_WAIT_WIN_ID;
    }
    else if (MMK_IsOpenWin(IDLE_SPECIAL_WAIT_WIN_ID))
    {
        window_id = IDLE_SPECIAL_WAIT_WIN_ID;
    }
    
    switch (window_id)
    {
    case PHONE_PIN_WAIT_WIN_ID:
        if ((sim_pin_cnf.pin_operate_result) == MNSIM_PIN_REQ_OK)
        {
            if (0 != g_operate_pin_win_tab)
            {
                //MMIAPISET_OpenMsgBox(g_operate_pin_win_tab, g_is_operate_pin_special_func);
                MMIAPISET_OpenMsgBoxCustom(g_operate_pin_win_tab, g_is_operate_pin_special_func, &sim_pin_cnf);
            }
            
            if (MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID) && (MNSIM_PIN_CHANGE != sim_pin_cnf.pin_function_type)) //记录下正确的pin码
            {
                MMI_STRING_T   pin_value = {0};   
                //GUIEDIT_GetString(MMI_PIN_INPUT_EDITBOX_CTRL_ID, &pin_value);
                MMI_HANDLE_T pinCtrl_handle = MMK_GetCtrlHandleByWin(MMI_PIN_INPUTING_WIN_ID, MMIPUB_INPUTFIELD_CTRL_ID);
                GUIEDIT_GetString(pinCtrl_handle,&pin_value);
                g_pin_value[sim_pin_cnf.dual_sys].blocklen = pin_value.wstr_len;

                MMI_WSTRNTOSTR(
                    g_pin_value[sim_pin_cnf.dual_sys].blockbuf,
                    MN_MAX_BLOCK_LEN,
                    pin_value.wstr_ptr,
                    g_pin_value[sim_pin_cnf.dual_sys].blocklen,
                    g_pin_value[sim_pin_cnf.dual_sys].blocklen
                    );
#ifdef MMI_RESET_PHONE_AUTO_PIN
                //set sim pin to nv
                MMINV_WRITE(MMINV_SIM_PIN_VALUE, g_pin_value);
#endif       
            }
            
            MMK_CloseWin(MMI_PIN_INPUTING_WIN_ID);
            MMK_CloseWin(MMI_PUK_INPUTING_WIN_ID);

            if (!g_is_operate_pin_special_func || 0 == g_operate_pin_win_tab)
            {
                // 关闭等待窗口
                MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
            }
        }
        else
        {
            if (sim_pin_cnf.pin_status.pin_blocked) //PIN被锁,要求输入PUK
            {
                if (MMK_IsOpenWin(MMI_PUK_INPUTING_WIN_ID))
                {
                    MMI_HANDLE_T pukCtrl_handle = MMK_GetCtrlHandleByWin(MMI_PUK_INPUTING_WIN_ID, MMIPUB_INPUTFIELD_CTRL_ID);
                    if (MNSIM_PIN1 == sim_pin_cnf.pin_num)//PIN1
                    {
                        if (sim_pin_cnf.unblock_pin_status.pin_blocked)
                        {
                            //提示错误窗口,PUK错被锁!
                            //MMIPUB_OpenAlertWarningWin(TXT_PUK_BLOCKED);
                            MMISET_DisplaybannerInfo(PNULL, STR_SIM_BLOCKED_EXT01, PNULL);

                            MMK_CloseWin(MMI_PUK_INPUTING_WIN_ID);
                        }
                        else
                        {
                            //提示错误窗口,PUK错误!
                            //MMIPUB_OpenAlertWarningWin(TXT_PUK_ERROR);
                        	uint16 puk_remain_times = 0;

                            puk_remain_times = MNSIM_GetPinOrPukFalseRemainEx(sim_pin_cnf.dual_sys, MNSIM_PIN1, TRUE);//剩余次数	    
                            if(puk_remain_times == 1)
                            {
                                MMISET_DisplaybannerInfo(PNULL, STR_INVALID_PUK_TRY_EXT01, PNULL);
                            }
                            else
                            {
                                MMISET_DisplaybannerInfoPinPukErrorTryTimes(PNULL, STR_INVALID_PUK_TRIES_EXT01, sim_pin_cnf.dual_sys, sim_pin_cnf.pin_num, TRUE);
                            }

                            //清空password
                            //GUIEDIT_ClearAllStr(MMI_PUK_INPUT_EDITBOX_CTRL_ID);
                            GUIEDIT_ClearAllStr(pukCtrl_handle);
                        }
                    }
                    else//PIN2
                    {
                        if (sim_pin_cnf.unblock_pin_status.pin_blocked)
                        {
                            //提示错误窗口,PUK2被锁!
                            //MMIPUB_OpenAlertWarningWin(TXT_PUK2_BLOCKED);
                            MMISET_DisplaybannerInfo(PNULL, STR_SIM_SERVICE_BLOCKED_EXT01, PNULL);

                            MMK_CloseWin(MMI_PUK_INPUTING_WIN_ID);
                        }
                        else
                        {
                            //提示错误窗口,PUK2错误!
                            //MMIPUB_OpenAlertWarningWin(TXT_PUK2_ERROR);
                        	uint16 puk_remain_times = 0;

                            puk_remain_times = MNSIM_GetPinOrPukFalseRemainEx(sim_pin_cnf.dual_sys, MNSIM_PIN2, TRUE);//剩余次数	    
                            if(puk_remain_times == 1)
                            {
                                MMISET_DisplaybannerInfo(PNULL, STR_INVALID_PUK2_TRY_EXT01, PNULL);
                            }
                            else
                            {
                                MMISET_DisplaybannerInfoPinPukErrorTryTimes(PNULL, STR_INVALID_PUK2_TRIES_EXT01, sim_pin_cnf.dual_sys, sim_pin_cnf.pin_num, TRUE);
                            }
                            //清空password
                            //GUIEDIT_ClearAllStr(MMI_PUK_INPUT_EDITBOX_CTRL_ID);
                            GUIEDIT_ClearAllStr(pukCtrl_handle);
                        }
                    }
                }
                else
                {
                    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(MMI_PIN_INPUTING_WIN_ID);
                    MMI_OPERATE_PIN_T   *operate_pin_ptr = PNULL;

                    SCI_MEMSET(&s_operate_pin_blocked_info,0,sizeof(MMI_OPERATE_PIN_T));

                    if(PNULL != win_info_ptr)
                    {
                        operate_pin_ptr = (MMI_OPERATE_PIN_T *)win_info_ptr->user_data;//(MMI_OPERATE_PIN_T *)MMK_GetWinAddDataPtr(MMI_PIN_INPUTING_WIN_ID);
                        SCI_MEMCPY(&s_operate_pin_blocked_info,operate_pin_ptr,sizeof(MMI_OPERATE_PIN_T));
                    }

                    if (MNSIM_PIN1 == sim_pin_cnf.pin_num)//PIN1
                    {
                        //提示错误窗口,PIN被锁!
MMIPUB_CloseAlertWin();
                        MMIPUB_OpenAlertWinByTextId(PNULL,STR_PIN_BLOCKED_EXT01,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlockedExceptPhone);
                    }
                    else//PIN2
                    {
MMIPUB_CloseAlertWin();
                        MMIPUB_OpenAlertWinByTextId(PNULL,STR_PIN2_BLOCKED_EXT01,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlockedExceptPhone);
                    }
                }
            }
            else
            {
            	uint16 pin_remain_times = 0;

                if (MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID))
                {
                    //清空password
                    //GUIEDIT_ClearAllStr(MMI_PIN_INPUT_EDITBOX_CTRL_ID);
                    MMI_HANDLE_T pinCtrl_handle = MMK_GetCtrlHandleByWin(MMI_PIN_INPUTING_WIN_ID, MMIPUB_INPUTFIELD_CTRL_ID);
                    GUIEDIT_ClearAllStr(pinCtrl_handle);
                    if(MNSIM_PIN_CHANGE == sim_pin_cnf.pin_function_type)
                    {
                        MMK_HideWindow(MMI_PIN_INPUTING_WIN_ID, FALSE);
                    }
                }
                if (MNSIM_PIN1 == sim_pin_cnf.pin_num)//PIN1
                {
                    //MMIPUB_OpenAlertWarningWin(TXT_PIN_ERROR);
                    pin_remain_times = MNSIM_GetPinOrPukFalseRemainEx(sim_pin_cnf.dual_sys, MNSIM_PIN1, FALSE);//剩余次数	    
                    if(pin_remain_times == 1)
                    {
                        MMISET_DisplaybannerInfo(PNULL, STR_INVALID_PIN_TRY_EXT01, PNULL);
                    }
                    else
                    {
                        MMISET_DisplaybannerInfoPinPukErrorTryTimes(PNULL, STR_INVALID_PIN_TRIES_EXT01, sim_pin_cnf.dual_sys, sim_pin_cnf.pin_num, FALSE);
                    }
                }
                else
                {
                    //MMIPUB_OpenAlertWarningWin(TXT_PIN2_ERROR);
                    pin_remain_times = MNSIM_GetPinOrPukFalseRemainEx(sim_pin_cnf.dual_sys, MNSIM_PIN2, FALSE);//剩余次数	    
                    if(pin_remain_times == 1)
                    {
                        MMISET_DisplaybannerInfo(PNULL, STR_INVALID_PIN2_TRY_EXT01, PNULL);
                    }
                    else
                    {
                        MMISET_DisplaybannerInfoPinPukErrorTryTimes(PNULL, STR_INVALID_PIN2_TRIES_EXT01, sim_pin_cnf.dual_sys, sim_pin_cnf.pin_num, FALSE);
                    }
                }
            }

            // 关闭等待窗口
            MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
        }
        break;
        
    case IDLE_SPECIAL_WAIT_WIN_ID://idle win,input expecial string
        //SCI_TRACE_LOW:"HandleSimPinCnf:pin_operate_result=%d, pin_num=%d, pin_function_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4267_112_2_18_2_45_8_93,(uint8*)"ddd",sim_pin_cnf.pin_operate_result, sim_pin_cnf.pin_num, sim_pin_cnf.pin_function_type);

        switch (sim_pin_cnf.pin_operate_result)
        {
        case MNSIM_PIN_REQ_OK:
            if (MNSIM_PIN_UNBLOCK == sim_pin_cnf.pin_function_type)
            {
                MMI_TEXT_ID_T text1_id = TXT_NULL;
                if(MNSIM_PIN1 == sim_pin_cnf.pin_num)
                {
                    text1_id = STR_PIN_UNLOCKED;
                }
                else if(MNSIM_PIN2 == sim_pin_cnf.pin_num)
                {
                    text1_id = STR_PIN2_UNLOCKED;
                }
                MMIPUB_OpenAlertSuccessWin(text1_id);
            }
            else if(MNSIM_PIN_CHANGE == sim_pin_cnf.pin_function_type)
            {
                if (MNSIM_PIN1 == sim_pin_cnf.pin_num)
                {
                    //MMIPUB_OpenAlertSuccessWin(TXT_PIN_CHANGED);
                    MMISET_DisplaybannerInfo(PNULL, STXT_PIN_CHANGED, PNULL);
                }
                else
                {
                    //MMIPUB_OpenAlertSuccessWin(TXT_PIN2_CHANGED);
                    MMISET_DisplaybannerInfo(PNULL, STXT_PIN2_CHANGED, PNULL);
                }
            }  
            break;

        case MNSIM_PIN_ERROR_WITH_BLOCKED:
            if (MNSIM_PIN1 == sim_pin_cnf.pin_num)
            {
                if (sim_pin_cnf.unblock_pin_status.pin_blocked)
                {
                    //MMIPUB_OpenAlertWarningWin(TXT_PUK_BLOCKED);
                    MMISET_DisplaybannerInfo(PNULL, STR_SIM_BLOCKED_EXT01, PNULL);
                }
                else
                {
                    SCI_MEMSET(&s_operate_pin_blocked_info,0,sizeof(MMI_OPERATE_PIN_T));
                    s_operate_pin_blocked_info.operate_type = sim_pin_cnf.pin_function_type;
                    s_operate_pin_blocked_info.pin_num = sim_pin_cnf.pin_num;

MMIPUB_CloseAlertWin();
                    MMIPUB_OpenAlertWinByTextId(PNULL,STR_PIN_BLOCKED_EXT01,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlockedExceptPhone);
                }
            }
            else
            {
                if (sim_pin_cnf.unblock_pin_status.pin_blocked)
                {
                    //MMIPUB_OpenAlertWarningWin(TXT_PUK2_BLOCKED);
                    MMISET_DisplaybannerInfo(PNULL, STR_SIM_SERVICE_BLOCKED_EXT01, PNULL);
                }
                else
                {
                    SCI_MEMSET(&s_operate_pin_blocked_info,0,sizeof(MMI_OPERATE_PIN_T));
                    s_operate_pin_blocked_info.operate_type = sim_pin_cnf.pin_function_type;
                    s_operate_pin_blocked_info.pin_num = sim_pin_cnf.pin_num;
MMIPUB_CloseAlertWin();
                    MMIPUB_OpenAlertWinByTextId(PNULL,STR_PIN2_BLOCKED_EXT01,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlockedExceptPhone);
                }
            }
            break;

        case MNSIM_PIN_ERROR_CONTRADICT_STATUS:
            if (MNSIM_PIN1 == sim_pin_cnf.pin_num)
            {
                MMIPUB_OpenAlertWarningWin(STR_ACTIVATE_PIN);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ENABLE_PIN2);
            }
            break;

        case MNSIM_PIN_ERROR_PIN_FAIL:
            if (MNSIM_PIN1 == sim_pin_cnf.pin_num)
            {
                if (MNSIM_PIN_UNBLOCK == sim_pin_cnf.pin_function_type)
                {
                    //MMIPUB_OpenAlertWarningWin(TXT_PUK_ERROR);
                	uint16 puk_remain_times = 0;

                    puk_remain_times = MNSIM_GetPinOrPukFalseRemainEx(sim_pin_cnf.dual_sys, MNSIM_PIN1, TRUE);//剩余次数	    
                    if(puk_remain_times == 1)
                    {
                        MMISET_DisplaybannerInfo(PNULL, STR_INVALID_PUK_TRY_EXT01, PNULL);
                    }
                    else
                    {
                        MMISET_DisplaybannerInfoPinPukErrorTryTimes(PNULL, STR_INVALID_PUK_TRIES_EXT01, sim_pin_cnf.dual_sys, sim_pin_cnf.pin_num, TRUE);
                    }
                }
                else
                {
                    //MMIPUB_OpenAlertWarningWin(TXT_PIN_ERROR);
                	uint16 pin_remain_times = 0;

                    pin_remain_times = MNSIM_GetPinOrPukFalseRemainEx(sim_pin_cnf.dual_sys, MNSIM_PIN1, FALSE);//剩余次数	    
                    if(pin_remain_times == 1)
                    {
                        MMISET_DisplaybannerInfo(PNULL, STR_INVALID_PIN_TRY_EXT01, PNULL);
                    }
                    else
                    {
                        MMISET_DisplaybannerInfoPinPukErrorTryTimes(PNULL, STR_INVALID_PIN_TRIES_EXT01, sim_pin_cnf.dual_sys, sim_pin_cnf.pin_num, FALSE);
                    }
                }
            }
            else
            {
                if (MNSIM_PIN_UNBLOCK == sim_pin_cnf.pin_function_type)
                {
                    //MMIPUB_OpenAlertWarningWin(TXT_PUK2_ERROR);
                	uint16 puk_remain_times = 0;

                    puk_remain_times = MNSIM_GetPinOrPukFalseRemainEx(sim_pin_cnf.dual_sys, MNSIM_PIN2, TRUE);//剩余次数	    
                    if(puk_remain_times == 1)
                    {
                        MMISET_DisplaybannerInfo(PNULL, STR_INVALID_PUK2_TRY_EXT01, PNULL);
                    }
                    else
                    {
                        MMISET_DisplaybannerInfoPinPukErrorTryTimes(PNULL, STR_INVALID_PUK2_TRIES_EXT01, sim_pin_cnf.dual_sys, sim_pin_cnf.pin_num, TRUE);
                    }
                }
                else
                {
                    //MMIPUB_OpenAlertWarningWin(TXT_PIN2_ERROR);
                	uint16 pin_remain_times = 0;

                    pin_remain_times = MNSIM_GetPinOrPukFalseRemainEx(sim_pin_cnf.dual_sys, MNSIM_PIN2, FALSE);//剩余次数	    
                    if(pin_remain_times == 1)
                    {
                        MMISET_DisplaybannerInfo(PNULL, STR_INVALID_PIN2_TRY_EXT01, PNULL);
                    }
                    else
                    {
                        MMISET_DisplaybannerInfoPinPukErrorTryTimes(PNULL, STR_INVALID_PIN2_TRIES_EXT01, sim_pin_cnf.dual_sys, sim_pin_cnf.pin_num, FALSE);
                    }
                }
            }
            break;

        default:
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            break;
        }
        // 关闭等待窗口
        MMK_CloseWin(IDLE_SPECIAL_WAIT_WIN_ID);
        // close idle dialing win
        MMIAPIIDLE_CloseDialWin();
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    //if assert reset and pin operate fale, enter pin input win
#ifdef MMI_RESET_PHONE_AUTO_PIN
    {
        POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();
        
        if((RESTART_BY_ASSERT == restart_condition) &&((sim_pin_cnf.pin_operate_result) != MNSIM_PIN_REQ_OK))
        {
            if(!MMK_IsOpenWin(PHONE_PIN_WAIT_WIN_ID) && !MMK_IsOpenWin(IDLE_SPECIAL_WAIT_WIN_ID))
            {
                APP_MN_SIM_GET_PIN_IND_T         sim_get_pin_ind = {0};
                MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                uint32 i = 0;
                BOOLEAN is_need_handle = FALSE;
#endif
                //set pin ind
                sim_get_pin_ind.dual_sys = sim_pin_cnf.dual_sys;
                sim_get_pin_ind.pin_num = sim_pin_cnf.pin_num;
                sim_get_pin_ind.pin_is_blocked = sim_pin_cnf.pin_status.pin_blocked;
                sim_get_pin_ind.remain_times = sim_pin_cnf.pin_status.false_remain;
                
                if(sim_get_pin_ind.dual_sys >= MMI_DUAL_SYS_MAX)
                {
                    //SCI_TRACE_LOW:"HandleSimPinInd:error dual_sys=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4084_112_2_18_2_45_8_92,(uint8*)"d", sim_get_pin_ind.dual_sys);
                    return MMI_RESULT_FALSE;
                }

                if (e_dualsys_setting & (0x01 << sim_get_pin_ind.dual_sys))
                {
                    PowerOnPsForEmerge(sim_get_pin_ind.dual_sys);
                }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
                s_handle_pin_status.sim_pin_ind[sim_get_pin_ind.dual_sys] = sim_get_pin_ind;

                for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if(( i != sim_get_pin_ind.dual_sys)&&(s_handle_pin_status.is_handling_pin[i]))
                    {
                        is_need_handle = TRUE;
                        break;            
                    }
                }
                
                if(is_need_handle)
                {
                    s_handle_pin_status.need_handle_pin[sim_get_pin_ind.dual_sys] = TRUE;
            		
                    return result;
                }
                else
                {            
                    s_handle_pin_status.is_handling_pin[sim_get_pin_ind.dual_sys] = TRUE;   //set the status (lock)
                    s_handle_pin_status.cur_handling_sim_id = sim_get_pin_ind.dual_sys;
                }    
#endif
                CreateInputPinWindow(sim_get_pin_ind);
            }
        }
    }
#endif    
   
    return (result);
}

/*****************************************************************************/
// 	Description : 在提示还可以 输入多少次密码后，启动pin输入框
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePin1Maxtimes(
                                      MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM 		        param		// 相应消息的参数
                                      )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        // close input pin window
        MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);
        
        // 打开输入PIN的界面
        MMK_CreatePubEditWin((uint32*)PHONE_PIN_INPUTING_WIN_TAB,PNULL);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_LOSE_FOCUS:
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}


/*****************************************************************************/
// 	Description : 在pin blocked的情况下关闭窗口，则启动输入puk的窗口
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinBlocked(
                                    MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                    DPARAM 		        param		// 相应消息的参数
                                    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO	lcd_dev_info = {0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        // close input pin window
        MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);

        MMK_CloseWin(PHONE_PUK_INPUT_WIN_ID);
        // 打开输入PUK的界面
        MMK_CreatePubEditWin((uint32*)PHONE_PUK_INPUTING_WIN_TAB,PNULL);
        break;

    case MSG_APP_CANCEL://pin被锁输入界面不允许返回
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMK_CloseWin(win_id);
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 在pin1或者pin2 blocked的情况下关闭窗口，则启动输入puk的窗口
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinBlockedExceptPhone(
                                               MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                               MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                               DPARAM 		        param		// 相应消息的参数
                                               )
{
    MMI_OPERATE_PIN_T   *operate_pin_ptr = &s_operate_pin_blocked_info;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CLOSE_WINDOW:
        {
                MMI_WIN_ID_T    pukWinId = MMI_PUK_INPUTING_WIN_ID;
            MMI_OPERATE_PIN_T   *new_opreate_pin_ptr = SCI_ALLOC_APP(sizeof(MMI_OPERATE_PIN_T));
            
            MMI_MEMCPY(new_opreate_pin_ptr,
                sizeof(MMI_OPERATE_PIN_T),
                operate_pin_ptr,
                sizeof(MMI_OPERATE_PIN_T),
                sizeof(MMI_OPERATE_PIN_T));
            
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            
            new_opreate_pin_ptr->operate_type = MNSIM_PIN_UNBLOCK;
            // 打开输入PUK的界面
            //MMK_CreatePubEditWin((uint32*)MMI_PUK_INPUTING_WIN_TAB,(ADD_DATA)new_opreate_pin_ptr);
                MMIPUB_OpenDialogPasswordWinByTextPtrEx(
                                    MMK_GetFirstAppletHandle(),
                                    PNULL,
                                    &pukWinId,
                                    PNULL,
                                    MMIPUB_SOFTKEY_OKEXIT,
                                    HandlePukInputExceptPhone,
                                    (ADD_DATA)new_opreate_pin_ptr);
            
            // close input pin window
            MMK_CloseWin(MMI_PIN_INPUTING_WIN_ID);
        }
        break;
        
    case MSG_APP_CANCEL://pin被锁输入界面不允许返回
    //case MSG_APP_RED:
        //MMK_CloseWin(MMI_PIN_INPUTING_WIN_ID);
        //MMK_CloseWin(MMI_NEW_PIN_INPUTING_WIN_ID);
        //MMK_CloseWin(MMI_NEW_PIN_AGAIN_INPUTING_WIN_ID);
        MMK_CloseWin(win_id);
        break;

    case MSG_LOSE_FOCUS:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMK_CloseWin(win_id);
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_READ_SIM_EF_CNF
//	Global resource dependence : 
//    Author: Tong Yongli
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimEFReadInd(
                                     DPARAM              param
                                     )
{
#ifdef MMI_SMSCB_SUPPORT
    APP_MN_SIM_READ_CNF_T sim_read_ind = *((APP_MN_SIM_READ_CNF_T *)param);
	MN_CB_ALERT_MSG_T t_cbmi_info;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    uint8 i;
    char *pcStr = NULL;

    if(MN_SIM_FILE_READ_OK != sim_read_ind.result)
    {
        SCI_TRACE_LOW("sim_read_ind.result = %d.",sim_read_ind.result);
        return MMI_RESULT_FALSE;
    }
    //SCI_TRACE_LOW("HandleSimEFReadInd data_length = %d, num_records = %d, = %s.dual_sys = %d.", sim_read_ind.read_data.data_length, sim_read_ind.read_data.num_records, pcStr, sim_read_ind.dual_sys);
    //for(i = 0; i < sim_read_ind.read_data.data_length; i++)
    //    SCI_TRACE_LOW("[index: %d] 0x%x", i, *(sim_read_ind.read_data.data_ptr + i));
    t_cbmi_info.sim_cbmi_num = (uint8)(sim_read_ind.read_data.data_length / 2);
    t_cbmi_info.emegence_num = MMI_CB_ALERT_NUM;
    for(i = 0; i < t_cbmi_info.sim_cbmi_num; i++)
    {
        t_cbmi_info.cb_alert_content[t_cbmi_info.emegence_num + i].cb_alert_id
            = (MN_CB_MSG_ID_T)(*(sim_read_ind.read_data.data_ptr + i * 2) << 8) + (MN_CB_MSG_ID_T)*(sim_read_ind.read_data.data_ptr + i * 2 + 1);
    }
    MMIAPISMSCB_StoreCBMIValue(&t_cbmi_info, sim_read_ind.dual_sys);
#else
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
#endif
	return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_READY_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimReadyInd(
                                     DPARAM              param
                                     )
{
    APP_MN_SIM_READY_IND_T      sim_ready_ind   = *((APP_MN_SIM_READY_IND_T *)param);
    MMI_WIN_ID_T     window_id       = MMK_GetFocusWinId();
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    uint8 index = 0;
    uint8 index_ia = 0;
    //uint8 index_ims = 0;
    uint32 i = 0;
    BOOLEAN is_handling_pin = FALSE;
    MMI_STRING_T   pin_value = {0};   
	MMICOM_BANNER_INFO_T banner_info = {0};
    MN_SIM_EF_MAPPING_GID_T gid1 = {0};
    char tstr[64] = {0};
    MN_IMSI_T imsi = {0};
    char tstr1[64] = {0};
    MN_PLMN_T carrier_hplmn[MMI_DUAL_SYS_MAX] = {0};
	GUI_POINT_T pin_alert_start_point = {0, 0};
    MMICONNECTION_LINKSETTING_DETAIL_T* link_ia_setting_ptr = PNULL; 
    //MMICONNECTION_LINKSETTING_DETAIL_T* link_ims_setting_ptr = PNULL; 
    MMICONNECTION_LINKSETTING_DETAIL_T* link_xcap_setting_ptr = PNULL; 
    MMICONNECTION_APNTYPEINDEX_T * apntype_index = PNULL;
	MMICONNECTION_SETTING_T *connection_setting_info = NULL;
	MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
#ifdef UNISOC_MMI_SIM_LOCK_SUPPORT
    MN_IMSI_T           cur_imsi_info = {0};
    MN_SIM_LOCK_STATUS_T sim_unlock_status = {0};	
#endif
    if (sim_ready_ind.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return result;
    }
    SCI_TRACE_LOW("HandleSimReadyInd, mcc=%d, mnc=%d, mnc_digit_num=%d", sim_ready_ind.hplmn.mcc, sim_ready_ind.hplmn.mnc, sim_ready_ind.hplmn.mnc_digit_num);
#ifdef WIN32
	sim_ready_ind.hplmn.mcc = 202;//460;
	sim_ready_ind.hplmn.mnc = 9;//0;
	sim_ready_ind.hplmn.mnc_digit_num = 2;
#endif
#ifdef SIM_LOCK_CT_SUPPORT
    g_smartcard_status_t[sim_ready_ind.dual_sys].sim_ready = 1;
    if(MMICDMA_GetSmartCard_NeedCheck(sim_ready_ind.dual_sys) && FALSE == MMICDMA_GetSmartCard_Over(sim_ready_ind.dual_sys))
    {
        if(SmartCard_IsCdma(sim_ready_ind.hplmn,sim_ready_ind.dual_sys))
        {
            Trace_Log_Print("PS: HandleSimReadyInd Is CDMA");
            MMICDMA_HandleUimPresent(sim_ready_ind.dual_sys,g_smartcard_status_t[sim_ready_ind.dual_sys].sim_present,g_smartcard_status_t[sim_ready_ind.dual_sys].sms_num);
            g_smartcard_status_t[sim_ready_ind.dual_sys].is_cdma = TRUE;
            MMIPhone_CDMA_SimPowerOver(sim_ready_ind.dual_sys);
        }
        else
        {
            Trace_Log_Print("PS: HandleSimReadyInd Is GSM");
            g_smartcard_status_t[sim_ready_ind.dual_sys].is_cdma = FALSE;
            return result;
        }
    }
#endif


#ifdef UNISOC_MMI_SIM_LOCK_SUPPORT    
    MMINV_READ(MMI_NV_SIM_LOCK_INFO_ID, &g_sim_lock_info, return_value);
    //换卡，重置
    if(MN_RETURN_SUCCESS == return_value)
    {
        cur_imsi_info = MNSIM_GetImsiEx(sim_ready_ind.dual_sys);
        //不一致
        if((MMIAPICOM_StrCmp(cur_imsi_info.imsi_val, cur_imsi_info.imsi_len, g_sim_lock_info.imsi_info.imsi_val, g_sim_lock_info.imsi_info.imsi_len)))
        {

            if(MNPHONE_GetSIMLockStatus(&sim_unlock_status))
            {
                g_sim_lock_info.counter = sim_unlock_status.max_num_trials;
            }
            else
            {
                g_sim_lock_info.counter = MAX_SIM_LOCK_NUMBER;
            }
            
            g_sim_lock_info.imsi_info = cur_imsi_info;
			g_sim_lock_info.cur_sim_id = sim_ready_ind.dual_sys;
            SCI_TRACE_LOW("mmiphone.c HandleSimLockInfo nv_len=%d,nv_imsi=%d%d%d%d%d%d%d%d",\
                g_sim_lock_info.imsi_info.imsi_len,\
                g_sim_lock_info.imsi_info.imsi_val[0],g_sim_lock_info.imsi_info.imsi_val[1],g_sim_lock_info.imsi_info.imsi_val[2],\
                g_sim_lock_info.imsi_info.imsi_val[3],g_sim_lock_info.imsi_info.imsi_val[4],g_sim_lock_info.imsi_info.imsi_val[5],\
                g_sim_lock_info.imsi_info.imsi_val[6],g_sim_lock_info.imsi_info.imsi_val[7]);


            MMINV_WRITE(MMI_NV_SIM_LOCK_INFO_ID, &g_sim_lock_info);
        }
    }   
#endif
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    SCI_TRACE_LOW("HandleSimReadyInd(),sim_lock, dual_sys[%d]",sim_ready_ind.dual_sys);
    if(!s_is_simlock_off)
    {
        if (MN_DUAL_SYS_1 == sim_ready_ind.dual_sys)
        {
#ifdef MMI_MULTI_SIM_SYS_DUAL
            //只有sim1是在开机初始化的时候进行power，
            // 其它的sim卡都需要等sim1启动之后才可以启动。
            MN_DUAL_SYS_E power_sim_index = MN_DUAL_SYS_2;
#endif
            s_is_sim1_poweron = TRUE;//        
            if (MMIPHONE_IsLegalSim(&sim_ready_ind))
            {
                 s_is_sim1_right = TRUE;
            }
#ifdef MMI_MULTI_SIM_SYS_DUAL
            // 该段是对sim的启动时序进行修改，
            // 需要配合MMIAPIPHONE_StartUpPsAccordingToSetting 接口中相关接口进行改动。
            // 解决SIM PIN 码输入的情况下，sim  lock 处理有问题.
            if(!s_is_sim2_poweron)
            {
                MNPHONE_PowerOnSimEx(power_sim_index, TRUE);
            }
#endif
        }
#ifdef MMI_MULTI_SIM_SYS_DUAL 
        else if ( MN_DUAL_SYS_2 == sim_ready_ind.dual_sys)
        {         
            s_is_sim2_poweron = TRUE;
            if (MMIPHONE_IsLegalSim(&sim_ready_ind))
            {
                 s_is_sim2_right = TRUE;
            }
        }
#endif
        else
        {
            //do not support SIM3 or SIM4,just for DUAL_SIM Project.
            SCI_TRACE_LOW("HandleSimReadyInd ERROR!do not support sim_lock in TRISIM or QUDASIM config");
        }
    }
#endif
    // 保存 SIM 状态信息
    StoreSimReadyStatus(&sim_ready_ind);
    if (!s_is_same_sim[sim_ready_ind.dual_sys])
    {
        MMIAPISET_ClearVodafoneFlagForNewSIM(sim_ready_ind.dual_sys);
    }
    
    MMIAPIPHONE_SetSimExistedStatus(sim_ready_ind.dual_sys, TRUE);
#ifdef MMI_NETWORK_NAME_SUPPORT
        //get PNN num when SIM ready
        MMIPHONE_GetPNNum(sim_ready_ind.dual_sys);
#endif
#ifdef MMI_GPRS_SUPPORT
#ifdef MMI_SIM_NODIGITNUM_SUPPORT
    SCI_TRACE_LOW("mmiphone.c HandleNetworkStatusInd:sim_ready_ind.hplmn.mnc_digit_num= %d", sim_ready_ind.hplmn.mnc_digit_num);
    if(MCC_MNC_DIGIT_INVALID == sim_ready_ind.hplmn.mnc_digit_num)
    {
        s_is_autoadapt_flag[sim_ready_ind.dual_sys] = TRUE;  
    } 
    
    if (s_is_autoadapt_flag[sim_ready_ind.dual_sys])
    {
        SCI_TRACE_LOW("mmiphone.c HandleSimReadyInd:s_sim_status.mcc= %d,s_sim_status.mnc= %d,s_sim_status.mnc_digit_num= %d", 
                       s_sim_status[sim_ready_ind.dual_sys].hplmn.mcc,s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc ,s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num);
        
        // correct sim mnc and digtal len
        if(404 == s_sim_status[sim_ready_ind.dual_sys].hplmn.mcc) 
        {
            
            s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc /= 10 ;
            
            s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 2;
        }
        else if(405 == s_sim_status[sim_ready_ind.dual_sys].hplmn.mcc)
        {
            if(s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc > 249 && s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc < 480)
            {
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc /= 10 ;
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 3;
            }
            else if(s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc > 749)
            {
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 3;
            }
            else if(s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc >= 100 && s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc < 750 )   //bug 522282, according to Indian operator's details.
            {
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc /= 10;  
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 2;
            }
            else            
            {    
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 2;
            }           
        }
        else
        {
            s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc /= 10 ;
            s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 2;
        }   
        s_is_autoadapt_flag[sim_ready_ind.dual_sys] = FALSE;
    }
#endif
#endif

// add for Vodafone to set reduce sms codec
// when SIM0 insert, use SIM0 MCC/MNC as primary SIM
// When SIMo not insert, use SIM1 MCC/MNC as primary SIM
#if 1
	if(MMIAPISET_IsVodafoneVersion())
	{
		uint16 t_mcc=s_sim_status[sim_ready_ind.dual_sys].hplmn.mcc;
		uint16 t_mnc=s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc;
		
		SCI_TRACE_LOW("HandleSimReadyInd: reduce SMS mcc=%d,mnc=%d ",t_mcc,t_mnc);
		
		if((((268==t_mcc)&&(1==t_mnc)) 
		||((202==t_mcc)&&(5==t_mnc))
		||((216==t_mcc)&&(70==t_mnc))
	#if 0 // For test by cmcc
		||((460==t_mcc)&&(0==t_mnc))
		||((460==t_mcc)&&(1==t_mnc))
		||((460==t_mcc)&&(2==t_mnc))
	#endif
		)&&((0==sim_ready_ind.dual_sys)||((1==sim_ready_ind.dual_sys)&&(SIM_STATUS_NO_SIM==s_sim_status[0].simStatus)))
		)
		{
			MMISMS_SetAlphabetManagementEx(0);
		}
    }
#endif

    MNSIM_GetGidData(sim_ready_ind.dual_sys, MNSIM_GID1, &gid1);
    SCI_TRACE_BUF("[szp]gid1 tele_num.number:", gid1.gid, gid1.len);
    
    MNPHONE_GetImsiEx(sim_ready_ind.dual_sys, &imsi);
    SCI_TRACE_BUF("[szp]imsi.number:", imsi.imsi_val, imsi.imsi_len);
    TransformBcdToStr(imsi.imsi_val,imsi.imsi_len, (uint8 *)tstr1);
    SCI_TRACE_LOW("[szp]imsi.number: %s", tstr1);
    
    MMIAPICONNECTION_SetPNNAPNExist(sim_ready_ind.dual_sys, FALSE, TRUE);
    MMIAPICONNECTION_AutoAdapting(sim_ready_ind.dual_sys);
    
    connection_setting_info = MMIMRAPP_GetConnetInfo();
    if ((NULL != connection_setting_info) && (0 == connection_setting_info->settinglink_num[sim_ready_ind.dual_sys]))
    {
	    MMINV_READ(MMINV_CARRIER_PLMN, carrier_hplmn, return_value);

		if (return_value != MN_RETURN_SUCCESS)
		{
			SCI_MEMSET(carrier_hplmn, 0x00, sizeof(carrier_hplmn));
		}
		else
		{
			carrier_hplmn[sim_ready_ind.dual_sys].mcc = 0x0;
			carrier_hplmn[sim_ready_ind.dual_sys].mnc = 0x0;
		}
        MMINV_WRITE(MMINV_CARRIER_PLMN, carrier_hplmn);
        SCI_TRACE_LOW("[szp]ReAutoAdapting. 0");
        MMIAPICONNECTION_AutoAdapting(sim_ready_ind.dual_sys);
    }
    
    apntype_index = MMIAPICONNECTION_GetApnTypeIndex(sim_ready_ind.dual_sys);
    if (NULL != apntype_index)
    {
        if (TRUE == apntype_index->xcap_index[sim_ready_ind.dual_sys].is_valid)
        {
            index = apntype_index->xcap_index[sim_ready_ind.dual_sys].index;
            link_xcap_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_ready_ind.dual_sys, index); 
        }
        else if (TRUE == apntype_index->internet_index[sim_ready_ind.dual_sys].is_valid)
        {
            index = apntype_index->internet_index[sim_ready_ind.dual_sys].index;
            link_xcap_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_ready_ind.dual_sys, index); 
        }
        else
        {
            SCI_TRACE_LOW("[szp]MMINV_CONNECTION_APN_INDEX ERROR!!!");
        }

        if (TRUE == apntype_index->ia_index[sim_ready_ind.dual_sys].is_valid)
        {
            index_ia = apntype_index->ia_index[sim_ready_ind.dual_sys].index;
            link_ia_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_ready_ind.dual_sys, index_ia); 
        }
        
        /*
        if (TRUE == apntype_index->ims_index[sim_ready_ind.dual_sys].is_valid)
        {
            index_ims = apntype_index->ia_index[sim_ready_ind.dual_sys].index;
            link_ims_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_ready_ind.dual_sys, index_ims); 
        }
        */
        
        if (NULL != link_xcap_setting_ptr)
        {
            MMIAPIPDP_ActiveInternalPdp(sim_ready_ind.dual_sys, link_xcap_setting_ptr);
        }
        SCI_TRACE_LOW("[szp]MMINV_CONNECTION_APN_INDEX xcap_index[%d]=%d,is_valid=%d",
                      sim_ready_ind.dual_sys, apntype_index->xcap_index[sim_ready_ind.dual_sys].index,apntype_index->xcap_index[sim_ready_ind.dual_sys].is_valid);
        if (NULL != link_ia_setting_ptr)
        {
            MMIAPIPDP_ActiveInternalPdp(sim_ready_ind.dual_sys, link_ia_setting_ptr);
        }
        SCI_TRACE_LOW("[szp]MMINV_CONNECTION_APN_INDEX ia_index[%d]=%d,is_valid=%d",
                      sim_ready_ind.dual_sys, apntype_index->ia_index[sim_ready_ind.dual_sys].index,apntype_index->ia_index[sim_ready_ind.dual_sys].is_valid);
        /*
        if (NULL != link_ims_setting_ptr)
        {
            MMIAPIPDP_ActiveInternalPdp(sim_ready_ind.dual_sys, link_ims_setting_ptr);
        }
        SCI_TRACE_LOW("[szp]MMINV_CONNECTION_APN_INDEX ims_index[%d]=%d,is_valid=%d",
                      sim_ready_ind.dual_sys, apntype_index->ims_index[sim_ready_ind.dual_sys].index,apntype_index->ims_index[sim_ready_ind.dual_sys].is_valid);
        */
    }
    
    MMISET_SetIsVMFirstQueryFromSIM(sim_ready_ind.dual_sys, TRUE);
    result = MNSMS_GetVoiMessAddrEx(sim_ready_ind.dual_sys, MN_MULTI_SUBSCRIBER_ID_1, MN_VOICE_MAIL);      
    SCI_TRACE_LOW("[szp]MNSMS_GetVoiMessAddrEx,result = %d, line_type= %d", sim_ready_ind.dual_sys, result);  
    
#if defined(UNISOC_MMI_SIM_LOCK_SUPPORT)
    if(MMK_IsOpenWin( PHONE_SIM_LOCK_INPUTING_CODE_WIN_ID)
        && !MMK_IsFocusWin(PHONE_SIM_LOCK_INPUTING_CODE_WIN_ID))
    {
        SCI_TraceLow("HandleSimReadyInd  Show");
        MMK_HideWindow(PHONE_SIM_LOCK_INPUTING_CODE_WIN_ID, FALSE);     
    }
#elif defined(MMI_SIM_LOCK_SUPPORT)
    if(MMK_IsOpenWin( PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
        && !MMK_IsFocusWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID))
    {
        MMK_HideWindow(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID, FALSE);     
    }
#endif
#ifdef MMI_ORTHODOX_VERSION
    InitSet2G4GMode(sim_ready_ind.dual_sys);
#endif
#ifdef MMI_VOLTE_SUPPORT
    MMIAPIPHONE_SetImsStatus(sim_ready_ind.dual_sys, s_sim_status[sim_ready_ind.dual_sys].hplmn);
    MMIAPISET_InitImsValue(sim_ready_ind.dual_sys);
#endif	
    if ( (MMK_IsOpenWin(PHONE_PIN_WAIT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_NEW_PIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
        && s_handle_pin_status.cur_handling_sim_id == sim_ready_ind.dual_sys)
    {
        window_id = PHONE_PIN_WAIT_WIN_ID;
        
        if (MMK_IsOpenWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID)
			|| MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))//记录下正确的pin码
        {
            GUIEDIT_GetString(PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID, &pin_value);
            g_pin_value[sim_ready_ind.dual_sys].blocklen = pin_value.wstr_len;

            MMI_WSTRNTOSTR(
                g_pin_value[sim_ready_ind.dual_sys].blockbuf,
                MN_MAX_BLOCK_LEN,
                pin_value.wstr_ptr,
                g_pin_value[sim_ready_ind.dual_sys].blocklen,
                g_pin_value[sim_ready_ind.dual_sys].blocklen
                );
		   MMIAPIPHONE_SetPukRemainStatus(1);
		   banner_info.banner_type = MMI_BANNER_TEXT;
	       banner_info.text1_id = STR_START_PUK_APPR_EXT01;//text需要加载资源,输入正确提示
		   banner_info.start_point = pin_alert_start_point;
            //bug 756003
            #if 0
	       MMIAPICOM_OpenBannerWin(PHONE_PUK_INPUT_WIN_ID, banner_info);
            #endif
            //bug 756003
        }
        else if (MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)) //记录下正确的pin码
        {
            GUIEDIT_GetString(PHONE_PIN_EDITBOX_CTRL_ID, &pin_value);
            g_pin_value[sim_ready_ind.dual_sys].blocklen = pin_value.wstr_len;

            MMI_WSTRNTOSTR(
                g_pin_value[sim_ready_ind.dual_sys].blockbuf,
                MN_MAX_BLOCK_LEN,
                pin_value.wstr_ptr,
                g_pin_value[sim_ready_ind.dual_sys].blocklen,
                g_pin_value[sim_ready_ind.dual_sys].blocklen
                );
		   banner_info.banner_type = MMI_BANNER_TEXT;
	       banner_info.text1_id = STR_START_PIN_APPR_EXT01;//text需要加载资源,输入正确提示
		   banner_info.start_point = pin_alert_start_point;
	       s_pin_input_ok_status = FALSE;
	       SCI_TraceLow("HandleSimReadyInd s_pin_input_ok_status =%d ",s_pin_input_ok_status);
	       MMIAPICOM_OpenBannerWin(PHONE_PIN_INPUT_WIN_ID, banner_info);
        }
      
#ifdef MMI_RESET_PHONE_AUTO_PIN
        //set sim pin to nv
        MMINV_WRITE(MMINV_SIM_PIN_VALUE, g_pin_value);
#endif       

        if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[sim_ready_ind.dual_sys])
        {
            g_service_status[sim_ready_ind.dual_sys].is_slecting = TRUE;
        }

        //关闭pin和puk输入窗口
        MMK_CloseWin(PHONE_PIN_ALERT_WIN_ID);
        MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_PUK_INPUT_WIN_ID);
    }
    else if (MMK_IsOpenWin(IDLE_SPECIAL_WAIT_WIN_ID)) 
	{
	     window_id = IDLE_SPECIAL_WAIT_WIN_ID;		
	}			
        
    //add for dual sys
    s_handle_pin_status.need_handle_pin[sim_ready_ind.dual_sys] = FALSE;
    s_handle_pin_status.is_handling_pin[sim_ready_ind.dual_sys] = FALSE;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (s_handle_pin_status.is_handling_pin[i])
        {
            is_handling_pin = TRUE;
            break;
        }
    }
    
    if (!is_handling_pin)
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (i != sim_ready_ind.dual_sys)
            {
                if (s_handle_pin_status.need_handle_pin[i])
                {
                    //set s_handle_pin_status
                    s_handle_pin_status.cur_handling_sim_id = (MN_DUAL_SYS_E)i;
                    s_handle_pin_status.is_handling_pin[i] = TRUE;
                    /* reset  s_pin_input_ok_status to FALSE, before handle other SIM*/
                    s_pin_input_ok_status = FALSE;
                    //create input pin window
                    CreateInputPinWindow(s_handle_pin_status.sim_pin_ind[i]);
                    return result;
                }
            }
        }
    }
    
    // 如果在输入PIN或者PUK之后，收到该消息
    switch (window_id)
    {
    case PHONE_PIN_WAIT_WIN_ID:
	{
		MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
        // 关闭等待窗口        
        if (s_handle_pin_status.cur_handling_sim_id == sim_ready_ind.dual_sys)
        {
            MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
        }
		// Bug 1351696, 输入pin若只插入一张被禁用sim卡，输入pin完后关闭紧急呼叫用的协议栈
		if (!(e_dualsys_setting & (0x01 << sim_ready_ind.dual_sys)) && 
			MMIAPIPHONE_GetSimExistedNum(PNULL, 0) == 1 && 
			s_ps_ready[sim_ready_ind.dual_sys] == MMIPHONE_PS_POWER_ON_EMERGE)
		{
			MMIAPIPHONE_PowerOffPs(sim_ready_ind.dual_sys);
		}
        break;
	}
    case IDLE_SPECIAL_WAIT_WIN_ID:
        break;
        
    default:
        //关闭pin和puk输入窗口
        if (s_handle_pin_status.cur_handling_sim_id == sim_ready_ind.dual_sys)
        {
            if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[sim_ready_ind.dual_sys])
            {
                g_service_status[sim_ready_ind.dual_sys].is_slecting = TRUE;
            }

            MMK_CloseWin(PHONE_PIN_ALERT_WIN_ID);   
 //           MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);
            MMK_CloseWin(PHONE_NEW_PIN_INPUT_WIN_ID);
            MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
 //           MMK_CloseWin(PHONE_PUK_INPUT_WIN_ID);
            MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
        }
        break;
    }
    
#if 0 //def MMI_SIM_LOCK_SUPPORT
    //update sim lock data in case,the data is changed in MN layer
    MMIPHONE_UpdateSIMUnlockData();
    //第一次开机,探测到符合条件的卡,打开SP Lock, 提示"SP Lock now on"
    if(MNPHONE_OrangeAutoLockEx(sim_ready_ind.dual_sys))
    {
        MMIPUB_OpenAlertSuccessWin(TXT_SIM_LOCK_SP_LOCK_ON);
    }
#endif    
    
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    //SIM ready,and load local default to system
    MMIAPIPLMN_LoadLocalDefaultConfig(sim_ready_ind.dual_sys);
#endif  
#if 0//def UNISOC_MMI_SIM_LOCK_SUPPORT//yamei
#ifndef MMI_MULTI_SIM_SYS_SINGLE
//开始启动卡时，会设置query sim的状态为SIM_QUERY_STATUS_START
//两张卡都收到power on cnf时，query sim的状态会置为SIM_QUERY_STATUS_END
    if(SIM_QUERY_STATUS_START == s_query_sim_status)
    {
        SCI_TRACE_LOW("[UNISOC_SIM_LOCK] HandleSimReadyInd sim %d ready",sim_ready_ind.dual_sys);
        if(MN_DUAL_SYS_1 == sim_ready_ind.dual_sys)
        {
            SCI_TRACE_LOW("[UNISOC_SIM_LOCK] HandleSimReadyInd power on sim 2");
            MMIPHONE_SetSimStatus(MN_DUAL_SYS_2, SIM_NOT_INITED);
            
            s_is_power_on_sim_cnf[MN_DUAL_SYS_2] = FALSE;
            MNPHONE_PowerOnSimEx(MN_DUAL_SYS_2, TRUE);
        }
    }
#endif
#endif
    
    return (result);
}
PUBLIC APP_MN_SIM_GET_PIN_IND_T GetSImReadyStatus(MN_DUAL_SYS_E dual_sys)
{
	return s_handle_pin_status.sim_pin_ind[dual_sys];
}
PUBLIC BOOLEAN GetIsSameCard(MN_DUAL_SYS_E dual_sys)
{
	return s_ims_same_sim[dual_sys];
}
/*****************************************************************************/
// Description : Get if is Orange Ploand card
// Global resource dependence:
// Author:dong.chunguang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPolandSimCard(PHONE_SIM_STATUS_T *service_status)
{
    BOOLEAN                     is_polandsim = FALSE;

    //get single card mnc value
    if( (260 == service_status->hplmn.mcc) &&
        (05 == service_status->hplmn.mnc)
        )
    {
        is_polandsim = TRUE;
    }
    else
    {
        is_polandsim = FALSE;
    }
    return(is_polandsim);
}

/*****************************************************************************/
// 	Description : 保存 SIM 状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreSimReadyStatus(
                               APP_MN_SIM_READY_IND_T  *sim_status_ptr
                               )
{
    char tstr1[64] = {0};
    MN_IMSI_T imsi[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MN_DUAL_SYS_E   dual_sys = MN_DUAL_SYS_1;
    MMI_RECORD_INSERT_SIM_CARD_T recordInsertSimInfo[MMI_DUAL_SYS_MAX] = {0};
    BOOLEAN is_rtcAbnormal = FALSE;
    dual_sys = sim_status_ptr->dual_sys;
    
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"StoreSimReadyStatus:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4715_112_2_18_2_45_9_94,(uint8*)"d", dual_sys);
        return;
    }    
    MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_OK);
    s_sim_status[dual_sys].pin1_is_enabled               = sim_status_ptr->pin1_is_enabled ; 	
    s_sim_status[dual_sys].pin1_is_blocked               = sim_status_ptr->pin1_is_blocked;		
    s_sim_status[dual_sys].pin1_status                   = sim_status_ptr->pin1_status;         
    s_sim_status[dual_sys].pin2_status                   = sim_status_ptr->pin2_status;         
    s_sim_status[dual_sys].unblock_pin1_status           = sim_status_ptr->unblock_pin1_status;	
    s_sim_status[dual_sys].unblock_pin2_Status           = sim_status_ptr->unblock_pin2_Status;	
    s_sim_status[dual_sys].is_pin2_required_acm_max      = sim_status_ptr->is_pin2_required_acm_max; 
    s_sim_status[dual_sys].is_pin2_Required_acm          = sim_status_ptr->is_pin2_Required_acm;	 
    s_sim_status[dual_sys].is_pin2_Required_puct         = sim_status_ptr->is_pin2_Required_puct;	 
    s_sim_status[dual_sys].is_aoc_support                = sim_status_ptr->AOC_is_support;	 
    s_sim_status[dual_sys].imsi                          = sim_status_ptr->imsi;			    
    s_sim_status[dual_sys].hplmn                         = sim_status_ptr->hplmn;
    s_sim_status[dual_sys].location_info                 = sim_status_ptr->location_info;		
    s_sim_status[dual_sys].phase                         = sim_status_ptr->phase;			    
    s_sim_status[dual_sys].sim_service                   = sim_status_ptr->sim_service;		    
    s_sim_status[dual_sys].admin                         = sim_status_ptr->admin;   

    //get imsi from nv
    MMINV_READ(MMINV_PHONE_IMSI,imsi,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(imsi,0,sizeof(imsi));
        MMINV_WRITE(MMINV_PHONE_IMSI,imsi);
    }
    
    SCI_TRACE_BUF("[szp]nv imsi.number:", imsi[dual_sys].imsi_val, imsi[dual_sys].imsi_len);
    TransformBcdToStr(imsi[dual_sys].imsi_val,imsi[dual_sys].imsi_len, (uint8 *)tstr1);
    SCI_TRACE_LOW("[szp]nv imsi.number: %s", tstr1);
    
      //add record insert sim time
	  MMINV_READ(MMI_NV_INSERT_SIM_TIME, recordInsertSimInfo, return_value);
	  if ( MN_RETURN_SUCCESS != return_value )
	  {
		SCI_MEMSET(recordInsertSimInfo,0,sizeof(recordInsertSimInfo));
        	MMINV_WRITE(MMI_NV_INSERT_SIM_TIME,recordInsertSimInfo);
	  }
	  
	  if ( (recordInsertSimInfo[dual_sys].imei.imsi_len == s_sim_status[dual_sys].imsi.imsi_len) &&
	  	(0 == memcmp(recordInsertSimInfo[dual_sys].imei.imsi_val, s_sim_status[dual_sys].imsi.imsi_val,recordInsertSimInfo[dual_sys].imei.imsi_len)))
	  {
		;
	  }
	  else
	  {
            recordInsertSimInfo[dual_sys].recordInsertSimTime.year = 2019;
            recordInsertSimInfo[dual_sys].recordInsertSimTime.mon = 1;
            recordInsertSimInfo[dual_sys].recordInsertSimTime.mday = 1;
            recordInsertSimInfo[dual_sys].recordInsertSimTime.wday = 31;
            
            SCI_MEMSET(&(recordInsertSimInfo[dual_sys].imei), 0x00, sizeof(recordInsertSimInfo[dual_sys].imei));
            SCI_MEMCPY(&(recordInsertSimInfo[dual_sys].imei), &(s_sim_status[dual_sys].imsi), sizeof(recordInsertSimInfo[dual_sys].imei));
            MMINV_WRITE(MMI_NV_INSERT_SIM_TIME, recordInsertSimInfo);
	  }
	  //End

    //the current sim card is same to the last sim card
    if ((imsi[dual_sys].imsi_len == s_sim_status[dual_sys].imsi.imsi_len) &&
        (0 == memcmp(imsi[dual_sys].imsi_val, s_sim_status[dual_sys].imsi.imsi_val,imsi[dual_sys].imsi_len)))
    {
        s_is_same_sim[dual_sys] = TRUE;
        s_ims_same_sim[dual_sys] = TRUE;
    }
    else
    {
#ifndef SFR_SUPPORT_CTCC
        SCI_MEMSET(&(imsi[dual_sys]), 0x00, sizeof(imsi[dual_sys]));
        SCI_MEMCPY(&(imsi[dual_sys]), &(s_sim_status[dual_sys].imsi), sizeof(imsi[dual_sys]));
        MMINV_WRITE(MMINV_PHONE_IMSI, imsi);
#endif
        s_is_same_sim[dual_sys] = FALSE;
        s_ims_same_sim[dual_sys] = FALSE;
    }
    SCI_TRACE_LOW("StoreSimReadyStatus s_ims_same_sim[%d]=%d",dual_sys,s_ims_same_sim[dual_sys]);	
    if(IsPolandSimCard(&s_sim_status[dual_sys]))
    {
        MMICONNECTION_UpdateIpType(dual_sys,s_is_same_sim[dual_sys]);
    }
	SCI_TRACE_LOW("MMIAPIPHONE_IsSameSim s_is_same_sim[%d]=%d", dual_sys, s_is_same_sim[dual_sys]);
    //if not the same SIM, clean network name in NV
    if(!s_is_same_sim[dual_sys])
    {
        MMIPHONE_NV_NETWORK_NAME_T* nv_network_name = MMIPHONE_GetNVNetworkNameInfo();
#ifdef MMI_NETWORK_NAME_SUPPORT
        MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
#endif

        nv_network_name[dual_sys].is_opn_exist = FALSE;
        nv_network_name[dual_sys].opn_dcs = 0;
        nv_network_name[dual_sys].opn_len  = 0;
        nv_network_name[dual_sys].is_add_ci = FALSE;
        SCI_MEMSET(nv_network_name[dual_sys].opn, 0x00, MAX_ONSHORTFORM_LEN);
        
        MMINV_WRITE(MMINV_NETWORK_NAME,nv_network_name);
        
    //set network name, store plmn
#ifdef MMI_NETWORK_NAME_SUPPORT
        network_name[dual_sys].opn_dcs = 0;
        network_name[dual_sys].opn_len = 0;
        network_name[dual_sys].is_add_ci = FALSE;
        SCI_MEMSET(network_name[dual_sys].opn, 0x00, MAX_ONSHORTFORM_LEN);
#endif    
#ifdef MMI_RESET_PHONE_AUTO_PIN
        SCI_TRACE_LOW("StoreSimReadyStatus,Reset auto pin value");
        SCI_MEMSET(&g_pin_value[dual_sys], 0x00, sizeof(MN_BLOCK_T));
        //set sim pin to nv
        MMINV_WRITE(MMINV_SIM_PIN_VALUE, g_pin_value);
#endif       
#ifdef MMI_CSP_SUPPORT
        //Reset SIM Related Parameter
        if(MMIAPIPHONE_IsCPHSSupport())
        {
        	MMIAPISET_ResetSelectLine(dual_sys);
        }
#endif
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
        MMIAPISMS_ResetVMInfo(dual_sys);
#endif
    }
#if 0 //def WIN32
    {
        MN_PLMN_T plmn_list[] = 
        {
            {460, 0, 2},
            {460, 0, 2},
            {460, 0, 2},
            {460, 1, 2},
        };
        
        s_sim_status[dual_sys].hplmn = plmn_list[dual_sys];
    }
#endif
}

/*****************************************************************************/
// 	Description : 保存 SIM  not ready 状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreSimNotReadyStatus(
                                  APP_MN_SIM_NOT_READY_IND_T  sim_status_ptr
                                  )
{
    MN_DUAL_SYS_E   dual_sys = MN_DUAL_SYS_1;
    
    dual_sys = sim_status_ptr.dual_sys;
    
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"StoreSimNotReadyStatus:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4786_112_2_18_2_45_9_95,(uint8*)"d", dual_sys);
        return;
    }
    
    switch (sim_status_ptr.reason)
    {
    case MNSIM_ERROR_NO_SECRETCODE_INITIALIZED:
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_REJECTED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, TRUE);
        break;
        
    case MNSIM_ERROR_REJECT_BY_NET:
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_REGISTRATION_FAILED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, TRUE);
        break;
        
    case MNSIM_ERROR_PIN1_BLOCKED:
    case MNSIM_ERROR_PIN2_BLOCKED:
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_PIN_BLOCKED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, TRUE);
        break;
        
    case MNSIM_ERROR_PIN1_UNBLOCK_BLOCKED:
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_PUK_BLOCKED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, TRUE);
        break;
        
    case MNSIM_ERROR_PIN2_UNBLOCK_BLOCKED:
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_PUK2_BLOCKED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, TRUE);
        break;

    case MNSIM_ERROR_REMOVED:
#ifdef MCARE_V31_SUPPORT
        McareV31_ExitMcare();
#endif
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_NO_SIM);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, FALSE);
#ifdef SIM_PLUG_IN_SUPPORT        
        NotifySIMPlugInd(dual_sys, MMIPHONE_NOTIFY_SIM_PLUG_OUT);
#endif
        break;
        
#ifdef MMI_SIM_LOCK_SUPPORT
    case MNSIM_ERROR_SIM_DATA_INTEGRITY_FAIL:
    case MNSIM_ERROR_SIM_NETWORT_LOCKED:
    case MNSIM_ERROR_SIM_NETWORT_SUBSET_LOCKED:
    case MNSIM_ERROR_SIM_SP_LOCKED:
    case MNSIM_ERROR_SIM_CORPORATE_LOCKED:
    case MNSIM_ERROR_SIM_USER_LOCKED:
        MMIPHONE_SetSimStatus(dual_sys, SIM_LOCKED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, FALSE);
        break;
#endif 

    default:
        SCI_TRACE_LOW("StorsimNotReadyStatus MMIPHONE_SetSimStatus");
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_NO_SIM);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, FALSE);
        break;
    }
}

/*****************************************************************************/
// 	Description : the current sim card is same to the last sim card
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsSameSim(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    return s_is_same_sim[dual_sys];
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_NOT_READY_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimNotReadyInd(
                                        DPARAM              param
                                        )
{
    APP_MN_SIM_NOT_READY_IND_T   sim_not_ready_ind = *(APP_MN_SIM_NOT_READY_IND_T *)param;
    MMI_RESULT_E        result = MMI_RESULT_FALSE;//MMI_RESULT_TRUE;
	MMICOM_BANNER_INFO_T banner_info = {0};
	GUI_POINT_T pin_alert_start_point = {0, 0};
#ifndef SUBLCD_SIZE_NONE
    SUB_CONTENT_E       sub_content = SUB_CONTENT_NONE;
#endif
    uint32 i = 0;
    BOOLEAN is_handling_pin = FALSE;
    
    StoreSimNotReadyStatus(sim_not_ready_ind);

#ifdef MMI_LDN_SUPPORT
    if (MMIAPIIDLE_IdleWinIsOpen())
    {
        MMIAPICL_InitCallInfo(sim_not_ready_ind.dual_sys);
    }
    else
    {
        s_ldn_flag |=0x01<<(sim_not_ready_ind.dual_sys);
    }
#endif
#ifdef UNISOC_MMI_SIM_LOCK_SUPPORT
#ifdef WIN32//debug 
    sim_not_ready_ind.reason = MNSIM_ERROR_PIN1_UNBLOCK_BLOCKED;
#endif
#endif

    s_pin_input_ok_status = FALSE;
    SCI_TRACE_LOW("mmiphone: s_pin_input_ok_status = %d", s_pin_input_ok_status);
#if defined (MMI_SIM_LOCK_SUPPORT) || defined (UNISOC_MMI_SIM_LOCK_SUPPORT)//8-20
    SCI_TRACE_LOW("mmiphone: sim_not_ready_ind.reason = %d", sim_not_ready_ind.reason);
    switch (sim_not_ready_ind.reason)
    {
#ifdef UNISOC_MMI_SIM_LOCK_SUPPORT
    //sim lock的input Win_Window
    //case MNSIM_ERROR_SIM_MS_LOCKED:
    case MNSIM_ERROR_SIM_NETWORT_LOCKED:
    case MNSIM_ERROR_SIM_NETWORT_SUBSET_LOCKED:
    case MNSIM_ERROR_SIM_SP_LOCKED:
    case MNSIM_ERROR_SIM_CORPORATE_LOCKED:
    case MNSIM_ERROR_SIM_USER_LOCKED:    	
        {
                //debug
             HandleSimLockInfo(sim_not_ready_ind);
            //打开输入解锁信息的窗口
            MMK_CreateWin((uint32*)SIM_LOCK_INPUTING_CODE_WIN_TAB,(ADD_DATA)sim_not_ready_ind.dual_sys);   
            if (s_handle_pin_status.is_handling_pin[s_handle_pin_status.cur_handling_sim_id]
                && s_handle_pin_status.cur_handling_sim_id != sim_not_ready_ind.dual_sys)
            {
                SCI_TraceLow("HandleSimNotReadyInd  Hide");
                MMK_HideWindow(PHONE_SIM_LOCK_INPUTING_CODE_WIN_ID, TRUE);
            }
            else
            {
                SCI_TraceLow("HandleSimNotReadyInd  Show");
                MMK_HideWindow(PHONE_SIM_LOCK_INPUTING_CODE_WIN_ID, FALSE);
            }
        }
        break;
 #elif defined(MMI_SIM_LOCK_SUPPORT)//8-20
    case MNSIM_ERROR_SIM_NETWORT_LOCKED:
    case MNSIM_ERROR_SIM_NETWORT_SUBSET_LOCKED:
    case MNSIM_ERROR_SIM_SP_LOCKED:
    case MNSIM_ERROR_SIM_CORPORATE_LOCKED:
    case MNSIM_ERROR_SIM_USER_LOCKED:
        {
            MMI_STRING_T pin_value ={0};
            SCI_TRACE_LOW("mmiphone.c MNSIM_ERROR_SIM_NETWORT_LOCKED");
            if (MMK_IsOpenWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID))//记录下正确的pin码
            {
                GUIEDIT_GetString(PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID, &pin_value);
                g_pin_value[sim_not_ready_ind.dual_sys].blocklen = pin_value.wstr_len;

                MMI_WSTRNTOSTR(
                    g_pin_value[sim_not_ready_ind.dual_sys].blockbuf,
                    MN_MAX_BLOCK_LEN,
                    pin_value.wstr_ptr,
                    g_pin_value[sim_not_ready_ind.dual_sys].blocklen,
                    g_pin_value[sim_not_ready_ind.dual_sys].blocklen
                    );
            }
            else if (MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)) //记录下正确的pin码
            {
                GUIEDIT_GetString(PHONE_PIN_EDITBOX_CTRL_ID, &pin_value);
                g_pin_value[sim_not_ready_ind.dual_sys].blocklen = pin_value.wstr_len;

                MMI_WSTRNTOSTR(
                    g_pin_value[sim_not_ready_ind.dual_sys].blockbuf,
                    MN_MAX_BLOCK_LEN,
                    pin_value.wstr_ptr,
                    g_pin_value[sim_not_ready_ind.dual_sys].blocklen,
                    g_pin_value[sim_not_ready_ind.dual_sys].blocklen
                    );
            }
#ifdef MMI_RESET_PHONE_AUTO_PIN
            //set sim pin to nv
            MMINV_WRITE(MMINV_SIM_PIN_VALUE, g_pin_value);
#endif       
            #if 0 //def MMI_MULTI_SIM_SYS_SINGLE  //reserved for Multi SIM
                    BOOLEAN is_need_handle = FALSE;
                    BOOLEAN is_handling_simlock = FALSE;
                    /*set sim_not_ready_ind*/
                    for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
                    {
                        /*有其他卡正处在SIM Lock handling 状态，设置当前卡为need handle*/
                        if(( i != sim_not_ready_ind.dual_sys)&&(s_handle_sim_lock_status.is_handling_simlock[i]))
                        {
                            is_need_handle = TRUE;
                            break;            
                        }
                    } 
                    if(is_need_handle)
                    {
                        s_handle_sim_lock_status.need_handle_simlock[sim_not_ready_ind.dual_sys] = TRUE;
                		
                        return result;
                    }
                    else
                    {
                        /*当前卡需要处理SIM Lock,记下状态，进入密码输入界面*/
                        s_handle_sim_lock_status.is_handling_simlock[sim_not_ready_ind.dual_sys] = TRUE;//set the status (lock)
                        s_handle_sim_lock_status.cur_handling_sim_id = sim_not_ready_ind.dual_sys;
                        //create input SIM lock window
                        MMIPHONE_CreateInputSimLockWindow(sim_not_ready_ind);
                        return result;
                    }    
            #else            
                    MMIPHONE_CreateInputSimLockWindow(sim_not_ready_ind);
       
                    if (s_handle_pin_status.is_handling_pin[s_handle_pin_status.cur_handling_sim_id]
                         && s_handle_pin_status.cur_handling_sim_id != sim_not_ready_ind.dual_sys)
                    {
                        MMK_HideWindow(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID, TRUE);                           
                    }
                    else 
                    {
                        MMK_HideWindow(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID, FALSE);  
                    }               
            #endif
         }
            break;
 #endif               
        case MNSIM_ERROR_SIM_DATA_INTEGRITY_FAIL:
            break;
                    
        default:
            break;
        }
 #endif
    if(MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID) //for puk block
        && (sim_not_ready_ind.dual_sys == MMIAPIPHONE_GetPukRemainDualSys()))//for bug514845 
    {
        MMIAPIPHONE_SetPukRemainStatus(1);
        banner_info.banner_type = MMI_BANNER_TEXT;
        banner_info.text1_id = STR_SIM_BLOCKED_EXT01;//SIM卡被锁
        banner_info.start_point = pin_alert_start_point;
        MMIAPICOM_OpenBannerWin(PHONE_PUK_INPUT_WIN_ID, banner_info);
    }
    if ( (MMK_IsOpenWin(PHONE_PIN_WAIT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_NEW_PIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
        && s_handle_pin_status.cur_handling_sim_id == sim_not_ready_ind.dual_sys)
    {
        if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[sim_not_ready_ind.dual_sys])
        {
            g_service_status[sim_not_ready_ind.dual_sys].is_slecting = TRUE;
        }

        //关闭pin和puk输入窗口
        MMK_CloseWin(PHONE_PIN_ALERT_WIN_ID);
        MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
//        MMK_CloseWin(PHONE_PUK_INPUT_WIN_ID);
    }

    //add for dual sys
    s_handle_pin_status.need_handle_pin[sim_not_ready_ind.dual_sys] = FALSE;
    s_handle_pin_status.is_handling_pin[sim_not_ready_ind.dual_sys] = FALSE;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (s_handle_pin_status.is_handling_pin[i])
        {
            is_handling_pin = TRUE;
            break;
        }
    }
    
    if (!is_handling_pin)
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (i != sim_not_ready_ind.dual_sys)
            {
                if (s_handle_pin_status.need_handle_pin[i])
                {
                    //set s_handle_pin_status
                    s_handle_pin_status.cur_handling_sim_id = (MN_DUAL_SYS_E)i;
                    s_handle_pin_status.is_handling_pin[i] = TRUE;
                    
                    //create input pin window
                    CreateInputPinWindow(s_handle_pin_status.sim_pin_ind[i]);
                    return result;
                }
            }
        }
    }
	//锁卡的基本设计基础: 先power sim1 再power sim2.
	//必须不是输入pin码界面才需要准备power sim2
	//解决卡1不存在或者注册失败的case，这个时候需要power sim2   
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    SCI_TRACE_LOW( "HandleSimNotReadyInd(dual_sys = %d): reason=%d,sim1_right=%d,sim2_right=%d", 
			sim_not_ready_ind.dual_sys,sim_not_ready_ind.reason,s_is_sim1_right,  s_is_sim2_right );
    if(!s_is_simlock_off)
    {
        //MNSIM_ERROR_REJECT_BY_NET for expired SIM handle.
        //sim ready ind first,then not ready ind ,so the ps has been powered on!
        if( MN_DUAL_SYS_1 == sim_not_ready_ind.dual_sys )
        {
            s_is_sim1_poweron = TRUE;
        }
#ifdef MMI_MULTI_SIM_SYS_DUAL 
        else if( MN_DUAL_SYS_2 == sim_not_ready_ind.dual_sys )
        {
            s_is_sim2_poweron = TRUE;
        }
#endif
        if((s_is_sim1_right || s_is_sim2_right) && !MMIPHONE_IsLegalState())  
        {
            MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
            for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
            {
                if( MMIPHONE_PS_POWER_ON_NORMAL == s_ps_ready[dual_sys] ) //关闭已经启动的PS
                {
                    MMIAPIPHONE_PowerOffPs(dual_sys);
                }
                
                //针对存在的sim卡并且卡被锁，则提示invlid sim.
                //其它所有not ready 的sim卡提示信息跟原有一致!
                if(MMIAPIPHONE_GetSimExistedStatus(dual_sys))                
                {
                    MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_LOCKED_INVALID);
                }
           }
        }
#ifdef MMI_MULTI_SIM_SYS_DUAL 
        else if( MN_DUAL_SYS_1 == sim_not_ready_ind.dual_sys )
        {
            //卡1 在注册失败或者不存在的时候，启动卡2 !   
            //MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
            if(!s_is_sim2_poweron)
            {
                MNPHONE_PowerOnSimEx(MN_DUAL_SYS_2, TRUE);
            }
        }
#endif
    }
#endif

#ifndef SUBLCD_SIZE_NONE    
    switch (sim_not_ready_ind.reason)
    {

    case MNSIM_ERROR_NO_SECRETCODE_INITIALIZED:
        // 打开SIM卡被拒界面! 
        sub_content = SUB_CONTENT_SIM_REJECTED;
        break;
        
    case MNSIM_ERROR_REJECT_BY_NET:
        // 打开SIM卡注册失败界面! 
        sub_content = SUB_CONTENT_SIM_REGISTRATION_FAILED;
        break;
        
    case MNSIM_ERROR_NOT_SUPPORT_GSM_ONLY://@zhaohui,cr109170
        MMIPUB_OpenAlertWarningWin(TXT_NOT_SUPPORT_SIM);	
        sub_content = SUB_CONTENT_SIM_REGISTRATION_FAILED;
        break;
        
    case MNSIM_ERROR_PIN1_BLOCKED:
    case MNSIM_ERROR_PIN2_BLOCKED:
        //jassmine cr14034 PIN被锁，不用处理此消息
        break;
        
    case MNSIM_ERROR_PIN1_UNBLOCK_BLOCKED:
        sub_content = SUB_CONTENT_PUK_BLOCKED;
        break;
        
    case MNSIM_ERROR_PIN2_UNBLOCK_BLOCKED:
        sub_content = SUB_CONTENT_PUK2_BLOCKED;
        break;
        
    case MNSIM_ERROR_REMOVED:
        sub_content = SUB_CONTENT_NO_SIM;
        break;
        
    default:
        sub_content = SUB_CONTENT_NO_SIM;
        break;
    }
#endif    
    MMIAPICOM_StatusAreaSetNetWorkName(); //MAIN_SetIdleNetWorkName();    
    
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    sim_not_ready_ind.dual_sys = MN_DUAL_SYS_1;
#endif

    //关闭pin和puk输入窗口
    if (s_handle_pin_status.cur_handling_sim_id == sim_not_ready_ind.dual_sys)
    {
        if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[sim_not_ready_ind.dual_sys])
        {
            g_service_status[sim_not_ready_ind.dual_sys].is_slecting = TRUE;
        }

        MMK_CloseWin(PHONE_PIN_ALERT_WIN_ID);
        MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
 //       MMK_CloseWin(PHONE_PUK_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
    }
#ifndef SUBLCD_SIZE_NONE    
    //刷新小屏,显示相应的Sim消息
    if(sub_content != SUB_CONTENT_NONE &&  SUB_CONTENT_NO_SIM != sub_content )
    {
        MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

        //有任意一张sim卡是NO SIM 的状态就直接return !!!
        for(dual_sys = MN_DUAL_SYS_1;dual_sys < MMI_DUAL_SYS_MAX;dual_sys++)
        {
            if(SIM_STATUS_NO_SIM !=MMIAPIPHONE_GetSimStatus(dual_sys))
            {
                return (result);
            }
        }
        
        //刷新小屏,显示相应的Sim消息
        MMISUB_IsPermitUpdate(TRUE);  
        MMISUB_SetSubLcdDisplay(TRUE,TRUE,sub_content,PNULL);
    }
#endif   
    if (MNSIM_ERROR_REMOVED == sim_not_ready_ind.reason)
    {
        MMIAPIPB_AbortPB();
		//bug 184713
#ifndef SIM_PLUG_IN_SUPPORT 
        MMIAPICC_ExitMoCall(sim_not_ready_ind.dual_sys);
#endif        
        //if (MMIAPIPB_IsExportContactOpc())
        //{
           // MMIPUB_OpenAlertFailWin(TXT_SIM_PLUG_OUT);
        //}
        MMIAPIPB_ResortPB();
    }

#if 0//def UNISOC_MMI_SIM_LOCK_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
//开始启动卡时，会设置query sim的状态为SIM_QUERY_STATUS_START
//两张卡都收到power on cnf时，query sim的状态会置为SIM_QUERY_STATUS_END
    if(SIM_QUERY_STATUS_START == s_query_sim_status)
    {
        SCI_TRACE_LOW("[UNISOC_SIM_LOCK] HandleSimNotReadyInd sim %d not ready, reason=%d",sim_not_ready_ind.dual_sys,sim_not_ready_ind.reason);
        if(MN_DUAL_SYS_1 == sim_not_ready_ind.dual_sys && MNSIM_ERROR_SIM_MS_LOCKED != sim_not_ready_ind.reason)
        {
            SCI_TRACE_LOW("[UNISOC_SIM_LOCK] HandleSimNotReadyInd ,power on sim 2");
            MMIPHONE_SetSimStatus(MN_DUAL_SYS_2, SIM_NOT_INITED);
            
            s_is_power_on_sim_cnf[MN_DUAL_SYS_2] = FALSE;
            MNPHONE_PowerOnSimEx(MN_DUAL_SYS_2, TRUE);
        }
    }
#endif
#endif
    return (result);
}

// adjust for queen: change  1: 1-7;	2: 8-11;	 3: 12-15;	4: 16-19;	5: 20-63
// 				    into	 1: 0-3;	2: 3-5;    3: 5-8;		4: 8-12;	5: 12-63
#define RSSI_DELTA      2
#define PHONE_RXLEVEL_DEFAULT_LEVEL 5
LOCAL uint8 MMIPHONE_GetRSSIMap( uint8 cur_rssi,MN_DUAL_SYS_E dual_sys)
{
    enum {rxlev_G, rxlev_L, rxlev_U};
    uint8 rxLevel = 0;
    uint8 rxlevelindex = 0;
    MMI_GMMREG_RAT_E    network_status_rat = MMI_RAT_TYPE_G;
    const int16 g_service_rxlev_map[][PHONE_RXLEVEL_DEFAULT_LEVEL] = {
        {21, 13, 7, 3, 0},   //GSM
        {42, 32, 22, 12, 0 },   //LTE
        { 16, 12, 8, 1 }    // unknown default
    };
#ifndef WIN32
    if(MNPHONE_GetRat(dual_sys) == 2)
        network_status_rat =MMI_RAT_TYPE_G;
    else
        network_status_rat = MMI_RAT_TYPE_LTE;
#endif
    switch( network_status_rat ) {
        case MMI_RAT_TYPE_G:
            rxlevelindex = rxlev_G;
            break;
        case MMI_RAT_TYPE_LTE:
            rxlevelindex = rxlev_L;
            break;
        case MMI_GMMREG_RAT_UNKNOWN:
        default:
            rxlevelindex = rxlev_U;
            break;
    }

    SCI_TRACE_LOW( "[%s] dual_sys[%d] rxlev[%d] rat[%d]", __FUNCTION__, dual_sys, cur_rssi, network_status_rat );

    if( g_service_rxlev_map[rxlevelindex][PHONE_RXLEVEL_DEFAULT_LEVEL - 5] <= cur_rssi ) {
        rxLevel = PHONE_RXLEVEL_DEFAULT_LEVEL ;
    }else if( g_service_rxlev_map[rxlevelindex][PHONE_RXLEVEL_DEFAULT_LEVEL - 4] <= cur_rssi ) {
        rxLevel = PHONE_RXLEVEL_DEFAULT_LEVEL - 1 ;
    } else if( g_service_rxlev_map[rxlevelindex][PHONE_RXLEVEL_DEFAULT_LEVEL - 3] <= cur_rssi ) {
        rxLevel = 3;
    } else if( g_service_rxlev_map[rxlevelindex][PHONE_RXLEVEL_DEFAULT_LEVEL - 2] <= cur_rssi ) {
        rxLevel = 2;
    } else if( g_service_rxlev_map[rxlevelindex][PHONE_RXLEVEL_DEFAULT_LEVEL - 1] <= cur_rssi ) {
        rxLevel = 1;
    } else {
        rxLevel = 0;
    }

    return rxLevel;
}
    
/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SCELL_RSSI_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleScellRssiInd(
                                      DPARAM              param
                                      )
{
    APP_MN_SCELL_RSSI_IND_T     srri_ind = *(APP_MN_SCELL_RSSI_IND_T *)param;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    uint8               rxLevel = 0;
    MN_DUAL_SYS_E       dual_sys = MN_DUAL_SYS_1;
    BOOLEAN is_direct_draw = FALSE;
    
    dual_sys = srri_ind.dual_sys;
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleScellRssiInd:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5110_112_2_18_2_45_10_96,(uint8*)"d", dual_sys);
        return MMI_RESULT_FALSE;
    }
    
    //设置全局变量
    if (g_service_status[dual_sys].is_first_rssi
        && 0 == g_service_status[dual_sys].rssiLevel
        && 0 < srri_ind.rxlev)//第一次收到非0的rssi直接显示
    {
        g_service_status[dual_sys].is_first_rssi = FALSE;
        is_direct_draw = TRUE;
    }
 
    g_service_status[dual_sys].rssiLevel = srri_ind.rxlev;

    rxLevel = MMIPHONE_GetRSSIMap(g_service_status[dual_sys].rssiLevel,dual_sys);
    if (rxLevel == 0 || g_service_status[dual_sys].old_rssiLevel == 0)
        {
            //Update immediately in zero case
        }
    else if (rxLevel ==  g_service_status[dual_sys].old_rssiLevel+1)     //up  one
        {
            rxLevel = MMIPHONE_GetRSSIMap(g_service_status[dual_sys].rssiLevel-RSSI_DELTA,dual_sys);
        }
    else if (rxLevel == g_service_status[dual_sys].old_rssiLevel -1)    //down one
        {
            rxLevel = MMIPHONE_GetRSSIMap(g_service_status[dual_sys].rssiLevel+RSSI_DELTA,dual_sys); 
        }

    SCI_TRACE_LOW( "[%s] line[%d] rxLevel[%d] ", __FUNCTION__,__LINE__, rxLevel);

    MMIAPICOM_StatusAreaSetRxLevel(dual_sys, rxLevel, is_direct_draw);
    g_service_status[dual_sys].old_rssiLevel = rxLevel;

    NotifyRxLevelChange(dual_sys, rxLevel);
    #if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_SetSignalStrength(rxLevel); //mdy by qing.yu@for cr
    #endif
    return (result);
}



/*****************************************************************************/
// 	Description : power off and query
//	Global resource dependence : 
//  Author: Jassmine
//	Note:sometimes we should query and confirm
/*****************************************************************************/
void MMIAPIPHONE_PowerOff(void)
{
#ifdef DL_SUPPORT
    if (MMIAPIDL_GetDownloadStatus())
    {
        MMIPHONE_OpenQueryPowerOff(MMIPHONE_QUERY_POWER_OFF_DOWNLOAD);
    }
    else
#endif
    {
        MMIPHONE_PowerOff();
    }
}

/*****************************************************************************/
// 	Description : power off directly
//	Global resource dependence : 
//  Author: Jassmine
//	Note: sometimes we should poweroff directly
/*****************************************************************************/
void MMIAPIPHONE_PowerOffEx(void)
{
    MMIPHONE_PowerOff();
}

/*****************************************************************************/
// 	Description : power off MS
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
void MMIPHONE_PowerOff(void)
{
    uint32 i=0;
    POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();
#ifdef ENG_SUPPORT
    uint32 tick = 0;
#endif
    //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOff:restart_condition=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5199_112_2_18_2_45_10_97,(uint8*)"d", restart_condition);
    
#if defined(MMI_SIM_LOCK_SUPPORT)    
        /*在倒计时界面关机时需要记住还剩下的时间*/
    if(MMK_IsOpenWin( PHONE_SIM_LOCK_COUNTDOWN_WIN_ID))
    {
        MMI_OPERATE_SIM_LOCK_T sim_lock_operate = {0};
        uint32 trials_counter = 0;
        uint32 time_out_value = 0;

        //SCI_TRACE_LOW:"MMIPHONE_PowerOff PHONE_SIM_LOCK_COUNTDOWN_WIN_ID open"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5209_112_2_18_2_45_10_98,(uint8*)"");
        sim_lock_operate.lock_type = s_sim_lock_operate_value.lock_type;
        MMIAPIPHONE_GetSIMLockRemainTrialsTimer(sim_lock_operate.lock_type,&trials_counter, &time_out_value);
        time_out_value = MMIPHONE_GetCurCountdownTime();
        MNPHONE_UpdateSIMUnlockRetryData(sim_lock_operate.lock_type, trials_counter, time_out_value);
        MMIAPIPHONE_UpdateSIMUnlockData();
    }
#endif    
    
    if(MMIAPIMTV_IsMTVRunning())
    {
        //SCI_TRACE_LOW:"MMIPHONE_PowerOff MMIAPIMTV_IsMTVRunning delay poweroff"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5220_112_2_18_2_45_10_99,(uint8*)"");
        MMIAPIMTV_SetDelayPowerOff(TRUE);
        MMIAPIMTV_Exit();
        return;
    }
    //关闭fm
    MMIAPIFM_Exit();

#ifdef MMIWNS_SUPPORT
    MMIAPIWNS_Stop();
#endif

#ifdef MMI_CSTAR_UDB_SUPPORT
    MMIIM_SaveUdbEntry();
#endif

#ifdef MMI_AUDIO_PLAYER_SUPPORT
    MMIAPIMP3_StopAudioPlayer();
#endif
#ifdef MMI_RECORD_SUPPORT
    MMIAPIRECORD_StopRecordOrPlay();
#endif
	MMIAPICC_StopCCRecord(TRUE);
#ifdef MMI_RECORD_SUPPORT
	MMIENG_StopENGRecord();
#endif
	MMIAPICC_QuitForPowerOff();
#ifdef MMS_SUPPORT
    if(MMIAPIMMS_IsSendingOrRecving())
        {
        MMIAPIMMS_ClearRecvSendModule();
    }
#endif
    if( MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID))
    {
        MMK_CloseWin(PHONE_STARTUP_NORMAL_WIN_ID);
    }

    // 调用MN层函数 请求关机
    for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        //MNPHONE_DeactivePsEx(i);
        MNPHONE_SetPowerOffFlag(1, i);
		
        if (MMIPHONE_PS_POWER_ON_NONE == s_ps_ready[i])//ps not ready
        {
            MNPHONE_PowerOffSimEx(i); //power off SIM
        }
        else
        {
            s_phone_is_power_off_req[i] = TRUE;

            MNPHONE_PowerOffPsEx(i); //Power Off PS
        }
    }

    MMIAPIENG_StopQData();

    MMI_FreeAllRes();
    // 打开关机窗口
    MMIAPIUDISK_ClosePCCamera();
    
#ifdef WIFI_SUPPORT
    MMIAPIWIFI_Off();
#endif

#ifdef MCARE_V31_SUPPORT 
	McareV31_ExitMcare();
#endif

#ifdef ENG_SUPPORT
    tick = SCI_GetTickCount();
    if(tick > 0)
    {
        tick = tick / MMIENG_STANDBY_TIME_COUNT;	
    }
    else
    {
        tick = 0;
    }
    MMI_WriteNVItem(MMIENG_NV_STANDBY_TIME_ID , &tick);
#endif

    if (RESTART_BY_CHARGE == restart_condition)
    {
        POWER_PowerOff();
    }
    else
    {
        MMK_CreateWin((uint32*)PHONE_POWER_OFF_WIN_TAB,PNULL);
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_AppDeInit();
#endif
    }
#ifdef WRE_SUPPORT
    MMIWRE_PowerOff(TRUE);
#endif
}

PUBLIC BOOLEAN MMIAPIPHONE_IsPoweringOff(void)
{
    return MMK_IsOpenWin(PHONE_POWER_OFF_WIN_ID);
}

LOCAL uint32 MMIAPIPHONE_GetThreadState(void)
{
	char thread_name[2][SCI_MAX_NAME_SIZE];
	char queue_name[2][SCI_MAX_NAME_SIZE];
	uint32 state[2];
	uint32 preempt[2];
	uint32 priority[2];
       uint32 result = 0;
       
	BLOCK_ID thread_id = EFS;

	if(SCI_SUCCESS !=SCI_GetThreadInfo(thread_id, &thread_name[0][0], &queue_name[0][0], &state[0],  &priority[0], &preempt[0]))
	{
		SCI_PASSERT(0,("get thread info error!"));    /*assert verified*/
	}
	
	SCI_TRACE_LOW("thread_name:%s,state=%d\n",&thread_name[0][0] ,state[0]);

	thread_id = P_NVSMS;
	if(SCI_SUCCESS !=SCI_GetThreadInfo(thread_id, &thread_name[1][0], &queue_name[1][0], &state[1],  &priority[1], &preempt[1]))
	{
		SCI_PASSERT(0,("get thread info error!"));    /*assert verified*/
	}
	
	SCI_TRACE_LOW("thread_name:%s,state=%d\n",&thread_name[1][0] ,state[1]);

       if ((state[0] == 5) && (state[1] == 5))
       {
            result = 1;
       }
	return  result;

}

/*****************************************************************************/
// 	Description : power reset the phone should de_init the BT
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerReset(void)
{   
    uint32 state;
    POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();
    
    SCI_TRACE_LOW("[MMIBT] MMIAPIPHONE_PowerReset restart_condition=%d", restart_condition);
    if (RESTART_BY_CHARGE != restart_condition)
    {
        //Exit Blue tooth
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_AppDeInit();
#endif
    }
    
    MMIAPISET_StopAllRing(FALSE);
    
    //在MMIPHONE_PowerReset之前关闭背光
    MMIDEFAULT_TurnOffBackLight();
    MMIDEFAULT_StopKeybadBackLightTimer();
    MMIDEFAULT_SetKeybadBackLight(FALSE);

    while(1)
    {
        if (0 == MMIAPIPHONE_GetThreadState())    //TX_READY=0
        {
            SCI_Sleep(10);
        }
        else
        {
            break;
        }
    }

    POWER_Reset();
}

/*****************************************************************************/
// 	Description : get plmn name, cmcc, unicom or others
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*****************************************************************************/
PUBLIC PHONE_PLMN_NAME_E MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_E dual_sys)
{
    PHONE_PLMN_NAME_E name = PLMN_INVALID;
    uint16 i = 0;
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    uint16 network_table_len = 0;
    
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetNetworkName err dual_sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5298_112_2_18_2_45_10_100,(uint8*)"d", dual_sys);
        return PLMN_INVALID;
    }

    if ((g_service_status[dual_sys].plmn.mnc < 100 && (3 == g_service_status[dual_sys].plmn.mnc_digit_num)))
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_mnc_digit_num_3_nametable;
        network_table_len = MAX_NETWORK_MNC_DIGIT_NUM_3_NUM;
    }
    else
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;
        network_table_len = MAX_NETWORK_NUM;
    }
    
    for (i = 0; i < network_table_len; i++)
    {
        if ((network_table_ptr[i].mnc == g_service_status[dual_sys].plmn.mnc)
            && (network_table_ptr[i].mcc == g_service_status[dual_sys].plmn.mcc))
        {
            name = network_table_ptr[i].name;
            break;
        }
    }
    
    return name;   
}
/*****************************************************************************/
// 	Description : get the network name
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIPHONE_GetNetWorkNameId(MMI_GMMREG_RAT_E rat, MN_PLMN_T *plmn_ptr)
{
    uint16 i = 0;
    MMI_TEXT_ID_T network_name_id = TXT_NULL;
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    uint16 network_table_len = 0;

    if (PNULL == plmn_ptr)
    {
        return network_name_id;
    }
    if ((plmn_ptr->mnc < 100 && (3 == plmn_ptr->mnc_digit_num)))
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_mnc_digit_num_3_nametable;
        network_table_len = MAX_NETWORK_MNC_DIGIT_NUM_3_NUM;
    }
    else
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;
        network_table_len = MAX_NETWORK_NUM;
    }
    
    for (i = 0 ; i < network_table_len ; i++)
    {
        if ((network_table_ptr[i].mnc == plmn_ptr->mnc)
            && (network_table_ptr[i].mcc == plmn_ptr->mcc))
        {
            network_name_id = network_table_ptr[i].name_index;
            SCI_TRACE_LOW("mmiphone: MMIAPIPHONE_GetNetWorkNameId s_network_nametable[%d]", i);

#ifdef CMCC_UI_STYLE
            if(MMI_GMMREG_RAT_3G == rat && TXT_NET_CMCC == network_name_id)
            {
                network_name_id = TXT_NET_CMCC_3G;
            }
#endif
            break;
        }
    }
    
    return (network_name_id);
}

/*****************************************************************************/
// 	Description : get the network id
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC PHONE_PLMN_NAME_E MMIAPIPHONE_GetNetWorkId(MN_PLMN_T *plmn_ptr)
{
    uint16 i = 0;
    PHONE_PLMN_NAME_E plmn_name = PLMN_INVALID;
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    uint16 network_table_len = 0;

    if (PNULL == plmn_ptr)
    {
        return plmn_name;
    }

    network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;
    network_table_len = MAX_NETWORK_NUM;
    
    for (i = 0 ; i < network_table_len ; i++)
    {
        if ((network_table_ptr[i].mnc == plmn_ptr->mnc)
            && (network_table_ptr[i].mcc == plmn_ptr->mcc))
        {
            plmn_name = network_table_ptr[i].name;

            break;
        }
    }

    if (PLMN_INVALID == plmn_name)
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_mnc_digit_num_3_nametable;
        network_table_len = MAX_NETWORK_MNC_DIGIT_NUM_3_NUM;

        for (i = 0 ; i < network_table_len ; i++)
        {
            if ((network_table_ptr[i].mnc == plmn_ptr->mnc)
                && (network_table_ptr[i].mcc == plmn_ptr->mcc))
            {
                plmn_name = network_table_ptr[i].name;

                break;
            }
        }
    }
    
    return (plmn_name);
}

/*****************************************************************************/
// 	Description : get the network info
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:in gprs data account,we only use one mnc to represent one operator,but it may have other mnc
//           if so,in auto adapting ,we will not find the right network id;so we use mcc and networkname index to get network name
/*****************************************************************************/
PUBLIC const MMI_NETWORK_NAME_T *MMIAPIPHONE_GetNetWorkInfo(PHONE_PLMN_NAME_E plmn_name)
{
    uint16 i = 0;
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    uint16 network_table_len = 0;

    network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;
    network_table_len = MAX_NETWORK_NUM;
    
    /*search from 0 to higher,so we need to set data account mnc as the smallest  of all mnc for the oprator*/
    for (i = 0 ; i < network_table_len ; i++)
    {
        if (network_table_ptr[i].name == plmn_name)
        {
            return &network_table_ptr[i];
        }
    }

    network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_mnc_digit_num_3_nametable;
    network_table_len = MAX_NETWORK_MNC_DIGIT_NUM_3_NUM;

    for (i = 0 ; i < network_table_len ; i++)
    {
        if (network_table_ptr[i].name == plmn_name)
        {
            return &network_table_ptr[i];
        }
    }
    
    return PNULL;
}

/*****************************************************************************/
// 	Description : get full plmn name, cmcc, unicom or others
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetFullPlmn(MN_DUAL_SYS_E dual_sys, PHONE_PLMN_NAME_E *plmn_name, PHONE_PLMN_NAME_E *hplmn_name)
{
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    uint16 network_table_len = 0;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    if (PNULL != plmn_name)
    {
        *plmn_name = MMIAPIPHONE_GetNetworkName(dual_sys);
    }

    if (PNULL != hplmn_name)
    {
        uint16 i = 0;

        *hplmn_name = PLMN_INVALID;

        if ((s_sim_status[dual_sys].hplmn.mnc < 100 && (3 == s_sim_status[dual_sys].hplmn.mnc_digit_num)))
        {
            network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_mnc_digit_num_3_nametable;
            network_table_len = MAX_NETWORK_MNC_DIGIT_NUM_3_NUM;
        }
        else
        {
            network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;
            network_table_len = MAX_NETWORK_NUM;
        }

        if (SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(dual_sys))
        {
            for (i = 0; i < network_table_len; i++)
            {
                if ((network_table_ptr[i].mnc == s_sim_status[dual_sys].hplmn.mnc) &&
                    (network_table_ptr[i].mcc == s_sim_status[dual_sys].hplmn.mcc))
                {
                    *hplmn_name = network_table_ptr[i].name;
                    break;
                }
            }
        }
    }
}


/*****************************************************************************/
// 	Description : 当sim卡注册失败和注册成功的时候都应该可以设置pin1
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPermitSetPin1(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN     is_permit = FALSE;
    SIM_STATUS_E sim_status = MMIAPIPHONE_GetSimStatus(dual_sys);
    
    is_permit = (BOOLEAN)((SIM_STATUS_OK == sim_status) 
                                        || (SIM_STATUS_REGISTRATION_FAILED == sim_status)
                                        || (SIM_STATUS_PIN_BLOCKED == sim_status));
    
    return(is_permit);
}

/*****************************************************************************/
// 	Description : 获取设置pin1 的sim 个数
//	Global resource dependence : 
//  Author: Michael.Shi
//	Note:  
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetPermitSetPin1Num(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_IsPermitSetPin1(i))
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
// 	Description : according to ps status, judge if wap & mms is available
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_ROAMING_STATUS_E MMIAPIPHONE_GetRoamingStatus(MN_DUAL_SYS_E dual_sys)
{
    MMIPHONE_ROAMING_STATUS_E roaming_status = MMIPHONE_ROAMING_STATUS_MAX;
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetRoamingStatus:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5561_112_2_18_2_45_11_101,(uint8*)"d", dual_sys);
        return MMIPHONE_ROAMING_STATUS_NO_ROMING;
    }
    if(!g_service_status[dual_sys].isRoaming)
    {
        roaming_status = MMIPHONE_ROAMING_STATUS_NO_ROMING;
    }
    else
    {
        if(g_service_status[dual_sys].plmn.mcc == s_sim_status[dual_sys].hplmn.mcc
            ||((g_service_status[dual_sys].plmn.mcc == 404)&&(s_sim_status[dual_sys].hplmn.mcc == 405))    
            ||((g_service_status[dual_sys].plmn.mcc == 405)&&(s_sim_status[dual_sys].hplmn.mcc == 404))//india MCC
        )
        {
            roaming_status = MMIPHONE_ROAMING_STATUS_NATIVE_ROMING;
        }
        else
        {
            roaming_status = MMIPHONE_ROAMING_STATUS_NATIONAL_ROMING;
        }
    }
    //SCI_TRACE_LOW:"roaming status: %d, net isroaming: %d, net mcc: %d, sim mcc: %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5583_112_2_18_2_45_11_102,(uint8*)"dddd", roaming_status, g_service_status[dual_sys].isRoaming, g_service_status[dual_sys].plmn.mcc, s_sim_status[dual_sys].hplmn.mcc);
    return roaming_status;
}

/*****************************************************************************/
// 	Description : get opn display string
//	Global resource dependence : 
//  Author: kun.yu
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_GetAciString(PHONE_SERVICE_STATUS_T *service_status,//IN:
                                         MMI_STRING_T *aci_str_ptr, //OUT
                                         uint16 max_aci_len //IN
                                         )
{
    uint16 i = 0;
    uint16 aci_index = 0;
    char   aic_info[MAX_ACI_INFO_LEN + 1] = {0};

    if( (PNULL == service_status) 
#if !defined(MMI_NETWORK_NAME_SUPPORT)
      || (!service_status->is_add_ci)
#endif
        ||(PNULL == aci_str_ptr))
    {
        return;
    }
    
    for(i = 0; i < ARR_SIZE(s_network_acitable); i++)
    {
        if(s_network_acitable[i].mcc == service_status->plmn.mcc)
        {
            aci_index = i;
            break;
        }
    }

    if(aci_index > 0)
    {
        aci_str_ptr->wstr_len = SCI_STRLEN((char*)s_network_acitable[aci_index].aci_str);
        aci_str_ptr->wstr_len = MIN(aci_str_ptr->wstr_len, max_aci_len);
        MMI_STRNTOWSTR(aci_str_ptr->wstr_ptr,
                                aci_str_ptr->wstr_len,
                                (uint8 *)s_network_acitable[aci_index].aci_str,
                                aci_str_ptr->wstr_len,
                                aci_str_ptr->wstr_len
                                );
    }
    else
    {
        sprintf((char *)aic_info, "%03d", service_status->plmn.mcc);
        aci_str_ptr->wstr_len = SCI_STRLEN((char *)aic_info);
        aci_str_ptr->wstr_len = MIN(aci_str_ptr->wstr_len, max_aci_len);
        MMI_STRNTOWSTR(aci_str_ptr->wstr_ptr,
                                    aci_str_ptr->wstr_len,
                                    (uint8 *)aic_info,
                                    aci_str_ptr->wstr_len,
                                    aci_str_ptr->wstr_len
                                    );
    }
}

/*****************************************************************************/
// 	Description : get opn display string
//	Global resource dependence : 
//  Author: wancan.you
//	Note:  
/*****************************************************************************/
LOCAL void GetOpnString(MN_DUAL_SYS_E dual_sys,//IN:
                                            MMI_STRING_T *opn_str_ptr,//IN/OUT
                                            uint16 max_opn_len
                                            )
{
#if !defined(MMI_NETWORK_NAME_SUPPORT)
    PHONE_SERVICE_STATUS_T  *service_status = &g_service_status[dual_sys];         //the status of service
    int i =0;
    if(PNULL == opn_str_ptr ||dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"GetOpnString dual_sys = %d OR opn_str_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5652_112_2_18_2_45_11_103,(uint8*)"d", dual_sys);
        return;
    }
   SCI_TRACE_BUF("zhangqiang test  service_status->opn ",service_status->opn,strlen((char *)service_status->opn));
    if (service_status->is_opn_exist && 0 < service_status->opn_len)
    {
        if (MN_OPN_DCS_8BIT == service_status->opn_dcs)
        {
            opn_str_ptr->wstr_len = MIN(service_status->opn_len, max_opn_len);

            opn_str_ptr->wstr_len = MMIAPICOM_Default2Wchar(
                                                            (uint8 *)service_status->opn,
                                                            opn_str_ptr->wstr_ptr,
                                                            opn_str_ptr->wstr_len
                                                            );
        }
        else
        {
            opn_str_ptr->wstr_len = MIN(service_status->opn_len/sizeof(wchar), max_opn_len);
            MMI_WSTRNCPY(opn_str_ptr->wstr_ptr,
                                    opn_str_ptr->wstr_len,
                                    (wchar *)service_status->opn,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_len
                                    );
            for(i = 0; i < opn_str_ptr->wstr_len; i++)
            {
                opn_str_ptr->wstr_ptr[i] = (opn_str_ptr->wstr_ptr[i] & 0xFF00)>>8 | (opn_str_ptr->wstr_ptr[i] & 0xFF)<<8;
            }
        }
    }
    else
    {
        MMIPHONE_NV_NETWORK_NAME_T* nv_network_name = MMIPHONE_GetNVNetworkNameInfo();
            //是否相同卡,同一张卡才能使用之前的OPN
        if(MMIAPIPHONE_IsSameSim(dual_sys)
            &&(nv_network_name[dual_sys].is_opn_exist)
            &&(0 < nv_network_name[dual_sys].opn_len))
        {
            if (MN_OPN_DCS_UCS2 == nv_network_name[dual_sys].opn_dcs)
            {
                opn_str_ptr->wstr_len = MIN(nv_network_name[dual_sys].opn_len/sizeof(wchar), max_opn_len);

                MMI_WSTRNCPY(opn_str_ptr->wstr_ptr,
                                        opn_str_ptr->wstr_len,
                                        (wchar *)nv_network_name[dual_sys].opn,
                                        opn_str_ptr->wstr_len,
                                        opn_str_ptr->wstr_len
                                        );
                for(i = 0; i < opn_str_ptr->wstr_len; i++)
                {
                    opn_str_ptr->wstr_ptr[i] = (opn_str_ptr->wstr_ptr[i] & 0xFF00)>>8 | (opn_str_ptr->wstr_ptr[i] & 0xFF)<<8;
                }
            }
            else
            {
                opn_str_ptr->wstr_len = MIN(nv_network_name[dual_sys].opn_len, max_opn_len);

                opn_str_ptr->wstr_len = MMIAPICOM_Default2Wchar(
                                                                (uint8 *)nv_network_name[dual_sys].opn,
                                                                opn_str_ptr->wstr_ptr,
                                                                opn_str_ptr->wstr_len
                                                                );
            }
        }
        else
        {
            opn_str_ptr->wstr_len = max_opn_len;

            MMIAPIPHONE_GenPLMNDisplay(
                                                    MMIAPIPHONE_GetTDOrGsm(dual_sys),
                                                    &service_status->plmn,
                                                    opn_str_ptr->wstr_ptr,
                                                    &opn_str_ptr->wstr_len,
                                                    FALSE);
        }
    }
#endif
}

/*****************************************************************************/
// 	Description : get spn display string
//	Global resource dependence : 
//  Author: wancan.you
//	Note:  
/*****************************************************************************/
LOCAL void GetSpnString(MN_DUAL_SYS_E dual_sys,//IN:
                                            MMI_STRING_T *spn_str_ptr,//IN/OUT
                                            uint16 max_spn_len
                                            )
{
#if !defined(MMI_NETWORK_NAME_SUPPORT)
    PHONE_SERVICE_STATUS_T  *service_status = &g_service_status[dual_sys];         //the status of service

    if(PNULL == spn_str_ptr || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"GetOpnString dual_sys = %d OR spn_str_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5736_112_2_18_2_45_11_104,(uint8*)"d", dual_sys);
        return;
    }

    if (0 < service_status->spn_len)
    {
        MMIAPICOM_ParseSimStr(service_status->spn,
                                            service_status->spn_len,
                                            spn_str_ptr,
                                            max_spn_len);
    }
#endif    
}

/*****************************************************************************/
// 	Description : compare spn and opn
//	Global resource dependence : 
//  Author: kun.yu
//	Note:  
/*****************************************************************************/
LOCAL BOOLEAN MMIPHONE_IsSPNEqualtoOPN(MMI_STRING_T *opn_str_ptr,//IN:
                                                                                    MMI_STRING_T *spn_str_ptr//IN:
                                                                                    )
{
    BOOLEAN result = FALSE;

    if((PNULL == opn_str_ptr->wstr_ptr) || (PNULL == spn_str_ptr->wstr_ptr))
    {
        return FALSE;
    }
    
    if(0 == MMIAPICOM_CompareTwoWstrExt(opn_str_ptr->wstr_ptr,
                                                        opn_str_ptr->wstr_len,
                                                        spn_str_ptr->wstr_ptr,
                                                        spn_str_ptr->wstr_len,
                                                        FALSE
                                                        ))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : cat opn spn display string
//	Global resource dependence : 
//   Author: kun.yu
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_CatAciOpnSpnString(MMI_STRING_T *aci_str_ptr,//IN 
                                        MMI_STRING_T *opn_str_ptr,//IN:
                                        MMI_STRING_T *spn_str_ptr,//IN:
                                        MMI_STRING_T *network_str_ptr//OUT
                                        )
{
    const char *lnk = "_";
    uint16 length = 0;

    if (aci_str_ptr->wstr_len + opn_str_ptr->wstr_len + spn_str_ptr->wstr_len >= MMIPHONE_MAX_OPER_NAME_LEN - 1)
    {
        return;
    }

    if(aci_str_ptr->wstr_len > 0)
    {
        MMI_WSTRNCPY(network_str_ptr->wstr_ptr,
                                aci_str_ptr->wstr_len,
                                aci_str_ptr->wstr_ptr,
                                aci_str_ptr->wstr_len,
                                aci_str_ptr->wstr_len
                                );

        network_str_ptr->wstr_len = aci_str_ptr->wstr_len;
    }

    if (opn_str_ptr->wstr_len > 0)
    {
        if(aci_str_ptr->wstr_len > 0)
        {
            length = strlen(lnk);
    
            MMI_STRNTOWSTR(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                                    length,
                                    (uint8 *)lnk,
                                    length,
                                    length
                                    );

            network_str_ptr->wstr_len += length;

            //opn
            MMI_WSTRNCPY(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_ptr,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_len
                                    );
            network_str_ptr->wstr_len += opn_str_ptr->wstr_len;

        }
        else
        {
            MMI_WSTRNCPY(network_str_ptr->wstr_ptr,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_ptr,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_len
                                    );

            network_str_ptr->wstr_len = opn_str_ptr->wstr_len;
        }
    }

	//select one of spn&opn to display CR 510183
    if ((spn_str_ptr->wstr_len > 0) && (!MMIPHONE_IsSPNEqualtoOPN(opn_str_ptr, spn_str_ptr)))
    {
        if (opn_str_ptr->wstr_len > 0)
        {
            length = strlen(lnk);
            
            MMI_STRNTOWSTR(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                                    length,
                                    (uint8 *)lnk,
                                    length,
                                    length
                                    );
            
            network_str_ptr->wstr_len += length;
        }

        MMI_WSTRNCPY(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                                spn_str_ptr->wstr_len,
                                spn_str_ptr->wstr_ptr,
                                spn_str_ptr->wstr_len,
                                spn_str_ptr->wstr_len
                                );

        network_str_ptr->wstr_len += spn_str_ptr->wstr_len;
    }

    return;
}

/*****************************************************************************/
// 	Description : cat ppn display string
//	Global resource dependence : 
//   Author: sam.hua
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_CatAciPpnString(MMI_STRING_T *aci_str_ptr,//IN 
                                                MMI_STRING_T *ppn_str_ptr,//IN:
                                                MMI_STRING_T *network_str_ptr//OUT
                                                )
{
    const char *lnk = "_";
    uint16 length = 0;

    if (aci_str_ptr->wstr_len + ppn_str_ptr->wstr_len  >= MMIPHONE_MAX_OPER_NAME_LEN - 1)
    {
        return;
    }

    if (aci_str_ptr->wstr_len > 0)
    {
        MMI_WSTRNCPY(network_str_ptr->wstr_ptr,
                                aci_str_ptr->wstr_len,
                                aci_str_ptr->wstr_ptr,
                                aci_str_ptr->wstr_len,
                                aci_str_ptr->wstr_len
                                );

        network_str_ptr->wstr_len = aci_str_ptr->wstr_len;
    }

    if (ppn_str_ptr->wstr_len > 0)
    {
        if(aci_str_ptr->wstr_len > 0)
        {
            length = strlen(lnk);
    
            MMI_STRNTOWSTR(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                                    length,
                                    (uint8 *)lnk,
                                    length,
                                    length
                                    );

            network_str_ptr->wstr_len += length;

            //ppn
            MMI_WSTRNCPY(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                                    ppn_str_ptr->wstr_len,
                                    ppn_str_ptr->wstr_ptr,
                                    ppn_str_ptr->wstr_len,
                                    ppn_str_ptr->wstr_len
                                    );
            network_str_ptr->wstr_len += ppn_str_ptr->wstr_len;

        }
        else
        {
            MMI_WSTRNCPY(network_str_ptr->wstr_ptr,
                                    ppn_str_ptr->wstr_len,
                                    ppn_str_ptr->wstr_ptr,
                                    ppn_str_ptr->wstr_len,
                                    ppn_str_ptr->wstr_len
                                    );

            network_str_ptr->wstr_len = ppn_str_ptr->wstr_len;
        }
    }

}

/*****************************************************************************/
// 	Description : is network selecting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsNetworkSelecting(MN_DUAL_SYS_E dual_sys)
{
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;

    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsNetworkSelecting dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5905_112_2_18_2_45_12_105,(uint8*)"d",dual_sys);
        return FALSE;
    }
    
    plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);

    //SCI_TRACE_LOW:"MMIAPIPHONE_IsNetworkSelecting plmn_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5911_112_2_18_2_45_12_106,(uint8*)"d", plmn_status);
    
    if(( PLMN_REGISTER_GPRS_ONLY == plmn_status)
        || ( PLMN_REGISTER_SERVICE == plmn_status)
        || g_service_status[dual_sys].is_slecting)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
typedef enum
{
    DISPLAY_VALID_SIM,
    DISPLAY_INVALID_SIM,
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
	DISPLAY_LOCKED_SIM,
#endif
    DISPLAY_NO_SIM,
    DISPLAY_SIM_MAX
}DISPLAY_SIM_TYPE_T;

typedef enum
{
    DISPLAY_NORMAL,
    DISPLAY_LIMITED,
    DISPLAY_NO_SERVICE,
    DISPLAY_SERVICE_MAX
}DISPLAY_SERVICE_TYPE_T;

typedef enum
{
    DISPLAY_RESULT_NO_SIM_CARD,
    DISPLAY_RESULT_LIMITED_SERVICE,
    DISPLAY_RESULT_NO_SERVICE,
    DISPLAY_RESULT_EMPTY,
    DISPLAY_RESULT_GET_NAME,
    DISPLAY_RESULT_NO_SIM_AND_LIMITED_SERVICE,
    DISPLAY_RESULT_NO_SIM_AND_NO_SERVICE,
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
	DISPLAY_RESULT_SIM_LOCKED,
#endif
    DISPLAY_RESULT_MAX
}SERVICE_DISPLAY_RESULT_T;

/*
0: General    1: Australia  2: China
	service_type	Registered Network					limited service							No Service
sim_type				
SIM Valid			Operator Name(0/1/2)				Empty(0)/Emergency calls only(1/2)		Empty(0/1)/No service(2)
SIM Invalid			Empty(0)/Emergency calls only(1/2)	Empty(0)/Emergency calls only(1/2)		Empty(0/1)/No service(2)
DISPLAY_LOCKED_SIM	SIM LOCKED(0/1/2)					SIM LOCKED(0/1/2)						SIM LOCKED(0/1/2)
No SIM				No SIM Card							No SIM Card(0/2)/Emergency calls only(1)No SIM Card
*/

//General
const uint16 general_display[DISPLAY_SIM_MAX][DISPLAY_SERVICE_MAX] ={
    {DISPLAY_RESULT_GET_NAME,         DISPLAY_RESULT_EMPTY,              DISPLAY_RESULT_EMPTY},
    {DISPLAY_RESULT_EMPTY,            DISPLAY_RESULT_EMPTY,              DISPLAY_RESULT_EMPTY},
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    {DISPLAY_RESULT_SIM_LOCKED,       DISPLAY_RESULT_SIM_LOCKED,         DISPLAY_RESULT_SIM_LOCKED},
#endif
    {DISPLAY_RESULT_NO_SIM_CARD,      DISPLAY_RESULT_NO_SIM_CARD,        DISPLAY_RESULT_NO_SIM_CARD},
};

//Australlia
const SERVICE_DISPLAY_RESULT_T australlia_display[DISPLAY_SIM_MAX][DISPLAY_SERVICE_MAX] ={
    {DISPLAY_RESULT_GET_NAME,         DISPLAY_RESULT_LIMITED_SERVICE,    DISPLAY_RESULT_NO_SERVICE},
    {DISPLAY_RESULT_LIMITED_SERVICE,  DISPLAY_RESULT_LIMITED_SERVICE,    DISPLAY_RESULT_NO_SERVICE},
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    {DISPLAY_RESULT_SIM_LOCKED,       DISPLAY_RESULT_SIM_LOCKED,         DISPLAY_RESULT_SIM_LOCKED},
#endif
	{DISPLAY_RESULT_NO_SIM_CARD,      DISPLAY_RESULT_NO_SIM_CARD,        DISPLAY_RESULT_NO_SIM_CARD}
};

//China
const SERVICE_DISPLAY_RESULT_T china_display[DISPLAY_SIM_MAX][DISPLAY_SERVICE_MAX] ={
    {DISPLAY_RESULT_GET_NAME,         DISPLAY_RESULT_LIMITED_SERVICE,    DISPLAY_RESULT_NO_SERVICE},
    {DISPLAY_RESULT_LIMITED_SERVICE,  DISPLAY_RESULT_LIMITED_SERVICE,    DISPLAY_RESULT_NO_SERVICE},
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    {DISPLAY_RESULT_SIM_LOCKED,       DISPLAY_RESULT_SIM_LOCKED,         DISPLAY_RESULT_SIM_LOCKED},
#endif
    {DISPLAY_RESULT_NO_SIM_CARD,      DISPLAY_RESULT_NO_SIM_CARD,        DISPLAY_RESULT_NO_SIM_CARD}
};

//New Sealand
const SERVICE_DISPLAY_RESULT_T newsealand_display[DISPLAY_SIM_MAX][DISPLAY_SERVICE_MAX] ={
    {DISPLAY_RESULT_GET_NAME,         DISPLAY_RESULT_EMPTY,    DISPLAY_RESULT_EMPTY},
    {DISPLAY_RESULT_EMPTY,            DISPLAY_RESULT_EMPTY,    DISPLAY_RESULT_EMPTY},
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    {DISPLAY_RESULT_SIM_LOCKED,       DISPLAY_RESULT_SIM_LOCKED,         DISPLAY_RESULT_SIM_LOCKED},
#endif
    {DISPLAY_RESULT_EMPTY,            DISPLAY_RESULT_EMPTY,    DISPLAY_RESULT_EMPTY},
};

const SERVICE_DISPLAY_RESULT_T vodafone_display[DISPLAY_SIM_MAX][DISPLAY_SERVICE_MAX] ={
    {DISPLAY_RESULT_GET_NAME,         DISPLAY_RESULT_LIMITED_SERVICE,    DISPLAY_RESULT_NO_SERVICE},
    {DISPLAY_RESULT_LIMITED_SERVICE,  DISPLAY_RESULT_LIMITED_SERVICE,    DISPLAY_RESULT_NO_SERVICE},
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
    {DISPLAY_RESULT_SIM_LOCKED,       DISPLAY_RESULT_SIM_LOCKED,         DISPLAY_RESULT_SIM_LOCKED},
#endif
    {DISPLAY_RESULT_NO_SIM_AND_LIMITED_SERVICE, DISPLAY_RESULT_NO_SIM_AND_LIMITED_SERVICE, DISPLAY_RESULT_NO_SIM_AND_NO_SERVICE},
};
/*****************************************************************************/
// 	Description : cat opn spn display string
//	Global resource dependence : 
//   Author: kun.yu
//	Note:  
/*****************************************************************************/
LOCAL void MMIPHONE_CatSimAndServiceStr(MMI_STRING_T *aci_str_ptr,//IN 
                                        MMI_STRING_T *opn_str_ptr,//IN:
                                        MMI_STRING_T *network_str_ptr//OUT
                                        )
{
    const char *lnk = "-";
    uint16 length = 0;

    if (aci_str_ptr->wstr_len + opn_str_ptr->wstr_len >= MMIPHONE_MAX_OPER_NAME_LEN - 1)
    {
        return;
    }

    if (aci_str_ptr->wstr_len > 0)
    {
        MMI_WSTRNCPY(network_str_ptr->wstr_ptr,
                     aci_str_ptr->wstr_len,
                     aci_str_ptr->wstr_ptr,
                     aci_str_ptr->wstr_len,
                     aci_str_ptr->wstr_len
                     );

        network_str_ptr->wstr_len = aci_str_ptr->wstr_len;
    }

    if (opn_str_ptr->wstr_len > 0)
    {
        if (aci_str_ptr->wstr_len > 0)
        {
            length = strlen(lnk);
    
            MMI_STRNTOWSTR(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                           length, (uint8 *)lnk, length, length);

            network_str_ptr->wstr_len += length;

            //opn
            MMI_WSTRNCPY(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_ptr,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_len
                                    );
            network_str_ptr->wstr_len += opn_str_ptr->wstr_len;

        }
        else
        {
            MMI_WSTRNCPY(network_str_ptr->wstr_ptr,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_ptr,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_len
                                    );

            network_str_ptr->wstr_len = opn_str_ptr->wstr_len;
        }
    }

    return;
}

LOCAL void MMIAPIPHONE_GetNetworkNameStr(MN_DUAL_SYS_E dual_sys, DISPLAY_SIM_TYPE_T sim_type, 
                                         DISPLAY_SERVICE_TYPE_T service_type, MMI_STRING_T *network_str_ptr)
{
    uint8   display_type = 0;
    MMI_STRING_T  sim_str = {0}; 
    MMI_STRING_T  service_str = {0}; 
    SERVICE_DISPLAY_RESULT_T dsiplay_result = DISPLAY_RESULT_GET_NAME;
    
    display_type =  MMI_DNT_ReadEmergencyCallUIConfigure();
    switch (display_type)      
    {
        case 0:
            dsiplay_result = general_display[sim_type][service_type];
            break;
        case 1:
            dsiplay_result = china_display[sim_type][service_type];
            break;
        case 2:
            dsiplay_result = australlia_display[sim_type][service_type];
            break;
        case 3:
            dsiplay_result = newsealand_display[sim_type][service_type];
            break;
        default:
            return;
    }
	
    if (MMIAPISET_IsVodafoneVersion())
	{
	    dsiplay_result = vodafone_display[sim_type][service_type];
	}
	
    SCI_TRACE_LOW("MMIAPIPHONE_GetSimAndServiceString IDLE SHOW:display_type [%d], sim type[%d], service type[%d], dsiplay_result[%x]",
                  display_type, sim_type, service_type, dsiplay_result);
                  
    switch (dsiplay_result)
    {
        case DISPLAY_RESULT_NO_SIM_CARD:
            MMI_GetLabelTextByLang(STR_NSM_IND_NO_SIM, network_str_ptr);
            break;
			
        case DISPLAY_RESULT_LIMITED_SERVICE:
            MMI_GetLabelTextByLang(STXT_EMERGENCY_ONLY, network_str_ptr);
            break;
			
        case DISPLAY_RESULT_NO_SERVICE:
            MMI_GetLabelTextByLang(STR_SIM_NO_SERVICE_EXT01, network_str_ptr);
            break; 
			
        case DISPLAY_RESULT_EMPTY:
            network_str_ptr->wstr_len = 0;
            break;

		case DISPLAY_RESULT_NO_SIM_AND_LIMITED_SERVICE:
	        MMI_GetLabelTextByLang(STR_NSM_IND_NO_SIM, &sim_str);
	        MMI_GetLabelTextByLang(STXT_EMERGENCY_ONLY, &service_str);
	        MMIPHONE_CatSimAndServiceStr(&sim_str, &service_str, network_str_ptr);
			break;
			
		case DISPLAY_RESULT_NO_SIM_AND_NO_SERVICE:
	        MMI_GetLabelTextByLang(STR_NSM_IND_NO_SIM, &sim_str);
	        MMI_GetLabelTextByLang(STR_SIM_NO_SERVICE_EXT01, &service_str);
	        MMIPHONE_CatSimAndServiceStr(&sim_str, &service_str, network_str_ptr);
			break;

#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
		case DISPLAY_RESULT_SIM_LOCKED:
	        MMI_GetLabelTextByLang(TXT_SIM_LOCKED_INVALID, &sim_str);
	        // MMI_GetLabelTextByLang(STR_SIM_NO_SERVICE_EXT01, &service_str);
	        MMIPHONE_CatSimAndServiceStr(&sim_str, &service_str, network_str_ptr);
			break;
#endif
			
        case DISPLAY_RESULT_GET_NAME:
            {
#ifdef MMI_NETWORK_NAME_SUPPORT
                MMIPHONE_GetNetworkNameString(dual_sys,network_str_ptr);
#else
                MMI_STRING_T aci_string = {0}; 
                MMI_STRING_T opn_string = {0};
                MMI_STRING_T spn_string = {0};
                wchar aci[MAX_ACI_INFO_LEN + 1] = {0};
                wchar opn[MAX_ONSHORTFORM_LEN + 1] = {0};
                wchar spn[MN_SPN_ID_NUM_LEN + 1] = {0};
                //get aci string
                if(g_service_status[dual_sys].is_add_ci)
                {
                    aci_string.wstr_ptr = (wchar *)aci;
                    MMIPHONE_GetAciString(&g_service_status[dual_sys], &aci_string, MAX_ACI_INFO_LEN);
                }
                
                //get opn string
                if (g_service_status[dual_sys].is_opn_need)
                {
                    opn_string.wstr_ptr = (wchar *)opn;
                    GetOpnString(dual_sys, &opn_string, MAX_ONSHORTFORM_LEN);
                }
                
                //get spn string
                if (g_service_status[dual_sys].is_spn_need)
                {
                    spn_string.wstr_ptr = (wchar *)spn;
                    GetSpnString(dual_sys, &spn_string, MN_SPN_ID_NUM_LEN);
                }
                
                MMIPHONE_CatAciOpnSpnString(&aci_string, &opn_string, &spn_string, network_str_ptr);
#endif
                //异常处理
                if (0 == network_str_ptr->wstr_len)
                {
                    network_str_ptr->wstr_len = MMIPHONE_MAX_OPER_NAME_LEN;
                    
                    MMIAPIPHONE_GenPLMNDisplay(MMIAPIPHONE_GetTDOrGsm(dual_sys),
                                               &g_service_status[dual_sys].plmn,
                                               network_str_ptr->wstr_ptr,
                                               &network_str_ptr->wstr_len,
                                               FALSE);
                }
            }
            break;
    }
    
    return;
}

/*****************************************************************************/
// 	Description : get sim card and plmn service status string to display in idle
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetSimAndServiceString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *network_str_ptr)
{
    MMI_TEXT_ID_T	idle_str_id = TXT_NULL;
    SIM_STATUS_E sim_status = MMIAPIPHONE_GetSimStatus(dual_sys);
    uint8   display_type = 0;
    DISPLAY_SIM_TYPE_T   sim_type=DISPLAY_VALID_SIM;
    DISPLAY_SERVICE_TYPE_T service_type = DISPLAY_NORMAL;    
    SERVICE_DISPLAY_RESULT_T dsiplay_result=DISPLAY_RESULT_GET_NAME;
    
    SCI_TRACE_LOW("MMIAPIPHONE_GetSimAndServiceString dual_sys= %d,sim_status = %d",dual_sys,sim_status);
    if(PNULL == network_str_ptr  ||PNULL == network_str_ptr->wstr_ptr|| dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"GetSimAndServiceString: the network_str_ptr || network_str_ptr->wstr_ptr ||dual_sys"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5936_112_2_18_2_45_12_107,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIAPIPHONE_GetSimAndServiceString s_sim_status[%d].simStatus %d,is_opn_need=%d,opn_len=%d,is_opn_exist=%d,is_spn_need=%d,spn_len=%d"
#if !defined(MMI_NETWORK_NAME_SUPPORT)
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5942_112_2_18_2_45_12_108,(uint8*)"ddddddd",dual_sys, sim_status,g_service_status[dual_sys].is_opn_need,g_service_status[dual_sys].opn_len,g_service_status[dual_sys].is_opn_exist,g_service_status[dual_sys].is_spn_need,g_service_status[dual_sys].spn_len);
#endif
    SCI_TRACE_LOW("MMIAPIPHONE_GetSimAndServiceString s_sim_status[%d].simStatus %d,plmn_status=%d",dual_sys,sim_status,g_service_status[dual_sys].plmn_status);
    network_str_ptr->wstr_len = 0;
    
    if (sim_status >= SIM_STATUS_MAX)
    {
        return;
    }
    display_type =  MMI_DNT_ReadEmergencyCallUIConfigure();
    
    //check No  SIM at first
    if(sim_status == SIM_STATUS_NO_SIM)
    {
        sim_type = DISPLAY_NO_SIM;
    }
    else if(sim_status == SIM_STATUS_OK) //sim is Invalid
    {
        sim_type = DISPLAY_VALID_SIM;
    }
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT)
	else if(sim_status == SIM_STATUS_LOCKED_INVALID) 
    {
		//sim is locked
        sim_type = DISPLAY_LOCKED_SIM;
    }
#endif
    else
    {
        sim_type = DISPLAY_INVALID_SIM;
    }
    //check service status
    switch(g_service_status[dual_sys].plmn_status)
    {
    case PLMN_NO_SERVICE:
        service_type = DISPLAY_NO_SERVICE;
        break;
    case PLMN_NORMAL_GPRS_ONLY:
    case PLMN_EMERGENCY_ONLY:
    case PLMN_EMERGENCY_SIM_INVALID:
    case PLMN_EMERGENCY_GPRS_ONLY:
    case PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY:
        service_type = DISPLAY_LIMITED;
        break;
    case PLMN_FULL_PS_SERVICE:
    case PLMN_REGISTER_GPRS_ONLY:
    case PLMN_REGISTER_SERVICE:                    
    case PLMN_NORMAL_GSM_ONLY:
    case PLMN_NORMAL_GSM_GPRS_BOTH:
        service_type = DISPLAY_NORMAL;
        break;
    default:
        service_type = DISPLAY_LIMITED;
    }

    MMIAPIPHONE_GetNetworkNameStr(dual_sys, sim_type, service_type, network_str_ptr);
    
    return;
}

/*****************************************************************************/
// 	Description : 获得sim card状态
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC SIM_STATUS_E MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetSimStatus err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6032_112_2_18_2_45_12_109,(uint8*)"d", dual_sys);
        return SIM_NOT_INITED;
    }
    
    SCI_TRACE_LOW("MMIAPIPHONE_GetSimStatus = %d", s_sim_status[dual_sys].simStatus);
    return s_sim_status[dual_sys].simStatus;
}

/*****************************************************************************/
// 	Description : 设置sim card状态
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_SetSimStatus(MN_DUAL_SYS_E dual_sys, SIM_STATUS_E sim_status)
{
    //SCI_TRACE_LOW:"MMIPHONE_SetSimStatus:dual sys = %d, dual status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6045_112_2_18_2_45_12_110,(uint8*)"dd",dual_sys,sim_status);

    if(dual_sys < MMI_DUAL_SYS_MAX)
    {

            s_sim_status[dual_sys].simStatus = sim_status;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIPHONE_SetSimStatus err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6053_112_2_18_2_45_12_111,(uint8*)"d", dual_sys);
    }
}

/*****************************************************************************/
// 	Description : set sim card exited status
//	Global resource dependence : 
//  Author:kelly.li
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_SetSimExistedStatus(
                                            MN_DUAL_SYS_E dual_sys,
                                            BOOLEAN       is_existed
                                            )
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_SetSimExistedStatus:dual sys = %d, is_existed = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6066_112_2_18_2_45_12_112,(uint8*)"dd",dual_sys,is_existed);
    
    if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        s_sim_existed_status[dual_sys] = is_existed;

        if (MMIAPIIDLE_IdleWinIsOpen())
        {
            MMIAPICOM_StatusAreaUpdateSignalState();
        }
        return TRUE;
    }
    else
    {

        //SCI_TRACE_LOW:"MMIAPIPHONE_SetSimExistedStatus:err dual sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6081_112_2_18_2_45_12_113,(uint8*)"d",dual_sys);
        return FALSE;
    }

}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetSimExistedStatusIncBootUP
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetSimExistedStatusIncBootUP(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }
    //Init phase,use another flag to get sim info
    if (SIM_NOT_INITED == MMIAPIPHONE_GetSimStatus(dual_sys))
    {
        if(s_handle_pin_status.need_handle_pin[dual_sys] || s_handle_pin_status.is_handling_pin[dual_sys])
        {
            return TRUE;
        }
        return FALSE;
    }
    
    return(s_sim_existed_status[dual_sys]);
}

/*****************************************************************************/
// 	Description : Get sim card exited status
//	Global resource dependence : 
//  Author:kelly.li
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }
    
    return(s_sim_existed_status[dual_sys]);
}


/*****************************************************************************/
// 	Description : Get sim card exited status after power on.
//	Global resource dependence : 
//  Author:sam.hua
//	Note:  s_sim_existed_status is set to True after power on,
//             so s_is_sim_present_ind should be checked at first
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetSimExistedStatusEx(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }
    //Init phase,use another flag to get sim info
    if (SIM_NOT_INITED == MMIAPIPHONE_GetSimStatus(dual_sys))
    {
        if(s_handle_pin_status.need_handle_pin[dual_sys] ||s_handle_pin_status.is_handling_pin[dual_sys])
        {
            return TRUE;
        }
        return FALSE;
    }
    
    return(s_sim_existed_status[dual_sys]);
}

/*****************************************************************************/
// 	Description : Sim卡是否OK，包括PS
//	Global resource dependence : 
//  Author:
//	Note:  SIM OK 的状态: 'SIM_STATUS_OK' and 'PS_Ready_Normal' and '!fly_Mode'
//     Pay attention to this interface used,should better considered MMIAPIPHONE_IsSimAvailable     
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_E dual_sys)
{
    return MMIPHONE_IsSimOk(dual_sys);
}

/*****************************************************************************/
// 	Description : Sim卡是否OK，包括PS
//	Global resource dependence : 
//  Author:
//	Note:  SIM OK 的状态: 'SIM_STATUS_OK' and 'PS_Ready_Normal' and '!fly_Mode'
//     Pay attention to this interface used,should better considered MMIAPIPHONE_IsSimAvailable     
/*****************************************************************************/
BOOLEAN MMIPHONE_IsSimOk(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsSimOk err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6120_112_2_18_2_45_12_114,(uint8*)"d", dual_sys);
        return FALSE;
    }
    
    return((BOOLEAN)(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(dual_sys)
                                && (MMIPHONE_PS_POWER_ON_NORMAL == s_ps_ready[dual_sys]) && !MMIAPISET_GetFlyMode()));
}

/*****************************************************************************/
// 	Description : Sim卡是否OK，不包括PS
//	Global resource dependence : 
//  Author:
//	Note:  SIM Card OK 的状态包括两种: SIM_STATUS_OK / SIM_STATUS_REGISTRATION_FAILED
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsSimCardOk err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6136_112_2_18_2_45_12_115,(uint8*)"d", dual_sys);
        return FALSE;
    }

    
    return((BOOLEAN)(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(dual_sys)
                                || SIM_STATUS_REGISTRATION_FAILED == MMIAPIPHONE_GetSimStatus(dual_sys)));
}

/*****************************************************************************/
// 	Description : Sim卡是否可用
//	Global resource dependence : 
//  Author: Michael.shi
//	Note:  SIM 卡是否可用的判断条件 : sim 卡存在并且处于待机设置！！
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsSimAvailable err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6153_112_2_18_2_45_12_116,(uint8*)"d", dual_sys);
        return FALSE;
    }

    // 非飞行模式才做处理!!!
    if( !MMIAPISET_GetFlyMode() )
    {
        MMISET_DUALSYS_TYPE_E e_dual_sys = MMIAPISET_GetMultiSysSetting();
        //sim 卡存在并且待机!!!
        if ((e_dual_sys & (1<<dual_sys)) && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
        {
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
			if ((dual_sys == MN_DUAL_SYS_1 && !s_is_sim1_right) || (dual_sys == MN_DUAL_SYS_2 && !s_is_sim2_right))
			{
				return FALSE;
			}
#endif
            return TRUE;
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
// 	Description : 可用的Sim卡个数
//	Global resource dependence : 
//  Author: Michael.Shi
//	Note:  获取存在并且设置为待机的SIM卡的个数
/*****************************************************************************/
uint32 MMIAPIPHONE_GetSimAvailableNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_IsSimAvailable(i))
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
// 	Description : Sim卡OK个数和卡，包括PS
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetSimOkNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_IsSimOk(i))
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
// 	Description : Sim卡OK个数和卡，不包括PS
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetSimCardOkNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_IsSimCardOk(i))
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
// 	Description : Get sim card existed status and num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetSimExistedNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(i))
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
// 	Description : Get ps ready status and num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetPSReadyNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if ((MMIPHONE_PS_POWER_ON_NONE != s_ps_ready[i]))
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
// 	Description : get sys ok num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetSysOkNum(BOOLEAN  *p, uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    if(PNULL == p)
    {
        //SCI_TRACE_LOW:"GetSysOkNum: the p=%d || idx_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6302_112_2_18_2_45_13_117,(uint8*)"dd", p, idx_ptr);
        return 0;
    }

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (p[i])
        {
            if ((idx_ptr != PNULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : get national roaming num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetNationalRoamingNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_GetRoamingStatus(i) != MMIPHONE_ROAMING_STATUS_NO_ROMING)
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
// 	Description : pop up an alert window to show sim wrong status
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
void MMIAPIPHONE_AlertSimNorOKStatusII(MN_DUAL_SYS_E dual_sys)
{
    MMI_TEXT_ID_T 	text_id = TXT_NULL;
    MMI_WIN_ID_T	alert_win_id = MMIPHONE_ALERT_SIM_NOT_OK_WIN_ID;
    SIM_STATUS_E sim_status = MMIAPIPHONE_GetSimStatus(dual_sys);
    MMI_IMAGE_ID_T image_id = IMAGE_PUBWIN_WARNING;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    
    if(sim_status >= SIM_STATUS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_AlertSimNorOKStatus:error simStatus=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6359_112_2_18_2_45_13_118,(uint8*)"d", sim_status);
        return;
    }
    
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_AlertSimNorOKStatus:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6365_112_2_18_2_45_13_119,(uint8*)"d", dual_sys);
        return;
    }
    
    if (MMIAPISET_GetFlyMode())
    {
        MMIAPISET_AlertFlyMode();
    }    
    else if(!MMIAPIPHONE_GetSimExistedStatus(dual_sys))
    {
        sim_status = SIM_STATUS_NO_SIM;        
        text_id = s_sim_status_string[sim_status];
        
        if (text_id != TXT_NULL)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
    }
    else if (!(e_dualsys_setting & (0x01 << dual_sys)))
    {
        text_id = TXT_SIM_DEACTIVATED;

        MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else
    {
        text_id = s_sim_status_string[sim_status];
        image_id = IMAGE_PUBWIN_WARNING;

        if (TXT_NULL == text_id)
        {
            if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[dual_sys])
            {
                text_id = TXT_SYS_WAITING_AND_TRY_LATER;
                image_id = IMAGE_PUBWIN_WAIT;
            }
        }
        
        if (text_id != TXT_NULL)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
    }
}

//[add display banner win when call exception]
LOCAL void DisplayBannerWin(MMI_WIN_ID_T win_id,MMI_TEXT_ID_T text1_id)
{
    GUI_POINT_T start_point = {0, 0};
    MMICOM_BANNER_INFO_T banner_info = {0};

    banner_info.banner_type = MMI_BANNER_TEXT;
    banner_info.text1_id = text1_id;
    banner_info.start_point = start_point;
    MMIAPICOM_OpenBannerWin(win_id,banner_info);
}
//[end]

/*****************************************************************************/
//  Description : pop up an alert window to show sim wrong status
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:  
/*****************************************************************************/
void MMIAPIPHONE_AlertSimNorOKStatus(void)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    MMI_WIN_ID_T    alert_win_id = MMIPHONE_ALERT_SIM_NOT_OK_WIN_ID;
    SIM_STATUS_E sim_status = SIM_STATUS_OK;
    MMI_IMAGE_ID_T image_id = IMAGE_PUBWIN_WARNING;
    uint16 sys_ok = MN_DUAL_SYS_1;

    if (MMIAPISET_GetFlyMode())
    {
        MMIAPISET_AlertFlyMode();
    }
    else
    {
        uint32 sys_num = MMIAPISET_GetMultiSysSettingNum(&sys_ok, 1) ;
        uint32 exist_sim_num = MMIAPIPHONE_GetSimExistedNum(PNULL,0);

        image_id = IMAGE_PUBWIN_WARNING;

        if (0 == sys_num)
        {
             ///没有可用的SIM 卡，可能是确实没有卡存在，也可能是待机卡为零
             if((0 < exist_sim_num )/* && ( MMI_DUAL_SYS_MAX > exist_sim_num)*/)
             {
                text_id = TXT_INSERT_SIM_AND_INFO;
                //MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                DisplayBannerWin(alert_win_id,text_id);

                return; /// return directly!!
             }
             else
             {
                sim_status = SIM_STATUS_NO_SIM; //// call text id :insert_sim or insert_sim_and_info
             }
        }
        else
        {
            sim_status = MMIAPIPHONE_GetSimStatus((MN_DUAL_SYS_E)sys_ok);
        }
        
        if(sim_status <= SIM_STATUS_MAX)
        {
            text_id = s_sim_status_string[sim_status];
            
            if (text_id != TXT_NULL)
            {
                //[display banner win when call exception]
                //MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                DisplayBannerWin(alert_win_id,text_id);
                //[end]
            }
            else
            {
                if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[sys_ok])
                {
                    text_id = TXT_SYS_WAITING_AND_TRY_LATER;
                    image_id = IMAGE_PUBWIN_WAIT;

                    //MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    DisplayBannerWin(alert_win_id,text_id);
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIPHONE_AlertSimNorOKStatus err sim status: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6474_112_2_18_2_45_13_120,(uint8*)"d", sim_status);
        }
    }
}


/*****************************************************************************/
// 	Description : Is Pin2 Required ACM
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsPin2RequiredACM(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsPin2RequiredACM err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6488_112_2_18_2_45_13_121,(uint8*)"d", dual_sys);
        return FALSE;
    }    
    return s_sim_status[dual_sys].is_pin2_Required_acm;
}

/*****************************************************************************/
// 	Description : Is Pin2 Required ACM Max
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsPin2RequiredACMMax(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsPin2RequiredACMMax err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6502_112_2_18_2_45_13_122,(uint8*)"d", dual_sys);
        return FALSE;
    }    
    
    return s_sim_status[dual_sys].is_pin2_required_acm_max;
}

/*****************************************************************************/
// 	Description : Is Pin2 Required Puct
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsPin2RequiredPuct(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsPin2RequiredPuct err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6517_112_2_18_2_45_13_123,(uint8*)"d", dual_sys);
        return FALSE;
    }    
    
    return s_sim_status[dual_sys].is_pin2_Required_puct;
}

/*****************************************************************************/
// 	Description : Is aoc support
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsAocSupport(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsAocSupport err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6532_112_2_18_2_45_13_124,(uint8*)"d", dual_sys);
        return FALSE;
    }    

#ifdef MMI_DUALMODE_ENABLE
    {
        MNSIM_AOC_T aoc_status = {0};

        aoc_status = MNSIM_GetAocEx(dual_sys);

        //SCI_TRACE_LOW:"MMIAPIPHONE_IsAocSupport aoc_is_support=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6542_112_2_18_2_45_13_125,(uint8*)"d", aoc_status.aoc_is_support);

        return aoc_status.aoc_is_support;
    }
#else
    return s_sim_status[dual_sys].is_aoc_support;
#endif
}

/*****************************************************************************/
// 	Description : get plmn status
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC MN_PHONE_PLMN_STATUS_E MMIAPIPHONE_GetPlmnStatus(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetPlmnStatus err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6559_112_2_18_2_45_13_126,(uint8*)"d", dual_sys);
        return PLMN_NO_SERVICE;
    }    
    
    return g_service_status[dual_sys].plmn_status;
}

/*****************************************************************************/
// 	Description : Get Pin/PUK Remain left times string//MS
//	Global resource dependence : 
//  Author:
//	Note:使用之后需要将控件释放
/*****************************************************************************/
LOCAL void MMIPHONE_PinRemainLeftTimesBanner(BOOLEAN pin_is_blocked)
{
	uint16 pin_remain_times = 0;
    MMI_STRING_T remain_str = {0};
    wchar  pin_remain_times_ptr[10] = {0};
	MMI_STRING_T  pin_string_banner = {0};
	wchar         wstr_old[]  = {L"%N"};
	wchar*       pin_remain_ptr = PNULL;
	uint16 length = 0;
	uint16 length1 = 0;
	MMICOM_BANNER_INFO_T banner_info = {0};
	GUI_POINT_T pin_alert_start_point = {0, 0};
    if(pin_is_blocked)
	{
		pin_remain_times = 0;//PIN码被锁，剩余0次输入机会
	}
	else
	{
        pin_remain_times = MNSIM_GetPinOrPukFalseRemainEx(MMIAPIPHONE_GetCurResponsePinSimID(), MNSIM_PIN1, pin_is_blocked);//剩余次数	    
	}
	SCI_TRACE_LOW("MMIPHONE_PinRemainLeftTimesBanner  pin_remain = %d", pin_remain_times);
	if(0 == pin_remain_times)
	{
		MMI_GetLabelTextByLang(STR_PIN_BLOCKED_EXT01, &pin_string_banner);//提示PIN码被锁
	}
	else if(1 == pin_remain_times)
	{
		MMI_GetLabelTextByLang(STR_INVALID_PIN_TRY_EXT01, &pin_string_banner);//提示剩余1次
	}
	else //剩余次数大于1，需要字串替换
	{
	sprintf((wchar *)pin_remain_times_ptr, "%d", pin_remain_times);
    length = MMIAPICOM_Wstrlen(pin_remain_times_ptr) * sizeof(wchar);
	//获得替换之前字符串
	MMI_GetLabelTextByLang(STR_INVALID_PIN_TRIES_EXT01, &remain_str);//提示字符串
    length = remain_str.wstr_len * sizeof(wchar) + length + 1;
	pin_string_banner.wstr_ptr = (wchar *) SCI_ALLOC_APPZ(length);
	MMIAPICOM_Wstrncpy(pin_string_banner.wstr_ptr, remain_str.wstr_ptr, remain_str.wstr_len);
	pin_remain_ptr = pin_string_banner.wstr_ptr;
	//替换字符串
	MMIAPICOM_WstrReplace(pin_remain_ptr, length, wstr_old, pin_remain_times_ptr);
    pin_string_banner.wstr_len = MMIAPICOM_Wstrlen(pin_string_banner.wstr_ptr);
	}
	if(PNULL != pin_string_banner.wstr_ptr)
	{
		SCI_TRACE_LOW("MMIPHONE_PinRemainLeftTimesBanner PIN open get string success");
	}
	//banner提示
	banner_info.banner_type = MMI_BANNER_STR_TEXT;
    banner_info.text1_str = pin_string_banner;// pin_string_banner;//pin_string_ptr;//text需要加载资源//PIN码输入错误，请重试STR_INFO_IAP_INVALID_PIN
	banner_info.start_point = pin_alert_start_point;
	MMIAPICOM_OpenBannerWin(PHONE_PIN_INPUT_WIN_ID, banner_info);
	if((0 != pin_remain_times) && (1 != pin_remain_times))
	{
        SCI_FREE(pin_string_banner.wstr_ptr);
	}

    GUILABEL_SetText(MMIPHONE_PIN_REMAIN_LABEL_CTRL_ID,&pin_string_banner,TRUE);
}
/*****************************************************************************/
// 	Description : Get Puk Remain left times string//MS
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void MMIPHONE_PukRemainTimesBanner(BOOLEAN pin_is_blocked)
{
	uint16 puk_remain_times = 0;
    MMI_STRING_T remain_str = {0};
    wchar  puk_remain_times_ptr[10] = {0};
	MMI_STRING_T  puk_string_banner = {0};
	wchar         wstr_old[]  = {L"%N"};
	wchar*       puk_remain_ptr = PNULL;
	uint16 length = 0;
	MMICOM_BANNER_INFO_T banner_info = {0};
	GUI_POINT_T puk_alert_start_point = {0, 0};
    puk_remain_times = MNSIM_GetPinOrPukFalseRemainEx(MMIAPIPHONE_GetCurResponsePinSimID(), MNSIM_PIN1, pin_is_blocked);//剩余次数	    
	SCI_TRACE_LOW("MMIPHONE_PukRemainTimesBanner  puk_remain = %d", puk_remain_times);
	if(0 == puk_remain_times)
	{
		MMI_GetLabelTextByLang(STR_SIM_BLOCKED_EXT01, &puk_string_banner);//提示PUK码被锁
	}
	else if(1 == puk_remain_times)
	{
		MMI_GetLabelTextByLang(STR_INVALID_PUK_TRY_EXT01, &puk_string_banner);//提示剩余1次
	}
	else //剩余次数大于1，需要字串替换
	{
	    //获得替换之前字符串
	    sprintf((wchar *)puk_remain_times_ptr, "%d", puk_remain_times);
        length = MMIAPICOM_Wstrlen(puk_remain_times_ptr) * sizeof(wchar);
	    MMI_GetLabelTextByLang(STR_INVALID_PUK_TRIES_EXT01, &remain_str);//提示字符串
        length = remain_str.wstr_len * sizeof(wchar) + length + 1;
	    puk_string_banner.wstr_ptr = (wchar *) SCI_ALLOC_APPZ(length);
	    MMIAPICOM_Wstrcpy(puk_string_banner.wstr_ptr, remain_str.wstr_ptr);
	    puk_remain_ptr = puk_string_banner.wstr_ptr;
	    //替换字符串
	    MMIAPICOM_WstrReplace(puk_remain_ptr, length, wstr_old, puk_remain_times_ptr);
        puk_string_banner.wstr_len = MMIAPICOM_Wstrlen(puk_string_banner.wstr_ptr);
	}
	if(PNULL != puk_string_banner.wstr_ptr)
	{
		SCI_TRACE_LOW("MMIPHONE_PukRemainTimesBanner PUK open get string success");
	}
	//banner提示
	banner_info.banner_type = MMI_BANNER_STR_TEXT;
    banner_info.text1_str = puk_string_banner;// puk_string_banner;//puk_string_ptr;//text需要加载资源//PUK码输入错误，请重试STR_INFO_IAP_INVALID_PIN
	banner_info.start_point = puk_alert_start_point;
	MMIAPICOM_OpenBannerWin(PHONE_PUK_INPUT_WIN_ID, banner_info);
	if((0 != puk_remain_times) && (1 != puk_remain_times))
	{
		SCI_FREE(puk_string_banner.wstr_ptr);
	}
}

/*****************************************************************************/
// 	Description : create input pin window
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E CreateInputPinWindow(APP_MN_SIM_GET_PIN_IND_T sim_get_pin_ind)
{    
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T alert_win = PHONE_PIN_ALERT_WIN_ID;
    MMICOM_BANNER_INFO_T banner_info = {0};
    GUI_POINT_T pin_alert_start_point = {0, 0};

    s_sim_pin_information.pin_num= sim_get_pin_ind.pin_num;
    s_sim_pin_information.pin_is_blocked= sim_get_pin_ind.pin_is_blocked;
    s_sim_pin_information.remain_times= sim_get_pin_ind.remain_times;
    s_sim_pin_information.dual_sys= sim_get_pin_ind.dual_sys;
    
    //SCI_TRACE_LOW:"mmiphone:CreateInputPinWindow dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6576_112_2_18_2_45_13_127,(uint8*)"d", sim_get_pin_ind.dual_sys);
    
    if (MNSIM_PIN1 == sim_get_pin_ind.pin_num)
    {
        MMK_CloseWin(alert_win);
        
        if (sim_get_pin_ind.pin_is_blocked)//PIN被锁，要求输入PUK
        {
            if (MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
            {
                //提示错误窗口,PUK错误!
                SCI_TRACE_LOW("[PIN] CreateInputPinWindow pin is blocked and PUK open");
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PUK_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlocked);
                MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);

                //清空password
                 GUIEDIT_ClearAllStr(PHONE_PUK_EDITBOX_CTRL_ID);
            }
            else
            {
                //提示错误窗口,PIN被锁!
                SCI_TRACE_LOW("[PIN] CreateInputPinWindow pin is blocked and PUK not open");
                MMIPUB_OpenAlertWinByTextId(PNULL,STR_PIN_BLOCKED_EXT01,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlocked);
            }
        }
        else//PIN没有被锁，要求输入PIN
        {            
            if (MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID))
            {
                //提示错误窗口,PIN 错误!
                SCI_TRACE_LOW("[PIN] CreateInputPinWindow PIN open");
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIN_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win,PNULL,MMIPUB_SOFTKEY_ONE,HandlePin1Maxtimes);

                //清空password
                GUIEDIT_ClearAllStr(PHONE_PIN_EDITBOX_CTRL_ID);
            }
            else if(MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
            {
                MMIAPIPHONE_SetPukRemainStatus(2);
                MMIAPIPHONE_SetSim2PinQueryFlay(TRUE);
            }
            else
            {
                // 打开输入PIN的界面
				MMK_CreatePubEditWin((uint32*) PHONE_PIN_INPUTING_WIN_TAB, PNULL);
            }
        }
    }
    
    // 关闭等待窗口
    MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
    
    return (result);
}

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MMI_MULTI_SIM_SYS_DUAL)
/*****************************************************************************/
// Description : get dual sim status
// Global resource dependence : 
// Author: bruce.chi
// Note: This API could only be used in SINGLE-SIM or DUAL SIM project,
// If in trisim or quad sim project,Please use another api(eg:MMIAPIPHONE_GetSimOkNum...) or other method.
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_GetDualSimStatus(BOOLEAN *is_sim1_ok_ptr, BOOLEAN *is_sim2_ok_ptr)
{
    if(PNULL == is_sim1_ok_ptr  ||PNULL == is_sim2_ok_ptr)
    {
        //SCI_TRACE_LOW:"GetSimAndServiceString: the is_sim1_ok_ptr=%d || is_sim2_ok_ptr =%d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6636_112_2_18_2_45_13_128,(uint8*)"dd", is_sim1_ok_ptr, is_sim2_ok_ptr);
        return FALSE;
    }
    
    if (MMIAPISET_GetFlyMode())
    {
        *is_sim1_ok_ptr = FALSE;
        *is_sim2_ok_ptr = FALSE;
    }
    else
    {
        *is_sim1_ok_ptr = MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1);
        
#ifdef MMI_MULTI_SIM_SYS_DUAL
        *is_sim2_ok_ptr = MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_2);
#else
        *is_sim2_ok_ptr = MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1);
#endif
    }
    
    //SCI_TRACE_LOW:"MMIAPIPHONE_GetDualSimStatus, is_sim1_ok=%d, is_sim2_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6656_112_2_18_2_45_13_129,(uint8*)"dd", *is_sim1_ok_ptr, *is_sim2_ok_ptr);
    return TRUE;
}
#endif

/*****************************************************************************/
// 	Description : IsNeedSetGPRSAttachMode
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedSetGPRSAttachMode(void)
{
    uint32 i = 0;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (!s_is_sim_present_ind[i])//(!s_is_power_on_sim_cnf[i] && !s_handle_pin_status.is_handling_pin[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSAttachMode
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSAttachMode(void)
{
#ifndef WIN32
    uint32 i =0;
#ifdef MMI_DUALMODE_ENABLE
//    static BOOLEAN is_need_set = TRUE;//Set only when Power ON
#endif

    if (!IsNeedSetGPRSAttachMode())
    {
       // return;
    }

#ifdef MMI_DUALMODE_ENABLE
    //SCI_TRACE_LOW:"MMIAPIENG_GetFtaFlag() = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6695_112_2_18_2_45_13_130,(uint8*)"d", MMIAPIENG_GetFtaFlag());
    //set gprs manual attach mode
/*    
    if(!is_need_set)
    {
        SCI_TRACE_LOW("MMIAPIPHONE_SetGPRSAttachMode attach has set");
        return;
    }
    is_need_set = FALSE;
*/    
#ifdef ENGTD_SUPPORT  
    if (MMIAPIENG_GetFtaFlag() && !MNPHONE_GetGprsAttachMode())
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MNPHONE_SetGprsAttachModeEx(i, TRUE);
        }
        }
    else
#endif
    {
        //多卡存在或者需要时连接模式
        if (MMIAPISET_GetMultiSysSettingNum(PNULL, 0) > 1)
        {
            SCI_TRACE_LOW("MMIAPIPHONE_SetGPRSAttachMode MMIAPISET_GetMultiSysSettingNum #10000");
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
            if (MNPHONE_GetGprsAttachMode_Ex(i))    //if it is auto attach, disable it
                {
                MNPHONE_SetGprsAttachModeEx(i, TRUE);
                MNGPRS_SetEnergySavingMode(i, FALSE);
                }
            }
        }
#ifdef MMI_GPRS_SWITCH_SUPPORT  //if switched off, disable auto attach
         else if(MMIAPICONNECTION_isGPRSSwitchedOff())
        {
            SCI_TRACE_LOW("MMIAPIPHONE_SetGPRSAttachMode MMIAPICONNECTION_isGPRSSwitchedOff #20000");

            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (MNPHONE_GetGprsAttachMode_Ex(i))    //if it is auto attach, disable it            
                    {
                    MNPHONE_SetGprsAttachModeEx(i, TRUE);
                    MNGPRS_SetEnergySavingMode(i, FALSE);
                    }
            }
        }
#endif
        else if (!MMIAPICONNECTION_isGPRSNeedAutoAttach())   //turn off in case of  MMICONNECTION_GPRS_ATTACH_ONNEED
        {
            SCI_TRACE_LOW("MMIAPIPHONE_SetGPRSAttachMode MMIAPICONNECTION_isGPRSNeedAutoAttach #30000");

            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (MNPHONE_GetGprsAttachMode_Ex(i))    //if it is auto attach, disable it            
                    {
                    MNPHONE_SetGprsAttachModeEx(i, TRUE);
                    MNGPRS_SetEnergySavingMode(i, FALSE);
                    }
            }            
        }
        else    //Turn on auto attach
        {
            MMICONNECTION_GPRS_SETTINGS_E grps_status = MMIAPICONNECTION_GetGPRSStatus();

            SCI_TRACE_LOW("MMIAPIPHONE_SetGPRSAttachMode Turn on#40000");
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                MNPHONE_SetGprsAttachModeEx(i, FALSE);

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
    }
#else
    //set gprs manual attach mode
#ifdef MMI_GPRS_SUPPORT
    if (MMIAPISET_GetMultiSysSettingNum(PNULL, 0) > 1
        || (!MMIAPICONNECTION_isGPRSNeedAutoAttach()))
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MNPHONE_SetGprsAttachModeEx(i, TRUE);
        }
    }
    else
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MNPHONE_SetGprsAttachModeEx(i, FALSE);
        }
    }
#else
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        MNPHONE_SetGprsAttachModeEx(i, TRUE);
    }
#endif
#endif
#endif
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSAttach
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSAttach(void)
{
    uint16 sys_ok = 0;
    BOOLEAN bIsFound = FALSE;

    if (MMIAPISET_GetMultiSysSettingNum(&sys_ok, 1) > 1)
    {
        bIsFound = MMIAPIPHONE_GetDataServiceSIM(&sys_ok);

        SCI_TRACE_LOW("MMIAPIPHONE_SetGPRSAttach  MMIAPIPHONE_GetDataServiceSIM = %d  , bIsFound %d" , sys_ok , bIsFound);

        if(bIsFound &&
            NO_DATA_SERVICE_TYPE != MMIAPIPHONE_GetDataServiceType(sys_ok) &&
            (FALSE == MMIAPICONNECTION_isGPRSSwitchedOff())
        )
        {
            SCI_TRACE_LOW("MMIAPIPHONE_SetGPRSAttach MNPHONE_GprsAttachEx !!!");
            MNPHONE_GprsAttachEx((MN_DUAL_SYS_E)sys_ok, MN_PHONE_GPRS_ATTACH);
        }	
    }
    else
    {
        if(MMIAPICONNECTION_isGPRSSwitchedOff())
        {
            return;
        }
        MNPHONE_GprsAttachEx((MN_DUAL_SYS_E)sys_ok, MN_PHONE_GPRS_ATTACH);
    }
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSDetach
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSDetach(void)
{
#ifndef WIN32
#ifdef MMI_DUALMODE_ENABLE
#ifdef ENGTD_SUPPORT 
    if (!MMIAPIENG_GetFtaFlag())
#endif
#endif
    {
#if defined(MMI_MULTI_SIM_SYS_SINGLE)
        //cant invoke GprsDetach In 8910 project
        #if 0
             MNPHONE_GprsDetachEx(MN_DUAL_SYS_1, MN_PHONE_GPRS_DETACH);
        #else
             SCI_TRACE_LOW("lzk log.MMIAPIPHONE_SetGPRSDetach() Do Nothing.");
        #endif
#else
        uint32 i =0;
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
        
        //SCI_TRACE_LOW:"MMIAPIPHONE_SetGPRSDetach e_dualsys_setting = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6802_112_2_18_2_45_14_131,(uint8*)"d", e_dualsys_setting);

        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (e_dualsys_setting & (0x01 << i))
            {
                MMIAPIPHONE_SetGPRSDetachEx(i);
            }
        }
#endif
    }
#endif
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSDetachEx
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSDetachEx(MN_DUAL_SYS_E dual_sys)
{
#ifndef WIN32    
    //SCI_TRACE_LOW:"MMIAPIPHONE_SetGPRSDetachEx dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6823_112_2_18_2_45_14_132,(uint8*)"d", dual_sys);

#ifdef MMI_DUALMODE_ENABLE
#ifdef ENGTD_SUPPORT 
    if (!MMIAPIENG_GetFtaFlag())
#endif
#endif
    {
    #if 0
        MNPHONE_GprsDetachEx(dual_sys, MN_PHONE_GPRS_DETACH);
    #else
        SCI_TRACE_LOW("lzk log. MMIAPIPHONE_SetGPRSDetachEx Do nothing.");		
    #endif
    }
#endif
}

/*****************************************************************************/
// 	Description : start up ps according to setting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
#ifdef PRODUCT_DM

#ifndef WIN32
extern void MNPHONE_SetMultiCardEx(uint8 ap_card_no);
#endif

#endif

#if defined(MMI_MULTI_SIM_SYS_DUAL)
static  uint8	flymode_forward_icon[MMI_DUAL_SYS_MAX]={0xff,0xff};
#else
static  uint8	flymode_forward_icon[MMI_DUAL_SYS_MAX]={0xff};
#endif
LOCAL void               flymode_update_forward_icon(void)
{
       int i;

        for( i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
               if(flymode_forward_icon[i]==0xff)
                {
                      flymode_forward_icon[i]=0;
                      MMIAPISET_SetIsQueryCfu(FALSE,i);
                }
               else
                {
                      SCI_TraceLow("fly mode:  show icon[%d]=%d",i,flymode_forward_icon[i]);
                      MMIAPICOM_StatusAreaSetForwardState(i,flymode_forward_icon[i]);	
                }
        }     
        
}
LOCAL void         flymode_save_forward_icon(void)
{
    int i;

     for( i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
            if(MMIAPISET_GetCfuInfo(i))
             {
                 flymode_forward_icon[i]=MMIIDLE_GetIdleWinInfo()->is_forward[i] ;
                 SCI_TraceLow("fly mode: save fly  icon[%d]=%d",i,flymode_forward_icon[i]);
             }
     }
        
}
BOOLEAN MMIAPIPHONE_StartUpPsAccordingToSetting(BOOLEAN is_power_on, BOOLEAN is_init_sim)
{
    uint32 i =0;
    if (is_power_on)
    {
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
#endif
#ifdef ENG_SECONDEDAC_SUPPORT
        MMIAPIENG_SECONDEDAC_InitforPowerOn();
#endif
#ifdef MMI_SIM_LOCK_SUPPORT
    	/*l4初始化需要一些sim_lock的信息，在此初始化这部分信息
		以免在特别的启动情况下有问题，比如release版本assert启动*/
        if(!MNPHONE_SIMLockDataIntegrityCheck())
        {         
            /*sim lock data error,the ME alert "sim Locked" then power off*/
            MMIPHONE_SIMLockedOpenShutOffWin();
            return FALSE;
        }
        else
        {
            MN_SIM_LOCK_STATUS_T* sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
            
            /*初始化SIM Lock Data*/
            SCI_MEMSET(sim_unlock_data,0x00,sizeof(MN_SIM_LOCK_STATUS_T));
            MMIAPIPHONE_UpdateSIMUnlockData();
        }
#endif        

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        s_query_sim_status = SIM_QUERY_STATUS_START;//开始查询SIM状态
#endif
        s_is_power_flag = TRUE;
        
        //SCI_TRACE_LOW:"MMIAPIPHONE_StartUpPsAccordingToSetting, start power on sim"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6854_112_2_18_2_45_14_133,(uint8*)"");

#ifdef PRODUCT_DM
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
#ifndef WIN32
        MNPHONE_SetMultiCardEx(0);  //set dual sim mode
#endif   
#endif
#endif
        
#if 0//def UNISOC_MMI_SIM_LOCK_SUPPORT
//根据要求，先启动卡1，在收到卡1 ready或者not ready时再启动卡2
        SCI_TRACE_LOW("[UNISOC_SIM_LOCK] MMIAPIPHONE_StartUpPsAccordingToSetting power on sim 1");
        MMIPHONE_SetSimStatus(MN_DUAL_SYS_1, SIM_NOT_INITED);

        s_is_power_on_sim_cnf[MN_DUAL_SYS_1] = FALSE;
        MNPHONE_PowerOnSimEx(MN_DUAL_SYS_1, is_init_sim);
#else
        //设置默认状态
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MMIPHONE_SetSimStatus(i, SIM_NOT_INITED);

            s_is_power_on_sim_cnf[i] = FALSE;

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
            if (is_init_sim)
            {
                if (e_dualsys_setting & (0x01 << i))
                {
                    MNPHONE_PowerOnSimEx(i, TRUE);
                }
                else
                {
                    MNPHONE_PowerOnSimEx(i, FALSE);
                }
            }
            else
            {
                MNPHONE_PowerOnSimEx(i, FALSE);
            }
#else
			//需要改变sim 的启动时序，先power sim1, 再power sim2 !
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
            if(!s_is_simlock_off)
            {
                if(MN_DUAL_SYS_1 == i )
                {
                    if(!s_is_sim1_poweron)
                    {
                        MNPHONE_PowerOnSimEx(MN_DUAL_SYS_1,is_init_sim);
                    }
                }
            }
            else
            {
                MNPHONE_PowerOnSimEx(i, is_init_sim);
            }
#else
            MNPHONE_PowerOnSimEx(i, is_init_sim);
#endif
#endif
        }
#endif
    }
    else
    {
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
        
        //SCI_TRACE_LOW:"MMIAPIPHONE_StartUpPsAccordingToSetting, e_dualsys_setting=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6888_112_2_18_2_45_14_134,(uint8*)"d", e_dualsys_setting);

        flymode_update_forward_icon();
        
        if (MMIAPISET_GetIsAutoUpdateTime())
        {
            MMIAPIAUT_SetIsNeedUpdateTimeByNet(TRUE); 
            MMIAPIAUT_SetIsFirstQuerySNTPFlag(TRUE);
        }
        
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            s_is_network_status_ready[i] = FALSE;

            if ((e_dualsys_setting & (0x01 << i))
                && MMIAPIPHONE_GetSimExistedStatus(i))
            {
 #ifdef SIM_LOCK_CT_SUPPORT
                    if(g_smartcard_status_t[i].is_cdma)
#endif
			{
#ifdef ENGTD_SUPPORT

                MMIAPIPHONE_PowerOnPs(i);
#else
                MMIAPIPHONE_PowerOnPs(i);
#endif
			}
            }
        }

        if (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))//无待机卡
        {
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
            {
                if (e_dualsys_setting & (0x01 << i))
                {
                    MMIAPIPHONE_PowerOnPs((MN_DUAL_SYS_E)i);
                    break;
                }
            }
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : deactive ps according to setting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_DeactivePsAccordingToSetting(void)
{
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;

    flymode_save_forward_icon();
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        MMIAPICOM_StatusAreaSetRoamingState(i, FALSE);
        if(!MMIAPISET_IsCfuStatusSavedInSim(i))    
        {
            MMIAPICOM_StatusAreaSetForwardState(i, FALSE);
        }
        #ifdef MMI_VOLTE_SUPPORT	
        MMIAPICOM_StatusAreaSetIdleVolteState(i);
        #endif	        
        MMIAPICOM_StatusAreaSetGprsState(i, FALSE);

    }

    //set gprs manual attach mode and start up ps 
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (!MMIAPIPHONE_GetSimExistedStatus(i) && 0 != MMIAPISET_GetMultiSysSettingNum(PNULL, 0))
        {
            continue;
        }
        if (e_dualsys_setting & (0x01 << i))
        {
            MMIAPIPHONE_PowerOffPs(i);
        }
        else
        {
            MMIAPIPHONE_SetIsPsDeactiveCnf(i, TRUE);
        }
        
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : PowerOnPsForEmerge
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
LOCAL void PowerOnPsForEmerge(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN  is_fly_mode_on = FALSE;

    if (MMIPHONE_PS_POWER_ON_NORMAL == s_ps_ready[dual_sys]
        ||MMIPHONE_PS_POWER_ON_EMERGE == s_ps_ready[dual_sys] )
    {
        //SCI_TRACE_LOW:"PowerOnPsForEmerge, s_ps_ready[%d]=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6968_112_2_18_2_45_14_135,(uint8*)"dd", dual_sys, s_ps_ready[dual_sys]);

        return;
    }

    is_fly_mode_on = MMIAPISET_GetFlyMode();

    if (is_fly_mode_on)
    {
        //SCI_TRACE_LOW:"PowerOnPsForEmerge Fly mode!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6977_112_2_18_2_45_14_136,(uint8*)"");

        return;
    }

#ifdef MMI_ONLY_4G_DEL_2G_CUSTOM
    // 开启VoLTE
    MMIAPISET_SetVolteInfo(dual_sys, TRUE);
    // 设置4G only
    MNPHONE_ChangeRat(RAT_ONLY_4G, dual_sys);
    Set2G4GMode(dual_sys, RAT_ONLY_4G);
#endif
    
    s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_EMERGE;
    s_is_network_status_ready[dual_sys] = FALSE;

    MMIAPIPHONE_SetIsPsReady(dual_sys, FALSE);
    
    MNPHONE_PowerOnPsEx(dual_sys);
}

#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
/*****************************************************************************/
// 	Description : MMIAPIPHONE_PowerOnPs
//	Global resource dependence : 
//  Author: liezhen.sha
//	Note:
/*****************************************************************************/
void MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_E dual_sys)
{
	SCI_TRACE_LOW("MMIAPIPHONE_PowerOnPs, dual_sys=%d", dual_sys);
	
	if (!s_is_simlock_off && !s_is_in_pwd_win)
	{
		BOOLEAN  is_need_poweron_ps = FALSE;
		SCI_TRACE_LOW("MMIAPIPHONE_PowerOnPs,sim1_right=%d,sim2_right=%d",s_is_sim1_right,s_is_sim2_right);
#ifdef MMI_MULTI_SIM_SYS_DUAL 
#ifdef DUAL_SIMLOCK_A//A
		if(s_is_sim1_right || s_is_sim2_right) 
		{
			is_need_poweron_ps = TRUE;
		}
#elif defined(DUAL_SIMLOCK_B)//B
		if(s_is_sim1_right) 
		{
			is_need_poweron_ps = TRUE;
		}
		else if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1) && s_is_sim2_right)
		{
			is_need_poweron_ps = TRUE;
		}
#elif defined(DUAL_SIMLOCK_C)//C	
		if ((dual_sys == MN_DUAL_SYS_1 && s_is_sim1_right) ||
			(dual_sys == MN_DUAL_SYS_2 && s_is_sim2_right)) 
		{
			is_need_poweron_ps = TRUE;
		}
#elif defined(DUAL_SIMLOCK_D)//D
		if (s_is_sim1_right) 
		{
			is_need_poweron_ps = TRUE;
		}
#elif defined(DUAL_SIMLOCK_E)//E
		if ((dual_sys == MN_DUAL_SYS_1 && s_is_sim1_right) ||
			(dual_sys == MN_DUAL_SYS_2)) 
		{
			is_need_poweron_ps = TRUE;
		}
#endif
		if (!is_need_poweron_ps && 
			(MMIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1) || MMIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2)))
		{
			//在非SIM POWER ON的状态下,至少有一张卡是正常接受的SIM 卡，才可以做PS 的启动.
			//如果两张卡都不存在，需要启动PS,便于产线做校准工作。
			SCI_TRACE_LOW("MMIAPIPHONE_PowerOnPs,return");
			return;
		}
#elif defined(MMI_MULTI_SIM_SYS_SINGLE)
		if((!s_is_sim1_right)&&(MMIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)))
		{
			SCI_TRACE_LOW("MMIAPIPHONE_PowerOnPs,return");
			return;
		}
#endif
	}

	if (MMIPHONE_PS_POWER_ON_NORMAL == s_ps_ready[dual_sys])
	{
		SCI_TRACE_LOW("MMIAPIPHONE_PowerOnPs, dual_sys=%d ps is already", dual_sys);
		return;
	}

	if (MMIAPISET_GetFlyMode())
	{
		SCI_TRACE_LOW("MMIAPIPHONE_PowerOnPs Fly mode!");
		return;
	}
	
	s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_NORMAL;
	s_is_network_status_ready[dual_sys] = FALSE;

	MMIPHONE_SetIsPsReady(dual_sys, FALSE);
	
	MNPHONE_PowerOnPsEx(dual_sys);

	MMIAPIPHONE_SetGPRSAttachMode();
}
#else
/*****************************************************************************/
// 	Description : MMIAPIPHONE_PowerOnPs
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
void MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN  is_fly_mode_on = FALSE;

    if (MMIPHONE_PS_POWER_ON_NORMAL == s_ps_ready[dual_sys])
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOnPs, dual_sys=%d ps is already"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7000_112_2_18_2_45_14_137,(uint8*)"d", dual_sys);

        return;
    }

    is_fly_mode_on = MMIAPISET_GetFlyMode();

    if (is_fly_mode_on)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOnPs Fly mode!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7009_112_2_18_2_45_14_138,(uint8*)"");

        return;
    }
    
    s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_NORMAL;
    s_is_network_status_ready[dual_sys] = FALSE;

    MMIAPIPHONE_SetIsPsReady(dual_sys, FALSE);

    MNPHONE_PowerOnPsEx(dual_sys);

    MMIAPIPHONE_SetGPRSAttachMode();
}
#endif /* MMI_SIM_LOCK_TYPE2_SUPPORT */

/*****************************************************************************/
// 	Description : MMIAPIPHONE_PowerOffPs
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
void MMIAPIPHONE_PowerOffPs(MN_DUAL_SYS_E dual_sys)
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOffPs, dual_sys=%d,s_ps_ready=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7030_112_2_18_2_45_14_139,(uint8*)"dd", dual_sys, s_ps_ready[dual_sys]);

    if (MMIPHONE_PS_POWER_ON_NONE == s_ps_ready[dual_sys])//ps not ready
    {
        return;
    }

    s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_NONE;
    
    s_is_network_status_ready[dual_sys] = FALSE;

    MMIAPICOM_StatusAreaSetRoamingState(dual_sys, FALSE);
    if(!MMIAPISET_IsCfuStatusSavedInSim(dual_sys))    
    {
        MMIAPICOM_StatusAreaSetForwardState(dual_sys, FALSE);
    }
    MMIAPICOM_StatusAreaSetGprsState(dual_sys, FALSE);
    
    MMIAPIPHONE_SetIsPsDeactiveCnf(dual_sys, FALSE);
#if defined(MMI_SMSCB_SUPPORT)
    ClearCbEnable(dual_sys);
#endif
    MNPHONE_PowerOffPsEx(dual_sys);
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetIsPsDeactiveCnf
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetIsPsDeactiveCnf(MN_DUAL_SYS_E dual_sys, BOOLEAN is_ps_deactive_cnf)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_SetIsPsDeactiveCnf err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7060_112_2_18_2_45_14_140,(uint8*)"d", dual_sys);
        return;
    }
    
    s_is_ps_deactive_cnf[dual_sys] = is_ps_deactive_cnf;
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetIsPsDeactiveCnf
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsPsDeactiveCnf(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetIsPsDeactiveCnf err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7075_112_2_18_2_45_14_141,(uint8*)"d", dual_sys);
        return FALSE;
    }

    SCI_TRACE_LOW("MMIAPIPHONE_GetIsPsDeactiveCnf dual sys: %d, is_ps_ready: %d", dual_sys, s_is_ps_deactive_cnf[dual_sys]);

    return s_is_ps_deactive_cnf[dual_sys];
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetIsPsReady
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetIsPsReady(MN_DUAL_SYS_E dual_sys, BOOLEAN is_ps_ready)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_SetIsPsReady err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7090_112_2_18_2_45_14_142,(uint8*)"d", dual_sys);
        return;
    }

    SCI_TRACE_LOW("MMIAPIPHONE_SetIsPsReady dual sys: %d, is_ps_ready: %d", dual_sys, is_ps_ready);

    s_is_ps_ready[dual_sys] = is_ps_ready;
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetIsPsReady
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsPsReady(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetIsPsReady err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7105_112_2_18_2_45_14_143,(uint8*)"d", dual_sys);
        return FALSE;
    }
    
    return s_is_ps_ready[dual_sys];
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetIsPsReady
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
#ifdef MMI_VOLTE_SUPPORT
BOOLEAN ims_status[MMI_DUAL_SYS_MAX] = {FALSE};
LOCAL void MMIAPICOM_StatusAreaSetVolteState(MN_DUAL_SYS_E dual_sys,MMI_GMMREG_RAT_E  rat)
{
    if(rat != MMI_GMMREG_RAT_4G)
        ims_status[dual_sys] = FALSE;
    MMIAPICOM_StatusAreaSetIdleVolteState(dual_sys);
}

LOCAL MMI_RESULT_E HandleImsStatusInd(DPARAM param )
{
    APP_MN_IMS_STATE_IND_T ims_status_ind = *((APP_MN_IMS_STATE_IND_T *)param);
    MMI_RESULT_E    result = MMI_RESULT_TRUE;    
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMISET_DAULSYS_ONLY_SIM1_ON;
    uint16 sim_ok = MN_DUAL_SYS_1;
    BOOLEAN bIsFound = FALSE;
    
    SCI_TraceLow("HandleImsStatusInd sim=%d,state=%d",ims_status_ind.dual_sys,ims_status_ind.nState);
    
    if((MN_IMS_SERVICE_STATE_ACTIVE == ims_status_ind.nState) || 
       (MN_IMS_SERVICE_STATE_ROAMING == ims_status_ind.nState) ||
       (MN_IMS_SERVICE_STATE_REREGISTER_SUCCESS == ims_status_ind.nState))
    {
        ims_status[ims_status_ind.dual_sys] = TRUE;
    }
    else
    {
    	ims_status[ims_status_ind.dual_sys] = FALSE;
    }
 
    MMIAPICOM_StatusAreaSetIdleVolteState(ims_status_ind.dual_sys);
    return result;	
}
PUBLIC BOOLEAN MMIAPIPHONE_GetImsStatus(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX <= dual_sys)
        return FALSE;
    SCI_TraceLow("MMIAPIPHONE_GetImsStatus ims_status[%d]=%d",dual_sys,ims_status[dual_sys]);	
    return ims_status[dual_sys];
}
#endif

#ifdef MMI_2G_4G_SUPPORT
LOCAL MMI_RESULT_E HandleRatChangeCnf(DPARAM param )
{
    APP_MN_CHANGE_RAT_CNF_T ratChangecnf = *((APP_MN_CHANGE_RAT_CNF_T *)param);
    MMI_RESULT_E    result = MMI_RESULT_TRUE;    
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMISET_DAULSYS_ONLY_SIM1_ON;
    uint16 sim_ok = MN_DUAL_SYS_1;
   SCI_TraceLow("HandleRatChangeCnf sim=%d,state=%d",ratChangecnf.dual_sys,ratChangecnf.nResult);
   MMISET_SetSwitchRatState(FALSE);
   return result;
}
#endif
/*****************************************************************************/
// 	Description : MMIAPIPHONE_IsPinBlockedNew
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPinBlockedNew(void)
{
     SCI_TRACE_LOW("MMIAPIPHONE_IsPinBlockedNew s_sim_pin_information.pin_is_blocked=%d", s_sim_pin_information.pin_is_blocked);
     return s_sim_pin_information.pin_is_blocked;	 
}

/*****************************************************************************/
// 	Description : get current handling Pin Ind sim id(response)
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPHONE_GetCurResponsePinSimID(void)
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_GetCurResponsePinSimID:sim_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7118_112_2_18_2_45_14_144,(uint8*)"d", s_handle_pin_status.cur_handling_sim_id);
    
    return s_handle_pin_status.cur_handling_sim_id;
}
#ifdef UNISOC_MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : get current handling  sim id
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPHONE_GetSimlockCurSimID(void)
{
    SCI_TRACE_LOW("[UNISOC_SIM_LOCK]MMIAPIPHONE_GetSimlockCurSimID:sim_id=%d",g_sim_lock_info.cur_sim_id);
 
    
    return g_sim_lock_info.cur_sim_id;
}
#endif
/*****************************************************************************/
// 	Description : get current handling sim id(operate pin)
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPHONE_GetCurHandlingPinSimID(void)
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_GetCurHandlingPinSimID:sim_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7129_112_2_18_2_45_14_145,(uint8*)"d", s_cur_handling_sim_id);
    return s_cur_handling_sim_id;
}

/*****************************************************************************/
// 	Description : set current handling sim id(operate pin)
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetCurHandlingPinSimID(MN_DUAL_SYS_E  cur_sim_id)
{
    s_cur_handling_sim_id = cur_sim_id;
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note:
/*****************************************************************************/
void MMIAPIPHONE_InitTDOrGsm(void)
{
#if defined(MMI_DUALMODE_ENABLE) ||defined (MMI_2G_4G_SUPPORT)
#ifndef WIN32
    uint32 i = 0;
    MN_GMMREG_RAT_E configue_rat = MN_GMMREG_RAT_UNKNOWN; 

    //SCI_TRACE_LOW:"MMIAPIPHONE_InitTDOrGsm configue_rat=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7152_112_2_18_2_45_14_146,(uint8*)"d", configue_rat);

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        configue_rat = MNPHONE_GetCurrentRAT(i);
        switch(configue_rat)
        {
        case MN_GMMREG_RAT_GPRS:
            s_is_TD[i] = MMI_GMMREG_RAT_GPRS;
            break;

        case MN_GMMREG_RAT_TD:
            s_is_TD[i] = MMI_GMMREG_RAT_TD;
            break;

        case MN_GMMREG_RAT_4G:
            s_is_TD[i] = MMI_GMMREG_RAT_4G;
            break;			

        default:
            s_is_TD[i] = MMI_GMMREG_RAT_UNKNOWN;
            break;
        }
    }

    MMIAPICOM_StatusAreaUpdateSignalState();
#endif
#endif
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note:
/*****************************************************************************/
void MMIAPIPHONE_TDOrGsm(MN_GMMREG_RAT_E     network_status_ind_rat,MN_DUAL_SYS_E dual_sys)
{
#if defined(MMI_DUALMODE_ENABLE) ||defined (MMI_2G_4G_SUPPORT)
    switch(network_status_ind_rat) 
    {
    case MN_GMMREG_RAT_GPRS:
        s_is_TD[dual_sys] = MMI_GMMREG_RAT_GPRS;
        break;
        
    case MN_GMMREG_RAT_3G:
        s_is_TD[dual_sys] = MMI_GMMREG_RAT_3G;
        break;
		
    case MN_GMMREG_RAT_4G:
        s_is_TD[dual_sys] = MMI_GMMREG_RAT_4G;
        break;
		
    case MN_GMMREG_RAT_UNKNOWN:
        s_is_TD[dual_sys] = MMI_GMMREG_RAT_UNKNOWN;
        break;
        
    default:
        break;
    }
#endif
}
/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note:
/*****************************************************************************/
PUBLIC MMI_GMMREG_RAT_E MMIAPIPHONE_GetTDOrGsm(MN_DUAL_SYS_E dual_sys)
{
#if defined(MMI_DUALMODE_ENABLE) ||defined (MMI_2G_4G_SUPPORT)
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        return s_is_TD[dual_sys];
    }
    else
    {
        uint32 i = 0;

        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (MMI_GMMREG_RAT_3G == s_is_TD[i])
            {
                return MMI_GMMREG_RAT_3G;
            }
            if (MMI_GMMREG_RAT_4G == s_is_TD[i])
            {
                return MMI_GMMREG_RAT_4G;
            }		
        }

        return MMI_GMMREG_RAT_GPRS;
    }
#else
    return MMI_GMMREG_RAT_GPRS;
#endif
}

/*****************************************************************************/
// 	Description :get current service status
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetCurServiceStatus(
                                            MN_DUAL_SYS_E dual_sys,
                                            PHONE_SERVICE_STATUS_T *status_ptr
                                            )
{
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    dual_sys = MN_DUAL_SYS_1;
#endif

    SCI_MEMCPY(status_ptr,&g_service_status[dual_sys],sizeof(PHONE_SERVICE_STATUS_T));
}

/*****************************************************************************/
// 	Description :get current service status
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC PHONE_SERVICE_STATUS_T* MMIAPIPHONE_GetServiceStatus(void)
{
    return g_service_status;
}


/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note: modify by yongwei.he 20070621
/*****************************************************************************/
void MMIAPIPHONE_StopSTTDOrGsm(void)
{
//macro replace
//#ifdef MMI_STREAMING_ENABLE
#ifdef STREAMING_SUPPORT_NEXTREAMING
    MMI_WIN_ID_T  focus_win_id;
    focus_win_id = MMK_GetFocusWinId();
    
    if(MMIST_MAIN_PLAY_WIN_ID == focus_win_id)
    {
        MMIAPIST_Close();
    }
    for (focus_win_id = MMIST_MAIN_PLAY_WIN_ID;focus_win_id < MMIST_MAX_WIN_ID;focus_win_id++)
    {
        MMK_CloseWin(focus_win_id);    
    }
#endif
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPresentInd(
                                     DPARAM param
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_SIM_PRESENT_IND_T sim_present_ind = *(APP_MN_SIM_PRESENT_IND_T *)param;
#ifdef SIM_LOCK_CT_SUPPORT
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif

    if(sim_present_ind.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleSimPresentInd:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7263_112_2_18_2_45_14_147,(uint8*)"d", sim_present_ind.dual_sys);
        return MMI_RESULT_FALSE;
    }

    //SCI_TRACE_LOW:"HandleSimPresentInd:dual_sys:%d, sim_sms_num:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7267_112_2_18_2_45_14_148,(uint8*)"dd", sim_present_ind.dual_sys, sim_present_ind.sms_num);
#ifdef SIM_LOCK_CT_SUPPORT
    dual_sys = sim_present_ind.dual_sys;
    if(sim_present_ind.is_sim_present)
    {
#ifndef WIN32
        if ((RM_GetPinTestFlag())||(RM_GetCalibrationPostMode())||(RM_GetCalibrationMode())||(RM_GetCalibrationPostNoLcmMode()))
        {
            g_smartcard_status_t[dual_sys].need_check = 0;
            g_smartcard_status_t[dual_sys].check_over = 1;
            g_smartcard_status_t[dual_sys].sim_present = sim_present_ind.is_sim_present;
            g_smartcard_status_t[dual_sys].sms_num = sim_present_ind.sms_num;
            g_smartcard_status_t[dual_sys].is_cdma = TRUE;
        }
        else
#endif
        if(FALSE == MMICDMA_GetSmartCard_Over(dual_sys))
        {
            g_smartcard_status_t[dual_sys].need_check = 1;
            g_smartcard_status_t[dual_sys].sim_present = 1;
            g_smartcard_status_t[dual_sys].sms_num = sim_present_ind.sms_num;
            Trace_Log_Print("PS: HandleSimReadyInd Start Check");
            return result;
        }
    }
    else
    {
         if(FALSE == MMICDMA_GetSmartCard_Over(dual_sys))
         {
            g_smartcard_status_t[dual_sys].need_check = 0;
            g_smartcard_status_t[dual_sys].check_over = 1;
         }
    }
#endif

    s_is_sim_present_ind[sim_present_ind.dual_sys] = TRUE;

    MMIAPISMS_SetMaxSimSmsNum(sim_present_ind.dual_sys, sim_present_ind.sms_num);

    MMIAPIPHONE_SetSimExistedStatus(sim_present_ind.dual_sys, sim_present_ind.is_sim_present);

    MMIAPIPHONE_SetGPRSAttachMode();

    //stk refresh type = SIMAT_RF_RESET(4) need power on ps.
    if (4 == MMIAPISTK_GetStkRefreshType(sim_present_ind.dual_sys)) 
    {
        MMIAPISTK_SetStkRefreshType(sim_present_ind.dual_sys, 0);

        MMIAPIPHONE_PowerOnPs(sim_present_ind.dual_sys);

        MMIAPISTK_CloseRefreshWaitingWin();
    }

    return result;
}
#ifdef MMI_2G_4G_SUPPORT
PUBLIC MMIPHONE_RAT_E MMIAPIPHONE_GetRat(MN_DUAL_SYS_E dual_sys)
{
    MMIPHONE_RAT_E currat = 0 ;
    uint8 curmode = 0;
#ifndef WIN32
    curmode = MNPHONE_GetRat(dual_sys);
#endif
    if(curmode == 2)
		currat =MMI_RAT_TYPE_G;
    else
	currat = MMI_RAT_TYPE_LTE;
    return currat;
}
#endif
#ifdef MMI_LDN_SUPPORT

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLdnReadyInd(
                                     DPARAM param
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_LND_READY_IND_T ldn_ready_ind = *(APP_MN_LND_READY_IND_T *)param;

    //SCI_TRACE_LOW:"HandleLdnReadyInd:dual_sys:%d,is_lnd_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7292_112_2_18_2_45_15_149,(uint8*)"dd", ldn_ready_ind.dual_sys, ldn_ready_ind.is_lnd_ok);

    //Just remember dual_sys,and wait until when...do it
    if (ldn_ready_ind.dual_sys>=MMI_DUAL_SYS_MAX)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMIAPIIDLE_IdleWinIsOpen())
    {
        MMIAPICL_InitCallInfo(ldn_ready_ind.dual_sys);
    }
    else
    {
        s_ldn_flag |=0x01<<(ldn_ready_ind.dual_sys);
    }    

    return result;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIPHONE_CheckLdnReadyInd(void)
{
    MN_DUAL_SYS_E dual_sys=0;   

    while(s_ldn_flag)
    {
        if(s_ldn_flag&0x01)
        {
            MMIAPICL_InitCallInfo(dual_sys);
        }
        s_ldn_flag =s_ldn_flag>>1;        
        dual_sys++;
    }
    return MMI_RESULT_TRUE;
}
#endif

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPlugInInd(
                                     DPARAM param
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_SIM_PLUG_IN_IND_T sim_plug_in_ind = *(APP_MN_SIM_PLUG_IN_IND_T *)param;

    if(sim_plug_in_ind.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleSimPlugInInd:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7313_112_2_18_2_45_15_150,(uint8*)"d", sim_plug_in_ind.dual_sys);
        return MMI_RESULT_FALSE;
    }
    //SCI_TRACE_LOW:"HandleSimPlugInInd:dual_sys:%d,"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7316_112_2_18_2_45_15_151,(uint8*)"d", sim_plug_in_ind.dual_sys);
    
    MMIPHONE_SetSimStatus(sim_plug_in_ind.dual_sys, SIM_NOT_INITED);

    MMIAPIPHONE_SetSimExistedStatus(sim_plug_in_ind.dual_sys, TRUE);

    NotifySIMPlugInd(sim_plug_in_ind.dual_sys, MMIPHONE_NOTIFY_SIM_PLUG_IN);

    return result;
}

/*****************************************************************************/
// 	Description : Handle SIM Plug Alert Win Msg
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMPlugAlertWinMsg(
                                                                                              MMI_WIN_ID_T win_id,
                                                                                              MMI_MESSAGE_ID_E 	msg_id,
                                                                                              DPARAM param
                                                                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    LOCAL MN_DUAL_SYS_E plug_sim_id = MN_DUAL_SYS_1;
    LOCAL MMIPHONE_SIM_PLUG_EVENT_E plug_event = MMIPHONE_NOTIFY_SIM_PLUG_MAX;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        plug_sim_id = s_plug_sim_id;
        plug_event = s_plug_event;
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    case MSG_CLOSE_WINDOW:
        if (plug_sim_id == s_plug_sim_id && plug_event == s_plug_event)
        {
            MMIAPIPHONE_SetStandByMode(s_plug_sim_id, s_plug_event);
        }

        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : Notify SIM Plug Ind
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifySIMPlugInd(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event)
{
    MMIPHONE_SIM_PLUG_NOTIFY_LIST_T *cursor = PNULL;
    MMI_WIN_ID_T alert_win_id = PHONE_SIM_PLUG_ALERT_WIN_ID;
    MMI_WIN_PRIORITY_E win_priority = WIN_ONE_LEVEL;

    //SCI_TRACE_LOW:"NotifySIMPlugInd dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7381_112_2_18_2_45_15_152,(uint8*)"dd", dual_sys, notify_event);

    if (dual_sys >= MMI_DUAL_SYS_MAX || notify_event >= MMIPHONE_NOTIFY_SIM_PLUG_MAX)
    {
        return FALSE;
    }

    if (!s_is_power_flag)
    {
        //SCI_TRACE_LOW:"NotifySIMPlugInd Intial not start, donot handle!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7390_112_2_18_2_45_15_153,(uint8*)"");

        return FALSE;
    }

    MMIDEFAULT_TurnOnBackLight();

    s_plug_sim_id = dual_sys;
    s_plug_event = notify_event;

    MMK_CloseWin(alert_win_id);

    if (MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID)
        || MMK_IsOpenWin(PHONE_POWER_OFF_WIN_ID))
    {
        win_priority = WIN_ONE_LEVEL;
    }
    else
    {
        win_priority = WIN_THREE_LEVEL;
    }

    if (MMIPHONE_NOTIFY_SIM_PLUG_IN == notify_event)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SIM_PLUG_IN, TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, &win_priority, MMIPUB_SOFTKEY_ONE, HandleSIMPlugAlertWinMsg);
    }
    else if (MMIPHONE_NOTIFY_SIM_PLUG_OUT == notify_event)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SIM_PLUG_OUT, TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, &win_priority, MMIPUB_SOFTKEY_ONE, HandleSIMPlugAlertWinMsg);
    }

    cursor = s_mmiphone_sim_plug_subscriber_list_ptr;
    while(PNULL != cursor)
    {
        cursor->notify_info.notify_func(dual_sys, notify_event, NULL);
        cursor = cursor->next;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : register SIM Plug Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_RegisterSIMPlugNotifyInfo(MMIPHONE_SIM_PLUG_NOTIFY_INFO_T *notify_info_ptr)
{
    BOOLEAN return_val = FALSE;
    MMIPHONE_SIM_PLUG_NOTIFY_LIST_T *cursor = PNULL;

    if(PNULL != notify_info_ptr)
    {
        if(PNULL == s_mmiphone_sim_plug_subscriber_list_ptr)
        {
            s_mmiphone_sim_plug_subscriber_list_ptr = SCI_ALLOC_APP(sizeof(MMIPHONE_SIM_PLUG_NOTIFY_LIST_T));
            SCI_MEMSET(s_mmiphone_sim_plug_subscriber_list_ptr, 0, sizeof(MMIPHONE_SIM_PLUG_NOTIFY_LIST_T));
            SCI_MEMCPY(&(s_mmiphone_sim_plug_subscriber_list_ptr->notify_info), notify_info_ptr, sizeof(MMIPHONE_SIM_PLUG_NOTIFY_INFO_T));
        }
        else
        {
            cursor = s_mmiphone_sim_plug_subscriber_list_ptr;
            while(PNULL != cursor->next)
            {
                if(cursor->notify_info.module_id == notify_info_ptr->module_id)
                {
                    return FALSE;   //already register
                }
                cursor = cursor->next;
            }
            if(cursor->notify_info.module_id == notify_info_ptr->module_id)
            {
                return FALSE;   //already register
            }
            cursor->next = SCI_ALLOC_APP(sizeof(MMIPHONE_SIM_PLUG_NOTIFY_LIST_T));
            SCI_MEMSET(cursor->next, 0, sizeof(MMIPHONE_SIM_PLUG_NOTIFY_LIST_T));
            SCI_MEMCPY(&(cursor->next->notify_info), notify_info_ptr, sizeof(MMIPHONE_SIM_PLUG_NOTIFY_INFO_T));
        }

        return_val = TRUE;
    }

    return return_val;
}

/*****************************************************************************/
// 	Description : Unregister SIM Plug Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_UnregisterSIMPlugNotifyInfo(uint32 module_id)
{
    MMIPHONE_SIM_PLUG_NOTIFY_LIST_T *cursor = PNULL;
    MMIPHONE_SIM_PLUG_NOTIFY_LIST_T *cursor_free = PNULL;

    cursor = s_mmiphone_sim_plug_subscriber_list_ptr;

    if(PNULL != cursor)
    {
        if(cursor->notify_info.module_id == module_id)
        {
            s_mmiphone_sim_plug_subscriber_list_ptr = cursor->next;
            SCI_FREE(cursor);
        }
        else
        {
            while(PNULL != cursor->next)
            {
                if (cursor->next->notify_info.module_id == module_id)
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
#endif

/*****************************************************************************/
// 	Description : Power On SIM
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOnSIM(MN_DUAL_SYS_E dual_sys)
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOnSIM, start power on sim dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7515_112_2_18_2_45_15_154,(uint8*)"d", dual_sys);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    s_query_sim_status = SIM_QUERY_STATUS_START;//开始查询SIM状态
#endif
    s_is_power_on_sim_cnf[dual_sys] = FALSE;
    SCI_TRACE_LOW("MMIAPIPHONE_PowerOnSIM MMIPHONE_SetSimStatus");
    MMIPHONE_SetSimStatus(dual_sys, SIM_NOT_INITED);
    MNPHONE_PowerOnSimEx(dual_sys, TRUE);
}

/*****************************************************************************/
// 	Description : Deactive PS
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_DeactivePS(MN_DUAL_SYS_E dual_sys)
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_DeactivePS, dual_sys=%d,s_ps_ready=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7533_112_2_18_2_45_15_155,(uint8*)"dd", dual_sys, s_ps_ready);

    s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_NONE;
    s_is_network_status_ready[dual_sys] = FALSE;
    SCI_TRACE_LOW("MMIAPIPHONE_DeactivePS,MMIPHONE_SetSimStatus");
    MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_NO_SIM);
    
    MMIAPIPHONE_SetIsPsDeactiveCnf(dual_sys, FALSE);
#if defined(MMI_SMSCB_SUPPORT)
    ClearCbEnable(dual_sys);
#endif
    MNPHONE_DeactivePsEx(dual_sys);
}

/*****************************************************************************/
// 	Description : Power off  PS or SIM
//	Global resource dependence : none
//  Author: chunjie.liu
//	Note:if ps is ready,power off ps,else power off sim
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PoweroffPsOrSim(MN_DUAL_SYS_E dual_sys)
{
    SCI_TRACE_LOW("MMIAPIPHONE_PoweroffPSorSim, dual_sys=%d,s_ps_ready=%d",dual_sys, s_ps_ready);

    if(MMIPHONE_PS_POWER_ON_NONE != s_ps_ready[dual_sys]) //ps is ready
    {
        s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_NONE;
        s_is_network_status_ready[dual_sys] = FALSE; 
        MMIAPIPHONE_SetIsPsDeactiveCnf(dual_sys, FALSE);
#if defined(MMI_SMSCB_SUPPORT)
        ClearCbEnable(dual_sys);
#endif
        s_phone_is_power_off_req[dual_sys] = TRUE;
        MNPHONE_PowerOffPsEx(dual_sys);
    }
    else //ps is not ready
    {
        SCI_TRACE_LOW("MMIAPIPHONE_PoweroffPsOrSim MMIPHONE_SetSimStatus");
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_NO_SIM);
        MNPHONE_PowerOffSimEx(dual_sys);
    }       
}

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
// 	Description : HandleSetStandByModeQueryWinMsg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetStandByModeQueryWinMsg( 
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E    msg_id, 
                                           DPARAM              param
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMIAPISET_OpenMultiSimStandbySettingsWindow();

        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
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
//  Description : HandleSetStandbyMdoeWaitingWindow
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetStandbyMdoeWaitingWindow(
                                         MMI_WIN_ID_T    win_id, // ???ID
                                         MMI_MESSAGE_ID_E   msg_id, // ???????ID
                                         DPARAM             param   // ???????
                                         )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"HandleSetStandbyMdoeWaitingWindow msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7595_112_2_18_2_45_15_156,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:       
        break;

    case MSG_APP_RED:
        break;

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
// 	Description : 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void OpenSetStandByModeWaiting(void)
{
    MMI_WIN_PRIORITY_E win_priority = WIN_ONE_LEVEL;
    MMI_WIN_ID_T wait_win_id = PHONE_SET_STANDBY_MODE_WAITING_WIN_ID;
    MMI_STRING_T wait_text = {0};

    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);

    if (MMIAPICC_IsInState(CC_IN_CALL_STATE)
        || MMIAPISET_IsNetworkConnecting()
#ifdef BROWSER_SUPPORT
        || MMIAPIBROWSER_IsRunning()
#endif
        )
    {
        win_priority = WIN_LOWEST_LEVEL;
    }
    else
    {
        win_priority = WIN_ONE_LEVEL;
    }

    MMIPUB_OpenWaitWin(1, &wait_text, PNULL, PNULL, wait_win_id, IMAGE_NULL,
                                        ANIM_PUBWIN_WAIT, win_priority, MMIPUB_SOFTKEY_NONE, HandleSetStandbyMdoeWaitingWindow);

    MMK_SetWinDisplayStyleState(wait_win_id, WS_DISABLE_RETURN_WIN, TRUE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void ImplementStandByModeAsync(void)
{
    //SCI_TRACE_LOW:"ImplementStandByModeAsync s_plug_sim_id=%d, s_plug_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7662_112_2_18_2_45_15_157,(uint8*)"dd", s_plug_sim_id, s_plug_event);

    if (s_plug_sim_id >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    if (MMK_IsOpenWin(PHONE_SET_STANDBY_MODE_WAITING_WIN_ID))
    {
        switch (s_plug_event)
        {
        case MMIPHONE_NOTIFY_SIM_PLUG_IN:
            MMIAPIPHONE_PowerOnSIM(s_plug_sim_id);
            break;

        default:
            break;
        }
    }

    MMK_CloseWin(PHONE_SET_STANDBY_MODE_WAITING_WIN_ID);
}

/*****************************************************************************/
// 	Description : set standby mode
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetStandByMode(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E plug_event)
{
    uint32 i = 0;
    BOOLEAN is_need_standby[MMI_DUAL_SYS_MAX] = {0};//是否需要待机
    uint32 sys_standby_num = 0;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MMI_WIN_ID_T query_win_id = PHONE_SET_STANDBY_MODE_QUERY_WIN_ID;

    //SCI_TRACE_LOW:"MMIAPIPHONE_SetStandByMode dual_sys=%d, plug_event=%d, e_sys_setting=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7698_112_2_18_2_45_15_158,(uint8*)"ddd",dual_sys, plug_event, e_dualsys_setting);

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (e_dualsys_setting & (0x01 << i))
        {
            is_need_standby[i] = TRUE;
            sys_standby_num ++;
        }
    }

    MMIPUB_CloseQuerytWin(&query_win_id);

    MMK_CloseWin(PHONE_SET_STANDBY_MODE_WAITING_WIN_ID);

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        MMIAPIPHONE_SetIdleGprsState(i);
    }

    switch (plug_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:
        MMIAPISET_SetIsQueryCfu(FALSE,dual_sys);

#ifdef MMI_GPRS_SUPPORT
        if (!MMIAPIPDP_PsIsPdpLinkExist())
        {
            if ((!MMIAPICONNECTION_isGPRSNeedAutoAttach())
                 || (sys_standby_num > 1))
            {
                MMIAPIPHONE_SetGPRSDetach();
            }
        }
#endif

        if (e_dualsys_setting & (0x01 << dual_sys))//待机卡插入
        {
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
            {
                if ((e_dualsys_setting & (0x01 << i))
                    && !MMIAPIPHONE_GetSimExistedStatus(i)
                    && (dual_sys != i))//关闭不存在SIM卡的PS
                {
                    MMIAPIPHONE_PowerOffPs((MN_DUAL_SYS_E)i);
                }
            }
        }

        OpenSetStandByModeWaiting();

        MMIAPIPHONE_DeactivePS(dual_sys);
        break;

    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
#ifdef MMI_GPRS_SUPPORT
#ifndef WIN32
        MMIAPIPHONE_SetGPRSAttachMode();

        MMIAPIPDP_ResetPdp(dual_sys);

        if (!MMIAPIPDP_PsIsPdpLinkExist())
        {
            if (1 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))
            {
                if ((!MMIAPICONNECTION_isGPRSNeedAutoAttach()))
                {
                    MMIAPIPHONE_SetGPRSDetach();
                }
                else
                {
                    MMIAPIPHONE_SetGPRSAttach();
                }
            }
        }
#endif
#endif

	    if(!MMIAPISET_IsCfuStatusSavedInSim(dual_sys))    
	    {
            MMIAPICOM_StatusAreaSetForwardState(dual_sys, FALSE);
	    }
        MMIAPICOM_StatusAreaSetRoamingState(dual_sys, FALSE);

        if (MMIAPIPHONE_GetPSReadyNum(PNULL, 0) > 1)
        {
            MMIAPIPHONE_PowerOffPs(dual_sys);
#ifdef QQ_SUPPORT
            MMIAPIQQ_UpdateSimSelect(dual_sys);
#endif
        }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if ((1 == sys_standby_num) && is_need_standby[dual_sys])
        {
            MMIPUB_OpenQueryWinByTextId(TXT_SIM_PLUG_OUT_SET_STANDBY_MODE_QUERY, IMAGE_PUBWIN_QUERY, &query_win_id, HandleSetStandByModeQueryWinMsg);
        }
#endif
        break;

    default:
        break;
    }
}
#endif

/*****************************************************************************/
// 	Description : according to ps status, judge if wap & mms is available
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPsServiceAvailble(void)
{
#ifdef WIN32
    return TRUE;
#else
#ifdef MMI_DUALMODE_ENABLE  //
    return TRUE;
#else
    return (ATC_GetModemStatus() != ATC_MODEM_ONDIAL);
#endif
#endif
}

/*****************************************************************************/
// 	Description :get current data service type
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_DATA_SERVICE_TYPE_E MMIAPIPHONE_GetDataServiceType( MN_DUAL_SYS_E dual_sys )
{
    MMIPHONE_DATA_SERVICE_TYPE_E service_type = NO_DATA_SERVICE_TYPE;
    if (g_service_status[dual_sys].hsupa_support)
    {
        service_type = HSUPA_DATA_SERVICE_TYPE;
    }
    else if (g_service_status[dual_sys].hsdpa_support)
    {
        service_type = HSDPA_DATA_SERVICE_TYPE;
    }
    else if (g_service_status[dual_sys].edge_support)
    {
        service_type = EDGE_DATA_SERVICE_TYPE;
    }
    else if ((MMI_GMMREG_RAT_GPRS_AND_3G == g_service_status[dual_sys].rat)
            ||(MMI_GMMREG_RAT_TD == g_service_status[dual_sys].rat))
    {
        service_type = TD_DATA_SERVICE_TYPE;
    }
    else if(g_service_status[dual_sys].gprs_support)
    {
        service_type = GPRS_DATA_SERVICE_TYPE;
    }
    else
    {
        //service_type = NO_DATA_SERVICE_TYPE;
    }

    //SCI_TRACE_LOW:"MMIAPIPHONE_GetDataServiceType: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7851_112_2_18_2_45_16_159,(uint8*)"d", service_type);
    return service_type;
}

/*****************************************************************************/
//  Description : Notify Rx level Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void NotifyRxLevelChange(MN_DUAL_SYS_E dual_sys, uint8 rx_level)
{
    LOCAL uint8 s_notify_rx_level[MMI_DUAL_SYS_MAX] = {0};/*lint !e64*/

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"NotifyRxLevelChange error dual_sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7865_112_2_18_2_45_16_160,(uint8*)"d", dual_sys);
        return;
    }

    //SCI_TRACE_LOW:"NotifyRxLevelChange rx_level=%d, s_notify_rx_level=%d, PlmnStatus=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7870_112_2_18_2_45_16_161,(uint8*)"ddd",rx_level, s_notify_rx_level[dual_sys], MMIAPIPHONE_GetPlmnStatus(dual_sys));

    if (0 == rx_level && 0 != s_notify_rx_level[dual_sys])
    {
        if (PLMN_NORMAL_GSM_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys)
            || PLMN_NORMAL_GPRS_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys)
            || PLMN_NORMAL_GSM_GPRS_BOTH == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        {
            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_UNAVAIL);

            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_UNAVAIL);
        }
    }
    else if (0 != rx_level && 0 == s_notify_rx_level[dual_sys])
    {
        if (PLMN_NORMAL_GSM_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        {
            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);
        }
        else if (PLMN_NORMAL_GPRS_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        {
            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
        }
        else if(PLMN_NORMAL_GSM_GPRS_BOTH == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        {
            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);
            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
        }
    }

    s_notify_rx_level[dual_sys] = rx_level;
}

/*****************************************************************************/
//  Description : Notify PLMN Status Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void NotifyPlmnStatusChange(MN_DUAL_SYS_E dual_sys, MN_PHONE_PLMN_STATUS_E plmn_status)
{
    LOCAL MN_PHONE_PLMN_STATUS_E s_notify_plmn_status[MMI_DUAL_SYS_MAX] = {0};/*lint !e64*/

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    //SCI_TRACE_LOW:"NotifyPlmnStatusChange plmn_status=%d, s_notify_plmn_status=%d, rssiLevel=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7917_112_2_18_2_45_16_162,(uint8*)"ddd",plmn_status, s_notify_plmn_status[dual_sys], g_service_status[dual_sys].rssiLevel);

    if (0 < g_service_status[dual_sys].rssiLevel)
    {
        switch (plmn_status)
        {
        case PLMN_NORMAL_GSM_ONLY:
            switch (s_notify_plmn_status[dual_sys])
            {
            case PLMN_NORMAL_GSM_ONLY:
                //plmn status not change
                break;

            case PLMN_NORMAL_GPRS_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_UNAVAIL);

                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);
                break;

            case PLMN_NORMAL_GSM_GPRS_BOTH:
                //NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_UNAVAIL);
                break;

            default:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);
                break;
            }
            break;

        case PLMN_NORMAL_GPRS_ONLY:
            switch (s_notify_plmn_status[dual_sys])
            {
            case PLMN_NORMAL_GSM_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_UNAVAIL);

                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
                break;

            case PLMN_NORMAL_GPRS_ONLY:
                //plmn status not change
                break;

            case PLMN_NORMAL_GSM_GPRS_BOTH:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_UNAVAIL);
                break;

            default:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
                break;
            }
            break;

        case PLMN_NORMAL_GSM_GPRS_BOTH:
            switch (s_notify_plmn_status[dual_sys])
            {
            case PLMN_NORMAL_GSM_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
                break;

            case PLMN_NORMAL_GPRS_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);
                break;

            case PLMN_NORMAL_GSM_GPRS_BOTH:
                //plmn status not change
                break;

            default:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);

                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
                break;
            }
            break;

        default:
            switch (s_notify_plmn_status[dual_sys])
            {
            case PLMN_NORMAL_GSM_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_UNAVAIL);
                break;

            case PLMN_NORMAL_GPRS_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_UNAVAIL);
                break;

            case PLMN_NORMAL_GSM_GPRS_BOTH:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_UNAVAIL);

                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_UNAVAIL);
                break;

            default:
                //need not notify
                break;
            }
            break;
        }
    }

    s_notify_plmn_status[dual_sys] = plmn_status;
}

/*****************************************************************************/
//  Description : Notify Service Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifyServiceChange(MN_DUAL_SYS_E dual_sys, MMIPHONE_NOTIFY_EVENT_E notify_event)
{
    MMIPHONE_NOTIFY_LIST_T *cursor = PNULL;

    //SCI_TRACE_LOW:"NotifyServiceChange dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8028_112_2_18_2_45_16_163,(uint8*)"dd", dual_sys, notify_event);

    if (dual_sys >= MMI_DUAL_SYS_MAX || notify_event >= MMIPHONE_NOTIFY_EVENT_MAX)
    {
        return FALSE;
    }

    cursor = s_mmiphone_subscriber_list_ptr;
    while(PNULL != cursor)
    {
        cursor->notify_info.notify_func(dual_sys, notify_event, NULL);
        cursor = cursor->next;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : register PS/CS Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_RegisterNotifyInfo(MMIPHONE_NOTIFY_INFO_T *notify_info_ptr)
{
    BOOLEAN return_val = FALSE;
    MMIPHONE_NOTIFY_LIST_T *cursor = PNULL;

    if(PNULL != notify_info_ptr)
    {
        if(PNULL == s_mmiphone_subscriber_list_ptr)
        {
            s_mmiphone_subscriber_list_ptr = SCI_ALLOC_APP(sizeof(MMIPHONE_NOTIFY_LIST_T));
            SCI_MEMSET(s_mmiphone_subscriber_list_ptr, 0, sizeof(MMIPHONE_NOTIFY_LIST_T));
            SCI_MEMCPY(&(s_mmiphone_subscriber_list_ptr->notify_info), notify_info_ptr, sizeof(MMIPHONE_NOTIFY_INFO_T));
        }
        else
        {
            cursor = s_mmiphone_subscriber_list_ptr;
            while(PNULL != cursor->next)
            {
                if(cursor->notify_info.module_id == notify_info_ptr->module_id)
                {
                    return FALSE;   //already register
                }
                cursor = cursor->next;
            }
            if(cursor->notify_info.module_id == notify_info_ptr->module_id)
            {
                return FALSE;   //already register
            }
            cursor->next = SCI_ALLOC_APP(sizeof(MMIPHONE_NOTIFY_LIST_T));
            SCI_MEMSET(cursor->next, 0, sizeof(MMIPHONE_NOTIFY_LIST_T));
            SCI_MEMCPY(&(cursor->next->notify_info), notify_info_ptr, sizeof(MMIPHONE_NOTIFY_INFO_T));
        }

        return_val = TRUE;
    }

    return return_val;
}

/*****************************************************************************/
// 	Description : Unregister PS/CS Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_UnregisterNotifyInfo(uint32 module_id)
{
    MMIPHONE_NOTIFY_LIST_T *cursor = PNULL;
    MMIPHONE_NOTIFY_LIST_T *cursor_free = PNULL;

    cursor = s_mmiphone_subscriber_list_ptr;
    if(PNULL != cursor)
    {
        if(cursor->notify_info.module_id == module_id)
        {
            s_mmiphone_subscriber_list_ptr = cursor->next;
            SCI_FREE(cursor);
        }
        else
        {
            while(PNULL != cursor->next)
            {
                if (cursor->next->notify_info.module_id == module_id)
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

/*****************************************************************************/
// 	Description : Is CS Available
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsCSAvail(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_cs_avail = FALSE;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return is_cs_avail;
    }

    if (0 < g_service_status[dual_sys].rssiLevel
        && (PLMN_NORMAL_GSM_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys)
            || PLMN_NORMAL_GSM_GPRS_BOTH == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        && !g_service_status[dual_sys].is_slecting)
    {
        is_cs_avail = TRUE;
    }

    return is_cs_avail;
}

/*****************************************************************************/
// 	Description : Is PS Available
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPSAvail(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_ps_avail = FALSE;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsPSAvail error dual_sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8157_112_2_18_2_45_17_164,(uint8*)"d", dual_sys);
        return is_ps_avail;
    }

    if (0 < g_service_status[dual_sys].rssiLevel
        && (PLMN_NORMAL_GSM_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys)
            || PLMN_NORMAL_GPRS_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys)
            || PLMN_NORMAL_GSM_GPRS_BOTH == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        && !g_service_status[dual_sys].is_slecting)
    {
        is_ps_avail = TRUE;
    }

    return is_ps_avail;
}

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
/*****************************************************************************/
// 	Description : MMIAPIPHONE_PowerOnPsEx
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOnPsEx(void)
{
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (e_dualsys_setting & (0x01 << i))
        {
            if (MMIAPIIDLE_IdleWinIsOpen()
                && s_is_power_on_sim_cnf[i]
                && MMIAPIPHONE_GetSimExistedStatus(i))
            {
                MMIAPIPHONE_PowerOnPs((MN_DUAL_SYS_E)i);
            }
            else
            {
                //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOnPsEx s_is_power_on_sim_cnf[%d]=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8195_112_2_18_2_45_17_165,(uint8*)"dd", i, s_is_power_on_sim_cnf[i]);
            }
        }
    }
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetIsPsReadyBySys
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsPsReadyBySys(MN_DUAL_SYS_E sys_type)
{
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = 0;
    BOOLEAN is_ps_ready = FALSE;

    e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    if (e_dualsys_setting & (1 << sys_type))
    {
        is_ps_ready = (MMIPHONE_PS_POWER_ON_NONE != s_ps_ready[sys_type]);
    }

    return is_ps_ready;
}
#endif /* !defined(MMI_MULTI_SIM_SYS_SINGLE) */

#ifdef DPHONE_SUPPORT 
/*****************************************************************************/
// 	Description : Get Pin Lock Hook Handfree Status
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_GetPinLockHookHandfreeStatus(void)
{
    return g_pin_lock_hookhandfree;
}

/*****************************************************************************/
// 	Description : Set Pin Lock Hook Handfree Status
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
void MMIAPIPHONE_SetPinLockHookHandfreeStatus(BOOLEAN hook_locked)
{
    g_pin_lock_hookhandfree = hook_locked;
}
#endif

#if defined(MMI_UPDATE_TIME_SUPPORT)
/*****************************************************************************/
// 	Description : 当前SIM是否注册网络 成功
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsRegNetSuccess(MN_DUAL_SYS_E cur_sim_index)
{
    BOOLEAN    result = FALSE;
    
    if(cur_sim_index < MMI_DUAL_SYS_MAX)
    {
#ifndef WIN32
        if (s_is_network_status_ready[cur_sim_index])
#endif
        {
            result = TRUE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsRegNetSuccess err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8260_112_2_18_2_45_17_166,(uint8*)"d", cur_sim_index);
    }

    return result;
}

/*****************************************************************************/
// 	Description : 是否所有SIM卡搜网 成功
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsAllSIMSelectNetSucc(void)
{
    BOOLEAN    result = TRUE;
    uint32  i = 0;
    
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        //SIM not avalible is also "status ready"
#if 1 //ndef WIN32
        if (MMIAPIPHONE_IsSimAvailable(i)  && !s_is_network_status_ready[i])
#endif
        {
            result = FALSE;
            break;
        }
    }

    SCI_TRACE_LOW("[mmiphone]: IsAllSIMSelectNetSucc result = %d, i = %d", result, i);

    return result;
}
/*****************************************************************************/
// 	Description : 判断SIM是否Attach gprs 成功
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsAttachGprsSuccess(MN_DUAL_SYS_E cur_sim_index)
{
    BOOLEAN    result = FALSE;
    
    if(cur_sim_index >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsAttachGprsSuccess err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8276_112_2_18_2_45_17_167,(uint8*)"d", cur_sim_index);
        return FALSE;        
    }
    
    if ((PLMN_NORMAL_GPRS_ONLY == g_service_status[cur_sim_index].plmn_status)
        || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[cur_sim_index].plmn_status))
    {
        result = TRUE;
    }
    
    return result;
}
#endif /*MMI_UPDATE_TIME_SUPPORT*/
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
//+++ mdy by qing.yu@for cr238585
/*****************************************************************************/
// 	Description : get sim card and plmn service status string text id
//	Global resource dependence : 
//  Author: qing.yu
//	Note:  
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIPHONE_OpeartorTextID(MN_DUAL_SYS_E dual_sys)
{
    MMI_TEXT_ID_T	idle_str_id = TXT_NULL;
    SIM_STATUS_E sim_status = MMIAPIPHONE_GetSimStatus(dual_sys);

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_OpeartorTextID param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8303_112_2_18_2_45_17_168,(uint8*)"");
        return idle_str_id;
    }

    //if sim error then display sim string in sim_status_string
    if (sim_status < SIM_STATUS_MAX)
    {
        if (sim_status != SIM_STATUS_OK) //sim not ready
        {
            idle_str_id = s_sim_status_string[sim_status];

        }
        else
        {
            idle_str_id = GetNetWorkNameId(
                dual_sys,
                &g_service_status[dual_sys].plmn,
                g_service_status[dual_sys].plmn_status,
                g_service_status[dual_sys].is_slecting
                );
        }
    }
	return idle_str_id;
}

PUBLIC BOOLEAN MMIAPIPHONE_GetIsNetworkRoaming(void)
{
	MN_DUAL_SYS_E dual_sys = MMIAPISET_GetActiveSim();

    return g_service_status[dual_sys].isRoaming;
}
//--- mdy by qing.yu@for cr238585
#endif

/*****************************************************************************/
// 	Description : Get Update NetworkStatus Win
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPIPHONE_GetUpdateNetworkStatusWin(void)
{
    return s_need_update_network_status_win;
}

/*****************************************************************************/
// 	Description : is network selecting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(MMI_HANDLE_T win_handle)
{
    s_need_update_network_status_win = win_handle;    
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_UpdateNetworkStatusWin(void)
{
    if (MMK_IsFocusWin(s_need_update_network_status_win))
    {
        MMK_UpdateScreen();
        //MMK_PostMsg(s_need_update_network_status_win, MSG_FULL_PAINT, PNULL, 0);
    }
}

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : 在SIM Lock 解锁次数达到限制的情况下弹出"SIM Restricted"窗口
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLocked(
                                    MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                    DPARAM 		        param		// 相应消息的参数
                                    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO	lcd_dev_info = {0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch (msg_id)
    {
    case MSG_APP_CANCEL://pin被锁输入界面不允许返回
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMK_CloseWin(win_id);
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : create input sim lock window
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:  
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_CreateInputSimLockWindow(APP_MN_SIM_NOT_READY_IND_T sim_not_ready_ind)
{    
    MN_SIM_LOCK_STATUS_T *sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    MMI_OPERATE_SIM_LOCK_T   *opreate_sim_lock_ptr =SCI_ALLOCAZ(sizeof(MMI_OPERATE_SIM_LOCK_T));
    uint32 unlock_trials = 0;
    uint32 time_out_value = 0;
    int32 left_trials = 0;
    MMI_RESULT_E result =MMI_RESULT_TRUE;
    
    if(PNULL == opreate_sim_lock_ptr)
    {
        //SCI_TRACE_LOW:"mmiphone:CreateInputSimLockWindow opreate_sim_lock_ptr = NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8423_112_2_18_2_45_17_169,(uint8*)"");
        return result;
    }
    
    //SCI_TRACE_LOW:"mmiphone:CreateInputSimLockWindow dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8427_112_2_18_2_45_17_170,(uint8*)"d", sim_not_ready_ind.dual_sys);
    
    SCI_MEMSET(opreate_sim_lock_ptr,0x00,sizeof(MMI_OPERATE_SIM_LOCK_T));
    opreate_sim_lock_ptr->dual_sys = sim_not_ready_ind.dual_sys;
    opreate_sim_lock_ptr->lock_type = MMI_SIM_NETWORK_LOCK<<((uint32)sim_not_ready_ind.reason - (uint32)MNSIM_ERROR_SIM_NETWORT_LOCKED);
    opreate_sim_lock_ptr->is_enable = TRUE;
    
    /*remember SIM Lock type in case power off the phone*/
    SCI_MEMSET(&s_sim_lock_operate_value,0x00,sizeof(MMI_OPERATE_SIM_LOCK_T));
    s_sim_lock_operate_value.dual_sys = opreate_sim_lock_ptr->dual_sys;
    s_sim_lock_operate_value.lock_type = opreate_sim_lock_ptr->lock_type;
    s_sim_lock_operate_value.is_enable = opreate_sim_lock_ptr->is_enable;
    
    MMIAPIPHONE_GetSIMLockRemainTrialsTimer(opreate_sim_lock_ptr->lock_type,&unlock_trials,&time_out_value);
    left_trials = sim_unlock_data->max_num_trials - unlock_trials;
    
    SCI_TRACE_LOW("MMIPHONE: MMIPHONE_CreateInputSimLockWindow, time_out_value = %d, left_trials = %d", time_out_value,left_trials);
    /*open sim unlock window*/
    if( 0 != time_out_value && 0 < left_trials)
    {
        if(!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID))
        {
            MMK_CreateWin((uint32*)SIM_LOCK_COUNTDOWN_RUN_WIN_TAB,(ADD_DATA)opreate_sim_lock_ptr);
        }
        return MMI_RESULT_TRUE;//
    }
    else if(!MMIPHONE_GetIsSIMUnlocked() && 0 < left_trials)
    {
        if(!MMK_IsOpenWin( PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID))
        {
            MMK_CreateWin((uint32*)SIM_LOCK_PASSWORD_INPUTING_WIN_TAB,(ADD_DATA)opreate_sim_lock_ptr);
        }
        return MMI_RESULT_TRUE;//
    }      
    else if(0 >= left_trials)
    {
        MMI_WIN_ID_T alert_win_id = PHONE_SIM_LOCK_ALERT_WIN_ID;
        uint32 alert_time = 0;
        
        MMIPUB_OpenAlertWinByTextId(&alert_time,TXT_SIM_LOCKED,TXT_NULL,
                                                        IMAGE_PUBWIN_WARNING,&alert_win_id,
                                                        PNULL,MMIPUB_SOFTKEY_NONE,HandleSIMLocked);

        SCI_TRACE_LOW("MMIPHONE: alert_win_id = %d", alert_win_id);
      
        MMK_SetWinDisplayStyleState(alert_win_id, WS_DISABLE_RETURN_WIN, TRUE);
        SCI_FREE(opreate_sim_lock_ptr);
    }
    else
    {
        SCI_FREE(opreate_sim_lock_ptr);
    }    
    return (result);
}

/*****************************************************************************/
// 	Description : get handle sim lock status ptr for multi sim card
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_HANDLE_SIMLOCK_STATUS_T *MMIPHONE_GetHandleSIMLockStatus(void)
{
    return &s_handle_sim_lock_status;
}
/*****************************************************************************/
// 	Description : MMIPHONE_Get SIM lock Config Data Ptr
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MN_SIM_LOCK_STATUS_T *MMIAPIPHONE_GetSIMUnlockConfigDataPtr(void)
{
    return  &s_sim_unlock_config_data;
}

/*****************************************************************************/
// 	Description : MMIPHONE Update SIM Unlock Data if MMI update sim lock data to MN
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_UpdateSIMUnlockData(void)
{
    return MNPHONE_GetSIMLockStatus(&s_sim_unlock_config_data);
}
#endif

#ifdef MMI_SIM_LANGUAGE_SUPPORT
/*****************************************************************************/
// 	Description : 判断是哪张sim卡做优选语言。
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPHONE_GetPreferLangSim(void)
{
    return s_prefer_language_sim;
}
#endif

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
// 	Description : Append All Plmn List ...
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AppendPlmnList(
                                                                                MN_DUAL_SYS_E dual_sys,
                                                                                MMI_CTRL_ID_T ctrl_id,                                                                               
                                                                                uint16 index
                                                                                )
{   
    GUILIST_ITEM_DATA_T item_data = {0};
    MN_PLMN_T plmn = {0};
    MMI_STRING_T plmn_name = {0};
    
    wchar plmn_code[MAX_MCC_MNC_DIGIT_DISPLAY +1] = {0};        
    uint16 plmn_code_len = MAX_MCC_MNC_DIGIT_DISPLAY;
    
    //SCI_TRACE_LOW:"MMIAPIPHONE_AppendPlmnList,index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8534_112_2_18_2_45_18_171,(uint8*)"d",index);
    
    if(index < MAX_NETWORK_NUM -1 )
    {
        plmn.mcc = s_network_nametable[index + 1].mcc;
        plmn.mnc = s_network_nametable[index + 1].mnc;        
        if(s_network_nametable[index+1].mnc >= 100)
        {
            plmn.mnc_digit_num = 3; // mnc 的长度有可能为2 或3
        }
        else
        {
            plmn.mnc_digit_num = 2; // mnc 的长度有可能为2 或3
        }        

        //PLMN NAME Content.
        //get plmn name info    
        MMI_GetLabelTextByLang(s_network_nametable[index+1].name_index, &plmn_name);
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
        item_data.item_content[0].item_data.text_buffer.wstr_len = plmn_name.wstr_len;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = plmn_name.wstr_ptr;

        //PLMN CODE Content .        
        //get plmn code info
        MMIAPIPHONE_GenPLMNDisplay(MMIAPIPHONE_GetTDOrGsm(dual_sys), &plmn, plmn_code, &plmn_code_len, TRUE);
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;        
        item_data.item_content[1].item_data.text_buffer.wstr_len = plmn_code_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = plmn_code;

        //set softkey
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;

        GUILIST_SetItemData( ctrl_id, &item_data, index );    
        GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
    }
}

/*****************************************************************************/
// 	Description : Append prefer plmn List ...
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AppendPreferPlmnList(
                                                                                MN_DUAL_SYS_E dual_sys,
                                                                                MMI_CTRL_ID_T ctrl_id,
                                                                                MN_PLMN_T *prefer_network,
                                                                                uint16 index)
{    
    GUILIST_ITEM_DATA_T item_data = {0};
    MN_PLMN_T plmn = {0};
    MMI_STRING_T plmn_name = {0};        
    wchar plmn_name_arr[MAX_ONSHORTFORM_LEN+1]={0};
    wchar plmn_code[MAX_MCC_MNC_DIGIT_DISPLAY +1] = {0};
    uint16 plmn_code_len = MAX_MCC_MNC_DIGIT_DISPLAY;

    //SCI_TRACE_LOW:"MMIAPIPHONE_AppendPreferPlmnList,index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8589_112_2_18_2_45_18_172,(uint8*)"d",index);

    if((PNULL!=prefer_network) && (index < MMISET_PREFER_PLMN_LIST_MAX) )
    {
        plmn.mcc = prefer_network[index].mcc;
        plmn.mnc = prefer_network[index].mnc;
        plmn.mnc_digit_num = prefer_network[index].mnc_digit_num;

        //PLMN NAME Content.
        //get plmn name info
        plmn_name.wstr_ptr =(wchar *)plmn_name_arr;
		plmn_name.wstr_len = MAX_ONSHORTFORM_LEN;
        MMIAPIPHONE_GenPLMNDisplay(MMIAPIPHONE_GetTDOrGsm(dual_sys), &plmn, plmn_name.wstr_ptr, &plmn_name.wstr_len, FALSE);
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
        item_data.item_content[0].item_data.text_buffer.wstr_len = plmn_name.wstr_len;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = plmn_name.wstr_ptr;

        //PLMN CODE Content .        
        //get plmn code info
        MMIAPIPHONE_GenPLMNDisplay(MMIAPIPHONE_GetTDOrGsm(dual_sys), &plmn,plmn_code, &plmn_code_len, TRUE);
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
        item_data.item_content[1].item_data.text_buffer.wstr_len = plmn_code_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = plmn_code;
        
        //set softkey
        item_data.softkey_id[0] = STXT_OPTION;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;

        GUILIST_SetItemData( ctrl_id, &item_data, index );    
        GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
    }
}
#endif

/*****************************************************************************/
// 	Description : get Plmn List Num...
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIPHONE_GetPlmnListNum(void)
{
    //s_network_nametable[]数组的第一项是test项，非实际的plmn。
    return MAX_NETWORK_NUM - 1;
}

/*****************************************************************************/
// 	Description :MMIAPIPHONE_GetPlmnInfoByIndex
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetPlmnInfoByIndex(                                                                                
                                                                                MN_PLMN_T *prefer_network,
                                                                                uint32 index)
{
    if(PNULL == prefer_network )
    {
        //SCI_TRACE_LOW:"GetSimAndServiceString: the prefer_network =%d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8643_112_2_18_2_45_18_173,(uint8*)"d", prefer_network);
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"MMIAPIPHONE_GetPlmnInfoByIndex,index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8647_112_2_18_2_45_18_174,(uint8*)"d",index);

    if( index < MAX_NETWORK_NUM)
    {
        prefer_network->mcc = s_network_nametable[index].mcc;
        prefer_network->mnc = s_network_nametable[index].mnc;
        if( s_network_nametable[index].mnc >= 100)
        {
            prefer_network->mnc_digit_num = 3; /// set mnc length as 3...
        }
        else
        {
            prefer_network->mnc_digit_num = 2; /// set mnc length as 2...
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifdef MMI_LDN_SUPPORT
/*****************************************************************************/
// 	Description : ldn update confirm
//	Global resource dependence : none
//	Author:dave.ruan
//	Note:delete all/datlete both type is erase
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLDNUpdateCnf( DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_LND_UPDATE_CNF_T *ldn_update_cnf_ptr = (APP_MN_LND_UPDATE_CNF_T *)param;
    #if 0
    BOOLEAN  is_ldn_all_deleted = TRUE;
    uint32  i = 0;
    #endif
    
    if (!ldn_update_cnf_ptr)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"ccapp.c HandleLDNUpdateCnf,type= %d, result =%d, dual_sys= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8683_112_2_18_2_45_18_175,(uint8*)"ddd",ldn_update_cnf_ptr->operate_type,ldn_update_cnf_ptr->is_ok,ldn_update_cnf_ptr->dual_sys);
                                
    if(MN_LND_ERASE == ldn_update_cnf_ptr->operate_type)
    {
	    if(MMIAPICL_GetLdnIsDeleteAll() && ldn_update_cnf_ptr->is_ok)//liezhen如果底层返回非OK的状态，则直接提示用户失败，不再向底层发送请求了。
	    {
	        MMIAPICL_SetLdnIsDeleteAll(FALSE);
	        if(MMIAPICL_DeleteAllLdnInfo(MMICL_CALL_DIALED))
	        {
	            return result;
	        }
	    }
	       
	    MMK_CloseWin(MMICC_LDN_UPDATE_WAITING_WIN_ID);

	    if(ldn_update_cnf_ptr->is_ok)
	    {
	        //success
	        //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
	    }
	    else
	    {
	        //reject
	        MMIPUB_OpenAlertWarningWin(TXT_COMMON_DEL_FAIL);
	    }
	    //close progess waiting win
	    //CC_UpdateCurrentCallStatus(TRUE);
	}
    return result;
}
#endif

/*****************************************************************************/
// 	Description : 获取状态正常的SIM卡总数和具体卡的位置
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:   sim_idex_ptr :point to ok sim index,if null,do not set sim index
//              array_len :ok sim index len
/*****************************************************************************/
PUBLIC uint8 MMIAPIPHONE_GetAllSimStateOKCount(uint16 *sim_idex_ptr, uint16 array_len)
{
    uint8 active_sim_count = 0;
    int32 sim_index = MMI_DUAL_SYS_MAX;

    for (sim_index = MN_DUAL_SYS_1; sim_index < MMI_DUAL_SYS_MAX; sim_index++)
    {
        if (MMIAPIPHONE_IsSimOk(sim_index))
        {
            if ((sim_idex_ptr != NULL) && (active_sim_count < array_len))
            {
                sim_idex_ptr[active_sim_count] = sim_index;
            }

            active_sim_count++;
        }
    }
    //SCI_TRACE_LOW:"[mmiphone]: MMIAPIPHONE_GetAllSimStateOKCount, active_sim_count=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8760_112_2_18_2_45_18_176,(uint8*)"d",active_sim_count);
    return active_sim_count;
}

/*****************************************************************************/
// 	Description : 获取可以使用数据服务的SIM卡
//	Global resource dependence : 
//   Author: sam.hua
//   Return value:
//        FALSE -- No available SIM for data service
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetDataServiceSIM(MN_DUAL_SYS_E *dual_sys_ptr)
{
    BOOLEAN ret = FALSE;
    
    if(dual_sys_ptr ==PNULL)
    {
        SCI_TRACE_LOW("mmiphone MMIAPIPHONE_GetDataServiceSIM: dual_sys_ptr == PNULL");
        return FALSE;
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE        //Only use in Dual SIM mode
    do
    {
            uint32 sim_ok_num = 0;
            uint16 sim_ok = MN_DUAL_SYS_1;
            MMISET_MAJOR_SIM_SEL_TYPE_E major_sim_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;

            sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);/*lint !e64*/
            if(sim_ok_num ==0)
            {
                *dual_sys_ptr =MMI_DUAL_SYS_MAX;
                ret = FALSE;
                break;
            }
            else if (sim_ok_num == 1)
            {
                *dual_sys_ptr = (MN_DUAL_SYS_E)sim_ok;
                ret = TRUE;
                break;
            }
            
            major_sim_type = MMISET_GetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_WAP);
            if (major_sim_type < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
            {
                *dual_sys_ptr = MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1);
                ret =TRUE;
                SCI_TRACE_LOW("mmiphone MMIAPIPHONE_GetDataServiceSIM: MMISET_GetMajorSimSelType dual_sys= %d",*dual_sys_ptr);
                break;
            }
            else
            {
                SCI_TRACE_LOW("mmiphone MMIAPIPHONE_GetDataServiceSIM: Warning, MMISET_GetMajorSimSelType shouldn't be manual select");
                *dual_sys_ptr = (MN_DUAL_SYS_E)sim_ok;
                ret =TRUE;
                break;
            }
    }while(0);
    
    SCI_TRACE_LOW("mmiphone MMIAPIPHONE_GetDataServiceSIM: result = %d , dual_sys= %d",ret,*dual_sys_ptr);
    return ret;

 #else
    *dual_sys_ptr = MN_DUAL_SYS_1;
    if(MMIAPIPHONE_IsSimAvailable(*dual_sys_ptr))
    {
        ret =TRUE;
    }
    SCI_TRACE_LOW("mmiphone MMIAPIPHONE_GetDataServiceSIM: result = %d , dual_sys= %d",ret,*dual_sys_ptr);
    return  ret;
 #endif
}

/*****************************************************************************/
// 	Description : is ofm(Operational Feature Monitor) actived, if it is, then the ciphering off indicator should displaying 
//    when the call connection is not encrypted
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsOfmActived(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_ofm_actived = FALSE;

    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        SCI_TRACE_LOW("MMIAPIPHONE_IsOfmActived err dual sys: %d", dual_sys);
        return FALSE;
    }
    
    is_ofm_actived = s_sim_status[dual_sys].admin.ofm_actived;

    return is_ofm_actived;
}

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
// 	Description : Set the network name from csc
//	Global resource dependence : 
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_SetNetWorkNameByCSC(MMICUS_CSC_GENERALINFO_NETWORKINFO_T *csc_network_name_ptr,
                                                                                                            uint16 network_num)
{
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *temp_csc_network_ptr = PNULL;
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *cur_csc_network_ptr = PNULL;
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *temp_cur_csc_network_ptr = PNULL;

    if (PNULL == csc_network_name_ptr || 0 == network_num)
    {
        return FALSE;
    }

    if (PNULL != s_csc_network_name_ptr)
    {
        cur_csc_network_ptr = s_csc_network_name_ptr->csc_network_info;

        while (PNULL != cur_csc_network_ptr)
        {
            temp_csc_network_ptr = cur_csc_network_ptr;
            cur_csc_network_ptr = cur_csc_network_ptr->next;

            SCI_FREE(temp_csc_network_ptr);
            temp_csc_network_ptr = PNULL;
        }

        SCI_FREE(s_csc_network_name_ptr);
        s_csc_network_name_ptr = PNULL;
    }

    s_csc_network_name_ptr = (MMI_CSC_NETWORK_NAME_T *)SCI_ALLOCAZ(sizeof(MMI_CSC_NETWORK_NAME_T));

    if (PNULL == s_csc_network_name_ptr)
    {
        return FALSE;
    }

    s_csc_network_name_ptr->net_work_num = network_num;

    temp_cur_csc_network_ptr = csc_network_name_ptr;

    while (PNULL != temp_cur_csc_network_ptr)
    {
        cur_csc_network_ptr = (MMICUS_CSC_GENERALINFO_NETWORKINFO_T *)SCI_ALLOCAZ(sizeof(MMICUS_CSC_GENERALINFO_NETWORKINFO_T));

        if (PNULL == cur_csc_network_ptr)
        {
            return FALSE;
        }
        SCI_MEMCPY(cur_csc_network_ptr, temp_cur_csc_network_ptr, sizeof(MMICUS_CSC_GENERALINFO_NETWORKINFO_T));
        cur_csc_network_ptr->next = PNULL;
        temp_csc_network_ptr = s_csc_network_name_ptr->csc_network_info;

        if (PNULL == temp_csc_network_ptr)
        {
            s_csc_network_name_ptr->csc_network_info = cur_csc_network_ptr;
        }
        else
        {
            while (PNULL != temp_csc_network_ptr->next)
            {
                temp_csc_network_ptr = temp_csc_network_ptr->next;
            }
            temp_csc_network_ptr->next = cur_csc_network_ptr;
        }

        temp_cur_csc_network_ptr = temp_cur_csc_network_ptr->next;
     }    

    return TRUE;
}

/*****************************************************************************/
// 	Description : Get the network name from csc
//	Global resource dependence : 
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetNetWorkNameByCSC(uint16 mcc, 
                                                                                                            uint16 mnc,
                                                                                                            MMI_STRING_T *name_string_str)
{
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *cur_csc_network_ptr = PNULL;
    uint16 network_num = 0;
    
    if (PNULL == s_csc_network_name_ptr || PNULL == name_string_str)
    {
        return FALSE;
    }

    network_num = s_csc_network_name_ptr->net_work_num;
    cur_csc_network_ptr = s_csc_network_name_ptr->csc_network_info;
    while (PNULL != cur_csc_network_ptr && 0 < network_num--)
    {
        if (cur_csc_network_ptr->mcc == mcc && cur_csc_network_ptr->mnc == mnc)
        {
            name_string_str->wstr_len = strlen((char *)cur_csc_network_ptr->network_name);
            name_string_str->wstr_len = MIN(name_string_str->wstr_len, XML_TAG_NAME_MAX_LEN);
            name_string_str->wstr_ptr = (wchar *)SCI_ALLOCAZ((name_string_str->wstr_len + 1)*sizeof(wchar));
            if (PNULL == name_string_str->wstr_ptr)
            {
                return FALSE;
            }

            MMI_STRNTOWSTR(name_string_str->wstr_ptr, name_string_str->wstr_len, cur_csc_network_ptr->network_name, name_string_str->wstr_len, name_string_str->wstr_len);
            return TRUE;
        }
    }

    return FALSE;
}
#endif


#if ((defined MMI_SIM_LOCK_SUPPORT || defined UNISOC_MMI_SIM_LOCK_SUPPORT)) && (defined MMI_RESET_PHONE_AUTO_PIN)
/*****************************************************************************/
// 	Description : Set whether use pin in nv for SIM lock
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetUsePinInNV(
                      BOOLEAN  use_pin_in_nv)
{
    SCI_TRACE_LOW("MMIAPIPHONE_SetUsePinInNV use_pin_in_nv:%d", use_pin_in_nv);
    s_use_pin_in_nv = use_pin_in_nv;
}
/*****************************************************************************/
// 	Description : Set whether use dual pin in nv for SIM lock
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetUseDualPinInNV(
                      BOOLEAN  use_pin_in_nv,
                      MN_DUAL_SYS_E dual_sys)
{
    SCI_TRACE_LOW("MMIAPIPHONE_SetUseDualPinInNV use_pin_in_nv:%d,dual:%d", use_pin_in_nv,dual_sys);
    s_use_dual_pin_in_nv[dual_sys] = use_pin_in_nv;
}
#endif //#if defined MMI_SIM_LOCK_SUPPORT && defined MMI_RESET_PHONE_AUTO_PIN
/*****************************************************************************/
// 	Description : get plmn name, cmcc, unicom or others id
//	Global resource dependence :
//  Author:dong.chunguang
//	Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIPHONE_GetCarrierNameId(MN_DUAL_SYS_E dual_sys)
{
    MMI_TEXT_ID_T nameid = STR_NSM_IND_NO_SIM;
    uint16 i = 0;
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    uint16 network_table_len = 0;

    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetNetworkName err dual_sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5298_112_2_18_2_45_10_100,(uint8*)"d", dual_sys);
        return PLMN_INVALID;
    }

    if ((g_service_status[dual_sys].plmn.mnc < 100 && (3 == g_service_status[dual_sys].plmn.mnc_digit_num)))
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_mnc_digit_num_3_nametable;
        network_table_len = MAX_NETWORK_MNC_DIGIT_NUM_3_NUM;
    }
    else
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;
        network_table_len = MAX_NETWORK_NUM;
    }

    for (i = 0; i < network_table_len; i++)
    {
        if ((network_table_ptr[i].mnc == g_service_status[dual_sys].plmn.mnc)
            && (network_table_ptr[i].mcc == g_service_status[dual_sys].plmn.mcc))
        {
            nameid = network_table_ptr[i].name_index;
            break;
        }
    }

    return nameid;
}
/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527

LOCAL SCI_EVENT_GROUP_PTR s_iedit_set_event = PNULL;
LOCAL IEDT_HANDLE_SYNC s_iedit_handle_sync={0};


LOCAL void HandleIeditSyncFunc(MMI_HANDLE_T  ctrl_handle, uint16 msg_id, BOOLEAN ret)
{
	if( s_iedit_handle_sync.is_sync &&
		(ctrl_handle == s_iedit_handle_sync.handle) && 
		(msg_id == s_iedit_handle_sync.msg_id)
		)
	{
		s_iedit_handle_sync.result = ret;
		s_iedit_handle_sync.is_sync = FALSE;
		SCI_SetEvent(s_iedit_set_event, msg_id, SCI_OR);
		
	}
}




/*****************************************************************************/
// 	Description : set bg for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetBg (
    MMI_HANDLE_T          ctrl_handle,
    GUI_BG_T              *bg_ptr
)
{
	IEDT_SETBG_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	if(PNULL == bg_ptr)
	{
		return FALSE;
	}
	signal_code = MSG_IEDT_SET_BG;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETBG_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->handle = ctrl_handle;
	signal->bg.bg_type = bg_ptr->bg_type;
	signal->bg.shape = bg_ptr->shape;
	signal->bg.img_id = bg_ptr->img_id;
	signal->bg.color = bg_ptr->color;
	signal->bg.is_screen_img = bg_ptr->is_screen_img;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}

/*****************************************************************************/
// 	Description : set bg for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetBgSync (
    MMI_HANDLE_T          ctrl_handle,
    GUI_BG_T              *bg_ptr
)
{
	uint32   event_flag = 0;
	if(PNULL == bg_ptr)
	{
		return FALSE;
	}
	s_iedit_handle_sync.handle = ctrl_handle;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_BG;
	GUIINLINEEDIT_SetBg(ctrl_handle, bg_ptr);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_BG,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set rect for inline edit
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetDisplayRect (
    MMI_HANDLE_T          ctrl_handle,
    const GUI_RECT_T        *rect_ptr,
	BOOLEAN           is_update
)
{
	IEDT_SETDISPLAYRECT_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	if(PNULL == rect_ptr)
	{
		return FALSE;
	}
	signal_code = MSG_IEDT_SET_DISPLAYRECT;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETDISPLAYRECT_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->handle = ctrl_handle;
	signal->rect.left = rect_ptr->left;
	signal->rect.right = rect_ptr->right;
	signal->rect.top = rect_ptr->top;
	signal->rect.bottom = rect_ptr->bottom;
	signal->is_update = is_update;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}

/*****************************************************************************/
// 	Description : set display rect for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetDisplayRectSync (
    MMI_HANDLE_T          ctrl_handle,
    const GUI_RECT_T        *rect_ptr,
	BOOLEAN           is_update
)
{
	uint32   event_flag = 0;
	if(PNULL == rect_ptr)
	{
		return FALSE;
	}
	s_iedit_handle_sync.handle = ctrl_handle;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_DISPLAYRECT;
	GUIINLINEEDIT_SetDisplayRect(ctrl_handle, rect_ptr, is_update);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_DISPLAYRECT,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set rect for inline edit
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetRect (
    MMI_HANDLE_T          ctrl_handle,
    const GUI_RECT_T        *rect_ptr
)
{
	IEDT_SETRECT_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	if(PNULL == rect_ptr)
	{
		return FALSE;
	}

	signal_code = MSG_IEDT_SET_RECT;

	MmiCreateSignal(signal_code, sizeof(IEDT_SETRECT_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->handle = ctrl_handle;
	signal->rect.left = rect_ptr->left;
	signal->rect.right = rect_ptr->right;
	signal->rect.top = rect_ptr->top;
	signal->rect.bottom = rect_ptr->bottom;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}

/*****************************************************************************/
// 	Description : set rect for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetRectSync (
    MMI_HANDLE_T          ctrl_handle,
    const GUI_RECT_T        *rect_ptr
)
{
	uint32   event_flag = 0;
	if(PNULL == rect_ptr)
	{
		return FALSE;
	}

	s_iedit_handle_sync.handle = ctrl_handle;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_RECT;
	GUIINLINEEDIT_SetRect(ctrl_handle, rect_ptr);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_RECT,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}

/*****************************************************************************/
// 	Description : set string max len for inline edit
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetMaxLen (
    MMI_HANDLE_T          ctrl_handle,
    uint16        str_max_len
)
{
	IEDT_SETLEN_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_MAXLEN;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETLEN_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->handle = ctrl_handle;
	signal->len = str_max_len;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}

/*****************************************************************************/
// 	Description : set maxlen for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetMaxLenSync (
    MMI_HANDLE_T          ctrl_handle,
    uint16        str_max_len
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_handle;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_MAXLEN;
	GUIINLINEEDIT_SetMaxLen(ctrl_handle, str_max_len);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_MAXLEN,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set border for inline edit
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetBorder (
    MMI_HANDLE_T          ctrl_handle,
    GUI_BORDER_T      *border_ptr
)
{
	IEDT_SETBORDER_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_BORDER;
	if(PNULL == border_ptr)
	{
		return FALSE;
	}
	MmiCreateSignal(signal_code, sizeof(IEDT_SETBORDER_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->handle = ctrl_handle;
	signal->border.width = border_ptr->width;
	signal->border.color = border_ptr->color;
	signal->border.type = border_ptr->type;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}



/*****************************************************************************/
// 	Description : set maxlen for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetBorderSync (
    MMI_HANDLE_T          ctrl_handle,
    GUI_BORDER_T      *border_ptr
)
{
	uint32   event_flag = 0;
	if(PNULL == border_ptr)
	{
		return FALSE;
	}
	s_iedit_handle_sync.handle = ctrl_handle;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_BORDER;
	GUIINLINEEDIT_SetBorder(ctrl_handle, border_ptr);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_BORDER,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set cursor for inline edit
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetCursorCurPos (
    MMI_HANDLE_T          ctrl_handle,
    uint16           cur_pos
)
{
	IEDT_SETLEN_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_CURSOR;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETLEN_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->handle = ctrl_handle;
	signal->len = cur_pos;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
// 	Description : set cursor for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetCursorCurPosSync (
    MMI_HANDLE_T          ctrl_handle,
    uint16           cur_pos
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_handle;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_CURSOR;
	GUIINLINEEDIT_SetCursorCurPos(ctrl_handle, cur_pos);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_CURSOR,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set focus for inline edit
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetFocus (
	MMI_HANDLE_T          win_handle,
    MMI_HANDLE_T          ctrl_handle,
	BOOLEAN           is_focus,
    BOOLEAN           is_need_paint
)
{
	IEDT_SETFOCUS_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_FOCUS;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETFOCUS_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->win_handle = win_handle;
	signal->handle = ctrl_handle;
	signal->is_focus = is_focus;
	signal->is_need_paint = is_need_paint;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
// 	Description : set cursor for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetFocusSync (
	MMI_HANDLE_T          win_handle,
    MMI_HANDLE_T          ctrl_handle,
	BOOLEAN           is_focus,
    BOOLEAN           is_need_paint
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_handle;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_FOCUS;
	GUIINLINEEDIT_SetFocus(win_handle, ctrl_handle, is_focus, is_need_paint);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_FOCUS,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set font for inline edit
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetFont (
   MMI_CTRL_ID_T   ctrl_id,//in
   GUI_FONT_T      font    //in
)
{
	IEDT_SETFONT_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_FONT;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETFONT_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->font = font;

	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}

/*****************************************************************************/
// 	Description : set font for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetFontSync (
   MMI_CTRL_ID_T   ctrl_id,//in
   GUI_FONT_T      font    //in
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_FONT;
	GUIINLINEEDIT_SetFont(ctrl_id, font);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_FONT,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set font for inline edit
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetFontColor (
   MMI_CTRL_ID_T   ctrl_id,//in
   GUI_COLOR_T    font_color    //in
)
{
	IEDT_SETFONTCOLOR_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_FONTCOLOR;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETFONTCOLOR_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->fontcolor = font_color;

	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}

/*****************************************************************************/
// 	Description : set font for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetFontColorSync (
   MMI_CTRL_ID_T   ctrl_id,//in
   GUI_COLOR_T    font_color    //in
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_FONTCOLOR;
	GUIINLINEEDIT_SetFontColor(ctrl_id, font_color);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_FONTCOLOR,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set font for inline edit
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetIM (
    MMI_CTRL_ID_T     ctrl_id,    //in
    GUIIM_TYPE_T      allow_im,   //in:允许的输入集合
    GUIIM_TYPE_T      init_im     //in:初始输入法
)
{
	IEDT_SETIM_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_IM;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETIM_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->allow_im = allow_im;
	signal->init_im = init_im;

	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}

/*****************************************************************************/
// 	Description : set IM for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetIMSync (
    MMI_CTRL_ID_T     ctrl_id,    //in
    GUIIM_TYPE_T      allow_im,   //in:允许的输入集合
    GUIIM_TYPE_T      init_im     //in:初始输入法
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_IM;
	GUIINLINEEDIT_SetIM(ctrl_id, allow_im, init_im);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_IM,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set invisible
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetVisible (
    MMI_CTRL_ID_T     ctrl_id,    //in
    GUIIM_TYPE_T      is_visible,   //in:允许的输入集合
    GUIIM_TYPE_T      is_update     //in:初始输入法
)
{
	IEDT_SETVISIBLE_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_VISIBLE;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETVISIBLE_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->is_visible = is_visible;
	signal->is_update = is_update;

	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
	
}


/*****************************************************************************/
// 	Description : set IM for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetVisibleSync (
    MMI_CTRL_ID_T     ctrl_id,    //in
    GUIIM_TYPE_T      is_visible,   //in:允许的输入集合
    GUIIM_TYPE_T      is_update     //in:初始输入法
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_VISIBLE;
	GUIINLINEEDIT_SetVisible(ctrl_id, is_visible, is_update);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_VISIBLE,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set invisible
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetStyle (
    MMI_CTRL_ID_T     ctrl_id,    //in
	GUIEDIT_STYLE_E    style
)
{
	IEDT_SETSTYLE_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_STYLE;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETSTYLE_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->style = style;

	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}
/*****************************************************************************/
// 	Description : set style for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetStyleeSync (
    MMI_CTRL_ID_T     ctrl_id,    //in
	GUIEDIT_STYLE_E    style
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_STYLE;
	GUIINLINEEDIT_SetStyle(ctrl_id, style);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_STYLE,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set invisible
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetString (
    MMI_CTRL_ID_T     ctrl_id,    //in
	wchar*    wstr_ptr,
	uint16   wstr_len
	)
{
	IEDT_SETSTRING_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_STRING;
	if(PNULL == wstr_ptr)
	{
		return FALSE;
	}
	MmiCreateSignal(signal_code, sizeof(IEDT_SETSTRING_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->wstr_len = wstr_len;
	signal->wstr_ptr = SCI_ALLOCA(sizeof(wchar)*wstr_len);
	if(PNULL==signal->wstr_ptr)
	{
		return FALSE;
	}
	MMIAPICOM_Wstrncpy(signal->wstr_ptr,wstr_ptr,wstr_len);
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}
/*****************************************************************************/
// 	Description : set string for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetStringSync (
    MMI_CTRL_ID_T     ctrl_id,    //in
	wchar*    wstr_ptr,
	uint16   wstr_len
	)
{
	uint32   event_flag = 0;

	if(PNULL == wstr_ptr)
	{
		return FALSE;
	}
	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_STRING;
	GUIINLINEEDIT_SetString(ctrl_id, wstr_ptr, wstr_len);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_STRING,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}
/*****************************************************************************/
// 	Description : set HandleRedKey
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetHandleRedKey (
    BOOLEAN         is_handle,
    MMI_CTRL_ID_T   ctrl_id
)
{
	IEDT_SETHANDLEREDKEY_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_HANDLEREDKEY;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETHANDLEREDKEY_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->is_handle = is_handle;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}



/*****************************************************************************/
// 	Description : set HandleRedKey for inline edit
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetHandleRedKeySync (
    BOOLEAN         is_handle,
    MMI_CTRL_ID_T   ctrl_id
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_HANDLEREDKEY;
	GUIINLINEEDIT_SetHandleRedKey(is_handle, ctrl_id);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_HANDLEREDKEY,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;

}


/*****************************************************************************/
//  Description : set edit password style
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetPasswordStyle (
    MMI_CTRL_ID_T              ctrl_id,
    GUIEDIT_PASSWORD_STYLE_E   style
)
{
	IEDT_SETPSWSTYLE_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_PSWSTYLE;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETPSWSTYLE_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->style = style;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}

/*****************************************************************************/
//  Description : set edit password style
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetPasswordStyleSync (
    MMI_CTRL_ID_T              ctrl_id,
    GUIEDIT_PASSWORD_STYLE_E   style
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_HANDLEREDKEY;
	GUIINLINEEDIT_SetPasswordStyle(ctrl_id, style);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_PSWSTYLE,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}


/*****************************************************************************/
//  Description : set edit DefaultString
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetDefaultString (
    MMI_CTRL_ID_T              ctrl_id,
    wchar               *default_string_ptr,    //in:
    uint16              default_string_len      //in:
)
{
	IEDT_SETDEFSTR_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_DEFSTR;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETDEFSTR_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->wstr_ptr = SCI_ALLOCA(sizeof(wchar)*default_string_len);
	if(PNULL==signal->wstr_ptr)
	{
		return FALSE;
	}
	MMIAPICOM_Wstrncpy(signal->wstr_ptr,default_string_ptr,default_string_len);
	signal->wstr_len =default_string_len;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : set edit DefaultString
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetDefaultStringSync (
    MMI_CTRL_ID_T              ctrl_id,
    wchar               *default_string_ptr,    //in:
    uint16              default_string_len      //in:
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_DEFSTR;
	GUIINLINEEDIT_SetDefaultString(ctrl_id, default_string_ptr, default_string_len);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_DEFSTR,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}


/*****************************************************************************/
//  Description : set edit enable
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_ConfigDisableChar (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    wchar             *value_ptr, //in:
    uint16            value_count //in:
)
{
	IEDT_CONFIGDISCHAR_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_CONFIGDISCHAR;
	MmiCreateSignal(signal_code, sizeof(IEDT_CONFIGDISCHAR_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->value_ptr = SCI_ALLOCA(sizeof(uint16)*value_count);
	if(PNULL==signal->value_ptr)
	{
		return FALSE;
	}
	MMIAPICOM_Wstrncpy(signal->value_ptr,value_ptr,value_count);
	signal->value_count =value_count;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : set edit enable
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_ConfigDisableCharSync (
    MMI_CTRL_ID_T              ctrl_id,
    wchar             *value_ptr, //in:
    uint16            value_count //in:
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_CONFIGDISCHAR;
	GUIINLINEEDIT_ConfigDisableChar(ctrl_id, value_ptr, value_count);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_CONFIGDISCHAR,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}


/*****************************************************************************/
//  Description : set edit enable
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetEnable (
    MMI_HANDLE_T      ctrl_handle,
    BOOLEAN           is_enabled
)
{
	IEDT_SETENABLE_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_ENABLE;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETENABLE_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_handle = ctrl_handle;
	signal->is_enable = is_enabled;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : set edit enable
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetEnableSync (
    MMI_HANDLE_T      ctrl_handle,
    BOOLEAN           is_enabled
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_handle;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_ENABLE;
	GUIINLINEEDIT_SetEnable(ctrl_handle, is_enabled);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_ENABLE,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}


/*****************************************************************************/
//  Description : set edit enable
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetDispLeftNum (
    MMI_CTRL_ID_T     ctrl_id,                //in:
    BOOLEAN           is_disp_num_info        //in:
)
{
	IEDT_SETDISPLEFTNUM_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_DISPLEFTNUM;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETDISPLEFTNUM_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->is_disp_num_info = is_disp_num_info;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : set edit enable
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetDispLeftNumSync (
    MMI_CTRL_ID_T     ctrl_id,                //in:
    BOOLEAN           is_disp_num_info        //in:
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_DISPLEFTNUM;
	GUIINLINEEDIT_SetDispLeftNum(ctrl_id, is_disp_num_info);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_DISPLEFTNUM,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}


/*****************************************************************************/
//  Description : set edit align
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetAlign (
    MMI_CTRL_ID_T  ctrl_id,//in
    GUI_ALIGN_E    align   //in
)
{
	IEDT_SETALIGN_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_ALIGN;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETALIGN_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->align = align;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : set edit align
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetAlignSync (
    MMI_CTRL_ID_T  ctrl_id,//in
    GUI_ALIGN_E    align   //in
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_ALIGN;
	GUIINLINEEDIT_SetAlign(ctrl_id, align);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_ALIGN,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}


/*****************************************************************************/
//  Description : set edit margin
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetMargin (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    uint16            margin_lr,  //in:left and right margin
    uint16            margin_tb   //in:top and bottom margin
)
{
	IEDT_SETMARGIN_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_MARGIN;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETMARGIN_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->margin_lr = margin_lr;
	signal->margin_tb = margin_tb;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : set edit MARGIN
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetMarginSync (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    uint16            margin_lr,  //in:left and right margin
    uint16            margin_tb   //in:top and bottom margin
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_MARGIN;
	GUIINLINEEDIT_SetMargin(ctrl_id, margin_lr, margin_tb);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_MARGIN,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}


/*****************************************************************************/
//  Description : set edit cursor hide
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetCursorHideState (
    MMI_CTRL_ID_T     ctrl_id,    //in:
	BOOLEAN          is_hide
)
{
	IEDT_SETCURSORHIDE_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_CURSORHIDE;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETCURSORHIDE_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->is_hide = is_hide;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : set edit cursor hide
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetCursorHideStateSync (
    MMI_CTRL_ID_T     ctrl_id,    //in:
	BOOLEAN          is_hide
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_CURSORHIDE;
	GUIINLINEEDIT_SetCursorHideState(ctrl_id, is_hide);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_CURSORHIDE,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}

/*****************************************************************************/
//  Description : set edit if permit border
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_PermitBorder  (
    MMI_CTRL_ID_T     ctrl_id,    //in:
	BOOLEAN          is_permit
)
{
	IEDT_PERMITBORDER_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_PERMITBORDER;
	MmiCreateSignal(signal_code, sizeof(IEDT_PERMITBORDER_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->is_permit = is_permit;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : set edit if permit border
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_PermitBorderSync (
    MMI_CTRL_ID_T     ctrl_id,    //in:
	BOOLEAN          is_permit
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_PERMITBORDER;
	GUIINLINEEDIT_PermitBorder(ctrl_id, is_permit);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_PERMITBORDER,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}

/*****************************************************************************/
//  Description : set edit bg Transparent
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetBgTransparent  (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_transparent
)
{
	IEDT_SETBGTRANSPARENT_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_BGTRANSPARENT;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETBGTRANSPARENT_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->is_transparent = is_transparent;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : set edit  bg Transparent
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetBgTransparentSync (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    BOOLEAN         is_transparent
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_BGTRANSPARENT;
	GUIINLINEEDIT_SetBgTransparent(ctrl_id, is_transparent);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_BGTRANSPARENT,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}

/*****************************************************************************/
//  Description : set edit dividing line
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetDividingLine  (
    MMI_CTRL_ID_T   ctrl_id,
    uint16          line_width, //in:0 don't display dividing line
    GUI_COLOR_T     line_color  //in:
)
{
	IEDT_SETDIVIDINGLINE_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_DIVIDINGLINE;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETDIVIDINGLINE_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->line_width = line_width;
	signal->line_color = line_color;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : set edit  dividing line
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetDividingLineSync (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    uint16          line_width, //in:0 don't display dividing line
    GUI_COLOR_T     line_color  //in:
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_DIVIDINGLINE;
	GUIINLINEEDIT_SetDividingLine(ctrl_id, line_width, line_color);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_DIVIDINGLINE,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}


/*****************************************************************************/
//  Description : set edit dividing line
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetThemeLine  (
    MMI_CTRL_ID_T   ctrl_id,
    GUI_COLOR_T     focused_color, //in:0 don't display dividing line
    GUI_COLOR_T     unfocused_color  //in:
)
{
	IEDT_SETTHEMELINE_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_SET_THEMELINE;
	MmiCreateSignal(signal_code, sizeof(IEDT_SETTHEMELINE_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	signal->line_focused_color = focused_color;
	signal->line_unfocused_color = unfocused_color;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : set edit  dividing line
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_SetThemeLineSync (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    GUI_COLOR_T          focused_color, //in:0 don't display dividing line
    GUI_COLOR_T     unfocused_color  //in:
)
{
	uint32   event_flag = 0;

	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_SET_THEMELINE;
	GUIINLINEEDIT_SetThemeLine(ctrl_id, focused_color, unfocused_color);
    if(PNULL != s_iedit_set_event)
    {
         SCI_GetEvent(s_iedit_set_event, 
                        MSG_IEDT_SET_THEMELINE,
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
    }
	return s_iedit_handle_sync.result;
}

/*****************************************************************************/
//  Description : clear all string
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_ClearAllStr (
											MMI_CTRL_ID_T   ctrl_id

											)
{
	IEDT_CLEARSTR_SIGNAL* signal=	PNULL;
    uint16      signal_code = 0;
	signal_code = MSG_IEDT_CLEARSTR;
	MmiCreateSignal(signal_code, sizeof(IEDT_CLEARSTR_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->ctrl_id = ctrl_id;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
	return TRUE;
}


/*****************************************************************************/
//  Description : clear all string
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIINLINEEDIT_ClearAllStrSync (
											   MMI_CTRL_ID_T     ctrl_id    //in:
											   )
{
	uint32   event_flag = 0;
	
	s_iedit_handle_sync.handle = ctrl_id;
	s_iedit_handle_sync.is_sync = TRUE;
	s_iedit_handle_sync.msg_id = MSG_IEDT_CLEARSTR;
	GUIINLINEEDIT_ClearAllStr(ctrl_id);
    if(PNULL != s_iedit_set_event)
    {
		SCI_GetEvent(s_iedit_set_event, 
			MSG_IEDT_CLEARSTR,
			SCI_OR_CLEAR,
			&event_flag, 
			SCI_WAIT_FOREVER
			);
    }
	return s_iedit_handle_sync.result;
}
LOCAL IEDTRETFUNC s_iedit_ret_callback;
/*****************************************************************************/
//  Description : for inlineeditor
//  Global resource dependence :
//  Author: mary xiao
//  Note:
/*****************************************************************************/
PUBLIC void RegisterIEditorFuncRetListener(IEDTRETFUNC func)
{
	s_iedit_ret_callback = func;
}

/*****************************************************************************/
//  Description : for inlineeditor
//  Global resource dependence :
//  Author: mary xiao
//  Note:
/*****************************************************************************/
PUBLIC void UnregisterIEditorFuncRetListener(IEDTRETFUNC func)
{
	s_iedit_ret_callback =	PNULL;
}
/*****************************************************************************/
//  Description : HandleInlineEditMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
////for inline edit
LOCAL MMI_RESULT_E HandleInlineEditMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
	BOOLEAN ret = FALSE;
	switch(msg_id)
	{
		
	case MSG_IEDT_SET_BG:
		{
			IEDT_SETBG_SIGNAL setbg_sig = *((IEDT_SETBG_SIGNAL *)param);
			ret = GUIEDIT_SetBg(setbg_sig.handle, &(setbg_sig.bg));
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setbg_sig.handle, msg_id, ret);
			}
			HandleIeditSyncFunc(setbg_sig.handle, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_RECT:
		{
			IEDT_SETRECT_SIGNAL setrect_sig = *((IEDT_SETRECT_SIGNAL *)param);
			ret = GUIEDIT_SetRect(setrect_sig.handle, &(setrect_sig.rect));
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setrect_sig.handle, msg_id, ret);
			}
			HandleIeditSyncFunc(setrect_sig.handle, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_DISPLAYRECT:
		{
			IEDT_SETDISPLAYRECT_SIGNAL setdisplayrect_sig = *((IEDT_SETDISPLAYRECT_SIGNAL *)param);
			ret = GUIEDIT_SetDisplayRect(setdisplayrect_sig.handle, &(setdisplayrect_sig.rect), setdisplayrect_sig.is_update);
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setdisplayrect_sig.handle, msg_id, ret);
			}
			HandleIeditSyncFunc(setdisplayrect_sig.handle, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_MAXLEN:
		{
			IEDT_SETLEN_SIGNAL setmaxlen_sig = *((IEDT_SETLEN_SIGNAL *)param);
			GUIEDIT_SetMaxLen(setmaxlen_sig.handle, setmaxlen_sig.len);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setmaxlen_sig.handle, msg_id, ret);
			}
			HandleIeditSyncFunc(setmaxlen_sig.handle, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_BORDER:
		{
			IEDT_SETBORDER_SIGNAL setborder_sig = *((IEDT_SETBORDER_SIGNAL *)param);
			ret = GUIEDIT_SetBorder(setborder_sig.handle, &(setborder_sig.border));
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setborder_sig.handle, msg_id, ret);
			}
			HandleIeditSyncFunc(setborder_sig.handle, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_CURSOR:
		{
			IEDT_SETLEN_SIGNAL setcursor_sig = *((IEDT_SETLEN_SIGNAL *)param);
			ret = GUIEDIT_SetCursorCurPos(setcursor_sig.handle, setcursor_sig.len);
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setcursor_sig.handle, msg_id, ret);
			}
			HandleIeditSyncFunc(setcursor_sig.handle, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_FOCUS:
		{
			IEDT_SETFOCUS_SIGNAL setfocus_sig = *((IEDT_SETFOCUS_SIGNAL *)param);
			if(setfocus_sig.is_focus)
			{
				
				ret = MMK_SetAtvCtrlEx(setfocus_sig.win_handle, setfocus_sig.handle, setfocus_sig.is_need_paint);
				MMK_SendMsg(setfocus_sig.handle, MSG_CTL_PAINT, PNULL);
			}
			else
			{
				if(GUIEDIT_IsActive(setfocus_sig.handle))
				{
				
					ret = CTRLBASE_SetActiveEx(setfocus_sig.handle,FALSE,FALSE);
					MMK_SendMsg(setfocus_sig.handle, MSG_CTL_LOSE_ACTIVE, PNULL);
					MMK_SendMsg(setfocus_sig.handle, MSG_CTL_PAINT, PNULL);
				}
				else
				{
					ret = TRUE;
				}
			}
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setfocus_sig.handle, msg_id, ret);
			}
			HandleIeditSyncFunc(setfocus_sig.handle, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_FONT:
		{
			IEDT_SETFONT_SIGNAL setfont_sig = *((IEDT_SETFONT_SIGNAL *)param);
			GUIEDIT_SetFont(setfont_sig.ctrl_id , setfont_sig.font);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setfont_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setfont_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_FONTCOLOR:
		{
			IEDT_SETFONTCOLOR_SIGNAL setfontcolor_sig = *((IEDT_SETFONTCOLOR_SIGNAL *)param);
			 GUIEDIT_SetFontColor(setfontcolor_sig.ctrl_id , setfontcolor_sig.fontcolor);
			 ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setfontcolor_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setfontcolor_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_IM:
		{
			IEDT_SETIM_SIGNAL setim_sig = *((IEDT_SETIM_SIGNAL *)param);
			GUIEDIT_SetIm(setim_sig.ctrl_id , setim_sig.allow_im, setim_sig.init_im);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setim_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setim_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_VISIBLE:
		{
			IEDT_SETVISIBLE_SIGNAL setvisible_sig = *((IEDT_SETVISIBLE_SIGNAL *)param);
			GUIEDIT_SetVisible(setvisible_sig.ctrl_id , setvisible_sig.is_visible, setvisible_sig.is_update);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setvisible_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setvisible_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_STYLE:
		{
			IEDT_SETSTYLE_SIGNAL setstyle_sig = *((IEDT_SETSTYLE_SIGNAL *)param);
			GUIEDIT_SetStyle(setstyle_sig.ctrl_id , setstyle_sig.style);
			ret =TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setstyle_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setstyle_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_STRING:
		{
			IEDT_SETSTRING_SIGNAL setstring_sig = *((IEDT_SETSTRING_SIGNAL *)param);
			ret = GUIEDIT_SetString(setstring_sig.ctrl_id , setstring_sig.wstr_ptr, setstring_sig.wstr_len);
			SCI_FREE(setstring_sig.wstr_ptr);
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setstring_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setstring_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_HANDLEREDKEY:
		{
			IEDT_SETHANDLEREDKEY_SIGNAL sethandleredkey_sig = *((IEDT_SETHANDLEREDKEY_SIGNAL *)param);
			GUIEDIT_SetHandleRedKey(sethandleredkey_sig.is_handle, sethandleredkey_sig.ctrl_id);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(sethandleredkey_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(sethandleredkey_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_PSWSTYLE:
		{
			IEDT_SETPSWSTYLE_SIGNAL setPswStyle_sig = *((IEDT_SETPSWSTYLE_SIGNAL *)param);
			GUIEDIT_SetPasswordStyle(setPswStyle_sig.ctrl_id, setPswStyle_sig.style);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setPswStyle_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setPswStyle_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_DEFSTR:
		{
			IEDT_SETDEFSTR_SIGNAL setdefstr_sig = *((IEDT_SETDEFSTR_SIGNAL *)param);
			GUIEDIT_SetDefaultString(setdefstr_sig.ctrl_id, setdefstr_sig.wstr_ptr, setdefstr_sig.wstr_len);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setdefstr_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setdefstr_sig.ctrl_id, msg_id, ret);
			SCI_FREE (setdefstr_sig.wstr_ptr);
		}
		break;	
	case MSG_IEDT_SET_CONFIGDISCHAR:
		{
			IEDT_CONFIGDISCHAR_SIGNAL configdischar_sig = *((IEDT_CONFIGDISCHAR_SIGNAL *)param);
			GUIEDIT_ConfigDisableChar(configdischar_sig.ctrl_id, configdischar_sig.value_ptr, configdischar_sig.value_count);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(configdischar_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(configdischar_sig.ctrl_id, msg_id, ret);
		}
		break;	
	case MSG_IEDT_SET_ENABLE:
		{
			IEDT_SETENABLE_SIGNAL setenable_sig = *((IEDT_SETENABLE_SIGNAL *)param);
			CTRLBASE_SetEnabled(setenable_sig.ctrl_handle, setenable_sig.is_enable);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setenable_sig.ctrl_handle, msg_id, ret);
			}
			HandleIeditSyncFunc(setenable_sig.ctrl_handle, msg_id, ret);
		}
		break;

	case MSG_IEDT_SET_DISPLEFTNUM:
		{
			IEDT_SETDISPLEFTNUM_SIGNAL setdispleftnum_sig = *((IEDT_SETDISPLEFTNUM_SIGNAL *)param);
			CTRLBASEFLEX_SetDispLeftNum(setdispleftnum_sig.ctrl_id, setdispleftnum_sig.is_disp_num_info);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setdispleftnum_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setdispleftnum_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_ALIGN:
		{
			IEDT_SETALIGN_SIGNAL setalign_sig = *((IEDT_SETALIGN_SIGNAL *)param);
			GUIEDIT_SetAlign(setalign_sig.ctrl_id, setalign_sig.align);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setalign_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setalign_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_MARGIN:
		{
			IEDT_SETMARGIN_SIGNAL setmargin_sig = *((IEDT_SETMARGIN_SIGNAL *)param);
			GUIEDIT_SetMargin(setmargin_sig.ctrl_id, setmargin_sig.margin_lr, setmargin_sig.margin_tb);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setmargin_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setmargin_sig.ctrl_id, msg_id, ret);
		}
		break;

	case MSG_IEDT_SET_CURSORHIDE:
		{
			IEDT_SETCURSORHIDE_SIGNAL setcursorhide_sig = *((IEDT_SETCURSORHIDE_SIGNAL *)param);
			GUIEDIT_SetCursorHideState(setcursorhide_sig.ctrl_id, setcursorhide_sig.is_hide);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setcursorhide_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setcursorhide_sig.ctrl_id, msg_id, ret);
		}
		break;

	case MSG_IEDT_SET_PERMITBORDER:
		{
			IEDT_PERMITBORDER_SIGNAL permitborder_sig = *((IEDT_PERMITBORDER_SIGNAL *)param);
			GUIEDIT_PermitBorder(permitborder_sig.ctrl_id, permitborder_sig.is_permit);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(permitborder_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(permitborder_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_BGTRANSPARENT:
		{
			IEDT_SETBGTRANSPARENT_SIGNAL setbgtransparent_sig = *((IEDT_SETBGTRANSPARENT_SIGNAL *)param);
			GUIEDIT_SetBgTransparent(setbgtransparent_sig.ctrl_id, setbgtransparent_sig.is_transparent);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setbgtransparent_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setbgtransparent_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_DIVIDINGLINE:
		{
			IEDT_SETDIVIDINGLINE_SIGNAL setdivingline_sig = *((IEDT_SETDIVIDINGLINE_SIGNAL *)param);
			GUIEDIT_SetDividingLine(setdivingline_sig.ctrl_id, setdivingline_sig.line_width, setdivingline_sig.line_color);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setdivingline_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setdivingline_sig.ctrl_id, msg_id, ret);
		}
		break;
	case MSG_IEDT_SET_THEMELINE:
		{
			IEDT_SETTHEMELINE_SIGNAL setthemeline_sig = *((IEDT_SETTHEMELINE_SIGNAL *)param);
			GUIEDIT_SetThemeLine(setthemeline_sig.ctrl_id, setthemeline_sig.line_focused_color, setthemeline_sig.line_unfocused_color);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(setthemeline_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(setthemeline_sig.ctrl_id, msg_id, ret);
		}
		break;

	case MSG_IEDT_CLEARSTR:
		{
			IEDT_CLEARSTR_SIGNAL clearstr_sig = *((IEDT_CLEARSTR_SIGNAL *)param);
			GUIEDIT_ClearAllStr(clearstr_sig.ctrl_id);
			ret = TRUE;
			if(PNULL != s_iedit_ret_callback)
			{
				s_iedit_ret_callback(clearstr_sig.ctrl_id, msg_id, ret);
			}
			HandleIeditSyncFunc(clearstr_sig.ctrl_id, msg_id, ret);
		}
		break;		
	default:
		break;

	}
    return ret;
	
}

////for inline edit
PUBLIC void GUIINLINEEDIT_Init(void)
{	
	g_ctrl_inlineedit.ProcessMsg = HandleInlineEditMsg;
    if(PNULL != s_iedit_set_event)
    {
        SCI_DeleteEvent(s_iedit_set_event);
        s_iedit_set_event = PNULL;
    }
	s_iedit_set_event = SCI_CreateEvent("s_iedit_set_event");
}

#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
LOCAL BOOLEAN MMIPHONE_IsLegalSim(APP_MN_SIM_READY_IND_T* sim_ready_ind_ptr)
{
    BOOLEAN result = FALSE;
    
    uint32 i = 0;
    uint16 mcc = 0;
    uint16 mnc =0;
	
    SCI_TRACE_LOW("MMIPHONE_IsLegalSim,mnc_digit_num=%d,",sim_ready_ind_ptr->hplmn.mnc_digit_num );

    if(s_is_simlock_off)
    {
        SCI_TRACE_LOW("MMIPHONE_IsLegalSim, SIM LOCK net is unlocked");//已经解锁
        return TRUE;
    }

    //hplmn :表示sim卡的home plmn信息，从PS Layer 得到。
    for(i = 0; i < MMI_MAX_SIM_LOCK_NUM; i++)
    {
        if((sim_lock_table[i].mcc == sim_ready_ind_ptr->hplmn.mcc)
            &&(sim_lock_table[i].mnc == sim_ready_ind_ptr->hplmn.mnc))
        {
            result = TRUE; 
        }
    }

    //TEST
    if( !result )
    {
        MN_PLMN_T plmn_str = MNSIM_GetHplmnEx(sim_ready_ind_ptr->dual_sys);
        SCI_TRACE_LOW("MMIPHONE_IsLegalSim (sim_%d), MNSIM_GetHplmnEx plmn_str(%d,%d,%d)",sim_ready_ind_ptr->dual_sys+1,plmn_str.mcc,plmn_str.mnc,plmn_str.mnc_digit_num);
	    for(i = 0; i < MMI_MAX_SIM_LOCK_NUM; i++)
	    {
	        if((sim_lock_table[i].mcc == plmn_str.mcc)
	            &&(sim_lock_table[i].mnc == plmn_str.mnc))
	        {
	            result = TRUE; 
	        }
	    }
    }
	
    // 防止某些特殊情况下hplmn信息不准确导致匹配错误:不同的sim卡或者网络可能会有一些不可预知的错误产生.
    // 因此，重新读取一次SIM card 的imsi 信息，从中截取hplmn,这个执行过程相对来说需要多耗费一些开机初始化时间
    //get mcc and mnc from imsi of sim card .
    if( !result )
    {
        MN_IMSI_T imsi_str = MNSIM_GetImsiEx(sim_ready_ind_ptr->dual_sys);

        mcc = 
        (imsi_str.imsi_val[0] >> 4) * 100 + 
        (imsi_str.imsi_val[1] & 0x0f) * 10 + 
        (imsi_str.imsi_val[1] >> 4);
        
		//mnc length maybe three or two !!!!
		if(sim_ready_ind_ptr->hplmn.mnc_digit_num == 3)
        {
            mnc = 
                (imsi_str.imsi_val[2] & 0x0f) * 100 + 
                (imsi_str.imsi_val[2] >> 4)*10 +
                (imsi_str.imsi_val[3] & 0x0f);
        }
        else
        {
            mnc = 
                (imsi_str.imsi_val[2] & 0x0f) * 10 + 
                (imsi_str.imsi_val[2] >> 4);
        }

        for(i = 0; i < MMI_MAX_SIM_LOCK_NUM; i++)
        {            
            if((sim_lock_table[i].mcc == mcc)
                &&(sim_lock_table[i].mnc == mnc))
            {
                result = TRUE; 
            }
        }
    }

    SCI_TRACE_LOW("simlock : MMIPHONE_IsLegalSim(sim_%d),hplmn(%d,%d),IMSI-Plmn(%d,%d),result = [%d]",
         sim_ready_ind_ptr->dual_sys+1,sim_ready_ind_ptr->hplmn.mcc,sim_ready_ind_ptr->hplmn.mnc,mcc,mnc,result);
    
    return result;
}

/*****************************************************************************/
// 	Description : Adjust the running state is Legal or not: TRUE(LOCKED),FALSE(UNLOCKED)
//	Global resource dependence : 
//  Author: liezhen.sha
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIPHONE_IsLegalState(void)
{      
    BOOLEAN result = FALSE;    

    //sim status is set in sim not ready or sim ready happened!
    //Thus,the function should be called after the status set.
    SIM_STATUS_E sim1_status = MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1);     
#ifdef MMI_MULTI_SIM_SYS_DUAL 
    SIM_STATUS_E sim2_status = MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2);
#elif defined(MMI_MULTI_SIM_SYS_SINGLE)
    SIM_STATUS_E sim2_status = SIM_NOT_INITED;
#endif

    if(s_is_sim1_right)
    {
        switch(sim1_status)
        {
            case SIM_STATUS_OK:
            case SIM_STATUS_PIN_BLOCKED:
            case SIM_STATUS_PUK_BLOCKED:
            case SIM_STATUS_PUK2_BLOCKED:
                result = TRUE;
                break;
            default:
                break;
        }
   }

#ifdef MMI_MULTI_SIM_SYS_DUAL 
   if(s_is_sim2_right)
   {
        switch(sim2_status)
        {
            case SIM_STATUS_OK:
            case SIM_STATUS_PIN_BLOCKED:
            case SIM_STATUS_PUK_BLOCKED:
            case SIM_STATUS_PUK2_BLOCKED:
                result = TRUE;
                break;
            default:
                break;
        }
   }
#endif
   
   SCI_TRACE_LOW( "MMIPHONE_IsLegalState(result = %d),sim_lock, sim1_status = %d,sim2_status = %d,s_is_sim1_right=%d,s_is_sim2_right",
                                  result,sim1_status,sim2_status, s_is_sim1_right, s_is_sim2_right );

   return result;        
}

/*****************************************************************************/
// 	Description : Get is any sim is valid
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsLegalState(void)
{
    return MMIPHONE_IsLegalState();
}

/*****************************************************************************/
// 	Description : MMIPHONE_Get SIM lock Config Data Ptr
//	Global resource dependence : none
//  Author: liezhen.sha
//	Note:
/*****************************************************************************/
PUBLIC PHONE_SIM_ID_T *MMIAPIPHONE_GetSIMLockTable(void)
{
    return  &sim_lock_table;
}

/*****************************************************************************/
// 	Description : MMIPHONE_Get SIM lock Config Data Ptr
//	Global resource dependence : none
//  Author: liezhen.sha
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIPHONE_GetSIMLockNum(void)
{
    return  MMI_MAX_SIM_LOCK_NUM;
}

/*****************************************************************************/
// 	Description : MMIPHONE_Get Sim1 Power ON Status
//	Global resource dependence : none
//  Author: liezhen.sha
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetSIM1PowerOnStatus(void)
{
    return  s_is_sim1_poweron;
}

/*****************************************************************************/
// 	Description : MMIPHONE_Set Sim1 Power ON Status
//	Global resource dependence : none
//  Author: liezhen.sha
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_SetSIM1PowerOnStatus(BOOLEAN status)
{
    s_is_sim1_poweron = status;
}

/*****************************************************************************/
// 	Description : MMIPHONE_Get Sim2 Power ON Status
//	Global resource dependence : none
//  Author: liezhen.sha
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetSIM2PowerOnStatus(void)
{
    return  s_is_sim2_poweron;
}

/*****************************************************************************/
// 	Description : MMIPHONE_Set Sim1 Power ON Status
//	Global resource dependence : none
//  Author: liezhen.sha
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_SetSIM2PowerOnStatus(BOOLEAN status)
{
    s_is_sim2_poweron = status;
}

/*****************************************************************************/
// 	Description : MMIPHONE_Check simlock Status
//	Global resource dependence : none
//  Author: liezhen.sha
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_CheckSimLockStatus(void)
{
#ifdef MMI_SIM_LOCK_TYPE2_UNLOCK_SUPPORT
    //取NV中记录的锁网状态 TRUE:已解锁 FALSE:未解锁
    if(MMIAPISET_GetNetLockSetting())
    {
        SCI_TRACE_LOW("MMIAPIPHONE_CheckSimLockStatus, SIM LOCK net is unlocked");//已经解锁
        s_is_simlock_off = TRUE;
    }
#endif
}

/*****************************************************************************/
// 	Description : MMIPHONE_Set Is In Pwd Win
//	Global resource dependence : none
//  Author: liezhen.sha
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_SetIsInPwdWin(BOOLEAN status)
{
    s_is_in_pwd_win = status;
}

PUBLIC void MMIAPIPHONE_PowerOnPsForEmerge(const MN_DUAL_SYS_E dual_sys)
{
	PowerOnPsForEmerge(dual_sys);
}
#endif /* MMI_SIM_LOCK_TYPE2_SUPPORT */
