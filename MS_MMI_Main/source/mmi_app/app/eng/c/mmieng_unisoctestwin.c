/*****************************************************************************
** File Name:      mmieng_uitestwin.c                                        *
** Author:                                                                   *
** Date:           09/2004                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 9/2004         Allen

******************************************************************************/

#define _MMIENG_UNISOCTESTWIN_C_

#ifdef MMI_UNISOC_MANUALLY_CIT_SUPPORT
/**--------------------------------------------------------------------------*
Include Files
**--------------------------------------------------------------------------*/
#include "mmi_app_eng_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"

#include "mmk_timer.h"
#include "mmieng_uitestwin.h"
#include "mmieng_export.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "mmi_theme.h"
#include "mmi_text.h"
#include "mmiidle_export.h"
#include "mmiacc.h"
#include "guilcd.h"
#include "guicommon.h"
#include "guitext.h"
#include "guilabel.h"
#include "mmiset_export.h"
#include "mmi_default.h"
#include "tb_dal.h"
#include "mmk_timer.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmi_osbridge.h"
#include "guifont.h"
#include "mmi_textfun.h"
#include "mmiidle_subwintab.h"
#include "gpio_prod_api.h"
#include "mmi_ring.h"
#include "mmisd_export.h"
#include "mmitv_out.h"
#include "mmi_appmsg.h"
#include "mmieng_text.h"
#include "mmieng_image.h"
#include "mmieng_id.h"
#include "mmieng_menutable.h"
#include "mmifm_export.h"
#include "mmifm_text.h"
#include "mmieng_nv.h"
#include "mmibt_export.h" 
#ifdef BLUETOOTH_SUPPORT
//#include "mmi_btinterface.h"
//#include "mmibt_text.h"
#include "mmibt_id.h"
#include "bt_abs.h"
#endif
#include "mmieng_internal.h"
#include "mmieng_position.h"

#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif

#include "mmifm_export.h"
#include "guiref_scale.h"
#include "mmiset_export.h"
#include "mmidc_export.h"
#include "guistring.h"
#include "guires.h"

#include "ref_param.h"
#include "msensor_drv.h"

#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#include "java_mmi_text.h"
#endif
#include "mmipub.h"
#include "mmisrvaud_api.h"
#include "guiform.h"
#include "guilistbox.h"
#include "sensor_atv.h"
#include "dal_recorder.h"
#include "prod_param.h"


#ifdef MEDIA_DEMO_SUPPORT
#include "aac_stream_demo.h"
#include "amr_stream_demo.h"
#include "amr_record_demo.h"
#include "wav_stream_demo.h"
#include "mp3_stream_demo.h"

#ifdef MIC_SAMPLE_SUPPORT
#include "mic_sample_demo.h"
#endif

#include "audio_express_demo.h"

#include "dc_video_demo.h"
#include "display_yuv_demo.h"

#ifdef CAT_MVOICE_SUPPORT
#include "cat_mvoice.h"
#include "cat_mvoice_demo.h"
#endif

#endif

//camera
#ifdef  CAMERA_SUPPORT
#include "dal_dcamera.h"
#include "sensor_drv.h"
#endif

#include "version.h"
#include "mmiset_id.h"
#include "audio_api.h"
#include "rpc_aud.h"
#include "mmifm_internal.h"
#include "lcm_prod.h"
#include "lcm_cfg.h"
#if defined(ZDT_SHOW_SIM_NETWORK_RSSI_IN_FACTORY_MODE)   
#include "mmiphone_export.h"
#endif

/**--------------------------------------------------------------------------*
MACRO DEFINITION
**--------------------------------------------------------------------------*/

//key test
#define UNISOC_TEST_KEY_TEXT_MAX_LEN             9
#define UNISOC_TEST_KEY_MARGIN_X			8
#define UNISOC_TEST_KEY_MARGIN_Y			8
#define UNISOC_TEST_KEY_HIGHT		40

typedef enum
{
	UNISOC_TEST_KEY_SL,
	UNISOC_TEST_KEY_SR,
	UNISOC_TEST_KEY_GREEN,
	UNISOC_TEST_KEY_RED,
	UNISOC_TEST_KEY_UP,
	UNISOC_TEST_KEY_DOWN,
	UNISOC_TEST_KEY_LEFT,
	UNISOC_TEST_KEY_RIGHT,
	UNISOC_TEST_KEY_WEB,
	UNISOC_TEST_KEY_HASH,
	UNISOC_TEST_KEY_STAR,
	//UNISOC_TEST_KEY_VOL_UP,
	//UNISOC_TEST_KEY_VOL_DOWN,
	UNISOC_TEST_KEY_0,
	UNISOC_TEST_KEY_1 ,
	UNISOC_TEST_KEY_2,
	UNISOC_TEST_KEY_3,
	UNISOC_TEST_KEY_4,
	UNISOC_TEST_KEY_5,
	UNISOC_TEST_KEY_6,
	UNISOC_TEST_KEY_7,
	UNISOC_TEST_KEY_8,
	UNISOC_TEST_KEY_9,
	UNISOC_TEST_KEY_MAX
} UNISOC_TEST_KEY_E;

#define UNISOC_TEST_KEY_COMPLETE_FLAG		0x1fffff

//lcd test 1000
#define UNISOC_TEST_LCD_DISPLAY_COLOR_NUM 5
#define UNISOC_TEST_LCD_DISPLAY_DELAY 1300

//vibrate test
#define UNISOC_TEST_VIBRATE_DELAY 1000*5

//keypad led test 500
#define UNISOC_TEST_KEYPAD_LED_DISPLAY_COUNT 4
#define UNISOC_TEST_KEYPAD_LED_DISPLAY_DELAY 1000

//charging test
#define UNISOC_TEST_CHARGING_DETECT_DELAY 500

/**--------------------------------------------------------------------------*
STATIC DEFINITION
**--------------------------------------------------------------------------*/
//for all
LOCAL MMI_STRING_T s_unisoctest_prompt_str = {0};
LOCAL const wchar s_unisoctest_prompt_wstr[] =
     { 'P', 'a', 's', 's',  ' ', 't', 'o', ' ', 'n', 'e', 'x', 't', '.', '\n',
        'F', 'a', 'i', 'l',  ' ', 't', 'o', ' ', 'e', 'x', 'i', 't', '.',  '\n',
        'O', 'r', ' ', 'R', 'e', 't', 'e', 's', 't', '.', '\n',
     0};

LOCAL MMI_TEXT_ID_T   s_unisoctest_softkey_id[GUISOFTKEY_BUTTON_NUM] = {0};
//key test
static uint8 s_unisoctest_key_ind_buf[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
static uint32 s_unisoctest_key_flag;
//lcd test
static uint8 s_unisoctest_lcd_timer_id = 0;
//vibrate test
static uint8 s_unisoctest_vibrate_timer_id = 0;
static uint32       s_mmieng_rtc_counter = 0;
static uint8        s_mmieng_testrtc_timer_id = 0;
LOCAL MMI_STRING_T s_mmieng_str_up_to_next = {0};
//speaker test
static uint8        s_unisoctest_speaker_volume = 0;
//keypad led test
static BOOLEAN  s_unisoctest_backlight_on = TRUE;
static uint8        s_unisoctest_backlight_timer_id = 0;
static uint8        s_unisoctest_backlight_count = 0;
//headset test
//static BOOLEAN  s_unisoctest_headset_is_in = FALSE;
//charging test
static uint8        s_unisoctest_charging_timer_id = 0;
//camera test
static GUI_LCD_DEV_INFO s_unisoctest_camera_osdlayer_handle;
static const wchar* s_unisoctest_file_name=L"cittest.jpg";
//bt test




/**--------------------------------------------------------------------------*
LOCAL FUNCTION DECLARE
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void MMIENG_RunUnisocTestInit(void);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL int32 ReadUnisocTestResultNV( uint32* is_tested, uint32* result  );

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL int32 WriteUnisocTestResultNV(uint32*  is_tested ,  uint32* result );

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL int32 SaveUnisocTestResult(uint32 win_id, uint32 result);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL int32 SetMMIUnisocTestResultBit(uint32* is_tested, uint32* result);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL  MMI_RESULT_E UnisocTestMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL  MMI_RESULT_E AgingTestMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL  MMI_RESULT_E UnisocTestResultWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestReadyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestLcdWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestVibrateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E UnisocTestRTCWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E UnisocTestTORCHWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL void UnisocTestSetLabelTextByCharArray(MMI_CTRL_ID_T ctrl_id, char *buffer,uint32 buffer_size, BOOLEAN  is_update);

#ifdef Z695_KEYLOCK_INT_SUPPORT
LOCAL MMI_RESULT_E UnisocTestKeylockWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestSpeakerWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestReceiverWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestKeypadLedWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestMicReceiverWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestMicSpeakerWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestHeadsetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestKeyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestMicHeadsetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestTFlashWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestSimWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestChargingWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestCameraWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestCameraCaptureWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
#ifdef BLUETOOTH_SUPPORT
LOCAL MMI_RESULT_E UnisocTestBTWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
#ifdef FM_SUPPORT
LOCAL MMI_RESULT_E UnisocTestFMWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

#if defined(ZDT_SHOW_SIM_NETWORK_RSSI_IN_FACTORY_MODE)
LOCAL MMI_RESULT_E UnisocTestSimNetworkWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif


LOCAL MMI_RESULT_E handleConfirmationDialogUNISOC(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                );
/**--------------------------------------------------------------------------*
GLOBAL DEFINITION
**--------------------------------------------------------------------------*/

WINDOW_TABLE(MMIENG_UNISOCTEST_MENU_WIN_TAB) =
{
	WIN_FUNC((uint32)UnisocTestMenuWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_MENU_WIN_ID),
	WIN_TITLE(TXT_ENG_PHONETEST),
	CREATE_MENU_CTRL( MENU_UNISOC_TEST, MMIENG_UITEST_MENU_CTRL_ID),
	WIN_SOFTKEY(TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN),
	END_WIN
};

WINDOW_TABLE(MMIENG_AGINGTEST_MENU_WIN_TAB) =
{
	WIN_FUNC((uint32)AgingTestMenuWinHandleMsg),
	WIN_ID(MMIENG_AGINGTEST_MENU_WIN_ID),
	WIN_TITLE(TXT_ENG_AGINGTEST),
	CREATE_MENU_CTRL( MENU_AGING_TEST, MMIENG_AGINGTEST_MENU_CTRL_ID),
	WIN_SOFTKEY(TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN),
	END_WIN
};


WINDOW_TABLE(MMIENG_UNISOCTEST_RESULT_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestResultWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_RESULT_WIN_ID),
	WIN_TITLE(TXT_ENG_PHONETEST),
	CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_ENG_RETURN),
	END_WIN
};
	
WINDOW_TABLE(MMIENG_UNISOCTEST_READY_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestReadyWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_READY_WIN_ID),
	WIN_TITLE(TXT_ENG_UITEST_TITLE),
	CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	WIN_SOFTKEY(TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_VERSION_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestVersionWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_VERSION_WIN_ID),
	WIN_TITLE(TXT_ENG_VERSION_INFO),
	CREATE_TEXT_CTRL( MMIENG_SHOW_VERSION_CTRL_ID),
	WIN_SOFTKEY(TXT_ENG_PASS, TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_MAINLCD_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestLcdWinHandleMsg),
	//WIN_TITLE(TXT_ENG_LCD_TEST),
	WIN_ID(MMIENG_UNISOCTEST_MAINLCD_WIN_ID),
	WIN_HIDE_STATUS,
//	CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_VIBRATE_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestVibrateWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_VIBRATE_WIN_ID),
	WIN_TITLE(TXT_ENG_VIBRATE_TEST),//TXT_ENG_UIVIBRATE_TITLE
//	CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};
WINDOW_TABLE(MMIENG_UNISOCTEST_RTC_WIN_TAB) = 
{
            WIN_FUNC((uint32)UnisocTestRTCWinHandleMsg),
            WIN_ID(MMIENG_UNISOCTEST_RTC_WIN_ID),
#ifdef MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
            WIN_TITLE(TXT_ENG_RTC_TEST),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            END_WIN
    };

WINDOW_TABLE(MMIENG_UNISOCTEST_TORCH_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestTORCHWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_TORCH_WIN_ID),
	WIN_TITLE(TXT_ENG_TORCHLED_TEST),
	CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};
#ifdef Z695_KEYLOCK_INT_SUPPORT
WINDOW_TABLE(MMIENG_UNISOCTEST_KEYLOCK_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestKeylockWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_KEYLOCK_WIN_ID),
	WIN_TITLE(TXT_ENG_KEYLOCK_TEST),
	CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};
#endif
WINDOW_TABLE(MMIENG_UNISOCTEST_SPEAKER_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestSpeakerWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_SPEAKER_WIN_ID),
	WIN_TITLE(TXT_ENG_SPEAKER_TEST),
//	CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	WIN_SOFTKEY(TXT_ENG_PASS , TXT_ENG_RETEST, TXT_ENG_FAIL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_RECEIVER_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestReceiverWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_RECEIVER_WIN_ID),
	WIN_TITLE(TXT_ENG_RECEIVER_TEST),
//	CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	WIN_SOFTKEY(TXT_ENG_PASS , TXT_ENG_RETEST, TXT_ENG_FAIL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_KEYPAD_LED_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestKeypadLedWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_KEYPAD_LED_WIN_ID),
	WIN_TITLE(TXT_ENG_KEYPAD_LED_TEST),
	CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL , TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_MIC_RECEIVER_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestMicReceiverWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_MIC_RECEIVER_WIN_ID),
	WIN_TITLE(TXT_ENG_MIC_RECEIVER_TEST),
//	CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL , TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_MIC_SPEAKER_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestMicSpeakerWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_MIC_SPEAKER_WIN_ID),
	WIN_TITLE(TXT_ENG_MIC_SPEAKER_TEST),
//	CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_HEADSET_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestHeadsetWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_HEADSET_WIN_ID),
	WIN_TITLE(TXT_ENG_HEADSET_TEST),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL , TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_KEY_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestKeyWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_KEY_WIN_ID),
	WIN_HIDE_STATUS,
	//WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_MIC_HEADSET_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestMicHeadsetWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_MIC_HEADSET_WIN_ID),
	WIN_TITLE(TXT_ENG_MIC_HEADSET_TEST),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL , TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_TFLASH_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestTFlashWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_TFLASH_WIN_ID),
	WIN_TITLE(TXT_ENG_TFLASH_TEST),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL , TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_SIM_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestSimWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_SIM_WIN_ID),
	WIN_TITLE(TXT_ENG_SIM_TEST),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL , TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_CHARGING_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestChargingWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_CHARGING_WIN_ID),
	WIN_TITLE(TXT_ENG_CHARGING_TEST),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL2_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL3_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL4_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL , TXT_NULL, TXT_NULL),
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_CAMERA_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestCameraWinHandleMsg),
#ifndef _WIN32
	WIN_STYLE( WS_DISABLE_COMMON_BG),
	WIN_ID(MMIENG_UNISOCTEST_CAMERA_WIN_ID),
	WIN_HIDE_STATUS,
#else
	WIN_TITLE(TXT_ENG_CAMERA_TEST),
	WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
#endif
	END_WIN
};

WINDOW_TABLE(MMIENG_UNISOCTEST_CAMERA_CAPTURE_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestCameraCaptureWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_CAMERA_CAPTURE_WIN_ID),
	WIN_HIDE_STATUS,
	WIN_STYLE (WS_HAS_ANIM_BG | WS_DISABLE_COMMON_BG),
	CREATE_ANIM_CTRL (UNISOC_TEST_CAMERA_ANIM_CTRL_ID, MMIENG_UNISOCTEST_CAMERA_CAPTURE_WIN_ID),
	WIN_SOFTKEY (TXT_NULL, TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};
#ifdef FM_SUPPORT
WINDOW_TABLE(MMIENG_UNISOCTEST_FM_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestFMWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_FM_WIN_ID),
	WIN_TITLE(TXT_ENG_FM_TEST),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL , TXT_NULL, TXT_ENG_FAIL),
	END_WIN
};
#endif

#if defined(ZDT_SHOW_SIM_NETWORK_RSSI_IN_FACTORY_MODE)
WINDOW_TABLE(MMIENG_UNISOCTEST_SIM_NETWORK_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestSimNetworkWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_SIM_NETWORK_WIN_ID),
	WIN_TITLE(TXT_ENG_SIM_NETWORK_TEST),
	//CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
	WIN_SOFTKEY(TXT_ENG_OK , TXT_NULL, TXT_ENG_RETURN),
	END_WIN
};

#endif
#ifdef BLUETOOTH_SUPPORT
WINDOW_TABLE(MMIENG_UNISOCTEST_BT_WIN_TAB) = 
{
	WIN_FUNC((uint32)UnisocTestBTWinHandleMsg),
	WIN_ID(MMIENG_UNISOCTEST_BT_WIN_ID),
	WIN_TITLE(TXT_ENG_BT_TEST),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_TEXTBOX_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_ENG_FAIL ),
	END_WIN
};
#endif

//s_unisoc_test_win_ptr_tab must match with s_unisoc_test_win_id    
const  uint32*  s_unisoc_test_win_ptr_tab[] =
{
	MMIENG_UNISOCTEST_READY_WIN_TAB,
	MMIENG_UNISOCTEST_VERSION_WIN_TAB,
	MMIENG_UNISOCTEST_SIM_WIN_TAB,
	MMIENG_UNISOCTEST_TFLASH_WIN_TAB,
	MMIENG_UNISOCTEST_MAINLCD_WIN_TAB,
	MMIENG_UNISOCTEST_RECEIVER_WIN_TAB,
	MMIENG_UNISOCTEST_SPEAKER_WIN_TAB,
	MMIENG_UNISOCTEST_MIC_RECEIVER_WIN_TAB,
	MMIENG_UNISOCTEST_MIC_SPEAKER_WIN_TAB,
	#ifdef  CAMERA_SUPPORT  
	MMIENG_UNISOCTEST_CAMERA_WIN_TAB,
	MMIENG_UNISOCTEST_CAMERA_CAPTURE_WIN_TAB,
	#endif
	#if 0
def MMI_TORCH_LED_SUPPORT
	MMIENG_UNISOCTEST_TORCH_WIN_TAB,
	#endif
#ifdef ZDT_PRODUCT_VIBRATE
	MMIENG_UNISOCTEST_VIBRATE_WIN_TAB,
#endif
	MMIENG_UNISOCTEST_KEYPAD_LED_WIN_TAB,
	MMIENG_UNISOCTEST_KEY_WIN_TAB,
#ifdef FM_SUPPORT
	MMIENG_UNISOCTEST_FM_WIN_TAB,
#endif
#ifdef Z695_KEYLOCK_INT_SUPPORT
	MMIENG_UNISOCTEST_KEYLOCK_WIN_TAB,
#endif
	//MMIENG_UNISOCTEST_HEADSET_WIN_TAB,
	//MMIENG_UNISOCTEST_MIC_HEADSET_WIN_TAB,
#ifdef BLUETOOTH_SUPPORT
	MMIENG_UNISOCTEST_BT_WIN_TAB,
#endif
	MMIENG_UNISOCTEST_CHARGING_WIN_TAB,
	MMIENG_UNISOCTEST_RTC_WIN_TAB,

	PNULL
};

//s_unisoc_test_win_id must match with s_unisoc_test_win_ptr_tab
const  uint32 s_unisoc_test_win_id[] =
{
	MMIENG_UNISOCTEST_VERSION_WIN_ID,
	MMIENG_UNISOCTEST_SIM_WIN_ID,
	MMIENG_UNISOCTEST_TFLASH_WIN_ID,
	MMIENG_UNISOCTEST_MAINLCD_WIN_ID,
	MMIENG_UNISOCTEST_RECEIVER_WIN_ID,
	MMIENG_UNISOCTEST_SPEAKER_WIN_ID,
	MMIENG_UNISOCTEST_MIC_RECEIVER_WIN_ID,
	MMIENG_UNISOCTEST_MIC_SPEAKER_WIN_ID,
	MMIENG_UNISOCTEST_CAMERA_WIN_ID,
	MMIENG_UNISOCTEST_CAMERA_CAPTURE_WIN_ID,
	MMIENG_UNISOCTEST_TORCH_WIN_ID,
	MMIENG_UNISOCTEST_VIBRATE_WIN_ID,
	MMIENG_UNISOCTEST_KEYPAD_LED_WIN_ID,
	MMIENG_UNISOCTEST_KEY_WIN_ID,	
	MMIENG_UNISOCTEST_FM_WIN_ID,
	MMIENG_UNISOCTEST_HEADSET_WIN_ID,
	MMIENG_UNISOCTEST_MIC_HEADSET_WIN_ID,
	MMIENG_UNISOCTEST_BT_WIN_ID,
	MMIENG_UNISOCTEST_CHARGING_WIN_ID,
	MMIENG_UNISOCTEST_RTC_WIN_ID,

};

const uint32 s_all_unisoc_test_win_id[]  = 
{
	MMIENG_UNISOCTEST_VERSION_WIN_ID,
	MMIENG_UNISOCTEST_SIM_WIN_ID,
	MMIENG_UNISOCTEST_TFLASH_WIN_ID,
	MMIENG_UNISOCTEST_MAINLCD_WIN_ID,
	MMIENG_UNISOCTEST_RECEIVER_WIN_ID,
	MMIENG_UNISOCTEST_SPEAKER_WIN_ID,
	MMIENG_UNISOCTEST_MIC_RECEIVER_WIN_ID,
	MMIENG_UNISOCTEST_MIC_SPEAKER_WIN_ID,
	MMIENG_UNISOCTEST_CAMERA_WIN_ID,
	MMIENG_UNISOCTEST_CAMERA_CAPTURE_WIN_ID,
	MMIENG_UNISOCTEST_TORCH_WIN_ID,
	MMIENG_UNISOCTEST_VIBRATE_WIN_ID,
	MMIENG_UNISOCTEST_KEYPAD_LED_WIN_ID,
	MMIENG_UNISOCTEST_KEY_WIN_ID,	
	MMIENG_UNISOCTEST_FM_WIN_ID,
	MMIENG_UNISOCTEST_HEADSET_WIN_ID,
	MMIENG_UNISOCTEST_MIC_HEADSET_WIN_ID,
	MMIENG_UNISOCTEST_BT_WIN_ID,
	MMIENG_UNISOCTEST_CHARGING_WIN_ID,
	MMIENG_UNISOCTEST_RTC_WIN_ID,

};

