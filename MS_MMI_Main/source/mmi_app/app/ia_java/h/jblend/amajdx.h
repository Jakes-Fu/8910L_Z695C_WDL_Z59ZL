/*
 *   Copyright 2009 Aplix Corporation. All rights reserved.
 */

/*
 * AJDX is a platform API independent module, focusing on 2D grahpics
 * rendering. Operations of AJDX APIs are all done on given memory
 * area and no memory allocation/release will take place inside AJDX.
 * In fact, only three C standard functions may be called by AJDX, memcpy,
 * memcmp and memset.
 *
 * AJDX must always keep such platform independency and thus being able
 * to present as a standalone library.
 */

#ifndef ___AMAJDX_H
#define ___AMAJDX_H

#include "jktypes.h"
#include "jktypesajdx.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Make the specified image region be copied unchanged.
 */
#define AJDX_TRANS_NONE                     0

/*
 * Make the image region to be reflected about its vertical center
 * and then rotated clockwise by 180 degrees before drawing.
 */
#define AJDX_TRANS_MIRROR_ROT180            1

/*
 * Make the image region to be reflected about its
 * vertical center before drawing.
 */
#define AJDX_TRANS_MIRROR                   2

/*
 * Make the image region be rotated clockwise by 180 degrees before drawing.
 */
#define AJDX_TRANS_ROT180                   3

/*
 * Make the image region to be reflected about its vertical center
 * and then rotated clockwise by 270 degrees before drawing.
 */
#define AJDX_TRANS_MIRROR_ROT270            4

/*
 * Make the image region be rotated clockwise by 90 degrees before drawing.
 */
#define AJDX_TRANS_ROT90                    5

/* 
 * Make the image region be rotated clockwise by 270 degrees before drawing.
 */
#define AJDX_TRANS_ROT270                   6

/*
 * Make the image region to be reflected about its vertical center
 * and then rotated clockwise by 90 degrees before drawing.
 */
#define AJDX_TRANS_MIRROR_ROT90             7



