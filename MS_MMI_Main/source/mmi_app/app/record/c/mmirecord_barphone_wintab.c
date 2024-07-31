/*****************************************************************************
** File Name:      mmirecord_wintab.c                                        *
** Author:         bin.ji                                                    *
** Date:           3/1/2005                                                  *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 3/1/2005       bin.ji           Create                                    *
** 8/11/2006      haiyang.hu       Modify                                    *
******************************************************************************/

#define _MMIRECORD_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_menutable.h"
#include "guilistbox.h"
#include "guianim.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guicommon.h"
#include "guilcd.h"
#include "guilabel.h"
#include "mmi_appmsg.h"
#include "mmirecord_id.h"
#include "mmirecord_image.h"
#include "mmirecord_text.h"
#include "mmirecord_menutable.h"
#include "guiref_scale.h"
#include "mmirecord_position.h"
#include "mmi_image.h"
#include "guibutton.h"
#include "graphics_draw.h"
#include "dal_img.h"
#include "mmimultim_image.h"
#include "mmirecord_wintablel.h"
#include "mmirecord_appcontrol.h"
#include "mmirecord_main.h"
#include "guires.h"
#include "mmisrvfmm_export.h"
#include "mmiset_export.h"
#include "mmitheme_pos.h"
#include "mmidisplay_data.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern BOOLEAN is_delete_record;
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
extern uint32 total_record_time;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : HandleWebKeyMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWebKeyMsg(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : Recorder Main Play win
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRecordMainPlayWinMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
);

/*****************************************************************************/
//  Description : display owner draw anim
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordOwnerDrawAnim(MMI_WIN_ID_T win_id);


