#include "sci_types.h"
#include "threadx_os.h"
#include "os_apiext.h"
#include "block_mem.h"
#include "mmidc_export.h"
//#include  "mmiaudio_ctrl.h"
#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"
#include "mmiidle_export.h"
#include "freq_cfg.h"
#include "os_api.h"
#include "mmicc_export.h"
#include "mmk_timer.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmisd_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmisms_id.h"
#include "mmiudisk_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmifm_export.h"
#include "mmitts_export.h"
#include "setjmp.h" 
#include "priority_app.h"
#include "mmipb_id.h"

#include "mmi_id.h"
#include "mmicc_id.h"
#include "mmienvset_export.h"
#if defined BROWSER_SUPPORT
#include "mmibrowser_id.h"
#endif
#include "mmiset_id.h"
#include "mmialarm_id.h"

#include "mn_type.h"
#include "mmisms_app.h"
#include "heroEngine_mmi_text.h"
#include "heroEngineApi.h"

//#include "IN_message.h"

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#include "mmiacc_id.h"
#include "mmiim_id.h"

#include "audio_config.h"
#ifndef _WIN32
#include "audio_api.h"
#else
//#include "mmiaudio_simu.h"
#endif
#include "generic_tone_adp.h"
#include "custom_tone_adp.h"
#include "dtmf_tone_adp.h"
#include "pcm_adp.h"

#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmirecord_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmiaudio_ctrl.h"
#include "wav_adp.h"

#include "mmitts_export.h"

#include "smartdualsim_id.h"
#include "mmimp3_id.h"
#include "mmifmm_id.h"
#include "mmimp3_export.h"
#include "mmieng_id.h"

#include "synthtext.h"
#include "hero_tts_app.h"

#define TIHO_TTS_PCM_BUF_SIZE         (24 * 1024)
#define TIHO_TTS_PCM_PLAYER_BUF_SIZE  (16 * 1024)
#define TIHO_TTS_STRING_BUF_SIZE      (2 * 1024)

enum {
	TIHO_TTS_PLAYER_IDLE,
	TIHO_TTS_PLAYER_PLAYING,
};

typedef struct tagHeroTtsContextSt{
	uint32 heroTtsRunThreadId;
	SCI_SEMAPHORE_PTR ttsStopSemaphore;

	MMISRV_HANDLE_T audioHandle;

	unsigned char ttsstatus;
	unsigned char taskIsStop;
	unsigned char ttsisrunning;
	unsigned char currentPlayerIndex;

	int ttsTimerId;
	int ttsisCall;

	void *userdata;
	hero_tts_play_callback callback;

	unsigned short stringsize;
	unsigned char stringbuf[TIHO_TTS_STRING_BUF_SIZE];

	//playerbuf
	char *pcmPlayerBufMap[2];

	//pcm buf
	char *heroTtsCycleBuffer;
	uint32 heroTtsCycleBufferLen;
	uint32 heroTtsPutPosition;
	uint32 heroTtsGetPosition;
}HeroTtsContextSt;

static uint16 gAppendTxtSize = 0;
static uint8 gAppendTxtBuf[512] = {0};

static void hero_tts_task_exe(HeroTtsContextSt *pContext);

static int hero_tts_send_msg_to_ui(int msgId);

void hero_tts_get_res_dir(char *outBuf)
{
}

int hero_tts_check_res(void)
{
	return 1;
}

int hero_tts_open_res()
{
	return 0;
}

void hero_tts_close_res(int handle)
{
}

int hero_tts_res_read(int handle, int pos, void *p, unsigned int l)
{
	return 0;
}

static HeroTtsContextSt *hero_tts_get_context()
{
	static unsigned char gIsInit = 0;
	static HeroTtsContextSt sHeroTtsContext = {0};
	if(!gIsInit){
		memset(&sHeroTtsContext, 0, sizeof(HeroTtsContextSt));
		sHeroTtsContext.heroTtsRunThreadId = SCI_INVALID_BLOCK_ID;
		sHeroTtsContext.ttsStopSemaphore = SCI_CreateSemaphore("ttsstops", 0);
		gIsInit = 1;
	}
	return &sHeroTtsContext;
}

