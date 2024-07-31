/*****************************************************************************
** File Name:      mmiim_t9_base.c                                           *
** Author:         haiwu.chen                                                *
** Date:           24/04/2012                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2012        haiwu.chen            Create                               *
** 06/2012        haiwu.chen            edit, handle t9 and xt9              *
******************************************************************************/

  
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "sci_types.h"
#include "mmi_default.h"
#include "mmi_text.h"
#include "mmi_textfun.h"
#include "mmiim_t9_base.h"
#include "mmiim_text.h"
#include "guistring.h"
#include "guilcd.h"
#include "mmitheme_im.h"
#include "mmi_text.h"
#ifdef _SNMC_SC6530_BUG_FIX_
#include "mmipb_id.h"
#endif
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIIM_AMHARIC_PUA_SYMS_NUM           (8)
#define MMIIM_AMHARIC_CONSONANT_NUM          (46)
// T9 PUA range
#define MMIIM_AMHARIC_PUA_SYMS_START         (0xEEEB)
#define MMIIM_AMHARIC_PUA_SYMS_END           (0xEEF2)
// Following is amharic consonant code definition
#define MMIIM_AMHARIC_CONSONANT_START        (0x1200)
#define MMIIM_AMHARIC_CONSONANT_END          (0x1358)
#define MMIIM_AMHARIC_SPECIAL_CONSONANT_MYA  (0x1359)
#define MMIIM_AMHARIC_SPECIAL_CONSONANT_FYA  (0x135A)
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
#ifdef IM_ENGINE_T9 
// different style flag used to draw different UI Layout
LOCAL EDITOR_STYLE_INFO g_multi_line_style_info = {0};

LOCAL const MMIIM_IM_T9_KEYID_CONVERT_T s_im_t9_keyid_convert[] = 
{
    { MMIIM_SP_KEY_0, T9KEYAMBIGA },
    { MMIIM_SP_KEY_1, T9KEYAMBIG1 },
    { MMIIM_SP_KEY_2, T9KEYAMBIG2 },
    { MMIIM_SP_KEY_3, T9KEYAMBIG3 },
    { MMIIM_SP_KEY_4, T9KEYAMBIG4 },
    { MMIIM_SP_KEY_5, T9KEYAMBIG5 },
    { MMIIM_SP_KEY_6, T9KEYAMBIG6 },
    { MMIIM_SP_KEY_7, T9KEYAMBIG7 },
    { MMIIM_SP_KEY_8, T9KEYAMBIG8 },
    { MMIIM_SP_KEY_9, T9KEYAMBIG9 },

#if 0
    {MMIIM_SP_KEY_A, 'a'},
    {MMIIM_SP_KEY_B, 'b'},
    {MMIIM_SP_KEY_C, 'c'},
    {MMIIM_SP_KEY_D, 'd'},
    {MMIIM_SP_KEY_E, 'e'},
    {MMIIM_SP_KEY_F, 'f'},
    {MMIIM_SP_KEY_G, 'g'},
    {MMIIM_SP_KEY_H, 'h'},
    {MMIIM_SP_KEY_I, 'i'},
    {MMIIM_SP_KEY_J, 'j'},
    {MMIIM_SP_KEY_K, 'k'},
    {MMIIM_SP_KEY_L, 'l'},
    {MMIIM_SP_KEY_M, 'm'},
    {MMIIM_SP_KEY_N, 'n'},
    {MMIIM_SP_KEY_O, 'o'},
    {MMIIM_SP_KEY_P, 'p'},
    {MMIIM_SP_KEY_Q, 'q'},
    {MMIIM_SP_KEY_R, 'r'},
    {MMIIM_SP_KEY_S, 's'},
    {MMIIM_SP_KEY_T, 't'},
    {MMIIM_SP_KEY_U, 'u'},
    {MMIIM_SP_KEY_V, 'v'},
    {MMIIM_SP_KEY_W, 'w'},
    {MMIIM_SP_KEY_X, 'x'},
    {MMIIM_SP_KEY_Y, 'y'},
    {MMIIM_SP_KEY_Z, 'z'},
    {MMIIM_SP_KEY_CANDIDATE_PAGEUP, T9KEYPREV},
    {MMIIM_SP_KEY_CANDIDATE_PAGEDOWN, T9KEYNEXT},    
#endif

    { MMIIM_SP_KEY_STAR, T9KEYAMBIGB },
    { MMIIM_SP_KEY_HASH, T9KEYNONE },
    { MMIIM_SP_KEY_OK, T9KEYOK },
    //{ MMIIM_SP_KEY_WEB, T9KEYOK },
    { MMIIM_SP_KEY_CANCEL, T9KEYCLEAR },
    //{ MMIIM_SP_KEY_BACKSPACE, T9KEYCLEAR },
    { MMIIM_SP_KEY_LEFT, T9KEYLEFT },
    { MMIIM_SP_KEY_RIGHT, T9KEYRIGHT },
    { MMIIM_SP_KEY_UP, T9KEYPREV },
    { MMIIM_SP_KEY_DOWN, T9KEYNEXT },
};

/***********************************************************************************
* The following is the list of the customized conversion table for several special
* language with phonetic such as Khmer\Vietnam\Sinhala\etc. The grammar rule should 
* be accordance with the keymap definition in the file mmiim_sp_ml9key_data.c
*
* Note:
* 1) The rule might not be correct absolutely because we should only extract the 
*    input syntax from the reference device MS provided and modify accordingly. Yet,
*    it seems there's no better solution available but the ugly approach due to we 
*    can not get the professional support from native speaker specialists.
* 2) Every table MUST be put the end flag -- {0xFFFF, 0x0000} as the final element.
************************************************************************************/
// @Khmer
static MMIIM_T9_CONVERSION_TABLE_DEF_T KhmerConversionSSSTable = {
	{0x17D2, 0xFFFF},
	{0x17BB, 0xFFFF},
	{0x17BC, 0xFFFF},
	{0x17BD, 0xFFFF},
	{0x17CB, 0xFFFF},
	{0x17B7, 0xFFFF},
	{0x17B8, 0xFFFF},
	{0x17B9, 0xFFFF},
	{0x17BA, 0xFFFF},
	{0x17C6, 0xFFFF},
	{0x17CF, 0xFFFF},
	{0x17C9, 0xFFFF},
	{0x17CA, 0xFFFF},
	{0x17D0, 0xFFFF},
	{0x17CD, 0xFFFF},
	{0x17CE, 0xFFFF},
	{0x17CC, 0xFFFF},
	{0x17B6, 0xFFFF},
	{0x17BE, 0xFFFF},
	{0x17BF, 0xFFFF},
	{0x17C0, 0xFFFF},
	{0x17C1, 0xFFFF},
	{0x17C2, 0xFFFF},
	{0x17C3, 0xFFFF},
	{0x17C4, 0xFFFF},
	{0x17C5, 0xFFFF},
	{0x17C7, 0xFFFF},
	{0x17C8, 0xFFFF},
	{0xF873, 0xFFFF},
	{0xF874, 0xFFFF},
	{0xF875, 0xFFFF},
	{0xF876, 0xFFFF},
	{0xF872, 0xFFFF},

	/* END FLAG */
	{0xFFFF, 0x0000}
};

static MMIIM_T9_CONVERSION_TABLE_DEF_T KhmerConversionSFSTable = {
	{0x17D2, 0xFFFF},
	{0x17BB, 0xFFFF},
	{0x17BC, 0xFFFF},
	{0x17BD, 0xFFFF},
	{0x17B6, 0xFFFF},
	{0x17BE, 0xFFFF},
	{0x17BF, 0xFFFF},
	{0x17C0, 0xFFFF},
	{0x17C1, 0xFFFF},
	{0x17C2, 0xFFFF},
	{0x17C3, 0xFFFF},
	{0x17C4, 0xFFFF},
	{0x17C5, 0xFFFF},
	{0x17C7, 0xFFFF},
	{0x17C8, 0xFFFF},
	{0xF873, 0xFFFF},
	{0xF874, 0xFFFF},
	{0xF875, 0xFFFF},
	{0xF876, 0xFFFF},
	{0xF872, 0xFFFF},

	/* END FLAG */
	{0xFFFF, 0x0000}
};

static MMIIM_T9_CONVERSION_TABLE_DEF_T KhmerConversionSFMTable = {
	{0x17D2, 0xFFFF},
	{0x17BB, 0xFFFF},
	{0x17BC, 0xFFFF},
	{0x17BD, 0xFFFF},
	{0x17B6, 0xFFFF},
	{0x17BE, 0xFFFF},
	{0x17BF, 0xFFFF},
	{0x17C0, 0xFFFF},
	{0x17C1, 0xFFFF},
	{0x17C2, 0xFFFF},
	{0x17C3, 0xFFFF},
	{0x17C4, 0xFFFF},
	{0x17C5, 0xFFFF},
	{0xF873, 0xFFFF},
	{0xF874, 0xFFFF},
	{0xF875, 0xFFFF},
	{0xF876, 0xFFFF},
	{0xF872, 0xFFFF},

	/* END FLAG */
	{0xFFFF, 0x0000}
};
// @Khmer -- end

// @Sinhala -- include 3 tables

// consonant
static MMIIM_T9_CONVERSION_TABLE_DEF_T SINHALA_CONVERSION_CONSONANT_TABLE = {
	{0x0D85, 0xFFFF},
	{0x0D86, 0xFFFF},
	{0x0D87, 0xFFFF},
	{0x0D88, 0xFFFF},
	{0x0D89, 0xFFFF},
	{0x0D8A, 0xFFFF},
	{0x0D8B, 0xFFFF},
	{0x0D8C, 0xFFFF},
	{0x0D8D, 0xFFFF},
	{0x0D91, 0xFFFF},
	{0x0D92, 0xFFFF},
	{0x0D93, 0xFFFF},
	{0x0D94, 0xFFFF},
	{0x0D95, 0xFFFF},
	{0x0D96, 0xFFFF},

	/* END FLAG */
	{0xFFFF, 0x0000}
};

