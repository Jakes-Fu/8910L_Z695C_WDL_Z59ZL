/*
 * Copyright 1998-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Framebuffer graphics support.
 *
 * This interface provides raw-bitmap font
 * support for the Jbed VM.
 *
 * <!-- #interface list begin -->
 * \section cpl_font_interface Interface
 * - Case Native font:
 *    - CPL_font_startup()
 *    - CPL_font_shutdown()
 *    - CPL_font_getFontId()
 *    - CPL_font_createFont()
 *    - CPL_font_releaseFont()
 *    - CPL_font_getFontCharExtent()
 *    - CPL_font_getFontCharBits()
 *    - CPL_font_prepareFace()
 * <!-- #interface list end -->
 *
 * Font sizes can either be specified as SIZE_SMALL, SIZE_MEDIUM, SIZE_LARGE or
 * as a negative number which is minus the requested font height in points.
 * The actual size of the selected font is target specific.
 *
 * Font faces can be specified as FACE_MONOSPACE, FACE_PROPORTIONAL
 * OR FACE_SYSTEM, or as a negative number which is an internal ID.
 * These IDs are allocated by the implementation as return values of
 * CPL_font_prepareFace.
 */

#ifndef __JBED_CPL_FONT_H__
#define __JBED_CPL_FONT_H__

#include <jbed_global.h>

#include <targ_font.h>  /* for definition of jbed_font_bpp */
#ifndef JBED_FONT_BPP
#define JBED_FONT_BPP     (1)
#endif /* JBED_FONT_BPP */