static uint32 heroTtsCycleBuffeGetEmptyNum(HeroTtsContextSt *pContext)
{
	if (pContext->heroTtsPutPosition >= pContext->heroTtsGetPosition){
		return pContext->heroTtsCycleBufferLen - (pContext->heroTtsPutPosition - pContext->heroTtsGetPosition) - 1;
	}
	return pContext->heroTtsGetPosition - pContext->heroTtsPutPosition - 1;
}

static uint32 heroTtsCycleBuffeGetUsedNum(HeroTtsContextSt *pContext)
{
	uint32 len = 0;

	if (pContext->heroTtsPutPosition > pContext->heroTtsGetPosition)
	{
		len = pContext->heroTtsPutPosition - pContext->heroTtsGetPosition;
	}
	else if(pContext->heroTtsGetPosition > pContext->heroTtsPutPosition)
	{
		len = pContext->heroTtsCycleBufferLen - (pContext->heroTtsGetPosition - pContext->heroTtsPutPosition);
	}
	return len;
}

static BOOLEAN heroTtsCycleBufferPut(HeroTtsContextSt *pContext, char* Buffer,uint32 BufferLen)
{
	BOOLEAN ret = FALSE;

	if (heroTtsCycleBuffeGetEmptyNum(pContext) > BufferLen)
	{
		uint32 i = pContext->heroTtsCycleBufferLen - pContext->heroTtsPutPosition;
		if (BufferLen + pContext->heroTtsPutPosition > pContext->heroTtsCycleBufferLen) 
		{
			SCI_MEMCPY(pContext->heroTtsCycleBuffer + pContext->heroTtsPutPosition, Buffer, i);
			SCI_MEMCPY(pContext->heroTtsCycleBuffer, Buffer + i, BufferLen - i);
		}
		else
		{
			SCI_MEMCPY(pContext->heroTtsCycleBuffer + pContext->heroTtsPutPosition, Buffer, BufferLen);
		}
		pContext->heroTtsPutPosition = (BufferLen + pContext->heroTtsPutPosition) % pContext->heroTtsCycleBufferLen;
		ret = TRUE;
	}   
	return ret;
}

static BOOLEAN heroTtsCycleBufferGet(HeroTtsContextSt *pContext, char* Buffer, uint32 *BufferLen_p)
{
	BOOLEAN ret = FALSE;
	uint32 activeNum = heroTtsCycleBuffeGetUsedNum(pContext);

	if (0 < activeNum)
	{
		uint32 i = pContext->heroTtsCycleBufferLen - pContext->heroTtsGetPosition;
		uint32 outLen = MIN(*BufferLen_p, activeNum);

		if (outLen + pContext->heroTtsGetPosition > pContext->heroTtsCycleBufferLen) 
		{
			SCI_MEMCPY(Buffer, pContext->heroTtsCycleBuffer + pContext->heroTtsGetPosition, i);
			SCI_MEMCPY(Buffer+i, pContext->heroTtsCycleBuffer, outLen-i);
		}
		else
		{
			SCI_MEMCPY(Buffer, pContext->heroTtsCycleBuffer + pContext->heroTtsGetPosition, outLen);
		}

		*BufferLen_p = outLen;
		pContext->heroTtsGetPosition = (pContext->heroTtsGetPosition + outLen) % pContext->heroTtsCycleBufferLen;
		ret = TRUE;
	}
	return ret;
}

