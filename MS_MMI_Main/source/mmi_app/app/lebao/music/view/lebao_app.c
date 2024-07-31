#include "port_cfg.h"
#include "lebao_app.h"

#include "std_header.h"
#include "mmk_app.h"
#include "mmi_common.h"
#include "mmk_timer.h"
#include "mmi_module.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#include "mmiset_export.h"
#include "mmisrvaud.h"
#include "mmisrvaud_api.h"

#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif

#ifdef BLUETOOTH_SUPPORT
#include "mmi_default.h"
#include "mmibt_export.h"
#endif

#include "lebao_ctrl.h"
#include "lebao_misc.h"
#include "lebao_win_base.h"

#include "lebao_id.h"
#include "lebao_text.h"
#include "lebao_image.h"
#include "mmiacc_event.h"

#define LEBAO_DEFAULT_VOLUME		3	// 0-5
#if defined(VOLUME_MAX_LEVEL_15)
	const int realVol_tab[] = {0, 1, 3, 7, 11, 15};
#elif defined(VOLUME_MAX_LEVEL_9)
	const int realVol_tab[] = {0, 1, 2, 5, 7, 9};
#else
	const int realVol_tab[] = {0, 1, 2, 3, 4, 5};
#endif

// public variables
uint8 gLebaoEventTimerId = 0;
unsigned char gLebaoCaixinSmsFilter = 0;
unsigned int gLebaoCainxinAppStartTimeValue = 0;
sds gLebaoCainxinAppStartTime = NULL;

// private variables
LOCAL int _lebaoNetId = 0;

// private prototypes
LOCAL void lebao_event_timer_proc(uint8  timer_id, uint32 param);
LOCAL BOOLEAN lebao_pdp_active(void);
LOCAL void lebao_handle_pdp_msg(MMIPDP_CNF_INFO_T *msg_ptr);
LOCAL void lebao_print_current_ringset(void);

// public functions
static uint32 _lebaoLastStartTime = 0;
static int _currentVolume = -1;
static int _lebao_debug_info_output = 1;

extern int lebao_set_ring_or_alarm_flag;
extern void lebao_player_set_next_max_count(const int count);

static void lebao_show_no_network(void)
{
	helper_trace("enter");

	MMIPUB_OpenAlertWarningWin(TEXT_LEBAO_NO_SIM_CARD);
}

void MMIAPIMENU_EnterLebao(void)
{
	uint32 nowTime = timestamp_seconds_int();

	_lebao_debug_info_output = 1;	// 1: enable detail debug info print; 0: disable ouput
	helper_trace("enter");

	// to avoid clicking too fast, it's actually started
	if ((nowTime - _lebaoLastStartTime) <= 2) {
		helper_trace("restart time is too short or abnormal exit, now=%d, last=%d", nowTime, _lebaoLastStartTime);
		return;
	}

	if (lebao_is_running()) { // 避免二次加载 
		helper_trace("lebao is running, close it first!");

		gLebaoCainxinAppStartTimeValue = 0;	// 屏蔽: 退出APP时上传log到服务器.
		lebao_exit();
	}

	_lebaoLastStartTime = nowTime;

#ifdef _WIN32
	if (1) {
		StartLebaoApp();
		return;
	}
#endif

	if (MMIAPIUDISK_UdiskIsRun()) {
		MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
		return;
	}
	else if (MMIAPICC_IsInState(CC_IN_CALL_STATE)) {
		return;
	}

#ifdef WIFI_SUPPORT
	// check the wifi status
	if (lebao_active_network() == TRUE) {
		StartLebaoApp();
		return;
	}
	else
#endif
	{
		uint16 sim_sys = MN_DUAL_SYS_1;
		uint32 sim_num = 0;

		sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
		if (sim_num <= 0) {
			lebao_show_no_network();
			return;
		}

		if (MMIAPISET_GetFlyMode()) {
			MMIPUB_OpenAlertWarningWin(TEXT_LEBAO_CAN_NOT_FLY_MODE);
			return;
		}

		if (lebao_gprs_is_opened() == FALSE) {
			MMIPUB_OpenAlertWarningWin(TEXT_LEBAO_OPEN_GPRS);
			return;
		}

#ifdef FM_SUPPORT
		helper_trace("MMIAPIFM_GetStatus=%d", MMIAPIFM_GetStatus());
		MMIAPIFM_Stop();	// MMIAPIFM_Pause(FALSE);		
		MMIAPIFM_Exit();	// 关闭FM
#endif
        /* 如果存在其它媒体播放器,可以在此检测, 提示或关闭  
    		// handle = MMISRVAUD_GetHandleByName("");
		    MMIPUB_OpenAlertWarningWin(TEXT_LEBAO_TIPS_MUSIC_USING);
		*/

	}
	
	if (lebao_active_network() == FALSE) {
		lebao_show_no_network();
		return;
	}

	StartLebaoApp();
}

