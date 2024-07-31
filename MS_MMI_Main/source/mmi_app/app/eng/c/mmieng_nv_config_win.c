/*****************************************************************************
** File Name:      mmieng_nv_config_win.c                                              *
** Author:        mingwen.ge                                                          *
** Date:           06/2015                                                   *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe nv config test                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 6/2015         GMW

******************************************************************************/


#define _MMIENG_NV_CONFIG_WIN_C_

/**--------------------------------------------------------------------------*
                                Include Files
 **--------------------------------------------------------------------------*/
//this line must be at the fisrt line of include sentences now
#include "mmi_app_eng_trc.h"

#include "mmk_app.h"
#include "mmk_type.h"
#include "mmk_timer.h"
#include "mmipub.h"
#include "window_parse.h"

#include "guilabel.h"

#include "mmieng_internal.h"
#include "mmieng_win.h"

#include "mmieng_id.h"
#include "mmieng_text.h"

//#include "mmi_appmsg.h"

#include "mmieng_text.h"
#include "mmiconnection_export.h"

#include "mmi_modu_main.h"

#include "nvitem.h"

#include "variant_config_nv.h"

/**--------------------------------------------------------------------------*
                                     MACRO DEFINITION
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
                                     STATIC DEFINITION
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
                                     TYPE AND CONSTANT
 **--------------------------------------------------------------------------*/

uint16 base_id = VARIANT_NV_CONFIG_USER_BASE;

//digit 2 prompt dial
BOOLEAN dial2_prompt;

//life timer enable/disable.enable:1 disable:0
BOOLEAN life_timer;

//show call last time.
//BOOLEAN show_call_time;

//show incoming call number.
/*
typedef struct
{
       //BOOLEAN show_incoming_call_number;
	//BOOLEAN auto_answer;
	BOOLEAN auto_redial;
	BOOLEAN reserved[3];
}CALL_CFG;*/
BOOLEAN call_auto_redial;

//Specify how how many digits should be compared for calls
BOOLEAN cli_number;

//Enable/disable speed dial feature. 0:disable 1:enable.
BOOLEAN speed_dial;

//Fast Dormancy.
BOOLEAN Fast_dormancy;

//First Show PNN/SPN/ONS
enum{
    FIRST_SHOW_PNN=0,
    FIRST_SHOW_SPN,
    FIRST_SHOW_ONS
};
BOOLEAN operator_show_order;

//Show PNN instead of SPN (MVNO solution.
//BOOLEAN show_pnn_instead_spn;

//Large font for dialinglarge dialing font: 1 normal dialing font: 0
//BOOLEAN large_dialing_font;

//set cphs on/off
BOOLEAN set_cphs_on;

//show clear code.
/*
typedef struct _NDT_TELE_CFG_T{
uint8 show_clear_code; //0:default
} _NDT_TELE_CFG_T;
*/
//
#ifdef MMI_ETWS_SUPPORT
uint16 cell_broadcast_config;
#else
BOOLEAN cell_broadcast;
#endif

//cell broadcast channel.
uint16 cell_broadcast_channel[5];

//call time summary.
//BOOLEAN calltime_summary;

//fill bit vamos
BOOLEAN fill_bit_vamos;

//reserved item,use to add nv item
BOOLEAN tele_reserved_item;

/*NV_VARIANT_CONFIG_MESSAGES item*/

//Config msg by user.
//BOOLEAN config_msg_by_user;

//SMS delivery report enable/disable.
BOOLEAN sms_delivery_report;

//sms encoding and other config.
/*typedef struct _NDT_SMS_CFG_T{
    BOOLEAN encoding_support;
    //uint8 SMS_encoding_Table;
    BOOLEAN reserved[3];
}_NDT_SMS_CFG_T;*/
BOOLEAN sms_encoding_support;

//mms config parameters.
typedef enum _NDT_MMS_MAXSIZE_E{
    MMS_100K,
    MMS_300K,
    MMS_600K
}_NDT_MMS_MAXSIZE_E;
typedef struct _NDT_MMS_CONFIG_T{
    uint16 max_number;
    uint8 max_body_size;
    uint8 mms_auto_receive;
}_NDT_MMS_CONFIG_T;

//mms timestamp type from local or network.
typedef enum _NDT_MMS_STAMP_SRC_E{
    TIME_STAMP_FROM_LOCAL = 0,
    TIME_STAMP_FROM_NET
}_NDT_MMS_STAMP_SRC_E;
enum _NDT_MMS_STAMP_SRC_E mms_stamp_src;

//MMS advertise on/off
BOOLEAN advertise_on;

// full sms char support.
//BOOLEAN  full_sms_char_support;

//large message font
//BOOLEAN large_message_font;

//MMS delivery report enable/disable.
BOOLEAN mms_delivery_report;

//Message Reserved item,use to add nv item.
//BOOLEAN message_reserved_item;

/*NV_VARIANT_CONFIG_APP nv items*/

