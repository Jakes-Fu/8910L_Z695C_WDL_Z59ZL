/*******************************************************************************
;*******************************************************************************
;**                                                                           **
;**                    COPYRIGHT 2011 TEGIC COMMUNICATIONS                    **
;**                                                                           **
;**                TEGIC COMMUNICATIONS PROPRIETARY INFORMATION               **
;**                                                                           **
;**     This software is supplied under the terms of a license agreement      **
;**     or non-disclosure agreement with Tegic Communications and may not     **
;**     be copied or disclosed except in accordance with the terms of that    **
;**     agreement.                                                            **
;**                                                                           **
;*******************************************************************************
;**                                                                           **
;**     FileName: t9gen.c                                                     **
;**                                                                           **
;**  Description: Access to internal language data base files.                **
;**                                                                           **
;*******************************************************************************
;******* 10 ****** 20 ****** 30 ****** 40 ****** 50 ****** 60 ****** 70 *******/

/******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/15/2011     haiwu.chen         edit                                    *
注意在这里完成T9多语的集成动作，把生成的东西集成于此
t9gen.c t9lang.c
******************************************************************************/

#define PAGESIZE    1024

#include "sci_types.h"
#include "mmi_modu_main.h"
#include "mmi_data.h"
#include "t9api.h"
/*#include "assert.h"*/
#include "mmi_base_common.h"
#include "nv_item_id.h"
#include "nvitem.h"
#include "mmi_common.h"
#include "mmi_base_common.h"
#include "ctrlim_base.h"
#include "mmitheme_im.h"
#include "mmiim_t9_chs.h"
#include "mmiim_t9.h"

///////////////////////////t9 alphabetic serial  pls add ldb in order//////////
///////////////////////////t9chinese serial//////////////////////////////////
//chs
#ifdef T9CHINESE
//#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_CHINESE_S_SUPPORT)
extern T9ChineseData T9FARDATA l0204;

//#endif

//#if defined(IM_CHINESE_T_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)
extern T9ChineseData T9FARDATA l0104;
/*HK*/
extern T9ChineseData T9FARDATA l0104_HK;
//#endif

#endif

LOCAL GUIIM_LANGUAGE_T g_input_lang = GUIIM_LANG_NONE;
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*each language ldb data ptr*/
/*IM_ENGLISH_SUPPORT*/
#ifdef WIN32
LOCAL uint8 *s_l0109b00_s_ptr = PNULL;
LOCAL uint8 *s_l0109b01_s_ptr = PNULL;
LOCAL uint8 *s_l0109b02_s_ptr = PNULL;
#else
LOCAL uint8 *s_l0109b00_ptr = PNULL;
LOCAL uint8 *s_l0109b01_ptr = PNULL;
LOCAL uint8 *s_l0109b02_ptr = PNULL;
#endif/*WIN32*/

/*IM_AFRIKAANS_SUPPORT*/
LOCAL uint8 *s_l0154b00_ptr = PNULL;
LOCAL uint8 *s_l0154b01_ptr = PNULL;
LOCAL uint8 *s_l0154b02_ptr = PNULL;
LOCAL uint8 *s_l0154b03_ptr = PNULL;

/*IM_AMHARIC_SUPPORT*/ 
LOCAL uint8 *s_l0168b00_ptr = PNULL;
LOCAL uint8 *s_l0168b01_ptr = PNULL;
LOCAL uint8 *s_l0168b02_ptr = PNULL;
LOCAL uint8 *s_l0168b03_ptr = PNULL;
LOCAL uint8 *s_l0168b04_ptr = PNULL;
LOCAL uint8 *s_l0168b05_ptr = PNULL;
LOCAL uint8 *s_l0168b06_ptr = PNULL;
LOCAL uint8 *s_l0168b07_ptr = PNULL;
LOCAL uint8 *s_l0168b08_ptr = PNULL;

/*IM_ALBANIAN_SUPPORT*/ 
LOCAL uint8 *s_l0128b00_ptr = PNULL;
LOCAL uint8 *s_l0128b01_ptr = PNULL;
LOCAL uint8 *s_l0128b02_ptr = PNULL;

/*IM_ARABIC_SUPPORT*/
LOCAL uint8 *s_l0101b00_ptr = PNULL;
LOCAL uint8 *s_l0101b01_ptr = PNULL;
LOCAL uint8 *s_l0101b02_ptr = PNULL;
LOCAL uint8 *s_l0101b03_ptr = PNULL;
LOCAL uint8 *s_l0101b04_ptr = PNULL;
LOCAL uint8 *s_l0101b05_ptr = PNULL;
LOCAL uint8 *s_l0101b06_ptr = PNULL;
LOCAL uint8 *s_l0101b07_ptr = PNULL;
LOCAL uint8 *s_l0101b08_ptr = PNULL;
LOCAL uint8 *s_l0101b09_ptr = PNULL;

/*IM_ASSAMESE_SUPPORT*/
LOCAL uint8 *s_l0169b00_ptr = PNULL;
LOCAL uint8 *s_l0169b01_ptr = PNULL;
LOCAL uint8 *s_l0169b02_ptr = PNULL;
LOCAL uint8 *s_l0169b03_ptr = PNULL;
LOCAL uint8 *s_l0169b04_ptr = PNULL;
LOCAL uint8 *s_l0169b05_ptr = PNULL;
LOCAL uint8 *s_l0169b06_ptr = PNULL;
LOCAL uint8 *s_l0169b07_ptr = PNULL;
LOCAL uint8 *s_l0169b08_ptr = PNULL;
LOCAL uint8 *s_l0169b09_ptr = PNULL;

/*IM_AZERBAIJANI_SUPPORT*/
LOCAL uint8 *s_l0171b00_ptr = PNULL;
LOCAL uint8 *s_l0171b01_ptr = PNULL;
LOCAL uint8 *s_l0171b02_ptr = PNULL;
LOCAL uint8 *s_l0171b03_ptr = PNULL;
LOCAL uint8 *s_l0171b04_ptr = PNULL;
LOCAL uint8 *s_l0171b05_ptr = PNULL;
LOCAL uint8 *s_l0171b06_ptr = PNULL;

/*IM_BOSNIAN_SUPPORT*/
LOCAL uint8 *s_l01151b00_ptr = PNULL;
LOCAL uint8 *s_l01151b01_ptr = PNULL;
LOCAL uint8 *s_l01151b02_ptr = PNULL;
LOCAL uint8 *s_l01151b03_ptr = PNULL;
LOCAL uint8 *s_l01151b04_ptr = PNULL;
LOCAL uint8 *s_l01151b05_ptr = PNULL;
LOCAL uint8 *s_l01151b06_ptr = PNULL;
LOCAL uint8 *s_l01151b07_ptr = PNULL;
LOCAL uint8 *s_l01151b08_ptr = PNULL;
LOCAL uint8 *s_l01151b09_ptr = PNULL;

/*IM_BULGARIAN_SUPPORT*/
LOCAL uint8 *s_l0102b00_ptr = PNULL;
LOCAL uint8 *s_l0102b01_ptr = PNULL;
LOCAL uint8 *s_l0102b02_ptr = PNULL;
LOCAL uint8 *s_l0102b03_ptr = PNULL;
LOCAL uint8 *s_l0102b04_ptr = PNULL;
LOCAL uint8 *s_l0102b05_ptr = PNULL;
LOCAL uint8 *s_l0102b06_ptr = PNULL;
LOCAL uint8 *s_l0102b07_ptr = PNULL;
LOCAL uint8 *s_l0102b08_ptr = PNULL;

/*IM_BENGALI_SUPPORT*/
LOCAL uint8 *s_l0175b00_ptr = PNULL;
LOCAL uint8 *s_l0175b01_ptr = PNULL;
LOCAL uint8 *s_l0175b02_ptr = PNULL;
LOCAL uint8 *s_l0175b03_ptr = PNULL;
LOCAL uint8 *s_l0175b04_ptr = PNULL;
LOCAL uint8 *s_l0175b05_ptr = PNULL;
LOCAL uint8 *s_l0175b06_ptr = PNULL;
LOCAL uint8 *s_l0175b07_ptr = PNULL;
LOCAL uint8 *s_l0175b08_ptr = PNULL;

/*IM_CROATIAN_SUPPORT*/
LOCAL uint8 *s_l0189b00_ptr = PNULL;
LOCAL uint8 *s_l0189b01_ptr = PNULL;
LOCAL uint8 *s_l0189b02_ptr = PNULL;
LOCAL uint8 *s_l0189b03_ptr = PNULL;

/*IM_CZECH_SUPPORT*/
LOCAL uint8 *s_l0105b00_ptr = PNULL;
LOCAL uint8 *s_l0105b01_ptr = PNULL;
LOCAL uint8 *s_l0105b02_ptr = PNULL;
LOCAL uint8 *s_l0105b03_ptr = PNULL;
LOCAL uint8 *s_l0105b04_ptr = PNULL;
LOCAL uint8 *s_l0105b05_ptr = PNULL;
LOCAL uint8 *s_l0105b06_ptr = PNULL;
LOCAL uint8 *s_l0105b07_ptr = PNULL;

/*IM_DANISH_SUPPORT*/
LOCAL uint8 *s_l0106b00_ptr = PNULL;
LOCAL uint8 *s_l0106b01_ptr = PNULL;
LOCAL uint8 *s_l0106b02_ptr = PNULL;
LOCAL uint8 *s_l0106b03_ptr = PNULL;

/*IM_DUTCH_SUPPORT*/
LOCAL uint8 *s_l0119b00_ptr = PNULL;
LOCAL uint8 *s_l0119b01_ptr = PNULL;
LOCAL uint8 *s_l0119b02_ptr = PNULL;
LOCAL uint8 *s_l0119b03_ptr = PNULL;

/*IM_ESTONIAN_SUPPORT*/
LOCAL uint8 *s_l0137b00_ptr = PNULL;
LOCAL uint8 *s_l0137b01_ptr = PNULL;
LOCAL uint8 *s_l0137b02_ptr = PNULL;
LOCAL uint8 *s_l0137b03_ptr = PNULL;
LOCAL uint8 *s_l0137b04_ptr = PNULL;
LOCAL uint8 *s_l0137b05_ptr = PNULL;
LOCAL uint8 *s_l0137b06_ptr = PNULL;
LOCAL uint8 *s_l0137b07_ptr = PNULL;

/*IM_FINNISH_SUPPORT*/
LOCAL uint8 *s_l0111b00_ptr = PNULL;
LOCAL uint8 *s_l0111b01_ptr = PNULL;
LOCAL uint8 *s_l0111b02_ptr = PNULL;
LOCAL uint8 *s_l0111b03_ptr = PNULL;
LOCAL uint8 *s_l0111b04_ptr = PNULL;
LOCAL uint8 *s_l0111b05_ptr = PNULL;

/*IM_FRENCH_SUPPORT*/
LOCAL uint8 *s_l0112b00_ptr = PNULL;
LOCAL uint8 *s_l0112b01_ptr = PNULL;
LOCAL uint8 *s_l0112b02_ptr = PNULL;

/*IM_GERMAN_SUPPORT*/
LOCAL uint8 *s_l0107b00_ptr = PNULL;
LOCAL uint8 *s_l0107b01_ptr = PNULL;
LOCAL uint8 *s_l0107b02_ptr = PNULL;

/*IM_GREEK_SUPPORT*/
LOCAL uint8 *s_l0108b00_ptr = PNULL;
LOCAL uint8 *s_l0108b01_ptr = PNULL;
LOCAL uint8 *s_l0108b02_ptr = PNULL;
LOCAL uint8 *s_l0108b03_ptr = PNULL;
LOCAL uint8 *s_l0108b04_ptr = PNULL;
LOCAL uint8 *s_l0108b05_ptr = PNULL;

/*IM_GUJARATI_SUPPORT*/
LOCAL uint8 *s_l0187b00_ptr = PNULL;
LOCAL uint8 *s_l0187b01_ptr = PNULL;
LOCAL uint8 *s_l0187b02_ptr = PNULL;
LOCAL uint8 *s_l0187b03_ptr = PNULL;
LOCAL uint8 *s_l0187b04_ptr = PNULL;
LOCAL uint8 *s_l0187b05_ptr = PNULL;
LOCAL uint8 *s_l0187b06_ptr = PNULL;
LOCAL uint8 *s_l0187b07_ptr = PNULL;

/*IM_HAUSA_SUPPORT*/
LOCAL uint8 *s_l0188b00_ptr = PNULL;
LOCAL uint8 *s_l0188b01_ptr = PNULL;
LOCAL uint8 *s_l0188b02_ptr = PNULL;

/*IM_HEBREW_SUPPORT*/
LOCAL uint8 *s_l0113b00_ptr = PNULL;
LOCAL uint8 *s_l0113b01_ptr = PNULL;
LOCAL uint8 *s_l0113b02_ptr = PNULL;
LOCAL uint8 *s_l0113b03_ptr = PNULL;
LOCAL uint8 *s_l0113b04_ptr = PNULL;
LOCAL uint8 *s_l0113b05_ptr = PNULL;
LOCAL uint8 *s_l0113b06_ptr = PNULL;
LOCAL uint8 *s_l0113b07_ptr = PNULL;
LOCAL uint8 *s_l0113b08_ptr = PNULL;

//#ifdef IM_HINDI_SUPPORT
LOCAL uint8 *s_l0157b00_ptr = PNULL;
LOCAL uint8 *s_l0157b01_ptr = PNULL;
//#endif

//#ifdef IM_HUNGARIAN_SUPPORT
LOCAL uint8 *s_l0114b00_ptr = PNULL;
LOCAL uint8 *s_l0114b01_ptr = PNULL;
LOCAL uint8 *s_l0114b02_ptr = PNULL;
LOCAL uint8 *s_l0114b03_ptr = PNULL;
LOCAL uint8 *s_l0114b04_ptr = PNULL;
LOCAL uint8 *s_l0114b05_ptr = PNULL;
LOCAL uint8 *s_l0114b06_ptr = PNULL;
//#endif

//#ifdef IM_ICELANDIC_SUPPORT
LOCAL uint8 *s_l0115b00_ptr = PNULL;
LOCAL uint8 *s_l0115b01_ptr = PNULL;
LOCAL uint8 *s_l0115b02_ptr = PNULL;
//#endif

//#ifdef IM_IGBO_SUPPORT
LOCAL uint8 *s_l01149b00_ptr = PNULL;
//#endif

//#ifdef IM_INDONESIAN_SUPPORT
LOCAL uint8 *s_l0133b00_ptr = PNULL;
LOCAL uint8 *s_l0133b01_ptr = PNULL;
LOCAL uint8 *s_l0133b02_ptr = PNULL;
LOCAL uint8 *s_l0133b03_ptr = PNULL;
//#endif

//#ifdef IM_ITALIAN_SUPPORT
LOCAL uint8 *s_l0116b00_ptr = PNULL;
LOCAL uint8 *s_l0116b01_ptr = PNULL;
LOCAL uint8 *s_l0116b02_ptr = PNULL;
LOCAL uint8 *s_l0116b03_ptr = PNULL;
//#endif

//#ifdef IM_KANNADA_SUPPORT
LOCAL uint8 *s_l0199b00_ptr = PNULL;
LOCAL uint8 *s_l0199b01_ptr = PNULL;
LOCAL uint8 *s_l0199b02_ptr = PNULL;
LOCAL uint8 *s_l0199b03_ptr = PNULL;
LOCAL uint8 *s_l0199b04_ptr = PNULL;
LOCAL uint8 *s_l0199b05_ptr = PNULL;
LOCAL uint8 *s_l0199b06_ptr = PNULL;
LOCAL uint8 *s_l0199b07_ptr = PNULL;
//#endif

//#ifdef IM_KAZAKH_SUPPORT
LOCAL uint8 *s_l0197b00_ptr = PNULL;
LOCAL uint8 *s_l0197b01_ptr = PNULL;
LOCAL uint8 *s_l0197b02_ptr = PNULL;
LOCAL uint8 *s_l0197b03_ptr = PNULL;
LOCAL uint8 *s_l0197b04_ptr = PNULL;
//#endif

//#ifdef IM_KHMER_SUPPORT
LOCAL uint8 *s_l0144b00_ptr = PNULL;
LOCAL uint8 *s_l0144b01_ptr = PNULL;
LOCAL uint8 *s_l0144b02_ptr = PNULL;
LOCAL uint8 *s_l0144b03_ptr = PNULL;
LOCAL uint8 *s_l0144b04_ptr = PNULL;
LOCAL uint8 *s_l0144b05_ptr = PNULL;
LOCAL uint8 *s_l0144b06_ptr = PNULL;
LOCAL uint8 *s_l0144b07_ptr = PNULL;
LOCAL uint8 *s_l0144b08_ptr = PNULL;
//#endif

