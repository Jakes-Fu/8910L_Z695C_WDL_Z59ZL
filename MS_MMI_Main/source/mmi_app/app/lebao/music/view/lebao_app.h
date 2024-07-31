#ifndef __LEBAO_APP_H
#define __LEBAO_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sci_types.h"
#include "mn_type.h"

typedef int(*LEBAO_COMMON_CALLBACK_T)(const int cmd, const int status, void *data);

typedef enum
{
	LEBAO_STATUS_STOP = 0,
	LEBAO_STATUS_PLAY,
	LEBAO_STATUS_PAUSE,
	LEBAO_STATUS_RESUME,
	LEBAO_STATUS_END,
	LEBAO_STATUS_ERROR,
	LEBAO_STATUS_FAILED,
	LEBAO_STATUS_REPLAY,
	LEBAO_STATUS_GUEST_LIMIT,

} lebao_play_status_t;

typedef enum
{
	LEBAO_STATUS_RECORD_STOP = 0,
	LEBAO_STATUS_RECORD_START,
	LEBAO_STATUS_RECORD_POST_START,
	LEBAO_STATUS_RECORD_POST_FAILED,
	LEBAO_STATUS_RECORD_ERROR,
	LEBAO_STATUS_RECORD_HAS_RESULT,
	LEBAO_STATUS_RECORD_LIMITED,
	LEBAO_STATUS_RECORD_GUEST_LIMITED,
	LEBAO_STATUS_RECORD_NO_RESULT,
} lebao_record_status_t;

// lebao_app.c
void StartLebaoApp(void);
void StopLebaoApp(void);

MN_DUAL_SYS_E lebao_get_active_sim(void);
int lebao_get_net_id(void);
int lebao_send_sms(const MN_DUAL_SYS_E dualSysUse, const char* called, const char* content);
int lebao_make_call(const MN_DUAL_SYS_E dualSysUse, const char* called);

BOOLEAN lebao_gprs_is_opened(void);
BOOLEAN lebao_active_network(void);
BOOLEAN lebao_deactive_network(void);
BOOLEAN lebao_network_is_connected(void);

// lebao_wintable.c
void MMIAPIMENU_EnterLebao(void);
void MMIAPIMENU_QuitLebao(void);

uint32	lebao_create_win(const int pageId);
void	lebao_close_win(const int pageId);
BOOLEAN	lebao_is_win_opened(const int pageId);
int lebao_set_win_softkey(const int page_id, const int sub_id, const uint32 win_id);
void lebao_player_update_playstatus_icon(const uint32 win_id);

// lebao_win_player.c
extern int lebao_playing_audio_dev_detect;
extern uint32 lebao_player_btn_prev_event_cb(void);
extern uint32 lebao_player_btn_play_event_cb(void);
extern uint32 lebao_player_btn_next_event_cb(void);
extern void lebao_open_bt_device(void);
extern void lebao_close_bt_device(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