//bluetooth config
/*typedef enum _NDT_BT_STORAGE_E{
    BLUETOOTH_FILE_STORAGE_TO_PHONE = 0,
    BLUETOOTH_FILE_STORAGE_TO_TCARD
}_NDT_BT_STORAGE_E;
typedef struct _NDT_BT_CONFIG_T{
	BOOLEAN is_visible;
	//uint8 name[20];
	//_NDT_BT_STORAGE_E storage_src;
	uint8 reserved[3];
}_NDT_BT_CONFIG_T;*/
BOOLEAN bt_is_visible;

//shot sound/silent
BOOLEAN camera_shot_sound;

//camera record indicator light on.
BOOLEAN camera_record_indicator_light;

//emergency call ui
BOOLEAN emergency_call_ui;

//fmradio record config.
typedef struct _NDT_RECORD_CFG_T{
    BOOLEAN recorder_enable;
    BOOLEAN recorder_sharing;
    BOOLEAN reserved[2];
}NDT_RECORD_CFG_T;

//fm tunning step selection.
typedef enum _NDT_FM_TUNNING_STEP_E{
    FM_TUNNING_MIN_STEP=100,
    FM_TUNNING_MID_STEP=300,
    FM_TUNNING_MAX_STEP=500
}_NDT_FM_TUNNING_STEP_E;

uint16 fm_turnning_step;

//default browser.
BOOLEAN default_browser;

//App reserved item,use to add nv item.
//BOOLEAN app_reserved_nv_item;

/*NV_VARIANT_CONFIG_SETTINGS nv items*/

//Enable/Disable T9 input method.
BOOLEAN input_t9_on;

//time format
typedef enum _NDT_TIME_FORMAT_E
{
    TIME_FORMAT_24H,
    TIME_FROMAT_12H
}_NDT_TIME_FORMAT_E;

uint32 time_format_type;

//NITZ time on/off
uint32 time_nitz_on;

//date format type.
uint32 date_format_type;
//ndt version
typedef struct{
uint8 lp[4];
uint8 pcode[12];
uint8 ndt_version[12];
uint8 res_version[12];
}_NDT_VERSION_T;

// show sar
BOOLEAN show_sar;

//sim ecc.
uint8 sim_ecc[10][32];

//no sim ecc.
uint8 no_sim_ecc[15][32];

//voice mail
uint8 voice_mail[32];

//homepage
uint8 homepage[128];

//bookmark
typedef struct{
uint8 name[10][16];
uint8 url[10][128];
}_NDT_BOOKMARK_T;

//default language
uint8 default_lang;

//ciphering a5
uint8 ciphering_a5;

//menu-tree config and app list order arrangement.
typedef struct _NDT_MENU_TREE_T{
    uint32 Total_App;
    uint32 APPID_LIST[50];
}_NDT_MENU_TREE_T;

/**--------------------------------------------------------------------------*
                                     LOCAL FUNCTION DECLARE
 **--------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E HandleConfigNVTestWinMsg(
                                                 MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                 DPARAM             param       // 相应消息的参数
                                                 );


/**--------------------------------------------------------------------------*
                                     GLOBAL DEFINITION
 **--------------------------------------------------------------------------*/

WINDOW_TABLE(MMIENG_CONFIG_NV_TEST_WIN_TAB) =
{
        //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32)HandleConfigNVTestWinMsg),
        WIN_ID(MMIENG_CONFIG_NV_TEST_WIN_ID),
              WIN_TITLE(TXT_ENG_NVCOUNT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_TEXT_CTRL( MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID),
        END_WIN
};


/**--------------------------------------------------------------------------*
                                     EXTERNAL DECLARE
 **--------------------------------------------------------------------------*/


/*===============================================================================
                                IMPLEMENTATION
===============================================================================*/

/********************************************************************************
 NAME:          MMIENGWIN_OpenConfigNVTestWin
 DESCRIPTION:
 PARAM IN:
 PARAM OUT:
 AUTHOR:        mingwen.ge
 DATE:      0   2015.05.12
********************************************************************************/
PUBLIC BOOLEAN MMIENGWIN_OpenConfigNVTestWin(void)
{
     MMK_CreateWin((uint32*)MMIENG_CONFIG_NV_TEST_WIN_TAB, PNULL);
     return TRUE;
}

/********************************************************************************
 NAME:          getConfigNVAndDisplay
 DESCRIPTION:  get config nv item status and display it
 PARAM IN:
 PARAM OUT:
 AUTHOR:        mingwen.ge
 DATE:      0   2015.05.12
********************************************************************************/

void getConfigNVAndDisplay( MMI_WIN_ID_T    win_id,
                                     uint16 item_id, BOOLEAN get_status)
{
    NVITEM_ERROR_E nv_reslut = NVERR_NONE;
	wchar wstr[256] = {0};
	char str[256] = {0};
	uint32 buffer_length = 255;
	uint16 str_length = 0;
	uint16 nv_len = 0;
	SCI_MEMSET(str, 0 ,buffer_length + 1) ;

	SCI_TRACE_LOW("gmw---getConfigNVAndDisplay:item_id=%d", item_id);

	nv_len = NVITEM_GetLength(item_id);
	nv_reslut = EFS_NvitemRead(item_id,(uint16)sizeof(get_status),(uint8*)&get_status);

	if (NVERR_NONE == nv_reslut)
	{
		sprintf(str, "NV item id: %d \nNV item value: %d \n", item_id, get_status);
	}
	else
	{
		sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", item_id, nv_reslut);
		SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", item_id, nv_reslut);
	}

	str_length = SCI_STRLEN(str);

	MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

	GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,  wstr,	str_length, FALSE);
	MMK_SetAtvCtrl(win_id,	MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);

}

