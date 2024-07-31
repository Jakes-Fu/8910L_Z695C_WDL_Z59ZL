/******************************************************************************/
// File Name:       McfInterface.c                                           
// Author:          wilsonzhu wiltan                                 
// DATE:            10/01/2011                                  
// Modifed:         09/26/2011
// Copyright:       2011 Tencent, Incoporated. All Rights Reserved.
// Description:     用来定义SIM卡相关接口                      
/******************************************************************************/
/******************************************************************************/
//                      include files
/******************************************************************************/
#include "mcfinterface.h"
#include "mcfidle.h"
#include "mcare_img_id.h"
#include "mcare_id.h"
#include "mcfsim.h"
#include "mcfkey.h"
#include "mcfpen.h"
#include "mcfaudio.h"
#include "mcfstdlib.h"
#include "mcare_interface.h"
#include "mcfmemory.h"
#include "mcfsms.h"
#include "mcfsocket.h"
#include "mcftimer.h"
#include "mcftrace.h"
#include "mcommondef.h"
#include "mcfgdi.h"


#include "mmiaudio_ctrl.h"
#include "mn_type.h"
#include "mmipb_export.h"
#include "mmiset_display.h"

#ifndef SPREAD_PLAT_SC6530
#include "mmicc.h "
#include "mmipb_internal.h"
#include "mmi_keylock.h"
#include "mmiset.h"
#include "mainapp.h"
#endif

#include "mmiaudio.h"
#include "mmidisplay_data.h"
#include "window_parse.h"

#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G)|| (defined SPREAD_PLAT_SC8800G_PDA)
#include"guiedit.h"

#ifdef MCARE_BROWSER_SUPPORT
#include "mmibrowser_api.h"
#endif

#include "mmienvset.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin.h"
#endif

#elif (defined SPREAD_PLAT_SC6530)
#include"guiedit.h"
//#include "mmienvset.h"
#include "mmibrowser_export.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmicc_export.h"
#include "mmisd_internal.h"
#include "mmienvset_internal.h"

#ifdef SPREAD_NEW_AUDIO_UPDATE
#include "mmisrvaud_api.h"
#include "mmivp_fsm.h"
#endif//#ifdef SPREAD_NEW_AUDIO_UPDATE

#else//#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530) 

#include "im.h"
#include "mmienvset_internal.h"
#if (defined SPREAD_PLAT_SC6620)
#include "mmibrowser_api.h"
#else
#include "mmiwap.h"
#endif
#include "sig_code.h"

#endif//#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530) 


#include "mmi_default.h"
#include "mmisms_send.h"
#include "mmk_timer.h"
#include "mmiudisk_export.h"
#include "gpio_prod_api.h"
#include "mmiudisk_internal.h"
#include "mcfpb.h"

#ifndef SPREAD_PLAT_SC6600L_0937
#include "socket_types.h"
#endif

#include "mmiacc_id.h"

#ifdef SPREAD_NEW_SMS_UPDATE
#include "mmisms_export.h"
#endif

#include "guistatusbar.h"

#ifndef SPREAD_PLAT_SC6530
#include "mmipdp_api.h"
#endif

/******************************************************************************/
//                      Macro Declaration
/******************************************************************************/

/******************************************************************************/
//                      Type and constant
/******************************************************************************/
/******************************************************************************/
//                      Local variables Declaration
/******************************************************************************/
/******************************************************************************/
//                      Local function Define
/******************************************************************************/
#ifdef SPREAD_NEW_PB_UPDATE
static McfPhbCallBack s_app_del_callback = {0};
static McfPhbCallBack s_app_update_callback = {0};
static McfPhbCallBack s_app_add_callback = {0};
static MBOOL s_app_is_null_pb   = MFALSE;
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcarePb
//	Descrption:窗口消息处理函数
//	Param:
//      IN   Mocro_Phb:展讯平台的记录信息
//      OUT  Mcf_Phb:MCARE 平台的记录信息
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------

static MBOOL McfPhb_MocroToMcarePb(const MMIPB_CONTACT_T* Mocro_Phb,
                                            McfPhbEntry* Mcf_Phb);

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroPb
//	Descrption:MCARE的记录转换成展讯的记录
//	Param:
//      IN  Mcf_Phb:MCARE 平台的记录信息
//      OUT   Mocro_Phb:展讯平台的记录信息
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------

static MBOOL McfPhb_McareToMocroPb(McfPhbEntry* Mcf_Phb, MMIPB_CONTACT_T* Mocro_Phb);

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcareNum
//	Descrption:展讯平台的号码转换成MCARE平台的号码
//	Param:
//      IN    Mocro_num:展讯 平台的实际存储num
//      IN    buffer_len: 保存MCARE平台num的buffer长度
//      OUT   MCR_num:MCARE平台的存储num
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
static MBOOL McfPhb_MocroToMcareNum(const MMIPB_BCD_NUMBER_T* Mocro_num, 
                                        MUSHORT* MCR_num ,MBYTE buffer_len);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroNum
//	Descrption:MCARE平台的号码转换成展讯平台的号码
//	Param:
//      IN   MCR_num:MCARE平台的存储num
//      OUT    Mocro_num:展讯 平台的实际存储num
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
static MBOOL McfPhb_McareToMocroNum(const MUSHORT* MCR_num , MMIPB_BCD_NUMBER_T* Mocro_num);
static MVOID McfPhb_DelPBCallBack(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error);
static MVOID McfPhb_UpdatePBCallBack(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error);
static MVOID McfPhb_AddPBCallBack(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error);

#else//#ifdef SPREAD_NEW_PB_UPDATE

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcarePb
//	Descrption:窗口消息处理函数
//	Param:
//      IN   Mocro_Phb:展讯平台的记录信息
//      OUT  Mcf_Phb:MCARE 平台的记录信息
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------

static MBOOL McfPhb_MocroToMcarePb(const MMIPB_PHONEBOOK_ENTRY_T* Mocro_Phb,
                                            McfPhbEntry* Mcf_Phb);

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroPb
//	Descrption:MCARE的记录转换成展讯的记录
//	Param:
//      IN  Mcf_Phb:MCARE 平台的记录信息
//      OUT   Mocro_Phb:展讯平台的记录信息
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------

static MBOOL McfPhb_McareToMocroPb(McfPhbEntry* Mcf_Phb, MMIPB_PHONEBOOK_ENTRY_T* Mocro_Phb);

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcareNum
//	Descrption:展讯平台的号码转换成MCARE平台的号码
//	Param:
//      IN    Mocro_num:展讯 平台的实际存储num
//      IN    buffer_len: 保存MCARE平台num的buffer长度
//      OUT   MCR_num:MCARE平台的存储num
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
static MBOOL McfPhb_MocroToMcareNum(const MMIPB_BCD_NUMBER_T* Mocro_num, 
                                        MUSHORT* MCR_num ,MBYTE buffer_len);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroNum
//	Descrption:MCARE平台的号码转换成展讯平台的号码
//	Param:
//      IN   MCR_num:MCARE平台的存储num
//      OUT    Mocro_num:展讯 平台的实际存储num
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
static MBOOL McfPhb_McareToMocroNum(const MUSHORT* MCR_num , MMIPB_BCD_NUMBER_T* Mocro_num);

#endif//#ifdef SPREAD_NEW_PB_UPDATE

//启动失败！
const MUSHORT s_mcare_run_failed[] = {0x542F,0x52A8,0x5931,0x8D25,0xFF01,0x0000};

/*=======================McfAudio.c Begin==========================*/

/*=======================McfAudio.c End==========================*/
/*=======================McfDtcnt.c API BEGIN=========================*/

#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC6610) || (defined SPREAD_PLAT_SC8800G)\
|| (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
static MUINT appMnPdpContextMap[McfCmn_MN_PDP_CONTEXT_MAX][2] = 
{
	{McfCmn_MN_ACTIVATE_PDP_CONTEXT_CNF, MMIPDP_ACTIVE_CNF},
	{McfCmn_MN_DEACTIVATE_PDP_CONTEXT_CNF, MMIPDP_DEACTIVE_CNF},
	{McfCmn_MN_DEACTIVATE_PDP_CONTEXT_IND, MMIPDP_DEACTIVE_IND},
};
#else
static MUINT appMnPdpContextMap[McfCmn_MN_PDP_CONTEXT_MAX][2] = 
{
	{McfCmn_MN_ACTIVATE_PDP_CONTEXT_CNF, APP_MN_ACTIVATE_PDP_CONTEXT_CNF},
	{McfCmn_MN_DEACTIVATE_PDP_CONTEXT_CNF, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF},
	{McfCmn_MN_DEACTIVATE_PDP_CONTEXT_IND, APP_MN_DEACTIVATE_PDP_CONTEXT_IND},
};
#endif
static MUINT mnGprsErrMap[McfCmn_GPRS_ERR_MAX][2] = 
{
	{McfCmn_GPRS_ERR_SUCCESS,MN_GPRS_ERR_SUCCESS},
	{McfCmn_GPRS_ERR_TEMPORARILY_BLOCKED,MN_GPRS_ERR_TEMPORARILY_BLOCKED},
	{McfCmn_GPRS_ERR_RETRYING,MN_GPRS_ERR_RETRYING},
};
/*=======================McfDtcnt.c API end=========================*/

//=======================McfEditor.c end=========================
static MMI_STRING_T s_str_info = {0};
static MWCHAR* pStr_tmp = MNULL;
static EDIT_CALLBACK g_mc_editcallback = {0};

static MMI_RESULT_E McfEditor_HandleEditBoxWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);


#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
WINDOW_TABLE(MMIMCARE_EDITBOX_WIN_TAB) = 
{
#ifdef PDA_UI_SUPPORT_ANDROID
	WIN_STYLE(WS_HAS_SOFTKEY),
#endif
		WIN_FUNC((MUINT)McfEditor_HandleEditBoxWinMsg),    
		WIN_ID(MMIMCARE_EDITBOX_WIN_ID),
		WIN_TITLE(TXT_NULL),
#ifdef MMI_PDA_SUPPORT
		WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
		WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
		END_WIN
};
#else
WINDOW_TABLE(MMIMCARE_EDITBOX_WIN_TAB) = 
{
	WIN_FUNC (McfEditor_HandleEditBoxWinMsg ),    
	WIN_ID(MMIMCARE_EDITBOX_WIN_ID),
	WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
	END_WIN
};
#endif


//=======================McfEditor.c end==========================

//=======================McfFs.c Begin============================
static MBOOL s_IsUsePublicDrv = MFALSE;
//=======================McfFs.c End=============================

//=======================McfScreen.c Begin==========================
static MBOOL g_is_show_welcom_tip = MTRUE;
#if (defined SPREAD_PLAT_SC6800H || defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA) 
static MUINT s_gsensor_switch_style = 0;
static MUINT s_gsensor_open_flag = 0;
static MUINT s_gsensor_Screen_Style = 0;
#endif

//=======================McfScreen.c End==========================
//=======================McfVibrate.c API Begin==========================

static MUINT s_vibrator_last_start_timer= 0;
//=======================McfVibrate.c API end==========================

//**************************************************************/
//                      function declare
//**************************************************************/

//=======================McfAudio.c API Begin==========================
#ifdef SPREAD_NEW_AUDIO_UPDATE

MUINT McfAud_McareToMocorVol(MBYTE volume)
{
    MUINT vol = MMISET_VOL_SEVEN;
    switch(volume)
    {
        case MAUD_VOL_MUTE:
            vol = MMISET_VOL_ZERO;
            break;
        case MAUD_VOL_LEVEL1:
            vol = MMISET_VOL_ONE;
            break;
        case MAUD_VOL_LEVEL2:
            vol = MMISET_VOL_TWO;
            break;
        case MAUD_VOL_LEVEL3:
            vol = MMISET_VOL_THREE;
            break;
        case MAUD_VOL_LEVEL4:
            vol = MMISET_VOL_FOUR;
            break;
        case MAUD_VOL_LEVEL5:
            vol = MMISET_VOL_FIVE;
            break;
        case MAUD_VOL_LEVEL6:
            vol = MMISET_VOL_SIX;
            break;
        case MAUD_VOL_LEVEL7:
        case MAUD_VOL_MAX:
            vol = MMISET_VOL_SEVEN;
            break;
        default:
            break;
    }  

    return vol;       

}

//---------------------------------------------------------------------------
//	FUNCTION:       McfAud_GetAudioType
//	Descrption:     根据MCARE的音频类型获取平台的音频类型
//	Param:
//	   	IN          MBYTE playStyle :MCARE的音频类型    
//	Return:         平台的音频类型
//  Note:
//----------------------------------------------------------------------------
MMISRVAUD_RING_FMT_E McfAud_GetAudioType(MBYTE format)
{
	MMISRVAUD_RING_FMT_E  ring_type = MMISRVAUD_RING_FMT_NONE;
	switch(format)
	{
	case MAUD_MIDI:
		{
			ring_type = MMISRVAUD_RING_FMT_MIDI;
		}
		break;
	case MAUD_WAV:
		{
			ring_type = MMISRVAUD_RING_FMT_WAVE;
		}
		break;
	case MAUD_AMR:
	    {
#ifdef SPREAD_RECORD_MP3_TYPE
	        ring_type = MMISRVAUD_RING_FMT_MP3;//MMISRVAUD_RING_FMT_AMR;
#else
	        ring_type = MMISRVAUD_RING_FMT_AMR;
#endif
	    }
	    break;
	default:
		{
			MCF_SCI_ASSERT(MFALSE); 
		}
		break;
	}
	return ring_type;
}

MUINT McfAud_GetRingVol(MVOID)
{
	return MAUD_VOL_LEVEL3;
}

MBOOL McfAud_IsSilentMode(MBYTE format)
{
	MBOOL result = MFALSE;
	switch(format)
	{
	case MAUD_MIDI:
		//用户可以根据不同的类型进行具的判断操作
		result = (MMIENVSET_GetCurModeId() == MMIENVSET_SILENT_MODE)?MTRUE:MFALSE;
		break;
	case MAUD_WAV:
		//用户可以根据不同的类型进行具的判断操作
		result = (MMIENVSET_GetCurModeId() == MMIENVSET_SILENT_MODE)?MTRUE:MFALSE;
		break;
	case MAUD_AMR:
		//用户可以根据不同的类型进行具的判断操作
		result = (MMIENVSET_GetCurModeId() == MMIENVSET_SILENT_MODE)?MTRUE:MFALSE;
		break;
	default:
		{
			MCF_SCI_ASSERT(MFALSE); 
		}
		break;
	}
	
	MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"result= %d, format = %d", result, format);
	return result;
}

MBYTE McfAud_GetFileFormat(MSHORT* pFile)
{
	MMISRVAUD_RING_FMT_E music_type =0;
	MBYTE mcare_type = MAUD_MIDI;
	music_type = MMIAPICOM_GetMusicType(pFile, McfStdC_wcslen(pFile));
	switch(music_type)
	{
	case MMISRVAUD_RING_FMT_MIDI:
		mcare_type = MAUD_MIDI;
		break;
	case MMISRVAUD_RING_FMT_WAVE:
		mcare_type = MAUD_WAV;
		break;
#ifdef SPREAD_RECORD_MP3_TYPE
	case MMISRVAUD_RING_FMT_MP3:
#else
	case MMISRVAUD_RING_FMT_AMR:
#endif
		mcare_type = MAUD_AMR ;
		break;
	default:
		MCF_SCI_ASSERT(MFALSE);
		break;
	}
	return mcare_type;	
}

MINT McfIF_Aud_PlayStream(MVOID* pUser, MVOID* pAudioData, MUINT len, MBYTE format, 
					   MBYTE playStyle, pfnMcfAudFunc handler)
{
	MINT audio_hand = AUDHAND_ERROR;
	MMISRVMGR_SERVICE_REQ_T req = {0};
	MMISRVAUD_TYPE_T audio_srv = {0};
	MMISRV_HANDLE_T handle = 0;//有问题
	
	//req.is_auto_resume_off = MTRUE;  //是否自动恢复被挂起的声音
	req.notify = McfAud_CallBack;	//回调函数
	req.pri = MMISRVAUD_PRI_NORMAL;  //优先级
	//req.is_auto_free = MTRUE;   //被打断或播放结束，是否自动释放

	audio_srv.duation = 0;		//播放时间，为0时play_times有效
	if(MAUD_PLAY_ONCE == playStyle)
	{
		audio_srv.play_times = 1;    //播放次数
	}
	else if(MAUD_PLAY_FOREVER == playStyle)
	{
		audio_srv.play_times = 1000;    //播放次数
	}
	audio_srv. all_support_route = MMISRVAUD_ROUTE_AUTO;
	audio_srv.volume = McfAud_GetRingVol();
	audio_srv.eq_mode = MMISRVAUD_EQ_REGULAR;

	audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF; //表示Buff中的声音
	audio_srv.info.ring_buf.fmt = McfAud_GetAudioType(format);
	audio_srv.info.ring_buf.data = pAudioData;
	audio_srv.info.ring_buf.data_len = len;
	
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

	if(0 != handle)
	{
		audio_hand = McfAud_AddIn(pUser, handler, format, handle);
		if(AUDHAND_ERROR != audio_hand)
		{//获取句柄成功
			if(MMISRVAUD_Play(handle, 0))
			{
				return audio_hand;
			}
			else
			{
				MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_PlayStream    MMISRVAUD_Play MAUD_FAIL");
				McfAud_DelHandle(audio_hand);
				MMISRVMGR_Free(handle);
				handle = 0;
				return MAUD_FAIL;
			}
		}
		else
		{
			MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_PlayStream    McfAud_AddIn MAUD_FAIL");
			MMISRVMGR_Free(handle);
			handle = 0;
			return MAUD_FAIL;
		}
	}
	else
	{
		MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_PlayStream    MMISRVMGR_Request MAUD_FAIL");
		return MAUD_FAIL;
	}
}

MINT McfIF_Aud_StopStream(MINT audHd)
{
	MINT mocor_hanlde = McfAud_GetMocorHandler(audHd);
	MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_StopStream audHd = %d, mocor_hanlde = %d", audHd, mocor_hanlde);
	MMISRVAUD_Stop(mocor_hanlde);
	MMISRVMGR_Free(mocor_hanlde);
	mocor_hanlde = 0;
	McfAud_DelHandle(audHd);
	return MAUD_SUCCESS;
}

MINT McfIF_Aud_PlayFile(MVOID* pUser, MSHORT* pFile, MBYTE playStyle, pfnMcfAudFunc handler)
{
	MINT audio_hand = AUDHAND_ERROR;
	MMISRVMGR_SERVICE_REQ_T req = {0};
	MMISRVAUD_TYPE_T audio_srv = {0};
	MMISRV_HANDLE_T handle = 0;//有问题		
	
	req.is_auto_resume_off = MTRUE;  //是否自动恢复被挂起的声音
	req.notify = McfAud_CallBack;	//回调函数
	req.pri = MMISRVAUD_PRI_NORMAL;  //优先级
	req.is_auto_free = MTRUE;   //被打断或播放结束，是否自动释放

	audio_srv.duation = 0;		//播放时间，为0时play_times有效
	if(MAUD_PLAY_ONCE == playStyle)
	{
		audio_srv.play_times = 1;    //播放次数
	}
	else if(MAUD_PLAY_FOREVER == playStyle)
	{
		audio_srv.play_times = 1000;    //播放次数
	}
	audio_srv. all_support_route = MMISRVAUD_ROUTE_AUTO;
	audio_srv.volume = McfAud_GetRingVol();
	audio_srv.eq_mode = MMISRVAUD_EQ_REGULAR;

	audio_srv.info.ring_file.type = MMISRVAUD_TYPE_RING_FILE; //表示是文件中的声音
	audio_srv.info.ring_file.name = (MWCHAR*)pFile;
	audio_srv.info.ring_file.name_len = MMIAPICOM_Wstrlen(pFile);
	
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

	if(0 != handle)
	{
		audio_hand = McfAud_AddIn(pUser, handler, McfAud_GetFileFormat(pFile), handle);
		if(AUDHAND_ERROR != audio_hand)
		{//获取句柄成功
			if(MMISRVAUD_Play(handle, 0))
			{
				return audio_hand;
			}
			else
			{
				MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_PlayFile    MMISRVAUD_Play MAUD_FAIL");
				McfAud_DelHandle(audio_hand);
				MMISRVMGR_Free(handle);
				handle = 0;
				return MAUD_FAIL;
			}
		}
		else
		{
			MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_PlayFile    McfAud_AddIn MAUD_FAIL");
			MMISRVMGR_Free(handle);
			handle = 0;
			return MAUD_FAIL;
		}
	}
	else
	{
		MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_PlayFile    MMISRVMGR_Request MAUD_FAIL");
		return MAUD_FAIL;
	}
}
MINT McfIF_Aud_StopFile(MINT audHd)
{
	MINT mocor_hanlde = McfAud_GetMocorHandler(audHd);
	MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_StopFile audHd = %d, mocor_hanlde = %d", audHd, mocor_hanlde);
	MMISRVAUD_Stop(mocor_hanlde);
	MMISRVMGR_Free(mocor_hanlde);
	mocor_hanlde = 0;
	McfAud_DelHandle(audHd);
	return MAUD_SUCCESS;
}
MINT McfIF_Aud_Pause(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler)
{
	MINT result = MAUD_FAIL;
	MINT mocor_hanlde = McfAud_GetMocorHandler(audHd);
	MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_Pause audHd = %d, mocor_hanlde = %d", audHd, mocor_hanlde);
	if(MMISRVAUD_Pause(mocor_hanlde))
	{
		result = MAUD_SUCCESS;
	}
	MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_Pause callback entry");
	(*handler)(pUser, result);
	MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_Pause callback leave");
	return result;
}
MINT McfIF_Aud_Resume(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler)
{
	MINT result = MAUD_FAIL;
	MINT mocor_hanlde = McfAud_GetMocorHandler(audHd);
	MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_Resume audHd = %d, mocor_hanlde = %d", audHd, mocor_hanlde);
	if(MMISRVAUD_Resume(mocor_hanlde))
	{
		result = MAUD_SUCCESS;
	}
	MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_Resume callback entry");
	(*handler)(pUser, result);
	MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_Resume callback leave");
	return result;
}
MBOOL MAPI_MMISRVMGR_Free(MUINT audio_handle)
{
	return MMISRVMGR_Free(audio_handle);
}
MBOOL McfIF_Aud_CallBack(MUINT mocor_hanlde, MVOID* pParam)
{
    MMISRVAUD_REPORT_T* report_ptr = MNULL;
	MBOOL result = MFALSE;
	MMISRVMGR_NOTIFY_PARAM_T* param = (MMISRVMGR_NOTIFY_PARAM_T*)pParam;
	MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfAud_CallBack mocor_hanlde = %d,param = 0x%x", mocor_hanlde,param);

	if(mocor_hanlde != AUDHAND_ERROR && param != MNULL)
	{
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            break;
        case MMISRVMGR_NOTIFY_RESUME:
            break;
        case MMISRVMGR_NOTIFY_EXT:
            report_ptr = (MMISRVAUD_REPORT_T *)param->data;
            if(MNULL != report_ptr)
            {
                switch(report_ptr->report)
                {
                case MMISRVAUD_REPORT_END:
                    MMISRVAUD_Stop(mocor_hanlde);
                    MMISRVMGR_Free(mocor_hanlde);
                    mocor_hanlde = 0;
                    break;
                    
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
        result = MTRUE;
	}
	return result;
}

MBOOL McfIF_Aud_IsPermit(MVOID)
{
	//控制6530上面，如果视频播放时，Mcare来消息会不播放声音
	if(0 != VP_GetCurrentFSMHandle())
	{
		MCFAPI_AUDIO_PRINT(ALOG_INFO,MLOG_SDKAUDIO, __MCR_FUNCTION__"[MCFAUDIO] McfIF_Aud_IsPermit return MFALSE");
		return MFALSE;
	}
	return MTRUE;
}

#else//#ifdef SPREAD_NEW_AUDIO_UPDATE

MBOOL MAPI_MMIAPISET_IsPermitPlayRing(MINT ring_type)
{
	MMISET_ALL_RING_TYPE_E  Ring_type = (MMISET_ALL_RING_TYPE_E)ring_type;
	return MMIAPISET_IsPermitPlayRing(Ring_type);
}


MBOOL MAPI_MMIAUDIO_AudioPlay(
							  MMIAUDIO_HTYPE   audio_handle_type,
							  MUINT           uiOffset,
							  MUINT           play_times,
							  MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
							  )
{
	return MMIAUDIO_AudioPlay(audio_handle_type,uiOffset,play_times,end_callback_func);
}   

MVOID MAPI_MMIAUDIO_AudioStop(MMIAUDIO_HTYPE audio_handle_type)
{
	MMIAUDIO_AudioStop(audio_handle_type);
}

MBOOL MAPI_MMIAUDIO_AudioPause(MMIAUDIO_HTYPE audio_handle_type)
{
	return MMIAUDIO_AudioPause(audio_handle_type);
}

MBOOL MAPI_MMIAUDIO_AudioResume(MMIAUDIO_HTYPE audio_handle_type)
{
	return MMIAUDIO_AudioResume(audio_handle_type);
}

AUDIO_RESULT_E MAPI_AUDIO_SetVolume(MUINT uiVol)
{
	return AUDIO_SetVolume(uiVol);
}

AUDIO_RESULT_E MAPI_AUDIO_GetVolume(MUINT *uiVol)
{
    return AUDIO_GetVolume(uiVol);
}

MUINT McfAud_McareToMocorVol(MBYTE volume)
{
    MUINT vol = MMISET_VOL_SEVEN;
    switch(volume)
    {
        case MAUD_VOL_MUTE:
            vol = MMISET_VOL_ZERO;
            break;
        case MAUD_VOL_LEVEL1:
            vol = MMISET_VOL_ONE;
            break;
        case MAUD_VOL_LEVEL2:
            vol = MMISET_VOL_TWO;
            break;
        case MAUD_VOL_LEVEL3:
            vol = MMISET_VOL_THREE;
            break;
        case MAUD_VOL_LEVEL4:
            vol = MMISET_VOL_FOUR;
            break;
        case MAUD_VOL_LEVEL5:
            vol = MMISET_VOL_FIVE;
            break;
        case MAUD_VOL_LEVEL6:
            vol = MMISET_VOL_SIX;
            break;
        case MAUD_VOL_LEVEL7:
        case MAUD_VOL_MAX:
            vol = MMISET_VOL_SEVEN;
            break;
        default:
            break;
         return vol;       
    }    
}

MMIAUDIO_HTYPE McfAud_GetAppType(MBYTE format)
{
	return MMIAPISET_GetRingHtype(McfAud_GetRingTypeOther(format));
}
//---------------------------------------------------------------------------
//	FUNCTION:       McfAud_GetAudioType
//	Descrption:     根据MCARE的音频类型获取平台的音频类型
//	Param:
//	   	IN          MBYTE playStyle :MCARE的音频类型    
//	Return:         平台的音频类型
//  Note:
//----------------------------------------------------------------------------
MMIAUDIO_TYPE_E McfAud_GetAudioType(MBYTE playStyle)
{
	MMIAUDIO_TYPE_E  music_type = MMIAUDIO_MIDI;
	switch(playStyle)
	{
	case MAUD_MIDI:
		{
			music_type = MMIAUDIO_MIDI;
		}
		break;
	case MAUD_WAV:
		{
			music_type = MMIAUDIO_WAVE;
		}
		break;
	case MAUD_AMR:
	    {
	        music_type = MMIAUDIO_AMR;
	    }
	    break;
	default:
		{
			MCF_SCI_ASSERT(MFALSE); 
		}
		break;
	}
	return music_type;
}

MMISET_ALL_RING_TYPE_E McfAud_GetRingTypeOther(MBYTE format)
{
	MMISET_ALL_RING_TYPE_E  music_type = MMISET_RING_TYPE_OTHER;
	switch(format)
	{
	case MAUD_MIDI:
		{
			music_type = MMISET_RING_TYPE_OTHER;
		}
		break;
	case MAUD_WAV:
		{
			music_type = MMISET_RING_TYPE_OTHER;
		}
		break;
	case MAUD_AMR:
	    {
	        music_type = MMISET_RING_TYPE_OTHER;
	    }
	    break;
	default:
		{
			MCF_SCI_ASSERT(MFALSE); 
		}
		break;
	}
	return music_type;
}

MUINT McfAud_GetRingVol(MVOID)
{
	return MAUD_VOL_LEVEL3;
}

MVOID McfAud_AudioCallBack(MMIAUDIO_RESULT result, DPARAM param)
{
	MINT ret = MAUD_FINISH;
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"[MCFAUDIO]McfAud_AudioCallBack entry");
	switch(result)
	{
	case MMIAUDIO_RLT_SUCCESS:
		ret = MAUD_FINISH;
		break;
	case MMIAUDIO_RLT_NOT_SUPPORT:
		ret = MAUD_FAIL;
		break;
	case MMIAUDIO_RLT_ERROR:
		ret = MAUD_FAIL;
		break;
	case MMIAUDIO_RLT_OTHER:
		ret = MAUD_FAIL;
		break;
	default:
		ret = MAUD_FAIL;
		break;
	}
	McfAud_CallBack(ret);
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"[MCFAUDIO]McfAud_AudioCallBack leave");
}

MBOOL McfAud_IsSilentMode(MBYTE format)
{
	MBOOL result = MFALSE;
	switch(format)
	{
	case MAUD_MIDI:
		//用户可以根据不同的类型进行具的判断操作
		result = (MMIENVSET_GetCurModeId() == MMIENVSET_SILENT_MODE)?MTRUE:MFALSE;
		break;
	case MAUD_WAV:
		//用户可以根据不同的类型进行具的判断操作
		result = (MMIENVSET_GetCurModeId() == MMIENVSET_SILENT_MODE)?MTRUE:MFALSE;
		break;
	case MAUD_AMR:
		//用户可以根据不同的类型进行具的判断操作
		//result = (MMIENVSET_GetCurModeId() == MMIENVSET_SILENT_MODE)?MTRUE:MFALSE;
		break;
	default:
		{
			MCF_SCI_ASSERT(MFALSE); 
		}
		break;
	}
	return result;
}

MBOOL McfIF_Aud_IsPermit(MVOID)
{
	return MTRUE;
}

MBOOL McfIF_Aud_IsPlaying(MVOID)
{
	return McfAud_IsPlaying();
}
//---------------------------------------------------------------------------
//FUNCTION: MAPI_CreateAudioBufHandle
//Descrption:音频播放的平台级的回调函数
//Param:
//  IN: 
//  OUT:audio_hand 创建的音频句柄
//Return: MTRUE :成功 MFALSE:失败
//Note:
//----------------------------------------------------------------------------
MBOOL MAPI_CreateAudioBufHandle(MBYTE playStyle,MVOID* pAudioData,
									  MUINT len)
{
	MMIAUDIO_TYPE_E  music_type = 0;
	music_type = McfAud_GetAudioType(playStyle);
	return MMIAUDIO_CreateAudioBufHandle(MFALSE,MMIAPISET_GetRingHtype(music_type) , music_type, (const uint8 *)pAudioData, len);

}
//---------------------------------------------------------------------------
//FUNCTION: MAPI_CreateAudioFileHandle
//Descrption:根据文件创建相应的音频
//Param:
//  IN:     创建音频文件的名字
//  OUT:audio_hand 创建的音频句柄
//Return: MTRUE :成功 MFALSE:失败
//Note:
//----------------------------------------------------------------------------
MBOOL MAPI_CreateAudioFileHandle(MSHORT* pFile)
{
	MMIAUDIO_TYPE_E music_type =0;
	music_type = MMIAPICOM_GetMusicType(pFile, McfStdC_wcslen(pFile));
	return MMIAUDIO_CreateAudioFileHandle(MFALSE,MMIAPISET_GetRingHtype(music_type),music_type,pFile);
}

MBYTE McfAud_GetFileFormat(MSHORT* pFile)
{
	MMIAUDIO_TYPE_E music_type =0;
	MBYTE mcare_type = MAUD_MIDI;
	music_type = MMIAPICOM_GetMusicType(pFile, McfStdC_wcslen(pFile));
	switch(music_type)
	{
	case MMIAUDIO_MIDI:
		{
			mcare_type = MAUD_MIDI;
		}
		break;
	case MMIAUDIO_WAVE:
		{
			mcare_type = MAUD_WAV;
		}
		break;
	case MMIAUDIO_AMR:
	    	{
	        		mcare_type = MAUD_AMR ;
	   	 }
	    	break;
	default:
		{
			MCF_SCI_ASSERT(MFALSE); 
		}
		break;
	}
	return mcare_type;	
}

MBOOL MAPI_MMIAUDIO_CloseAudioHandle(MMIAUDIO_HTYPE audio_handle_type)
{
	return MMIAUDIO_CloseAudioHandle(audio_handle_type);
}

MVOID McfAud_EnableKeyRing(MBOOL is_enable)
{
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_RECORDER, is_enable);	
#else
	MMIDEFAULT_EnableKeyRing(is_enable);
	MMIDEFAULT_EnableTpRing(is_enable);
#endif
}

MMIAUDIO_HTYPE s_audio_pause_handle = MMIAUDIO_NONE;
MUINT s_audio_pause_vol = 0;
AUDIO_DEVICE_MODE_TYPE_E s_audio_pause_mod = AUDIO_DEVICE_MODE_MAX;

MMIAUDIO_HTYPE MAPI_MMIAUDIO_GetAudioHtype(MVOID)
{
    return MMIAUDIO_GetAudioHtype();
}

MBOOL McfIF_MMIAUDIO_GetIsPlaying(MVOID)
{
	//return MMIAUDIO_GetIsPlaying();
	//return (MMIAUDIO_GetIsPlaying() || MMIAPIFM_IsFmActive());
	return MTRUE;
}

MBOOL McfIF_MMIAUDIO_AudioResume(MVOID)
{
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__" McfIF_MMIAUDIO_AudioResume MAPI_MMIAUDIO_ResumeBgPlay");
	MAPI_MMIAUDIO_ResumeBgPlay();
	return MTRUE;
}

MBOOL McfIF_MMIAUDIO_AudioPause(MVOID)
{
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__" McfIF_MMIAUDIO_AudioPause MAPI_MMIAUDIO_PauseBgPlay");
	MAPI_MMIAUDIO_PauseBgPlay();
	return MTRUE;
}

MVOID MAPI_MMIAPISET_PlayRingByPtr(
                                    MMISET_ALL_RING_TYPE_E      ring_type,
                                    uint8                       ring_vol,   // the ring volume
                                    uint8                       *ring_data_ptr,
                                    uint32                      ring_data_len,
                                    uint32                      play_times,
                                    MMIAUDIO_TYPE_E             audio_type,
                                    MMISET_RING_CALLBACK_PFUNC  callback_func
                                    )
{
 	MMIAPISET_PlayRingByPtr(ring_type, ring_vol, ring_data_ptr, ring_data_len, play_times, audio_type, callback_func);
}


#endif//#ifdef SPREAD_NEW_AUDIO_UPDATE
//=======================McfAudio.c End==========================


// =======================McfBrowser.c API BEGIN=========================

//---------------------------------------------------------------------------
//	FUNCTION:       McfIF_Brw_StartWapReq
//	Descrption:     调用系统浏览器的函数
//	Param:
//	   	IN          url:传入的url    
//	Return:         
//                  0 : success
//                  -1 : fail
//  Note:
//----------------------------------------------------------------------------
MINT McfIF_Brw_StartWapReq(MCHAR* url)
{
#ifdef MCARE_BROWSER_SUPPORT
	
#if (defined SPREAD_PLAT_SC6800H || defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6610)\
	|| (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC8800G_PDA) 
	MMIBROWSER_ENTRY_PARAM entry_param = {0};
	MMIBROWSER_ERROR_E   entry_result = MMIBROWSER_SUCC;
	
	entry_param.type = MMIBROWSER_ACCESS_URL;
	entry_param.is_use_wifi = MFALSE;
	entry_param.dual_sys = MN_DUAL_SYS_MAX;
	entry_param.url_ptr = url;
	entry_param.user_agent_ptr = MNULL;
	
	entry_result = MMIAPIBROWSER_Entry(&entry_param);
	
	return ((MMIBROWSER_SUCC == entry_result)? 0: (-1));
#elif defined SPREAD_PLAT_SC6530
	MMIBROWSER_ENTRY_PARAM entry_param = {0};
	MMIBROWSER_ERROR_E   entry_result = MMIBROWSER_SUCC;
	
	entry_param.type = MMIBROWSER_ACCESS_URL;
	entry_param.is_use_wifi = MFALSE;
	entry_param.dual_sys = MN_DUAL_SYS_MAX;
	entry_param.url_ptr = url;
	entry_param.user_agent_ptr = MNULL;
	
	entry_result = MMIAPIBROWSER_Entry(&entry_param);
	
	return ((MMIBROWSER_SUCC == entry_result)? 0: (-1));
#else
	//0937和1025的分支
	//是否需要
	extern MINT MGlobal_GetSimId(MVOID);
	if(MMICMSBRW_AccessUrl(url, McfSim_GetSimID(MGlobal_GetSimId())))
	{
		return 0;
	}
	else
	{
		return -1;
	}
#endif
	
#else//#ifdef MCARE_BROWSER_SUPPORT
	return -1;
#endif//#ifdef MCARE_BROWSER_SUPPORT
}

//---------------------------------------------------------------------------
//	FUNCTION:       McfBrw_StartWapReq
//	Descrption:     获取浏览器 USER AGENT         头部信息
//	Param:
//	   	OUT         pUaHeader:获取到的UA 地址
//	   	IN          len:传入buffer 的长度
//	Return:         
//                  success : 返回头部信息
//                  fail   : 返回MNULL
//  Note:
//----------------------------------------------------------------------------
MCHAR* McfIF_Brw_GetUaHeader(MCHAR* pUaHeader, MINT len)
{
	if(0 == len || MNULL == pUaHeader)
	{
		return pUaHeader;
	}
	else
	{
#ifdef MCARE_BROWSER_SUPPORT
		
#if (defined SPREAD_PLAT_SC6800H || defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6610)\
		|| (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
		MBOOL result = MFALSE;
		MCHAR* user_agent_ptr = MNULL;
#ifdef BROWSER_SUPPORT_DORADO
#ifdef CUSTOM_DEFINE_UA//客户自定义UA
        user_agent_ptr = (MCHAR*)MMIAPICOM_GetUserAgent();
		if(MNULL != user_agent_ptr)
		{
			result = MTRUE;
		}
#else
		result = MMIBROWSER_GetDefaultUserAgent(&user_agent_ptr);
#endif
#else//#ifdef BROWSER_SUPPORT_DORADO
		result = MFALSE;
#endif//#ifdef BROWSER_SUPPORT_DORADO
		if(result)
		{
			MINT str_lenth = McfStdC_strlen(user_agent_ptr);
			if(0 >= str_lenth)
			{
				return pUaHeader;
			}
			if(str_lenth > len - 1)
			{
				McfStdC_strncpy(pUaHeader, user_agent_ptr, len - 1);
				pUaHeader[len - 1] = '\0';
			}
			else
			{
				McfStdC_strncpy(pUaHeader, user_agent_ptr, str_lenth);
				pUaHeader[str_lenth] = '\0';
			}
		}
		return pUaHeader;
#else
		MINT str_lenth = McfStdC_strlen(MMIBRW_GetUserAgent());
		if(0 >= str_lenth)
		{
			return pUaHeader;
		}
		if(str_lenth > len - 1)
		{
			McfStdC_strncpy(pUaHeader, MMIBRW_GetUserAgent(), len - 1);
			pUaHeader[len - 1] = '\0';
		}
		else
		{
			McfStdC_strncpy(pUaHeader, MMIBRW_GetUserAgent(), str_lenth);
			pUaHeader[str_lenth] = '\0';
		}
		return pUaHeader;
#endif
#else//#ifdef MCARE_BROWSER_SUPPORT
		return pUaHeader;	
#endif//#ifdef MCARE_BROWSER_SUPPORT
	}
}

//=======================McfBrowser.c API END=========================== 

//=======================McfChset.c API Begin============================

MUSHORT MAPI_GUI_GB2UCS(MWCHAR*ucs2_ptr,const MCHAR*gb_ptr,MUSHORT len)
{
	return GUI_GB2UCS(ucs2_ptr,gb_ptr,len);
}

//---------------------------------------------------------------------------
//	FUNCTION:       MAPI_GUI_UTF8ToWstr
//	Descrption:     把UTF8转换成WCHAR
//	Param:
//	   	OUT         wstr_ptr:保存转换字符串结果的buff
//	   	IN          wstr_len:buffer 的长度
//	   	IN          utf8_ptr:需要转换的字符串的地址
//	   	IN          utf8_len:想转换的长度

//	Return:         
//                  len : 返回转换成后的字符串长度(字符个数)
//  Note:
//----------------------------------------------------------------------------
MINT MAPI_GUI_UTF8ToWstr(MWCHAR* wstr_ptr, MUINT wstr_mem_size, const MCHAR* utf8_ptr, MUINT utf8_len)
{
	MINT	i = 0;
	MINT	wstr_lenth = 0;
	MINT	k = 0;
	MINT		max_len = 0;
	MWCHAR	ucs2_char = 0;
	MBYTE 	head_char = 0;
	
	if(MNULL == wstr_ptr || MNULL == utf8_ptr || wstr_mem_size < 2)
	{
		return wstr_lenth;
	}
	max_len = (wstr_mem_size - 2)/2;
	
	while(i < (MINT)utf8_len && wstr_lenth < max_len)
	{
		k = i;             
		head_char = utf8_ptr[i++];
		
		if (head_char <= 0x7F)
		{
			//0xxx xxxx
			//0111 1111 = 0X7F
			wstr_ptr[wstr_lenth ++] = head_char;
		}
		else if(head_char <= 0xDF)
		{
			if (i < (MINT)utf8_len)
			{
				//110x xxxx 10xx xxxx
				//1101 1111	== 0xDF
				//0001 1111 == 0x1F		0011 1111 == 0x3F
				ucs2_char = (uint16)(((head_char & 0x1F) << 6) | ((utf8_ptr[i++] & 0x3F)));
				wstr_ptr[wstr_lenth++] = ucs2_char;
			}
			else
			{
				break;
			}						
		}
		else if(head_char <= 0xEF)
		{
			if (i + 1 < (MINT)utf8_len)
			{
				//1110 xxxx 10xx xxxx 10xx xxxx
				//1110 1111 == 0xEF
				//0000 1111 == 0xF	0011 1111 == 0x3F
				ucs2_char = (uint16)(((head_char & 0xF) << 12) | ((utf8_ptr[i] & 0x3F) << 6) | (utf8_ptr[i + 1] & 0x3F));
				i += 2;
				wstr_ptr[wstr_lenth++] = ucs2_char;
			}
			else
			{
				break;
			}
			
		}
		else if(head_char <= 0xF7)
		{
			//1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx
			//1111 0111 == 0xF7
			//0000 0111 == 0x7	0011 1111 == 0x3F
			if (i + 1 < (MINT)utf8_len)
			{
				ucs2_char = (uint16)(((head_char & 0x7) << 18) | ((utf8_ptr[i] & 0x3F) << 12) | ((utf8_ptr[i+1] & 0x3F) << 6) | (utf8_ptr[i + 2] & 0x3F));
				i += 3;
				wstr_ptr[wstr_lenth++] = ucs2_char;
			}
			else
			{
				break;
			}
		}
		else if(head_char <= 0xFB)
		{
			//1111 10xx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx
			//1111 1011 == 0xFB
			//0000 0011 == 0x3	0011 1111 == 0x3F
			if (i + 1 < (MINT)utf8_len)
			{
				ucs2_char = (uint16)(((head_char & 0x3) << 24) | ((utf8_ptr[i] & 0x3F) << 18) | ((utf8_ptr[i+1] & 0x3F) << 12) 
					| ((utf8_ptr[i+2] & 0x3F) << 6) | (utf8_ptr[i + 3] & 0x3F));
				i += 4;
				wstr_ptr[wstr_lenth++] = ucs2_char;
			}
			else
			{
				break;
			}
			
		}
		else if(head_char <= 0xFD)
		{
			//1111 110x 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx
			//1111 1101 == 0xFD
			//0000 0001 == 0x1	0011 1111 == 0x3F
			if (i + 1 < (MINT)utf8_len)
			{
				ucs2_char = (uint16)(((head_char & 0x3) << 30) | ((utf8_ptr[i] & 0x3F) << 24) | ((utf8_ptr[i+1] & 0x3F) << 18) 
					| ((utf8_ptr[i+2] & 0x3F) << 12) | ((utf8_ptr[i+3] & 0x3F) << 6) | (utf8_ptr[i + 4] & 0x3F));
				i += 5;
				wstr_ptr[wstr_lenth++] = ucs2_char;
			}
			else
			{
				break;
			}
		}
		else
		{
			MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"MAPI_GUI_UTF8ToWstr: i %d char %x", i, head_char);
			break;
		}
		k = i;
	}
	wstr_ptr[wstr_lenth] = L'\0';
	return wstr_lenth;
}

//---------------------------------------------------------------------------
//	FUNCTION:       MAPI_GUI_WstrToUTF8
//	Descrption:     把WCHAR转换成UTF8
//	Param:
//	   	OUT         utf8_ptr:保存转换字符串结果的buff
//	   	IN          utf8_buf_len:buffer 的长度
//	   	IN          wstr_ptr:需要转换的字符串的地址
//	   	IN          wstr_len:想转换的长度

//	Return:         
//                  len : 返回转换成后的字符串长度(字符个数)
//  Note:
//----------------------------------------------------------------------------
MINT MAPI_GUI_WstrToUTF8(MCHAR* utf8_ptr, MUINT utf8_buf_len, const MWCHAR* wstr_ptr, MUINT wstr_len)
{
	MWCHAR	ucs2_char = 0;	//here, we only consider UCS2. UCS4 is not considered.
	MINT	i = 0;
	MINT	str_lenth = 0;
	MINT	max_len = 0;
	
	if(MNULL == wstr_ptr || MNULL == utf8_ptr || utf8_buf_len < 1)
	{
		return str_lenth;
	}
	
	max_len = utf8_buf_len - 1;
	
	for(i = 0; i < (MINT)wstr_len; i++)
	{
		//get unicode character
		ucs2_char = wstr_ptr[i];
		
		if(ucs2_char <= 0x7F)
		{
			//0xxx xxxx 
			//0111 1111 == 0x7F
			if(str_lenth >= max_len)
			{
				break;
			}
			utf8_ptr[str_lenth++] = (MCHAR)(ucs2_char & 0x7F);
		}
		else if(ucs2_char <= 0x7FF)
		{
			if(str_lenth + 1 >= max_len)
			{
				break;
			}
			//110x xxxx 10xx xxxx
			//11位拆成5，6位
			//0000 0111 1100 0000 == 0x7C0	110 == 0x6	//取出高5位＋(110 << 5)
			utf8_ptr[str_lenth++] = (MCHAR)(((ucs2_char & 0x7C0) >> 6) | (0x6 << 5));			
			//0000 0000 0011 1111 = 0x3F	10 == 0x2	//取出低6位＋(10 << 6)
			utf8_ptr[str_lenth++] = (MCHAR)((ucs2_char & 0x3F) | (0x2 << 6));
		}
		else
		{
			if(str_lenth + 2 >= max_len)
			{
				break;
			}
			//1110 xxxx 10xx xxxxxx 10xx xxxxxx
			//16位拆成4, 6, 6
			//1111 0000 0000 0000 == 0xF000		1110 == 0xE	//取出高4位 ＋ (1110 << 4)
			utf8_ptr[str_lenth++] = (MCHAR)(((ucs2_char & 0xF000) >> 12) | (0xE << 4));
			//0000 1111 1100 0000 == 0xFC0		10 = 0x2	//中间6位 ＋ (10 << 6)
			utf8_ptr[str_lenth++] = (MCHAR)(((ucs2_char & 0xFC0 ) >> 6) | (0x2 << 6));
			//0000 0000 0011 1111 == 0x3F		10 = 0x2	//最后6位 ＋ (10 << 6)
			utf8_ptr[str_lenth++] = (MCHAR)((ucs2_char & 0x3F) | (0x2 << 6));
		}
	}
	utf8_ptr[str_lenth] = '\0';
	return str_lenth;
}

//=======================McfChset.c API End============================= 


//=======================McfDtcnt.c API BEGIN============================


#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)
ERR_MNGPRS_CODE_E MAPI_MNGPRS_SetAndActivePdpContextEx(MN_DUAL_SYS_E dual_sys, MBYTE* APNcn, MN_PS_SERVICE_TYPE_E ps_service_type)
{
#ifdef PRODUCT_DM
	return MNGPRS_SetAndActivePdpContextEx(dual_sys,APNcn, ps_service_type);
#else
	return MNGPRS_SetAndActivePdpContextEx(dual_sys,APNcn);
#endif
}

ERR_MNGPRS_CODE_E MAPI_MNGPRS_ResetAndDeactivePdpContextEx(MN_DUAL_SYS_E dual_sys)
{
	return MNGPRS_ResetAndDeactivePdpContextEx(dual_sys);
}

MINT McfCmn_GetMCareGprsID(MVOID)
{
	return MMICMSWAP_MCARE;
}

#endif//#ifdef SPREAD_PLAT_SC6600L_1025



MUSHORT McfCmn_GetAppMnSignal(MUSHORT appMnSignal)
{
	MUINT i = 0;
	
	for(i = 0; i < McfCmn_MN_PDP_CONTEXT_MAX; i++)
	{
		if(appMnSignal == appMnPdpContextMap[i][1])
		{
			return appMnPdpContextMap[i][0];
		}
	}
	
	return McfCmn_MN_PDP_CONTEXT_MAX;
}

MUSHORT McfCmn_GetMnGprsErr(MUSHORT mnGprsErr)
{
	MUINT i = 0;
	
	for(i = 0; i < McfCmn_GPRS_ERR_MAX; i++)
	{
		if(mnGprsErr == mnGprsErrMap[i][1])
		{
			return mnGprsErrMap[i][0];
		}
	}
	
	return McfCmn_GPRS_ERR_MAX;
}

ERR_MNGPRS_CODE_E McfCmn_MNGPRS_No_error_code(MVOID)
{
	return ERR_MNGPRS_NO_ERR;
}


MN_GPRS_ERR_E McfCmn_MNGPRS_ERR_SUC(MVOID)
{
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC6610) || (defined SPREAD_PLAT_SC8800G)\
|| (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
	return MMIPDP_RESULT_SUCC;
#else
	return MN_GPRS_ERR_SUCCESS;
#endif
}


MUSHORT McfDcnt_GetAppMnSignal(MUSHORT appMnSignal)
{
	MUINT i = 0;
	
	for(i = 0; i < McfCmn_MN_PDP_CONTEXT_MAX; i++)
	{
		if(appMnSignal == appMnPdpContextMap[i][1])
		{
			return appMnPdpContextMap[i][0];
		}
	}
	
	return McfCmn_MN_PDP_CONTEXT_MAX;
}

MINT McfIF_GetModuleID(MVOID)
{
	return MMI_MODULE_MCARE_MINI;
}


#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC6610) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
MBYTE McfIF_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
	MBYTE index = 0;
	//待有统一get setting 的接口后再修正
#ifdef BROWSER_SUPPORT
#ifdef SPREAD_PLAT_SC6530
    index = MMIAPIBROWSER_GetNetSettingIndex(dual_sys);
#else
	index = MMIBROWSER_GetNetSettingIndex(dual_sys);
#endif
#endif
	return index;
}
MINT McfIF_GetPDPInterfaceWIFI(MVOID)
{
	return MMIPDP_INTERFACE_WIFI;
}
MINT McfIF_GetPDPInterfaceGPRS(MVOID)
{
	return MMIPDP_INTERFACE_GPRS;
}
MINT McfIF_GetPSServiceRat(MVOID)
{
	return MN_UNSPECIFIED;
}

ERR_MNGPRS_CODE_E MAPI_MNGPRS_ReadStorageExtReqQosEx(
													 MN_DUAL_SYS_E		   	dual_sys,
													 MN_GPRS_STORAGE_E      	storage,
													 MN_PS_SERVICE_TYPE_E   	ps_service_type,
													 MN_GPRS_EXT_QOS_INFO_T*	qos_info
													 )
{
	return MNGPRS_ReadStorageExtReqQosEx(   dual_sys, storage, ps_service_type,qos_info);
}  

MN_GPRS_STORAGE_E McfDcnt_GetGprsType()
{
	return MN_GPRS_STORAGE_ALL;
}


MMICONNECTION_LINKSETTING_DETAIL_T* MAPI_CONNECTION_GetLinkSettingItemByIndex(MN_DUAL_SYS_E dual_sys,uint8 setting_index)
{
#if (defined SPREAD_PLAT_SC6800H_W1122) || (defined SPREAD_PLAT_SC6800H_PDA) || (defined SPREAD_PLAT_SC8800G_PDA)
	return MMICONNECTION_GetLinkSettingItemByIndex(dual_sys,setting_index);
#elif defined SPREAD_PLAT_SC6530
	return MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, setting_index);
#else
	return MMICONNECTION_GetLinkSettingItemByIndex(setting_index);
#endif
}

BOOLEAN MAPI_MMIAPIPDP_Active(MMIPDP_ACTIVE_INFO_T* app_info_ptr)
{
    SCI_TRACE_LOW("MAPI_MMIAPIPDP_Active handle_msg_callback = %x",app_info_ptr->handle_msg_callback);
	return MMIAPIPDP_Active(app_info_ptr);
}

BOOLEAN MAPI_MMIAPIPDP_Deactive(MUINT app_handler)
{
	return MMIAPIPDP_Deactive(app_handler);
}

#endif//#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC6610) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC6530) 


MN_PS_SERVICE_TYPE_E McfDcnt_GetSerivceType(MVOID)
{
	//TD 下，GSM网络上网慢的问题修改
	return BROWSER_E;//IM_E;
}

//=======================McfDtcnt.c API END==========================  

//=====================McfEditor.c API BEGIN==========================

MMI_HANDLE_T MAPI_MMK_CreateWin(MUINT* win_table_ptr,ADD_DATA add_data_ptr)
{
	return MMK_CreateWin((MUINT*)win_table_ptr,add_data_ptr);
}

#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
static MMI_RESULT_E McfEditor_HandleEditBoxWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E 	result = MMI_RESULT_TRUE;
	MMI_TEXT_ID_T 	title_id = TXT_NULL;
	
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"zhukey HandleEditBoxWinMsg msg_id = 0x%x, win_id = %d", msg_id, win_id);
	MCF_SCI_PASSERT(MMIMCARE_EDITBOX_WIN_ID == win_id, ("mcf zhu HandleEditBoxWinMsg is fail!"));
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		pStr_tmp = MNULL;
		title_id = (MMI_TEXT_ID_T)MMK_GetWinAddDataPtr(win_id);
		GUIWIN_SetTitleTextId(win_id, title_id, MFALSE);
		MMK_SetAtvCtrl(win_id, MMIMCR_EDIT_CONTROL_ID);
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_MIDSK:
		McfStdC_memset(&s_str_info, 0, sizeof(s_str_info));
		if (EDITBOX_PHONENUM_E == GUIEDITBOX_GetType(GUIEDITBOX_GetEditBoxPtr(MMIMCR_EDIT_CONTROL_ID)))
		{
			GUIEDITBOX_GetPhoneNumEditBoxInfoExt(MMIMCR_EDIT_CONTROL_ID, &s_str_info);
		}
		else if (EDITBOX_PASSWORD_E == GUIEDITBOX_GetType(GUIEDITBOX_GetEditBoxPtr(MMIMCR_EDIT_CONTROL_ID)))
		{
			GUIEDITBOX_GetPasswordEditBoxInfo(MMIMCR_EDIT_CONTROL_ID, &s_str_info);
		}
		else
		{
			GUIEDITBOX_GetTextEditBoxInfo(MMIMCR_EDIT_CONTROL_ID, &s_str_info);
		}
		
		g_mc_editcallback.callback_param.len = s_str_info.wstr_len;
		{
			pStr_tmp = (MWCHAR*)MAPI_SCI_ALLOCA_ByZero((s_str_info.wstr_len+1)*2);
			if(MNULL == pStr_tmp)
			{
				return MMI_RESULT_FALSE;//????
			}
			//McfStdC_memset(pStr_tmp, 0, (s_str_info.wstr_len+1)*2);
			MMIAPICOM_Wstrncpy(pStr_tmp, s_str_info.wstr_ptr, s_str_info.wstr_len);
		}
		g_mc_editcallback.callback_param.pdata = pStr_tmp;
		if(g_mc_editcallback.callback)
		{
			MUINT keyCode = 0;
			MUINT keyType = 0;	
			McfKey_GetkeyInfo(&keyCode, &keyType, msg_id);			
			(*g_mc_editcallback.callback)(g_mc_editcallback.callback_param.pUser, g_mc_editcallback.callback_param.pdata
				, g_mc_editcallback.callback_param.len, keyCode);
		}
		MMK_CloseWin(win_id);
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	case MSG_KEYUP_RED:
		if(g_mc_editcallback.callback)
		{
			MUINT keyCode = 0;
			MUINT keyType = 0;	
			McfKey_GetkeyInfo(&keyCode, &keyType, msg_id);			
			(*g_mc_editcallback.callback)(g_mc_editcallback.callback_param.pUser,MNULL,0, keyCode);
		}
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		MAPI_SCI_FREE(pStr_tmp);
		pStr_tmp = MNULL;
		break;
	case MSG_KEYLONG_WEB:
	case MSG_KEYPRESSUP_WEB:
		result = MMI_RESULT_TRUE;
		break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return result;
}

MINT McfEditor_CreateEditBox(MMI_STRING_T *text_ptr,
							 MUSHORT max_alpha_len,
							 MUSHORT max_ucs2_len,
							 MBYTE input_type,
							 MMI_WIN_ID_T win_id)
{
	GUI_FONT_T              font_type = MMI_DEFAULT_TEXT_FONT;
	MMI_CONTROL_CREATE_T    create = {0};
	GUI_EDITBOX_INIT_DATA_T init_data = {0};
	MMI_CTRL_ID_T           edit_ctrl_id = MMIMCR_EDIT_CONTROL_ID;
    MBYTE                   first_im = 0;
    MBYTE                   all_im = 0;
    MBYTE                   edit_type = 0;
    
	GUI_RECT_T  rect = {MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT, MMI_EDITBOX_FULLSCREEN_CLIENT_TOP
		, MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT, MMI_EDITBOX_FULLSCREEN_CLIENT_BOTTOM}; 
#ifdef PASSWORD_EDITOR_ENHANCE      //nicklei add about password
	MBOOL	isPasswordFlag = MFALSE;
#endif	
	MCF_SCI_PASSERT(MMIMCARE_EDITBOX_WIN_ID == win_id, ("mcf zhu HandleEditBoxWinMsg  CreateEditBox is fail!"));
	
	if(!MMK_IsOpenWin(win_id))
	{
		MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"zhukey CreateEditBox window already opened !");
		return -1;
	}
	if(!McfEditor_GetMEditorType(input_type,&edit_type,&all_im,&first_im))
	{
		MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"CreateEditBox window McfEditor_GetMEditorType Fail !");
		return -1;
	}

	if(input_type == MCFEDIT_TY_LETTERS_P)
	{
#ifdef PASSWORD_EDITOR_ENHANCE      //nicklei add about password
		isPasswordFlag = MTRUE;
#endif
	}
	init_data.max_str_len = max_alpha_len;
	init_data.add_data.text_add.max_ucs2_len = max_ucs2_len;
	init_data.add_data.text_add.max_default_len = max_alpha_len;
	init_data.add_data.text_add.allow_inputmethod = all_im;
	init_data.add_data.text_add.cur_inputmethod = first_im;

	if (EDITBOX_PASSWORD_E == edit_type || EDITBOX_PHONENUM_E == edit_type)
	{
		rect.top = MMI_TITLE_HEIGHT;
		rect.bottom = MMI_CLIENT_RECT_BOTTOM;
	}

	init_data.type = edit_type;
	init_data.rect = &rect;
	init_data.wstr_ptr = text_ptr->wstr_ptr;
	init_data.wstr_len = text_ptr->wstr_len;
	
	create.ctrl_id = edit_ctrl_id;
	create.guid    = SPRD_GUI_EDITBOX_ID;
	create.parent_win_handle = win_id;//MCARE_EDITBOX_WIN_ID;
	create.init_data_ptr = &init_data;
		
	if(MNULL == MMK_CreateControl( &create ))
	{
		MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"zhukey CreateEditBox edit control failed!");
		return -1;
	}
#ifdef PASSWORD_EDITOR_ENHANCE     //nicklei add about password
	    if(isPasswordFlag)
	    {
	        GUIEDITBOX_PasswordOn(MMIMCR_EDIT_CONTROL_ID);				
	        isPasswordFlag = MFALSE;
	    }
#endif

	MMK_SetAtvCtrl(win_id, edit_ctrl_id);
	return 0;
}
MBOOL McfEditor_GetMEditorType(MBYTE input_type, MBYTE* edit_type, 
                                                  MBYTE* all_im, MBYTE* first_im)
{
	MBOOL ret = MTRUE;
	if(MNULL==edit_type || MNULL==all_im || MNULL==first_im)
	{
	    MCF_SCI_ASSERT(MFALSE); 
		return MFALSE;
	}
	switch(input_type)
	{
	case MCFEDIT_TY_SYMBOLS:
	case MCFEDIT_TY_SYMBOLS_P:
		{
			MMISET_LANGUAGE_TYPE_E  lang_type = 0;
			MMIAPISET_GetLanguageType(&lang_type);
			if (MMISET_LANGUAGE_SIMP_CHINESE == lang_type)
			{
				*first_im = IM_SMART_MODE;
			}
			else
			{
				*first_im = IM_CHARACTER_MODE;
			}
			if(input_type == MCFEDIT_TY_SYMBOLS_P)
			{
				*edit_type = EDITBOX_PASSWORD_E;
			}
			else
			{
				*edit_type = EDITBOX_TEXT_E;
			}
			*all_im = (IM_SMART_TYPE | IM_MULTITAP_TYPE | IM_ENGLISH_TYPE | IM_DIGITAL_TYPE);
		}
		break;
	case MCFEDIT_TY_LETTERS:
	case MCFEDIT_TY_LETTERS_P:
		{
			if(input_type == MCFEDIT_TY_LETTERS_P)
			{
				//*edit_type = EDITBOX_PASSWORD_E;
				//暂时使用EDITBOX_TEXT_E，因为平台支持的密码模式仅支持数字
				*edit_type = EDITBOX_TEXT_E;
			}
			else
			{
				*edit_type = EDITBOX_TEXT_E;
			}
			*all_im = (IM_MULTITAP_TYPE | IM_DIGITAL_TYPE);
			*first_im = IM_MULTITAP_MODE;
		}
		break;
	case MCFEDIT_TY_NUMBER:
	case MCFEDIT_TY_NUMBER_P:
		{
			if(input_type == MCFEDIT_TY_NUMBER_P)
			{
				*edit_type = EDITBOX_PASSWORD_E;
			}
			else
			{
			#if 1//(defined SPREAD_PLAT_SC6620)
				*edit_type = EDITBOX_TEXT_E;//所有平台都是为Text
			#else
				*edit_type = EDITBOX_PHONENUM_E;
			#endif
			}
			*first_im = IM_DIGITAL_MODE;
			*all_im = IM_DIGITAL_TYPE;
		}
		break;

	case MCFEDIT_TY_PINYIN:
	case MCFEDIT_TY_PINYIN_P:
		{
			if(input_type == MCFEDIT_TY_PINYIN_P)
			{
				*edit_type = EDITBOX_PASSWORD_E;
			}
			else
			{
				*edit_type = EDITBOX_TEXT_E;
			}
			*all_im = (IM_SMART_TYPE | IM_STROKE_TYPE | IM_MULTITAP_TYPE | IM_ENGLISH_TYPE | IM_DIGITAL_TYPE);
			*first_im = IM_SMART_MODE;
		}
		break;
	case MCFEDIT_TY_STROKE:
	case MCFEDIT_TY_STROKE_P:
		{
			if(input_type == MCFEDIT_TY_STROKE_P)
			{
				*edit_type = EDITBOX_PASSWORD_E;
			}
			else
			{
				*edit_type = EDITBOX_TEXT_E;
			}
			//*all_im = IM_STROKE_TYPE;//根据和章哥确认的信息，笔画需要加入数字和字母
			*all_im = (IM_STROKE_TYPE | IM_MULTITAP_TYPE | IM_DIGITAL_TYPE);
			*first_im = IM_STROKE_MODE;
		}
		break;
	default:
		ret = MFALSE;
		break;
	}
	return ret;
}

#else//#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)
static MMI_RESULT_E McfEditor_HandleEditBoxWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E   result = MMI_RESULT_TRUE;
	MMI_TEXT_ID_T  title_id = TXT_NULL;
	//GUIEDIT_CTRL_T* edit_ctrl_ptr = MNULL;
	
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"zhukey HandleEditBoxWinMsg msg_id = 0x%x, win_id = %d", msg_id, win_id);
	MCF_SCI_PASSERT(MMIMCARE_EDITBOX_WIN_ID == win_id, ("mcf zhu HandleEditBoxWinMsg is fail!"));
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		pStr_tmp = MNULL;
		title_id = (MMI_TEXT_ID_T)MMK_GetWinAddDataPtr(win_id);
		GUIWIN_SetTitleTextId(win_id, title_id, MFALSE);
		MMK_SetAtvCtrl(win_id, MMIMCR_EDIT_CONTROL_ID);
		break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
		if (MNULL != param)
		{
			MUINT src_id = ((MMI_NOTIFY_T*)param)->src_id;
			switch(src_id)
			{
			case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
				MMK_SendMsg(win_id, MSG_APP_OK, MNULL);
				break;
			case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
				break;
			case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
				MMK_SendMsg(win_id, MSG_APP_CANCEL, MNULL);
				break;
			default:
				break;
			}
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
	case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
		McfStdC_memset(&s_str_info, 0, sizeof(s_str_info));
		if (GUIEDIT_TYPE_PHONENUM == GUIEDIT_GetType(MMIMCR_EDIT_CONTROL_ID))
		{
			GUIEDIT_GetPhoneNumString(MMIMCR_EDIT_CONTROL_ID, &s_str_info);
		}
		else if (GUIEDIT_TYPE_PASSWORD== GUIEDIT_GetType(MMIMCR_EDIT_CONTROL_ID))
		{
			GUIEDIT_GetString(MMIMCR_EDIT_CONTROL_ID, &s_str_info);
		}
		else
		{
			GUIEDIT_GetString(MMIMCR_EDIT_CONTROL_ID, &s_str_info);
		}
		
		g_mc_editcallback.callback_param.len = s_str_info.wstr_len;
		{
			pStr_tmp = (MWCHAR*)MAPI_SCI_ALLOCA_ByZero((s_str_info.wstr_len+1)*2);
			if(MNULL == pStr_tmp)
			{
				return MMI_RESULT_FALSE;//????
			}
			//McfStdC_memset(pStr_tmp, 0, (s_str_info.wstr_len+1)*2);
			MMIAPICOM_Wstrncpy(pStr_tmp, s_str_info.wstr_ptr, s_str_info.wstr_len);
		}
		g_mc_editcallback.callback_param.pdata = pStr_tmp;
		
		if(g_mc_editcallback.callback)
		{
			MUINT keyCode = 0;
			MUINT keyType = 0;
			McfKey_GetkeyInfo(&keyCode, &keyType, msg_id);			
			(*g_mc_editcallback.callback)(g_mc_editcallback.callback_param.pUser, g_mc_editcallback.callback_param.pdata, g_mc_editcallback.callback_param.len, keyCode);
		}
#ifdef SPREAD_PLAT_SC6530
		{
		//关于展讯editbug导致花屏的补丁处理
			MMI_HANDLE_T handle = MMK_GetCtrlHandleByGuid(win_id, MMIMCR_EDIT_CONTROL_ID);
			if(0 != handle)
			{
				GUICTRL_PostNotifyEx(handle, MSG_NOTIFY_IM_HIDE, MNULL, 0);
			}
		}
#endif
		MMK_CloseWin(win_id);
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	case MSG_KEYUP_RED:
		if(g_mc_editcallback.callback)
		{
			MUINT keyCode = 0;
			MUINT keyType = 0;	
			McfKey_GetkeyInfo(&keyCode, &keyType, msg_id);			
			(*g_mc_editcallback.callback)(g_mc_editcallback.callback_param.pUser,MNULL, 0, keyCode);
		}
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		MAPI_SCI_FREE(pStr_tmp);
		pStr_tmp = MNULL;
		break;
	case MSG_KEYLONG_WEB:
	case MSG_KEYPRESSUP_WEB:
		result = MMI_RESULT_TRUE;
		break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return result;
}

MINT McfEditor_CreateEditBox(MMI_STRING_T *text_ptr,
							 MUSHORT max_alpha_len,
							 MUSHORT max_ucs2_len,
							 MBYTE  input_type,
							 MMI_WIN_ID_T win_id)
{
	GUI_FONT_T              font_type = MMI_DEFAULT_TEXT_FONT;
	MMI_CONTROL_CREATE_T    create = {0};
	GUIEDIT_INIT_DATA_T     init_data = {0};
	MMI_CTRL_ID_T           edit_ctrl_id = MMIMCR_EDIT_CONTROL_ID;
    MBYTE                   first_im = 0;
    MBYTE                   all_im = 0;
    MBYTE                   edit_type = 0;
	
	
	if(!MMK_IsOpenWin(win_id))
	{
		MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"CreateEditBox window already opened !");
		return -1;
	}
	if(!McfEditor_GetMEditorType(input_type,&edit_type,&all_im,&first_im))
	{
		MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"CreateEditBox window McfEditor_GetMEditorType Fail !");
		return -1;
	}
	
	init_data.type = (GUIEDIT_TYPE_E)edit_type;
	init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
	init_data.str_max_len = max_ucs2_len;
#ifdef PDA_UI_SUPPORT_ANDROID  //cr252401
#ifdef PDA_UI_ANDROID_SOFTKEY
	init_data.both_rect.v_rect.bottom = McfGdi_GetScrHeight() - McfIF_GetSoftbarHeight() - 1;
#else
	init_data.both_rect.v_rect.bottom = McfGdi_GetScrHeight() - McfIF_GetFullSoftbarHeight() - 1;
#endif
#endif
	create.ctrl_id = edit_ctrl_id;
	create.guid = SPRD_GUI_EDITBOX_ID;
	create.parent_win_handle = win_id;//MCARE_EDITBOX_WIN_ID;
	create.init_data_ptr = &init_data;
		
	if(MNULL == MMK_CreateControl( &create ))
	{
		MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"CreateEditBox edit control failed!");
		return -1;
	}
	
	GUIEDIT_SetFont(edit_ctrl_id, font_type);
	
	if(MCFEDIT_TY_LETTERS_P == input_type)
	{
		GUIEDIT_SetAlign(edit_ctrl_id, ALIGN_LEFT);//靠左上起	
	}
	
	GUIEDIT_SetIm(edit_ctrl_id, (GUIIM_TYPE_T)all_im, (GUIIM_TYPE_T)first_im);
    //设置纯数字
	if(all_im == GUIIM_TYPE_DIGITAL)
    {
        GUIEDIT_SetImTag(edit_ctrl_id, GUIIM_TAG_PURE_NUM);
    }
    
	MMK_SetAtvCtrl(win_id, edit_ctrl_id);
	if (MNULL != text_ptr)
	{
		GUIEDIT_SetString(create.ctrl_id,text_ptr->wstr_ptr,text_ptr->wstr_len);
		return 0;
	}
	else
	{
		return -1;
	}
}

MBOOL McfEditor_GetMEditorType(MBYTE input_type, MBYTE* edit_type, 
                                                  MBYTE* all_im, MBYTE* first_im)
{
	MBOOL ret = MTRUE;
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfEditor_GetMEditorType input_type = %d", input_type);
	if(MNULL==edit_type || MNULL==all_im || MNULL==first_im)
	{
	    MCF_SCI_ASSERT(MFALSE); 
		return MFALSE;
	}
	switch(input_type)
	{
	case MCFEDIT_TY_SYMBOLS:
	case MCFEDIT_TY_SYMBOLS_P:
		{
			MMISET_LANGUAGE_TYPE_E  lang_type = 0;
			MMIAPISET_GetLanguageType(&lang_type);
			if (MMISET_LANGUAGE_SIMP_CHINESE == lang_type)
			{
				*first_im = GUIIM_TYPE_SMART;
			}
			else
			{
				*first_im = GUIIM_TYPE_ABC;
			}
			if(input_type == MCFEDIT_TY_SYMBOLS_P)
			{
				*edit_type = GUIEDIT_TYPE_PASSWORD;
			}
			else
			{
				*edit_type = GUIEDIT_TYPE_TEXT;
			}
			*all_im = GUIIM_TYPE_SET_ALL;
		}
		break;
	case MCFEDIT_TY_LETTERS:
	case MCFEDIT_TY_LETTERS_P:
		{
			if(input_type == MCFEDIT_TY_LETTERS_P)
			{
				*edit_type = GUIEDIT_TYPE_PASSWORD;
			}
			else
			{
				*edit_type = GUIEDIT_TYPE_TEXT;
			}
			*all_im = GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_MULTITAP;
			*first_im = GUIIM_TYPE_ABC;
		}
		break;
	case MCFEDIT_TY_NUMBER:
	case MCFEDIT_TY_NUMBER_P:
		{
			if(input_type == MCFEDIT_TY_PINYIN_P)
			{
				*edit_type = GUIEDIT_TYPE_PASSWORD;
			}
			else
			{
				*edit_type = GUIEDIT_TYPE_TEXT;
			}
			*first_im = GUIIM_TYPE_DIGITAL;
			*all_im = GUIIM_TYPE_DIGITAL;
		}
		break;

	case MCFEDIT_TY_PINYIN:
	case MCFEDIT_TY_PINYIN_P:
		{
			if(input_type == MCFEDIT_TY_PINYIN_P)
			{
				*edit_type = GUIEDIT_TYPE_PASSWORD;
			}
			else
			{
				*edit_type = GUIEDIT_TYPE_TEXT;
			}
			*all_im = GUIIM_TYPE_SET_ALL;
			*first_im = GUIIM_TYPE_SMART;
		}
		break;
	case MCFEDIT_TY_STROKE:
	case MCFEDIT_TY_STROKE_P:
		{
			if(input_type == MCFEDIT_TY_STROKE_P)
			{
				*edit_type = GUIEDIT_TYPE_PASSWORD;
			}
			else
			{
				*edit_type = GUIEDIT_TYPE_TEXT;
			}
			//*all_im = GUIIM_TYPE_STROKE;//根据和章哥确认的信息，笔画需要加入数字和字母
			*all_im = GUIIM_TYPE_STROKE | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_MULTITAP | GUIIM_TYPE_ABC;
			*first_im = GUIIM_TYPE_STROKE;
		}
		break;
	default:
		ret = MFALSE;
		break;
	}
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfEditor_GetMEditorType edit_type = %d, all_im= %d, first_im =%d", *edit_type, *all_im, *first_im);
	return ret;
}

#endif//#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)

MVOID McfEditor_SetCallBack(MVOID* pUser, pfnMcfInputCb func)
{
	g_mc_editcallback.callback_param.pUser = pUser;
	g_mc_editcallback.callback = func;
}

MUINT McfEditor_GetMEditorWinTab(MVOID)
{
	return (MUINT)MMIMCARE_EDITBOX_WIN_TAB;
}

MUINT McfEditor_GetMEditorWinID(MVOID)
{
	return MMIMCARE_EDITBOX_WIN_ID;
}
//======================McfEditor.c API END==========================  

//=======================McfFs.c Begin=============================
SFS_ERROR_E MAPI_SFS_CloseFile(SFS_HANDLE handle)
{
	return SFS_CloseFile(handle);
}

SFS_ERROR_E MAPI_SFS_ReadFile(
							  SFS_HANDLE 			handle,
							  MVOID* 				buffer,
							  MUINT 				bytes_to_read,
							  MUINT* 				bytes_read,
							  SFS_OVERLAPPED_T* 	overlapped
							  )
{	
	return SFS_ReadFile(handle,buffer,bytes_to_read,bytes_read,overlapped);	
}

SFS_ERROR_E MAPI_SFS_WriteFile(
							   SFS_HANDLE 			handle,
							   const MVOID* 			buffer,
							   MUINT 					bytes_to_write,
							   MUINT* 				bytes_written,
							   SFS_OVERLAPPED_T* 		overlapped
							   )
{
	
	return SFS_WriteFile(handle,buffer,bytes_to_write,bytes_written,overlapped);	
}

SFS_ERROR_E MAPI_SFS_DeleteFile(const MWCHAR* name,SFS_OVERLAPPED_T* overlapped_ptr)
{
	return SFS_DeleteFile(name,overlapped_ptr);
}

SFS_ERROR_E MAPI_SFS_FlushFile(SFS_HANDLE handle,SFS_OVERLAPPED_T* overlapped)
{
	return SFS_FlushFile(handle,overlapped);
}

SFS_ERROR_E MAPI_SFS_GetFileSize(SFS_HANDLE sfs_handle, MUINT* size)
{
	return SFS_GetFileSize(sfs_handle,size);
}

SFS_ERROR_E MAPI_SFS_SetFileSize(SFS_HANDLE sfs_handle, MUINT size)
{
	return SFS_SetFileSize(sfs_handle,size);
}

SFS_ERROR_E MAPI_SFS_SetFilePointer(SFS_HANDLE handle, MLONG offset,MUINT origin)
{
	return SFS_SetFilePointer(handle,offset,origin);
}

SFS_HANDLE MAPI_SFS_FindFirstFile(const MWCHAR* find, SFS_FIND_DATA_T* fd)
{
	return SFS_FindFirstFile(find,fd);
}

SFS_ERROR_E MAPI_SFS_FindNextFile(SFS_HANDLE sfs_handle, SFS_FIND_DATA_T* fd)
{
	return SFS_FindNextFile(sfs_handle,fd);
}

SFS_ERROR_E MAPI_SFS_FindClose(SFS_HANDLE sfs_handle)
{
	return SFS_FindClose(sfs_handle);
}

SFS_ERROR_E MAPI_SFS_GetFilePointer(SFS_HANDLE handle, MUINT origin, MINT* currentpos)
{
	return SFS_GetFilePointer(handle,origin,currentpos);
}

SFS_ERROR_E MAPI_SFS_GetDeviceFreeSpace(const MWCHAR* device_name, MUINT* high_word, MUINT* low_word)
{
	return SFS_GetDeviceFreeSpace(device_name,high_word,low_word);
}

SFS_ERROR_E MAPI_SFS_GetDeviceStatus(const MWCHAR* dev_name)
{
	return SFS_GetDeviceStatus(dev_name);
}

SFS_ERROR_E MAPI_SFS_CreateDirectory(const MWCHAR* path)
{
	return SFS_CreateDirectory(path);
}

SFS_ERROR_E MAPI_SFS_RenameFile(const MWCHAR* sour,const MWCHAR* dest,SFS_OVERLAPPED_T* overlapped_ptr)
{
	return SFS_RenameFile(sour,dest,overlapped_ptr);
}

SFS_ERROR_E MAPI_SFS_DeleteDirectory(const MWCHAR* path)
{
	return SFS_DeleteDirectory(path);
}

MBOOL MAPI_MMIFILE_IsFileExist(const MWCHAR* full_path_ptr,MUSHORT full_path_len)
{
	return MMIFILE_IsFileExist(full_path_ptr,full_path_len);
}

MBOOL MAPI_MMIFILE_IsFolderExist(const MWCHAR* full_path_ptr,MUSHORT full_path_len)
{
	return MMIFILE_IsFolderExist(full_path_ptr,full_path_len);
}

MMIFMM_OPENWIN_RET_E MAPI_MMIFMM_OpenFileWinByLayer(
													MWCHAR* 				path_ptr,      //默认打开目录，如果是NULL，则从根目录打开
													MUSHORT 				path_len,
													MMIFMM_FILTER_T*		filter_ptr,    //筛选参数
													MMIFMM_FUNC_TYPE_E       func_type,      //类型
													MBOOL                 		is_multi_sel,   //是否多选
													PROCESSMSG_FUNC         	handle_func,    //自定义处理函数
													FILEARRAY               		ret_array,      //输出数组
													MMIFMM_OpenFileCallBack 	callback        //回调函数
													)
{
#if (defined SPREAD_PLAT_SC6530)
	return MMIAPIFMM_OpenFileWinByLayer(path_ptr,path_len,filter_ptr,func_type,is_multi_sel,handle_func,ret_array,callback, MTRUE);
#else
	return MMIFMM_OpenFileWinByLayer(path_ptr,path_len,filter_ptr,func_type,is_multi_sel,handle_func,ret_array,callback);
#endif
}

MVOID MAPI_MMIFILEARRAY_Destroy(FILEARRAY* file_array_ptr)
{
#ifdef SPREAD_NEW_FS_UPDATE
	MMIAPIFILEARRAY_Destroy(file_array_ptr);
#else
	MMIFILEARRAY_Destroy(file_array_ptr);
#endif
}

MUINT MAPI_MMIFILEARRAY_GetArraySize(FILEARRAY file_array)
{
#ifdef SPREAD_NEW_FS_UPDATE
	return MMIAPIFILEARRAY_GetArraySize(file_array);
#else
	return MMIFILEARRAY_GetArraySize(file_array);
#endif
}

MBOOL MAPI_MMIFILEARRAY_Read(FILEARRAY file_array, MUINT index, FILEARRAY_DATA_T* data_ptr)
{
#ifdef SPREAD_NEW_FS_UPDATE
	return MMIAPIFILEARRAY_Read(file_array,index,data_ptr);
#else
	return MMIFILEARRAY_Read(file_array,index,data_ptr);
#endif
}

FILEARRAY MAPI_MMIFILEARRAY_Create(void) //返回分配的指针
{
	return MMIAPIFILEARRAY_Create();
}

MBOOL MAPI_MMIAPICOM_CombinePath(
								 const MWCHAR*	dir_ptr,           //in
								 MUSHORT           	dir_len,            //in, 双字节为单位
								 const MWCHAR*	file_name_ptr,     //in, can be NULL
								 MUSHORT           	file_name_len,      //in, 双字节为单位
								 MBOOL          		is_extname,         //in, 是否为扩展名
								 MWCHAR*			full_path_ptr,     //out
								 MUSHORT*		full_path_len_ptr  //out, 双字节为单位
								 )
{
	return MMIAPICOM_CombinePath(dir_ptr,dir_len,file_name_ptr,file_name_len,is_extname,full_path_ptr,full_path_len_ptr);
}

MWCHAR McfCmn_GetCardDrv(MVOID)
{
#ifdef SPREAD_NEW_FS_UPDATE
#if(defined SPREAD_PLAT_SC6800H_W1122) || (defined SPREAD_PLAT_SC6800H_PDA) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
	MWCHAR *drv = MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);
	if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD))
		&& MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD_1)))
	{
		drv = MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);
	}
	else if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD))
		&& !MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD_1)))
	{
		drv = MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);
	}
	else if(!MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD))
		&& MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD_1)))
	{
		drv = MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1);	
	}
	else
	{}
	return *drv;
#else
	MWCHAR *drv = MMIFILE_DEVICE_SDCARD;
	return *drv;
#endif
#else//#ifdef SPREAD_NEW_FS_UPDATE
#if(defined SPREAD_PLAT_SC6800H_W1122) || (defined SPREAD_PLAT_SC6800H_PDA) || (defined SPREAD_PLAT_SC6530_PDA)|| (defined SPREAD_PLAT_SC8800G_PDA)
	MWCHAR *drv = MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD);
	if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD))
		&& MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD_1), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD_1)))
	{
		drv = MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD);
	}
	else if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD))
		&& !MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD_1), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD_1)))
	{
		drv = MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD);
	}
	else if(!MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD))
		&& MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD_1), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD_1)))
	{
		drv = MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD_1);	
	}
	else
	{}
	return *drv;
#else
	MWCHAR *drv = MMIFILE_DEVICE_SDCARD;
	return *drv;
#endif
#endif//#ifdef SPREAD_NEW_FS_UPDATE
}

MWCHAR McfCmn_GetPublicDrv(MVOID)
{
#ifdef SPREAD_NEW_FS_UPDATE
#if(defined SPREAD_PLAT_SC6800H_W1122) || (defined SPREAD_PLAT_SC6800H_PDA) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
	MWCHAR *drv = MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK);
	return *drv;
#else
	MWCHAR *drv = MMIFILE_DEVICE_UDISK;
	return *drv;
#endif
#else//#ifdef SPREAD_NEW_FS_UPDATE
#if(defined SPREAD_PLAT_SC6800H_W1122) || (defined SPREAD_PLAT_SC6800H_PDA) || (defined SPREAD_PLAT_SC6530_PDA)|| (defined SPREAD_PLAT_SC8800G_PDA)
	MWCHAR *drv = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
	return *drv;
#else
	MWCHAR *drv = MMIFILE_DEVICE_UDISK;
	return *drv;
#endif
#endif//#ifdef SPREAD_NEW_FS_UPDATE
}

MINT McfFile_return_error_code(SFS_ERROR_E ret)
{
	MINT erro_code=0;
	switch(ret)
	{
	case SFS_NO_ERROR://success
		erro_code= MFILE_SUCCESS;
		break;
	case SFS_ERROR_DEVICE://device error
		erro_code= MFILE_FAIL;
		break;
	case SFS_ERROR_SYSTEM://os error
		erro_code= MFILE_FAIL;
		break;
	case SFS_ERROR_INVALID_PARAM://param you give is invalid
		erro_code= MFILE_ERR_BAD_FSNAME;
		break;
	case SFS_ERROR_NO_ENOUGH_RESOURCE://memory or else is limited
		erro_code= MFILE_FAIL;
		break;
	case SFS_ERROR_IO_PENDING://when you call api in asynchronism ,api will return SFS_ERROR_IO_PENDING immediately.Real error message and return value will be given in callback funtion.
		erro_code= MFILE_FAIL;
		break;
	case SFS_ERROR_ACCESS://can not access the file ,maybe it is being used by other handler
		erro_code= MFILE_ERR_LOCKED;
		break;
	case SFS_ERROR_NOT_EXIST://file or device not exist
		erro_code= MFILE_ERR_NOT_FOUND;
		break;
	case SFS_ERROR_HAS_EXIST://file or device has exist
		erro_code= MFILE_ERR_ALREADY_EXIST;
		break;
		
	case SFS_ERROR_HARDWARE_FAILED://device-lay error code
		erro_code= MFILE_FAIL ;
		break;
	case SFS_ERROR_WRITE_CONFLICT://device-lay error code
		erro_code= MFILE_FAIL ;
		break;
	case SFS_ERROR_NO_SPACE://item-lay error code
		erro_code= MFILE_ERR_DISK_FULL;
		break;
	default :
		erro_code= MFILE_FAIL;     
		break;
	}
	MCFAPI_FILE_PRINT(ALOG_INFO,MLOG_SDKFILE, __MCR_FUNCTION__"[MCFFS]McfFile_return_error_code erro_code = %d, ret = %d", erro_code, ret);
	return erro_code;
}

MINT McfFile_GetSeekOrigin(MINT SeekType)
{
	MINT seek_type = MFILE_ERR_BAD_FSNAME;
	switch(SeekType)
	{
	case MFILE_SEEK_SET:
		seek_type = SFS_SEEK_BEGIN;
		break;
	case MFILE_SEEK_CUR:
		seek_type = SFS_SEEK_CUR;
		break;
	case MFILE_SEEK_END:
		seek_type = SFS_SEEK_END;
		break;
	default:
		seek_type = MFILE_ERR_BAD_FSNAME;
		break;
	}
	return seek_type;
}

MINT McfFile_GetMaxPath(MVOID)
{
	return SFS_MAX_PATH;
}

MINT McfFile_GetAttrDir(MVOID)
{
	return SFS_ATTR_DIR;
}
MMIFMM_FUNC_TYPE_E McfFile_GetFuncType(MVOID)
{
#ifdef MMI_PDA_SUPPORT
	return FUNC_FIND_FILE_AND_ALLFOLER;
#else
	return FUNC_FIND_FILE_AND_FOLDER;
#endif
}

MMIFMM_OPENWIN_RET_E McfFile_GetOpenWinRet(MVOID)
{
	return FMM_OPENWIN_SUCCESS;
}

MINT McfFile_SetFileAttrEx(const MWCHAR* pcwszFileName, MBYTE nAttr)
{
	MINT ret = 0;
	MUSHORT attr = MFILE_ATTR_NONE;
	MUINT len = McfStdC_wcslen(pcwszFileName);
	MWCHAR FileName[MAX_FILEPATH_LEN] = {0};

	if (MNULL == pcwszFileName)
	{
		return MFILE_ERR_BAD_FSNAME;
	}

  	McfStdC_wcscpy(FileName,pcwszFileName);
	
	if('\\' == FileName[len-1])
	{
		FileName[len-1] = 0;
	}
	
#ifdef SPREAD_PLAT_SC6600L_0937
	/* 这里暂时要注销掉,因为这个版本的SFS.lib没有包含这个接口，所以编译的时候无法link   0937 的平台要删除*/
#else///SPREAD_PLAT_SC6600L_1025
	
	if(nAttr & MFILE_ATTR_READ_ONLY)
	{
		attr = attr | SFS_ATTR_RO;
	}
	if(nAttr & MFILE_ATTR_HIDDEN)
	{
		attr = attr | SFS_ATTR_HIDDEN;
	}
	if(nAttr & MFILE_ATTR_ARCHIVE)
	{
		attr = attr | SFS_ATTR_ARCH;
	}
	ret = SFS_SetAttr(FileName, attr);
#endif
	
	return McfFile_return_error_code(ret);
}

MFd McfFile_OpenEx(const MWCHAR* pcwszFileName, MUINT nOpenFlag)
{
	SFS_HANDLE fh = 0;
	MUINT iFlag = 0;
	if(pcwszFileName == MNULL)
	{
		MCFAPI_FILE_TRACE("[MCARE] McfFile_OpenEx pcwszFileName == MNULL");
		return MFILE_ERR_BAD_FSNAME;
	}
	iFlag=0;
	
	switch (nOpenFlag)
	{
	case MFILE_OPEN_RO:
		iFlag |= SFS_MODE_OPEN_EXISTING | SFS_MODE_READ;
		break;
	case MFILE_OPEN_RW:
		iFlag |= SFS_MODE_OPEN_EXISTING | SFS_MODE_WRITE | SFS_MODE_READ;
		break;
	case MFILE_OPEN_CW:
		iFlag |= SFS_MODE_CREATE_ALWAYS | SFS_MODE_WRITE | SFS_MODE_READ;
		break;
	default:
		break;
	}
	
	fh = SFS_CreateFile(pcwszFileName, iFlag, MNULL, MNULL);
	MCFAPI_FILE_TRACE("[MCARE] McfFile_OpenEx end fh = %d, iFlag = %d",fh, iFlag);
	if(fh != SFS_INVALID_HANDLE)
	{
		//return fh;
		return McfFile_InsertHandle(fh);
	}
	else
	{
		return MFILE_FAIL;
	}
}

MBOOL McfIF_IsUsePublicDrv(MVOID)
{
	return s_IsUsePublicDrv;
}

MVOID McfIF_SetUsePublicDrv(MBOOL is_udisk)
{
	s_IsUsePublicDrv = is_udisk;
}

MUINT McfIF_GetFsSizeReq(MVOID)
{
	if ((MBOOL)McfIF_IsUsePublicDrv())
	{
		return 2*1024*1024;
	}
	else
	{
		return MCR_FS_REQ;
	}
}

MBOOL McfIF_IsStorageReady(MVOID)
{//此处初始化MCare的运行盘
	if (McfFile_CheckDrvReady(McfCmn_GetCardDrv()))
	{
		McfIF_SetUsePublicDrv(MFALSE);
		return MTRUE;
	}
	else if(McfFile_CheckDrvReady(McfCmn_GetPublicDrv()))
	{
		McfIF_SetUsePublicDrv(MTRUE);
		return MTRUE;
	}
	else
	{
		return MFALSE;
	}
}


static MWCHAR g_Root_Drv[4] = {0};
//----------------------------------------------------------------------------
//FUNCTION: McfIF_GetRootDrv
//Descrption:获得Mcare运行的根目录
//Param:
//  IN: MVOID
//  OUT:MWCHAR*
//Return:Mcare运行的根目录like L"E:" or L"D:"
//Note:
//----------------------------------------------------------------------------
MWCHAR* McfIF_GetRootDrv(MVOID)
{
    g_Root_Drv[0]= McfFile_GetStorageDrv();
    g_Root_Drv[1]=':';
    g_Root_Drv[2]='\0';
	return g_Root_Drv;
}

//=======================McfFs.c End================================

//=======================McfGdi.c Begin===============================

IMG_RES_ERROR_E MAPI_IMG_RES_GetImageInfo(const IMG_RES_SRC_T* img_src_ptr, IMG_RES_INFO_T* img_info_ptr)
{
	return IMG_RES_GetImageInfo(img_src_ptr, img_info_ptr);
}

IMG_RES_ERROR_E MAPI_IMG_RES_Display_Img(const IMG_RES_SRC_T* img_src_ptr, MVOID* img_disp_ptr, IMG_RES_INFO_T* img_info_ptr)
{

#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
	return IMG_RES_Display_Img(img_src_ptr, (IMG_RES_DISP_T*)img_disp_ptr, img_info_ptr);
#else
	return IMG_RES_Display_Img(img_src_ptr, (IMG_RES_DST_T*)img_disp_ptr);
#endif
}

MBOOL MAPI_MMITHEME_StoreUpdateRect(GUI_LCD_DEV_INFO const* lcd_dev_ptr, GUI_RECT_T update_rect)
{
	return MMITHEME_StoreUpdateRect(lcd_dev_ptr, update_rect);
}

MVOID MAPI_LCD_DrawRect(GUI_LCD_DEV_INFO const* dev_info_ptr, GUI_RECT_T box, GUI_COLOR_T color)
{
	LCD_DrawRect(dev_info_ptr, box, color);
}

MVOID MAPI_LCD_FillRect(GUI_LCD_DEV_INFO const* dev_info_ptr, GUI_RECT_T box, GUI_COLOR_T color)
{
	LCD_FillRect(dev_info_ptr, box, color);
}
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
MBOOL MAPI_GUILCD_GetInfo(LCD_ID_E lcd_id, LCD_INFO_T* lcd_info_ptr)
{
	return GUILCD_GetInfo(lcd_id, lcd_info_ptr);
}
#endif
MUINT MAPI_GUI_GetStringWidth(GUI_FONT_T font_type, const MWCHAR* str_ptr, MUINT str_len)
{
	return GUI_GetStringWidth(font_type, str_ptr, str_len);
}

MBOOL MAPI_GUILCD_InvalidateRect(LCD_ID_E lcd_id, GUI_RECT_T rect,  MUINT update_block_set)
{
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
	return GUILCD_InvalidateLCDRect(McfCmn_GetLCdID(), rect.left, rect.top, rect.right, rect.bottom, update_block_set);
#else
	return GUILCD_InvalidateRect(lcd_id, rect, update_block_set);
#endif	
}

#if (defined SPREAD_PLAT_SC6800H)|| (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
MBOOL MAPI_GUILCD_GetLogicWidthHeight(LCD_ID_E lcd_id, MUSHORT* logic_width_ptr, MUSHORT* logic_height_ptr)
{
	return GUILCD_GetLogicWidthHeight(lcd_id, logic_width_ptr, logic_height_ptr);
}
#endif

GUI_COLOR_T* MAPI_GUIBLOCK_GetBuf(GUI_LCD_DEV_INFO* lcd_dev_ptr)
{
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
	return ((GUI_COLOR_T*)MMIAPICOM_GetMainLcdBufferPtr());
#else
	return GUIBLOCK_GetBuf(lcd_dev_ptr);
#endif
}

MBOOL McfIF_Gdi_bNeedDecode(MINT img_type)
{
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
	return MTRUE;
#else
	if(MGDI_IMG_PNG == img_type)
	{
		return MTRUE;
	}
	else if(MGDI_IMG_GIF == img_type)
	{
#ifdef SPREAD_PLAT_SC6600L_0937
		return MTRUE;
#else//SPREAD_PLAT_SC6600L_1025
		return MFALSE;
#endif
	}
	else if(MGDI_IMG_BMP == img_type)
	{
		return MTRUE;
	}
	else
	{
		return MFALSE;
	}
#endif//#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)
}

GUI_LCD_DEV_INFO McfCmn_GetDevInfo(MVOID)
{
#ifdef MCARE_LAYER_SUPPORT
	return McfCmn_GetMcareLayerInfo();
#else
	GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	return dev_info;
#endif
}

IMG_RES_ERROR_E McfCmn_GetResError(MUINT type)
{
	IMG_RES_ERROR_E res_result = IMG_RES_BAD_FILE_DATA;
	switch(type)
	{
	case 0:
		res_result = IMG_RES_BAD_FILE_DATA;
		break;
	case 1:
		res_result = IMG_RES_SUCCESS;
		break;
	default:
		res_result = IMG_RES_BAD_FILE_DATA;
		break;
	}
	return res_result;
}

GUI_FONT_T McfCmn_GetFontId(MINT fontSize)
{
	GUI_FONT_T font_id = MMI_DEFAULT_NORMAL_FONT;
	switch(fontSize)
	{
	case MGDI_FONTSIZE_SMALL:
		font_id = MMI_DEFAULT_SMALL_FONT;
		break;
	case MGDI_FONTSIZE_MEDIUM:
		font_id = MMI_DEFAULT_NORMAL_FONT;
		break;
	case MGDI_FONTSIZE_LARGE:
		font_id = MMI_DEFAULT_BIG_FONT;
		break;
	default:
		break;
	}
	return font_id;
}

MINT McfIF_GetCharWidth(GUI_FONT_T font)
{
	MINT Width=0;
	//Width=GUI_GetFontWidth(font, 0x4e00);
//取得字符宽度，为了适配斗地主修改为取得“8”的宽度
	Width=GUI_GetFontWidth(font, 0x0038);	
	return Width;
}

MINT McfIF_GetCharHeight(GUI_FONT_T font)
{
	MINT Height=0;
	Height = GUI_GetFontHeight(font, 0x4e00);
	return Height;
}

GUI_ALIGN_E McfCmn_GetAligenType(MINT aligen_enum)
{
	GUI_ALIGN_E aligen_type = ALIGN_LVMIDDLE;
	return aligen_type;
}

#if (defined SPREAD_PLAT_SC6800H) ||(defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
MINT McfIF_GetScrWidth(MVOID)
{
	MINT lcd_width=0;
	MINT lcd_height=0;
	LCD_ANGLE_E  rotate_angle = LCD_ANGLE_0;
	LCD_INFO_T lcd_info;
	GUI_LCD_DEV_INFO dev = McfCmn_GetDevInfo();    
	GUILCD_GetInfo(dev.lcd_id, &lcd_info);    
	
	rotate_angle=GUILCD_GetRotateAngle(dev.lcd_id);
	///MMK_GetWinAngle
	switch (rotate_angle)
	{
	case LCD_ANGLE_90:
	case LCD_ANGLE_270:
		lcd_width = lcd_info.lcd_height;
		lcd_height = lcd_info.lcd_width;
		break;
	default:
		lcd_width = lcd_info.lcd_width;
		lcd_height = lcd_info.lcd_height;        
		break;
	}
	//MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"[MCFSCREEN]zhukey McfIF_GetScrWidth lcd_width = %d", lcd_width);
	return lcd_width;
}

MINT McfIF_GetScrHeight(MVOID)
{
	MINT lcd_width = 0;
	MINT lcd_height = 0;
	LCD_ANGLE_E  rotate_angle = LCD_ANGLE_0;
	LCD_INFO_T lcd_info = {0};
	GUI_LCD_DEV_INFO dev = McfCmn_GetDevInfo();
	GUILCD_GetInfo(dev.lcd_id, &lcd_info);
	
	rotate_angle=GUILCD_GetRotateAngle(dev.lcd_id);
	switch (rotate_angle)
	{
	case LCD_ANGLE_90:
	case LCD_ANGLE_270:
		lcd_width = lcd_info.lcd_height;
		lcd_height = lcd_info.lcd_width;
		break;
	default:
		lcd_width = lcd_info.lcd_width;
		lcd_height = lcd_info.lcd_height;        
		break;
	}
	//MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"[MCFSCREEN]zhukey McfIF_GetScrHeight lcd_height = %d", lcd_height);
	return lcd_height;
}

#else
MINT McfIF_GetScrWidth(MVOID)
{
	MINT width = 0;
	MINT height = 0;
	GUI_GetLcdSize(McfCmn_GetLCdID(), &width, &height);
	
	return (MINT)width;
}

MINT McfIF_GetScrHeight(MVOID)
{
	MINT scr_width = 0;
	MINT scr_height = 0;
	GUI_GetLcdSize(McfCmn_GetLCdID(), &scr_width, &scr_height);
	
	return (MINT)scr_height;
}
#endif

MUINT McfCmn_GetLCdID(MVOID)
{
	return GUI_MAIN_LCD_ID;
}

MUINT McfCmn_GetBlockID(MVOID)
{
#ifdef MCARE_LAYER_SUPPORT
	return McfCmn_GetMcareLayerBlockID();
#else
	return GUI_BLOCK_MAIN;
#endif
}


#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)

MINT __Gdi_DrawImageBuffer(MINT x, MINT y, const MBYTE* pImgData, MUINT nDataSize)
{
#ifdef MCARE_DECODE_GIF_ALPHA_SUPPORT
#define PIXEL_DEPTH 2
#else
#define PIXEL_DEPTH 2
#endif
	GUI_LCD_DEV_INFO dev_info = McfCmn_GetDevInfo();
	GUI_RECT_T box = {0};
	GUIIMG_BITMAP_T bitmap = {0};
	GUI_POINT_T startPos = {0};
	GUIIMG_SRC_T src_ptr = {0};
	GUIIMG_DISPLAY_T display_ptr = {0};
	GUIIMG_DEC_OUT_T output_ptr = {0};
	MUINT img_w = 0;
	MUINT img_h = 0;
	MINT result = MGDI_ERROR;
#ifdef MCARE_DECODE_GIF_ALPHA_SUPPORT
	MUSHORT* data_buf = MNULL;
#else
	MBYTE* data_buf = MNULL;
#endif
	MImageInfo MImgInfo = {0};
	GUI_RECT_T CurClip = {0};
	GUI_RECT_T *pCurClip = &CurClip;
	
	result = McfGdi_GetImageInfo(pImgData, nDataSize, &MImgInfo);
	
	img_w = MImgInfo.m_nWidth;
	img_h = MImgInfo.m_nHeight;
	
	//pCurClip = McfGDI_GetCurClip();
	McfGDI_GetCurClip(&pCurClip->top, &pCurClip->left, &pCurClip->bottom, &pCurClip->right);
	
	if(x > pCurClip->right || y > pCurClip->bottom || (x + (MINT)img_w) < pCurClip->left || (y + (MINT)img_h) < pCurClip->top)
	{
		return MGDI_PARAM_ERR;
	}
	
#ifdef MCARE_DECODE_GIF_ALPHA_SUPPORT
	data_buf = (MUSHORT*)MAPI_SCI_ALLOCA_ByZero(img_w * img_h * PIXEL_DEPTH);
#else
	data_buf = (MBYTE*)MAPI_SCI_ALLOCA_ByZero(img_w * img_h * PIXEL_DEPTH);
#endif
	
	if(MNULL == data_buf)
	{
		return MGDI_ERROR;
	}
	McfStdC_memset(data_buf, 0, img_w*img_h*PIXEL_DEPTH);
	
	output_ptr.actual_width = 0;
	
	box.left = x;
	box.top = y;
	box.right = x + (MINT)img_w;
	box.bottom = y + (MINT)img_h;
	if(y < pCurClip->top)
	{
		box.top = pCurClip->top;
	}
	if(y+(MINT)img_h > pCurClip->bottom)
	{
		box.bottom = pCurClip->bottom;
	}
	if(x < pCurClip->left)
	{
		box.left = pCurClip->left;
	}
	if(x+(MINT)img_w > pCurClip->right)
	{
		box.right = pCurClip->right;
	}
	startPos.x = x;
	startPos.y = y;
	
	src_ptr.is_file = MFALSE;
	src_ptr.full_path_ptr = MNULL;
	src_ptr.src_buf_ptr = pImgData;
	src_ptr.src_data_size = nDataSize;
	src_ptr.alpha_buf_ptr = MNULL;
	src_ptr.alpha_buf_size = 0;
	
#ifdef MCARE_DECODE_GIF_ALPHA_SUPPORT
	display_ptr.is_exist_bg = MTRUE;
#endif	
	display_ptr.crop_rect_ptr = MNULL;
	display_ptr.dest_width = MImgInfo.m_nWidth;
	display_ptr.dest_height = MImgInfo.m_nHeight;
	
	output_ptr.is_handle_alpha = MFALSE;
#ifdef MCARE_DECODE_GIF_ALPHA_SUPPORT
	output_ptr.actual_width = MImgInfo.m_nWidth;
	output_ptr.actual_height = MImgInfo.m_nHeight;
	{
		MUSHORT* temp_buf = data_buf;
		MUSHORT* lcd_buf = GUIBLOCK_GetBuf(&dev_info);
		MINT i = 0;
		MINT j = 0;
		
		lcd_buf += (box.top*McfGdi_GetScrWidth() + box.left);
		temp_buf += ((box.top - y)*MImgInfo.m_nWidth + (box.left - x));
		
		for(i = 0; i < (box.bottom - box.top); i++)
		{
			for(j = 0; j < (box.right - box.left); j++)
			{
				memcpy(temp_buf, lcd_buf, sizeof(MUSHORT));
				lcd_buf++;
				temp_buf++;
			}
			lcd_buf += (McfGdi_GetScrWidth() - (box.right - box.left));
			temp_buf += (MImgInfo.m_nWidth - (box.right - box.left));
		}
	}
#else
	output_ptr.actual_width = 0;
	output_ptr.actual_height = 0;
#endif
	output_ptr.decode_data_ptr = (MBYTE*)data_buf;
	output_ptr.decode_data_size = img_w * img_h * PIXEL_DEPTH;
	
	if(GUIIMG_Decode(&src_ptr,&display_ptr,&output_ptr))
	{
#if (defined SPREAD_PLAT_SC6530)
		bitmap.img_width = output_ptr.actual_width;
		bitmap.img_height = output_ptr.actual_height;
#else
		bitmap.img_width = img_w;
		bitmap.img_height = img_h;
#endif
		bitmap.bit_ptr = data_buf;
		
		GUIIMG_DisplayBmp(MFALSE, &box, &startPos, &bitmap, &dev_info);
	}
	else
	{
	}
	MAPI_SCI_FREE(data_buf);
	data_buf = MNULL;
#undef PIXEL_DEPTH
	
	return MGDI_SUCCESS;
}
MINT McfIF_Gdi_DrawText(MINT x, MINT y, const MWCHAR* pcwszStr)
{
	GUI_LCD_DEV_INFO dev = McfCmn_GetDevInfo();//{GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T text_rect={0};
	MMI_STRING_T str = {0};
	GUISTR_STYLE_T str_style = {0};
	GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN;// | GUISTR_STATE_SINGLE_LINE;
	GUI_RECT_T CurClip = {0};
	GUI_RECT_T *pCurClip = &CurClip;
	
	if(MNULL == pcwszStr)
	{
		return MGDI_PARAM_ERR;
	}
	
	if(!McfIF_IsLayerActive())
	{
		return MGDI_ERROR;
	}
	
	text_rect.left = x;
#ifdef SPREAD_PLAT_SC8800G
	text_rect.top = y + 2;
#else
	text_rect.top = y;
#endif
	text_rect.right = text_rect.left + McfGdi_GetTextWidth(pcwszStr);
	text_rect.bottom = text_rect.top + McfGdi_GetCharHeight();
	text_rect.right = (text_rect.right < (McfGdi_GetScrWidth() - 1))?text_rect.right: (McfGdi_GetScrWidth() - 1);
	str.wstr_ptr=pcwszStr;
	str.wstr_len=MMIAPICOM_Wstrlen(pcwszStr);
	
	if(0 == McfGDI_GetGlobalFont())
	{
		McfGDI_SetGlobalFont(McfCmn_GetFontId(MGDI_FONTSIZE_SMALL));
	}
	
	str_style.font = McfGDI_GetGlobalFont();
	str_style.font_color = McfGDI_GetGlobalColor();
	str_style.line_space = 0;
	str_style.align = McfCmn_GetAligenType(0);//ALIGN_LVMIDDLE
	
	//CurClip = McfGDI_GetCurClip();
	McfGDI_GetCurClip(&pCurClip->top, &pCurClip->left, &pCurClip->bottom, &pCurClip->right);
	
	GUISTR_DrawTextToLCDInRect(&dev, &text_rect, pCurClip, &str, &str_style, str_state, GUISTR_TEXT_DIR_AUTO);
	return MGDI_SUCCESS;
}

MINT McfIF_Gdi_DrawTextN(MINT x, MINT y, const MWCHAR* pcwszStr, MINT n)
{
	GUI_LCD_DEV_INFO dev = McfCmn_GetDevInfo();//{GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	//MINT len=0;
	MINT text_height=0;
	//MINT i=0;
	MWCHAR * dis_str = MNULL;	
	GUI_RECT_T text_rect={0};
	MMI_STRING_T    str = {0};
	GUISTR_STYLE_T        str_style = {0};
	GUISTR_STATE_T        str_state = GUISTR_STATE_ALIGN;// | GUISTR_STATE_SINGLE_LINE;// | GUISTR_STATE_WORDBREAK ;

	GUI_RECT_T CurClip = {0};
	GUI_RECT_T *pCurClip = &CurClip;
	
	if(MNULL == pcwszStr)
	{
		return MGDI_PARAM_ERR;
	}
	
	if(!McfIF_IsLayerActive())
	{
		return MGDI_ERROR;
	}
	
	text_rect.left = x;
#ifdef SPREAD_PLAT_SC8800G
	text_rect.top = y + 2;
#else
	text_rect.top = y;
#endif
	text_rect.right = text_rect.left + McfGdi_GetTextWidth(pcwszStr);
	text_rect.bottom= text_rect.top+text_height+McfGdi_GetCharHeight();
	text_rect.right = (text_rect.right <  (McfGdi_GetScrWidth() - 1))?text_rect.right: (McfGdi_GetScrWidth() - 1);
	
	str.wstr_ptr=pcwszStr;
	str.wstr_len=MMIAPICOM_Wstrlen(pcwszStr);
	if(n > str.wstr_len)
	{
		n = str.wstr_len;
	}
	if(0 == McfGDI_GetGlobalFont())
	{
		McfGDI_SetGlobalFont(McfCmn_GetFontId(MGDI_FONTSIZE_SMALL));
	}
	
	dis_str = MAPI_SCI_ALLOCA_ByZero((n+1)*sizeof(MWCHAR));
	if(MNULL == dis_str)
	{
		return MGDI_ERROR;
	}
	McfStdC_memset(dis_str,0,(n+1)*sizeof(MWCHAR));
	McfStdC_wcsncpy(dis_str, pcwszStr, n);
	str.wstr_ptr = dis_str;
	#ifdef SPREAD_PLAT_SC6530
	//修改长度为n
	str.wstr_len = n;
	#endif
	
	str_style.font = McfGDI_GetGlobalFont();
	str_style.font_color = McfGDI_GetGlobalColor();
	str_style.line_space = 0;
	str_style.align = McfCmn_GetAligenType(0);//ALIGN_LVMIDDLE
	
	//pCurClip = McfGDI_GetCurClip();
	McfGDI_GetCurClip(&pCurClip->top, &pCurClip->left, &pCurClip->bottom, &pCurClip->right);
	
	GUISTR_DrawTextToLCDInRect(&dev, &text_rect, pCurClip, &str, &str_style, str_state, GUISTR_TEXT_DIR_LTR); 
	MAPI_SCI_FREE(dis_str);
	dis_str = MNULL;
	return MGDI_SUCCESS;
}
#else
MINT __Gdi_DrawImageBuffer(MINT x, MINT y, const MBYTE* pImgData, MUINT nDataSize)
{
#define PIXEL_DEPTH 2
	GUI_LCD_DEV_INFO dev_info = McfCmn_GetDevInfo();//{GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T box = {0};
	MImageInfo MImgInfo = {0};
	MBYTE* data_buf = MNULL;
	MBYTE* data_buf_temp = MNULL;
	GUIREF_IMG_DATA_T img_data = {0};
	MUINT img_w = 0;
	MUINT img_h = 0;
	MINT type = 0;
	MINT result = MGDI_ERROR;
	MBOOL bDecResult = MFALSE;
	MUSHORT total_frame_ptr = 1;
	T_GUI_Bitmap bitmap = {0};
	GUI_RECT_T CurClip = {0};
	
	McfGDI_GetCurClip(&CurClip.top, &CurClip.left, &CurClip.bottom, &CurClip.right);
	result = McfGdi_GetImageInfo(pImgData, nDataSize, &MImgInfo);	
	if(result != MGDI_SUCCESS)
	{	
		MCFAPI_GDI_PRINT(ALOG_INFO,MLOG_SDKGDI, __MCR_FUNCTION__"******GUIREF_AllocMemory fail******");
		return MGDI_ERROR;
	}	
	img_w = MImgInfo.m_nWidth;
	img_h = MImgInfo.m_nHeight;
	type = MImgInfo.m_imageType;	
	if(x > CurClip.right || y > CurClip.bottom || (x + img_w) < CurClip.left || (y + img_h) < CurClip.top)
	{
		return MGDI_PARAM_ERR;
	}	
	
	data_buf = MAPI_SCI_ALLOCA_ByZero(img_w * img_h * PIXEL_DEPTH);
	if(MNULL == data_buf)
	{
		return MGDI_ERROR;
	}
	McfStdC_memset(data_buf,0,img_w*img_h*PIXEL_DEPTH);
	
	McfStdC_memset((void*)&img_data,0,sizeof(img_data));
	img_data.src_buf_ptr = pImgData,
	img_data.src_data_size = nDataSize;		
	if(!GUIREF_AllocMemory(MFALSE,MTRUE,MTRUE,MTRUE))
	{
		MCFAPI_GDI_PRINT(ALOG_INFO,MLOG_SDKGDI, __MCR_FUNCTION__"******GUIREF_AllocMemory fail******");
		MAPI_SCI_FREE(data_buf);
		data_buf == MNULL;
		return MGDI_ERROR;
	}	
	if(MGDI_IMG_GIF == type)
	{
		McfGdi_SetIsDecodeGif(MTRUE);
		bDecResult = GUIREF_DecodeGif(data_buf,MNULL,&total_frame_ptr,img_w * img_h * PIXEL_DEPTH,img_w,img_h,&img_data);
		McfGdi_SetIsDecodeGif(MFALSE);
	}
	else if(MGDI_IMG_PNG == type)
	{
		bDecResult = GUIREF_DecodePng(data_buf,MNULL,img_w * img_h * PIXEL_DEPTH,img_w,img_h,&img_w,&img_h,MNULL,&img_data);
	}
	else if(MGDI_IMG_JPG == type)
	{
		bDecResult = GUIREF_DecodeJpg(data_buf,MNULL,img_w * img_h * PIXEL_DEPTH,img_w,img_h,&img_w,&img_h,MNULL,&img_data);
	}
	else if(MGDI_IMG_BMP == type)
	{
		bDecResult=GUIREF_DecodeBmpWbmp(MTRUE,data_buf,MNULL,img_w * img_h * PIXEL_DEPTH,img_w,img_h,&img_w,&img_h,MNULL,&img_data);
	}
	if(bDecResult)
	{
		box.left = x;
		box.top = y;
		box.right = x + img_w;
		box.bottom = y + img_h;
		data_buf_temp = data_buf;
		if(y < CurClip.top)
		{
			box.top = CurClip.top;
			data_buf_temp += (img_w * (CurClip.top - y) * 2);
			img_h -= (CurClip.top - y);
		}
		if(y+img_h > CurClip.bottom)
		{
			box.bottom = CurClip.bottom;
		}
		if(x < CurClip.left)
		{
			box.left = CurClip.left;
		}
		if(x+img_w > CurClip.right)
		{
			box.right = CurClip.right;
		}
		//PMI_DisplayBmpByBuf(&dev_info,x,y,img_w,img_h,data_buf,&box);/*这个接口无法显示位深度为8的PNG图片*/
#ifdef MCARE_L_PLAT_DECODE_GIF
		{
			MUSHORT* temp_buf = data_buf_temp;
			MUSHORT* lcd_buf = MMIAPICOM_GetMainLcdBufferPtr();
			MINT i = 0;
			MINT j = 0;

			lcd_buf += (box.top*McfGdi_GetScrWidth() + box.left);
			temp_buf += ((box.top - y)*MImgInfo.m_nWidth + (box.left - x));

			for(i = 0; i < (box.bottom - box.top); i++)
			{
				for(j = 0; j < (box.right - box.left); j++)
				{
					if(0xFFFF == (MUSHORT)(*temp_buf))
					{
						memcpy(temp_buf, lcd_buf, sizeof(MUSHORT));
					}
					lcd_buf++;
					temp_buf++;
				}
				lcd_buf += (McfGdi_GetScrWidth() - (box.right - box.left));
				temp_buf += (MImgInfo.m_nWidth - (box.right - box.left));
			}
		}
#endif//#ifdef MCARE_L_PLAT_DECODE_GIF
		bitmap.wType = 0;
		bitmap.wWidth = img_w;
		bitmap.wHeight = img_h;
		bitmap.wOriginHeight = img_w;
		bitmap.wOriginWidth = img_h;
		bitmap.ubPlanes = 0;
		bitmap.ubBitsPixel = (PIXEL_DEPTH * 8);
		bitmap.vpBits = data_buf_temp;
		bitmap.uwTransparent = 0;
		bitmap.IsProgressive = 0;
		PMI_DisplayBMPImageExt(MFALSE, &dev_info, box.left, box.top, &box, &bitmap);
	}
	else
	{
		MCFAPI_GDI_PRINT(ALOG_INFO,MLOG_SDKGDI, __MCR_FUNCTION__"******QImage decodepng fail******");
	}		
	GUIREF_FreeEncOrDecBuf();
	MAPI_SCI_FREE(data_buf);
	data_buf = MNULL;
#undef PIXEL_DEPTH
	return MGDI_SUCCESS;

}

#endif//#if (defined(SPREAD_PLAT_SC6800H)||defined(SPREAD_PLAT_SC8800G)) || (defined SPREAD_PLAT_SC6530)

MBOOL McfCmn_IsSupportGradRect(MVOID)
{
#ifdef MCARE_NOT_SUPPORT_GRAD_RECT
	return MFALSE;
#else
	return MTRUE;
#endif
}

MVOID McfIF_Gdi_DrawLine(GUI_LCD_DEV_INFO const *dev_info_ptr, MINT x1, MINT y1, MINT x2, MINT y2, MColor line_color)
{
	LCD_DrawLine(dev_info_ptr, x1, y1, x2, y2, line_color);
}

MUINT McfIF_GetImgResBMP(MVOID)
{
	return IMG_RES_BMP;
}
//=======================McfGdi.c Begin=============================

//=======================McfIdle.c Begin=============================

MBOOL MAPI_MMIDEFAULT_GetBackLightState(MVOID)
{
	return MMIDEFAULT_GetBackLightState();
}

MVOID MAPI_MMIDEFAULT_TurnOffBackLight(MVOID)
{
	MMIDEFAULT_TurnOffBackLight();
}

MVOID MAPI_MMIDEFAULT_TurnOnBackLight(MVOID)
{
	MMIDEFAULT_TurnOnBackLight();
}

MBOOL MAPI_GUIWIN_SetStbItemIconId(MUINT item_index, MMI_IMAGE_ID_T icon_id)
{
	return GUIWIN_SetStbItemIconId(item_index, icon_id);
}

MBOOL MAPI_GUIWIN_SetStbItemIconCurFrame(MUINT item_index, MUINT cur_frame)
{
	return GUIWIN_SetStbItemIconCurFrame(item_index, cur_frame);
}

MBOOL MAPI_GUIWIN_GetStbItemIconStyle(MUINT item_index, GUIWIN_STBDATA_ICON_STYLE_E* icon_style_ptr)
{
	return GUIWIN_GetStbItemIconStyle(item_index, icon_style_ptr);
}

MBOOL MAPI_GUIWIN_SetStbItemVisible(MUINT item_index, MBOOL is_visible)
{
	return GUIWIN_SetStbItemVisible(item_index, is_visible);
}

MBOOL MAPI_GUIWIN_AppendStbItemData(GUIWIN_STBDATA_ITEM_DATA_T *item_data_ptr)
{
	if(MNULL == item_data_ptr)
	{
		return MFALSE;
	}
	return GUIWIN_AppendStbItemData(item_data_ptr);
	
}
MINT McfIdle_Get_GUIWIN_STBDATA_ICON_NORMAL(MVOID)
{
	return (MINT)GUIWIN_STBDATA_ICON_NORMAL;
}

GUIWIN_STBDATA_ICON_STYLE_E McfIdle_GetIconStyleNormal(MVOID)
{
	return GUIWIN_STBDATA_ICON_NORMAL;
}

MUINT McfIdle_GetIdleIconID(MVOID)
{
	return Mcare_idle_iconid();
}

MBOOL McfIdle_IsPhoneLocked(MVOID)
{
#ifdef SPREAD_PLAT_SC6530
	return MMIAPIKL_IsPhoneLocked();
#else
	return (MBOOL)MMIKL_IsPhoneLocked();
#endif
}

MBOOL McfIdle_IsIdleQQMsgBlinkLCD(MVOID)
{
#ifdef MCAREV30_IDLE_QQMSG_BLINK_LCD
	return MTRUE;
#else
	return MFALSE;
#endif
}

MBOOL McfIF_IsIdleQQMsgLightLCD(MVOID)
{
#ifdef MCR_BLACKLIGHT_TURNON
	return MTRUE;
#else
	return MFALSE;
#endif
}

MBOOL McfIF_IsOnlyStaterBarIconMsg(MVOID)
{
	return MFALSE;
	//return MTRUE;
}
	
MUINT McfIdle_GetMocorImageIDNull(MVOID)
{
	return IMAGE_NULL;
}

MUINT McfIdle_GetIdleIconImageIDNull(MVOID)
{
	return IMAGE_MCARE_MINI_QQ_NULL;
}
MUINT McfIdle_GetIdleIconImageIDMcare(MVOID)
{
	return IMAGE_MCARE_MINI_MCARE;
}

MUINT McfIdle_GetIdleIconImageIDQqOnline(MVOID)
{
	return IMAGE_MCARE_MINI_QQ_ONLINE;
}

MUINT McfIdle_GetIdleIconImageIDQqOffline(MVOID)
{
	return IMAGE_MCARE_MINI_QQ_OFFLINE;
}

MUINT McfIdle_GetIdleIconImageIDQqAway(MVOID)
{
	return IMAGE_MCARE_MINI_QQ_AWAY;
}

MUINT McfIdle_GetIdleIconImageIDQqInvisible(MVOID)
{
	return IMAGE_MCARE_MINI_QQ_INVISIBLE;
}

MUINT McfIdle_GetIdleIconImageIDQqIncomeMsg(MVOID)
{
	return IMAGE_MCARE_MINI_QQ_INCOME_MSG;
}


MUINT McfIdle_GetIdleIconImageIDContact(MVOID)
{
    return IMAGE_MCARE_MINI_CONTACT;
}
MVOID McfIdle_UpdateStaterBarIcon(MVOID)
{
	GUIWIN_UpdateStb();
	//GUIWIN_UpdateStbIcon();///此处 要求QQ消息闪烁的图标和QQ的状态图标一致才能解决不描画背景的花屏问题
}

MVOID McfIdle_DropDownNotifyRecord(MBOOL is_add, MINT icon_id)
{
#ifdef PDA_UI_DROPDOWN_WIN
	DROPDOWN_NOTIFY_E noty_type = DROPDOWN_NOTIFY_MAX;
	MCFAPI_IDLE_PRINT(ALOG_INFO,MLOG_SDKIDLE, __MCR_FUNCTION__"McfIdle_DropDownNotifyRecord is_add= %d, icon_id = %d", is_add, icon_id);
	if(MIDLE_ICON_CONTACT == icon_id)
	{
		noty_type = DROPDOWN_NOTIFY_MCARE_QC;
	}
	else if(MIDLE_ICON_QQ_ONLINE <= icon_id && MIDLE_ICON_QQ_HAVEMSG >= icon_id)
	{
		noty_type = DROPDOWN_NOTIFY_MCARE_QQ;
	}
	else
	{
	}
	MCFAPI_IDLE_PRINT(ALOG_INFO,MLOG_SDKIDLE, __MCR_FUNCTION__"McfIdle_DropDownNotifyRecord noty_type= %d", noty_type);
	
	if(is_add)
	{
		MMIDROPDOWNWIN_AddNotifyRecord(noty_type);
	}
	else
	{
		MMIDROPDOWNWIN_DelNotifyRecord(noty_type);
	}
#endif
}

#ifdef PDA_UI_DROPDOWN_WIN
MUINT McfIF_GetIdleIconImageID(MINT idle_icon)
{
	if(DROPDOWN_NOTIFY_MCARE_QQ == idle_icon)
	{
		if(McfIdle_GetIdleIconImageIDContact() == McfCmn_GetIdleIconImageID())
		{
			return McfIdle_GetIdleIconImageIDNull();
		}
		else
		{
			return McfCmn_GetIdleIconImageID();
		}
	}
	else if(DROPDOWN_NOTIFY_MCARE_QC == idle_icon)
	{
		if(McfIdle_GetIdleIconImageIDContact() != McfCmn_GetIdleIconImageID())
		{
			return McfIdle_GetIdleIconImageIDNull();
		}
		else
		{
			return McfIdle_GetIdleIconImageIDContact();
		}
	}
	else
	{
		return McfIdle_GetIdleIconImageIDMcare();
	}
}
#endif//#ifdef PDA_UI_DROPDOWN_WIN

MVOID McfIF_SetStatuBarInfo(MVOID)
{
	McfIdle_IdleSatuBarInit();
}

MBOOL McfIdle_IsShowContanct(MVOID)
{
#ifdef SPREAD_PLAT_SC6530
	return MTRUE;
#else
	return MFALSE;
#endif
}

//=======================McfIdle.c End==========================

//=======================McfKey.c Begin==========================
MVOID McfKey_GetkeyInfo(MUINT* keyCode, MUINT* keyType, MUINT msg_id)
{
	if((msg_id & 0xff00) == KEY_PRESSED)
	{
		*keyType = MEVTKEY_PRESS;
	}
	else if((msg_id & 0xff00) == KEY_RELEASED)
	{
		*keyType = MEVTKEY_RELEASE;
	}
	else if((msg_id & 0xff00) == KEY_LONG_PRESSED)
	{
		*keyType = MEVTKEY_LONGPRESS;
	}
	else if((msg_id & 0xff00) == KEY_LONG_RELEASED)
	{
		*keyType = MEVTKEY_RELEASE;
	}
	else if((msg_id & 0xff00) == KEY_REPEATED) 
	{
		*keyType = MEVTKEY_REPEAT;
	}
	else
	{
		*keyType = MEVTKEY_NONE;
	}
	
	switch(msg_id)
	{
	case MSG_KEYDOWN_UP:
	case MSG_KEYDOWN_UPSIDE:
	case MSG_KEYUP_UPSIDE:
	case MSG_KEYREPEAT_UP:
	case MSG_KEYUP_UP:
	case MSG_KEYPRESSUP_UP:
	case MSG_KEYLONG_UP:
	case MSG_KEYLONG_UPSIDE:
	case MSG_KEYREPEAT_UPSIDE:
	case MSG_KEYPRESSUP_UPSIDE:
		*keyCode = MKEY_UP;
		break;
	case MSG_KEYDOWN_DOWN:
	case MSG_KEYDOWN_DOWNSIDE:
	case MSG_KEYUP_DOWNSIDE:
	case MSG_KEYREPEAT_DOWN:
	case MSG_KEYUP_DOWN:
	case MSG_KEYPRESSUP_DOWN:
	case MSG_KEYLONG_DOWN:
	case MSG_KEYLONG_DOWNSIDE:
	case MSG_KEYREPEAT_DOWNSIDE:
	case MSG_KEYPRESSUP_DOWNSIDE:
		*keyCode = MKEY_DOWN;
		break;
	case MSG_KEYDOWN_LEFT:
	case MSG_KEYREPEAT_LEFT:
	case MSG_KEYUP_LEFT:
	case MSG_KEYPRESSUP_LEFT:
	case MSG_KEYLONG_LEFT:
		*keyCode = MKEY_LEFT;
		break;
	case MSG_KEYDOWN_RIGHT:
	case MSG_KEYREPEAT_RIGHT:
	case MSG_KEYUP_RIGHT:
	case MSG_KEYPRESSUP_RIGHT:
	case MSG_KEYLONG_RIGHT:
		*keyCode = MKEY_RIGHT;
		break;
	case MSG_KEYDOWN_0:
	case MSG_KEYUP_0:
	case MSG_KEYLONG_0:
	case MSG_KEYPRESSUP_0:
	case MSG_KEYREPEAT_0:
		*keyCode = MKEY_0;
		break;
	case MSG_KEYUP_1:
	case MSG_KEYDOWN_1:
	case MSG_KEYLONG_1:
	case MSG_KEYPRESSUP_1:
	case MSG_KEYREPEAT_1:
		*keyCode = MKEY_1;
		break;
	case MSG_KEYDOWN_2:
	case MSG_KEYUP_2:
	case MSG_KEYLONG_2:
	case MSG_KEYPRESSUP_2:
	case MSG_KEYREPEAT_2:
		*keyCode = MKEY_2;
		break;
	case MSG_KEYDOWN_3:
	case MSG_KEYUP_3:
	case MSG_KEYLONG_3:
	case MSG_KEYPRESSUP_3:
	case MSG_KEYREPEAT_3:
		*keyCode = MKEY_3;
		break;
	case MSG_KEYDOWN_4:
	case MSG_KEYUP_4:
	case MSG_KEYLONG_4:
	case MSG_KEYPRESSUP_4:
	case MSG_KEYREPEAT_4:
		*keyCode = MKEY_4;
		break;
	case MSG_KEYDOWN_5:
	case MSG_KEYUP_5:
	case MSG_KEYLONG_5:
	case MSG_KEYPRESSUP_5:
	case MSG_KEYREPEAT_5:
		*keyCode = MKEY_5;
		break;
	case MSG_KEYDOWN_6:
	case MSG_KEYUP_6:
	case MSG_KEYLONG_6:
	case MSG_KEYPRESSUP_6:
	case MSG_KEYREPEAT_6:
		*keyCode = MKEY_6;
		break;
	case MSG_KEYDOWN_7:
	case MSG_KEYUP_7:
	case MSG_KEYLONG_7:
	case MSG_KEYPRESSUP_7:
	case MSG_KEYREPEAT_7:
		*keyCode = MKEY_7;
		break;
	case MSG_KEYDOWN_8:
	case MSG_KEYUP_8:
	case MSG_KEYLONG_8:
	case MSG_KEYPRESSUP_8:
	case MSG_KEYREPEAT_8:
		*keyCode = MKEY_8;
		break;
	case MSG_KEYDOWN_9:
	case MSG_KEYUP_9:
	case MSG_KEYLONG_9:
	case MSG_KEYPRESSUP_9:
	case MSG_KEYREPEAT_9:
		*keyCode = MKEY_9;
		break;
	case MSG_KEYDOWN_OK:
	case MSG_KEYUP_OK:
	case MSG_CTL_OK:
	case MSG_KEYLONG_OK:
	case MSG_KEYPRESSUP_OK:
	case MSG_KEYREPEAT_OK:
#ifdef MMI_PDA_SUPPORT
	case MSG_KEYDOWN_MENU:
	case MSG_KEYUP_MENU:
	case MSG_KEYREPEAT_MENU:
	case MSG_KEYLONG_MENU:
#endif
		*keyCode = MKEY_SOFT1;
		break;
	case MSG_KEYDOWN_HASH:
	case MSG_KEYUP_HASH:
	case MSG_KEYLONG_HASH:
	case MSG_KEYPRESSUP_HASH:
	case MSG_KEYREPEAT_HASH:
		*keyCode = MKEY_POUND;
		break;
	case MSG_KEYDOWN_STAR:
	case MSG_KEYUP_STAR:
	case MSG_KEYLONG_STAR:
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
	case MSG_KEYPRESSUP_START:
	case MSG_KEYREPEAT_START:
#else
	case MSG_KEYPRESSUP_STAR:
	case MSG_KEYREPEAT_STAR:
#endif	
		*keyCode = MKEY_STAR;
		break;
	case MSG_KEYDOWN_CANCEL:
	case MSG_KEYUP_CANCEL:
	case MSG_CTL_CANCEL:
	case MSG_KEYLONG_CANCEL:
	case MSG_KEYPRESSUP_CANCEL:
	case MSG_KEYREPEAT_CANCEL:
		*keyCode = MKEY_SOFT2;
		break;
	case MSG_KEYDOWN_WEB:
	case MSG_KEYUP_WEB:
	case MSG_KEYLONG_WEB:
	case MSG_KEYPRESSUP_WEB:
	case MSG_KEYREPEAT_WEB:
		*keyCode = MKEY_SELECT;
		break;
	case MSG_KEYDOWN_RED:
	case MSG_KEYUP_RED:
	case MSG_KEYDOWN_FLIP:
	case MSG_KEYUP_FLIP:
		if(msg_id == MSG_KEYDOWN_FLIP)
		{
			McfIF_SetKeyDownSlide(MTRUE);
		}
		*keyCode = MKEY_END;
		break;
#ifndef SPREAD_PLAT_SC6600L_0937
	case MSG_KEYDOWN_SLIDE:
	case MSG_KEYUP_SLIDE:
		if(msg_id == MSG_KEYDOWN_SLIDE)
		{
			McfIF_SetKeyDownSlide(MTRUE);
		}
		*keyCode = MKEY_END;
		break;
#endif
	case MSG_KEYUP_GREEN:
	case MSG_KEYDOWN_GREEN:
		*keyCode = MKEY_SEND;
		break;
	default:
		*keyCode = MKEY_NONE;
		break;
}
}
//=======================McfKey.c End=============================


//=======================McfMemory.c Begin==========================
MVOID* MAPI_SCI_ALLOCA(MUINT size)
{
	MVOID* mem_tmp = MNULL; 
	if(0 == size)
	{
		return MNULL;
	}
	mem_tmp = (MVOID*)SCI_ALLOCA(size);
	if(MNULL != mem_tmp)
	{
		McfStdC_memset(mem_tmp, 0xcc, size);
	}
	return mem_tmp;
}

MVOID* MAPI_SCI_ALLOCA_ByZero(MUINT size)
{
	MVOID* mem_tmp = MNULL; 
	if(0 == size)
	{
		return MNULL;
	}
	mem_tmp = (MVOID*)SCI_ALLOCA(size);
	if(MNULL != mem_tmp)
	{
		McfStdC_memset(mem_tmp, 0, size);
	}
	return mem_tmp;
}
MVOID MAPI_SCI_FREE(MVOID* pMem)
{
	SCI_FREE(pMem);
}

//=======================McfMemory.c End==========================

//=======================McfMisc.c API Begin========================== 
MBOOL  MAPI_MNNV_GetIMEIEx(MBYTE simId,MN_IMEI_T imei)   // TRUE if read IMEI success
{
	MN_DUAL_SYS_E   dual_sys = McfSim_GetSimID((MINT)simId);
	if(dual_sys >= MN_DUAL_SYS_MAX)
	{
		return MFALSE;
	}
	return MNNV_GetIMEIEx(dual_sys,imei);              
}

MN_IMSI_T MAPI_MNSIM_GetImsiEx(MBYTE simId)
{
	MN_DUAL_SYS_E   dual_sys =McfSim_GetSimID((MINT)simId);
	MCF_SCI_ASSERT(dual_sys < MN_DUAL_SYS_MAX); 
	
	return MNSIM_GetImsiEx(dual_sys);
}

//------------------------------------------------------------------------------
//	FUNCTION: McfMisc_GetLanguageCodeIndex
//	Descrption:获取对应的语言CODE存储在数组中的索引值
//	Param:
//      IN    languagetype:对应的语言类型
//	Return: 
//            对应语言的code存储在数组中的索引值
//  Note:
//------------------------------------------------------------------------------

MUINT McfMisc_GetLanguageCodeIndex(MINT languagetype)
{
    MUINT langindex = 0; 
    switch(languagetype)
    {
        case MMISET_LANGUAGE_ENGLISH:
            langindex = MCARE_LANGUAGE_ENGLISH;
            break;
        case MMISET_LANGUAGE_SIMP_CHINESE:
            langindex = MCARE_LANGUAGE_SIMP_CHINESE;
            break;
        case MMISET_LANGUAGE_TRAD_CHINESE:
            langindex = MCARE_LANGUAGE_TRAD_CHINESE;
            break;
        case MMISET_LANGUAGE_ARABIC:
            langindex = MCARE_LANGUAGE_ARABIC;
            break;
        case MMISET_LANGUAGE_FRENCH:
            langindex = MCARE_LANGUAGE_FRENCH;
            break;
        case MMISET_LANGUAGE_HINDI:
            langindex = MCARE_LANGUAGE_HINDI;
            break;
        case MMISET_LANGUAGE_HUNGARIAN:
            langindex = MCARE_LANGUAGE_HUNGARIAN;
            break;
        case MMISET_LANGUAGE_INDONESIAN:
            langindex = MCARE_LANGUAGE_INDONESIAN;
            break;
        case MMISET_LANGUAGE_MALAY:
            langindex = MCARE_LANGUAGE_MALAY;
            break;
        case MMISET_LANGUAGE_PORTUGUESE:
            langindex = MCARE_LANGUAGE_PORTUGUESE;
            break;
        case MMISET_LANGUAGE_RUSSIAN:
            langindex = MCARE_LANGUAGE_RUSSIAN;
            break;
        case MMISET_LANGUAGE_SPANISH:
            langindex = MCARE_LANGUAGE_SPANISH;
            break;
        case MMISET_LANGUAGE_TAGALOG:
            langindex = MCARE_LANGUAGE_TAGALOG;
            break;
        case MMISET_LANGUAGE_THAI:
            langindex = MCARE_LANGUAGE_THAI;
            break;
        case MMISET_LANGUAGE_VIETNAMESE:
            langindex = MCARE_LANGUAGE_VIETNAMESE;
            break;
        case MMISET_LANGUAGE_URDU:
            langindex = MCARE_LANGUAGE_URDU;
            break;
        case MMISET_LANGUAGE_ITALIAN:
            langindex = MCARE_LANGUAGE_ITALIAN;
            break;         
        case MMISET_LANGUAGE_PERSIAN:
            langindex = MCARE_LANGUAGE_PERSIAN;
            break;            
        case MMISET_LANGUAGE_TURKISH:
            langindex = MCARE_LANGUAGE_TURKISH;
            break;       
        case MMISET_LANGUAGE_GERMAN:
            langindex = MCARE_LANGUAGE_GERMAN;
            break;       
        case MMISET_LANGUAGE_GREEK:
            langindex = MCARE_LANGUAGE_GREEK;
            break;
#ifndef SPREAD_PLAT_SC6600L_0937
        case MMISET_LANGUAGE_HEBREW:
            langindex = MCARE_LANGUAGE_HEBREW;
            break;    
        case MMISET_LANGUAGE_BENGALI:
            langindex = MCARE_LANGUAGE_BENGALI;
            break;    
        case MMISET_LANGUAGE_CZECH:
            langindex = MCARE_LANGUAGE_CZECH;
            break;    
        case MMISET_LANGUAGE_SLOVENIAN:
            langindex = MCARE_LANGUAGE_SLOVENIAN;
            break;    
        case MMISET_LANGUAGE_ROMANIAN:
            langindex = MCARE_LANGUAGE_ROMANIAN;
            break;
#endif
        default:
            langindex = MCARE_LANGUAGE_MAX;
            break;    
    }
    return langindex;
}
//=======================McfMisc.c API End===========================

//=======================McfPen.c API Begin========================== 
MUINT McfPen_GetPenPressed(MVOID)
{
	return MSG_TP_PRESS_DOWN;
}

MUINT McfPen_GetPenReleased(MVOID)
{
	return MSG_TP_PRESS_UP;
}

MUINT McfPen_GetPenMoved(MVOID)
{
	return MSG_TP_PRESS_MOVE;
}
//=======================McfPen.c API End==========================



//=======================McfPb McfPhonebook.c API Begin==========================

MBOOL McfPB_IsStorageReady(MINT nStorage)
{	
	MINT sim_id = MSIM_NONE;
	switch(nStorage)
	{
	case MPHB_ST_SIM1:
		sim_id = MSIM_CARD1;
		break;
#if (defined MCR_SIM2_SUPPORT) || (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
	case MPHB_ST_SIM2:
		sim_id = MSIM_CARD2;
		break;
#endif
#if (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
	case MPHB_ST_SIM3:
		sim_id = MSIM_CARD3;
		break;
#endif
#if (defined MCR_SIM4_SUPPORT)
	case MPHB_ST_SIM4:
		sim_id = MSIM_CARD4;
		break;
#endif

	default:
		sim_id = MSIM_NONE;
		break;
	}
	
	if(MSIM_NONE != sim_id)
	{
		if(!McfSim_IsReady(sim_id))
		{
			McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_GetNextIdByStorage !McfSim_IsReady(sim_id) return MFALSE");
			return MFALSE;			
		}
	}
	return MTRUE;
}


#ifdef SPREAD_NEW_PB_UPDATE

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroId
//	Descrption:窗口消息处理函数
//	Param:
//      IN    mcare_id:MCARE 平台的逻辑存储ID
//      OUT   mocro_id:获取到的相关展讯平台的实际存储组ID
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_McareToMocroId(MINT mcare_id, MUSHORT* mocro_id)
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroId mcare_id=%d,mocro_id=0x%x",mcare_id,mocro_id);

    if(mcare_id <= 0)
    {
        return MFALSE;
    }
    if(MNULL != mocro_id)
    {
        *mocro_id = (MUSHORT)mcare_id;
    }
    return MTRUE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_GetMocroStorage
//	Descrption:窗口消息处理函数
//	Param:
//      IN    mocro_id:展讯 平台的实际存储ID
//      OUT   mcare_id:MCARE平台的逻辑存储组ID
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_MocroToMcareId(MUSHORT mocro_id, MINT* mcare_id )
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareId mocro_id=%d,mcare_id=0x%x",mocro_id,mcare_id);
    if(0 == mocro_id)
    {
        return MFALSE;
    }
    if(MNULL != mcare_id)
    {
        *mcare_id = (MINT)mocro_id;
    }
    return MTRUE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroStorage
//	Descrption:窗口消息处理函数
//	Param:
//      IN    storage:MCARE 平台的逻辑存储类型
//      OUT   group_id:获取到的相关展讯平台的实际存储组ID
//      OUT   mocro_storage:获取到的相关展讯平台的实际存储类型
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_McareToMocroStorage(MINT storage,MBYTE* group_id,MUSHORT* mocro_storage)
{
    MBYTE   groupID = 0;
    MUSHORT   storageID = 0;
    MBOOL   ret = MTRUE;
	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage nStorage=%d",storage);
	switch(storage)
	{
	case MPHB_ST_SIM1:
		groupID = PB_GROUP_SIM_1;
		storageID = MMIPB_STORAGE_SIM1;
		break;
#if (defined MCR_SIM2_SUPPORT) || (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
	case MPHB_ST_SIM2:
		groupID = PB_GROUP_SIM_2;
		storageID = MMIPB_STORAGE_SIM2;
		break;
#endif
#if (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
	case MPHB_ST_SIM3:
		groupID = PB_GROUP_SIM_3;
		storageID = MMIPB_STORAGE_SIM3;
		break;
#endif
#if (defined MCR_SIM4_SUPPORT)
	case MPHB_ST_SIM4:
		groupID = PB_GROUP_SIM_4;
		storageID = MMIPB_STORAGE_SIM4;
		break;
#endif
	case MPHB_ST_PHONE:
		groupID= PB_GROUP_ALL;
		storageID = MMIPB_STORAGE_PHONE;
		break;
	default:
		ret = MFALSE;
		break;
	}
    if(MNULL != group_id)
    {
        *group_id = groupID;
    }
    if(MNULL != mocro_storage)
    {
        *mocro_storage = storageID;
    }
	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage groupID = %d, storageID=%d",groupID, storageID);
	return ret;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcareStorage
//	Descrption:窗口消息处理函数
//	Param:
//      IN   group_id:获取到的相关展讯平台的实际存储组ID
//      OUT    storage:MCARE 平台的逻辑存储类型
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_MocroToMcareStorage(MUSHORT storage_id,MINT* storage)
{
    MINT storageID = 0;
    MBOOL ret = MTRUE;
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareStorage storage_id=%d,storage=0x%x",storage_id,storage);
    storage_id = storage_id&0xFF;
    switch(storage_id)
    {
    case MMIPB_STORAGE_SIM1:
        storageID = MPHB_ST_SIM1;
        break;
#if (defined MCR_SIM2_SUPPORT) || (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
    case MMIPB_STORAGE_SIM2:
        storageID = MPHB_ST_SIM2;
        break;
#endif
#if (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
    case MMIPB_STORAGE_SIM3:
        storageID = MPHB_ST_SIM3;
        break;
#endif
#if (defined MCR_SIM4_SUPPORT)
    case MMIPB_STORAGE_SIM4:
        storageID = MPHB_ST_SIM4;
        break;
#endif
    case MMIPB_STORAGE_PHONE:
        storageID = MPHB_ST_PHONE;
        break;
    default:
		ret = MFALSE;
        break;
    }
    if(MNULL != storage)
    {
        *storage = storageID;
    }
	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareStorage storageID=%d",storageID);
    return ret;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcareNum
//	Descrption:窗口消息处理函数
//	Param:
//      IN    Mocro_num:展讯 平台的实际存储num
//      IN    buffer_len: 保存MCARE平台num的buffer长度
//      OUT   MCR_num:MCARE平台的存储num
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
static MBOOL McfPhb_MocroToMcareNum(const MMIPB_BCD_NUMBER_T* Mocro_num, 
                                        MUSHORT* MCR_num ,MBYTE buffer_len)
{
    MBYTE tel_num[MMIPB_BCD_NUMBER_MAX_LEN*2+3] ={0};
    MBYTE max_len = MMIPB_BCD_NUMBER_MAX_LEN*2+3;
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareNum Mocro_num=0x%x,MCR_num=0x%x,buffer_len=%d",
                Mocro_num,MCR_num,buffer_len);
    if(MNULL == Mocro_num || MNULL == MCR_num || 0==buffer_len)
    {
        return MFALSE;
    }
    max_len = MMIAPICOM_GenDispNumber(MMIPB_GetNumberTypeFromUint8(Mocro_num->npi_ton), 
		Mocro_num->number_len,Mocro_num->number, tel_num,  MMIPB_BCD_NUMBER_MAX_LEN*2+3);
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" MMIAPICOM_GenDispNumber tel_num=%s,max_len=%d",
                tel_num,max_len);
    if(max_len > buffer_len)
    {
        max_len = buffer_len;
    }
    if( max_len > 0)
    {
        MAPI_GUI_UTF8ToWstr(MCR_num,buffer_len*2,tel_num,max_len);
        return MTRUE;
    }
    return MFALSE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroNum
//	Descrption:MCARE平台的号码转换成展讯平台的号码
//	Param:
//      IN   MCR_num:MCARE平台的存储num
//      OUT    Mocro_num:展讯 平台的实际存储num
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
static MBOOL McfPhb_McareToMocroNum(const MUSHORT* MCR_num , MMIPB_BCD_NUMBER_T* Mocro_num)
{	
	MMI_PARTY_NUMBER_T party_num = {0};
	MINT16 num_len = MMIAPICOM_Wstrlen(MCR_num);
	MBYTE num_buf[MMIPB_NUMBER_MAX_STRING_LEN] = {0};
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroNum MCR_num=0x%x,Mocro_num=0x%x",
                MCR_num,Mocro_num);
	if(MNULL == MCR_num || MNULL == Mocro_num || 0>= num_len)
	{
        return MFALSE;
	}
	MMI_WSTRNTOSTR(num_buf, MMIPB_NUMBER_MAX_STRING_LEN, MCR_num, MMIPB_NUMBER_MAX_STRING_LEN, num_len);
	if (num_len > 0&& MMIAPICOM_GenPartyNumber((MBYTE*)(num_buf), num_len, &party_num))
	{
		Mocro_num->npi_ton = (MBYTE)((party_num.number_type << 0X04) | MMIPB_DEFAULT_NPI);
		Mocro_num->number_len =  (MBYTE)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
		McfStdC_memcpy(Mocro_num->number, party_num.bcd_num, Mocro_num->number_len);

	}
	else
	{
		Mocro_num->npi_ton = 0;
		Mocro_num->number_len = 0;
	}
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroNum number_len=%d",Mocro_num->number_len);
	return MTRUE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_GetNextIdByStorage
//	Descrption:窗口消息处理函数
//	Param:
//      IN    storage:MCARE 平台的逻辑存储类型
//      OUT   group_id:获取到的相关展讯平台的实际存储组ID
//      OUT   mocro_storage:获取到的相关展讯平台的实际存储类型
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_GetNextIdByStorage(MINT *entry_id,MUINT *m_index, MINT storage_id)
{
    MBOOL   ret = MFALSE;
    MUINT group_id = 0;
	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"McfPhb_GetNextIdByStorage m_index=%d,storage_id=%d",*m_index,storage_id);
    if(MNULL == m_index || (!MMIPB_IsPbReady()))
    {
		McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"McfPhb_GetNextIdByStorage m_index=MNULL,MMIPB_IsPbReady()=%d",MMIPB_IsPbReady());
        return MFALSE;
    }
    if(!McfPhb_McareToMocroStorage(storage_id,&group_id,MNULL))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MFASLE");
        return MFALSE;
	}
	if(!MMIPB_GetFirstIdWithIndex(group_id,m_index,entry_id))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" MMIPB_GetFirstIdWithIndex return MFASLE");
        return MFALSE;
	}
	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"m_index=%d,entry_id=%d",*m_index,*entry_id);
	return MTRUE;
}

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_ReadEntryByID
//	Descrption:窗口消息处理函数
//	Param:
//      IN   group_id:展讯平台的实际存储组ID
//      IN   entry_id:展讯平台的实际存储ID
//      IN   mocro_storage:展讯平台的实际存储类型
//      OUT  pEntry:MCARE 平台的记录信息
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_ReadEntryByID(MINT entry_id,MINT  Storage_id,McfPhbEntry* pEntry)
{
	MMIPB_CONTACT_T    tPb_Entry = {0};
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" entry_id=%d,Storage_id=%d",
    entry_id,Storage_id);
	if(!McfPhb_McareToMocroId(entry_id,&(tPb_Entry.contact_id)))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroId return MFASLE");
        return MFALSE;
	}

    if(!McfPhb_McareToMocroStorage(Storage_id,&(tPb_Entry.union_t.group),&(tPb_Entry.storage_id)))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MFASLE");
        return MFALSE;
	}
	if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(tPb_Entry.contact_id, tPb_Entry.storage_id, &tPb_Entry))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"McfPhb_ReadEntryByID  return McfPhb_MocroToMcarePb");
		return McfPhb_MocroToMcarePb(&tPb_Entry, pEntry);
	}
	return MFALSE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_AddEntry
//	Descrption:添加记录
//	Param:
//      IN   pUser:回调函数的参数
//      IN   pEntry:待添加的MCARE记录
//      IN   fnAddCb:回调函数
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_AddEntry(MVOID* pUser, McfPhbEntry* pEntry, pfnPhbNotifyCb fnAddCb)
{
	MMIPB_CONTACT_T    tPb_Entry = {0};
    MMIPB_HANDLE_T pb_handle = 0;
    MUSHORT pb_count = 0;
	MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_AddEntry pEntry = 0x%x", pEntry);
    if(MNULL == pEntry)
    {
        return MPHB_ERROR;
    }
	if(!McfPB_IsStorageReady(pEntry->storage))
	{
	    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"McfPhb_AddEntry !McfPB_IsStorageReady(nStorage) MPHB_ERROR");
        return MPHB_ERROR;
	}
    pEntry->id = 1;//随便定义一个大于0的初始值
	if(!McfPhb_McareToMocroPb(pEntry,&tPb_Entry))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"McfPhb_AddEntry McfPhb_McareToMocroPb return MPHB_ERROR");
        return MPHB_ERROR;
	}
	//MMIPB_SetOperateType(MMIPB_OPERATE_TYPE_ADD_ONE);
	
    tPb_Entry.contact_id = McfPhb_GetSpareEntryID(tPb_Entry.storage_id);
    if(!McfPhb_MocroToMcareId(tPb_Entry.contact_id,&(pEntry->id )))
    {
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareId return MFALSE");
        return MPHB_ERROR;
    }

	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_AddEntry tPb_Entry.contact_id = %d", tPb_Entry.contact_id);
	if(0 == tPb_Entry.contact_id)
	{
        return MPHB_ERROR;
	}
    pb_handle = MMIAPIPB_SearchList (MNULL,MMIPB_LIST_FIELD_WHOLE,&pb_count);
    s_app_is_null_pb = MFALSE;
    if(0 == pb_handle)
    {
        pb_handle = MMIAPIPB_CreateHandle();
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"  pb_handle= %d",pb_handle);
        if(0== pb_handle)
        {
            return MPHB_ERROR;
        }
        else
        {
            s_app_is_null_pb = MTRUE;
        }
	}
    ret=MMIAPIPB_AddContactAsyn(pb_handle,&tPb_Entry,McfPhb_AddPBCallBack);
	if(MMIPB_ERROR_SUCCESS !=ret && MMIPB_ERROR_PENDING !=ret)
	{
	    if(s_app_is_null_pb)
	    {
	        MMIAPIPB_CloseHandle(pb_handle);
	    }
	    else
	    {
	        MMIAPIPB_SearchExit(pb_handle);
	    }
		McfPhb_SetAddCallBack(fnAddCb,pUser,pEntry->storage,pEntry->id,MPHB_ERROR);
	    McfTm_StartTimer(MNULL, 50, McfPhb_AddCallBack);
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_AddEntry MMIAPIPB_AddContactAsyn FAIL");
	}	
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" ret=%d, pb_handle=%d",ret,pb_handle);
    s_app_add_callback.m_nId = pEntry->id;
    s_app_add_callback.m_pUser = pUser;
    s_app_add_callback.m_nStorage = pEntry->storage;
    s_app_add_callback.m_pCallBack = fnAddCb;
	
	return MPHB_WAIT;//此处返回的是调用成功，不代表立即add完毕
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcarePb
//	Descrption:展讯的记录转换成MCARE的记录
//	Param:
//      IN   Mocro_Phb:展讯平台的记录信息
//      OUT  Mcf_Phb:MCARE 平台的记录信息
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------

static MBOOL McfPhb_MocroToMcarePb(const MMIPB_CONTACT_T* Mocro_Phb,           McfPhbEntry* Mcf_Phb)
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"  McfPhb_MocroToMcarePb Mocro_Phb=0x%x,Mcf_Phb=0x%x",
                Mocro_Phb,Mcf_Phb);
    if(MNULL == Mocro_Phb || MNULL == Mcf_Phb)
    {
        return MFALSE;
    }
	MAPI_memset(Mcf_Phb, 0, sizeof(McfPhbEntry));
	
	if(!McfPhb_MocroToMcareId(Mocro_Phb->contact_id,&(Mcf_Phb->id )))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareId return MFALSE");
        return MFALSE;
	}
	
    if(!McfPhb_MocroToMcareStorage(Mocro_Phb->storage_id,&(Mcf_Phb->storage)))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareStorage return MFALSE");
        return MFALSE;
	}
    MMIAPICOM_Wstrncpy(Mcf_Phb->name, Mocro_Phb->name.wstr,MPHB_NAME_LENGTH);
	if(Mocro_Phb->number[MOBILE_NUMBER].number_len > 0)
	{
        McfPhb_MocroToMcareNum(&Mocro_Phb->number[MOBILE_NUMBER],Mcf_Phb->number,MPHB_NUMBER_LENGTH+1);
	}
	if(Mocro_Phb->number[HOME_NUMBER].number_len > 0)
	{
        McfPhb_MocroToMcareNum(&Mocro_Phb->number[HOME_NUMBER],Mcf_Phb->homeNumber,MPHB_NUMBER_LENGTH+1);
	}
    if(OFFICE_NUMBER <MMIPB_MAX_NV_PHONE_NUM && Mocro_Phb->number[OFFICE_NUMBER].number_len > 0)
    {
        McfPhb_MocroToMcareNum(&Mocro_Phb->number[OFFICE_NUMBER],Mcf_Phb->officeNumber,MPHB_NUMBER_LENGTH+1);
    }
    if(FAX_NUMBER <MMIPB_MAX_NV_PHONE_NUM && Mocro_Phb->number[FAX_NUMBER].number_len > 0)
    {
        McfPhb_MocroToMcareNum(&Mocro_Phb->number[FAX_NUMBER],Mcf_Phb->faxNumber,MPHB_NUMBER_LENGTH+1);
    }
#ifdef MMIPB_MAIL_SUPPORT		
    MMIAPICOM_Wstrncpy(Mcf_Phb->emailAddr, Mocro_Phb->mail.wstr, MPHB_EMAIL_LENGTH);
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIAPICOM_Wstrncpy(Mcf_Phb->note, Mocro_Phb->memo.wstr, MPHB_NOTE_LENGTH);

#if 0
    MMIAPICOM_Wstrncpy(Mcf_Phb->title, Mocro_Phb->title.wstr, MPHB_TITLE_LENGTH);
    MMIAPICOM_Wstrncpy(Mcf_Phb->companyName, Mocro_Phb->company.wstr, MPHB_NAME_LENGTH);
    MMIAPICOM_Wstrncpy(Mcf_Phb->adrPobox, Mocro_Phb->address.wstr, MPHB_ADDRESS_LENGTH);
#endif

#endif
    return MTRUE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroPb
//	Descrption:MCARE的记录转换成展讯的记录
//	Param:
//      IN  Mcf_Phb:MCARE 平台的记录信息
//      OUT   Mocro_Phb:展讯平台的记录信息
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------

static MBOOL McfPhb_McareToMocroPb(McfPhbEntry* Mcf_Phb, MMIPB_CONTACT_T* Mocro_Phb)
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroPb Mcf_Phb=0x%x,Mocro_Phb=0x%x",
                Mcf_Phb,Mocro_Phb);
    if(MNULL == Mocro_Phb || MNULL == Mcf_Phb)
    {
        return MFALSE;
    }
	SCI_MEMSET(Mocro_Phb, 0, sizeof(MMIPB_CONTACT_T));
	
	if(!McfPhb_McareToMocroId(Mcf_Phb->id,&(Mocro_Phb->contact_id)))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroId return MFASLE");
        return MFALSE;
	}
	
    if(!McfPhb_McareToMocroStorage(Mcf_Phb->storage,&(Mocro_Phb->union_t.group),&(Mocro_Phb->storage_id)))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MFASLE");
        return MFALSE;
	}

	MMIAPICOM_Wstrncpy(Mocro_Phb->name.wstr, Mcf_Phb->name, MMIPB_NAME_MAX_LEN);
    Mocro_Phb->name.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->name.wstr);

	McfPhb_McareToMocroNum(Mcf_Phb->number,&Mocro_Phb->number[MOBILE_NUMBER]);

	McfPhb_McareToMocroNum(Mcf_Phb->homeNumber,&Mocro_Phb->number[HOME_NUMBER]);

    if(OFFICE_NUMBER <MMIPB_MAX_NV_PHONE_NUM)
    {
		McfPhb_McareToMocroNum(Mcf_Phb->officeNumber,&Mocro_Phb->number[OFFICE_NUMBER]);
    }
    if(FAX_NUMBER <MMIPB_MAX_NV_PHONE_NUM)
    {        
		McfPhb_McareToMocroNum(Mcf_Phb->faxNumber,&Mocro_Phb->number[FAX_NUMBER]);
    }
#ifdef MMIPB_MAIL_SUPPORT
    MMIAPICOM_Wstrncpy(Mocro_Phb->mail.wstr,Mcf_Phb->emailAddr, MMIPB_MAX_MAIL_LEN);
    Mocro_Phb->mail.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->mail.wstr);
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIAPICOM_Wstrncpy(Mocro_Phb->memo.wstr,Mcf_Phb->note, MMIPB_MAX_CUSTOM_TEXT_LEN);
    Mocro_Phb->memo.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->memo.wstr);

#if 0
    MMIAPICOM_Wstrncpy(Mocro_Phb->title.wstr,Mcf_Phb->title, MMIPB_MAX_TITLE_LEN);
    Mocro_Phb->title.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->title.wstr);

    MMIAPICOM_Wstrncpy(Mocro_Phb->company.wstr,Mcf_Phb->companyName, MMIPB_MAX_ORG_LEN);
    Mocro_Phb->company.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->company.wstr);

    MMIAPICOM_Wstrncpy(Mocro_Phb->address.wstr,Mcf_Phb->adrPobox, MMIPB_MAX_WORKADR_LEN);
    Mocro_Phb->address.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->address.wstr);
#endif

#endif
    return MTRUE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_GetSpareEntryID
//	Descrption:MCARE的获取展讯通讯录里面空闲的ID
//	Param:
//      IN  group_id:展讯平台的组ID
//	Return: 
//      相应的空闲ID,0:表示没有空闲的ID
//  Note:
//------------------------------------------------------------------------------

MUSHORT McfPhb_GetSpareEntryID(MUSHORT         storage_id)
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_GetSpareEntryID storage_id=%d",storage_id);
	
	if(MMIPB_ERROR_SUCCESS != MMIAPIPB_GetStorageFreeSpace(storage_id, PNULL))
	{
		McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_GetSpareEntryID return 0");
		return 0;
	}
	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_GetSpareEntryID %d", MMIPB_GetFreeContactId(storage_id));
    return MMIPB_GetFreeContactId(storage_id);
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_UpdateEntry
//	Descrption:更新记录
//	Param:
//      IN   pUser:回调函数的参数
//      IN   pEntry:待添加的MCARE记录
//      IN   fnAddCb:回调函数
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_UpdateEntry(MVOID* pUser, McfPhbEntry* pEntry, pfnPhbNotifyCb fnUpdateCb)
{
	MMIPB_CONTACT_T   tPb_Entry = {0};
    MBYTE mocro_storage = 0;
	MMIPB_HANDLE_T pb_handle = 0;
	MUSHORT pb_count = 0;
	MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_UpdateEntry pEntry=0x%x",pEntry);

    if(MNULL == pEntry)
    {
        return MPHB_ERROR;
    }
	if(!McfPB_IsStorageReady(pEntry->storage))
	{
	    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"McfPhb_UpdateEntry !McfPB_IsStorageReady(nStorage) MPHB_ERROR");
        return MPHB_ERROR;
	}
	if(!McfPhb_McareToMocroPb(pEntry,&tPb_Entry))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroPb return MFALSE");
        return MPHB_ERROR;
	}
	
    pb_handle = MMIAPIPB_SearchList (MNULL,MMIPB_LIST_FIELD_WHOLE,&pb_count);
    if(0 == pb_handle)
    {
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" pb_handle= NULL");
        return MPHB_ERROR;
	}
    ret=MMIAPIPB_ModifyContactAsyn(pb_handle,&tPb_Entry,McfPhb_UpdatePBCallBack);
	if(MMIPB_ERROR_SUCCESS !=ret && MMIPB_ERROR_PENDING !=ret)
	{
	    MMIAPIPB_SearchExit(pb_handle);
		McfPhb_SetUpdateCallBack(fnUpdateCb,pUser,pEntry->storage,pEntry->id,MPHB_ERROR);
	    McfTm_StartTimer(MNULL, 50, McfPhb_UpdateCallBack);
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_UpdateEntry MMIAPIPB_ModifyContactAsyn FAIL");
	}	
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" ret=%d, pb_handle=%d",ret,pb_handle);
    s_app_update_callback.m_nId = pEntry->id;
    s_app_update_callback.m_pUser = pUser;
    s_app_update_callback.m_nStorage = pEntry->storage;
    s_app_update_callback.m_pCallBack = fnUpdateCb;

	return MPHB_WAIT;//此处返回的是调用成功，不代表立即add完毕
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_DelEntry
//	Descrption:删除记录
//	Param:
//      IN   pUser:回调函数的参数
//      IN   nId: 删除的ID
//      IN   nStorage: 删除记录存储的类型
//      IN   fnAddCb:回调函数
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------
MINT McfPhb_DelEntry(MVOID* pUser, MINT nId, MINT nStorage, pfnPhbNotifyCb fnDelCb)
{
	MUSHORT entry_id = 0;
	MUSHORT mocro_storage = 0;
	MMIPB_HANDLE_T pb_handle = 0;
	MUSHORT pb_count = 0;
	MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_DelEntry nId=%d,nStorage=%d",
                nId,nStorage);
	
	if(!McfPB_IsStorageReady(nStorage))
	{
	    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"McfPhb_DelEntry !McfPB_IsStorageReady(nStorage) MPHB_ERROR");
        return MPHB_ERROR;
	}
	if(!McfPhb_McareToMocroId(nId,&entry_id))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroId return MFALSE");
        return MPHB_ERROR;
    }
    if(!McfPhb_McareToMocroStorage(nStorage,MNULL,&mocro_storage))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MFASLE");
        return MPHB_ERROR;
	}
    pb_handle = MMIAPIPB_SearchList (MNULL,MMIPB_LIST_FIELD_WHOLE,&pb_count);
    if(0 == pb_handle)
    {
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" pb_handle= NULL");
        return MPHB_ERROR;
	}
    ret=MMIAPIPB_DeleteContactAsyn(pb_handle,entry_id,mocro_storage,McfPhb_DelPBCallBack);
	if(MMIPB_ERROR_SUCCESS !=ret && MMIPB_ERROR_PENDING !=ret)
	{
	    MMIAPIPB_SearchExit(pb_handle);
		McfPhb_SetDelCallBack(fnDelCb,pUser,nStorage,nId,MPHB_ERROR);
	    McfTm_StartTimer(MNULL, 50, McfPhb_DelCallBack);
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_DelEntry MMIPB_DeleteContact FAIL");
	}	
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" ret=%d, pb_handle=%d",ret,pb_handle);
    s_app_del_callback.m_nId = nId;
    s_app_del_callback.m_pUser = pUser;
    s_app_del_callback.m_nStorage = nStorage;
    s_app_del_callback.m_pCallBack = fnDelCb;
	return MPHB_WAIT;//此处返回的是调用成功，不代表立即add完毕
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_QueryProperty
//	Descrption:获取相关的电话本中的属性
//	Param:
//      IN   pProp:保存获取到的属性值
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_QueryProperty(McfPhbProp* pProp)
{
    if(MNULL != pProp)
    {
        MUINT mask = 0;
        mask |= MPHB_FILED_NAME|MPHB_FILED_NUMBER|MPHB_FILED_HOMENUM;
        if(OFFICE_NUMBER <MMIPB_MAX_NV_PHONE_NUM)
        {
            mask |= MPHB_FILED_OFFICENUM;
        }
        if(FAX_NUMBER <MMIPB_MAX_NV_PHONE_NUM)
        {
            mask |= MPHB_FILED_FAXNUM;
        }
        mask |= MPHB_FILED_EMAIL;

#ifndef PB_SUPPORT_LOW_MEMORY
        mask |= MPHB_FILED_COMPANYNAME;
        mask |= MPHB_FILED_NOTE;
        mask |= MPHB_FILED_TITLE;
#endif
        
        pProp->mask = mask;
        pProp->maxNameLen = MPHB_NAME_LENGTH;
        if(MMIPB_NAME_MAX_LEN < MPHB_NAME_LENGTH)
        {
            pProp->maxNameLen = MMIPB_NAME_MAX_LEN;
        }
        pProp->maxNumLen = (MMIPB_BCD_NUMBER_MAX_LEN<<1);
        pProp->reserved1 = 0;
        pProp->reserved2 = 0;
    }
    return MPHB_SUCCESS;
}

MINT McfIF_Misc_GetCharPinyin(MUSHORT ucs2Char, MCHAR* pPinyin, MINT nSize)
{
	MCHAR tempPinyin[128] = {0}; 
	MWCHAR str[2] = {0};

	MWCHAR wstr_pinyin[128] = {0};	
	MMIPB_NAME_T s_name = {0};
	
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfIF_Misc_GetCharPinyin ucs2Char = %d, pPinyin = 0x%x, %s, nSize = %d", ucs2Char, pPinyin, pPinyin, nSize);

	if(nSize <= 1)
	{
		return 0;
	}
	
	str[0] = ucs2Char;
	s_name.wstr_len = McfStdC_wcslen((const MWCHAR *)str);
	McfStdC_wcsncpy(s_name.wstr, (const MWCHAR *)str, (MINT)s_name.wstr_len);
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
#ifdef PB_SEARCH_SURPPORT_CHAR
{
	MMIPB_NAME_LETTERS_T letters = {0};
	MMIPB_NAME_LETTERS_T number = {0};
	MMIPB_GetNameLetters(&s_name,&letters);
	McfStdC_wcsncpy(wstr_pinyin, letters.full_letters, letters.full_letters_len);
}
#else
	//此处需要客户的接口支持获取汉字的拼音
#endif
#else
{
	MMIPB_NAME_LETTERS_T letters = {0};
#ifdef SPREAD_PLAT_SC6530
	MMIPB_GetNameLetters(&s_name,MMIPB_NAME_HEAD_CHAR,TRUE,&letters);
#else
	MMIPB_NAME_LETTERS_T number = {0};
	MMIPB_GetNameLetters(&s_name,&letters,&number);
#endif
	McfStdC_wcsncpy(wstr_pinyin, letters.full_letters, letters.full_letters_len);
}
#endif

	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfIF_Misc_GetCharPinyin McfStdC_wcslen(wstr_pinyin) = %d", McfStdC_wcslen(wstr_pinyin));
	if(0 >= McfStdC_wcslen(wstr_pinyin))
	{
		pPinyin[0] = '\0';
		return 0;
	}
	
	McfStdC_wstrtostr(tempPinyin,wstr_pinyin);

	if(McfStdC_strlen(tempPinyin) >= nSize - 1)
	{
		McfStdC_strncpy(pPinyin, tempPinyin, nSize-1);
		pPinyin[nSize-1] = '\0';
		MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfIF_Misc_GetCharPinyin 11 pPinyin = 0x%x, %s", pPinyin, pPinyin);
		return (nSize - 1);
	}
	else
	{
		McfStdC_strncpy(pPinyin, tempPinyin, McfStdC_strlen(tempPinyin));
		pPinyin[McfStdC_strlen(tempPinyin)] = '\0';
		MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfIF_Misc_GetCharPinyin 22 pPinyin = 0x%x, %s", pPinyin, pPinyin);
		return McfStdC_strlen(tempPinyin);
	}
}

MBOOL MAPI_MMIPB_GetPBIsSorting(MVOID)
{
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"MAPI_MMIPB_GetPBIsSorting %d", !MMIPB_IsPbReady());
	return !MMIPB_IsPbReady();
}

MUSHORT MAPI_MMIPB_GetEntryCountInNv(MVOID)
{
    return MCARE_MMIPB_GetUsedContactNumOfNV();
}

MUSHORT MAPI_MMIPB_GetMaxContactNumOfNV(MVOID)
{
    return MMIPB_GetMaxContactNumOfNV();
}
MUSHORT Mcfphb_GetMaxRecordNumByStorage(MINT storge_id)
{
    MUSHORT mocro_storage=0;
    if(!McfPhb_McareToMocroStorage(storge_id,MNULL,&mocro_storage))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MFASLE");
        return 0;
	}

    return MCARE_MMIPB_GetMaxRecordNumByStorage(mocro_storage);
}
MUSHORT Mcfphb_GetUsedRecordNumByStorage(MINT storge_id)
{
    MUSHORT mocro_storage=0;
    if(!McfPhb_McareToMocroStorage(storge_id,MNULL,&mocro_storage))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MFASLE");
        return 0;
	}

    return MCARE_MMIPB_GetUsedRecordNumByStorage(mocro_storage);
}
static MVOID McfPhb_DelPBCallBack(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" handle=%d error=%d",handle,error);
    if(MMIPB_ERROR_SUCCESS == error)
    {
        s_app_del_callback.m_nResult = MPHB_SUCCESS;

    }
    else
    {
        s_app_del_callback.m_nResult = MPHB_ERROR;
    }
    McfPhb_SetDelCallBack(s_app_del_callback.m_pCallBack,
                            s_app_del_callback.m_pUser,
                            s_app_del_callback.m_nStorage,
                            s_app_del_callback.m_nId,
                            s_app_del_callback.m_nResult);
    McfPhb_DelCallBack(MNULL);
    MMIAPIPB_SearchExit(handle);
}
static MVOID McfPhb_UpdatePBCallBack(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" handle=%d error=%d",handle,error);
    if(MMIPB_ERROR_SUCCESS == error)
    {
        s_app_update_callback.m_nResult = MPHB_SUCCESS;

    }
    else
    {
        s_app_update_callback.m_nResult = MPHB_ERROR;
    }
    McfPhb_SetUpdateCallBack(s_app_update_callback.m_pCallBack,
                            s_app_update_callback.m_pUser,
                            s_app_update_callback.m_nStorage,
                            s_app_update_callback.m_nId,
                            s_app_update_callback.m_nResult);
    McfPhb_UpdateCallBack(MNULL);
    MMIAPIPB_SearchExit(handle);
}
static MVOID McfPhb_AddPBCallBack(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" handle=%d error=%d",handle,error);
    if(MMIPB_ERROR_SUCCESS == error)
    {
        s_app_add_callback.m_nResult = MPHB_SUCCESS;

    }
    else
    {
        s_app_add_callback.m_nResult = MPHB_ERROR;
    }
    McfPhb_SetAddCallBack(s_app_add_callback.m_pCallBack,
                            s_app_add_callback.m_pUser,
                            s_app_add_callback.m_nStorage,
                            s_app_add_callback.m_nId,
                            s_app_add_callback.m_nResult);
    McfPhb_AddCallBack(MNULL);
    if(s_app_is_null_pb)
    {
        MMIAPIPB_CloseHandle(handle);
    }
    else
    {
        MMIAPIPB_SearchExit(handle);
    }

}

#else//#ifdef SPREAD_NEW_PB_UPDATE

MBOOL  MAPI_MMIAPIPB_OpenListAllGroupWin(MVOID)
{
	return MMIAPIPB_OpenListAllGroupWin();
}


MINT McfIF_GetMcarePbList(MVOID)
{
	return MMIPB_LIST_FOR_MCARE;
}
MINT McfIF_GetPbListType(MVOID)
{
	return MMIPB_LIST_TYPE_NONE;
}

#if (defined SPREAD_PLAT_SC6800H) ||(defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
MINT McfIF_GetPbSearchType(MVOID)
{
	return MMIPB_SEARCH_NONE;
}
#endif

MVOID McfPB_OpenNumberList(MVOID) //Note: nicklei  add  McfPB_OpenNumberList
{
#if (defined SPREAD_PLAT_SC6800H) ||(defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)||(defined SPREAD_PLAT_SC8800G_PDA)
	MMIPB_OpenNumberList(McfIF_GetMcarePbList(), McfIF_GetPbListType(), McfIF_GetPbSearchType(), 1, MTRUE, MNULL);
#else
	MMIPB_OpenNumberList(McfIF_GetMcarePbList(), McfIF_GetPbListType(), 1, MTRUE, MNULL);
#endif
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroId
//	Descrption:窗口消息处理函数
//	Param:
//      IN    mcare_id:MCARE 平台的逻辑存储ID
//      OUT   mocro_id:获取到的相关展讯平台的实际存储组ID
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_McareToMocroId(MINT mcare_id, MUSHORT* mocro_id)
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroId mcare_id=%d,mocro_id=0x%x",mcare_id,mocro_id);

    if(mcare_id <= 0)
    {
        return MFALSE;
    }
    if(MNULL != mocro_id)
    {
        *mocro_id = (MUSHORT)mcare_id;
    }
    return MTRUE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_GetMocroStorage
//	Descrption:窗口消息处理函数
//	Param:
//      IN    mocro_id:展讯 平台的实际存储ID
//      OUT   mcare_id:MCARE平台的逻辑存储组ID
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_MocroToMcareId(MUSHORT mocro_id, MINT* mcare_id )
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareId mocro_id=%d,mcare_id=0x%x",mocro_id,mcare_id);
    if(0 == mocro_id)
    {
        return MFALSE;
    }
    if(MNULL != mcare_id)
    {
        *mcare_id = (MINT)mocro_id;
    }
    return MTRUE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroStorage
//	Descrption:窗口消息处理函数
//	Param:
//      IN    storage:MCARE 平台的逻辑存储类型
//      OUT   group_id:获取到的相关展讯平台的实际存储组ID
//      OUT   mocro_storage:获取到的相关展讯平台的实际存储类型
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_McareToMocroStorage(MINT storage,MBYTE* group_id,MUSHORT* mocro_storage)
{
    MBYTE   groupID = 0;
    MUSHORT   storageID = 0;
    MBOOL   ret = MTRUE;
	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage nStorage=%d",storage);
	switch(storage)
	{
    	case MPHB_ST_SIM1:
    		groupID = PB_GROUP_SIM_1;
    		storageID = PHONEBOOK_SIM_STORAGE;
    		break;
#if (defined MCR_SIM2_SUPPORT) || (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
    	case MPHB_ST_SIM2:
    		groupID = PB_GROUP_SIM_2;
    		storageID = PHONEBOOK_SIM_STORAGE;
    		break;
#endif
#if (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
    	case MPHB_ST_SIM3:
    		groupID = PB_GROUP_SIM_3;
    		storageID = PHONEBOOK_SIM_STORAGE;
    		break;
#endif
#if (defined MCR_SIM4_SUPPORT)
    	case MPHB_ST_SIM4:
    		groupID = PB_GROUP_SIM_4;
    		storageID = PHONEBOOK_SIM_STORAGE;
    		break;
#endif
    	case MPHB_ST_PHONE:
    		groupID= PB_GROUP_UNCLASS;
    		storageID = PHONEBOOK_NV_STORAGE;
    		break;
    	default:
    		ret = MFALSE;
    		break;
	}
    if(MNULL != group_id)
    {
        *group_id = groupID;
    }
    if(MNULL != mocro_storage)
    {
        *mocro_storage = storageID;
    }
	return ret;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcareStorage
//	Descrption:窗口消息处理函数
//	Param:
//      IN   group_id:获取到的相关展讯平台的实际存储组ID
//      OUT    storage:MCARE 平台的逻辑存储类型
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_MocroToMcareStorage(MBYTE group_id,MINT* storage)
{
    MINT storageID = 0;
    MBOOL ret = MTRUE;
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareStorage group_id=%d,storage=0x%x",group_id,storage);
    switch(group_id)
    {
        case PB_GROUP_SIM_1:
            storageID = MPHB_ST_SIM1;
            break;
#if (defined MCR_SIM2_SUPPORT) || (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
        case PB_GROUP_SIM_2:
            storageID = MPHB_ST_SIM2;
            break;
#endif
#if (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
        case PB_GROUP_SIM_3:
            storageID = MPHB_ST_SIM3;
            break;
#endif
#if (defined MCR_SIM4_SUPPORT)
        case PB_GROUP_SIM_4:
            storageID = MPHB_ST_SIM4;
            break;
#endif
        case PB_GROUP_ALL:
            ret = MFALSE;
        default:
            storageID = MPHB_ST_PHONE;
            break;
    }
    if(MNULL != storage)
    {
        *storage = storageID;
    }
    return ret;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcareNum
//	Descrption:窗口消息处理函数
//	Param:
//      IN    Mocro_num:展讯 平台的实际存储num
//      IN    buffer_len: 保存MCARE平台num的buffer长度
//      OUT   MCR_num:MCARE平台的存储num
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
static MBOOL McfPhb_MocroToMcareNum(const MMIPB_BCD_NUMBER_T* Mocro_num, 
                                        MUSHORT* MCR_num ,MBYTE buffer_len)
{
    MBYTE tel_num[MMIPB_BCD_NUMBER_MAX_LEN*2+3] ={0};
    MBYTE max_len = MMIPB_BCD_NUMBER_MAX_LEN*2+3;
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareNum Mocro_num=0x%x,MCR_num=0x%x,buffer_len=%d",
                Mocro_num,MCR_num,buffer_len);
    if(MNULL == Mocro_num || MNULL == MCR_num || 0==buffer_len)
    {
        return MFALSE;
    }
    max_len = MMIAPICOM_GenDispNumber(MMIPB_GetNumberTypeFromUint8(Mocro_num->npi_ton), 
		Mocro_num->number_len,Mocro_num->number, tel_num,  MMIPB_BCD_NUMBER_MAX_LEN*2+3);
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" MMIAPICOM_GenDispNumber tel_num=%s,max_len=%d",
                tel_num,max_len);
    if(max_len > buffer_len)
    {
        max_len = buffer_len;
    }
    if( max_len > 0)
    {
        MAPI_GUI_UTF8ToWstr(MCR_num,buffer_len*2,tel_num,max_len);
        return MTRUE;
    }
    return MFALSE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroNum
//	Descrption:MCARE平台的号码转换成展讯平台的号码
//	Param:
//      IN   MCR_num:MCARE平台的存储num
//      OUT    Mocro_num:展讯 平台的实际存储num
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
static MBOOL McfPhb_McareToMocroNum(const MUSHORT* MCR_num , MMIPB_BCD_NUMBER_T* Mocro_num)
{	
	MMI_PARTY_NUMBER_T party_num = {0};
	MINT16 num_len = MMIAPICOM_Wstrlen(MCR_num);
	MBYTE num_buf[MMIPB_NUMBER_MAX_STRING_LEN] = {0};
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroNum MCR_num=0x%x,Mocro_num=0x%x",
                MCR_num,Mocro_num);
	if(MNULL == MCR_num || MNULL == Mocro_num || 0>= num_len)
	{
        return MFALSE;
	}
	MMI_WSTRNTOSTR(num_buf, MMIPB_NUMBER_MAX_STRING_LEN, MCR_num, MMIPB_NUMBER_MAX_STRING_LEN, num_len);
	if (num_len > 0&& MMIAPICOM_GenPartyNumber((MBYTE*)(num_buf), num_len, &party_num))
	{
		Mocro_num->npi_ton = (MBYTE)((party_num.number_type << 0X04) | MMIPB_DEFAULT_NPI);
		Mocro_num->number_len =  (MBYTE)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
		McfStdC_memcpy(Mocro_num->number, party_num.bcd_num, Mocro_num->number_len);

	}
	else
	{
		Mocro_num->npi_ton = 0;
		Mocro_num->number_len = 0;
	}
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroNum number_len=%d",Mocro_num->number_len);
	return MTRUE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_GetNextIdByGroup
//	Descrption:窗口消息处理函数
//	Param:
//      IN    storage:MCARE 平台的逻辑存储类型
//      OUT   group_id:获取到的相关展讯平台的实际存储组ID
//      OUT   mocro_storage:获取到的相关展讯平台的实际存储类型
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_GetNextIdByStorage(MINT *entry_id,MUINT *m_index, MINT storage_id)
{
    MBOOL   ret = MFALSE;
    MUINT index = 0;
    MBYTE group_id = 0;
	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"m_index=%d,group_id=%d",m_index,group_id);
    if(MNULL == m_index || !g_mmipb_entry_list.sort_t.is_initialized)
    {
	    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"is_initialized=%d",g_mmipb_entry_list.sort_t.is_initialized);
        return MFALSE;
    }
    if(!McfPhb_McareToMocroStorage(storage_id, &group_id, MNULL))
    {
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MPHB_ERROR");
        return MFALSE;
    }
    if( group_id >= MMIPB_MAX_GROUP_NUM || 0 == g_mmipb_entry_list.sort_t.group_count[group_id])
    {
	    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"group_count=%d",g_mmipb_entry_list.sort_t.group_count[group_id]);
        return MFALSE;
    }
	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"m_index=%d,group_id=%d,count=%d",*m_index,group_id,g_mmipb_entry_list.sort_t.count);

	for(index = *m_index; index < g_mmipb_entry_list.sort_t.count; index++)
	{
        if(group_id == g_mmipb_entry_list.sort_t.content[index].group_id)
        {
            if(MNULL != entry_id)
            {
                *entry_id = g_mmipb_entry_list.sort_t.content[index].entry_id;
            }
            if(MNULL != m_index)
            {
                *m_index = index;
            }
            ret = MTRUE;
            break;
        }
	}
	McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"m_index=%d,entry_id=%d",*m_index,g_mmipb_entry_list.sort_t.content[index].entry_id);
	return ret;
}

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_ReadEntryByID
//	Descrption:窗口消息处理函数
//	Param:
//      IN   group_id:展讯平台的实际存储组ID
//      IN   entry_id:展讯平台的实际存储ID
//      IN   mocro_storage:展讯平台的实际存储类型
//      OUT  pEntry:MCARE 平台的记录信息
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------

MBOOL McfPhb_ReadEntryByID(MINT entry_id, MINT  Storage_id,McfPhbEntry* pEntry)
{
	MMIPB_PHONEBOOK_ENTRY_T   tPb_Entry = {0};
	MUSHORT mocro_storage =0;
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_ReadEntryByID entry_id=%d,Storage_id=%d",
    entry_id,Storage_id);
	if(!McfPhb_McareToMocroId(entry_id,&(tPb_Entry.entry_id)))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroId return MFASLE");
        return MFALSE;
	}
	
	if(!McfPhb_McareToMocroStorage(Storage_id, &(tPb_Entry.group_id), &mocro_storage))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MPHB_ERROR");
        return MFALSE;
	}
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
	if(MMIPB_ReadPhonebookEntry(mocro_storage, &tPb_Entry))
#else
	if(MMIPB_ReadPhonebookFullEntry(mocro_storage, &tPb_Entry))
#endif
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"  return McfPhb_MocroToMcarePb");
		return McfPhb_MocroToMcarePb(&tPb_Entry, pEntry);
	}
	return MFALSE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_AddEntry
//	Descrption:添加记录
//	Param:
//      IN   pUser:回调函数的参数
//      IN   pEntry:待添加的MCARE记录
//      IN   fnAddCb:回调函数
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_AddEntry(MVOID* pUser, McfPhbEntry* pEntry, pfnPhbNotifyCb fnAddCb)
{
	MMIPB_PHONEBOOK_ENTRY_T   tPb_Entry = {0};
	MUSHORT mocro_storage = 0;
	MINT result = MPHB_ERROR;
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" pEntry=%d,pUser=%d",pEntry,pUser);
    if(MNULL == pEntry)
    {
        return MPHB_ERROR;
    }
    pEntry->id = 1;//随便定义一个大于0的初始值
	if(!McfPhb_McareToMocroStorage(pEntry->storage, MNULL, &mocro_storage))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MPHB_ERROR");
        return MPHB_ERROR;
	}

	if(!McfPhb_McareToMocroPb(pEntry,&tPb_Entry))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroPb return MPHB_ERROR");
        return MPHB_ERROR;
	}
	MMIPB_SetOperateType(MMIPB_OPERATE_TYPE_ADD_ONE);
	
    tPb_Entry.entry_id = McfPhb_GetSpareEntryID(tPb_Entry.group_id) ;
    if(!McfPhb_MocroToMcareId(tPb_Entry.entry_id,&pEntry->id))
    {
        return MPHB_ERROR;
    }
    if(0 == tPb_Entry.entry_id)
    {
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_GetSpareEntryID return 0");
        return MPHB_SUCCESS;
	}
	if (MMIPB_UpdatePhonebookEntry(mocro_storage, &tPb_Entry))
	{
        g_mmipb_entry_t.entry_id = tPb_Entry.entry_id;
        g_mmipb_entry_t.group_id = tPb_Entry.group_id;
		if(MMIPB_Add(&tPb_Entry))
		{
            McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" MMIPB_Add SUCCESS");
			result = MPHB_SUCCESS;
		}
	}
	McfPhb_SetAddCallBack(fnAddCb,pUser,pEntry->storage,(MINT)tPb_Entry.entry_id,result);
    //同步的时候不需要调用回调,
    McfTm_StartTimer(MNULL, 50, McfPhb_AddCallBack);
	return MPHB_WAIT;//此处返回的是调用成功，不代表立即add完毕
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcarePb
//	Descrption:展讯的记录转换成MCARE的记录
//	Param:
//      IN   Mocro_Phb:展讯平台的记录信息
//      OUT  Mcf_Phb:MCARE 平台的记录信息
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------

static MBOOL McfPhb_MocroToMcarePb(const MMIPB_PHONEBOOK_ENTRY_T* Mocro_Phb,
                                            McfPhbEntry* Mcf_Phb)
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__"  McfPhb_MocroToMcarePb Mocro_Phb=0x%x,Mcf_Phb=0x%x",
                Mocro_Phb,Mcf_Phb);
    if(MNULL == Mocro_Phb || MNULL == Mcf_Phb)
    {
        return MFALSE;
    }
	MAPI_memset(Mcf_Phb, 0, sizeof(McfPhbEntry));
	
	if(!McfPhb_MocroToMcareId(Mocro_Phb->entry_id,&(Mcf_Phb->id )))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareId return MFALSE");
        return MFALSE;
	}
	
    if(!McfPhb_MocroToMcareStorage(Mocro_Phb->group_id,&(Mcf_Phb->storage)))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_MocroToMcareStorage return MFALSE");
        return MFALSE;
	}
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
    MMIAPICOM_Wstrncpy(Mcf_Phb->name, Mocro_Phb->entry_name.wstr,MPHB_NAME_LENGTH);
#else
    MMIAPICOM_Wstrncpy(Mcf_Phb->name, Mocro_Phb->union_t.entry_name.wstr,MPHB_NAME_LENGTH);
#endif
	if(Mocro_Phb->number_t[MOBILE_NUMBER].number_len > 0)
	{
        McfPhb_MocroToMcareNum(&Mocro_Phb->number_t[MOBILE_NUMBER],Mcf_Phb->number,MPHB_NUMBER_LENGTH+1);
	}
	if(Mocro_Phb->number_t[HOME_NUMBER].number_len > 0)
	{
        McfPhb_MocroToMcareNum(&Mocro_Phb->number_t[HOME_NUMBER],Mcf_Phb->homeNumber,MPHB_NUMBER_LENGTH+1);
	}
    if(OFFICE_NUMBER <MMIPB_MAX_NV_PHONE_NUM && Mocro_Phb->number_t[OFFICE_NUMBER].number_len > 0)
    {
        McfPhb_MocroToMcareNum(&Mocro_Phb->number_t[OFFICE_NUMBER],Mcf_Phb->officeNumber,MPHB_NUMBER_LENGTH+1);
    }
    if(FAX_NUMBER <MMIPB_MAX_NV_PHONE_NUM && Mocro_Phb->number_t[FAX_NUMBER].number_len > 0)
    {
        McfPhb_MocroToMcareNum(&Mocro_Phb->number_t[FAX_NUMBER],Mcf_Phb->faxNumber,MPHB_NUMBER_LENGTH+1);
    }
		
    MMIAPICOM_Wstrncpy(Mcf_Phb->emailAddr, Mocro_Phb->mail.wstr, MPHB_EMAIL_LENGTH);
#ifndef PB_SUPPORT_LOW_MEMORY

    MMIAPICOM_Wstrncpy(Mcf_Phb->title, Mocro_Phb->title.wstr, MPHB_TITLE_LENGTH);
    
    MMIAPICOM_Wstrncpy(Mcf_Phb->companyName, Mocro_Phb->org.wstr, MPHB_NAME_LENGTH);

    MMIAPICOM_Wstrncpy(Mcf_Phb->note, Mocro_Phb->custom_text.wstr, MPHB_NOTE_LENGTH);
#endif
    return MTRUE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroPb
//	Descrption:MCARE的记录转换成展讯的记录
//	Param:
//      IN  Mcf_Phb:MCARE 平台的记录信息
//      OUT   Mocro_Phb:展讯平台的记录信息
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------

static MBOOL McfPhb_McareToMocroPb(McfPhbEntry* Mcf_Phb, MMIPB_PHONEBOOK_ENTRY_T* Mocro_Phb)
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroPb Mcf_Phb=0x%x,Mocro_Phb=0x%x",
                Mcf_Phb,Mocro_Phb);
    if(MNULL == Mocro_Phb || MNULL == Mcf_Phb)
    {
        return MFALSE;
    }
	SCI_MEMSET(Mocro_Phb, 0, sizeof(MMIPB_PHONEBOOK_ENTRY_T));
	
	if(!McfPhb_McareToMocroId(Mcf_Phb->id,&(Mocro_Phb->entry_id)))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroId return MFASLE");
        return MFALSE;
	}
	
    if(!McfPhb_McareToMocroStorage(Mcf_Phb->storage,&(Mocro_Phb->group_id),MNULL))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MFASLE");
        return MFALSE;
	}

#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
	MMIAPICOM_Wstrncpy(Mocro_Phb->entry_name.wstr, Mcf_Phb->name,MMIPB_NAME_MAX_LEN);
    Mocro_Phb->entry_name.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->entry_name.wstr);
#else
	MMIAPICOM_Wstrncpy(Mocro_Phb->union_t.entry_name.wstr, Mcf_Phb->name,MMIPB_NAME_MAX_LEN);
    Mocro_Phb->union_t.entry_name.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->union_t.entry_name.wstr);
#endif

    McfPhb_McareToMocroNum(Mcf_Phb->number,&Mocro_Phb->number_t[MOBILE_NUMBER]);


    McfPhb_McareToMocroNum(Mcf_Phb->homeNumber,&Mocro_Phb->number_t[HOME_NUMBER]);
	

    if(OFFICE_NUMBER <MMIPB_MAX_NV_PHONE_NUM)
    {
        McfPhb_McareToMocroNum(Mcf_Phb->officeNumber,&Mocro_Phb->number_t[OFFICE_NUMBER]);
    }
    if(FAX_NUMBER <MMIPB_MAX_NV_PHONE_NUM)
    {        
        McfPhb_McareToMocroNum(Mcf_Phb->faxNumber,&Mocro_Phb->number_t[FAX_NUMBER]);
    }

    MMIAPICOM_Wstrncpy(Mocro_Phb->mail.wstr,Mcf_Phb->emailAddr,MMIPB_MAX_MAIL_LEN);
    Mocro_Phb->mail.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->mail.wstr);
#ifndef PB_SUPPORT_LOW_MEMORY

    MMIAPICOM_Wstrncpy(Mocro_Phb->title.wstr,Mcf_Phb->title,MMIPB_MAX_TITLE_LEN);
    Mocro_Phb->title.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->title.wstr);

    MMIAPICOM_Wstrncpy(Mocro_Phb->org.wstr,Mcf_Phb->companyName,MMIPB_MAX_ORG_LEN);
    Mocro_Phb->org.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->org.wstr);

    MMIAPICOM_Wstrncpy(Mocro_Phb->custom_text.wstr,Mcf_Phb->note,MMIPB_MAX_CUSTOM_TEXT_LEN);
    Mocro_Phb->custom_text.wstr_len = MMIAPICOM_Wstrlen(Mocro_Phb->custom_text.wstr);

    //MMIAPICOM_Wstrncpy(Mocro_Phb->work_adr.wstr,Mcf_Phb->note,MMIAPICOM_Wstrlen(Mcf_Phb->note));
#endif
    return MTRUE;
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_GetSpareEntryID
//	Descrption:MCARE的获取展讯通讯录里面空闲的ID
//	Param:
//      IN  group_id:展讯平台的组ID
//	Return: 
//      相应的空闲ID,0:表示没有空闲的ID
//  Note:
//------------------------------------------------------------------------------

MUSHORT McfPhb_GetSpareEntryID(MBYTE         group_id)
{
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_GetSpareEntryID group_id=%d",group_id);
    if(!MMIPB_CheckSpace(group_id,1))
    {
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" MMIPB_CheckSpace return MFALSE");
        return 0;
    }
    return MMIPB_GetSpareId(&(g_mmipb_entry_list.nv_entry_id_t), group_id);
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_UpdateEntry
//	Descrption:更新记录
//	Param:
//      IN   pUser:回调函数的参数
//      IN   pEntry:待添加的MCARE记录
//      IN   fnAddCb:回调函数
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_UpdateEntry(MVOID* pUser, McfPhbEntry* pEntry, pfnPhbNotifyCb fnUpdateCb)
{
	MMIPB_PHONEBOOK_ENTRY_T   tPb_Entry = {0};
	MINT result = MPHB_ERROR;
    MUSHORT mocro_storage = 0;
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_UpdateEntry pEntry=0x%x",pEntry);

    if(MNULL == pEntry)
    {
        return MPHB_ERROR;
    }
	if(!McfPhb_McareToMocroStorage(pEntry->storage, MNULL, &mocro_storage))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MFALSE");
        return MPHB_ERROR;
	}
	if(!McfPhb_McareToMocroPb(pEntry,&tPb_Entry))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroPb return MFALSE");
        return MPHB_ERROR;
	}
	
	if (MMIPB_UpdatePhonebookEntry(mocro_storage, &tPb_Entry))
	{
	    g_mmipb_entry_t.entry_id = tPb_Entry.entry_id;
	    g_mmipb_entry_t.group_id = tPb_Entry.group_id;
        if(MMIPB_Erase(tPb_Entry.entry_id,tPb_Entry.group_id))
        {
            McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" MMIPB_Erase return MTRUE");
            if(MMIPB_IsInNvGroup(tPb_Entry.group_id))
            {
                McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" call MMIPB_Add");
                MMIPB_Add(&tPb_Entry);
            }
            result = MPHB_SUCCESS;
        }
	}
	//return MPHB_ERROR;
	McfPhb_SetUpdateCallBack(fnUpdateCb,pUser,pEntry->storage,pEntry->id,(MINT)result);
	McfTm_StartTimer(MNULL, 50, McfPhb_UpdateCallBack);
	return MPHB_WAIT;//此处返回的是调用成功，不代表立即add完毕
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_DelEntry
//	Descrption:删除记录
//	Param:
//      IN   pUser:回调函数的参数
//      IN   nId: 删除的ID
//      IN   nStorage: 删除记录存储的类型
//      IN   fnAddCb:回调函数
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------
MINT McfPhb_DelEntry(MVOID* pUser, MINT nId, MINT nStorage, pfnPhbNotifyCb fnDelCb)
{
	MN_DUAL_SYS_E eDual_sys = MN_DUAL_SYS_1;
	MINT result = MPHB_WAIT;
	MBYTE group_id = 0;
	MUSHORT mocro_storage = 0;
	MUSHORT entry_id=0;
    McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_DelEntry nId=%d,nStorage=%d",
                nId,nStorage);
    if(!McfPhb_McareToMocroStorage(nStorage,&group_id,&mocro_storage))
    {
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MFALSE");
        return MPHB_ERROR;
    }
    if(!McfPhb_McareToMocroId(nId,&entry_id))
    {
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroId return MFASLE");
        return MPHB_ERROR;
    }
    eDual_sys = MMIPB_GetDualSysFromGroupId(group_id);
	if(MMIPB_DeletePhonebookEntry(eDual_sys, mocro_storage, entry_id))
	{
        g_mmipb_entry_t.entry_id = entry_id;
        g_mmipb_entry_t.group_id = group_id;
		if(MMIPB_Erase(entry_id, group_id))
		{
            McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" MMIPB_Erase return MFALSE");
			result = MPHB_SUCCESS;
		}
	}	
	McfPhb_SetDelCallBack(fnDelCb,pUser,nStorage,nId,result);

	McfTm_StartTimer(MNULL, 50, McfPhb_DelCallBack);
	return MPHB_WAIT;//此处返回的是调用成功，不代表立即add完毕
}
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_QueryProperty
//	Descrption:获取相关的电话本中的属性
//	Param:
//      IN   pProp:保存获取到的属性值
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_QueryProperty(McfPhbProp* pProp)
{
    if(MNULL != pProp)
    {
        MUINT mask = 0;
        mask |= MPHB_FILED_NAME|MPHB_FILED_NUMBER|MPHB_FILED_HOMENUM;
        if(OFFICE_NUMBER <MMIPB_MAX_NV_PHONE_NUM)
        {
            mask |= MPHB_FILED_OFFICENUM;
        }
        if(FAX_NUMBER <MMIPB_MAX_NV_PHONE_NUM)
        {
            mask |= MPHB_FILED_FAXNUM;
        }
        mask |= MPHB_FILED_EMAIL;

#ifndef PB_SUPPORT_LOW_MEMORY
        mask |= MPHB_FILED_COMPANYNAME;
        mask |= MPHB_FILED_NOTE;
        mask |= MPHB_FILED_TITLE;
#endif
        
        pProp->mask = mask;
        pProp->maxNameLen = MPHB_NAME_LENGTH;
        if(MMIPB_NAME_MAX_LEN < MPHB_NAME_LENGTH)
        {
            pProp->maxNameLen = MMIPB_NAME_MAX_LEN;
        }
        pProp->maxNumLen = (MMIPB_BCD_NUMBER_MAX_LEN<<1);
        pProp->reserved1 = 0;
        pProp->reserved2 = 0;
    }
    return MPHB_SUCCESS;
}

MINT McfIF_Misc_GetCharPinyin(MUSHORT ucs2Char, MCHAR* pPinyin, MINT nSize)
{
	MCHAR tempPinyin[128] = {0}; 
	MWCHAR str[2] = {0};

	MWCHAR wstr_pinyin[128] = {0};	
	MMIPB_NAME_T s_name = {0};
	
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfIF_Misc_GetCharPinyin ucs2Char = %d, pPinyin = 0x%x, %s, nSize = %d", ucs2Char, pPinyin, pPinyin, nSize);

	if(nSize <= 1)
	{
		return 0;
	}
	
	str[0] = ucs2Char;
	s_name.wstr_len = McfStdC_wcslen((const MWCHAR *)str);
	McfStdC_wcsncpy(s_name.wstr, (const MWCHAR *)str, (MINT)s_name.wstr_len);
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
#ifdef PB_SEARCH_SURPPORT_CHAR
	{
		MMIPB_NAME_LETTERS_T letters = {0};
		MMIPB_NAME_LETTERS_T number = {0};
		MMIPB_GetNameLetters(&s_name,&letters);
		McfStdC_wcsncpy(wstr_pinyin, letters.full_letters, letters.full_letters_len);
	}
#else//#ifdef PB_SEARCH_SURPPORT_CHAR
	//此处需要客户的接口支持获取汉字的拼音
#endif//#ifdef PB_SEARCH_SURPPORT_CHAR
#else//#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
	{
		MMIPB_NAME_LETTERS_T letters = {0};
		MMIPB_NAME_LETTERS_T number = {0};
		MMIPB_GetNameLetters(&s_name,&letters,&number);
		McfStdC_wcsncpy(wstr_pinyin, letters.full_letters, letters.full_letters_len);
	}
#endif//#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)

	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfIF_Misc_GetCharPinyin McfStdC_wcslen(wstr_pinyin) = %d", McfStdC_wcslen(wstr_pinyin));
	if(0 >= McfStdC_wcslen(wstr_pinyin))
	{
		pPinyin[0] = '\0';
		return 0;
	}
	
	McfStdC_wstrtostr(tempPinyin,wstr_pinyin);

	if(McfStdC_strlen(tempPinyin) >= nSize - 1)
	{
		McfStdC_strncpy(pPinyin, tempPinyin, nSize-1);
		pPinyin[nSize-1] = '\0';
		MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfIF_Misc_GetCharPinyin 11 pPinyin = 0x%x, %s", pPinyin, pPinyin);
		return (nSize - 1);
	}
	else
	{
		McfStdC_strncpy(pPinyin, tempPinyin, McfStdC_strlen(tempPinyin));
		pPinyin[McfStdC_strlen(tempPinyin)] = '\0';
		MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfIF_Misc_GetCharPinyin 22 pPinyin = 0x%x, %s", pPinyin, pPinyin);
		return McfStdC_strlen(tempPinyin);
	}
}

MBOOL MAPI_MMIPB_GetPBIsSorting(MVOID)
{
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"MAPI_MMIPB_GetPBIsSorting %ds", MCARE_IsPBSorted());
    return !MCARE_IsPBSorted();
}

MUSHORT MAPI_MMIPB_GetEntryCountInNv(MVOID)
{
    return MMIPB_GetEntryCountInNv();
}

MUSHORT Mcfphb_GetMaxRecordNumByStorage(MINT storge_id)
{
    MBYTE group_id=0;
    if(!McfPhb_McareToMocroStorage(storge_id,&group_id,MNULL))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MFASLE");
        return 0;
	}
    return MCARE_GetEntryMaxCountInGroup(group_id);

}
MUSHORT Mcfphb_GetUsedRecordNumByStorage(MINT storge_id)
{
    MBYTE group_id=0;
    if(!McfPhb_McareToMocroStorage(storge_id,&group_id,MNULL))
	{
        McfLog_Print(MLOG_INFO, MLOG_SDKPHONEBOOK,   __MCR_FUNCTION__" McfPhb_McareToMocroStorage return MFASLE");
        return 0;
	}

    return MCARE_GetEntryCountInGroup(group_id);
}

#endif//#ifdef SPREAD_NEW_PB_UPDATE

#ifdef SPREAD_NEW_AUDIO_UPDATE

MINT McfIF_Aud_GetDuration(MUINT* pDuration, MINT nType, MVOID* pSource, MINT nFormat, MINT nLen)
{
	MMISRVAUD_CONTENT_INFO_T   info = {0};
	MUINT time_lenth = 0;
	MMISRVAUD_RING_FMT_E  ring_type = MMISRVAUD_RING_FMT_NONE;
	switch(nFormat)
	{
	case MAUD_MIDI:
		ring_type = MMISRVAUD_RING_FMT_MIDI;
		break;
	case MAUD_WAV:
		ring_type = MMISRVAUD_RING_FMT_WAVE;
		break;
	case MAUD_AMR:
#ifdef SPREAD_RECORD_MP3_TYPE
		ring_type = MMISRVAUD_RING_FMT_MP3;//MMISRVAUD_RING_FMT_AMR;
#else
		ring_type = MMISRVAUD_RING_FMT_AMR;
#endif
		break;
	default:
		break;
	}
	
	if(1 == nType)//文件
	{
		if(MMISRVAUD_GetFileContentInfo((MWCHAR*)pSource, McfStdC_wcslen((MWCHAR*)pSource), &info))
		{
			time_lenth = info.total_time;
		}
	}
	else if(0 == nType) //buf
	{
		if(MMISRVAUD_GetBuffContentInfo((MBYTE*)pSource, nLen, ring_type, &info))
		{
			time_lenth = info.total_time;
		}
	}

	*pDuration = time_lenth;
	return time_lenth;
}

MVOID MAPI_MNPHONE_GetPlmnInfoEx(MN_DUAL_SYS_E dual_sys, MSG_CAMP_STATUS_T* plmn_info_ptr)
{
	MNPHONE_GetPlmnInfoEx(dual_sys, plmn_info_ptr);
}

MBOOL McfIF_Aud_StartRecord(MVOID* pUser, McfAudRecord* pRecord, pfnAudFunc fnAudRecd, MUINT* record_hanlde)
{
	MBOOL result = MFALSE;
#ifdef MMI_RECORD_SUPPORT    
    MMISRV_HANDLE_T audio_handle = 0;
	MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MCFAPI_PB_PRINT(ALOG_INFO,MLOG_SDKPHONEBOOK, __MCR_FUNCTION__"McfAud_StartRecord pUser = 0x%x, pRecord = 0x%x,fnAudRecd =0x%x, audio_handle = %d",pUser,pRecord,fnAudRecd, audio_handle);

	if(!McfRecord_CheckPermit())
	{
		return MAUD_FAIL;
	}  
	
    
    req.notify = McfRecord_callback;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;
	audio_srv.info.record_file.type = MMISRVAUD_TYPE_RECORD_FILE;    
 #ifdef SPREAD_RECORD_MP3_TYPE
	audio_srv.info.record_file.fmt= MMISRVAUD_RECORD_FMT_MP3;//MMISRVAUD_RECORD_FMT_AMR;
#else
	audio_srv.info.record_file.fmt = MMISRVAUD_RECORD_FMT_AMR;
#endif     
    audio_srv.info.record_file.name = pRecord->pFilename;
    audio_srv.info.record_file.name_len = MMIAPICOM_Wstrlen(pRecord->pFilename); 
    audio_srv.info.record_file.source   = MMISRVAUD_RECORD_SOURCE_NORMAL;
    audio_srv.info.record_file.frame_len= MMISRVAUD_RECORD_FRAME_LEN_DEFAULT;
    audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
 

    audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);	

	if(PNULL != audio_handle)
	{
		result = TRUE;
		*record_hanlde = audio_handle;
	}

	MCFAPI_PB_PRINT(ALOG_INFO,MLOG_SDKPHONEBOOK, __MCR_FUNCTION__"McfAud_StartRecord result = %d, audio_handle = %d", result, audio_handle);
	if(result)
	{
		if(MMISRVAUD_RET_OK != MMISRVAUD_Play(audio_handle, 0))
		{
			MCFAPI_PB_PRINT(ALOG_INFO,MLOG_SDKPHONEBOOK, __MCR_FUNCTION__"McfAud_StartRecord audioMMISRVAUD_Stop");
			//MMISRVAUD_Stop(audio_handle);
			MMISRVMGR_Free(audio_handle);        
			result = MFALSE;
			MCF_SCI_PASSERT(MFALSE, ("McfAud_StartRecord MMISRVAUD_Play failed"));
		}
	}
    #endif
	return result;
}


MINT McfIF_Aud_StopRecord(MINT nHd)
{
    MCFAPI_PB_PRINT(ALOG_INFO,MLOG_SDKPHONEBOOK, __MCR_FUNCTION__"McfIF_Aud_StopRecord nHd = %d, McfRecord_GetRcordStatue() = %d",nHd, McfRecord_GetRcordStatue());
	if(McfRecord_GetRcordStatue())
	{
		MCFAPI_PB_PRINT(ALOG_INFO,MLOG_SDKPHONEBOOK, __MCR_FUNCTION__"McfIF_Aud_StopRecord MMISRVAUD_Stop");
		MMISRVAUD_Stop(nHd);
		MMISRVMGR_Free(nHd);    
	}
	return MAUD_SUCCESS; 
}

MVOID McfIF_Aud_TimerCallBack(MINT nHd)
{
	MCFAPI_PB_PRINT(ALOG_INFO,MLOG_SDKPHONEBOOK, __MCR_FUNCTION__"McfIF_Aud_TimerCallBack nHd = %d, McfRecord_GetRcordStatue() = %d", nHd, McfRecord_GetRcordStatue());
	if(McfRecord_GetRcordStatue())
	{
		if(nHd > 0)
		{
			MCFAPI_PB_PRINT(ALOG_INFO,MLOG_SDKPHONEBOOK, __MCR_FUNCTION__"McfIF_Aud_TimerCallBack MMISRVAUD_Stop");
			MMISRVAUD_Stop(nHd);
			MMISRVMGR_Free(nHd);        
		}
	}
}

MBOOL McfIF_Record_callback(MUINT mocor_hanlde, MVOID* pParam)
{
	MMISRVMGR_NOTIFY_PARAM_T* param = (MMISRVMGR_NOTIFY_PARAM_T*)pParam;
    MMISRVAUD_REPORT_T *report_ptr = MNULL;
	MBOOL result = MFALSE;
	MCFAPI_PB_PRINT(ALOG_INFO,MLOG_SDKPHONEBOOK, __MCR_FUNCTION__"McfIF_Record_callback mocor_hanlde = %d, pParam = 0x%x", mocor_hanlde, pParam);
    if(param != MNULL && mocor_hanlde > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;        
        if(report_ptr != MNULL)
        {        
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
                MCFAPI_PB_PRINT(ALOG_INFO,MLOG_SDKPHONEBOOK, __MCR_FUNCTION__"McfIF_Record_callback MMISRVAUD_REPORT_END");
                MMISRVAUD_Stop(mocor_hanlde);
                MMISRVMGR_Free(mocor_hanlde);
                break;
            default:
                break;
            }        
        }
        result = MTRUE;
    }
    else
    {
        result = MFALSE;
    }
	MCFAPI_PB_PRINT(ALOG_INFO,MLOG_SDKPHONEBOOK, __MCR_FUNCTION__"McfIF_Record_callback result = %d", result);
	return result;
}
#else//#ifdef SPREAD_NEW_AUDIO_UPDATE

//------------------------------------------------------------------------------
//	FUNCTION: McfPb_IsRcording
//	Descrption:判断是否在录音
//	Param:

//	Return: 
//      MTRUE : 
//      MFALSE
//  Note:
//------------------------------------------------------------------------------

MBOOL McfPb_IsRcording(MVOID)
{
    return McfRecord_GetRcordStatue();
}

MINT McfRecord_ChangeRet(MMIAUDIO_RESULT result)
{
	MINT ret = MAUD_SUCCESS;
	switch(result)
	{
	case MMIAUDIO_RLT_SUCCESS:
		ret = MAUD_SUCCESS;
		break;
	case MMIAUDIO_RLT_ERROR:
		ret = MAUD_FAIL;
		break;
	case MMIAUDIO_RLT_NOT_SUPPORT:
		ret = MAUD_NO_PERMISSION;
		break;
	default :
		ret = MAUD_FAIL;
		break;		
	}
	return ret;
}

MBOOL MAPI_MMIAUDIO_StopRecord(MVOID)
{
	return MMIAUDIO_StopRecord();
}

MBOOL MAPI_MMIAUDIO_EndRecord(MVOID)
{
	return MMIAUDIO_EndRecord();
}

MVOID MAPI_MMIAUDIO_ResumeBgPlay(MVOID)
{
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
    MMIAUDIO_ResumeBgPlay(MMIBGPLAY_TYPE_ALL, MMIBGPLAY_MODULE_RECORDER);
#else
    MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_RECORDER);
#endif
}

MVOID MAPI_MMIAPISET_ClearCurRingType(MVOID)
{
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
    MMIAPISET_ClearCurRingType(MMISET_RING_TYPE_OTHER);
#else
	MMIAPISET_ClearCurRingType(MMISET_RING_TYPE_RECORDER_RECORD);
#endif
}

MVOID MAPI_MMIAPISET_InitAudioDevice(MVOID)
{
	MMIAPISET_InitAudioDevice(AUD_MAX_SPEAKER_VOLUME);
}

MVOID MAPI_MMIAUDIO_PauseBgPlay(MVOID)
{
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
    MMIAUDIO_PauseBgPlay(MMIBGPLAY_TYPE_ALL, MMIBGPLAY_MODULE_RECORDER);	
#else
    MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_RECORDER);	
#endif
}

MVOID MAPI_MMIAPISET_SetCurRingType(MVOID)
{
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
    MMIAPISET_SetCurRingType(MMISET_RING_TYPE_OTHER);
#else
    MMIAPISET_SetCurRingType(MMISET_RING_TYPE_RECORDER_RECORD);
#endif
}
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
#else
MBOOL MAPI_MMIAUDIO_StartRecord(
                    const wchar     *name_ptr,
                    MMIFILE_HANDLE  file_handle,    //file handle created, can be null
                    MMIAUDIO_RECORD_FORMAT_T format,
                    MMIAUDIO_RECORD_FRAME_LENGTH_T frame_length,
                    MMIAUDIO_RECORD_TYPE_E      record_type,
                    MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                    )
{
	return MMIAUDIO_StartRecord(name_ptr, file_handle, format, frame_length, record_type, end_callback_func);
}

MMIAUDIO_RECORD_FORMAT_T McfRecord_GetFormatAMR(MVOID)
{
	return MMIAUDIO_RECORD_FORMAT_AMR;
}

MMIAUDIO_RECORD_FRAME_LENGTH_T McfRecord_GetFrameLenthAMR(MVOID)
{
	return MMIAUDIO_RECORD_DEFAULT_FRAME_LEN;
}

MMIAUDIO_RECORD_TYPE_E McfRecord_GetRecordTypeAPP(MVOID)
{
	return MMIAUDIO_RECORD_NORMAL_APP;
}
#endif
MINT McfIF_Aud_GetDuration(MUINT* pDuration, MINT nType, MVOID* pSource, MINT nFormat, MINT nLen)
{
	AUDIO_FORMATINFO_T  file_info = {0};
	MUINT time_lenth = 0;
	MMIAUDIO_TYPE_E ring_type = MMIAUDIO_TYPE_MAX;
	switch(nFormat)
	{
	case MAUD_MIDI:
		ring_type = MMIAUDIO_MIDI;
		break;
	case MAUD_WAV:
		ring_type = MMIAUDIO_WAVE;
		break;
	case MAUD_AMR:
		ring_type = MMIAUDIO_AMR;
		break;
	default:
		break;
	}
	
	if(1 == nType)//文件
	{
		MMIAUDIO_CreateAudioFileHandle(MFALSE, MMIAUDIO_APP, ring_type, (MWCHAR*)pSource);
		MMIAUDIO_AudioGetFileInfo(MMIAUDIO_APP, &file_info);
		time_lenth = file_info.uiTotalTime;
		MMIAUDIO_CloseAudioHandle(MMIAUDIO_APP);
	}
	else if(0 == nType) //buf
	{
		MMIAUDIO_CreateAudioBufHandle(MFALSE,MMIAUDIO_APP,ring_type,pSource,nLen);
		MMIAUDIO_AudioGetFileInfo(MMIAUDIO_APP, &file_info);
		time_lenth = file_info.uiTotalTime;
		MMIAUDIO_CloseAudioHandle(MMIAUDIO_APP);
	}

	*pDuration = time_lenth;
	return time_lenth;
}

MVOID MAPI_MNPHONE_GetPlmnInfoEx(MN_DUAL_SYS_E dual_sys, MSG_CAMP_STATUS_T* plmn_info_ptr)
{
	MNPHONE_GetPlmnInfoEx(dual_sys, plmn_info_ptr);
}

#endif//#ifdef SPREAD_NEW_AUDIO_UPDATE

//=======================McfPhonebook.c API End==========================


//======================McfScreen.c API BEGIN============================
#ifdef MCARE_BACKLIGHT_OPEN_FOREVER
static MMISET_BACKLIGHT_SET_T s_backlight_set = {0};
#endif
//------------------------------------------------------------------------------
//	FUNCTION: HandleNewScreenMsg
//	Descrption:窗口消息处理函数
//	Param:
//      IN    win_id:窗口ID  
//      IN    msg_id:传递的消息
//      IN    param:传递的参数   
//	Return: 
//      MMI_RESULT_TRUE : success
//      MMI_RESULT_FALSE : fail
//  Note:
//------------------------------------------------------------------------------
static MMI_RESULT_E HandleNewScreenMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
static MVOID McfScrn_SetBacklight(MVOID);
static MVOID McfScrn_ReSetBacklight(MVOID);
#if !(defined SPREAD_PLAT_SC6600L_1025) && !(defined SPREAD_PLAT_SC6600L_0937) && !(defined SPREAD_PLAT_SC6620)
static MVOID McfScrn_SetWinAngle(MUINT win_id);
#endif
static MVOID Set3DStyleOther(MVOID);
static MVOID Reset3DStyleOther(MVOID);


#if !(defined SPREAD_PLAT_SC6600L_1025) && !(defined SPREAD_PLAT_SC6600L_0937) && !(defined SPREAD_PLAT_SC6620)
static MVOID McfScrn_SetWinAngle(MUINT win_id)
{
	LCD_ANGLE_E  rotate_angle = LCD_ANGLE_0;
	LCD_INFO_T lcd_info = {0};
	GUI_LCD_DEV_INFO dev = McfCmn_GetDevInfo();
	GUILCD_GetInfo(dev.lcd_id, &lcd_info);
	rotate_angle=GUILCD_GetRotateAngle(dev.lcd_id);
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"[MCFSCREEN]zhukey HandleNewScreenMsg msg_id = MCARE_MSG_OPEN_WINDOW rotate_angle = %d", rotate_angle);
	switch (rotate_angle)
	{
	case LCD_ANGLE_0:
        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
        MMK_SetWinAngle(win_id, WIN_SUPPORT_ANGLE_0, MFALSE);
		break;
	case LCD_ANGLE_90:
        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_90);
        MMK_SetWinAngle(win_id, WIN_SUPPORT_ANGLE_90, MFALSE);
		break;
	case LCD_ANGLE_180:
        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_180);
        MMK_SetWinAngle(win_id, WIN_SUPPORT_ANGLE_180, MFALSE);
		break;
	case LCD_ANGLE_270:
        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_270);
        MMK_SetWinAngle(win_id, WIN_SUPPORT_ANGLE_270, MFALSE);
		break;
	default:
        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
        MMK_SetWinAngle(win_id, LCD_ANGLE_0, MFALSE);        
		break;
	}
}
#endif

static MVOID Set3DStyleOther(MVOID)
{
	if(McfIF_IsMcareSupport3D())
	{
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
		s_gsensor_switch_style = MMIAPISET_GetGsensorSwitchStyle();
		MMISET_SetGsensorSwitchStyle(MMISET_G_SENSOR_SWITCH_NONE);
		s_gsensor_Screen_Style = MMISET_Get3DStyleScreen();
		MCAREV31_Set3DStyleScreen(MMISET_3D_STYLE_SCREEN_NONE);
#endif
	}
	else
	{
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
		s_gsensor_switch_style = MMIAPISET_GetGsensorSwitchStyle();
		MMISET_SetGsensorSwitchStyle(MMISET_G_SENSOR_SWITCH_NONE);
		s_gsensor_open_flag = MMISET_GetIsSensorOpenFlag();
		MMISET_SetIsSensorOpenFlag(0);
		s_gsensor_Screen_Style = MMISET_Get3DStyleScreen();
		MCAREV31_Set3DStyleScreen(MMISET_3D_STYLE_SCREEN_NONE);
#else
		McfScrn_SetEnable3DMMI(MFALSE);
#endif
	}
}

static MVOID Reset3DStyleOther(MVOID)
{
	if(McfIF_IsMcareSupport3D())
	{
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
		MMISET_SetGsensorSwitchStyle(s_gsensor_switch_style);
		s_gsensor_switch_style = MMISET_G_SENSOR_SWITCH_DEFAULT;
		MCAREV31_Set3DStyleScreen(s_gsensor_Screen_Style);
		s_gsensor_Screen_Style = 0;
#endif
	}
	else
	{
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
		MMISET_SetGsensorSwitchStyle(s_gsensor_switch_style);
		s_gsensor_switch_style = MMISET_G_SENSOR_SWITCH_DEFAULT;
		MMISET_SetIsSensorOpenFlag(s_gsensor_open_flag);
		s_gsensor_open_flag = 0;
		MCAREV31_Set3DStyleScreen(s_gsensor_Screen_Style);
		s_gsensor_Screen_Style = 0;
#else
		McfScrn_SetEnable3DMMI(MTRUE);
#endif
	}
}


//------------------------------------------------------------------------------
//	FUNCTION: HandleNewScreenMsg
//	Descrption:窗口消息处理函数
//	Param:
//      IN    win_id:窗口ID  
//      IN    msg_id:传递的消息
//      IN    param:传递的参数   
//	Return: 
//      MMI_RESULT_TRUE : success
//      MMI_RESULT_FALSE : fail
//  Note:
//------------------------------------------------------------------------------
static MMI_RESULT_E HandleNewScreenMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E   result   = McfIF_GetMmiResult(MTRUE);

	MCFAPI_INTERFACE_TRACE("[MCARE] zhukey HandleNewScreenMsg msg_id = 0x%x, win_id = %d", msg_id, win_id);

	msg_id = McfCmn_GetWindowMsg(msg_id);
	
	switch (msg_id)
	{
	case MCARE_MSG_OPEN_WINDOW:
		{
			MCFAPI_INTERFACE_TRACE("[MCFSCREEN]zhukey HandleNewScreenMsg msg_id = MCARE_MSG_OPEN_WINDOW");
			if(!McfIF_IsMcareSupport3D())
			{
#if !(defined SPREAD_PLAT_SC6600L_1025) && !(defined SPREAD_PLAT_SC6600L_0937) && !(defined SPREAD_PLAT_SC6620)
				//McfScrn_SetWinAngle((MUINT)win_id);
#ifndef MAINLCD_DEV_SIZE_320X240            				
//===================如只支持竖屏显示，角度为0 begin=================
		        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
		        MMK_SetWinAngle(win_id, LCD_ANGLE_0, MFALSE);
//===================如只支持竖屏显示，角度为0 end==================
#else
//===================如只支持横屏显示，角度为90 begin================
		        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_90);
		        MMK_SetWinAngle(win_id, LCD_ANGLE_90, MFALSE);
//===================如只支持横屏显示，角度为90 end=================
#endif

#endif
			}
			McfScrn_SetBacklight();
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)
			Set3DStyleOther();
#endif
			if(McfIF_IsMcareSupport3D())
			{
				MAPI_MMK_OpenGsensor(win_id);
			}
			McareV31_EntryMcareWinCallBack();
			MCareV31_RunApp();
			McfIF_CloseActiveGPRSScreen();
		}
		break;
		//删除对这个消息的处理是因为QQ消息来了后，会有闪烁
		//闪烁的时候把屏点亮的时候，会导致应用状态为挂起的时候，屏亮着
		//结果就是按键不会响应。产生所谓的定屏现象
#if 0
        case MCARE_MSG_BACKLIGHT_ON:
#endif
	case MCARE_MSG_GET_FOCUS:
		MCFAPI_INTERFACE_TRACE("[MCFSCREEN]zhukey HandleNewScreenMsg msg_id = MCARE_MSG_GET_FOCUS");
	    {
	        McfScreenFun *  entry_func = MNULL;
            McfScrn_SetBacklight();
	        entry_func = McfScrn_GetCurrEntryFunc();
    		if(entry_func->m_CB)
    		{
    			//McfIF_ShowEnterMcareTip();
    			(*entry_func->m_CB)();
    		}
	    }
		break;
		//删除对这个消息的处理是因为QQ消息来了后，会有闪烁
		//闪烁的时候把屏点亮的时候，会导致应用状态为挂起的时候，屏亮着
		//结果就是按键不会响应。产生所谓的定屏现象		
#if 0
	case MCARE_MSG_BACKLIGHT_OFF:
#endif
	case MCARE_MSG_LOSE_FOCUS:
	    {
            McfScreenFun *  exit_func = MNULL;
            exit_func = McfScrn_GetCurrExitFunc();
            if(exit_func->m_CB)
            {
                (*exit_func->m_CB)();
            }
            if(McfIF_GetIsMcareLayerExit())
            {
                McfIF_ReleaseMcareLayer();
            }
            McfScrn_ReSetBacklight();
	    }
		break;
		
	case MCARE_MSG_CLOSE_WINDOW:
	    {
            McfScreenDelFun *  del_func = MNULL;
            del_func = McfScrn_GetDelScrnCallback(McfScrn_GetCurScreenID());
            if(del_func->m_DelCB)
            {
                (*del_func->m_DelCB)(del_func->m_pUser);
            }
            McfIF_ReleaseMcareLayer();
            McareV31_ExitMcareWinCallBack();
            if(McfIF_IsMcareSupport3D())
            {
                MAPI_MMK_CloseGsensor(win_id);
            }
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)
            Reset3DStyleOther();
#endif
            McfScrn_ReSetBacklight();
	    }
		break;
	case MCARE_MSG_GSENSOR:
		if(McfIF_IsMcareSupport3D())
		{
			if(MMK_IsFocusWin(win_id))
			{
				extern MVOID MCR_RotateTencentMCare(MINT nAngle);
				MCFAPI_INTERFACE_TRACE("[MCFSCREEN]zhukey HandleNewScreenMsg msg_id = MCARE_MSG_GSENSOR");
				McfGdi_Reinit();
				MCR_RotateTencentMCare(McfIF_GetCurAngle());
			}
		}
		else
		{
			MCFAPI_INTERFACE_TRACE("[MCFSCREEN]zhukey HandleNewScreenMsg msg_id = MCARE_MSG_GSENSOR default");
			result = McfIF_GetMmiResult(MFALSE);
		}
		break;
	default:
		if(McfCmn_IsTPMsg(msg_id))
		{
			MINT pos_x = 0;
			MINT pos_y = 0;
			pos_x = MMK_GET_TP_X(param);
			pos_y = MMK_GET_TP_Y(param);
			MCFAPI_INTERFACE_TRACE("[MCFSCREEN]zhukey HandleNewScreenMsg msg_id = McfCmn_IsTPMsg timer_time = %d",McfTime_GetSysTimeMs());
			McfPen_CallHandlePen(msg_id, pos_x, pos_y);
		}
		else if(McfCmn_IsKeyMsg(msg_id))
		{
			MCFAPI_INTERFACE_TRACE("[MCFSCREEN]zhukey HandleNewScreenMsg msg_id = McfCmn_IsKeyMsg timer_time = %d",McfTime_GetSysTimeMs());
			McfKey_CallHandleKey((MUINT)msg_id);
		}
		else
		{
			MCFAPI_INTERFACE_TRACE("[MCFSCREEN]zhukey HandleNewScreenMsg msg_id = default");
			result = McfIF_GetMmiResult(MFALSE);
		}
		break;
	}
	return result;
}
static MVOID McfScrn_SetBacklight(MVOID)
{
#ifdef MCARE_BACKLIGHT_OPEN_FOREVER
    MMISET_BACKLIGHT_SET_T backlight_info ={0,MMISET_BACKLIGHT_OPEN_FOREVER};
    MMIAPISET_GetBackLightInfo(&s_backlight_set);
    MMISET_SetBackLightInfo(backlight_info);
    //MMIDEFAULT_TurnOnLCDBackLight();
	MMIDEFAULT_TurnOnBackLight();
#endif
}
static MVOID McfScrn_ReSetBacklight(MVOID)
{
#ifdef MCARE_BACKLIGHT_OPEN_FOREVER
    MMISET_SetBackLightInfo(s_backlight_set);
    //MMIDEFAULT_TurnOnLCDBackLight();
	MMIDEFAULT_TurnOnBackLight();
#endif
}
MBOOL MAPI_MMK_OpenGsensor(MMI_HANDLE_T handle)
{
#if (defined SPREAD_PLAT_SC6800H) ||(defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
	return MMK_OpenGsensor(handle);
#endif
}

MBOOL MAPI_MMK_CloseGsensor(MMI_HANDLE_T handle)
{
#if (defined SPREAD_PLAT_SC6800H) ||(defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
	return MMK_CloseGsensor(handle);
#endif
}


MBOOL MAPI_MMK_IsOpenWin(MMI_HANDLE_T win_handle)
{
	return MMK_IsOpenWin(win_handle);
}

MBOOL MAPI_MMK_CloseWin(MMI_HANDLE_T win_handle)
{
	return MMK_CloseWin(win_handle);
}

MBOOL MAPI_MMK_IsFocusWin(MMI_HANDLE_T win_handle)
{
	return MMK_IsFocusWin(win_handle);
}

MBOOL MAPI_MMK_ReturnIdleWin(MVOID)
{
	return MMK_ReturnIdleWin();
}

MBOOL  MAPI_MMK_SendMsg(MMI_HANDLE_T  handle,MMI_MESSAGE_ID_E msg_id,DPARAM param_ptr)
{
	return  MMK_SendMsg(handle,msg_id,param_ptr);
}


MVOID MAPI_MMIPUB_OpenAlertWinByTextPtr(
										MUINT* 					time_period_ptr,   //time period,default is 3s
										MMI_STRING_T*				text1_ptr,         //text 1
										MMI_STRING_T*				text2_ptr,         //text 2
										MMI_IMAGE_ID_T      			image_id,           //alert image id
										MMI_WIN_ID_T*				win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
										MMI_WIN_PRIORITY_E*		win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
										MMIPUB_SOFTKEY_STYLE_E    	softkey_style,      //softkey style
										MMIPUB_HANDLE_FUNC        	handle_func         //handle function
										)
{
	MMIPUB_OpenAlertWinByTextPtr(time_period_ptr,text1_ptr,text2_ptr,image_id,win_id_ptr,win_priority_ptr,softkey_style,handle_func);
}


MBOOL McfCmn_IsTouchScreen(MVOID)
{
#ifdef TOUCH_PANEL_SUPPORT
	return MTRUE;
#else
	return MFALSE;
#endif
}

MUINT McfCmn_GetWindowMsg(MUINT msg_id)
{
    MUINT mcare_msg = msg_id;
    switch(msg_id)
    {
        case MSG_GET_FOCUS:
            mcare_msg= MCARE_MSG_GET_FOCUS;
            break;
        case MSG_LOSE_FOCUS:
            mcare_msg= MCARE_MSG_LOSE_FOCUS;
            break;
        case MSG_BACKLIGHT_TURN_ON:
            mcare_msg= MCARE_MSG_BACKLIGHT_ON;
            break;
        case MSG_BACKLIGHT_TURN_OFF:
            mcare_msg= MCARE_MSG_BACKLIGHT_OFF;
            break;
        case MSG_OPEN_WINDOW:
            mcare_msg= MCARE_MSG_OPEN_WINDOW;
            break;
        case MSG_CLOSE_WINDOW:
            mcare_msg= MCARE_MSG_CLOSE_WINDOW;
            break;
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
        case MSG_GSENSOR:
            mcare_msg= MCARE_MSG_GSENSOR;
            break;
#endif
    }
    return mcare_msg;
}

#ifdef MCARE_3D_SUPPORT
MBOOL s_is_support_3D = MTRUE;
#else
MBOOL s_is_support_3D = MFALSE;
#endif
MBOOL McfIF_IsMcareSupport3D(MVOID)
{
	return s_is_support_3D;
}
MVOID McfIF_SetMcareSupport3D(MBOOL is_support)
{
	s_is_support_3D = is_support;
}

MVOID McfSrceen_Enable3DMMI(MBOOL is_enable) //nicklei add
{
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
	MMI_Enable3DMMI(is_enable);
#endif
}

MUINT McfCmn_GetWarningImgId(MVOID)
{
	return IMAGE_PUBWIN_WARNING;
}

MUINT McfIF_GetSoftkeyOne(MVOID)
{
	return MMIPUB_SOFTKEY_ONE;
}
MINT McfScreen_GetAvailScreen(MVOID)
{
	MINT scrnId = MCARE_WIN_ID_START_PLAT;
	for(scrnId = MCARE_WIN_ID_START_PLAT; scrnId < MCARE_MAX_WIN_ID; scrnId++)
	{
		if(!MMK_IsOpenWin(scrnId))
		{
			return scrnId;
		}
	}
	return 0;
}

MVOID McfIF_ShowEnterMcareTip(MVOID)
{
#define IF_DEEP_BLUE (0x2b72)
#define IF_WHITE (McfGdi_MakeMColorFromRGB(255,255,255))
	//启动中，请稍候...
	const MWCHAR wait[] = {0x542F,0x52A8,0x4E2D,0xFF0C,0x8BF7,0x7A0D,0x5019,0x002E,0x002E,0x002E,0x0000};
	MINT nScrHeight = McfGdi_GetScrHeight();
	MINT nScrWidth = McfGdi_GetScrWidth();
	
	McfGdi_Init();
	McfGdi_SetFont(MGDI_FONTSIZE_MEDIUM,0);
	
	McfGdi_BeginDraw();
	McfGdi_FillRect(0, 0, nScrWidth, nScrHeight, IF_DEEP_BLUE);
	McfGdi_SetTextColor_Int(IF_WHITE);
	McfGdi_DrawText(nScrWidth/2 - McfGdi_GetTextWidth(wait)/2, nScrHeight/2-McfGdi_GetCharHeight()/2, wait);
	McfGdi_EndDraw();
#undef IF_WHITE
#undef IF_DEEP_BLUE
}

//---------------------------------------------------------------------------
//	FUNCTION: EntryNewScreen
//	Descrption:进入一个新窗口
//	Param:
//      IN    win_id:窗口ID  
//      IN    exitFunc:退出窗口的回调函数
//      IN    entryFunc:进入窗口的回调函数
//	Return: 
//      VOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScreen_EntryNewScreen(MINT win_id, pfnFunc exitFunc, pfnFunc entryFunc,MVOID* flag)
{
	if(!MMK_IsOpenWin(win_id))
	{
		MMI_WINDOW_CREATE_T win_create = {0};
		
		win_create.applet_handle = MMK_GetFirstAppletHandle();
		win_create.win_id = win_id;
		win_create.func = HandleNewScreenMsg;
		win_create.win_priority = WIN_ONE_LEVEL;
		win_create.window_move_style = MOVE_FORBIDDEN;
#ifdef SPREAD_PLAT_SC8800G_PDA
        win_create.window_style = WS_NO_DEFAULT_STYLE ;
#endif
		
		McfScrn_SetCurrEntryFunc(entryFunc);
		McfScrn_SetCurrExitFunc(exitFunc);
		MMK_CreateWindow(&win_create );
		McfIF_ScreenDestryCtl();
		
		//McfIF_ShowEnterMcareTip();
	}
}

//======================McfScreen.c API END==========================  

//======================McfSim.c API Begin==========================  


//获取一张已经就绪的SIM卡，返回EMSimCard
MINT McfSim_GetSimReadyFirst(MVOID)
{
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfSim_GetSimReadyFirst");
	if(MMIPHONE_IsSimOk(MN_DUAL_SYS_1))
	{
        return (MINT)McfSim_GetCardID(MN_DUAL_SYS_1);		
	}
#if (defined MCR_SIM2_SUPPORT) || (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
	if(MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
	{
        return (MINT)McfSim_GetCardID(MN_DUAL_SYS_2);		
	}
#endif
#if (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
	if(MMIPHONE_IsSimOk(MN_DUAL_SYS_3))
	{
        return (MINT)McfSim_GetCardID(MN_DUAL_SYS_3);		
	}
#endif
#if (defined MCR_SIM4_SUPPORT)
	if(MMIPHONE_IsSimOk(MN_DUAL_SYS_4))
	{
        return (MINT)McfSim_GetCardID(MN_DUAL_SYS_4);		
	}
#endif
	return MSIM_NONE;
}

//******************************************************************************/
//	FUNCTION: McfIF_Sim_GetSimOperator
//	Descrption: 得到有效的sim卡的类型
//	Param:  
//	IN:         simId:SIM卡ID
//	OUT:        
//	Return:     运营商的ID
//  Note: 
/******************************************************************************/
MINT McfIF_Sim_GetSimOperator(MINT select_card)
{
	PHONE_PLMN_NAME_E PLMN_name = PLMN_CMCC; //nicklei add
	MINT operater = MSIM_OPR_UNKOWN;
	MN_DUAL_SYS_E dual_sys = McfSim_GetSimID(select_card);
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfIF_Sim_GetSimOperator select_card = %d", select_card);

	if(MSIM_VWIFI == dual_sys)
	{//WIFI特殊处理
		return MSIM_OPR_WIFI;
	}

	if(MN_DUAL_SYS_MAX <= dual_sys)
	{
		return MSIM_OPR_UNKOWN;
	}

	if(!MMIPHONE_IsSimOk(dual_sys))
	{
		return MSIM_OPR_UNKOWN;
	}
#ifdef SPREAD_PLAT_SC6530
	PLMN_name = MMIAPIPHONE_GetNetworkName(dual_sys);
#else
	PLMN_name = MMIPHONE_GetNetworkName(dual_sys);
#endif
	
	switch(PLMN_name)
	{
	case PLMN_CMCC:
		operater = MSIM_OPR_CMCC;
		break;
	case PLMN_UNICOM:
		operater = MSIM_OPR_UNICOM;
		break;
 #ifndef SPREAD_PLAT_SC6600L_1025_PDA
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)
	case PLMN_TELCEL:
		operater = MSIM_OPR_TELCOM;
		break;
#endif
#endif
	default:
		operater = MSIM_OPR_UNKOWN;
		break;
	}
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfIF_Sim_GetSimOperator operater = %d", operater);
	return operater;
}


//判断某张SIM是否支持
MBOOL McfSim_IsVerify(MINT select_card)
{
    MBOOL   isVerify = MFALSE;
	MN_DUAL_SYS_E dual_sys = McfSim_GetSimID(select_card);
	
	switch(dual_sys)
	{
        case MN_DUAL_SYS_1:
#if (defined MCR_SIM2_SUPPORT) || (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
        case MN_DUAL_SYS_2:
#endif          
#if (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
        case MN_DUAL_SYS_3:
#endif            
#if (defined MCR_SIM4_SUPPORT)
        case MN_DUAL_SYS_4:
#endif         
        isVerify = MTRUE;
            break;
        default :
            break;
    }
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfSim_IsVerify select_card = %d, dual_sys = %d, isVerify = %d", select_card, dual_sys, isVerify);
	return isVerify;
}

//判断某张SIM卡是否已经就绪
MBOOL McfSim_IsReady(MINT select_card)
{
	MN_DUAL_SYS_E dual_sys = McfSim_GetSimID(select_card);
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfSim_IsReady select_card = %d, dual_sys = %d, MMIPHONE_IsSimOk(dual_sys) = %d", select_card, dual_sys, MMIPHONE_IsSimOk(dual_sys));
#if 0	
	if(dual_sys >= MN_DUAL_SYS_MAX)
	{
		return MFALSE;
	}
#endif
	MCF_SCI_ASSERT(dual_sys < MN_DUAL_SYS_MAX); 
	return MMIPHONE_IsSimOk(dual_sys);
}


MN_DUAL_SYS_E McfSim_GetSimID(MINT simId)
{
	MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
	switch(simId)
	{
        case MSIM_CARD1:
            dual_sys = MN_DUAL_SYS_1;
            break;
        case MSIM_CARD2:
#if (defined MCR_SIM2_SUPPORT) || (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
            dual_sys = MN_DUAL_SYS_2;
#endif
            break;
        case MSIM_CARD3:
#if (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
            dual_sys = MN_DUAL_SYS_3;
#endif
            break;
        case MSIM_CARD4:
#if (defined MCR_SIM4_SUPPORT)
            dual_sys = MN_DUAL_SYS_4;
#endif
            break;

        case MSIM_VWIFI:
            dual_sys = (MN_DUAL_SYS_E) MSIM_VWIFI;
            break;

        default :
            break;
	}
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfSim_GetSimID simId = %d, dual_sys = %d", simId, dual_sys);
	return dual_sys;
}

MINT McfSim_GetCardID(MN_DUAL_SYS_E dual_sys)
{
	//MUINT i =0;
    MINT    simId =MSIM_NONE;
    switch(dual_sys)
    {
        case MN_DUAL_SYS_1:
            simId = MSIM_CARD1;
            break;
#if (defined MCR_SIM2_SUPPORT) || (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
        case MN_DUAL_SYS_2:
            simId = MSIM_CARD2;
            break;
#endif
#if (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
        case MN_DUAL_SYS_3:
            simId = MSIM_CARD3;
            break;
#endif
#if (defined MCR_SIM4_SUPPORT)
        case MN_DUAL_SYS_4:
            simId = MSIM_CARD4;
            break;
#endif

        case MSIM_VWIFI:
            simId = MSIM_VWIFI;
            break;

        default :
            break;
    
    }
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfSim_GetCardID dual_sys = %d, simId = %d", dual_sys, simId);
    return simId;
}

MINT McfIF_Sim_GetValidSim(MVOID)
{
	MINT index_ready_sim = MSIM_NONE;

	if(McfIF_IsFlyMode())
	{//修改bug
		return MSIM_NONE;
	}
	
	if(McfSim_IsReady(MSIM_CARD1))
	{
		index_ready_sim = MSIM_CARD1;
	}
#if (defined MCR_SIM2_SUPPORT) || (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
	if(McfSim_IsReady(MSIM_CARD2))
	{
		index_ready_sim = index_ready_sim | MSIM_CARD2;
	}
#endif
#if (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
	if(McfSim_IsReady(MSIM_CARD3))
	{
		index_ready_sim = index_ready_sim | MSIM_CARD3;
	}
#endif
#if (defined MCR_SIM4_SUPPORT)
	if(McfSim_IsReady(MSIM_CARD4))
	{
		index_ready_sim= index_ready_sim | MSIM_CARD4;
	}
#endif

#if (defined MCR_WIFI_SUPPORT)
	index_ready_sim= index_ready_sim | MSIM_VWIFI;
#endif
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"McfIF_Sim_GetValidSim index_ready_sim = %d", index_ready_sim);
    return index_ready_sim;
}
//======================McfSim.c API End==========================


//======================McfSms.c API Begin==========================


static MINT s_nOption = MSMS_OPT_NPOP;//add by rico 2011-12-31用来区分是否弹消息发送成功/失败框

//信息发送成功！
const MUSHORT s_sms_send_succes[] = {0x4FE1,0x606F,0x53D1,0x9001,0x6210,0x529F,0xFF01,0x0000};
//信息发送失败！
const MUSHORT s_sms_send_failed[] = {0x4FE1,0x606F,0x53D1,0x9001,0x5931,0x8D25,0xFF01,0x0000};
#ifdef SPREAD_NEW_SMS_UPDATE
MVOID McfIF_SmsCallback(MBOOL result, MVOID* pUser)
#else
MVOID McfIF_SmsCallback(MBOOL result)
#endif
{
	MINT ret = MCR_FAIL;
	if(result)
	{
		if(MSMS_OPT_POP == s_nOption)
		{
			McfGdi_DisplayWarnning(s_sms_send_succes);
		}
		ret = MCR_SUCCESS;
	}
	else
	{
		if(MSMS_OPT_POP == s_nOption)
		{
			McfGdi_DisplayWarnning(s_sms_send_failed);
		}
		ret = MCR_FAIL;
	}
	
#ifdef SPREAD_NEW_SMS_UPDATE
	McfSms_callback(ret, MNULL);
#else
	McfSms_callback(ret);
#endif
}

MBOOL McfIF_SmsGetMcareStatus(MVOID)
{
	return McfSms_GetMcareStatus();
}

MVOID McfIF_SmsSetMcareStatus(MBOOL is_mcare)
{
	McfSms_SetMcareStatus(is_mcare);
}

/*************************************
* McfIF_Sms_WriteMsg
* @pNumber --[in]destination number
* @pContent --[in]msg default content,not support now@0909,arlo
* @nSize --[in]size of msg default content,not support now
* #return none
*/
MVOID McfIF_Sms_WriteMsg(MCHAR* pNumber, MINT nNumberCnt, MWCHAR* pContent, MINT nSize)
{
    MBYTE num_len = 0;
    MMI_STRING_T  sms_init_text = {0}; 
    if(MNULL != pNumber)
    {
        num_len = MAPI_strlen(pNumber);
    }
    if (MMISMS_IsOrderOk() && MMISMS_IsMMSOrderReady())
    {
        sms_init_text.wstr_len = nSize;
        sms_init_text.wstr_ptr = pContent;
        MMISMS_WriteNewMessage(MN_DUAL_SYS_MAX, &sms_init_text, FALSE, pNumber,num_len);
    }
    else
    {
        #if 0
        MMI_STRING_T    waiting_text = {0};

        MMI_GetLabelTextByLang(TXTSYS_WAITING, &waiting_text);
        MMIPUB_OpenWaitWin(1, &waiting_text, PNULL, PNULL, MMISMS_WAITING_WIN_ID, IMAGE_NULL,
            ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, HandleWaitingWinMsg);
        #endif
    }
}

#ifdef SPREAD_NEW_SMS_UPDATE
MINT McfIF_Sms_SendSms(MVOID* pUser, MINT simId, MINT nOption, const MCHAR* pNumber, const MCHAR* pData,
					MINT dataSize, MINT encodeType, pfnMcfSmsSendCb sendSmsCb)
{
	MINT sms_len =0;
	MBOOL bresult = MFALSE;
	MINT sms_result = MFALSE;
	MWCHAR wcsms_content[MCFSMS_CONTENT_MAX] = {0};
    MMISMS_SEND_DATA_T send_data = {0};
    MMI_STRING_T    message_content = {0};

	MCFAPI_SMS_PRINT(ALOG_INFO,MLOG_SDKSMS, __MCR_FUNCTION__"[MCFSMS] McfSms_SendSms pUser = 0x%x, simId = %d, pNumber = 0x%x, pData = 0x%x,dataSize = %d,encodeType = %d",
		pUser,simId,pNumber,pData,dataSize,encodeType);
	
	McfSms_SetMcareStatus(MFALSE);
	
	if(!McfSim_IsVerify(simId))
	{
		if(MSIM_NONE != (MINT)McfSim_GetSimReadyFirst())
		{
			simId = (MINT)McfSim_GetSimReadyFirst();
		}
		else
		{
			return MCR_FAIL;//SIM卡不存在
		}
	}
	//这里存在dataSize的争议
	if(encodeType == MENCODE_UCS2)
	{
		McfStdC_wcscpy(wcsms_content, (const MWCHAR*)pData);
		//McfStdC_wcsncpy(wcsms_content, (const MWCHAR*)pData, dataSize/2);
	}
	else if(encodeType == MENCODE_ASCII)
	{
		MAPI_GUI_GB2UCS(wcsms_content, (const MCHAR*)pData, (MUSHORT)McfStdC_strlen(pData));
		//McfStdC_strtowstr(wcsms_content, (const MCHAR*)pData);
	}
	sms_len = McfStdC_wcslen(wcsms_content);

    message_content.wstr_ptr = wcsms_content;
    message_content.wstr_len = sms_len;
    send_data.sms_content_ptr = &message_content;
    send_data.dst_port = 0;
    send_data.src_port = 0;
    send_data.dest_addr_ptr = pNumber;
    send_data.dest_addr_len = (MBYTE)McfStdC_strlen(pNumber);
	//bug 需要转换mcare的simid为展讯的simid。
    send_data.dual_sys = McfSim_GetSimID(simId);
    send_data.is_need_packet = MFALSE;
    send_data.send_callback = McfIF_SmsCallback;

    bresult = MMIAPISMS_SendSmsForOther(&send_data);
	if(bresult)
	{
		McfSms_SetMcareStatus(MTRUE);
		sms_result = MTRUE;
	}
	else
	{
		McfSms_SetMcareStatus(MFALSE);
		sms_result = MFALSE;
	}
	MCFAPI_SMS_PRINT(ALOG_INFO,MLOG_SDKSMS, __MCR_FUNCTION__"[MCFSMS] McfSms_SendSms dual_sys = %d, bresult = %d,sms_result = %d",
		send_data.dual_sys, bresult,sms_result);
	
	return sms_result;
}
#else//#ifdef SPREAD_NEW_SMS_UPDATE
MVOID  MAPI_MMISMS_SetNewMsgTOSend(MVOID)
{
	MMISMS_SetNewMsgTOSend();
}

MBOOL MAPI_MMISMS_AppSendSmsForMSD(MINT simId, MWCHAR* sms_content, MINT sms_len,
								   MBYTE* dest_addr_ptr, MBYTE dest_addr_len)
{
	MMI_STRING_T sms_str = {0};
	if(MNULL == dest_addr_ptr || 0 == dest_addr_len)
	{
		return MFALSE;
	}
	sms_str.wstr_len = (MUSHORT)sms_len;
	sms_str.wstr_ptr = (MWCHAR*)sms_content;
	MMISMS_SetOperStatus(MMISMS_SEND_SMS);
	return MMISMS_AppSendSmsForMSD(McfSim_GetSimID(simId), &sms_str, dest_addr_ptr, dest_addr_len);
}
#endif//#ifdef SPREAD_NEW_SMS_UPDATE
//======================McfSms.c API End==========================


//=======================McfSocket.c API BEGIN========================= 

MBOOL McfIF_IsFlyMode(MVOID)
{
	return MMIAPISET_GetFlyMode();
}

MINT McfSoc_ErrorNo(MVOID)
{
	return EINPROGRESS;
}

MINT McfSoc_WouldBlock(MVOID)
{
	return EWOULDBLOCK;
}
//=======================McfSocket.c API End==========================

//=======================McfStdlib.c API Begin==========================
MUINT MAPI_strlen(const MCHAR* pStr)
{
	if(MNULL == pStr)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_strlen MNULL == pStr"));
	}
	return (MUINT)strlen(pStr);
}

MINT MAPI_strcmp(const MCHAR* pStr0, const MCHAR* pStr1)
{
	if(MNULL == pStr0 && MNULL == pStr1)
	{
		return 0;
	}
	else if(MNULL == pStr0 && MNULL != pStr1)
	{
		return -1;
	}
	else if(MNULL != pStr0 && MNULL == pStr1)
	{
		return 1;
	}
	return (MINT)strcmp(pStr0, pStr1);
}

MINT MAPI_stricmp(const MCHAR* pStr0,  const MCHAR* pStr1)
{
	if(MNULL == pStr0 && MNULL == pStr1)
	{
		return 0;
	}
	else if(MNULL == pStr0 && MNULL != pStr1)
	{
		return -1;
	}
	else if(MNULL != pStr0 && MNULL == pStr1)
	{
		return 1;
	}
	return (MINT)stricmp(pStr0, pStr1);
}
MCHAR* MAPI_strcat(MCHAR* pStrDest,  const MCHAR* pStrSrc)
{
	if(MNULL == pStrDest && MNULL == pStrSrc)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_strcat MNULL == pStrDest && MNULL == pStrSrc"));
	}
	else if(MNULL == pStrDest || MNULL == pStrSrc)
	{
		return pStrDest;
	}
	return (MCHAR*)strcat(pStrDest, pStrSrc);
}
MCHAR* MAPI_strcpy(MCHAR* pStrDest,  const MCHAR* pStrSrc)
{
	if(MNULL == pStrDest && MNULL == pStrSrc)
	{
		MCF_SCI_PASSERT(MFALSE,("MAPI_strcpy MNULL == pStrDest && MNULL == pStrSrc"));
	}
	else if(MNULL == pStrDest || MNULL == pStrSrc)
	{
		return pStrDest;
	}
	return (MCHAR*)strcpy(pStrDest, pStrSrc);
}
MCHAR* MAPI_strncpy(MCHAR* pStrDest, const MCHAR* pStrSrc, MUINT nSize)
{
	if(MNULL == pStrDest && MNULL == pStrSrc)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_strncpy MNULL == pStrDest && MNULL == pStrSrc"));
	}
	else if(MNULL == pStrDest ||MNULL == pStrSrc || 0 == nSize)
	{
		return pStrDest;
	}
	return (MCHAR*)strncpy(pStrDest, pStrSrc, nSize);
}

MCHAR* MAPI_strchr(const MCHAR* pStr,  MINT chr) //参数为MINT
{
	if(MNULL == pStr)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_strchr MNULL == pStr"));
	}
	return (MCHAR*)strchr(pStr, chr);
}

MCHAR* MAPI_strrchr(const MCHAR* pStr,  MINT chr) //参数为MINT
{
	if(MNULL == pStr)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_strchr MNULL == pStr"));
	}
	return (MCHAR*)strrchr(pStr, chr);
}

MCHAR* MAPI_strstr(const MCHAR* pStr, const MCHAR* pStrSearch)
{
	if(MNULL == pStr &&  MNULL == pStrSearch )
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_strstr MNULL == pStr &&  MNULL == pStrSearch"));
	}
	else if(MNULL == pStr || MNULL == pStrSearch)
	{
		return pStr;
	}
	return (MCHAR*)strstr(pStr,pStrSearch);//strstr 不存在
}

MINT MAPI_atoi(const MCHAR* pStr)
{
	if(MNULL== pStr)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_atoi MNULL== pStr"));
	}
	return (MINT)atoi(pStr);
}

MCHAR* MAPI_MMIAPICOM_Strlower(MCHAR* pStr)
{
#if 0
	if(MNULL == pStr)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_MMIAPICOM_Strlower MNULL== pStr"));
	}
#endif
	return MMIAPICOM_Strlower(pStr);
}

MCHAR* MAPI_MMIAPICOM_Strupper(MCHAR* pStr)
{
#if 0
	if(MNULL ==pStr )
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_MMIAPICOM_Strupper MNULL== pStr"));
	}
#endif
	return MMIAPICOM_Strupper(pStr);    
}

MUINT MAPI_MMIAPICOM_Wstrlen(const MWCHAR* pwstr)
{
#if 0
	if(MNULL == pwstr)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_MMIAPICOM_Wstrlen MNULL == pwstr"));
	}
#endif
	return MMIAPICOM_Wstrlen(pwstr);
}

MINT MAPI_MMIAPICOM_Wstrcmp(const MWCHAR* pwstr0,const MWCHAR* pwstr1)
{
	return MMIAPICOM_Wstrcmp(pwstr0, pwstr1);
}

MWCHAR* MAPI_MMIAPICOM_Wstrlower(MWCHAR* pwstr)
{
#if 0
	if(MNULL == pwstr)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_MMIAPICOM_Wstrlower MNULL== pwstr"));
	}
#endif
	return MMIAPICOM_Wstrlower(pwstr);
}

MWCHAR* MAPI_MMIAPICOM_Wstrcat(MWCHAR* dest_str,const MWCHAR* src_str)
{
	if(MNULL == dest_str && MNULL == src_str )
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_MMIAPICOM_Wstrcat MNULL == dest_str && MNULL == src_str "));
	}
	else if(MNULL == dest_str || MNULL == src_str )
	{
		return dest_str;
	}
	return MMIAPICOM_Wstrcat(dest_str, src_str);
}

MWCHAR* MAPI_MMIAPICOM_Wstrcpy(MWCHAR* pwstrDest, const MWCHAR* pwstrSrc)
{
	if(MNULL == pwstrDest && MNULL == pwstrSrc)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_MMIAPICOM_Wstrcpy MNULL == pwstrDest && MNULL == pwstrSrc"));
	}
	else  if(MNULL == pwstrDest || MNULL == pwstrSrc)
	{
		return pwstrDest;
	}
	return MMIAPICOM_Wstrcpy(pwstrDest, pwstrSrc);
}

MWCHAR* MAPI_McfStdC_wcschr(const MWCHAR* str1_ptr, MUINT str1_len, MWCHAR* str2_ptr, MUINT str2_len)
{//nick 回头修改下，提高效率
	MINT      match_times = 0;
	MINT       i = 0;
	MINT       j = 0;
	MWCHAR*  cmp1_ptr = MNULL;
	MWCHAR*  cmp2_ptr = MNULL;

#if 0
	if(MNULL == str1_ptr && MNULL == str2_ptr)
	{
		//暂时注释掉，调试通过
		MCF_SCI_PASSERT(MFALSE, ("MAPI_McfStdC_wcschr MNULL == str1_ptr ||MNULL == str2_ptr || str2_len >= str1_len"));
	}
	else if(MNULL == str1_ptr || MNULL == str2_ptr)
	{
		return MNULL;
	}
#endif
	match_times = str1_len - str2_len + 1;
	
	for(i=0; i < match_times; i++)
	{
		cmp1_ptr = str2_ptr;
		cmp2_ptr = str1_ptr + i;
		for(j=0; j < str2_len; j++)
		{
			if(*cmp1_ptr != *(cmp2_ptr+j))
			{
				break;
			}
			cmp1_ptr++;
		}
		if(j == str2_len)
		{
			return cmp2_ptr; 
		}
	}
	return MNULL;
}


MINT MAPI_MMIAPICOM_Wstrncmp(const MWCHAR* pwstr0,const MWCHAR* pwstr1,MINT icount)
{
	return MMIAPICOM_Wstrncmp(pwstr0, pwstr1, icount);
}

MWCHAR* MAPI_MMIAPICOM_Wstrncpy(MWCHAR* pwstrDest, const MWCHAR* pwstrSrc, MINT icount)
{
	if((MNULL == pwstrDest || MNULL == pwstrSrc) && 0 != icount)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_MMIAPICOM_Wstrncpy (MNULL == pwstrDest || MNULL == pwstrSrc) && 0 != icount"));
	}	
	return MMIAPICOM_Wstrncpy(pwstrDest, pwstrSrc, icount);
}
MWCHAR* MAPI_MMIAPICOM_Wstrncat(MWCHAR* pwstrDest, const MWCHAR* pwstrSrc, MINT icount)
{
	if((MNULL == pwstrDest || MNULL == pwstrSrc) && 0 != icount)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_MMIAPICOM_Wstrncat (MNULL == pwstrDest || MNULL == pwstrSrc) && 0 != icount"));
	}
	return MMIAPICOM_Wstrncat(pwstrDest, pwstrSrc, icount);
}
MWCHAR* MAPI_MMIAPICOM_Wstrupper(MWCHAR* pwstr)
{
#if 0
	if(MNULL == pwstr)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_MMIAPICOM_Wstrupper MNULL== pwstr"));
	}
#endif
	return MMIAPICOM_Wstrupper(pwstr);
}
MWCHAR* MAPI_MMIAPICOM_StrToWstr(const MCHAR* pstrSrc, MWCHAR* pwstrDest)
{
	if(MNULL == pstrSrc &&  MNULL == pwstrDest)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_MMIAPICOM_StrToWstr MNULL == pstrSrc && MNULL == pwstrDest"));
	}
	else if(MNULL == pstrSrc || MNULL == pwstrDest )
	{
		return pwstrDest;
	}
	return MMIAPICOM_StrToWstr(pstrSrc, pwstrDest);
}
MCHAR* MAPI_MMIAPICOM_WstrToStr(const MWCHAR* pwstrSrc,MCHAR* pstrDest)
{
	if(MNULL == pwstrSrc && MNULL == pstrDest)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_MMIAPICOM_WstrToStr MNULL == pwstrSrc && MNULL == pstrDest"));
	}
	else if(MNULL == pwstrSrc || MNULL == pstrDest)
	{
		return pstrDest;
	}
	return (MCHAR*)MMIAPICOM_WstrToStr(pwstrSrc, pstrDest);
}

MINT MAPI_memcmp(const MVOID* buf1, const MVOID* buf2, MUINT  count)
{
	return (MINT)memcmp(buf1, buf2, count);
}

MVOID* MAPI_memcpy(MVOID* pDst, const MVOID* pSrc, MUINT count)
{
	MCFAPI_INTERFACE_TRACE("[MCARE] MAPI_memcpy pDst = 0x%x, pSrc = 0x%x, count = %d", pDst, pSrc, count);
#ifdef SPREAD_PLAT_SC6530
	if(MNULL == pDst)
	{
		return MNULL;
	}
	else if(MNULL == pSrc || 0 == count)
	{
		return pDst;
	}
#endif
	if((MNULL == pDst || MNULL == pSrc) && 0 !=count)
	{	
		MCF_SCI_PASSERT(MFALSE, ("MAPI_memcpy (MNULL == pDst || MNULL == pSrc) && 0!=count"));
	}
	
	return memcpy(pDst, pSrc, count);
}

MVOID* MAPI_memset(MVOID* buf1, MINT set_value, MUINT count)
{
	if(MNULL == buf1 && 0 != count)
	{
		//浏览器assert
		//QQ assert
		//MCF_SCI_PASSERT(MFALSE, ("MAPI_memset MNULL == buf1 && 0 == count"));
		//浏览器启动时多次发生的assert错误，一直未修改，此处做保护处理。
		return MNULL;
	}
	return memset(buf1, set_value, count);
}

MVOID* MAPI_memchr(const MVOID* buf1, MINT chr, MUINT  count)
{
	if(MNULL == buf1 && 0 != count )
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_memchr MNULL == buf1&& 0 == count "));
	}
	return memchr(buf1, chr, count);
}

MVOID* MAPI_memmove(MVOID* pDst, const MVOID* pSrc, MUINT count)
{
	if((MNULL == pDst || MNULL == pSrc) && 0!=count)
	{
		MCF_SCI_PASSERT(MFALSE, ("MAPI_memmove (MNULL == pDst || MNULL == pSrc) && 0!=count"));
	}
	return memmove(pDst, pSrc, count);
}

MVOID MAPI_srand(MUINT seed)
{
	srand(seed);
}

MINT MAPI_rand(MVOID)
{
	return rand();
}

MINT MAPI__vsnprintf(MCHAR* pstrDest,MUINT nSize, const MCHAR* pstrFmt, va_list aList)
{
	return _vsnprintf(pstrDest, nSize, pstrFmt, aList);
}

//=======================McfStdlib.c API End==========================

//=======================McfTelephone.c API Begin==========================
MINT MAPI_MMIAPICC_MakeCall(MINT simId, const MCHAR* pNumber, MINT call_sim_type)  //nicklei add
{
	CC_RESULT_E cc_result = CC_RESULT_ERROR;
	MINT result = MCR_FAIL;
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA) 

#if (defined SPREAD_PLAT_SC6800H_PDA) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA) 
	cc_result = MMIAPICC_MakeCall(
									McfSim_GetSimID(simId),
									pNumber,
									McfStdC_strlen(pNumber),
									MNULL,
									MNULL,
									call_sim_type,
									McfIF_GetCallNormalCall(),
									MNULL);
#else//#if (defined SPREAD_PLAT_SC6800H_PDA) || (defined SPREAD_PLAT_SC6530_PDA)
	cc_result = MMIAPICC_MakeCall(
									McfSim_GetSimID(simId),
									pNumber,
									McfStdC_strlen(pNumber),
									MNULL,
									call_sim_type,
									McfIF_GetCallNormalCall(),
									MNULL);    
#endif//#if (defined SPREAD_PLAT_SC6800H_PDA) || (defined SPREAD_PLAT_SC6530_PDA)

#else//#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)
	cc_result = MMIAPICC_MakeCall(
							        McfSim_GetSimID(simId),
									pNumber,
									McfStdC_strlen(pNumber),
									MNULL,
									call_sim_type,
									McfIF_GetCallNormalCall());
#endif//#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)

	switch(cc_result)
	{
	case CC_RESULT_SUCCESS:
		result = MCR_SUCCESS;
		break;
	case CC_RESULT_ERROR:
		result = MCR_FAIL;
		break;
	default:
		result = MCR_FAIL;
		break;
	}
	return result;
}

MINT McfIF_GetCallNormalCall(MVOID)
{
	return CC_CALL_NORMAL_CALL;
}

MINT McfCmn_GetCallType(MINT simId)
{
	MINT call_type = CC_CALL_SIM_MAX;
	switch(simId)
	{
	case MSIM_CARD1:
		call_type = CC_SIM1_CALL;
		break;
		
#if (defined MCR_SIM2_SUPPORT) || (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
	case MSIM_CARD2:
		call_type = CC_SIM2_CALL;
		break;
#endif
		
#if (defined MCR_SIM3_SUPPORT) || (defined MCR_SIM4_SUPPORT)
	case MSIM_CARD3:
		call_type = CC_SIM3_CALL;
		break;
#endif
		
#if (defined MCR_SIM4_SUPPORT)
	case MSIM_CARD4:
		call_type = CC_SIM4_CALL;
		break;
#endif

	case MSIM_VWIFI:
		call_type = CC_CALL_SIM_MAX;
		break;

	case MSIM_NONE:
		call_type = CC_CALL_SIM_MAX;
		break;
		
	default:
		break;
	}
	return call_type;
}

//=======================McfStdlib.c API End==========================


//=======================McfTime.c API Begin==========================
ERR_TM_E MAPI_TM_GetSysDate(SCI_DATE_T  *date_ptr )
{
	return TM_GetSysDate(date_ptr);
}
ERR_TM_E MAPI_TM_GetSysTime(SCI_TIME_T  *time_ptr)
{
	return TM_GetSysTime(time_ptr);
}

MUINT MAPI_SCI_GetTickCount(MVOID)    // Number of ticks.
{
	return SCI_GetTickCount();
}

ERR_TM_E McfTm_GetError(MVOID)
{
	return ERR_TM_NONE;
}

//=======================McfTime.c API End==========================


//=======================McfTimer.c API Begin==========================
MBOOL MAPI_MMK_StopTimer(MBYTE timer_id)
{
	return MMK_StopTimer(timer_id);
}


MBOOL McfIF_HandleTimer(MVOID* param)
{
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
	return MFALSE;
#else
	return McfTimer_HandleTimer(*((MBYTE*)param), 0);
#endif
}

MBYTE McfTm_CreateTimer(MUINT interval, MCF_TIMER_FUNC func, MBOOL is_period)
{
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
	return MMK_CreateTimerCallback(interval, func, MNULL, is_period);
#else
	return MMK_CreateWinTimer(McfTm_GetIdleWinId(), interval, is_period);
#endif
}

MMI_HANDLE_T McfTm_GetIdleWinId(MVOID)
{
	return VIRTUAL_WIN_ID;
}
//=======================McfTimer.c API End==========================



//=======================McfUsb.c API Begin==========================
MBOOL MAPI_MMIAPIUDISK_UdiskIsRun(MVOID)
{
	return MMIAPIUDISK_UdiskIsRun();
}
//=======================McfUsb.c API End==========================

//=======================McfVibrate.c API Begin==========================
#ifdef SPREAD_NEW_AUDIO_UPDATE
MVOID McfIF_Vib_PlayVibratorOnce(MVOID)
{
	MMISRVAUD_StartVibrate(MMISRVAUD_PRI_NORMAL, MCARE_VIBRATE_TIME, 0, 1, MNULL);
}
#else//#ifdef SPREAD_NEW_AUDIO_UPDATE

MBOOL MAPI_MMIAPISET_IsVibratTimerActive(MVOID)
{
	return MMIAPISET_IsVibratTimerActive();
}
MVOID MAPI_MMIAPISET_StopVibratTimer(MVOID)
{
	MMIAPISET_StopVibratTimer();
}

MINT MAPI_GetOtherSetRingType(MVOID)
{
	MINT ring_type = (MINT)MMISET_RING_TYPE_OTHER;
	return ring_type;
}

MBOOL MAPI_MMIAUDIO_Vibrate(MUINT vibrate_time)
{
    MBOOL ret= MFALSE;
    MUINT nStartTime = SCI_GetTickCount();
	MUINT nEndTime = 0;

	if(McfRecord_GetRcordStatue())
	{//如果当前正在录音，就把震动屏蔽掉
		return ret;
	}
	
    if(s_vibrator_last_start_timer > nStartTime)//上次的震动还没结束
    {
		return ret;
    }
	
    if (MMIAPISET_IsPermitPlayRing(McfAud_GetRingTypeOther(MAUD_MIDI)))
    {
        if(MMIAPISET_IsVibratTimerActive())
        {
            MMIAPISET_StopVibratTimer();
        }
#ifndef WIN32
        GPIO_SetVibrator(MTRUE);
#endif
    }
	//等待1s
	while(1)
	{
		nEndTime = SCI_GetTickCount();
		if(nEndTime - nStartTime > 50)
		{
			break;
		}
	}

#ifndef WIN32
	GPIO_SetVibrator(MFALSE);
#endif

	s_vibrator_last_start_timer = nEndTime + 1000;
	return MTRUE;
}

MINT McfVib_GetVibTime(MVOID)
{
    return MCARE_VIBRATE_TIME;
}
MVOID McfVib_StopInvalidVibrator(MVOID)
{
    MUINT current_tick = SCI_GetTickCount();
    if(s_vibrator_last_start_timer <= current_tick && 0!=s_vibrator_last_start_timer)
    {
        MMIAUDIO_StopVibrate();
        s_vibrator_last_start_timer = 0;
    }
}

#endif//#ifdef SPREAD_NEW_AUDIO_UPDATE
//=======================McfVibrate.c API Begin==========================

//=======================McfZlib.c API Begin============================

//=======================McfZlib.c API End============================

//--------------------------------Layer-begin----------------------------------
#ifdef MCARE_LAYER_SUPPORT
static GUI_LCD_DEV_INFO  s_mcare_layer_handle = {0, UILAYER_NULL_HANDLE};
static MBOOL is_mcarelayer_exit = MFALSE;
#endif

#ifdef MCARE_LAYER_SUPPORT
GUI_LCD_DEV_INFO McfCmn_GetMcareLayerInfo(MVOID)
{
	return s_mcare_layer_handle;
}

MUINT McfCmn_GetMcareLayerBlockID(MVOID)
{
	return s_mcare_layer_handle.block_id;
}
#endif

MVOID McfIF_CreateMcareLayer(MMI_WIN_ID_T win_id)
{
#ifdef MCARE_LAYER_SUPPORT
	UILAYER_CREATE_T    create_info     = {0};
	UILAYER_APPEND_BLT_T        append_layer = {0};
	
	create_info.lcd_id = McfCmn_GetLCdID();
	create_info.owner_handle = win_id;
	create_info.offset_x = 0;
	create_info.offset_y = 0;
	create_info.width = McfIF_GetScrWidth();
	create_info.height = McfIF_GetScrHeight();
	create_info.is_bg_layer = MFALSE;        // ARGB888
	create_info.is_static_layer = MFALSE;//MTRUE;
	UILAYER_CreateLayer(&create_info, &s_mcare_layer_handle);
	UILAYER_Clear(&s_mcare_layer_handle);
	UILAYER_SetLayerColorKey( &s_mcare_layer_handle, MTRUE, UILAYER_TRANSPARENT_COLOR );
	
	//blt
	append_layer.lcd_dev_info = s_mcare_layer_handle;
	append_layer.layer_level = UILAYER_LEVEL_NORMAL;
	UILAYER_AppendBltLayer(&append_layer);
	
	McfIF_SetIsMcareLayerExit(MTRUE);
#endif	
}

MVOID McfIF_ReleaseMcareLayer(MVOID)
{
#ifdef MCARE_LAYER_SUPPORT
	if (UILAYER_IsMultiLayerEnable())
	{
		UILAYER_ReleaseLayer(&s_mcare_layer_handle);
		s_mcare_layer_handle.block_id = UILAYER_NULL_HANDLE;
	}
	McfIF_SetIsMcareLayerExit(MFALSE);
#endif
}

MBOOL McfIF_IsLayerActive(MVOID)
{
#ifdef MCARE_LAYER_SUPPORT
	return UILAYER_IsLayerActive(&s_mcare_layer_handle);
#else
	return MTRUE;
#endif
}

MBOOL McfIF_GetIsMcareLayerExit(MVOID)
{
#ifdef MCARE_LAYER_SUPPORT
	return is_mcarelayer_exit;
#else
	return MFALSE;
#endif
}

MVOID McfIF_SetIsMcareLayerExit(MBOOL is_exit)
{
#ifdef MCARE_LAYER_SUPPORT
	is_mcarelayer_exit = is_exit;
#endif
}
//--------------------------------Layer-end------------------------------------


/***************************************Lib API end************************************************/
MUINT McfIF_GetDataBufferSize(MVOID)
{
	return (10*1024);
}

/***************************************Lib API end************************************************/

#if (defined SPREAD_PLAT_SC6800H_PDA) || (defined SPREAD_PLAT_SC6530)
MUINT McfIF_GetSoftbarHeight(MVOID)
{
#ifdef MMI_PDA_SUPPORT
	return MMITHEME_GetSoftkeyHeight();
#else
	return 0;
#endif
}
MUINT McfIF_GetFullSoftbarHeight(MVOID)
{
	return MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
}
MVOID McfIF_ScreenDestryCtl(MVOID)
{
	MMK_DestroyControl(MMITHEME_GetStatusBarCtrlId());
}

#else
MUINT McfIF_GetSoftbarHeight(MVOID)
{
#ifdef PDA_UI_SUPPORT_ANDROID
	return MMI_SOFTKEY_BAR_HEIGHT;
#else
	return 0;
#endif
}

MUINT McfIF_GetFullSoftbarHeight(MVOID)
{
#ifdef PDA_UI_SUPPORT_ANDROID
	return MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
#else
	return 0;
#endif
}

MVOID McfIF_ScreenDestryCtl(MVOID)
{
#ifdef PDA_UI_SUPPORT_ANDROID
	MMK_DestroyControl(MMITHEME_GetStatusBarCtrlId());
#endif
}
#endif


MBOOL McfCmn_IsKeyMsg(MUINT msg_id)
{
	if(msg_id == MSG_KEYDOWN_UP || 
		msg_id == MSG_KEYDOWN_UPSIDE || 
		msg_id == MSG_KEYREPEAT_UP || 
		msg_id == MSG_KEYUP_UP || 
		msg_id == MSG_KEYUP_UPSIDE || 	
		msg_id == MSG_KEYLONG_UPSIDE ||
		msg_id == MSG_KEYREPEAT_UPSIDE ||
		msg_id == MSG_KEYPRESSUP_UPSIDE ||
		msg_id == MSG_KEYPRESSUP_UP || 
		msg_id == MSG_KEYLONG_UP || 
		msg_id == MSG_KEYDOWN_DOWN || 
		msg_id == MSG_KEYDOWN_DOWNSIDE || 
		msg_id == MSG_KEYREPEAT_DOWN || 
		msg_id == MSG_KEYUP_DOWN || 
		msg_id == MSG_KEYUP_DOWNSIDE || 
		msg_id == MSG_KEYLONG_DOWNSIDE ||
		msg_id == MSG_KEYREPEAT_DOWNSIDE ||
		msg_id == MSG_KEYPRESSUP_DOWNSIDE ||
		msg_id == MSG_KEYPRESSUP_DOWN || 
		msg_id == MSG_KEYLONG_DOWN || 
		msg_id == MSG_KEYDOWN_LEFT || 
		msg_id == MSG_KEYREPEAT_LEFT || 
		msg_id == MSG_KEYUP_LEFT || 
		msg_id == MSG_KEYPRESSUP_LEFT || 
		msg_id == MSG_KEYLONG_LEFT || 
		msg_id == MSG_KEYDOWN_RIGHT || 
		msg_id == MSG_KEYREPEAT_RIGHT || 
		msg_id == MSG_KEYUP_RIGHT || 
		msg_id == MSG_KEYPRESSUP_RIGHT || 
		msg_id == MSG_KEYLONG_RIGHT || 
		msg_id == MSG_KEYUP_1 || 
		msg_id == MSG_KEYDOWN_1 || 
		msg_id == MSG_KEYDOWN_2 || 
		msg_id == MSG_KEYUP_2 || 
		msg_id == MSG_KEYDOWN_3 || 
		msg_id == MSG_KEYUP_3 || 
		msg_id == MSG_KEYDOWN_4 || 
		msg_id == MSG_KEYUP_4 || 
		msg_id == MSG_KEYDOWN_5 || 
		msg_id == MSG_KEYUP_5 || 
		msg_id == MSG_KEYDOWN_6 || 
		msg_id == MSG_KEYUP_6 || 
		msg_id == MSG_KEYDOWN_7 || 
		msg_id == MSG_KEYUP_7 || 
		msg_id == MSG_KEYDOWN_8 || 
		msg_id == MSG_KEYUP_8 || 
		msg_id == MSG_KEYDOWN_9 || 
		msg_id == MSG_KEYUP_9 || 
		msg_id == MSG_KEYDOWN_0 || 
		msg_id == MSG_KEYUP_0 || 
		msg_id == MSG_KEYDOWN_OK || 
		msg_id == MSG_KEYUP_OK || 
		msg_id == MSG_KEYDOWN_HASH || 
		msg_id == MSG_KEYUP_HASH || 
		msg_id == MSG_KEYDOWN_STAR || 
		msg_id == MSG_KEYUP_STAR || 
		msg_id == MSG_KEYDOWN_CANCEL || 
		msg_id == MSG_KEYUP_CANCEL || 
		msg_id == MSG_KEYDOWN_WEB || 
		msg_id == MSG_KEYUP_WEB || 
		msg_id == MSG_KEYDOWN_RED || 
		msg_id == MSG_KEYUP_RED || 
		msg_id == MSG_KEYDOWN_FLIP || 
		msg_id == MSG_KEYUP_FLIP		
#ifndef SPREAD_PLAT_SC6600L_0937
		|| msg_id == MSG_KEYDOWN_SLIDE
		|| msg_id == MSG_KEYUP_SLIDE
#endif
		|| msg_id == MSG_KEYLONG_1
		|| msg_id == MSG_KEYLONG_2
		|| msg_id == MSG_KEYLONG_3
		|| msg_id == MSG_KEYLONG_4
		|| msg_id == MSG_KEYLONG_5
		|| msg_id == MSG_KEYLONG_6
		|| msg_id == MSG_KEYLONG_7
		|| msg_id == MSG_KEYLONG_8
		|| msg_id == MSG_KEYLONG_9
		|| msg_id == MSG_KEYLONG_STAR
		|| msg_id == MSG_KEYLONG_0
		|| msg_id == MSG_KEYLONG_HASH
		|| msg_id == MSG_KEYLONG_OK
		|| msg_id == MSG_KEYLONG_WEB
		|| msg_id == MSG_KEYLONG_CANCEL
		|| msg_id == MSG_KEYPRESSUP_1
		|| msg_id == MSG_KEYPRESSUP_2
		|| msg_id == MSG_KEYPRESSUP_3
		|| msg_id == MSG_KEYPRESSUP_4
		|| msg_id == MSG_KEYPRESSUP_5
		|| msg_id == MSG_KEYPRESSUP_6
		|| msg_id == MSG_KEYPRESSUP_7
		|| msg_id == MSG_KEYPRESSUP_8
		|| msg_id == MSG_KEYPRESSUP_9
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
		|| msg_id == MSG_KEYPRESSUP_START
#else
		|| msg_id == MSG_KEYPRESSUP_STAR
#endif
		|| msg_id == MSG_KEYPRESSUP_0
		|| msg_id == MSG_KEYPRESSUP_HASH
		|| msg_id == MSG_KEYPRESSUP_OK
		|| msg_id == MSG_KEYPRESSUP_WEB
		|| msg_id == MSG_KEYPRESSUP_CANCEL
		|| msg_id == MSG_KEYREPEAT_1
		|| msg_id == MSG_KEYREPEAT_2
		|| msg_id == MSG_KEYREPEAT_3
		|| msg_id == MSG_KEYREPEAT_4
		|| msg_id == MSG_KEYREPEAT_5
		|| msg_id == MSG_KEYREPEAT_6
		|| msg_id == MSG_KEYREPEAT_7
		|| msg_id == MSG_KEYREPEAT_8
		|| msg_id == MSG_KEYREPEAT_9
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937) ||(defined SPREAD_PLAT_SC6620)
		|| msg_id == MSG_KEYREPEAT_START
#endif        
		|| msg_id == MSG_KEYREPEAT_0
		|| msg_id == MSG_KEYREPEAT_HASH
		|| msg_id == MSG_KEYREPEAT_OK
		|| msg_id == MSG_KEYREPEAT_WEB
		|| msg_id == MSG_KEYREPEAT_CANCEL
#ifdef MMI_PDA_SUPPORT
		|| msg_id == MSG_KEYDOWN_MENU
		|| msg_id == MSG_KEYUP_MENU
		|| msg_id == MSG_KEYREPEAT_MENU
		|| msg_id == MSG_KEYLONG_MENU
#endif
		)
		{
			return MTRUE;
		}
		else
		{
			return MFALSE;
		}
}

MBOOL McfCmn_IsTPMsg(MUINT msg_id)
{
	if(MSG_TP_PRESS_DOWN == msg_id
		|| MSG_TP_PRESS_UP == msg_id
		|| MSG_TP_PRESS_MOVE == msg_id)
	{
		return MTRUE;
	}
	else
	{
		return MFALSE;
	}
	
}
MUINT McfIF_GetMmiResult(MBOOL is_true)
{
	if(is_true)
	{
		return MMI_RESULT_TRUE;
	}
	else
	{
		return MMI_RESULT_FALSE;
	}
}

MBOOL s_IsKeyDownSlide = MFALSE;
MBOOL __McfIF_GetKeyDownSlide(MVOID)
{
	return s_IsKeyDownSlide;
}

MVOID __McfIF_SetKeyDownSlide(MBOOL isKeyDownSlide)
{
	s_IsKeyDownSlide = isKeyDownSlide;
}
MBOOL McfIF_GetKeyDownSlide(MVOID)
{
	return __McfIF_GetKeyDownSlide();
}

MVOID McfIF_SetKeyDownSlide(MBOOL isKeyDownSlide)
{
	__McfIF_SetKeyDownSlide(isKeyDownSlide);
}


#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
MVOID McfIF_NotifyGifDecodeOneFrame(IMGPROC_GIF_DISPLAY_INFO_T   *display_info_ptr)
{
}
MBOOL McfIF_IsContinueDecodeGif(MVOID)
{
	return MFALSE;
}
MBOOL McfIF_IsAppDecodeGif(MVOID)
{
	return McfGdi_IsDecodeGif();
}
#endif


////进入Mcare的操作
MBOOL  McareV31_EntryMcareCallBack(MVOID)
{
	////此处用户添加，进入Mcare  时需要执行的定制化操作
	return MFALSE;
}
////将要退出Mcare的操作
BOOLEAN  McareV31_WillExitMcareCallBack(void)
{
	////此处用户添加，将要退出Mcare  时需要执行的定制化操作
	return MFALSE;
}
////完全退出Mcare的操作
MBOOL  McareV31_ExitMcareCallBack(MVOID)
{
	////此处用户添加，退出Mcare  时需要执行的定制化操作
#ifdef MMI_TASK_MANAGER
	MCFAPI_INTERFACE_TRACE("[MCARE] McareV31_ExitMcareCallBack MCare_StopApplet");
	MCare_StopApplet();
#endif
	return MFALSE;
}

////进入Mcare前台的操作
BOOLEAN  McareV31_EntryMcareFrontCallBack(void)
{
	////此处用户添加，进入Mcare前台  时需要执行的定制化操作
	return MFALSE;
}
////进入Mcare后台的操作
BOOLEAN  McareV31_ExitMcareBackCallBack(void)
{
	////此处用户添加，进入Mcare后台  时需要执行的定制化操作
	return MFALSE;
}

////进入Mcare界面的操作
BOOLEAN  McareV31_EntryMcareWinCallBack(void)
{
	////此处用户添加，进入Mcare  界面时需要执行的定制化操作
	return MFALSE;
}
////退出Mcare界面的操作
BOOLEAN  McareV31_ExitMcareWinCallBack(void)
{
	////此处用户添加，退出Mcare  界面时需要执行的定制化操作
	return MFALSE;
}


#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
extern void MMU_IcacheSyncAll (void);
extern void MMU_DcacheSyncAll (void);
#endif

MVOID McfIF_InvalidelCACHEALL(MVOID)
{
#ifndef WIN32
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
	MMU_DcacheSyncAll();
	MMU_IcacheSyncAll();
#endif
#endif
}


/////////////////////////////////exit confirm//////////////////

/*
*QQ应用中心正在运行中，是否退出？
*/
const MUSHORT strMcareExitConfirm[] = {0x0051,0x0051,0x5E94,0x7528,0x4E2D,0x5FC3,0x6B63,0x5728,0x8FD0,0x884C,0x4E2D,0xFF0C,0x662F,0x5426,0x9000,0x51FA,0xFF1F,0x0000};
static MBOOL Start_Udisk = MFALSE;
MVOID Mcare_Udisk_Start(MVOID)
{
	Start_Udisk = MTRUE;
}
static MMI_RESULT_E Mcare_exit_PubWin_Handle(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{ 
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id)
	{
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		if(Start_Udisk)
		{
			Start_Udisk = MFALSE;      
		}
		break;
		
	case MSG_CTL_OK:
	case MSG_APP_OK:
	case MSG_APP_WEB:
		if(MMK_IsOpenWin(MMIMCARE_EDITBOX_WIN_ID))
		{
			MMK_CloseWin(MMIMCARE_EDITBOX_WIN_ID);
		}
		MCFAPI_INTERFACE_TRACE("[MCARE] Mcare_exit_PubWin_Handle McareV31_ExitMcare");
		McareV31_ExitMcare();
		MMK_CloseWin(win_id);
		if(Start_Udisk)
		{
			if(!MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_USB))
			{
				MMIUDISK_StartUdisk();
				MMK_CloseWin(MMIUDISK_USB_OPER_SELECT_WIN_ID);
			}
			else
			{
#if (defined SPREAD_PLAT_SC6530)
				MCAREV31_StartUdisk();
#else
				MMIAPISET_ValidatePrivacyPwd( MMIUDISK_USB_OPER_SELECT_WIN_ID, MMISET_PROTECT_USB_TYPE );
#endif
			}
			Start_Udisk = MFALSE;
		}
		break;
		
	case MSG_TIMER:
		break;
		
	default:
		result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
		break;
	}
	return(result);
}

MBOOL MCareV31_Exit_Confirm(MVOID)
{
	if(McareV31_IsMcareRunning())
	{
		MMI_STRING_T strNotice = {0};
		MMI_WIN_ID_T win_id = MMIMCARE_EXITCONFIRM_WIN_ID;
		strNotice.wstr_len = MMIAPICOM_Wstrlen(strMcareExitConfirm);
		strNotice.wstr_ptr = (MWCHAR*)strMcareExitConfirm;
		MMIPUB_OpenAlertWinByTextPtr(MNULL,&strNotice,MNULL,McfCmn_GetWarningImgId(),&win_id,MNULL,MMIPUB_SOFTKEY_OKCANCEL, Mcare_exit_PubWin_Handle);	
		return MTRUE;
	}
	else
	{
		return MFALSE;
	}
}

MBOOL MCareV31_Close_ExitConfirm_PubWin(void)
{
	if(MMK_IsOpenWin(MMIMCARE_EXITCONFIRM_WIN_ID))
	{
		MMK_CloseWin(MMIMCARE_EXITCONFIRM_WIN_ID);
	}
	return MTRUE;
}

/////////////////////////////////exit confirm//////////////////


#ifdef MMI_TASK_MANAGER
static MMI_RESULT_E Mcare_Applet_HandleEvent(IAPPLET_T* iapplet_ptr, MMI_MESSAGE_ID_E msg_id, DPARAM param);

typedef struct 
{
	CAF_APPLET_T caf_applet;
	MUINT  user_data;
}MMI_MCARE_APPLET_T;   

static MMI_MCARE_APPLET_T* s_mcare_applet_instance = MNULL; 
//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mcare_applet_array[] = 
{
#if(defined SPREAD_PLAT_SC6530)
//1.注意在展讯的SC6530上面，目前发现从W12.22版本以后，因为展讯升级，导致此处需要修改结构体
//2.CAF_STATIC_APPLET_T,添加GUID_NAME_DEF。FAE移植时根据具体情况修改此处。
//3.表现形式为进入mcare就死机，在applet里面死机。
	{GUID_NAME_DEF(MCARE_APPLET_ID), sizeof(MMI_MCARE_APPLET_T), Mcare_Applet_HandleEvent, 
		IMAGE_SECMENU_ICON_TMC, TXT_TMC_APP},
#else
	{MCARE_APPLET_ID, sizeof(MMI_MCARE_APPLET_T), Mcare_Applet_HandleEvent, 
		IMAGE_SECMENU_ICON_TMC, TXT_TMC_APP},
#endif
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T g_mcare_applet_info = 
{
	s_mcare_applet_array, ARR_SIZE(s_mcare_applet_array)
};

static MBOOL s_mcare_stop_applet = MTRUE; 

MVOID McfIF_RegAppletInfo(MVOID)
{
	MCFAPI_INTERFACE_TRACE("[Mcare] McfIF_RegAppletInfo");
	//暂时注释掉
	MMI_RegAppletInfo(MMI_MODULE_MCARE_MINI, &g_mcare_applet_info);
}

static MMI_RESULT_E Mcare_Applet_HandleEvent(IAPPLET_T* iapplet_ptr, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_MCARE_APPLET_T* applet_ptr = (MMI_MCARE_APPLET_T*)iapplet_ptr;
	
	MCFAPI_INTERFACE_TRACE("Mcare_Applet_HandleEvent msg_id = 0x%x",msg_id);
	switch (msg_id)
	{
	case MSG_APPLET_START:
	case MSG_APPLET_RESUME:
	case MSG_APPLET_SWITCH:
		s_mcare_applet_instance = applet_ptr;
		if(MNULL !=param)
		{
			if(MCR_FAIL == McfScrn_EntryNewScreen(MNULL, MNULL, MNULL))
			{
				McfGdi_DisplayWarnning(s_mcare_run_failed);
				MCFAPI_INTERFACE_TRACE("[MCARE] Mcare_Applet_HandleEvent MCare_StopApplet");
				MCare_StopApplet();
				return;
			}
		}
        else
        {
            //MCareV31_Main_Entry();
            MCareV31_SetAppType(MCareV31_GetAppTypeMcare);
			if(MCR_FAIL == McfScrn_EntryNewScreen(MNULL, MNULL, MNULL))
			{
				McfGdi_DisplayWarnning(s_mcare_run_failed);
				MCFAPI_INTERFACE_TRACE("[MCARE] Mcare_Applet_HandleEvent 22 MCare_StopApplet");
				MCare_StopApplet();
				return;
			}
        }
		break;
	case MSG_APPLET_STOP:
        s_mcare_applet_instance = MNULL;
		if(s_mcare_stop_applet)
		{
			MCFAPI_INTERFACE_TRACE("[MCARE] Mcare_Applet_HandleEvent McareV31_ExitMcare");
	        McareV31_ExitMcare();
		}
		s_mcare_stop_applet = MTRUE;
		break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}    
	return result;
}

MVOID MCare_StartApplet(MINT applet_type)
{
	MCFAPI_INTERFACE_TRACE("[MCARE] MCare_StartApplet applet_type = %d", applet_type);
	if(McareV31_IsMcareRunning())
	{
		if(MCR_FAIL == McfScrn_EntryNewScreen(MNULL, MNULL, MNULL))
		{
			McfGdi_DisplayWarnning(s_mcare_run_failed);
			MCFAPI_INTERFACE_TRACE("[MCARE] MCare_StartApplet MCare_StopApplet");
			MCare_StopApplet();
			return;
		}
	}
	else
	{
		if(MNULL == s_mcare_applet_instance)
		{
			MMI_APPLET_START_T start = {0};    
			start.guid = MCARE_APPLET_ID;
			start.state = MMI_APPLET_STATE_BG_HIDE_WIN|MMI_APPLET_STATE_TASK_MANAGER;
			start.param_ptr = (void *)&applet_type;
			start.size_of_param = sizeof(applet_type);
			MMK_StartApplet(&start);
		}
	}
}

MVOID MCare_StopApplet(MVOID)
{
	MCFAPI_INTERFACE_TRACE("[MCARE] MCare_StopApplet");
	if(MNULL != s_mcare_applet_instance)
	{
		MMK_CloseApplet(MCARE_APPLET_ID);
	}
}

#endif//#ifdef MMI_TASK_MANAGER


/*****************************************************************************/
//  Description : get the type of language 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
MVOID MAPI_MMIAPISET_GetLanguageType(MINT* language_type_ptr)
{
	MMIAPISET_GetLanguageType((MMISET_LANGUAGE_TYPE_E*)language_type_ptr);
}


MINT MCR_DoSome1(MINT nType,MINT nParam)
{
	MINT nRet = 0;
	switch(nType)
	{
	case DOSO_SOCREAD_INTERVAL:
		nRet = MTRUE;
		break;
	case DOSO_MULTIBG_SUPPORT:
		nRet = Mcf_SupportMultiBG();
		break;
	case DOSO_APP_NO_BG_SUPPORT:
		if(!McfIF_SupportAppBG(nParam))
		{
			nRet = 1;
		}
		else
		{
			nRet = 0;
		}
		break;
	case DOSO_ENDKEY_BACK:
		nRet = 0;
		break;
	default:
		break;
	}
	return nRet;
}

MINT MCR_DoSome2(MINT nType,MINT nParam,MVOID *pParam)
{
    MINT nRet = 0;
    return nRet;
}

MVOID McfIF_Init(MVOID)
{
	s_vibrator_last_start_timer = 0;
}

MVOID McfIF_Deinit(MVOID)
{
	s_vibrator_last_start_timer = 0;
}


MVOID Mcare_Exit_Exception(MVOID)
{
	MCFAPI_INTERFACE_TRACE("[MCARE] Mcare_Exit_Exception MCare_StopApplet");
	McfScrn_CloseScreen(McfScrn_GetCurScreenID());
#ifdef MMI_TASK_MANAGER
	s_mcare_stop_applet = MFALSE;
	MCare_StopApplet();
#endif
	McfIF_CloseGPRS();
}

MBOOL McfRecord_CheckPermit(MVOID)
{
	return MTRUE;
}

MINT McfIF_GetCurAngle(MVOID)
{
#ifdef MCARE_3D_SUPPORT
	MINT cur_angle = MSCREEN_ROTATE_0;
	LCD_ANGLE_E  rotate_angle = LCD_ANGLE_0;
	LCD_INFO_T lcd_info = {0};
	GUI_LCD_DEV_INFO dev = McfCmn_GetDevInfo();
	GUILCD_GetInfo(dev.lcd_id, &lcd_info);
	
	rotate_angle = GUILCD_GetRotateAngle(dev.lcd_id);
	switch (rotate_angle)
	{
	case LCD_ANGLE_0:
		cur_angle = MSCREEN_ROTATE_0;
		break;
	case LCD_ANGLE_90:
		cur_angle = MSCREEN_ROTATE_90;
		break;
	case LCD_ANGLE_180:
		cur_angle = MSCREEN_ROTATE_180;
		break;
	case LCD_ANGLE_270:
		cur_angle = MSCREEN_ROTATE_270;
		break;
	default:
		cur_angle = MSCREEN_ROTATE_0;
		break;
	}
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"[MCFSCREEN]zhukey McfIF_GetCurAngle cur_angle = %d", cur_angle);
	return cur_angle;
#else
	MCFAPI_INTERFACE_PRINT(ALOG_INFO,MLOG_SDKINTERFACE, __MCR_FUNCTION__"[MCFSCREEN]zhukey McfIF_GetCurAngle cur_angle = 0 not support MCARE_3D_SUPPORT");
	return 0;
#endif
}



MVOID McareV31_PreEntry(MVOID)
{
	MCFAPI_INTERFACE_TRACE("[Mcare] McareV31_PreEntry");
#ifdef MMI_TASK_MANAGER
	MCare_StartApplet(MCareV31_GetAppType());
#else
	if(MCR_FAIL == McfScrn_EntryNewScreen(MNULL, MNULL, MNULL))
	{
		McfGdi_DisplayWarnning(s_mcare_run_failed);
		return;
	}
#endif
}


MVOID McareV31_DoEndKey(MVOID)
{	
	MMK_SendMsg(McfScrn_GetCurScreenID(), MSG_KEYDOWN_RED, MNULL);
	MMK_SendMsg(McfScrn_GetCurScreenID(), MSG_KEYUP_RED, MNULL);
}


////////////////////////////Active GPRS before entry//////////////////////////////////////////////
static MMI_RESULT_E McfIF_HandleActiveGPRS(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
static MBYTE s_active_gprs_timerid = 0;

WINDOW_TABLE(MMIMCARE_ACTIVEGPRS_WIN_TAB) = 
{
	WIN_FUNC(McfIF_HandleActiveGPRS),
	WIN_ID(MMIMCARE_ACTIVEGPRS_WIN_ID),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
#ifdef MMI_PDA_SUPPORT
    WIN_HIDE_STATUS,
#endif
	END_WIN
};

static MVOID McfIF_DrawActiveGprs(MVOID)
{
#define IF_DEEP_BLUE (0x2b72)
#define IF_WHITE (McfGdi_MakeMColorFromRGB(255,255,255))
	//网络激活中，请稍候…
	const MWCHAR wait[] = {0x7F51,0x7EDC,0x6FC0,0x6D3B,0x4E2D,0xFF0C,0x8BF7,0x7A0D,0x5019,0x2026,0x0000};
	MINT nScrHeight = McfGdi_GetScrHeight();
	MINT nScrWidth = McfGdi_GetScrWidth();
	
	McfGdi_Init();
	McfGdi_SetFont(MGDI_FONTSIZE_MEDIUM,0);
	
	McfGdi_BeginDraw();
	McfGdi_FillRect(0, 0, nScrWidth, nScrHeight, IF_DEEP_BLUE);
	McfGdi_SetTextColor_Int(IF_WHITE);
	McfGdi_DrawText(nScrWidth/2 - McfGdi_GetTextWidth(wait)/2, nScrHeight/2-McfGdi_GetCharHeight()/2, wait);
	McfGdi_EndDraw();
#undef IF_WHITE
#undef IF_DEEP_BLUE
}

static MMI_RESULT_E McfIF_HandleActiveGPRS(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_HandleActiveGPRS msg_id = 0x%x, win_id = %d", msg_id, win_id);
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
#if (defined SPREAD_PLAT_SC6600L_1025)  || (defined SPREAD_PLAT_SC6600L_0937)
		//此处需要FAE自行添加处理，屏蔽掉STATUSBAR
#else

#if defined(PDA_UI_SUPPORT_ANDROID)   
		IGUICTRL_SetState(MMK_GetCtrlPtr(MMICOMMON_STATUSBAR_CTRL_ID), GUICTRL_STATE_INVISIBLE, MTRUE);	
#endif  

#endif
		McfIF_DrawActiveGprs();
		if(0 != s_active_gprs_timerid)
		{
			MAPI_MMK_StopTimer(s_active_gprs_timerid);
			s_active_gprs_timerid = 0;
		}

		McfIF_ApnInit();
		if(MSIM_VWIFI != McfIF_GetGPRSSIMID())
		{
			s_active_gprs_timerid = MMK_CreateWinTimer(win_id,20000, MFALSE);
		}
		break;
	case MSG_FULL_PAINT:
		McfIF_DrawActiveGprs();		
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	case MSG_KEYUP_RED:
		McfIF_CloseGPRS();
		MMK_CloseWin(MMIMCARE_ACTIVEGPRS_WIN_ID);
		break;
	case MSG_TIMER:
        if (s_active_gprs_timerid == *(MBYTE*)param)
        {
			MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_HandleActiveGPRS s_active_gprs_timerid = %d, *(MBYTE*)param = %d", s_active_gprs_timerid, *(MBYTE*)param);
			MAPI_MMK_StopTimer(s_active_gprs_timerid);
			s_active_gprs_timerid = 0;
			McfIF_CloseGPRS();
			McfIF_CloseActiveGPRSScreenAndWarnnig();
        }
		break;
	case MSG_CLOSE_WINDOW:
		if(0 != s_active_gprs_timerid)
		{
			MAPI_MMK_StopTimer(s_active_gprs_timerid);
			s_active_gprs_timerid = 0;
		}
		break;
	default:
		//此处无需去相应其他的消息
		result = MMI_RESULT_TRUE;
		break;
	}
	return result;
}

extern int MCR_PreEntryCheck(void);
MVOID McfIF_ShowActiveGPRSScreen(MVOID)
{
	MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_ShowActiveGPRSScreen entry");

	if(0 == MCR_PreEntryCheck())
	{
		MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_ShowActiveGPRSScreen 0 == MCR_PreEntryCheck() return");
		return;
	}
	
	if(McfDtcnt_GprsActived())
	{
		MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_ShowActiveGPRSScreen McfDtcnt_GprsActived McareV31_PreEntry");
		//挂后台时进入，已经不需要再去初始化
		//Entry
		McareV31_PreEntry();
	}
	else
	{
		//第一次进入时，需要将初始化信息提前，并在McfCmn_Initialize中去除
		McfDtcnt_Init();
		McfIF_Init();
		McfFile_Init();
		McfTimer_Initialize();
		McfGdi_Init();
		//第一次进入时，需要将初始化信息提前，并在McfCmn_Initialize中去除
		
		if(MMK_IsOpenWin(MMIMCARE_ACTIVEGPRS_WIN_ID))
		{
			MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_ShowActiveGPRSScreen MMK_CloseWin");
	        MMK_CloseWin(MMIMCARE_ACTIVEGPRS_WIN_ID);
		}
		MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_ShowActiveGPRSScreen MMK_CreateWin");
		//create win
		MMK_CreateWin((MUINT *)MMIMCARE_ACTIVEGPRS_WIN_TAB, MNULL);
	}
}

MVOID McfIF_CloseActiveGPRSScreenAndEntry(MVOID)
{
	MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_CloseActiveGPRSScreenAndEntry entry");
	if(MMK_IsOpenWin(MMIMCARE_ACTIVEGPRS_WIN_ID))
	{
		MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_CloseActiveGPRSScreenAndEntry McareV31_PreEntry");
		//Entry
		McareV31_PreEntry();
        //MMK_CloseWin(MMIMCARE_ACTIVEGPRS_WIN_ID);
	}
}

//网络激活失败，是否继续？
static MUSHORT s_mcare_active_gprs_failed[] = {0x7F51,0x7EDC,0x6FC0,0x6D3B,0x5931,0x8D25,0xFF0C,0x662F,0x5426,0x7EE7,0x7EED,0xFF1F,0x0000};
MVOID McfIF_CloseActiveGPRSScreenAndWarnnig(MVOID)
{
	MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_CloseActiveGPRSScreenAndWarnnig entry");
	if(MMK_IsOpenWin(MMIMCARE_ACTIVEGPRS_WIN_ID))
	{
		MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_CloseActiveGPRSScreenAndWarnnig McfGdi_DisplayWarnning");
		McfIF_DisplayOKCancle(s_mcare_active_gprs_failed);
        MMK_CloseWin(MMIMCARE_ACTIVEGPRS_WIN_ID);
	}
}

MVOID McfIF_CloseActiveGPRSScreen(MVOID)
{
	MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_CloseActiveGPRSScreen entry");
	if(MMK_IsOpenWin(MMIMCARE_ACTIVEGPRS_WIN_ID))
	{
		MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_CloseActiveGPRSScreen MMK_CloseWin(MMIMCARE_ACTIVEGPRS_WIN_ID)");
		MMK_CloseWin(MMIMCARE_ACTIVEGPRS_WIN_ID);
	}
}

static MMI_RESULT_E McfIF_HanldeOKCancle(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{ 
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id)
	{
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
	case MSG_KEYUP_RED:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_CTL_OK:
	case MSG_APP_OK:
	case MSG_APP_WEB:
		MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_HanldeOKCancle McareV31_PreEntry");
		//Entry
		McareV31_PreEntry();
		MMK_CloseWin(win_id);
		break;
		
	case MSG_TIMER:
		break;
		
	default:
		result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
		break;
	}
	return(result);
}

MBOOL McfIF_DisplayOKCancle(MUSHORT* wstr)
{
	MMI_STRING_T strNotice = {0};
	MMI_WIN_ID_T win_id = MMIMCARE_ACTIVEGPRS_OKC_WIN_ID;
	strNotice.wstr_len = MMIAPICOM_Wstrlen(wstr);
	strNotice.wstr_ptr = (MWCHAR*)wstr;

	MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_DisplayOKCancle");
	
	MMIPUB_OpenAlertWinByTextPtr(MNULL,&strNotice,MNULL,McfCmn_GetWarningImgId(),&win_id,MNULL,MMIPUB_SOFTKEY_OKCANCEL, McfIF_HanldeOKCancle);	
	return MTRUE;
}

MBOOL McfIF_CloseOKCancle(void)
{
	MCFAPI_INTERFACE_TRACE("[MCARE] McfIF_CloseOKCancle");
	if(MMK_IsOpenWin(MMIMCARE_ACTIVEGPRS_OKC_WIN_ID))
	{
		MMK_CloseWin(MMIMCARE_ACTIVEGPRS_OKC_WIN_ID);
	}
	return MTRUE;
}

MBOOL Mcf_SupportMultiBG(MVOID)
{
#ifdef MCARE_SUPPORT_MULTI_BG
    return MFALSE;
#else
    return MTRUE;
#endif
}


////////////////////////////Active GPRS before end//////////////////////////////////////////////

MINT McfIF_GetInvalidFSHandle(MVOID)
{
	return SFS_INVALID_HANDLE;
}

MINT McfIF_GetFSErrorNone(MVOID)
{
	return SFS_ERROR_NONE;
}


MBOOL McfIF_SupportQCBG(MVOID)
{
#ifdef MCARE_SUPPORT_QC_BG
    return MTRUE;
#else
    return MFALSE;
#endif
}
MBOOL McfIF_SupportQQBG(MVOID)
{
#ifdef MCARE_SUPPORT_QQ_BG
    return MTRUE;
#else
    return MFALSE;
#endif
}

MBOOL McfIF_SetSIMPriority(MVOID)
{
#ifdef MCARE_SET_SIM_PRIORITY
    return MTRUE;
#else
    return MFALSE;
#endif
}

///////////////////////////Trace接口的开放begin/////////////////////

extern MCHAR logbuf[512];
extern MWCHAR logTempBuf[256];

//---------------------------------------------------------------------------
//FUNCTION: McfTrace_TraceFormatA
//Descrption:log MCAHR型,以log输出，需要展讯的工具支持
//Param:
//  IN:format, ...格式化参数
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTrace_TraceFormatA(const MCHAR *format, ...)
{
	va_list arg_list;

    memset((MCHAR*)logTempBuf, 0, 512);
	sprintf((MCHAR*)logTempBuf, "[MCARE]");
	va_start(arg_list ,format);
	//vsprintf(logbuf+strlen(logbuf), format, arg_list);
	_vsnprintf((MCHAR*)logTempBuf+strlen((MCHAR*)logTempBuf), 
	            512 - strlen((MCHAR*)logTempBuf), format, arg_list);
	va_end(arg_list);

	SCI_TRACE_LOW((MCHAR*)logTempBuf);
}


//---------------------------------------------------------------------------
//FUNCTION: McfTrace_TraceFormat
//Descrption:log MWCHAR型,以log输出，需要展讯的工具支持
//Param:
//  IN:format, ...格式化参数
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTrace_TraceFormat(const MWCHAR* format, ...)
{
	va_list arg_list;

	va_start(arg_list ,format);
	memset((MCHAR*)logTempBuf, 0, 512);
	McfVsnwprintf(logTempBuf, 0xff, (const MWCHAR*)format,arg_list);
	va_end(arg_list);
	memset(logbuf, 0, 512);
	McfStdC_wstrtostr( logbuf,logTempBuf);

	SCI_TRACE_LOW((MCHAR*)logbuf);
}
///////////////////////////Trace接口的开放end/////////////////////

/////////////////////////Idle接口开放start////////////////////////////
MVOID McfIF_IdleSatuBarInit(MINT iconFlag)
{
	MUINT img_id = McfCmn_GetIdleIconImageID();
	GUIWIN_STBDATA_ITEM_DATA_T  item_data = {0};
	
	item_data.icon_info.icon_num = 1;
	item_data.icon_info.icon_type = (GUIWIN_STBDATA_ICON_STYLE_E)McfIdle_Get_GUIWIN_STBDATA_ICON_NORMAL();
	item_data.icon_info.is_display = MTRUE;
	item_data.cur_frame = 0;
	item_data.is_handle_callback = MTRUE;
	
	item_data.icon_info.icon_array[0] = img_id;
	item_data.app_handle_func = McfIdle_HandlePenIconFunc;
	
	MAPI_GUIWIN_AppendStbItemData(&item_data);
	if(MIDLE_ICON_BLINK == iconFlag || MIDLE_ICON_SHOW == iconFlag)
	{
		MAPI_GUIWIN_SetStbItemVisible(McfIdle_GetIdleIconID(), MTRUE);
	}
	else
	{
		MAPI_GUIWIN_SetStbItemVisible(McfIdle_GetIdleIconID(), MFALSE);
	}
}

MVOID McfIF_UpdateStatusIcons(MINT iconFlag, MINT iconID)
{
	MUINT image_id = 0;
	GUIWIN_STBDATA_ICON_STYLE_E icon_type = McfIdle_GetIconStyleNormal();
	image_id = McfCmn_GetIdleIconImageID();
	
	//MCFAPI_IDLE_PRINT(ALOG_INFO,MLOG_SDKIDLE, __MCR_FUNCTION__"McfIdle_UpdateStatusIcons g_mc_statusbar.m_IconID = %d, g_mc_statusbar.m_IconFlag = %d", g_mc_statusbar.m_IconID, g_mc_statusbar.m_IconFlag);
	if(McfIF_IsOnlyStaterBarIconMsg())
	{
		if(iconFlag == MIDLE_ICON_BLINK && iconID != MIDLE_ICON_QQ_HAVEMSG)
		{
			image_id = McfIdle_GetIdleIconImageIDNull();
		}
	}
	
	MAPI_GUIWIN_SetStbItemIconId(McfIdle_GetIdleIconID(), image_id);
	MAPI_GUIWIN_SetStbItemIconCurFrame(McfIdle_GetIdleIconID(), 0);
	MAPI_GUIWIN_GetStbItemIconStyle(McfIdle_GetIdleIconID(),&icon_type);
	if (iconFlag == MIDLE_ICON_SHOW)
	{
		MCFAPI_IDLE_PRINT(ALOG_INFO,MLOG_SDKIDLE, __MCR_FUNCTION__"McfIdle_UpdateStatusIcons MIDLE_ICON_SHOW");
		MAPI_GUIWIN_SetStbItemVisible(McfIdle_GetIdleIconID(), MTRUE);
		//McfIdle_DropDownNotifyRecord(MTRUE, g_mc_statusbar.m_IconID);
	}
	else if (iconFlag == MIDLE_ICON_BLINK)
	{
		MCFAPI_IDLE_PRINT(ALOG_INFO,MLOG_SDKIDLE, __MCR_FUNCTION__"McfIdle_UpdateStatusIcons MIDLE_ICON_BLINK");
		MAPI_GUIWIN_SetStbItemVisible(McfIdle_GetIdleIconID(), MTRUE);
		//McfIdle_DropDownNotifyRecord(MTRUE, g_mc_statusbar.m_IconID);
		//blink时是不需要设置的
	}
	else
	{
		MCFAPI_IDLE_PRINT(ALOG_INFO,MLOG_SDKIDLE, __MCR_FUNCTION__"McfIdle_UpdateStatusIcons else");
		MAPI_GUIWIN_SetStbItemVisible(McfIdle_GetIdleIconID(), MFALSE);
		//McfIdle_DropDownNotifyRecord(MFALSE, g_mc_statusbar.m_IconID);
	}
	if(!McfIdle_IsPhoneLocked())
	{
		McfIdle_UpdateStaterBarIcon();
	}
}

/////////////////////////Idle接口开放end////////////////////////////

#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
MBOOL McfIF_Dtcnt_ActiveWifi(pfnHandleFunc pfn)
{
	MMIPDP_ACTIVE_INFO_T app_info = {0};
	MBOOL result = MFALSE;
	
	MCFAPI_SOCKET_PRINT(ALOG_INFO,MLOG_SDKSOCKET, __MCR_FUNCTION__"zhu GPRS McfDtcnt_ActiveWifi");
	app_info.app_handler = McfIF_GetModuleID();
	app_info.handle_msg_callback = pfn;
	app_info.ps_interface = (MMIPDP_INTERFACE_E)McfIF_GetPDPInterfaceWIFI();
	
	MCFAPI_SOCKET_PRINT(ALOG_INFO,MLOG_SDKSOCKET, __MCR_FUNCTION__"zhu GPRS McfDtcnt_ActiveWifi app_handler = %d, callback = 0x%x, ps_interface = %d", app_info.app_handler, app_info.handle_msg_callback, app_info.ps_interface);
	result = MAPI_MMIAPIPDP_Active(&app_info);
	
	MCFAPI_SOCKET_PRINT(ALOG_INFO,MLOG_SDKSOCKET, __MCR_FUNCTION__"zhu GPRS McfDtcnt_ActiveWifi result = %d", result);
	return result;
}
#endif

#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC6610) || (defined SPREAD_PLAT_SC8800G)\
|| (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)

MBOOL McfIF_Dtcnt_ActiveGprs(MINT select_card, MBYTE* APNcn, pfnHandleFunc pfn)
{
	MBOOL return_val = MFALSE;
	MBYTE setting_index = 0;
	MMIPDP_ACTIVE_INFO_T app_info = {0};
	MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = MNULL;
	MN_DUAL_SYS_E dual_sys = McfSim_GetSimID(select_card);
	
	MCFAPI_SOCKET_PRINT(ALOG_INFO,MLOG_SDKSOCKET, __MCR_FUNCTION__"zhu GPRS McfDtcnt_ActiveGprs dual_sys = %d, apn = %s", select_card, APNcn);

	if(MSIM_VWIFI == select_card)
	{//WIFI分支处理
	
		MCFAPI_SOCKET_PRINT(ALOG_INFO,MLOG_SDKSOCKET, __MCR_FUNCTION__"zhu GPRS McfDtcnt_ActiveGprs McfDtcnt_ActiveWifi return");
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
#ifndef WIN32
    McfDtcnt_ActiveWifi();
#endif
#endif
		return MTRUE;
	}
	
	if(MNULL == APNcn || !McfSim_IsVerify(select_card))
	{
		MCFAPI_SOCKET_PRINT(ALOG_INFO,MLOG_SDKSOCKET, __MCR_FUNCTION__"zhu GPRS McfDtcnt_ActiveGprs MNULL == APNcn || !McfSim_IsVerify(dual_sys) return");
		return MFALSE;
	}
	//已经连接成功
#ifndef WIN32
	if(McfDtcnt_GprsActived() || McfDtcnt_GprsActiving())
	{
		MCFAPI_SOCKET_PRINT(ALOG_INFO,MLOG_SDKSOCKET, __MCR_FUNCTION__"zhu GPRS McfDtcnt_ActiveGprs McfDtcnt_GprsActived() || McfDtcnt_GprsActiving() return");
		return MFALSE;
	}
#endif

	setting_index = McfIF_GetNetSettingIndex(dual_sys);
	
	setting_item_ptr = MAPI_CONNECTION_GetLinkSettingItemByIndex(dual_sys,setting_index);
	
	if(setting_item_ptr != MNULL)
	{
		app_info.app_handler = McfIF_GetModuleID();
		app_info.auth_type = setting_item_ptr->auth_type;
		app_info.apn_ptr = APNcn;
		app_info.user_name_ptr = (MCHAR*)setting_item_ptr->username;
		app_info.psw_ptr = (MCHAR*)setting_item_ptr->password;
		app_info.dual_sys = dual_sys;
		app_info.priority = 3;
		#if (!defined SPREAD_PLAT_SC6620)
		app_info.ps_service_rat = (MN_PS_SERVICE_RAT_E)McfIF_GetPSServiceRat();
		#endif
		app_info.ps_interface = (MMIPDP_INTERFACE_E)McfIF_GetPDPInterfaceGPRS();
		app_info.handle_msg_callback = pfn;
		MAPI_MNGPRS_ReadStorageExtReqQosEx(dual_sys, McfDcnt_GetGprsType(), McfDcnt_GetSerivceType(), &app_info.qos_info);
		
		return_val = MAPI_MMIAPIPDP_Active(&app_info);
		MCFAPI_SOCKET_PRINT(ALOG_INFO,MLOG_SDKSOCKET, __MCR_FUNCTION__"zhu GPRS McfDtcnt_ActiveGprs apn = %s", APNcn);
	}
	
	MCFAPI_SOCKET_PRINT(ALOG_INFO,MLOG_SDKSOCKET, __MCR_FUNCTION__"zhu GPRS McfDtcnt_ActiveGprs dual_sys = %d, return_val = %d", dual_sys, return_val);
	if(return_val)
	{
		return MTRUE;
	}
	else
	{
		return MFALSE;
	}
}
#endif

