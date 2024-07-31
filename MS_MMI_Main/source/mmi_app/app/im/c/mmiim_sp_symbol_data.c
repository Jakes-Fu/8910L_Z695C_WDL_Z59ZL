/*==============================================================================
File Name: spim_symbol.c
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 符号
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/

#include "mmiim_sp_symbol_data.h"
#include "mmk_type.h"

#include "os_api.h"


static __inline uint32 spim_wcslen(wchar *str)
{
    uint32 len = 0;

    if (NULL != str)
    {
        while (str[len]) len++;
    }

    return len;
}



static wchar const *symb_sign_str[] =
{
    L"http://", //MMIIM_SP_SYMB_SIGN_HTTP
    L"www.", //MMIIM_SP_SYMB_SIGN_WWW
    L".com", //MMIIM_SP_SYMB_SIGN_COM
    L".net", //MMIIM_SP_SYMB_SIGN_NET
    L".org", //MMIIM_SP_SYMB_SIGN_ORG
};

#if MMIIM_SP_SYMBOL_SUPPORT_FACE
static wchar const *symb_face_str[] =
{
    L":)",      //MMIIM_SP_SYMB_FACE_EM00
    L":(",      //MMIIM_SP_SYMB_FACE_EM01
    L":D",      //MMIIM_SP_SYMB_FACE_EM02
    L":P",      //MMIIM_SP_SYMB_FACE_EM03
    L":O",      //MMIIM_SP_SYMB_FACE_EM04
    L":X",      //MMIIM_SP_SYMB_FACE_EM05
    L":|",      //MMIIM_SP_SYMB_FACE_EM06
    L":-)",     //MMIIM_SP_SYMB_FACE_EM07
    L":-(",     //MMIIM_SP_SYMB_FACE_EM08
    L":-D",     //MMIIM_SP_SYMB_FACE_EM09
    L":-P",     //MMIIM_SP_SYMB_FACE_EM10
    L":-O",     //MMIIM_SP_SYMB_FACE_EM11
    L":-X",     //MMIIM_SP_SYMB_FACE_EM12
    L":-|",     //MMIIM_SP_SYMB_FACE_EM13
    L":-#",     //MMIIM_SP_SYMB_FACE_EM14
    L"^_^",     //MMIIM_SP_SYMB_FACE_EM15
    L"-_-",     //MMIIM_SP_SYMB_FACE_EM16
    L"=_=",     //MMIIM_SP_SYMB_FACE_EM17
    L"+_+",     //MMIIM_SP_SYMB_FACE_EM18
    L"*_*",     //MMIIM_SP_SYMB_FACE_EM19
    L"$_$",     //MMIIM_SP_SYMB_FACE_EM20
    L"@_@",     //MMIIM_SP_SYMB_FACE_EM21
    L"#_#",     //MMIIM_SP_SYMB_FACE_EM22
    L"&_&",     //MMIIM_SP_SYMB_FACE_EM23
    L"!_!",     //MMIIM_SP_SYMB_FACE_EM24
    L"Q_Q",     //MMIIM_SP_SYMB_FACE_EM25
    L"T_T",     //MMIIM_SP_SYMB_FACE_EM26
    L"O_O",     //MMIIM_SP_SYMB_FACE_EM27
    L"O_o",     //MMIIM_SP_SYMB_FACE_EM28
    L"p_q",     //MMIIM_SP_SYMB_FACE_EM29
    L">_<",     //MMIIM_SP_SYMB_FACE_EM30
    L"^3^",     //MMIIM_SP_SYMB_FACE_EM31
    L"^o^",     //MMIIM_SP_SYMB_FACE_EM32
    L"orz",     //MMIIM_SP_SYMB_FACE_EM33
    L"Orz",     //MMIIM_SP_SYMB_FACE_EM34
    L"or2",     //MMIIM_SP_SYMB_FACE_EM35
    L"sto",     //MMIIM_SP_SYMB_FACE_EM36
    L"stO",     //MMIIM_SP_SYMB_FACE_EM37
    L"-_-+",    //MMIIM_SP_SYMB_FACE_EM38
    L"-_-||",   //MMIIM_SP_SYMB_FACE_EM39
    L"-_-;;",   //MMIIM_SP_SYMB_FACE_EM40
    L">_<||",   //MMIIM_SP_SYMB_FACE_EM41
    L"(-.-)",   //MMIIM_SP_SYMB_FACE_EM42
    L"{>~<}",   //MMIIM_SP_SYMB_FACE_EM43
    L"*^_^*",   //MMIIM_SP_SYMB_FACE_EM44
    L"#^_^#",   //MMIIM_SP_SYMB_FACE_EM45
    L"Y^o^Y",   //MMIIM_SP_SYMB_FACE_EM46
    L"=^.^=",   //MMIIM_SP_SYMB_FACE_EM47
    L"^(oo)^",  //MMIIM_SP_SYMB_FACE_EM48
    L"(T__T)",  //MMIIM_SP_SYMB_FACE_EM49
    L"(^_^)?",  //MMIIM_SP_SYMB_FACE_EM50
    L"(?_?)?",  //MMIIM_SP_SYMB_FACE_EM51
    L"(#^_^)",  //MMIIM_SP_SYMB_FACE_EM52
    L"(#_<-)",  //MMIIM_SP_SYMB_FACE_EM53
    L"q(^_^)p", //MMIIM_SP_SYMB_FACE_EM54
    L"\\{O.O)/", //MMIIM_SP_SYMB_FACE_EM55
    L"-_-)y-~", //MMIIM_SP_SYMB_FACE_EM56
    L"::>_<::", //MMIIM_SP_SYMB_FACE_EM57
    L"..>_<..", //MMIIM_SP_SYMB_FACE_EM58
    L"-.-)zZZ", //MMIIM_SP_SYMB_FACE_EM59
    L"(_r_)=3", //MMIIM_SP_SYMB_FACE_EM60
    L"..<{=..", //MMIIM_SP_SYMB_FACE_EM61
    L"{^o~o^}", //MMIIM_SP_SYMB_FACE_EM62
    L"|(-_-)|", //MMIIM_SP_SYMB_FACE_EM63
};
#endif
//换行符的处理
static wchar const symb_line_break[] =
{
    '\n',
};
//空格符的处理
static wchar const symb_space_char[] =
{
    0x20,
};
static wchar const symb_img[] =
{
    ' ',                    //MMIIM_SP_SYMB_IMG_SPACE_HALF
    '\n',                   //MMIIM_SP_SYMB_IMG_ENTER
    
#if MMIIM_SP_SYMBOL_SUPPORT_FULL
    0x3000,                 //MMIIM_SP_SYMB_IMG_SPACE
#endif
};

//常用标点符号
//,.?!:;"'~|\空格/@$回车%#&^*_-+=<>()[]{}`
#if 0//MS_MMI_Delete
static wchar const symb_en[] =
{
    0x002c, 0x002e, 0x003f, 0x0021, 0x003a, 0x003b, 0x0022, 0x0027,
    0x007e, 0x007c, 0x005c, MMIIM_SP_SYMB_IMG_SPACE_HALF, 0x002f, 0x0040, 0x0024, MMIIM_SP_SYMB_IMG_ENTER, 
    0x0025, 0x0023, 0x0026, 0x005e, 0x002a, 0x005f, 0x002d, 0x002b, 
    0x003d, 0x003c, 0x003e, 0x0028, 0x0029, 0x005b, 0x005d, 0x007b, 
    0x007d, 0x00bf, 0xa1,
};
#endif
static wchar const symb_en[] =
{
    0x002e,0x002c,0x0027,0x003f,0x0021,0x0022,0x002d, 
    0x0028,0x0029,0x0040,0x007E,0x003a,0x005f,0x003b,
    0x002b,0x0026,0x0025,0x002a,0x003d,0x003c,0x003e,
    0x00a3,0x20ac,0x0024,0x00a5,0x20b9,0x00a4,0x005b,
    0x005d,0x007b,0x007d,0x002F,0x005c,0x005e,0x0060,
    0x00a1,0x00bf,0x00a7,0x0023,0x007c,MMIIM_SP_SYMB_IMG_SPACE_HALF,0x21b5,
};

//add for symb
/*
Norwegian:                         GUIIM_LANG_NORWEGIAN   = 53
Polish:                                GUIIM_LANG_POLISH      = 54
Portuguese_BR:                     GUIIM_LANG_PORTUGUESE  = 55
Romanian:                          GUIIM_LANG_ROMANIAN    = 56
Russia:                               GUIIM_LANG_RUSSIAN     = 57
Sesotho:                             GUIIM_LANG_SESOTHO     = 59
Swahili:                              GUIIM_LANG_SWAHILI     = 63
Swedish:                            GUIIM_LANG_SWEDISH     = 64
Tagalog:                             GUIIM_LANG_TAGALOG     = 16
Turkish:                              GUIIM_LANG_TURKISH     = 65
Ukrainian:                           GUIIM_LANG_UKRAINIAN   = 66
Vietnamese:                        GUIIM_LANG_VIETNAMESE  = 67
Xhosa:                               GUIIM_LANG_XHOSA       = 68
Afrikaans:                          GUIIM_LANG_AFRIKAANS   = 21
Belarusian:白俄罗斯
Bulgarian:                          GUIIM_LANG_BULGARIAN   = 26,
Danish:                              GUIIM_LANG_DANISH      = 30,
Dutch:                               GUIIM_LANG_DUTCH       = 31,
English_US:
French_CA,French_FR:                 GUIIM_LANG_FRENCH      = 36,
Finnish:                               GUIIM_LANG_FINNISH     = 35,
Zulu:                                  GUIIM_LANG_ZULU
Malagasy
Lingala
Indonesian:                         GUIIM_LANG_INDONESIAN
German:                             GUIIM_LANG_GERMAN      = 38
Hungarian:                          GUIIM_LANG_HUNGARIAN   = 41
Icelandic:                            GUIIM_LANG_ICELANDIC   = 42
Italian:                                GUIIM_LANG_ITALIAN     = 46
Kazakh:                              GUIIM_LANG_KAZAKH      = 47
*/
static wchar const symb_norwegian[] =
{
    0x2e, 0x2c, 0x27, 0x3f, 0x21, 0x22, 0x2d, 0x28,
    0x29, 0x40, 0x2f, 0x3a, 0x5f, 0x3b, 0x2b, 0x26, 0x25,
    0x2a, 0x3d, 0x3c, 0x3e, 0xa3, 0x20ac, 0x24, 0xa5,
    0xa4, 0x5b, 0x5d, 0x7b, 0x7d, 0x5c, 0x7e, 0x5e,
    0x60, 0xa1, 0xbf, 0xa7, 0x23, 0x7c, 0x20, 0x21b5,
};
/*
Chinese_CN                          GUIIM_LANG_CHS        = 19
*/
static wchar const symb_chinese[] =
{
    0xFF0C, 0x3002, 0xFF1F, 0xFF01, 0xFF1A, 0xFF1B, 0x3001,
    0x2026, 0x201C, 0x201D, 0x2018, 0x2019, 0xFF08, 0xFF09,
    0xFF20, 0x300A, 0x300B, 0xFFE5, 0xFF04, 0xFFE1, 0x20ac,
    0xFF03, 0xFF05, 0xFF06, 0xFF0A, 0xFF0B, 0xFF0D, 0xFF3C,
    0xFF1C, 0xFF1D, 0xFF1E, 0xFF3F, 0x2F, 0xFF5E, 0x27, 0x60,
    0x20, 0x21b5,
};
/*
Thai :                                GUIIM_LANG_THAI        = 19
*/
static wchar const symb_thai[] =
{
    0x0e31,0x0e34,0x0e35,0x0e36,0x0e37,0x0e38,0x0e39,0x0e47,0x0e48,
    0x0e49,0x0e4a,0x0e4b,0x0e4c,0x0e2f,0x0e3f,0x0e46,0x0e24,0x0e26,
    MMIIM_SP_SYMB_SPACE_CHAR, 0x2e, 0x2c, 0x3f, 0x21, 0x27, 0x22, 0x2d,
    0x28, 0x29, 0x40, 0x2f, 0x3a, 0x5f, 0x3b, 0x2b,
    0x26, 0x25, 0x2a, 0x3d, 0x3c, 0x3e, 0xa3, 0x20ac,
    0x24, 0xa5, 0xa4, 0x5b, 0x5d, 0x7b, 0x7d, 0x5c,
    0x7e, 0x5e, 0xa1, 0xbf, 0xa7, 0x23, 0x7c, 0x21b5,
    0x0e45, 
};
static wchar const symb_thai_zero[] = 
{
   0x0E40, 0x0E41, 0x0E42, 0x0E43, 0x0E44, 0x0E30, 
   0x0E32, 0x0E33, 0x0E45, 0x0030, MMIIM_SP_SYMB_SPACE_CHAR, MMIIM_SP_SYMB_LINE_BREAK,
};
/*
Urdu:                                 GUIIM_LANG_URDU        = 20
Arabic:                               GUIIM_LANG_ARABIC      = 3,
Farsi:                                  GUIIM_LANG_PERSIAN
*/
static wchar const symb_urdu[] =
{
    0x2e, 0x061f,0x060c, 0x21, 0x22, 0x27, 0x2d, 0x28,
    0x29,0x40, 0x2f, 0x3a, 0x5f, 0x3b, 0x3f, 0x2b, 0x25,
    0x2a, 0x3d, 0x3c, 0x3e, 0xa3,0x20ac, 0x24, 0xa5, 0xa4, 
    0x5b, 0x5d, 0x7b, 0x7d, 0x5c, 0x7e, 0x5e, 0x60, 0xa1,
    0xbf, 0xa7, 0x23, 0x7c, 0x20,0x21b5,
};
/*
SA_Spanish/Spanish:          GUIIM_LANG_SPANISH     = 62
*/
static wchar const symb_spanish[]=
{
    0x2e, 0x2c, 0xbf, 0x3f, 0xa1, 0x21, 0xaa, 0xba,
    0x27, 0x22, 0x2d, 0x28, 0x29, 0x40, 0x2f, 0x3a,
    0x5f, 0x3b, 0x2b, 0x26, 0x25, 0x2a, 0x3d, 0x3c,
    0x3e, 0xa3, 0x20ac, 0x24, 0xa5, 0xa4, 0x5b, 0x5d,
    0x7b, 0x7d, 0x5c, 0x7e, 0x5e, 0x60, 0xa7, 0x23,
    0x7c, 0x20, 0x21b5,
};
/*
Amharic:                            GUIIM_LANG_AMHARIC     = 72,
*/
static wchar const symb_amharic[]=
{
    0x1362,0x1363, 0x1361, 0x1364, 0x1366, 0x1365, 0x1367,
    0x2e, 0x2c, 0x27, 0x3f, 0x21, 0x22, 0x2d, 
    0x28, 0x29, 0x40, 0x2f, 0x3a, 0x5f, 0x3b,
    0x2b, 0x26, 0x25, 0x2a, 0x3d, 0x3c, 0x3e,
    0xa3,0x20ac, 0x24, 0xa5, 0xa4, 0x5b, 0x5d, 
    0x7b, 0x7d, 0x5c, 0x7e, 0x5e, 0xa1, 0xbf,
    0xa7, 0x23,0x7c, 0x1369, 0x136a, 0x136b, 0x136c, 
    0x136d, 0x136e, 0x136f, 0x1370, 0x1371, 0x1372, 0x1373, 
    0x1374, 0x1375, 0x1376, 0x1377, 0x1378, 0x1379, 0x137a, 
    0x137b, 0x137c, 0x20,0x21b5,
};
/*
Bengali-BD:                          GUIIM_LANG_BENGALI     = 5,
0xE100=0x09AA,0x09CD,0x09B0
0xE101=0x0995,0x09CD,0x09B7
0xE102=0x09A8,0x09CD,0x09A4
0xE103=0x09A4,0x09CD,0x09B0
0xE104=0x09A8,0x09CD,0x09A6
0xE105=0x09B8,0x09CD,0x09A4
0xE106=0x09B8,0x09CD,0x09B9
0xE107=0x0997,0x09CD,0x09B0
0xE108=0x09B7,0x09CD,0x099F
0xE109=0x09B8,0x09CD,0x09AC
0xE110=0x0995,0x09CD,0x09A4
0xE111=0x0995,0x09CD,0x09B0
0xE112=0x09A4,0x09CD,0x09A4
0xE113=0x099A,0x09CD,0x099B
0xE114=0x09B8,0x09CD,0x09AA
0xE115=0x09A6,0x09CD,0x09B0
0xE116=0x09A6,0x09CD,0x09A7
0xE117=0x0995,0x09CD,0x0995
0xE118=0x0995,0x09CD,0x09AC
0xE119=0x0997,0x09CD,0x09A7
0xE120=0x0999,0x09CD,0x0995
0xE121=0x0999,0x09CD,0x0997
0xE122=0x099C,0x09CD,0x099D
0xE123=0x099E,0x09CD,0x099A
0xE124=0x099E,0x09CD,0x099B
0xE125=0x099E,0x09CD,0x099C
0xE126=0x099E,0x09CD,0x099D
0xE127=0x099F,0x09CD,0x099F
0xE128=0x09A3,0x09CD,0x09A0
0xE129=0x09A3,0x09CD,0x09A1
0xE130=0x09A4,0x09CD,0x09A5
0xE131=0x09A8,0x09CD,0x09A7
0xE132=0x09AC,0x09CD,0x09A7
0xE133=0x09AD,0x09CD,0x09B0
0xE134=0x09B7,0x09CD,0x09A3
0xE135=0x09B9,0x09CD,0x09A3
0xE136=0x09B9,0x09CD,0x09A8
0xE137=0x09B9,0x09CD,0x09AE
0xE138=0x09CD,0x200c
*/
static wchar const symb_bengali[]=
{   
    0x0964, 0x0965,0x2e, 
    0x2c,0x27,0x3f,0x21,0x22,0x2d,0x28, 
    0x29,0x40,0x2f,0x3a,0x5f,0x3b,0x2b,
    0x26,0x25,0x2a,0x3d,0x3c,0x3e,0xa3,
    0x20ac,0x24,0xa5,0xa4,0x5b,0x5d,0x7b, 
    0x7d,0x5c,0x7e,0x5e,0x60,0xa1,0xbf,
    0xa7,0x23, 0x7c, 0x20, 0x21b5,
    0xE100,0xE101,0xE102,0xE103,0xE104,0xE105,0xE106,
    0xE107,0xE108,0xE109,0xE10A,0xE10B,0xE10C,0xE10D,
    0xE10E,0xE10F,0xE110,0xE111,0xE112,0xE113,0xE114,
    0xE115,0xE116,0xE117,0xE118,0xE119,0xE11A,0xE11B,
    0xE11C,0xE11D,0xE11E,0xE11F,0xE120,0xE121,0xE122,
    0xE123,0xE124,0xE125,0xE126,
};
/*
English_CN                                  GUIIM_LANG_ENGLISH
*/
static wchar const symb_english_cn[]=
{
     0x2e,0x2c, 0x27, 0x3f, 0x21, 0x22, 0x2d, 0x28, 0x29, 0x40,
     0x2f, 0x3a, 0x5f, 0x3b, 0x2b, 0x26, 0x25, 0x2a, 0x3d, 0x3c,
     0x3e, 0xa3,0x20ac, 0x24, 0xa5,0x20b9, 0xa4, 0x5b, 0x5d, 0x7b,
     0x7d, 0x5c, 0x7e, 0x5e, 0x60, 0xa1, 0xbf, 0xa7, 0x23, 0x7c, 0x20,
     0x21b5,
};
static wchar const symb_hebrew[] = 
{
    0x2e,0x2c,0x21,0x22,0x27,0x2d,0x29,
    0x28,0x40,0x2f,0x3a,0x5f,0x3b,0x3f,
    0x2b,0x25,0x2a,0x3d,0x3e,0x3c,0xa3,
    0x20ac,0x24,0xa5,0xa4,0x5d,0x5b,0x7d,
    0x7b,0x5c,0x7e,0x5e,0x60,0xa1,0xbf,
    0xa7,0x23,0x7c,0x20,0x21b5,

};
/*
Yoruba:                                     GUIIM_LANG_YORUBA
*/
static wchar const symb_yoruba[]=
{
    0x2e, 0x2c, 0x27, 0x3f, 0x21, 0x22, 0x2d,
    0x28, 0x29, 0x40, 0x2f, 0x3a, 0x5f, 0x3b,
    0x2b, 0x26, 0x25, 0x2a, 0x3d, 0x3c, 0x3e,
    0x20a6, 0xa3, 0x20ac, 0x24, 0xa5, 0xa4, 0x5b,
    0x5d, 0x7b, 0x7d, 0x5c, 0x7e, 0x5e, 0x60,
    0xa1, 0xbf, 0xa7, 0x23, 0x7c, 0x20, 0x21b5,
};
/*
Assamese:                                GUIIM_LANG_ASSAMESE
0xE127=0x09AA,0x09CD,0x09F0
0xE128=0x09A8,0x09CD,0x09A4
0xE129=0x09A4,0x09CD,0x09F0
0xE12A=0x09A8,0x09CD,0x09A6
0xE12B=0x09B8,0x09CD,0x09A4
0xE12C=0x09B8,0x09CD,0x09B9
0xE12D=0x0997,0x09CD,0x09F0
0xE12E=0x09B7,0x09CD,0x099F
0xE12F=0x09B8,0x09CD,0x09AC
0xE130=0x0995,0x09CD,0x09A4
0xE131=0x0995,0x09CD,0x09F0
0xE132=0x09A4,0x09CD,0x09A4
0xE133=0x099A,0x09CD,0x099B
0xE134=0x09B8,0x09CD,0x09AA
0xE135=0x09A6,0x09CD,0x09F0
0xE136=0x09A6,0x09CD,0x09A7
0xE137=0x0995,0x09CD,0x0995
0xE138=0x0995,0x09CD,0x09AC
0xE139=0x0997,0x09CD,0x09A7
0xE13A=0x0999,0x09CD,0x0995
0xE13B=0x0999,0x09CD,0x0997
0xE13C=0x099C,0x09CD,0x099D
0xE13D=0x099E,0x09CD,0x099A
0xE13E=0x099E,0x09CD,0x099B
0xE13F=0x099E,0x09CD,0x099C
0xE140=0x099E,0x09CD,0x099D
0xE141=0x099F,0x09CD,0x099F
0xE142=0x09A3,0x09CD,0x09A0
0xE143=0x09A3,0x09CD,0x09A1
0xE144=0x09A4,0x09CD,0x09A5
0xE145=0x09A8,0x09CD,0x09A7
0xE146=0x09AC,0x09CD,0x09A7
0xE147=0x09AD,0x09CD,0x09F0
0xE148=0x09B7,0x09CD,0x09A3
0xE149=0x09B9,0x09CD,0x09A3
0xE14A=0x09B9,0x09CD,0x09A8
0xE14B=0x09B9,0x09CD,0x09AE
0xE14C=0x09CD,0x200C
*/
static wchar const symb_assamese[]=
{
    0x0964, 0x0965, 0x2e, 0x2c, 0x27, 
    0x3f, 0x21,0x22, 0x2d, 0x28, 0x29, 0x40, 0x2f, 0x3a,0x5f,
    0x3b, 0x2b, 0x26, 0x25, 0x2a, 0x3d,0x3c, 0x3e, 0x20b9,0xa3,
    0x20ac, 0x24, 0xa5,0xa4, 0x5b, 0x5d, 0x7b, 0x7d, 0x5c, 0x7e,
    0x5e, 0x60, 0xa1, 0xbf, 0xa7, 0x23, 0x7c,0x20, 0x21b5, 
    0xE127,0xE128,0xE129,0xE12A,0xE12B,0xE12C,0xE12D,
    0xE12E,0xE12F,0xE130,0xE131,0xE132,0xE133,0xE134,
    0xE135,0xE136,0xE137,0xE138,0xE139,0xE13A,0xE13B,
    0xE13C,0xE13D,0xE13E,0xE13F,0xE140,0xE141,0xE142,
    0xE143,0xE144,0xE145,0xE146,0xE147,0xE148,0xE149,
    0xE14A,0xE14B,0xE14C,
};
/*
Gujarati:                                GUIIM_LANG_GUJARATI
0xE14D=0x0A95,0x0ACD,0x0AB7 
0xE14E=0x0A9C,0x0ACD,0x0A9E 
0xE14F=0x0AA4,0x0ACD,0x0AA4 
0xE150=0x0AA6,0x0ACD,0x0AA6 
0xE151=0x0AA6,0x0ACD,0x0AA7 
0xE152=0x0AA6,0x0ACD,0x0AAE 
0xE153=0x0AA6,0x0ACD,0x0AAF 
0xE154=0x0AA6,0x0ACD,0x0AB0 
0xE155=0x0AA6,0x0ACD,0x0AB5 
0xE156=0x0AB6,0x0ACD,0x0AB0 
0xE157=0x0acd ,0x200c       
*/
static wchar const symb_gujarati[]=
{
    /*0xE077,0xE078,0xE079,0xE080,0xE081,0xE082,0xE083,0xE084,0xE085,0xE086,0xE087,*/
    0x0964, 0x0ad0, 0x0965, 0x2e, 0x2c, 0x27, 0x3f,0x21, 0x22,
    0x2d, 0x28, 0x29, 0x40, 0x2f,0x3a, 0x5f, 0x3b, 0x2b, 0x26,
    0x25, 0x2a,0x3d, 0x3c, 0x3e, 0x20b9, 0xa3, 0x20ac, 0x24,
    0xa5, 0xa4, 0x5b, 0x5d, 0x7b, 0x7d, 0x5c,0x7e, 0x5e, 0x60,
    0xa1, 0xbf, 0xa7, 0x23,0x7c, 0x20, 0x21b5,
    0xE14D,0xE14E,0xE14F,0xE150,0xE151,0xE152,0xE153,0xE154,0xE155,0xE156,0xE157,
};
/*
Kannada:                                GUIIM_LANG_KANNADA
0xE158=0x0c95,0x0ccd,0x0cb7
0xE159=0x0ccd,0x200c
*/
static wchar const symb_kannada[]=
{
    0x0964, 0x0965, 0x2e, 0x2c, 0x27, 0x3f, 0x21,
    0x22, 0x2d, 0x28, 0x29, 0x40, 0x2f, 0x3a,0x5f, 0x3b, 0x2b,
    0x26, 0x25, 0x2a, 0x3d,0x3c, 0x3e, 0x20b9, 0xa3, 0x20ac,
    0x24, 0xa5,0xa4, 0x5b, 0x5d, 0x7b, 0x7d, 0x5c, 0x7e,0x5e, 
    0x60, 0xa1, 0xbf, 0xa7, 0x23, 0x7c,0x20, 0x21b5, 
    0xE158,0xE159,
};
/*
Malayalam:                             GUIIM_LANG_MALAYALAM
0xE15A=0x0d19,0x0d4d,0x0d15
0xE15B=0x0d2e,0x0d4d,0x0d2a
0xE15C=0x0d31,0x0d4d,0x0d31
0xE15D=0x0d4d,0x200c
*/
static wchar const symb_malayalam[]=
{
    0x0964, 0x0965, 0x2e, 0x2c, 0x27, 0x3f,
    0x21,0x22, 0x2d, 0x28, 0x29, 0x40, 0x2f, 0x3a,0x5f, 0x3b, 0x2b,
    0x26, 0x25, 0x2a, 0x3d,0x3c, 0x3e, 0x20b9, 0xa3, 0x20ac, 0x24,
    0xa5,0xa4, 0x5b, 0x5d, 0x7b, 0x7d, 0x5c, 0x7e,0x5e, 0x60, 0xa1,
    0xbf, 0xa7, 0x23, 0x7c,0x20, 0x21b5,
    0xE15A,0xE15B,0xE15C,0xE15D,
};
/*
Oriya:                                     GUIIM_LANG_ORIYA
0xE15E=0x0b4d, 0x200c
*/
static wchar const symb_oriya[]=
{
    0x0964, 0x0965, 0x2e, 0x2c, 0x27, 0x3f, 0x21,0x22, 0x2d,
    0x28, 0x29, 0x40, 0x2f, 0x3a,0x5f, 0x3b, 0x2b, 0x26, 0x25, 0x2a,
    0x3d,0x3c, 0x3e, 0x20b9, 0xa3, 0x20ac, 0x24, 0xa5,0xa4, 0x5b,
    0x5d, 0x7b, 0x7d, 0x5c, 0x7e,0x5e, 0x60, 0xa1, 0xbf, 0xa7, 0x23, 0x7c,
    0x20, 0x21b5,
    0xE15E,
};
/*
Punjabi:                                    GUIIM_LANG_PUNJABI
*/
static wchar const symb_punjabi[]=
{
    0x0964, 0x0965, 0x2e, 0x2c, 0x27, 0x3f, 0x21,0x22, 0x2d, 0x28,
    0x29, 0x40, 0x2f, 0x3a,0x5f, 0x3b, 0x2b, 0x26, 0x25, 0x2a, 0x3d,
    0x3c, 0x3e, 0x20b9, 0xa3, 0x20ac, 0x24, 0xa5,0xa4, 0x5b, 0x5d,
    0x7b, 0x7d, 0x5c, 0x7e,0x5e, 0x60, 0xa1, 0xbf, 0xa7, 0x23, 0x7c,
    0x20, 0x21b5,
};
/*
Tamil:                                      GUIIM_LANG_TAMIL
*/
static wchar const symb_tamil[]=
{
    0x2e, 0x2c, 0x27, 0x3f, 0x21, 0x22, 0x2d,
    0x28, 0x29, 0x40, 0x2f, 0x3a, 0x5f, 0x3b,
    0x2b, 0x26, 0x25, 0x2a, 0x3d, 0x3c, 0x3e,
    0x20b9, 0xa3, 0x20ac, 0x24, 0xa5, 0xa4, 0x5b,
    0x5d, 0x7b, 0x7d, 0x5c, 0x7e, 0x5e, 0x60,
    0xa1, 0xbf, 0xa7, 0x23, 0x7c, 0x20, 0x21b5,
};
/*
Telugu:                                   GUIIM_LANG_TELUGU
0xE15F=0x0c4d, 0x200c
*/
static wchar const symb_telugu[]=
{
    0x0964, 0x0965, 0x2e, 0x2c, 0x27, 0x3f, 0x21,
    0x22, 0x2d, 0x28, 0x29, 0x40, 0x2f, 0x3a,
    0x5f, 0x3b, 0x2b, 0x26, 0x25, 0x2a, 0x3d,
    0x3c, 0x3e, 0x20b9, 0xa3, 0x20ac, 0x24, 0xa5,
    0xa4, 0x5b, 0x5d, 0x7b, 0x7d, 0x5c, 0x7e,
    0x5e, 0x60, 0xa1, 0xbf, 0xa7, 0x23, 0x7c,
    0x20, 0x21b5,
    0xE15F,
};
/*
Hausa:                                    GUIIM_LANG_HAUSA       = 40
*/
static wchar const symb_hausa[] =
{
    0x27,0x2e,0x2c,0x3f,0x21,0x22,0x2d,0x28,
    0x29,0x40,0x2f,0x3a,0x5f,0x3b,0x2b,0x26,
    0x25,0x2a,0x3d,0x3c,0x3e,0x20a6,0xa3,0x20ac,
    0x24,0xa5,0xa4,0x5b,0x5d,0x7b,0x7d,0x5c,
    0x7e,0x5e,0xa1,0xbf,0xa7,0x23,0x7c,0x20,0x21b5,
};
/*
Hindi:                                     GUIIM_LANG_HINDI       = 9
0xE160 = 0x0915,0x094d,0x0937, 
0xE161 = 0x0924,0x094d,0x0930, 
0xE162 = 0x091c,0x094d,0x091e, 
0xE163 = 0x094d,0x200c         
*/
static wchar const symb_hindi_1[] =
{
    0x0950,0x0964,0x0965,0x0970,
    0x093d,0x2e,0x2c,0x27,0x3f,0x21,0x22,0x2d,
    0x28, 0x29, 0x40, 0x2f,0x3a,0x5f,0x3b,0x2b,
    0x26,0x25,0x2a,0x3d,0x3c,0x3e,0x20b9,0xa3,
    0x20ac,0x24,0xa5,0xa4,0x5b,0x5d,0x7b,0x7d,
    0x5c,0x7e,0x5e,0x60,0xa1,0xbf,0xa7,0x23,
    0x7c,0x20,0x21b5,
    0xE160,0xE161,0xE162,0xE163,
};
/*
Igbo:                                        GUIIM_LANG_IGBO        = 43
*/
static wchar const symb_igbo[] =
{
    0x2e,0x2c,0x27,0x3f,0x21,0x22,0x2d,0x28,
    0x29,0x40,0x2f,0x3a,0x5f,0x3b,0x2b,0x26,
    0x25,0x2a,0x3d,0x3c,0x3e,0x20a6,0xa3,0x20ac,
    0x24,0xa5,0xa4,0x5b,0x5d,0x7b,0x7d,0x5c,
    0x7e,0x5e,0xa1,0xbf,0xa7,0x23,0x7c,0x20,0x21b5,
};
/*
Khmer:                                     GUIIM_LANG_KHMER       = 73
0xE164 =  0x1781,0x17d2,0x1789,0x17bb,0x17c6,
0xE165 = 0x17b2,0x17d2,0x1799, 
*/
static wchar const symb_khmer[] =
{
    0x17d4,0x17d7,0x17d6,0x17d5,0x21,0x3f,0x22,0x2e,0x2d,0x2c,0x27,
    0x28,0x29,0x40,0x2f,0x3a,0x5f,0x3b,0x2b,0x26,0x25,0x2a,
    0x3d,0x3c,0x3e,0xa3,0x20ac,0x24,0xa5,0xa4,0x5b,0x5d,0x7b,
    0x7d,0x5c,0x7e,0x5e,0x60,0xa1,0xbf,0xa7,0x23,0x7c,
    0x20,0x21b5,
    0xE164,0xE165,0x17db,0x179d,0x179e,0x17dc,0x17a8,
    //0xE165,0xE167,0xE168,0xE169,0xE16A,
};
/*
Marathi:                                     GUIIM_LANG_MARATHI     = 12
0xE166 = 0x0915,0x094D,0x0924,
0xE167 = 0x0924,0x094D,0x0924,
0xE168 = 0x0924,0x094D,0x0930,
0xE169 = 0x0926,0x094D,0x0926,
0xE16A = 0x0926,0x094D,0x0927,
0xE16B = 0x0926,0x094D,0x092F,
0xE16C = 0x0926,0x094D,0x0935,
0xE16D = 0x0928,0x094D,0x0928,
0xE16E = 0x0936,0x094D,0x0930,
0xE16F = 0x094D,0x200C         
*/
static wchar const symb_marathi[] =
{
    /*0xE102,0xE103,0xE104,0xE105,0xE106,0xE107,0xE108,0xE109,0xE110,0xE111,*/
    0x0950,0x0964,0x0965,0x0970,0x093d,0x2e,0x2c,
    0x27,0x3f,0x21,0x22,0x2d,0x28,0x29,0x40,0x2f,
    0x3a,0x5f,0x3b,0x2b,0x26,0x25,0x2a,0x3d,0x3c,
    0x3e,0x20b9,0xa3,0x20ac,0x24,0xa5,0xa4,0x5b,
    0x5d,0x7b,0x7d,0x5c,0x7e,0x5e,0x60,0xa1,0xbf,
    0xa7,0x23,0x7c,0x20,0x21b5,
    0xE166,0xE167,0xE168,0xE169,0xE16A,0xE16B,0xE16C,
    0xE16D,0xE16E,0xE16F,
};
//三个unicode的组合
static MMIIM_SP_SYMBOL_TABLE_DEF_T symb_combine_all = 
{
    //symb_bengali
    {0x09AA,0x09CD,0x09B0},/*E100*/
    {0x0995,0x09CD,0x09B7},
    {0x09A8,0x09CD,0x09A4},
    {0x09A4,0x09CD,0x09B0},
    {0x09A8,0x09CD,0x09A6},
    {0x09B8,0x09CD,0x09A4},
    {0x09B8,0x09CD,0x09B9},
    {0x0997,0x09CD,0x09B0},
    {0x09B7,0x09CD,0x099F},
    {0x09B8,0x09CD,0x09AC},
    {0x0995,0x09CD,0x09A4},
    {0x0995,0x09CD,0x09B0},
    {0x09A4,0x09CD,0x09A4},
    {0x099A,0x09CD,0x099B},
    {0x09B8,0x09CD,0x09AA},
    {0x09A6,0x09CD,0x09B0},
    {0x09A6,0x09CD,0x09A7},
    {0x0995,0x09CD,0x0995},
    {0x0995,0x09CD,0x09AC},
    {0x0997,0x09CD,0x09A7},
    {0x0999,0x09CD,0x0995},
    {0x0999,0x09CD,0x0997},
    {0x099C,0x09CD,0x099D},
    {0x099E,0x09CD,0x099A},
    {0x099E,0x09CD,0x099B},
    {0x099E,0x09CD,0x099C},
    {0x099E,0x09CD,0x099D},
    {0x099F,0x09CD,0x099F},
    {0x09A3,0x09CD,0x09A0},
    {0x09A3,0x09CD,0x09A1},
    {0x09A4,0x09CD,0x09A5},
    {0x09A8,0x09CD,0x09A7},
    {0x09AC,0x09CD,0x09A7},
    {0x09AD,0x09CD,0x09B0},
    {0x09B7,0x09CD,0x09A3},
    {0x09B9,0x09CD,0x09A3},
    {0x09B9,0x09CD,0x09A8},
    {0x09B9,0x09CD,0x09AE},
    {0x09CD,0x200c},/*E126*/
    //symb_assamese
    {0x09AA,0x09CD,0x09F0},/*E127*/
    {0x09A8,0x09CD,0x09A4},
    {0x09A4,0x09CD,0x09F0},
    {0x09A8,0x09CD,0x09A6},
    {0x09B8,0x09CD,0x09A4},
    {0x09B8,0x09CD,0x09B9},
    {0x0997,0x09CD,0x09F0},
    {0x09B7,0x09CD,0x099F},
    {0x09B8,0x09CD,0x09AC},
    {0x0995,0x09CD,0x09A4},
    {0x0995,0x09CD,0x09F0},
    {0x09A4,0x09CD,0x09A4},
    {0x099A,0x09CD,0x099B},
    {0x09B8,0x09CD,0x09AA},
    {0x09A6,0x09CD,0x09F0},
    {0x09A6,0x09CD,0x09A7},
    {0x0995,0x09CD,0x0995},
    {0x0995,0x09CD,0x09AC},
    {0x0997,0x09CD,0x09A7},
    {0x0999,0x09CD,0x0995},
    {0x0999,0x09CD,0x0997},
    {0x099C,0x09CD,0x099D},
    {0x099E,0x09CD,0x099A},
    {0x099E,0x09CD,0x099B},
    {0x099E,0x09CD,0x099C},
    {0x099E,0x09CD,0x099D},
    {0x099F,0x09CD,0x099F},
    {0x09A3,0x09CD,0x09A0},
    {0x09A3,0x09CD,0x09A1},
    {0x09A4,0x09CD,0x09A5},
    {0x09A8,0x09CD,0x09A7},
    {0x09AC,0x09CD,0x09A7},
    {0x09AD,0x09CD,0x09F0},
    {0x09B7,0x09CD,0x09A3},
    {0x09B9,0x09CD,0x09A3},
    {0x09B9,0x09CD,0x09A8},
    {0x09B9,0x09CD,0x09AE},
    {0x09CD,0x200C       },/*E14C*/
    //symb_gujarati
    {0x0A95,0x0ACD,0x0AB7},/*E14D*/
    {0x0A9C,0x0ACD,0x0A9E},
    {0x0AA4,0x0ACD,0x0AA4},
    {0x0AA6,0x0ACD,0x0AA6},
    {0x0AA6,0x0ACD,0x0AA7},
    {0x0AA6,0x0ACD,0x0AAE},
    {0x0AA6,0x0ACD,0x0AAF},
    {0x0AA6,0x0ACD,0x0AB0},
    {0x0AA6,0x0ACD,0x0AB5},
    {0x0AB6,0x0ACD,0x0AB0},
    {0x0acd ,0x200c      },/*E157*/
    //symb_kannada
    {0x0c95,0x0ccd,0x0cb7},/*E158*/
    {0x0ccd,0x200c},/*E159*/
    //symb_malayalam
    {0x0d19,0x0d4d,0x0d15},/*0xE15A*/
    {0x0d2e,0x0d4d,0x0d2a},
    {0x0d31,0x0d4d,0x0d31},
    {0x0d4d,0x200c       },/*0xE15D*/
    //symb_oriya
    {0x0b4d, 0x200c},/*0xE15E*/
    //symb_telugu
    {0x0c4d, 0x200c},/*0xE15F*/
    //symb_hindi_1
    {0x0915,0x094d,0x0937},/*0xE160*/
    {0x0924,0x094d,0x0930},
    {0x091c,0x094d,0x091e},
    {0x094d,0x200c       },/*0xE163*/
    //symb_khmer
    {0x1781,0x17d2,0x1789,0x17bb,0x17c6},/*0xE164 */
    {0x17b2,0x17d2,0x1799},/*0xE165*/
    //symb_marathi  
    { 0x0915,0x094D,0x0924},/*E166*/
    { 0x0924,0x094D,0x0924},
    { 0x0924,0x094D,0x0930},
    { 0x0926,0x094D,0x0926},
    { 0x0926,0x094D,0x0927},
    { 0x0926,0x094D,0x092F},
    { 0x0926,0x094D,0x0935},
    { 0x0928,0x094D,0x0928},
    { 0x0936,0x094D,0x0930},
    { 0x094D,0x200C       },/*0xE16f*/

};