/**
 *  Draw the source image to the specified clip area of destination image,
 *  taking (x, y) as anchor point.
 *
 *  If the source image contains transparent pixels, the corresponding pixels 
 *  in the destination image must be left untouched. If the source image contains 
 *  partially transparent pixels, a compositing operation must be performed with 
 *  the destination pixels, leaving all pixels of the destination image fully 
 *  opaque.
 *
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  x              The x-coordinate of destination image for drawing.
 *
 *  @param  y              The y-coordinate of destination image for drawing.
 *
 *  @param  src            The specified image to be drawn, must not be null.
 *                         The source image and destination image could be
 *                         the same, but must never be partially overlapped.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxDrawImage (
                AJDX_IMAGE          *dest,
                const AJDX_CLIP     *clip,
                JKSint              x,
                JKSint              y,
                const AJDX_IMAGE    *src);

/**
 *  Copies a region of the specified source image to a location within 
 *  the destination, possibly transforming (rotating and reflecting) the 
 *  image data using the chosen transformation function.
 *
 *  The transformation function used must be one of AJDX_TRANS_XXXXX.
 *
 *  If the source region contains transparent pixels, the corresponding
 *  pixels in the destination region must be left untouched. If the source
 *  region contains partially transparent pixels, a compositing operation
 *  must be performed with the destination pixels, leaving all pixels of
 *  the destination region fully opaque.
 *
 *  The (x_src, y_src) coordinates are relative to the upper left corner of the 
 *  source image. The x_src, y_src, width, and height parameters specify a
 *  rectangular region of the source image. It is illegal for this region to
 *  extend beyond the bounds of the source image.
 *
 *  The (x_dest, y_dest) coordinates are relative to the coordinate system of 
 *  destination image. It is legal for the destination area to extend beyond 
 *  the bounds of the destinaion image. Pixels outside the bounds of the 
 *  destinaion image will not be drawn.
 *
 *  The transformation is applied to the image data from the region of the
 *  source image, and the result is rendered with location (x_dest, y_dest)
 *  in the destination image.
 *
 *  The source region and destination region must not overlap.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  x_dest         The x-coordinate of the anchor point in the 
 *                         destination drawing area.
 *
 *  @param  y_dest         the y-coordinate of the anchor point in the 
 *                         destination drawing area
 *
 *  @param  src            The source image to copy from, must not be null.
 *
 *  @param  x_src          The x-coordinate of the upper left corner of the 
 *                         region within the source image to copy.
 *
 *  @param  y_src          The y-coordinate of the upper left corner of the 
 *                         region within the source image to copy.
 *
 *  @param  w_src          Width of the region to copy.
 *
 *  @param  h_src          Height of the region to copy.
 *
 *  @param  transform      The desired transformation for the selected 
 *                         region being copied.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxDrawImageRegion (
                AJDX_IMAGE          *dest,
                const AJDX_CLIP     *clip,
                JKSint              x_dest,
                JKSint              y_dest,
                const AJDX_IMAGE    *src,
                JKSint              x_src,
                JKSint              y_src,
                JKSint              w_src,
                JKSint              h_src,
                JKSint              transform);

/**
 *  Copies a region of the specified source 32-bits ARGB image to a location 
 *  within the destination, possibly transforming (rotating and reflecting) the 
 *  image data using the chosen transformation function.
 *
 *  The transformation function used must be one of AJDX_TRANS_XXXXX.
 *
 *  If the source region contains transparent pixels, the corresponding
 *  pixels in  the destination region must be left untouched. If the source
 *  region contains partially transparent pixels, a compositing operation
 *  must be performed with the destination pixels, leaving all pixels of the
 *  destination region fully opaque.
 *
 *  The (x_src, y_src) coordinates are relative to the upper left corner of the
 *  source image. The x_src, y_src, w_src, and h_src parameters specify a
 *  rectangular region of the source image. It is illegal for this region to
 *  extend beyond the bounds of the source image.
 *
 *  The (x_dest, y_dest) coordinates are relative to the coordinate system of 
 *  destination image. It is legal for the destination area to extend beyond 
 *  the bounds of the destinaion image. Pixels outside of the bounds of the 
 *  destinaion image will not be drawn.
 *
 *  The transformation is applied to the image data from the region of the
 *  source  image, and the result is rendered with location (x_dest, y_dest)
 *  in the destination.
 *
 *  The source region and destination region must not overlap.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  x_dest         The x coordinate of the anchor point in the 
 *                         destination drawing area.
 *
 *  @param  y_dest         the y coordinate of the anchor point in the 
 *                         destination drawing area
 *
 *  @param  src            The source 32-bits ARGB image to copy from,
 *                         must not be null.
 *
 *  @param  x_src          The x coordinate of the upper left corner of the 
 *                         region within the source image to copy.
 *
 *  @param  y_src          The y coordinate of the upper left corner of the 
 *                         region within the source image to copy.
 *
 *  @param  w_src          The width of the region to copy.
 *
 *  @param  h_src          The height of the region to copy.
 *
 *  @param  transform      The desired transformation for the selected 
 *                         region being copied.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxDrawARGBImageRegion (
                AJDX_IMAGE              *dest,
                const AJDX_CLIP         *clip,
                JKSint                  x_dest,
                JKSint                  y_dest,
                const AJDX_ARGB_IMAGE   *src,
                JKSint                  x_src,
                JKSint                  y_src,
                JKSint                  w_src,
                JKSint                  h_src,
                JKSint                  transform);

/**
 *  Stores the designated image area in ARGB format in the designated array.
 *
 *
 *  @param  dest           Buffer to store the ARGB-format data, must not
 *                         be null. ARGB data will be stored in dest
 *                         in the order starting from beginning.
 *
 *  @param  scanlen_dest   The number of elements per line in the
 *                         data-storing array, must not be smaller than w_src.
 *
 *  @param  src            The source image to extract, must not be null.
 *
 *  @param  x_src          The x coordinate of the upper left corner of the 
 *                         region within the source image.
 *
 *  @param  y_src          The y coordinate of the upper left corner of the 
 *                         region within the source image.
 *
 *  @param  w_src          Width of the image region to be extracted.
 *
 *  @param  h_src          Height of the image region to be extracted.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxGetARGBPixelArray(
                JKUint32            *dest, 
                JKSint              scanlen_dest, 
                const AJDX_IMAGE    *src, 
                JKSint              x_src,
                JKSint              y_src,
                JKSint              w_src,
                JKSint              h_src);

/**
 *  Draw an image using ARGB data.
 *
 *  @param  dest           The destination image, must not be null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly. This function will then
 *                         fills dest->data, omitting dest->isMutable.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  x_dest         The x coordinate of the anchor point in the 
 *                         destination drawing area.
 *
 *  @param  y_dest         the y coordinate of the anchor point in the 
 *                         destination drawing area
 *
 *  @param  src            An array storing the ARGB-format data to be drawn,
 *                         must not be null.
 *
 *  @param  w_src          Width of the image identified by src data.
 *
 *  @param  h_src          Height of the image identified by src data.
 *
 *  @param  processAlpha   Use alpha channel of src or not.(JK_TRUE if used)
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxSetARGBPixelArray(
                AJDX_IMAGE      *dest,
                const AJDX_CLIP *clip,
                JKSint          x_dest,
                JKSint          y_dest, 
                const JKUint32  *src, 
                JKSint          w_src,
                JKSint          h_src,
                JKBool          processAlpha);



/**
 *  Convert true color to AJDX Pixel.
 *
 *  @param  trueColor      RGB true color to be converted.
 *
 *  @param  processApha    If to process alpha data.
 *
 *  @return Coverted AJDX_PIXEL from the true color.
 */
