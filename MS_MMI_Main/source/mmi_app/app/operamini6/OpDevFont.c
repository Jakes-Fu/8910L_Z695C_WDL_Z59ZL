/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#include "opcfg.h"
#include "opdevinternal.h"
#include "opimport.h"

#include "guifont.h"
#include "guistring.h"

OD_STATUS OD_FontInit(OD_Context *context)
{
	return OD_SUCCESS;
}

OD_STATUS OD_FontDeinit(OD_Context *context)
{
	return OD_SUCCESS;
}

static void* OD_GetFontLayerBuffer(OD_Context *context)
{
	OD_CHECK_ARGS(context, NULL);
	return context->data.gdi.text_buffer;
}

static OD_GDIHandle OD_GetFontLayerHandle(OD_Context *context)
{
	OD_CHECK_ARGS(context, OD_INVALID_GDI_HANDLE);
	return context->data.gdi.text_layer;
}

static int _OD_MapOpDevFontSizeToPlatform(int opdevFontSize)
{
	if (opdevFontSize < 0)
	{
		switch (opdevFontSize)
		{
		case OD_FONT_SMALL:
			return MMI_DEFAULT_SMALL_FONT;
		case OD_FONT_MEDIUM:
			return MMI_DEFAULT_NORMAL_FONT;
		case OD_FONT_LARGE:
			return MMI_DEFAULT_BIG_FONT;
		default:
			OD_ASSERT(!"Shouldn't happen!");
			return MMI_DEFAULT_NORMAL_FONT;
		}
	}
	else
	{
		OD_ASSERT(opdevFontSize >= OD_FONT_REAL_SIZE_START && opdevFontSize <= OD_FONT_REAL_SIZE_END);
		return opdevFontSize;
	}
}

static OD_BOOL _OD_FontNeedCheckCharFits(OD_Context *context, const OD_FontAttr *font_attr)
{
	(context);
	(font_attr);

	/* Always check charfits, see HULA-607. */
	return OD_TRUE;
}

static void _OD_translateParams(GUISTR_STYLE_T *style, GUISTR_STATE_T *state, const OD_FontAttr *font_attr, int color)
{
	style->font = _OD_MapOpDevFontSizeToPlatform(font_attr->font_size);
	style->font_color = color;

	*state = GUISTR_STATE_SINGLE_LINE;
	if (font_attr->is_bold)
		*state |= GUISTR_STATE_BOLD;
	if (font_attr->is_italic)
		*state |= GUISTR_STATE_ITALIC;
	if (font_attr->is_underline)
		*state |= GUISTR_STATE_UNDERLINE;
}

static OD_STATUS _OD_GetStringWH(const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, int *w, int *h)
{
	MMI_STRING_T str = {0};
	GUISTR_STYLE_T style = {0};
	GUISTR_STATE_T state = 0;
	GUISTR_INFO_T info = {0};

	OD_CHECK_ARGS_RETURN_STATUS(font_attr);
	OD_CHECK_ARGS_RETURN_STATUS(text);
	OD_CHECK_ARGS_RETURN_STATUS(w);
	OD_CHECK_ARGS_RETURN_STATUS(h);

	*w = 0;
	*h = 0;

	_OD_translateParams(&style, &state, font_attr, 0);

	str.wstr_ptr = (wchar *)text;
	str.wstr_len = text_len < 0 ? od_wcslen(text) : text_len;

	if (!GUISTR_GetStringInfo(&style, &str, state, &info))
	{
		return OD_ERROR;
	}

	*w = info.width;
	*h = info.height;

	return OD_SUCCESS;
}

OD_STATUS OD_DrawText(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, const OD_DrawTextParams *params)
{
	GUISTR_BUF_T buf = {0};
	GUI_RECT_T rect = {0};
	GUI_RECT_T clip = {0};
	MMI_STRING_T str = {0};
	GUISTR_STYLE_T style = {0};
	GUISTR_STATE_T state = 0;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(font_attr);
	OD_CHECK_ARGS_RETURN_STATUS(text);
	OD_CHECK_ARGS_RETURN_STATUS(params);
	OD_CHECK_ARGS_RETURN_STATUS(text_len == -1 || text_len >= 0);

	buf.buffer = params->buf;
	buf.width = params->buf_w;
	buf.height = params->buf_h;
#ifndef PLATFORM_SC6600L
	buf.mem_width = params->buf_w;
	buf.type = GUISTR_BUF_RGB565;
#endif /* PLATFORM_SC6600L */

	rect.left = params->x;
	rect.top = params->y;
	rect.right = params->buf_w;
	rect.bottom = params->buf_h;

	/* There's one bug when the width or height of clip rect is 1, application needs to make a workaround. */
	OD_ASSERT(params->clip_w > 1);
	OD_ASSERT(params->clip_h > 1);

	clip.left = params->clip_x;
	clip.top = params->clip_y;
	/* clip rect for sprd's api contains point (clip.right, clip.bottom) */
	clip.right = params->clip_x + params->clip_w - 1;
	clip.bottom = params->clip_y + params->clip_h - 1;

	_OD_translateParams(&style, &state, font_attr, params->color);

	str.wstr_ptr = (wchar *)text;
	str.wstr_len = (text_len == -1) ? od_wcslen(text) : text_len;

	GUISTR_DrawTextInRect(&buf, &rect, &clip, &str, &style, state, GUISTR_TEXT_DIR_AUTO);

	return OD_SUCCESS;
}

