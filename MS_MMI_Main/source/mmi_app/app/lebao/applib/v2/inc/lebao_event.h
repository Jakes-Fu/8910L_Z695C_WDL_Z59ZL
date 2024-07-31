#ifndef __LEBAO_EVENT_H
#define __LEBAO_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sds.h"

typedef enum
{
	LEBAO_PAGE_SRV = 0,
	LEBAO_PAGE_CTRL,
	LEBAO_PAGE_SPLASH,
	LEBAO_PAGE_MENU,
	LEBAO_PAGE_CHART,
	LEBAO_PAGE_MUSICLIST,
	LEBAO_PAGE_BOOKLIST,
	LEBAO_PAGE_PLAYER,
	LEBAO_PAGE_VOLUME,
	LEBAO_PAGE_LOCAL,
	LEBAO_PAGE_SEARCH,
	LEBAO_PAGE_RECORD,
	LEBAO_PAGE_WAITING,
	LEBAO_PAGE_ORDER,
	LEBAO_PAGE_STREAM,
    LEBAO_PAGE_ABOUT,
	LEBAO_PAGE_CONFIRM,
	LEBAO_PAGE_OPTION,
	LEBAO_PAGE_VIP,
	LEBAO_PAGE_HISTORY,
	LEBAO_PAGE_LOGIN,
	LEBAO_PAGE_MAX
} lebao_page_id_t;

typedef enum
{
	LEBAO_SUB_PAGE_NULL = 0,
	
	LEBAO_SUB_PAGE_PLAYER_OPTION,
	
	LEBAO_SUB_PAGE_WAITING_RETURN,
	LEBAO_SUB_PAGE_WAITING_NO_RETURN,
	
	LEBAO_SUB_PAGE_ORDER_SHOW_MID_KEY,
	LEBAO_SUB_PAGE_ORDER_NO_MID_KEY,
	LEBAO_SUB_PAGE_ORDER_OPTION,
	
	LEBAO_SUB_PAGE_VIP_SHOW_LEFT_KEY,
	LEBAO_SUB_PAGE_VIP_NO_LEFT_KEY,

	LEBAO_SUB_PAGE_CONFIRM_ONE_RETRUN,
	LEBAO_SUB_PAGE_CONFIRM_OK_RETRUN,
	LEBAO_SUB_PAGE_CONFIRM_AGRESS_RETRUN,
    LEBAO_SUB_PAGE_CONFIRM_AGRESS_GUESTLOGIN,

	LEBAO_SUB_PAGE_LOCAL_NO_SONG,
	LEBAO_SUB_PAGE_LOCAL_EXIST_SONG,
} lebao_sub_page_id_t;

typedef enum
{
	EVT_SRV_LEBAO_BEGIN = 0,
	EVT_SRV_LEBAO_VOICE_RESULT,
	EVT_SRV_LEBAO_DOWNLOAD_RESULT,
	EVT_SRV_LEBAO_SAVE_IMAGE_RESULT,
	EVT_SRV_LEBAO_RUNONCE_RESULT,
	EVT_SRV_LEBAO_ORDER_RESULT,
	EVT_SRV_LEBAO_IVR_INFO,
	EVT_SRV_LEBAO_TIPS_DATA,
	EVT_SRV_LEBAO_ORDER_CHECK_RESULT,
	EVT_SRV_LEBAO_POST_CAPTCHA_RESULT,
	EVT_SRV_LEBAO_POST_CHART_SUB_CONTENT,

	EVT_CTRL_LEBAO_QUIT = 100,
	EVT_CTRL_LEBAO_CREATE_PAGE,
	EVT_CTRL_LEBAO_CLOSE_PAGE,
	EVT_CTRL_LEBAO_SET_RING,
	
	EVT_CTRL_LEBAO_PLAY_MUSIC,
	EVT_CTRL_LEBAO_PAUSE_MUSIC,
	EVT_CTRL_LEBAO_RESUME_MUSIC,
	EVT_CTRL_LEBAO_STOP_MUSIC,

	EVT_CTRL_LEBAO_START_RECORD,
	EVT_CTRL_LEBAO_STOP_RECORD,
	EVT_CTRL_LEBAO_POST_RECORD,

	EVT_CTRL_LEBAO_ADD_FAVORITE,
	EVT_CTRL_LEBAO_DEL_FAVORITE,

	EVT_CTRL_LEBAO_LOGOUT = 200,
	EVT_LEBAO_PLAY_STATUS,
	EVT_LEBAO_RECORD_STATUS,
	EVT_LEBAO_URI_UPDATE,
	EVT_LEBAO_RING_RESULT,
	EVT_LEBAO_TIMER_ONE_SECOND,
	EVT_LEBAO_LOGINQR_UPDATE,
	EVT_LEBAO_RELOGIN,
	
} lebao_evt_id_t;

typedef enum
{
	LEBAO_MAINMENU_ENTER = 1,
	LEBAO_VIP_HOTLINE_CLICK = 2,
	LEBAO_VIP_OPTIOM_CALL_CLICK = 3,
	LEBAO_PLAY_RING_SELECT = 4,
	LEBAO_PLAY_VIP_SING_TRIG = 5,
	LEBAO_SEARCH_VIP_SING_TRIG = 6,
	LEBAO_GUEST_TRIG = 7,
	LEBAO_PLAY_ALARM_SELECT = 8,
} lebao_vip_page_result_t;

typedef void(*LEBAO_EVENT_PROC)(const int eventId, const int cmd, void* data);
typedef void(*LEBAO_EVENT_FREE)(void* data);


typedef struct
{
	lebao_evt_id_t evtId;
	LEBAO_EVENT_PROC callback;
} lebao_evt_callback_t;

typedef struct
{
	int eventId;
	int pageId;
	int cmd;
	void* data;
	int priority;
	LEBAO_EVENT_FREE free_cb;
} lebao_evt_t;

extern const char *lebao_mainmenu_title[LEBAO_PAGE_MAX];

void lebao_event_init(void);
void lebao_event_close(void);
void lebao_event_task_run(void * data);

int lebao_event_send(const int eventId, const int pageId, const int cmd, void* data);
int lebao_event_send_priority(const int eventId, const int pageId, const int cmd, void* data, int priority);
void lebao_event_add_listen(int eventId, int pageId, LEBAO_EVENT_PROC callback);
void lebao_event_remove_listen(int eventId, int pageId);
void lebao_event_add_view(const lebao_evt_callback_t callback[], const int pageId);
void lebao_event_remove_view(const int pageId);

#ifdef __cplusplus
}
#endif

#endif /*__LEBAO_EVENT_H*/
