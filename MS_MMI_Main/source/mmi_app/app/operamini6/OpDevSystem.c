/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#include "opcfg.h"
#include "opdevinternal.h"
#include "opimport.h"

#include "os_api.h"
#include "version.h"

#ifndef PLATFORM_SC6600L
#include "mmiclipbrd_export.h"
#endif /*PLATFORM_SC6600L*/

OD_STATUS OD_SystemInit(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);

	return OD_SUCCESS;
}

OD_STATUS OD_SystemDeinit(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);

	return OD_SUCCESS;
}

#ifndef PLATFORM_SC6600L
static OD_STATUS _OD_TranslateClipboardError(CLIPBRD_RESULT_E clip_res )
{
    OD_STATUS res = OD_ERROR;
    switch(clip_res)
    {
    case CLIPBRD_RESULT_NOERROR:
        res = OD_SUCCESS;
        break;
    case CLIPBRD_RESULT_FORMAT_NOT_SUPPORT:
        res = OD_NOT_SUPPORTED;
        break;
    case CLIPBRD_RESULT_INVALID_ARGUMENT:
        res = OD_INVALID_PARAM;
        break;
    case CLIPBRD_RESULT_OUT_OF_RANGE:
        res = OD_OUT_OF_RANGE;
        break;
    case CLIPBRD_RESULT_OUT_OF_MEMORY:
        /* don't set res to be OD_ON_MEMORY, since clipboard system and opera use different memory pool,
        Opera can do nothing when system out of memory. */
    default:
        res = OD_ERROR;
    }
    return res;
}
#endif /*PLATFORM_SC6600L*/

OD_STATUS OD_ClipboardSetData(OD_Context *context, OD_CLIPBOARD_FORMAT format, void* data, int len)
{
#ifdef PLATFORM_SC6600L
    return OD_NOT_SUPPORTED;
#else
    CLIPBRD_RESULT_E clip_res = CLIPBRD_RESULT_FORMAT_NOT_SUPPORT;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(data);

    if(format == OD_CLIPBOARD_FORMAT_TEXT_UNICODE)
    {
        clip_res = MMIAPICLIPBRD_SetData(CLIPBRD_FORMAT_TEXT, data, len);
    }
    return _OD_TranslateClipboardError(clip_res);
#endif /*PLATFORM_SC6600L*/
}

OD_STATUS OD_ClipboardGetData(OD_Context *context, OD_CLIPBOARD_FORMAT format, void** data, int* len)
{
#ifdef PLATFORM_SC6600L
    return OD_NOT_SUPPORTED;
#else
    CLIPBRD_RESULT_E clip_res = CLIPBRD_RESULT_FORMAT_NOT_SUPPORT;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(data);
	OD_CHECK_ARGS_RETURN_STATUS(len);

    if(format == OD_CLIPBOARD_FORMAT_TEXT_UNICODE)
    {
        clip_res = MMIAPICLIPBRD_GetData(CLIPBRD_FORMAT_TEXT, data, len);
    }
    return _OD_TranslateClipboardError(clip_res);
#endif /*PLATFORM_SC6600L*/
}

OD_STATUS OD_ClipboardHasData(OD_Context *context, OD_CLIPBOARD_FORMAT format, OD_BOOL* hasData)
{
#ifdef PLATFORM_SC6600L
    return OD_NOT_SUPPORTED;
#else
    CLIPBRD_RESULT_E clip_res = CLIPBRD_RESULT_FORMAT_NOT_SUPPORT;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(hasData);

    if(format == OD_CLIPBOARD_FORMAT_TEXT_UNICODE)
    {
        *hasData = MMIAPICLIPBRD_IsAvailable(CLIPBRD_FORMAT_TEXT);
        clip_res = CLIPBRD_RESULT_NOERROR;
    }
    return _OD_TranslateClipboardError(clip_res);
#endif /*PLATFORM_SC6600L*/
}

unsigned int OD_GetTickCount()
{
    // milliseconds from system boot
    return SCI_GetTickCount();
}

