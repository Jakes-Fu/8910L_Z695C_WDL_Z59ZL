/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_ia_java_trc.h"
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "os_api.h"
#include "dal_time.h"
#include "kjava_sys_core.h"
#include "guilcd.h"
#include "mmk_type.h"
#include "ajms_arc.h"
#include "block_mem.h"
#include "mn_type.h"


#include "mmijava_internal.h"
#include "mmifmm_export.h"
#include "mmk_app.h"
#include "mmi_appmsg.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif

#ifdef WORLD_CLOCK_SUPPORT
#include "mmiwclk_export.h"
#endif
//#include "mmiset.h"
#include "mmiset_export.h"
#define DebugForMM

#ifdef JAVA_LOW_POWER_ENABLE          
#ifndef WIN32
#include "mn_api.h"
#endif
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define KJAVA_SECOND_BLOCK_SIZE      (50*1024)
#define KJAVA_SECOND_BLOC_SIZE_FOR_INSTALL  (600*1024)

extern int g_runningTckLevel;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN s_kjava_time_is_inited = FALSE;
LOCAL uint32 s_begin_tick = 0;
const char java_event_name[] = "java event";
const char kjava_sem_name[] = "kjava semaphore1";
const char kjava_sem2_name[] = "kjava semaphore2";

LOCAL SCI_EVENT_GROUP_PTR s_kjava_event = PNULL;
LOCAL BOOLEAN s_kjava_event_is_inited = FALSE;
LOCAL SCI_SEMAPHORE_PTR s_kjava_sem = PNULL, s_kjava_sem2 = PNULL;
LOCAL BOOLEAN s_kjava_sem_is_inited = FALSE;
BOOLEAN is_platform_request = FALSE;

typedef struct propty_locale_t
{
    MMISET_LANGUAGE_TYPE_E eng_id;
    char* locale;
} propty_locale_struct;

const propty_locale_struct propty_locale[]=
{
    {MMISET_LANGUAGE_ENGLISH, "en"},
    {MMISET_LANGUAGE_SIMP_CHINESE, "zh-cn"},  
    {MMISET_LANGUAGE_TRAD_CHINESE, "zh-tw"},
    {MMISET_LANGUAGE_ARABIC, "ar"},     
    {MMISET_LANGUAGE_FRENCH, "fr"},
    {MMISET_LANGUAGE_HINDI, "hi-IN"},  
    {MMISET_LANGUAGE_HUNGARIAN, "hu"},
    {MMISET_LANGUAGE_INDONESIAN, "id"}, 
    {MMISET_LANGUAGE_MALAY, "ms"},
    {MMISET_LANGUAGE_PORTUGUESE, "pt"},  
    {MMISET_LANGUAGE_RUSSIAN, "ru"},
    {MMISET_LANGUAGE_SPANISH, "es"},     
    {MMISET_LANGUAGE_TAGALOG, "tl"},
    {MMISET_LANGUAGE_THAI, "th"},  
    {MMISET_LANGUAGE_VIETNAMESE, "vi"},
    {MMISET_LANGUAGE_URDU, "ur"},  
    {MMISET_LANGUAGE_ITALIAN, "it"},
    {MMISET_LANGUAGE_PERSIAN, "en"},  
    {MMISET_LANGUAGE_TURKISH, "tr"},
    {MMISET_LANGUAGE_GERMAN, "de"},     
    {MMISET_LANGUAGE_GREEK, "el"},
    {MMISET_LANGUAGE_HEBREW, "he"},  
    {MMISET_LANGUAGE_BENGALI, "bn"},
    {MMISET_LANGUAGE_CZECH, "cs"}, 
    {MMISET_LANGUAGE_SLOVENIAN, "sl"},
    {MMISET_LANGUAGE_ROMANIAN, "ro"},
    
    {MMISET_LANGUAGE_TELUGU, "en"},
    {MMISET_LANGUAGE_MARATHI, "en"},
    {MMISET_LANGUAGE_TAMIL, "en"},
    {MMISET_LANGUAGE_GUJARATI, "en"},
    {MMISET_LANGUAGE_KANNADA, "en"},
    {MMISET_LANGUAGE_MALAYALAM, "en"},
    {MMISET_LANGUAGE_ORIYA, "en"},
    {MMISET_LANGUAGE_PUNJABI, "en"},
    {MMISET_LANGUAGE_AFRIKAANS, "en"},
    {MMISET_LANGUAGE_ALBANIAN, "en"},
    {MMISET_LANGUAGE_ARMENIAN, "en"},
    {MMISET_LANGUAGE_AZERBAIJANI, "en"},
    {MMISET_LANGUAGE_BASQUE, "en"},
    {MMISET_LANGUAGE_BULGARIAN, "en"},
    {MMISET_LANGUAGE_CATALAN, "en"},
    {MMISET_LANGUAGE_CROATIAN, "en"},
    {MMISET_LANGUAGE_DANISH, "en"},
    {MMISET_LANGUAGE_DUTCH, "en"},
    {MMISET_LANGUAGE_ESTONIAN, "en"},
    {MMISET_LANGUAGE_FILIPINO, "en"},
    {MMISET_LANGUAGE_FINNISH, "en"},
    {MMISET_LANGUAGE_GALICIAN, "en"},
    {MMISET_LANGUAGE_GEORGIAN, "en"},
    {MMISET_LANGUAGE_HAUSA, "en"},
    {MMISET_LANGUAGE_ICELANDIC, "en"},
    {MMISET_LANGUAGE_IGBO, "en"},
    {MMISET_LANGUAGE_IRISH, "en"},
    {MMISET_LANGUAGE_KAZAKH, "en"},
    {MMISET_LANGUAGE_LATVIAN, "en"},
    {MMISET_LANGUAGE_LITHUANIAN, "en"},
    {MMISET_LANGUAGE_MACEDONIAN, "en"},
    {MMISET_LANGUAGE_MOLDOVAN, "en"},
    {MMISET_LANGUAGE_NORWEGIAN, "en"},
    {MMISET_LANGUAGE_POLISH, "en"},
    {MMISET_LANGUAGE_SERBIAN, "en"},
    {MMISET_LANGUAGE_SESOTHO, "en"},
    {MMISET_LANGUAGE_SLOVAK, "en"},
    {MMISET_LANGUAGE_SWEDISH, "en"},
    {MMISET_LANGUAGE_UKRAINIAN, "en"},
    {MMISET_LANGUAGE_YORUBA, "en"},
    {MMISET_LANGUAGE_XHOSA, "en"},     
    {MMISET_LANGUAGE_ZULU, "en"},     
    {MMISET_LANGUAGE_ASSAMESE, "en"}, 
    {MMISET_LANGUAGE_SWAHILI, "en"},

    {MMISET_MAX_LANGUAGE, "en"}
};