/*
 退出前设置 gLebaoCainxinAppStartTimeValue = 0; 
 将关闭log上传服务器动作, Lebao直接关闭并退出,减少等待时间
*/
void MMIAPIMENU_QuitLebao(void)
{
	helper_trace("enter");

	StopLebaoApp();
}

/*
mode = 0, user operation exit
mode = 1, idle timeout and exit
*/
void MMIAPIMENU_real_QuitLebao(int mode)
{
	helper_trace("enter");

	lebao_deactive_network();

	_lebaoLastStartTime = 0;
}

void StartLebaoApp(void)
{
	helper_trace("start, screen=%dx%d.\n", lebao_get_screen_width(), lebao_get_screen_height());

	// overwrite the last setting
	// reduce the default volume when the headset is connected
	if ((_currentVolume < 0) || (_currentVolume > lebao_get_max_volume()))
	{	
		lebao_set_volume(LEBAO_DEFAULT_VOLUME);
	}
	else {
		lebao_set_volume(_currentVolume);
	}

	// maximum count of songs played automatically, 1 ~ 1000， default:5
	lebao_player_set_next_max_count(5);

	// recording mode, 0 : file, 1 : streaming
#if defined(PLATFORM_UMS9117) || defined(PLATFORM_ANTISW3)
	lebao_ctrl_set_record_mode(0);
#else
	lebao_ctrl_set_record_mode(1);
#endif

	// roll the text back and forth, first fix the ctrllabel.c (align)
	lebao_set_label_can_roll();
	
	// start app
	if (gLebaoEventTimerId != 0) {
		MMK_StopTimer(gLebaoEventTimerId);
		gLebaoEventTimerId = 0;
	}
	gLebaoEventTimerId = MMK_CreateTimerCallback(30, lebao_event_timer_proc, 0, TRUE);

	// external relations, continuous ID, non interruptible, for ctrl lib
	lebao_set_res_text_id(TEXT_LEBAO_WELCOME, TEXT_LEBAO_END_TEXT_ID);
	lebao_set_res_image_id(IMAGE_LEBAO_LOGO, IMAGE_LEBAO_VOLUME_5);
	lebao_set_res_ctrl_id(MMI_LEBAO_CTRL_ID_MENU_OPTION, MMI_LEBAO_CTRL_ID_LABEL_CAPTCHA);
	lebao_set_res_win_id(MMI_LEBAO_WIN_ID_SPLASH, MMI_LEBAO_WIN_ID_LOGIN);

	gLebaoCaixinSmsFilter = 0;
	
	lebao_set_app_key("yisai8910", "014025R");	

	lebao_ctrl_init();

	// delete MP3 when the disk space is less than 30kb
	if (lebao_ctrl_can_write_file(CFG_MP3_MIN_FILE_SIZE) != 0) {
		lebao_ctrl_clear_mp3();
	}
	else {
		int maxSize = (CFG_MP3_MAX_FILE_SIZE < (300 * 1024)) ? (300 * 1024) : CFG_MP3_MAX_FILE_SIZE;

		if (lebao_ctrl_can_write_file(maxSize * 2) == 0) {
			lebao_get_config()->mp3MaxFileSize = maxSize;
		}
	}

	lebao_print_current_ringset();
}

void StopLebaoApp(void)
{
	helper_trace("enter");

/*	if (gLebaoEventTimerId != 0) {
		MMK_StopTimer(gLebaoEventTimerId);
		gLebaoEventTimerId = 0;
	} */

	// free all resources, timer, thread, list, window...
	lebao_exit();
}

//-------------------------------audio-------------------------------

// for play file or buffer
// mmisrvaud_api.h
// the priority from high to low is BT -> EARPHONE -> SPEAKER
int lebao_get_audio_srv_route_type(void)
{
	return MMISRVAUD_ROUTE_HANDHOLD|MMISRVAUD_ROUTE_SPEAKER|MMISRVAUD_ROUTE_EARPHONE|MMISRVAUD_ROUTE_BLUETOOTH; // MMISRVAUD_ROUTE_NONE;
}