static unsigned int _OD_GetDayNumOfDate(SCI_DATE_T *date)
{
    OD_BOOL  isLeap = OD_FALSE;          // Indicate whether it's leap year
    unsigned int day = 0;
    const unsigned int pastDaysInMonth[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int years = 0;
    int leaps = 0;

    // This is (roughly) valid region for presenting the seconds in UINT.
    // The calculation of leap years below is "optimized" based on this assumption.
    // It may *not* be applicable to some larger region.
    OD_ASSERT(date->year >= 1970 && date->year <= 2106);

    years = date->year - 1970;
    leaps = (years + 1) / 4;            // Compute how many leap years from 1970 to date->year
    day = years * 365 + leaps;

    //isLeap = (date->year % 4 == 0 && date->year % 100 != 0) || (date->year % 400 == 0);
    isLeap = ((date->year - 1968) % 4 == 0);

    // The calculation of leaps and isLeap will fail for special case "2100". Adjust it here.
    // On the other hand, it is useless until year 2100 :)
    if (date->year > 2100)
    {
        day--;
    }
    else if (date->year == 2100)
    {
        isLeap = OD_FALSE;
    }

    day += pastDaysInMonth[date->mon - 1];
    if (isLeap && date->mon > 2)
    {
        day++;
    }

    day += date->mday - 1;

    return day;
}

static unsigned int _OD_GetCurrentSeconds()
{
    uint32      sec = 0;
    SCI_DATE_T  date = {0};
    SCI_TIME_T  time = {0};

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);

    sec = _OD_GetDayNumOfDate(&date) * 24 * 3600
        + time.hour * 3600 + time.min * 60 + time.sec;

    return sec;
}

unsigned int OD_GetUTCTime(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);

    return _OD_GetCurrentSeconds() - OD_GetTimeZone(context);
}

OD_STATUS OD_GetLocalTime(OD_Context* context, int* h, int* m, int* s)
{
    SCI_TIME_T  time = {0};

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(h);
	OD_CHECK_ARGS_RETURN_STATUS(m);
	OD_CHECK_ARGS_RETURN_STATUS(s);

    TM_GetSysTime(&time);
    
    *h = time.hour;
    *m = time.min;
    *s = time.sec;
    return OD_SUCCESS;
}

int OD_GetTimeZone(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);

#ifdef WORLD_CLOCK_SUPPORT
    return (int)(MMIAPIACC_GetLocalTimeZone() * 3600);
#else
    //FIXME
    return 0;
#endif
}

OD_STATUS OD_GetSystemProperty(OD_Context *context, OD_SYSTEM_PROPERTY_TYPE property, int* value)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(value);

	switch(property)
	{
	case OD_TOUCH_SUPPORT:
		{
#ifdef TOUCHPANEL_TYPE
			*value = OD_TOUCH_SUPPORT_PRESS | OD_TOUCH_SUPPORT_RELEASE | OD_TOUCH_SUPPORT_MOVE;
#else
			*value = OD_TOUCH_SUPPORT_NONE;
#endif
			return OD_SUCCESS;
		}
	case OD_KEYPAD_SUPPORT:
		{
#ifdef KEYPAD_TYPE_TYPICAL_KEYPAD
			*value = OD_KEYPAD_SUPPORT_NORMAL;
#elif defined(KEYPAD_TYPE_QWERTY_KEYPAD)
			*value = OD_KEYPAD_SUPPORT_QWERTY;
#else
			*value = OD_KEYPAD_SUPPORT_NONE;
#endif
			return OD_SUCCESS;
		}
	case OD_PHYSICALKEY_SUPPORT:
		{
			/* Spreadtrum people tell us no api to detect key existing. */
#ifdef OD_TWEAK_PHYSICALKEYS
			*value = OD_TWEAK_PHYSICALKEYS;
#else
			/* By default, we think we have soft and direction keys. */
			*value = OD_PHYSICALKEY_SUPPORT_SOFT_KEYS | OD_PHYSICALKEY_SUPPORT_DIRECTION_KEYS;
#endif
			return OD_SUCCESS;
		}
	case OD_NATIVE_INPUT_SUPPORT:
		{
			*value = OD_NATIVE_INPUT_SUPPORT_FULLSCREEN;
			return OD_SUCCESS;
		}
	case OD_CLIPBOARD_SUPPORT:
		{
			*value = TRUE;
			return OD_SUCCESS;
		}
		/* 
		* There's no real way to figure out whether a device
		* supports vibration or not, so let's assume they all do.
		*/
	case OD_VIBRATION_SUPPORT:
		{
			*value = OD_TRUE;
			return OD_SUCCESS;
		}
	case OD_MMS_SUPPORT:
		{
#ifdef MMS_SUPPORT
			*value = OD_TRUE;
#else
			*value = OD_FALSE;
#endif /* MMS_SUPPORT */
			return OD_SUCCESS;
		}

	case OD_BLUETOOTH_SUPPORT:
		{
#ifdef BLUETOOTH_SUPPORT
			*value = OD_TRUE;
#else
			*value = OD_FALSE;
#endif /* BLUETOOTH_SUPPORT */
			return OD_SUCCESS;
		}

	case OD_SCREEN_ROTATION_SUPPORT:
		{
#ifdef PLATFORM_SC6600L
			*value = OD_FALSE;
#else
			*value = OD_TRUE;
#endif /* PLATFORM_SC6600L */
			return OD_SUCCESS;
		}

		/* 
		* There doesn't seem to be a single definition to see
		* whether GPS is available or not, so let's assume it's
		* never available to keep things simple.
		*/
	case OD_GEOLOCATION_SUPPORT:
		{
			*value = OD_FALSE;
			return OD_SUCCESS;
		}

	case OD_MAILTO_SUPPORT:
		{
#ifdef MMIEMAIL_SUPPORT
			*value = OD_TRUE;
#else
			*value = OD_FALSE;
#endif
			return OD_SUCCESS;
		}
    }

    OD_ASSERT(!"unknown system property");
    return OD_NOT_SUPPORTED;
}

