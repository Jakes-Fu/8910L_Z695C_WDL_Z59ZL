// csc.c
/*=====================================================================================

  FILE NAME :  csc.c
  MODULE NAME : WE Framework csc
  
   PRE-INCLUDE FILES DESCRIPTION

   GENERAL DESCRIPTION
   This file can be included by program when need to convert charset.In this file all csc interfaces and 
   related macro is defined.
   These Info is  aptotic and is not changed by switching Project.

   TechFaith Software Confidential Proprietary
   Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
   =======================================================================================
   Revision History

   Modification                 Tracking
   Date           Author         Number        Description of changes
   ----------   ------------    ---------   --------------------------------------------
   2007-05-23   George Yang                  Init

   Self-documenting Code
   Describe/explain low-level design, especially things in header files, of this module and/or group 
   of functions and/or specific function that are hard to understand by reading code and thus requires 
   detail description.
   Free format !

=====================================================================================*/
#include "we_scl.h"
#include "we_csc.h"
#include "csc_def.h"

#ifdef   WE_CSC_CFG_SUPPORT_GB18030
#include "csc_core_gb18030.h"
#endif   // #ifdef   WE_CSC_CFG_SUPPORT_GB18030

#ifdef   WE_CSC_CFG_SUPPORT_GBK
#include "csc_core_gbk.h"
#endif   // #ifdef   WE_CSC_CFG_SUPPORT_GBK

#ifdef   WE_CSC_CFG_SUPPORT_BIG5
#include "csc_core_big5.h"
#endif   // #ifdef   WE_CSC_CFG_SUPPORT_BIG5

#ifdef   WE_CSC_CFG_SUPPORT_GB2312
#include "csc_core_gb2312.h"
#endif   // #ifdef   WE_CSC_CFG_SUPPORT_GB2312

/******************************************************************************
 * Define Constants
 *****************************************************************************/
#define WE_CSC_BIG_ENDIAN                    1
#define WE_CSC_LITTLE_ENDIAN                 2

#define WE_CSC_SURROGATE_HIGH_START          0xd800UL
#define WE_CSC_SURROGATE_HIGH_END            0xdbffUL
#define WE_CSC_SURROGATE_LOW_START           0xdc00UL
#define WE_CSC_SURROGATE_LOW_END             0xdfffUL
#define WE_CSC_REPLACEMENT_CHAR              0x0000fffdUL

#define  WE_CSC_NO_ASCII_CHAR                0x3F

#define WE_CSC_CVT_TABLE_SIZE                38

static const WE_UCHAR   aucUtf8BytePerChar[] = 
{
   1, 1, 1, 1, 1, 1, 1, 1,
   0, 0, 0, 0, 2, 2, 3, 4
}; // aucUtf8BytePerChar

static const WE_UINT32  aunUtf8Offset[] =
{
   0, 0, 0x00003080, 0x000e2080UL, 0x03c82080UL
}; // aunUtf8Offset


static const WE_UINT16 ausLatin2Code[256] = 
{ 
   /* Not used */ 
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 

   /* Latin-2 */ 
   0x0a0, 0x104, 0x2d8, 0x141, 0x0a4, 0x13d, 0x15a, 0x0a7,         
   0x0a8, 0x160, 0x15e, 0x164, 0x179, 0x0ad, 0x17d, 0x17b, 
   0x0b0, 0x105, 0x2db, 0x142, 0x0b4, 0x13e, 0x15b, 0x2c7, 
   0x0b8, 0x161, 0x15f, 0x165, 0x17a, 0x2dd, 0x17e, 0x17c, 
   0x154, 0x0c1, 0x0c2, 0x102, 0x0c4, 0x139, 0x106, 0x0c7, 
   0x10c, 0x0c9, 0x118, 0x0cb, 0x11a, 0x0cd, 0x0ce, 0x10e, 
   0x110, 0x143, 0x147, 0x0d3, 0x0d4, 0x150, 0x0d6, 0x0d7, 
   0x158, 0x16e, 0x0da, 0x170, 0x0dc, 0x0dd, 0x162, 0x0df, 
   0x155, 0x0e1, 0x0e2, 0x103, 0x0e4, 0x13a, 0x107, 0x0e7, 
   0x10d, 0x0e9, 0x119, 0x0eb, 0x11b, 0x0ed, 0x0ee, 0x10f, 
   0x111, 0x144, 0x148, 0x0f3, 0x0f4, 0x151, 0x0f6, 0x0f7,     
   0x159, 0x16f, 0x0fa, 0x171, 0x0fc, 0x0fd, 0x163, 0x2d9 
}; // ausLatin2Code



/**********************************************************************
* Support for character set ISO-8859-5 (Hebrew)
**********************************************************************/

static const WE_UINT16 ausHebrewCode[256] = 
{
   /* Not used */
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,

   /* Like Latin-1 (almost) */
   0x0a0, 0x000, 0x0a2, 0x0a3, 0x0a4, 0x0a5, 0x0a6, 0x0a7,
   0x0a8, 0x0a9, 0x0d7, 0x0ab, 0x0ac, 0x0ad, 0x0ae, 0x0af,
   0x0b0, 0x0b1, 0x0b2, 0x0b3, 0x0b4, 0x0b5, 0x0b6, 0x0b7,
   0x0b8, 0x0b9, 0x0f7, 0x0bb, 0x0bc, 0x0bd, 0x0be, 0x000,

   /* Not used (except position 0xdf) */
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
   0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x2017,

   /* Hebrew letters */
   0x5d0, 0x5d1, 0x5d2, 0x5d3, 0x5d4, 0x5d5, 0x5d6, 0x5d7,
   0x5d8, 0x5d9, 0x5da, 0x5db, 0x5dc, 0x5dd, 0x5de, 0x5df,
   0x5e0, 0x5e1, 0x5e2, 0x5e3, 0x5e4, 0x5e5, 0x5e6, 0x5e7,
   0x5e8, 0x5e9, 0x5ea, 0x000, 0x000, 0x200e, 0x200f, 0x000
}; // ausHebrewCode


#define WE_CSC_STRINGS_TABLE_SIZE            42
#define WE_CSC_HASH_TABLE_SIZE               59

static const WE_UCHAR aucCharsetHashTable[WE_CSC_HASH_TABLE_SIZE] =
{
   30,   255,    4,   18,   7,  38, 255,   24,  26, 34, 
   255,  255,   28,    0,   3,  12,  17,   14,  21, 39, 
   15,     9,    1,  255,  25,  19,   8,   20,  37, 40, 
   255,   11,   36,  255, 255,   6,   2,  255, 255, 16, 
   22,   255,  255,  255,   5,  255, 32,   33,  27, 29, 
   255,  255,   13,   35,  31,   10, 23,   41, 255
}; //   aucCharsetHashTable

static const WE_UCHAR  aucCharsetString[WE_CSC_STRINGS_TABLE_SIZE][20] = 
{
   "us-ascii",
   "ANSI_X3.4-1968",
   "iso-ir-6",
   "ansi_x3.4-1986",
   "iso_646.irv:1991",
   "ascii",
   "iso646-us",
   "us",
   "ibm367",
   "cp367",
   "csascii",
   "iso-8859-1",
   "latin-1",
   "iso_8859-1:1987",
   "iso-ir-100",
   "iso-8859-1",
   "latin1",
   "l1",
   "ibm819",
   "cp819",
   "csisolatin1",
   "utf-8",
   "iso-10646-ucs-2",
   "csunicode",
   "utf-16be",
   "utf-16le",
   "utf-16",
   "big5",
   "gb2312",
   "iso-8859-2",
   "latin2",
   "iso-8859-2",
   "iso-ir-101",
   "csISOLatin2",
   "iso-8859-8",
   "hebrew",
   "iso-ir-138",
   "csISOLatinHebrew",
   "GBK",
   "GB18030",
   "UCS-4",
   "unicode"
}; // aucCharsetString

static const WE_INT32 aunCharsetInt[WE_CSC_STRINGS_TABLE_SIZE] =
{
   WE_CSC_ASCII,        /* "us-ascii"           */
   WE_CSC_ASCII,        /* "ANSI_X3.4-1968"     */
   WE_CSC_ASCII,        /* "iso-ir-6"           */
   WE_CSC_ASCII,        /* "ansi_x3.4-1986"     */
   WE_CSC_ASCII,        /* "iso_646.irv:1991"   */
   WE_CSC_ASCII,        /* "ascii"              */
   WE_CSC_ASCII,        /* "iso646-us"          */
   WE_CSC_ASCII,        /* "us"                 */
   WE_CSC_ASCII,        /* "ibm367"             */
   WE_CSC_ASCII,        /* "cp367"              */
   WE_CSC_ASCII,        /* "csascii"            */
   WE_CSC_LATIN_1,      /* "iso-8859-1"         */
   WE_CSC_LATIN_1,      /* "latin-1"            */
   WE_CSC_LATIN_1,      /* "iso_8859-1:1987"    */
   WE_CSC_LATIN_1,      /* "iso-ir-100"         */
   WE_CSC_LATIN_1,      /* "iso-8859-1"         */
   WE_CSC_LATIN_1,      /* "latin1"             */
   WE_CSC_LATIN_1,      /* "l1"                 */
   WE_CSC_LATIN_1,      /* "ibm819"             */
   WE_CSC_LATIN_1,      /* "cp819"              */
   WE_CSC_LATIN_1,      /* "csisolatin1"        */
   WE_CSC_UTF_8,        /* "utf-8"              */
   WE_CSC_UCS_2,        /* "iso-10646-ucs-2"    */
   WE_CSC_UCS_2,        /* "csunicode"          */
   WE_CSC_UTF_16BE,     /* "utf-16be"           */
   WE_CSC_UTF_16LE,     /* "utf-16le"           */
   WE_CSC_UTF_16,       /* "utf-16"             */
   WE_CSC_BIG5,         /* "big5"               */
   WE_CSC_GB2312,       /* "gb2312"             */
   WE_CSC_LATIN_2,      /* "iso-8859-2"         */
   WE_CSC_LATIN_2,      /* "latin2"             */
   WE_CSC_LATIN_2,      /* "iso-8859-2"         */
   WE_CSC_LATIN_2,      /* "iso-ir-101"         */
   WE_CSC_LATIN_2,      /* "csISOLatin2"        */
   WE_CSC_HEBREW,       /* "iso-8859-8"         */
   WE_CSC_HEBREW,       /* "hebrew"             */
   WE_CSC_HEBREW,       /* "iso-ir-138"         */
   WE_CSC_HEBREW,       /* "csISOLatinHebrew"   */
   WE_CSC_GBK,          /* "GBK"                */
   WE_CSC_GB18030,      /* "GB13000"            */
   WE_CSC_UCS_4,        /* "UCS-4"              */
   WE_CSC_UNICODE       /*  "unicode"           */
}; // aunCharsetInt

/******************************************************************************
 * Define Structs
 *****************************************************************************/

/*the charset struct */
typedef struct tagSt_CSCCvt
{
   WE_INT32                iFromCharset;     /*charset type                   */
   WE_INT32		            iToCharset;       /* charset type                  */
   Fn_We_Charset_Convert*     pfnConvert;       /* charset convert founction     */
}St_CSCCvt, *P_St_CSCCvt;