// for streaming
// audio_api.h, AUDIO_DEVICE_MODE_TYPE_E
int lebao_get_audo_dev_mode(void)
{
#ifdef BLUETOOTH_SUPPORT
	if (MMIAPIBT_GetActiveBTHeadset())
		return AUDIO_DEVICE_MODE_BLUEPHONE; // AUDIO_DEVICE_MODE_HANDFREE
	else
#endif
	{
		if (MMISRVAUD_IsHeadSetPlugIn())
			return AUDIO_DEVICE_MODE_EARPHONE;
		else
			return AUDIO_DEVICE_MODE_HANDFREE;
	}
}

unsigned int lebao_get_audo_play_dev(void)
{
#ifdef BLUETOOTH_SUPPORT
	if (MMIAPIBT_GetActiveBTHeadset()) 
	{
	    MMIBT_HEADSET_TYPE type = MMIAPIBT_GetActiveHeadsetType();
	    
	    helper_trace("select bt output mode %d.", type);
	    if (type == MMIBT_A2DP_HEADSET) {
    		return hA2DPDev;    // AUDIO_DEVICE_MODE_BLUEPHONE; 
        }
        else if (type == MMIBT_HFG_HEADSET) {
            return hPCMDev;
        }
	} 
#endif

	if (MMISRVAUD_IsHeadSetPlugIn())
		return hARMVB;  // AUDIO_DEVICE_MODE_EARPHONE;
	else
		return hARMVB;  // AUDIO_DEVICE_MODE_HANDFREE;
}

// for recording
// audio_api.h, AUDIO_DEVICE_MODE_TYPE_E
int lebao_get_record_dev_mode(void)
{
	return AUDIO_DEVICE_MODE_HANDFREE;
}

//-----------------------------set the volume----------------------------------

// 0 <= vol <= 5
int lebao_set_volume(const int vol)
{
	int realVol;

	_currentVolume = vol;
	realVol = lebao_get_real_volume();

#ifndef SIMULATOR_WATCH
	if (lebao_get_audioHandle() != 0) {
		// TYPE_LEBAO_PLAY_STREAM == 0, TYPE_LEBAO_RECORD_FILE == 3
		if (lebao_get_playType() == 0 || lebao_get_playType() == 3) {
			if (vol == 0) {
				AUDIO_SetDownLinkMute(TRUE);
			}
			else {
				AUDIO_SetDownLinkMute(FALSE);
				AUDIO_SetVolume(realVol);
			}
		}
		else {
			MMISRVAUD_SetVolume(lebao_get_audioHandle(), realVol);
		}
	}
#endif

	//MMISRVAUD_SetVolumeNoHandle
	return 0;
}

// system real volume
int lebao_get_real_volume(void)
{

	if (_currentVolume < 0) _currentVolume = 0;
	else if (_currentVolume > lebao_get_max_volume()) _currentVolume = lebao_get_max_volume();

	return realVol_tab[_currentVolume];
}

// lebao current volume
int lebao_get_volume(void)
{
	return _currentVolume;
}

int lebao_get_max_volume(void)
{
	return 5;
}

int lebao_get_volume_percent(const int vol)
{
	int per = 0;
	
	if (lebao_get_max_volume() > 0) {
		per = vol * 100 / lebao_get_max_volume();
	}

	if (per > 100) per = 100;
	else if (per <= 10) per = 0;

	return per;
}

