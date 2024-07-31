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

#include "mmk_app.h"
#include "mmk_shell.h"
#include "mmi_position.h"
#include "guilcd.h"
#include "guiwin.h"
#include "guistatusbar.h"

#ifdef PLATFORM_SC6600L
/*
* Only 6600 need to explicitly declare it, because it is not included in 
* any public header files.
*/
extern BOOLEAN MMK_UpdateScreen( void );
#endif /* */

static int _OD_GetStatusBarHeight(void);

/************************************************************************/
/* GDI Layer                                                            */
/************************************************************************/
OD_STATUS OD_GDIInit(OD_Context *context)
{
	return OD_SUCCESS;
}

OD_STATUS OD_GDIDeinit(OD_Context *context)
{
	return OD_SUCCESS;
}

OD_STATUS OD_GDICreateLayer(OD_Context *context, OD_PixelFormat format, 
							int width, int height, 
							char *buffer, OD_GDIHandle *handle)
{
	OD_CHECK_ARGS_RETURN_STATUS(handle != NULL);

    // Dummy, spreamtrum don't user layer now, only set *handle to be valid value.
    *handle = 1;

	return OD_SUCCESS;
}

OD_STATUS OD_GDIDestroyLayer(OD_Context *context, OD_GDIHandle handle)
{
    // Dummy, spreamtrum don't use layer now
	return OD_SUCCESS;
}

OD_STATUS OD_GDIAllocMemory(OD_Context *context, int size, char **pBuffer)
{
	OD_CHECK_ARGS_RETURN_STATUS(pBuffer);
	OD_CHECK_ARGS_RETURN_STATUS(size > 0);

	*pBuffer = (char*)SCI_ALLOCA(size);

	OD_TRACE_LOW("OD_GDIAllocMemory: *pBuffer=0x%x, size=%d", *pBuffer, size);

	/* Not a same memory pool with opera, so don't return OD_NO_MEMORY when no allocation failed. */
	return *pBuffer ? OD_SUCCESS : OD_ERROR;
}

void OD_GDIFreeMemory(OD_Context *context, char *pBuffer)
{
	OD_CHECK_ARGS_RETURN_VOID(pBuffer);

	OD_TRACE_LOW("OD_GDIFreeMemory: pBuffer=0x%x", pBuffer);

	SCI_FREE(pBuffer);
}

OD_BOOL OD_GDIUseNativeMemory(OD_Context *context)
{
	return OD_FALSE;
}

/************************************************************************/
/* GDI Util                                                             */
/************************************************************************/

OD_STATUS OD_GetScreenBuffer(OD_Context *context, char** buffer)
{
    OD_CHECK_ARGS_RETURN_STATUS(buffer);

#if defined(OD_MULTILAYER_DISABLED)
    {
        /* 6600 has no layers. Multilayer feature may be disabled by SDK. 
        Then we directly use LCD buffer. */
        LCD_INFO_T info;
        BOOLEAN succeed = 
#ifdef PLATFORM_SC6600L
        	/* GUILCD_GetInfo have different interface on 6600 SDK. */
        	ERR_LCD_NONE == GUILCD_GetInfo(MAIN_LCD_ID, &info);
#else
        	GUILCD_GetInfo(MAIN_LCD_ID, &info);
#endif
        if(succeed == FALSE)
        {
            *buffer = NULL;
        }
        else
        {
            *buffer = info.lcdbuff_ptr;
        }
    }
#else
    {
        GUI_LCD_DEV_INFO main_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        *buffer = UILAYER_GetLayerBufferPtr(&main_dev_info);
    }
#endif

	if(!*buffer)
    {
        OD_ASSERT(!"OD_GetScreenBuffer failed");
        return OD_ERROR;
    }
	return OD_SUCCESS;
}

int OD_GetScreenDPI(OD_Context *context)
{
	return 170; // Wait for spreadtrum API
}


OD_STATUS OD_GetLayerDimension(OD_LayerType type, int *width, int *height)
{
	OD_CHECK_ARGS_RETURN_STATUS(width);
	OD_CHECK_ARGS_RETURN_STATUS(height);

	/* 
	 * We should get logical screen size, which consider screen rotation.
	 * When screen rotated, width and height should exchange.
	 */
	{
#ifdef PLATFORM_SC6600L
		/* No screen rotation support in 6600, so get lcd info directly. 
		Besides, no other function to get width and height. */
		LCD_INFO_T info = {0};
		if(ERR_LCD_NONE != GUILCD_GetInfo(GUI_MAIN_LCD_ID, &info))
		{
			OD_ASSERT(!"GUILCD_GetInfo failed");
			return OD_ERROR;
		}
		*width = info.lcd_width;
		*height = info.lcd_height;
#elif defined(OD_6530_12A) || defined(OD_6530_12B) || defined(OD_6530_12C) || defined(OD_8501_MAIN)
		{
			unsigned short int w = 0, h = 0;
			if(!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &w, &h))
			{
				OD_ASSERT(!"GUILCD_GetLogicWidthHeight failed");
				return OD_ERROR;
			}
			*width = w;
			*height = h;
		}
#else
		/* Support screen rotation, so get logical screen size. */
		CAF_SCREENINFO_T si = {0};
		ISHELL_GetScreenInfo(&si);
		*width = si.width;
		*height = si.height;
#endif
	}

	switch (type)
	{
	case OD_LAYER_OPERA:
		break;
	case OD_LAYER_TEXT:
		{
			*width = OD_MAX(*width, *height);
			*height = OD_TEXT_LAYER_HEIGHT;
		}
		break;
	case OD_LAYER_STATUS_BAR:
		{
			*width = OD_MAX(*width, *height);
			*height = OD_STATUSBAR_LAYER_HEIGHT;
		}
		break;
	default:
		return OD_INVALID_PARAM;
	}

	return OD_SUCCESS;
}

