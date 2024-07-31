/*lint -save -e91*/
/*lint -save -e774*/
#include "wd_def.h"
#include "sg_common_public.h"


#include "os_api.h"     //Ê¹ÓÃsci_memcpy´úÌæÔ­memcpy£¬ÒÔÃâpclint¸æ¾¯


extern void copy_languageInfo_2_inner(void* languageInfo,WD_UINT8 imecount);

	/******************************* ²Ù×÷Êý¾Ý±í ********************************/
#if defined(DIGIT_KEYS)

const WD_UINT16 caoZuoShuJuBiao[9][MAX_KEY_COUNT-KEY_BASE]={
/* ======¼üÂë======          0      1      2      3      4      5      6      7      8      9      *       #     C     OK     LSK    RSK     UP   DOWN   LEFT  RIGHT     L0    L1     L2     L3     L4      L5     L6    L7     L8     L9      L*    L#     LC     LOK   LLSK   LRSK   LUP    LDOWN  LLEFT LRIGHT  */
/*WDS_SYMBOL_TABLE     */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x004c,0x005c,0xfffe,0x0046,0x005d,0x005c,0x0049,0x004a,0x0041,0x0042,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0080,0x0056,0x0077,0x0046,0x005d,0x005c,0x0049,0x004a,0x0041,0x0042,},
/*WDS_SYMBOL_GROUP     */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x004c,0x0056,0xfffe,0x0045,0x005d,0x005c,0x0047,0x0048,0x0041,0x0042,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0xffff,0x0056,0x0077,0x0045,0xffff,0xffff,0x0047,0x0048,0x0041,0x0042,},
/*WDS_DIGIT_INPUT      */ {0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,0x0018,0x0019,0x0054,0x005c,0xfffe,0xfffe,0x0054,0x005c,0xfffe,0xfffe,0xfffe,0xfffe,0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,0x0018,0x0019,0x0053,0x0056,0xfffe,0xfffe,0x005d,0x005c,0xfffe,0xfffe,0xfffe,0xfffe,},
/*WDS_IME_CHANGE       */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x005a,0x005b,0x0077,0x0057,0xffff,0xffff,0x0058,0x0059,0x005a,0x005b,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x005a,0x005b,0x0077,0x0057,0xffff,0xffff,0x0058,0x0059,0x005a,0x005b,},
/*WDS_HELP             */ {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0077,0x0067,0xffff,0xffff,0x0065,0x0066,0x0095,0x0096,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0077,0x0067,0xffff,0xffff,0x0065,0x0066,0x0095,0x0096,},
/*WDS_CONFIG           */ {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005f,0x005e,0xffff,0xffff,0x0060,0x0061,0x0062,0x0063,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005f,0x005e,0xffff,0xffff,0x0060,0x0061,0x0062,0x0063,},
/*WDS_USERDICT_MANAGER */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0xffff,0xffff,0x0082,0x0070,0xffff,0xffff,0x0071,0x0072,0x0073,0x0074,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0082,0x0070,0xffff,0xffff,0x0071,0x0072,0x0073,0x0074,},
/*WDS_HOTKEY_SYMBOL    */ {0x009a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0041,0x0042,0x0077,0x0046,0x005d,0x005c,0x0049,0x004a,0x0041,0x0042,0x009a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0041,0x0042,0x0077,0x0046,0x005d,0x005c,0x0049,0x004a,0x0041,0x0042,},
/*WDS_HOTKEY_FACE      */ {0xffff,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x004c,0x0056,0x0032,0x0046,0xffff,0xffff,0x0049,0x004a,0x0041,0x0042,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0080,0xffff,0xffff,0xffff,0xffff,0xffff,0x0049,0x004a,0x0041,0x0042,},
};

#elif defined(QWERT_KEYS)

const WD_UINT16 caoZuoShuJuBiao[9][MAX_KEY_COUNT-KEY_BASE]={
/*======¼üÂë======           A      B      C      D      E      F      G      H      I      J      K      L      M      N      O      P      Q      R      S      T      U      V      W      X      Y      Z    ÇÐ»»   ·Ö¸ô   ·ûºÅ   É¾³ý    OK     ¿Õ¸ñ   »Ø³µ   UP    DOWN   LEFT   RIGHT   LST    RST    ´óÐ¡   FN    Shift   Alt   Ctrl    LA     LB     LC     LD     LE     LF     LG     LH     LI     LJ     LK     LL     LM     LN     LO     LP     LQ     LR     LS     LT     LU     LV     LW     LX     LY     LZ    LÇÐ»» L·Ö¸ô   L·ûºÅ LÉ¾³ý   LOK    L¿Õ¸ñ  L»Ø³µ   LUP   LDOWN  LLEFT LRIGHT  LLST   LRST   L´óÐ¡  LFN    LShift  LAlt  LCtrl  */
/*WDS_SYMBOL_TABLE     */ {0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0x002a,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0x005c,0xffff,0x004c,0xfffe,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0x002a,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0x005c,0xffff,0x0080,0x0077,0x0046,0xfffe,0xfffe,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},
/*WDS_SYMBOL_GROUP     */ {0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0x002a,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0x005c,0xffff,0x004c,0xfffe,0x0045,0x0045,0x0045,0x0047,0x0048,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0x002a,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0x005c,0xffff,0x0080,0x0077,0x0045,0xfffe,0xfffe,0x0047,0x0048,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},
/*WDS_DIGIT_INPUT      */ {0xffff,0x0018,0xffff,0xffff,0xffff,0x0014,0x0015,0x0016,0xffff,0x001b,0xffff,0xffff,0x0010,0x0019,0xffff,0xffff,0xffff,0x0011,0xffff,0x0012,0x001a,0x0017,0xffff,0xffff,0x0013,0xffff,0x005c,0xffff,0x0054,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xffff,0xfffe,0xfffe,0xfffe,0xfffe,0xffff,0x0018,0xffff,0xffff,0xffff,0x0014,0x0015,0x0016,0xffff,0x001b,0xffff,0xffff,0x0010,0x0019,0xffff,0xffff,0xffff,0x0011,0xffff,0x0012,0x001a,0x0017,0xffff,0xffff,0x0013,0xffff,0x0056,0xffff,0x0053,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xffff,0xfffe,0xfffe,0xfffe,0xfffe,},
/*WDS_IME_CHANGE       */ {0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0x002a,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0x005b,0xffff,0xffff,0x0077,0x0057,0x0057,0x0057,0x0058,0x0059,0x005a,0x005b,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0x002a,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0x005b,0xffff,0xffff,0x0077,0x0057,0xffff,0xffff,0x0058,0x0059,0x005a,0x005b,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},
/*WDS_HELP             */ {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0077,0x0067,0x0067,0x0067,0x0065,0x0066,0x0095,0x0096,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0077,0x0067,0xffff,0xffff,0x0065,0x0066,0x0095,0x0096,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},
/*WDS_CONFIG           */ {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005f,0x005e,0x005e,0x005e,0x0060,0x0061,0x0062,0x0063,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005f,0x005e,0xffff,0xffff,0x0060,0x0061,0x0062,0x0063,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},
/*WDS_USERDICT_MANAGER */ {0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0x002a,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0xffff,0xffff,0xffff,0x0082,0x0070,0x0070,0x0070,0x0071,0x0072,0x0073,0x0074,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0x002a,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0xffff,0xffff,0xffff,0x0082,0x0070,0xffff,0xffff,0x0071,0x0072,0x0073,0x0074,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},
/*WDS_HOTKEY_SYMBOL    */ {0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0xffff,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0x005c,0xffff,0xffff,0x0077,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0xffff,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0x005c,0xffff,0xffff,0x0077,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},
/*WDS_HOTKEY_FACE      */ {0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0x002a,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0x005c,0xffff,0xffff,0x0032,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0028,0xffff,0xffff,0xffff,0x0024,0x0025,0x0026,0xffff,0xffff,0xffff,0xffff,0x002a,0x0029,0xffff,0xffff,0xffff,0x0021,0xffff,0x0022,0xffff,0x0027,0xffff,0xffff,0x0023,0xffff,0x005c,0xffff,0xffff,0x0032,0x0046,0xffff,0xffff,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},
};

#elif defined(INTEVQWERTY)//OTHER_KEYSÎïÀí26¼ü+ÐéÄâ9¼ü

