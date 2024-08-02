
#ifdef DPHONE_SUPPORT
#define MMISET_RING_USER_MAX_NUM              10
#elif defined(MMI_LOW_MEMORY_RING)
#define MMISET_RING_USER_MAX_NUM          4//    8
#define MMISET_RING_USER_MAX_NUM_EX       4///    9
#else
#define MMISET_RING_USER_MAX_NUM         4//     14
#endif
#define MMISET_ALERT_RING_MAX_NUM_EX          2
#define MMISET_ALERT_RING_MAX_NUM             2
#ifdef MMI_LOW_MEMORY_RING
#define MMISET_MSGRING_USER_MAX_NUM          2
#define MMISET_MMSRING_USER_MAX_NUM          2
#else
#define MMISET_MSGRING_USER_MAX_NUM          6
#define MMISET_MMSRING_USER_MAX_NUM          3
#endif

//[alarm ring tone UI and function]
#define MMIALM_CLOCK_RING_MAX_NUM            2
#define MMIALM_CLOCK_RING_MAX_NUM_MAT        1
//[end]

RES_SET_IMG_CMP_DEFAULT(PNG,IMG_CMP_PNG_ARGB,0)
RES_SET_IMG_CMP_DEFAULT(JPG,IMG_CMP_JPG_SCI,0)
RES_SET_IMG_CMP_DEFAULT(GIF,IMG_CMP_GIF_SCI,0)
RES_SET_IMG_CMP_DEFAULT(BMP_8BIT,IMG_CMP_BMP_8_SCI,0)
RES_SET_IMG_CMP_DEFAULT(BMP_16BIT,IMG_CMP_BMP_16_565_RLE,0)
RES_SET_IMG_CMP_DEFAULT(BMP_24BIT,IMG_CMP_BMP_16_565_RLE,0)
RES_SET_IMG_CMP_DEFAULT(BMP_32BIT,IMG_CMP_BMP_32_ARGB,0)

#ifdef IM_ENGLISH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_ENGLISH, "english-gb")//英文
#endif

#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
RES_ADD_LANGUAGE(LANGUAGE_SIMP_CHINESE,"chinese-cn")//简体中文
#endif

#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
RES_ADD_LANGUAGE(LANGUAGE_TRAD_CHINESE,"chinese-tw")//繁体中文
#endif

#ifdef IM_TRAD_CHINESE_HK_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_TRAD_CHINESE_HK,"chinese-hk")//繁体中文
#endif

#ifdef IM_ARABIC_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_ARABIC, "arabic")//阿拉伯语
#endif
#ifdef IM_FRENCH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_FRENCH, "french-fr")//法语
#endif
#ifdef IM_HINDI_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_HINDI, "hindi")//印地语
#endif
#ifdef IM_HUNGARIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_HUNGARIAN, "hungarian")//匈牙利语
#endif
#ifdef IM_INDONESIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_INDONESIAN, "indonesian")//印度尼西亚语
#endif
#ifdef IM_MALAY_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_MALAY, "malay")//马来语
#endif
#ifdef IM_PORTUGUESE_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_PORTUGUESE, "portuguese-pt")//葡萄牙语
#endif
#ifdef IM_RUSSIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_RUSSIAN, "russian")//俄语
#endif
#ifdef IM_SPANISH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_SPANISH, "spanish-es")//西班牙语
#endif
#ifdef IM_TAGALOG_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_TAGALOG, "tagalog")//他加诺语
#endif
#ifdef IM_THAI_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_THAI, "thai")//泰语
#endif
#ifdef IM_VIETNAMESE_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_VIETNAMESE, "vietnamese")//越南语
#endif
#ifdef IM_URDU_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_URDU, "urdu")//乌尔都语
#endif
#ifdef IM_ITALIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_ITALIAN, "italian")//意大利语
#endif
#ifdef IM_PERSIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_FARSI, "persian-fa")//波斯语
#endif
#ifdef IM_TURKISH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_TURKISH, "turkish")//土耳其语
#endif
#ifdef IM_GERMAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_GERMAN, "german")//德语
#endif
#ifdef IM_GREEK_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_GREEK, "greek")//希腊语
#endif
#ifdef IM_HEBREW_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_HEBREW, "hebrew")//希伯来语
#endif
#ifdef IM_BENGALI_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_BENGALI, "Bengali")//孟加拉语
#endif
#ifdef IM_CZECH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_CZECH, "czech")//捷克语
#endif
#ifdef IM_SLOVENIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_SLOVENIAN, "slovenian")//斯诺文尼亚语
#endif
#ifdef IM_ROMANIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_ROMANIAN, "romanian")//罗马尼亚语
#endif
#ifdef IM_TELUGU_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_TELUGU, "telugu")	//泰卢固语
#endif
#ifdef IM_MARATHI_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_MARATHI, "marathi")	//马拉地
#endif
#ifdef IM_TAMIL_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_TAMIL, "tamil")	//泰米尔语
#endif
#ifdef IM_GUJARATI_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_GUJARATI, "gujarati")		//古吉阿特语
#endif
#ifdef IM_KANNADA_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_KANNADA, "kannada")		//卡纳达语
#endif
#ifdef IM_MALAYALAM_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_MALAYALAM, "malayalam")		//马拉亚拉姆语
#endif
#ifdef IM_ORIYA_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_ORIYA, "oriya")		//奥里亚语
#endif
#ifdef IM_PUNJABI_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_PUNJABI, "punjabi")		//旁遮普语
#endif
#ifdef IM_AFRIKAANS_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_AFRIKAANS, "afrikaans")		//南非的公用荷兰语
#endif
#ifdef IM_ALBANIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_ALBANIAN, "albanian")		//阿尔巴尼亚语
#endif
#ifdef IM_ARMENIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_ARMENIAN, "armenian")		//亚美尼亚语
#endif
#ifdef IM_AZERBAIJANI_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_AZERBAIJANI, "azerbaijani")		//阿塞拜疆语
#endif
#ifdef IM_BASQUE_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_BASQUE, "Basque")		//巴斯克语
#endif
#ifdef IM_BULGARIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_BULGARIAN, "bulgarian")//保加利亚语
#endif
#ifdef IM_CATALAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_CATALAN, "Catalan")		//加泰罗尼亚语
#endif
#ifdef IM_CROATIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_CROATIAN, "croatian")//克罗地亚语
#endif
#ifdef IM_DANISH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_DANISH, "danish")//丹麦语
#endif
#ifdef IM_DUTCH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_DUTCH, "dutch")//荷兰语
#endif
#ifdef IM_ESTONIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_ESTONIAN, "estonian")		//爱沙尼亚语
#endif
#ifdef IM_FILIPINO_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_FILIPINO, "Filipino")		//菲律宾语
#endif
#ifdef IM_FINNISH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_FINNISH, "finnish")
#endif
#ifdef IM_GALICIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_GALICIAN, "Calician")		//加利西亚语
#endif
#ifdef IM_GEORGIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_GEORGIAN, "georgian")		//格鲁吉亚语
#endif
#ifdef IM_HAUSA_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_HAUSA, "hausa")		//豪萨语
#endif
#ifdef IM_ICELANDIC_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_ICELANDIC, "icelandic")		//冰岛语
#endif
#ifdef IM_IGBO_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_IGBO, "igbo")		//伊博语
#endif
#ifdef IM_IRISH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_IRISH, "Irish")		//爱尔兰语
#endif
#ifdef IM_KAZAKH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_KAZAKH, "kazakh")		//哈萨克斯坦语
#endif
#ifdef IM_LATVIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_LATVIAN, "latvian")		//拉脱维亚语
#endif
#ifdef IM_LITHUANIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_LITHUANIAN, "lithuanian") //立陶宛语
#endif
#ifdef IM_MACEDONIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_MACEDONIAN, "macedonian")		//马其顿语
#endif
#ifdef IM_MOLDOVAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_MOLDOVAN, "Moldovan")		//摩尔多瓦语
#endif
#ifdef IM_NORWEGIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_NORWEGIAN, "norwegian")//挪威语
#endif
#ifdef IM_POLISH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_POLISH, "polish")//波兰语
#endif
#ifdef IM_SERBIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_SERBIAN, "serbian")		//塞尔维亚语
#endif
#ifdef IM_SESOTHO_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_SESOTHO, "sesotho")		//塞索托语
#endif
#ifdef IM_SLOVAK_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_SLOVAK, "slovak")//斯洛伐克语
#endif
#ifdef IM_SWEDISH_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_SWEDISH, "swedish")//瑞典语
#endif
#ifdef IM_UKRAINIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_UKRAINIAN, "ukrainian")//乌克兰语
#endif
#ifdef IM_YORUBA_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_YORUBA, "yoruba")		//约鲁巴语
#endif
#ifdef IM_XHOSA_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_XHOSA, "xhosa")//科萨语
#endif
#ifdef IM_ZULU_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_ZULU, "zulu")//祖鲁语
#endif
#ifdef IM_ASSAMESE_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_ASSAMESE, "assamese")//印度语支的阿萨姆语
#endif
#ifdef IM_SWAHILI_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_SWAHILI, "swahili")//斯瓦西里语
#endif

#ifdef IM_MYANMAR_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_MYANMAR, "Myanmar")  //缅甸语
#endif

