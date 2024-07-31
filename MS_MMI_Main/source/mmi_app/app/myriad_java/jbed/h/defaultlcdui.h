/*
 * Copyright 2001-2009 Myriad Group AG. All Rights Reserved.
 * $Id$
 */

/*
 * @(#)defaultLCDUI.h  1.41 01/06/19 @(#)
 * Copyright (c) 1999-2001 Sun Microsystems, Inc. All Rights Reserved.
 *
 * This software is the confidential and proprietary information of Sun
 * Microsystems, Inc. ("Confidential Information").  You shall not
 * disclose such Confidential Information and shall use it only in
 * accordance with the terms of the license agreement you entered into
 * with Sun.
 *
 * SUN MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
 * SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT. SUN SHALL NOT BE LIABLE FOR ANY DAMAGES
 * SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
 * THIS SOFTWARE OR ITS DERIVATIVES.
 */
#ifndef DEFAULTLCDUI_H_INCLUDED
#define DEFAULTLCDUI_H_INCLUDED

#include <jbed_jbni.h>
#include <jbed_legacy.h>
#include <javastatic.h>  /* krom-generated static final int values */
#include <targ_rbgraphics.h>
#include <midp_events.h>

#include <lcduikeys.h>

#define TRANSLATE(g, x, y)  ((x) += ((g)->transX), (y) += ((g)->transY))

#define LINESTYLE(g) (IBOOL)((g)->style)
#define SETLINESTYLE(g, newStyle) (g)->style = (jint)(newStyle)

#define PIXEL(g)     (IU32)((g)->pixel)
#define SETPIXEL(g, newPixel)   (g)->pixel = newPixel

#define ARGBCOLOR(g)    (IU32)((g)->argbColor)
#define SETARGBCOLOR(g, newArgbColor)        (g)->argbColor = newArgbColor

/**
 * Get a pointer to the current clipping region.
 * Point at the Graphics fields directly to save copying
 * (asserts below ensure the structure is correct).
 */
#define CLIP(g) ((IS32*)(&(g)->clipX1))
/* TODO: Switch to using a structure instead of and array of IS32 */

#define IMAGEDST(g) getImageData((g)->destination)

#define IMAGEDSTADDR(g) \
do { \
    (g)->bitmapAdr = (jint)IMAGEDST(g); \
} while (0)


#define DECLARE_FONT_PARAMS(font)            \
    int face  = font->face;  \
    int style = font->style; \
    int size  = font->size


/* reference point locations, from Graphics.java */
#define HCENTER   STATIC_javax_microedition_lcdui_Graphics_HCENTER
#define VCENTER   STATIC_javax_microedition_lcdui_Graphics_VCENTER
#define LEFT      STATIC_javax_microedition_lcdui_Graphics_LEFT
#define RIGHT     STATIC_javax_microedition_lcdui_Graphics_RIGHT
#define TOP       STATIC_javax_microedition_lcdui_Graphics_TOP
#define BOTTOM    STATIC_javax_microedition_lcdui_Graphics_BOTTOM
#define BASELINE  STATIC_javax_microedition_lcdui_Graphics_BASELINE

/* flags for font descriptors */
#define STYLE_PLAIN       STATIC_javax_microedition_lcdui_Font_STYLE_PLAIN
#define STYLE_BOLD        STATIC_javax_microedition_lcdui_Font_STYLE_BOLD
#define STYLE_ITALIC      STATIC_javax_microedition_lcdui_Font_STYLE_ITALIC
#define STYLE_UNDERLINED  STATIC_javax_microedition_lcdui_Font_STYLE_UNDERLINED

/* flags for line types */
#define SOLID   STATIC_javax_microedition_lcdui_Graphics_SOLID
#define DOTTED  STATIC_javax_microedition_lcdui_Graphics_DOTTED

#define SIZE_SMALL     STATIC_javax_microedition_lcdui_Font_SIZE_SMALL
#define SIZE_MEDIUM    STATIC_javax_microedition_lcdui_Font_SIZE_MEDIUM
#define SIZE_LARGE     STATIC_javax_microedition_lcdui_Font_SIZE_LARGE

#define FACE_SYSTEM       STATIC_javax_microedition_lcdui_Font_FACE_SYSTEM
#define FACE_MONOSPACE    STATIC_javax_microedition_lcdui_Font_FACE_MONOSPACE
#define FACE_PROPORTIONAL STATIC_javax_microedition_lcdui_Font_FACE_PROPORTIONAL

