/*
 * Copyright 2001-2009 Myriad Group AG. All Rights Reserved.
 * $Id$
 */

/*
 * @(#)nativeGUI.h
 *
 * Copyright (c) 1999-2002 Sun Microsystems, Inc.  All rights reserved.
 * PROPRIETARY/CONFIDENTIAL
 * Use is subject to license terms.
 *
 */

#ifndef RBGRAPHICS_H_INCLUDED
#define RBGRAPHICS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <defaultlcdui.h>


#ifdef PRIMITIVES_DO_ALPHA
#define JBED_IS_FULL_OPAQUE(alpha) (alpha == 0xff)
#else
#define JBED_IS_FULL_OPAQUE(alpha) (TRUE)
#endif

/* there are two choices for the dithering done: simple ordered dithering
 * and an implementation of the floyd/steinberg dithering algorithm. The
 * default is to use the latter, when needed. The target include file below
 * can undefine either just USE_FS_DITHER to switch to ordered dither, or
 * both to disable dithering altogether.
 */
#define DITHERIMAGE
#define USE_FS_DITHER

#define USE_TRANSPARENCY
/* FAST_ALPHA_BLENDING loses 1 bit of alpha value precision for 565 mode
 * over the slower variant.
 */
#define FAST_ALPHA_BLENDING

#include <targ_rbgraphics.h>
#include <cpl_font.h>

/* no need to dither if we have sufficient colors */
#ifndef USE_256_COLORS
#undef DITHERIMAGE
#endif

/* Prevent an illegal combination of dither options. */
#if defined(USE_FS_DITHER) && !defined(DITHERIMAGE)
#undef USE_FS_DITHER
#endif

/* Color depth is set by one of the following defines. Exactly
 * one must be defined:
 * USE_256_COLORS   - 8 bits per pixel
 * USE_16BIT_COLORS - 16 bits per pixel
 * USE_24BIT_COLORS - 24 bits per pixel
 * USE_32BIT_COLORS - 32 bits per pixel (one byte unused)
 */

#if !defined(USE_256_COLORS) && !defined(USE_16BIT_COLORS) && \
    !defined(USE_24BIT_COLORS) && !defined(USE_32BIT_COLORS)
#error Colour depth must be defined
#endif

/* For 24 or 32 bit color, six addition defines determine the
 * locations of color channels within the pixel:
 *
 * GUI_24BPP_RED_BYTE, GUI_24BPP_GREEN_BYTE and GUI_24BPP_BLUE_BYTE
 * define the position of individual bytes in memory such that
 * given a pointer to the lowest addressed byte in the pixel they are
 * the offset to the byte for the required color channel.
 *
 * GUI_24BPP_RED_SHIFT, GUI_24BPP_GREEN_SHIFT and GUI_24BPP_BLUE_SHIFT
 * specify the bit positions of the color channels in a pixel in
 * a register. So that, pixel>>GUI_24BPP_BLUE_SHIFT puts the blue
 * color channel in the bottom 8 bits.
 */

#if defined(USE_24BIT_COLORS) || defined(USE_32BIT_COLORS)
#if !defined(GUI_24BPP_RED_BYTE) || !defined(GUI_24BPP_GREEN_BYTE) || \
    !defined(GUI_24BPP_BLUE_BYTE) || !defined(GUI_24BPP_RED_SHIFT) || \
    !defined(GUI_24BPP_GREEN_SHIFT) || !defined(GUI_24BPP_BLUE_SHIFT)
#error 24/32bpp colour not correctly configured
#endif
#endif

/* GUI_DibColor is a type large enough to contain a pixel. It is
 * not neccesarilly the exact size of a pixel
 * GUI_DibPixel is defined to be the same a GUI_DibColor and is
 * retained for backwards compatibility only.
 * BYTES_PER_DIB_PIXEL is set to the number of bytes in each pixel.
 * This should be used in preference to sizeof(GUI_DibColor).
 */

#if defined(USE_256_COLORS)

/* 8 bit color */
typedef uint8_t GUI_DibColor;

#define BYTES_PER_DIB_PIXEL 1