#ifdef IM_AMHARIC_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_AMHARIC, "amharic")  //阿姆哈拉语
#endif
#ifdef IM_KHMER_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_KHMER, "khmer")  //柬埔寨语
#endif
#ifdef IM_LAO_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_LAO, "Lao")  //老挝语
#endif
#ifdef IM_UYGHUR_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_UYGHUR, "Uyghur")  //维吾尔语
#endif
#ifdef IM_TIBETAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_TIBETAN, "Tibetan")  //西藏语
#endif
#ifdef IM_SINHALESE_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_SINHALA, "sinhala")  //僧伽罗语
#endif
#ifdef IM_BOSNIAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_BOSNIAN, "bosnian")  //波斯尼亚
#endif
#ifdef IM_AMERICAN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_AMERICAN, "english-gb_english-us")  //英语美国
#endif
#ifdef IM_PORTUGUESE_BR_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_PORTUGUESE_BR, "portuguese-br")  //葡萄牙语-巴西
#endif
#ifdef IM_TURKMEN_LATIN_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_TURKMEN_LATIN, "turkmen_latin")  //土库曼-拉丁
#endif
#ifdef IM_SPANISH_AM_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_SPANISH_AM, "spanish-am")  //西班牙-AM
#endif

#ifdef IM_BELARUS_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_BELARUS, "belarusian")  //belarus
#endif

#ifdef IM_LINGALA_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_LINGALA, "lingala")  //lingala
#endif

#ifdef IM_MALAGASY_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_MALAGASY, "malagasy")  //malagasy
#endif

#ifdef IM_PASHTO_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_PASHTO, "pushto")  //pashto 
#endif

#ifdef IM_UZBEK_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_UZBEK, "uzbek")  //uzbek
#endif

#ifdef IM_BENGALI_BD_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_BENGALI, "bengali-bd")//bengali-bd
#endif
#ifdef IM_TEST_SUPPORT
RES_ADD_LANGUAGE(LANGUAGE_TEST, "Test")  //Test
#endif

//只有中英文的情况下使用basic字库
#if ( !defined IM_ARABIC_SUPPORT &&!defined IM_FRENCH_SUPPORT &&!defined IM_HINDI_SUPPORT &&!defined IM_HUNGARIAN_SUPPORT \
    &&!defined IM_INDONESIAN_SUPPORT &&!defined IM_MALAY_SUPPORT &&!defined IM_PORTUGUESE_SUPPORT &&!defined IM_RUSSIAN_SUPPORT \
    &&!defined IM_SPANISH_SUPPORT &&!defined IM_TAGALOG_SUPPORT &&!defined IM_THAI_SUPPORT &&!defined IM_VIETNAMESE_SUPPORT \
    &&!defined IM_URDU_SUPPORT &&!defined IM_ITALIAN_SUPPORT &&!defined IM_PERSIAN_SUPPORT &&!defined IM_TURKISH_SUPPORT \
    &&!defined IM_GERMAN_SUPPORT &&!defined IM_GREEK_SUPPORT &&!defined IM_ROMANIAN_SUPPORT &&!defined IM_SLOVENIAN_SUPPORT \
    &&!defined IM_HEBREW_SUPPORT &&!defined IM_BENGALI_SUPPORT &&!defined IM_CZECH_SUPPORT &&!defined IM_TELUGU_SUPPORT \
    &&!defined IM_MARATHI_SUPPORT &&!defined IM_TAMIL_SUPPORT &&!defined IM_GUJARATI_SUPPORT &&!defined IM_KANNADA_SUPPORT \
    &&!defined IM_MALAYALAM_SUPPORT &&!defined IM_ORIYA_SUPPORT &&!defined IM_PUNJABI_SUPPORT &&!defined IM_AFRIKAANS_SUPPORT \
    &&!defined IM_ALBANIAN_SUPPORT &&!defined IM_ARMENIAN_SUPPORT &&!defined IM_AZERBAIJANI_SUPPORT &&!defined IM_BASQUE_SUPPORT \
    &&!defined IM_BULGARIAN_SUPPORT &&!defined IM_CATALAN_SUPPORT &&!defined IM_CROATIAN_SUPPORT &&!defined IM_DANISH_SUPPORT \
    &&!defined IM_DUTCH_SUPPORT &&!defined IM_ESTONIAN_SUPPORT &&!defined IM_FILIPINO_SUPPORT &&!defined IM_FINNISH_SUPPORT \
    &&!defined IM_GALICIAN_SUPPORT &&!defined IM_GEORGIAN_SUPPORT &&!defined IM_HAUSA_SUPPORT &&!defined IM_ICELANDIC_SUPPORT \
    &&!defined IM_IGBO_SUPPORT &&!defined IM_IRISH_SUPPORT &&!defined IM_KAZAKH_SUPPORT &&!defined IM_LATVIAN_SUPPORT \
    &&!defined IM_LITHUANIAN_SUPPORT &&!defined IM_MACEDONIAN_SUPPORT &&!defined IM_MOLDOVAN_SUPPORT &&!defined IM_NORWEGIAN_SUPPORT \
    &&!defined IM_POLISH_SUPPORT &&!defined IM_SERBIAN_SUPPORT &&!defined IM_SESOTHO_SUPPORT &&!defined IM_SLOVAK_SUPPORT \
    &&!defined IM_SWEDISH_SUPPORT &&!defined IM_UKRAINIAN_SUPPORT &&!defined IM_YORUBA_SUPPORT &&!defined IM_XHOSA_SUPPORT \
    &&!defined IM_ZULU_SUPPORT &&!defined IM_ASSAMESE_SUPPORT &&!defined IM_SWAHILI_SUPPORT && !defined IM_MYANMAR_SUPPORT \
    &&!defined IM_SINHALESE_SUPPORT &&!defined IM_TIBETAN_SUPPORT \
    &&!defined IM_BELARUS_SUPPORT &&!defined IM_LINGALA_SUPPORT &&!defined IM_MALAGASY_SUPPORT &&!defined IM_PASHTO_SUPPORT &&!defined IM_UZBEK_SUPPORT \
    &&!defined IM_BENGALI_BD_SUPPORT &&!defined IM_PORTUGUESE_BR_SUPPORT && !defined IM_SPANISH_AM_SUPPORT)
#define MMI_FONT_LATIN_BASIC
#endif
#ifdef  NANDBOOT_SUPPORT
//#undef MMI_FONT_LATIN_BASIC
#endif
RES_SET_VERSION("1.0.0")
RES_SET_MAX_MODULE_NUM(128)
//RES_SET_IMG_GLOBAL_COMPRESS_TYPE(IMG_CMP_BMP,75)
//RES_SET_ANIM_GLOBAL_COMPRESS_TYPE(ANIM_CMP_MBMP,75)
RES_SET_IMG_DEFAULT_COMPRESS_TYPE(IMG_CMP_BMP,75)
RES_SET_ANIM_DEFAULT_COMPRESS_TYPE(ANIM_CMP_MBMP,75)
RES_SET_ANIM_GLOBAL_KEY_COLOR(0x000000)
//RES_SET_GLOBAL_JPEG_COMPRESS_RATE(75)
RES_SET_MAX_RING_SIZE(10485760) //10 * 1024 * 1024

#ifdef MMI_RES_ORIENT_PORTRAIT

#ifdef MULTI_THEME_SUPPORT
RES_ADD_SKIN("THEME1","1.0.0",0)
#endif

#elif MMI_RES_ORIENT_LANDSCAPE

#ifdef MULTI_THEME_SUPPORT
RES_ADD_SKIN("THEME1_H","1.0.0",0)
#endif

#elif MMI_RES_ORIENT_BOTH

RES_ADD_SKIN("DEFAULT_H","1.0.0",0)

#ifdef MULTI_THEME_SUPPORT
RES_ADD_SKIN("THEME1","1.0.0",0)
RES_ADD_SKIN("THEME1_H","1.0.0",0)
#endif

#endif