/*****************************************************************************/
//  Description : display record time
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordTime(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : SetNameLabelText
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetNameLabelText(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : SetTimeLabelText
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetTimeLabelText(uint32 cur_time_elapsed, uint32 total_time);

/*****************************************************************************/
//  Description : SetFormParam
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFormParam(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : SetPortraitAndLandscapeFormParam
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetPortraitAndLandscapeFormParam(MMI_WIN_ID_T win_id);


/*****************************************************************************/
//  Description : display record pointer
//  Global resource dependence : none
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordPointer(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : display progress
//  Global resource dependence : none
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void DisplayProgress(MMI_HANDLE_T win_handle);


/*****************************************************************************/
//  Description : DisplayRecordTimeImg
//  Global resource dependence :
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordTimeImg(MMI_HANDLE_T win_handle , GUI_RECT_T *rect_ptr, uint8 time_num);


/******************************************************************************/
//  Description: decompressed the ARGB image
//  Author: chunyou.zhu
//  Input:
//          src_img_ptr:	pointer of input image information
//			dst_buf_ptr:	pointer to the target buffer
//			dst_buf_size:	target buffer size
//  Output:
//			dst_img_ptr:	pointer to the decompressed image information
//  Return:
//			TRUE:			successful
//          FALSE:			failed
//  Note:
/******************************************************************************/
LOCAL BOOLEAN RecordDecompressARGB(const MMIRECORD_IMAGE_T *src_img_ptr, MMIRECORD_IMAGE_T *dst_img_ptr,
                                   uint8 *dst_buf_ptr, uint32 dst_buf_size);

/*****************************************************************************/
//  Description : RecordCreateLayer
//  Global resource dependence :
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void RecordCreateLayer(MMI_WIN_ID_T      win_id);

/*****************************************************************************/
//  Description : RecordReleaseLayer
//  Global resource dependence :
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void RecordReleaseLayer(MMI_WIN_ID_T      win_id);


/*****************************************************************************/
//  Description : GetAnimalFrame
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetAnimalFrame(uint32 cur_frame_index);

/*****************************************************************************/
//  Description : update vol icon
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void UpdateVolIcon(MMI_HANDLE_T win_handle) ;

/*****************************************************************************/
//  Description : DisplayRecordAllBG
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordAllBG(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : display softkey
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplaySoftkey(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : get image id for record time
//  Global resource dependence :
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetTimeNumImageId(uint8 time_num);

/**--------------------------------------------------------------------------*
**                          WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIRECORD_MAINPLAY_WIN_CTRL_TAB) =
{
#if 1//def MMI_RECORD_MINI_SUPPORT
    WIN_TITLE(TXT_RECORDER),
#endif
//#ifndef MMI_RECORD_MINI_SUPPORT
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIRECORD_NAME_LABEL_CTRL_ID),
//#endif
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIRECORD_PLAY_TIME_LABEL_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMIRECORD_PLAY_ALL_TIME_LABEL_CTRL_ID),

    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};

/*****************************************************************************/
//  Description : Anim
//  Global resource dependence : none
//  Author:dong.chunguang
//  Note: 
/*****************************************************************************/
LOCAL void MMIRECORD_CreateLabel(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,GUI_RECT_T *rect)
{
    //GUI_RECT_T   			rect = {0};
    GUI_BG_T    			label_bg = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    GUILABEL_INIT_DATA_T    init_data = {0};
    GUILABEL_ALIGN_E        align = GUILABEL_ALIGN_MIDDLE;
	GUI_LCD_DEV_INFO 		dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    //set init data
    init_data.align = align;
    init_data.both_rect.v_rect = *rect;
    init_data.both_rect.h_rect = *rect;
    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_LABEL_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;
    MMK_CreateControl( &create );
    MMK_SetCtrlLcdDevInfo(ctrl_id, &dev_info);

    label_bg.bg_type = GUI_BG_COLOR;
    label_bg.color = MMI_BLACK_COLOR;

    GUILABEL_SetBg(ctrl_id,&label_bg);  
#if defined (MAINLCD_SIZE_128X160)
    GUILABEL_SetFont(ctrl_id, SONG_FONT_22,MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT));
#else
    GUILABEL_SetFont(ctrl_id, SONG_FONT_26,MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT));
#endif
}

/*****************************************************************************/
//  Description : GetAnimalFrame
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetAnimalFrame(uint32 cur_frame_index)
{
#ifdef MMI_COMPILE_E
    MMI_IMAGE_ID_T  anim_frame_BLUE[RECORD_ANIMAL_TOTAL_FRAME_NUM] = {IMAGE_RECORD_BLUE_1, IMAGE_RECORD_BLUE_2, IMAGE_RECORD_BLUE_3, IMAGE_RECORD_BLUE_4,
																IMAGE_RECORD_BLUE_5,IMAGE_RECORD_BLUE_6,IMAGE_RECORD_BLUE_7,IMAGE_RECORD_BLUE_8,
																IMAGE_RECORD_BLUE_9,IMAGE_RECORD_BLUE_10};
#else//8910UIX
    MMI_IMAGE_ID_T  anim_frame_red[RECORD_ANIMAL_TOTAL_FRAME_NUM] = {IMAGE_RECORD_RED_1, IMAGE_RECORD_RED_2, IMAGE_RECORD_RED_3, IMAGE_RECORD_RED_4,
																IMAGE_RECORD_RED_5,IMAGE_RECORD_RED_6,IMAGE_RECORD_RED_7,IMAGE_RECORD_RED_8,
																IMAGE_RECORD_RED_9,IMAGE_RECORD_RED_10};
#endif
/*
    MMI_IMAGE_ID_T  anim_frame_copper[RECORD_ANIMAL_TOTAL_FRAME_NUM] = {IMAGE_RECORD_COPPER_1, IMAGE_RECORD_COPPER_2, IMAGE_RECORD_COPPER_3, IMAGE_RECORD_COPPER_4,
																IMAGE_RECORD_COPPER_5,IMAGE_RECORD_COPPER_6,IMAGE_RECORD_COPPER_7,IMAGE_RECORD_COPPER_8,
																IMAGE_RECORD_COPPER_9,IMAGE_RECORD_COPPER_10};

    MMI_IMAGE_ID_T  anim_frame_green[RECORD_ANIMAL_TOTAL_FRAME_NUM] = {IMAGE_RECORD_GREEN_1, IMAGE_RECORD_GREEN_2, IMAGE_RECORD_GREEN_3, IMAGE_RECORD_GREEN_4,
																IMAGE_RECORD_GREEN_5,IMAGE_RECORD_GREEN_6,IMAGE_RECORD_GREEN_7,IMAGE_RECORD_GREEN_8,
																IMAGE_RECORD_GREEN_9,IMAGE_RECORD_GREEN_10};

    MMI_IMAGE_ID_T  anim_frame_red[RECORD_ANIMAL_TOTAL_FRAME_NUM] = {IMAGE_RECORD_RED_1, IMAGE_RECORD_RED_2, IMAGE_RECORD_RED_3, IMAGE_RECORD_RED_4,
																IMAGE_RECORD_RED_5,IMAGE_RECORD_RED_6,IMAGE_RECORD_RED_7,IMAGE_RECORD_RED_8,
																IMAGE_RECORD_RED_9,IMAGE_RECORD_RED_10};

    MMI_IMAGE_ID_T  anim_frame_violet[RECORD_ANIMAL_TOTAL_FRAME_NUM] = {IMAGE_RECORD_VIOLET_1, IMAGE_RECORD_VIOLET_2, IMAGE_RECORD_VIOLET_3, IMAGE_RECORD_VIOLET_4,
																IMAGE_RECORD_VIOLET_5,IMAGE_RECORD_VIOLET_6,IMAGE_RECORD_VIOLET_7,IMAGE_RECORD_VIOLET_8,
																IMAGE_RECORD_VIOLET_9,IMAGE_RECORD_VIOLET_10};

    MMI_IMAGE_ID_T  anim_frame_yellow[RECORD_ANIMAL_TOTAL_FRAME_NUM] = {IMAGE_RECORD_YELLOW_1, IMAGE_RECORD_YELLOW_2, IMAGE_RECORD_YELLOW_3, IMAGE_RECORD_YELLOW_4,
																IMAGE_RECORD_YELLOW_5,IMAGE_RECORD_YELLOW_6,IMAGE_RECORD_YELLOW_7,IMAGE_RECORD_YELLOW_8,
																IMAGE_RECORD_YELLOW_9,IMAGE_RECORD_YELLOW_10};
*/
    if(cur_frame_index < RECORD_ANIMAL_TOTAL_FRAME_NUM)
    {
        //if (MMITHEME_ACCENT_COLOR_BLUE == MMITHEME_GetAccentId())
        {
#ifdef MMI_COMPILE_E
            return anim_frame_BLUE[cur_frame_index];
#else//8910UIX
            return anim_frame_red[cur_frame_index];
#endif
        }
 /*       else if(MMITHEME_ACCENT_COLOR_COPPER== MMITHEME_GetAccentId())
        {
            return anim_frame_copper[cur_frame_index];
        }
        else if(MMITHEME_ACCENT_COLOR_GREEN== MMITHEME_GetAccentId())
        {
            return anim_frame_green[cur_frame_index];
        }
        else if(MMITHEME_ACCENT_COLOR_RED== MMITHEME_GetAccentId())
        {
            return anim_frame_red[cur_frame_index];
        }
        else if(MMITHEME_ACCENT_COLOR_VIOLENT== MMITHEME_GetAccentId())
        {
            return anim_frame_violet[cur_frame_index];
        }
        else if(MMITHEME_ACCENT_COLOR_YELLOW== MMITHEME_GetAccentId())
        {
            return anim_frame_yellow[cur_frame_index];
        }*/
    }

    return 0;
}
/*****************************************************************************/
//  Description : Recorder Main Play win
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIRECORD_HandleRecordMainPlayWinMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);
#if 0
	GUI_BG_T    common_bg = {0};
#endif
    if(PNULL == record_win_data_ptr)
    {
        return MMI_RESULT_TRUE;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_CreateCtrlByWinTabEx(win_id, MMIRECORD_MAINPLAY_WIN_CTRL_TAB);

        if(!SetFormParam(win_id))
        {
            MMK_CloseWin(win_id);
        }

        RecordCreateLayer(win_id);
        MMIRECORD_UpdateButtonStatusReady(record_win_data_ptr->win_handle);
#if 0		
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_LIST, LEFT_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_RECORD, MIDDLE_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
	    common_bg.bg_type = GUI_BG_COLOR;
	    common_bg.color = MMI_BLACK_COLOR;
	    GUIWIN_SetSoftkeyCommonBg(win_id, &common_bg);
	    GUIWIN_SetSoftkeyBgAlpha(win_id,255);
	    GUIWIN_SetSoftkeyStyle(win_id,MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT),GUI_SOFTKEY_BG_COLOR_ONLY,MMI_BLACK_COLOR,0);
#endif
        break;

    case MSG_FULL_PAINT:    //显示背景图，音量调节杆
    	{
			MMI_STRING_T    temp_str = {0};
			/*MMI_GetLabelTextByLang(STXT_HEADER_VOICE_REC, &temp_str);
			if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len)) {
				GUIWIN_UpdateStb();
			}*/
    	}
        MMIRECORD_UpdateScreen(record_win_data_ptr->win_handle, TRUE, TRUE);
        break;

    case MSG_UPDATE_VOL_ICON:
        UpdateVolIcon(record_win_data_ptr->win_handle);
        break;

    case MSG_RECORD_FILE_DELETED:
        MMIRECORD_UpdateButtonStatusReady(record_win_data_ptr->win_handle);
        MMIRECORD_UpdateScreen(win_id, FALSE, TRUE);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
        {
            MMK_PostMsg(win_id, MSG_PAUSE_RECORD, PNULL, PNULL);
            is_delete_record = TRUE;
			//MMK_PostMsg(win_id, MSG_PAUSE_RECORD, PNULL, PNULL);
        }
		/*
        else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
        {
            MMK_PostMsg(win_id, MSG_RECORD_STOP_PLAY, PNULL, PNULL);
        }
        */
        else
        {
            total_record_time = 0;
            MMK_CloseWin(win_id);
        }

        break;


    case MSG_CLOSE_WINDOW:
        RecordReleaseLayer(win_id);
        break;

    case MSG_LCD_SWITCH:
        SetFormParam(win_id);
        RecordReleaseLayer(win_id);
        RecordCreateLayer(win_id);
        break;

    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_APP_UP:
    case MSG_APP_DOWN:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : DisplayRecordAllBG
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordAllBG(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T display_rect = {0};
    GUI_RECT_T image_rect = {0};
	GUI_RECT_T time_rect = {0};
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }
    image_rect.left = 0;
    image_rect.top = MMI_TITLE_THEME_HEIGHT;//MMI_STATUSBAR_HEIGHT;//change by James li
    image_rect.right = MMI_MAINSCREEN_WIDTH;
    image_rect.bottom = MMI_MAINSCREEN_HEIGHT;

    LCD_FillRect(&lcd_dev_info, image_rect,MMI_BLACK_COLOR);

    //3.IMAGE_RECORD_NUMBER_0;record_time
    time_rect.left = 0;
	time_rect.right = MMI_MAINSCREEN_WIDTH;
#if defined (MAINLCD_SIZE_128X160)
    time_rect.top = 21;
    time_rect.bottom = 39;
#else
    time_rect.top = 40;//26;
    time_rect.bottom = 68;//64;
#endif

	MMIRECORD_CreateLabel(win_id,MMIRECORD_TIME_LABEL_CTRL_ID,&time_rect);

    display_rect.left = record_win_data_ptr->position.record_record_bg_x;
    display_rect.top = record_win_data_ptr->position.record_record_bg_y;
    display_rect.right = display_rect.left  + record_win_data_ptr->position.record_record_bg_w - 1;
    display_rect.bottom =  display_rect.top + record_win_data_ptr->position.record_record_bg_h - 1;
    GUIRES_DisplayImg(PNULL,
                      &display_rect,
                      NULL,
                      win_id,
                      IMAGE_RECORD_INDICATOR,
                      &lcd_dev_info);
}


/*****************************************************************************/
//  Description : display record
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateScreen(MMI_WIN_ID_T win_id,
                                   BOOLEAN is_full_paint, BOOLEAN is_update_panel)
{
    if(!MMK_IsFocusWin(win_id))
    {
        if(!is_full_paint)
        {
            return;
        }
    }
	//if (!((MMIRECORD_STATE_PLAYING == MMIRECORD_GetRecordState()) || 
    //    (MMIRECORD_STATE_PLAY_PAUSED == MMIRECORD_GetRecordState()) || 
	//	(MMIRECORD_STATE_STOPPED == MMIRECORD_GetRecordState()))
	//	)
	//{
		//DisplayRecordTime(win_id);
	//}
    
    //DisplayRecordPointer(win_id);
    //DisplayProgress(win_id);
    //SetNameLabelText(win_id);
    if(is_update_panel){
        DisplayRecordAllBG(win_id);
        DisplayRecordOwnerDrawAnim(win_id);
    }
    DisplayRecordTime(win_id);
    DisplaySoftkey(win_id);
    MMIAPICOM_UpdatePanel();

}

/*****************************************************************************/
//  Description : display softkey
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplaySoftkey(MMI_WIN_ID_T win_id)
{
#if !defined (KEYPAD_TYPE_COMPACT_KEYPAD) && !defined (KEYPAD_TYPE_FOUR_KEY_UNIVERSE)
    MMI_TEXT_ID_T leftsoft_id = TXT_NULL;
    MMI_TEXT_ID_T middlesoft_id = TXT_NULL;
    MMI_TEXT_ID_T rightsoft_id = TXT_NULL;
    RECORD_WIN_DATA_T * record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);
    uint32 softkey_mid_image = IMAGE_NULL;
#if 0
	GUI_BG_T    common_bg = {0};
#endif
    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
    {
#if 0
        leftsoft_id = TXT_PAUSE;
        rightsoft_id = STXT_STOP;
        GUIWIN_SetSoftkeyTextId(win_id,  leftsoft_id, middlesoft_id, rightsoft_id, TRUE);
#ifndef MMI_RES_LOW_COST
        softkey_mid_image = IMAGE_COMMON_BTN_PAUSE_FOCUS;
        GUIWIN_SeSoftkeytButtonIconId(win_id, softkey_mid_image, MIDDLE_BUTTON, TRUE);
#endif
#else
		leftsoft_id = STXT_STOP;
        rightsoft_id = STXT_CANCEL;
        GUIWIN_SetSoftkeyTextId(win_id,  leftsoft_id, middlesoft_id, rightsoft_id, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, LEFT_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_STOP, LEFT_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, MIDDLE_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_PAUSE, MIDDLE_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, RIGHT_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_RECORDING_CANCEL, RIGHT_BUTTON, TRUE);
#endif
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
#if 0
        leftsoft_id = TXT_COMMON_COMMON_CONTINUE;
        rightsoft_id = STXT_STOP;
        GUIWIN_SetSoftkeyTextId(win_id,  leftsoft_id, middlesoft_id, rightsoft_id, TRUE);
#if 0//def MID_SOFTKEY_USE_IMG_SUPPORT
        softkey_mid_image = IMAGE_COMMON_SOFTKEY_START_ICON;
#else
#ifndef MMI_RES_LOW_COST
        softkey_mid_image = IMAGE_COMMON_BTN_PLAY_FOCUS;
#endif
#endif
#else
		leftsoft_id = STXT_STOP;
        rightsoft_id = STXT_CANCEL;
        GUIWIN_SetSoftkeyTextId(win_id,  leftsoft_id, middlesoft_id, rightsoft_id, TRUE);
        GUIWIN_SeSoftkeytButtonIconId(win_id, softkey_mid_image, MIDDLE_BUTTON, TRUE);
        //GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, LEFT_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_STOP, LEFT_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, MIDDLE_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_RECORD, MIDDLE_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, RIGHT_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_RECORDING_CANCEL, RIGHT_BUTTON, TRUE);
   #endif

    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY])
    {
#if 1//change by James li
        leftsoft_id = TXT_LIST;//STR_MAINMENU_FILES;
        //middlesoft_id = TXT_MULTIM_AUDITION;
        rightsoft_id = STXT_RETURN;
        GUIWIN_SetSoftkeyTextId(win_id,  leftsoft_id, middlesoft_id, rightsoft_id, TRUE);
#if 0//def MID_SOFTKEY_USE_IMG_SUPPORT
        softkey_mid_image = IMAGE_COMMON_SOFTKEY_START_ICON;
#else
#ifndef MMI_RES_LOW_COST
        softkey_mid_image = IMAGE_COMMON_BTN_PLAY_FOCUS;
#endif
#endif
        GUIWIN_SeSoftkeytButtonIconId(win_id, softkey_mid_image, MIDDLE_BUTTON, TRUE);
#else
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, LEFT_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_LIST, LEFT_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, RIGHT_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_RECORDING_BACK, RIGHT_BUTTON, TRUE);
		leftsoft_id = STR_MAINMENU_FILES;
             rightsoft_id = STXT_RETURN;
             GUIWIN_SetSoftkeyTextId(win_id,  leftsoft_id, middlesoft_id, rightsoft_id, TRUE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, MIDDLE_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_RECORD, MIDDLE_BUTTON, TRUE);
#endif
    }
    else
    {
#if 1//change by James li
        leftsoft_id = TXT_LIST;//STR_MAINMENU_FILES
        middlesoft_id = TXT_FM_RECORD;
        rightsoft_id = STXT_RETURN;
        GUIWIN_SetSoftkeyTextId(win_id,  leftsoft_id, middlesoft_id, rightsoft_id, TRUE);
#ifdef GUI_MID_SOFTKEY_USE_IMG
        softkey_mid_image = IMAGE_COMMON_SOFTKEY_RECORD_ICON;
        GUIWIN_SeSoftkeytButtonIconId(win_id, softkey_mid_image, MIDDLE_BUTTON, TRUE);
#endif
#else
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, LEFT_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_LIST, LEFT_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, RIGHT_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_RECORDING_BACK, RIGHT_BUTTON, TRUE);
		leftsoft_id = STR_MAINMENU_FILES;
             rightsoft_id = STXT_RETURN;
             GUIWIN_SetSoftkeyTextId(win_id,  leftsoft_id, middlesoft_id, rightsoft_id, TRUE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, MIDDLE_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_RECORD_RECORD, MIDDLE_BUTTON, TRUE);