/********************************************************************************
 NAME:          getConfigNVAndDisplayEx
 DESCRIPTION:  get config nv item value and display it
 PARAM IN:
 PARAM OUT:
 AUTHOR:        mingwen.ge
 DATE:      0   2015.05.12
********************************************************************************/

void getConfigNVAndDisplayEx( MMI_WIN_ID_T    win_id,
                                     uint16 item_id, void* get_status)
{
		NVITEM_ERROR_E nv_reslut = NVERR_NONE;
		wchar wstr[256] = {0};
		char str[256] = {0};
		uint32 buffer_length = 255;
		uint16 str_length = 0;
		uint16 nv_len = 0;
		SCI_MEMSET(str, 0 ,buffer_length + 1) ;

		SCI_TRACE_LOW("gmw---getConfigNVAndDisplay:item_id=%d", item_id);

		nv_len = NVITEM_GetLength(item_id);
		nv_reslut = EFS_NvitemRead(item_id, nv_len, (uint8*)get_status);

		if (NVERR_NONE == nv_reslut)
		{
			sprintf(str, "NV item id: %d \nNV item value: %d \n", item_id, * (uint16 *)get_status);
		}
		else
		{
			sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", item_id, nv_reslut);
			SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", item_id, nv_reslut);
		}

		str_length = SCI_STRLEN(str);
		MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

		GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,  wstr,	str_length, FALSE);
		MMK_SetAtvCtrl(win_id,	MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);

}

/********************************************************************************
 NAME:          getConfigNVNotImplement
 DESCRIPTION:  use to prompt not implement nv config item
 PARAM IN:
 PARAM OUT:
 AUTHOR:        mingwen.ge
 DATE:      0   2015.05.12
********************************************************************************/

void getConfigNVNotImplement( MMI_WIN_ID_T    win_id, uint16 item_id)
{
    NVITEM_ERROR_E nv_reslut = NVERR_NONE;
	wchar wstr[100] = {0};
	char str[100] = {0};
	uint32 buffer_length = 99;
	uint32 str_length = 0;
	SCI_MEMSET(str, 0 ,buffer_length + 1) ;

	sprintf(str, "NV item id: %d \nNV item not implement \n", item_id);

	str_length = SCI_STRLEN(str);

	MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

	GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,  wstr,	str_length, FALSE);
	MMK_SetAtvCtrl(win_id,	MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);

}

/********************************************************************************
 NAME:          ConfigNvDisplay
 DESCRIPTION:  all config nv items display function
 PARAM IN:
 PARAM OUT:
 AUTHOR:        mingwen.ge
 DATE:      0   2015.05.12
********************************************************************************/

