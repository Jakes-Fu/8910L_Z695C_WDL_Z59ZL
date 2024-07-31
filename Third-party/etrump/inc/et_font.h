#ifndef _ET_FONT_H_
#define _ET_FONT_H_


// define MMISET_LANGUAGE_TYPE_E
//#include "mmiset_export.h"
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
    &&!defined IM_SINHALESE_SUPPORT &&!defined IM_TIBETAN_SUPPORT)
#define MMI_FONT_LATIN_BASIC
#endif
typedef enum
{
#ifdef MMI_SIM_LANGUAGE_SUPPORT
    MMISET_LANGUAGE_AUTO,          // Auto.
#endif
    MMISET_LANGUAGE_ENGLISH,        //英文
    MMISET_LANGUAGE_SIMP_CHINESE,   //中文简体 
    MMISET_LANGUAGE_TRAD_CHINESE,   //中文繁体
    MMISET_LANGUAGE_ARABIC,         //阿拉伯
    MMISET_LANGUAGE_FRENCH,         //法语
    MMISET_LANGUAGE_HINDI,          //印第
    MMISET_LANGUAGE_HUNGARIAN,      //匈牙利
    MMISET_LANGUAGE_INDONESIAN,     //印度尼西亚
    MMISET_LANGUAGE_MALAY,          //马来
    MMISET_LANGUAGE_PORTUGUESE,     //葡萄牙
    MMISET_LANGUAGE_RUSSIAN,        //俄语
    MMISET_LANGUAGE_SPANISH,        //西班牙
    MMISET_LANGUAGE_TAGALOG,        //他加诺
    MMISET_LANGUAGE_THAI,           //泰语
    MMISET_LANGUAGE_VIETNAMESE,     //越南语
    MMISET_LANGUAGE_URDU,           //乌尔都
    MMISET_LANGUAGE_ITALIAN,        //意大利
    MMISET_LANGUAGE_PERSIAN,        //波斯
    MMISET_LANGUAGE_TURKISH,        //土耳其
    MMISET_LANGUAGE_GERMAN,         //德语
    MMISET_LANGUAGE_GREEK,          //希腊
    MMISET_LANGUAGE_HEBREW,         //希伯莱
	MMISET_LANGUAGE_BENGALI,        //孟加拉
	MMISET_LANGUAGE_CZECH,          //捷克
    MMISET_LANGUAGE_SLOVENIAN,      //斯诺文尼亚
	MMISET_LANGUAGE_ROMANIAN,       //罗马尼亚
    MMISET_LANGUAGE_TELUGU,         //泰卢固
    MMISET_LANGUAGE_MARATHI,        //马拉提
    MMISET_LANGUAGE_TAMIL,          //泰米尔
    MMISET_LANGUAGE_GUJARATI,       //古吉拉特
    MMISET_LANGUAGE_KANNADA,        //卡纳达
    MMISET_LANGUAGE_MALAYALAM,      //马拉亚拉姆
    MMISET_LANGUAGE_ORIYA,          //欧瑞亚
    MMISET_LANGUAGE_PUNJABI,        //旁加比
    MMISET_LANGUAGE_AFRIKAANS,      //南非语
    MMISET_LANGUAGE_ALBANIAN,       //阿尔巴尼亚
    MMISET_LANGUAGE_ARMENIAN,       //亚美尼亚  not support now @etrump
    MMISET_LANGUAGE_AZERBAIJANI,    //
    MMISET_LANGUAGE_BASQUE,
    MMISET_LANGUAGE_BULGARIAN,
    MMISET_LANGUAGE_CATALAN,        //加泰罗尼亚
    MMISET_LANGUAGE_CROATIAN,       //克罗地亚
    MMISET_LANGUAGE_DANISH,
    MMISET_LANGUAGE_DUTCH,
    MMISET_LANGUAGE_ESTONIAN,
    MMISET_LANGUAGE_FILIPINO,
    MMISET_LANGUAGE_FINNISH,
    MMISET_LANGUAGE_GALICIAN,
    MMISET_LANGUAGE_GEORGIAN,      //格鲁吉亚  not support now @etrump
    MMISET_LANGUAGE_HAUSA,
    MMISET_LANGUAGE_ICELANDIC,
    MMISET_LANGUAGE_IGBO,
    MMISET_LANGUAGE_IRISH,
    MMISET_LANGUAGE_KAZAKH,
    MMISET_LANGUAGE_LATVIAN,
    MMISET_LANGUAGE_LITHUANIAN,
    MMISET_LANGUAGE_MACEDONIAN,
    MMISET_LANGUAGE_MOLDOVAN,
    MMISET_LANGUAGE_NORWEGIAN,
    MMISET_LANGUAGE_POLISH,
    MMISET_LANGUAGE_SERBIAN,
    MMISET_LANGUAGE_SESOTHO,
    MMISET_LANGUAGE_SLOVAK,
    MMISET_LANGUAGE_SWEDISH,
    MMISET_LANGUAGE_UKRAINIAN,
    MMISET_LANGUAGE_YORUBA,

    MMISET_LANGUAGE_XHOSA,		//科萨语	add. chenyg@spread. 2011-05-12
    MMISET_LANGUAGE_ZULU,		//祖鲁语	add. chenyg@spread. 2011-05-12
    MMISET_LANGUAGE_ASSAMESE,		//(印度语支的)阿萨姆语	add. chenyg@spread. 2011-05-12
    MMISET_LANGUAGE_SWAHILI,	//斯瓦西里	add. chenyg@spread. 2011-05-12
    MMISET_LANGUAGE_MYANMAR,    //缅甸语
    MMISET_LANGUAGE_AMHARIC,    //阿姆哈拉   not support now @etrump
    MMISET_LANGUAGE_KHMER,    //柬埔寨
    MMISET_LANGUAGE_LAO,    //老挝
    MMISET_LANGUAGE_UYGHUR,    //维吾尔语
    MMISET_LANGUAGE_TIBETAN,   //藏语
    MMISET_LANGUAGE_SINHALESE,	//僧伽罗
    
    MMISET_MAX_LANGUAGE
} MMISET_LANGUAGE_TYPE_E;



