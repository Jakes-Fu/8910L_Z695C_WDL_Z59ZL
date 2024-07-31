/*=====================================================================================
FILE NAME   : eme_charset.h 
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
      This File is the common header file of eme_charset_big5ucs2.c and eme_charset_gbkucs2.c
      These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
        20070308    wxs        01                create
                
                 
=====================================================================================*/

#ifndef _EME_CHARSET_H_
#define _EME_CHARSET_H_

#include "we_def.h"



/* convert gb2312 to utf16 */
WE_BOOL EME_GbToUcs2(const WE_UINT8* strIn, WE_UINT16* strOut);
	
/* convert big5 to utf16 */
WE_BOOL EME_Big5ToUcs2(const WE_CHAR* strIn, WE_UINT16* strOut);

	



#endif