//all test name must match with all test win id
const char*  s_all_unisoc_test_name[] =
{
	"Version",
	"SIM",      
	"T-Flash",      
	"Lcd",
	"Receiver",
	"Speaker",
	"Mic-Receiver",
	"Mic-Speaker",      
	"Camera",
	"Camera Capture",
	"Torch",
	"Vibrate",
	"Keypad Led",
	"Key",
	"FM",
	"Headset",      
	"Mic-Headset",   
	"BT",
	"Charging",
	"Rtc",
};

LOCAL  uint32 s_is_fulltest_unisoc = 1;    //default as full test mode
LOCAL  uint32 s_is_tested_unisoc = 0;      //0:not tested; 1,tested
LOCAL  uint32 s_test_result_unisoc = 0;    //test result in memory
LOCAL  int    s_unisoc_test_win_tab_index = 0;
LOCAL  BOOLEAN s_test_fail = FALSE;

/**--------------------------------------------------------------------------*
EXTERNAL DECLARE
**--------------------------------------------------------------------------*/
extern void ARMVB_TestModeStart_Direct (AUDIO_DEVICE_MODE_TYPE_E eMode, AUD_INPUT_TYPE_T input_type, uint32 uiVol, BOOL start);
/*===============================================================================
IMPLEMENTATION
===============================================================================*/

#ifdef WIN32
void ARMVB_TestModeStart_Direct (AUDIO_DEVICE_MODE_TYPE_E eMode, AUD_INPUT_TYPE_T input_type, uint32 uiVol, BOOL start)
{

}
#endif
void MMIENG_UnisocTest_CreatWin(void)
{
	s_is_fulltest_unisoc = 0 ;
	MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_KEY_WIN_TAB,  PNULL);
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL  MMI_RESULT_E UnisocTestMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint32 group_id = 0;
	uint32 menu_id = 0;
	
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);

		MMK_SetAtvCtrl(win_id, MMIENG_UITEST_MENU_CTRL_ID);
		break;
		
	case MSG_KEYDOWN_RED:	
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL:
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMK_CloseWin(win_id);           
		break;
		
	case MSG_CTL_MIDSK:
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB: 
		MMIENG_RunUnisocTestInit();
		GUIMENU_GetId(MMIENG_UITEST_MENU_CTRL_ID, &group_id, &menu_id);
		switch (menu_id)
		{
		case ID_UNISOC_TEST_FULLTEST:
			s_is_fulltest_unisoc = 1;
			MMIAPIENG_StartUnisocTest();
			break;
		case ID_UNISOC_TEST_VIEWRESULT:
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_RESULT_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_VERSION:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_VERSION_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_MAINLCD:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_MAINLCD_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_VIBRATER:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_VIBRATE_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_RTC:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_RTC_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_SPEAKER:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_SPEAKER_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_RECEIVER:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_RECEIVER_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_KEYPAD_LED:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_KEYPAD_LED_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_MIC_RECEIVER:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_MIC_RECEIVER_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_MIC_SPEAKER:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_MIC_SPEAKER_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_HEADSET:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_HEADSET_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_KEY:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_KEY_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_MIC_HEADSET:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_MIC_HEADSET_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_TFLASH:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_TFLASH_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_SIM:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_SIM_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_CHARGING:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_CHARGING_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_CAMERA:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_CAMERA_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_CAMERA_CAPTURE:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_CAMERA_CAPTURE_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_TORCH:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_TORCH_WIN_TAB,  PNULL);
			break;
#ifdef Z695_KEYLOCK_INT_SUPPORT
		case ID_UNISOC_TEST_KEYLOCK:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_KEYLOCK_WIN_TAB,  PNULL);
			break;
#endif
#ifdef FM_SUPPORT
		case ID_UNISOC_TEST_FM:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_FM_WIN_TAB,  PNULL);
			break;
#endif

#ifdef BLUETOOTH_SUPPORT
		case ID_UNISOC_TEST_BT:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_BT_WIN_TAB,  PNULL);
			break;
#endif
		case ID_UNISOC_TEST_RESET:
			s_is_fulltest_unisoc = 0 ;
			MMIAPIENG_RunUnisocTestRESET();
			break;
	
		default:
			break;
		}
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}


LOCAL  MMI_RESULT_E AgingTestMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint32 group_id = 0;
	uint32 menu_id = 0;
	
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);

		MMK_SetAtvCtrl(win_id, MMIENG_AGINGTEST_MENU_CTRL_ID);
		break;
	case MSG_KEYDOWN_RED:	
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL:
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMK_CloseWin(win_id);           
		break;
		
	case MSG_CTL_MIDSK:
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB: 
		
		GUIMENU_GetId(MMIENG_AGINGTEST_MENU_CTRL_ID, &group_id, &menu_id);
		switch (menu_id)
		{
		
		case ID_UNISOC_TEST_MAINLCD:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_MAINLCD_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_VIBRATER:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_VIBRATE_WIN_TAB,  PNULL);
			break;
			case ID_UNISOC_TEST_SPEAKER:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_SPEAKER_WIN_TAB,  PNULL);
			break;
		case ID_UNISOC_TEST_RECEIVER:
			s_is_fulltest_unisoc = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UNISOCTEST_RECEIVER_WIN_TAB,  PNULL);
			break;
	
		default:
			break;
		}
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL int32 GetUnisocTestResultInfo(char* buffer, int32 buffer_length)
{
    int32 offset = 0;
    uint32 is_tested = 0 ;
    uint32 test_result = 0;     //full test result
    uint32 item_result = 0;     //item test result
    int32 str_length = 0 ;
    char temp_str[16] = {0};
    uint32 i=0 ;
    uint32 j=0 ;
    
    const char *if_tested[] = {"not tested", "tested"};
    const char *if_passed[] = {"failed", "passed"};
    
    int32 tested_length[]  = {0,0};
    int32 passed_length[]  = {0,0};
    
    tested_length[0] = SCI_STRLEN(if_tested[0]);
    tested_length[1] = SCI_STRLEN(if_tested[1]);
    passed_length[0] = SCI_STRLEN(if_passed[0]);
    passed_length[1] = SCI_STRLEN(if_passed[1]);    
    
    ReadUnisocTestResultNV(&is_tested, &test_result);  
    
    str_length = SCI_STRLEN("<Test Result>\n");
    if ( offset + str_length >= buffer_length)
    {
        return offset ;
    }    
    SCI_MEMCPY(buffer + offset,  "<Test Result>\n",  str_length);
    offset += str_length;  
    
    
    for(i = 0,j =1; i < ARR_SIZE(s_all_unisoc_test_win_id); i++)
    {
        if(0 == s_all_unisoc_test_win_id[i])
        {
            continue;
        }   
        
        //serial num               
        sprintf(temp_str,"<%d> ", j);
        str_length = SCI_STRLEN(temp_str) ;
        if ( offset + str_length >= buffer_length )
        {
            return offset ;
        }
        SCI_MEMCPY(buffer + offset, temp_str, str_length);      
        offset += str_length;   
        
        //item 
        str_length =  SCI_STRLEN(s_all_unisoc_test_name[i]) ; 
        if ( offset + str_length >= buffer_length )
        {
            return offset ;
        }      
        SCI_MEMCPY(buffer + offset, s_all_unisoc_test_name[i], str_length) ;
        offset += str_length;
        
        //space key
        if ( offset + 1 >= buffer_length)
        {
            return offset ;
        }   
        buffer[offset] = BL_CHAR;
        offset += 1;
        
        //if tested
        item_result = (is_tested >>i) & 0x00000001 ;            
        str_length =  tested_length[item_result] ;
        if ( offset + str_length >= buffer_length ) 
        {
            return offset ;
        }
        SCI_MEMCPY(buffer + offset, if_tested[item_result], str_length) ;
        offset += str_length;    
        
        if(1 == item_result)
        {     //space key
            if ( offset + 1 >= buffer_length)
            {
                return offset ;
            }   
            buffer[offset] = BL_CHAR;
            offset += 1;
            //result
            item_result = (test_result >>i) & 0x00000001 ;            
            str_length =   passed_length[item_result] ;
            if ( offset + str_length >= buffer_length ) 
            {
                return offset ;
            }
            SCI_MEMCPY(buffer + offset, if_passed[item_result], str_length) ;
            offset += str_length;              
        } 
        //enter key
        if ( offset + 1 >= buffer_length)
        {
            return offset ;
        }   
        buffer[offset] = CR_CHAR;
        offset += 1;
        
        j++;
    }      
    
    return offset ;
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL  MMI_RESULT_E UnisocTestResultWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIENG_UITEST_TEXT_CTRL_ID);
        {
            uint32  buffer_length = 1023 ;
            uint32  str_length = 0;
            char    buffer[1024] = {0};
            wchar   wstr[1024] = {0} ;
            str_length = GetUnisocTestResultInfo(buffer, buffer_length) ;
            MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)buffer, buffer_length, str_length) ;
            GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, wstr, (uint16)str_length, FALSE);
	        GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, TRUE);

        }
        
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:            
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB: 
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestReadyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_STRING_T     str = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			MMI_GetLabelTextByLang(TXT_ENG_SLPASS_SRFAIL , &str);
		//	const char *str = {"Please press left key when pass.\nPress right key when failed."} ;
		/*	uint32  length = 0;
			wchar   wstr[256] = {0} ;
			length = SCI_STRLEN(str);
			MMI_STRNTOWSTR(wstr, 255, (uint8*)str, length, length);*/
			GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, str.wstr_ptr,str.wstr_len,  FALSE);
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);

		}
		break;
		
	case MSG_FULL_PAINT:
		break;
		
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);
		break;
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
		
	case MSG_KEYDOWN_RED:
		break;
		
	case MSG_CLOSE_WINDOW:
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 显示版本信息(包括软件,硬件,base版本)
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void UnisocTestShowVersionInfo(void)                         
{
    wchar           unicode_cr = 0x0d;
    MMI_STRING_T    cr_s = {0};
    MMI_STRING_T    text_s = {0};
    uint16          uint16_str[MMIENG_3RDPARTYVER_TEXT_LEN + 1] = {0};
    uint16          uint16_str_len = 0;
    uint16 cal_ok[10] = {0x5DF2, 0x6821, 0x51C6, 0, 0}; //已校准5DF2 6821 51C6 
    uint16 cal_fail[10] = {0x672A, 0x6821, 0x51C6, 0, 0}; //未校准672A 6821 51C6 
    uint16 tts_ok[10] = {0x0054, 0x0054, 0x0053, 0x5DF2, 0x6FC0, 0x6D3B, 0}; //TTS已激活0054 0054 0053 5DF2 6FC0 6D3B 
    uint16 tts_fail[10] = {0x0054, 0x0054, 0x0053, 0x672A, 0x6FC0, 0x6D3B, 0}; //TTS未激活0054 0054 0053 672A 6FC0 6D3B 
    char*           temp_ptr = PNULL;
    uint16          temp_len = 0;
    uint8            pcode[12];	//product code
    BOOLEAN is_calibration = FALSE;
    uint32 cal_info = 0;
    LCM_CFG_INFO_T_PTR cfg_info_ptr = PNULL;
    uint16          number = 0;
    uint32 			   count		= 0;
    cr_s.wstr_ptr = &unicode_cr;
    cr_s.wstr_len = 1;
	
    is_calibration = ZDT_CheckChipCalibrationed(&cal_info);
    if(is_calibration)
    {
        SCI_MEMCPY(uint16_str,cal_ok,6);
    }
    else
    {
        SCI_MEMCPY(uint16_str,cal_fail,6);
    }
    uint16_str_len = 3;
    uint16_str[uint16_str_len++] = unicode_cr;
#ifdef TTS_SUPPORT_YOUNGTONE_ROM
#ifndef WIN32
    if (0 != yt_tts_is_active_300())
    {
        SCI_MEMCPY(&uint16_str[uint16_str_len],tts_fail,12);
    }
    else
    {
        SCI_MEMCPY(&uint16_str[uint16_str_len],tts_ok,12);
    }
    uint16_str_len += 6;
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);
#endif
#endif
#if 0    
	//Software V:
    temp_ptr = (char*)"SV:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	
#ifndef _WIN32
    temp_ptr = VERSION_GetInfo(PROJECT_VERSION);   /*lint !e605*/
#else
    temp_ptr = (char*)"Platform Version:  MOCOR_W09.23_Debug";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	//rfid,product 
#ifndef _WIN32
 	temp_ptr = SCI_GET_RF_ID();
#else
	temp_ptr =(char*)"RFPA ID:000";
#endif
//end houlili
	temp_len = SCI_STRLEN((char*)temp_ptr);
 	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
 	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换?
 	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行?
#ifndef _WIN32
	LP_CODE_Read(pcode);
	temp_ptr = (char*)"NTD code:";
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	temp_ptr = (char*)pcode;
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	temp_ptr = (char*)"_";
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	PRODUCT_CODE_Read(pcode);
	temp_ptr = (char*)pcode;
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	temp_ptr = (char*)"_";
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	Ndt_Version_Read(pcode);
	temp_ptr = (char*)pcode;
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);

	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#endif
#endif
	//Project V:
    temp_ptr = (char*)"Project V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	
#ifndef _WIN32
    temp_ptr = VERSION_GetInfo(PROJECT_VERSION);   /*lint !e605*/
#else
    temp_ptr = (char*)"Project Version:  sc7701_320X480_PDA_64MB";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	
	//Hardware V:
    temp_ptr = (char*)"Hardware V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	
#ifndef _WIN32
    temp_ptr = VERSION_GetInfo(HW_VERSION);     /*lint !e605*/
#else
	//    temp_ptr = "SP7100A-V1.0.1-B7-M6416 SP7100A-V1.0.1-B7-M6416";	// this is a example for simulator
    temp_ptr = (char*)"HW Version:        SC6600L_sp6601l";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#ifndef WIN32
    temp_ptr = (char*)"LCD Info:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	
    cfg_info_ptr = LCM_GetCfgInfo( 0, &count );
    number = PROD_GetPeripheralNVParam( PROD_NV_ID_MAIN_LCD );
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)cfg_info_ptr[number].name, strlen(cfg_info_ptr[number].name));
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
 //   MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#endif
#if 0
	//Base V:
    temp_ptr = (char*)"Base V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	
#ifndef _WIN32
#ifdef PLATFORM_SC6800H
    temp_ptr = SCI_GetBaseVersionInfo();
#else
    temp_ptr = VERSION_GetInfo(BASE_VERSION);     /*lint !e605*/
#endif
#else
	//    temp_ptr = "BASE_1.0008";	// this is a example for simulator
    temp_ptr = (char*)"BASE Version:     	 BASE_W09.23";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#endif	
	
	//Build Time:
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    temp_ptr = (char*)"Build Version:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	
#ifndef _WIN32
    temp_ptr = VERSION_GetInfo(BUILD_TIME);     /*lint !e605*/
#else
    temp_ptr = (char*)"2010-03-24";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	
    text_s.wstr_ptr = uint16_str;
    text_s.wstr_len = uint16_str_len;
    GUITEXT_SetString(MMIENG_SHOW_VERSION_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, FALSE);
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		MMK_SetAtvCtrl(win_id, MMIENG_SHOW_VERSION_CTRL_ID);
		UnisocTestShowVersionInfo();
		GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		break;
		
	case MSG_FULL_PAINT:
		break;
		
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
		SaveUnisocTestResult(win_id, 1);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);
		break;
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL:
		SaveUnisocTestResult(win_id, 0);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);
		break;
		
		
	case MSG_KEYDOWN_RED:
		break;
		
	case MSG_CLOSE_WINDOW:
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
LOCAL void UnisocTestEditWinDrawFocusRect(
                                GUI_RECT_T rect,
                                GUI_COLOR_T color
                                )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};    
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.top, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.left, rect.top, rect.bottom, color);
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.bottom, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.right, rect.top, rect.bottom, color);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void UnisocTestDrawTestKeyBG( void )
{
    uint16      x = 0;
    uint16      y = 0;
    wchar       disp_text[9] = {0};
    GUI_RECT_T  draw_rect = MMITHEME_GetFullScreenRect();
    int16       TESTPAD_KEY_WIDTH = (draw_rect.right - draw_rect.left - UNISOC_TEST_KEY_MARGIN_X*2)>>2 ;
    const GUI_LCD_DEV_INFO  *lcd_dev_info = MMITHEME_GetDefaultLcdDev() ;
    MMI_STRING_T        string = {0};
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
	
    char s_test_key_text[24][UNISOC_TEST_KEY_TEXT_MAX_LEN] =
    {
        {"SR"},     {"Green"},  {"Up"},     {"Down"},
        {"SL"},     {"Red"},    {"Left"},   {"Right"},
        {"1"},      {"2"},      {"3"},      {"4"},
        {"5"},      {"6"},      {"7"},      {"8"},
        {"9"},      {"0"},      {"*"},      {"#"},
        {"Web"},    {" "}, {" "}, {" "}
    };
	
    
    draw_rect.top = 0;
    LCD_FillRect(lcd_dev_info, draw_rect, UITEST_COLOR_WHITE);
    
    for (y=0;y<6;y++)
    {
        for (x=0;x<4;x++)
        {
            draw_rect.left = (uint16)UNISOC_TEST_KEY_MARGIN_X + (uint16)(x * TESTPAD_KEY_WIDTH);
            draw_rect.top = UNISOC_TEST_KEY_MARGIN_Y + y * UNISOC_TEST_KEY_HIGHT;
            draw_rect.right = draw_rect.left + TESTPAD_KEY_WIDTH-1;
            draw_rect.bottom = draw_rect.top + UNISOC_TEST_KEY_HIGHT-1;
            UnisocTestEditWinDrawFocusRect(draw_rect, UITEST_COLOR_BLACK);
            
            string.wstr_len = strlen(s_test_key_text[y*4+x]);
            MMI_STRNTOWSTR(disp_text,UNISOC_TEST_KEY_TEXT_MAX_LEN, (uint8*)s_test_key_text[y*4+x],string.wstr_len,string.wstr_len);
            string.wstr_ptr = disp_text;
            
            // display
            text_style.align = ALIGN_HVMIDDLE;
            text_style.font = MMI_DEFAULT_TEXT_FONT;
            text_style.font_color = MMI_BLACK_COLOR;
            //  for(;GUI_GetStringWidth(text_style.font, string.wstr_ptr,string.wstr_len)>(TESTPAD_KEY_WIDTH-1)&&(text_style.font>2);text_style.font-=2);
            while(GUI_GetStringWidth(text_style.font, string.wstr_ptr,string.wstr_len)>(TESTPAD_KEY_WIDTH-1)&&(text_style.font>2))
            {
                text_style.font-=2    ;         
            }
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)lcd_dev_info,
                (const GUI_RECT_T      *)&draw_rect,       //the fixed display area
                (const GUI_RECT_T      *)&draw_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&string,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 
        }
    }
    
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN UnisocTestKeyboard(MMI_MESSAGE_ID_E key_msg_id)
{
	int16               test_key_x      = 0;
	int16               test_key_y      = 0;
	int16               key_on_map_idx  = 0;
	int16               byte_idx        = 0;
	int16               bit_idx         = 0;
	uint8               key_buf_byte    = 0x00;
	GUI_RECT_T          draw_rect       = {0};
	GUI_RECT_T          update_rect     = {0};
	GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	
	
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_1908_112_2_18_2_17_27_62,(uint8*)"d",key_msg_id);
	
	
	switch (key_msg_id)
	{
	case MSG_KEYDOWN_1:
		test_key_y = 2;
		test_key_x = 0;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_1);
		break;
		
	case MSG_KEYDOWN_2:
		test_key_y = 2;
		test_key_x = 1;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_2);
		break;
		
	case MSG_KEYDOWN_3:
		test_key_y = 2;
		test_key_x = 2;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_3);
		break;
		
	case MSG_KEYDOWN_4:
		test_key_y = 2;
		test_key_x = 3;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_4);
		break;
		
	case MSG_KEYDOWN_5:
		test_key_y = 3;
		test_key_x = 0;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_5);
		break;
		
	case MSG_KEYDOWN_6:
		test_key_y = 3;
		test_key_x = 1;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_6);
		break;
		
	case MSG_KEYDOWN_7:
		test_key_y = 3;
		test_key_x = 2;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_7);
		break;
		
	case MSG_KEYDOWN_8:
		test_key_y = 3;
		test_key_x = 3;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_8);
		break;
		
	case MSG_KEYDOWN_9:
		test_key_y = 4;
		test_key_x = 0;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_9);
		break;
		
	case MSG_KEYDOWN_0:
		test_key_y = 4;
		test_key_x = 1;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_0);
		break;
		
	case MSG_KEYDOWN_STAR:
		test_key_y = 4;
		test_key_x = 2;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_STAR);
		break;
		
	case MSG_KEYDOWN_HASH:
		test_key_y = 4;
		test_key_x = 3;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_HASH);
		break;
		
	case MSG_KEYDOWN_UP:
		test_key_y = 0;
		test_key_x = 2;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_UP);
		break;
		
	case MSG_KEYDOWN_DOWN:
		test_key_y = 0;
		test_key_x = 3;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_DOWN);
		break;
		
	case MSG_KEYDOWN_LEFT:
		test_key_y = 1;
		test_key_x = 2;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_LEFT);
		break;
		
	case MSG_KEYDOWN_RIGHT:
		test_key_y = 1;
		test_key_x = 3;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_RIGHT);
		break;
		
	case MSG_KEYDOWN_GREEN:
		test_key_y = 0;
		test_key_x = 1;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_GREEN);
		break;
		
	case MSG_KEYDOWN_RED:
		test_key_y = 1;
		test_key_x = 1;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_RED);
		break;
	case MSG_KEYDOWN_OK:
		test_key_y = 1;
		test_key_x = 0;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_SL);
		break;
		
	case MSG_KEYDOWN_CANCEL:
		test_key_y = 0;
		test_key_x = 0;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_SR);
		break;
		
	case MSG_KEYDOWN_WEB:
		test_key_y = 5;
		test_key_x = 0;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_WEB);
		break;