#ifdef DebugForMM
static char defaultTz[7]="GMT+";
typedef struct confpropty_setting_struct_t
{
    char *key;
    char *value;
} confpropty_setting_struct;
/* System Configuration */
const confpropty_setting_struct m_property[] =
{
    /* DO NOT modify this property */
    {"microedition.configuration",                      "CLDC-1.1"},

    {"microedition.hostname",                           ""},
    /* DO NOT modify this property */
    {"microedition.profiles",                           "MIDP-2.1"},
//    {"microedition.locale",                             "zh-CN"},
    /* DO NOT modify this property */
    {"microedition.codebase",                           "j2me"},
    /* DO NOT modify this property */
    {"microedition.commports",                          "COM1"},//this is need
    {"com.sun.midp.io.j2me.comm.buffersize",            "256"},
    {"com.sun.midp.io.j2me.socket.buffersize",          "0"},
    {"SHIFT_JIS_InternalEncodingName",                  "SJIS"},
    {"X_SJIS_InternalEncodingName",                     "SJIS"},
    /* DO NOT modify this property */
    {"javax.microedition.io.Connector.protocolpath",    "com.sun.midp.io"},
    {"microedition.media.version",                      "1.2"},
    {"supports.mixing",                                 "false"},
    {"supports.audio.capture",                          "false"},
    //  {"audio.encodings",                                 "encoding=audio/wav encoding=audio/amr"},
#ifdef JAVA_SUPPORT_MEDIA_CAMERA    
    {"supports.video.capture", 							"true"},
    {"video.encodings", 								"encoding=video/3gp"},
    {"video.snapshot.encodings",						"encoding=jpeg"},
#else
    {"supports.video.capture", 							"false"},
#endif
	{"supports.recording", 								"false"},
	{"microedition.jtwi.version",						"1.0"},		/*for jsr185 specification*/		
	//{"microedition.m3g.version", 						"1.1"},
	{"com.sun.midp.io.j2me.http.untrusted", 			"UNTRUSTED/1.0"},
	{"microedition.platform", 						"Jblend/2.0.01"},//this is need
    {"file.separator",                                  "/"},
    {"microedition.io.file.FileConnection.version",     "1.0"},
    /* DO NOT modify the following settings of JSR82 */
    {"bluetooth.api.version",        "1.1"},
    {"obex.api.version",         "1.1"},
    {"bluetooth.l2cap.receiveMTU.max",   "339"},
    {"bluetooth.connected.devices.max",  "3"},
    {"bluetooth.connected.inquiry",      "true"},
    {"bluetooth.connected.page",     "true"},
    {"bluetooth.connected.inquiry.scan", "true"},
    {"bluetooth.connected.page.scan",    "true"},
    {"bluetooth.master.switch",      "true"},
    {"bluetooth.sd.trans.max",       "1"},
    {"bluetooth.sd.attr.retrievable.max",    "7"},
{"microedition.useragent", "HUAWEI T5050_TD/1.0 ThreadX_OS/04.00 Mocor/W10 Release/1.07.2011 Browser/NetFont-4.0 Profile/MIDP-2.1 Configuration/CLDC-1.1"},
	{NULL,NULL} /* please DO NOT remove the NULL terminator */
};


#else

static const char *m_property[] =
{
    "microedition.media.version", "1.2",
    //  "microedition.encoding",      "ISO-10646-1",
//    "microedition.locale",        "zh-CN",
    "supports.mixing",            "false",
    "supports.audio.capture",     "false",
#ifdef JAVA_SUPPORT_MEDIA_CAMERA    
        "supports.video.capture",     "true",
#else
        "supports.video.capture",     "false",
#endif
    "supports.recording",         "false",
    "microedition.commports",     "0",
    "microedition.platform",  "Jblend",
    "",                           "",
};
#endif

static const unsigned char permission_ok[]=
{
    0xE5,0xAE,0x8C,0xE6,0x88,0x90,0x00,0x00,0x00
};

static const unsigned char permission_cancel[]=
{
    0xE5,0x8F,0x96,0xE6,0xB6,0x88,0x00,0x00,0x00
};

static const unsigned char permission_blanket[]=
{
    0xE5 ,0x85 ,0x81 ,0xE8 ,0xAE ,0xB8 ,0xE4 ,0xB8 ,0x94 ,0xE4 ,0xB8 ,0x8D ,0xE5 ,0x86 ,0x8D ,0xE6,
    0x8F ,0x90 ,0xE7 ,0xA4 ,0xBA ,0x00 ,0x00 ,0x00
};

static const unsigned char permission_session[]=
{
    0xE6 ,0x9C ,0xAC ,0xE6 ,0xAC ,0xA1 ,0xE4 ,0xBC ,0x9A ,0xE8 ,0xAF ,0x9D ,0xE6 ,0x9C ,0x9F ,0xE5,
    0x85 ,0x81 ,0xE8 ,0xAE ,0xB8 ,0x00 ,0x00 ,0x00
};

static const unsigned char permission_sessionno[]=
{
    0xE6 ,0x9C ,0xAC ,0xE6 ,0xAC ,0xA1 ,0xE4 ,0xBC ,0x9A ,0xE8 ,0xAF ,0x9D ,0xE6 ,0x9C ,0x9F ,0xE4,
    0xB8, 0x8D, 0xE5, 0x85 ,0x81 ,0xE8 ,0xAE ,0xB8 ,0x00 ,0x00 ,0x00
};

static const unsigned char permission_oneshot[]=
{
    0xE6 ,0x9C ,0xAC ,0xE6 ,0xAC ,0xA1 ,0xE5 ,0x85 ,0x81 ,0xE8 ,0xAE ,0xB8 ,0xE4 ,0xB8 ,0x94 ,0xE4,
    0xB8 ,0x8B ,0xE6 ,0xAC ,0xA1 ,0xE5 ,0x86 ,0x8D ,0xE6 ,0x8F ,0x90 ,0xE7 ,0xA4 ,0xBA ,0x00 ,0x00,
    0x00
};

static const unsigned char permission_never[]=
{
    0xE4 ,0xB8 ,0x8D ,0xE5 ,0x85 ,0x81 ,0xE8 ,0xAE ,0xB8 ,0xE4 ,0xB8 ,0x94 ,0xE4 ,0xB8 ,0x8D ,0xE5,
    0x86 ,0x8D ,0xE6 ,0x8F ,0x90 ,0xE7 ,0xA4 ,0xBA ,0x00 ,0x00 ,0x00
};

static const unsigned char permission_notnow[]=
{
    0xE6 ,0x9C ,0xAC ,0xE6 ,0xAC ,0xA1 ,0xE4 ,0xB8 ,0x8D ,0xE5 ,0x85 ,0x81 ,0xE8 ,0xAE ,0xB8 ,0x00,
    0x00 ,0x00
};

static const unsigned char menu_string[9] =
{
    0xE8, 0x8F, 0x9C, 0xE5, 0x8D,
    0x95, 0x00, 0x00, 0x00
};

static const unsigned char done_string[9] =
{
    0xE5, 0xAE, 0x8C, 0xE6, 0x88,
    0x90, 0x00, 0x00, 0x00
};

static const unsigned char exit_string[9] =
{
    0xE9, 0x80, 0x80, 0xE5, 0x87,
    0xBA, 0x00, 0x00, 0x00
};