//#ifdef IM_LATVIAN_SUPPORT
LOCAL uint8 *s_l0138b00_ptr = PNULL;
LOCAL uint8 *s_l0138b01_ptr = PNULL;
LOCAL uint8 *s_l0138b02_ptr = PNULL;
LOCAL uint8 *s_l0138b03_ptr = PNULL;
LOCAL uint8 *s_l0138b04_ptr = PNULL;
LOCAL uint8 *s_l0138b05_ptr = PNULL;
LOCAL uint8 *s_l0138b06_ptr = PNULL;
//#endif

//#ifdef IM_LITHUANIAN_SUPPORT
LOCAL uint8 *s_l0139b00_ptr = PNULL;
LOCAL uint8 *s_l0139b01_ptr = PNULL;
LOCAL uint8 *s_l0139b02_ptr = PNULL;
LOCAL uint8 *s_l0139b03_ptr = PNULL;
LOCAL uint8 *s_l0139b04_ptr = PNULL;
LOCAL uint8 *s_l0139b05_ptr = PNULL;
LOCAL uint8 *s_l0139b06_ptr = PNULL;
//#endif

//#ifdef IM_MACEDONIAN_SUPPORT
LOCAL uint8 *s_l0147b00_ptr = PNULL;
LOCAL uint8 *s_l0147b01_ptr = PNULL;
LOCAL uint8 *s_l0147b02_ptr = PNULL;
LOCAL uint8 *s_l0147b03_ptr = PNULL;
LOCAL uint8 *s_l0147b04_ptr = PNULL;
//#endif
//#ifdef IM_MALAY_SUPPORT
LOCAL uint8 *s_l0162b00_ptr = PNULL;
LOCAL uint8 *s_l0162b01_ptr = PNULL;
LOCAL uint8 *s_l0162b02_ptr = PNULL;
//#endif

//#ifdef IM_MALAYALAM_SUPPORT
LOCAL uint8 *s_l01106b00_ptr = PNULL;
LOCAL uint8 *s_l01106b01_ptr = PNULL;
LOCAL uint8 *s_l01106b02_ptr = PNULL;
LOCAL uint8 *s_l01106b03_ptr = PNULL;
LOCAL uint8 *s_l01106b04_ptr = PNULL;
LOCAL uint8 *s_l01106b05_ptr = PNULL;
LOCAL uint8 *s_l01106b06_ptr = PNULL;
LOCAL uint8 *s_l01106b07_ptr = PNULL;
LOCAL uint8 *s_l01106b08_ptr = PNULL;
LOCAL uint8 *s_l01106b09_ptr = PNULL;
LOCAL uint8 *s_l01106b10_ptr = PNULL;
LOCAL uint8 *s_l01106b11_ptr = PNULL;
LOCAL uint8 *s_l01106b12_ptr = PNULL;
//#endif
//#ifdef IM_MARATHI_SUPPORT
LOCAL uint8 *s_l01109b00_ptr = PNULL;
LOCAL uint8 *s_l01109b01_ptr = PNULL;
LOCAL uint8 *s_l01109b02_ptr = PNULL;
LOCAL uint8 *s_l01109b03_ptr = PNULL;
LOCAL uint8 *s_l01109b04_ptr = PNULL;
LOCAL uint8 *s_l01109b05_ptr = PNULL;
LOCAL uint8 *s_l01109b06_ptr = PNULL;
//#endif

//#ifdef IM_NORWEGIAN_SUPPORT
LOCAL uint8 *s_l0120b00_ptr = PNULL;
LOCAL uint8 *s_l0120b01_ptr = PNULL;
LOCAL uint8 *s_l0120b02_ptr = PNULL;
LOCAL uint8 *s_l0120b03_ptr = PNULL;
//#endif

//#ifdef IM_ORIYA_SUPPORT
LOCAL uint8 *s_l01115b00_ptr = PNULL;
LOCAL uint8 *s_l01115b01_ptr = PNULL;
LOCAL uint8 *s_l01115b02_ptr = PNULL;
LOCAL uint8 *s_l01115b03_ptr = PNULL;
LOCAL uint8 *s_l01115b04_ptr = PNULL;
LOCAL uint8 *s_l01115b05_ptr = PNULL;
//#endif

//#ifdef IM_PERSIAN_SUPPORT
LOCAL uint8 *s_l0141b00_ptr = PNULL;
LOCAL uint8 *s_l0141b01_ptr = PNULL;
LOCAL uint8 *s_l0141b02_ptr = PNULL;
LOCAL uint8 *s_l0141b03_ptr = PNULL;
LOCAL uint8 *s_l0141b04_ptr = PNULL;
LOCAL uint8 *s_l0141b05_ptr = PNULL;
LOCAL uint8 *s_l0141b06_ptr = PNULL;
LOCAL uint8 *s_l0141b07_ptr = PNULL;
//#endif
//#ifdef IM_POLISH_SUPPORT
LOCAL uint8 *s_l0121b00_ptr = PNULL;
LOCAL uint8 *s_l0121b01_ptr = PNULL;
LOCAL uint8 *s_l0121b02_ptr = PNULL;
LOCAL uint8 *s_l0121b03_ptr = PNULL;
LOCAL uint8 *s_l0121b04_ptr = PNULL;
//#endif
//#ifdef IM_PORTUGUESE_SUPPORT
LOCAL uint8 *s_l0122b00_ptr = PNULL;
LOCAL uint8 *s_l0122b01_ptr = PNULL;
LOCAL uint8 *s_l0122b02_ptr = PNULL;
//#endif
//#ifdef IM_PUNJABI_SUPPORT
LOCAL uint8 *s_l01116b00_ptr = PNULL;
LOCAL uint8 *s_l01116b01_ptr = PNULL;
LOCAL uint8 *s_l01116b02_ptr = PNULL;
LOCAL uint8 *s_l01116b03_ptr = PNULL;
//#endif
//#ifdef IM_ROMANIAN_SUPPORT
LOCAL uint8 *s_l0124b00_ptr = PNULL;
LOCAL uint8 *s_l0124b01_ptr = PNULL;
LOCAL uint8 *s_l0124b02_ptr = PNULL;
LOCAL uint8 *s_l0124b03_ptr = PNULL;
//#endif
//#ifdef IM_RUSSIAN_SUPPORT
LOCAL uint8 *s_l0125b00_ptr = PNULL;
LOCAL uint8 *s_l0125b01_ptr = PNULL;
LOCAL uint8 *s_l0125b02_ptr = PNULL;
LOCAL uint8 *s_l0125b03_ptr = PNULL;
LOCAL uint8 *s_l0125b04_ptr = PNULL;
LOCAL uint8 *s_l0125b05_ptr = PNULL;
LOCAL uint8 *s_l0125b06_ptr = PNULL;
//#endif
//#ifdef IM_SERBIAN_SUPPORT
LOCAL uint8 *s_l01128b00_ptr = PNULL;
LOCAL uint8 *s_l01128b01_ptr = PNULL;
LOCAL uint8 *s_l01128b02_ptr = PNULL;
LOCAL uint8 *s_l01128b03_ptr = PNULL;
//#endif
//#ifdef IM_SESOTHO_SUPPORT
LOCAL uint8 *s_l01130b00_ptr = PNULL;
LOCAL uint8 *s_l01130b01_ptr = PNULL;
//#endif
//#ifdef IM_SINHALESE_SUPPORT
LOCAL uint8 *s_l01124b00_ptr = PNULL;
LOCAL uint8 *s_l01124b01_ptr = PNULL;
LOCAL uint8 *s_l01124b02_ptr = PNULL;
LOCAL uint8 *s_l01124b03_ptr = PNULL;
LOCAL uint8 *s_l01124b04_ptr = PNULL;
LOCAL uint8 *s_l01124b05_ptr = PNULL;
LOCAL uint8 *s_l01124b06_ptr = PNULL;
LOCAL uint8 *s_l01124b07_ptr = PNULL;
LOCAL uint8 *s_l01124b08_ptr = PNULL;
LOCAL uint8 *s_l01124b09_ptr = PNULL;
LOCAL uint8 *s_l01124b10_ptr = PNULL;
LOCAL uint8 *s_l01124b11_ptr = PNULL;
LOCAL uint8 *s_l01124b12_ptr = PNULL;
LOCAL uint8 *s_l01124b13_ptr = PNULL;
LOCAL uint8 *s_l01124b14_ptr = PNULL;
LOCAL uint8 *s_l01124b15_ptr = PNULL;
//#endif
//#ifdef IM_SLOVAK_SUPPORT
LOCAL uint8 *s_l0127b00_ptr = PNULL;
LOCAL uint8 *s_l0127b01_ptr = PNULL;
LOCAL uint8 *s_l0127b02_ptr = PNULL;
LOCAL uint8 *s_l0127b03_ptr = PNULL;
LOCAL uint8 *s_l0127b04_ptr = PNULL;
LOCAL uint8 *s_l0127b05_ptr = PNULL;
LOCAL uint8 *s_l0127b06_ptr = PNULL;
//#endif
//#ifdef IM_SLOVENIAN_SUPPORT
LOCAL uint8 *s_l0136b00_ptr = PNULL;
LOCAL uint8 *s_l0136b01_ptr = PNULL;
LOCAL uint8 *s_l0136b02_ptr = PNULL;
LOCAL uint8 *s_l0136b03_ptr = PNULL;
//#endif
//#ifdef IM_SPANISH_SUPPORT
LOCAL uint8 *s_l0110b00_ptr = PNULL;
LOCAL uint8 *s_l0110b01_ptr = PNULL;
LOCAL uint8 *s_l0110b02_ptr = PNULL;
//#endif
//#ifdef IM_SWAHILI_SUPPORT
LOCAL uint8 *s_l0165b00_ptr = PNULL;
LOCAL uint8 *s_l0165b01_ptr = PNULL;
LOCAL uint8 *s_l0165b02_ptr = PNULL;
LOCAL uint8 *s_l0165b03_ptr = PNULL;
LOCAL uint8 *s_l0165b04_ptr = PNULL;
LOCAL uint8 *s_l0165b05_ptr = PNULL;
LOCAL uint8 *s_l0165b06_ptr = PNULL;
//#endif
//#ifdef IM_SWEDISH_SUPPORT
LOCAL uint8 *s_l0129b00_ptr = PNULL;
LOCAL uint8 *s_l0129b01_ptr = PNULL;
LOCAL uint8 *s_l0129b02_ptr = PNULL;
LOCAL uint8 *s_l0129b03_ptr = PNULL;
//#endif
//#ifdef IM_TAGALOG_SUPPORT
LOCAL uint8 *s_l01137b00_ptr = PNULL;
LOCAL uint8 *s_l01137b01_ptr = PNULL;
LOCAL uint8 *s_l01137b02_ptr = PNULL;
//#endif
//#ifdef IM_TAMIL_SUPPORT
LOCAL uint8 *s_l01132b00_ptr = PNULL;
LOCAL uint8 *s_l01132b01_ptr = PNULL;
LOCAL uint8 *s_l01132b02_ptr = PNULL;
LOCAL uint8 *s_l01132b03_ptr = PNULL;
LOCAL uint8 *s_l01132b04_ptr = PNULL;
LOCAL uint8 *s_l01132b05_ptr = PNULL;
LOCAL uint8 *s_l01132b06_ptr = PNULL;
LOCAL uint8 *s_l01132b07_ptr = PNULL;
LOCAL uint8 *s_l01132b08_ptr = PNULL;
LOCAL uint8 *s_l01132b09_ptr = PNULL;
LOCAL uint8 *s_l01132b10_ptr = PNULL;
LOCAL uint8 *s_l01132b11_ptr = PNULL;
LOCAL uint8 *s_l01132b12_ptr = PNULL;
//#endif
//#ifdef IM_TELUGU_SUPPORT
LOCAL uint8 *s_l01133b00_ptr = PNULL;
LOCAL uint8 *s_l01133b01_ptr = PNULL;
LOCAL uint8 *s_l01133b02_ptr = PNULL;
LOCAL uint8 *s_l01133b03_ptr = PNULL;
LOCAL uint8 *s_l01133b04_ptr = PNULL;
LOCAL uint8 *s_l01133b05_ptr = PNULL;
LOCAL uint8 *s_l01133b06_ptr = PNULL;
LOCAL uint8 *s_l01133b07_ptr = PNULL;
LOCAL uint8 *s_l01133b08_ptr = PNULL;
LOCAL uint8 *s_l01133b09_ptr = PNULL;
LOCAL uint8 *s_l01133b10_ptr = PNULL;
LOCAL uint8 *s_l01133b11_ptr = PNULL;
//#endif
//#ifdef IM_THAI_SUPPORT
LOCAL uint8 *s_l0130b00_ptr = PNULL;
LOCAL uint8 *s_l0130b01_ptr = PNULL;
LOCAL uint8 *s_l0130b02_ptr = PNULL;
LOCAL uint8 *s_l0130b03_ptr = PNULL;
//#endif
//#ifdef IM_TURKISH_SUPPORT
LOCAL uint8 *s_l0131b00_ptr = PNULL;
LOCAL uint8 *s_l0131b01_ptr = PNULL;
LOCAL uint8 *s_l0131b02_ptr = PNULL;
LOCAL uint8 *s_l0131b03_ptr = PNULL;
LOCAL uint8 *s_l0131b04_ptr = PNULL;
LOCAL uint8 *s_l0131b05_ptr = PNULL;
LOCAL uint8 *s_l0131b06_ptr = PNULL;
LOCAL uint8 *s_l0131b07_ptr = PNULL;
//#endif
//#ifdef IM_UKRAINIAN_SUPPORT
LOCAL uint8 *s_l0134b00_ptr = PNULL;
LOCAL uint8 *s_l0134b01_ptr = PNULL;
LOCAL uint8 *s_l0134b02_ptr = PNULL;
LOCAL uint8 *s_l0134b03_ptr = PNULL;
LOCAL uint8 *s_l0134b04_ptr = PNULL;
LOCAL uint8 *s_l0134b05_ptr = PNULL;
LOCAL uint8 *s_l0134b06_ptr = PNULL;
LOCAL uint8 *s_l0134b07_ptr = PNULL;
//#endif
//#ifdef IM_URDU_SUPPORT
LOCAL uint8 *s_l0132b00_ptr = PNULL;
LOCAL uint8 *s_l0132b01_ptr = PNULL;
LOCAL uint8 *s_l0132b02_ptr = PNULL;
//#endif
//#ifdef IM_VIETNAMESE_SUPPORT
LOCAL uint8 *s_l0142b00_ptr = PNULL;
//#endif
//#ifdef IM_XHOSA_SUPPORT
LOCAL uint8 *s_l0152b00_ptr = PNULL;
LOCAL uint8 *s_l0152b01_ptr = PNULL;
LOCAL uint8 *s_l0152b02_ptr = PNULL;
LOCAL uint8 *s_l0152b03_ptr = PNULL;
LOCAL uint8 *s_l0152b04_ptr = PNULL;
LOCAL uint8 *s_l0152b05_ptr = PNULL;
LOCAL uint8 *s_l0152b06_ptr = PNULL;
//#endif
//#ifdef IM_YORUBA_SUPPORT
LOCAL uint8 *s_l01147b00_ptr = PNULL;
LOCAL uint8 *s_l01147b01_ptr = PNULL;
//#endif
//#ifdef IM_ZULU_SUPPORT
LOCAL uint8 *s_l0153b00_ptr = PNULL;
LOCAL uint8 *s_l0153b01_ptr = PNULL;
LOCAL uint8 *s_l0153b02_ptr = PNULL;
LOCAL uint8 *s_l0153b03_ptr = PNULL;
LOCAL uint8 *s_l0153b04_ptr = PNULL;
LOCAL uint8 *s_l0153b05_ptr = PNULL;
//#endif
//#ifdef IM_PASHTO_SUPPORT
LOCAL uint8 *s_l01117b00_ptr = PNULL;
LOCAL uint8 *s_l01117b01_ptr = PNULL;
LOCAL uint8 *s_l01117b02_ptr = PNULL;
LOCAL uint8 *s_l01117b03_ptr = PNULL;
LOCAL uint8 *s_l01117b04_ptr = PNULL;
LOCAL uint8 *s_l01117b05_ptr = PNULL;
LOCAL uint8 *s_l01117b06_ptr = PNULL;
LOCAL uint8 *s_l01117b07_ptr = PNULL;
//#endif
//#ifdef IM_UZBEK_SUPPORT
LOCAL uint8 *s_l01143b00_ptr = PNULL;
LOCAL uint8 *s_l01143b01_ptr = PNULL;
LOCAL uint8 *s_l01143b02_ptr = PNULL;
LOCAL uint8 *s_l01143b03_ptr = PNULL;
LOCAL uint8 *s_l01143b04_ptr = PNULL;
LOCAL uint8 *s_l01143b05_ptr = PNULL;
//#endif
/*chs*/
LOCAL uint8 *s_l0204b00_ptr = PNULL;
LOCAL uint8 *s_l0204b01_ptr = PNULL;
LOCAL uint8 *s_l0204b02_ptr = PNULL;
LOCAL uint8 *s_l0204b08_ptr = PNULL;
LOCAL uint8 *s_l0204b09_ptr = PNULL;
LOCAL uint8 *s_l0204b10_ptr = PNULL;
LOCAL uint8 *s_l0204b11_ptr = PNULL;
LOCAL uint8 *s_l0204b17_ptr = PNULL;
LOCAL uint8 *s_l0204b18_ptr = PNULL;
LOCAL uint8 *s_l0204b19_ptr = PNULL;
LOCAL uint8 *s_l0204b25_ptr = PNULL;
LOCAL uint8 *s_l0204b26_ptr = PNULL;
LOCAL uint8 *s_l0204b27_ptr = PNULL;
LOCAL uint8 *s_l0204b37_ptr = PNULL;
LOCAL uint8 *s_l0204b38_ptr = PNULL;
LOCAL uint8 *s_l0204b41_ptr = PNULL;
LOCAL uint8 *s_l0204b42_ptr = PNULL;
LOCAL uint8 *s_l0204b43_ptr = PNULL;
LOCAL uint8 *s_l0204b46_ptr = PNULL;
LOCAL uint8 *s_l0204b47_ptr = PNULL;
LOCAL uint8 *s_l0204b48_ptr = PNULL;
LOCAL uint8 *s_l0204b49_ptr = PNULL;
LOCAL uint8 *s_l0204b50_ptr = PNULL;
LOCAL uint8 *s_l0204b51_ptr = PNULL;
LOCAL uint8 *s_l0204b52_ptr = PNULL;
LOCAL uint8 *s_l0204b59_ptr = PNULL;
LOCAL uint8 *s_l0204b60_ptr = PNULL;
LOCAL uint8 *s_l0204b61_ptr = PNULL;
LOCAL uint8 *s_l0204b64_ptr = PNULL;
/*trad chinese tw*/
LOCAL uint8 *s_l0104b00_ptr = PNULL;
LOCAL uint8 *s_l0104b01_ptr = PNULL;
LOCAL uint8 *s_l0104b02_ptr = PNULL;
LOCAL uint8 *s_l0104b08_ptr = PNULL;
LOCAL uint8 *s_l0104b09_ptr = PNULL;
LOCAL uint8 *s_l0104b10_ptr = PNULL;
LOCAL uint8 *s_l0104b11_ptr = PNULL;
LOCAL uint8 *s_l0104b17_ptr = PNULL;
LOCAL uint8 *s_l0104b18_ptr = PNULL;
LOCAL uint8 *s_l0104b19_ptr = PNULL;
LOCAL uint8 *s_l0104b25_ptr = PNULL;
LOCAL uint8 *s_l0104b26_ptr = PNULL;
LOCAL uint8 *s_l0104b27_ptr = PNULL;
LOCAL uint8 *s_l0104b37_ptr = PNULL;
LOCAL uint8 *s_l0104b38_ptr = PNULL;
LOCAL uint8 *s_l0104b41_ptr = PNULL;
LOCAL uint8 *s_l0104b42_ptr = PNULL;
LOCAL uint8 *s_l0104b43_ptr = PNULL;
LOCAL uint8 *s_l0104b46_ptr = PNULL;
LOCAL uint8 *s_l0104b47_ptr = PNULL;
LOCAL uint8 *s_l0104b48_ptr = PNULL;
LOCAL uint8 *s_l0104b49_ptr = PNULL;
LOCAL uint8 *s_l0104b50_ptr = PNULL;
LOCAL uint8 *s_l0104b59_ptr = PNULL;
LOCAL uint8 *s_l0104b60_ptr = PNULL;
LOCAL uint8 *s_l0104b61_ptr = PNULL;
LOCAL uint8 *s_l0104b64_ptr = PNULL;