#if 0
	case MSG_KEYDOWN_UPSIDE:
		test_key_y = 5;
		test_key_x = 1;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_VOL_UP);
		break;
	case MSG_KEYDOWN_DOWNSIDE:
		test_key_y = 5;
		test_key_x = 2;
		s_unisoctest_key_flag |= (0x00000001 << UNISOC_TEST_KEY_VOL_DOWN);
		break;
#endif
	default:
		break;
    }
    key_on_map_idx = test_key_y * 4 + test_key_x;
    key_on_map_idx %= UI_TEST_KEY_NUM;
    byte_idx = key_on_map_idx / 8;
    bit_idx = key_on_map_idx % 8;
    key_buf_byte = s_unisoctest_key_ind_buf[byte_idx];
    s_unisoctest_key_ind_buf[byte_idx] &= (unsigned char)(~(1<<bit_idx));/*lint !e502*/  
    // show key passed
    if ((key_buf_byte^s_unisoctest_key_ind_buf[byte_idx]) != 0)
    {        
        {        
            GUI_RECT_T rect = MMITHEME_GetClientRect();
            int32 TESTPAD_KEY_WIDTH = (rect.right- rect.left - MMIENG_TESTPAD_MARGIN_X*2)>>2 ;
            
            draw_rect.left = MMIENG_TESTPAD_MARGIN_X + test_key_x * TESTPAD_KEY_WIDTH;
            draw_rect.top = MMIENG_TESTPAD_MARGIN_Y + test_key_y * MMIENG_TESTPAD_KEY_HIGHT;
            draw_rect.right = draw_rect.left + TESTPAD_KEY_WIDTH - 1;
            draw_rect.bottom = draw_rect.top + MMIENG_TESTPAD_KEY_HIGHT-1;
            LCD_FillRect(&lcd_dev_info, draw_rect, UITEST_COLOR_BLACK);
            
            update_rect.right = rect.right;
            update_rect.bottom = rect.bottom;
            GUILCD_InvalidateRect(GUI_MAIN_LCD_ID,update_rect, GUIREF_GetUpdateBlockSet(GUI_BLOCK_MAIN));
        }
    }
    return (0);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestKeyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
		s_unisoctest_key_flag = 0;
		break;
		
	case MSG_FULL_PAINT:
		s_unisoctest_key_ind_buf[0] = 0xFF;
		s_unisoctest_key_ind_buf[1] = 0xFF;
		s_unisoctest_key_ind_buf[2] = 0xFF;
		s_unisoctest_key_ind_buf[3] = 0x01;
		s_unisoctest_key_ind_buf[4] = 0x00;
		UnisocTestDrawTestKeyBG();
		break;
		
	case MSG_KEYDOWN_GREEN:
	case MSG_KEYDOWN_CALL2:    
	case MSG_KEYDOWN_CALL3:
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
	case MSG_KEYDOWN_STAR:
	case MSG_KEYDOWN_HASH:
	case MSG_KEYDOWN_DOWN:
	case MSG_KEYDOWN_LEFT:
	case MSG_KEYDOWN_RIGHT:
	case MSG_KEYDOWN_RED:
	case MSG_KEYDOWN_UP:                
	case MSG_KEYDOWN_UPSIDE:
	case MSG_KEYDOWN_DOWNSIDE:
	case MSG_KEYDOWN_OK:
	case MSG_KEYDOWN_CANCEL:    
		UnisocTestKeyboard(msg_id);
		if(UNISOC_TEST_KEY_COMPLETE_FLAG == s_unisoctest_key_flag)
		{
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id) ;
		}
		break;
	case MSG_KEYDOWN_WEB:
		UnisocTestKeyboard(msg_id);
		if(UNISOC_TEST_KEY_COMPLETE_FLAG == s_unisoctest_key_flag)
		{
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id) ;
		}
		else{
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_NULL, TRUE);
		}			
		break;
		
		
	case MSG_CLOSE_WINDOW:
		break;
		
	default:
		break;
	}
	return (result);
}

#if 1//def	ZDT_PRODUCT_JGW
BOOLEAN lcd_is_retest = FALSE ;
#endif
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestLcdWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	GUI_RECT_T draw_rect = MMITHEME_GetFullScreenRect();
	GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
	GUI_COLOR_T draw_color[UNISOC_TEST_LCD_DISPLAY_COLOR_NUM] 
		= {UITEST_COLOR_WHITE,UITEST_COLOR_BLUE,UITEST_COLOR_GREEN,UITEST_COLOR_RED,UITEST_COLOR_BLACK};
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	static uint8 index = 0;
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		index = 0;
		LCD_FillRect(&lcd_dev_info, full_rect, draw_color[index]);
		s_unisoctest_lcd_timer_id = MMK_CreateWinTimer(win_id, UNISOC_TEST_LCD_DISPLAY_DELAY,TRUE);
		break;
		
	case MSG_FULL_PAINT:
		break;
	case MSG_TIMER:
		if (*(uint8*)param == s_unisoctest_lcd_timer_id) 
		{
			index += 1;
			if(index < UNISOC_TEST_LCD_DISPLAY_COLOR_NUM)
			{
				LCD_FillRect(&lcd_dev_info, full_rect, draw_color[index]);
			}
			else if(index >= UNISOC_TEST_LCD_DISPLAY_COLOR_NUM)
			{
				GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
				MMI_CONTROL_CREATE_T softkey_create = {0};
				
				if (s_unisoctest_lcd_timer_id != 0)
				{
					MMK_StopTimer(s_unisoctest_lcd_timer_id);
					s_unisoctest_lcd_timer_id = 0;
				} 
#if 1//def  ZDT_PRODUCT_JGW
			s_unisoctest_lcd_timer_id = MMK_CreateWinTimer(win_id, 3000,TRUE);
			index = 0;
			lcd_is_retest =TRUE ;
#endif

				LCD_FillRect(&lcd_dev_info, full_rect, UITEST_COLOR_WHITE);
				GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, s_unisoctest_prompt_str.wstr_ptr,s_unisoctest_prompt_str.wstr_len, TRUE);

				softkey_data.leftsoft_id = TXT_ENG_PASS;
				softkey_data.midsoft_id = TXT_ENG_RETEST;
				softkey_data.rightsoft_id = TXT_ENG_FAIL;
				
				softkey_create.ctrl_id           = MMITHEME_GetSoftkeyCtrlId();
				softkey_create.guid              = SPRD_GUI_SOFTKEY_ID;
				softkey_create.init_data_ptr     = &softkey_data;
				softkey_create.parent_win_handle = win_id;
				MMK_CreateControl(&softkey_create);
				MMK_SetWinSoftkeyCtrlId(win_id,softkey_create.ctrl_id);
				GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_ENG_RETEST, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			}
		}
		else
		{
			result = MMI_RESULT_FALSE;      // need additional process
		}
		break;
	case MSG_GET_FOCUS:
		break;
	case MSG_KEYDOWN_UP:
	case MSG_KEYDOWN_DOWN:
	case MSG_KEYDOWN_LEFT:
	case MSG_KEYDOWN_RIGHT:
	       break;
	case MSG_KEYDOWN_OK:
		#if 1//def  ZDT_PRODUCT_JGW	
		if((index >= UNISOC_TEST_LCD_DISPLAY_COLOR_NUM) ||(lcd_is_retest == TRUE))
			#else	
		if(index >= UNISOC_TEST_LCD_DISPLAY_COLOR_NUM)
		#endif	
		{
#if 1//def  ZDT_PRODUCT_JGW	
			if((lcd_is_retest == TRUE))
			{
				if (s_unisoctest_lcd_timer_id != 0)
				{
				MMK_StopTimer(s_unisoctest_lcd_timer_id);
				s_unisoctest_lcd_timer_id = 0;
				} 
				lcd_is_retest =FALSE ;
			}
#endif

			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin( win_id );  
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:
	#if 1//def	ZDT_PRODUCT_JGW	
		if((index >= UNISOC_TEST_LCD_DISPLAY_COLOR_NUM) ||(lcd_is_retest == TRUE))
	#else		
		if(index >= UNISOC_TEST_LCD_DISPLAY_COLOR_NUM)
	#endif		
		{
#if 1//def	ZDT_PRODUCT_JGW	
			if((lcd_is_retest == TRUE))
			{
				if (s_unisoctest_lcd_timer_id != 0)
				{
					MMK_StopTimer(s_unisoctest_lcd_timer_id);
					s_unisoctest_lcd_timer_id = 0;
				} 
				lcd_is_retest =FALSE ;
			}
#endif
		
			SaveUnisocTestResult(win_id, 0);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin( win_id ) ; 
		}
		break;
	case MSG_KEYDOWN_WEB:
		if(index >= UNISOC_TEST_LCD_DISPLAY_COLOR_NUM)
		{
			index = 0;
			LCD_FillRect(&lcd_dev_info, full_rect, draw_color[index]);
			s_unisoctest_lcd_timer_id = MMK_CreateWinTimer(win_id, UNISOC_TEST_LCD_DISPLAY_DELAY,TRUE);
		}
		break;	
	case MSG_KEYDOWN_RED:
		break;
	case MSG_CLOSE_WINDOW:
		if (s_unisoctest_lcd_timer_id != 0)
		{
			MMK_StopTimer(s_unisoctest_lcd_timer_id);
			s_unisoctest_lcd_timer_id = 0;
		} 
#if 1//def	ZDT_PRODUCT_JGW	
		lcd_is_retest =FALSE ;
#endif

		break;
		
	default:            
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestVibrateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T text = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:             
		
		MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_OTHER);
		s_unisoctest_vibrate_timer_id = MMK_CreateWinTimer(win_id, UNISOC_TEST_VIBRATE_DELAY,FALSE);
		break; 
		
	case MSG_FULL_PAINT:
		{
			GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
			GUI_RECT_T          client_rect = MMITHEME_GetClientRectEx(win_id);
			GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  IMAGE_COMMON_BG, &lcd_dev_info);
			text = s_unisoctest_prompt_str;
			GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		}
		break;
	case MSG_TIMER:
		if (*(uint8*)param == s_unisoctest_vibrate_timer_id) 
		{
			MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_OTHER);
			if (s_unisoctest_vibrate_timer_id != 0)
			{
				MMK_StopTimer(s_unisoctest_vibrate_timer_id);
				s_unisoctest_vibrate_timer_id = 0;
			} 
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_ENG_RETEST, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		}
		else
		{
			result = MMI_RESULT_FALSE;      // need additional process
		}
		break;
	case MSG_KEYDOWN_OK:            
	case MSG_CTL_OK:
		MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_OTHER);
		SaveUnisocTestResult(win_id, 1);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);
		break;
		
	case MSG_LOSE_FOCUS:
		MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_OTHER);
		if (s_unisoctest_vibrate_timer_id != 0)
		{
			MMK_StopTimer(s_unisoctest_vibrate_timer_id);
			s_unisoctest_vibrate_timer_id = 0;
		} 
		GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_ENG_RETEST, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		break;
		
	case MSG_GET_FOCUS:
		//MMIAPISET_SetVibrator(TRUE, MMISET_RING_TYPE_OTHER);
		break;
		
	case MSG_APP_CANCEL :
	case MSG_CTL_CANCEL:
		MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_OTHER);
		SaveUnisocTestResult(win_id, 0);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);
		break;
		
	case MSG_KEYDOWN_RED:
		break;
	case MSG_KEYDOWN_WEB:
		if(0 == s_unisoctest_vibrate_timer_id)
		{
			MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_OTHER);
			s_unisoctest_vibrate_timer_id = MMK_CreateWinTimer(win_id, UNISOC_TEST_VIBRATE_DELAY,FALSE);
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		}
		break;	
		
	case MSG_CLOSE_WINDOW:
		MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_OTHER);
		if (s_unisoctest_vibrate_timer_id != 0)
		{
			MMK_StopTimer(s_unisoctest_vibrate_timer_id);
			s_unisoctest_vibrate_timer_id = 0;
		} 
		
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}
LOCAL MMI_RESULT_E UnisocTestRTCWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T timer_text = {0};
    uint8 timer_buf[5] = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T		lab_rect = {0};
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        lab_rect = client_rect;
        lab_rect.bottom =   (client_rect.bottom + client_rect.top)/2;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);

        // lab_rect.bottom = client_rect.top + (client_rect.bottom - client_rect.top)*4/5;
        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);

        s_mmieng_rtc_counter = 0;
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }
        s_mmieng_testrtc_timer_id = MMK_CreateWinTimer(MMIENG_UNISOCTEST_RTC_WIN_ID, 1000, TRUE);
        timer_text = s_mmieng_str_up_to_next;

        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &timer_text, FALSE);
        sprintf((char*)timer_buf, "%04d", s_mmieng_rtc_counter);
        UnisocTestSetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, (char*)timer_buf, 4, FALSE);
        break;
#ifndef MMI_RES_LOW_COST
    case MSG_LCD_SWITCH:
        lab_rect = client_rect;
        lab_rect.bottom =   (client_rect.bottom + client_rect.top)/2;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);

        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        break;
#endif
    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  IMAGE_COMMON_BG, &lcd_dev_info);
 	    GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
        break;
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
    case MSG_CTL_OK:
    case MSG_KEYDOWN_OK:
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }
        SaveUnisocTestResult(win_id, 1);
        MMIAPIENG_RunUnisocTestNextStep();
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }
        SaveUnisocTestResult(win_id, 0);
        MMIAPIENG_RunUnisocTestNextStep();
        MMK_CloseWin(win_id);

        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_KEYDOWN_FLIP:
#endif
    case MSG_KEYDOWN_RED:
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif
        }
        break;
#endif

    case MSG_TIMER:
        if (*(uint8*)param == s_mmieng_testrtc_timer_id)
        {
            s_mmieng_rtc_counter++;
            if (s_mmieng_rtc_counter > 9999)
                s_mmieng_rtc_counter = 0;
            sprintf((char*)timer_buf, "%04d", s_mmieng_rtc_counter);
            UnisocTestSetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, (char*)timer_buf, 4, TRUE);
        }
        else
            result = MMI_RESULT_FALSE;
        break;


    case MSG_CLOSE_WINDOW:
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }
	 if(0 != s_is_fulltest_unisoc)
	  {
	          MMIAPIENG_RunUnisocTestRESET();
	       // MMIAPISET_OpenAboutResetPwdWin(TRUE);
		/*MMISET_CleanUserData();
		MMISET_ResetFactorySetting();
		MMIAPIPHONE_PowerReset();*/
	  }		
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
LOCAL MMI_RESULT_E UnisocTestTORCHWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T text = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	wchar wstr[80] = {0};

	//char* str = "up --- open torch led.\ndown --- close torch led.";
	 MMI_GetLabelTextByLang(TXT_ENG_TORCHNOTE, &text );

	//uint16 len = strlen( str );

	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		//text.wstr_ptr = wstr;
		//text.wstr_len = len;
		//MMI_STRNTOWSTR( wstr, 80, (uint8*)str, len, len );
		GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
		MMK_SetAtvCtrl(MMIENG_UNISOCTEST_TORCH_WIN_ID, MMIENG_UITEST_TEXT_CTRL_ID);
		GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
#ifndef _WIN32
		GPIO_SetTorch(TRUE);
#endif
             break;

	case MSG_CTL_MIDSK:
	case MSG_CTL_OK:
	case MSG_KEYDOWN_OK:
#ifndef _WIN32
		GPIO_SetTorch(FALSE);
#endif
             SaveUnisocTestResult(win_id, 1);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);
		break;
	case MSG_APP_WEB:
	case MSG_APP_MENU:

		break;

	case MSG_CTL_CANCEL:
	case MSG_KEYDOWN_CANCEL:
#ifndef _WIN32
		GPIO_SetTorch(FALSE);
#endif
		SaveUnisocTestResult(win_id, 0);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);
		break;
	case MSG_KEYDOWN_UP:
#ifndef _WIN32
		GPIO_SetTorch(TRUE);
#endif
		break;
	case MSG_KEYDOWN_DOWN:
#ifndef _WIN32
		GPIO_SetTorch(FALSE);
#endif
		break;
	case MSG_CLOSE_WINDOW:
#ifndef _WIN32
		GPIO_SetTorch(FALSE);
#endif
		break;

	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return result;
}
#if defined(ZDT_SHOW_SIM_NETWORK_RSSI_IN_FACTORY_MODE)   
//#include "mmiphone_export.h"
extern PHONE_SERVICE_STATUS_T  g_service_status[MMI_DUAL_SYS_MAX];
#define eng_abs(x)  ((x) > 0 ? (x) : (-(x)))

static char s_curr_plmn_buff[20];
static uint8 sim_network_update_timer_id = 0;
 
LOCAL void ZDT_Factory_Sim_Network_Update_Timer_Start()
{
    if(sim_network_update_timer_id != 0)
    {
        MMK_StopTimer(sim_network_update_timer_id);
        sim_network_update_timer_id=0;	
    }
    sim_network_update_timer_id =  MMK_CreateWinTimer(MMIENG_UNISOCTEST_SIM_NETWORK_WIN_ID, 1000,TRUE);

}

LOCAL void ZDT_Factory_Sim_Network_Update_Timer_Stop()
{
    if(sim_network_update_timer_id != 0)
    {
        MMK_StopTimer(sim_network_update_timer_id);
	 sim_network_update_timer_id=0; 	
    }
}