#ifdef DebugForMM
typedef struct _m_option_tag
{
    char *option;
    char *value;
} option;
char romName[3]="E:";
static option m_option[]=
{
    /* System */
    { "default.timezone"        ,       "Asia/Shanghai"         },
    { "language.direction"      ,       "0"             },

    /* GUI */
    /* The symbol of truncate indication when the wrap policy of list is OFF */
    { "truncate_indication_symbols",       "..."        },
    /*
    { "midp_selector_exit"      ,       "Exit"              },
    { "midp_alert_done"         ,       "Done"              },
    { "midp_command_menu"       ,       "Menu"              },
    { "midp_selector_launch_failed" ,   "Launching %n failed."      },
    { "midp_use_scrollbar"      ,       "1"             },
    { "midp_form_scrollbar_vpos"    ,       "1"             },
    { "midp_vertical_scroll"        ,   "12"                },
    { "midp_scrollbar_width"        ,   "0"             },
    { "midp_screen_background"      ,       "0x00FFFFFF"            },
    { "midp_screen_foreground"      ,       "0x00000000"            },
    { "midp_screen_linked_color"    ,       "0x000000FF"            },
    { "midp_form_inset_top"     ,   "0"             },
    { "midp_form_inset_left"        ,       "0"             },
    { "midp_form_inset_right"       ,       "0"             },
    { "midp_form_inset_bottom"      ,       "0"             },
    */
    /* Permission*/
    /*
    { "midp20_permissionOk"     ,       "OK"                        },
    { "midp20_permissionCancel"     ,       "Cancel"                    },
    { "midp20_permissionBlanket"    ,   "Allow and don't ask again" },
    { "midp20_permissionSession"    ,       "Allow for this session"    },
    { "midp20_permissionSessionNo"  ,       "No for this session"       },
    { "midp20_permissionOneshot"    ,       "Allow and ask me next time"},
    { "midp20_permissionNotNow"     ,   "Not now"                   },
    { "midp20_permissionNever"      ,       "Never"                     },
    */
    //"midp20_permissionUseNativeDialog",   "1"
    /*   {"default.timezone",                     "Asia/Shanghai"},
        {   "midp_selector_exit",                   (const char*)exit_string},
          { "midp_alert_done",                      (const char*)done_string},
        {   "midp_command_menu",                    (const char*)menu_string},
         {  "midp_selector_launch_failed",          "Launching n failed."},
         {  "midp20_permissionOk",                  (const char*)permission_ok},
          { "midp20_permissionCancel",              (const char*)permission_cancel},
          { "midp20_permissionBlanket",             (const char*)permission_blanket},
          { "midp20_permissionSession",             (const char*)permission_session},
          { "midp20_permissionOneshot",             (const char*)permission_oneshot},
          { "midp20_permissionNotNow",              (const char*)permission_notnow},
          { "midp20_permissionNever",               (const char*)permission_never},
          { "midp20_permissionSessionNo",           (const char*)permission_sessionno},
          { "rms_quota_size_unspecified",           "450000"},//数字为想要设置的RMS最大值
           {"fc_support_writable"   ,               "1"},
           {"fc_support_readable",              "1"},
           {"fc_support_hidden",                    "0"},
           {"fc_support_dir_writable",              "1"},
           {"fc_support_dir_readable",          "1"},
           {"fc_support_dir_hidden",                "0"},
    */

    //  { "midp20_permissionUseNativeDialog",   "1"                         },
    /*
    { "midp20_permissionDialog_background", "0x00FFFFFF"            },
    { "midp20_permissionDialog_foreground", "0x00000000"            },

    { "midp20_permissionNetAccess"  ,       "Use network?"              },
    { "midp20_permissionMessaging"  ,       "Use messaging?"            },
    { "midp20_permissionShareRecordstore",  "Share recordstore?"        },
    { "midp20_permissionAutoInvocation" ,   "Use auto invocation?"      },
    { "midp20_permissionLocalConnectivity", "Use local connectivity?"   },
    { "midp20_permissionRecording"  ,       "Use recording?"            },
    { "midp20_permissionDefault"    ,       "Allow?"                    },
    */
    /*
    { "midp20_permission_allow:javax.microedion.io.Connector.file.read", "1"},
    */

    /*
    { "midp_enhance_create_cropped_image",  "0"             },
    { "midp_enhance_flush_game_graphics",   "0"             },
    { "midp_game_flush_compatible_wt_wtk",  "0"             },
    { "midp_command_actsWithPressed"    ,   "1"             },
    */
    /* JTWI */
    /*
    { "jtwi.supported"          ,   "1"             },
    { "jtwi.gsm.security.required"  ,   "1"             },
    */
    /* File Connection */
    //XXX,TBD
    //  { "fc_rom_root_name"        ,   romName    },
    { "fc_support_writable"     ,   "1"             },
    { "fc_support_readable"       ,   "1"             },
    { "fc_support_hidden"       ,   "1"             },
    { "fc_support_dir_writable"     ,   "1"             },
    { "fc_support_dir_readable"       ,   "1"             },
    { "fc_support_dir_hidden"       ,   "1"             },
    /*

    //XXX,TBD
    { "midp20_maxConnectionCount"   ,   "10"                },
    { "midp20_maxHttpConnectionCount"   ,   "10"                },
    { "midp20_maxSocketConnectionCount" ,   "10"                },
    { "midp20_maxSecureConnectionCount" ,   "10"                },
    */
    /* RMS */

    { "rms_quota_size_unspecified"  ,   "4500000"                },
    { "rms_quota_size_manufacturer" ,   "3174400"                },
    { "rms_quota_size_operator"     ,   "3174400"                },
    { "rms_quota_size_third_party"  ,   "3174400"                },

    /* BG VM Resource Management */
    { "bgvm.platform.request"   ,   "false"             },
    { "bgvm.platform.request.call"  ,   "false"             },
    { "bgvm.platform.request.wap_browser"   ,   "false"     },
    { "bgvm.platform.request.video_player"      ,   "false" },

    { "bgvm.wma.sms"    ,   "false"             },
    { "bgvm.wma.mms"    ,   "false"             },
    { "bgvm.multimedia" ,   "false"             },
    { "midp21.supported",   "1"                 },
    { "pim_use_native_parser",      "0"  },
    { NULL              ,   NULL                },
};

