/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_GDI_H
#define OPDEV_GDI_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _OD_GdiContext
{
	/* Screen buffer to paint opera UI */
	char* screen_buffer;
	/* The screen layer to paint opera UI, some platform GDI functions need a layer to paint */
	OD_GDIHandle screen_layer;
	/* Text buffer to temporarily paint text */
	char* text_buffer;
	/* The layer to temporarily paint text, some platform GDI functions need a layer to paint */
	OD_GDIHandle text_layer;

	/* The buffer to paint status bar, only used in some platforms whose status bar need external buffer */
	char* statusbar_buffer;
	/* The previous and current status bar layer */
	OD_GDIHandle statusbar_layer;
	OD_GDIHandle prev_statusbar_layer;
	/* Key color of status bar layer */
	int statusbar_key_color;

	/* Viewport of opera, x, y, w, h */
	int vp_x, vp_y;
	int vp_w, vp_h;

	/* If we are in fullscreen mode now. In fullscreen mode, status bar don't show.*/
	OD_BOOL fullScreen;
} OD_GdiContext;

/* The angle of screen rotation */
typedef enum _OD_RotationAngle
{
	OD_ANGLE_0,
	OD_ANGLE_90,
	OD_ANGLE_180,
	OD_ANGLE_270
} OD_RotationAngle;

/* The screen mode*/
typedef enum _OD_ScreenMode
{
	OD_LANDSCAPE,
	OD_PORTRAIT
} OD_ScreenMode;

/* The pixel format */
typedef enum _OD_PixelFormat
{
	OD_RGB565,
	OD_RGB888,
	OD_PARGB8888
} OD_PixelFormat;

/* The layer type, opera have many layers in some platforms */
typedef enum _OD_LayerType
{
	OD_LAYER_OPERA,
	OD_LAYER_STATUS_BAR,
	OD_LAYER_TEXT,
	OD_LAYER_VIDEO
} OD_LayerType;


/************************************************************************/
/* GDI Layer                                                            */
/************************************************************************/
/**
* Init GDI context.
*/
OD_MMI_TASK OD_STATUS OD_GDIInit(OD_Context *context);

/**
* Deinit GDI context.
*/
OD_MMI_TASK OD_STATUS OD_GDIDeinit(OD_Context *context);

/**
 * Check if platform must use native memory to create gdi buffer.
 *
 * @return OD_TRUE if must use native memory.
 * @note On some platforms, gdi buffer must be created with non-cachable memory
 */
OD_MMI_TASK OD_BOOL OD_GDIUseNativeMemory(OD_Context *context);

/**
 * Allocate memory for gdi layer.
 *
 * @param[in] size size of the memory.
 * @param[out] buffer pointer to the bitmap buffer to be allocated.
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_GDIAllocMemory(OD_Context *context, int size, char** pBuffer);

/**
* Free GDI memory.
*
* @param[in] pointer of the buffer.
* @return OD_SUCCESS if the operation is successful, or the appropriate
*         error code from the OD_Error enumeration.
*/
OD_MMI_TASK void OD_GDIFreeMemory(OD_Context *context, char* pBuffer);

/**
 * Create a GDI layer for opera to paint on. usually called when entering opera.
 *
 * @param[in] format pixel format of the layer.
 * @param[in] width width of the layer.
 * @param[in] height height of the layer.
 * @param[in] buffer bitmap buffer associated with the layer, can be NULL.
 * @param[out] handle handle of the layer.
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 *         if OD_NOT_SUPPORTED is returned, try call it again with buffer
 *         or NULL buffer.
 */
OD_MMI_TASK OD_STATUS OD_GDICreateLayer(OD_Context *context, OD_PixelFormat format,
										int width, int height,
										char *buffer, OD_GDIHandle *handle);

/**
 * Destroy a GDI layer. usually called when exiting opera.
 *
 * @param[in] handle handle of the layer to be destroyed.
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_GDIDestroyLayer(OD_Context *context, OD_GDIHandle handle);

/************************************************************************/
/* GDI Util                                                             */
/************************************************************************/

/**
 * Called when opera is switched to foreground.
 *
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_ShowOpera(OD_Context *context);

/**
 * Called when opera is switched to background and won't update the screen.
 *
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_HideOpera(OD_Context *context);

/**
 * Invalidate screen and force repaint.
 *
 * @param[in] x left of invalid area.
 * @param[in] y top of invalid area.
 * @param[in] w width of invalid area.
 * @param[in] h height of invalid area.
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_Paint(OD_Context *context, int x, int y, int w, int h);

/**
 * Obtain pointer to the device LCD screen buffer.
 *
 * @param[out] buffer pointer to the LCD screen buffer.
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_GetScreenBuffer(OD_Context *context, char** buffer);

/**
 * Obtain client area for opera to draw on.
 *
 * @param[out] x left of the area.
 * @param[out] y top of the area.
 * @param[out] w width of the area.
 * @param[out] h height of the area.
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_GetViewportArea(OD_Context *context, int *x, int *y, int *w, int *h);

/**
 * Obtain dimension of the device LCD screen.
 *
 * @param[in] type indicate which layer to operate.
 * @param[out] width width of the LCD screen.
 * @param[out] height height of the LCD screen.
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_GetLayerDimension(OD_LayerType type, int *width, int *height);


/**
 * Obtain pixel format of a layer.
 *
 * @param[in] type indicate which layer to operate.
 * @return pixel format.
 */
OD_MMI_TASK OD_PixelFormat OD_GetLayerPixelFormat(OD_LayerType type);

/**
 * Obtain BPP(bytes per pixel) of the device LCD screen buffer.
 *
 * @return bpp value.
 */
OD_MMI_TASK int OD_GetLayerBPP(OD_PixelFormat format);

/**
 * Obtain DPI(dots per inch) of the device LCD screen buffer.
 *
 * @return dpi value.
 */
OD_MMI_TASK int OD_GetScreenDPI(OD_Context *context);

/**
 * Obtain current direction of screen.
 *
 * @return current angle.
 */
OD_MMI_TASK OD_RotationAngle OD_GetRotationAngle(OD_Context *context);

/**
 * Set direction of screen.
 *
 * @param[in] angle the angle to set.
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_RotateScreen(OD_Context *context, OD_RotationAngle angle);

/**
 * Obtain current screen mode (landscape or portrait).
 *
 * @return current screen mode.
 */
OD_MMI_TASK OD_ScreenMode OD_GetScreenMode(OD_Context *context);

/**
 * Set screen size (fullscreen or not).
 *
 * @param[in] bFullScreen screen size to set.
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_ToggleFullScreen(OD_Context *context, OD_BOOL bFullScreen);

#ifdef __cplusplus
};
#endif

#endif /* OPDEV_GDI_H */