LOCAL MMI_RESULT_E UnisocTestSimNetworkWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
		MMI_MENU_GROUP_ID_T 		group_id		=	0;
		MMI_MENU_ID_T				menu_id 		=	0;
		MMI_RESULT_E				recode			=	MMI_RESULT_TRUE;
		GUI_RECT_T		bg_rect = {0, 0, 239, 319};
		GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
		MMI_STRING_T		str_data = {0};
		GUI_RECT_T	text_display_rect={0};
		GUISTR_STYLE_T		text_style = {0};
		GUISTR_STATE_T		state = GUISTR_STATE_ALIGN|GUISTR_STATE_ELLIPSIS;
		GUISTR_INFO_T		text_info = {0};
		uint8 temp_str[32] = {0};
		wchar temp_wstr[32] = {0};		
		uint16	temp_len=0;
		
		MMI_STRING_T show_content_string = {0};
		MMI_STRING_T temp_string = {0};

		wchar show_content[200] = {0};

		BOOLEAN sim1_card_ok = FALSE;
		BOOLEAN sim2_card_ok = FALSE;
		uint16 uint16_str_len = 0;
		
		MN_DUAL_SYS_E dual_sys_use=MN_DUAL_SYS_1;	
		MN_PLMN_T plmn_info;
		MMI_GMMREG_RAT_E	network_status_rat = MMI_GMMREG_RAT_GPRS;

		MMI_STRING_T    cr_s = {0};
		wchar           unicode_cr = 0x0d;

		cr_s.wstr_ptr = &unicode_cr;
		cr_s.wstr_len = 1;		

		if(MMIAPIPHONE_GetSimExistedStatus(0))
		{
		    sim1_card_ok = TRUE;
		}
		
		if(MMIAPIPHONE_GetSimExistedStatus(1))
		{				
		    sim2_card_ok = TRUE;
		}
		
		switch(msg_id) 
		{
		    case MSG_OPEN_WINDOW:
			ZDT_Factory_Sim_Network_Update_Timer_Start();
			break;
				
		    case MSG_FULL_PAINT:				
			GUI_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
				
			text_style.align = ALIGN_LEFT; 
			text_style.font_color = MMI_BLACK_COLOR;
			text_style.effect = FONT_EFFECT_REVERSE_EDGE;
			text_style.font = SONG_FONT_24;


		    if((sim1_card_ok==TRUE) && (sim2_card_ok==FALSE))
		    {
       		dual_sys_use=MN_DUAL_SYS_1;		    
			
			//line  1		SIM N INFO:
			//SCI_MEMSET(show_content, 0, 200*2);
			sprintf((char*) temp_str, "SIM1 INFO:\n");
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));
                    MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
                    //line 2    OPN
       		MMIAPIPHONE_GetSimHPLMN(&plmn_info, dual_sys_use);
       		memset(s_curr_plmn_buff,0,sizeof(s_curr_plmn_buff));
       		sprintf(s_curr_plmn_buff, "%.3d%.2d", plmn_info.mcc, plmn_info.mnc);	
       		s_curr_plmn_buff[5]='\0';		
			memset(temp_str,0,sizeof(temp_str));
       		if((strcmp(s_curr_plmn_buff, "46000")==0) || (strcmp(s_curr_plmn_buff, "46002")==0) || (strcmp(s_curr_plmn_buff, "46007")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CMCC");
       		}
       		else if((strcmp(s_curr_plmn_buff, "46001")==0) || (strcmp(s_curr_plmn_buff, "46006")==0) || (strcmp(s_curr_plmn_buff, "46009")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CUCC");
       		}	
       		else if((strcmp(s_curr_plmn_buff, "46003")==0) || (strcmp(s_curr_plmn_buff, "46005")==0) || (strcmp(s_curr_plmn_buff, "46011")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CTCC");
       		}
       		else
       		{
       		    sprintf((char*) temp_str, "OPN: %s", "CBNET");	//UNKNOW	//中国广电
       		}
       		MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));	
			MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行

             // line 3   RSSI:
                    memset(temp_str,0,sizeof(temp_str));
			sprintf((char*) temp_str, "RSSI:  -%d",eng_abs(g_service_status[dual_sys_use].rssiLevel -140));
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));	
			MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
			
             // line 4     NET TPYE
                    memset(temp_str,0,sizeof(temp_str));
                    network_status_rat = MMIAPIPHONE_GetTDOrGsm(dual_sys_use);
                    switch (network_status_rat) 
                    {
                           case MMI_GMMREG_RAT_GPRS:
       			    sprintf((char*) temp_str, "Network Type:  %s","RAT_GPRS");
                           break;
             
                           case MN_GMMREG_RAT_4G:
                               sprintf((char*) temp_str, "Network Type:  %s","RAT_4G");
                           break;
                           case MMI_GMMREG_RAT_UNKNOWN:
                           default:
                               sprintf((char*) temp_str, "Network Type:  %s","RAT_U");
                           break;
                    }
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));			
		       MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
		       
			show_content_string.wstr_ptr = show_content;
			show_content_string.wstr_len = MMIAPICOM_Wstrlen(show_content);
			
			text_display_rect.left = 0;
			text_display_rect.top = 42;
			text_display_rect.right=MMI_MAINSCREEN_WIDTH;
			text_display_rect.bottom=MMI_MAINSCREEN_HEIGHT - 40; ; 

			GUISTR_DrawTextToLCDInRect( 
					MMITHEME_GetDefaultLcdDev(),
					(const GUI_RECT_T	   *)&text_display_rect,	   //the fixed display area
					(const GUI_RECT_T	   *)&text_display_rect,	   //用户要剪切的实际区域
					(const MMI_STRING_T    *)&show_content_string,
					&text_style,
					state,
					GUISTR_TEXT_DIR_AUTO
					);
  			
		}
		else if((sim1_card_ok==FALSE) && (sim2_card_ok==TRUE))
		{
       		dual_sys_use=MN_DUAL_SYS_2;		    
			
			//line  1		SIM N INFO:
			//SCI_MEMSET(show_content, 0, 100*2);
			sprintf((char*) temp_str, "SIM2 INFO:\n");
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));
                    MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
                    //line 2    OPN
       		MMIAPIPHONE_GetSimHPLMN(&plmn_info, dual_sys_use);
       		memset(s_curr_plmn_buff,0,sizeof(s_curr_plmn_buff));
       		sprintf(s_curr_plmn_buff, "%.3d%.2d", plmn_info.mcc, plmn_info.mnc);	
       		s_curr_plmn_buff[5]='\0';		
			memset(temp_str,0,sizeof(temp_str));
       		if((strcmp(s_curr_plmn_buff, "46000")==0) || (strcmp(s_curr_plmn_buff, "46002")==0) || (strcmp(s_curr_plmn_buff, "46007")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CMCC");
       		}
       		else if((strcmp(s_curr_plmn_buff, "46001")==0) || (strcmp(s_curr_plmn_buff, "46006")==0) || (strcmp(s_curr_plmn_buff, "46009")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CUCC");
       		}	
       		else if((strcmp(s_curr_plmn_buff, "46003")==0) || (strcmp(s_curr_plmn_buff, "46005")==0) || (strcmp(s_curr_plmn_buff, "46011")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CTCC");
       		}
       		else
       		{
       		    sprintf((char*) temp_str, "OPN: %s", "CBNET");	//UNKNOW	//中国广电
       		}
       		MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));	
			MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行

             // line 3   RSSI:
                    memset(temp_str,0,sizeof(temp_str));
			sprintf((char*) temp_str, "RSSI:  -%d",eng_abs(g_service_status[dual_sys_use].rssiLevel -140));
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));	
			MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
			
             // line 4     NET TPYE
                    memset(temp_str,0,sizeof(temp_str));
                    network_status_rat = MMIAPIPHONE_GetTDOrGsm(dual_sys_use);
                    switch (network_status_rat) 
                    {
                           case MMI_GMMREG_RAT_GPRS:
       			    sprintf((char*) temp_str, "Network Type:  %s","RAT_GPRS");
                           break;
                      
                           case MN_GMMREG_RAT_4G:
                               sprintf((char*) temp_str, "Network Type:  %s","RAT_4G");
                           break;
                           case MMI_GMMREG_RAT_UNKNOWN:
                           default:
                               sprintf((char*) temp_str, "Network Type:  %s","RAT_U");
                           break;
                    }
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));			
		       MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
		       
			show_content_string.wstr_ptr = show_content;
			show_content_string.wstr_len = MMIAPICOM_Wstrlen(show_content);
			
			text_display_rect.left = 0;
			text_display_rect.top = 42;
			text_display_rect.right=MMI_MAINSCREEN_WIDTH;
			text_display_rect.bottom=MMI_MAINSCREEN_HEIGHT - 40; ; 

			GUISTR_DrawTextToLCDInRect( 
					MMITHEME_GetDefaultLcdDev(),
					(const GUI_RECT_T	   *)&text_display_rect,	   //the fixed display area
					(const GUI_RECT_T	   *)&text_display_rect,	   //用户要剪切的实际区域
					(const MMI_STRING_T    *)&show_content_string,
					&text_style,
					state,
					GUISTR_TEXT_DIR_AUTO
					);
  			
		}
		else if((sim1_card_ok==TRUE) && (sim2_card_ok==TRUE))
		{
       		dual_sys_use=MN_DUAL_SYS_1;		    
			
			//line  1		SIM N INFO:
			//SCI_MEMSET(show_content, 0, 100*2);
			sprintf((char*) temp_str, "SIM1 INFO:");
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));
                    MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
                    //line 2    OPN
       		MMIAPIPHONE_GetSimHPLMN(&plmn_info, dual_sys_use);
       		memset(s_curr_plmn_buff,0,sizeof(s_curr_plmn_buff));
       		sprintf(s_curr_plmn_buff, "%.3d%.2d", plmn_info.mcc, plmn_info.mnc);	
       		s_curr_plmn_buff[5]='\0';		
			memset(temp_str,0,sizeof(temp_str));
       		if((strcmp(s_curr_plmn_buff, "46000")==0) || (strcmp(s_curr_plmn_buff, "46002")==0) || (strcmp(s_curr_plmn_buff, "46007")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CMCC");
       		}
       		else if((strcmp(s_curr_plmn_buff, "46001")==0) || (strcmp(s_curr_plmn_buff, "46006")==0) || (strcmp(s_curr_plmn_buff, "46009")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CUCC");
       		}	
       		else if((strcmp(s_curr_plmn_buff, "46003")==0) || (strcmp(s_curr_plmn_buff, "46005")==0) || (strcmp(s_curr_plmn_buff, "46011")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CTCC");
       		}
       		else
       		{
       		    sprintf((char*) temp_str, "OPN: %s", "CBNET");//	UNKNOW //中国广电	
       		}
       		MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));	
			MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行

             // line 3   RSSI:
                    memset(temp_str,0,sizeof(temp_str));
			sprintf((char*) temp_str, "RSSI:  -%d",eng_abs(g_service_status[dual_sys_use].rssiLevel -140));
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));	
			MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
			
             // line 4     NET TPYE
                    memset(temp_str,0,sizeof(temp_str));
                    network_status_rat = MMIAPIPHONE_GetTDOrGsm(dual_sys_use);
                    switch (network_status_rat) 
                    {
                           case MMI_GMMREG_RAT_GPRS:
       			    sprintf((char*) temp_str, "Network Type:  %s","RAT_GPRS");
                           break;
                     
                           case MMI_GMMREG_RAT_4G:
                               sprintf((char*) temp_str, "Network Type:  %s","4G");
                           break;
                           case MMI_GMMREG_RAT_UNKNOWN:
                           default:
                               sprintf((char*) temp_str, "Network Type:  %s","RAT_U");
                           break;
                    }
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));			
		       MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
		       MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行

       		dual_sys_use=MN_DUAL_SYS_2;		    
			
			//line  5		SIM N INFO:
			//SCI_MEMSET(show_content, 0, 100*2);
			sprintf((char*) temp_str, "SIM2 INFO:");
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));
                    MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
                    //line 6    OPN
       		MMIAPIPHONE_GetSimHPLMN(&plmn_info, dual_sys_use);
       		memset(s_curr_plmn_buff,0,sizeof(s_curr_plmn_buff));
       		sprintf(s_curr_plmn_buff, "%.3d%.2d", plmn_info.mcc, plmn_info.mnc);	
       		s_curr_plmn_buff[5]='\0';		
			memset(temp_str,0,sizeof(temp_str));
       		if((strcmp(s_curr_plmn_buff, "46000")==0) || (strcmp(s_curr_plmn_buff, "46002")==0) || (strcmp(s_curr_plmn_buff, "46007")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CMCC");
       		}
       		else if((strcmp(s_curr_plmn_buff, "46001")==0) || (strcmp(s_curr_plmn_buff, "46006")==0) || (strcmp(s_curr_plmn_buff, "46009")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CUCC");
       		}	
       		else if((strcmp(s_curr_plmn_buff, "46003")==0) || (strcmp(s_curr_plmn_buff, "46005")==0) || (strcmp(s_curr_plmn_buff, "46011")==0))
       		{
       		    sprintf((char*) temp_str, "OPN:  %s", "CTCC");
       		}
       		else
       		{
       		    sprintf((char*) temp_str, "OPN: %s", "CBNET");	//	UNKNOW //中国广电
       		}
       		MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));	
			MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行

             // line 7   RSSI:
                    memset(temp_str,0,sizeof(temp_str));
			sprintf((char*) temp_str, "RSSI:  -%d",eng_abs(g_service_status[dual_sys_use].rssiLevel -140));
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));	
			MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
			
             // line 8     NET TPYE
                    memset(temp_str,0,sizeof(temp_str));
                    network_status_rat = MMIAPIPHONE_GetTDOrGsm(dual_sys_use);
                    switch (network_status_rat) 
                    {
                           case MMI_GMMREG_RAT_GPRS:
       			    sprintf((char*) temp_str, "Network Type:  %s","RAT_GPRS");
                           break;
                              case MMI_GMMREG_RAT_4G:
                               sprintf((char*) temp_str, "Network Type:  %s","4G");
                           break;
                           case MMI_GMMREG_RAT_UNKNOWN:
                           default:
                               sprintf((char*) temp_str, "Network Type:  %s","RAT_U");
                           break;
                    }
			MMIAPICOM_StrcatFromStrToUCS2(show_content, &uint16_str_len, temp_str, strlen(temp_str));			
		       MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &cr_s);//换行
		       
			show_content_string.wstr_ptr = show_content;
			show_content_string.wstr_len = MMIAPICOM_Wstrlen(show_content);
			
			text_display_rect.left = 0;
			text_display_rect.top = 42;
			text_display_rect.right=MMI_MAINSCREEN_WIDTH;
			text_display_rect.bottom=MMI_MAINSCREEN_HEIGHT - 40; ; 

			GUISTR_DrawTextToLCDInRect( 
					MMITHEME_GetDefaultLcdDev(),
					(const GUI_RECT_T	   *)&text_display_rect,	   //the fixed display area
					(const GUI_RECT_T	   *)&text_display_rect,	   //用户要剪切的实际区域
					(const MMI_STRING_T    *)&show_content_string,
					&text_style,
					state,
					GUISTR_TEXT_DIR_AUTO
					);			
		}
		else
		{
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ENG_SIM_NETWORK_INSERT_SIM, &str_data);  	
	             MMIAPICOM_StrcatFromSTRINGToUCS2(show_content, &uint16_str_len, &str_data);	
			show_content_string.wstr_ptr = show_content;
			show_content_string.wstr_len = MMIAPICOM_Wstrlen(show_content);					
			text_display_rect.left = 0;
			text_display_rect.top = 42;
			text_display_rect.right=MMI_MAINSCREEN_WIDTH;
			text_display_rect.bottom=MMI_MAINSCREEN_HEIGHT - 40; ; 
			//GUISTR_GetStringInfo(&text_style, &str_data, state, &text_info);

			GUISTR_DrawTextToLCDInRect( 
					MMITHEME_GetDefaultLcdDev(),
					(const GUI_RECT_T	   *)&text_display_rect,	   //the fixed display area
					(const GUI_RECT_T	   *)&text_display_rect,	   //用户要剪切的实际区域
					(const MMI_STRING_T    *)&show_content_string,
					&text_style,
					state,
					GUISTR_TEXT_DIR_AUTO
					);					
		}
			break;
				
			case MSG_APP_OK:
			case MSG_APP_WEB:
			case MSG_CTL_MIDSK:
				MMK_CloseWin(win_id);
				break;
				
			case MSG_TIMER:
			{
				if (*(uint8*)param == sim_network_update_timer_id)
				{
					MMK_SendMsg(MMK_ConvertIdToHandle(win_id), MSG_FULL_PAINT, PNULL);
				}
			}			
			break;
			
			case MSG_LOSE_FOCUS:
			{ 
				ZDT_Factory_Sim_Network_Update_Timer_Stop();
			}
			
			break;
			
			case MSG_KEYDOWN_RED:
				break;
		
			case MSG_CTL_CANCEL:
			case MSG_APP_CANCEL:
				ZDT_Factory_Sim_Network_Update_Timer_Stop();
				MMK_CloseWin(win_id);
				break;
	
			default:
				recode = MMI_RESULT_FALSE;
				break;
		}
		
		return recode;
	
}

#endif

#ifdef Z695_KEYLOCK_INT_SUPPORT
LOCAL MMI_RESULT_E UnisocTestKeylockWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T text = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	wchar wstr[80] = {0};
	 MMI_GetLabelTextByLang(TXT_ENG_KEYLOCK_TEST, &text );
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
		MMK_SetAtvCtrl(MMIENG_UNISOCTEST_KEYLOCK_WIN_ID, MMIENG_UITEST_TEXT_CTRL_ID);
		GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
#if 0
ndef _WIN32
		GPIO_SetTorch(TRUE);
#endif
             break;
	case MSG_CTL_MIDSK:
	case MSG_CTL_OK:
	case MSG_KEYDOWN_OK:
#if 0
ndef _WIN32
		GPIO_SetTorch(FALSE);
#endif
             SaveUnisocTestResult(win_id, 1);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);
		break;
	case MSG_APP_WEB:
	case MSG_APP_MENU:
		break;
	case MSG_KEYDOWN_SLIDE:
		MMIDEFAULT_HandleKeyLock(TRUE);
		break;
	case MSG_KEYUP_SLIDE:
		MMIDEFAULT_HandleKeyLock(FALSE);
		break; 
	case MSG_CTL_CANCEL:
	case MSG_KEYDOWN_CANCEL:
#ifndef _WIN32
#endif
		SaveUnisocTestResult(win_id, 0);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
#ifndef _WIN32
#endif
		break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return result;
}
#endif

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestSpeakerWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T text = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	LOCAL   uint8  ring_id = 1;
	BOOLEAN headset_is_plug = FALSE;
	//wchar headset_is_in_wstr[] = { 'P', 'l', 'e', 'a', 's', 'e', ' ', 'p', 'l', 'u', 'g', ' ', 'o', 'u', 't', ' ', 'h', 'e', 'a', 'd', 's', 'e', 't', 0 };
	MMI_GetLabelTextByLang(TXT_ENG_SPKPLUGOUTHEADSET, &text );

	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
		/*s_unisoctest_speaker_volume = MMIAPISET_GetCallRingVolume();
		if (0 == s_unisoctest_speaker_volume)
		{
			s_unisoctest_speaker_volume = 3;
		}*/
		s_unisoctest_speaker_volume = 9;
		break;
		
	case MSG_FULL_PAINT:  
		{
			GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
			GUI_RECT_T          client_rect = MMITHEME_GetClientRectEx(win_id);
			GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  IMAGE_COMMON_BG, &lcd_dev_info);
#ifndef _WIN32
			headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
			if (headset_is_plug)
			{
				MMIAPISET_StopAppRing();
				//text.wstr_ptr = headset_is_in_wstr;
				//text.wstr_len = (uint16)MMIAPICOM_Wstrlen( text.wstr_ptr);
				GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
				GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, TRUE);
			}
			else
			{
				MMIAPISET_PlayCallRingByVol(s_unisoctest_speaker_volume, ring_id,AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
				text = s_unisoctest_prompt_str;
				GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
				GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_ENG_RETEST, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			}
		}
		break;
		
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			MMIAPISET_StopAppRing();
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL:  
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			MMIAPISET_StopAppRing();
			SaveUnisocTestResult(win_id, 0);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_UPSIDE:
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			if (s_unisoctest_speaker_volume < MMISET_VOL_MAX)
			{
				s_unisoctest_speaker_volume++;
				MMIAPISET_PlayCallRingByVol(s_unisoctest_speaker_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
			}
		}
		break;
		
	case MSG_KEYDOWN_DOWNSIDE:
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			if (s_unisoctest_speaker_volume > MMISET_VOL_ONE)
			{
				s_unisoctest_speaker_volume--;
				MMIAPISET_PlayCallRingByVol(s_unisoctest_speaker_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
			}
		}
		break;
		
	case MSG_KEYDOWN_RED:
		break;
		
	case MSG_LOSE_FOCUS:
		// stop melody
		MMIAPISET_StopAppRing();
		break;
		
	case MSG_GET_FOCUS:
		// start melody playing
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			MMIAPISET_PlayCallRingByVol(s_unisoctest_speaker_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
		}
		break;
	case MSG_KEYDOWN_WEB:
		MMIAPISET_StopAppRing();
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			MMIAPISET_PlayCallRingByVol(s_unisoctest_speaker_volume, ring_id,AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
		}
		break;
	case MSG_KEYDOWN_HEADSET_DETECT:
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		result = MMI_RESULT_FALSE;
		break;
	case MSG_KEYUP_HEADSET_DETECT:
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		result = MMI_RESULT_FALSE;
		break;
	case MSG_CLOSE_WINDOW:
		// stop melody
		MMIAPISET_StopAppRing();
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestReceiverWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T text = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	LOCAL   uint8  ring_id = 1;
	BOOLEAN headset_is_plug = FALSE;
	//wchar headset_is_in_wstr[] = { 'P', 'l', 'e', 'a', 's', 'e', ' ', 'p', 'l', 'u', 'g', ' ', 'o', 'u', 't', ' ', 'h', 'e', 'a', 'd', 's', 'e', 't', 0 };
	MMI_GetLabelTextByLang(TXT_ENG_SPKPLUGOUTHEADSET, &text );

	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
		/*s_unisoctest_speaker_volume = MMIAPISET_GetCallRingVolume();
		if (0 == s_unisoctest_speaker_volume)
		{
			s_unisoctest_speaker_volume = 3;
		}*/
		s_unisoctest_speaker_volume = 3;
		break;
		
	case MSG_FULL_PAINT:  
		{
			GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
			GUI_RECT_T          client_rect = MMITHEME_GetClientRectEx(win_id);
			GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  IMAGE_COMMON_BG, &lcd_dev_info);
#ifndef _WIN32
			headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
			if (headset_is_plug)
			{
				MMIAPISET_StopAppRing();
				//text.wstr_ptr = headset_is_in_wstr;
				//text.wstr_len = (uint16)MMIAPICOM_Wstrlen( text.wstr_ptr);
				GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
				GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, TRUE);
			}
			else
			{
				MMIAPISET_PlayCallRingByVol(s_unisoctest_speaker_volume, ring_id,AUD_PLAY_FOREVER, MMISET_RING_TYPE_HANDHOLD, PNULL);
				text = s_unisoctest_prompt_str;
				GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
				GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_ENG_RETEST, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			}
		}
		break;
		
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			MMIAPISET_StopAppRing();
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL:  
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			MMIAPISET_StopAppRing();
			SaveUnisocTestResult(win_id, 0);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_UPSIDE:
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			if (s_unisoctest_speaker_volume < MMISET_VOL_MAX)
			{
				s_unisoctest_speaker_volume++;
				MMIAPISET_PlayCallRingByVol(s_unisoctest_speaker_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_HANDHOLD, PNULL);
			}
		}
		break;
		
	case MSG_KEYDOWN_DOWNSIDE:
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			if (s_unisoctest_speaker_volume > MMISET_VOL_ONE)
			{
				s_unisoctest_speaker_volume--;
				MMIAPISET_PlayCallRingByVol(s_unisoctest_speaker_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_HANDHOLD, PNULL);
			}
		}
		break;
		
	case MSG_KEYDOWN_RED:
		break;
		
	case MSG_LOSE_FOCUS:
		// stop melody
		MMIAPISET_StopAppRing();
		break;
		
	case MSG_GET_FOCUS:
		// start melody playing
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			MMIAPISET_PlayCallRingByVol(s_unisoctest_speaker_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_HANDHOLD, PNULL);
		}
		break;
	case MSG_KEYDOWN_WEB:
		MMIAPISET_StopAppRing();
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (!headset_is_plug)
		{
			MMIAPISET_PlayCallRingByVol(s_unisoctest_speaker_volume, ring_id,AUD_PLAY_FOREVER, MMISET_RING_TYPE_HANDHOLD, PNULL);
		}
		break;
	case MSG_KEYDOWN_HEADSET_DETECT:
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		result = MMI_RESULT_FALSE;
		break;
	case MSG_KEYUP_HEADSET_DETECT:
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		result = MMI_RESULT_FALSE;
		break;
	case MSG_CLOSE_WINDOW:
		// stop melody
		MMIAPISET_StopAppRing();
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void UnisocTestSetBacklight(BOOLEAN is_open)
{    
	if (is_open)
	{
		MMIDEFAULT_TurnOnBackLight();
		MMIDEFAULT_SetKeybadBackLight(TRUE);
	}
	else
	{
		MMIDEFAULT_TurnOffBackLight();
#ifndef WIN32   
		MMIDEFAULT_StopKeybadBackLightTimer();
		MMIDEFAULT_SetKeybadBackLight(FALSE);
#endif
		
	}
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestKeypadLedWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T text = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T          client_rect = MMITHEME_GetClientRectEx(win_id);
	
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
		s_unisoctest_backlight_on = TRUE;
		s_unisoctest_backlight_count = 0;
		UnisocTestSetBacklight(TRUE);
		
		if (s_unisoctest_backlight_timer_id != 0)
		{
			MMK_StopTimer(s_unisoctest_backlight_timer_id);
			s_unisoctest_backlight_timer_id = 0;
		}
		s_unisoctest_backlight_timer_id = MMK_CreateWinTimer(win_id,UNISOC_TEST_KEYPAD_LED_DISPLAY_DELAY, TRUE);
		break;
		
	case MSG_FULL_PAINT:  
		GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_RESET, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		break;
		
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
		if(0 == s_unisoctest_backlight_timer_id)
		{
			MMIAPISET_StopAppRing();
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL:  
		if(0 == s_unisoctest_backlight_timer_id)
		{
			MMIAPISET_StopAppRing();
			SaveUnisocTestResult(win_id, 0);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
	case MSG_TIMER:
		if (*(uint8*)param == s_unisoctest_backlight_timer_id) 
		{
			s_unisoctest_backlight_count++;
			if(s_unisoctest_backlight_count < UNISOC_TEST_KEYPAD_LED_DISPLAY_COUNT)
			{
				s_unisoctest_backlight_on = !s_unisoctest_backlight_on;
				UnisocTestSetBacklight(s_unisoctest_backlight_on);
			}
			else
			{
				if (s_unisoctest_backlight_timer_id != 0)
				{
					MMK_StopTimer(s_unisoctest_backlight_timer_id);
					s_unisoctest_backlight_timer_id = 0;
				} 
				UnisocTestSetBacklight(TRUE);
				GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  IMAGE_COMMON_BG, &lcd_dev_info);
				text = s_unisoctest_prompt_str;
				GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, text.wstr_ptr,text.wstr_len, TRUE);
				GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_ENG_RETEST, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			}
			
		}
		else
		{
			result = MMI_RESULT_FALSE; 
		}
		break;
	case MSG_KEYDOWN_WEB:
		if( 0 == s_unisoctest_backlight_timer_id)
		{
			s_unisoctest_backlight_on = TRUE;
			s_unisoctest_backlight_count = 0;
			UnisocTestSetBacklight(TRUE);
			s_unisoctest_backlight_timer_id = MMK_CreateWinTimer(win_id,UNISOC_TEST_KEYPAD_LED_DISPLAY_DELAY, TRUE);
			GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  IMAGE_COMMON_BG, &lcd_dev_info);
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, TRUE);
		}
		break;
	case MSG_LOSE_FOCUS:            
		if (s_unisoctest_backlight_timer_id != 0)
		{
			MMK_StopTimer(s_unisoctest_backlight_timer_id);
			s_unisoctest_backlight_timer_id = 0;
		} 
		UnisocTestSetBacklight(TRUE);
		break;
		
	case MSG_GET_FOCUS:
		if(s_unisoctest_backlight_count < UNISOC_TEST_KEYPAD_LED_DISPLAY_COUNT)
		{
			if (s_unisoctest_backlight_timer_id != 0)
			{
				MMK_StopTimer(s_unisoctest_backlight_timer_id);
				s_unisoctest_backlight_timer_id = 0;
			}                
			s_unisoctest_backlight_timer_id = MMK_CreateWinTimer(win_id, UNISOC_TEST_KEYPAD_LED_DISPLAY_DELAY, TRUE);
			UnisocTestSetBacklight(s_unisoctest_backlight_on);
		}
		break;
	case MSG_KEYDOWN_RED:
		break;
		
	case MSG_CLOSE_WINDOW:
		if (s_unisoctest_backlight_timer_id != 0)
		{
			MMK_StopTimer(s_unisoctest_backlight_timer_id);
			s_unisoctest_backlight_timer_id = 0;
		} 
		if (!s_unisoctest_backlight_on)
		{
			s_unisoctest_backlight_on = TRUE;
			UnisocTestSetBacklight(TRUE);
		}
		break;      
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E UnisocTestMicReceiverWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T text = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	const  uint16  test_call_volume = 9;
	BOOLEAN headset_is_plug = FALSE;
	wchar headset_is_in_wstr[] = { 'P', 'l', 'e', 'a', 's', 'e', ' ', 'p', 'l', 'u', 'g', ' ', 'o', 'u', 't', ' ', 'h', 'e', 'a', 'd', 's', 'e', 't', 0 };
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		break;
	case MSG_FULL_PAINT:
		{
			GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
			GUI_RECT_T          client_rect = MMITHEME_GetClientRectEx(win_id);
			GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  IMAGE_COMMON_BG, &lcd_dev_info);