static const char *m_option2[] =
{
    //"midp20_permissionUseNativeDialog",   "1"
    "default.timezone",                   "Asia/Shanghai" ,/// "Asia/Shanghai",
    "midp_selector_exit", (const char *) exit_string,
    "midp_alert_done", (const char *) done_string,
    "midp_command_menu", (const char *) menu_string,
    "midp_selector_launch_failed",          "Launching n failed.",
    "midp20_permissionOk", (const char *) permission_ok,
    "midp20_permissionCancel", (const char *) permission_cancel,
    "midp20_permissionBlanket", (const char *) permission_blanket,
    "midp20_permissionSession", (const char *) permission_session,
    "midp20_permissionOneshot", (const char *) permission_oneshot,
    "midp20_permissionNotNow", (const char *) permission_notnow,
    "midp20_permissionNever", (const char *) permission_never,
    "midp20_permissionSessionNo", (const char *) permission_sessionno,
    "rms_quota_size_unspecified",   "4500000" ,//数字为想要设置的RMS最大值
    "rms_quota_size_manufacturer" ,   "3174400" ,
    "rms_quota_size_operator"     ,   "3174400",
    "rms_quota_size_third_party"  ,   "3174400" ,
    "fc_support_writable"   ,               "1",
    "fc_support_readable",              "1",
    "fc_support_hidden",                    "1",
    "fc_support_dir_writable",              "1",
    "fc_support_dir_readable",          "1",
    "fc_support_dir_hidden",                "1",
    "",                                     "",
};

#else
static const char *m_option[] =
{
    //"midp20_permissionUseNativeDialog",   "1"
    "default.timezone",                     "Asia/Shanghai",
    "midp_selector_exit", (const char *) exit_string,
    "midp_alert_done", (const char *) done_string,
    "midp_command_menu", (const char *) menu_string,
    "midp_selector_launch_failed",          "Launching n failed.",
    "midp20_permissionOk", (const char *) permission_ok,
    "midp20_permissionCancel", (const char *) permission_cancel,
    "midp20_permissionBlanket", (const char *) permission_blanket,
    "midp20_permissionSession", (const char *) permission_session,
    "midp20_permissionOneshot", (const char *) permission_oneshot,
    "midp20_permissionNotNow", (const char *) permission_notnow,
    "midp20_permissionNever", (const char *) permission_never,
    "midp20_permissionSessionNo", (const char *) permission_sessionno,
    "rms_quota_size_unspecified",           "450000",//数字为想要设置的RMS最大值
    "fc_support_writable"   ,               "1",
    "fc_support_readable",              "1",
    "fc_support_hidden",                    "0",
    "fc_support_dir_writable",              "1",
    "fc_support_dir_readable",          "1",
    "fc_support_dir_hidden",                "0",
    "",                                     "",
};
#endif
MMIJAVA_WAP_INFO    g_java_wap_info = {0};
uint8              *g_java_call_info_ptr = PNULL;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
#ifdef CMCC_JAVA_V41_SUPPORT
int current_running_midlet_domain=0;
extern void jbNativeMethod_raiseException (const char *exceptionClassName);
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
char gstrIMEI[20];
char gstrIMSI[20];

LOCAL void ConvertBcdToDigitalStr (uint8 length, uint8 *bcd_ptr, uint8 *digital_ptr)
{
    int32         i;
    uint8         temp;

    // get the first digital
    temp = (uint8) ( (*bcd_ptr >> 4) &0x0f);

    if (temp >= 0x0a)
    {
        *digital_ptr = (uint8) ( (temp - 0x0a) + 'A');
    }
    else
    {
        *digital_ptr = (uint8) (temp + '0');
    }

    bcd_ptr++;
    digital_ptr++;

    for (i=0; i< (length - 1); i++)
    {
        temp = *bcd_ptr;
        temp &= 0x0f;

        if (temp >= 0x0a)
        {
            *digital_ptr = (uint8) ( (temp - 0x0a) + 'A');
        }
        else
        {
            *digital_ptr = (uint8) (temp + '0');
        }

        digital_ptr++;
        temp = *bcd_ptr;
        temp = (uint8) ( (temp & 0xf0) >> 4);

        if ( (temp == 0x0f) && (i == (length -1)))
        {
            *digital_ptr = '\0';
            return;
        }
        else if (temp>=0x0a)
        {
            *digital_ptr = (uint8) ( (temp - 0x0a) + 'A');
        }
        else
        {
            *digital_ptr = (uint8) (temp + '0');
        }

        digital_ptr++;
        bcd_ptr++;
    }

    *digital_ptr = '\0';
}

static const char *getIMEI()
{
    int ret;
    uint8               temp_str[20];
    MN_IMEI_T           imei_arr;

    SCI_MEMSET (gstrIMEI, 0, sizeof (gstrIMEI));
#ifdef CMCC_JAVA_V41_SUPPORT

    if (current_running_midlet_domain!= AJMS_PCS_DOMAIN_OPERATOR)
    {
        jbNativeMethod_raiseException ("java/lang/SecurityException");
        return 0;
    }

#endif
#ifdef _USE_OLD_L4_API
    ret = MNNV_GetIMEI (imei_arr);
#else
    ret = MNNV_GetIMEIEx (MMIAPISET_GetActiveSim(), imei_arr);
#endif
    ConvertBcdToDigitalStr (MN_MAX_IMEI_LENGTH, imei_arr, temp_str);
    sprintf ( (char *) gstrIMEI, "%s",temp_str);

    if (!ret)
    {
        return 0;
    }

    JWE_LOG (getIMSI, ("gstrIMEI=%s",gstrIMEI));
    return gstrIMEI;
}

