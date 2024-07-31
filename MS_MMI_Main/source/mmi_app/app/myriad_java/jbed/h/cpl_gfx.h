/*
 * Copyright 1998-2007 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Framebuffer graphics support (VM-independent)
 *
 * This interface provides framebuffer graphics support for the Jbed VM.
 *
 * <!-- #interface list begin -->
 * \section cpl_gfx_interface Interface
 * - Case Native framebuffer for graphics:
 *    - CPL_gfx_startup()
 *    - CPL_gfx_shutdown()
 *    - CPL_gfx_getFrameBuffer()
 *    - CPL_gfx_update()
 *
 * - Case PBP. The following functions MAY be stubs if the described
 * functionality is not supported:
 *    - CPL_gfx_setCursor()
 *    - CPL_gfx_beep()
 *    - CPL_gfx_setTitle()
 * <!-- #interface list end -->
 *
 */

#ifndef __JBED_CPL_GFX_H__
#define __JBED_CPL_GFX_H__

#include <jbed_global.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
  * @name Return values for the CPL_gfx functions.
  *
  * Error values are negative, success values are zero or positive.
  * This allows to do simply:
  * <pre>
  * if(retVal < 0) Trace("An error happened %d", retVal);
  * </pre>
  */

/*@{*/

/**
 * OK code returned by CPL_gfx functions.
 */
#define CPL_GFX_OK 0

/**
 * Failure code returned by CPL_gfx functions.
 */
#define CPL_GFX_ERROR -1

/*@}*/


/**
 * Initialize the gfx module.
 *
 * Called when the application starts up.
 *
 * @return error code
 */

int32_t CPL_gfx_startup(void);

/**
 * Shutdown the gfx module.
 * Free framebuffer resources.
 *
 * Called when the application shuts down.
 *
 * @return error code
 */

int32_t CPL_gfx_shutdown(void);

#ifdef EXTERNAL_DISPLAY
/**
 * Get the number of displays.
 *
 * Called once per VM startup to find out how many display screens are on
 * the device.
 * @return the number of displays
 */
int32_t CPL_gfx_numDisplays();

/**
 * Initialize framebuffer.
 *
 * Called once per VM startup to prepare the displays and obtain information
 * about the framebuffers.
 *
 * The framebuffer is organized so that the pixel in the upper left corner is
 * at the lowest address, and the pixel to the right of it is at the next
 * higher pixel address.
 *
 * @param scrn is the screen number for which a display buffer is required
 * @param addr returns the address of the framebuffer.
 * @param width returns the width in pixels.
 * @param height returns the height in pixels.
 * @param stride returns the byte offset in the framebuffer between two
 * vertically contiguous pixels in the display.
 *
 * @return error code
 */
int32_t CPL_gfx_getFrameBuffer(int32_t scrn,
                            void **addr,
                            int32_t *width,
                            int32_t *height,
                            int32_t *stride);

/**
 * Update the display using the framebuffer contents.
 *
 * Called whenever the VM has finished modifying the framebuffer to update a
 * region of the display from the framebuffer.  The display update must be
 * synchronous with this function call, i.e., when this function returns the
 * update must have been completed.
 *
 * @param scrn the screen number
 * @param xs the x coordinate of the leftmost pixel
 * of the update region (from left).
 * @param ys the y coordinate of the topmost pixel
 * of the update region (from top).
 * @param xe the x coordinate of the rightmost pixel + 1
 * of the update region.
 * @param ye the y coordinate of the bottommost pixel + 1
 * of the update region.
 *
 * @return error code
 */

int32_t CPL_gfx_update(int32_t scrn,
                       int32_t xs,
                       int32_t ys,
                       int32_t xe,
                       int32_t ye);