extern BOOLEAN s_lang_status_info[MMISET_MAX_LANGUAGE + 1][2];
/*****************************************************************************/
//  Description : init ldb lang data info
//  Global resource dependence : none
//  Author:haiwu.chen
//  Note: 读取输入法的LDB info
/*****************************************************************************/

PUBLIC BOOLEAN MMIIM_T9_InitLDBInfo(MMIIM_T9_DATA_T *data_ptr)
{
    MMIRES_DATA_INFO_T file_info = {0};
    BOOLEAN is_ret = FALSE;
   	GUIIM_LANGUAGE_T input_lang = GUIIM_LANG_NONE;
    uint32 offset = 0;
    
    input_lang = data_ptr->init_param.method_param_ptr->im_set.lang;
	//input_lang = MMIAPIIM_GetLanguage();
	SCI_TRACE_LOW("[MMIIM] MMIIM_T9_InitCHSLDBInfo : input_lang=%d.", input_lang);
    
    if(((s_lang_status_info[MMISET_LANGUAGE_ENGLISH][1]) && (GUIIM_LANG_ENGLISH == input_lang))
        || ((s_lang_status_info[MMISET_LANGUAGE_AMERICAN][1]) && (GUIIM_LANG_AMERICAN == input_lang)) )
    {
 #ifdef WIN32
    	/*Get l0109b00_s*/
        is_ret = MMI_GetDataInfo(T9_LDB_ENGLISH_l0109b00_s, &file_info);
        if (is_ret) 
        {
            s_l0109b00_s_ptr = PNULL;
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0109b00_s_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
            //s_l0109b00_s_ptr = file_info.data_ptr;
        }
        /*Reset file_info*/
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        /*Get l0109b01*/
        is_ret = MMI_GetDataInfo(T9_LDB_ENGLISH_l0109b01_s, &file_info);
        if (is_ret) 
        {
            s_l0109b01_s_ptr = PNULL;
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0109b01_s_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
            //s_l0109b00_s_ptr = file_info.data_ptr;
        }
        /*Reset file_info*/
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        /*Get l0109b02*/
        is_ret = MMI_GetDataInfo(T9_LDB_ENGLISH_l0109b02_s, &file_info);
        if (is_ret) 
        {
            s_l0109b02_s_ptr = PNULL;
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0109b02_s_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
            //s_l0109b00_s_ptr = file_info.data_ptr;
        }
#else
        /*Get l0109b00*/
        is_ret = MMI_GetDataInfo(T9_LDB_ENGLISH_l0109b00, &file_info);
        if (is_ret) 
        {
            s_l0109b00_ptr = PNULL;
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0109b00_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
            //s_l0109b00_ptr = file_info.data_ptr;
        }
        /*Reset file_info*/
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        /*Get l0109b01*/
        is_ret = MMI_GetDataInfo(T9_LDB_ENGLISH_l0109b01, &file_info);
        if (is_ret) 
        {
            s_l0109b01_ptr = PNULL;
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0109b01_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
            //s_l0109b00_ptr = file_info.data_ptr;
        }
        /*Reset file_info*/
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        /*Get l0109b02*/
        is_ret = MMI_GetDataInfo(T9_LDB_ENGLISH_l0109b02, &file_info);
        if (is_ret) 
        {
            s_l0109b02_ptr = PNULL;
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0109b02_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
            //s_l0109b00_ptr = file_info.data_ptr;
        }
#endif/*WIN32*/

    }

    /*IM_AFRIKAANS_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_AFRIKAANS][1]) && (GUIIM_LANG_AFRIKAANS == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_AFRIKAANS_l0154b00, &file_info);
        if (is_ret) 
        {
            s_l0154b00_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_AFRIKAANS_l0154b01, &file_info);
        if (is_ret) 
        {
            s_l0154b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_AFRIKAANS_l0154b02, &file_info);
        if (is_ret) 
        {
            s_l0154b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_AFRIKAANS_l0154b03, &file_info);
        if (is_ret) 
        {
            s_l0154b03_ptr = file_info.data_ptr;
        }
    }
    /*IM_AMHARIC_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_AMHARIC][1]) && (GUIIM_LANG_AMHARIC == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_AMHARIC_l0168b00, &file_info);
        if (is_ret) 
        {
            s_l0168b00_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_AMHARIC_l0168b01, &file_info);
        if (is_ret) 
        {
            s_l0168b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_AMHARIC_l0168b02, &file_info);
        if (is_ret) 
        {
            s_l0168b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_AMHARIC_l0168b03, &file_info);
        if (is_ret) 
        {
            s_l0168b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_AMHARIC_l0168b04, &file_info);
        if (is_ret) 
        {
            s_l0168b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_AMHARIC_l0168b05, &file_info);
        if (is_ret) 
        {
            s_l0168b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_AMHARIC_l0168b06, &file_info);
        if (is_ret) 
        {
            s_l0168b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_AMHARIC_l0168b07, &file_info);
        if (is_ret) 
        {
            s_l0168b07_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_AMHARIC_l0168b08, &file_info);
        if (is_ret) 
        {
            s_l0168b08_ptr = file_info.data_ptr;
        }
    }
	/*IM_ALBANIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_ALBANIAN][1]) && (GUIIM_LANG_ALBANIAN== input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_ALBANIAN_l0128b00, &file_info);
        if (is_ret) 
    	{
    		s_l0128b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ALBANIAN_l0128b01, &file_info);
        if (is_ret) 
    	{
    		s_l0128b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ALBANIAN_l0128b02, &file_info);
        if (is_ret) 
    	{
    		s_l0128b02_ptr = file_info.data_ptr;
        }
    }
    /*IM_ARABIC_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_ARABIC][1]) && (GUIIM_LANG_ARABIC == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_ARABIC_l0101b00, &file_info);
        if (is_ret) 
        {
            s_l0101b00_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ARABIC_l0101b01, &file_info);
        if (is_ret) 
        {
            s_l0101b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ARABIC_l0101b02, &file_info);
        if (is_ret) 
        {
            s_l0101b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ARABIC_l0101b03, &file_info);
        if (is_ret) 
        {
            s_l0101b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ARABIC_l0101b04, &file_info);
        if (is_ret) 
        {
            s_l0101b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ARABIC_l0101b05, &file_info);
        if (is_ret) 
        {
            s_l0101b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ARABIC_l0101b06, &file_info);
        if (is_ret) 
        {
            s_l0101b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ARABIC_l0101b07, &file_info);
        if (is_ret) 
        {
            s_l0101b07_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ARABIC_l0101b08, &file_info);
        if (is_ret) 
        {
            s_l0101b08_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ARABIC_l0101b09, &file_info);
        if (is_ret) 
        {
            s_l0101b09_ptr = file_info.data_ptr;
        }
    }

    /*IM_ASSAMESE_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_ASSAMESE][1]) && (GUIIM_LANG_ASSAMESE == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_ASSAMESE_l0169b00, &file_info);
        if (is_ret) 
        {
            s_l0169b00_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ASSAMESE_l0169b01, &file_info);
        if (is_ret) 
        {
            s_l0169b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ASSAMESE_l0169b02, &file_info);
        if (is_ret) 
        {
            s_l0169b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ASSAMESE_l0169b03, &file_info);
        if (is_ret) 
        {
            s_l0169b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ASSAMESE_l0169b04, &file_info);
        if (is_ret) 
        {
            s_l0169b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ASSAMESE_l0169b05, &file_info);
        if (is_ret) 
        {
            s_l0169b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ASSAMESE_l0169b06, &file_info);
        if (is_ret) 
        {
            s_l0169b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ASSAMESE_l0169b07, &file_info);
        if (is_ret) 
        {
            s_l0169b07_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ASSAMESE_l0169b08, &file_info);
        if (is_ret) 
        {
            s_l0169b08_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_ASSAMESE_l0169b09, &file_info);
        if (is_ret) 
        {
            s_l0169b09_ptr = file_info.data_ptr;
        }
        
    }

    /*IM_AZERBAIJANI_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_AZERBAIJANI][1]) && (GUIIM_LANG_AZERBAIJANI == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_AZERBAIJANI_l0171b00, &file_info);
        if (is_ret) 
    	{
    		s_l0171b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_AZERBAIJANI_l0171b01, &file_info);
        if (is_ret) 
    	{
    		s_l0171b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_AZERBAIJANI_l0171b02, &file_info);
        if (is_ret) 
    	{
    		s_l0171b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_AZERBAIJANI_l0171b03, &file_info);
        if (is_ret) 
    	{
    		s_l0171b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_AZERBAIJANI_l0171b04, &file_info);
        if (is_ret) 
    	{
    		s_l0171b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_AZERBAIJANI_l0171b05, &file_info);
        if (is_ret) 
    	{
    		s_l0171b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_AZERBAIJANI_l0171b06, &file_info);
        if (is_ret) 
    	{
    		s_l0171b06_ptr = file_info.data_ptr;
        }
    }

	/*IM_BOSNIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_BOSNIAN][1]) && (GUIIM_LANG_BOSNIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_BOSNIAN_l01151b00, &file_info);
        if (is_ret) 
    	{
    		s_l01151b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BOSNIAN_l01151b01, &file_info);
        if (is_ret) 
    	{
    		s_l01151b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BOSNIAN_l01151b02, &file_info);
        if (is_ret) 
    	{
    		s_l01151b02_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BOSNIAN_l01151b03, &file_info);
        if (is_ret) 
    	{
    		s_l01151b03_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BOSNIAN_l01151b04, &file_info);
        if (is_ret) 
    	{
    		s_l01151b04_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BOSNIAN_l01151b05, &file_info);
        if (is_ret) 
    	{
    		s_l01151b05_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BOSNIAN_l01151b06, &file_info);
        if (is_ret) 
    	{
    		s_l01151b06_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BOSNIAN_l01151b07, &file_info);
        if (is_ret) 
    	{
    		s_l01151b07_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BOSNIAN_l01151b08, &file_info);
        if (is_ret) 
    	{
    		s_l01151b08_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BOSNIAN_l01151b09, &file_info);
        if (is_ret) 
    	{
    		s_l01151b09_ptr = file_info.data_ptr;
        }
    }


    /*IM_BULGARIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_BULGARIAN][1]) && (GUIIM_LANG_BULGARIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_BULGARIAN_l0102b00, &file_info);
        if (is_ret) 
    	{
    		s_l0102b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BULGARIAN_l0102b01, &file_info);
        if (is_ret) 
    	{
    		s_l0102b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BULGARIAN_l0102b02, &file_info);
        if (is_ret) 
    	{
    		s_l0102b02_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BULGARIAN_l0102b03, &file_info);
        if (is_ret) 
    	{
    		s_l0102b03_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BULGARIAN_l0102b04, &file_info);
        if (is_ret) 
    	{
    		s_l0102b04_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BULGARIAN_l0102b05, &file_info);
        if (is_ret) 
    	{
    		s_l0102b05_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BULGARIAN_l0102b06, &file_info);
        if (is_ret) 
    	{
    		s_l0102b06_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BULGARIAN_l0102b07, &file_info);
        if (is_ret) 
    	{
    		s_l0102b07_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BULGARIAN_l0102b08, &file_info);
        if (is_ret) 
    	{
    		s_l0102b08_ptr = file_info.data_ptr;
        }
    }

    /*IM_BENGALI_SUPPORT*/
    if(    ((s_lang_status_info[MMISET_LANGUAGE_BENGALI][1]) && (GUIIM_LANG_BENGALI == input_lang))
	|| ((s_lang_status_info[MMISET_LANGUAGE_BENGALI_BD][1]) && (GUIIM_LANG_BENGALI_BD== input_lang)))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_BENGALI_l0175b00, &file_info);
        if (is_ret) 
    	{
    		s_l0175b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BENGALI_l0175b01, &file_info);
        if (is_ret) 
    	{
    		s_l0175b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BENGALI_l0175b02, &file_info);
        if (is_ret) 
    	{
    		s_l0175b02_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BENGALI_l0175b03, &file_info);
        if (is_ret) 
    	{
    		s_l0175b03_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BENGALI_l0175b04, &file_info);
        if (is_ret) 
    	{
    		s_l0175b04_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BENGALI_l0175b05, &file_info);
        if (is_ret) 
    	{
    		s_l0175b05_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BENGALI_l0175b06, &file_info);
        if (is_ret) 
    	{
    		s_l0175b06_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BENGALI_l0175b07, &file_info);
        if (is_ret) 
    	{
    		s_l0175b07_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_BENGALI_l0175b08, &file_info);
        if (is_ret) 
    	{
    		s_l0175b08_ptr = file_info.data_ptr;
        }
    }

    /*IM_CROATIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_CROATIAN][1]) && (GUIIM_LANG_CROATIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_CROATIAN_l0189b00, &file_info);
        if (is_ret) 
    	{
    		s_l0189b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_CROATIAN_l0189b01, &file_info);
        if (is_ret) 
    	{
    		s_l0189b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_CROATIAN_l0189b02, &file_info);
        if (is_ret) 
    	{
    		s_l0189b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_CROATIAN_l0189b03, &file_info);
        if (is_ret) 
    	{
    		s_l0189b03_ptr = file_info.data_ptr;
        }
    }

    /*IM_CZECH_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_CZECH][1]) && (GUIIM_LANG_CZECH == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_CZECH_l0105b00, &file_info);
        if (is_ret) 
    	{
    		s_l0105b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_CZECH_l0105b01, &file_info);
        if (is_ret) 
    	{
    		s_l0105b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_CZECH_l0105b02, &file_info);
        if (is_ret) 
    	{
    		s_l0105b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_CZECH_l0105b03, &file_info);
        if (is_ret) 
    	{
    		s_l0105b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_CZECH_l0105b04, &file_info);
        if (is_ret) 
    	{
    		s_l0105b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_CZECH_l0105b05, &file_info);
        if (is_ret) 
    	{
    		s_l0105b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_CZECH_l0105b06, &file_info);
        if (is_ret) 
    	{
    		s_l0105b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_CZECH_l0105b07, &file_info);
        if (is_ret) 
    	{
    		s_l0105b07_ptr = file_info.data_ptr;
        }
    }

    /*IM_DANISH_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_DANISH][1]) && (GUIIM_LANG_DANISH == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_DANISH_l0106b00, &file_info);
        if (is_ret) 
    	{
    		s_l0106b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_DANISH_l0106b01, &file_info);
        if (is_ret) 
    	{
    		s_l0106b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_DANISH_l0106b02, &file_info);
        if (is_ret) 
    	{
    		s_l0106b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_DANISH_l0106b03, &file_info);
        if (is_ret) 
    	{
    		s_l0106b03_ptr = file_info.data_ptr;
        }
    }
    /*IM_DUTCH_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_DUTCH][1]) && (GUIIM_LANG_DUTCH == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_DUTCH_l0119b00, &file_info);
        if (is_ret) 
    	{
    		s_l0119b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_DUTCH_l0119b01, &file_info);
        if (is_ret) 
    	{
    		s_l0119b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_DUTCH_l0119b02, &file_info);
        if (is_ret) 
    	{
    		s_l0119b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_DUTCH_l0119b03, &file_info);
        if (is_ret) 
    	{
    		s_l0119b03_ptr = file_info.data_ptr;
        }

    }
    
    /*IM_ESTONIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_ESTONIAN][1]) && (GUIIM_LANG_ESTONIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_ESTONIAN_l0137b00, &file_info);
        if (is_ret) 
    	{
    		s_l0137b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ESTONIAN_l0137b01, &file_info);
        if (is_ret) 
    	{
    		s_l0137b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ESTONIAN_l0137b02, &file_info);
        if (is_ret) 
    	{
    		s_l0137b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ESTONIAN_l0137b03, &file_info);
        if (is_ret) 
    	{
    		s_l0137b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ESTONIAN_l0137b04, &file_info);
        if (is_ret) 
    	{
    		s_l0137b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ESTONIAN_l0137b05, &file_info);
        if (is_ret) 
    	{
    		s_l0137b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ESTONIAN_l0137b06, &file_info);
        if (is_ret) 
    	{
    		s_l0137b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ESTONIAN_l0137b07, &file_info);
        if (is_ret) 
    	{
    		s_l0137b07_ptr = file_info.data_ptr;
        }       
    }

    /*IM_FINNISH_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_FINNISH][1]) && (GUIIM_LANG_FINNISH == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_FINNISH_l0111b00, &file_info);
        if (is_ret) 
    	{
    		s_l0111b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_FINNISH_l0111b01, &file_info);
        if (is_ret) 
    	{
    		s_l0111b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_FINNISH_l0111b02, &file_info);
        if (is_ret) 
    	{
    		s_l0111b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_FINNISH_l0111b03, &file_info);
        if (is_ret) 
    	{
    		s_l0111b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_FINNISH_l0111b04, &file_info);
        if (is_ret) 
    	{
    		s_l0111b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_FINNISH_l0111b05, &file_info);
        if (is_ret) 
    	{
    		s_l0111b05_ptr = file_info.data_ptr;
        }
    }

    /*IM_FRENCH_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_FRENCH][1]) && (GUIIM_LANG_FRENCH == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_FRENCH_l0112b00, &file_info);
        if (is_ret) 
    	{
    		s_l0112b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_FRENCH_l0112b01, &file_info);
        if (is_ret) 
    	{
    		s_l0112b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_FRENCH_l0112b02, &file_info);
        if (is_ret) 
    	{
    		s_l0112b02_ptr = file_info.data_ptr;
        }       
    }
    /*IM_GERMAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_GERMAN][1]) && (GUIIM_LANG_GERMAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_GERMAN_l0107b00, &file_info);
        if (is_ret) 
    	{
    		s_l0107b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GERMAN_l0107b01, &file_info);
        if (is_ret) 
    	{
    		s_l0107b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GERMAN_l0107b02, &file_info);
        if (is_ret) 
    	{
    		s_l0107b02_ptr = file_info.data_ptr;
        } 
    }
    if((s_lang_status_info[MMISET_LANGUAGE_GREEK][1]) && (GUIIM_LANG_GREEK == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_GREEK_l0108b00, &file_info);
        if (is_ret) 
    	{
    		s_l0108b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GREEK_l0108b01, &file_info);
        if (is_ret) 
    	{
    		s_l0108b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GREEK_l0108b02, &file_info);
        if (is_ret) 
    	{
    		s_l0108b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GREEK_l0108b03, &file_info);
        if (is_ret) 
    	{
    		s_l0108b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GREEK_l0108b04, &file_info);
        if (is_ret) 
    	{
    		s_l0108b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GREEK_l0108b05, &file_info);
        if (is_ret) 
    	{
    		s_l0108b05_ptr = file_info.data_ptr;
        }        
    }

    /*IM_GUJARATI_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_GUJARATI][1]) && (GUIIM_LANG_GUJARATI == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_GUJARATI_l0187b00, &file_info);
        if (is_ret) 
    	{
    		s_l0187b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GUJARATI_l0187b01, &file_info);
        if (is_ret) 
    	{
    		s_l0187b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GUJARATI_l0187b02, &file_info);
        if (is_ret) 
    	{
    		s_l0187b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GUJARATI_l0187b03, &file_info);
        if (is_ret) 
    	{
    		s_l0187b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GUJARATI_l0187b04, &file_info);
        if (is_ret) 
    	{
    		s_l0187b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GUJARATI_l0187b05, &file_info);
        if (is_ret) 
    	{
    		s_l0187b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GUJARATI_l0187b06, &file_info);
        if (is_ret) 
    	{
    		s_l0187b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_GUJARATI_l0187b07, &file_info);
        if (is_ret) 
    	{
    		s_l0187b07_ptr = file_info.data_ptr;
        }
    }
    /*IM_HAUSA_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_HAUSA][1]) && (GUIIM_LANG_HAUSA == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_HAUSA_l0188b00, &file_info);
        if (is_ret) 
    	{
    		s_l0188b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HAUSA_l0188b01, &file_info);
        if (is_ret) 
    	{
    		s_l0188b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HAUSA_l0188b02, &file_info);
        if (is_ret) 
    	{
    		s_l0188b02_ptr = file_info.data_ptr;
        }

    }
    /*IM_HEBREW_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_HEBREW][1]) && (GUIIM_LANG_HEBREW == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_HEBREW_l0113b00, &file_info);
        if (is_ret) 
    	{
    		s_l0113b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HEBREW_l0113b01, &file_info);
        if (is_ret) 
    	{
    		s_l0113b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HEBREW_l0113b02, &file_info);
        if (is_ret) 
    	{
    		s_l0113b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HEBREW_l0113b03, &file_info);
        if (is_ret) 
    	{
    		s_l0113b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HEBREW_l0113b04, &file_info);
        if (is_ret) 
    	{
    		s_l0113b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HEBREW_l0113b05, &file_info);
        if (is_ret) 
    	{
    		s_l0113b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HEBREW_l0113b06, &file_info);
        if (is_ret) 
    	{
    		s_l0113b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HEBREW_l0113b07, &file_info);
        if (is_ret) 
    	{
    		s_l0113b07_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HEBREW_l0113b08, &file_info);
        if (is_ret) 
    	{
    		s_l0113b08_ptr = file_info.data_ptr;
        }
    }
    /*IM_HINDI_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_HINDI][1]) && (GUIIM_LANG_HINDI == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_HINDI_l0157B00, &file_info);
        if (is_ret) 
    	{
    		s_l0157b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HINDI_l0157B01, &file_info);
        if (is_ret) 
    	{
    		s_l0157b01_ptr = file_info.data_ptr;
        }
    }

    /*IM_HUNGARIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_HUNGARIAN][1]) && (GUIIM_LANG_HUNGARIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_HUNGARIAN_l0114B00, &file_info);
        if (is_ret) 
    	{
    		s_l0114b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HUNGARIAN_l0114B01, &file_info);
        if (is_ret) 
    	{
    		s_l0114b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HUNGARIAN_l0114B02, &file_info);
        if (is_ret) 
    	{
    		s_l0114b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HUNGARIAN_l0114B03, &file_info);
        if (is_ret) 
    	{
    		s_l0114b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HUNGARIAN_l0114B04, &file_info);
        if (is_ret) 
    	{
    		s_l0114b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HUNGARIAN_l0114B05, &file_info);
        if (is_ret) 
    	{
    		s_l0114b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_HUNGARIAN_l0114B06, &file_info);
        if (is_ret) 
    	{
    		s_l0114b06_ptr = file_info.data_ptr;
        }
    }
    
    /*IM_ICELANDIC_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_ICELANDIC][1]) && (GUIIM_LANG_ICELANDIC == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_ICELANDIC_l0115B00, &file_info);
        if (is_ret) 
    	{
    		s_l0115b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ICELANDIC_l0115B01, &file_info);
        if (is_ret) 
    	{
    		s_l0115b01_ptr = file_info.data_ptr;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ICELANDIC_l0115B02, &file_info);
        if (is_ret) 
    	{
    		s_l0115b02_ptr = file_info.data_ptr;
        }
    }

    /*IM_IGBO_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_IGBO][1]) && (GUIIM_LANG_IGBO == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_IGBO_l01149B00, &file_info);
        if (is_ret) 
    	{
    		s_l01149b00_ptr = file_info.data_ptr;
        }
    }
    
    /*IM_INDONESIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_INDONESIAN][1]) && (GUIIM_LANG_INDONESIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_INDONESIAN_l0133B00, &file_info);
        if (is_ret) 
    	{
    		s_l0133b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_INDONESIAN_l0133B01, &file_info);
        if (is_ret) 
    	{
    		s_l0133b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_INDONESIAN_l0133B02, &file_info);
        if (is_ret) 
    	{
    		s_l0133b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_INDONESIAN_l0133B03, &file_info);
        if (is_ret) 
    	{
    		s_l0133b03_ptr = file_info.data_ptr;
        }
    }

    /*IM_ITALIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_ITALIAN][1]) && (GUIIM_LANG_ITALIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_ITALIAN_l0116B00, &file_info);
        if (is_ret) 
    	{
    		s_l0116b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ITALIAN_l0116B01, &file_info);
        if (is_ret) 
    	{
    		s_l0116b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ITALIAN_l0116B02, &file_info);
        if (is_ret) 
    	{
    		s_l0116b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ITALIAN_l0116B03, &file_info);
        if (is_ret) 
    	{
    		s_l0116b03_ptr = file_info.data_ptr;
        }
    }

    /*IM_KANNADA_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_KANNADA][1]) && (GUIIM_LANG_KANNADA == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_KANNADA_l0199b00, &file_info);
        if (is_ret) 
    	{
    		s_l0199b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KANNADA_l0199b01, &file_info);
        if (is_ret) 
    	{
    		s_l0199b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KANNADA_l0199b02, &file_info);
        if (is_ret) 
    	{
    		s_l0199b02_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KANNADA_l0199b03, &file_info);
        if (is_ret) 
    	{
    		s_l0199b03_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KANNADA_l0199b04, &file_info);
        if (is_ret) 
    	{
    		s_l0199b04_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KANNADA_l0199b05, &file_info);
        if (is_ret) 
    	{
    		s_l0199b05_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KANNADA_l0199b06, &file_info);
        if (is_ret) 
    	{
    		s_l0199b06_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KANNADA_l0199b07, &file_info);
        if (is_ret) 
    	{
    		s_l0199b07_ptr = file_info.data_ptr;
        }   
    }

    /*IM_KAZAKH_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_KAZAKH][1]) && (GUIIM_LANG_KAZAKH == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_KAZAKH_l0197B00, &file_info);
        if (is_ret) 
    	{
    		s_l0197b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KAZAKH_l0197B01, &file_info);
        if (is_ret) 
    	{
    		s_l0197b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KAZAKH_l0197B02, &file_info);
        if (is_ret) 
    	{
    		s_l0197b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KAZAKH_l0197B03, &file_info);
        if (is_ret) 
    	{
    		s_l0197b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KAZAKH_l0197B04, &file_info);
        if (is_ret) 
    	{
    		s_l0197b04_ptr = file_info.data_ptr;
        }
    }

    /*IM_KHMER_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_KHMER][1]) && (GUIIM_LANG_KHMER == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_KHMER_l0144B00, &file_info);
        if (is_ret) 
    	{
    		s_l0144b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KHMER_l0144B01, &file_info);
        if (is_ret) 
    	{
    		s_l0144b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KHMER_l0144B02, &file_info);
        if (is_ret) 
    	{
    		s_l0144b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KHMER_l0144B03, &file_info);
        if (is_ret) 
    	{
    		s_l0144b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KHMER_l0144B04, &file_info);
        if (is_ret) 
    	{
    		s_l0144b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KHMER_l0144B05, &file_info);
        if (is_ret) 
    	{
    		s_l0144b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KHMER_l0144B06, &file_info);
        if (is_ret) 
    	{
    		s_l0144b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KHMER_l0144B07, &file_info);
        if (is_ret) 
    	{
    		s_l0144b07_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_KHMER_l0144B08, &file_info);
        if (is_ret) 
    	{
    		s_l0144b08_ptr = file_info.data_ptr;
        }        
    }

    /*IM_LATVIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_LATVIAN][1]) && (GUIIM_LANG_LATVIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_LATVIAN_l0138B00, &file_info);
        if (is_ret) 
    	{
    		s_l0138b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LATVIAN_l0138B01, &file_info);
        if (is_ret) 
    	{
    		s_l0138b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LATVIAN_l0138B02, &file_info);
        if (is_ret) 
    	{
    		s_l0138b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LATVIAN_l0138B03, &file_info);
        if (is_ret) 
    	{
    		s_l0138b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LATVIAN_l0138B04, &file_info);
        if (is_ret) 
    	{
    		s_l0138b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LATVIAN_l0138B05, &file_info);
        if (is_ret) 
    	{
    		s_l0138b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LATVIAN_l0138B06, &file_info);
        if (is_ret) 
    	{
    		s_l0138b06_ptr = file_info.data_ptr;
        }
    }

    /*IM_LITHUANIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_LITHUANIAN][1]) && (GUIIM_LANG_LITHUANIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_LITHUANIAN_l0139B00, &file_info);
        if (is_ret) 
    	{
    		s_l0139b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LITHUANIAN_l0139B01, &file_info);
        if (is_ret) 
    	{
    		s_l0139b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LITHUANIAN_l0139B02, &file_info);
        if (is_ret) 
    	{
    		s_l0139b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LITHUANIAN_l0139B03, &file_info);
        if (is_ret) 
    	{
    		s_l0139b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LITHUANIAN_l0139B04, &file_info);
        if (is_ret) 
    	{
    		s_l0139b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LITHUANIAN_l0139B05, &file_info);
        if (is_ret) 
    	{
    		s_l0139b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_LITHUANIAN_l0139B06, &file_info);
        if (is_ret) 
    	{
    		s_l0139b06_ptr = file_info.data_ptr;
        }
    }

    /*IM_MACEDONIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_MACEDONIAN][1]) && (GUIIM_LANG_MACEDONIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_MACEDONIAN_l0147B00, &file_info);
        if (is_ret) 
    	{
    		s_l0147b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MACEDONIAN_l0147B01, &file_info);
        if (is_ret) 
    	{
    		s_l0147b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MACEDONIAN_l0147B02, &file_info);
        if (is_ret) 
    	{
    		s_l0147b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MACEDONIAN_l0147B03, &file_info);
        if (is_ret) 
    	{
    		s_l0147b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MACEDONIAN_l0147B04, &file_info);
        if (is_ret) 
    	{
    		s_l0147b04_ptr = file_info.data_ptr;
        }
    }

    /*IM_MALAY_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_MALAY][1]) && (GUIIM_LANG_MALAY == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_MALAY_l0162b00, &file_info);
        if (is_ret) 
    	{
    		s_l0162b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAY_l0162b01, &file_info);
        if (is_ret) 
    	{
    		s_l0162b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAY_l0162b02, &file_info);
        if (is_ret) 
    	{
    		s_l0162b02_ptr = file_info.data_ptr;
        }
    }

    /*IM_MALAYALAM_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_MALAYALAM][1]) && (GUIIM_LANG_MALAYALAM == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b00, &file_info);
        if (is_ret) 
    	{
    		s_l01106b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b01, &file_info);
        if (is_ret) 
    	{
    		s_l01106b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b02, &file_info);
        if (is_ret) 
    	{
    		s_l01106b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b03, &file_info);
        if (is_ret) 
    	{
    		s_l01106b03_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b04, &file_info);
        if (is_ret) 
    	{
    		s_l01106b04_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b05, &file_info);
        if (is_ret) 
    	{
    		s_l01106b06_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b07, &file_info);
        if (is_ret) 
    	{
    		s_l01106b07_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b08, &file_info);
        if (is_ret) 
    	{
    		s_l01106b08_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b09, &file_info);
        if (is_ret) 
    	{
    		s_l01106b09_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b10, &file_info);
        if (is_ret) 
    	{
    		s_l01106b10_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b11, &file_info);
        if (is_ret) 
    	{
    		s_l01106b11_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MALAYALAM_l01106b12, &file_info);
        if (is_ret) 
    	{
    		s_l01106b12_ptr = file_info.data_ptr;
        }
    }

    /*IM_MARATHI_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_MARATHI][1]) && (GUIIM_LANG_MARATHI == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_MARATHI_l01109b00, &file_info);
        if (is_ret) 
    	{
    		s_l01109b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MARATHI_l01109b01, &file_info);
        if (is_ret) 
    	{
    		s_l01109b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MARATHI_l01109b02, &file_info);
        if (is_ret) 
    	{
    		s_l01109b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MARATHI_l01109b03, &file_info);
        if (is_ret) 
    	{
    		s_l01109b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MARATHI_l01109b04, &file_info);
        if (is_ret) 
    	{
    		s_l01109b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MARATHI_l01109b05, &file_info);
        if (is_ret) 
    	{
    		s_l01109b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_MARATHI_l01109b06, &file_info);
        if (is_ret) 
    	{
    		s_l01109b06_ptr = file_info.data_ptr;
        }
    }

    /*IM_NORWEGIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_NORWEGIAN][1]) && (GUIIM_LANG_NORWEGIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_NORWEGIAN_l0120b00, &file_info);
        if (is_ret) 
    	{
    		s_l0120b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_NORWEGIAN_l0120b01, &file_info);
        if (is_ret) 
    	{
    		s_l0120b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_NORWEGIAN_l0120b02, &file_info);
        if (is_ret) 
    	{
    		s_l0120b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_NORWEGIAN_l0120b03, &file_info);
        if (is_ret) 
    	{
    		s_l0120b03_ptr = file_info.data_ptr;
        }
    }

    /*IM_ORIYA_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_ORIYA][1]) && (GUIIM_LANG_ORIYA == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_ORIYA_l01115b00, &file_info);
        if (is_ret) 
    	{
    		s_l01115b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ORIYA_l01115b01, &file_info);
        if (is_ret) 
    	{
    		s_l01115b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ORIYA_l01115b02, &file_info);
        if (is_ret) 
    	{
    		s_l01115b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ORIYA_l01115b03, &file_info);
        if (is_ret) 
    	{
    		s_l01115b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ORIYA_l01115b04, &file_info);
        if (is_ret) 
    	{
    		s_l01115b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ORIYA_l01115b05, &file_info);
        if (is_ret) 
    	{
    		s_l01115b05_ptr = file_info.data_ptr;
        }
    }

    /*IM_PERSIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_PERSIAN][1]) && (GUIIM_LANG_PERSIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_PERSIAN_l0141b00, &file_info);
        if (is_ret) 
    	{
    		s_l0141b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PERSIAN_l0141b01, &file_info);
        if (is_ret) 
    	{
    		s_l0141b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PERSIAN_l0141b02, &file_info);
        if (is_ret) 
    	{
    		s_l0141b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PERSIAN_l0141b03, &file_info);
        if (is_ret) 
    	{
    		s_l0141b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PERSIAN_l0141b04, &file_info);
        if (is_ret) 
    	{
    		s_l0141b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PERSIAN_l0141b05, &file_info);
        if (is_ret) 
    	{
    		s_l0141b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PERSIAN_l0141b06, &file_info);
        if (is_ret) 
    	{
    		s_l0141b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PERSIAN_l0141b07, &file_info);
        if (is_ret) 
    	{
    		s_l0141b07_ptr = file_info.data_ptr;
        }
    }

    /*IM_POLISH_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_POLISH][1]) && (GUIIM_LANG_POLISH == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_POLISH_l0121b00, &file_info);
        if (is_ret) 
    	{
    		s_l0121b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_POLISH_l0121b01, &file_info);
        if (is_ret) 
    	{
    		s_l0121b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_POLISH_l0121b02, &file_info);
        if (is_ret) 
    	{
    		s_l0121b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_POLISH_l0121b03, &file_info);
        if (is_ret) 
    	{
    		s_l0121b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_POLISH_l0121b04, &file_info);
        if (is_ret) 
    	{
    		s_l0121b04_ptr = file_info.data_ptr;
        }
    }

    /*IM_PORTUGUESE_SUPPORT*/
    if(((s_lang_status_info[MMISET_LANGUAGE_PORTUGUESE][1]) && (GUIIM_LANG_PORTUGUESE == input_lang)) ||
			((s_lang_status_info[MMISET_LANGUAGE_PORTUGUESE_BR][1]) && (GUIIM_LANG_PORTUGUESE_BR == input_lang)))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_PORTUGUESE_l0122b00, &file_info);
        if (is_ret) 
    	{
    		s_l0122b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PORTUGUESE_l0122b01, &file_info);
        if (is_ret) 
    	{
    		s_l0122b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PORTUGUESE_l0122b02, &file_info);
        if (is_ret) 
    	{
    		s_l0122b02_ptr = file_info.data_ptr;
        }
    }
    /*IM_PUNJABI_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_PUNJABI][1]) && (GUIIM_LANG_PUNJABI == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_PUNJABI_l01116b00, &file_info);
        if (is_ret) 
    	{
    		s_l01116b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PUNJABI_l01116b01, &file_info);
        if (is_ret) 
    	{
    		s_l01116b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PUNJABI_l01116b02, &file_info);
        if (is_ret) 
    	{
    		s_l01116b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_PUNJABI_l01116b03, &file_info);
        if (is_ret) 
    	{
    		s_l01116b03_ptr = file_info.data_ptr;
        }
    }
    /*IM_ROMANIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_ROMANIAN][1]) && (GUIIM_LANG_ROMANIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_ROMANIAN_l0124b00, &file_info);
        if (is_ret) 
    	{
    		s_l0124b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ROMANIAN_l0124b01, &file_info);
        if (is_ret) 
    	{
    		s_l0124b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ROMANIAN_l0124b02, &file_info);
        if (is_ret) 
    	{
    		s_l0124b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ROMANIAN_l0124b03, &file_info);
        if (is_ret) 
    	{
    		s_l0124b03_ptr = file_info.data_ptr;
        }
    }

    /*IM_RUSSIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_RUSSIAN][1]) && (GUIIM_LANG_RUSSIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_RUSSIAN_l0125b00, &file_info);
        if (is_ret) 
    	{
    		s_l0125b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_RUSSIAN_l0125b01, &file_info);
        if (is_ret) 
    	{
    		s_l0125b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_RUSSIAN_l0125b02, &file_info);
        if (is_ret) 
    	{
    		s_l0125b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_RUSSIAN_l0125b03, &file_info);
        if (is_ret) 
    	{
    		s_l0125b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_RUSSIAN_l0125b04, &file_info);
        if (is_ret) 
    	{
    		s_l0125b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_RUSSIAN_l0125b05, &file_info);
        if (is_ret) 
    	{
    		s_l0125b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_RUSSIAN_l0125b06, &file_info);
        if (is_ret) 
    	{
    		s_l0125b06_ptr = file_info.data_ptr;
        }
    }

    /*IM_SERBIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_SERBIAN][1]) && (GUIIM_LANG_SERBIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_SERBIAN_l01128b00, &file_info);
        if (is_ret) 
    	{
    		s_l01128b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SERBIAN_l01128b01, &file_info);
        if (is_ret) 
    	{
    		s_l01128b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SERBIAN_l01128b02, &file_info);
        if (is_ret) 
    	{
    		s_l01128b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SERBIAN_l01128b03, &file_info);
        if (is_ret) 
    	{
    		s_l01128b03_ptr = file_info.data_ptr;
        }
    }

    /*IM_SESOTHO_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_SESOTHO][1]) && (GUIIM_LANG_SESOTHO == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_SESOTHO_l01130b00, &file_info);
        if (is_ret) 
    	{
    		s_l01130b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SESOTHO_l01130b01, &file_info);
        if (is_ret) 
    	{
    		s_l01130b01_ptr = file_info.data_ptr;
        }
    }
    /*IM_SINHALESE_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_SINHALESE][1]) && (GUIIM_LANG_SINHALESE == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b00, &file_info);
        if (is_ret) 
    	{
    		s_l01124b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b01, &file_info);
        if (is_ret) 
    	{
    		s_l01124b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b02, &file_info);
        if (is_ret) 
    	{
    		s_l01124b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b03, &file_info);
        if (is_ret) 
    	{
    		s_l01124b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b04, &file_info);
        if (is_ret) 
    	{
    		s_l01124b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b05, &file_info);
        if (is_ret) 
    	{
    		s_l01124b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b06, &file_info);
        if (is_ret) 
    	{
    		s_l01124b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b07, &file_info);
        if (is_ret) 
    	{
    		s_l01124b07_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b08, &file_info);
        if (is_ret) 
    	{
    		s_l01124b08_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b09, &file_info);
        if (is_ret) 
    	{
    		s_l01124b09_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b10, &file_info);
        if (is_ret) 
    	{
    		s_l01124b10_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b11, &file_info);
        if (is_ret) 
    	{
    		s_l01124b11_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b12, &file_info);
        if (is_ret) 
    	{
    		s_l01124b12_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b13, &file_info);
        if (is_ret) 
    	{
    		s_l01124b13_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b14, &file_info);
        if (is_ret) 
    	{
    		s_l01124b14_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SINHALESE_l01124b15, &file_info);
        if (is_ret) 
    	{
    		s_l01124b15_ptr = file_info.data_ptr;
        }
    }

    /*IM_SLOVAK_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_SLOVAK][1]) && (GUIIM_LANG_SLOVAK == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_SLOVAK_l0127b00, &file_info);
        if (is_ret) 
    	{
    		s_l0127b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SLOVAK_l0127b01, &file_info);
        if (is_ret) 
    	{
    		s_l0127b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SLOVAK_l0127b02, &file_info);
        if (is_ret) 
    	{
    		s_l0127b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SLOVAK_l0127b03, &file_info);
        if (is_ret) 
    	{
    		s_l0127b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SLOVAK_l0127b04, &file_info);
        if (is_ret) 
    	{
    		s_l0127b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SLOVAK_l0127b05, &file_info);
        if (is_ret) 
    	{
    		s_l0127b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SLOVAK_l0127b06, &file_info);
        if (is_ret) 
    	{
    		s_l0127b06_ptr = file_info.data_ptr;
        }
    }

    /*IM_SLOVENIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_SLOVENIAN][1]) && (GUIIM_LANG_SLOVENIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_SLOVENIAN_l0136b00, &file_info);
        if (is_ret) 
    	{
    		s_l0136b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SLOVENIAN_l0136b01, &file_info);
        if (is_ret) 
    	{
    		s_l0136b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SLOVENIAN_l0136b02, &file_info);
        if (is_ret) 
    	{
    		s_l0136b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SLOVENIAN_l0136b03, &file_info);
        if (is_ret) 
    	{
    		s_l0136b03_ptr = file_info.data_ptr;
        }
    }

    /*IM_SPANISH_SUPPORT*/
    if(((s_lang_status_info[MMISET_LANGUAGE_SPANISH][1]) && (GUIIM_LANG_SPANISH == input_lang)) ||
			((s_lang_status_info[MMISET_LANGUAGE_SPANISH_AM][1]) && (GUIIM_LANG_SPANISH_AM == input_lang)))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_SPANISH_l0110b00, &file_info);
        if (is_ret) 
    	{
    		s_l0110b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SPANISH_l0110b01, &file_info);
        if (is_ret) 
    	{
    		s_l0110b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SPANISH_l0110b02, &file_info);
        if (is_ret) 
    	{
    		s_l0110b02_ptr = file_info.data_ptr;
        }
    }

    /*IM_SWAHILI_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_SWAHILI][1]) && (GUIIM_LANG_SWAHILI == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_SWAHILI_l0165b00, &file_info);
        if (is_ret) 
    	{
    		s_l0165b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SWAHILI_l0165b01, &file_info);
        if (is_ret) 
    	{
    		s_l0165b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SWAHILI_l0165b02, &file_info);
        if (is_ret) 
    	{
    		s_l0165b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SWAHILI_l0165b03, &file_info);
        if (is_ret) 
    	{
    		s_l0165b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SWAHILI_l0165b04, &file_info);
        if (is_ret) 
    	{
    		s_l0165b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SWAHILI_l0165b05, &file_info);
        if (is_ret) 
    	{
    		s_l0165b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SWAHILI_l0165b06, &file_info);
        if (is_ret) 
    	{
    		s_l0165b06_ptr = file_info.data_ptr;
        }
    }

    /*IM_SWEDISH_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_SWEDISH][1]) && (GUIIM_LANG_SWEDISH == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_SWEDISH_l0129b00, &file_info);
        if (is_ret) 
    	{
    		s_l0129b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SWEDISH_l0129b01, &file_info);
        if (is_ret) 
    	{
    		s_l0129b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SWEDISH_l0129b02, &file_info);
        if (is_ret) 
    	{
    		s_l0129b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SWEDISH_l0129b03, &file_info);
        if (is_ret) 
    	{
    		s_l0129b03_ptr = file_info.data_ptr;
        }
    }

    /*IM_TAGALOG_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_TAGALOG][1]) && (GUIIM_LANG_TAGALOG == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_TAGALOG_l01137b00, &file_info);
        if (is_ret) 
    	{
    		s_l01137b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAGALOG_l01137b01, &file_info);
        if (is_ret) 
    	{
    		s_l01137b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAGALOG_l01137b02, &file_info);
        if (is_ret) 
    	{
    		s_l01137b02_ptr = file_info.data_ptr;
        }
    }

    /*IM_TAMIL_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_TAMIL][1]) && (GUIIM_LANG_TAMIL == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b00, &file_info);
        if (is_ret) 
    	{
    		s_l01132b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b01, &file_info);
        if (is_ret) 
    	{
    		s_l01132b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b02, &file_info);
        if (is_ret) 
    	{
    		s_l01132b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b03, &file_info);
        if (is_ret) 
    	{
    		s_l01132b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b04, &file_info);
        if (is_ret) 
    	{
    		s_l01132b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b05, &file_info);
        if (is_ret) 
    	{
    		s_l01132b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b06, &file_info);
        if (is_ret) 
    	{
    		s_l01132b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b07, &file_info);
        if (is_ret) 
    	{
    		s_l01132b07_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b08, &file_info);
        if (is_ret) 
    	{
    		s_l01132b08_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b09, &file_info);
        if (is_ret) 
    	{
    		s_l01132b09_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b10, &file_info);
        if (is_ret) 
    	{
    		s_l01132b10_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b11, &file_info);
        if (is_ret) 
    	{
    		s_l01132b11_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TAMIL_l01132b12, &file_info);
        if (is_ret) 
    	{
    		s_l01132b12_ptr = file_info.data_ptr;
        }
    }

    /*IM_TELUGU_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_TELUGU][1]) && (GUIIM_LANG_TELUGU == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b00, &file_info);
        if (is_ret) 
    	{
    		s_l01133b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b01, &file_info);
        if (is_ret) 
    	{
    		s_l01133b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b02, &file_info);
        if (is_ret) 
    	{
    		s_l01133b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b03, &file_info);
        if (is_ret) 
    	{
    		s_l01133b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b04, &file_info);
        if (is_ret) 
    	{
    		s_l01133b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b05, &file_info);
        if (is_ret) 
    	{
    		s_l01133b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b06, &file_info);
        if (is_ret) 
    	{
    		s_l01133b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b07, &file_info);
        if (is_ret) 
    	{
    		s_l01133b07_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b08, &file_info);
        if (is_ret) 
    	{
    		s_l01133b08_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b09, &file_info);
        if (is_ret) 
    	{
    		s_l01133b09_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b10, &file_info);
        if (is_ret) 
    	{
    		s_l01133b10_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TELUGU_l01133b11, &file_info);
        if (is_ret) 
    	{
    		s_l01133b11_ptr = file_info.data_ptr;
        }
    }
    /*IM_THAI_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_THAI][1]) && (GUIIM_LANG_THAI == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_THAI_l0130b00, &file_info);
        if (is_ret) 
    	{
    		s_l0130b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_THAI_l0130b01, &file_info);
        if (is_ret) 
    	{
    		s_l0130b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_THAI_l0130b02, &file_info);
        if (is_ret) 
    	{
    		s_l0130b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_THAI_l0130b03, &file_info);
        if (is_ret) 
    	{
    		s_l0130b03_ptr = file_info.data_ptr;
        }
    }

    /*IM_TURKISH_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_TURKISH][1]) && (GUIIM_LANG_TURKISH == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_TURKISH_l0131b00, &file_info);
        if (is_ret) 
    	{
    		s_l0131b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TURKISH_l0131b01, &file_info);
        if (is_ret) 
    	{
    		s_l0131b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TURKISH_l0131b02, &file_info);
        if (is_ret) 
    	{
    		s_l0131b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TURKISH_l0131b03, &file_info);
        if (is_ret) 
    	{
    		s_l0131b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TURKISH_l0131b04, &file_info);
        if (is_ret) 
    	{
    		s_l0131b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TURKISH_l0131b05, &file_info);
        if (is_ret) 
    	{
    		s_l0131b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TURKISH_l0131b06, &file_info);
        if (is_ret) 
    	{
    		s_l0131b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TURKISH_l0131b07, &file_info);
        if (is_ret) 
    	{
    		s_l0131b07_ptr = file_info.data_ptr;
        }
    }

    /*IM_UKRAINIAN_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_UKRAINIAN][1]) && (GUIIM_LANG_UKRAINIAN == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_UKRAINIAN_l0134b00, &file_info);
        if (is_ret) 
    	{
    		s_l0134b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_UKRAINIAN_l0134b01, &file_info);
        if (is_ret) 
    	{
    		s_l0134b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_UKRAINIAN_l0134b02, &file_info);
        if (is_ret) 
    	{
    		s_l0134b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_UKRAINIAN_l0134b03, &file_info);
        if (is_ret) 
    	{
    		s_l0134b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_UKRAINIAN_l0134b04, &file_info);
        if (is_ret) 
    	{
    		s_l0134b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_UKRAINIAN_l0134b05, &file_info);
        if (is_ret) 
    	{
    		s_l0134b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_UKRAINIAN_l0134b06, &file_info);
        if (is_ret) 
    	{
    		s_l0134b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_UKRAINIAN_l0134b07, &file_info);
        if (is_ret) 
    	{
    		s_l0134b07_ptr = file_info.data_ptr;
        }
    }

    /*IM_URDU_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_URDU][1]) && (GUIIM_LANG_URDU == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_URDU_l0132b00, &file_info);
        if (is_ret) 
    	{
    		s_l0132b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_URDU_l0132b01, &file_info);
        if (is_ret) 
    	{
    		s_l0132b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_URDU_l0132b02, &file_info);
        if (is_ret) 
    	{
    		s_l0132b02_ptr = file_info.data_ptr;
        }
    }
    /*IM_VIETNAMESE_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_VIETNAMESE][1]) && (GUIIM_LANG_VIETNAMESE == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_VIETNAMESE_l0142b00, &file_info);
        if (is_ret) 
    	{
    		s_l0142b00_ptr = file_info.data_ptr;
        }
    }

    /*IM_XHOSA_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_XHOSA][1]) && (GUIIM_LANG_XHOSA == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_XHOSA_l0152b00, &file_info);
        if (is_ret) 
    	{
    		s_l0152b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_XHOSA_l0152b01, &file_info);
        if (is_ret) 
    	{
    		s_l0152b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_XHOSA_l0152b02, &file_info);
        if (is_ret) 
    	{
    		s_l0152b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_XHOSA_l0152b03, &file_info);
        if (is_ret) 
    	{
    		s_l0152b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_XHOSA_l0152b04, &file_info);
        if (is_ret) 
    	{
    		s_l0152b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_XHOSA_l0152b05, &file_info);
        if (is_ret) 
    	{
    		s_l0152b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_XHOSA_l0152b06, &file_info);
        if (is_ret) 
    	{
    		s_l0152b06_ptr = file_info.data_ptr;
        }
    }

    /*IM_YORUBA_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_YORUBA][1]) && (GUIIM_LANG_YORUBA == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_YORUBA_l01147b00, &file_info);
        if (is_ret) 
    	{
    		s_l01147b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_YORUBA_l01147b01, &file_info);
        if (is_ret) 
    	{
    		s_l01147b01_ptr = file_info.data_ptr;
        }
    }

    /*IM_ZULU_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_ZULU][1]) && (GUIIM_LANG_ZULU == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_ZULU_l0153b00, &file_info);
        if (is_ret) 
    	{
    		s_l0153b00_ptr = file_info.data_ptr;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ZULU_l0153b01, &file_info);
        if (is_ret) 
    	{
    		s_l0153b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ZULU_l0153b02, &file_info);
        if (is_ret) 
    	{
    		s_l0153b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ZULU_l0153b03, &file_info);
        if (is_ret) 
    	{
    		s_l0153b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ZULU_l0153b04, &file_info);
        if (is_ret) 
    	{
    		s_l0153b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_ZULU_l0153b05, &file_info);
        if (is_ret) 
    	{
    		s_l0153b05_ptr = file_info.data_ptr;
        }
    }
    
    /*IM_PASHTO_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_PASHTO][1]) && (GUIIM_LANG_PASHTO == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_PASHTO_l01117b00, &file_info);
        if (is_ret) 
        {
            s_l01117b00_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_PASHTO_l01117b01, &file_info);
        if (is_ret) 
        {
            s_l01117b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_PASHTO_l01117b02, &file_info);
        if (is_ret) 
        {
            s_l01117b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_PASHTO_l01117b03, &file_info);
        if (is_ret) 
        {
            s_l01117b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_PASHTO_l01117b04, &file_info);
        if (is_ret) 
        {
            s_l01117b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_PASHTO_l01117b05, &file_info);
        if (is_ret) 
        {
            s_l01117b05_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_PASHTO_l01117b06, &file_info);
        if (is_ret) 
        {
            s_l01117b06_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_PASHTO_l01117b07, &file_info);
        if (is_ret) 
        {
            s_l01117b07_ptr = file_info.data_ptr;
        }
    }

    /*IM_UZBEK_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_UZBEK][1]) && (GUIIM_LANG_UZBEK == input_lang))
    {
        is_ret = MMI_GetDataInfo(T9_LDB_UZBEK_l01143b00, &file_info);
        if (is_ret) 
        {
            s_l01143b00_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_UZBEK_l01143b01, &file_info);
        if (is_ret) 
        {
            s_l01143b01_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_UZBEK_l01143b02, &file_info);
        if (is_ret) 
        {
            s_l01143b02_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_UZBEK_l01143b03, &file_info);
        if (is_ret) 
        {
            s_l01143b03_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_UZBEK_l01143b04, &file_info);
        if (is_ret) 
        {
            s_l01143b04_ptr = file_info.data_ptr;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_UZBEK_l01143b05, &file_info);
        if (is_ret) 
        {
            s_l01143b05_ptr = file_info.data_ptr;
        }
    }
    return (is_ret);
}

/*---------------------------------------------------------------------------
 *
 *  Function: T9ReadLdbData
 *                                 
 *  Synopsis: Callback from T9Core to access a non-Chinese language database
 *                                           
 *     Input: T9FieldInfo *pFieldInfo -- Field info struct that holds database number
 *            T9U32 dwOffset - byte offset from LDB start to read from
 *            T9FARPOINTER *ppbDst - Ptr to a ptr to the data cache.
 *            T9U32 *pdwStart - Ptr to offset of start of data.
 *            T9NUM nSize    - number of bytes desired (but only 1 is required)
 *            T9NUM mCacheIndex - cache number (igored for compiled in LDBs)
 * 
 *    Return: Number of bytes available at *pdwStart
 *                                                                            
 *---------------------------------------------------------------------------*/