#endif
    }
#endif
#if 0
    common_bg.bg_type = GUI_BG_COLOR;
    common_bg.color = MMI_BLACK_COLOR;
    GUIWIN_SetSoftkeyCommonBg(win_id, &common_bg);
    //GUIWIN_SetSoftkeyBgAlpha(win_id,0);
#endif
}
/*****************************************************************************/
//  Description : display record time
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordTime(MMI_HANDLE_T win_handle)
{
    /*lint -e529*/
    MMIRECORD_TIME_T  cur_time = {0};
    GUI_POINT_T disp_point = {0};
    uint16 img_num_width = 0;
    uint16 img_point_width = 0;
    uint16 img_height = 0;
    GUI_RECT_T rect = {0};
    RECORD_WIN_DATA_T       *record_win_data_ptr = PNULL;
	MMI_STRING_T text = {0};
	uint8           cur_time_str[32]    = {0};
	wchar 			time_wstr[18] = {0};
	uint32  cur_hours = 0;
	uint32  cur_minutes = 0;
	uint32  cur_seconds = 0;

    record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }
    //SCI_TRACE_LOW("DisplayRecordTime line[%d] file[%s] ", __LINE__, __FILE__);

    cur_time = MMIRECORD_GetRecordTime();

	/*
    //if (total_record_time > 20)
    {
        cur_time = MMIRECORD_GetRecordTime();
    }
    else
    {
        cur_hours = (total_record_time / 3600);
        cur_minutes = ((total_record_time % 3600) / 60);
        cur_seconds = ((total_record_time % 3600) % 60);

        cur_time.second_low = (uint8)(cur_seconds % 10);
        cur_time.second_high = (uint8)(cur_seconds / 10);
        cur_time.minute_low = (uint8)(cur_minutes % 10);
        cur_time.minute_high = (uint8)(cur_minutes / 10);
        cur_time.hour_low = (uint8)(cur_hours % 10);
        cur_time.hour_high = (uint8)(cur_hours / 10);
    }

    //GUIRES_GetImgWidthHeight(&img_num_width, &img_height, IMAGE_RECORD_NUMBER_0, win_handle);
    //GUIRES_GetImgWidthHeight(&img_point_width, PNULL, IMAGE_RECORD_NUMBER_POINT, win_handle);
    disp_point.x = record_win_data_ptr->position.record_time_x;
    disp_point.y = record_win_data_ptr->position.record_time_y;

    rect.top = disp_point.y;
    rect.bottom = rect.top + img_height;
    rect.left = disp_point.x;
    rect.right = rect.left + img_num_width;
    DisplayRecordTimeImg(win_handle, &rect, cur_time.hour_high);

    rect.left = rect.left + MMIRECORD_TIME_SPACE + img_num_width;
    rect.right = rect.left + img_num_width;
    DisplayRecordTimeImg(win_handle, &rect, cur_time.hour_low);

    rect.left = rect.left + MMIRECORD_TIME_SPACE + img_num_width;
    rect.right = rect.left + img_point_width;
    DisplayRecordTimeImg(win_handle, &rect, 10);

    rect.left = rect.left + MMIRECORD_TIME_SPACE + img_point_width;
    rect.right = rect.left + img_num_width;
    DisplayRecordTimeImg(win_handle, &rect, cur_time.minute_high);

    rect.left = rect.left + MMIRECORD_TIME_SPACE + img_num_width;
    rect.right = rect.left + img_num_width;
    DisplayRecordTimeImg(win_handle, &rect, cur_time.minute_low);

    rect.left = rect.left + MMIRECORD_TIME_SPACE + img_num_width;
    rect.right = rect.left + img_point_width;
    DisplayRecordTimeImg(win_handle, &rect, 10);

    rect.left = rect.left + MMIRECORD_TIME_SPACE + img_point_width;
    rect.right = rect.left + img_num_width;
    DisplayRecordTimeImg(win_handle, &rect, cur_time.second_high);

    rect.left = rect.left + MMIRECORD_TIME_SPACE + img_num_width;
    rect.right = rect.left + img_num_width;
    DisplayRecordTimeImg(win_handle, &rect, cur_time.second_low);
	*/
	sprintf((char*)cur_time_str, "%d%d:%d%d:%d%d",
                                 cur_time.hour_high,cur_time.hour_low,
                                 cur_time.minute_high,cur_time.minute_low,
                                 cur_time.second_high,cur_time.second_low);
    MMIAPICOM_StrToWstr(cur_time_str,&time_wstr);
    text.wstr_ptr = time_wstr;
    text.wstr_len  = MMIAPICOM_Wstrlen(text.wstr_ptr);
    GUILABEL_SetText(MMIRECORD_TIME_LABEL_CTRL_ID, &text, TRUE);
#if 0
    if (MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]
        && total_record_time <= 20)
    {
        total_record_time++;
    }
#endif
    /*lint -e529*/
}

/*****************************************************************************/
//  Description : UpdateButtonStatusPlaying
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusPlaying(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;

}


/*****************************************************************************/
//  Description : UpdateButtonStatusPlayPause
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusPlayPause(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;

}

/*****************************************************************************/
//  Description : UpdateButtonStatusRecording
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusRecording(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;

}

/*****************************************************************************/
//  Description : UpdateButtonStatusRecordPaused
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusRecordPaused(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;
}

/*****************************************************************************/
//  Description : UpdateButtonStatusStopped
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusStopped(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;

    record_win_data_ptr->is_begin = FALSE;
}

/*****************************************************************************/
//  Description : UpdateButtonStatusReady
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusReady(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->is_begin = FALSE;
}


/*****************************************************************************/
//  Description : display owner draw anim
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordOwnerDrawAnim(MMI_WIN_ID_T win_id)
{
    BOOLEAN             is_fill_bg = TRUE;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);
    MMI_IMAGE_ID_T cur_img_id = 0;
    uint16 				img_width = 0;
    uint16 				img_height = 0;
    uint16 				centor_x = 0;
    uint16 				centor_y = 0;
	uint16 				centor_r = 0;

    if(PNULL == record_win_data_ptr
            || ((MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
                 || MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE])
                && (MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
                    || MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]))
      )
    {
        return;
    }
    if(GUI_IsInvalidRect(record_win_data_ptr->record_play_anim.dis_rect))
    {
        return;
    }

    if(record_win_data_ptr->record_play_anim.total_frame > 0 && record_win_data_ptr->record_play_anim.cur_frame < record_win_data_ptr->record_play_anim.total_frame)
    {
        if(0 != record_win_data_ptr->record_play_anim.cur_frame)
        {
            is_fill_bg = FALSE;
        }

        if(FALSE)
        {
            switch(record_win_data_ptr->record_play_anim.bg.bg_type)
            {
            case GUI_BG_IMG:
                {
                    GUIRES_DisplayImg(PNULL, &record_win_data_ptr->record_play_anim.dis_rect, PNULL, record_win_data_ptr->record_play_anim.win_id, record_win_data_ptr->record_play_anim.bg.img_id, &record_win_data_ptr->record_play_anim.lcd_dev);
                }
                break;

            case GUI_BG_COLOR:
                {
                    LCD_FillRect(&record_win_data_ptr->record_play_anim.lcd_dev, record_win_data_ptr->record_play_anim.dis_rect, record_win_data_ptr->record_play_anim.bg.color);
                }
                break;

            default:
                break;
            }
        }


        cur_img_id = GetAnimalFrame(record_win_data_ptr->record_play_anim.cur_frame);
    	GUIRES_GetImgWidthHeight(&img_width, &img_height, cur_img_id, win_id);
		centor_r = record_win_data_ptr->position.record_record_bg_w/2;
		centor_x = record_win_data_ptr->position.record_record_bg_x+centor_r;
		centor_y = record_win_data_ptr->position.record_record_bg_y+centor_r;

	    record_win_data_ptr->record_play_anim.dis_rect = GUI_CreateRect(centor_x - img_width/2,centor_y - img_width/2,
	            centor_x+img_width/2,
	            centor_y+img_height/2);

        if(0 != cur_img_id)
        {
            //GUIRES_DisplayImg(PNULL, &record_win_data_ptr->record_play_anim.dis_rect, PNULL, record_win_data_ptr->record_play_anim.win_id, cur_img_id, &record_win_data_ptr->record_play_anim.lcd_dev);
			GUIRES_DisplayImgWitchColorExchange(PNULL, &record_win_data_ptr->record_play_anim.dis_rect, PNULL, record_win_data_ptr->record_play_anim.win_id, cur_img_id, &record_win_data_ptr->record_play_anim.lcd_dev,TRUE,MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT));

            record_win_data_ptr->record_play_anim.cur_frame++;
            record_win_data_ptr->record_play_anim.cur_frame = (uint16)(record_win_data_ptr->record_play_anim.cur_frame % record_win_data_ptr->record_play_anim.total_frame);
        }
    }
}


