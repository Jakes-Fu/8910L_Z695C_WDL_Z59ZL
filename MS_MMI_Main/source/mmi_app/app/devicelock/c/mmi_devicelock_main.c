/*****************************************************************************
** File Name:                                                  *
** Author:                                                                   *
** Date:                                                       *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe key lock                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**                             *
*****************************************************************************/
#define _MMI_DEVICELOCK_MAIN_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_devicelock_trc.h"
#include "mmi_id.h"
#include "mmi_text.h"
#include "window_parse.h"
#include "mmi_appmsg.h"
#include "guires.h"
#include "graphics_draw.h"
#include "guictrl_api.h"
#include "mmiset_nv.h"
#include "guistatusbar.h"
#include "mmipub.h"
#include "guicommon.h"
#include "mmidisplay_data.h" 
#include "dal_time.h"
#include "mmicom_banner.h"

#include "mmiidle_statusbar.h"


#include "guilcd.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "mmi_textfun.h"


#include "mmiidle_export.h"
#include "mmi_wallpaper_export.h"
#include "mmicl_export.h"

#include "mmi_devicelock_internal.h"
#include "mmi_devicelock_export.h"
#include "mmi_devicelock_id.h"
#include "mmi_devicelock_menutable.h"
#include "mmi_devicelock_image.h"
#include "mmi_devicelock_position.h"
#include "mmi_devicelock_nv.h"
#include "mmi_devicelock_text.h"

#include "gpio_prod_api.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL DEVICELOCK_NOTIFICATION_STRUCT s_devicelock_notification[MMISET_NOTIFICATION_MAX] = {0};

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL GUI_LCD_DEV_INFO            grayscale_layer_handle;
LOCAL GUI_LCD_DEV_INFO            clock_layer_handle;
LOCAL GUI_LCD_DEV_INFO            stb_layer_handle;
LOCAL MMI_DEVICELOCK_PWD_T s_devicelock_code = {0};

const MMI_WIN_ID_T  g_devicelock_win = MMI_DEVICELOCK_WIN_ID;

char s_num_input[SOS_INPUT_MAX+1] = {0};
static uint8 s_input_timer_id = 0;