#elif defined(USE_16BIT_COLORS)

/* 16 bit color */
typedef uint16_t GUI_DibColor;

#define BYTES_PER_DIB_PIXEL 2

#elif defined(USE_24BIT_COLORS)

/* 24 bit color */
typedef uint32_t GUI_DibColor;

#define BYTES_PER_DIB_PIXEL 3

#elif defined(USE_32BIT_COLORS)

/* 32 bit color */
typedef uint32_t GUI_DibColor;

#define BYTES_PER_DIB_PIXEL 4

#endif

/* GUI_DibPixel is retained for backwards compatibility only
 * and should not be used in new code.
 */
typedef GUI_DibColor GUI_DibPixel;

/*
 * PIXELS_NEQ_COLOR is used to compare pixels with a color
 * this is currently only used for transparency processing.
 * ppix is and IU8 *, color must be a GUI_DibColor
 *
 * GUI_copyPixel is used to copy a pixel from one image to
 * another.
 */

#if defined(USE_256_COLORS) || defined(USE_16BIT_COLORS) || \
    defined(USE_32BIT_COLORS)

/* For 8, 16 and 32bpp we can rely on being able to just copy/compare the whole
 * pixel in one go.
 */
#define PIXELS_NEQ_COLOR(ppix,colour) \
            (*((GUI_DibPixel *)(ppix)) != colour)

#define GUI_copyPixel(dst, src) \
            *((GUI_DibPixel *)dst) = *((GUI_DibPixel *)src)

#elif defined(USE_24BIT_COLORS)

/* For 24bpp we have to do one byte at a time */

#define    PIXELS_NEQ_COLOR(ppix,colour) \
    ( \
        (((uint8_t *)(ppix))[GUI_24BPP_RED_BYTE] != \
                (((colour)>>GUI_24BPP_RED_SHIFT) & 0xff)) || \
        (((uint8_t *)(ppix))[GUI_24BPP_GREEN_BYTE] != \
                (((colour)>>GUI_24BPP_GREEN_SHIFT) & 0xff)) ||\
        (((uint8_t *)(ppix))[GUI_24BPP_BLUE_BYTE] != \
                (((colour)>>GUI_24BPP_BLUE_SHIFT) & 0xff)) \
    )

#define GUI_copyPixel(dst, src) \
            do { \
                ((uint8_t *)(dst))[0] = ((uint8_t *)(src))[0]; \
                ((uint8_t *)(dst))[1] = ((uint8_t *)(src))[1]; \
                ((uint8_t *)(dst))[2] = ((uint8_t *)(src))[2]; \
            } while (0)

#endif

/* Structure defining a bitmap */
typedef struct GUI_BitmapDesc_s {
#ifdef JVM_FBCC
    /* FIXME see CR21718: This structure is written to the start of a Java
     * byte[], which is anchored in an Image object. The finalizer of the Image
     * object does native cleanup of this structure, which leads to problems,
     * since the byte[] may already have been swept. The following 4-byte
     * buffer variable makes sure that the actual structure contents lie above
     * the 20 bytes that are overwritten and read by the GC and allocator. */
    IU32 __leave_unused;
#endif
#ifdef BMP_IN_HEAP
    IU32 pixelsOffset;
#else
    GUI_TargDIB bitmap;
    IU8 *pixelsBase;
#endif
    IS32 bytesPerLine;
    IU32 width;
    IU32 height;
    IU16 flags;
    GUI_DibColor transparency;
#ifdef SCREEN_HAS_ALPHA
    IU8 *alphaBase;
    IS16 alphasPerLine;
#endif
} GUI_BitmapDesc, *GUI_BitmapDescPtr;

/* Macros for dealing with bitmap flags */

#define BMP_FLG_MUTABLE                0x1
#define BMP_FLG_TRANSPARENT            0x2
#define BMP_FLG_PARTIAL_TRANSPARENT    0x4
#define BMP_FLG_TRANS_BY_COLOUR        0x8

#define GUI_bmpIsMutable(bmp) \
                (((bmp)->flags & BMP_FLG_MUTABLE) != 0)
