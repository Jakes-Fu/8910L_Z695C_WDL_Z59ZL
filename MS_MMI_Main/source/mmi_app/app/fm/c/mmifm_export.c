/*****************************************************************************
** File Name:      mmifm_export.c                                            *
** Author:         baokun.yin                                                   *
** Date:           12/2008                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008       baokun.yin            Create                                    *
******************************************************************************/

#include "mmi_app_fm_trc.h"
#if defined(__cplusplus)
extern "C" {
#endif
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
    **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmifm_export.h"
#include "mmifm_internal.h"
#include "mmiidle_export.h"
#ifdef FM_SUPPORT
#include "mmiidle_subwintab.h"
#include "mmiidle_statusbar.h"
#include "mmiidle_cstyle.h"
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init FM application
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPIFM_Init(void)
{
    MMIFM_Init();
}
    
/*****************************************************************************/
//  Description : intial FM module  
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_InitModule(void)
{
    MMIFM_InitModule();
}

/*****************************************************************************/
//  Description : get FM's status, play, stop or suspended
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIFM_STATUS_E MMIAPIFM_GetStatus(void)
{
    return MMIFM_GetStatus();
}


/*****************************************************************************/
//  Description : adjust FM sound volume
//  Global resource dependence : 
//  Parameter: adjust volume to current volume + step 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_AdjustVolume(int32 step)
{    
    return MMIFM_AdjustVolume(step);
}

/*****************************************************************************/
//  Description : open play FM main window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_OpenMainWin(void)
{
    MMIFM_OpenMainWin();
}
/*****************************************************************************/
//  Description : Create FM Timer Record Info Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_CreateFMTimerRecordInfoWin(void)
{
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    CreateFMTimerRecordInfoWin();
#endif
} 
/*****************************************************************************/
//  Description : Play FM with input freq
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Play(MMISRV_HANDLE_T audio_handle,uint16 input_freq)
{
    BOOLEAN ret = TRUE;
    ret = MMIFM_PlayByAudioHandle(input_freq,audio_handle);
    return ret;
}
/*****************************************************************************/
//  Description : Close FM device
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Stop(void)
{
    if(FM_SUSPENDED != MMIFM_GetStatus())
    {
        return MMIFM_Stop();
    }
    return FALSE;
}
/*****************************************************************************/
//  Description : Close FM play and exit fm windows
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Exit(void)
{
    return MMIFM_Exit(FALSE);
}
/*****************************************************************************/
//  Description : Stop FM Record for CC
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_StopRecord(void)
{
#if defined(MMIFM_SUPPORT_FM_RECORD) || defined(MMI_FM_TIMER_RECORD_SUPPORT)
    MMIFM_StopRecord();
#endif
}
/*****************************************************************************/
//  Description : Stop FM Auto Search
//  Global resource dependence : 
//  Author:langhua.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_StopAutoSearch(void)
{
    MMIFM_StopAutoSearch();
}
/*****************************************************************************/
//  Description : Continue FM play, used for other apps
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_Resume(BOOLEAN resume_by_fm)
{
    //if(FM_SUSPENDED == MMIAPIFM_GetStatus() && resume_by_fm == MMIFM_GetSuspendByFM())
    if(FM_SUSPENDED == MMIAPIFM_GetStatus())
    {
        MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_FM);
#ifndef SUBLCD_SIZE_NONE
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_FM,PNULL); 
#endif
        MMIFM_Play(FALSE);
    }  
}
/*****************************************************************************/
//  Description : Pause FM play, used for other apps
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Pause(BOOLEAN suspend_by_fm)
{  
    return MMIFM_Pause(suspend_by_fm);
}

/*****************************************************************************/
//  Description : handle headset plug in and plug out action
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_HeadsetInd(MMI_MESSAGE_ID_E msg_id)
{
#ifdef MMI_FM_NEED_HEADSET    //atv�Ͷ����������ߣ��˴�����Ҫ������
    MMIFM_HandleHeadsetAction(msg_id);
#endif
}