OD_STATUS OD_GetIMEI(OD_Context *context, OD_IMEI *od_imei)
{
    MN_IMEI_T imei;
    int dst_size = MN_MAX_IMEI_LENGTH << 1;

    OD_TRACE_LOW("OD_GetIMEI");

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(od_imei);

    if(dst_size >= sizeof(od_imei->data))
    {
        OD_ASSERT(!"The size of OD_IMEI.data is not enough");
        return OD_ERROR;
    }

    od_memset(od_imei->data, 0, sizeof(od_imei->data));
    if(MNNV_GetIMEIEx(MMIAPISET_GetActiveSim(), imei)) /*lint !e64*/
    {
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,(uint8*)imei, dst_size, od_imei->data);
        od_imei->data[dst_size] = 0;
        OD_TRACE_LOW("OD_GetIMEI succeed, imei: %s", od_imei->data);
        return OD_SUCCESS;
    }
    return OD_ERROR;
}

OD_STATUS OD_GetIMSI(OD_Context *context, OD_IMSI* od_imsi)
{
    MN_IMSI_T imsi;
    int dst_size = 0;

    OD_TRACE_LOW("OD_GetIMSI");

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(od_imsi);

    imsi = MNSIM_GetImsiEx(MMIAPISET_GetActiveSim());   //FIXME, If failed ?
    dst_size = imsi.imsi_len << 1;
    if(dst_size >= sizeof(od_imsi->data))
    {
        OD_ASSERT(!"The size of OD_IMSI.data is not enough");
        return OD_ERROR;
    }

    od_memset(od_imsi->data, 0, sizeof(od_imsi->data));
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, imsi.imsi_val, dst_size, od_imsi->data);
    od_imsi->data[dst_size] = 0;
    OD_TRACE_LOW("OD_GetIMSI succeed, imsi:%s", od_imsi->data);
    return OD_SUCCESS;
}