#define GUI_bmpHasTransparency(bmp) \
                (((bmp)->flags & BMP_FLG_TRANSPARENT) != 0)
#define GUI_bmpHasPartialTransparency(bmp) \
                (((bmp)->flags & BMP_FLG_PARTIAL_TRANSPARENT) != 0)
#define GUI_bmpTransViaColour(bmp) \
                (((bmp)->flags & BMP_FLG_TRANS_BY_COLOUR) != 0)

#define GUI_bmpSetFlag(bmp, val, flag) \
    do {                               \
        if (val)                       \
            (bmp)->flags |= flag;      \
        else                           \
            (bmp)->flags &= ~flag;     \
    } while (0)

#define GUI_bmpSetMutable(bmp, val) \
                GUI_bmpSetFlag((bmp), (val), BMP_FLG_MUTABLE)
#define GUI_bmpSetHasTransparency(bmp, val) \
                GUI_bmpSetFlag((bmp), (val), BMP_FLG_TRANSPARENT)
#define GUI_bmpSetHasPartialTransparency(bmp, val) \
                GUI_bmpSetFlag((bmp), (val), BMP_FLG_PARTIAL_TRANSPARENT)
#define GUI_bmpSetTransViaColour(bmp, val) \
                GUI_bmpSetFlag((bmp), (val), BMP_FLG_TRANS_BY_COLOUR)

#ifdef BMP_IN_HEAP
#define GUI_getBmpAddr(bPtr)    (GUI_TargDIB)((bPtr)+1)
#define GUI_getPixelAddr(bPtr)  (((IU8*)(bPtr)) + (bPtr)->pixelsOffset)
extern int GUI_getBmpStoreSize(int width, int height);
#else
#define GUI_getBmpAddr(bPtr)    ((bPtr)->bitmap)
#define GUI_getPixelAddr(bPtr)  ((bPtr)->pixelsBase)
#endif

/* A macro used for checking that a pixel is within the image.
 * This is used to double check clipping on asserting builds.
 */
#if defined(IASSERTS) && !defined(SCREEN_ROTATION_SUPPORT) /* { */

#define checkPixelAddrValid(adr, bmp)                                         \
    do {                                                                      \
        IS32 __bmpSize = (IS32)((IS32)(bmp)->bytesPerLine*((bmp)->height-1)); \
        IU8* __startAddr = GUI_getPixelAddr(bmp);                             \
        if ((bmp)->bytesPerLine < 0)                                          \
        {                                                                     \
            __startAddr += ((IS32)(bmp)->bytesPerLine * ((bmp)->height-1));   \
            __bmpSize = -__bmpSize;                                           \
        }                                                                     \
        __bmpSize +=  (IS32)(bmp)->width * BYTES_PER_DIB_PIXEL;               \
        Iassert((IU32)(adr - __startAddr) < (IU32)__bmpSize);                 \
    } while(0)
#else /* } IASSERTS { */

#define checkPixelAddrValid(adr, bmp)     do { UNUSED(bmp); }while(0)

#endif /* } IASSERTS */


/* GUI_setPixelColor sets a pixel in an image to the given pixel value.
 * GUI_getPixelColor gets reads a pixel value from an image.
 *
 * The layout of colors may differ with targets, so target code can override
 * by defining its own version of GUI_setPixelColor and GUI_getPixelColor.
 */

#if !defined(GUI_setPixelColor)

#if defined(USE_256_COLORS) || defined(USE_16BIT_COLORS) || \
    defined(USE_32BIT_COLORS)
/* 8, 16 or 32 bit color */

#define GUI_getPixelColor(ppix) *((GUI_DibPixel *)(ppix))
#define GUI_setPixelColor(colour, ppix) \
            *((GUI_DibPixel *)(ppix)) = (GUI_DibPixel)(colour)

#elif defined(USE_24BIT_COLORS)
/* 24 bit color */