/*****************************************************************************/
//  Description : return freq name of FM playing now
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_GetCurFreqName(MMI_STRING_T *str_ptr, uint16 max_name_len)
{
    MMIFM_CUR_INFO_T *cur_info_ptr = MMIFM_GetFMCurrentInfo();
    MMIFM_CHANNEL_T  channel = {0};
    uint16           cat_len = 0;
    uint16           ch_index = 0;
    uint8             freq_number[MMIFM_FREQ_MAX_LEN] = {0};

    if(PNULL == str_ptr || PNULL == str_ptr->wstr_ptr) 
    {
        //SCI_TRACE_LOW:"[MMIFM] MMIAPIFM_GetCurFreqName  str_ptr valid !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_EXPORT_179_112_2_18_2_19_26_58,(uint8*)"");
        return;
    }
    if(PNULL != cur_info_ptr)
    {
        //fm radio
        ch_index = MMIFM_GetChannelIndexByFreq(cur_info_ptr->cur_freq);
        if(MMIFM_INVALID_CHANNEL != ch_index)
        {
             ch_index = cur_info_ptr->cur_channel_index;
            if(MMIFM_GetOneChannel(ch_index, &channel))
            {
                MMIFM_GetChannelDispName(&channel, ch_index, str_ptr->wstr_ptr, max_name_len);
            }
        }
        str_ptr->wstr_len = MMIAPICOM_Wstrlen(str_ptr->wstr_ptr);
        // frequency text
        sprintf((char *)freq_number," %d.%d ",cur_info_ptr->cur_freq/10, cur_info_ptr->cur_freq%10);
        cat_len = (uint16)strlen((char*)freq_number);
        cat_len = MIN(max_name_len - str_ptr->wstr_len, cat_len);
        if(cat_len > 0)
        {
            MMI_STRNTOWSTR(&(str_ptr->wstr_ptr[str_ptr->wstr_len]), cat_len, freq_number, MMIFM_FREQ_MAX_LEN, cat_len);
            str_ptr->wstr_len += cat_len; 
        }
    }
    return;
}

/*****************************************************************************/
//  Description : play FM in eng test
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_SANT_EngTestPlay(void)
{
    MMIFM_EngTestPlay(FALSE);
}
/*****************************************************************************/
//  Description : play FM in eng test
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_EngTestPlay(const BOOLEAN is_auto)
{
    MMIFM_EngTestPlay(is_auto);
}

/*****************************************************************************/
//  Description : stop FM in eng test
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_EngTestStop(void)
{
	MMIFM_EngTestStop();
	// Bug 1370182, �뿪��ģFM����ʱ����FMӦ��״̬����ΪSTOP
	MMIFM_SetStatus(FM_STOP);
}
/*****************************************************************************/
//  Description : Create Alarm FM Channel List
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_CreateAlarmChannelList(ADD_DATA param)
{
    MMIFM_CreateAlarmChannelList(param);
}
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Description : Start FM timer record 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_StartFMTimerRecord(void)
{    
    //SCI_TRACE_LOW:"[MMIFM] MMIAPIFM_StartFMTimerRecord  enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_EXPORT_239_112_2_18_2_19_26_59,(uint8*)"");
    MMIFM_StartFMTimerRecord();
}
#endif
/*****************************************************************************/
//  Description : reset timer fm info
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMAPIIFM_Reset(void)
{
    MMIFM_Reset();    
}

/*****************************************************************************/
//  Description : �ж�fm�Ƿ�playing,���ⲿʹ��
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsFmPlaying(void)
{
	BOOLEAN result = FALSE;
 
	//get fm status
	if (FM_PLAYING == MMIAPIFM_GetStatus())
	{
		result =  TRUE;
	}
	SCI_TRACE_LOW("MMIAPIFM_IsFmPlaying:result %d",result);
	return result;
}