/* lebao 播放数据时, 定时 1s 检测外部播放设备是否改变,并自动优选选择蓝牙/耳机 */
void lebao_playing_detect_ouput_device(void)	
{
#if 0
	static int diff_count = 0;
	int detect_mode;	

	if ((lebao_player_is_opened() == 0) && lebao_player_is_backend() == 0) { // 已关闭或播放提示音, 不需要检测
		diff_count = 0;
		return;
	}

	if ((lebao_playing_audio_dev_detect >= AUDIO_DEVICE_MODE_MAX) || (lebao_playing_audio_dev_detect < 0)) { // lebao指定的播放设备不存在
		diff_count = 0;
		return;
	}
	if ((lebao_player_get_status() != LEBAO_STATUS_PLAY) || lebao_active_voice_incoming()) { // lebao没有播放, 或存在外部应用中断lebao播放
		diff_count = 0;
		return;
	}

	detect_mode = lebao_get_audo_dev_mode();
	if (detect_mode != lebao_playing_audio_dev_detect) {
		diff_count++;
		if (diff_count >= 2) {
			if ((detect_mode == AUDIO_DEVICE_MODE_BLUEPHONE) || (lebao_playing_audio_dev_detect == AUDIO_DEVICE_MODE_BLUEPHONE)) { // 如果是蓝牙耳机动作, 需要重新播放当前歌曲切换播放外设
				lebao_event_send(EVT_LEBAO_PLAY_STATUS, LEBAO_PAGE_PLAYER, LEBAO_STATUS_REPLAY, 0);
			} 
			else {
				AUDIO_SetDevMode(detect_mode);		// 耳机/外放 可以直接切换
				lebao_playing_audio_dev_detect = detect_mode;
				// lebao_set_volume(lebao_get_volume());
			}

			diff_count = 0;
			helper_trace("select new audio output mode %d.", detect_mode);
		}
	}
	else {
		diff_count = 0;
	}
#endif
}

//-------------------------------ringtone-------------------------------

extern MMISET_CALL_RING_T MMIAPIENVSET_GetCallRingInfo(MN_DUAL_SYS_E dual_sys, uint8 mode_id);

// W307
#if defined(PLATFORM_ANTISW3)
extern BOOLEAN MMIAPIENVSET_SetMoreCallRingInfo(
	MN_DUAL_SYS_E dual_sys,
	MMISET_CALL_MORE_RING_T ring_info,
	uint8 mode_id
);
#else
extern BOOLEAN MMIAPIENVSET_SetMoreCallRingInfo(
	MN_DUAL_SYS_E dual_sys,
	MMISET_CALL_MORE_RING_T* ring_info,
	uint8 mode_id
);
#endif

LOCAL void lebao_print_current_ringset(void)
{
	MMISET_CALL_RING_T call_ring_info = { MMISET_CALL_RING_FIXED, 0, 0 };

	call_ring_info = MMIAPIENVSET_GetCallRingInfo(MMIAPISET_GetActiveSim(), MMIENVSET_GetCurModeId());

	if (MMISET_CALL_RING_MORE_RING == call_ring_info.call_ring_type) {
		char path[256] = { 0 };
		GUI_WstrToUTF8(path, 255, call_ring_info.more_ring_info.name_wstr, call_ring_info.more_ring_info.name_wstr_len);

		if (!MMIAPIFMM_IsFileExist(call_ring_info.more_ring_info.name_wstr, call_ring_info.more_ring_info.name_wstr_len)) {
			helper_trace("not exist, filename = %s", path);
		}
		else {
			helper_trace("ringtone, filename=%s", path);
		}
	}
	else {
		helper_trace("ringtone, type=%d", call_ring_info.call_ring_type);
	}
}

// for set rintone
// int lebao_query_ringtone(MMISET_CALL_MORE_RING_T* filePath, MMISET_CALL_MORE_RING_T* songName);
int lebao_set_ringtone(const char* filename)
{
	BOOLEAN ret = FALSE;
	MMISET_CALL_MORE_RING_T ring_info = { 0 };
	int len = (filename == NULL) ? 0 : os_strlen(filename);
	
	if (len <= 0 || len > MMISET_CALL_RING_NAME_MAX_LEN)
		return -1;

	ring_info.name_wstr_len = GUI_GBToWstr(ring_info.name_wstr, (const uint8*)filename, len);
	if (ring_info.name_wstr_len <= 0) return -1;

	if (!MMIAPIFMM_IsFileExist(ring_info.name_wstr, ring_info.name_wstr_len)) {
		helper_trace("not exist, filename = %s", filename);
		// return -1;
	}
	
	if (lebao_set_ring_or_alarm_flag == 0)
	{	
#if defined(PLATFORM_ANTISW3)
		ret = MMIAPIENVSET_SetMoreCallRingInfo(MMIAPISET_GetActiveSim(), ring_info, MMIENVSET_GetCurModeId());
#else
		ret = MMIAPIENVSET_SetMoreCallRingInfo(MMIAPISET_GetActiveSim(), &ring_info, MMIENVSET_GetCurModeId());
#endif
		helper_trace("set ringtone %s, filename = %s", (ret == TRUE) ? "ok" : "failed", filename);
	}
	else
	{
		uint16 alarm_id;
		
		for(alarm_id=EVENT_ALARM_0; alarm_id<= EVENT_ALARM_MAX; alarm_id++)
		{
			MMIAPIFMM_SetMusicToRing(MMIMULTIM_SET_ALARM_RING, (DPARAM)&alarm_id, ring_info.name_wstr, ring_info.name_wstr_len);
		}
		for(alarm_id=EVENT_ALARM_DELAY_0; alarm_id<= EVENT_ALARM_DELAY_MAX; alarm_id++)
		{
			// MMIAPIFMM_SetMusicToRing(MMIMULTIM_SET_ALARM_RING, (DPARAM)&alarm_id, ring_info.name_wstr, ring_info.name_wstr_len);
		}
	}

	lebao_event_send(EVT_LEBAO_RING_RESULT, LEBAO_PAGE_PLAYER, ret, NULL);
	return 0; // ((ret == TRUE) ? 0 : -1)
}