// not consonant
static MMIIM_T9_CONVERSION_TABLE_DEF_T SINHALA_CONVERSION_VOWEL_MATRA_NONCONSONANT_TABLE = {
	{0x0DCA, 0xFFFF},
	{0x0DCF, 0xFFFF},
	{0x0DD1, 0xFFFF},
	{0x0DD2, 0xFFFF},
	{0x0DD3, 0xFFFF},
	{0x0DD4, 0xFFFF},
	{0x0DD5, 0xFFFF},
	{0x0DD6, 0xFFFF},
	{0x0DD8, 0xFFFF},
	{0x0DD9, 0xFFFF},
	{0x0DDA, 0xFFFF},
	{0x0DDB, 0xFFFF},
	{0x0DDC, 0xFFFF},
	{0x0DDD, 0xFFFF},
	{0x0DDE, 0xFFFF},
	{0x0DF2, 0xFFFF},
	{0xEED6, 0xFFFF},
	{0xEED7, 0xFFFF},
	{0XEEE9, 0xFFFF},

	/* END FLAG */
	{0xFFFF, 0x0000}
};

// not consonant and not vowel and not matra
static MMIIM_T9_CONVERSION_TABLE_DEF_T SINHALA_CONVERSION_NONALL_TABLE = {
	{0x0D82, 0xFFFF},
	{0x0D83, 0xFFFF},
	{0x0DCA, 0xFFFF},
	{0x0DCF, 0xFFFF},
	{0x0DD1, 0xFFFF},
	{0x0DD2, 0xFFFF},
	{0x0DD3, 0xFFFF},
	{0x0DD4, 0xFFFF},
	{0x0DD5, 0xFFFF},
	{0x0DD6, 0xFFFF},
	{0x0DD8, 0xFFFF},
	{0x0DD9, 0xFFFF},
	{0x0DDA, 0xFFFF},
	{0x0DDB, 0xFFFF},
	{0x0DDC, 0xFFFF},
	{0x0DDD, 0xFFFF},
	{0x0DDE, 0xFFFF},
	{0x0DF2, 0xFFFF},
	{0xEED6, 0xFFFF},
	{0xEED7, 0xFFFF},
	{0XEEE9, 0xFFFF},
	
	/* END FLAG */
	{0xFFFF, 0x0000}
};
// @Sinhala end

// @Amharic start
static MMIIM_T9_CONVERSION_TABLE_DEF_T AMHARIC_CONVERSION_NOVOWEL_TABLE = {
    {0xEEEB, 0xFFFF},
    {0xEEEC, 0xFFFF},
    {0xEEED, 0xFFFF},
    {0xEEEE, 0xFFFF},
    {0xEEEF, 0xFFFF},
    {0xEEF0, 0xFFFF},
    {0xEEF1, 0xFFFF},
    {0xEEF2, 0xFFFF},
    
    /* END FLAG */
	{0xFFFF, 0x0000}
};
// @Amharic end

// @The mapping table between t9 PUA characters and SPRD Combine chars
static MMIIM_T9_CONVERSION_TABLE_DEF_T T9_PUA_COMBINE_MAPPING_TABLE = 
{
	//TELUGU
	/*Kye 1*/{0x0000, 0xE100},/*E100*/
	//Tamil
	/*Kye 9*/{0x0000, 0xE101},///1
	/*Kye 9*/{0x0000, 0xE102},///2
	//Sinhala
	/*Kye 1*/{0xEED6, 0xE103},///3
	/*Kye 1*/{0xEED7, 0xE104},///4
	/*Kye 9*/{0xEED8, 0xE105},///5
	/*Kye 9*/{0xEED9, 0xE106},///6
	//Oriya
	/*Kye 0*/{0x0000, 0xE107},///7
	/*Kye 1*/{0x0000, 0xE108},///8
	/*Kye 1*/{0x0000, 0xE109},///9
	//Myanmar
	/*Kye 1*/{0x0000, 0xE10A},///10
	//Marathi
	/*Kye 1*/{0x0000, 0xE10B},///11
	/*Kye 3*/{0x0000, 0xE10C},///12
	/*Kye 9*/{0x0000, 0xE10D},///13
	/*Kye 9*/{0x0000, 0xE10E},///14
	//Malayalam
	/*Kye 0*/{0x0000, 0xE10F},///15
	/*Kye 6*/{0x0000, 0xE110},///16
	/*Kye 7*/{0x0000, 0xE111},///17
	/*Kye 9*/{0x0000, 0xE112},///18
	/*Kye 9*/{0x0000, 0xE113},///19
	//Malagasy (not support)
	///*Kye 6L*/{},
	///*Kye 6U*/{},
	//Khmer
	/*Kye 1*/{0xF873, 0xE114},///20
	/*Kye 1*/{0xF874, 0xE115},///21
	/*Kye 3*/{0xF872, 0xE116},///22
	/*Kye 3*/{0xF875, 0xE117},///23
	/*Kye 3*/{0xF876, 0xE118},///24
	//Kannada
	/*Kye 1*/{0x0000, 0xE119},///25
	//Hindi
	/*Kye 1*/{0x0000, 0xE11A},///26
	/*Kye 1*/{0x0000, 0xE11B},///27
	//Gujarati
	/*Kye 1*/{0x0000, 0xE11C},///28
	//Bengali-IN (not support)
	///*Kye 1*/{},
	//Bengali-BD (not support)
	///*Kye 1*/{},
	//Assamese
	/*Kye 0*/{0x0000, 0xE11D},///29
	/*Kye 1*/{0x0000, 0xE11E},///30
	/*Kye 1*/{0x0000, 0xE11F},///31
	/*Kye 1*/{0x0000, 0xE120},///32
	/*Kye 1*/{0x0000, 0xE121},///33
	
	//Marathi
	/*Kye 9*/{0x0000, 0xE122},///34

	//Sinhala-1
	/*Kye 1*/{0xEEE9, 0xE123},///35

	/* END FLAG */
	{0xFFFF, 0x0000}
};

static wchar AmharicTransTable[][MMIIM_AMHARIC_PUA_SYMS_NUM + 1] = {
    {0x1200,0x1201,0x1202,0x1203,0x1204,0x1205,0x1206,0x1207},
    {0x1208,0x1209,0x120A,0x120B,0x120C,0x120D,0x120E,0x120F},
    {0x1210,0x1211,0x1212,0x1213,0x1214,0x1215,0x1216,0x1217},
    {0x1218,0x1219,0x121A,0x121B,0x121C,0x121D,0x121E,0x121F},
    {0x1220,0x1221,0x1222,0x1223,0x1224,0x1225,0x1226,0x1227},
    {0x1228,0x1229,0x122A,0x122B,0x122C,0x122D,0x122E,0x122F},
    {0x1230,0x1231,0x1232,0x1233,0x1234,0x1235,0x1236,0x1237},
    {0x1238,0x1239,0x123A,0x123B,0x123C,0x123D,0x123E,0x123F},
    {0x1240,0x1241,0x1242,0x1243,0x1244,0x1245,0x1246,0x1247},
    {0x1248,0x003F,0x124A,0x124B,0x124C,0x124D,0x003F,0x003F},
    {0x1250,0x1251,0x1252,0x1253,0x1254,0x1255,0x1256,0x003F},
    {0x1258,0x003F,0x125A,0x125B,0x125C,0x125D,0x003F,0x003F},
    {0x1260,0x1261,0x1262,0x1263,0x1264,0x1265,0x1266,0x1267},
    {0x1268,0x1269,0x126A,0x126B,0x126C,0x126D,0x126E,0x126F},
    {0x1270,0x1271,0x1272,0x1273,0x1274,0x1275,0x1276,0x1277},
    {0x1278,0x1279,0x127A,0x127B,0x127C,0x127D,0x127E,0x127F},
    {0x1280,0x1281,0x1282,0x1283,0x1284,0x1285,0x1286,0x1287},
    {0x1288,0x003F,0x128A,0x128B,0x128C,0x128D,0x003F,0x003F},
    {0x1290,0x1291,0x1292,0x1293,0x1294,0x1295,0x1296,0x1297},
    {0x1298,0x1299,0x129A,0x129B,0x129C,0x129D,0x129E,0x129F},
    {0x12A0,0x12A1,0x12A2,0x12A3,0x12A4,0x12A5,0x12A6,0x12A7},
    {0x12A8,0x12A9,0x12AA,0x12AB,0x12AC,0x12AD,0x12AE,0x12AF},
    {0x12B0,0x003F,0x12B2,0x12B3,0x12B4,0x12B5,0x003F,0x003F},
    {0x12B8,0x12B9,0x12BA,0x12BB,0x12BC,0x12BD,0x12BE,0x003F},
    {0x12C0,0x003F,0x12C2,0x12C3,0x12C4,0x12C5,0x003F,0x003F},
    {0x12C8,0x12C9,0x12CA,0x12CB,0x12CC,0x12CD,0x12CE,0x12CF},
    {0x12D0,0x12D1,0x12D2,0x12D3,0x12D4,0x12D5,0x12D6,0x003F},
    {0x12D8,0x12D9,0x12DA,0x12DB,0x12DC,0x12DD,0x12DE,0x12DF},
    {0x12E0,0x12E1,0x12E2,0x12E3,0x12E4,0x12E5,0x12E6,0x12E7},
    {0x12E8,0x12E9,0x12EA,0x12EB,0x12EC,0x12ED,0x12EE,0x12EF},
    {0x12F0,0x12F1,0x12F2,0x12F3,0x12F4,0x12F5,0x12F6,0x12F7},
    {0x12F8,0x12F9,0x12FA,0x12FB,0x12FC,0x12FD,0x12FE,0x12FF},
    {0x1300,0x1301,0x1302,0x1303,0x1304,0x1305,0x1306,0x1307},
    {0x1308,0x1309,0x130A,0x130B,0x130C,0x130D,0x130E,0x130F},
    {0x1310,0x003F,0x1312,0x1313,0x1314,0x1315,0x003F,0x003F},
    {0x1318,0x1319,0x131A,0x131B,0x131C,0x131D,0x131E,0x131F},
    {0x1320,0x1321,0x1322,0x1323,0x1324,0x1325,0x1326,0x1327},
    {0x1328,0x1329,0x132A,0x132B,0x132C,0x132D,0x132E,0x132F},
    {0x1330,0x1331,0x1332,0x1333,0x1334,0x1335,0x1336,0x1337},
    {0x1338,0x1339,0x133A,0x133B,0x133C,0x133D,0x133E,0x133F},
    {0x1340,0x1341,0x1342,0x1343,0x1344,0x1345,0x1346,0x1347},
    {0x1348,0x1349,0x134A,0x134B,0x134C,0x134D,0x134E,0x134F},
    {0x1350,0x1351,0x1352,0x1353,0x1354,0x1355,0x1356,0x1357},
    {0x1358},
    {0x1359},
    {0x135A}
};

