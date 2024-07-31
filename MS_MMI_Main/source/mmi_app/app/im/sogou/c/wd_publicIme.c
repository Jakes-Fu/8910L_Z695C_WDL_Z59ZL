/*lint -save -e91*/
/*lint -save -e774*/
#include "wd_def.h"
#include "sg_common_public.h"


#include "os_api.h"     //ʹ��sci_memcpy����ԭmemcpy������pclint�澯


extern void copy_languageInfo_2_inner(void* languageInfo,WD_UINT8 imecount);

	/******************************* �������ݱ� ********************************/
#if defined(DIGIT_KEYS)

const WD_UINT16 caoZuoShuJuBiao[9][MAX_KEY_COUNT-KEY_BASE]={
/* ======����======          0      1      2      3      4      5      6      7      8      9      *       #     C     OK     LSK    RSK     UP   DOWN   LEFT  RIGHT     L0    L1     L2     L3     L4      L5     L6    L7     L8     L9      L*    L#     LC     LOK   LLSK   LRSK   LUP    LDOWN  LLEFT LRIGHT  */
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
/*======����======           A      B      C      D      E      F      G      H      I      J      K      L      M      N      O      P      Q      R      S      T      U      V      W      X      Y      Z    �л�   �ָ�   ����   ɾ��    OK     �ո�   �س�   UP    DOWN   LEFT   RIGHT   LST    RST    ��С   FN    Shift   Alt   Ctrl    LA     LB     LC     LD     LE     LF     LG     LH     LI     LJ     LK     LL     LM     LN     LO     LP     LQ     LR     LS     LT     LU     LV     LW     LX     LY     LZ    L�л� L�ָ�   L���� Lɾ��   LOK    L�ո�  L�س�   LUP   LDOWN  LLEFT LRIGHT  LLST   LRST   L��С  LFN    LShift  LAlt  LCtrl  */
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

#elif defined(INTEVQWERTY)//OTHER_KEYS����26��+����9��

const WD_UINT16 caoZuoShuJuBiao[9][MAX_KEY_COUNT-KEY_BASE]={
	/* ======����======          0      1      2      3      4      5      6      7      8      9      *      #      A      B      C      D      E      F      G      H      I      J      K      L      M      N      O      P      Q      R      S      T      U      V      W      X      Y      Z    �л�   �ָ�   ����   ɾ��    OK     �ո�   �س�   UP    DOWN   LEFT   RIGHT   LST    RST    ��С   FN   Shift   Alt     Ctrl    L0     L1     L2     L3     L4     L5     L6     L7     L8     L9     L*     L#     LA     LB     LC     LD     LE     LF     LG     LH     LI     LJ     LK     LL     LM     LN     LO     LP     LQ     LR     LS     LT     LU     LV     LW     LX     LY     LZ   L�л�  L�ָ�   L����  Lɾ��   LOK   L�ո�  L�س�   LUP  LDOWN   LLEFT LRIGHT  LLST   LRST   L��С  LFN   LShift  LAlt   LCtrl  */
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
	/* ======����======          0      1      2      3      4      5      6      7      8      9      *      #      A      B      C      D      E      F      G      H      I      J      K      L      M      N      O      P      Q      R      S      T      U      V      W      X      Y      Z    �л�   �ָ�   ����   ɾ��    OK     �ո�   �س�   UP    DOWN   LEFT   RIGHT   LST    RST    ��С   FN   Shift   Alt     Ctrl    L0     L1     L2     L3     L4     L5     L6     L7     L8     L9     L*     L#     LA     LB     LC     LD     LE     LF     LG     LH     LI     LJ     LK     LL     LM     LN     LO     LP     LQ     LR     LS     LT     LU     LV     LW     LX     LY     LZ   L�л�  L�ָ�   L����  Lɾ��   LOK   L�ո�  L�س�   LUP  LDOWN   LLEFT LRIGHT  LLST   LRST   L��С  LFN   LShift  LAlt   LCtrl  */
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

///////////////��������/////////////////////////
void cpy_data_2_data(WD_UINT8 i, lan_keymap_Info_t* data, void* Ime_data);
/////////////////������������/////////////////////////


///////////////���뷽ʽ//////////////////
#define USE_UNICODE			//���ע�͵����У���ΪGBK�룬��ΪUNICODE
////////////////////���Կ���////////////////////////

/////�ͻ�ʹ��/////////////////////////

	#define WD_DIGIT			//�������뷨
////����
	#define WD_CN_CPINYIN			//��ƴ���뷨
    #define WD_CN_FULLPINYIN		//ȫ��ĸƴ�����뷨
	#define WD_CN_BIHUA				//�ʻ����뷨

#ifdef IM_TRAD_CHINESE_SUPPORT	
 	#define WD_CN_BIHUAF			//����ʻ����뷨
 	#define	WD_CN_ZHUYIN			//ע�����뷨
 	#define WD_CN_FULLZHUYIN		//ȫ��ĸ��ע��
#endif

//����
	//#define WD_WDI_ABC
	#define WD_ENGLISH_ABC				//Ӣ����ĸ���뷨
// 	#define	WD_LATIN_ABC				//������ϵ����ĸ���뷨
#ifdef IM_ARABIC_SUPPORT
 	#define WD_ARABIC_ABC				//����������ĸ���뷨
#endif 	

#ifdef IM_RUSSIAN_SUPPORT
 	#define WD_RUSSIAN_ABC				//������ĸ���뷨
#endif

#ifdef IM_FRENCH_SUPPORT
 	#define WD_FRENCH_ABC				//������ĸ���뷨
#endif

#ifdef IM_VIETNAMESE_SUPPORT
 	#define WD_VIETNAMESE_ABC			//Խ������ĸ���뷨
#endif 	

#ifdef IM_THAI_SUPPORT
 	#define WD_THAI_ABC					//̩����ĸ���뷨
#endif 	

#ifdef IM_INDONESIAN_SUPPORT
 	#define WD_INDONESIAN_ABC			//ӡ����������ĸ��ĸ���뷨
#endif 	

#ifdef IM_PORTUGUESE_SUPPORT
 	#define WD_PORTUGUESE_ABC			//����������ĸ���뷨
#endif 	

#ifdef IM_GERMAN_SUPPORT
 	#define WD_GERMAN_ABC				//������ĸ���뷨
#endif 	

#ifdef IM_SPANISH_SUPPORT
 	#define WD_SPANISH_ABC				//����������ĸ���뷨
#endif 	

#ifdef IM_MALAY_SUPPORT
 	#define WD_MALAY_ABC				//��������ĸ���뷨
#endif 	

#ifdef IM_PERSIAN_SUPPORT
 	#define WD_PERSIAN_ABC				//��˹����ĸ���뷨
#endif 	

#ifdef IM_TURKISH_SUPPORT
 	#define WD_TURKISH_ABC				//����������ĸ���뷨
#endif 	

#ifdef IM_ITALIAN_SUPPORT
 	#define WD_ITALIAN_ABC				//���������ĸ���뷨
#endif 	

#ifdef IM_HINDI_SUPPORT
 	#define WD_HINDI_ABC				//����ӡ������ĸ���뷨
#endif

#ifdef IM_CZECH_SUPPORT
 	#define WD_CZECH_ABC				//��ĸ�ݿ���
#endif 	

#ifdef IM_DUTCH_SUPPORT
 	#define WD_DUTCH_ABC				//��ĸ������
#endif 	

#ifdef IM_HEBREW_SUPPORT
 	#define WD_HEBREW_ABC				//��ĸϣ������
#endif 	

#ifdef IM_POLISH_SUPPORT
 	#define WD_POLISH_ABC				//��ĸ������
#endif 	

#ifdef IM_UYGHUR_SUPPORT
 	#define WD_UIGHUR_ABC				//��ĸά�����
#endif 	

#ifdef IM_SWEDISH_SUPPORT
 	#define WD_SWEDISH_ABC				//��ĸ�����
#endif 	

#ifdef IM_FINNISH_SUPPORT
 	#define WD_FINNISH_ABC				//��ĸ������
#endif 	

#ifdef IM_NORWEGIAN_SUPPORT
 	#define WD_NORWEGIAN_ABC			//��ĸŲ����
#endif 	

#ifdef IM_DANISH_SUPPORT
 	#define WD_DANISH_ABC				//��ĸ������
#endif 	

#ifdef IM_LAO_SUPPORT
 	#define WD_LAO_ABC					//��ĸ������
#endif 	

#ifdef IM_URDU_SUPPORT
 	#define WD_URDU_ABC					//��ĸ�ڶ�����
#endif 	

#ifdef IM_KAZAK_SUPPORT
 	#define WD_KAZAK_ABC				//��ĸ�����ˣ��й�����
#endif 	

#ifdef IM_ICELANDIC_SUPPORT
 	#define WD_ICELANDIC_ABC			//��ĸ������
#endif 	

#ifdef IM_SLOVAK_SUPPORT
 	#define WD_SLOVAK_ABC				//��ĸ˹�工����
#endif 	

#ifdef IM_ESTONIAN_SUPPORT
 	#define WD_ESTONIAN_ABC				//��ĸ��ɳ������
#endif 	

#ifdef IM_BELARUSIAN_SUPPORT
 	#define WD_BELARUSIAN_ABC			//��ĸ�׶���˹��
#endif 	

#ifdef IM_LATVIAN_SUPPORT
 	#define WD_LATVIAN_ABC				//��ĸ����ά����// 
#endif 	

#ifdef IM_SLOVENIAN_SUPPORT
 	#define WD_SLOVENIAN_ABC			//��ĸ˹����������
#endif 	

#ifdef IM_CROATIAN_SUPPORT
 	#define WD_CROATIAN_ABC				//��ĸ���޵�����
#endif 	

#ifdef IM_SERBIAN_SUPPORT
 	#define WD_SERBIAN_ABC				//��ĸ����ά����
#endif 	

#ifdef IM_HAUSA_SUPPORT
 	#define WD_HAUSA_ABC				//��ĸ������
#endif 	

#ifdef IM_IGBO_SUPPORT
 	#define WD_IGBO_ABC					//��ĸ������
#endif 	

#ifdef IM_TURKMEN_SUPPORT
 	#define WD_TURKMEN_ABC				//��ĸ��������
#endif 	

#ifdef IM_SWAHILI_SUPPORT
 	#define WD_SWAHILI_ABC				//��ĸ˹��ϣ����
#endif 	

#ifdef IM_HUNGARIAN_SUPPORT
 	#define WD_HUNGARIAN_ABC			//��ĸ��������
#endif 	

#ifdef IM_GREEK_SUPPORT
 	#define WD_GREEK_ABC				//��ĸϣ����
#endif 	

#ifdef IM_KAZAKH_SUPPORT
 	#define WD_KAZAKH_ABC				//��ĸ��������
#endif 	

#ifdef IM_ROMANIAN_SUPPORT
 	#define WD_ROMANIAN_ABC				//��ĸ����������
#endif 	

#ifdef IM_TAGALOG_SUPPORT
 	#define WD_TAGALOG_ABC				//��ĸ����»��
#endif 	

#ifdef IM_BOSNIAN_SUPPORT
 	#define WD_BOSNIAN_ABC				//��ĸ��˹������
#endif 	

#ifdef IM_TAMIL_SUPPORT
    #define WD_TAMIL_ABC				//��ĸ̩�׶���
#endif 	

#ifdef IM_CYRILLIC_SUPPORT
 	#define WD_CYRILLIC_ABC				//��ĸ�������ϵ
#endif 	

#ifdef IM_SERBIANC_SUPPORT
  #define WD_SERBIANC_ABC				//��ĸ����ά��(�����)
#endif 	

#ifdef IM_BENGALI_SUPPORT
 	#define WD_BENGALI_ABC		//��ĸ�ϼ�����   qls
#endif 	

#ifdef IM_KHMER_SUPPORT
	#define WD_KHMER_ABC		//��ĸ������  qls
#endif 	

#ifdef IM_NEPALI_SUPPORT
 	#define WD_NEPALI_ABC		//��ĸ�Ჴ����  qls
#endif 	

#ifdef IM_UKRAINIAN_SUPPORT
 	#define WD_UKRAINIAN_ABC	//��ĸ�ڿ�����
#endif 	

#ifdef IM_BULGARIAN_SUPPORT
 	#define WD_BULGARIAN_ABC	//��ĸ����������
#endif 	

#ifdef IM_CHOSEN_SUPPORT
 	#define WD_CHOSEN_ABC		//��ĸ������
#endif 	

#ifdef IM_MIAO_SUPPORT
 	#define	WD_MIAO_ABC			//��ĸ����
#endif 	

#ifdef IM_MONGOLIAN_TRADITIONAL_SUPPORT
 	#define WD_MONGOLIAN_TRADITIONAL_ABC	//��ͳ��ĸ�ɹ���
#endif 	

#ifdef IM_MONGOLIAN_SUPPORT
 	#define	WD_MONGOLIAN_ABC	//��ĸ�ɹ���
#endif 	

#ifdef IM_GEORGIAN_SUPPORT
 	#define WD_GEORGIAN_ABC	//��ĸ��³����
#endif 	

#ifdef IM_TELUGU_SUPPORT
  #define WD_TELUGU_ABC		//��ĸ̩¬����	
#endif 	

#ifdef IM_BURMESE_SUPPORT
 	#define WD_BURMESE_ABC			  //��ĸ�����					
#endif 	

#ifdef IM_MARATHI_SUPPORT
 	#define	WD_MARATHI_ABC			  //��ĸ��������
#endif 	

#ifdef IM_KANNADA_SUPPORT
 	#define	WD_KANNADA_ABC			  //��ĸ���ɴ���
#endif 	

#ifdef IM_ASSAMESE_SUPPORT
 	#define	WD_ASSAMESE_ABC		  //��ĸ����ķ��
#endif 	

#ifdef IM_MALAYALAM_SUPPORT
 	#define	WD_MALAYALAM_ABC		  //��ĸ��������ķ��
#endif 	

#ifdef IM_PUNJABI_SUPPORT
 	#define	WD_PUNJABI_ABC			  //��ĸ��������
#endif 	

#ifdef IM_MAITHILI_SUPPORT
 	#define	WD_MAITHILI_ABC		  //��ĸ��������
#endif 	

#ifdef IM_GUJARATI_SUPPORT
 	#define	WD_GUJARATI_ABC		  //��ĸ�ż�������
#endif 	

#ifdef IM_IRISH_SUPPORT
 	#define	WD_IRISH_ABC			  //��ĸ��������
#endif 	

#ifdef IM_CATALAN_SUPPORT
 	#define	WD_CATALAN_ABC			  //��ĸ��̩¡��
#endif 	

#ifdef IM_AZERBAIJANI_SUPPORT
 	#define	WD_AZERBAIJANI_ABC		  //��ĸ�����ݽ���
#endif 	

#ifdef IM_PASHTO_SUPPORT
		#define WD_PASHTO_ABC				//��ĸ��ʲͼ��
#endif 	

//#define WD_JAVANESE_ABC //צ����
//#define WD_MOROCCAN_ABC //Ħ�����
//#define WD_TUNISIAN_ABC//ͻ��˹��
//#define WD_SUDANESE_ABC     //�յ���
//#define WD_PAKISTAN_ABC    //�ͻ�˹̹��
#ifdef IM_SINHALESE_SUPPORT
	#define WD_SINHALESE_ABC  //ɮ٤����
#endif	
//#define WD_MACEDONIAN_ABC    //�������
//#define WD_KINYARWANDA_ABC  //¬������
//#define WD_ZULU_ABC    		    //��³��
//#define WD_TATAR_ABC    	    //������
//#define WD_YEMENI_ABC     //Ҳ����
//#define WD_BHUTANI_ABC      //������
//#define WD_FIJIAN_ABC    	  //쳼���
//#define WD_KONGO_ABC    		   //�չ���
//#define WD_SYRIAC_ABC       //��������
//#define WD_ALGERIAN_ABC      //������������
//#define WD_MALTESE_ABC       //�������
//#define WD_WELSH_ABC    	  //����ʿ��
//#define WD_MALDIVIAN_ABC      //���������

//����
// 	#define WD_HUNGARIAN			//������������
	#define WD_ENGLISH				//Ӣ���������뷨
#ifdef IM_FRENCH_SUPPORT
 	#define WD_FRENCH				//�����������뷨
#endif 	

#ifdef IM_HEBREW_SUPPORT
  #define WD_HEBREW				//ϣ�������������뷨
#endif

#ifdef IM_RUSSIAN_SUPPORT
   #define WD_RUSSIAN				//�����������뷨
#endif

#ifdef IM_ARABIC_SUPPORT
 	#define WD_ARABIC				//���������������뷨
#endif 	

#ifdef IM_THAI_SUPPORT
  #define	WD_THAI					//̩���������뷨
#endif
  
#ifdef IM_VIETNAMESE_SUPPORT
   #define WD_VIETNAMESE			//Խ�����������뷨
#endif
   
#ifdef IM_ITALIAN_SUPPORT
 	#define WD_ITALIAN				//�������
#endif 	

#ifdef IM_DANISH_SUPPORT
        #define WD_DANISH				//�������������뷨
#endif

#ifdef IM_SWEDISH_SUPPORT
        #define	WD_SWEDISH				//���������
#endif

#ifdef IM_NORWEGIAN_SUPPORT
        #define WD_NORWEGIAN			//����Ų����
#endif

#ifdef IM_PORTUGUESE_SUPPORT
 	#define WD_PORTUGUESE			//�����������뷨
#endif

#ifdef IM_SPANISH_SUPPORT
 	#define WD_SPANISH				//�����������뷨
#endif

#ifdef IM_HINDI_SUPPORT
 	#define WD_HINDI				//����ӡ����
#endif

#ifdef IM_MALAY_SUPPORT
 	#define WD_MALAY				//����������
#endif

#ifdef IM_KOREAN_SUPPORT
	#define WD_KOREAN				//�������뷨
#endif

#ifdef IM_JAPANESE_SUPPORT
	#define WD_JAPANESE				//�������뷨
#endif

#ifdef IM_CZECH_SUPPORT
	#define WD_CZECH				//�ݿ���
#endif

#ifdef IM_POLISH_SUPPORT
 	#define WD_POLISH				//������                
#endif

#ifdef IM_TURKISH_SUPPORT
 	#define WD_TURKISH				//������������
#endif

#ifdef IM_GERMAN_SUPPORT
 	#define WD_GERMAN				//���ܵ���
#endif

#ifdef IM_PERSIAN_SUPPORT
	#define WD_PERSIAN				//��˹��
#endif

#ifdef IM_LAO_SUPPORT
	#define WD_LAO				    //����
#endif

#ifdef IM_TAGALOG_SUPPORT
  #define WD_TAGALOG			    //����»��
#endif

#ifdef IM_INDONESIAN_SUPPORT
 	 #define WD_INDONESIAN			//ӡ����������
#endif

#ifdef IM_UKRAINIANH_SUPPORT
	#define WD_UKRAINIAN			//�ڿ�����
#endif

#ifdef IM_DUTCH_SUPPORT
 	#define WD_DUTCH				//���ܺ�����
#endif

#ifdef IM_GREEK_SUPPORT
	#define WD_GREEK				//����ϣ����
#endif

#ifdef IM_URDU_SUPPORT
	#define WD_URDU					//�ڶ�����
#endif

#ifdef IM_KAZAKH_SUPPORT
  #define WD_KAZAKH				//���ܹ�������	
#endif

#ifdef IM_BENGALI_SUPPORT
  #define  WD_BENGALI    //�����ϼ����� 
#endif

#ifdef IM_KHMER_SUPPORT
	#define WD_KHMER			//���ܸ�����  qls
#endif

#ifdef IM_MYANMAR_SUPPORT
	#define WD_BURMESE			//���������
#endif

#ifdef IM_NEPALI_SUPPORT
 	#define WD_NEPALI			//�����Ჴ����  qls
#endif

#ifdef IM_ROMANIAN_SUPPORT
 	#define WD_ROMANIAN
#endif

#ifdef IM_SLOVENIAN_SUPPORT
 	#define WD_SLOVENIAN	
#endif

#ifdef IM_TELUGU_SUPPORT
 	#define WD_TELUGU		//����̩¬����
#endif

#ifdef IM_BULGARIAN_SUPPORT
 	#define WD_BULGARIAN	//���ܱ���������
#endif

#ifdef IM_GEORGIAN_SUPPORT
 	#define WD_GEORGIAN		//���ܸ�³����
#endif

#ifdef IM_ESTONIAN_SUPPORT
 	#define WD_ESTONIAN		//���ܰ�ɳ������
#endif

#ifdef IM_ICELANDIC_SUPPORT
 	#define WD_ICELANDIC	//���ܱ�����
#endif

#ifdef IM_FINNISH_SUPPORT
 	#define	WD_FINNISH		//���ܷ�����
#endif

#ifdef IM_BELARUSIAN_SUPPORT
 	#define	WD_BELARUSIAN	//���ܰ׶���˹��
#endif

#ifdef IM_SERBIAN_SUPPORT
 	#define WD_SERBIAN		//��������ά����
#endif

#ifdef IM_SLOVAK_SUPPORT
 	#define	WD_SLOVAK		//����˹�工����
#endif

#ifdef IM_TAMIL_SUPPORT
	#define WD_TAMIL		//����̩�׶���
#endif

#ifdef IM_TURKMEN_SUPPORT
 	#define	WD_TURKMEN		//������������
#endif

#ifdef IM_CHOSEN_SUPPORT
 	#define WD_CHOSEN		//���ܳ�����
#endif

#ifdef IM_MONGOLIAN_SUPPORT
 	#define	WD_MONGOLIAN	//�����ɹ���
#endif

#ifdef IM_SWAHILI_SUPPORT
 	#define WD_SWAHILI		//����˹������
#endif

#ifdef IM_MARATHI_SUPPORT
 #define	WD_MARATHI			//������������
#endif

#ifdef IM_KANNADA_SUPPORT
 #define	WD_KANNADA			//���ܿ��ɴ���
#endif

#ifdef IM_ASSAMESE_SUPPORT
 #define	WD_ASSAMESE			//���ܰ���ķ��
#endif

#ifdef IM_MALAYALAM_SUPPORT
 #define	WD_MALAYALAM		//������������ķ��
#endif

#ifdef IM_PUNJABI_SUPPORT
 #define	WD_PUNJABI			//������������
#endif

#ifdef IM_MAITHILI_SUPPORT
 #define	WD_MAITHILI			//������������
#endif

#ifdef IM_GUJARATI_SUPPORT
 #define	WD_GUJARATI			//���ܹż�������
#endif

#ifdef IM_SERBIANC_SUPPORT
 #define  WD_SERBIANC			//��������ά�������
#endif

#ifdef IM_BOSNIAN_SUPPORT
 #define  WD_BOSNIAN			//��˹�������������뷨
#endif

#ifdef IM_IRISH_SUPPORT
 #define  WD_IRISH			//���������������뷨
#endif

#ifdef IM_AZERBAIJANI_SUPPORT
 #define  WD_AZERBAIJANI		//�����ݽ����������뷨
#endif

#ifdef IM_IGBO_SUPPORT
 #define  WD_IGBO				//������
#endif

#ifdef IM_HAUSA_SUPPORT
 #define  WD_HAUSA			//������
#endif

#ifdef IM_LATVIAN_SUPPORT
 #define  WD_LATVIAN			//����ά����// 
#endif

#ifdef IM_CROATIAN_SUPPORT
 #define  WD_CROATIAN			//���޵�����
#endif

#ifdef IM_PASHTO_SUPPORT
	 #define WD_PASHTO			//������ʲͼ��
#endif

//������������
// 	#define WD_YI				//�����������ԣ�����
//	#define WD_CNOSEN				//�����������ԣ�������
//	#define WD_TIBETAN				//�����������ԣ�����wanghui
//	#define WD_TIBETAN_ABC			//������ĸ���뷨 qls
#ifdef IM_UYGHUR_SUPPORT
 	#define WD_UIGHUR				//�����������ԣ�ά��
#endif 	
// 	#define WD_MIAO					//�����������ԣ�����
// 	#define WD_ZHUANG				//�����������ԣ�׳��
// 	#define WD_DONG					//�����������ԣ�����
// 	#define WD_KAZAK				//���ܹ�������(�й�)		
// 	#define WD_MONGOLIAN_TRADITIONAL		//�����������ԣ��ɹ���
// 	#define WD_DEHONG_DAI			//�����������ԣ�����
	//#define WD_UIGHUR //����ά�����

//#define WD_JAVANESE //צ����
//#define WD_MOROCCAN //Ħ�����
//#define WD_TUNISIAN//ͻ��˹��
//#define WD_SUDANESE     //�յ���
//#define WD_PAKISTAN    //�ͻ�˹̹��
#ifdef IM_SINHALESE_SUPPORT
	#define WD_SINHALESE  //ɮ٤����
#endif	
//#define WD_MACEDONIAN    //�������
//#define WD_KINYARWANDA  //¬������
//#define WD_ZULU    		    //��³��
//#define WD_TATAR    	    //������
//#define WD_YEMENI     //Ҳ����
//#define WD_BHUTANI      //������
//#define WD_FIJIAN    	  //쳼���
//#define WD_KONGO    		   //�չ���
//#define WD_SYRIAC       //��������
//#define WD_ALGERIAN      //������������
//#define WD_MALTESE       //�������
//#define WD_WELSH    	  //����ʿ��
//#define WD_MALDIVIAN      //���������

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
		return 3;//�����Ѷ���
	}
	else
	{
		if ((existTemp[existNum] >>bit)&1)
		{
			return 1;//���Դ���
		}
		else
		{
			return 0;//���Բ�����
		}
	}
	
}