enum {
	MSG_ID_TTS_TASK_START = 0x1,
	MSG_ID_TTS_TASK_STOP,
	MSG_ID_TTS_END
};
typedef struct tagHeroTtsTaskMsgSt{
	SIGNAL_VARS
}HeroTtsTaskMsgSt;
static void hero_tts_thread_callback(uint32 param, void* obj_ptr)
{
	HeroTtsTaskMsgSt signal;
	HeroTtsTaskMsgSt *sig_ptr = NULL;
	uint32 task_id = SCI_IdentifyThread();
	HeroTtsContextSt *pContext = (HeroTtsContextSt*)obj_ptr;

	while(1)
	{
		if(pContext == NULL){
			break;
		}
		sig_ptr = (HeroTtsTaskMsgSt *)SCI_GetSignal(task_id);
		signal = *sig_ptr;
		SCI_FREE(sig_ptr);

		LOG_DEBUG("hero_tts_thread_callback code = %d", signal.SignalCode);
		if(signal.SignalCode == MSG_ID_TTS_TASK_START) {
			hero_tts_task_exe(pContext);
		} else if(signal.SignalCode == MSG_ID_TTS_TASK_STOP) {
			uint32 sRet = SCI_PutSemaphore(pContext->ttsStopSemaphore);
			SCI_ASSERT(sRet == SCI_SUCCESS);
			gAppendTxtSize = 0;
		}
	}
}

static int hero_tts_send_msg_to_task(HeroTtsContextSt *pContext, int msgId)
{
	HeroTtsTaskMsgSt *sig_ptr = NULL;

	if(pContext->heroTtsRunThreadId == SCI_INVALID_BLOCK_ID)
	{
		pContext->heroTtsRunThreadId = SCI_CreateAppThread("herottsthd", "herottsthd",
			hero_tts_thread_callback,
			0,
			(void*)pContext,
			8 * 1024,
			10,
			TX_USER_NORMALE,
			SCI_PREEMPT,
			SCI_AUTO_START);
	}
	LOG_DEBUG("hero_tts_send_msg_to_task pContext->heroTtsRunThreadId = %d", pContext->heroTtsRunThreadId);
	if(pContext->heroTtsRunThreadId == SCI_INVALID_BLOCK_ID) {
		return 0;
	}

	if(SCI_QUEUE_FULL == SCI_IsThreadQueueAvilable(pContext->heroTtsRunThreadId)) {
		return 0;
	}
	sig_ptr = (HeroTtsTaskMsgSt *)SCI_ALLOC_APP(sizeof(HeroTtsTaskMsgSt));
	SCI_MEMSET(sig_ptr, 0, sizeof(HeroTtsTaskMsgSt));

	sig_ptr->SignalCode = msgId;
	if (SCI_SUCCESS != SCI_SendSignal((xSignalHeader)sig_ptr, pContext->heroTtsRunThreadId)) {
		SCI_ASSERT(FALSE); /*assert verified*/
	}

	return 1;
}

void tiho_tts_audio_send_data(void *data, int dataSize)
{
	HeroTtsContextSt *pContext = hero_tts_get_context();

	LOG_DEBUG("tiho_tts_audio_send_data = %d, data = %d", dataSize, data);
	if(pContext->taskIsStop || pContext->heroTtsCycleBuffer == NULL)
	{
		LOG_DEBUG("task stop ...");
		tiho_tts_stop();
		return;
	}

	LOG_DEBUG("tiho_tts_audio_send_data used size = %d, pContext->ttsstatus = %d, dataSize = %d", heroTtsCycleBuffeGetUsedNum(pContext), pContext->ttsstatus, dataSize);
	if(heroTtsCycleBuffeGetUsedNum(pContext) >= 8 * 1024 || dataSize == 0)
	{
		if(TIHO_TTS_PLAYER_IDLE == pContext->ttsstatus) //start player
		{
			if(hero_tts_send_msg_to_ui(hero_tts_convert_moudle_id(HERO_TTS_MOUDLE_ID_START_PCM))) {
				pContext->ttsstatus = TIHO_TTS_PLAYER_PLAYING;
			}
		}
	}

	if(data != NULL && dataSize > 0)
	{
		while(heroTtsCycleBuffeGetEmptyNum(pContext) < dataSize)
		{
			LOG_DEBUG("tiho_tts_audio_send_data empty size = %d, dataSize = %d", heroTtsCycleBuffeGetEmptyNum(pContext), dataSize);
			if(pContext->taskIsStop){
				return;
			}
			SCI_Sleep(50);
		}
	    heroTtsCycleBufferPut(pContext, (char*)data, dataSize);
	}
}

