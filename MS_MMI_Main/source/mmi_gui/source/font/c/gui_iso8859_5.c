//#if defined(IM_RUSSIAN_SUPPORT) || defined(MULTI_VARIANT_SUPPORT)
/*****************************************************************************
** File Name:      gui_iso8859_5.c                                              *
** Author:                                                                   *
** Date:           18/02/2012                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe conversion function         *
**                 from iso8859_5 to unicode                                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 18/02/2012    dafeng.xu         Create                                  *
******************************************************************************/

#ifndef _GUI_ISO8859_5_C_
#define _GUI_ISO8859_5_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "caf.h"
#include "sci_codec.h"
#include "gui_ucs2b_converter.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*
 * ISO-8859-5
 */

LOCAL const uint16 s_iso8859_5_2uni[96] = {
  /* 0xa0 */
  0x00a0, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407,
  0x0408, 0x0409, 0x040a, 0x040b, 0x040c, 0x00ad, 0x040e, 0x040f,
  /* 0xb0 */
  0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,
  0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, 0x041f,
  /* 0xc0 */
  0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,
  0x0428, 0x0429, 0x042a, 0x042b, 0x042c, 0x042d, 0x042e, 0x042f,
  /* 0xd0 */
  0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,
  0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e, 0x043f,
  /* 0xe0 */
  0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,
  0x0448, 0x0449, 0x044a, 0x044b, 0x044c, 0x044d, 0x044e, 0x044f,
  /* 0xf0 */
  0x2116, 0x0451, 0x0452, 0x0453, 0x0454, 0x0455, 0x0456, 0x0457,
  0x0458, 0x0459, 0x045a, 0x045b, 0x045c, 0x00a7, 0x045e, 0x045f,
};

LOCAL const uint8 s_iso8859_5_page00[16] = {
  0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, /* 0xa0-0xa7 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0xad, 0x00, 0x00, /* 0xa8-0xaf */
};
LOCAL const uint8 s_iso8859_5_page04[96] = {
  0x00, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, /* 0x00-0x07 */
  0xa8, 0xa9, 0xaa, 0xab, 0xac, 0x00, 0xae, 0xaf, /* 0x08-0x0f */
  0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, /* 0x10-0x17 */
  0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, /* 0x18-0x1f */
  0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, /* 0x20-0x27 */
  0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, /* 0x28-0x2f */
  0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, /* 0x30-0x37 */
  0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, /* 0x38-0x3f */
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, /* 0x40-0x47 */
  0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, /* 0x48-0x4f */
  0x00, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, /* 0x50-0x57 */
  0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0x00, 0xfe, 0xff, /* 0x58-0x5f */
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : iso8859_5 to wchar
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN Iso8859_5ToWchar(
                           uint8 ch,
                           wchar* wch_ptr
                           )
{
    SCI_ASSERT( PNULL != wch_ptr ); /*assert verified*/

    if (ch < 0xa0) {
        *wch_ptr = ch;
    }
    else {
        *wch_ptr = s_iso8859_5_2uni[ch - 0xa0];
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : wchar to iso8859_5 
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN WcharToIso8859_5(
                            uint8 *ch_ptr, //out
                            wchar w_char
                            )
{
    uint8 c = 0;
    if (w_char < 0x00a0) {
        *ch_ptr = w_char;
        return TRUE;
    }
	else if (w_char >= 0x00a0 && w_char < 0x00b0)
		c = s_iso8859_5_page00[w_char-0x00a0];
	else if (w_char >= 0x0400 && w_char < 0x0460)
		c = s_iso8859_5_page04[w_char-0x0400];
	else if (w_char == 0x2116)
		c = 0xf0;
	if (c != 0) {
		*ch_ptr = c;
		return TRUE;
	}
     return FALSE;
}

/*****************************************************************************/
// 	Description : iso8859_5 to wstr
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_Iso8859_5ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *iso8859_5_ptr,    //source: string pointer to iso8859_5 string
                               uint32      str_len         //the length to convert from iso8859_5 to UCS2.
                               )
{
    uint32 i = 0;
    uint32 len = str_len;
    
    if ( PNULL == iso8859_5_ptr || PNULL == wstr_ptr || 0 == str_len )
    {
        return 0;
    }

    for ( i = 0; i < str_len; i++ )
    {
        if ( !Iso8859_5ToWchar( iso8859_5_ptr[i], &wstr_ptr[i]) )
        {
            len = i;
            break;
        }
    }

    return len;
}

/*****************************************************************************/
// 	Description : wstr to iso8859_5 
//	Global resource dependence : none
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToIso8859_5(//return the number of UCS2 code that have been converted.
                               uint8 *iso8859_5_ptr,        //destination: string pointer to iso8859_5 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to iso8859_5.
                               )
{
    uint32 i = 0;
    uint32 len = wstr_len;
    
    if ( PNULL == iso8859_5_ptr || PNULL == wstr_ptr || 0 == wstr_len )
    {
        return 0;
    }
   
    for ( i = 0; i < wstr_len; i++ )
    {
        if ( !WcharToIso8859_5( &iso8859_5_ptr[i], wstr_ptr[i] ) )
        {
            len = i;
            break;
        }
    }
    
    return len;
}

PUBLIC uint32 GUI_Iso8859_5ToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *iso8859_5_ptr,  //source: string pointer to iso8859_5 string
									  uint32 str_len,        //the length to convert from iso8859_5 to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  )
{
    uint32 ret = 0;
    if (PNULL == wstr_ptr || PNULL == iso8859_5_ptr || 0 == str_len) {
        return 0;
    }

    ret = GUI_Iso8859_5ToWstr(wstr_ptr, iso8859_5_ptr, str_len);
    if (NULL != src_end_pos) {
        *src_end_pos = (uint32)iso8859_5_ptr + ret;
    }
    return ret;
}


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif
//#endif