typedef struct
{
	unsigned char*  fontdata;        // font data pointer
	unsigned long   datalen;         // font data length
	MMISET_LANGUAGE_TYPE_E* supportLanguages;
}et_font;



/************************************************************************/
/* English                                                              */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E latin_basic_supportLanguages[] = {
	MMISET_LANGUAGE_ENGLISH,
	MMISET_MAX_LANGUAGE
};

et_font et_font_latin_basic = {
	NULL, 0, latin_basic_supportLanguages
};

#define ET_FONT_LATIN_BASIC &et_font_latin_basic,


/************************************************************************/
/* Chinese                                                              */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E han_supportLanguages[] = {
#if defined(IM_SIMP_CHINESE_SUPPORT)
	MMISET_LANGUAGE_SIMP_CHINESE,
#endif
#if defined(IM_TRAD_CHINESE_SUPPORT)
	MMISET_LANGUAGE_TRAD_CHINESE,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_han = {
	NULL, 0, han_supportLanguages
};

#if (defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT))
#define CHINESE_FACE_MEM_SIZE  12*1024
#define ET_FONT_CHINESE        &et_font_han,
#else
#define CHINESE_FACE_MEM_SIZE  0
#define ET_FONT_CHINESE
#endif




/************************************************************************/
/* Arabic, Persian, Urdu, Hebrew                                        */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E arabic_supportLanguages[] = {
#if defined(IM_ARABIC_SUPPORT)
	MMISET_LANGUAGE_ARABIC,
#endif
#if defined(IM_PERSIAN_SUPPORT)
	MMISET_LANGUAGE_PERSIAN,
#endif
#if defined(IM_HEBREW_SUPPORT)
	MMISET_LANGUAGE_HEBREW,
#endif
#if defined(IM_URDU_SUPPORT)
	MMISET_LANGUAGE_URDU,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_arabic = {
	NULL, 0, arabic_supportLanguages
};

#if (defined(IM_ARABIC_SUPPORT) \
	|| defined(IM_PERSIAN_SUPPORT) \
	|| defined(IM_HEBREW_SUPPORT) \
	|| defined(IM_URDU_SUPPORT))
#define ARABIC_FACE_MEM_SIZE  12*1024
#define ET_FONT_ARABIC        &et_font_arabic,
#else
#define ARABIC_FACE_MEM_SIZE  0
#define ET_FONT_ARABIC
#endif




/************************************************************************/
/* Bengali                                                              */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E bengal_supportLanguages[] = {
#if defined(IM_BENGALI_SUPPORT)
	MMISET_LANGUAGE_BENGALI,
#endif
#if defined(IM_ASSAMESE_SUPPORT)
	MMISET_LANGUAGE_ASSAMESE,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_bengali = {
	NULL, 0, bengal_supportLanguages
};

#if (defined(IM_BENGALI_SUPPORT) || defined(IM_ASSAMESE_SUPPORT))
#define BENGALI_FACE_MEM_SIZE  12*1024
#define ET_FONT_BENGALI        &et_font_bengali,
#else
#define BENGALI_FACE_MEM_SIZE  0
#define ET_FONT_BENGALI
#endif




/************************************************************************/
/* Gujarati                                                             */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E gujarati_supportLanguages[] = {
#if defined(IM_GUJARATI_SUPPORT)
	MMISET_LANGUAGE_GUJARATI,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_gujarati = {
	NULL, 0, gujarati_supportLanguages
};

#if defined(IM_GUJARATI_SUPPORT)
#define GUJARATI_FACE_MEM_SIZE  12*1024
#define ET_FONT_GUJARATI        &et_font_gujarati,
#else
#define GUJARATI_FACE_MEM_SIZE  0
#define ET_FONT_GUJARATI
#endif




/************************************************************************/
/* Hindi and Marathi                                                    */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E hindi_supportLanguages[] = {
#if defined(IM_HINDI_SUPPORT)
	MMISET_LANGUAGE_HINDI,
#endif
#if defined(IM_MARATHI_SUPPORT)
	MMISET_LANGUAGE_MARATHI,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_hindi = {
	NULL, 0, hindi_supportLanguages
};

#if (defined(IM_HINDI_SUPPORT) || defined(IM_MARATHI_SUPPORT))
#define HINDI_FACE_MEM_SIZE  12*1024
#define ET_FONT_HINDI        &et_font_hindi,
#else
#define HINDI_FACE_MEM_SIZE  0
#define ET_FONT_HINDI
#endif




/************************************************************************/
/* Kannada                                                              */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E kannada_supportLanguages[] = {
#if defined(IM_KANNADA_SUPPORT)
	MMISET_LANGUAGE_KANNADA,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_kannada = {
	NULL, 0, kannada_supportLanguages
};

#if defined(IM_KANNADA_SUPPORT)
#define KANNADA_FACE_MEM_SIZE  12*1024
#define ET_FONT_KANNADA        &et_font_kannada,
#else
#define KANNADA_FACE_MEM_SIZE  0
#define ET_FONT_KANNADA
#endif




/************************************************************************/
/* Lao                                                                  */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E lao_supportLanguages[] = {
#if defined(IM_LAO_SUPPORT)
	MMISET_LANGUAGE_LAO,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_lao = {
	NULL, 0, lao_supportLanguages
};

#if defined(IM_LAO_SUPPORT)
#define LAO_FACE_MEM_SIZE  12*1024
#define ET_FONT_LAO        &et_font_lao,
#else
#define LAO_FACE_MEM_SIZE  0
#define ET_FONT_LAO
#endif




/************************************************************************/
/* Malayalam                                                            */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E malayalam_supportLanguages[] = {
#if defined(IM_MALAYALAM_SUPPORT)
	MMISET_LANGUAGE_MALAYALAM,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_malayalam = {
	NULL, 0, malayalam_supportLanguages
};

#if defined(IM_MALAYALAM_SUPPORT)
#define MALAYALAM_FACE_MEM_SIZE  12*1024
#define ET_FONT_MALAYALAM        &et_font_malayalam,
#else
#define MALAYALAM_FACE_MEM_SIZE  0
#define ET_FONT_MALAYALAM
#endif




/************************************************************************/
/* Myanmar                                                              */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E myanmar_supportLanguages[] = {
#if defined(IM_MYANMAR_SUPPORT)
	MMISET_LANGUAGE_MYANMAR,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_myanmar = {
	NULL, 0, myanmar_supportLanguages
};

#if defined(IM_MYANMAR_SUPPORT)
#define MYANMAR_FACE_MEM_SIZE  12*1024
#define ET_FONT_MYANMAR        &et_font_myanmar,
#else
#define MYANMAR_FACE_MEM_SIZE  0
#define ET_FONT_MYANMAR
#endif




/************************************************************************/
/* Tamil                                                                */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E tamil_supportLanguages[] = {
#if defined(IM_TAMIL_SUPPORT)
	MMISET_LANGUAGE_TAMIL,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_tamil = {
	NULL, 0, tamil_supportLanguages
};

#if defined(IM_TAMIL_SUPPORT)
#define TAMIL_FACE_MEM_SIZE  12*1024
#define ET_FONT_TAMIL        &et_font_tamil,
#else
#define TAMIL_FACE_MEM_SIZE  0
#define ET_FONT_TAMIL
#endif




/************************************************************************/
/* Telugu                                                               */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E telugu_supportLanguages[] = {
#if defined(IM_TELUGU_SUPPORT)
	MMISET_LANGUAGE_TELUGU,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_telugu = {
	NULL, 0, telugu_supportLanguages
};

#if defined(IM_TELUGU_SUPPORT)
#define TELUGU_FACE_MEM_SIZE  12*1024
#define ET_FONT_TELUGU        &et_font_telugu,
#else
#define TELUGU_FACE_MEM_SIZE  0
#define ET_FONT_TELUGU
#endif




/************************************************************************/
/* Thai                                                                 */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E thai_supportLanguages[] = {
#if defined(IM_THAI_SUPPORT)
	MMISET_LANGUAGE_THAI,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_thai = {
	NULL, 0, thai_supportLanguages
};

#if defined(IM_THAI_SUPPORT)
#define THAI_FACE_MEM_SIZE  12*1024
#define ET_FONT_THAI        &et_font_thai,
#else
#define THAI_FACE_MEM_SIZE  0
#define ET_FONT_THAI
#endif




/************************************************************************/
/* Tibetan                                                              */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E tibetan_supportLanguages[] = {
#if defined(IM_TIBETAN_SUPPORT)
	MMISET_LANGUAGE_TIBETAN,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_tibetan = {
	NULL, 0, tibetan_supportLanguages
};

#if defined(IM_TIBETAN_SUPPORT)
#define TIBETAN_FACE_MEM_SIZE  12*1024
#define ET_FONT_TIBETAN        &et_font_tibetan,
#else
#define TIBETAN_FACE_MEM_SIZE  0
#define ET_FONT_TIBETAN
#endif




/************************************************************************/
/* Khmer                                                                */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E khmer_supportLanguages[] = {
#if defined(IM_KHMER_SUPPORT)
	MMISET_LANGUAGE_KHMER,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_khmer = {
	NULL, 0, khmer_supportLanguages
};

#if defined(IM_KHMER_SUPPORT)
#define KHMER_FACE_MEM_SIZE  12*1024
#define ET_FONT_KHMER        &et_font_khmer,
#else
#define KHMER_FACE_MEM_SIZE  0
#define ET_FONT_KHMER
#endif


/************************************************************************/
/* sinhala                                                                */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E sinhala_supportLanguages[] = {
#if defined(IM_SINHALESE_SUPPORT)
	MMISET_LANGUAGE_SINHALESE,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_sinhala = {
	NULL, 0, sinhala_supportLanguages
};

#if defined(IM_SINHALESE_SUPPORT)
#define SINHALA_FACE_MEM_SIZE  12*1024
#define ET_FONT_SINHALA        &et_font_sinhala,
#else
#define SINHALA_FACE_MEM_SIZE  0
#define ET_FONT_SINHALA
#endif

/************************************************************************/
/* Uyghur                                                               */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E uyghur_supportLanguages[] = {
#if defined(IM_UYGHUR_SUPPORT)
	MMISET_LANGUAGE_UYGHUR,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_uyghur = {
	NULL, 0, uyghur_supportLanguages
};

#if defined(IM_UYGHUR_SUPPORT)
#define UYGHUR_FACE_MEM_SIZE  12*1024
#define ET_FONT_UYGHUR        &et_font_uyghur,
#else
#define UYGHUR_FACE_MEM_SIZE  0
#define ET_FONT_UYGHUR
#endif




/************************************************************************/
/* Latin full                                                           */
/************************************************************************/
MMISET_LANGUAGE_TYPE_E latin_full_supportLanguages[] =
{
#if defined(IM_ENGLISH_SUPPORT)
	MMISET_LANGUAGE_ENGLISH,
#endif
#if defined(IM_FRENCH_SUPPORT)
	MMISET_LANGUAGE_FRENCH,
#endif
#if defined(IM_HUNGARIAN_SUPPORT)
	MMISET_LANGUAGE_HUNGARIAN,
#endif
#if defined(IM_INDONESIAN_SUPPORT)
	MMISET_LANGUAGE_INDONESIAN,
#endif
#if defined(IM_MALAY_SUPPORT)
	MMISET_LANGUAGE_MALAY,
#endif
#if defined(IM_PORTUGUESE_SUPPORT)
	MMISET_LANGUAGE_PORTUGUESE,
#endif
#if defined(IM_RUSSIAN_SUPPORT)
	MMISET_LANGUAGE_RUSSIAN,
#endif
#if defined(IM_SPANISH_SUPPORT)
	MMISET_LANGUAGE_SPANISH,
#endif
#if defined(IM_TAGALOG_SUPPORT)
	MMISET_LANGUAGE_TAGALOG,
#endif
#if defined(IM_VIETNAMESE_SUPPORT)
	MMISET_LANGUAGE_VIETNAMESE,
#endif
#if defined(IM_ITALIAN_SUPPORT)
	MMISET_LANGUAGE_ITALIAN,
#endif
#if defined(IM_TURKISH_SUPPORT)
	MMISET_LANGUAGE_TURKISH,
#endif
#if defined(IM_GERMAN_SUPPORT)
	MMISET_LANGUAGE_GERMAN,
#endif
#if defined(IM_GREEK_SUPPORT)
	MMISET_LANGUAGE_GREEK,
#endif
#if defined(IM_CZECH_SUPPORT)
	MMISET_LANGUAGE_CZECH,
#endif
#if defined(IM_ROMANIAN_SUPPORT)
	MMISET_LANGUAGE_ROMANIAN,
#endif
#if defined(IM_SLOVENIAN_SUPPORT)
	MMISET_LANGUAGE_SLOVENIAN,
#endif
#if defined(IM_AFRIKAANS_SUPPORT)
	MMISET_LANGUAGE_AFRIKAANS,
#endif
#if defined(IM_ALBANIAN_SUPPORT)
	MMISET_LANGUAGE_ALBANIAN,
#endif
#if defined(IM_AZERBAIJANI_SUPPORT)
	MMISET_LANGUAGE_AZERBAIJANI,
#endif
#if defined(IM_BASQUE_SUPPORT)
	MMISET_LANGUAGE_BASQUE,
#endif
#if defined(IM_BULGARIAN_SUPPORT)
	MMISET_LANGUAGE_BULGARIAN,
#endif
#if defined(IM_CATALAN_SUPPORT)
	MMISET_LANGUAGE_CATALAN,
#endif
#if defined(IM_CROATIAN_SUPPORT)
	MMISET_LANGUAGE_CROATIAN,
#endif
#if defined(IM_DANISH_SUPPORT)
	MMISET_LANGUAGE_DANISH,
#endif
#if defined(IM_DUTCH_SUPPORT)
	MMISET_LANGUAGE_DUTCH,
#endif
#if defined(IM_ESTONIAN_SUPPORT)
	MMISET_LANGUAGE_ESTONIAN,
#endif
#if defined(IM_FILIPINO_SUPPORT)
	MMISET_LANGUAGE_FILIPINO,
#endif
#if defined(IM_FINNISH_SUPPORT)
	MMISET_LANGUAGE_FINNISH,
#endif
#if defined(IM_GALICIAN_SUPPORT)
	MMISET_LANGUAGE_GALICIAN,
#endif
#if defined(IM_HAUSA_SUPPORT)
	MMISET_LANGUAGE_HAUSA,
#endif
#if defined(IM_ICELANDIC_SUPPORT)
	MMISET_LANGUAGE_ICELANDIC,
#endif
#if defined(IM_IGBO_SUPPORT)
	MMISET_LANGUAGE_IGBO,
#endif
#if defined(IM_IRISH_SUPPORT)
	MMISET_LANGUAGE_IRISH,
#endif
#if defined(IM_KAZAKH_SUPPORT)
	MMISET_LANGUAGE_KAZAKH,
#endif
#if defined(IM_LATVIAN_SUPPORT)
	MMISET_LANGUAGE_LATVIAN,
#endif
#if defined(IM_LITHUANIAN_SUPPORT)
	MMISET_LANGUAGE_LITHUANIAN,
#endif
#if defined(IM_MACEDONIAN_SUPPORT)
	MMISET_LANGUAGE_MACEDONIAN,
#endif
#if defined(IM_MOLDOVAN_SUPPORT)
	MMISET_LANGUAGE_MOLDOVAN,
#endif
#if defined(IM_NORWEGIAN_SUPPORT)
	MMISET_LANGUAGE_NORWEGIAN,
#endif
#if defined(IM_POLISH_SUPPORT)
	MMISET_LANGUAGE_POLISH,
#endif
#if defined(IM_SERBIAN_SUPPORT)
	MMISET_LANGUAGE_SERBIAN,
#endif
#if defined(IM_SESOTHO_SUPPORT)
	MMISET_LANGUAGE_SESOTHO,
#endif
#if defined(IM_SLOVAK_SUPPORT)
	MMISET_LANGUAGE_SLOVAK,
#endif
#if defined(IM_SWEDISH_SUPPORT)
	MMISET_LANGUAGE_SWEDISH,
#endif
#if defined(IM_UKRAINIAN_SUPPORT)
	MMISET_LANGUAGE_UKRAINIAN,
#endif
#if defined(IM_YORUBA_SUPPORT)
	MMISET_LANGUAGE_YORUBA,
#endif
#if defined(IM_XHOSA_SUPPORT)
	MMISET_LANGUAGE_XHOSA,
#endif
#if defined(IM_ZULU_SUPPORT)
	MMISET_LANGUAGE_ZULU,
#endif
#if defined(IM_SWAHILI_SUPPORT)
	MMISET_LANGUAGE_SWAHILI,
#endif
	MMISET_MAX_LANGUAGE
};

et_font et_font_latin_full = {
	NULL, 0, latin_full_supportLanguages
};

/*#if (defined(IM_ENGLISH_SUPPORT) \
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
    || defined(IM_SWAHILI_SUPPORT) )*/
#if !defined MMI_FONT_LATIN_BASIC
#define LATIN_FULL_FACE_MEM_SIZE  12*1024
#define LATIN_FULL_BASIC_MEM_SIZE  0
#define ET_FONT_LATIN      &et_font_latin_full,
#else
#define LATIN_FULL_FACE_MEM_SIZE  0
#define LATIN_FULL_BASIC_MEM_SIZE  12*1024
#define ET_FONT_LATIN        &et_font_latin_basic,
#endif



/************************************************************************/
/* etrump font engine mem                                               */
/************************************************************************/
#define RASTER_MEM_SIZE 32*1024
#define CACHE_MEM_SIZE  96*1024

#define POOL_SIZE (RASTER_MEM_SIZE +          \
	               CACHE_MEM_SIZE +           \
				   CHINESE_FACE_MEM_SIZE +    \
				   ARABIC_FACE_MEM_SIZE +     \
				   BENGALI_FACE_MEM_SIZE +    \
				   GUJARATI_FACE_MEM_SIZE +   \
				   HINDI_FACE_MEM_SIZE +      \
				   KANNADA_FACE_MEM_SIZE +    \
				   LAO_FACE_MEM_SIZE +        \
				   MALAYALAM_FACE_MEM_SIZE +  \
   				   MYANMAR_FACE_MEM_SIZE +    \
				   TAMIL_FACE_MEM_SIZE +      \
				   TELUGU_FACE_MEM_SIZE +     \
				   THAI_FACE_MEM_SIZE +       \
				   TIBETAN_FACE_MEM_SIZE +    \
				   UYGHUR_FACE_MEM_SIZE +     \
				   KHMER_FACE_MEM_SIZE +      \
				   SINHALA_FACE_MEM_SIZE +      \
                   LATIN_FULL_FACE_MEM_SIZE   +\
                   LATIN_FULL_BASIC_MEM_SIZE)



/************************************************************************/
/* et_font                                                              */
/************************************************************************/
et_font* et_font_list[] =
{
	ET_FONT_CHINESE
	ET_FONT_LATIN
	ET_FONT_ARABIC
	ET_FONT_BENGALI
	ET_FONT_GUJARATI
	ET_FONT_HINDI
	ET_FONT_KANNADA
	ET_FONT_LAO
	ET_FONT_MALAYALAM
	ET_FONT_MYANMAR
	ET_FONT_TAMIL
	ET_FONT_TELUGU
	ET_FONT_THAI
	ET_FONT_TIBETAN
	ET_FONT_UYGHUR
	ET_FONT_KHMER
	ET_FONT_SINHALA
	NULL
};

#if ( defined IM_BENGALI_SUPPORT \
    || defined  IM_HINDI_SUPPORT \
    || defined  IM_DEVANAGARI_SUPPORT \
    || defined  IM_GUJARATI_SUPPORT \
    || defined  IM_GURMUKHI_SUPPORT \
    || defined  IM_KANNADA_SUPPORT \
    || defined  IM_MALAYALAM_SUPPORT \
    || defined  IM_ORIYA_SUPPORT \
    || defined  IM_TAMIL_SUPPORT \
    || defined  IM_TELUGU_SUPPORT \
    || defined  IM_SINHALA_SUPPORT \
    || defined  IM_ARABIC_SUPPORT \
    || defined  IM_HEBREW_SUPPORT \
    || defined  IM_THAI_SUPPORT \
    || defined  IM_KHMER_SUPPORT \
    || defined  IM_TIBETAN_SUPPORT  )
#define NOT_PERMIT_GLYPH_SPACE
#endif

#endif  // _ET_FONT_H_