/* flags for LCDUIgetDisplayParams */
#define SUPPORTS_COLOR         1
#define SUPPORTS_POINTER       2
#define SUPPORTS_MOTION        4
#define SUPPORTS_REPEAT        8
#define SUPPORTS_DOUBLEBUFFER 16

/* The max number of characters in a command menu item
 * FIXME: should be in a target-specific header, but it's not
 * clear which one, and all targets currently use this value
 */
#define MAX_COMMAND_LENGTH    127


#define COMMAND_TYPE_SCREEN STATIC_javax_microedition_lcdui_Command_SCREEN
#define COMMAND_TYPE_BACK   STATIC_javax_microedition_lcdui_Command_BACK
#define COMMAND_TYPE_CANCEL STATIC_javax_microedition_lcdui_Command_CANCEL
#define COMMAND_TYPE_OK     STATIC_javax_microedition_lcdui_Command_OK
#define COMMAND_TYPE_HELP   STATIC_javax_microedition_lcdui_Command_HELP
#define COMMAND_TYPE_STOP   STATIC_javax_microedition_lcdui_Command_STOP
#define COMMAND_TYPE_EXIT   STATIC_javax_microedition_lcdui_Command_EXIT
#define COMMAND_TYPE_ITEM   STATIC_javax_microedition_lcdui_Command_ITEM

#define CANVAS_UP         STATIC_javax_microedition_lcdui_Canvas_UP
#define CANVAS_DOWN       STATIC_javax_microedition_lcdui_Canvas_DOWN
#define CANVAS_LEFT       STATIC_javax_microedition_lcdui_Canvas_LEFT
#define CANVAS_RIGHT      STATIC_javax_microedition_lcdui_Canvas_RIGHT
#define CANVAS_FIRE       STATIC_javax_microedition_lcdui_Canvas_FIRE
#define CANVAS_GAME_A     STATIC_javax_microedition_lcdui_Canvas_GAME_A
#define CANVAS_GAME_B     STATIC_javax_microedition_lcdui_Canvas_GAME_B
#define CANVAS_GAME_C     STATIC_javax_microedition_lcdui_Canvas_GAME_C
#define CANVAS_GAME_D     STATIC_javax_microedition_lcdui_Canvas_GAME_D

#ifdef SPRINT_SUPPORT
#define ExternalCanvas_EXTERNAL_A \
    STATIC_com_sprintpcs_lcdui_ExternalCanvas_EXTERNAL_A
#define ExternalCanvas_EXTERNAL_B \
    STATIC_com_sprintpcs_lcdui_ExternalCanvas_EXTERNAL_B
#define ExternalCanvas_EXTERNAL_C \
    STATIC_com_sprintpcs_lcdui_ExternalCanvas_EXTERNAL_C
#define ExternalCanvas_EXTERNAL_D \
    STATIC_com_sprintpcs_lcdui_ExternalCanvas_EXTERNAL_D
#endif

#ifdef EXTERNAL_DISPLAY
typedef struct INSTANCE_com_sun_midp_lcdui_MultiDeviceCaps_s
    *MULTIDEVICECAPS_INSTANCE;
#endif /* EXTERNAL_DISPLAY */
#define JBED_EXT_SCROLL_UP      STATIC_com_jbed_midp_MIDPToolkit_SCROLL_UP
#define JBED_EXT_SCROLL_DOWN    STATIC_com_jbed_midp_MIDPToolkit_SCROLL_DOWN
#define JBED_EXT_MOVE_LEFT      STATIC_com_jbed_midp_MIDPToolkit_MOVE_LEFT
#define JBED_EXT_MOVE_RIGHT     STATIC_com_jbed_midp_MIDPToolkit_MOVE_RIGHT
#define JBED_EXT_SELECT         STATIC_com_jbed_midp_MIDPToolkit_SELECT

/* command manager keycodes */
#define JBED_EXT_SOFT_1   STATIC_com_jbed_midp_MIDPToolkit_SOFT_1
#define JBED_EXT_SOFT_2   STATIC_com_jbed_midp_MIDPToolkit_SOFT_2
#define JBED_EXT_EXIT     STATIC_com_jbed_midp_MIDPToolkit_EXIT
#define JBED_EXT_CLEAR    STATIC_com_jbed_midp_MIDPToolkit_CLEAR
#define JBED_EXT_SEND     STATIC_com_jbed_midp_MIDPToolkit_SEND