OD_STATUS OD_GetLangCountryCode(OD_Context *context, char *buf, int buf_size)
{
    char *langCountry = NULL;
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
    MMIAPISET_GetLanguageType(&lang_type);

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(buf);

    switch(lang_type)
    {
//#ifdef IM_ENGLISH_SUPPORT
    case MMISET_LANGUAGE_ENGLISH:
        langCountry = "en";
        break;
//#endif /* IM_ENGLISH_SUPPORT */
//#if defined(IM_CHINESE_S_SUPPORT) || defined(IM_SIMP_CHINESE_SUPPORT)
    case MMISET_LANGUAGE_SIMP_CHINESE:
        langCountry = "zh-CN";
        break;
//#endif /*defined(IM_CHINESE_S_SUPPORT)||defined(IM_SIMP_CHINESE_SUPPORT)*/
//#if defined(IM_CHINESE_T_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)
    case MMISET_LANGUAGE_TRAD_CHINESE:
    case MMISET_LANGUAGE_TRAD_CHINESE_HK:
        langCountry = "zh-TW";
        break;
//#endif /*defined(IM_CHINESE_T_SUPPORT)||defined(IM_TRAD_CHINESE_SUPPORT)   */
//#ifdef IM_AMHARIC_SUPPORT
    case MMISET_LANGUAGE_AMHARIC:
        langCountry = "am-ET";
        break;
//#endif /* IM_AMHARIC_SUPPORT */
//#ifdef IM_ARABIC_SUPPORT
    case MMISET_LANGUAGE_ARABIC:
        langCountry = "ar-SA";
        break;
//#endif /* IM_ARABIC_SUPPORT */
//#ifdef IM_FRENCH_SUPPORT
    case MMISET_LANGUAGE_FRENCH:
        langCountry = "fr-FR";
        break;
//#endif /* IM_FRENCH_SUPPORT */
//#ifdef IM_HINDI_SUPPORT
    case MMISET_LANGUAGE_HINDI:
        langCountry = "hi-IN";
        break;
//#endif /* IM_HINDI_SUPPORT */
//#ifdef IM_HUNGARIAN_SUPPORT
    case MMISET_LANGUAGE_HUNGARIAN:
        langCountry = "hu-HU";
        break;
//#endif /* IM_HUNGARIAN_SUPPORT */
//#ifdef IM_INDONESIAN_SUPPORT
    case MMISET_LANGUAGE_INDONESIAN:
        langCountry = "id-ID";
        break;
//#endif /* IM_INDONESIAN_SUPPORT */
//#ifdef IM_MALAY_SUPPORT
    case MMISET_LANGUAGE_MALAY:
        langCountry = "ms-MY";
        break;
//#endif /* IM_MALAY_SUPPORT */
//#ifdef IM_PORTUGUESE_SUPPORT
    case MMISET_LANGUAGE_PORTUGUESE:
        langCountry = "pt-PT";
        break;
//#endif /* IM_PORTUGUESE_SUPPORT */
//#ifdef IM_RUSSIAN_SUPPORT
    case MMISET_LANGUAGE_RUSSIAN:
        langCountry = "ru-RU";
        break;
//#endif /* IM_RUSSIAN_SUPPORT */
//#ifdef IM_SPANISH_SUPPORT
    case MMISET_LANGUAGE_SPANISH:
        langCountry = "es-ES";
        break;
//#endif /* IM_SPANISH_SUPPORT */
//#ifdef IM_TAGALOG_SUPPORT
    case MMISET_LANGUAGE_TAGALOG:
        langCountry = "tl-TL";
        break;
//#endif /* IM_TAGALOG_SUPPORT */
//#ifdef IM_THAI_SUPPORT
    case MMISET_LANGUAGE_THAI:
        langCountry = "th-TH";
        break;
//#endif /* IM_THAI_SUPPORT */
//#ifdef IM_VIETNAMESE_SUPPORT
    case MMISET_LANGUAGE_VIETNAMESE:
        langCountry = "vi-VN";
        break;
//#endif /* IM_VIETNAMESE_SUPPORT */
//#ifdef IM_URDU_SUPPORT
    case MMISET_LANGUAGE_URDU:
        langCountry = "ur-PK";
        break;
//#endif /* IM_URDU_SUPPORT */
//#ifdef IM_ITALIAN_SUPPORT
    case MMISET_LANGUAGE_ITALIAN:
        langCountry = "it-IT";
        break;
//#endif /* IM_ITALIAN_SUPPORT */
//#ifdef IM_PERSIAN_SUPPORT
    case MMISET_LANGUAGE_PERSIAN:
        langCountry = "fa-IR";
        break;
//#endif /* IM_PERSIAN_SUPPORT */
//#ifdef IM_TURKISH_SUPPORT
    case MMISET_LANGUAGE_TURKISH:
        langCountry = "tr-TR";
        break;
//#endif /* IM_TURKISH_SUPPORT */
//#ifdef IM_GERMAN_SUPPORT
    case MMISET_LANGUAGE_GERMAN:
        langCountry = "de-DE";
        break;
//#endif /* IM_GERMAN_SUPPORT */
//#ifdef IM_GREEK_SUPPORT
    case MMISET_LANGUAGE_GREEK:
        langCountry = "el-GR";
        break;
//#endif /* IM_GREEK_SUPPORT */
//#ifdef IM_HEBREW_SUPPORT
    case MMISET_LANGUAGE_HEBREW:
        langCountry = "he-IL";
        break;
//#endif /* IM_HEBREW_SUPPORT */
//#ifdef IM_BENGALI_SUPPORT
    case MMISET_LANGUAGE_BENGALI:
        langCountry = "bn-IN";
        break;
//#endif /* IM_BENGALI_SUPPORT */
//#ifdef IM_CZECH_SUPPORT
    case MMISET_LANGUAGE_CZECH:
        langCountry = "cs-CZ";
        break;
//#endif /* IM_CZECH_SUPPORT */
//#ifdef IM_SLOVENIAN_SUPPORT
    case MMISET_LANGUAGE_SLOVENIAN:
        langCountry = "sl-SI";
        break;
//#endif /* IM_SLOVENIAN_SUPPORT */
//#ifdef IM_ROMANIAN_SUPPORT
    case MMISET_LANGUAGE_ROMANIAN:
        langCountry = "ro-RO";
        break;
//#endif /* IM_ROMANIAN_SUPPORT */
//#ifdef IM_TELUGU_SUPPORT
    case MMISET_LANGUAGE_TELUGU:
        langCountry = "te-IN";
        break;
//#endif /* IM_TELUGU_SUPPORT */
//#ifdef IM_MARATHI_SUPPORT
    case MMISET_LANGUAGE_MARATHI:
        langCountry = "mr-IN";
        break;
//#endif /* IM_MARATHI_SUPPORT */
//#ifdef IM_TAMIL_SUPPORT
    case MMISET_LANGUAGE_TAMIL:
        langCountry = "ta-IN";
        break;
//#endif /* IM_TAMIL_SUPPORT */
//#ifdef IM_GUJARATI_SUPPORT
    case MMISET_LANGUAGE_GUJARATI:
        langCountry = "gu-IN";
        break;
//#endif /* IM_GUJARATI_SUPPORT */
//#ifdef IM_KANNADA_SUPPORT
    case MMISET_LANGUAGE_KANNADA:
        langCountry = "kn-IN";
        break;
//#endif /* IM_KANNADA_SUPPORT */
//#ifdef IM_MALAYALAM_SUPPORT
    case MMISET_LANGUAGE_MALAYALAM:
        langCountry = "ml-IN";
        break;
//#endif /* IM_MALAYALAM_SUPPORT */
//#ifdef IM_ORIYA_SUPPORT
    case MMISET_LANGUAGE_ORIYA:
        langCountry = "or-IN";
        break;
//#endif /* IM_ORIYA_SUPPORT */
//#ifdef IM_PUNJABI_SUPPORT
    case MMISET_LANGUAGE_PUNJABI:
        langCountry = "pa-IN";
        break;
//#endif /* IM_PUNJABI_SUPPORT */
//#ifdef IM_AFRIKAANS_SUPPORT
    case MMISET_LANGUAGE_AFRIKAANS:
        langCountry = "af-NA";
        break;
//#endif /* IM_AFRIKAANS_SUPPORT */
//#ifdef IM_ALBANIAN_SUPPORT
    case MMISET_LANGUAGE_ALBANIAN:
        langCountry = "sq-SQ";
        break;
//#endif /* IM_ALBANIAN_SUPPORT */
//#ifdef IM_ARMENIAN_SUPPORT
    case MMISET_LANGUAGE_ARMENIAN:
        langCountry = "hy-AM";
        break;
//#endif /* IM_ARMENIAN_SUPPORT */
//#ifdef IM_AZERBAIJANI_SUPPORT
    case MMISET_LANGUAGE_AZERBAIJANI:
        langCountry = "az-AZ";
        break;
//#endif /* IM_AZERBAIJANI_SUPPORT */
//#ifdef IM_BASQUE_SUPPORT
    case MMISET_LANGUAGE_BASQUE:
        langCountry = "eu-ES";
        break;
//#endif /* IM_BASQUE_SUPPORT */
//#ifdef IM_BULGARIAN_SUPPORT
    case MMISET_LANGUAGE_BULGARIAN:
        langCountry = "bg-BG";
        break;
//#endif /* IM_BULGARIAN_SUPPORT */
//#ifdef IM_CATALAN_SUPPORT
    case MMISET_LANGUAGE_CATALAN:
        langCountry = "ca-ES";
        break;
//#endif /* IM_CATALAN_SUPPORT */
//#ifdef IM_CROATIAN_SUPPORT
    case MMISET_LANGUAGE_CROATIAN:
        langCountry = "hr-HR";
        break;
//#endif /* IM_CROATIAN_SUPPORT */
//#ifdef IM_DANISH_SUPPORT
    case MMISET_LANGUAGE_DANISH:
        langCountry = "da-DK";
        break;
//#endif /* IM_DANISH_SUPPORT */
//#ifdef IM_DUTCH_SUPPORT
    case MMISET_LANGUAGE_DUTCH:
        langCountry = "nl-NL";
        break;
//#endif /* IM_DUTCH_SUPPORT */
//#ifdef IM_ESTONIAN_SUPPORT
    case MMISET_LANGUAGE_ESTONIAN:
        langCountry = "et-EE";
        break;
//#endif /* IM_ESTONIAN_SUPPORT */
//#ifdef IM_FILIPINO_SUPPORT
    case MMISET_LANGUAGE_FILIPINO:
        langCountry = "tl-PH";
        break;
//#endif /* IM_FILIPINO_SUPPORT */
//#ifdef IM_FINNISH_SUPPORT
    case MMISET_LANGUAGE_FINNISH:
        langCountry = "fi-FI";
        break;
//#endif /* IM_FINNISH_SUPPORT */
//#ifdef IM_GALICIAN_SUPPORT
    case MMISET_LANGUAGE_GALICIAN:
        langCountry = "gl-ES";
        break;
//#endif /* IM_GALICIAN_SUPPORT */
//#ifdef IM_GEORGIAN_SUPPORT
    case MMISET_LANGUAGE_GEORGIAN:
        langCountry = "ka-GE";
        break;
//#endif /* IM_GEORGIAN_SUPPORT */
//#ifdef IM_HAUSA_SUPPORT
    case MMISET_LANGUAGE_HAUSA:
        langCountry = "ha-HA";
        break;
//#endif /* IM_HAUSA_SUPPORT */
//#ifdef IM_ICELANDIC_SUPPORT
    case MMISET_LANGUAGE_ICELANDIC:
        langCountry = "is-IS";
        break;
//#endif /* IM_ICELANDIC_SUPPORT */
//#ifdef IM_IGBO_SUPPORT
    case MMISET_LANGUAGE_IGBO:
        langCountry = "ig-NG";
        break;
//#endif /* IM_IGBO_SUPPORT */
//#ifdef IM_IRISH_SUPPORT
    case MMISET_LANGUAGE_IRISH:
        langCountry = "ga-IE";
        break;
//#endif /* IM_IRISH_SUPPORT */
//#ifdef IM_KAZAKH_SUPPORT
    case MMISET_LANGUAGE_KAZAKH:
        langCountry = "kk-KK";
        break;
//#endif /* IM_KAZAKH_SUPPORT */
//#ifdef IM_LATVIAN_SUPPORT
    case MMISET_LANGUAGE_LATVIAN:
        langCountry = "lv-LV";
        break;
//#endif /* IM_LATVIAN_SUPPORT */
//#ifdef IM_LITHUANIAN_SUPPORT
    case MMISET_LANGUAGE_LITHUANIAN:
        langCountry = "lt-LT";
        break;
//#endif /* IM_LITHUANIAN_SUPPORT */
//#ifdef IM_MACEDONIAN_SUPPORT
    case MMISET_LANGUAGE_MACEDONIAN:
        langCountry = "mk-MK";
        break;
//#endif /* IM_MACEDONIAN_SUPPORT */
//#ifdef IM_MOLDOVAN_SUPPORT
    case MMISET_LANGUAGE_MOLDOVAN:
        langCountry = "ro-MD";
        break;
//#endif /* IM_MOLDOVAN_SUPPORT */
//#ifdef IM_NORWEGIAN_SUPPORT
    case MMISET_LANGUAGE_NORWEGIAN:
        langCountry = "nb-NO";
        break;
//#endif /* IM_NORWEGIAN_SUPPORT */
//#ifdef IM_POLISH_SUPPORT
    case MMISET_LANGUAGE_POLISH:
        langCountry = "pl-PL";
        break;
//#endif /* IM_POLISH_SUPPORT */
//#ifdef IM_SERBIAN_SUPPORT
    case MMISET_LANGUAGE_SERBIAN:
        langCountry = "sr-SR";
        break;
//#endif /* IM_SERBIAN_SUPPORT */
//#ifdef IM_SESOTHO_SUPPORT
    case MMISET_LANGUAGE_SESOTHO:
        langCountry = "st-ST";
        break;
//#endif /* IM_SESOTHO_SUPPORT */
//#ifdef IM_SLOVAK_SUPPORT
    case MMISET_LANGUAGE_SLOVAK:
        langCountry = "sk-SK";
        break;
//#endif /* IM_SLOVAK_SUPPORT */
//#ifdef IM_SWEDISH_SUPPORT
    case MMISET_LANGUAGE_SWEDISH:
        langCountry = "sv-SE";
        break;
//#endif /* IM_SWEDISH_SUPPORT */
//#ifdef IM_UKRAINIAN_SUPPORT
    case MMISET_LANGUAGE_UKRAINIAN:
        langCountry = "uk-UA";
        break;
//#endif /* IM_UKRAINIAN_SUPPORT */
//#ifdef IM_YORUBA_SUPPORT
    case MMISET_LANGUAGE_YORUBA:
        langCountry = "yo-YO";
        break;
//#endif /* IM_YORUBA_SUPPORT */
//#ifdef IM_XHOSA_SUPPORT
    case MMISET_LANGUAGE_XHOSA:
        langCountry = "xh-ZA";
        break;
//#endif /* IM_XHOSA_SUPPORT */
//#ifdef IM_ZULU_SUPPORT
    case MMISET_LANGUAGE_ZULU:
        langCountry = "zu-SZ";
        break;
//#endif /* IM_ZULU_SUPPORT */
//#ifdef IM_ASSAMESE_SUPPORT
    case MMISET_LANGUAGE_ASSAMESE:
        langCountry = "as-IN";
        break;
//#endif /* IM_ASSAMESE_SUPPORT */
//#ifdef IM_SWAHILI_SUPPORT
    case MMISET_LANGUAGE_SWAHILI:
        langCountry = "sw-TZ";
        break;
//#endif /* IM_SWAHILI_SUPPORT */
//#ifdef IM_SPANISH_AM_SUPPORT
    case MMISET_LANGUAGE_SPANISH_AM:
        langCountry = "es-LA";
        break;
//#endif /* IM_SPANISH_AM_SUPPORT */
    default:
        langCountry = "en";
        break;
    }

#define OD_LANG_COUNTRY_CODE_MIN_LEN	5
    OD_ASSERT(buf_size >= OD_LANG_COUNTRY_CODE_MIN_LEN);
    od_strncpy(buf, langCountry, buf_size);

    OD_TRACE_LOW("OD_GetLangCountryCode: %s", langCountry);
    return OD_SUCCESS;
}