LOCAL uint8 T9_BPMF_Letter_Order[] = {
        0x62, /* b */    0x70, /* p */    0x6D, /* m */    0x66, /* f */    0x64, /* d */
        0x74, /* t */    0x6E, /* n */    0x6C, /* l */    0x67, /* g */    0x6B, /* k */
        0x68, /* h */    0x6A, /* j */    0x71, /* q */    0x78, /* x */    0xDF, /* zh */
        0xE7, /* ch */   0x9A, /* sh */   0x72, /* r */    0x7A, /* z */    0x63, /* c */
        0x73, /* s */    0x61, /* a */    0x6F, /* o */    0x65, /* e */    0xE9, /* e(C) */
        0xE4, /* ai */   0xF6, /* ei */   0xE5, /* ao */   0xFD, /* ou */   0xE3, /* an */
        0xF5, /* en */   0xE2, /* ang */  0xF4, /* eng */  0xF8, /* er */   0x69, /* i */
        0x75, /* u */    0xFC, /* ?*/     0 };

#endif //#ifdef IM_ENGINE_T9 

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
 #ifdef IM_ENGINE_T9 
 #ifdef T9CHINESE
/******************************************************************************/
// Description: get idc color by other state
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL GUI_COLOR_T GetIDCColor(MMIIM_IDC_STATE_T state);

/******************************************************************************/
// Description: 
// Global resource dependence: NONE
// Author: 
// Note: t9 maybe draw a icon here
/******************************************************************************/
LOCAL void DrawLeftIDC(GUI_LCD_DEV_INFO const *lcd_info_ptr, GUI_RECT_T const *r, GUI_COLOR_T c);

/******************************************************************************/
// Description: 
// Global resource dependence: NONE
// Author: 
// Note: t9 maybe draw a icon here
/******************************************************************************/
LOCAL void DrawRightIDC(GUI_LCD_DEV_INFO const *lcd_info_ptr, GUI_RECT_T const *r, GUI_COLOR_T c);

#endif
#endif //#ifdef IM_ENGINE_T9 

/*****************************************************************************/
//  Description : init flag g_is_multi_line_style
//  Global resource dependence : none
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
LOCAL void MMIIM_InitMultiLineStyle(CAF_HANDLE_T ctrl_handle);