#ifdef SPRINT_SUPPORT
#define JBED_EXT_SIDE_DOWN      STATIC_com_jbed_midp_MIDPToolkit_SIDE_DOWN
#define JBED_EXT_SIDE_UP        STATIC_com_jbed_midp_MIDPToolkit_SIDE_UP
#endif //SPRINT_SUPPORT


/* Transform definitions must match the definitions of the same name in
 * javax.microedition.lcdui.game.Sprite.java
 */
#define TRANS_NONE        \
        STATIC_javax_microedition_lcdui_game_Sprite_TRANS_NONE
#define TRANS_MIRROR_ROT180    \
        STATIC_javax_microedition_lcdui_game_Sprite_TRANS_MIRROR_ROT180
#define TRANS_MIRROR      \
        STATIC_javax_microedition_lcdui_game_Sprite_TRANS_MIRROR
#define TRANS_ROT180      \
        STATIC_javax_microedition_lcdui_game_Sprite_TRANS_ROT180
#define TRANS_MIRROR_ROT270    \
        STATIC_javax_microedition_lcdui_game_Sprite_TRANS_MIRROR_ROT270
#define TRANS_ROT90        \
        STATIC_javax_microedition_lcdui_game_Sprite_TRANS_ROT90
#define TRANS_ROT270      \
        STATIC_javax_microedition_lcdui_game_Sprite_TRANS_ROT270
#define TRANS_MIRROR_ROT90    \
        STATIC_javax_microedition_lcdui_game_Sprite_TRANS_MIRROR_ROT90

#define TRANSFORM_FLIP_Y       TRANS_MIRROR_ROT180
#define TRANSFORM_FLIP_X       TRANS_MIRROR
#define TRANSFORM_INVERT_AXES  TRANS_MIRROR_ROT270

#define FONTPARAMS face, style, size
#define FONTPARAMS_PROTO int face, int style, int size

/* Display the labels on the soft buttons */
extern void LCDUIupdateButtons(void);

/* translate rgb or grayscale to device-dependent pixel value */
extern IU32 LCDUIgetPixel(IU32 rgb, IU32 gray, IBOOL isGray);

/*
 * Return the key code corresponding to the given abstract game action.
 */
extern IS32  LCDUIgetKeyCode(IS32);

/*
 * Return the abstract game action corresponding to the given key code.
 */
extern IS32  LCDUIgetGameAction(IS32);

/*
 * Return the key string to the given key code.
 */
extern char *LCDUIgetKeyName(IS32);

#if defined(JVM_FDAC) || defined(JVM_FBCC)
#include <javainstance.h>
#ifdef MIDP
typedef struct INSTANCE_javax_microedition_lcdui_Graphics_s LCDUI_Context;
typedef struct INSTANCE_javax_microedition_lcdui_Graphics_s * LCDUI_ContextPtr;
#else /* For CDCA PBP graphics */
typedef struct INSTANCE_java_awt_Graphics_s LCDUI_Context;
typedef struct INSTANCE_java_awt_Graphics_s * LCDUI_ContextPtr;
#endif
#else /* non JVM build */
typedef struct {
    int32_t clipX1; /* the clip area */
    int32_t clipY1;
    int32_t clipX2;
    int32_t clipY2;
    uint32_t argbColor; /* the argb color with the format 0xAARRGGBB*/
    uint32_t pixel; /* the native pixel to be written into the vram */
    bool_t style; /* the stytle of the line */
    int32_t bitmapAdr; /* the address of the destination bitmap */
}LCDUI_Context, *LCDUI_ContextPtr;
#endif

/*
 * Draw a line between two points (x1,y1) and (x2,y2).
 */
extern void LCDUIdrawLine(LCDUI_ContextPtr lcduiCtxtPtr, int x1, int y1,
                          int x2, int y2);

/*
 * Draw a rectangle at (x,y) with the given width and height.
 */
extern void LCDUIdrawRect(LCDUI_ContextPtr lcduiCtxtPtr, int x, int y,
                          int width, int height);

/*
 * Fill a rectangle at (x,y) with the given width and height.
 */