OD_STATUS OD_GetPlatformName(OD_Context *context, char *buf, int buf_size)
{
#if defined(WIN32)
	// No version.lib (should include VERSION_GetInfo) on simulator.
	const char *platformName = NULL;
#else
	const char *platformName = VERSION_GetInfo(HW_VERSION);
#endif /* WIN32 */

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(buf);
	OD_CHECK_ARGS_RETURN_STATUS(buf_size > 0);
	if (platformName)
	{
		od_snprintf(buf, buf_size - 1, "Spreadtrum/%s", platformName);
		buf[buf_size - 1] = '\0';
	}
	else
	{
		od_snprintf(buf, buf_size - 1, "Spreadtrum/Unknown");
		buf[buf_size - 1] = '\0';
	}
	OD_TRACE_LOW("OD_GetPlatformName: %s", buf);
	return OD_SUCCESS;
}

static void _OD_DummyTimerFun(unsigned long int value)
{
	(value);

	OD_ASSERT(!"Please assign new timer func before start the timer");
}

OD_STATUS OD_TimerCreate(OD_Context *context, OD_TaskID task, OD_TimerID* timer, OD_TimerArg userData)
{
    SCI_TIMER_PTR timer_id = SCI_CreateTimer(OD_TIMER_NAME, _OD_DummyTimerFun, (uint32)userData, 20, SCI_NO_ACTIVATE);

	OD_TRACE_LOW("OD_TimerCreate: created timer_id: 0x%x", timer_id);

	(task);
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(timer);

    OD_ASSERT(timer_id != NULL);
    if (timer_id == SCI_NULL)
    {
        return OD_ERROR;
    }

    *timer = (OD_TimerID)timer_id;
    return OD_SUCCESS;
}

