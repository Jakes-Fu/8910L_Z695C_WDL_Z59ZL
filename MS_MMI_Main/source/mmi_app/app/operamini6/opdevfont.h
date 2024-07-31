/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_FONT_H
#define OPDEV_FONT_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _OD_FontAttr
{
	/* abstract size on OpDev layer, need to map to real font size on specified platform. 
	see also OD_FontSize */
	int font_size;
	int is_bold;
	int is_italic;
	int is_underline;

	/* Currently only one font face is supported, enable this field if we get to support multi font faces. */
	/* const char *face_name; */
} OD_FontAttr;

typedef struct _OD_DrawTextParams
{
	/* layer which text will be drawn on, note we provide both layer and buf(and its 
	pitch/height) here for platforms' compatibility, pass valid value for one of 
	them should be ok, platforms will handle this difference in implementation. */
	OD_GDIHandle layer;
	void *buf;
	/* buf_w should be the pitch, rather than width. */
	int buf_w, buf_h;

	/* color of font, must be in consistent format(see OD_PixelFormat) of the buf. */
	int color;
	int baseline;
	int x, y;
	int clip_x, clip_y, clip_w, clip_h;
} OD_DrawTextParams;

typedef struct _OD_GlyphMetrics
{
	/* bearing x for padding/kerning */
	int bearing_x;
	int width, height;
	int ascent, descent;
	int advance;
} OD_GlyphMetrics;

typedef struct _OD_FontMetrics
{
	int ascent, descent;
	int height;
	/* max width of this font, can be -1 if it can not be retrieved on some platforms. */
	int max_width;
} OD_FontMetrics;

typedef enum _OD_FontSize
{
	/* Abstracted font size, corresponding to what's shown/set in the settings,
	which should be mapped to real font size on platform.
	For example, A typical mapping can be small:12, medium:16, large:20. */
	OD_FONT_SMALL	= -1,
	OD_FONT_MEDIUM	= -2,
	OD_FONT_LARGE	= -3,

	OD_FONT_REAL_SIZE_START	= 0,
	/* The "real" size of font here, can be any positive number among this period,
	which present as expected font height. When a matched font can not be found, a
	successive larger font will be used.
	e.g. may be 8 ~ 72 in practice. */
	OD_FONT_REAL_SIZE_END		= 255
} OD_FontSize;

/**
 * Initialize the font engine instance. usually called when entering opera.
 *
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FontInit(OD_Context *context);

/**
 * Destroy the font engine instance. usually called when exiting opera.
 *
 * @return OD_SUCCESS if the operation is successful, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FontDeinit(OD_Context *context);

/**
* Get font metrics.
*
* @param[in] font_attr the designated font.
* @param[out] font_metrics font metrics describing width/height etc. of font.
*
* @return OD_SUCCESS if the operation is successful, or the appropriate
*         error code from the OD_Error enumeration.
*/
OD_ALL_TASK OD_STATUS OD_GetFontMetrics(OD_Context *context, const OD_FontAttr *font_attr, OD_FontMetrics *font_metrics);

/**
* Get metrics for a specified character.
* @note: we use this function generally for the purpose of measure character width
* inside strings, so in addition to width/height, bearing x and advanced width, if any,
* is also need.
*
* @param[in] font_attr the designated font.
* @param[in] c the designated character.
* @param[out] char_metrics metrics describing width/height etc. of the char.
*
* @return OD_SUCCESS if the operation is successful, or the appropriate
*         error code from the OD_Error enumeration.
*/
OD_ALL_TASK OD_STATUS OD_GetCharMetrics(OD_Context *context, const OD_FontAttr *font_attr, OD_WCHAR c, OD_GlyphMetrics *char_metrics);

/**
* Get width of a text string.
* We will only play with single line text.
*
* @param[in] font_attr the designated font.
* @param[in] text the designated text string.
* @param[in] text_len length of the text string. -1 to indicate need
*				recalculating with strlen().
* @param[out] text_width the measured width of the text.
*
* @return OD_SUCCESS if the operation is successful, or the appropriate
*         error code from the OD_Error enumeration.
*/
OD_ALL_TASK OD_STATUS OD_GetTextWidth(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, int *text_width);

/**
* 'Measure' a string in designated font, that is, calculate the charfit num
* and width on that charfit.
*
* @param[in] font_attr the designated font.
* @param[in] text the designated text string.
* @param[in] text_len length of the text string. -1 to indicate need
*				recalculating with strlen().
* @param[in] max_extent max extent for the charfit calculation, can be negative
*				as no limit.
* @param[out] charfit_num position of the charfit.
* @param[out] fit_width width on that charfit. could be NULL.
*
* @return OD_SUCCESS if the operation is successful, or the appropriate
*         error code from the OD_Error enumeration.
*/
OD_ALL_TASK OD_STATUS OD_MeasureString(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, int max_extent, int *charfit_num, int *fit_width);

/**
* Draw text on a designated layer/buffer.
*
* @param[in] font_attr the designated font.
* @param[in] text the designated text string.
* @param[in] text_len length of the text string. -1 to indicate need
*				recalculating with strlen().
* @param[in] params parameters for drawing, indicate the layer/buffer, font color,
*				and the location info (x, y), etc. The layer/buffer inside is for output.
*
* @return OD_SUCCESS if the operation is successful, or the appropriate
*         error code from the OD_Error enumeration.
*/
OD_ALL_TASK OD_STATUS OD_DrawText(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, const OD_DrawTextParams *params);

/**
* Draw text as glyph on a designated layer/buffer. While OD_DrawText is designed
* mainly to draw a text on real screen, this function is to retrieve glyphs of text,
* to be manipulated later, such as be a caching of text drawing.
*
* @param[in] font_attr the designated font.
* @param[in] text the designated text string.
* @param[in] text_len length of the text string. -1 to indicate need
*				recalculating with strlen().
* @param[in] params parameters for drawing, indicate the layer/buffer, font color,
*				and the location info (x, y), etc. The layer/buffer inside is for output.
*
* @return OD_SUCCESS if the operation is successful, or the appropriate
*         error code from the OD_Error enumeration.
*/
OD_ALL_TASK OD_STATUS OD_GetTextData(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, const OD_DrawTextParams *params);

#ifdef __cplusplus
};
#endif

#endif /* OPDEV_FONT_H */