#ifndef _WIN32
			headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
			if (headset_is_plug)
			{
				text.wstr_ptr = headset_is_in_wstr;
				text.wstr_len = (uint16)MMIAPICOM_Wstrlen( text.wstr_ptr);
				GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
				GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, TRUE);
			}
			else
			{
				//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_HANDHOLD,AUD_INPUT_TYPE_MAINMIC,test_call_volume,TRUE);	
				MMISRVAUD_EnableVoiceLB(TRUE, test_call_volume);
				text = s_unisoctest_prompt_str;
				GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
				GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			}
		}
		break;
		
	case MSG_KEYDOWN_OK:  
	case MSG_CTL_OK: 
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if(!headset_is_plug)
		{
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL: 
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if(!headset_is_plug)
		{
			SaveUnisocTestResult(win_id, 0);
			MMIAPIENG_RunUnisocTestNextStep();   
			MMK_CloseWin(  win_id) ;
		}
		break;
		
	case MSG_KEYDOWN_RED:
		break;
		
	case MSG_GET_FOCUS:
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if(!headset_is_plug)
		{
			//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_HANDHOLD,AUD_INPUT_TYPE_MAINMIC,test_call_volume,TRUE);	
			MMISRVAUD_EnableVoiceLB(TRUE, test_call_volume);
		}
		break;
	case MSG_KEYDOWN_HEADSET_DETECT:
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_HANDHOLD,AUD_INPUT_TYPE_MAINMIC,test_call_volume,FALSE);
		MMISRVAUD_EnableVoiceLB(FALSE, test_call_volume);
		result = MMI_RESULT_FALSE;
		break;
		
	case MSG_KEYUP_HEADSET_DETECT:
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		result = MMI_RESULT_FALSE;
		break;
	case MSG_LOSE_FOCUS:   
	case MSG_CLOSE_WINDOW:
		//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_HANDHOLD,AUD_INPUT_TYPE_MAINMIC,test_call_volume,FALSE);	
		MMISRVAUD_EnableVoiceLB(FALSE, test_call_volume);
		break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E UnisocTestMicSpeakerWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T text = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	const  uint16  test_call_volume = 6;
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_HANDFREE,AUD_INPUT_TYPE_MAINMIC,test_call_volume,TRUE);
		MMISRVAUD_EnableVoiceLBWithRoute(TRUE, test_call_volume,MMISRVAUD_ROUTE_SPEAKER);
		break;
	case MSG_FULL_PAINT:
		{
			GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
			GUI_RECT_T          client_rect = MMITHEME_GetClientRectEx(win_id);
			GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  IMAGE_COMMON_BG, &lcd_dev_info);
			text = s_unisoctest_prompt_str;
			GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		}
		break;
		
	case MSG_KEYDOWN_OK:  
	case MSG_CTL_OK: 
		SaveUnisocTestResult(win_id, 1);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);        
		break;
		
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL: 
		SaveUnisocTestResult(win_id, 0);
		MMIAPIENG_RunUnisocTestNextStep();   
		MMK_CloseWin(  win_id) ;      
		break;
		
	case MSG_KEYDOWN_RED:
		break;
		
	case MSG_GET_FOCUS:
		//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_HANDFREE,AUD_INPUT_TYPE_MAINMIC,test_call_volume,TRUE);
		MMISRVAUD_EnableVoiceLBWithRoute(TRUE, test_call_volume,MMISRVAUD_ROUTE_SPEAKER);
		break;
		
	case MSG_LOSE_FOCUS:   
	case MSG_CLOSE_WINDOW:
		//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_HANDFREE,AUD_INPUT_TYPE_MAINMIC,test_call_volume,FALSE);
		MMISRVAUD_EnableVoiceLBWithRoute(FALSE, test_call_volume,MMISRVAUD_ROUTE_SPEAKER);
		break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E UnisocTestHeadsetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T textin = {0};
	MMI_STRING_T textout = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	//wchar headset_is_in_wstr[] = { 'H', 'e', 'a', 'd', 's', 'e', 't', ' ', 'i', 's', ' ', 'i', 'n', ' ', 0 };
	//wchar headset_is_out_wstr[] = { 'H', 'e', 'a', 'd', 's', 'e', 't', ' ', 'i', 's', ' ', 'o', 'u', 't', 0 };
	GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
	GUI_RECT_T lab_rect = client_rect;
	BOOLEAN headset_is_plug = FALSE;
	static BOOLEAN headset_has_plug = FALSE;
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	uint8 call_vol = MMIAPISET_GetCallRingVolume();
	if (0 == call_vol)
	{
		call_vol = 3;
	}
	
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:        
		lab_rect = client_rect;
		lab_rect.bottom =   (client_rect.bottom + client_rect.top)/2;
		GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
#ifndef _WIN32
		headset_has_plug = GPIO_CheckHeadsetStatus();
#endif
		if(headset_has_plug)
		{
			MMIAPISET_PlayCallRingByVol(call_vol, 1,AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
		}
		
		break;
	case MSG_FULL_PAINT:
		LCD_FillRect(&lcd_dev_info, client_rect, UITEST_COLOR_WHITE);
		GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (headset_is_plug)
		{
			MMI_GetLabelTextByLang(TXT_ENG_HEADSETIN, &textin );
			//text.wstr_ptr = headset_is_in_wstr;//(uint8*)"Headset is in ";
		//	textin.wstr_len = (uint16)MMIAPICOM_Wstrlen( textin.wstr_ptr);
			GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &textin, TRUE);
		}
		else
		{
			MMI_GetLabelTextByLang(TXT_ENG_HEADSETOUT, &textout );
			//text.wstr_ptr = headset_is_out_wstr;//(uint8*)"Headset is out";
		//	textout.wstr_len = (uint16)MMIAPICOM_Wstrlen( textout.wstr_ptr);
			GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &textout, TRUE);
		}

		if(headset_has_plug)
		{
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		}
		break;
		
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
		if(headset_has_plug)
		{
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:         
	case MSG_CTL_CANCEL: 
	//	if(headset_has_plug)
		{
			SaveUnisocTestResult( win_id, 0) ;
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(  win_id);
		}
		break;
		
	case MSG_KEYDOWN_RED:
		break;
	case MSG_KEYDOWN_WEB:
		break;
	case MSG_GET_FOCUS:
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (headset_is_plug)
		{
			MMIAPISET_PlayCallRingByVol(call_vol, 1,AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
		}
		
		break;
		
	case MSG_KEYDOWN_HEADSET_DETECT:
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		headset_has_plug = TRUE;
		MMIAPISET_PlayCallRingByVol(call_vol, 1,AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
		result = MMI_RESULT_FALSE;
		break;
		
	case MSG_KEYUP_HEADSET_DETECT:
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		MMIAPISET_StopAppRing();
		result = MMI_RESULT_FALSE;
		break;
	case MSG_LOSE_FOCUS:            
	case MSG_CLOSE_WINDOW:
		MMIAPISET_StopAppRing();
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E UnisocTestMicHeadsetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T textin = {0};
	MMI_STRING_T textout = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	//wchar headset_is_in_wstr[] = { 'H', 'e', 'a', 'd', 's', 'e', 't', ' ', 'i', 's', ' ', 'i', 'n', ' ', 0 };
	//wchar headset_is_out_wstr[] = { 'H', 'e', 'a', 'd', 's', 'e', 't', ' ', 'i', 's', ' ', 'o', 'u', 't', 0 };
	GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
	GUI_RECT_T lab_rect = client_rect;
	BOOLEAN headset_is_plug = FALSE;
	const  uint16  test_call_volume = 9;
	static BOOLEAN headset_has_plug = FALSE;
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
#ifndef _WIN32
		headset_has_plug = GPIO_CheckHeadsetStatus();
#endif
		if(headset_has_plug)
		{
			//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_EARPHONE,AUD_INPUT_TYPE_HPMIC_USE_L,test_call_volume,TRUE);
			MMISRVAUD_EnableVoiceLB(TRUE, test_call_volume);	
		}
		
		break;
	case MSG_FULL_PAINT:
		LCD_FillRect(&lcd_dev_info, client_rect, UITEST_COLOR_WHITE);
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (headset_is_plug)
		{
			MMI_GetLabelTextByLang(TXT_ENG_HEADSETIN, &textin );
			//text.wstr_ptr = headset_is_in_wstr;//(uint8*)"Headset is in ";
			//textin.wstr_len = (uint16)MMIAPICOM_Wstrlen( textin.wstr_ptr);
			GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &textin, TRUE);
		}
		else
		{
			MMI_GetLabelTextByLang(TXT_ENG_HEADSETOUT, &textout );
			//text.wstr_ptr = headset_is_out_wstr;//(uint8*)"Headset is out";
		//	textout.wstr_len = (uint16)MMIAPICOM_Wstrlen( textout.wstr_ptr);
			GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &textout, TRUE);
		}

		if(headset_has_plug)
		{
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		}else{
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_RESET, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		}
		break;
		
	case MSG_KEYDOWN_OK:  
	case MSG_CTL_OK: 
		if(headset_has_plug)
		{
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL:
	//	if(headset_has_plug)
		{
			SaveUnisocTestResult(win_id, 0);
			MMIAPIENG_RunUnisocTestNextStep();   
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_RED:
		break;
		
	case MSG_KEYDOWN_HEADSET_DETECT:
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		headset_has_plug = TRUE;
		//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_EARPHONE,AUD_INPUT_TYPE_HPMIC_USE_L,test_call_volume,TRUE);	
		MMISRVAUD_EnableVoiceLB(TRUE, test_call_volume);
		result = MMI_RESULT_FALSE;
		break;
		
	case MSG_KEYUP_HEADSET_DETECT:
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_EARPHONE,AUD_INPUT_TYPE_HPMIC_USE_L,test_call_volume,FALSE);	
		MMISRVAUD_EnableVoiceLB(FALSE, test_call_volume);
		result = MMI_RESULT_FALSE;
		break;
		
	case MSG_GET_FOCUS:
#ifndef _WIN32
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
		if (headset_is_plug)
		{
			//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_EARPHONE,AUD_INPUT_TYPE_HPMIC_USE_L,test_call_volume,TRUE);	
			MMISRVAUD_EnableVoiceLB(TRUE, test_call_volume);
		}
		
		break;
		
	case MSG_LOSE_FOCUS:   
	case MSG_CLOSE_WINDOW:
		//ARMVB_TestModeStart_Direct(AUDIO_DEVICE_MODE_EARPHONE,AUD_INPUT_TYPE_HPMIC_USE_L,test_call_volume,FALSE);	
		MMISRVAUD_EnableVoiceLB(FALSE, test_call_volume);
		break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E UnisocTestTFlashWinHandleMsg(
											  MMI_WIN_ID_T  win_id, 
											  MMI_MESSAGE_ID_E msg_id, 
											  DPARAM           param
											  )
{
	MMI_STRING_T textin = {0};
	MMI_STRING_T textout = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	//wchar have_sdcard_wstr[] = { 'H', 'a', 'v', 'e', ' ', 'S', 'D', 'C', 'a', 'r', 'd', 0 };
	//wchar no_sdcard_wstr[] = { 'N', 'o', ' ', 'S', 'D', 'C', 'a', 'r', 'd', 0 };

	MMIFILE_DEVICE_E device = MMI_DEVICE_SDCARD;
	BOOLEAN sd_card_ok = FALSE;
	MMI_GetLabelTextByLang(TXT_ENG_SDIN, &textin );
	MMI_GetLabelTextByLang(TXT_ENG_SDOUT, &textout );
	
	do
	{
		if(MMIAPIFMM_GetDeviceTypeStatus(device))
		{
			sd_card_ok = TRUE;
			break;
		}
	}while(++device < MMI_DEVICE_NUM);
	
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
		
		break;
	case MSG_FULL_PAINT:
		{
			GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
			GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  
				IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
		}
		
		if(sd_card_ok)
		{
			//text.wstr_ptr = have_sdcard_wstr;//(uint8*)"Have SDCard";
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			//textin.wstr_len = MMIAPICOM_Wstrlen( textin.wstr_ptr);
			GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &textin, FALSE);
		}
		else
		{
			//text.wstr_ptr = no_sdcard_wstr;//(uint8*)"No SDCard";
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			//textout.wstr_len = MMIAPICOM_Wstrlen( textout.wstr_ptr);
			GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &textout, FALSE);
		}

		break;
		
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
		if(sd_card_ok)
		{
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:    
	case MSG_CTL_CANCEL:
		//if(sd_card_ok)
		//{
			SaveUnisocTestResult(win_id, 0);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id) ;    
		//}
		
		break;  
	case MSG_KEYDOWN_RED:
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E UnisocTestSimWinHandleMsg(
										   MMI_WIN_ID_T  win_id, 
										   MMI_MESSAGE_ID_E msg_id, 
										   DPARAM           param
										   )
{
	MMI_STRING_T textin = {0};
	MMI_STRING_T textout = {0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint8           *str_ptr = PNULL;
	GUI_RECT_T      lab_rect = {0};
	GUI_RECT_T      client_rect = MMITHEME_GetClientRectEx(win_id);
	//wchar have_simcard_wstr[] = { 'H', 'a', 'v', 'e', ' ', 'S', 'I', 'M', ' ', 'C', 'a', 'r', 'd', 0 };
	//wchar no_simcard_wstr[] = { 'N', 'o', ' ',  'S', 'I', 'M', ' ', 'C', 'a', 'r', 'd', 0 };

	BOOLEAN sim1_card_ok = FALSE;
	BOOLEAN sim2_card_ok = FALSE;
	MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
		for(i = 0; i <MMI_DUAL_SYS_MAX; i++)
		{
			if(MMIAPIPHONE_GetSimExistedStatus(0))
			{
				sim1_card_ok = TRUE;
			}
			if(MMIAPIPHONE_GetSimExistedStatus(1))
			{				
				sim2_card_ok = TRUE;
			}
		}
		
	
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
	
		lab_rect = client_rect;
		lab_rect.bottom = (client_rect.bottom + client_rect.top)/2;
		GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);
		
		lab_rect.top =  lab_rect.bottom + 1;
		lab_rect.bottom = client_rect.bottom ;
		GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
		break;
	case MSG_FULL_PAINT:
		{
			GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  
				IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
		}
#ifdef MMI_MULTI_SIM_SYS_SINGLE
             if(sim1_card_ok)
		{
			MMI_GetLabelTextByLang(TXT_ENG_SIMIN, &textin );
			//text.wstr_ptr = have_simcard_wstr;//(uint8*)"Have simCard";
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &textin, FALSE);
		}
		else
		{
			MMI_GetLabelTextByLang(TXT_ENG_SIMOUT, &textout );	
			//text.wstr_ptr = no_simcard_wstr;//(uint8*)"No simCard";
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);/*TXT_NULL*/
			GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &textout, FALSE);
		}
		//text.wstr_len = MMIAPICOM_Wstrlen( text.wstr_ptr);
#else
	//SCI_TRACE_LOW("sim1_card_ok %d",sim1_card_ok);
	//SCI_TRACE_LOW("sim2_card_ok %d",sim2_card_ok);
		if(sim1_card_ok)
		{	
			str_ptr = (uint8*)"Have SIM1 Card";
			UnisocTestSetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, (char*)str_ptr, 35, FALSE);		
		}
		else
		{
			str_ptr = (uint8*)"No SIM1 Card";
			UnisocTestSetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, (char*)str_ptr, 35, FALSE);			
		}
		str_ptr=PNULL;
		if(sim2_card_ok)
		{
			str_ptr = (uint8*)"Have SIM2 Card";
			UnisocTestSetLabelTextByCharArray(MMIENG_LABEL1_CTRL_ID, (char*)str_ptr, 35, FALSE);			
		}
		else
		{
			str_ptr = (uint8*)"No SIM2 Card";
			UnisocTestSetLabelTextByCharArray(MMIENG_LABEL1_CTRL_ID, (char*)str_ptr, 35, FALSE);
		}
		if(sim1_card_ok && sim2_card_ok)
		{
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
		}
		else
		{
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);/*TXT_NULL*/
		}
		str_ptr=PNULL; 
			
#endif
		break;
		
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
		
#ifdef MMI_MULTI_SIM_SYS_SINGLE
		if(sim1_card_ok)
#else
		if(sim1_card_ok && sim2_card_ok)
