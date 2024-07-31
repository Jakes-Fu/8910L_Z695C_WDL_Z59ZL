/****************************************************************************
** File Name:      mmidatacounter_result.c                            
** Author:                                                                  
** Date:            2015/08/24
** Description:    This file is used to process msg to main window of DATACOUNTER
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
****************************************************************************/
#define _MMIDATACOUNTER_RESULT_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "guicommon.h"
#include "mmi_text.h"
#include "tb_dal.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "guimenu.h"

#include "guimsgbox.h"
#include "guilabel.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "guibutton.h"
#include "guistatusbar.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmk_msg.h"
#include "guiwin.h"
#include "guimenu.h"

#include "mmi_resource.h"
#include "guistring.h"
#include "mmipdp_export.h"
#include "mmi_nv.h"

#include "datacounter_text.h"
#include "datacounter_menutable.h"
#include "datacounter_internal.h"
#include "mmidatacounter_nv.h"
#include "datacounter_id.h"
#include "mmiphone_export.h"
#include "mmiphone_internal.h"
#include "mmicalendar_export.h"
#include "mmipub.h"
#include "mn_type.h"
#include "mmisms_export.h"
#include "mmisms_app.h"

LOCAL MN_DUAL_SYS_E m_simId = MMI_SmartDS_SIM_ID_1;

LOCAL MMI_RESULT_E HandleDataCounterResultWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param);

LOCAL MMI_RESULT_E HandleClearALLCounterPopMenuWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           );
LOCAL uint32 MMIAPIDataCounter_readDataNV(MN_DUAL_SYS_E simCard, MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv);

WINDOW_TABLE( DATA_COUNTER_WIN_RESULT_TAB ) = 
{
	WIN_FUNC((uint32) HandleDataCounterResultWinMsg),
	WIN_STATUSBAR,
	WIN_ID(MMIACC_DATA_COUNTER_WIN_RESULT_ID),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_DATACOUNTER_SIM_RESULT_CTRL_ID),
	WIN_SOFTKEY(STXT_OPTION, STXT_SOFTKEY_SELECT_MK, STXT_RETURN),
	END_WIN
};