extern void LCDUIfillRect(LCDUI_ContextPtr lcduiCtxtPtr, int x, int y,
                          int width, int height);


/*
 * Draw a rectangle at (x,y) with the given width and height. arcWidth and
 * arcHeight, if nonzero, indicate how much of the corners to round off.
 */
extern void LCDUIdrawRoundRect(LCDUI_ContextPtr lcduiCtxtPtr,
                               int x, int y, int width, int height,
                               int arcWidth, int arcHeight);

/*
 * Fill a rectangle at (x,y) with the given width and height. arcWidth and
 * arcHeight, if nonzero, indicate how much of the corners to round off.
 */
extern void LCDUIfillRoundRect(LCDUI_ContextPtr lcduiCtxtPtr,
                               int x, int y, int width, int height,
                               int arcWidth, int arcHeight);

/*
 * Draw an elliptical arc centered in the given rectangle.  If filled is
 * true, fill the contents of the arc; otherwise just stroke it.  The
 * portion of the arc to be drawn starts at startAngle (with 0 at the
 * 3 o'clock position) and proceeds counterclockwise by <arcAngle>
 * degrees.  arcAngle may not be negative.
 */
extern void LCDUIdrawArc(LCDUI_ContextPtr lcduiCtxtPtr,
                         int x, int y, int width, int height,
                         int startAngle, int arcAngle);

/*
 * Fill an elliptical arc centered in the given rectangle.  The
 * portion of the arc to be drawn starts at startAngle (with 0 at the
 * 3 o'clock position) and proceeds counterclockwise by <arcAngle>
 * degrees.  arcAngle may not be negative.
 */
extern void LCDUIfillArc(LCDUI_ContextPtr lcduiCtxtPtr,
                         int x, int y, int width, int height,
                         int startAngle, int arcAngle);

/*
 * Fill a triangle defined by the given three points (x1,y1) (x2,y2)
 * and (x3,y3) with the current color.
 */
extern  void LCDUIfillTriangle(LCDUI_ContextPtr lcduiCtxtPtr,
                               int x1, int y1, int x2, int y2, int x3, int y3);


/*
 * Draw the first n characters in chararray, with the anchor point of the
 * entire (sub)string located at x, y.
 */
extern void LCDUIdrawChars(LCDUI_ContextPtr lcduiCtxtPtr,
                           FONTPARAMS_PROTO,
                           int x, int y, int anchor,
                           const jchar *chararray, int n);

/*
 * Get alpha value array pointer
 */
extern IU8* LCDUIgetImageAlphaValues(void* img, void* imgObj);

/*
 * Draw the given image data, locating its anchor point at x, y
 */
extern void LCDUIdrawImage(LCDUI_ContextPtr lcduiCtxtPtr,
                           int x, int y, int anchor, void *img, IU8* srcAlphas);

/* Draw the given part of image data, locating anchor point at x,y and
 * transforming as indicated.
 */

extern void LCDUIdrawRegion(LCDUI_ContextPtr lcduiCtxtPtr, void *img,
                            int x, int y, int anchor,
                            int x_src, int y_src,
                            int width, int height, int transform,
                            IBOOL useAlpha, IBOOL copyAlpha,
                            IU8* srcAlpha, IU8* dstAlpha);

/*
 * Copy the specified region of the given image data to a new
 * destination, locating its anchor point at x, y.
*/
extern void LCDUIcopyArea(LCDUI_ContextPtr lcduiCtxtPtr,
                          int x_src, int y_src,int width, int height,
                          int x, int y, int anchor);

#ifdef EXTERNAL_DISPLAY
/*
 * Refresh the given area.  For double buffering purposes.
 */
extern void LCDUIrefresh(int scrn, int x, int y, int w, int h);
#else /* EXTERNAL_DISPLAY */
/*
 * Refresh the given area.  For double buffering purposes.
 */
extern void LCDUIrefresh(int x, int y, int w, int h);
#endif /* EXTERNAL_DISPLAY */

/*
 * Get the target font info by FONTPARAMS.
 */
extern GUI_TargFont LCDUIgetTargFont(FONTPARAMS_PROTO);

/*
 * Get the ascent, descent and leading info for the font indicated
 * by FONTPARAMS.
 */
extern void LCDUIgetFontInfo(FONTPARAMS_PROTO,
                             int *ascent, int *descent, int *leading);