#endif		
             {
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:    
	case MSG_CTL_CANCEL:
		//if(sim_card_ok)
		{
			SaveUnisocTestResult(win_id, 0);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(  win_id) ;    
		}
		
		break;  
	case MSG_KEYDOWN_RED:
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestChargingWinHandleMsg(MMI_WIN_ID_T win_id, 
										  MMI_MESSAGE_ID_E msg_id, 
										  DPARAM param
										  )
{
	MMI_RESULT_E            result = MMI_RESULT_TRUE;
	MMI_STRING_T text = {0};
	CHGMNG_STATE_INFO_T charging_info = {0};
	//wchar have_charging_wstr[] = { 'C', 'h', 'a', 'r', 'g', 'i', 'n', 'g', ' ', 'P', 'l', 'u', 'g', ' ', 'I', 'n', 0 };
	//wchar no_charging_wstr[] = { 'C', 'h', 'a', 'r', 'g', 'i', 'n', 'g', ' ', 'P', 'l', 'u', 'g', ' ', 'O', 'u', 't', 0 };
	char temp_str[64] = {0};
	wchar temp_wstr[64] = {0};
	uint8 i = 0;
	uint16 temp_str_len = 0; 
	  GUI_FONT_T font = SONG_FONT_20;
    GUI_COLOR_T font_color = MMI_BLACK_COLOR;
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
		if (s_unisoctest_charging_timer_id != 0)
		{
			MMK_StopTimer(s_unisoctest_charging_timer_id);
			s_unisoctest_charging_timer_id = 0;
		}
		s_unisoctest_charging_timer_id = MMK_CreateWinTimer(win_id,UNISOC_TEST_CHARGING_DETECT_DELAY, TRUE);
		break;
		
	case MSG_FULL_PAINT:
		{
		GUI_RECT_T      lab_rect = {0};
		GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
		GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  
			IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());

		lab_rect = client_rect;
		lab_rect.top = client_rect.top+10;
		lab_rect.bottom = (client_rect.bottom + client_rect.top)/7+20+ 10;
		GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);

		lab_rect.top =  lab_rect.bottom + 8;
		lab_rect.bottom = (client_rect.bottom + client_rect.top)/7 + lab_rect.bottom ;
		GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);

		lab_rect.top =  lab_rect.bottom + 1;
		lab_rect.bottom = (client_rect.bottom + client_rect.top)/7 + lab_rect.bottom ;
		GUILABEL_SetRect(MMIENG_LABEL2_CTRL_ID, &lab_rect, FALSE);

		lab_rect.top =  lab_rect.bottom + 1;
		lab_rect.bottom = (client_rect.bottom + client_rect.top)/7 + lab_rect.bottom ;
		GUILABEL_SetRect(MMIENG_LABEL3_CTRL_ID, &lab_rect, FALSE);

		lab_rect.top =  lab_rect.bottom + 1;
		lab_rect.bottom = (client_rect.bottom + client_rect.top)/7 + lab_rect.bottom ;
		GUILABEL_SetRect(MMIENG_LABEL4_CTRL_ID, &lab_rect, FALSE);

		charging_info = *CHGMNG_GetModuleState();
		if(charging_info.chgmng_state != CHGMNG_IDLE)
		{

			MMI_GetLabelTextByLang(TXT_ENG_PLUGIN, &text );
			//text.wstr_ptr = have_charging_wstr;
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			//text.wstr_len = MMIAPICOM_Wstrlen( text.wstr_ptr);
			CTRLLABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,font,font_color);
			GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
			{		
				sprintf(temp_str, "Voltage: %6.2f", (double)(charging_info.bat_statistic_vol) / 1000);  
				temp_str_len = strlen((char*)temp_str); 
				MMI_STRNTOWSTR( temp_wstr, 64,  (const uint8 *)temp_str, temp_str_len, temp_str_len);
				text.wstr_ptr = temp_wstr;
				text.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
				 CTRLLABEL_SetFont(MMIENG_LABEL1_CTRL_ID,font,font_color);
				GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
				SCI_MEMSET(temp_str, 0, 64);
            			SCI_MEMSET(temp_wstr, 0, 64*2);
				
				sprintf(&temp_str, "Cur voltage: %6.2f", (double)(charging_info.bat_cur_vol) / 1000);
				temp_str_len = strlen((char*)temp_str); 
				MMI_STRNTOWSTR(temp_wstr, 64,  (const uint8 *)temp_str, temp_str_len, temp_str_len);
				text.wstr_ptr = temp_wstr;
				text.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
				 CTRLLABEL_SetFont(MMIENG_LABEL2_CTRL_ID,font,font_color);
				GUILABEL_SetText(MMIENG_LABEL2_CTRL_ID, &text, TRUE);
				SCI_MEMSET(temp_str, 0, 64);
            			SCI_MEMSET(temp_wstr, 0, 64*2);
				
				sprintf(&temp_str, "Charging voltage: %6.2f", (double)(charging_info.chg_vol) / 1000);
				temp_str_len = strlen((char*)temp_str); 
				MMI_STRNTOWSTR( temp_wstr, 64,  (const uint8 *)temp_str, temp_str_len, temp_str_len);
				text.wstr_ptr = temp_wstr;
				text.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
				 CTRLLABEL_SetFont(MMIENG_LABEL3_CTRL_ID,font,font_color);
				GUILABEL_SetText(MMIENG_LABEL3_CTRL_ID, &text, TRUE);
				SCI_MEMSET(temp_str, 0, 64);
            			SCI_MEMSET(temp_wstr, 0, 64*2);
			
				sprintf(&temp_str, "Charging cvalue: %6.2f", (double)(charging_info.charging_current) / 1000);
				temp_str_len = strlen((char*)temp_str); 
				MMI_STRNTOWSTR( temp_wstr, 64,  (const uint8 *)temp_str, temp_str_len, temp_str_len);
				text.wstr_ptr = temp_wstr;
				text.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
				 CTRLLABEL_SetFont(MMIENG_LABEL4_CTRL_ID,font,font_color);	
				GUILABEL_SetText(MMIENG_LABEL4_CTRL_ID, &text, TRUE);
				SCI_MEMSET(temp_str, 0, 64);
            			SCI_MEMSET(temp_wstr, 0, 64*2);					
			}		
		}
		else
		{
			MMI_GetLabelTextByLang(TXT_ENG_PLUGOUT, &text);	
			//text.wstr_ptr = no_charging_wstr;
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			//text.wstr_len = MMIAPICOM_Wstrlen( text.wstr_ptr);
			CTRLLABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,font,font_color);
			GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);	
			{		
				sprintf(temp_str, " ");  
				temp_str_len = strlen((char*)temp_str); 
				MMI_STRNTOWSTR( temp_wstr, 64,  (const uint8 *)temp_str, temp_str_len, temp_str_len);
				text.wstr_ptr = temp_wstr;
				text.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
				CTRLLABEL_SetFont(MMIENG_LABEL1_CTRL_ID,font,font_color);	
				GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
				SCI_MEMSET(temp_str, 0, 64);
            			SCI_MEMSET(temp_wstr, 0, 64*2);
				
				sprintf(temp_str, " ");
				temp_str_len = strlen((char*)temp_str); 
				MMI_STRNTOWSTR(temp_wstr, 64,  (const uint8 *)temp_str, temp_str_len, temp_str_len);
				text.wstr_ptr = temp_wstr;
				text.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
				CTRLLABEL_SetFont(MMIENG_LABEL2_CTRL_ID,font,font_color);
				GUILABEL_SetText(MMIENG_LABEL2_CTRL_ID, &text, TRUE);
				SCI_MEMSET(temp_str, 0, 64);
            			SCI_MEMSET(temp_wstr, 0, 64*2);
				
				sprintf(temp_str, " ");
				temp_str_len = strlen((char*)temp_str); 
				MMI_STRNTOWSTR( temp_wstr, 64,  (const uint8 *)temp_str, temp_str_len, temp_str_len);
				text.wstr_ptr = temp_wstr;
				text.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
					CTRLLABEL_SetFont(MMIENG_LABEL3_CTRL_ID,font,font_color);
				GUILABEL_SetText(MMIENG_LABEL3_CTRL_ID, &text, TRUE);
				SCI_MEMSET(temp_str, 0, 64);
            			SCI_MEMSET(temp_wstr, 0, 64*2);
			
				sprintf(temp_str, " ");
				temp_str_len = strlen((char*)temp_str); 
				MMI_STRNTOWSTR( temp_wstr, 64,  (const uint8 *)temp_str, temp_str_len, temp_str_len);
				text.wstr_ptr = temp_wstr;
				text.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
				CTRLLABEL_SetFont(MMIENG_LABEL4_CTRL_ID,font,font_color);
				GUILABEL_SetText(MMIENG_LABEL4_CTRL_ID, &text, TRUE);
				SCI_MEMSET(temp_str, 0, 64);
            			SCI_MEMSET(temp_wstr, 0, 64*2);					
			}
		}

		}
		break;   
	case MSG_TIMER:
		if (*(uint8*)param == s_unisoctest_charging_timer_id) 
		{
			if(MMK_IsFocusWin(win_id))
			{
				MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
			}
		}
		else
		{
			result = MMI_RESULT_FALSE; 
		}
		break;
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
		charging_info = *CHGMNG_GetModuleState();
		if(charging_info.chgmng_state != CHGMNG_IDLE)
		{
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}        break;
		
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL:   
		/*charging_info = *CHGMNG_GetModuleState();
		if(charging_info.chgmng_state != CHGMNG_IDLE)*/
		{
			SaveUnisocTestResult(win_id, 0);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_RED:           
		break;   
		
		
	case MSG_CLOSE_WINDOW:
		if (s_unisoctest_charging_timer_id != 0)
		{
			MMK_StopTimer(s_unisoctest_charging_timer_id);
			s_unisoctest_charging_timer_id = 0;
		} 
		break;   
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T UnisocTestRequestVirtualAudService(void)
{
	MMISRVMGR_SERVICE_REQ_T req = {0};
	MMISRVAUD_TYPE_T audio_srv = {0};
	
	req.pri = MMISRVAUD_PRI_HIGH;
	audio_srv.info.type = MMISRVAUD_TYPE_VIRTUAL;
	return MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL int8 UnisocTestGetSensorID(void)
{
#ifdef  CAMERA_SUPPORT
    SENSOR_ID_E sensor_id = 0;
    uint32 sensor_param = 0;
	
    DCAMERA_GetInfo(DCAMERA_INFO_CUR_IMG_SENSOR, &sensor_param);
    sensor_id = sensor_param;

    return sensor_id;
#else
    return 0;
#endif
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN UnisocTestDcCreateSoftkey(MMI_WIN_ID_T win_id)
{
	GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
	MMI_CONTROL_CREATE_T softkey_create = {0};
	
	softkey_data.leftsoft_id = TXT_NULL;
	softkey_data.midsoft_id = TXT_NULL;
	softkey_data.rightsoft_id = TXT_NULL;
	
	softkey_create.ctrl_id           = MMITHEME_GetSoftkeyCtrlId();
	softkey_create.guid              = SPRD_GUI_SOFTKEY_ID;
	softkey_create.init_data_ptr     = &softkey_data;
	softkey_create.parent_win_handle = win_id;
	MMK_CreateControl(&softkey_create);
	MMK_SetWinSoftkeyCtrlId(win_id,softkey_create.ctrl_id);
	MMK_SetCtrlLcdDevInfo(softkey_create.ctrl_id, &s_unisoctest_camera_osdlayer_handle);
	return TRUE;
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN UnisocTestDcCreateOSDLayer(MMI_WIN_ID_T win_id)
{
	BOOLEAN result = FALSE;
#ifdef UI_MULTILAYER_SUPPORT	
	UILAYER_CREATE_T    create_info = {0};
	GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	UILAYER_RESULT_E ret = 0;
	UILAYER_APPEND_BLT_T append_layer = {0};
	LCD_INFO_T lcd_info = {0};
	
	if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&dev_info))
	{
		//creat layer
		GUILCD_GetInfo(0, &lcd_info);
		
		create_info.lcd_id = GUI_MAIN_LCD_ID;
		create_info.owner_handle = MMIENG_UNISOCTEST_CAMERA_WIN_ID;
		create_info.offset_x = 0;
		create_info.offset_y = 0;
		create_info.width = lcd_info.lcd_width;
		create_info.height = lcd_info.lcd_height;
		create_info.is_bg_layer = FALSE;
		create_info.is_static_layer = TRUE; 
		ret = UILAYER_CreateLayer(
			&create_info,
			&dev_info
			);
		
		if (UILAYER_RESULT_SUCCESS == ret)
		{
			result = TRUE;
			
			//UILAYER_RemoveMainLayer();
			//UILAYER_EnableOsdLayer(TRUE);
			s_unisoctest_camera_osdlayer_handle = dev_info;
			
			append_layer.layer_level = UILAYER_LEVEL_NORMAL;
			append_layer.lcd_dev_info = dev_info;
			UILAYER_AppendBltLayer(&append_layer);
		}
		
	}

	if (UILAYER_IsLayerActive(&s_unisoctest_camera_osdlayer_handle))
	{
		UILAYER_Clear(&s_unisoctest_camera_osdlayer_handle);
		UnisocTestDcCreateSoftkey(win_id);
	}
#endif
	return result;
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void UnisocTestDcReleaseOSDLayer(void)
{
#ifdef UI_MULTILAYER_SUPPORT	
	// 释放层
	if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_unisoctest_camera_osdlayer_handle))    
	{
		UILAYER_RELEASELAYER(&s_unisoctest_camera_osdlayer_handle);
	}
#endif
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E UnisocTestDcPreview()
{
	DCAMERA_RETURN_VALUE_E  ret = DCAMERA_OP_SUCCESS;
	DCAMERA_PREVIEW_PARAM_T preview_param = {0};
	LCD_INFO_T lcd_info = {0};
	GUI_LCD_DEV_INFO dev_info_ptr = {0};
	GUI_RECT_T       rect = {0};    
	MMI_STRING_T     text_str = {0};
	
	GUISTR_STYLE_T   text_style = {0};
	GUISTR_STATE_T   state = GUISTR_STATE_BOLD;
	
	GUI_COLOR_T      color = MMI_BLACK_COLOR;
	
	uint32 sensor_number = 0;
	GUI_RECT_T  str_rect = {0};
	wchar  sensor_wchar[] = {'S', '1', '/', '1',0};
	int16 cur_sensor_id = 0;

    MMI_STRING_T CaptureStr = {0};
    MMI_STRING_T FailStr = {0};
    GUI_RECT_T CaptureRect = {0};
    GUI_RECT_T FailRect ={0};

	// text style
	text_style.char_space = 1;
	text_style.line_space = 0;
	text_style.effect = FONT_EFFECT_NONE;
	text_style.font = SONG_FONT_16;
	text_style.font_color = MMI_GRAY_WHITE_COLOR;
	
	GUILCD_GetInfo(0, &lcd_info);
	
	
	cur_sensor_id = UnisocTestGetSensorID();
	
	sensor_wchar[1] = sensor_wchar[1] + cur_sensor_id;
	sensor_wchar[3] = sensor_wchar[3] + sensor_number - 1;
	
	DCAMERA_StopPreview();
	
	//填充透明色
	rect = MMITHEME_GetFullScreenRect();
	GUI_FillRect(&dev_info_ptr, rect, color);
	MMITHEME_UpdateRect();
	
	str_rect.left = 10;
	str_rect.right = str_rect.left + 50 - 1;
	str_rect.top = 5;
	str_rect.bottom = str_rect.top + 20 - 1;
	
	text_str.wstr_ptr = sensor_wchar;
	text_str.wstr_len = MMIAPICOM_Wstrlen(sensor_wchar);
	
	// display
	GUISTR_DrawTextToLCDInRect( 
		(const GUI_LCD_DEV_INFO *)&dev_info_ptr,
		(const GUI_RECT_T      *)&str_rect,   
		(const GUI_RECT_T      *)&str_rect, 
		(const MMI_STRING_T    *)&text_str,
		&text_style,
		state,
		GUISTR_TEXT_DIR_AUTO
		);
	
	MMITHEME_UpdateRect();

	//需要设置默认参数，否则会画面暗
	DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_BRIGHTNESS, DCAMERA_EV_03,PNULL);
	
	DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_CONTRAST, DCAMERA_EV_03,PNULL);
	
	DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EFFECT, DCAMERA_EFFECT_NORMAL,PNULL);
	
	DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION, DCAMERA_EV_03,PNULL); 
	
	DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_FLICKER_COMPENSATION, DCAMERA_EV_00,PNULL);
	
	DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_PREVIEW_MODE, DCAMERA_ENVIRONMENT_NORMAL,PNULL);
	DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_WB_MODE, DCAMERA_WB_MODE_AUTO,PNULL);
	
	
	preview_param.lcd_id        = GUI_MAIN_LCD_ID;
	preview_param.disp_rect.w   = lcd_info.lcd_width;
	preview_param.disp_rect.h   = lcd_info.lcd_height;
	preview_param.disp_mode = DCAMERA_DISP_FULLSCREEN;
	preview_param.rotation_mode = DCAMERA_ROTATION_90;
	
	preview_param.disp_rect.x   = 0;
	preview_param.disp_rect.y   = 0;
	preview_param.lcd_block_id  = GUI_BLOCK_0;
	preview_param.target_rect = preview_param.disp_rect;
	
#if 1

    SCI_TRACE_LOW("unisoc lcd_id = %d",preview_param.lcd_id);
    SCI_TRACE_LOW("unisoc lcd_block_id = %d",preview_param.lcd_block_id);
    SCI_TRACE_LOW("unisoc rotation_mode = %d",preview_param.rotation_mode);
    SCI_TRACE_LOW("unisoc disp_mode = %d",preview_param.disp_mode);

    SCI_TRACE_LOW("unisoc disp_rect = %d, %d, %d,%d", 
        preview_param.disp_rect.x,
        preview_param.disp_rect.y,
        preview_param.disp_rect.w,
        preview_param.disp_rect.h);
    SCI_TRACE_LOW("unisoc target_rect w = %d, h = %d.",
        preview_param.target_rect.w, preview_param.target_rect.h);

    //display softkey
    MMI_GetLabelTextByLang(TXT_ENG_CAMERA_CAPTURE, &CaptureStr);
    MMI_GetLabelTextByLang(TXT_ENG_FAIL, &FailStr);

    CaptureRect.left = 10;
    CaptureRect.top = preview_param.disp_rect.h - 25 -1;
    CaptureRect.right = CaptureRect.left +50;
    CaptureRect.bottom = CaptureRect.top +50;

    SCI_TRACE_LOW("unisoc CaptureRect = %d, %d, %d, %d", 
        CaptureRect.left,CaptureRect.top, CaptureRect.right, CaptureRect.bottom);

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
        (const GUI_RECT_T      *)&CaptureRect,   
        (const GUI_RECT_T      *)&CaptureRect, 
        (const MMI_STRING_T    *)&CaptureStr,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    FailRect.left = preview_param.disp_rect.w - 40 -1;
    FailRect.top = CaptureRect.top;
    FailRect.right = FailRect.left +50;
    FailRect.bottom = FailRect.top +50;

    SCI_TRACE_LOW("unisoc FailRect = %d, %d, %d, %d", 
        FailRect.left,FailRect.top, FailRect.right, FailRect.bottom);	

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
        (const GUI_RECT_T      *)&FailRect,   
        (const GUI_RECT_T      *)&FailRect, 
        (const MMI_STRING_T    *)&FailStr,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

#endif

	ret = DCAMERA_StartPreview(&preview_param);
	
	if(DCAMERA_OP_SUCCESS != ret)
	{
		
		DCAMERA_Close();
		rect = MMITHEME_GetFullScreenRect();
		GUI_FillRect(&dev_info_ptr, rect, MMI_WINDOW_BACKGROUND_COLOR);
		MMI_GetLabelTextByLang(TXT_DC_OPEN_FAIL , &text_str);
		rect.top = rect.bottom / 4;
		rect.left = (rect.right - GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, SONG_FONT_16, 1)) / 2;
		
		
		// display
		GUISTR_DrawTextToLCDInRect( 
			(const GUI_LCD_DEV_INFO *)&dev_info_ptr,
			(const GUI_RECT_T      *)&rect, 
			(const GUI_RECT_T      *)&rect,
			(const MMI_STRING_T    *)&text_str,
			&text_style,
			state,
			GUISTR_TEXT_DIR_AUTO
			);
		
		MMITHEME_UpdateRect();
	}  
	
	return ret;
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E UnisocTestDcSaveDataToFile(uint8 *data_buffer,uint32 data_len)
{
#if 0
	MMIFILE_ERROR_E ret = SFS_ERROR_SYSTEM;
	wchar save_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	BOOLEAN create_ret = FALSE;
	wchar *temp_file_name_ptr;
	SFS_HANDLE sfs_handle = 0;
	uint32 write_size = 0;
	MMIFILE_ERROR_E  write_ret = 0;
	
	
	if ((PNULL == data_buffer)
		|| (0 == data_len)
		)
	{
		return ret;
	}
	MMIAPIFMM_CombineFullPathEx(save_path,MMIFILE_FULL_PATH_MAX_LEN,MMI_DEVICE_UDISK,MMIMULTIM_DIR_PICTURE, PNULL);
	ret = SFS_CreateFileAsyn(save_path, s_unisoctest_file_name, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL, &sfs_handle, PNULL);
	if (SFS_ERROR_NONE == ret)
	{
		write_ret = MMIAPIFMM_WriteFile(sfs_handle, data_buffer, data_len, &write_size, PNULL);
		if( (SFS_NO_ERROR == write_ret) && (data_len == write_size) )
		{
		}
	}
	else
	{
	}
	if(0 != sfs_handle)
	{ 
		SFS_CloseFile(sfs_handle);
	}
	return ret;
#else
    uint32 opt_mode;
    MMIFILE_HANDLE file_handle = 0;
    char *filename_char = "E:\\Photos\\cittest.jpg";
    wchar filename[100] = {0};
    uint32 write_size = 0;

	if(!MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen (MMI_DEVICE_SDCARD)))
	{
		filename_char = "D:\\Photos\\cittest.jpg";
	}

    SCI_TRACE_LOW("lzk log. UnisocTestDcSaveDataToFile() enter.");

    if (data_buffer == NULL)
    {
        SCI_TRACE_LOW("lzk log. UnisocTestDcSaveDataToFile() error. data_buffer == NULL");
        return 0;
    }

    SCI_TRACE_LOW("lzk log. UnisocTestDcSaveDataToFile() enter. data_len = %d", data_len);

    if (data_len == 0)
    {
        SCI_TRACE_LOW("lzk log. UnisocTestDcSaveDataToFile() error. data_len == 0");
        return 0;
    }

    opt_mode = SFS_MODE_READ |SFS_MODE_WRITE |SFS_MODE_CREATE_ALWAYS; /*lint !e655*/

    MMIAPICOM_StrToWstr((char*)filename_char,(wchar *) filename);
    file_handle = MMIAPIFMM_CreateFile((wchar *)filename, 
                      opt_mode, 
                      PNULL, 
                      PNULL
                      );

    if (file_handle != PNULL)
    {
        MMIFILE_ERROR_E write_ret = 0;
        SCI_TRACE_LOW("lzk log. UnisocTestDcSaveDataToFile() call MMIAPIFMM_WriteFile().");
        write_ret = MMIAPIFMM_WriteFile(file_handle, data_buffer, data_len, &write_size, PNULL);
        SCI_TRACE_LOW("lzk log. UnisocTestDcSaveDataToFile() call MMIAPIFMM_WriteFile(). retval = %d.", write_ret);

        MMIAPIFMM_CloseFile(file_handle);
    }

    SCI_TRACE_LOW("lzk log. UnisocTestDcSaveDataToFile() exit.");

    return 1;
#endif
}


