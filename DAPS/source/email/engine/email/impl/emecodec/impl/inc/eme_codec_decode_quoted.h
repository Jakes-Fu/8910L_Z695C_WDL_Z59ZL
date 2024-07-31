/*=====================================================================================
FILE NAME   : eme_codec_decode_quoted.h
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
    This File is the header file of eme_codec_decode_quoted.c.
    These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
         20070306     wxs                         create
        
                 
=====================================================================================*/

/*==================================================================================================
* Multi-Include-Prevent Section
==================================================================================================*/
#ifndef _EME_CODEC_DECODE_QUOTED_H_
#define _EME_CODEC_DECODE_QUOTED_H_

/*---------------------------------------------------------------
 *       Include Files
 * ------------------------------------------------------------*/
/*Include System head file*/
#include "eme_utils_api.h"
#include "we_def.h"

/* =============function declaration =============== */
WE_INT32 EME_FromQuoted(WE_CHAR *out, WE_CHAR *in, WE_UINT32 uiInSize);
WE_UINT8 EME_HexToDec( WE_UINT8 C );

/*get decoded string size from original string*/
WE_UINT32 EME_QuotedDecodeSize(WE_UINT32 iSize);




#endif /* _EM_QUOTED_H */