int hero_app_get_tts_status();
static int hero_tts_start_task(char *data, int size, hero_tts_play_callback callback, void *userdata)
{
	int ret = 0;
	HeroTtsContextSt *pContext = hero_tts_get_context();
	do
	{
		int index = 0;

		LOG_DEBUG("hero_tts_start_task = %d, size = %d, handle = %d", data, size, pContext->audioHandle);
		if(data == NULL || size == 0) {
			break;
		}

		//if(hero_app_get_tts_status()){
		//	break;
		//}

		if(pContext->audioHandle != 0)
		{
			MMISRVAUD_Stop(pContext->audioHandle);
			MMISRVMGR_Free(pContext->audioHandle);
			pContext->audioHandle = 0;
		}

		pContext->userdata = userdata;
		pContext->callback = callback;

		pContext->stringsize = MIN(size, TIHO_TTS_STRING_BUF_SIZE - 2);
		if((unsigned char*)data != pContext->stringbuf) {
			memset(pContext->stringbuf, 0, TIHO_TTS_STRING_BUF_SIZE);
			memcpy(pContext->stringbuf, data, pContext->stringsize);
		}

		if(pContext->heroTtsCycleBuffer == NULL) {
			pContext->heroTtsCycleBuffer = hero_tts_malloc(TIHO_TTS_PCM_BUF_SIZE);
		}
		LOG_DEBUG("hero_tts_start_task heroTtsCycleBuffer = %d", pContext->heroTtsCycleBuffer);
		if(pContext->heroTtsCycleBuffer == NULL) {
			break;
		}
		pContext->heroTtsPutPosition = 0;
		pContext->heroTtsGetPosition = 0;
		pContext->heroTtsCycleBufferLen = TIHO_TTS_PCM_BUF_SIZE;

		pContext->currentPlayerIndex = 0;
		for(index = 0; index < sizeof(pContext->pcmPlayerBufMap)/sizeof(pContext->pcmPlayerBufMap[0]); index++)
		{
			if(pContext->pcmPlayerBufMap[index] == NULL)
			{
				pContext->pcmPlayerBufMap[index] = hero_tts_malloc(TIHO_TTS_PCM_PLAYER_BUF_SIZE);
				LOG_DEBUG("hero_tts_start_task pcmPlayerBufMap[%d] = %d", index, pContext->pcmPlayerBufMap[index]);
				if(pContext->pcmPlayerBufMap[index] == NULL){
					break;
				}
			}
		}
		LOG_DEBUG("hero_tts_start_task map index = %d", index);
		if(index != sizeof(pContext->pcmPlayerBufMap)/sizeof(pContext->pcmPlayerBufMap[0])){
			break;
		}

		pContext->taskIsStop = 0;
		pContext->ttsisrunning = 1;
		if(!hero_tts_send_msg_to_task(pContext, MSG_ID_TTS_TASK_START)){
			pContext->ttsisrunning = 0;
			break;
		}

		ret = 1;
	}while(0);
	LOG_DEBUG("hero_tts_start_task ret = %d...", ret);
	if(!ret) {
		pContext->ttsisrunning = 1;
		hero_tts_stop_play();
	}
	return ret;
}