void ConfigNvDisplay(MMI_WIN_ID_T    win_id)
{
    NVITEM_ERROR_E nv_reslut = NVERR_NONE;
    wchar wstr[256] = {0};
    char str[256] = {0};
    uint32 buffer_length = 255;
    uint16 str_length = 0;

	switch (base_id)
	{
		case TELE_NV_DIAL2_PROMPT:
		{
			dial2_prompt = SCI_FALSE;
			getConfigNVAndDisplay(win_id, TELE_NV_DIAL2_PROMPT, dial2_prompt);
		}
		break;

		case TELE_NV_LIFE_TIMER:
		{
			life_timer = SCI_FALSE;
			getConfigNVAndDisplay(win_id, TELE_NV_LIFE_TIMER, life_timer);
		}
		break;

		//case TELE_NV_SHOW_CALL_TIME:
		//{
		//	show_call_time = SCI_FALSE;
		//	getConfigNVAndDisplay(win_id, TELE_NV_SHOW_CALL_TIME, show_call_time);
		//}
		//break;

		case TELE_NV_CALL_CFG:
		{
			/*
			CALL_CFG call_config;
			uint16 nv_len = 0;
			SCI_MEMSET(str, 0 ,buffer_length + 1) ;

			//call_config.show_incoming_call_number = SCI_FALSE;
			//call_config.auto_answer = SCI_FALSE;
			call_config.auto_redial= SCI_FALSE;

			nv_len = NVITEM_GetLength(base_id);
			nv_reslut = EFS_NvitemRead(TELE_NV_CALL_CFG, nv_len, (uint8*)&call_config);
			//SCI_TRACE_LOW("gmw---nv:show_incoming_call_number=%d auto_answer=%d auto_redial=%d",call_config.show_incoming_call_number, call_config.auto_answer, call_config.auto_redial);

			if (NVERR_NONE == nv_reslut)
			{
				//sprintf(str, "NV item id: %d \nNV item value \nshow incoming call : %d \nauto_answer: %d \nauto_redail: %d \n", \
				//	TELE_NV_CALL_CFG, call_config.show_incoming_call_number, \
				//	call_config.auto_answer, call_config.auto_redial);
				sprintf(str, "NV item id: %d \nNV item value \nauto_redail: %d \n", \
					TELE_NV_CALL_CFG, call_config.auto_redial);
			}
			else
			{
				sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", base_id, nv_reslut);
				SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", base_id, nv_reslut);
			}

			str_length = SCI_STRLEN(str);
			MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

			GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,  wstr,	str_length, FALSE);
			MMK_SetAtvCtrl(win_id,	MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);
			*/
			call_auto_redial = SCI_FALSE;
			getConfigNVAndDisplay(win_id, base_id, call_auto_redial);
		}
		break;

	   case TELE_NV_CLI_NUMBER:
	   {
		   cli_number = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, TELE_NV_CLI_NUMBER, cli_number);
	   }
	   break;

	   case TELE_NV_ENABLE_SPEED_DIAL:
	   {
		   speed_dial = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, TELE_NV_ENABLE_SPEED_DIAL, speed_dial);
	   }
	   break;

	   case TELE_NV_FAST_DORMANCY:
	   {
		   Fast_dormancy = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, TELE_NV_FAST_DORMANCY, Fast_dormancy);
	   }
	   break;

	   case TELE_NV_OPERATOR_SHOW_ORDER:
	   {
		   operator_show_order = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, TELE_NV_OPERATOR_SHOW_ORDER, operator_show_order);
	   }
	   break;
         /*
	   case TELE_NV_MNVO_SHOW_PNN:
	   {
		   show_pnn_instead_spn = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, TELE_NV_MNVO_SHOW_PNN, show_pnn_instead_spn);
	   }
	   break;
	   */
	   //case TELE_NV_LARGE_DIALING_FONT:
	   //{
	//	   large_dialing_font = SCI_FALSE;
	//	   getConfigNVAndDisplay(win_id, TELE_NV_LARGE_DIALING_FONT, large_dialing_font);
	   //}
	   break;

	   case TELE_NV_SET_CPHS_ON:
	   {
		   set_cphs_on = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, TELE_NV_SET_CPHS_ON, set_cphs_on);
	   }
	   break;

	  /* case TELE_NV_TELE_CFG:
	   {
		_NDT_TELE_CFG_T tele_cfg;
		uint16 nv_len = 0;

		SCI_MEMSET(str, 0 ,buffer_length + 1) ;
		tele_cfg.show_clear_code = 0;

		nv_len = NVITEM_GetLength(base_id);
		nv_reslut = EFS_NvitemRead(base_id, nv_len ,(uint8*)(&tele_cfg));

		if (NVERR_NONE == nv_reslut)
		{
			sprintf(str, "NV item id: %d \nNV item value \nshow clear code : %d \n", \
				base_id, tele_cfg.show_clear_code);
		}
		else
		{
			sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", base_id, nv_reslut);
			SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", base_id, nv_reslut);
		}
		str_length = SCI_STRLEN(str);

		MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

		GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,	wstr,  str_length, FALSE);
		MMK_SetAtvCtrl(win_id,  MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);
	   }
	   break;*/

	   case TELE_NV_CELL_BROADCAST:
	   {
#ifdef MMI_ETWS_SUPPORT
                cell_broadcast_config = SCI_FALSE;
		   getConfigNVAndDisplayEx(win_id, base_id, cell_broadcast_config);
#else
		   cell_broadcast = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, base_id, cell_broadcast);
#endif
	   }
	   break;

       case TELE_NV_CELL_BROADCAST_CHANNEL:
	   {
		uint16 nv_len = 0;

		SCI_MEMSET(str, 0 ,buffer_length + 1) ;
		SCI_MEMSET(cell_broadcast_channel, 0, 5);

		nv_len = NVITEM_GetLength(base_id);
		nv_reslut = EFS_NvitemRead(base_id,nv_len , (uint8*)cell_broadcast_channel);
		if (NVERR_NONE == nv_reslut)
		{
			sprintf(str, "NV item id: %d \nNV item value \ncell_channel : %s \n", \
				base_id, (uint8*)cell_broadcast_channel);
		}
		else
		{
			sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", base_id, nv_reslut);
			SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", base_id, nv_reslut);
		}

		str_length = SCI_STRLEN(str);
		MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

		GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,	wstr,  str_length, FALSE);
		MMK_SetAtvCtrl(win_id,  MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);
	   }
	   break;
         /*
	   case TELE_NV_CALL_SUMMARY_ENABLE:
	   {
		   calltime_summary = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, TELE_NV_CALL_SUMMARY_ENABLE, calltime_summary);
	   }*/

	   case TEL_NV_L2_FILL_BIT_VAMOS:
	   {
		   fill_bit_vamos = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, base_id, fill_bit_vamos);
	   }
	   break;

	   case TEL_NV_OPERATOR_PAY_SET:
	   {
		   uint8 pay_set = 0; 
		   getConfigNVAndDisplay(win_id, base_id, pay_set);
	   }
	   break;

	   case TEL_NV_NETWORK_TYPE_SELECT:
	   {
		   uint8 network_set = 1; 
		   getConfigNVAndDisplay(win_id, base_id, network_set);
	   }
	   break;
	   	   
	   case TELE_NV_RESERVED_ITEM:
	   {
		//Reserved item
	   }
	   break;
