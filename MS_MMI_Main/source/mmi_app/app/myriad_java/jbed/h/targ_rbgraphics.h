/*[
********************************************************************************

    Project:            Spreadtrum

    Version:            $Id$

    Copyright 2001-2011 Myriad Group AG. All Rights Reserved.

********************************************************************************
]*/


#ifndef TARG_RBGRAPHICS_H_INCLUDED
#define TARG_RBGRAPHICS_H_INCLUDED

/**
 * Font information structure.
 *
 * Most of this filled in by CPL_font_createFont().
 * The Java visible height of the font = leading + ascender + descender.
 * The height of the pixel data request will be = ascender + descender.
 */

/* For Windows XP, choose between 16 bpp (555) here. */
#ifdef USE_256_COLORS
#undef USE_256_COLORS
#endif
#define USE_16BIT_COLORS
#ifdef USE_555_16BIT_FORMAT
#undef USE_555_16BIT_FORMAT
#endif

#define BMP_IN_HEAP

/* use a (shared) version of pixel colour lookup */
#define STD_LCD_PIXEL_CONVERT
//#define GRAPHICS_MAX_REFRESH_RATE

typedef uint16_t * GUI_TargDIB;
typedef int32_t    GUI_TargFont;

#define GUI_BAD_TARG_FONT      NULL

#ifdef PROD
#define CHECK_RETURN(expr)     do { (expr); } while (0)
#else
#define CHECK_RETURN(expr)                                   \
    do {                                                     \
        if ((expr) == 0) {                                   \
            CRTL_printf("%s returned error (%s:%d)\n",       \
                #expr, __FILE__, __LINE__);                  \
            checkRetFail();                                  \
        }                                                    \
    } while(0)

extern void checkRetFail(void);

#endif

#endif /* TARG_RBGRAPHICS_H_INCLUDED */