//clear all counter
WINDOW_TABLE(MMITEST_CLEAR_ALL_COUNTER_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleClearALLCounterPopMenuWinMsg),
    WIN_ID(MMIACC_DATA_COUNTER_WIN_PUPMENU_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_HIDE_STATUS,
    CREATE_POPMENU_CTRL(MENU_DATACOUNTER_CLEAR_ALL_TABLE_ID,MMI_DATACOUNTER_CLEAR_ALL_COUNTER_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMITEST_CLEAR_ALL_CONFIRM_WIN_TAB) =
{
	WIN_ID(MMIACC_DATA_COUNTER_WIN_CLEAR_ALL_ID),
	WIN_STATUSBAR,
	WIN_STYLE(WS_DISABLE_RETURN_WIN),
	END_WIN
};

LOCAL void GetDataCounter_month(uint8 month_index, uint8* str_buff)
{
    MMI_STRING_T res_strid = {0};
    
    switch(month_index)
    {
        case 0:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_JAN, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
        case 1:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_FEB, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
        case 2:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_MAR, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
        case 3:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_APR, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
         case 4:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_MAY, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
         case 5:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_JUN, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
         case 6:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_JUL, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
         case 7:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_AUG, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
         case 8:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_SEP, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
         case 9:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_OCT, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
         case 10:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_NOV, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
         case 11:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_DEC, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
         default:
            MMI_GetLabelTextByLang(STR_SSHEET_MONTH_JAN, &res_strid);
            MMIAPICOM_WstrToStr(res_strid.wstr_ptr, str_buff);
            break;
    }
    
}

PUBLIC void MMIDataCounter_FormatTimeStrByTime(
                                        uint32 duration_time,
                                        MMI_STRING_T *p_time_str
                                        )
{
    MMI_STRING_T res_strid = {0};
    MMI_STRING_T txt_hou = {0};
    MMI_STRING_T txt_min = {0};
    //MMI_STRING_T txt_sec = {0};
    uint8 time_str[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    uint8 time_min_str[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    //uint8 time_sec_str[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    wchar wsz_hour[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    wchar wsz_min[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    //wchar wsz_sec[MMICL_DURATION_TIME_STR_MAX_LEN] = {0};
    MMI_STRING_T txt_temp = {0};
    MMI_STRING_T txt_temp2 = {0};
    MMI_TM_T tm = {0};

    if (p_time_str == PNULL)
    {
        return;
    }

    tm = MMIAPICOM_Second2Tm(duration_time);
    SCI_TRACE_LOW("MMIDataCounter_FormatTimeStrByTime ---- [hour: min:sec] %d: %d: %d",tm.tm_hour, tm.tm_min,tm.tm_sec);
    sprintf((char *)time_str,"%u ",tm.tm_hour);
    sprintf((char *)time_min_str,"%u ",tm.tm_min);
    //sprintf((char *)time_sec_str," %u ",tm.tm_sec);

    MMIAPICOM_StrToWstr(time_str, wsz_hour);
    MMIAPICOM_StrToWstr(time_min_str, wsz_min);
    //MMIAPICOM_StrToWstr(time_sec_str, wsz_sec);

    txt_hou.wstr_ptr = wsz_hour;
    txt_hou.wstr_len = MMIAPICOM_Wstrlen(wsz_hour);
    txt_min.wstr_ptr = wsz_min;
    txt_min.wstr_len = MMIAPICOM_Wstrlen(wsz_min);
    
    //txt_sec.wstr_ptr = wsz_sec;
    //txt_sec.wstr_len = MMIAPICOM_Wstrlen(wsz_sec);
    
    if (tm.tm_hour > 0)
    {
        MMI_GetLabelTextByLang(STR_COUN_UNIT_H_EXT02, &res_strid);
        
        MMIAPICOM_CatTwoString(&txt_temp, &txt_hou, &res_strid);
        MMI_GetLabelTextByLang(STR_START_DEALY_MIN, &res_strid);
        MMIAPICOM_CatTwoString(&txt_temp2, &txt_temp, &txt_min);
        MMIAPICOM_CatTwoString(p_time_str, &txt_temp2, &res_strid);
        MMIAPICOM_DestroyString(txt_temp);
        MMIAPICOM_DestroyString(txt_temp2);
    }
    else
    {
        MMI_GetLabelTextByLang(STR_START_DEALY_MIN, &res_strid);
        MMIAPICOM_CatTwoString(p_time_str, &txt_min, &res_strid);
    }
    
}

LOCAL MMI_RESULT_E HandleClearAllDataQuery(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE; 
    switch (msg_id)
    {
       case MSG_OPEN_WINDOW:
	        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);	
              break;
	case MSG_GET_FOCUS:
		 break;
	case MSG_CTL_OK:
	case MSG_APP_WEB:
	case MSG_APP_OK:	
	case MSG_NOTIFY_MIDSK:
              ClearDataCounterPeriodAllRecord(3, m_simId);
	        MMK_CloseWin(win_id);
		break;
       case MSG_APP_CANCEL:
		if (win_id == MMK_GetFocusWinId())
		{
			MMK_CloseWin(win_id);
		}		
		break;
       default:
		recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return recode;
}

LOCAL void Clear_AllConfirmationDialogWindow(void)
{
	 MMI_WIN_ID_T     win_id_query = MMIACC_DATA_COUNTER_WIN_CLEAR_ALL_ID;
	 MMIPUB_HANDLE_FUNC     handle_func = HandleClearAllDataQuery;
	 MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_COUN_QUERY_ALL_EXT01, PNULL, PNULL, &win_id_query,PNULL, MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,handle_func);
}

LOCAL MMI_RESULT_E HandleClearALLCounterPopMenuWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMI_DATACOUNTER_CLEAR_ALL_COUNTER_MENU_CTRL_ID;
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_FULL_PAINT: 
	break;
    case MSG_CTL_OK:
        break;
    case MSG_CTL_MIDSK:
	 Clear_AllConfirmationDialogWindow();
	 MMK_CloseWin(win_id);
		break;
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

PUBLIC void ReadResetPeriodDate(MMI_DATACOUNTER_DATE_NV_E* date_nv_e)
{

      MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
	
	if ( m_simId == MMI_SmartDS_SIM_ID_1 )
	{
		MMINV_READ(MMI_DATACOUNTER_SIM1_SAVE_SELECT_DATE, date_nv_e, return_value);
		
		if ( return_value == MN_RETURN_FAILURE )
		{
			SCI_MEMSET(date_nv_e, 0x00, sizeof(MMI_DATACOUNTER_DATE_NV_E));
			MMINV_WRITE(MMI_DATACOUNTER_SIM1_SAVE_SELECT_DATE, date_nv_e);
		}
	}
	else if( m_simId == MMI_SmartDS_SIM_ID_2 )
	{
		MMINV_READ(MMI_DATACOUNTER_SIM2_SAVE_SELECT_DATE, date_nv_e, return_value);
		if ( return_value == MN_RETURN_FAILURE )
		{
			SCI_MEMSET(date_nv_e, 0x00, sizeof(MMI_DATACOUNTER_DATE_NV_E));
			MMINV_WRITE(MMI_DATACOUNTER_SIM2_SAVE_SELECT_DATE, date_nv_e);
		}
	}
}

PUBLIC void GetCallLogRecord(
	MN_DUAL_SYS_E dual_sys, 
       BOOLEAN isPeroidSet,
	uint32 mday,
       uint32 mon, 
       uint32 year, 
       MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv)
{
    uint32 cl_sec = 0;
    uint32 sys_sec = 0;
    uint32 sec_send = 0;
    uint32 sec_recv = 0;
    uint32 sec_total = 0;
    uint32 item_id = 0;
    SCI_DATE_T sys_date = {0};
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint8 call_duration_wchar[20] = {0};
    MMI_STRING_T time_str = {0};
    time_str.wstr_ptr = call_duration_wchar;
    time_str.wstr_len = sizeof(call_duration_wchar)/sizeof(wchar);

    if (MMI_SmartDS_SIM_ID_1 == dual_sys)
    {
        item_id = MMI_DATACOUNTER_SIM1_CALL_RECORD;
    }
    else if (MMI_SmartDS_SIM_ID_2 == dual_sys)
    {
        item_id = MMI_DATACOUNTER_SIM2_CALL_RECORD;
    }
    
    MMINV_READ(item_id, record_data_nv, return_value);
    SCI_TRACE_LOW("[MMIDataCounter]GetCallLogRecord ----Read NV return_value= %d", return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(record_data_nv, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        record_data_nv->clearFlag = 0;
        MMINV_WRITE(item_id, &record_data_nv);
    }

    //考虑设置的日期范围不涉汲当前月，只需从NV读取
    TM_GetSysDate(&sys_date);
    if (!(sys_date.year == year && sys_date.mon == mon))//跨月或跨年设置
    {
        if (!isPeroidSet)//排除sim卡插入时间为准
        {
            ;
        }
        else
        {
            return;
        }
    }

     //从nv当中读取，如果原先没有进行数据流量存储，则从接口当中重新获取
   /*if ( record_data_nv->clearFlag == 0 || 
   	record_data_nv->clearFlag == 1 ||
   	record_data_nv->clearFlag == 3 ||
   	record_data_nv->clearFlag == 4)*/
   {
       cl_sec = MMIAPICOM_Tm2Second(0,0,0, mday, mon, year);
	
       sys_sec = MMIAPICOM_GetCurTime();

	//Sent
	SCI_MEMSET(call_duration_wchar, 0x00, sizeof(call_duration_wchar));
       sec_send = MMIAPICL_GetSimCallDurTimeByPeriod(dual_sys, MMICL_CALL_DIALED, cl_sec, sys_sec, &time_str);
       MMIAPICOM_DestroyString(time_str);
       record_data_nv->recordClearSendData = ((sec_send - record_data_nv->pre_sendData) > sec_send ? 0 : record_data_nv->recordClearSendData);
       record_data_nv->pre_sendData = sec_send;
       sec_send = ((sec_send - record_data_nv->recordClearSendData) > sec_send  ? sec_send : (sec_send - record_data_nv->recordClearSendData));
	SCI_MEMSET(record_data_nv->buffer_send_outgoing, 0x00, sizeof(wchar)*20);
	MMIAPICOM_Int2Str(sec_send, call_duration_wchar, sizeof(call_duration_wchar));
       MMIAPICOM_StrToWstr(call_duration_wchar, record_data_nv->buffer_send_outgoing);
       
	//Received
	SCI_MEMSET(call_duration_wchar, 0x00, sizeof(call_duration_wchar));
       sec_recv = MMIAPICL_GetSimCallDurTimeByPeriod(dual_sys, MMICL_CALL_RECEIVED, cl_sec, sys_sec, &time_str);
       MMIAPICOM_DestroyString(time_str);
       record_data_nv->recordClearRecvData = ((sec_recv - record_data_nv->pre_recvData) > sec_recv ? 0 : record_data_nv->recordClearRecvData);
       record_data_nv->pre_recvData = sec_recv;
       sec_recv = ((sec_recv - record_data_nv->recordClearRecvData) > sec_recv  ? sec_recv : (sec_recv - record_data_nv->recordClearRecvData));
	SCI_MEMSET(record_data_nv->buffer_recv_incoming, 0x00, sizeof(wchar)*20);
	MMIAPICOM_Int2Str(sec_recv, call_duration_wchar, sizeof(call_duration_wchar));
       MMIAPICOM_StrToWstr(call_duration_wchar, record_data_nv->buffer_recv_incoming);
        //All
       sec_total = sec_send + sec_recv;
       SCI_MEMSET(record_data_nv->buffer_ALL, 0x00, sizeof(wchar)*20);
	MMIAPICOM_Int2Str(sec_total, call_duration_wchar, sizeof(call_duration_wchar));
       MMIAPICOM_StrToWstr(call_duration_wchar, record_data_nv->buffer_ALL);
       SCI_TRACE_LOW("[MMIDataCounter]GetCallLogRecord ----sec_send=%d, sec_recv=%d , sec_total: %d", sec_send, sec_recv, sec_total);
	GetSIMIccId(dual_sys, record_data_nv->iccid);
	record_data_nv->clearFlag = 1;
       MMINV_WRITE(item_id, record_data_nv);
   }
   
}

PUBLIC void FormatDataFlowrate(MMI_STRING_T *result_str, uint8 data_buff_length, uint32 data_flowrate)
{
    uint8 temp_b = 0;
    float temp_c = 0.00f;
    float temp_d = 0.00f;
    MMI_STRING_T dataTypeStr = {0};

    uint8 dataSize[10] = {0};
    wchar wstrDataSize[10] = {0};

    if (data_flowrate / (1024*1024) >= 1024)//GB
    {
        temp_b = data_flowrate / (1024*1024*1024);
        temp_c = ((float)(data_flowrate % (1024*1024*1024)))/(1024*1024*1024);
        MMI_GetLabelTextByLang(STR_GAL_LIST_GB, &dataTypeStr);
    }
    else
    {
        temp_b = data_flowrate /(1024*1024);//MB
        temp_c = ((float)(data_flowrate %(1024*1024)))/(1024*1024);
        MMI_GetLabelTextByLang(STR_GAL_LIST_MB, &dataTypeStr);
    }
    temp_d = (float)(temp_b + temp_c);
    sprintf(dataSize,"%.2f",temp_d);
    MMIAPICOM_StrToWstr(dataSize, wstrDataSize);
    MMIAPICOM_CustomGenerateReplaceStr(&dataTypeStr, L"%U", wstrDataSize, data_buff_length, result_str);
   
}

LOCAL void ResetDataCounterPeroidDate()
{
     MMI_DATACOUNTER_DATE_NV_E date_nv_e = {0};
     TM_GetSysDate(&date_nv_e.date);
     date_nv_e.dataResetFlag = 1;
     SCI_TRACE_LOW("mmiDataCounter_main.c ---- ResetDataCounterPeroidDate");
     if ( m_simId == MMI_SmartDS_SIM_ID_1 )
     {
           MMINV_WRITE(MMI_DATACOUNTER_SIM1_SAVE_SELECT_DATE, &date_nv_e);
     }
     else if( m_simId == MMI_SmartDS_SIM_ID_2 )
     {
           MMINV_WRITE(MMI_DATACOUNTER_SIM2_SAVE_SELECT_DATE, &date_nv_e);
     }
}

PUBLIC void ClearDataCounterPartRecord(uint8 clearFlag, MN_DUAL_SYS_E sim_card)
{
    ClearDataNV(clearFlag, sim_card);
    ClearCallNV(clearFlag, sim_card);
    ClearSmsNV(clearFlag, sim_card);
    ClearMmsNV(clearFlag, sim_card);
}

PUBLIC void ClearDataCounterPeriodAllRecord(uint8 clearFlag, MN_DUAL_SYS_E sim_card)
{
    ClearDataNV(clearFlag, sim_card);//清除数据业务流量
    ClearCallNV(clearFlag, sim_card);//清除call记录
    ClearSmsNV(clearFlag, sim_card);//清除sms记录

    ClearMmsNV(clearFlag, sim_card);//清除mms记录

    ResetDataCounterPeroidDate();
}

PUBLIC void ClearSmsNV(uint8 clearFlag, MN_DUAL_SYS_E sim_card)
{
     MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMI_DATACOUNTER_RECORD_DATA_NV_E* temp_record_data = (MMI_DATACOUNTER_RECORD_DATA_NV_E*)SCI_ALLOCAZ(sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
    uint32 item_id = 0;
    uint32 data_size = 0;
    MMI_DATACOUNTER_RECORD_DATA_NV_E record_data_nv = {0};
    record_data_nv.clearFlag = clearFlag;

    MMIAPICOM_Wstrcpy(record_data_nv.buffer_ALL, L"0");
    MMIAPICOM_Wstrcpy(record_data_nv.buffer_send_outgoing, L"0");
    MMIAPICOM_Wstrcpy(record_data_nv.buffer_recv_incoming, L"0");
    MMIAPICOM_Wstrcpy(record_data_nv.iccid, L"0");
    
    if (MMI_SmartDS_SIM_ID_1 == sim_card)
    {
        item_id = MMI_DATACOUNTER_SIM1_SMS_RECORD;
    }
    else if( MMI_SmartDS_SIM_ID_2 == sim_card )
    {
        item_id = MMI_DATACOUNTER_SIM2_SMS_RECORD;
    }

    if (temp_record_data == PNULL)
    {
        return;
    }

    SCI_MEMSET(temp_record_data, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
    
    if (clearFlag != 4)
    {
        MMINV_READ(item_id, temp_record_data, return_value);
        if ( return_value != MN_RETURN_SUCCESS )
        {
            SCI_MEMSET(temp_record_data, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        }
    }
 
    MMIAPICOM_WstrToNum(temp_record_data->buffer_send_outgoing, MMIAPICOM_Wstrlen(temp_record_data->buffer_send_outgoing), &data_size);    
    record_data_nv.recordClearSendData = data_size + temp_record_data->recordClearSendData;
    record_data_nv.pre_sendData = temp_record_data->pre_sendData;

    data_size = 0;
    MMIAPICOM_WstrToNum(temp_record_data->buffer_recv_incoming, MMIAPICOM_Wstrlen(temp_record_data->buffer_recv_incoming), &data_size);    
    record_data_nv.recordClearRecvData = data_size + temp_record_data->recordClearRecvData;
    record_data_nv.pre_recvData = temp_record_data->pre_recvData;
    MMIAPICOM_Wstrcpy(record_data_nv.iccid, temp_record_data->iccid);
    
    MMINV_WRITE(item_id, &record_data_nv);
    
    ResetSmsShowJustOne(sim_card, 0, FALSE);

    if ( temp_record_data != PNULL )
    {
        SCI_FREE(temp_record_data);
        temp_record_data = PNULL;
    }
     
}

PUBLIC void ClearMmsNV(uint8 clearFlag, MN_DUAL_SYS_E sim_card)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMI_DATACOUNTER_RECORD_DATA_NV_E* temp_record_data = (MMI_DATACOUNTER_RECORD_DATA_NV_E*)SCI_ALLOCAZ(sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
    uint32 item_id = 0;
    uint32 data_size = 0;
    MMI_DATACOUNTER_RECORD_DATA_NV_E record_data_nv = {0};
    record_data_nv.clearFlag = clearFlag;
    MMIAPICOM_Wstrcpy(record_data_nv.buffer_ALL, L"0");
    MMIAPICOM_Wstrcpy(record_data_nv.buffer_send_outgoing, L"0");
    MMIAPICOM_Wstrcpy(record_data_nv.buffer_recv_incoming, L"0");
    MMIAPICOM_Wstrcpy(record_data_nv.iccid, L"0");
    
    if ( MMI_SmartDS_SIM_ID_1 == sim_card )
    {
        item_id = MMI_DATACOUNTER_SIM1_MMS_RECORD;
    }
    else if( MMI_SmartDS_SIM_ID_2 == sim_card )
    {
        item_id = MMI_DATACOUNTER_SIM2_MMS_RECORD;
    }

    if ( temp_record_data  == PNULL )
    {
        return;
    }

    SCI_MEMSET(temp_record_data, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
    if ( clearFlag != 4 )
    {
        MMINV_READ(item_id, temp_record_data, return_value);
        if ( return_value != MN_RETURN_SUCCESS )
        {
            SCI_MEMSET(temp_record_data, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        }
    }

    MMIAPICOM_WstrToNum(temp_record_data->buffer_send_outgoing, MMIAPICOM_Wstrlen(temp_record_data->buffer_send_outgoing), &data_size);    
    record_data_nv.recordClearSendData = data_size + temp_record_data->recordClearSendData;
    record_data_nv.pre_sendData = temp_record_data->pre_sendData;

    data_size = 0;
    MMIAPICOM_WstrToNum(temp_record_data->buffer_recv_incoming, MMIAPICOM_Wstrlen(temp_record_data->buffer_recv_incoming), &data_size);    
    record_data_nv.recordClearRecvData = data_size + temp_record_data->recordClearRecvData;
    record_data_nv.pre_recvData = temp_record_data->pre_recvData;
    
    MMIAPICOM_Wstrcpy(record_data_nv.iccid, temp_record_data->iccid);
    
    MMINV_WRITE(item_id, &record_data_nv);
    ResetMmsShowJustOne(sim_card, 0, FALSE);

    if ( temp_record_data != PNULL )
    {
        SCI_FREE(temp_record_data);
        temp_record_data = PNULL;
    }
}

PUBLIC void ClearCallNV(uint8 clearFlag, MN_DUAL_SYS_E sim_card)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMI_DATACOUNTER_RECORD_DATA_NV_E record_data_nv = {0};
    MMI_DATACOUNTER_RECORD_DATA_NV_E* temp_record_data = (MMI_DATACOUNTER_RECORD_DATA_NV_E*)SCI_ALLOCAZ(sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
    uint32 item_id = 0;
    uint32 data_size = 0;
    record_data_nv.clearFlag = clearFlag;
    
    if ( MMI_SmartDS_SIM_ID_1 == sim_card )
    {
        item_id = MMI_DATACOUNTER_SIM1_CALL_RECORD;
    }
    else if( MMI_SmartDS_SIM_ID_2 == sim_card )
    {
        item_id = MMI_DATACOUNTER_SIM2_CALL_RECORD;
    }

    if ( temp_record_data  == PNULL )
    {
        return;
    }
    
    SCI_MEMSET(temp_record_data, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));

    if ( clearFlag != 4 )
    {
        MMINV_READ(item_id, temp_record_data, return_value);
        if ( return_value != MN_RETURN_SUCCESS )
        {
            SCI_MEMSET(temp_record_data, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        }
    }

    MMIAPICOM_WstrToNum(temp_record_data->buffer_send_outgoing, MMIAPICOM_Wstrlen(temp_record_data->buffer_send_outgoing), &data_size);    
    record_data_nv.recordClearSendData = data_size + temp_record_data->recordClearSendData;
    record_data_nv.pre_sendData = temp_record_data->pre_sendData;
    data_size = 0;
    MMIAPICOM_WstrToNum(temp_record_data->buffer_recv_incoming, MMIAPICOM_Wstrlen(temp_record_data->buffer_recv_incoming), &data_size);    
    record_data_nv.recordClearRecvData = data_size + temp_record_data->recordClearRecvData;
    record_data_nv.pre_recvData = temp_record_data->pre_recvData;
    MMIAPICOM_Wstrcpy(record_data_nv.iccid, temp_record_data->iccid);
    
    MMINV_WRITE(item_id, &record_data_nv);

    ResetCallShowJustOne(sim_card, 0, FALSE);//同样清除第一次弹框问题

    if ( temp_record_data != PNULL )
    {
        SCI_FREE(temp_record_data);
        temp_record_data = PNULL;
    }

}

PUBLIC void ClearDataNV(uint8 clearFlag, MN_DUAL_SYS_E sim_card)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMI_DATACOUNTER_RECORD_DATA_NV_E record_data_nv = {0};
    MMI_DATACOUNTER_RECORD_DATA_NV_E* temp_record_data = (MMI_DATACOUNTER_RECORD_DATA_NV_E*)SCI_ALLOCAZ(sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
    uint32 item_id = 0;
    uint32 data_size = 0;
   
    record_data_nv.clearFlag = clearFlag;

    if ( MMI_SmartDS_SIM_ID_1 == sim_card )
     {
        item_id = MMI_DATACOUNTER_SIM1_DATA_RECORD;
     }
     else if( MMI_SmartDS_SIM_ID_2 == sim_card )
     {
        item_id = MMI_DATACOUNTER_SIM2_DATA_RECORD;
     }

    if ( temp_record_data == PNULL )
    {
        return;
    }

    SCI_MEMSET(temp_record_data, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
    if ( clearFlag != 4 )
    {
        MMINV_READ(item_id, temp_record_data, return_value);

        if ( return_value != MN_RETURN_SUCCESS )
        {
            SCI_MEMSET(temp_record_data, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        }
    }

    MMIAPICOM_WstrToNum(temp_record_data->buffer_send_outgoing, MMIAPICOM_Wstrlen(temp_record_data->buffer_send_outgoing), &data_size);    
    record_data_nv.recordClearSendData = data_size + temp_record_data->recordClearSendData;
    record_data_nv.pre_sendData = temp_record_data->pre_sendData;
    data_size = 0;
    MMIAPICOM_WstrToNum(temp_record_data->buffer_recv_incoming, MMIAPICOM_Wstrlen(temp_record_data->buffer_recv_incoming), &data_size);    
    record_data_nv.recordClearRecvData = data_size + temp_record_data->recordClearRecvData;
    record_data_nv.pre_recvData = temp_record_data->pre_recvData;
    MMIAPICOM_Wstrcpy(record_data_nv.iccid, temp_record_data->iccid);
    //清除数据
    MMINV_WRITE(item_id, &record_data_nv);
    ResetDataShowJustOne(sim_card, 0, FALSE);

    if ( temp_record_data != PNULL )
    {
        SCI_FREE(temp_record_data);
        temp_record_data = PNULL;
    }
}

LOCAL uint32 MMIAPIDataCounter_readDataNV(MN_DUAL_SYS_E simCard, MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv)
{
    uint32 item_id = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    if ( MMI_SmartDS_SIM_ID_1 == simCard )
    {
        item_id = MMI_DATACOUNTER_SIM1_DATA_RECORD;
    }
    else if( MMI_SmartDS_SIM_ID_2 == simCard )
    {
        item_id = MMI_DATACOUNTER_SIM2_DATA_RECORD;
    }
    
    MMINV_READ(item_id, record_data_nv, return_value);
   
    if ( return_value != MN_RETURN_SUCCESS )
    {
        SCI_MEMSET(record_data_nv, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        record_data_nv->clearFlag = 0;
        MMINV_WRITE(item_id, record_data_nv);
    }

    return item_id;	
    
}
PUBLIC void LoadDataNV(MN_DUAL_SYS_E simCard, BOOLEAN isPeroidSet, uint8 nDate,
	uint16 nYear, uint8 nMonth, uint8 monthDay, MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv)
{
    uint32 i = 0; 
    uint32 item_id  = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    uint32 nSumData_flowrate = 0;
    SCI_DATE_T sys_date = {0};
    uint32 total_send = 0;
    uint32 total_rev = 0;
    
    MMIPDP_FLOWRATE_T   flowrate_to_write_arr[MMI_DUAL_SYS_MAX] = {0};    
    uint8 data_buff[15] = {0};

   item_id = MMIAPIDataCounter_readDataNV(simCard, record_data_nv);
    //考虑设置的日期范围不涉汲当前月，只需从NV读取
    TM_GetSysDate(&sys_date);
    if ( !(sys_date.year == nYear && sys_date.mon == nMonth) )//跨月或跨年设置
    {
        if ( !isPeroidSet )//排除sim卡插入时间为准
        {
            ;
        }
        else
        {
            return;
        }
    }
    //从nv当中读取，如果原先没有进行数据流量存储，则从接口当中重新获取
  /* if ( record_data_nv->clearFlag == 0 || 
   	record_data_nv->clearFlag == 1 ||
   	record_data_nv->clearFlag == 3 ||
   	record_data_nv->clearFlag == 4)*/
   {
        MMINV_READ(MMINV_PDP_GPRS_FLOW_RATE,flowrate_to_write_arr,return_value);
	 SCI_MEMSET(data_buff, 0x00, sizeof(data_buff));
        if ( !isPeroidSet )
	 {
            if ( flowrate_to_write_arr[simCard].nYear >= nYear )
            {
                //Sent
                total_send = flowrate_to_write_arr[simCard].total_send;
                //Recv
                total_rev = flowrate_to_write_arr[simCard].total_recv;
                //All
                nSumData_flowrate = total_send + total_rev;
            }
            else
            {
                total_send = 0;
                total_rev = 0;
                nSumData_flowrate = total_send + total_rev;
            }
	 }
	else
	{
	      if ( flowrate_to_write_arr[simCard].nYear == nYear
			&& flowrate_to_write_arr[simCard].nMonth == nMonth)
		{
                 for( i = nDate; i <= monthDay; ++i )
		    {
                      total_send += flowrate_to_write_arr[simCard].month_send_data_array[i-1];
                      total_rev += flowrate_to_write_arr[simCard].month_recv_data_array[i-1];
		    }
		    nSumData_flowrate = total_send + total_rev;
		}
		else
		{
		   total_send = 0;
		   total_rev = 0;
                nSumData_flowrate = total_send + total_rev;
		}
	 }

        //Sent
        record_data_nv->recordClearSendData = ((total_send - record_data_nv->pre_sendData) > total_send ? 0 : record_data_nv->recordClearSendData);
        record_data_nv->pre_sendData = total_send;
        total_send = ((total_send - record_data_nv->recordClearSendData) > total_send ? total_send : (total_send - record_data_nv->recordClearSendData));
        SCI_MEMSET(data_buff, 0x00, sizeof(data_buff));
        MMIAPICOM_Int2Str(total_send, data_buff, sizeof(data_buff));
        MMIAPICOM_StrToWstr(data_buff, record_data_nv->buffer_send_outgoing);
        //Received
        record_data_nv->recordClearRecvData = ((total_rev - record_data_nv->pre_recvData) > total_rev ? 0 : record_data_nv->recordClearRecvData);
        record_data_nv->pre_recvData = total_rev;
        total_rev = ((total_rev - record_data_nv->recordClearRecvData) >total_rev ? total_rev: (total_rev -record_data_nv->recordClearRecvData));
        SCI_MEMSET(data_buff, 0x00, sizeof(data_buff));
        MMIAPICOM_Int2Str(total_rev, data_buff, sizeof(data_buff));
        MMIAPICOM_StrToWstr(data_buff, record_data_nv->buffer_recv_incoming);

         //ALL
        nSumData_flowrate  =  total_send + total_rev;
        MMIAPICOM_Int2Str(nSumData_flowrate, data_buff, sizeof(data_buff));
        MMIAPICOM_StrToWstr(data_buff, record_data_nv->buffer_ALL);

        GetSIMIccId(simCard, record_data_nv->iccid);

        record_data_nv->clearFlag = 1;
        MMINV_WRITE(item_id, record_data_nv);
   }

}

#ifdef MMI_SMS_CHAT_SUPPORT
PUBLIC void MMIDATACounter_GetSmsRecord(
                                        MN_DUAL_SYS_E dual_sys,
                                        BOOLEAN isPeroidSet,
                                        uint8 mday,
                                        uint8 mon,
                                        uint16 year,
                                        MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv
                                        )
{
    uint32 cl_sec = 0;
    uint32 sys_sec = 0;
    uint32 item_id = 0;
    SCI_DATE_T sys_date = {0};
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint8 num_buff[10] = {0};
    wchar num_dst[10] = {0};
    MMI_STRING_T time_str = {0};
    uint32 sent_num = 0;
    uint32 recv_num = 0;
    uint32 total_num = 0;
    
    if (MMI_SmartDS_SIM_ID_1 == dual_sys)
    {
        item_id = MMI_DATACOUNTER_SIM1_SMS_RECORD;
    }
    else if (MMI_SmartDS_SIM_ID_2 == dual_sys)
    {
        item_id = MMI_DATACOUNTER_SIM2_SMS_RECORD;
    }
    
    MMINV_READ(item_id, record_data_nv, return_value);
    if (return_value != MN_RETURN_SUCCESS)
    {
        SCI_MEMSET(record_data_nv, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        record_data_nv->clearFlag = 0;
        MMINV_WRITE(item_id, record_data_nv);
    }

    //考虑设置的日期范围不涉汲当前月，只需从NV读取
    TM_GetSysDate(&sys_date);
    if (!(sys_date.year == year && sys_date.mon == mon))//跨月或跨年设置
    {
        if ( !isPeroidSet )//排除sim卡插入时间为准
        {
            ;
        }
        else
        {
            return;
        }
    }
    
     //从nv当中读取，如果原先没有进行数据流量存储，则从接口当中重新获取
   /*if ( record_data_nv->clearFlag == 0 || 
   	record_data_nv->clearFlag == 1 ||
   	record_data_nv->clearFlag == 3 ||
   	record_data_nv->clearFlag == 4)*/
   {
       cl_sec = MMIAPICOM_Tm2Second(0,0,0, mday, mon, year);
	
       sys_sec = MMIAPICOM_GetCurTime();

       //Sent
       sent_num = MMISMS_GetChatMessageNumbyConditions(
                    cl_sec,
                    sys_sec,
                    MMISMS_TYPE_SMS,
                    MMISMS_MO_TYPE,
                    dual_sys
                    );

        record_data_nv->recordClearSendData = ((sent_num - record_data_nv->pre_sendData) > sent_num ? 0 : record_data_nv->recordClearSendData);
        record_data_nv->pre_sendData = sent_num;
        sent_num = ((sent_num - record_data_nv->recordClearSendData) > sent_num ? sent_num : (sent_num - record_data_nv->recordClearSendData));
        MMIAPICOM_Int2Str(sent_num, num_buff, sizeof(num_buff));
        SCI_MEMSET(num_dst, 0x00, sizeof(wchar)*10);
        MMIAPICOM_StrToWstr(num_buff, num_dst);
        SCI_MEMSET(record_data_nv->buffer_send_outgoing, 0x00, sizeof(wchar)*20);
        MMIAPICOM_Wstrcpy(record_data_nv->buffer_send_outgoing, num_dst);

	//Recv
	recv_num = MMISMS_GetChatMessageNumbyConditions(
                    cl_sec,
                    sys_sec,
                    MMISMS_TYPE_SMS,
                    MMISMS_MT_TYPE,
                    dual_sys
                    );
        
       record_data_nv->recordClearRecvData = ((recv_num - record_data_nv->pre_recvData) > recv_num ? 0 : record_data_nv->recordClearRecvData);
       record_data_nv->pre_recvData = recv_num;
       recv_num = ((recv_num - record_data_nv->recordClearRecvData) > recv_num ? recv_num : (recv_num - record_data_nv->recordClearRecvData));
       SCI_MEMSET(num_buff, 0x00, sizeof(num_buff));
       MMIAPICOM_Int2Str(recv_num, num_buff, sizeof(num_buff));
       SCI_MEMSET(num_dst, 0x00, sizeof(wchar)*10);
       MMIAPICOM_StrToWstr(num_buff, num_dst);
	SCI_MEMSET(record_data_nv->buffer_recv_incoming, 0x00, sizeof(wchar)*20);
	MMIAPICOM_Wstrcpy(record_data_nv->buffer_recv_incoming, num_dst);

      //All
      total_num = sent_num + recv_num;
      SCI_MEMSET(num_buff, 0x00, sizeof(num_buff));
       MMIAPICOM_Int2Str(total_num, num_buff, sizeof(num_buff));
       SCI_MEMSET(num_dst, 0x00, sizeof(wchar)*10);
       MMIAPICOM_StrToWstr(num_buff, num_dst);
	SCI_MEMSET(record_data_nv->buffer_ALL, 0x00, sizeof(wchar)*20);
	MMIAPICOM_Wstrcpy(record_data_nv->buffer_ALL, num_dst);

#ifdef MMI_MULTI_SIM_SYS_SINGLE
    dual_sys = MN_DUAL_SYS_1;
#endif

    GetSIMIccId(dual_sys, record_data_nv->iccid);
	record_data_nv->clearFlag = 1;
       MMINV_WRITE(item_id, record_data_nv);
   }
}

PUBLIC void MMIDATACounter_GetMmsRecord(MN_DUAL_SYS_E dual_sys, BOOLEAN isPeroidSet,
	uint8 mday,
       uint8 mon, 
       uint16 year, 
       MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv)
{
    uint32 cl_sec = 0;
    uint32 sys_sec = 0;
    uint32 item_id = 0;
    SCI_DATE_T sys_date = {0};
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint8 num_buff[10] = {0};
    wchar num_dst[10] = {0};
    MMI_STRING_T time_str = {0};
    uint32 sent_num = 0;
    uint32 recv_num = 0;
    uint32 total_num = 0;

    if ( MMI_SmartDS_SIM_ID_1 == dual_sys )
   {
        item_id = MMI_DATACOUNTER_SIM1_MMS_RECORD;
   }
    else if( MMI_SmartDS_SIM_ID_2 == dual_sys )
     {
        item_id = MMI_DATACOUNTER_SIM2_MMS_RECORD;
    }
    MMINV_READ(item_id, record_data_nv, return_value);
    if ( return_value != MN_RETURN_SUCCESS )
    {
        SCI_MEMSET(record_data_nv, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        record_data_nv->clearFlag = 0;
        MMINV_WRITE(item_id, &record_data_nv);
    }

    //考虑设置的日期范围不涉汲当前月，只需从NV读取
    TM_GetSysDate(&sys_date);
     if ( !(sys_date.year == year && sys_date.mon == mon) )//跨月或跨年设置
    {
        if ( !isPeroidSet )//排除sim卡插入时间为准
        {
            ;
        }
        else
        {
            return;
        }
    }
    
        //从nv当中读取，如果原先没有进行数据流量存储，则从接口当中重新获取
   /* if ( record_data_nv->clearFlag == 0 || 
    record_data_nv->clearFlag == 1 ||
    record_data_nv->clearFlag == 3 ||
    record_data_nv->clearFlag == 4)*/
    {
        cl_sec = MMIAPICOM_Tm2Second(0,0,0, mday, mon, year);

        sys_sec = MMIAPICOM_GetCurTime();

        //Sent
        sent_num = MMISMS_GetChatMessageNumbyConditions(
                    cl_sec,
                    sys_sec,
                    MMISMS_TYPE_MMS,
                    MMISMS_MO_TYPE,
                    dual_sys
                    );
        
        record_data_nv->recordClearSendData = ((sent_num - record_data_nv->pre_sendData) > sent_num ? 0 : record_data_nv->recordClearSendData);
        record_data_nv->pre_sendData = sent_num;
        sent_num = ((sent_num - record_data_nv->recordClearSendData) > sent_num ? sent_num : (sent_num - record_data_nv->recordClearSendData));
        MMIAPICOM_Int2Str(sent_num, num_buff, sizeof(num_buff));
        SCI_MEMSET(num_dst, 0x00, sizeof(wchar)*10);
        MMIAPICOM_StrToWstr(num_buff, num_dst);
        SCI_MEMSET(record_data_nv->buffer_send_outgoing, 0x00, sizeof(wchar)*20);
        MMIAPICOM_Wstrcpy(record_data_nv->buffer_send_outgoing, num_dst);
        //Recv
        recv_num = MMISMS_GetChatMessageNumbyConditions(
                    cl_sec,
                    sys_sec,
                    MMISMS_TYPE_MMS,
                    MMISMS_MT_TYPE,
                    dual_sys
                    );
        
        record_data_nv->recordClearRecvData = ((recv_num - record_data_nv->pre_recvData) > recv_num ? 0 : record_data_nv->recordClearRecvData);
        record_data_nv->pre_recvData = recv_num;
        recv_num = ((recv_num - record_data_nv->recordClearRecvData) > recv_num ? recv_num : (recv_num - record_data_nv->recordClearRecvData));
        SCI_MEMSET(num_buff, 0x00, sizeof(num_buff));
        MMIAPICOM_Int2Str(recv_num, num_buff, sizeof(num_buff));
        SCI_MEMSET(num_dst, 0x00, sizeof(wchar)*10);
        MMIAPICOM_StrToWstr(num_buff, num_dst);
        SCI_MEMSET(record_data_nv->buffer_recv_incoming, 0x00, sizeof(wchar)*20);
        MMIAPICOM_Wstrcpy(record_data_nv->buffer_recv_incoming, num_dst);

        //All
        total_num = sent_num + recv_num;
        SCI_MEMSET(num_buff, 0x00, sizeof(num_buff));
        MMIAPICOM_Int2Str(total_num, num_buff, sizeof(num_buff));
        SCI_MEMSET(num_dst, 0x00, sizeof(wchar)*10);
        MMIAPICOM_StrToWstr(num_buff, num_dst);
        SCI_MEMSET(record_data_nv->buffer_ALL, 0x00, sizeof(wchar)*20);
        MMIAPICOM_Wstrcpy(record_data_nv->buffer_ALL, num_dst);

        GetSIMIccId(dual_sys, record_data_nv->iccid);
        record_data_nv->clearFlag = 1;
        MMINV_WRITE(item_id, record_data_nv);
    }
}
#endif

LOCAL void MMIDataCounter_OptionItems(MN_DUAL_SYS_E simCard)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMI_CTRL_ID_T ctrl_id = MMI_DATACOUNTER_SIM_RESULT_CTRL_ID;
    MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv = PNULL; 
    MMI_STRING_T since_label = {0};
    wchar buffer[30] = {0};
    wchar replace_str[20] = {0};
    wchar str_buff[20] = {0};
    uint8 since_str[8] = {0};
    uint8 str_month[5] = {0};
    GUILIST_ITEM_T item_t = {0};
    MMI_STRING_T str = {0};
    uint32 data_size = 0;
    uint32 call_sec = 0;
    wchar call_buffer[20] = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16 nYear = 0;
    uint8 nMonth = 0;
    uint8 nDate = 0;
    uint8 monthDay = 0;
    BOOLEAN isPeroidSet = FALSE;
    MMI_DATACOUNTER_DATE_NV_E date_nv_e = {0};
    MMI_RECORD_INSERT_SIM_CARD_T recordInsertSimInfo[MMI_DUAL_SYS_MAX] = {0};
    item_t.item_data_ptr = &item_data;

    record_data_nv = (MMI_DATACOUNTER_RECORD_DATA_NV_E*)SCI_ALLOCAZ(sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
    if (PNULL != record_data_nv)
    {
        GUILIST_SetMaxItem(ctrl_id, MMI_DATACOUNTER_RESULT_ITEM_NUM,FALSE);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        ReadResetPeriodDate(&date_nv_e);
        
        str.wstr_ptr = buffer;
        str.wstr_len = sizeof(buffer)/sizeof(wchar);
        
        if ( date_nv_e.date.year == 0 &&  date_nv_e.date.mon == 0 && date_nv_e.date.mday == 0  )//未设置选择日期
        {
            MMINV_READ(MMI_NV_INSERT_SIM_TIME, &recordInsertSimInfo, return_value);
            nYear = recordInsertSimInfo[simCard].recordInsertSimTime.year;
            nMonth = recordInsertSimInfo[simCard].recordInsertSimTime.mon;
            nDate = recordInsertSimInfo[simCard].recordInsertSimTime.mday;
            monthDay = MMICALENDAR_GetMonthday(nYear, nMonth);
        
            SCI_MEMSET(str_month, 0x00, sizeof(str_month));
            GetDataCounter_month(nMonth-1, str_month);
            
            //MMIAPISET_FormatDateStrByDateAndMonStrEx(nYear, nMonth, nDate,'/', str_buff, 20, TRUE);
            MMIAPISET_FormatDateStrByDateEx(nYear, nMonth, nDate,'/', str_buff, 20, TRUE);
        
            //sprintf(str_buff, "%s%s%d%s%d", str_month, " ", nDate, " '", nYear);
            MMIAPICOM_StrToWstr(str_buff, replace_str);
            MMIAPICOM_CustomGenerateReplaceStrByTextId(STR_COUN_PERIOD_SINCE_EXT01, L"%U", replace_str, str.wstr_len, &str);
            isPeroidSet = FALSE;
        }
        else
        {
            SCI_TRACE_LOW("mmiDataCounter_result.c ---- MMIDataCounter_OptionItems SET ResetFlag= %d",date_nv_e.dataResetFlag);
            nYear = date_nv_e.date.year;
            nMonth = date_nv_e.date.mon;
            nDate = date_nv_e.date.mday;
            monthDay = MMICALENDAR_GetMonthday(nYear, nMonth);
            SCI_MEMSET(str_month, 0x00, sizeof(str_month));
            GetDataCounter_month(nMonth-1, str_month);
            if ( date_nv_e.dataResetFlag )
            {
                //MMIAPISET_FormatDateStrByDateAndMonStrEx(nYear, nMonth, nDate,'/', str_buff, 20, TRUE);
                MMIAPISET_FormatDateStrByDateEx(nYear, nMonth, nDate,'/', str_buff, 20, TRUE);
                //sprintf(str_buff, "%s%s%d%s%d", str_month, " ", nDate, " '", nYear);
                MMIAPICOM_StrToWstr(str_buff, replace_str);
                MMIAPICOM_CustomGenerateReplaceStrByTextId(STR_COUN_PERIOD_SINCE_EXT01, L"%U", replace_str, str.wstr_len, &str);
            }
            else
            {
                //MMIAPISET_FormatDateStrByDateAndMonStrEx(nYear, nMonth, nDate,'/', str_buff, 20, TRUE);
                MMIAPISET_FormatDateStrByDateEx(nYear, nMonth, nDate,'/', str_buff, 20, TRUE);
                //sprintf(str_buff, "%s%s%d%s%s%s%d%s%d", str_month, " ", nDate, " - ", str_month, " ", monthDay, " '", nYear);
                MMIAPICOM_StrToWstr(str_buff, buffer);
            }
            
            isPeroidSet = TRUE;
        }
        
        
        str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);
        //set item "Counter period"
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
        item_t.user_data = MMI_DATACOUNTER_PERIOD;
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = STR_COUN_LIST_PERIOD_EXT01;
        item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer = str;
        //item_data.item_content[1].is_opacity = TRUE;
        //GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
        GUILIST_InsertItem(ctrl_id, &item_t, MMI_DATACOUNTER_PERIOD);
        item_data.item_content[1].is_opacity = FALSE;
        //set item "data"
        SCI_MEMSET(record_data_nv, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        LoadDataNV(simCard, isPeroidSet, nDate, nYear, nMonth, monthDay, record_data_nv);
        MMIAPICOM_WstrToNum(record_data_nv->buffer_ALL, MMIAPICOM_Wstrlen(record_data_nv->buffer_ALL), &data_size);
        SCI_MEMSET(str_buff, 0x00, sizeof(str_buff));
        str.wstr_ptr = str_buff;
        FormatDataFlowrate(&str, sizeof(str_buff)/sizeof(wchar), data_size);
        str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);
        
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_AND_TEXT_MS_EXT;
        item_t.user_data = MMI_DATACOUNTER_DATA;
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = STR_SIM_MANAGER_DATA;
        item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer = str;
        GUILIST_InsertItem(ctrl_id, &item_t, MMI_DATACOUNTER_DATA);
        
        //set item "call"
        SCI_MEMSET(record_data_nv, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        GetCallLogRecord(simCard, isPeroidSet, nDate, nMonth, nYear, record_data_nv);
        MMIAPICOM_WstrToNum(record_data_nv->buffer_ALL, MMIAPICOM_Wstrlen(record_data_nv->buffer_ALL), &call_sec);
        
        //str.wstr_ptr = call_buffer;
        //str.wstr_len = sizeof(call_buffer)/sizeof(wchar);
        MMIDataCounter_FormatTimeStrByTime(call_sec, &str);
        
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_AND_TEXT_MS_EXT;
        item_t.user_data = MMI_DATACOUNTER_CALL;
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = STR_SIM_MANAGER_CALL;
        item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer = str;
        GUILIST_InsertItem(ctrl_id, &item_t, MMI_DATACOUNTER_CALL);
        MMIAPICOM_DestroyString(str);
        
#ifdef MMI_SMS_CHAT_SUPPORT
        //set item "SMS"
        SCI_MEMSET(record_data_nv, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        MMIDATACounter_GetSmsRecord(simCard, isPeroidSet, nDate, nMonth, nYear, record_data_nv);
        str.wstr_ptr = record_data_nv->buffer_ALL;
        str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_AND_TEXT_MS_EXT;
        item_t.user_data = MMI_DATACOUNTER_SMS;
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = STR_SIM_MANAGER_SMS;
        item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer = str;
        GUILIST_InsertItem(ctrl_id, &item_t, MMI_DATACOUNTER_SMS);
        
        //set item "MMS"
        SCI_MEMSET(record_data_nv, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        MMIDATACounter_GetMmsRecord(simCard, isPeroidSet, nDate, nMonth, nYear, record_data_nv);
        str.wstr_ptr = record_data_nv->buffer_ALL;
        str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_AND_TEXT_MS;
        item_t.user_data = MMI_DATACOUNTER_MMS;
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = STR_SIM_MANAGER_MMS;
        item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer = str;
        GUILIST_InsertItem(ctrl_id, &item_t, MMI_DATACOUNTER_MMS);
#endif

        if(TRUE == MMIAPISET_IsDisableShareViaOption())
        {
                CTRLLIST_RemoveItem(ctrl_id,	MMI_DATACOUNTER_MMS );				                
                CTRLLIST_RemoveItem(ctrl_id,	MMI_DATACOUNTER_SMS  );
                CTRLLIST_RemoveItem(ctrl_id,	MMI_DATACOUNTER_DATA );
        }
		
        SCI_FREE(record_data_nv);
        record_data_nv = PNULL;
    }

    return;
}

LOCAL MMI_RESULT_E HandleDataCounterResultWinMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_MENU_ID_T menu_id = 0;
    MMI_MENU_ID_T cur_item = 0;
    MN_DUAL_SYS_E simID = m_simId;
    MMI_STRING_T    statusBar_str = {0};
    const GUILIST_ITEM_T * list_item = PNULL;
    switch(msg_id)
    {
       case MSG_OPEN_WINDOW:
		MMK_SetAtvCtrl(win_id, MMI_DATACOUNTER_SIM_RESULT_CTRL_ID);
	   	break;
       case MSG_FULL_PAINT: 
                MMIDataCounter_OptionItems(simID);
                CTRLLIST_SetCurItemIndex(MMI_DATACOUNTER_SIM_RESULT_CTRL_ID, item_select_state);

                #ifdef MMI_MULTI_SIM_SYS_SINGLE
                    MMI_GetLabelTextByLang(STR_LIST_SIM_EXT01, &statusBar_str);
                #else
                    if ( simID == MMI_SmartDS_SIM_ID_1 )
                    {
                    MMI_GetLabelTextByLang(STR_ILIST_SIM_ONE, &statusBar_str);
                    }
                    else if( simID == MMI_SmartDS_SIM_ID_2 )
                    {
                    MMI_GetLabelTextByLang(STR_ILIST_SIM_TWO, &statusBar_str);
                    } 
                #endif

                if(MMIAPICOM_StatusAreaSetViewHeaderinfo(statusBar_str.wstr_ptr, statusBar_str.wstr_len)) 
                {
                    GUIWIN_UpdateStb();
                }
            break;
       case MSG_KEYUP_DOWN:
       case MSG_KEYUP_UP:
                cur_item = GUILIST_GetCurItemIndex(MMI_DATACOUNTER_SIM_RESULT_CTRL_ID);
                item_select_state = cur_item;
		break;
		
	   case MSG_APP_OK:
	      break;
	   case MSG_CTL_MIDSK:
	   	cur_item = GUILIST_GetCurItemIndex(MMI_DATACOUNTER_SIM_RESULT_CTRL_ID);
		CTRLLIST_GetItemData(MMI_DATACOUNTER_SIM_RESULT_CTRL_ID,cur_item,&menu_id);
		switch(menu_id)
		{
			case MMI_DATACOUNTER_PERIOD:
				MMIAPIDATACOUNTER_OpenPeroidWin(&simID);
				break;
			case MMI_DATACOUNTER_DATA:
				MMIAPIDATACOUNTER_OpenDataWin(&simID);
				break;
			case MMI_DATACOUNTER_CALL:
				MMIAPIDATACOUNTER_OpenCallWin(&simID);
				break;
			case MMI_DATACOUNTER_SMS:
				MMIAPIDATACOUNTER_OpenSmsWin(&simID);
				break;
			case MMI_DATACOUNTER_MMS:
				MMIAPIDATACOUNTER_OpenMmsWin(&simID);
				break;
			default:
				;
				break;

		}
	   	break;

       case MSG_CTL_OK:	
		MMK_CreateWin((uint32*)MMITEST_CLEAR_ALL_COUNTER_WIN_TAB,PNULL);
	   	break;
	case MSG_APP_RED:
       case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
	     MMK_CloseWin(win_id);
            item_select_state = 0;
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;

    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : MMIAPIDATACOUNTER_OpenResultWin
//         
//	Global resource dependence : 
//   Author:        zhanglu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIDATACOUNTER_OpenResultWin(void* param)
{

   m_simId = *((MN_DUAL_SYS_E*)param);
   MMK_CreateWin((uint32*)DATA_COUNTER_WIN_RESULT_TAB,(ADD_DATA)param);
    

}
//#else
/*****************************************************************************/
// 	Description : MMIAPICALC_OpenMainWin
//
//	Global resource dependence :
//   Author:        zhaohui
//	Note:
/*****************************************************************************/
/*
PUBLIC void MMIAPIDATACOUNTER_OpenResultWin(uint32 simcard)
{

}*/
//#endif//DATA_COUNTER_SUPPORT


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527