/*****************************************************************************/
//  Description : MMIRECORD_LoadRecordMenu
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_LoadRecordMenu(MMI_HANDLE_T parent_handle)
{
    MMI_CTRL_ID_T           ctrl_id     = MMIRECORD_STARTSTATUS_OPTIONMENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T     group_id    = MENU_RECORD_START_OPTION;
    MMI_MENU_ID_T           menu_id     = ID_RECORD_STORIGE_POS;
    GUIMENU_BUTTON_INFO_T   button_info                   = {0};
    GUIMENU_TITLE_INFO_T        title_info                      = {0};
    MMI_STRING_T title_str = {0};
    MMIFILE_DEVICE_E        device = MMI_DEVICE_UDISK;
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
    MMIRECORD_WORK_FORMAT_E   file_formattype = MMIRECORD_WORK_FORMAT_MAX;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(parent_handle);


    if(PNULL != record_win_data_ptr
        && MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]
        && MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        menu_id = ID_RECORD_START;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        menu_id = ID_RECORD_STOPPED_PLAY;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        menu_id = ID_RECORD_AGAIN;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        menu_id = ID_RECORD_DELETE;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        menu_id = ID_RECORD_INSERT;//recordwork
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        menu_id = ID_RECORD_SEND;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);

        if(MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_AMR)
                || MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_MP3))
        {
            menu_id = ID_RECORD_STORIGE_FORMAT; //andy.he_record
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        }
        else
        {
            menu_id = ID_RECORD_STORIGE_FORMAT;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        }

    }
    else
    {
        menu_id = ID_RECORD_START;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        menu_id = ID_RECORD_STOPPED_PLAY;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        menu_id = ID_RECORD_AGAIN;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        menu_id = ID_RECORD_DELETE;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);

        if(PNULL != record_win_data_ptr
                && record_win_data_ptr->is_insert_record)
        {
            menu_id = ID_RECORD_SEND;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
            menu_id = ID_RECORD_INSERT;//recordwork
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        }
        else
        {
            menu_id = ID_RECORD_SEND;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
            menu_id = ID_RECORD_INSERT;//recordwork
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        }

        if(MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_AMR)
                || MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_MP3))
        {
            menu_id = ID_RECORD_STORIGE_FORMAT;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        }
        else
        {
            menu_id = ID_RECORD_STORIGE_FORMAT;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        }

    }


    if(MMIRECORD_IsExistValidateDevice())
    {
        menu_id = ID_RECORD_STORIGE_POS;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        group_id = MENU_RECORD_STORE_STATUS;

        for(device = MMI_DEVICE_UDISK; device < MMI_DEVICE_NUM; device++)
        {
            menu_id = ID_RECORD_UDISK + device - MMI_DEVICE_UDISK;

            if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device)))
            {
                GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
            }
            else
            {
                GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
            }
        }

        pop_item_info.is_static = TRUE;
        pop_item_info.ctrl_id = ctrl_id;
        pop_item_info.group_id = MENU_RECORD_STORE_STATUS;
        pop_item_info.menu_id = ID_RECORD_UDISK + MMIRECORD_GetRecordDeviceIndex();
        GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
    }
    else
    {
        menu_id = ID_RECORD_STORIGE_POS;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
    }


    //file-format style sub context menu  andy.he_record
    if(PNULL != record_win_data_ptr
            &&	record_win_data_ptr->is_insert_record)
    {
        menu_id = ID_RECORD_STORIGE_FORMAT;
        group_id = MENU_RECORD_START_OPTION;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
    }
    else
    {
        group_id = MENU_RECORD_STORE_FORMAT;

        if(MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_MP3))
        {
            menu_id = ID_RECORD_STORIGE_FORMAT_MP3;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        }
        else
        {
            menu_id = ID_RECORD_STORIGE_FORMAT_MP3;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        }

        if(MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_AMR))
        {
            menu_id = ID_RECORD_STORIGE_FORMAT_AMR;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        }
        else
        {
            menu_id = ID_RECORD_STORIGE_FORMAT_AMR;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        }

        pop_item_info.is_static = TRUE;
        pop_item_info.ctrl_id = ctrl_id;
        pop_item_info.group_id = MENU_RECORD_STORE_FORMAT;
        file_formattype = MMIRECORD_GetRecordFileFormatType();

        if(MMIRECORD_WORK_FORMAT_MP3 == file_formattype)
        {
            pop_item_info.menu_id = ID_RECORD_STORIGE_FORMAT_MP3;
        }
        else if(MMIRECORD_WORK_FORMAT_AMR == file_formattype)
        {
            pop_item_info.menu_id = ID_RECORD_STORIGE_FORMAT_AMR;
        }
        else
        {
            pop_item_info.menu_id = ID_RECORD_STORIGE_FORMAT_WAV;
        }

        GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
    }
}

/*****************************************************************************/
//  Description : SetNameLabelText
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetNameLabelText(MMI_WIN_ID_T win_id)
{
#ifndef MMI_RECORD_MINI_SUPPORT
    MMI_STRING_T cur_file_s = {0};
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    cur_file_s.wstr_ptr = record_win_data_ptr->record_file_name;
    cur_file_s.wstr_len = record_win_data_ptr->record_file_name_len;

    if(0 != cur_file_s.wstr_len)
    {
        GUILABEL_SetText(MMIRECORD_NAME_LABEL_H_CTRL_ID, &cur_file_s, FALSE);
        GUILABEL_SetText(MMIRECORD_NAME_LABEL_CTRL_ID, &cur_file_s, FALSE);
    }

    if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY])
    {
        GUILABEL_SetVisible(MMIRECORD_NAME_LABEL_CTRL_ID, TRUE, TRUE);
    }
    else
    {
        GUILABEL_SetVisible(MMIRECORD_NAME_LABEL_CTRL_ID, FALSE, TRUE);
    }

#endif
}

/*****************************************************************************/
//  Description : SetTimeLabelText
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetTimeLabelText(uint32 cur_time_elapsed, uint32 total_time)
{
    uint8           cur_time_str[32]    = {0};
    uint8           total_time_str[32]  = {0};
    wchar           cur_time_wstr[32]   = {0};
    wchar           total_time_wstr[32] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    MMI_STRING_T    total_str_t = {0};


#if (defined MAINLCD_DEV_SIZE_160X128) || (defined MAINLCD_DEV_SIZE_220X176)
        //only display sec and min
    cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld", 
                                 (cur_time_elapsed/60)%60,
                                 (cur_time_elapsed%60));
        
    total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld", 
                                 (total_time/60)%60, 
                                 total_time%60);
#else
    cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld:%.2ld",
                                 (cur_time_elapsed / 60) / 60,
                                 (cur_time_elapsed / 60) % 60,
                                 (cur_time_elapsed % 60));

    total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld:%.2ld",
                                   (total_time / 60) / 60,
                                   (total_time / 60) % 60,
                                   total_time % 60);
#endif
    cur_str_t.wstr_ptr = cur_time_wstr;
    total_str_t.wstr_ptr = total_time_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)cur_time_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    MMI_STRNTOWSTR(total_str_t.wstr_ptr, total_str_t.wstr_len, (uint8*)total_time_str, total_str_t.wstr_len, total_str_t.wstr_len);

    GUILABEL_SetText(MMIRECORD_PLAY_TIME_LABEL_CTRL_ID, &cur_str_t, FALSE);
    GUILABEL_SetText(MMIRECORD_PLAY_ALL_TIME_LABEL_CTRL_ID, &total_str_t, FALSE);

    GUILABEL_SetVisible(MMIRECORD_PLAY_TIME_LABEL_CTRL_ID, TRUE, TRUE);
    GUILABEL_SetVisible(MMIRECORD_PLAY_ALL_TIME_LABEL_CTRL_ID, TRUE, TRUE);

    SCI_TRACE_LOW("[RECORD] SetLabelText cur_time_elapsed = %d, total_time = %d", cur_time_elapsed, total_time);

}


/*****************************************************************************/
//  Description : SetFormParam
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFormParam(MMI_WIN_ID_T win_id)
{
    /*lint -e529*/
    GUI_BG_T    FormBg = {0};
    GUI_BG_T    gui_bg = {0};
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);

    if(PNULL == record_win_data_ptr)
    {
        return FALSE;
    }


    {
        GUI_RECT_T rect = {0};
        GUIANIM_CTRL_INFO_T     control_info = {0};
        GUIANIM_DATA_INFO_T     data_info = {0};
        GUIANIM_FILE_INFO_T     file_info = {0};
        GUIANIM_DISPLAY_INFO_T  display_info = {0};
	    uint16  layer_width = 0;
	    uint16  layer_height = 0;
		//GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        gui_bg.bg_type = GUI_BG_COLOR;
        gui_bg.color = MMI_BLACK_COLOR;
        GUISTATUSBAR_SetBg(win_id, gui_bg);

		//GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);
		//LCD_FillRect(&lcd_dev_info,rect, MMI_BLACK_COLOR);

        SetPortraitAndLandscapeFormParam(win_id);

        record_win_data_ptr->record_play_anim.dis_rect = GUI_CreateRect(record_win_data_ptr->position.record_record_bg_x, record_win_data_ptr->position.record_record_bg_y,
                record_win_data_ptr->position.record_record_bg_x + record_win_data_ptr->position.record_record_bg_w,
                record_win_data_ptr->position.record_record_bg_y + record_win_data_ptr->position.record_record_bg_h);
/*

#ifndef MMI_RECORD_MINI_SUPPORT
        GUILABEL_SetFont(MMIRECORD_NAME_LABEL_CTRL_ID,  MMIRECORD_FILENAME_FONT, MMI_WHITE_COLOR);
#endif
        GUILABEL_SetFont(MMIRECORD_PLAY_TIME_LABEL_CTRL_ID,  MMIRECORD_FILENAME_FONT, MMI_WHITE_COLOR);
        GUILABEL_SetFont(MMIRECORD_PLAY_ALL_TIME_LABEL_CTRL_ID,  MMIRECORD_FILENAME_FONT, MMI_WHITE_COLOR);



#ifndef MMI_RECORD_MINI_SUPPORT
        rect.left = record_win_data_ptr->position.name_label_x;
        rect.top = record_win_data_ptr->position.name_label_y;
        rect.right = record_win_data_ptr->position.name_label_x + record_win_data_ptr->position.name_label_w - 1;
        rect.bottom = record_win_data_ptr->position.name_label_y + record_win_data_ptr->position.name_label_h - 1;
        GUILABEL_SetRect(MMIRECORD_NAME_LABEL_CTRL_ID, &rect, FALSE);
#endif
        rect.left = record_win_data_ptr->position.play_time_label_x;
        rect.top = record_win_data_ptr->position.play_time_label_y;
        rect.right = record_win_data_ptr->position.play_time_label_x + record_win_data_ptr->position.play_time_label_w;
        rect.bottom = record_win_data_ptr->position.play_time_label_y + record_win_data_ptr->position.play_time_label_h;
        GUILABEL_SetRect(MMIRECORD_PLAY_TIME_LABEL_CTRL_ID, &rect, FALSE);

        rect.left = record_win_data_ptr->position.play_all_time_label_x;
        rect.top = record_win_data_ptr->position.play_all_time_label_y;
        rect.right = record_win_data_ptr->position.play_all_time_label_x + record_win_data_ptr->position.play_all_time_label_w;
        rect.bottom = record_win_data_ptr->position.play_all_time_label_y + record_win_data_ptr->position.play_all_time_label_h;
        GUILABEL_SetRect(MMIRECORD_PLAY_ALL_TIME_LABEL_CTRL_ID, &rect, FALSE);

*/
    }
/*
#ifndef MMI_RECORD_MINI_SUPPORT
    IGUICTRL_SetState(MMK_GetCtrlPtr(MMIRECORD_NAME_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
#endif
    IGUICTRL_SetState(MMK_GetCtrlPtr(MMIRECORD_PLAY_TIME_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
    IGUICTRL_SetState(MMK_GetCtrlPtr(MMIRECORD_PLAY_ALL_TIME_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
*/
    return TRUE;
    /*lint -e529*/
}