//MESSAGES item
	   //case MESSAGES_NV_SMSC_CONFIG_BY_USER:
	   //{
	//	   config_msg_by_user = SCI_FALSE;
	//	   getConfigNVAndDisplay(win_id, MESSAGES_NV_SMSC_CONFIG_BY_USER, config_msg_by_user);
	 //  }
	 //  break;

	   case MESSAGES_NV_SMS_DELIVERY_REPORT:
	   {
		   sms_delivery_report = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, base_id, sms_delivery_report);
	   }
	   break;

	   case MESSAGES_NV_SMS_CFG:
	   {
			sms_encoding_support = SCI_FALSE;
			getConfigNVAndDisplay(win_id, base_id, sms_encoding_support);
	   }
	   break;

	   case MESSAGES_NV_MMS_CONFIG:
	   {
		   getConfigNVNotImplement(win_id, base_id);
	   }
	   break;

	   case MESSAGES_NV_TIME_STAMP_TYPE:
	   {
		   mms_stamp_src = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, MESSAGES_NV_TIME_STAMP_TYPE, mms_stamp_src);
	   }
	   break;

	   case MESSAGES_NV_MADVERTISE_ON:
	   {
		   advertise_on = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, MESSAGES_NV_MADVERTISE_ON, advertise_on);
	   }
	   break;

	   /*case MESSAGES_NV_FULL_SMS_CHAR:
	   {
		   full_sms_char_support = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, MESSAGES_NV_FULL_SMS_CHAR, full_sms_char_support);
	   }
	   break;*/

	   //case MESSAGES_NV_LARGE_MSG_FONT:
	   //{
	//	   large_message_font = SCI_FALSE;
	///	   getConfigNVAndDisplay(win_id, MESSAGES_NV_LARGE_MSG_FONT, large_message_font);
	  // }
	  // break;
	   case MESSAGES_NV_MMS_DELIVERY_REPORT:
	   {
		   mms_delivery_report = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, base_id, mms_delivery_report);
	   }
	   break;
	   case MESSAGES_NV_SMSC_EDITABLE:
	   	{
		   uint8 smsc_editable = 0; 
		   getConfigNVAndDisplay(win_id, base_id, smsc_editable);			
		}
	   break;
	   
	   case MESSAGES_NV_RESERVED_ITEM:
	   {
	   }
	   break;

//APP item
	   case APP_NV_BT_CONFIG:
	   {
	   	/*
		_NDT_BT_CONFIG_T bt_config;
		uint16 nv_len = 0;

		SCI_MEMSET(str, 0 ,buffer_length + 1) ;
		bt_config.is_visible = 0;
		//SCI_MEMSET(bt_config.name, 0, 20);
		//bt_config.storage_src = 0;
		nv_len = NVITEM_GetLength(APP_NV_BT_CONFIG);
		nv_reslut = EFS_NvitemRead(APP_NV_BT_CONFIG, nv_len ,(uint8*)(&bt_config));

		if (NVERR_NONE == nv_reslut)
		{
			//sprintf(str, "NV item id: %d \nNV item value \nis_visible : %d \nstorage: %d", \
				//APP_NV_BT_CONFIG, bt_config.is_visible,  bt_config.storage_src);
			sprintf(str, "NV item id: %d \nNV item value \nis_visible : %d \n", \
				APP_NV_BT_CONFIG, bt_config.is_visible);
		}
		else
		{
			sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", base_id, nv_reslut);
			SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", base_id, nv_reslut);
		}
		str_length = SCI_STRLEN(str);

		MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

		GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,	wstr,  str_length, FALSE);
		MMK_SetAtvCtrl(win_id,  MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);
		*/
		   bt_is_visible= SCI_FALSE;
		   getConfigNVAndDisplay(win_id, base_id, bt_is_visible);

	   }
	   break;

	   case APP_NV_CAMERA_SHOT_SOUND:
	   {
		   camera_shot_sound = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, APP_NV_CAMERA_SHOT_SOUND, camera_shot_sound);
	   }
	   break;

	   case APP_NV_CAMERA_RECORD_INDICATOR:
	   {
		   camera_record_indicator_light = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, APP_NV_CAMERA_RECORD_INDICATOR, camera_record_indicator_light);
	   }
	   break;

	   case APP_NV_EMERGENCY_CALL_UI:
	   {
		   emergency_call_ui = 0;
		   getConfigNVAndDisplay(win_id, APP_NV_EMERGENCY_CALL_UI, emergency_call_ui);
	   }
	   break;

	   case APP_NV_FM_RECORD_CFG:
	   {
		NDT_RECORD_CFG_T fm_record_cfg;
		uint16 nv_len = 0;

		SCI_MEMSET(str, 0 ,buffer_length + 1) ;
		fm_record_cfg.recorder_enable = 0;
		fm_record_cfg.recorder_sharing = 0;

		  nv_len = NVITEM_GetLength(APP_NV_FM_RECORD_CFG);
		nv_reslut = EFS_NvitemRead(APP_NV_FM_RECORD_CFG, nv_len ,(uint8*)(&fm_record_cfg));

		if (NVERR_NONE == nv_reslut)
		{
			sprintf(str, "NV item id: %d \nNV item value \nrecord enable : %d \nrecord share: %d \n", \
				APP_NV_FM_RECORD_CFG, fm_record_cfg.recorder_enable, fm_record_cfg.recorder_sharing);
		}
		else
		{
			sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", base_id, nv_reslut);
			SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", base_id, nv_reslut);
		}

		str_length = SCI_STRLEN(str);
		MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

		GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,	wstr,  str_length, FALSE);
		MMK_SetAtvCtrl(win_id,  MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);
	   }
	   break;

	   case APP_NV_FM_TUNNING_STEP:
	   {
		   fm_turnning_step = SCI_FALSE;
		   getConfigNVAndDisplayEx(win_id, APP_NV_FM_TUNNING_STEP, &fm_turnning_step);
	   }
	   break;

	   case APP_NV_DEFAULT_BROWSER_APP:
	   {
		   default_browser = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, base_id, default_browser);
	   }
	   break;

	   case APP_NV_RESERVED_NV_ITEM:
	   {

	   }
	   break;