int lebao_query_ringtone(MMISET_CALL_MORE_RING_T* filePath, MMISET_CALL_MORE_RING_T* songName)
{
	int i = 0;
	link_list_node_t* node = NULL;
	song_info_t* song = NULL;

	link_list_t * localSongs = lebao_get_playlist();
	if (localSongs == NULL || localSongs->len <= 0)
		return -1;

	for (i = 0; i < (int)localSongs->len; i++) {
		node = link_list_at(localSongs, i);
		song = node->val;
		if (song == NULL || song->contentId == NULL || song->ringListenUrl == NULL) 
			continue;

		if (helper_is_local_file(song->ringListenUrl) && helper_is_file_exist(song->ringListenUrl)) {
			if(filePath != NULL && sdslen(song->ringListenUrl) <= MMISET_CALL_RING_NAME_MAX_LEN) {
				filePath->name_wstr_len = GUI_GBToWstr(filePath->name_wstr, 
					(const uint8*)song->ringListenUrl, sdslen(song->ringListenUrl));
			}

			if(songName != NULL && song->songName != NULL && sdslen(song->songName) <= MMISET_CALL_RING_NAME_MAX_LEN) {
				songName->name_wstr_len = GUI_UTF8ToWstr(songName->name_wstr,
					MMISET_CALL_RING_NAME_MAX_LEN,
					(const uint8*)song->songName, sdslen(song->songName));				
			}
			return 0;
		}
	}

	return -1;
}


//-------------------------------network-------------------------------

#if defined(MULTI_SIM_SYS_SINGLE)
MN_DUAL_SYS_E lebao_get_active_sim(void)
{
	return MMIAPISET_GetActiveSim();
}
#else
MN_DUAL_SYS_E lebao_get_active_sim(void)
{
	MN_DUAL_SYS_E dualSysUse = MN_DUAL_SYS_1;

	if (!(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)) && !(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2)))
		return MMIAPISET_GetActiveSim();

	if (MMIAPIPHONE_GetSimExistedStatus(dualSysUse) && MMIAPIPHONE_GetDataServiceSIM(&dualSysUse))
		return dualSysUse;

	dualSysUse = MN_DUAL_SYS_2;
	if (MMIAPIPHONE_GetSimExistedStatus(dualSysUse) && MMIAPIPHONE_GetDataServiceSIM(&dualSysUse))
		return dualSysUse;

	return MMIAPISET_GetActiveSim();
}
#endif

int lebao_get_net_id(void)
{
	return _lebaoNetId;
}

#if 0
extern void WatchWIFI_MainWin_Enter(void);
#endif

BOOLEAN lebao_active_network(void)
{
	BOOLEAN result = FALSE;

	_lebaoNetId = 0;
	
#ifdef WIFI_SUPPORT
	// priority  wifi -> ps
	if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
		return TRUE;

	do {
#if defined(MULTI_SIM_SYS_SINGLE)
		if (MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1))
			break;
#else
		if (MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1) || MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_2))
			break;
#endif
		else {
			// only wifi
			#if !defined(PLATFORM_ANTISW3)
				MMIPDP_ACTIVE_INFO_T active_info = { 0 };
				active_info.app_handler = MMI_MODULE_LEBAO;
				active_info.handle_msg_callback = lebao_handle_pdp_msg;
				active_info.ps_interface = MMIPDP_INTERFACE_WIFI;
				result = MMIAPIPDP_Active(&active_info);
			#else
				WatchWIFI_MainWin_Enter();
				//WatchSET_MainWin_Enter();
			#endif
		}
	} while (0);