#define GUI_getPixelColor(ppix) \
        ((GUI_DibColor) \
            ( \
                (((uint32_t)(((uint8_t *)(ppix))[GUI_24BPP_RED_BYTE])) << \
                    GUI_24BPP_RED_SHIFT) | \
                (((uint32_t)(((uint8_t *)(ppix))[GUI_24BPP_GREEN_BYTE])) << \
                    GUI_24BPP_GREEN_SHIFT) | \
                (((uint32_t)(((uint8_t *)(ppix))[GUI_24BPP_BLUE_BYTE])) << \
                    GUI_24BPP_BLUE_SHIFT) \
            ) \
         )

#define GUI_setPixelColor(colour, ppix) \
            do { \
                ((uint8_t *)(ppix))[GUI_24BPP_RED_BYTE] = \
                    (uint8_t)((colour>>GUI_24BPP_RED_SHIFT) & 0xff); \
                ((uint8_t *)(ppix))[GUI_24BPP_GREEN_BYTE] = \
                    (uint8_t)((colour>>GUI_24BPP_GREEN_SHIFT) & 0xff); \
                ((uint8_t *)(ppix))[GUI_24BPP_BLUE_BYTE] = \
                    (uint8_t)((colour>>GUI_24BPP_BLUE_SHIFT) & 0xff); \
            } while(0)

#endif

#endif /* !defined(GUI_setPixelColor) */

/* GUI_drawPixelInternal is used to plot a single pixel in a bitmap. On an
 * asserting build it first checks if the address is within the image. This
 * should be used in preference to GUI_setPixelColor unless the pixel
 * address has been checked in some other way. eg by checking either end of
 * a horizontal line of pixels.
 * Use this macro only within the header file. In source, use
 * GUI_drawPixelDirect instead.
 */
#define GUI_drawPixelInternal(pix, adr, bmp)     \
            do {                               \
                checkPixelAddrValid(adr,bmp);  \
                (void)(adr - (IU8*)0);         \
                GUI_setPixelColor(pix,adr);    \
            } while(0)

/* GUI_convertPixelColor converts from a java color to a platform
 * specific pixel value. GUI_unConvertPixelColor does the reverse.
 */

#ifndef GUI_convertPixelColor

#if defined(USE_256_COLORS) || defined(USE_16BIT_COLORS)
/* 8 bit color is implemented by each port as function
 * 16 bit color is either implemented as a macro in
 * targ_rbgraphics.h or as a function in the port.
 */
extern GUI_DibColor GUI_convertPixelColor(IU32 pixel);

#elif defined(USE_24BIT_COLORS) || defined(USE_32BIT_COLORS)
/* 24 and 32 bit color can be done here as macros */

#define GUI_convertPixelColor(pix) \
    ((GUI_DibColor) \
        ( \
            (((((IU32)(pix)) >> 16) & 0xff) << GUI_24BPP_RED_SHIFT) | \
            (((((IU32)(pix)) >>  8) & 0xff) << GUI_24BPP_GREEN_SHIFT) | \
            (((((IU32)(pix)) >>  0) & 0xff) << GUI_24BPP_BLUE_SHIFT) \
        ) \
    )
#endif
#endif


#ifndef GUI_unConvertPixelColor

#if defined(USE_256_COLORS) || defined(USE_16BIT_COLORS)
/* 8 bit color is implemented by each port as function
 * 16 bit color is either implemented as a macro in
 * targ_rbgraphics.h or as a function in the port.
 */
extern IU32 GUI_unConvertPixelColor(GUI_DibColor pixel);

#elif defined(USE_24BIT_COLORS) || defined(USE_32BIT_COLORS)
/* 24 and 32 bit color can be done here as macros */

#define GUI_unConvertPixelColor(pix) \
    ((GUI_DibColor) \
        ( \
            (((((IU32)(pix)) >> GUI_24BPP_RED_SHIFT) & 0xff) << 16) | \
            (((((IU32)(pix)) >> GUI_24BPP_GREEN_SHIFT) & 0xff) << 8) | \
            (((((IU32)(pix)) >> GUI_24BPP_BLUE_SHIFT) & 0xff) << 0) \
        ) \
    )
#endif
#endif


#define GUI_clipXIdx        0
#define GUI_clipYIdx        1
#define GUI_clipWidthIdx    2
#define GUI_clipHeightIdx   3