/*****************************************************************************/
//  Description : popup list remove layer
//  Global resource dependence : none
//  Author:cheney.wu
//  Note:
/*****************************************************************************/
LOCAL void PopupListRemoveLayer(GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : popup list append layer
//  Global resource dependence : none
//  Author:cheney.wu
//  Note:
/*****************************************************************************/
LOCAL void PopupListAppendLayer(GUI_LCD_DEV_INFO *lcd_dev_info_ptr);

/*****************************************************************************/
//  Description : popup list append layer for chinese
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcInsertBorderRect(GUI_RECT_T *left_rect, GUI_RECT_T *right_rect);

/*****************************************************************************/
//  Description : check and decode the combine char if needed
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIM_CheckAndDecodeCombineChar(wchar curChar, MMI_STRING_T* keymap_info_ptr);

/*****************************************************************************/
//  Description : check and decode the combine char if needed
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIM_CheckAmharicMappingChar(wchar preChar, wchar curChar, MMI_STRING_T* keymap_info_ptr);

/*****************************************************************************/
//  Description : check and decode the combine char if needed
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
LOCAL wchar const* MMIIM_GetAmharicMappingChar(uint16 colIdx, uint16 vowIdx);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

#ifdef IM_ENGINE_T9 
/*****************************************************************************/
//  Description : MMIIM_ConvertMsgToT9KeyID
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_ConvertMsgToT9KeyID(MMIIM_SP_MSG_T *msg_ptr, T9KEY *eKeyID_ptr)
{
    BOOLEAN is_ret = FALSE;

    if (PNULL == msg_ptr
        || PNULL == eKeyID_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_ConvertMsgToT9KeyID: invalid param.");
        return is_ret;
    }

    if (MMIIM_SP_MSG_KEY_PRESS == msg_ptr->id
         || MMIIM_SP_MSG_KEY_RELEASE == msg_ptr->id
         || MMIIM_SP_MSG_KEY_LONG_PRESS == msg_ptr->id
         || MMIIM_SP_MSG_KEY_LONG_RELEASE == msg_ptr->id
         || MMIIM_SP_MSG_KEY_REPEAT_PRESS == msg_ptr->id)
    {
        is_ret = MMIIM_ConvertSpKeyToT9KeyID(msg_ptr->para.key, eKeyID_ptr);
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : MMIIM_ConvertSpKeyToT9KeyID
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_ConvertSpKeyToT9KeyID(
    MMIIM_SP_VKEY_T msg_id,
    T9KEY *T9KeyID_ptr
)
{
    T9KEY dst_key = T9KEYNONE;
    BOOLEAN is_ret = FALSE;

    uint32 i = 0;
    uint32 table_size = ARR_SIZE(s_im_t9_keyid_convert);
    
    if (PNULL == T9KeyID_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_ConvertSpKeyToT9KeyID: keyid_ptr is null.");
        return is_ret;
    }

    if (MMIIM_SP_KEY_NONE == msg_id)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_ConvertSpKeyToT9KeyID: msg_id is MMIIM_SP_KEY_NONE.");
        return is_ret;
    }

    for (i = 0; i < table_size; i++)
    {
        if (msg_id == s_im_t9_keyid_convert[i].im_vkey_id)
        {
            dst_key = (T9KEY)s_im_t9_keyid_convert[i].t9_key_id;
            is_ret = TRUE;
            break;
        }
    }

    *T9KeyID_ptr = dst_key;
    
    return is_ret;
}
#endif //#ifdef IM_ENGINE_T9 

 
//operation common
/*****************************************************************************/
//  Description : select highlight word
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_EditorSelectHighlightChars(
    GUIIM_INIT_KERNEL_T *init_param_ptr,
    uint16 first_idx,
    uint16 last_idx
)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_SET_HIGHLIGHT_MSG_DATA_T highlight_data = {0};

    if (PNULL == init_param_ptr)
    {
        return;
    }

    highlight_data.start_index = first_idx;
    highlight_data.end_index = last_idx;

    notify_pack.notify = GUIIM_NOTIFY_SET_HIGHLIGHT;
    notify_pack.data.set_highlight_msg = highlight_data;

    GUIIM_CommitData(init_param_ptr->im_handle, &notify_pack);
}

/*****************************************************************************/
//  Description : cursor is at the end
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_IsCursorAtTheEnd(GUIIM_INIT_KERNEL_T *init_param_ptr)
{
    BOOLEAN is_ret = FALSE;
    uint32 cur_pos = 0;
    GUIIM_CONTEXT_T im_context = {0};
    
    if (PNULL == init_param_ptr)
    {
        return cur_pos;
    }

    MMIIM_GetEditContent(init_param_ptr, &im_context);

    if (im_context.str_len == im_context.pos)
    {
        is_ret = TRUE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : get current cursor position
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_GetEditContent(GUIIM_INIT_KERNEL_T *init_param_ptr, GUIIM_CONTEXT_T *con_ptr)
{
    BOOLEAN is_ret = FALSE;
    
    if (PNULL == init_param_ptr || PNULL == con_ptr)
    {
        return is_ret;
    }
    
    if (PNULL != init_param_ptr->f_get_context)
    {
        MMI_HANDLE_T edit_ctrl_handle = MMK_GetParentCtrlHandle(init_param_ptr->im_handle);

        (*(init_param_ptr->f_get_context))(edit_ctrl_handle, con_ptr);
        is_ret = TRUE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : MMIIM_IsContentEmpty
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_IsContentEmpty(GUIIM_INIT_KERNEL_T *init_param_ptr)
{
    BOOLEAN is_ret = TRUE;
    GUIIM_CONTEXT_T cont = {0};
    
    if (PNULL == init_param_ptr)
    {
        return is_ret;
    }

    MMIIM_GetEditContent(init_param_ptr, &cont);

    if (0 != cont.str_len)
    {
        is_ret = FALSE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : MMIIM_NeedSwitchLeadCap
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_NeedSwitchLeadCap(GUIIM_INIT_KERNEL_T *init_param_ptr)
{
    BOOLEAN is_ret = FALSE;
    GUIIM_CONTEXT_T cont = {0};
    
    if (PNULL == init_param_ptr)
    {
        return is_ret;
    }

    if (MMIIM_GetEditContent(init_param_ptr, &cont))
    {
        if (cont.str_len == 0)
        {
            is_ret = TRUE;
        }
        else if (cont.str_len >= 2 && cont.pos >= 2 && cont.str_ptr != PNULL
                && ' ' == cont.str_ptr[cont.pos-1] && '.' == cont.str_ptr[cont.pos-2])
        {
            is_ret = TRUE;
        }
#ifdef _SNMC_SC6530_BUG_FIX_
        else if ((MMK_GetCtrlHandleByWin(init_param_ptr->win_id,MMIPB_EDITBOX_NAME_ID) == MMK_GetParentCtrlHandle(init_param_ptr->im_handle))
            && (cont.str_len >= 1 && cont.pos >= 1 && cont.str_ptr != PNULL
                && ' ' == cont.str_ptr[cont.pos-1]))
        {
            is_ret = TRUE;
        }
#endif
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : get current cursor position
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIIM_GetEditCursor(GUIIM_INIT_KERNEL_T *init_param_ptr)
{
    uint32 cur_pos = 0;
    GUIIM_CONTEXT_T im_context = {0};
    
    if (PNULL == init_param_ptr)
    {
        return cur_pos;
    }

    MMIIM_GetEditContent(init_param_ptr, &im_context);

    if (PNULL != im_context.str_ptr && 0 != im_context.str_len)
    {
        cur_pos = im_context.pos;
    }

    return cur_pos;
}

/*****************************************************************************/
//  Description : move cursor
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_SetEditCursor(
    GUIIM_INIT_KERNEL_T *init_param_ptr,
    GUIIM_SETCURSOR_TYPE_E type,
    int16 value
)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_SETCURSOR_MSG_DATA_T cursor_data = {0};

    if (PNULL == init_param_ptr)
    {
        return;
    }

    cursor_data.type = type;
    cursor_data.value = value;

    notify_pack.notify = GUIIM_NOTIFY_SETCURSOR;
    notify_pack.data.setcursor_msg = cursor_data;

    GUIIM_CommitData(init_param_ptr->im_handle, &notify_pack);
}

/*****************************************************************************/
//  Description : 把高亮的确定
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_ClearImeComp(GUIIM_INIT_KERNEL_T *init_param_ptr)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    if (PNULL == init_param_ptr)
    {
        return;
    }

    notify_pack.notify = GUIIM_NOTIFY_COMMIT_UNDERLINE;

    GUIIM_CommitData(init_param_ptr->im_handle, &notify_pack);
}

/*****************************************************************************/
//  Description : Is shade state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_Editor_IsShadeState(void)
{
    return FALSE;//s_ime_editor.editbox_ptr->is_shade;
}

/*******************************************************************************
 *
 *	Function name	: BPMFInternalToUnicode
 *	Description	    : Convert 8 bit Tegic BoPoMoFo internal char code to Unicode
 *                    (Where unicodes start from 3100, Linear offsets start from 0)
 *
 *	Return     		: wUnicode
 *
 *	Pass            : T9U8 - alpha Tegic BoPoMoFo internal char code to be translated
 *
 ******************************************************************************/
LOCAL uint16 T9BPMFInternalToUnicode(uint8 bChar)
{
    T9U16 nIdx;

    for (nIdx = 0; T9_BPMF_Letter_Order[nIdx]; nIdx++) 
    {
        if (T9_BPMF_Letter_Order[nIdx] == bChar)
        {
            break;
        }
    }

    /* If the character was not found in the table just return it */
    if (T9_BPMF_Letter_Order[nIdx])
    {
        return(0x3105 + nIdx);
    }
    else
    {
        return (uint16)bChar;
    }
}

/*****************************************************************************/
//Description : add replace chars
//Global resource dependence : none
//Author: haiwu.chen
//Note: include commit or comp chars, txt_ptr=commit buf+underline buf(in order)
/*****************************************************************************/
PUBLIC void MMIIM_ReplaceChars(
    GUIIM_INIT_KERNEL_T *init_param_ptr,
    uint16 *txt_ptr,
    uint16 commit_len,
    uint16 underline_len
)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_COMMIT_DATA_T commit_data = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint16 nIdx = 0;
    wchar curChar = 0, preChar = 0;
    MMI_STRING_T keymap_info = {0};
//    GUIIM_LANGUAGE_T input_lang = GUIIM_LANG_NONE;
    
    if (PNULL == init_param_ptr || PNULL == txt_ptr)
    {
        return;
    }

//    input_lang = MMIAPIIM_GetLanguage();
    
    /* should check and decode every char and decode the PUA character if exists */
    for(i = 0, preChar = 0; i < GUIIM_COMMIT_BUFFER_MAX && i < commit_len; i++)
    {
        curChar = *(txt_ptr + i);
        if (MMIIM_CheckAndDecodeCombineChar(curChar, &keymap_info))
        {
            for (j = 0; j < keymap_info.wstr_len; j++)
            {
                commit_data.commit[nIdx++] = *(keymap_info.wstr_ptr + j);
            }
        }
        // Check Amharic modifier Vowel specifically
        else if (MMIIM_CheckAmharicMappingChar(preChar, curChar, &keymap_info))
        {
            // Override (the preChar if exists) or (curChar and plus nIdx one)
            j = ((nIdx > 0) ? (nIdx - 1) : nIdx++);
            commit_data.commit[j] = *(keymap_info.wstr_ptr);
        }
        else
        {
            commit_data.commit[nIdx++] = curChar;
        }
        
        preChar = commit_data.commit[nIdx - 1];
    }
    commit_data.commit_len = nIdx;    
    
    for(nIdx = 0, j = 0, preChar = 0; j < GUIIM_COMMIT_BUFFER_MAX && j < underline_len; j++)
    {
        curChar = *(txt_ptr + commit_len + j);
        if (MMIIM_CheckAndDecodeCombineChar(curChar, &keymap_info))
        {
            for (i = 0; i < keymap_info.wstr_len; i++)
            {
                commit_data.underline[nIdx++] = *(keymap_info.wstr_ptr + i);
            }
        }
        // Check Amharic modifier Vowel specifically
        else if (MMIIM_CheckAmharicMappingChar(preChar, curChar, &keymap_info))
        {
            // Override (the preChar if exists) or (curChar and plus nIdx one)
            i = ((nIdx > 0) ? (nIdx - 1) : nIdx++);
            commit_data.underline[i] = *(keymap_info.wstr_ptr);
        }
        else
        {
            if(GUIIM_LANG_CHT == init_param_ptr->method_param_ptr->im_set.lang)
            {
                commit_data.underline[nIdx++] = T9BPMFInternalToUnicode((uint8)curChar);
            }
            else
            {
                commit_data.underline[nIdx++] = curChar;
            }
        }
        
        preChar = commit_data.underline[nIdx - 1];
    }
    commit_data.underline_len = nIdx;
    
    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    notify_pack.data.commit_data = commit_data;
    
    GUIIM_CommitData(init_param_ptr->im_handle, &notify_pack);
}

/*****************************************************************************/
//  Description : set flag g_is_multi_line_style
//  Global resource dependence : none
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_ResetMultiLineStyle()
{
	g_multi_line_style_info.is_initialized = FALSE;
	g_multi_line_style_info.is_multi_line_style = FALSE;
}

/*****************************************************************************/
//  Description : get the flag g_is_multi_line_style value
//  Global resource dependence : none
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_GetMultiLineStyle(CAF_HANDLE_T ctrl_handle)
{
	BOOLEAN is_multi_line_style = FALSE;

	if (!g_multi_line_style_info.is_initialized)
	{
		MMIIM_InitMultiLineStyle(ctrl_handle);
	}

	is_multi_line_style = g_multi_line_style_info.is_multi_line_style;

	return is_multi_line_style;
}

/*****************************************************************************/
//  Description : init flag g_is_multi_line_style
//  Global resource dependence : none
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
LOCAL void MMIIM_InitMultiLineStyle(CAF_HANDLE_T ctrl_handle)
{
	GUIEDIT_STYLE_E style = GUIEDIT_STYLE_NULL;
    GUICTRL_SendNotifyEx(ctrl_handle, MSG_NOTIFY_IM_GET_EDITOR_STYLE, &style);

	g_multi_line_style_info.is_initialized = TRUE;
	if (GUIEDIT_STYLE_NULL < style && GUIEDIT_STYLE_MAX > style)
	{
		g_multi_line_style_info.is_multi_line_style = ((GUIEDIT_STYLE_SINGLE == style) ? FALSE:TRUE);
	}
	else
	{
		g_multi_line_style_info.is_multi_line_style = FALSE;
	}
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: arvin
//  Note:
/*****************************************************************************/
PUBLIC MMIIM_T9_CONVERSION_TABLE_DEF_T* MMIIM_CheckConversionTable(T9UINT lang, T9SYMB previous_char)
{
	MMIIM_T9_CONVERSION_TABLE_DEF_T *key_table = PNULL;
	SCI_TRACE_LOW("[MMIIM] MMIIM_CheckConversionTable: lang=%d, previous_char=%x.", lang, previous_char);
	switch (lang) {
		case T9PIDKhmer:
			if (previous_char)
			{
				//BC [1780,17A2], BV [17A3,17B3], CS  [17C9,17CA]
				if ((previous_char >= 0x1780 && previous_char <= 0x17B3) ||(previous_char >= 0x17C9 && previous_char <= 0x17CA))
				{
					// T9 engine is running freely without any limition
					break;
				}
				else
				{
					//DV [17B4,17C5]
					if(previous_char >= 0x17B4 && previous_char <= 0x17C5)
					{
						key_table = &KhmerConversionSFMTable;
					}
					else if((previous_char >= 0x17C6 && previous_char <= 0x17C8) || previous_char == 0x17CC)
					{
						//RO[17CC,17CC], VM[17C6,17C8]
						key_table = &KhmerConversionSFSTable;
					}
				}
			}
			if (PNULL == key_table)
			{
				key_table = &KhmerConversionSSSTable;
			}
			break;
		case T9PIDSinhala:
			key_table = &SINHALA_CONVERSION_NONALL_TABLE;
			if (previous_char)
			{
				// is_sinhalese_consonant
				if (previous_char >= 0x0D9A && previous_char <= 0x0DC6)
				{
					key_table = &SINHALA_CONVERSION_CONSONANT_TABLE;
				}
				// is_sinhalese_vowel || is_sinhalese_matra
				else if ((previous_char >= 0x0D85 && previous_char <= 0x0D96)
					|| (previous_char >= 0x0DCF && previous_char <= 0x0DDF)
					|| (previous_char >= 0x0DF2 && previous_char <= 0x0DF3))
				{
					key_table = &SINHALA_CONVERSION_VOWEL_MATRA_NONCONSONANT_TABLE;
				}
			}
			break;

        case T9PIDAmharic:
            // forbid all vowel chars input if previous is null.
            if (!previous_char)
            {
                key_table = &AMHARIC_CONVERSION_NOVOWEL_TABLE;
            }
            break;

		default:
			/* don't do any conversion languages */
			break;
	}
	return key_table;
}


#ifdef IM_ENGINE_T9
//#ifdef T9CHINESE
//extern MMIIM_UI_CONFIG_T const g_mmiim_ui_config;



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/******************************************************************************/
// Description: get idc color by other state
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL GUI_COLOR_T GetIDCColor(MMIIM_IDC_STATE_T state)
{
    GUI_COLOR_T color = MMIIM_IDC_COLOR_INACTIVE;
    
    if (MMIIM_IDC_NONE == state
        || state == MMIIM_IDC_INACTIVE)
    {
        color = MMIIM_IDC_COLOR_INACTIVE;
    }
    else if (MMIIM_IDC_ACTIVE == state)
    {
        color = MMIIM_IDC_COLOR_ACTIVE;
    }
    else if (MMIIM_IDC_PRESSED == state)
    {
        color = MMIIM_IDC_COLOR_PRESSED;
    }

    return color;
}

/******************************************************************************/
// Description: 
// Global resource dependence: NONE
// Author: 
// Note: t9 maybe draw a icon here
/******************************************************************************/
LOCAL void DrawLeftIDC(GUI_LCD_DEV_INFO const *lcd_info_ptr, GUI_RECT_T const *r, GUI_COLOR_T c)
{
    uint16 x = 0, y = 0;
    int32 i;
    
    x = r->left + (r->right - r->left - MMIIM_IDC_WIDTH)/2;
    y = r->top + (r->bottom - r->top)/2;

    for(i=0; i<MMIIM_IDC_WIDTH; i++)
    {
        LCD_DrawVLine(lcd_info_ptr, x+i, y-i, y+i, c);
    }

    return;
}

/******************************************************************************/
// Description: 
// Global resource dependence: NONE
// Author: 
// Note: t9 maybe draw a icon here
/******************************************************************************/
LOCAL void DrawRightIDC(GUI_LCD_DEV_INFO const *lcd_info_ptr, GUI_RECT_T const *r, GUI_COLOR_T c)
{
    uint16 x = 0, y = 0;
    int32 i;
    uint16 delta_y;
    
    x = r->left + (r->right - r->left - MMIIM_IDC_WIDTH)/2;
    y = r->top + (r->bottom - r->top)/2;

    for(i=0; i<MMIIM_IDC_WIDTH; i++)
    {
        delta_y = MMIIM_IDC_WIDTH - 1 - i;
        LCD_DrawVLine(lcd_info_ptr, x+i, y-delta_y, y+delta_y, c);
    }

    return;
}


/*****************************************************************************/
//  Description : MMIIM_TextBarGenTextSeg
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: generate bar data
/*****************************************************************************/
PUBLIC int32 MMIIM_TextBarGenSeg(
    MMIIM_TEXT_BAR_T * bar_ptr,
    wchar split,
    MMIIM_BAR_TYPE_T bar_type
)
{
    uint32 text_idx = 0;
    uint16 cur_seg_len = 0;
    uint16 cur_seg_idx = 0;
    wchar *cur_seg_ptr = PNULL;
    GUI_FONT_T font_type = SONG_FONT_16;

    if (PNULL == bar_ptr)
    {
        return 1;
    }

    bar_ptr->seg_cnt = 0;
    SCI_MEMSET(bar_ptr->seg, 0x00, 
        MMIIM_TEXT_BAR_SEG_MAX*sizeof(MMIIM_TEXT_BAR_SEG_INFO_T));
        
    switch (bar_type)
    {
        case MMIIM_BAR_CAND:
            font_type = g_mmiim_ui_config.font_candidate;
            break;
        case MMIIM_BAR_SPELL:
            font_type = g_mmiim_ui_config.font_pre_choice;
            break;
        case MMIIM_BAR_EDIT:
            font_type = g_mmiim_ui_config.font_pre_edit;
            break;
        default:
            break;
    }
    
    cur_seg_ptr = bar_ptr->text;
    for (text_idx = 0; text_idx < bar_ptr->text_len; text_idx++)
    {
        if (split == bar_ptr->text[text_idx])
        {
            //set seg content
            bar_ptr->seg[cur_seg_idx].word_ptr = cur_seg_ptr;
            bar_ptr->seg[cur_seg_idx].len = cur_seg_len;
            bar_ptr->seg[cur_seg_idx].width = 
                GUI_GetStringWidth(font_type, 
                bar_ptr->seg[cur_seg_idx].word_ptr, cur_seg_len);
            bar_ptr->seg[cur_seg_idx].page_index = bar_ptr->page_index;
            
            cur_seg_len = 0;            
            cur_seg_ptr = &(bar_ptr->text[text_idx+1]);
            cur_seg_idx++;
            bar_ptr->seg_cnt = cur_seg_idx;
        }
        else
        {
            cur_seg_len++;
        }
    }

    return 0;
}

/*****************************************************************************/
//  Description : popup list create layer
//  Global resource dependence : none
//  Author:cheney.wu
//  Note:
/*****************************************************************************/
LOCAL void PopupListCreateLayer(GUI_LCD_DEV_INFO *dev_info_ptr, uint16 width, uint16 height)
{
#ifdef UI_MULTILAYER_SUPPORT	
    uint16 logic_lcd_width = 0;
    uint16 logic_lcd_height = 0;
    
    if (0 != dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != dev_info_ptr->block_id)
    {
        return;
    }

    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};
        GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
        
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle = 0;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = width;
        create_info.height = height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = TRUE;
        create_info.format = UILAYER_MEM_DOUBLE_COPY;
        UILAYER_CreateLayer(&create_info, dev_info_ptr);
        
        UILAYER_SetLayerColorKey(dev_info_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
    }
    
    if (UILAYER_IsLayerActive(dev_info_ptr))
    {
        UILAYER_Clear(dev_info_ptr);
    }
#endif
}

/*****************************************************************************/
//  Description : popup list release layer
//  Global resource dependence : none
//  Author:cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_PopupListReleaseLayer(MMIIM_TEXT_BAR_T * bar_ptr)
{
#ifdef UI_MULTILAYER_SUPPORT	
    if (0 != bar_ptr->lcd_dev_info.block_id
        && UILAYER_NULL_HANDLE != bar_ptr->lcd_dev_info.block_id)
    {
        UILAYER_ReleaseLayer(&bar_ptr->lcd_dev_info);
        bar_ptr->lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
    }
#else
    //MMK_UpdateScreen(); // bug-1009918:删除该刷新，会导致英语下新建信息左软键菜单无法显示
#endif
}

/*****************************************************************************/
//  Description : popup list remove layer
//  Global resource dependence : none
//  Author:cheney.wu
//  Note:
/*****************************************************************************/
LOCAL void PopupListRemoveLayer(GUI_LCD_DEV_INFO *dev_info_ptr)
{
#ifdef UI_MULTILAYER_SUPPORT	
    UILAYER_APPEND_BLT_T append_layer={0};
    
    append_layer.layer_level=UILAYER_LEVEL_HIGH;
    append_layer.lcd_dev_info=*dev_info_ptr;
    UILAYER_RemoveBltLayer(&append_layer);
#endif
}

/*****************************************************************************/
//  Description : popup list append layer
//  Global resource dependence : none
//  Author:cheney.wu
//  Note:
/*****************************************************************************/
LOCAL void PopupListAppendLayer(GUI_LCD_DEV_INFO *lcd_dev_info_ptr)
{
#ifdef UI_MULTILAYER_SUPPORT	
    UILAYER_APPEND_BLT_T append_layer={0};
    
    if (PNULL != lcd_dev_info_ptr
        && 0 != lcd_dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info_ptr->block_id)
    {
        PopupListRemoveLayer(lcd_dev_info_ptr);

        append_layer.layer_level=UILAYER_LEVEL_HIGH;
        append_layer.lcd_dev_info=*lcd_dev_info_ptr;
        UILAYER_AppendBltLayer(&append_layer);
    }
#endif
}

/*****************************************************************************/
//  Description : popup list clear layer
//  Global resource dependence : none
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_PopupListClearLayer(MMIIM_TEXT_BAR_T const * bar_ptr)
{
#ifdef UI_MULTILAYER_SUPPORT	
    if (PNULL == bar_ptr)
    {
        return 1;
    }
    
    UILAYER_Clear(&bar_ptr->lcd_dev_info);
    
    UILAYER_InvalidateRect(bar_ptr->lcd_dev_info.lcd_id, &bar_ptr->rect);
#endif
    return 0;
}

/*****************************************************************************/
//  Description : MMIIM_EditBarDraw
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: used for stroke
/*****************************************************************************/
PUBLIC int32 MMIIM_EditBarDraw(MMIIM_TEXT_BAR_T const * bar_ptr)
{
    uint8 seg_idx = 0;

    GUI_RECT_T bar_rect = {0};
    GUI_RECT_T text_rect = {0};
    //GUI_RECT_T hot_rect = {0};

    MMI_STRING_T text_info = {0};
    GUISTR_STYLE_T text_style = {0};

    GUISTR_STATE_T state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN;

    if (PNULL == bar_ptr)
    {
        return 1;
    }

    bar_rect = bar_ptr->rect;

    // 1 draw edit bar back ground, maybe samsung can replace to image,here just fill rect for simple
    LCD_FillRect(&bar_ptr->lcd_dev_info, bar_rect, 
        g_mmiim_ui_config.color_bg_pre_edit);

    if (0 == bar_ptr->text_len)
    {
        return 0;
    }
        
    // 2 draw label and text
//    text_rect = bar_ptr->rect;
    text_rect.top += MMIIM_TEXT_BAR_SPELL_Y;
    text_rect.bottom -= MMIIM_TEXT_BAR_SPELL_Y;

    text_style.font = g_mmiim_ui_config.font_pre_edit;
    text_style.font_color = g_mmiim_ui_config.color_font_pre_edit;
    text_style.align = ALIGN_LVMIDDLE;

    for (seg_idx = 0; seg_idx < bar_ptr->seg_cnt; seg_idx++)
    {
        if (0 == bar_ptr->seg[seg_idx].len) continue;
        
        if (bar_ptr->has_hot && bar_ptr->hot_index == seg_idx)
        {
            text_style.font_color = g_mmiim_ui_config.color_hot_pre_edit;
        }
        else
        {
            text_style.font_color = g_mmiim_ui_config.color_font_pre_edit;
        }

        text_rect.left += bar_ptr->width_space;

        if (text_rect.left + bar_ptr->seg[seg_idx].width > text_rect.right)
        {
            text_rect.left -= (text_rect.left + bar_ptr->seg[seg_idx].width 
                - text_rect.right);
        }

        // text
        text_info.wstr_ptr = bar_ptr->seg[seg_idx].word_ptr;
        text_info.wstr_len = bar_ptr->seg[seg_idx].len;

        GUISTR_DrawTextToLCDInRect(&bar_ptr->lcd_dev_info, 
            &text_rect, &bar_rect, &text_info, 
            &text_style, state, GUISTR_TEXT_DIR_AUTO);

        text_rect.left += bar_ptr->seg[seg_idx].width;
    }
        
    return 0;
}

/*****************************************************************************/
//  Description : MMIIM_SpellBarDraw
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: used for pinyin
/*****************************************************************************/
PUBLIC int32 MMIIM_SpellBarDraw(MMIIM_TEXT_BAR_T * bar_ptr, BOOLEAN is_need_border)
{
    uint8 seg_idx = 0;
    uint16 start_idx = 0;
    uint16 end_idx = 0;
    uint16 layer_width = 0;
    uint16 layer_height = 0;
    uint16 text_height = MMIIM_TEXT_BAR_EDIT_HEIGHT + 2*MMIIM_TEXT_BAR_BORDER_WIDTH;

    GUI_RECT_T bar_rect = {0};
    GUI_RECT_T text_rect = {0};
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T bar_alpha_rect = {0};

    MMI_STRING_T text_info = {0};
    GUISTR_STYLE_T text_style = {0};
    uint16 bpmf_text_buffer[MMIIM_T9_CC_SELECTPAGE_NUM + 1] = {0};
    GUISTR_STATE_T state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN;
    GUIIM_LANGUAGE_T input_lang = GUIIM_LANG_NONE;
    
    input_lang = MMIAPIIM_GetLanguage();

    if (PNULL == bar_ptr || 0 == bar_ptr->seg_cnt)
    {
        return 1;
    }

    // get the common ui style : font, font_color and align
    text_style.font = (is_need_border ? g_mmiim_ui_config.font_candidate : g_mmiim_ui_config.font_default);
    text_style.font_color = g_mmiim_ui_config.color_font_candidate;
    //label_style.font_color = g_mmiim_ui_config.color_font_lable;
    text_style.align = ALIGN_HVMIDDLE;

#ifdef UI_MULTILAYER_SUPPORT	
    // Create the layer with the maximum size(screen_width * full_single_page_hight) and place it at the (0,0)
    layer_width = MMIIM_TEXT_POPUP_LAYER_WIDTH;
    layer_height = text_height * MMIIM_T9_CC_SELECTPAGE_NUM;//MMIIM_TEXT_POPUP_HEIGHT + 2*MMIIM_TEXT_POPUP_MARGIN + 2*MMIIM_TEXT_BAR_BORDER_WIDTH;
    PopupListCreateLayer(&bar_ptr->lcd_dev_info, layer_width, layer_height);
    MMIIM_PopupListClearLayer(bar_ptr);
    UILAYER_SetLayerPosition(&bar_ptr->lcd_dev_info, 0, 0);

    layer_width = bar_ptr->rect.right - bar_ptr->rect.left;
    bar_alpha_rect.right = layer_width;
    bar_alpha_rect.bottom = bar_ptr->rect.bottom - bar_ptr->rect.top;
    bar_rect = bar_alpha_rect;
#else
    bar_rect = bar_ptr->rect;
#endif

    if(is_need_border)
    {
        text_height -= 2*MMIIM_TEXT_BAR_BORDER_WIDTH;

        LCD_FillArgbRect(
	        &bar_ptr->lcd_dev_info,
	        bar_alpha_rect,
	        MMI_BLACK_COLOR,
	        GUIILIST_MASK_BG_ALPHA);

        bar_rect.left += MMIIM_TEXT_BAR_BORDER_WIDTH;
        bar_rect.top += MMIIM_TEXT_BAR_BORDER_WIDTH;
        bar_rect.bottom -= MMIIM_TEXT_BAR_BORDER_WIDTH;
    }

    LCD_FillRect(
		&bar_ptr->lcd_dev_info,
		bar_rect, 
		MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_2));

    text_rect.left = bar_rect.left;
    text_rect.top = bar_rect.top;
    text_rect.right = bar_rect.right;

    // handle the multi pages case based on the hot_index
    start_idx = ((bar_ptr->hot_index)/MMIIM_T9_CC_SELECTPAGE_NUM)*MMIIM_T9_CC_SELECTPAGE_NUM;
    end_idx = MIN((start_idx + MMIIM_T9_CC_SELECTPAGE_NUM - 1), (bar_ptr->seg_cnt));

    for (seg_idx = start_idx; seg_idx <= end_idx; seg_idx++)
    {
        text_rect.bottom = text_rect.top + text_height - 1;

        if (bar_ptr->hot_index == seg_idx)
        {
            LCD_FillRect(&bar_ptr->lcd_dev_info, text_rect, 
                MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_POS_1));
        }

        // If in BPMF mode, need to change the mapping
        if(GUIIM_LANG_CHT == input_lang)
        {
            memset(bpmf_text_buffer, 0x00, MMIIM_T9_CC_SELECTPAGE_NUM*sizeof(wchar));
            if(PNULL != bar_ptr->seg[seg_idx].word_ptr)
            {
                int i;
                for(i = 0; i < bar_ptr->seg[seg_idx].len; i++)
                {
                    bpmf_text_buffer[i] = T9BPMFInternalToUnicode((uint8)(bar_ptr->seg[seg_idx].word_ptr[i]));
                }
            }
            text_info.wstr_ptr = bpmf_text_buffer;
            text_info.wstr_len = bar_ptr->seg[seg_idx].len;
        }
        else
        {
            text_info.wstr_ptr = bar_ptr->seg[seg_idx].word_ptr;
            text_info.wstr_len = bar_ptr->seg[seg_idx].len;
        }

        GUISTR_DrawTextToLCDInRect(&bar_ptr->lcd_dev_info, &text_rect, 
            &text_rect, &text_info, &text_style, state, GUISTR_TEXT_DIR_AUTO);

        text_rect.top += text_height;
    }

#ifdef UI_MULTILAYER_SUPPORT	
    UILAYER_SetLayerPosition(
        &bar_ptr->lcd_dev_info,
        bar_ptr->rect.left,
        bar_ptr->rect.top);
    PopupListAppendLayer(&bar_ptr->lcd_dev_info);
    UILAYER_InvalidateRect(bar_ptr->lcd_dev_info.lcd_id, &bar_ptr->rect);
#else
    GUILCD_InvalidateRect(bar_ptr->lcd_dev_info.lcd_id, bar_ptr->rect, 0);
#endif

    return 0;
}

/*****************************************************************************/
//  Description : MMIIM_CandBarDraw
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_CandBarDraw(MMIIM_TEXT_BAR_T * bar_ptr, GUI_RECT_T pre_bar_rect, BOOLEAN is_need_border)
{
    uint8 seg_idx = 0;
    uint16 start_idx = 0;
    uint16 end_idx = 0;
    uint16 layer_width = 0;
    uint16 layer_height = 0;
    uint16 text_height = MMIIM_TEXT_BAR_EDIT_HEIGHT + 2*MMIIM_TEXT_BAR_BORDER_WIDTH;
    uint16 layer_position_offset_minus = 0;//used for adjusting the layer start position when the mid_border is in the spell bar area

    GUI_RECT_T bar_rect = {0};
    GUI_RECT_T bar_alpha_rect = {0};
    GUI_RECT_T text_rect = {0};
    GUI_RECT_T disp_rect = {0};

    GUISTR_STYLE_T label_style = {0};
    MMI_STRING_T text_info = {0};
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN;

    BOOLEAN is_only_candbar_shown = ((0 == pre_bar_rect.bottom) && (0 == pre_bar_rect.right));

    if (PNULL == bar_ptr || 0 == bar_ptr->seg_cnt)
    {
        return 1;
    }

    // [develop!!!]should always show hot for pinyin spelling word
    start_idx = ((bar_ptr->hot_index)/MMIIM_T9_CC_SELECTPAGE_NUM)*MMIIM_T9_CC_SELECTPAGE_NUM;
    end_idx = MIN((start_idx + MMIIM_T9_CC_SELECTPAGE_NUM - 1), (bar_ptr->seg_cnt - 1));
	
#ifdef UI_MULTILAYER_SUPPORT	
    layer_width = MMIIM_TEXT_POPUP_LAYER_WIDTH;
    layer_height = text_height * MMIIM_T9_CC_SELECTPAGE_NUM;
    PopupListCreateLayer(&bar_ptr->lcd_dev_info, layer_width, layer_height);
    MMIIM_PopupListClearLayer(bar_ptr);
    UILAYER_SetLayerPosition(&bar_ptr->lcd_dev_info, 0, 0);
#else
    bar_alpha_rect.left = ((layer_position_offset_minus > 0) ? (bar_ptr->rect.left - layer_position_offset_minus): bar_ptr->rect.left);
     bar_alpha_rect.top = bar_ptr->rect.top;
#endif
	// 1 draw candidate bar background, maybe can replace to image,here just fill rect for simple
    text_style.font = (is_need_border ? g_mmiim_ui_config.font_candidate : g_mmiim_ui_config.font_default);
    text_style.font_color = g_mmiim_ui_config.color_font_candidate;
    text_style.align = (is_only_candbar_shown ? ALIGN_LVMIDDLE : ALIGN_HVMIDDLE);

    layer_width = bar_ptr->rect.right - bar_ptr->rect.left;

    bar_alpha_rect.right = bar_alpha_rect.left + layer_width;// + 2*MMIIM_TEXT_BAR_BORDER_WIDTH;
    bar_alpha_rect.bottom = bar_alpha_rect.top + bar_ptr->rect.bottom - bar_ptr->rect.top;

    if(is_need_border)
    {
        text_height -= 2*MMIIM_TEXT_BAR_BORDER_WIDTH;

        if (0 != pre_bar_rect.bottom && pre_bar_rect.bottom != bar_ptr->rect.bottom)
        {
            //handle the region(mid-border) between the two bars
            //1. get the difference rect of the two rect
            uint16 start_x = 0;
            uint16 start_y = 0;
            GUI_RECT_T mid_border_rect = {0};

            mid_border_rect.left = 0;
            mid_border_rect.right = MMIIM_TEXT_BAR_BORDER_WIDTH;

            if (pre_bar_rect.bottom > bar_ptr->rect.bottom)
            {
                start_x = pre_bar_rect.right;
                start_y = bar_ptr->rect.bottom;
                mid_border_rect.bottom = pre_bar_rect.bottom - bar_ptr->rect.bottom;
            } 
            else
            {
                start_x = pre_bar_rect.right - MMIIM_TEXT_BAR_BORDER_WIDTH;
                start_y = pre_bar_rect.bottom;
                mid_border_rect.bottom = bar_ptr->rect.bottom - pre_bar_rect.bottom;
                layer_position_offset_minus = MMIIM_TEXT_BAR_BORDER_WIDTH;//mark the offset
            }
            mid_border_rect.top = start_y - pre_bar_rect.top + 1;//pre_bar_rect.top is always equal to bar_ptr->rect.top
            mid_border_rect.bottom += mid_border_rect.top - 1;

            LCD_FillArgbRect(
				&bar_ptr->lcd_dev_info,
				mid_border_rect,
				MMI_BLACK_COLOR,
				GUIILIST_MASK_BG_ALPHA);

            if (layer_position_offset_minus > 0)
            {
                bar_alpha_rect.left += layer_position_offset_minus;
                bar_alpha_rect.right += layer_position_offset_minus;
            }
			
        }
    }

    if (!is_only_candbar_shown)
    {
        bar_alpha_rect.right -= MMIIM_TEXT_BAR_BORDER_WIDTH;
    }
    
    bar_rect = bar_alpha_rect;
    
    LCD_FillArgbRect(
        &bar_ptr->lcd_dev_info,
        bar_alpha_rect,
        MMI_BLACK_COLOR,
        GUIILIST_MASK_BG_ALPHA);
    
    if (is_only_candbar_shown)
    {
        bar_rect.left += MMIIM_TEXT_BAR_BORDER_WIDTH;
    }
    if(is_need_border)
    {
        bar_rect.top += MMIIM_TEXT_BAR_BORDER_WIDTH;
        bar_rect.bottom -= MMIIM_TEXT_BAR_BORDER_WIDTH;
        bar_rect.right -= MMIIM_TEXT_BAR_BORDER_WIDTH;
    }
    
    LCD_FillRect(
    	&bar_ptr->lcd_dev_info,
    	bar_rect, 
       MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_2));
	
    text_rect.left = bar_rect.left;
    text_rect.top = bar_rect.top;
    text_rect.right = bar_rect.right;

    text_style.font_color = g_mmiim_ui_config.color_font_candidate;
    label_style.font_color = g_mmiim_ui_config.color_font_lable;

    for (seg_idx = start_idx; seg_idx <= end_idx; seg_idx++)
    {
        text_rect.bottom = text_rect.top + text_height - 1;

        if (bar_ptr->has_hot && bar_ptr->hot_index == seg_idx)
        {
            LCD_FillRect(&bar_ptr->lcd_dev_info, text_rect, 
                MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_POS_1));
        }

        // text
        text_info.wstr_ptr = bar_ptr->seg[seg_idx].word_ptr;
        text_info.wstr_len = bar_ptr->seg[seg_idx].len;

        GUISTR_DrawTextToLCDInRect(&bar_ptr->lcd_dev_info, &text_rect, 
            &text_rect, &text_info, &text_style, state, GUISTR_TEXT_DIR_AUTO);
        SCI_TRACE_LOW("MMIIM_CandBarDraw : The %d word = %08X.", seg_idx, *(text_info.wstr_ptr));

        text_rect.top += text_height;
    }

#ifdef UI_MULTILAYER_SUPPORT	
    UILAYER_SetLayerPosition(
        &bar_ptr->lcd_dev_info,
        ((layer_position_offset_minus > 0) ? (bar_ptr->rect.left - layer_position_offset_minus): bar_ptr->rect.left),
        bar_ptr->rect.top);
    PopupListAppendLayer(&bar_ptr->lcd_dev_info);
    UILAYER_InvalidateRect(bar_ptr->lcd_dev_info.lcd_id, &bar_ptr->rect);
#endif

    return 0;
}
/*****************************************************************************/
//  Description : MMIIM_PredictBarDraw
//  Global resource dependence : none
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_PredictBarDraw(
	MMIIM_TEXT_BAR_T * bar_ptr,
	MMIIM_CAND_WIN_T *cand_info_ptr,
	MMI_DIRECTION_SET_E dis_direction,
	BOOLEAN is_need_border)
{    
    uint8 seg_idx = 0;
    uint16 i = 0, j = 0;
    uint16 nIdx = 0;
    uint16 start_idx = 0;
    uint16 end_idx = 0;
    uint16 layer_width = 0;
    uint16 layer_height = 0;
    uint16 word_display_margin = MMIIM_TEXT_EDIT_BAR_SPACE;
    uint16 text_height = MMIIM_TEXT_BAR_EDIT_HEIGHT + 2*MMIIM_TEXT_BAR_BORDER_WIDTH;
    
    wchar curChar = 0, preChar = 0;
    wchar candBuffer[GUIIM_COMMIT_BUFFER_MAX + 1] = {0};//temp buffer used to store display string based on every candidate
    
    GUI_RECT_T bar_rect = {0};
    GUI_RECT_T text_rect = {0};
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T bar_alpha_rect = {0};
    MMI_STRING_T text_info = {0};
    MMI_STRING_T keymap_info = {0};
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN;

    if (PNULL == bar_ptr)
    {
        return 1;
    }

#ifdef UI_MULTILAYER_SUPPORT	
    // Create the layer with the maximum size(screen_width * full_single_page_hight) and place it at the (0,0)
    layer_width = MMIIM_TEXT_POPUP_LAYER_WIDTH;
    layer_height = text_height * MMIIM_T9_CC_SELECTPAGE_NUM;//MMIIM_TEXT_POPUP_HEIGHT + 2*MMIIM_TEXT_POPUP_MARGIN;

    PopupListCreateLayer(&bar_ptr->lcd_dev_info, layer_width, layer_height);
    MMIIM_PopupListClearLayer(bar_ptr);
    UILAYER_SetLayerPosition(&bar_ptr->lcd_dev_info, 0, 0);

    layer_width = bar_ptr->rect.right - bar_ptr->rect.left;
    bar_alpha_rect.right = layer_width;
    bar_alpha_rect.bottom = bar_ptr->rect.bottom - bar_ptr->rect.top;
    bar_rect = bar_alpha_rect;
#else
    bar_rect = bar_ptr->rect;
    bar_alpha_rect = bar_rect;
#endif

    if(is_need_border)
    {
        word_display_margin += MMIIM_TEXT_BAR_BORDER_WIDTH;
        text_height -= 2*MMIIM_TEXT_BAR_BORDER_WIDTH;

        LCD_FillArgbRect(
	        &bar_ptr->lcd_dev_info,
	        bar_alpha_rect,
	        MMI_BLACK_COLOR,
	        GUIILIST_MASK_BG_ALPHA);
		
        bar_rect.left += MMIIM_TEXT_BAR_BORDER_WIDTH;
        bar_rect.top += MMIIM_TEXT_BAR_BORDER_WIDTH;
        bar_rect.right -= MMIIM_TEXT_BAR_BORDER_WIDTH;
        bar_rect.bottom -= MMIIM_TEXT_BAR_BORDER_WIDTH;
    }

    LCD_FillRect(
		&bar_ptr->lcd_dev_info,
		bar_rect, 
		MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_2));

    text_rect.left = bar_rect.left;
    text_rect.top = bar_rect.top;
    text_rect.right = bar_rect.right;

    // get the common ui style : font, font_color and align
    text_style.font = (is_need_border ? g_mmiim_ui_config.font_candidate : g_mmiim_ui_config.font_default);
    text_style.font_color = g_mmiim_ui_config.color_font_candidate;
    text_style.align = ((MMI_DIRECTION_LEFT == dis_direction) ? ALIGN_LVMIDDLE : ALIGN_RVMIDDLE);

    // handle the multi pages case based on the hot_index
    start_idx = ((cand_info_ptr->cur_sel-1)/MMIIM_T9_CC_SELECTPAGE_NUM)*MMIIM_T9_CC_SELECTPAGE_NUM;
    end_idx = MIN((start_idx+MMIIM_T9_CC_SELECTPAGE_NUM-1), (cand_info_ptr->count));

    for (seg_idx = start_idx; seg_idx <= end_idx; seg_idx++)
    {
        text_rect.bottom = text_rect.top + text_height - 1;

        if ((cand_info_ptr->cur_sel-1) == seg_idx)
        {
            LCD_FillRect(&bar_ptr->lcd_dev_info, text_rect, 
                MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_POS_1));
        }

        if(seg_idx == cand_info_ptr->count)
        {
            // +add
            MMI_GetLabelTextByLang(STR_INPUT_ADD_EXT01, &text_info);
        }
        else
        {
            // text
            //text_info.wstr_ptr = cand_info_ptr->buffer[seg_idx];
            text_info.wstr_len = MMIAPICOM_Wstrlen(cand_info_ptr->buffer[seg_idx]);

            for (nIdx = 0, i = 0, preChar = 0; i < text_info.wstr_len; i++)
            {
                curChar = cand_info_ptr->buffer[seg_idx][i];
                if (MMIIM_CheckAndDecodeCombineChar(curChar, &keymap_info))
                {
                    for (j = 0; j < keymap_info.wstr_len; j++)
                    {
                        candBuffer[nIdx++] = *(keymap_info.wstr_ptr + j);
                    }
                }
                // Check Amharic modifier Vowel specifically
                else if (MMIIM_CheckAmharicMappingChar(preChar, curChar, &keymap_info))
                {
                    // Override (the preChar if exists) or (curChar and plus nIdx one)
                    j = ((nIdx > 0) ? (nIdx - 1) : nIdx++);
                    candBuffer[j] = *(keymap_info.wstr_ptr);
                }
                else
                {
                    candBuffer[nIdx++] = curChar;
                }
                
                //update preChar for the next loop
                preChar = candBuffer[nIdx - 1];
            }
            text_info.wstr_ptr = candBuffer;
            text_info.wstr_len = nIdx;
        }

        disp_rect = text_rect;
        // every word should have 6px(+3px if need border) margin between the boundary
        if (MMI_DIRECTION_LEFT == dis_direction)
        {
		disp_rect.left += word_display_margin;
        }
        else
        {
            disp_rect.right -= word_display_margin;
        }

        GUISTR_DrawTextToLCDInRect(&bar_ptr->lcd_dev_info, &disp_rect, 
            &text_rect, &text_info, &text_style, state, GUISTR_TEXT_DIR_AUTO);
		
        text_rect.top += text_height;

    }
#ifdef UI_MULTILAYER_SUPPORT	
    UILAYER_SetLayerPosition(
        &bar_ptr->lcd_dev_info,
        bar_ptr->rect.left,
        bar_ptr->rect.top);
    PopupListAppendLayer(&bar_ptr->lcd_dev_info);
    UILAYER_InvalidateRect(bar_ptr->lcd_dev_info.lcd_id, &bar_ptr->rect);
#else
    GUILCD_InvalidateRect(bar_ptr->lcd_dev_info.lcd_id, bar_ptr->rect, 0);
#endif
    return 0;
}