static void hero_tts_task_exe(HeroTtsContextSt *pContext)
{
	LOG_DEBUG("tiho_tts_start taskIsStop = %d", pContext->taskIsStop);
	if(pContext->taskIsStop) {
		pContext->taskIsStop = 0;
		return;
	}
	if(!tiho_tts_start((unsigned short*)pContext->stringbuf, pContext->stringsize))
	{
		tiho_tts_stop();
		hero_tts_send_msg_to_ui(hero_tts_convert_moudle_id(HERO_TTS_MOUDLE_ID_CALLBACK));
		LOG_DEBUG("tiho_tts_start error!!!!!!!!");
		return;
	}

	LOG_DEBUG("tiho_tts_start end ...");
	tiho_tts_audio_send_data(NULL, 0);

	/// append txt
	if(gAppendTxtSize > 0 && !pContext->taskIsStop)
	{
		if(!tiho_tts_start((unsigned short*)gAppendTxtBuf, gAppendTxtSize))
		{
			tiho_tts_stop();
			hero_tts_send_msg_to_ui(hero_tts_convert_moudle_id(HERO_TTS_MOUDLE_ID_CALLBACK));
		}
	}
	gAppendTxtSize = 0;
}

LOCAL const WAV_STREAM_BUFFER_T heroTtsHeadInfo = {SCI_TRUE, 0, 0, 0, WAV_PCM_BIGENDIAN, 8000, 0x10, 1, sizeof(WAV_STREAM_BUFFER_T), 84, NULL};//IMPORTANT!!!

PUBLIC BOOLEAN heroTtsHandleNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
	MMISRVAUD_REPORT_T *report_ptr = PNULL;
	HeroTtsContextSt *pContext = hero_tts_get_context();

	LOG_DEBUG("heroTtsHandleNotify audioHandle = %d, param = %d, handle = %d", pContext->audioHandle, param, handle);
	if(param != PNULL && handle > 0)
	{
		report_ptr = (MMISRVAUD_REPORT_T *)param->data;
		LOG_DEBUG("heroTtsHandleNotify report_ptr = %d", report_ptr);
		if(report_ptr != PNULL)
		{
			LOG_DEBUG("heroTtsHandleNotify report_ptr->report = %d", report_ptr->report);
			switch(report_ptr->report)
			{
				case MMISRVAUD_REPORT_END:
					hero_tts_send_msg_to_ui(hero_tts_convert_moudle_id(HERO_TTS_MOUDLE_ID_CALLBACK));
				break;
				default:
				break;
			}
		}
		return TRUE;
	}
	return FALSE;
}

static AUDIO_STREAM_GET_SRC_RESULT_E heroTtsPcmStreamPlayCallback(HAUDIO hAudio, AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo, void *pvOtherInfo )
{
	unsigned char currentIndex = 0; uint32 usedDataLen = 0;
	HeroTtsContextSt *pContext = hero_tts_get_context();

	LOG_DEBUG("heroTtsPcmStreamPlayCallback pContext->heroTtsCycleBuffer = %d", pContext->heroTtsCycleBuffer);
	if(pContext->heroTtsCycleBuffer == NULL){
		return AUDIO_STREAM_GET_SRC_ERROR;
	}

	currentIndex = pContext->currentPlayerIndex%(sizeof(pContext->pcmPlayerBufMap)/sizeof(pContext->pcmPlayerBufMap[0]));
	LOG_DEBUG("heroTtsPcmStreamPlayCallback currentIndex = %d, %d", currentIndex, pContext->pcmPlayerBufMap[currentIndex]);
	if(pContext->pcmPlayerBufMap[currentIndex] == NULL){
		return AUDIO_STREAM_GET_SRC_ERROR;
	}
    pContext->currentPlayerIndex = currentIndex + 1;

    ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = TIHO_TTS_PCM_PLAYER_BUF_SIZE;
    ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr = pContext->pcmPlayerBufMap[currentIndex];

	usedDataLen = heroTtsCycleBuffeGetUsedNum(pContext);
	usedDataLen -= usedDataLen%2;
	LOG_DEBUG("heroTtsPcmStreamPlayCallback usedDataLen = %d", usedDataLen);

	ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = MIN(TIHO_TTS_PCM_PLAYER_BUF_SIZE, usedDataLen);
	heroTtsCycleBufferGet(pContext, (char*)(ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr), (uint32 *)&(ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength));

	LOG_DEBUG("heroTtsPcmStreamPlayCallback heroTtsCycleBufferGet = %d", ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength);
	if(0 < ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength) {
		return AUDIO_STREAM_GET_SRC_SUCCESS;
	}

	return AUDIO_STREAM_GET_SRC_ERROR;
}