AJDX_PIXEL AmDxARGBToPixel(JKUint32 trueColor, JKBool processApha);

/**
 *  Convert AJDX pixel to true color.
 *
 *  @param  pixel          AJDX pixel to be converted.
 *
 *  @return Coverted true color from AJDX_PIXEL.
 */
JKUint32 AmDxPixelToARGB(AJDX_PIXEL pixel);

/**
 *  Set the pixels with specified color in designated image area.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels.
 *
 *  @param  x              The x-coordinate of destination image region.
 *
 *  @param  y              The y-coordinate of destination image region.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxSetPixel (
                AJDX_IMAGE      *dest, 
                const AJDX_CLIP *clip, 
                AJDX_PIXEL      color, 
                JKSint          x, 
                JKSint          y);

/**
 *  Get pixel at specified point (x, y) in designated image.
 *
 *  @param  src            The source image.
 *
 *  @param  x              The x -oordinate of the point in source image.
 *
 *  @param  y              The y-coordinate of the point in source image.
 *
 *  @return The pixel of selected point.
 */
AJDX_PIXEL AmDxGetPixel(const AJDX_IMAGE *src, JKSint x, JKSint y);

/**
 *  Draw a line between the coordinates (x1,y1) and (x2,y2).
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels, the format could be AJDX_PIXEL or 32 bit ARGB.
 *
 *  @param  x1             The x-coordinate of the start of the line.
 *
 *  @param  y1             The y-coordinate of the start of the line.
 *
 *  @param  x2             The x-coordinate of the end of the line.
 *
 *  @param  y2             The y-coordinate of the end of the line.
 *
 *  @param  alpha         JK_TRUE if color format is 32 bit ARGB, JK_FALSE if color format is AJDX_PIXEL.
 *
 *  @param  dotted        JK_TRUE if dotted line required, otherwise JK_FALSE.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxDrawLine(
                    AJDX_IMAGE      *dest,
                    const AJDX_CLIP *clip,
                    JKUint32      color,
                    JKSint          x1,
                    JKSint          y1,
                    JKSint          x2,
                    JKSint          y2,
                    JKBool         alpha,
                    JKBool        dotted);

/**
 *  Draw the outline of the specified rectangle.
 *
 *  The resulting rectangle will cover an area (width + 1) pixels wide by
 *  (height + 1) pixels tall.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels.
 *
 *  @param  x              The x-coordinate of the rectangle to be drawn.
 *
 *  @param  y              The y-coordinate of the rectangle to be drawn.
 *
 *  @param  w              Width of the rectangle to be drawn.
 *
 *  @param  h              Height of the rectangle to be drawn.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxDrawRect (
                AJDX_IMAGE      *dest,
                const AJDX_CLIP *clip,
                AJDX_PIXEL      color,
                JKSint          x,
                JKSint          y,
                JKSint          w,
                JKSint          h);

/**
 *  Draws the outline of the specified rectangle with dotted line style.
 *
 *  The resulting rectangle will cover an area (width + 1) pixels wide by 
 *  (height + 1) pixels tall.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels.
 *
 *  @param  x              The x-coordinate of the rectangle to be drawn.
 *
 *  @param  y              The y-coordinate of the rectangle to be drawn.
 *
 *  @param  w              Width of the rectangle to be drawn.
 *
 *  @param  h              Height of the rectangle to be drawn.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxDrawDottedRect (
                AJDX_IMAGE      *dest,
                const AJDX_CLIP *clip,
                AJDX_PIXEL      color,
                JKSint          x,
                JKSint          y,
                JKSint          w,
                JKSint          h);

/**
 *  Fill the specified rectangle with designated color.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels.
 *
 *  @param  x              The x-coordinate of the rectangle to be filled.
 *
 *  @param  y              The y-coordinate of the rectangle to be filled.
 *
 *  @param  w              Width of the rectangle to be filled.
 *
 *  @param  h              Height of the rectangle to be filled.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxFillRect (
                AJDX_IMAGE      *dest,
                const AJDX_CLIP *clip,
                AJDX_PIXEL      color,
                JKSint          x,
                JKSint          y,
                JKSint          w,
                JKSint          h);

/**
 *  Draw the outline of the specified rounded corner rectangle.
 *
 *  The resulting rectangle will cover an area (width + 1) pixels wide 
 *  by (height + 1) pixels tall.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels.
 *
 *  @param  x              The x-coordinate of the rectangle to be drawn.
 *
 *  @param  y              The y-coordinate of the rectangle to be drawn.
 *
 *  @param  w              Width of the rectangle to be drawn.
 *
 *  @param  h              Height of the rectangle to be drawn.
 *
 *  @param  arcW           Horizontal diameter of the arc at the four corners,
 *                         must not be larger than 'w'.
 *
 *  @param  arcH           Vertical diameter of the arc at the four corners,
 *                         must not be larger than 'h'.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxDrawRoundRect (
                AJDX_IMAGE      *dest,
                const AJDX_CLIP *clip,
                AJDX_PIXEL      color,
                JKSint          x,
                JKSint          y,
                JKSint          w,
                JKSint          h,
                JKSint          arcW,
                JKSint          arcH);

/**
 *  Draw the outline of the specified rounded corner rectangle with
 *  dotted line style.
 *
 *  The resulting rectangle will cover an area (width + 1) pixels wide 
 *  by (height + 1) pixels tall.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels.
 *
 *  @param  x              The x-coordinate of the rectangle to be drawn.
 *
 *  @param  y              The y-coordinate of the rectangle to be drawn.
 *
 *  @param  w              Width of the rectangle to be drawn.
 *
 *  @param  h              Height of the rectangle to be drawn.
 *
 *  @param  arcW           Horizontal diameter of the arc at the four corners,
 *                         must not be larger than 'w'.
 *
 *  @param  arcH           Vertical diameter of the arc at the four corners,
 *                         must not be larger than 'h'.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxDrawDottedRoundRect (
                AJDX_IMAGE      *dest,
                const AJDX_CLIP *clip,
                AJDX_PIXEL      color,
                JKSint          x,
                JKSint          y,
                JKSint          w,
                JKSint          h,
                JKSint          arcW,
                JKSint          arcH);

/**
 *  Fill the specified rounded corner rectangle.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels.
 *
 *  @param  x              The x-coordinate of the rectangle to be filled.
 *
 *  @param  y              The y-coordinate of the rectangle to be filled.
 *
 *  @param  w              Width of the rectangle to be filled.
 *
 *  @param  h              Height of the rectangle to be filled.
 *
 *  @param  arcW           Horizontal diameter of the arc at the four corners,
 *                         must not be larger than 'w'.
 *
 *  @param  arcH           Vertical diameter of the arc at the four corners,
 *                         must not be larger than 'h'.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxFillRoundRect(
                AJDX_IMAGE      *dest,
                const AJDX_CLIP *clip,
                AJDX_PIXEL      color,
                JKSint          x,
                JKSint          y,
                JKSint          w,
                JKSint          h,
                JKSint          arcW,
                JKSint          arcH);

/**
 *  Draw the outline of a circular or elliptical arc covering the 
 *  specified rectangle.
 *
 *  The resulting arc begins at 'startAngle' and extends for 'arcAngle'
 *  degrees, using the current color. Angles are interpreted such that 0
 *  degree is at the 3 o'clock position. A positive value indicates a
 *  counter-clockwise rotation while a negative value indicates a
 *  clockwise rotation. 
 *
 *  The center of the arc is the center of the rectangle whose origin
 *  is (x, y) and whose size is specified by the width and height arguments.
 *
 *  The resulting arc covers an area (width + 1) pixels wide by (height + 1)
 *  pixels tall.
 *
 *  The angles are specified relative to the non-square extents of the bounding
 *  rectangle such that 45 degrees always falls on the line from the center of
 *  the ellipse to the upper right corner of the bounding rectangle.
 *  As a result, if the bounding rectangle is noticeably longer in one axis
 *  than the other, the angles to the start and end of the arc segment will be
 *  skewed farther along the longer axis of the bounds. 
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels.
 *
 *  @param  x              The x-coordinate of the upper-left corner of the arc
 *                         to be drawn.
 *
 *  @param  y              The y-coordinate of the upper-left corner of the arc 
 *                         to be drawn.
 *
 *  @param  w              Width of the arc to be drawn.
 *
 *  @param  h              Height of the arc to be drawn.
 *
 *  @param  startAngle     The beginning angle.
 *
 *  @param  arcAngle       The angular extent of the arc, relative to
 *                         'startAngle', must not be zero.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxDrawArc (
                AJDX_IMAGE      *dest,
                const AJDX_CLIP *clip,
                AJDX_PIXEL      color,
                JKSint          x,
                JKSint          y,
                JKSint          w,
                JKSint          h,
                JKSint          startAngle,
                JKSint          arcAngle);

/**
 *  Draw the outline of a circular or elliptical arc covering the 
 *  specified rectangle with the dotted line style.
 *
 *  The resulting arc begins at startAngle and extends for arcAngle degrees,
 *  using the current color. Angles are interpreted such that 0 degrees is
 *  at the 3 o'clock position. A positive value indicates a counter-clockwise
 *  rotation while a negative value indicates a clockwise rotation.
 *
 *  The center of the arc is the center of the rectangle whose origin is (x, y)
 *  and whose size is specified by the width and height arguments.
 *
 *  The resulting arc covers an area (width + 1) pixels wide by (height + 1)
 *  pixels tall.
 *
 *  The angles are specified relative to the non-square extents of the bounding
 *  rectangle such that 45 degrees always falls on the line from the center of
 *  the ellipse to the upper right corner of the bounding rectangle.
 *  As a result, if the bounding rectangle is noticeably longer in one axis
 *  than the other, the angles to the start and end of the arc segment will be
 *  skewed farther along the longer axis of the bounds.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels.
 *
 *  @param  x              The x-coordinate of the upper-left corner of the arc
 *                         to be drawn.
 *
 *  @param  y              The y-coordinate of the upper-left corner of the arc 
 *                         to be drawn.
 *
 *  @param  w              Width of the arc to be drawn.
 *
 *  @param  h              Height of the arc to be drawn.
 *
 *  @param  startAngle     The beginning angle.
 *
 *  @param  arcAngle       The angular extent of the arc, relative to
 *                         'startAngle', must not be zero.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxDrawDottedArc (
                AJDX_IMAGE      *dest,
                const AJDX_CLIP *clip,
                AJDX_PIXEL      color,
                JKSint          x,
                JKSint          y,
                JKSint          w,
                JKSint          h,
                JKSint          startAngle,
                JKSint          arcAngle);

/**
 *  Fill a circular or elliptical arc covering the specified rectangle.
 *
 *  The resulting arc begins at startAngle and extends for arcAngle degrees.
 *  Angles are interpreted such that 0 degrees is at the 3 o'clock position.
 *  A positive value indicates a counter-clockwise rotation while a negative
 *  value indicates a clockwise rotation.
 *
 *  The center of the arc is the center of the rectangle whose origin is (x, y)
 *  and whose size is specified by the width and height arguments.
 *
 *  The filled region consists of the "pie wedge" region bounded by the arc
 *  segment as if drawn by drawArc(), the radius extending from the center to
 *  this arc at startAngle degrees, and radius extending from the center to this
 *  arc at startAngle + arcAngle degrees.
 *
 *  The angles are specified relative to the non-square extents of the bounding
 *  rectangle such that 45 degrees always falls on the line from the center of
 *  the ellipse to the upper right corner of the bounding rectangle.
 *  As a result, if the bounding rectangle is noticeably longer in one axis
 *  than the other, the angles to the start and end of the arc segment will be
 *  skewed farther along the longer axis of the bounds.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels.
 *
 *  @param  x              The x-coordinate of the upper-left corner of the arc
 *                         to be filled.
 *
 *  @param  y              The y-coordinate of the upper-left corner of the arc 
 *                         to be filled.
 *
 *  @param  w              Width of the arc to be filled.
 *
 *  @param  h              Height of the arc to be filled.
 *
 *  @param  startAngle     The beginning angle.
 *
 *  @param  arcAngle       The angular extent of the arc, relative to
 *                         'startAngle', must not be zero.
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxFillArc (
                AJDX_IMAGE      *dest,
                const AJDX_CLIP *clip,
                AJDX_PIXEL      color,
                JKSint          x,
                JKSint          y,
                JKSint          w,
                JKSint          h,
                JKSint          startAngle,
                JKSint          arcAngle);

/**
 *  Fill a polygon.
 *
 *  If the designated polygon coordinates are outside the clip area
 *  designated for the graphics context, draw only the portion inside the
 *  clip area.
 *
 *  @param  dest           The destination image, must be mutable and not null.
 *                         Before calling this function, all fields of 'dest'
 *                         must be set properly.
 *
 *  @param  clip           The clipping rectangle. Rendering operations have
 *                         no effect outside of the clipping area. If the
 *                         parameter is null, the entire destination image is
 *                         treated as clipping area.
 *
 *  @param  color          Specified color to render pixels.
 *
 *  @param  x              Array of polygon vertex x-coordinates,
 *                         must not be null.
 *
 *  @param  y              Array of polygon vertex y-coordinates,
 *                         must not be null.
 *
 *  @param  num            The number of polygon coordinates, must be between
 *                         3 and 32 (both included).
 *
 *  @return JK_FALSE if any parameters are invalid; JK_TRUE otherwise.
 */
JKBool AmDxFillPolygon(
                AJDX_IMAGE      *dest,
                const AJDX_CLIP *clip,
                AJDX_PIXEL      color,
                const JKSint    *x,
                const JKSint    *y,
                JKSint          num);


JKBool AmDxFillPolygonEx(
    AJDX_IMAGE      *dest,
    const AJDX_CLIP *clip,
    JKUint32      color,
    const JKSint    *x,
    const JKSint    *y,
    JKSint          num,
    void* buf,
    JKBool alpha );

JKSint32 AmDxGetConfigInt(
	int configType
	);


JKSint32 AmDxGetPolygonConfigStructSize();



#ifdef __cplusplus
}
#endif

#endif /* ___AMAJDX_H */