//End add for symb

//sign
static wchar const symb_sign[] =
{
    MMIIM_SP_SYMB_SIGN_HTTP,
    MMIIM_SP_SYMB_SIGN_WWW,
    MMIIM_SP_SYMB_SIGN_COM,
    MMIIM_SP_SYMB_SIGN_NET,
    MMIIM_SP_SYMB_SIGN_ORG,
};

#if MMIIM_SP_SYMBOL_SUPPORT_FULL
//中文标点符号
//，。、；：？！% * # @ ”‘ ’｜…·ˉ（）《》〈〉～【】［］｛｝
static wchar const symb_ch[] =
{
    0xff0c, 0x3002, 0xff1f, 0xff01, 0xff1a, 0xff1b, 0x3001,
    0x2026, 0x201c, 0x201d, 0x2018, 0x2019, 0xff08, 0xff09,
    0xff20, 0x300a, 0x300b, 0xffe5, 0xff04, 0xffe1, 0x20ac,
    0xff03, 0xff05, 0xff06, 0xff0a, 0xff0b, 0xff0d, 0xff3c,
    0xff1c, 0xff1d, 0xff1e, 0xff3f, 0x002f, 0xff5e, 0x0027, 
    0x0060, MMIIM_SP_SYMB_IMG_SPACE,MMIIM_SP_SYMB_IMG_ENTER,
 #if 0   
    0xff0c, 0x3002, 0x3001, 0xff1b, 0xff1a, 0xff1f, 0xff01, 0x0025, 
    0x002a, 0x0023, 0x0040, 0x201c, 0x201d, 0x2018, MMIIM_SP_SYMB_IMG_SPACE, 0x2019, 
    0xff5c, 0x2026, MMIIM_SP_SYMB_IMG_ENTER,0x00b7, 0x02c9, 0xff08, 0xff09, 0x300a, 
    0x300b, 0x3008, 0x3009, 0xff5e, 0x3010, 0x3011, 0xff3b, 0xff3d, 
    0xff5b, 0xff5d,
#endif    
};

