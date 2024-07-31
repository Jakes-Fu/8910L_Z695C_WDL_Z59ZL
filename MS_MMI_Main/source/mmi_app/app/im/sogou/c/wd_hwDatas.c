/*****************************************************************************
** File Name:      wd_hwDatas.c                                              *
** Author:                                                                   *
** Date:           12/2008                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************/
//#ifdef MULTI_LAN_SUPPORT_SOGOU

#include "wd_hwdef.h"
//#include "gui_multilanguage.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define SG_NULL 0
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

#if 0 /* unused variable */
extern WD_HwData* HwImeData;

extern const WD_HwLetter* useData;
extern  WD_UINT8* useDataPassword;
extern WD_UINT16 WD_useDatacount;
#endif

/* Number */
#ifdef CUSTOMER_HW_NUMBER
extern const WD_HwLetter NumberData[];
extern WD_UINT16 NumberDatacount;
extern WD_UINT8 Number_HW_password[];
#endif

/* English */
#ifdef CUSTOMER_HW_ENGLISH
extern const WD_HwLetter EnglishData[];
extern WD_UINT16 EnglishDatacount;
extern WD_UINT8 English_HW_password[];
#endif

/* English_Uppercase */
#ifdef CUSTOMER_HW_ENGLISH_UPPERCASE
extern const WD_HwLetter English_UppercaseData[];
extern WD_UINT16 English_UppercaseDatacount;
extern WD_UINT8 English_Uppercase_HW_password[];
#endif

/* English_Lowercase */
#ifdef CUSTOMER_HW_ENGLISH_LOWERCASE
extern const WD_HwLetter English_LowercaseData[];
extern WD_UINT16 English_LowercaseDatacount;
extern WD_UINT8 English_Lowercase_HW_password[];
#endif

/* Bulgarian */
#ifdef CUSTOMER_HW_BULGARIAN
extern const WD_HwLetter BulgarianData[];
extern WD_UINT16 BulgarianDatacount;
extern WD_UINT8 Bulgarian_HW_password[];
#endif

/* Bulgarian_Uppercase */
#ifdef CUSTOMER_HW_BULGARIAN_UPPERCASE
extern const WD_HwLetter Bulgarian_UppercaseData[];
extern WD_UINT16 Bulgarian_UppercaseDatacount;
extern WD_UINT8 Bulgarian_Uppercase_HW_password[];
#endif

/* Bulgarian_Lowercase */
#ifdef CUSTOMER_HW_BULGARIAN_LOWERCASE
extern const WD_HwLetter Bulgarian_LowercaseData[];
extern WD_UINT16 Bulgarian_LowercaseDatacount;
extern WD_UINT8 Bulgarian_Lowercase_HW_password[];
#endif

/* Croatian */
#ifdef CUSTOMER_HW_CROATIAN 
extern const WD_HwLetter CroatianData[];
extern WD_UINT16 CroatianDatacount;
extern WD_UINT8 Croatian_HW_password[];
#endif

/* Croatian_Uppercase */
#ifdef CUSTOMER_HW_CROATIAN_UPPERCASE 
extern const WD_HwLetter Croatian_UppercaseData[];
extern WD_UINT16 Croatian_UppercaseDatacount;
extern WD_UINT8 Croatian_Uppercase_HW_password[];
#endif

/* Croatian_Lowercase */
#ifdef CUSTOMER_HW_CROATIAN_LOWERCASE
extern const WD_HwLetter Croatian_LowercaseData[];
extern WD_UINT16 Croatian_LowercaseDatacount;
extern WD_UINT8 Croatian_Lowercase_HW_password[];
#endif

/* Estonian */
#ifdef CUSTOMER_HW_ESTONIAN 
extern const WD_HwLetter EstonianData[];
extern WD_UINT16 EstonianDatacount;
extern WD_UINT8 Estonian_HW_password[];
#endif

/* Estonian_Uppercase */
#ifdef CUSTOMER_HW_ESTONIAN_UPPERCASE
extern const WD_HwLetter Estonian_UppercaseData[];
extern WD_UINT16 Estonian_UppercaseDatacount;
extern WD_UINT8 Estonian_Uppercase_HW_password[];
#endif

/* Estonian_Lowercase */
#ifdef CUSTOMER_HW_ESTONIAN_LOWERCASE
extern const WD_HwLetter Estonian_LowercaseData[];
extern WD_UINT16 Estonian_LowercaseDatacount;
extern WD_UINT8 Estonian_Lowercase_HW_password[];
#endif

/* Icelandic */
#ifdef CUSTOMER_HW_ICELANDIC
extern const WD_HwLetter IcelandicData[];
extern WD_UINT16 IcelandicDatacount;
extern WD_UINT8 Icelandic_HW_password[];
#endif

/* Icelandic_Uppercase */
#ifdef CUSTOMER_HW_ICELANDIC_UPPERCASE
extern const WD_HwLetter Icelandic_UppercaseData[];
extern WD_UINT16 Icelandic_UppercaseDatacount;
extern WD_UINT8 Icelandic_Uppercase_HW_password[];
#endif

/* Icelandic_Lowercase */
#ifdef CUSTOMER_HW_ICELANDIC_LOWERCASE
extern const WD_HwLetter Icelandic_LowercaseData[];
extern WD_UINT16 Icelandic_LowercaseDatacount;
extern WD_UINT8 Icelandic_Lowercase_HW_password[];
#endif

/* Igbo */
#ifdef CUSTOMER_HW_IGBO
extern const WD_HwLetter IgboData[];
extern WD_UINT16 IgboDatacount;
extern WD_UINT8 IgboData_HW_password[];
#endif

/* Igbo_Uppercase */
#ifdef CUSTOMER_HW_IGBO_UPPERCASE
extern const WD_HwLetter Igbo_UppercaseData[];
extern WD_UINT16 Igbo_UppercaseDatacount;
extern WD_UINT8 Igbo_Uppercase_HW_password[];
#endif

/* Igbo_Lowercase */
#ifdef CUSTOMER_HW_IGBO_LOWERCASE
extern const WD_HwLetter Igbo_LowercaseData[];
extern WD_UINT16 Igbo_LowercaseDatacount;
extern WD_UINT8 Igbo_Lowercase_HW_password[];
#endif

/* Latvian */
#ifdef CUSTOMER_HW_LATVIAN
extern const WD_HwLetter LatvianData[];
extern WD_UINT16 LatvianDatacount;
extern WD_UINT8 Latvian_HW_password[];
#endif

/* Latvian_Uppercase */
#ifdef CUSTOMER_HW_LATVIAN_UPPERCASE
extern const WD_HwLetter Latvian_UppercaseData[];
extern WD_UINT16 Latvian_UppercaseDatacount;
extern WD_UINT8 Latvian_Uppercase_HW_password[];
#endif

/* Latvian_Lowercase */
#ifdef CUSTOMER_HW_LATVIAN_LOWERCASE
extern const WD_HwLetter Latvian_LowercaseData[];
extern WD_UINT16 Latvian_LowercaseDatacount;
extern WD_UINT8 Latvian_Lowercase_HW_password[];
#endif