#if defined(__cplusplus)
extern "C" {
#endif

/**
  * @name Return values for the CPL_font functions.
  *
  * Error values are negative, success values are zero or positive.
  *
  * @internal
  * This allows to do simply:
  * <pre>
  * if(retVal < 0) Trace("An error happened %d", retVal);
  * </pre>
  */

/*@{*/

/**
 * OK code returned by CPL_font functions.
 */
#define CPL_FONT_OK 0

/**
 * Failure code returned by CPL_font functions.
 */
#define CPL_FONT_ERROR -1

/*@}*/

#ifdef BASIS_PROFILE
/**
  * @name Constants for font API parameters
  *
  * The FACE_ constants are the possible values of the 'face'
  * parameter to the various APIs, except that negative values
  * are also allowed, as described in CPL_font_prepareFace.
  *
  * The SIZE_ constants are the possible values of the 'size'
  * parameter to the various APIs, except that negative values
  * are also allowed, as described in CPL_font_createFont.
  *
  * @internal
  * The values of these constants are cross-checked against the values
  * used in Java at build time.
  * This is because the Java classes providing the values vary between
  * MIDP and PBP, and the relevant Java headers are not shipped as part
  * of the porting API.
  */

/*@{*/

/** Constant for system font face selection in MIDP. */
#define FACE_SYSTEM         0

/** Constant for monospaced font face selection in MIDP. */
#define FACE_MONOSPACE      32

/** Constant for proportional font face selection in MIDP. */
#define FACE_PROPORTIONAL   64

/** Constant for small font size selection in MIDP. */
#define SIZE_SMALL          8

/** Constant for medium font size selection in MIDP. */
#define SIZE_MEDIUM         0

/** Constant for large font size selection in MIDP. */
#define SIZE_LARGE          16

/** Bitmask for plain font style, matching that in java.awt.Font. */
#define STYLE_PLAIN         0

/** Bitmask for bold font style, matching that in java.awt.Font. */
#define STYLE_BOLD          1

/** Bitmask for italic font style, matching that in java.awt.Font. */
#define STYLE_ITALIC        2

/** Bitmask for underline font style, matching that in java.awt.Font. */
#define STYLE_UNDERLINED    4


/*@}*/

#endif  /* BASIS_PROFILE */

/**
 * @file targ_rbgraphics.h
 * Definitions that reflect the configuration of the Jbed internal drawing code.
 *
 * This function among other things data structures to represent
 * pixels, images and fonts.  This file is included to ensure externally visible
 * definitions stay in sync with the configuration. This file is normally
 * configured by Esmertec at release creation time. Non-source customers should
 * contact Esmertec in the unlikely event anything needs changing.
 *
 * <em>The file included in this documentation is an example only. Unless documented
 * otherwise defines, declarations, typedefs etc are for Esmertec internal use only.</em>
 */

/**
 * @var typedef uint8_t* GUI_TargFont
 * Target font data. Effectively an opaque pointer type.
 * Typedef it to point to your font data type before including
 * cpl_font.h.
 *
 * This is normally a generic pointer field. (Partial) binary customers who
 * cannot recompile rb_text.c should contact Esmertec in the rare case
 * a different definition is required.
 */

/**
 * Font information structure.
 *
 * Most of this filled in by CPL_font_createFont().
 * The Java visible height of the font = leading + ascender + descender.
 * The height of the pixel data request will be = ascender + descender.
 * PBP additionally needs a maximum advance value, but will accept -1
 * meaning "information not available".
 */
typedef struct GUI_FontInfo_s
{
#ifdef  JBED_FREETYPE2
    struct FT_FaceRec_*    targInfo; /**< Target specific font information */
#else   /* JBED_FREETYPE2 */
    GUI_TargFont targInfo; /**< Target specific font information */
#endif  /* JBED_FREETYPE2 */
    uint32_t    id;     /**< Font ID value as returned by CPL_font_getFontID */
    int     ascender;   /**< Lines of characters above and including base line */
    int     descender;  /**< Lines of characters below base line */
    int     leading;    /**< Whitespace to be added to the top of the lines */
#ifdef BASIS_PROFILE
    int     maxAdvance; /**< Largest advance of any char in the font (PBP) */
#endif
} GUI_FontInfo, *GUI_FontInfoPtr;

/**
 * Perform any required initialization tasks for the target-specific
 * support for raw bitmap text rendering.
 *
 * @return CPL_FONT_OK (function is currently not allowed to fail)
 */
int32_t CPL_font_startup(void);


/**
 * Perform any required tidy-up tasks for the target-specific
 * support for raw bitmap text rendering.
 *
 * @return CPL_FONT_OK (function is currently not allowed to fail)
 */
int32_t CPL_font_shutdown(void);

/**
 * Obtain a unique integer identifier for the target font to be used
 * when rendering the java font described. Note that a target may
 * map several fonts which are different in java terms to the same
 * target font.
 *
 * @param face    The java font face indicator
 * @param size    The java font size indicator or minus the font height in
 *                points if negative
 * @param style   The java font style indicator
 *
 * @return        the unique identifier for the target font
 */
uint32_t CPL_font_getFontId(int face,
                            int style,
                            int size);

/**
 * Acquire a target font corresponding to the java parameters and/or
 * the unique target font id supplied.
 *
 * @param font    Pointer to a structure which describes the target font.
 * This structure already has the unique ID for the target
 * font (as chosen by CPL_font_getFontId) in its id field.
 *
 * The created target font should be in the targInfo field of the
 * font structure, and also the ascender, descender and leading
 * height fields should be initialised.
 *
 * @param face    The java font face indicator
 * @param size    The java font size indicator or minus the font height in
 *                points if negative
 * @param style   The java font style indicator
 *
 * @return CPL_FONT_OK (function is currently not allowed to fail)
 */
int32_t CPL_font_createFont(GUI_FontInfoPtr font,
                            int face,
                            int style,
                            int size);

/**
 * Release a target font previously acquired via CPL_font_createFont.
 *
 * @param theFont   the target font to be released
 *
 * @return CPL_FONT_OK (function is currently not allowed to fail)
 */
int32_t CPL_font_releaseFont(GUI_FontInfoPtr theFont);

/**
 * Obtain the width in pixels for a supplied single unicode
 * character in the specified target font.
 *
 * The width in pixels of the character in the location pointed to
 * by pwidth.
 *
 * @param theFont    the target font to be used
 * @param theChar    the unicode character
 * @param pwidth     location of the result
 *
 * @return the extent in pixels of the character
 */
int CPL_font_getFontCharExtent(GUI_FontInfoPtr theFont,
                               uint16_t theChar,
                               uint8_t *pwidth);

/**
 * Obtain the left position and the cursor advancement requirement
 * for a supplied single unicode character in the specified target
 * font.
 *
 * @param theFont    the target font to be used
 * @param theChar    the unicode character
 * @param pWidth     the width of the rendered character bitmap
 * @param pLeft      the left-most position of the char (possibly negative)
 * @param pAdvance   the amount the cursor should advance
 *
 * @return font status code: CPL_FONT_OK = 0
 */
#ifdef SIMPLE_KERNING_SUPPORT
int CPL_font_getFontCharPosX(GUI_FontInfoPtr theFont,
                               uint16_t theChar,
                               int      *pWidth,
                               int      *pLeft,
                               int      *pAdvance);
#endif

/**
 * Obtain bitmap data for a single character in the specified font.
 * The bitmap data is in one bit per pixel format, with each scanline
 * starting on a byte boundary.
 *
 * The resulting in bytes of character data are stored
 * at the location pointed by dst.
 *
 * Note: This is a cofunction of getFontCharExtent(). It will only
 * be called if a prior call to getFontCharExtent() has been made.
 *
 * @param dst        Where to put the bitmap data (storage for
 *                   <code>width*height</code> bytes)
 * @param theFont    The font to find the character data for
 * @param charNum    The character
 * @param width      The width of the character data (in pixels)
 * @param height     The height of the character data (in scanlines)
 *
 * @return CPL_FONT_OK (function is currently not allowed to fail)
 *
 * <b>Bitmap Format detail</b>
 *
 * Format is Top Left Right.  Rows are byte aligned.  Bit order is MSB
 * first.  <em>Note that this is the inverse bit order from the
 * equivalent JbedME FastBCC 5.0 interface.</em>  The returned bits
 * should include leading and trailing whitespace and should include
 * any extent.
 *
 * Example:
 * \code
 * Assume that the font is 9 pixels wide(extent) and 11 pixels tall, then a call to
 * CPL_font_getCharbits(dst,font, '!', 9, 11) should return:
 *
 *               Bit layout                 Byte Layout
 *        Bit  No     7      0 7      0
 *        dst     -> |----++--|-*******|    |  0 |  1 |
 *                   |----++--|-*******|    |  2 |  3 |
 *                   |----++--|-*******|    |  4 |  5 |
 *                   |----++--|-*******|    |  6 |  7 |
 *                   |----++--|-*******|    |  8 |  9 |
 *                   |--------|-*******|    | 10 | 11 |
 *                   |--------|-*******|    | 12 | 13 |
 *                   |----++--|-*******|    | 14 | 15 |
 *                   |----++--|-*******|    | 16 | 17 |
 *                   |--------|-*******|    | 18 | 19 |
 *                   |--------|-*******|    | 20 | 21 |
 *
 * | = byte boundary; + = active bit; - inactive bit; * must be zero
 * \endcode
 */
int32_t CPL_font_getFontCharBits(uint8_t *dst,
                                 GUI_FontInfoPtr theFont,
                                 uint16_t charNum,
                                 uint8_t width,
                                 uint8_t height);

#ifdef BASIS_PROFILE
/** Prepare face from hint string and return unique ID.
 *
 * This API is only called for PBP ports, and does not need to be
 * implemented for MIDP ports.
 *
 * The higher level code may wish to access a wider range of font
 * faces than the three standard MIDP ones.
 * It can call this API to supply hints to the implementation
 * font engine, to identify which fonts it wants.
 * They could be filenames, encoded handle values or even the actual
 * font memory encoded as a very long string!
 * The implementaion is responsible for interpreting the string and
 * validating it.
 * If it is a file name for example, the file should be opened and
 * read by this API, so that the hint is known to be valid.
 *
 * The same String will be passed down multiple times by the Java code.
 * Native code should first match against previous hints and return
 * the same ID as before. Only if new should it try to use the hint.
 * This means creating the face data structures as hinted and checking
 * that the font is usable.
 * If successful, then it should allocate a new ID.
 *
 * If all hints are rejected, the MIDP FACE_* constants will be used
 * instead, so just returning zero is a sensible default
 * implementation for legacy ports.
 *
 * @param hintString    character string whose meaning depends on
 *                      the underlying native font engine.
 * @return  a face ID to be used when drawing text, or zero to
 *      indicate that the hint is not acceptable and will not be used.
 *      ID values must be negative, to distinguish them from
 *      the default MIDP FACE_* constants.
 */
int32_t CPL_font_prepareFace(const char *hintString);
#endif  /* BASIS_PROFILE */

#ifdef JBED_NATIVE_DRAWSTRING_SUPPORT
/**
 * Function makes a bitmap for whole string and writes it in the bmp.
 *
 * @param theFont the target font to be used
 * @param bmp Bitmap, where shold be done string image
 * @param size Size of memory, allocated for bitmap
 * @param charArray Character array to draw
 * @param nChars Number of characters in array
 */
void CPL_font_drawString(GUI_FontInfoPtr theFont, uint8_t *bmp, int32_t size,
                         const jchar *charArray, int32_t nChars);

/**
 * Calculate the width of unicode string.
 * Note that this function returns two types of widths:
 * extent (returned)   - is the width including any overhang
 * advance (*advWidth) - is the width excluding any overhang, and is
 * the offset where adjacent characters should be drawn.
 *
 * @param theFont       the target font to be used
 * @param charrArray    Character array to draw
 * @param nChars        Number of characters to draw
 * @param advWidth      Ptr to advance width (may be NULL)
 * @return Extent width of characters
 */
int32_t CPL_font_getStringWidth(GUI_FontInfoPtr theFont,
                                const jchar *charArray, int32_t nChars,
                                int32_t* advWidth);
#endif /* JBED_NATIVE_DRAWSTRING_SUPPORT */

#ifdef NATIVE_VECTOR_FONT

/**
 * Function draws the whole string and writes it on context.
 *
 * @param lcduiCtxtPtr the drawing context
 * @param fontInfo the drawing font discription
 * @param anchorX the start drawing point x
 * @param anchorY the start drawing point y
 * @param anchor drawing align type
 * @param charArray Character array to draw
 * @param nChars Number of characters in array
 */
void CPL_font_drawString(LCDUI_ContextPtr lcduiCtxtPtr,
                        GUI_FontInfo fontInfo,
                        int anchorX, int anchorY,
                        int anchor,
                        const jchar *charArray,
                        int32_t nChars);

/**
 * Calculate the width of unicode string.
 * Note that this function returns two types of widths:
 * extent (returned)   - is the width including any overhang
 * advance (*advWidth) - is the width excluding any overhang, and is
 * the offset where adjacent characters should be drawn.
 *
 * @param fontInfo       the target font to be used
 * @param charrArray    Character array to draw
 * @param nChars        Number of characters to draw
 * @param advWidth      Ptr to advance width (may be NULL)
 * @return Extent width of characters
 */

int32_t CPL_font_getStringWidth(GUI_FontInfo * fontInfo,
                                const jchar *charArray,
                                int32_t nChars,
                                int * advWidth);

#endif /* NATIVE_VECTOR_FONT */

#if defined(__cplusplus)
}
#endif

#endif /* __JBED_CPL_FONT_H__ */