//SETTINGS
	   case SETTINGS_NV_INPUT_T9_ON:
	   {
		   input_t9_on = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, SETTINGS_NV_INPUT_T9_ON, input_t9_on);
	   }
	   break;

	   case SETTINGS_NV_TIME_FORMAT_TYPE:
	   {
		   time_format_type = SCI_FALSE;
		   getConfigNVAndDisplayEx(win_id, SETTINGS_NV_TIME_FORMAT_TYPE, &time_format_type);
	   }
	   break;

	   case SETTINGS_NV_TIME_NITZ_ON:
	   {
		   time_nitz_on = SCI_FALSE;
		   getConfigNVAndDisplayEx(win_id, SETTINGS_NV_TIME_NITZ_ON, &time_nitz_on);
	   }
	   break;

	   case SETTINGS_NV_NDT_VERSION:
	   {
		_NDT_VERSION_T ndt_version;
		uint16 nv_len = 0;

		SCI_MEMSET(str, 0 ,buffer_length + 1) ;
		SCI_MEMSET(ndt_version.lp, 0, 4);
		SCI_MEMSET(ndt_version.pcode, 0, 12);
		SCI_MEMSET(ndt_version.ndt_version, 0, 12);
		SCI_MEMSET(ndt_version.res_version, 0, 12);

		nv_len = NVITEM_GetLength(SETTINGS_NV_NDT_VERSION);
		nv_reslut = EFS_NvitemRead(SETTINGS_NV_NDT_VERSION,nv_len , (uint8*)(&ndt_version));
		if (NVERR_NONE == nv_reslut)
		{
			sprintf(str, "NV item id: %d \nNV item value \nlp : %s \nproduct code: %s \nndt_version: %s \nres_version:%s\n", \
				SETTINGS_NV_NDT_VERSION, ndt_version.lp, ndt_version.pcode, ndt_version.ndt_version, ndt_version.res_version);
		}
		else
		{
			sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", base_id, nv_reslut);
			SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", base_id, nv_reslut);
		}

		str_length = SCI_STRLEN(str);
		MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

		GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,	wstr,  str_length, FALSE);
		MMK_SetAtvCtrl(win_id,  MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);
	   }
	   break;

	   case SETTINGS_NV_SHOW_INDIA_SAR:
	   {
		   show_sar = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, SETTINGS_NV_SHOW_INDIA_SAR, show_sar);
	   }
	   break;

	   case SETTINGS_NV_DEFAULT_SIM_ECC:
	   {
	   	uint16 nv_len = 0;
	   	SCI_MEMSET(sim_ecc, 0, 320);
		nv_len = NVITEM_GetLength(base_id);
		nv_reslut = EFS_NvitemRead(base_id,nv_len , (uint8*)(sim_ecc));
		if (NVERR_NONE == nv_reslut)
		{
			sprintf(str, "NV item id: %d \nNV item value \nsim_ecc[0]: %s \nsim_ecc[1]: %s \n", \
				base_id, sim_ecc[0], sim_ecc[1]);
		}
		else
		{
			sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", base_id, nv_reslut);
			SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", base_id, nv_reslut);
		}

		str_length = SCI_STRLEN(str);
		MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

		GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,	wstr,  str_length, FALSE);
		MMK_SetAtvCtrl(win_id,  MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);
	   }
	   break;

	   case SETTINGS_NV_DEFAULT_NOSIM_ECC:
	   {
	   	uint16 nv_len = 0;
	   	SCI_MEMSET(no_sim_ecc, 0, 480);
		nv_len = NVITEM_GetLength(base_id);
		nv_reslut = EFS_NvitemRead(base_id,nv_len , (uint8*)(no_sim_ecc));
		if (NVERR_NONE == nv_reslut)
		{
			sprintf(str, "NV item id: %d \nNV item value \nno_sim_ecc[0]: %s \nno_sim_ecc[1]: %s \n", \
				base_id, no_sim_ecc[0], no_sim_ecc[1]);
		}
		else
		{
			sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", base_id, nv_reslut);
			SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", base_id, nv_reslut);
		}

		str_length = SCI_STRLEN(str);
		MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

		GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,	wstr,  str_length, FALSE);
		MMK_SetAtvCtrl(win_id,  MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);
	   }
	   break;

	   case SETTINGS_NV_DEFAULT_VOICEMAIL:
	   {
		uint16 nv_len = 0;

		SCI_MEMSET(str, 0 ,buffer_length + 1) ;
		SCI_MEMSET(voice_mail, 0, 32);

		nv_len = NVITEM_GetLength(base_id);
		nv_reslut = EFS_NvitemRead(base_id,nv_len , (uint8*)voice_mail);
		if (NVERR_NONE == nv_reslut)
		{
			sprintf(str, "NV item id: %d \nNV item value \default voicemail : %s \n", \
				base_id,  (uint8*)voice_mail);
		}
		else
		{
			sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", base_id, nv_reslut);
			SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", base_id, nv_reslut);
		}

		str_length = SCI_STRLEN(str);
		MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

		GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,	wstr,  str_length, FALSE);
		MMK_SetAtvCtrl(win_id,  MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);
	   }
	   break;

	   case SETTINGS_NV_DEFAULT_HOMEPAGE:
	   {
		uint16 nv_len = 0;

		SCI_MEMSET(str, 0 ,buffer_length + 1) ;
		SCI_MEMSET(homepage, 0, 128);

		nv_len = NVITEM_GetLength(base_id);
		nv_reslut = EFS_NvitemRead(base_id,nv_len , (uint8*)homepage);
		if (NVERR_NONE == nv_reslut)
		{
			sprintf(str, "NV item id: %d \nNV item value \nhomepage : %s \n", \
				base_id,  (uint8*)homepage);
		}
		else
		{
			sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", base_id, nv_reslut);
			SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", base_id, nv_reslut);
		}

		str_length = SCI_STRLEN(str);
		MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

		GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,	wstr,  str_length, FALSE);
		MMK_SetAtvCtrl(win_id,  MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);
	   }
	   break;

	   case SETTINGS_NV_DEFAULT_BOOKMARK:
	   {
		   getConfigNVNotImplement(win_id, base_id);
	   }
	   break;

	   case SETTINGS_NV_DEFAULT_LANGUAGE:
	   {
	   	/*
		uint16 nv_len = 0;

		SCI_MEMSET(str, 0 ,buffer_length + 1) ;
		SCI_MEMSET(default_lang, 0, 32);

		nv_len = NVITEM_GetLength(base_id);
		nv_reslut = EFS_NvitemRead(base_id,nv_len , (uint8*)default_lang);
		if (NVERR_NONE == nv_reslut)
		{
			sprintf(str, "NV item id: %d \nNV item value \nlanguage : %s \n", \
				base_id,  (uint8*)default_lang);
		}
		else
		{
			sprintf(str, "NV item id: %d \nNV item value \nnv read error : %d \n", base_id, nv_reslut);
			SCI_TRACE_LOW("gmw--item id:%d nv_reslut=%d", base_id, nv_reslut);
		}

		str_length = SCI_STRLEN(str);
		MMI_STRNTOWSTR(wstr, buffer_length, (uint8*)str, str_length, str_length );

		GUITEXT_SetString(MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID,	wstr,  str_length, FALSE);
		MMK_SetAtvCtrl(win_id,  MMIENG_CONFIG_NV_TEXTBOX_CTRL_ID);
		*/
		   default_lang = 0;
		   getConfigNVAndDisplay(win_id, base_id, default_lang);
	   }
	   break;
	   case SETTINGS_NV_CIPHERING_A5:
	   {
		   ciphering_a5 = SCI_FALSE;
		   getConfigNVAndDisplay(win_id, base_id, ciphering_a5);
	   }
	   break;	

	   case SETTINGS_NV_CIPHERING_GEA:
	   {
		   uint8 ciphering_gea = 0; 
		   getConfigNVAndDisplay(win_id, base_id, ciphering_gea);
	   }
	   break;

	   case SETTINGS_NV_IP_PORTOCOL_SELECTION:
	   {
		   uint8 ip_protocol_selection = 0; 
		   getConfigNVAndDisplay(win_id, base_id, ip_protocol_selection);
	   }
	   break;	

       case SETTINGS_NV_ENABLE_DOMESTIC_ROAMING:
       {
           uint8 is_support_domestic_roaming = 0; 
           getConfigNVAndDisplay(win_id, base_id, is_support_domestic_roaming);
       }
       break;	

       case SETTINGS_NV_RESERVED_ITEM:
	   {

	   }
	   break;

	   case OTHER_NV_MENU_TREE_LIST:
	   {
		   getConfigNVNotImplement(win_id, base_id);
	   }
	   break;

	   case OTHER_NV_PRODUCT_TYPE:
	   {
		   //
                 uint8 product_type = 1;
		   getConfigNVAndDisplay(win_id, base_id, product_type);
	   }
	   break;	

	   case OTHER_NV_OPERATOR_LOGO:
	   {
		   //
                 uint8 operator_logo = 0; 
		   getConfigNVAndDisplay(win_id, base_id, operator_logo);
	   }
	   break;	
	   
	   default:
		break;
	}

}
/*****************************************************************************/
//  Description : handle config nv item test
//  Global resource dependence : none
//  Author: mingwen.ge
//  Note:   2015.05.12
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConfigNVTestWinMsg(
                                                 MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                 DPARAM             param       // 相应消息的参数
                                                 )
{

    MMI_RESULT_E result = MMI_RESULT_TRUE;
    NVITEM_ERROR_E nv_reslut = NVERR_NONE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
	      base_id = TELE_NV_DIAL2_PROMPT;
	      getConfigNVAndDisplay(win_id, TELE_NV_DIAL2_PROMPT, dial2_prompt);
        }
        break;

    case MSG_FULL_PAINT:
        break;
    case MSG_KEYDOWN_DOWN:
		{
			if((base_id >= TELE_NV_DIAL2_PROMPT)&& (base_id < (TELE_NV_RESERVED_ITEM - 1)))
			{
	 			++base_id;
			}
			else if(base_id == (TELE_NV_RESERVED_ITEM - 1))
				{
				    base_id = MESSAGES_NV_SMS_DELIVERY_REPORT;
				}
			else if((base_id >= MESSAGES_NV_SMS_DELIVERY_REPORT)&& (base_id < (MESSAGES_NV_RESERVED_ITEM - 1)))
				{
	 			    ++base_id;
				}
			else if(base_id == (MESSAGES_NV_RESERVED_ITEM - 1))
				{
				    base_id = APP_NV_BT_CONFIG;
				}
			else if((base_id >= APP_NV_BT_CONFIG)&& (base_id < (APP_NV_RESERVED_NV_ITEM - 1)))
				{
	 			    ++base_id;
				}
			else if(base_id == (APP_NV_RESERVED_NV_ITEM - 1))
				{
				    base_id = SETTINGS_NV_INPUT_T9_ON;
				}
			else if((base_id >= SETTINGS_NV_INPUT_T9_ON)&& (base_id < (SETTINGS_NV_RESERVED_ITEM - 1)))
				{
	 			    ++base_id;
				}
			else  if(base_id == (SETTINGS_NV_RESERVED_ITEM - 1))
				{
				     base_id = OTHER_NV_MENU_TREE_LIST;
				}
			else if((base_id >= OTHER_NV_MENU_TREE_LIST)&& (base_id < (OTHER_NV_RESERVED_ITEM - 1)))
				{
	 			    ++base_id;
				}
			else  if(base_id == (OTHER_NV_RESERVED_ITEM - 1))
				{
				     base_id = TELE_NV_DIAL2_PROMPT;
				}

			ConfigNvDisplay(win_id);

			MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    	}
		break;
	case MSG_KEYDOWN_UP:
		{
			if((base_id == TELE_NV_DIAL2_PROMPT))
			{
			    base_id = (OTHER_NV_RESERVED_ITEM - 1);
			}
			else if((base_id > TELE_NV_DIAL2_PROMPT)&& (base_id <= (TELE_NV_RESERVED_ITEM - 1)))
			{
	 			--base_id;
			}
			else if(base_id == MESSAGES_NV_SMS_DELIVERY_REPORT)
				{
				    base_id = (TELE_NV_RESERVED_ITEM - 1);
				}
			else if((base_id > MESSAGES_NV_SMS_DELIVERY_REPORT)&& (base_id <= (MESSAGES_NV_RESERVED_ITEM - 1)))
				{
	 			    --base_id;
				}
			else if(base_id == APP_NV_BT_CONFIG)
				{
				    base_id = (MESSAGES_NV_RESERVED_ITEM - 1);
				}
			else if((base_id > APP_NV_BT_CONFIG)&& (base_id <= (APP_NV_RESERVED_NV_ITEM - 1)))
				{
	 			    --base_id;
				}
			else if(base_id == SETTINGS_NV_INPUT_T9_ON)
				{
				    base_id = (APP_NV_RESERVED_NV_ITEM - 1);
				}
			else if((base_id > SETTINGS_NV_INPUT_T9_ON)&& (base_id <= (SETTINGS_NV_RESERVED_ITEM - 1)))
				{
	 			    --base_id;
				}
			else if(base_id == OTHER_NV_MENU_TREE_LIST)
				{
				    base_id = (SETTINGS_NV_RESERVED_ITEM - 1);
				}
			else if((base_id > OTHER_NV_MENU_TREE_LIST)&& (base_id <= (OTHER_NV_RESERVED_ITEM- 1)))
				{
	 			    --base_id;
				}

			ConfigNvDisplay(win_id);

			MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    	}
		break;

    case MSG_APP_WEB:
    case MSG_KEYDOWN_OK:
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

//gmw---end