/* Polish */
#ifdef CUSTOMER_HW_POLISH
extern const WD_HwLetter PolishData[];
extern WD_UINT16 PolishDatacount;
extern WD_UINT8 Polish_HW_password[];
#endif

/* Polish_Uppercase */
#ifdef CUSTOMER_HW_POLISH_UPPERCASE
extern const WD_HwLetter Polish_UppercaseData[];
extern WD_UINT16 Polish_UppercaseDatacount;
extern WD_UINT8 Polish_Uppercase_HW_password[];
#endif

/* Polish_Lowercase */
#ifdef CUSTOMER_HW_POLISH_LOWERCASE
extern const WD_HwLetter Polish_LowercaseData[];
extern WD_UINT16 Polish_LowercaseDatacount;
extern WD_UINT8 Polish_Lowercase_HW_password[];
#endif

/* Romanian */
#ifdef CUSTOMER_HW_ROMANIAN
extern const WD_HwLetter RomanianData[];
extern WD_UINT16 RomanianDatacount;
extern WD_UINT8 Romanian_HW_password[];
#endif

/* Romanian_Uppercase */
#ifdef CUSTOMER_HW_ROMANIAN_UPPERCASE
extern const WD_HwLetter Romanian_UppercaseData[];
extern WD_UINT16 Romanian_UppercaseDatacount;
extern WD_UINT8 Romanian_Uppercase_HW_password[];
#endif

/* Romanian_Lowercase */
#ifdef CUSTOMER_HW_ROMANIAN_LOWERCASE
extern const WD_HwLetter Romanian_LowercaseData[];
extern WD_UINT16 Romanian_LowercaseDatacount;
extern WD_UINT8 Romanian_Lowercase_HW_password[];
#endif

/* Serbian */
#ifdef CUSTOMER_HW_SERBIAN
extern const WD_HwLetter SerbianData[];
extern WD_UINT16 SerbianDatacount;
extern WD_UINT8 Serbian_HW_password[];
#endif

/* Serbian_Uppercase */
#ifdef CUSTOMER_HW_SERBIAN_UPPERCASE
extern const WD_HwLetter Serbian_UppercaseData[];
extern WD_UINT16 Serbian_UppercaseDatacount;
extern WD_UINT8 Serbian_Uppercase_HW_password[];
#endif

/* Serbian_Lowercase */
#ifdef CUSTOMER_HW_SERBIAN_LOWERCASE
extern const WD_HwLetter Serbian_LowercaseData[];
extern WD_UINT16 Serbian_LowercaseDatacount;
extern WD_UINT8 Serbian_Lowercase_HW_password[];
#endif

/* Slovak */
#ifdef CUSTOMER_HW_SLOVAK
extern const WD_HwLetter SlovakData[];
extern WD_UINT16 SlovakDatacount;
extern WD_UINT8 Slovak_HW_password[];
#endif

/* Slovak_Uppercase */
#ifdef CUSTOMER_HW_SLOVAK_UPPERCASE
extern const WD_HwLetter Slovak_UppercaseData[];
extern WD_UINT16 Slovak_UppercaseDatacount;
extern WD_UINT8 Slovak_Uppercase_HW_password[];
#endif

/* Slovak_Lowercase */
#ifdef CUSTOMER_HW_SLOVAK_LOWERCASE
extern const WD_HwLetter Slovak_LowercaseData[];
extern WD_UINT16 Slovak_LowercaseDatacount;
extern WD_UINT8 Slovak_Lowercase_HW_password[];
#endif

/* Slovenian */
#ifdef CUSTOMER_HW_SLOVENIAN
extern const WD_HwLetter SlovenianData[];
extern WD_UINT16 SlovenianDatacount;
extern WD_UINT8 Slovenian_HW_password[];
#endif

/* Slovenian_Uppercase */
#ifdef CUSTOMER_HW_SLOVENIAN_UPPERCASE
extern const WD_HwLetter Slovenian_UppercaseData[];
extern WD_UINT16 Slovenian_UppercaseDatacount;
extern WD_UINT8 Slovenian_Uppercase_HW_password[];
#endif

/* Slovenian_Lowercase */
#ifdef CUSTOMER_HW_SLOVENIAN_LOWERCASE
extern const WD_HwLetter Slovenian_LowercaseData[];
extern WD_UINT16 Slovenian_LowercaseDatacount;
extern WD_UINT8 Slovenian_Lowercase_HW_password[];
#endif

/* Swahili */
#ifdef CUSTOMER_HW_SWAHILI
extern const WD_HwLetter SwahiliData[];
extern WD_UINT16 SwahiliDatacount;
extern WD_UINT8 Swahili_HW_password[];
#endif

/* Swahili_Uppercase */
#ifdef CUSTOMER_HW_SWAHILI_UPPERCASE
extern const WD_HwLetter Swahili_UppercaseData[];
extern WD_UINT16 Swahili_UppercaseDatacount;
extern WD_UINT8 Swahili_Uppercase_HW_password[];
#endif

/* Swahili_Lowercase */
#ifdef CUSTOMER_HW_SWAHILI_LOWERCASE
extern const WD_HwLetter Swahili_LowercaseData[];
extern WD_UINT16 Swahili_LowercaseDatacount;
extern WD_UINT8 Swahili_Lowercase_HW_password[];
#endif

/* Turkmen */
#ifdef CUSTOMER_HW_TURKMEN
extern const WD_HwLetter TurkmenData[];
extern WD_UINT16 TurkmenDatacount;
extern WD_UINT8 Turkmen_HW_password[];
#endif

/* Swahili_Uppercase */
#ifdef CUSTOMER_HW_TURKMEN_UPPERCASE
extern const WD_HwLetter Turkmen_UppercaseData[];
extern WD_UINT16 Turkmen_UppercaseDatacount;
extern WD_UINT8 Turkmen_Uppercase_HW_password[];
#endif
 
/* Swahili_Lowercase */
#ifdef CUSTOMER_HW_TURKMEN_LOWERCASE
extern const WD_HwLetter Turkmen_LowercaseData[];
extern WD_UINT16 Turkmen_LowercaseDatacount;
extern WD_UINT8 Turkmen_Lowercase_HW_password[];
#endif
/* Arabic */
#ifdef CUSTOMER_HW_ARABIC
extern const WD_HwLetter ArabicData[];
extern WD_UINT16 ArabicDatacount;
extern WD_UINT8 Arabic_HW_password[];
#endif

/* Czech */
#ifdef CUSTOMER_HW_CZECH
extern const WD_HwLetter CzechData[];
extern WD_UINT16 CzechDatacount;
extern WD_UINT8 Czech_HW_password[];
#endif

/* Czech_Uppercase */
#ifdef CUSTOMER_HW_CZECH_UPPERCASE
extern const WD_HwLetter Czech_UppercaseData[];
extern WD_UINT16 Czech_UppercaseDatacount;
extern WD_UINT8 Czech_Uppercase_HW_password[];
#endif

/* Czech_Lowercase */
#ifdef CUSTOMER_HW_CZECH_LOWERCASE
extern const WD_HwLetter Czech_LowercaseData[];
extern WD_UINT16 Czech_LowercaseDatacount;
extern WD_UINT8 Czech_Lowercase_HW_password[];
#endif