static MMISRV_HANDLE_T heroTtsCreatePcmStreamHandle(int ttsisCall)
{
	MMISRVMGR_SERVICE_REQ_T req = {0};
	MMISRVAUD_TYPE_T audio_srv = {0};

	req.notify = heroTtsHandleNotify;
	req.pri = ttsisCall ? MMISRVAUD_PRI_HIGH : MMISRVAUD_PRI_NORMAL;
	audio_srv.volume  = TTS_GetTTSVolume();
	audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_NONE;
	audio_srv.duation = 0;
	audio_srv.eq_mode = 0;
	audio_srv.is_mixing_enable = FALSE;
	audio_srv.play_times = 1;
	audio_srv.info.streaming.type = MMISRVAUD_TYPE_STREAMING;
	audio_srv.info.streaming.data = (uint8 *)(&heroTtsHeadInfo);
	audio_srv.info.streaming.data_len = sizeof(heroTtsHeadInfo);
	audio_srv.info.streaming.fmt = MMISRVAUD_RING_FMT_WAVE;
	audio_srv.info.streaming.cb = (void *)heroTtsPcmStreamPlayCallback;
	audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;

	LOG_DEBUG("heroTtsCreatePcmStreamHandle req ... ttsisCall = %d", ttsisCall);
	return MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
}

typedef struct tagHeroTtsSigSt {
	SIGNAL_VARS
} HeroTtsSigSt;
MMI_RESULT_E handle_hero_tts_pp_msg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
	HeroTtsContextSt *pContext = hero_tts_get_context();

	LOG_DEBUG("handle_hero_tts_pp_msg msg_id = %d, HERO_HERO_TTS_MOUDLE_SIGNAL_BEG(%d)", msg_id, hero_tts_convert_moudle_id(HERO_TTS_MOUDLE_ID_SIGNAL_BEG));
	if(msg_id >= hero_tts_convert_moudle_id(HERO_TTS_MOUDLE_ID_SIGNAL_BEG) && msg_id <= hero_tts_convert_moudle_id(HERO_TTS_MOUDLE_ID_SIGNAL_END))
	{
		msg_id = msg_id - hero_tts_convert_moudle_id(HERO_TTS_MOUDLE_ID_SIGNAL_BEG);
		LOG_DEBUG("handle_hero_tts_pp_msg msgid = %d", msg_id);
		switch(msg_id)
		{
			case HERO_TTS_MOUDLE_ID_START_PCM:
			{
				if(pContext->audioHandle != 0) {
					MMISRVAUD_Stop(pContext->audioHandle);
					MMISRVMGR_Free(pContext->audioHandle);
					pContext->audioHandle = 0;
				}

				LOG_DEBUG("handle_hero_tts_pp_msg pContext->taskIsStop = %d", pContext->taskIsStop);
				if(!pContext->taskIsStop)
				{
					pContext->audioHandle = heroTtsCreatePcmStreamHandle(pContext->ttsisCall);
					LOG_DEBUG("handle_hero_tts_pp_msg start pcm = %d", pContext->audioHandle);
					if(pContext->audioHandle != 0)
					{
						if(MMISRVAUD_Play(pContext->audioHandle, 0) != MMISRVAUD_RET_OK)
						{
							MMISRVMGR_Free(pContext->audioHandle);
							pContext->audioHandle = 0;

							if(pContext->callback != NULL) {
								pContext->callback(0, pContext->userdata);
							}
							hero_tts_stop_play();
						}
					}
					else
					{
						if(pContext->callback != NULL) {
							pContext->callback(0, pContext->userdata);
						}
						hero_tts_stop_play();
					}
				}
			}
			break;
			case HERO_TTS_MOUDLE_ID_STOP_PCM:
			{
				if(pContext->audioHandle != 0)
				{
					MMISRVAUD_Stop(pContext->audioHandle);
					MMISRVMGR_Free(pContext->audioHandle);
					pContext->audioHandle = 0;
				}
			}
			break;
			case HERO_TTS_MOUDLE_ID_CALLBACK:
				if(pContext->callback != NULL) {
					pContext->callback(1, pContext->userdata);
				}
				hero_tts_stop_play();
			break;
		}
		return MMI_RESULT_TRUE;
	}

	return MMI_RESULT_FALSE;
}