/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL uint32 UnisocTestDcCaptureCallBack(uint32 ret, uint16* data_buffer, uint32 data_len)
{
	uint32 result = 0;
	SCI_TRACE_LOW("UnisocTestDcCaptureCallBack");
	do
	{
		MMIFILE_ERROR_E file_ret = SFS_ERROR_SYSTEM;
		if((ret != 0) || (data_len == 0) || (PNULL == data_buffer) )
		{
		    result = 0xFF;
		    break;
		}
		
		file_ret = UnisocTestDcSaveDataToFile((uint8*)data_buffer, data_len);
		if (SFS_ERROR_NONE != file_ret)
		{
			result = 0xFF;
			break;
		}
		else
		{
			result = 0;
		}
		
		
	}while(0);
	return result;       
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN UnisocTestDcSnapShot(void)
{
	DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
	DCAMERA_SNAPSHOT_PARAM_T snapshot_param = {0};
	DCAMERA_SNAPSHOT_RETURN_PARAM_T snapshot_return = {0};
	GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	register uint16* buffer = PNULL;
	LCD_INFO_T lcd_info = {0};
	GUILCD_GetInfo(0, &lcd_info);
	
	SCI_TRACE_LOW("UnisocTestDcSnapShot");
	snapshot_param.data_type = DCAMERA_DATA_TYPE_JPEG;
	snapshot_param.buf_len = 0;
	snapshot_param.data_buf = PNULL;
	snapshot_param.file_name = PNULL;
	snapshot_param.snapshot_callback = UnisocTestDcCaptureCallBack;
	snapshot_param.rotation_mode = DCAMERA_ROTATION_90;
	snapshot_param.enc_quality_level = 6;
	snapshot_param.flash_enable	= 0;    
	snapshot_param.personal_snapshot.select_personal = 0;

	ret = DCAMERA_DoSnapshot(&snapshot_param, &snapshot_return);
	return (DCAMERA_OP_SUCCESS == ret);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestCameraWinHandleMsg(
									   MMI_WIN_ID_T      win_id, 
									   MMI_MESSAGE_ID_E  msg_id, 
									   DPARAM                param   
									   )
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	DCAMERA_RETURN_VALUE_E  ret = DCAMERA_OP_SUCCESS;
	GUI_LCD_DEV_INFO dev_info_ptr = {0};
	GUI_RECT_T       rect = {0};    
	MMI_STRING_T     text_str = {0};
	GUISTR_STYLE_T   text_style = {0};
	GUISTR_STATE_T   state = GUISTR_STATE_BOLD;
	static MMISRV_HANDLE_T audio_handle = 0;
	GUI_COLOR_T      color = MMI_BLACK_COLOR;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:    
	case MSG_GET_FOCUS:   
		
#ifdef JAVA_SUPPORT
		if (MMIAPIJAVA_IsJavaRuning())
		{
			MMIPUB_OpenAlertWarningWin( TXT_EXIT_BACKGROUND_JAVA);
			break;
		}
#endif
		
		//MMIDEFAULT_TurnOnBackLight();
		//MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		UnisocTestDcCreateOSDLayer(win_id);
		
#ifdef WIN32
		GUIBLOCK_SetType(0, color, GUIBLOCK_TYPE_OSD, &dev_info_ptr);
#else
		GUIBLOCK_SetType(255, color, GUIBLOCK_TYPE_COMMON, &dev_info_ptr);
#endif
		
		MMITHEME_SetUpdateDelayCount(0);
		
		// text style
		text_style.char_space = 1;
		text_style.line_space = 0;
		text_style.effect = FONT_EFFECT_NONE;
		text_style.font = SONG_FONT_16;
		text_style.font_color = MMI_GRAY_WHITE_COLOR;
		audio_handle = UnisocTestRequestVirtualAudService();
		ret = DCAMERA_Open(DCAMERA_OP_NORMAL);
		if(DCAMERA_OP_SUCCESS != ret)
		{
			rect = MMITHEME_GetFullScreenRect();
			GUI_FillRect(&dev_info_ptr, rect, MMI_WINDOW_BACKGROUND_COLOR);
			MMI_GetLabelTextByLang(TXT_DC_OPEN_FAIL , &text_str);
			rect.top = rect.bottom / 4;
			rect.left = (rect.right - GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, SONG_FONT_16, 1)) / 2;
			
			// display
			GUISTR_DrawTextToLCDInRect( 
				(const GUI_LCD_DEV_INFO *)&dev_info_ptr,
				(const GUI_RECT_T      *)&rect,       //the fixed display area
				(const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
				(const MMI_STRING_T    *)&text_str,
				&text_style,
				state,
				GUISTR_TEXT_DIR_AUTO
				);
			
			MMITHEME_UpdateRect();
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
			break;
		}
		DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_SET_TARGET_SIZE, DCAMERA_PHOTO_SIZE_1600X1200, PNULL);
		if(DCAMERA_OP_SUCCESS == UnisocTestDcPreview())
		{
			//GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_CAMERA_CAPTURE, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
		}
		else
		{
			//GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
		}
		break;
	case MSG_KEYDOWN_RED:
		break;
	case MSG_APP_WEB:
		break;
	case MSG_APP_LEFT:
		break;
	case MSG_APP_UP:
		break;
	case MSG_APP_RIGHT:
		break;
	case MSG_APP_DOWN:
		break;
	case MSG_LOSE_FOCUS:
	case MSG_CLOSE_WINDOW:
#ifndef WIN32
		DCAMERA_StopPreview();
#endif
		DCAMERA_Close();
		
		
		MMISRVMGR_Free(audio_handle);
		audio_handle = 0;
		//MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		
		rect = MMITHEME_GetFullScreenRect();
		GUIRES_DisplayImg(PNULL,
			&rect,
			&rect,
			win_id,
			IMAGE_COMMON_BG,
			&dev_info_ptr);
		UnisocTestDcReleaseOSDLayer();
		break;
		
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:

#if 1		
		GUIWIN_GetSoftkeyTextId (win_id, &s_unisoctest_softkey_id[0], &s_unisoctest_softkey_id[1], &s_unisoctest_softkey_id[2]);
		SCI_TRACE_LOW("lzk log. UnisocTestCameraWinHandleMsg() MSG_CTL_OK pressed");
		//if((MMI_TEXT_ID_T)TXT_NULL != s_unisoctest_softkey_id[0])
		{
			DCAMERA_StopPreview();
			UnisocTestDcSnapShot();
			SCI_SLEEP(2000);
			
			//MMIDEFAULT_AllowTurnOffBackLight(TRUE);
			SaveUnisocTestResult( win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin( win_id ) ;
		}
#else
        SCI_TRACE_LOW("lzk log. UnisocTestCameraWinHandleMsg() MSG_CTL_OK pressed.");
        DCAMERA_StopPreview();
        SCI_TRACE_LOW("lzk log. UnisocTestCameraWinHandleMsg() invoke UnisocTestDcSnapShot.");
        UnisocTestDcSnapShot();
        SCI_SLEEP(2000);
        SCI_TRACE_LOW("lzk log. UnisocTestCameraWinHandleMsg() invoke SaveUnisocTestResult.");	 
        SaveUnisocTestResult( win_id, 1);
        MMIAPIENG_RunUnisocTestNextStep();
        MMK_CloseWin( win_id ) ;		 
#endif
		break;
		
	case MSG_KEYDOWN_CANCEL :    
	case MSG_CTL_CANCEL:
		//DCAMERA_StopPreview();
		//DCAMERA_Close();
		
		//MMISRVMGR_Free(audio_handle);
		//audio_handle = 0;
		
		//MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		SaveUnisocTestResult( win_id, 0);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin( win_id ) ;
		break;
		
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN UnisocTestSetAnimParam (
								   BOOLEAN          is_update,
								   MMI_CTRL_ID_T    ctrl_id
								   )
{
	BOOLEAN                 result = FALSE;
	GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
	GUIANIM_CTRL_INFO_T     control_info = {0};
	GUIANIM_FILE_INFO_T     anim_info = {0};
	GUIANIM_DISPLAY_INFO_T  display_info = {0};
	
	wchar               *file_dev_ptr = PNULL;
	uint16              file_dev_len = 0;
	wchar               full_path_arr[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};
	uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
	
	
	//set anim min period
	GUIANIM_SetPeriod (MMITHEME_ANIM_MIN_PERIOD, ctrl_id);
	
	//set anim param
	control_info.is_ctrl_id = TRUE;
	control_info.ctrl_id    = ctrl_id;

	if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen (MMI_DEVICE_SDCARD)))
	{
		file_dev_ptr = MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);
		file_dev_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD);
	}
	else	if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen (MMI_DEVICE_UDISK)))
	{
		file_dev_ptr = MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK);
		file_dev_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK);
	}
	
	if((PNULL != file_dev_ptr) && (0 != file_dev_len))
	{
		if(MMIAPIFMM_CombineFullPath((wchar *)file_dev_ptr, file_dev_len,
			(wchar *)MMIMULTIM_DIR_PICTURE, (MMIMULTIM_DIR_PIC_LEN), 
			s_unisoctest_file_name, MMIAPICOM_Wstrlen(s_unisoctest_file_name), 
			(wchar *)full_path_arr, &full_path_len))
		{
			anim_info.full_path_wstr_ptr = full_path_arr;
			anim_info.full_path_wstr_len = full_path_len;
			display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
			display_info.is_update   = is_update;
			display_info.bg.bg_type  = GUI_BG_COLOR;
			display_info.bg.color    = MMI_BLACK_COLOR;
			
			anim_result = GUIANIM_SetParam (&control_info, PNULL, &anim_info, &display_info);
			
			
			if ( (GUIANIM_RESULT_SUCC == anim_result) ||
				(GUIANIM_RESULT_WAIT == anim_result))
			{
				result = TRUE;
			}
			
		}
	}
	return result;
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN UnisocTestDcDeleteCaptureImage ()
{
	BOOLEAN                 result = FALSE;
	wchar               *file_dev_ptr = PNULL;
	uint16              file_dev_len = 0;
	wchar               full_path_arr[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};
	uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
	
	
	
	if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen (MMI_DEVICE_SDCARD)))
	{
		file_dev_ptr = MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);
		file_dev_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD);
	}
	else	if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen (MMI_DEVICE_UDISK)))
	{
		file_dev_ptr = MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK);
		file_dev_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK);
	}
	
	if((PNULL != file_dev_ptr) && (0 != file_dev_len))
	{
		if(MMIAPIFMM_CombineFullPath((wchar *)file_dev_ptr, file_dev_len,
			(wchar *)MMIMULTIM_DIR_PICTURE, (MMIMULTIM_DIR_PIC_LEN), 
			s_unisoctest_file_name, MMIAPICOM_Wstrlen(s_unisoctest_file_name), 
			(wchar *)full_path_arr, &full_path_len))
		{
			result = MMIAPIFMM_DeleteFileSyn(full_path_arr, full_path_len);
			
		}
	}
	return result;
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestCameraCaptureWinHandleMsg (
													 MMI_WIN_ID_T       win_id,
													 MMI_MESSAGE_ID_E   msg_id,
													 DPARAM             param
													 )
{
	BOOLEAN                 is_next = FALSE;
	uint16                  item_index = 0;
	MMI_RESULT_E            result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T           ctrl_id = UNISOC_TEST_CAMERA_ANIM_CTRL_ID;
	GUI_RECT_T         both_rect = MMITHEME_GetClientRectEx(win_id);
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		GUIAPICTRL_SetRect (ctrl_id, &both_rect);
		if(UnisocTestSetAnimParam (FALSE, ctrl_id))
		{
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
		}
		else
		{
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
		}
		MMK_SetAtvCtrl (win_id, ctrl_id);
		break;
		
	case MSG_FULL_PAINT:
		break;   
	case MSG_NOTIFY_UPDATE:
		GUIANIM_UpdateRect (* (GUI_RECT_T *) param, ctrl_id);
		break;
		
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
		GUIWIN_GetSoftkeyTextId (win_id, &s_unisoctest_softkey_id[0], &s_unisoctest_softkey_id[1], &s_unisoctest_softkey_id[2]);
		if((MMI_TEXT_ID_T)TXT_NULL != s_unisoctest_softkey_id[0])
		{
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL:   
		SaveUnisocTestResult(win_id, 0);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);
		break;
		
	case MSG_KEYDOWN_RED:           
		break;   	
		
	case MSG_CLOSE_WINDOW:
		if (!UnisocTestDcDeleteCaptureImage())
		{
			SCI_TRACE_LOW("Delete image fail");
		}
		
		break;   
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

#ifdef BLUETOOTH_SUPPORT

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestBTWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;	
	uint8           *str_ptr = PNULL;
	MMI_STRING_T    text = {0};
	wchar           wstr[100] = {0};
	uint16          str_len = 0;
	BT_STATUS  ret_val = BT_SUCCESS;	
	static BOOLEAN bluetooth_on = FALSE;
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		//MMIDEFAULT_SetBackLight(TRUE);
		//MMIDEFAULT_SetLcdBackLight(TRUE);
		{
			GUI_RECT_T      lab_rect = {0};
			GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
			GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  
				IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
			lab_rect = client_rect;
			lab_rect.bottom = (client_rect.bottom + client_rect.top)/2;
			GUILABEL_SetRect(MMIENG_UITEST_TEXTBOX_CTRL_ID, &lab_rect, FALSE);
			
			lab_rect.top =  lab_rect.bottom + 1;
			lab_rect.bottom = client_rect.bottom ;
			GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
		}
		GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);

		bluetooth_on = MMIAPIBT_IsBtOn();
		if(MMIAPISET_GetFlyMode())
		{
			//str_ptr = (uint8*)"Fly Mode On, Fail key to next step!";
			MMI_GetLabelTextByLang(TXT_ENG_BTFLYMODE, &text );
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);            }
		else if (bluetooth_on)
		{
			//str_ptr = (uint8*)"Bluetooth has open!";
			MMI_GetLabelTextByLang(TXT_ENG_BT_HAS_OPEN, &text );
			ret_val = BT_CancelSearch();
			if(BT_PENDING != ret_val)//already canceled, search directly
			{
				ret_val = BT_SearchDevice(BT_SERVICE_ALL);
				if(BT_PENDING == ret_val)
				{
					//str_ptr = (uint8*)"Bluetooth search ...";
					MMI_GetLabelTextByLang(TXT_ENG_BTSEARCH, &text );
				}
				else
				{
					//str_ptr = (uint8*)"Bluetooth search fail";
					MMI_GetLabelTextByLang(TXT_ENG_BTSEARCH_FAIL, &text );
				}
			}
			
		}
		else
		{
			if(!BT_GetState())
			{
				ret_val = BT_Start();
				if(BT_PENDING == ret_val)
				{
					//str_ptr = (uint8*)"Open bluetooth, waiting ...";
					MMI_GetLabelTextByLang(TXT_ENG_BTOPEN_WAIT, &text );
				}
				else
				{
					//str_ptr = (uint8*)"Open bluetooth fail";
					MMI_GetLabelTextByLang(TXT_ENG_BTOPEN_FAIL, &text );
					
				}
			}else
			{
				//str_ptr = (uint8*)"Open bluetooth fail";
				MMI_GetLabelTextByLang(TXT_ENG_BTOPEN_FAIL, &text );
				
			}
		}
		//str_len = SCI_STRLEN((char*)str_ptr);
		//MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
		//text.wstr_ptr = wstr;
		//text.wstr_len = str_len;
		GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, FALSE);
		//MMK_SetAtvCtrl(win_id,MMIENG_UITEST_TEXTBOX_CTRL_ID);
		break;
	case MMI_BT_ME_ON_CNF:
		{
			BT_MSG_T     *msg_body = (BT_MSG_T*)param;
			SCI_TRACE_LOW("UnisocTestBTWinHandleMsg MMI_BT_ME_ON_CNF");
			if(BT_SUCCESS == msg_body->status)
			{
				//str_ptr = (uint8*)"Bluetooth is open!";
				MMI_GetLabelTextByLang(TXT_ENG_BT_IS_OPEN, &text );
				//str_len = SCI_STRLEN((char*)str_ptr);
				//MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
				//text.wstr_ptr = wstr;
				//text.wstr_len = str_len;
				GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
				ret_val = BT_CancelSearch();
				if(BT_PENDING != ret_val)//already canceled, search directly
				{
					ret_val = BT_SearchDevice(BT_SERVICE_ALL);
					
					if(BT_PENDING == ret_val)
					{
						//str_ptr = (uint8*)"Bluetooth search ...";
						MMI_GetLabelTextByLang(TXT_ENG_BTSEARCH, &text );
					}
					else
					{
						//str_ptr = (uint8*)"Bluetooth search fail";
						MMI_GetLabelTextByLang(TXT_ENG_BTSEARCH_FAIL, &text );
					}
				}
			}
			else
			{
				//str_ptr = (uint8*)"Bluetooth open fail";
				MMI_GetLabelTextByLang(TXT_ENG_BT_OPEN_FAIL, &text );
			}
			/*str_len = SCI_STRLEN((char*)str_ptr);
			MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
			text.wstr_ptr = wstr;
			text.wstr_len = str_len;*/
			GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
		}
		break;
		
	case MMI_BT_INQ_DEV_NAME:
		{
			BT_MSG_T     *msg_body = (BT_MSG_T*)param;
			uint8 addr_str[BT_ADDRESS_SIZE*4] = {0};
			wchar addr_wstr[BT_ADDRESS_SIZE*4] = {0};
			uint8 i = 0;
			uint16 addr_str_len = 0; 
			
			if(BT_SUCCESS == msg_body->status)
			{
				BT_DEVICE_INFO  dev_info   = *((BT_DEVICE_INFO*)(msg_body->body_ptr));
				MMI_STRING_T    text2 = {0};
				
				sprintf((char*)addr_str, "%x", dev_info.addr.addr[0]);
				for(i = 1; i < BT_ADDRESS_SIZE; i++)
				{
					addr_str_len = strlen((char*)addr_str);
					sprintf((char*)&addr_str[addr_str_len], ":%x", dev_info.addr.addr[i]);
				}
				addr_str_len = strlen((char*)addr_str);
				MMI_STRNTOWSTR( addr_wstr, BT_ADDRESS_SIZE*4, addr_str, addr_str_len, addr_str_len);
				
				text.wstr_ptr = dev_info.name;
				text.wstr_len = MMIAPICOM_Wstrlen(dev_info.name);
				GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
				
				text2.wstr_ptr = addr_wstr;
				text2.wstr_len = MMIAPICOM_Wstrlen(addr_wstr);
				GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text2, TRUE);
				
				GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			}
			else
			{
				/*str_ptr = (uint8*)"Bluetooth search fail";
				str_len = SCI_STRLEN((char*)str_ptr);
				MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
				text.wstr_ptr = wstr;
				text.wstr_len = str_len;*/
				MMI_GetLabelTextByLang(TXT_ENG_BTSEARCH_FAIL, &text );
				GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
				GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
			}
			ret_val = BT_CancelSearch();
		}
		break;
	case MMI_BT_INQ_COMP_CNF:
		MMI_GetLabelTextByLang(TXT_ENG_BTSEARCH_TIMEOUT, &text );
		/*str_ptr = (uint8*)"Bluetooth search timeout";
		str_len = SCI_STRLEN((char*)str_ptr);
		MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
		text.wstr_ptr = wstr;
		text.wstr_len = str_len;*/
		GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
		GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETEST, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		break;
	case MSG_FULL_PAINT:
	case MSG_KEYDOWN_UP:
	case MSG_KEYDOWN_DOWN:
	case MSG_KEYDOWN_LEFT:
	case MSG_KEYDOWN_RIGHT:
		break;
		
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
		GUIWIN_GetSoftkeyTextId (win_id, &s_unisoctest_softkey_id[0], &s_unisoctest_softkey_id[1], &s_unisoctest_softkey_id[2]);
		if(!MMIAPISET_GetFlyMode() && (MMI_TEXT_ID_T)TXT_NULL != s_unisoctest_softkey_id[0])
		{
			ret_val = BT_CancelSearch();
			if(FALSE == bluetooth_on)
			{
				if(BT_GetState())
				{
					ret_val = BT_Stop();
				}   
			}
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:        
	case MSG_CTL_CANCEL:
		ret_val = BT_CancelSearch();
		if(FALSE == bluetooth_on)
		{
			if(BT_GetState())
			{
				ret_val = BT_Stop();
			}   
		}
		SaveUnisocTestResult(win_id, 0);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(win_id);
		break;
		
	case MSG_KEYDOWN_WEB:
		GUIWIN_GetSoftkeyTextId (win_id, &s_unisoctest_softkey_id[0], &s_unisoctest_softkey_id[1], &s_unisoctest_softkey_id[2]);
		if(!MMIAPISET_GetFlyMode() && (MMI_TEXT_ID_T)TXT_NULL != s_unisoctest_softkey_id[1])
		{
			ret_val = BT_CancelSearch();
			if(BT_PENDING != ret_val)
			{
				ret_val = BT_SearchDevice(BT_SERVICE_ALL);
				if(BT_PENDING == ret_val)
				{
					//str_ptr = (uint8*)"Bluetooth search ...";
					MMI_GetLabelTextByLang(TXT_ENG_BTSEARCH, &text );
				}
				else
				{
					//str_ptr = (uint8*)"Bluetooth search fail";
					MMI_GetLabelTextByLang(TXT_ENG_BTSEARCH_FAIL, &text );
				}
			}
			
			/*str_len = SCI_STRLEN((char*)str_ptr);
			MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
			text.wstr_ptr = wstr;
			text.wstr_len = str_len;*/
			GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);//resolve bt reset cant update search
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		}
		break;

	case MSG_KEYDOWN_RED:
		break;
		
	case MSG_CLOSE_WINDOW:
		ret_val = BT_CancelSearch();
		if(FALSE == bluetooth_on)
		{
			if(BT_GetState())
			{
				ret_val = BT_Stop();
			}   
		}
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

#endif

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void UnisocTestSetLabelTextByCharArray(MMI_CTRL_ID_T ctrl_id,
                                   char    *buffer,
                                   uint32   buffer_size,   //must <50
                                   BOOLEAN  is_update)
{
    MMI_STRING_T    text ={0};
    uint32          str_length = 0;
    wchar           wstr[50] = {0};

    str_length  = SCI_STRLEN(buffer);
    SCI_MEMSET(wstr, 0,  buffer_size*sizeof(wchar));
    MMI_STRNTOWSTR(wstr, buffer_size, (uint8*)buffer, buffer_size, MIN(str_length,49));
    text.wstr_ptr = wstr ;
    text.wstr_len = MMIAPICOM_Wstrlen(text.wstr_ptr);
    GUILABEL_SetText( ctrl_id, &text, is_update);
}

