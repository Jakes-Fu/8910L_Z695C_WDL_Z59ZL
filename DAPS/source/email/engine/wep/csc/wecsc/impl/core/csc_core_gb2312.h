// csc_core_gb2312.h
/*=====================================================================================
  FILE NAME :  csc_core_gb2312.h
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

#ifndef __CSC_CORE_GB2312_H__
#define __CSC_CORE_GB2312_H__

#include "csc_def.h"

WE_BOOL  UnicodeToGb2312(const WE_UINT16 *pusSrc, WE_UINT8 *pucDes, WE_INT32 *plDstLen);
WE_BOOL  Gb2312ToUnicode(const WE_UINT8  *pucSrc, WE_INT32 *piSrcLen, WE_UINT16 *pusDes, WE_INT32  *piDstLen);

#endif   // #ifndef __CSC_CORE_GB2312_H__