static int hero_tts_send_msg_to_ui(int msgId)
{
	HeroTtsSigSt *psig = PNULL;

	LOG_DEBUG("hero_tts_send_msg_to_ui msgid = %d", msgId);
	MMI_CREATE_SIGNAL((xSignalHeaderRec*)psig, msgId, sizeof(HeroTtsSigSt), SCI_IdentifyThread());
	MMI_SEND_SIGNAL((xSignalHeaderRec*)psig, P_APP);
	return 1;
}

int hero_tts_stop_play(void)
{
	HeroTtsContextSt *pContext = hero_tts_get_context();

	LOG_DEBUG("hero_tts_stop_play ttsisrunning = %d", pContext->ttsisrunning);
	if(pContext->ttsisrunning)
	{
		int index = 0; uint32 sRet = 0;

		pContext->taskIsStop = 1;
		tiho_tts_stop();

		if(pContext->audioHandle != 0) {
			MMISRVAUD_Stop(pContext->audioHandle);
			MMISRVMGR_Free(pContext->audioHandle);
			pContext->audioHandle = 0;
		}

		if(pContext->ttsTimerId != 0){
			MMK_StopTimer(pContext->ttsTimerId);
			pContext->ttsTimerId = 0;
		}

		pContext->callback = NULL;
		pContext->userdata = NULL;

		pContext->ttsisrunning = 0;
		pContext->ttsstatus = TIHO_TTS_PLAYER_IDLE;

		if(pContext->heroTtsRunThreadId != SCI_INVALID_BLOCK_ID)
		{
			while (1)
			{
				void *sig_ptr = (void*)SCI_PeekSignal(pContext->heroTtsRunThreadId);
				LOG_DEBUG("hero_tts_stop_play sig_ptr = %d", sig_ptr);
				if (sig_ptr != PNULL){
					SCI_FREE(sig_ptr);
				}else{
					break;
				}
			}
		}

		LOG_DEBUG("hero_tts_stop_play send stop msg ...");
		if(!hero_tts_send_msg_to_task(pContext, MSG_ID_TTS_TASK_STOP)) {
			return 0;
		}
		sRet = SCI_GetSemaphore(pContext->ttsStopSemaphore, SCI_WAIT_FOREVER); //wating stop
		SCI_ASSERT(sRet == SCI_SUCCESS);
		LOG_DEBUG("hero_tts_stop_play wating end ...");

		//free memory
		if(pContext->heroTtsCycleBuffer != NULL) {
			hero_tts_free(pContext->heroTtsCycleBuffer);
			pContext->heroTtsCycleBuffer = NULL;
		}
		pContext->heroTtsPutPosition = 0;
		pContext->heroTtsGetPosition = 0;
		pContext->heroTtsCycleBufferLen = 0;

		pContext->currentPlayerIndex = 0;
		for(index = 0; index < sizeof(pContext->pcmPlayerBufMap)/sizeof(pContext->pcmPlayerBufMap[0]); index++) {
			if(pContext->pcmPlayerBufMap[index] != NULL) {
				hero_tts_free(pContext->pcmPlayerBufMap[index]);
				pContext->pcmPlayerBufMap[index] = NULL;
			}
		}
		gAppendTxtSize = 0;

		LOG_DEBUG("hero_tts_stop_play end pContext->audioHandle = %d", pContext->audioHandle);
		return 1;
	}
	return 1;
}