/*****************************************************************************/
//  Description : MMIIM_SP_TextBarClear
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_TextBarClear(MMIIM_TEXT_BAR_T *bar_ptr)
{
    if (PNULL == bar_ptr)
    {
        return 1;
    }
    
    SCI_MEMSET(bar_ptr->seg, 0x00, MMIIM_TEXT_BAR_SEG_MAX*sizeof(MMIIM_TEXT_BAR_SEG_INFO_T));
    
    bar_ptr->seg_cnt = 0;
    bar_ptr->hot_index = 0; 
    bar_ptr->text_len = 0; 
    bar_ptr->page_index = 0;

    bar_ptr->seg_index = 0;
    bar_ptr->cur_page_cnt = 0;
    bar_ptr->page_cnt = 0;

    return 0;
}

/*****************************************************************************/
//  Description : MMIIM_SP_TextBarInit
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_TextBarInit(MMIIM_TEXT_BAR_T * bar_ptr)
{
    if (PNULL == bar_ptr)
    {
        return 1;
    }

    SCI_MEMSET(bar_ptr, 0, sizeof(MMIIM_TEXT_BAR_T));

    return 0;
}

/*****************************************************************************/
//  Description : MMIIM_InitIdcRect
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_InitIdcRect(MMIIM_TEXT_BAR_T * bar_ptr)
{
    uint16 mid_top = 0;
    uint16 mid_bottom = 0;

    uint16 right_left = 0, left_left = 0;
    uint16 right_right = 0;

    mid_top = (MMIIM_TEXT_BAR_EDIT_HEIGHT - MMIIM_TEXT_BAR_IDC_H)/2;
    mid_bottom = mid_top + MMIIM_TEXT_BAR_IDC_H - 1;

    right_right = bar_ptr->rect.right - MMIIM_TEXT_BAR_SPACE - 1;
    right_left = right_right - MMIIM_TEXT_BAR_IDC_W + 1;
    left_left = bar_ptr->rect.left + MMIIM_TEXT_BAR_SPACE;
        
    //left
    bar_ptr->rect_idc_left.bottom = bar_ptr->rect.top + mid_bottom;
    bar_ptr->rect_idc_left.top = bar_ptr->rect.top + mid_top;
    bar_ptr->rect_idc_left.left = left_left;
    bar_ptr->rect_idc_left.right = bar_ptr->rect_idc_left.left 
        + MMIIM_TEXT_BAR_IDC_W - 1;

    //right
    bar_ptr->rect_idc_right.top = bar_ptr->rect_idc_left.top;
    bar_ptr->rect_idc_right.bottom = bar_ptr->rect_idc_left.bottom;
    bar_ptr->rect_idc_right.right = right_right;
    bar_ptr->rect_idc_right.left = right_left;
    
    return 0;
}