/* Flag set during paint operation */
#define GUI_duringPaint     STATIC_javax_microedition_lcdui_Display_duringPaint

#ifdef SCREEN_ROTATION_SUPPORT

/**
 * The anti-clockwise rotation angle.
 * Note: the current version only supports Rotate_0 and Rotate_90
 */
typedef enum _ScreenRotationType{
    Rotate_0 = 0,
    Rotate_90,
    Rotate_180,
    Rotate_270
}ScreenRotationType;
extern ScreenRotationType getCurrentScreenRotationState(void);

#endif /* SCREEN_ROTATION_SUPPORT */

extern IU32 backgroundColor;
extern int LCDUI_depth;
extern IBOOL doubleBuffer;
#ifdef EXTERNAL_DISPLAY
extern void GUI_setupScreens(int maxScreen);
extern GUI_BitmapDescPtr *screenBitmap;
#else /* EXTERNAL_DISPLAY */
extern GUI_BitmapDescPtr screenBitmap;
extern GUI_BitmapDescPtr horScreenBitmap;
extern GUI_BitmapDescPtr currentScreenBitmap;
#endif /* EXTERNAL_DISPLAY */
extern IBOOL GUI_missedPaint;

extern GUI_BitmapDescPtr getActiveScreenBitmap(void);
#ifdef SPRINT_EXTERNAL_DISPLAY
extern void GUI_createBitmap(GUI_BitmapDescPtr bmpDesc, IDisplay *theScreen);
#else
extern void GUI_createBitmap(GUI_BitmapDescPtr bmpDesc);
#endif
extern void GUI_releaseBitmap(GUI_BitmapDescPtr bmpDesc);

extern IU32 GUI_getFontId(FONTPARAMS_PROTO);
extern void GUI_createFont(GUI_FontInfoPtr font, FONTPARAMS_PROTO);
extern void GUI_releaseFont(GUI_TargFont theFont);
extern int GUI_getFontCharExtent(GUI_TargFont theFont,
                            IU16 theChar, IU8 *width);
extern void GUI_getFontCharBits(IU8 *dst, GUI_TargFont theFont, IU16 charNum,
                            IU8 width, IU8 height);
extern void GUI_initText(void);
extern void GUI_finalizeText(void);

extern void LCDUIinitText(void);
extern void LCDUIfinalizeText(void);

#ifdef SPRINT_SUPPORT
#define JBED__PRIMARY_LCD STATIC_javax_microedition_lcdui_Graphics_PRIMARY_LCD
extern GUI_BitmapDescPtr extScreenBitmap;
extern void refreshExternalPaintWindow(int x1, int y1, int x2, int y2);
extern void RB_extRegionIsDirty(int x1, int y1, int x2, int y2);
#endif

#ifdef EXTERNAL_DISPLAY
extern void refreshPaintWindow(int s, int x1, int y1, int x2, int y2);
extern void RB_regionIsDirty(int s, int x1, int y1, int x2, int y2);
#else /* EXTERNAL_DISPLAY */
extern void refreshPaintWindow(int x1, int y1, int x2, int y2);
extern void RB_regionIsDirty(int x1, int y1, int x2, int y2);
#endif /* EXTERNAL_DISPLAY */
extern void GUI_checkPostponedRefresh(void);

#ifdef SLOW_DRAWING
extern int drawingInterval;

#define drawSleep() do { OSAPI_sleep(drawingInterval); } while (0)
#define drawShortSleep() do { OSAPI_sleep(drawingInterval/1000); } while (0)

#else

#define drawSleep() do { /* nothing */ } while (0)
#define drawShortSleep() do { /* nothing */ } while (0)

#endif

#ifndef NEED_TIMESLICE_COUNTDOWN
extern IU32 GUI_maxRefreshSeparationMs;
#endif

#ifdef ALPHA_BLENDING_SUPPORT
/* Define GUI_blendPixel in targ_rbgraphics.h if the pixel format
 * is non standard. */
#ifndef GUI_blendPixel
#if defined(USE_256_COLORS)
/* For 256 colors, just choose one of the colors as blending is
 * probably pointless.
 */
