#include "window_parse.h"
#include "std_header.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guimsgbox.h"
#include "guicommon.h"
#include "guitext.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "guimenu.h"
#include "guiedit.h"
#include "guilistbox.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "mmisrvaud_api.h"
#include "mmiset_export.h"
#include "mmipub.h"
#include "ffs.h"
#include "ffs_error.h"
#include "mn_type.h"
#include "Mn_api.h"
#include "mmisms_app.h"
#include "mmisms_export.h"
#include "mmiidle_export.h"
#include "mmidc_export.h"
#include "block_mem.h"
#include "sci_log.h"
#include "arm_reg.h"
#include "tb_hal.h"
#include "aud_gen.h"
#include "mmi_default.h"
#include "Mmiphone_export.h"
#include "Mmipb_export.h"
#include "mmimp3_export.h"
#include "heroEngineInternal.h"
#include "heroEngineId.h"
#include "heroEngineExport.h"
#include "freq_cfg.h"
#include "mmicc_internal.h"
#include "mmicc_text.h"
#include "sig_code.h"
#include "Mmi_filemgr.h"
#include "mmi_nv.h"
#include "mmi_id.h"
#include "mmisms_send.h"
#include "mmisms_edit.h"
#include "stdarg.h"
#include "Mmisd_export.h"
#include "mmi_common.h"
#include "Mmienvset_internal.h"
#include "Guiimg.h"
//#include "Guianim_internal.h"
#include "dal_lcd.h"
#include "mmiudisk_export.h"
#include "version.h"
#ifdef __HR_RECORD_SUPPORT__
#include "mmirecord_export.h"
#endif
#include "mmiset_id.h"
#ifndef WIN32
#include "iram_mgr.h"
#endif
#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmifmm_export.h"
#include "ctrlanim.h"

#include "mmienvset_export.h" //MMIENVSET_INSIDE_MODE
#include "mmiidle_func.h"

#ifdef FM_SUPPORT
#include "mmifm_export.h"
#endif

#ifndef WIN32
typedef unsigned short		unshort;
typedef unsigned short		u_short;
#include "app_tcp_if.h"
#else
#include "sys\stat.h"
#include "stdio.h"
#endif
#include "mmiconnection_export.h"
#include "mmiset_id.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "wav_adp.h"
#include "apm_express.h"

#include "mmimms_edit.h"
#include "mmimms_internal.h"
#include "pe_process.h"
#include "mmidc_setting.h"
#ifdef __HR_CFG_MULTITP_SUPPORT__
#include "mmk_type.h"
#include "mmk_tp.h"
#endif
#ifdef __HR_CFG_SHORTCUT_SUPPORT__
#include "guianim_internal.h"
#endif
//modify 20181120 begin
#include "mmiacc_event.h"

#ifdef HERO_ENGINE_TTS_SUPPORT
#include "hero_tts_api.h"
#endif
//modify 20181120 end

#include "socket_api.h"
#include "mmi_wallpaper_export.h"
#include "img_dec_interface.h"
#include "mmi_menutable.h"
#ifdef __HR_RECORD_SUPPORT__
#include "mmisrvrecord_export.h"
#include "mmirecord_appcontrol.h"
#endif
#include "mmismsapp_interface.h"
#include "mmicl_internal.h"
#include "mmicl_export.h"
#include "mmidc_main.h"
#if defined BROWSER_SUPPORT
#include "mmibrowser_setting.h"
#endif
#include "heroEngineHostName.h"
#include "IN_message.h"

#define HERO_ASYN_HOST_NAME_FLAG      0x1

typedef struct tagHeroAsynHostNameContextSt{
	uint32 taskRef;
	volatile unsigned char hoststart;
}HeroAsynHostNameContextSt;

typedef struct tagHeroAsynHostNameTaskMsgSt{
	SIGNAL_VARS
	char name[128];
	void *userdata; 
	asyn_host_name_callback callback;
	asyn_host_name_get_net_id netidcallback;	
}HeroAsynHostNameTaskMsgSt;

int MMIHEROAPP_SendMsgToUi(int msgId, void *parm1, void *parm2, void *parm3, void *parm4);

static HeroAsynHostNameContextSt *hero_asyn_host_name_get_context()
{
	static unsigned char isInit = 0;
	static HeroAsynHostNameContextSt sHeroAsynHostNameContext = {0};
	if(!isInit) {
		isInit = 1;
		memset(&sHeroAsynHostNameContext, 0, sizeof(HeroAsynHostNameContextSt));
		sHeroAsynHostNameContext.taskRef = SCI_INVALID_BLOCK_ID;
	}
	return &sHeroAsynHostNameContext;
}