//dong.chunguang modify the postion and size should be consistent with settings shortcut
#if 0
LOCAL const MMI_IMAGE_ID_T s_shortcut_list_img[MMISET_SHORTCUT_MAX] =
{
	IMAGE_LOCK_SCREEN_MESSAGING,
	IMAGE_LOCK_SCREEN_MESSENGER,
	IMAGE_LOCK_SCREEN_MUSIC,
	IMAGE_LOCK_SCREEN_OPERA,
	IMAGE_LOCK_SCREEN_OUTLOOK,
	IMAGE_LOCK_SCREEN_PHONE,
	IMAGE_LOCK_SCREEN_PHOTOS,
	IMAGE_LOCK_SCREEN_RECORDER,
	IMAGE_LOCK_SCREEN_SETTINGS,
	IMAGE_LOCK_SCREEN_SKYPE,
	IMAGE_LOCK_SCREEN_TORCH,
	IMAGE_LOCK_SCREEN_VIDEOS,
	IMAGE_LOCK_SCREEN_ALARM_CLOCK,
	IMAGE_LOCK_SCREEN_CALCULATOR,
	IMAGE_LOCK_SCREEN_CALENDAR,
	IMAGE_LOCK_SCREEN_CAMERA,
	IMAGE_LOCK_SCREEN_CONTACTS,
	IMAGE_LOCK_SCREEN_FILE_EXPLORER,
	IMAGE_LOCK_SCREEN_FM_RADIO,
	IMAGE_NULL,
};
#else
LOCAL const MMI_IMAGE_ID_T s_shortcut_list_img[MMISET_SHORTCUT_MAX] =
{
	IMAGE_NULL,
	IMAGE_LOCK_SCREEN_CONTACTS,
	IMAGE_DEVICELOCK_PHONE_SHORTCUT_ICON,
	IMAGE_LOCK_SCREEN_MESSAGING,
#ifdef MMI_AUDIO_PLAYER_SUPPORT
	IMAGE_LOCK_SCREEN_MUSIC,
#endif
#ifdef VIDEO_PLAYER_SUPPORT
	IMAGE_LOCK_SCREEN_VIDEOS,
#endif
#ifdef FM_SUPPORT
	IMAGE_LOCK_SCREEN_FM_RADIO,
#endif
#ifdef  CAMERA_SUPPORT
	IMAGE_DEVICELOCK_CAMERA_SHORTCUT_ICON,
#endif
	IMAGE_LOCK_SCREEN_ALARM_CLOCK,
	IMAGE_LOCK_SCREEN_FILE_EXPLORER,
#ifdef PIC_VIEWER_SUPPORT
	IMAGE_LOCK_SCREEN_PHOTOS,
#endif
	IMAGE_LOCK_SCREEN_CALENDAR,
	IMAGE_LOCK_SCREEN_CALCULATOR,
#ifdef BROWSER_SUPPORT
	IMAGE_LOCK_SCREEN_OPERA,
#endif
	IMAGE_LOCK_SCREEN_RECORDER,
	IMAGE_SETTINGS,
	IMAGE_EXTRAS,
	IMAGE_COUNTERS,
};
#endif//end
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDEVICELOCK_HandleWinMsg(
										 MMI_WIN_ID_T    win_id, 
										 uint16          msg_id, 
										 DPARAM          param
										 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_CreateGrayscaleLayer(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_CreateClockLayer(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_CreateStbLayer(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_ReleaseGrayscaleLayer(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_ReleaseClockLayer(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_ReleaseStbLayer(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDEVICELOCK_CreatWallpaperCtrl(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_CreateWinCtrlDynamic(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_DisplayClockCallbackFun (GUIANIM_OWNER_DRAW_T  *draw_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_DisplayWinSoftkey(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_DisplayTime(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_DisplayNotifaction(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIDEVICELOCK_HandleSecurityWinMsg(
													  MMI_WIN_ID_T      win_id,
													  MMI_MESSAGE_ID_E     msg_id,
													  DPARAM               param
													  );

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
WINDOW_TABLE(MMI_DEVICELOCK_WINTAB) =
{
    WIN_FUNC((uint32)MMIDEVICELOCK_HandleWinMsg),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_ID(MMI_DEVICELOCK_WIN_ID),
    //WIN_STYLE( WS_DISABLE_RETURN_WIN|WS_HAS_ANIM_BG|WS_DISABLE_COMMON_BG ),
    WIN_STYLE( WS_DISABLE_RETURN_WIN|WS_HAS_ANIM_BG ),
    CREATE_ANIM_CTRL(MMI_DEVICELOCK_ANIM_BG_ID,MMI_DEVICELOCK_WIN_ID),
    //WIN_SOFTKEY(TXT_IDLE_UNLOCK,TXT_NULL,TXT_NULL),
    END_WIN
};

WINDOW_TABLE( MMI_DEVICELOCK_SECURITY_WINTAB ) =
{
        WIN_FUNC( (uint32)MMIDEVICELOCK_HandleSecurityWinMsg ),    
        WIN_ID( MMI_DEVICELOCK_SECURITY_WIN_ID ),
        WIN_STATUSBAR,
        CREATE_ANIM_CTRL(MMI_DEVICELOCK_SECURITY_ANIM_ID,MMI_DEVICELOCK_SECURITY_WIN_ID),
        CREATE_EDIT_PASSWORD_CTRL(MMI_DEVICELOCK_PWD_MAX_LEN,MMI_DEVICELOCK_SECURITY_PWD_ID),
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
        END_WIN
};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/


/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDEVICELOCK_GetInputNum(uint16  msg_id, char* input_num)
{
	if(PNULL == input_num)
	{
		return FALSE;
	}
	if(MSG_KEYDOWN_1 == msg_id)
	{
		sprintf((char *)input_num,"%s","1");	
	}
	else if(MSG_KEYDOWN_2 == msg_id)
	{
		sprintf((char *)input_num,"%s","2");	
	}
	else if(MSG_KEYDOWN_3 == msg_id)
	{
		sprintf((char *)input_num,"%s","3");	
	}
	else if(MSG_KEYDOWN_4 == msg_id)
	{
		sprintf((char *)input_num,"%s","4");	
	}
	else if(MSG_KEYDOWN_5 == msg_id)
	{
		sprintf((char *)input_num,"%s","5");	
	}
	else if(MSG_KEYDOWN_6 == msg_id)
	{
		sprintf((char *)input_num,"%s","6");	
	}
	else if(MSG_KEYDOWN_7 == msg_id)
	{
		sprintf((char *)input_num,"%s","7");	
	}
	else if(MSG_KEYDOWN_8 == msg_id)
	{
		sprintf((char *)input_num,"%s","8");	
	}
	else if(MSG_KEYDOWN_9 == msg_id)
	{
		sprintf((char *)input_num,"%s","9");	
	}
	else if(MSG_KEYDOWN_0 == msg_id)
	{
		sprintf((char *)input_num,"%s","0");	
	}
	return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDEVICELOCK_HandleWinMsg(
										 MMI_WIN_ID_T    win_id, 
										 uint16          msg_id, 
										 DPARAM          param
										 )
{   
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	wchar num_input_wstr[SOS_INPUT_MAX+1] = {0};
	MMI_STRING_T emergency_str_info = {0};  
	uint16 emergency_call_len = 0;
	char input_num[2] = {0};

	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		SCI_MEMSET(s_num_input, 0x00, sizeof(s_num_input));
	case MSG_GET_FOCUS:
		GUIWIN_SetStbState(win_id,GUISTATUSBAR_STATE_WITHOUT_HEADER,TRUE);
		MMIAPICOM_StatusAreaLockscreenSet();
		MMIDEVICELOCK_ReleaseGrayscaleLayer();
		MMIDEVICELOCK_ReleaseClockLayer();
		MMIDEVICELOCK_ReleaseStbLayer();
		MMIDEVICELOCK_CreateGrayscaleLayer();
		MMIDEVICELOCK_CreateClockLayer();
		MMIDEVICELOCK_CreateStbLayer();
		MMIDEVICELOCK_CreatWallpaperCtrl();
		
		//MMIDEVICELOCK_CreateWinCtrlDynamic();
		break;
		
	case MSG_FULL_PAINT:
		{			
			UILAYER_Clear(&clock_layer_handle);
			MMIDEVICELOCK_DisplayTime();
			MMIDEVICELOCK_DisplayNotifaction();
			MMIDEVICELOCK_DisplayWinSoftkey();
		}
		
		break;
	case MSG_CLOSE_WINDOW:
	case MSG_LOSE_FOCUS:
		GUIWIN_SetStbState(win_id,GUISTATUSBAR_STATE_WITHOUT_HEADER,FALSE);
		MMIAPICOM_StatusAreaWithheaderSet();
		MMIDEVICELOCK_ReleaseGrayscaleLayer();
		MMIDEVICELOCK_ReleaseClockLayer();
		MMIDEVICELOCK_ReleaseStbLayer();
		if (s_input_timer_id != 0)
		{
			MMK_StopTimer(s_input_timer_id);
			s_input_timer_id = 0;
		} 
		break;
	case MSG_CTL_ANIM_DISPLAY_IND:
	case MSG_CTL_ANIM_GIF_UPDATE:   
		
		break;
	case MSG_KEYDOWN_0:
	case MSG_KEYDOWN_1:
	case MSG_KEYDOWN_2:
	case MSG_KEYDOWN_3:
	case MSG_KEYDOWN_4:
	case MSG_KEYDOWN_5:
	case MSG_KEYDOWN_6:
	case MSG_KEYDOWN_7:
	case MSG_KEYDOWN_8:
	case MSG_KEYDOWN_9:
		if (s_input_timer_id != 0)
		{
			MMK_StopTimer(s_input_timer_id);
			s_input_timer_id = 0;
		} 
		s_input_timer_id = MMK_CreateWinTimer(win_id, DEVICELOCK_SOS_INPUT_DELAY,FALSE);
		MMIDEVICELOCK_GetInputNum(msg_id, input_num);
		if(strlen(s_num_input) >= SOS_INPUT_MAX) 
		{
			SCI_MEMSET(s_num_input, 0x00, sizeof(s_num_input));
			break;
		}
		strcat(s_num_input, input_num);
		if(!MMIAPICC_IsInSosString(s_num_input, MN_DUAL_SYS_1))
		{
			SCI_MEMSET(s_num_input, 0x00, sizeof(s_num_input));
		}else
		{
			SCI_MEMSET(num_input_wstr, 0x00, sizeof(num_input_wstr));
			MMI_STRNTOWSTR(num_input_wstr,
				SOS_INPUT_MAX,
				(uint8 *)s_num_input,
				SOS_INPUT_MAX,
				strlen(s_num_input));
			emergency_str_info.wstr_ptr = num_input_wstr;
			emergency_str_info.wstr_len = MMIAPICOM_Wstrlen(num_input_wstr);
			if(MMIAPICC_IsInputEmergencyCall(&emergency_str_info, &emergency_call_len, MN_DUAL_SYS_1)
				&& emergency_str_info.wstr_len == emergency_call_len)
			{
				MMIAPIIDLE_EditDialWin(msg_id, FALSE, s_num_input, TRUE);
				SCI_MEMSET(s_num_input, 0x00, sizeof(s_num_input));
			}
		}
		break;
	case MSG_TIMER:
		if (*(uint8*)param == s_input_timer_id) 
		{
			SCI_MEMSET(s_num_input, 0x00, sizeof(s_num_input));
		}
		break;
	case MSG_APP_OK:
		if(s_devicelock_code.is_enable)
		{
			uint32* win_info_ptr =(uint32*)SCI_ALLOC_APP(sizeof(uint32));
			if(PNULL != win_info_ptr)
			{
				SCI_MEMSET(win_info_ptr, 0x00, sizeof(uint32));
				*win_info_ptr = MMISET_FUNCKEY_LEFT;
				MMK_CreateWin((uint32 *)MMI_DEVICELOCK_SECURITY_WINTAB, win_info_ptr);
			}
		}
		else
		{
			MMIAPISET_OpenFuncKeyWin(win_id, MMISET_FUNCKEY_LEFT);
		}
		break;
	case MSG_APP_CANCEL:
		if(s_devicelock_code.is_enable)
		{
			uint32* win_info_ptr =(uint32*)SCI_ALLOC_APP(sizeof(uint32));
			if(PNULL != win_info_ptr)
			{
				SCI_MEMSET(win_info_ptr, 0x00, sizeof(uint32));
				*win_info_ptr = MMISET_FUNCKEY_RIGHT;
				MMK_CreateWin((uint32 *)MMI_DEVICELOCK_SECURITY_WINTAB, win_info_ptr);
			}
		}
		else
		{
			MMIAPISET_OpenFuncKeyWin(win_id, MMISET_FUNCKEY_RIGHT);
		}
		break;
	case MSG_KEYUP_RED:
		break;
	case MSG_KEYLONG_RED://锁键盘界面不能关机
	case MSG_KEYDOWN_RED:
		break;
	case MSG_KEYDOWN_WEB:
		if(!MMIAPIDEVICELOCK_EnterInputSecurityWin())
		{
			MMK_CloseWin(win_id);
		}
		break;
	case MSG_KEYDOWN_GREEN:
		break;
	case MSG_KEYDOWN_UP:
		MMK_NeedKbdClkNotifyOnce();
		break;		
	case MSG_KEYDOWN_DOWN:
		break;
	case MSG_KEYDOWN_LEFT:
	case MSG_KEYDOWN_RIGHT:
	case MSG_KEYDOWN_STAR:
	case MSG_KEYDOWN_HASH:
		break;
	case MSG_KEY_DBLCLK:
#ifndef _WIN32
		{
			uint8 key_code = *(uint8*)param;
			if(KEY_UP == key_code)
			{
                MMIACC_SwtichTorch(MMIACC_FLASHLIGHT_MODE_SWITCH);
            }
        }
#endif
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
	
	return recode;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIDEVICELOCK_CreatWallpaperCtrl(void)
{
    GUI_BOTH_RECT_T         anim_ctrl_rect  = MMITHEME_GetFullScreenBothRect();
    BOOLEAN                 result           = TRUE;
    MMIAPIIDLE_SetInitWallpaper_KL(FALSE, MMI_DEVICELOCK_WIN_ID, MMI_DEVICELOCK_ANIM_BG_ID,TRUE);    
    GUIAPICTRL_SetBothRect(MMI_DEVICELOCK_ANIM_BG_ID,&anim_ctrl_rect);
    GUIAPICTRL_SetState(MMI_DEVICELOCK_ANIM_BG_ID, GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
    return (result);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_CreateGrayscaleLayer(void)
{
	GUI_RECT_T          icon_rect = {0};
	UILAYER_CREATE_T    create_info = {0};
	
	if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&grayscale_layer_handle))    
	{
		
		uint16 logic_lcd_width = 0;
		uint16 logic_lcd_height = 0;
		UILAYER_APPEND_BLT_T append_layer_info = {0};
		GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
		
		create_info.lcd_id = GUI_MAIN_LCD_ID;
		create_info.owner_handle = MMI_DEVICELOCK_WIN_ID;
		create_info.offset_x = 0;
		create_info.offset_y = 0;
		create_info.width = logic_lcd_width;
		create_info.height = logic_lcd_height;
		create_info.is_bg_layer = FALSE;
		create_info.is_static_layer = FALSE;
		UILAYER_CreateLayer(
			&create_info,
			&grayscale_layer_handle
			);
		//set color key
		UILAYER_SetLayerColorKey(&grayscale_layer_handle,TRUE,UILAYER_TRANSPARENT_COLOR);
		
		append_layer_info.lcd_dev_info = grayscale_layer_handle;
		append_layer_info.layer_level = UILAYER_LEVEL_NORMAL;
		UILAYER_AppendBltLayer(&append_layer_info);
		
	}
	if (UILAYER_IsLayerActive(&grayscale_layer_handle))
	{
		GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
		
		LCD_FillArgbRect( &grayscale_layer_handle, 
			full_rect, 
			MMI_BLACK_COLOR,
			DEVICELOCK_MASK_LAYER_ALPHA );
	}
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_ReleaseGrayscaleLayer(void)
{
    // 释放层
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&grayscale_layer_handle))    
    {
        UILAYER_RELEASELAYER(&grayscale_layer_handle);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_CreateClockLayer(void)
{
	GUI_RECT_T          icon_rect = {0};
	UILAYER_CREATE_T    create_info = {0};
	
	if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&clock_layer_handle))    
	{
		
		uint16 logic_lcd_width = 0;
		uint16 logic_lcd_height = 0;
		UILAYER_APPEND_BLT_T append_layer_info = {0};
		GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
		
		create_info.lcd_id = GUI_MAIN_LCD_ID;
		create_info.owner_handle = MMI_DEVICELOCK_WIN_ID;
		create_info.offset_x = 0;
		create_info.offset_y = 0;
		create_info.width = logic_lcd_width;
		create_info.height = logic_lcd_height;
		create_info.is_bg_layer = FALSE;
		create_info.is_static_layer = FALSE;
		UILAYER_CreateLayer(
			&create_info,
			&clock_layer_handle
			);
		
		append_layer_info.lcd_dev_info = clock_layer_handle;
		append_layer_info.layer_level = UILAYER_LEVEL_NORMAL;
		UILAYER_AppendBltLayer(&append_layer_info);
		
	}
	if (UILAYER_IsLayerActive(&clock_layer_handle))
	{
		UILAYER_Clear(&clock_layer_handle);
	}
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_ReleaseClockLayer(void)
{
	// 释放层
	if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&clock_layer_handle))    
	{
		UILAYER_RELEASELAYER(&clock_layer_handle);
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_CreateStbLayer(void)
{
	GUI_RECT_T          icon_rect = {0};
	UILAYER_CREATE_T    create_info = {0};
	
	if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&stb_layer_handle))    
	{
		
		uint16 logic_lcd_width = 0;
		uint16 logic_lcd_height = 0;
		UILAYER_APPEND_BLT_T append_layer_info = {0};
		GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
		
		create_info.lcd_id = GUI_MAIN_LCD_ID;
		create_info.owner_handle = MMI_DEVICELOCK_WIN_ID;
		create_info.offset_x = 0;
		create_info.offset_y = 0;
		create_info.width = logic_lcd_width;
		create_info.height = logic_lcd_height;
		create_info.is_bg_layer = FALSE;
		create_info.is_static_layer = FALSE;
		UILAYER_CreateLayer(
			&create_info,
			&stb_layer_handle
			);
		
		append_layer_info.lcd_dev_info = stb_layer_handle;
		append_layer_info.layer_level = UILAYER_LEVEL_NORMAL;
		UILAYER_AppendBltLayer(&append_layer_info);
		GUIWIN_CreateStbDyn(MMI_DEVICELOCK_WIN_ID);
		MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(MMI_DEVICELOCK_WIN_ID, MMICOMMON_STATUSBAR_CTRL_ID), &stb_layer_handle);             
		GUISTATUSBAR_SetBgIsDesplay(MMI_DEVICELOCK_WIN_ID, FALSE);   		
	}
	if (UILAYER_IsLayerActive(&stb_layer_handle))
	{
		UILAYER_Clear(&stb_layer_handle);
	}
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_ReleaseStbLayer(void)
{
	// 释放层
	if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&stb_layer_handle))    
	{
		MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(MMI_DEVICELOCK_WIN_ID,
			MMICOMMON_STATUSBAR_CTRL_ID),  MMITHEME_GetDefaultLcdDev());    
		UILAYER_RELEASELAYER(&stb_layer_handle);
		
	}
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_DisplayClockCallbackFun (GUIANIM_OWNER_DRAW_T  *draw_ptr)
{
	GUI_POINT_T        	 		point = {0};
	GUI_LCD_DEV_INFO			lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
	
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;	
	
	
	GUIRES_DisplayImg(&point,PNULL,PNULL,MMI_DEVICELOCK_WIN_ID,CUSTOM_DEVICE_LOCK ,&clock_layer_handle);
	
}
/******************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_CreateWinCtrlDynamic(void)
{
	GUI_BOTH_RECT_T  anim_ctrl_rect  = MMITHEME_GetFullScreenBothRect();
	MMI_CONTROL_CREATE_T create          = {0};	
	GUIANIM_INIT_DATA_T    anim_init_data = {0};
	GUIANIM_DATA_INFO_T         data_info =    {0};
	GUIANIM_DISPLAY_INFO_T      display_info = {0};
	GUIANIM_CTRL_INFO_T         control_info = {0};
	
	
	
	create.parent_win_handle = MMI_DEVICELOCK_WIN_ID;
	
	anim_init_data.both_rect = anim_ctrl_rect;
	create.init_data_ptr = &anim_init_data;
	
	//MMIDEVICELOCK_CreatWallpaperCtrl();
	
	create.guid = SPRD_GUI_ANIM_ID;
	create.ctrl_id = MMI_DEVICELOCK_CLOCK_ANIM_ID;
	MMK_CreateControl( &create );
	MMK_SetCtrlLcdDevInfo(MMI_DEVICELOCK_CLOCK_ANIM_ID, &clock_layer_handle);
	//data_info.img_id = NULL;
	control_info.is_ctrl_id = TRUE;
	control_info.ctrl_id = MMI_DEVICELOCK_CLOCK_ANIM_ID;
	control_info.ctrl_ptr = PNULL;
	
	display_info.is_syn_decode = TRUE;
	display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
	GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
	GUIANIM_SetOwnerDraw(MMI_DEVICELOCK_CLOCK_ANIM_ID,MMIDEVICELOCK_DisplayClockCallbackFun);
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_DisplayTime2(void)
{
	GUI_POINT_T             point                = {0};
	GUI_LCD_DEV_INFO        lcd_dev_info             = clock_layer_handle;
	SCI_TIME_T         	 		time = {0};
	MMISET_TIME_DISPLAY_TYPE_E  time_display_type = MMIAPISET_GetTimeDisplayType();
	BOOLEAN		       			b_am = TRUE;
	uint16                      image_width = 0,image_height = 0;
	
	
	TM_GetSysTime(&time);
	if(time_display_type == MMISET_TIME_12HOURS)
	{
		GET_12STYLE_HOUR(time.hour, b_am);
	}
	point.x = TIME_AREA_LEFT_MARGIN;
	point.y = TIME_AREA_TOP_MARGIN;
	GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_TIME_0,MMI_DEVICELOCK_WIN_ID);
	
	GUIRES_DisplayImg(&point,PNULL,PNULL,MMI_DEVICELOCK_WIN_ID,IMAGE_IDLE_TIME_0 + time.hour/10,&lcd_dev_info);
	point.x = point.x + image_width;
	GUIRES_DisplayImg(&point,PNULL,PNULL,MMI_DEVICELOCK_WIN_ID,IMAGE_IDLE_TIME_0 + time.hour%10,&lcd_dev_info);
	point.x = point.x + image_width;
	GUIRES_DisplayImg(&point,PNULL,PNULL,MMI_DEVICELOCK_WIN_ID,IMAGE_IDLE_TIME_COLON,&lcd_dev_info);
	//COLON IMAGE WIDTH
	GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_TIME_COLON,MMI_DEVICELOCK_WIN_ID);
	//MIN
	point.x = point.x + image_width;
	GUIRES_DisplayImg(&point,PNULL,PNULL,MMI_DEVICELOCK_WIN_ID,IMAGE_IDLE_TIME_0 + time.min/10,&lcd_dev_info);
	
	GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_TIME_0,MMI_DEVICELOCK_WIN_ID);
	point.x = point.x + image_width;
	GUIRES_DisplayImg(&point,PNULL,PNULL,MMI_DEVICELOCK_WIN_ID,IMAGE_IDLE_TIME_0 + time.min%10,&lcd_dev_info);
	
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_DisplayTime(void)
{
	GUI_POINT_T             point                = {0};
	GUI_LCD_DEV_INFO        lcd_dev_info             = clock_layer_handle;
	SCI_TIME_T         	 		time = {0};
	MMISET_TIME_DISPLAY_TYPE_E  time_display_type = MMIAPISET_GetTimeDisplayType();
	BOOLEAN		       			b_am = TRUE;
	uint8               time_str[10]  = {0};
	wchar               time_wstr[10]    = {0};
	GUISTR_STYLE_T      str_style = {0};
	GUISTR_STATE_T state = {0};
	MMI_STRING_T string = {0};
	uint16 time_height = 0, time_width = 0, ampm_height = 0;
	uint8               ampm_str[10]  = {0};
	wchar               ampm_wstr[10]    = {0};
	
	GUI_RECT_T display_rect = {0};	
	uint16 logic_lcd_width = 0;
	uint16 logic_lcd_height = 0;
	GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
	
	
	TM_GetSysTime(&time);
	if(time_display_type == MMISET_TIME_12HOURS)
	{
		GET_12STYLE_HOUR(time.hour, b_am);
	}
	//point.x = TIME_AREA_LEFT_MARGIN;
	time_height = GUIFONT_GetHeight(DEVICELOCK_TIME_FONT);
	//point.y = TIME_AREA_TOP_MARGIN + TIME_AREA_HEIGHT/2 - time_height/2;
	str_style.align = ALIGN_VMIDDLE;
	str_style.font  = DEVICELOCK_TIME_FONT;
	
	str_style.font_color = MMI_WHITE_COLOR;
	//state = GUISTR_STATE_BOLD | GUISTR_STATE_ALIGN;
	
	
	sprintf((char *)time_str,"%02d:%02d",time.hour,time.min);	
	MMIAPICOM_StrToWstr(time_str, time_wstr); 
	string.wstr_ptr = time_wstr;
	string.wstr_len = MMIAPICOM_Wstrlen(time_wstr);
	
	display_rect.left = TIME_AREA_LEFT_MARGIN;
	display_rect.top = TIME_AREA_TOP_MARGIN;
	display_rect.right = logic_lcd_width - TIME_AREA_LEFT_MARGIN;
	display_rect.bottom = display_rect.top + TIME_AREA_HEIGHT;
	GUISTR_DrawTextToLCDInRect( 
		&lcd_dev_info,
		(const GUI_RECT_T *)&display_rect,       //the fixed display area
		(const GUI_RECT_T *)&display_rect,       //用户要剪切的实际区域
		(const MMI_STRING_T *)&string,
		&str_style,
		state,
		GUISTR_TEXT_DIR_AUTO
		); 
	
	//GUISTR_DrawTextToLCDSingleLine(&lcd_dev_info, &point, &string, &str_style, state);
	
	if(time_display_type == MMISET_TIME_12HOURS)
	{
		if(b_am){
			sprintf((char *)ampm_str,"%s","am");
		}
		else
		{
			sprintf((char *)ampm_str,"%s","pm");
			
		}
		time_width = GUI_GetStringWidth(DEVICELOCK_TIME_FONT, time_wstr,MMIAPICOM_Wstrlen(time_wstr));
		point.x = TIME_AREA_LEFT_MARGIN + time_width +  TIME_AMPM_INTERVAL;
		
		ampm_height = GUIFONT_GetHeight(DEVICELOCK_AMPM_FONT);
		point.y = TIME_AREA_TOP_MARGIN + TIME_AREA_HEIGHT/2 + time_height/2 - ampm_height - 10;
		
		str_style.align = ALIGN_LEFT;
		str_style.font  = DEVICELOCK_AMPM_FONT;
		
		str_style.font_color = MMI_WHITE_COLOR;
		//state = GUISTR_STATE_BOLD | GUISTR_STATE_ALIGN;
		
		MMIAPICOM_StrToWstr(ampm_str, ampm_wstr); 
		string.wstr_ptr = ampm_wstr;
		string.wstr_len = MMIAPICOM_Wstrlen(ampm_wstr);
		GUISTR_DrawTextToLCDSingleLine(&lcd_dev_info, &point, &string, &str_style, state);		
		
	}	
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_NotifactionInsertSort(DEVICELOCK_NOTIFICATION_STRUCT *notification, int num)
{
	int i, j;
	DEVICELOCK_NOTIFICATION_STRUCT t;
	for (i=1; i<num; i++)
	{
		t=*(notification+i);
		for (j=i-1; j>=0 && t.nv.timestamp>(*(notification+j)).nv.timestamp; j--)
		{
			*(notification+j+1) = *(notification+j); 
		}
		*(notification+j+1) = t; 
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_GetDisplayNotifaction(DEVICELOCK_NOTIFICATION_DISPLAY_STRUCT *notification_display)
{
	int i = 0, total = 0;
	DEVICELOCK_NOTIFICATION_STRUCT notification[MMISET_NOTIFICATION_MAX] = {0};
	SCI_ASSERT(notification_display != NULL);
	SCI_MEMSET(notification_display, 0x00, sizeof(DEVICELOCK_NOTIFICATION_DISPLAY_STRUCT));
	
	for(i = MMISET_NOTIFICATION_MESSAGES; i < MMISET_NOTIFICATION_MAX; i++){
		if(s_devicelock_notification[i].visible_enable == TRUE && s_devicelock_notification[i].nv.has_indicator == TRUE)
		{
			notification[total].type= s_devicelock_notification[i].type;
			notification[total].visible_enable= s_devicelock_notification[i].visible_enable;
			notification[total].nv.has_indicator= s_devicelock_notification[i].nv.has_indicator;
			notification[total].nv.icon= s_devicelock_notification[i].nv.icon;
			notification[total].nv.timestamp= s_devicelock_notification[i].nv.timestamp;
			total += 1;
		}
	}
	if(total > 0){
		MMIDEVICELOCK_NotifactionInsertSort(notification, total);
		for(i = 0; i < total && i < MMIDEVICELOCK_NOTIFICATION_DISPLAY_MAX; i++){
			notification_display->index[i] = notification[i].type;
			notification_display->num += 1;
		}
	}
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_DisplayNotifaction(void)
{
	GUI_POINT_T             point                = {0};
	GUI_LCD_DEV_INFO        lcd_dev_info             = clock_layer_handle;
	uint16                      image_width = 0,image_height = 0;
	int i = 0;
	int one_line_num = MMIDEVICELOCK_NOTIFICATION_DISPLAY_MAX/2;
	
	DEVICELOCK_NOTIFICATION_DISPLAY_STRUCT notification_display;
	MMIDEVICELOCK_GetDisplayNotifaction(&notification_display);
	
	GUIRES_GetImgWidthHeight(&image_width,&image_height, CUSTOM_LOCK_SCREEN_N_NEW_MESSAGE,MMI_DEVICELOCK_WIN_ID);
	
	if(notification_display.num > 0 && notification_display.num <= one_line_num){
		point.y = NOTIFICATION_AREA_TOP_MARGIN;
		for(i = notification_display.num - 1; i >= 0; i--)
		{
			point.x = TIME_AREA_LEFT_MARGIN + NOTIFICATION_WIDTH_HEIGHT*i  + NOTIFICATION_INTERVAL_MARGIN*i;
			GUIRES_DisplayImg(&point,PNULL,PNULL,MMI_DEVICELOCK_WIN_ID,s_devicelock_notification[notification_display.index[i]].nv.icon,&lcd_dev_info);
		}
	}
	else if(notification_display.num > one_line_num)
	{
		point.y = NOTIFICATION_AREA_TOP_MARGIN + NOTIFICATION_WIDTH_HEIGHT + NOTIFICATION_INTERVAL_MARGIN;
		for(i = notification_display.num - 1; i >= one_line_num; i--)
		{
			point.x = TIME_AREA_LEFT_MARGIN + NOTIFICATION_WIDTH_HEIGHT*(i - one_line_num)  + NOTIFICATION_INTERVAL_MARGIN*(i - one_line_num);
			GUIRES_DisplayImg(&point,PNULL,PNULL,MMI_DEVICELOCK_WIN_ID,
				s_devicelock_notification[notification_display.index[i]].nv.icon,
				&lcd_dev_info);
		}
		point.y = NOTIFICATION_AREA_TOP_MARGIN;		
		for(i = one_line_num - 1; i >= 0; i--)
		{
			point.x = TIME_AREA_LEFT_MARGIN + NOTIFICATION_WIDTH_HEIGHT*i  + NOTIFICATION_INTERVAL_MARGIN*i;
			GUIRES_DisplayImg(&point,PNULL,PNULL,MMI_DEVICELOCK_WIN_ID,
				s_devicelock_notification[notification_display.index[i]].nv.icon,
				&lcd_dev_info);
		}
		
		
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T MMIDEVICELOCK_GetSoftkeyIcon(MMISET_FUNCKEY_E  index)
{
	MMISET_SHORTCUT_TYPE_E shortcut = MMIAPISET_GetFuncKeyType(index);
	return s_shortcut_list_img[shortcut];
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_DisplayWinSoftkey(void)
{
	GUI_POINT_T             point                = {0};
	GUI_LCD_DEV_INFO        lcd_dev_info             = clock_layer_handle;
	GUI_RECT_T              mainlcd_rect             = MMITHEME_GetFullScreenRect();
	
	uint16                  idle_softkey_icon_width  = 0;
	uint16                  idle_softkey_icon_height = 0; 
	uint32                  i                        = 0;
	MMI_IMAGE_ID_T          idle_softkey_icon[2] = {0};
	
	MMI_TEXT_ID_T  label = STXT_SOFTKEY_UNLOCK_MK;
	MMI_STRING_T        text = {0};
	uint16 mid_softkey_width = 0, mid_softkey_height =0;
	GUISTR_STYLE_T      str_style = {0};
	GUISTR_STATE_T state = {0};
	
	
	idle_softkey_icon[0] = CUSTOM_LOCK_SCREEN_CAMERA;
	idle_softkey_icon[1] = CUSTOM_LOCK_SCREEN_PHONE;
	
	idle_softkey_icon[0] = MMIDEVICELOCK_GetSoftkeyIcon(MMISET_FUNCKEY_LEFT);
	idle_softkey_icon[1] = MMIDEVICELOCK_GetSoftkeyIcon(MMISET_FUNCKEY_RIGHT);

	
	GUIRES_GetImgWidthHeight(&idle_softkey_icon_width, &idle_softkey_icon_height, idle_softkey_icon[0], MMI_DEVICELOCK_WIN_ID);
	
	
	point.x = mainlcd_rect.left + SOFTKEY_ICON_LEFT_OR_RIGHT_MARGIN;//left icon
	point.y = mainlcd_rect.bottom - idle_softkey_icon_height - SOFTKEY_ICON_BOTTOM_MARGIN;// the same height
	GUIRES_DisplayImg(&point,
		PNULL,
		PNULL,
		MMI_DEVICELOCK_WIN_ID,
		idle_softkey_icon[0],
		&lcd_dev_info);
	
	point.x = mainlcd_rect.right  - SOFTKEY_ICON_LEFT_OR_RIGHT_MARGIN - idle_softkey_icon_width;//right icon		
	//point.y = mainlcd_rect.bottom - idle_softkey_icon_height - SOFTKEY_ICON_BOTTOM_MARGIN;// the same height
	GUIRES_DisplayImg(&point,
		PNULL,
		PNULL,
		MMI_DEVICELOCK_WIN_ID,
		idle_softkey_icon[1],
		&lcd_dev_info);
	
	MMI_GetLabelTextByLang(label, &text);
	mid_softkey_width = GUI_GetStringWidth(DEVICELOCK_SOFTKEY_FONT, text.wstr_ptr,text.wstr_len);
	mid_softkey_height = GUIFONT_GetHeight(DEVICELOCK_SOFTKEY_FONT);
	point.x = mainlcd_rect.left;//(mainlcd_rect.right>>1 )- mid_softkey_width;
	point.y = mainlcd_rect.bottom - 6 - mid_softkey_height;
	str_style.align = ALIGN_HMIDDLE;
	str_style.font  = DEVICELOCK_SOFTKEY_FONT;
	
	str_style.font_color = MMI_WHITE_COLOR;
	state = GUISTR_STATE_ALIGN;
	
	GUISTR_DrawTextToLCDSingleLine(&lcd_dev_info, &point, &text, &str_style, state);
	
	
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIDEVICELOCK_GetNotificationIcon(MMISET_NOTIFICATION_TYPE_E n_type)
{
	MMI_IMAGE_ID_T          image_id    =   IMAGE_NULL;
	switch(n_type){
	case MMISET_NOTIFICATION_MESSAGES:
		image_id = CUSTOM_LOCK_SCREEN_N_NEW_MESSAGE;
		break;
	case MMISET_NOTIFICATION_CALLS:
		image_id = CUSTOM_LOCK_SCREEN_N_MISSED_CALL;
		break;
	case MMISET_NOTIFICATION_ALARMS:
		image_id = CUSTOM_LOCK_SCREEN_N_SNOOZED_ALARM;
		break;
	case MMISET_NOTIFICATION_CALENDAR:
		image_id = CUSTOM_LOCK_SCREEN_N_SNOOZED_CALENAR;
		break;
	case MMISET_NOTIFICATION_MAILS:
		image_id = CUSTOM_LOCK_SCREEN_N_VOICE_MAIL;
		break;
	case MMISET_NOTIFICATION_OUTLOOK:
		image_id = CUSTOM_LOCK_SCREEN_N_MAILBOX;
		break;
	case MMISET_NOTIFICATION_OPERATOR:
		image_id = CUSTOM_LOCK_SCREEN_N_OPERATOR_SETTINGS;
		break;
	case MMISET_NOTIFICATION_RADIO:
		image_id = CUSTOM_LOCK_SCREEN_N_FM;
		break;
	case MMISET_NOTIFICATION_MUSIC:
		image_id = CUSTOM_LOCK_SCREEN_N_MUSIC_PLAYER;
		break;
	default:
		//not found
		break;
	}
	return image_id;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDEVICELOCK_InitNotification(void)
{
	MN_RETURN_RESULT_E nv_return = MN_RETURN_SUCCESS;
	MMISET_NOTIFICATION_TYPE_E notifi_type  = MMISET_NOTIFICATION_MESSAGES;
	for(notifi_type = MMISET_NOTIFICATION_MESSAGES; notifi_type < MMISET_NOTIFICATION_MAX; notifi_type++)
	{
		s_devicelock_notification[notifi_type].type = notifi_type;
		s_devicelock_notification[notifi_type].visible_enable = MMISet_GetNotification(notifi_type);
		MMINV_READ(MMINV_DEVICELOCK_NOTIFICATION_MESSAGES+notifi_type, (void *)&s_devicelock_notification[notifi_type].nv, nv_return);
		if (MN_RETURN_SUCCESS != nv_return)
		{
			s_devicelock_notification[notifi_type].nv.has_indicator = FALSE;
			s_devicelock_notification[notifi_type].nv.timestamp = 0; //TM_GetTotalSeconds();
			s_devicelock_notification[notifi_type].nv.icon = MMIAPIDEVICELOCK_GetNotificationIcon(notifi_type);
			MMINV_WRITE(MMINV_DEVICELOCK_NOTIFICATION_MESSAGES+notifi_type, (void *)&s_devicelock_notification[notifi_type].nv);
		}
	}	
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_EnableDisplayNotification
(
 MMISET_NOTIFICATION_TYPE_E notifi_type,
 BOOLEAN enable
 )
{
	s_devicelock_notification[notifi_type].visible_enable = enable;
	if(!enable)
	{
		s_devicelock_notification[notifi_type].nv.has_indicator = FALSE;
		s_devicelock_notification[notifi_type].nv.timestamp = 0; //TM_GetTotalSeconds();
		s_devicelock_notification[notifi_type].nv.icon = MMIAPIDEVICELOCK_GetNotificationIcon(notifi_type);
		MMINV_WRITE(MMINV_DEVICELOCK_NOTIFICATION_MESSAGES+notifi_type, (void *)&s_devicelock_notification[notifi_type].nv);
	}
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_SetDisplayNotification
(
 MMISET_NOTIFICATION_TYPE_E notifi_type,
 BOOLEAN display,
 BOOLEAN update_timestamp,
 MMI_IMAGE_ID_T icon_id
 )
{
	if(s_devicelock_notification[notifi_type].visible_enable)
	{
		if(s_devicelock_notification[notifi_type].nv.has_indicator != display)
		{
			s_devicelock_notification[notifi_type].nv.has_indicator = display;
			if(display)
			{
				s_devicelock_notification[notifi_type].nv.timestamp = TM_GetTotalSeconds();
				if(IMAGE_NULL != icon_id)
				{
					s_devicelock_notification[notifi_type].nv.icon = icon_id;
				}
				else
				{
					s_devicelock_notification[notifi_type].nv.icon = MMIAPIDEVICELOCK_GetNotificationIcon(notifi_type);
				}
			}
			else
			{
				s_devicelock_notification[notifi_type].nv.timestamp = 0;
				s_devicelock_notification[notifi_type].nv.icon = MMIAPIDEVICELOCK_GetNotificationIcon(notifi_type);
			}
			MMINV_WRITE(MMINV_DEVICELOCK_NOTIFICATION_MESSAGES+notifi_type, (void *)&s_devicelock_notification[notifi_type].nv);
		}
		else
		{
			if(display && update_timestamp)
			{
				s_devicelock_notification[notifi_type].nv.timestamp = TM_GetTotalSeconds();
				MMINV_WRITE(MMINV_DEVICELOCK_NOTIFICATION_MESSAGES+notifi_type, (void *)&s_devicelock_notification[notifi_type].nv);
			}
		}
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEVICELOCK_ReadPwd(MMI_DEVICELOCK_PWD_T* devicelock_code )
{
	MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;
	uint8 pwd_num[MMI_DEVICELOCK_PWD_MAX_LEN] =   {0};
	
	SCI_MEMSET(devicelock_code, 0 , sizeof(MMI_DEVICELOCK_PWD_T));
	MMINV_READ(MMINV_DEVICELOCK_PWD,devicelock_code,return_value);
	if (MN_RETURN_SUCCESS != return_value)
	{
		return FALSE;
	}
	return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEVICELOCK_SetPwd(const wchar *pwd_ptr, uint16 pwd_len)
{
	if(PNULL == pwd_ptr)
	{
		return FALSE;
	}
	s_devicelock_code.pwd_len = (uint8)(pwd_len & 0x00FF);
	MMI_WSTRNTOSTR((uint8 *)s_devicelock_code.pwd,MMI_DEVICELOCK_PWD_MAX_LEN,pwd_ptr,pwd_len,pwd_len);
	MMINV_WRITE(MMINV_DEVICELOCK_PWD,&s_devicelock_code);
	return TRUE;
 }

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEVICELOCK_SetPwdEnable(BOOLEAN is_enable)
{
	s_devicelock_code.is_enable = is_enable;
	MMINV_WRITE(MMINV_DEVICELOCK_PWD,&s_devicelock_code);
	return TRUE;
 }

#if 0
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_InitPwd(MMI_DEVICELOCK_PWD_T* devicelock_code )
{
	MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;
	uint8 pwd_num[MMI_DEVICELOCK_PWD_MAX_LEN] =   {0};
	
	SCI_MEMSET(devicelock_code, 0 , sizeof(MMI_DEVICELOCK_PWD_T));
	MMINV_READ(MMINV_DEVICELOCK_PWD,devicelock_code,return_value);
	if (MN_RETURN_SUCCESS != return_value)
	{
		SCI_MEMSET(devicelock_code,0,sizeof(MMI_DEVICELOCK_PWD_T));
		sprintf((char*)pwd_num, "%s" , MMI_DEVICELOCK_INIT_PWD);
		devicelock_code->pwd_len = strlen((char*)pwd_num);
		if(devicelock_code->pwd_len >= MMI_DEVICELOCK_PWD_MAX_LEN)
		{
			devicelock_code->pwd_len= MMI_DEVICELOCK_PWD_MAX_LEN;
		}
		SCI_MEMCPY(devicelock_code->pwd, pwd_num, devicelock_code->pwd_len);  
		MMINV_WRITE(MMINV_DEVICELOCK_PWD,devicelock_code);
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIDEVICELOCK_ResetPwdStatus(void)
{
	MMI_DEVICELOCK_PWD_T   devicelock_code = {0};
	uint8 pwd_num[MMI_DEVICELOCK_PWD_MAX_LEN] =   {0};
	
	SCI_MEMSET(&devicelock_code,0,sizeof(MMI_DEVICELOCK_PWD_T));
	sprintf((char*)pwd_num, "%s" , MMI_DEVICELOCK_INIT_PWD);
	devicelock_code.pwd_len = strlen((char*)pwd_num);
	
	if(devicelock_code.pwd_len >= MMI_DEVICELOCK_PWD_MAX_LEN)
	{
		devicelock_code.pwd_len = MMI_DEVICELOCK_PWD_MAX_LEN;
	}
	
	SCI_MEMCPY(devicelock_code.pwd, pwd_num, devicelock_code.pwd_len);  
	MMINV_WRITE(MMINV_DEVICELOCK_PWD,&devicelock_code);
}
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEVICELOCK_IsCorrectPwd(const wchar *pwd_ptr, uint16 pwd_len)
{
	BOOLEAN                 result = FALSE;
	
	if(PNULL == pwd_ptr)
	{
		return result;
	}
	if( pwd_len == s_devicelock_code.pwd_len
		&& 0 == MMIAPICOM_WstrStrncmp( pwd_ptr, (uint8 *)s_devicelock_code.pwd, pwd_len ) )
	{
		result = TRUE;
	}
	return (result);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIDEVICELOCK_CreatSecurityAnimCtrl(void)
{
	GUI_RECT_T         anim_ctrl_rect  = MMITHEME_GetClientRect();
	BOOLEAN                 result           = TRUE;
	GUIANIM_CTRL_INFO_T     control_info = {0};
	GUIANIM_DATA_INFO_T     data_info = {0};
	GUIANIM_DISPLAY_INFO_T  display_info = {0};
	GUI_RECT_T              mainlcd_rect             = MMITHEME_GetFullScreenRect();
	
	
	anim_ctrl_rect.bottom = mainlcd_rect.bottom - SECURITY_ANIM_BOTTOM_MARGIN;
	anim_ctrl_rect.top = anim_ctrl_rect.bottom - SECURITY_ANIM_WIDTH_HEIGHT;
	anim_ctrl_rect.right = mainlcd_rect.right - SECURITY_ANIM_RIGHT_MARGIN;	
	anim_ctrl_rect.left = anim_ctrl_rect.right - SECURITY_ANIM_WIDTH_HEIGHT;
	
	//MMIAPIIDLE_SetInitWallpaper_KL(FALSE, MMI_DEVICELOCK_SECURITY_WIN_ID, MMI_DEVICELOCK_SECURITY_BG_ID,TRUE);    
	GUIAPICTRL_SetRect(MMI_DEVICELOCK_SECURITY_ANIM_ID,&anim_ctrl_rect);
	GUIAPICTRL_SetState(MMI_DEVICELOCK_SECURITY_ANIM_ID, GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
	
	
	//set anim param
	control_info.is_ctrl_id = TRUE;
	control_info.ctrl_id = MMI_DEVICELOCK_SECURITY_ANIM_ID;
	
	data_info.img_id = CUSTOM_DEVICE_LOCK;
	
	display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
	display_info.bg.bg_type = GUI_BG_COLOR;
	display_info.bg.color   = MMI_TRANSPARENCE_COLOR;
	
	GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
	
	return (result);
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDEVICELOCK_PwdInputWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      )
{
	BOOLEAN  result = FALSE;
	BOOLEAN is_input_emergency_call = FALSE;
	uint16 emergency_call_len = 0;
	MMI_STRING_T str_info = {0};
	
	//set string info
	str_info.wstr_ptr = str_ptr;
	str_info.wstr_len = str_len;
	
	is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MN_DUAL_SYS_1);//开机密码界面，未启动SIM卡，输入卡1即可
	if ((is_input_emergency_call) &&
		(str_len == emergency_call_len))
	{
		*text_id_ptr = STR_LOCK_SOFTKEY_CALL_MK;
		result = TRUE;
	}
	else if (str_len >= 1)
	{
		*text_id_ptr = TXT_COMMON_OK;
		result = TRUE;
	}
	
	return (result);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIDEVICELOCK_DisplayEmergencyNumPrompt(BOOLEAN display)
{
	GUI_LCD_DEV_INFO        lcd_dev_info             = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T display_rect = {0};	
	GUI_RECT_T client_rect = MMITHEME_GetClientRect();	
	GUISTR_STYLE_T      str_style = {0};
	GUISTR_STATE_T state = {0};
	MMI_STRING_T string = {0};
	
	str_style.align = ALIGN_HVMIDDLE;
	str_style.font  = DEVICELOCK_SOFTKEY_FONT;
	
	str_style.font_color = MMI_DIAL_GRAY_COLOR;
	//state = GUISTR_STATE_BOLD | GUISTR_STATE_ALIGN;
	
	MMI_GetLabelTextByLang(STR_LOCK_EMERGENCY_NUM_PROMPT, &string);
	display_rect.left = 0;
	display_rect.top = client_rect.bottom - SECURITY_EMERGENCY_NUM_PROMPT_HEITHT;
	display_rect.right = client_rect.right;
	display_rect.bottom = client_rect.bottom;
	if(display)
	{
		GUISTR_DrawTextToLCDInRect( 
			&lcd_dev_info,
			(const GUI_RECT_T *)&display_rect,       //the fixed display area
			(const GUI_RECT_T *)&display_rect,       //用户要剪切的实际区域
			(const MMI_STRING_T *)&string,
			&str_style,
			state,
			GUISTR_TEXT_DIR_AUTO
			); 
	}
	else
	{
		LCD_FillRect(&lcd_dev_info, display_rect, MMI_BLACK_COLOR);
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIDEVICELOCK_HandleSecurityWinMsg(
													 MMI_WIN_ID_T      win_id,
													 MMI_MESSAGE_ID_E     msg_id,
													 DPARAM               param
													 )
{   
	MMI_STRING_T pwd_value = {0};  
	MMI_STRING_T pwd_value_cnf = {0};  
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	GUI_RECT_T edit_rect = {0};
	GUI_RECT_T mainlcd_rect  = MMITHEME_GetFullScreenRect();
	uint16 emergency_call_len = 0;
	uint32 *win_info_ptr = (uint32 *)MMK_GetWinAddDataPtr(win_id);

	switch (msg_id)
	{
	case MSG_GET_FOCUS:
		break;
	case MSG_OPEN_WINDOW:	
		GUIEDIT_SetStyle(MMI_DEVICELOCK_SECURITY_PWD_ID,GUIEDIT_STYLE_MULTI_DYN_DOWN);
		edit_rect.left = 0;
		edit_rect.bottom = mainlcd_rect.bottom - SECURITY_PWD_BOTTOM_MARGIN;
		edit_rect.top = edit_rect.bottom - SECURITY_PWD_HEIGHT;
		edit_rect.right = mainlcd_rect.right;
		CTRLBASEEDIT_SetLabelId(win_id,MMI_DEVICELOCK_SECURITY_PWD_ID,STR_SECUR_CODE_QUERY_EXT02,FALSE);
		GUIAPICTRL_SetRect(MMI_DEVICELOCK_SECURITY_PWD_ID, &edit_rect);
		GUIEDIT_SetIm (MMI_DEVICELOCK_SECURITY_PWD_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
		
		//GUIEDIT_SetSoftkey(MMI_DEVICELOCK_SECURITY_PWD_ID,0,0,TXT_NULL,TXT_NULL,PNULL);
		GUIEDIT_SetSoftkey(MMI_DEVICELOCK_SECURITY_PWD_ID,1,1,TXT_NULL,TXT_COMMON_OK,MMIDEVICELOCK_PwdInputWinSoftkey);
		GUIEDIT_SetSoftkeyEx(MMI_DEVICELOCK_SECURITY_PWD_ID, 
			2, 
			0,
			TXT_COMMON_COMMON_BACK,
			TXT_COMMON_COMMON_BACK,
			PNULL
			PNULL, 
			EDIT_BUTTON_TEXT_ID,
			PNULL
			);
		MMK_SetAtvCtrl(win_id,MMI_DEVICELOCK_SECURITY_PWD_ID);
		break;
	case MSG_FULL_PAINT:
		{
			GUI_RECT_T rect = MMITHEME_GetClientRect();
			GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
			rect.top -= 10;
			LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
			GUIWIN_SetStbItemTextValid(0, FALSE);
		}
		MMIDEVICELOCK_CreatSecurityAnimCtrl();
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
		GUIEDIT_GetString(MMI_DEVICELOCK_SECURITY_PWD_ID,&pwd_value);
		if(MMIAPICC_IsInputEmergencyCall(&pwd_value, &emergency_call_len, MN_DUAL_SYS_1)
			&& pwd_value.wstr_len == emergency_call_len)
		{
			uint8 temp_str[CC_MAX_TELE_NUM_LEN + 1] = {0};
			uint32 multi_sys_num = 0;
			uint16 multi_sys = MN_DUAL_SYS_1;
			MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
			
			MMI_WSTRNTOSTR((uint8 *)temp_str, 
				pwd_value.wstr_len,
				pwd_value.wstr_ptr,
				pwd_value.wstr_len,
				pwd_value.wstr_len);
			multi_sys_num = MMIAPISET_GetMultiSysSettingNum(&multi_sys, 1);
			
			if( 0 == multi_sys_num)
			{
				dual_sys = MN_DUAL_SYS_1;
			}
			else
			{
				dual_sys = (MN_DUAL_SYS_E)multi_sys;
			}
			
			MMIAPICC_MakeCall(
				dual_sys,
				temp_str,
				(uint8)(pwd_value.wstr_len),
				PNULL,
				PNULL,
				MMIAPICC_GetCallSimType(dual_sys),
				CC_CALL_NORMAL_CALL,
				PNULL
				);
			MMK_CloseWin(win_id);
		}
		else
		{
			if (MMIDEVICELOCK_IsCorrectPwd(pwd_value.wstr_ptr, pwd_value.wstr_len))
			{
				MMICOM_BANNER_INFO_T banner_info = {0};
				MMIAPIDEVICELOCK_UnlockPhone();			
				MMK_CloseWin(win_id);
				if(PNULL != win_info_ptr)
				{
					MMIAPISET_OpenFuncKeyWin(win_id, *win_info_ptr);
				}
				banner_info.banner_type = MMI_BANNER_TEXT;
				banner_info.start_point.x = 0;
				banner_info.start_point.y = 0;
				banner_info.text1_id = STR_LOCK_CODE_APPR_EXT01;
				MMIAPICOM_OpenBannerWin(win_id, banner_info);
			}
			else
			{
				MMICOM_BANNER_INFO_T banner_info = {0};
				banner_info.banner_type = MMI_BANNER_TEXT;
				banner_info.start_point.x = 0;
				banner_info.start_point.y = 0;
				banner_info.text1_id = STR_LOCK_CODE_REJE_EXT01;
				GUIEDIT_ClearAllStr(MMI_DEVICELOCK_SECURITY_PWD_ID);
				MMIAPICOM_OpenBannerWin(win_id, banner_info);
			}
		}
		break;
		
	case MSG_CTL_EDITBOX_UPDATE_STRNUM:
		GUIEDIT_GetString(MMI_DEVICELOCK_SECURITY_PWD_ID,&pwd_value);
		if(MMIAPICC_IsInputEmergencyCall(&pwd_value, &emergency_call_len, MN_DUAL_SYS_1)
			&& pwd_value.wstr_len == emergency_call_len)
		{
			CTRLBASEEDIT_SetDispPassword(win_id,MMI_DEVICELOCK_SECURITY_PWD_ID,TRUE,TRUE);
			MMIDEVICELOCK_DisplayEmergencyNumPrompt(TRUE);
		}
		else
		{
			CTRLBASEEDIT_SetDispPassword(win_id,MMI_DEVICELOCK_SECURITY_PWD_ID,FALSE,TRUE);
			MMIDEVICELOCK_DisplayEmergencyNumPrompt(FALSE);
		}
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;

	case MSG_CLOSE_WINDOW:
		if(PNULL!= win_info_ptr)
		{
			MMK_FreeWinAddData(win_id);
		}
		break;

	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	
	return (result);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_EnterInputSecurityWin(void)
{
	if(s_devicelock_code.is_enable)
	{
		MMK_CreateWin((uint32 *)MMI_DEVICELOCK_SECURITY_WINTAB, PNULL);
		return TRUE;
	}
	return FALSE;
}

/*****************************************************************************/
//  Description : lock phone
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_LockPhone(void)
{
    MMI_WIN_ID_T    win_id  = MMI_DEVICELOCK_WIN_ID;
	
    if (!MMK_IsOpenWin(win_id))
    {
        MMK_CreateWin((uint32 *)MMI_DEVICELOCK_WINTAB, PNULL);
    }
    else
    {
        MMK_WinGrabFocus(win_id);
    }
    
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_IsPhoneLocked(void)
{
        return MMK_IsOpenWin(MMI_DEVICELOCK_WIN_ID);
}

/*****************************************************************************/
//  Description : unlock phone
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_UnlockPhone(void)
{
    MMI_COMMON_WINDOW_ID_E  win_id = MMI_DEVICELOCK_WIN_ID;

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    else
    {
        //do nothing
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : unlock phone
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_IsDeviceLockWinOnTop(void)
{
    return MMK_IsFocusWin(MMI_DEVICELOCK_WIN_ID);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDEVICELOCK_SendHourOrMinuteInd(void)
{
    if (MMK_IsFocusWin(MMI_DEVICELOCK_WIN_ID))
    {
        MMK_SendMsg(MMI_DEVICELOCK_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDEVICELOCK_Init(void)
{
    MMIAPIDEVICELOCK_InitNotification();  
    if(!MMIDEVICELOCK_ReadPwd(&s_devicelock_code))
    {
        s_devicelock_code.is_enable == FALSE;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDEVICELOCK_InitModule(void)
{
    MMIDEVICELOCK_RegMenuGroup();  
    MMIDEVICELOCK_RegNv();   
    MMIDEVICELOCK_RegWinIdNameArr();
}