#endif

#if 0
	if (MMIAPIPDP_PsIsPdpLinkExist())
		return TRUE;
#endif

	if(result == FALSE)
		result = lebao_pdp_active();

	return result;
}

BOOLEAN lebao_deactive_network(void)
{
	_lebaoNetId = 0;
	return MMIAPIPDP_Deactive(MMI_MODULE_LEBAO);
}

BOOLEAN lebao_network_is_connected(void)
{
#ifndef _WIN32
	#ifdef WIFI_SUPPORT
	if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
		return TRUE;
	#endif

	return MMIAPIPDP_PsIsActivedPdpLinkExist();
#else
	return TRUE;
#endif
}

// private functions
LOCAL void lebao_event_timer_proc(uint8 timer_id, uint32 param)
{
	lebao_event_task_run(NULL);
}

LOCAL void lebao_handle_pdp_msg(MMIPDP_CNF_INFO_T *msg_ptr)
{
	if (PNULL == msg_ptr) {
		return;
	}

	helper_trace("nsapi = %d, msg_id = %d, app = %d", 
		msg_ptr->nsapi,
		msg_ptr->msg_id,
		msg_ptr->app_handler);

	switch (msg_ptr->msg_id)
	{
	case MMIPDP_ACTIVE_CNF:
		if (MMIPDP_RESULT_SUCC == msg_ptr->result) {
			_lebaoNetId = msg_ptr->nsapi;
		}
		else {
			lebao_deactive_network();
#ifdef WIFI_SUPPORT
			if (MMIPDP_INTERFACE_WIFI == msg_ptr->ps_interface) {
				// Todo : change to GPRS
			}
#endif
		}
		break;

	case MMIPDP_DEACTIVE_CNF:
		break;

	case MMIPDP_DEACTIVE_IND: {
		lebao_deactive_network();
		break;
	}
	
	default:
		break;
	}

	MMI_CheckAllocatedMemInfo();
}

LOCAL BOOLEAN lebao_pdp_active(void)
{
	MN_DUAL_SYS_E dual_sys = lebao_get_active_sim();
	MMIPDP_ACTIVE_INFO_T    active_info = { 0 };
	BOOLEAN                 result = FALSE;
	MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;

#if 0 // !defined(MULTI_SIM_SYS_SINGLE)
	{
		MN_DUAL_SYS_E sim_num = MMIAPISET_GetDataSimFromNV();

		if ((sim_num == MN_DUAL_SYS_1) || (sim_num == MN_DUAL_SYS_2))
		{
			dual_sys = sim_num;
		}
	}
#endif

	linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, 0);

	if(linksetting == NULL) {
		helper_trace("MMIAPICONNECTION_GetLinkSettingItemByIndex, idx=%d, failed.", dual_sys);
		return FALSE;
	}

	active_info.app_handler = MMI_MODULE_LEBAO;
	active_info.dual_sys = dual_sys;
	active_info.apn_ptr =  (char*)linksetting->apn;
	active_info.user_name_ptr = (char*)linksetting->username;
	active_info.psw_ptr = (char*)linksetting->password;
	active_info.priority = 3;
	active_info.ps_service_rat = MN_UNSPECIFIED;
	active_info.ps_interface = MMIPDP_INTERFACE_GPRS;
	active_info.handle_msg_callback = lebao_handle_pdp_msg;
	active_info.ps_service_type = BROWSER_E;
	active_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
	active_info.ip_type = MMICONNECTION_IP_V4;//linksetting->ip_type;
#endif
	if (MMIAPIPDP_Active(&active_info)) {
		result = TRUE;
	}

	helper_trace("lebao_pdp_active:sim=%d, result=%d", dual_sys, result);

	return result;
}


BOOLEAN lebao_gprs_is_opened(void)
{
#ifdef MMI_GPRS_SUPPORT
	if (MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_OFF)
		return FALSE;
#endif
	return TRUE;
}

//-------------------------------sms-------------------------------

#include "mmisms_send.h"

extern int lebao_order_set_captcha(const char* captcha);