static void hero_tts_start_play_callback(uint8 timer_id, uint32 param)
{
	HeroTtsContextSt *pContext = (HeroTtsContextSt*)param;

	LOG_DEBUG("hero_tts_start_play pContext = %d", pContext);
	if(pContext != NULL)
	{
		if(pContext->ttsTimerId != 0)
		{
			MMK_StopTimer(pContext->ttsTimerId);
			pContext->ttsTimerId = 0;
		}

		if(!hero_tts_start_task(pContext->stringbuf, pContext->stringsize, pContext->callback, pContext->userdata)) {
			hero_tts_send_msg_to_ui(hero_tts_convert_moudle_id(HERO_TTS_MOUDLE_ID_CALLBACK));
		}
	}
}

int hero_tts_start_play(char *string, int stringsize, hero_tts_play_callback callback, void *userdata, int ttsisCall)
{
	HeroTtsContextSt *pContext = hero_tts_get_context();

	LOG_DEBUG("hero_tts_start_play begin...");
	if(!hero_tts_stop_play()) {
		return 0;
	}

	pContext->ttsisCall = ttsisCall;
	pContext->stringsize = MIN(stringsize, TIHO_TTS_STRING_BUF_SIZE - 2);
	if((unsigned char*)string != pContext->stringbuf)
	{
		memset(pContext->stringbuf, 0, TIHO_TTS_STRING_BUF_SIZE);
		memcpy(pContext->stringbuf, string, pContext->stringsize);
	}
	pContext->userdata = userdata;
	pContext->callback = callback;

	if(pContext->ttsTimerId != 0)
	{
		MMK_StopTimer(pContext->ttsTimerId);
	}
#if 1
	pContext->ttsTimerId = 0;
	hero_tts_start_play_callback(0, (uint32)pContext);
	return 1;
#else
	pContext->ttsTimerId = MMK_CreateTimerCallback(50, hero_tts_start_play_callback, (int32)pContext, FALSE);
	return pContext->ttsTimerId != 0;
#endif
}

char *hero_tts_get_string_buf(int *bufsize)
{
	HeroTtsContextSt *pContext = hero_tts_get_context();
	if(bufsize != NULL){
		*bufsize = TIHO_TTS_STRING_BUF_SIZE;
	}
	return pContext->stringbuf;
}

int hero_tts_is_play_ring(void)
{
	HeroTtsContextSt *pContext = hero_tts_get_context();
	return (pContext->ttsisrunning) || (pContext->ttsTimerId != 0); ///(pContext->ttsisrunning && (TIHO_TTS_PLAYER_PLAYING == pContext->ttsstatus));
}

void hero_tts_volume_set(int volume)
{
	HeroTtsContextSt *pContext = hero_tts_get_context();
	if(pContext->audioHandle != 0)
	{
        MMISRVAUD_SetVolume(pContext->audioHandle, volume);
	}
}

void hero_tts_append_txt(char *data, int size)
{
	HeroTtsContextSt *pContext = hero_tts_get_context();
	if(data == NULL || size == 0 || pContext->taskIsStop){
		return;
	}
	memset(gAppendTxtBuf, 0, sizeof(gAppendTxtBuf));
	gAppendTxtSize = MIN(sizeof(gAppendTxtBuf) - 2, size);
	memcpy(gAppendTxtBuf, data, gAppendTxtSize);
}

