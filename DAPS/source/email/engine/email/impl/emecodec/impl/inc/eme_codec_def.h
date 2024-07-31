/*=====================================================================================
FILE NAME   : eme_codec_def.h
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
    This File is the common header file for all codec module.
    These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
         20070305       wxs                         create
        
                 
=====================================================================================*/
/*==================================================================================================
* Multi-Include-Prevent Section
==================================================================================================*/
#ifndef _EME_CODEC_DEF_H_
#define _EME_CODEC_DEF_H_

#include "we_def.h"
#include "eme_constant_api.h"
#include "eme_codec_api.h"

/*=======================================================================*/
/*         macro     define                                            */
/**======================================================================*/
#define FLAG_D    0x0D
#define FLAG_A    0x0A
#define FLAG_END  0x00

#define EME_MSG_MAX_HEADER_NUM    10
#define EME_MSG_MAX_BODYPART_NUM  9

/* one way is buffer ,other is in file*/
#define EME_DATA_STORAGE_TYPE_NUM 2 

/* following define just using in Email engine */ 
#define EME_MINSTRLEN      50
#define EME_MAXSTRLEN      125
#define EME_TEXTTYPE_NUM   2

#define EME_UTIS_ABS(v) ((v) > 0 ? (v) : -(v))


#define EME_CHARSET_UTF8                  "utf-8"
#define EME_CHARSET_UTF16                 "utf-16"
#define EME_CHARSET_US_ASCII              "us-ascii"
#define EME_CHARSET_ISO_8859_1            "iso-8859-1"
#define EME_CHARSET_GB2312                "gb2312"
#define EME_CHARSET_BIG5                  "big5"
#define EME_CHARSET_HZ_GB_2312            "hz-gb-2312"
#define EME_CHARSET_GB18030               "gb18030"
#define EME_CHARSET_GBK                   "gbk"
#define EME_CHARSET_KOI8R                 "koi8-r"
#define EME_CHARSET_WIN1252               "windows-1251"

#define EME_TRANS_ENC_BASE64              "base64"
#define EME_TRANS_ENC_QUOTED_PRINTABLE    "quoted-printable"
#define EME_TRANS_ENC_7BIT                "7bit"
#define EME_TRANS_ENC_8BIT                "8bit"
#define EME_TRANS_ENC_UUENCODE            "uuencode"
#define EME_TRANS_ENC_BINHEX              "binhex"


#define EME_CHARSET_UTF8_G                  "UTF-8"
#define EME_CHARSET_UTF16_G                 "UTF-16"
#define EME_CHARSET_US_ASCII_G              "US-ASCII"
#define EME_CHARSET_ISO_8859_1_G            "ISO-8859-1"
#define EME_CHARSET_GB2312_G                "GB2312"
#define EME_CHARSET_BIG5_G                  "BIG5"
#define EME_CHARSET_HZ_GB_2312_G            "HZ-GB-2312"
#define EME_CHARSET_GB18030_G               "GB18030"
#define EME_CHARSET_GBK_G                   "GBK"
#define EME_CHARSET_KOI8R_G                 "KOI8-R"
#define EME_CHARSET_WIN1252_G               "WINDOWS-1251"

#define EME_TRANS_ENC_BASE64_G              "BASE64"
#define EME_TRANS_ENC_QUOTED_PRINTABLE_G    "QUOTED-PRINTABLE"
#define EME_TRANS_ENC_7BIT_G                "7BIT"
#define EME_TRANS_ENC_8BIT_G                "8BIT"
#define EME_TRANS_ENC_UUENCODE_G            "UUENCODE"
#define EME_TRANS_ENC_BINHEX_G              "BINHEX"


typedef enum _E_EncodeType
{
   E_ENCODETYPE_UNSET = 0,
   E_ENCODETYPE_7BIT,
   E_ENCODETYPE_8BIT,
   E_ENCODETYPE_BASE64,
   E_ENCODETYPE_QUOTEDPRINTABLE,
   E_ENCODETYPE_UUENCODE,
   E_ENCODETYPE_BINHEX,

   E_ENCODETYPE_UNKNOW,
   E_ENCODETYPE_MAX_FOURBYTES = 0x7FFFFFFF
      
}E_EncodeType;

typedef enum _E_Charset
{
   E_CHARSET_US_ASCII,
   E_CHARSET_ISO_8859_1,
   E_CHARSET_UTF8,
   E_CHARSET_UTF16,
   E_CHARSET_GB2312,
   E_CHARSET_BIG5,
   E_CHARSET_HZ_GB_2312,
   E_CHARSET_GB18030,
   E_CHARSET_GBK,
   E_CHARSET_KOI8R,
   E_CHARSET_WIN1251,

   E_CHARSET_UNKNOW,
   E_CHARSET_MAX_FOURBYTES = 0x7FFFFFFF
      
}E_Charset;

/*==================================================================================================
* Type Define Section
==================================================================================================*/  
typedef struct _St_MsgPartList
{
   EmailOpt *pstPartOpts;
   struct _St_MsgPartList *pstNext;
}St_MsgPartList;

typedef struct
{
   void     *pvEmailMsg;         /* Email message */
   void     *pvMemMgr;
   void     *pIFileMgr;
   void     *pICSC;
   St_MsgPartList *pstPartList;  /* message part opts list */  
   EmailOpt *pstHeaderOpts;      /* message header opts */
   EmailOpt *pstEncodedOpts;     /* encoding opts */ 
   WE_UINT32  uiPartsNum;        /* part number */

   /*used for file operate*/
   PFNENCODINGCB pfnEncoding;
   PFNDECODINGCB pfnDecoding;
   void          *pvUserData;
   void          *pICBMgr;

   PFNNOTIFY     pfnRegister;
   void          *pvRegisterData;
}stEMSGHandle;

///*add new from  em_time.h*/
//typedef struct
//{
//   WE_UINT16 wYear;
//   WE_UINT16 wMonth;
//   WE_UINT16 wDay;
//   WE_UINT16 wHour;
//   WE_UINT16 wMinute;
//   WE_UINT16 wSecond;
//   WE_UINT16 wWeekDay;
//}St_JulianDateType;



#endif