OD_PixelFormat OD_GetLayerPixelFormat(OD_LayerType layer)
{
	/* All layers use RGB565 in spreadtrum. */
	return OD_RGB565;
}

int OD_GetLayerBPP(OD_PixelFormat format)
{
	switch(format)
	{
	case OD_RGB565:
		return 2;
	case OD_RGB888:
		return 3;
	default:
		OD_ASSERT(!"Error. Get layer BPP with unknown pixel format");
	}

	return 0;
}

#define ANGLE_CONVERT_O2S(angle) (angle)
#define ANGLE_CONVERT_S2O(angle) (angle)

OD_RotationAngle OD_GetRotationAngle(OD_Context *context)
{
#ifdef PLATFORM_SC6600L
    /* 6600 has no screen rotation support */
    return OD_ANGLE_0;
#else
	return ANGLE_CONVERT_S2O(MMK_GetScreenAngle());
#endif
}

OD_ScreenMode OD_GetScreenMode(OD_Context *context)
{
	OD_RotationAngle ang = OD_ANGLE_0;
	
	OD_CHECK_ARGS(context, ang);

	OD_GetRotationAngle(context);
	if (ang == OD_ANGLE_0 || ang == OD_ANGLE_180)
		return OD_PORTRAIT;
	if (ang == OD_ANGLE_90 || ang == OD_ANGLE_270)
		return OD_LANDSCAPE;
	return OD_PORTRAIT;
}

OD_STATUS OD_GetViewportArea(OD_Context *context, int *x, int *y, int *w, int *h)
{
	OD_STATUS ret = OD_SUCCESS;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(x);
	OD_CHECK_ARGS_RETURN_STATUS(y);
	OD_CHECK_ARGS_RETURN_STATUS(w);
	OD_CHECK_ARGS_RETURN_STATUS(h);

	*x = 0;
	*y = 0;
	ret = OD_GetLayerDimension(OD_LAYER_OPERA, w, h); 
	if (ret == OD_SUCCESS && !context->data.gdi.fullScreen)
	{
	/* If the window has no status bar, we needn't minus the height */
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
		*y = _OD_GetStatusBarHeight();
		*h -= *y;
#endif
	}
	return ret;
}

OD_STATUS OD_Paint(OD_Context *context, int x, int y, int width, int height)
{
	GUI_RECT_T rect;
    BOOLEAN invalicateResult = FALSE;
    /* Sometimes the parameter maybe be (0, 0, 0, 65535), which make device assert. */
	if(width <= 0 || height <= 0)
		return OD_SUCCESS;

	rect.left = x;
	rect.top = y;
	rect.right = x + width - 1;
	rect.bottom = y + height - 1;

#ifdef PLATFORM_SC6600L
    invalicateResult = GUILCD_InvalidateLCDRect(GUI_MAIN_LCD_ID, rect.left, rect.top, rect.right, rect.bottom, 0);
#else
    invalicateResult = GUILCD_InvalidateRect(GUI_MAIN_LCD_ID, rect, 0);
#endif
    if (!invalicateResult)
    {
    	OD_TRACE_LOW("OD_Paint failed");
    	return OD_ERROR;
    }
	return OD_SUCCESS;
}

OD_STATUS OD_ShowOpera(OD_Context *context)
{
	(context);
	return OD_SUCCESS;
}

OD_STATUS OD_HideOpera(OD_Context *context)
{
	(context);
	return OD_SUCCESS;
}

OD_STATUS OD_RotateScreen(OD_Context *context, OD_RotationAngle angle)
{
#ifdef PLATFORM_SC6600L
    /* 6600 has no screen rotation support */
    return OD_NOT_SUPPORTED;
#else
	if(MMK_SetScreenAngle(ANGLE_CONVERT_O2S(angle)))
		return OD_SUCCESS;
	OD_TRACE_LOW("OD_RotateScreen failed");
	return OD_ERROR;
#endif
}

OD_STATUS OD_ToggleFullScreen(OD_Context *context, int bFullScreen)
{
	MMI_HANDLE_T ctrl_handle = 0;

	OD_CHECK_ARGS_RETURN_STATUS(context);

	/** Assume opera is not in fullscreen mode after create */
	if(bFullScreen == context->data.gdi.fullScreen) 
	{
		OD_TRACE_LOW("FullScreen has already been set to %d, abort.", bFullScreen);
		return OD_ERROR;
	}

	context->data.gdi.fullScreen = bFullScreen;
	ctrl_handle = MMK_GetCtrlHandleByWin(context->data.app.win_id,  MMITHEME_GetStatusBarCtrlId());

	if(bFullScreen)
	{	
		if(ctrl_handle)
		{
			MMK_DestroyControl(ctrl_handle);
		}
	}
	else
	{
		if (0 == ctrl_handle)
		{
			GUIWIN_CreateStbDyn(context->data.app.win_id);
		}

		GUIWIN_UpdateStb();
		MMK_UpdateScreen();
	}

	OD_TRACE_LOW("OD_ToggleFullScreen succeed");
	return OD_SUCCESS;
}

static int _OD_GetStatusBarHeight(void)
{
    return OD_STATUSBAR_LAYER_HEIGHT;
}
