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

#ifndef _GUI_ISO8859_2_C_
#define _GUI_ISO8859_2_C_

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

LOCAL const uint16 s_iso8859_2_2uni[96] = {
  /* 0xa0 */
  0x00a0, 0x0104, 0x02d8, 0x0141, 0x00a4, 0x013d, 0x015a, 0x00a7,	
  0x00a8, 0x0160, 0x015e, 0x0164, 0x0179, 0x00ad, 0x017d, 0x017b,	
  /* 0xb0 */
  0x00b0, 0x0105, 0x02db, 0x0142, 0x00b4, 0x013e, 0x015b, 0x02c7,	
  0x00b8, 0x0161, 0x015f, 0x0165, 0x017a, 0x02dd, 0x017e, 0x017c,	
  /* 0xc0 */
  0x0154, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x0139, 0x0106, 0x00c7,	
  0x010c, 0x00c9, 0x0118, 0x00cb, 0x011a, 0x00cd, 0x00Ce, 0x010e,	
  /* 0xd0 */
  0x0110, 0x0143, 0x0147, 0x00d3, 0x00d4, 0x0150, 0x00d6, 0x00d7,	
  0x0158, 0x016e, 0x00Da, 0x0170, 0x00dC, 0x00dd, 0x0162, 0x00df,	
  /* 0xe0 */
  0x0155, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x013a, 0x0107, 0x00e7,	
  0x010d, 0x00e9, 0x0119, 0x00eb, 0x011b, 0x00ed, 0x00ee, 0x010f,	
  /* 0xf0 */
  0x0111, 0x0144, 0x0148, 0x00f3, 0x00f4, 0x0151, 0x00f6, 0x00f7,	
  0x0159, 0x016f, 0x00fa, 0x0171, 0x00fc, 0x00fd, 0x0163, 0x02d9,
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
LOCAL BOOLEAN Iso8859_2ToWchar(
                           uint8 ch,
                           wchar* wch_ptr
                           )
{
    if ( PNULL == wch_ptr )
    {
        return FALSE;
    }

    if (ch < 0xa0) 
    {
        *wch_ptr = ch;
    }
    else
    {
        *wch_ptr = s_iso8859_2_2uni[ch - 0xa0];
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : wchar to iso8859_5 
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN WcharToIso8859_2(
                            uint8 *ch_ptr, //out
                            wchar w_char
                            )
{
    uint8 ch = 0;
    uint8 i = 0;

    if ( PNULL == ch_ptr )
    {
        return FALSE;
    }
	
    for(i = 0; i < 96; i++)
    {
 //       if(s_iso8859_2_2uni[i - 0xa0] == w_char)
    	if(s_iso8859_2_2uni[i] == w_char)
        {
            ch = 0xa0 + i;

            *ch_ptr = ch;
            return TRUE;
        }
    }
	
    *ch_ptr = w_char;
    return TRUE;
}

/*****************************************************************************/
// 	Description : iso8859_5 to wstr
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_Iso8859_2ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *iso8859_2_ptr,    //source: string pointer to iso8859_5 string
                               uint32      str_len         //the length to convert from iso8859_5 to UCS2.
                               )
{
    uint32 i = 0;
    uint32 len = str_len;
    
    if ( PNULL == iso8859_2_ptr || PNULL == wstr_ptr || 0 == str_len )
    {
        return 0;
    }

    for ( i = 0; i < str_len; i++ )
    {
        if ( !Iso8859_2ToWchar( iso8859_2_ptr[i], &wstr_ptr[i]) )
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
PUBLIC uint32 GUI_WstrToIso8859_2(//return the number of UCS2 code that have been converted.
                               uint8 *iso8859_2_ptr,        //destination: string pointer to iso8859_5 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to iso8859_5.
                               )
{
    uint32 i = 0;
    uint32 len = wstr_len;
    
    if ( PNULL == iso8859_2_ptr || PNULL == wstr_ptr || 0 == wstr_len )
    {
        return 0;
    }
   
    for ( i = 0; i < wstr_len; i++ )
    {
        if ( !WcharToIso8859_2( &iso8859_2_ptr[i], wstr_ptr[i] ) )
        {
            len = i;
            break;
        }
    }
    
    return len;
}

PUBLIC uint32 GUI_Iso8859_2ToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *iso8859_2_ptr,  //source: string pointer to iso8859_5 string
									  uint32 str_len,        //the length to convert from iso8859_5 to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  )
{
    uint32 ret = 0;
    if (PNULL == wstr_ptr || PNULL == iso8859_2_ptr || 0 == str_len) {
        return 0;
    }

    ret = GUI_Iso8859_2ToWstr(wstr_ptr, iso8859_2_ptr, str_len);
    if (NULL != src_end_pos) {
        *src_end_pos = (uint32)iso8859_2_ptr + ret;
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