OD_STATUS OD_GetCharMetrics(OD_Context *context, const OD_FontAttr *font_attr, OD_WCHAR c, OD_GlyphMetrics *char_metrics)
{
	int w, h;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(font_attr);
	OD_CHECK_ARGS_RETURN_STATUS(char_metrics);

	OD_RETURNIF(_OD_GetStringWH(font_attr, &c, 1, &w, &h));

	char_metrics->bearing_x = 0;
	char_metrics->width = w;
	char_metrics->height = h;
	char_metrics->ascent = char_metrics->height;
	char_metrics->descent = 0;
	char_metrics->advance = char_metrics->width;

	return OD_SUCCESS;
}

OD_STATUS OD_GetFontMetrics(OD_Context *context, const OD_FontAttr *font_attr, OD_FontMetrics *font_metrics)
{
	OD_WCHAR const string_for_maxh[] = {
		// Add predefined characters to check the max height for a font.
		'j',
		'|',
		0x4F4F,	// CJK Unified Ideographs(4E00-9FBF)
		0xFF01,	// fullwidth exclamation mark
		0xFF5C,	// fullwidth vertical line

		// Additional sample characters can be added here.

		'y', 'g',	// HULA-965: the bottom of 'y' and 'g' can't be displayed completely
		'[', ']',	// HUFEI-15: the bottom of '[' and ']' can't be displayed completely

		'\0',	// Don't forget to add null at last because we will use it as a string.
	};
	int w, h;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(font_attr);
	OD_CHECK_ARGS_RETURN_STATUS(font_metrics);

	font_metrics->height = GUIFONT_GetHeight(_OD_MapOpDevFontSizeToPlatform(font_attr->font_size));
	OD_RETURNIF(_OD_GetStringWH(font_attr, string_for_maxh, -1, &w, &h));
	font_metrics->height = OD_MAX(font_metrics->height, h);
	font_metrics->ascent = font_metrics->height;
	font_metrics->descent = 0;
	font_metrics->max_width = font_metrics->height;

	return OD_SUCCESS;
}

OD_STATUS OD_MeasureString(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, int max_extent, int *charfit_num, int *fit_width)
{
	int len = (text_len == -1) ? od_wcslen(text) : text_len;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(font_attr);
	OD_CHECK_ARGS_RETURN_STATUS(text);
	OD_CHECK_ARGS_RETURN_STATUS(charfit_num);
	OD_CHECK_ARGS_RETURN_STATUS(text_len == -1 || text_len >= 0);

	*charfit_num = 0;
	if (fit_width != NULL)
	{
		*fit_width = 0;
	}

	if (max_extent < 0)
	{
		/* There's no limitation, all chars will be fit. */
		*charfit_num = len;
	}
	else
	{
		int cf_width;

		if (OD_GetTextWidth(context, font_attr, text, len, &cf_width) != OD_SUCCESS)
		{
			return OD_ERROR;
		}

		if (cf_width <= max_extent)
		{
			/* All chars can be fit in given space. */
			*charfit_num = len;
			if (fit_width != NULL)
			{
				*fit_width = cf_width;
			}
		}
		else
		{
			uint16 cn = 0, pn = 0;
			GUISTR_STYLE_T style = {0};
			GUISTR_STATE_T state = 0;
			OD_BOOL check_charfit = _OD_FontNeedCheckCharFits(context, font_attr);

			_OD_translateParams(&style, &state, font_attr, 0);
			if(GUISTR_LineBreak(GUISTR_TEXT_DIR_AUTO, (wchar *)text, len, &style, state, (uint16)max_extent, &cn, &pn) == FALSE)
			{
				cn = len / 2;			/* to make the initial search space be half of len. */
				check_charfit = OD_TRUE;
			}
			*charfit_num = OD_MIN(cn, len);

			/*
			* On specified devices the return value from "GUISTR_LineBreak" will be incorrect,
			* then we need to check the return value to get the exact char fit.
			*/
			if (check_charfit)
			{
				/*
				* cf_min and cf_max are the possible char fit number, inclusive.
				* The text width of the first cf_min chars is always less than or equal to the given space.
				* The text width of the first cf_max chars may be greater than the given space.
				*/
				int cf_min, cf_max;

				if (*charfit_num == len)
				{
					cf_min = 0;
					cf_max = len - 1;
				}
				else
				{
					if (OD_GetTextWidth(context, font_attr, text, *charfit_num, &cf_width) != OD_SUCCESS)
					{
						return OD_ERROR;
					}

					if (cf_width <= max_extent)
					{
						/* Yes, check it even if the width is equal to max extent, esp. for complex script language. */
						cf_min = *charfit_num;
						cf_max = len;
					}
					else
					{
						cf_min = 0;
						cf_max = *charfit_num - 1;
					}
				}
				cf_min = OD_MAX(cf_min, 0);
				cf_max = OD_MAX(cf_min, cf_max);

				*charfit_num = OD_MIN(OD_MAX(*charfit_num, cf_min), cf_max);
				while (cf_min <= cf_max)
				{
					if (cf_max - cf_min == 1)
					{
						if (OD_GetTextWidth(context, font_attr, text, cf_max, &cf_width) != OD_SUCCESS)
						{
							return OD_ERROR;
						}
						if (cf_width <= max_extent)
						{
							*charfit_num = cf_max;
							if (fit_width != NULL)
							{
								*fit_width = cf_width;
							}
							break;
						}
						else
						{
							--cf_max;
						}
					}
					if (cf_max - cf_min == 0)
					{
						*charfit_num = cf_min;
						break;
					}
					*charfit_num = cf_min + (cf_max - cf_min)/2;
					if (OD_GetTextWidth(context, font_attr, text, *charfit_num, &cf_width) != OD_SUCCESS)
					{
						return OD_ERROR;
					}

					if (cf_width <= max_extent)
					{
						cf_min = *charfit_num;
					}
					else
					{
						cf_max = *charfit_num - 1;
					}
				}
				OD_ASSERT(cf_min <= *charfit_num && *charfit_num <= cf_max);
				OD_ASSERT(*charfit_num < len);
			}
		}
	}

	if (fit_width != NULL && *fit_width == 0 && *charfit_num != 0)
	{
		if(OD_GetTextWidth(context, font_attr, text, *charfit_num, fit_width) != OD_SUCCESS)
		{
			return OD_ERROR;
		}
		OD_ASSERT(max_extent < 0 || (0 <= *fit_width && *fit_width <= max_extent));
	}

	return OD_SUCCESS;
}