void hero_aysn_host_ui_modle_callback(void *parm1, void *parm2, void *parm3, void *parm4)
{
	asyn_host_name_callback callback = (asyn_host_name_callback*)parm2;

	MMIHR_Trace(("hero_aysn_host_ui_modle_callback %d, callback = 0x%x", (int)parm1, callback));
	if(callback != NULL) {
		callback((int)parm1, parm3);
	}
}

static void hero_asyn_host_by_name_thread_callback(uint32 param, void* obj_ptr)
{
	HeroAsynHostNameContextSt *pContext = (HeroAsynHostNameContextSt*)obj_ptr;
	if(pContext != NULL)
	{
		HeroAsynHostNameTaskMsgSt signal = {0};
		HeroAsynHostNameTaskMsgSt *sig_ptr = NULL;

		while(1)
		{
			sig_ptr = (HeroAsynHostNameTaskMsgSt*)SCI_GetSignal(pContext->taskRef);
			signal = *sig_ptr;
			SCI_FREE(sig_ptr);

			MMIHR_Trace(("hero_asyn_host_by_name_thread_callback actual_flags = %d", signal.SignalCode));
			if(signal.SignalCode == HERO_ASYN_HOST_NAME_FLAG)
			{
				int resValue = HR_FAILED;
				do
				{
					int netid = 0;
					int32 hostAddr = 0;
					struct sci_hostent *hostent = NULL;

					netid = signal.netidcallback();
					if(netid == 0){
						break;
					}

					hostent = sci_gethostbyname((char*)signal.name);
					if(hostent == NULL){
						break;
					}

					SCI_MEMCPY(&hostAddr, hostent->h_addr_list[0], 4);
					resValue = ntohl(hostAddr);
				}while(0);
				MMIHR_Trace(("hero_asyn_host_by_name_thread_callback name = %s, resValue = %d", signal.name, resValue));

				pContext->hoststart = 0;
				MMIHEROAPP_SendMsgToUi(HERO_SIGNAL_ASYN_HOSTNAME, (void*)resValue, (void*)signal.callback, (void*)signal.userdata, NULL);
			}
		}
	}
}

int hero_asyn_host_by_name(char *name, asyn_host_name_callback callback, void *userdata, asyn_host_name_get_net_id netidcallback)
{
	int ret = 0;
	do
	{
		HeroAsynHostNameTaskMsgSt *sig_ptr = NULL;
		HeroAsynHostNameContextSt *pContext = hero_asyn_host_name_get_context();

		if(name == NULL || callback == NULL || netidcallback == NULL){
			break;
		}

		if(SCI_INVALID_BLOCK_ID == pContext->taskRef)
		{
			pContext->taskRef = SCI_CreateAppThread("hrhost", "hrhost",
							hero_asyn_host_by_name_thread_callback,
							0,
							(void*)pContext,
							4 * 1024,
							1,
							250,
							SCI_PREEMPT,
							SCI_AUTO_START);
			if(pContext->taskRef == NULL)
			{
				break;
			}
		}

		if(SCI_QUEUE_FULL == SCI_IsThreadQueueAvilable(pContext->taskRef)) {
			break;
		}

		MMIHR_Trace(("hero_asyn_host_by_name = %d", pContext->hoststart));
		//if(pContext->hoststart){
		//	break;
		//}

		sig_ptr = (HeroAsynHostNameTaskMsgSt *)SCI_ALLOC_APP(sizeof(HeroAsynHostNameTaskMsgSt));
		SCI_MEMSET(sig_ptr, 0, sizeof(HeroAsynHostNameTaskMsgSt));

		sig_ptr->SignalCode = HERO_ASYN_HOST_NAME_FLAG;
		sig_ptr->SignalSize = sizeof(HeroAsynHostNameTaskMsgSt);
		sig_ptr->Sender = SCI_IdentifyThread();

		sig_ptr->userdata = userdata;
		sig_ptr->callback = callback;
		sig_ptr->netidcallback = netidcallback;
		strncpy(sig_ptr->name, name, sizeof(sig_ptr->name) - 1);

		pContext->hoststart = 1;
		if (SCI_SUCCESS != SCI_SendSignal((xSignalHeader)sig_ptr, pContext->taskRef)) {
			pContext->hoststart = 0;
			SCI_FREE(sig_ptr);
			break;
		}

		ret = 1;
	}while(0);
	return ret;
}