// sms received
static BOOLEAN _lebao_caixin_sms_captcha_filter(DPARAM param)
{
	APP_MN_SMS_IND_T *sigPtr = (APP_MN_SMS_IND_T *)param;
	APP_SMS_USER_DATA_T smsData = { 0 };
	MMIPB_BCD_NUMBER_T origNum = { 0 };

	BOOLEAN isTarget = FALSE;
	BOOLEAN findNameTag = FALSE;
	BOOLEAN findDataTag = FALSE;

	char captcha[7] = { 0 };
	uint8 tele[128] = { 0 };
	uint8 tele_len = 0;
	int i, j, begin = 0;

	const uint8  miguyinyue[14] = { 0x54, 0xaa, 0x54, 0x95, 0x97, 0xf3, 0x4e, 0x50, 0x52, 0x9F, 0x80, 0xFD, 0x67, 0x3A }; // 咪咕音乐功能机
	const uint8  yanzhengma[6] = { 0x9a, 0x8c, 0x8b, 0xc1, 0x78, 0x01 }; // 验证码

	origNum.number_len = sigPtr->sms_t.origin_addr_t.num_len;
	origNum.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(sigPtr->sms_t.origin_addr_t.number_type, MN_NUM_PLAN_UNKNOW);
	SCI_MEMCPY(&origNum.number, &sigPtr->sms_t.origin_addr_t.party_num, origNum.number_len);
	tele_len = MMIAPIPB_BCDNumberToString(&origNum, 128, tele);

	MMISMS_DecodeUserHeadAndContent(
		sigPtr->dual_sys,
		sigPtr->sms_t.user_head_is_exist,
		&(sigPtr->sms_t.dcs),
		&(sigPtr->sms_t.user_data_t),
		&smsData);
	
	if (smsData.user_valid_data_t.length <= 14) {
		return isTarget;
	}

	helper_trace("caller=%s, length=%d", tele, smsData.user_valid_data_t.length);

	for (i = 0; i < (smsData.user_valid_data_t.length - 14); i++)
	{
		if (findNameTag == TRUE && findDataTag == TRUE) {
			break;
		}

		if (!findNameTag && (os_memcmp(&smsData.user_valid_data_t.user_valid_data_arr[i], miguyinyue, 14) == 0)) {
			findNameTag = TRUE;
			continue;
		}
		if (!findDataTag && (os_memcmp(&smsData.user_valid_data_t.user_valid_data_arr[i], yanzhengma, 6) == 0)) {
			findDataTag = TRUE;
			begin = i + 5 + 2;
			continue;
		}
	}

	if (findNameTag == TRUE && findDataTag == TRUE) 
	{
		for (j = 0; (j < 6) && (begin < smsData.user_valid_data_t.length); begin += 2) 
		{
			uint8 c = smsData.user_valid_data_t.user_valid_data_arr[begin];

			if ((c < '0') || (c > '9')) {
				if (j > 0) {
					break;
				}
				continue;
			}

			captcha[j] = c;
			j++;
		}

		captcha[6] = '\0';
		helper_trace("find captcha=%s", captcha);
		isTarget = TRUE;
		lebao_order_set_captcha(captcha);
	}

	return isTarget;
}

BOOLEAN lebao_sms_captcha_filter(DPARAM param)
{
		if (gLebaoCaixinSmsFilter)
		{
			return _lebao_caixin_sms_captcha_filter(param);
		}
		return FALSE;
}

static BOOLEAN  _lebao_send_sms_status = 0;

static BOOLEAN lebao_SMS_SEND_CALLBACK(BOOLEAN is_succss, DPARAM parm)
{
    _lebao_send_sms_status = 0;
    
	helper_trace("succssed=%d", is_succss);
	return TRUE;
}

int lebao_send_sms(const MN_DUAL_SYS_E dualSysUse, const char* called, const char* content)
{
	MMI_STRING_T strContent = { 0 };
	wchar w_sms[80] = { 0 };
	uint16 sms_len = 0;	
	MMISMS_SEND_DATA_T sendData = { 0 };
	BOOLEAN result = FALSE;

	if (called == NULL || content == NULL)
		return -1;

	helper_trace("called=%s, content=%s", called, content);

	sms_len = os_strlen(content);
	if ((sms_len <= 0) || (sms_len > 79)) {
		helper_trace("content len=%d, skip send sms",sms_len);
		return -1;
	}

	// content
	SCI_MEMSET(w_sms, 0, sizeof(w_sms));
	MMIAPICOM_StrToWstr(content, w_sms);
	sms_len = MMIAPICOM_Wstrlen(w_sms);
	if (sms_len <= 0)
		return -1;
    
    _lebao_send_sms_status = TRUE;
    
	strContent.wstr_ptr = w_sms;
	strContent.wstr_len = sms_len;

	sendData.sms_content_ptr = &strContent;
	sendData.dst_port = 0;
	sendData.src_port = 0;
	sendData.dest_addr_ptr = (uint8 *)called;
	sendData.dest_addr_len = os_strlen(called);
	sendData.dual_sys = dualSysUse;
	sendData.is_need_packet = TRUE;
	sendData.send_callback = lebao_SMS_SEND_CALLBACK;

	result = MMISMS_SendSmsForOther(&sendData);
	return (result == TRUE ? 0 : 1);
}

