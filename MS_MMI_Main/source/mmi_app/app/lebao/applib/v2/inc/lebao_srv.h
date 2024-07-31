#ifndef __LEBAO_SRV_H
#define __LEBAO_SRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lebao_api.h"

typedef struct
{
	sds filepath;
	sds keyword;
	link_list_t* songs;
} lebao_srv_voice_result_t;

typedef struct
{
	int type;
	int page;
	int count;
	sds param;
	link_list_t* stream;
} lebao_srv_voice_request_t;

typedef enum
{
	SRV_LEBAO_FOR_PLAY_FILE,
	SRV_LEBAO_FOR_PLAY_BUFFER,
	SRV_LEBAO_FOR_RING,
	SRV_LEBAO_FOR_IMAGE,
	SRV_LEBAO_FOR_PLAY_STREAM,
} lebao_srv_operation_type_t;

typedef struct
{
	// request
	int index;
	int oprType;  // lebao_srv_operation_type_t
	sds id;
	sds uri;
	sds savepath; // only for download file

	int rangStart;
	int rangEnd;

	// response
	int status;
	sds data;
	int totalSize;

	// internal
	int id3Length;
	int skipLength;
	int srcPageId;
	int connectTimeout;
} lebao_srv_download_request_t;

typedef struct
{
	int status;
	sds orderId;
	sds type;
	sds captcha;
} lebao_srv_onekey_result_t;

typedef struct
{
	int		curChartIndex;
	int		curMusicIndex;
	sds		chartId;	
	sds		chartName;
	sds		songId;
	sds		songName;
	sds		artist;
	sds		preListenUrl;
	void*   data;
	int		srcPage;
	int		songType;
	sds		playStartTime;
	unsigned int playStartTimeSeconds;
	unsigned char isMusicType;
} lebao_wnd_data_t;

typedef int(*LEBAO_THREAD_PROC)(void * param);
extern int 	   g_lebaoUpdatingBusyCount;

void	lebao_free_download_request(lebao_srv_download_request_t* request);
void	lebao_free_voice_result(lebao_srv_voice_result_t* result);
void	lebao_free_voice_request(lebao_srv_voice_request_t* request);
void	lebao_free_onekey_result(lebao_srv_onekey_result_t* request);
void	lebao_free_wnd_data(lebao_wnd_data_t* data);

int		lebao_srv_post_update_thread(LEBAO_THREAD_PROC proc, void * param);
int		lebao_srv_post_download_thread(LEBAO_THREAD_PROC proc, void * param);

int 	lebao_srv_init(void);
int 	lebao_srv_destroy(void);
void	lebao_srv_quit_download(const int tryWaiting);
int		lebao_srv_is_updating(void);

int 	lebao_srv_is_vip(void);
int 	lebao_srv_check_vip(void);
sds		lebao_srv_get_order_uri(void);

int 	lebao_srv_run_once(void);
int 	lebao_srv_get_chart_data(void);
int 	lebao_srv_download_mp3(lebao_srv_download_request_t * request);
int 	lebao_srv_download_mp3_buffer(lebao_srv_download_request_t * request);
int 	lebao_srv_save_image_from_net(lebao_srv_download_request_t * request);
int		lebao_srv_get_asr_type(void);
int 	lebao_srv_post_voice_search(const int type, const char * filename);
int 	lebao_srv_post_voice_stream(const int type, link_list_t * stream);

int		lebao_srv_get_mp3_type(void);
sds		lebao_srv_get_mp3_uri(const char * musicId, int *songType);

int		lebao_srv_get_ivr_info(void);

int 	lebao_srv_caixin_login_send_sms(void);
int		lebao_srv_caixin_get_music_playurl_from_net(lebao_srv_download_request_t * request);
int		lebao_srv_caixin_get_qr_lic_url(sds info);
int lebao_ctrl_net_favorite_action(const int pageId, int add, const char *songId);
int	lebao_srv_caixin_login_check_state(int login_mode);

void lebao_vip_limited_open(int action, song_info_t* song, sds *id);
void lebao_behavior_record_start(int action, const char *channelNo);
int	lebao_srv_caixin_post_behavior(sds body);

#ifdef __cplusplus
}
#endif

#endif