OD_STATUS OD_TimerDestroy(OD_Context *context, OD_TaskID task, OD_TimerID timer)
{
    SCI_TIMER_PTR timer_id = (SCI_TIMER_PTR)timer;

    (task);
	OD_CHECK_ARGS_RETURN_STATUS(context);

    if (SCI_IsTimerActive(timer_id) != SCI_FALSE)
    {
        // Do not care the return value, we will try delete the timer anyway, regardless it's stopped successfully.
        SCI_DeactiveTimer(timer_id);
    }

    if (SCI_DeleteTimer(timer_id) == SCI_SUCCESS)
    {
        return OD_SUCCESS;
    }
    return OD_ERROR;
}

OD_STATUS OD_TimerStart(OD_Context *context, OD_TaskID task, OD_TimerID timer, OD_TIMERFUN callback, int delay, OD_TimerArg user_data)
{
    SCI_TIMER_PTR timer_id = (SCI_TIMER_PTR)timer;

    (task);
	OD_CHECK_ARGS_RETURN_STATUS(context);

    if(SCI_IsTimerActive(timer_id) != SCI_FALSE)
    {
        OD_ASSERT(!"Attempt to start a same timer twice! There must be something wrong, please check");
        return OD_TIMER_ALREADY_RUN;
    }

	if (SCI_ChangeTimer(timer_id, (TIMER_FUN)callback, delay) == SCI_SUCCESS)
	{
		if (SCI_ActiveTimer(timer_id) == SCI_SUCCESS)
		{
			return OD_SUCCESS;
		}
	}

    return OD_ERROR;
}