//特殊符号
//№☆★○●◎◇◆□■△▲→←↑↓〓
static wchar const symb_special[] =
{
    0x2116, 0x2606, 0x2605, 0x25cb, 0x25cf, 0x25ce, 0x25c7, 0x25c6,
    0x25a1, 0x25a0, 0x25b3, 0x25b2, 0x2192, 0x2190, 0x2191, 0x2193, 
    0x3013,
};

//数字序号
//ⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩⅪⅫ⒈⒉⒊⒋⒌⒍⒎⒏⒐⒑㈠㈡㈢㈣㈤㈥㈦㈧㈨㈩⑴⑵⑶⑷⑸⑹⑺⑻⑼⑽
static wchar const symb_num[] =
{
    0x2160, 0x2161, 0x2162, 0x2163, 0x2164, 0x2165, 0x2166, 0x2167,
    0x2168, 0x2169, 0x216a, 0x216b, 0x2488, 0x2489, 0x248a, 0x248b,
    0x248c, 0x248d, 0x248e, 0x248f, 0x2490, 0x2491, 0x3220, 0x3221,
    0x3222, 0x3223, 0x3224, 0x3225, 0x3226, 0x3227, 0x3228, 0x3229,
    0x2474, 0x2475, 0x2476, 0x2477, 0x2478, 0x2479, 0x247a, 0x247b,
    0x247c, 0x247d,
};