/*****************************************************************************/
//  Description : auto seek Play FM
//  Global resource dependence : 
//  Author:
//  Note: 2019/6/10
/*****************************************************************************/
#ifdef FM_SUPPORT
PUBLIC BOOLEAN MMIFMENG_AutoSeekPlay(BOOLEAN isAutoSeek, BOOLEAN is_up, uint16 *cur_freq)
{
    uint32 ret = FALSE;
    FM_ALL_CHANNEL_INFO_T channel_info = {0};
    uint16  s_start_freq = *cur_freq;
    uint32  loc_cur_freq = 0;
    FM_SCAN_DIR_E dir = FM_SCAN_UP;

    if (!is_up)
    {
        dir = FM_SCAN_DOWN;
    }
    if(MMIFM_ReqPlaySrv())
    {
        MMIFM_EnableMute();
        do{
			if(dir == FM_SCAN_UP){
			    s_start_freq += 10;
		    }
		    else{
			    s_start_freq -= 10;
		    }
			if(!isAutoSeek)
			{
			   if(s_start_freq < MMIFM_FREQ_RANGE_LOW)
			   {
                   s_start_freq = MMIFM_FREQ_RANGE_HIGH;
			   }
			   else if(s_start_freq > MMIFM_FREQ_RANGE_HIGH)
			   {
                   s_start_freq = MMIFM_FREQ_RANGE_LOW;
			   }
			}
            ret = FM_AutoSeek(s_start_freq, dir, FM_SCAN_ONE, &channel_info);
			loc_cur_freq = channel_info.auto_freq_tab_ptr[0];
		} while((channel_info.auto_freq_tab_ptr[0] == 0) && ((s_start_freq >= MMIFM_FREQ_RANGE_LOW)&&(s_start_freq <= MMIFM_FREQ_RANGE_HIGH)));

        if(channel_info.auto_freq_tab_ptr[0])
		{
           MMIFM_DisableMute();
           MMISRVAUD_Play(MMIFM_GetPlayHandle(), 0);
		}
    }
    if(0 != loc_cur_freq)
	{
        *cur_freq = loc_cur_freq;
    }
	else
    {
        if(s_start_freq >= MMIFM_FREQ_RANGE_HIGH)
		{
            s_start_freq = MMIFM_FREQ_RANGE_HIGH;
	    }else if(s_start_freq <= MMIFM_FREQ_RANGE_LOW){
            s_start_freq = MMIFM_FREQ_RANGE_LOW;
		}
        *cur_freq = s_start_freq;
    }
    SCI_TRACE_LOW("[YYM] MMIFMENG_AutoSeekPlay channel_info.auto_freq_tab_ptr[0] = %d, *cur_freq=%d, ret = %d", channel_info.auto_freq_tab_ptr[0],*cur_freq, ret);
    return ret;
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E UnisocTestFMWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
	//uint8           *str_ptr = PNULL;
	GUI_RECT_T      client_rect = MMITHEME_GetClientRectEx(win_id);
	GUI_RECT_T      lab_rect = {0};
	BOOLEAN headset_is_plug = FALSE;
	MMI_STRING_T    text = {0};
	char str_channel[35] = {0};
    static uint16 cur_freq = 9900;
    uint32  init_result = FM_SUCCESS;
    SCI_TRACE_LOW("[YYM] UnisocTestFMWinHandleMsg  msg_id =  0x%x", msg_id);
	
#ifndef WIN32
#ifdef FM_S_ANT_SUPPORT
    //耳机无论插拔，根据状态进行长短天线和对应音频的切换
        headset_is_plug = MMIFM_CheckHeadsetStatusWithoutPrompt();
#else
		headset_is_plug = GPIO_CheckHeadsetStatus();
#endif
#endif
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		lab_rect = client_rect;
		lab_rect.bottom = (client_rect.bottom + client_rect.top)/2;
		GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, TRUE);
		
		lab_rect.top =  lab_rect.bottom + 1;
		lab_rect.bottom = client_rect.bottom ;
		GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
		if(!FM_IsInit())
        {
            init_result = FM_Init();
        }
		
		if (headset_is_plug && (!MMIAPISET_GetFlyMode()))
		{
			//MMIAPIFM_EngTestPlay();
			MMIFMENG_AutoSeekPlay(TRUE, TRUE,&cur_freq);
            SCI_MEMSET(str_channel, 0 ,  sizeof(str_channel));
            sprintf(str_channel, "%.1f", (float)cur_freq/100);
            UnisocTestSetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, str_channel, 35, TRUE);
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
		}
		break;
	case MSG_FULL_PAINT:
		GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id, 
			IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
		//str_ptr = (uint8*)"99.0MHz";
		//UnisocTestSetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, (char*)str_ptr, 35, FALSE);
		//GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);

		if(MMIAPISET_GetFlyMode())
		{
			//str_ptr = (uint8*)"Fly Mode On !";
			MMI_GetLabelTextByLang(TXT_ENG_FM_FLYMODEON, &text );
		}
#ifndef WIN32
		else if (GPIO_CheckHeadsetStatus())
		{
			//str_ptr = (uint8*)"Headset is in";
			MMI_GetLabelTextByLang(TXT_ENG_FM_HEADSETIN, &text );
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
		}
#endif
		else
		{
#ifdef FM_S_ANT_SUPPORT
			MMI_GetLabelTextByLang(TXT_ENG_HEADSETOUT, &text );
            GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
#else
			MMI_GetLabelTextByLang(TXT_ENG_FM_INSERT_HEADSET, &text );
			GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
#endif
			//str_ptr = (uint8*)"Insert Headset!";
			//MMI_GetLabelTextByLang(TXT_ENG_FM_INSERT_HEADSET, &text );
		}
		//UnisocTestSetLabelTextByCharArray(MMIENG_LABEL1_CTRL_ID, (char*)str_ptr, 35, FALSE);
		GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);
		break;

	case MSG_KEYDOWN_UP:
        if ((!MMIAPISET_GetFlyMode()) && headset_is_plug)
        {
            MMIFMENG_AutoSeekPlay(FALSE,TRUE,&cur_freq);
            SCI_MEMSET(str_channel, 0 ,  sizeof(str_channel));
            sprintf(str_channel, "%.1f", (float)cur_freq/100);
			SCI_TRACE_LOW("[YYM] UnisocTestFMWinHandleMsg  str_channel =  %s", str_channel);
            UnisocTestSetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, str_channel, 35, TRUE);
        }
        break;
    case MSG_KEYDOWN_DOWN:
        if ((!MMIAPISET_GetFlyMode()) && headset_is_plug)
        {
            MMIFMENG_AutoSeekPlay(FALSE,FALSE,&cur_freq);
            SCI_MEMSET(str_channel, 0 ,  sizeof(str_channel));
            sprintf(str_channel, "%.1f", (float)cur_freq/100);
			SCI_TRACE_LOW("[YYM] UnisocTestFMWinHandleMsg  str_channel =  %s", str_channel);
            UnisocTestSetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, str_channel, 35, TRUE);
        }
        break;
		
	case MSG_KEYUP_HEADSET_BUTTON:          
		break;
		
	case MSG_KEYDOWN_HEADSET_BUTTON:            
		break;
		
#ifdef FM_S_ANT_SUPPORT
    case MSG_KEYUP_HEADSET_DETECT:
	case MSG_KEYDOWN_HEADSET_DETECT:
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		MMIFM_CheckAndSwitchFmAnt(TRUE, msg_id);
		result = MMI_RESULT_FALSE;
		break;
#else		
	case MSG_KEYUP_HEADSET_DETECT:
        headset_is_plug = FALSE;
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
		
		MMIAPIFM_EngTestStop();
		UnisocTestSetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, NULL, 35, TRUE);
		result = MMI_RESULT_FALSE;
		break;
		
	case MSG_KEYDOWN_HEADSET_DETECT:
		headset_is_plug = TRUE;
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
        MMIFMENG_AutoSeekPlay(TRUE, TRUE,&cur_freq);
        SCI_MEMSET(str_channel, 0 ,  sizeof(str_channel));
        sprintf(str_channel, "%.1f", (float)cur_freq/100);
        UnisocTestSetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, str_channel, 35, TRUE);
		result = MMI_RESULT_FALSE;
		break;
#endif
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
		GUIWIN_GetSoftkeyTextId (win_id, &s_unisoctest_softkey_id[0], &s_unisoctest_softkey_id[1], &s_unisoctest_softkey_id[2]);
		if((MMI_TEXT_ID_T)TXT_NULL != s_unisoctest_softkey_id[0] && headset_is_plug)
		{
			MMIAPIFM_EngTestStop();           
			SaveUnisocTestResult(win_id, 1);
			MMIAPIENG_RunUnisocTestNextStep();
			MMK_CloseWin(win_id);
		}
		break;
		
	case MSG_KEYDOWN_CANCEL:          
	case MSG_CTL_CANCEL:            
		MMIAPIFM_EngTestStop();           
		SaveUnisocTestResult(win_id, 0);
		MMIAPIENG_RunUnisocTestNextStep();
		MMK_CloseWin(  win_id) ; 
		break;      
		
	case MSG_KEYDOWN_RED:
		break;
		
	case MSG_LOSE_FOCUS:
		break;
	case MSG_CLOSE_WINDOW:        
		MMIAPIFM_EngTestStop();  
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}
#endif
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_RunUnisocTestNextStep(void)
{    
	if(0 != s_is_fulltest_unisoc)
	{
		if(TRUE == s_test_fail)
		{
		    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		    MMK_ReturnIdleWin();
		    MMIPUB_OpenAlertFailWin(TXT_ENG_FAIL);
		    return FALSE;
		}

		if (PNULL != s_unisoc_test_win_ptr_tab[s_unisoc_test_win_tab_index])
		{ 
			MMK_CreateWin((uint32 *)s_unisoc_test_win_ptr_tab[s_unisoc_test_win_tab_index ++], PNULL);
		}
		else
		{
			s_unisoc_test_win_tab_index = 0;        
			WriteUnisocTestResultNV(&s_is_tested_unisoc, &s_test_result_unisoc);  
			SetMMIUnisocTestResultBit(&s_is_tested_unisoc, &s_test_result_unisoc);
			//MMK_CreateWin((uint32*)MMIENG_UITEST_RESULT_WIN_TAB,  PNULL);
		}
	}
	return TRUE;
}

LOCAL MMI_RESULT_E HandleResetOrCleanDataWaitWinUNISOC(
                                                 MMI_WIN_ID_T    win_id, // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                 DPARAM             param   // 相应消息的参数
                                                 )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    static uint32   s_waitting_num = 0;
    static BOOLEAN is_cleandata =FALSE;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_waitting_num = 0;
        //recode  =   MMI_RESULT_FALSE;
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_KEYDOWN_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_APP_RED:
        break;
    case MSG_SET_RESET_NEED_WAIT_IND:
        //add one wait action
        s_waitting_num++;
        break;
    case MSG_SET_RESET_FACTORY_OVER_IND:
        if(s_waitting_num > 0)
        {
            //finished one, wait number -1
            s_waitting_num --;
        }
        //no one to wait, finished
        if(s_waitting_num == 0)
        {
            if(is_cleandata)
            {
                //MMK_CloseWin(win_id);
                MMIAPIPHONE_PowerReset();
            }
            else
            {
                //MMK_CloseWin(win_id);
                MMK_ReturnIdleWin(); //crMS00187558 @murphy.xie
                //success!
#ifndef FLASH_SUPPORT
         //       MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePromptCompleteThemeWin);
#endif
            }
        }
        break;

    case MSG_SET_CLEAN_USER_DATA_OVER_IND:
        is_cleandata = TRUE;
        break;

    case MSG_CLOSE_WINDOW:
        //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_RESET_FACT, TRUE);
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_ClearAllDevice();
#endif
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return recode;
}
LOCAL MMI_RESULT_E handleConfirmationDialogUNISOC(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;
    MMI_WIN_ID_T                wait_win_id             =   0;
    uint32                      is_delete_all_data      =   FALSE;
    uint32                      time_out                =   0;

    is_delete_all_data = (uint32)(MMK_GetWinAddDataPtr(win_id));

    switch (msg_id)
    {
    //zhuyongjing add
    case MSG_OPEN_WINDOW:
	//result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
	#if  0
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, RIGHT_BUTTON, TRUE);
#endif
		GUIWIN_SeSoftkeytButtonTextId(win_id, IMAGE_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_ENG_OK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_CANCEL, RIGHT_BUTTON, TRUE);
		
	//  MMK_PostMsg(win_id, MSG_APP_WEB, PNULL,PNULL);
    break;
    case MSG_SET_CLEAN_DATE_IND:
            MMISET_CleanUserData();
            MMISET_ResetFactorySetting();
            MMK_CloseWin(win_id);
        break;
    //add end
    case MSG_APP_WEB:
    case MSG_APP_OK:
        MMIPUB_CloseQuerytWin(PNULL);
		//进入等待界面
        wait_win_id = MMISET_RESET_FACTORY_WAITING_WIN_ID;
        MMIPUB_OpenAlertWinByTextId(&time_out,TXT_ENG_RESET,TXT_NULL,IMAGE_PUBWIN_WAIT,
            &wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,
            HandleResetOrCleanDataWaitWinUNISOC);

        MMK_PostMsg(win_id, MSG_SET_CLEAN_DATE_IND, PNULL,PNULL);
        break;

    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    default:
        break;
    }

   result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
    return (result);
}
PUBLIC BOOLEAN MMIAPIENG_RunUnisocTestRESET(void)
{
      MMI_STRING_T textin = {0};
      MMI_CTRL_ID_T  ctrl_id = MMISET_INPUT_RESET_FACTORY_PWD_CTRL_ID;

	MMI_WIN_PRIORITY_E       win_priority = WIN_THREE_LEVEL;

	MMI_GetLabelTextByLang(TXT_ENG_RESET, &textin );
	//MMIPUB_OpenConfirmationDialogByTextId(PNULL,(MMI_TEXT_ID_T)TXT_ENG_RESET,PNULL,PNULL,&ctrl_id,PNULL,MMIPUB_SOFTKEY_NONE,handleConfirmationDialogUNISOC);
	MMIPUB_OpenConfirmationDialogByTextId(PNULL,(MMI_TEXT_ID_T)TXT_ENG_RESET,PNULL,PNULL,&ctrl_id,&win_priority,MMIPUB_SOFTKEY_DIALOG_OK,handleConfirmationDialogUNISOC);
	return TRUE;
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL int32 ReadUnisocTestResultNV( uint32* is_tested, uint32* result  )
{
	uint8 nvitem[64] = {0}; 
	uint32 nv_length = 64;
	uint32 return_code = 0 ;
#ifndef WIN32    
	return_code = PROD_ReadEngModeTestResult((void*)nvitem, nv_length);
#endif    
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8134_112_2_18_2_17_40_110,(uint8*)"d", return_code);
	if(return_code == 0)   
	{        
		*is_tested = *(uint32*)&nvitem[0] ;
		*result = *(uint32*)(&nvitem[0] + 8) ;        
		//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8139_112_2_18_2_17_40_111,(uint8*)"dd", *is_tested, *result);
	}
	else
	{
		SCI_MEMSET(nvitem, 0, nv_length);
#ifndef WIN32    
		return_code = PROD_WriteEngModeTestResult((void*)nvitem, nv_length);
#endif    
	}    
	return return_code;
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL int32 WriteUnisocTestResultNV(uint32*  is_tested ,  uint32* result )
{       
	uint8 nvitem[64] ={ 0}; 
	uint32 nv_length = 64;
	uint32 return_code = 0 ;    
	uint32 temp_is_tested = 0;
	uint32 temp_result = 0;

#if 0    
#ifndef WIN32    
	return_code = ReadUnisocTestResultNV(&temp_is_tested, &temp_result);   
#endif    
	if((temp_is_tested != *is_tested) || (temp_result != *result))
	{        
		*(uint32*)&nvitem[0]  = *is_tested ;
		*(uint32*)(&nvitem[0] + 8) = *result ;
#ifndef WIN32    
		return_code = PROD_WriteEngModeTestResult((void*)nvitem, nv_length);  
#endif
		//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8176_112_2_18_2_17_40_112,(uint8*)"d", return_code);
		if(SCI_SUCCESS !=  return_code )
		{
			SCI_MEMSET(nvitem, 0, nv_length);
#ifndef WIN32    
			return_code = PROD_WriteEngModeTestResult((void*)nvitem, nv_length);
#endif 
		}
		else
		{
			//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8186_112_2_18_2_17_40_113,(uint8*)"dd", *is_tested, *result);
		}
	}
	else
	{
		//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8191_112_2_18_2_17_40_114,(uint8*)"dd", *is_tested, *result);
	}
#endif    
	return return_code ; 
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL int32 SetUnisocTestResult(uint32 win_id, uint32 result)
{
	uint32 i =0 ;
	
	for( i=0; i < (uint32)( ARR_SIZE(s_all_unisoc_test_win_id)); i++ )
	{
		if(*(s_all_unisoc_test_win_id + i) == win_id)
		{
			s_is_tested_unisoc |=  (0x00000001 << i);  //if tested
			
			result &=  0x000001 ;           
			s_test_result_unisoc &=  ~(0x00000001 << i);   /*lint !e502*/ //clear the bit 
			s_test_result_unisoc |= (result << i);         //set the bit
			break;
		}
		
	}   
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8220_112_2_18_2_17_40_115,(uint8*)"dd", i, result);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8221_112_2_18_2_17_40_116,(uint8*)"dd", s_is_tested, s_test_result);
	//if( 0 == result) s_test_fail = TRUE;   //quit if fail 
	return i;
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL int32 SaveUnisocTestResult(uint32 win_id, uint32 result)
{
	int32 return_error = 0;
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8236_112_2_18_2_17_40_117,(uint8*)"dd", s_is_fulltest, result);
	//if( 0 != s_is_fulltest_unisoc )    //full test
	{
		return SetUnisocTestResult(win_id, result);       
	}
#if 0    
	else
	{
		//get test result
		return_error = ReadUnisocTestResultNV(&s_is_tested_unisoc, &s_test_result_unisoc);
		
		//modify the cuurent test result
		SetUnisocTestResult(win_id, result) ;        
		
		//save to nv
		WriteUnisocTestResultNV(&s_is_tested_unisoc, &s_test_result_unisoc);
		
		//set adc7_reserved MMI SET BIT
		SetMMIUnisocTestResultBit(&s_is_tested_unisoc, &s_test_result_unisoc);
		return return_error;
	}
#endif    
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL uint32  SetADC7MMIUnisocTestBit(uint32 result)
{
    
    uint32 adc_reserved_info = 0;
    uint32 return_code = 0 ;
    //从NV中读取adc_reserved[7]的信息
#ifdef WIN32    
    adc_reserved_info = 0xc3fe0155;
#else
    adc_reserved_info = REFPARAM_GetCaliMarkerValue();
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8326_112_2_18_2_17_40_121,(uint8*)"d",adc_reserved_info);
#endif
	
    if ((result & 0x01) == ((adc_reserved_info >> 27) & 0x01))
    {
        return 1;
    }
    
    adc_reserved_info &= ~ (0x00000001 << 27) ;
    adc_reserved_info |=  (result&0x01) << 27;
    return_code =  REFPARAM_SetCaliMarkerValue(adc_reserved_info) ; 
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8332_112_2_18_2_17_40_122,(uint8*)"dd", result, return_code);
    return return_code;
}   

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL int32 SetMMIUnisocTestResultBit(uint32* is_tested, uint32* result)
{
	uint32 i= 0;
	uint32 j= 0;
	
	for(i = 0; i < ARR_SIZE(s_unisoc_test_win_id) ; i++)
	{
		for( j=0; j < ARR_SIZE(s_all_unisoc_test_win_id); j++)
		{
			if( *(s_unisoc_test_win_id + i) == *(s_all_unisoc_test_win_id + j))
				//check
			{
				if( ((*is_tested >> j) & 0x00000001 ) == 1)
				{
					if( ((*result >> j) & 0x00000001 ) == 1)
					{
						continue;
					}
					else
					{
						//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8292_112_2_18_2_17_40_118,(uint8*)"d", j);
						return SetADC7MMIUnisocTestBit(0);        
						
					}
				}
				else
				{
					//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8299_112_2_18_2_17_40_119,(uint8*)"d", j);
					return SetADC7MMIUnisocTestBit(0);
				}
			}
		}
	}
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8305_112_2_18_2_17_40_120,(uint8*)"d", j);
	return SetADC7MMIUnisocTestBit(1) ;
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_OpenUnisocTestWin(void)
{
    MMK_CreateWin((uint32 *)(MMIENG_UNISOCTEST_MENU_WIN_TAB), PNULL);
    return TRUE;
}

PUBLIC BOOLEAN MMIENG_OpenAgingTestWin(void)
{
    MMK_CreateWin((uint32 *)(MMIENG_AGINGTEST_MENU_WIN_TAB), PNULL);
    return TRUE;
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void MMIENG_RunUnisocTestInit(void)
{
    MMI_STRING_T        s_unisoctest_prompt_str = {0};
    MMI_GetLabelTextByLang(TXT_ENG_PASS_FAIL_RETEST, &s_unisoctest_prompt_str );
   // s_unisoctest_prompt_str.wstr_ptr = s_unisoctest_prompt_wstr;
   // s_unisoctest_prompt_str.wstr_len = MMIAPICOM_Wstrlen(s_unisoctest_prompt_wstr);
    s_unisoc_test_win_tab_index = 0;
    s_test_fail = FALSE;
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StartUnisocTest(void)
{
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    MMIAPIMP3_ResetAllMp3();
#endif
    MMIAPIFM_Exit();
	
#ifdef KURO_SUPPORT
    MMIAPIKUR_ResetAllKur();
#endif
	
    MMIENG_RunUnisocTestInit();
	
    MMIAPIENG_RunUnisocTestNextStep();
	
    return TRUE;
}


PUBLIC BOOLEAN MMIEng_IsOpenUNITEST(void)
{

	if (MMK_IsOpenWin(MMIENG_UNISOCTEST_MENU_WIN_ID))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

#if defined(ZDT_SHOW_SIM_NETWORK_RSSI_IN_FACTORY_MODE)
PUBLIC BOOLEAN MMIENG_OpenUnisocSimnetworkTestWin(void)
{
    MMK_CreateWin((uint32 *)(MMIENG_UNISOCTEST_SIM_NETWORK_WIN_TAB), PNULL);
    return TRUE;
}
#endif
#endif/*MMI_UNISOC_MANUALLY_CIT_SUPPORT*/