static const char *getIMSI()
{
    // int i;
    uint8              temp_str[20];
    MN_IMSI_T   imsi_str;

    SCI_MEMSET (gstrIMSI, 0, sizeof (gstrIMSI));
#ifdef CMCC_JAVA_V41_SUPPORT

    if (current_running_midlet_domain!=AJMS_PCS_DOMAIN_OPERATOR)
    {
        jbNativeMethod_raiseException ("java/lang/SecurityException");
        return 0;
    }

#endif

#ifdef _USE_OLD_L4_API
    imsi_str = MNSIM_GetImsi();
#else
    imsi_str = MNSIM_GetImsiEx (MMIAPISET_GetActiveSim());
#endif

    if (MN_MAX_IMSI_ARR_LEN >= imsi_str.imsi_len)
    {
        ConvertBcdToDigitalStr (imsi_str.imsi_len, imsi_str.imsi_val, temp_str);
        sprintf ( (char *) gstrIMSI, "%s", temp_str);
        //ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else
    {
        return 0;
    }

    JWE_LOG (getIMSI, ("gstrIMSI=%s",gstrIMSI));

    return gstrIMSI;
}

//***********************************************************************************************
//  Description:init time
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void time_init (void);
//***********************************************************************************************
//  Description:finial time
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void time_fini (void);
//***********************************************************************************************
//  Description:init semaphore
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void sem_init (void);
//***********************************************************************************************
//  Description:finial semaphore
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void sem_fini (void);
//***********************************************************************************************
//  Description:init event
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void event_init (void);
//***********************************************************************************************
//  Description:finial event
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void event_fini (void);
/*****************************************************************************/
//  Description :CB_Start_RequestConn
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CB_Start_RequestConn (const char *para);


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
//***********************************************************************************************
//  Description:Initializes the CORE subsystem. This function is called in the KVE thread context.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void kjava_core_init (void)
{
    JWE_LOG (kjava_core_init, ("[kjava_core_init] enter."))
    time_init();
    event_init();
    sem_init();
    g_java_wap_info.is_start_wap = FALSE;
    g_java_wap_info.url  = PNULL;
}

//***********************************************************************************************
//  Description:Finalizes the CORE subsystem. This function is called in the KVE thread context.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void kjava_core_fini (void)
{
    JWE_LOG (kjava_core_fini, ("[kjava_core_fini] enter."))
    sem_fini();
    event_fini();
    time_fini();

}

//***********************************************************************************************
//  Description:Allocates two huge memory blocks.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_core_allocateHugeMemory (KJavaHugeMemory *kjavaHugeMemory_p,int actionType)
{
    JWE_LOG (kjava_core_allocateHugeMemory, ("[kjava_core_allocateHugeMemory] (kjavaHugeMemory_p) = (0x%08X)", kjavaHugeMemory_p));


    if (kjavaHugeMemory_p == SCI_NULL)
    {
        JWE_LOG (kjava_core_allocateHugeMemory, ("[kjava_allocateHugeMemory] kjavaHugeMemory_p == NULL"));
        return 0;
    }

    SCI_MEMSET (kjavaHugeMemory_p, 0, sizeof (KJavaHugeMemory));
    BLKMEM_GarbageClean();
    UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);


    if (MMIAPIFMM_IsFmmMainWinOpen() ||MMK_IsOpenWin (MMIJAVA_CHECK_WIN_ID))
    {
        kjavaHugeMemory_p->secondBlock = MMI_BL_MALLOC( (BLOCK_MEM_ID_E) BLOCK_MEM_POOL_JAVA);
    }
    else
    {
        kjavaHugeMemory_p->secondBlock = MMI_BL_MALLOC( (BLOCK_MEM_ID_E) BLOCK_MEM_POOL_JAVA);
    }

    if (kjavaHugeMemory_p->secondBlock== SCI_NULL)
    {
        JWE_LOG (kjava_core_allocateHugeMemory, ("[kjava_allocateHugeMemory] failed to allocate secondBlock"));
        UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);
        return 0;
    }
    else
    {

        if (actionType == KJAVA_ACTION_TYPE_AJMS_MEM || actionType== KJAVA_ACTION_TYPE_TCK)
        {
            kjavaHugeMemory_p->secondBlockSize= KJAVA_SECOND_BLOC_SIZE_FOR_INSTALL;
        }
        else
        {
            kjavaHugeMemory_p->secondBlockSize= KJAVA_SECOND_BLOCK_SIZE;
        }
    }

    kjavaHugeMemory_p->firstBlock = kjavaHugeMemory_p->secondBlock + kjavaHugeMemory_p->secondBlockSize;

    if (MMIAPIFMM_IsFmmMainWinOpen() ||MMK_IsOpenWin (MMIJAVA_CHECK_WIN_ID))
    {
        kjavaHugeMemory_p->firstBlockSize= (int) BL_GetSize ( (BLOCK_MEM_ID_E) BLOCK_MEM_POOL_JAVA) - kjavaHugeMemory_p->secondBlockSize;
    }
    else
    {
        kjavaHugeMemory_p->firstBlockSize= (int) BL_GetSize ( (BLOCK_MEM_ID_E) BLOCK_MEM_POOL_JAVA) - kjavaHugeMemory_p->secondBlockSize;
    }

    JWE_LOG (kjava_core_allocateHugeMemory, ("kjava_core_allocateHugeMemory -- allocate memory success!"));
    //SCI_TRACE_LOW:"first=%d,second =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_762_112_2_18_2_23_11_353,(uint8*)"dd",kjavaHugeMemory_p->firstBlockSize,kjavaHugeMemory_p->secondBlockSize);
    //SCI_TRACE_LOW:"kjava_core_allocateHugeMemory-------allocate memory size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_763_112_2_18_2_23_11_354,(uint8*)"d",kjavaHugeMemory_p->firstBlockSize+kjavaHugeMemory_p->secondBlockSize);
    UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);

    return 1;
}

//***********************************************************************************************
//  Description:Releases the allocated memory blocks
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void kjava_core_freeHugeMemory (KJavaHugeMemory *kjavaHugeMemory_p,int actionType)
{
    BLOCK_MEM_STATUS_E staus = 0;
    JWE_LOG (kjava_core_freeHugeMemory, ("[kjava_core_freeHugeMemory] (kjavaHugeMemory_p) = (0x%08X)", kjavaHugeMemory_p));

    //SCI_TRACE_LOW:"FREE ENTER"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_778_112_2_18_2_23_11_355,(uint8*)"");
    /*
    staus = BL_GetStatus(BLOCK_MEM_POOL_JAVA_INSTALL);
    //SCI_TRACE_LOW:"kjava_core_freeHugeMemory status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_781_112_2_18_2_23_11_356,(uint8*)"d",staus);
    if ((NOT_USE != staus)&&(ALLOC_BY_OTHERS != staus))
    {
        MMI_BL_FREE(BLOCK_MEM_POOL_JAVA_INSTALL);
    }
    */
    staus = BL_GetStatus ( (BLOCK_MEM_ID_E) BLOCK_MEM_POOL_JAVA);
    //SCI_TRACE_LOW:"kjava_core_freeHugeMemory2 status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_788_112_2_18_2_23_11_357,(uint8*)"d",staus);

    if ( (NOT_USE != staus) && (ALLOC_BY_OTHERS != staus))
    {
        MMI_BL_FREE ( (BLOCK_MEM_ID_E) BLOCK_MEM_POOL_JAVA);
    }





    //SCI_TRACE_LOW:"FREE OK"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_799_112_2_18_2_23_11_358,(uint8*)"");
}


/*****************************************************************************/
//  Description : Gets a system property to be returned by the java.lang.System class  getProperty method.
//  Global resource dependence :
//  Author:
//  Note: Please note developer can implement the function without any codes  because JSC has default values.  (just returns NULL).
/*****************************************************************************/
const char *kjava_core_getProperty (const char *key)
{
    // char *storekey = PNULL, *property = PNULL;
    int32  index = 0;
    MMISET_LANGUAGE_TYPE_E  language_type;

    JWE_LOG (kjava_core_getProperty, ("(key)=(%s)",key))
    //SCI_TRACE_LOW:"kjava_core_getProperty (key)=(%s)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_814_112_2_18_2_23_11_359,(uint8*)"s",key);

    if (strcmp (key,"IMEI") == 0)
    {
        return getIMEI();
    }
    else if (strcmp (key,"IMSI") == 0)
    {
        return getIMSI();
    }
    else if (strcmp (key, "fileconn.dir.memory_card") == 0)
    {
        return "file:///memory_card";
    }
    else if (strcmp(key, "microedition.locale") == 0)
    {
        MMIAPISET_GetLanguageType(&language_type);
        return propty_locale[language_type].locale;
    }

#ifdef DebugForMM

    while (m_property[index].key!=NULL)
    {
        if (strcmp (m_property[index].key, key) ==0)
        {
            //SCI_TRACE_LOW:"kjava_core_getProperty return property.value=(%s)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_840_112_2_18_2_23_11_360,(uint8*)"s",m_property[index].value);
            return m_property[index].value;
        }

        index++;
    }

#else
    storekey = m_property[index++];
    property = m_property[index++];

    while (strlen (storekey) > 0)
    {
        if (strcmp (storekey, key) == 0)
        {
            //SCI_TRACE_LOW:"kjava_core_getProperty return property=(%s)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_855_112_2_18_2_23_11_361,(uint8*)"s",property);
            return property;
        }

        storekey = m_property[index++];
        property = m_property[index++];
    }

#endif
    //SCI_TRACE_LOW:"kjava_core_getProperty return null"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_864_112_2_18_2_23_11_362,(uint8*)"");
    return (char *) NULL; /* In the default */

}


