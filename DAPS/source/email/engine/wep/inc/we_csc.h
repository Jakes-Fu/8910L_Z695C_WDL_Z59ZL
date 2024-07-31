// csc.h
/*=====================================================================================

  FILE NAME :  csc.h
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
#ifndef __CSC_H__
#define __CSC_H__
#include "we_def.h"



#ifdef __cplusplus
extern "C" 
{
#endif

typedef  void *     HCSC;


/*******************************************************************************
*   Macro Define Section
*******************************************************************************/


#define WE_CSC_UNKNOWN       0
#define WE_CSC_ASCII         3
#define WE_CSC_ISO_8859_1    4
#define WE_CSC_LATIN_1       4
#define WE_CSC_ISO_8859_2    5
#define WE_CSC_LATIN_2       5
#define WE_CSC_ISO_8859_8    11
#define WE_CSC_HEBREW        11
#define WE_CSC_UCS_2         1000
#define WE_CSC_UCS_2BE       10001
#define WE_CSC_UCS_2LE       10002
#define WE_CSC_UTF_16BE      1013
#define WE_CSC_UTF_16LE      1014
#define WE_CSC_UTF_16        1015
#define WE_CSC_UTF_8         106
#define WE_CSC_BIG5          2026  
#define WE_CSC_GB2312        2025  

#define WE_CSC_GBK           113
#define WE_CSC_GB18030       114
/*user define*/
#define WE_CSC_UNICODE       115  
#define WE_CSC_KOI8_R        116
#define WE_CSC_WINDOWS_1251  117

#define WE_CSC_UCS_4         1001


/*@@@@@@@@@@@@@@ add by tom */
#define  ICSC                void 
/*******************************************************************************
*   Type Define Section
*******************************************************************************/


/*Define function type start*/
typedef WE_BOOL Fn_We_Charset_Convert
(
   void *pMe,
   const WE_CHAR *pcSrc,   /* The source buffer. */
   WE_LONG *plSrcLen,      /* On input, the number of bytes
                            * available in the source buffer;
                            * on output, the number of source
                            * bytes consumed. */
   WE_CHAR *pcDst,         /* The destination buffer. */
   WE_LONG *plDstLen       /* On input, the amount of free space
                            * in the destination buffer;
                            * on output, the number of bytes
                            * placed in the output buffer. */
);

/*Define function type end*/

/*Define struct &  union type start*/
typedef struct _WeCscXchrEntry
{
   WE_INT32               iCharacterSet;
   const WE_CHAR          *pcPreferredName;
   const WE_CHAR          *pcAliases;
   Fn_We_Charset_Convert* fnCvtToUtf8;  
   Fn_We_Charset_Convert* fnCvtFromUtf8;  
} St_WeCscXchrEntry;
/*Define struct &  union type end*/




/*Define Enum type start*/
typedef enum _WECSCERR
{
   E_WECSC_SUCCESS      = 0,
   E_WECSC_FAILED,
   E_WECSC_BAD_PRARAM,
   E_WECSC_NO_ENOUGH_MEMERY,
   E_WECSC_NO_FILE
}E_WECSCERR;
/*Define Enum type end*/



/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/


E_WECSCERR  WE_CSC_Create(HCSC *phCSC);
void     WE_CSC_Delete(HCSC *phCSC);

Fn_We_Charset_Convert * WE_CSC_FindFunctionByVal(HCSC hCSC, WE_INT32 iFrom, WE_INT32 iTo);
Fn_We_Charset_Convert * WE_CSC_FindFunctionByROM(HCSC hCSC, WE_INT32 iFrom, WE_INT32 iTo, const WE_CHAR *pszStr, WE_INT32 *piStrLen);

WE_UINT32  WE_CSC_Char_Utf8ToUnicode(HCSC hCSC, const WE_CHAR *pszUtf8, WE_UINT32 *puiUnicode);
WE_UINT32  WE_CSC_Char_UnicodeToUtf8(HCSC hCSC, const WE_UINT32 uiUnicode, WE_CHAR *pszUtf8);

const char* WE_CSC_Int2Str(HCSC hCSC, WE_INT32 iCharset);
WE_INT32 WE_CSC_Str2Int(HCSC hCSC, const WE_CHAR *pszCharset, WE_INT32 iLen);

/*The function about the convert between unicode and utf8 in the mode of strings*/
WE_BOOL WE_CSC_Utf8ToUnicode(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);
WE_BOOL WE_CSC_UnicodeToUtf8(void *pMe, const WE_CHAR *pcSrc, WE_LONG *plSrcLen, WE_CHAR *pcDst, WE_LONG *plDstLen);

#define WE_FINDFUNCTIONOFCSC           WE_CSC_FindFunctionByVal
#define WE_FINDFUNCTIONOFCSCBYBOM      WE_CSC_FindFunctionByROM
#define WE_CSC_UTF8TOUNICODE           WE_CSC_Char_Utf8ToUnicode
#define WE_CSC_UNICODETOUTF8           WE_CSC_Char_UnicodeToUtf8
#define WE_CSC_INT2STR                 WE_CSC_Int2Str
#define WE_CSC_STR2INT                 WE_CSC_Str2Int 
    
/*@@@@@@@@@@@@@@ add by tom */
#define ICSC_FINDFUNCTIONOFCSC WE_FINDFUNCTIONOFCSC


#ifdef __cplusplus
}
#endif

#endif   // #ifndef __CSC_H__