//数学符号
//≈≡≠＝≤≥＜＞≮≯∷±＋－×÷／∫∮∝∞∧∨∑∏∪∩∈∵∴⊥∥∠⌒⊙≌∽√
static wchar const symb_math[] =
{
    0x2248, 0x2261, 0x2260, 0xff1d, 0x2264, 0x2265, 0xff1c, 0xff1e,
    0x226e, 0x226f, 0x2237, 0x00b1, 0xff0b, 0xff0d, 0x00d7, 0x00f7,
    0xff0f, 0x222b, 0x222e, 0x221d, 0x221e, 0x2227, 0x2228, 0x2211,
    0x220f, 0x222a, 0x2229, 0x2208, 0x2235, 0x2234, 0x22a5, 0x2225,
    0x2220, 0x2312, 0x2299, 0x224c, 0x223d, 0x221a,
};

//单位符号
//°′″＄￡￥‰％℃¤￠
//〇一二三四五六七八九十百千万亿兆吉太拍艾分厘毫微零壹贰叁肆伍陆柒捌玖拾佰仟
static wchar const symb_unit[] =
{
    0x00b0, 0x2032, 0x2033, 0xff04, 0xffe1, 0xffe5, 0x2030, 0xff05,
    0x2103, 0x00a4, 0xffe0,
#if 0
    // 〇 会导致显示错误，而死机。
    0x3007, 0x4e00, 0x4e8c, 0x4e09, 0x56db,
    0x4e94, 0x516d, 0x4e03, 0x516b, 0x4e5d, 0x5341, 0x767e, 0x5343,
    0x4e07, 0x4ebf, 0x5146, 0x5409, 0x592a, 0x62cd, 0x827e, 0x5206,
    0x5398, 0x6beb, 0x5fae, 0x96f6, 0x58f9, 0x8d30, 0x53c1, 0x8086,
    0x4f0d, 0x9646, 0x67d2, 0x634c, 0x7396, 0x62fe, 0x4f70, 0x4edf,
#endif
};