/*****************************************************************************/
//  Description : SetPortraitAndLandscapeFormParam
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetPortraitAndLandscapeFormParam(MMI_WIN_ID_T win_id)
{
    uint16 space = 0;
    GUI_RECT_T win_rect = {0};
    int16 mov_h = 0;
    //GUIFORM_CHILD_WIDTH_T   sbs = {0};


    uint16 img_num_width = 0;
    uint16 img_point_width = 0;
    uint16 record_time_num_bg_x = 0;
    uint16 record_time_num_bg_y = 0;
    uint16 record_time_display_width = 0;
    uint16 record_record_bg_y = 0;
    uint16 record_time_owndraw_y = 0;
    uint16 record_needle_bg_y = 0;
    uint16 record_needle_center_point_x = 0;
    uint16 record_needle_center_point_y = 0;
    uint16 record_needle_rotate_point_offset_x = 0;
    uint16 record_needle_rotate_point_offset_y = 0;
    uint16 record_volue_button_y = 0;
    uint16 record_volue_button_x = 0;
    uint16 record_left_button_y = 0;
    uint16 record_play_bg_x = 0;
    uint16 record_play_bg_w = 0;
#ifdef MMI_RECORD_MINI_SUPPORT
    uint16 record_play_bg_y = 0;
    uint16 record_progress_bg_y = 0;
#endif
    uint16 record_play_time_label_w = 0;
    uint16 record_play_all_time_label_w = 0;
    uint16 record_left_rect_w = 0;
    uint16 record_left_rect_margin_w = 0;
    uint16 record_right_rect_w = 0;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    MMK_GetWinRect(win_id, &win_rect);

    if(MMITHEME_IsMainScreenLandscape())
    {
        record_left_rect_w                  = MMI_MAINSCREEN_WIDTH; //will be calculated later;
        //record_left_rect_margin_w           = MMI_MAINSCREEN_WIDTH; //will be calculated later;
        record_right_rect_w                 = MMI_MAINSCREEN_WIDTH; //will be calculated later;
        record_time_num_bg_x                = MMIRECORD_TIME_NUM_BG_H_X;
        record_time_num_bg_y                = MMIRECORD_TIME_NUM_BG_H_Y;
        record_time_display_width           = MMIRECORD_TIME_NUM_BG_H_W;

        record_record_bg_y                  = MMIRECORD_RECORD_RECORD_BG_H_Y;
        record_time_owndraw_y               = MMIRECORD_TIME_OWNDRAW_H_Y;
        record_needle_bg_y                  = MMIRECORD_NEEDLE_BG_H_Y;

        record_needle_center_point_x        = MMIRECORD_NEEDLE_CENTER_POINT_H_X;
        record_needle_center_point_y        = MMIRECORD_NEEDLE_CENTER_POINT_H_Y;
        record_needle_rotate_point_offset_x = MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X;
        record_needle_rotate_point_offset_y = MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y;

        record_volue_button_y               = MMIRECORD_VOLUME_BUTTON_H_Y;
        record_volue_button_x               = MMIRECORD_VOLUME_BUTTON_H_X;
        record_left_button_y                = MMIRECORD_LEFT_BUTTON_H_Y;

        record_play_bg_x                    = MMIRECORD_PLAY_BG_H_X;
#ifdef MMI_RECORD_MINI_SUPPORT
        record_play_bg_y                    = MMIRECORD_PLAY_BG_H_Y;
        record_progress_bg_y                = MMIRECORD_PROGRESS_BG_H_Y;
#endif
        record_play_bg_w                    = MMIRECORD_PLAY_BG_H_W;
        record_play_time_label_w            = MMIRECORD_PLAY_TIME_LABEL_H_W;
        record_play_all_time_label_w        = MMIRECORD_PLAY_ALL_TIME_LABEL_H_W;
    }
    else
    {
        record_left_rect_w                  = MMI_MAINSCREEN_WIDTH;
        //record_left_rect_margin_w           = 0;
        record_right_rect_w                 = MMI_MAINSCREEN_WIDTH;
        record_time_num_bg_x                = MMIRECORD_TIME_NUM_BG_X;
        record_time_num_bg_y                = MMIRECORD_TIME_NUM_BG_Y;
        record_time_display_width           = MMIRECORD_TIME_NUM_BG_W;

        record_record_bg_y                  = 70;//64;
        record_time_owndraw_y               = 26;
        record_needle_bg_y                  = 0;

        record_needle_center_point_x        = MMIRECORD_NEEDLE_CENTER_POINT_X;
        record_needle_center_point_y        = MMIRECORD_NEEDLE_CENTER_POINT_Y;
        record_needle_rotate_point_offset_x = MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X;
        record_needle_rotate_point_offset_y = MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y;

        record_volue_button_y               = MMIRECORD_VOLUME_BUTTON_Y;
        record_volue_button_x               = MMIRECORD_VOLUME_BUTTON_X;
        record_left_button_y                = MMIRECORD_LEFT_BUTTON_Y;

        record_play_bg_x                    = MMIRECORD_PLAY_BG_X;
#ifdef MMI_RECORD_MINI_SUPPORT
        record_play_bg_y                    = MMIRECORD_PLAY_BG_Y;
        record_progress_bg_y                = MMIRECORD_PROGRESS_BG_Y;
#endif
        record_play_bg_w                    = MMIRECORD_PLAY_BG_W;
        record_play_time_label_w            = MMIRECORD_PLAY_TIME_LABEL_W;
        record_play_all_time_label_w        = MMIRECORD_PLAY_ALL_TIME_LABEL_W;
    }

    //1.IMAGE_RECORD_NUMBER_BG;record_num_bg
    record_win_data_ptr->position.record_num_bg_x = record_time_num_bg_x;
    record_win_data_ptr->position.record_num_bg_y = record_time_num_bg_y;
    record_win_data_ptr->position.record_num_bg_w = record_time_display_width;
    GUIRES_GetImgWidthHeight(PNULL, (uint16 *)&record_win_data_ptr->position.record_num_bg_h, IMAGE_RECORD_NUMBER_BG, win_id);
    //record_win_data_ptr->position.record_num_bg_x = (record_left_rect_w - record_win_data_ptr->position.record_num_bg_w) / 2;

    //calculate record's some big rect;
    if(MMITHEME_IsMainScreenLandscape())
    {
        record_left_rect_w        = record_win_data_ptr->position.record_num_bg_w + record_win_data_ptr->position.record_num_bg_x * 2;
        record_left_rect_margin_w = record_win_data_ptr->position.record_num_bg_x + record_win_data_ptr->position.record_num_bg_w;
        record_right_rect_w       = MMI_MAINSCREEN_HEIGHT - record_left_rect_margin_w;
    }
    else
    {
        record_left_rect_w        = MMI_MAINSCREEN_WIDTH;
        record_left_rect_margin_w = 0;
        record_right_rect_w       = MMI_MAINSCREEN_WIDTH;
    }

    //2.IMAGE_RECORD_RECORD_BG;record_record_bg
    record_win_data_ptr->position.record_record_bg_y = record_record_bg_y;
    GUIRES_GetImgWidthHeight(&record_win_data_ptr->position.record_record_bg_w, &record_win_data_ptr->position.record_record_bg_h, IMAGE_RECORD_INDICATOR, win_id);
#if defined (MAINLCD_SIZE_128X160)
    record_win_data_ptr->position.record_record_bg_x = 13;
    record_win_data_ptr->position.record_record_bg_y = 39;
#else
    record_win_data_ptr->position.record_record_bg_x = 19;
#endif
	/*
    //3.IMAGE_RECORD_NUMBER_0;record_time
    record_win_data_ptr->position.record_time_y = record_time_owndraw_y;
    GUIRES_GetImgWidthHeight(&img_num_width, &record_win_data_ptr->position.record_time_h, IMAGE_RECORD_NUMBER_0, win_id);
    GUIRES_GetImgWidthHeight(&img_point_width, PNULL, IMAGE_RECORD_NUMBER_POINT, win_id);
    record_win_data_ptr->position.record_time_w = (img_num_width * 6 + img_point_width * 2 + MMIRECORD_TIME_SPACE * 7);
    record_win_data_ptr->position.record_time_x = (record_left_rect_w - record_win_data_ptr->position.record_time_w) / 2;
#ifdef MMI_RECORD_MINI_SUPPORT //replace above record_win_data_ptr->position.record_time_x; 

    if(MMITHEME_IsMainScreenLandscape())
    {
        record_win_data_ptr->position.record_time_x = (record_right_rect_w - record_win_data_ptr->position.record_time_w) / 2 + record_left_rect_margin_w;
    }

#endif

#ifndef MMI_RECORD_MINI_SUPPORT
    //4.IMAGE_RECORD_NEEDLE_MASK;record_num_bg
    record_win_data_ptr->position.needle_bg_y = record_needle_bg_y;
    GUIRES_GetImgWidthHeight(&record_win_data_ptr->position.needle_bg_w, &record_win_data_ptr->position.needle_bg_h, IMAGE_RECORD_NEEDLE_MASK, win_id);

    if(MMITHEME_IsMainScreenLandscape())
    {
#if defined MAINLCD_SIZE_240X320
        record_win_data_ptr->position.needle_bg_x = (record_right_rect_w - record_win_data_ptr->position.needle_bg_w) / 2 + record_left_rect_margin_w + 3; //adjust position, + 3;BUG-79941;
#endif
    }
    else
    {
        record_win_data_ptr->position.needle_bg_x = (record_right_rect_w - record_win_data_ptr->position.needle_bg_w) / 2 + record_left_rect_margin_w;
    }

    record_win_data_ptr->position.needle_center_point_x = record_needle_center_point_x;
    record_win_data_ptr->position.needle_center_point_y = record_needle_center_point_y;

    record_win_data_ptr->position.needle_rotate_point_offset_x = record_needle_rotate_point_offset_x;
    record_win_data_ptr->position.needle_rotate_point_offset_y = record_needle_rotate_point_offset_y;
    //5.IMAGE_RECORD_VOLUME;volume_button
    record_win_data_ptr->position.volume_button_y     = record_volue_button_y;
    GUIRES_GetImgWidthHeight(&record_win_data_ptr->position.volume_button_w, &record_win_data_ptr->position.volume_button_h, IMAGE_RECORD_VOLUME, win_id);
    record_win_data_ptr->position.volume_button_x = record_volue_button_x;
//#ifndef RECORD_BARPHONE_UI_STYLE_SUPPORT
    //6.IMAGE_RECORD_RECORD_BTN_PDA_FOCUS; left_button
    record_win_data_ptr->position.left_button_y = record_left_button_y;
    GUIRES_GetImgWidthHeight(&record_win_data_ptr->position.left_button_w, &record_win_data_ptr->position.left_button_h, IMAGE_RECORD_RECORD_BTN_PDA_FOCUS, win_id);

    //7.IMAGE_RECORD_STOP_BTN_FOCUS;right_button
    record_win_data_ptr->position.right_button_y = record_win_data_ptr->position.left_button_y;
    GUIRES_GetImgWidthHeight(&record_win_data_ptr->position.right_button_w, &record_win_data_ptr->position.right_button_h, IMAGE_RECORD_STOP_BTN_FOCUS, win_id);

    //8.IMAGE_RECORD_BACK_BTN_FOCUS;_back_button
    GUIRES_GetImgWidthHeight(&record_win_data_ptr->position.istyle_back_button_w, &record_win_data_ptr->position.istyle_back_button_h, IMAGE_RECORD_BACK_BTN_FOCUS, win_id);
    record_win_data_ptr->position.istyle_back_button_y = record_win_data_ptr->position.left_button_y + (record_win_data_ptr->position.left_button_h - record_win_data_ptr->position.istyle_back_button_h) / 2;

    //9.IMAGE_RECORD_OPTION_BTN_FOCUS;option_button
    record_win_data_ptr->position.istyle_option_button_y = record_win_data_ptr->position.istyle_back_button_y;
    GUIRES_GetImgWidthHeight(&record_win_data_ptr->position.istyle_option_button_w, &record_win_data_ptr->position.istyle_option_button_h, IMAGE_RECORD_OPTION_BTN_FOCUS, win_id);

    //handle left,right,back,option,these four button's X position;
#if defined(MMI_ISTYLE_SUPPORT)    
    if(MMITHEME_IsIstyle())
    {
        space = (record_right_rect_w - 2 * record_win_data_ptr->position.left_button_w - 2 * record_win_data_ptr->position.istyle_back_button_w) / 5 ;
        record_win_data_ptr->position.istyle_back_button_x   = record_left_rect_margin_w + space;
        record_win_data_ptr->position.left_button_x          = record_win_data_ptr->position.istyle_back_button_x + record_win_data_ptr->position.istyle_back_button_w + space;
        record_win_data_ptr->position.right_button_x         = record_win_data_ptr->position.left_button_x + record_win_data_ptr->position.left_button_w + space;
        record_win_data_ptr->position.istyle_option_button_x = record_win_data_ptr->position.right_button_x + record_win_data_ptr->position.right_button_w + space;
    }
    else
#endif		
    {
        space = (record_right_rect_w - 2 * record_win_data_ptr->position.left_button_w) / 3 ;
        record_win_data_ptr->position.left_button_x  = record_left_rect_margin_w + space;
        record_win_data_ptr->position.right_button_x = record_win_data_ptr->position.left_button_x + record_win_data_ptr->position.left_button_w + space;
    }

//#endif
#endif

    //10.IMAGE_RECORD_RECORD_PDA_BG;need_refresh_bg;
    if(MMITHEME_IsMainScreenLandscape())
    {
        record_win_data_ptr->position.need_refresh_bg_x = 0;
#ifdef MMI_RECORD_MINI_SUPPORT
		record_win_data_ptr->position.need_refresh_bg_y = MMI_TITLE_HEIGHT;
#else
		record_win_data_ptr->position.need_refresh_bg_y = MMI_STATUSBAR_HEIGHT;
#endif       
        record_win_data_ptr->position.need_refresh_bg_w = MMI_MAINSCREEN_HEIGHT;
        record_win_data_ptr->position.need_refresh_bg_h = MMI_MAINSCREEN_WIDTH - record_win_data_ptr->position.need_refresh_bg_y;
    }
    else
    {
        record_win_data_ptr->position.need_refresh_bg_x = 0;
#ifdef MMI_RECORD_MINI_SUPPORT
		record_win_data_ptr->position.need_refresh_bg_y = MMI_TITLE_HEIGHT;
#else
		record_win_data_ptr->position.need_refresh_bg_y = MMI_STATUSBAR_HEIGHT;
#endif 	 
        record_win_data_ptr->position.need_refresh_bg_w = MMI_MAINSCREEN_WIDTH;
        record_win_data_ptr->position.need_refresh_bg_h = MMI_MAINSCREEN_HEIGHT - record_win_data_ptr->position.need_refresh_bg_y;
    }

#ifndef MMI_PDA_SUPPORT

    if(MMITHEME_IsMainScreenLandscape())
    {
        record_win_data_ptr->position.need_refresh_bg_h = MMI_MAINSCREEN_WIDTH - record_win_data_ptr->position.need_refresh_bg_y - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
    }
    else
    {
        record_win_data_ptr->position.need_refresh_bg_h = MMI_MAINSCREEN_HEIGHT - record_win_data_ptr->position.need_refresh_bg_y - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
    }

#endif

    //11.IMAGE_RECORD_PLAY_BK_BG;progress_bg;
    record_win_data_ptr->position.play_bg_x = record_play_bg_x;
    record_win_data_ptr->position.play_bg_w = record_play_bg_w;
    GUIRES_GetImgWidthHeight(PNULL, (uint16 *)&record_win_data_ptr->position.play_bg_h, IMAGE_RECORD_PLAY_BK_BG, win_id);
#ifndef MMI_RECORD_MINI_SUPPORT
    record_win_data_ptr->position.play_bg_y = win_rect.bottom - record_win_data_ptr->position.left_button_h - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT
            - (win_rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - (record_win_data_ptr->position.left_button_y + record_win_data_ptr->position.left_button_h)) * 2
            - record_win_data_ptr->position.play_bg_h;

    if(record_win_data_ptr->position.play_bg_y <= (record_win_data_ptr->position.volume_button_y + record_win_data_ptr->position.volume_button_h))
    {
        mov_h = record_win_data_ptr->position.volume_button_y + record_win_data_ptr->position.volume_button_h - record_win_data_ptr->position.play_bg_y;
        record_win_data_ptr->position.play_bg_y = record_win_data_ptr->position.volume_button_y + record_win_data_ptr->position.volume_button_h;

        //re-caculate record button position
        record_win_data_ptr->position.left_button_y     += mov_h / 2;
        record_win_data_ptr->position.right_button_y    += mov_h / 2;
        record_win_data_ptr->position.needle_bg_y       += mov_h / 2;
        record_win_data_ptr->position.needle_centre_y   += mov_h / 2;
        record_win_data_ptr->position.needle_center_point_y += mov_h / 2;
    }

    record_win_data_ptr->position.play_bg_y = win_rect.bottom  - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - record_win_data_ptr->position.play_bg_h;

#else
    record_win_data_ptr->position.play_bg_y = record_play_bg_y;
#endif

    //12.IMAGE_COMMON_PLAYER_PROCESS_GROOVE;progress_bg;
    GUIRES_GetImgWidthHeight(PNULL, (uint16 *)&record_win_data_ptr->position.progress_bg_h, IMAGE_COMMON_PLAYER_PROCESS_GROOVE, win_id);
#ifndef MMI_RECORD_MINI_SUPPORT
    record_win_data_ptr->position.progress_bg_y = record_win_data_ptr->position.play_bg_y + record_win_data_ptr->position.progress_bg_h * 2 + (record_win_data_ptr->position.play_bg_h - record_win_data_ptr->position.progress_bg_h) / 2;
#else
    record_win_data_ptr->position.progress_bg_y = record_progress_bg_y;
#endif

    //13.IMAGE_COMMON_PLAYER_PROCESS_SLIDE;progress_point;
    GUIRES_GetImgWidthHeight((uint16 *)&record_win_data_ptr->position.progress_point_w, (uint16 *)&record_win_data_ptr->position.progress_point_h, IMAGE_COMMON_PLAYER_PROCESS_SLIDE, win_id);
    record_win_data_ptr->position.progress_point_y = record_win_data_ptr->position.progress_bg_y - (record_win_data_ptr->position.progress_point_h - record_win_data_ptr->position.progress_bg_h) / 2;
    record_win_data_ptr->position.progress_point_x = record_left_rect_margin_w;
    record_win_data_ptr->position.progress_bg_x    = record_win_data_ptr->position.progress_point_w / 2 + record_left_rect_margin_w;

    record_win_data_ptr->position.progress_bg_w = record_right_rect_w - record_win_data_ptr->position.progress_point_w;

#ifndef MMI_RECORD_MINI_SUPPORT
//#ifndef RECORD_BARPHONE_UI_STYLE_SUPPORT
    //14.IMAGE_RECORD_PLAY_BTN_FOCUSE;play_stop_button;IMAGE_RECORD_PLAY_BTN_UNSEL
    GUIRES_GetImgWidthHeight((uint16 *)&record_win_data_ptr->position.play_stop_button_w, (uint16 *)&record_win_data_ptr->position.play_stop_button_h, IMAGE_RECORD_PLAY_BTN_FOCUSE, win_id);
    record_win_data_ptr->position.play_stop_button_y = record_win_data_ptr->position.play_bg_y
            + (record_win_data_ptr->position.progress_point_y - record_win_data_ptr->position.play_bg_y - record_win_data_ptr->position.play_stop_button_h) / 2;
    record_win_data_ptr->position.play_stop_button_x = record_win_data_ptr->position.play_bg_x;
//#endif

    //15.name_label;
    record_win_data_ptr->position.name_label_x = record_win_data_ptr->position.play_bg_x + record_win_data_ptr->position.play_stop_button_w + 1;
    record_win_data_ptr->position.name_label_y = record_win_data_ptr->position.play_bg_y;
    record_win_data_ptr->position.name_label_w = record_win_data_ptr->position.play_bg_w - record_win_data_ptr->position.play_stop_button_w - 2;
    record_win_data_ptr->position.name_label_h = record_win_data_ptr->position.progress_point_y - record_win_data_ptr->position.play_bg_y;

    //judget name_label_w;
    if(record_win_data_ptr->position.name_label_w > win_rect.right - record_win_data_ptr->position.name_label_x)
    {
        record_win_data_ptr->position.name_label_w = win_rect.right - record_win_data_ptr->position.name_label_x;
    }

#endif

    //16.play_time_label;
#ifndef MMI_RECORD_MINI_SUPPORT 
    record_win_data_ptr->position.play_time_label_y = record_win_data_ptr->position.progress_point_y + record_win_data_ptr->position.progress_point_h + 1;
#else
    record_win_data_ptr->position.play_time_label_y = record_win_data_ptr->position.progress_point_y + record_win_data_ptr->position.progress_point_h - 1;
#endif
    record_win_data_ptr->position.play_time_label_h = record_win_data_ptr->position.play_bg_y + record_win_data_ptr->position.play_bg_h
            - (record_win_data_ptr->position.progress_point_y + record_win_data_ptr->position.progress_point_h);
    record_win_data_ptr->position.play_time_label_w = record_play_time_label_w;

    //17.play_all_time_label;
    record_win_data_ptr->position.play_all_time_label_y = record_win_data_ptr->position.play_time_label_y;
    record_win_data_ptr->position.play_all_time_label_h = record_win_data_ptr->position.play_time_label_h;
    record_win_data_ptr->position.play_all_time_label_w = record_play_all_time_label_w;

    record_win_data_ptr->position.play_time_label_x     = record_win_data_ptr->position.progress_point_w / 2 + record_left_rect_margin_w;
    record_win_data_ptr->position.play_all_time_label_x = record_right_rect_w - record_win_data_ptr->position.play_time_label_w - record_win_data_ptr->position.progress_point_w / 2 + record_left_rect_margin_w;
#ifdef MMI_RECORD_MINI_SUPPORT //replace above record_win_data_ptr->position.play_time_label_x,record_win_data_ptr->position.play_all_time_label_x;

    if(MMITHEME_IsMainScreenLandscape())
    {
        record_win_data_ptr->position.play_time_label_x     = record_left_rect_margin_w;
        record_win_data_ptr->position.play_all_time_label_x = record_right_rect_w - record_win_data_ptr->position.play_time_label_w + record_left_rect_margin_w;
    }

#endif
*/
}