const WD_UINT16 caoZuoShuJuBiao[9][MAX_KEY_COUNT-KEY_BASE]={
	/* ======¼üÂë======          0      1      2      3      4      5      6      7      8      9      *      #      A      B      C      D      E      F      G      H      I      J      K      L      M      N      O      P      Q      R      S      T      U      V      W      X      Y      Z    ÇÐ»»   ·Ö¸ô   ·ûºÅ   É¾³ý    OK     ¿Õ¸ñ   »Ø³µ   UP    DOWN   LEFT   RIGHT   LST    RST    ´óÐ¡   FN   Shift   Alt     Ctrl    L0     L1     L2     L3     L4     L5     L6     L7     L8     L9     L*     L#     LA     LB     LC     LD     LE     LF     LG     LH     LI     LJ     LK     LL     LM     LN     LO     LP     LQ     LR     LS     LT     LU     LV     LW     LX     LY     LZ   LÇÐ»»  L·Ö¸ô   L·ûºÅ  LÉ¾³ý   LOK   L¿Õ¸ñ  L»Ø³µ   LUP  LDOWN   LLEFT LRIGHT  LLST   LRST   L´óÐ¡  LFN   LShift  LAlt   LCtrl  */
	/*WDS_SYMBOL_TABLE     */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x004c,0x005c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005c,0xffff,0x004c,0xfffe,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0080,0x0056,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005c,0xffff,0x0080,0x0077,0x0046,0xfffe,0xfffe,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_SYMBOL_GROUP     */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x004c,0x0056,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005c,0xffff,0x004c,0xfffe,0x0045,0x0045,0x0045,0x0047,0x0048,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0xffff,0x0056,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005c,0xffff,0x0080,0x0077,0x0045,0xfffe,0xfffe,0x0047,0x0048,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_DIGIT_INPUT      */ {0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,0x0018,0x0019,0x0054,0x005c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005c,0xffff,0x0054,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xffff,0xfffe,0xfffe,0xfffe,0xfffe,0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,0x0018,0x0019,0x0053,0x0056,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0056,0xffff,0x0053,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xffff,0xfffe,0xfffe,0xfffe,0xfffe,},                                                     
	/*WDS_IME_CHANGE       */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x005a,0x005b,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005b,0xffff,0xffff,0x0077,0x0057,0x0057,0x0057,0x0058,0x0059,0x005a,0x005b,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x005a,0x005b,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005b,0xffff,0xffff,0x0077,0x0057,0xffff,0xffff,0x0058,0x0059,0x005a,0x005b,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_HELP             */ {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0077,0x0067,0x0067,0x0067,0x0065,0x0066,0x0095,0x0096,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0077,0x0067,0xffff,0xffff,0x0065,0x0066,0x0095,0x0096,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_CONFIG           */ {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005f,0x005e,0x005e,0x005e,0x0060,0x0061,0x0062,0x0063,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005f,0x005e,0xffff,0xffff,0x0060,0x0061,0x0062,0x0063,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_USERDICT_MANAGER */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0082,0x0070,0x0070,0x0070,0x0071,0x0072,0x0073,0x0074,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0082,0x0070,0xffff,0xffff,0x0071,0x0072,0x0073,0x0074,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_HOTKEY_SYMBOL    */ {0x009a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0049,0x004a,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005c,0xffff,0xffff,0x0077,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x009a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0049,0x004a,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005c,0xffff,0xffff,0x0077,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_HOTKEY_FACE      */ {0xffff,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x004c,0x0056,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005c,0xffff,0xffff,0x0032,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0080,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005c,0xffff,0xffff,0x0032,0x0046,0xffff,0xffff,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
};
#else

const WD_UINT16 caoZuoShuJuBiao[9][MAX_KEY_COUNT-KEY_BASE]={
	/* ======¼üÂë======          0      1      2      3      4      5      6      7      8      9      *      #      A      B      C      D      E      F      G      H      I      J      K      L      M      N      O      P      Q      R      S      T      U      V      W      X      Y      Z    ÇÐ»»   ·Ö¸ô   ·ûºÅ   É¾³ý    OK     ¿Õ¸ñ   »Ø³µ   UP    DOWN   LEFT   RIGHT   LST    RST    ´óÐ¡   FN   Shift   Alt     Ctrl    L0     L1     L2     L3     L4     L5     L6     L7     L8     L9     L*     L#     LA     LB     LC     LD     LE     LF     LG     LH     LI     LJ     LK     LL     LM     LN     LO     LP     LQ     LR     LS     LT     LU     LV     LW     LX     LY     LZ   LÇÐ»»  L·Ö¸ô   L·ûºÅ  LÉ¾³ý   LOK   L¿Õ¸ñ  L»Ø³µ   LUP  LDOWN   LLEFT LRIGHT  LLST   LRST   L´óÐ¡  LFN   LShift  LAlt   LCtrl  */
	/*WDS_SYMBOL_TABLE     */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x004c,0x005c,0xffff,0xffff,0x0029,0x0025,0x0022,0x0026,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0023,0x0024,0xffff,0xffff,0xffff,0x0021,0x0028,0xffff,0x0027,0x005c,0xffff,0x004c,0xfffe,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0080,0x0056,0xffff,0xffff,0x0029,0x0025,0x0022,0x0026,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0023,0x0024,0xffff,0xffff,0xffff,0x0021,0x0028,0xffff,0x0027,0x005c,0xffff,0x0080,0x0077,0x0046,0xfffe,0xfffe,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_SYMBOL_GROUP     */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x004c,0x0056,0xffff,0xffff,0x0029,0x0025,0x0022,0x0026,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0023,0x0024,0xffff,0xffff,0xffff,0x0021,0x0028,0xffff,0x0027,0x005c,0xffff,0x004c,0xfffe,0x0045,0x0045,0x0045,0x0047,0x0048,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0xffff,0x0056,0xffff,0xffff,0x0029,0x0025,0x0022,0x0026,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0023,0x0024,0xffff,0xffff,0xffff,0x0021,0x0028,0xffff,0x0027,0x005c,0xffff,0x0080,0x0077,0x0045,0xfffe,0xfffe,0x0047,0x0048,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_DIGIT_INPUT      */ {0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,0x0018,0x0019,0x0054,0x005c,0xffff,0xffff,0x0029,0x0025,0x0022,0x0026,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0023,0x0024,0xffff,0xffff,0xffff,0x0021,0x0028,0xffff,0x0027,0x005c,0xffff,0x0054,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xffff,0xfffe,0xfffe,0xfffe,0xfffe,0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,0x0018,0x0019,0x0053,0x0056,0xffff,0xffff,0x0029,0x0025,0x0022,0x0026,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0023,0x0024,0xffff,0xffff,0xffff,0x0021,0x0028,0xffff,0x0027,0x0056,0xffff,0x0053,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xfffe,0xffff,0xfffe,0xfffe,0xfffe,0xfffe,},                                                     
	/*WDS_IME_CHANGE       */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x005a,0x005b,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005b,0xffff,0xffff,0x0077,0x0057,0x0057,0x0057,0x0058,0x0059,0x005a,0x005b,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x005a,0x005b,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005b,0xffff,0xffff,0x0077,0x0057,0xffff,0xffff,0x0058,0x0059,0x005a,0x005b,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_HELP             */ {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0077,0x0067,0x0067,0x0067,0x0065,0x0066,0x0095,0x0096,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0077,0x0067,0xffff,0xffff,0x0065,0x0066,0x0095,0x0096,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_CONFIG           */ {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005f,0x005e,0x005e,0x005e,0x0060,0x0061,0x0062,0x0063,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005f,0x005e,0xffff,0xffff,0x0060,0x0061,0x0062,0x0063,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_USERDICT_MANAGER */ {0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0082,0x0070,0x0070,0x0070,0x0071,0x0072,0x0073,0x0074,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0082,0x0070,0xffff,0xffff,0x0071,0x0072,0x0073,0x0074,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_HOTKEY_SYMBOL    */ {0x009a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0049,0x004a,0xffff,0xffff,0x0029,0x0025,0x0022,0x0026,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0023,0x0024,0xffff,0xffff,0xffff,0x0021,0x0028,0xffff,0x0027,0x005c,0xffff,0xffff,0x0077,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x009a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0049,0x004a,0xffff,0xffff,0x0029,0x0025,0x0022,0x0026,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0023,0x0024,0xffff,0xffff,0xffff,0x0021,0x0028,0xffff,0x0027,0x005c,0xffff,0xffff,0x0077,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
	/*WDS_HOTKEY_FACE      */ {0xffff,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x004c,0x0056,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005c,0xffff,0xffff,0x0032,0x0046,0x0046,0x0046,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x002a,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x0080,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x005c,0xffff,0xffff,0x0032,0x0046,0xffff,0xffff,0x0049,0x004a,0x0041,0x0042,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,},                                                     
};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         

#endif

///////////////º¯ÊýÉùÃ÷/////////////////////////
void cpy_data_2_data(WD_UINT8 i, lan_keymap_Info_t* data, void* Ime_data);
/////////////////º¯ÊýÉùÃ÷½áÊø/////////////////////////


///////////////±àÂë·½Ê½//////////////////
#define USE_UNICODE			//Èç¹û×¢ÊÍµô±¾ÐÐ£¬ÔòÎªGBKÂë£¬´ò¿ªÎªUNICODE
////////////////////ÓïÑÔ¿ª¹Ø////////////////////////

/////¿Í»§Ê¹ÓÃ/////////////////////////

	#define WD_DIGIT			//Êý×ÖÊäÈë·¨
////ÖÐÎÄ
	#define WD_CN_CPINYIN			//´¿Æ´ÊäÈë·¨
    #define WD_CN_FULLPINYIN		//È«×ÖÄ¸Æ´ÒôÊäÈë·¨
	#define WD_CN_BIHUA				//±Ê»­ÊäÈë·¨

#ifdef IM_TRAD_CHINESE_SUPPORT	
 	#define WD_CN_BIHUAF			//·±Ìå±Ê»­ÊäÈë·¨
 	#define	WD_CN_ZHUYIN			//×¢ÒôÊäÈë·¨
 	#define WD_CN_FULLZHUYIN		//È«×ÖÄ¸¼ü×¢Òô
#endif

//ÍâÎÄ
	//#define WD_WDI_ABC
	#define WD_ENGLISH_ABC				//Ó¢ÎÄ×ÖÄ¸ÊäÈë·¨
// 	#define	WD_LATIN_ABC				//À­¶¡ÓïÏµµÄ×ÖÄ¸ÊäÈë·¨
#ifdef IM_ARABIC_SUPPORT
 	#define WD_ARABIC_ABC				//°¢À­²®ÎÄ×ÖÄ¸ÊäÈë·¨
#endif 	

#ifdef IM_RUSSIAN_SUPPORT
 	#define WD_RUSSIAN_ABC				//¶íÓï×ÖÄ¸ÊäÈë·¨
#endif

#ifdef IM_FRENCH_SUPPORT
 	#define WD_FRENCH_ABC				//·¨ÎÄ×ÖÄ¸ÊäÈë·¨
#endif

#ifdef IM_VIETNAMESE_SUPPORT
 	#define WD_VIETNAMESE_ABC			//Ô½ÄÏÓï×ÖÄ¸ÊäÈë·¨
#endif 	

#ifdef IM_THAI_SUPPORT
 	#define WD_THAI_ABC					//Ì©ÎÄ×ÖÄ¸ÊäÈë·¨
#endif 	

#ifdef IM_INDONESIAN_SUPPORT
 	#define WD_INDONESIAN_ABC			//Ó¡¶ÈÄáÎ÷ÑÇ×ÖÄ¸×ÖÄ¸ÊäÈë·¨
#endif 	

#ifdef IM_PORTUGUESE_SUPPORT
 	#define WD_PORTUGUESE_ABC			//ÆÏÌÑÑÀÎÄ×ÖÄ¸ÊäÈë·¨
#endif 	

#ifdef IM_GERMAN_SUPPORT
 	#define WD_GERMAN_ABC				//µÂÎÄ×ÖÄ¸ÊäÈë·¨
#endif 	

#ifdef IM_SPANISH_SUPPORT
 	#define WD_SPANISH_ABC				//Î÷°àÑÀÎÄ×ÖÄ¸ÊäÈë·¨
#endif 	

#ifdef IM_MALAY_SUPPORT
 	#define WD_MALAY_ABC				//ÂíÀ´Óï×ÖÄ¸ÊäÈë·¨
#endif 	

#ifdef IM_PERSIAN_SUPPORT
 	#define WD_PERSIAN_ABC				//²¨Ë¹Óï×ÖÄ¸ÊäÈë·¨
#endif 	

#ifdef IM_TURKISH_SUPPORT
 	#define WD_TURKISH_ABC				//ÍÁ¶úÆäÓï×ÖÄ¸ÊäÈë·¨
#endif 	

#ifdef IM_ITALIAN_SUPPORT
 	#define WD_ITALIAN_ABC				//Òâ´óÀûÓï×ÖÄ¸ÊäÈë·¨
#endif 	

#ifdef IM_HINDI_SUPPORT
 	#define WD_HINDI_ABC				//ÖÇÄÜÓ¡µØÓï×ÖÄ¸ÊäÈë·¨
#endif

#ifdef IM_CZECH_SUPPORT
 	#define WD_CZECH_ABC				//×ÖÄ¸½Ý¿ËÓï
#endif 	

#ifdef IM_DUTCH_SUPPORT
 	#define WD_DUTCH_ABC				//×ÖÄ¸ºÉÀ¼Óï
#endif 	

#ifdef IM_HEBREW_SUPPORT
 	#define WD_HEBREW_ABC				//×ÖÄ¸Ï£²®À´Óï
#endif 	

#ifdef IM_POLISH_SUPPORT
 	#define WD_POLISH_ABC				//×ÖÄ¸²¨À¼Óï
#endif 	

#ifdef IM_UYGHUR_SUPPORT
 	#define WD_UIGHUR_ABC				//×ÖÄ¸Î¬Îá¶ûÓï
#endif 	

#ifdef IM_SWEDISH_SUPPORT
 	#define WD_SWEDISH_ABC				//×ÖÄ¸ÈðµäÓï
#endif 	

#ifdef IM_FINNISH_SUPPORT
 	#define WD_FINNISH_ABC				//×ÖÄ¸·ÒÀ¼Óï
#endif 	

#ifdef IM_NORWEGIAN_SUPPORT
 	#define WD_NORWEGIAN_ABC			//×ÖÄ¸Å²ÍþÓï
#endif 	

#ifdef IM_DANISH_SUPPORT
 	#define WD_DANISH_ABC				//×ÖÄ¸µ¤ÂóÓï
#endif 	

#ifdef IM_LAO_SUPPORT
 	#define WD_LAO_ABC					//×ÖÄ¸ÀÏÎÎÓï
#endif 	

#ifdef IM_URDU_SUPPORT
 	#define WD_URDU_ABC					//×ÖÄ¸ÎÚ¶û¶¼Óï
#endif 	

#ifdef IM_KAZAK_SUPPORT
 	#define WD_KAZAK_ABC				//×ÖÄ¸¹þÈø¿Ë£¨ÖÐ¹ú£©Óï
#endif 	

#ifdef IM_ICELANDIC_SUPPORT
 	#define WD_ICELANDIC_ABC			//×ÖÄ¸±ùµºÓï
#endif 	

#ifdef IM_SLOVAK_SUPPORT
 	#define WD_SLOVAK_ABC				//×ÖÄ¸Ë¹Âå·¥¿ËÓï
#endif 	

#ifdef IM_ESTONIAN_SUPPORT
 	#define WD_ESTONIAN_ABC				//×ÖÄ¸°®É³ÄáÑÇÓï
#endif 	

#ifdef IM_BELARUSIAN_SUPPORT
 	#define WD_BELARUSIAN_ABC			//×ÖÄ¸°×¶íÂÞË¹Óï
#endif 	

#ifdef IM_LATVIAN_SUPPORT
 	#define WD_LATVIAN_ABC				//×ÖÄ¸À­ÍÑÎ¬ÑÇÓï// 
#endif 	

#ifdef IM_SLOVENIAN_SUPPORT
 	#define WD_SLOVENIAN_ABC			//×ÖÄ¸Ë¹ÂåÎÄÄáÑÇÓï
#endif 	

#ifdef IM_CROATIAN_SUPPORT
 	#define WD_CROATIAN_ABC				//×ÖÄ¸¿ËÂÞµØÑÇÓï
#endif 	

#ifdef IM_SERBIAN_SUPPORT
 	#define WD_SERBIAN_ABC				//×ÖÄ¸Èû¶ûÎ¬ÑÇÓï
#endif 	

#ifdef IM_HAUSA_SUPPORT
 	#define WD_HAUSA_ABC				//×ÖÄ¸ºÀÈøÓï
#endif 	

#ifdef IM_IGBO_SUPPORT
 	#define WD_IGBO_ABC					//×ÖÄ¸ÒÁ²©Óï
#endif 	

#ifdef IM_TURKMEN_SUPPORT
 	#define WD_TURKMEN_ABC				//×ÖÄ¸ÍÁ¿âÂüÓï
#endif 	

#ifdef IM_SWAHILI_SUPPORT
 	#define WD_SWAHILI_ABC				//×ÖÄ¸Ë¹ÍßÏ£ÀïÓï
#endif 	

#ifdef IM_HUNGARIAN_SUPPORT
 	#define WD_HUNGARIAN_ABC			//×ÖÄ¸ÐÙÑÀÀûÓï
#endif 	

#ifdef IM_GREEK_SUPPORT
 	#define WD_GREEK_ABC				//×ÖÄ¸Ï£À°Óï
#endif 	

#ifdef IM_KAZAKH_SUPPORT
 	#define WD_KAZAKH_ABC				//×ÖÄ¸¹þÈø¿ËÓï
#endif 	

#ifdef IM_ROMANIAN_SUPPORT
 	#define WD_ROMANIAN_ABC				//×ÖÄ¸ÂÞÂíÄáÑÇÓï
#endif 	

#ifdef IM_TAGALOG_SUPPORT
 	#define WD_TAGALOG_ABC				//×ÖÄ¸Ëû¼ÓÂ»Óï
#endif 	

#ifdef IM_BOSNIAN_SUPPORT
 	#define WD_BOSNIAN_ABC				//×ÖÄ¸²¨Ë¹ÄáÑÇÓï
#endif 	

#ifdef IM_TAMIL_SUPPORT
    #define WD_TAMIL_ABC				//×ÖÄ¸Ì©Ã×¶ûÓï
#endif 	

#ifdef IM_CYRILLIC_SUPPORT
 	#define WD_CYRILLIC_ABC				//×ÖÄ¸Î÷Àï¶ûÓïÏµ
#endif 	

#ifdef IM_SERBIANC_SUPPORT
  #define WD_SERBIANC_ABC				//×ÖÄ¸Èû¶ûÎ¬ÑÇ(Î÷Àï¶û)
#endif 	

#ifdef IM_BENGALI_SUPPORT
 	#define WD_BENGALI_ABC		//×ÖÄ¸ÃÏ¼ÓÀ­Óï   qls
#endif 	

#ifdef IM_KHMER_SUPPORT
	#define WD_KHMER_ABC		//×ÖÄ¸¸ßÃÞÓï  qls
#endif 	

#ifdef IM_NEPALI_SUPPORT
 	#define WD_NEPALI_ABC		//×ÖÄ¸Äá²´¶ûÓï  qls
#endif 	

#ifdef IM_UKRAINIAN_SUPPORT
 	#define WD_UKRAINIAN_ABC	//×ÖÄ¸ÎÚ¿ËÀ¼Óï
#endif 	

#ifdef IM_BULGARIAN_SUPPORT
 	#define WD_BULGARIAN_ABC	//×ÖÄ¸±£¼ÓÀûÑÇÓï
#endif 	

#ifdef IM_CHOSEN_SUPPORT
 	#define WD_CHOSEN_ABC		//×ÖÄ¸³¯ÏÊÓï
#endif 	

#ifdef IM_MIAO_SUPPORT
 	#define	WD_MIAO_ABC			//×ÖÄ¸ÃçÓï
#endif 	

#ifdef IM_MONGOLIAN_TRADITIONAL_SUPPORT
 	#define WD_MONGOLIAN_TRADITIONAL_ABC	//´«Í³×ÖÄ¸ÃÉ¹ÅÓï
#endif 	

#ifdef IM_MONGOLIAN_SUPPORT
 	#define	WD_MONGOLIAN_ABC	//×ÖÄ¸ÃÉ¹ÅÓï
#endif 	

#ifdef IM_GEORGIAN_SUPPORT
 	#define WD_GEORGIAN_ABC	//×ÖÄ¸¸ñÂ³¼ªÑÇ
#endif 	

#ifdef IM_TELUGU_SUPPORT
  #define WD_TELUGU_ABC		//×ÖÄ¸Ì©Â¬¹ÌÓï	
#endif 	

#ifdef IM_BURMESE_SUPPORT
 	#define WD_BURMESE_ABC			  //×ÖÄ¸ÃåµéÓï					
#endif 	

#ifdef IM_MARATHI_SUPPORT
 	#define	WD_MARATHI_ABC			  //×ÖÄ¸ÂíÀ­µØÓï
#endif 	

#ifdef IM_KANNADA_SUPPORT
 	#define	WD_KANNADA_ABC			  //×ÖÄ¸¿²ÄÉ´ïÓï
#endif 	

#ifdef IM_ASSAMESE_SUPPORT
 	#define	WD_ASSAMESE_ABC		  //×ÖÄ¸°¢ÈøÄ·Óï
#endif 	

#ifdef IM_MALAYALAM_SUPPORT
 	#define	WD_MALAYALAM_ABC		  //×ÖÄ¸ÂíÀ­ÑÅÀ­Ä·Óï
#endif 	

#ifdef IM_PUNJABI_SUPPORT
 	#define	WD_PUNJABI_ABC			  //×ÖÄ¸ÅÔÕÚÆÕÓï
#endif 	

#ifdef IM_MAITHILI_SUPPORT
 	#define	WD_MAITHILI_ABC		  //×ÖÄ¸ÂõµÙÀûÓï
#endif 	

#ifdef IM_GUJARATI_SUPPORT
 	#define	WD_GUJARATI_ABC		  //×ÖÄ¸¹Å¼ªÀ­ÌØÓï
#endif 	

#ifdef IM_IRISH_SUPPORT
 	#define	WD_IRISH_ABC			  //×ÖÄ¸°®¶ûÀ¼Óï
#endif 	

#ifdef IM_CATALAN_SUPPORT
 	#define	WD_CATALAN_ABC			  //×ÖÄ¸¼ÓÌ©Â¡Óï
#endif 	

#ifdef IM_AZERBAIJANI_SUPPORT
 	#define	WD_AZERBAIJANI_ABC		  //×ÖÄ¸°¢Èû°Ý½®Óï
#endif 	

#ifdef IM_PASHTO_SUPPORT
		#define WD_PASHTO_ABC				//×ÖÄ¸ÆÕÊ²Í¼Óï
#endif 	

//#define WD_JAVANESE_ABC //×¦ÍÛÓï
//#define WD_MOROCCAN_ABC //Ä¦Âå¸çÓï
//#define WD_TUNISIAN_ABC//Í»ÄáË¹Óï
//#define WD_SUDANESE_ABC     //ËÕµ¤Óï
//#define WD_PAKISTAN_ABC    //°Í»ùË¹Ì¹Óï
#ifdef IM_SINHALESE_SUPPORT
	#define WD_SINHALESE_ABC  //É®Ù¤ÂÞÓï
#endif	
//#define WD_MACEDONIAN_ABC    //ÂíÆä¶ÙÓï
//#define WD_KINYARWANDA_ABC  //Â¬Íú´ïÓï
//#define WD_ZULU_ABC    		    //×æÂ³Óï
//#define WD_TATAR_ABC    	    //÷²÷°Óï
//#define WD_YEMENI_ABC     //Ò²ÃÅÓï
//#define WD_BHUTANI_ABC      //²»µ¤Óï
//#define WD_FIJIAN_ABC    	  //ì³¼ÃÓï
//#define WD_KONGO_ABC    		   //¸Õ¹ûÓï
//#define WD_SYRIAC_ABC       //ÐðÀûÑÇÓï
//#define WD_ALGERIAN_ABC      //°¢¶û¼°ÀûÑÇÓï
//#define WD_MALTESE_ABC       //Âí¶úËûÓï
//#define WD_WELSH_ABC    	  //Íþ¶ûÊ¿Óï
//#define WD_MALDIVIAN_ABC      //Âí¶û´ú·òÓï

//ÖÇÄÜ
// 	#define WD_HUNGARIAN			//ÐÙÑÀÀûÓïÖÇÄÜ
	#define WD_ENGLISH				//Ó¢ÎÄÖÇÄÜÊäÈë·¨
#ifdef IM_FRENCH_SUPPORT
 	#define WD_FRENCH				//·¨ÎÄÖÇÄÜÊäÈë·¨
#endif 	

#ifdef IM_HEBREW_SUPPORT
  #define WD_HEBREW				//Ï£²®À´ÓïÖÇÄÜÊäÈë·¨
#endif

#ifdef IM_RUSSIAN_SUPPORT
   #define WD_RUSSIAN				//¶íÓïÖÇÄÜÊäÈë·¨
#endif

#ifdef IM_ARABIC_SUPPORT
 	#define WD_ARABIC				//°¢À­²®ÎÄÖÇÄÜÊäÈë·¨
#endif 	

#ifdef IM_THAI_SUPPORT
  #define	WD_THAI					//Ì©ÎÄÖÇÄÜÊäÈë·¨
#endif
  
#ifdef IM_VIETNAMESE_SUPPORT
   #define WD_VIETNAMESE			//Ô½ÄÏÓïÖÇÄÜÊäÈë·¨
#endif
   
#ifdef IM_ITALIAN_SUPPORT
 	#define WD_ITALIAN				//Òâ´óÀûÓï
#endif 	

#ifdef IM_DANISH_SUPPORT
        #define WD_DANISH				//µ¤ÂóÓïÖÇÄÜÊäÈë·¨
#endif

#ifdef IM_SWEDISH_SUPPORT
        #define	WD_SWEDISH				//ÖÇÄÜÈðµäÓï
#endif

#ifdef IM_NORWEGIAN_SUPPORT
        #define WD_NORWEGIAN			//ÖÇÄÜÅ²ÍþÓï
#endif

#ifdef IM_PORTUGUESE_SUPPORT
 	#define WD_PORTUGUESE			//ÆÏÌÑÑÀÎÄÊäÈë·¨
#endif

#ifdef IM_SPANISH_SUPPORT
 	#define WD_SPANISH				//Î÷°àÑÀÎÄÊäÈë·¨
#endif

#ifdef IM_HINDI_SUPPORT
 	#define WD_HINDI				//ÖÇÄÜÓ¡µØÓï
#endif

#ifdef IM_MALAY_SUPPORT
 	#define WD_MALAY				//ÖÇÄÜÂíÀ´Óï
#endif

#ifdef IM_KOREAN_SUPPORT
	#define WD_KOREAN				//º«ÓïÊäÈë·¨
#endif

#ifdef IM_JAPANESE_SUPPORT
	#define WD_JAPANESE				//ÈÕÓïÊäÈë·¨
#endif

#ifdef IM_CZECH_SUPPORT
	#define WD_CZECH				//½Ý¿ËÓï
#endif

#ifdef IM_POLISH_SUPPORT
 	#define WD_POLISH				//²¨À¼Óï                
#endif

#ifdef IM_TURKISH_SUPPORT
 	#define WD_TURKISH				//ÖÇÄÜÍÁ¶úÆäÓï
#endif

#ifdef IM_GERMAN_SUPPORT
 	#define WD_GERMAN				//ÖÇÄÜµÂÓï
#endif

#ifdef IM_PERSIAN_SUPPORT
	#define WD_PERSIAN				//²¨Ë¹Óï
#endif

#ifdef IM_LAO_SUPPORT
	#define WD_LAO				    //ÀÏÎÎ
#endif

#ifdef IM_TAGALOG_SUPPORT
  #define WD_TAGALOG			    //Ëû¼ÓÂ»Óï
#endif

#ifdef IM_INDONESIAN_SUPPORT
 	 #define WD_INDONESIAN			//Ó¡¶ÈÄáÎ÷ÑÇÓï
#endif

#ifdef IM_UKRAINIANH_SUPPORT
	#define WD_UKRAINIAN			//ÎÚ¿ËÀ¼Óï
#endif

#ifdef IM_DUTCH_SUPPORT
 	#define WD_DUTCH				//ÖÇÄÜºÉÀ¼Óï
#endif

#ifdef IM_GREEK_SUPPORT
	#define WD_GREEK				//ÖÇÄÜÏ£À°Óï
#endif

#ifdef IM_URDU_SUPPORT
	#define WD_URDU					//ÎÚ¶ú¶¼Óï
#endif

#ifdef IM_KAZAKH_SUPPORT
  #define WD_KAZAKH				//ÖÇÄÜ¹þÈø¿ËÓï	
#endif

#ifdef IM_BENGALI_SUPPORT
  #define  WD_BENGALI    //ÖÇÄÜÃÏ¼ÓÀ­Óï 
#endif

#ifdef IM_KHMER_SUPPORT
	#define WD_KHMER			//ÖÇÄÜ¸ßÃÞÓï  qls
#endif

#ifdef IM_MYANMAR_SUPPORT
	#define WD_BURMESE			//ÖÇÄÜÃåµéÓï
#endif

#ifdef IM_NEPALI_SUPPORT
 	#define WD_NEPALI			//ÖÇÄÜÄá²´¶ûÓï  qls
#endif

#ifdef IM_ROMANIAN_SUPPORT
 	#define WD_ROMANIAN
#endif

#ifdef IM_SLOVENIAN_SUPPORT
 	#define WD_SLOVENIAN	
#endif

#ifdef IM_TELUGU_SUPPORT
 	#define WD_TELUGU		//ÖÇÄÜÌ©Â¬¹ÌÓï
#endif

#ifdef IM_BULGARIAN_SUPPORT
 	#define WD_BULGARIAN	//ÖÇÄÜ±£¼ÓÀûÑÇÓï
#endif

#ifdef IM_GEORGIAN_SUPPORT
 	#define WD_GEORGIAN		//ÖÇÄÜ¸ñÂ³¼ªÑÇ
#endif

#ifdef IM_ESTONIAN_SUPPORT
 	#define WD_ESTONIAN		//ÖÇÄÜ°®É³ÄáÑÇÓï
#endif

#ifdef IM_ICELANDIC_SUPPORT
 	#define WD_ICELANDIC	//ÖÇÄÜ±ùµºÓï
#endif

#ifdef IM_FINNISH_SUPPORT
 	#define	WD_FINNISH		//ÖÇÄÜ·ÒÀ¼Óï
#endif

#ifdef IM_BELARUSIAN_SUPPORT
 	#define	WD_BELARUSIAN	//ÖÇÄÜ°×¶íÂÞË¹Óï
#endif

#ifdef IM_SERBIAN_SUPPORT
 	#define WD_SERBIAN		//ÖÇÄÜÈü¶ûÎ¬ÑÇÓï
#endif

#ifdef IM_SLOVAK_SUPPORT
 	#define	WD_SLOVAK		//ÖÇÄÜË¹Âå·¥¿ËÓï
#endif

#ifdef IM_TAMIL_SUPPORT
	#define WD_TAMIL		//ÖÇÄÜÌ©Ã×¶ûÓï
#endif

#ifdef IM_TURKMEN_SUPPORT
 	#define	WD_TURKMEN		//ÖÇÄÜÍÁ¿âÂüÓï
#endif

#ifdef IM_CHOSEN_SUPPORT
 	#define WD_CHOSEN		//ÖÇÄÜ³¯ÏÊÓï
#endif

#ifdef IM_MONGOLIAN_SUPPORT
 	#define	WD_MONGOLIAN	//ÖÇÄÜÃÉ¹ÅÓï
#endif

#ifdef IM_SWAHILI_SUPPORT
 	#define WD_SWAHILI		//ÖÇÄÜË¹ÍßÎ÷Àï
#endif

#ifdef IM_MARATHI_SUPPORT
 #define	WD_MARATHI			//ÖÇÄÜÂíÀ­µØÓï
#endif

#ifdef IM_KANNADA_SUPPORT
 #define	WD_KANNADA			//ÖÇÄÜ¿²ÄÉ´ïÓï
#endif

#ifdef IM_ASSAMESE_SUPPORT
 #define	WD_ASSAMESE			//ÖÇÄÜ°¢ÈøÄ·Óï
#endif

#ifdef IM_MALAYALAM_SUPPORT
 #define	WD_MALAYALAM		//ÖÇÄÜÂíÀ­ÑÅÀ­Ä·Óï
#endif

#ifdef IM_PUNJABI_SUPPORT
 #define	WD_PUNJABI			//ÖÇÄÜÅÔÕÚÆÕÓï
#endif

#ifdef IM_MAITHILI_SUPPORT
 #define	WD_MAITHILI			//ÖÇÄÜÂõµÙÀûÓï
#endif

#ifdef IM_GUJARATI_SUPPORT
 #define	WD_GUJARATI			//ÖÇÄÜ¹Å¼ªÀ­ÌØÓï
#endif

#ifdef IM_SERBIANC_SUPPORT
 #define  WD_SERBIANC			//ÖÇÄÜÈû¶ûÎ¬ÑÇÎ÷Àï¶û
#endif

#ifdef IM_BOSNIAN_SUPPORT
 #define  WD_BOSNIAN			//²¨Ë¹ÄáÑÇÓïÖÇÄÜÊäÈë·¨
#endif

#ifdef IM_IRISH_SUPPORT
 #define  WD_IRISH			//°®¶ûÀ¼ÓïÖÇÄÜÊäÈë·¨
#endif

#ifdef IM_AZERBAIJANI_SUPPORT
 #define  WD_AZERBAIJANI		//°¢Èû°Ý½®ÓïÖÇÄÜÊäÈë·¨
#endif

#ifdef IM_IGBO_SUPPORT
 #define  WD_IGBO				//ÒÁ²©Óï
#endif

#ifdef IM_HAUSA_SUPPORT
 #define  WD_HAUSA			//ºÀÈøÓï
#endif

#ifdef IM_LATVIAN_SUPPORT
 #define  WD_LATVIAN			//À­ÍÑÎ¬ÑÇÓï// 
#endif

#ifdef IM_CROATIAN_SUPPORT
 #define  WD_CROATIAN			//¿ËÂÞµØÑÇÓï
#endif

#ifdef IM_PASHTO_SUPPORT
	 #define WD_PASHTO			//ÖÇÄÜÆÕÊ²Í¼Óï
#endif

//ÉÙÊýÃñ×åÓïÑÔ
// 	#define WD_YI				//ÉÙÊýÃñ×åÓïÑÔ£ºÒÍÓï
//	#define WD_CNOSEN				//ÉÙÊýÃñ×åÓïÑÔ£º³¯ÏÊÓï
//	#define WD_TIBETAN				//ÉÙÊýÃñ×åÓïÑÔ£º²ØÓïwanghui
//	#define WD_TIBETAN_ABC			//²ØÓï×ÖÄ¸ÊäÈë·¨ qls
#ifdef IM_UYGHUR_SUPPORT
 	#define WD_UIGHUR				//ÉÙÊýÃñ×åÓïÑÔ£ºÎ¬Óï
#endif 	
// 	#define WD_MIAO					//ÉÙÊýÃñ×åÓïÑÔ£ºÃçÓï
// 	#define WD_ZHUANG				//ÉÙÊýÃñ×åÓïÑÔ£º×³Óï
// 	#define WD_DONG					//ÉÙÊýÃñ×åÓïÑÔ£º¶±Óï
// 	#define WD_KAZAK				//ÖÇÄÜ¹þÈø¿ËÓï(ÖÐ¹ú)		
// 	#define WD_MONGOLIAN_TRADITIONAL		//ÉÙÊýÃñ×åÓïÑÔ£ºÃÉ¹ÅÓï
// 	#define WD_DEHONG_DAI			//ÉÙÊýÃñ×åÓïÑÔ£º´öÓï
	//#define WD_UIGHUR //ÖÇÄÜÎ¬Îá¶ûÓï

//#define WD_JAVANESE //×¦ÍÛÓï
//#define WD_MOROCCAN //Ä¦Âå¸çÓï
//#define WD_TUNISIAN//Í»ÄáË¹Óï
//#define WD_SUDANESE     //ËÕµ¤Óï
//#define WD_PAKISTAN    //°Í»ùË¹Ì¹Óï
#ifdef IM_SINHALESE_SUPPORT
	#define WD_SINHALESE  //É®Ù¤ÂÞÓï
#endif	
//#define WD_MACEDONIAN    //ÂíÆä¶ÙÓï
//#define WD_KINYARWANDA  //Â¬Íú´ïÓï
//#define WD_ZULU    		    //×æÂ³Óï
//#define WD_TATAR    	    //÷²÷°Óï
//#define WD_YEMENI     //Ò²ÃÅÓï
//#define WD_BHUTANI      //²»µ¤Óï
//#define WD_FIJIAN    	  //ì³¼ÃÓï
//#define WD_KONGO    		   //¸Õ¹ûÓï
//#define WD_SYRIAC       //ÐðÀûÑÇÓï
//#define WD_ALGERIAN      //°¢¶û¼°ÀûÑÇÓï
//#define WD_MALTESE       //Âí¶úËûÓï
//#define WD_WELSH    	  //Íþ¶ûÊ¿Óï
//#define WD_MALDIVIAN      //Âí¶û´ú·òÓï

WD_abcdefg	languageInfo[WD_IME_TOTAL]={0};

	
WD_UINT8 IsLanguageExist(WD_UINT8 languageType,WD_UINT8* exist,WD_UINT8 type)
{
	WD_UINT8 existTemp[64]={0},bit,existNum;
	
	SCI_MEMCPY(existTemp,exist,64*sizeof(WD_UINT8));
	bit=languageType%4;
	existNum=languageType/4;
	
	if (type==1)

	{
		switch(bit)
		{
		case 0:
			existTemp[existNum]+=1;
			break;	
		case 1:
			existTemp[existNum]+=2;
			break;
		case 2:
			existTemp[existNum]+=4;
			break;
		case 3:
			existTemp[existNum]+=8;
			break;
		default:
			break;
		}
		SCI_MEMCPY(exist,existTemp,64*sizeof(WD_UINT8));
		return 3;//ÓïÑÔÒÑ¶¨Òå
	}
	else
	{
		if ((existTemp[existNum] >>bit)&1)
		{
			return 1;//ÓïÑÔ´æÔÚ
		}
		else
		{
			return 0;//ÓïÑÔ²»´æÔÚ
		}
	}
	
}

/*lint -save -e605*/	
void ImeCopy(WD_UINT8* exist)
{
	WD_UINT8  i = 0;  //ÓÃÓÚ»ñÈ¡ÊäÈë·¨¸öÊý
	WD_UINT8 existTemp[64]={0};
	SCI_MEMSET(&languageInfo , 0, sizeof(languageInfo));



{//Êý×Ö
#ifdef WD_DIGIT
	languageInfo[i].imeType = WDI_DIGIT;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x6570;
	languageInfo[i].imeString[1] = 0x5B57;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "Êý×Ö", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
#endif
}


//´¿Æ´Òô
#ifdef WD_CN_CPINYIN
{
	languageInfo[i].imeType = WDI_CN_CPINYIN;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x7EAF;
	languageInfo[i].imeString[1] = 0x62FC;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "´¿Æ´", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//ÖØ¸´¼üÆ´Òô
#ifdef WD_CN_FULLPINYIN
{
	languageInfo[i].imeType = WDI_CN_FULLPINYIN;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x5168;
	languageInfo[i].imeString[1] = 0x62fc;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "È«Æ´", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//Ì¨Íå×¢Òô
#ifdef WD_CN_ZHUYIN
{
	languageInfo[i].imeType = WDI_CN_ZHUYIN;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x6CE8;
	languageInfo[i].imeString[1] = 0x97F3;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "×¢Òô", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//±Ê»®USE_WDI_CN_BIHUA
#ifdef WD_CN_BIHUA
{
	languageInfo[i].imeType = WDI_CN_BIHUA;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x7B14;
	languageInfo[i].imeString[1] = 0x5212;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "±Ê»®", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//±Ê»®·±Ìå
#ifdef WD_CN_BIHUAF
{
	languageInfo[i].imeType = WDI_CN_BIHUAF;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x7B46;
	languageInfo[i].imeString[1] = 0x756B;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "¹P®‹", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//Ì¨Íå×¢Òô
#ifdef WD_CN_FULLZHUYIN
{
	languageInfo[i].imeType = WDI_CN_FULLZHUYIN;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x5168;
	languageInfo[i].imeString[1] = 0x6CE8;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "È«×¢", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ABC
#ifdef WD_ENGLISH_ABC
{
	extern const lan_keymap_Info_t English_ABC_keymap_Info;
	cpy_data_2_data(i, &English_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
//Á¢ÌÕÍðÓïABC
#ifdef WD_LITHUANIAN_ABC
{
	extern lan_keymap_Info_t Lithuanian_ABC_keymap_Info;
	cpy_data_2_data(i, &Lithuanian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

//LA DING ABC
#ifdef WD_LATIN_ABC
{
	extern lan_keymap_Info_t Latin_ABC_keymap_Info;
	cpy_data_2_data(i, &Latin_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//¶íÓï×ÖÄ¸ÊäÈë·¨
#ifdef WD_RUSSIAN_ABC
{
	extern lan_keymap_Info_t Russian_ABC_keymap_Info;
	cpy_data_2_data(i, &Russian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//·¨Óï×ÖÄ¸ÊäÈë·¨
#ifdef WD_FRENCH_ABC
{
	extern const lan_keymap_Info_t French_ABC_keymap_Info;
	cpy_data_2_data(i, &French_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
	
//Ô½ÄÏÓï×ÖÄ¸ÊäÈë·¨
#ifdef WD_VIETNAMESE_ABC
{
	extern lan_keymap_Info_t Vietnamese_ABC_keymap_Info;
	cpy_data_2_data(i, &Vietnamese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//Ì©Óï×ÖÄ¸ÊäÈë·¨
#ifdef WD_THAI_ABC
{
	extern lan_keymap_Info_t Thai_ABC_keymap_Info;
	cpy_data_2_data(i, &Thai_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//Ó¡ÄáÓï×ÖÄ¸ÊäÈë·¨
#ifdef WD_INDONESIAN_ABC
{
	extern lan_keymap_Info_t Indonesian_ABC_keymap_Info;
	cpy_data_2_data(i, &Indonesian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÆÏÌÑÑÀÓï×ÖÄ¸ÊäÈë·¨
#ifdef WD_PORTUGUESE_ABC
{
	extern lan_keymap_Info_t Portuguese_ABC_keymap_Info;
	cpy_data_2_data(i, &Portuguese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


	//×ÖÄ¸°¢À­²®Óï
#ifdef WD_ARABIC_ABC
{
	extern const lan_keymap_Info_t Arabic_ABC_keymap_Info;
	cpy_data_2_data(i, &Arabic_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
	
	//×ÖÄ¸µÂÓï
#ifdef WD_GERMAN_ABC
{
	extern lan_keymap_Info_t German_ABC_keymap_Info;
	cpy_data_2_data(i, &German_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Ó¡µØÓï
#ifdef WD_HINDI_ABC
{
	extern lan_keymap_Info_t Hindi_ABC_keymap_Info;
	cpy_data_2_data(i, &Hindi_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Òâ´óÀûÓï
#ifdef WD_ITALIAN_ABC
{
	extern const lan_keymap_Info_t Italian_ABC_keymap_Info;
	cpy_data_2_data(i, &Italian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸ÂíÀ´Óï
#ifdef WD_MALAY_ABC
{
	extern lan_keymap_Info_t Malay_ABC_keymap_Info;
	cpy_data_2_data(i, &Malay_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸²¨Ë¹Óï
#ifdef WD_PERSIAN_ABC
{
	extern lan_keymap_Info_t Persian_ABC_keymap_Info;
	cpy_data_2_data(i, &Persian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Î÷°àÑÀÓï
#ifdef WD_SPANISH_ABC
{
	extern lan_keymap_Info_t Spanish_ABC_keymap_Info;
	cpy_data_2_data(i, &Spanish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Î÷°àÑÀÓï
#ifdef WD_TURKISH_ABC
{
	extern lan_keymap_Info_t Turkish_ABC_keymap_Info;
	cpy_data_2_data(i, &Turkish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸½Ý¿ËÓï
#ifdef WD_CZECH_ABC
{
	extern lan_keymap_Info_t Czech_ABC_keymap_Info;
	cpy_data_2_data(i, &Czech_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸ºÉÀ¼Óï
#ifdef WD_DUTCH_ABC
{
	extern lan_keymap_Info_t Dutch_ABC_keymap_Info;
	cpy_data_2_data(i, &Dutch_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Ï£²®À´Óï
#ifdef WD_HEBREW_ABC
{
	extern lan_keymap_Info_t Hebrew_ABC_keymap_Info;
	cpy_data_2_data(i, &Hebrew_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸²¨À¼Óï
#ifdef WD_POLISH_ABC
{
	extern lan_keymap_Info_t Polish_ABC_keymap_Info;
	cpy_data_2_data(i, &Polish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Î¬Îá¶ûÓï
#ifdef WD_UIGHUR_ABC
{
	extern lan_keymap_Info_t Uighur_ABC_keymap_Info;
	cpy_data_2_data(i, &Uighur_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif	

	//×ÖÄ¸ÈðµäÓï
#ifdef WD_SWEDISH_ABC
{
	extern lan_keymap_Info_t Swedish_ABC_keymap_Info;
	cpy_data_2_data(i, &Swedish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif	

	//×ÖÄ¸·ÒÀ¼Óï
#ifdef WD_FINNISH_ABC
{
	extern lan_keymap_Info_t Finnish_ABC_keymap_Info;
	cpy_data_2_data(i, &Finnish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Å²ÍþÓï
#ifdef WD_NORWEGIAN_ABC
{
	extern lan_keymap_Info_t Norwegian_ABC_keymap_Info;
	cpy_data_2_data(i, &Norwegian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸µ¤ÂóÓï
#ifdef WD_DANISH_ABC
{
	extern lan_keymap_Info_t Danish_ABC_keymap_Info;
	cpy_data_2_data(i, &Danish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸ÀÏÎÎÓï
#ifdef WD_LAO_ABC
{
	extern lan_keymap_Info_t Lao_ABC_keymap_Info;
	cpy_data_2_data(i, &Lao_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸ÎÚ¶û¶¼Óï
#ifdef WD_URDU_ABC
{
	extern lan_keymap_Info_t Urdu_ABC_keymap_Info;
	cpy_data_2_data(i, &Urdu_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸¹þÈø¿ËÓï(ÖÐ¹ú)
#ifdef WD_KAZAK_ABC
{
	extern lan_keymap_Info_t Kazak_ABC_keymap_Info;
	cpy_data_2_data(i, &Kazak_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸±ùµºÓï
#ifdef WD_ICELANDIC_ABC
{
	extern lan_keymap_Info_t Iceland_ABC_keymap_Info;
	cpy_data_2_data(i, &Iceland_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Ë¹Âå·¥¿ËÓï
#ifdef WD_SLOVAK_ABC
{
	extern lan_keymap_Info_t Slovak_ABC_keymap_Info;
	cpy_data_2_data(i, &Slovak_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸°®É³ÄáÑÇÓï
#ifdef WD_ESTONIAN_ABC
{
	extern lan_keymap_Info_t Estonian_ABC_keymap_Info;
	cpy_data_2_data(i, &Estonian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


	//×ÖÄ¸°×¶íÂÞË¹Óï
#ifdef WD_BELARUSIAN_ABC
{
	extern lan_keymap_Info_t Belarusian_ABC_keymap_Info;
	cpy_data_2_data(i, &Belarusian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸À­ÍÑÎ¬ÑÇÓï
#ifdef WD_LATVIAN_ABC
{
	extern lan_keymap_Info_t Latvian_ABC_keymap_Info;
	cpy_data_2_data(i, &Latvian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Ë¹ÂåÎÄÄáÑÇÓï
#ifdef WD_SLOVENIAN_ABC
{
	extern lan_keymap_Info_t Slovenian_ABC_keymap_Info;
	cpy_data_2_data(i, &Slovenian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸¿ËÂÞµØÑÇÓï
#ifdef WD_CROATIAN_ABC
{
	extern lan_keymap_Info_t Croatian_ABC_keymap_Info;
	cpy_data_2_data(i, &Croatian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Èû¶ûÎ¬ÑÇÓï
#ifdef WD_SERBIAN_ABC
{
	extern lan_keymap_Info_t Serbian_ABC_keymap_Info;
	cpy_data_2_data(i, &Serbian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸ºÀÈøÓï
#ifdef WD_HAUSA_ABC
{
	extern lan_keymap_Info_t Hausa_ABC_keymap_Info;
	cpy_data_2_data(i, &Hausa_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸ÒÁ²©Óï
#ifdef WD_IGBO_ABC
{
	extern lan_keymap_Info_t Igbo_ABC_keymap_Info;
	cpy_data_2_data(i, &Igbo_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸ÍÁ¿âÂüÓï
#ifdef WD_TURKMEN_ABC
{
	extern lan_keymap_Info_t Turkmen_ABC_keymap_Info;
	cpy_data_2_data(i, &Turkmen_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Ë¹ÍßÏ£ÀïÓï
#ifdef WD_SWAHILI_ABC
{
	extern lan_keymap_Info_t Swahili_ABC_keymap_Info;
	cpy_data_2_data(i, &Swahili_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸ÐÙÑÀÀûÓï
#ifdef WD_HUNGARIAN_ABC
{
	extern lan_keymap_Info_t Hungarian_ABC_keymap_Info;
	cpy_data_2_data(i, &Hungarian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Ï£À°Óï
#ifdef WD_GREEK_ABC
{
	extern lan_keymap_Info_t Greek_ABC_keymap_Info;
	cpy_data_2_data(i, &Greek_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

		//×ÖÄ¸¹þÈø¿ËÓï
#ifdef WD_KAZAKH_ABC
{
	extern lan_keymap_Info_t Kazakh_ABC_keymap_Info;
	cpy_data_2_data(i, &Kazakh_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
	
	
	//×ÖÄ¸ÂÞÂíÄáÑÇÓï
#ifdef WD_ROMANIAN_ABC
{
	extern lan_keymap_Info_t Romanian_ABC_keymap_Info;
	cpy_data_2_data(i, &Romanian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Ëû¼ÓÂ»Óï
#ifdef WD_TAGALOG_ABC
{
	extern lan_keymap_Info_t Tagalog_ABC_keymap_Info;
	cpy_data_2_data(i, &Tagalog_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
	//×ÖÄ¸²¨Ë¹ÄáÑÇÓï
#ifdef WD_BOSNIAN_ABC
{
	extern lan_keymap_Info_t Bosnian_ABC_keymap_Info;
	cpy_data_2_data(i, &Bosnian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Ì©Ã×¶ûÓïwanghui
#ifdef WD_TAMIL_ABC
{
	extern lan_keymap_Info_t Tamil_ABC_keymap_Info;
	cpy_data_2_data(i, &Tamil_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Î÷Àï¶ûÓïwanghui
#ifdef WD_CYRILLIC_ABC
{
	extern lan_keymap_Info_t Cyrillic_ABC_keymap_Info;
	cpy_data_2_data(i, &Cyrillic_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//×ÖÄ¸Èû¶ûÎ¬ÑÇ(Î÷Àï¶ûÓï)wanghui
#ifdef WD_SERBIANC_ABC
{
	extern lan_keymap_Info_t Serbianc_ABC_keymap_Info;
	cpy_data_2_data(i, &Serbianc_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_TELUGU_ABC			//Ì©Â¬¹ÌÓï×ÖÄ¸ÊäÈë·¨
{
	extern lan_keymap_Info_t Telugu_ABC_keymap_Info;
	cpy_data_2_data(i, &Telugu_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_BULGARIAN_ABC		/* ±£¼ÓÀûÑÇÓï×ÖÄ¸				*/
{
	extern lan_keymap_Info_t Bulgarian_ABC_keymap_Info;
	cpy_data_2_data(i, &Bulgarian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_BENGALI_ABC		// ÃÏ¼ÓÀ­Óï×ÖÄ¸ÊäÈë·¨qls
{
	extern lan_keymap_Info_t Bengali_ABC_keymap_Info;
	cpy_data_2_data(i, &Bengali_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_UKRAINIAN_ABC
{
	extern lan_keymap_Info_t Ukrainian_ABC_keymap_Info;
	cpy_data_2_data(i, &Ukrainian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;

}
#endif
#ifdef WD_KHMER_ABC		//×ÖÄ¸¸ßÃÞÓï  qls
{
	extern lan_keymap_Info_t Khmer_ABC_keymap_Info;
	cpy_data_2_data(i, &Khmer_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_NEPALI_ABC		//×ÖÄ¸Äá²´¶ûÓï  qls
{
	extern lan_keymap_Info_t Nepali_ABC_keymap_Info;
	cpy_data_2_data(i, &Nepali_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_CHOSEN_ABC		//×ÖÄ¸³¯ÏÊÓï
{
	extern lan_keymap_Info_t Chosen_ABC_keymap_Info;
	cpy_data_2_data(i, &Chosen_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MIAO_ABC			//×ÖÄ¸ÃçÓï
{
	extern lan_keymap_Info_t Miao_ABC_keymap_Info;
	cpy_data_2_data(i, &Miao_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MONGOLIAN_TRADITIONAL_ABC	//´«Í³×ÖÄ¸ÃÉ¹ÅÓï
{
	extern lan_keymap_Info_t Mongolian_Traditional_ABC_keymap_Info;
	cpy_data_2_data(i, &Mongolian_Traditional_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MONGOLIAN_ABC	//×ÖÄ¸ÃÉ¹ÅÓï
{
	extern lan_keymap_Info_t Mongolian_ABC_keymap_Info;
	cpy_data_2_data(i, &Mongolian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_GEORGIAN_ABC	//×ÖÄ¸¸ñÂ³¼ªÑÇ
{
	extern lan_keymap_Info_t Georgian_ABC_keymap_Info;
	cpy_data_2_data(i, &Georgian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_BURMESE_ABC	//×ÖÄ¸Ãåµé
{
	extern lan_keymap_Info_t Burmese_ABC_keymap_Info;
	cpy_data_2_data(i, &Burmese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MARATHI_ABC	//×ÖÄ¸ÂíÀ­µØ
{
	extern lan_keymap_Info_t Marathi_ABC_keymap_Info;
	cpy_data_2_data(i, &Marathi_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_KANNADA_ABC	//×ÖÄ¸¿²ÄÉ´ï
{
	extern lan_keymap_Info_t Kannada_ABC_keymap_Info;
	cpy_data_2_data(i, &Kannada_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_ASSAMESE_ABC	//×ÖÄ¸°¢ÈøÄ·
{
	extern lan_keymap_Info_t Assamese_ABC_keymap_Info;
	cpy_data_2_data(i, &Assamese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MALAYALAM_ABC	//×ÖÄ¸ÂíÀ­ÑÇÀ­Ä·
{
	extern lan_keymap_Info_t Malayalam_ABC_keymap_Info;
	cpy_data_2_data(i, &Malayalam_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_PUNJABI_ABC	//×ÖÄ¸ÅÔÕÚÆÕ
{
	extern lan_keymap_Info_t Punjabi_ABC_keymap_Info;
	cpy_data_2_data(i, &Punjabi_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MAITHILI_ABC	//×ÖÄ¸ÂõµÙÀû
{
	extern lan_keymap_Info_t Maithiti_ABC_keymap_Info;
	cpy_data_2_data(i, &Maithiti_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_GUJARATI_ABC	//×ÖÄ¸¹Å¼ªÀ­ÌØ
{
	extern lan_keymap_Info_t Gujarati_ABC_keymap_Info;
	cpy_data_2_data(i, &Gujarati_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_IRISH_ABC	//×ÖÄ¸°®¶ûÀ¼
{
	extern lan_keymap_Info_t Irish_ABC_keymap_Info;
	cpy_data_2_data(i, &Irish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_CATALAN_ABC	//×ÖÄ¸¼ÓÌ©Â¡
{
	extern lan_keymap_Info_t Catalan_ABC_keymap_Info;
	cpy_data_2_data(i, &Catalan_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_AZERBAIJANI_ABC	//×ÖÄ¸¼ÓÌ©Â¡
{
	extern lan_keymap_Info_t Azerbaijani_ABC_keymap_Info;
	cpy_data_2_data(i, &Azerbaijani_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

				
#ifdef WD_PASHTO_ABC	//×ÖÄ¸ÆÕÊ²Í¼Óï
{
	extern lan_keymap_Info_t Pashto_ABC_keymap_Info;
	cpy_data_2_data(i, &Pashto_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif


#ifdef WD_PASHTO	//ÖÇÄÜÆÕÊ²Í¼Óï
{
	extern lan_ddd Pashto_Ime_Data;
	extern lan_keymap_Info_t Pashto_keymap_Info;
	cpy_data_2_data(i, &Pashto_keymap_Info, &Pashto_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_JAVANESE_ABC //×ÖÄ¸×¦ÍÛÓï
{
	extern lan_keymap_Info_t Javanese_ABC_keymap_Info;
	cpy_data_2_data(i, &Javanese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MOROCCAN_ABC //×ÖÄ¸Ä¦Âå¸çÓï
{
	extern lan_keymap_Info_t Moroccan_ABC_keymap_Info;
	cpy_data_2_data(i, &Moroccan_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_TUNISIAN_ABC//×ÖÄ¸Í»ÄáË¹Óï
{
	extern lan_keymap_Info_t Tunisian_ABC_keymap_Info;
	cpy_data_2_data(i, &Tunisian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SUDANESE_ABC     //×ÖÄ¸ËÕµ¤Óï
{
	extern lan_keymap_Info_t Sudanese_ABC_keymap_Info;
	cpy_data_2_data(i, &Sudanese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_PAKISTAN_ABC    //×ÖÄ¸°Í»ùË¹Ì¹Óï
{
	extern lan_keymap_Info_t Pakistan_ABC_keymap_Info;
	cpy_data_2_data(i, &Pakistan_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SINHALESE_ABC  //×ÖÄ¸É®Ù¤ÂÞÓï
{
	extern lan_keymap_Info_t Sinhalese_ABC_keymap_Info;
	cpy_data_2_data(i, &Sinhalese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MACEDONIAN_ABC    //×ÖÄ¸ÂíÆä¶ÙÓï
{
	extern lan_keymap_Info_t Macedonian_ABC_keymap_Info;
	cpy_data_2_data(i, &Macedonian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_KINYARWANDA_ABC  //×ÖÄ¸Â¬Íú´ïÓï
{
	extern lan_keymap_Info_t Kinyarwanda_ABC_keymap_Info;
	cpy_data_2_data(i, &Kinyarwanda_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_ZULU_ABC    		    //×ÖÄ¸×æÂ³Óï
{
	extern lan_keymap_Info_t Zulu_ABC_keymap_Info;
	cpy_data_2_data(i, &Zulu_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_TATAR_ABC    	    //×ÖÄ¸÷²÷°Óï
{
	extern lan_keymap_Info_t Tatar_ABC_keymap_Info;
	cpy_data_2_data(i, &Tatar_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_YEMENI_ABC     //×ÖÄ¸Ò²ÃÅÓï
{
	extern lan_keymap_Info_t Yemeni_ABC_keymap_Info;
	cpy_data_2_data(i, &Yemeni_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_BHUTANI_ABC      //×ÖÄ¸²»µ¤Óï
{
	extern lan_keymap_Info_t Bhutani_ABC_keymap_Info;
	cpy_data_2_data(i, &Bhutani_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_FIJIAN_ABC    	  //×ÖÄ¸ì³¼ÃÓï
{
	extern lan_keymap_Info_t Fijian_ABC_keymap_Info;
	cpy_data_2_data(i, &Fijian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_KONGO_ABC    		   //×ÖÄ¸¸Õ¹ûÓï
{
	extern lan_keymap_Info_t Kongo_ABC_keymap_Info;
	cpy_data_2_data(i, &Kongo_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SYRIAC_ABC       //×ÖÄ¸ÐðÀûÑÇÓï
{
	extern lan_keymap_Info_t Syriac_ABC_keymap_Info;
	cpy_data_2_data(i, &Syriac_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_ALGERIAN_ABC      //×ÖÄ¸°¢¶û¼°ÀûÑÇÓï
{
	extern lan_keymap_Info_t Algerian_ABC_keymap_Info;
	cpy_data_2_data(i, &Algerian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MALTESE_ABC       //×ÖÄ¸Âí¶úËûÓï
{
	extern lan_keymap_Info_t Maltese_ABC_keymap_Info;
	cpy_data_2_data(i, &Maltese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_WELSH_ABC    	  //×ÖÄ¸Íþ¶ûÊ¿Óï
{
	extern lan_keymap_Info_t Welsh_ABC_keymap_Info;
	cpy_data_2_data(i, &Welsh_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MALDIVIAN_ABC      //×ÖÄ¸Âí¶û´ú·òÓï
{
	extern lan_keymap_Info_t Maldivian_ABC_keymap_Info;
	cpy_data_2_data(i, &Maldivian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_JAVANESE //ÖÇÄÜ×¦ÍÛÓï
{
	extern lan_ddd Javanese_Ime_Data;
	extern lan_keymap_Info_t Javanese_keymap_Info;
	cpy_data_2_data(i, &Javanese_keymap_Info, &Javanese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MOROCCAN //ÖÇÄÜÄ¦Âå¸çÓï
{
	extern lan_ddd Moroccan_Ime_Data;
	extern lan_keymap_Info_t Moroccan_keymap_Info;
	cpy_data_2_data(i, &Moroccan_keymap_Info, &Moroccan_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_TUNISIAN//ÖÇÄÜÍ»ÄáË¹Óï
{
	extern lan_ddd Tunisian_Ime_Data;
	extern lan_keymap_Info_t Tunisian_keymap_Info;
	cpy_data_2_data(i, &Tunisian_keymap_Info, &Tunisian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SUDANESE     //ÖÇÄÜËÕµ¤Óï
{
	extern lan_ddd Sudanese_Ime_Data;
	extern lan_keymap_Info_t Sudanese_keymap_Info;
	cpy_data_2_data(i, &Sudanese_keymap_Info, &Sudanese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_PAKISTAN    //ÖÇÄÜ°Í»ùË¹Ì¹Óï
{
	extern lan_ddd Pakistan_Ime_Data;
	extern lan_keymap_Info_t Pakistan_keymap_Info;
	cpy_data_2_data(i, &Pakistan_keymap_Info, &Pakistan_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SINHALESE  //ÖÇÄÜÉ®Ù¤ÂÞÓï
{
	extern lan_ddd Sinhalese_Ime_Data;
	extern lan_keymap_Info_t Sinhalese_keymap_Info;
	cpy_data_2_data(i, &Sinhalese_keymap_Info, &Sinhalese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MACEDONIAN    //ÖÇÄÜÂíÆä¶ÙÓï
{
	extern lan_ddd Macedonian_Ime_Data;
	extern lan_keymap_Info_t Macedonian_keymap_Info;
	cpy_data_2_data(i, &Macedonian_keymap_Info, &Macedonian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_KINYARWANDA  //ÖÇÄÜÂ¬Íú´ïÓï
{
	extern lan_ddd Kinyarwanda_Ime_Data;
	extern lan_keymap_Info_t Kinyarwanda_keymap_Info;
	cpy_data_2_data(i, &Kinyarwanda_keymap_Info, &Kinyarwanda_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_ZULU    		    //ÖÇÄÜ×æÂ³Óï
{
	extern lan_ddd Zulu_Ime_Data;
	extern lan_keymap_Info_t Zulu_keymap_Info;
	cpy_data_2_data(i, &Zulu_keymap_Info, &Zulu_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_TATAR    	    //ÖÇÄÜ÷²÷°Óï
{
	extern lan_ddd Tatar_Ime_Data;
	extern lan_keymap_Info_t Tatar_keymap_Info;
	cpy_data_2_data(i, &Tatar_keymap_Info, &Tatar_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_YEMENI     //ÖÇÄÜÒ²ÃÅÓï
{
	extern lan_ddd Yemeni_Ime_Data;
	extern lan_keymap_Info_t Yemeni_keymap_Info;
	cpy_data_2_data(i, &Yemeni_keymap_Info, &Yemeni_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_BHUTANI      //ÖÇÄÜ²»µ¤Óï
{
	extern lan_ddd Bhutani_Ime_Data;
	extern lan_keymap_Info_t Bhutani_keymap_Info;
	cpy_data_2_data(i, &Bhutani_keymap_Info, &Bhutani_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_FIJIAN    	  //ÖÇÄÜì³¼ÃÓï
{
	extern lan_ddd Fijian_Ime_Data;
	extern lan_keymap_Info_t Fijian_keymap_Info;
	cpy_data_2_data(i, &Fijian_keymap_Info, &Fijian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_KONGO    		   //ÖÇÄÜ¸Õ¹ûÓï
{
	extern lan_ddd Kongo_Ime_Data;
	extern lan_keymap_Info_t Kongo_keymap_Info;
	cpy_data_2_data(i, &Kongo_keymap_Info, &Kongo_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SYRIAC       //ÖÇÄÜÐðÀûÑÇÓï
{
	extern lan_ddd Syriac_Ime_Data;
	extern lan_keymap_Info_t Syriac_keymap_Info;
	cpy_data_2_data(i, &Syriac_keymap_Info, &Syriac_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_ALGERIAN      //ÖÇÄÜ°¢¶û¼°ÀûÑÇÓï
{
	extern lan_ddd Algerian_Ime_Data;
	extern lan_keymap_Info_t Algerian_keymap_Info;
	cpy_data_2_data(i, &Algerian_keymap_Info, &Algerian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MALTESE       //ÖÇÄÜÂí¶úËûÓï
{
	extern lan_ddd Maltese_Ime_Data;
	extern lan_keymap_Info_t Maltese_keymap_Info;
	cpy_data_2_data(i, &Maltese_keymap_Info, &Maltese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_WELSH    	  //ÖÇÄÜÍþ¶ûÊ¿Óï
{
	extern lan_ddd Welsh_Ime_Data;
	extern lan_keymap_Info_t Welsh_keymap_Info;
	cpy_data_2_data(i, &Welsh_keymap_Info, &Welsh_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MALDIVIAN      //ÖÇÄÜÂí¶û´ú·òÓï
{
	extern lan_ddd Maldivian_Ime_Data;
	extern lan_keymap_Info_t Maldivian_keymap_Info;
	cpy_data_2_data(i, &Maldivian_keymap_Info, &Maldivian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

//ÖÇÄÜÎ¬Îá¶ûÓï
#ifdef WD_UIGHUR/*_ABC*///change by wanghui
{
	extern lan_ddd Uighur_Ime_Data;
	extern lan_keymap_Info_t Uighur_keymap_Info;
	cpy_data_2_data(i, &Uighur_keymap_Info, &Uighur_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÖÇÄÜ²ØÓï
#ifdef WD_TIBETAN
{
	extern lan_ddd Tibetan_Ime_Data;
	extern lan_keymap_Info_t Tibetan_keymap_Info;
	cpy_data_2_data(i, &Tibetan_keymap_Info, &Tibetan_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//×ÖÄ¸²ØÓï
#ifdef WD_TIBETAN_ABC
{
	extern lan_keymap_Info_t Tibetan_ABC_keymap_Info;
	cpy_data_2_data(i, &Tibetan_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÖÇÄÜÒÍÓï
#ifdef WD_YI
{
	extern lan_ddd Yi_Ime_Data;
	extern lan_keymap_Info_t Yi_keymap_Info;
	cpy_data_2_data(i, &Yi_keymap_Info, &Yi_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
	
#endif

	//ÐÙÑÀÀûÓï
#ifdef WD_HUNGARIAN
{
	extern lan_ddd Hungarian_Ime_Data;
	extern lan_keymap_Info_t Hungarian_keymap_Info;
	cpy_data_2_data(i, &Hungarian_keymap_Info, &Hungarian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//×ÖÄ¸ÃÉÓï
#ifdef WD_MONGAL_ABC
{
	extern lan_keymap_Info_t Mongal_ABC_keymap_Info;
	cpy_data_2_data(i, &Mongal_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}

#endif
// ÎÚ¶ú¶¼Óï
#ifdef WD_URDU
{
	extern lan_ddd Urdu_Ime_Data;
	extern lan_keymap_Info_t Urdu_keymap_Info;
	cpy_data_2_data(i, &Urdu_keymap_Info, &Urdu_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//ÖÇÄÜ¹þÈø¿ËÓï(ÖÐ¹ú)
#ifdef WD_KAZAK/*_ABC*///change by wanghui
{
	extern lan_ddd Kazak_Ime_Data;
	extern lan_keymap_Info_t Kazak_keymap_Info;
	cpy_data_2_data(i, &Kazak_keymap_Info, &Kazak_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
	
#endif
	
	//ÖÇÄÜ¹þÈø¿ËÓïwanghui
#ifdef WD_KAZAKH
{
	extern lan_ddd Kazakh_Ime_Data;
	extern lan_keymap_Info_t Kazakh_keymap_Info;
	cpy_data_2_data(i, &Kazakh_keymap_Info, &Kazakh_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
	
#endif

//ÖÇÄÜ³¯ÏÊÓï
#ifdef WD_CNKOREAN_ABC
{
	extern lan_keymap_Info_t Cnkorean_ABC_keymap_Info;
	cpy_data_2_data(i, &Cnkorean_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//´öÓï
#ifdef WD_CHNDAI_ABC
{
	extern lan_keymap_Info_t Chndai_ABC_keymap_Info;
	cpy_data_2_data(i, &Chndai_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÖÇÄÜ×³Óï
#ifdef WD_CHNZHUANG_ABC
{
	extern lan_keymap_Info_t Chnzhuang_ABC_keymap_Info;
	cpy_data_2_data(i, &Chnzhuang_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//ÖÇÄÜ¶±Óï
#ifdef WD_CHNDONG_ABC
{
	extern lan_keymap_Info_t Chndong_ABC_keymap_Info;
	cpy_data_2_data(i, &Chndong_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÖÇÄÜÃçÓï
#ifdef WD_MIAO//wanghui
{
	extern lan_ddd Miao_Ime_Data;
	extern lan_keymap_Info_t Miao_keymap_Info;
	cpy_data_2_data(i, &Miao_keymap_Info, &Miao_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

// ÍùÏÂÊÇÍâÎÄ 
//ÖÇÄÜÓ¢Óï
#ifdef WD_ENGLISH
{
	extern lan_ddd English_Ime_Data;
	extern const lan_keymap_Info_t English_keymap_Info;
	cpy_data_2_data(i, &English_keymap_Info, &English_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
//ÖÇÄÜÁ¢ÌÕÍðÓï
#ifdef WD_LITHUANIAN
{
	extern lan_ddd Lithuanian_Ime_Data;
	extern lan_keymap_Info_t Lithuanian_keymap_Info;
	cpy_data_2_data(i, &Lithuanian_keymap_Info, &Lithuanian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
//ÖÇÄÜ·¨Óï
#ifdef WD_FRENCH
{
	extern lan_ddd French_Ime_Data;
	extern const lan_keymap_Info_t French_keymap_Info;
	cpy_data_2_data(i, &French_keymap_Info, &French_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
//ºÉÀ¼Óï
#ifdef WD_DUTCH
{
	extern lan_ddd Dutch_Ime_Data;
	extern lan_keymap_Info_t Dutch_keymap_Info;
	cpy_data_2_data(i, &Dutch_keymap_Info, &Dutch_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
//Ï£À°Óï
#ifdef WD_GREEK
{
	extern lan_ddd Greek_Ime_Data;
	extern lan_keymap_Info_t Greek_keymap_Info;
	cpy_data_2_data(i, &Greek_keymap_Info, &Greek_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}

#endif
//ÖÇÄÜµÂÓï
#ifdef WD_GERMAN
{
	extern lan_ddd German_Ime_Data;
	extern lan_keymap_Info_t German_keymap_Info;
	cpy_data_2_data(i, &German_keymap_Info, &German_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//ÖÇÄÜÍÁ¶úÆäÓï
#ifdef WD_TURKISH
{
	extern lan_ddd Turkish_Ime_Data;
	extern lan_keymap_Info_t Turkish_keymap_Info;
	cpy_data_2_data(i, &Turkish_keymap_Info, &Turkish_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}

#endif

//ÖÇÄÜÒâ´óÀûÓï
#ifdef WD_ITALIAN
{
	extern lan_ddd Italian_Ime_Data;
	extern const lan_keymap_Info_t Italian_keymap_Info;
	cpy_data_2_data(i, &Italian_keymap_Info, &Italian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}

#endif

//ÖÇÄÜµ¤Âó
#ifdef WD_DANISH
{
	extern lan_ddd Danish_Ime_Data;
	extern lan_keymap_Info_t Danish_keymap_Info;
	cpy_data_2_data(i, &Danish_keymap_Info, &Danish_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
//ÖÇÄÜÈðµä
#ifdef WD_SWEDISH
{
	extern lan_ddd Swedish_Ime_Data;
	extern lan_keymap_Info_t Swedish_keymap_Info;
	cpy_data_2_data(i, &Swedish_keymap_Info, &Swedish_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÖÇÄÜÅ²Íþ
#ifdef WD_NORWEGIAN
{
	extern lan_ddd Norwegian_Ime_Data;
	extern lan_keymap_Info_t Norwegian_keymap_Info;
	cpy_data_2_data(i, &Norwegian_keymap_Info, &Norwegian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÖÇÄÜÎ÷°àÑÀÓï
#ifdef WD_SPANISH
{
	extern lan_ddd Spanish_Ime_Data;
	extern lan_keymap_Info_t Spanish_keymap_Info;
	cpy_data_2_data(i, &Spanish_keymap_Info, &Spanish_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
//½Ý¿ËÓï
#ifdef WD_CZECH
{
	extern lan_ddd Czech_Ime_Data;
	extern lan_keymap_Info_t Czech_keymap_Info;
	cpy_data_2_data(i, &Czech_keymap_Info, &Czech_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
//²¨À¼Óï
#ifdef WD_POLISH
{
	extern lan_ddd Polish_Ime_Data;
	extern lan_keymap_Info_t Polish_keymap_Info;
	cpy_data_2_data(i, &Polish_keymap_Info, &Polish_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}

#endif

//ÖÇÄÜÓ¡ÄáÓï
#ifdef WD_INDONESIAN
{
	extern lan_ddd Indonesian_Ime_Data;
	extern lan_keymap_Info_t Indonesian_keymap_Info;
	cpy_data_2_data(i, &Indonesian_keymap_Info, &Indonesian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}

#endif

//ÖÇÄÜÆÏÌÑÑÀÓï
#ifdef WD_PORTUGUESE
{
	extern lan_ddd Portuguese_Ime_Data;
	extern lan_keymap_Info_t Portuguese_keymap_Info;
	cpy_data_2_data(i, &Portuguese_keymap_Info, &Portuguese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//²¨Ë¹Óï
#ifdef WD_PERSIAN
{
	extern lan_ddd Persian_Ime_Data;
	extern lan_keymap_Info_t Persian_keymap_Info;
	cpy_data_2_data(i, &Persian_keymap_Info, &Persian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif



//ÖÇÄÜ¼ÓÌ©Â¡Óï
#ifdef WD_CATALAN
{
	extern lan_ddd Catalan_Ime_Data;
	extern lan_keymap_Info_t Catalan_keymap_Info;
	cpy_data_2_data(i, &Catalan_keymap_Info, &Catalan_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)	goto end;
}
#endif

// #endif
//ÖÇÄÜ¶íÓï
#ifdef WD_RUSSIAN
{
	extern lan_ddd Russian_Ime_Data;
	extern lan_keymap_Info_t Russian_keymap_Info;
	cpy_data_2_data(i, &Russian_keymap_Info, &Russian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//ÖÇÄÜÌ©¹úÓï
#ifdef WD_THAI
{
	extern lan_ddd Thai_Ime_Data;
	extern lan_keymap_Info_t Thai_keymap_Info;
	cpy_data_2_data(i, &Thai_keymap_Info, &Thai_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÎÚ¿ËÀ¼Óï
#ifdef WD_UKRAINIAN
{
	extern lan_ddd Ukrainian_Ime_Data;
	extern lan_keymap_Info_t Ukrainian_keymap_Info;
	cpy_data_2_data(i, &Ukrainian_keymap_Info, &Ukrainian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//ÖÇÄÜÂíÀ´Óï
#ifdef WD_MALAY
{
	extern lan_ddd Malay_Ime_Data;
	extern lan_keymap_Info_t Malay_keymap_Info;
	cpy_data_2_data(i, &Malay_keymap_Info, &Malay_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÖÇÄÜÔ½ÄÏÓï 
#ifdef WD_VIETNAMESE
{
	extern lan_ddd Vietnamese_Ime_Data;
	extern lan_keymap_Info_t Vietnamese_keymap_Info;
	cpy_data_2_data(i, &Vietnamese_keymap_Info, &Vietnamese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÖÇÄÜÀÏÎÎÓï
#ifdef WD_LAO
{
	extern lan_ddd Lao_Ime_Data;
	extern lan_keymap_Info_t Lao_keymap_Info;
	cpy_data_2_data(i, &Lao_keymap_Info, &Lao_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÖÇÄÜ°¢À­²®Óï
#ifdef WD_ARABIC
{
	extern lan_ddd Arabic_Ime_Data;
	extern const lan_keymap_Info_t Arabic_keymap_Info;
	cpy_data_2_data(i, &Arabic_keymap_Info, &Arabic_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//ÖÇÄÜÓ¡µØÓï

#ifdef WD_HINDI
{
	extern lan_ddd Hindi_Ime_Data;
	extern lan_keymap_Info_t Hindi_keymap_Info;
	cpy_data_2_data(i, &Hindi_keymap_Info, &Hindi_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ÖÇÄÜËû¼ÓÂ»Óï

#ifdef WD_TAGALOG
{
	extern lan_ddd Tagalog_Ime_Data;
	extern lan_keymap_Info_t Tagalog_keymap_Info;
	cpy_data_2_data(i, &Tagalog_keymap_Info, &Tagalog_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif



//ÖÇÄÜÏ£²®À³Óï

#ifdef WD_HEBREW
{
	extern lan_ddd Hebrew_Ime_Data;
	extern lan_keymap_Info_t Hebrew_keymap_Info;
	cpy_data_2_data(i, &Hebrew_keymap_Info, &Hebrew_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_ROMANIAN			//ÂÞÂíÄáÑÇÓï
{
	extern lan_ddd Romanian_Ime_Data;
	extern lan_keymap_Info_t Romanian_keymap_Info;
	cpy_data_2_data(i, &Romanian_keymap_Info, &Romanian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_BENGALI			//ÃÏ¼ÓÀ­Óï
{
	extern lan_ddd Bengali_Ime_Data;
	extern lan_keymap_Info_t Bengali_keymap_Info;
	cpy_data_2_data(i, &Bengali_keymap_Info, &Bengali_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_TELUGU			//Ì©Â¬¹ÌÓïÊäÈë·¨
{
	extern lan_ddd Telugu_Ime_Data;
	extern lan_keymap_Info_t Telugu_keymap_Info;
	cpy_data_2_data(i, &Telugu_keymap_Info, &Telugu_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_BULGARIAN		/* ±£¼ÓÀûÑÇÓï				*/
{
	extern lan_ddd Bulgarian_Ime_Data;
	extern lan_keymap_Info_t Bulgarian_keymap_Info;
	cpy_data_2_data(i, &Bulgarian_keymap_Info, &Bulgarian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_SLOVENIAN		//Ë¹ÂåÎÄÄáÑÇÓïÊäÈë·¨
{
	extern lan_ddd Slovenian_Ime_Data;
	extern lan_keymap_Info_t Slovenian_keymap_Info;
	cpy_data_2_data(i, &Slovenian_keymap_Info, &Slovenian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_KHMER
{
	extern lan_ddd Khmer_Ime_Data;
	extern lan_keymap_Info_t Khmer_keymap_Info;
	cpy_data_2_data(i, &Khmer_keymap_Info, &Khmer_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_BURMESE
{
	extern lan_ddd Burmese_Ime_Data;
	extern lan_keymap_Info_t Burmese_keymap_Info;
	cpy_data_2_data(i, &Burmese_keymap_Info, &Burmese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_NEPALI
{
	extern lan_ddd Nepali_Ime_Data;
	extern lan_keymap_Info_t Nepali_keymap_Info;
	cpy_data_2_data(i, &Nepali_keymap_Info, &Nepali_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_GEORGIAN
{
	extern lan_ddd Georgian_Ime_Data;
	extern lan_keymap_Info_t Georgian_keymap_Info;
	cpy_data_2_data(i, &Georgian_keymap_Info, &Georgian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_ESTONIAN
{
	extern lan_ddd Estonian_Ime_Data;
	extern lan_keymap_Info_t Estonian_keymap_Info;
	cpy_data_2_data(i, &Estonian_keymap_Info, &Estonian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif	
#ifdef WD_ICELANDIC
{
	extern lan_ddd Icelandic_Ime_Data;
	extern lan_keymap_Info_t Icelandic_keymap_Info;
	cpy_data_2_data(i, &Icelandic_keymap_Info, &Icelandic_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_FINNISH
{
	extern lan_ddd Finnish_Ime_Data;
	extern lan_keymap_Info_t Finnish_keymap_Info;
	cpy_data_2_data(i, &Finnish_keymap_Info, &Finnish_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_BELARUSIAN
{
	extern lan_ddd Belarusian_Ime_Data;
	extern lan_keymap_Info_t Belarusian_keymap_Info;
	cpy_data_2_data(i, &Belarusian_keymap_Info, &Belarusian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_SERBIAN
{
	extern lan_ddd Serbian_Ime_Data;
	extern lan_keymap_Info_t Serbian_keymap_Info;
	cpy_data_2_data(i, &Serbian_keymap_Info, &Serbian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_SERBIANC
{
	extern lan_ddd Serbianc_Ime_Data;
	extern lan_keymap_Info_t Serbianc_keymap_Info;
	cpy_data_2_data(i, &Serbianc_keymap_Info, &Serbianc_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_SLOVAK
{
	extern lan_ddd Slovak_Ime_Data;
	extern lan_keymap_Info_t Slovak_keymap_Info;
	cpy_data_2_data(i, &Slovak_keymap_Info, &Slovak_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_TAMIL
{
	extern lan_ddd Tamil_Ime_Data;
	extern lan_keymap_Info_t Tamil_keymap_Info;
	cpy_data_2_data(i, &Tamil_keymap_Info, &Tamil_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_TURKMEN
{
	extern lan_ddd Turkmen_Ime_Data;
	extern lan_keymap_Info_t Turkmen_keymap_Info;
	cpy_data_2_data(i, &Turkmen_keymap_Info, &Turkmen_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_CHOSEN
{
	extern lan_ddd Chosen_Ime_Data;
	extern lan_keymap_Info_t Chosen_keymap_Info;
	cpy_data_2_data(i, &Chosen_keymap_Info, &Chosen_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_MONGOLIAN
{
	extern lan_ddd Mongolian_Ime_Data;
	extern lan_keymap_Info_t Mongolian_keymap_Info;
	cpy_data_2_data(i, &Mongolian_keymap_Info, &Mongolian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_SWAHILI
{
	extern lan_ddd Swahili_Ime_Data;
	extern lan_keymap_Info_t Swahili_keymap_Info;
	cpy_data_2_data(i, &Swahili_keymap_Info, &Swahili_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_MARATHI
{
	extern lan_ddd Marathi_Ime_Data;
	extern lan_keymap_Info_t Marathi_keymap_Info;
	cpy_data_2_data(i, &Marathi_keymap_Info, &Marathi_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_KANNADA
{
	extern lan_ddd Kannada_Ime_Data;
	extern lan_keymap_Info_t Kannada_keymap_Info;
	cpy_data_2_data(i, &Kannada_keymap_Info, &Kannada_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_MALAYALAM
{
	extern lan_ddd Malayalam_Ime_Data;
	extern lan_keymap_Info_t Malayalam_keymap_Info;
	cpy_data_2_data(i, &Malayalam_keymap_Info, &Malayalam_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_ASSAMESE
{
	extern lan_ddd Assamese_Ime_Data;
	extern lan_keymap_Info_t Assamese_keymap_Info;
	cpy_data_2_data(i, &Assamese_keymap_Info, &Assamese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_PUNJABI //ÅÔÕÚÆÕ
{
	extern lan_ddd Punjabi_Ime_Data;
	extern lan_keymap_Info_t Punjabi_keymap_Info;
	cpy_data_2_data(i, &Punjabi_keymap_Info, &Punjabi_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_MAITHILI//ÂõµÙÀû
{
	extern lan_ddd Maithili_Ime_Data;
	extern lan_keymap_Info_t Maithili_keymap_Info;
	cpy_data_2_data(i, &Maithili_keymap_Info, &Maithili_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_BOSNIAN//²¨Ë¹ÄáÑÇ
{
	extern lan_ddd Bosnian_Ime_Data;
	extern lan_keymap_Info_t Bosnian_keymap_Info;
	cpy_data_2_data(i, &Bosnian_keymap_Info, &Bosnian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_CROATIAN//¿ËÂÞµØÑÇ
{
	extern lan_ddd Croatian_Ime_Data;
	extern lan_keymap_Info_t Croatian_keymap_Info;
	cpy_data_2_data(i, &Croatian_keymap_Info, &Croatian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_LATVIAN//À­ÍÑÎ¬ÑÇ
{
	extern lan_ddd Latvian_Ime_Data;
	extern lan_keymap_Info_t Latvian_keymap_Info;
	cpy_data_2_data(i, &Latvian_keymap_Info, &Latvian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_IRISH//°®¶ûÀ¼
{
	extern lan_ddd Irish_Ime_Data;
	extern lan_keymap_Info_t Irish_keymap_Info;
	cpy_data_2_data(i, &Irish_keymap_Info, &Irish_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_GUJARATI   //¹Å¼ªÀ­ÌØ
{
	extern lan_ddd Gujarati_Ime_Data;
	extern lan_keymap_Info_t Gujarati_keymap_Info;
	cpy_data_2_data(i, &Gujarati_keymap_Info, &Gujarati_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_HAUSA   //ºÀÈø
{
	extern lan_ddd Hausa_Ime_Data;
	extern lan_keymap_Info_t Hausa_keymap_Info;
	cpy_data_2_data(i, &Hausa_keymap_Info, &Hausa_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_IGBO   //ÒÁ²©
{
	extern lan_ddd Igbo_Ime_Data;
	extern lan_keymap_Info_t Igbo_keymap_Info;
	cpy_data_2_data(i, &Igbo_keymap_Info, &Igbo_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_AZERBAIJANI   //°¢Èû°Ý½®
{
	extern lan_ddd Azerbaijani_Ime_Data;
	extern lan_keymap_Info_t Azerbaijani_keymap_Info;
	cpy_data_2_data(i, &Azerbaijani_keymap_Info, &Azerbaijani_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//µÃµ½Êµ¼ÊÊäÈë·¨µÄ×ÜÊý
end:	
	copy_languageInfo_2_inner(languageInfo,i);      /*lint !e718 !e746 !e628*/
	SCI_MEMCPY(exist,existTemp,64*sizeof(WD_UINT8));
}
/*lint -restore*/

void cpy_data_2_data(WD_UINT8 i, lan_keymap_Info_t* data, void* Ime_data)
{
	languageInfo[i].landdd = Ime_data;
	
	languageInfo[i].cur_language = data;
	languageInfo[i].imeType = data->imeType;
	SCI_MEMCPY(languageInfo[i].imeString, data->imeString, sizeof(languageInfo[i].imeString));
}
/*lint -restore*/
/*lint -restore*/


