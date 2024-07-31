/****************************************************************************
** File Name:      mmieng_base.h							                                                 *
** Author:          hui.zhao								                                                 *
** Date:             2009.07.28														*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.      				*
** Description:    this part will also be used when eng_support macor is closed  									               					*
*****************************************************************************
**                         Important Edit History                        							*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             						*
** 07/2009      hui.zhao               Create
**
****************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "guilabel.h"

#include "version.h"

//#include "mmieng.h"
#include "mmieng_internal.h"
#include "mmieng_id.h"
#include "mmieng_win.h"
#include "mmieng_text.h"
#include "mmieng_uitestwin.h"
#ifdef NORMAL_TO_DEBUG_MODE
#include "mmipub.h" 
#endif
#include "production_test.h"
#include "ref_outport.h"

/**--------------------------------------------------------------------------*
			                         MACRO DEFINITION
 **--------------------------------------------------------------------------*/
#define MMIENG_VERSION_BUF_MAX_LEN  300

/**--------------------------------------------------------------------------*
			                         LOCAL FUNCTION DECLARE
 **--------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E EngShowVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
PUBLIC int32 GetProductionSNStr(uint8* pd_buf, uint8 buf_len,  const PRODUCTION_TEST_INFO_T *s_mmieng_product);

LOCAL void ShowVersionInfo(BOOLEAN   need_update);
#ifdef ENG_SUPPORT
LOCAL MMI_RESULT_E EngShowOutVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
/**--------------------------------------------------------------------------*
			                         GLOBAL DEFINITION
 **--------------------------------------------------------------------------*/
const MMI_ENG_DISPLAY_T mmi_eng_display =
{
	MMI_DEFAULT_TEXT_FONT,				// text_font
	MMI_DEFAULT_TEXT_COLOR,				// text_color
	MMI_RED_COLOR,						// spec_text_color
	MMI_WHITE_COLOR,//NEWMS00181616     // bkgd_color
	0,									// char_space
	0,									// line_space
	SCROLL_BAR_WIDTH					// scrollbar_width
};

BOOLEAN g_mmieng_showversion_from_ui = FALSE;	//from uitest
BOOLEAN g_mmieng_showversion_for1 = FALSE; //  TRUE: #*8378#1#; FALSE: #*8378#0#

/**--------------------------------------------------------------------------*
**                         WINTAB DEFINITION                                *
**--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIENG_SHOWVERSION_WIN_TAB) =
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngShowVersionWinHandleMsg),
	WIN_ID(MMIENG_SHOWVERSION_WIN_ID),
#if defined MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
	WIN_TITLE(TXT_ENG_VERSION_INFO),
	//WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
    CREATE_TEXT_CTRL(MMIENG_SHOW_VERSION_CTRL_ID),
    END_WIN
};

#ifdef ENG_SUPPORT
//显示外部版本号
WINDOW_TABLE(MMIENG_SHOW_OUTVERSION_WIN_TAB) =
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
	WIN_FUNC((uint32)EngShowOutVersionWinHandleMsg),
	WIN_ID(MMIENG_SHOW_OUTVERSION_WIN_ID),
	WIN_TITLE(TXT_ENG_VERSION_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	//CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIENG_LABEL1_CTRL_ID),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
//#ifdef SHOW_HW_VERSION
//	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIENG_LABEL2_CTRL_ID),
//	CREATE_TEXT_CTRL(MMIENG_TEXTBOX2_CTRL_ID),
//#endif
	END_WIN
};

/*===============================================================================
								IMPLEMENTATION
===============================================================================*/
/********************************************************************************
 NAME:			MMIAPIENG_CreateShowOutVersionWin
 DESCRIPTION:
 PARAM IN:
 PARAM OUT:
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowOutVersionWin(void)
{
    return MMK_CreateWin((uint32*)MMIENG_SHOW_OUTVERSION_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			EngShowOutVersionWinHandleMsg
 DESCRIPTION:
 PARAM IN:		win_id -
				msg_id -
				param -
 PARAM OUT:
 AUTHOR:		allen
 DATE:			2004.09.08
********************************************************************************/
LOCAL MMI_RESULT_E EngShowOutVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;

	int32 length = 0;
	int32 offset = 0;