/*****************************************************************************/
//  Description : popup list create layer for chinese
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_PopupListCreateLayer(MMIIM_TEXT_BAR_T * bar_ptr)
{
#ifdef UI_MULTILAYER_SUPPORT	
    uint16 layer_width = 0;
    uint16 layer_height = 0;

    layer_width = MMIIM_TEXT_POPUP_LAYER_WIDTH;
    layer_height = MMIIM_TEXT_POPUP_HEIGHT + 2*MMIIM_TEXT_POPUP_MARGIN;
    PopupListCreateLayer(&bar_ptr->lcd_dev_info, layer_width, layer_height);

    MMIIM_PopupListClearLayer(bar_ptr);
    UILAYER_SetLayerPosition(&bar_ptr->lcd_dev_info, 0, 0);
#endif
}

/*****************************************************************************/
//  Description : popup list append layer for chinese
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_PopupListAppendLayer(GUI_LCD_DEV_INFO *dev_info_ptr, GUI_RECT_T *bar_rect)
{
#ifdef UI_MULTILAYER_SUPPORT	
    UILAYER_SetLayerPosition(dev_info_ptr, bar_rect->left, bar_rect->top);
    PopupListAppendLayer(dev_info_ptr);
    UILAYER_InvalidateRect(dev_info_ptr->lcd_id, &bar_rect);
#else
    GUILCD_InvalidateRect(dev_info_ptr->lcd_id, *bar_rect, 0);
#endif
}

