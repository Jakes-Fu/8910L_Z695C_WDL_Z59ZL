// we_csc_gbk.h

/*=====================================================================================

  FILE NAME : we_csc_gbk.c
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
   2007-05-18     George Yang                Init

   Self-documenting Code
   Describe/explain low-level design, especially things in header files, of this module and/or group 
   of functions and/or specific function that are hard to understand by reading code and thus requires 
   detail description.
   Free format !

=====================================================================================*/



#ifndef __WE_CSC_GBK_H__
#define __WE_CSC_GBK_H__

#include "csc_def.h"

WE_BOOL GBKToUnicode(WE_UINT16 *pdwTo, WE_UCHAR *pucFrom, WE_INT32 *piSrcLen, WE_INT32 *piDesLen);
WE_INT32 UnicodeToGBK(WE_CHAR *to, WE_UINT16 *from, WE_INT32 words);

#endif   // #ifndef __WE_CSC_GBK_H__




