#define GUI_blendPixel(dColor, screenColor, col, al) \
    do { \
        (dColor) = (((al) < 128) ? (screenColor) : (col)); \
    } while (0)

#elif defined(USE_16BIT_COLORS)
#ifdef FAST_ALPHA_BLENDING
#define GUI_blendPixel(color, d, s, alpha)            \
do {                                                  \
    unsigned int srgb = ((s << 16) | s) & 0x07E0F81F; \
    unsigned int drgb = ((d << 16) | d) & 0x07E0F81F; \
    drgb += ((srgb - drgb) * (alpha >> 3)) >> 5;      \
    drgb &= 0x07E0F81F;                               \
    (color) = (GUI_DibPixel)(drgb >> 16 | drgb);      \
} while (0)
#else
#define GUI_blendPixel(color, d, s, alpha)      \
do {                                            \
    unsigned int srb = (s) & 0xF81F;            \
    unsigned int sg = (s) & 0x07E0;             \
    unsigned int drb = (d) & 0xF81F;            \
    unsigned int dg = (d) & 0x07E0;             \
    drb += ((srb - drb) * (alpha >> 2)) >> 6;   \
    dg += ((sg - dg) * (alpha >> 2)) >> 6;      \
    drb &= 0xF81F;                              \
    dg &= 0x07E0;                               \
    (color) = (GUI_DibPixel)(drb | dg);         \
} while (0)
#endif

#elif defined(USE_24BIT_COLORS) || defined(USE_32BIT_COLORS)
/* For 24 and 32 bpp, convert to Java format, blend and
 * convert back again. If the compiler is paying attention
 * it can possibly optimize the conversions away.
 */
#define LCDUIPixelBlend(color, d, s, a)         \
{                                               \
    const IU32 dstrb = (d) & 0xFF00FF;          \
    const IU32 dstg  = (d) & 0xFF00;            \
                                                \
    const IU32 srcrb = (s) & 0xFF00FF;          \
    const IU32 srcg  = (s) & 0xFF00;            \
                                                \
    IU32 drb = srcrb - dstrb;                   \
    IU32 dg  =  srcg - dstg;                    \
                                                \
    IU32 rb, g;                                 \
                                                \
    (a) += 1;                                   \
                                                \
    drb *= (a);                                 \
    dg  *= (a);                                 \
    drb >>= 8;                                  \
    dg  >>= 8;                                  \
                                                \
    rb = (drb + dstrb) & 0xFF00FF;              \
    g  = (dg  + dstg) & 0xFF00;                 \
                                                \
    (color) = rb | g;                           \
}
#define GUI_blendPixel(dColor, screenColor, col, al)   \
{                                                      \
    IU32 tempColor;                                    \
    IU32 d1 = GUI_unConvertPixelColor((screenColor));  \
    IU32 s1 = GUI_unConvertPixelColor((col));          \
    IU32 al1 = (al);                                   \
    LCDUIPixelBlend(tempColor, d1, s1, al1);           \
    dColor = GUI_convertPixelColor(tempColor);         \
}
#endif
#endif/*!GUI_blendPixel*/
#endif/*ALPHA_BLENDING_SUPPORT*/

#ifdef SCREEN_HAS_ALPHA /* { */
#define SET_OPAQUE_ALPHA(pAddr, dst) \
do { \
    IU8 *aAddr = (dst)->alphaBase; \
    if (aAddr) { \
        int pixOffset = (pAddr) - GUI_getPixelAddr(dst); \
        aAddr += (pixOffset / BYTES_PER_DIB_PIXEL); \
        *aAddr = 0xff; \
    } \
} while (0)
#else   /* } SCREEN_HAS_ALPHA { */
#define SET_OPAQUE_ALPHA(addr, dst) do {} while (0)
#endif  /* } SCREEN_HAS_ALPHA */

/* Macro to draw pixel in "fast case" code where we know the alpha
 * is opaque. Simply write opaque alpha value too if required. */
#define GUI_drawPixelDirect(srcCol, pAddr, dst) \
do { \
    SET_OPAQUE_ALPHA(pAddr, dst); \
    GUI_drawPixelInternal(srcCol, pAddr, dst); \
} while (0)