//拼音符号
//āáǎàōóǒòêēéěèīíǐìūúǔùǖǘǚǜüa-z
static wchar const symb_pyin[] =
{
    0x0101, 0x00e1, 0x01ce, 0x00e0, 0x014d, 0x00f3, 0x01d2, 0x00f2,
    0x00ea, 0x0113, 0x00e9, 0x011b, 0x00e8, 0x012b, 0x00ed, 0x01d0,
    0x00ec, 0x016b, 0x00fa, 0x01d4, 0x00f9, 
            0x00fc, 0x0062, 0x0070, 0x006d, 0x0066, 0x0064, 0x0074,
    0x006e, 0x006c, 0x0067, 0x006b, 0x0068, 0x006a, 0x0071, 0x0078,
    0x0072, 0x007a, 0x0073, 0x0063, 0x0079, 0x0077,
};
#endif
//以arabic为例
/*
Urdu:                                 GUIIM_LANG_URDU        = 20
Arabic:                               GUIIM_LANG_ARABIC      = 3,
Farsi:                                  GUIIM_LANG_PERSIAN
*/
static wchar const symb_arabic[] = 
{
    0x2e,0x061f,0x060c,0x21,0x22,0x27,0x2d,
    0x28,0x29,0x40,0x2f,0x3a,0x5f,0x3b,
    0x3f,0x2b,0x25,0x2a,0x3d,0x3c,0x3e,
    0xa3,0x20ac,0x24,0xa5,0xa4,0x5b,0x5d,0x7b,
    0x7d,0x5c,0x7e,0x5e,0x60,0xa1,0xbf,
    0xa7,0x23,0x7c,0x20,0x21b5,
};