T9U32 T9FARCALL T9ReadLdbData(T9FieldInfo *pFieldInfo, T9U32 dwOffset, T9FARPOINTER *ppbDst, T9U32 *pdwStart, T9U32 nSize, T9MINOR mCacheIndex)
{
    nSize = 15360;
    *pdwStart = dwOffset - (dwOffset % 15360);
    
    //Init language ldb date
    //MMIIM_T9_InitLDBInfo();
    
    switch (pFieldInfo->nLdbNum) {
    case ( 1 * 256) +  9 : /* English */
#ifdef WIN32
		    if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0109b00_s_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0109b01_s_ptr;
        else if (dwOffset <  36651) {*ppbDst = (T9FARPOINTER) s_l0109b02_s_ptr; nSize =   5931;}
        else                        nSize = 0; /* Invalid offset into database! */
#else
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0109b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0109b01_ptr;
        else if (dwOffset <  36651) {*ppbDst = (T9FARPOINTER) s_l0109b02_ptr; nSize =   5931;}
        else                        nSize = 0; /* Invalid offset into database! */
#endif
        break;
    /*IM_AFRIKAANS_SUPPORT*/
     case ( 1 * 256) + 54 : /* Afrikaans */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0154b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0154b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0154b02_ptr;
        else if (dwOffset <  59121) {*ppbDst = (T9FARPOINTER) s_l0154b03_ptr; nSize =  13041;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_AMHARIC_SUPPORT*/
     case ( 1 * 256) + 68 : /* Amharic */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0168b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0168b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0168b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0168b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0168b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0168b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0168b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l0168b07_ptr;
        else if (dwOffset < 124940) {*ppbDst = (T9FARPOINTER) s_l0168b08_ptr; nSize =   2060;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_ALBANIAN_SUPPORT*/
     case ( 1 * 256) + 28 : /* ALBANIAN */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0128b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0128b01_ptr;
        else if (dwOffset <  38841) {*ppbDst = (T9FARPOINTER) s_l0128b02_ptr; nSize =   8121;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_ARABIC_SUPPORT*/
     case ( 1 * 256) +  1 : /* Arabic */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0101b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0101b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0101b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0101b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0101b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0101b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0101b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l0101b07_ptr;
        else if (dwOffset < 138240) *ppbDst = (T9FARPOINTER) s_l0101b08_ptr;
        else if (dwOffset < 139968) {*ppbDst = (T9FARPOINTER) s_l0101b09_ptr; nSize =   1728;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_ASSAMESE_SUPPORT*/
    case ( 1 * 256) + 69 : /* Assamese */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0169b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0169b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0169b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0169b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0169b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0169b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0169b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l0169b07_ptr;
        else if (dwOffset < 138240) *ppbDst = (T9FARPOINTER) s_l0169b08_ptr;
        else if (dwOffset < 138954) {*ppbDst = (T9FARPOINTER) s_l0169b09_ptr; nSize =    714;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_AZERBAIJANI_SUPPORT*/
    case ( 1 * 256) + 71 : /* Azerbaijani */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0171b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0171b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0171b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0171b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0171b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0171b05_ptr;
        else if (dwOffset < 101969) {*ppbDst = (T9FARPOINTER) s_l0171b06_ptr; nSize =   9809;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

	/*IM_BOSNIAN_SUPPORT*/
    case ( 1 * 256) +  151 : /* Bosnian */
	         if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01151b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01151b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l01151b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l01151b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l01151b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l01151b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l01151b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l01151b07_ptr;
        else if (dwOffset < 138240) *ppbDst = (T9FARPOINTER) s_l01151b08_ptr;
        else if (dwOffset < 144777) {*ppbDst = (T9FARPOINTER) s_l01151b09_ptr; nSize =   6537;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

        /*IM_BENGALI_SUPPORT*/
    case ( 1 * 256) + 75 : /* Bengali */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0175b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0175b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0175b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0175b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0175b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0175b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0175b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l0175b07_ptr;
        else if (dwOffset < 132264) {*ppbDst = (T9FARPOINTER) s_l0175b08_ptr; nSize =   9384;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
    /*IM_BULGARIAN_SUPPORT*/
    case ( 1 * 256) +  2 : /* Bulgarian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0102b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0102b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0102b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0102b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0102b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0102b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0102b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l0102b07_ptr;
        else if (dwOffset < 125795) {*ppbDst = (T9FARPOINTER) s_l0102b08_ptr; nSize =   2915;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_CROATIAN_SUPPORT*/
    case ( 1 * 256) + 89 : /* Croatian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0189b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0189b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0189b02_ptr;
        else if (dwOffset <  60228) {*ppbDst = (T9FARPOINTER) s_l0189b03_ptr; nSize =  14148;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_CZECH_SUPPORT*/
    case ( 1 * 256) +  5 : /* Czech */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0105b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0105b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0105b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0105b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0105b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0105b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0105b06_ptr;
        else if (dwOffset < 111077) {*ppbDst = (T9FARPOINTER) s_l0105b07_ptr; nSize =   3557;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_DANISH_SUPPORT*/
    case ( 1 * 256) +  6 : /* Danish */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0106b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0106b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0106b02_ptr;
        else if (dwOffset <  54401) {*ppbDst = (T9FARPOINTER) s_l0106b03_ptr; nSize =   8321;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_DUTCH_SUPPORT*/
    case ( 1 * 256) + 19 : /* Dutch */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0119b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0119b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0119b02_ptr;
        else if (dwOffset <  52249) {*ppbDst = (T9FARPOINTER) s_l0119b03_ptr; nSize =   6169;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
        
    /*IM_ESTONIAN_SUPPORT*/
    case ( 1 * 256) + 37 : /* Estonian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0137b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0137b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0137b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0137b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0137b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0137b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0137b06_ptr;
        else if (dwOffset < 108692) {*ppbDst = (T9FARPOINTER) s_l0137b07_ptr; nSize =   1172;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_FINNISH_SUPPORT*/
    case ( 1 * 256) + 11 : /* Finnish */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0111b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0111b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0111b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0111b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0111b04_ptr;
        else if (dwOffset <  85733) {*ppbDst = (T9FARPOINTER) s_l0111b05_ptr; nSize =   8933;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_FRENCH_SUPPORT*/
    case ( 1 * 256) + 12 : /* French */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0112b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0112b01_ptr;
        else if (dwOffset <  41832) {*ppbDst = (T9FARPOINTER) s_l0112b02_ptr; nSize =  11112;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
        
    /*IM_GERMAN_SUPPORT*/
    case ( 1 * 256) +  7 : /* German */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0107b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0107b01_ptr;
        else if (dwOffset <  45973) {*ppbDst = (T9FARPOINTER) s_l0107b02_ptr; nSize =  15253;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_GREEK_SUPPORT*/
    case ( 1 * 256) +  8 : /* Greek */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0108b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0108b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0108b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0108b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0108b04_ptr;
        else if (dwOffset <  86993) {*ppbDst = (T9FARPOINTER) s_l0108b05_ptr; nSize =  10193;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_GUJARATI_SUPPORT*/
    case ( 1 * 256) + 87 : /* Gujarati */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0187b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0187b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0187b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0187b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0187b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0187b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0187b06_ptr;
        else if (dwOffset < 122737) {*ppbDst = (T9FARPOINTER) s_l0187b07_ptr; nSize =  15217;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_HAUSA_SUPPORT*/
    case ( 1 * 256) + 88 : /* Hausa */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0188b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0188b01_ptr;
        else if (dwOffset <  41171) {*ppbDst = (T9FARPOINTER) s_l0188b02_ptr; nSize =  10451;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_HEBREW_SUPPORT*/
    case ( 1 * 256) + 13 : /* Hebrew */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0113b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0113b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0113b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0113b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0113b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0113b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0113b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l0113b07_ptr;
        else if (dwOffset < 125654) {*ppbDst = (T9FARPOINTER) s_l0113b08_ptr; nSize =   2774;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;


    /*IM_HINDI_SUPPORT*/
    case ( 1 * 256) + 57 : /* Hindi */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0157b00_ptr;
        else if (dwOffset <  29272) {*ppbDst = (T9FARPOINTER) s_l0157b01_ptr; nSize =  13912;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_HUNGARIAN_SUPPORT*/
    case ( 1 * 256) + 14 : /* Hungarian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0114b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0114b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0114b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0114b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0114b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0114b05_ptr;
        else if (dwOffset < 107287) {*ppbDst = (T9FARPOINTER) s_l0114b06_ptr; nSize =  15127;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_ICELANDIC_SUPPORT*/
    case ( 1 * 256) + 15 : /* Icelandic */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0115b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0115b01_ptr;
        else if (dwOffset <  43054) {*ppbDst = (T9FARPOINTER) s_l0115b02_ptr; nSize =  12334;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_IGBO_SUPPORT*/
    case ( 1 * 256) + 149 : /* Igbo */
          if (dwOffset <  13492) {*ppbDst = (T9FARPOINTER) s_l01149b00_ptr; nSize =  13492;}
        else                        nSize = 0; /* Invalid offset into database! */
        break; 

    /*IM_INDONESIAN_SUPPORT*/
     case ( 1 * 256) + 33 : /* Indonesian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0133b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0133b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0133b02_ptr;
        else if (dwOffset <  60450) {*ppbDst = (T9FARPOINTER) s_l0133b03_ptr; nSize =  14370;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_ITALIAN_SUPPORT*/
     case ( 1 * 256) + 16 : /* Italian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0116b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0116b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0116b02_ptr;
        else if (dwOffset <  46110) {*ppbDst = (T9FARPOINTER) s_l0116b03_ptr; nSize =     30;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_KANNADA_SUPPORT*/
    case ( 1 * 256) + 99 : /* Kannada */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0199b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0199b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0199b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0199b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0199b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0199b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0199b06_ptr;
        else if (dwOffset < 113848) {*ppbDst = (T9FARPOINTER) s_l0199b07_ptr; nSize =   6328;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_KAZAKH_SUPPORT*/
    case ( 1 * 256) + 97 : /* Kazakh */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0197b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0197b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0197b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0197b03_ptr;
        else if (dwOffset <  67357) {*ppbDst = (T9FARPOINTER) s_l0197b04_ptr; nSize =   5917;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_KHMER_SUPPORT*/
    case ( 1 * 256) + 44 : /* Khmer */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0144b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0144b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0144b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0144b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0144b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0144b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0144b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l0144b07_ptr;
        else if (dwOffset < 127909) {*ppbDst = (T9FARPOINTER) s_l0144b08_ptr; nSize =   5029;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_LATVIAN_SUPPORT*/
    case ( 1 * 256) + 38 : /* Latvian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0138b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0138b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0138b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0138b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0138b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0138b05_ptr;
        else if (dwOffset <  98418) {*ppbDst = (T9FARPOINTER) s_l0138b06_ptr; nSize =   6258;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_LITHUANIAN_SUPPORT*/
    case ( 1 * 256) + 39 : /* Lithuanian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0139b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0139b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0139b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0139b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0139b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0139b05_ptr;
        else if (dwOffset < 106310) {*ppbDst = (T9FARPOINTER) s_l0139b06_ptr; nSize =  14150;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_MACEDONIAN_SUPPORT*/
    case ( 1 * 256) + 47 : /* Macedonian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0147b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0147b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0147b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0147b03_ptr;
        else if (dwOffset <  64463) {*ppbDst = (T9FARPOINTER) s_l0147b04_ptr; nSize =   3023;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_MALAY_SUPPORT*/
    case ( 1 * 256) + 62 : /* Malay */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0162b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0162b01_ptr;
        else if (dwOffset <  42050) {*ppbDst = (T9FARPOINTER) s_l0162b02_ptr; nSize =  11330;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_MALAYALAM_SUPPORT*/
     case ( 1 * 256) + 106 : /* Malayalam */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01106b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01106b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l01106b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l01106b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l01106b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l01106b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l01106b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l01106b07_ptr;
        else if (dwOffset < 138240) *ppbDst = (T9FARPOINTER) s_l01106b08_ptr;
        else if (dwOffset < 153600) *ppbDst = (T9FARPOINTER) s_l01106b09_ptr;
        else if (dwOffset < 168960) *ppbDst = (T9FARPOINTER) s_l01106b10_ptr;
        else if (dwOffset < 184320) *ppbDst = (T9FARPOINTER) s_l01106b11_ptr;
        else if (dwOffset < 186288) {*ppbDst = (T9FARPOINTER) s_l01106b12_ptr; nSize =   1968;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_MARATHI_SUPPORT*/
    case ( 1 * 256) + 109 : /* Marathi */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01109b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01109b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l01109b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l01109b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l01109b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l01109b05_ptr;
        else if (dwOffset <  99797) {*ppbDst = (T9FARPOINTER) s_l01109b06_ptr; nSize =   7637;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_NORWEGIAN_SUPPORT*/
    case ( 1 * 256) + 20 : /* Norwegian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0120b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0120b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0120b02_ptr;
        else if (dwOffset <  52320) {*ppbDst = (T9FARPOINTER) s_l0120b03_ptr; nSize =   6240;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_ORIYA_SUPPORT*/
    case ( 1 * 256) + 115 : /* Oriya */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01115b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01115b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l01115b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l01115b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l01115b04_ptr;
        else if (dwOffset <  82294) {*ppbDst = (T9FARPOINTER) s_l01115b05_ptr; nSize =   5494;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_PERSIAN_SUPPORT*/
    case ( 1 * 256) + 41 : /* Farsi */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0141b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0141b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0141b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0141b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0141b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0141b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0141b06_ptr;
        else if (dwOffset < 114497) {*ppbDst = (T9FARPOINTER) s_l0141b07_ptr; nSize =   6977;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_POLISH_SUPPORT*/
    case ( 1 * 256) + 21 : /* Polish */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0121b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0121b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0121b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0121b03_ptr;
        else if (dwOffset <  76335) {*ppbDst = (T9FARPOINTER) s_l0121b04_ptr; nSize =  14895;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_PORTUGUESE_SUPPORT*/
    case ( 1 * 256) + 22 : /* Portuguese */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0122b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0122b01_ptr;
        else if (dwOffset <  41164) {*ppbDst = (T9FARPOINTER) s_l0122b02_ptr; nSize =  10444;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_PUNJABI_SUPPORT*/
    case ( 1 * 256) + 116 : /* Punjabi */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01116b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01116b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l01116b02_ptr;
        else if (dwOffset <  50090) {*ppbDst = (T9FARPOINTER) s_l01116b03_ptr; nSize =   4010;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_ROMANIAN_SUPPORT*/
    case ( 1 * 256) + 24 : /* Romanian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0124b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0124b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0124b02_ptr;
        else if (dwOffset <  55239) {*ppbDst = (T9FARPOINTER) s_l0124b03_ptr; nSize =   9159;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_RUSSIAN_SUPPORT*/
    case ( 1 * 256) + 25 : /* Russian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0125b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0125b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0125b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0125b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0125b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0125b05_ptr;
        else if (dwOffset <  99384) {*ppbDst = (T9FARPOINTER) s_l0125b06_ptr; nSize =   7224;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_SERBIAN_SUPPORT*/
    case ( 1 * 256) + 128 : /* Serbian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01128b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01128b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l01128b02_ptr;
        else if (dwOffset <  58166) {*ppbDst = (T9FARPOINTER) s_l01128b03_ptr; nSize =  12086;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
    /*IM_SESOTHO_SUPPORT*/
    case ( 1 * 256) + 130 : /* Sesotho */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01130b00_ptr;
        else if (dwOffset <  23742) {*ppbDst = (T9FARPOINTER) s_l01130b01_ptr; nSize =   8382;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_SINHALESE_SUPPORT*/
    case ( 1 * 256) + 124 : /* Sinhala */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01124b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01124b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l01124b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l01124b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l01124b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l01124b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l01124b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l01124b07_ptr;
        else if (dwOffset < 138240) *ppbDst = (T9FARPOINTER) s_l01124b08_ptr;
        else if (dwOffset < 153600) *ppbDst = (T9FARPOINTER) s_l01124b09_ptr;
        else if (dwOffset < 168960) *ppbDst = (T9FARPOINTER) s_l01124b10_ptr;
        else if (dwOffset < 184320) *ppbDst = (T9FARPOINTER) s_l01124b11_ptr;
        else if (dwOffset < 199680) *ppbDst = (T9FARPOINTER) s_l01124b12_ptr;
        else if (dwOffset < 215040) *ppbDst = (T9FARPOINTER) s_l01124b13_ptr;
        else if (dwOffset < 230400) *ppbDst = (T9FARPOINTER) s_l01124b14_ptr;
        else if (dwOffset < 245512) {*ppbDst = (T9FARPOINTER) s_l01124b15_ptr; nSize =  15112;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_SLOVAK_SUPPORT*/
    case ( 1 * 256) + 27 : /* Slovak */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0127b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0127b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0127b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0127b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0127b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0127b05_ptr;
        else if (dwOffset <  92865) {*ppbDst = (T9FARPOINTER) s_l0127b06_ptr; nSize =    705;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_SLOVENIAN_SUPPORT*/
    case ( 1 * 256) + 36 : /* Slovenian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0136b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0136b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0136b02_ptr;
        else if (dwOffset <  60051) {*ppbDst = (T9FARPOINTER) s_l0136b03_ptr; nSize =  13971;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_SPANISH_SUPPORT*/
    case ( 1 * 256) + 10 : /* Spanish */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0110b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0110b01_ptr;
        else if (dwOffset <  44410) {*ppbDst = (T9FARPOINTER) s_l0110b02_ptr; nSize =  13690;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_SWAHILI_SUPPORT*/
    case ( 1 * 256) + 65 : /* Swahili */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0165b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0165b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0165b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0165b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0165b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0165b05_ptr;
        else if (dwOffset <  96725) {*ppbDst = (T9FARPOINTER) s_l0165b06_ptr; nSize =   4565;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_SWEDISH_SUPPORT*/
    case ( 1 * 256) + 29 : /* Swedish */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0129b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0129b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0129b02_ptr;
        else if (dwOffset <  53232) {*ppbDst = (T9FARPOINTER) s_l0129b03_ptr; nSize =   7152;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
    /*IM_TAGALOG_SUPPORT*/
    case ( 1 * 256) + 137 : /* Tagalog */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01137b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01137b01_ptr;
        else if (dwOffset <  40095) {*ppbDst = (T9FARPOINTER) s_l01137b02_ptr; nSize =   9375;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
    /*IM_TAMIL_SUPPORT*/
    case ( 1 * 256) + 132 : /* Tamil */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01132b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01132b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l01132b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l01132b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l01132b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l01132b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l01132b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l01132b07_ptr;
        else if (dwOffset < 138240) *ppbDst = (T9FARPOINTER) s_l01132b08_ptr;
        else if (dwOffset < 153600) *ppbDst = (T9FARPOINTER) s_l01132b09_ptr;
        else if (dwOffset < 168960) *ppbDst = (T9FARPOINTER) s_l01132b10_ptr;
        else if (dwOffset < 184320) *ppbDst = (T9FARPOINTER) s_l01132b11_ptr;
        else if (dwOffset < 189330) {*ppbDst = (T9FARPOINTER) s_l01132b12_ptr; nSize =   5010;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
    /*IM_TELUGU_SUPPORT*/
    case ( 1 * 256) + 133 : /* Telugu */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01133b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01133b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l01133b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l01133b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l01133b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l01133b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l01133b06_ptr;
        else if (dwOffset < 122880) *ppbDst = (T9FARPOINTER) s_l01133b07_ptr;
        else if (dwOffset < 138240) *ppbDst = (T9FARPOINTER) s_l01133b08_ptr;
        else if (dwOffset < 153600) *ppbDst = (T9FARPOINTER) s_l01133b09_ptr;
        else if (dwOffset < 168960) *ppbDst = (T9FARPOINTER) s_l01133b10_ptr;
        else if (dwOffset < 183365) {*ppbDst = (T9FARPOINTER) s_l01133b11_ptr; nSize =  14405;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
    /*IM_THAI_SUPPORT*/
    case ( 1 * 256) + 30 : /* Thai */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0130b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0130b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0130b02_ptr;
        else if (dwOffset <  52218) {*ppbDst = (T9FARPOINTER) s_l0130b03_ptr; nSize =   6138;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_TURKISH_SUPPORT*/
    case ( 1 * 256) + 31 : /* Turkish */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0131b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0131b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0131b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0131b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0131b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0131b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0131b06_ptr;
        else if (dwOffset < 116088) {*ppbDst = (T9FARPOINTER) s_l0131b07_ptr; nSize =   8568;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
    /*IM_UKRAINIAN_SUPPORT*/
    case ( 1 * 256) + 34 : /* Ukrainian */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0134b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0134b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0134b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0134b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0134b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0134b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l0134b06_ptr;
        else if (dwOffset < 111599) {*ppbDst = (T9FARPOINTER) s_l0134b07_ptr; nSize =   4079;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
    /*IM_URDU_SUPPORT*/
    case ( 1 * 256) + 32 : /* Urdu */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0132b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0132b01_ptr;
        else if (dwOffset <  38348) {*ppbDst = (T9FARPOINTER) s_l0132b02_ptr; nSize =   7628;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_VIETNAMESE_SUPPORT*/
    case ( 1 * 256) + 42 : /* Vietnamese */
          if (dwOffset <  10464) {*ppbDst = (T9FARPOINTER) s_l0142b00_ptr; nSize =  10464;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_XHOSA_SUPPORT*/
    case ( 1 * 256) + 52 : /* Xhosa */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0152b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0152b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0152b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0152b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0152b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l0152b05_ptr;
        else if (dwOffset < 103703) {*ppbDst = (T9FARPOINTER) s_l0152b06_ptr; nSize =  11543;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

     /*IM_YORUBA_SUPPORT*/
     case ( 1 * 256) + 147 : /* Yoruba */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01147b00_ptr;
        else if (dwOffset <  21795) {*ppbDst = (T9FARPOINTER) s_l01147b01_ptr; nSize =   6435;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_ZULU_SUPPORT*/
    case ( 1 * 256) + 53 : /* Zulu */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l0153b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l0153b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l0153b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l0153b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l0153b04_ptr;
        else if (dwOffset <  82895) {*ppbDst = (T9FARPOINTER) s_l0153b05_ptr; nSize =   6095;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
    /*IM_PASHTO_SUPPORT*/
    case ( 1 * 256) + 117 : /* Pashto */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01117b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01117b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l01117b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l01117b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l01117b04_ptr;
        else if (dwOffset <  92160) *ppbDst = (T9FARPOINTER) s_l01117b05_ptr;
        else if (dwOffset < 107520) *ppbDst = (T9FARPOINTER) s_l01117b06_ptr;
        else if (dwOffset < 108258) {*ppbDst = (T9FARPOINTER) s_l01117b07_ptr; nSize =    738;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;

    /*IM_UZBEK_SUPPORT*/
    case ( 1 * 256) + 143 : /* Uzbek */
             if (dwOffset <  15360) *ppbDst = (T9FARPOINTER) s_l01143b00_ptr;
        else if (dwOffset <  30720) *ppbDst = (T9FARPOINTER) s_l01143b01_ptr;
        else if (dwOffset <  46080) *ppbDst = (T9FARPOINTER) s_l01143b02_ptr;
        else if (dwOffset <  61440) *ppbDst = (T9FARPOINTER) s_l01143b03_ptr;
        else if (dwOffset <  76800) *ppbDst = (T9FARPOINTER) s_l01143b04_ptr;
        else if (dwOffset <  87430) {*ppbDst = (T9FARPOINTER) s_l01143b05_ptr; nSize =  10630;}
        else                        nSize = 0; /* Invalid offset into database! */
        break;
   default :
       nSize = 0;
       /* The following assert is a place holder.  If we reach here, the T9FieldInfo does not have a valid ldb number.
          Please change the code to handle the error if necessary. */
       /*assert(nSize != 0);*/
   }

    return nSize;
}
/*****************************************************************************/
//  Description : init chinese ldb lang data info
//  Global resource dependence : none
//  Author:
//  Note: 读取中文输入法的LDB info
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_T9_InitCHSLDBInfo(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    MMIRES_DATA_INFO_T file_info = {0};
    BOOLEAN is_ret = FALSE;
    GUIIM_LANGUAGE_T input_lang = GUIIM_LANG_NONE;
    uint32 offset = 0;

	/*为便于出现问题后调试,仍保留这些全局变量,待版本稳定后整理*/
	s_l0204b00_ptr = PNULL;
	s_l0204b01_ptr = PNULL;
	s_l0204b02_ptr = PNULL;
	s_l0204b08_ptr = PNULL;
	s_l0204b09_ptr = PNULL;
	s_l0204b10_ptr = PNULL;
	s_l0204b11_ptr = PNULL;
	s_l0204b17_ptr = PNULL;
	s_l0204b18_ptr = PNULL;
	s_l0204b19_ptr = PNULL;
	s_l0204b25_ptr = PNULL;
	s_l0204b26_ptr = PNULL;
	s_l0204b27_ptr = PNULL;
	s_l0204b37_ptr = PNULL;
	s_l0204b38_ptr = PNULL;
	s_l0204b41_ptr = PNULL;
	s_l0204b42_ptr = PNULL;
	s_l0204b43_ptr = PNULL;
	s_l0204b46_ptr = PNULL;
	s_l0204b47_ptr = PNULL;
	s_l0204b48_ptr = PNULL;
	s_l0204b49_ptr = PNULL;
	s_l0204b50_ptr = PNULL;
	s_l0204b51_ptr = PNULL;
	s_l0204b52_ptr = PNULL;
	s_l0204b59_ptr = PNULL;
	s_l0204b60_ptr = PNULL;
	s_l0204b61_ptr = PNULL;
	s_l0204b64_ptr = PNULL;
	/*trad chinese tw*/
	s_l0104b00_ptr = PNULL;
	s_l0104b01_ptr = PNULL;
	s_l0104b02_ptr = PNULL;
	s_l0104b08_ptr = PNULL;
	s_l0104b09_ptr = PNULL;
	s_l0104b10_ptr = PNULL;
	s_l0104b11_ptr = PNULL;
	s_l0104b17_ptr = PNULL;
	s_l0104b18_ptr = PNULL;
	s_l0104b19_ptr = PNULL;
	s_l0104b25_ptr = PNULL;
	s_l0104b26_ptr = PNULL;
	s_l0104b27_ptr = PNULL;
	s_l0104b37_ptr = PNULL;
	s_l0104b38_ptr = PNULL;
	s_l0104b41_ptr = PNULL;
	s_l0104b42_ptr = PNULL;
	s_l0104b43_ptr = PNULL;
	s_l0104b46_ptr = PNULL;
	s_l0104b47_ptr = PNULL;
	s_l0104b48_ptr = PNULL;
	s_l0104b49_ptr = PNULL;
    s_l0104b50_ptr = PNULL;
	s_l0104b59_ptr = PNULL;
	s_l0104b60_ptr = PNULL;
	s_l0104b61_ptr = PNULL;
	s_l0104b64_ptr = PNULL;
    input_lang = data_ptr->init_param.method_param_ptr->im_set.lang;
	SCI_TRACE_LOW("[MMIIM] MMIIM_T9_InitCHSLDBInfo : input_lang=%d.", input_lang);
	
     /*IM_SIMP_CHINESE_SUPPORT*/
    if((s_lang_status_info[MMISET_LANGUAGE_SIMP_CHINESE][1]) && (GUIIM_LANG_CHS == input_lang))
    {
        /*GB2312*/
        is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b00, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0204b00_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b08, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b08_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b09, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b09_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b17, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b17_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b25, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b25_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b37, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b37_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b41, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b41_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b42, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b42_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b43, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b43_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b46, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b46_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b47, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b47_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b48, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b48_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b59, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b59_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b60, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b60_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b64, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b64_ptr = &data_ptr->t9_ldb_buff[offset];
        }
#if 0//13000
        is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b00, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0204b00_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b01, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0204b01_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
    		
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b02, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b02_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b08, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b08_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b09, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b09_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b10, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b10_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b11, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b11_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b17, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b17_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b18, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b18_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b19, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b19_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b25, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b25_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b26, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b26_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b27, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b27_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b37, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b37_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b38, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b38_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b41, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b41_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b42, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b42_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b43, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b43_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b46, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b46_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b47, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b47_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b48, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b48_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b49, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b49_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b50, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b50_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b51, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b51_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b52, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b52_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b59, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b59_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b60, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b60_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b61, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b61_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_SIMP_CHINESE_l0204b64, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
    		s_l0204b64_ptr = &data_ptr->t9_ldb_buff[offset];
        }
#endif
        /* Copy block pointers to 1024 */
#ifdef WIN32
        l0204.pBlockPointers[0] = (T9U8 *)s_l0204b00_ptr;
        l0204.pBlockPointers[1] = (T9U8 *)s_l0204b01_ptr;
        l0204.pBlockPointers[2] = (T9U8 *)s_l0204b02_ptr;
        l0204.pBlockPointers[8] = (T9U8 *)s_l0204b08_ptr;
        l0204.pBlockPointers[9] = (T9U8 *)s_l0204b09_ptr;
        l0204.pBlockPointers[10] = (T9U8 *)s_l0204b10_ptr;
        l0204.pBlockPointers[11] = (T9U8 *)s_l0204b11_ptr;
        l0204.pBlockPointers[17] = (T9U8 *)s_l0204b17_ptr;
        l0204.pBlockPointers[18] = (T9U8 *)s_l0204b18_ptr;
        l0204.pBlockPointers[19] = (T9U8 *)s_l0204b19_ptr;
        l0204.pBlockPointers[25] = (T9U8 *)s_l0204b25_ptr;
        l0204.pBlockPointers[26] = (T9U8 *)s_l0204b26_ptr;
        l0204.pBlockPointers[27] = (T9U8 *)s_l0204b27_ptr;
        l0204.pBlockPointers[37] = (T9U8 *)s_l0204b37_ptr;
        l0204.pBlockPointers[38] = (T9U8 *)s_l0204b38_ptr;
        l0204.pBlockPointers[41] = (T9U8 *)s_l0204b41_ptr;
        l0204.pBlockPointers[42] = (T9U8 *)s_l0204b42_ptr;
        l0204.pBlockPointers[43] = (T9U8 *)s_l0204b43_ptr;
        l0204.pBlockPointers[46] = (T9U8 *)s_l0204b46_ptr;
        l0204.pBlockPointers[47] = (T9U8 *)s_l0204b47_ptr;
        l0204.pBlockPointers[48] = (T9U8 *)s_l0204b48_ptr;
        l0204.pBlockPointers[49] = (T9U8 *)s_l0204b49_ptr;
        l0204.pBlockPointers[50] = (T9U8 *)s_l0204b50_ptr;
        l0204.pBlockPointers[51] = (T9U8 *)s_l0204b51_ptr;
        l0204.pBlockPointers[52] = (T9U8 *)s_l0204b52_ptr;
        l0204.pBlockPointers[59] = (T9U8 *)s_l0204b59_ptr;
        l0204.pBlockPointers[60] = (T9U8 *)s_l0204b60_ptr;
        l0204.pBlockPointers[61] = (T9U8 *)s_l0204b61_ptr;
        l0204.pBlockPointers[64] = (T9U8 *)s_l0204b64_ptr;
#else
        l0204.pBlockPointers[0] = (T9U8 *)s_l0204b00_ptr;
        l0204.pBlockPointers[8] = (T9U8 *)s_l0204b08_ptr;
        l0204.pBlockPointers[9] = (T9U8 *)s_l0204b09_ptr;
        l0204.pBlockPointers[17] = (T9U8 *)s_l0204b17_ptr;
        l0204.pBlockPointers[25] = (T9U8 *)s_l0204b25_ptr;
        l0204.pBlockPointers[37] = (T9U8 *)s_l0204b37_ptr;
        l0204.pBlockPointers[41] = (T9U8 *)s_l0204b41_ptr;
        l0204.pBlockPointers[42] = (T9U8 *)s_l0204b42_ptr;
        l0204.pBlockPointers[43] = (T9U8 *)s_l0204b43_ptr;
        l0204.pBlockPointers[46] = (T9U8 *)s_l0204b46_ptr;
        l0204.pBlockPointers[47] = (T9U8 *)s_l0204b47_ptr;
        l0204.pBlockPointers[48] = (T9U8 *)s_l0204b48_ptr;
        l0204.pBlockPointers[59] = (T9U8 *)s_l0204b59_ptr;
        l0204.pBlockPointers[60] = (T9U8 *)s_l0204b60_ptr;
        l0204.pBlockPointers[64] = (T9U8 *)s_l0204b64_ptr;
#endif
    }
	/*trad chinese tw*/
    else if((s_lang_status_info[MMISET_LANGUAGE_TRAD_CHINESE][1]) && (GUIIM_LANG_CHT == input_lang))
    {
        g_input_lang = GUIIM_LANG_CHT;
		is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b00_TW, &file_info);
        if (is_ret)
		{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b00_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b01_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b01_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b08_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b08_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b09_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b09_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b10_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b10_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b17_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b17_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b18_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b18_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b25_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b25_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b26_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b26_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b27_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b27_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b37_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b37_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b41_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b41_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b42_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b42_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
        is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b43_TW, &file_info);
        if (is_ret) 
        {
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b43_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b46_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b46_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b47_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b47_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b48_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b48_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b49_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b49_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b59_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b59_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b60_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b60_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b64_TW, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b64_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		l0104.pBlockPointers[0] = (T9U8 *)s_l0104b00_ptr;
		l0104.pBlockPointers[1] = (T9U8 *)s_l0104b01_ptr;
		l0104.pBlockPointers[8] = (T9U8 *)s_l0104b08_ptr;
		l0104.pBlockPointers[9] = (T9U8 *)s_l0104b09_ptr;
		l0104.pBlockPointers[10] = (T9U8 *)s_l0104b10_ptr;
		l0104.pBlockPointers[17] = (T9U8 *)s_l0104b17_ptr;
		l0104.pBlockPointers[18] = (T9U8 *)s_l0104b18_ptr;
		l0104.pBlockPointers[25] = (T9U8 *)s_l0104b25_ptr;
		l0104.pBlockPointers[26] = (T9U8 *)s_l0104b26_ptr;
		l0104.pBlockPointers[27] = (T9U8 *)s_l0104b27_ptr;
		l0104.pBlockPointers[37] = (T9U8 *)s_l0104b37_ptr;
		l0104.pBlockPointers[41] = (T9U8 *)s_l0104b41_ptr;
		l0104.pBlockPointers[42] = (T9U8 *)s_l0104b42_ptr;
		l0104.pBlockPointers[43] = (T9U8 *)s_l0104b43_ptr;
		l0104.pBlockPointers[46] = (T9U8 *)s_l0104b46_ptr;
		l0104.pBlockPointers[47] = (T9U8 *)s_l0104b47_ptr;
		l0104.pBlockPointers[48] = (T9U8 *)s_l0104b48_ptr;
		l0104.pBlockPointers[49] = (T9U8 *)s_l0104b49_ptr;
		l0104.pBlockPointers[59] = (T9U8 *)s_l0104b59_ptr;
		l0104.pBlockPointers[60] = (T9U8 *)s_l0104b60_ptr;
		l0104.pBlockPointers[64] = (T9U8 *)s_l0104b64_ptr;
    }
	else if((s_lang_status_info[MMISET_LANGUAGE_TRAD_CHINESE_HK][1]) && (GUIIM_LANG_TRAD_CHINESE_HK == input_lang))
	{
        g_input_lang = GUIIM_LANG_TRAD_CHINESE_HK;
		is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b00_HK, &file_info);
        if (is_ret)
		{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b00_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b01_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b01_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
    	SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b08_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b08_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b09_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b09_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b10_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b10_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b17_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b17_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b18_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b18_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b25_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b25_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b26_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b26_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b27_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b27_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b37_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b37_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b41_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b41_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b42_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b42_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b43_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b43_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b46_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b46_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b47_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b47_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b48_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b48_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b49_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b49_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
        SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b50_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b50_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b59_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b59_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b60_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b60_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		SCI_MEMSET(&file_info,0x00,sizeof(MMIRES_DATA_INFO_T));
    	is_ret = MMI_GetDataInfo(T9_LDB_TRAD_CHINESE_l0104b64_HK, &file_info);
        if (is_ret) 
    	{
            SCI_MEMCPY(&data_ptr->t9_ldb_buff[offset],file_info.data_ptr,file_info.data_len);
            s_l0104b64_ptr = &data_ptr->t9_ldb_buff[offset];
            offset = offset + file_info.data_len;
        }
		l0104_HK.pBlockPointers[0] = (T9U8 *)s_l0104b00_ptr;
		l0104_HK.pBlockPointers[1] = (T9U8 *)s_l0104b01_ptr;
		l0104_HK.pBlockPointers[8] = (T9U8 *)s_l0104b08_ptr;
		l0104_HK.pBlockPointers[9] = (T9U8 *)s_l0104b09_ptr;
		l0104_HK.pBlockPointers[10] = (T9U8 *)s_l0104b10_ptr;
		l0104_HK.pBlockPointers[17] = (T9U8 *)s_l0104b17_ptr;
		l0104_HK.pBlockPointers[18] = (T9U8 *)s_l0104b18_ptr;
		l0104_HK.pBlockPointers[25] = (T9U8 *)s_l0104b25_ptr;
		l0104_HK.pBlockPointers[26] = (T9U8 *)s_l0104b26_ptr;
		l0104_HK.pBlockPointers[27] = (T9U8 *)s_l0104b27_ptr;
		l0104_HK.pBlockPointers[37] = (T9U8 *)s_l0104b37_ptr;
		l0104_HK.pBlockPointers[41] = (T9U8 *)s_l0104b41_ptr;
		l0104_HK.pBlockPointers[42] = (T9U8 *)s_l0104b42_ptr;
		l0104_HK.pBlockPointers[43] = (T9U8 *)s_l0104b43_ptr;
		l0104_HK.pBlockPointers[46] = (T9U8 *)s_l0104b46_ptr;
		l0104_HK.pBlockPointers[47] = (T9U8 *)s_l0104b47_ptr;
		l0104_HK.pBlockPointers[48] = (T9U8 *)s_l0104b48_ptr;
		l0104_HK.pBlockPointers[49] = (T9U8 *)s_l0104b49_ptr;
        l0104_HK.pBlockPointers[50] = (T9U8 *)s_l0104b50_ptr;
		l0104_HK.pBlockPointers[59] = (T9U8 *)s_l0104b59_ptr;
		l0104_HK.pBlockPointers[60] = (T9U8 *)s_l0104b60_ptr;
		l0104_HK.pBlockPointers[64] = (T9U8 *)s_l0104b64_ptr;
    }
	else
	{
		SCI_TRACE_LOW("[MMIIM] MMIIM_T9_InitCHSLDBInfo : error");
        return FALSE;
	}
    return TRUE;
}
/*---------------------------------------------------------------------------
 *
 *  Function: T9CCLoadLdb
 *
 *  Synopsis: Callback from T9Core to load the Chinese language database
 *
 *     Input: pCCFieldInfo   -- T9 Chinese Fieldinfo structure
 *            pT9CCLdbHeader -- Pointer to the buffer that the callback
 *                              function loads the LDB data to.
 *
 *    Return: T9STATNONE if the loading succeeds;
 *            T9STATLOADLDBFAIL if the language ID in the T9 FieldInfo
 *            structure is not valid.
 *
 *---------------------------------------------------------------------------*/

#ifdef T9CHINESE
T9STATUS T9FARCALL T9CCLoadLdb(T9CCFieldInfo *pCCFieldInfo, T9ChineseData T9FARDATA *pT9CCLdbHeader)
{
    T9ChineseData const T9FARDATA *pLdbHeader;
    T9UINT i;
    //MMIIM_T9_InitCHSLDBInfo();
	SCI_TRACE_LOW("[MMIIM] T9CCLoadLdb nLdbNum:%d",pCCFieldInfo->G.nLdbNum);
    /* Copy block pointers */
    switch (pCCFieldInfo->G.nLdbNum) {
        
//#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_CHINESE_S_SUPPORT)
        /* Load Simplified Chinese LDB */
        case (2 * 256 + 4):
            pLdbHeader = &l0204;
            break;
//#endif
//#if defined(IM_CHINESE_T_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)
        /* Load Traditional Chinese LDB */
        case (1 * 256 + 4):
            if(GUIIM_LANG_TRAD_CHINESE_HK == g_input_lang)
            {
                pLdbHeader = &l0104_HK;
            }
            else
            {
                pLdbHeader = &l0104;
            }
            break;
//#endif
        default :
            return T9STATLOADLDBFAIL;
    }

    /* Copy the header part */
    for (i = 0; i < T9CCLdbHeaderSize; i++)
        pT9CCLdbHeader->bHeader[i] = pLdbHeader->bHeader[i];

    /* Copy block pointers */
    for (i = 0; i < T9CCLDBBLOCKCOUNT; i++)
        pT9CCLdbHeader->pBlockPointers[i] = pLdbHeader->pBlockPointers[i];

    return T9STATNONE;
}
/*-----------------------------------< eof >----------------------------------*/
#endif