static int _OD_565To256(int color)
{
	int r = (color >> 8) & 0xF8;
	int g = (color >> 3) & 0xFC;
	int b = (color << 3) & 0xF8;
	return (r + g + b) / 3;
}

OD_STATUS OD_GetTextData(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, const OD_DrawTextParams *params)
{
	GUISTR_BUF_T buf = {0};
	GUI_POINT_T p = {0};
	MMI_STRING_T str = {0};
	GUISTR_STYLE_T style = {0};
	GUISTR_STATE_T state = 0;
	int x, y;
	int text_layer_w, text_layer_h, text_layer_bpp;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(font_attr);
	OD_CHECK_ARGS_RETURN_STATUS(text);
	OD_CHECK_ARGS_RETURN_STATUS(params);
	OD_CHECK_ARGS_RETURN_STATUS(text_len == -1 || text_len >= 0);

	OD_GetLayerDimension(OD_LAYER_TEXT, &text_layer_w, &text_layer_h);
	text_layer_bpp = OD_GetLayerBPP(OD_GetLayerPixelFormat(OD_LAYER_TEXT));

	buf.buffer = context->data.gdi.text_buffer;
	buf.width = text_layer_w;
	buf.height = text_layer_h;
#ifndef PLATFORM_SC6600L
	buf.mem_width = text_layer_w;
	buf.type = GUISTR_BUF_RGB565;
#endif /* PLATFORM_SC6600L */

	od_memset(buf.buffer, 0, text_layer_w * text_layer_h * text_layer_bpp);

	p.x = params->x;
	p.y = params->y;

	_OD_translateParams(&style, &state, font_attr, 0xffff);

	str.wstr_ptr = (wchar *)text;
	str.wstr_len = (text_len == -1) ? od_wcslen(text) : text_len;

	GUISTR_DrawTextSingleLine(&buf, &p, &str, &style, state);

	for (y = 0; y < OD_MIN(text_layer_h, params->buf_h); y ++)
	{
		for (x = 0; x < OD_MIN(text_layer_w, params->buf_w); x ++)
		{
			((unsigned char *)params->buf)[y * params->buf_w + x] = _OD_565To256(((uint16 *)context->data.gdi.text_buffer)[y * text_layer_w + x]);
		}
	}

	return OD_SUCCESS;
}

OD_STATUS OD_GetTextWidth(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, int *text_width)
{
	int len = (text_len == -1) ? od_wcslen(text) : text_len;
	MMI_STRING_T str = {0};
	GUISTR_STYLE_T style = {0};
	GUISTR_STATE_T state = 0;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(font_attr);
	OD_CHECK_ARGS_RETURN_STATUS(text);
	OD_CHECK_ARGS_RETURN_STATUS(text_width);
	OD_CHECK_ARGS_RETURN_STATUS(text_len == -1 || text_len >= 0);

	_OD_translateParams(&style, &state, font_attr, 0);

	str.wstr_ptr = (wchar *)text;
	str.wstr_len = (text_len == -1) ? od_wcslen(text) : text_len;

	*text_width = GUISTR_GetStringWidth(&style, &str, state);

	return OD_SUCCESS;
}