#ifdef DebugForMM
static void itoa_1 (int i, char *string)
{
    int power, j ;

    j=i;

    for (power=1; j >= 10; j/=10)
    {
        power*=10;
    }

    for (; power > 0 ; power/=10)
    {
        *string++ = '0'+i/power;
        i%=power;
    }

    *string = '\0';
    JWE_LOG (itoa_1, ("return string = %s.", string))
    //SCI_TRACE_LOW:"return string = %s."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_890_112_2_18_2_23_11_363,(uint8*)"s", string);

}

const char *kjava_core_getOption (const char *key)
{
    int                     i=0;
    char                    offsetStr[3] = {0};
    int                     offset = -1;
    const char             *storekey = PNULL;
    const char             *option_local = PNULL;
    int32                   index = 0;
    MMISET_LANGUAGE_TYPE_E  java_language_type = 0;

    JWE_LOG (kjava_core_getOption, ("(key)=(%s)", key))


    MMIAPISET_GetLanguageType (&java_language_type);

    JWE_LOG (kjava_core_getOption, ("hanker java_language_type = %d", java_language_type))

    if (g_runningTckLevel)
    {
        if (strstr ( (char *) key, "midp20_permission_allow:"))
        {
            return "1";
        }
    }

    if (java_language_type == MMISET_LANGUAGE_SIMP_CHINESE)
    {
        storekey = m_option2[index++];
        option_local = m_option2[index++];
        JWE_LOG (kjava_core_getOption, ("hanker storekey = %s;option=%s", storekey,option_local))

        while (strlen (storekey) > 0)
        {
            if (strcmp (storekey, key) == 0)
            {
                return option_local;
            }

            storekey = m_option2[index++];
            option_local = m_option2[index++];
        }
    }


    while (m_option[i].option!=NULL)
    {
        if (strcmp (m_option[i].option , key) ==0)
        {
            JWE_LOG (kjava_core_getOption, ("(m_option[i].option)=(%s)", m_option[i].option));

            if (strcmp ("default.timezone",key) ==0)
            {
                //SCI_TRACE_LOW:"[kjava_core_getOption]22 (m_option[%d].option)=(%s)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_946_112_2_18_2_23_11_364,(uint8*)"ds",i, m_option[i].option);

#ifdef WORLD_CLOCK_SUPPORT
                offset = (int) MMIAPIACC_GetLocalTimeZone(); //8;// (int)jvm_get_timezone();
#endif

                if (offset < 0)
                {
                    defaultTz[3] = '-';
                    offset = -offset;       // get the absolute value
                }
                else
                {
                    defaultTz[3] = '+';
                }

                itoa_1 (offset, offsetStr);
                JWE_LOG (kjava_core_getOption, ("timezone offset = %s.", offsetStr))
                * (defaultTz+4) ='\0';
                strcat (defaultTz,offsetStr);
                JWE_LOG (kjava_core_getOption, ("defaultTz=%s.", defaultTz))
                return defaultTz;
            }
            else
            {
                JWE_LOG (kjava_core_getOption, ("(m_option[i].option)=(%s)", m_option[i].option))
                JWE_LOG (kjava_core_getOption, ("(m_option[i].value)=(%s)", m_option[i].value))

                return m_option[i].value;
            }
        }

        i++;

    }

    JWE_LOG (kjava_core_getOption, ("return Null"))
    return (char *) NULL; //default

}

#else

/*****************************************************************************/
//  Description : Gets a JBlend customization property.
//  Global resource dependence :
//  Author:
//  Note: Please note developer can implement the function without any codes  because JSC has default values.  (just returns NULL).
/*****************************************************************************/
const char *kjava_core_getOption (const char *key)
{
    char *storekey = PNULL, *option_local = PNULL;
    int32  index = 0;
    MMISET_LANGUAGE_TYPE_E java_language_type = 0;
    JWE_LOG (kjava_core_getOption, ("key = %s", key))

    MMIAPISET_GetLanguageType (&java_language_type);

    JWE_LOG (kjava_core_getOption, ("hanker java_language_type = %d", MMISET_LANGUAGE_SIMP_CHINESE))

    if (java_language_type == MMISET_LANGUAGE_SIMP_CHINESE)
    {
        storekey = m_option[index++];
        option_local = m_option[index++];

        JWE_LOG (kjava_core_getOption, ("storekey = %s;option=%s", storekey,option))

        while (strlen (storekey) > 0)
        {
            if (strcmp (storekey, key) == 0)
            {
                return option_local;
            }

            storekey = m_option[index++];
            option_local = m_option[index++];
        }
    }

    if (g_runningTckLevel)
    {
        if (strstr ( (char *) key, "midp20_permission_allow:"))
        {
            return "1";
        }
    }

    JWE_LOG (kjava_core_getOption, ("kjava_core_getOption null"));

    return (char *) NULL; /* In the default */
}
#endif
/*****************************************************************************/
//  Description : Requests a connection to the designated URL.
//  Global resource dependence :
//  Author:
//  Note: Please note developer can implement the function without any codes  because JSC has default values.  (just returns NULL).
/*****************************************************************************/
/**
* Requests a connection to the designated URL.
*
* Execute a native application for making connection to the URL designated
* in the url parameter. This function corresponds to the
* javax.microedition.midlet.MIDlet class platformRequest method. For
* details of processing for the URL given in the url parameter, see the
* explanations of the javax.microedition.midlet.MIDlet class platformRequest
* method in the MIDP 2.0 specification.
*
* \param url   The destination URL.
*
* \return
*                  One of the following is designated.
*                  <ul>
*                      <li>KJAVA_EXTAPP_UNSUPPORTED_CONNECTION</li>
*                      <li>KJAVA_EXTAPP_CONNECT_IMMEDIATE</li>
*                      <li>KJAVA_EXTAPP_CONNECT_LATER</li>
*                  </ul>
*/
int kjava_core_requestConnection (const char *url)
{
    int32 ret = 0;
    //int32 i = 0;

    JWE_LOG (kjava_core_requestConnection, ("[kjava_core_requestConnection] (url) = (0x%08X)", url));

    if (! (strncmp ("tel:", url, 4)
            && strncmp ("tel://", url, 6)
#ifdef JAVA_OTA_SUPPORT
            &&strncmp ("http://", url, 7)
            &&strncmp ("https://", url, 8)
#endif
          )

       )
    {
        // ret = ajms_arc_terminateApp();
        CB_Start_RequestConn (url);

        ret = KJAVA_EXTAPP_CONNECT_IMMEDIATE;//KJAVA_EXTAPP_CONNECT_LATER;
    }
    else if (*url == '\0')
    {
        ret = KJAVA_EXTAPP_CONNECT_IMMEDIATE;
    }
    else
    {
        ret = KJAVA_EXTAPP_UNSUPPORTED_CONNECTION;
    }

    return ret;
}