#if MMIIM_SP_SYMBOL_SUPPORT_HINDI
static wchar const symb_hindi[] =
{
    0x0950, 0x0964, 0x0965, 0x0970, 0x093d, 0x20a8, 0x20b9,
};
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_EUR
static wchar const symb_eur[] =
{
    0xa1, 0xa2, 0xa3,
};
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_FACE
//face
static wchar const symb_face[] =
{
    MMIIM_SP_SYMB_FACE_EM00,
    MMIIM_SP_SYMB_FACE_EM01,
    MMIIM_SP_SYMB_FACE_EM02,
    MMIIM_SP_SYMB_FACE_EM03,
    MMIIM_SP_SYMB_FACE_EM04,
    MMIIM_SP_SYMB_FACE_EM05,
    MMIIM_SP_SYMB_FACE_EM06,
    MMIIM_SP_SYMB_FACE_EM07,
    MMIIM_SP_SYMB_FACE_EM08,
    MMIIM_SP_SYMB_FACE_EM09,
    MMIIM_SP_SYMB_FACE_EM10,
    MMIIM_SP_SYMB_FACE_EM11,
    MMIIM_SP_SYMB_FACE_EM12,
    MMIIM_SP_SYMB_FACE_EM13,
    MMIIM_SP_SYMB_FACE_EM14,
    MMIIM_SP_SYMB_FACE_EM15,
    MMIIM_SP_SYMB_FACE_EM16,
    MMIIM_SP_SYMB_FACE_EM17,
    MMIIM_SP_SYMB_FACE_EM18,
    MMIIM_SP_SYMB_FACE_EM19,
    MMIIM_SP_SYMB_FACE_EM20,
    MMIIM_SP_SYMB_FACE_EM21,
    MMIIM_SP_SYMB_FACE_EM22,
    MMIIM_SP_SYMB_FACE_EM23,
    MMIIM_SP_SYMB_FACE_EM24,
    MMIIM_SP_SYMB_FACE_EM25,
    MMIIM_SP_SYMB_FACE_EM26,
    MMIIM_SP_SYMB_FACE_EM27,
    MMIIM_SP_SYMB_FACE_EM28,
    MMIIM_SP_SYMB_FACE_EM29,
    MMIIM_SP_SYMB_FACE_EM30,
    MMIIM_SP_SYMB_FACE_EM31,
    MMIIM_SP_SYMB_FACE_EM32,
    MMIIM_SP_SYMB_FACE_EM33,
    MMIIM_SP_SYMB_FACE_EM34,
    MMIIM_SP_SYMB_FACE_EM35,
    MMIIM_SP_SYMB_FACE_EM36,
    MMIIM_SP_SYMB_FACE_EM37,
    MMIIM_SP_SYMB_FACE_EM38,
    MMIIM_SP_SYMB_FACE_EM39,
    MMIIM_SP_SYMB_FACE_EM40,
    MMIIM_SP_SYMB_FACE_EM41,
    MMIIM_SP_SYMB_FACE_EM42,
    MMIIM_SP_SYMB_FACE_EM43,
    MMIIM_SP_SYMB_FACE_EM44,
    MMIIM_SP_SYMB_FACE_EM45,
    MMIIM_SP_SYMB_FACE_EM46,
    MMIIM_SP_SYMB_FACE_EM47,
    MMIIM_SP_SYMB_FACE_EM48,
    MMIIM_SP_SYMB_FACE_EM49,
    MMIIM_SP_SYMB_FACE_EM50,
    MMIIM_SP_SYMB_FACE_EM51,
    MMIIM_SP_SYMB_FACE_EM52,
    MMIIM_SP_SYMB_FACE_EM53,
    MMIIM_SP_SYMB_FACE_EM54,
    MMIIM_SP_SYMB_FACE_EM55,
    MMIIM_SP_SYMB_FACE_EM56,
    MMIIM_SP_SYMB_FACE_EM57,
    MMIIM_SP_SYMB_FACE_EM58,
    MMIIM_SP_SYMB_FACE_EM59,
    MMIIM_SP_SYMB_FACE_EM60,
    MMIIM_SP_SYMB_FACE_EM61,
    MMIIM_SP_SYMB_FACE_EM62,
    MMIIM_SP_SYMB_FACE_EM63,
};
#endif