/*****************************************************************************/
//  Description : update vol icon
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void UpdateVolIcon(MMI_HANDLE_T win_handle)
{


    GUI_BG_T buttonFg = {0};
    GUI_BG_T buttonFg_press = {0};

    buttonFg.bg_type = GUI_BG_IMG;
#ifndef MMI_RECORD_MINI_SUPPORT
    if(0 == MMIAPISET_GetMultimVolume())
    {
        buttonFg.img_id = IMAGE_RECORD_VOLUME_MUTE;
        buttonFg_press.img_id = IMAGE_RECORD_VOLUME_MUTE;
    }
    else
    {
        buttonFg.img_id = IMAGE_RECORD_VOLUME;
        buttonFg_press.img_id = IMAGE_RECORD_VOLUME;
    }
#endif
    GUIBUTTON_SetPressedFg(MMIRECORD_VOLUME_BUTTON_CTRL_ID, &buttonFg_press);
    GUIBUTTON_SetFg(MMIRECORD_VOLUME_BUTTON_CTRL_ID, &buttonFg);

    if(MMK_IsFocusWin(win_handle))
    {
        GUIBUTTON_Update(MMIRECORD_VOLUME_BUTTON_CTRL_ID);
    }

}



//#define RGB8882RGB565(rgb888)   ((((rgb888)&0xf80000)>>8)|(((rgb888)&0xfc00)>>5)|(((rgb888)&0xf8)>>3))
/*****************************************************************************/
//  Description : display record pointer
//  Global resource dependence : none
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordPointer(MMI_HANDLE_T win_handle)
{
#ifndef MMI_RECORD_MINI_SUPPORT
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    uint16 img_width = 0;
    uint16 img_height = 0;
    GUI_RECT_T rect = {0};
    ROTATE_ARBITRARY_IN_T	rot_in		= {0};
    ROTATE_ARBITRARY_OUT_T	rot_out		= {0};
    uint32              img_size       = 0;
    uint8*              img_ptr         = PNULL;
    uint32              rotated_buf_size       = 0;
    uint8*              rotated_buf_ptr         = PNULL;
    MMIRECORD_IMAGE_T src_img = {0};
    MMIRECORD_IMAGE_T dst_img = {0};
    uint8* decomp_buf_ptr = PNULL;
    uint32 decomp_buf_size = 0;
    uint8* layer_ptr = PNULL;
    RECORD_WIN_DATA_T       *record_win_data_ptr = PNULL;

    record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    do
    {
        rect.left = record_win_data_ptr->position.needle_bg_x;
        rect.top = record_win_data_ptr->position.needle_bg_y;
        rect.right = record_win_data_ptr->position.needle_bg_x + record_win_data_ptr->position.needle_bg_w;
        rect.bottom = record_win_data_ptr->position.needle_bg_y + record_win_data_ptr->position.needle_bg_h;
        GUIRES_DisplayImg(PNULL, &rect, PNULL, win_handle, IMAGE_RECORD_NEEDLE_BG, &lcd_dev_info);

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_RECORD_NEEDLE, win_handle);

        rotated_buf_size = (img_width + img_height) * (img_width + img_height) * sizeof(uint32);
        rotated_buf_ptr = SCI_ALLOCA((rotated_buf_size) * sizeof(uint8));

        if(PNULL == rotated_buf_ptr)
        {
            return;
        }

        decomp_buf_size = img_width * img_height * sizeof(uint32);
        decomp_buf_ptr = SCI_ALLOCA((decomp_buf_size) * sizeof(uint8));

        if(PNULL == decomp_buf_ptr)
        {
            break;
        }

        img_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_RECORD_NEEDLE, win_handle , &img_size);
        if(PNULL == img_ptr)
        {
            break;
        }
        src_img.data_ptr = img_ptr;
        src_img.data_size = img_size;
        src_img.data_type = IMGREF_FORMAT_RGB565;
        src_img.width = img_width;
        src_img.height = img_height;
        SCI_MEMSET(decomp_buf_ptr, 0, decomp_buf_size);

        if(!RecordDecompressARGB(&src_img, &dst_img, decomp_buf_ptr, decomp_buf_size))
        {
            break;
        }

        // if(status == MMIRECORD_STATE_START || status == MMIRECORD_STATE_RECORD_PAUSED)
        if(MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
        {
            rot_in.angle = MMIRECORD_ROTATE_POINTER_ANGLE;
        }
        else
        {
            RECORD_VOICEDB_T record_param = {0};
#ifdef WIN32
            static int16 angle = MMIRECORD_ROTATE_POINTER_ANGLE;
#endif

            record_param.db = MMIRECORD_GetRecordDB();

            if(record_param.db < 48)
            {
                rot_in.angle = MMIRECORD_ROTATE_POINTER_ANGLE + (int32)(record_param.db * (360 - MMIRECORD_ROTATE_POINTER_ANGLE) / 48);
            }
            else if(record_param.db >= 48)
            {
                rot_in.angle = (int32)((record_param.db - 48) * (360 - MMIRECORD_ROTATE_POINTER_ANGLE) / 48);
            }

#ifdef WIN32
            angle++;

            if(angle >= 360)
            {
                angle = 0;
            }

            if(angle == (360 - MMIRECORD_ROTATE_POINTER_ANGLE))
            {
                angle = MMIRECORD_ROTATE_POINTER_ANGLE;
            }

            rot_in.angle = angle;
#endif
        }

        rot_in.src_format = IMGREF_FORMAT_ARGB888;
        rot_in.src_size.w = img_width;
        rot_in.src_size.h = img_height;
        rot_in.src_center.x = img_width / 2;
        rot_in.src_center.y = img_height / 2;
        rot_in.src_chn.chn0.ptr = (void *)dst_img.data_ptr;
        rot_in.src_chn.chn0.size = img_width * img_height * sizeof(uint32);

        rot_in.target_format = IMGREF_FORMAT_ARGB888;
        rot_in.target_size.w = img_width + img_height;
        rot_in.target_size.h = img_width + img_height;
        rot_in.target_chn.chn0.ptr = (void *)rotated_buf_ptr;
        rot_in.target_chn.chn0.size = rotated_buf_size;
        rot_in.target_center.x = (img_width + img_height) / 2;
        rot_in.target_center.y = (img_width + img_height) / 2;

        rot_in.mode = ROTATE_ARBITRARY_AUTO_CROP;

        SCI_MEMSET(rotated_buf_ptr, 0, rotated_buf_size);

        if(SCI_SUCCESS != GRAPH_RotateArbitrary(&rot_in, &rot_out))
        {
            break;
        }

#ifdef UI_MULTILAYER_SUPPORT

        if(UILAYER_IsMultiLayerEnable())
        {
            UILAYER_APPEND_BLT_T append_layer = {0};
            UILAYER_COPYBUF_PARAM_T     copy_param = {0};
            UILAYER_COPYBUF_T           copy_buf = {0};

            append_layer.lcd_dev_info.lcd_id = record_win_data_ptr->lcd_dev_info.lcd_id;
            append_layer.lcd_dev_info.block_id = record_win_data_ptr->lcd_dev_info.block_id;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;

            UILAYER_SetLayerColorKey(&(record_win_data_ptr->lcd_dev_info), TRUE, UILAYER_TRANSPARENT_COLOR);

            copy_param.rect = UILAYER_GetLayerRect(&(record_win_data_ptr->lcd_dev_info));
            copy_param.rect.right = (copy_param.rect.left + img_width + img_height - 1);
            copy_param.rect.bottom = (copy_param.rect.top + img_width + img_height - 1);

            copy_buf.buf_ptr = (uint8*)rot_out.output_chn.chn0.ptr;
            copy_buf.data_type = DATA_TYPE_ARGB888;
            copy_buf.width = (img_width + img_height);
            copy_buf.height = (img_width + img_height);

            UILAYER_CopyBufferToLayer(&(record_win_data_ptr->lcd_dev_info), &copy_param, &copy_buf);

        }

#endif

        if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
        {
            rect.left = record_win_data_ptr->position.needle_bg_x;
            rect.top = record_win_data_ptr->position.needle_bg_y;
            rect.right = record_win_data_ptr->position.needle_bg_x + record_win_data_ptr->position.needle_bg_w;
            rect.bottom = record_win_data_ptr->position.needle_bg_y + record_win_data_ptr->position.needle_bg_h;
            GUIRES_DisplayImg(PNULL, &rect, PNULL, win_handle, IMAGE_RECORD_NEEDLE_MASK, &(record_win_data_ptr->lcd_dev_info));
        }
    }
    while(0);

    if(PNULL != rotated_buf_ptr) /*lint !e774*/
    {
        SCI_FREE(rotated_buf_ptr);
        rotated_buf_ptr = PNULL;
    }

    if(PNULL != decomp_buf_ptr) /*lint !e774*/
    {
        SCI_FREE(decomp_buf_ptr);
        decomp_buf_ptr = PNULL;
    }

    {
        GUI_LCD_DEV_INFO main_lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        GUI_RECT_T       layer_rect   = {0};
#ifdef UI_MULTILAYER_SUPPORT
        layer_rect = UILAYER_GetLayerRect(&(record_win_data_ptr->lcd_dev_info));
#endif

        {
            GUI_RECT_T client_rect = MMITHEME_GetClientRectEx(win_handle);

            if(layer_rect.bottom  > client_rect.bottom)
            {
                layer_rect.bottom = client_rect.bottom;
            }
        }

#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_BltLayerToLayer(&main_lcd_dev, &(record_win_data_ptr->lcd_dev_info), &layer_rect, TRUE);
#endif
    }