/* The compiler will remove the code of 'else' branch
   for non-PRIMITIVE_DO_ALPHA builds because JBED_IS_FULL_OPAQUE(al)
   is true when PRIMITIVE_DO_ALPHA is not defined */
#define GUI_drawAlphaPixel(al, col, addr, dst)              \
{                                                           \
    if (JBED_IS_FULL_OPAQUE(al)) {                          \
        SET_OPAQUE_ALPHA(addr, dst);                        \
        GUI_drawPixelInternal(col, addr, dst);              \
    } else {                                                \
        GUI_drawAlphaPixelAlways(al, col, addr, dst);       \
    }                                                       \
}

#ifdef SCREEN_HAS_ALPHA /* { */

#define GUI_drawAlphaPixelAlways(srcAlpha, srcCol, pAddr, dst) \
do { \
    IU8 *aAddr = (dst)->alphaBase; \
    if (aAddr) { /* Special case when destination has alpha */ \
        unsigned int notSrcA = 255 - (srcAlpha); \
        unsigned int sr = (srcCol) & 0xF800; \
        unsigned int sg = (srcCol) & 0x07E0; \
        unsigned int sb = (srcCol) & 0x001F; \
        unsigned int dr = (*((GUI_DibColor *)(pAddr))) & 0xF800; \
        unsigned int dg = (*((GUI_DibColor *)(pAddr))) & 0x07E0; \
        unsigned int db = (*((GUI_DibColor *)(pAddr))) & 0x001F; \
        unsigned int da, na; /* weighted-old and new dst alphas */ \
        int pixOffset = (pAddr) - GUI_getPixelAddr(dst); \
        aAddr += (pixOffset / BYTES_PER_DIB_PIXEL); \
        da = *((IU8 *)(aAddr)) * notSrcA; /* Reuse weighted dst below */ \
        /* Divide by 255 using shifts and adds */ \
        da = (da + (da >> 8) + 0x80) >> 8; \
        /* New alpha is required to get correct color components */ \
        na = (srcAlpha) + da; \
        *((IU8 *)(aAddr)) = (IU8)na; \
        if (na == 0) { \
            /* Don't bother writing pixel value if transparent */ \
            *((GUI_DibColor *)(pAddr)) = 0; \
        } else { \
            dr = ( (sr * (srcAlpha)) + (dr * da) + (na << (11 - 1)) ) / na; \
            dr &= 0xF800; \
            dg = ( (sg * (srcAlpha)) + (dg * da) + (na << ( 5 - 1)) ) / na; \
            dg &= 0x07E0; \
            db = ( (sb * (srcAlpha)) + (db * da) + (na >> 1) ) / na; \
            db &= 0x001F; \
            *((GUI_DibColor *)(pAddr)) = (GUI_DibColor)(dr | dg | db); \
        } \
    } \
    else \
    {   /* No alpha channel in destination ... normal case */ \
        GUI_DibPixel dColor; \
        GUI_DibPixel screenColor = GUI_getPixelColor(pAddr); \
        GUI_blendPixel(dColor, screenColor, srcCol, srcAlpha); \
        GUI_drawPixelInternal(dColor, pAddr, dst); \
    } \
} while (0)

#else   /* } SCREEN_HAS_ALPHA { */

/* Draw alpha pixel always (assume alpha is NOT fully opaque). */
#define GUI_drawAlphaPixelAlways(al, col, addr, dst)        \
{                                                           \
    GUI_DibPixel dColor;                                    \
    GUI_DibPixel screenColor = GUI_getPixelColor(addr);     \
    GUI_blendPixel(dColor, screenColor, col, al);           \
    GUI_drawPixelInternal(dColor, addr, dst);                 \
}

#endif  /* } SCREEN_HAS_ALPHA */

#if defined(JVM_FBCC) && defined(GRAPHICS_MAX_REFRESH_RATE)
extern void TH_arrangeForAsyncActionLater(int64_t now, int64_t wakeupTime);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* RBGRAPHICS_H_INCLUDED */