/*****************************************************************************/
//  Description : popup list append layer for chinese
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcInsertBorderRect(GUI_RECT_T *left_rect, GUI_RECT_T *right_rect)
{
	GUI_RECT_T insert_rect = {0};

	if (PNULL == left_rect || PNULL == right_rect || (left_rect->bottom == right_rect->bottom))
	{
		return insert_rect;
	}
	
	if (0 == left_rect->bottom || (left_rect->bottom < right_rect->bottom))
	{
		insert_rect.top = ((0 == left_rect->bottom) ? (right_rect->top + MMIIM_TEXT_BAR_BORDER_WIDTH) : left_rect->bottom);
		insert_rect.right = right_rect->left;
		insert_rect.left = insert_rect.right - MMIIM_TEXT_BAR_BORDER_WIDTH;
		insert_rect.bottom = right_rect->bottom + MMIIM_TEXT_BAR_BORDER_WIDTH;
	}
	else
	{
		insert_rect.top = right_rect->bottom;
		insert_rect.left = left_rect->right;
		insert_rect.right = insert_rect.left + MMIIM_TEXT_BAR_BORDER_WIDTH;
		insert_rect.bottom = left_rect->bottom + MMIIM_TEXT_BAR_BORDER_WIDTH;
	}


	return insert_rect;
}

