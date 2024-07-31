#if defined(IM_RUSSIAN_SUPPORT) || defined(MULTI_VARIANT_SUPPORT)
/*****************************************************************************
** File Name:      gui_cp866.c                                              *
** Author:                                                                   *
** Date:           18/02/2012                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe conversion function         *
**                 from CP1251 to unicode                                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 18/02/2012    dafeng.xu         Create                                  *
******************************************************************************/

#ifndef _GUI_CP866_C_
#define _GUI_CP866_C_

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
 * CP866
 */
LOCAL const uint16 s_cp866_2uni[80] = {
  /* 0xb0 */
  0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
  0x2555, 0x2563, 0x2551, 0x2557, 0x255d, 0x255c, 0x255b, 0x2510,
  /* 0xc0 */
  0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x255e, 0x255f,
  0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x2567,
  /* 0xd0 */
  0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256b,
  0x256a, 0x2518, 0x250c, 0x2588, 0x2584, 0x258c, 0x2590, 0x2580,
  /* 0xe0 */
  0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,
  0x0448, 0x0449, 0x044a, 0x044b, 0x044c, 0x044d, 0x044e, 0x044f,
  /* 0xf0 */
  0x0401, 0x0451, 0x0404, 0x0454, 0x0407, 0x0457, 0x040e, 0x045e,
  0x00b0, 0x2219, 0x00b7, 0x221a, 0x2116, 0x00a4, 0x25a0, 0x00a0,
};

LOCAL const uint8 s_cp866_page00[24] = {
  0xff, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x00, 0x00, /* 0xa0-0xa7 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xa8-0xaf */
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfa, /* 0xb0-0xb7 */
};

LOCAL const uint8 s_cp866_page04[96] = {
  0x00, 0xf0, 0x00, 0x00, 0xf2, 0x00, 0x00, 0xf4, /* 0x00-0x07 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf6, 0x00, /* 0x08-0x0f */
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, /* 0x10-0x17 */
  0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, /* 0x18-0x1f */
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, /* 0x20-0x27 */
  0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, /* 0x28-0x2f */
  0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, /* 0x30-0x37 */
  0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, /* 0x38-0x3f */
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, /* 0x40-0x47 */
  0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, /* 0x48-0x4f */
  0x00, 0xf1, 0x00, 0x00, 0xf3, 0x00, 0x00, 0xf5, /* 0x50-0x57 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7, 0x00, /* 0x58-0x5f */
};

LOCAL const uint8 s_cp866_page22[8] = {
  0x00, 0xf9, 0xfb, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x18-0x1f */
};