int lebao_is_sending_sms(void)
{
    return _lebao_send_sms_status;
}

//-------------------------------call-------------------------------

int lebao_make_call(const MN_DUAL_SYS_E dualSysUse, const char* called)
{
	if (called == NULL) return -1;

	MMIAPIMENU_QuitLebao();
	MMK_ReturnIdleWin();

	return MMIAPICC_MakeCall(
		dualSysUse,
		called,
		os_strlen(called),
		PNULL,
		PNULL,
		MMIAPICC_GetCallSimType(dualSysUse),
		CC_CALL_NORMAL_CALL,
		PNULL);
}


int lebao_active_voice_incoming(void)
{
	unsigned int sid = MMIAPIALM_GetAlarmPromptWinId();

	if (MMIALM_HasActiveEvent() && MMK_IsFocusWin(sid)) {
		return 1;
	}
	if (MMICC_IsExistIncommingCall() || MMICC_IsExistActiveCall() || MMICC_IsExistHoldCall() || MMICC_IsExistOperatedCall()) {
		return 1;
	}

	return 0;
}

/*
	player_status = 1, 在 lebao_win_player 播放界面中调用, 当前空操作, 不需要继续处理
	player_status = 0, 在其它页面/状态下调用, 需要判断处理 
*/
int lebao_playing_enter_stop_state(int player_status)
{
	if (lebao_is_running() && lebao_player_is_opened())
	{
		int status = lebao_player_get_status();

		if (player_status == 1){ 
			// 从 play 前台界面中(消息MSG_LOSE_FOCUS)调用; 如果后续的系统消息可以继续处理播放暂停/停止,可以跳过此处避免重复处理.
			return 0;
		}
		
		helper_trace("break off play");

		status = lebao_player_get_status();
		
		if (status == LEBAO_STATUS_PLAY) {
			lebao_ctrl_pause_music(0, 0, NULL);
		}
		else if (status == LEBAO_STATUS_PAUSE) {

		}
		else { // 状态可能是在切换下一首连续播放, 需要 stop .
			lebao_player_stop_playing();
		}
	}
	return 0;
}

void lebao_open_bt_device(void)
{
#ifdef BLUETOOTH_SUPPORT
	if (lebao_playing_audio_dev_detect == AUDIO_DEVICE_MODE_BLUEPHONE) {
		MMIBT_OpenDevice(MMISRVAUD_SAMPLE_RATE_DEFAULT);
	}
#endif
}

void lebao_close_bt_device(void)
{
#ifdef BLUETOOTH_SUPPORT
	if (lebao_playing_audio_dev_detect == AUDIO_DEVICE_MODE_BLUEPHONE) {
		MMIBT_StopDevice(MMIBT_A2DP_AUDIO_PLAYER);
	}
#endif
}

void lebao_helper_log_printf(const char* fmt, ...)
{
#ifdef WIN32	// WIN32, LINUX, MacOS
	va_list ap;
	int   len;
	char buf[256];

	va_start(ap, fmt);
	len = vsnprintf(buf, sizeof(buf) - 2, fmt, ap);
	va_end(ap);
	
	if ((len <= 0) || (len > (sizeof(buf) - 2))) {
		return;
	}

	buf[len] = 0;
	helper_log_printf("%s", buf);
#else
  #ifdef   SCI_TRACE_MODE
	if (_lebao_debug_info_output)
	{
		va_list ap;
		int   len;
		char buf[256];

		va_start(ap, fmt);
		len = vsnprintf(buf, sizeof(buf) - 2, fmt, ap);
		va_end(ap);

		if ((len <= 0) || (len > (sizeof(buf) - 2))) {
			return;
		}

		buf[len] = 0;
		SCI_TraceLow("{%s} %s", "lebao", buf);
	}
  #endif
#endif
}