#endif
}
/*****************************************************************************/
//  Description : display progress
//  Global resource dependence : none
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void DisplayProgress(MMI_HANDLE_T win_handle)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T rect = {0};
    uint32 cur_time_elapsed = MMIRECORD_GetRecordSecondTime();
    uint32 total_time = MMIRECORD_GetRecordFileTotalTime();
    GUI_POINT_T display_point = {0};
    RECORD_WIN_DATA_T       *record_win_data_ptr = PNULL;
    uint16 percent = 0;


    SCI_TRACE_LOW("[RECORD] DisplayProgress cur_time_elapsed = %d, total_time = %d", cur_time_elapsed, total_time);

    record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr
            || (MMIRECORD_BUTTON_STATE_HIDE == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
                && MMIRECORD_BUTTON_STATE_HIDE == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]))
    {
        GUILABEL_SetVisible(MMIRECORD_PLAY_TIME_LABEL_CTRL_ID, FALSE, FALSE);
        GUILABEL_SetVisible(MMIRECORD_PLAY_ALL_TIME_LABEL_CTRL_ID, FALSE, FALSE);
        return;
    }

    /*if(1 == total_time)    //只有1s的时候
    {
        cur_time_elapsed = total_time;
    }
    else*/ 
    if(!record_win_data_ptr->is_begin
            && 0 < cur_time_elapsed)           //开始播放
    {
        record_win_data_ptr->is_begin = TRUE;
    }
    else if(record_win_data_ptr->is_begin
            && 0 == cur_time_elapsed)   //开始播放没有结束前，播放时间不能回到起点。
    {
        cur_time_elapsed = total_time;
    }
    
    if(((MMIRECORD_STATE_PLAYING == MMIRECORD_GetRecordState()) || 
        (MMIRECORD_STATE_PLAY_PAUSED == MMIRECORD_GetRecordState()))
        && (1 == total_time))
    {
        cur_time_elapsed = total_time;
    }

    rect.left = record_win_data_ptr->position.play_bg_x;
    rect.top = record_win_data_ptr->position.play_bg_y;
    rect.right = record_win_data_ptr->position.play_bg_x + record_win_data_ptr->position.play_bg_w;
    rect.bottom = record_win_data_ptr->position.play_bg_y + record_win_data_ptr->position.play_bg_h;
    GUIRES_DisplayImg(PNULL,
                      &rect,
                      PNULL,
                      win_handle,
                      IMAGE_RECORD_PLAY_BK_BG,
                      &lcd_dev_info);

    rect.left = record_win_data_ptr->position.progress_bg_x;
    rect.top = record_win_data_ptr->position.progress_bg_y;
    rect.right = record_win_data_ptr->position.progress_bg_x + record_win_data_ptr->position.progress_bg_w;
    rect.bottom = record_win_data_ptr->position.progress_bg_y + record_win_data_ptr->position.progress_bg_h;
    GUIRES_DisplayImg(PNULL,
                      &rect,
                      PNULL,
                      win_handle,
                      IMAGE_COMMON_PLAYER_PROCESS_GROOVE,
                      &lcd_dev_info);

    if(total_time == 0)
    {
        display_point.x = record_win_data_ptr->position.progress_bg_x - (record_win_data_ptr->position.progress_point_w / 2);
    }
    else if(total_time == cur_time_elapsed)
    {
        display_point.x = record_win_data_ptr->position.progress_bg_x + record_win_data_ptr->position.progress_bg_w - (record_win_data_ptr->position.progress_point_w / 2);
    }
    else
    {
        display_point.x = record_win_data_ptr->position.progress_bg_x - (record_win_data_ptr->position.progress_point_w / 2) + (record_win_data_ptr->position.progress_bg_w * (int16)cur_time_elapsed / (int16)total_time);
    }

    display_point.y = record_win_data_ptr->position.progress_point_y;
    rect.left = record_win_data_ptr->position.progress_bg_x;
    rect.top = record_win_data_ptr->position.progress_bg_y;
    rect.right = display_point.x + (record_win_data_ptr->position.progress_point_w / 2);
    rect.bottom = record_win_data_ptr->position.progress_bg_y + record_win_data_ptr->position.progress_bg_h;
    GUIRES_DisplayImg(PNULL,
                      &rect,
                      PNULL,
                      win_handle ,
                      IMAGE_COMMON_PLAYER_PROCESS,
                      &lcd_dev_info);

    GUIRES_DisplayImg(&display_point,
                      PNULL,
                      PNULL,
                      win_handle,
                      IMAGE_COMMON_PLAYER_PROCESS_SLIDE,
                      &lcd_dev_info);

    SetTimeLabelText(cur_time_elapsed, total_time);

}