// 	char buffer[MMIENG_VERSION_BUF_MAX_LEN] = {0};
	char text[MMIENG_VERSION_BUF_MAX_LEN]= {0} ;

	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
        {
            const char* str = PNULL;
            wchar wstr[MMIENG_VERSION_BUF_MAX_LEN] = {0};
            uint32 cal_info = 0;
            uint16          uint16_str[MMIENG_VERSION_BUF_MAX_LEN + 1] = {0};
            uint16          uint16_str_len = 0;
            BOOLEAN is_calibration = FALSE;
            uint16 cal_ok[10] = {0x5DF2, 0x6821, 0x51C6, 0, 0}; //已校准5DF2 6821 51C6 
            uint16 cal_fail[10] = {0x672A, 0x6821, 0x51C6, 0, 0}; //未校准672A 6821 51C6 
            uint16 tts_ok[10] = {0x0054, 0x0054, 0x0053, 0x5DF2, 0x6FC0, 0x6D3B, 0}; //TTS已激活0054 0054 0053 5DF2 6FC0 6D3B 
            uint16 tts_fail[10] = {0x0054, 0x0054, 0x0053, 0x672A, 0x6FC0, 0x6D3B, 0}; //TTS未激活0054 0054 0053 672A 6FC0 6D3B 
            
            
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
            uint16_str[uint16_str_len++] = CR_CHAR;
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
            uint16_str[uint16_str_len++] = CR_CHAR;
#endif			
#endif

            str = "Software V:\n";
            length = strlen(str);
	    SCI_MEMCPY(text + offset, str, length);
		offset += length ;

#ifndef _WIN32
            //text.wstr_ptr = COMMON_GetMMIOutVersionInfo();
            str =  VERSION_GetInfo(MMI_VERSION);
#else
            str = "GPHONE_R1.0.8003.C0.2";	// this is a example for simulator
#endif
	   length = strlen(str);
	    SCI_MEMCPY(text + offset, str, length);
		offset += length ;

		text[offset] = CR_CHAR;
		offset += 1;

#ifdef SHOW_HW_VERSION
            str = "Hardware V:\n";
            length = strlen(str);
	    SCI_MEMCPY(text + offset, str, length);
		offset += length ;

#ifndef _WIN32
            str =  VERSION_GetInfo(HW_VERSION);
#else
            str = "SP7100A-V1.0.1-B7-M6416";	// this is a example for simulator
#endif

	   length = strlen(str);
	    SCI_MEMCPY(text + offset, str, length);
		offset += length ;

		text[offset] = CR_CHAR;
		offset += 1;
#endif
            MMI_STRNTOWSTR( wstr, MMIENG_VERSION_BUF_MAX_LEN, (uint8*)text, offset, offset );
            MMIAPICOM_Wstrcat(uint16_str, wstr);
            uint16_str_len += offset;
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, uint16_str, uint16_str_len, FALSE);
        }
        break;

	case MSG_FULL_PAINT:
		// the softkey is different between UI-TEST mode and Engineer mode
		if (g_mmieng_showversion_from_ui)
			GUIWIN_SetSoftkeyTextId(MMIENG_SHOW_OUTVERSION_WIN_ID,  TXT_NULL, (MMI_TEXT_ID_T)STXT_UP, TXT_NULL, FALSE);
		else
			GUIWIN_SetSoftkeyTextId(MMIENG_SHOW_OUTVERSION_WIN_ID,  (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
		break;


    case MSG_APP_OK:
    case MSG_APP_CANCEL:
	case MSG_CTL_MIDSK:
	case MSG_CTL_OK:
	case MSG_CTL_CANCEL:
		// only in UI-TEST mode, UP key will do the next action; otherwise will do nothing
		if (g_mmieng_showversion_from_ui)
		{
			MMIAPIENG_RunUITestNextStep();
		}
		 MMK_CloseWin(win_id);

		break;

	case MSG_CLOSE_WINDOW:
		g_mmieng_showversion_for1 = FALSE;
		break;

#ifdef FLIP_PHONE_SUPPORT
	case MSG_KEYDOWN_FLIP:		// close flip
		// in UI-TEST mode, close FLIP will do nothing
		if (!g_mmieng_showversion_from_ui)
			result = MMI_RESULT_FALSE;
		break;
#endif

	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}
#endif
/********************************************************************************/
//  NAME:			MMIAPIENG_CreateShowOutVersionWin
//  DESCRIPTION:
//  PARAM IN:
//  PARAM OUT:
//  AUTHOR:		songbin.zeng
//  DATE:			2006.10.11
/********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowVersionWin(void)
{
    MMI_HANDLE_T handle = PNULL;

    handle = MMK_CreateWin((uint32*)MMIENG_SHOWVERSION_WIN_TAB, PNULL);

    if (PNULL == handle)
    {
        return FALSE;
    }

    return TRUE;
}
#ifdef NORMAL_TO_DEBUG_MODE
PUBLIC BOOLEAN MMIAPIENG_SwitchAutoUsbLogOn(void)
{

   MMI_TEXT_ID_T status_id = TXT_NULL;
   status_id = TXT_ENG_USBLOG_ON;
   REFPARAM_SetUsbServicesFlag(1);
   MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NULL,status_id,IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
   return TRUE;

}

PUBLIC BOOLEAN MMIAPIENG_SwitchAutoUsbLogOff(void)
{
   
   MMI_TEXT_ID_T status_id = TXT_NULL;
   status_id = TXT_ENG_USBLOG_OFF;
   REFPARAM_SetUsbServicesFlag(0);
   MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NULL,status_id,IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
   return TRUE;
  
}
#endif
/********************************************************************************/
//  NAME:          EngShowVersionWinHandleMsg
//  DESCRIPTION:
//  PARAM IN:      win_id -
//                 msg_id -
//                 param -
//  PARAM OUT:
//  AUTHOR:        allen
//  DATE:          2004.09.08
/********************************************************************************/
LOCAL MMI_RESULT_E EngShowVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(!g_mmieng_showversion_for1)
        {
            if(g_mmieng_showversion_from_ui)
            {
                GUITEXT_SetCircularHandle(FALSE,MMIENG_SHOW_VERSION_CTRL_ID);
            }
        }
        MMK_SetAtvCtrl(win_id, MMIENG_SHOW_VERSION_CTRL_ID);

        // fix cr187363
        // set version info into text when created.
        // while setting at MSG_FULL_PAINT, it couldn't be selected all by clipboard
        ShowVersionInfo(FALSE); //for 3D effect cr190356, modify parameter from TRUE to FALSE to avoid to draw content to LCD in open_window msg.

            // the softkey is different between UI-TEST mode and Engineer mode
//#if defined KEYPAD_TYPE_TYPICAL_KEYPAD || defined KEYPAD_TYPE_QWERTY_KEYPAD
            if (!g_mmieng_showversion_for1)
            {
                //if (g_mmieng_showversion_from_ui)
                //{
                    GUIWIN_SetSoftkeyTextId(MMIENG_SHOWVERSION_WIN_ID,  TXT_ENG_PASS, TXT_NULL, TXT_ENG_FAIL, FALSE);
                //}
                //else
                //{
               //     GUIWIN_SetSoftkeyTextId(MMIENG_SHOWVERSION_WIN_ID,  (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
                //}
            }
            else
//#endif
            {
                 GUIWIN_SetSoftkeyTextId(MMIENG_SHOWVERSION_WIN_ID,  TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_NULL, STXT_RETURN, FALSE);
            }

        break;

    case MSG_FULL_PAINT:
     break;

     //add chunjuanliang
         case MSG_KEYDOWN_OK:
         case MSG_CTL_OK:
         {
             if (!g_mmieng_showversion_for1)
             {
                 //save result = 1
                 MMIAPIENG_SaveTestResult(win_id,1);
                 //call nextstep
                 MMIAPIENG_RunUITestNextStep();
                
             }
      
             //close winid;
             MMK_CloseWin(win_id);
      
             break;
         }
         case MSG_KEYDOWN_CANCEL:
		 case MSG_CTL_CANCEL:
         {
             if (!g_mmieng_showversion_for1)
             {
                 //save result = 0
                 MMIAPIENG_SaveTestResult(win_id,0);
                 //call nextstep
                 MMIAPIENG_RunUITestNextStep();
             }
      
             //close winid;
             MMK_CloseWin(win_id);
      
             break;
         }
         //add chunjuanliang


#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    //case MSG_KEYDOWN_OK:
    //case MSG_KEYDOWN_CANCEL:
    //case MSG_CTL_OK:
    //case MSG_CTL_CANCEL:
        /*
        // only in Engineer mode, OK / CANCEL key will close this window; otherwise will do nothing
        if (!g_mmieng_showversion_for1)
        {
            if (g_mmieng_showversion_from_ui)
            {
              MMIAPIENG_RunUITestNextStep();
            }
        }
        MMK_CloseWin(win_id);
    */
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
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
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_RED:
        if (!g_mmieng_showversion_for1)
        {
            if (!g_mmieng_showversion_from_ui)
            {
                MMK_CloseWin(MMIENG_SHOWVERSION_WIN_ID);
            }
        }
        else
        {
            g_mmieng_showversion_for1 = FALSE;
            result = MMI_RESULT_FALSE;
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_KEYDOWN_FLIP:      // close flip
        // in UI-TEST mode, close FLIP will do nothing
        if (g_mmieng_showversion_for1)
        {
            result = MMI_RESULT_FALSE;
        }
        break;
#endif

    case MSG_CLOSE_WINDOW:
        // not in UI-TEST mode
        if (g_mmieng_showversion_for1)
        {
            g_mmieng_showversion_for1 = FALSE ;
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
PUBLIC int32 GetProductionSNStr(uint8* pd_buf, uint8 buf_len, 
                        const PRODUCTION_TEST_INFO_T *s_mmieng_product )
{
    int len1 = 0;
    int offset = 0;
  
    if(PNULL == pd_buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetProductionSNStr pd_buf == null"
      //  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_7674_112_2_18_2_17_56_153,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_win.c GetProductionSNStr buf_len is %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_7677_112_2_18_2_17_56_154,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
    ////////////////
    //  S/N
    ////////////////
    MMI_MEMCPY(pd_buf + offset, buf_len - offset, "<S/N>", 5, 5);
    offset += 5;
    pd_buf[offset] = CR_CHAR;
    offset += 1;
    len1 = SCI_STRLEN(s_mmieng_product->serial_num);
     MMI_MEMCPY(pd_buf + offset, buf_len - offset, s_mmieng_product->serial_num, 32, len1);
    offset += len1;
    pd_buf[offset] = CR_CHAR;
    offset += 1;

    return (offset);
}

PUBLIC int32 SN_Read(uint8* pd_buf, uint8 buf_len, 
                        const PRODUCTION_TEST_INFO_T *s_mmieng_product )
{
    int len1 = 0;
    int offset = 0;
  
    if(PNULL == pd_buf)
    {
        return -1;
    }    
    if(2 > buf_len)
    {
        return -2;
    }
    
    len1 = SCI_STRLEN(s_mmieng_product->serial_num);
    *pd_buf = s_mmieng_product->serial_num[len1 - 1];
    *(++pd_buf) = s_mmieng_product->serial_num[len1 - 2];
    *(++pd_buf) = '\0';

	offset = 2;

    return (offset);
}



/*****************************************************************************/
//  Description : 显示版本信息(包括软件,硬件,base版本)
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void ShowVersionInfo(
                            BOOLEAN   need_update
                            )
{   
    wchar           unicode_cr = 0x0d;
    MMI_STRING_T    cr_s = {0};
    MMI_STRING_T    text_s = {0};
    uint16          uint16_str[MMIENG_3RDPARTYVER_TEXT_LEN + 100] = {0};
    uint16          uint16_str_len = 0;
    char*           temp_ptr = PNULL;
    uint16          temp_len = 0;
	uint8 result;
	PRODUCTION_TEST_INFO_T s_mmieng_product = {0};
	uint8 text_buf[151] = {0};
	wchar wstr[151] = {0};
	uint8* str = PNULL;
	uint8 pcode[12];	//product code
    cr_s.wstr_ptr = &unicode_cr;
    cr_s.wstr_len = 1;
	
//Software V:
#ifdef _DOUBLESHOW_VERSION_
    temp_ptr = (char*)"Software V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#endif
#ifndef _WIN32
    temp_ptr = VERSION_GetInfo(PLATFORM_VERSION);   /*lint !e605*/
#else
//    temp_ptr = "GPHONE_R1.0.5003.C0.2 GPHONE_R1.0.5003.C0.2";	// this is a example for simulator
    temp_ptr = (char*)"Platform Version:  MOCOR_W09.23_Debug";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行


//Project V:
#ifdef _DOUBLESHOW_VERSION_
    temp_ptr = (char*)"Project V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#endif
#ifndef _WIN32
	temp_ptr = VERSION_GetInfo(PROJECT_VERSION);
#else
    temp_ptr = (char*)"Project Version:  sc7701_320X480_PDA_64MB";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

//temp_ptr = strchr(temp_ptr,':')+1;
//Hardware V:
#ifdef _DOUBLESHOW_VERSION_
    temp_ptr = (char*)"Hardware V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#endif
#ifndef _WIN32
    temp_ptr = VERSION_GetInfo(HW_VERSION);     /*lint !e605*/
#else
//    temp_ptr = "SP7100A-V1.0.1-B7-M6416 SP7100A-V1.0.1-B7-M6416";	// this is a example for simulator
    temp_ptr = (char*)"HW Version:        SC6600L_sp6601l";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行


//wangfeizhou add for show RFPA ID HWID
// RFPA ID
//houlili modify,2015
#ifndef _WIN32
 	temp_ptr = SCI_GET_RF_ID();
#else
	temp_ptr =(char*)"RFPA ID:000";
#endif
//end houlili
	temp_len = SCI_STRLEN((char*)temp_ptr);
 	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
 	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#ifndef _WIN32
LP_CODE_Read(pcode);
temp_ptr = (char*)"NDT code:";
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
if(0==temp_len)
{
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, "0", 1);
}
else
{
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
}
MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#endif
#ifndef _WIN32
//HWID
	temp_ptr = (char*)"HWID:";
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	temp_ptr = (char*)HWID_Read();
	temp_len = SCI_STRLEN((char*)temp_ptr);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);
	if (SCI_SUCCESS == SCI_GetProductionTestInfo(&s_mmieng_product))
	{
		 temp_len = GetProductionSNStr(text_buf, 150,&s_mmieng_product);
		 temp_ptr = (char*) text_buf;
		MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);			  
       }
#endif

#ifdef _DOUBLESHOW_VERSION_
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
 
//AP SVN Version
#ifndef _WIN32
    temp_ptr = VERSION_GetInfo(AP_SVN_VERSION);     /*lint !e605*/
#else
//    temp_ptr = "SP7100A-V1.0.1-B7-M6416 SP7100A-V1.0.1-B7-M6416";	// this is a example for simulator
    temp_ptr = (char*)"AP SVN :    0000";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    
//CP SVN Version
#ifndef _WIN32
	SCI_MEMSET(text_buf,0,151);
	sprintf(text_buf,"CP SVN :%d",ipc_get_modem_version());
	temp_ptr = (char * )text_buf;
    ///temp_ptr = VERSION_GetInfo(CP_SVN_VERSION);     /*lint !e605*/
#else
//    temp_ptr = "SP7100A-V1.0.1-B7-M6416 SP7100A-V1.0.1-B7-M6416";	// this is a example for simulator
    temp_ptr = (char*)"CP SVN :    0000";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

 //Build Time:
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
    //MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    {
       GUI_FONT_T font = MMI_DEFAULT_TEXT_FONT;
       GUI_COLOR_T font_color = MMI_BLACK_COLOR;
       GUITEXT_SetFont(MMIENG_SHOW_VERSION_CTRL_ID,&font,&font_color);
    }
    text_s.wstr_ptr = uint16_str;
    text_s.wstr_len = uint16_str_len;
    GUITEXT_SetString(MMIENG_SHOW_VERSION_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, need_update);
}

/*Edit by script, ignore 1 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