#else /* EXTERNAL_DISPLAY */
/**
 * Initialize framebuffer.
 *
 * Called once per VM startup to prepare the display and obtain information
 * about the framebuffer.
 *
 * The framebuffer is organized so that the pixel in the upper left corner is
 * at the lowest address, and the pixel to the right of it is at the next
 * higher pixel address.
 *
 * @param addr returns the address of the framebuffer.
 * @param width returns the width in pixels.
 * @param height returns the height in pixels.
 * @param stride returns the byte offset in the framebuffer between two
 * vertically contiguous pixels in the display.
 *
 * @return error code
 */
int32_t CPL_gfx_getFrameBuffer(void **addr,
                            int32_t *width,
                            int32_t *height,
                            int32_t *stride);
/**
 * Update the display using the framebuffer contents.
 *
 * Called whenever the VM has finished modifying the framebuffer to update a
 * region of the display from the framebuffer.  The display update must be
 * synchronous with this function call, i.e., when this function returns the
 * update must have been completed.
 *
 * @param xs the x coordinate of the leftmost pixel
 * of the update region (from left).
 * @param ys the y coordinate of the topmost pixel
 * of the update region (from top).
 * @param xe the x coordinate of the rightmost pixel + 1
 * of the update region.
 * @param ye the y coordinate of the bottommost pixel + 1
 * of the update region.
 *
 * @return error code
 */

int32_t CPL_gfx_update(int32_t xs,
                       int32_t ys,
                       int32_t xe,
                       int32_t ye);
#endif /* EXTERNAL_DISPLAY */

#ifdef BASIS_PROFILE
/**
  * @name Cursor types for the CPL_gfx_setCursor function.
  *
  * The values match the corresponding constants in java.awt.Cursor.
  */

/*@{*/

/**
 * Default cursor.
 */
#define CPL_GFX_DEFAULT_CURSOR 0

/**
 * Crosshair cursor.
 */
#define CPL_GFX_CROSSHAIR_CURSOR 1

/**
 * Text beam cursor.
 */
#define CPL_GFX_TEXT_CURSOR 2

/**
 * Wait cursor.
 */
#define CPL_GFX_WAIT_CURSOR 3

/**
 * South-west resize cursor.
 */
#define CPL_GFX_SW_RESIZE_CURSOR 4

/**
 * South-east resize cursor.
 */
#define CPL_GFX_SE_RESIZE_CURSOR 5

/**
 * North-west resize cursor.
 */
#define CPL_GFX_NW_RESIZE_CURSOR 6

/**
 * North-east resize cursor.
 */
#define CPL_GFX_NE_RESIZE_CURSOR 7

/**
 * North resize cursor.
 */
#define CPL_GFX_N_RESIZE_CURSOR 8

/**
 * South resize cursor.
 */
#define CPL_GFX_S_RESIZE_CURSOR 9

/**
 * West resize cursor.
 */
#define CPL_GFX_W_RESIZE_CURSOR 10

/**
 * East resize cursor.
 */
#define CPL_GFX_E_RESIZE_CURSOR 11

/**
 * Hand cursor.
 */
#define CPL_GFX_HAND_CURSOR 12

/**
 * Move cursor.
 */
#define CPL_GFX_MOVE_CURSOR 13

/*@}*/

/**
 * Set the mouse cursor.
 *
 * If mouse cursors are implemented, this function will return CPL_GFX_OK
 * and cause the displayed mouse cursor to change immediately or in
 * response to mouse movement.
 * The system property java.awt.Component.setCursor.isRestricted should be
 * undefined (the default) or set to false.
 *
 * If mouse cursors are not implemented, this function will return CPL_GFX_ERROR.
 * The system property java.awt.Component.setCursor.isRestricted should be set
 * to true.
 *
 * @param type cursor type.
 * @return error code
 */

int32_t CPL_gfx_setCursor(int32_t type);

/**
 * Make beep sound.
 */

void CPL_gfx_beep(void);

/**
 * Set window title.
 *
 * @param title the title to set (UTF8)
 */

void CPL_gfx_setTitle(char *title);

#endif /* BASIS_PROFILE */

#if defined(__cplusplus)
}
#endif

#endif /* __JBED_CPL_GFX_H__ */