/*****************************************************************************/
//  Description :CB_Start_RequestConn
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CB_Start_RequestConn (const char *para)
{

    char                   *pszTel = PNULL;
    uint16                  para_len =0;
    PLATFORMREQUEST_TYPE    type = MMI_PLATFORM_REQUEST_MAX;

    JWE_LOG (CB_Start_RequestConn, ("[CB_Start_RequestConn] para: %s", (char *) para));

    if (!strncmp ("tel://", (char *) para, 6))
    {
        pszTel = (char *) (para+6);
        para_len = strlen (para+6);

        if (PNULL != g_java_call_info_ptr)
        {
            //SCI_TRACE_LOW:"CB_Start_RequestConn call error, prev call has't ended"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_1114_112_2_18_2_23_12_365,(uint8*)"");
            return;
        }

        g_java_call_info_ptr= SCI_ALLOC_APP (para_len + 1);
        SCI_MEMSET (g_java_call_info_ptr,0, (para_len + 1));
        SCI_MEMCPY (g_java_call_info_ptr,para+6,para_len);

        is_platform_request = TRUE;
        type = MMI_PLATFORM_REQUEST_TEL;
        MMIAPIJAVA_SendSignal (MSG_JAVA_PLATFORM_REQUEST,type);
        //MMIAPICC_MakeCall(MMIJAVA_GetAcitveSim(),(uint8*)pszTel,strlen(pszTel),PNULL,CC_CALL_SIM_MAX,CC_CALL_NORMAL_CALL,PNULL);
        //SCI_TRACE_LOW:"CALL OK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_1126_112_2_18_2_23_12_366,(uint8*)"");
    }
    else if (!strncmp ("tel:", (char *) para, 4))
    {
        pszTel = (char *) (para+4);
        para_len = strlen (para+4);

        if (PNULL != g_java_call_info_ptr)
        {
            //SCI_TRACE_LOW:"CB_Start_RequestConn call error, prev call has't ended"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_1135_112_2_18_2_23_12_367,(uint8*)"");
            return;
        }

        g_java_call_info_ptr= SCI_ALLOC_APP (para_len + 1);
        SCI_MEMSET (g_java_call_info_ptr,0, (para_len + 1));
        SCI_MEMCPY (g_java_call_info_ptr,para+4,para_len);

        is_platform_request = TRUE;
        type = MMI_PLATFORM_REQUEST_TEL;
        MMIAPIJAVA_SendSignal (MSG_JAVA_PLATFORM_REQUEST,type);
        //MMIAPICC_MakeCall(MMIJAVA_GetAcitveSim(),(uint8*)pszTel,strlen(pszTel),PNULL,CC_CALL_SIM_MAX,CC_CALL_NORMAL_CALL,PNULL);
        //SCI_TRACE_LOW:"CALL OK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_1147_112_2_18_2_23_12_368,(uint8*)"");
    }
    else if (!strncmp ("http://", (char *) para, 7))
    {
        //SCI_TRACE_LOW:"HTTP ENTER"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_1151_112_2_18_2_23_12_369,(uint8*)"");
        g_java_wap_info.is_start_wap = TRUE;
        para_len = strlen (para);
        g_java_wap_info.url = SCI_ALLOC_APP (para_len + 1);
        SCI_MEMSET (g_java_wap_info.url,0, (para_len + 1));
        SCI_MEMCPY (g_java_wap_info.url,para,para_len);
        is_platform_request = TRUE;

        type = MMI_PLATFORM_REQUEST_HTTP;
        MMIAPIJAVA_SendSignal (MSG_JAVA_PLATFORM_REQUEST,type);

        //SCI_TRACE_LOW:"CB_Start_RequestConn para = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_1162_112_2_18_2_23_12_370,(uint8*)"s",para);
        //SCI_TRACE_LOW:"CB_Start_RequestConn g_java_wap_info.url = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_1163_112_2_18_2_23_12_371,(uint8*)"s",g_java_wap_info.url);
        //SCI_TRACE_LOW:"HTTP OK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_CORE_1164_112_2_18_2_23_12_372,(uint8*)"");
    }
    else if (!strncmp ("https://", (char *) para, 8))
    {
        /* as the wap don't realize SSL connetion,
        * so https request for futher realize!
         */
        JWE_LOG (CB_Start_RequestConn, ("[CB_Start_RequestConn] https request"));
    }
    else
    {
        JWE_LOG (CB_Start_RequestConn, ("[CB_Start_RequestConn] donothing for else request"));
    }

}

//***********************************************************************************************
//  Description:Waits for an event for a certain period.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_core_waitEvent (unsigned int msec)
{
    uint32 status = 0;
    uint32 event_value = 0;

#ifdef JAVA_LOW_POWER_ENABLE 
	if(!MMIDEFAULT_GetBackLightState())
	{
		msec=msec<2000?2000:msec;//过滤掉2秒内的event，防止频繁心跳影响功耗
#ifndef WIN32
//        MNGPRS_SendRabRelMsgEx();
#endif
    	if(s_kjava_event_is_inited)
    	{       
        
        	status = SCI_GetEvent(s_kjava_event, 0x01, SCI_OR_CLEAR, &event_value, msec);
        
        	if(status == SCI_SUCCESS)
        	{
            
            	JWE_LOG(kjava_core_waitEvent,("KJAVA_EVENT_OK s_kjava_event = %d",s_kjava_event))
               	return KJAVA_EVENT_OK;
        	}
        	else
        	{
            
            	JWE_LOG(kjava_core_waitEvent,("KJAVA_EVENT_TIMEOUT"))
            	return KJAVA_EVENT_TIMEOUT;
        	}
    	}
	}
#else
   //  JWE_LOG(kjava_core_waitEvent,("(msec)=(%d)",msec))
    if (s_kjava_event_is_inited)
    {

        status = SCI_GetEvent (s_kjava_event, 0x01, SCI_OR_CLEAR, &event_value, msec);

        if (status == SCI_SUCCESS)
        {

            //         JWE_LOG(kjava_core_waitEvent,("KJAVA_EVENT_OK s_kjava_event = %d",s_kjava_event))
            return KJAVA_EVENT_OK;
        }
        else
        {

            //         JWE_LOG(kjava_core_waitEvent,("KJAVA_EVENT_TIMEOUT"))
            return KJAVA_EVENT_TIMEOUT;
        }
    }
#endif
    return KJAVA_EVENT_TIMEOUT;
}