/*****************************************************************************/
//  Description : �ж�fm�Ƿ�active,���ⲿʹ��
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsFmActive(void)
{
    BOOLEAN         result = FALSE;
    MMIFM_STATUS_E  fm_status = FM_PLAYING;

    //get fm status
    fm_status = MMIAPIFM_GetStatus();
    switch (fm_status)
    {
    case FM_PLAYING:
    case FM_SUSPENDED:
        result = TRUE;
        break;

    case FM_STOP:
        result = FALSE;
        break;

    default:
        break;
    }

	//SCI_TRACE_LOW:"isplayFmName:fm_status %d is error!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_EXPORT_277_112_2_18_2_19_26_60,(uint8*)"d", fm_status);
    return result;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsFmRecordActive(void)
{
    BOOLEAN         result = FALSE;
#ifdef MMIFM_SUPPORT_FM_RECORD
    if(MMIFMRECORD_STATE_RECORDING == MMIFM_GetRecordStatus()
        ||MMIFMRECORD_STATE_RECORD_PAUSED == MMIFM_GetRecordStatus()
    )
        result = TRUE;
    
    //SCI_TRACE_LOW:"[MMIFM] MMIAPIFM_IsFmRecordActive  result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_EXPORT_293_112_2_18_2_19_26_61,(uint8*)"d",result);
#endif
    return result;
}
/*****************************************************************************/
//  Description : get fm audio current device mode
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
/*PUBLIC AUDIO_DEVICE_MODE_TYPE_E MMIAPIFM_GetFMCurrentDeviceMode(void)
{   
    AUDIO_DEVICE_MODE_TYPE_E mode = AUDIO_DEVICE_MODE_EARPHONE|AUDIO_DEVICE_MODE_HANDFREE;
        
    if(FM_SPEAKER_MODE == MMIFM_GetFMCurrentDeviceMode())
    {
        mode = AUDIO_DEVICE_MODE_HANDFREE;
    }

    return mode;
}*/
/*****************************************************************************/
//  Description : preview fm frequency status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsPreviewFM(void)
{
    return MMIFM_IsPreviewFM();
}
/*****************************************************************************/
//  Description : Search one  channel.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_SearchValidFreq(int32 direct)
{
    return (BOOLEAN)MMIFM_SearchValidFreq(direct);
}
/*****************************************************************************/
//  Description : Adjust FM channel.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_AdjustChannel(int32 step)
{
    return MMIFM_AdjustChannel(step);
}
/*****************************************************************************/
//  Description : Adjust FM channel.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIENGFM_CUR_INFO_T * MMIAPIFM_GetFMCurrentInfo(void)
{
    return (MMIENGFM_CUR_INFO_T *)MMIFM_GetFMCurrentInfo();
}

/*****************************************************************************/
//  Description : Get FM play handle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIAPIFM_GetPlayHandle(void)
{
    return MMIFM_GetPlayHandle();
}

/*****************************************************************************/
//  Description : Convert FM frequency from digit (uint16) to string (char*)
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_ConvertFreqToString(const uint16 freq, char* freq_buf)
{
	if (freq_buf != PNULL)
	{
		/*
		FMͨ���˽ӿڻ�ȡ����Ƶ�ʣ�����ݲ�ͬ����ֵ�᲻һ����
		���50khz 8750~10800
		���100khz 875~1050

		������Ҫ���ýӿڣ�MMIFM_GetFmTuneStep�������ж��¡�
		�˽ӿڷ���ֵ��0,1�� 0 ��Ӧ 8750~10800  1 ��Ӧ875~1050
		*/
		if (MMIFM_GetFmTuneStep() == 0)
		{
			sprintf(freq_buf, "%d.%d", freq / 100, (freq % 100) / 10);
		}
		else
		{
			sprintf(freq_buf, "%d.%d", freq / 10, freq % 10);
		}
	}
}

/*****************************************************************************/
//  Description : Convert FM frequency from string (char*) to digit (uint16)
//  Global resource dependence : 
//  Parameter: str_ptr[IN] : the number string with decimal
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIFM_ConvertStringToFreq(const MMI_STRING_T* str_ptr)
{
	int16	ret =0;
	int16	point_index = 0;
	int32	i = 0;
	int32	mul_scale = MMIFM_GetFmTuneStep() == 0 ? 100 : 10;
	uint8	num_buf[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
	uint8	decimal_buf[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
	uint16	num_len = 0;
	BOOLEAN	result = TRUE;

	if (str_ptr != PNULL && str_ptr->wstr_ptr != PNULL)
	{
		num_len = MIN(MMIFM_CHANNEL_NAME_LEN, str_ptr->wstr_len);
		MMI_WSTRNTOSTR(num_buf, MMIFM_CHANNEL_NAME_LEN, str_ptr->wstr_ptr, MMIFM_CHANNEL_NAME_LEN, num_len);
		
		//get point postion
		for (i = 0; i < num_len;i++)
		{
			if ('.' == num_buf[i])
			{
				point_index = i;
				break;
			}
		}

		for (i = point_index + 1;i < num_len;i++)
		{
			if (('0' > num_buf[i]) || ('9' < num_buf[i]))
			{
				result = FALSE;
				break;
			}
		}

		if (result)
		{
			if (0 != point_index && (point_index + 1 < num_len)) 
			{
				strncpy((char*) decimal_buf, (char*) &num_buf[point_index + 1], (num_len - point_index - 1));
			}
			if (point_index > 0)
			{
				//no point
				num_buf[point_index] = 0;
			}
			ret = atoi((char*) num_buf) * mul_scale;
			ret += atoi((char*) decimal_buf) * ((num_len - point_index - 1 == 1) ? 10 : 1);
		}
		else
		{
			ret = 0;
		}
	}
	
	SCI_TRACE_LOW("[MMIFM] MMIAPIFM_ConvertStringToFreq ret %d", ret);

	return ret;
}

/*****************************************************************************/
//  Description : Get FM valid channels number
//  Global resource dependence : 
//  Parameter:
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIFM_GetValidChannelCount(void)
{
	return MMIFM_GetValidChannelCount();
}
#else
/*****************************************************************************/
//  Description : Continue FM play, used for other apps
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_HeadsetInd(MMI_MESSAGE_ID_E msg_id)
{
    
}
/*****************************************************************************/
//  Description : init FM application
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_Init(void)
{
}

/*****************************************************************************/
//  Description : intial FM module  
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_InitModule(void)
{
}

/*****************************************************************************/
//  Description : get FM's status, play, stop or suspended
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIFM_STATUS_E MMIAPIFM_GetStatus(void)
{
    return FM_STOP;
}


/*****************************************************************************/
//  Description : adjust FM sound volume
//  Global resource dependence : 
//  Parameter: adjust volume to current volume + step 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_AdjustVolume(int32 step)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : open play FM main window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_OpenMainWin(void)
{
}
/*****************************************************************************/
//  Description : Pause FM play, used for other apps
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Pause(BOOLEAN suspend_by_fm)
{    
    return TRUE;
}

/*****************************************************************************/
//  Description : Continue FM play, used for other apps
//  Global resource dependence : 
//  Author:Liqing Peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_Resume(BOOLEAN resume_by_fm)
{
}

/*****************************************************************************/
//  Description : Close FM play, used for other apps
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Stop(void)
{
    return TRUE;
    
}
/*****************************************************************************/
//  Description : play FM in eng test
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_EngTestPlay(const BOOLEAN is_auto)
{
}

/*****************************************************************************/
//  Description : stop FM in eng test
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_EngTestStop(void)
{
}

/*****************************************************************************/
//  Description : return freq name of FM playing now
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_GetCurFreqName(MMI_STRING_T *str_ptr, uint16 max_name_len)
{
    return;
}

/*****************************************************************************/
//  Description : Close FM play and exit fm windows
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Exit(void)
{
    return TRUE;
}
/*****************************************************************************/
//  Description : Stop FM Record for CC
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_StopRecord(void)
{
}
/*****************************************************************************/
//  Description : Stop FM Auto Search
//  Global resource dependence : 
//  Author:langhua.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_StopAutoSearch(void)
{
}
/*****************************************************************************/
//     Description : get sum size of all fm nv itme
//    Global resource dependence : 
//  Author: jian.ma
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFM_GetSumNvSize(void)
{
    return 0;
}
/*****************************************************************************/
//  Description : set fm digital type
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_SetFmDigitalType(void)
{
}
/*****************************************************************************/
//  Description : Play FM with input freq
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Play(MMISRV_HANDLE_T audio_handle,uint16 input_freq)
{
    return TRUE;
}
/*****************************************************************************/
//  Description : Create Alarm FM Channel List
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_CreateAlarmChannelList(ADD_DATA param)
{    
}

/*****************************************************************************/
//  Description : �ж�fm�Ƿ�active,���ⲿʹ��
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsFmActive(void)
{
    return FALSE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsFmRecordActive(void)
{
    return FALSE;
}
/*****************************************************************************/
//  Description : get fm audio current device mode
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
/*PUBLIC AUDIO_DEVICE_MODE_TYPE_E MMIAPIFM_GetFMCurrentDeviceMode(void)
{   
    return FALSE;
}*/
/*****************************************************************************/
//  Description : preview fm frequency status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsPreviewFM(void)
{
    return FALSE;
}
/*****************************************************************************/
//  Description : Search one channel.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_SearchValidFreq(int32 direct)
{
    return FALSE;
}
/*****************************************************************************/
//  Description : Adjust FM channel.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_AdjustChannel(int32 step)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Adjust FM channel.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIENGFM_CUR_INFO_T* MMIAPIFM_GetFMCurrentInfo(void)
{
    return NULL;
}

/*****************************************************************************/
//  Description : Convert FM frequency from digit (uint16) to string (char*)
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_ConvertFreqToString(const uint16 freq, char* freq_buf)
{
}

/*****************************************************************************/
//  Description : Convert FM frequency from string (char*) to digit (uint16)
//  Global resource dependence : 
//  Parameter: str_ptr[IN] : the number string with decimal
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIFM_ConvertStringToFreq(const MMI_STRING_T* str_ptr)
{
	return 0;
}

/*****************************************************************************/
//  Description : Get FM valid channels number
//  Global resource dependence : 
//  Parameter:
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIFM_GetValidChannelCount(void)
{
	return 0;
}
#endif
    
#if defined(__cplusplus)
}
#endif