/* Danish */
#ifdef CUSTOMER_HW_DANISH
extern const WD_HwLetter DanishData[];
extern WD_UINT16 DanishDatacount;
extern WD_UINT8 Danish_HW_password[];
#endif

/* Danish_Uppercase */
#ifdef CUSTOMER_HW_DANISH_UPPERCASE
extern const WD_HwLetter Danish_UppercaseData[];
extern WD_UINT16 Danish_UppercaseDatacount;
extern WD_UINT8 Danish_Uppercase_HW_password[];
#endif

/* Danish_Lowercase */
#ifdef CUSTOMER_HW_DANISH_LOWERCASE
extern const WD_HwLetter Danish_LowercaseData[];
extern WD_UINT16 Danish_LowercaseDatacount;
extern WD_UINT8 Danish_Lowercase_HW_password[];
#endif

/* Dutch */
#ifdef CUSTOMER_HW_DUTCH
extern const WD_HwLetter DutchData[];
extern WD_UINT16 DutchDatacount;
extern WD_UINT8 Dutch_HW_password[];
#endif

/* Dutch_Uppercase */
#ifdef CUSTOMER_HW_DUTCH_UPPERCASE
extern const WD_HwLetter Dutch_UppercaseData[];
extern WD_UINT16 Dutch_UppercaseDatacount;
extern WD_UINT8 Dutch_Uppercase_HW_password[];
#endif

/* Dutch_Lowercase */
#ifdef CUSTOMER_HW_DUTCH_LOWERCASE
extern const WD_HwLetter Dutch_LowercaseData[];
extern WD_UINT16 Dutch_LowercaseDatacount;
extern WD_UINT8 Dutch_Lowercase_HW_password[];
#endif

/* Finnish */
#ifdef CUSTOMER_HW_FINNISH
extern const WD_HwLetter FinnishData[];
extern WD_UINT16 FinnishDatacount;
extern WD_UINT8 Finnish_HW_password[];
#endif

/* French */
#ifdef CUSTOMER_HW_FRENCH
extern const WD_HwLetter FrenchData[];
extern WD_UINT16 FrenchDatacount;
extern WD_UINT8 French_HW_password[];
#endif

/* French_Uppercase */
#ifdef CUSTOMER_HW_FRENCH_UPPERCASE
extern const WD_HwLetter French_UppercaseData[];
extern WD_UINT16 French_UppercaseDatacount;
extern WD_UINT8 French_Uppercase_HW_password[];
#endif

/* French_Lowercase */
#ifdef CUSTOMER_HW_FRENCH_LOWERCASE
extern const WD_HwLetter French_LowercaseData[];
extern WD_UINT16 French_LowercaseDatacount;
extern WD_UINT8 French_Lowercase_HW_password[];
#endif

/*Ukrainian*/
#ifdef CUSTOMER_HW_UKRAINIAN
extern const WD_HwLetter UkrainianData[];
extern WD_UINT16 UkrainianDatacount;
extern WD_UINT8 Ukrainian_HW_password[];
#endif 

/*BELARUSIAN*/
#ifdef CUSTOMER_HW_BELARUSIAN
extern const WD_HwLetter BelarusianData[];
extern WD_UINT16 BelarusianDatacount;
extern WD_UINT8 Belarusian_HW_password[];
#endif 
/* German */
#ifdef CUSTOMER_HW_GERMAN
extern const WD_HwLetter GermanData[];
extern WD_UINT16 GermanDatacount;
extern WD_UINT8 German_HW_password[];
#endif

/* German_Uppercase */
#ifdef CUSTOMER_HW_GERMAN_UPPERCASE
extern const WD_HwLetter German_UppercaseData[];
extern WD_UINT16 German_UppercaseDatacount;
extern WD_UINT8 German_Uppercase_HW_password[];
#endif

/* German_Lowercase */
#ifdef CUSTOMER_HW_GERMAN_LOWERCASE
extern const WD_HwLetter German_LowercaseData[];
extern WD_UINT16 German_LowercaseDatacount;
extern WD_UINT8 German_Lowercase_HW_password[];
#endif

/* Hindi */
#ifdef CUSTOMER_HW_HINDI
extern const WD_HwLetter HindiData[];
extern WD_UINT16 HindiDatacount;
extern WD_UINT8 Hindi_HW_password[];
#endif

/* Hebrew */
#ifdef CUSTOMER_HW_HEBREW
extern const WD_HwLetter HebrewData[];
extern WD_UINT16 HebrewDatacount;
extern WD_UINT8 Hebrew_HW_password[];
#endif
/* Hungarian */
#ifdef CUSTOMER_HW_HUNGARIAN
extern const WD_HwLetter HungarianData[];
extern WD_UINT16 HungarianDatacount;
extern WD_UINT8 Hungarian_HW_password[];
#endif

/*Ukrainian*/
#ifdef CUSTOMER_HW_GREEK
extern const WD_HwLetter GreekData[];
extern WD_UINT16 GreekDatacount;
extern WD_UINT8 Greek_HW_password[];
#endif


/* Indonesian */
#ifdef CUSTOMER_HW_INDONESIAN
extern const WD_HwLetter IndonesianData[];
extern WD_UINT16 IndonesianDatacount;
extern WD_UINT8 Indonesian_HW_password[];
#endif

/* Indonesian_Uppercase */
#ifdef CUSTOMER_HW_INDONESIAN_UPPERCASE
extern const WD_HwLetter Indonesian_UppercaseData[];
extern WD_UINT16 Indonesian_UppercaseDatacount;
extern WD_UINT8 Indonesian_Uppercase_HW_password[];
#endif

/* Indonesian_Lowercase */
#ifdef CUSTOMER_HW_INDONESIAN_LOWERCASE
extern const WD_HwLetter Indonesian_LowercaseData[];
extern WD_UINT16 Indonesian_LowercaseDatacount;
extern WD_UINT8 Indonesian_Lowercase_HW_password[];
#endif

/* Italian */
#ifdef CUSTOMER_HW_ITALIAN
extern const WD_HwLetter ItalianData[];
extern WD_UINT16 ItalianDatacount;
extern WD_UINT8 Italian_HW_password[];
#endif

/* Japanese */
#ifdef CUSTOMER_HW_JAPANESE
extern const WD_HwLetter JapaneseData[];
extern WD_UINT16 JapaneseDatacount;
extern WD_UINT8 Japanese_HW_password[];
#endif

/* Kazak */
#ifdef CUSTOMER_HW_KAZAK
extern const WD_HwLetter KazakData[];
extern WD_UINT16 KazakDatacount;
extern WD_UINT8 Kazak_HW_password[];
#endif

/* Korean */
#ifdef CUSTOMER_HW_KOREAN
extern const WD_HwLetter KoreanData[];
extern WD_UINT16 KoreanDatacount;
extern WD_UINT8 Korean_HW_password[];
#endif

/* Malay */
#ifdef CUSTOMER_HW_MALAY
extern const WD_HwLetter MalayData[];
extern WD_UINT16 MalayDatacount;
extern WD_UINT8 Malay_HW_password[];
#endif