//***********************************************************************************************
//  Description:Sets an event
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void kjava_core_notifyEvent (void)
{
    uint32 status = 0;

    //  JWE_LOG(kjava_core_notifyEvent, ("Enter"))
    if (s_kjava_event_is_inited)
    {
        status = SCI_SetEvent (s_kjava_event, 0x01, SCI_OR);
        //     JWE_LOG(kjava_core_notifyEvent, ("stasus %d s_kjava_event = %d", status,s_kjava_event))
    }

}

//***********************************************************************************************
//  Description:Resets the event.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void kjava_core_clearEvent (void)
{
    uint32 status = 0;//huangjc modify at 2007.6.25

    //  JWE_LOG(kjava_core_clearEvent, ("Enter"))
    status =SCI_SetEvent (s_kjava_event, 0x00, SCI_AND); //huangjc modify at 2007.6.25

    return;
}

//***********************************************************************************************
//  Description:Gets a semaphore. This function is used by JBlend for mutual exclusion  control.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void kjava_core_setSemaphore (int sem_id)
{
    //  JWE_LOG(kjava_core_setSemaphore, ("kjava_core_setSemaphore -- enter!"));
    if (s_kjava_sem_is_inited)
    {
        if (sem_id == 1)
        {
            SCI_GetSemaphore (s_kjava_sem, SCI_WAIT_FOREVER);
        }
        else if (sem_id == 2)
        {
            SCI_GetSemaphore (s_kjava_sem2, SCI_WAIT_FOREVER);
        }

    }
}

//***********************************************************************************************
//  Description:Releases a semaphore. This function is used by JBlend for mutual exclusion control.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void kjava_core_releaseSemaphore (int sem_id)
{
    //   JWE_LOG(kjava_core_releaseSemaphore, ("kjava_core_releaseSemaphore -- enter!"));

    if (s_kjava_sem_is_inited)
    {
        if (sem_id == 1)
        {
            SCI_PutSemaphore (s_kjava_sem);
        }
        else if (sem_id == 2)
        {
            SCI_PutSemaphore (s_kjava_sem2);
        }

    }

}

/**
* Gets the elapsed time.
*
* Store the elapsed time (monotonic increasing value) in millisecond as
* a 64-bit value to the arguments high and low. It is recommended that
* the elapsed time starts from 0. The JVM presupposes that the value
* obtained by this function increases monotonically.
*
* In case of JVM suspension, the elapsed time may stop increasing.
* However, when JVM restarts, the elapsed time needs to be updated as
* if it were continuously increased during the JVM suspension.
*
* The JVM controls the thread using the value returned by this function.
* Since the accuracy of the elapsed time influences the accuracy of JVM
* execution directly, this value should be acquired with the highest
* possible accuracy. It is recommended that the accuracy ranges within
* 10 milliseconds.
*
* \param high  A pointer to the memory area for storing the higher 32 bits.
* \param low   A pointer to the memory area for storing the lower 32 bits.
*/
void kjava_core_getCurrentTick (unsigned long *high, unsigned long *low)
{
    uint32 cur_tick = SCI_GetTickCount();

    //   JWE_LOG(kjava_core_getCurrentTick, ("kjava_core_getCurrentTick -- cur_tick = %d", cur_tick));
    *high = 0;
    *low = cur_tick - s_begin_tick;
    //  JWE_LOG(kjava_core_getCurrentTick, ("kjava_core_getCurrentTick -- leave!"));
}


/**
* Gets the current time.
*
* Store the current time (the elapsed time from 00:00:00.000, January 1, 1970
* of Greenwich mean time (GMT)) in millisecond as a 64-bit value to the
* arguments high and low. It is not required to compensate for summer time
* (daylight saving time).
*
* Put 0, when the current time of the system has not been set. In case the
* current time of the system is changed during JVM suspension, put the
* changed value after JVM is resumed.
*
* Since the accuracy of the current time acquired by this function
* influences the accuracy of the value obtained by
* java.lang.System.currentTimeMills method, this value should be acquired
* with the highest possible accuracy. It is recommended that the resolution
* ranges within 100 milliseconds.
*
* \param high  A pointer to the memory area for storing the higher 32 bits.
* \param low   A pointer to the memory area for storing the lower 32 bits.
*/
void kjava_core_getCurrentTime (unsigned long *high, unsigned long *low)
{

    int64_t cur_millisecond = 0;


    //    cur_millisecond = (int64_t)CmsGetCurrentTime();
    cur_millisecond = (int64_t) MMIJAVA_GetCurTime();
    JWE_LOG (kjava_core_getCurrentTime, ("MMIAPICOM_GetCurTime = %ld",cur_millisecond));
    cur_millisecond = cur_millisecond - 3600 * 8;
    cur_millisecond *= 1000;

    *high = (unsigned long) (cur_millisecond >> 32);
    *low  = (unsigned long) (cur_millisecond);
    //  JWE_LOG(kjava_core_getCurrentTime, ("kjava_core_getCurrentTime -- high = %08x, low =%08x",  *high, *low));
    //  SCI_TRACE_LOW("kjava_core_getCurrentTime -- high = %08x, low =%08x",  *high, *low);

}

//***********************************************************************************************
//  Description:init semaphore
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void sem_init (void)
{
    //   JWE_LOG(sem_init, ("Enter"))
    if (!s_kjava_sem_is_inited)
    {
        s_kjava_sem = SCI_CreateSemaphore (kjava_sem_name, 1);
        s_kjava_sem2 = SCI_CreateSemaphore (kjava_sem2_name, 1);
        s_kjava_sem_is_inited = TRUE;

    }

}

//***********************************************************************************************
//  Description:finial semaphore
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void sem_fini (void)
{
    //  JWE_LOG(sem_fini, ("Enter"))
    if (s_kjava_sem_is_inited)
    {
        SCI_DeleteSemaphore (s_kjava_sem);
        SCI_DeleteSemaphore (s_kjava_sem2);
        s_kjava_sem_is_inited = FALSE;
    }

}

//***********************************************************************************************
//  Description:init time
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void time_init()
{
    // JWE_LOG(time_init, ("Enter"))
    if (!s_kjava_time_is_inited)
    {
        s_begin_tick = SCI_GetTickCount();
        s_kjava_time_is_inited = TRUE;
    }

    return;
}

//***********************************************************************************************
//  Description:finial time
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void time_fini()
{
    s_kjava_time_is_inited = FALSE;
    //  JWE_LOG(time_fini, ("Enter"))
}


//***********************************************************************************************
//  Description:init event
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void event_init()
{
    // JWE_LOG(event_init, ("Enter"))
    if (!s_kjava_event_is_inited)
    {
        s_kjava_event = SCI_CreateEvent (java_event_name);

        s_kjava_event_is_inited = TRUE;
    }
}

//***********************************************************************************************
//  Description:finial event
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void event_fini()
{
    //  JWE_LOG(event_fini, ("Enter"))
    if (s_kjava_event_is_inited)
    {
        SCI_DeleteEvent (s_kjava_event);
        s_kjava_event_is_inited = FALSE;
    }
}
#endif