//配置
static MMIIM_SP_SYMBOL_CFG_T const symb_cfg[] =
{
    {symb_en, ARR_SIZE(symb_en)},
    {symb_sign, ARR_SIZE(symb_sign)},
    //Begin add for symb
    //和MMIIM_SP_SYMBOL_TYPE_T符号顺序一致，以arabic为例
    {symb_arabic,ARR_SIZE(symb_arabic)},
    {symb_norwegian, ARR_SIZE(symb_norwegian)},
    {symb_chinese,ARR_SIZE(symb_chinese)},
    {symb_thai,ARR_SIZE(symb_thai)},
    {symb_spanish,ARR_SIZE(symb_spanish)},
    {symb_amharic,ARR_SIZE(symb_amharic)},
    {symb_bengali,ARR_SIZE(symb_bengali)},
    {symb_english_cn,ARR_SIZE(symb_english_cn)},
    {symb_yoruba,ARR_SIZE(symb_yoruba)},
    {symb_assamese,ARR_SIZE(symb_assamese)},
    {symb_gujarati,ARR_SIZE(symb_gujarati)},
    {symb_kannada,ARR_SIZE(symb_kannada)},
    {symb_malayalam,ARR_SIZE(symb_malayalam)},
    {symb_oriya,ARR_SIZE(symb_oriya)},
    {symb_punjabi,ARR_SIZE(symb_punjabi)},
    {symb_tamil,ARR_SIZE(symb_tamil)},
    {symb_telugu,ARR_SIZE(symb_telugu)},
    {symb_hausa,ARR_SIZE(symb_hausa)},
    {symb_hindi_1,ARR_SIZE(symb_hindi_1)},
    {symb_igbo,ARR_SIZE(symb_igbo)},
    {symb_khmer,ARR_SIZE(symb_khmer)},
    {symb_marathi,ARR_SIZE(symb_marathi)},
    {symb_hebrew,ARR_SIZE(symb_hebrew)},
    {symb_thai_zero,ARR_SIZE(symb_thai_zero)},
    //End add for symb
#if MMIIM_SP_SYMBOL_SUPPORT_FULL
    {symb_ch, ARR_SIZE(symb_ch)},
    {symb_special, ARR_SIZE(symb_special)},
    {symb_num, ARR_SIZE(symb_num)},
    {symb_math, ARR_SIZE(symb_math)},
    {symb_unit, ARR_SIZE(symb_unit)},
    {symb_pyin, ARR_SIZE(symb_pyin)},
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_FACE
    {symb_face, ARR_SIZE(symb_face)},
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_HINDI
    {symb_hindi, ARR_SIZE(symb_hindi)},
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_EUR
    {symb_eur, ARR_SIZE(symb_eur)},
#endif
};