/* Malay_Uppercase */
#ifdef CUSTOMER_HW_MALAY_UPPERCASE
extern const WD_HwLetter Malay_UppercaseData[];
extern WD_UINT16 Malay_UppercaseDatacount;
extern WD_UINT8 Malay_Uppercase_HW_password[];
#endif

/* Malay_Lowercase */
#ifdef CUSTOMER_HW_MALAY_LOWERCASE
extern const WD_HwLetter Malay_LowercaseData[];
extern WD_UINT16 Malay_LowercaseDatacount;
extern WD_UINT8 Malay_Lowercase_HW_password[];
#endif

/* Mongolian */
#ifdef CUSTOMER_HW_MONGOLIAN
extern const WD_HwLetter MongolianData[];
extern WD_UINT16 MongolianDatacount;
extern WD_UINT8 Mongolian_HW_password[];
#endif

/* Norwegian */
#ifdef CUSTOMER_HW_NORWEGIAN
extern const WD_HwLetter NorwegianData[];
extern WD_UINT16 NorwegianDatacount;
extern WD_UINT8 Norwegian_HW_password[];
#endif

/* Persian */
#ifdef CUSTOMER_HW_PERSIAN
extern const WD_HwLetter PersianData[];
extern WD_UINT16 PersianDatacount;
extern WD_UINT8 Persian_HW_password[];
#endif

/* Portuguese */
#ifdef CUSTOMER_HW_PORTUGUESE
extern const WD_HwLetter PortugueseData[];
extern WD_UINT16 PortugueseDatacount;
extern WD_UINT8 Portuguese_HW_password[];
#endif

/* Portuguese_Uppercase */
#ifdef CUSTOMER_HW_PORTUGUESE_UPPERCASE
extern const WD_HwLetter Portuguese_UppercaseData[];
extern WD_UINT16 Portuguese_UppercaseDatacount;
extern WD_UINT8 Portuguese_HW_password[];
#endif

/* Portuguese_Lowercase */
#ifdef CUSTOMER_HW_PORTUGUESE_LOWERCASE
extern const WD_HwLetter Portuguese_LowercaseData[];
extern WD_UINT16 Portuguese_LowercaseDatacount;
extern WD_UINT8 Portuguese_Lowercase_HW_password[];
#endif

/* Russian */
#ifdef CUSTOMER_HW_RUSSIAN
extern const WD_HwLetter RussianData[];
extern WD_UINT16 RussianDatacount ;
extern WD_UINT8 Russian_HW_password[];
#endif

/* Russian_Uppercase */
#ifdef CUSTOMER_HW_RUSSIAN_UPPERCASE
extern const WD_HwLetter Russian_UppercaseData[];
extern WD_UINT16 Russian_UppercaseDatacount ;
extern WD_UINT8 Russian_Uppercase_HW_password[];
#endif

/* Russian_Lowercase */
#ifdef CUSTOMER_HW_RUSSIAN_LOWERCASE
extern const WD_HwLetter Russian_LowercaseData[];
extern WD_UINT16 Russian_LowercaseDatacount ;
extern WD_UINT8 Russian_Lowercase_HW_password[];
#endif

/* Spanish */
#ifdef CUSTOMER_HW_SPANISH
extern const WD_HwLetter SpanishData[];
extern WD_UINT16 SpanishDatacount;
extern WD_UINT8 Spanish_HW_password[];
#endif

/* Spanish_Uppercase */
#ifdef CUSTOMER_HW_SPANISH_UPPERCASE
extern const WD_HwLetter Spanish_UppercaseData[];
extern WD_UINT16 Spanish_UppercaseDatacount;
extern WD_UINT8 Spanish_Uppercase_HW_password[];
#endif

/* Spanish_Lowercase */
#ifdef CUSTOMER_HW_SPANISH_LOWERCASE
extern const WD_HwLetter Spanish_LowercaseData[];
extern WD_UINT16 Spanish_LowercaseDatacount;
extern WD_UINT8 Spanish_Lowercase_HW_password[];
#endif

/* Swedish */
#ifdef CUSTOMER_HW_SWEDISH
extern const WD_HwLetter SwedishData[];
extern WD_UINT16 SwedishDatacount;
extern WD_UINT8 Swedish_HW_password[];
#endif

/* Thai */
#ifdef CUSTOMER_HW_THAI
extern const WD_HwLetter ThaiData[];
extern WD_UINT16 ThaiDatacount;
extern WD_UINT8 Thai_HW_password[];
#endif

/* Tibetan */
#ifdef CUSTOMER_HW_TIBETAN
extern const WD_HwLetter TibetanData[];
extern WD_UINT16 TibetanDatacount;
extern WD_UINT8 Tibetan_HW_password[];
#endif

/* Mongolian_Traditional */
#ifdef CUSTOMER_HW_MONGOLIAN_TRADITIONAL
extern const WD_HwLetter Mongolian_TraditionalData[];
extern WD_UINT16 Mongolian_TraditionalDatacount;
extern WD_UINT8 Mongolian_Traditional_HW_password[];
#endif

/* Turkish */
#ifdef CUSTOMER_HW_TURKISH
extern const WD_HwLetter TurkishData[];
extern WD_UINT16 TurkishDatacount;
extern WD_UINT8 Turkish_HW_password[];
#endif

/* Uighur */
#ifdef CUSTOMER_HW_UIGHUR
extern const WD_HwLetter UighurData[];
extern WD_UINT16 UighurDatacount;
extern WD_UINT8 Uighur_HW_password[];
#endif

/* Ukrainian */
#ifdef CUSTOMER_HW_UKRAINIAN
extern const WD_HwLetter UkrainianData[];
extern WD_UINT16 UkrainianDatacount;
extern WD_UINT8 Ukrainian_HW_password[];
#endif

/* Urdu */
#ifdef CUSTOMER_HW_URDU//CUSTOMER_HW_URDU
extern const WD_HwLetter UrduData[];
extern WD_UINT16 UrduDatacount;
extern WD_UINT8 Urdu_HW_password[];
#endif

/* Vietnamesenamese */
#ifdef CUSTOMER_HW_VIETNAMESE//CUSTOMER_HW_VIETNAMESE
extern const WD_HwLetter VietnameseData[];
extern WD_UINT16 VietnameseDatacount;
extern WD_UINT8 Vietnamese_HW_password[];
#endif

/* Vietnamesenamese */
#ifdef CUSTOMER_HW_VIETNAMESE_UPPERCASE
extern const WD_HwLetter Vietnamese_UppercaseData[];
extern WD_UINT16 Vietnamese_UppercaseDatacount;
extern WD_UINT8 Vietnamese_Uppercase_HW_password[];
#endif

/* Vietnamesenamese */
#ifdef CUSTOMER_HW_VIETNAMESE_LOWERCASE
extern const WD_HwLetter Vietnamese_LowercaseData[];
extern WD_UINT16 Vietnamese_LowercaseDatacount;
extern WD_UINT8 Vietnamese_Lowercase_HW_password[];
#endif
#ifdef CUSTOMER_HW_BOSNIAN
extern const WD_HwLetter BosnianData[];
extern WD_UINT16 BosnianDatacount;
extern WD_UINT8 Bosnian_HW_password[];
#endif