LOCAL const uint8 s_cp866_page25[168] = {
  0xc4, 0x00, 0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x00-0x07 */
  0x00, 0x00, 0x00, 0x00, 0xda, 0x00, 0x00, 0x00, /* 0x08-0x0f */
  0xbf, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, /* 0x10-0x17 */
  0xd9, 0x00, 0x00, 0x00, 0xc3, 0x00, 0x00, 0x00, /* 0x18-0x1f */
  0x00, 0x00, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00, /* 0x20-0x27 */
  0x00, 0x00, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, /* 0x28-0x2f */
  0x00, 0x00, 0x00, 0x00, 0xc1, 0x00, 0x00, 0x00, /* 0x30-0x37 */
  0x00, 0x00, 0x00, 0x00, 0xc5, 0x00, 0x00, 0x00, /* 0x38-0x3f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x40-0x47 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x48-0x4f */
  0xcd, 0xba, 0xd5, 0xd6, 0xc9, 0xb8, 0xb7, 0xbb, /* 0x50-0x57 */
  0xd4, 0xd3, 0xc8, 0xbe, 0xbd, 0xbc, 0xc6, 0xc7, /* 0x58-0x5f */
  0xcc, 0xb5, 0xb6, 0xb9, 0xd1, 0xd2, 0xcb, 0xcf, /* 0x60-0x67 */
  0xd0, 0xca, 0xd8, 0xd7, 0xce, 0x00, 0x00, 0x00, /* 0x68-0x6f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x70-0x77 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x78-0x7f */
  0xdf, 0x00, 0x00, 0x00, 0xdc, 0x00, 0x00, 0x00, /* 0x80-0x87 */
  0xdb, 0x00, 0x00, 0x00, 0xdd, 0x00, 0x00, 0x00, /* 0x88-0x8f */
  0xde, 0xb0, 0xb1, 0xb2, 0x00, 0x00, 0x00, 0x00, /* 0x90-0x97 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x98-0x9f */
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xa0-0xa7 */
};


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : cp866 to wchar
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CP866ToWchar(
                           uint8 ch,
                           wchar* wch_ptr
                           )
{
    SCI_ASSERT( PNULL != wch_ptr ); /*assert verified*/

    if (ch < 0x80) {
        *wch_ptr = ch;
    }
    else if (ch < 0xb0) {
        *wch_ptr = ch + 0x0390;
    }
    else {
        *wch_ptr = s_cp866_2uni[ch-0xb0];
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : wchar to cp866 
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN WcharToCP866(
                            uint8 *ch_ptr, //out
                            wchar w_char
                            )
{
    uint8 c = 0;
    if (w_char < 0x0080) {
        *ch_ptr = w_char;
        return TRUE;
    }
    else if (w_char >= 0x00a0 && w_char < 0x00b8)
        c = s_cp866_page00[w_char-0x00a0];
    else if (w_char >= 0x0400 && w_char < 0x0460)
        c = s_cp866_page04[w_char-0x0400];
    else if (w_char == 0x2116)
        c = 0xfc;
    else if (w_char >= 0x2218 && w_char < 0x2220)
        c = s_cp866_page22[w_char-0x2218];
    else if (w_char >= 0x2500 && w_char < 0x25a8)
        c = s_cp866_page25[w_char-0x2500];
    if (c != 0) {
        *ch_ptr = c;
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : cp866 to wstr
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_CP866ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *cp866_ptr,    //source: string pointer to cp855 string
                               uint32      str_len         //the length to convert from cp855 to UCS2.
                               )
{
    uint32 i = 0;
    uint32 len = str_len;
    
    if ( PNULL == cp866_ptr || PNULL == wstr_ptr || 0 == str_len )
    {
        return 0;
    }

    for ( i = 0; i < str_len; i++ )
    {
        if ( !CP866ToWchar( cp866_ptr[i], &wstr_ptr[i]) )
        {
            len = i;
            break;
        }
    }

    return len;
}

/*****************************************************************************/
// 	Description : wstr to cp866 
//	Global resource dependence : none
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToCP866(//return the number of UCS2 code that have been converted.
                               uint8 *cp866_ptr,        //destination: string pointer to cp855 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp855.
                               )
{
    uint32 i = 0;
    uint32 len = wstr_len;
    
    if ( PNULL == cp866_ptr || PNULL == wstr_ptr || 0 == wstr_len )
    {
        return 0;
    }
   
    for ( i = 0; i < wstr_len; i++ )
    {
        if ( !WcharToCP866( &cp866_ptr[i], wstr_ptr[i] ) )
        {
            len = i;
            break;
        }
    }
    
    return len;
}

PUBLIC uint32 GUI_CP866ToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *cp866_ptr,  //source: string pointer to CP866 string
									  uint32 str_len,        //the length to convert from CP866 to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  )
{
    uint32 ret = 0;
    if (PNULL == wstr_ptr || PNULL == cp866_ptr || 0 == str_len) {
        return 0;
    }

    ret = GUI_CP866ToWstr(wstr_ptr, cp866_ptr, str_len);
    if (NULL != src_end_pos) {
        *src_end_pos = (uint32)cp866_ptr + ret;
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
#endif