RES_ADD_MODULE(MMI_MODULE_COMMON,"\\common\\common_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_FONT,"font")
RES_ADD_MODULE(MMI_MODULE_IM,"\\im\\im_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_TP,"\\tp\\tp_mdu_def.h")
#ifdef BROWSER_SUPPORT
RES_ADD_MODULE(MMI_MODULE_MMIBROWSER_MANAGER,"\\browser_manager\\browser_manager_mdu_def.h")
#endif
#if defined(BROWSER_SUPPORT_DORADO) || defined(OPERA_MINI_SUPPORT)
RES_ADD_MODULE(MMI_MODULE_MMIBROWSER,"\\browser\\browser_mdu_def.h")
#endif
#ifdef OAUTH_SUPPORT
RES_ADD_MODULE(MMI_MODULE_NET_PROXY,"\\netproxy\\netproxy_mdu_def.h")
#endif
RES_ADD_MODULE(MMI_MODULE_MMS,"\\mms\\mms_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_SMS,"\\sms\\sms_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_FM,"\\fm\\fm_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_EBOOK,"\\ebook\\ebook_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_MULTIM,"\\multim\\multim_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_GAME_MANAGER,"\\game_manager\\game_manager_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_SET,"\\set\\set_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_MP3,"\\audioplayer\\audioplayer_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_CC,"\\cc\\cc_mdu_def.h")
#ifdef MMI_REFACTOR_SAMPLE
RES_ADD_MODULE(MMI_MODULE_APPSAMPLE,"\\appsample\\appsample_mdu_def.h")
#endif
RES_ADD_MODULE(MMI_MODULE_USSD,"\\ussd\\ussd_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_PB,"\\pb\\pb_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_STK,"\\stk\\stk_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_ENG,"\\eng\\eng_mdu_def.h")
#ifdef BROWSER_SUPPORT_NETFRONT
RES_ADD_MODULE(MMI_MODULE_WWW,"\\www\\www_mdu_def.h")
#endif

#ifdef DL_SUPPORT
RES_ADD_MODULE(MMI_MODULE_DL,"\\dl\\dl_mdu_def.h")
#endif

#ifdef APP_PRODUCT_DM
RES_ADD_MODULE(MMI_MODULE_ENGTD,"\\engtd\\engtd_mdu_def.h")
#endif
RES_ADD_MODULE(MMI_MODULE_RECORD,"\\record\\record_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_ACC,"\\tools\\tools_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_ALARM,"\\clock\\clock_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_BT,"\\bt\\bt_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_GAME_LINK,"\\game_link\\game_link_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_GAME_BOXMAN,"\\game_boxman\\game_boxman_mdu_def.h")
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
RES_ADD_MODULE(MMI_MODULE_GAME_BUBBLEBOBBLE,"\\game_bubblebobble\\game_bubblebobble_mdu_def.h")
#endif
#ifdef GAME_GOLDMINER_SUPPORT
RES_ADD_MODULE(MMI_MODULE_GAME_GOLDMINER,"\\game_goldminer\\game_goldminer_mdu_def.h")
#endif
#ifdef GAME_SNAKE_SUPPORT
RES_ADD_MODULE(MMI_MODULE_GAME_SNAKE,"\\game_snake\\game_snake_mdu_def.h")
#endif
RES_ADD_MODULE(MMI_MODULE_GAME_QUINTET,"\\game_quintet\\game_quintet_mdu_def.h")

//michael wang
RES_ADD_MODULE(MMI_MODULE_CONNECTION,"\\connection\\connection_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_FMM,"\\files_manager\\files_manager_mdu_def.h")

#if defined(DRM_SUPPORT)
RES_ADD_MODULE(MMI_MODULE_DRM,"\\drm\\drm_mdu_def.h")
#endif

#ifdef OCR_SUPPORT
RES_ADD_MODULE(MMI_MODULE_OCR,"\\ocr\\ocr_mdu_def.h")
#endif
#ifdef PRODUCT_DM
RES_ADD_MODULE(MMI_MODULE_VT,"\\vt\\vt_mdu_def.h")
#endif
#ifdef DCD_SUPPORT
RES_ADD_MODULE(MMI_MODULE_DCD,"\\dcd\\dcd_mdu_def.h")
#endif
#ifdef CMMB_SUPPORT
RES_ADD_MODULE(MMI_MODULE_MTV,"\\mobiletv\\mobiletv_mdu_def.h")
#endif
#ifdef ATV_SUPPORT
RES_ADD_MODULE(MMI_MODULE_ATV,"\\atv\\atv_mdu_def.h")
#endif
#ifdef GPS_SUPPORT
RES_ADD_MODULE(MMI_MODULE_GPS,"\\gps\\gps_mdu_def.h")
#endif
RES_ADD_MODULE(MMI_MODULE_CAMERA,"\\camera\\camera_mdu_def.h")
#ifdef KURO_SUPPORT
RES_ADD_MODULE(MMI_MODULE_KURO,"\\kuro\\kuro_mdu_def.h")
#endif
#ifdef MMI_PIC_CROP_SUPPORT
RES_ADD_MODULE(MMI_MODULE_PIC_CROP,"\\pic_crop\\pic_crop_mdu_def.h")
#endif
RES_ADD_MODULE(MMI_MODULE_PIC,"\\pic\\pic_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_PIC_VIEWER,"\\pic_viewer\\pic_viewer_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_DYNA_APPLIST,"\\dyna\\dyna_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_SAMPLE,"\\sample\\sample_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_OTA,"\\otapush\\otapush_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_PIM,"\\pim\\pim_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_CLIPBRD,"\\clipbrd\\clipbrd_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_SEARCH,"\\search\\search_mdu_def.h")

#ifdef NPS_SUPPORT
RES_ADD_MODULE(MMI_MODULE_NPS,"\\nps\\nps_mdu_def.h")
#endif

#if defined(BROWSER_SUPPORT_WEBKIT)
RES_ADD_MODULE(MMI_MODULE_WK,"\\webkit\\webkit_mdu_def.h")
#endif

#ifdef MMIUZONE_SUPPORT
RES_ADD_MODULE(MMI_MODULE_UZONE,"\\uzone\\uzone_mdu_def.h")
#endif

#ifdef CMCC_UI_STYLE
RES_ADD_MODULE(MMI_MODULE_CMCS,"\\cs\\cs_mdu_def.h")
#endif
#ifdef ASP_SUPPORT
RES_ADD_MODULE(MMI_MODULE_ASP,"\\asp\\asp_mdu_def.h")
#endif

#ifdef QQ_SUPPORT
RES_ADD_MODULE(MMI_MODULE_QQ,"\\qq\\qq_mdu_def.h")
#endif

#ifdef PUSH_EMAIL_SUPPORT
RES_ADD_MODULE(MMI_MODULE_PUSHMAIL,"\\push_mail\\push_mail_mdu_def.h")
#endif
//============== MET MEX start ===================
#ifdef MET_MEX_SUPPORT
RES_ADD_MODULE(MMI_MODULE_MET_MEX,"\\met_mex\\met_mex_mdu_def.h")
#endif // MET_MEX_SUPPORT
//============== MET MEX end ===================

#ifdef VIDEO_PLAYER_SUPPORT
RES_ADD_MODULE(MMI_MODULE_VP,"\\videoplayer\\videoplayer_mdu_def.h")
#endif

#ifdef STREAMING_SUPPORT
#ifdef MOBILE_VIDEO_SUPPORT
RES_ADD_MODULE(MMI_MODULE_MV,"\\mobile_video\\mobile_video_mdu_def.h")
#endif
#endif

#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT) || (defined QBTHEME_SUPPORT) || (defined WEATHER_SUPPORT)
RES_ADD_MODULE(MMI_MODULE_WIDGET,"\\widget\\widget_mdu_def.h")
#endif

#ifdef JAVA_SUPPORT
RES_ADD_MODULE(MMI_MODULE_JAVA,"\\java\\java_mdu_def.h")
#endif

#ifdef MRAPP_SUPPORT
RES_ADD_MODULE(MMI_MODULE_MRAPP,"\\mrapp\\mrapp_mdu_def.h")
#endif
#ifdef HERO_ENGINE_SUPPORT
RES_ADD_MODULE(MMI_MODULE_HEROENGINE,"\\heroengine\\heroengine_mdu_def.h")
RES_ADD_MODULE(MMI_MODULE_HEROAPP,"\\heroapp\\heroapp_mdu_def.h")
#endif
RES_ADD_MODULE(MMI_MODULE_WALLPAPER,"\\wallpaper\\wallpaper_mdu_def.h")

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
RES_ADD_MODULE(MMI_MODULE_LIVEWALLPAPER,"\\livewallpaper\\livewallpaper_mdu_def.h")
#endif
#ifdef MCARE_V31_SUPPORT
RES_ADD_MODULE(MMI_MODULE_MCARE_MINI,"\\TencentMcareV31\\TencentMcareV31_mdu_def.h")
#endif

#ifdef  SALES_SUPPORT
RES_ADD_MODULE(MMI_MODULE_SALES,"\\sales\\sales_mdu_def.h")
#endif

#ifdef WEATHER_SUPPORT
RES_ADD_MODULE(MMI_MODULE_WEATHER,"\\weather\\weather_mdu_def.h")
#endif

#ifdef PDA_UI_DROPDOWN_WIN
//#ifdef MMI_PDA_SUPPORT
RES_ADD_MODULE(MMI_MODULE_DROPDOWNWIN,"\\dropdownwin\\dropdownwin_mdu_def.h")
#endif

#ifdef QBTHEME_SUPPORT
RES_ADD_MODULE(MMI_MODULE_QBTHEME,"\\qbtheme\\qbtheme_mdu_def.h")
#endif

#ifdef WRE_SUPPORT
RES_ADD_MODULE(MMI_MODULE_WRE,"\\wre_boot\\wre_boot_mdu_def.h")
#endif

#ifdef IKEYBACKUP_SUPPORT
RES_ADD_MODULE(MMI_MODULE_IKEYBACKUP,"\\ikeybackup\\ikeybackup_mdu_def.h")
#endif

#ifdef MMI_KING_MOVIE_SUPPORT
RES_ADD_MODULE(MMI_MODULE_KMOVIE,"\\kmovie\\kmovie_mdu_def.h")
#endif
#ifdef MMI_MEMO_SUPPORT
RES_ADD_MODULE(MMI_MODULE_MEMO,"\\memo\\memo_mdu_def.h")
#endif

#ifdef CSC_SUPPORT
RES_ADD_MODULE(MMI_MODULE_CUS, "\\customer\\customer_mdu_def.h")
#endif

#ifdef  FOTA_SUPPORT
RES_ADD_MODULE(MMI_MODULE_FOTA,"\\fota\\fota_mdu_def.h")
#endif

//[[define ring infomation

//  not define NEW_RESOURCE_SUPPORT
#ifdef MMI_INDIAN_SOS_SUPPORT
RES_ADD_RING(R_SOS_ALERT_RING,"\\RING\\R_SOS_ALERT.mp3")
#endif
#ifdef MMI_RES_LOW_COST
RES_ADD_RING_RANGE(R_CALL, MMISET_RING_USER_MAX_NUM, "\\RING\\R_CALL_lowcost_1.mid")
RES_ADD_RING_RANGE(R_MMS, MMISET_MMSRING_USER_MAX_NUM, "\\RING\\R_MMS_lowcost_1.mid")
RES_ADD_RING(R_SMS_1, "\\RING\\SIMP_R_SMS_1.mid")
#else
#ifdef MMI_CALL_RING_MIDI_TYPE
RES_ADD_RING_RANGE(R_CALL, 14, "\\RING\\R_CALL_1.mid")
#else
#ifdef ZDT_WDL_DZ_SUPPORT
      RES_ADD_RING(R_CALL_1,"\\RING\\R_CALL_1_DZ.mp3")
      RES_ADD_RING(R_CALL_2,"\\RING\\R_CALL_2.mp3")
#else
RES_ADD_RING_RANGE(R_CALL, MMISET_RING_USER_MAX_NUM, "\\RING\\R_CALL_1.mp3")
#endif
#endif

RES_ADD_RING_RANGE(R_ALERT, 5, "\\RING\\R_ALERT_1.mid")
RES_ADD_RING_RANGE(R_SMS, MMISET_MSGRING_USER_MAX_NUM, "\\RING\\R_SMS_1.mid")

RES_ADD_RING_RANGE(R_CLOCK, MMIALM_CLOCK_RING_MAX_NUM, "\\RING\\R_CLOCK_1.mid")

RES_ADD_RING_RANGE(R_OPER_CALL, 15, "\\RING\\R_OPER_CALL_1.mp3")

#ifdef ZDT_WDL_Z62_SUPPORT
RES_ADD_RING(R_FIXED_MP3_1,"\\RING\\R_CALL_1_HYL.mp3")//add by James li
#else
RES_ADD_RING(R_FIXED_MP3_1,"\\RING\\R_FIXED_MP3.mp3")//add by James li
#endif
#ifdef SOS_KEYLONGWEB
RES_ADD_RING(R_SOS_ALARM,"\\RING\\R_SOS_ALARM.amr")
#endif

#ifdef SXH_APPLE_SUPPORT
RES_ADD_MODULE(MMI_MODULE_SXH_APPLE,"\\apple\\apple_mdu_def.h")
#endif
#ifdef CMCC_UI_STYLE
RES_ADD_RING(R_CMCC_POWER_ON,"\\RING\\R_CMCC_POWER_ON.mid")
RES_ADD_RING(R_CMCC_POWER_OFF,"\\RING\\R_CMCC_POWER_OFF.mid")
#endif
//wuxx add power ring mp3
RES_ADD_RING(R_POWER_MP3_1,"\\RING\\R_POWER_ON.mp3")

#if 0//(defined MMI_RES_SLIM_SUPPORT) && (defined IM_SIMP_CHINESE_SUPPORT) && (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
//RES_ADD_RING(R_POWER_1,"\\RING\\SIMP_R_POWER_1.mid")
RES_ADD_RING(R_POWER_1,"\\RING\\Startup.mid")
RES_ADD_RING_TAG(R_POWER_1,"\\RING\\Startup.mid")
//#else
//RES_ADD_RING(R_POWER_1,"\\RING\\R_POWER_1.mid")
RES_ADD_RING(R_POWER_1,"\\RING\\Startup.mid")
RES_ADD_RING_TAG(R_POWER_1,"\\RING\\Startup.mid")
#endif
//RES_ADD_RING(R_POWER_1,"\\RING\\R_POWER_ON.mp3")
//RES_ADD_RING_TAG(R_POWER_1,"\\RING\\R_POWER_ON.mp3")
//RES_ADD_RING(R_POWER_2,"\\RING\\R_POWER_2.mid")

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
RES_ADD_RING(R_FLIP_1,"\\RING\\R_FLIP_1.WAV")
RES_ADD_RING(R_FLIP_2,"\\RING\\R_FLIP_2.WAV")
#endif

#ifdef LCD_SLIDE_SUPPORT
RES_ADD_RING(R_SLIDE_1,"\\RING\\R_SLIDE_1.WAV")
RES_ADD_RING(R_SLIDE_2,"\\RING\\R_SLIDE_2.WAV")
#endif
#ifndef MMI_LOW_MEMORY_RING
RES_ADD_RING(R_KEY_1,"\\RING\\R_KEY_1.WAV")
RES_ADD_RING(R_KEY_LOUND_1,"\\RING\\R_KEY_LOUND_1.WAV")
#endif
#ifdef MMI_RES_LOW_COST
RES_ADD_RING(R_KEY_2,"\\RING\\R_KEY_lowcost_1.WAV")
RES_ADD_RING(R_KEY_LOUND_2,"\\RING\\R_KEY_LOUND_lowcost_2.WAV")
RES_ADD_RING(R_DC_COUNT,"\\RING\\R_DC_COUNT_lowcost.wav")
RES_ADD_RING(R_TP_KEYLOCK,"\\RING\\R_TP_KEYLOCK_lowcost.amr")
RES_ADD_RING(R_DL_1,"\\RING\\R_DL_lowcost_1.mid")
#else
RES_ADD_RING(R_KEY_LOUND_2,"\\RING\\SIMP_R_KEY_LOUND_2.WAV")
RES_ADD_RING(R_KEY_2,"\\RING\\SIMP_R_KEY_2.WAV")
RES_ADD_RING(R_DC_COUNT,"\\RING\\SIMP_R_DC_COUNT.wav")
RES_ADD_RING(R_TP_KEYLOCK,"\\RING\\SIMP_R_TP_KEYLOCK.amr")
RES_ADD_RING(R_DL_1,"\\RING\\R_DL_1.WAV")
#endif

RES_ADD_RING(R_DC_1,"\\RING\\R_DC_2.wav")
RES_ADD_RING(R_BAT_1,"\\RING\\R_BAT_1.mp3")
#ifdef MMIWNS_SUPPORT
RES_ADD_RING(R_WNS_1,"\\RING\\R_WNS_1.mp3")
#endif
#ifdef ASP_SUPPORT
RES_ADD_RING(R_ASP_1,"\\RING\\R_ASP_1.wav")
RES_ADD_RING(R_ASP_2,"\\RING\\R_ASP_2.wav")
RES_ADD_RING(R_ASP_3,"\\RING\\R_ASP_3.wav")
RES_ADD_RING(R_ASP_4,"\\RING\\R_ASP_4.wav")
RES_ADD_RING(R_ASP_5,"\\RING\\R_ASP_5.wav")
RES_ADD_RING(R_ASP_6,"\\RING\\R_ASP_6.wav")
RES_ADD_RING(R_ASP_7,"\\RING\\R_ASP_7.wav")
RES_ADD_RING(R_ASP_8,"\\RING\\R_ASP_8.wav")
#endif

//RES_ADD_RING(R_DC_FINAL_COUNT,"\\RING\\R_DC_FINAL_COUNT.bin")

//RES_ADD_RING(R_BAT_2,"\\RING\\R_BAT_2.mid")
//RES_ADD_RING(R_DEL_1,"\\RING\\R_DEL_1.mid")
//RES_ADD_RING(R_DEL_2,"\\RING\\R_DEL_2.mid")

#ifdef WIFI_SUPPORT
RES_ADD_MODULE(MMI_MODULE_WIFI,"\\wifi\\wifi_mdu_def.h")
#endif

#ifdef OPERA_MOBILE_SUPPORT_V11
RES_ADD_MODULE(MMI_MODULE_OPERA_MOBILE,"\\operamobile\\operamobile_mdu_def.h")
#endif

#if defined(OPERA_MINI_SUPPORT_VER42) || defined(OPERA_MINI_SUPPORT_VER6)
RES_ADD_MODULE(MMI_MODULE_OPERA_MINI,"\\operamini\\operamini_mdu_def.h")
#endif

#ifdef MMI_POWERONOFFMP4_ENABLE
RES_ADD_RING(OPEN_MP4,"\\RING\\OPEN_MP4.3gp")
#endif

#ifdef MV_SUPPORT
#ifndef MV_FILE_RES
RES_ADD_RING(MVOICE_RES,"\\RING\\MVOICE_RES.irf")
#endif
#endif

#ifdef LIVE_WALLPAPER_SUPPORT_PAPERTOWN
RES_ADD_RING(R_LIVEPAPER_BIRD1,"\\RING\\R_LIVEPAPER_BIRD1.wav")
RES_ADD_RING(R_LIVEPAPER_BIRD2,"\\RING\\R_LIVEPAPER_BIRD2.wav")
RES_ADD_RING(R_LIVEPAPER_CAT,"\\RING\\R_LIVEPAPER_CAT.wav")
RES_ADD_RING(R_LIVEPAPER_DOG,"\\RING\\R_LIVEPAPER_DOG.mp3")
RES_ADD_RING(R_LIVEPAPER_TRAFFIC,"\\RING\\R_LIVEPAPER_TRAFFIC.mp3")
#endif

#ifdef MMI_AZAN_SUPPORT
#ifdef MMI_AZAN_MATRIX_SUPPORT
RES_ADD_RING(R_AZAN_MP3_1,"\\RING\\azan_part.mp3")
#else
RES_ADD_RING(R_AZAN_MP3_1,"\\RING\\azan_part1.mp3")
RES_ADD_RING(R_AZAN_MP3_2,"\\RING\\azan_part2.mp3")
RES_ADD_RING(R_AZAN_MP3_3,"\\RING\\azan_part3.mp3")
RES_ADD_RING(R_AZAN_MP3_4,"\\RING\\azan_part4.mp3")
RES_ADD_RING(R_AZAN_MP3_5,"\\RING\\azan_part5.mp3")
RES_ADD_RING(R_AZAN_MP3_6,"\\RING\\azan_part6.mp3")
RES_ADD_RING(R_AZAN_MP3_7,"\\RING\\azan_part7.mp3")
RES_ADD_RING(R_AZAN_MP3_8,"\\RING\\azan_part8.mp3")
RES_ADD_RING(R_AZAN_MP3_9,"\\RING\\azan_part9.mp3")
RES_ADD_RING(R_AZAN_MP3_10,"\\RING\\azan_part10.mp3")
RES_ADD_RING(R_AZAN_MP3_11,"\\RING\\azan_part11.mp3")
RES_ADD_RING(R_AZAN_MP3_12,"\\RING\\azan_part12.mp3")
RES_ADD_RING(R_AZAN_MP3_13,"\\RING\\azan_part13.mp3")
RES_ADD_RING(R_AZAN_MP3_14,"\\RING\\azan_part14.mp3")
#endif /*MMI_AZAN_MATRIX_SUPPORT*/
#endif

#ifdef VIRTUAL_VIBRATE_FUNC
RES_ADD_RING(R_VIBRATE_200HZ, "\\RING\\R_VIBRATE_MUTE.wav")
#endif

#ifdef MMI_ETWS_SUPPORT
RES_ADD_RING(R_ETWS,"\\RING\\R_ETWS.mid")
RES_ADD_RING(R_ETWS_PWS,"\\RING\\R_ETWS.mid")
#endif

//operator logo ring
#if 0
RES_ADD_RING(PROMO_OPERATOR_RING,"\\RING\\promo_operator_ring.mp3")
RES_ADD_RING_TAG(PROMO_OPERATOR_RING,"\\RING\\promo_operator_ring.mp3")
#endif
//]]define ring infomation
#ifdef MMI_COMPILE_E//8910UIX
RES_ADD_RING(R_RECORD_ON,"\\RING\\camera_rec_on.wav")
RES_ADD_RING(R_RECORD_OFF,"\\RING\\camera_rec_off.wav")


RES_ADD_RING(R_BEEP,"\\RING\\Beep.mid")
RES_ADD_RING(R_CAMERA,"\\RING\\Camera.mid")

#else
RES_ADD_RING(R_RECORD_ON,"\\RING\\SIMP_R_KEY_LOUND_2.WAV")
RES_ADD_RING(R_RECORD_OFF,"\\RING\\SIMP_R_KEY_LOUND_2.WAV")
//#define R_RECORD_OFF R_RECORD_ON
#endif
RES_ADD_RING(TONE_425HZ,"\\RING\\425HZ.amr")

#ifdef PHONE_NUMBER_VOICE
RES_ADD_RING(R_NUM_0,"\\RING\\R_NUM_0.mp3")
RES_ADD_RING(R_NUM_1,"\\RING\\R_NUM_1.mp3")
RES_ADD_RING(R_NUM_2,"\\RING\\R_NUM_2.mp3")
RES_ADD_RING(R_NUM_3,"\\RING\\R_NUM_3.mp3")
RES_ADD_RING(R_NUM_4,"\\RING\\R_NUM_4.mp3")
RES_ADD_RING(R_NUM_5,"\\RING\\R_NUM_5.mp3")
RES_ADD_RING(R_NUM_6,"\\RING\\R_NUM_6.mp3")
RES_ADD_RING(R_NUM_7,"\\RING\\R_NUM_7.mp3")
RES_ADD_RING(R_NUM_8,"\\RING\\R_NUM_8.mp3")
RES_ADD_RING(R_NUM_9,"\\RING\\R_NUM_9.mp3")
RES_ADD_RING(R_NUM_ADD,"\\RING\\R_NUM_ADD.mp3")
RES_ADD_RING(R_NUM_CALL,"\\RING\\R_NUM_CALL.WAV")
RES_ADD_RING(R_NUM_HUSH,"\\RING\\R_NUM_HUSH.mp3")
RES_ADD_RING(R_NUM_P,"\\RING\\R_NUM_P.mp3")
RES_ADD_RING(R_NUM_STAR,"\\RING\\R_NUM_STAR.mp3")
RES_ADD_RING(R_NUM_W,"\\RING\\R_NUM_W.mp3")
#endif
//dong.chunguang instead by R_WNS_1
//#ifdef MMIWNS_SUPPORT
//RES_ADD_RING(R_GENERIC_ALERT,"\\RING\\GenericAlert.mp3")
//RES_ADD_RING_TAG(R_GENERIC_ALERT,"\\RING\\GenericAlert.mp3")
//#endif

// add for gameloft
#ifdef GAMELOFT_COM_SUPPORT
#ifdef GAMELOFT_PLAY_RING_BY_ID

#ifdef GAMELOFT_NINJA_SUPPORT
    RES_ADD_RING(RING_ID_NINJA_LOSE,"\\RING\\gameloft_a_ninja_lose.mid")
  #ifdef GAMELOFT_PLAY_RING_ONCE
    RES_ADD_RING(RING_ID_NINJA_MOVE,"\\RING\\gameloft_a_ninja_move.mid") 
  #else
    RES_ADD_RING(RING_ID_NINJA_GAME,"\\RING\\gameloft_a_ninja_game.mid")
  #endif
#endif
              
#ifdef GAMELOFT_SKYGIFT_SUPPORT
    RES_ADD_RING(RING_ID_SKYGIFT_LOSE, "\\RING\\gameloft_a_skygift_lose.mid")
  #ifdef GAMELOFT_PLAY_RING_ONCE
    RES_ADD_RING(RING_ID_SKYGIFT_CATCH,"\\RING\\gameloft_a_skygift_catch.mid")
  #else
    RES_ADD_RING(RING_ID_SKYGIFT_GAME, "\\RING\\gameloft_a_skygift_game.mid")
  #endif
#endif

   
#ifdef GAMELOFT_NITRORACING_SUPPORT
    RES_ADD_RING(RING_ID_NITRORACING_LOSE, "\\RING\\gameloft_a_nitroracing_lose.mid")
  #ifdef GAMELOFT_PLAY_RING_ONCE
    RES_ADD_RING(RING_ID_NITRORACING_LEFT, "\\RING\\gameloft_a_nitroracing_left.mid")
    RES_ADD_RING(RING_ID_NITRORACING_RIGHT,"\\RING\\gameloft_a_nitroracing_right.mid")
  #else
    RES_ADD_RING(RING_ID_NITRORACING_GAME, "\\RING\\gameloft_a_nitroracing_game.mid")
  #endif
#endif
 
#ifdef GAMELOFT_AIRSTRIKE_SUPPORT
    RES_ADD_RING(RING_ID_AIRSTRIKE_WIN, "\\RING\\gameloft_a_airstrike_win.mid")
  #ifdef GAMELOFT_PLAY_RING_ONCE
    RES_ADD_RING(RING_ID_AIRSTRIKE_DASH,"\\RING\\gameloft_a_airstrike_destroy.mid")
  #else
    RES_ADD_RING(RING_ID_AIRSTRIKE_GAME,"\\RING\\gameloft_a_airstrike_game.mid")
  #endif
#endif

#ifdef GAMELOFT_TETRIS_SUPPORT
    RES_ADD_RING(RING_ID_TETRIS_CLEAR,"\\RING\\gameloft_a_tetris_clear.mid")
    RES_ADD_RING(RING_ID_TETRIS_GAME,"\\RING\\gameloft_a_tetris_game.mid")
    RES_ADD_RING(RING_ID_TETRIS_LOSE, "\\RING\\gameloft_a_tetris_lose.mid")
#endif

#ifdef GAMELOFT_SNAKEX_SUPPORT
    RES_ADD_RING(RING_ID_SNAKEX_EAT,  "\\RING\\gameloft_a_snakex_eat.mid")
    RES_ADD_RING(RING_ID_SNAKEX_LOSE, "\\RING\\gameloft_a_snakex_lose.mid")
#endif

#endif
#endif
#endif

//矢量字体
#if defined( FONT_TYPE_SUPPORT_VECTOR) && !defined (RES_FONT_SUPPORT_POINTARRAY)
#ifdef VECTOR_FONT_SUPPORT_ETRUMP

#if (defined(IM_ENGLISH_SUPPORT) \
	|| defined(IM_FRENCH_SUPPORT) \
	|| defined(IM_HUNGARIAN_SUPPORT) \
	|| defined(IM_INDONESIAN_SUPPORT) \
	|| defined(IM_MALAY_SUPPORT) \
	|| defined(IM_PORTUGUESE_SUPPORT) \
	|| defined(IM_RUSSIAN_SUPPORT) \
	|| defined(IM_SPANISH_SUPPORT) \
	|| defined(IM_TAGALOG_SUPPORT) \
	|| defined(IM_VIETNAMESE_SUPPORT) \
	|| defined(IM_ITALIAN_SUPPORT) \
	|| defined(IM_TURKISH_SUPPORT) \
	|| defined(IM_GERMAN_SUPPORT) \
	|| defined(IM_GREEK_SUPPORT) \
	|| defined(IM_CZECH_SUPPORT) \
	|| defined(IM_ROMANIAN_SUPPORT) \
	|| defined(IM_SLOVENIAN_SUPPORT) \
	|| defined(IM_AFRIKAANS_SUPPORT) \
	|| defined(IM_ALBANIAN_SUPPORT) \
	|| defined(IM_AZERBAIJANI_SUPPORT) \
	|| defined(IM_BASQUE_SUPPORT) \
	|| defined(IM_BULGARIAN_SUPPORT) \
	|| defined(IM_CATALAN_SUPPORT) \
	|| defined(IM_CROATIAN_SUPPORT) \
	|| defined(IM_DANISH_SUPPORT) \
	|| defined(IM_DUTCH_SUPPORT) \
	|| defined(IM_ESTONIAN_SUPPORT) \
	|| defined(IM_FILIPINO_SUPPORT) \
	|| defined(IM_FINNISH_SUPPORT) \
	|| defined(IM_GALICIAN_SUPPORT) \
	|| defined(IM_HAUSA_SUPPORT) \
	|| defined(IM_ICELANDIC_SUPPORT) \
	|| defined(IM_IGBO_SUPPORT) \
	|| defined(IM_IRISH_SUPPORT) \
	|| defined(IM_KAZAKH_SUPPORT) \
	|| defined(IM_LATVIAN_SUPPORT) \
	|| defined(IM_LITHUANIAN_SUPPORT) \
	|| defined(IM_MACEDONIAN_SUPPORT) \
	|| defined(IM_MOLDOVAN_SUPPORT) \
	|| defined(IM_NORWEGIA_SUPPORT) \
	|| defined(IM_POLISH_SUPPORT) \
	|| defined(IM_SERBIAN_SUPPORT) \
	|| defined(IM_SESOTHO_SUPPORT) \
	|| defined(IM_SLOVAK_SUPPORT) \
	|| defined(IM_SWEDISH_SUPPORT) \
	|| defined(IM_UKRAINIAN_SUPPORT) \
	|| defined(IM_YORUBA_SUPPORT) \
	|| defined(IM_XHOSA_SUPPORT) \
    || defined(IM_ZULU_SUPPORT) \
    || defined(IM_SWAHILI_SUPPORT) )
RES_ADD_FONT(LANG_FONT_LATINFULL,"\\FONT\\Spreadtrum_SansSerifLatinFullSetF_OTTB_SE.ttf","")
#endif

#if (defined IM_SIMP_CHINESE_SUPPORT && defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT && defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\Spreadtrum_YH13000F.ttf","")
#elif defined(IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\Spreadtrum_YH2312F.ttf","")
#elif defined(IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\Spreadtrum_YHBIG5F.ttf","")
#endif

#if (defined(IM_ARABIC_SUPPORT) \
	|| defined(IM_PERSIAN_SUPPORT) \
	|| defined(IM_HEBREW_SUPPORT) \
    || defined(IM_URDU_SUPPORT))
RES_ADD_FONT(LANG_FONT_ARABIC,"\\FONT\\Spreadtrum_SansSerifArabicF_OTTB_SE.ttf","")
#endif

#if (defined(IM_BENGALI_SUPPORT) || defined(IM_ASSAMESE_SUPPORT))
RES_ADD_FONT(LANG_FONT_BENGALI,"\\FONT\\Spreadtrum_SansSerifBengalF_OTTB_SE.ttf","")
#endif

#if defined(IM_GUJARATI_SUPPORT)
RES_ADD_FONT(LANG_FONT_GUJARATI,"\\FONT\\Spreadtrum_SansSerifGujaratiF_OTTB_SE.ttf","")
#endif

#if defined(IM_HINDI_SUPPORT) || defined(IM_MARATHI_SUPPORT)
RES_ADD_FONT(LANG_FONT_HINDI,"\\FONT\\Spreadtrum_SansSerifHindiF_OTTB_SE.ttf","")
#endif

#if defined(IM_KANNADA_SUPPORT)
RES_ADD_FONT(LANG_FONT_KANNADA,"\\FONT\\Spreadtrum_SansSerifKanadaF_OTTB_SE.ttf","")
#endif

#if defined(IM_LAO_SUPPORT)
RES_ADD_FONT(LANG_FONT_LAO,"\\FONT\\Spreadtrum_SansSerifLaosF_OTTB_SE.ttf","")
#endif

#if defined(IM_MALAYALAM_SUPPORT)
RES_ADD_FONT(LANG_FONT_MALAYALAM,"\\FONT\\Spreadtrum_SansSerifMalaylamF_OTTB_SE.ttf","")
#endif

#if defined(IM_MYANMAR_SUPPORT)
RES_ADD_FONT(LANG_FONT_MYANMAR,"\\FONT\\Spreadtrum_SansSerifMyanmarF_OTTB_SE.ttf","")
#endif

#if defined(IM_TAMIL_SUPPORT)
RES_ADD_FONT(LANG_FONT_TAMIL,"\\FONT\\Spreadtrum_SansSerifTamilF_OTTB_SE.ttf","")
#endif

#if defined(IM_TELUGU_SUPPORT)
RES_ADD_FONT(LANG_FONT_TELUGU,"\\FONT\\Spreadtrum_SansSerifTeluguF_OTTB_SE.ttf","")
#endif

#if defined(IM_THAI_SUPPORT)
RES_ADD_FONT(LANG_FONT_THAI,"\\FONT\\Spreadtrum_SansSerifThaiF_OTTB_SE.ttf","")
#endif

#if defined(IM_TIBETAN_SUPPORT)
RES_ADD_FONT(LANG_FONT_TIBETAN,"\\FONT\\Spreadtrum_SansSerifTibetanF_OTTB_SE.ttf","")
#endif

#if defined(IM_UYGHUR_SUPPORT)
RES_ADD_FONT(LANG_FONT_UYGHUR,"\\FONT\\Spreadtrum_SansSerifUKKF_OTTB_SE.ttf","")
#endif

#if defined(IM_KHMER_SUPPORT)
RES_ADD_FONT(LANG_FONT_KHMER,"\\FONT\\Spreadtrum_SansSerifKhmerF_OTTB_SE.ttf","")
#endif

#if defined(IM_SINHALESE_SUPPORT)
RES_ADD_FONT(LANG_FONT_SINHALA,"\\FONT\\Spreadtrum_SansSerifSinhalaF_OTTB_SE.ttf","")
#endif

#else    // else of VECTOR_FONT_SUPPORT_ETRUMP
// use default vector font
//拉丁语使用latin.ini生成
#if defined MMI_FONT_LATIN_BASIC || defined IM_SIMP_CHINESE_SUPPORT || defined IM_TEST_SUPPORT || defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT
RES_ADD_FONT(LANG_FONT_LATIN,"\\FONT\\LANG_FONT_LATIN_BASIC_VECTOR.lib","")
#else
RES_ADD_FONT(LANG_FONT_LATIN,"\\FONT\\LANG_FONT_LATIN_VECTOR.lib","")
#endif

#if defined IM_SIMP_CHINESE_SUPPORT || defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT
RES_ADD_FONT(LANG_FONT_HAN_SIMP,"\\FONT\\LANG_FONT_HAN_VECTOR.lib","")
#endif
//zmt add start
RES_ADD_FONT(LANG_FONT_LATIN_YINBIAO,"\\FONT\\ADD_NEW_YINBIAO_FILE.lib","")
//zmt add end
#if defined IM_TRAD_CHINESE_SUPPORT || defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT
RES_ADD_FONT(LANG_FONT_HAN_TRAD_TW,"\\FONT\\LANG_FONT_HAN_TRAD_TW_VECTOR.lib","")
#endif
#ifdef IM_TRAD_CHINESE_HK_SUPPORT
RES_ADD_FONT(LANG_FONT_HAN_TRAD_HK,"\\FONT\\LANG_FONT_HAN_TRAD_HK_VECTOR.lib","")
#endif

#if defined IM_ARABIC_SUPPORT || defined IM_URDU_SUPPORT || defined IM_PERSIAN_SUPPORT || defined IM_HAUSA_SUPPORT || defined IM_PASHTO_SUPPORT
//阿拉伯语使用arabic.ini生成
RES_ADD_FONT(LANG_FONT_ARABIC,"\\FONT\\LANG_FONT_ARABIC_VECTOR.lib","")
#endif

#ifdef IM_THAI_SUPPORT
//泰语使用thai.ini生成
RES_ADD_FONT(LANG_FONT_THAI,"\\FONT\\LANG_FONT_THAI_VECTOR.lib","")
#endif

#if defined IM_HINDI_SUPPORT || defined IM_MARATHI_SUPPORT
//印度语使用hindi.ini生成
RES_ADD_FONT(LANG_FONT_HINDI,"\\FONT\\LANG_FONT_HINDI_VECTOR.lib","")
#endif

#if defined IM_RUSSIAN_SUPPORT || defined IM_BULGARIAN_SUPPORT || defined IM_MACEDONIAN_SUPPORT || defined IM_KAZAKH_SUPPORT || defined IM_BULGARIAN_SUPPORT  || defined IM_UZBEK_SUPPORT  || defined IM_LINGALA_SUPPORT
//俄语 保加利亚语 使用russian.ini生成
RES_ADD_FONT(LANG_FONT_CYRILLIC,"\\FONT\\LANG_FONT_CYRILLIC_VECTOR.lib","")
#endif

#ifdef IM_GREEK_SUPPORT
//希腊语使用greek.ini生成
RES_ADD_FONT(LANG_FONT_GREEK,"\\FONT\\LANG_FONT_GREEK_VECTOR.lib","")
#endif

#ifdef IM_HEBREW_SUPPORT
//希伯来语使用hebrew.ini生成
RES_ADD_FONT(LANG_FONT_HEBREW,"\\FONT\\LANG_FONT_HEBREW_VECTOR.lib","")
#endif

#if defined IM_BENGALI_SUPPORT || defined IM_ASSAMESE_SUPPORT || defined(IM_BENGALI_BD_SUPPORT)
//孟加拉语使用bengali.ini生成
RES_ADD_FONT(LANG_FONT_BENGALI,"\\FONT\\LANG_FONT_BENGALI_VECTOR.lib","")
#endif

#ifdef IM_TAMIL_SUPPORT
//泰米尔语使用tamil.ini生成
RES_ADD_FONT(LANG_FONT_TAMIL,"\\FONT\\LANG_FONT_TAMIL_VECTOR.lib","")
#endif

#ifdef IM_TELUGU_SUPPORT
//泰卢固语使用telugu.ini生成
RES_ADD_FONT(LANG_FONT_TELUGU,"\\FONT\\LANG_FONT_TELUGU_VECTOR.lib","")
#endif

#ifdef IM_SINHALESE_SUPPORT
//僧伽罗语使用sinhala.ini生成
RES_ADD_FONT(LANG_FONT_SINHALA,"\\FONT\\LANG_FONT_SINHALA_VECTOR.lib","")
#endif

#ifdef IM_TIBETAN_SUPPORT
//西藏语使用Tibetan.ini生成
RES_ADD_FONT(LANG_FONT_TIBETAN,"\\FONT\\LANG_FONT_TIBETAN_VECTOR.lib","")
#endif

#ifdef IM_GUJARATI_SUPPORT
RES_ADD_FONT(LANG_FONT_GUJARATI,"\\FONT\\LANG_FONT_GUJARATI_VECTOR.lib","")
#endif

#ifdef IM_KANNADA_SUPPORT
RES_ADD_FONT(LANG_FONT_KANNADA,"\\FONT\\LANG_FONT_KANNADA_VECTOR.lib","")
#endif

#ifdef IM_MALAYALAM_SUPPORT
RES_ADD_FONT(LANG_FONT_MALAYALAM,"\\FONT\\LANG_FONT_MALAYALAM_VECTOR.lib","")
#endif

#ifdef IM_ORIYA_SUPPORT
RES_ADD_FONT(LANG_FONT_ORIYA,"\\FONT\\LANG_FONT_ORIYA_VECTOR.lib","")
#endif

#ifdef IM_PUNJABI_SUPPORT
RES_ADD_FONT(LANG_FONT_PUNJABI,"\\FONT\\LANG_FONT_GURMUKHI_VECTOR.lib","")
#endif

#ifdef IM_MYANMAR_SUPPORT
RES_ADD_FONT(LANG_FONT_MYANMAR,"\\FONT\\LANG_FONT_MYANMAR_VECTOR.lib","")
#endif

#ifdef IM_KHMER_SUPPORT
RES_ADD_FONT(LANG_FONT_KHMER,"\\FONT\\LANG_FONT_KHMER_VECTOR.lib","")
#endif

#ifdef IM_AMHARIC_SUPPORT
RES_ADD_FONT(LANG_FONT_ETHIOPIC,"\\FONT\\LANG_FONT_ETHIOPIC_VECTOR.lib","")
#endif

#ifdef IM_LAO_SUPPORT
RES_ADD_FONT(LANG_FONT_LAO,"\\FONT\\LANG_FONT_LAO_VECTOR.lib","")
#endif

#endif // end   VECTOR_FONT_SUPPORT
//灰度字体
#elif defined FONT_TYPE_SUPPORT_GRAY
#if 1
#error 
#else
//拉丁语使用latin.ini生成
RES_ADD_FONT(LANG_FONT_LATIN,"\\FONT\\LANG_FONT_LATIN_GRAY.lib","")

#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
//简体繁体中文都使用HAN字库, 纯简体用hans.ini生成, 纯繁体用hant.ini生成, 简体繁体用hanst.ini生成
#ifdef MAINLCD_SIZE_176X220
//176x220使用14,16号字体
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_14_16_GRAY.lib","")
#else
#ifdef PLATFORM_SC8800H
//8801h使用12,14,16,20号字体, 如资源空间剩余较大, 可使用灰度字体
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_8801h_GRAY.lib","")
#else
//其他情况使用16,20号字体
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_GRAY.lib","")
#endif
#endif
#endif

#if defined IM_ARABIC_SUPPORT || defined IM_URDU_SUPPORT || defined IM_PERSIAN_SUPPORT || defined IM_HAUSA_SUPPORT
//阿拉伯语使用arabic.ini生成
RES_ADD_FONT(LANG_FONT_ARABIC,"\\FONT\\LANG_FONT_ARABIC_GRAY.lib","")
#endif

#ifdef IM_THAI_SUPPORT
//泰语使用thai.ini生成
RES_ADD_FONT(LANG_FONT_THAI,"\\FONT\\LANG_FONT_THAI_GRAY.lib","")
#endif

#if defined IM_HINDI_SUPPORT || defined IM_MARATHI_SUPPORT
//印度语使用hindi.ini生成
RES_ADD_FONT(LANG_FONT_HINDI,"\\FONT\\LANG_FONT_HINDI_GRAY.lib","")
#endif

#if defined IM_RUSSIAN_SUPPORT || defined IM_BULGARIAN_SUPPORT
//俄语 保加利亚语 使用russian.ini生成
RES_ADD_FONT(LANG_FONT_RUSSIAN,"\\FONT\\LANG_FONT_RUSSIAN_GRAY.lib","")
#endif

#ifdef IM_GREEK_SUPPORT
//希腊语使用greek.ini生成
RES_ADD_FONT(LANG_FONT_GREEK,"\\FONT\\LANG_FONT_GREEK_GRAY.lib","")
#endif

#ifdef IM_HEBREW_SUPPORT
//希伯来语使用hebrew.ini生成
RES_ADD_FONT(LANG_FONT_HEBREW,"\\FONT\\LANG_FONT_HEBREW_GRAY.lib","")
#endif

#if defined IM_BENGALI_SUPPORT || defined IM_ASSAMESE_SUPPORT
//孟加拉语使用bengali.ini生成
RES_ADD_FONT(LANG_FONT_BENGALI,"\\FONT\\LANG_FONT_BENGALI_GRAY.lib","")
#endif

#ifdef IM_TAMIL_SUPPORT
//泰米尔语使用tamil.ini生成
RES_ADD_FONT(LANG_FONT_TAMIL,"\\FONT\\LANG_FONT_TAMIL_GRAY.lib","")
#endif

#ifdef IM_TELUGU_SUPPORT
//泰卢固语使用telugu.ini生成
RES_ADD_FONT(LANG_FONT_TELUGU,"\\FONT\\LANG_FONT_TELUGU_GRAY.lib","")
#endif

#ifdef IM_SINHALESE_SUPPORT
//僧伽罗语使用sinhala.ini生成
RES_ADD_FONT(LANG_FONT_SINHALA,"\\FONT\\LANG_FONT_SINHALA_GRAY.lib","")
#endif

#ifdef IM_TIBETAN_SUPPORT
//西藏语使用Tibetan.ini生成
RES_ADD_FONT(LANG_FONT_TIBETAN,"\\FONT\\LANG_FONT_TIBETAN_GRAY.lib","")
#endif

#ifdef IM_GUJARATI_SUPPORT
RES_ADD_FONT(LANG_FONT_GUJARATI,"\\FONT\\LANG_FONT_GUJARATI_GRAY.lib","")
#endif

#ifdef IM_KANNADA_SUPPORT
RES_ADD_FONT(LANG_FONT_KANNADA,"\\FONT\\LANG_FONT_KANNADA_GRAY.lib","")
#endif

#ifdef IM_MALAYALAM_SUPPORT
RES_ADD_FONT(LANG_FONT_MALAYALAM,"\\FONT\\LANG_FONT_MALAYALAM_GRAY.lib","")
#endif

#ifdef IM_ORIYA_SUPPORT
RES_ADD_FONT(LANG_FONT_ORIYA,"\\FONT\\LANG_FONT_ORIYA_GRAY.lib","")
#endif

#ifdef IM_PUNJABI_SUPPORT
RES_ADD_FONT(LANG_FONT_PUNJABI,"\\FONT\\LANG_FONT_GURMUKHI_GRAY.lib","")
#endif

#ifdef IM_MYANMAR_SUPPORT
RES_ADD_FONT(LANG_FONT_MYANMAR,"\\FONT\\LANG_FONT_MYANMAR_GRAY.lib","")
#endif

#ifdef IM_KHMER_SUPPORT
RES_ADD_FONT(LANG_FONT_KHMER,"\\FONT\\LANG_FONT_KHMER_GRAY.lib","")
#endif

#ifdef IM_AMHARIC_SUPPORT
RES_ADD_FONT(LANG_FONT_ETHIOPIC,"\\FONT\\LANG_FONT_ETHIOPIC_GRAY.lib","")
#endif
#endif
//点阵字体
#else

//拉丁语使用latin.ini生成
#if defined MMI_FONT_LATIN_BASIC || defined IM_SIMP_CHINESE_SUPPORT || defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT
RES_ADD_FONT(LANG_FONT_LATIN,"\\FONT\\LANG_FONT_LATIN_BASIC.lib","")
#else
RES_ADD_FONT(LANG_FONT_LATIN,"\\FONT\\LANG_FONT_LATIN.lib","")
#endif

#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_HK_SUPPORT || defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN.lib","") 
#endif
#ifdef IM_TRAD_CHINESE_HK_SUPPORT
//RES_ADD_FONT(LANG_FONT_HAN_TRAD_HK,"\\FONT\\LANG_FONT_HAN_TRAD_HK.lib","")
#endif

#if defined IM_ARABIC_SUPPORT || defined IM_URDU_SUPPORT || defined IM_PERSIAN_SUPPORT || defined IM_HAUSA_SUPPORT || defined IM_PASHTO_SUPPORT
//阿拉伯语使用arabic.ini生成
RES_ADD_FONT(LANG_FONT_ARABIC,"\\FONT\\LANG_FONT_ARABIC.lib","")
#endif

#ifdef IM_THAI_SUPPORT
//泰语使用thai.ini生成
RES_ADD_FONT(LANG_FONT_THAI,"\\FONT\\LANG_FONT_THAI.lib","")
#endif

#if defined IM_HINDI_SUPPORT || defined IM_MARATHI_SUPPORT
//印度语使用hindi.ini生成
RES_ADD_FONT(LANG_FONT_HINDI,"\\FONT\\LANG_FONT_HINDI.lib","")
#endif

#if defined IM_RUSSIAN_SUPPORT || defined IM_BULGARIAN_SUPPORT || defined IM_BELARUS_SUPPORT || defined IM_UZBEK_SUPPORT || defined IM_MACEDONIAN_SUPPORT || defined IM_LINGALA_SUPPORT
//俄语 保加利亚语 使用russian.ini生成
RES_ADD_FONT(LANG_FONT_RUSSIAN,"\\FONT\\LANG_FONT_RUSSIAN.lib","")
#endif

#ifdef IM_GREEK_SUPPORT
//希腊语使用greek.ini生成
RES_ADD_FONT(LANG_FONT_GREEK,"\\FONT\\LANG_FONT_GREEK.lib","")
#endif

#ifdef IM_HEBREW_SUPPORT
//希伯来语使用hebrew.ini生成
RES_ADD_FONT(LANG_FONT_HEBREW,"\\FONT\\LANG_FONT_HEBREW.lib","")
#endif

#if defined IM_BENGALI_SUPPORT || defined IM_ASSAMESE_SUPPORT || defined(IM_BENGALI_BD_SUPPORT)
//孟加拉语使用bengali.ini生成
RES_ADD_FONT(LANG_FONT_BENGALI,"\\FONT\\LANG_FONT_BENGALI.lib","")
#endif

#ifdef IM_TAMIL_SUPPORT
//泰米尔语使用tamil.ini生成
RES_ADD_FONT(LANG_FONT_TAMIL,"\\FONT\\LANG_FONT_TAMIL.lib","")
#endif

#ifdef IM_TELUGU_SUPPORT
//泰卢固语使用telugu.ini生成
RES_ADD_FONT(LANG_FONT_TELUGU,"\\FONT\\LANG_FONT_TELUGU.lib","")
#endif

#ifdef IM_SINHALESE_SUPPORT
//僧伽罗语使用sinhala.ini生成
RES_ADD_FONT(LANG_FONT_SINHALA,"\\FONT\\LANG_FONT_SINHALA.lib","")
#endif

#ifdef IM_TIBETAN_SUPPORT
//西藏语使用Tibetan.ini生成
RES_ADD_FONT(LANG_FONT_TIBETAN,"\\FONT\\LANG_FONT_TIBETAN.lib","")
#endif

#ifdef IM_GUJARATI_SUPPORT
RES_ADD_FONT(LANG_FONT_GUJARATI,"\\FONT\\LANG_FONT_GUJARATI.lib","")
#endif

#ifdef IM_KANNADA_SUPPORT
RES_ADD_FONT(LANG_FONT_KANNADA,"\\FONT\\LANG_FONT_KANNADA.lib","")
#endif

#ifdef IM_MALAYALAM_SUPPORT
RES_ADD_FONT(LANG_FONT_MALAYALAM,"\\FONT\\LANG_FONT_MALAYALAM.lib","")
#endif

#ifdef IM_ORIYA_SUPPORT
RES_ADD_FONT(LANG_FONT_ORIYA,"\\FONT\\LANG_FONT_ORIYA.lib","")
#endif

#ifdef IM_PUNJABI_SUPPORT
RES_ADD_FONT(LANG_FONT_PUNJABI,"\\FONT\\LANG_FONT_GURMUKHI.lib","")
#endif

#ifdef IM_MYANMAR_SUPPORT
RES_ADD_FONT(LANG_FONT_MYANMAR,"\\FONT\\LANG_FONT_MYANMAR.lib","")
#endif

#ifdef IM_KHMER_SUPPORT
RES_ADD_FONT(LANG_FONT_KHMER,"\\FONT\\LANG_FONT_KHMER.lib","")
#endif

#ifdef IM_AMHARIC_SUPPORT
RES_ADD_FONT(LANG_FONT_ETHIOPIC,"\\FONT\\LANG_FONT_ETHIOPIC.lib","")
#endif

#endif

//]]define font infomation

//[[define logo infomation
//添加Logo有顺序要求，第一个就main logo, 第二个就是sub logo
#ifdef MMI_RES_ORIENT_LANDSCAPE
RES_ADD_LOGO(LOGO_1,"\\logo_landscape.bmp",IMG_CMP_BMP_16_565_RLE,0)
#else
RES_ADD_LOGO(LOGO_1,"\\IMAGE_TEMPORARY_BOOTUP_DEFAULT.bmp",IMG_CMP_BMP_16_565_RLE,0)
#endif
#if defined(SUBLCD_SIZE_NONE)
#elif defined(SUBLCD_SIZE_96X96)
RES_ADD_LOGO(LOGO_2,"\\sublogo_96X96.bmp",IMG_CMP_BMP_16_565_RLE,0)
#elif defined(SUBLCD_SIZE_96X64)
RES_ADD_LOGO(LOGO_2,"\\sublogo_96X64.bmp",IMG_CMP_BMP_16_565_RLE,0)
#else
#error "please add sub lcd resource"
#endif
//]]

#ifdef MMIEMAIL_SUPPORT
RES_ADD_MODULE(MMI_MODULE_EMAIL,"\\email\\email_mdu_def.h")
#endif

#ifdef SNS_SUPPORT
RES_ADD_MODULE(MMI_MODULE_MMISNS,"\\sns\\sns_mdu_def.h")
#endif

#ifdef AUTODEMO_SUPPORT
RES_ADD_MODULE(MMI_MODULE_AD,"\\autodemo\\autodemo_mdu_def.h")
#endif

/*add devicelock*/
//RES_ADD_MODULE(MMI_MODULE_DEVICELOCK,"\\devicelock\\devicelock_mdu_def.h")
/*add devicelock end*/
//add control panel
RES_ADD_MODULE(MMI_MODULE_CP,"\\cp\\cp_mdu_def.h")

//add SmartDualSIM
#ifdef MMI_SMART_DUAL_SIM_SUPPORT
RES_ADD_MODULE(MMI_MODULE_SMARTDUALSIM,"\\smartdualsim\\smartdualsim_mdu_def.h")
#endif
//add SmartDualSIM

//RES_ADD_MODULE(MMI_MODULE_MSA,"\\msa\\msa_mdu_def.h")


#ifdef MMIWNS_SUPPORT
RES_ADD_MODULE(MMI_MODULE_WNS,"\\wns\\wns_mdu_def.h")
#endif

RES_ADD_MODULE(MMI_MODULE_DATA_COUNTER,"\\datacounter\\datacounter_mdu_def.h")

#ifdef MMIPB_SYNC_WITH_O365
RES_ADD_MODULE(MMI_MODULE_CONTACTSYNC,"\\contactsync\\contactsync_mdu_def.h")
#endif

#ifdef  SFR_SUPPORT
RES_ADD_MODULE(MMI_MODULE_SFR,"\\selfreg\\selfreg_mdu_def.h")
#endif

#ifdef TTS_SUPPORT
RES_ADD_MODULE(MMI_MODULE_TTS,"\\tts\\tts_mdu_def.h")
#endif 

#ifdef MMIENG_PING_SUPPORT
RES_ADD_MODULE(MMI_MODULE_PING,"\\ping\\ping_mdu_def.h")
#endif

#ifdef  CMCCDM_SUPPORT
RES_ADD_MODULE(MMI_MODULE_CMCCDM,"\\cmccdm\\cmccdm_mdu_def.h")
#endif
#ifdef LEBAO_MUSIC_SUPPORT
RES_ADD_MODULE(MMI_MODULE_LEBAO, "\\lebao\\lebao_mdu_def.h")
#endif
//zmt add start
#ifdef FORMULA_SUPPORT
RES_ADD_MODULE(MMI_MODULE_FORMULA_WIN_ID,"\\formula\\formula_mdu_def.h")
#endif
#ifdef  MATH_COUNT_SUPPORT
RES_ADD_MODULE(MMI_MODULE_MATH_COUNT,"\\math_count\\math_count_mdu_def.h")
#endif
#ifdef  WORD_CARD_SUPPORT
RES_ADD_MODULE(MMI_MODULE_ZMT_WORD,"\\zmt_word\\zmt_word_mdu_def.h")
#endif
#ifdef  HANZI_CARD_SUPPORT
RES_ADD_MODULE(MMI_MODULE_ZMT_HANZI,"\\zmt_hanzi\\zmt_hanzi_mdu_def.h")
#endif
#ifdef  ZMT_PINYIN_SUPPORT
RES_ADD_MODULE(MMI_MODULE_ZMT_PINYIN,"\\zmt_pinyin\\zmt_pinyin_mdu_def.h")
#endif
#ifdef  ZMT_YINBIAO_SUPPORT
RES_ADD_MODULE(MMI_MODULE_ZMT_YINBIAO,"\\zmt_yinbiao\\zmt_yinbiao_mdu_def.h")
#endif
#ifdef  POETRY_LISTEN_SUPPORT
RES_ADD_MODULE(MMI_MODULE_ZMT_POETRY,"\\zmt_poetry\\zmt_poetry_mdu_def.h")
#endif
#ifdef ZDT_HTTP_APP_SUPPORT
RES_ADD_MODULE(MMI_MODULE_ZDTHTTP,"\\zdthttp\\zdthttp_mdu_def.h")
#endif
//zmt add end