/*Tagalog*/
#ifdef CUSTOMER_HW_TAGALOG//CUSTOMER_HW_TAGALOG
extern const WD_HwLetter TagalogData[];
extern WD_UINT16 TagalogDatacount;
extern WD_UINT8 Tagalog_HW_password[];
#endif
#ifdef CUSTOMER_HW_KAZAKH
extern const WD_HwLetter KazakhData[];
extern WD_UINT16 KazakhDatacount;
extern WD_UINT8 Kazakh_HW_password[];
#endif
#ifdef CUSTOMER_HW_TELUGU
extern const WD_HwLetter TeluguData[];
extern WD_UINT16 TeluguDatacount;
extern WD_UINT8 Telugu_HW_password[];
#endif
#ifdef CUSTOMER_HW_BURMESE
extern const WD_HwLetter BurmeseData[];
extern WD_UINT16 BurmeseDatacount;
extern WD_UINT8 Burmese_HW_password[];
#endif
#ifdef CUSTOMER_HW_TAMIL
extern const WD_HwLetter TamilData[];
extern WD_UINT16 TamilDatacount;
extern WD_UINT8 Tamil_HW_password[];
#endif

#ifdef CUSTOMER_HW_BENGALI
extern const WD_HwLetter   BengaliData[];
extern WD_UINT16   BengaliDatacount;
extern WD_UINT8    Bengali_HW_password[];
#endif
WD_Data_Struct wd_datas[] =
{
{
	#ifdef CUSTOMER_HW_NUMBER
		NumberData,
		&NumberDatacount,
		Number_HW_password,
	#else
		SG_NULL,
		0,
		SG_NULL,
	#endif
},  //0
{
	#ifdef CUSTOMER_HW_NUMBER
	NumberData,
	&NumberDatacount,
	Number_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//1

{
	#ifdef CUSTOMER_HW_HW_GESTURE//wanghui¸Ä·ñ?
	GestureData,
	&GestureDatacount,
	Gesture_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//2

{
	#ifdef CUSTOMER_HW_FULL_PUNCTUATION
	Full_PunctuationData,
	&Full_PunctuationDatacount,
	Full_Punctuation_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//3

{
	#ifdef CUSTOMER_HW_PUNCTUATION
	PunctuationData,
	&PunctuationDatacount,
	Punctuation_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//4

{
	#ifdef CUSTOMER_HW_CHINESE_SIMPLIFIED
	Chinese_SimplifiedData,
	&Chinese_SimplifiedDatacount,
	Chinese_Simplified_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//5

{
	#ifdef CUSTOMER_HW_CHINESE_TRADITIONAL
	Chinese_TraditionalData,
	&Chinese_TraditionalDatacount,
	Chinese_Traditional_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//6

{
	#ifdef CUSTOMER_HW_DANISH_JAPANESE
	Danish_JananeseData,
	&Danish_JananeseDatacount,
	Danish_Jananese_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//7

{
	#ifdef CUSTOMER_HW_HIRAGANA
	HiraganaData,
	&HiraganaDatacount,
	Hiragana_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//8

{
	#ifdef CUSTOMER_HW_KATAKANA
	KatakanaData,
	&KatakanaDatacount,
	Katakana_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//9

{
	#ifdef CUSTOMER_HW_KOREA
	KoreaData,
	&KoreaDatacount,
	Korea_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//10

{
	#ifdef CUSTOMER_HW_THAI
	ThaiData,
	&ThaiDatacount,
	Thai_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//11

{
	#ifdef CUSTOMER_HW_HINDI
	HindiData,
	&HindiDatacount,
	Hindi_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//12

{
	#ifdef CUSTOMER_HW_HEBREW
	HebrewData,
	&HebrewDatacount,
	Hebrew_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//13

{
	#ifdef CUSTOMER_HW_KHMER
	KhmerData,
	&KhmerDatacount,
	Khmer_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//14

{
	#ifdef CUSTOMER_HW_NEPALI
	NepaliData,
	&NepaliDatacount,
	Nepali_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//15

{
	#ifdef CUSTOMER_HW_TAMIL
	TamilData,
	&TamilDatacount,
	Tamil_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//16

{
	#ifdef CUSTOMER_HW_TELUGU
	TeluguData,
	&TeluguDatacount,
	Telugu_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//17

{
	#ifdef CUSTOMER_HW_BURMESE
	BurmeseData,
	&BurmeseDatacount,
	Burmese_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//18

{
	#ifdef CUSTOMER_HW_CHOSEN
	ChosenData,
	&ChosenDatacount,
	Chosen_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//19

{
	#ifdef CUSTOMER_HW_PUNJABI
	PunjabiData,
	&PunjabiDatacount,
	Punjabi_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//20

{
	#ifdef CUSTOMER_HW_MARATHI
	MarathiData,
	&MarathiDatacount,
	Marathi_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//21

{
	#ifdef CUSTOMER_HW_BENGALI
	BengaliData,
	&BengaliDatacount,
	Bengali_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//22

{
	#ifdef CUSTOMER_HW_SINHALESE
	SinhaleseData,
	&SinhaleseDatacount,
	Sinhalese_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//23

{
	#ifdef CUSTOMER_HW_PUSHTO
	PushtoData,
	&PushtoDatacount,
	Pushto_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//24

{
	#ifdef CUSTOMER_HW_ARABIC
	ArabicData,
	&ArabicDatacount,
	Arabic_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//25

{
	#ifdef CUSTOMER_HW_PERSIAN
	PersianData,
	&PersianDatacount,
	Persian_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//26

{
	#ifdef CUSTOMER_HW_URDU
	UrduData,
	&UrduDatacount,
	Urdu_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//27

{
	#ifdef CUSTOMER_HW_LAO
	LaoData,
	&LaoDatacount,
	Lao_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//28

{
	#ifdef CUSTOMER_HW_UIGHUR
	UighurData,
	&UighurDatacount,
	Uighur_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//29

{
	#ifdef CUSTOMER_HW_GREEK_UPPERCASE
	Greek_UppercaseData,
	&Greek_UppercaseDatacount,
	Greek_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//30

{
	#ifdef CUSTOMER_HW_GREEK_LOWERCASE
	Greek_LowercaseData,
	&Greek_LowercaseDatacount,
	Greek_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//31

{
	#ifdef CUSTOMER_HW_GREEK
	GreekData,
	&GreekDatacount,
	Greek_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//32

{
	#ifdef CUSTOMER_HW_INDONESIAN_UPPERCASE
	Indonesian_UppercaseData,
	&Indonesian_UppercaseDatacount,
	Indonesian_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//33

{
	#ifdef CUSTOMER_HW_INDONESIAN_LOWERCASE
	Indonesian_LowercaseData,
	&Indonesian_LowercaseDatacount,
	Indonesian_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//34

{
	#ifdef CUSTOMER_HW_INDONESIAN
	IndonesianData,
	&IndonesianDatacount,
	Indonesian_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//35

{
	#ifdef CUSTOMER_HW_MALAY_UPPERCASE
	Malay_UppercaseData,
	&Malay_UppercaseDatacount,
	Malay_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//36

{
	#ifdef CUSTOMER_HW_MALAY_LOWERCASE
	Malay_LowercaseData,
	&Malay_LowercaseDatacount,
	Malay_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//37

{
	#ifdef CUSTOMER_HW_MALAY
	MalayData,
	&MalayDatacount,
	Malay_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//38

{
	#ifdef CUSTOMER_HW_TAGALOG_UPPERCASE
	Tagalog_UppercaseData,
	&Tagalog_UppercaseDatacount,
	Tagalog_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//39

{
	#ifdef CUSTOMER_HW_TAGALOG_LOWERCASE
	Tagalog_LowercaseData,
	&Tagalog_LowercaseDatacount,
	Tagalog_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//40

{
	#ifdef CUSTOMER_HW_TAGALOG
	TagalogData,
	&TagalogDatacount,
	Tagalog_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//41

{
	#ifdef CUSTOMER_HW_VIETNAMESE_UPPERCASE
	Vietnamese_UppercaseData,
	&Vietnamese_UppercaseDatacount,
	Vietnamese_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//42

{
	#ifdef CUSTOMER_HW_VIETNAMESE_LOWERCASE
	Vietnamese_LowercaseData,
	&Vietnamese_LowercaseDatacount,
	Vietnamese_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//43

{
	#ifdef CUSTOMER_HW_VIETNAMESE
	VietnameseData,
	&VietnameseDatacount,
	Vietnamese_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//44

{
	#ifdef CUSTOMER_HW_MONGOLIAN
	MongolianData,
	&MongolianDatacount,
	Mongolian_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//45

{
	#ifdef CUSTOMER_HW_MONGOLIAN_UPPERCASE
	Mongolian_UppercaseData,
	&Mongolian_UppercaseDatacount,
	Mongolian_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//46

{
	#ifdef CUSTOMER_HW_MONGOLIAN_LOWERCASE
	Mongolian_LowercaseData,
	&Mongolian_LowercaseDatacount,
	Mongolian_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//47

{
#ifdef CUSTOMER_HW_KANNADA                //¿²ÄÇ´ïÓï

	KannadaData,
	&KannadaDataDatacount,
	KannadaData_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//48
{
#ifdef CUSTOMER_HW_MALAYALAM                

	MalayalamData,
	&MalayalamDataDatacount,
	MalayalamData_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//49
{
	#ifdef CUSTOMER_HW_GUJARATI
	GujaratiData,
	&GujaratiDataDatacount,
	GujaratiData_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//50

{
#ifdef CUSTOMER_HW_SINHALA//zhanglu add
	SinhalalamData,
	&SinhalaDataDatacount,
	SinhalaData_HW_password,
#else
	SG_NULL,
	0,
	SG_NULL,
#endif
},

{
	#ifdef CUSTOMER_HW_ENGLISH
	EnglishData,
	&EnglishDatacount,
	English_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//51

{
	#ifdef CUSTOMER_HW_ENGLISH_UPPERCASE
	English_UppercaseData,
	&English_UppercaseDatacount,
	English_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//52

{
	#ifdef CUSTOMER_HW_ENGLISH_LOWERCASE
	English_LowercaseData,
	&English_LowercaseDatacount,
	English_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//53

{
	#ifdef CUSTOMER_HW_FRENCH
	FrenchData,
	&FrenchDatacount,
	French_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//54

{
	#ifdef CUSTOMER_HW_FRENCH_UPPERCASE
	French_UppercaseData,
	&French_UppercaseDatacount,
	French_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//55

{
	#ifdef CUSTOMER_HW_FRENCH_LOWERCASE
	French_LowercaseData,
	&French_LowercaseDatacount,
	French_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif 
},//56

{
	#ifdef CUSTOMER_HW_GERMAN
	GermanData,
	&GermanDatacount,
	German_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//57

{
	#ifdef CUSTOMER_HW_GERMAN_UPPERCASE
	German_UppercaseData,
	&German_UppercaseDatacount,
	German_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//58

{
	#ifdef CUSTOMER_HW_GERMAN_LOWERCASE
	Greman_LowercaseData,
	&Greman_LowercaseDatacount,
	Greman_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//59

{
	#ifdef CUSTOMER_HW_ITALIAN
	ItalianData,
	&ItalianDatacount,
	Italian_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//60

{
	#ifdef CUSTOMER_HW_ITALIAN_UPPERCASE
	Italian_UppercaseData,
	&Italian_UppercaseDatacount,
	Italian_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//61

{
	#ifdef CUSTOMER_HW_ITALIAN_LOWERCASE
	Italian_LowercaseData,
	&Italian_LowercaseDatacount,
	Italian_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//62

{
	#ifdef CUSTOMER_HW_DANISH
	DanishData,
	&DanishDatacount,
	Danish_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//63

{
	#ifdef CUSTOMER_HW_DANISH_UPPERCASE
	Danish_UppercaseData,
	&Danish_UppercaseDatacount,
	Danish_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//64

{
	#ifdef CUSTOMER_HW_DANISH_LOWERCASE
	Danish_LowercaseData,
	&Danish_LowercaseDatacount,
	Danish_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//65

{
	#ifdef CUSTOMER_HW_NORWEGIAN
	NorwegianData,
	&NorwegianDatacount,
	Norwegian_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//66

{
	#ifdef CUSTOMER_HW_NORWEGIAN_UPPERCASE
	Norwegian_UppercaseData,
	&Norwegian_UppercaseDatacount,
	Norwegian_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//67

{
	#ifdef CUSTOMER_HW_NORWEGIAN_LOWERCASE
	Norwegian_LowercaseData,
	&Norwegian_LowercaseDatacount,
	Norwegian_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//68

{
	#ifdef CUSTOMER_HW_SPANISH
	SpanishData,
	&SpanishDatacount,
	Spanish_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//69

{
	#ifdef CUSTOMER_HW_SPANISH_UPPERCASE
	Spanish_UppercaseData,
	&Spanish_UppercaseDatacount,
	Spanish_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//70

{
	#ifdef CUSTOMER_HW_SPANISH_LOWERCASE
	Spanish_LowercaseData,
	&Spanish_LowercaseDatacount,
	Spanish_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//71

{
	#ifdef CUSTOMER_HW_PORTUGUESE
	PortugueseData,
	&PortugueseDatacount,
	Portuguese_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//72

{
	#ifdef CUSTOMER_HW_PORTUGUESE_UPPERCASE
	Portuguese_UppercaseData,
	&Portuguese_UppercaseDatacount,
	Portuguese_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//73

{
	#ifdef CUSTOMER_HW_PORTUGUESE_LOWERCASE
	Portuguese_LowercaseData,
	&Portuguese_LowercaseDatacount,
	Portuguese_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//74

{
	#ifdef CUSTOMER_HW_DUTCH
	DutchData,
	&DutchDatacount,
	Dutch_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//75

{
	#ifdef CUSTOMER_HW_DUTCH_UPPERCASE
	Dutch_UppercaseData,
	&Dutch_UppercaseDatacount,
	Dutch_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//76

{
	#ifdef CUSTOMER_HW_DUTCH_LOWERCASE
	Dotch_LowercaseData,
	&Dotch_LowercaseDatacount,
	Dotch_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//77

{
	#ifdef CUSTOMER_HW_SWEDISH
	SwedishData,
	&SwedishDatacount,
	Swedish_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//78

{
	#ifdef CUSTOMER_HW_SWEDISH_UPPERCASE
	Swedish_UppercaseData,
	&Swedish_UppercaseDatacount,
	Swedish_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//79

{
	#ifdef CUSTOMER_HW_SWEDISH_LOWERCASE
	Swedish_LowercaseData,
	&Swedish_LowercaseDatacount,
	Swedish_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//80

{
	#ifdef CUSTOMER_HW_CZECH
	CzechData,
	&CzechDatacount,
	Czech_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//81

{
	#ifdef CUSTOMER_HW_CZECH_UPPERCASE
	Czech_UppercaseData,
	&Czech_UppercaseDatacount,
	Czech_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//82

{
	#ifdef CUSTOMER_HW_CZECH_LOWERCASE
	Czech_LowercaseData,
	&Czech_LowercaseDatacount,
	Czech_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//83
{
	#ifdef CUSTOMER_HW_TURKISH
	TurkishData,
	&TurkishDatacount,
	Turkish_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//84

{
	#ifdef CUSTOMER_HW_TURKISH_UPPERCASE
	Turkish_UppercaseData,
	&Turkish_UppercaseDatacount,
	Turkish_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//85

{
	#ifdef CUSTOMER_HW_TURKISH_LOWERCASE
	Turkish_LowercaseData,
	&Turkish_LowercaseDatacount,
	Turkish_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//86

{
	#ifdef CUSTOMER_HW_HUNGARIAN
	HungarianData,
	&HungarianDatacount,
	Hungarian_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//87

{
	#ifdef CUSTOMER_HW_HUNGARIAN_UPPERCASE
	Hungarian_UppercaseData,
	&Hungarian_UppercaseDatacount,
	Hungarian_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//88

{
	#ifdef CUSTOMER_HW_HUNGARIAN_LOWERCASE
	Hungarian_LowercaseData,
	&Hungarian_LowercaseDatacount,
	Hungarian_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//89

{
	#ifdef CUSTOMER_HW_FINNISH
	FinnishData,
	&FinnishDatacount,
	Finnish_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//90

{
	#ifdef CUSTOMER_HW_FINNISH_UPPERCASE
	Finnish_UppercaseData,
	&Finnish_UppercaseDatacount,
	Finnish_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//91

{
	#ifdef CUSTOMER_HW_FINNISH_LOWERCASE
	Finnish_LowercaseData,
	&Finnish_LowercaseDatacount,
	Finnish_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//92

{
	#ifdef CUSTOMER_HW_ICELANDIC
	IcelandicData,
	&IcelandicDatacount,
	Icelandic_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//93

{
	#ifdef CUSTOMER_HW_ICELANDIC_UPPERCASE
	Icelandic_UppercaseData,
	&Icelandic_UppercaseDatacount,
	Icelandic_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//94

{
	#ifdef CUSTOMER_HW_ICELANDIC_LOWERCASE
	Icelandic_LowercaseData,
	&Icelandic_LowercaseDatacount,
	Icelandic_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//95

{
	#ifdef CUSTOMER_HW_POLISH
	PolishData,
	&PolishDatacount,
	Polish_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//96

{
	#ifdef CUSTOMER_HW_POLISH_UPPERCASE
	Polish_UppercaseData,
	&Polish_UppercaseDatacount,
	Polish_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//97

{
	#ifdef CUSTOMER_HW_POLISH_LOWERCASE
	Polish_LowercaseData,
	&Polish_LowercaseDatacount,
	Polish_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//98

{
	#ifdef CUSTOMER_HW_SLOVAK
	SlovakData,
	&SlovakDatacount,
	Slovak_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//99

{
	#ifdef CUSTOMER_HW_SLOVAK_UPPERCASE
	Slovak_UppercaseData,
	&Slovak_UppercaseDatacount,
	Slovak_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//100

{
	#ifdef CUSTOMER_HW_SLOVAK_LOWERCASE
	Slovak_LowercaseData,
	&Slovak_LowercaseDatacount,
	Slovak_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//101

{
	#ifdef CUSTOMER_HW_ROMANIAN
	RomanianData,
	&RomanianDatacount,
	Romanian_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//102

{
	#ifdef CUSTOMER_HW_ROMANIAN_UPPERCASE
	Romanian_UppercaseData,
	&Romanian_UppercaseDatacount,
	Romanian_Uppercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//103

{
	#ifdef CUSTOMER_HW_ROMANIAN_LOWERCASE
	Romanian_LowercaseData,
	&Romanian_LowercaseDatacount,
	Romanian_Lowercase_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//104

{
	#ifdef CUSTOMER_HW_SLOVENIAN
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//105

{
	#ifdef CUSTOMER_HW_SLOVENIAN_UPPERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//106

{
	#ifdef CUSTOMER_HW_SLOVENIAN_LOWERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//107

{
	#ifdef CUSTOMER_HW_LATVIAN
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//108

{
	#ifdef CUSTOMER_HW_LATVIAN_UPPERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//109

{
	#ifdef CUSTOMER_HW_LATVIAN_LOWERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//110

{
	#ifdef CUSTOMER_HW_ESTONIAN
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//111

{
	#ifdef CUSTOMER_HW_ESTONIAN_UPPERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//112

{
	#ifdef CUSTOMER_HW_ESTONIAN_LOWERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//113

{
	#ifdef CUSTOMER_HW_LITHUANIAN
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//114

{
	#ifdef CUSTOMER_HW_LITHUANIAN_UPPERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//115

{
	#ifdef CUSTOMER_HW_LITHUANIAN_LOWERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//116

{
	#ifdef CUSTOMER_HW_CROATIAN
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//117

{
	#ifdef CUSTOMER_HW_CROATIAN_UPPERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//118

{
	#ifdef CUSTOMER_HW_CROATIAN_LOWERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//119

{
	#ifdef CUSTOMER_HW_BOSNIAN
	BosnianData,
	&BosnianDatacount,
	SG_NULL,//Bosnian_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//120

{
	#ifdef CUSTOMER_HW_BOSNIAN_UPPERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//121

{
	#ifdef CUSTOMER_HW_BOSNIAN_LOWERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//122

{
	#ifdef CUSTOMER_HW_AFRIKAANS
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//123

{
	#ifdef CUSTOMER_HW_AFRIKAANS_UPPERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//124

{
	#ifdef CUSTOMER_HW_AFRIKAANS_LOWERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//125

{
	#ifdef CUSTOMER_HW_SWAHILI
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//126

{
	#ifdef CUSTOMER_HW_SWAHILI_UPPERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//127

{
	#ifdef CUSTOMER_HW_SWAHILI_LOWERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//128

{
	#ifdef CUSTOMER_HW_HAUSA
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//129

{
	#ifdef CUSTOMER_HW_HAUSA_UPPERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//130

{
	#ifdef CUSTOMER_HW_HW_HAUSA_LOWERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//131

{
	#ifdef CUSTOMER_HW_IGBO
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//132

{
	#ifdef CUSTOMER_HW_IGBO_UPPERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//133

{
	#ifdef CUSTOMER_HW_IGBO_LOWERCASE
	SG_NULL,
	0,
	SG_NULL,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//134
	{
#ifdef  CUSTOMER_HW_TURKMEN
		SG_NULL,
			0,
	SG_NULL,
#else
	SG_NULL,
	0,
	SG_NULL,
#endif
	}, //135

	{
#ifdef  CUSTOMER_HW_TURKMEN_UPPERCASE
		SG_NULL,
			0,
			SG_NULL,
#else
			SG_NULL,
			0,
			SG_NULL,
#endif
	}, //136
	{
#ifdef  CUSTOMER_HW_TURKMEN_LOWERCASE
	SG_NULL,
		0,
		SG_NULL,
#else
		SG_NULL,
		0,
		SG_NULL,
#endif
	}, //137
{
	#ifdef CUSTOMER_HW_RUSSIAN
	RussianData,
	&RussianDatacount,
	Russian_HW_password,
	#else
	SG_NULL,
	0,
	SG_NULL,
	#endif
},//138

	{
#ifdef CUSTOMER_HW_RUSSIAN_UPPERCASE
		SG_NULL,
			0,
			SG_NULL,
#else
			SG_NULL,
			0,
			SG_NULL,
#endif
},//139

	{
#ifdef CUSTOMER_HW_RUSSIAN_LOWERCASE
			SG_NULL,
				0,
				SG_NULL,
#else
				SG_NULL,
				0,
				SG_NULL,
#endif
},//140

	
		{
#ifdef CUSTOMER_HW_UKRAINIAN
			UkrainianData,
			&UkrainianDatacount,
			Ukrainian_HW_password,
#else
					SG_NULL,
					0,
					SG_NULL,
#endif
},//141


			{
#ifdef CUSTOMER_HW_UKRAINIAN_UPPERCASE
					SG_NULL,
						0,
						SG_NULL,
#else
						SG_NULL,
						0,
						SG_NULL,
#endif
},//142

{
					
#ifdef CUSTOMER_HW_UKRAINIAN_LOWERCASE
SG_NULL,
0,
SG_NULL,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//143

{
#ifdef CUSTOMER_HW_BELARUSIAN
SG_NULL,
0,
SG_NULL,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//144

{
#ifdef CUSTOMER_HW_BELARUSIAN_UPPERCASE
SG_NULL,
0,
SG_NULL,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//145

{
#ifdef CUSTOMER_HW_BELARUSIAN_LOWERCASE
SG_NULL,
0,
SG_NULL,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//146

{
#ifdef CUSTOMER_HW_BULGARIAN
SG_NULL,
0,
SG_NULL,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//147

{
#ifdef CUSTOMER_HW_BULGARIAN_UPPERCASE
SG_NULL,
0,
SG_NULL,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//148



{
#ifdef CUSTOMER_HW_BULGARIAN_LOWERCASE
SG_NULL,
0,
SG_NULL,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//149

	{
#ifdef CUSTOMER_HW_KAZAKH
			KazakhData,
				&KazakhDatacount,
				Kazakh_HW_password,
#else
				SG_NULL,
				0,
				SG_NULL,
#endif
},//150

					{
#ifdef CUSTOMER_HW_KAZAKH_UPPERCASE
SG_NULL,
0,
SG_NULL,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//151


			{
#ifdef CUSTOMER_HW_KAZAKH_LOWERCASE
SG_NULL,
0,
SG_NULL,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//152


{
#ifdef CUSTOMER_HW_TAJIKI                  //Ëþ¼ª¿ËÓï
SG_NULL,
0,
SG_NULL,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//153


{
#ifdef CUSTOMER_HW_SERBIAN             //Èû¶ûÎ¬ÑÇÓï      
	SG_NULL,
		0,
		SG_NULL,
#else
		SG_NULL,
		0,
		SG_NULL,
#endif
},//154

{
#ifdef CUSTOMER_HW_SERBIAN_UPPERCASE      //´óÐ´Èû¶ûÎ¬ÑÇÓï      
		SG_NULL,
			0,
			SG_NULL,
#else
			SG_NULL,
			0,
			SG_NULL,
#endif
},//155

	{
#ifdef CUSTOMER_HW_SERBIAN_LOWERCASE      //Ð¡Ð´Èû¶ûÎ¬ÑÇÓï     
			SG_NULL,
				0,
				SG_NULL,
#else
				SG_NULL,
				0,
				SG_NULL,
#endif
},//156

		{
#ifdef CUSTOMER_HW_UZBEK                  //ÎÚ×È±ð¿ËÓï      
				SG_NULL,
					0,
					SG_NULL,
#else
					SG_NULL,
					0,
					SG_NULL,
#endif
},//157

			{
#ifdef CUSTOMER_HW_UZBEK_UPPERCASE        //´óÐ´ÎÚ×È±ð¿ËÓï      
					SG_NULL,
						0,
						SG_NULL,
#else
						SG_NULL,
						0,
						SG_NULL,
#endif
},//158

				{
#ifdef CUSTOMER_HW_UZBEK_LOWERCASE        //Ð¡Ð´ÎÚ×È±ð¿ËÓï      
						SG_NULL,
							0,
							SG_NULL,
#else
							SG_NULL,
							0,
							SG_NULL,
#endif
},//159

					{
#ifdef CUSTOMER_HW_MACEDONIAN             //ÂíÆä¶ÙÓï     
							SG_NULL,
								0,
								SG_NULL,
#else
								SG_NULL,
								0,
								SG_NULL,
#endif
},//160

						{
#ifdef CUSTOMER_HW_MACEDONIAN_UPPERCASE   //´óÐ´ÂíÆä¶ÙÓï      
								SG_NULL,
									0,
									SG_NULL,
#else
									SG_NULL,
									0,
									SG_NULL,
#endif
},//161

							{
#ifdef CUSTOMER_HW_MACEDONIAN_LOWERCASE   //Ð¡Ð´ÂíÆä¶ÙÓï      
									SG_NULL,
										0,
										SG_NULL,
#else
										SG_NULL,
										0,
										SG_NULL,
#endif
},//162

								{
#ifdef CUSTOMER_HW_YI            //ÒÍÓï      
										SG_NULL,
											0,
											SG_NULL,
#else
											SG_NULL,
											0,
											SG_NULL,
#endif
},//163

									{
#ifdef CUSTOMER_HW_TIBETAN             //²ØÓï      
TibetanData,
&TibetanDatacount,
Tibetan_HW_password,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//164
 
        
 	{
#ifdef CUSTOMER_HW_KAZAK//wanghui add
KazakData,
&KazakDatacount,
Kazak_HW_password,
#else
SG_NULL,
0,
SG_NULL,
#endif
},//165




};
//#endif