OD_STATUS OD_TimerStop(OD_Context *context, OD_TaskID task, OD_TimerID timer)
{
    SCI_TIMER_PTR timer_id = (SCI_TIMER_PTR)timer;

    (task);
	OD_CHECK_ARGS_RETURN_STATUS(context);

    if(SCI_IsTimerActive(timer_id) != SCI_FALSE)
    {
        if (SCI_DeactiveTimer(timer_id) == SCI_SUCCESS)
        {
            return OD_SUCCESS;
        }
    }
    return SCI_IsTimerActive(timer_id) == SCI_FALSE ? OD_SUCCESS : OD_ERROR;
}

OD_ALL_TASK OD_BOOL OD_IsTimerStarted(OD_Context *context, OD_TaskID task, OD_TimerID timer)
{
    SCI_TIMER_PTR timer_id = (SCI_TIMER_PTR)timer;

    (task);
	OD_CHECK_ARGS_RETURN_STATUS(context);

    return SCI_IsTimerActive(timer_id) == SCI_FALSE ? OD_FALSE : OD_TRUE;
}

OD_STATUS OD_PlatformOpenDocument(OD_Context *context, const OD_FileName *path , const  char *mimetype)
{
	OD_TRACE_LOW("OD_PlatformOpenDocument: mimetype=%s, path=", mimetype);
	OD_TRACE_W_LOW(path);

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(path);
	OD_CHECK_ARGS_RETURN_STATUS(mimetype);

    // Because there're no direct api on srpd platform, so reuse this function.
    return OD_DownloadHandleMIMEDownloaded(context, "", mimetype, path);

}

OD_BOOL OD_PlatformCanOpenDocument(OD_Context *context, const char * mimetype)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(mimetype);

    // Because there're no direct api on srpd platform, reuse this instead.
    return OD_DownloadIsMimeTypeSupport(mimetype);
}
