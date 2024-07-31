// csc_core_big5.h
/*=====================================================================================

  FILE NAME :  csc_core_big5.h
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

#ifndef __CSC_CORE_BIG5_H__
#define __CSC_CORE_BIG5_H__ 

#include "csc_def.h"

WE_BOOL  Big5ToUnicode(const WE_UCHAR *pucBig5, WE_UINT32 *puiBig5Byte, WE_UINT16 *pusUnicode, WE_UINT32 *puiUnicodeWord);
WE_BOOL  UnicodeToBig5(const WE_UINT16 *pusUnicode, WE_UINT32 *puiUnicodeWord, WE_UCHAR *pucBig5, WE_UINT32 *puiBig5Byte);

#endif   // #ifndef __CSC_CORE_BIG5_H__
