/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

/***************************************************************************
 * FILE: kjava_sys_gfx.h
 * 
 * DESCRIPTION:
 *      The module is one of KJava interface modules. It is for graphics 
 *      operations.
 *
 * UPDATED DATE:
 *      2004/11/03
 *
 * REVISION:
 *      2004/09/19,  Yagamy,     draft version.
 *      2004/10/21,  Simon,      Add two element(viewRegion and softKeyRegion)
 *                               of GfxInfo
 *      2004/11/03,  Simon,      Change the parameter of getSuitableFont for 
 *                               Default font
 *
 ***************************************************************************/

#ifndef KJAVA_SYS_GFX_HEADER
#define KJAVA_SYS_GFX_HEADER
#include "jktypes.h"
#include "jktypesajdx.h"


/**
 * @file kjava_sys_gfx.h
 *  Low-Level Graphics KSI
 */

#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup graphics_ksi
 *  @{
 */

/*
 *  Interfaces listed in the group are used for:
 *      - GUI information acquire.
 *      - Virtual screen buffer allocation and flushing.
 *      - Font information acquire and drawing.
 *      - Image codec.
 *      .
 */

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

/** 
 * @addtogroup Font_Drawing Font Drawing
 * @{
 */

/**
 * \brief Plain font 
 */
#define KJAVA_FONT_STYLE_PLAIN            0
/** 
 * \brief Bold font 
 */
#define KJAVA_FONT_STYLE_BOLD             1
/** 
 * \brief Italic font 
 */
#define KJAVA_FONT_STYLE_ITALIC           2
/** 
 * \brief Underline font 
 */
#define KJAVA_FONT_STYLE_UNDERLINE        4

/** 
 * \brief Default font 
 */

#define KJAVA_FONT_FACE_DEFAULT           0

/** 
 * \brief Monospace font 
 */
#define KJAVA_FONT_FACE_MONOSPACE         32
/** 
 * \brief Proportional font 
 */
#define KJAVA_FONT_FACE_PROPORTIONALE     64

/** 
 * \brief The default font used for low-level graphics drawing 
 */
#define KJAVA_FONT_TYPE_DEFAULT           0
/** 
 * \brief The default display font used by GUI 
 */
#define KJAVA_FONT_TYPE_STATIC_TEXT       0x1000
/** 
 * \brief The default editing font used by GUI 
 */
#define KJAVA_FONT_TYPE_INPUT_TEXT        0x1001


/** 
 * \brief Small size font 
 */
#define KJAVA_FONT_SIZE_SMALL             1
/** 
 * \brief Medium size font 
 */
#define KJAVA_FONT_SIZE_MEDIUM            2
/** 
 * \brief Large size font 
 */
#define KJAVA_FONT_SIZE_LARGE             3

/** @} */

/** 
 * @addtogroup LCD_Drawing Low Level LCD Drawing
 * @{
 */

/** 
 * \brief Normal screen mode 
 */
#define KJAVA_VIDEO_SCREEN_NORMAL         0
/** 
 * \brief Full screen mode 
 */
#define KJAVA_VIDEO_SCREEN_FULL           1

/** 
 * \brief No Displayable set. 
 *
 * This means that a <code>null</code> object was passed to <code>setCurrent</code> method. 
 */
#define KJAVA_VIDEO_DISPLAYABLE_NULL      0
/** 
 * \brief Screen type set. 
 *
 * This means that a <code>Screen</code> object was passed to <code>setCurrent</code> method. 
 */
#define KJAVA_VIDEO_DISPLAYABLE_SCREEN    1
/** 
 * \brief Canvas type set. 
 *
 * This means that a <code>Canvas</code> object was passed to <code>setCurrent</code> method. 
 */
#define KJAVA_VIDEO_DISPLAYABLE_CANVAS    2
/** 
 * \brief A permission confirmation dialog displayed. 
 *
 * This means that a permission confirmation dialog is currently displayed. 
 */
#define KJAVA_VIDEO_DISPLAYABLE_PERMISSIONDIALOG      4

/** @} */

/** 
 * @addtogroup Image_Codec Image Codec
 * @{
 */

/** 
 * \brief Image decoding or analysis succeeded
 */
#define KJAVA_CODEC_SUCCESS       0
 
/** 
 * \brief Unsupported image format
 */
#define KJAVA_CODEC_UNSUPPORT     1
 
/** 
 * \brief Bad source format 
 */
#define KJAVA_CODEC_BAD_FORMAT    2

/** 
 * \brief Insufficient resource for image decoding 
 */
#define KJAVA_CODEC_NO_RESOURCE   3

/** 
 * \brief The type for storing a UCS2 character 
 */

 /**
  * The type for storing a pixel data
  */
 typedef AJDX_PIXEL KJavaPixel;



typedef unsigned short  KJavaUnicode;

#if 0
#if defined(KJAVA_GFX_DISPLAY_565)
    /** 
     * The type for storing a pixel data
     */
    typedef unsigned short  KJavaPixel;

#elif defined (KJAVA_GFX_DISPLAY_555)
    /** 
     * The type for storing a pixel data
     */
    typedef unsigned short  KJavaPixel;

#elif defined (KJAVA_GFX_DISPLAY_666)
    /** 
     * The type for storing a pixel data
     */
    typedef unsigned int    KJavaPixel;
    
#elif defined (KJAVA_GFX_DISPLAY_888)
    /** 
     * The type for storing a pixel data
     */
    typedef unsigned int    KJavaPixel;
    
#else
     typedef unsigned short    KJavaPixel;
//#error You MUST define either pixel format of KJAVA_GFX_DISPLAY_565, KJAVA_GFX_DISPLAY_666, or KJAVA_GFX_DISPLAY_888 in the build settings.
#endif

#endif
/*
 *
 *  brief The structure type for containing the information of a screen buffer.
 */
typedef struct _KJavaImageTag
{
    /*
     *
     * brief A pointer to the pixel buffer.
     *
     * Only when <i>isMutable<i> is 1, the data is mutable and this field should be converted 
     * to <i>unsigned short* <i>
     */
    const    KJavaPixel* data;
    
    /*
     * 
     *  brief The width of the screen buffer 
     */
    int         width;
    
    /*
     * 
     *  brief The height of the screen buffer 
     */
    int         height;
    
    /*
     * 
     *  brief A flag indicating whether or not the pixel buffer contains transparency information.
     */
    int         transparency;
    
    /*
     * 
     *  brief A flag indicating whether of not the pixel buffer is mutalbe
     */
    int         isMutable;
}KJavaImage;

/** @} */

/** 
 * @ingroup LCD_Drawing
 */

/**
 * \brief The structure type for representing a clipping rectangle.
 */
typedef struct _KJavaClipTag
{
    /** 
     * \brief The x coordinate of the clipping rectangle. 
     */
    int       x;
    
    /** 
     * \brief The y coordinate of the clipping rectangle. 
     */
    int       y;
    
    /** 
     * \brief The width of the clipping rectangle. 
     */
    int       width;
    
    /** 
     * \brief The height of the clipping rectangle. 
     */
    int       height;
}
KJavaClip;

/** 
 * @ingroup Font_Drawing
 */
 
/**
 * \brief The structure type for containing font information.
 */
typedef struct _KJavaFontTag
{
    /** 
     * \brief The font identifier. This is reserved for future use.
     */
    int         id;
    
    /** 
     * \brief The font face. 
     *
     * The value is set as a logical OR of the following values.
     * - #KJAVA_FONT_FACE_MONOSPACE
     * - #KJAVA_FONT_FACE_PROPORTIONALE
     * .
     */
    int         face;
    
    /**
     * \brief The font style
     *
     * The value is set as a logical OR of the following values.
     * - #KJAVA_FONT_STYLE_PLAIN
     * - #KJAVA_FONT_STYLE_BOLD
     * - #KJAVA_FONT_STYLE_ITALIC
     * - #KJAVA_FONT_STYLE_UNDERLINE
     * .
     */
    int         style;
    
    /** 
     * \brief The font size
     *
     * The value one of the following values.
     * - #KJAVA_FONT_SIZE_SMALL
     * - #KJAVA_FONT_SIZE_MEDIUM
     * - #KJAVA_FONT_SIZE_LARGE
     * .
     */
    int         size;
    
    /** 
     * \brief The font leading
     *
     *  \image html ksi-gfx-font-1.jpg
     */
    int         char_leading;

    /** 
     * \brief The font ascent
     *
     *  \image html ksi-gfx-font-1.jpg
     */
    int         char_ascent;

    /** 
     * \brief The font descent
     *
     *  \image html ksi-gfx-font-1.jpg
     */
    int         char_descent;

    /** 
     * \brief The font height
     */
    int         char_height;

    /** 
     * \brief A reserved field 
     */
    int         reserved;
}
KJavaFont;

/** 
 * @ingroup LCD_Drawing
 */

/**
 * \brief The structure type for containing the infomation about the graphics environment of the target device.
 */
typedef struct _KJavaGfxInfoTag
{
    /** 
     * \brief The off-screen buffer handle, identifying a virtual screen.
     *
     * This value must be the handle indicating an off-screen buffer that 
     * can be used.
     */
    KJavaImage  *offscreen;
    
    /** 
     * \brief The clipping rectangle for Java application drawing.
     *
     * Java application cannot draw outside of this rectangle.
     */
    KJavaClip   viewRegion;
    
    /**
     * \brief The clipping rectangle for softkey drawing.
     */
    KJavaClip   softKeyRegion;
    
    /** 
     * \brief Whether or not color display is supported. 
     */
    int         isColor;
    
    /** 
     * \brief The number of colors. 
     */
    int         numColors;
    
    /** 
     * \brief The number of alpha channel levels supported. 
     */
    int         numAlphaLevels;
    
    /** 
     * \brief Whether or not the device has key repeat event 
     */
    int         hasRepeatEvents;
    
    /** 
     * \brief Whether or not the device has pointer event 
     */
    int         hasPointerEvents;
    
    /** 
     * \brief Whether or not the device has pointer motion event 
     */
    int         hasPointerMotionEvents;
    
    /** 
     * \brief Whether or not wordwrap is required 
     */
    int         wordwrap;
    
    /** 
     * \brief The number of softkeys 
     */
    int         numSoftkeys;
}
KJavaGfxInfo;

/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/

/**
 * \brief Initializes the native system graphics component.
 * 
 * The function needs to:
 *      - prepare the virtual screen for the device. (its type is KJavaImage)
 *      - prepare the system fonts' information.
 *      .
 *
 * This function is called in the KVE thread.
 */
void kjava_gfx_init(void);


/**
 * \brief Finalizes the native system graphics component..
 *
 * The function needs to release all the graphics system resources that are 
 * allocated after #kjava_gfx_init is called. 
 *
 * This function is called in the KVE thread.
 */
void kjava_gfx_fini(void);

/** 
 * @ingroup Font_Drawing
 */

/**
 * \brief Finds the suitable system font with given criteria 
 *        (size, face, and style).
 * @param size      
 *        The size for the criteria. One of the following is designated.
 *        - #KJAVA_FONT_SIZE_SMALL
 *        - #KJAVA_FONT_SIZE_MEDIUM
 *        - #KJAVA_FONT_SIZE_LARGE
 *        .
 * @param style     
 *        The font style. One of the following is designated.
 *        - #KJAVA_FONT_STYLE_PLAIN
 *        - #KJAVA_FONT_STYLE_BOLD
 *        - #KJAVA_FONT_STYLE_ITALIC
 *        - #KJAVA_FONT_STYLE_UNDERLINE
 *        .
 * @param face      
 *        The font face. One of the following is designated.
 *        - #KJAVA_FONT_FACE_MONOSPACE
 *        - #KJAVA_FONT_FACE_PROPORTIONALE
 *        .
 * @param defaultFont   
 *        The default font. One of the following is designated.
 *        - #KJAVA_FONT_TYPE_DEFAULT
 *        - #KJAVA_FONT_TYPE_STATIC_TEXT
 *        - #KJAVA_FONT_TYPE_INPUT_TEXT
 *        .
 * @return 
 *        Return a handle identifying the font that suits given criteria. 
 *        If the suitable font is not found, return the handle for the 
 *        default system font at least.
 *
 * Determine a font that suits given criteria and return its font handle. 
 *
 * If <i>size</i> == <i>style</i> == <i>face</i> == 0, use <i>defaultFont</i>
 * as the criteria; otherwise, use <i>size</i>, <i>style</i>, and <i>face</i>
 * as the criteria and ignore <i>defaultFont</i>.
 */
const KJavaFont* kjava_gfx_getSuitableFont(int size, int style, int face, int defaultFont);

/** 
 * @ingroup Font_Drawing
 */

/**
 * \brief Gets the width of given UCS2 string in the designated system font.
 * @param font
 *        The handle of the font to be used in the calculation.
 * @param str       
 *        A pointer to the UCS2 string for which the width should be calculated.
 * @param length    
 *        The number of characters in the UCS2 string indicated by <i>str</i>.
 * @return
 *        Return the width in pixels of the UCS2 string.
 */
int kjava_gfx_getCharsWidth (
        const KJavaFont *font, const KJavaUnicode *str, int length);

/** 
 * @ingroup Font_Drawing
 */

/**
 * \brief Gets the number of characters that fit in the designated width.
 * @param font      
 *        The handle of the font to be used in the calculation.
 * @param str       
 *        A pointer to the UCS2 string for which the number should be calculated.
 * @param length    
 *        The number of characters in the UCS2 string indicated by <i>str</i>.
 * @param width     
 *        The width.
 * @return 
 *        Return the number of characters that fits in the designated width. 
 *
 * If the string designated in the <i>str</i> parameter contains a character 
 * not defined in the platform, treat it as a single blank space.
 */
int kjava_gfx_getCountChars (
        const KJavaFont *font, const KJavaUnicode *str, int length, int width);

/** 
 * @ingroup Font_Drawing
 */

/**
 * \brief Draws a character string on a given offscreen buffer with 
 *        designated clipping rectangle, font, and pixel color.
 * @param offscreen 
 *        A pointer to the #KJavaImage structure holding the offscreen buffer information.
 * @param clip      
 *        A pointer to the the #KJavaClip strucnture holding the clipping range for drawing.
 * @param font      
 *        A pointer to the the #KJavaFont structure holding the font information.
 * @param string    
 *        A pointer to the UCS2 string to be drawn.
 * @param length    
 *        The number of characters in the string inidcated by <i>string</i>.
 * @param x         
 *        The x coordinate of the string drawing position in the offscreen buffer.
 * @param y         
 *        The y coordinate of the string drawing position in the offscreen buffer.
 * @param color     
 *        The color for drawing. It is designated in the platform pixel
 *        format (e.g. RGB 565, RGB 666, or RGB 888) specified in the 
 *        platform information matrix. 
 */
void kjava_gfx_drawChars (
        KJavaImage      *offscreen, 
        const KJavaClip *clip, 
        const KJavaFont *font, 
        const KJavaUnicode *string, int length, 
        int x, int y, 
        const KJavaPixel color);

/** 
 * @ingroup LCD_Drawing
 */

/**
 * \brief Switches modes of the virtual screen size.
 * @param screenMode      
 *        The screen mode. One of the following is designated:
 *        - #KJAVA_VIDEO_SCREEN_NORMAL
 *        - #KJAVA_VIDEO_SCREEN_FULL
 *        .
 * @return 
 *        Return 1 if the mode switch succeeds, or 0 if it fails.
 *        
 * When the mode switch succeeds, thereafter when setting device information 
 * for the graphics context to be obtained by #kjava_gfx_getInfo, be sure to 
 * reflect the mode change in the information returned to JBlendia.
 */
int kjava_gfx_requestScreenMode(int screenMode);

/** 
 * @ingroup LCD_Drawing
 */

/**
 * \brief Gets the video device properties.
 * @param info      
 *        A pointer to the #KJavaGfxInfo structure for putting the video device information.
 *
 * The information passed through <i>info</i> must reflect the virtual screen mode.
 */
void kjava_gfx_getInfo(KJavaGfxInfo *info);

/** 
 * @ingroup LCD_Drawing
 */

/**
 * \brief Gets the infomation of the virtual-screen. 
 * @return 
 *        Return a pointer to the #KJavaImage structure holding the virtual screen information.
 *        This pointer must remain valid until next time this funciton is called.
 *
 * The virtual screen is a graphics off-screen, with the same size as 
 * real screen.
 */
KJavaImage* kjava_gfx_getVirtualScreen(void);

/** 
 * @ingroup LCD_Drawing
 */

/**
 * \brief Starts drawing on the virtual screen 
 *
 * From the time this function is called and until 
 * #kjava_gfx_paintVirtualEnd is called, drawing takes place on the virtual screen. 
 */
void kjava_gfx_paintVirtualBegin(void);

/** 
 * @ingroup LCD_Drawing
 */

/**
 * \brief Ends drawing on the virtual screen.
 * @param flush     
 *        When this flag is 0, do not transfer the virtual screen 
 *        contents to the real screen.
 * @param clip      
 *        The clipping range for updating screen.
 *
 * Once this function completes its processing, treat drawing in the virtual 
 * screen as temporarily stopped and send the drawing result to the real screen. 
 */
void kjava_gfx_paintVirtualEnd(int flush,const KJavaClip* clip);

/** 
 * @ingroup LCD_Drawing
 */

/**
 * \brief Sets the type of the current Displayable displayed by the 
 *        Java application.
 * @param type      
 *        The displayable type. One of the following is designated:
 *        - #KJAVA_VIDEO_DISPLAYABLE_NULL
 *        - #KJAVA_VIDEO_DISPLAYABLE_SCREEN
 *        - #KJAVA_VIDEO_DISPLAYABLE_CANVAS
 *        - #KJAVA_VIDEO_DISPLAYABLE_PERMISSIONDIALOG
 *        .
 *
 * This function is called when a Java application has changed the 
 * current <code>Displayable</code> by calling 
 * <code>javax.microedition.lcdui.Display</code> class 
 * <code>setCurrent(Displayable)</code> method, or when 
 * JBlend displays a permission confirmation dialg. 
 *
 * If no use is made of the information designated in this function 
 * for drawing or other purposes, it may be implemented as an empty function. 
 */
void kjava_gfx_setDisplayableType(int type);

/** 
 * @ingroup Image_Codec
 */

/**
 *  \brief Determines whether or not the native system can decode a
 *         compressed image.
 * @param  srcBuf  
 *         A pointer to the buffer holding the compressed image of unknown format.
 * @param  srcSize 
 *         The size in bytes of the image data in the buffer indicated by <i>srcBuf</i>.
 * @param  width   
 *         A pointer to the buffer for putting the width of the image.
 * @param  height  
 *         A pointer to the buffer for putting the height of the image.
 * @return 
 *         Return #KJAVA_CODEC_SUCCESS if the image can be decoded by the natime system,  
 *         otherwise return #KJAVA_CODEC_UNSUPPORT.
 *
 * Check the image format of the image given by <i>srcBuf</i> and 
 * if the image can be decoded by the native system, store its
 * width and height in the buffers indicated by <i>width</i> and 
 * <i>height</i> and return #KJAVA_CODEC_SUCCESS.
 */
int kjava_gfx_imageAnalyze(const unsigned char* srcBuf, unsigned int srcSize, int* width, int* height);

/** 
 * @ingroup Image_Codec
 */

/**
 * \brief Decodes a compressed image.
 * @param srcBuf   
 *        A pointer to the buffer holding the compressed image to be decoded.
 * @param srcSize
 *        The size in bytes of the image data in the buffer indicated by <i>srcBuf</i>.
 * @param argbData
 *        A pointer to the memory area for putting the ARGB data of decoded source image
 * @param dataLength
 *        The size of the memory area for putting ARGB data. This value is width*height 
 *        that return from #kjava_gfx_imageAnalyze
 * @param processAlpha
 *        Indicate Alpha channel used or not if drawing this image to any offscreen
 *        0 - not used
 *        1 - used
 * @return 
 *        Return one of the following values.
 *        - #KJAVA_CODEC_SUCCESS  if the image was normally decoded.
 *        - #KJAVA_CODEC_UNSUPPORT if the native decoder can't decode the source data format.
 *        - #KJAVA_CODEC_BAD_FORMAT if the data contains bad format.
 *        - #KJAVA_CODEC_NO_RESOURCE if enough resource could not be allocated to decode this image.
 *        .
 *
 * Only the images checked by #kjava_gfx_imageAnalyze are passed to this function.
 */
int kjava_gfx_imageDecode( const unsigned char* srcBuf,
                           unsigned int srcSize, 
                           KJavaPixel* pixelData,
                           unsigned char* alphaData,
                           int dataLength,
                           int *processAlpha);

/** @} */

#ifdef __cplusplus
}
#endif 


#endif /* KJAVA_SYS_CODEC_HEADER */
