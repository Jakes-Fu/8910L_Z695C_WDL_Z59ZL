// WE_CSC_def.h
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
#ifndef __WE_CSC_DEF_H__
#define __WE_CSC_DEF_H__

#include "we_def.h"
#include "we_log.h"

#ifndef WE_CSC_USED_SPREADTRUM
#define WE_CSC_USED_SPREADTRUM
#endif

#ifdef WE_CSC_USED_SPREADTRUM
#include "gui_ucs2b_converter.h"
#endif

/* Define Configure           */
#define  WE_CSC_CFG_LOG
//#define  WE_CSC_CFG_BREW
//#define  WE_CSC_CFG_VS2005
#define  WE_CSC_CFG_PHILIPS


//#define  WE_CSC_CFG_SUPPORT_GB18030
#define  WE_CSC_CFG_SUPPORT_GBK
#define  WE_CSC_CFG_SUPPORT_BIG5
#define  WE_CSC_CFG_SUPPORT_GB2312




/* Define Log Function        */
#ifdef   WE_CSC_CFG_LOG
   #define  WE_CSC_LOG_ERR                   We_LogInfo_High
   #define  WE_CSC_LOG_INFO                  We_LogInfo_Low
#endif   // #ifdef   WE_CSC_LOG

#ifndef MAX
   #define  MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#endif   // #ifndef MAX

#ifndef MIN
   #define  MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#endif   // #ifndef MIN

/* Define Functions           */
#ifdef  WE_CSC_CFG_BREW
#include "AEEStdLib.h"
   #define  WE_CSC_STRDUP                    STRDUP
   #define  WE_CSC_WSTRDUP                   WSTRDUP

   #define  WE_CSC_MALLOC                    MALLOC
   #define  WE_CSC_FREEIF                    FREEIF
   #define  WE_CSC_FREE                      FREE

   #define  WE_CSC_WSTRLEN                   WSTRLEN
   #define  WE_CSC_STRLEN                    STRLEN
   #define  WE_CSC_MEMSET                    MEMSET
   #define  WE_CSC_MEMCPY                    MEMCPY
#elif   defined  WE_CSC_CFG_VS2005
#include "malloc.h"
#include "string.h"

    #ifdef  MALLOC
    #undef  MALLOC
    #endif

    #ifdef  FREE
    #undef  FREE
    #endif

    #ifdef  FREEIF
    #undef  FREEIF
    #endif

    #ifdef  MEMSET
    #undef  MEMSET
    #endif

    #ifdef  MEMCPY
    #undef  MEMCPY
    #endif

    #ifdef  STRDUP
    #undef  STRDUP
    #endif

    #ifdef  WSTRDUP
    #undef  WSTRDUP
    #endif

    #ifdef  STRLEN
    #undef  STRLEN
    #endif

    #ifdef  WSTRLEN
    #undef  WSTRLEN
    #endif

   #define  WE_CSC_STRDUP                    strdup
   #define  WE_CSC_WSTRDUP                   wstrdup

   #define  WE_CSC_MALLOC                    malloc
   #define  WE_CSC_FREEIF(x)                 ((x) ? (free(x), (x) == NULL) : (x) == NULL)
   #define  WE_CSC_FREE                      free

   #define  WE_CSC_WSTRLEN                   wcslen
   #define  WE_CSC_STRLEN                    strlen
   #define  WE_CSC_MEMSET                    memset
   #define  WE_CSC_MEMCPY                    memmove
#elif    defined  WE_CSC_CFG_PHILIPS
#include "we_scl.h"
   #define  WE_CSC_STRDUP                    WE_SCL_STRDUP
   #define  WE_CSC_WSTRDUP                   WE_SCL_WSTRDUP

   #define  WE_CSC_MALLOC                    WE_SCL_MALLOC
   #define  WE_CSC_FREEIF                    WE_SCL_FREEIF
   #define  WE_CSC_FREE                      WE_SCL_FREE

   #define  WE_CSC_WSTRLEN                   WE_SCL_WSTRLEN
   #define  WE_CSC_STRLEN                    WE_SCL_STRLEN
   #define  WE_CSC_MEMSET                    WE_SCL_MEMSET
   #define  WE_CSC_MEMCPY                    WE_SCL_MEMCPY
#endif  

#endif   // #ifndef __WE_CSC_DEF_H__