/*****************************************************************************/
//  Description : get image id for record time
//  Global resource dependence :
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetTimeNumImageId(uint8 time_num)
{
    MMI_IMAGE_ID_T img_id = 0;

    switch(time_num)
    {
    case 0:
        img_id = IMAGE_RECORD_NUMBER_0;
        break;

    case 1:
        img_id = IMAGE_RECORD_NUMBER_1;
        break;

    case 2:
        img_id = IMAGE_RECORD_NUMBER_2;
        break;

    case 3:
        img_id = IMAGE_RECORD_NUMBER_3;
        break;

    case 4:
        img_id = IMAGE_RECORD_NUMBER_4;
        break;

    case 5:
        img_id = IMAGE_RECORD_NUMBER_5;
        break;

    case 6:
        img_id = IMAGE_RECORD_NUMBER_6;
        break;

    case 7:
        img_id = IMAGE_RECORD_NUMBER_7;
        break;

    case 8:
        img_id = IMAGE_RECORD_NUMBER_8;
        break;

    case 9:
        img_id = IMAGE_RECORD_NUMBER_9;
        break;

    case 10:
        img_id = IMAGE_RECORD_NUMBER_POINT;
        break;

    default:
        break;
    }

    return img_id;
}

/*****************************************************************************/
//  Description : DisplayRecordTimeImg
//  Global resource dependence :
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordTimeImg(MMI_HANDLE_T win_handle, GUI_RECT_T *rect_ptr, uint8 time_num)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_IMAGE_ID_T img_id = GetTimeNumImageId(time_num);
    GUIRES_DisplayImg(PNULL, rect_ptr, PNULL, win_handle, img_id, &lcd_dev_info);
}



/******************************************************************************/
//  Description: decompressed the ARGB image
//  Author: chunyou.zhu
//  Input:
//          src_img_ptr:	pointer of input image information
//			dst_buf_ptr:	pointer to the target buffer
//			dst_buf_size:	target buffer size
//  Output:
//			dst_img_ptr:	pointer to the decompressed image information
//  Return:
//			TRUE:			successful
//          FALSE:			failed
//  Note:
/******************************************************************************/
LOCAL BOOLEAN RecordDecompressARGB(const MMIRECORD_IMAGE_T *src_img_ptr, MMIRECORD_IMAGE_T *dst_img_ptr,
                                   uint8 *dst_buf_ptr, uint32 dst_buf_size)
{
#ifndef WIN32_TEST
    uint32			img_bytes	= 0;
    IMG_RES_SRC_T	img_src		= {0};
    IMG_RES_DST_T	img_dst		= {0};
    IMG_RES_INFO_T	img_info	= {0};
    IMG_RES_ERROR_E error		= 0;
    IMG_RES_DST_CTL_T dstctl   = {0};

    //get the compressed ARGB info, width and height
    //calc img_bytes
    img_src.imgstream_ptr = (uint8 *)src_img_ptr->data_ptr;
    img_src.imgstreamsize = src_img_ptr->data_size;

    error = IMG_RES_GetImageInfo(&img_src, &img_info);

    if(IMG_RES_SUCCESS != error)
    {
        return FALSE;
    }

    img_bytes = img_info.width * img_info.height * 4;

    if(PNULL == dst_buf_ptr || dst_buf_size < img_bytes)
    {
        return FALSE;
    }

    img_src.imgrect.left = 0;
    img_src.imgrect.top = 0;
    img_src.imgrect.right = img_info.width - 1;
    img_src.imgrect.bottom = img_info.height - 1;

    //decompress ARGB image
    img_dst.dstctl = &dstctl;
    img_dst.dstctl->bg_type = IMG_BG_NORMAL;
    img_dst.dstctl->format = IMGREF_FORMAT_ARGB888;
    img_dst.dstctl->dstmem = dst_buf_ptr;
    img_dst.dstctl->width = img_info.width;
    img_dst.dstctl->height = img_info.height;
    img_dst.dstrect.top = 0;
    img_dst.dstrect.left = 0;
    img_dst.dstrect.right = img_info.width - 1;
    img_dst.dstrect.bottom = img_info.height - 1;

    SCI_MEMSET(dst_buf_ptr, 0, dst_buf_size);

    error = IMG_RES_Display_Img(&img_src, &img_dst);

    if(IMG_RES_SUCCESS != error)
    {
        return FALSE;
    }

    *dst_img_ptr = *src_img_ptr;

    dst_img_ptr->data_ptr = dst_buf_ptr;
    dst_img_ptr->data_size = img_bytes;
    dst_img_ptr->data_type = IMGREF_FORMAT_ARGB888;
#endif
    return TRUE;
}
/*****************************************************************************/
//  Description : RecordCreateLayer
//  Global resource dependence :
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void RecordCreateLayer(MMI_WIN_ID_T      win_id)
{

#ifdef UI_MULTILAYER_SUPPORT
#if !defined(MMI_GUI_STYLE_TYPICAL) && !defined(MMI_RECORD_MINI_SUPPORT)
    if(UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};
        uint16 img_width = 0;
        uint16 img_height = 0;
        RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);

        if(PNULL == record_win_data_ptr)
        {
            return;
        }

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_RECORD_NEEDLE, win_id);
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = record_win_data_ptr->position.needle_center_point_x - (img_width + img_height) / 2;
        create_info.offset_y = record_win_data_ptr->position.needle_center_point_y - (img_width + img_height) / 2;
        create_info.width = img_width + img_height;
        create_info.height = img_width + img_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer(&create_info, &(record_win_data_ptr->lcd_dev_info));
    }
#endif
#endif

}
/*****************************************************************************/
//  Description : RecordReleaseLayer
//  Global resource dependence :
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void RecordReleaseLayer(MMI_WIN_ID_T      win_id)
{
#ifdef UI_MULTILAYER_SUPPORT
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    if(UILAYER_IsMultiLayerEnable())
    {
        UILAYER_RELEASELAYER(&(record_win_data_ptr->lcd_dev_info));   /*lint !e506 !e774*/
    }

#endif
}


/*Edit by script, ignore 2 case. Thu Apr 26 19:01:05 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