/*query info*/
typedef struct tagSt_QueryInfo
{      
   WE_UINT16    usMinCode;
   WE_UINT16    usMaxCode;    
   WE_INT32     iCount;
   WE_UINT16    usOffset;    
   WE_INT16     sContinue;
}St_QueryInfo;

/*the csc main struct*/
typedef struct tagSt_CSC
{
      St_CSCCvt               *pstCvtTable;
    St_WeSCLStrtableInfo      *pstCharsetInfo;
}St_CSC, *P_St_CSC;


/******************************************************************************
 * Declare Functions
 *****************************************************************************/
static E_WECSCERR WE_CSC_Init(St_CSC  *pstMe);
static void    WE_CSC_Clear(St_CSC  *pstMe);


static WE_BOOL WE_CSC_AsciiToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Latin1ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf16beToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf16leToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf16ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Ucs2beToUtf8 (void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Ucs2leToUtf8 (void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Ucs2ToUtf8 (void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToAscii(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToLatin1(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToUtf16be (void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToUtf16le(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToUtf16(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToUcs2be(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToUcs2le(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToUcs2(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_AsciiToUtf16be(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_AsciiToUtf16le(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Latin1ToUtf16be(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Latin1ToUtf16le(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf16beToUtf16le(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
#ifdef   WE_CSC_CFG_SUPPORT_BIG5
static WE_BOOL WE_CSC_Big5ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToBig5(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
#endif 
#ifdef   WE_CSC_CFG_SUPPORT_GB2312
static WE_BOOL WE_CSC_Gb2312ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToGb2312(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
#endif

static WE_BOOL WE_CSC_Latin2ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToLatin2(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_HebrewToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToHebrew(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Utf8ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_GBK2UTF8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_GB18030ToUTF8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_UTF8ToGB18030(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);

static WE_BOOL WE_CSC_KOI8RToUTF8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
static WE_BOOL WE_CSC_Windows1251ToUTF8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);

/******************************************************************************
 * Define Functions
 *****************************************************************************/

E_WECSCERR  WE_CSC_Create(HCSC *phCSC)
{
   E_WECSCERR  eRet     = E_WECSC_FAILED;
   
   St_CSC   *pstCSC  = NULL;

   if (NULL == phCSC) 
   {
      return E_WECSC_BAD_PRARAM;
   }

   pstCSC   = WE_CSC_MALLOC(sizeof(St_CSC));
   if (NULL == pstCSC) 
   {
      return E_WECSC_NO_ENOUGH_MEMERY;
   }

   WE_CSC_MEMSET(pstCSC, 0x00, sizeof(St_CSC));

   eRet  = WE_CSC_Init(pstCSC);
   if (E_WECSC_SUCCESS != eRet) 
   {
      WE_CSC_FREEIF(pstCSC);
      pstCSC = NULL;
   }

   *phCSC   = (HCSC)pstCSC;

   return eRet;
}//   WE_CSC_Create

void     WE_CSC_Delete(HCSC *phCSC)
{
   St_CSC   *pstCSC     = NULL;

   if (NULL == phCSC || NULL == *phCSC) 
   {
      return;
   }

   pstCSC = (St_CSC *)(*phCSC);

   WE_CSC_Clear(pstCSC);

   WE_CSC_FREEIF(pstCSC);
   pstCSC   = NULL;
   *phCSC   = NULL;
}//   WE_CSC_Delete


///////////////////////////////////////////////////////////////////////////////
Fn_We_Charset_Convert * WE_CSC_FindFunctionByVal(HCSC hCSC, WE_INT32 iFrom, WE_INT32 iTo)
{
   WE_INT32    iIndex   = 0;
   St_CSC   *pstCSC  = NULL;

   if(NULL == hCSC)
   {
      return NULL;
   }
   pstCSC = (St_CSC *)hCSC;
   if (NULL == pstCSC->pstCvtTable) 
   {
      return NULL;
   }

   for(iIndex = 0; pstCSC->pstCvtTable[iIndex].iFromCharset != 0; iIndex++)        
   {
      if((pstCSC->pstCvtTable[iIndex].iFromCharset == iFrom) \
         && (pstCSC->pstCvtTable[iIndex].iToCharset == iTo))
      {
         return  (pstCSC->pstCvtTable[iIndex].pfnConvert);
      }
   }
   
    return  NULL;
}//   WE_CSC_FindFunctionByVal


Fn_We_Charset_Convert * WE_CSC_FindFunctionByROM(HCSC hCSC, WE_INT32 iFrom, WE_INT32 iTo, const WE_CHAR *pszStr, WE_INT32 *piStrLen)
{
   St_CSC   *pstCSC     = NULL;

   WE_INT8    ucOrderOfByte   = 0;
   WE_INT32   uiLenOfUsed     = 0;
   WE_INT32   iIndex          = 0;
   
   if( NULL == piStrLen || NULL == hCSC || NULL == pszStr)
   {
      return NULL;    
   }
   
   pstCSC   = (St_CSC *)hCSC;
   if (NULL == pstCSC->pstCvtTable) 
   {
      return NULL;
   }

   /* Check for Byte-Order Mark (BOM) to determine which encoding to use. */
   if (*piStrLen >= 2) 
   {
      if (((WE_UCHAR)pszStr[0] == 0xfe) && ((WE_UCHAR)pszStr[1] == 0xff)) 
      {
         ucOrderOfByte = WE_CSC_BIG_ENDIAN;
         uiLenOfUsed = 2;
      }
      else if (((WE_UCHAR)pszStr[0] == 0xff) && ((WE_UCHAR)pszStr[1] == 0xfe))
      {
         ucOrderOfByte = WE_CSC_LITTLE_ENDIAN;
         uiLenOfUsed = 2;
      }
      else if ((iFrom == WE_CSC_UCS_2) ||
         (iFrom == WE_CSC_UTF_16)) 
      {
         /* We really need the byte order! */
         if (((WE_UCHAR)pszStr[0] == 0) && ((WE_UCHAR)pszStr[1] != 0))
         {
            ucOrderOfByte = WE_CSC_BIG_ENDIAN;
         }
         else if (((WE_UCHAR)pszStr[0] != 0) && ((WE_UCHAR)pszStr[1] == 0))
         {
            ucOrderOfByte = WE_CSC_LITTLE_ENDIAN;
         }
      }
   }
   *piStrLen = uiLenOfUsed;
   
   if (ucOrderOfByte == WE_CSC_BIG_ENDIAN)
   {
      if (iFrom == WE_CSC_UCS_2)
      {
         iFrom = WE_CSC_UCS_2BE;
      }
      else if (iFrom == WE_CSC_UTF_16)
      {
         iFrom = WE_CSC_UTF_16BE;
      }
   }
   else if (ucOrderOfByte == WE_CSC_LITTLE_ENDIAN) 
   {
      if (iFrom == WE_CSC_UCS_2)
      {
         iFrom = WE_CSC_UCS_2LE;
      }
      else if (iFrom == WE_CSC_UTF_16) 
      {
         iFrom = WE_CSC_UTF_16LE;
      }
   }
   
   if (iTo == WE_CSC_UCS_2)
   {
      iTo = WE_CSC_UCS_2BE;
   }
   else if (iTo == WE_CSC_UTF_16) 
   {
      iTo = WE_CSC_UTF_16BE;
   }
   
   for (iIndex = 0; pstCSC->pstCvtTable[iIndex].iFromCharset!= 0; iIndex++)
   {
      if ((pstCSC->pstCvtTable[iIndex].iFromCharset == iFrom) &&
         (pstCSC->pstCvtTable[iIndex].iToCharset == iTo))
      {
         return pstCSC->pstCvtTable[iIndex].pfnConvert;
      }
   }
   
   return NULL;
}//   WE_CSC_FindFunctionByROM

///////////////////////////////////////////////////////////////////////////////
WE_UINT32  WE_CSC_Char_Utf8ToUnicode(HCSC hCSC, const WE_CHAR *pszUtf8, WE_UINT32 *puiUnicode)
{
    WE_UINT32 uiResult = 0;
    WE_UINT32  ucNumOfByte = 0;
    if(NULL == pszUtf8 || NULL == puiUnicode || NULL == hCSC)
    {
       return E_WECSC_BAD_PRARAM;
    }
    
    /* Read one Unicode character in UTF-8 format: */
    ucNumOfByte = aucUtf8BytePerChar[(WE_UCHAR)*pszUtf8 >> 4];
    if(ucNumOfByte > (WE_UINT32)WE_CSC_STRLEN(pszUtf8))
    {
       return FALSE;
    }
    
    uiResult = 0;
    switch (ucNumOfByte)
    {
    case 0:
       return FALSE;
       
    case 4: uiResult += (WE_UCHAR)*pszUtf8++; uiResult <<= 6;
    case 3: uiResult += (WE_UCHAR)*pszUtf8++; uiResult <<= 6;
    case 2: uiResult += (WE_UCHAR)*pszUtf8++; uiResult <<= 6;
    case 1: uiResult += (WE_UCHAR)*pszUtf8++;
    }

    *puiUnicode = uiResult - aunUtf8Offset[ucNumOfByte];
    
    return ucNumOfByte;
}//   WE_CSC_Utf8ToUnicode

WE_UINT32  WE_CSC_Char_UnicodeToUtf8(HCSC hCSC, const WE_UINT32 uiUnicode, WE_CHAR *pszUtf8)
{
   WE_UINT8   ucNumOfByte = 0;
   WE_UCHAR   ucMaskOfUttf8 = 0;
   
   /* Determine how many UTF8-bytes are needed for this character,
   * and which bits should be set in the first byte. */
   if(/*pStrSource == NULL ||*/ NULL == hCSC)
   {
      return E_WECSC_BAD_PRARAM;
   }

   if (uiUnicode < 0x80) 
   {
      ucNumOfByte = 1;
      ucMaskOfUttf8 = 0;
   }
   else if (uiUnicode < 0x800) 
   {
      ucNumOfByte = 2;
      ucMaskOfUttf8 = 0xC0;
   }
   else if (uiUnicode < 0x10000) 
   {
      ucNumOfByte = 3;
      ucMaskOfUttf8 = 0xE0;
   }
   else if (uiUnicode < 0x200000) 
   {
      ucNumOfByte = 4;
      ucMaskOfUttf8 = 0xF0;
   }
   else 
   {
      return FALSE;
   }
   
   
   if (pszUtf8 != NULL)
   {
      WE_UINT32   uiTemp   = uiUnicode;
      switch (ucNumOfByte)
      {
      case 4:
         pszUtf8[3] = (WE_UCHAR)((uiTemp & 0x3F) | 0x80);
         uiTemp >>= 6;
      case 3:
         pszUtf8[2] = (WE_UCHAR)((uiTemp & 0x3F) | 0x80);
         uiTemp >>= 6;
      case 2:
         pszUtf8[1] = (WE_UCHAR)((uiTemp & 0x3F) | 0x80);
         uiTemp >>= 6;
      case 1:
         pszUtf8[0] = (WE_UCHAR)(uiTemp | ucMaskOfUttf8);
      }
   }
   
   return ucNumOfByte;

}//   WE_CSC_Char_UnicodeToUtf8

///////////////////////////////////////////////////////////////////////////////
const char* WE_CSC_Int2Str(HCSC hCSC, WE_INT32 iCharset)
{   
   St_CSC   *pstCSC     = NULL;
   
   const char  *pszCharset = NULL;

   if(NULL == hCSC)
   {
      return NULL;
   }  

   pstCSC = (St_CSC *)hCSC;
   
   pszCharset = (const char *)WE_SCL_INT2STR(iCharset, pstCSC->pstCharsetInfo);   
   if (pszCharset == NULL) 
   {
      WE_INT32 i = 0;
      for (i = 0 ; i < WE_CSC_STRINGS_TABLE_SIZE; i++) 
      {
         if (pstCSC->pstCharsetInfo->pstStringTable[i].iValue == iCharset) 
         {
            pszCharset = pstCSC->pstCharsetInfo->pstStringTable[i].pcName;
            break;
         }
      }
    }

   return pszCharset;
}//   WE_CSC_Int2Str

WE_INT32 WE_CSC_Str2Int(HCSC hCSC, const WE_CHAR *pszCharset, WE_INT32 iLen)
{
   WE_INT32    iCharset = WE_CSC_UNKNOWN;
   St_CSC   *pstCSC  = NULL;

   if (NULL == hCSC || NULL == pszCharset || iLen <= 0) 
   {
      return WE_CSC_UNKNOWN;
   }
   pstCSC = (St_CSC *)hCSC;

   iCharset = WE_SCL_STR2INTLC(pszCharset, iLen, pstCSC->pstCharsetInfo);
   if (iCharset < 0) 
   {
      WE_INT32  i = 0;      
      iCharset = WE_CSC_UNKNOWN;
      
      for (i = 0; i < WE_CSC_STRINGS_TABLE_SIZE; i++) 
      {
         if (0 == WE_SCL_STRNCMPNC (pstCSC->pstCharsetInfo->pstStringTable[i].pcName, pszCharset, iLen)) 
         {
            iCharset = pstCSC->pstCharsetInfo->pstStringTable[i].iValue;
            break;
         }
         
      }
   }

   return iCharset;
}//   WE_CSC_Str2Int


///////////////////////////////////////////////////////////////////////////////
// Static Functions
///////////////////////////////////////////////////////////////////////////////
static E_WECSCERR    WE_CSC_CreateCvtTable(St_CSCCvt **ppstCvtTable)
{
   E_WECSCERR     eRet           = E_WECSC_FAILED;
   WE_INT32       iIndex         = 0;
   St_CSCCvt   *pstCvtTable   = NULL;

   if (NULL == ppstCvtTable) 
   {
      return E_WECSC_BAD_PRARAM;
   }

   do 
   {
   	pstCvtTable   = (St_CSCCvt *)WE_CSC_MALLOC(WE_CSC_CVT_TABLE_SIZE * sizeof(St_CSCCvt));
      if (NULL == pstCvtTable) 
      {
         eRet  = E_WECSC_NO_ENOUGH_MEMERY;
         break;
      }
      WE_CSC_MEMSET(pstCvtTable, 0x00, WE_CSC_CVT_TABLE_SIZE * sizeof(St_CSCCvt));
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_ASCII ;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_AsciiToUtf8;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_LATIN_1;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Latin1ToUtf8;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_16BE;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf16beToUtf8;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_16LE;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf16leToUtf8;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_16 ;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf16ToUtf8;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UCS_2BE;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Ucs2beToUtf8 ;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UCS_2LE;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Ucs2leToUtf8 ;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UCS_2 ;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Ucs2ToUtf8 ;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8 ;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_ASCII;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToAscii;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_LATIN_1;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToLatin1;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_16BE;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToUtf16be ;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_16LE;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToUtf16le;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_16;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToUtf16;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UCS_2BE;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToUcs2be;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UCS_2LE;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToUcs2le;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UCS_2;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToUcs2;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_ASCII;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_16BE;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_AsciiToUtf16be;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_ASCII;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_16LE;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_AsciiToUtf16le;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_LATIN_1;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_16BE;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Latin1ToUtf16be;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_LATIN_1;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_16LE;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Latin1ToUtf16le;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_16BE;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_16LE;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf16beToUtf16le;
      iIndex ++;

      #ifdef   WE_CSC_CFG_SUPPORT_BIG5
      pstCvtTable[iIndex].iFromCharset = WE_CSC_BIG5;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Big5ToUtf8;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_BIG5;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToBig5;
      iIndex ++;
      #endif

      #ifdef   WE_CSC_CFG_SUPPORT_GB2312
      pstCvtTable[iIndex].iFromCharset = WE_CSC_GB2312;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Gb2312ToUtf8;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_GB2312;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToGb2312;
      iIndex ++;
      #endif
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_LATIN_2;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Latin2ToUtf8;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_LATIN_2;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToLatin2;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_HEBREW;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_HebrewToUtf8;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_HEBREW;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToHebrew;
      iIndex ++;
      
      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToUtf8;
      iIndex ++;
      
#ifdef WE_CSC_CFG_SUPPORT_GBK   
      pstCvtTable[iIndex].iFromCharset = WE_CSC_GBK;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_GBK2UTF8;
      iIndex ++;
#endif   // #ifdef WE_CSC_CFG_SUPPORT_GBK      

//#ifdef WE_CSC_CFG_SUPPORT_GB18030
      pstCvtTable[iIndex].iFromCharset = WE_CSC_GB18030;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_GB18030ToUTF8;
      iIndex ++;  

      pstCvtTable[iIndex].iFromCharset = WE_CSC_UCS_4;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_GB18030ToUTF8;
      iIndex ++;  
//#endif //   #ifdef WE_CSC_CFG_SUPPORT_GB18030


      /*add the convert between unicode and utf8*/

      pstCvtTable[iIndex].iFromCharset = WE_CSC_UNICODE;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_UnicodeToUtf8;
      iIndex ++; 

      pstCvtTable[iIndex].iFromCharset = WE_CSC_UTF_8;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UNICODE;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Utf8ToUnicode;
      iIndex ++; 

      pstCvtTable[iIndex].iFromCharset = WE_CSC_KOI8_R;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_KOI8RToUTF8;
      iIndex ++; 

      pstCvtTable[iIndex].iFromCharset = WE_CSC_WINDOWS_1251;
      pstCvtTable[iIndex].iToCharset   = WE_CSC_UTF_8;
      pstCvtTable[iIndex].pfnConvert   = WE_CSC_Windows1251ToUTF8;
      iIndex ++; 
      
      pstCvtTable[iIndex].iFromCharset = 0;
      pstCvtTable[iIndex].iToCharset   = 0;
      pstCvtTable[iIndex].pfnConvert   = NULL;
      iIndex ++;

      *ppstCvtTable  = pstCvtTable;
      eRet = E_WECSC_SUCCESS;

   } while(0);

   if (E_WECSC_SUCCESS != eRet) 
   {
      WE_CSC_FREEIF(pstCvtTable);
      pstCvtTable    = NULL;
      *ppstCvtTable  = NULL;
   }

   return eRet;
}//   WE_CSC_CreateCvtTable

static void    WE_CSC_DeleteCvtTable(St_CSCCvt **ppstCvtTable)
{
   if (NULL == ppstCvtTable || NULL == *ppstCvtTable) 
   {
      return ;
   }

   WE_CSC_FREEIF(*ppstCvtTable);
   *ppstCvtTable  = NULL;
}//   WE_CSC_DeleteCvtTable

///////////////////////////////////////////////////////////////////////////////
static E_WECSCERR   WE_CSC_CreateCharsetInfo(St_WeSCLStrtableInfo  **ppstInfo)
{
   E_WECSCERR  eRet     = E_WECSC_FAILED;
   
   WE_INT32    iIndex   = 0;
   
   St_WeSCLStrtableInfo *pstInfo = NULL;
   
   if (NULL == ppstInfo) 
   {
      return E_WECSC_BAD_PRARAM;
   }
   
   do 
   {
      pstInfo  = WE_CSC_MALLOC(sizeof(St_WeSCLStrtableInfo));
      if (NULL == pstInfo) 
      {
         eRet  = E_WECSC_NO_ENOUGH_MEMERY;
         break;
      }
      WE_CSC_MEMSET(pstInfo, 0x00, sizeof(St_WeSCLStrtableInfo));
      
      pstInfo->pstStringTable   = WE_CSC_MALLOC(WE_CSC_STRINGS_TABLE_SIZE * sizeof(St_WeSCLStrTableEntry));
      if (NULL == pstInfo->pstStringTable) 
      {
         eRet  = E_WECSC_NO_ENOUGH_MEMERY;
         break;
      }
      WE_CSC_MEMSET(pstInfo->pstStringTable, 0x00, WE_CSC_STRINGS_TABLE_SIZE * sizeof(St_WeSCLStrTableEntry));
      
      for (iIndex = 0; iIndex < WE_CSC_STRINGS_TABLE_SIZE; iIndex++)
      {
         pstInfo->pstStringTable[iIndex].iValue    = aunCharsetInt[iIndex];
         pstInfo->pstStringTable[iIndex].pcName    = (char *)aucCharsetString[iIndex];
      }// for
      
      pstInfo->pucHashTable     = (unsigned char *)aucCharsetHashTable;
      pstInfo->iHashTableSize   = WE_CSC_HASH_TABLE_SIZE;
      pstInfo->iStringTableSize = WE_CSC_STRINGS_TABLE_SIZE;
      pstInfo->sSearchMethod    = WE_TABLE_SEARCH_LINEAR;
      
      *ppstInfo   = pstInfo;
      
      eRet     = E_WECSC_SUCCESS;
   } while(0);
   
   if (E_WECSC_SUCCESS != eRet) 
   {
      if (NULL != pstInfo) 
      {
         WE_CSC_FREEIF(pstInfo->pstStringTable);
      }
      WE_CSC_FREEIF(pstInfo);
      
      *ppstInfo   = NULL;
   }
   return eRet;
}//   WE_CSC_CreateCharsetInfo

static void WE_CSC_DeleteCharsetInfo(St_WeSCLStrtableInfo  **ppstInfo)
{
   if (NULL == ppstInfo || NULL == *ppstInfo) 
   {
      return;
   }
   
   WE_CSC_FREEIF( (*ppstInfo)->pstStringTable );
   WE_CSC_FREEIF(*ppstInfo);
}//   WE_CSC_DeleteCharsetInfo

///////////////////////////////////////////////////////////////////////////////

static E_WECSCERR  WE_CSC_Init(St_CSC *pstMe)
{
   E_WECSCERR  eRet  = E_WECSC_FAILED;

   if (NULL == pstMe) 
   {
      return E_WECSC_BAD_PRARAM;
   }

   do 
   {
      
      /* Init the Charset Table */
      eRet  = WE_CSC_CreateCharsetInfo(&pstMe->pstCharsetInfo);
      if (E_WECSC_SUCCESS != eRet || NULL == pstMe->pstCharsetInfo) 
      {
         WE_CSC_LOG_ERR(("CSC: Create Charset Table Info Failed!"));
         eRet  = E_WECSC_FAILED;
         break;
      }
   
      eRet  = WE_CSC_CreateCvtTable(&pstMe->pstCvtTable);
      if (E_WECSC_SUCCESS != eRet || NULL == pstMe->pstCvtTable) 
      {
         WE_CSC_LOG_ERR(("CSC: Create Charset Cvt Table Failed!"));
         eRet  = E_WECSC_FAILED;
         break;
      }

   } while(0);

   return eRet;
}//   WE_CSC_Init


static void WE_CSC_Clear(St_CSC  *pstMe)
{   
   if (NULL == pstMe) 
   {
      return;
   }

   WE_CSC_DeleteCharsetInfo(&pstMe->pstCharsetInfo);
   WE_CSC_DeleteCvtTable(&pstMe->pstCvtTable);

}//   WE_CSC_Clear

///////////////////////////////////////////////////////////////////////////////
static WE_BOOL WE_CSC_AsciiToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_CHAR     *pcTemp  = NULL;
   WE_UINT32   uiLen    = 0; 

   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }

   uiLen = MIN (*plSrcLen, *plDstLen);

   if (pcDst != NULL)
   {
      for (pcTemp = pcDst; pcTemp < pcDst + uiLen; pcTemp++, pcSrc++) 
      {
         if (((WE_UCHAR)*pcSrc) < 0x80)
         {
            *pcTemp = *pcSrc;
         }
         else
         {
            *pcTemp = WE_CSC_NO_ASCII_CHAR; /* Non-ASCII characters become '?' */
         }
      }
   }
   *plSrcLen = uiLen;
   *plDstLen = uiLen;
  
   return TRUE;
}

static WE_BOOL WE_CSC_AsciiToUtf16any(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen, WE_BOOL bBigEndian)
{
   WE_INT32       iSindex        = 0;
   WE_INT32       iDindex        = 0;
   WE_UINT32      uiResult       = 0;
   WE_UINT32      uiNumFirst     = 0;
   WE_UINT32      uiNumSecond    = 0;   
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   if (bBigEndian) 
   {
      uiNumFirst = 0; 
      uiNumSecond = 1;
   }
   else 
   {
      uiNumFirst = 1; 
      uiNumSecond = 0;
   }
   
   while (iSindex < *plSrcLen) 
   {
      /* Read one Unicode character in ASCII: */
      if (iSindex + 1 > *plSrcLen)
         break;
      
      uiResult = (WE_UCHAR)*pcSrc++;
      
      /* Write one Unicode character in UTF-16 format: */
      if (pcDst != NULL) 
      {
         if (iDindex + 2 > *plDstLen)
            break;
         
         pcDst[iDindex + uiNumFirst]   = 0;
         pcDst[iDindex + uiNumSecond]  = (WE_CHAR)((uiResult & 0xFF));
      }
      
      iSindex += 1;
      iDindex += 2;
   }
   
   *plSrcLen = iSindex;
   *plDstLen = iDindex;
   
   return TRUE;
}//   WE_CSC_AsciiToUtf16le

static WE_BOOL WE_CSC_AsciiToUtf16be(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
    
   return WE_CSC_AsciiToUtf16any(pMe, pcSrc, plSrcLen, pcDst, plDstLen,TRUE);
}

static WE_BOOL WE_CSC_AsciiToUtf16le(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_AsciiToUtf16any(pMe, pcSrc, plSrcLen, pcDst, plDstLen,FALSE);
}


static WE_BOOL WE_CSC_Utf8ToAscii(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_UINT32     uiSindex        = 0;
   WE_UINT32     uiDindex        = 0;
   WE_INT32      iSouNumOfByte   = 0;
   WE_UINT32     uiResult        = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   while ((WE_LONG)(uiSindex) < *plSrcLen) 
   {
      /* Read one Unicode character in UTF-8 format: */
      iSouNumOfByte =aucUtf8BytePerChar[(WE_UCHAR)*pcSrc >> 4];
      if ((WE_LONG)(uiSindex + iSouNumOfByte) > *plSrcLen)
         break;
      
      uiResult = 0;
      switch (iSouNumOfByte)
      {
      case 0:
         return FALSE;
         
      case 4: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 3: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 2: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 1: uiResult += (WE_UCHAR)*pcSrc++;
      }
      uiResult -= aunUtf8Offset[iSouNumOfByte];
      
      /* Write one Unicode character in Latin-1 format: */
      if (uiResult> 0x7f)
         uiResult = 0x3f; /* Non-ASCII characters become '?' */
      
      if (pcDst != NULL)
      {
         if((WE_LONG)(uiDindex +1) > *plDstLen)
            break;
         
         pcDst[uiDindex] = (WE_UCHAR)(uiResult & 0xff);
      }
      uiSindex += iSouNumOfByte;
      uiDindex += 1;
   }
   *plSrcLen = uiSindex;
   *plDstLen = uiDindex;
   
   return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
static WE_BOOL WE_CSC_Latin1ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_UINT32   uiTransToUcs   = 0;
   WE_UINT32   uiSindex       = 0;
   WE_UINT32   uiDindex       = 0;
   WE_UINT8    ucNumOfByte    = 0;
   WE_UINT8    ucMaskOfUtf8   = 0;

   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }

   for (;;) 
   {
      /* First, translate to UCS4 */
      if((WE_LONG) (uiSindex + 1) > *plSrcLen)
         break;
      
      uiTransToUcs = (WE_UINT32)((WE_UCHAR)pcSrc[uiSindex]);
      
      /* Determine how many UTF8-bytes are needed for this character,
      * and which bits should be set in the first byte. */
      if (uiTransToUcs < 0x80)
      {
         ucNumOfByte = 1;
         ucMaskOfUtf8 = 0;
      }
      else 
      {
         ucNumOfByte = 2;
         ucMaskOfUtf8 = 0xc0;
      }
      
      /* Only write to destination vector if pcDst != MSF_NULL */
      if (pcDst != NULL)
      {         
         /* Is there room for this in the destination vector? */
         if((WE_LONG) (uiDindex + ucNumOfByte) > *plDstLen)
            break;
         
         switch (ucNumOfByte)
         {
         case 2:
            pcDst[uiDindex + 1] = (WE_UCHAR) ((uiTransToUcs& 0x3f) | 0x80);
            uiTransToUcs >>= 6;
            
         case 1:
            pcDst[uiDindex] = (WE_UCHAR) (uiTransToUcs| ucMaskOfUtf8);
         }
      }
      uiSindex += 1;
      uiDindex += ucNumOfByte;
   }// for
   
   *plSrcLen = uiSindex;
   *plDstLen = uiDindex;
   
   return TRUE;
}

static WE_BOOL WE_CSC_Latin1ToUtf16any(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen, WE_BOOL bBigEndian)
{
   WE_INT32    iSindex     = 0;
   WE_INT32    iDindex     = 0;
   WE_UINT32   uiResult    = 0;
   WE_UINT32   uiNumFirst  = 0;
   WE_UINT32   uiNumSecond = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }

   if (bBigEndian) 
   {
      uiNumFirst = 0; 
      uiNumSecond = 1;
   }
   else 
   {
      uiNumFirst = 1;
      uiNumSecond = 0;
   }
   
   while (iSindex < *plSrcLen) 
   {
      /* Read one Unicode character in Latin-1: */
      if (iSindex + 1 > *plSrcLen)
         break;
      
      uiResult = (WE_UCHAR)*pcSrc++;
      
      /* Write one Unicode character in UTF-16 format: */        
      if (pcDst != NULL) 
      {
         if (iDindex + 2 > *plDstLen)
            break;
         
         pcDst[iDindex + uiNumFirst] = 0;
         pcDst[iDindex + uiNumSecond] = (WE_CHAR)((uiResult & 0xFF));
      }
      
      iSindex += 1;
      iDindex += 2;
   }
   *plSrcLen = iSindex;
    *plDstLen = iDindex;
    
   return TRUE;
}//   WE_CSC_Latin1ToUtf16be

static WE_BOOL WE_CSC_Latin1ToUtf16le(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Latin1ToUtf16any(pMe, pcSrc, plSrcLen, pcDst, plDstLen,FALSE);
}

static WE_BOOL WE_CSC_Latin1ToUtf16be(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Latin1ToUtf16any(pMe, pcSrc, plSrcLen, pcDst, plDstLen,TRUE);
}

static WE_BOOL WE_CSC_Utf8ToLatin1(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   WE_INT32   iSindex         = 0;
   WE_INT32   iDindex         = 0;
   WE_INT32   iSouNumOfByte   = 0;
   WE_UINT32  uiResult        = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   while (iSindex < *plSrcLen)
   {
      /* Read one Unicode character in UTF-8 format: */
      iSouNumOfByte = aucUtf8BytePerChar[(WE_UCHAR)*pcSrc >> 4];
      if (iSindex + iSouNumOfByte > *plSrcLen)
         break;
      
      uiResult = 0;
      switch (iSouNumOfByte) 
      {
      case 0:
         return FALSE;
         
      case 4: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 3: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 2: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 1: uiResult += (WE_UCHAR)*pcSrc++;
      }
      uiResult -= aunUtf8Offset[iSouNumOfByte];
      
      /* Write one Unicode character in Latin-1 format: */
      if (uiResult > 0xff)
         uiResult = 0x3f; /* Non-Latin-1 characters become '?' */
      
      
      if (pcDst != NULL)
      {
         if(iDindex+1 > *plDstLen)
            break;
         
         pcDst[iDindex] = (WE_CHAR)(uiResult & 0xff);
      }
      iSindex += iSouNumOfByte;
      iDindex += 1;
   }
   *plSrcLen = iSindex;
   *plDstLen = iDindex;
   
   return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
static WE_BOOL WE_CSC_Iso8859ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen,  const WE_UINT16 *pusCodeTable)
{
   WE_UINT32  uiResult        = 0;
   WE_INT32   iSindex         = 0;
   WE_INT32   iDindex         = 0;
   WE_INT32   iDesNumOfByte   = 0;
   WE_UINT8   ucMaskOfUtf8    = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   while(1)
   {
      /* First, translate to UCS4 */
      if (iSindex + 1 > *plSrcLen)
         break;
      
      if (((WE_UCHAR)pcSrc[iSindex]) <= 127) 
      {
         uiResult = (WE_UCHAR)pcSrc[iSindex];
      }
      else 
      {
         uiResult = (WE_UINT32)(pusCodeTable[(WE_UCHAR)pcSrc[iSindex] - 128]);
      }
      
      /* Determine how many UTF8-bytes are needed for this character,
      * and which bits should be set in the first byte. */
      if (uiResult < 0x80) 
      {
         iDesNumOfByte = 1;
         ucMaskOfUtf8 = 0;
      }
      else if (uiResult < 0x800) 
      {
         iDesNumOfByte = 2;
         ucMaskOfUtf8 = 0xc0;
      }
      else 
      {
         iDesNumOfByte = 3;
         ucMaskOfUtf8 = 0xe0;
      }
      
      /* Only write to destination vector if pcDst != MSF_NULL */
      if (pcDst != NULL) 
      {        
         /* Is there room for this in the destination vector */           
         if (iDindex + iDesNumOfByte > *plDstLen)
            break;
         
         switch (iDesNumOfByte) 
         {
         case 3:
            pcDst[iDindex + 2] = (WE_CHAR) ((uiResult & 0x3f) | 0x80);
            uiResult >>= 6;
            
         case 2:
            pcDst[iDindex + 1] = (WE_CHAR) ((uiResult & 0x3f) | 0x80);
            uiResult >>= 6;
            
         case 1:
            pcDst[iDindex] = (WE_CHAR) (uiResult | ucMaskOfUtf8);
         }
      }
      iSindex += 1;
      iDindex += iDesNumOfByte;
   }
   *plSrcLen = iSindex;
   *plDstLen = iDindex;
   
   return TRUE;
}//   WE_CSC_Iso8859ToUtf8

///////////////////////////////////////////////////////////////////////////////
static WE_BOOL WE_CSC_Latin2ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Iso8859ToUtf8(pMe, pcSrc, plSrcLen, pcDst, plDstLen, ausLatin2Code);
}

static WE_BOOL WE_CSC_Utf8ToLatin2(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{    
   WE_INT32   iSindex         = 0;
   WE_INT32   iDindex         = 0;
   WE_INT32   iSouNumOfByte   = 0;
   WE_UINT32  uiResult        = 0;
   WE_INT32   iHc             = 0;
   WE_INT32   i               = 0;

   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   while (iSindex < *plSrcLen)
   {
      /* Read one Unicode character in UTF-8 format: */
      iSouNumOfByte = aucUtf8BytePerChar[(WE_UCHAR)*pcSrc >> 4];
      if (iSindex + iSouNumOfByte > *plSrcLen)
         break;
      
      uiResult = 0;
      switch (iSouNumOfByte) 
      {
      case 0:
         return FALSE;
         
      case 4: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 3: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 2: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 1: uiResult += (WE_UCHAR)*pcSrc++;
      }
      uiResult -= aunUtf8Offset[iSouNumOfByte];
      
      /* Encode one Unicode character in Latin-2 alphabet: */
      if (uiResult <= 0x7f) 
      {
         iHc = uiResult;
      }
      else 
      {
         iHc = '?';            
         for (i = 0x80; i <= 0xff; i++)
         {
            if (ausLatin2Code[i] == uiResult)
            {
               iHc = i;
               break;
            }
         }
      }
      
      if (pcDst != NULL) 
      {          
         if (iDindex + 1 > *plDstLen)
            break;
         
         pcDst[iDindex] = (WE_CHAR)(iHc);
      }
      iSindex += iSouNumOfByte;
      iDindex += 1;
   }
   *plSrcLen = iSindex;
   *plDstLen = iDindex;
   
   return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

static WE_BOOL WE_CSC_HebrewToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Iso8859ToUtf8 (pMe, pcSrc, plSrcLen, pcDst, plDstLen, ausHebrewCode);
}

static WE_BOOL WE_CSC_Utf8ToHebrew(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_INT32   iSindex         = 0;
   WE_INT32   iDindex         = 0;
   WE_INT32   iSouNumOfByte   = 0;
   WE_UINT32  uiResult        = 0;
   WE_INT32   iHc             = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return E_WECSC_BAD_PRARAM;
   }
   
   while (iSindex < *plSrcLen) 
   {
      /* Read one Unicode character in UTF-8 format: */
      iSouNumOfByte = aucUtf8BytePerChar[(WE_UCHAR)*pcSrc >> 4];
      if (iSindex + iSouNumOfByte > *plSrcLen)
         break;
      
      uiResult = 0;
      switch (iSouNumOfByte) 
      {
      case 0:
         return FALSE;
         
      case 4: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 3: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 2: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 1: uiResult += (WE_UCHAR)*pcSrc++;
      }
      uiResult -= aunUtf8Offset[iSouNumOfByte];
      
      /* Encode one Unicode character in Hebrew alphabet: */
      if (uiResult == 0xd7) 
      {
         iHc = 0xaa;
      }
      else if (uiResult == 0xf7) 
      {
         iHc = 0xba;
      }
      else if (uiResult == 0x2017) 
      {
         iHc = 0xdf;
      }
      else if (uiResult == 0x200e) 
      {
         iHc = 0xfd;
      }
      else if (uiResult == 0x200f) 
      {
         iHc = 0xfe;
      }
      else if ((uiResult <= 0x7f) || ((uiResult >= 0xa0) && (uiResult <= 0xbe))) 
      {
         iHc = (WE_INT32)uiResult;
      }
      else if ((uiResult >= 0x5d0) && (uiResult <= 0x5ea)) 
      {
         iHc = (uiResult - 0x5d0) + 0xe0;
      }
      else
      {
         iHc = '?';
      }
      
      if (pcDst != NULL)
      {
         if( iDindex +1 > *plDstLen)
            break;
         
         pcDst[iDindex] = (WE_CHAR)(iHc);
      }
      iSindex += iSouNumOfByte;
      iDindex += 1;
   }
   *plSrcLen = iSindex;
   *plDstLen = iDindex;
   
   return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
static WE_BOOL WE_CSC_Utf16beToUtf16le(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_INT32 iMin  = 0;
   WE_INT32 i     = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return E_WECSC_BAD_PRARAM;
   }
   
   iMin     = MIN (*plSrcLen, *plDstLen);

   while (i + 1 < iMin) 
   {
      if (pcDst != NULL)
      {
         pcDst[i] = pcSrc[i + 1];
         pcDst[i + 1] = pcSrc[i];
      }
      i += 2;
   }
   *plSrcLen = i;
   *plDstLen = i;
   
   return TRUE;
}

static WE_BOOL WE_CSC_Utf16AnyToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen, WE_BOOL bBigEndian)
{
   WE_UINT32   uiTransToUcs      = 0;
   WE_UINT32   uiTransToUcsEx    = 0;
   WE_UINT32   uiSindex          = 0;
   WE_UINT32   uiDindex          = 0;
   WE_INT32    iSouNumOfByte     = 0;
   WE_INT32    iDesNumOfByte     = 0;
   WE_UINT8    ucMaskOfUtf8      = 0;
   WE_INT32    iShiftFirst       = 0;
   WE_INT32    iShiftSecond      = 0;
   WE_INT32    iShiftThird       = 0;
   WE_INT32    iShiftForth       = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   if (bBigEndian) 
   {
      iShiftFirst = 0; iShiftSecond = 1; iShiftThird = 2; iShiftForth = 3;
   }
   else 
   {
      iShiftFirst = 1; iShiftSecond = 0; iShiftThird = 3; iShiftForth = 2;
   }
   
   for (;;) 
   {
      /* First, translate to UCS4 */
      if ((WE_LONG)(uiSindex + 2 ) > *plSrcLen)
         break;
      
      uiTransToUcs = ((WE_UCHAR)pcSrc[uiSindex + iShiftFirst] << 8) |
         (WE_UCHAR)pcSrc[uiSindex + iShiftSecond];
      iSouNumOfByte = 2;
      
      if ((uiTransToUcs >= WE_CSC_SURROGATE_HIGH_START) &&
         (uiTransToUcs <= WE_CSC_SURROGATE_HIGH_END)) 
      {
         if ((WE_LONG)(uiSindex + 4 )> *plSrcLen)
            break;
         uiTransToUcsEx = ((WE_UCHAR)pcSrc[uiSindex + iShiftThird] << 8) |
            (WE_UCHAR)pcSrc[uiSindex + iShiftForth];
         iSouNumOfByte = 4;
         if ((uiTransToUcsEx >= WE_CSC_SURROGATE_LOW_START) &&
            (uiTransToUcsEx <= WE_CSC_SURROGATE_LOW_END)) 
         {
            uiTransToUcs = ((uiTransToUcs- WE_CSC_SURROGATE_HIGH_START) << 10)
               + (uiTransToUcsEx - WE_CSC_SURROGATE_LOW_START) + 0x10000UL;
         }
      }
      
      /* Determine how many UTF8-bytes are needed for this character,
      * and which bits should be set in the first byte. */
      if (uiTransToUcs < 0x80) 
      {
         iDesNumOfByte = 1;
         ucMaskOfUtf8 = 0;
      }
      else if (uiTransToUcs < 0x800)
      {
         iDesNumOfByte= 2;
         ucMaskOfUtf8 = 0xc0;
      }
      else if (uiTransToUcs < 0x10000) 
      {
         iDesNumOfByte = 3;
         ucMaskOfUtf8 = 0xe0;
      }
      else if (uiTransToUcs < 0x200000) 
      {
         iDesNumOfByte = 4;
         ucMaskOfUtf8 = 0xf0;
      }
      else 
      {
         iDesNumOfByte = 2;
         uiTransToUcs = WE_CSC_REPLACEMENT_CHAR;
         ucMaskOfUtf8 = 0xc0;
      }
      
      
      /* Only write to destination vector if pcDst != MSF_NULL */
      if (pcDst != NULL)
      {
         /* Is there room for this in the destination vector? */
         if ((WE_LONG)(uiDindex + iDesNumOfByte )> *plDstLen)
            break;
         
         switch (iDesNumOfByte)
         {
         case 4:
            pcDst[uiDindex + 3] = (WE_CHAR)((uiTransToUcs & 0x3f) | 0x80);
            uiTransToUcs>>= 6;
         case 3:
            pcDst[uiDindex + 2] = (WE_CHAR) ((uiTransToUcs & 0x3f) | 0x80);
            uiTransToUcs >>= 6;
         case 2:
            pcDst[uiDindex + 1] = (WE_CHAR) ((uiTransToUcs & 0x3f) | 0x80);
            uiTransToUcs >>= 6;
         case 1:
            pcDst[uiDindex] = (WE_CHAR) (uiTransToUcs | ucMaskOfUtf8);
         }
      }
      
      uiSindex += iSouNumOfByte;
      uiDindex += iDesNumOfByte;
   }
   *plSrcLen = uiSindex;
   *plDstLen = uiDindex;
    
   return TRUE;
}//   WE_CSC_Utf16leToUtf8

static WE_BOOL WE_CSC_Utf16beToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Utf16AnyToUtf8 (pMe,pcSrc, plSrcLen, pcDst, plDstLen, TRUE);
}

static WE_BOOL WE_CSC_Utf16leToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Utf16AnyToUtf8 (pMe,pcSrc, plSrcLen, pcDst, plDstLen, FALSE);
}


static WE_BOOL WE_CSC_Utf16ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Utf16AnyToUtf8 (pMe,pcSrc, plSrcLen, pcDst, plDstLen, TRUE);
}


static WE_BOOL WE_CSC_Utf8ToUtf16any(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen, WE_BOOL bBigEndian)
{
   WE_INT32   iSindex         = 0;
   WE_INT32   iDindex         = 0;
   WE_INT32   iSouNumOfByte   = 0;
   WE_INT32   iDesNumOfByte   = 0;
   WE_UINT32  uiResult        = 0;
   WE_UINT32  uiTmp           = 0;
   WE_INT32   iMaskFirst      = 0;
   WE_INT32   iMaskSecond     = 0;
   WE_INT32   iMaskThird      = 0;
   WE_INT32   iMaskForth      = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   if (bBigEndian) 
   {
      iMaskFirst  = 0; 
      iMaskSecond = 1; 
      iMaskThird  = 2;
      iMaskForth  = 3;
   }
   else
   {
      iMaskFirst  = 1;
      iMaskSecond = 0; 
      iMaskThird  = 3; 
      iMaskForth  = 2;
   }
   
   while (iSindex < *plSrcLen) 
   {
      /* Read one Unicode character in UTF-8 format: */
      iSouNumOfByte = aucUtf8BytePerChar[(WE_UCHAR)*pcSrc >> 4];
      if (iSindex + iSouNumOfByte > *plSrcLen)
         break;
      
      uiResult = 0;
      switch (iSouNumOfByte) 
      {
      case 0:
         return FALSE;
         
      case 4: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 3: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 2: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 1: uiResult += (WE_UCHAR)*pcSrc++;
      }
      uiResult -= aunUtf8Offset[iSouNumOfByte];
      
      /* Write one Unicode character in UTF-16 format: */
      if (uiResult < 0x10000) 
      {           
         if (pcDst != NULL)
         {               
            if (iDindex + 2 > *plDstLen)
               break;
            
            pcDst[iDindex + iMaskFirst] = (WE_CHAR)((uiResult >> 8) & 0xff);
            pcDst[iDindex + iMaskSecond] = (WE_CHAR)((uiResult & 0xff));
         }
         iDesNumOfByte = 2;
      }
      else
      {
         if (pcDst != NULL)
         {               
            if (iDindex + 4 > *plDstLen)
               break;
            
            uiTmp = 0xd7c0 + (uiResult >> 10);
            pcDst[iDindex + iMaskFirst] = (WE_CHAR)((uiTmp >> 8) & 0xff);
            pcDst[iDindex + iMaskSecond] = (WE_CHAR)((uiTmp & 0xff));
            pcDst[iDindex + iMaskThird] = (WE_CHAR)(0xdc | ((uiResult & 0x3ff) >> 8));
            pcDst[iDindex + iMaskForth] = (WE_CHAR)(uiResult & 0xff);
         }
         iDesNumOfByte = 4;
      }
      iSindex += iSouNumOfByte;
      iDindex += iDesNumOfByte;
   }
   *plSrcLen = iSindex;
   *plDstLen = iDindex;
   
   return TRUE;
}//   WE_CSC_Utf8ToUtf16

static WE_BOOL WE_CSC_Utf8ToUtf16be(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Utf8ToUtf16any(pMe, pcSrc, plSrcLen, pcDst, plDstLen, TRUE);
}

static WE_BOOL WE_CSC_Utf8ToUtf16le(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Utf8ToUtf16any(pMe, pcSrc, plSrcLen, pcDst, plDstLen, FALSE);
}

static WE_BOOL WE_CSC_Utf8ToUtf16(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Utf8ToUtf16any(pMe, pcSrc, plSrcLen, pcDst, plDstLen, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
static WE_BOOL WE_CSC_Ucs2AnyToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen, WE_BOOL bBigEndian)
{
   WE_UINT32   uiTransToUcs   = 0;
   WE_UINT32   uiSindex       = 0;
   WE_UINT32   uiDindex       = 0;
   WE_INT32    iSouNumOfByte  = 0; 
   WE_INT32    iDesNumOfByte  = 0;
   WE_UINT8    ucMaskOfUtf8   = 0;
   WE_INT32    iShiftFirst    = 0;
   WE_INT32    iShiftSecond   = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   if (bBigEndian)
   {
      iShiftFirst = 0; iShiftSecond = 1; 
   }
   else
   {
      iShiftFirst = 1; iShiftSecond = 0; 
   }
   
   while(1)
   {
      /* First, translate to UCS4 */
      if ((WE_LONG)(uiSindex + 2 )> *plSrcLen)
         break;
      uiTransToUcs = ((WE_UCHAR)pcSrc[uiSindex + iShiftFirst] << 8) |
         (WE_UCHAR)pcSrc[uiSindex + iShiftSecond];
      iSouNumOfByte = 2;
      
      /* Determine how many UTF8-bytes are needed for this character,
      * and which bits should be set in the first byte. */
      if (uiTransToUcs < 0x80)
      {
         iDesNumOfByte = 1;
         ucMaskOfUtf8 = 0;
      }
      else if (uiTransToUcs < 0x800)
      {
         iDesNumOfByte = 2;
         ucMaskOfUtf8 = 0xc0;
      }
      else 
      {
         iDesNumOfByte = 3;
         ucMaskOfUtf8 = 0xe0;
      }
      
      /* Only write to destination vector if pcDst != MSF_NULL */
      if (pcDst != NULL) 
      {
         /* Is there room for this in the destination vector? */
         if ((WE_LONG)(uiDindex + iDesNumOfByte) > *plDstLen)
            break;
         
         switch (iDesNumOfByte)
         {
         case 3:
            pcDst[uiDindex + 2] = (WE_UCHAR) ((uiTransToUcs & 0x3f) | 0x80);
            uiTransToUcs >>= 6;
         case 2:
            pcDst[uiDindex + 1] = (WE_UCHAR) ((uiTransToUcs & 0x3f) | 0x80);
            uiTransToUcs >>= 6;
         case 1:
            pcDst[uiDindex] = (WE_UCHAR) (uiTransToUcs | ucMaskOfUtf8);
         }
      }
      
      uiSindex += iSouNumOfByte;
      uiDindex += iDesNumOfByte;
   }
   *plSrcLen = uiSindex;
   *plDstLen = uiDindex;

   return TRUE;
}//   WE_CSC_Ucs2AnyToUtf8

static WE_BOOL WE_CSC_Ucs2beToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Ucs2AnyToUtf8 (pMe,pcSrc, plSrcLen, pcDst, plDstLen, TRUE);
}

static WE_BOOL WE_CSC_Ucs2leToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Ucs2AnyToUtf8(pMe,pcSrc, plSrcLen, pcDst, plDstLen, FALSE);
}

static WE_BOOL WE_CSC_Ucs2ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Ucs2AnyToUtf8(pMe,pcSrc, plSrcLen, pcDst, plDstLen, TRUE);
}


static WE_BOOL WE_CSC_Utf8ToUcs2any(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen, WE_BOOL bBigEndian)
{   
   WE_INT32        iSindex       = 0;
   WE_INT32        iDindex       = 0;
   WE_INT32        iSouNumOfByte = 0; 
   WE_INT32        iDesNumOfByte = 0;
   WE_UINT32       uiResult      = 0;
   WE_UINT32       uiNumFirst    = 0; 
   WE_UINT32       uiNumSecond   = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }

   if (bBigEndian)
   {
      uiNumFirst = 0; 
      uiNumSecond = 1;
   }
   else
   {
      uiNumFirst = 1; 
      uiNumSecond = 0;
   }
   
   while (iSindex < *plSrcLen)
   {
      /* Read one Unicode character in UTF-8 format: */
      iSouNumOfByte = aucUtf8BytePerChar[(WE_UCHAR)*pcSrc >> 4];
      if (iSindex + iSouNumOfByte > *plSrcLen)
         break;
      
      uiResult = 0;
      switch (iSouNumOfByte) 
      {
      case 0:
         return FALSE;
         
      case 4: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 3: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 2: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 1: uiResult += (WE_UCHAR)*pcSrc++;
      }
      
      uiResult -= aunUtf8Offset[iSouNumOfByte];
      
      /* Write one Unicode character in UCS-2 format: */
      uiResult &= 0xffff;
      
      
      
      if (pcDst != NULL) 
      {
         if (iDindex + 2 > *plDstLen)
            break;
         
         pcDst[iDindex + uiNumFirst] = (WE_CHAR)((uiResult >> 8) & 0xff);
         pcDst[iDindex + uiNumSecond] = (WE_CHAR)((uiResult & 0xff));
      }
      
      iDesNumOfByte = 2;
      iSindex += iSouNumOfByte;
      iDindex += iDesNumOfByte;
   }
   
   *plSrcLen = iSindex;
   *plDstLen = iDindex;
       
   return TRUE;
}//   WE_CSC_Utf8ToUcs2any

static WE_BOOL WE_CSC_Utf8ToUcs2be(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }

   return WE_CSC_Utf8ToUcs2any(pMe, pcSrc, plSrcLen, pcDst, plDstLen, TRUE);
}

static WE_BOOL WE_CSC_Utf8ToUcs2le(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Utf8ToUcs2any(pMe, pcSrc, plSrcLen, pcDst, plDstLen, FALSE);
}

static WE_BOOL WE_CSC_Utf8ToUcs2(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   return WE_CSC_Utf8ToUcs2any(pMe, pcSrc, plSrcLen, pcDst, plDstLen, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
static WE_BOOL WE_CSC_Utf8ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_INT32    iSindex  = 0;
   WE_INT32    iDindex  = 0;
   WE_INT32    iSnum    = 0;
   WE_INT32    iCharOk  = FALSE;
   WE_UINT32   uiResult = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   while (iSindex < *plSrcLen) 
   {
      /* Read one Unicode character in UTF-8 format: */
      iSnum = aucUtf8BytePerChar[(WE_UCHAR)*pcSrc >> 4];
      
      if (iSindex + iSnum > *plSrcLen || iDindex + iSnum > *plDstLen || (iSnum == 0 && iDindex + 3 >= *plDstLen))
         break;
      
      /* Convert to UCS2 */
      if(iSnum > 1)
      {
         uiResult = 0;
         switch (iSnum) 
         {
         case 4: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
         case 3: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
         case 2: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
         case 1: uiResult += (WE_UCHAR)*pcSrc++;
         }
         
         uiResult = uiResult - aunUtf8Offset[iSnum];
         pcSrc -= iSnum;
      }
      switch(iSnum)
      {
      case 0:
         iCharOk = FALSE;
         break;
      case 1:
         iCharOk = TRUE;
         break;
      case 2:
         iCharOk = uiResult >= 0x80UL && (*(pcSrc+1) & 0xc0) == 0x80;
         break;
      case 3:
         iCharOk = uiResult >= 0x800UL && ((*(pcSrc+1) & 0xc0) == 0x80) && ((*(pcSrc+2) & 0xc0) == 0x80);
         break;
      case 4:
         iCharOk = uiResult >= 0x10000UL && !(*pcSrc & 0x08) && ((*(pcSrc+1) & 0xc0) == 0x80) && ((*(pcSrc+2) & 0xc0) == 0x80) && ((*(pcSrc+3) & 0xc0) == 0x80);
         break;
      }
      
      
      if(iCharOk)
      { /* Valid character. Copy the bytes */
         if (pcDst != NULL)
         {
            switch(iSnum)
            {
            case 4: *pcDst++ = *pcSrc++; 
            case 3: *pcDst++ = *pcSrc++;
            case 2: *pcDst++ = *pcSrc++;
            case 1: *pcDst++ = *pcSrc++;
            }
         }
         else
         {
            pcSrc += iSnum;
         }
         iSindex += iSnum;
         iDindex += iSnum;
      }
      else
      { /* Invalid character. Replace with WE_CHARSET_REPLACEMENT_CHAR */ 
         if(iDindex + 3 < *plDstLen)
         {
            WE_INT32  iLen = WE_CSC_Char_UnicodeToUtf8(pMe, WE_CSC_REPLACEMENT_CHAR ,pcDst);
            
            if (pcDst != NULL)
            {
               if(iLen >0)
               {
                  pcDst += iLen;
                  iDindex += iLen;
               }
               
            }
            else
            {
               iDindex += iLen;
            }
         }
         pcSrc++;
         iSindex++;
         /* If this byte was a valid start byte but the character was invalid scan for the next start byte. */
         if(iSnum != 0)
         {
            while(aucUtf8BytePerChar[(WE_UCHAR)*pcSrc >> 4] == 0 && iSindex < *plSrcLen)
            {
               pcSrc++;
               iSindex++;
            }
         }
      }
   }
   *plSrcLen = iSindex;
   *plDstLen = iDindex;
   
   return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
WE_BOOL WE_CSC_UnicodeToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_UINT32   uiResult       = 0;
   WE_INT32    iSindex        = 0;
   WE_INT32    iDindex        = 0;
   WE_INT32    iSouNumOfByte  = 0;
   WE_INT32    iDesNumOfByte  = 0;
   WE_UINT8    uiMask         = 0;
   WE_INT32    iShiftFirst    = 0;
   WE_INT32    iShiftSecond   = 0;
   WE_UINT16   check = 1;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return E_WECSC_BAD_PRARAM;
   }

   if(*(WE_CHAR*)&check == 1)
   {
      iShiftFirst = 0; 
      iShiftSecond = 1; 
   }
   else
   {
      iShiftFirst = 1; 
      iShiftSecond = 0; 
   }
   
   while(1)
   {
      /* First, translate to UCS4 */
      if (iSindex + 2 > *plSrcLen)
         break;
      uiResult = ((WE_UCHAR)pcSrc[iSindex + iShiftSecond] << 8) |
         (WE_UCHAR)pcSrc[iSindex + iShiftFirst];
      iSouNumOfByte = 2;
      
      /* Determine how many UTF8-bytes are needed for this character,
      * and which bits should be set in the first byte. */
      if (uiResult < 0x80) 
      {
         iDesNumOfByte = 1;
         uiMask = 0;
      }
      else if (uiResult < 0x800) 
      {
         iDesNumOfByte = 2;
         uiMask = 0xc0;
      }
      else 
      {
         iDesNumOfByte = 3;
         uiMask = 0xe0;
      } 
      
      /* Only write to destination vector if pcDst != OP_NULL */
      if (pcDst != NULL) 
      {
         /* Is there room for this in the destination vector */ 
         if (iDindex + iDesNumOfByte > *plDstLen)
            break;
         
         switch (iDesNumOfByte) 
         {
         case 3:
            pcDst[iDindex + 2] = (WE_CHAR) ((uiResult & 0x3f) | 0x80);
            uiResult >>= 6;
            /* falltrough */
         case 2:
            pcDst[iDindex + 1] = (WE_CHAR) ((uiResult & 0x3f) | 0x80);
            uiResult >>= 6;
            /* falltrough */
         case 1:
            pcDst[iDindex] = (WE_CHAR) (uiResult | uiMask);
            break;
         default:
            break;
         }
      }
      
      iSindex += iSouNumOfByte;
      iDindex += iDesNumOfByte;
   }
   *plSrcLen = iSindex;
   *plDstLen = iDindex;
   
   return TRUE;
}

 WE_BOOL WE_CSC_Utf8ToUnicode(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_INT32     iSindex       = 0;
   WE_INT32     iDindex       = 0;
   WE_INT32     iSouNumOfByte = 0; 
   WE_INT32     iDesNumOfByte = 0;
   WE_UINT32    uiResult      = 0;
   WE_INT32     iShiftFirst   = 0;
   WE_INT32     iShiftSecond  = 0;

   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return E_WECSC_BAD_PRARAM;
   }
   
   iShiftFirst = 0;
   iShiftSecond = 1; 
   
   while (iSindex < *plSrcLen) 
   {
      /* Read one Unicode character in UTF-8 format: */
      iSouNumOfByte = aucUtf8BytePerChar[(WE_UCHAR)*pcSrc >> 4];
      if (iSindex + iSouNumOfByte > *plSrcLen)
         break;
      
      uiResult = 0;
      switch (iSouNumOfByte) 
      {
      case 0:
         return FALSE;
         
      case 4: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 3: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 2: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 1: uiResult += (WE_UCHAR)*pcSrc++;
      }
      uiResult -=aunUtf8Offset[iSouNumOfByte];
      
      /* Write one Unicode character: */
      uiResult &= 0xffff;
      
      if (pcDst != NULL) 
      {
         if (iDindex + 2> *plDstLen)
            break;
         
         pcDst[iDindex + iShiftFirst] = (WE_CHAR)((uiResult & 0xff));            
         pcDst[iDindex + iShiftSecond] = (WE_CHAR)((uiResult >> 8) & 0xff);
      }
      iDesNumOfByte = 2;
      iSindex += iSouNumOfByte;
      iDindex += iDesNumOfByte;
   }
   
   *plSrcLen = iSindex;
   *plDstLen = iDindex;
   
   return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
#ifdef   WE_CSC_CFG_SUPPORT_BIG5
static WE_BOOL WE_CSC_Big5ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_BOOL     bResult     = FALSE;
   WE_INT32    iUniLen     = 0;
   WE_UINT32   uiTempLen   = 0;
   WE_UCHAR    *pucBig5Str = NULL;
   WE_CHAR     *pcUniStr   = NULL;

   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
#ifdef WE_CSC_USED_SPREADTRUM
   if(NULL == pcDst)
   {
      *plDstLen = (*plSrcLen) * 3;
   }
   else
   {
      WE_UINT32  uiUcs2Len = 0;
      WE_UINT32  uiUtf8Len = 0;
      
      pcUniStr = (WE_CHAR*)WE_CSC_MALLOC((*plSrcLen)* 2 + 2 );
      if(!pcUniStr)
      {
         return FALSE;
      }
      
      uiUcs2Len = GUI_OtherCodeToWstr(pcUniStr, (*plSrcLen) * 2, GUI_CHAR_CODE_ANSI_BIG5, pcSrc, *plSrcLen);
      
      uiUtf8Len = GUI_WstrToUTF8(pcDst, *plDstLen, pcUniStr, uiUcs2Len);
      
      WE_CSC_FREEIF(pcUniStr);
      
      *plDstLen = uiUtf8Len;
   }
#else
   pucBig5Str = (WE_UCHAR*)WE_CSC_MALLOC((*plSrcLen) + 1 );
   if(NULL == pucBig5Str)
   {
      return FALSE;
   }
   WE_CSC_MEMSET(pucBig5Str, 0x00, (*plSrcLen) + 1 );
   
   pcUniStr = WE_CSC_MALLOC((*plSrcLen)* 2 + 2 );
   if(!pcUniStr)
   {
      WE_CSC_FREEIF(pucBig5Str);
      return FALSE;
   }
   
   /*copy  big5Str string */
   WE_CSC_MEMCPY(pucBig5Str,(WE_CHAR *)pcSrc,*plSrcLen);
   
   /* change big5 string to unicode string*/
   bResult = Big5ToUnicode((const WE_UCHAR *)pucBig5Str, (WE_UINT32 *)plSrcLen, (WE_UINT16*)pcUniStr, &uiTempLen);
   if(FALSE == bResult || 0 == uiTempLen)
   {
      WE_CSC_FREEIF(pucBig5Str);
      WE_CSC_FREEIF(pcUniStr);
      return FALSE;
   }
   iUniLen   = WE_CSC_WSTRLEN((WE_UINT16 *)pcUniStr) * sizeof(WE_UINT16);
   //  iUniLen = (*plSrcLen) * 2 ;
   
   /* change unicode string to utf8 string*/
   if( FALSE == WE_CSC_UnicodeToUtf8(pMe, pcUniStr,(WE_LONG*)&iUniLen,pcDst,(WE_LONG*)plDstLen))
   {
      WE_CSC_FREEIF(pucBig5Str);
      WE_CSC_FREEIF(pcUniStr);
      return FALSE;
   }
   WE_CSC_FREEIF(pucBig5Str);
   WE_CSC_FREEIF(pcUniStr);
#endif

   return TRUE;
}


static WE_BOOL WE_CSC_Utf8ToBig5(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_UINT32   uiUniLen    = 0x7FFFFFFF;
   WE_CHAR     *pcUniStr   = NULL;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return E_WECSC_BAD_PRARAM;
   }

#ifdef WE_CSC_USED_SPREADTRUM
//    if(NULL == pcDst)
//    {
//       *plDstLen = (*plSrcLen) * 2;
//    }
//    else
//    {
//       WE_UINT32  uiUcs2Len = 0;
//       WE_UINT32  uiBig5Len = 0;
//       
//       pcUniStr = (WE_CHAR*)WE_CSC_MALLOC((*plSrcLen)* 2 + 2 );
//       if(!pcUniStr)
//       {
//          return FALSE;
//       }
//       WE_CSC_MEMSET(pcUniStr, 0x00, (*plSrcLen)* 2 + 2);
//       
//       uiUcs2Len = GUI_UTF8ToWstr(pcUniStr, (*plSrcLen) * 2, pcSrc, *plSrcLen);
//       
//       uiBig5Len = GUI_WstrToBig5(pcDst, pcUniStr, uiUcs2Len);
//       
//       *plDstLen = uiBig5Len;
//    }

   return FALSE;

#else
   pcUniStr = (WE_CHAR*) WE_CSC_MALLOC((*plSrcLen)* 2 + 2 );
   if(!pcUniStr)
   {
      return FALSE;
   }
   WE_CSC_MEMSET(pcUniStr, 0x00, (*plSrcLen)* 2 + 2);
   
   if(FALSE == WE_CSC_Utf8ToUnicode(pMe, pcSrc, plSrcLen, pcUniStr, (WE_LONG*)&uiUniLen) )    
   {
      WE_CSC_FREEIF(pcUniStr);      
      return FALSE;
   }
   
   if(FALSE == UnicodeToBig5((const WE_UINT16*)pcUniStr, &uiUniLen, (WE_UCHAR *)pcDst, (WE_UINT32 *)plDstLen))
   {
      WE_CSC_FREEIF(pcUniStr);      
      return FALSE;
   }
   
   WE_CSC_FREEIF(pcUniStr);
#endif
   return TRUE;
}
#endif   // #ifdef   WE_CSC_CFG_SUPPORT_BIG5

///////////////////////////////////////////////////////////////////////////////
#ifdef   WE_CSC_CFG_SUPPORT_GB2312
static WE_BOOL WE_CSC_Gb2312ToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_INT32    iUniLen        = 0;
   
   WE_UCHAR    *pucGb2312Str  = NULL;
   WE_INT32    iGb2312StrSize = 0;
   WE_CHAR     *pucUniStr     = NULL;

   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
#ifdef WE_CSC_USED_SPREADTRUM
   if(NULL == pcDst)
   {
      *plDstLen = (*plSrcLen) * 3;
   }
   else
   {
      WE_UINT32  uiUcs2Len = 0;
      WE_UINT32  uiUtf8Len = 0;

      pucUniStr = (WE_CHAR*)WE_CSC_MALLOC((*plSrcLen)* 2 + 2 );
      if(!pucUniStr)
      {
         WE_CSC_FREEIF(pucGb2312Str);
         return FALSE;
      }

      uiUcs2Len = GUI_OtherCodeToWstr(pucUniStr, (*plSrcLen) * 2, GUI_CHAR_CODE_ANSI, pcSrc, *plSrcLen);

      uiUtf8Len = GUI_WstrToUTF8(pcDst, *plDstLen, pucUniStr, uiUcs2Len);

      WE_CSC_FREEIF(pucUniStr);

      *plDstLen = uiUtf8Len;
   }
#else
   iGb2312StrSize = (*plSrcLen) + 1;
   
   pucGb2312Str = (WE_UCHAR*)WE_CSC_MALLOC(iGb2312StrSize);
   if(!pucGb2312Str)
   {
      return FALSE;
   }
   WE_CSC_MEMSET(pucGb2312Str, 0x00, iGb2312StrSize);
   
   /*copy  gb2312Str string */
   WE_CSC_MEMCPY((WE_CHAR *)pucGb2312Str, (WE_CHAR *)pcSrc, *plSrcLen);
   
   pucUniStr = (WE_CHAR*)WE_CSC_MALLOC((*plSrcLen)* 2 + 2 );
   if(!pucUniStr)
   {
      WE_CSC_FREEIF(pucGb2312Str);
      return FALSE;
   }
   
   
   /* change gb2312Str string to unicode string*/
   if(TRUE != Gb2312ToUnicode(pucGb2312Str, (WE_INT32*)plSrcLen, (WE_UINT16 *)pucUniStr, &iUniLen))
   {
      WE_CSC_FREEIF(pucGb2312Str);
      WE_CSC_FREEIF(pucUniStr);
      return FALSE;
   }
   
   iUniLen <<= 1;    // Get bytes.
   
   /* change unicode string to utf8 string*/
   if(FALSE == WE_CSC_UnicodeToUtf8(pMe, pucUniStr,(WE_LONG*)&iUniLen,pcDst,plDstLen))
   {
      WE_CSC_FREEIF(pucGb2312Str);
      WE_CSC_FREEIF(pucUniStr);
      return FALSE;
   }
   
   WE_CSC_FREEIF(pucGb2312Str);
   WE_CSC_FREEIF(pucUniStr);
#endif

   return TRUE;
}
static WE_BOOL WE_CSC_Utf8ToGb2312(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_INT32 iUniLen     = 0x7fffffff;
   WE_CHAR *pucUniStr   = NULL;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
#ifdef WE_CSC_USED_SPREADTRUM
   if(NULL == pcDst)
   {
      *plDstLen = (*pcSrc) * 2;
   }
   else
   {
      WE_UINT32  uiUcs2Len = 0;
      WE_UINT32  uiBgLen = 0;

      pucUniStr = (WE_CHAR*)WE_CSC_MALLOC((*plSrcLen)* 2 + 2 );
      if(!pucUniStr)
      {
         return FALSE;
      }
      WE_CSC_MEMSET(pucUniStr, 0x00, (*plSrcLen)* 2 + 2);

      uiUcs2Len = GUI_UTF8ToWstr(pucUniStr, (*plSrcLen) * 2, pcSrc, *plSrcLen);

      uiBgLen = GUI_WstrToGB(pcDst, pucUniStr, uiUcs2Len);

      *plDstLen = uiBgLen;
   }
#else
   pucUniStr = (WE_CHAR*)WE_CSC_MALLOC((*plSrcLen)* 2 + 2 );
   if(!pucUniStr)
   {
      return FALSE;
   }
   WE_CSC_MEMSET(pucUniStr, 0x00, (*plSrcLen)* 2 + 2);
   
   if(FALSE == WE_CSC_Utf8ToUnicode(pMe, pcSrc,plSrcLen,pucUniStr,(WE_LONG*)&iUniLen))
   {
      WE_CSC_FREEIF(pucUniStr);
      return FALSE;      
   }
   
   if(FALSE == UnicodeToGb2312((WE_UINT16 *)pucUniStr,(WE_UINT8*)pcDst, (WE_INT32*)plDstLen))
   {
      WE_CSC_FREEIF(pucUniStr);
      return FALSE;        
   }
   
   WE_CSC_FREEIF(pucUniStr);
#endif

   return TRUE;
}
#endif   // #ifdef   WE_CSC_CFG_SUPPORT_GB2312

///////////////////////////////////////////////////////////////////////////////
#ifdef   WE_CSC_CFG_SUPPORT_GBK
static WE_BOOL WE_CSC_GBK2UTF8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
   WE_CHAR     *pcGBK         = NULL;
   WE_CHAR     *pcUnicode     = NULL;
   WE_UINT32   unTempLen      = 0;
   WE_UINT32   unUnicodeLen   = 0;
   
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
#ifdef WE_CSC_USED_SPREADTRUM
   if(NULL == pcDst)
   {
      *plDstLen = (*plSrcLen) * 3;
   }
   else
   {
      WE_UINT32  uiUcs2Len = 0;
      WE_UINT32  uiUtf8Len = 0;
      
      pcUnicode = (WE_CHAR*)WE_CSC_MALLOC((*plSrcLen) * 2 + 2 );
      if(!pcUnicode)
      {
         return FALSE;
      }
      
      uiUcs2Len = GUI_OtherCodeToWstr(pcUnicode, (*plSrcLen) * 2, GUI_CHAR_CODE_ANSI, pcSrc, *plSrcLen);
      
      uiUtf8Len = GUI_WstrToUTF8(pcDst, *plDstLen, pcUnicode, uiUcs2Len);
      
      WE_CSC_FREEIF(pcUnicode);
      
      *plDstLen = uiUtf8Len;
   }
#else
   pcGBK = (WE_CHAR*)WE_CSC_MALLOC((*plSrcLen) + 1);
   if (NULL == pcGBK) 
   {
      return FALSE;
   }
   WE_CSC_MEMSET(pcGBK, 0x00, (*plSrcLen) + 1);
   WE_CSC_MEMCPY(pcGBK, pcSrc, *plSrcLen);
   
   pcUnicode = (WE_CHAR*)WE_CSC_MALLOC((*plSrcLen) * 2 + 2);
   if (NULL == pcUnicode) 
   {
      WE_CSC_FREEIF(pcGBK);
      pcGBK = NULL;
      
      return FALSE;
   }
   WE_CSC_MEMSET(pcUnicode, 0x00, (*plSrcLen) * 2 + 2);
   
//   unTempLen = GBKToUnicode((WE_UINT16*)pcUnicode, (WE_UCHAR*)pcGBK, *plSrcLen);
//   if (unTempLen == 0) 
//   {
//      WE_CSC_FREEIF(pcGBK);
//      WE_CSC_FREEIF(pcUnicode);
//      
//      return FALSE;
//   }
   /************************************************************************/
   /*[Rachel 2007/11/7 ] Update the GBKToUnicode                           */
   /************************************************************************/
   if ((TRUE != GBKToUnicode((WE_UINT16*)pcUnicode,(WE_UCHAR*)pcGBK, (WE_INT32*)plSrcLen, (WE_INT32*)&unTempLen)) 
      || 0 == unTempLen)
   {
      WE_CSC_FREEIF(pcGBK);
      WE_CSC_FREEIF(pcUnicode); 
      
      return FALSE;
      
   }
   unUnicodeLen = unTempLen * 2;
   
   if ((FALSE == WE_CSC_UnicodeToUtf8(pMe, (const WE_CHAR*)pcUnicode, (WE_LONG *)&unUnicodeLen, pcDst, (WE_LONG*)plDstLen) ) )
   {
      WE_CSC_FREEIF(pcGBK);
      WE_CSC_FREEIF(pcUnicode);
      
      return FALSE;
   }
   
   WE_CSC_FREEIF(pcGBK);
   WE_CSC_FREEIF(pcUnicode);
#endif

   return TRUE;
}
#endif   // #ifdef   WE_CSC_CFG_SUPPORT_GBK

///////////////////////////////////////////////////////////////////////////////
//modify by xianhe.yang  gb18030 using GBK decoder
static WE_BOOL WE_CSC_GB18030ToUTF8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
#ifdef WE_CSC_USED_SPREADTRUM
    
#ifdef   WE_CSC_CFG_SUPPORT_GBK
    return WE_CSC_GBK2UTF8(pMe, pcSrc, plSrcLen, pcDst, plDstLen);
#endif
    
#else
#ifdef   WE_CSC_CFG_SUPPORT_GB18030
   if (NULL == pMe || NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return E_WECSC_BAD_PRARAM;
   }

   return GB18030ToUFT8((const unsigned char *)pcSrc, (int *)plSrcLen, (unsigned char *)pcDst, (int *)plDstLen);
#endif   // #ifdef   WE_CSC_CFG_SUPPORT_GB18030
#endif   // #ifdef   WE_CSC_USED_SPREADTRUM
}


static WE_BOOL WE_CSC_KOI8RToUTF8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
#ifdef WE_CSC_USED_SPREADTRUM
   WE_CHAR     *pucUniStr     = NULL;
   if(NULL == pcDst)
   {
      *plDstLen = (*plSrcLen) * 3;
   }
   else
   {
      WE_UINT32  uiUcs2Len = 0;
      WE_UINT32  uiUtf8Len = 0;

      pucUniStr = (WE_CHAR*)WE_CSC_MALLOC((*plSrcLen)* 2 + 2 );
      if(!pucUniStr)
      {
         return FALSE;
      }

      uiUcs2Len = GUI_OtherCodeToWstr(pucUniStr, (*plSrcLen) * 2, GUI_CHAR_CODE_KOI8R, pcSrc, *plSrcLen);

      uiUtf8Len = GUI_WstrToUTF8(pcDst, *plDstLen, pucUniStr, uiUcs2Len);

      WE_CSC_FREEIF(pucUniStr);

      *plDstLen = uiUtf8Len;
   }
   return TRUE;
#else
   return FALSE;
#endif
}

static WE_BOOL WE_CSC_Windows1251ToUTF8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen)
{
#ifdef WE_CSC_USED_SPREADTRUM
   WE_CHAR     *pucUniStr     = NULL;
   if(NULL == pcDst)
   {
      *plDstLen = (*plSrcLen) * 3;
   }
   else
   {
      WE_UINT32  uiUcs2Len = 0;
      WE_UINT32  uiUtf8Len = 0;

      pucUniStr = (WE_CHAR*)WE_CSC_MALLOC((*plSrcLen)* 2 + 2 );
      if(!pucUniStr)
      {
         return FALSE;
      }

      uiUcs2Len = GUI_OtherCodeToWstr(pucUniStr, (*plSrcLen) * 2, GUI_CHAR_CODE_WINDOW1251, pcSrc, *plSrcLen);

      uiUtf8Len = GUI_WstrToUTF8(pcDst, *plDstLen, pucUniStr, uiUcs2Len);

      WE_CSC_FREEIF(pucUniStr);

      *plDstLen = uiUtf8Len;
   }
   return TRUE;
#else
   return FALSE;
#endif
}
///////////////////////////////////////////////////////////////////////////////

/*=========================   File End  =====================================*/