/*==============================================================================
Function Name: MMIIM_SP_SYMB_GetSymbols
Description: 按照类型获取符号列表，返回指定数量之内的符号
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
    type - symbol type
    *cnt_ptr - [in\out]want get symbol count and the get count
    *symb_ptr - [out]get symbols
    return - get count
==============================================================================*/
int32 MMIIM_SP_SYMB_GetSymbols(
    MMIIM_SP_SYMBOL_TYPE_T type,
    uint32 *cnt_ptr,
    wchar *symb_ptr
)
{
    // int32 is_error_param = 0;
    wchar const *symb_list_ptr = NULL;
    uint32 symb_cnt = 0;
    // int32 symb_left = 0;

    if (NULL == cnt_ptr) return 0;

    if (NULL == symb_ptr) return 0;

    if (type >= ARR_SIZE(symb_cfg)) return 0;

    if (*cnt_ptr == 0) return 0;

    symb_list_ptr = symb_cfg[type].symb_list;

    if (PNULL == symb_list_ptr) return 0;

    symb_cnt = symb_cfg[type].symb_cnt;

    if (symb_cnt > *cnt_ptr) symb_cnt = *cnt_ptr;

    SCI_MEMCPY(symb_ptr, symb_list_ptr, symb_cnt * sizeof(wchar));
    *cnt_ptr = symb_cnt;

    return symb_cnt;
}


/*==============================================================================
Function Name: MMIIM_SP_SYMB_GetSymbStrPtr
Description: 获取字符串的指针
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 宽字符，0结尾
==============================================================================*/
wchar const * MMIIM_SP_SYMB_GetSymbStrPtr(wchar symb)
{
    wchar const *symb_str_ptr = NULL;

    if (MMIIM_SP_SYMB_SIGN_START <= symb && symb < MMIIM_SP_SYMB_SIGN_MAX)
    {
        symb_str_ptr = symb_sign_str[symb - MMIIM_SP_SYMB_SIGN_START];
    }
#if MMIIM_SP_SYMBOL_SUPPORT_FACE
    else if (MMIIM_SP_SYMB_FACE_START <= symb && symb < MMIIM_SP_SYMB_FACE_MAX)
    {
        symb_str_ptr = symb_face_str[symb - MMIIM_SP_SYMB_FACE_START];
    }
#endif
    else if (MMIIM_SP_SYMB_IMG_START <= symb && symb < MMIIM_SP_SYMB_IMG_MAX)
    {
        symb_str_ptr = &symb_img[symb - MMIIM_SP_SYMB_IMG_START];
    }
#if 1 //三个unicode的组合
    else if (MMIIM_SP_SYMB_COMBINE_START <= symb && symb< MMIIM_SP_SYMB_COMBINE_END)
    {
        symb_str_ptr = symb_combine_all[symb - MMIIM_SP_SYMB_COMBINE_START];
    }
    else if( MMIIM_SP_SYMB_LINE_BREAK == symb)
    {
        symb_str_ptr = &symb_line_break;
    }
    else if( MMIIM_SP_SYMB_SPACE_CHAR == symb)
    {
        symb_str_ptr = &symb_space_char;
    }
#endif
    return symb_str_ptr;
}