/*****************************************************************************/
//  Description : check and decode the combine char if needed
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIM_CheckAndDecodeCombineChar(wchar curChar, MMI_STRING_T* keymap_info_ptr)
{
	uint16 nIndex = 0;
	BOOLEAN result = FALSE;

	/* first, should map the PUA character provided by T9 engine to the its combine char value */
	while ((T9_PUA_COMBINE_MAPPING_TABLE[nIndex][0] != 0xFFFF) && (T9_PUA_COMBINE_MAPPING_TABLE[nIndex][0] != curChar))
	{
		nIndex++;
	}
	SCI_TRACE_LOW("[MMIIM] MMIIM_CheckAndDecodeCombineChar: curChar=%x, mapping=%x.", curChar, T9_PUA_COMBINE_MAPPING_TABLE[nIndex][1]);
	curChar = T9_PUA_COMBINE_MAPPING_TABLE[nIndex][1];

	/* second, get combine data if the current character is in the COMBINE interval */
	if ((PNULL != keymap_info_ptr) && ((MMIIM_SP_KEYMAP_COMBINE_START <= curChar) && (MMIIM_SP_KEYMAP_COMBINE_END >= curChar)))
	{
		keymap_info_ptr->wstr_ptr = MMIIM_SP_ML9KEY_GetCombindata(curChar);
		keymap_info_ptr->wstr_len = MMIAPICOM_Wstrlen(keymap_info_ptr->wstr_ptr);
		result = TRUE;
	}
	
	return result;
}

/*****************************************************************************/
//  Description : check and decode the combine char if needed
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIM_CheckAmharicMappingChar(wchar preChar, wchar curChar, MMI_STRING_T* keymap_info_ptr)
{
    BOOLEAN result = FALSE;
    uint16 conIdx = MMIIM_AMHARIC_CONSONANT_NUM;
    uint16 vowIdx = MMIIM_AMHARIC_PUA_SYMS_NUM;
    
    if (PNULL != keymap_info_ptr)
    {
        keymap_info_ptr->wstr_ptr = NULL;
        
        if ((MMIIM_AMHARIC_PUA_SYMS_START <= curChar) && (MMIIM_AMHARIC_PUA_SYMS_END >= curChar))
        {
            // To calculate the right conIdx and vowIdx if preChar is in Amharic Unicode Block: 
            // MYA, FYA or the Unicode is in [1200, 1358] and can be divisible by 0x0008 as well.
            if (MMIIM_AMHARIC_SPECIAL_CONSONANT_MYA == preChar || MMIIM_AMHARIC_SPECIAL_CONSONANT_FYA == preChar)
            {
                vowIdx = 0;
                conIdx = MMIIM_AMHARIC_CONSONANT_NUM - (MMIIM_AMHARIC_SPECIAL_CONSONANT_FYA - preChar + 1);
            }
            else if ((MMIIM_AMHARIC_CONSONANT_START <= preChar) && (MMIIM_AMHARIC_CONSONANT_END >= preChar) && ((preChar % MMIIM_AMHARIC_PUA_SYMS_NUM) == 0))
            {
                conIdx = (preChar - MMIIM_AMHARIC_CONSONANT_START) / MMIIM_AMHARIC_PUA_SYMS_NUM;
                vowIdx = (curChar - MMIIM_AMHARIC_PUA_SYMS_START);
            }

            SCI_TRACE_LOW("[MMIIM] MMIIM_CheckAmharicMappingChar: preChar=%x, curChar=%x, conIdx=%d, vowIdx=%d.", preChar, curChar, conIdx, vowIdx);
            if (keymap_info_ptr->wstr_ptr = MMIIM_GetAmharicMappingChar(conIdx, vowIdx))
            {
                keymap_info_ptr->wstr_len = 1;//MMIAPICOM_Wstrlen(keymap_info_ptr->wstr_ptr);
                result = TRUE;
            }
        }
    }
    
   	return result;
}

/*****************************************************************************/
//  Description : check and decode the combine char if needed
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
LOCAL wchar const* MMIIM_GetAmharicMappingChar(uint16 colIdx, uint16 vowIdx)
{
    wchar const *mapping_char_ptr = NULL;

    if (MMIIM_AMHARIC_CONSONANT_NUM > colIdx && MMIIM_AMHARIC_PUA_SYMS_NUM > vowIdx)
    {
        mapping_char_ptr = &AmharicTransTable[colIdx][vowIdx];
    }

    return mapping_char_ptr;
}
//#endif
#endif  //#ifdef IM_ENGINE_T9 


#ifdef __cplusplus
    }
#endif