/*
 * Get the advance width for the first n characters in charArray if
 * they were to be drawn in the font indicated by FONTPARAMS.
 */
extern int  LCDUIcharsWidth(FONTPARAMS_PROTO, const jchar *charArray,
                            int n, int *advWidth);

#ifdef SPRINT_SUPPORT
/*
 * Get the width and height of the external display
 */
extern IBOOL LCDUIgetExternalDisplayParams(IU32 *extWidth, IU32 * extHeight);

/*
 * Refresh the given area in the external screen.
 */
extern void LCDUIrefreshExternal(int x1, int y1, int x2, int y2);
#endif
/*
 * Get parameters describing the window system's capabilities
 */
extern void LCDUIgetDisplayParams(
#ifdef EXTERNAL_DISPLAY
                                  int screen,
#endif /* EXTERNAL_DISPLAY */
                                  IU32 *width, IU32 *height,
#ifdef MIDP2
                                  IU32 *fullscreenWidth, IU32 *fullscreenHeight,
#endif
                                  IU32 *eraseColor, IU32 *depth, IU32 *flags);

#ifdef USE_NATIVE_STATUS_BAR

/*
 * switch the screen to full screen or not
 * param : fullscreen, indicate whether the new mode is fullscreen,
 *                     1 means fullscreen mode
 *                     0 means normal mode
 */
extern void LCDUISetFullScreenMode(int fullScreen);
#endif /*USE_NATIVE_STATUS_BAR*/

/*
 * set input mode
 */
extern void LCDUIsetInputMode(int mode);

/*
 * destroy native image resources; called by cleanup handler
 */
extern void LCDUIdestroyNativeImage(void* imagePtr);

/*
 * Given an RGB pixel (color) in 0x00RRGGBB format, return the
 * actual color displayed in 0x00RRGGBB format.
 */
extern IU32 LCDUIgetDisplayColor(IU32 color);

/*
 * Fill rgb buffer with pixels (in 0xAARRGGBB format) from an image.
 */
extern void LCDUIgetRGB(ARRAY pRGBData, int offset, int scanLength,
                        int x, int y, int width, int height,
                        void *img, void* imgObj);

/*
 * render an rgb buffer pixels (in 0xAARRGGBB format) onto an image.
 */
extern void LCDUIdrawRGB(LCDUI_ContextPtr lcduiCtxtPtr, ARRAY pRGBData,
                         int offset, int scanlen,
                         int x, int y, int width, int height,
                         IBOOL processAlpha);

#ifdef MIDP2
/*
 * set the screen mode and rotation if supported
 */
extern void LCDUISetScreenMode(IS32 mode, IS32 rotation);

#ifdef ORIENTATION
/*
 * get the screen size for a given screen mode and orientation
 */
extern void LCDUIGetScreenSize(IS32 screenMode, IS32 rotation,
                               IS32 *width, IS32 *height);
#endif
#endif

/*
 * Private initialization and finalization for windows/graphics system
 */
extern void InitializeWindowSystem(void);
extern void FinalizeWindowSystem(void);

#ifdef MIDP2
/*
 * Private method to set the screen mode for commands and menus.
 */
extern void setCommandsFullScreenMode(IBOOL mode);

/*
 * Private method to set the screen mode.
 */
extern void setScreenMode(IS32 mode, IS32 rotation);

/*
 * Private method to restore the screen mode.
 */
extern void restoreScreenMode(void);
#endif /* MIDP2 */

typedef struct INSTANCE_com_sun_midp_lcdui_DeviceCaps_s  *DEVICECAPS_INSTANCE;
typedef struct INSTANCE_javax_microedition_lcdui_Image_s *IMAGE_INSTANCE;

extern void * getImageData(IMAGE_INSTANCE img);

#ifdef ADJUSTABLE_LOOK_AND_FEEL
extern void targ_getAdjUIConstants(DEVICECAPS_INSTANCE dcaps);
extern IBOOL targ_loadAdjUiImage(IMAGE_INSTANCE *img, char *name);
#endif

#ifdef SCREEN_ROTATION_SUPPORT
/**
 * Switch the native screen buffer.
 */
extern void LCDUIswitchScreenBuffer(jint rotationState);
#endif

#endif /* DEFAULTLCDUI_H_INCLUDED */