/*lint -save -e605*/	
void ImeCopy(WD_UINT8* exist)
{
	WD_UINT8  i = 0;  //���ڻ�ȡ���뷨����
	WD_UINT8 existTemp[64]={0};
	SCI_MEMSET(&languageInfo , 0, sizeof(languageInfo));



{//����
#ifdef WD_DIGIT
	languageInfo[i].imeType = WDI_DIGIT;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x6570;
	languageInfo[i].imeString[1] = 0x5B57;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "����", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
#endif
}


//��ƴ��
#ifdef WD_CN_CPINYIN
{
	languageInfo[i].imeType = WDI_CN_CPINYIN;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x7EAF;
	languageInfo[i].imeString[1] = 0x62FC;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "��ƴ", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//�ظ���ƴ��
#ifdef WD_CN_FULLPINYIN
{
	languageInfo[i].imeType = WDI_CN_FULLPINYIN;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x5168;
	languageInfo[i].imeString[1] = 0x62fc;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "ȫƴ", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//̨��ע��
#ifdef WD_CN_ZHUYIN
{
	languageInfo[i].imeType = WDI_CN_ZHUYIN;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x6CE8;
	languageInfo[i].imeString[1] = 0x97F3;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "ע��", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//�ʻ�USE_WDI_CN_BIHUA
#ifdef WD_CN_BIHUA
{
	languageInfo[i].imeType = WDI_CN_BIHUA;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x7B14;
	languageInfo[i].imeString[1] = 0x5212;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "�ʻ�", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//�ʻ�����
#ifdef WD_CN_BIHUAF
{
	languageInfo[i].imeType = WDI_CN_BIHUAF;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x7B46;
	languageInfo[i].imeString[1] = 0x756B;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "�P��", sizeof(languageInfo[i].imeString));
#endif
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//̨��ע��
#ifdef WD_CN_FULLZHUYIN
{
	languageInfo[i].imeType = WDI_CN_FULLZHUYIN;
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

#ifdef USE_UNICODE
	languageInfo[i].imeString[0] = 0x5168;
	languageInfo[i].imeString[1] = 0x6CE8;
#else
	SCI_MEMCPY(languageInfo[i].imeString , "ȫע", sizeof(languageInfo[i].imeString));
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
//��������ABC
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

//������ĸ���뷨
#ifdef WD_RUSSIAN_ABC
{
	extern lan_keymap_Info_t Russian_ABC_keymap_Info;
	cpy_data_2_data(i, &Russian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//������ĸ���뷨
#ifdef WD_FRENCH_ABC
{
	extern const lan_keymap_Info_t French_ABC_keymap_Info;
	cpy_data_2_data(i, &French_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
	
//Խ������ĸ���뷨
#ifdef WD_VIETNAMESE_ABC
{
	extern lan_keymap_Info_t Vietnamese_ABC_keymap_Info;
	cpy_data_2_data(i, &Vietnamese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);

	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//̩����ĸ���뷨
#ifdef WD_THAI_ABC
{
	extern lan_keymap_Info_t Thai_ABC_keymap_Info;
	cpy_data_2_data(i, &Thai_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//ӡ������ĸ���뷨
#ifdef WD_INDONESIAN_ABC
{
	extern lan_keymap_Info_t Indonesian_ABC_keymap_Info;
	cpy_data_2_data(i, &Indonesian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//����������ĸ���뷨
#ifdef WD_PORTUGUESE_ABC
{
	extern lan_keymap_Info_t Portuguese_ABC_keymap_Info;
	cpy_data_2_data(i, &Portuguese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


	//��ĸ��������
#ifdef WD_ARABIC_ABC
{
	extern const lan_keymap_Info_t Arabic_ABC_keymap_Info;
	cpy_data_2_data(i, &Arabic_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
	
	//��ĸ����
#ifdef WD_GERMAN_ABC
{
	extern lan_keymap_Info_t German_ABC_keymap_Info;
	cpy_data_2_data(i, &German_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸӡ����
#ifdef WD_HINDI_ABC
{
	extern lan_keymap_Info_t Hindi_ABC_keymap_Info;
	cpy_data_2_data(i, &Hindi_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ�������
#ifdef WD_ITALIAN_ABC
{
	extern const lan_keymap_Info_t Italian_ABC_keymap_Info;
	cpy_data_2_data(i, &Italian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ������
#ifdef WD_MALAY_ABC
{
	extern lan_keymap_Info_t Malay_ABC_keymap_Info;
	cpy_data_2_data(i, &Malay_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ��˹��
#ifdef WD_PERSIAN_ABC
{
	extern lan_keymap_Info_t Persian_ABC_keymap_Info;
	cpy_data_2_data(i, &Persian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ��������
#ifdef WD_SPANISH_ABC
{
	extern lan_keymap_Info_t Spanish_ABC_keymap_Info;
	cpy_data_2_data(i, &Spanish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ��������
#ifdef WD_TURKISH_ABC
{
	extern lan_keymap_Info_t Turkish_ABC_keymap_Info;
	cpy_data_2_data(i, &Turkish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ�ݿ���
#ifdef WD_CZECH_ABC
{
	extern lan_keymap_Info_t Czech_ABC_keymap_Info;
	cpy_data_2_data(i, &Czech_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ������
#ifdef WD_DUTCH_ABC
{
	extern lan_keymap_Info_t Dutch_ABC_keymap_Info;
	cpy_data_2_data(i, &Dutch_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸϣ������
#ifdef WD_HEBREW_ABC
{
	extern lan_keymap_Info_t Hebrew_ABC_keymap_Info;
	cpy_data_2_data(i, &Hebrew_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ������
#ifdef WD_POLISH_ABC
{
	extern lan_keymap_Info_t Polish_ABC_keymap_Info;
	cpy_data_2_data(i, &Polish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸά�����
#ifdef WD_UIGHUR_ABC
{
	extern lan_keymap_Info_t Uighur_ABC_keymap_Info;
	cpy_data_2_data(i, &Uighur_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif	

	//��ĸ�����
#ifdef WD_SWEDISH_ABC
{
	extern lan_keymap_Info_t Swedish_ABC_keymap_Info;
	cpy_data_2_data(i, &Swedish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif	

	//��ĸ������
#ifdef WD_FINNISH_ABC
{
	extern lan_keymap_Info_t Finnish_ABC_keymap_Info;
	cpy_data_2_data(i, &Finnish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸŲ����
#ifdef WD_NORWEGIAN_ABC
{
	extern lan_keymap_Info_t Norwegian_ABC_keymap_Info;
	cpy_data_2_data(i, &Norwegian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ������
#ifdef WD_DANISH_ABC
{
	extern lan_keymap_Info_t Danish_ABC_keymap_Info;
	cpy_data_2_data(i, &Danish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ������
#ifdef WD_LAO_ABC
{
	extern lan_keymap_Info_t Lao_ABC_keymap_Info;
	cpy_data_2_data(i, &Lao_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ�ڶ�����
#ifdef WD_URDU_ABC
{
	extern lan_keymap_Info_t Urdu_ABC_keymap_Info;
	cpy_data_2_data(i, &Urdu_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ��������(�й�)
#ifdef WD_KAZAK_ABC
{
	extern lan_keymap_Info_t Kazak_ABC_keymap_Info;
	cpy_data_2_data(i, &Kazak_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ������
#ifdef WD_ICELANDIC_ABC
{
	extern lan_keymap_Info_t Iceland_ABC_keymap_Info;
	cpy_data_2_data(i, &Iceland_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ˹�工����
#ifdef WD_SLOVAK_ABC
{
	extern lan_keymap_Info_t Slovak_ABC_keymap_Info;
	cpy_data_2_data(i, &Slovak_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ��ɳ������
#ifdef WD_ESTONIAN_ABC
{
	extern lan_keymap_Info_t Estonian_ABC_keymap_Info;
	cpy_data_2_data(i, &Estonian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


	//��ĸ�׶���˹��
#ifdef WD_BELARUSIAN_ABC
{
	extern lan_keymap_Info_t Belarusian_ABC_keymap_Info;
	cpy_data_2_data(i, &Belarusian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ����ά����
#ifdef WD_LATVIAN_ABC
{
	extern lan_keymap_Info_t Latvian_ABC_keymap_Info;
	cpy_data_2_data(i, &Latvian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ˹����������
#ifdef WD_SLOVENIAN_ABC
{
	extern lan_keymap_Info_t Slovenian_ABC_keymap_Info;
	cpy_data_2_data(i, &Slovenian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ���޵�����
#ifdef WD_CROATIAN_ABC
{
	extern lan_keymap_Info_t Croatian_ABC_keymap_Info;
	cpy_data_2_data(i, &Croatian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ����ά����
#ifdef WD_SERBIAN_ABC
{
	extern lan_keymap_Info_t Serbian_ABC_keymap_Info;
	cpy_data_2_data(i, &Serbian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ������
#ifdef WD_HAUSA_ABC
{
	extern lan_keymap_Info_t Hausa_ABC_keymap_Info;
	cpy_data_2_data(i, &Hausa_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ������
#ifdef WD_IGBO_ABC
{
	extern lan_keymap_Info_t Igbo_ABC_keymap_Info;
	cpy_data_2_data(i, &Igbo_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ��������
#ifdef WD_TURKMEN_ABC
{
	extern lan_keymap_Info_t Turkmen_ABC_keymap_Info;
	cpy_data_2_data(i, &Turkmen_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ˹��ϣ����
#ifdef WD_SWAHILI_ABC
{
	extern lan_keymap_Info_t Swahili_ABC_keymap_Info;
	cpy_data_2_data(i, &Swahili_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ��������
#ifdef WD_HUNGARIAN_ABC
{
	extern lan_keymap_Info_t Hungarian_ABC_keymap_Info;
	cpy_data_2_data(i, &Hungarian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸϣ����
#ifdef WD_GREEK_ABC
{
	extern lan_keymap_Info_t Greek_ABC_keymap_Info;
	cpy_data_2_data(i, &Greek_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

		//��ĸ��������
#ifdef WD_KAZAKH_ABC
{
	extern lan_keymap_Info_t Kazakh_ABC_keymap_Info;
	cpy_data_2_data(i, &Kazakh_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
	
	
	//��ĸ����������
#ifdef WD_ROMANIAN_ABC
{
	extern lan_keymap_Info_t Romanian_ABC_keymap_Info;
	cpy_data_2_data(i, &Romanian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ����»��
#ifdef WD_TAGALOG_ABC
{
	extern lan_keymap_Info_t Tagalog_ABC_keymap_Info;
	cpy_data_2_data(i, &Tagalog_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
	//��ĸ��˹������
#ifdef WD_BOSNIAN_ABC
{
	extern lan_keymap_Info_t Bosnian_ABC_keymap_Info;
	cpy_data_2_data(i, &Bosnian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ̩�׶���wanghui
#ifdef WD_TAMIL_ABC
{
	extern lan_keymap_Info_t Tamil_ABC_keymap_Info;
	cpy_data_2_data(i, &Tamil_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ�������wanghui
#ifdef WD_CYRILLIC_ABC
{
	extern lan_keymap_Info_t Cyrillic_ABC_keymap_Info;
	cpy_data_2_data(i, &Cyrillic_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

	//��ĸ����ά��(�������)wanghui
#ifdef WD_SERBIANC_ABC
{
	extern lan_keymap_Info_t Serbianc_ABC_keymap_Info;
	cpy_data_2_data(i, &Serbianc_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif
#ifdef WD_TELUGU_ABC			//̩¬������ĸ���뷨
{
	extern lan_keymap_Info_t Telugu_ABC_keymap_Info;
	cpy_data_2_data(i, &Telugu_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_BULGARIAN_ABC		/* ������������ĸ				*/
{
	extern lan_keymap_Info_t Bulgarian_ABC_keymap_Info;
	cpy_data_2_data(i, &Bulgarian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_BENGALI_ABC		// �ϼ�������ĸ���뷨qls
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
#ifdef WD_KHMER_ABC		//��ĸ������  qls
{
	extern lan_keymap_Info_t Khmer_ABC_keymap_Info;
	cpy_data_2_data(i, &Khmer_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_NEPALI_ABC		//��ĸ�Ჴ����  qls
{
	extern lan_keymap_Info_t Nepali_ABC_keymap_Info;
	cpy_data_2_data(i, &Nepali_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_CHOSEN_ABC		//��ĸ������
{
	extern lan_keymap_Info_t Chosen_ABC_keymap_Info;
	cpy_data_2_data(i, &Chosen_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MIAO_ABC			//��ĸ����
{
	extern lan_keymap_Info_t Miao_ABC_keymap_Info;
	cpy_data_2_data(i, &Miao_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MONGOLIAN_TRADITIONAL_ABC	//��ͳ��ĸ�ɹ���
{
	extern lan_keymap_Info_t Mongolian_Traditional_ABC_keymap_Info;
	cpy_data_2_data(i, &Mongolian_Traditional_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MONGOLIAN_ABC	//��ĸ�ɹ���
{
	extern lan_keymap_Info_t Mongolian_ABC_keymap_Info;
	cpy_data_2_data(i, &Mongolian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_GEORGIAN_ABC	//��ĸ��³����
{
	extern lan_keymap_Info_t Georgian_ABC_keymap_Info;
	cpy_data_2_data(i, &Georgian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_BURMESE_ABC	//��ĸ���
{
	extern lan_keymap_Info_t Burmese_ABC_keymap_Info;
	cpy_data_2_data(i, &Burmese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MARATHI_ABC	//��ĸ������
{
	extern lan_keymap_Info_t Marathi_ABC_keymap_Info;
	cpy_data_2_data(i, &Marathi_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_KANNADA_ABC	//��ĸ���ɴ�
{
	extern lan_keymap_Info_t Kannada_ABC_keymap_Info;
	cpy_data_2_data(i, &Kannada_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_ASSAMESE_ABC	//��ĸ����ķ
{
	extern lan_keymap_Info_t Assamese_ABC_keymap_Info;
	cpy_data_2_data(i, &Assamese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MALAYALAM_ABC	//��ĸ��������ķ
{
	extern lan_keymap_Info_t Malayalam_ABC_keymap_Info;
	cpy_data_2_data(i, &Malayalam_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_PUNJABI_ABC	//��ĸ������
{
	extern lan_keymap_Info_t Punjabi_ABC_keymap_Info;
	cpy_data_2_data(i, &Punjabi_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MAITHILI_ABC	//��ĸ������
{
	extern lan_keymap_Info_t Maithiti_ABC_keymap_Info;
	cpy_data_2_data(i, &Maithiti_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_GUJARATI_ABC	//��ĸ�ż�����
{
	extern lan_keymap_Info_t Gujarati_ABC_keymap_Info;
	cpy_data_2_data(i, &Gujarati_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_IRISH_ABC	//��ĸ������
{
	extern lan_keymap_Info_t Irish_ABC_keymap_Info;
	cpy_data_2_data(i, &Irish_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_CATALAN_ABC	//��ĸ��̩¡
{
	extern lan_keymap_Info_t Catalan_ABC_keymap_Info;
	cpy_data_2_data(i, &Catalan_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_AZERBAIJANI_ABC	//��ĸ��̩¡
{
	extern lan_keymap_Info_t Azerbaijani_ABC_keymap_Info;
	cpy_data_2_data(i, &Azerbaijani_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

				
#ifdef WD_PASHTO_ABC	//��ĸ��ʲͼ��
{
	extern lan_keymap_Info_t Pashto_ABC_keymap_Info;
	cpy_data_2_data(i, &Pashto_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif


#ifdef WD_PASHTO	//������ʲͼ��
{
	extern lan_ddd Pashto_Ime_Data;
	extern lan_keymap_Info_t Pashto_keymap_Info;
	cpy_data_2_data(i, &Pashto_keymap_Info, &Pashto_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_JAVANESE_ABC //��ĸצ����
{
	extern lan_keymap_Info_t Javanese_ABC_keymap_Info;
	cpy_data_2_data(i, &Javanese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MOROCCAN_ABC //��ĸĦ�����
{
	extern lan_keymap_Info_t Moroccan_ABC_keymap_Info;
	cpy_data_2_data(i, &Moroccan_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_TUNISIAN_ABC//��ĸͻ��˹��
{
	extern lan_keymap_Info_t Tunisian_ABC_keymap_Info;
	cpy_data_2_data(i, &Tunisian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SUDANESE_ABC     //��ĸ�յ���
{
	extern lan_keymap_Info_t Sudanese_ABC_keymap_Info;
	cpy_data_2_data(i, &Sudanese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_PAKISTAN_ABC    //��ĸ�ͻ�˹̹��
{
	extern lan_keymap_Info_t Pakistan_ABC_keymap_Info;
	cpy_data_2_data(i, &Pakistan_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SINHALESE_ABC  //��ĸɮ٤����
{
	extern lan_keymap_Info_t Sinhalese_ABC_keymap_Info;
	cpy_data_2_data(i, &Sinhalese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MACEDONIAN_ABC    //��ĸ�������
{
	extern lan_keymap_Info_t Macedonian_ABC_keymap_Info;
	cpy_data_2_data(i, &Macedonian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_KINYARWANDA_ABC  //��ĸ¬������
{
	extern lan_keymap_Info_t Kinyarwanda_ABC_keymap_Info;
	cpy_data_2_data(i, &Kinyarwanda_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_ZULU_ABC    		    //��ĸ��³��
{
	extern lan_keymap_Info_t Zulu_ABC_keymap_Info;
	cpy_data_2_data(i, &Zulu_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_TATAR_ABC    	    //��ĸ������
{
	extern lan_keymap_Info_t Tatar_ABC_keymap_Info;
	cpy_data_2_data(i, &Tatar_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_YEMENI_ABC     //��ĸҲ����
{
	extern lan_keymap_Info_t Yemeni_ABC_keymap_Info;
	cpy_data_2_data(i, &Yemeni_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_BHUTANI_ABC      //��ĸ������
{
	extern lan_keymap_Info_t Bhutani_ABC_keymap_Info;
	cpy_data_2_data(i, &Bhutani_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_FIJIAN_ABC    	  //��ĸ쳼���
{
	extern lan_keymap_Info_t Fijian_ABC_keymap_Info;
	cpy_data_2_data(i, &Fijian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_KONGO_ABC    		   //��ĸ�չ���
{
	extern lan_keymap_Info_t Kongo_ABC_keymap_Info;
	cpy_data_2_data(i, &Kongo_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SYRIAC_ABC       //��ĸ��������
{
	extern lan_keymap_Info_t Syriac_ABC_keymap_Info;
	cpy_data_2_data(i, &Syriac_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_ALGERIAN_ABC      //��ĸ������������
{
	extern lan_keymap_Info_t Algerian_ABC_keymap_Info;
	cpy_data_2_data(i, &Algerian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MALTESE_ABC       //��ĸ�������
{
	extern lan_keymap_Info_t Maltese_ABC_keymap_Info;
	cpy_data_2_data(i, &Maltese_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_WELSH_ABC    	  //��ĸ����ʿ��
{
	extern lan_keymap_Info_t Welsh_ABC_keymap_Info;
	cpy_data_2_data(i, &Welsh_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MALDIVIAN_ABC      //��ĸ���������
{
	extern lan_keymap_Info_t Maldivian_ABC_keymap_Info;
	cpy_data_2_data(i, &Maldivian_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

#ifdef WD_JAVANESE //����צ����
{
	extern lan_ddd Javanese_Ime_Data;
	extern lan_keymap_Info_t Javanese_keymap_Info;
	cpy_data_2_data(i, &Javanese_keymap_Info, &Javanese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MOROCCAN //����Ħ�����
{
	extern lan_ddd Moroccan_Ime_Data;
	extern lan_keymap_Info_t Moroccan_keymap_Info;
	cpy_data_2_data(i, &Moroccan_keymap_Info, &Moroccan_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_TUNISIAN//����ͻ��˹��
{
	extern lan_ddd Tunisian_Ime_Data;
	extern lan_keymap_Info_t Tunisian_keymap_Info;
	cpy_data_2_data(i, &Tunisian_keymap_Info, &Tunisian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SUDANESE     //�����յ���
{
	extern lan_ddd Sudanese_Ime_Data;
	extern lan_keymap_Info_t Sudanese_keymap_Info;
	cpy_data_2_data(i, &Sudanese_keymap_Info, &Sudanese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_PAKISTAN    //���ܰͻ�˹̹��
{
	extern lan_ddd Pakistan_Ime_Data;
	extern lan_keymap_Info_t Pakistan_keymap_Info;
	cpy_data_2_data(i, &Pakistan_keymap_Info, &Pakistan_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SINHALESE  //����ɮ٤����
{
	extern lan_ddd Sinhalese_Ime_Data;
	extern lan_keymap_Info_t Sinhalese_keymap_Info;
	cpy_data_2_data(i, &Sinhalese_keymap_Info, &Sinhalese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MACEDONIAN    //�����������
{
	extern lan_ddd Macedonian_Ime_Data;
	extern lan_keymap_Info_t Macedonian_keymap_Info;
	cpy_data_2_data(i, &Macedonian_keymap_Info, &Macedonian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_KINYARWANDA  //����¬������
{
	extern lan_ddd Kinyarwanda_Ime_Data;
	extern lan_keymap_Info_t Kinyarwanda_keymap_Info;
	cpy_data_2_data(i, &Kinyarwanda_keymap_Info, &Kinyarwanda_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_ZULU    		    //������³��
{
	extern lan_ddd Zulu_Ime_Data;
	extern lan_keymap_Info_t Zulu_keymap_Info;
	cpy_data_2_data(i, &Zulu_keymap_Info, &Zulu_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_TATAR    	    //����������
{
	extern lan_ddd Tatar_Ime_Data;
	extern lan_keymap_Info_t Tatar_keymap_Info;
	cpy_data_2_data(i, &Tatar_keymap_Info, &Tatar_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_YEMENI     //����Ҳ����
{
	extern lan_ddd Yemeni_Ime_Data;
	extern lan_keymap_Info_t Yemeni_keymap_Info;
	cpy_data_2_data(i, &Yemeni_keymap_Info, &Yemeni_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_BHUTANI      //���ܲ�����
{
	extern lan_ddd Bhutani_Ime_Data;
	extern lan_keymap_Info_t Bhutani_keymap_Info;
	cpy_data_2_data(i, &Bhutani_keymap_Info, &Bhutani_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_FIJIAN    	  //����쳼���
{
	extern lan_ddd Fijian_Ime_Data;
	extern lan_keymap_Info_t Fijian_keymap_Info;
	cpy_data_2_data(i, &Fijian_keymap_Info, &Fijian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_KONGO    		   //���ܸչ���
{
	extern lan_ddd Kongo_Ime_Data;
	extern lan_keymap_Info_t Kongo_keymap_Info;
	cpy_data_2_data(i, &Kongo_keymap_Info, &Kongo_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_SYRIAC       //������������
{
	extern lan_ddd Syriac_Ime_Data;
	extern lan_keymap_Info_t Syriac_keymap_Info;
	cpy_data_2_data(i, &Syriac_keymap_Info, &Syriac_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_ALGERIAN      //���ܰ�����������
{
	extern lan_ddd Algerian_Ime_Data;
	extern lan_keymap_Info_t Algerian_keymap_Info;
	cpy_data_2_data(i, &Algerian_keymap_Info, &Algerian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MALTESE       //�����������
{
	extern lan_ddd Maltese_Ime_Data;
	extern lan_keymap_Info_t Maltese_keymap_Info;
	cpy_data_2_data(i, &Maltese_keymap_Info, &Maltese_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_WELSH    	  //��������ʿ��
{
	extern lan_ddd Welsh_Ime_Data;
	extern lan_keymap_Info_t Welsh_keymap_Info;
	cpy_data_2_data(i, &Welsh_keymap_Info, &Welsh_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif
#ifdef WD_MALDIVIAN      //�������������
{
	extern lan_ddd Maldivian_Ime_Data;
	extern lan_keymap_Info_t Maldivian_keymap_Info;
	cpy_data_2_data(i, &Maldivian_keymap_Info, &Maldivian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
	
}
#endif

//����ά�����
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

//���ܲ���
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

//��ĸ����
#ifdef WD_TIBETAN_ABC
{
	extern lan_keymap_Info_t Tibetan_ABC_keymap_Info;
	cpy_data_2_data(i, &Tibetan_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//��������
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

	//��������
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

//��ĸ����
#ifdef WD_MONGAL_ABC
{
	extern lan_keymap_Info_t Mongal_ABC_keymap_Info;
	cpy_data_2_data(i, &Mongal_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}

#endif
// �ڶ�����
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

	//���ܹ�������(�й�)
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
	
	//���ܹ�������wanghui
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

//���ܳ�����
#ifdef WD_CNKOREAN_ABC
{
	extern lan_keymap_Info_t Cnkorean_ABC_keymap_Info;
	cpy_data_2_data(i, &Cnkorean_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//����
#ifdef WD_CHNDAI_ABC
{
	extern lan_keymap_Info_t Chndai_ABC_keymap_Info;
	cpy_data_2_data(i, &Chndai_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//����׳��
#ifdef WD_CHNZHUANG_ABC
{
	extern lan_keymap_Info_t Chnzhuang_ABC_keymap_Info;
	cpy_data_2_data(i, &Chnzhuang_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif


//���ܶ���
#ifdef WD_CHNDONG_ABC
{
	extern lan_keymap_Info_t Chndong_ABC_keymap_Info;
	cpy_data_2_data(i, &Chndong_ABC_keymap_Info, 0);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//��������
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

// ���������� 
//����Ӣ��
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
//������������
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
//���ܷ���
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
//������
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
//ϣ����
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
//���ܵ���
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


//������������
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

//�����������
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

//���ܵ���
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
//�������
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

//����Ų��
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

//������������
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
//�ݿ���
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
//������
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

//����ӡ����
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

//������������
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


//��˹��
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



//���ܼ�̩¡��
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
//���ܶ���
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


//����̩����
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

//�ڿ�����
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


//����������
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

//����Խ���� 
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

//����������
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

//���ܰ�������
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


//����ӡ����

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

//��������»��

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



//����ϣ������

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

#ifdef WD_ROMANIAN			//����������
{
	extern lan_ddd Romanian_Ime_Data;
	extern lan_keymap_Info_t Romanian_keymap_Info;
	cpy_data_2_data(i, &Romanian_keymap_Info, &Romanian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_BENGALI			//�ϼ�����
{
	extern lan_ddd Bengali_Ime_Data;
	extern lan_keymap_Info_t Bengali_keymap_Info;
	cpy_data_2_data(i, &Bengali_keymap_Info, &Bengali_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_TELUGU			//̩¬�������뷨
{
	extern lan_ddd Telugu_Ime_Data;
	extern lan_keymap_Info_t Telugu_keymap_Info;
	cpy_data_2_data(i, &Telugu_keymap_Info, &Telugu_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_BULGARIAN		/* ����������				*/
{
	extern lan_ddd Bulgarian_Ime_Data;
	extern lan_keymap_Info_t Bulgarian_keymap_Info;
	cpy_data_2_data(i, &Bulgarian_keymap_Info, &Bulgarian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_SLOVENIAN		//˹�������������뷨
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

#ifdef WD_PUNJABI //������
{
	extern lan_ddd Punjabi_Ime_Data;
	extern lan_keymap_Info_t Punjabi_keymap_Info;
	cpy_data_2_data(i, &Punjabi_keymap_Info, &Punjabi_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_MAITHILI//������
{
	extern lan_ddd Maithili_Ime_Data;
	extern lan_keymap_Info_t Maithili_keymap_Info;
	cpy_data_2_data(i, &Maithili_keymap_Info, &Maithili_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_BOSNIAN//��˹����
{
	extern lan_ddd Bosnian_Ime_Data;
	extern lan_keymap_Info_t Bosnian_keymap_Info;
	cpy_data_2_data(i, &Bosnian_keymap_Info, &Bosnian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_CROATIAN//���޵���
{
	extern lan_ddd Croatian_Ime_Data;
	extern lan_keymap_Info_t Croatian_keymap_Info;
	cpy_data_2_data(i, &Croatian_keymap_Info, &Croatian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_LATVIAN//����ά��
{
	extern lan_ddd Latvian_Ime_Data;
	extern lan_keymap_Info_t Latvian_keymap_Info;
	cpy_data_2_data(i, &Latvian_keymap_Info, &Latvian_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_IRISH//������
{
	extern lan_ddd Irish_Ime_Data;
	extern lan_keymap_Info_t Irish_keymap_Info;
	cpy_data_2_data(i, &Irish_keymap_Info, &Irish_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_GUJARATI   //�ż�����
{
	extern lan_ddd Gujarati_Ime_Data;
	extern lan_keymap_Info_t Gujarati_keymap_Info;
	cpy_data_2_data(i, &Gujarati_keymap_Info, &Gujarati_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_HAUSA   //����
{
	extern lan_ddd Hausa_Ime_Data;
	extern lan_keymap_Info_t Hausa_keymap_Info;
	cpy_data_2_data(i, &Hausa_keymap_Info, &Hausa_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_IGBO   //����
{
	extern lan_ddd Igbo_Ime_Data;
	extern lan_keymap_Info_t Igbo_keymap_Info;
	cpy_data_2_data(i, &Igbo_keymap_Info, &Igbo_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

#ifdef WD_AZERBAIJANI   //�����ݽ�
{
	extern lan_ddd Azerbaijani_Ime_Data;
	extern lan_keymap_Info_t Azerbaijani_keymap_Info;
	cpy_data_2_data(i, &Azerbaijani_keymap_Info, &Azerbaijani_Ime_Data);
	IsLanguageExist(languageInfo[i].imeType,existTemp,1);
	i++;
	if(i>=WD_IME_TOTAL)goto end;
}
#endif

//�õ�ʵ�����뷨������
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


